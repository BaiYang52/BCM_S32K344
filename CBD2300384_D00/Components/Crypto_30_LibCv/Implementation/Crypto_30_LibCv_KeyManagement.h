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
/*      \file  Crypto_30_LibCv_KeyManagement.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Internal header file for service key management function prototypes
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#ifndef CRYPTO_30_LIBCV_KEYMANAGEMENT_H
# define CRYPTO_30_LIBCV_KEYMANAGEMENT_H
# include "Csm_Types.h"
# include "Crypto_30_LibCv_Cfg.h"

# define CRYPTO_30_LIBCV_START_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_KeyCopy()
 *********************************************************************************************************************/
/*! \brief         Copy the key
 *  \details       Copies a key with all its elements to another key in the same crypto driver.
 *                 The key element can only be copied, if the destination key element write access right is less than
 *                 WA_INTERNAL_COPY.
 *                 Additional the read access right of the source must be less than RA_INTERNAL_COPY and the destination
 *                 read access right must be higher or equal than the source read access right.
 *  \param[in]     cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 *                                              element.
 *  \param[in]     targetCryptoKeyId            Holds the identifier of the key whose key element shall be the destination
 *                                              element.
 *  \return        E_OK                         Request successful.
 *                 E_NOT_OK                     Request failed.
 *                 CRYPTO_E_BUSY                Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_READ_FAIL       Request failed, read access was denied.
 *                 CRYPTO_E_KEY_WRITE_FAIL      Request failed, write access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE   Request failed, the key is not available.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, the key element sizes are not compatible.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *  \trace         CREQ-131140
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyCopy(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_KeyElementCopy()
 *********************************************************************************************************************/
/*! \brief         Copy key element
 *  \details       Copies a key element to another key element in the same crypto driver.
 *                 The key element can only be copied, if the destination key element write access right is less than
 *                 WA_INTERNAL_COPY.
 *                 Additional the read access right of the source must be less than RA_INTERNAL_COPY and the destination
 *                 read access right must be higher or equal than the source read access right.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be the source
 *                                         element.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be the source for the
 *                                         copy operation.
 *  \param[in]     targetCryptoKeyId       Holds the identifier of the key whose key element shall be the destination
 *                                         element.
 *  \param[in]     targetKeyElementId      Holds the identifier of the key element which shall be the destination for
 *                                         the copy operation.
 *  \return        E_OK                         Request successful.
 *                 E_NOT_OK                     Request failed.
 *                 CRYPTO_E_BUSY                Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_READ_FAIL       Request failed, read access was denied.
 *                 CRYPTO_E_KEY_WRITE_FAIL      Request failed, write access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE   Request failed, the key is not available.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, the key element sizes are not compatible.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *  \trace         CREQ-131141
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementCopy(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  uint32 targetCryptoKeyId,
  uint32 targetKeyElementId);

/**********************************************************************************************************************
 * Crypto_30_LibCv_KeyElementCopyPartial()
 **********************************************************************************************************************/
/*! \brief         Copy key element partial
 *  \details       Copies a key element to another key element in the same crypto driver. The keyElementSourceOffset
 *                 and keyElementCopyLength allows to copy just a part of the source key element into the destination.
 *                 The offset of the target key is also specified with this function.
 *                 The target key element needs to have partial access.
 *                 The key element can only be copied, if the destination key element write access right is less than
 *                 WA_INTERNAL_COPY.
 *                 Additional the read access right of the source must be less than RA_INTERNAL_COPY and the destination
 *                 read access right must be higher or equal than the source read access right.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be the source
 *                                         element.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be the source for the
 *                                         copy operation.
 *  \param[in]     keyElementSourceOffset  Holds the offset of the of the source key element indicating the start index
 *                                         of the copy operation.
 *  \param[in]     keyElementTargetOffset  Holds the offset of the of the target key element indicating the start index
 *                                         of the copy operation.
 *  \param[in]     keyElementCopyLength    Holds the number of bytes that shall be copied.
 *  \param[in]     targetCryptoKeyId       Holds the identifier of the key whose key element shall be the
 *                                         destination element.
 *  \param[in]     targetKeyElementId      Holds the identifier of the key element which shall be the destination for
 *                                         the copy operation.
 *  \return        E_OK                         Request successful.
 *                 E_NOT_OK                     Request failed.
 *                 CRYPTO_E_BUSY                Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_READ_FAIL       Request failed, read access was denied.
 *                 CRYPTO_E_KEY_WRITE_FAIL      Request failed, write access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE   Request failed, the key is not available.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, the key element sizes are not compatible.
 *                 CRYPTO_E_KEY_EMPTY           Request failed, uninitialized source key element.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *  \trace         CREQ-174489
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementCopyPartial(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  uint32 keyElementSourceOffset,
  uint32 keyElementTargetOffset,
  uint32 keyElementCopyLength,
  uint32 targetCryptoKeyId,
  uint32 targetKeyElementId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_KeyElementIdsGet()
 *********************************************************************************************************************/
