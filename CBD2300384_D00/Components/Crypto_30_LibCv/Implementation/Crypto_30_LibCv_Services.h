/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*      \file  Crypto_30_LibCv_Services.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Internal header file for service dispatch function prototypes
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (CRYPTO_30_LIBCV_SERVICE_H)
# define CRYPTO_30_LIBCV_SERVICE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Csm_Types.h"
# include "Crypto_30_LibCv_Cfg.h"

# include "ESLib_version.h"

# if !defined (CRYPTO_30_LIBCV_LOCAL) /* COV_CRYPTO_30_LIBCV_LOCAL_DEFINE */
#  define CRYPTO_30_LIBCV_LOCAL                                       static
# endif

# if !defined (CRYPTO_30_LIBCV_LOCAL_INLINE) /* COV_CRYPTO_30_LIBCV_LOCAL_DEFINE */
#  define CRYPTO_30_LIBCV_LOCAL_INLINE                                LOCAL_INLINE
# endif

# if !defined (CRYPTO_30_LIBCV_ALWAYS_LOCAL_INLINE) /* COV_CRYPTO_30_LIBCV_LOCAL_DEFINE */
#  define CRYPTO_30_LIBCV_ALWAYS_LOCAL_INLINE                         static INLINE /* "static" cannot be removed in this definition */
# endif

# if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
#  include "Det.h"
# endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# if !defined (CRYPTO_30_LIBCV_BYTES_PER_DIGIT) /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT */
#  define CRYPTO_30_LIBCV_BYTES_PER_DIGIT                             actBN_BYTES_PER_DIGIT
# endif

/* Masks for KeyElementState */
# define CRYPTO_30_LIBCV_KEYELEMENTSTATE_CLEAR_NORMAL_MASK            CRYPTO_30_LIBCV_KEYELEMENTSTATE_CLEAR_ALL_MASK
# define CRYPTO_30_LIBCV_KEYELEMENTSTATE_CLEAR_ALL_MASK               (0x00u) /* clear all flags */
# define CRYPTO_30_LIBCV_KEYELEMENTSTATE_WRITTEN_ONCE_MASK            (0x80u)
# define CRYPTO_30_LIBCV_KEYELEMENTSTATE_WRITTEN_ONCE_INV_MASK        (0x7Fu)
# define CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_INIT_MASK               (0x08u)
# define CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_INIT_INV_MASK           (0xF7u)
# define CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALUE_USED_MASK              (0x04u)
# define CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALUE_USED_INV_MASK          (0xFBu)
# define CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK                   (0x01u)
# define CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_INV_MASK               (0xFEu)

/* Mask for KeyElementExtension */
# define CRYPTO_30_LIBCV_KEYELEMENTSEXTENSION_CLEAR_ALL_MASK          (0x00u) /* clear all flags */
# define CRYPTO_30_LIBCV_KEYELEMENTSEXTENSION_SHE_KEY_MASK            (0x01u) /* she key flags */
# define CRYPTO_30_LIBCV_KEYELEMENTSEXTENSION_SHE_PLAIN_KEY_MASK      (0x03u) /* she written plain flags */
# define CRYPTO_30_LIBCV_KEYELEMENTSEXTENSION_SHE_CLEAR_PLAIN_KEY_MASK (0xFDu) /* clear she written plain flags */

# if (CRYPTO_30_LIBCV_RSAPKCS1GENERATE == STD_ON) || (CRYPTO_30_LIBCV_RSAPSSGENERATE == STD_ON)
#  define CRYPTO_30_LIBCV_RSAGENERATE                                 STD_ON
# else
#  define CRYPTO_30_LIBCV_RSAGENERATE                                 STD_OFF
# endif

# if (CRYPTO_30_LIBCV_RSAPKCS1VERIFY == STD_ON) || (CRYPTO_30_LIBCV_RSAPSSVERIFY == STD_ON)
#  define CRYPTO_30_LIBCV_RSAVERIFY                                   STD_ON
# else
#  define CRYPTO_30_LIBCV_RSAVERIFY                                   STD_OFF
# endif

/* General Target Key Element -> always id 1 */
# define CRYPTO_30_LIBCV_KE_TARGET_KEY                                (1u)

/* Size definitions */
# define CRYPTO_30_LIBCV_CMACAES_MAX_KEY_SIZE                         (16u)
# define CRYPTO_30_LIBCV_CMACAES_MAC_SIZE                             (16u)
# define CRYPTO_30_LIBCV_AES_BLOCK_SIZE                               (16u)
# define CRYPTO_30_LIBCV_AESCCM_SUM_OF_NONCE_LENGTH_AND_L             (15u)

# define CRYPTO_30_LIBCV_KEY_LOCK_READ_MAX                            (0xFFu)
# define CRYPTO_30_LIBCV_KEY_LOCK_READ_ONE                            (3u)
# define CRYPTO_30_LIBCV_KEY_LOCK_FREE                                (2u)
# define CRYPTO_30_LIBCV_KEY_LOCK_WRITE                               (1u)

# define CRYPTO_30_LIBCV_LONG_TERM_WS_LOCK_FREE                       (CRYPTO_30_LIBCV_MAX_LONGTERMWSLOCK)

# define CRYPTO_30_LIBCV_SIZEOF_UINT32                                (4u)

# define CRYPTO_30_LIBCV_UINT32_MAX                                   (0xFFFFFFFFuL)
# define CRYPTO_30_LIBCV_UINT24_MAX                                   (0xFFFFFFuL)
# define CRYPTO_30_LIBCV_UINT16_MAX                                   (0xFFFFu)

/* Length Check */
# define CRYPTO_30_LIBCV_LENGTH_CHECK_NONE                            (0u) /* length is not checked */
# define CRYPTO_30_LIBCV_LENGTH_CHECK_MAX                             (1u) /* length is smaller / equal to key length */
# define CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL                           (2u) /* length is equal to key length */
# define CRYPTO_30_LIBCV_LENGTH_CHECK_MIN                             (3u) /* length is bigger / equal to key length  */

/* SHE key usage types */
# define CRYPTO_30_LIBCV_SHE_SERVICE_ENCRYPT_DECRYPT                  (0u)
# define CRYPTO_30_LIBCV_SHE_SERVICE_MAC_GENERATE                     (1u)
# define CRYPTO_30_LIBCV_SHE_SERVICE_MAC_VERIFY                       (2u)
# define CRYPTO_30_LIBCV_SHE_SERVICE_KEY_SERVICE                      (3u)
# define CRYPTO_30_LIBCV_SHE_SERVICE_OTHER                            (4u)
# define CRYPTO_30_LIBCV_SHE_NUM_SERVICES                             (4u)

# define CRYPTO_30_LIBCV_BOOT_PROTECTED                               (0x01u)
# define CRYPTO_30_LIBCV_DEBUGGER_PROTECTED                           (0x01u)

# define CRYPTO_30_LIBCV_SIZEOF_ECC_160_P                             (20u)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_160_N                             (21u)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_224_P                             (28u)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_224_N                             (28u)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_256_P                             (32u)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_256_N                             (32u)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_384_P                             (48u)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_384_N                             (48u)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_521_P                             (66u)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_521_N                             (66u)

# define CRYPTO_30_LIBCV_SIZEOF_ECC_160_SIGNATURE                     (CRYPTO_30_LIBCV_SIZEOF_ECC_160_N)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_224_SIGNATURE                     (CRYPTO_30_LIBCV_SIZEOF_ECC_224_N)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_256_SIGNATURE                     (CRYPTO_30_LIBCV_SIZEOF_ECC_256_N)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_384_SIGNATURE                     (CRYPTO_30_LIBCV_SIZEOF_ECC_384_N)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_521_SIGNATURE                     (CRYPTO_30_LIBCV_SIZEOF_ECC_521_N)

