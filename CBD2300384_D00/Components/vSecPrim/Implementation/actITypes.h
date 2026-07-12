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
/*!        \file  actITypes.h
 *        \brief  Basic type definitions.
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library actCLib
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

#ifndef ACTITYPES_H
# define ACTITYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actPlatformTypes.h"
# include "actConfig.h"

/****************************************************************************
 ** Types and constants
 ***************************************************************************/

/* basic unsigned int types */
typedef actPlatformUint8 actU8;
typedef actPlatformUint16 actU16;
typedef actPlatformUint32 actU32;
# ifdef ACT_PLATFORM_UINT64_AVAILABLE
typedef actPlatformUint64 actU64;
# endif

# ifndef TRUE /* COV_VSECPRIM_STD_VALUES */
#  define TRUE                                                        (1u)
# endif

# ifndef FALSE /* COV_VSECPRIM_STD_VALUES */
#  define FALSE                                                       (0u)
# endif

typedef actU32 actLengthType;
typedef actLengthType actBNLENGTH;
typedef unsigned char actBoolean; /*        TRUE .. FALSE           */

typedef actLengthType actDRBGReseedCounterType;

/* Pointer to key data usually located in ROM but moved to RAM for special use-case, (e.g. RSA keys) */
VSECPRIM_P2CONST_TYPE(VSECPRIM_NONE, actU8) actPKey;

/* Pointer to byte(s) located in ROM, e.g. digest information */
VSECPRIM_P2ROMCONST_TYPE(VSECPRIM_NONE, actU8) actPROMU8;

/****************************************************************************
 ** Main arithmetic data types:    actLengthType, actBNDIGIT, actBNDDIGIT
 **
 **  A long number is represented as a little endian array of actBNDIGITs,
 **  the length of this array will be represented as actLengthType.
 **
 ** CAUTION:
 **  These types are strongly dependend of the used hardware platform!
 **  The arithmetic module is designed to work on 8-Bit, 16-Bit and 32-Bit
 **  platforms, which means actBNDIGIT should be an unsigned type of this
 **  size. Basically your choice have to fulfill these conditions:
 **
 ** MAJOR CONDITIONS:
 **  - actLengthType is a unsigned type
 **  - actBNDIGIT and actBNDDIGIT are unsigned types
 **  - an actBNDIGIT has 8, 16 or 32 bits
 **  - an actBNDDIGIT is exactly twice as long as an actBNDIGIT
 **  - for efficiency the processor should be able to multiply (add) two
 **    actDIGITs into one actBNDDIGIT
 ****************************************************************************
 **
 ** !!! NOTE !!! NOTE !!! NOTE !!!
 **
 **  The arithmetic modules were successfully tested on various 16-Bit and
 **  32-Bit platforms, which are the recommended platforms. On these
 **  platforms, switches to 8-Bit digit size were also tested, but not on
 **  a 'real' 8-Bit platform!
 **  If an 'int' has only 8 bit, there may be some 'length problems' left
 **  for numbers with more than 127 bytes (if actLengthType is an 'unit32').
 **  Also, functions of type 'int' may have incorrect return values (>127).
 ***************************************************************************/

/*
The BigNum digit and double digit types:
 */
# define actBN_BITS_PER_DIGIT                                         (ACT_PLATFORM_BITS_PER_DIGIT)
# define actBN_BYTES_PER_DIGIT                                        ((actBN_BITS_PER_DIGIT)>>3)

 /* calculate digit length from bit length */
# define actBNGetDigitLengthFromBit(bit_len)                          (((bit_len)+actBN_BITS_PER_DIGIT-1u)/actBN_BITS_PER_DIGIT)

# if (actBN_BITS_PER_DIGIT == 8) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  define actBN_BITS_PER_DIGIT_IS_VALID
/*  8-Bit digits, 16-Bit digits */
typedef actU8 actBNDIGIT;
typedef actU16 actBNDDIGIT;
# endif
# if (actBN_BITS_PER_DIGIT == 16)
#  define actBN_BITS_PER_DIGIT_IS_VALID
/* 16-Bit digits, 32-Bit digits */
typedef actU16 actBNDIGIT;
typedef actU32 actBNDDIGIT;
# endif
# if (actBN_BITS_PER_DIGIT == 32)
#  define actBN_BITS_PER_DIGIT_IS_VALID
/* 32-Bit digits, 64-Bit digits */
#  if defined(ACT_PLATFORM_UINT64_AVAILABLE)
typedef actU32 actBNDIGIT;
typedef actU64 actBNDDIGIT;
#  else
#   error UNABLE TO DEFINE 64 BIT DOUBLE DIGIT TYPE (NOT AVAILABLE)
#  endif
# endif
# ifndef actBN_BITS_PER_DIGIT_IS_VALID
#  error actBN_BITS_PER_DIGIT value unsupported
# endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Our returncodes are actBYTEs */
typedef actU32 actRETURNCODE;

/* no failures */
# define actOK                                                        (0x00u)
/* no failures and no further computation necessary */
# define actDONE                                                      (0x02u)
/* internal error during computation */
# define actEXCEPTION_UNKNOWN                                         (0x01u)

# define actCOMPARE_SMALLER                                           (0x10u)
# define actCOMPARE_EQUAL                                             (0x11u)
# define actCOMPARE_LARGER                                            (0x12u)

/* null pointer exception */
# define actEXCEPTION_NULL                                            (0xFFu)
/* invalid curve domain or extension data (decoding error) */
# define actEXCEPTION_DOMAIN                                          (0xFEu)
# define actEXCEPTION_DOMAIN_EXT                                      (0xFDu)
# define actEXCEPTION_SPEEDUP_EXT                                     (0xFCu)
/* invalid private key d (d==0 || d>=n) */
# define actEXCEPTION_PRIVKEY                                         (0xFBu)
/* invalid point (invalid TAG or not on curve) */
# define actEXCEPTION_POINT                                           (0xFAu)
/* invalid public key exception */
# define actEXCEPTION_PUBKEY                                          (0xF9u)
/* invalid length */
# define actEXCEPTION_LENGTH                                          (0xF8u)
/* invalid algorithm identifier */
# define actEXCEPTION_ALGOID                                          (0xF7u)
/* unpad error */
# define actEXCEPTION_PADDING                                         (0xF6u)
/* bad input length */
# define actEXCEPTION_INPUT_LENGTH                                    (0xF5u)
/* workspace or buffer too small */
# define actEXCEPTION_MEMORY                                          (0xF4u)
/* signature does NOT verify */
# define actVERIFICATION_FAILED                                       (0xF3u)
/* invalid operating mode */
# define actEXCEPTION_MODE                                            (0xF2u)
/* random generator failed */
# define actEXCEPTION_RANDOM                                          (0xF1u)
/* entropy exhausted, DRBG instance requires reseed */
# define actEXCEPTION_ENTROPY_EXHAUSTION                              (0xF0u)
/* output buffer too small */
# define actEXCEPTION_OUTPUT_BUFFER                                   (0xEFu)

/* Padding mode */
# define actPADDING_PM_OFF                                            (0x00u)
# define actPADDING_PM_ONEWITHZEROES                                  (0x10u)
/* PKCS #7 */
# define actPADDING_PM_PKCS7                                          (0x20u)

# define actPADDING_PM_MASK                                           (0xF0u)

/****************************************
 * AES
 ****************************************/
# define actAES_BLOCK_SIZE                                            (16u)

/* Block mode */
/* ECB */
# define actAES_BM_ECB                                                (0x00u)
/* CBC */
# define actAES_BM_CBC                                                (0x01u)
/* CTR */
# define actAES_BM_CTR                                                (0x04u)

# define actAES_BM_MASK                                               (0x0Fu)

/* Check block mode mask against padding mode mask (== 0) */
# if ((actAES_BM_MASK & actPADDING_PM_MASK) != 0x00u)
#  error Bits of actAES_BM_MASK and actPADDING_PM_MASK overlap
# endif

/* Compatibility modes */
/* ECB with PKCS #7 */
# define actAES_ECB                                                   (actAES_BM_ECB | actPADDING_PM_PKCS7)
/* CBC with PKCS #7 */
# define actAES_CBC                                                   (actAES_BM_CBC | actPADDING_PM_PKCS7)

/****************************************
 * SHA
 ****************************************/
# define actHASH_SIZE_SHA                                             (20u)
# define actHASH_BLOCK_SIZE_SHA                                       (64u)

# define actHASH_SIZE_SHA224                                          (28u)
# define actHASH_SIZE_SHA256                                          (32u)
# define actHASH_BLOCK_SIZE_SHA256                                    (64u)

# define actHASH_SIZE_SHA512_224                                      (28u)
# define actHASH_SIZE_SHA512_256                                      (32u)
# define actHASH_SIZE_SHA384                                          (48u)
# define actHASH_SIZE_SHA512                                          (64u)
# define actHASH_BLOCK_SIZE_SHA512                                    (128u)

# define actHASH_SIZE_SHA3_224                                        (224u / 8u)
# define actHASH_SIZE_SHA3_256                                        (256u / 8u)
# define actHASH_SIZE_SHA3_384                                        (384u / 8u)
# define actHASH_SIZE_SHA3_512                                        (512u / 8u)

