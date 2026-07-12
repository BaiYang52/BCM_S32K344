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
/*!        \file  ESLib_t.h
 *        \brief  Basic type definitions.
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

#ifndef ESLIB_T_H
# define ESLIB_T_H

# include "actConfig.h"
# include "actITypes.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define ESL_UNCOMPRESSED_POINT_ENCODING_BYTE                         (0x04u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

# define Esl_BitsToBytes(x)                                           ((x) >> 3)
# define Esl_BytesToBits(x)                                           ((x) << 3)

# define Esl_ValueIsEven(x)                                           (((x % 2u) == 0u) ? TRUE:FALSE)
# define Esl_ValueIsOdd(x)                                            (((x % 2u) == 1u) ? TRUE:FALSE)

# define Esl_DivideByTwo(x)                                           ((x) >> 1)

/*****************************************************************************
 * Basic types and constants
 *****************************************************************************/

# define ESL_BITS_PER_DIGIT                                           (ACT_PLATFORM_BITS_PER_DIGIT)

typedef actPlatformUint8 eslt_Size8;
typedef actPlatformUint16 eslt_Size16;
typedef actPlatformUint32 eslt_Size32;
# ifdef ACT_PLATFORM_UINT64_AVAILABLE
typedef actPlatformUint64 eslt_Size64;
# endif

typedef eslt_Size8 eslt_Byte;
# if (vSecPrim_GetSizeOfEsltLengthOfGeneral() == VSECPRIM_SIZE_UINT32) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
typedef eslt_Size32 eslt_Length;
# else
typedef eslt_Size16 eslt_Length;
# endif
typedef eslt_Size8 eslt_Mode;
typedef eslt_Size16 eslt_WSStatus;
typedef eslt_Size32 eslt_WSCheckSum;

/* Define kept for compatibility reasons (CPL) */
# define eslt_pRomByte                                                VSECPRIM_P2CONST_PARA(eslt_Byte)

# if defined (ESL_MAX_KEY_SIZE_ECP) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
# else
#  if (vSecPrim_GetECPMaxKeySizeOfGeneral() == VSECPRIM_ECP_MAX_KEY_SIZE_160) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#   define ESL_MAX_KEY_SIZE_ECP                                       160
#  elif (vSecPrim_GetECPMaxKeySizeOfGeneral() == VSECPRIM_ECP_MAX_KEY_SIZE_192) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#   define ESL_MAX_KEY_SIZE_ECP                                       192
#  elif (vSecPrim_GetECPMaxKeySizeOfGeneral() == VSECPRIM_ECP_MAX_KEY_SIZE_224) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#   define ESL_MAX_KEY_SIZE_ECP                                       224
#  elif (vSecPrim_GetECPMaxKeySizeOfGeneral() == VSECPRIM_ECP_MAX_KEY_SIZE_256)
#   define ESL_MAX_KEY_SIZE_ECP                                       256
#  elif (vSecPrim_GetECPMaxKeySizeOfGeneral() == VSECPRIM_ECP_MAX_KEY_SIZE_320) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#   define ESL_MAX_KEY_SIZE_ECP                                       320
#  elif (vSecPrim_GetECPMaxKeySizeOfGeneral() == VSECPRIM_ECP_MAX_KEY_SIZE_384)
#   define ESL_MAX_KEY_SIZE_ECP                                       384
#  elif (vSecPrim_GetECPMaxKeySizeOfGeneral() == VSECPRIM_ECP_MAX_KEY_SIZE_512) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#   define ESL_MAX_KEY_SIZE_ECP                                       512
#  elif (vSecPrim_GetECPMaxKeySizeOfGeneral() == VSECPRIM_ECP_MAX_KEY_SIZE_521) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#   define ESL_MAX_KEY_SIZE_ECP                                       521
#  else
#   error Invalid maximum Key size for ECP selected
#  endif
# endif /* ESL_MAX_KEY_SIZE_ECP */

# if defined (ESL_RSA_KEYSIZE) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
# else
#  if (vSecPrim_GetRSAMaxKeySizeOfGeneral() == VSECPRIM_RSA_MAX_KEY_SIZE_512) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#   define ESL_RSA_KEYSIZE                                            512u
#  elif (vSecPrim_GetRSAMaxKeySizeOfGeneral() == VSECPRIM_RSA_MAX_KEY_SIZE_1024) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#   define ESL_RSA_KEYSIZE                                            1024u
#  elif (vSecPrim_GetRSAMaxKeySizeOfGeneral() == VSECPRIM_RSA_MAX_KEY_SIZE_1536) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#   define ESL_RSA_KEYSIZE                                            1536u
#  elif (vSecPrim_GetRSAMaxKeySizeOfGeneral() == VSECPRIM_RSA_MAX_KEY_SIZE_2048)
#   define ESL_RSA_KEYSIZE                                            2048u
#  elif (vSecPrim_GetRSAMaxKeySizeOfGeneral() == VSECPRIM_RSA_MAX_KEY_SIZE_3072) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#   define ESL_RSA_KEYSIZE                                            3072u
#  elif (vSecPrim_GetRSAMaxKeySizeOfGeneral() == VSECPRIM_RSA_MAX_KEY_SIZE_4096)
#   define ESL_RSA_KEYSIZE                                            4096u
#  else
#   error Invalid maximum Key size for RSA selected
#  endif
# endif /* ESL_RSA_KEYSISZE */