# define CRYPTO_30_LIBCV_SIZEOF_ECC_160_KEY_PRIVATE                   (CRYPTO_30_LIBCV_SIZEOF_ECC_160_N)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PRIVATE                   (CRYPTO_30_LIBCV_SIZEOF_ECC_224_N)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PRIVATE                   (CRYPTO_30_LIBCV_SIZEOF_ECC_256_N)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_384_KEY_PRIVATE                   (CRYPTO_30_LIBCV_SIZEOF_ECC_384_N)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_521_KEY_PRIVATE                   (CRYPTO_30_LIBCV_SIZEOF_ECC_521_N)

# define CRYPTO_30_LIBCV_SIZEOF_ECC_160_KEY_PUBLIC                    (CRYPTO_30_LIBCV_SIZEOF_ECC_160_P)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC                    (CRYPTO_30_LIBCV_SIZEOF_ECC_224_P)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PUBLIC                    (CRYPTO_30_LIBCV_SIZEOF_ECC_256_P)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_384_KEY_PUBLIC                    (CRYPTO_30_LIBCV_SIZEOF_ECC_384_P)
# define CRYPTO_30_LIBCV_SIZEOF_ECC_521_KEY_PUBLIC                    (CRYPTO_30_LIBCV_SIZEOF_ECC_521_P)

# define CRYPTO_30_LIBCV_SIZEOF_ECC_160_KEY_PUBLIC_XY                 (2u * (CRYPTO_30_LIBCV_SIZEOF_ECC_160_P))
# define CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY                 (2u * (CRYPTO_30_LIBCV_SIZEOF_ECC_224_P))
# define CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PUBLIC_XY                 (2u * (CRYPTO_30_LIBCV_SIZEOF_ECC_256_P))
# define CRYPTO_30_LIBCV_SIZEOF_ECC_384_KEY_PUBLIC_XY                 (2u * (CRYPTO_30_LIBCV_SIZEOF_ECC_384_P))
# define CRYPTO_30_LIBCV_SIZEOF_ECC_521_KEY_PUBLIC_XY                 (2u * (CRYPTO_30_LIBCV_SIZEOF_ECC_521_P))

# if(CRYPTO_30_LIBCV_ECP521GENERATE == STD_ON)
#  define CRYPTO_30_LIBCV_MAX_SIZEOF_SIGNATURE_GENERATE_ECC_KEY       (CRYPTO_30_LIBCV_SIZEOF_ECC_521_SIGNATURE)
# elif(CRYPTO_30_LIBCV_ECP384GENERATE == STD_ON)
#  define CRYPTO_30_LIBCV_MAX_SIZEOF_SIGNATURE_GENERATE_ECC_KEY       (CRYPTO_30_LIBCV_SIZEOF_ECC_384_SIGNATURE)
# else
#  if(CRYPTO_30_LIBCV_ECP256GENERATE == STD_ON)
#   define CRYPTO_30_LIBCV_MAX_SIZEOF_SIGNATURE_GENERATE_ECC_KEY      (CRYPTO_30_LIBCV_SIZEOF_ECC_256_SIGNATURE)
#  else
/* The signature contains an R and an S component, the length of the R component is always 160 bits and the length of the S component can be 161 bits,
   therefore the maximum size is fixed at 161 bits. */
#   define CRYPTO_30_LIBCV_MAX_SIZEOF_SIGNATURE_GENERATE_ECC_KEY      (CRYPTO_30_LIBCV_SIZEOF_ECC_160_SIGNATURE)
#  endif
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

# define Crypto_30_LibCv_NvM_Persist(idx)                             Crypto_30_LibCv_NvBlock_Write_Req(Crypto_30_LibCv_GetNvBlockIdxOfKey(idx))

# define Crypto_30_LibCv_IsLongTermWsLock(id, value)                  (Crypto_30_LibCv_GetLongTermWsLock((id)) == (value))

# define Crypto_30_LibCv_Uint8CheckMask(value, mask)                  (((value) & (mask)) == (mask))
# define Crypto_30_LibCv_Uint8SetMask(value, mask)                    ((value) = (uint8)((value) | (mask)))
# define Crypto_30_LibCv_Uint8ClearMask(value, mask)                  ((value) = (uint8)((value) & (mask)))

/* Macros for all elements */
# define Crypto_30_LibCv_IsKeyElementPartial(Index)                   (Crypto_30_LibCv_IsPartialOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements((Index))))
# define Crypto_30_LibCv_IsKeyElementPersist(Index)                   (Crypto_30_LibCv_IsPersistOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements((Index))))
# define Crypto_30_LibCv_IsKeyElementWriteOnce(Index)                 (Crypto_30_LibCv_IsOnceOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements((Index))))
# define Crypto_30_LibCv_GetKeyElementLength(Index)                   (Crypto_30_LibCv_GetLengthOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements((Index))))
# define Crypto_30_LibCv_HasKeyElementInitValue(Index)                (Crypto_30_LibCv_IsInitValueUsedOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements((Index))))
# define Crypto_30_LibCv_GetKeyElementInitValueLength(Index)          ((uint16)Crypto_30_LibCv_GetInitValueEndIdxOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements((Index))) - (uint16)Crypto_30_LibCv_GetInitValueStartIdxOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(Index)))
# define Crypto_30_LibCv_IsKeyElementValid(Index)                     ((Crypto_30_LibCv_GetKeyStorage(Crypto_30_LibCv_GetKeyStorageValidIdxOfKeyElements((Index))) & CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK) ==  CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK)
# define Crypto_30_LibCv_SetKeyElementState(Index, Value)             (Crypto_30_LibCv_SetKeyStorage(Crypto_30_LibCv_GetKeyStorageValidIdxOfKeyElements((Index)), (Value)))
# define Crypto_30_LibCv_GetKeyElementState(Index)                    (Crypto_30_LibCv_GetKeyStorage(Crypto_30_LibCv_GetKeyStorageValidIdxOfKeyElements((Index))))
# define Crypto_30_LibCv_ClearKeyElementStateByMask(Index, Mask)      (Crypto_30_LibCv_SetKeyElementState((Index), (uint8)(Crypto_30_LibCv_GetKeyElementState((Index)) & (Mask))))
# define Crypto_30_LibCv_SetKeyElementStateByMask(Index, Mask)        (Crypto_30_LibCv_SetKeyElementState((Index), (uint8)(Crypto_30_LibCv_GetKeyElementState((Index)) | (Mask))))
# define Crypto_30_LibCv_IsKeyElementStateByMask(Index, Mask)         ((Crypto_30_LibCv_GetKeyElementState((Index)) & (uint8)(Mask)) == (Mask))

# define Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(Index)        (Crypto_30_LibCv_GetAddrKeyStorage(Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(Index)))

# define Crypto_30_LibCv_SetKeyElementExtension(Index, Value)         (Crypto_30_LibCv_SetKeyStorage(Crypto_30_LibCv_GetKeyStorageExtensionIdxOfKeyElements((Index)), (Value)))
# define Crypto_30_LibCv_GetKeyElementExtension(Index)                (Crypto_30_LibCv_GetKeyStorage(Crypto_30_LibCv_GetKeyStorageExtensionIdxOfKeyElements((Index))))
# define Crypto_30_LibCv_ClearKeyElementExtensionByMask(Index, Mask)  (Crypto_30_LibCv_SetKeyElementExtension((Index), (uint8)(Crypto_30_LibCv_GetKeyElementExtension((Index)) & (Mask))))
# define Crypto_30_LibCv_SetKeyElementExtensionByMask(Index, Mask)    (Crypto_30_LibCv_SetKeyElementExtension((Index), (uint8)(Crypto_30_LibCv_GetKeyElementExtension((Index)) | (Mask))))
# define Crypto_30_LibCv_IsKeyElementExtensionByMask(Index, Mask)     ((Crypto_30_LibCv_GetKeyElementExtension((Index)) & (uint8)(Mask)) == (Mask))

