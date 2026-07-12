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
/*      \file  Crypto_30_LibCv_Aead.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Implementation for AEAD Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_AEAD_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Crypto_30_LibCv.h"
#include "Crypto_30_LibCv_Services.h"
#include "Crypto_30_LibCv_AeadEncrypt.h"
#include "Crypto_30_LibCv_AeadDecrypt.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_AESGCM_MAX_OUT_SIZE                           (16u)
#define CRYPTO_30_LIBCV_AESCCM_MAX_OUT_SIZE                           (16u)

#define CRYPTO_30_LIBCV_CHACHA20POLY1305_KEY_LENGTH                   ESL_SIZEOF_ChaCha20_KEY
#define CRYPTO_30_LIBCV_CHACHA20POLY1305_NONCE_LENGTH                 ESL_SIZEOF_ChaCha20_NONCE
#define CRYPTO_30_LIBCV_POLY1305_OUT_SIZE                             ESL_SIZEOF_Poly1305_TAG

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CRYPTO_30_LIBCV_AESGCMENCRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_AesGcmEncrypt_Start()
 *********************************************************************************************************************/
/*! \brief          Dispatches the AesGcmEncrypt start job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  workspace          Pointer to the esl workspace.
 *  \param[in]      job                Pointer to the job which shall be dispatched.
 *  \return         E_OK                         Dispatching was successful.
 *                  E_NOT_OK                     Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID       Request failed, the key is not valid.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, a key element has the wrong size.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesGcmEncrypt_Start(
  P2VAR(eslt_WorkSpaceGCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_AesGcmEncrypt_Update()
 *********************************************************************************************************************/
/*! \brief          Dispatches the AesGcmEncrypt update job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  workspace          Pointer to the esl workspace.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesGcmEncrypt_Update(
  P2VAR(eslt_WorkSpaceGCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_AesGcmEncrypt_Finish()
 *********************************************************************************************************************/
/*! \brief          Dispatches the AesGcmEncrypt finish job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  workspace          Pointer to the esl workspace.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesGcmEncrypt_Finish(
  P2VAR(eslt_WorkSpaceGCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

#endif /* (CRYPTO_30_LIBCV_AESGCMENCRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_AESGCMDECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_AesGcmDecrypt_Start()
 *********************************************************************************************************************/
/*! \brief          Dispatches the AesGcmDecrypt start job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  workspace                    Pointer to the esl workspace.
 *  \param[in]      job                          Pointer to the job which shall be dispatched.
 *  \return         E_OK                         Dispatching was successful.
 *                  E_NOT_OK                     Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID       Request failed, the key is not valid.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, a key element has the wrong size.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesGcmDecrypt_Start(
  P2VAR(eslt_WorkSpaceGCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_AesGcmDecrypt_Update()
 *********************************************************************************************************************/
/*! \brief          Dispatches the AesGcmDecrypt update job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  workspace          Pointer to the esl workspace.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesGcmDecrypt_Update(
  P2VAR(eslt_WorkSpaceGCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_AesGcmDecrypt_Finish()
 *********************************************************************************************************************/
/*! \brief          Dispatches the AesGcmDecrypt finish job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  workspace          Pointer to the esl workspace.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesGcmDecrypt_Finish(
  P2VAR(eslt_WorkSpaceGCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_AESGCMDECRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_AESCCMENCRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_AesCcmEncrypt_Start()
 *********************************************************************************************************************/
/*! \brief          Dispatches the AesCcmEncrypt start job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service
 *                  by passing the workspace to Crypto_30_LibCv_Dispatch_AesCcm_Start.
 *  \param[in,out]  workspace                    Pointer to the esl workspace.
 *  \param[in]      job                          Pointer to the job which shall be dispatched.
 *  \return         E_OK                         Dispatching was successful.
 *                  E_NOT_OK                     Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID       Request failed, the key is not valid.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, a key element has the wrong size.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesCcmEncrypt_Start(
  P2VAR(eslt_WorkSpaceAESCCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_AesCcmEncrypt_Update()
 *********************************************************************************************************************/