# if (VSECPRIM_RSA_V15_ENABLED == STD_ON) \
     || (VSECPRIM_RSA_OAEP_SHA1_ENABLED == STD_ON) \
     || (VSECPRIM_RSA_OAEP_SHA2_256_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF xf xf xf */
#  define VSECPRIM_RSA_CIPHER_ENABLED                                 STD_ON
# else
#  define VSECPRIM_RSA_CIPHER_ENABLED                                 STD_OFF
# endif

# if (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON) \
     || (VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) \
     || (VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON) \
     || (VSECPRIM_RSA_V15_RIPEMD160_ENABLED == STD_ON) \
     || (VSECPRIM_RSA_V15_SHA2_256_ENABLED == STD_ON) \
     || (VSECPRIM_RSA_V15_SHA1_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG TF xf xf tf xf xf xf */
#  define VSECPRIM_RSA_SIGNATURE_ENABLED                              STD_ON
# else
#  define VSECPRIM_RSA_SIGNATURE_ENABLED                              STD_OFF
# endif

# if ( (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON) \
     || (VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) \
     || (VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON)) \
     && ((VSECPRIM_RSA_V15_RIPEMD160_ENABLED == STD_OFF) \
     && (VSECPRIM_RSA_V15_SHA2_256_ENABLED == STD_OFF) \
     && (VSECPRIM_RSA_V15_SHA1_ENABLED == STD_OFF)) /* COV_VSECPRIM_NO_SAFE_CONFIG TF xf xf tf tx tx tx */
#  define VSECPRIM_RSAPSSONLY_SIGNATURE_ENABLED                       STD_ON
# else
#  define VSECPRIM_RSAPSSONLY_SIGNATURE_ENABLED                       STD_OFF
# endif

# if (VSECPRIM_RSA_CIPHER_ENABLED == STD_ON) \
     || (VSECPRIM_RSA_V15_RIPEMD160_ENABLED == STD_ON) \
     || (VSECPRIM_RSA_V15_SHA2_256_ENABLED == STD_ON) \
     || (VSECPRIM_RSA_V15_SHA1_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF xf xf xf xf */
#  define VSECPRIM_RSA_ENABLED                                        STD_ON
# else
#  define VSECPRIM_RSA_ENABLED                                        STD_OFF
# endif

# if (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  define VSECPRIM_RSA_HASHALGORITHM_RIPEMD160_ENABLED                STD_ON
# else
#  define VSECPRIM_RSA_HASHALGORITHM_RIPEMD160_ENABLED                STD_OFF
# endif

# if (VSECPRIM_RSA_OAEP_SHA1_ENABLED == STD_ON) || ( VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF xf xf */
#  define VSECPRIM_RSA_HASHALGORITHM_SHA1_ENABLED                     STD_ON
# else
#  define VSECPRIM_RSA_HASHALGORITHM_SHA1_ENABLED                     STD_OFF
# endif

# if (VSECPRIM_RSA_OAEP_SHA2_256_ENABLED == STD_ON) || ( VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG TF xf tf */
#  define VSECPRIM_RSA_HASHALGORITHM_SHA2_256_ENABLED                 STD_ON
# else
#  define VSECPRIM_RSA_HASHALGORITHM_SHA2_256_ENABLED                 STD_OFF
# endif

# if ((VSECPRIM_RSA_OAEP_SHA1_ENABLED == STD_ON) || (VSECPRIM_RSA_OAEP_SHA2_256_ENABLED == STD_ON)) /* COV_VSECPRIM_NO_SAFE_CONFIG XF xf xf */
#  define VSECPRIM_RSA_OAEP_ENABLED                                   STD_ON
# else
#  define VSECPRIM_RSA_OAEP_ENABLED                                   STD_OFF
# endif

# if ((VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) || ( VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON)) /* COV_VSECPRIM_NO_SAFE_CONFIG TF xf xf tf */
#  define VSECPRIM_RSA_PSS_ENABLED                                    STD_ON
# else
#  define VSECPRIM_RSA_PSS_ENABLED                                    STD_OFF
# endif

# if ((VSECPRIM_RSA_OAEP_ENABLED == STD_ON) || (VSECPRIM_RSA_V15_ENABLED == STD_ON) || (VSECPRIM_RSA_V15_RIPEMD160_ENABLED == STD_ON) || (VSECPRIM_RSA_V15_SHA2_256_ENABLED == STD_ON)  || (VSECPRIM_RSA_V15_SHA1_ENABLED == STD_ON)) /* COV_VSECPRIM_NO_SAFE_CONFIG XF xf xf xf xf xf */
#  define VSECPRIM_RSACRT                                             STD_ON
# else
#  define VSECPRIM_RSACRT                                             STD_OFF
# endif

/*****************************************************************************
 * Library version
 *****************************************************************************/

typedef struct
{
  VSECPRIM_P2ROMCONST(VSECPRIM_NONE, char) parameter;
  eslt_Size16 value;
}eslt_Version;

/* ******************************************************************************
 * Algorithm specific types and constants
 ****************************************************************************** */

typedef eslt_Mode eslt_PaddingMode;
# define ESL_PM_PKCS5                                                 0x01u
# define ESL_PM_OFF                                                   0x02u
# define ESL_PM_ONEWITHZEROES                                         0x04u

typedef eslt_Mode eslt_HashAlgorithm;
# define ESL_HA_SHA1                                                  (0x00u)
# define ESL_HA_SHA2_224                                              (0x01u)
# define ESL_HA_SHA2_256                                              (0x02u)
# define ESL_HA_SHA2_384                                              (0x03u)
# define ESL_HA_SHA2_512                                              (0x04u)
# define ESL_HA_SHA2_512_224                                          (0x05u)
# define ESL_HA_SHA2_512_256                                          (0x06u)
/* The SHA3 identifiers need to be direct successors to each other! This is used in vSecPrim_initHash(). */
# define ESL_HA_SHA3_224                                              (0x07u)
# define ESL_HA_SHA3_256                                              (0x08u)
# define ESL_HA_SHA3_384                                              (0x09u)
# define ESL_HA_SHA3_512                                              (0x0Au)
/* The SHAKE identifiers need to be direct successors to each other! This is used in vSecPrim_initHash(). */
# define ESL_HA_SHAKE_128                                             (0x0Bu)
# define ESL_HA_SHAKE_256                                             (0x0Cu)
# define ESL_HA_RMD160                                                (0x0Du)
# define ESL_HA_MD5                                                   (0x0Eu)

typedef eslt_Mode eslt_BlockMode;
# define ESL_BM_ECB                                                   0x00u
# define ESL_BM_CBC                                                   0x01u
/*# define ESL_BM_CFB                        0x02u
 # define ESL_BM_OFB                        0x03u */
# define ESL_BM_CTR                                                   0x04u

typedef eslt_Mode eslt_DRBGMode;
# define ESL_DRBGMODE_CTRDRBG_AES128_DF                               (actDRBGMODE_CTRDRBG_AES128_DF)
# define ESL_DRBGMODE_CTRDRBG_AES128_NODF                             (actDRBGMODE_CTRDRBG_AES128_NODF)
# define ESL_DRBGMODE_CTRDRBG_AES256_DF                               (actDRBGMODE_CTRDRBG_AES256_DF)
# define ESL_DRBGMODE_CTRDRBG_AES256_NODF                             (actDRBGMODE_CTRDRBG_AES256_NODF)
# define ESL_DRBGMODE_HASHDRBG_SHA512                                 (actDRBGMODE_HASHDRBG_SHA512)

typedef eslt_Size32 eslt_DRBGReseedCounterType;

/* DRBG seed state */
typedef eslt_Byte eslt_DRBGSeedStatusType;
# define ESL_DRBG_SEED_STATUS_UNSEEDED                                (actDRBG_SEED_STATE_UNSEEDED)
# define ESL_DRBG_SEED_STATUS_SEEDED                                  (actDRBG_SEED_STATE_SEEDED)
# define ESL_DRBG_SEED_STATUS_NEED_RESEED                             (actDRBG_SEED_STATE_NEED_RESEED)

/* SPAKE2P types and constants */

typedef eslt_Size32 eslt_SPAKE2PMode;
# define ESL_SPAKE2P_CURVE_P256                                       (actSPAKE2P_CURVE_P256)
# define ESL_SPAKE2P_HASHFCT_SHA256                                   (actSPAKE2P_HASHFCT_SHA256)
# define ESL_SPAKE2P_KDF_HKDF_HMAC_SHA256                             (actSPAKE2P_KDF_HKDF_HMAC_SHA256)
# define ESL_SPAKE2P_MAC_CMAC128                                      (actSPAKE2P_MAC_CMAC128)

/* Key A first means that AAD -> KDF(nil, Ka, "ConfirmationKeys" || AAD) = KcA || KcB (newer version, according to www.tools.ietf.org/id/draft-bar-cfrg-spake2plus-00.html)
 * Key B first means that AAD -> KDF(nil, Ka, "ConfirmationKeys" || AAD) = KcB || KcA (older version, according to CCC R2 v1.0.0) */
# define ESL_SPAKE2P_VERSION_KEY_A_FIRST                              (actSPAKE2P_VERSION_KEY_A_FIRST)
# define ESL_SPAKE2P_VERSION_KEY_B_FIRST                              (actSPAKE2P_VERSION_KEY_B_FIRST)

/* Table of cipher suites for SPAKE2+, taken from www.tools.ietf.org/id/draft-bar-cfrg-spake2plus-00.html
   +------------------+---------------+-------------+------------------+
   |        G         |      Hash     |     KDF     |       MAC        |
   +------------------+---------------+-------------+------------------+
   |      P-256       |     SHA256    |     HKDF    |  HMAC [RFC2104]  |
   |                  |   [RFC6234]   |  [RFC5869]  |                  |
   |                  |               |             |                  |
   |      P-256       |     SHA512    |     HKDF    |  HMAC [RFC2104]  |
   |                  |   [RFC6234]   |  [RFC5869]  |                  |
   |                  |               |             |                  |
   |      P-384       |     SHA256    |     HKDF    |  HMAC [RFC2104]  |
   |                  |   [RFC6234]   |  [RFC5869]  |                  |
   |                  |               |             |                  |
   |      P-384       |     SHA512    |     HKDF    |  HMAC [RFC2104]  |
   |                  |   [RFC6234]   |  [RFC5869]  |                  |
   |                  |               |             |                  |
   |      P-512       |     SHA512    |     HKDF    |  HMAC [RFC2104]  |
   |                  |   [RFC6234]   |  [RFC5869]  |                  |
   |                  |               |             |                  |
   |   edwards25519   |     SHA256    |     HKDF    |  HMAC [RFC2104]  |
   |    [RFC7748]     |   [RFC6234]   |  [RFC5869]  |                  |
   |                  |               |             |                  |
   |    edwards448    |     SHA512    |     HKDF    |  HMAC [RFC2104]  |
   |    [RFC7748]     |   [RFC6234]   |  [RFC5869]  |                  |
   |                  |               |             |                  |
   |      P-256       |     SHA256    |     HKDF    |   CMAC-AES-128   |
   |                  |   [RFC6234]   |  [RFC5869]  |    [RFC4493]     |
   |                  |               |             |                  |
   |      P-256       |     SHA512    |     HKDF    |   CMAC-AES-128   |
   |                  |   [RFC6234]   |  [RFC5869]  |    [RFC4493]     |
   +------------------+---------------+-------------+------------------+   */

/* Ciphersuite defined in row 8 in table above, starting from row 1 */
# define ESL_SPAKE2P_MODE_CIPHERSUITE_8_1                             (ESL_SPAKE2P_CURVE_P256 + ESL_SPAKE2P_VERSION_KEY_A_FIRST + ESL_SPAKE2P_HASHFCT_SHA256 + ESL_SPAKE2P_KDF_HKDF_HMAC_SHA256 + ESL_SPAKE2P_MAC_CMAC128)
# define ESL_SPAKE2P_MODE_CIPHERSUITE_8_2                             (ESL_SPAKE2P_CURVE_P256 + ESL_SPAKE2P_VERSION_KEY_B_FIRST + ESL_SPAKE2P_HASHFCT_SHA256 + ESL_SPAKE2P_KDF_HKDF_HMAC_SHA256 + ESL_SPAKE2P_MAC_CMAC128)

/* algorithm states */
# define ESL_SPAKE2P_PREAMBLE_DATA_NOT_SET                            (0x00u)
# define ESL_SPAKE2P_PREAMBLE_DATA_SET                                (0x01u)
# define ESL_SPAKE2P_PUBLIC_VALUE_CALCULATED                          (0x02u)
# define ESL_SPAKE2P_SHARED_SECRET_CALCULATED                         (0x03u)

/* HKDF variant mode */
typedef eslt_Byte eslt_HKDFMode;
# define ESL_HKDF_ONE_STEP_VARIANT                                    (actHKDF_ONESTEPMETHOD)
# define ESL_HKDF_TWO_STEP_VARIANT                                    (actHKDF_TWOSTEPMETHOD)

/* ALGORITHM SPECIFIC SIZES */
# define ESL_SIZEOF_AES_BLOCK                                         16u

# define ESL_SIZEOF_AES128_KEY                                        16u
# define ESL_SIZEOF_AES128_BLOCK                                      ESL_SIZEOF_AES_BLOCK

# define ESL_SIZEOF_AES192_KEY                                        24u
# define ESL_SIZEOF_AES192_BLOCK                                      ESL_SIZEOF_AES_BLOCK

# define ESL_SIZEOF_AES256_KEY                                        32u
# define ESL_SIZEOF_AES256_BLOCK                                      ESL_SIZEOF_AES_BLOCK

# define ESL_SIZEOF_SIPHASH_KEY                                       16u

# define ESL_SIZEOF_DES_KEY                                           8u
# define ESL_SIZEOF_DES_BLOCK                                         8u
# define ESL_SIZEOF_2TDES_KEY                                         16u
# define ESL_SIZEOF_3TDES_KEY                                         24u

# define ESL_SIZEOF_RC2_KEY                                           16u
# define ESL_SIZEOF_RC2_EFF_KEY                                       16u
# define ESL_SIZEOF_RC2_BLOCK                                         8u

# define ESL_SIZEOF_GCM_BLOCK                                         ESL_SIZEOF_AES_BLOCK
# define ESL_SIZEOF_GCM_TAG                                           ESL_SIZEOF_AES_BLOCK

# define ESL_SIZEOF_CCM_TAG                                           ESL_SIZEOF_AES_BLOCK

# define ESL_SIZEOF_SHA1_BLOCK                                        64u
# define ESL_SIZEOF_SHA1_DIGEST                                       20u

# define ESL_SIZEOF_RIPEMD160_BLOCK                                   64u
# define ESL_SIZEOF_RIPEMD160_DIGEST                                  20u

# define ESL_SIZEOF_SHA256_BLOCK                                      64u
# define ESL_SIZEOF_SHA224_DIGEST                                     28u
# define ESL_SIZEOF_SHA256_DIGEST                                     32u

# define ESL_SIZEOF_SHA512_BLOCK                                      128u
# define ESL_SIZEOF_SHA512_224_DIGEST                                 ESL_SIZEOF_SHA224_DIGEST
# define ESL_SIZEOF_SHA512_256_DIGEST                                 ESL_SIZEOF_SHA256_DIGEST
# define ESL_SIZEOF_SHA384_DIGEST                                     48u
# define ESL_SIZEOF_SHA512_DIGEST                                     64u

# define ESL_SIZEOF_KECCAK_STATE                                      (1600u / 8u)

# define ESL_SIZEOF_SHA3_224_DIGEST                                   (224u / 8u)
# define ESL_SIZEOF_SHA3_256_DIGEST                                   (256u / 8u)
# define ESL_SIZEOF_SHA3_384_DIGEST                                   (384u / 8u)
# define ESL_SIZEOF_SHA3_512_DIGEST                                   (512u / 8u)
# define ESL_SIZEOF_SHAKE128_DIGEST                                   (128u * 2u / 8u)
# define ESL_SIZEOF_SHAKE256_DIGEST                                   (256u * 2u / 8u)

# define ESL_SIZEOF_SHAKE128_STRENGTH                                 (128u)
# define ESL_SIZEOF_SHAKE256_STRENGTH                                 (256u)

# define ESL_SIZEOF_MD5_BLOCK                                         64u
# define ESL_SIZEOF_MD5_DIGEST                                        16u

# define ESL_SIZEOF_Ed25519_KEY                                       32u
# define ESL_SIZEOF_Ed25519_SECRET_KEY                                ESL_SIZEOF_Ed25519_KEY
# define ESL_SIZEOF_Ed25519_PUBLIC_KEY                                ESL_SIZEOF_Ed25519_KEY
# define ESL_SIZEOF_Ed25519_SIGNATURE                                 64u
# define ESL_INSTANCE_Ed25519                                         0u    /* MUST match actEd25519pure */
# define ESL_INSTANCE_Ed25519ctx                                      1u    /* MUST match actEd25519ctx */
# define ESL_INSTANCE_Ed25519ph                                       2u    /* MUST match actEd25519ph */

# define ESL_SIZEOF_X25519_KEY                                        32u
# define ESL_SIZEOF_X25519_PRIVATE_KEY                                ESL_SIZEOF_X25519_KEY
# define ESL_SIZEOF_X25519_PUBLIC_KEY                                 ESL_SIZEOF_X25519_KEY
# define ESL_SIZEOF_X25519_SHARED_SECRET                              ESL_SIZEOF_X25519_KEY

# define ESL_Curve25519                                               25519u
# define ESL_Curve448                                                 448u

# define ESL_SIZEOF_ChaCha20_KEY                                      32u
# define ESL_SIZEOF_ChaCha20_NONCE                                    12u

# define ESL_SIZEOF_Poly1305_KEY                                      32u
# define ESL_SIZEOF_Poly1305_TAG                                      16u

# define ESL_SIZEOF_RNGFIPS186_STATE                                  (ESL_SIZEOF_SHA1_DIGEST)

# define ESL_KDF2_DEFAULT_ITERATION_COUNT                             1u

# define ESL_SIZEOF_RSA_PSS_PADDING1                                  8u

/* Seed length for DRBG, value in BYTE */
# define ESL_CTRDRBG_AES128_SEEDLEN                                   32u  /* value in byte */
# define ESL_CTRDRBG_AES256_SEEDLEN                                   48u  /* value in byte */
# define ESL_HASHDRBG_SHA512_SEED_LEN                                 111u /* value in byte */

# define ESL_SIZEOF_CTRDRBG_AES128_KEY                                ESL_SIZEOF_AES128_KEY
# define ESL_SIZEOF_CTRDRBG_AES256_KEY                                ESL_SIZEOF_AES256_KEY
# define ESL_SIZEOF_CTRDRBG_BLOCK                                     ESL_SIZEOF_AES_BLOCK

/* ECC domain data types */

/* Variables of type eslt_EccDomain shall contain the elliptic curve domain parameters as defined in X9.62, namely
 * - the field prime p,
 * - the curve parameters a and b,
 * - the base point G,
 * - the order n,
 * - and the cofactor h.
 * This information shall be represented in the following format:
 * TLV(SEQUENCE)
 * {
 *   TLV(INTEGER(version)),
 *   TLV(SEQUENCE)
 *   {
 *     TLV(OBJECT_IDENTIFIER(prime OID)),
 *     TLV(INTEGER(p)
 *   },
 *   TLV(SEQUENCE)
 *   {
 *     TLV(OCTETSTRING(a)),
 *     TLV(OCTETSTRING(b))
 *   },
 *   TLV(OCTETSTRING(G)),
 *   TLV(INTEGER(n)),
 *   TLV(INTEGER(h))
 * },
 * where TLV is short for (Tag - Length - Value) and shall be encoded using DER (Distinguished Encoding Rules).
 * It is assumed that the base point representation is uncompressed, i.e., both coordinates are contained
 * in TLV(OCTETSTING(G)).
 * The length of the overall sequence is thus contained in the length-octets of the first TLV(SEQUENCE) object.
 * The length is not passed separately, when a variable of type eslt_EccDomain is passed to a function and not
 * stored in another global variable, either.
 */
typedef eslt_Byte eslt_EccDomain;

/* Variables of type eslt_EccDomainExt shall contain additional elliptic curve domain parameters as defined
 *  in X9.62, namely
 * - the field prime Montgomery constant p_RR,
 * - the negative field inverse p_bar (the low digit of -(p^(-1))),
 * - the Montgomery representations of the curve parameters a and b, a_R and b_R,
 * - the base point in Montgomery representation G_R,
 * - the order Montgomery constant n_RR and
 * - the negative order inverse n_bar (the low digit of -(n^(-1))).
 * This information shall be represented in the following format:
 * TLV(SEQUENCE)
 * {
 *  TLV(SEQUENCE)
 *  {
 *    TLV(OCTETSTRING(p_RR)),
 *    TLV(OCTETSTRING(p_bar)),
 *    TLV(SEQUENCE)
 *    {
 *      TLV(OCTETSTRING(a_R)),
 *      TLV(OCTETSTRING(b_R))
 *    },
 *    TLV(OCTETSTRING(G_R)),
 *  }
 *  TLV(SEQUENCE)
 *  {
 *    TLV(OCTETSTRING(n_RR)),
 *    TLV(OCTETSTRING(n_bar)),
 *  }
 * }
 * and shall be encoded using DER (Distinguished Encoding Rules).
 * It is assumed that the base point representation is uncompressed, i.e., both coordinates are contained
 * in TLV(OCTETSTING(G_R)).
 * The length of the overall sequence is thus contained in the length-octets of the first TLV(SEQUENCE) object.
 * The length is not passed separately, when a variable of type eslt_EccDomainExt is passed to a function and not
 * stored in another global variable, either.
 */
typedef eslt_Byte eslt_EccDomainExt;

/* Variables of type eslt_EccSpeedUpExt shall contain precomputed points on the elliptic curve specified by
 *  eslt_EccDomain (and possibly also by eslt_EccDomainExt) as defined in X9.62, namely
 * - the group parameter, indicating how many precomputed points are provided and
 * - the precomputed points.
 * This information shall be represented in the following format:
 * TLV(SEQUENCE)
 * {
 *   TLV(OCTETSTRING(groups)),
 *   TLV(SEQUENCE)
 *   {
 *     TLV(OCTETSTRING(2*G_R)),
 *     ...
 *     [2^groups - 1 precomputed points and correction point D_R]
 *     ...
 *     TLV(OCTETSTRING(D_R))
 *   },
 * }
 * and shall be encoded using DER (Distinguished Encoding Rules).
 * It is assumed that each point representation is uncompressed, i.e., both coordinates are contained
 * in TLV(OCTETSTING(point)).
 * The length of the overall sequence is thus contained in the length-octets of the first TLV(SEQUENCE) object.
 * The length is not passed separately, when a variable of type eslt_SpeedUpExt is passed to a function and not
 * stored in another global variable, either.
 */
typedef eslt_Byte eslt_EccSpeedUpExt;

/* ******************************************************************************
 * Work space types and constants
 ****************************************************************************** */

/* =============================================================================
 = Basic work space type definitions
 ============================================================================= */
typedef VSECPRIM_P2FUNC(VSECPRIM_NONE, void, esl_WatchdogFuncPtr)(void);

/* NOTE: Any algorithm only accepts Workspaces that have initialized correctly! */
typedef struct
{
  eslt_Length size; /* for overflow-checks */
  eslt_WSStatus status; /* for integrity checks */
  esl_WatchdogFuncPtr watchdog; /* watchdog function */
  eslt_WSCheckSum checkSum; /* for manipulation checks */
}eslt_WorkSpaceHeader;

/* =============================================================================
 = ensure correct alignment of data
 ============================================================================= */

# ifdef ACT_PLATFORM_ALIGNMENT
#  if (ACT_PLATFORM_ALIGNMENT == 1u) || (ACT_PLATFORM_ALIGNMENT == 2u) || (ACT_PLATFORM_ALIGNMENT == 4u) || (ACT_PLATFORM_ALIGNMENT == 8u)

#   define ACT_ALIGN(size)                                            ((((size) +((ACT_PLATFORM_ALIGNMENT) - 1u))/(ACT_PLATFORM_ALIGNMENT))*(ACT_PLATFORM_ALIGNMENT))
#  else
#   error Unknown alignment
#  endif
# else
#  error Alignment is not specified
# endif

# define ACT_MAX_ALIGN_OFFSET                                         (ACT_PLATFORM_ALIGNMENT-1u)

/* =============================================================================
 = Symmetric algorithm work space sizes
 ============================================================================= */

/* AES128 workSpace sizes */
/* For (initTransformBlock, transformBlock, transformBlock, ..) calls */
# define ESL_MAXSIZEOF_WS_AES128BLOCK                                 (260u)
/* For (init, transform(input, len), ... , transform(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_AES128                                      (sizeof(actAESSTRUCT))
/* For (init, transform(block), ... , transform(block)) sequences */
# define ESL_MINSIZEOF_WS_AES128                                      (sizeof(actAESSTRUCT))

/* AES192 workSpace sizes */
/* For (initTransformBlock, transformBlock, transformBlock, ..) calls */
# define ESL_MAXSIZEOF_WS_AES192BLOCK                                 (260u)
/* For (init, transform(input, len), ... , transform(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_AES192                                      (sizeof(actAESSTRUCT))
/* For (init, transform(block), ... , transform(block)) sequences */
# define ESL_MINSIZEOF_WS_AES192                                      (sizeof(actAESSTRUCT))

/* AES256 workSpace sizes */
/* For (initTransformBlock, transformBlock, transformBlock, ..) calls */
# define ESL_MAXSIZEOF_WS_AES256BLOCK                                 (260u)
/* For (init, transform(input, len), ... , transform(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_AES256                                      (sizeof(actAESSTRUCT))
/* For (init, transform(block), ... , transform(block)) sequences */
# define ESL_MINSIZEOF_WS_AES256                                      (sizeof(actAESSTRUCT))

/* DES workSpace sizes */
/* For (initTransformBlock, transformBlock, transformBlock, ..) calls */
# define ESL_MAXSIZEOF_WS_DESBLOCK                                    (200u)
/* For (init, transform(input, len), ... , transform(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_DES                                         (ESL_MAXSIZEOF_WS_DESBLOCK + (2u * ESL_SIZEOF_DES_BLOCK) + 8u)
/* For (init, transform(block), ... , transform(block)) sequences */
# define ESL_MINSIZEOF_WS_DES                                         (ESL_MAXSIZEOF_WS_DES - ESL_SIZEOF_DES_BLOCK)

/* TDES workSpace sizes */
/* For (initTransformBlock, transformBlock, transformBlock, ..) calls */
# define ESL_MAXSIZEOF_WS_TDESBLOCK                                   (500u)
/* For (init, transform(input, len), ... , transform(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_TDES                                        (ESL_MAXSIZEOF_WS_TDESBLOCK + (2u * ESL_SIZEOF_DES_BLOCK) + 8u)
/* For (init, transform(block), ... , transform(block)) sequences */
# define ESL_MINSIZEOF_WS_TDES                                        (ESL_MAXSIZEOF_WS_TDES - ESL_SIZEOF_DES_BLOCK)

/* RC2 workSpace sizes */
/* For (initTransformBlock, transformBlock, transformBlock, ..) calls */
# define ESL_MAXSIZEOF_WS_RC2BLOCK                                    (200u)
/* For (init, transform(input, len), ... , transform(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_RC2                                         (ESL_MAXSIZEOF_WS_RC2BLOCK + (3u * ESL_SIZEOF_RC2_BLOCK) + 8u)
/* For (init, transform(block), ... , transform(block)) sequences */
# define ESL_MINSIZEOF_WS_RC2                                         (ESL_MAXSIZEOF_WS_RC2 - ESL_SIZEOF_RC2_BLOCK)

/* GCM and GMAC workSpace sizes */
/* For (init, update(input, len), ... , update(input, len), finalize) sequences
 ##########################################################################
 ##  Please read the "library/es Porting Guide" for more information!
 ########################################################################## */
/* # define ESL_SIZEOF_GHASH_SPEEDUP            0 */ /*  for actGHASH_SPEED_UP set to 0  (see "Porting Guide") */
# define ESL_SIZEOF_GHASH_SPEEDUP                                     (256u)    /*  for actGHASH_SPEED_UP set to 1  (see "Porting Guide") */
/* # define ESL_SIZEOF_GHASH_SPEEDUP         4096 */ /*  for actGHASH_SPEED_UP set to 4  (see "Porting Guide") */
# define ESL_SIZEOF_WS_GHASH                                          ((3u * ESL_SIZEOF_GCM_TAG) + 4u + ESL_SIZEOF_GHASH_SPEEDUP)
# define ESL_MAXSIZEOF_WS_GMAC                                        (sizeof(actGCMSTRUCT))
# define ESL_MAXSIZEOF_WS_GCM                                         (sizeof(actGCMSTRUCT))

/* SHA-1 workSpace sizes */
/* For (init, update(input, len), ... , update(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_SHA1                                        (sizeof(actSHASTRUCT))
/* For (init, update(block), ... , update(block)) sequences */
# define ESL_MINSIZEOF_WS_SHA1                                        (sizeof(actSHASTRUCT))

/* SHA-224 workSpace sizes */
/* For (init, update(input, len), ... , update(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_SHA224                                      (sizeof(actSHA224STRUCT))    /*  same as SHA256!! */
/* For (init, update(block), ... , update(block)) sequences */
# define ESL_MINSIZEOF_WS_SHA224                                      (sizeof(actSHA224STRUCT))    /*  same as SHA256!! */

/* SHA-256 workSpace sizes */
/* For (init, update(input, len), ... , update(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_SHA256                                      (sizeof(actSHA256STRUCT))
/* For (init, update(block), ... , update(block)) sequences */
# define ESL_MINSIZEOF_WS_SHA256                                      (sizeof(actSHA256STRUCT))

/* SHA-512 workSpace sizes */
/* For (init, update(input, len), ... , update(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_SHA512                                      (sizeof(actSHA512STRUCT))
/* For (init, update(block), ... , update(block)) sequences */
# define ESL_MINSIZEOF_WS_SHA512                                      (sizeof(actSHA512STRUCT))

/* SHA-512/224 workSpace sizes */
/* For (init, update(input, len), ... , update(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_SHA512_224                                  (sizeof(actSHA512_224STRUCT))    /*  same as SHA512!! */
/* For (init, update(block), ... , update(block)) sequences */
# define ESL_MINSIZEOF_WS_SHA512_224                                  (sizeof(actSHA512_224STRUCT))    /*  same as SHA512!! */

/* SHA-512/256 workSpace sizes */
/* For (init, update(input, len), ... , update(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_SHA512_256                                  (sizeof(actSHA512_256STRUCT))    /*  same as SHA512!! */
/* For (init, update(block), ... , update(block)) sequences */
# define ESL_MINSIZEOF_WS_SHA512_256                                  (sizeof(actSHA512_256STRUCT))    /*  same as SHA512!! */

/* SHA-384 workSpace sizes */
/* For (init, update(input, len), ... , update(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_SHA384                                      (sizeof(actSHA384STRUCT))    /*  same as SHA512!! */
/* For (init, update(block), ... , update(block)) sequences */
# define ESL_MINSIZEOF_WS_SHA384                                      (sizeof(actSHA384STRUCT))    /*  same as SHA512!! */

/* SHA3 workSpace sizes */
/* valid for either bit length (224, 256, 384, 512) */
/*  For (init, update(input, len), ... ,update(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_SHA3                                        (sizeof(actKECCAKcSTRUCT))

/* SHAKExxx workSpace sizes */
/* For (init, update(input, len), ... ,update(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_SHAKE                                       ESL_MAXSIZEOF_WS_SHA3        /*  same as SHA-3!! */

/* MD5 workSpace sizes */
/* For (init, update(input, len), ... , update(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_MD5                                         (ACT_ALIGN (ESL_SIZEOF_MD5_BLOCK + ESL_SIZEOF_MD5_DIGEST + 12u))
/* For (init, update(block), ... , update(block)) sequences */
# define ESL_MINSIZEOF_WS_MD5                                         (ESL_MAXSIZEOF_WS_MD5 - ESL_SIZEOF_MD5_BLOCK)

/* HMACSHA-1 workSpace sizes */
/* For (init, update(input, len), ... , update(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_HMACSHA1                                    (sizeof(actHASHMACSTRUCT))

/* HMACSHA-256 workSpace sizes */
/* For (init, update(input, len), ... , update(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_HMACSHA256                                  (sizeof(actHASHMACSHA256STRUCT))

/* HMACSHA-384 workSpace sizes */
/* For (init, update(input, len), ... , update(input, len), finalize) sequences */
# define ESL_MAXSIZEOF_WS_HMACSHA384                                  (sizeof(actHASHMACSHA384STRUCT))

/* RIPEMD-160 */
# define ESL_MAXSIZEOF_WS_RIPEMD160                                   (sizeof(actRMD160STRUCT))
# define ESL_MAXSIZEOF_WS_HMACRIPEMD160                               (sizeof(actHASHMACRMD160STRUCT))
# define ESL_MINSIZEOF_WS_RIPEMD160                                   (ESL_MAXSIZEOF_WS_RIPEMD160)

/* CMAC (AES-128) */
# define ESL_MAXSIZEOF_WS_CMACAES                                     (sizeof(actCMACAESSTRUCT))

/* SipHash */
# define ESL_MAXSIZEOF_WS_SIPHASH                                     (sizeof(actSipHashSTRUCT))

/* KDF-2 workSpace sizes */
# define ESL_MAXSIZEOF_WS_KDF2HMACSHA1                                (sizeof(actKDF2STRUCT))
# define ESL_MAXSIZEOF_WS_KDF2HMACSHA256                              ESL_MAXSIZEOF_WS_KDF2HMACSHA1

/* KDF-X963 workSpace sizes */
# define ESL_MAXSIZEOF_WS_KDFX963SHA1                                 (sizeof(actKDFX963STRUCT))
# define ESL_MAXSIZEOF_WS_KDFX963SHA256                               (sizeof(actKDFX963SHA256))
# define ESL_MAXSIZEOF_WS_KDFX963SHA512                               (sizeof(actKDFX963SHA512))

/* HKDF WorkSpace sizes */
# define ESL_MAXSIZEOF_WS_HKDF_HMAC                                   (sizeof(actHKDFSTRUCT))
# define ESL_MAXSIZEOF_WS_HKDF_HASH                                   (sizeof(actHKDFHASHSTRUCT))

/* FIPS186 workspace sizes */
# define ESL_MAXSIZEOF_WS_FIPS186                                     (sizeof(actFIPS186STRUCT))

/* CTR DRBG WorkSpace size */
# define ESL_MAXSIZEOF_WS_CTRDRBG                                     (sizeof(eslt_WorkSpaceCTRDRBG))
# define ESL_MAXSIZEOF_WS_HASHDRBG                                    (sizeof(eslt_WorkSpaceHASHDRBG))

/* =============================================================================
 = ECC algorithm work space sizes
 ============================================================================= */

/* ECP workSpace sizes

 All sizes up to the listed length in bits of the field primes p and n.
 For key generation (kg), signature generation (sp), signature verification (vp),
 Diffie-Hellman generate secret (dh_prim) or key exchange (dh) different memory
 sizes are required. */

/* p_bits, n_bits <= 160                                  (Fermat inversion values:) */
# define ESL_SIZEOF_WS_ECP_160_KG                                     sizeof(actECPSTRUCT)     /* kg, dh_prim  */
# define ESL_SIZEOF_WS_ECP_160_DH                                     sizeof(actECPSTRUCT)     /* dh           */
# define ESL_SIZEOF_WS_ECP_160_SP                                     sizeof(actECPSTRUCT)     /* sp           */
# define ESL_SIZEOF_WS_ECP_160_VP                                     sizeof(actECPSTRUCT)     /* vp           */
/* p_bits, n_bits <= 192 */
# define ESL_SIZEOF_WS_ECP_192_KG                                     sizeof(actECPSTRUCT)     /* kg, dh_prim  */
# define ESL_SIZEOF_WS_ECP_192_DH                                     sizeof(actECPSTRUCT)     /* dh           */
# define ESL_SIZEOF_WS_ECP_192_SP                                     sizeof(actECPSTRUCT)     /* sp           */
# define ESL_SIZEOF_WS_ECP_192_VP                                     sizeof(actECPSTRUCT)     /* vp           */
/* p_bits, n_bits <= 224 */
# define ESL_SIZEOF_WS_ECP_224_KG                                     sizeof(actECPSTRUCT)     /* kg, dh_prim  */
# define ESL_SIZEOF_WS_ECP_224_DH                                     sizeof(actECPSTRUCT)     /* dh           */
# define ESL_SIZEOF_WS_ECP_224_SP                                     sizeof(actECPSTRUCT)     /* sp           */
# define ESL_SIZEOF_WS_ECP_224_VP                                     sizeof(actECPSTRUCT)     /* vp           */
/* p_bits, n_bits <= 256 */
# define ESL_SIZEOF_WS_ECP_256_KG                                     sizeof(actECPSTRUCT)     /* kg, dh_prim  */
# define ESL_SIZEOF_WS_ECP_256_DH                                     sizeof(actECPSTRUCT)     /* dh           */
# define ESL_SIZEOF_WS_ECP_256_SP                                     sizeof(actECPSTRUCT)     /* sp           */
# define ESL_SIZEOF_WS_ECP_256_VP                                     sizeof(actECPSTRUCT)     /* vp           */
/* p_bits, n_bits <= 320 */
# define ESL_SIZEOF_WS_ECP_320_KG                                     sizeof(actECPSTRUCT)     /* kg, dh_prim  */
# define ESL_SIZEOF_WS_ECP_320_DH                                     sizeof(actECPSTRUCT)     /* dh           */
# define ESL_SIZEOF_WS_ECP_320_SP                                     sizeof(actECPSTRUCT)     /* sp           */
# define ESL_SIZEOF_WS_ECP_320_VP                                     sizeof(actECPSTRUCT)     /* vp           */
/* p_bits, n_bits <= 384 */
# define ESL_SIZEOF_WS_ECP_384_KG                                     sizeof(actECPSTRUCT)     /* kg, dh_prim  */
# define ESL_SIZEOF_WS_ECP_384_DH                                     sizeof(actECPSTRUCT)     /* dh           */
# define ESL_SIZEOF_WS_ECP_384_SP                                     sizeof(actECPSTRUCT)     /* sp           */
# define ESL_SIZEOF_WS_ECP_384_VP                                     sizeof(actECPSTRUCT)     /* vp           */
/* p_bits, n_bits <= 512 */
# define ESL_SIZEOF_WS_ECP_512_KG                                     sizeof(actECPSTRUCT)     /* kg, dh_prim  */
# define ESL_SIZEOF_WS_ECP_512_DH                                     sizeof(actECPSTRUCT)     /* dh           */
# define ESL_SIZEOF_WS_ECP_512_SP                                     sizeof(actECPSTRUCT)     /* sp           */
# define ESL_SIZEOF_WS_ECP_512_VP                                     sizeof(actECPSTRUCT)     /* vp           */
/* p_bits, n_bits <= 521 (should be sufficient for 544 bits) */
# define ESL_SIZEOF_WS_ECP_521_KG                                     sizeof(actECPSTRUCT)     /* kg, dh_prim  */
# define ESL_SIZEOF_WS_ECP_521_DH                                     sizeof(actECPSTRUCT)     /* dh           */
# define ESL_SIZEOF_WS_ECP_521_SP                                     sizeof(actECPSTRUCT)     /* sp           */
# define ESL_SIZEOF_WS_ECP_521_VP                                     sizeof(actECPSTRUCT)     /* vp           */

/* 256 bit size chosen for eslt_WorkSpaceEcP_small definition: */
# define ESL_MAXSIZEOF_WS_ECP_SMALL                                   ESL_SIZEOF_WS_ECP_256_VP /* kg, dh_prim, dh, sp, vp */

/* 521 bit size chosen for eslt_WorkSpaceEcP_large definition: */
# define ESL_MAXSIZEOF_WS_ECP_LARGE                                   ESL_SIZEOF_WS_ECP_521_VP /* kg, dh_prim, dh, sp, vp */

/* =============================================================================
 * = EC-B/D key agreement workspace size
 * ============================================================================= */
# define ESL_SIZEOF_WS_ECBD_160                                       sizeof(actECBDstruct)
# define ESL_SIZEOF_WS_ECBD_192                                       sizeof(actECBDstruct)
# define ESL_SIZEOF_WS_ECBD_224                                       sizeof(actECBDstruct)
# define ESL_SIZEOF_WS_ECBD_256                                       sizeof(actECBDstruct)
# define ESL_SIZEOF_WS_ECBD_320                                       sizeof(actECBDstruct)
# define ESL_SIZEOF_WS_ECBD_384                                       sizeof(actECBDstruct)
# define ESL_SIZEOF_WS_ECBD_512                                       sizeof(actECBDstruct)
# define ESL_SIZEOF_WS_ECBD_521                                       sizeof(actECBDstruct)

/* ESL_MAX_KEY_SIZE_ECP bits chosen for eslt_WorkSpaceEcP definition: */
# ifdef ESL_MAX_KEY_SIZE_ECP /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
#  if (160 == ESL_MAX_KEY_SIZE_ECP) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#   define ESL_MAXSIZEOF_WS_ECP                                       ESL_SIZEOF_WS_ECP_160_VP
#   define ESL_MAXSIZEOF_WS_ECBD                                      ESL_SIZEOF_WS_ECBD_160
#  elif (192 == ESL_MAX_KEY_SIZE_ECP) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#   define ESL_MAXSIZEOF_WS_ECP                                       ESL_SIZEOF_WS_ECP_192_VP
#   define ESL_MAXSIZEOF_WS_ECBD                                      ESL_SIZEOF_WS_ECBD_192
#  elif (224 == ESL_MAX_KEY_SIZE_ECP) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#   define ESL_MAXSIZEOF_WS_ECP                                       ESL_SIZEOF_WS_ECP_224_VP
#   define ESL_MAXSIZEOF_WS_ECBD                                      ESL_SIZEOF_WS_ECBD_224
#  elif (256 == ESL_MAX_KEY_SIZE_ECP)
#   define ESL_MAXSIZEOF_WS_ECP                                       ESL_SIZEOF_WS_ECP_256_VP
#   define ESL_MAXSIZEOF_WS_ECBD                                      ESL_SIZEOF_WS_ECBD_256
#  elif (320 == ESL_MAX_KEY_SIZE_ECP) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#   define ESL_MAXSIZEOF_WS_ECP                                       ESL_SIZEOF_WS_ECP_320_VP
#   define ESL_MAXSIZEOF_WS_ECBD                                      ESL_SIZEOF_WS_ECBD_320
#  elif (384 == ESL_MAX_KEY_SIZE_ECP) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
#   define ESL_MAXSIZEOF_WS_ECP                                       ESL_SIZEOF_WS_ECP_384_VP
#   define ESL_MAXSIZEOF_WS_ECBD                                      ESL_SIZEOF_WS_ECBD_384
#  elif (512 == ESL_MAX_KEY_SIZE_ECP) /* COV_VSECPRIM_NO_SAFE_CONFIG XX */
#   define ESL_MAXSIZEOF_WS_ECP                                       ESL_SIZEOF_WS_ECP_512_VP
#   define ESL_MAXSIZEOF_WS_ECBD                                      ESL_SIZEOF_WS_ECBD_512
#  elif (521 == ESL_MAX_KEY_SIZE_ECP) /* COV_VSECPRIM_NO_SAFE_CONFIG XX */
#   define ESL_MAXSIZEOF_WS_ECP                                       ESL_SIZEOF_WS_ECP_521_VP
#   define ESL_MAXSIZEOF_WS_ECBD                                      ESL_SIZEOF_WS_ECBD_521
#  else
#   define ESL_MAXSIZEOF_WS_ECP                                       ESL_SIZEOF_WS_ECP_256_VP
#   define ESL_MAXSIZEOF_WS_ECBD                                      ESL_SIZEOF_WS_ECBD_256
#  endif
# else
#  define ESL_MAXSIZEOF_WS_ECP                                        ESL_SIZEOF_WS_ECP_256_VP
#  define ESL_MAXSIZEOF_WS_ECBD                                       ESL_SIZEOF_WS_ECBD_256
# endif

/* =============================================================================
 = RSA work space sizes (core)
 ============================================================================= */
/* RSA Prim Work Space Sizes */
# define ESL_MAXSIZEOF_WS_RSA_PRIM                                    (sizeof(actRSAPRIMSTRUCT))

# define ESL_SIZEOF_WS_HASHUNION_RSA                                  (sizeof(eslt_WorkSpaceUnionHashFunctions_RSA) - sizeof(eslt_WorkSpaceHeader))

/* WS size defines, kept for compatibility reasons */
/* n_bits <= 512 */
# define ESL_SIZEOF_WS_RSA_PUB_512                                    ESL_MAXSIZEOF_WS_RSA_PRIM
# define ESL_SIZEOF_WS_RSA_PRIV_512                                   ESL_MAXSIZEOF_WS_RSA_PRIM
# define ESL_SIZEOF_WS_RSA_PRIV_CRT_512                               (sizeof(actRSACRTTMPSTRUCT))   /* assumes: p_bits, q_bits <=  */
/* n_bits <= 1024 */
# define ESL_SIZEOF_WS_RSA_PUB_1024                                   ESL_MAXSIZEOF_WS_RSA_PRIM
# define ESL_SIZEOF_WS_RSA_PRIV_1024                                  ESL_MAXSIZEOF_WS_RSA_PRIM
# define ESL_SIZEOF_WS_RSA_PRIV_CRT_1024                              (sizeof(actRSACRTTMPSTRUCT))   /* assumes: p_bits, q_bits <= 544 */
/* n_bits <= 1536 */
# define ESL_SIZEOF_WS_RSA_PUB_1536                                   ESL_MAXSIZEOF_WS_RSA_PRIM
# define ESL_SIZEOF_WS_RSA_PRIV_1536                                  ESL_MAXSIZEOF_WS_RSA_PRIM
# define ESL_SIZEOF_WS_RSA_PRIV_CRT_1536                              (sizeof(actRSACRTTMPSTRUCT))   /* assumes: p_bits, q_bits <= 800 */
/* n_bits <= 2048 */
# define ESL_SIZEOF_WS_RSA_PUB_2048                                   ESL_MAXSIZEOF_WS_RSA_PRIM
# define ESL_SIZEOF_WS_RSA_PRIV_2048                                  ESL_MAXSIZEOF_WS_RSA_PRIM
# define ESL_SIZEOF_WS_RSA_PRIV_CRT_2048                              (sizeof(actRSACRTTMPSTRUCT))   /* assumes: p_bits, q_bits <= 1056 */
/* n_bits <= 3072 */
# define ESL_SIZEOF_WS_RSA_PUB_3072                                   ESL_MAXSIZEOF_WS_RSA_PRIM
# define ESL_SIZEOF_WS_RSA_PRIV_3072                                  ESL_MAXSIZEOF_WS_RSA_PRIM
# define ESL_SIZEOF_WS_RSA_PRIV_CRT_3072                              (sizeof(actRSACRTTMPSTRUCT))   /* assumes: p_bits, q_bits <= 1568 */
/* n_bits <= 4096 */
# define ESL_SIZEOF_WS_RSA_PUB_4096                                   ESL_MAXSIZEOF_WS_RSA_PRIM
# define ESL_SIZEOF_WS_RSA_PRIV_4096                                  ESL_MAXSIZEOF_WS_RSA_PRIM
# define ESL_SIZEOF_WS_RSA_PRIV_CRT_4096                              (sizeof(actRSACRTTMPSTRUCT))   /* assumes: p_bits, q_bits <= 2080 */
/* end of WS size defines, kept for compatibility reasons */

/* =============================================================================
 = RSA PKCS#1 V1.5 algorithm work space sizes
 ============================================================================= */
# define ESL_MAXSIZEOF_WS_RSA_ENC                                     (sizeof(eslt_WorkSpaceRSAenc)         - sizeof(eslt_WorkSpaceHeader))
# define ESL_MAXSIZEOF_WS_RSA_DEC                                     (sizeof(eslt_WorkSpaceRSAdec)         - sizeof(eslt_WorkSpaceHeader))

# define ESL_MAXSIZEOF_WS_RSA_ENC_PRIM                                (sizeof(actRSAPRIMSTRUCT))
# define ESL_MAXSIZEOF_WS_RSA_DEC_PRIM                                (sizeof(actRSAPRIMSTRUCT))

# define ESL_MAXSIZEOF_WS_RSA_SIG                                     (sizeof(eslt_WorkSpaceRSAsig)         - sizeof(eslt_WorkSpaceHeader))
# define ESL_MAXSIZEOF_WS_RSA_VER                                     (sizeof(eslt_WorkSpaceRSAver)         - sizeof(eslt_WorkSpaceHeader))

 /* NOTE: All CRT size values assume:            p_bits, q_bits <= n_bits + 32 */
# define ESL_MAXSIZEOF_WS_RSA_CRT_DEC                                 (sizeof(eslt_WorkSpaceRSACRTdec)      - sizeof(eslt_WorkSpaceHeader))
# define ESL_MAXSIZEOF_WS_RSA_CRT_DEC_PRIM                            (sizeof(actRSACRTSTRUCT))
# define ESL_MAXSIZEOF_WS_RSA_CRT_SIG                                 (sizeof(eslt_WorkSpaceRSACRTsig)      - sizeof(eslt_WorkSpaceHeader))
# define ESL_MAXSIZEOF_WS_RSA_CRT_SIG_PRIM                            (sizeof(actRSACRTSTRUCT))

/* RSA WS size defines, kept for compatability reasons */
# define ESL_MAXSIZEOF_WS_RSA_SIG_PRIM                                (sizeof(actRSAPRIMSTRUCT))
# define ESL_MAXSIZEOF_WS_RSA_VER_PRIM                                (sizeof(actRSAPRIMSTRUCT))
# define ESL_MAXSIZEOF_WS_RSA_V15                                     ESL_MAXSIZEOF_WS_RSA_VER

/* RSA PRIM 512 */
# define ESL_SIZEOF_WS_RSA_ENC_PRIM_512                               ESL_MAXSIZEOF_WS_RSA_ENC_PRIM
# define ESL_SIZEOF_WS_RSA_DEC_PRIM_512                               ESL_MAXSIZEOF_WS_RSA_DEC_PRIM
# define ESL_SIZEOF_WS_RSA_CRT_DEC_PRIM_512                           ESL_MAXSIZEOF_WS_RSA_CRT_DEC_PRIM
/* RSA PRIM 512 sig/ver */
# define ESL_SIZEOF_WS_RSA_SIG_PRIM_512                               ESL_MAXSIZEOF_WS_RSA_SIG_PRIM
# define ESL_SIZEOF_WS_RSA_CRT_SIG_PRIM_512                           ESL_MAXSIZEOF_WS_RSA_CRT_SIG_PRIM
# define ESL_SIZEOF_WS_RSA_VER_PRIM_512                               ESL_MAXSIZEOF_WS_RSA_VER_PRIM
/* RSA PRIM 512 enc/dec/sig/ver */
# define ESL_SIZEOF_WS_RSA_PRIM_512                                   ESL_MAXSIZEOF_WS_RSA_VER_PRIM

/* RSA PRIM 1k enc/dec */
# define ESL_SIZEOF_WS_RSA_ENC_PRIM_1024                              ESL_MAXSIZEOF_WS_RSA_ENC_PRIM
# define ESL_SIZEOF_WS_RSA_DEC_PRIM_1024                              ESL_MAXSIZEOF_WS_RSA_DEC_PRIM
# define ESL_SIZEOF_WS_RSA_CRT_DEC_PRIM_1024                          ESL_MAXSIZEOF_WS_RSA_CRT_DEC_PRIM
/* RSA PRIM 1k sig/ver */
# define ESL_SIZEOF_WS_RSA_SIG_PRIM_1024                              ESL_MAXSIZEOF_WS_RSA_SIG_PRIM
# define ESL_SIZEOF_WS_RSA_CRT_SIG_PRIM_1024                          ESL_MAXSIZEOF_WS_RSA_CRT_SIG_PRIM
# define ESL_SIZEOF_WS_RSA_VER_PRIM_1024                              ESL_MAXSIZEOF_WS_RSA_VER_PRIM
/* RSA PRIM 1k enc/dec/sig/ver */
# define ESL_SIZEOF_WS_RSA_PRIM_1024                                  ESL_MAXSIZEOF_WS_RSA_VER_PRIM

/* RSA PRIM 1.5k enc/dec */
# define ESL_SIZEOF_WS_RSA_ENC_PRIM_1536                              ESL_MAXSIZEOF_WS_RSA_ENC_PRIM
# define ESL_SIZEOF_WS_RSA_DEC_PRIM_1536                              ESL_MAXSIZEOF_WS_RSA_DEC_PRIM
# define ESL_SIZEOF_WS_RSA_CRT_DEC_PRIM_1536                          ESL_MAXSIZEOF_WS_RSA_CRT_DEC_PRIM
/* RSA PRIM 1.5k sig/ver */
# define ESL_SIZEOF_WS_RSA_SIG_PRIM_1536                              ESL_MAXSIZEOF_WS_RSA_SIG_PRIM
# define ESL_SIZEOF_WS_RSA_CRT_SIG_PRIM_1536                          ESL_MAXSIZEOF_WS_RSA_CRT_SIG_PRIM
# define ESL_SIZEOF_WS_RSA_VER_PRIM_1536                              ESL_MAXSIZEOF_WS_RSA_VER_PRIM
/* RSA PRIM 1.5k enc/dec/sig/ver */
# define ESL_SIZEOF_WS_RSA_PRIM_1536                                  ESL_MAXSIZEOF_WS_RSA_VER_PRIM

/* RSA PRIM 2k enc/dec */
# define ESL_SIZEOF_WS_RSA_ENC_PRIM_2048                              ESL_MAXSIZEOF_WS_RSA_ENC_PRIM
# define ESL_SIZEOF_WS_RSA_DEC_PRIM_2048                              ESL_MAXSIZEOF_WS_RSA_DEC_PRIM
# define ESL_SIZEOF_WS_RSA_CRT_DEC_PRIM_2048                          ESL_MAXSIZEOF_WS_RSA_CRT_DEC_PRIM
/* RSA PRIM 2k sig/ver */
# define ESL_SIZEOF_WS_RSA_SIG_PRIM_2048                              ESL_MAXSIZEOF_WS_RSA_SIG_PRIM
# define ESL_SIZEOF_WS_RSA_CRT_SIG_PRIM_2048                          ESL_MAXSIZEOF_WS_RSA_CRT_SIG_PRIM
# define ESL_SIZEOF_WS_RSA_VER_PRIM_2048                              ESL_MAXSIZEOF_WS_RSA_VER_PRIM
/* RSA PRIM 2k enc/dec/sig/ver */
# define ESL_SIZEOF_WS_RSA_PRIM_2048                                  ESL_MAXSIZEOF_WS_RSA_VER_PRIM

/* RSA PRIM 3k enc/dec */
# define ESL_SIZEOF_WS_RSA_ENC_PRIM_3072                              ESL_MAXSIZEOF_WS_RSA_ENC_PRIM
# define ESL_SIZEOF_WS_RSA_DEC_PRIM_3072                              ESL_MAXSIZEOF_WS_RSA_DEC_PRIM
# define ESL_SIZEOF_WS_RSA_CRT_DEC_PRIM_3072                          ESL_MAXSIZEOF_WS_RSA_CRT_DEC_PRIM
/* RSA PRIM 3k sig/ver */
# define ESL_SIZEOF_WS_RSA_SIG_PRIM_3072                              ESL_MAXSIZEOF_WS_RSA_SIG_PRIM
# define ESL_SIZEOF_WS_RSA_CRT_SIG_PRIM_3072                          ESL_MAXSIZEOF_WS_RSA_CRT_SIG_PRIM
# define ESL_SIZEOF_WS_RSA_VER_PRIM_3072                              ESL_MAXSIZEOF_WS_RSA_VER_PRIM
/* RSA PRIM 3k enc/dec/sig/ver */
# define ESL_SIZEOF_WS_RSA_PRIM_3072                                  ESL_MAXSIZEOF_WS_RSA_VER_PRIM

/* RSA PRIM 4k enc/dec */
# define ESL_SIZEOF_WS_RSA_ENC_PRIM_4096                              ESL_MAXSIZEOF_WS_RSA_ENC_PRIM
# define ESL_SIZEOF_WS_RSA_DEC_PRIM_4096                              ESL_MAXSIZEOF_WS_RSA_DEC_PRIM
# define ESL_SIZEOF_WS_RSA_CRT_DEC_PRIM_4096                          ESL_MAXSIZEOF_WS_RSA_CRT_DEC_PRIM
/* RSA PRIM 4k sig/ver */
# define ESL_SIZEOF_WS_RSA_SIG_PRIM_4096                              ESL_MAXSIZEOF_WS_RSA_SIG_PRIM
# define ESL_SIZEOF_WS_RSA_CRT_SIG_PRIM_4096                          ESL_MAXSIZEOF_WS_RSA_CRT_SIG_PRIM
# define ESL_SIZEOF_WS_RSA_VER_PRIM_4096                              ESL_MAXSIZEOF_WS_RSA_VER_PRIM
/* RSA PRIM 4k enc/dec/sig/ver */
# define ESL_SIZEOF_WS_RSA_PRIM_4096                                  ESL_MAXSIZEOF_WS_RSA_VER_PRIM

/* RSA V15 512 */
# define ESL_SIZEOF_WS_RSA_ENC_512                                    ESL_MAXSIZEOF_WS_RSA_ENC
# define ESL_SIZEOF_WS_RSA_DEC_512                                    ESL_MAXSIZEOF_WS_RSA_DEC
# define ESL_SIZEOF_WS_RSA_CRT_DEC_512                                ESL_MAXSIZEOF_WS_RSA_CRT_DEC
/* RSA V15 512 sig/ver */
# define ESL_SIZEOF_WS_RSA_SIG_512                                    ESL_MAXSIZEOF_WS_RSA_SIG
# define ESL_SIZEOF_WS_RSA_CRT_SIG_512                                ESL_MAXSIZEOF_WS_RSA_CRT_SIG
# define ESL_SIZEOF_WS_RSA_VER_512                                    ESL_MAXSIZEOF_WS_RSA_VER
/* RSA V15 512 enc/dec/sig/ver */
# define ESL_SIZEOF_WS_RSA_V15_512                                    ESL_MAXSIZEOF_WS_RSA_VER

/* RSA V15 1k enc/dec */
# define ESL_SIZEOF_WS_RSA_ENC_1024                                   ESL_MAXSIZEOF_WS_RSA_ENC
# define ESL_SIZEOF_WS_RSA_DEC_1024                                   ESL_MAXSIZEOF_WS_RSA_DEC
# define ESL_SIZEOF_WS_RSA_CRT_DEC_1024                               ESL_MAXSIZEOF_WS_RSA_CRT_DEC
/* RSA V15 1k sig/ver */
# define ESL_SIZEOF_WS_RSA_SIG_1024                                   ESL_MAXSIZEOF_WS_RSA_SIG
# define ESL_SIZEOF_WS_RSA_CRT_SIG_1024                               ESL_MAXSIZEOF_WS_RSA_CRT_SIG
# define ESL_SIZEOF_WS_RSA_VER_1024                                   ESL_MAXSIZEOF_WS_RSA_VER
/* RSA V15 1k enc/dec/sig/ver */
# define ESL_SIZEOF_WS_RSA_V15_1024                                   ESL_MAXSIZEOF_WS_RSA_VER

/* RSA V15 1.5k enc/dec */
# define ESL_SIZEOF_WS_RSA_ENC_1536                                   ESL_MAXSIZEOF_WS_RSA_ENC
# define ESL_SIZEOF_WS_RSA_DEC_1536                                   ESL_MAXSIZEOF_WS_RSA_DEC
# define ESL_SIZEOF_WS_RSA_CRT_DEC_1536                               ESL_MAXSIZEOF_WS_RSA_CRT_DEC
/* RSA V15 1.5k sig/ver */
# define ESL_SIZEOF_WS_RSA_SIG_1536                                   ESL_MAXSIZEOF_WS_RSA_SIG
# define ESL_SIZEOF_WS_RSA_CRT_SIG_1536                               ESL_MAXSIZEOF_WS_RSA_CRT_SIG
# define ESL_SIZEOF_WS_RSA_VER_1536                                   ESL_MAXSIZEOF_WS_RSA_VER
/* RSA V15 1.5k enc/dec/sig/ver */
# define ESL_SIZEOF_WS_RSA_V15_1536                                   ESL_MAXSIZEOF_WS_RSA_VER

/* RSA V15 2k enc/dec */
# define ESL_SIZEOF_WS_RSA_ENC_2048                                   ESL_MAXSIZEOF_WS_RSA_ENC
# define ESL_SIZEOF_WS_RSA_DEC_2048                                   ESL_MAXSIZEOF_WS_RSA_DEC
# define ESL_SIZEOF_WS_RSA_CRT_DEC_2048                               ESL_MAXSIZEOF_WS_RSA_CRT_DEC
/* RSA V15 2k sig/ver */
# define ESL_SIZEOF_WS_RSA_SIG_2048                                   ESL_MAXSIZEOF_WS_RSA_SIG
# define ESL_SIZEOF_WS_RSA_CRT_SIG_2048                               ESL_MAXSIZEOF_WS_RSA_CRT_SIG
# define ESL_SIZEOF_WS_RSA_VER_2048                                   ESL_MAXSIZEOF_WS_RSA_VER
/* RSA V15 2k enc/dec/sig/ver */
# define ESL_SIZEOF_WS_RSA_V15_2048                                   ESL_MAXSIZEOF_WS_RSA_VER

/* RSA V15 3k enc/dec */
# define ESL_SIZEOF_WS_RSA_ENC_3072                                   ESL_MAXSIZEOF_WS_RSA_ENC
# define ESL_SIZEOF_WS_RSA_DEC_3072                                   ESL_MAXSIZEOF_WS_RSA_DEC
# define ESL_SIZEOF_WS_RSA_CRT_DEC_3072                               ESL_MAXSIZEOF_WS_RSA_CRT_DEC
/* RSA V15 2k sig/ver */
# define ESL_SIZEOF_WS_RSA_SIG_3072                                   ESL_MAXSIZEOF_WS_RSA_SIG
# define ESL_SIZEOF_WS_RSA_CRT_SIG_3072                               ESL_MAXSIZEOF_WS_RSA_CRT_SIG
# define ESL_SIZEOF_WS_RSA_VER_3072                                   ESL_MAXSIZEOF_WS_RSA_VER
/* RSA V15 2k enc/dec/sig/ver */
# define ESL_SIZEOF_WS_RSA_V15_3072                                   ESL_MAXSIZEOF_WS_RSA_VER

/* RSA V15 3k enc/dec */
# define ESL_SIZEOF_WS_RSA_ENC_4096                                   ESL_MAXSIZEOF_WS_RSA_ENC
# define ESL_SIZEOF_WS_RSA_DEC_4096                                   ESL_MAXSIZEOF_WS_RSA_DEC
# define ESL_SIZEOF_WS_RSA_CRT_DEC_4096                               ESL_MAXSIZEOF_WS_RSA_CRT_DEC
/* RSA V15 2k sig/ver */
# define ESL_SIZEOF_WS_RSA_SIG_4096                                   ESL_MAXSIZEOF_WS_RSA_SIG
# define ESL_SIZEOF_WS_RSA_CRT_SIG_4096                               ESL_MAXSIZEOF_WS_RSA_CRT_SIG
# define ESL_SIZEOF_WS_RSA_VER_4096                                   ESL_MAXSIZEOF_WS_RSA_VER
/* RSA V15 2k enc/dec/sig/ver */
# define ESL_SIZEOF_WS_RSA_V15_4096                                   ESL_MAXSIZEOF_WS_RSA_VER
/* end of WS size defines, kept for compatibility reasons */

/* =============================================================================
 = RSA OAEP algorithm work space sizes
 ============================================================================= */
/* NOTE: All CRT size values assume:            p_bits, q_bits <= n_bits + 32 */
/* RSA OAEP 512 */
# define ESL_MAXSIZEOF_WS_RSA_OAEP_ENC                                (sizeof(eslt_WorkSpaceRSAOAEPenc)    - sizeof(eslt_WorkSpaceHeader))
# define ESL_MAXSIZEOF_WS_RSA_OAEP_DEC                                (sizeof(eslt_WorkSpaceRSAOAEPdec)    - sizeof(eslt_WorkSpaceHeader))
# define ESL_MAXSIZEOF_WS_RSA_CRT_OAEP_DEC                            (sizeof(eslt_WorkSpaceRSACRTOAEPdec) - sizeof(eslt_WorkSpaceHeader))

/* WS size defines, kept for compatibility reasons */
# define ESL_MAXSIZEOF_WS_RSA_OAEP                                    ESL_MAXSIZEOF_WS_RSA_OAEP_ENC

/* RSA OAEP 512 */
# define ESL_SIZEOF_WS_RSA_OAEP_ENC_512                               ESL_MAXSIZEOF_WS_RSA_OAEP_ENC
# define ESL_SIZEOF_WS_RSA_OAEP_DEC_512                               ESL_MAXSIZEOF_WS_RSA_OAEP_DEC
# define ESL_SIZEOF_WS_RSA_CRT_OAEP_DEC_512                           ESL_MAXSIZEOF_WS_RSA_CRT_OAEP_DEC
/* RSA OAEP 512 enc/dec */
# define ESL_SIZEOF_WS_RSA_OAEP_512                                   ESL_MAXSIZEOF_WS_RSA_OAEP_ENC

/* RSA OAEP 1k enc/dec */
# define ESL_SIZEOF_WS_RSA_OAEP_ENC_1024                              ESL_MAXSIZEOF_WS_RSA_OAEP_ENC
# define ESL_SIZEOF_WS_RSA_OAEP_DEC_1024                              ESL_MAXSIZEOF_WS_RSA_OAEP_DEC
# define ESL_SIZEOF_WS_RSA_CRT_OAEP_DEC_1024                          ESL_MAXSIZEOF_WS_RSA_CRT_OAEP_DEC
/* RSA OAEP 1k enc/dec */
# define ESL_SIZEOF_WS_RSA_OAEP_1024                                  ESL_MAXSIZEOF_WS_RSA_OAEP_ENC

/* RSA OAEP 1.5k enc/dec */
# define ESL_SIZEOF_WS_RSA_OAEP_ENC_1536                              ESL_MAXSIZEOF_WS_RSA_OAEP_ENC
# define ESL_SIZEOF_WS_RSA_OAEP_DEC_1536                              ESL_MAXSIZEOF_WS_RSA_OAEP_DEC
# define ESL_SIZEOF_WS_RSA_CRT_OAEP_DEC_1536                          ESL_MAXSIZEOF_WS_RSA_CRT_OAEP_DEC
/* RSA OAEP 1.5k enc/dec */
# define ESL_SIZEOF_WS_RSA_OAEP_1536                                  ESL_MAXSIZEOF_WS_RSA_OAEP_ENC

/* RSA OAEP 2k enc/dec */
# define ESL_SIZEOF_WS_RSA_OAEP_ENC_2048                              ESL_MAXSIZEOF_WS_RSA_OAEP_ENC
# define ESL_SIZEOF_WS_RSA_OAEP_DEC_2048                              ESL_MAXSIZEOF_WS_RSA_OAEP_DEC
# define ESL_SIZEOF_WS_RSA_CRT_OAEP_DEC_2048                          ESL_MAXSIZEOF_WS_RSA_CRT_OAEP_DEC
/* RSA OAEP 2k enc/dec */
# define ESL_SIZEOF_WS_RSA_OAEP_2048                                  ESL_MAXSIZEOF_WS_RSA_OAEP_ENC

/* RSA OAEP 3k enc/dec */
# define ESL_SIZEOF_WS_RSA_OAEP_ENC_3072                              ESL_MAXSIZEOF_WS_RSA_OAEP_ENC
# define ESL_SIZEOF_WS_RSA_OAEP_DEC_3072                              ESL_MAXSIZEOF_WS_RSA_OAEP_DEC
# define ESL_SIZEOF_WS_RSA_CRT_OAEP_DEC_3072                          ESL_MAXSIZEOF_WS_RSA_CRT_OAEP_DEC
/* RSA OAEP 2k enc/dec */
# define ESL_SIZEOF_WS_RSA_OAEP_3072                                  ESL_MAXSIZEOF_WS_RSA_OAEP_ENC

/* RSA OAEP 3k enc/dec */
# define ESL_SIZEOF_WS_RSA_OAEP_ENC_4096                              ESL_MAXSIZEOF_WS_RSA_OAEP_ENC
# define ESL_SIZEOF_WS_RSA_OAEP_DEC_4096                              ESL_MAXSIZEOF_WS_RSA_OAEP_DEC
# define ESL_SIZEOF_WS_RSA_CRT_OAEP_DEC_4096                          ESL_MAXSIZEOF_WS_RSA_CRT_OAEP_DEC
/* RSA OAEP 2k enc/dec */
# define ESL_SIZEOF_WS_RSA_OAEP_4096                                  ESL_MAXSIZEOF_WS_RSA_OAEP_ENC
/* end of WS size defines, kept for compatibility reasons */

/* =============================================================================
 = RSA PKCS#1 PSS algorithm work space sizes
 ============================================================================= */
/* RSA PSS sig/ver */
# define ESL_MAXSIZEOF_WS_RSA_PSS_SIG                                 (sizeof(eslt_WorkSpaceRSAPSSsig)    - sizeof(eslt_WorkSpaceHeader))
# define ESL_MAXSIZEOF_WS_RSA_PSS_VER                                 (sizeof(eslt_WorkSpaceRSAPSSver)    - sizeof(eslt_WorkSpaceHeader))

/* WS sizes defines, kept for compatibility reasons */
# define ESL_MAXSIZEOF_WS_RSA_PSS_CRT_SIG                             (0u)      /* CRT not implemented for RSA PSS */
# define ESL_MAXSIZEOF_WS_RSA_PSS                                     (sizeof(eslt_WorkSpaceRSAPSSsig))

/* RSA PSS 512 */
# define ESL_SIZEOF_WS_RSA_PSS_SIG_512                                ESL_MAXSIZEOF_WS_RSA_PSS_SIG
# define ESL_SIZEOF_WS_RSA_PSS_CRT_SIG_512                            ESL_MAXSIZEOF_WS_RSA_PSS_CRT_SIG
# define ESL_SIZEOF_WS_RSA_PSS_VER_512                                ESL_MAXSIZEOF_WS_RSA_PSS_VER
# define ESL_SIZEOF_WS_RSA_PSS_512                                    ESL_MAXSIZEOF_WS_RSA_PSS

/* RSA PSS 1k sig/ver */
# define ESL_SIZEOF_WS_RSA_PSS_SIG_1024                               ESL_MAXSIZEOF_WS_RSA_PSS_SIG
# define ESL_SIZEOF_WS_RSA_PSS_CRT_SIG_1024                           ESL_MAXSIZEOF_WS_RSA_PSS_CRT_SIG
# define ESL_SIZEOF_WS_RSA_PSS_VER_1024                               ESL_MAXSIZEOF_WS_RSA_PSS_VER
# define ESL_SIZEOF_WS_RSA_PSS_1024                                   ESL_MAXSIZEOF_WS_RSA_PSS

/* RSA PSS 1.5k sig/ver */
# define ESL_SIZEOF_WS_RSA_PSS_SIG_1536                               ESL_MAXSIZEOF_WS_RSA_PSS_SIG
# define ESL_SIZEOF_WS_RSA_PSS_CRT_SIG_1536                           ESL_MAXSIZEOF_WS_RSA_PSS_CRT_SIG
# define ESL_SIZEOF_WS_RSA_PSS_VER_1536                               ESL_MAXSIZEOF_WS_RSA_PSS_VER
# define ESL_SIZEOF_WS_RSA_PSS_1536                                   ESL_MAXSIZEOF_WS_RSA_PSS

/* RSA PSS 2k sig/ver */
# define ESL_SIZEOF_WS_RSA_PSS_SIG_2048                               ESL_MAXSIZEOF_WS_RSA_PSS_SIG
# define ESL_SIZEOF_WS_RSA_PSS_CRT_SIG_2048                           ESL_MAXSIZEOF_WS_RSA_PSS_CRT_SIG
# define ESL_SIZEOF_WS_RSA_PSS_VER_2048                               ESL_MAXSIZEOF_WS_RSA_PSS_VER
# define ESL_SIZEOF_WS_RSA_PSS_2048                                   ESL_MAXSIZEOF_WS_RSA_PSS

/* RSA PSS 3k sig/ver */
# define ESL_SIZEOF_WS_RSA_PSS_SIG_3072                               ESL_MAXSIZEOF_WS_RSA_PSS_SIG
# define ESL_SIZEOF_WS_RSA_PSS_CRT_SIG_3072                           ESL_MAXSIZEOF_WS_RSA_PSS_CRT_SIG
# define ESL_SIZEOF_WS_RSA_PSS_VER_3072                               ESL_MAXSIZEOF_WS_RSA_PSS_VER
# define ESL_SIZEOF_WS_RSA_PSS_3072                                   ESL_MAXSIZEOF_WS_RSA_PSS

/* RSA PSS 4k sig/ver */
# define ESL_SIZEOF_WS_RSA_PSS_SIG_4096                               ESL_MAXSIZEOF_WS_RSA_PSS_SIG
# define ESL_SIZEOF_WS_RSA_PSS_CRT_SIG_4096                           ESL_MAXSIZEOF_WS_RSA_PSS_CRT_SIG
# define ESL_SIZEOF_WS_RSA_PSS_VER_4096                               ESL_MAXSIZEOF_WS_RSA_PSS_VER
# define ESL_SIZEOF_WS_RSA_PSS_4096                                   ESL_MAXSIZEOF_WS_RSA_PSS
/* end of WS size defines, kept for compatibility reasons */

/* =============================================================================
= SPAKE2+
============================================================================= */
# define ESL_SIZEOF_WS_SPAKE2P                                        sizeof(eslt_WorkSpaceSPAKE2P) - sizeof(eslt_WorkSpaceHeader)
# define ESL_SIZEOF_WS_SPAKE2PPRE                                     sizeof(actSPAKE2PPreambleStruct)

/* =============================================================================
 = EdDSA & ECDH workspace sizes
 ============================================================================= */
# define ESL_SIZEOF_WS_Ed25519                                        (sizeof(actEd25519STRUCT))
# define ESL_SIZEOF_WS_X25519                                         (sizeof(actX25519STRUCT))

/* =============================================================================
 = ChaCha20 & Poly1305 workspace sizes
 = (take alignment to 64bit into account)
 ============================================================================= */
# define ESL_SIZEOF_WS_ChaCha20                                       (140u) /*  2* sizeof (block) + sizeof (int) */
# define ESL_SIZEOF_WS_Poly1305                                       (180u) /*  sizeof (block) + sizeof (actU32) + sizeof (opaque) */
# define ESL_SIZEOF_WS_AEAD_ChaCha_Poly                               (390u) /*  ESL_SIZEOF_WS_ChaCha20 + ESL_SIZEOF_WS_Poly1305 + 9* sizeof (actU32) */

# define ESL_SIZEOF_WS_AESCCM                                         (sizeof(actAESCCMSTRUCT))

/* =============================================================================
   = Workspace definitions
   ============================================================================= */

/* common struct */
typedef struct
{
  eslt_WorkSpaceHeader header;
  actAESSTRUCT wsAES;
} eslt_WorkSpaceAES;

typedef eslt_WorkSpaceAES eslt_WorkSpaceAES128;
typedef eslt_WorkSpaceAES eslt_WorkSpaceAES128Block;
typedef eslt_WorkSpaceAES eslt_WorkSpaceAES192;
typedef eslt_WorkSpaceAES eslt_WorkSpaceAES192Block;
typedef eslt_WorkSpaceAES eslt_WorkSpaceAES256;
typedef eslt_WorkSpaceAES eslt_WorkSpaceAES256Block;

typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_Byte wsDES[ESL_MAXSIZEOF_WS_DES];
} eslt_WorkSpaceDES;

typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_Byte wsDESBlock[ESL_MAXSIZEOF_WS_DESBLOCK];
} eslt_WorkSpaceDESBlock;

typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_Byte wsTDES[ESL_MAXSIZEOF_WS_TDES];
} eslt_WorkSpaceTDES;

typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_Byte wsTDESBlock[ESL_MAXSIZEOF_WS_TDESBLOCK];
} eslt_WorkSpaceTDESBlock;

typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_Byte wsRC2[ESL_MAXSIZEOF_WS_RC2];
} eslt_WorkSpaceRC2;

typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_Byte wsRC2Block[ESL_MAXSIZEOF_WS_RC2BLOCK];
} eslt_WorkSpaceRC2Block;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actGCMSTRUCT wsGCM;
} eslt_WorkSpaceGCM;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actGCMSTRUCT wsGMAC;
} eslt_WorkSpaceGMAC;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actSHASTRUCT wsSHA1;
} eslt_WorkSpaceSHA1;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actSHA224STRUCT wsSHA224;
} eslt_WorkSpaceSHA224;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actSHA256STRUCT wsSHA256;
} eslt_WorkSpaceSHA256;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actSHA512_224STRUCT wsSHA512_224;
} eslt_WorkSpaceSHA512_224;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actSHA512_256STRUCT wsSHA512_256;
} eslt_WorkSpaceSHA512_256;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actSHA384STRUCT wsSHA384;
} eslt_WorkSpaceSHA384;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actSHA512STRUCT wsSHA512;
} eslt_WorkSpaceSHA512;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actKECCAKcSTRUCT wsSHA3;
} eslt_WorkSpaceSHA3;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actKECCAKcSTRUCT wsSHAKE;
} eslt_WorkSpaceSHAKE;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actRMD160STRUCT wsRIPEMD160;
} eslt_WorkSpaceRIPEMD160;

typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_Byte wsMD5[ESL_MAXSIZEOF_WS_MD5];
} eslt_WorkSpaceMD5;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actHASHMACSTRUCT wsHMACSHA1;
} eslt_WorkSpaceHMACSHA1;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actHASHMACSHA256STRUCT wsHMACSHA256;
} eslt_WorkSpaceHMACSHA256;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actHASHMACSHA384STRUCT wsHMACSHA384;
} eslt_WorkSpaceHMACSHA384;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actHASHMACRMD160STRUCT wsHMACRIPEMD160;
} eslt_WorkSpaceHMACRIPEMD160;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actCMACAESSTRUCT wsCMACAESbuffer;
  VSECPRIM_P2VAR_PARA(eslt_Byte) wsCMACAES; /* Backward compatible pointer */
} eslt_WorkSpaceCMACAES;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actSipHashSTRUCT wsSipHash;
} eslt_WorkSpaceSipHash;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actFIPS186STRUCT wsFIPS186buffer;
  VSECPRIM_P2VAR_PARA(eslt_Byte) wsFIPS186; /* Backward compatible pointer */
} eslt_WorkSpaceFIPS186;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actCTRDRBGSTRUCT wsDRBG;
} eslt_WorkSpaceCTRDRBG;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actHASHDRBGSTRUCT wsDRBG;
} eslt_WorkSpaceHASHDRBG;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actKDF2STRUCT wsKDF2HMACSHA;
} eslt_WorkSpaceKDF2;