/* SheKeys */
# define Crypto_30_LibCv_IsSheKey(Index)                              (Crypto_30_LibCv_IsKeyElementExtensionByMask((Index), CRYPTO_30_LIBCV_KEYELEMENTSEXTENSION_SHE_KEY_MASK)? TRUE : FALSE)

/* Job mode macro */
# define Crypto_30_LibCv_IsJobMode(Job, Mask)                         ((((Job)->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.mode) & (Mask)) == (Mask))

/* EcP Curves */
# if ((CRYPTO_30_LIBCV_ECP256GENERATE == STD_ON) || (CRYPTO_30_LIBCV_ECP384GENERATE == STD_ON) || (CRYPTO_30_LIBCV_ECP160GENERATE == STD_ON) || (CRYPTO_30_LIBCV_ECP521GENERATE == STD_ON))
#  define CRYPTO_30_LIBCV_ECPGENERATE                                 STD_ON
# else
#  define CRYPTO_30_LIBCV_ECPGENERATE                                 STD_OFF
# endif

# if ((CRYPTO_30_LIBCV_ECP256VERIFY == STD_ON) || (CRYPTO_30_LIBCV_ECP384VERIFY == STD_ON) || (CRYPTO_30_LIBCV_ECP160VERIFY == STD_ON) || (CRYPTO_30_LIBCV_ECP521VERIFY == STD_ON))
#  define CRYPTO_30_LIBCV_ECPVERIFY                                   STD_ON
# else
#  define CRYPTO_30_LIBCV_ECPVERIFY                                   STD_OFF
# endif

# if ((CRYPTO_30_LIBCV_SIGNATURE_GENERATE_SECFAM_SHA1 == STD_ON)||(CRYPTO_30_LIBCV_SIGNATURE_VERIFY_SECFAM_SHA1 == STD_ON))
#  define CRYPTO_30_LIBCV_SIGNATURE_SECFAM_SHA1                       STD_ON
# else
#  define CRYPTO_30_LIBCV_SIGNATURE_SECFAM_SHA1                       STD_OFF
# endif

# if ((CRYPTO_30_LIBCV_SIGNATURE_GENERATE_SECFAM_SHA2_256 == STD_ON)||(CRYPTO_30_LIBCV_SIGNATURE_VERIFY_SECFAM_SHA2_256 == STD_ON))
#  define CRYPTO_30_LIBCV_SIGNATURE_SECFAM_SHA2_256                   STD_ON
# else
#  define CRYPTO_30_LIBCV_SIGNATURE_SECFAM_SHA2_256                   STD_OFF
# endif

# if ((CRYPTO_30_LIBCV_SIGNATURE_GENERATE_SECFAM_SHA2_384 == STD_ON)||(CRYPTO_30_LIBCV_SIGNATURE_VERIFY_SECFAM_SHA2_384 == STD_ON))
#  define CRYPTO_30_LIBCV_SIGNATURE_SECFAM_SHA2_384                   STD_ON
# else
#  define CRYPTO_30_LIBCV_SIGNATURE_SECFAM_SHA2_384                   STD_OFF
# endif

# if ((CRYPTO_30_LIBCV_SIGNATURE_GENERATE_SECFAM_SHA2_512 == STD_ON)||(CRYPTO_30_LIBCV_SIGNATURE_VERIFY_SECFAM_SHA2_512 == STD_ON))
#  define CRYPTO_30_LIBCV_SIGNATURE_SECFAM_SHA2_512                   STD_ON
# else
#  define CRYPTO_30_LIBCV_SIGNATURE_SECFAM_SHA2_512                   STD_OFF
# endif

# if ((CRYPTO_30_LIBCV_SIGNATURE_GENERATE_SECFAM_SHA1 == STD_ON) || (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_SECFAM_SHA2_256 == STD_ON) ||  (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_SECFAM_SHA2_384 == STD_ON) || (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_SECFAM_SHA2_512 == STD_ON))
#  define CRYPTO_30_LIBCV_SIGNATURE_GENERATE_PRE_HASH                 STD_ON
# else
#  define CRYPTO_30_LIBCV_SIGNATURE_GENERATE_PRE_HASH                 STD_OFF
# endif

# if ((CRYPTO_30_LIBCV_SIGNATURE_VERIFY_SECFAM_SHA1 == STD_ON) || (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_SECFAM_SHA2_256 == STD_ON) ||  (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_SECFAM_SHA2_384 == STD_ON) || (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_SECFAM_SHA2_512 == STD_ON))
#  define CRYPTO_30_LIBCV_SIGNATURE_VERIFY_PRE_HASH                   STD_ON
# else
#  define CRYPTO_30_LIBCV_SIGNATURE_VERIFY_PRE_HASH                   STD_OFF
# endif

# if ((CRYPTO_30_LIBCV_SIGNATURE_SECFAM_SHA1 == STD_ON) || (CRYPTO_30_LIBCV_SIGNATURE_SECFAM_SHA2_256 == STD_ON) ||  (CRYPTO_30_LIBCV_SIGNATURE_SECFAM_SHA2_384 == STD_ON) || (CRYPTO_30_LIBCV_SIGNATURE_SECFAM_SHA2_512 == STD_ON))
#  define CRYPTO_30_LIBCV_SIGNATURE_PRE_HASH                          STD_ON
# else
#  define CRYPTO_30_LIBCV_SIGNATURE_PRE_HASH                          STD_OFF
# endif

# if ((CRYPTO_30_LIBCV_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON)\
     || (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)\
     || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_ANSIP256R1_ENABLED == STD_ON)\
     || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP256R1_ENABLED == STD_ON))
#  define CRYPTO_30_LIBCV_KEY_EXCHANGE_P256R1_DOMAIN                  STD_ON
# else
#  define CRYPTO_30_LIBCV_KEY_EXCHANGE_P256R1_DOMAIN                  STD_OFF
# endif

# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP521R1_ENABLED == STD_ON)
/* max size of ECC key -> corresponding to curve -> p521r1 = 66Byte */
#  define CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE                             (CRYPTO_30_LIBCV_SIZEOF_ECC_521_KEY_PRIVATE)
# elif ((CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP384R1_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_NIST_SEC_P384R1_ENABLED == STD_ON))
/* max size of ECC key -> corresponding to curve -> p384r1 = 48Byte */
#  define CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE                             (CRYPTO_30_LIBCV_SIZEOF_ECC_384_KEY_PRIVATE)
# else
#  define CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE                             (CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PRIVATE)
# endif

/* Mathematic Operation Macros */
# define Crypto_30_LibCv_Math_Mul2(Value)                             ((Value) << 1u)
# define Crypto_30_LibCv_Math_Div2(Value)                             ((Value) >> 1u)
# define Crypto_30_LibCv_Math_IsEven(Value)                           (((Value) & (1u)) == 0u)
# define Crypto_30_LibCv_Math_CalcMiddle(Value1, Value2)              (((Value1) + (Value2)) / (uint8)2u)

# define Crypto_30_LibCv_Byte2Bit(Byte)                               ((Byte) << 3u)
# define Crypto_30_LibCv_Bit2Byte(Bit)                                ((Bit) >> 3u)

