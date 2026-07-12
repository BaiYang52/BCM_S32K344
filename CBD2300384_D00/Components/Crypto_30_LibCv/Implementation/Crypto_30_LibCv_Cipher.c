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
/*      \file  Crypto_30_LibCv_Cipher.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Implementation for CIPHER Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_CIPHER_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Crypto_30_LibCv.h"
#include "Crypto_30_LibCv_Services.h"
#include "Crypto_30_LibCv_Encrypt.h"
#include "Crypto_30_LibCv_Decrypt.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_AES128_KEY_LENGTH                             CRYPTO_30_LIBCV_AES_BLOCK_SIZE
#define CRYPTO_30_LIBCV_AES128_IV_LENGTH                              CRYPTO_30_LIBCV_AES_BLOCK_SIZE
#define CRYPTO_30_LIBCV_AES192_KEY_LENGTH                             (24u)
#define CRYPTO_30_LIBCV_AES192_IV_LENGTH                              CRYPTO_30_LIBCV_AES_BLOCK_SIZE
#define CRYPTO_30_LIBCV_AES256_KEY_LENGTH                             (32u)
#define CRYPTO_30_LIBCV_AES256_IV_LENGTH                              CRYPTO_30_LIBCV_AES_BLOCK_SIZE
#define CRYPTO_30_LIBCV_AES_MODE_128                                  (0u)
#define CRYPTO_30_LIBCV_AES_MODE_192                                  (1u)
#define CRYPTO_30_LIBCV_AES_MODE_256                                  (2u)

/* RSA decryption mode */
/* normal mode */
#define CRYPTO_30_LIBCV_RSA_DECRYPT_MODE_PRIVATE                      (0u)
/* inverse mode */
#define CRYPTO_30_LIBCV_RSA_DECRYPT_MODE_PUBLIC                       (1u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if ((CRYPTO_30_LIBCV_AESENCRYPT == STD_ON) || (CRYPTO_30_LIBCV_AESDECRYPT == STD_ON))
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherAesKey()
 *********************************************************************************************************************/
/*! \brief          Load AES key and iv.
 *  \details        This function loads the AES key and iv and checks the AES key length.
 *                  The iv is used as counter in CTR mode.
 *  \param[in]      job                Pointer to the job which shall be dispatched.
 *  \param[in,out]  workspace          Pointer to the workspace.
 *  \param[out]     keyIndexPtr        Pointer to the key storage index which holds the key.
 *  \param[out]     ivIndexPtr         Pointer to the key storage index which holds the iv or counter, according to the aes-mode.
 *  \return         E_OK                    Dispatching was successful.
 *                  E_NOT_OK                Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *  \pre            all pointers need to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAesKey(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_WorkSpaceAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) keyIndexPtr,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) ivIndexPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherAesPaddingMode()
 *********************************************************************************************************************/
/*! \brief          Load AES padding mode.
 *  \details        This function loads the AES padding mode.
 *  \param[in]      secondaryAlgorithmFamilyOfJob  secondary algorithm family of the job which shall be dispatched.
 *  \return         AES paddingMode
 *  \pre            all pointers need to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_PaddingMode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAesPaddingMode(
  Crypto_AlgorithmSecondaryFamilyType secondaryAlgorithmFamilyOfJob);
#endif

#if (CRYPTO_30_LIBCV_AESENCRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherAesEncryptStart()
 *********************************************************************************************************************/
/*! \brief          Dispatches the CipherAesEncrypt job for key sizes 128, 192 and 256 bit in operation mode start
 *                  to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in]      job                Pointer to the job which shall be dispatched.
 *  \param[in,out]  workspace          Pointer to the workspace.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *                  workspace has to be a valid Crypto_30_LibCv_WorkSpaceAES workspace.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAesEncryptStart(
  uint32 objectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_WorkSpaceAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherAesEncryptUpdate()
 *********************************************************************************************************************/
/*! \brief          Dispatches the CipherAesEncrypt job for key sizes 128, 192 and 256 bit in operation mode update
 *                  to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in,out]  workspace          Pointer to the workspace.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *                  workspace has to be a valid Crypto_30_LibCv_WorkSpaceAES workspace.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAesEncryptUpdate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_WorkSpaceAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherAesEncryptFinish()
 *********************************************************************************************************************/
/*! \brief          Dispatches the CipherAesEncrypt job for key sizes 128, 192 and 256 bit in operation mode finish
 *                  to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in,out]  workspace          Pointer to the workspace.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *                  workspace has to be a valid Crypto_30_LibCv_WorkSpaceAES workspace.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAesEncryptFinish(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_WorkSpaceAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace);
#endif /* (CRYPTO_30_LIBCV_AESENCRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_AESDECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherAesDecryptStart()
 *********************************************************************************************************************/
/*! \brief          Dispatches the CipherAesDecrypt job for key sizes 128, 192 and 256 bit in operation mode start
 *                  to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in]      job                Pointer to the job which shall be dispatched.
 *  \param[in,out]  workspace          Pointer to the workspace.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *                  workspace has to be a valid Crypto_30_LibCv_WorkSpaceAES workspace.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAesDecryptStart(
  uint32 objectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_WorkSpaceAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherAesDecryptUpdate()
 *********************************************************************************************************************/
/*! \brief          Dispatches the CipherAesDecrypt job for key sizes 128, 192 and 256 bit in operation mode update
 *                  to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in,out]  workspace          Pointer to the workspace.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *                  workspace has to be a valid Crypto_30_LibCv_WorkSpaceAES workspace.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAesDecryptUpdate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_WorkSpaceAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherAesDecryptFinish()
 *********************************************************************************************************************/
/*! \brief          Dispatches the CipherAesDecrypt job for key sizes 128, 192 and 256 bit in operation mode finish
 *                  to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in,out]  workspace          Pointer to the workspace.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *                  workspace has to be a valid Crypto_30_LibCv_WorkSpaceAES workspace.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAesDecryptFinish(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_WorkSpaceAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace);
#endif /* (CRYPTO_30_LIBCV_AESDECRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_ENCRYPT_RSA == STD_ON) || (CRYPTO_30_LIBCV_DECRYPT_RSA == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherRsa_Finish()
 *********************************************************************************************************************/
/*! \brief          Handle operation mode finish for Rsa chipher.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \pre            job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherRsa_Finish(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_ENCRYPT_RSA == STD_ON) || (CRYPTO_30_LIBCV_DECRYPT_RSA == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAPKCS1DECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaPkcs1Decrypt_Start()
 *********************************************************************************************************************/
/*! \brief          Dispatches the RsaPkcs1Decrypt job to the lower layer for operation mode start.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation start, it triggers different stages of the service.
 *  \param[in,out]  workspace          Holds the workspace of the Crypto Driver Object.
 *  \param[in]      job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *  \pre            workspace has to be a valid esLib workspace.
 *                  objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaPkcs1Decrypt_Start(
  P2VAR(Crypto_30_LibCv_WorkSpaceRsaDecrypt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaPkcs1Decrypt_Update()
 *********************************************************************************************************************/
/*! \brief          Dispatches the RsaPkcs1Decrypt job to the lower layer for operation mode start.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation start, it triggers different stages of the service.
 *  \param[in,out]  workspace          Holds the workspace of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            workspace has to be a valid esLib workspace.
 *                  objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaPkcs1Decrypt_Update(
  P2VAR(Crypto_30_LibCv_WorkSpaceRsaDecrypt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_RSAPKCS1DECRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAOAEPSHA1ENCRYPT == STD_ON) || (CRYPTO_30_LIBCV_RSAOAEPSHA256ENCRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherRsaOaepEncrypt_Start()
 *********************************************************************************************************************/