# define eslt_WorkSpaceKDF2HMACSHA1                                   eslt_WorkSpaceKDF2
# define eslt_WorkSpaceKDF2HMACSHA256                                 eslt_WorkSpaceKDF2

typedef struct
{
  eslt_WorkSpaceHeader header;
  actKDFX963STRUCT wsKDFX963SHA1;
} eslt_WorkSpaceKDFX963SHA1;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actKDFX963SHA256 wsKDFX963SHA256;
} eslt_WorkSpaceKDFX963SHA256;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actKDFX963SHA512 wsKDFX963SHA512;
} eslt_WorkSpaceKDFX963SHA512;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actHKDFHASHSTRUCT wsHKDF;
}eslt_WorkSpaceHKDFHASH;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actHKDFSTRUCT wsHKDF;
}eslt_WorkSpaceHKDFHMAC;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actECPSTRUCT wsEcP;
} eslt_WorkSpaceEcP;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actSPAKE2PSTRUCT workSpaceSpake;
  actCMACAESSTRUCT workSpaceCMAC;
  actHKDFSTRUCT workSpaceHKDF;
  actSHA256STRUCT workSpaceSHA;
} eslt_WorkSpaceSPAKE2P;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actSPAKE2PPreambleStruct wsSPAKEPreamble;
} eslt_WorkSpaceSPAKE2PPreamble;

