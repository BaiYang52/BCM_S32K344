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
/*      \file   Crypto_30_LibCv_KeyDerive.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Implementation of Key Derive Services.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_KEYDERIVE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Crypto_30_LibCv.h"
#include "Crypto_30_LibCv_Services.h"
#include "Crypto_30_LibCv_KeyDerive.h"
#include "Crypto_30_LibCv_KeyExchange.h"

#include "Crypto_30_LibCv_Custom.h"
#include "Crypto_30_LibCv_Curve.h"

#if (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
# include "actBigNum.h"
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

 /* KDF */
#define CRYPTO_30_LIBCV_KEY_DERIVE_SIZEOF_ALGORITHM                   (1u)

/* KDF ISO15118 */
#define CRYPTO_30_LIBCV_ISO15118_SIZEOF_IV                            (16u)
#define CRYPTO_30_LIBCV_ISO15118_PRIV_KEY_START                       (CRYPTO_30_LIBCV_ISO15118_SIZEOF_IV)
#define CRYPTO_30_LIBCV_ISO15118_SIZEOF_IV_AND_PRIV_KEY               (48u)
#define CRYPTO_30_LIBCV_ISO15118_SIZEOF_OTHER_INFO                    (3u)

/* KDF NIST 800-56A */
#define CRYPTO_30_LIBCV_NIST80056A_COUNTER_SIZE                       (4u)
#define CRYPTO_30_LIBCV_NIST80056A_OTHER_INFO_MAXSIZE                 (5u)

/* NIST 800-108 NIST/FIPS 186.4 */
#define CRYPTO_30_LIBCV_SIZEOF_KDF_PRF_INPUT_BUFFER                   (256u + 12u)
#define CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_PARENT_KEY                     (256u)
#define CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_TARGET_KEY                     (256u)
#define CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_TARGET_KEY_PLUS_8              (256u + 8u)
#define CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_SYMMETRIC                     (6u)
#define CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_ASYMMETRIC                    (38u)
#define CRYPTO_30_LIBCV_SIZEOF_KDF_PRIME                              (32u)
#define CRYPTO_30_LIBCV_SIZEOF_KDF_CONTEXT                            (4u)
#define CRYPTO_30_LIBCV_SIZEOF_KDF_TARGET_KEY_LENGTH                  (2u)
#define CRYPTO_30_LIBCV_SIZEOF_KDF_LABEL_LENGTH                       (4u)
#define CRYPTO_30_LIBCV_SIZEOF_KDF_ALGORITHM_LENGTH                   (1u)
#define CRYPTO_30_LIBCV_SIZEOF_KDF_PRIME_ASYM_EXTEND                  (8u)

/* General */
#define CRYPTO_30_LIBCV_SIZEOF_KDF_ITERATIONSLENGTH                   (CRYPTO_30_LIBCV_SIZEOF_UINT32)
#define CRYPTO_30_LIBCV_SIZEOF_HKDF_ITERATIONSLENGTH                  (1u)

#if ((CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA256_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA384_ENABLED == STD_ON))
# define CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_ENABLED                   STD_ON
#else
# define CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_ENABLED                   STD_OFF
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if ((CRYPTO_30_LIBCV_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON))

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Kdf_HandleInputParams()
 *********************************************************************************************************************/
/*! \brief        Check KDF input parameters
 *  \details      Gets and checks input parameters of the KDF according to NIST 800-108 NIST/FIPS 186-4.
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[in]    deriveAlgorithm         The algorithm from the Algorithm key element
 *  \param[out]   prfInputBuffer          Input for the PRF
 *  \param[out]   prfInputBufferLength    Written Length of the PRF intput
 *  \param[out]   saltBuffer              The salt from the Salt key element (needs to be defined in the calling function because primeMinus1PtrPtr points into that buffer)
 *  \param[out]   targetKeyLength         Length of the target key
 *  \param[out]   parentKeyLength         Length of the parent key
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *                CRYPTO_E_BUSY           The required key is busy.
 *  \pre          All pointers need to be valid.
 *                Length of the buffer provided by deriveAlgorithm has to be 1 Byte.
 *                Length of the buffer provided by prfInputBuffer has to be CRYPTO_30_LIBCV_SIZEOF_KDF_PRF_INPUT_BUFFER Bytes.
 *                Length of the buffer provided by saltBuffer has to be CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_ASYMMETRIC Bytes.
 *                cryptoKeyId and targetCryptoKeyId have to be a valid identifiers.
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_HandleInputParams(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  uint8 deriveAlgorithm,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) prfInputBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) prfInputBufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) saltBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) targetKeyLength,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) parentKeyLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt()
 *********************************************************************************************************************/
/*! \brief        Gets and checks the salt input parameter of the KDF
 *  \details      Gets and checks the salt input parameter of the KDF according to NIST 800-108 NIST/FIPS 186-4.
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[in]    deriveAlgorithm         The algorithm from the Algorithm key element
 *  \param[out]   prfInputBuffer          Input for the PRF
 *  \param[out]   prfInputBufferLength    Written Length of the PRF intput
 *  \param[out]   saltBuffer              The salt from the Salt key element (needs to be defined in the calling function because primeMinus1PtrPtr points into that buffer)
 *  \param[out]   targetKeyLength         Length of the target key
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          Length of the buffer provided by deriveAlgorithm has to be 1 Byte.
 *                Length of the buffer provided by prfInputBuffer has to be CRYPTO_30_LIBCV_SIZEOF_KDF_PRF_INPUT_BUFFER Bytes.
 *                Length of the buffer provided by saltBuffer has to be CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_ASYMMETRIC Bytes.
 *                cryptoKeyId and targetCryptoKeyId have to be a valid identifiers.
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  uint8 deriveAlgorithm,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) prfInputBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) prfInputBufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) saltBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) targetKeyLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt_TargetKeyLength()
 *********************************************************************************************************************/
/*! \brief        Gets and checks the target key length from the salt input parameter of the KDF
 *  \details      Gets and checks the target key length from the salt input parameter of the KDF according to
 *                NIST 800-108 NIST/FIPS 186-4.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[in]    deriveAlgorithm         The algorithm from the Algorithm key element
 *  \param[in]    saltBuffer              The salt from the Salt key element (needs to be defined in the calling function because primeMinus1PtrPtr points into that buffer)
 *  \param[out]   targetKeyLength         Length of the target key
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          Length of the buffer provided by deriveAlgorithm has to be 1 Byte.
 *                Length of the buffer provided by saltBuffer has to be CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_ASYMMETRIC Bytes.
 *                targetCryptoKeyId has to be a valid identifier.
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt_TargetKeyLength(
  uint32 targetCryptoKeyId,
  uint8 deriveAlgorithm,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) saltBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) targetKeyLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Kdf_CallPRF()
 *********************************************************************************************************************/
/*! \brief         Calls the PRF function of the KDF
 *  \details       Calls the PRF function of the KDF according to NIST 800-108 NIST/FIPS 186-4.
 *  \param[in,out] workspace               Holds the primitive workspace.
 *  \param[in,out] prfInputBuffer          Input for the PRF
 *  \param[in]     prfInputBufferLength    Written Length of the PRF intput
 *  \param[out]    targetKey               Buffer for the target key
 *  \param[in]     targetKeyLength         Length of the target key
 *  \param[in]     parentKeyLength         Length of the parent key
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by targetKey has to be CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_TARGET_KEY_PLUS_8 Bytes.
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_CallPRF(
  P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) prfInputBuffer,
  uint32 prfInputBufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) targetKey,
  uint32 targetKeyLength, uint32 parentKeyLength);
#endif /* #if ((CRYPTO_30_LIBCV_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Kdf_AsymDivideByPrimeMinusOne()
 *********************************************************************************************************************/
/*! \brief        Divides the given target key by the given prime-1 and adds one to the result
 *  \details      Divides the given target key by the given prime-1 and adds one to the result according to NIST/FIPS 186-4.
 *  \param[out]   targetKey               Buffer for the target key
 *  \param[in]    targetKeyLength         Length of the target key
 *  \param[in]    primeMinus1Ptr       Pointer to the position of the prime minus-1 data within the salt
 *  \param[in]    primeMinus1Length       Length of the prime-1
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          Length of the buffer provided by targetKey has to be CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_TARGET_KEY_PLUS_8 Bytes.
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_AsymDivideByPrimeMinusOne(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) targetKey,
  uint32 targetKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) primeMinus1Ptr,
  uint32 primeMinus1Length);
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S_With_Ws_AndLoadKey()
 *********************************************************************************************************************/
/*! \brief        Calculate one pass c1e1s KDF.
 *  \details      Calculate the the one pass c1e1s corresponding to NIST 800 56A and load all required key elements.
 *  \param[in,out] workspace               Holds the primitive workspace.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]     targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *                CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre          cryptoKeyId is a valid key id.
 *                targetCryptoKeyId is a valid key id.
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S_With_Ws_AndLoadKey(
  P2VAR(Crypto_30_LibCv_WorkSpaceISO15118, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S_AndLoadKey()
 *********************************************************************************************************************/
/*! \brief        Calculate one pass c1e1s KDF.
 *  \details      Calculate the the one pass c1e1s corresponding to NIST 800 56A and load all required key elements.
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *                CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre          cryptoKeyId is a valid key id.
 *                targetCryptoKeyId is a valid key id.
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *  \trace        CREQ-145593
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S_AndLoadKey(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON) */

#if ((CRYPTO_30_LIBCV_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON) \
    || (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON))
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyDerive_Nist80056A_SingleStepKdfHash()
 *********************************************************************************************************************/
/*! \brief          Calculate single step KDF.
 *  \details        Calculate the single step kdf with HASH/Sha256 corresponding to NIST 800 56A.
 *  \param[in,out]  workspace             Hols the primitive workspace.
 *  \param[in]      sharedSecretPtr       Pointer to the shared secret.
 *  \param[in]      sharedSecretLength    Length of the shared secret.
 *  \param[in]      otherInfoPtr          Pointer to the other-information.
 *  \param[in]      otherInfoLength       Length of the other-information.
 *  \param[out]     derivedKeyPtr         Pointer to the derived key.
 *  \param[in,out]  derivedKeyLengthPtr   Pointer to the length of the derived key.
 *  \return         E_OK                  Request successful.
 *                  E_NOT_OK              Request failed.
 *  \pre            sharedSecretPtr has to be a valid pointer with the length sharedSecretLength.
 *                  otherInfoPtr has to be a valid pointer with the length otherInfoLength.
 *                  derivedKeyPtr has to be a valid pointer with the length *derivedKeyLengthPtr.
 *                  derivedKeyLengthPtr has to be a valid length pointer.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Nist80056A_SingleStepKdfHash(
  P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) sharedSecretPtr,
  uint32 sharedSecretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) otherInfoPtr,
  uint32 otherInfoLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) derivedKeyPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) derivedKeyLengthPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S()
 *********************************************************************************************************************/
/*! \brief          Calculate one pass c1e1s KDF.
 *  \details        Calculate the the one pass c1e1s corresponding to NIST 800 56A.
 *  \param[in,out]  workspace             Hols the primitive workspace.
 *  \param[in]      privateKeyPtr         Pointer to the private key.
 *  \param[in]      privateKeyLength      Length of the private key.
 *  \param[in]      partnerPubKeyPtr      Pointer to the partner public key.
 *  \param[in]      partnerPubKeyLength   Length of the partner public key.
 *  \param[in]      otherInfoPtr          Pointer to the other-information.
 *  \param[in]      otherInfoPtrLength    Length of the other-information.
 *  \param[out]     derivedKeyPtr         Pointer to the derived key.
 *  \param[in,out]  derivedKeyLengthPtr   Pointer to the length of the derived key.
 *  \return         E_OK                  Request successful.
 *                  E_NOT_OK              Request failed.
 *  \pre            privateKeyPtr has to be a valid pointer with the length privateKeyLength.
 *                  partnerPubKeyPtr has to be a valid pointer with the length partnerPubKeyLength.
 *                  otherInfoPtr has to be a valid pointer with the length otherInfoLength.
 *                  derivedKeyPtr has to be a valid pointer with the length *derivedKeyLengthPtr.
 *                  derivedKeyLengthPtr has to be a valid length pointer.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S(
  P2VAR(Crypto_30_LibCv_WorkSpaceISO15118, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) privateKeyPtr,
  uint32 privateKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) partnerPubKeyPtr,
  uint32 partnerPubKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) otherInfoPtr,
  uint32 otherInfoPtrLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) derivedKeyPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) derivedKeyLengthPtr);
#endif /* ((CRYPTO_30_LIBCV_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON) \
              || (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyDerive_ISO15118_CheckKey()
 *********************************************************************************************************************/
/*! \brief          Check key according to ISO15118.
 *  \details        Check if the key pair is valid according to ISO15118.
 *  \param[in,out]  workspace               Hols the primitive workspace.
 *  \param[in]      privateKeyPtr         Pointer to the private key.
 *  \param[in]      privateKeyLength      Length of the private key.
 *  \param[in]      pubKeyPtr             Pointer to the public key.
 *  \param[in]      pubKeyLength          Length of the public key.
 *  \return         E_OK                  Request successful.
 *                  E_NOT_OK              Request failed.
 *  \pre            privateKeyPtr has to be a valid pointer with the length privateKeyLength.
 *                  pubKeyPtr has to be a valid pointer with the length pubKeyLength.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_ISO15118_CheckKey(
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) privateKeyPtr,
  uint32 privateKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) pubKeyPtr,
  uint32 pubKeyLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyDerive_ISO15118_Aes128Decrypt()
 *********************************************************************************************************************/
/*! \brief          Decrypt Aes128 data.
 *  \details        Decrypt Aes128 data corresponding to ISO15118(extract private key).
 *  \param[in,out]  workspace               Hols the primitive workspace.
 *  \param[in]      ivPtr                   Pointer to the initial vector.
 *  \param[in]      keyPtr                  Pointer to the encryption key.
 *  \param[in]      encryptedMessagePtr     Pointer to the encrypted message.
 *  \param[in]      encryptedMessageLength  Length of the encrypted message.
 *  \param[out]     plainMessagePtr         Pointer to the plain message.
 *  \param[in,out]  plainMessageLengthPtr   Length of theplain message.
 *  \return         E_OK                  Request successful.
 *                  E_NOT_OK              Request failed.
 *  \pre            encryptedMessagePtr has to be a valid pointer with the length encryptedMessageLength.
 *                  plainMessagePtr has to be a valid pointer with the length plainMessageLengthPtr.
 *                  plainMessageLengthPtr has to be a length valid pointer.
 *                  ivPtr has to be a length valid pointer to a valid IV with the length CRYPTO_30_LIBCV_ISO15118_SIZEOF_IV.
 *                  keyPtr has to be a length valid pointer to a valid AES128 key with the length ESL_SIZEOF_AES128_KEY.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_ISO15118_Aes128Decrypt(
  P2VAR(eslt_WorkSpaceAES128, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) ivPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) encryptedMessagePtr,
  uint32 encryptedMessageLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) plainMessagePtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) plainMessageLengthPtr);
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON) */

#if (CRYPTO_30_LIBCV_SERVICE_KEY_DERIVE == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_Derive_UpdateKeyState()
 **********************************************************************************************************************/
/*! \brief         Update Key State after key derive.
 *  \details       -
 *  \param[in]     objectId           Holds the identifier of driver object.
 *  \param[in]     targetCryptoKeyId  Holds the identifier of the key whose key element shall be set.
 *  \param[in]     retVal             Holds the retVal of the key derive.
 *  \pre           cryptoKeyId has to be a valid identifier.
 *                 objectId has to be a valid identifier.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Derive_UpdateKeyState(
  uint32 objectId,
  uint32 targetCryptoKeyId,
  Std_ReturnType retVal);
#endif

#if (CRYPTO_30_LIBCV_KEY_DERIVE_ALGORITHM == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementSet_Check()
 **********************************************************************************************************************/