# define Crypto_30_LibCv_IsModuleInitialized()                        ((Crypto_30_LibCv_ModuleInitialized & CRYPTO_30_LIBCV_INITIALIZED) == CRYPTO_30_LIBCV_INITIALIZED)
# define Crypto_30_LibCv_IsModuleNotInitialized()                     ((Crypto_30_LibCv_ModuleInitialized & CRYPTO_30_LIBCV_INITIALIZED) != CRYPTO_30_LIBCV_INITIALIZED)
# define Crypto_30_LibCv_IsModuleBootProtected(value)                 (((value) & CRYPTO_30_LIBCV_BOOT_PROTECTED) == CRYPTO_30_LIBCV_BOOT_PROTECTED)
# define Crypto_30_LibCv_IsModuleDebuggerProtected(value)             (((value) & CRYPTO_30_LIBCV_DEBUGGER_PROTECTED) == CRYPTO_30_LIBCV_DEBUGGER_PROTECTED)

# define Crypto_30_LibCv_IsUint32Overflow(Value1, Value2)             (((Value2) > (uint32)(CRYPTO_30_LIBCV_UINT32_MAX - Value1)) ? TRUE:FALSE )

/* Redirection */
# define CRYPTO_30_LIBCV_NUM_OF_REDIRECTION_KEYS                      (5u)

/* Key List */
/* Keys need to be locked
 *
 * CryptoKey        1x
 * TargetKey        1x
 * RandomKey        1x
 * RedirectionKeys  5x
 */
# if (CRYPTO_30_LIBCV_REDIRECTION == STD_ON)
/* Relevant Keys: CryptoKey, TargetKey, RandomKey, RedirectionKeys */
/* Worst case number of Keys: 7x (Job target Key is only relevant if not all redirection keys are used)*/
#  define CRYPTO_30_LIBCV_MAX_NUM_OF_USED_KEYS                        (CRYPTO_30_LIBCV_NUM_OF_REDIRECTION_KEYS + 2u)
# else
/* Relevant Keys: CryptoKey, TargetKey, RandomKey */
/* Worst case number of Keys: 3x */
#  define CRYPTO_30_LIBCV_MAX_NUM_OF_USED_KEYS                        (3u)
# endif

# define CRYPTO_30_LIBCV_KEY_ACCESS_WRITE                             (1u)
# define CRYPTO_30_LIBCV_KEY_ACCESS_READ                              (0u)

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

typedef Crypto_30_LibCv_SizeOfKeyStorageType Crypto_30_LibCv_KeyStorageIndexType;

/* Read multiple key elements type definition */
typedef struct
{
  uint32 keyElementId;
  uint32 keyElementLength;
  Crypto_30_LibCv_SizeOfKeyStorageType keyElementIndex;
} Crypto_30_LibCv_KeyElementGetType;

typedef uint8 Crypto_30_LibCv_KeyElementGetSizeType;

/* Length Check */
typedef uint8 Crypto_30_LibCv_LengthCheckType;

/* Service Check */
typedef uint8 Crypto_30_LibCv_ServiceType;

/* Redirection */
typedef struct
{
  uint32 firstOutputLength;
  uint32 firstOutputLengthWritten;
  uint32 secondaryOutputLength;
  uint32 secondaryOutputLengthWritten;
  Crypto_JobPrimitiveInputOutputType jobPrimitiveInputOutput_Restore;
} Crypto_30_LibCv_Redirect_Type;

/* Key List */
typedef struct
{
  uint32 keyId;
  uint8 keyAccess;
} Crypto_30_LibCv_KeyLockKeyType;

typedef struct
{
  Crypto_30_LibCv_KeyLockKeyType keyLockList[CRYPTO_30_LIBCV_MAX_NUM_OF_USED_KEYS];
  uint8 numKeys;
} Crypto_30_LibCv_KeyLockListType;

# define CRYPTO_30_LIBCV_START_SEC_VAR_ZERO_INIT_8BIT
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
/*! Initialization state of the module */
extern VAR(uint8, CRYPTO_30_LIBCV_VAR_ZERO_INIT) Crypto_30_LibCv_ModuleInitialized;
# endif

# define CRYPTO_30_LIBCV_STOP_SEC_VAR_ZERO_INIT_8BIT
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_8BIT
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

extern VAR(boolean, CRYPTO_30_LIBCV_VAR_NOINIT) Crypto_30_LibCv_ProcessJob_Trigger_Write[Crypto_30_LibCv_GetSizeOfDriverObjectState()];

# if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
#  if (CRYPTO_30_LIBCV_SHE_DEBUG_CMD == STD_ON)
/* It is only allowed to access this data if the SheInfoKey is locked */
extern VAR(boolean, CRYPTO_30_LIBCV_VAR_NOINIT) Crypto_30_LibCv_She_Debug_Cmd_ChallengeFlag;
#  endif
# endif

# define CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_8BIT
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_16BIT
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if (CRYPTO_30_LIBCV_DEFAULT_RANDOM_SOURCE == STD_ON)
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
extern VAR(uint16, CRYPTO_30_LIBCV_VAR_NOINIT) Crypto_30_LibCv_RandomSourceGenerateCount; /* PRQA S 1514 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# endif

# define CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_16BIT
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define CRYPTO_30_LIBCV_START_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Init_Key()
 *********************************************************************************************************************/
/*! \brief       Initializes the key element.
 *  \details     -
 *  \param[in]   elementIndex            Holds the pointer to the found key element index.
 *  \param[in]   initAllBytes            Decide if all data bytes shall be init.
 *  \pre         Key is locked for write operations.
 *               Element index need to be valid.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Init_Key(
  Crypto_30_LibCv_KeyElementsIterType elementIndex,
  boolean initAllBytes);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Init_KeySimple()
 *********************************************************************************************************************/
/*! \brief       Initializes the key element.
 *  \details     Initialize only written length, extension and status.
 *  \param[in]   elementIndex            Holds the pointer to the found key element index.
 *  \pre         Key is locked for write operations.
 *               Element index need to be valid.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Init_KeySimple(
  Crypto_30_LibCv_KeyElementsIterType elementIndex);

# if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Init_KeySheAdditional()
 *********************************************************************************************************************/
/*! \brief       Initializes the She key element.
 *  \details     This function initializes additional fields for she keys.
 *  \param[in]   elementIndex            Holds the pointer to the found key element index.
 *  \pre         Key is locked for write operations.
 *               Crypto_30_LibCv_Init_Key need to be called for complete key element initialization.
 *               Element index need to be valid.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        Specification of module initialization
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Init_KeySheAdditional(
  Crypto_30_LibCv_KeyElementsIterType elementIndex);
# endif /* (CRYPTO_30_LIBCV_SHEKEYS == STD_ON) */

# if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_NvBlock_State_Init()
 *********************************************************************************************************************/
/*! \brief         Init Nv State
 *  \details       Init Nv state for the given block.
 *  \param[in]     blockIdx     idx of the block.
 *  \pre           blockIdx need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_State_Init(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_NvBlock_MainFunction()
 *********************************************************************************************************************/
/*! \brief         Nv main function of the module.
 *  \details       This interface process the provided asynchronous NvBlock request / triggers write block.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_MainFunction(void);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_NvBlock_ReadFromBlock()
 **********************************************************************************************************************/
/*!\brief       Read from Block callback routine.
 * \details     Block specific callback routine which is called by NvM in order to let the Crypto driver copy data from
 *              NvM RAM mirror to Crypto RAM block.
 * \param[in]   blockIdx               The internal identifier of the NvM block.
 * \param[in]   NvMBuffer   RAM mirror where Ram block data can be read from.
 * \return      E_OK      Data was copied from buffer.
 * \return      E_NOT_OK  Data was not copied.
 * \pre         blockIdx need to be valid.
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_ReadFromBlock(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx,
  P2CONST(void, AUTOMATIC, CRYPTO_30_LIBCV_APPL_DATA) NvMBuffer);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_NvBlock_WriteToBlock()
 **********************************************************************************************************************/
