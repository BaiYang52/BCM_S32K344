/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is protected under intellectual property laws and proprietary to cv cryptovision GmbH
 *                and/or Vector Informatik GmbH.
 *                No right or license is granted save as expressly set out in the applicable license conditions.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  actISPAKE2P.h
 *        \brief  Interface for SPAKE2P key exchange.
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

#ifndef ACTISPAKE2P_H
# define ACTISPAKE2P_H

# include "actITypes.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/**********************************************************************************************************************
 *  actSPAKE2PPreambleInit()
 *********************************************************************************************************************/
/*! \brief          Initializes the preamble phase of the SPAKE2+ protocol.
 *  \details        This function parses the domain and the domain extension to initialize the preamble phase of
 *                  the SPAKE2+ protocol.
 *  \param[in,out]  wksp       (in)  pointer to context structure
 *                             (out) pointer to context structure
 *  \param[in]      domain     pointer to domain parameter structure
 *  \param[in]      domain_ext pointer to domain parameter extension structure
 *  \param[in]      wksp_len   available work space size
 *  \return         actEXCEPTION_DOMAIN        domain decoding error
 *                  actEXCEPTION_DOMAIN_EXT    domain_ext decoding error
 *                  actEXCEPTION_MEMORY        wksp_len to small
 *                  actEXCEPTION_LENGTH        a different curve than P256 is used
 *                  actOK                      else
 *  \pre            All pointers must be valid.
 *                  domain and domain_ext have to be valid ASN1 structures for EC domain parameters.
 *                  The addresses of domain and domain_ext have to be valid until returning from
 *                  actSPAKE2PPreambleCalc(), because the pointers to the data stored in domain and domain_ext
 *                  are stored in the workspace. These pointers are used as long as the workspace is used.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSPAKE2PPreambleInit(
  VSECPRIM_P2VAR_PARA(actSPAKE2PPreambleStruct) wksp,
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
  VSECPRIM_P2ROMCONST_PARA(actU8) domain_ext,
  actLengthType wksp_len);

/**********************************************************************************************************************
 * actSPAKE2PPreambleCalc()
 *********************************************************************************************************************/
/*! \brief          This function calculates the values w0 and w1 as w0 = w0s mod n and w1 = w1s mod n according to
 *                  the SPAKE2+ preamble phase.
 *  \details        w0s and w1s are assumed to be concatenated in the parameter 'PBKDFOutput'. n is the group order.
 *  \param[in,out]  wksp               (in)  algorithm context buffer, initialized by actSPAKE2PPreambleInit(..)
 *                                     (out) algorithm context buffer,
 *  \param[in]      pbkdfOutput        PBKDF output, [w0s || w1s] = PBKDF(...)
 *  \param[in]      pbkdfOutputLength  length of the buffer referenced by pbkdfOutput
 *  \param[in,out]  w0OutputBuffer     (in)  buffer to store the result w0 = w0s mod n
 *                                     (out) the result w0
 *  \param[in,out]  w0BufferLength     (in)  length of buffer referenced by w0
 *                                     (out) data length (in bytes) written to w0
 *  \param[in,out]  w1OutputBuffer     (in)  buffer to store the result w1 = w1s mod n
 *                                     (out) the result w1
 *  \param[in,out]  w1BufferLength     (in)  length of buffer referenced by w1
 *                                     (out) data length (in bytes) written to w1
 *  \param[in,out]  pointL             (in)  buffer to store the point L = w1*P in uncompressed representation,
 *                                           including the encoding byte 0x04
 *                                     (out) the point L = w1*P in uncompressed representation,
 *                                           including the encoding byte 0x04
 *  \param[in,out]  pointLBufferLength (in)  length of the buffer referenced by pointL
 *                                     (out) data length (in bytes) written to pointL
 *  \param[in]      watchdog           pointer to watchdog reset function
 *                  actEXCEPTION_MEMORY        memory provided by output buffers not sufficient
 *                  actEXCEPTION_INPUT_LENGTH  length of pbkdfOutputLength greater than configured maximal size
 *                  actEXCEPTION_POINT         an error occurred during the EC operation
 *                  actEXCEPTION_LENGTH        an error occurred during the reduction
 *                  actOK                      operation successful
 *  \pre            All pointers must be valid.
 *                  workSpace is initialized by actSPAKE2PPreambleInit(..)
 *                  The buffer referenced by w0OutputBuffer must provide at least as many bytes as given by the
 *                  value referenced by w0BufferLength.
 *                  The buffer referenced by w1OutputBuffer must provide at least as many bytes as given by the
 *                  value referenced by w1BufferLength.
 *                  The buffer referenced by pointL must provide at least as many bytes as given by the
 *                  value referenced by pointLBufferLength.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSPAKE2PPreambleCalc(
  VSECPRIM_P2VAR_PARA(actSPAKE2PPreambleStruct) wksp,
  VSECPRIM_P2CONST_PARA(actU8) pbkdfOutput,
  const actLengthType pbkdfOutputLength,
  VSECPRIM_P2VAR_PARA(actU8) w0OutputBuffer,
  VSECPRIM_P2VAR_PARA(actLengthType) w0BufferLength,
  VSECPRIM_P2VAR_PARA(actU8) w1OutputBuffer,
  VSECPRIM_P2VAR_PARA(actLengthType) w1BufferLength,
  VSECPRIM_P2VAR_PARA(actU8) pointL,
  VSECPRIM_P2VAR_PARA(actLengthType) pointLBufferLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actSPAKE2PInit()
 *********************************************************************************************************************/