/***************************************************************************
 * RSA primitive workspaces
 **************************************************************************/
typedef struct
{
  eslt_WorkSpaceHeader header;
  actRSAPRIMSTRUCT wsRSAPrimBuf;
} eslt_WorkSpaceRSAenc_prim;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actRSAPRIMSTRUCT wsRSAPrimBuf;
} eslt_WorkSpaceRSAdec_prim;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actRSACRTSTRUCT CRTStruct;
} eslt_WorkSpaceRSACRTdec_prim;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actRSAPRIMSTRUCT wsRSAPrimBuf;
} eslt_WorkSpaceRSAsig_prim;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actRSACRTSTRUCT CRTStruct;
} eslt_WorkSpaceRSACRTsig_prim;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actRSAPRIMSTRUCT wsRSAPrimBuf;
} eslt_WorkSpaceRSAver_prim;

typedef union
{  /* PRQA S 0750 */ /* MD_MSR_Union */
# if ( VSECPRIM_RSA_OAEP_SHA2_256_ENABLED == STD_ON || VSECPRIM_RSA_V15_SHA2_256_ENABLED == STD_ON || VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON ) /* COV_VSECPRIM_NO_SAFE_CONFIG TF xf xf tf */
  eslt_WorkSpaceSHA256 SHA256ws;
# endif
# if ( VSECPRIM_RSA_V15_RIPEMD160_ENABLED == STD_ON || VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON ) /* COV_VSECPRIM_NO_SAFE_CONFIG XF xf xf */
  eslt_WorkSpaceRIPEMD160 RIPEMD160ws;
# endif
  eslt_WorkSpaceSHA1 SHA1ws;
} eslt_WorkSpaceUnionHashFunctions_RSA;