# define actHASH_SHA3_WIDTH                                           (1600u)    /*  width of the permutation; fixed to 1600 for SHA3 */
# define actHASH_SHA3_LANE                                            (64u)      /*  64 bits in a LANE  */

/* Padding values for byte-aligned messages; see FIPS PUB 202, Table 6 */
# define actHASH_SHA3_PAD_1_SHA3                                      (0x06)    /*  with domain separation suffix   '01'  */
# define actHASH_SHA3_PAD_1_SHAKE                                     (0x1F)    /*  with domain separation suffix '1111'  */
# define actHASH_SHA3_PAD_1_cSHAKE                                    (0x04)    /*  with domain separation suffix   '00'  */
# define actHASH_SHA3_PAD_2                                           (0x80)    /*  ... remaining pad10*1 padding         */

/****************************************
 * RIPEMD160
 ****************************************/
# define actHASH_SIZE_RMD160                                          (20u)
# define actHASH_BLOCK_SIZE_RMD160                                    (64u)

/****************************************
 * KDF2
 ****************************************/
# ifdef actHASH_SIZE_SHA_KDF2 /* COV_VSECPRIM_STD_VALUES */
# else
#  if VSECPRIM_ACTKDF2HMACSHA256_ENABLED /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#   define actHASH_SIZE_SHA_KDF2                                      actHASH_SIZE_SHA256
#  else
#   define actHASH_SIZE_SHA_KDF2                                      actHASH_SIZE_SHA
#  endif
# endif

# if ((VSECPRIM_ACTKDF2HMACSHA1_ENABLED == STD_ON) || (VSECPRIM_ACTKDF2HMACSHA256_ENABLED == STD_ON)) /* COV_VSECPRIM_NO_SAFE_CONFIG XF xf xf */
#  define VSECPRIM_ACTKDF2_ENABLED                                    STD_ON
# else
#  define VSECPRIM_ACTKDF2_ENABLED                                    STD_OFF
# endif

/****************************************
 * HKDF
 ****************************************/
# define actHKDF_ONESTEPMETHOD                                        (0x01u)
# define actHKDF_TWOSTEPMETHOD                                        (0x02u)

# ifdef actHASH_SIZE_SHA_HKDF /* COV_VSECPRIM_STD_VALUES */
# else
#  if VSECPRIM_ACTHKDFHMACSHA384_ENABLED
#   define actHASH_SIZE_SHA_HKDF                                      actHASH_SIZE_SHA384
#  else
#   define actHASH_SIZE_SHA_HKDF                                      actHASH_SIZE_SHA256
#  endif
# endif

# if ((VSECPRIM_ACTHKDFHMACSHA256_ENABLED == STD_ON) || (VSECPRIM_ACTHKDFHMACSHA384_ENABLED == STD_ON)) /* COV_VSECPRIM_NO_SAFE_CONFIG TF tf xf */
#  define VSECPRIM_ACTHKDF_ENABLED                                    STD_ON
# else
#  define VSECPRIM_ACTHKDF_ENABLED                                    STD_OFF
# endif

 /****************************************
 * HMAC SHA IDs for HDF AND KDF2
 ****************************************/
/* Do not change this order! These values are used to access actHashMACInitTable,
 * actHashMACUpdateTable and actHashMACFinalizeTable. */
# define actHASHMACSHA1_ALGO_ID                                       (0u)
# define actHASHMACSHA256_ALGO_ID                                     (1u)
# define actHASHMACSHA384_ALGO_ID                                     (2u)

/****************************************
 * FIPS-186.2
 ****************************************/
/* Byte_length of base 2^b, b = 160  */
# define actFIPS186_BASE_LENGTH                                       (20u)
# define actFIPS186_SEED_SIZE                                         actFIPS186_BASE_LENGTH

/****************************************
 * DRBG
 ****************************************/

# define actITypesMax(x, y)                                           (((x)>(y))? (x):(y))

# define actCTRDRBG_ALGSIZE_AES128                                    (0x01u)
# define actCTRDRBG_ALGSIZE_AES256                                    (0x03u)
# define actHASHDRBG_ALGSIZE_SHA512                                   (0x05u) /* Value chosen to keep compatibility to ESLib_t.h */

# define actCTRDRBG_DFMODE_USE_NO_DF                                  (0x00u)
# define actCTRDRBG_DFMODE_USE_BLOCKCIPHER_DF                         (0x10u)

# define actDRBG_ALGSIZE_MASK                                         (0x0Fu)
# define actDRBG_DFMODE_MASK                                          (0xF0u)

# define actDRBGMODE_CTRDRBG_AES128_DF                                (actCTRDRBG_DFMODE_USE_BLOCKCIPHER_DF + actCTRDRBG_ALGSIZE_AES128)
# define actDRBGMODE_CTRDRBG_AES128_NODF                              (actCTRDRBG_DFMODE_USE_NO_DF + actCTRDRBG_ALGSIZE_AES128)
# define actDRBGMODE_CTRDRBG_AES256_DF                                (actCTRDRBG_DFMODE_USE_BLOCKCIPHER_DF + actCTRDRBG_ALGSIZE_AES256)
# define actDRBGMODE_CTRDRBG_AES256_NODF                              (actCTRDRBG_DFMODE_USE_NO_DF + actCTRDRBG_ALGSIZE_AES256)
/* Hash DRBG always used a DF, so no case distinction is necessary */
# define actDRBGMODE_HASHDRBG_SHA512                                  (actHASHDRBG_ALGSIZE_SHA512)

/* The DRBG states are the same for HASH DRBG, CTR DRBG and HMAC DRBG */
# define actDRBG_SEED_STATE_UNSEEDED                                  (0x00u)
# define actDRBG_SEED_STATE_SEEDED                                    (0x01u)
# define actDRBG_SEED_STATE_NEED_RESEED                               (0x02u)

/* DRBG AES */
# define actAES128_KEY_SIZE                                           (16u)
# define actAES256_KEY_SIZE                                           (32u)
# define actCTRDRBG_AES128_KEY_LEN                                    (actAES128_KEY_SIZE)
# define actCTRDRBG_AES256_KEY_LEN                                    (actAES256_KEY_SIZE)

/* NIST SP800-90A 10.2.1 Table 3 Definitions for the CTR_DRBG */
/* Block length */
# define actCTRDRBG_BLOCK_LEN                                         (actAES_BLOCK_SIZE) /* same block length for CTR DRBG AES128, AES192 and AES256 */
/* Key length */
# define actAESCTRDRBG_MAX_KEY_LEN                                    actCTRDRBG_AES256_KEY_LEN
/* Seed length */
# define actAES128CTRDRBG_SEED_LEN                                    (actCTRDRBG_AES128_KEY_LEN + actCTRDRBG_BLOCK_LEN)
# define actAES256CTRDRBG_SEED_LEN                                    (actCTRDRBG_AES256_KEY_LEN + actCTRDRBG_BLOCK_LEN)
/* max seed length */
# define actAESCTRDRBG_SEED_LEN                                       (actAESCTRDRBG_MAX_KEY_LEN + actCTRDRBG_BLOCK_LEN)

/* Size of 32Bit in bytes */
# define actDRBG_SIZEOF_INPUTLENGTH                                   (sizeof(actLengthType))
/* Offset for the inputData in the S construct */
# define actAESCTRDRBG_DF_BUFFER_OFFSET                               (actDRBG_SIZEOF_INPUTLENGTH * 2u) /* 2x uint32 to store (L|N) */

/* Max. size of random number output, identical for AES128 and AES256 */
# define actAESCTRDRBG_MAX_OUTPUT_SIZE                                (1024u) /* max_number_of_bits_per_request 2^13 = 8192 =>1024 Bytes / has to be the multiple of actAESCTRDRBG_BLOCK_LEN */

# define actCTRDRBG_AES128_RESEED_INTERVAL                            VSECPRIM_DRBG_RESEED_INTERVAL
# define actHASHDRBG_RESEED_INTERVAL                                  VSECPRIM_DRBG_RESEED_INTERVAL

/* Seed length */
# define actHASHDRBG_SHA512_SEED_LEN                                  (111u) /* seed len in byte */
# define actHASHDRBG_SHA512_OUTLEN                                    (64u) /* outlen in byte */
# define actHASHDRBG_MAX_OUTPUT_SIZE                                  (65535u) /* max_number_of_bits_per_request 2^19 = 2^16 Bytes, set max output len to uint16max */

/****************************************
 * ECC
 ****************************************/
/*
As workspace for operations in GF(p) and GF(n) like point addition,
doubling, point (de)compression and field inversions we need temporary
BigNum variables, each of word (digit) length:
max_length + 1, where max_length = max(p_length, n_length)
 */

# if (VSECPRIM_ACTBNEUCLID_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/* 6 variables at minimum (4 for GCD + 2) */
#  define actEC_BIGNUM_TMP                                            actMax((actEC_MOD_EXP_TMP+2u), 6u)
# else
/* 4 variables at minimum (for actPoint Addition) */
#  define actEC_BIGNUM_TMP                                            actITypesMax((actEC_MOD_EXP_TMP+2u), 4u)
# endif

/****************************************
 * Ed25519/X25519
 ****************************************/
# define actEd25519_KEY_SIZE                                          (32u)
# define actEd25519pure                                               (0u)
# define actEd25519ctx                                                (1u)
# define actEd25519ph                                                 (2u)
# define actX25519_KEY_SIZE                                           (32u)