/*!\brief       Write to Block callback routine.
 * \details     Block specific callback routine which is called by NvM in order to let the Crypto driver copy data from
 *              RAM block to NvM RAM mirror.
 * \param[in]   blockIdx               The internal identifier of the NvM block.
 * \param[out]  NvMBuffer  RAM mirror where Ram block data shall be written to.
 * \return      E_OK      Data was copied to buffer.
 * \return      E_NOT_OK  No data was copied.
 * \pre         blockIdx need to be valid.
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_WriteToBlock(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx,
  P2VAR(void, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) NvMBuffer);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_NvBlock_Init()
 **********************************************************************************************************************/
/*!\brief       Init Block callback routine.
 * \details     Block specific callback routine which is called by NvM in order to let the Crypto driver copy default
 *              data to a RAM block.
 * \param[in]   blockIdx               The internal identifier of the NvM block.
 * \return      E_OK      Data initialized.
 *              E_NOT_OK  Any error occurred.
 * \pre         blockIdx need to be valid.
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_Init(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_NvBlock_Callback()
 **********************************************************************************************************************/
/*!\brief       Request finished Block callback routine.
 * \details     Block specific callback routine which is called by NvM in order to notify the Crypto driver that an
 *              asynchronous single block request has been finished.
 * \param[in]   blockIdx               The internal identifier of the NvM block.
 * \param[in]   NvM_ServiceIdType      The service identifier of the completed request.
 * \param[in]   NvM_RequestResultType  Result of the single block job.
 * \return      E_OK      Callback function has been processed successfully.
 * \return      E_NOT_OK  Callback function has not been processed successfully.
 * \pre         blockIdx need to be valid.
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_Callback(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx,
  uint8 ServiceId,
  NvM_RequestResultType JobResult);
# endif /* (CRYPTO_30_LIBCV_NVBLOCK == STD_ON) */

# if (CRYPTO_30_LIBCV_USE_VSTD_LIB == STD_OFF)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_CopyData_Implementation()
 *********************************************************************************************************************/
/*! \brief         Copies data from source to destination.
 *  \details       Copies data from source to destination for the given length. Exemplary implementation.
 *  \param[out]    targetData           Pointer to buffer where the data shall be copied to
 *  \param[in]     sourceData           Pointer to data which shall be copied
 *  \param[in]     dataLength           Number of bytes which shall be copied
 *  \pre           Length of the buffer provided by targetDataBuf can hold at least the number of bytes given in dataLength.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_CopyData_Implementation(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) targetData,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) sourceData,
  uint32 dataLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_SetData_Implementation()
 *********************************************************************************************************************/
/*! \brief         Sets data buffer.
 *  \details       Sets data buffer for the given length with pattern value. Exemplary implementation.
 *  \param[out]    dataBuf                 Pointer to data which shall be set.
 *  \param[in]     pattern                 Holds the value which shall be set.
 *  \param[in]     dataLength              Number of bytes which shall be set.
 *  \pre           Length of the buffer provided by targetDataBuf can hold at least the number of bytes given in dataLength.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_SetData_Implementation(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataBuf,
  uint8 pattern,
  uint32 dataLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_ClearData_Implementation()
 *********************************************************************************************************************/
/*! \brief         Clears data buffer.
 *  \details       Clears data buffer for the given length (overwrite with 0x00). Exemplary implementation.
 *  \param[out]    dataBuf                 Pointer to data which shall be cleared
 *  \param[in]     dataLength              Number of bytes which shall be cleared
 *  \pre           Length of the buffer provided by targetDataBuf can hold at least the number of bytes given in dataLength
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_ClearData_Implementation(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataBuf,
  uint32 dataLength);
# endif /* # (CRYPTO_30_LIBCV_USE_VSTD_LIB == STD_OFF) */

# if (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_CompareData_IsSmaller()
 *********************************************************************************************************************/
/*! \brief         Check if the target value is smaller than the reference value
 *  \details       Compare data big-endian bytewise and check if the target value is smaller than the reference value.
 *  \param[in]     targetData     target value array
 *  \param[in]     referenceData  reference value array
 *  \param[in]     dataLength     compare length
 *  \pre           targetData has to be a valid array with the given dataLength.
 *  \pre           referenceData has to be a valid array with the given dataLength.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_CompareData_IsSmaller(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) targetData,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) referenceData,
  uint32 dataLength);
# endif /* (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Uint32ToUint8ArrayBigEndian()
 *********************************************************************************************************************/
/*! \brief         Copies data from source to destination.
 *  \details       Copies data from source to destination from uint32 to uint8 array big endian.
 *  \param[out]    targetData           Pointer to buffer where the data shall be copied to
 *  \param[in]     sourceData           Data which shall be copied
 *  \pre           All pointers need to be valid.
 *                 targetData need to have at least byte size of unit32.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_ALWAYS_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Uint32ToUint8ArrayBigEndian(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) targetData,
  uint32 sourceData);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Uint8ArrayToUint32BigEndian()
 *********************************************************************************************************************/
/*! \brief         Copies data from source to destination.
 *  \details       Copies data from source to destination from uint8 array to uint32 big endian.
 *  \param[out]    targetData           Pointer to buffer where the data shall be copied to
 *  \param[in]     sourceData           Pointer to data which shall be copied
 *  \pre           All pointers need to be valid.
 *                 sourceData need to have at least byte size of unit32.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_ALWAYS_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Uint8ArrayToUint32BigEndian(
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) targetData,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) sourceData);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyListAddKey()
 **********************************************************************************************************************/
/*! \brief         Add Key to key list.
 *  \details       Add key to keyList if the key is not in keyList. If the Key is in KeyList update keyAccess.
 *  \param[in,out] keyList       Pointer to the keyList.
 *  \param[in]     cryptoKeyId   KeyId of the key which shall be add.
 *  \param[in]     keyAccess     Key access right of the key which shall be add.
 *  \pre           keyList has to be a valid keyList object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyListAddKey(
  P2VAR(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList,
  uint32 cryptoKeyId,
  uint8 keyAccess);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_GetKeyList()
 **********************************************************************************************************************/
/*! \brief         Create list of  keys.
 *  \details       A key is only referenced once in the list with the highest access.
 *  \param[in]     job       Pointer to the job which shall checked.
 *  \param[in,out] keyList   Pointer to the keyList.
 *  \pre           job has to be a valid job object.
 *                 keyList has to be a valid keyList object with at least 6 elements if redirection is used, otherwise 1.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_GetKeyList(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_GetKeyListAndDet()
 **********************************************************************************************************************/
/*! \brief         Create list of  keys and check if key exist.
 *  \details       A key is only referenced once in the list with the highest access.
 *  \param[in]     job       Pointer to the job which shall checked.
 *  \param[in,out] keyList   Pointer to the keyList.
 *  \return        E_OK      Request success.
 *                 E_NOT_OK  Request failed, since key id is not valid.
 *  \pre           job has to be a valid job object.
 *                 keyList has to be a valid keyList object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_GetKeyListAndDet(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyListPreLockKeys()
 **********************************************************************************************************************/