/*! \brief          Handle operation mode start for RsaOaep Encrypt Sha1 and Sha256.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  workspace          Holds the workspace of the Crypto Driver Object.
 *  \param[in]      job                Pointer to the job which shall be dispatched.
 *  \param[in]      secondaryFamily    Holds the secondary algorithm family.
 *  \return         E_OK                   Dispatching was successful.
 *                  E_NOT_OK               Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID Request failed, the key is not valid.
 *  \pre            workspace has to be a valid esLib workspace.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherRsaOaepEncrypt_Start(
  P2VAR(eslt_WorkSpaceRSAOAEPenc, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_AlgorithmSecondaryFamilyType secondaryFamily);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherRsaOaepEncrypt_Update()
 *********************************************************************************************************************/
/*! \brief          Handle operation mode update for RsaOaep Encrypt Sha1 and Sha256.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  workspace          Holds the workspace of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      secondaryFamily    Holds the secondary algorithm family.
 *  \return         E_OK                   Dispatching was successful.
 *                  E_NOT_OK               Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID Request failed, the key is not valid.
 *  \pre            workspace has to be a valid esLib workspace.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherRsaOaepEncrypt_Update(
  P2VAR(eslt_WorkSpaceRSAOAEPenc, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_AlgorithmSecondaryFamilyType secondaryFamily);
#endif /* (CRYPTO_30_LIBCV_RSAOAEPSHA1ENCRYPT == STD_ON) || (CRYPTO_30_LIBCV_RSAOAEPSHA256ENCRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAOAEPSHA1DECRYPT == STD_ON) || (CRYPTO_30_LIBCV_RSAOAEPSHA256DECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherRsaOaepDecrypt_Start()
 *********************************************************************************************************************/
/*! \brief          Handle operation mode start for RsaOaep Decrypt Sha1 and Sha256.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  workspace          Holds the workspace of the Crypto Driver Object.
 *  \param[in]      job                Pointer to the job which shall be dispatched.
 *  \param[in]      secondaryFamily    Holds the secondary algorithm family.
 *  \return         E_OK                   Dispatching was successful.
 *                  E_NOT_OK               Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID Request failed, the key is not valid.
 *  \pre            workspace has to be a valid esLib workspace.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherRsaOaepDecrypt_Start(
  P2VAR(eslt_WorkSpaceRSAOAEPdec, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_AlgorithmSecondaryFamilyType secondaryFamily);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherRsaOaepDecrypt_Update()
 *********************************************************************************************************************/
/*! \brief          Handle operation mode update for RsaOaep Decrypt Sha1 and Sha256.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  workspace          Holds the workspace of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      secondaryFamily    Holds the secondary algorithm family.
 *  \return         E_OK                   Dispatching was successful.
 *                  E_NOT_OK               Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID Request failed, the key is not valid.
 *  \pre            workspace has to be a valid esLib workspace.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherRsaOaepDecrypt_Update(
  P2VAR(eslt_WorkSpaceRSAOAEPdec, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_AlgorithmSecondaryFamilyType secondaryFamily);
#endif /* (CRYPTO_30_LIBCV_RSAOAEPSHA1DECRYPT == STD_ON) || (CRYPTO_30_LIBCV_RSAOAEPSHA256DECRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAOAEPCRTSHA1DECRYPT == STD_ON) || (CRYPTO_30_LIBCV_RSAOAEPCRTSHA256DECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherRsaOaepCrtDecrypt_Start()
 *********************************************************************************************************************/
/*! \brief          Handle operation mode start for RsaOaep Crt Decrypt Sha1 and Sha256.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  workspace          Holds the workspace of the Crypto Driver Object.
 *  \param[in]      job                Pointer to the job which shall be dispatched.
 *  \param[in]      secondaryFamily    Holds the secondary algorithm family.
 *  \return         E_OK                   Dispatching was successful.
 *                  E_NOT_OK               Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID Request failed, the key is not valid.
 *  \pre            workspace has to be a valid esLib workspace.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherRsaOaepCrtDecrypt_Start(
  P2VAR(eslt_WorkSpaceRSACRTOAEPdec, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_AlgorithmSecondaryFamilyType secondaryFamily);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherRsaOaepCrtDecrypt_Update()
 *********************************************************************************************************************/
/*! \brief          Handle operation mode update for RsaOaep Crt Decrypt Sha1 and Sha256.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  workspace          Holds the workspace of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      secondaryFamily    Holds the secondary algorithm family.
 *  \return         E_OK                   Dispatching was successful.
 *                  E_NOT_OK               Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID Request failed, the key is not valid.
 *  \pre            workspace has to be a valid esLib workspace.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherRsaOaepCrtDecrypt_Update(
  P2VAR(eslt_WorkSpaceRSACRTOAEPdec, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_AlgorithmSecondaryFamilyType secondaryFamily);
#endif /* (CRYPTO_30_LIBCV_RSAOAEPCRTSHA1DECRYPT == STD_ON) || (CRYPTO_30_LIBCV_RSAOAEPCRTSHA256DECRYPT == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if ((CRYPTO_30_LIBCV_AESDECRYPT == STD_ON) || (CRYPTO_30_LIBCV_AESENCRYPT == STD_ON))
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchCipherAesKey()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 5 */ /* MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAesKey(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_WorkSpaceAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) keyIndexPtr,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) ivIndexPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 aesKeyLength, aesIvLength;
  Std_ReturnType retVal = E_NOT_OK, localRetVal;
# if ((CRYPTO_30_LIBCV_ENABLE_AES192 != STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 != STD_ON))
  uint8 errorId = CRYPTO_E_NO_ERROR;
# endif

  /* ----- Implementation ------------------------------------------------- */
  /* Load key element from storage. */
  localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndexExtended(job->cryptoKeyId, CRYPTO_KE_CIPHER_KEY, keyIndexPtr, &aesKeyLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE, CRYPTO_30_LIBCV_SHE_SERVICE_ENCRYPT_DECRYPT); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  /* check the key length and differ algorithm */
  if (localRetVal == E_OK)
  {
    if (aesKeyLength == CRYPTO_30_LIBCV_AES128_KEY_LENGTH)
    {
      workspace->mode = CRYPTO_30_LIBCV_AES_MODE_128; /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
      aesIvLength = CRYPTO_30_LIBCV_AES128_IV_LENGTH;
    }
    else if (aesKeyLength == CRYPTO_30_LIBCV_AES192_KEY_LENGTH)
    {
# if (CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON)
      workspace->mode = CRYPTO_30_LIBCV_AES_MODE_192; /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
      aesIvLength = CRYPTO_30_LIBCV_AES192_IV_LENGTH;
# else
      errorId = CRYPTO_E_PARAM_HANDLE;
      localRetVal = E_NOT_OK;
# endif
    }
    else if (aesKeyLength == CRYPTO_30_LIBCV_AES256_KEY_LENGTH)
    {
# if (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)
      workspace->mode = CRYPTO_30_LIBCV_AES_MODE_256; /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
      aesIvLength = CRYPTO_30_LIBCV_AES256_IV_LENGTH;
# else
      errorId = CRYPTO_E_PARAM_HANDLE;
      localRetVal = E_NOT_OK;
# endif

    }
    else
    {
      localRetVal = E_NOT_OK;
    }

    if (localRetVal == E_OK)
    {
      /* Load iv for CBC or CTR mode, iv is used as counter in CTR mode */
      if ((job->jobPrimitiveInfo->primitiveInfo->algorithm.mode == CRYPTO_ALGOMODE_CBC)
       || (job->jobPrimitiveInfo->primitiveInfo->algorithm.mode == CRYPTO_ALGOMODE_CTR))
      {
        retVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndexJob(job->cryptoKeyId, CRYPTO_KE_CIPHER_IV, ivIndexPtr, &aesIvLength, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
      }
      else
      {
        retVal = E_OK;
      }
    }
  }
  else if (localRetVal == CRYPTO_E_KEY_NOT_VALID)
  {
    retVal = CRYPTO_E_KEY_NOT_VALID;
  }
  else
  {
    /* retVal is already set */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (((CRYPTO_30_LIBCV_ENABLE_AES192 != STD_ON)) || (CRYPTO_30_LIBCV_ENABLE_AES256 != STD_ON))
#  if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYPTO_30_LIBCV_MODULE_ID, CRYPTO_30_LIBCV_INSTANCE_ID, CRYPTO_30_LIBCV_SID_PROCESS_JOB, errorId);
  }
#  else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif
# endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchCipherAesPaddingMode()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_PaddingMode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAesPaddingMode(
  Crypto_AlgorithmSecondaryFamilyType secondaryAlgorithmFamilyOfJob)
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_PaddingMode paddingMode = ESL_PM_OFF;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Set AES padding mode from the primitive's secondary family. */
  if (secondaryAlgorithmFamilyOfJob == CRYPTO_ALGOFAM_CUSTOM_PADDING_PKCS7)
  {
    /* esLib uses PKCS5 define for PKCS7 padding */
    paddingMode = ESL_PM_PKCS5;
  }
  else if (secondaryAlgorithmFamilyOfJob == CRYPTO_ALGOFAM_PADDING_PKCS7)
  {
    /* esLib uses PKCS5 define for PKCS7 padding */
    paddingMode = ESL_PM_PKCS5;
  }
  else if (secondaryAlgorithmFamilyOfJob == CRYPTO_ALGOFAM_PADDING_ONEWITHZEROS)
  {
    /* esLib uses zero padding */
    paddingMode = ESL_PM_ONEWITHZEROES;
  }
  else /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
  {
    /* Additional misra statement */
  }

  return paddingMode;
}
#endif /* AES decrypt and encrypt */