# define BNDIGITS_256                                                 (256u /8u /sizeof (actBNDIGIT))
# define BNDIGITS_512                                                 (512u /8u /sizeof (actBNDIGIT))
# define BNBYTES_256                                                  (256u /8u /sizeof (actU8))
# define BNBYTES_512                                                  (512u /8u /sizeof (actU8))
# define MASK_MSBit                                                   (0x7FFFFFFFuL >> (32u - actBN_BITS_PER_DIGIT))

/****************************************
 * SPAKE2P
 ****************************************/
# define actSPAKE2P_MAX_AAD_SIZE                                      VSECPRIM_SPAKE2_P_MAX_AADSIZE

/* masks */
# define actSPAKE2P_HASH_MASK                                         (0x0000Fu)
# define actSPAKE2P_KDF_MASK                                          (0x000F0u)
# define actSPAKE2P_MAC_MASK                                          (0x00F00u)
# define actSPAKE2P_VERSION_MASK                                      (0x0F000u)
# define actSPAKE2P_CURVE_MASK                                        (0xF0000u)

/* modes */
# define actSPAKE2P_HASHFCT_SHA256                                    (0x0001u) /* value chosen to be consistent with values in ESLib_t.h */
# define actSPAKE2P_KDF_HKDF_HMAC_SHA256                              (0x0010u)
# define actSPAKE2P_MAC_CMAC128                                       (0x0100u)

/* Key A first means that AAD -> KDF(nil, Ka, "ConfirmationKeys" || AAD) = KcA || KcB (newer version, according to www.tools.ietf.org/id/draft-bar-cfrg-spake2plus-00.html)
 * Key B first means that AAD -> KDF(nil, Ka, "ConfirmationKeys" || AAD) = KcB || KcA (older version, according to CCC R2 v1.0.0) */
# define actSPAKE2P_VERSION_KEY_A_FIRST                               (0x1000u)
# define actSPAKE2P_VERSION_KEY_B_FIRST                               (0x2000u)

# define actSPAKE2P_CURVE_P256                                        (0x30000u)

# define actSPAKE2P_MODE_CIPHERSUITE_8_1                              (actSPAKE2P_CURVE_P256 + actSPAKE2P_VERSION_KEY_A_FIRST + actSPAKE2P_HASHFCT_SHA256 + actSPAKE2P_KDF_HKDF_HMAC_SHA256 + actSPAKE2P_MAC_CMAC128)
# define actSPAKE2P_MODE_CIPHERSUITE_8_2                              (actSPAKE2P_CURVE_P256 + actSPAKE2P_VERSION_KEY_B_FIRST + actSPAKE2P_HASHFCT_SHA256 + actSPAKE2P_KDF_HKDF_HMAC_SHA256 + actSPAKE2P_MAC_CMAC128)

/* preamble phase */
# define actSPAKE2P_PREAMBLE_OKM_LENGTH                               (VSECPRIM_SPAKE2_P_PREAMBLE_OKMLENGTH) /* value in bytes */
/* +1 in case the configured value is odd */
# define actSPAKE2P_PREAMBLE_WDIGIT_LENGTH                            ((((actSPAKE2P_PREAMBLE_OKM_LENGTH + 1u)/2u) + actBN_BYTES_PER_DIGIT - 1u)/actBN_BYTES_PER_DIGIT)

/****************************************
 * SipHash
 ****************************************/
/* Configure SipHash-2-4 */
# define ACT_SIPHASH_CROUNDS                                          (2u)
# define ACT_SIPHASH_DROUNDS                                          (4u)

# define actSIPHASH_BLOCK_SIZE                                        (8u)

/****************************************
 * GHash
 ****************************************/
/*
###########################################################################
##
##   LIMIT SPEED UP SETTING!
##   Please read the "library/es Porting Guide"
##   on how to further speed up GHash!
##
###########################################################################
 */
# if (actGHASH_SPEED_UP > 1)  /* COV_VSECPRIM_GHASH_SPEED_UP XF */
#  define actGHASH_SPEED_UP                                           1 /*  limit speedup  */
# endif

# define actGHASH_BLOCK_SIZE                                          (16u)
# define actGHASH_WORD_SIZE                                           (4u)
# define actGHASH_WORDS_PER_BLOCK                                     (actGHASH_BLOCK_SIZE/actGHASH_WORD_SIZE)

/****************************************
 * GHash
 ****************************************/
# define actGCM_BLOCK_SIZE                                            16u
# define actGCM_BYTE_SIZE                                             8u
# define actGCM_IV_DEFAULT_SIZE                                       12u
# define actGCM_TAG_LENGTH                                            16u

/* RSA */
# if (vSecPrim_GetRSAMaxKeySizeOfGeneral() == VSECPRIM_RSA_MAX_KEY_SIZE_512) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  define actRSA_MAX_KEY_SIZE_IN_DIGIT                                (512u/actBN_BITS_PER_DIGIT)
# elif (vSecPrim_GetRSAMaxKeySizeOfGeneral() == VSECPRIM_RSA_MAX_KEY_SIZE_1024) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  define actRSA_MAX_KEY_SIZE_IN_DIGIT                                (1024u/actBN_BITS_PER_DIGIT)
# elif (vSecPrim_GetRSAMaxKeySizeOfGeneral() == VSECPRIM_RSA_MAX_KEY_SIZE_1536) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  define actRSA_MAX_KEY_SIZE_IN_DIGIT                                (1536u/actBN_BITS_PER_DIGIT)
# elif (vSecPrim_GetRSAMaxKeySizeOfGeneral() == VSECPRIM_RSA_MAX_KEY_SIZE_2048)
#  define actRSA_MAX_KEY_SIZE_IN_DIGIT                                (2048u/actBN_BITS_PER_DIGIT)
# elif (vSecPrim_GetRSAMaxKeySizeOfGeneral() == VSECPRIM_RSA_MAX_KEY_SIZE_3072) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  define actRSA_MAX_KEY_SIZE_IN_DIGIT                                (3072u/actBN_BITS_PER_DIGIT)
# elif (vSecPrim_GetRSAMaxKeySizeOfGeneral() == VSECPRIM_RSA_MAX_KEY_SIZE_4096)
#  define actRSA_MAX_KEY_SIZE_IN_DIGIT                                (4096u/actBN_BITS_PER_DIGIT)
# else
#  error Invalid maximum Key size for RSA selected
# endif

/* It holds that p_bit + q_bit = n_bit, but p_bit and q_bit do not have to be equal.
 * NOTE: All CRT size values assume: p_bits, q_bits <= n_bits/2 + 32,
 * i.e., one prime can be up to  32 bit longer than n_bit/2, but in turn the other prime has to be short than n_bit/2
 * That means that p and q together can be one, two or four digits longer than the modulus in digits
 * (depending on the configuration, i.e., the value of actBN_BITS_PER_DIGIT) */
# define actRSA_CRT_PRIME_SIZE                                        (((actRSA_MAX_KEY_SIZE_IN_DIGIT + 1u) / 2u) + (32u / actBN_BITS_PER_DIGIT))

/* Assumption: the larger prime of p and q is ((module length+1)/2) + 32 bit long
 * The roles of p and q need to be interchangeable, so whenever a buffer is used that is supposed
 * to store a value of p_length and a value of q_length, we need a buffer of total length
 * ((module length+1)/2) + 2*32 bit */
# define actRSA_CRT_PRIMEPAIR_SIZE                                    (actRSA_MAX_KEY_SIZE_IN_DIGIT + (32u / actBN_BITS_PER_DIGIT))