/*! \brief         Lock keys in key list.
 *  \details       Lock keys in key list according to their read/write access.
 *  \param[in]     keyList   Pointer to the keyList.
 *  \return        E_OK      Request success.
 *                 E_NOT_OK  Request failed, since key is already locked.
 *  \pre           keyList has to be a valid keyList object.
 *  \context       TASK
 *  \reentrant     TRUE, for different keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyListPreLockKeys(
  P2CONST(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyListPostFreeKeys()
 **********************************************************************************************************************/
/*! \brief         Free keys in key list.
 *  \details       Free keys in key list according to their read/write access.
 *  \param[in]     keyList   Pointer to the keyList.
 *  \pre           keyList has to be a valid keyList object.
 *  \context       TASK
 *  \reentrant     TRUE, for different keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyListPostFreeKeys(
  P2CONST(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyElementSearch()
 *********************************************************************************************************************/
/*! \brief         Searches the key elementIndex.
 *  \details       Searches the key elementIndex of the given keyElementId in the given cryptoKeyId.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be set.
 *  \param[out]    elementIndex            Holds the pointer to the found key element index.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed, key element not found.
 *  \pre           cryptoKeyId has to be a valid identifier to a key in the keyStorage.
 *                 elementIndex has to be a valid element index Ptr.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementSearch(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) elementIndex);

# if ((CRYPTO_30_LIBCV_KEY_DERIVE_ALGORITHM == STD_ON) || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM == STD_ON) || (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM == STD_ON))
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyElementSet()
 *********************************************************************************************************************/
/*! \brief         Sets a key element. This Api is only for internal usage.
 *  \details       Sets the given key element bytes to the key identified by cryptoKeyId.
 *                 If keyLength is zero the old value is deleted.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be set.
 *  \param[in]     keyPtr                  Holds the pointer to the key data which shall be set as key element.
 *  \param[in]     keyLength               Contains the length of the key element in bytes.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_WRITE_FAIL Request failed, write access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                             data.
 *  \pre           cryptoKeyId has to be a valid identifier to a key in the keyStorage.
 *                 The crypto key has to be locked for write access.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementSet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength);
# endif /* ((CRYPTO_30_LIBCV_KEY_DERIVE_ALGORITHM == STD_ON) || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM == STD_ON) || (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM == STD_ON)) */

# if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_NvBlock_Write_Req()
 **********************************************************************************************************************/
/*! \brief         Set write request.
 *  \details       Set request wo write block.
 *  \param[in]     blockIdx             Holds the identifier of the nv block.
 *  \pre           blockIdx has to be a valid identifier.
 *  \context       TASK
 *  \reentrant     TRUE, for different blocks.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_Write_Req(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx);
# endif

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyElementGetStorageIndex()
 *********************************************************************************************************************/
/*! \brief         The key element can be provided for all read access rights values.
 *  \details       This interface shall be used internal to get a key element of the key identified by the cryptoKeyId and store
 *                 the key element in the memory location pointed to by the result pointer.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key element storage index.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be set.
 *  \param[in,out] resultIndexPtr          Holds the pointer to the key data which shall be set as key element.
 *  \param[in,out] resultLengthPtr         Contains the length of the key element in bytes.
 *  \param[in]     lengthCheck             Length check mode.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_KEY_READ_FAIL  Request failed, read access was denied.
 *                 CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                 CRYPTO_E_SMALL_BUFFER       Request failed, the provided buffer is too small to store the result.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid identifier to a key in the keyStorage.
 *                 The crypto key has to be locked for read access.
 *                 resultIndexPtr has to be a valid ptr.
 *                 resultLengthPtr has to be a valid ptr.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetStorageIndex(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) resultIndexPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) resultLengthPtr,
  Crypto_30_LibCv_LengthCheckType lengthCheck);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_GetElementsIndexJob()
 *********************************************************************************************************************/
/*! \brief         Get the key element storage index for all key elements in the list used for Job Processing.
 *  \details       The function will provide the storage index of all required key elements.
 *                 The function return values can be passed to process job.
 *                 If the request fails the function return the first error also if there are several.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key element storage index.
 *  \param[in,out] keyElements             Holds the pointer to the key elements structure.
 *  \param[in]     numberOfElements        Holds the total number of key elements contained in keyElements.
 *  \param[in]     lengthCheck             Length check mode.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid identifier to a key in the keyStorage.
 *                 The crypto key has to be locked for read access.
 *                 keyElements has to be a valid ptr with the number of numberOfElements elements.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_GetElementsIndexJob(
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_KeyElementGetType, AUTOMATIC, AUTOMATIC) keyElements,
  Crypto_30_LibCv_KeyElementGetSizeType numberOfElements,
  Crypto_30_LibCv_LengthCheckType lengthCheck);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_ElementGetterSetId()
 **********************************************************************************************************************/
/*! \brief         Set key element id in keyElements structure.
 *  \details       -
 *  \param[in,out] keyElements             Holds the pointer to the key elements structure.
 *  \param[in]     elementNumber           Holds the total number of the key element which shall be accessed.
 *  \param[in]     keyElementId            Holds the key element id which shall be set.
 *  \pre           keyElements has to be a valid ptr with at least the length of elementNumber elements.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_ElementGetterSetId(
  P2VAR(Crypto_30_LibCv_KeyElementGetType, AUTOMATIC, AUTOMATIC) keyElements,
  Crypto_30_LibCv_KeyElementGetSizeType elementNumber,
  uint32 keyElementId);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_ElementGetterSetIdAndLength()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
/*! \brief         Set key element id and expected length in keyElements structure.
 *  \details       -
 *  \param[in,out] keyElements             Holds the pointer to the key elements structure.
 *  \param[in]     elementNumber           Holds the total number of the key element which shall be accessed.
 *  \param[in]     keyElementId            Holds the key element id which shall be set.
 *  \param[in]     keyLength               Holds the key element expected length which shall be set.
 *  \pre           keyElements has to be a valid ptr with at least the length of elementNumber elements.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_ElementGetterSetIdAndLength(
  P2VAR(Crypto_30_LibCv_KeyElementGetType, AUTOMATIC, AUTOMATIC) keyElements,
  Crypto_30_LibCv_KeyElementGetSizeType elementNumber,
  uint32 keyElementId,
  uint32 keyLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyElementGetStorageIndexJobOptional()
 *********************************************************************************************************************/
/*! \brief         Get the key element storage index for an optional key element used for Job Processing.
 *  \details       The function will provide the storage index of the required key element.
 *                 The function return values can be passed to process job expect CRYPTO_E_KEY_NOT_AVAILABLE.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key element storage index.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be set.
 *  \param[in,out] resultIndexPtr          Holds the pointer to the key data which shall be set as key element.
 *  \param[in,out] resultLengthPtr         Contains the length of the key element in bytes.
 *  \param[in]     lengthCheck             Length check mode.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid identifier to a key in the keyStorage.
 *                 The crypto key has to be locked for read access.
 *                 resultIndexPtr has to be a valid ptr.
 *                 resultLengthPtr has to be a valid ptr.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetStorageIndexJobOptional(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) resultIndexPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) resultLengthPtr,
  Crypto_30_LibCv_LengthCheckType lengthCheck);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyElementGetStorageIndexJob()
 *********************************************************************************************************************/
/*! \brief         Get the key element storage index for a key element used for Job Processing.
 *  \details       The function will provide the storage index of the required key element.
 *                 The function return values can be passed to process job.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key element storage index.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be set.
 *  \param[in,out] resultIndexPtr          Holds the pointer to the key data which shall be set as key element.
 *  \param[in,out] resultLengthPtr         Contains the length of the key element in bytes.
 *  \param[in]     lengthCheck             Length check mode.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid identifier to a key in the keyStorage.
 *                 The crypto key has to be locked for read access.
 *                 resultIndexPtr has to be a valid ptr.
 *                 resultLengthPtr has to be a valid ptr.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetStorageIndexJob(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) resultIndexPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) resultLengthPtr,
  Crypto_30_LibCv_LengthCheckType lengthCheck);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyElementGetStorageIndexExtended()
 *********************************************************************************************************************/