/*! \brief         Used to retrieve information which key elements are available in a given key.
 *  \details       The service provides a list of key element which are available in the given key.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose available element ids shall be exported.
 *  \param[out]    keyElementIdsPtr        Contains the pointer to the array where the ids of the key
 *                                         elements shall be stored.
 *  \param[out]     keyElementIdsLengthPtr  Holds a pointer to the memory location in which the number of key element
 *                                         in the given key is stored. On calling this function, this parameter
 *                                         shall contain the size of the buffer provided by keyElementIdsPtr. When the
 *                                         request has finished, the actual number of key elements is stored.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-131139
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementIdsGet(
  uint32 cryptoKeyId,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyElementIdsPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyElementIdsLengthPtr);

/* [SWS_Crypto_91004] */
/**********************************************************************************************************************
 *  Crypto_30_LibCv_KeyElementSet()
 *********************************************************************************************************************/
/*! \brief         Sets a key element
 *  \details       Sets the given key element bytes to the key identified by cryptoKeyId.
 *                 The key element can only be set, if the write access right is WA_ALLOWED or WA_ENCRYPTED.
 *                 The access right need to be WA_ALLOWED for normal key, WA_ENCRYPTED for She key and WA_ALLOWED for She ram key.
 *                 If keyLength is zero the old value is deleted.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be set.
 *  \param[in]     keyPtr                  Holds the pointer to the key data which shall be set as key element.
 *  \param[in]     keyLength               Contains the length of the key element in bytes.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_WRITE_FAIL Request failed, write access was denied.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                             data.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *  \trace         CREQ-131144
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementSet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength);

/* [SWS_Crypto_91005] */
/**********************************************************************************************************************
 *  Crypto_30_LibCv_KeyValidSet()
 *********************************************************************************************************************/
/*! \brief         Sets the key to valid
 *  \details       Sets the key state of the key identified by cryptoKeyId to valid.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key elements shall be set to valid.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *  \trace         CREQ-131143
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyValidSet(
  uint32 cryptoKeyId);

/* [SWS_Crypto_91006] */
/**********************************************************************************************************************
 *  Crypto_30_LibCv_KeyElementGet()
 *********************************************************************************************************************/
/*! \brief         This interface shall be used to get a key element of the key identified by the cryptoKeyId and store
 *                 the key element in the memory location pointed to by the result pointer.
 *  \details       The key element can only be provided, if the read access right is RA_ALLOWED. If the key element
 *                 is configured to allow partial access, resultLengthPtr contains the amount of data which should
 *                 be read from the key element.
 *                 The behavior for partial access can be switched on or off in the configuration.
 *                 If the read access right is RA_ENCRYPTED the She ram key can be exported.
 *                 (64 Bytes for M1-M3 and 112Bytes for M1-M5)
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be set.
 *  \param[out]    resultPtr               Holds the pointer to the key data which shall be set as key element.
 *  \param[in,out] resultLengthPtr         Contains the length of the key element in bytes.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_READ_FAIL  Request failed, read access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-131142
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementGet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr);

/* [SWS_Crypto_91013] */
/**********************************************************************************************************************
 *  Crypto_30_LibCv_RandomSeed()
 *********************************************************************************************************************/
/*! \brief         Initialize the seed
 *  \details       This function generates the internal seed state using the provided entropy source.
 *                 Furthermore, this function can be used to update the seed state with new entropy
 *  \param[in]     cryptoKeyId             Holds the identifier of the key for which a new seed shall be generated.
 *  \param[in]     entropyPtr              Holds a pointer to the memory location which contains the
 *                                         data to feed the entropy.
 *  \param[in]     entropyLength           Contains the length of the entropy in bytes.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *  \trace         CREQ-131137
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_RandomSeed(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength);

/* [SWS_Crypto_91007] */
/**********************************************************************************************************************
 *  Crypto_30_LibCv_KeyGenerate()
 *********************************************************************************************************************/
/*! \brief         Generates a key
 *  \details       This function shall dispatch the key generate function to the configured crypto driver object.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which is to be updated with the generated
 *                                         value.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyGenerate(
  uint32 cryptoKeyId);

/* [SWS_Crypto_91008] */
/**********************************************************************************************************************
 *  Crypto_30_LibCv_KeyDerive()
 *********************************************************************************************************************/