# if (vSecPrim_GetECPMaxKeySizeOfGeneral() >= VSECPRIM_ECP_MAX_KEY_SIZE_521) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  define actECC_MAX_P_DIGIT_LEN                                      (actBNGetDigitLengthFromBit(521u))
#  define actECC_MAX_N_DIGIT_LEN                                      (actBNGetDigitLengthFromBit(521u))
#  define actECC_MAX_DIGIT_LEN                                        (actBNGetDigitLengthFromBit(521u))
#  define actECC_MAX_BYTE_LEN                                         (66u)
# elif (vSecPrim_GetECPMaxKeySizeOfGeneral() >= VSECPRIM_ECP_MAX_KEY_SIZE_512) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  define actECC_MAX_P_DIGIT_LEN                                      (512u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_N_DIGIT_LEN                                      (512u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_DIGIT_LEN                                        (512u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_BYTE_LEN                                         (64u)
# elif (vSecPrim_GetECPMaxKeySizeOfGeneral() >= VSECPRIM_ECP_MAX_KEY_SIZE_384)
#  define actECC_MAX_P_DIGIT_LEN                                      (384u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_N_DIGIT_LEN                                      (384u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_DIGIT_LEN                                        (384u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_BYTE_LEN                                         (48u)
# elif (vSecPrim_GetECPMaxKeySizeOfGeneral() >= VSECPRIM_ECP_MAX_KEY_SIZE_320) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  define actECC_MAX_P_DIGIT_LEN                                      (320u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_N_DIGIT_LEN                                      (320u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_DIGIT_LEN                                        (320u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_BYTE_LEN                                         (40u)
# elif (vSecPrim_GetECPMaxKeySizeOfGeneral() >= VSECPRIM_ECP_MAX_KEY_SIZE_256) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
#  define actECC_MAX_P_DIGIT_LEN                                      (256u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_N_DIGIT_LEN                                      (256u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_DIGIT_LEN                                        (256u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_BYTE_LEN                                         (32u)
# elif (vSecPrim_GetECPMaxKeySizeOfGeneral() >= VSECPRIM_ECP_MAX_KEY_SIZE_224) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  define actECC_MAX_P_DIGIT_LEN                                      (224u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_N_DIGIT_LEN                                      (224u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_DIGIT_LEN                                        (224u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_BYTE_LEN                                         (28u)
# elif (vSecPrim_GetECPMaxKeySizeOfGeneral() >= VSECPRIM_ECP_MAX_KEY_SIZE_192) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  define actECC_MAX_P_DIGIT_LEN                                      (192u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_N_DIGIT_LEN                                      (192u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_DIGIT_LEN                                        (192u/actBN_BITS_PER_DIGIT)
#  define actECC_MAX_BYTE_LEN                                         (24u)
# else /* (vSecPrim_GetECPMaxKeySizeOfGeneral() <= VSECPRIM_ECP_MAX_KEY_SIZE_160) */
#  define actECC_MAX_P_DIGIT_LEN                                      (160u/actBN_BITS_PER_DIGIT)        /* p length in bit: 160 */
#  define actECC_MAX_N_DIGIT_LEN                                      (actBNGetDigitLengthFromBit(161u)) /* n length in bit: 161 */
#  define actECC_MAX_DIGIT_LEN                                        (actBNGetDigitLengthFromBit(161u))
#  define actECC_MAX_BYTE_LEN                                         (21u)
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/* expand a 32-bit value into a set of actBNDIGITs depending upon the actBNDIGIT size */

# if (8 == actBN_BITS_PER_DIGIT) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  define DIGITs(value)                                               ((actBNDIGIT) ((value) >>  0)), \
                                                                      ((actBNDIGIT)((value) >> 8)), \
                                                                      ((actBNDIGIT)((value) >> 16)), \
                                                                      ((actBNDIGIT)((value) >> 24))
# elif (16 == actBN_BITS_PER_DIGIT)
#  define DIGITs(value)                                               ((actBNDIGIT) ((value) >>  0)), \
                                                                      ((actBNDIGIT)((value) >> 16))
# elif (32 == actBN_BITS_PER_DIGIT)
#  define DIGITs(value)                                               (value)
# else
#  error actBN_BITS_PER_DIGIT value unsupported
# endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* Context structure of the AES algorithm */
typedef struct
{
  actU32 e_key[64];
  actLengthType key_dword_len;
  actLengthType buffer_used;
  actU8 prev_block[actAES_BLOCK_SIZE];
  actU8 buffer[actAES_BLOCK_SIZE];
  actU8 mode;
} actAESSTRUCT;

/* Workspace structure for AES CMAC */
typedef actAESSTRUCT actCMACAESSTRUCT;

/* Workspace structure for SHA-1 */
typedef struct
{
  actU32 H[5];                  /* message digest state buffer */ /* PRQA S 0784 */ /* MD_VSECPRIM_5.6 */ /* Justification only applies for QM_ONLY part of the component */
  actU32 low_count, hi_count;   /* 64 bit input count */
  actLengthType buffer_used;              /* number of bytes saved in buffer */
  actU8 buffer[actHASH_BLOCK_SIZE_SHA]; /* remaining data buffer */
} actSHASTRUCT;

/* Workspace structure for SHA-224 & SHA-256 */
typedef struct
{
  actU32 H[8]; /* message digest state buffer */
  actU32 low_count, hi_count; /* 64 bit input count */
  actLengthType buffer_used; /* number of bytes saved in buffer */
  actU16 result_length; /* number of bytes to return as hash-value */
  actU8 buffer[actHASH_BLOCK_SIZE_SHA256]; /* remaining data buffer */
} actSHA224STRUCT, actSHA256STRUCT;

/* Workspace structure for SHA-384 & SHA-512 */
typedef struct
{
# ifdef ACT_PLATFORM_UINT64_AVAILABLE
  actU64 H[8]; /* message digest state buffer */
  actU64 count_L, count_H; /* 128 bit input count */
# else
  actU32 H[16]; /* message digest state buffer */
  actU32 count_L, count_2, count_3, count_H; /* 128 bit input count */
# endif
  actLengthType buffer_used; /* number of bytes saved in buffer */
  actU16 result_length; /* number of bytes to return as hash-value */
  actU8 buffer[actHASH_BLOCK_SIZE_SHA512]; /* remaining data buffer */
} actSHA512_224STRUCT, actSHA512_256STRUCT, actSHA384STRUCT, actSHA512STRUCT;

/* Workspace structure for SHA3 */
# ifdef ACT_PLATFORM_UINT64_AVAILABLE
typedef struct
{
  actU64  state[actHASH_SHA3_WIDTH/64u];     /* KECCAK[c] state  -  keep aligned on a 64bit boundary (the structure itself WILL be)!!  */
  actLengthType  strength;                   /* algorithm strength / digest size (BITs)  */
  actLengthType  rUsed;                      /* number of lanes absorbed / squeezed  */
  actLengthType  dUsed;                      /* number of bytes absorbed / squeezed  */
  actLengthType  rate;                       /* KECCEC[c] rate (r)     - LANEs (64 bit words), equals the input block size  */
  actU8   digest[actHASH_SIZE_SHA3_512];     /* tmp buffer for hash value */
  actU8   data[actHASH_SHA3_LANE >> 3];      /* data buffer - single lane deep  */
  actU8   pad1;                              /* 1st padding byte; includes domain separation suffix  */
  actU8   phase;                             /* KECCAK[c] phase (absorb / squeeze)  */
} actKECCAKcSTRUCT;
# else
typedef struct
{
  actU32  state[actHASH_SHA3_WIDTH/32u];     /* KECCAK[c] state  */
  actLengthType strength;                    /* algorithm strength / digest size (BITs)  */
  actLengthType  rUsed;                      /* number of lanes absorbed / squeezed  */
  actLengthType  dUsed;                      /* number of bytes absorbed / squeezed  */
  actLengthType  rate;                       /* KECCEC[c] rate (r)     - LANEs (64 bit words), equals the input block size  */
  actU8   digest[actHASH_SIZE_SHA3_512];     /* tmp buffer for hash value */
  actU8   data[actHASH_SHA3_LANE >> 3];      /* data buffer - single lane deep  */
  actU8   pad1;                              /* 1st padding byte; includes domain separation suffix  */
  actU8   phase;                             /* KECCAK[c] phase (absorb / squeeze)  */
} actKECCAKcSTRUCT;
# endif

/* Workspace structure for RMD160 */
typedef struct
{
   actU32 H[5];                  /* message digest state buffer */
   actU32 low_count, hi_count;   /* 64 bit input count */
   actLengthType buffer_used;              /* number of bytes saved in buffer */
   actU8 buffer[actHASH_BLOCK_SIZE_RMD160];     /* remaining data buffer */
} actRMD160STRUCT;

/* Workspace structure for HashMAC SHA-1 */
typedef struct
{
  actSHASTRUCT sha;
  actU8 key_buf[actHASH_BLOCK_SIZE_SHA];
  actLengthType key_length;
} actHASHMACSTRUCT;

/* Workspace structure for HashMAC SHA-256 */
typedef struct
{
  actSHA256STRUCT sha256;
  actU8 key_buf[actHASH_BLOCK_SIZE_SHA256];
  actLengthType key_length;
} actHASHMACSHA256STRUCT;

/* Workspace structure for HashMAC SHA-384 */
typedef struct
{
   actSHA384STRUCT sha384;
   actU8 key_buf[actHASH_BLOCK_SIZE_SHA512];
   actLengthType key_length;
} actHASHMACSHA384STRUCT;

/* Workspace structure for HashMAC RMD160 */
typedef struct
{
   actRMD160STRUCT rmd160;
   actU8 key_buf[actHASH_BLOCK_SIZE_RMD160];
   actLengthType key_length;
} actHASHMACRMD160STRUCT;

typedef union
{ /* PRQA S 0750 */ /* MD_MSR_Union */
# if VSECPRIM_ACTKDF2HMACSHA256_ENABLED == STD_ON /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  actHASHMACSHA256STRUCT hmac_sha256;
# endif
  actHASHMACSTRUCT hmac_sha1;
}actHASHMACUNION_KDF2;

/* Workspace structure for KDF2 */
typedef struct
{
  actU32 iteration_count;
  actHASHMACUNION_KDF2 hmac;
  /* 2 internal buffers for HMAC results: */
  actU8 U_buf[actHASH_SIZE_SHA_KDF2];
  actU8 F_buf[actHASH_SIZE_SHA_KDF2];
} actKDF2STRUCT;

/* Workspace structure for KDFX963 */
typedef struct
{
  actSHASTRUCT actwsHash;
  actU8 hashBuf[actHASH_SIZE_SHA];
} actKDFX963STRUCT;

/* Workspace structure for KDF X9.63 w/ SHA-256 */
typedef struct
{
  actSHA256STRUCT actwsHash;
  actU8           hashBuf[actHASH_SIZE_SHA256];
} actKDFX963SHA256;