/*! \brief          Dispatches the AesCcmEncrypt update job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  workspace          Pointer to the esl workspace.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesCcmEncrypt_Update(
  P2VAR(eslt_WorkSpaceAESCCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_AesCcmEncrypt_Finish()
 *********************************************************************************************************************/
/*! \brief          Dispatches the AesCcmEncrypt finish job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  workspace          Pointer to the esl workspace.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesCcmEncrypt_Finish(
  P2VAR(eslt_WorkSpaceAESCCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

#endif /* (CRYPTO_30_LIBCV_AESCCMENCRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_AESCCMDECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_AesCcmDecrypt_Start()
 *********************************************************************************************************************/
/*! \brief          Dispatches the AesCcmDecrypt start job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *                  By passing a suitable workspace to Crypto_30_LibCv_Dispatch_AesCcm_Start
 *  \param[in,out]  workspace     Pointer to the esl workspace.
 *  \param[in]      job           Pointer to the job which shall be dispatched.
 *  \return         E_OK                         Dispatching was successful.
 *                  E_NOT_OK                     Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID       Request failed, the key is not valid.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, a key element has the wrong size.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesCcmDecrypt_Start(
  P2VAR(eslt_WorkSpaceAESCCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_AesCcmDecrypt_Update()
 *********************************************************************************************************************/
/*! \brief          Dispatches the AesCcmDecrypt update job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  workspace          Pointer to the esl workspace.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesCcmDecrypt_Update(
  P2VAR(eslt_WorkSpaceAESCCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_AesCcmDecrypt_Finish()
 *********************************************************************************************************************/
/*! \brief          Dispatches the AesCcmDecrypt finish job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  workspace          Pointer to the esl workspace.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesCcmDecrypt_Finish(
  P2VAR(eslt_WorkSpaceAESCCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_AESCCMDECRYPT == STD_ON) */

#if ((CRYPTO_30_LIBCV_AESCCMENCRYPT == STD_ON) || (CRYPTO_30_LIBCV_AESCCMDECRYPT == STD_ON))
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_AesCcm_Start()
 *********************************************************************************************************************/