/*! \brief         The key element storage index can be provided for all read access rights values.
 *  \details       This interface shall be used internal to get a key element of the key identified by the
 *                 cryptoKeyId and store the key element in the memory location pointed to by the result pointer.
 *                 The key element will be searched and checked vor validity.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be provided.
 *  \param[in]     keyElementId            Holds the Id of the key element which shall be provided.
 *  \param[in,out] resultIndexPtr          Holds the pointer to the storage index which shall be provided.
 *  \param[in,out] resultLengthPtr         Contains the length of the key element in bytes.
 *  \param[in]     lengthCheck             Length check mode.
 *  \param[in]     serviceType             SHE service Type.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_KEY_READ_FAIL  Request failed, read access was denied.
 *                 CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                 CRYPTO_E_SMALL_BUFFER       Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid identifier to a key in the keyStorage.
 *                 The crypto key has to be locked for read access.
 *                 resultIndexPtr has to be a valid ptr.
 *                 resultLengthPtr has to be a valid ptr.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetStorageIndexExtended(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultIndexPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr,
  Crypto_30_LibCv_LengthCheckType lengthCheck,
  Crypto_30_LibCv_ServiceType serviceType);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyElementGetStorageIndexBasic()
 *********************************************************************************************************************/
/*! \brief         The key element storage index can be provided for all read access rights values.
 *  \details       This interface shall be used internal to get a key element of the key identified by the
 *                 cryptoKeyId and store the key element in the memory location pointed to by the result pointer.
 *                 The key element will not be checked for validity.
 *  \param[in]     elementIndex            Holds the index of the key element which shall be provided.
 *  \param[in,out] resultIndexPtr          Holds the pointer to the storage index which shall be provided.
 *  \param[in,out] resultLengthPtr         Contains the length of the key element in bytes.
 *  \param[in]     lengthCheck             Length check mode.
 *  \param[in]     serviceType             SHE service Type.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_KEY_READ_FAIL  Request failed, read access was denied.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           elementIndex has to be a valid identifier to a key in the keyStorage.
 *                 The crypto key has to be locked for read access.
 *                 resultIndexPtr has to be a valid ptr.
 *                 resultLengthPtr has to be a valid ptr.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetStorageIndexBasic(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) resultIndexPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) resultLengthPtr,
  Crypto_30_LibCv_LengthCheckType lengthCheck,
  Crypto_30_LibCv_ServiceType serviceType);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyElementSetInternal()
 *********************************************************************************************************************/
/*! \brief         Sets a key element. This Api is only for internal usage.
 *  \details       Sets the given key element bytes to the key identified by cryptoKeyId.
 *                 If keyLength is zero the old value is deleted.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key storage index.
 *  \param[in]     elementIndex            Holds the index of the key element which shall be set.
 *  \param[in]     keyPtr                  Holds the pointer to the key data which shall be set as key element.
 *  \param[in]     keyLength               Contains the length of the key element in bytes.
 *  \param[in]     writeAccess             Hold the max write access right which could be accessed.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_WRITE_FAIL Request failed, write access was denied.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                             data.
 *  \pre           cryptoKeyId has to be a valid identifier to a key in the keyStorage.
 *                 elementIndex has to be a valid identifier to a keyElement in the keyStorage.
 *                 The crypto key has to be locked for write access.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementSetInternal(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength,
  Crypto_30_LibCv_WriteOfKeyElementInfoType writeAccess);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_KeyElementSetInternalStandard()
 *********************************************************************************************************************/
/*! \brief         Copies data to key storage.
 *  \details       Copies data of the keyPtr to the internal key storage identified by the elementIndex.
 *                 If keyLength is zero the old value is deleted.
 *  \param[in]     elementIndex            Holds the identifier of the keyElement in the keyStorage.
 *  \param[in]     keyPtr                  Holds the data which shall be copied to the key storage
 *  \param[in]     keyLength               Specifies the number of bytes which shall be copied.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed, keyElement was written, but connected key not found.
 *  \pre           elementIndex has to be a valid identifier to a keyElement in the keyStorage.
 *                 keyLength has to be smaller or equal in length to available key storage space.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementSetInternalStandard(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_KeyElementSetInternalStandardWithCryptoKeyId()
 *********************************************************************************************************************/
/*! \brief         Copies data to key storage.
 *  \details       Copies data of the keyPtr to the internal key storage identified by the elementIndex.
 *                 If keyLength is zero the old value is deleted.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key storage index.
 *  \param[in]     elementIndex            Holds the identifier of the keyElement in the keyStorage.
 *  \param[in]     keyPtr                  Holds the data which shall be copied to the key storage
 *  \param[in]     keyLength               Specifies the number of bytes which shall be copied.
 *  \pre           cryptoKeyId has to be a valid identifier to a key in the keyStorage.
 *                 elementIndex has to be a valid identifier to a keyElement in the keyStorage.
 *                 keyPtr has to be a valid ptr.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementSetInternalStandardWithCryptoKeyId(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SetKeyElementWrittenLength()
 *********************************************************************************************************************/
/*! \brief         Sets the key element written length.
 *  \details       Sets the written length of a key element in the key storage.
 *  \param[in]     keyElementIndex         Holds the index of the key element whose written length shall be set.
 *  \param[in]     keyElementLength        Written length of the key element
 *  \pre           keyElementId has to hold a valid identifier of the key element
 *  \note          This function does not call the callout for the key value changed
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SetKeyElementWrittenLength(
  Crypto_30_LibCv_SizeOfKeyElementsType keyElementIndex,
  uint32 keyElementLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyIdSearch()
 *********************************************************************************************************************/
/*! \brief         Sets the key element written length.
 *  \details       Sets the written length of a key element in the keystore and searches the crypto key ID
 *                 to call the callout to change the key value.
 *  \param[in]     keyElementIndex         Holds the index of the key element whose written length shall be set.
 *  \param[in]     keyElementLength        Written length of the key element
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed, key not found.
 *  \pre           keyElementId has to hold a valid identifier of the key element
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyIdSearch(
  Crypto_30_LibCv_SizeOfKeyElementsType keyElementIndex,
  uint32 keyElementLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId()
 *********************************************************************************************************************/
/*! \brief         Sets the key element written length.
 *  \details       Sets the written length of a key element in the key storage.
 *  \param[in]     cryptoKeyId             Holds the id of the key.
 *  \param[in]     keyElementIndex         Holds the index of the key element whose written length shall be set.
 *  \param[in]     keyElementLength        Written length of the key element
 *  \pre           cryptoKeyId has to be a valid identifier to a key in the keyStorage.
 *                 keyElementId has to hold a valid identifier of the key element
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType keyElementIndex,
  uint32 keyElementLength);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_SetKeyElementStateWritten()
 **********************************************************************************************************************/
/*! \brief         Sets the key element state for a written key element.
 *  \details       Sets the state to invalid and write once if required.
 *  \param[in]     elementIndex   Holds the index of the key element whose written length shall be set.
 *  \pre           keyElementId has to hold a valid identifier of the key element
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_SetKeyElementStateWritten(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SetKeyState()
 *********************************************************************************************************************/