/*! \brief         Checks the availability of the key element, the write access and determines the configured key element length
 *  \details       -
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     keyElementId            Holds the id of the key element which shall be set.
 *  \param[out]    elementIndexPtr         Holds the index of the key element which shall be set.
 *  \param[out]    keyLengthPtr            Contains the length of the key element in bytes.
 *  \param[in]     writeAccess             Hold the max write access right which could be accessed.
 *  \return        E_OK                    Request successful.
 *                 CRYPTO_E_KEY_WRITE_FAIL Request failed, write access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *  \pre           cryptoKeyId has to be a valid identifier.
 *                 The crypto key has to be locked for write access.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementSet_Check( /* PRQA S 3219 */ /* MD_CRYPTO_30_LIBCV_3219_1 */
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) elementIndexPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) keyLengthPtr,
  Crypto_30_LibCv_WriteOfKeyElementInfoType writeAccess);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementSet_CheckAndLength()
 **********************************************************************************************************************/
/*! \brief         Checks the availability of the key element, the write access, determines and checks the configured key element length
 *  \details       -
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     keyElementId            Holds the id of the key element which shall be set.
 *  \param[out]    elementIndexPtr         Holds the index of the key element which shall be set.
 *  \param[in]     requestedKeyLength      Contains the length of the key element in bytes.
 *  \param[in]     writeAccess             Hold the max write access right which could be accessed.
 *  \return        E_OK                    Request successful.
 *                 CRYPTO_E_KEY_WRITE_FAIL Request failed, write access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *  \pre           cryptoKeyId has to be a valid identifier.
 *                 The crypto key has to be locked for write access.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementSet_CheckAndLength( /* PRQA S 3219 */ /* MD_CRYPTO_30_LIBCV_3219_1 */
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) elementIndexPtr,
  uint32 requestedKeyLength,
  Crypto_30_LibCv_WriteOfKeyElementInfoType writeAccess);
#endif /* CRYPTO_30_LIBCV_KEY_DERIVE_ALGORITHM == STD_ON */

#if ((CRYPTO_30_LIBCV_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON))
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyDerive_Nist800108NistFips1864_WS()
 *********************************************************************************************************************/
/*! \brief        Derive Key for NIST 800-108 NIST/FIPS 186.4
 *  \details      Dispatch and derive the key according to NIST 800-108 NIST/FIPS 186-4.
 *  \param[in,out] workspace               Holds the primitive workspace.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]     targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[in]     deriveAlgorithm         The algorithm from the Algorithm key element
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *                CRYPTO_E_BUSY           The required key is busy.
 *  \pre          cryptoKeyId has to be a valid crypto key.
 *                targetCryptoKeyId has to be a valid crypto key.
 *                deriveAlgorithm has to be a valid (CRYPTO_30_LIBCV_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256
 *                or CRYPTO_30_LIBCV_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB).
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Nist800108NistFips1864_WS(
  P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  uint8 deriveAlgorithm);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyDerive_Nist800108NistFips1864()
 *********************************************************************************************************************/
/*! \brief        Derive Key for NIST 800-108 NIST/FIPS 186.4
 *  \details      Dispatch and derive the key according to NIST 800-108 NIST/FIPS 186-4.
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[in]    deriveAlgorithm         The algorithm from the Algorithm key element
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *                CRYPTO_E_BUSY           The required key is busy.
 *  \pre          cryptoKeyId has to be a valid crypto key.
 *                targetCryptoKeyId has to be a valid crypto key.
 *                deriveAlgorithm has to be a valid (CRYPTO_30_LIBCV_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256
 *                or CRYPTO_30_LIBCV_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB).
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *  \trace        CREQ-132407, CREQ-145592
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Nist800108NistFips1864(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  uint8 deriveAlgorithm);
#endif /* CRYPTO_30_LIBCV_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED || CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED */

#if (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyDerive_ISO15118_WS()
 *********************************************************************************************************************/
/*! \brief        Derives a ISO15118 key.
 *  \details      Install and update the key according to ISO15118. Calculate the session key an decrypt the new private key.
 *  \param[in,out] workspace               Holds the primitives workspace.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]     targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *                CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre          cryptoKeyId is a valid key id.
 *                targetCryptoKeyId is a valid key id.
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_ISO15118_WS(
  P2VAR(Crypto_30_LibCv_WorkSpaceISO15118, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyDerive_ISO15118()
 *********************************************************************************************************************/
/*! \brief        Derives a ISO15118 key.
 *  \details      Install and update the key according to ISO15118. Calculate the session key an decrypt the new private key.
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *                CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre          cryptoKeyId is a valid key id.
 *                targetCryptoKeyId is a valid key id.
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *  \trace        CREQ-145594
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_ISO15118(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);
#endif /* CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED */

#if (CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA1_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA1()
 **********************************************************************************************************************/
/*! \brief        Derives a PBKDF2 HMAC SHA1 key.
 *  \details      Derives a key according to PBKDF2 HMAC SHA1 with the given CRYPTO_KE_KEYDERIVATION_PASSWORD(Secret),
 *                CRYPTO_KE_KEYDERIVATION_SALT(Seed) and CRYPTO_KE_KEYDERIVATION_ITERATIONS(Iteration).
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          cryptoKeyId is a valid key id.
 *                targetCryptoKeyId is a valid key id.
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *  \trace        CREQ-248610
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA1(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Calc_PBKDF2_HMAC_SHA1()
 **********************************************************************************************************************/
/*! \brief         Derives a PBKDF2 HMAC SHA1 key.
 *  \details       Derives a key according to PBKDF2 HMAC SHA1 with the given CRYPTO_KE_KEYDERIVATION_PASSWORD(Secret),
 *                 CRYPTO_KE_KEYDERIVATION_SALT(Seed) and CRYPTO_KE_KEYDERIVATION_ITERATIONS(Iteration).
 *  \param[in,out] workspace               Holds the primitive workspace.
 *  \param[in]     secretLength            Holds the length of the secret.
 *  \param[in]     secretPtr               Holds the pointer to the secret with length of secretLength.
 *  \param[in]     infoLength              Holds the length of the info.
 *  \param[in]     infoPtr                 Holds the pointer to the info with length of infoLength.
 *  \param[in]     keyLength               Holds the length of the key.
 *  \param[in,out] keyPtr                  Holds the pointer to the key with length of keyLength.
 *  \param[in]     iterations              Holds the value to the iterations.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Calc_PBKDF2_HMAC_SHA1(
  P2VAR(eslt_WorkSpaceKDF2HMACSHA1, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 secretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) secretPtr,
  uint32 infoLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) infoPtr,
  uint32 keyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  uint32 iterations);
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA1_ENABLED == STD_ON) */

#if((CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA1_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA256_ENABLED == STD_ON))
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA_WS()
 **********************************************************************************************************************/
/*! \brief        Derives a PBKDF2 HMAC SHA1 or PBKDF2 HMAC SHA256 key.
 *  \details       Derives a key according to PBKDF2 HMAC SHA1 or PBKDF2 HMAC SHA256 with the given CRYPTO_KE_KEYDERIVATION_PASSWORD(Secret),
 *                 CRYPTO_KE_KEYDERIVATION_SALT(Seed) and CRYPTO_KE_KEYDERIVATION_ITERATIONS(Iteration).
 *  \param[in,out] workspace               Holds the primitive workspace.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]     targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[in]     algorithm               Holds the identifier of the algorithm which is used for the derived key.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId is a valid key id.
 *                 targetCryptoKeyId is a valid key id.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA_WS(
  P2VAR(eslt_WorkSpaceKDF2, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  uint8 algorithm);
#endif /* ( CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA1_ENABLED == STD_ON || CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA256_ENABLED == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA256_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA256()
 **********************************************************************************************************************/
/*! \brief        Derives a PBKDF2 HMAC SHA256 key.
 *  \details      Derives a key according to PBKDF2 HMAC SHA256 with the given CRYPTO_KE_KEYDERIVATION_PASSWORD(Secret),
 *                CRYPTO_KE_KEYDERIVATION_SALT(Seed) and CRYPTO_KE_KEYDERIVATION_ITERATIONS(Iteration).
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          cryptoKeyId is a valid key id.
 *                targetCryptoKeyId is a valid key id.
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *  \trace        CREQ-248610
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA256(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Calc_PBKDF2_HMAC_SHA256()
 **********************************************************************************************************************/
/*! \brief         Derives a PBKDF2 HMAC SHA256 key.
 *  \details       Derives a key according to PBKDF2 HMAC SHA256 with the given CRYPTO_KE_KEYDERIVATION_PASSWORD(Secret),
 *                 CRYPTO_KE_KEYDERIVATION_SALT(Seed) and CRYPTO_KE_KEYDERIVATION_ITERATIONS(Iteration).
 *  \param[in,out] workspace               Holds the primitive workspace.
 *  \param[in]     secretLength            Holds the length of the secret.
 *  \param[in]     secretPtr               Holds the pointer to the secret with length of secretLength.
 *  \param[in]     infoLength              Holds the length of the info.
 *  \param[in]     infoPtr                 Holds the pointer to the info with length of infoLength.
 *  \param[in]     keyLength               Holds the length of the key.
 *  \param[in,out] keyPtr                 Holds the pointer to the key with length of keyLength.
 *  \param[in]     iterations              Holds the value of the iterations.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Calc_PBKDF2_HMAC_SHA256(
  P2VAR(eslt_WorkSpaceKDF2HMACSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 secretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) secretPtr,
  uint32 infoLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) infoPtr,
  uint32 keyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  uint32 iterations);
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA256_ENABLED == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_HKDF_HMAC_WS()
 **********************************************************************************************************************/
/*! \brief        Derives a HKDF HMAC key.
 *  \details       Derives a key according to HKDF HMAC with the given CRYPTO_KE_KEYDERIVATION_PASSWORD(Secret),
 *                 CRYPTO_KE_KEYDERIVATION_SALT(Seed) and CRYPTO_KE_KEYDERIVATION_ITERATIONS(Iteration).
 *  \param[in,out] workspace               Holds the primitive workspace.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]     targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[in]     hashAlgorithmId         Holds the identifier of the hash algorithm to use for HMAC.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId is a valid key id.
 *                 targetCryptoKeyId is a valid key id.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_HKDF_HMAC_WS(
  P2VAR(eslt_WorkSpaceHKDFHMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  uint8 hashAlgorithmId);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_HKDF_HMAC()
 **********************************************************************************************************************/
/*! \brief        Derives a HKDF HMAC SHA256 key.
 *  \details      Derives a key according to HKDF HMAC with the given CRYPTO_KE_KEYDERIVATION_PASSWORD(Secret),
 *                CRYPTO_KE_KEYDERIVATION_SALT(Seed) and CRYPTO_KE_KEYDERIVATION_ITERATIONS(Iteration).
 *                By passing a suitable workspace to Crypto_30_LibCv_Local_KeyDerive_HKDF_HMAC_WS
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[in]    hashAlgorithmId         Holds the identifier of the hash algorithm to use for HMAC.
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          cryptoKeyId is a valid key id.
 *                targetCryptoKeyId is a valid key id.
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *  \trace        CREQ-266053
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_HKDF_HMAC(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  uint8 hashAlgorithmId);
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_ENABLED == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA256_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Calc_HKDF_HMAC_SHA256()
 **********************************************************************************************************************/
/*! \brief        Derives a HKDF HMAC SHA256 key.
 *  \details      Derives a key according to HKDF HMAC SHA256.
 *  \param[in,out] workspace               Holds the primitive workspace.
 *  \param[in]    secretLength            Holds the length of the secret.
 *  \param[in]    secretPtr               Holds the pointer to the secret with length of secretLength.
 *  \param[in]    saltLength              Holds the length of the salt.
 *  \param[in]    saltPtr                 Holds the pointer to the salt with length of saltLength.
 *  \param[in]    infoLength              Holds the length of the info.
 *  \param[in]    infoPtr                 Holds the pointer to the info with length of infoLength.
 *  \param[in]    keyLength               Holds the length of the key.
 *  \param[in,out] keyPtr                 Holds the pointer to the key with length of keyLength.
 *  \param[in]    iterations              Holds the value of the iterations.
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          All pointers need to be valid.
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Calc_HKDF_HMAC_SHA256(
  P2VAR(eslt_WorkSpaceHKDFHMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 secretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) secretPtr,
  uint32 saltLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) saltPtr,
  uint32 infoLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) infoPtr,
  uint32 keyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  uint8 iterations);
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA256_ENABLED == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_OPTION1_SHA256_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_HKDF_Hash_Option_1_WS()
 **********************************************************************************************************************/
/*! \brief        Derives a HKDF Hash Option 1 key.
 *  \details       Derives a key according to HKDF option 1 with the specified CRYPTO_KE_KEYDERIVATION_PASSWORD(Secret).
 *                 The hash function is specified using the CRYPTO_KE_KEYDERIVATION_ALGORITHM(Algorithm).
 *  \param[in,out] workspace               Holds the primitive workspace.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]     targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[in]     hashId                  Holds the identifier of the Hash.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId is a valid key id.
 *                 targetCryptoKeyId is a valid key id.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *  \trace         CREQ-304740
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_HKDF_Hash_Option_1_WS(
  P2VAR(eslt_WorkSpaceHKDFHASH, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  eslt_HashAlgorithm hashId);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_HKDF_Hash_Option_1()
 **********************************************************************************************************************/
/*! \brief        Derives a HKDF Hash Option 1 key.
 *  \details      Derives a key according to HKDF Hash Option 1 with the given CRYPTO_KE_KEYDERIVATION_PASSWORD(Secret)
 *                and optionally CRYPTO_KE_CUSTOM_KEYDERIVATION_ADDITIONAL_INFO (additional information).
 *                The hash function is specified using the CRYPTO_KE_KEYDERIVATION_ALGORITHM(Algorithm).
 *                By passing a suitable workspace to Crypto_30_LibCv_Local_KeyDerive_HKDF_Hash_Option_1_WS.
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[in]    hashId                  Holds the identifier of the hash.
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          cryptoKeyId is a valid key id.
 *                targetCryptoKeyId is a valid key id.
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_HKDF_Hash_Option_1(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  eslt_HashAlgorithm hashId);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Calc_HKDF_Hash_Option_1()
 **********************************************************************************************************************/
/*! \brief        Derives a HKDF Hash Option 1 key.
 *  \details      Derives a key according to HKDF Hash Option 1.
 *  \param[in,out] workspace              Holds the primitive workspace.
 *  \param[in]    secretLength            Holds the length of the secret.
 *  \param[in]    secretPtr               Holds the pointer to the secret with length of secretLength.
 *  \param[in]    infoLength              Holds the length of the info.
 *  \param[in]    infoPtr                 Holds the pointer to the info with length of infoLength.
 *  \param[in]    keyLength               Holds the length of the key.
 *  \param[in,out] keyPtr                 Holds the pointer to the key with length of keyLength.
 *  \param[in]    hashId                  Holds the value of the hash id.
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          All pointers need to be valid.
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Calc_HKDF_Hash_Option_1(
  P2VAR(eslt_WorkSpaceHKDFHASH, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 secretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) secretPtr,
  uint32 infoLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) infoPtr,
  uint32 keyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  eslt_HashAlgorithm hashId);
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_OPTION1_SHA256_ENABLED == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA384_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Calc_HKDF_HMAC_SHA384()
 **********************************************************************************************************************/
