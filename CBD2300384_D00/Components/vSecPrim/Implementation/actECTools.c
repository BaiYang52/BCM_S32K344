/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2016 cv cryptovision GmbH.                                                All rights reserved.
 *
 *  For modifications by Vector Informatik GmbH:
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is protected under intellectual property laws and proprietary to cv cryptovision GmbH
 *                and/or Vector Informatik GmbH.
 *                No right or license is granted save as expressly set out in the applicable license conditions.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  actECTools.c
 *        \brief  ECC domain parameter initialization
 *
 *      \details This file is part of the embedded systems library cvActLib
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to module's header file.
 *********************************************************************************************************************/

#define ACTECTOOLS_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actITypes.h"
#include "actECTools.h"
#include "actUtilities.h"
#include "actBigNum.h"
#include "actECPoint.h"
#include "actECLengthInfo.h"

#if (VSECPRIM_ACTECTOOLS_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define actTAG_SEQUENCE                                              (0x30u)
# define actTAG_INTEGER                                               (0x02u)
# define actTAG_OCTETSTRING                                           (0x04u)

# define actASN1_MIN_VERSION_LEN                                      (0x03u)
# define actASN1_MIN_SEQ_LEN                                          (0x02u)

/* tags indication the encoding / representation of an EC point:
 *  compressed encoding: only the x-coordinate is stored in the ASN1 struct.
 *                       The y-coordinate can be computed using the Weierstrass equation, which allows for a positive
 *                       and a negative solution. The tag encodes which solution shall be used for the y-coordinate.
 *  uncompressed encoding: both the x- and the y-coordinate of the point are stored in the ASN1 struct. */
# define actBASEPOINTENCODING_COMPRESSED_POSITIVE                     (0x02u)
# define actBASEPOINTENCODING_COMPRESSED_NEGATIVE                     (0x03u)
# define actBASEPOINTENCODING_UNCOMPRESSED                            (0x04u)
# define actBASEPOINTENCODING_UNCOMPRESSED_LEADING06                  (0x06u)
# define actBASEPOINTENCODING_UNCOMPRESSED_LEADING07                  (0x07u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 actParseTL()
 **********************************************************************************************************************/
/*! \brief      TLV object macro to parse Tag-Length
 *  \details    Given the start of a data stream in an ASN1 struct, this function returns the start index of the value
 *              and stores the parsed length in value_length.
 *              The parsed length is only written to value_length, if value_length is not a null pointer.
 *  \param[in]     input        data to be parsed
 *  \param[in,out] value_length (in)  buffer to store the length value or a null pointer if length value is not
 *                                    of interest
 *                              (out) length value if value_length is not a null pointer
 *  \return     number of bytes initializing the sequence ( = start index of next data entry)
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        input must be a valid pointer
 *              input must be in valid ASN1 format of Tag|Length|Value
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actLengthType) actParseTL(
  VSECPRIM_P2ROMCONST_PARA(actU8) input,
  VSECPRIM_P2VAR_PARA(actLengthType) value_length);

/**********************************************************************************************************************
 actParseTLVElement()
 **********************************************************************************************************************/
/*! \brief      TLV object macro to parse a field element octetstring
 *  \details    Given the start of a data stream in an ASN1 struct, this function returns the start index of the next
 *              data entry in the struct and stores the address of the element in addr.
 *              The start address of the element is only written to addr, if addr is not a null pointer.
 *  \param[in]     input           data to be parsed
 *  \param[in]     element_length  length of the element to be parsed in byte
 *  \param[in,out] addr            (in)  buffer to store the address of the first field prime byte or a null pointer
 *                                       if the address is not of interest
 *                                 (out) address of the first field prime byte, if addr is not a null pointer
 *  \return     number of bytes to skip in input in order to get to the next data entry in the ASN1 struct
 *              0, if the expected length of the element to parse, element_length, does not coincide with the
 *              parsed length of the element
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        input must be a valid pointer
 *              input must be in valid ASN1 format of Tag|Length|Value
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actLengthType) actParseTLVElement(
  VSECPRIM_P2ROMCONST_PARA(actU8) input,
  actLengthType element_length,
  VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) addr);

/**********************************************************************************************************************
 actParseTLVFieldPrime()
 **********************************************************************************************************************/
/*! \brief      TLV object macro to parse an integer
 *  \details    Given the start of a data stream in an ASN1 struct, this function returns the start index of the next
 *              data entry in the struct and stores the address of the field prime in addr.
 *              The start address of the field prime is only written to addr, if addr is not a null pointer.
 *  \param[in]     input        data to be parsed
 *  \param[in,out] addr   (in)  buffer to store the address of the first field prime byte or a null pointer if the
 *                              address is not of interest
 *                        (out) address of the first field prime byte, if addr is not a null pointer
 *  \param[in,out] length (in)  buffer to store the byte length of the field prime to parse
 *                        (out) length of the field prime
 *  \return     number of bytes to skip in input in order to get to the next data entry in the ASN1 struct
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        input and length must be valid pointers
 *              input must be in valid ASN1 format of Tag|Length|Value
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actLengthType) actParseTLVFieldPrime(
  VSECPRIM_P2ROMCONST_PARA(actU8) input,
  VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) addr,
  VSECPRIM_P2VAR_PARA(actLengthType) length);

/**********************************************************************************************************************
 actParseTLVPoint()
 **********************************************************************************************************************/
/*! \brief      TLV object macro to parse a point
 *  \details    Given the start of a data stream in an ASN1 struct, this function returns the start index of the next
 *              data entry in the struct and stores a flag for the number of coordinates in the point in tag
 *              The flag is only written to tag, if tag is not a null pointer.
 *  \param[in]     input        data to be parsed
 *  \param[in]     x_y_length   length of a single coordinate of the point
 *  \param[in,out] tag          (in)  buffer to store the tag for the number of coordinates of the point or a null
 *                                    pointer if the tag is not of interest
 *                              (out) tag for the number of coordinates of the point if tag is not a null pointer
 *  \return     number of bytes to skip in input in order to get to the next data entry in the ASN1 struct
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        input must be a valid pointer
 *              input must be in valid ASN1 format of Tag|Length|Value
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actLengthType) actParseTLVPoint(
  VSECPRIM_P2ROMCONST_PARA(actU8) input,
  actLengthType x_y_length,
  VSECPRIM_P2VAR_PARA(actLengthType) tag);

/**********************************************************************************************************************
 actParsePoint()
 **********************************************************************************************************************/