/*! \brief          This function initializes the SPAKE2+ key exchange algorithm.
 *  \details        This function parses the domain and the domain extension and stores the points M and N in the
 *                  work space to initialize the SPAKE2+ protocol.
 *  \param[in,out]  wksp         (in) algorithm context buffer
 *                               (out) initialized algorithm context structure
 *  \param[in]      domain       pointer to domain parameter structure
 *  \param[in]      domain_ext   pointer to domain parameter extension structure
 *  \param[in]      pointM       Point M on the curve in uncompressed representation, including the encoding byte 0x04
 *  \param[in]      mlength      length of buffer referenced by pointM
 *  \param[in]      pointN       Point N on the curve in uncompressed representation, including the encoding byte 0x04
 *  \param[in]      nlength      length of buffer referenced by pointN
 *  \param[in]      wksp_len     available work space size
 *  \return         actEXCEPTION_DOMAIN        domain decoding error
 *                  actEXCEPTION_DOMAIN_EXT    domain_ext decoding error
 *                  actEXCEPTION_MEMORY        wksp_len to small
 *                  actEXCEPTION_LENGTH        a different curve than P256 is used
 *                  actEXCEPTION_INPUT_LENGTH  mlength or nlength invalid
 *                  actOK                      else
 *  \pre            All pointers must be valid.
 *                  domain and domain_ext have to be valid ASN1 structures for EC domain parameters.
 *                  The addresses of domain and domain_ext have to be valid until returning from
 *                  actSPAKE2PCalcSharedSecret(), because the pointers to the data stored in domain and domain_ext
 *                  are stored in the workspace. These pointers are used as long as the workspace is used.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSPAKE2PInit(
  VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wksp,
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
  VSECPRIM_P2ROMCONST_PARA(actU8) domain_ext,
  VSECPRIM_P2CONST_PARA(actU8) pointM,
  const actLengthType mlength,
  VSECPRIM_P2CONST_PARA(actU8) pointN,
  const actLengthType nlength,
  actLengthType wksp_len);

/**********************************************************************************************************************
 *  actSPAKE2PSetPreambleData()
 *********************************************************************************************************************/
/*! \brief           This function provides data from the preamble phase to the SPAKE2+ algorithm for party A or B.
 *  \details         The provided data will be stored in the workspace. In this function the workspace also sets a
 *                   flag that the operations for party A shall be executed.
 *  \param[in,out]  wsSpake      (in) algorithm context buffer, initialized by actSPAKE2PInit()
 *                               (out) algorithm context structure, now storing the preamble data
 *  \param[in]      w0           value w0 (w0 is the first half of the OKM of a PBKDF mod n, computed during the
 *                               preamble phase)
 *  \param[in]      w0length     length of buffer referenced by w0
 *  \param[in]      w1           value w1 (w1 is the second half of the OKM of a PBKDF mod n, computed during the
 *                               preamble phase)
 *  \param[in]      w1length     length of buffer referenced by w1
 *  \param[in]      pointL       point L in uncompressed representation, including the encoding byte 0x04
 *                               (L = w1*P, where w1 is the second half of the OKM of a PBKDF mod p,
 *                               computed during the preamble phase, and P is the base point of the curve)
 *  \param[in]      pointLLength length of buffer referenced by L
 *                  actEXCEPTION_INPUT_LENGTH  w0length, w1length or pointLLength invalid
 *                  actEXCEPTION_NULL          w1 and pointL both are null pointers
 *                  actOK                      else
 *  \pre            w0 must be a valid pointer.
 *                  Either w1 or pointL must be a valid pointer.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
 VSECPRIM_FUNC(actRETURNCODE) actSPAKE2PSetPreambleData(
   VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wsSpake,
   VSECPRIM_P2CONST_PARA(actU8) w0,
   const actLengthType w0length,
   VSECPRIM_P2CONST_PARA(actU8) w1,
   const actLengthType w1length,
   VSECPRIM_P2CONST_PARA(actU8) pointL,
   const actLengthType pointLLength);

 /**********************************************************************************************************************
 *  actSPAKE2PCalcPubVal()
 *********************************************************************************************************************/
