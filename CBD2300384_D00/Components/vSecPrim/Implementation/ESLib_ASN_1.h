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
/*!         \file  ESLib_ASN_1.h
 *         \brief  ASN1 header.
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library cvActLib/ES
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

/* This header file contains some extracts from some asn.1 modules that are
   important for the library/es.
   These OIDs are mainly used inside the library to detect erroneous inputs and
   in order to produce correct outputs, that are matching currently used
   standards. */

#ifndef ESLIB_ASN_1_H
# define ESLIB_ASN_1_H

/* ansi_x9_62 */
# define OID_ansi_x9_62                                               1*40+2,       /* iso(1) mmber-body(2) */ \
                                                                      0x86, 0x48,   /* us(840)              */ \
                                                                      0xce, 0x3d     /* 10045                */
# define LengthOfOID_ansi_x9_62                                       0x05

/* ansi_x9_62_fieldType_prime                   fieldType (1) GfP(1) */
# define OID_ansi_x9_62_fieldType_prime                               OID_ansi_x9_62, 0x01,  0x01
# define LengthOfOID_ansi_x9_62_fieldType_prime                       (2+LengthOfOID_ansi_x9_62)

/* Gf2 OIDs are only added for completeness. They will be supported by the
  library modules in future versions. */

/* ansi_x9_62_fieldType_char2                   fieldType (1) Gf2(2) */
# define OID_ansi_x9_62_fieldType_char2                               OID_ansi_x9_62, 0x01,  0x02
# define LengthOfOID_ansi_x9_62_fieldType_char2                       (2+LengthOfOID_ansi_x9_62)
/* ansi_x9_62_fieldType_char2gn                                    Basis_gn (1) */
# define OID_ansi_x9_62_fieldType_char2gn                             OID_ansi_x9_62_fieldType_char2, 0x01
# define LengthOfOID_ansi_x9_62_fieldType_char2gn                     (1+LengthOfOID_ansi_x9_62_fieldType_char2)
/* ansi_x9_62_fieldType_char2tp                                    Basis_tp (2) */
# define OID_ansi_x9_62_fieldType_char2tp                             OID_ansi_x9_62_fieldType_char2, 0x02
# define LengthOfOID_ansi_x9_62_fieldType_char2tp                     (1+LengthOfOID_ansi_x9_62_fieldType_char2)
/* ansi_x9_62_fieldType_char2pp                                    Basis_pp (3) */
# define OID_ansi_x9_62_fieldType_char2pp                             OID_ansi_x9_62_fieldType_char2, 0x03
# define LengthOfOID_ansi_x9_62_fieldType_char2pp                     (1+LengthOfOID_ansi_x9_62_fieldType_char2)

# define asn1Tag_INTEGER                                              2

# define asn1Tag_OBJECT_IDENTIFIER                                    6

# define asn1Tag_OCTET_STRING                                         4

# define asn1Tag_BIT_STRING                                           3

# define asn1Tag_COMPOUND                                             0x20

# define asn1Tag_SEQUENCE                                             (0x10 | asn1Tag_COMPOUND)

/* Block type for private key operation - uses 0x00 as padding (not recommended) */
# define ASN1_PADDING_BLOCK_TYPE_PRIVATE_ZERO                         0x00u
/* Block type for private key operation - uses 0xFF as padding  (recommended) */
# define ASN1_PADDING_BLOCK_TYPE_PRIVATE                              0x01u
/* Block type for public key operation */
# define ASN1_PADDING_BLOCK_TYPE_PUBLIC                               0x02u

# define ASN1_MINIMAL_PADDING_LENGTH                                  8u

/* tlvHeader_len + minSizeOfRandomBytes */
# define ASN1_SIZEOF_MINIMAL_PADDING                                  (3u + ASN1_MINIMAL_PADDING_LENGTH)

/* hash_len + tlvHeader_len + sizeof(byte) + digestInfo_len */
# define ASN1_SIZEOF_PARAMETERS_WITH_FIXED_LENGTH                     (20u + 3u + 15u)
# define ASN1_SIZEOF_SHA256_PARAMETERS_WITH_FIXED_LENGTH              (32u + 3u + 19u)

# define ASN1_MINIMAL_SIGVER_KEY_LENGTH                               (ASN1_SIZEOF_PARAMETERS_WITH_FIXED_LENGTH + ASN1_MINIMAL_PADDING_LENGTH)
# define ASN1_MINIMAL_SIGVER_KEY_LENGTH_SHA256                        (ASN1_SIZEOF_SHA256_PARAMETERS_WITH_FIXED_LENGTH + ASN1_MINIMAL_PADDING_LENGTH)

/* DigestInfo algorithmIdentifier */
# define ASN1_DIGESTINFO_SHA1                                         {0x30u, 0x21u, 0x30u, 0x09u, 0x06u, 0x05u, 0x2bu, 0x0eu, 0x03u, 0x02u, 0x1au, 0x05u, 0x00u, 0x04u, 0x14u}
# define ASN1_DIGESTINFO_SHA256                                       {0x30u, 0x31u, 0x30u, 0x0du, 0x06u, 0x09u, 0x60u, 0x86u, 0x48u, 0x01u, 0x65u, 0x03u, 0x04u, 0x02u, 0x01u, 0x05u, 0x00u, 0x04u, 0x20u}
# define ASN1_DIGESTINFO_RIPEMD160                                    {0x30u, 0x21u, 0x30u, 0x09u, 0x06u, 0x05u, 0x2bu, 0x24u, 0x03u, 0x02u, 0x01u, 0x05u, 0x00u, 0x04u, 0x14u}

#endif /* ESLIB_ASN_1_H */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_ASN_1.h
 *********************************************************************************************************************/