/*! \brief      TLV object macro to parse a [T|L|V] octet string containing a precomputed point
 *  \details    Given the start of a data stream in an ASN1 struct, this function checks whether the point is encoded
 *              correctly in ASN1 format. If the point to be parsed is the first precomputed point in 'input',
 *              then its address is assigned to the Curve member prec_first.
 *  \param[in]     input               data to be parsed, a subarray of the speedUp extension
 *  \param[in,out] Curve               (in)  underlying curve
                                       (out) underlying curve, Curve->prec_first points to start index of first
 *                                           precomputed point in input
 *  \param[in]     pointNumber         number of precomputed point to be parsed
 *  \param[in]     p_bytes             length of one coordinate of the point to be parsed
 *  \param[in]     lengthOfASN1Struct   overall length of the ASN1 struct that contains the precomputed points
 *  \param[in,out] currentIndex        (in)  index in input at which the [T|L|V] octet string containing the current
 *                                           precomputed point starts
                                       (out) index in input at which the next [T|L|V] octet string starts
 *  \return     actEXCEPTION_SPEEDUP_EXT point cannot be parsed correctly
 *              actOK point parsed successfully
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        input, Curve and currentIndex must be valid pointers
 *              input must be in valid ASN1 format of Tag|Length|Value
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParsePoint(
  VSECPRIM_P2ROMCONST_PARA(actU8) input,
  VSECPRIM_P2VAR_PARA(actECCURVE) Curve,
  actLengthType pointNumber,
  actLengthType p_bytes,
  actLengthType lengthOfASN1Struct,
  VSECPRIM_P2VAR_PARA(actLengthType) currentIndex);

/**********************************************************************************************************************
 actECCheckWorkSpaceSize()
 **********************************************************************************************************************/
/*! \brief      Checks whether provided workspace size is big enough
 *  \details    -
 *  \param[in] algo_flag  flag indicating for which EC algorithm the workspace is going to be used
 *  \param[in] p_length   field prime length in byte
 *  \param[in] n_length   base order length in byte
 *  \param[in] wksp_len   length of provided workspace in byte
 *  \return     actEXCEPTION_MEMORY if workspace size is not sufficient
 *              actEXCEPTION_ALGOID algo_flag does not correspond to one of the flags for the EC algorithms
 *                                  defined above
 *              actOK else
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        -
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actECCheckWorkSpaceSize(
  actU16 algo_flag,
  actLengthType p_length,
  actLengthType n_length,
  actLengthType wksp_len);

/**********************************************************************************************************************
 actParseSkipSequenceTag()
 **********************************************************************************************************************/
/*! \brief      TLV object macro to parse sequence tag
 *  \details    Given a buffer domainIn containing an ASN1 struct starting with a sequence tag, this function
 *              skips the tag by storing the start index of the data to follow in startIndexData.
 *  \param[in]     domain            data to be parsed
 *  \param[in,out] tagLength         (in)  buffer to store the length of the tag or a null pointer if tag length is not
 *                                         of interest
 *                                   (out) tag length value if tagLength is not anull pointer
 *  \param[in,out] startIndexData    (in)  buffer to store the start index of the data entry to follow the tag
 *                                   (out) data start index
 *  \param[in]     retValInErrorCase desired return value in case of error
 *  \return     actEXCEPTION_DOMAIN       domain does not start with the sequence tag and function was called while
 *                                        parsing the domain
 *              actEXCEPTION_DOMAIN_EXT   domain does not start with the sequence tag and function was called while
 *                                        parsing the domain extension
 *              actEXCEPTION_SPEEDUP_EXT  domain does not start with the sequence tag and function was called while
 *                                        parsing the speedUp extension
 *              actOK                     else
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        domain and startIndexData must be  valid pointers
 *              domain must be in valid ASN1 format of Tag|Length|Value
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParseSkipSequenceTag(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
  VSECPRIM_P2VAR_PARA(actLengthType) tagLength,
  VSECPRIM_P2VAR_PARA(actLengthType) startIndexData,
  actRETURNCODE retValInErrorCase);

/**********************************************************************************************************************
 actParseFieldConstants()
 **********************************************************************************************************************/
/*! \brief      TLV object macro to parse field->RR or field->m_bar
 *  \details    Given a buffer domainIn containing an ASN1 struct starting at the octet string containing a Montgomery
 *              constant, this function stores the parsed data to field->RR and to field->m_bar. The index of the next
 *              data entry in domainIn will be stored in startIndexNextDataEntry.
 *  \param[in]     domain                   data to be parsed
 *  \param[in,out] field                    (in)  field in whose members the parsed data shall be stored, either
 *                                                Curve->p_field or Curve->n_field
 *                                          (out) Curve->p_field or Curve->n_field, members RR and m_bar store
 *                                                the parsed values
 *  \param[in]     dataLengthRR             length of the Montgomery constant RR in bytes
 *  \param[in,out] startIndexNextDataEntry  (in)  buffer to store the index at which the next data entry in the
 *                                                ASN1 struct will start
 *                                          (out) start index of the next data entry
 *  \return     actEXCEPTION_DOMAIN_EXT domain_in does not start with the tag for octet strings
 *              actOK else
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        domain, field and startIndexNextDataEntry must be  valid pointers
 *              domain must be in valid ASN1 format of [Tag|Length|Value, Tag|Length|Value]
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParseFieldConstants(
  VSECPRIM_P2CONST_PARA(actU8) domain,
  VSECPRIM_P2VAR_PARA(actBNRING) field,
  actLengthType dataLengthRR,
  VSECPRIM_P2VAR_PARA(actLengthType) startIndexNextDataEntry);

/**********************************************************************************************************************
 actParseCurveCoefficient()
 **********************************************************************************************************************/
/*! \brief      TLV object macro to parse curve coefficients a or b (or a_R / b_R in Montgomery form)
 *  \details    Given a buffer domainIn containing an ASN1 struct starting at the octet string containing a curve
 *              parameter, this function stores the addresses of the parsed data either to dataAddr or to curveAddr,
 *              if Curve is not a null pointer. The index of the next data entry in domainIn will be stored in
 *              startIndexNextDataEntry.
 *  \param[in]     domain                   data to be parsed
 *  \param[in]     dataLength               length of Curve->a_R/Curve->b_R/Curve->G_R in bytes (they have the same
 *                                          length)
 *  \param[in,out] dataAddr                 (in)  buffer to store the start address of the curve parameter
 *                                                (only used for a) or a null pointer
 *                                          (out) start address of the curve parameter a, if dataAddr was not a null
 *                                                pointer
 *  \param[in,out] startIndexNextDataEntry  (in)  buffer to store the index at which the next data entry in the
 *                                                ASN1 struct will start
 *                                          (out) start index of the next data entry
 *  \param[in]     retValDomainExtension    flag indication whether the return value shall be actEXCEPTION_DOMAIN or
 *                                          actEXCEPTION_DOMAIN_EXT
 *  \return     actEXCEPTION_DOMAIN domainIn does not start with the tag for octet strings and the function was called
 *                                  from actParseDomain
 *              actEXCEPTION_DOMAIN_EXT domainIn does not start with the tag for octet strings and the function was
 *                                      called from actParseDomainExtension
 *              actOK else
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        domain and startIndexNextDataEntry must be valid pointers (dataAddr can be a null pointer)
 *              domain must be in valid ASN1 format of Tag|Length|Value
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParseCurveCoefficient(
  VSECPRIM_P2CONST_PARA(actU8) domain,
  actLengthType dataLength,
  VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) dataAddr,
  VSECPRIM_P2VAR_PARA(actLengthType) startIndexNextDataEntry,
  boolean retValDomainExtension);

/**********************************************************************************************************************
 actParseCurveParameters()
 **********************************************************************************************************************/