/*! \brief        Derives a HKDF HMAC SHA384 key.
 *  \details      Derives a key according to HKDF HMAC SHA384.
 *  \param[in,out] workspace               Holds the primitive workspace.
 *  \param[in]    secretLength            Holds the length of the secret.
 *  \param[in]    secretPtr               Holds the pointer to the secret with length of secretLength.
 *  \param[in]    saltLength              Holds the length of the salt.
 *  \param[in]    saltPtr                 Holds the pointer to the salt with length of saltLength.
 *  \param[in]    infoLength              Holds the length of the info.
 *  \param[in]    infoPtr                 Holds the pointer to the info with length of infoLength.
 *  \param[in]    keyLength               Holds the length of the key.
 *  \param[in,out] keyPtr                 Holds the pointer to the key with length of keyLength.
 *  \param[in]    iterations              Holds the value of the iterations.
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          All pointers need to be valid.
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Calc_HKDF_HMAC_SHA384(
  P2VAR(eslt_WorkSpaceHKDFHMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 secretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) secretPtr,
  uint32 saltLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) saltPtr,
  uint32 infoLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) infoPtr,
  uint32 keyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  uint8 iterations);
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA384_ENABLED == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_SPAKE2_PLUS_P256R1_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Spake2Plus_WS()
 **********************************************************************************************************************/
/*! \brief        Derives a Spake2+ key.
 *  \details       Derives a key according to Spake2+ preamble phase with the given Secret [w0s|w1s], the function derives
 *                 w0, w1 and l.
 *  \param[in,out] workspace               Holds the primitive workspace.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]     targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId is a valid key id.
 *                 targetCryptoKeyId is a valid key id.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Spake2Plus_WS(
  P2VAR(eslt_WorkSpaceSPAKE2PPreamble, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Spake2Plus()
 **********************************************************************************************************************/
/*! \brief        Derives a Spake2+ key.
 *  \details      Derives a key according to Spake2+ preamble phase with the given Secret [w0s|w1s], the function derives
 *                w0, w1 and l.
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          cryptoKeyId is a valid key id.
 *                targetCryptoKeyId is a valid key id.
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Spake2Plus(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Calc_Spake2Plus()
 **********************************************************************************************************************/
/*! \brief          Derives a Spake2+ key.
 *  \details        Derives a key according to Spake2+ with the given Secret [w0s|w1s], the function derives
 *                  w0, w1 and l.
 *  \param[in,out]  workspace             Holds the primitive workspace.
 *  \param[in]      domainPtr             Pointer to domain parameter structure
 *  \param[in]      domainExtPtr          Pointer to domain parameter extension structure
 *  \param[in]      secretPtr             Holds the pointer to the secret [w0s|w1s] with length of secretLength.
 *  \param[in]      secretLength          Holds the length of the secret.
 *  \param[in,out]  w0Ptr                 Holds the pointer to the value w0 with length of w0Length.
 *  \param[in]      w0Length              Holds the length of the w0.
 *  \param[in,out]  w1Ptr                 Holds the pointer to the value w1 with length of w1Length.
 *  \param[in]      w1Length              Holds the length of the w1.
 *  \param[in,out]  lPtr                  Holds the pointer to the point l with length of lLength.
 *  \param[in]      lLength               Holds the length of the l.
 *  \return         E_OK                  Request successful.
 *                  E_NOT_OK              Request failed.
 *  \pre            All pointers need to be valid.
 *  \context        TASK
 *  \reentrant      TRUE, for different crypto keys
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Calc_Spake2Plus(
  P2VAR(eslt_WorkSpaceSPAKE2PPreamble, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainExtPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) secretPtr,
  uint32 secretLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) w0Ptr,
  uint32 w0Length,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) w1Ptr,
  uint32 w1Length,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) lPtr,
  uint32 lLength);
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA256_ENABLED == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA256_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_KDF_963_256_Calc()
 **********************************************************************************************************************/
/*! \brief         Derives a KDF X9.63 key with Sha-256.
 *  \details       Derives a key according to KDF X9.63 Sha-256 with the given secret and
 *                 optional seed.
 *  \param[in,out] workspace               Holds the primitive workspace.
 *  \param[in]     secretLength            Holds the length of the secret.
 *  \param[in]     secretPtr               Holds the pointer to the secret with length of secretLength.
 *  \param[in]     infoLength              Holds the length of the info.
 *  \param[in]     infoPtr                 Holds the pointer to the info with length of infoLength.
 *  \param[in]     keyLength               Holds the length of the key.
 *  \param[in,out] keyPtr                  Holds the pointer to the key with length of keyLength.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE, for different keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_KDF_963_256_Calc(
  P2VAR(eslt_WorkSpaceKDFX963SHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 secretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) secretPtr,
  uint32 infoLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) infoPtr,
  uint32 keyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) keyPtr);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_KDF_963_256()
 **********************************************************************************************************************/
/*! \brief        Derives a KDF X9.63 key with Sha-256.
 *  \details      Derives a key according to KDF X9.63 Sha-256 with the given CRYPTO_KE_KEYDERIVATION_PASSWORD(Secret) and
 *                optional CRYPTO_KE_KEYDERIVATION_SALT(Seed).
 *                By passing a suitable workspace to Crypto_30_LibCv_Local_KeyDerive_KDF_963_256_WS
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          cryptoKeyId is a valid key id.
 *                targetCryptoKeyId is a valid key id.
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *  \trace        CREQ-191710
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_KDF_963_256(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_KDF_963_256_WS()
 **********************************************************************************************************************/
/*! \brief         Derives a KDF X9.63 key with Sha-256.
 *  \details       Derives a key according to KDF X9.63 Sha-256 with the given CRYPTO_KE_KEYDERIVATION_PASSWORD(Secret) and
 *                 optional CRYPTO_KE_KEYDERIVATION_SALT(Seed).
 *  \param[in,out] workspace               Holds the primitive workspace.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]     targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_KDF_963_256_WS(
  P2VAR(eslt_WorkSpaceKDFX963SHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA256_ENABLED == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA512_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_KDF_963_512_Calc()
 **********************************************************************************************************************/
/*! \brief         Derives a KDF X9.63 key with Sha-512.
 *  \details       Derives a key according tp KDF X9.63 Sha-512 with the given secret and
 *                 optional seed.
 *  \param[in,out] workspace               Holds the primitive workspace.
 *  \param[in]     secretLength            Holds the length of the secret.
 *  \param[in]     secretPtr               Holds the pointer to the secret with length of secretLength.
 *  \param[in]     infoLength              Holds the length of the info.
 *  \param[in]     infoPtr                 Holds the pointer to the info with length of infoLength.
 *  \param[in]     keyLength               Holds the length of the key.
 *  \param[in,out] keyPtr                  Holds the pointer to the key with length of keyLength.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE, for different keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_KDF_963_512_Calc(
  P2VAR(eslt_WorkSpaceKDFX963SHA512, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 secretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) secretPtr,
  uint32 infoLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) infoPtr,
  uint32 keyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) keyPtr);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_KDF_963_512()
 **********************************************************************************************************************/
/*! \brief        Derives a KDF X9.63 key with Sha-512.
 *  \details      Derives a key according tp KDF X9.63 Sha-512 with the given CRYPTO_KE_KEYDERIVATION_PASSWORD(Secret) and
 *                optional CRYPTO_KE_KEYDERIVATION_SALT(Seed).
 *                By passing a suitable workspace to Crypto_30_LibCv_Local_KeyDerive_KDF_963_512_WS
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          cryptoKeyId is a valid key id.
 *                targetCryptoKeyId is a valid key id.
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *  \trace        CREQ-191710
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_KDF_963_512(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_KDF_963_512_WS()
 **********************************************************************************************************************/
/*! \brief         Derives a KDF X9.63 key with Sha-512.
 *  \details       Derives a key according to KDF X9.63 Sha-512 with the given CRYPTO_KE_KEYDERIVATION_PASSWORD(Secret) and
 *                 optional CRYPTO_KE_KEYDERIVATION_SALT(Seed).
 *  \param[in,out] workspace               Holds the primitive workspace.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]     targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_KDF_963_512_WS(
  P2VAR(eslt_WorkSpaceKDFX963SHA512, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA512_ENABLED == STD_ON) */

#if ((CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA256_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA512_ENABLED == STD_ON))
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_KDF_963_Get_Secret_And_Salt()
 **********************************************************************************************************************/