/* Workspace structure for KDF X9.63 w/ SHA-512 */
typedef struct
{
  actSHA512STRUCT actwsHash;
  actU8           hashBuf[actHASH_SIZE_SHA512];
} actKDFX963SHA512;


typedef union
{ /* PRQA S 0750 */ /* MD_MSR_Union */
# if VSECPRIM_ACTHKDFHMACSHA384_ENABLED == STD_ON /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  actHASHMACSHA384STRUCT hmac_sha384;
# endif
  actHASHMACSHA256STRUCT hmac_sha256;
}actHASHMACUNION_HKDF;

/* Workspace for HKDF HMAC SHA256 */
typedef struct
{
  actHASHMACUNION_HKDF actwsHmac;
  union
  { /* PRQA S 0750 */ /* MD_MSR_Union */
    /* tmpKey will store an all-zero array, which is used as MAC key in case no salt is given during the call to
     * actHKDF_HMAC_SHA256_Extract. As per specification, this all-zero array should be of size
     * actHASH_BLOCK_SIZE_SHA***.
     * However, the key is automatically padded with zeros to be of size actHASH_BLOCK_SIZE_SHA*** in
     * actHashMACSHA***Init(). Thus, a smaller temporary key suffices in order to save memory. */
    actU8 tmpKey[actHASH_SIZE_SHA_HKDF];
    actU8 blockT[actHASH_SIZE_SHA_HKDF];
  }tmpBlock;
  actU8 keyDerivationKey[actHASH_SIZE_SHA_HKDF];
  actU8 methodID;
}actHKDFSTRUCT;

/* Workspace for HKDF Hash (SHA2-256) */
typedef struct
{
  actSHA256STRUCT actwsSha;
  actU8 blockT[actHASH_SIZE_SHA256];
  actU8 hashID;
}actHKDFHASHSTRUCT;

/* Workspace structure for FIPS186 */
typedef struct
{
  actLengthType bytes_remain;
  actU8 X_KEY[actFIPS186_BASE_LENGTH];
  actSHASTRUCT sha1;
} actFIPS186STRUCT;

/* Workspace structure for DRBG */
typedef struct
{
  actU8 result[actCTRDRBG_BLOCK_LEN];
  actU8 input[actCTRDRBG_BLOCK_LEN];
} actCTRDRBGTmpBufferStruct;

typedef struct
{
  actAESSTRUCT wsAES;
  union
  { /* PRQA S 0750 */ /* MD_MSR_Union */
    actU8 additionalInputPrepared[actAESCTRDRBG_SEED_LEN]; /* used during RN generation, must be exactly seedlen bits in length */
    actU8 seedData[actAESCTRDRBG_SEED_LEN];                /* used during instantiation, must be exactly seedlen bits in length */
  } tmpBuffer;
  union
  { /* PRQA S 0750 */ /* MD_MSR_Union */
    actU8 blockUpdateTmpBuffer[actAESCTRDRBG_SEED_LEN];    /* This must be exactly seedlen bits in length; used in blockUpdate -> instantiation and generation */
    actCTRDRBGTmpBufferStruct BCCTmpBuffer;                /* used in BCC -> instantiation and generation */
  } BCCUpdateTmpBuffer;
  actU8 DFencryptBuffer[actAESCTRDRBG_SEED_LEN];           /* used in DF  -> instantiation and generation */
  actU8 DFIV[actCTRDRBG_BLOCK_LEN];                        /* used in DF  -> instantiation and generation */
  union
  { /* PRQA S 0750 */ /* MD_MSR_Union */
    actU8 blockOfS[actCTRDRBG_BLOCK_LEN];                  /* used in BCC -> instantiation and generation  */
    actU8 tempAesBlock[actCTRDRBG_BLOCK_LEN];              /* used in EncryptBlock -> instantiation and generation */
  } tempBlocks;
  actU8 key[actAESCTRDRBG_MAX_KEY_LEN];                    /* part of the internal state of the DRBG */
  actU8 variable[actCTRDRBG_BLOCK_LEN];                    /* part of the internal state of the DRBG */
  actDRBGReseedCounterType reseedCount;                    /* part of the working state of the DRBG */
  actU8 mode;                                              /* [DF mode | algorithm size] (4 bits each) */
  actU8 seedState;
} actCTRDRBGSTRUCT;

typedef struct
{
  actSHA512STRUCT wsSHA512;
  actU8 DFtmp[2u * actHASH_SIZE_SHA512];                   /* used in hashDF */
  actU8 hashdata[actHASH_BLOCK_SIZE_SHA512];               /* used in hashDF, hashGen and GeneratePrepareV */
  actU8 tmpdigest[actHASH_SIZE_SHA512];                    /* used in to store hash value in generate, generatePrepareV and hashGen */
  actU8 constant[actHASHDRBG_SHA512_SEED_LEN];             /* part of the internal state of the DRBG */
  actU8 variable[actHASHDRBG_SHA512_SEED_LEN];             /* part of the internal state of the DRBG */
  actU32 reseedCount;                                      /* part of the working state of the DRBG */
  actU8 mode;                                              /* algorithm size, stored in the last 4 bits */
  actU8 seedState;
} actHASHDRBGSTRUCT;

/* Big Number Types */
/* Curve point represented in extended homogeneous coordinates. */
typedef struct
{
  actBNDIGIT x[BNDIGITS_256];
  actBNDIGIT y[BNDIGITS_256];
  actBNDIGIT z[BNDIGITS_256];
  actBNDIGIT t[BNDIGITS_256];
} actPoint;

/*
BigNum modular ring type

Contains:
- the modulus (MUST BE ODD for Montgomery !!!)
- length information of the modulus
- additional constants for Montgomery
- optimization switch for Montgomery
 */
typedef struct
{
  VSECPRIM_P2VAR_PARA(actBNDIGIT) m;  /* ring modulus */
  actLengthType m_length;             /* length of m in digit */
  actLengthType m_byte_length;        /* length of m in byte */
  VSECPRIM_P2VAR_PARA(actBNDIGIT) RR; /* RR = R^2, R = base^m_length mod m */
  actBNDIGIT m_bar;                   /* mbar = -(m^-1) mod base, needed for Montgomery product */
# if (actBN_MONT_MUL_VERSION==1)      /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  actU32 prime_structure;             /* flag for optimization in MontMul */
# endif
# if (actBN_MOD_EXP_WINDOW_SIZE==0)   /* variable k-bit window algorithm */ /* COV_VSECPRIM_NOT_CONFIGURABLE XF */
  actU32 window_size;                 /* is used for modular exponentiation */
# endif
} actBNRING;

typedef struct
{
  actBNRING p_field; /* underlying field GF(p) */
  actBNRING n_field; /* field GF(n), where n is the basepoint order */
  VSECPRIM_P2ROMCONST_PARA(actU8) a_R; /* a in Montgomery Representation */
  VSECPRIM_P2ROMCONST_PARA(actU8) b_R; /* b in Montgomery Representation */
  VSECPRIM_P2ROMCONST_PARA(actU8) G_R; /* G in Montgomery Representation */
  boolean a_equals_p_minus_3;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) t[actEC_BIGNUM_TMP]; /* temporary variables */ /* PRQA S 3494 */ /* MD_VSECPRIM_CONST_BOOL_OPERAND */ /*lint !e506 */
  actLengthType groups;
  VSECPRIM_P2ROMCONST_PARA(actU8) prec_first; /* pointer to first pre-computated point */
} actECCURVE;

/* Structs for ECDSA, ECDH and ECKG */
typedef struct
{
  actBNDIGIT privKey[actECC_MAX_N_DIGIT_LEN]; /* SIGN: private key d, VERIFY: u1 */
  actBNDIGIT ephKey[actECC_MAX_N_DIGIT_LEN];  /* SIGN: ephemeral key k, VERIFY: u2 */
} actECPPrivateKeys;

typedef struct
{
  actBNDIGIT u1[actECC_MAX_N_DIGIT_LEN + 1u]; /* SIGN: private key d, VERIFY: u1 */
  actBNDIGIT u2[actECC_MAX_N_DIGIT_LEN + 1u]; /* SIGN: ephemeral key k, VERIFY: u2 */
} actECPTmpVarVerify;

typedef struct
{
  actBNDIGIT xcoord[actECC_MAX_P_DIGIT_LEN];
  actBNDIGIT ycoord[actECC_MAX_P_DIGIT_LEN];
} actECPaffinePointBuffer;

typedef struct
{
  actBNDIGIT xcoord[actECC_MAX_P_DIGIT_LEN];
  actBNDIGIT ycoord[actECC_MAX_P_DIGIT_LEN];
  actBNDIGIT zcoord[actECC_MAX_P_DIGIT_LEN];
} actECPprojectivePointBuffer;