/*! \brief      TLV object macro to parse curve data a, b, and G (or a_R, b_R and G_R in Montgomery form)
 *  \details    Given a buffer domainIn containing an ASN1 struct starting at the octet string containing Curve-> a or
 *              Curve->a_R, this function stores the addresses of the parsed data to addressA, addressB and Curve->G_R.
 *              The index of the next data entry in domainIn will be stored in startIndexNextDataEntry.
 *  \param[in]     domain                   data to be parsed
 *  \param[in,out] Curve                    (in)  Curve in whose members the parsed data shall be stored or a null
 *                                                pointer, if nothing shall be assigned to a curve member
 *                                          (out) Curve, Curve->G_R is set to the address of the parsed data for G_R
 *                                                if Curve is not a null pointer
 *  \param[in]     dataLength               length of a,b,G / A_R, b_R, G_R in bytes (they have the same length)
 *  \param[in,out] addressA                 (in)  buffer to store the address of the first parameter a/a_r in domainIn
 *                                          (out) address of the first parameter a/a_r in domainIn
 *  \param[in,out] addressB                 (in) buffer to store the address of the first parameter b/b_r in domainIn
 *                                          (out) address of the first parameter b/b_r in domainIn
 *  \param[in]      retValDomainExtension   flag indication whether the return value shall be actEXCEPTION_DOMAIN or
 *                                          actEXCEPTION_DOMAIN_EXT
 *  \param[in,out] startIndexNextDataEntry  (in)  buffer to store the index at which the next data entry in the
 *                                                ASN1 struct will start
 *                                          (out) start index of the next data entry
 *  \return     actEXCEPTION_DOMAIN domainIn does not start with the tag for octet strings and the function was called
 *                                  from actParseDomain
 *              actEXCEPTION_DOMAIN_EXT domainIn does not start with the tag for octet strings and the function was
 *                                      called from actParseDomainExtension
 *              actOK else
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        domain and startIndexNextDataEntry must be valid pointers (Curve, addressA and addressB can be
 *              null pointers)
 *              domain must be in valid ASN1 format of Tag|Length|Value
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParseCurveParameters(
  VSECPRIM_P2CONST_PARA(actU8) domain,
  VSECPRIM_P2VAR_PARA(actECCURVE) Curve,
  actLengthType dataLength,
  VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) addressA,
  VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) addressB,
  boolean retValDomainExtension,
  VSECPRIM_P2VAR_PARA(actLengthType) startIndexNextDataEntry);

/**********************************************************************************************************************
 actParseSkipUntilFieldPrime()
 **********************************************************************************************************************/
/*! \brief      TLV object macro to parse ASN1 EC struct until the start address of the first field prime p.
 *  \details    Given a buffer domainIn containing an ASN1 struct from the start, this function skips the first
 *              sequence tag, the version, the second sequence tag and the OID in domainIn. The index of the start
 *              of the field prime p in domainIn will be stored in startIndexData.
 *  \param[in]     domain              data to be parsed
 *  \param[in,out] lengthOfASN1Struct  (in)  buffer to store the total length of the ASN1 struct domain to be parsed
 *                                     (out) length of the ASN1 struct domain
 *  \param[in,out] startIndexData      (in)  buffer to store the index at which the field prime p in the
 *                                           ASN1 struct will start
 *                                     (out) start index of the field prime p
 *  \return     actEXCEPTION_DOMAIN domain_in does not start with the tag for octet strings
 *              actOK else
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        domain, lengthOfASN1Struct and startIndexData must be  valid pointers
 *              domain must be in valid ASN1 format of Tag|Length|Value
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParseSkipUntilFieldPrime(
  VSECPRIM_P2CONST_PARA(actU8) domain,
  VSECPRIM_P2VAR_PARA(actLengthType) lengthOfASN1Struct,
  VSECPRIM_P2VAR_PARA(actLengthType) startIndexData);

/**********************************************************************************************************************
 actParseFieldPrime()
 **********************************************************************************************************************/
/*! \brief      TLV object macro to parse the field primes p or n
 *  \details    Given a buffer domainIn containing an ASN1 struct starting at the octet string containing a field
 *              prime, this function stores the start address and size of the parsed data in primeStartAddr and
 *              primeByteLength. The index of the next data entry in domainIn will be stored in
 *              startIndexNextDataEntry, if this is not a null pointer.
 *  \param[in]     domain                   data to be parsed
 *  \param[in,out] primeByteLength          (in)  buffer to store the length of the field prime in bytes
 *                                          (out) field prime length
 *  \param[in,out] primeStartAddr           (in)  buffer to store the start address of the field prime
 *                                          (out) start address of the field prime
 *  \param[in,out] startIndexNextDataEntry  (in)  buffer to store the index at which the next data entry in the
 *                                                ASN1 struct will start or a null pointer, if the index is not
 *                                                of interest
 *                                          (out) start index of the next data entry, if startIndexNextDataEntry is not
 *                                                a null pointer
 *  \return     actEXCEPTION_DOMAIN domain_in does not start with the tag for octet strings
 *              actOK else
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        domain, primeByteLength and primeStartAddress must be  valid pointers (startIndexNextDataEntry can
 *              be a null pointer)
 *              domain must be in valid ASN1 format of Tag|Length|Value
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParseFieldPrime(
  VSECPRIM_P2CONST_PARA(actU8) domain,
  VSECPRIM_P2VAR_PARA(actLengthType) primeByteLength,
  VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) primeStartAddr,
  VSECPRIM_P2VAR_PARA(actLengthType) startIndexNextDataEntry);

/**********************************************************************************************************************
 actECParseDomainExtension()
 *********************************************************************************************************************/
/*! \brief          Function to parse domain extension TLV object.
 *  \details        -
 *  \param[in]      domainExtension      buffer of domain extension parameters
 *  \param[in,out]  Curve                (in)  underlying curve
 *                                       (out) underlying curve, a_r, b_r and G_R are parsed from domain_ext
 *  \param[in]      p_bytes              length of field prime in bytes
 *  \param[in]      p_length             length of field prime in actBNDIGITs
 *  \param[in]      n_bytes              length of base point order in bytes
 *  \param[in]      n_length             length of base point order in actBNDIGITs
 *  \return         actEXCEPTION_DOMAIN_EXT   domain extension decoding error
 *                  actOK                     else
 *  \pre            domainExtension and Curve must be valid pointers
 *                  domainExtension must be a valid ASN1 structure for EC domain parameters.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParseDomainExtension(
  VSECPRIM_P2ROMCONST_PARA(actU8) domainExtension,
  VSECPRIM_P2VAR_PARA(actECCURVE) Curve,
  actLengthType p_bytes,
  actLengthType n_bytes);

/**********************************************************************************************************************
 actECParseSpeedUpExtension()
 *********************************************************************************************************************/