/*! \brief         Gets the secret and salt index and checks the target key.
 *  \details       Gets the index for secret and salt and checks the target key for validity.
 *  \param[in,out] secretLength        Holds the length of the secret.
 *  \param[in,out] secretIndex         Holds the secret index.
 *  \param[in,out] saltLength          Holds the length of the salt.
 *  \param[in,out] saltIndex           Holds the salt index.
 *  \param[in,out] keyLength           Holds the length of the key.
 *  \param[in,out] keyElementIndex     Holds the key element index.
 *  \param[in]     cryptoKeyId         Holds the identifier of the key which is used for key derivation.
 *  \param[in]     targetCryptoKeyId   Holds the identifier of the key which is used to store the derived key.
 *  \return        E_OK                Request successful.
 *                 E_NOT_OK            Request failed.
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_KDF_963_Get_Secret_And_Salt(
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) secretLength,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) secretIndex,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) saltLength,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) saltIndex,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) keyLength,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) keyElementIndex,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);
#endif /* ((CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA256_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA512_ENABLED == STD_ON)) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if ((CRYPTO_30_LIBCV_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON))
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt_TargetKeyLength()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt_TargetKeyLength(
  uint32 targetCryptoKeyId,
  uint8 deriveAlgorithm,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) saltBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) targetKeyLength)
{
  Std_ReturnType retVal = E_OK;

  uint32 configuredTargetKeyLength = 0u;

  /* #1 Get configured target key length */
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex;
  if (Crypto_30_LibCv_Local_KeyElementSearch(targetCryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, &elementIndex) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  {
    /* Get Configured Target Key Length */
    configuredTargetKeyLength = Crypto_30_LibCv_GetKeyElementLength(elementIndex);
  }
  else
  {
    retVal = E_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  }

  /* #2 Compare the configured length against the target key length given in salt */
  /* The keyLength (k) in salt is given in bytes and Big Endian Format. This has to be converted to size in bits and little endian format later */
  (*targetKeyLength) = (((((uint32)(saltBuffer[4])) << 8) | ((uint32)(saltBuffer[5])))); /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */

  /* Check valid target key length */
  if ((*targetKeyLength > CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_TARGET_KEY) ||
    ((*targetKeyLength) > configuredTargetKeyLength))
  {
    retVal = E_NOT_OK;
  }

# if (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
#  if (CRYPTO_30_LIBCV_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON)
  /* #3 For Asym keys check that length is supported by used prime numbers */
  if (deriveAlgorithm == CRYPTO_30_LIBCV_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB)
#  else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(deriveAlgorithm); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif
  {
    if ((*targetKeyLength) != CRYPTO_30_LIBCV_SIZEOF_KDF_PRIME)
    {
      retVal = E_NOT_OK;
    }

    /* #4 For Asymmetric key derivation target key has to be extended by 8 bytes, because it is divided by prime-1 in the second step */
    (*targetKeyLength) += CRYPTO_30_LIBCV_SIZEOF_KDF_PRIME_ASYM_EXTEND; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */
  }
# else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(deriveAlgorithm); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6060 8 */ /* MD_CRYPTO_30_LIBCV_STPAR */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  uint8 deriveAlgorithm,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) prfInputBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) prfInputBufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) saltBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) targetKeyLength)
{
  Std_ReturnType retVal;

  Crypto_30_LibCv_SizeOfKeyStorageType saltIndex;
  uint32 saltLength;

  /* #1 Load Salt / Context from the salt key element */
  /* 4 byte Context | 2 byte target Key Length in Big Endian (k) | 32 byte curve specific Prime-1 */
  /* For Asym keys the salt contains also the 32 byte prime-1 */
  retVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_SALT, &saltIndex, &saltLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  /* #2 Check expected salt length according to the algorithm */
  if (retVal == E_OK)
  {
# if (CRYPTO_30_LIBCV_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON)
#  if (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
    if (deriveAlgorithm == CRYPTO_30_LIBCV_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256)
#  endif
    {
      if (saltLength != CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_SYMMETRIC)
      {
        retVal = E_NOT_OK;
      }
    }
#  if (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
    else
#  endif
# endif
# if (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
    {
      /* CRYPTO_30_LIBCV_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB */
      if (saltLength != CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_ASYMMETRIC)
      {
        retVal = E_NOT_OK;
      }
    }
# endif
  }

  /* #4 Copy the 4 byte Context into the PRF input buffer */
  if (retVal == E_OK)
  {
    Crypto_30_LibCv_CopyData(saltBuffer, Crypto_30_LibCv_GetAddrKeyStorage(saltIndex), saltLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
    Crypto_30_LibCv_CopyData(&(prfInputBuffer[*prfInputBufferLength]), saltBuffer, CRYPTO_30_LIBCV_SIZEOF_KDF_CONTEXT); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
    (*prfInputBufferLength) += CRYPTO_30_LIBCV_SIZEOF_KDF_CONTEXT; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */

    retVal = Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt_TargetKeyLength(targetCryptoKeyId, deriveAlgorithm, saltBuffer, targetKeyLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  }
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_Kdf_HandleInputParams()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060 9 */ /* MD_CRYPTO_30_LIBCV_STPAR */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_HandleInputParams(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  uint8 deriveAlgorithm,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) prfInputBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) prfInputBufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) saltBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) targetKeyLength,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) parentKeyLength)
{
  Std_ReturnType retVal;
  Crypto_30_LibCv_SizeOfKeyStorageType passwordIndex = 0u; /* init with valid value */
  Crypto_30_LibCv_SizeOfKeyStorageType labelIndex = 0u; /* init with valid value */
  uint32 labelLength = CRYPTO_30_LIBCV_SIZEOF_KDF_LABEL_LENGTH;

  (*parentKeyLength) = CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_PARENT_KEY; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */

  /* #2 Get Parent Key from the password key element and write it to the PRF input buffer */

  retVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, &passwordIndex, parentKeyLength, CRYPTO_30_LIBCV_LENGTH_CHECK_MAX); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (retVal == E_OK)
  {
    Crypto_30_LibCv_CopyData(prfInputBuffer, Crypto_30_LibCv_GetAddrKeyStorage(passwordIndex), *parentKeyLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    (*prfInputBufferLength) += *parentKeyLength; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */

    /* #3 Get Label from the Label key element and write it to the PRF input buffer */
    /* Skip one byte for iteration counter */
    (*prfInputBufferLength)++; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */

    retVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_KEYDERIVATION_LABEL, &labelIndex, &labelLength, CRYPTO_30_LIBCV_LENGTH_CHECK_MAX); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  }

  if (retVal == E_OK)
  {
    Crypto_30_LibCv_CopyData(&(prfInputBuffer[*prfInputBufferLength]), Crypto_30_LibCv_GetAddrKeyStorage(labelIndex), labelLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    (*prfInputBufferLength) += labelLength; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */

    /* #4 Add constant byte 0x00 to the PRF input buffer */
    prfInputBuffer[*prfInputBufferLength] = 0x00u; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
    (*prfInputBufferLength)++; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */

    /* #5 Get and check the Salt element */
    retVal = Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt(cryptoKeyId, targetCryptoKeyId, deriveAlgorithm, prfInputBuffer, prfInputBufferLength, saltBuffer, targetKeyLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  }

  /* #6 Convert target key length to bits and little endian and copy it to the input buffer */
  if (retVal == E_OK)
  {
    /* Set k in bits and little endian (lo byte first) */
    prfInputBuffer[*prfInputBufferLength] = (uint8)((uint16)((*targetKeyLength << 3u)) & 0x00FFu); /* lo byte */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
    prfInputBuffer[*prfInputBufferLength + 1u] = (uint8)((uint16)((*targetKeyLength) >> 5u) & 0x00FFu); /* hi byte */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */ /* PRQA S 2985 */ /* MD_CRYPTO_30_LIBCV_2985 */
    (*prfInputBufferLength) += CRYPTO_30_LIBCV_SIZEOF_KDF_TARGET_KEY_LENGTH; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_Kdf_CallPRF()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 7 */ /* MD_CRYPTO_30_LIBCV_STPAR, MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_CallPRF(
  P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) prfInputBuffer,
  uint32 prfInputBufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) targetKey,
  uint32 targetKeyLength,
  uint32 parentKeyLength)
{
  Std_ReturnType retVal = E_OK;
  uint32_least derivedLength;
  uint8 i = 1u;

  /* #1 Iterate over the target key length */
  for (derivedLength = 0u; derivedLength < targetKeyLength; derivedLength += ESL_SIZEOF_SHA256_DIGEST) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_PARAMETER_DEPENDENT_UPPER_BOUND_WITH_CHECK */
  {
    uint8 tempHashBuf[ESL_SIZEOF_SHA256_DIGEST] = { 0u };

    /* #11 Set the iteration counter i in the PRF input buffer. Inserted after parent key */
    prfInputBuffer[parentKeyLength] = i; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
    i++;

    /* #12 Call SHA-256 as PRF */
    retVal = E_NOT_OK;
    /* CRYPTO_OPERATIONMODE_START: */
    if (esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workspace->header, ESL_MAXSIZEOF_WS_SHA256, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */
    {
      if (esl_initSHA256((P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
      {
        /* CRYPTO_OPERATIONMODE_UPDATE: */
        if (esl_updateSHA256((P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, (eslt_Length)prfInputBufferLength, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))prfInputBuffer) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_DATA_PTR */
        {
          /* CRYPTO_OPERATIONMODE_FINISH: */
          if (esl_finalizeSHA256((P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))tempHashBuf) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS */
          {
            retVal = E_OK;
          }
        }
      }
    }

    /* #13 Copy the PRF result to the end of the target key buffer */
    if (retVal == E_OK)
    {
      if ((derivedLength + ESL_SIZEOF_SHA256_DIGEST) <= targetKeyLength)
      {
        Crypto_30_LibCv_CopyData(&(targetKey[derivedLength]), tempHashBuf, ESL_SIZEOF_SHA256_DIGEST); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
      }
      else
      {
        Crypto_30_LibCv_CopyData(&(targetKey[derivedLength]), tempHashBuf, (uint16)(targetKeyLength - derivedLength)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
      }
    }
    else
    {
      break;
    }
    /* END Calculate Hash */
  }
  return retVal;
}

# if (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_Kdf_AsymDivideByPrimeMinusOne()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_AsymDivideByPrimeMinusOne(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) targetKey,
  uint32 targetKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) primeMinus1Ptr,
  uint32 primeMinus1Length)
{
  Std_ReturnType retVal = E_OK;
  /* #1 Use the library function actBNReduce to divide big numbers */
  /*  r = a mod b
  **
  **  int actBNReduce(actBNDIGIT* a,
  **                  actBNLENGTH a_length,
  **                  actBNDIGIT* b,
  **                  actBNLENGTH b_length,
  **                  actBNDIGIT* r,
  **                  void (*watchdog) (void));
  **
  **   BigNum reduce (r = a mod b).
  **
  ** input:                                              (length in digits)
  ** - a:       first number (will be destroyed)              a_length+1
  ** - b:       second number (will be returned const         b_length
  **            but touched by the normalization)
  ** - r:       result buffer                                 b_length+1
  **
  ** output:
  ** - a:       the normalized result a mod b                 b_length
  ** - b:       second number                                 b_length
  ** - r:       the result a mod b                            b_length
  ** - returns: -1    if exact b_length <= 1 (error)
  **             0    else (success) */

#  if (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 4)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
  actBNDIGIT a[264u >> 2]; /* 256 + 8 */
  actBNLENGTH a_length = (actBNLENGTH)(targetKeyLength >> 2); /*  PRQA S 4394 */  /* MD_CRYPTO_30_LIBCV_4394 */
  actBNDIGIT b[256u >> 2];
  actBNLENGTH b_length = (actBNLENGTH)(primeMinus1Length >> 2); /*  PRQA S 4394 */  /* MD_CRYPTO_30_LIBCV_4394 */
  actBNDIGIT r[264u >> 2];
#  else
#   error "Value of CRYPTO_30_LIBCV_BYTES_PER_DIGIT is not supported"
#  endif

  uint8 one = 1u;
  uint32 res; /* PRQA S 5013 */ /* MD_CRYPTO_30_LIBCV_5013 */

  /* #11 Set the so far calculated target key as dividend */
  actBNSetOctetString(a, a_length, targetKey, (uint32)targetKeyLength); /* PRQA S 5013 */ /* MD_CRYPTO_30_LIBCV_5013 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */

  /* #12 Set the prime-1 as divisor */
  actBNSetOctetString(b, b_length, primeMinus1Ptr, (uint32)primeMinus1Length); /* PRQA S 5013 */ /* MD_CRYPTO_30_LIBCV_5013 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */

  /* #13 Call actBNReduce and store the value in targetKey */
  res = actBNReduce(a, a_length, b, b_length, r, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  if (res == (uint32)0)
  {
    /* #2 Add one and return the result */
    actBNSetOctetString(a, a_length, &one, (uint32)1); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    (void)actBNAdd(r, a, r, a_length); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    actBNOctetString(targetKey, (uint32)targetKeyLength, r, a_length); /* PRQA S 5013 */ /* MD_CRYPTO_30_LIBCV_5013 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
  }
  else
  {
    retVal = E_NOT_OK;
  }
  return retVal;
}
# endif /* (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON) */

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Nist800108NistFips1864_WS()
 **********************************************************************************************************************/
/*!
 *
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Nist800108NistFips1864_WS(
  P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  uint8 deriveAlgorithm)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  uint32 targetKeyLength;

  uint32 parentKeyLength;
  uint32 prfInputBufferLength = 0u;
# if (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
  uint16 primeMinus1Length;
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) primeMinus1Ptr; /* PRQA S 3678 */ /* MD_CRYPTO_30_LIBCV_3678 */
# endif

  /* Input buffer for the Pseudo Random Function. */
  /* Buffer size is 256 byte maximum parent key length + 12 byte (1 byte iteration | 4 byte Label | 1 byte 0x00 | 6 byte of salt (2 byte TypeID | 2 byte training Counter | 2 byte target Key Length in littel endian (k))) */
  uint8 prfInputBuffer[CRYPTO_30_LIBCV_SIZEOF_KDF_PRF_INPUT_BUFFER];
  uint8 saltBuffer[CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_ASYMMETRIC];
  uint8 targetKey[CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_TARGET_KEY_PLUS_8]; /* 256+8 for asym KDF */

  /* ----- Implementation ------------------------------------------------- */
  Crypto_30_LibCv_ClearData(targetKey, CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_TARGET_KEY_PLUS_8); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  /* #1 Handle input parameter */
  retVal = Crypto_30_LibCv_Local_Kdf_HandleInputParams(cryptoKeyId, targetCryptoKeyId, deriveAlgorithm, prfInputBuffer, &prfInputBufferLength, saltBuffer, &targetKeyLength, &parentKeyLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  /* #5 Call PRF */
  if (retVal == E_OK)
  {
    retVal = Crypto_30_LibCv_Local_Kdf_CallPRF(workspace, prfInputBuffer, prfInputBufferLength, targetKey, targetKeyLength, parentKeyLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  }

  /* #10 Handle the results of the PRF according to the algorithm */
  if (retVal == E_OK)
  {
# if (CRYPTO_30_LIBCV_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON)
#  if (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
    if (deriveAlgorithm == CRYPTO_30_LIBCV_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256)
#  endif
    {
      /* #15 Symmetric Key Derive finished here. Copy calculated key to key element */
      retVal = Crypto_30_LibCv_Local_KeyElementSet(targetCryptoKeyId, CRYPTO_30_LIBCV_KE_TARGET_KEY, targetKey, targetKeyLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    }
#  if (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
    else
#  endif
# endif
# if (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
    {
      /* CRYPTO_30_LIBCV_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB */
      /* #20 For Asymmetric Key Derive: */

      /* #25 Set the prime minus-1 pointer to the start address of the value within the salt buffer and set the data length */
      primeMinus1Ptr = &(saltBuffer[CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_SYMMETRIC]);
      primeMinus1Length = CRYPTO_30_LIBCV_SIZEOF_KDF_PRIME;

      /* #30 Calculate private key for asymmetric keys only by dividing key by given prime-1 */
      retVal = Crypto_30_LibCv_Local_Kdf_AsymDivideByPrimeMinusOne(targetKey, targetKeyLength, primeMinus1Ptr, primeMinus1Length); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

      if (retVal == E_OK)
      {
        /* Skip the first 8 additional bytes */
        /* #35 Asymmetric Key Derive finished. Copy calculated key to key element */
        retVal = Crypto_30_LibCv_Local_KeyElementSet(targetCryptoKeyId, CRYPTO_30_LIBCV_KE_TARGET_KEY, &(targetKey[8]), targetKeyLength - 8u); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
      }
    }
# endif
  }
  Crypto_30_LibCv_ClearData(targetKey, CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_TARGET_KEY_PLUS_8); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  /* #40 If key was derived successfully, mark it as valid */
  if (retVal != E_OK)
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Nist800108NistFips1864()
 **********************************************************************************************************************/
/*!
 *
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Nist800108NistFips1864(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  uint8 deriveAlgorithm)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  eslt_WorkSpaceSHA256 workspace;
  /* ----- Implementation ------------------------------------------------- */
  retVal = Crypto_30_LibCv_Local_KeyDerive_Nist800108NistFips1864_WS(&workspace, cryptoKeyId, targetCryptoKeyId, deriveAlgorithm);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
  return retVal;
}
#endif /* ((CRYPTO_30_LIBCV_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)) */

#if ((CRYPTO_30_LIBCV_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON) \
    || (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON))
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Nist80056A_SingleStepKdfHash()
 **********************************************************************************************************************/
/*!
 *
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
/* PRQA S 6060, 6080 8 */ /* MD_CRYPTO_30_LIBCV_STPAR, MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Nist80056A_SingleStepKdfHash(
  P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) sharedSecretPtr,
  uint32 sharedSecretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) otherInfoPtr,
  uint32 otherInfoLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) derivedKeyPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) derivedKeyLengthPtr)
{
  /* This check is not required while the sharedSecret size and hash digest size are equal but is required by IS15118 (marked with \fr) */
  /* \fr uint32 repeatSteps; */
  /* \fr uint32 counter; */
  uint32 derivedLength = 0u;
  uint8 concatenateBuf[CRYPTO_30_LIBCV_NIST80056A_COUNTER_SIZE + CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE + CRYPTO_30_LIBCV_NIST80056A_OTHER_INFO_MAXSIZE];
  Std_ReturnType retVal = E_NOT_OK;
  /* #1 init workspace */
  if (esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workspace->header, ESL_MAXSIZEOF_WS_SHA256, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */
  {
    /* Single-step KDF Option 1 Hash */
    /* Implementation according to NIST SP 56A: Revision 2, May 2013 (5.8.1.1  The Single-Step KDF Specification) */

    /* #5 (Spec-Step: 1) calculate repeat Steps. */
    /* repeat Steps = KeyLength(bit)/HashLength(bit) */
    /* This check is not required while the sharedSecret size and hash digest size are equal */
    /* \fr repeatSteps = Crypto_30_LibCv_Byte2Bit(sharedSecretLength) / Crypto_30_LibCv_Byte2Bit(ESL_SIZEOF_SHA256_DIGEST); */

    /* #10 (Spec-Step: 2) check max repeat Steps. */
    /* This check is not required while the sharedSecret size and hash digest size are equal */
    /* \fr if ((repeatSteps * ESL_SIZEOF_SHA256_DIGEST) <= *derivedKeyLengthPtr) */
    {

      /* #12 add shared secret to string. */
      /* (counter || Z || OtherInfo) */
      Crypto_30_LibCv_CopyData(&concatenateBuf[CRYPTO_30_LIBCV_NIST80056A_COUNTER_SIZE], sharedSecretPtr, sharedSecretLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */

      /* #15 add otherInformation to string. */
      /* OtherInfo = (AlgorithmID || PartyUInfo(Transmitter) || PartyVInfo(Receiver)) */
      Crypto_30_LibCv_CopyData(&concatenateBuf[CRYPTO_30_LIBCV_NIST80056A_COUNTER_SIZE + sharedSecretLength], otherInfoPtr, otherInfoLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */

      /* #20 (Spec-Step: 4) check max input length hash */
      /* not required */

      /* #25 (Spec-Step: 3) init Counter */
      /* create the concatenated shared secret (counter || Z || OtherInfo) */
      /* Initialize a 32-bit counter as 1 (big-endian) */
      /* #30 (Spec-Step: 5 + 5.2) process repeat steps + increment counter */
      /* This check is not required while the sharedSecret size and hash digest size are equal */
      /* \fr for (counter = 1u; counter <= repeatSteps; counter++) */
      {
        /* retVal = E_NOT_OK; */

        /* #35 copy counter to string. */
        /* \fr Crypto_30_LibCv_Local_Uint32ToUint8ArrayBigEndian(concatenateBuf, counter);  SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
        Crypto_30_LibCv_Local_Uint32ToUint8ArrayBigEndian(concatenateBuf, (uint32)1u); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

        /* #40 (Spec-Step: 5.1) Hash(counter || Z || OtherInfo) */
        if (esl_initSHA256((P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE */
        {
          if (esl_updateSHA256((P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, (eslt_Length)(CRYPTO_30_LIBCV_NIST80056A_COUNTER_SIZE + sharedSecretLength + otherInfoLength), (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))concatenateBuf) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          {
            if (esl_finalizeSHA256((P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&derivedKeyPtr[derivedLength]) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
            {
              derivedLength += ESL_SIZEOF_SHA256_DIGEST;
              retVal = E_OK;
            }
          }
        }
        /* This check is not required while the sharedSecret size and hash digest size are equal */
        /* \fr if (retVal != E_OK)
        {
        break;
        } */
      }
    }
  }

  /* #45 Set derived key length. */
  if (retVal == E_OK)
  {
    *derivedKeyLengthPtr = derivedLength; /* SBSW_CRYPTO_30_LIBCV_VARIABLE_ACCESS_PTR_FORM_STACK */
  }

  /* #50 clear buffer */
  Crypto_30_LibCv_ClearData(concatenateBuf, CRYPTO_30_LIBCV_NIST80056A_COUNTER_SIZE + CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE + CRYPTO_30_LIBCV_NIST80056A_OTHER_INFO_MAXSIZE); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 10 */ /* MD_CRYPTO_30_LIBCV_STPAR */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S(
  P2VAR(Crypto_30_LibCv_WorkSpaceISO15118, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) privateKeyPtr,
  uint32 privateKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) partnerPubKeyPtr,
  uint32 partnerPubKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) otherInfoPtr,
  uint32 otherInfoPtrLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) derivedKeyPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) derivedKeyLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 sharedSecret[CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PRIVATE * 2u];
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if (Crypto_30_LibCv_Local_Dispatch_EcP_CalculateSharedSecret_With_Ws(
    privateKeyPtr, privateKeyLength,
    partnerPubKeyPtr, partnerPubKeyLength,
    sharedSecret, CRYPTO_30_LIBCV_ECDHE_256_ID,
    &workspace->wsEcP) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
  {
    /* #1  Single-step KDF Option 1 Hash */
    /* use only shared secret X for concatenation */
    retVal = Crypto_30_LibCv_Local_KeyDerive_Nist80056A_SingleStepKdfHash(
      &workspace->wsSHA256,
      sharedSecret, CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PRIVATE,
      otherInfoPtr, otherInfoPtrLength,
      derivedKeyPtr, derivedKeyLengthPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
  }

  /* #5 destroy all working copies. */
  Crypto_30_LibCv_ClearData(sharedSecret, CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PRIVATE * 2u); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  return retVal;
}
#endif /* ((CRYPTO_30_LIBCV_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S_With_Ws_AndLoadKey()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 4 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S_With_Ws_AndLoadKey(
  P2VAR(Crypto_30_LibCv_WorkSpaceISO15118, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */

  uint32 partnerPubKeyLength = CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE * 2u;
  uint32 privKeyLength = CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE;
  uint32 otherInfoLength = CRYPTO_30_LIBCV_NIST80056A_OTHER_INFO_MAXSIZE;
  uint32 derivedKeyBufLength = CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE;

  Crypto_30_LibCv_SizeOfKeyStorageType partnerPubKeyIndex;
  Crypto_30_LibCv_SizeOfKeyStorageType privKeyIndex;
  Crypto_30_LibCv_SizeOfKeyStorageType otherInfoIndex;
  uint8 derivedKeyBuf[CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE];
  Std_ReturnType retVal = E_NOT_OK;
  Std_ReturnType localRet;

  /* ----- Implementation ----------------------------------------------- */
  Crypto_30_LibCv_ClearData(derivedKeyBuf, CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  /* #1 read other info from key element. */
  localRet = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_KEYDERIVATION_ADDITIONAL_INFO, &otherInfoIndex, &otherInfoLength, CRYPTO_30_LIBCV_LENGTH_CHECK_MAX); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  localRet |= Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_KEYEXCHANGE_PARTNER_PUB_KEY, &partnerPubKeyIndex, &partnerPubKeyLength, CRYPTO_30_LIBCV_LENGTH_CHECK_MAX); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  localRet |= Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, &privKeyIndex, &privKeyLength, CRYPTO_30_LIBCV_LENGTH_CHECK_MAX); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (localRet == E_OK)
  {
    /* #5 calculate NIST 800 56A */
    if (Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S(workspace,
                                                                 Crypto_30_LibCv_GetAddrKeyStorage(privKeyIndex), privKeyLength,
                                                                 Crypto_30_LibCv_GetAddrKeyStorage(partnerPubKeyIndex), partnerPubKeyLength,
                                                                 Crypto_30_LibCv_GetAddrKeyStorage(otherInfoIndex), otherInfoLength,
                                                                 derivedKeyBuf, &derivedKeyBufLength) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    {
      if (Crypto_30_LibCv_Local_KeyElementSet(targetCryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, derivedKeyBuf, derivedKeyBufLength) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
      {
        retVal = E_OK;
      }
    }
  }

  /* #10 destroy all working copies. */
  Crypto_30_LibCv_ClearData(derivedKeyBuf, derivedKeyBufLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S_AndLoadKey()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S_AndLoadKey(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval;
  Crypto_30_LibCv_WorkSpaceISO15118 workspace; /* PRQA S 0759 */ /* MD_MSR_Union */

  /* ----- Implementation ----------------------------------------------- */
  retval = Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S_With_Ws_AndLoadKey(&workspace, cryptoKeyId, targetCryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
  return retval;
}
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_ISO15118_CheckKey()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_ISO15118_CheckKey(
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) privateKeyPtr,
  uint32 privateKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) pubKeyPtr,
  uint32 pubKeyLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 sharedSecret[CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PRIVATE * 2u];
  Std_ReturnType retVal = E_NOT_OK;

  /* NISTp256r1 = ANSIp256r1 = SECp256r1 */
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_CONST) basePointOrder = &Crypto_30_LibCv_EccCurveNistAnsiSecP256R1Domain[192];

  /* ----- Implementation ----------------------------------------------- */
  /* #1 check if the private key is smaller than the order of the curve base point */

  if (Crypto_30_LibCv_Local_CompareData_IsSmaller(privateKeyPtr, basePointOrder, privateKeyLength) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
  {

    /* #5 verify key pair with esLib */
    if (Crypto_30_LibCv_Local_Dispatch_EcP_CalculateSharedSecret_With_Ws(privateKeyPtr, privateKeyLength, pubKeyPtr, pubKeyLength, sharedSecret, CRYPTO_30_LIBCV_ECDHE_256_ID, workspace) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
    {
      retVal = E_OK;
    }
  }

  /* #10 destroy all working copies. */
  Crypto_30_LibCv_ClearData(sharedSecret, CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PRIVATE * 2u); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_ISO15118_Aes128Decrypt()
 **********************************************************************************************************************/
/*!
 *
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
/* PRQA S 6060 8 */ /* MD_CRYPTO_30_LIBCV_STPAR_VSECPRIM */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_ISO15118_Aes128Decrypt(
  P2VAR(eslt_WorkSpaceAES128, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) ivPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) encryptedMessagePtr,
  uint32 encryptedMessageLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) plainMessagePtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) plainMessageLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;

  eslt_Length outputDataLen;
  uint32 totalOutputDataLen = 0u;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* #0 initialize the AES workspaces */
  if (esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workspace->header, ESL_MAXSIZEOF_WS_AES128, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */
  {

    /* #5 initialize the decryption */
    eslRet = esl_initDecryptAES128((P2VAR(eslt_WorkSpaceAES128, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))keyPtr, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE */
                                   ESL_BM_CBC, ESL_PM_OFF, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))ivPtr);

    if (eslRet == ESL_ERC_NO_ERROR)
    {
      outputDataLen = (eslt_Length)*plainMessageLengthPtr;
      /* #10 decrypt the actual private key */
      eslRet = esl_decryptAES128((P2VAR(eslt_WorkSpaceAES128, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace,
        (eslt_Length)encryptedMessageLength, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))encryptedMessagePtr,
                                 (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputDataLen, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))plainMessagePtr); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */  /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */

      if (eslRet == ESL_ERC_NO_ERROR)
      {
        /* #15 save the decrypted length */
        totalOutputDataLen = outputDataLen;
        /* reset the OutputDataLen */
        outputDataLen = (eslt_Length)(*plainMessageLengthPtr - totalOutputDataLen);

        /* #20 add padding to get least block */
        eslRet = esl_decryptAES128((P2VAR(eslt_WorkSpaceAES128, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace,  /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          (eslt_Length)1u, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))encryptedMessagePtr,
                                   (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputDataLen, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&plainMessagePtr[totalOutputDataLen]); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */

        if (eslRet == ESL_ERC_NO_ERROR)
        {
          /* #25 update the decrypted length */
          totalOutputDataLen += outputDataLen;
        }
      }
    }
    if (eslRet == ESL_ERC_NO_ERROR)
    {
      /* #30 provide the total output data len to the application */
      *plainMessageLengthPtr = totalOutputDataLen; /* SBSW_CRYPTO_30_LIBCV_VARIABLE_ACCESS_PTR_FORM_STACK */
      retVal = E_OK;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_ISO15118_WS()
 **********************************************************************************************************************/
/*!
 *
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
/* PRQA S 6080 4 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_ISO15118_WS(
  P2VAR(Crypto_30_LibCv_WorkSpaceISO15118, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 encryptedIvAndPrivKeyLength = CRYPTO_30_LIBCV_ISO15118_SIZEOF_IV_AND_PRIV_KEY;

  uint32 privKeyLength = CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE;
  uint32 partnerPubKeyLength = CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE * 2u;

  uint32 derivedKeyLength = CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE;

  uint32 newPrivateKeyLength = CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE;
  uint32 newPubKeyLength = CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE * 2u;

  Crypto_30_LibCv_SizeOfKeyStorageType encryptedIvAndPrivKeyIndex;

  Crypto_30_LibCv_SizeOfKeyStorageType privKeyIndex;
  Crypto_30_LibCv_SizeOfKeyStorageType partnerPubKeyIndex;

  Crypto_30_LibCv_SizeOfKeyStorageType newPubKeyIndex;

  uint8 otherInfo[CRYPTO_30_LIBCV_ISO15118_SIZEOF_OTHER_INFO];
  uint8 derivedKey[CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE];

  uint8 newPrivateKey[CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE];

  Std_ReturnType retVal = E_NOT_OK;
  Std_ReturnType localRet;
  /* ----- Implementation ----------------------------------------------- */
  Crypto_30_LibCv_ClearData(derivedKey, CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  /* #5 Get KeyElements for CertificateInstallation/Update (ISO 15118) */
  localRet = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(targetCryptoKeyId, CRYPTO_KE_CUSTOM_SCC_CONTRACT_PUBLIC_KEY, &newPubKeyIndex, &newPubKeyLength, CRYPTO_30_LIBCV_LENGTH_CHECK_MAX); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  localRet |= Crypto_30_LibCv_Local_KeyElementGetStorageIndex(targetCryptoKeyId, CRYPTO_KE_CUSTOM_SCC_IV_AND_ENCRYPTED_PRIVATE_KEY, &encryptedIvAndPrivKeyIndex, &encryptedIvAndPrivKeyLength, CRYPTO_30_LIBCV_LENGTH_CHECK_MAX); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  localRet |= Crypto_30_LibCv_Local_KeyElementGetStorageIndex(targetCryptoKeyId, CRYPTO_KE_CUSTOM_KEYEXCHANGE_PARTNER_PUB_KEY, &partnerPubKeyIndex, &partnerPubKeyLength, CRYPTO_30_LIBCV_LENGTH_CHECK_MAX); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  /* load old partner public value and private key */
  localRet |= Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, &privKeyIndex, &privKeyLength, CRYPTO_30_LIBCV_LENGTH_CHECK_MAX); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if ((localRet == E_OK) &&
    (encryptedIvAndPrivKeyLength == CRYPTO_30_LIBCV_ISO15118_SIZEOF_IV_AND_PRIV_KEY))
  {
    /* [V2G2-818]
    * The algorithm ID shall be one character 0x01. The sender name ID U shall be one character "U" = 0x55 , the receiver name ID V shall be
    * one character "V" = 0x56. A symmetric encryption key of exactly 128 bits shall be derived. */

    /* #10 Set OtherInfo = (AlgorithmID || PartyUInfo(Transmitter) || PartyVInfo(Receiver)) */
    otherInfo[0] = 0x01u; /* SBSW_CRYPTO_30_LIBCV_VARIABLE_ACCESS_PTR_FORM_STACK */
    otherInfo[1] = 0x55u; /* SBSW_CRYPTO_30_LIBCV_VARIABLE_ACCESS_PTR_FORM_STACK */
    otherInfo[2] = 0x56u; /* SBSW_CRYPTO_30_LIBCV_VARIABLE_ACCESS_PTR_FORM_STACK */

    /* #15 Execute Concatenation KDF */
    localRet = Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S(workspace,
                                                                        Crypto_30_LibCv_GetAddrKeyStorage(privKeyIndex), privKeyLength,
                                                                        Crypto_30_LibCv_GetAddrKeyStorage(partnerPubKeyIndex), partnerPubKeyLength,
                                                                        otherInfo, CRYPTO_30_LIBCV_ISO15118_SIZEOF_OTHER_INFO,
                                                                        derivedKey, &derivedKeyLength); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    if (localRet == E_OK)
    {
      /* #20 decrypt new key */
      localRet = Crypto_30_LibCv_Local_KeyDerive_ISO15118_Aes128Decrypt(&workspace->wsAes,
                                                                        Crypto_30_LibCv_GetAddrKeyStorage(encryptedIvAndPrivKeyIndex),
                                                                        derivedKey,
                                                                        Crypto_30_LibCv_GetAddrKeyStorage(encryptedIvAndPrivKeyIndex + CRYPTO_30_LIBCV_ISO15118_PRIV_KEY_START),
                                                                        encryptedIvAndPrivKeyLength - CRYPTO_30_LIBCV_ISO15118_PRIV_KEY_START,
                                                                        newPrivateKey, &newPrivateKeyLength); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
      if (localRet == E_OK)
      {
        /* #25 Verify new key pair */
        /* The best way the verify that the keys are valid is to generate and verify a signature. This need to be implemented by upper layer e.g. TLS */
        if (Crypto_30_LibCv_Local_KeyDerive_ISO15118_CheckKey(&workspace->wsEcP,
            newPrivateKey, newPrivateKeyLength,
            Crypto_30_LibCv_GetAddrKeyStorage(newPubKeyIndex), newPubKeyLength)
            == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
        {
          /* #30 Store new key pair */
          if (Crypto_30_LibCv_Local_KeyElementSet(targetCryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, &newPrivateKey[0], newPrivateKeyLength) == E_OK)  /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
          {
            retVal = E_OK;
          }
        }
      }
    }
  }

  /* #40 clear data */
  Crypto_30_LibCv_ClearData(derivedKey, derivedKeyLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  Crypto_30_LibCv_ClearData(newPrivateKey, newPrivateKeyLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_ISO15118()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_ISO15118(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Crypto_30_LibCv_WorkSpaceISO15118 workspace;  /* PRQA S 0759 */ /* MD_MSR_Union */

  /* ----- Implementation ----------------------------------------------- */
  retVal = Crypto_30_LibCv_Local_KeyDerive_ISO15118_WS(&workspace, cryptoKeyId, targetCryptoKeyId);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA256_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_KDF_963_256_Calc()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 9 */ /* MD_CRYPTO_30_LIBCV_STPAR_VSECPRIM */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_KDF_963_256_Calc(
  P2VAR(eslt_WorkSpaceKDFX963SHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 secretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) secretPtr,
  uint32 infoLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) infoPtr,
  uint32 keyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) keyPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;

  /* ----- Implementation ----------------------------------------------- */

  /*  Init  workspace header */
  eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workspace->header, ESL_MAXSIZEOF_WS_KDFX963SHA256, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /*  Init  workspace */
    eslRet = esl_initKDFX963SHA256((P2VAR(eslt_WorkSpaceKDFX963SHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /*  Derive a key with the given length. */
    eslRet = esl_deriveKeyKDFX963SHA256((P2VAR(eslt_WorkSpaceKDFX963SHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, /* PRQA S 0310, 0311 */ /* MD_CRYPTO_30_LIBCV_0310, MD_CRYPTO_30_LIBCV_0311 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
                                        (eslt_Length)secretLength,
                                        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))secretPtr,
                                        (eslt_Length)infoLength,
                                        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))infoPtr,
                                        (eslt_Length)keyLength,
                                        (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))keyPtr);
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_KDF_963_256()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_KDF_963_256(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  eslt_WorkSpaceKDFX963SHA256 workspace;

  /* ----- Implementation ----------------------------------------------- */
  retVal= Crypto_30_LibCv_Local_KeyDerive_KDF_963_256_WS(&workspace, cryptoKeyId, targetCryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_KDF_963_256_WS()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_KDF_963_256_WS(
  P2VAR(eslt_WorkSpaceKDFX963SHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK, localRet;

  uint32 secretLength, saltLength, keyLength;
  Crypto_30_LibCv_SizeOfKeyStorageType secretIndex, saltIndex;
  Crypto_30_LibCv_SizeOfKeyElementsType keyElementIndex;
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) saltPtr = NULL_PTR; /* PRQA S 3678 */ /* MD_CRYPTO_30_LIBCV_3678 */

  /* ----- Implementation ----------------------------------------------- */
  /* get key elements for algorithm: CRYPTO_KE_KEYDERIVATION_PASSWORD and optional CRYPTO_KE_KEYDERIVATION_SALT */
  /* get target key element index, length and check access */
  localRet = Crypto_30_LibCv_Local_KeyDerive_KDF_963_Get_Secret_And_Salt(
    &secretLength,
    &secretIndex,
    &saltLength,
    &saltIndex,
    &keyLength,
    &keyElementIndex,
    cryptoKeyId,
    targetCryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */

  if (localRet == E_OK)
  {
    if (saltLength > 0u)
    {
      saltPtr = Crypto_30_LibCv_GetAddrKeyStorage(saltIndex);
    }

    /* derive key */
    retVal = Crypto_30_LibCv_Local_KeyDerive_KDF_963_256_Calc(
      workspace,
      secretLength,
      Crypto_30_LibCv_GetAddrKeyStorage(secretIndex),
      saltLength,
      saltPtr,
      keyLength,
      Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(keyElementIndex)); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    if (retVal == E_OK)
    {
      /* update key element */
      Crypto_30_LibCv_Local_SetKeyElementStateWritten(keyElementIndex);
      Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId(targetCryptoKeyId, keyElementIndex, keyLength);
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA256_ENABLED == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA512_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_KDF_963_512_Calc()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 8 */ /* MD_CRYPTO_30_LIBCV_STPAR_VSECPRIM */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_KDF_963_512_Calc(
  P2VAR(eslt_WorkSpaceKDFX963SHA512, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 secretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) secretPtr,
  uint32 infoLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) infoPtr,
  uint32 keyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) keyPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;

  /* ----- Implementation ----------------------------------------------- */

  /*  Init  workspace header */
  eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workspace->header, ESL_MAXSIZEOF_WS_KDFX963SHA512, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /*  Init  workspace */
    eslRet = esl_initKDFX963SHA512((P2VAR(eslt_WorkSpaceKDFX963SHA512, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /*  Derive a key with the given length. */
    eslRet = esl_deriveKeyKDFX963SHA512((P2VAR(eslt_WorkSpaceKDFX963SHA512, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, /* PRQA S 0310, 0311 */ /* MD_CRYPTO_30_LIBCV_0310, MD_CRYPTO_30_LIBCV_0311 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
                                        (eslt_Length)secretLength,
                                        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))secretPtr,
                                        (eslt_Length)infoLength,
                                        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))infoPtr,
                                        (eslt_Length)keyLength,
                                        (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))keyPtr);
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_KDF_963_512()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_KDF_963_512(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  eslt_WorkSpaceKDFX963SHA512 workspace;

  /* ----- Implementation ----------------------------------------------- */
  retVal= Crypto_30_LibCv_Local_KeyDerive_KDF_963_512_WS(&workspace, cryptoKeyId, targetCryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_KDF_963_512_WS()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_KDF_963_512_WS(
  P2VAR(eslt_WorkSpaceKDFX963SHA512, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK, localRet;

  uint32 secretLength, saltLength, keyLength;
  Crypto_30_LibCv_SizeOfKeyStorageType secretIndex, saltIndex;
  Crypto_30_LibCv_SizeOfKeyElementsType keyElementIndex;
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) saltPtr = NULL_PTR; /* PRQA S 3678 */ /* MD_CRYPTO_30_LIBCV_3678 */

  /* ----- Implementation ----------------------------------------------- */
  /* get key elements for algorithm: CRYPTO_KE_KEYDERIVATION_PASSWORD and optional CRYPTO_KE_KEYDERIVATION_SALT */
  /* get target key element index, length and check access */
  localRet = Crypto_30_LibCv_Local_KeyDerive_KDF_963_Get_Secret_And_Salt(
    &secretLength,
    &secretIndex,
    &saltLength,
    &saltIndex,
    &keyLength,
    &keyElementIndex,
    cryptoKeyId,
    targetCryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */

  if (localRet == E_OK)
  {
    if (saltLength > 0u)
    {
      saltPtr = Crypto_30_LibCv_GetAddrKeyStorage(saltIndex);
    }

    /* derive key */
    retVal = Crypto_30_LibCv_Local_KeyDerive_KDF_963_512_Calc(
      workspace,
      secretLength,
      Crypto_30_LibCv_GetAddrKeyStorage(secretIndex),
      saltLength,
      saltPtr,
      keyLength,
      Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(keyElementIndex)); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    if (retVal == E_OK)
    {
      /* update key element */
      Crypto_30_LibCv_Local_SetKeyElementStateWritten(keyElementIndex);
      Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId(targetCryptoKeyId, keyElementIndex, keyLength);
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA512_ENABLED == STD_ON) */

#if ((CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA256_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA512_ENABLED == STD_ON))
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_KDF_963_Get_Secret_And_Salt()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6060 9 */ /* MD_CRYPTO_30_LIBCV_STPAR */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_KDF_963_Get_Secret_And_Salt(
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) secretLength,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) secretIndex,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) saltLength,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) saltIndex,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) keyLength,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) keyElementIndex,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType localRet;

  /* ----- Implementation ----------------------------------------------- */
  /* Get secret */
  localRet = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, secretIndex, secretLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (localRet == E_OK)
  {
    /* Get optional salt */
    localRet = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_SALT, saltIndex, saltLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    if (localRet == CRYPTO_E_KEY_NOT_AVAILABLE)
    {
      /* Set length since input in not required. */
      *saltLength = 0u; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */

      /* continue processing */
      localRet = E_OK;
    }
    else
    {
      /* retVal is already set */
    }

    if (localRet == E_OK)
    {
      /* get target key element index, length and check access */
      localRet = Crypto_30_LibCv_Local_KeyElementSet_Check(targetCryptoKeyId, CRYPTO_30_LIBCV_KE_TARGET_KEY, keyElementIndex, keyLength, CRYPTO_30_LIBCV_WA_INTERNAL_COPY); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    }
  }
  return localRet;
}
#endif /* ((CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA256_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA512_ENABLED == STD_ON)) */

#if (CRYPTO_30_LIBCV_SERVICE_KEY_DERIVE == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_Derive_UpdateKeyState()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Derive_UpdateKeyState(
  uint32 objectId,
  uint32 targetCryptoKeyId,
  Std_ReturnType retVal)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */

  /* If key was derived successfully, mark it as valid */
  if (retVal == E_OK)
  {
    Crypto_30_LibCv_ProcessJob_Trigger_Write[objectId] = Crypto_30_LibCv_SetKeyState(targetCryptoKeyId, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK); /* SBSW_CRYPTO_30_LIBCV_GLOBAL_ARRAY_ACCESS_WITH_SIZE */
  }
}
#endif /* (CRYPTO_30_LIBCV_SERVICE_KEY_DERIVE == STD_ON) */

#if (CRYPTO_30_LIBCV_KEY_DERIVE_ALGORITHM == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementSet_Check()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementSet_Check( /* PRQA S 3219 */ /* MD_CRYPTO_30_LIBCV_3219_1 */
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) elementIndexPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) keyLengthPtr,
  Crypto_30_LibCv_WriteOfKeyElementInfoType writeAccess)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* Check if keyElmentId is available */
  retVal = Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, keyElementId, elementIndexPtr); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (retVal != E_OK)
  {
    /* Parse return value */
    retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
  }
  else
# if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
    if (Crypto_30_LibCv_IsSheKey(*elementIndexPtr))
    {
      /* Access not allowed due to she key */
      retVal = CRYPTO_E_KEY_WRITE_FAIL;
    }
    else
# endif
      /* Check if it is a write once key which is written already. */
      if (Crypto_30_LibCv_IsKeyElementStateByMask(*elementIndexPtr, CRYPTO_30_LIBCV_KEYELEMENTSTATE_WRITTEN_ONCE_MASK))
      {
        retVal = CRYPTO_E_KEY_WRITE_FAIL;
      }
      else if (writeAccess >= (Crypto_30_LibCv_GetWriteOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(*elementIndexPtr))))
      {
        /* Access allowed due to access permission check */
        /* Get configured key element length  */
        *keyLengthPtr = Crypto_30_LibCv_GetKeyElementLength(*elementIndexPtr); /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        retVal = E_OK;
      }
      else
      {
        retVal = CRYPTO_E_KEY_WRITE_FAIL;
      }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementSet_CheckAndLength()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementSet_CheckAndLength( /* PRQA S 3219 */ /* MD_CRYPTO_30_LIBCV_3219_1 */
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) elementIndexPtr,
  uint32 requestedKeyLength,
  Crypto_30_LibCv_WriteOfKeyElementInfoType writeAccess)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint32 keyLength;

  /* ----- Implementation ----------------------------------------------- */
  /* Check if keyElmentId is available */
  retVal = Crypto_30_LibCv_Local_KeyElementSet_Check(cryptoKeyId, /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
                                                     keyElementId,
                                                     elementIndexPtr,
                                                     &keyLength,
                                                     writeAccess);

  if (retVal == E_OK)
  {
    if (keyLength < requestedKeyLength)
    {
      retVal = CRYPTO_E_KEY_SIZE_MISMATCH;
    }
    else if ((keyLength > requestedKeyLength) && (Crypto_30_LibCv_IsKeyElementPartial(*elementIndexPtr) == FALSE))
    {
      retVal = CRYPTO_E_KEY_SIZE_MISMATCH;
    }
    else
    {
      /* retVal is already set */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6030, 6050, 6080 4 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) writeBlock)
{
  uint32 deriveAlgorithmLength = CRYPTO_30_LIBCV_KEY_DERIVE_SIZEOF_ALGORITHM;
  Crypto_30_LibCv_SizeOfKeyStorageType deriveAlgorithmIndex;

  Std_ReturnType retVal;

  /* #1 Get Algorithm from the algorithm key element */
  retVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_ALGORITHM, &deriveAlgorithmIndex, &deriveAlgorithmLength, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (retVal == E_OK)
  {
    /* #5 Dispatch key derivation according the given algorithm */
    switch (Crypto_30_LibCv_GetKeyStorage(deriveAlgorithmIndex))
    {
# if (CRYPTO_30_LIBCV_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON)
      case CRYPTO_30_LIBCV_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256:
        retVal = Crypto_30_LibCv_Local_KeyDerive_Nist800108NistFips1864(cryptoKeyId, targetCryptoKeyId, CRYPTO_30_LIBCV_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256);
        break;
# endif
# if (CRYPTO_30_LIBCV_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
      case CRYPTO_30_LIBCV_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB:
        retVal = Crypto_30_LibCv_Local_KeyDerive_Nist800108NistFips1864(cryptoKeyId, targetCryptoKeyId, CRYPTO_30_LIBCV_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB);
        break;
# endif
# if (CRYPTO_30_LIBCV_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON)
      case CRYPTO_30_LIBCV_KDF_ALGO_KDF_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256:
        retVal = Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S_AndLoadKey(cryptoKeyId, targetCryptoKeyId);
        break;
# endif
# if (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)
      case CRYPTO_30_LIBCV_KDF_ALGO_KDF_ISO_15118_CERTIFICATE_HANDLING:
        retVal = Crypto_30_LibCv_Local_KeyDerive_ISO15118(cryptoKeyId, targetCryptoKeyId);
        break;
# endif
# if (CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA256_ENABLED == STD_ON)
      case CRYPTO_30_LIBCV_KDF_ALGO_KDF_X963_SHA256:
        retVal = Crypto_30_LibCv_Local_KeyDerive_KDF_963_256(cryptoKeyId, targetCryptoKeyId);
        break;
# endif
# if (CRYPTO_30_LIBCV_KDF_ALGO_X963_SHA512_ENABLED == STD_ON)
      case CRYPTO_30_LIBCV_KDF_ALGO_KDF_X963_SHA512:
        retVal = Crypto_30_LibCv_Local_KeyDerive_KDF_963_512(cryptoKeyId, targetCryptoKeyId);
        break;
# endif
# if (CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA1_ENABLED == STD_ON)
      case CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA1:
        retVal = Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA1(cryptoKeyId, targetCryptoKeyId);
        break;
# endif
# if (CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA256_ENABLED == STD_ON)
      case CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA256:
        retVal = Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA256(cryptoKeyId, targetCryptoKeyId);
        break;
# endif
# if (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA256_ENABLED == STD_ON)
      case CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA256:
        retVal = Crypto_30_LibCv_Local_KeyDerive_HKDF_HMAC(cryptoKeyId, targetCryptoKeyId, CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA256);
        break;
# endif
# if (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_OPTION1_SHA256_ENABLED == STD_ON)
      case CRYPTO_30_LIBCV_KDF_ALGO_HKDF_OPTION1_SHA256:
        retVal = Crypto_30_LibCv_Local_KeyDerive_HKDF_Hash_Option_1(cryptoKeyId, targetCryptoKeyId, ESL_HA_SHA2_256);
        break;
# endif
# if (CRYPTO_30_LIBCV_KDF_ALGO_SPAKE2_PLUS_P256R1_ENABLED == STD_ON)
      case CRYPTO_30_LIBCV_KDF_ALGO_SPAKE2_PLUS_P256R1:
        retVal = Crypto_30_LibCv_Local_KeyDerive_Spake2Plus(cryptoKeyId, targetCryptoKeyId);
        break;
# endif
# if (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA384_ENABLED == STD_ON)
      case CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA384:
        retVal = Crypto_30_LibCv_Local_KeyDerive_HKDF_HMAC(cryptoKeyId, targetCryptoKeyId, CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA384);
        break;
# endif

      default:
        retVal = E_NOT_OK;
        break;
    }
  }
  else
  {
    retVal = E_NOT_OK;
  }

  /* #10 If key was derived successfully, mark it as valid */
  if (retVal == E_OK)
  {
    *writeBlock = Crypto_30_LibCv_SetKeyState(targetCryptoKeyId, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK);  /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  }
  else
  {
    retVal = E_NOT_OK;
    *writeBlock = FALSE;  /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_SUPPORT_KEY_DERIVE == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA1_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Calc_PBKDF2_HMAC_SHA1()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 9 */ /* MD_CRYPTO_30_LIBCV_STPAR_VSECPRIM */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Calc_PBKDF2_HMAC_SHA1(
  P2VAR(eslt_WorkSpaceKDF2HMACSHA1, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 secretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) secretPtr,
  uint32 infoLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) infoPtr,
  uint32 keyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  uint32 iterations)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;

  /* ----- Implementation ----------------------------------------------- */

  /*  Init  workspace header */
  eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workspace->header, ESL_MAXSIZEOF_WS_KDF2HMACSHA1, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /*  Init  workspace */
    eslRet = esl_initKDF2HMACSHA1((P2VAR(eslt_WorkSpaceKDF2HMACSHA1, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, (eslt_Length)iterations); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /*  Derive a key with the given length. */
    eslRet = esl_deriveKeyKDF2HMACSHA1((P2VAR(eslt_WorkSpaceKDF2HMACSHA1, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, /* PRQA S 0310, 0311 */ /* MD_CRYPTO_30_LIBCV_0310, MD_CRYPTO_30_LIBCV_0311 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
                                        (eslt_Length)secretLength,
                                        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))secretPtr,
                                        (eslt_Length)infoLength,
                                        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))infoPtr,
                                        (eslt_Length)keyLength,
                                        (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))keyPtr);
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA1()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA1(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  eslt_WorkSpaceKDF2HMACSHA1 workspace;

  /* ----- Implementation ----------------------------------------------- */
  retVal= Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA_WS(&workspace, cryptoKeyId, targetCryptoKeyId, CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA1); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_KEYDERIVEKDF2HMACSHA1 == STD_ON) */

#if((CRYPTO_30_LIBCV_KEYDERIVEKDF2HMACSHA1 == STD_ON) || (CRYPTO_30_LIBCV_KEYDERIVEKDF2HMACSHA256 == STD_ON))
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA_WS()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA_WS(
  P2VAR(eslt_WorkSpaceKDF2, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  uint8 algorithm)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK, localRet;
  uint32 iterationLength = CRYPTO_30_LIBCV_SIZEOF_KDF_ITERATIONSLENGTH;
  uint32 secretLength, saltLength, keyLength;
  Crypto_30_LibCv_SizeOfKeyStorageType secretIndex, saltIndex, iterationIndex;
  Crypto_30_LibCv_SizeOfKeyElementsType keyElementIndex;
  uint32 iterations;

  /* ----- Implementation ----------------------------------------------- */
  /* Get secret */
  localRet = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, &secretIndex, &secretLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  /* Get salt */
  localRet |= Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_SALT, &saltIndex, &saltLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  /* Get iterations */
  localRet |= Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_ITERATIONS, &iterationIndex, &iterationLength, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  /* Get target key element index, length and check access */
  localRet |= Crypto_30_LibCv_Local_KeyElementSet_Check(targetCryptoKeyId, CRYPTO_30_LIBCV_KE_TARGET_KEY, &keyElementIndex, &keyLength, CRYPTO_30_LIBCV_WA_INTERNAL_COPY); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (localRet == E_OK)
  {
    Crypto_30_LibCv_Local_Uint8ArrayToUint32BigEndian(&iterations, Crypto_30_LibCv_GetAddrKeyStorage(iterationIndex)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

    /* Dispatch key derivation according the given algorithm */
    switch (algorithm)
    {
# if (CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA1_ENABLED == STD_ON)
    case CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA1:
      /* Derive key */
      retVal = Crypto_30_LibCv_Local_KeyDerive_Calc_PBKDF2_HMAC_SHA1(workspace,
        secretLength, Crypto_30_LibCv_GetAddrKeyStorage(secretIndex),
        saltLength, Crypto_30_LibCv_GetAddrKeyStorage(saltIndex),
        keyLength, Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(keyElementIndex),
        iterations); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
      break;
# endif
# if (CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA256_ENABLED == STD_ON)
    case CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA256:
      /* Derive key */
      retVal = Crypto_30_LibCv_Local_KeyDerive_Calc_PBKDF2_HMAC_SHA256(workspace,
        secretLength, Crypto_30_LibCv_GetAddrKeyStorage(secretIndex),
        saltLength, Crypto_30_LibCv_GetAddrKeyStorage(saltIndex),
        keyLength, Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(keyElementIndex),
        iterations); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
      break;
# endif

    default: /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
      /* Not supported State */
      retVal = E_NOT_OK;
      break;
    }

    if (retVal == E_OK)
    {
      /* Update key element */
      Crypto_30_LibCv_Local_SetKeyElementStateWritten(keyElementIndex);
      Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId(targetCryptoKeyId, keyElementIndex, keyLength);
    }
  }

  return retVal;
}
#endif /* ( CRYPTO_30_LIBCV_KEYDERIVEKDF2HMACSHA1 == STD_ON || CRYPTO_30_LIBCV_KEYDERIVEKDF2HMACSHA256 == STD_ON) */

#if (CRYPTO_30_LIBCV_KEYDERIVEKDF2HMACSHA256 == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA256()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA256(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  eslt_WorkSpaceKDF2HMACSHA256 workspace;

  /* ----- Implementation ----------------------------------------------- */
  retVal= Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA_WS(&workspace, cryptoKeyId, targetCryptoKeyId, CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA256); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Calc_PBKDF2_HMAC_SHA256()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 9 */ /* MD_CRYPTO_30_LIBCV_STPAR_VSECPRIM */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Calc_PBKDF2_HMAC_SHA256(
  P2VAR(eslt_WorkSpaceKDF2HMACSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 secretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) secretPtr,
  uint32 infoLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) infoPtr,
  uint32 keyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  uint32 iterations)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;

  /* ----- Implementation ----------------------------------------------- */

  /*  Init  workspace header */
  eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workspace->header, ESL_MAXSIZEOF_WS_KDF2HMACSHA256, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /*  Init  workspace */
    eslRet = esl_initKDF2HMACSHA256((P2VAR(eslt_WorkSpaceKDF2HMACSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, (eslt_Length)iterations); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /*  Derive a key with the given length. */
    eslRet = esl_deriveKeyKDF2HMACSHA256((P2VAR(eslt_WorkSpaceKDF2HMACSHA1, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, /* PRQA S 0310, 0311 */ /* MD_CRYPTO_30_LIBCV_0310, MD_CRYPTO_30_LIBCV_0311 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
                                        (eslt_Length)secretLength,
                                        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))secretPtr,
                                        (eslt_Length)infoLength,
                                        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))infoPtr,
                                        (eslt_Length)keyLength,
                                        (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))keyPtr);
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_KEYDERIVEKDF2HMACSHA256 == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_HKDF_HMAC_WS()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_HKDF_HMAC_WS(
  P2VAR(eslt_WorkSpaceHKDFHMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  uint8 hashAlgorithmId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK, localRet, localRet_2;
  uint32 iterationLength = CRYPTO_30_LIBCV_SIZEOF_HKDF_ITERATIONSLENGTH;
  uint32 secretLength, saltLength, infoLength, keyLength;
  Crypto_30_LibCv_SizeOfKeyStorageType secretIndex, saltIndex = 0u, infoIndex = 0u, iterationIndex;
  Crypto_30_LibCv_SizeOfKeyElementsType keyElementIndex;
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) saltPtr;
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) infoPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* Get salt */
  localRet = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_SALT, &saltIndex, &saltLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  if (localRet == CRYPTO_E_KEY_NOT_AVAILABLE)
  {
    localRet = E_OK;
    saltPtr = NULL_PTR;
    saltLength = 0u;
  }
  else
  {
    saltPtr = Crypto_30_LibCv_GetAddrKeyStorage(saltIndex);
  }

  /* Get additional info */
  localRet_2 = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_KEYDERIVATION_ADDITIONAL_INFO, &infoIndex, &infoLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  if (localRet_2 == CRYPTO_E_KEY_NOT_AVAILABLE)
  {
    infoPtr = NULL_PTR;
    infoLength = 0u;
  }
  else
  {
    localRet |= localRet_2;
    infoPtr = Crypto_30_LibCv_GetAddrKeyStorage(infoIndex);
  }

  /* Get secret */
  localRet |= Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, &secretIndex, &secretLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  /* Get iterations */
  localRet |= Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_ITERATIONS, &iterationIndex, &iterationLength, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  /* Get target key element index, length and check access */
  localRet |= Crypto_30_LibCv_Local_KeyElementSet_Check(targetCryptoKeyId, CRYPTO_30_LIBCV_KE_TARGET_KEY, &keyElementIndex, &keyLength, CRYPTO_30_LIBCV_WA_INTERNAL_COPY); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (localRet == E_OK)
  {
    /* Dispatch key derivation according the given algorithm */
    /* Derive key */
    switch(hashAlgorithmId)
    {
# if (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA256_ENABLED == STD_ON)
      case CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA256:
        retVal = Crypto_30_LibCv_Local_KeyDerive_Calc_HKDF_HMAC_SHA256(workspace,
          secretLength, Crypto_30_LibCv_GetAddrKeyStorage(secretIndex),
          saltLength, saltPtr,
          infoLength, infoPtr,
          keyLength, Crypto_30_LibCv_GetAddrKeyStorage(Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(keyElementIndex)),
          *Crypto_30_LibCv_GetAddrKeyStorage(iterationIndex)); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
        break;
# endif
# if (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA384_ENABLED == STD_ON)
      case CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA384:
        retVal = Crypto_30_LibCv_Local_KeyDerive_Calc_HKDF_HMAC_SHA384(workspace,
          secretLength, Crypto_30_LibCv_GetAddrKeyStorage(secretIndex),
          saltLength, saltPtr,
          infoLength, infoPtr,
          keyLength, Crypto_30_LibCv_GetAddrKeyStorage(Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(keyElementIndex)),
          *Crypto_30_LibCv_GetAddrKeyStorage(iterationIndex)); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
        break;
# endif
      default: /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
        /* Not supported algorithm */
        retVal = E_NOT_OK;
        break;
    }

    if (retVal == E_OK)
    {
      /* Update key element */
      Crypto_30_LibCv_Local_SetKeyElementStateWritten(keyElementIndex);
      Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId(targetCryptoKeyId, keyElementIndex, keyLength);
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_HKDF_HMAC()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_HKDF_HMAC(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  uint8 hashAlgorithmId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  eslt_WorkSpaceHKDFHMAC workspace;

  /* ----- Implementation ----------------------------------------------- */
  retVal = Crypto_30_LibCv_Local_KeyDerive_HKDF_HMAC_WS(&workspace, cryptoKeyId, targetCryptoKeyId, hashAlgorithmId); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_ENABLED == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA256_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Calc_HKDF_HMAC_SHA256()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 10 */ /* MD_MSR_STPAR */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Calc_HKDF_HMAC_SHA256(
  P2VAR(eslt_WorkSpaceHKDFHMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 secretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) secretPtr,
  uint32 saltLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) saltPtr,
  uint32 infoLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) infoPtr,
  uint32 keyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  uint8 iterations)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;

  /* ----- Implementation ----------------------------------------------- */

  /* Init workspace header */
  eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workspace->header, ESL_MAXSIZEOF_WS_HKDF_HMAC, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /* Init workspace */
    eslRet = esl_initHKDF_HMAC_SHA256((P2VAR(eslt_WorkSpaceHKDFHMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, iterations); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /* Derive a key with the given length. */
    eslRet = esl_deriveKeyHKDF_HMAC_SHA256((P2VAR(eslt_WorkSpaceHKDFHMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, /* PRQA S 0310, 0311 */ /* MD_CRYPTO_30_LIBCV_0310, MD_CRYPTO_30_LIBCV_0311 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
                                           (eslt_Length)secretLength,
                                           (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))secretPtr,
                                           (eslt_Length)saltLength,
                                           (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))saltPtr,
                                           (eslt_Length)infoLength,
                                           (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))infoPtr,
                                           (eslt_Length)keyLength,
                                           (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))keyPtr);
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA256_ENABLED == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_OPTION1_SHA256_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_HKDF_Hash_Option_1_WS()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_HKDF_Hash_Option_1_WS(
  P2VAR(eslt_WorkSpaceHKDFHASH, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  eslt_HashAlgorithm hashId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK, localRet;
  uint32 secretLength, infoLength, keyLength = 0u;
  Crypto_30_LibCv_SizeOfKeyStorageType secretIndex = 0u, infoIndex = 0u;
  Crypto_30_LibCv_SizeOfKeyElementsType keyElementIndex;
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) infoPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* Get additional info */
  localRet = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_KEYDERIVATION_ADDITIONAL_INFO, &infoIndex, &infoLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  if (localRet == CRYPTO_E_KEY_NOT_AVAILABLE)
  {
    infoPtr = NULL_PTR;
    infoLength = 0u;
    localRet = E_OK;
  }
  else
  {
    infoPtr = Crypto_30_LibCv_GetAddrKeyStorage(infoIndex);
  }

  /* Get secret */
  localRet |= Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, &secretIndex, &secretLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  /* Get target key element index, length and check access */
  localRet |= Crypto_30_LibCv_Local_KeyElementSet_Check(targetCryptoKeyId, CRYPTO_30_LIBCV_KE_TARGET_KEY, &keyElementIndex, &keyLength, CRYPTO_30_LIBCV_WA_INTERNAL_COPY); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (localRet == E_OK)
  {
    /* Dispatch key derivation according the given algorithm */
    /* Derive key */
    retVal = Crypto_30_LibCv_Local_KeyDerive_Calc_HKDF_Hash_Option_1(workspace,
                                                                     secretLength, Crypto_30_LibCv_GetAddrKeyStorage(secretIndex),
                                                                     infoLength, infoPtr,
                                                                     keyLength, Crypto_30_LibCv_GetAddrKeyStorage(Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(keyElementIndex)),
                                                                     hashId); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    if (retVal == E_OK)
    {
      /* Update key element */
      Crypto_30_LibCv_Local_SetKeyElementStateWritten(keyElementIndex);
      Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId(targetCryptoKeyId, keyElementIndex, keyLength);
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_HKDF_Hash_Option_1()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_HKDF_Hash_Option_1(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  eslt_HashAlgorithm hashId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  eslt_WorkSpaceHKDFHASH workspace;

  /* ----- Implementation ----------------------------------------------- */
  retVal = Crypto_30_LibCv_Local_KeyDerive_HKDF_Hash_Option_1_WS(&workspace, cryptoKeyId, targetCryptoKeyId, hashId); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Calc_HKDF_Hash_Option_1()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 10 */ /* MD_MSR_STPAR */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Calc_HKDF_Hash_Option_1(
  P2VAR(eslt_WorkSpaceHKDFHASH, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 secretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) secretPtr,
  uint32 infoLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) infoPtr,
  uint32 keyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  eslt_HashAlgorithm hashId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;

  /* ----- Implementation ----------------------------------------------- */

  /* Init workspace header */
  eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workspace->header, ESL_MAXSIZEOF_WS_HKDF_HASH, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /* Init workspace */
    eslRet = esl_initHKDF_Hash_OneStep((P2VAR(eslt_WorkSpaceHKDFHASH, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, hashId); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /* Derive a key with the given length. */
    eslRet = esl_deriveKeyHKDF_Hash_OneStep((P2VAR(eslt_WorkSpaceHKDFHASH, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, /* PRQA S 0310, 0311 */ /* MD_CRYPTO_30_LIBCV_0310, MD_CRYPTO_30_LIBCV_0311 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
                                           (eslt_Length)secretLength,
                                           (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))secretPtr,
                                           (eslt_Length)infoLength,
                                           (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))infoPtr,
                                           (eslt_Length)keyLength,
                                           (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))keyPtr);
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_OPTION1_SHA256_ENABLED == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA384_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Calc_HKDF_HMAC_SHA384()
 **********************************************************************************************************************/
 /*!
  *
  * Internal comment removed.
 *
 *
 *
  */
  /* PRQA S 6060 10 */ /* MD_MSR_STPAR */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Calc_HKDF_HMAC_SHA384(
  P2VAR(eslt_WorkSpaceHKDFHMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 secretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) secretPtr,
  uint32 saltLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) saltPtr,
  uint32 infoLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) infoPtr,
  uint32 keyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  uint8 iterations)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;

  /* ----- Implementation ----------------------------------------------- */

  /* Init workspace header */
  eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workspace->header, ESL_MAXSIZEOF_WS_HKDF_HMAC, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /* Init workspace */
    eslRet = esl_initHKDF_HMAC_SHA384((P2VAR(eslt_WorkSpaceHKDFHMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, iterations); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /* Derive a key with the given length. */
    eslRet = esl_deriveKeyHKDF_HMAC_SHA384((P2VAR(eslt_WorkSpaceHKDFHMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, /* PRQA S 0310, 0311 */ /* MD_CRYPTO_30_LIBCV_0310, MD_CRYPTO_30_LIBCV_0311 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
      (eslt_Length)secretLength,
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))secretPtr,
      (eslt_Length)saltLength,
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))saltPtr,
      (eslt_Length)infoLength,
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))infoPtr,
      (eslt_Length)keyLength,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))keyPtr);
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA384_ENABLED == STD_ON) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_SPAKE2_PLUS_P256R1_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Spake2Plus_WS()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Spake2Plus_WS(
  P2VAR(eslt_WorkSpaceSPAKE2PPreamble, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK, localRet;
  uint32 secretLength, w0Length, w1Length, lLength;
  Crypto_30_LibCv_SizeOfKeyStorageType secretIndex;
  Crypto_30_LibCv_SizeOfKeyElementsType w0ElementIndex, w1ElementIndex, lElementIndex;

  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainPtr;
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainExtPtr;

  /* ----- Implementation ----------------------------------------------- */

    /* Curve Sizes */
  domainPtr = (P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1Domain;
  domainExtPtr = (P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt;
  w0Length = CRYPTO_30_LIBCV_SIZEOF_ECC_256_N;
  w1Length = CRYPTO_30_LIBCV_SIZEOF_ECC_256_N;
  lLength = 1u + (2u * CRYPTO_30_LIBCV_SIZEOF_ECC_256_P); /* Prefix + Double P Size */

  /* Get secret [w0s|w1s] */
  localRet = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, &secretIndex, &secretLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  /* Get w0 key element index, length and check access */
  localRet |= Crypto_30_LibCv_Local_KeyElementSet_CheckAndLength(targetCryptoKeyId, CRYPTO_KE_CUSTOM_W0, &w0ElementIndex, w0Length, CRYPTO_30_LIBCV_WA_INTERNAL_COPY); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    /* Get w1 key element index, length and check access */
  localRet |= Crypto_30_LibCv_Local_KeyElementSet_CheckAndLength(targetCryptoKeyId, CRYPTO_KE_CUSTOM_W1, &w1ElementIndex, w1Length, CRYPTO_30_LIBCV_WA_INTERNAL_COPY); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    /* Get l (lx | ly) key element index, length and check access */
  localRet |= Crypto_30_LibCv_Local_KeyElementSet_CheckAndLength(targetCryptoKeyId, CRYPTO_KE_CUSTOM_L, &lElementIndex, lLength, CRYPTO_30_LIBCV_WA_INTERNAL_COPY); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (localRet == E_OK)
  {
    /* Derive key */
    retVal = Crypto_30_LibCv_Local_KeyDerive_Calc_Spake2Plus(workspace, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_GLOBAL_DATA_PTR */
                                                             domainPtr,
                                                             domainExtPtr,
                                                             Crypto_30_LibCv_GetAddrKeyStorage(secretIndex), secretLength,
                                                             Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(w0ElementIndex), w0Length,
                                                             Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(w1ElementIndex), w1Length,
                                                             Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(lElementIndex), lLength);

    if (retVal == E_OK)
    {
      /* Update key element */
      /* w0 */
      Crypto_30_LibCv_Local_SetKeyElementStateWritten(w0ElementIndex);
      Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId(targetCryptoKeyId, w0ElementIndex, w0Length);
      /* w1 */
      Crypto_30_LibCv_Local_SetKeyElementStateWritten(w1ElementIndex);
      Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId(targetCryptoKeyId, w1ElementIndex, w1Length);
      /* l */
      Crypto_30_LibCv_Local_SetKeyElementStateWritten(lElementIndex);
      Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId(targetCryptoKeyId, lElementIndex, lLength);
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Spake2Plus()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Spake2Plus(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  eslt_WorkSpaceSPAKE2PPreamble workspace;

  /* ----- Implementation ----------------------------------------------- */
  retVal = Crypto_30_LibCv_Local_KeyDerive_Spake2Plus_WS(&workspace, cryptoKeyId, targetCryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyDerive_Calc_Spake2Plus()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6060 12 */ /* MD_CRYPTO_30_LIBCV_STPAR_VSECPRIM */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive_Calc_Spake2Plus(
  P2VAR(eslt_WorkSpaceSPAKE2PPreamble, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainExtPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) secretPtr,
  uint32 secretLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) w0Ptr,
  uint32 w0Length,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) w1Ptr,
  uint32 w1Length,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) lPtr,
  uint32 lLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;
  eslt_Length lLengthPtr, w0LengthPtr, w1LengthPtr;

  /* ----- Implementation ----------------------------------------------- */

  lLengthPtr = (eslt_Length)lLength;
  w0LengthPtr = (eslt_Length)w0Length;
  w1LengthPtr = (eslt_Length)w1Length;
/*  Init  workspace header */
  eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workspace->header, ESL_SIZEOF_WS_SPAKE2PPRE, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /*  Init  workspace */
    eslRet = esl_initSPAKE2PPreamble((P2VAR(eslt_WorkSpaceSPAKE2PPreamble, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace,
                                     domainPtr,
                                     domainExtPtr); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /*  Derive a key with the given length. */
    eslRet = esl_calcSPAKE2PPreamble((P2VAR(eslt_WorkSpaceSPAKE2PPreamble, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, /* PRQA S 0310, 0311 */ /* MD_CRYPTO_30_LIBCV_0310, MD_CRYPTO_30_LIBCV_0311 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_GLOBAL_DATA_PTR */
                                     (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))secretPtr,
                                     (eslt_Length)secretLength,
                                     (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))w0Ptr,
                                     (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&w0LengthPtr,
                                     (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))w1Ptr,
                                     (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&w1LengthPtr,
                                     (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))lPtr,
                                     (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&lLengthPtr);
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
      retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_SPAKE2_PLUS_P256R1_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if (CRYPTO_30_LIBCV_KEYDERIVENISTFIPS186ERB == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyDeriveNistFips186Erb()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveNistFips186Erb(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyDeriveNistFips186Erb(Crypto_30_LibCv_GetKeyDeriveNistFips186ErbIdxOfObjectInfo(objectId));

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #10 Call Crypto_30_LibCv_Local_KeyDerive_Nist800108NistFips1864 */
    retVal = Crypto_30_LibCv_Local_KeyDerive_Nist800108NistFips1864_WS(workspace, job->cryptoKeyId, job->targetCryptoKeyId, CRYPTO_30_LIBCV_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */

    /* #20 If key was derived successfully, mark it as valid */
    Crypto_30_LibCv_Local_Derive_UpdateKeyState(objectId, job->targetCryptoKeyId, retVal);
  }
  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEYDERIVENISTFIPS186ERB() */

#if (CRYPTO_30_LIBCV_KEYDERIVEISO15118 == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyDeriveISO15118()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveISO15118(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  P2VAR(Crypto_30_LibCv_WorkSpaceISO15118, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyDeriveISO15118(Crypto_30_LibCv_GetKeyDeriveISO15118IdxOfObjectInfo(objectId));

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #10 Call Crypto_30_LibCv_Local_KeyDerive_ISO15118 */
    retVal = Crypto_30_LibCv_Local_KeyDerive_ISO15118_WS(workspace, job->cryptoKeyId, job->targetCryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */

    /* #20 If key was derived successfully, mark it as valid */
    Crypto_30_LibCv_Local_Derive_UpdateKeyState(objectId, job->targetCryptoKeyId, retVal);
  }
  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEYDERIVEISO15118() */

#if (CRYPTO_30_LIBCV_KEYDERIVEX963SHA256 == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyDeriveX963SHA256()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveX963SHA256(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    P2VAR(eslt_WorkSpaceKDFX963SHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyDeriveX963SHA256(Crypto_30_LibCv_GetKeyDeriveX963SHA256IdxOfObjectInfo(objectId));

    /* #10 Call Crypto_30_LibCv_Local_KeyDerive_KDF_963_256_WS */
    retVal = Crypto_30_LibCv_Local_KeyDerive_KDF_963_256_WS(workspace, job->cryptoKeyId, job->targetCryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */

    /* #20 If key was derived successfully, mark it as valid */
    Crypto_30_LibCv_Local_Derive_UpdateKeyState(objectId, job->targetCryptoKeyId, retVal);
  }
  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEYDERIVEX963SHA256() */

#if (CRYPTO_30_LIBCV_KEYDERIVEX963SHA512 == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyDeriveX963SHA512()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveX963SHA512(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    P2VAR(eslt_WorkSpaceKDFX963SHA512, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyDeriveX963SHA512(Crypto_30_LibCv_GetKeyDeriveX963SHA512IdxOfObjectInfo(objectId));

    /* #10 Call Crypto_30_LibCv_Local_KeyDerive_KDF_963_512_WS */
    retVal = Crypto_30_LibCv_Local_KeyDerive_KDF_963_512_WS(workspace, job->cryptoKeyId, job->targetCryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */

    /* #20 If key was derived successfully, mark it as valid */
    Crypto_30_LibCv_Local_Derive_UpdateKeyState(objectId, job->targetCryptoKeyId, retVal);
  }
  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEYDERIVEX963() */

#if (CRYPTO_30_LIBCV_KEYDERIVENIST80056AONEPASS == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyDeriveNist80056AOnePass()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveNist80056AOnePass(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  P2VAR(Crypto_30_LibCv_WorkSpaceISO15118, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyDeriveNist80056AOnePass(Crypto_30_LibCv_GetKeyDeriveNist80056AOnePassIdxOfObjectInfo(objectId));

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #10 Call Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S_AndLoadKey */
    retVal = Crypto_30_LibCv_Local_KeyDerive_Nist80056A_OnePass_C1E1S_With_Ws_AndLoadKey(workspace, job->cryptoKeyId, job->targetCryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */

    /* #20 If key was derived successfully, mark it as valid */
    Crypto_30_LibCv_Local_Derive_UpdateKeyState(objectId, job->targetCryptoKeyId, retVal);
  }
  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEYDERIVENIST80056AONEPASS() */

#if (CRYPTO_30_LIBCV_KEYDERIVENIST800108CNT == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyDeriveNist800108Cnt()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveNist800108Cnt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyDeriveNist800108Cnt(Crypto_30_LibCv_GetKeyDeriveNist800108CntIdxOfObjectInfo(objectId));

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #10 Call Crypto_30_LibCv_Local_KeyDerive_Nist800108NistFips1864 */
    retVal = Crypto_30_LibCv_Local_KeyDerive_Nist800108NistFips1864_WS(workspace, job->cryptoKeyId, job->targetCryptoKeyId, CRYPTO_30_LIBCV_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */

    /* #20 If key was derived successfully, mark it as valid */
    Crypto_30_LibCv_Local_Derive_UpdateKeyState(objectId, job->targetCryptoKeyId, retVal);
  }
  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEYDERIVENIST800108CNT() */

#if (CRYPTO_30_LIBCV_KEYDERIVEKDF2HMACSHA1 == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyDeriveKDF2HMACSHA1()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveKDF2HMACSHA1(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    P2VAR(eslt_WorkSpaceKDF2HMACSHA1, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyDeriveKDF2HMACSHA1(Crypto_30_LibCv_GetKeyDeriveKDF2HMACSHA1IdxOfObjectInfo(objectId));

    /* #10 Call Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA_WS */
    retVal = Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA_WS(workspace, job->cryptoKeyId, job->targetCryptoKeyId, CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA1); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */

    /* #20 If key was derived successfully, mark it as valid */
    Crypto_30_LibCv_Local_Derive_UpdateKeyState(objectId, job->targetCryptoKeyId, retVal);
  }
  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_KEYDERIVEKDF2HMACSHA1 == STD_ON) */

#if (CRYPTO_30_LIBCV_KEYDERIVEKDF2HMACSHA256 == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyDeriveKDF2HMACSHA256()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveKDF2HMACSHA256(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    P2VAR(eslt_WorkSpaceKDF2HMACSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyDeriveKDF2HMACSHA256(Crypto_30_LibCv_GetKeyDeriveKDF2HMACSHA256IdxOfObjectInfo(objectId));

    /* #10 Call Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA_WS */
    retVal = Crypto_30_LibCv_Local_KeyDerive_PBKDF2_HMAC_SHA_WS(workspace, job->cryptoKeyId, job->targetCryptoKeyId, CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA256); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */

    /* #20 If key was derived successfully, mark it as valid */
    Crypto_30_LibCv_Local_Derive_UpdateKeyState(objectId, job->targetCryptoKeyId, retVal);
  }
  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_KEYDERIVEKDF2HMACSHA256 == STD_ON) */

#if (CRYPTO_30_LIBCV_KEYDERIVEHKDFHMACSHA256 == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyDeriveHKDFHMACSHA256()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveHKDFHMACSHA256(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    P2VAR(eslt_WorkSpaceHKDFHMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyDeriveHKDFHMACSHA256(Crypto_30_LibCv_GetKeyDeriveHKDFHMACSHA256IdxOfObjectInfo(objectId));

    /* #10 Call Crypto_30_LibCv_Local_KeyDerive_HKDF_HMAC_WS with SHA256 */
    retVal = Crypto_30_LibCv_Local_KeyDerive_HKDF_HMAC_WS(workspace, job->cryptoKeyId, job->targetCryptoKeyId, CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA256); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */

    /* #20 If key was derived successfully, mark it as valid */
    Crypto_30_LibCv_Local_Derive_UpdateKeyState(objectId, job->targetCryptoKeyId, retVal);
  }
  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_KEYDERIVEHKDFHMACSHA256 == STD_ON) */

#if (CRYPTO_30_LIBCV_KEYDERIVEHKDFHMACSHA384 == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyDeriveHKDFHMACSHA384()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveHKDFHMACSHA384(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    P2VAR(eslt_WorkSpaceHKDFHMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyDeriveHKDFHMACSHA384(Crypto_30_LibCv_GetKeyDeriveHKDFHMACSHA384IdxOfObjectInfo(objectId));

    /* #10 Call Crypto_30_LibCv_Local_KeyDerive_HKDF_HMAC_WS with SHA384 */
    retVal = Crypto_30_LibCv_Local_KeyDerive_HKDF_HMAC_WS(workspace, job->cryptoKeyId, job->targetCryptoKeyId, CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA384); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */

    /* #20 If key was derived successfully, mark it as valid */
    Crypto_30_LibCv_Local_Derive_UpdateKeyState(objectId, job->targetCryptoKeyId, retVal);
  }
  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_KEYDERIVEHKDFHMACSHA384 == STD_ON) */

#if (CRYPTO_30_LIBCV_KEYDERIVESPAKE2P == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyDeriveSpake2P()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveSpake2P(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    P2VAR(eslt_WorkSpaceSPAKE2PPreamble, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyDeriveSpake2P(Crypto_30_LibCv_GetKeyDeriveSpake2PIdxOfObjectInfo(objectId));

    /* #10 Derive key */
    retVal = Crypto_30_LibCv_Local_KeyDerive_Spake2Plus_WS(workspace, job->cryptoKeyId, job->targetCryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */

    /* #20 If key was derived successfully, mark it as valid */
    Crypto_30_LibCv_Local_Derive_UpdateKeyState(objectId, job->targetCryptoKeyId, retVal);
  }
  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_KEYDERIVESPAKE2P == STD_ON) */

#if (CRYPTO_30_LIBCV_KEYDERIVEHKDFHASHOPTION1 == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyDeriveHKDFHashOption1()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveHKDFHashOption1(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    P2VAR(eslt_WorkSpaceHKDFHASH, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyDeriveHKDFHashOption1(Crypto_30_LibCv_GetKeyDeriveHKDFHashOption1IdxOfObjectInfo(objectId));

    /* #10 Call Crypto_30_LibCv_Local_KeyDerive_HKDF_Hash_Option_1_WS */
    retVal = Crypto_30_LibCv_Local_KeyDerive_HKDF_Hash_Option_1_WS(workspace, job->cryptoKeyId, job->targetCryptoKeyId, ESL_HA_SHA2_256); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */

    /* #20 If key was derived successfully, mark it as valid */
    Crypto_30_LibCv_Local_Derive_UpdateKeyState(objectId, job->targetCryptoKeyId, retVal);

  }
  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_KEYDERIVEHKDFHASHOPTION1 == STD_ON) */

#define CRYPTO_30_LIBCV_STOP_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_KeyDerive.c
 *********************************************************************************************************************/