/* Basic Workspace for ECC, used for ECDSA Sign/Verify, EcP KeyGenerate, DH, ECBD */
typedef struct
{
  actECCURVE Curve;
  actBNDIGIT p_field_modulus[actECC_MAX_P_DIGIT_LEN];
  actBNDIGIT p_field_RR[actECC_MAX_P_DIGIT_LEN];
  actBNDIGIT n_field_modulus[actECC_MAX_N_DIGIT_LEN];
  actBNDIGIT n_field_RR[actECC_MAX_N_DIGIT_LEN];
  actBNDIGIT Curve_tmp0[actECC_MAX_DIGIT_LEN + 1u];
  actBNDIGIT Curve_tmp1[actECC_MAX_DIGIT_LEN + 1u];
  actBNDIGIT Curve_tmp2[actECC_MAX_DIGIT_LEN + 1u];
  actBNDIGIT Curve_tmp3[actECC_MAX_DIGIT_LEN + 1u];
}actECPBasicStruct;

/* struct members not aligned according to working guide line, because the basic workspace for ECC is set up during
 * actECInit, which is also called during the ECDB routine. In this case, the ECBD workspace is cast to this struct
 * and therefore, the order of struct members and entries in the ECBD workspace have to match. */
typedef struct
{
  /* Basic Workspace for ECC, used for ECDSA Sign/Verify, EcP KeyGenerate, EcP DH, ECBD */
  actECPBasicStruct baseData;

  union
  {                                                     /* PRQA S 0750 */ /* MD_MSR_Union */
    actECPTmpVarVerify u12;                             /* used during signature verification, stores u1 and u2 */
    actECPPrivateKeys privKeys;                         /* used during signature generation, key generation and Diffie Hellman, stores the private and the ephemeral key */
  } dataPair;

  union
  {                                                     /* PRQA S 0750 */ /* MD_MSR_Union */
    actBNDIGIT hashed_message[actECC_MAX_N_DIGIT_LEN];  /* used during signature generation and verification */
    actBNDIGIT secret_x[actECC_MAX_P_DIGIT_LEN];        /* used during Diffie Hellman key exchange */
  }msg;

  actBNDIGIT signature_r[actECC_MAX_N_DIGIT_LEN];
  actBNDIGIT signature_s[actECC_MAX_N_DIGIT_LEN];
  actBNDIGIT public_key_x[actECC_MAX_P_DIGIT_LEN];      /* used during signature verification and Diffie Hellman */
  actBNDIGIT public_key_y[actECC_MAX_P_DIGIT_LEN];      /* used during signature verification and Diffie Hellman */

  union
  {                                                     /* PRQA S 0750 */ /* MD_MSR_Union */
    actECPprojectivePointBuffer kG;                     /* used during signature generation ([k]G), key generation ([d]G) and Diffie Hellman ([d]Q) */
    actECPprojectivePointBuffer QG;                     /* used during signature verification, stores G + P_A */
  } multG;

  union
  {                                                     /* PRQA S 0750 */ /* MD_MSR_Union */
    actECPaffinePointBuffer precomputedPoint;           /* used during signature generation and key generation, stores a precomputed point */
    actECPaffinePointBuffer basePoint;                  /* used during signature verification, stores the base point */
  } curvePoint;

  actECPprojectivePointBuffer Q;                        /* used during signature verification, stores [u1]G + [u2]P_A */

} actECPSTRUCT;

/* SPAKE2+ struct */
typedef struct
{
  /* Basic Workspace for ECC */
  actECPBasicStruct baseData;

  /* SPAKE specific data */
  actECPprojectivePointBuffer M;
  actECPprojectivePointBuffer N;
  actECPprojectivePointBuffer L;
  actECPprojectivePointBuffer pA;
  actECPprojectivePointBuffer pB;
  actECPprojectivePointBuffer Z;
  actECPprojectivePointBuffer V;

  /* auxiliary variables */
  actECPprojectivePointBuffer kG;
  actECPprojectivePointBuffer tmp1;

  union
  { /* PRQA S 0750 */ /* MD_MSR_Union */
    actECPprojectivePointBuffer tmp2;
    actU8 hashTT[actHASH_SIZE_SHA256];
  } tmpVariables;

  actECPaffinePointBuffer basePoint;

  /* A requires w0 und w1, B requires w0 */
  actBNDIGIT w0[actECC_MAX_N_DIGIT_LEN];
  actBNDIGIT w1[actECC_MAX_N_DIGIT_LEN];

  actBNDIGIT randomNumber[actECC_MAX_N_DIGIT_LEN];

  actLengthType aadLength;
  actU32 ciphersuite;

  /* addInHKDF: required to store the concatenation of "ConfirmationKeys" and aad as input parameter for HKDF */
  actU8 addInHKDF[16u + actSPAKE2P_MAX_AAD_SIZE];
  actU8 pointToByteString[actECC_MAX_BYTE_LEN];
  actU8 macKeys[2u * actAES128_KEY_SIZE];

  actU8 algoState;
  actU8 setAdditionalInformationWasCalled;
  actU8 isPartyA;
} actSPAKE2PSTRUCT;

/* SPAKE2+ preamble struct */
typedef struct
{
  /* Basic Workspace for ECC */
  actECPBasicStruct baseData;

  /* SPAKE2+ preamble specific data */
  actECPprojectivePointBuffer L;
  actECPaffinePointBuffer basePoint;

  /* additional digits required for actBNReduce */
  actBNDIGIT digitsW0[actSPAKE2P_PREAMBLE_WDIGIT_LENGTH + 1u];
  actBNDIGIT digitsW1[actSPAKE2P_PREAMBLE_WDIGIT_LENGTH + 1u];
  actBNDIGIT result[actECC_MAX_N_DIGIT_LEN + 1u];
} actSPAKE2PPreambleStruct;

/* ECBD STRUCTS */
typedef struct
{
  actU32       nECUs;     /*  number of ECUs                      */
  actU32       ecuID;     /*  own ECU ID                          */
} actECBD;

typedef struct
{
  actECBD      bd;        /*  Burmester/Desmedt  data             */
  /*    scratch pad is assumed here!!       */
  actECPBasicStruct baseData;

  actBNDIGIT ai[actECC_MAX_N_DIGIT_LEN];
  actBNDIGIT Zi_x[actECC_MAX_P_DIGIT_LEN];
  actBNDIGIT Zi_y[actECC_MAX_P_DIGIT_LEN];
  actBNDIGIT Zi_z[actECC_MAX_P_DIGIT_LEN];
  actBNDIGIT Xi_x[actECC_MAX_P_DIGIT_LEN];
  actBNDIGIT Xi_y[actECC_MAX_P_DIGIT_LEN];
  actBNDIGIT Xi_z[actECC_MAX_P_DIGIT_LEN];
  actBNDIGIT TP_x[actECC_MAX_P_DIGIT_LEN];
  actBNDIGIT TP_y[actECC_MAX_P_DIGIT_LEN];
  actBNDIGIT TP_z[actECC_MAX_P_DIGIT_LEN];
} actECBDstruct;

/* Ed25519 STRUCTS */
typedef struct
{
  actBNDIGIT tempA[BNDIGITS_256]; /* used in actEd25519* functions */
  actBNDIGIT tempB[BNDIGITS_256];
  actBNDIGIT tempC[BNDIGITS_256];
  actBNDIGIT tempD[BNDIGITS_256];
  actBNDIGIT tempE[BNDIGITS_256]; /* used in actCurve25519* functions */
  actBNDIGIT tempF[BNDIGITS_256];
  actBNDIGIT tempG[BNDIGITS_256];
  actBNDIGIT tempH[BNDIGITS_256];
  actBNDIGIT temp1[BNDIGITS_512]; /* 512 bit temp variable (used for multiplication) */
  actBNDIGIT tempQ[BNDIGITS_256]; /* 256 bit temp variable (used for reduction) */
} actTempType;

/* Curve structure */
typedef struct
{
  actBNDIGIT prime[BNDIGITS_256];
  actBNDIGIT order[BNDIGITS_256];
  actBNDIGIT orderB[BNDIGITS_256 + 1u];  /* 2^512 / q (order) */

  actBNRING pRing;
  actBNRING qRing;
  actTempType auxVar;
} actCurve25519ws;            /* size <= 520 bytes (64bit pointers) */

/* Workspace structure for Ed25519 */
typedef struct
{
  actU32 instance;                        /* algorithm instance  (plain, context, pre-hash) */
  union
  {                                 /* PRQA S 0750 */ /* MD_MSR_Union */
    actSHA512STRUCT sha512;               /* SHA-512         - used for pre-hash and Ed25519 */
    actPoint point_A;                     /* A:  need 128 bytes - actSHA512Struct is 212 bytes */
    actPoint point_rB;                    /* rB: need 128 bytes - actSHA512Struct is 212 bytes */
    actPoint point_sB;                    /* sB: sB / A are NEVER used concurrently! */
  } shaWs;
  union
  {                                 /* PRQA S 0750 */ /* MD_MSR_Union */
    actU8 hashValue[actHASH_SIZE_SHA512]; /* hashed message  - used for pre-hash */
    actBNDIGIT bnDigit_s;                 /* s: need  32 bytes - hashValue is 64 bytes */
    actBNDIGIT bnDigit_k;                 /* k: k / s  are NEVER used concurrently! */
  } hashBuf;
  actU8 privateKey[actHASH_SIZE_SHA512];  /* private scalar & prefix */
  union
  {                                 /* PRQA S 0750 */ /* MD_MSR_Union */
    actU8 publicKey[actEd25519_KEY_SIZE]; /* public key  */
    actBNDIGIT bnDigit_a;                 /* a: need  32 bytes - publicKey is 32 bytes */
  } pubKeyBuf;
  VSECPRIM_P2CONST_PARA(actU8) message;   /* input to signature (probably hashValue) */
  actU32 msgLen;                          /* length of message */
  VSECPRIM_P2CONST_PARA(actU8) context;   /* context */
  actU32 ctxLen;                          /* length of context */
  actCurve25519ws coreWS;                 /* workspace for calculation core (layout depends on core used) */
} actEd25519STRUCT;