/*! \brief         Derives a key
 *  \details       Derives a new key by using the key elements in the given key identified by the cryptoKeyId.
 *                 The key derivation function supports different algorithm which are specified by the
 *                 algorithm key element(CRYPTO_KE_KEYDERIVATION_ALGORITHM).
 *               - For CRYPTO_30_LIBCV_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256 and
 *                 CRYPTO_30_LIBCV_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB:
 *                 The given key must contain the key elements for the password (CRYPTO_KE_KEYDERIVATION_PASSWORD),
 *                 salt (CRYPTO_KE_KEYDERIVATION_SALT), algorithm (CRYPTO_KE_KEYDERIVATION_ALGORITHM) and the costum
 *                 element label(CRYPTO_KE_CUSTOM_KEYDERIVATION_LABEL). The number of iterations is automatically
 *                 determined by the needed key length. The derived key is stored in the key element with id 1 of the
 *                 given key identified by targetCryptoKeyId. The write access right for the destination key element
 *                 must be less than or equal to WA_INTERNAL_COPY.
 *               - For CRYPTO_30_LIBCV_KDF_ALGO_KDF_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256:
 *                 The KDF is reads the algorithm configuration from the cryptoKeyId key elements. The KDF requires
 *                 the following key elements the private key (CRYPTO_KE_KEYDERIVATION_PASSWORD),
 *                 the public key (CRYPTO_KE_CUSTOM_KEYEXCHANGE_PARTNER_PUB_KEY) and
 *                 the other Information (CRYPTO_KE_CUSTOM_KEYDERIVATION_ADDITIONAL_INFO).
 *                 The derived key is set at the target key element (CRYPTO_KE_KEYDERIVATION_PASSWORD).
 *               - For CRYPTO_30_LIBCV_KDF_ALGO_KDF_ISO_15118_CERTIFICATE_HANDLING:
 *                 For certificate installation the cryptoKeyId need to be the root certificate.
 *                 For certificate update the cryptoKeyId and targetCryptoKeyId need to be the same key Id.
 *                 The KDF is reads the algorithm configuration from the cryptoKeyId key elements. The KDF requires
 *                 the following key element the private key (CRYPTO_KE_KEYDERIVATION_PASSWORD).
 *                 The following key elements need to be set at the targetCryptoKeyId:
 *                 the contract/new public key (CRYPTO_KE_CUSTOM_SCC_CONTRACT_PUBLIC_KEY),
 *                 the IV and encrypted private key (CRYPTO_KE_CUSTOM_SCC_IV_AND_ENCRYPTED_PRIVATE_KEY) and
 *                 the public key (CRYPTO_KE_CUSTOM_KEYEXCHANGE_PARTNER_PUB_KEY).
 *                 The derived key is set at the target key element (CRYPTO_KE_KEYDERIVATION_PASSWORD).
 *               - For CRYPTO_30_LIBCV_KDF_ALGO_KDF_X963_SHA256 and CRYPTO_30_LIBCV_KDF_ALGO_KDF_X963_SHA512:
 *                 Derives a key according to KDF X9.63 with the given CRYPTO_KE_KEYDERIVATION_PASSWORD(Secret)
 *                 and optional CRYPTO_KE_KEYDERIVATION_SALT(Seed).
 *               - For CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA1 and CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA256:
 *                 The given key must contain the key elements for the password (CRYPTO_KE_KEYDERIVATION_PASSWORD),
 *                 salt (CRYPTO_KE_KEYDERIVATION_SALT), algorithm (CRYPTO_KE_KEYDERIVATION_ALGORITHM)
 *                 and the iteration element (CRYPTO_KE_KEYDERIVATION_ITERATIONS).
 *                 The number of iterations is determined by the CRYPTO_KE_KEYDERIVATION_ITERATIONS
 *                 element and must always be set with 4 bytes. The derived key is stored
 *                 in the key element with id 1 of the given key identified by targetCryptoKeyId.
 *                 The write access right for the destination key element must be less than or equal to WA_INTERNAL_COPY.
 *               - For CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA256 and CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA384:
 *                 The given key must contain the key elements for the password (CRYPTO_KE_KEYDERIVATION_PASSWORD),
 *                 salt (CRYPTO_KE_KEYDERIVATION_SALT), algorithm (CRYPTO_KE_KEYDERIVATION_ALGORITHM)
 *                 iteration element (CRYPTO_KE_KEYDERIVATION_ITERATIONS) and optionally
 *                 CRYPTO_KE_CUSTOM_KEYDERIVATION_ADDITIONAL_INFO (additional information).
 *                 The iteration element specifies whether the one-step or two-step variant is to be used.
 *                 The derived key is stored in the key element with id 1 of the given key identified by targetCryptoKeyId.
 *                 The write access right for the destination key element must be less than or equal to WA_INTERNAL_COPY.
 *               - For CRYPTO_30_LIBCV_KDF_ALGO_HKDF_OPTION1_SHA256:
 *                 The given key must contain the key elements for the password (CRYPTO_KE_KEYDERIVATION_PASSWORD),
 *                 algorithm (CRYPTO_KE_KEYDERIVATION_ALGORITHM) and optionally CRYPTO_KE_CUSTOM_KEYDERIVATION_ADDITIONAL_INFO (additional information).
 *                 The Algorithm element specifies which hash variant is to be used.
 *                 The derived key is stored in the key element with id 1 of the given key identified by targetCryptoKeyId.
 *                 The write access right for the destination key element must be less than or equal to WA_INTERNAL_COPY.
 *               - For CRYPTO_30_LIBCV_KDF_ALGO_SPAKE2_PLUS_P256R1:
 *                 The given key must contain the key element for the password (CRYPTO_KE_KEYDERIVATION_PASSWORD).
 *                 The derived key is stored in the key element with id 1 of the given key identified by targetCryptoKeyId.
 *                 The write access right for the destination key element must be less than or equal to WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]     targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyDerive(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);

/* [SWS_Crypto_91009] */
/**********************************************************************************************************************
 *  Crypto_30_LibCv_KeyExchangeCalcPubVal()
 *********************************************************************************************************************/