/*! \brief         Sets the state of a key.
 *  \details       The function sets the key state and respectively the key states of all associated key elements.
 *  \param[in]     cryptoKeyId           Holds the identifier of the key.
 *  \param[in]     mask                  State of the key, consistent of different flags.
 *  \pre           cryptoKeyId has to hold a valid identifier of a key
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(boolean, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SetKeyState(
  uint32 cryptoKeyId,
  uint8 mask);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyReadLockGet()
 *********************************************************************************************************************/
/*!
 * \brief           Try to get key read lock.
 * \details         Try to get a key read lock. Increment the key read lock, if there is no write lock.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \return          E_OK               Lock key read access success.
 *                  E_NOT_OK           Lock key read access failed.
 * \pre             cryptoKeyId        Has to hold a valid identifier of a key.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyReadLockGet(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyReadLockRelease()
 *********************************************************************************************************************/
/*!
 * \brief           Release key read lock.
 * \details         Release key read lock. Decrement the key read lock.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \pre             cryptoKeyId        Has to hold a valid identifier of a key.
 *                  The caller has to ensure that the key is locked for read access.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyReadLockRelease(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyReadLockGetNotProtected()
 *********************************************************************************************************************/
/*!
 * \brief           Try to get key read lock.
 * \details         This access is not protected by a exclusive area.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \return          E_OK               Lock key read access success.
 *                  E_NOT_OK           Lock key read access failed.
 * \pre             cryptoKeyId        Has to hold a valid identifier of a key.
 *                  The caller has to ensure that this call is protected by a exclusive area.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyReadLockGetNotProtected(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyReadLockReleaseNotProtected()
 *********************************************************************************************************************/
/*!
 * \brief           Release key read lock.
 * \details         This access is not protected by a exclusive area.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \pre             cryptoKeyId        Has to hold a valid identifier of a key.
 *                  The caller has to ensure that this call is protected by a exclusive area.
 *                  The caller has to ensure that the key is locked for read access.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyReadLockReleaseNotProtected(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyWriteLockGet()
 *********************************************************************************************************************/
/*!
 * \brief           Try to get key write lock.
 * \details         Try to get key write lock. Only one write lock can be allocated, if there is no read lock.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \return          E_OK               Lock key write access success.
 *                  E_NOT_OK           Lock key write access failed.
 * \pre             cryptoKeyId        Has to hold a valid identifier of a key.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyWriteLockGet(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyWriteLockRelease()
 *********************************************************************************************************************/
/*!
 * \brief           Release key write lock.
 * \details         Release key write lock. Only one write lock can be allocated.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \pre             cryptoKeyId        Has to hold a valid identifier of a key.
 *                  The caller has to ensure that the key is locked for write access.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyWriteLockRelease(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyWriteLockGetNotProtected()
 *********************************************************************************************************************/
/*!
 * \brief           Try to get key write lock.
 * \details         Try to get key write lock. Only one write lock can be allocated, if there is no read lock.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \return          E_OK               Lock key write access success.
 *                  E_NOT_OK           Lock key write access failed.
 *                  The caller has to ensure that this call is protected by a exclusive area.
 * \pre             cryptoKeyId        Has to hold a valid identifier of a key.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyWriteLockGetNotProtected(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyWriteLockReleaseNotProtected()
 *********************************************************************************************************************/
/*!
 * \brief           Release key write lock.
 * \details         Release key write lock. Only one write lock can be allocated.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \pre             cryptoKeyId        Has to hold a valid identifier of a key.
 *                  The caller has to ensure that this call is protected by a exclusive area.
 *                  The caller has to ensure that the key is locked for write access.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyWriteLockReleaseNotProtected(
  uint32 cryptoKeyId);

# if (CRYPTO_30_LIBCV_LONGTERMWS == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_LongWsLockGet()
 **********************************************************************************************************************/
/*! \brief          Try to obtain long term workspace lock.
 *  \details        This function tries to lock the long term workspace and determines the workspace idx.
 *  \param[in]      cryptoKeyId       Holds the key id which will lock the workspace
 *  \param[out]     longWsIdxPtr      Provided long term workspace idx.
 *  \return         E_OK              Lock is obtained or was already obtained.
 *                  E_NOT_OK          Request Failed
 *  \pre            longWsIdxPtr has to be a valid pointer.
 *  \note           Is a lock is already occupied for the key id the service will return this Idx.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_LongWsLockGet(
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_SizeOfLongTermWsLockType, AUTOMATIC, AUTOMATIC) longWsIdxPtr);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_LongWsIsLock()
 **********************************************************************************************************************/
/*! \brief          Check if long term workspace is locked.
 *  \details        This function checks if the long term workspace is locked and proviedes the workspace idx.
 *  \param[in]      cryptoKeyId       Holds the key id which will lock the workspace
 *  \param[out]     longWsIdxPtr      Provided long term workspace idx.
 *  \return         E_OK              Lock is obtained or was already obtained.
 *                  E_NOT_OK          Request Failed
 *  \pre            longWsIdxPtr has to be a valid pointer.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_LongWsIsLock(
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_SizeOfLongTermWsLockType, AUTOMATIC, AUTOMATIC) longWsIdxPtr);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_LongWsLockRelease()
 **********************************************************************************************************************/
/*! \brief          Release long term workspace lock.
 *  \details        -
 *  \param[in]      cryptoKeyId       Holds the key id which will lock the workspace
 *  \param[in]      longWsIdx         Holds the long term workspace idx.
 *  \pre            longWsIdx has to be a valid index.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_LongWsLockRelease(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfLongTermWsLockType longWsIdx);
# endif /* (CRYPTO_30_LIBCV_LONGTERMWS == STD_ON) */

# if (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_IsObjectWorkspaceUnchanged()
 *********************************************************************************************************************/
/*! \brief          Check if the driver object wokspace type is unchanged.
 *  \details        -
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in]      job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_IsObjectWorkspaceUnchanged(
  uint32 objectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_ClearObjectWorkspaceForChangedKey()
 *********************************************************************************************************************/
/*! \brief          Clear the last driver object wokspace type for all driver objects which the given key was used.
 *  \details        -
 *  \param[in]      cryptoKeyId        Id of the crypto key.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            cryptoKeyId        Has to hold a valid identifier of a key.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_ClearObjectWorkspaceForChangedKey(
  uint32 cryptoKeyId);
# endif /* (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Uint32ToUint8ArrayBigEndian()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_ALWAYS_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Uint32ToUint8ArrayBigEndian( /* PRQA S 3219 */ /* MD_CRYPTO_30_LIBCV_3219 */
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) targetData,
  uint32 sourceData)
{
  targetData[0] = (uint8)(((sourceData)& 0xff000000u) >> 24); /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  targetData[1] = (uint8)(((sourceData)& 0x00ff0000u) >> 16); /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  targetData[2] = (uint8)(((sourceData)& 0x0000ff00u) >> 8); /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  targetData[3] = (uint8)(((sourceData)& 0x000000ffu)); /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Uint8ArrayToUint32BigEndian()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_ALWAYS_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Uint8ArrayToUint32BigEndian( /* PRQA S 3219 */ /* MD_CRYPTO_30_LIBCV_3219 */
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) targetData,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) sourceData)
{
  *targetData = ((((uint32)sourceData[0]) & 0x000000FFu) << 24) | ((((uint32)sourceData[1]) & 0x000000FFu) << 16) | ((((uint32)sourceData[2]) & 0x000000FFu) << 8) | (((uint32)sourceData[3]) & 0x000000FFu); /* PRQA S 2985 */ /* MD_CRYPTO_30_LIBCV_2985 */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
}

# define CRYPTO_30_LIBCV_STOP_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CRYPTO_30_LIBCV_SERVICE_H */
/**********************************************************************************************************************
 *  END OF FILE: CRYPTO_30_LIBCV_SERVICES.H
 *********************************************************************************************************************/