/* Workspace structure for X25519 */
typedef struct
{
  actU8 privateKey[actX25519_KEY_SIZE];   /* private key */
  union
  {                                 /* PRQA S 0750 */ /* MD_MSR_Union */
    actU8 uCoord[actX25519_KEY_SIZE];     /* u (x) coordinate */
    actBNDIGIT bnDigit_x_1[BNDIGITS_256]; /* x_1: x_1 / u  are NEVER used concurrently! */
  } basePointBuf;
  actCurve25519ws coreWS;                 /* workspace for calculation core */
} actX25519STRUCT;

/* Compatibility */
typedef actX25519STRUCT actX25519Struct;
typedef actEd25519STRUCT actEd25519Struct;

/* Workspace structure for SipHash */
typedef struct
{
# if defined ACT_PLATFORM_UINT64_AVAILABLE
  actU64 v0;
  actU64 v1;
  actU64 v2;
  actU64 v3;
# endif
  actU8 buffer[actSIPHASH_BLOCK_SIZE];
  actU32 length;
  actU8 buffer_used;
} actSipHashSTRUCT;

/* Workspace structure for GHash */
typedef struct
{
  actLengthType buffer_used;          /* number of bytes saved in buffer */
  actU8 buffer[actGHASH_BLOCK_SIZE];  /* the input buffer */
  actU32 H[actGHASH_WORDS_PER_BLOCK]; /* the hash subkey */
  actU8 Y[actGHASH_BLOCK_SIZE];       /* the hash state buffer */
# if (actGHASH_SPEED_UP >= 4)                                           /* COV_VSECPRIM_GHASH_SPEED_UP XF */
  actU32 M[256][4];                   /* the precomputation table */
# elif (actGHASH_SPEED_UP >= 1)                                         /* COV_VSECPRIM_GHASH_SPEED_UP TX */
  actU32 M[16][4];                    /* the precomputation table */
# endif
} actGHASHSTRUCT;

/* Workspace structure for GCM */
typedef struct
{
  actAESSTRUCT aes;                  /* the aes context structure */
  actGHASHSTRUCT ghash;              /* the ghash context structure */
  actU8 J0[actGCM_BLOCK_SIZE];       /* the first counter block depending on the iv for the tag computation */
  actU8 J[actGCM_BLOCK_SIZE];        /* the counter block that is iterated */
  actU8 encJ[actGCM_BLOCK_SIZE];     /* the encrypted counter block */
  actLengthType buffer_used;         /* number of bytes saved in buffer */
  actU8 buffer[actGCM_BLOCK_SIZE];   /* the input buffer */
  uint8 state;                       /* the current internal state (authdata or ciphertext) */
  actU32 count[4];                   /* BE 64 bit AAD count and data count */
} actGCMSTRUCT;

/* Workspace for AES128 CCM */
typedef struct
{
  actAESSTRUCT wsAES;
  actLengthType aadLen;
  actLengthType msgLen;
  actLengthType remainingLen;
  actU8 AESCounter[actAES_BLOCK_SIZE];
  actU8 dataBlockB[actAES_BLOCK_SIZE];
  actU8 dataBlockIn[actAES_BLOCK_SIZE];
  actU8 dataBlockOut[actAES_BLOCK_SIZE];
  actU8 MAC[actAES_BLOCK_SIZE];
  actU8 nonce[actAES_BLOCK_SIZE - 1u];
  actU8 authFieldSize;
  actU8 lengthFieldSize;
  actU8 updateState;
} actAESCCMSTRUCT;

/* Workspace structure for RSA exponentiation primitives */
typedef struct
{
  actBNDIGIT cipher[actRSA_MAX_KEY_SIZE_IN_DIGIT];
  actBNDIGIT message[actRSA_MAX_KEY_SIZE_IN_DIGIT + 1u];
} actRSACipherAndMSgStruct;

typedef struct
{
  actBNRING  wsRSARing;

  union
  {  /* PRQA S 0750 */ /* MD_MSR_Union */
    actBNDIGIT tmpInit[(2u*actRSA_MAX_KEY_SIZE_IN_DIGIT) + 2u]; /* only used in actRSAInitExponentiation(); additional digit is required for actBNReduce() */
    actRSACipherAndMSgStruct cipherAndMsg; /* only used in actRSAExponentiation() */
  } tmp;

  actBNDIGIT RRBuffer[actRSA_MAX_KEY_SIZE_IN_DIGIT + 1u];
  actBNDIGIT exponent[actRSA_MAX_KEY_SIZE_IN_DIGIT];
  actBNDIGIT modulusBuffer[actRSA_MAX_KEY_SIZE_IN_DIGIT];
  actBNDIGIT tmpExp[actRSA_MAX_KEY_SIZE_IN_DIGIT + 1u];

}actRSAPRIMSTRUCT;

/* struct for CRT key parameters */

/* Note:
 * The CRT routine consists of the following steps:
 * - actRSAInitPrivateKeyOperationCRT
 * - actRSAPrivateKeyOperationCRT:
 *   - CRT STEP 1: calculate m2 = cq^dq mod q
 *   - CRT STEP 2: calculate m1 = cp^dp mod p
 *   - CRT STEP 3: calculate h = (m1-m2)*qInv mod p
 *   - CRT STEP 4: calculate m = m2 + q*h
 */

/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------|
 * Additional information on the length of the buffers in the CRT workspace                                                                                          |
 * ------------------------------------------------------------------------------------------------------------------------------------------------------------------|
 * ------------------------------------------------------------------------------------------------------------------------------------------------------------------|
 * Buffer                            |      When used?                                       |    Length requirement                                                 |
 * ----------------------------------|-------------------------------------------------------|-----------------------------------------------------------------------|
 * fullModulus                       |     actRSAInitPrivateKeyOperationCRT()                |    must be at least p_length + q_length long                          |
 *                                   |     stores result N = p*q                             |    -> minimal length = 2 * prime size (in digits)                     |
 *                                   |     -> used as output buffer for actBNMult()          |                                                                       |
 * ----------------------------------|-------------------------------------------------------|-----------------------------------------------------------------------|
 * origCipherBeforeReduction         |     actRSAPrivateKeyOperationCRT(),                   |    must be cipher length + 1 digits long                              |
 *                                   |     used as input buffer for actBNReduce()            |                                                                       |
 *                                   |     to compute the reduced cipher                     |                                                                       |
 *                                   |                                                       |                                                                       |
 * reducedCipherInDigits             |     actRSAPrivateKeyOperationCRT(),                   |    must be prime size + 1 digits long                                 |
 *                                   |     used as output buffer for actBNReduce()           |    (result is prime size digits long,                                 |
 *                                   |     to store the reduced cipher (mod p / q)           |    additional digit required for actBNReduce())                       |
 * ----------------------------------|-------------------------------------------------------|-----------------------------------------------------------------------|
 * The following three buffers are part of actCRTSTRUCT.wsUnion1.actRSAPRIMSTRUCT.wsRSAPrim:                                                                         |
 *                                                                                                                                                                   |
 * rsaInitRR                         |     actRSAPrivateKeyOperationCRT(), call to           |    must be (n_length + 1) digits long, where n_length                 |
 *                                   |     actRSAInitExponentiation()                        |    is the length of the modulus passed to actRSAInitExponentiation()  |
 *                                   |     -> used as output buffer for actBNReduce()        |    (i.e., one of the primes p and q)                                  |
 *                                   |                                                       |    -> minimal length = prime size + 1 (in digits)                     |
 * rsaExpMsg                         |     actRSAPrivateKeyOperationCRT(), during call to    |    must be (n_length + 1) digits long, where n_length                 |
 *                                   |     actRSAExponentiation(),                           |    is the length of the modulus passed to actRSAInitExponentiation()  |
 *                                   |     -> used as output buffer for actBNModExp          |    (i.e., one of the primes p and q)                                  |
 *                                   |                                                       |    -> minimal length = prime size + 1 (in digits)                     |
 *                                   |                                                       |                                                                       |
 * rsaTmpVar                         |     actRSAPrivateKeyOperationCRT(), call to           |    must be (2*n_length + 2) digits long, where n_length               |
 *                                   |     actRSAInitExponentiation()                        |    is the length of the modulus passed to actRSAInitExponentiation()  |
 *                                   |                                                       |    (i.e., one of the primes p and q)                                  |
 *                                   |                                                       |    -> minimal length = 2 * prime size + 2 (in digits)                 |
 * ----------------------------------|-------------------------------------------------------|-----------------------------------------------------------------------|
 * m2                                |     actRSAPrivateKeyOperationCRT(),                   |    Note: m2 already fulfills m2 < q                                   |
 *                                   |     used as input buffer for actBNReduce()            |    must be prime size + 1 digits long                                 |
 *                                   |     to compute the reduced message m2 mod p           |                                                                       |
 *                                   |                                                       |                                                                       |
 * m2ModP                            |     actRSAPrivateKeyOperationCRT(),                   |    must be prime size + 1 digits long                                 |
 *                                   |     used as output buffer for actBNReduce()           |    (result is prime size digits long,                                 |
 *                                   |     to store the reduced message (mod p / q)          |    additional digit required for actBNReduce())                       |
 * ----------------------------------|-------------------------------------------------------|-----------------------------------------------------------------------|
 * qInvM1M2R                         |     actRSAPrivateKeyOperationCRT(),                   |    must be prime size + 1 digits long                                 |
 *                                   |     used as output buffer for actBNMontMul()          |    (result is prime size digits long,                                 |
 *                                   |     to store (qInv * (m1 - m2) mod p)*R^(-1) mod p    |    additional digit required for actBNMontMul())                      |
 *                                   |                                                       |                                                                       |
 * qInvM1M2                          |     actRSAPrivateKeyOperationCRT(),                   |    must be prime size + 1 digits long                                 |
 *                                   |     used as output buffer for actBNMontMul()          |    (result is prime size digits long,                                 |
 *                                   |     to store (qInv * (m1 - m2) mod p)                 |    additional digit required for actBNMontMul())                      |
 *                                   |                                                       |                                                                       |
 * qInvM1M2q                         |     actRSAInitPrivateKeyOperationCRT()                |    must be at least p_length + q_length long                          |
 *                                   |     stores result (qInv * (m1 - m2) mod p)*q          |    -> minimal length = 2 * prime size (in digits)                     |
 *                                   |     -> used as output buffer for actBNMult()          |                                                                       |
 * ----------------------------------|-------------------------------------------------------|-----------------------------------------------------------------------|
 */

                                                                                    /* | used where?                                     | value stored                       | additional length  | */