/*! \brief         Calculation of the public value
 *  \details       Calculates the public value for the key exchange and stores the public key in the
 *                 memory location pointed to by the public value pointer. The write access right for the destination
 *                 key element must be less than or equal to WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[out]    publicValuePtr          Contains the pointer to the data where the public value shall be stored.
 *  \param[in,out] publicValueLengthPtr    Holds a pointer to the memory location in which the public value length
 *                                         information is stored. On calling this function, this parameter shall
 *                                         contain the size of the buffer provided by publicValuePtr. When the request
 *                                         has finished, the actual length of the returned value shall be stored.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *  \trace         CREQ-132409
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyExchangeCalcPubVal(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr);

/* [SWS_Crypto_91010] */
/**********************************************************************************************************************
 *  Crypto_30_LibCv_KeyExchangeCalcSecret()
 *********************************************************************************************************************/
/*! \brief         Calculation of the secret
 *  \details       Calculates the shared secret key for the key exchange with the key material of the key identified
 *                 by the cryptoKeyId and the partner public key. The shared secret key is stored
 *                 as a key element in the same key. The write access right for the destination key element
 *                 must be less than or equal to WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[in]     partnerPublicValuePtr   Holds the pointer to the memory location which contains the partners
 *                                         public value.
 *  \param[in]     partnerPublicValueLength Contains the length of the partners public value in bytes.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *  \trace         CREQ-132408
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyExchangeCalcSecret(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_CertificateParse()
 *********************************************************************************************************************/
/*! \brief         Parse stored certificate
 *  \details       Parses the certificate data stored in the key element CRYPTO_KE_CERT_DATA and fills the key elements
 *                 CRYPTO_KE_CERT_SIGNEDDATA, CRYPTO_KE_CERT_PARSEDPUBLICKEY and CRYPTO_KE_CERT_SIGNATURE
 *  \param[in]     cryptoKeyId              Holds the identifier of the key slot in which the certificate has been
 *                                         stored.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_CertificateParse(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_CertificateVerify()
 *********************************************************************************************************************/
/*! \brief         Certificate verification
 *  \details       Verifies the certificate stored in the key referenced by verifyCryptoKeyId with the
 *                 certificate stored in the key referenced by cryptoKeyId.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used to validate the
 *                                         certificate.
 *  \param[in]     verifyCryptoKeyId       Holds the identifier of the key containing the certificate, which shall be
 *                                         verified.
 *  \param[out]    verifyPtr               Holds a pointer to the memory location which will contain the result of the
 *                                         certificate verification.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_CertificateVerify(
  uint32 cryptoKeyId,
  uint32 verifyCryptoKeyId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) verifyPtr);

# if(CRYPTO_30_LIBCV_SHECMDGETID == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_SheCmdGetId()
 *********************************************************************************************************************/
/*! \brief          Dispatches the She Cmd Get Id job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_SheCmdGetId(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_SHECMDGETID == STD_ON) */

# define CRYPTO_30_LIBCV_STOP_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CRYPTO_30_LIBCV_KEYMANAGEMENT_H */
/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_KeyManagement.h
 *********************************************************************************************************************/