/*! \brief           This function calculates the public value for party A or party B according to the SPAKE2+ protocol.
 *  \details         The workspace stores a flag indicating whether the operations for party A or for party B shall be
 *                   executed, i.e., this function computes
 *                   - X = pA = x*P + w0*M or
 *                   - Y = pB = y*P + w0*N
 *  \param[in,out] wsSpake                   (in)  algorithm context buffer, initialized by actSPAKE2PInit
 *                                           (out) algorithm context buffer, now storing the own public value
 *  \param[in,out]  pubValXCoordinate        (in)  buffer to store the x-coordinate of the public value X or Y
 *                                                 (of length getLengthOfEcPpublicKey_comp(domain))
 *                                           (out) the x-coordinate of the public value X or Y
 *  \param[in,out]  pubValYCoordinate        (in)  buffer to store the y-coordinate of the public value X or Y
 *                                                 (of length getLengthOfEcPpublicKey_comp(domain))
 *                                           (out) the y-coordinate of the public value X or Y
 *  \param[in,out]  pubValCoordinateLength   (in)  length of the buffers referenced by pubVal_x and pubVal_y
 *                                           (out) length of the data in byte written to pubVal_x and pubVal_y
 *  \param[in]      watchdog                 Watchdog trigger function pointer
 *  \return         actEXCEPTION_OUTPUT_BUFFER  memory provided by output buffers not sufficient
 *                  actEXCEPTION_UNKNOWN        random number generation failed
 *                  actEXCEPTION_POINT          EC operation failed
 *                  actOK                       else
 *  \pre            All pointers must be valid.
 *                  The buffers referenced by pubVal_x and pubVal_y have to provide at least as many bytes as
 *                  given in the value referenced by pubVal_coordinateLength.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSPAKE2PCalcPubVal(
  VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wsSpake,
  VSECPRIM_P2VAR_PARA(actU8) pubValXCoordinate,
  VSECPRIM_P2VAR_PARA(actU8) pubValYCoordinate,
  VSECPRIM_P2VAR_PARA(actLengthType) pubValCoordinateLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actSPAKE2PSetAdditionalInformation()
 *********************************************************************************************************************/
/*! \brief           This optional function provides additional input data for the derivation of the shared secret
 *                   according to the SPAKE2+ protocol.
 *  \details         The provided data will be stored in the workspace. This function does not distinguish between
 *                   the two parties A and B. Calling this function is optional.
 *                   The workspace flag, indicating that this function was called, is set to TRUE.
 *  \param[in,out]  wsSpake      (in)  SPAKE algorithm context buffer, initialized by actSPAKE2PInit(..)
 *                               (out) algorithm context structure
 *  \param[in,out]  wsHash       (in)  SHA256 algorithm context buffer, a part of the overall esl-spake-workspace
 *                               (out) SHA256 algorithm context structure, possibly initialized
 *  \param[in]      infoA        additional information concering party A (e.g. A's name,...) or NULL
 *  \param[in]      infoALength  length of buffer referenced by infoA, possibly 0
 *  \param[in]      infoB        additional information concering party B (e.g. B's name,...) or NULL
 *  \param[in]      infoBLength  length of buffer referenced by infoB, possibly 0
 *  \param[in]      aad          additional authentication data, used as input for the KDF, or NULL
 *  \param[in]      aadLength    length of buffer referenced by aad, possbily 0
 *  \param[in]      watchdog     pointer to watchdog reset function
 *  \return         actEXCEPTION_INPUT_LENGTH    aadLength greater than configured maximal aad length
 *                  actOK                        else
 *  \pre            wsSpake was initialized by actSPAKE2PInit
 *  \note           The call to this function is optional. This function can be called at any time after the call to
 *                  actSPAKE2PInit() and before the call to  actSPAKE2PCalcSharedSecret().
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSPAKE2PSetAdditionalInformation(
  VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wsSpake,
  VSECPRIM_P2VAR_PARA(actSHA256STRUCT) wsHash,
  VSECPRIM_P2CONST_PARA(actU8) infoA,
  const actLengthType infoALength,
  VSECPRIM_P2CONST_PARA(actU8) infoB,
  const actLengthType infoBLength,
  VSECPRIM_P2CONST_PARA(actU8) aad,
  const actLengthType aadLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actSPAKE2PCalcSharedSecret()
 *********************************************************************************************************************/
