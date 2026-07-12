/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*      \file  Crypto_30_Vtt.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Implementation of the MICROSAR Crypto Driver (Crypto)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define CRYPTO_30_VTT_SOURCE
/* PRQA S 0777, 0779, 3453 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_19.7 */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Crypto_30_Vtt.h"
#include "Crypto_30_Vtt_Services.h"
#include "CryIf_Cbk.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

 /* Check the version of CRYPTO header file */
#if ( (CRYPTO_30_VTT_MAJOR_VERSION != (1u)) \
   || (CRYPTO_30_VTT_MINOR_VERSION != (0u)) \
   || (CRYPTO_30_VTT_PATCH_VERSION != (2u)))
# error "Vendor specific version numbers of Crypto_30_Vtt.c and Crypto_30_Vtt.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (CRYPTO_30_VTT_LOCAL) /* COV_CRYPTO_30_VTT_LOCAL_DEFINE */
# define CRYPTO_30_VTT_LOCAL static
#endif

#if !defined (CRYPTO_30_VTT_LOCAL_INLINE) /* COV_CRYPTO_30_VTT_LOCAL_DEFINE */
# define CRYPTO_30_VTT_LOCAL_INLINE LOCAL_INLINE
#endif

#define CRYPTO_30_VTT_DRIVEROBJECTSTATE_IDLE          (0x00u)
#define CRYPTO_30_VTT_DRIVEROBJECTSTATE_PROCESSING    (0x01u)

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_VTT_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
/*! Initialization state of the module */
VAR(uint8, CRYPTO_30_VTT_VAR_ZERO_INIT) Crypto_30_Vtt_ModuleInitialized = CRYPTO_30_VTT_UNINIT;
#endif

#define CRYPTO_30_VTT_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_VTT_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
*  Crypto_30_Vtt_TryObtainingLock()
*********************************************************************************************************************/
/*! \brief         Try to obtain lock workspace.
*  \details        This function tries to lock the workspace with the job.
*  \param[in]      objectId          Holds the identifier of the lock.
*  \param[in]      jobId             Holds the identifier of the related job.
*  \param[in]      cryptoKeyId       Holds the identifier of the related crypto key.
*  \param[in]      mode              Operation mode in which the job currently is.
*  \return         E_OK              Lock is obtained or was already obtained.
*                  E_NOT_OK          Request Failed
*                  CRYPTO_E_BUSY     Lock is occupied by other job.
*  \pre            lockIdx has to be a valid lock identifier.
*                  Has to be called inside an exclusive area which protects the lock.
*  \context        TASK
*  \reentrant      TRUE
*  \synchronous    TRUE
*********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_TryObtainingLock(uint32 objectId, uint32 jobId, uint32 cryptoKeyId, Crypto_OperationModeType mode);

/**********************************************************************************************************************
*  Crypto_30_Vtt_UpdateJobState()
*********************************************************************************************************************/
/*! \brief          Updates the state of the job according to retVal of dispatcher.
 *  \details        This function updates the state of job according to retVal of dispatcher as long as dispatcher is not busy.
 *  \param[in]      retVal             Return value of the dispatcher
 *  \param[in]      job                Pointer to the job whose state shall be updated
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
*********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_UpdateJobState(Std_ReturnType retVal,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job);

/**********************************************************************************************************************
*  Crypto_30_Vtt_DispatchService()
*********************************************************************************************************************/
/*! \brief         Dispatches the job to a service
 *  \details       This function dispatches the provided job to the service as specified in the job configuration.
 *  \param[in]     objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in]     job                Pointer to the job which shall be dispatched.
 *  \param[in]     mode               Operation mode in which the job currently is.
 *  \return        E_OK               Dispatching was successful.
 *                 E_NOT_OK           Dispatching failed.
 *  \pre           objectId has to be a valid driver object handle.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_DispatchService(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job,
  Crypto_OperationModeType mode);

#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
# if (CRYPTO_30_VTT_SERVICE_HASH == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_DetChecksServiceValues_Hash()
*********************************************************************************************************************/
/*! \brief         Det Checks for the Hash service parameters.
 *  \details       This function checks the Det-Checks for all required in- and output-values.
 *  \param[in]     job                Pointer to the job which shall be dispatched.
 *  \return        CRYPTO_E_NO_ERROR        No Det error detected.
 *                 CRYPTO_E_PARAM_POINTER   Parameter is a invalid pointer error.
 *                 CRYPTO_E_PARAM_VALUE     Parameter is a invalid value error.
 *  \pre           job has to be a valid job object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_Hash(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job);
# endif

# if (CRYPTO_30_VTT_SERVICE_MAC_GENERATE == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_DetChecksServiceValues_MacGenerate()
*********************************************************************************************************************/
/*! \brief         Det Checks for the Mac generate service parameters.
 *  \details       This function checks the Det-Checks for all required in- and output-values.
 *  \param[in]     job                Pointer to the job which shall be dispatched.
 *  \return        CRYPTO_E_NO_ERROR        No Det error detected.
 *                 CRYPTO_E_PARAM_POINTER   Parameter is a invalid pointer error.
 *                 CRYPTO_E_PARAM_VALUE     Parameter is a invalid value error.
 *  \pre           job has to be a valid job object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_MacGenerate(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job);
# endif

# if (CRYPTO_30_VTT_SERVICE_MAC_VERIFY == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_DetChecksServiceValues_MacVerify()
*********************************************************************************************************************/
/*! \brief         Det Checks for the Mac verify service parameters.
 *  \details       This function checks the Det-Checks for all required in- and output-values.
 *  \param[in]     job                Pointer to the job which shall be dispatched.
 *  \return        CRYPTO_E_NO_ERROR        No Det error detected.
 *                 CRYPTO_E_PARAM_POINTER   Parameter is a invalid pointer error.
 *                 CRYPTO_E_PARAM_VALUE     Parameter is a invalid value error.
 *  \pre           job has to be a valid job object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_MacVerify(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job);
# endif

# if (CRYPTO_30_VTT_SERVICE_ENCRYPT == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_DetChecksServiceValues_Encrypt()
*********************************************************************************************************************/
/*! \brief         Det Checks for the encrypt service parameters.
 *  \details       This function checks the Det-Checks for all required in- and output-values.
 *  \param[in]     job                Pointer to the job which shall be dispatched.
 *  \return        CRYPTO_E_NO_ERROR        No Det error detected.
 *                 CRYPTO_E_PARAM_POINTER   Parameter is a invalid pointer error.
 *                 CRYPTO_E_PARAM_VALUE     Parameter is a invalid value error.
 *  \pre           job has to be a valid job object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_Encrypt(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job);
# endif

# if (CRYPTO_30_VTT_SERVICE_DECRYPT == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_DetChecksServiceValues_Decrypt()
*********************************************************************************************************************/
/*! \brief         Det Checks for the decrypt service parameters.
 *  \details       This function checks the Det-Checks for all required in- and output-values.
 *  \param[in]     job                Pointer to the job which shall be dispatched.
 *  \return        CRYPTO_E_NO_ERROR        No Det error detected.
 *                 CRYPTO_E_PARAM_POINTER   Parameter is a invalid pointer error.
 *                 CRYPTO_E_PARAM_VALUE     Parameter is a invalid value error.
 *  \pre           job has to be a valid job object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_Decrypt(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job);
# endif

# if (CRYPTO_30_VTT_SERVICE_AEADENCRYPT == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_DetChecksServiceValues_AeadEncrypt()
*********************************************************************************************************************/
/*! \brief         Det Checks for the Aead encrypt service parameters.
 *  \details       This function checks the Det-Checks for all required in- and output-values.
 *  \param[in]     job                Pointer to the job which shall be dispatched.
 *  \return        CRYPTO_E_NO_ERROR        No Det error detected.
 *                 CRYPTO_E_PARAM_POINTER   Parameter is a invalid pointer error.
 *                 CRYPTO_E_PARAM_VALUE     Parameter is a invalid value error.
 *  \pre           job has to be a valid job object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_AeadEncrypt(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job);
# endif

# if (CRYPTO_30_VTT_SERVICE_AEADDECRYPT == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_DetChecksServiceValues_AeadDecrypt()
*********************************************************************************************************************/
/*! \brief         Det Checks for the Aead decrypt service parameters.
 *  \details       This function checks the Det-Checks for all required in- and output-values.
 *  \param[in]     job                Pointer to the job which shall be dispatched.
 *  \return        CRYPTO_E_NO_ERROR        No Det error detected.
 *                 CRYPTO_E_PARAM_POINTER   Parameter is a invalid pointer error.
 *                 CRYPTO_E_PARAM_VALUE     Parameter is a invalid value error.
 *  \pre           job has to be a valid job object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_AeadDecrypt(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job);
# endif

# if (CRYPTO_30_VTT_SERVICE_SIGNATURE_GENERATE == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_DetChecksServiceValues_SignatureGenerate()
*********************************************************************************************************************/
/*! \brief         Det Checks for the signature generate service parameters.
 *  \details       This function checks the Det-Checks for all required in- and output-values.
 *  \param[in]     job                Pointer to the job which shall be dispatched.
 *  \return        CRYPTO_E_NO_ERROR        No Det error detected.
 *                 CRYPTO_E_PARAM_POINTER   Parameter is a invalid pointer error.
 *                 CRYPTO_E_PARAM_VALUE     Parameter is a invalid value error.
 *  \pre           job has to be a valid job object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_SignatureGenerate(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job);
# endif

# if (CRYPTO_30_VTT_SERVICE_SIGNATURE_VERIFY == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_DetChecksServiceValues_SignatureVerify()
*********************************************************************************************************************/
/*! \brief         Det Checks for the signature verify service parameters.
 *  \details       This function checks the Det-Checks for all required in- and output-values.
 *  \param[in]     job                Pointer to the job which shall be dispatched.
 *  \return        CRYPTO_E_NO_ERROR        No Det error detected.
 *                 CRYPTO_E_PARAM_POINTER   Parameter is a invalid pointer error.
 *                 CRYPTO_E_PARAM_VALUE     Parameter is a invalid value error.
 *  \pre           job has to be a valid job object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_SignatureVerify(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job);
# endif

# if (CRYPTO_30_VTT_SERVICE_RANDOM == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_DetChecksServiceValues_Random()
*********************************************************************************************************************/
/*! \brief         Det Checks for the random service parameters.
 *  \details       This function checks the Det-Checks for all required in- and output-values.
 *  \param[in]     job                Pointer to the job which shall be dispatched.
 *  \return        CRYPTO_E_NO_ERROR        No Det error detected.
 *                 CRYPTO_E_PARAM_POINTER   Parameter is a invalid pointer error.
 *                 CRYPTO_E_PARAM_VALUE     Parameter is a invalid value error.
 *  \pre           job has to be a valid job object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_Random(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job);
# endif

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_DetChecksServiceValues()
*********************************************************************************************************************/
/*! \brief         Det Checks for the Service Parameters
 *  \details       This function checks the Det-Checks for all required in- and output-values.
 *  \param[in]     job                Pointer to the job which shall be dispatched.
 *  \param[out]    errorId            Output Pointer to the detected Error-Type-Id
 *  \return        E_OK               No Det error detected.
 *                 E_NOT_OK           Det error detected.
 *  \pre           objectId has to be a valid driver object handle.
 *                 errorId has to be a valid pointer.
 *                 job has to be a valid job object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) errorId);
#endif /* (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON) */