/*! \brief          Function to parse speed up extension TLV object (containing pre-computed points).
 *  \details        -
 *  \param[in]      speedupExtension     buffer of speedup extension parameters
 *  \param[in,out]  Curve                (in)  underlying curve
 *                                       (out) underlying curve, prec_first is parsed from domain_ext
 *  \param[in]      p_bytes              length of field prime in bytes
 *  \return         actEXCEPTION_SPEEDUP_EXT  speedup extension decoding error
 *                  actOK                     else
 *  \pre            speedupExtension and Curve must be valid pointers
 *                  speedupExtension must be a valid ASN1 structure for precomputed EC points.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParseSpeedUpExtension(
  VSECPRIM_P2ROMCONST_PARA(actU8) speedupExtension,
  VSECPRIM_P2VAR_PARA(actECCURVE) Curve,
  actLengthType p_bytes);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  actParseTL()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actLengthType) actParseTL(
  VSECPRIM_P2ROMCONST_PARA(actU8) input,
  VSECPRIM_P2VAR_PARA(actLengthType) value_length)
{
  actLengthType returnLength;

  if (ACT_ROM_R_BYT(input[1]) < 0x80u)
  {
    if (value_length != NULL_PTR)
    {
      *value_length = ACT_ROM_R_BYT(input[1]); /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    returnLength = 2u;
  }
  else if (ACT_ROM_R_BYT(input[1]) == 0x81u)
  {
    if (value_length != NULL_PTR)
    {
      *value_length = ACT_ROM_R_BYT(input[2]); /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    returnLength = 3u;
  }
  else if (ACT_ROM_R_BYT(input[1]) == 0x82u)
  {
    if (value_length != NULL_PTR)
    {
      *value_length = ((actLengthType)ACT_ROM_R_BYT(input[2]) << 8u) | ACT_ROM_R_BYT(input[3]); /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    returnLength = 4u;
  }
  else
  {
    returnLength = 0u;
  }

  return returnLength;
}

/**********************************************************************************************************************
 *  actParseTLVElement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actLengthType) actParseTLVElement(
  VSECPRIM_P2ROMCONST_PARA(actU8) input,
  actLengthType element_length,
  VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) addr)
{
  actLengthType parsedElementLength, returnLength;
  actLengthType startIndexElement = actParseTL(input, &parsedElementLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */

  /* If tag length was not encoded correctly or the expected element length does not
   *  coincide with the parsed element length, return 0. */
  if ((startIndexElement == 0u) || (parsedElementLength > element_length))
  {
    returnLength = 0u;
  }
  else
  {
    if (addr != NULL_PTR)
    {
      *addr = &input[startIndexElement]; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    returnLength = startIndexElement + element_length;
  }

  return returnLength;
}

/**********************************************************************************************************************
 *  actParseTLVFieldPrime()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actLengthType) actParseTLVFieldPrime(
  VSECPRIM_P2ROMCONST_PARA(actU8) input,
  VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) addr,
  VSECPRIM_P2VAR_PARA(actLengthType) length)
{
  actLengthType startIndexNextElement;
  actLengthType startIndexPrime = actParseTL(input, length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  if (startIndexPrime == 0u)
  {
    /* tag length is not encoded correctly */
    startIndexNextElement = 0u;
  }
  else
  {
    /* skip leading zeros, adjust length to number of leading zeros */
    while (ACT_ROM_R_BYT(input[startIndexPrime]) == 0u)
    {
      ++startIndexPrime;
      *length = *length - 1u; /* SBSW_VSECPRIM_WRITE_POINTER */
    }

    if (addr != NULL_PTR)
    {
      *addr = &input[startIndexPrime]; /* SBSW_VSECPRIM_WRITE_POINTER */
    }

    startIndexNextElement = startIndexPrime + (*length);
  }

  return startIndexNextElement;
}

/**********************************************************************************************************************
 *  actParseTLVPoint()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actLengthType) actParseTLVPoint(
  VSECPRIM_P2ROMCONST_PARA(actU8) input,
  actLengthType x_y_length,
  VSECPRIM_P2VAR_PARA(actLengthType) tag)
{
  actLengthType indexTag, pointLength;
  actLengthType startIndexNextDataEntry;
  indexTag = actParseTL(input, &pointLength);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */

  /* If the tag length is not encoded correctly or the expected point length
   *  does not coincide with the parsed point length, return 0. */
  if ((indexTag == 0u) || (pointLength > ((2u*x_y_length) + 1u)))
  {
    startIndexNextDataEntry = 0u;
  }
  else
  {
    if (tag != NULL_PTR)
    {
      *tag = ACT_ROM_R_BYT(input[indexTag]);  /* SBSW_VSECPRIM_WRITE_POINTER */
    }

    switch (ACT_ROM_R_BYT(input[indexTag])) /* equivalent to switch(flag), but flag could ba a null pointer */
    {
      /* If the point is in compressed representation, the ASN1 struct only stores one coordinate of the point.
         See also comments in Section LOCAL CONSTANT MACROS. */
    case actBASEPOINTENCODING_COMPRESSED_POSITIVE:
    case actBASEPOINTENCODING_COMPRESSED_NEGATIVE:
      {
        startIndexNextDataEntry = indexTag + x_y_length + 1u;
        break;
      }
    /* If the point is in uncompressed representation, the ASN1 struct stores both the x- and the y-coordinate
    *  of the point. See also comments in Section LOCAL CONSTANT MACROS. */
    case actBASEPOINTENCODING_UNCOMPRESSED:
    case actBASEPOINTENCODING_UNCOMPRESSED_LEADING06:
    case actBASEPOINTENCODING_UNCOMPRESSED_LEADING07:
      {
        startIndexNextDataEntry = indexTag + ((2u * x_y_length) + 1u);
        break;
      }
    default:
      {
        startIndexNextDataEntry = 0u;
        break;
      }
    }
  }
  return startIndexNextDataEntry;
}

/**********************************************************************************************************************
 *  actParsePoint()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParsePoint(
  VSECPRIM_P2ROMCONST_PARA(actU8) input,
  VSECPRIM_P2VAR_PARA(actECCURVE) Curve,
  actLengthType pointNumber,
  actLengthType p_bytes,
  actLengthType lengthOfASN1Struct,
  VSECPRIM_P2VAR_PARA(actLengthType) currentIndex)
{
  actRETURNCODE retVal;
  actLengthType pointUncompressedTag, startIndexNextPoint;

  if (ACT_ROM_BYT(input) != actTAG_OCTETSTRING)
  {
    retVal = actEXCEPTION_SPEEDUP_EXT;
  }
  else
  {
    startIndexNextPoint = actParseTLVPoint(input, p_bytes, &pointUncompressedTag); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
    if ((startIndexNextPoint == 0u) || (pointUncompressedTag < actBASEPOINTENCODING_UNCOMPRESSED) ||
        ((*currentIndex + startIndexNextPoint) > lengthOfASN1Struct))
    {
      retVal = actEXCEPTION_SPEEDUP_EXT;
    }
    else
    {
      if (pointNumber == 0u)
      {
        Curve->prec_first = input; /* SBSW_VSECPRIM_WRITE_POINTER */
      }

      *currentIndex += startIndexNextPoint; /* SBSW_VSECPRIM_WRITE_POINTER */
      retVal = actOK;
    }
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actECCheckWorkSpaceSize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actECCheckWorkSpaceSize(
  actU16 algo_flag,
  actLengthType p_length,
  actLengthType n_length,
  actLengthType wksp_len)
{
  actRETURNCODE result = actOK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  actLengthType max_length = 0u; /* PRQA S 2981 */ /* MD_VSECPRIM_2.2 */

  /* check workspace length */
  if ((algo_flag != 0u) && (algo_flag < actEC_ALGO_FLAG_BDKA))
  {
    max_length = sizeof(actECPSTRUCT);
  }
  else if (algo_flag == actEC_ALGO_FLAG_BDKA)
  {
    max_length = (actECBDWksp(p_length, n_length) * actBN_BYTES_PER_DIGIT) + sizeof(actECCURVE);
  }
  else if (algo_flag == actEC_ALGO_FLAG_SPAKE2P)
  {
    max_length = sizeof(actSPAKE2PPreambleStruct);
  }
  else
  {
    result = actEXCEPTION_ALGOID;
  }

  if (result == actOK)
  {
    if (wksp_len < max_length)
    {
      result = actEXCEPTION_MEMORY;
    }
  }

  return result;
}