/* Buffer for en- and decoding (OAEP and PSS) of the message
Memory layout
EM = [ maskedDB | saltedHash | 0xBC]
-> length of the encoded message chosen to be length of public modulus in byte
DB = [ 0-Padding string | 0x01 | salt ]
-> length of padding string chosen such that DBLen = emLen - hashDigestLen - 1
-> DBLen = maskedDBLen;
choose hashDigestLen to be the minimal digest length among all available hash functions for RSA PSS
tempVariables = hash values for MGF
-> choose length of tempVariables to be the maximal hash digest length among all
available hash functions for RSA PSS */

typedef struct
{
  eslt_Byte encodedMessage[Esl_BitsToBytes(ESL_RSA_KEYSIZE)];
  eslt_Byte dataBlock[Esl_BitsToBytes(ESL_RSA_KEYSIZE) - ESL_SIZEOF_SHA1_DIGEST - 1u];
  eslt_Byte tempVariables[ESL_SIZEOF_SHA256_DIGEST];
}eslt_EMBuffer;

typedef struct
{
  eslt_WorkSpaceHeader header;
  VSECPRIM_P2VAR_PARA(eslt_EMBuffer) encodedMessageBufferPtr;
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceUnionHashFunctions_RSA) wsHashUnionPtr;
  VSECPRIM_P2VAR_PARA(eslt_Length) modulusInformationPtr;
} eslt_WorkSpace;