/**********************************************************************************************************************
*  Crypto_30_Vtt_Process()
*********************************************************************************************************************/
/*! \brief         Processes the received job
 *  \details       Performs the crypto primitive, that is configured in the job parameter. Takes care of jobs operation mode.
 *  \param[in]     objectId                Holds the identifier of the Crypto Driver Object.
 *  \param[in,out] job                     Pointer to the configuration of the job. Contains structures with job and
 *                                         primitive relevant information but also pointer to result buffers.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *                 CRYPTO_E_QUEUE_FULL     Request failed, the queue is full.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           Lock has to be reserved for the job object.
 *                 objectId has to be a valid driver object handle.
 *                 job has to be a job object handle.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Process(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
*  Crypto_30_Vtt_TryObtainingLock()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_TryObtainingLock(uint32 objectId,
  uint32 jobId, uint32 cryptoKeyId, Crypto_OperationModeType mode)
{
  Std_ReturnType retVal;
  /* #05 Lock driver object for the given job if it is not already locked. */

  if (Crypto_30_Vtt_IsLockOccupiedByJob(objectId, jobId))
  {
    retVal = E_OK;
  }
  else if ((mode & CRYPTO_OPERATIONMODE_START) == CRYPTO_OPERATIONMODE_START)
  {
    retVal = CRYPTO_E_BUSY;
    if (Crypto_30_Vtt_IsLockFree(objectId))
    {
      /* #10 Lock key if operation mode is start */
      if (Crypto_30_Vtt_Local_KeyReadLockGetNotProtected(cryptoKeyId) == E_OK)
      {
        Crypto_30_Vtt_SetLock(objectId, jobId); /* SBSW_CRYPTO_30_VTT_CSL01_OBJECTID */
        retVal = E_OK;
      }
    }
  }
  else
  {
    /* [SWS_Crypto_00118] No Start flag while in idle mode */
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
*  Crypto_30_Vtt_UpdateJobState()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_UpdateJobState(Std_ReturnType retVal,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job)
{
  /* #05 Set job state based on parameter return value. */
  if (retVal == E_OK)
  {
    job->state = CRYPTO_JOBSTATE_ACTIVE; /* SBSW_CRYPTO_30_VTT_UPDATE_OF_JOB_STATE */
  }
  else
  {
    /* [SWS_Crypto_00025] [SWS_Crypto_00119] Set job to IDLE if error occurred */
    job->state = CRYPTO_JOBSTATE_IDLE; /* SBSW_CRYPTO_30_VTT_UPDATE_OF_JOB_STATE */
  }
}

#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
# if (CRYPTO_30_VTT_SERVICE_HASH == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_DetChecksServiceValues_Hash()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_Hash(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job)
{
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* #1 Check Hash Pointers and Length Parameter */
  /* [SWS_Crypto_00071] [SWS_Crypto_00070] Check Pointer */
  /* Optional check of inputPtr and inputLength for finish mode is not required. */
  if (((job->jobPrimitiveInputOutput.inputPtr == NULL_PTR) && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)) ||
    (((job->jobPrimitiveInputOutput.outputPtr == NULL_PTR) || (job->jobPrimitiveInputOutput.outputLengthPtr == NULL_PTR)) && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)))
  {
    errorId = CRYPTO_E_PARAM_POINTER; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  /* [SWS_Crypto_00071] [SWS_Crypto_00142] Check Length */
  else if ((((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE) && (job->jobPrimitiveInputOutput.inputLength == 0u)) ||
    (((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH) && (*(job->jobPrimitiveInputOutput.outputLengthPtr) == 0u)))
  {
    errorId = CRYPTO_E_PARAM_VALUE; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  else
  {
    /* Det Checks passed. Nothing has to be done. */
  }

  return errorId;
}
# endif

# if (CRYPTO_30_VTT_SERVICE_MAC_GENERATE == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_DetChecksServiceValues_MacGenerate()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_MacGenerate(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job)
{
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* #2 Check MAC Generate Pointers and Length Parameter */
  /* [SWS_Crypto_00071] [SWS_Crypto_00070] Check Pointer */
  /* Optional check of inputPtr and inputLength for finish mode is not required. */
  if (((job->jobPrimitiveInputOutput.inputPtr == NULL_PTR) && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)) ||
    (((job->jobPrimitiveInputOutput.outputPtr == NULL_PTR) || (job->jobPrimitiveInputOutput.outputLengthPtr == NULL_PTR)) && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)))
  {
    errorId = CRYPTO_E_PARAM_POINTER; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  /* [SWS_Crypto_00071] [SWS_Crypto_00142] Check Length */
  else if ((((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE) && (job->jobPrimitiveInputOutput.inputLength == 0u)) ||
    (((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH) && (*(job->jobPrimitiveInputOutput.outputLengthPtr) == 0u)))
  {
    errorId = CRYPTO_E_PARAM_VALUE; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  else
  {
    /* Det Checks passed. Nothing has to be done. */
  }
  return errorId;
}
# endif

# if (CRYPTO_30_VTT_SERVICE_MAC_VERIFY == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_DetChecksServiceValues_MacVerify()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_MacVerify(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job)
{
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* #3 Check MAC Verify Pointers and Length Parameter */
  /* [SWS_Crypto_00071] [SWS_Crypto_00070] Check Pointer */
  /* Optional check of inputPtr and inputLength for finish mode is not required. */
  if (((job->jobPrimitiveInputOutput.inputPtr == NULL_PTR) && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)) ||
    (((job->jobPrimitiveInputOutput.secondaryInputPtr == NULL_PTR) || (job->jobPrimitiveInputOutput.verifyPtr == NULL_PTR)) && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)))
  {
    errorId = CRYPTO_E_PARAM_POINTER; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  /* [SWS_Crypto_00071] [SWS_Crypto_00142] Check Length */
  else if ((((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE) && (job->jobPrimitiveInputOutput.inputLength == 0u)) ||
    (((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH) && (job->jobPrimitiveInputOutput.secondaryInputLength == 0u)))
  {
    errorId = CRYPTO_E_PARAM_VALUE; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  else
  {
    /* Det Checks passed. Nothing has to be done. */
  }
  return errorId;

}
# endif

# if (CRYPTO_30_VTT_SERVICE_ENCRYPT == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_DetChecksServiceValues_Encrypt()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_Encrypt(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job)
{
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* #4 Check Encrypt Pointers and Length Parameter */
  /* [SWS_Crypto_00071] [SWS_Crypto_00070] Check Pointer */
  /* Optional check of inputPtr and inputLength for finish mode is not required. */
  if (((job->jobPrimitiveInputOutput.inputPtr == NULL_PTR) && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)) ||
    (((job->jobPrimitiveInputOutput.outputPtr == NULL_PTR) || (job->jobPrimitiveInputOutput.outputLengthPtr == NULL_PTR)) && (((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE) || ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))))
  {
    errorId = CRYPTO_E_PARAM_POINTER; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  /* [SWS_Crypto_00071] [SWS_Crypto_00142] Check Length */
  else if ((((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE) && (job->jobPrimitiveInputOutput.inputLength == 0u)) ||
    ((((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE) || ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)) && (*(job->jobPrimitiveInputOutput.outputLengthPtr) == 0u)))
  {
    errorId = CRYPTO_E_PARAM_VALUE; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  else
  {
    /* Det Checks passed. Nothing has to be done. */
  }
  return errorId;
}
# endif

# if (CRYPTO_30_VTT_SERVICE_DECRYPT == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_DetChecksServiceValues_Decrypt()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_Decrypt(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job)
{
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* #5 Check Decrypt Pointers and Length Parameter */
  /* [SWS_Crypto_00071] [SWS_Crypto_00070] Check Pointer */
  /* Optional check of inputPtr and inputLength for finish mode is not required. */
  if (((job->jobPrimitiveInputOutput.inputPtr == NULL_PTR) && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)) ||
    (((job->jobPrimitiveInputOutput.outputPtr == NULL_PTR) || (job->jobPrimitiveInputOutput.outputLengthPtr == NULL_PTR)) && (((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE) || ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))))
  {
    errorId = CRYPTO_E_PARAM_POINTER; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  /* [SWS_Crypto_00071] [SWS_Crypto_00142] Check Length */
  else if ((((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE) && (job->jobPrimitiveInputOutput.inputLength == 0u)) ||
    ((((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE) || ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)) && (*(job->jobPrimitiveInputOutput.outputLengthPtr) == 0u)))
  {
    errorId = CRYPTO_E_PARAM_VALUE; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  else
  {
    /* Det Checks passed. Nothing has to be done. */
  }
  return errorId;
}
# endif

# if (CRYPTO_30_VTT_SERVICE_AEADENCRYPT == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_DetChecksServiceValues_AeadEncrypt()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_AeadEncrypt(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job)
{
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* #6 Check AEAD Encrypt Pointers and Length Parameter */
  /* [SWS_Crypto_00071] [SWS_Crypto_00070] Check Pointer */
  /* Optional check of inputPtr and inputLength for finish mode is not required. */
  if ((((job->jobPrimitiveInputOutput.inputPtr == NULL_PTR) || ((job->jobPrimitiveInputOutput.secondaryInputPtr == NULL_PTR) && (job->jobPrimitiveInputOutput.secondaryInputLength != 0u))) && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)) ||
    (((job->jobPrimitiveInputOutput.outputPtr == NULL_PTR) || (job->jobPrimitiveInputOutput.outputLengthPtr == NULL_PTR)) && (((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE) || ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))) ||
    (((job->jobPrimitiveInputOutput.secondaryOutputPtr == NULL_PTR) || (job->jobPrimitiveInputOutput.secondaryOutputLengthPtr == NULL_PTR)) && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)))
  {
    errorId = CRYPTO_E_PARAM_POINTER; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  /* [SWS_Crypto_00071] [SWS_Crypto_00142] Check Length */
  else if (((((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE) || ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)) && (*(job->jobPrimitiveInputOutput.outputLengthPtr) == 0u)) ||
    (((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH) && ((*(job->jobPrimitiveInputOutput.secondaryOutputLengthPtr) == 0u))))
  {
    errorId = CRYPTO_E_PARAM_VALUE; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  else
  {
    /* Det Checks passed. Nothing has to be done. */
  }
  return errorId;
}
# endif

# if (CRYPTO_30_VTT_SERVICE_AEADDECRYPT == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_DetChecksServiceValues_AeadDecrypt()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_AeadDecrypt(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job)
{
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* #7 Check AEAD Decrypt Pointers and Length Parameter */
  /* [SWS_Crypto_00071] [SWS_Crypto_00070] Check Pointer */
  /* Optional check of inputPtr and inputLength for finish mode is not required. */
  if ((((job->jobPrimitiveInputOutput.inputPtr == NULL_PTR) || ((job->jobPrimitiveInputOutput.secondaryInputLength != 0u) && (job->jobPrimitiveInputOutput.secondaryInputPtr == NULL_PTR))) && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)) ||
    (((job->jobPrimitiveInputOutput.outputPtr == NULL_PTR) || (job->jobPrimitiveInputOutput.outputLengthPtr == NULL_PTR)) && (((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE) || ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))) ||
    (((job->jobPrimitiveInputOutput.tertiaryInputPtr == NULL_PTR) || (job->jobPrimitiveInputOutput.verifyPtr == NULL_PTR)) && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)))
  {
    errorId = CRYPTO_E_PARAM_POINTER; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  /* [SWS_Crypto_00071] [SWS_Crypto_00142] Check Length */
  else if ((((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE) && (job->jobPrimitiveInputOutput.inputLength == 0u)) ||
    ((((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE) || ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)) && (*(job->jobPrimitiveInputOutput.outputLengthPtr) == 0u)) ||
    (((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH) && ((job->jobPrimitiveInputOutput.tertiaryInputLength == 0u))))
  {
    errorId = CRYPTO_E_PARAM_VALUE; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  else
  {
    /* Det Checks passed. Nothing has to be done. */
  }
  return errorId;
}
# endif

# if (CRYPTO_30_VTT_SERVICE_SIGNATURE_GENERATE == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_DetChecksServiceValues_SignatureGenerate()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_SignatureGenerate(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job)
{
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* #8 Check Signature Generate Pointers and Length Parameter */
  /* [SWS_Crypto_00071] [SWS_Crypto_00070] Check Pointer */
  /* Optional check of inputPtr and inputLength for finish mode is required. */
  if (((job->jobPrimitiveInputOutput.inputPtr == NULL_PTR) && (((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE) || ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))) ||
    (((job->jobPrimitiveInputOutput.outputPtr == NULL_PTR) || (job->jobPrimitiveInputOutput.outputLengthPtr == NULL_PTR)) && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)))
  {
    errorId = CRYPTO_E_PARAM_POINTER; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  /* [SWS_Crypto_00071] [SWS_Crypto_00142] Check Length */
  else if (((((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)) && (job->jobPrimitiveInputOutput.inputLength == 0u)) ||
    (((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH) && (*(job->jobPrimitiveInputOutput.outputLengthPtr) == 0u)))
  {
    errorId = CRYPTO_E_PARAM_VALUE; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  else
  {
    /* Det Checks passed. Nothing has to be done. */
  }
  return errorId;
}
# endif

# if (CRYPTO_30_VTT_SERVICE_SIGNATURE_VERIFY == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_DetChecksServiceValues_SignatureVerify()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_SignatureVerify(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job)
{
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* #9 Check Signature Verify Pointers and Length Parameter */
  /* [SWS_Crypto_00071] [SWS_Crypto_00070] Check Pointer */
  /* Optional check of inputPtr and inputLength for finish mode is required. */
  if (((job->jobPrimitiveInputOutput.inputPtr == NULL_PTR) && (((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE) || ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))) ||
    (((job->jobPrimitiveInputOutput.secondaryInputPtr == NULL_PTR) || (job->jobPrimitiveInputOutput.verifyPtr == NULL_PTR)) && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)))
  {
    errorId = CRYPTO_E_PARAM_POINTER; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  /* [SWS_Crypto_00071] [SWS_Crypto_00142] Check Length */
  else if (((((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)) && (job->jobPrimitiveInputOutput.inputLength == 0u)) ||
    (((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH) && (job->jobPrimitiveInputOutput.secondaryInputLength == 0u)))
  {
    errorId = CRYPTO_E_PARAM_VALUE; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  else
  {
    /* Det Checks passed. Nothing has to be done. */
  }
  return errorId;
}
# endif

# if (CRYPTO_30_VTT_SERVICE_RANDOM == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_DetChecksServiceValues_Random()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(uint8, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues_Random(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job)
{
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* #10 Check Random Generate Pointers and Length Parameter */
  /* [SWS_Crypto_00071] [SWS_Crypto_00070] Check Pointer */
  if (((job->jobPrimitiveInputOutput.outputPtr == NULL_PTR) || (job->jobPrimitiveInputOutput.outputLengthPtr == NULL_PTR)) && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))
  {
    errorId = CRYPTO_E_PARAM_POINTER; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  else if (((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH) && (*(job->jobPrimitiveInputOutput.outputLengthPtr) == 0u))
  {
    errorId = CRYPTO_E_PARAM_VALUE; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
  }
  else
  {
    /* Det Checks passed. Nothing has to be done. */
  }
  return errorId;
}
# endif

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_DetChecksServiceValues()
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

CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_DetChecksServiceValues(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) errorId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Development Error Checks --------------------------------------- */
  /* #0 Check plausibility of parameters */
  switch (job->jobPrimitiveInfo->primitiveInfo->service)
  {
# if (CRYPTO_30_VTT_SERVICE_HASH == STD_ON)
  case CRYPTO_HASH:
    *errorId = Crypto_30_Vtt_Local_DetChecksServiceValues_Hash(job); /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */ /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
    break;

# endif
# if (CRYPTO_30_VTT_SERVICE_MAC_GENERATE == STD_ON)
  case CRYPTO_MACGENERATE:
    *errorId = Crypto_30_Vtt_Local_DetChecksServiceValues_MacGenerate(job); /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */ /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
    break;

# endif
# if (CRYPTO_30_VTT_SERVICE_MAC_VERIFY == STD_ON)
  case CRYPTO_MACVERIFY:
    *errorId = Crypto_30_Vtt_Local_DetChecksServiceValues_MacVerify(job); /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */ /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
    break;

# endif
# if (CRYPTO_30_VTT_SERVICE_ENCRYPT == STD_ON)
  case CRYPTO_ENCRYPT:
    *errorId = Crypto_30_Vtt_Local_DetChecksServiceValues_Encrypt(job); /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */ /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
    break;

# endif
# if (CRYPTO_30_VTT_SERVICE_DECRYPT == STD_ON)
  case CRYPTO_DECRYPT:
    *errorId = Crypto_30_Vtt_Local_DetChecksServiceValues_Decrypt(job); /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */ /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
    break;

# endif
# if (CRYPTO_30_VTT_SERVICE_AEADENCRYPT == STD_ON)
  case CRYPTO_AEADENCRYPT:
    *errorId = Crypto_30_Vtt_Local_DetChecksServiceValues_AeadEncrypt(job); /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */ /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
    break;

# endif
# if (CRYPTO_30_VTT_SERVICE_AEADDECRYPT == STD_ON)
  case CRYPTO_AEADDECRYPT:
    *errorId = Crypto_30_Vtt_Local_DetChecksServiceValues_AeadDecrypt(job); /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */ /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
    break;

# endif
# if (CRYPTO_30_VTT_SERVICE_SIGNATURE_GENERATE == STD_ON)
  case CRYPTO_SIGNATUREGENERATE:
    *errorId = Crypto_30_Vtt_Local_DetChecksServiceValues_SignatureGenerate(job); /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */ /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
    break;

# endif
# if (CRYPTO_30_VTT_SERVICE_SIGNATURE_VERIFY == STD_ON)
  case CRYPTO_SIGNATUREVERIFY:
    *errorId = Crypto_30_Vtt_Local_DetChecksServiceValues_SignatureVerify(job); /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */ /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
    break;

# endif
# if (CRYPTO_30_VTT_SERVICE_RANDOM == STD_ON)
  case CRYPTO_RANDOM:
    *errorId = Crypto_30_Vtt_Local_DetChecksServiceValues_Random(job); /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */ /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
    break;

# endif
  default:
    /* #11 Service is not supported by Crypto Driver Object */
    /* [SWS_Crypto_00064] check if job->jobPrimitiveInfo->primitiveInfo->service is supported by Crypto Driver Object */
    *errorId = CRYPTO_E_PARAM_HANDLE; /* SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR */
    break;

  }

  /* #12 Set return Value to E_NOT_OK if a failure was detected. */
  if ((*errorId == CRYPTO_E_PARAM_POINTER) || (*errorId == CRYPTO_E_PARAM_VALUE) || (*errorId == CRYPTO_E_PARAM_HANDLE))
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}/* PRQA S 6030, 6050 */ /* MD_MSR_STCYC, MD_MSR_STCAL */
#endif

/**********************************************************************************************************************
 * Crypto_30_Vtt_Process()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Process(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8 unlockingNecessary = FALSE;

  /* #10 Dispatch job according to job mode in order START, UPDATE and FINISH. */
  /* [SWS_Crypto_00017] [SWS_Crypto_00020] START while IDLE or ACTIVE */
  if (((job->state == CRYPTO_JOBSTATE_IDLE) || (job->state == CRYPTO_JOBSTATE_ACTIVE))
    && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_START) == CRYPTO_OPERATIONMODE_START))
  {
    retVal = Crypto_30_Vtt_DispatchService(objectId, job, CRYPTO_OPERATIONMODE_START); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
    Crypto_30_Vtt_UpdateJobState(retVal, job); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
  }

  /* UPDATE while ACTIVE */
  if ((job->state == CRYPTO_JOBSTATE_ACTIVE)
    && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)) /* [SWS_Crypto_00024] */
  {
    retVal = Crypto_30_Vtt_DispatchService(objectId, job, CRYPTO_OPERATIONMODE_UPDATE); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
    Crypto_30_Vtt_UpdateJobState(retVal, job); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
  }

  /* [SWS_Crypto_00023] FINISH while ACTIVE */
  if ((job->state == CRYPTO_JOBSTATE_ACTIVE)
    && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)) /* [SWS_Crypto_00024] */
  {
    retVal = Crypto_30_Vtt_DispatchService(objectId, job, CRYPTO_OPERATIONMODE_FINISH); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
    job->state = CRYPTO_JOBSTATE_IDLE; /* SBSW_CRYPTO_30_VTT_DIRECT_ACCESS_TO_JOB_STATE */
    unlockingNecessary = TRUE;
  }

  /* #15 Free workspace in case of an error or if FINISH mode completed. */
  if ((retVal != E_OK) || (unlockingNecessary == TRUE))
  {
    SchM_Enter_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();
    Crypto_30_Vtt_FreeLock(objectId); /* SBSW_CRYPTO_30_VTT_CSL01_OBJECTID */
    Crypto_30_Vtt_Local_KeyReadLockReleaseNotProtected(job->cryptoKeyId);
    SchM_Exit_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();
  }

  /* #20 Notify CryIf about the result of the job if it was processed asynchronous. */
  if (job->jobPrimitiveInfo->processingType == CRYPTO_PROCESSING_ASYNC)
  {
    /* Call Callback */
    CryIf_CallbackNotification(job, retVal); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_Vtt_DispatchService()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_DispatchService(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_VTT_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #05 Dispatch Service according to job. */
  switch (job->jobPrimitiveInfo->primitiveInfo->service)
  {
#if (CRYPTO_30_VTT_SERVICE_HASH == STD_ON)
  case CRYPTO_HASH:
  {
      retVal = Crypto_30_Vtt_DispatchHash(objectId, job, mode); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
      break;
    }
#endif
#if (CRYPTO_30_VTT_SERVICE_MAC_GENERATE == STD_ON)
    case CRYPTO_MACGENERATE:
    {
      retVal = Crypto_30_Vtt_DispatchMacGenerate(objectId, job, mode); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
      break;
    }
#endif
#if (CRYPTO_30_VTT_SERVICE_MAC_VERIFY == STD_ON)
    case CRYPTO_MACVERIFY:
    {
      retVal = Crypto_30_Vtt_DispatchMacVerify(objectId, job, mode); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
      break;
    }
#endif
#if (CRYPTO_30_VTT_SERVICE_ENCRYPT == STD_ON)
    case CRYPTO_ENCRYPT:
    {
      retVal = Crypto_30_Vtt_DispatchCipherEncrypt(objectId, job, mode); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
      break;
    }
#endif
#if (CRYPTO_30_VTT_SERVICE_DECRYPT == STD_ON)
    case CRYPTO_DECRYPT:
    {
      retVal = Crypto_30_Vtt_DispatchCipherDecrypt(objectId, job, mode); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
      break;
    }
#endif
#if (CRYPTO_30_VTT_SERVICE_AEADENCRYPT == STD_ON)
    case CRYPTO_AEADENCRYPT:
    {
      retVal = Crypto_30_Vtt_DispatchAeadEncrypt(objectId, job, mode); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
      break;
    }
#endif
#if (CRYPTO_30_VTT_SERVICE_AEADDECRYPT == STD_ON)
    case CRYPTO_AEADDECRYPT:
    {
      retVal = Crypto_30_Vtt_DispatchAeadDecrypt(objectId, job, mode); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
      break;
    }
#endif
#if (CRYPTO_30_VTT_SERVICE_SIGNATURE_GENERATE == STD_ON)
    case CRYPTO_SIGNATUREGENERATE:
    {
      retVal = Crypto_30_Vtt_DispatchSignatureGenerate(objectId, job, mode); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
      break;
    }
#endif
#if (CRYPTO_30_VTT_SERVICE_SIGNATURE_VERIFY == STD_ON)
    case CRYPTO_SIGNATUREVERIFY:
    {
      retVal = Crypto_30_Vtt_DispatchSignatureVerify(objectId, job, mode); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
      break;
    }
#endif
#if (CRYPTO_30_VTT_SERVICE_RANDOM == STD_ON)
    case CRYPTO_RANDOM:
    {
      retVal = Crypto_30_Vtt_DispatchRandom(objectId, job, mode); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
      break;
    }
#endif
    default: /* COV_CRYPTO_30_VTT_MISRA_ADDITIONAL_STATEMENT */
    {
      break;
    }
  }

  return retVal;
}/* PRQA S 6030, 6050 */ /* MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Crypto_30_Vtt_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  /* #05 Uninitialize module. */
  Crypto_30_Vtt_ModuleInitialized = (uint8)CRYPTO_30_VTT_UNINIT;
#endif
} /* Crypto_30_Vtt_InitMemory() */

/**********************************************************************************************************************
 *  Crypto_30_Vtt_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  /* #05 Check initialization state of the component. */
  if (Crypto_30_Vtt_ModuleInitialized == (uint8)CRYPTO_30_VTT_INITIALIZED)
  {
    errorId = CRYPTO_E_INIT_FAILED;
  }
  else
#endif
  {

    Crypto_30_Vtt_KeyElementsIterType elementIndex;
    Crypto_30_Vtt_KeyIterType keyIndex;
#if (CRYPTO_30_VTT_INITVALUE == STD_ON)
    uint32 initValueIdx;
#endif
    uint32 keyStorageIdx;
    Crypto_30_Vtt_QueueIterType queueIdx;
    Crypto_30_Vtt_LockIterType lockIdx;
    Crypto_30_Vtt_DriverObjectIterType objectId;

    /* #10 Initialize key elements which have a configured initial value. */
    for (elementIndex = 0u; elementIndex < Crypto_30_Vtt_GetSizeOfKeyElements(); elementIndex++)
    {
#if (CRYPTO_30_VTT_INITVALUE == STD_ON)
      /* Fill elements with init values */
      if (Crypto_30_Vtt_HasKeyElementInitValue(elementIndex))
      {
        keyStorageIdx = Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements(elementIndex);
        initValueIdx = Crypto_30_Vtt_GetInitValueStartIdxOfKeyElementInfo(Crypto_30_Vtt_GetKeyElementInfoIdxOfKeyElements(elementIndex));

        /* Iterate through keyElements */
        while (initValueIdx < Crypto_30_Vtt_GetInitValueEndIdxOfKeyElementInfo(Crypto_30_Vtt_GetKeyElementInfoIdxOfKeyElements(elementIndex)))
        {
          /* Store the init value t the key element */
          Crypto_30_Vtt_SetKeyStorage(keyStorageIdx, Crypto_30_Vtt_GetInitValue(initValueIdx)); /* SBSW_CRYPTO_30_VTT_KEY_STORAGE_ACCESS_INIT */
          CANoeAPI_ConsumeTicks(1);
          initValueIdx++;
          keyStorageIdx++;
        }

        /* Set the size of the init value written to the element */
        keyStorageIdx = Crypto_30_Vtt_GetKeyStorageWrittenLengthStartIdxOfKeyElements(elementIndex);
        Crypto_30_Vtt_SetKeyStorage(keyStorageIdx, (uint8)(((uint16)Crypto_30_Vtt_GetKeyElementInitValueLength(elementIndex)) >> 8)); /* SBSW_CRYPTO_30_VTT_KEY_STORAGE_ACCESS_INIT */
        Crypto_30_Vtt_SetKeyStorage(keyStorageIdx + 1u, (uint8)(Crypto_30_Vtt_GetKeyElementInitValueLength(elementIndex))); /* SBSW_CRYPTO_30_VTT_KEY_STORAGE_ACCESS_INIT */

# if (CRYPTO_30_VTT_KEYELEMENTINFOVIRTUALIDXOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_VTT_VIRTUALKEY */
        /* Mark element as invalid if it is a virtual element (because offset has not been set yet and it is not usable until e.g. the certificate is parsed) */
        if (Crypto_30_Vtt_IsKeyElementVirtual(elementIndex))
        {
          Crypto_30_Vtt_SetKeyElementState(elementIndex, FALSE);
        }
        else
# endif
        {
          Crypto_30_Vtt_SetKeyElementState(elementIndex, CRYPTO_30_VTT_KEYELEMENTSTATE_VALID_MASK); /* SBSW_CRYPTO_30_VTT_KEY_STORAGE_ACCESS_INIT */
        }
      }
      else
#endif
      {
          keyStorageIdx = Crypto_30_Vtt_GetKeyStorageWrittenLengthStartIdxOfKeyElements(elementIndex);
          Crypto_30_Vtt_SetKeyStorage(keyStorageIdx, 0u); /* SBSW_CRYPTO_30_VTT_KEY_STORAGE_ACCESS_INIT */
          Crypto_30_Vtt_SetKeyStorage(keyStorageIdx + 1u, 0u); /* SBSW_CRYPTO_30_VTT_KEY_STORAGE_ACCESS_INIT */
          Crypto_30_Vtt_SetKeyElementState(elementIndex, FALSE); /* SBSW_CRYPTO_30_VTT_KEY_STORAGE_ACCESS_INIT */
      }
    }

    /* #15 Free all locks of workspaces. */
    for(lockIdx = 0u; lockIdx < Crypto_30_Vtt_GetSizeOfLock(); lockIdx++)
    {
      Crypto_30_Vtt_FreeLock(lockIdx); /* SBSW_CRYPTO_30_VTT_CSL01 */
    }

    /* #20 Clear job queue. */
    for(queueIdx = 0u; queueIdx < Crypto_30_Vtt_GetSizeOfQueue(); queueIdx++)
    {
      Crypto_30_Vtt_SetQueue(queueIdx, NULL_PTR); /* SBSW_CRYPTO_30_VTT_CSL01 */
    }

    /* #25 Init Driver Object State. */
    for (objectId = 0u; objectId < Crypto_30_Vtt_GetSizeOfDriverObject(); objectId++)
    {
      Crypto_30_Vtt_SetDriverObjectState(objectId, CRYPTO_30_VTT_DRIVEROBJECTSTATE_IDLE); /* SBSW_CRYPTO_30_VTT_CSL01_OBJECTID */
    }

    for (keyIndex = 0u; keyIndex < Crypto_30_Vtt_GetSizeOfKeyLock(); keyIndex++)
    {
      Crypto_30_Vtt_SetKeyLock(keyIndex, CRYPTO_30_VTT_KEY_LOCK_FREE); /* SBSW_CRYPTO_30_VTT_CSL01 */
    }

	VttCntrl_OpenCryptoNvFile();
  VttCntrl_ReadCryptoNvFile();

    /* Set module state to initialized */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
    Crypto_30_Vtt_ModuleInitialized = CRYPTO_30_VTT_INITIALIZED;
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYPTO_E_NO_ERROR)
  {
   (void)Det_ReportError(CRYPTO_30_VTT_MODULE_ID, CRYPTO_30_VTT_INSTANCE_ID, CRYPTO_30_VTT_SID_INIT, errorId);
  }
#else
  CRYPTO_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif
} /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */

#if (CRYPTO_30_VTT_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_Vtt_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  /* #05 Check plausibility of parameters. */
  if (versioninfo == NULL_PTR)
  {
    /* [SWS_Crypto_00042] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #10 Write versionInfo pointer with modules versions. */
    versioninfo->vendorID = (uint16)CRYPTO_30_VTT_VENDOR_ID; /* SBSW_CRYPTO_30_VTT_GETVERSIONINFO */
    versioninfo->moduleID = (uint8)CRYPTO_30_VTT_MODULE_ID; /* SBSW_CRYPTO_30_VTT_GETVERSIONINFO */
    versioninfo->sw_major_version = (uint8)CRYPTO_30_VTT_MAJOR_VERSION; /* SBSW_CRYPTO_30_VTT_GETVERSIONINFO */
    versioninfo->sw_minor_version = (uint8)CRYPTO_30_VTT_MINOR_VERSION; /* SBSW_CRYPTO_30_VTT_GETVERSIONINFO */
    versioninfo->sw_patch_version = (uint8)CRYPTO_30_VTT_PATCH_VERSION; /* SBSW_CRYPTO_30_VTT_GETVERSIONINFO */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_VTT_MODULE_ID,
                          CRYPTO_30_VTT_INSTANCE_ID,
                          (uint8)CRYPTO_30_VTT_SID_GET_VERSION_INFO,
                          errorId);
  }
# else
  CRYPTO_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif
}
#endif

/**********************************************************************************************************************
 *  Crypto_30_Vtt_MainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Crypto_30_Vtt_DriverObjectIterType objectId;

  /* ----- Implementation ----------------------------------------------- */
  /* #05 Process next enqueued job for all available queues. */
  for (objectId = 0u; objectId < Crypto_30_Vtt_GetSizeOfDriverObject(); objectId++)
  {
    uint8 processJob = FALSE;
    Crypto_30_Vtt_QueueIdxOfDriverObjectType queueIdx = Crypto_30_Vtt_GetQueueIdxOfDriverObject(objectId);
    Crypto_30_Vtt_QueueElementType job;

    SchM_Enter_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();
    job = Crypto_30_Vtt_GetQueue(queueIdx);
    if (job != NULL_PTR)
    {
      if (Crypto_30_Vtt_TryObtainingLock(objectId, job->jobInfo->jobId, job->cryptoKeyId, job->jobPrimitiveInputOutput.mode) == E_OK)
      {
        Crypto_30_Vtt_SetDriverObjectState(objectId, CRYPTO_30_VTT_DRIVEROBJECTSTATE_PROCESSING); /* SBSW_CRYPTO_30_VTT_CSL01_OBJECTID */
        processJob = TRUE;
      }
    }
    SchM_Exit_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();

    if (processJob == TRUE)
    {
      /* #10 Remove job from queue if it was processed or an error occurred. */
      (void)Crypto_30_Vtt_Process(objectId, job); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */

      /* Remove job from queue and release processing */
      Crypto_30_Vtt_SetQueue(queueIdx, NULL_PTR); /* SBSW_CRYPTO_30_VTT_CSL02_QUEUE_VIA_DRIVEROBJECT */
      Crypto_30_Vtt_SetDriverObjectState(objectId, CRYPTO_30_VTT_DRIVEROBJECTSTATE_IDLE); /* SBSW_CRYPTO_30_VTT_CSL01_OBJECTID */
    }
  }
}

/**********************************************************************************************************************
 *  Crypto_30_Vtt_ProcessJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_ProcessJob(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* [SWS_Crypto_00057] */
  /* #5 Check initialization state of the component. */
  if (Crypto_30_Vtt_ModuleInitialized == (uint8)CRYPTO_30_VTT_UNINIT)
  {
    errorId = CRYPTO_E_UNINIT;
  }
  /* [SWS_Crypto_00058] */
  /* #10 Check plausibility of parameters. */
  else if (objectId >= Crypto_30_Vtt_GetSizeOfDriverObject())
  {
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  /* [SWS_Crypto_00059] Check if job is a null pointer */
  else if (job == NULL_PTR)
  {
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (Crypto_30_Vtt_Local_DetChecksServiceValues(job, &errorId) == E_NOT_OK) /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
  {
    /* Error Id is set by sub-function */
  }
  else
#endif
  {
    /* #15 Directly process job if it is synchronous. */
    if (job->jobPrimitiveInfo->processingType == CRYPTO_PROCESSING_SYNC)
    {
      SchM_Enter_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();
      retVal = Crypto_30_Vtt_TryObtainingLock(objectId, job->jobInfo->jobId, job->cryptoKeyId, job->jobPrimitiveInputOutput.mode);
      SchM_Exit_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();
      if (retVal == E_OK)
      {
        Crypto_30_Vtt_SetDriverObjectState(objectId, CRYPTO_30_VTT_DRIVEROBJECTSTATE_PROCESSING); /* SBSW_CRYPTO_30_VTT_CSL01_OBJECTID */

        retVal = Crypto_30_Vtt_Process(objectId, job); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
        Crypto_30_Vtt_SetDriverObjectState(objectId, CRYPTO_30_VTT_DRIVEROBJECTSTATE_IDLE); /* SBSW_CRYPTO_30_VTT_CSL01_OBJECTID */
      }
    }
    else
    {
      /* #20 Enqueue job if it is asynchronous and the queue is not full and the lock is not occupied by another job. */
      Crypto_30_Vtt_QueueIdxOfDriverObjectType queueIdx = Crypto_30_Vtt_GetQueueIdxOfDriverObject(objectId);
      retVal = CRYPTO_E_QUEUE_FULL;

      SchM_Enter_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();
      /* If job is asynchronous and job queue is not full, queue job. Otherwise return CRYPTO_E_QUEUE_FULL. */
      if (Crypto_30_Vtt_GetQueue(queueIdx) == NULL_PTR)
      {
        if ((Crypto_30_Vtt_IsLockFree(objectId))
          || ((Crypto_30_Vtt_IsLockOccupiedByJob(objectId, job->jobInfo->jobId)) == TRUE))
        {
          Crypto_30_Vtt_SetQueue(queueIdx, job); /* SBSW_CRYPTO_30_VTT_CSL02_QUEUE_VIA_DRIVEROBJECT */
          retVal = E_OK;
        }
      }
      SchM_Exit_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();
    }
  }

   /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYPTO_30_VTT_MODULE_ID, CRYPTO_30_VTT_INSTANCE_ID, CRYPTO_30_VTT_SID_PROCESS_JOB, errorId);
  }
#else
  CRYPTO_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
*  Crypto_30_Vtt_CancelJob()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
 *
*/
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_CancelJob(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_Crypto_00123] */
  /* #05 Check initialization state of the component. */
  if (Crypto_30_Vtt_ModuleInitialized == (uint8)CRYPTO_30_VTT_UNINIT)
  {
    errorId = CRYPTO_E_UNINIT;
  }
  /* [SWS_Crypto_00124] */
  /* #10 Check plausibility of parameters. */
  else if (objectId >= Crypto_30_Vtt_GetSizeOfDriverObject())
  {
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  /* [SWS_Crypto_00125] Check if job is a null pointer */
  else if (job == NULL_PTR)
  {
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else
#endif
  {

    SchM_Enter_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();
    /* Driver object is not processing anything at the moment */
    if (Crypto_30_Vtt_GetDriverObjectState(objectId) == CRYPTO_30_VTT_DRIVEROBJECTSTATE_IDLE)
    {
      Crypto_30_Vtt_QueueIdxOfDriverObjectType queueIdx = Crypto_30_Vtt_GetQueueIdxOfDriverObject(objectId);
      Crypto_30_Vtt_QueueElementType queuedJob = Crypto_30_Vtt_GetQueue(queueIdx);

      if ((queuedJob != NULL_PTR) && (queuedJob->jobInfo->jobId == job->jobInfo->jobId))
      {
        Crypto_30_Vtt_SetQueue(queueIdx, NULL_PTR); /* SBSW_CRYPTO_30_VTT_CSL02_QUEUE_VIA_DRIVEROBJECT */
        retVal = E_OK;
      }

      if (Crypto_30_Vtt_IsLockOccupiedByJob(objectId, job->jobInfo->jobId))
      {
        Crypto_30_Vtt_FreeLock(objectId); /* SBSW_CRYPTO_30_VTT_CSL01 */
        job->state = CRYPTO_JOBSTATE_IDLE; /* SBSW_CRYPTO_30_VTT_ACCESS_OF_JOB_MEMBER_CANCEL_JOB */
        Crypto_30_Vtt_Local_KeyReadLockReleaseNotProtected(job->cryptoKeyId);
        retVal = E_OK;
      }
    }
    SchM_Exit_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();

    if ((retVal == E_OK) && (job->jobPrimitiveInfo->processingType == CRYPTO_PROCESSING_ASYNC))
    {
      CryIf_CallbackNotification(job, CRYPTO_E_JOB_CANCELED); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYPTO_30_VTT_MODULE_ID, CRYPTO_30_VTT_INSTANCE_ID, CRYPTO_30_VTT_SID_CANCEL_JOB, errorId);
  }
#else
  CRYPTO_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

#define CRYPTO_30_VTT_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
* Module specific MISRA deviations
*********************************************************************************************************************/
/* Module specific MISRA deviations:

MD_CRYPTO_30_VTT_8.7:
Reason:     The deceleration is on block. However the static source is only required by one function.
Risk:       None
Prevention: Not necessary

MD_CRYPTO_30_VTT_19.7:
Reason:     Since this data service defines a basic implementation and can be user defined as well it is necessary to use macros.
Risk:       None
Prevention: Not necessary

MD_CRYPTO_30_VTT_0277:
Reason:     Cast is needed since lower layer crypto library expects -1 as parameter value.
Risk:       Cast from signed to unsigned results in an invalid value.
Prevention: Covered by code review.

MD_CRYPTO_30_VTT_0310:
Reason:     Cast is needed since lower layer crypto library expects super type as parameter. Internally it is casted back to concrete workspace type.
Risk:       Wrong workspace type is casted or size does not match.
Prevention: Covered by code review.

MD_CRYPTO_30_VTT_0311:
Reason:     Parameter has to be casted to var since lower layer crypto library expects a var parameter even if it does not write to it.
Risk:       Lower layer API could modify the given parameter.
Prevention: Covered by code review and component test.

MD_CRYPTO_30_VTT_3354:
Reason:     Parameter is set in future code execution. The code section is only reachable if the value is already set before.
Risk:       This function argument points to data that is possibly unset.
Prevention: Covered by code review and component test.

MD_CRYPTO_30_VTT_3395:
Reason:     Define is provided by header of lower layer crypto library.
Risk:       Compiler wrongly calculates the value.
Prevention: Covered by component test.

MD_CRYPTO_30_VTT_3673_1:
Reason:     Parameter cannot be set to const since it is passed as var parameter to lower layer crypto library.
Risk:       None
Prevention: None

MD_CRYPTO_30_VTT_3673_2:
Reason:     Pointer can only be const when method is empty and only Dummy statements are used.
Risk:       None
Prevention: Covered by code review.

MD_CRYPTO_30_VTT_3673_API_REQUIREMENT
Reason:     Pointer cannot be const because of the Autosar API requirements.
Risk:       None
Prevention: None

MD_CRYPTO_30_VTT_5013:
Reason:     Signed integer type is given as return type by lower layer crypto library. Size does not matter since only -1 and 0 is returned.
Risk:       None
Prevention: None

MD_CRYPTO_30_VTT_0310_CRYPTOLIBRARY_CAST:
Reason:     It is necessary to access inner members of the structure and therefore, the cast has to be applied.
Risk:       None
Prevention: Covered by code review.

*/

/*
\CM CM_CRYPTO_30_VTT_STACK_ARRAY_WITH_SIZE The stack arrays are given with its size to the called API.
\CM CM_CRYPTO_30_VTT_STACK_ARRAY_WITHOUT_SIZE The stack array has the same length as expected by the called API.
\CM CM_CRYPTO_30_VTT_WORKSPACE_STACK The workspace is located on the stack and the pointer is valid throughout the call.
\CM CM_CRYPTO_30_VTT_WORKSPACE_VIA_COMSTACKLIB_RUNTIME The workspace is received via ComStackLib access macros. As it is an 0:1 relation, a runtime check for availability is performed in the Dispatching function.
\CM CM_CRYPTO_30_VTT_JOB_INPUT_OUTPUT_BUFFERS_RUNTIME All job concerning in- and output buffer were checked in Crypto_30_Vtt_DetChecksServiceValues before based on service and mode and therefore are valid in this context.
*/

/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_CRYPTO_30_VTT_FORWARDING_PTR
\DESCRIPTION    The caller forwards as parameter one or more pointer.
\COUNTERMEASURE \N The pointer is forwarded and assumed to be valid.

\ID SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER
\DESCRIPTION The function is called with pointers pointing to stack buffers.
\COUNTERMEASURE \N Either the sizes of the buffers are given in the call or the sizes are according to the callee requirements.

\ID SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER
\DESCRIPTION The function is called with pointers pointing to stack buffers and pointer given as parameter to the caller.
\COUNTERMEASURE \N Either the sizes of the buffers are given in the call or the sizes are according to the callee requirements. It is assumed that the pointer given as parameter to the caller are valid.

\ID SBSW_CRYPTO_30_VTT_KEY_OUTPUT_BUFFER_PARAMETER_KEY_STACK
\DESCRIPTION Crypto_30_Vtt_SheKeyUpdateDecryptKey is called and a pointer to a keybuffer and to an output buffer is passed as parameter. In addition to that, a pointer to a local key buffer located on the stack is passed as parameter.
\COUNTERMEASURE \N The output buffer and the keybuffer are passed as argument to the caller and are assumed to be valid and of the correct size as requested by the callee.
                   The size of the local key buffer is according to the requirements of the callee.

\ID SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE_WITH_KEY_PTR
\DESCRIPTION The esl library is called and a pointer to a workspace is forwarded which is located on the stack. A pointer to a key buffer is provided with a size which is smaller then the provided buffer.
\COUNTERMEASURE \R [CM_CRYPTO_30_VTT_WORKSPACE_STACK]
                   The key pointer is checked in function Crypto_30_Vtt_Local_KeyElementSet for size.

\ID SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE_WITH_DATA_PTR
\DESCRIPTION The esl library is called and a pointer to a workspace is forwarded which is located on the stack. A pointer to a data buffer is given with a size which is smaller then or equal to the provided buffer.
\COUNTERMEASURE \N [CM_CRYPTO_30_VTT_WORKSPACE_STACK]
                   The data buffer is passed as argument to the caller and is assumed to be valid.

\ID SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE_WITH_OUTPUT_PTR
\DESCRIPTION The esl library is called and a pointer to a workspace is forwarded which is located on the stack. A pointer to an output buffer is provided without a reference to the size of the buffer.
\COUNTERMEASURE \N [CM_CRYPTO_30_VTT_WORKSPACE_STACK]
                   The output buffer is passed as argument to the caller and is assumed to be valid and of the correct size as requested by the callee.

\ID SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE_WITH_DATA_OUTPUT
\DESCRIPTION The esl library is called and a pointer to a workspace is forwarded which is located on the stack. A pointer to an output buffer is given without a reference to the size of the buffer.
             In addition to that, a pointer to a data buffer is given with a size which is smaller or equal to the provided buffer.
\COUNTERMEASURE \N [CM_CRYPTO_30_VTT_WORKSPACE_STACK]
                   The output buffer is passed as argument to the caller and is assumed to be valid and of the correct size as requested by the callee.
                   The data buffer is passed as argument to the caller and is assumed to be valid.

\ID SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE
\DESCRIPTION The esl library is called and a pointer to a workspace is forwarded which is located on the stack.
\COUNTERMEASURE \N [CM_CRYPTO_30_VTT_WORKSPACE_STACK]

\ID SBSW_CRYPTO_30_VTT_ESL_CONST_WORKSPACE
\DESCRIPTION The esl library is called and a pointer to a workspace is forwarded which is located on the const ram.
\COUNTERMEASURE \N The workspace is located on the const ram and the pointer is valid throughout the call.

\ID SBSW_CRYPTO_30_VTT_WORKSPACE_STACK_WITH_STACK_BUFFERS
\DESCRIPTION The esl library is called and a pointer to a workspace is forwarded which is located on the stack. Other parameters are located on the stack and have the same size as expected from the called API.
\COUNTERMEASURE \N [CM_CRYPTO_30_VTT_STACK_ARRAY_WITHOUT_SIZE]

\ID SBSW_CRYPTO_30_VTT_WORKSPACE_STACK_WITH_STACK_BUFFERS_AND_PTR_AS_PARAMETER
\DESCRIPTION  The esl library is called and a pointer to a workspace is forwarded which is located on the stack.
              Other parameters are located on the stack and have the same size as expected from the called API.
              In addition, there is one pointer which is already given as parameter to the calling function.
\COUNTERMEASURE \N  [CM_CRYPTO_30_VTT_WORKSPACE_STACK]
                    [CM_CRYPTO_30_VTT_STACK_ARRAY_WITHOUT_SIZE]

\ID SBSW_CRYPTO_30_VTT_PTR_ACCESS_INLINE_STACK
\DESCRIPTION Write access to a pointer which is passed to the inlined function. The memory pointed to by the pointer is located in the stack of the calling function.
\COUNTERMEASURE \N The pointer points to memory located on the stack. The call context is synchronous and therefore, the memory stays valid throughout the call.

\ID SBSW_CRYPTO_30_VTT_FORWARDING_OF_KEYPTR
\DESCRIPTION A pointer to a key buffer is forwarded to the callee without a size.
\COUNTERMEASURE \R The key pointer is checked in function Crypto_30_Vtt_Local_KeyElementSet for size and matches the requirements of the callee.

\ID SBSW_CRYPTO_30_VTT_FORWARDING_OF_KEYPTR_KEY_OUTPUT
\DESCRIPTION A pointer to a key buffer is provided with its size and a pointer to another key array located on the stack is provided without size. In addition to that, an output buffer is forwarded to the callee.
\COUNTERMEASURE \R The key pointer is checked in function Crypto_30_Vtt_Local_KeyElementSet for size.
                   The array located on the stack is sufficiently large and the call context is synchronous. The output buffer is only forwarded and assumed to be valid.

\ID SBSW_CRYPTO_30_VTT_FORWARDING_OF_KDFBUFFER_AND_KEY
\DESCRIPTION The KDFbuffer is forwarded to the callee with its size. The key ptr is pointing to a stack array which corresponds to the API requirements.
\COUNTERMEASURE \N KDFbuffer is only forwarded and assumed to be valid. The buffer is located on the stack and the call is synchronous.

\ID SBSW_CRYPTO_30_VTT_FORWARDING_OF_KDFBUFFER_KEY_OUTPUTBUFFER
\DESCRIPTION The KDFbuffer is located on the stack and given with its size to the callee. The outputbuffer is also located on the stack and matches the callees requirements in terms of size. The pointer to the key is given as parameter to the caller and only forwarded to the callee.
\COUNTERMEASURE \N The call is synchronous and the stack arrays stay valid throughout the call. The key pointer is assumed to be valid.

\ID SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS
\DESCRIPTION Write access to array given by pointer through a parameter. The API requires the array to have a certain size and it is assumed that the size is available in the given pointer.
\COUNTERMEASURE \N The caller has to assure that the API requirements are matched and the size of the buffer is according to these requirements.

\ID SBSW_CRYPTO_30_VTT_CSL01
\DESCRIPTION Access of Queue, KeyElementWrittenLength, KeyElement, RandomSeedBuffer and Lock using ComStackLib.
\COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_CRYPTO_30_VTT_CSL01_OBJECTID
\DESCRIPTION Access of WrittenLength, BufferLength, DriverObjectState and Lock using ComStackLib. The GetSizeOf-check is performed in one of the global APIs Crypto_30_Vtt_CancelJob or Crypto_30_Vtt_ProcessJob.
\COUNTERMEASURE \N Qualified use case CSL01 of ComStackLib.

\ID SBSW_CRYPTO_30_VTT_CSL02_QUEUE_VIA_DRIVEROBJECT
\DESCRIPTION Access of Queue (via indirection over DriverObject) with same index using ComStackLib.
\COUNTERMEASURE \N Qualified use case CSL02 of ComStackLib.

\ID SBSW_CRYPTO_30_VTT_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT
\DESCRIPTION Access of KeyStorage (via indirection over keyElement) with same index using ComStackLib.
\COUNTERMEASURE \N Qualified use case CSL02 of ComStackLib.

\ID SBSW_CRYPTO_30_VTT_CSL02_KEY_ELEMENT_VIA_KEY
\DESCRIPTION Access of KeyElement (via indirection over Key) with same index using ComStackLib.
\COUNTERMEASURE \N Qualified use case CSL02 of ComStackLib.

\ID SBSW_CRYPTO_30_VTT_STACK_ARRAY_ACCESS_WITH_SIZE
\DESCRIPTION Access of array located on stack.
\COUNTERMEASURE \N The static size of the array is used as upper bound and therefore no illegal write access is done.

\ID SBSW_CRYPTO_30_VTT_STACK_VARIABLE_AS_PTR
\DESCRIPTION A variable located on the stack is given as parameter pointer to a function.
\COUNTERMEASURE \N The pointer is forwarded and assumed to be valid.

\ID SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR
\DESCRIPTION A pointer to an array located on the stack is given as parameter to a function including its size.
\COUNTERMEASURE \N The function call is synchronous and the size of the array is given, therefore the stack array is valid for the function call.

\ID SBSW_CRYPTO_30_VTT_ACCESS_TO_ERRORID_PTR
\DESCRIPTION Access of errorId pointer to set the errorId. The context is an inlined function which is always called synchronously from Crypto_30_Vtt_Process,
             where the variable is located on the stack.
\COUNTERMEASURE \N The function call is synchronous and the variable errorId has to be located on the stack.

\ID SBSW_CRYPTO_30_VTT_FORWARDING_OF_JOB_PTR
\DESCRIPTION The call is only forwarded and has as parameter a job pointer.
\COUNTERMEASURE \N The pointer is forwarded and assumed to be valid.

\ID SBSW_CRYPTO_30_VTT_UPDATE_OF_JOB_STATE
\DESCRIPTION Direct write access to state member of job ptr. The context is the inlined function Crypto_30_Vtt_UpdateJobState,
             which is always called synchronously from Crypto_30_Vtt_Process.
\COUNTERMEASURE \R The ptr is checked in the global API Crypto_ProcessJob or Crypto_MainFunction.

\ID SBSW_CRYPTO_30_VTT_DIRECT_ACCESS_TO_JOB_STATE
\DESCRIPTION Direct write access to state member of job ptr. The context is the inlined function Crypto_30_Vtt_Process,
             which is always called synchronously from Crypto_30_Vtt_ProcessJob or Crypto_30_Vtt_MainFunction.
\COUNTERMEASURE \R The job object is already checked in global API.

\ID SBSW_CRYPTO_30_VTT_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR
\DESCRIPTION Write access to a pointer of the jobs primitive input output object.
\COUNTERMEASURE \R [CM_CRYPTO_30_VTT_JOB_INPUT_OUTPUT_BUFFERS_RUNTIME]

\ID SBSW_CRYPTO_30_VTT_ACCESS_OF_JOB_MEMBER_CANCEL_JOB
\DESCRIPTION Write access to the state member of job pointer.
\COUNTERMEASURE \N The pointer is checked for validity in the beginning of the Crypto_30_Vtt_CancelJob function.

\ID SBSW_CRYPTO_30_VTT_KEY_STORAGE_ACCESS_INIT
\DESCRIPTION Direct array access to key storage array generated by ComStackLib.
             The access is in bounds since it is iterated over all key elements and the access is either direct or via indirection (KeyElement to KeyStorage).
\COUNTERMEASURE \N Qualified use case CSL02 of ComStackLib.

\ID SBSW_CRYPTO_30_VTT_INIT_ESL_WORKSPACE
\DESCRIPTION The function esl_initWorkSpaceHeader is called with a pointer to the workspace header and a size.
             The workspace pointer was received by calling Crypto_30_Vtt_GetWorkspaceOf<Name of Primitive> which is provided by the ComStackLib.
\COUNTERMEASURE \R CM_CRYPTO_30_VTT_WORKSPACE_VIA_COMSTACKLIB_RUNTIME

\ID SBSW_CRYPTO_30_VTT_INIT_ESL_STACK_WORKSPACE
\DESCRIPTION The function esl_initWorkSpaceHeader is called with a pointer to the workspace header and a size. The workspace is located on the stack.
\COUNTERMEASURE \N The call to esl_initWorkSpaceHeader is synchronous and therefore, the workspace stays valid during the lifetime of the call.

\ID SBSW_CRYPTO_30_VTT_ACT_RANDOM_WORKSPACE_ARRAY_ACCESS
\DESCRIPTION Array access to X_KEY member of random workspace pointer of type ActFIPS186STRUCT.
\COUNTERMEASURE \N The validity of the pointer to the random workspace is given since the workspace is provided by the ComStackLib and the access is in bounds.
                   Since the accessing index is limited by the static size of the X_KEY array, it is ensured that is in range.

\ID SBSW_CRYPTO_30_VTT_ACT_RANDOM_WORKSPACE_MEMBER_ACCESS
\DESCRIPTION Access to bytes_remain member of random workspace pointer of type ActFIPS186STRUCT.
\COUNTERMEASURE \N The validity of the pointer to the random workspace is given since the workspace is provided by the ComStackLib.

\ID SBSW_CRYPTO_30_VTT_WORKSPACE_MEMBER_ACCESS
\DESCRIPTION Access to member of workspace pointer.
\COUNTERMEASURE \N The validity of the pointer to the workspace is given since the workspace is provided by the ComStackLib.

\ID SBSW_CRYPTO_30_VTT_ESL_CALL_WITH_WORKSPACE
\DESCRIPTION The esl library is called and a pointer to a workspace is forwarded. The workspace pointer points to a workspace provided by ComStackLibrary via Indirection over ObjectId.
\COUNTERMEASURE \R [CM_CRYPTO_30_VTT_WORKSPACE_VIA_COMSTACKLIB_RUNTIME]

\ID SBSW_CRYPTO_30_VTT_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS
\DESCRIPTION The esl library is called and a pointer to a workspace as well as in - and output buffers are forwarded.
             The pointer to the workspace is already initialized before and therefore valid. All other buffers are checked before in local function Crypto_30_Vtt_Local_DetChecksServiceValues,
             which is called in the context of the global API Crypto_30_Vtt_Process.
\COUNTERMEASURE \R [CM_CRYPTO_30_VTT_WORKSPACE_VIA_COMSTACKLIB_RUNTIME]
                \R [CM_CRYPTO_30_VTT_JOB_INPUT_OUTPUT_BUFFERS_RUNTIME]

\ID SBSW_CRYPTO_30_VTT_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER
\DESCRIPTION The esl library is called and a pointer to a workspace as well as buffer and lengths of stack variables are given as parameter to the function call.
             The pointer to the workspace is already initialized before and therefore valid. All other buffers are located on the stack and the function call is synchronous. Therefore, the memory
             stays valid throughout the call.
\COUNTERMEASURE \N All pointers point to stack variables and the length is also provided to the called function.

\ID SBSW_CRYPTO_30_VTT_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER
\DESCRIPTION The esl library is called with a pointer to a workspace,
             a pointer located at the job object (pointing to a buffer) with an length denoting the length of the buffer
             and a pointer to an buffer which is located on the stack and without size.
\COUNTERMEASURE \R  [CM_CRYPTO_30_VTT_WORKSPACE_VIA_COMSTACKLIB_RUNTIME]
                    [CM_CRYPTO_30_VTT_JOB_INPUT_OUTPUT_BUFFERS_RUNTIME]
                    [CM_CRYPTO_30_VTT_STACK_ARRAY_WITHOUT_SIZE]

\ID SBSW_CRYPTO_30_VTT_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR
\DESCRIPTION The esl library is called with a pointer to a workspace and a null pointer is given to the library as parameter.
\COUNTERMEASURE \N [CM_CRYPTO_30_VTT_WORKSPACE_VIA_COMSTACKLIB_RUNTIME]
                   The null pointer is a valid input to the calling function.

\ID SBSW_CRYPTO_30_VTT_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN
\DESCRIPTION The esl library is called and a pointer to a workspace as well as pointer to stack arrays are given without an information how large the arrays are.
             This is due to the fact that these esl library function do not provide an parameter for the length of the buffer.
\COUNTERMEASURE \N The stack arrays have the same length as the esl library expects.

\ID SBSW_CRYPTO_30_VTT_CALL_WITH_WORKSPACE
\DESCRIPTION The service is called and a pointer to a workspace is forwarded. The workspace pointer points to a workspace provided by ComStackLibrary via Indirection over ObjectId.
\COUNTERMEASURE \R [CM_CRYPTO_30_VTT_WORKSPACE_VIA_COMSTACKLIB_RUNTIME]

\ID SBSW_CRYPTO_30_VTT_VARIABLE_ACCESS_PTR_FORM_STACK
\DESCRIPTION The value of a pointer to a variable located on the stack is written.
\COUNTERMEASURE \N The function call is synchronous and there is no array access.

\ID SBSW_CRYPTO_30_VTT_ARRAY_ACCESS_WITH_SIZE_CHECK
\DESCRIPTION The array is written with an index, the size of the array is known in this context.
\COUNTERMEASURE \R It is checked that there is no out of bounds via the provided length.

\ID SBSW_CRYPTO_30_VTT_PTR_ACCESS_WITH_DET_CHECK
\DESCRIPTION Write access to a pointer.
\COUNTERMEASURE \R The pointer is checked for validity before.

\ID SBSW_CRYPTO_30_VTT_PTR_FORWARDING_WITH_DET_CHECK
\DESCRIPTION One or more pointer are forwarded as parameter to the callee.
\COUNTERMEASURE \R The pointer are checked for validity before.

\ID SBSW_CRYPTO_30_VTT_RNG_SEED
\DESCRIPTION Access to KeyElementState via indirection over Key.
\COUNTERMEASURE \N The index is not explicitly checked. Since the code is only executed if the Crypto_30_Vtt_KeyElementSet has successfully written the same KeyElement, it is guaranteed that the key element exist and therefore the index is also valid.

\ID SBSW_CRYPTO_30_VTT_GETVERSIONINFO
\DESCRIPTION The function Crypto_30_Vtt_GetVersionInfo writes to the object referenced by parameter VersionInfo
\COUNTERMEASURE \N The caller ensures that the pointers passed to the parameter VersionInfo is valid.

SBSW_JUSTIFICATION_END */

/* COV_JUSTIFICATION_BEGIN

\ID COV_CRYPTO_30_VTT_VAR_ELISA_STD_ON_TX
\ACCEPT TX
\REASON Feature must always be enabled for SafeBsw secured by the Elisa plugin.

\ID COV_CRYPTO_30_VTT_VAR_OPTIONAL_DEFINE_TX
\ACCEPT TX
\REASON This encapsulation is optional and already added for future releases.

\ID COV_CRYPTO_30_VTT_LOCAL_DEFINE
\ACCEPT XF
\REASON CRYPTO_30_VTT_LOCAL is always defined externally.

\ID COV_CRYPTO_30_VTT_VIRTUALKEY
\ACCEPT XF
\REASON Virtual keys are still beta.

\ID COV_CRYPTO_30_VTT_CSM_DEFINES
\ACCEPT XF
\ACCEPT TX
\REASON This defines could be defined in other crypto drivers as well. In order to increase flexibility, defines are defined in crypto driver if not provided yet.

\ID COV_CRYPTO_30_VTT_MISRA_ADDITIONAL_STATEMENT
\ACCEPT X
\REASON This else/default statements are required by misra but cannot be reached in code. The statement is captured by upper Api.

\ID COV_CRYPTO_30_VTT_ACTBN_BYTES_PER_DIGIT_DEFINE
\ACCEPT XF
\ACCEPT TX
\REASON This encapsulation is for different platform types. Not all platform types can be covered by development test. Covered by code review and integration test.

\ID COV_CRYPTO_30_VTT_BIT_ORDER
\ACCEPT XF
\ACCEPT TX
\REASON This encapsulation is for different platform types. Not all platform types can be covered by development test. Covered by code review and integration test.

\ID COV_CRYPTO_30_VTT_WATCHDOG_FCTN_PTR
\ACCEPT TX
\REASON The watchdog pointer, required for the esLib, is typically define by the crypto module as NULL, but can be user defined.

\ID COV_CRYPTO_30_VTT_CUSTOM_KEY_ELEMENTS
\ACCEPT TX
\REASON The Crypto needs custom key elements, which could be defined in other modules as well.

\ID COV_CRYPTO_30_VTT_KEY_ELEMENTS_RIGHTS
\ACCEPT TX
\REASON The Crypto provides key elements rights which can be defined in upper layer.

\ID COV_CRYPTO_30_VTT_CUSTOM_ALGORITHM
\ACCEPT TX
\REASON The Crypto needs custom algorithm and algorithm mode defines, which could be defined in other modules as well.

\ID COV_CRYPTO_30_VTT_CUSTOM_KEY_ALGORITHM
\ACCEPT TX
\REASON The Crypto needs custom key algorithm, which could be defined in other modules as well.

\ID COV_CRYPTO_30_VTT_CUSTOM_KDF
\ACCEPT TX
\REASON The Crypto needs custom key algorithm, which could be defined in other modules as well.

\ID COV_CRYPTO_30_VTT_KEY_LOCKING_RELEASE
\ACCEPT TX
\REASON This check can only be false if there is a implementation mistake.

COV_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_Vtt.c
 *********************************************************************************************************************/