typedef struct                                                                      /* |                                                 |                                    | information given? | */
{                                                                                   /* ------------------------------------------------------------------------------------------------------------- */
    actBNDIGIT primeP[actRSA_CRT_PRIME_SIZE];                                       /* | actRSAInitPrivateKeyOperationCRT                |  p, p-1                            |  no                | */
    actBNDIGIT primeQ[actRSA_CRT_PRIME_SIZE];                                       /* | actRSAInitPrivateKeyOperationCRT                |  q, q-1                            |  no                | */
    actBNDIGIT primeDP[actRSA_CRT_PRIME_SIZE];                                      /* | actRSAInitPrivateKeyOperationCRT                |  dq                                |  no                | */
    actBNDIGIT primeDQ[actRSA_CRT_PRIME_SIZE];                                      /* | actRSAInitPrivateKeyOperationCRT                |  dq                                |  no                | */
    actBNDIGIT qInv[actRSA_CRT_PRIME_SIZE];                                         /* | actRSAInitPrivateKeyOperationCRT                |  qInv                              |  no                | */
    actBNDIGIT one[actRSA_CRT_PRIME_SIZE];                                          /* | actRSAInitPrivateKeyOperationCRT                |  big num of value '1'              |  no                | */
} actCRTINITSTRUCT;                                                                 /* ------------------------------------------------------------------------------------------------------------- */

typedef struct
{                                                                                   /* ------------------------------------------------------------------------------------------------------------- */
  actBNDIGIT origCipherBeforeReduction[actRSA_MAX_KEY_SIZE_IN_DIGIT + 1u];          /* | CRT STEP 1, CRT STEP 2                          |  c                                 |  yes               | */
  actU8 reducedCipherInBytes[actRSA_CRT_PRIME_SIZE * actBN_BYTES_PER_DIGIT];        /* | CRT STEP 1, CRT STEP 2                          |  c mod q, c mod p                  |  no                | */
  actBNDIGIT reducedCipherInDigits[actRSA_CRT_PRIME_SIZE + 1u];                     /* | CRT STEP 1, CRT STEP 2                          |  c mod q, c mod p                  |  yes               | */
} actCRTCIPHERSTRUCT;                                                               /* ------------------------------------------------------------------------------------------------------------- */

typedef struct
{                                                                                   /* ------------------------------------------------------------------------------------------------------------- */
  actBNDIGIT m1[actRSA_CRT_PRIME_SIZE];                                             /* | CRT STEP 2 (end), CRT STEP 3 (beginning)        |  m1                                |  no                | */
  actBNDIGIT m2[actRSA_CRT_PRIME_SIZE + 1u];                                        /* | CRT STEP 1, CRT STEP 3                          |  m2                                |  yes               | */
} actCRTMSGSTRUCT;                                                                  /* ------------------------------------------------------------------------------------------------------------- */

typedef struct
{                                                                                   /* ------------------------------------------------------------------------------------------------------------- */
  actBNDIGIT m2Copy[actRSA_CRT_PRIME_SIZE + 1u];                                    /* | CRT STEP 3                                      |  m2                                |  no                | */
  actBNDIGIT m2ModP[actRSA_CRT_PRIME_SIZE + 1u];                                    /* | CRT STEP 3                                      |  m2 mod p                          |  yes               | */
  actBNDIGIT m1m2[actRSA_CRT_PRIME_SIZE];                                           /* | CRT STEP 3                                      |  m1 - m2 mod p                     |  no                | */
  actBNDIGIT qInv[actRSA_CRT_PRIME_SIZE];                                           /* | CRT STEP 3                                      |  qInv                              |  no                | */
  actBNDIGIT qInvM1M2R[actRSA_CRT_PRIME_SIZE + 1u];                                 /* | CRT STEP 3                                      |  qInv*((m1-m2) mod p)*R^(-1) mod p |  yes               | */
  actBNDIGIT qInvM1M2[actRSA_CRT_PRIME_SIZE + 1u];                                  /* | CRT STEP 3                                      |  (qInv*(m1 - m2) mod p)            |  yes               | */
} actCRTSTEP3STRUCT;                                                                /* ------------------------------------------------------------------------------------------------------------- */

typedef struct
{                                                                                   /* ------------------------------------------------------------------------------------------------------------- */
  actBNDIGIT m2Padded[actRSA_MAX_KEY_SIZE_IN_DIGIT];                                /* | CRT STEP 4                                      |  [m2 | 0,..., 0]                   |  no                | */
  actBNDIGIT primeQ[actRSA_CRT_PRIME_SIZE];                                         /* | CRT STEP 4                                      |  q                                 |  no                | */
  actBNDIGIT qInvM1M2q[2u* actRSA_CRT_PRIME_SIZE];                                  /* | CRT STEP 4                                      |  qInv*((m1-m2) mod p)*q            |  yes               | */
  actBNDIGIT msg[actRSA_MAX_KEY_SIZE_IN_DIGIT];                                     /* | CRT STEP 4                                      |  signature / decrypted msg         |  no                | */
} actCRTSTEP4STRUCT;                                                                /* ------------------------------------------------------------------------------------------------------------- */

typedef struct
{
  union
  { /* PRQA S 0750 */ /* MD_MSR_Union */                                            /* ------------------------------------------------------------------------------------------------------------- */
    actRSAPRIMSTRUCT wsRSAPrim;                                                     /* | CRT STEP 1, 2, 3                                |                                    |                    | */
    actCRTINITSTRUCT wsCRTInit;                                                     /* | actRSAInitPrivateKeyOperationCRT                |                                    |                    | */
    actCRTSTEP4STRUCT wsCRTSTEP4;                                                   /* | CRT STEP 4                                      |                                    |                    | */
  } wsUnion1;                                                                       /* ------------------------------------------------------------------------------------------------------------- */

  union
  { /* PRQA S 0750 */ /* MD_MSR_Union */                                            /* ------------------------------------------------------------------------------------------------------------- */
    actCRTCIPHERSTRUCT wsCRTCipherOperations;                                       /* | CRT STEP 1, 2                                   |                                    |                    | */
    actCRTSTEP3STRUCT wsCRTStep3;                                                   /* | CRT STEP 3                                      |                                    |                    | */
  } wsUnion2;                                                                       /* ------------------------------------------------------------------------------------------------------------- */

  union
  { /* PRQA S 0750 */ /* MD_MSR_Union */                                            /* ------------------------------------------------------------------------------------------------------------- */
    actCRTMSGSTRUCT messages;                                                       /* | CRT STEP 1, 2, 3                                |                                    |                    | */
    actBNDIGIT fullModulus[2u * actRSA_CRT_PRIME_SIZE];                             /* | before CRT STEP 1                               |  modulus N = p*q                   |  yes               | */
  } keySizeBuffer;                                                                  /* ------------------------------------------------------------------------------------------------------------- */

  actLengthType p_bytes;
  actLengthType q_bytes;
  actLengthType dp_bytes;
  actLengthType dq_bytes;
  actLengthType q_inv_bytes;
  actLengthType n_bytes;

  /* pointers to RSA CRT keys */
  VSECPRIM_P2CONST_PARA(actU8) p;
  VSECPRIM_P2CONST_PARA(actU8) q;
  VSECPRIM_P2CONST_PARA(actU8) dp;
  VSECPRIM_P2CONST_PARA(actU8) dq;
  VSECPRIM_P2CONST_PARA(actU8) q_inv;

} actRSACRTSTRUCT;

#endif /* ACTITYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: actITypes.h
 *********************************************************************************************************************/