/*! \brief          Dispatches the AesCcm start job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in,out]  workspace                    Pointer to the esl workspace.
 *  \param[in]      job                          Pointer to the job which shall be dispatched.
 *  \param[in]      authenticationFieldSize      Holds the length of authentication field size.
 *  \return         E_OK                         Dispatching was successful.
 *                  E_NOT_OK                     Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID       Request failed, the key is not valid.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, a key element has the wrong size.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesCcm_Start(
  P2VAR(eslt_WorkSpaceAESCCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  uint8 authenticationFieldSize);
#endif /* (CRYPTO_30_LIBCV_AESCCMENCRYPT == STD_ON) || (CRYPTO_30_LIBCV_AESCCMDECRYPT == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (CRYPTO_30_LIBCV_AESGCMENCRYPT == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesGcmEncrypt_Start()
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesGcmEncrypt_Start(
  P2VAR(eslt_WorkSpaceGCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  Crypto_30_LibCv_KeyElementGetType keyElements[2];

  /* ----- Implementation ------------------------------------------------- */

  /* # Handle operationmode START. */

  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_CIPHER_KEY); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 1u, CRYPTO_KE_CIPHER_IV); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  /* # Initialize workspace header. */
  if (esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_GCM, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
  {
    /* Check if key element is accessible */
    /* # Read key elements. */
    retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 2u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    if (retVal == E_OK)
    {
      /* # Initialize algorithm. */
      retVal = E_NOT_OK;
      retValCv = esl_initEncryptGCM(workspace,
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex),
        (eslt_Length)keyElements[0u].keyElementLength,
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1u].keyElementIndex),
        (eslt_Length)keyElements[1u].keyElementLength); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesGcmEncrypt_Update()
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesGcmEncrypt_Update(
  P2VAR(eslt_WorkSpaceGCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv;

  /* ----- Implementation ------------------------------------------------- */

  /* # Handle operationmode UPDATE. */

  /* Update GCM Calculation */
  Crypto_30_LibCv_SetBufferLength(objectId, *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

  /* # Check if output size is sufficient otherwise return error. */
  if (((job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength + (CRYPTO_30_LIBCV_AESGCM_MAX_OUT_SIZE - 1u)) & 0xFFFFFFF0u) > (*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr))
  {
    retValCv = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
  }
  else
  {
    retValCv = ESL_ERC_NO_ERROR;
  }

  *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = 0u; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

  if ((retValCv == ESL_ERC_NO_ERROR) && (job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength != 0u))
  {
    /* # Process authentic data. */
    retValCv = esl_updateAuthDataEncryptGCM(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr,
      (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength);
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    /* # Process plaintext data. */
    retValCv = esl_updatePlaintextEncryptGCM(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
      (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr,
      (P2VAR(eslt_Size32, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr);

  }

  /* # Store current written length and remaining buffer length for operationmode FINISH. */
  Crypto_30_LibCv_SetWrittenLength(objectId, *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  Crypto_30_LibCv_SetBufferLength(objectId, Crypto_30_LibCv_GetBufferLength(objectId) - Crypto_30_LibCv_GetWrittenLength(objectId)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesGcmEncrypt_Finish()
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesGcmEncrypt_Finish(
  P2VAR(eslt_WorkSpaceGCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv;
  /* buffer holding Auth Tag */
  uint8 authTagTempBuffer[CRYPTO_30_LIBCV_AESGCM_MAX_OUT_SIZE] = { 0u };

  /* ----- Implementation ------------------------------------------------- */

  /* # Handle operationmode FINISH. */
  /* # Reinitialize written length and remaining buffer if UPDATE was not part of this job. */
  if ((job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.mode & CRYPTO_OPERATIONMODE_UPDATE) != CRYPTO_OPERATIONMODE_UPDATE)
  {
    Crypto_30_LibCv_SetWrittenLength(objectId, 0u); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
    Crypto_30_LibCv_SetBufferLength(objectId, *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  }

  /* # Check if output size is sufficient otherwise return error. */
  if (Crypto_30_LibCv_GetBufferLength(objectId) < CRYPTO_30_LIBCV_AESGCM_MAX_OUT_SIZE)
  {
    retValCv = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
  }
  else
  {
    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = Crypto_30_LibCv_GetWrittenLength(objectId); /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

    /* # Write results in output buffers. */
    retValCv = esl_finalizeEncryptGCM(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr[Crypto_30_LibCv_GetWrittenLength(objectId)],
      (P2VAR(eslt_Size32, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))authTagTempBuffer);

     /* # Adapt output size of auth tag. */
    if (*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryOutputLengthPtr > CRYPTO_30_LIBCV_AESGCM_MAX_OUT_SIZE)
    {
      *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryOutputLengthPtr = CRYPTO_30_LIBCV_AESGCM_MAX_OUT_SIZE; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    }
    /* # Copy auth tag to output buffer. */
    Crypto_30_LibCv_CopyData(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryOutputPtr, authTagTempBuffer, *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryOutputLengthPtr); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_AESGCMENCRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_AESGCMDECRYPT == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesGcmDecrypt_Start()
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesGcmDecrypt_Start(
  P2VAR(eslt_WorkSpaceGCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  Crypto_30_LibCv_KeyElementGetType keyElements[2];

  /* ----- Implementation ------------------------------------------------- */

  /* # Handle operationmode START. */
  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_CIPHER_KEY); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 1u, CRYPTO_KE_CIPHER_IV); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  /* # Initialize workspace header. */
  if (esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_GCM, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
  {
    /* Check if key element is accessible */
    /* # Read key elements. */
    retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 2u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    if (retVal == E_OK)
    {
      /* # Initialize algorithm. */
      retVal = E_NOT_OK;
      retValCv = esl_initDecryptGCM(workspace,
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex),
        (eslt_Length)keyElements[0u].keyElementLength,
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1u].keyElementIndex),
        (eslt_Length)keyElements[1u].keyElementLength); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesGcmDecrypt_Update()
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesGcmDecrypt_Update(
  P2VAR(eslt_WorkSpaceGCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv;

  /* ----- Implementation ------------------------------------------------- */

  /* # Handle operationmode UPDATE. */
  /* Update GCM Calculation */
  Crypto_30_LibCv_SetBufferLength(objectId, *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  /* # Check if output size is sufficient otherwise return error. */
  if (((job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength + (CRYPTO_30_LIBCV_AESGCM_MAX_OUT_SIZE - 1u)) & 0xFFFFFFF0u) > (*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr))
  {
    retValCv = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
  }
  else
  {
    retValCv = ESL_ERC_NO_ERROR;
  }

  *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = 0u; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

  /* # Process authentic data. */
  if ((retValCv == ESL_ERC_NO_ERROR) && (job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength != 0u))
  {
    retValCv = esl_updateAuthDataDecryptGCM(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr,
      (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength);
  }

  /* # Process plaintext data. */
  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retValCv = esl_updateCiphertextDecryptGCM(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
      (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr,
      (P2VAR(eslt_Size32, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr);
  }

  /* # Store current written length and remaining buffer length for operationmode FINISH. */
  Crypto_30_LibCv_SetWrittenLength(objectId, *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  Crypto_30_LibCv_SetBufferLength(objectId, Crypto_30_LibCv_GetBufferLength(objectId) - Crypto_30_LibCv_GetWrittenLength(objectId)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesGcmDecrypt_Finish()
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesGcmDecrypt_Finish(
  P2VAR(eslt_WorkSpaceGCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv;

  /* ----- Implementation ------------------------------------------------- */

  /* # Handle operationmode FINISH. */
  /* # Reinitialize written length and remaining buffer if UPDATE was not part of this job. */
  if ((job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.mode & CRYPTO_OPERATIONMODE_UPDATE) != CRYPTO_OPERATIONMODE_UPDATE)
  {
    Crypto_30_LibCv_SetWrittenLength(objectId, 0u); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
    Crypto_30_LibCv_SetBufferLength(objectId, *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  }

  /* # Check if output size is sufficient otherwise return error. */
  if (Crypto_30_LibCv_GetBufferLength(objectId) < CRYPTO_30_LIBCV_AESGCM_MAX_OUT_SIZE)
  {
    retValCv = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
  }
  else
  {
    retValCv = ESL_ERC_NO_ERROR;
  }

  *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = Crypto_30_LibCv_GetWrittenLength(objectId); /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

  /* # Write results in output buffers. */
  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retValCv = esl_finalizeDecryptGCM(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr[Crypto_30_LibCv_GetWrittenLength(objectId)],
      (P2VAR(eslt_Size32, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr,
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.tertiaryInputPtr);
  }

  /* # Check if verification was successful. */
  if (retValCv == ESL_ERC_NO_ERROR)
  {
    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr = CRYPTO_E_VER_OK; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
  }
  else if (retValCv == ESL_ERC_GCM_TAG_VERIFICATION_FAILED)
  {
    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr = CRYPTO_E_VER_NOT_OK; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    retValCv = ESL_ERC_NO_ERROR;
  }
  else
  {
    /* For all other errors do nothing */
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_AESGCMDECRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_AESCCMENCRYPT == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesCcmEncrypt_Start()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesCcmEncrypt_Start(
  P2VAR(eslt_WorkSpaceAESCCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;

  /* ----- Implementation ------------------------------------------------- */

  /* # Call Crypto_30_LibCv_Dispatch_AesCcm_Start */
  retVal = Crypto_30_LibCv_Dispatch_AesCcm_Start(
    workspace,
    job,
    (uint8)*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryOutputLengthPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesCcmEncrypt_Update()
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesCcmEncrypt_Update(
  P2VAR(eslt_WorkSpaceAESCCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_ErrorCode retValCv;
  eslt_Length outlength;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */

  /* # Handle operationmode UPDATE. */

  /* Update CCM Calculation */
  Crypto_30_LibCv_SetBufferLength(objectId, *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

  outlength = (eslt_Length)*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr;

  /* # Process plaintext data. */
  retValCv = esl_updateAESCCMEncrypt(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr,
    (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength,
    (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
    (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
    (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr,
    (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outlength);

  *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = outlength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

  /* # Store current written length and remaining buffer length for operationmode FINISH. */
  Crypto_30_LibCv_SetWrittenLength(objectId, *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  Crypto_30_LibCv_SetBufferLength(objectId, Crypto_30_LibCv_GetBufferLength(objectId) - Crypto_30_LibCv_GetWrittenLength(objectId)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesCcmEncrypt_Finish()
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesCcmEncrypt_Finish(
  P2VAR(eslt_WorkSpaceAESCCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv;
  /* buffer holding Auth Tag */
  eslt_Length outlength, secOutlength;

  /* ----- Implementation ------------------------------------------------- */

  /* # Handle operationmode FINISH. */
  outlength = (eslt_Length)Crypto_30_LibCv_GetBufferLength(objectId); /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
  secOutlength = (eslt_Length)*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryOutputLengthPtr;

  /* # Write results in output buffers. */
  retValCv = esl_finalizeAESCCMEncrypt(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
    (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr[Crypto_30_LibCv_GetWrittenLength(objectId)],
    (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outlength,
    (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryOutputPtr,
    (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&secOutlength);

  *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr += outlength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_AESCCMENCRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_AESCCMDECRYPT == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesCcmDecrypt_Start()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesCcmDecrypt_Start(
  P2VAR(eslt_WorkSpaceAESCCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;

  /* ----- Implementation ------------------------------------------------- */

  /* # Call Crypto_30_LibCv_Dispatch_AesCcm_Start */
  retVal = Crypto_30_LibCv_Dispatch_AesCcm_Start(
    workspace,
    job,
    (uint8)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.tertiaryInputLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesCcmDecrypt_Update()
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesCcmDecrypt_Update(
  P2VAR(eslt_WorkSpaceAESCCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_ErrorCode retValCv;
  eslt_Length outlength;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */

  /* # Handle operationmode UPDATE. */
  /* Update CCM Calculation */
  Crypto_30_LibCv_SetBufferLength(objectId, *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

  outlength = (eslt_Length)*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr;
  /* # Process plaintext data. */
  retValCv = esl_updateAESCCMDecrypt(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr,
    (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength,
    (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
    (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
    (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr,
    (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outlength);

  *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = outlength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

  /* # Store current written length and remaining buffer length for operationmode FINISH. */
  Crypto_30_LibCv_SetWrittenLength(objectId, *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  Crypto_30_LibCv_SetBufferLength(objectId, Crypto_30_LibCv_GetBufferLength(objectId) - Crypto_30_LibCv_GetWrittenLength(objectId)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesCcmDecrypt_Finish()
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesCcmDecrypt_Finish(
  P2VAR(eslt_WorkSpaceAESCCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_ErrorCode retValCv;
  eslt_Length outlength;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */

  /* # Handle operationmode FINISH. */
  outlength = (eslt_Length)Crypto_30_LibCv_GetBufferLength(objectId); /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

  /* # Write results in output buffers. */
  retValCv = esl_finalizeAESCCMDecrypt(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
    (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr[Crypto_30_LibCv_GetWrittenLength(objectId)],
    (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outlength,
    (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.tertiaryInputPtr);

  *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr += outlength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

  /* # Check if verification was successful. */
  if (retValCv == ESL_ERC_NO_ERROR)
  {
    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr = CRYPTO_E_VER_OK; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
  }
  else if (retValCv == ESL_ERC_INCORRECT_TAG)
  {
    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr = CRYPTO_E_VER_NOT_OK; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    retValCv = ESL_ERC_NO_ERROR;
  }
  else
  {
    /* For all other errors do nothing */
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_AESCCMDECRYPT == STD_ON) */

#if ((CRYPTO_30_LIBCV_AESCCMENCRYPT == STD_ON) || (CRYPTO_30_LIBCV_AESCCMDECRYPT == STD_ON))
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesCcm_Start()
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
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesCcm_Start(
  P2VAR(eslt_WorkSpaceAESCCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  uint8 authenticationFieldSize)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  Crypto_30_LibCv_KeyElementGetType keyElements[2];

  /* ----- Implementation ------------------------------------------------- */

  /* # Handle operationmode START. */
  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_CIPHER_KEY); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 1u, CRYPTO_KE_CIPHER_IV); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  /* # Initialize workspace header. */
  if (esl_initWorkSpaceHeader(&(workspace->header), ESL_SIZEOF_WS_AESCCM, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
  {
    /* Check if key element is accessible */
    /* # Read key elements. */
    retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 2u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    if (retVal == E_OK)
    {
      eslt_Byte lengthFieldSize;
      retVal = E_NOT_OK;
      lengthFieldSize = (eslt_Byte)(CRYPTO_30_LIBCV_AESCCM_SUM_OF_NONCE_LENGTH_AND_L - keyElements[1u].keyElementLength);

      /* # Initialize algorithm. */
      retValCv = esl_initAESCCM(workspace,
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex),
        (eslt_Length)keyElements[0u].keyElementLength,
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1u].keyElementIndex),
        (eslt_Length)keyElements[1u].keyElementLength,
        (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength,
        (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
        (eslt_Byte)authenticationFieldSize,
        lengthFieldSize); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_AESCCMENCRYPT == STD_ON) || (CRYPTO_30_LIBCV_AESCCMDECRYPT == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#if (CRYPTO_30_LIBCV_AESGCMENCRYPT == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesGcmEncrypt()
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesGcmEncrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceGCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfAesGcmEncrypt(Crypto_30_LibCv_GetAesGcmEncryptIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceGCM)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_Dispatch_AesGcmEncrypt_Start(workspace, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_Dispatch_AesGcmEncrypt_Update(workspace, objectId, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retVal = Crypto_30_LibCv_Dispatch_AesGcmEncrypt_Finish(workspace, objectId, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
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
#endif /* (CRYPTO_30_LIBCV_AESGCMENCRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_AESGCMDECRYPT == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesGcmDecrypt()
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesGcmDecrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceGCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfAesGcmDecrypt(Crypto_30_LibCv_GetAesGcmDecryptIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceGCM)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_Dispatch_AesGcmDecrypt_Start(workspace, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_Dispatch_AesGcmDecrypt_Update(workspace, objectId, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retVal = Crypto_30_LibCv_Dispatch_AesGcmDecrypt_Finish(workspace, objectId, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
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
#endif /* (CRYPTO_30_LIBCV_AESGCMDECRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_AESCCMENCRYPT == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesCcmEncrypt()
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesCcmEncrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceAESCCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfAesCcmEncrypt(Crypto_30_LibCv_GetAesCcmEncryptIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceAESCCM)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  if (Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_SINGLECALL))
  {
  /* # Distinguish modes. */
    switch (mode)
    {
      /* # Handle operationmode START. */
      case CRYPTO_OPERATIONMODE_START:
      {
        retVal = Crypto_30_LibCv_Dispatch_AesCcmEncrypt_Start(workspace, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
        break;
      }

      /* # Handle operationmode UPDATE. */
      case CRYPTO_OPERATIONMODE_UPDATE:
      {
        retVal = Crypto_30_LibCv_Dispatch_AesCcmEncrypt_Update(workspace, objectId, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
        break;
      }

      /* # Handle operationmode FINISH. */
      case CRYPTO_OPERATIONMODE_FINISH:
      {
        retVal = Crypto_30_LibCv_Dispatch_AesCcmEncrypt_Finish(workspace, objectId, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
        break;
      }

      default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
      {
        /* Not supported State */
        break;
      }
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_AESCCMENCRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_AESCCMDECRYPT == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AesCcmDecrypt()
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesCcmDecrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceAESCCM, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfAesCcmDecrypt(Crypto_30_LibCv_GetAesCcmDecryptIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceAESCCM)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  if (Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_SINGLECALL))
  {
  /* # Distinguish modes. */
    switch (mode)
    {
      /* # Handle operationmode START. */
      case CRYPTO_OPERATIONMODE_START:
      {
        retVal = Crypto_30_LibCv_Dispatch_AesCcmDecrypt_Start(workspace, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
        break;
      }

      /* # Handle operationmode UPDATE. */
      case CRYPTO_OPERATIONMODE_UPDATE:
      {
        retVal = Crypto_30_LibCv_Dispatch_AesCcmDecrypt_Update(workspace, objectId, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
        break;
      }

      /* # Handle operationmode FINISH. */
      case CRYPTO_OPERATIONMODE_FINISH:
      {
        retVal = Crypto_30_LibCv_Dispatch_AesCcmDecrypt_Finish(workspace, objectId, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
        break;
      }

      default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
      {
        /* Not supported State */
        break;
      }
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_AESCCMDECRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_AEADCHACHAPOLY1305ENCRYPT == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AEADChaChaPoly1305Encrypt()
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
 *
 *
 *
 */
/* PRQA S 6030, 6050 4 */ /* MD_MSR_STCYC, MD_MSR_STCAL */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AEADChaChaPoly1305Encrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceChaChaPoly, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfAEADChaChaPoly1305Encrypt(Crypto_30_LibCv_GetAEADChaChaPoly1305EncryptIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceChaChaPoly)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      Crypto_30_LibCv_KeyElementGetType keyElements[2];

      Crypto_30_LibCv_Local_ElementGetterSetIdAndLength(keyElements, 0u, CRYPTO_KE_CIPHER_KEY, CRYPTO_30_LIBCV_CHACHA20POLY1305_KEY_LENGTH); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
      Crypto_30_LibCv_Local_ElementGetterSetIdAndLength(keyElements, 1u, CRYPTO_KE_CIPHER_IV, CRYPTO_30_LIBCV_CHACHA20POLY1305_NONCE_LENGTH); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

      /* # Initialize workspace header. */
      if (esl_initWorkSpaceHeader(&(workspace->header), ESL_SIZEOF_WS_AEAD_ChaCha_Poly, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      {
        /* Check if key element is accessible */
        /* # Read key elements. */
        retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 2u, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
        if (retVal == E_OK)
        {
          /* # Initialize algorithm. */
          retVal = E_NOT_OK;
          retValCv = esl_initEncryptChaChaPoly(workspace,
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex),
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1u].keyElementIndex)); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
        }
      }
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* Get output length in local variable to omit type mismatches */
      eslt_Length outputLength;
      outputLength = (eslt_Length)*(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr);

      /* # Check if output size is sufficient otherwise return error. */
      if ((job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength) > (outputLength))
      {
        retValCv = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
      }
      else
      {
        retValCv = ESL_ERC_NO_ERROR;
      }

      if ((retValCv == ESL_ERC_NO_ERROR) && (job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength != 0u))
      {
        /* # Process authentic data. */
        retValCv = esl_updateAADChaChaPoly(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr,
          (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength);
      }

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* # Process plaintext data. */
        retValCv = esl_updateDataChaChaPoly(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
          (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
          (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr,
          (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength);

        /* copy back written output length */
        *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr) = outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      }

      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      /* buffer holding Auth Tag */
      uint8 authTagTempBuffer[CRYPTO_30_LIBCV_POLY1305_OUT_SIZE] = { 0u };

      /* # Write results in output buffers. */
      retValCv = esl_finalizeChaChaPoly(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
        (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))authTagTempBuffer);

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* # Adapt output size of auth tag. */
        if (*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryOutputLengthPtr > CRYPTO_30_LIBCV_POLY1305_OUT_SIZE)
        {
          *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryOutputLengthPtr = CRYPTO_30_LIBCV_POLY1305_OUT_SIZE; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        }

        /* # Copy auth tag to output buffer. */
        Crypto_30_LibCv_CopyData(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryOutputPtr, authTagTempBuffer, *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryOutputLengthPtr); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      }
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
#endif /* (CRYPTO_30_LIBCV_AEADCHACHAPOLY1305ENCRYPT == STD_ON) */

#if (CRYPTO_30_LIBCV_AEADCHACHAPOLY1305DECRYPT == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_AEADChaChaPoly1305Decrypt()
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
 *
 *
 *
 */
/* PRQA S 6030 4 */ /* MD_MSR_STCYC */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AEADChaChaPoly1305Decrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceChaChaPoly, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfAEADChaChaPoly1305Decrypt(Crypto_30_LibCv_GetAEADChaChaPoly1305DecryptIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceChaChaPoly)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      Crypto_30_LibCv_KeyElementGetType keyElements[2];

      Crypto_30_LibCv_Local_ElementGetterSetIdAndLength(keyElements, 0u, CRYPTO_KE_CIPHER_KEY, CRYPTO_30_LIBCV_CHACHA20POLY1305_KEY_LENGTH); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
      Crypto_30_LibCv_Local_ElementGetterSetIdAndLength(keyElements, 1u, CRYPTO_KE_CIPHER_IV, CRYPTO_30_LIBCV_CHACHA20POLY1305_NONCE_LENGTH); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

      /* # Initialize workspace header. */
      if (esl_initWorkSpaceHeader(&(workspace->header), ESL_SIZEOF_WS_AEAD_ChaCha_Poly, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      {
        /* Check if key element is accessible */
        /* # Read key elements. */
        retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 2u, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

        if (retVal == E_OK)
        {
          /* # Initialize algorithm. */
          retVal = E_NOT_OK;
          retValCv = esl_initDecryptChaChaPoly(workspace,
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex),
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1u].keyElementIndex)); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
        }
      }
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* Get output length in local variable to omit type mismatches */
      eslt_Length outputLength;
      outputLength = (eslt_Length)*(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr);

      /* # Check if output size is sufficient otherwise return error. */
      if ((job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength) > (outputLength))
      {
        retValCv = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
      }
      else
      {
        retValCv = ESL_ERC_NO_ERROR;
      }

      /* # Process authentic data. */
      if ((retValCv == ESL_ERC_NO_ERROR) && (job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength != 0u))
      {
        retValCv = esl_updateAADChaChaPoly(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr,
          (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength);
      }

      /* # Process plaintext data. */
      if (retValCv == ESL_ERC_NO_ERROR)
      {
        retValCv = esl_updateDataChaChaPoly(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
          (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
          (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr,
          (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength);

        /* copy back written output length */
        *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr) = outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      }

      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      /* # Check if output size is sufficient otherwise return error. */
      if (job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.tertiaryInputLength == CRYPTO_30_LIBCV_POLY1305_OUT_SIZE)
      {
        /* # Verify tag. */
        retValCv = esl_verifyChaChaPoly(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.tertiaryInputPtr);
      }
      else
      {
        retValCv = ESL_ERC_INCORRECT_TAG;
      }

      /* # Check if verification was successful. */
      if (retValCv == ESL_ERC_NO_ERROR)
      {
        *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr = CRYPTO_E_VER_OK; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      }
      else if (retValCv == ESL_ERC_INCORRECT_TAG)
      {
        *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr = CRYPTO_E_VER_NOT_OK; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        retValCv = ESL_ERC_NO_ERROR;
      }
      else
      {
        /* For all other errors do nothing */
      }

      break;
    }

    default: /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
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
#endif /* (CRYPTO_30_LIBCV_AEADCHACHAPOLY1305DECRYPT == STD_ON) */

#define CRYPTO_30_LIBCV_STOP_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_Aead.c
 *********************************************************************************************************************/