/*! \brief          This function calculates the common secret Ke, where Ka||Ke = Hash(TT), the MAC keys and the
 *                  confirmation MAC according to the SPAKE2+ protocol.
 *  \details        The MAC keys are not written to output buffers, but only stored in the workspace.
 *  \param[in,out]  wsSpake                 (in)  SPAKE algorithm context buffer, initialized by actSPAKE2PInit(..)
 *                                          (out) SPAKE algorithm context buffer, now storing the common values
 *                                                Z and V, the shared secret and the MAC keys
 *  \param[in,out]  wsHash                  (in)  SHA256 algorithm context buffer, a part of the overall
 *                                                esl-spake-workspace
 *                                          (out) SHA256 algorithm context structure
 *  \param[in,out]  wsHKDF                  (in)  HKDF algorithm context buffer, a part of the overall
 *                                                esl-spake-workspace
 *                                          (out) HKDF algorithm context structure
 *  \param[in,out]  wsCMAC                  (in)  CMAC algorithm context buffer, a part of the overall
 *                                                esl-spake-workspace
 *                                          (out) CMAC algorithm context structure
 *  \param[in]      pubValXCoordinate       x-coordinate of the other party's public value
 *                                          (of length getLengthOfEcPpublicKey_comp(domain))
 *  \param[in]      pubValYCoordinate       y-coordinate of the other party's public value
 *                                          (of length getLengthOfEcPpublicKey_comp(domain))
 *  \param[in]      pubValCoordinateLength  length of the buffers referenced by pubVal_x and pubVal_y
 *  \param[in,out]  sharedSecret            (in)  buffer to store the secret Ke
 *                                          (out) secret Ke
 *  \param[in,out]  confirmationMAC         (in)  buffer to store confirmation MAC
 *                                          (out) confirmation MAC
 *  \param[in]      watchdog                pointer to watchdog reset function
 *  \return         actEXCEPTION_INPUT_LENGTH  pubValLength invalid
 *                  actEXCEPTION_POINT         computation failed
 *                  actOK                      else
 *  \pre            Provided pointers must be valid.
 *                  The buffer referenced by sharedSecret must provide at least actHASH_SIZE_SHA256/2 bytes.
 *                  The buffer referenced by confirmationMAC must provide at least actAES_BLOCK_SIZE bytes.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSPAKE2PCalcSharedSecret(
  VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wsSpake,
  VSECPRIM_P2VAR_PARA(actSHA256STRUCT) wsHash,
  VSECPRIM_P2VAR_PARA(actHKDFSTRUCT) wsHKDF,
  VSECPRIM_P2VAR_PARA(actCMACAESSTRUCT) wsCMAC,
  VSECPRIM_P2CONST_PARA(actU8) pubValXCoordinate,
  VSECPRIM_P2CONST_PARA(actU8) pubValYCoordinate,
  const actLengthType pubValCoordinateLength,
  VSECPRIM_P2VAR_PARA(actU8) sharedSecret,
  VSECPRIM_P2VAR_PARA(actU8) confirmationMAC,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actSPAKE2PConfirmKey()
 *********************************************************************************************************************/
/*! \brief         This function re-calculates the other party's confirmation MAC according to the SPAKE2+ protocol.
 *  \details       This function performs the required key confirmation by comparing the given MAC to the recomputed
 *                 one based on data stored in the workspace.
 *  \param[in,out]  wsSpake          (in)  SPAKE algorithm context buffer, initialized by actSPAKE2PInit(..)
 *                                   (out) SPAKE algorithm context buffer
 *  \param[in,out]  wsCMAC           (in)  CMAC algorithm context buffer, a part of the overall esl-spake-workspace
 *                                   (out) CMAC algorithm context structure
 *  \param[in]      mac              other party's MAC to be verified
 *  \param[in,out]  recomputedMAC    (in)  buffer to store recomputed MAC
 *                                   (out) buffer storing the recomputed MAC
 *  \param[in]      watchdog         pointer to watchdog reset function
 *  \return         actVERIFICATION_FAILED   the recomputed MAC does not match the given one
 *                  actOK                    key confirmation successful
 *  \pre            Provided pointers must be valid.
 *                  The buffer referenced by recomputedMAC must provide at least actAES_BLOCK_SIZE bytes.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSPAKE2PConfirmKey(
  VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wsSpake,
  VSECPRIM_P2VAR_PARA(actCMACAESSTRUCT) wsCMAC,
  VSECPRIM_P2CONST_PARA(actU8) mac,
  VSECPRIM_P2VAR_PARA(actU8) recomputedMAC,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
}                               /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTISPAKE2P_H */

/**********************************************************************************************************************
 *  END OF FILE: actISPAKE2P.h
 *********************************************************************************************************************/