#if (CRYPTO_30_LIBCV_AESENCRYPT == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchCipherAesEncryptStart()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAesEncryptStart(
  uint32 objectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_WorkSpaceAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  Crypto_30_LibCv_SizeOfKeyStorageType aesEncryptKeyIndex;
  Crypto_30_LibCv_SizeOfKeyStorageType aesEncryptIvIndex;

  eslt_BlockMode blockMode;
  eslt_PaddingMode paddingMode = ESL_PM_OFF;

  P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) aesEncryptIvPtr = NULL_PTR;

  /* ----- Implementation ------------------------------------------------- */

  /* Init workspace variables */
  Crypto_30_LibCv_SetBufferLength(objectId, 0u); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  Crypto_30_LibCv_SetWrittenLength(objectId, 0u); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

  /* #15 Load key element from storage. */
  retVal = Crypto_30_LibCv_DispatchCipherAesKey(job, workspace, &aesEncryptKeyIndex, &aesEncryptIvIndex); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */

  /* #20 Set AES mode and padding mode. */
  if (retVal == E_OK)
  {
    retVal = E_NOT_OK;

    if (job->jobPrimitiveInfo->primitiveInfo->algorithm.mode == CRYPTO_ALGOMODE_CBC)
    {
      blockMode = ESL_BM_CBC;
      aesEncryptIvPtr = (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(aesEncryptIvIndex);
    }
    else if (job->jobPrimitiveInfo->primitiveInfo->algorithm.mode == CRYPTO_ALGOMODE_CTR)
    {
      blockMode = ESL_BM_CTR;
      aesEncryptIvPtr = (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(aesEncryptIvIndex);
    }
    else
    {
      blockMode = ESL_BM_ECB;
    }

    paddingMode = Crypto_30_LibCv_DispatchCipherAesPaddingMode(job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily);

    /* #25 Initialize algorithm with keys depending on the key size: 256, 192 or 128 (default) bit. */
# if ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON))
    switch (workspace->mode)
    {
#  if (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)
    case CRYPTO_30_LIBCV_AES_MODE_256:
      if (esl_initWorkSpaceHeader(&workspace->wsAES.wsAES256.header, ESL_MAXSIZEOF_WS_AES256, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      {
        retValCv = esl_initEncryptAES256(&workspace->wsAES.wsAES256, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
                                         (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(aesEncryptKeyIndex),
                                         blockMode,
                                         paddingMode,
                                         aesEncryptIvPtr);
      }
      break;
#  endif /* (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON) */

#  if (CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON)
    case CRYPTO_30_LIBCV_AES_MODE_192:
      if (esl_initWorkSpaceHeader(&workspace->wsAES.wsAES192.header, ESL_MAXSIZEOF_WS_AES192, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      {
        retValCv = esl_initEncryptAES192(&workspace->wsAES.wsAES192, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
                                         (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(aesEncryptKeyIndex),
                                         blockMode,
                                         paddingMode,
                                         aesEncryptIvPtr);
      }
      break;
#  endif /* (CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) */

    default:
    /* case CRYPTO_30_LIBCV_AES_MODE_128 */
# endif /* ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)) */
      if (esl_initWorkSpaceHeader(&workspace->wsAES.wsAES128.header, ESL_MAXSIZEOF_WS_AES128, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      {
        retValCv = esl_initEncryptAES128(&workspace->wsAES.wsAES128, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
                                         (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(aesEncryptKeyIndex),
                                         blockMode,
                                         paddingMode,
                                         aesEncryptIvPtr);
      }

# if ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON))
      break;
    }
# endif /* ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)) */

    if (retValCv == ESL_ERC_NO_ERROR)
    {
      retVal = E_OK;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchCipherAesEncryptUpdate()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAesEncryptUpdate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  P2VAR(Crypto_30_LibCv_WorkSpaceAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace)
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_Length outputLength;
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv;

  /* ----- Implementation ------------------------------------------------- */
  Crypto_30_LibCv_SetBufferLength(objectId, *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

  /* Get output length in local variable to omit type mismatches */
  outputLength = (eslt_Length)*(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr);

  /* #45 Process input data depending on the key size: 256, 192 or 128 (default) bit. */
# if ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON))
  switch (workspace->mode)
  {
#  if (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)
  case CRYPTO_30_LIBCV_AES_MODE_256:
    retValCv = esl_encryptAES256(&workspace->wsAES.wsAES256, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                 (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                 (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
                                 (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr);
    break;
#  endif /* (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON) */

#  if (CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON)
  case CRYPTO_30_LIBCV_AES_MODE_192:
    retValCv = esl_encryptAES192(&workspace->wsAES.wsAES192, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
    (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
    (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
    (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr);
    break;
#  endif /* (CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) */

  default:
  /* case CRYPTO_30_LIBCV_AES_MODE_128 */
# endif /* ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)) */
    retValCv = esl_encryptAES128(&workspace->wsAES.wsAES128, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                 (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                 (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
                                 (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr);

# if ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON))
    break;
  }
# endif /* ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)) */

  /* copy back written output length */
  *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr) = outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

  Crypto_30_LibCv_SetWrittenLength(objectId, *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  Crypto_30_LibCv_SetBufferLength(objectId, Crypto_30_LibCv_GetBufferLength(objectId) - Crypto_30_LibCv_GetWrittenLength(objectId)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchCipherAesEncryptFinish()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAesEncryptFinish(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  P2VAR(Crypto_30_LibCv_WorkSpaceAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace)
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_Length outputLength;
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv;

  /* ----- Implementation ------------------------------------------------- */
  /* #75 Reinitialize written length and remaining buffer if UPDATE was not part of this job. */
  if ((job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.mode & CRYPTO_OPERATIONMODE_UPDATE) != CRYPTO_OPERATIONMODE_UPDATE)
  {
    Crypto_30_LibCv_SetWrittenLength(objectId, 0u); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
    Crypto_30_LibCv_SetBufferLength(objectId, *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  }

  /* Get output length in local variable to omit type mismatches */
  outputLength = (eslt_Length)(Crypto_30_LibCv_GetBufferLength(objectId));

  /* #80 Write results in output buffers depending on the key size: 256, 192 or 128 (default) bit. */
# if ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON))
  switch (workspace->mode)
  {
#  if (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)
  case CRYPTO_30_LIBCV_AES_MODE_256:
    retValCv = esl_finalizeEncryptAES256(&workspace->wsAES.wsAES256, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                         (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
                                         (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr[Crypto_30_LibCv_GetWrittenLength(objectId)]);
    break;
#  endif /* (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON) */

#  if (CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON)
  case CRYPTO_30_LIBCV_AES_MODE_192:
    retValCv = esl_finalizeEncryptAES192(&workspace->wsAES.wsAES192, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr[Crypto_30_LibCv_GetWrittenLength(objectId)]);
    break;
#  endif /* (CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) */

  default:
  /* case CRYPTO_30_LIBCV_AES_MODE_128 */
# endif /* ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)) */
    retValCv = esl_finalizeEncryptAES128(&workspace->wsAES.wsAES128, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                         (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
                                         (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr[Crypto_30_LibCv_GetWrittenLength(objectId)]);

# if ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON))
    break;
  }
# endif /* ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)) */

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    /* copy back written output length and add writtenLength */
    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = outputLength + Crypto_30_LibCv_GetWrittenLength(objectId); /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesEncrypt()
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesEncrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(Crypto_30_LibCv_WorkSpaceAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfAesEncrypt(Crypto_30_LibCv_GetAesEncryptIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(Crypto_30_LibCv_WorkSpaceAES)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* #5 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    retVal = Crypto_30_LibCv_DispatchCipherAesEncryptStart(objectId, job, workspace); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    break;

    /* #40 Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    retVal = Crypto_30_LibCv_DispatchCipherAesEncryptUpdate(objectId, job, workspace); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    break;

    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    retVal = Crypto_30_LibCv_DispatchCipherAesEncryptFinish(objectId, job, workspace); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    break;

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    /* Not supported State */
    break;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_AESENCRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_AESDECRYPT == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchCipherAesDecryptStart()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6010, 6030 4 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAesDecryptStart(
  uint32 objectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_WorkSpaceAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  Crypto_30_LibCv_SizeOfKeyStorageType aesDecryptKeyIndex;
  Crypto_30_LibCv_SizeOfKeyStorageType aesDecryptIvIndex;

  eslt_BlockMode blockMode;
  eslt_PaddingMode paddingMode = ESL_PM_OFF;
  P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) aesDecryptIvPtr = NULL_PTR;

  /* ----- Implementation ------------------------------------------------- */
  /* Init workspace variables */
  Crypto_30_LibCv_SetWrittenLength(objectId, 0u); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  Crypto_30_LibCv_SetBufferLength(objectId, 0u); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

  /* #15 Load key element from storage. */
  retVal = Crypto_30_LibCv_DispatchCipherAesKey(job, workspace, &aesDecryptKeyIndex, &aesDecryptIvIndex); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */

  /* #20 Set AES mode and padding mode. */
  if (retVal == E_OK)
  {
    retVal = E_NOT_OK;
    if (job->jobPrimitiveInfo->primitiveInfo->algorithm.mode == CRYPTO_ALGOMODE_CBC)
    {
      blockMode = ESL_BM_CBC;
      aesDecryptIvPtr = (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(aesDecryptIvIndex);
    }
    else if (job->jobPrimitiveInfo->primitiveInfo->algorithm.mode == CRYPTO_ALGOMODE_CTR)
    {
      blockMode = ESL_BM_CTR;
      aesDecryptIvPtr = (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(aesDecryptIvIndex);
    }
    else
    {
      blockMode = ESL_BM_ECB;
    }

    paddingMode = Crypto_30_LibCv_DispatchCipherAesPaddingMode(job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily);

    /* #25 Initialize algorithm with keys depending on the key size: 256, 192 or 128 (default) bit. */
# if ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON))
    switch (workspace->mode)
    {
#  if (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)
    case CRYPTO_30_LIBCV_AES_MODE_256:
      if (esl_initWorkSpaceHeader(&(workspace->wsAES.wsAES256.header), ESL_MAXSIZEOF_WS_AES256, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      {
        retValCv = esl_initDecryptAES256(&workspace->wsAES.wsAES256, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(aesDecryptKeyIndex),
          blockMode,
          paddingMode,
          aesDecryptIvPtr);
      }
      break;
#  endif /* (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON) */

#  if (CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON)
    case CRYPTO_30_LIBCV_AES_MODE_192:
      if (esl_initWorkSpaceHeader(&(workspace->wsAES.wsAES192.header), ESL_MAXSIZEOF_WS_AES192, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      {
        retValCv = esl_initDecryptAES192(&workspace->wsAES.wsAES192, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(aesDecryptKeyIndex),
          blockMode,
          paddingMode,
          aesDecryptIvPtr);
      }
      break;
#  endif /* (CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) */

    default:
    /* case CRYPTO_30_LIBCV_AES_MODE_128 */
# endif /* ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)) */
      if (esl_initWorkSpaceHeader(&(workspace->wsAES.wsAES128.header), ESL_MAXSIZEOF_WS_AES128, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      {
        retValCv = esl_initDecryptAES128(&workspace->wsAES.wsAES128, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
                                         (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(aesDecryptKeyIndex),
                                         blockMode,
                                         paddingMode,
                                         aesDecryptIvPtr);
      }

# if ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON))
      break;
    }
# endif /* ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)) */

    if (retValCv == ESL_ERC_NO_ERROR)
    {
      retVal = E_OK;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchCipherAesDecryptUpdate()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAesDecryptUpdate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  P2VAR(Crypto_30_LibCv_WorkSpaceAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace)
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_Length outputLength;
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv;

  /* ----- Implementation ------------------------------------------------- */
  /* Get output length in local variable to omit type mismatches */
  outputLength = (eslt_Length)*(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr);

  /* #45 Process input data depending on the key size: 256, 192 or 128 (default) bit. */
# if ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON))
  switch (workspace->mode)
  {
#  if (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)
  case CRYPTO_30_LIBCV_AES_MODE_256:
    retValCv = esl_decryptAES256(&workspace->wsAES.wsAES256, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
      (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr);
    break;
#  endif /* (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON) */

#  if (CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON)
  case CRYPTO_30_LIBCV_AES_MODE_192:
    retValCv = esl_decryptAES192(&workspace->wsAES.wsAES192, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
      (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr);
    break;
#  endif /* (CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) */

  default:
  /* case CRYPTO_30_LIBCV_AES_MODE_128 */
# endif /* ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)) */
    retValCv = esl_decryptAES128(&workspace->wsAES.wsAES128, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
      (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr);

# if ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON))
    break;
  }
# endif /* ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)) */

  Crypto_30_LibCv_SetBufferLength(objectId, *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr) - outputLength); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  Crypto_30_LibCv_SetWrittenLength(objectId, outputLength); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

  /* copy back written output length */
  *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr) = outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchCipherAesDecryptFinish()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAesDecryptFinish(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  P2VAR(Crypto_30_LibCv_WorkSpaceAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace)
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_Length outputLength;
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv;

  /* ----- Implementation ------------------------------------------------- */
  /* #75 Reinitialize written length and remaining buffer if UPDATE was not part of this job. */
  if ((job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.mode & CRYPTO_OPERATIONMODE_UPDATE) != CRYPTO_OPERATIONMODE_UPDATE)
  {
    Crypto_30_LibCv_SetWrittenLength(objectId, 0u); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
    Crypto_30_LibCv_SetBufferLength(objectId, *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  }

  /* Get output length in local variable to omit type mismatches */
  outputLength = (eslt_Length)Crypto_30_LibCv_GetBufferLength(objectId);

  /* #80 Write results in output buffers depending on the key size: 256, 192 or 128 (default) bit. */
# if ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON))
  switch (workspace->mode)
  {
#  if (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)
  case CRYPTO_30_LIBCV_AES_MODE_256:
    retValCv = esl_finalizeDecryptAES256(&workspace->wsAES.wsAES256, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr[Crypto_30_LibCv_GetWrittenLength(objectId)]);
    break;
#  endif /* (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON) */

#  if (CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON)
  case CRYPTO_30_LIBCV_AES_MODE_192:
    retValCv = esl_finalizeDecryptAES192(&workspace->wsAES.wsAES192, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr[Crypto_30_LibCv_GetWrittenLength(objectId)]);
    break;
#  endif /* (CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) */

  default:
  /* case CRYPTO_30_LIBCV_AES_MODE_128 */
# endif /* ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)) */
    retValCv = esl_finalizeDecryptAES128(&workspace->wsAES.wsAES128, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr[Crypto_30_LibCv_GetWrittenLength(objectId)]);

# if ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON))
    break;
  }
# endif /* ((CRYPTO_30_LIBCV_ENABLE_AES192 == STD_ON) || (CRYPTO_30_LIBCV_ENABLE_AES256 == STD_ON)) */

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = outputLength + Crypto_30_LibCv_GetWrittenLength(objectId); /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesDecrypt()
 **********************************************************************************************************************/
/*!
 * #
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesDecrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(Crypto_30_LibCv_WorkSpaceAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfAesDecrypt(Crypto_30_LibCv_GetAesDecryptIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(Crypto_30_LibCv_WorkSpaceAES)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* ----- Implementation ------------------------------------------------- */
  /* #5 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    retVal = Crypto_30_LibCv_DispatchCipherAesDecryptStart(objectId, job, workspace); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    break;

    /* #40 Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    retVal = Crypto_30_LibCv_DispatchCipherAesDecryptUpdate(objectId, job, workspace); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    break;

    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    retVal = Crypto_30_LibCv_DispatchCipherAesDecryptFinish(objectId, job, workspace); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    break;

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    /* Not supported State */
    break;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_AESDECRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAPKCS1ENCRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaPkcs1Encrypt()
 *********************************************************************************************************************/
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
 */

FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaPkcs1Encrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceRSAenc, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfRsaPkcs1Encrypt(Crypto_30_LibCv_GetRsaPkcs1EncryptIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceRSAenc)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* #5 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      Crypto_30_LibCv_KeyElementGetType keyElements[2];

      Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_CUSTOM_RSA_MODULUS); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
      Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 1u, CRYPTO_KE_CUSTOM_RSA_PUBLIC_EXPONENT); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

      /* #15 Initialize workspace header. */
      if (esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_RSA_ENC, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      {
        /* Check if key element is accessible */
        /* #20 Read key elements. */
        retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 2u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
        if (retVal == E_OK)
        {
          /* #25 Initialize algorithm. */
          retVal = E_NOT_OK;
          retValCv = esl_initEncryptRSA_V15(workspace,
                                            /* Modulus */
                                            (eslt_Length)keyElements[0u].keyElementLength,
                                            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex),
                                            /* Public Exponent */
                                            (eslt_Length)keyElements[1u].keyElementLength,
                                            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1u].keyElementIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
        }
      }
      break;
    }

    /* #40 Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* #45 Update algorithm and save result. */
      eslt_Length outputLength;
      outputLength = (eslt_Length)(*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr);
      /* #75 Copy Result to output PTR. */

      retValCv = esl_encryptRSA_V15(workspace,
                                    (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                    (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                    (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
                                    (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */

      *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = (uint32)outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      break;
    }

    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retVal = E_OK;
      Crypto_30_LibCv_DispatchCipherRsa_Finish(job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_RSAPKCS1ENCRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAPKCS1DECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaPkcs1Decrypt_Start()
 *********************************************************************************************************************/
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaPkcs1Decrypt_Start(
  P2VAR(Crypto_30_LibCv_WorkSpaceRsaDecrypt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  Std_ReturnType retValKey;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  Crypto_30_LibCv_KeyElementGetType keyElements[2];

  /* ----- Implementation ------------------------------------------------ */

  /* # Handle operationmode START. */

  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_CUSTOM_RSA_MODULUS); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 1u, CRYPTO_KE_CUSTOM_RSA_PRIVATE_EXPONENT); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  /* Check if key element is accessible */
  /* # Read key elements. */
  retValKey = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 1u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  if (retValKey == E_OK)
  {
    retValKey = Crypto_30_LibCv_Local_KeyElementGetStorageIndexJobOptional(job->cryptoKeyId, CRYPTO_KE_CUSTOM_RSA_PRIVATE_EXPONENT, &keyElements[1u].keyElementIndex, &keyElements[1u].keyElementLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    /* # If private exponent is not available use check if decryption with public exponent is used. */
    if (retValKey == CRYPTO_E_KEY_NOT_AVAILABLE)
    {
      workspace->decryptionMode = CRYPTO_30_LIBCV_RSA_DECRYPT_MODE_PUBLIC; /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
      retValKey = Crypto_30_LibCv_Local_KeyElementGetStorageIndexJobOptional(job->cryptoKeyId, CRYPTO_KE_CUSTOM_RSA_PUBLIC_EXPONENT, &keyElements[1u].keyElementIndex, &keyElements[1u].keyElementLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    }
    else
    {
      workspace->decryptionMode = CRYPTO_30_LIBCV_RSA_DECRYPT_MODE_PRIVATE; /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
    }

    if (retValKey == CRYPTO_E_KEY_NOT_AVAILABLE)
    {
      retValKey = E_NOT_OK;
    }
  }

  if (retValKey == E_OK)
  {
    /* # Initialize workspace header. */
    if (esl_initWorkSpaceHeader(&(workspace->wsRsa.header), ESL_MAXSIZEOF_WS_RSA_DEC, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
    {

      /* # Initialize algorithm. */
      retValCv = esl_initDecryptRSA_V15(&(workspace->wsRsa),
                                        /* Modulus */
                                        (eslt_Length)keyElements[0u].keyElementLength,
                                        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex),
                                        /* Private/Public Exponent */
                                        (eslt_Length)keyElements[1u].keyElementLength,
                                        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1u].keyElementIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
    }
  }
  else
  {
    retVal = retValKey;
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaPkcs1Decrypt_Update()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaPkcs1Decrypt_Update(
  P2VAR(Crypto_30_LibCv_WorkSpaceRsaDecrypt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_ErrorCode retValCv;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------ */
  /* # Handle operationmode UPDATE. */

  /* # Update algorithm and save result. */
  eslt_Length outputLength;
  outputLength = (eslt_Length)(*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr);
  /* # Copy Result to output PTR. */
  /* # If private exponent is not available use check if decryption with public exponent is used. */
  if(workspace->decryptionMode == CRYPTO_30_LIBCV_RSA_DECRYPT_MODE_PRIVATE)
  {
    retValCv = esl_decryptRSA_V15(&(workspace->wsRsa),
                                  (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                  (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
                                  (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
  }
  else
  {
    retValCv = esl_decryptPubRSA_V15(&(workspace->wsRsa),
                                     (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                     (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
                                     (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
  }

  *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = (uint32)outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaPkcs1Decrypt()
 *********************************************************************************************************************/
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

FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaPkcs1Decrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  P2VAR(Crypto_30_LibCv_WorkSpaceRsaDecrypt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfRsaPkcs1Decrypt(Crypto_30_LibCv_GetRsaPkcs1DecryptIdxOfObjectInfo(objectId));

  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(Crypto_30_LibCv_WorkSpaceRsaDecrypt)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_Dispatch_RsaPkcs1Decrypt_Start(workspace, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_Dispatch_RsaPkcs1Decrypt_Update(workspace, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */

      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retVal = E_OK;
      Crypto_30_LibCv_DispatchCipherRsa_Finish(job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_RSAPKCS1DECRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_ENCRYPT_RSA == STD_ON) || (CRYPTO_30_LIBCV_DECRYPT_RSA == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherRsa_Finish()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherRsa_Finish(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_3 */
{
  if (job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = 0u; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
  }
}
#endif /* (CRYPTO_30_LIBCV_ENCRYPT_RSA == STD_ON) || (CRYPTO_30_LIBCV_DECRYPT_RSA == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAOAEPSHA1ENCRYPT == STD_ON) || (CRYPTO_30_LIBCV_RSAOAEPSHA256ENCRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherRsaOaepEncrypt_Start()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherRsaOaepEncrypt_Start(
  P2VAR(eslt_WorkSpaceRSAOAEPenc, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_AlgorithmSecondaryFamilyType secondaryFamily)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  Crypto_30_LibCv_KeyElementGetType keyElements[2];

  /* # Initialize workspace header. */
  if (esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_RSA_OAEP_ENC, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
  {
    /* Check if key element is accessible */
    /* # Read key elements. */

    Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_CUSTOM_RSA_MODULUS); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 1u, CRYPTO_KE_CUSTOM_RSA_PUBLIC_EXPONENT); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 2u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    if (retVal == E_OK)
    {
      /* # Initialize algorithm. */
      switch (secondaryFamily)
      {
# if (CRYPTO_30_LIBCV_RSAOAEPSHA1ENCRYPT == STD_ON)
        case CRYPTO_ALGOFAM_SHA1:
        {
          retValCv = esl_initEncryptRSASHA1_OAEP(workspace,
                                                 /* Modulus */
                                                 (eslt_Length)keyElements[0].keyElementLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0].keyElementIndex),
                                                 /* Public Exponent */
                                                 (eslt_Length)keyElements[1].keyElementLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1].keyElementIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
          break;
        }
# endif
# if (CRYPTO_30_LIBCV_RSAOAEPSHA256ENCRYPT == STD_ON)
        case CRYPTO_ALGOFAM_SHA2_256:
        {
          retValCv = esl_initEncryptRSASHA256_OAEP(workspace,
                                                   (eslt_Length)keyElements[0].keyElementLength,
                                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0].keyElementIndex),
                                                   (eslt_Length)keyElements[1].keyElementLength,
                                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1].keyElementIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
          break;
        }
# endif

        default:   /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
        break;
      }

      if (retValCv != ESL_ERC_NO_ERROR)
      {
        retVal = E_NOT_OK;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherRsaOaepEncrypt_Update()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherRsaOaepEncrypt_Update(
  P2VAR(eslt_WorkSpaceRSAOAEPenc, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_AlgorithmSecondaryFamilyType secondaryFamily)
{
  Std_ReturnType retVal = E_NOT_OK, localRetVal;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  uint32 labelLength = 0;
  P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) labelPtr = NULL_PTR;
  Crypto_30_LibCv_SizeOfKeyStorageType labelIndex;
  boolean process = FALSE;

  /* # Update algorithm and save result. */
  eslt_Length outputLength;
  outputLength = (eslt_Length)(*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr);

  /* # Check if label is available. */
  localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndexJobOptional(job->cryptoKeyId, CRYPTO_KE_CUSTOM_LABEL, &labelIndex, &labelLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  if (localRetVal == E_OK)
  {
    labelPtr = (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(labelIndex);
    process = TRUE;
  }
  else if (localRetVal == CRYPTO_E_KEY_NOT_AVAILABLE)
  {
    process = TRUE;
  }
  else
  {
    retVal = localRetVal;
  }

  if (process == TRUE)
  {

    /* # Execute algorithm calculation. */
    switch (secondaryFamily)
    {
# if (CRYPTO_30_LIBCV_RSAOAEPSHA1ENCRYPT == STD_ON)
      case CRYPTO_ALGOFAM_SHA1:
      {
        retValCv = esl_encryptRSASHA1_OAEP_Label(workspace,
                                                 (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                                 (eslt_Length)labelLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) labelPtr,
                                                 (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
                                                 (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
        break;
      }
# endif
# if (CRYPTO_30_LIBCV_RSAOAEPSHA256ENCRYPT == STD_ON)
      case CRYPTO_ALGOFAM_SHA2_256:
      {
        retValCv = esl_encryptRSASHA256_OAEP_Label(workspace,
                                                   (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                                   (eslt_Length)labelLength,
                                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) labelPtr,
                                                   (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
                                                   (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
        break;
      }
# endif

      default:   /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
      break;
    }

    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = (uint32)outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

    if (retValCv == ESL_ERC_NO_ERROR)
    {
      retVal = E_OK;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_RSAOAEPSHA1ENCRYPT == STD_ON) || (CRYPTO_30_LIBCV_RSAOAEPSHA256ENCRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAOAEPSHA1ENCRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaOaepSha1Encrypt()
 *********************************************************************************************************************/
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

FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaOaepSha1Encrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  Crypto_AlgorithmSecondaryFamilyType secAlgoFam = job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily;

  P2VAR(eslt_WorkSpaceRSAOAEPenc, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace;

  workspace = Crypto_30_LibCv_GetWorkspaceOfRsaOaepSha1Encrypt(Crypto_30_LibCv_GetRsaOaepSha1EncryptIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceRSAOAEPenc)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* #5 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchCipherRsaOaepEncrypt_Start(workspace, job, secAlgoFam); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      break;
    }

    /* #40 Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_DispatchCipherRsaOaepEncrypt_Update(workspace, job, secAlgoFam); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      break;
    }

    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      Crypto_30_LibCv_DispatchCipherRsa_Finish(job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      retVal = E_OK;
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_RSAOAEPSHA1ENCRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAOAEPSHA256ENCRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaOaepSha256Encrypt()
 *********************************************************************************************************************/
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

FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaOaepSha256Encrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  Crypto_AlgorithmSecondaryFamilyType secAlgoFam = job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily;

  P2VAR(eslt_WorkSpaceRSAOAEPenc, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace;

  workspace = Crypto_30_LibCv_GetWorkspaceOfRsaOaepSha256Encrypt(Crypto_30_LibCv_GetRsaOaepSha256EncryptIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceRSAOAEPenc)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* #5 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchCipherRsaOaepEncrypt_Start(workspace, job, secAlgoFam); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      break;
    }

    /* #40 Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_DispatchCipherRsaOaepEncrypt_Update(workspace, job, secAlgoFam); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      break;
    }

    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      Crypto_30_LibCv_DispatchCipherRsa_Finish(job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      retVal = E_OK;
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_RSAOAEPSHA256ENCRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAOAEPSHA1DECRYPT == STD_ON) || (CRYPTO_30_LIBCV_RSAOAEPSHA256DECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherRsaOaepDecrypt_Start()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherRsaOaepDecrypt_Start(
  P2VAR(eslt_WorkSpaceRSAOAEPdec, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_AlgorithmSecondaryFamilyType secondaryFamily)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  Crypto_30_LibCv_KeyElementGetType keyElements[2];

  /* # Initialize workspace header. */
  if (esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_RSA_OAEP_DEC, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
  {
    /* Check if key element is accessible */
    /* # Read key elements. */
    Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_CUSTOM_RSA_MODULUS); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 1u, CRYPTO_KE_CUSTOM_RSA_PRIVATE_EXPONENT); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 2u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    if (retVal == E_OK)
    {
      /* # Initialize algorithm according to secondaryFamily. */
      switch (secondaryFamily)
      {
# if (CRYPTO_30_LIBCV_RSAOAEPSHA1DECRYPT == STD_ON)
        case CRYPTO_ALGOFAM_SHA1:
        {
          retValCv = esl_initDecryptRSASHA1_OAEP(workspace,
                                                 /* Modulus */
                                                 (eslt_Length)keyElements[0].keyElementLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0].keyElementIndex),
                                                 /* Private Exponent */
                                                 (eslt_Length)keyElements[1].keyElementLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1].keyElementIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
          break;
        }
# endif
# if (CRYPTO_30_LIBCV_RSAOAEPSHA256DECRYPT == STD_ON)
        case CRYPTO_ALGOFAM_SHA2_256:
        {
          retValCv = esl_initDecryptRSASHA256_OAEP(workspace,
                                                   (eslt_Length)keyElements[0].keyElementLength,
                                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0].keyElementIndex),
                                                   (eslt_Length)keyElements[1].keyElementLength,
                                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1].keyElementIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
          break;
        }
# endif

        default:   /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
        break;
      }

      if (retValCv != ESL_ERC_NO_ERROR)
      {
        retVal = E_NOT_OK;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherRsaOaepDecrypt_Update()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherRsaOaepDecrypt_Update(
  P2VAR(eslt_WorkSpaceRSAOAEPdec, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_AlgorithmSecondaryFamilyType secondaryFamily)
{
  Std_ReturnType retVal = E_NOT_OK, localRetVal;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  uint32 labelLength = 0;
  P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) labelPtr = NULL_PTR;
  Crypto_30_LibCv_SizeOfKeyStorageType labelIndex;
  boolean process = FALSE;

  /* # Update algorithm and save result. */
  eslt_Length outputLength;
  outputLength = (eslt_Length)(*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr);

  /* # Check if label is available. */
  localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndexJobOptional(job->cryptoKeyId, CRYPTO_KE_CUSTOM_LABEL, &labelIndex, &labelLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  if (localRetVal == E_OK)
  {
    labelPtr = (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(labelIndex);
    process = TRUE;
  }
  else if (localRetVal == CRYPTO_E_KEY_NOT_AVAILABLE)
  {
    process = TRUE;
  }
  else
  {
    retVal = localRetVal;
  }

  if (process == TRUE)
  {
    /* # Execute algorithm calculation. */
    switch (secondaryFamily)
    {
# if (CRYPTO_30_LIBCV_RSAOAEPSHA1DECRYPT == STD_ON)
      case CRYPTO_ALGOFAM_SHA1:
      {
        retValCv = esl_decryptRSASHA1_OAEP_Label(workspace,
                                                 (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                                 (eslt_Length)labelLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) labelPtr,
                                                 (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
                                                 (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS_AND_NULL_PTR */
        break;
      }
# endif
# if (CRYPTO_30_LIBCV_RSAOAEPSHA256DECRYPT == STD_ON)
      case CRYPTO_ALGOFAM_SHA2_256:
      {
        retValCv = esl_decryptRSASHA256_OAEP_Label(workspace,
                                                   (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                                   (eslt_Length)labelLength,
                                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) labelPtr,
                                                   (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
                                                   (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS_AND_NULL_PTR */
        break;
      }
# endif

      default:   /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
      break;
    }

    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = (uint32)outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

    if (retValCv == ESL_ERC_NO_ERROR)
    {
      retVal = E_OK;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_RSAOAEPSHA1DECRYPT == STD_ON) || (CRYPTO_30_LIBCV_RSAOAEPSHA256DECRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAOAEPSHA1DECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaOaepSha1Decrypt()
 *********************************************************************************************************************/
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

FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaOaepSha1Decrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  Crypto_AlgorithmSecondaryFamilyType secAlgoFam = job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily;

  P2VAR(eslt_WorkSpaceRSAOAEPdec, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace;

  workspace = Crypto_30_LibCv_GetWorkspaceOfRsaOaepSha1Decrypt(Crypto_30_LibCv_GetRsaOaepSha1DecryptIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceRSAOAEPdec)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchCipherRsaOaepDecrypt_Start(workspace, job, secAlgoFam); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_DispatchCipherRsaOaepDecrypt_Update(workspace, job, secAlgoFam); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      Crypto_30_LibCv_DispatchCipherRsa_Finish(job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      retVal = E_OK;
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_RSAOAEPSHA1DECRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAOAEPSHA256DECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaOaepSha256Decrypt()
 *********************************************************************************************************************/
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

FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaOaepSha256Decrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  Crypto_AlgorithmSecondaryFamilyType secAlgoFam = job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily;

  P2VAR(eslt_WorkSpaceRSAOAEPdec, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace;

  workspace = Crypto_30_LibCv_GetWorkspaceOfRsaOaepSha256Decrypt(Crypto_30_LibCv_GetRsaOaepSha256DecryptIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceRSAOAEPdec)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchCipherRsaOaepDecrypt_Start(workspace, job, secAlgoFam); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_DispatchCipherRsaOaepDecrypt_Update(workspace, job, secAlgoFam); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      Crypto_30_LibCv_DispatchCipherRsa_Finish(job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      retVal = E_OK;
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_RSAOAEPSHA256DECRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAOAEPCRTSHA1DECRYPT == STD_ON) || (CRYPTO_30_LIBCV_RSAOAEPCRTSHA256DECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherRsaOaepCrtDecrypt_Start()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherRsaOaepCrtDecrypt_Start(
  P2VAR(eslt_WorkSpaceRSACRTOAEPdec, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_AlgorithmSecondaryFamilyType secondaryFamily)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  Crypto_30_LibCv_KeyElementGetType keyElements[5];

  /* # Initialize workspace header. */
  if (esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_RSA_CRT_OAEP_DEC, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
  {
    /* Check if key element is accessible */
    /* # Read key elements. */
    Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_CUSTOM_RSA_PRIME_P); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 1u, CRYPTO_KE_CUSTOM_RSA_PRIME_Q); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 2u, CRYPTO_KE_CUSTOM_RSA_EXPONENT_DP); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 3u, CRYPTO_KE_CUSTOM_RSA_EXPONENT_DQ); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 4u, CRYPTO_KE_CUSTOM_RSA_INVERSE_QI); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 5u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    if (retVal == E_OK)
    {
      /* # Initialize algorithm. */
      switch (secondaryFamily)
      {
# if (CRYPTO_30_LIBCV_RSAOAEPCRTSHA1DECRYPT == STD_ON)
        case CRYPTO_ALGOFAM_SHA1:
        {
          retValCv = esl_initDecryptRSACRTSHA1_OAEP(
            workspace,
            /* Prime p */
            (eslt_Length)keyElements[0].keyElementLength,
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0].keyElementIndex),
            /* Prime q */
            (eslt_Length)keyElements[1].keyElementLength,
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1].keyElementIndex),
            /* Exponent  dP */
            (eslt_Length)keyElements[2].keyElementLength,
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[2].keyElementIndex),
            /* Exponent  dQ */
            (eslt_Length)keyElements[3].keyElementLength,
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[3].keyElementIndex),
            /* Inverse  qInv */
            (eslt_Length)keyElements[4].keyElementLength,
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[4].keyElementIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
          break;
        }
# endif
# if (CRYPTO_30_LIBCV_RSAOAEPCRTSHA256DECRYPT == STD_ON)
        case CRYPTO_ALGOFAM_SHA2_256:
        {
          retValCv = esl_initDecryptRSACRTSHA256_OAEP(
            workspace,
            /* Prime p */
            (eslt_Length)keyElements[0].keyElementLength,
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0].keyElementIndex),
            /* Prime q */
            (eslt_Length)keyElements[1].keyElementLength,
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1].keyElementIndex),
            /* Exponent  dP */
            (eslt_Length)keyElements[2].keyElementLength,
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[2].keyElementIndex),
            /* Exponent  dQ */
            (eslt_Length)keyElements[3].keyElementLength,
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[3].keyElementIndex),
            /* Inverse  qInv */
            (eslt_Length)keyElements[4].keyElementLength,
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[4].keyElementIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
          break;
        }
# endif

        default:   /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
        break;
      }

      if (retValCv != ESL_ERC_NO_ERROR)
      {
        retVal = E_NOT_OK;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherRsaOaepCrtDecrypt_Update()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherRsaOaepCrtDecrypt_Update(
  P2VAR(eslt_WorkSpaceRSACRTOAEPdec, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_AlgorithmSecondaryFamilyType secondaryFamily)
{
  Std_ReturnType retVal = E_NOT_OK, localRetVal;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  uint32 labelLength = 0;
  P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) labelPtr = NULL_PTR;
  Crypto_30_LibCv_SizeOfKeyStorageType labelIndex;
  boolean process = FALSE;

  /* # Update algorithm and save result. */
  eslt_Length outputLength;
  outputLength = (eslt_Length)(*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr);

  /* # Check if label is available. */
  localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndexJobOptional(job->cryptoKeyId, CRYPTO_KE_CUSTOM_LABEL, &labelIndex, &labelLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  if (localRetVal == E_OK)
  {
    labelPtr = (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(labelIndex);
    process = TRUE;
  }
  else if (localRetVal == CRYPTO_E_KEY_NOT_AVAILABLE)
  {
    process = TRUE;
  }
  else
  {
    retVal = localRetVal;
  }

  if (process == TRUE)
  {
    /* # Execute algorithm calculation. */
    switch (secondaryFamily)
    {
# if (CRYPTO_30_LIBCV_RSAOAEPCRTSHA1DECRYPT == STD_ON)
      case CRYPTO_ALGOFAM_SHA1:
      {
        retValCv = esl_decryptRSACRTSHA1_OAEP_Label(workspace,
                                                    (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                                    (eslt_Length)labelLength,
                                                    (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) labelPtr,
                                                    (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
                                                    (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS_AND_NULL_PTR */
        break;
      }
# endif
# if (CRYPTO_30_LIBCV_RSAOAEPCRTSHA256DECRYPT == STD_ON)
      case CRYPTO_ALGOFAM_SHA2_256:
      {
        retValCv = esl_decryptRSACRTSHA256_OAEP_Label(workspace,
                                                      (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                                      (eslt_Length)labelLength,
                                                      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) labelPtr,
                                                      (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
                                                      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS_AND_NULL_PTR */
        break;
      }
# endif

      default:   /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
      break;
    }

    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = (uint32)outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

    if (retValCv == ESL_ERC_NO_ERROR)
    {
      retVal = E_OK;
    }
  }
  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_RSAOAEPCRTSHA1DECRYPT == STD_ON) || (CRYPTO_30_LIBCV_RSAOAEPCRTSHA256DECRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAOAEPCRTSHA1DECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaOaepCrtSha1Decrypt()
 *********************************************************************************************************************/
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

FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaOaepCrtSha1Decrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  Crypto_AlgorithmSecondaryFamilyType secAlgoFam = job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily;

  P2VAR(eslt_WorkSpaceRSACRTOAEPdec, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace;
  workspace = Crypto_30_LibCv_GetWorkspaceOfRsaOaepCrtSha1Decrypt(Crypto_30_LibCv_GetRsaOaepCrtSha1DecryptIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceRSACRTOAEPdec)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchCipherRsaOaepCrtDecrypt_Start(workspace, job, secAlgoFam); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_DispatchCipherRsaOaepCrtDecrypt_Update(workspace, job, secAlgoFam); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      Crypto_30_LibCv_DispatchCipherRsa_Finish(job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      retVal = E_OK;
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_RSAOAEPCRTSHA1DECRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAOAEPCRTSHA256DECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaOaepCrtSha256Decrypt()
 *********************************************************************************************************************/
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

FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaOaepCrtSha256Decrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  Crypto_AlgorithmSecondaryFamilyType secAlgoFam = job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily;

  P2VAR(eslt_WorkSpaceRSACRTOAEPdec, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace;
  workspace = Crypto_30_LibCv_GetWorkspaceOfRsaOaepCrtSha256Decrypt(Crypto_30_LibCv_GetRsaOaepCrtSha256DecryptIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceRSACRTOAEPdec)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchCipherRsaOaepCrtDecrypt_Start(workspace, job, secAlgoFam); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_DispatchCipherRsaOaepCrtDecrypt_Update(workspace, job, secAlgoFam); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      Crypto_30_LibCv_DispatchCipherRsa_Finish(job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      retVal = E_OK;
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_RSAOAEPCRTSHA256DECRYPT == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CRYPTO_30_LIBCV_STOP_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_Cipher.c
 *********************************************************************************************************************/