/***************************************************************************
 * RSA PKCS1 v15 Encryption and Decryption workspaces
 **************************************************************************/
typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_Byte messageBuffer[Esl_BitsToBytes(ESL_RSA_KEYSIZE)];
  eslt_WorkSpaceRSAenc_prim wsRSAEncPrim;
  eslt_Length modulusSizeInformation;
} eslt_WorkSpaceRSAenc;

typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_Byte messageBuffer[Esl_BitsToBytes(ESL_RSA_KEYSIZE)];
  eslt_WorkSpaceRSAdec_prim wsRSADecPrim;
  eslt_Length modulusSizeInformation;
} eslt_WorkSpaceRSAdec;

typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_Byte wsEncoding[Esl_BitsToBytes(ESL_RSA_KEYSIZE)];
  eslt_WorkSpaceRSACRTdec_prim wsRSACRTDecPrim;
  eslt_Length primeSizesInformation;
} eslt_WorkSpaceRSACRTdec;

/***************************************************************************
 * RSA PKCS1 v15 Signature generation and verification workspaces
 **************************************************************************/
typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_Byte messageBuffer[Esl_BitsToBytes(ESL_RSA_KEYSIZE)];
  eslt_WorkSpaceUnionHashFunctions_RSA wsHashUnion;
  eslt_WorkSpaceRSAsig_prim wsRSASigPrim;
  eslt_Length modulusSizeInformation;
} eslt_WorkSpaceRSAsig;

typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_Byte wsEncoding[Esl_BitsToBytes(ESL_RSA_KEYSIZE)];
  eslt_WorkSpaceUnionHashFunctions_RSA wsHashUnion;
  eslt_WorkSpaceRSACRTsig_prim wsRSACRTSigPrim;
  eslt_Length primeSizesInformation;
} eslt_WorkSpaceRSACRTsig;

typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_Byte messageBuffer[Esl_BitsToBytes(ESL_RSA_KEYSIZE)];
  eslt_WorkSpaceUnionHashFunctions_RSA wsHashUnion;
  eslt_WorkSpaceRSAver_prim wsRSAVerPrim;
  eslt_Length modulusSizeInformation;
} eslt_WorkSpaceRSAver;

/***************************************************************************
 * RSA PKCS1 OAEP Encryption and Decryption workspaces
 **************************************************************************/

typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_EMBuffer encodedMessageBuffer;
  eslt_WorkSpaceUnionHashFunctions_RSA wsHashUnion;
  eslt_WorkSpaceRSAenc_prim wsRSAEncPrim;
  eslt_Length modulusSizeInformation;
} eslt_WorkSpaceRSAOAEPenc;

typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_EMBuffer encodedMessageBuffer;
  eslt_WorkSpaceUnionHashFunctions_RSA wsHashUnion;
  eslt_WorkSpaceRSAdec_prim wsRSADecPrim;
  eslt_Length modulusSizeInformation;
} eslt_WorkSpaceRSAOAEPdec;

typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_EMBuffer encodedMessageBuffer;
  eslt_WorkSpaceUnionHashFunctions_RSA wsHashUnion;
  eslt_WorkSpaceRSACRTdec_prim wsRSACRTDecPrim;
  eslt_Length modulusSizeInformation;
} eslt_WorkSpaceRSACRTOAEPdec;

/***************************************************************************
 * RSA SSA PSS Signature generation and verification workspaces
 **************************************************************************/

typedef struct
{
  eslt_WorkSpaceHeader  header;
  eslt_EMBuffer encodedMessageBuffer;
  eslt_WorkSpaceRSAsig_prim wsRSASigPrim;
  eslt_WorkSpaceUnionHashFunctions_RSA wsHashUnion;
  eslt_Length modulusSizeInformation;
} eslt_WorkSpaceRSAPSSsig;

typedef struct
{
  eslt_WorkSpaceHeader  header;
  eslt_EMBuffer encodedMessageBuffer;
  eslt_WorkSpaceRSAver_prim wsRSAVerPrim;
  eslt_WorkSpaceUnionHashFunctions_RSA wsHashUnion;
  eslt_Length modulusSizeInformation;
} eslt_WorkSpaceRSAPSSver;

/***************************************************************************
 * EdDSA & ECDH workSpaces
 ***************************************************************************/

typedef struct
{
  eslt_WorkSpaceHeader header;
  actEd25519STRUCT wsEd25519buffer;
  VSECPRIM_P2VAR_PARA(eslt_Byte) wsEd25519; /* Backward compatible pointer */
} eslt_WorkSpaceEd25519;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actX25519STRUCT wsX25519buffer;
  VSECPRIM_P2VAR_PARA(eslt_Byte) wsX25519; /* Backward compatible pointer */
} eslt_WorkSpaceX25519;

/* ChaCha20 and Poly1305 workSpaces */
typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_Byte wsChaCha20 [ESL_SIZEOF_WS_ChaCha20];
} eslt_WorkSpaceChaCha20;

typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_Byte wsPoly1305 [ESL_SIZEOF_WS_Poly1305];
} eslt_WorkSpacePoly1305;

typedef struct
{
  eslt_WorkSpaceHeader header;
  eslt_Byte wsAEADChaChaPoly [ESL_SIZEOF_WS_AEAD_ChaCha_Poly];
} eslt_WorkSpaceChaChaPoly;

typedef struct
{
  eslt_WorkSpaceHeader header;
  actAESCCMSTRUCT wsAESCCM;
} eslt_WorkSpaceAESCCM;

/* EC-B/D workSpace */
typedef struct
{
  eslt_WorkSpaceHeader header;
  actECBDstruct wsECBD;
} eslt_WorkSpaceECBD;

#endif /* __ESLib_t_H__ */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_t.h
 *********************************************************************************************************************/