/**********************************************************************************************************************
 *  actParseSkipSequenceTag()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParseSkipSequenceTag(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
  VSECPRIM_P2VAR_PARA(actLengthType) tagLength,
  VSECPRIM_P2VAR_PARA(actLengthType) startIndexData,
  actRETURNCODE retValInErrorCase)
{
  actRETURNCODE retVal = actOK;

  if (ACT_ROM_BYT(domain) != actTAG_SEQUENCE)
  {
    retVal = retValInErrorCase;
  }
  else
  {
    *startIndexData = actParseTL(domain, tagLength);  /* SBSW_VSECPRIM_WRITE_POINTER */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    if (*startIndexData == 0u)
    {
      retVal = retValInErrorCase;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  actParseFieldConstants()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParseFieldConstants(
  VSECPRIM_P2CONST_PARA(actU8) domain,
  VSECPRIM_P2VAR_PARA(actBNRING) field,
  actLengthType dataLengthRR,
  VSECPRIM_P2VAR_PARA(actLengthType) startIndexNextDataEntry)
{
  actRETURNCODE retVal;
  actLengthType currentIndex, startIndexNextDataEntryLocal = 0u; /* PRQA S 2981 */ /* MD_VSECPRIM_2.2 */
  actLengthType digitLengthRR = actBNGetDigitLength(dataLengthRR);

  retVal = actOK;

  /* parse p_RR / n_RR */
  if (ACT_ROM_BYT(domain) != actTAG_OCTETSTRING)
  {
    retVal = actEXCEPTION_DOMAIN_EXT;
  }
  else
  {
    startIndexNextDataEntryLocal = actParseTLVElement(domain, dataLengthRR, (VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)))NULL_PTR);  /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */

    if (startIndexNextDataEntryLocal == 0u)
    {
      retVal = actEXCEPTION_DOMAIN_EXT;
    }
  }

  if (retVal == actOK)
  {
    currentIndex = startIndexNextDataEntryLocal;
    /* field->RR points a to an address in the workspace. The buffer size provided is the workspace is actECC_MAX_P_DIGIT_LEN (or actECC_MAX_N_DIGIT_LEN, respectively).
    * This is sufficient, because otherwise actParseTLVElement would have returned 0. */
    actBNSetOctetStringROM(field->RR, digitLengthRR, &domain[currentIndex - dataLengthRR], dataLengthRR);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

    /* parse p_bar / n_bar */
    if (ACT_ROM_BYT(&domain[currentIndex]) != actTAG_OCTETSTRING)
    {
      retVal = actEXCEPTION_DOMAIN_EXT;
    }
    else
    {
      startIndexNextDataEntryLocal = actParseTLVElement(&domain[currentIndex], actBN_BYTES_PER_DIGIT, (VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)))NULL_PTR);  /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */

      if (startIndexNextDataEntryLocal == 0u)
      {
        retVal = actEXCEPTION_DOMAIN_EXT;
      }
      else
      {
        currentIndex += startIndexNextDataEntryLocal;
        actBNSetOctetStringROM(&(field->m_bar), 1, &domain[currentIndex - actBN_BYTES_PER_DIGIT], actBN_BYTES_PER_DIGIT); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        *startIndexNextDataEntry = currentIndex; /* SBSW_VSECPRIM_WRITE_POINTER */
      }
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 *  actParseCurveCoefficient()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParseCurveCoefficient(
  VSECPRIM_P2CONST_PARA(actU8) domain,
  actLengthType dataLength,
  VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) dataAddr, /* PRQA S 3673 */ /* MD_VSECPRIM_CONST_POINTER */
  VSECPRIM_P2VAR_PARA(actLengthType) startIndexNextDataEntry,
  boolean retValDomainExtension)
{
  actRETURNCODE retVal;
  actRETURNCODE retValError;

  retVal = actOK;
  retValError = ((retValDomainExtension == TRUE) ? actEXCEPTION_DOMAIN_EXT : actEXCEPTION_DOMAIN);

  if (ACT_ROM_BYT(domain) != actTAG_OCTETSTRING)
  {
    retVal = retValError;
  }
  else
  {
    *startIndexNextDataEntry = actParseTLVElement(domain, dataLength, dataAddr); /* SBSW_VSECPRIM_WRITE_POINTER */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* If dataLength does not coincide with the length value stored in domain, stop parsin the domain. */
    if (*startIndexNextDataEntry == 0u)
    {
      retVal = retValError;
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 *  actParseCurveParameters()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParseCurveParameters(
  VSECPRIM_P2CONST_PARA(actU8) domain,
  VSECPRIM_P2VAR_PARA(actECCURVE) Curve,
  actLengthType dataLength,
  VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) addressA, /* PRQA S 3673 */ /* MD_VSECPRIM_CONST_POINTER */
  VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) addressB, /* PRQA S 3673 */ /* MD_VSECPRIM_CONST_POINTER */
  boolean retValDomainExtension,
  VSECPRIM_P2VAR_PARA(actLengthType) startIndexNextDataEntry)
{
  actRETURNCODE retVal;
  actRETURNCODE retValError;

  actLengthType startIndexNextDataEntryLocal, tag, currentIndex = 0u;  /* PRQA S 2981 */ /* MD_VSECPRIM_2.2 */

  retValError = ((retValDomainExtension == TRUE) ? actEXCEPTION_DOMAIN_EXT : actEXCEPTION_DOMAIN);

  /* parse a / a_R */
  retVal = actParseCurveCoefficient(domain, dataLength, addressA, &startIndexNextDataEntryLocal, retValDomainExtension); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */

  /* parse b / b_R */
  if (retVal == actOK)
  {
    currentIndex = startIndexNextDataEntryLocal;
    retVal = actParseCurveCoefficient(&domain[currentIndex], dataLength, addressB, &startIndexNextDataEntryLocal, retValDomainExtension); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
  }
  else
  {
    retVal = retValError;
  }

  /* parse G / G_R */
  if (retVal == actOK)
  {
    currentIndex += startIndexNextDataEntryLocal;

    if (ACT_ROM_BYT(&domain[currentIndex]) != actTAG_OCTETSTRING)
    {
      retVal = retValError;
    }
    else
    {
      startIndexNextDataEntryLocal = actParseTLVPoint(&domain[currentIndex], dataLength, &tag); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */

      if ((startIndexNextDataEntryLocal == 0u) || (tag < 0x04u))
      {
        retVal = retValError;
      }
      else
      {
        currentIndex += startIndexNextDataEntryLocal;
        *startIndexNextDataEntry = currentIndex; /* SBSW_VSECPRIM_WRITE_POINTER */

        if (Curve != NULL_PTR)
        {
          Curve->G_R = &domain[currentIndex - (2u * dataLength)]; /* SBSW_VSECPRIM_WRITE_POINTER */
        }
      }

    }
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actParseSkipUntilFieldPrime()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParseSkipUntilFieldPrime(
 VSECPRIM_P2CONST_PARA(actU8) domain,
  VSECPRIM_P2VAR_PARA(actLengthType) lengthOfASN1Struct,
  VSECPRIM_P2VAR_PARA(actLengthType) startIndexData)
{
  actRETURNCODE retVal;
  actLengthType startIndexDataLocal, currentIndex, tagLengthLocal;
  const actU8 PRIME_OID_TLV[] = { 0x06u, 0x07u, 0x2au, 0x86u, 0x48u, 0xceu, 0x3du, 0x01u, 0x01u };

  startIndexDataLocal = 0u;
  currentIndex = 0u;  /* PRQA S 2982 */ /* MD_VSECPRIM_2.2 */

  /* skip TAG sequence */
  retVal = actParseSkipSequenceTag(domain, lengthOfASN1Struct, &startIndexDataLocal, actEXCEPTION_DOMAIN); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
  /* Currently, lengthOfASN1Struct contains the length of the ASN1 struct without the initial sequence.
   * To obtain the overall length of the struct including the initial sequence, we add the startIndexData. */
  *lengthOfASN1Struct += startIndexDataLocal; /* SBSW_VSECPRIM_WRITE_POINTER */

  /* If the struct length is less than 18 bytes, then one of the following operations would lead to an out-of-bounds-reading-access */
  if (*lengthOfASN1Struct < (startIndexDataLocal + actASN1_MIN_VERSION_LEN + actASN1_MIN_SEQ_LEN + sizeof(PRIME_OID_TLV)))
  {
    retVal = actEXCEPTION_DOMAIN;
  }

  /* skip version, but still check that version is encoded correctly */
  if (retVal == actOK)
  {
    currentIndex = startIndexDataLocal;

    if ((ACT_ROM_BYT(&domain[currentIndex]) != actTAG_INTEGER) ||
        (ACT_ROM_BYT(&domain[currentIndex + 1u]) != 0x01u) ||
        (ACT_ROM_BYT(&domain[currentIndex + 2u]) != 0x01u))
    {
      retVal = actEXCEPTION_DOMAIN;
    }
    else
    {
      currentIndex += 3u;
      retVal = actParseSkipSequenceTag(&domain[currentIndex], &tagLengthLocal, &startIndexDataLocal, actEXCEPTION_DOMAIN); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */

    }
  }

  /* skip prime OID; second condition prevents out-of-bounds-reading-access */
  if (retVal == actOK)
  {
    currentIndex += startIndexDataLocal;

    if (actMemcmp_ROM(&domain[currentIndex], PRIME_OID_TLV, sizeof(PRIME_OID_TLV)) == FALSE) /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
    {
      retVal = actEXCEPTION_DOMAIN;
    }
    else
    {
      currentIndex += sizeof(PRIME_OID_TLV);
      *startIndexData = currentIndex; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 *  actParseFieldPrime()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParseFieldPrime(
  VSECPRIM_P2CONST_PARA(actU8) domain,
  VSECPRIM_P2VAR_PARA(actLengthType) primeByteLength,
  VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) primeStartAddr, /* PRQA S 3673 */ /* MD_VSECPRIM_CONST_POINTER */
  VSECPRIM_P2VAR_PARA(actLengthType) startIndexNextDataEntry)
{
  actLengthType tagLength;
  actRETURNCODE retVal = actOK;

  if (ACT_ROM_BYT(domain) != actTAG_INTEGER)
  {
    retVal = actEXCEPTION_DOMAIN;
  }
  else
  {
    if (startIndexNextDataEntry != NULL_PTR)
    {
      *startIndexNextDataEntry = actParseTLVFieldPrime(domain, primeStartAddr, primeByteLength); /* SBSW_VSECPRIM_WRITE_POINTER */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      if (*startIndexNextDataEntry == 0u)
      {
        retVal = actEXCEPTION_DOMAIN;
      }
    }
    else
    {
      tagLength = actParseTLVFieldPrime(domain, primeStartAddr, primeByteLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      if (tagLength == 0u)
      {
        retVal = actEXCEPTION_DOMAIN;
      }
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 *  actParseDomainExtension()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParseDomainExtension(
  VSECPRIM_P2ROMCONST_PARA(actU8) domainExtension,
  VSECPRIM_P2VAR_PARA(actECCURVE) Curve,
  actLengthType p_bytes,
  actLengthType n_bytes)
{
  actRETURNCODE retVal;
  actLengthType startIndexData, lengthOfANS1Struct, startIndexNextDataEntry, currentIndex;

  VSECPRIM_P2VAR_PARA(actBNRING) p_field = &(Curve->p_field);
  VSECPRIM_P2VAR_PARA(actBNRING) n_field = &(Curve->n_field);

  startIndexData = 0u;
  lengthOfANS1Struct = 0u;
  startIndexNextDataEntry = 0u;
  currentIndex = 0u; /* PRQA S 2982 */ /* MD_VSECPRIM_2.2 */

  /* skip sequence tag */
  retVal = actParseSkipSequenceTag(domainExtension, &lengthOfANS1Struct, &startIndexData, actEXCEPTION_DOMAIN_EXT); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
  /* Currently, lengthOfASN1Struct contains the length of the ASN1 struct without the initial sequence.
   * To obtain the overall length of the struct including the initial sequence, we add the startIndexData. */
  lengthOfANS1Struct += startIndexData;

  /* skip next sequence tag */
  if (retVal == actOK) /* startIndexData < lengthOfANS1Struct automatically fulfilled */
  {
    currentIndex = startIndexData;
    retVal = actParseSkipSequenceTag(&domainExtension[currentIndex], (VSECPRIM_P2VAR_PARA(actLengthType))NULL_PTR, &startIndexData, actEXCEPTION_DOMAIN_EXT); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
  }

  /* parse p_RR and p_bar */
  if ((retVal == actOK) && ((currentIndex + startIndexData) < lengthOfANS1Struct))
  {
    currentIndex += startIndexData;
    retVal = actParseFieldConstants(&domainExtension[currentIndex], p_field, p_bytes, &startIndexNextDataEntry); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
  }
  else
  {
    /* set retVal in case ((currentIndex + startIndexData) > lengthOfANS1Struct)*/
    retVal = actEXCEPTION_DOMAIN_EXT;
  }

  /* skip sequence tag */
  if ((retVal == actOK) && ((currentIndex + startIndexNextDataEntry) < lengthOfANS1Struct))
  {
    currentIndex += startIndexNextDataEntry;
    /* Here, the tag length is not checked, because in the next step the length of each single curve parameter will be checked. */
    retVal = actParseSkipSequenceTag(&domainExtension[currentIndex], (VSECPRIM_P2VAR_PARA(actLengthType))NULL_PTR, &startIndexData, actEXCEPTION_DOMAIN_EXT); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
  }
  else
  {
    /* set retVal in case ((currentIndex + startIndexNextDataEntry) > lengthOfANS1Struct)*/
    retVal = actEXCEPTION_DOMAIN_EXT;
  }

  /* parse a_r, b_R and G_R */
  if ((retVal == actOK) && ((currentIndex + startIndexData) < lengthOfANS1Struct))
  {
    currentIndex += startIndexData;
    retVal = actParseCurveParameters(&domainExtension[currentIndex], Curve, p_bytes, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
                                     &Curve->a_R, &Curve->b_R, TRUE, &startIndexNextDataEntry);
  }
  else
  {
    /* set retVal in case ((currentIndex + startIndexData) > lengthOfANS1Struct)*/
    retVal = actEXCEPTION_DOMAIN_EXT;
  }

  /* skip sequence tag */
  if ((retVal == actOK) && ((currentIndex + startIndexNextDataEntry) < lengthOfANS1Struct))
  {
    currentIndex += startIndexNextDataEntry;
    /* Here, the tag length is not checked, because in the next step the length of the field constants will be checked individually. */
    retVal = actParseSkipSequenceTag(&domainExtension[currentIndex], (VSECPRIM_P2VAR_PARA(actLengthType))NULL_PTR, &startIndexData, actEXCEPTION_DOMAIN_EXT); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
  }
  else
  {
    /* set retVal in case ((currentIndex + startIndexNextDataEntry) > lengthOfANS1Struct)*/
    retVal = actEXCEPTION_DOMAIN_EXT;
  }

  /* parse n_RR and n_bar */
  if ((retVal == actOK) && ((currentIndex + startIndexData) < lengthOfANS1Struct))
  {
    currentIndex += startIndexData;
    retVal = actParseFieldConstants(&domainExtension[currentIndex], n_field, n_bytes, &startIndexNextDataEntry); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
  }
  else
  {
    /* set retVal in case ((currentIndex + startIndexData) > lengthOfANS1Struct)*/
    retVal = actEXCEPTION_DOMAIN_EXT;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  actParseSpeedUpExtension()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actParseSpeedUpExtension(
  VSECPRIM_P2ROMCONST_PARA(actU8) speedupExtension,
  VSECPRIM_P2VAR_PARA(actECCURVE) Curve,
  actLengthType p_bytes)
{
  actRETURNCODE retVal;
  actLengthType i, startIndexData, startIndexNextDataEntry, currentIndex, lengthOfANS1Struct;
  actU32 num_of_points;

  startIndexData = 0u;
  currentIndex = 0u;
  lengthOfANS1Struct = 0u;

  /* skip sequence TAG */
  retVal = actParseSkipSequenceTag(speedupExtension, &lengthOfANS1Struct, &startIndexData, actEXCEPTION_SPEEDUP_EXT); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
  /* Currently, lengthOfASN1Struct contains the length of the ASN1 struct without the initial sequence.
   * To obtain the overall length of the struct including the initial sequence, we add the startIndexData. */
  lengthOfANS1Struct += startIndexData;

  /* parse groups parameter */
  if (retVal == actOK) /* startIndexData < lengthOfANS1Struct automatically fulfilled */
  {
    currentIndex = startIndexData;
    if (ACT_ROM_BYT(&speedupExtension[currentIndex]) != actTAG_OCTETSTRING)
    {
      retVal = actEXCEPTION_SPEEDUP_EXT;
    }
    else
    {
      startIndexNextDataEntry = actParseTLVElement(&speedupExtension[currentIndex], 1,  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS */
                                                   (VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)))NULL_PTR);

      if ((startIndexNextDataEntry == 0u) || ((currentIndex + startIndexNextDataEntry) > lengthOfANS1Struct))
      {
        retVal = actEXCEPTION_SPEEDUP_EXT;
      }
      else
      {
        currentIndex += startIndexNextDataEntry;
        Curve->groups = ACT_ROM_BYT(&speedupExtension[currentIndex - 1u]); /* SBSW_VSECPRIM_WRITE_POINTER */
      }
    }
  }

  if (retVal == actOK)
  {
    if (Curve->groups < 2u)
    {
      retVal = actEXCEPTION_SPEEDUP_EXT;
    }
    else
    {
      /* skip sequence TAG */
      retVal = actParseSkipSequenceTag(&speedupExtension[currentIndex], (VSECPRIM_P2VAR_PARA(actLengthType))NULL_PTR,
                                       &startIndexData, actEXCEPTION_SPEEDUP_EXT); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS */
      if ((currentIndex + startIndexData) > lengthOfANS1Struct)
      {
        retVal = actEXCEPTION_SPEEDUP_EXT;
      }
      else
      {
        currentIndex += startIndexData;
      }
    }
  }

  /* parse points */
  num_of_points = (actLengthType)(1UL << Curve->groups);
  for (i = 0u; i < num_of_points; ++i)
  {
    if (retVal == actOK)
    {
      retVal = actParsePoint(&speedupExtension[currentIndex], Curve, i, p_bytes,  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
                             lengthOfANS1Struct, &currentIndex);
    }
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  actECParseDomain()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actECParseDomain(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
  VSECPRIM_P2VAR_PARA(actLengthType) p_bytes,
  VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) p_addr, /* PRQA S 3673 */ /* MD_VSECPRIM_CONST_POINTER */
  VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) a_addr, /* PRQA S 3673 */ /* MD_VSECPRIM_CONST_POINTER */
  VSECPRIM_P2VAR_PARA(actLengthType) n_bytes,
  VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) n_addr) /* PRQA S 3673 */ /* MD_VSECPRIM_CONST_POINTER */
{
  actRETURNCODE retVal;
  actLengthType startIndexData = 0u;
  actLengthType startIndexNextDataEntry = 0u;
  actLengthType currentIndex = 0u; /* PRQA S 2981 */ /* MD_VSECPRIM_2.2 */
  actLengthType localTagLength = 0u;
  actLengthType lengthOfASN1Struct = 0u;

  *p_bytes = 0u; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
  *n_bytes = 0u; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */

  retVal = actParseSkipUntilFieldPrime(domain, &lengthOfASN1Struct, &startIndexData); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */

  /* parse prime p */
  if (retVal == actOK) /* startIndexData < lengthOfASN1Struct was already checked in actParseSkipUntilFieldPrime */
  {
    currentIndex = startIndexData;
    retVal = actParseFieldPrime(&domain[currentIndex], p_bytes, p_addr, &startIndexNextDataEntry); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */

    if (*p_bytes > actECC_MAX_BYTE_LEN)
    {
      retVal = actEXCEPTION_DOMAIN;
    }
  }

  /* skip TAG sequence */
  if ((retVal == actOK) && ((currentIndex + startIndexNextDataEntry) < lengthOfASN1Struct))
  {
    currentIndex += startIndexNextDataEntry;
    retVal = actParseSkipSequenceTag(&domain[currentIndex], &localTagLength, &startIndexData, actEXCEPTION_DOMAIN); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
  }
  else
  {
    /* set retVal in case ((currentIndex + startIndexNextDataEntry) < lengthOfASN1Struct) */
    retVal = actEXCEPTION_DOMAIN;
  }

  /* parse curve parameter a, skip curve parameter b and base point G */
  if ((retVal == actOK) && ((currentIndex + localTagLength) < lengthOfASN1Struct))
  {
    currentIndex += startIndexData;
    retVal = actParseCurveParameters(&domain[currentIndex], (VSECPRIM_P2VAR_PARA(actECCURVE))NULL_PTR, *p_bytes, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
                                     a_addr, (VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)))NULL_PTR, FALSE, &startIndexNextDataEntry);
  }
  else
  {
    /* set retVal in case ((currentIndex + localTagLength) < lengthOfASN1Struct) */
    retVal = actEXCEPTION_DOMAIN;
  }

  /* parse prime n */
  if ((retVal == actOK) && ((currentIndex + startIndexNextDataEntry) < lengthOfASN1Struct))
  {
    currentIndex += startIndexNextDataEntry;
    retVal = actParseFieldPrime(&domain[currentIndex], n_bytes, n_addr, (VSECPRIM_P2VAR_PARA(actLengthType))NULL_PTR); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS */

    if (*n_bytes > actECC_MAX_BYTE_LEN)
    {
      retVal = actEXCEPTION_DOMAIN;
    }
  }
  else
  {
    /* set retVal in case ((currentIndex + startIndexNextDataEntry) > lengthOfASN1Struct) */
    retVal = actEXCEPTION_DOMAIN;
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actECInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actECInit(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
  VSECPRIM_P2ROMCONST_PARA(actU8) domain_ext,
  VSECPRIM_P2ROMCONST_PARA(actU8) speedup_ext,
  actU16 algo_flag,
  VSECPRIM_P2VAR_PARA(actECPBasicStruct) wksp,
  actLengthType wksp_len)
{
  actRETURNCODE result;
  VSECPRIM_P2CONST_PARA(actU8) p_addr;
  VSECPRIM_P2CONST_PARA(actU8) a_addr;
  VSECPRIM_P2CONST_PARA(actU8) n_addr;
  actLengthType p_bytes, n_bytes, p_length, n_length;

  VSECPRIM_P2VAR_PARA(actBNDIGIT) p_minus_3;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a;

  VSECPRIM_P2VAR_PARA(actECCURVE) Curve = &(wksp->Curve);
  VSECPRIM_P2VAR_PARA(actBNRING) p_field = &Curve->p_field;
  VSECPRIM_P2VAR_PARA(actBNRING) n_field = &Curve->n_field;

  result = actECParseDomain(domain, &p_bytes, &p_addr, &a_addr, &n_bytes, &n_addr); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */

  if (result == actOK)
  {
    p_length = actBNGetDigitLength(p_bytes);
    n_length = actBNGetDigitLength(n_bytes);

    result = actECCheckWorkSpaceSize(algo_flag, p_length, n_length, wksp_len);

    if (result == actOK)
    {
      /* assign temporary bignum workspace for p_field and n_field */
      p_field->m = wksp->p_field_modulus; /* SBSW_VSECPRIM_WRITE_POINTER */
      p_field->RR = wksp->p_field_RR; /* SBSW_VSECPRIM_WRITE_POINTER */
      n_field->m = wksp->n_field_modulus; /* SBSW_VSECPRIM_WRITE_POINTER */
      n_field->RR = wksp->n_field_RR; /* SBSW_VSECPRIM_WRITE_POINTER */

      /* assign temporary bignum workspace used for point operations etc; currently, actEC_BIGNUM_TMP is always 4 */
      Curve->t[0] = wksp->Curve_tmp0; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
      Curve->t[1] = wksp->Curve_tmp1; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
      Curve->t[2] = wksp->Curve_tmp2; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
      Curve->t[3] = wksp->Curve_tmp3; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */

      /* initialize prime and order size values */
      p_field->m_length = p_length;      /* SBSW_VSECPRIM_WRITE_POINTER */
      p_field->m_byte_length = p_bytes;  /* SBSW_VSECPRIM_WRITE_POINTER */
      n_field->m_length = n_length;      /* SBSW_VSECPRIM_WRITE_POINTER */
      n_field->m_byte_length = n_bytes;  /* SBSW_VSECPRIM_WRITE_POINTER */

      /* init prime p; The buffer size of p_field->m is sufficient, since this was checked in actECParseDomain */
      actBNSetOctetStringROM(p_field->m, p_length, p_addr, p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

      /* check if a = p - 3 */
      p_minus_3 = Curve->t[0];
      a = Curve->t[1];

      /* store a in Curve->t[1]; The buffer size of Curve->t[1] is sufficient, since this was checked in actECParseDomain */
      actBNSetOctetStringROM(a, p_length, a_addr, p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      actBNSetZero(p_minus_3, p_length);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      p_minus_3[0] = 3;  /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
      actBNSub(p_field->m, p_minus_3, p_minus_3, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      if (actBNCompare(a, p_minus_3, p_length) == actCOMPARE_EQUAL) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
      {
        Curve->a_equals_p_minus_3 = TRUE; /* SBSW_VSECPRIM_WRITE_POINTER */
      }
      else
      {
        Curve->a_equals_p_minus_3 = FALSE; /* SBSW_VSECPRIM_WRITE_POINTER */
      }

      /* init order n; The buffer size of n_field->m is sufficient, since this was checked in actECParseDomain */
      actBNSetOctetStringROM(n_field->m, n_length, n_addr, n_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

      /* parse domain extension; parsed value will be stored in Curve members */
      result = actParseDomainExtension(domain_ext, Curve, p_bytes, n_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }

    if ((result == actOK) && (speedup_ext != NULL_PTR))
    {
        result = actParseSpeedUpExtension(speedup_ext, Curve, p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
  }

    /* additional settings */

# if (actBN_MONT_MUL_VERSION==1) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    /* initialize prime and order structure switch */
    p_field->prime_structure = 0u;
    n_field->prime_structure = 0u;
# endif

    /* initialize window size for modular exponentiation */
# if (actBN_MOD_EXP_WINDOW_SIZE==0)   /* variable k-bit window algorithm */ /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    p_field->window_size = actEC_MOD_EXP_WINDOW_SIZE;
    n_field->window_size = actEC_MOD_EXP_WINDOW_SIZE;
# endif

  return result;
} /* PRQA S 6050, 6060 */ /* MD_MSR_STCAL, MD_VSECPRIM_STPAR */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTECTOOLS_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actECTools.c
 *********************************************************************************************************************/
