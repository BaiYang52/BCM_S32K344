/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Csm.c
 *        \brief  MICROSAR Crypto Service Manager (CSM)
 *
 *      \details  Implementation of the MICROSAR Crypto Service Manager (CSM)
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

#define CSM_SOURCE
/* PRQA S 6060 EOF */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Csm.h"
#include "Csm_Cbk.h"
#include "CryIf.h"
#include "SchM_Csm.h"

#if (CSM_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

 /* Check the version of CSM header file */
#if ((CSM_SW_MAJOR_VERSION != (6u)) \
    || (CSM_SW_MINOR_VERSION != (3u)) \
    || (CSM_SW_PATCH_VERSION != (0u)))
# error "Vendor specific version numbers of Csm.c and Csm.h are inconsistent"
#endif

/* AUTOSAR version information check has to match definition in header file */
#if ((CSM_AR_RELEASE_MAJOR_VERSION != (4u)) \
    || (CSM_AR_RELEASE_MINOR_VERSION != (4u)) \
    || (CSM_AR_RELEASE_REVISION_VERSION != (0u)))
# error "AUTOSAR Specification Version numbers of Csm.c and Csm.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/*! Callback is idle and must not be notified to RTE. */
#define CSM_CALLBACK_IDLE                                             (0xFEu)

/*! Job ID which is used to mark Job as empty/free for usage. */
#define CSM_EMPTY_JOB                                                 (0xFFFFFFFFuL)

/* States used for internal job state of Csm. */

/*! Job is IDLE - Read to be used by application */
#define CSM_JOB_STATE_IDLE                                            CRYPTO_JOBSTATE_IDLE

/*! Job is Active - That means was started but waits for a further UPDATE or the FINISH call */
#define CSM_JOB_STATE_ACTIVE                                          CRYPTO_JOBSTATE_ACTIVE

/*! Job is currently progressing in lower layer - Csm is waiting for function to return or for callback notification. */
#define CSM_JOB_STATE_PROGRESSING                                     CRYPTO_JOBSTATE_PROGRESSING

/*! An asynchronous job was passed to lower layer - Csm is waiting for the callback, job is now cancelable as known by
 *  lower layer */
#define CSM_JOB_STATE_WAITING                                         3u

/*! Asynchronous job is on QUEUE */
#define CSM_JOB_STATE_QUEUED                                          4u

/*! Job cancelling pending - currenlty the job is tried to be canceled in lower layer */
#define CSM_JOB_STATE_CANCELING                                       5u

/*! Asynchronous Job was cancelled in lower layer but CallbackNotification is required to complete cancelation. */
#define CSM_JOB_STATE_CANCELED                                        6u

/*! Asynchronous job's callback has occurred  */
#define CSM_JOB_STATE_CALLBACK_OCCURRED                               64u

/*! Clear mask for state Asynchronous job's callback has occurred */
#define CSM_JOB_STATE_CALLBACK_OCCURRED_CLEAR_MASK                    191u /* = ~CSM_JOB_STATE_CALLBACK_OCCURRED */

/*! State of asynchronous job retriggering  */
#define CSM_ASYNC_JOB_RETRIGGER_STATE_IDLE                            (0x00u)
#define CSM_ASYNC_JOB_RETRIGGER_STATE_PENDING                         (0x01u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (CSM_LOCAL) /* COV_CSM_LOCAL_DEFINE */
# define CSM_LOCAL                                                    static
#endif

#if !defined (CSM_LOCAL_INLINE) /* COV_CSM_LOCAL_DEFINE */
# define CSM_LOCAL_INLINE                                             LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define CSM_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CSM_DEV_ERROR_DETECT == STD_ON)
/*! Initialization state of the module */
CSM_LOCAL VAR(uint8, CSM_VAR_ZERO_INIT) Csm_IsInitialized = CSM_UNINIT;
#endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */

#define CSM_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CSM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CSM_JOB == STD_ON)
/**********************************************************************************************************************
 *  Csm_ProcessJob()
 *********************************************************************************************************************/
/*! \brief         Process the received job
 *  \details       This function unifies all external calls to call CryIf_ProcessJob.
 *  \param[in,out] job                     Pointer to the configuration of the job. Contains structures with user and
 *                                         primitive relevant information.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \return        CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \return        CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *  \return        CRYPTO_E_QUEUE_FULL     Request failed, the queue is full.
 *  \return        CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           Param queueIdx needs to be a valid index.
 *                 Job must point to a valid job object.
 *                 CSM_EXCLUSIVE_AREA_0 must be entered as it is left inside this function.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL FUNC(Std_ReturnType, CSM_CODE) Csm_ProcessJob(P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job);

/**********************************************************************************************************************
 *  Csm_GetJobObj()
 *********************************************************************************************************************/
/*! \brief         Fetches the job object for the given job id.
 *  \details       Job has either a dedicated object or it is received from the job pool
 *  \param[in]     jobId                   Holds the identifier of the job.
 *  \param[out]    jobObjId                Pointer which will hold the id of the job object to use.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \return        CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \return        CRYPTO_E_QUEUE_FULL     Request failed, the queue is full.
 *  \pre           Should only be called with locked interrupts
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL FUNC(Std_ReturnType, CSM_CODE) Csm_GetJobObj(uint32 jobId, P2VAR(Csm_SizeOfJobType, AUTOMATIC, AUTOMATIC) jobObjId);

# if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
/**********************************************************************************************************************
 *  Csm_PrepareJobObj()
 *********************************************************************************************************************/
/*! \brief         Set all fields of the given job structure to the data of the given job id.
 *  \details       -
 *  \param[in]     jobId                   Holds the identifier of the job.
 *  \param[out]    job                     Pointer to a job which will be adapted to the given job id.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_PrepareJobObj(uint32 jobId, P2VAR(Crypto_JobType, AUTOMATIC, AUTOMATIC) job);
# endif

# if (CSM_ASYNC_PROCESSING == STD_ON)
/**********************************************************************************************************************
 *  Csm_EnqueueJob()
 *********************************************************************************************************************/
/*! \brief         Enqueue Job.
 *  \details       This function is enqueuing a job to the given queue directly sorting by its priority.
 *  \param[in]     queueIdx                Holds the identifier of the queue.
 *  \param[in]     job                     Pointer to a job that shall be enqueued.
 *  \return        E_OK                    Request successful.
 *  \return        CRYPTO_E_QUEUE_FULL     Request failed, the queue is full.
 *  \pre           Param queueIdx needs to be a valid index.
 *  \note          Shall only be called with locked interrupts
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(Std_ReturnType, CSM_CODE) Csm_EnqueueJob(Csm_QueueInfoIterType queueIdx, P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job);

/**********************************************************************************************************************
 *  Csm_DequeueJob()
 *********************************************************************************************************************/
/*! \brief         Dequeue Job.
 *  \details       This function removes a given job id from the queue filling gaps.
 *  \param[in]     queueIdx                Holds the identifier of the queue.
 *  \param[in]     jobId                   Id of the job that shall be removed.
 *  \pre           Param queueIdx needs to be a valid index.
 *  \note          Shall only be called with locked interrupts
 *  \context       TASK|ISR
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL FUNC(void, CSM_CODE) Csm_DequeueJob(Csm_QueueInfoIterType queueIdx, uint32 jobId);

/**********************************************************************************************************************
 *  Csm_TriggerAsynchJobProcessing()
 *********************************************************************************************************************/
/*! \brief        Triggers the Processing of asynchronous jobs
 *  \details      This function checks for the given queue if an asynchronous job is available. If so, it triggers the
 *                CryIf to process this asynchronous job.
 *  \param[in]    queueIdx  Holds the identifier of the queue.
 *  \pre          Param queueIdx needs to be a valid index.
 *                Must be called within CSM_EXCLUSIVE_AREA_0.
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
CSM_LOCAL FUNC(void, CSM_CODE) Csm_TriggerAsynchJobProcessing(Csm_QueueInfoIterType queueIdx);

/**********************************************************************************************************************
 *  Csm_CancelAsynchronousJob()
 *********************************************************************************************************************/
/*! \brief        Cancels an asynchronous job
 *  \details      Depending on job state, the necessary steps for canceling an asynchronous jobs are executed
 *  \param[in]    job                Holds the asynchronous job object which shall be cancelled
 *  \return       E_OK                    Request successful - Job was cancelled or was already Idle
 *                E_NOT_OK                Request failed - Job could not be cancelled
 *                CRYPTO_E_JOB_CANCELED   Request pending - Job will be cancelled with next callback notification
 *  \pre          CSM_EXCLUSIVE_AREA_0 must be entered as is is left inside this function.
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(Std_ReturnType, CSM_CODE) Csm_CancelAsynchronousJob(P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job);

/**********************************************************************************************************************
 *  Csm_HandleApplicationCallback()
 *********************************************************************************************************************/
/*! \brief        Handles the application callback.
 *  \details      Calls the configured user callback function.
 *  \param[in]    job       Holds a pointer to the job, which has finished.
 *  \param[in]    result    Contains the result of the cryptographic operation.
 *  \param[out]   errorId   Pointer to a buffer where the ID of the detected development error shall be written to.
 *  \pre          CSM_EXCLUSIVE_AREA_0 must be entered as it is left inside this function.
 *  \context      TASK, ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_HandleApplicationCallback(
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
  Std_ReturnType result,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId);

#  if (CSM_RTECALLBACK == STD_ON)
/**********************************************************************************************************************
 *  Csm_HandleRteCallbacks()
 *********************************************************************************************************************/
 /*! \brief        Handles the RTE callbacks.
  *  \details      Calls the RTE callback functions.
  *  \pre          -
  *  \context      TASK
  *  \reentrant    FALSE
  *  \synchronous  TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_HandleRteCallbacks(void);
#  endif

# endif /* (CSM_ASYNC_PROCESSING == STD_ON) */

/**********************************************************************************************************************
 *  Csm_HandleJobProcessing()
 *********************************************************************************************************************/
/*! \brief         Triggers the CryIf to process the given job.
 *  \details       This function triggers the CryIf to process the corresponding job. Furthermore, pre- and postjob callouts are called from this function if enabled.
 *  \param[in]     channelId           Holds the identifier of the queue.
 *  \param[in,out] job                 Pointer to the corresponding job
 *  \pre           Param channelId needs to be a valid CryIf channel.
 *                 Param Job needs to be a valid pointer to a job object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL FUNC(Std_ReturnType, CSM_CODE) Csm_HandleJobProcessing(Csm_ChannelIdOfQueueInfoType channelId, P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job);

/**********************************************************************************************************************
 *  Csm_AppendRedirectionToJob()
 *********************************************************************************************************************/
/*! \brief         Appends redirection info to job object.
 *  \details       If the job is configured to use an in-out redirection, it is appended to the job object in this function.
 *  \param[in,out] job     Pointer to the corresponding job.
 *  \pre           Param job needs to point to a valid job object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_AppendRedirectionToJob(P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job);
#endif /* (CSM_JOB == STD_ON) */

#if (CSM_CALLOUT == STD_ON)
/**********************************************************************************************************************
 *  Csm_CallPreJobCallout()
 *********************************************************************************************************************/
/*! \brief         Triggers the calling of the configured prejob callout.
 *  \details       This function checks for the given job if an prejob callout has to be called. If so, it triggers the
 *                 call.
 *  \param[in,out] job     Pointer to the corresponding job.
 *  \pre           Param job needs to point to a valid job object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(Std_ReturnType, CSM_CODE) Csm_CallPreJobCallout(P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job);

/**********************************************************************************************************************
 *  Csm_CallPostJobCallout()
 *********************************************************************************************************************/
/*! \brief         Triggers the calling of the configured postjob callout.
 *  \details       This function checks for the given job if an postjob callout has to be called. If so, it triggers the
 *                 call.
 *  \param[in,out] job                       Pointer to the job.
 *  \param[in,out] retValFromProcessing      Pointer to the processing result of the corresponding job. It can be overwritten by the postjob callout.
 *  \pre           Param job needs to point to a valid job object.
 *  \pre           Param retValFromProcessing needs to point to a valid location.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL FUNC(void, CSM_CODE) Csm_CallPostJobCallout(P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job, P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) retValFromProcessing);
#endif /* (CSM_CALLOUT == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (CSM_CALLOUT == STD_ON)
/**********************************************************************************************************************
 *  Csm_CallPostJobCallout()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
CSM_LOCAL FUNC(void, CSM_CODE) Csm_CallPostJobCallout(P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job, P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) retValFromProcessing)
{
  Csm_CalloutInfoIdxOfJobTableType calloutInfoId;

  calloutInfoId = Csm_GetCalloutInfoIdxOfJobTable(job->jobId);

  /* #10 Check if postjob callout is configured for the given job. */
  if (calloutInfoId < Csm_GetSizeOfCalloutInfo())
  {
    /* #20 Call postjob callout and reset the callout state to IDLE. */
    (void)Csm_GetPostJobCalloutFuncOfCallout(Csm_GetCalloutIdxOfCalloutInfo(calloutInfoId))( /* SBSW_CSM_FUNCTION_POINTER */
      job,
      CSM_CALLOUT_STATE_POST_INITIAL,
      retValFromProcessing);

    Csm_SetCalloutStateOfCalloutState(calloutInfoId, CSM_CALLOUT_STATE_IDLE); /* SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE */
  }
}

/**********************************************************************************************************************
 *  Csm_CallPreJobCallout()
 *********************************************************************************************************************/
/*!
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
CSM_LOCAL_INLINE FUNC(Std_ReturnType, CSM_CODE) Csm_CallPreJobCallout(P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job)
{
  Csm_CalloutInfoIdxOfJobTableType calloutInfoId;
  Std_ReturnType retVal = E_OK;

  /* Retrieve reference to callout for given job */
  calloutInfoId = Csm_GetCalloutInfoIdxOfJobTable(job->jobId);

  /* Check if prejob callout is configured for the given job. */
  if (calloutInfoId < Csm_GetSizeOfCalloutInfo())
  {
# if (CSM_ASYNC_PROCESSING == STD_ON)
    uint8 calloutState = Csm_GetCalloutStateOfCalloutState(calloutInfoId);

    if( job->jobPrimitiveInfo->processingType == CRYPTO_PROCESSING_ASYNC)
    {
      /* If no callout has been called before or callout did not finish yet, call prejob. */
      if ((calloutState == CSM_CALLOUT_STATE_IDLE) || (calloutState == CSM_CALLOUT_STATE_PRE_PENDING))
      {
        /* Call prejob callout */
        retVal = Csm_GetPreJobCalloutFuncOfCallout(Csm_GetCalloutIdxOfCalloutInfo(calloutInfoId))( /* SBSW_CSM_FUNCTION_POINTER */
          job,
          (calloutState == CSM_CALLOUT_STATE_IDLE) ? (CSM_CALLOUT_STATE_PRE_INITIAL) : (CSM_CALLOUT_STATE_PRE_PENDING));

        /* Set state according to return value of callout. */
        switch (retVal)
        {
          case E_OK:
          {
            /* prejob callout has finished, Processing can start */
            Csm_SetCalloutStateOfCalloutState(calloutInfoId, CSM_CALLOUT_STATE_PROCESSING); /* SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE */
            break;
          }

          case CSM_E_PENDING:
          {
            /* prejob callout has not yet finished, this is only possible in asynchronous job configurations */
            Csm_SetCalloutStateOfCalloutState(calloutInfoId, CSM_CALLOUT_STATE_PRE_PENDING); /* SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE */
            break;
          }

          default:
          {
            Csm_SetCalloutStateOfCalloutState(calloutInfoId, CSM_CALLOUT_STATE_PROCESSING_ABORTED_BY_CALLOUT); /* SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE */
            /* Change retVal to E_OK to remove job from queue since processing shall never be started */
            retVal = E_OK;
            break;
          }
        }
      }
    }
    else
# endif /* (CSM_ASYNC_PROCESSING == STD_ON) */
    {
      /* Call prejob callout. */
      retVal = Csm_GetPreJobCalloutFuncOfCallout(Csm_GetCalloutIdxOfCalloutInfo(calloutInfoId))(job, CSM_CALLOUT_STATE_PRE_INITIAL); /* SBSW_CSM_FUNCTION_POINTER */

      /* Set state according to return value of callout. */
      if(retVal == E_OK)
      {
        /* If return value is equal to E_OK, the job shall be executed as without callout. */
        Csm_SetCalloutStateOfCalloutState(calloutInfoId, CSM_CALLOUT_STATE_PROCESSING); /* SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE */
      }
      else
      {
        /* If return value is different to E_OK, processing of the job shall be supressed. */
        Csm_SetCalloutStateOfCalloutState(calloutInfoId, CSM_CALLOUT_STATE_POST_INITIAL); /* SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE */
        /* Call postcallout. */
        Csm_CallPostJobCallout(job, &retVal); /* SBSW_CSM_PASS_JOB_AND_STACK */
      }
    }
  }
  return retVal;
}
#endif /* (CSM_CALLOUT == STD_ON) */

#if (CSM_JOB == STD_ON)
/**********************************************************************************************************************
 *  Csm_AppendRedirectionToJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_AppendRedirectionToJob(P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
# if (CSM_INOUTREDIRECTION == STD_ON)
  uint32 jobId = job->jobId;

  /* #10 If job in- or output shall be redirected, copy corresponding redirection sources and destinations to the job object. */
  if(Csm_IsInOutRedirectionMapUsedOfJobTable(jobId))
  {
    Csm_SizeOfInOutJobRedirectionTableType indexOfInOutRedirectionTable = Csm_GetInOutJobRedirectionTableIdxOfInOutRedirectionMap(Csm_GetInOutRedirectionMapIdxOfJobTable(jobId));
    Csm_RedirectionConfigValueOfInOutJobRedirectionTableType redirectionConfigValue = Csm_GetRedirectionConfigValueOfInOutJobRedirectionTable(indexOfInOutRedirectionTable);

    job->jobRedirectionInfoRef = Csm_GetAddrInOutRedirection(Csm_GetInOutRedirectionMapIdxOfJobTable(jobId)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobRedirectionInfoRef->redirectionConfig = redirectionConfigValue; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

    if((redirectionConfigValue & ((uint8)CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT)) == (uint8)CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT)
    {
      job->jobRedirectionInfoRef->inputKeyElementId = Csm_GetInputKeyElementIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobRedirectionInfoRef->inputKeyId = Csm_GetCryIfKeyIdOfKey(Csm_GetInputKeyIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    }

    if((redirectionConfigValue & ((uint8)CRYPTO_REDIRECT_CONFIG_SECONDARY_INPUT)) == (uint8)CRYPTO_REDIRECT_CONFIG_SECONDARY_INPUT)
    {
      job->jobRedirectionInfoRef->secondaryInputKeyElementId = Csm_GetSecondaryInputKeyElementIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobRedirectionInfoRef->secondaryInputKeyId = Csm_GetCryIfKeyIdOfKey(Csm_GetSecondaryInputKeyIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    }

    if((redirectionConfigValue & ((uint8)CRYPTO_REDIRECT_CONFIG_TERTIARY_INPUT)) == (uint8)CRYPTO_REDIRECT_CONFIG_TERTIARY_INPUT)
    {
      job->jobRedirectionInfoRef->tertiaryInputKeyElementId = Csm_GetTertiaryInputKeyElementIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobRedirectionInfoRef->tertiaryInputKeyId = Csm_GetCryIfKeyIdOfKey(Csm_GetTertiaryInputKeyIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    }

    if((redirectionConfigValue & ((uint8)CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT)) == (uint8)CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT)
    {
      job->jobRedirectionInfoRef->outputKeyElementId = Csm_GetOutputKeyElementIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobRedirectionInfoRef->outputKeyId = Csm_GetCryIfKeyIdOfKey(Csm_GetOutputKeyIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    }

    if((redirectionConfigValue & ((uint8)CRYPTO_REDIRECT_CONFIG_SECONDARY_OUTPUT)) == (uint8)CRYPTO_REDIRECT_CONFIG_SECONDARY_OUTPUT)
    {
      job->jobRedirectionInfoRef->secondaryOutputKeyElementId = Csm_GetSecondaryOutputKeyElementIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobRedirectionInfoRef->secondaryOutputKeyId = Csm_GetCryIfKeyIdOfKey(Csm_GetSecondaryOutputKeyIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    }
  }
  else
# endif /* (CSM_INOUTREDIRECTION == STD_ON) */
  {
    /* #20 Else set redirection to dummy value. */
# if (CSM_JOB_TYPE_LAYOUT_REDIRECTION_INFO_REF == STD_ON)
    job->jobRedirectionInfoRef = (P2VAR(Crypto_JobRedirectionInfoType, AUTOMATIC, CSM_VAR_NOINIT)) (NULL_PTR); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
# else
    CSM_DUMMY_STATEMENT(job); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  }
}

/**********************************************************************************************************************
 *  Csm_HandleJobProcessing()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CSM_LOCAL FUNC(Std_ReturnType, CSM_CODE) Csm_HandleJobProcessing(Csm_ChannelIdOfQueueInfoType channelId, P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job)
{
  Std_ReturnType retVal;

# if (CSM_CALLOUT == STD_ON)
  Csm_CalloutInfoIdxOfJobTableType calloutInfoId;

  /* Retrieve reference to callout for given job */
  calloutInfoId = Csm_GetCalloutInfoIdxOfJobTable(job->jobId);

  /* #10 Call preJobCallout */
  retVal = Csm_CallPreJobCallout(job); /* SBSW_CSM_PASS_JOB_POINTER_API_REQUIREMENT */

  if ((calloutInfoId >= Csm_GetSizeOfCalloutInfo()) || (Csm_GetCalloutStateOfCalloutState(calloutInfoId) == CSM_CALLOUT_STATE_PROCESSING))
# endif /* (CSM_CALLOUT == STD_ON) */
  {
    /* #20 Call CryIf to process given job */
    retVal = CryIf_ProcessJob(channelId, job); /* SBSW_CSM_PASS_JOB_POINTER_API_REQUIREMENT */
# if (CSM_CALLOUT == STD_ON)
    if (calloutInfoId < Csm_GetSizeOfCalloutInfo())
    {
      /* Job has finished, either successful (E_OK) or not */
#  if (CSM_ASYNC_PROCESSING == STD_ON)
      if (job->jobPrimitiveInfo->processingType == CRYPTO_PROCESSING_SYNC)
#  endif /* (CSM_ASYNC_PROCESSING == STD_ON) */
      {
        /* We need to call the post callout routine */
        Csm_SetCalloutStateOfCalloutState(calloutInfoId, CSM_CALLOUT_STATE_POST_INITIAL); /* SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE */
        Csm_CallPostJobCallout(job, &retVal); /* SBSW_CSM_PASS_JOB_POINTER_API_REQUIREMENT */
      }
    }
# endif /* # (CSM_CALLOUT == STD_ON) */
  }
  return retVal;
}

/**********************************************************************************************************************
 *  Csm_ProcessJob()
 *********************************************************************************************************************/
/*!
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
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
CSM_LOCAL FUNC(Std_ReturnType, CSM_CODE) Csm_ProcessJob(P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job)
{
  Std_ReturnType retVal;
  uint32 jobId = job->jobId;
  Csm_QueueInfoIterType queueIdx = Csm_GetQueueRefIdxOfJobTable(jobId);
  Csm_ChannelIdOfQueueInfoType channelId = Csm_GetChannelIdOfQueueInfo(queueIdx);
# if (CSM_ASYNC_PROCESSING == STD_ON)
  Crypto_JobStateType combinedJobState = job->jobState;

#  if (CSM_JOB_TYPE_LAYOUT_ASR430_COMPATIBILITY == STD_ON)
  combinedJobState |= job->state;
#  endif /* (CSM_JOB_TYPE_LAYOUT_ASR430_COMPATIBILITY == STD_ON) */
# endif

  /* #05 Append job redirection to job object */
  Csm_AppendRedirectionToJob(job); /* SBSW_CSM_PASS_JOB_POINTER_API_REQUIREMENT */

  /* Switch depending on whether there is asynchronous processing at all */
# if (CSM_ASYNC_PROCESSING == STD_ON)
  /* Check if the job is asynchronous. */
  if (job->jobPrimitiveInfo->processingType == CRYPTO_PROCESSING_ASYNC)
  {
    boolean gotLockOfQueue = (Csm_GetLockOfQueueState(queueIdx) == 0u) ? TRUE : FALSE;
    /* Job is asynchronous */
    /* #20 Determine if queue is empty or job is already active and queue is not locked
     *     (e.g. by this function) and call Csm_HandleJobProcessing if so */
    if ((gotLockOfQueue == TRUE) &&
      ((Csm_GetQueueIdxOfQueueState(queueIdx) == Csm_GetQueueStartIdxOfQueueInfo(queueIdx))
        || ((combinedJobState & CRYPTO_JOBSTATE_ACTIVE) == CRYPTO_JOBSTATE_ACTIVE)))
    {
      Csm_SetJobState(jobId, CSM_JOB_STATE_PROGRESSING); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
      Csm_IncLockOfQueueState(queueIdx);  /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID */
#  if (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON)
      Csm_SetRetriggeringOfQueueState(queueIdx, CSM_ASYNC_JOB_RETRIGGER_STATE_IDLE); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE */
#  endif
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
      /* Call CryIf_ProcessJob() */
      retVal = Csm_HandleJobProcessing(channelId, job); /* SBSW_CSM_PASS_JOB */

      SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
      Csm_DecLockOfQueueState(queueIdx);  /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID */
    }
    else
    {
      /* Set retVal temporarily to busy to force enqueuing */
      retVal = CRYPTO_E_BUSY;
    }

    /* #22 Enqueuing necessary if return value is either BUSY or QUEUE_FULL */
    if ((retVal == CRYPTO_E_BUSY) || (retVal == CRYPTO_E_QUEUE_FULL))
    {
      /* #23 Enqueue job or return QUEUE_FULL */
      retVal = Csm_EnqueueJob(queueIdx, job); /* SBSW_CSM_PASS_JOB */
#  if (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON)
      if (Csm_IsTriggerAsynchJobsInCallbackOfQueueInfo(queueIdx)) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
      {
        /* #24 If triggering asynchronous jobs is enabled for the queue handle the retriggering depending on whether
         *      getting the queue lock failed or not. */
        if (!gotLockOfQueue)
        {
          Csm_SetRetriggeringOfQueueState(queueIdx, CSM_ASYNC_JOB_RETRIGGER_STATE_PENDING); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE */
        }
        else if (Csm_GetRetriggeringOfQueueState(queueIdx) == CSM_ASYNC_JOB_RETRIGGER_STATE_PENDING)
        {
          Csm_TriggerAsynchJobProcessing(queueIdx);
        }
        else
        {
          /* Do nothing */
        }
      }
#  endif
    }
    /* #25 Request has been successfully passed to lower layer */
    else if (retVal == E_OK)
    {
      /* #26 Check if the job is still in state PROGRESSING or callback already occurred */
      if (Csm_GetJobState(jobId) == CSM_JOB_STATE_PROGRESSING)
      {
        Csm_SetJobState(jobId, CSM_JOB_STATE_WAITING); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
      }
      else
      {
        /* Callback already occurred but was not ready to process the application callback - do that now. */
        /* Lock queue to prevent a recursive call of Csm_ProcessJob from the application callback function */
        Csm_IncLockOfQueueState(queueIdx); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID */
        SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
        Csm_CallbackNotification(job, Csm_GetJobState(jobId) & CSM_JOB_STATE_CALLBACK_OCCURRED_CLEAR_MASK); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
        SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
        Csm_DecLockOfQueueState(queueIdx); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID */
      }
    }
    else
    {
      Csm_SetJobState(jobId, CSM_JOB_STATE_IDLE); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
#  if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
      job->jobId = CSM_EMPTY_JOB; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
#  endif
    }
    SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
  }
  else
  {
    /* The job is synchronous */
    /* #35 If any jobs are queued and job is not active, compare the job priority with the highest priority in the queue */
    if (
      (Csm_GetQueueIdxOfQueueState(queueIdx) != Csm_GetQueueStartIdxOfQueueInfo(queueIdx))
      && ((combinedJobState & CRYPTO_JOBSTATE_ACTIVE) != CRYPTO_JOBSTATE_ACTIVE)
      )
    {
      /* #37 Call Csm_HandleJobProcessing function if job priority is higher than the highest priority in the queue,
       *     otherwise return busy */
      if ((job->jobInfo->jobPriority > (Csm_GetQueue((Csm_QueueIdxOfQueueStateType)(Csm_GetQueueIdxOfQueueState(queueIdx) - 1u))->jobInfo->jobPriority)))
      {
        Csm_SetJobState(jobId, CSM_JOB_STATE_PROGRESSING); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
        Csm_IncLockOfQueueState(queueIdx); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID */
        SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
        retVal = Csm_HandleJobProcessing(channelId, job); /* SBSW_CSM_PASS_JOB */
        SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
        Csm_DecLockOfQueueState(queueIdx); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID */
      }
      else
      {
        retVal = CRYPTO_E_BUSY;
      }

      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
    else
# else
  {
# endif
    {
      /* #40 Process job if Queue is empty or job is active */
      Csm_SetJobState(jobId, CSM_JOB_STATE_PROGRESSING); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
      /* Call CryIf_ProcessJob() */
      retVal = Csm_HandleJobProcessing(channelId, job); /* SBSW_CSM_PASS_JOB */
    }

    /* #41 Free object if job finishes or is rejected, otherwise set to active */
    if ((retVal != E_OK) || ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))
    {
      Csm_SetJobState(jobId, CSM_JOB_STATE_IDLE); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
# if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
      job->jobId = CSM_EMPTY_JOB; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
# endif
    }
    else
    {
      Csm_SetJobState(jobId, CSM_JOB_STATE_ACTIVE); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
    }
  }

  return retVal;
} /* Csm_ProcessJob() */

# if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
/**********************************************************************************************************************
 *  Csm_PrepareJobObj()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_PrepareJobObj(uint32 jobId, P2VAR(Crypto_JobType, AUTOMATIC, AUTOMATIC) job)
{
  job->jobId = jobId; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
  job->jobPrimitiveInfo = Csm_GetAddrJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
  job->jobInfo = Csm_GetAddrJobInfo(Csm_GetJobInfoIdxOfJobTable(jobId)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
  job->jobState = CRYPTO_JOBSTATE_IDLE; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

#  if (CSM_JOB_TYPE_LAYOUT_ASR430_COMPATIBILITY == STD_ON)
  job->PrimitiveInputOutput = job->jobPrimitiveInputOutput; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
  job->state = job->jobState; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
#  endif
}
# endif

 /**********************************************************************************************************************
 *  Csm_GetJobObj()
 *********************************************************************************************************************/
 /*!
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
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
CSM_LOCAL FUNC(Std_ReturnType, CSM_CODE) Csm_GetJobObj(uint32 jobId, P2VAR(Csm_SizeOfJobType, AUTOMATIC, AUTOMATIC) jobObjId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = CRYPTO_E_BUSY;

  /* #10 Differ between job states */
  if (Csm_GetJobState(jobId) >= CSM_JOB_STATE_PROGRESSING)
  {
    /* #11 Job is currently in use, e.g. queued or in driver, reject request with busy */
    /* retval is already set. */
  }
  else if (Csm_GetJobState(jobId) == CSM_JOB_STATE_ACTIVE)
  {
    /* #12 Job is active but is waiting for further input. Job object was reserved already - return it.*/
    *jobObjId = Csm_GetJobToObjMap(jobId); /* SBSW_WRITE_TO_PASSED_LOCAL_STACK_VARIABLE */
    retVal = E_OK;
  }
# if (CSM_JOBIDXOFJOBTABLE == STD_ON)
  else
#  if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
  if (Csm_IsJobUsedOfJobTable(jobId))
#  endif
  {
    /* #20 Job has a dedicated job object, return it. */
    *jobObjId = Csm_GetJobIdxOfJobTable(jobId); /* SBSW_WRITE_TO_PASSED_LOCAL_STACK_VARIABLE */
    Csm_SetJobToObjMap(jobId, *jobObjId); /* SBSW_WRITE_TO_OBJECTMAP_BY_JOBID */
#  if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
    Csm_GetAddrJob(*jobObjId)->jobId = jobId; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
#  endif
    retVal = E_OK;
  }
# endif
# if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
  else
  {
    /* #21 Job Sharing: Find a free slot and return it, return busy if there is none */
    Csm_QueueInfoIterType queueIdx = Csm_GetQueueRefIdxOfJobTable(jobId);
    Csm_SizeOfJobType idx;
#  if (CSM_ASYNC_PROCESSING == STD_ON)
    /* If it is an asynchronous job fetch the next job object from the pool which is free for usage. */
    if (Csm_IsAsynchronousOfJobTable(jobId))
    {
      retVal = CRYPTO_E_QUEUE_FULL;

      for (idx = Csm_GetJobPoolStartIdxOfQueueInfo(queueIdx); idx < Csm_GetJobPoolEndIdxOfQueueInfo(queueIdx); idx++)
      {
        if (Csm_GetJob(idx).jobId == CSM_EMPTY_JOB)
        {
          *jobObjId = idx; /* SBSW_WRITE_TO_PASSED_LOCAL_STACK_VARIABLE */
          Csm_SetJobToObjMap(jobId, idx); /* SBSW_WRITE_TO_OBJECTMAP_BY_JOBID */
          Csm_PrepareJobObj(jobId, Csm_GetAddrJob(idx)); /* SBSW_CSM_PASS_JOB */
          retVal = E_OK;
          break;
        }
      }
    }
    else
#  endif
    {
      /* Fetch the one job object reserved for all synchronous jobs for this queue and check if it is free for usage. */
      idx = Csm_GetJobObjSynchronousIdxOfQueueInfo(queueIdx);
      if (Csm_GetJob(idx).jobId == CSM_EMPTY_JOB)
      {
        *jobObjId = idx; /* SBSW_WRITE_TO_PASSED_LOCAL_STACK_VARIABLE */
        Csm_SetJobToObjMap(jobId, idx); /* SBSW_WRITE_TO_OBJECTMAP_BY_JOBID */
        Csm_PrepareJobObj(jobId, Csm_GetAddrJob(idx)); /* SBSW_CSM_PASS_JOB */
        retVal = E_OK;
      }
    }
  }
# endif

  return retVal;
} /* Csm_GetJobObj() */

# if (CSM_ASYNC_PROCESSING == STD_ON)
/**********************************************************************************************************************
 *  Csm_EnqueueJob()
 *********************************************************************************************************************/
/*!
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
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
CSM_LOCAL_INLINE FUNC(Std_ReturnType, CSM_CODE) Csm_EnqueueJob(Csm_QueueInfoIterType queueIdx, P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job)
{
  Std_ReturnType retVal = E_OK;
  Csm_QueueIterType i, j;

  /* #5 Check if job that shall be enqueued is active and if queue has an empty place.*/
  if ((Csm_GetJobState(job->jobId) == CSM_JOB_STATE_ACTIVE) && (Csm_GetQueueIdxOfQueueState(queueIdx) < Csm_GetQueueEndIdxOfQueueInfo(queueIdx)))
  {
    Csm_SetQueue(Csm_GetQueueIdxOfQueueState(queueIdx), job); /* SBSW_CSM_WRITE_TO_QUEUE_BY_LOCALIDX */

    Csm_IncQueueIdxOfQueueState(queueIdx); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE */
  }
  /* #10 Check if queue has two empty places, so that there is always one empty place for an active job,
   *     otherwise return QUEUE_FULL.*/
  else if (Csm_GetQueueIdxOfQueueState(queueIdx) < (Csm_GetQueueEndIdxOfQueueInfo(queueIdx) - 1u))
  {
    for (i = Csm_GetQueueStartIdxOfQueueInfo(queueIdx); i <= Csm_GetQueueIdxOfQueueState(queueIdx); i++)
    {
      /* #11 If end of queue is reached, new job has the highest priority. Simply add new job at the end. */
      if (i == Csm_GetQueueIdxOfQueueState(queueIdx))
      {
        Csm_SetQueue(i, job); /* SBSW_CSM_WRITE_TO_QUEUE_BY_LOCALIDX */
      }
      /* #12 If job priority of job is lower or equal to the one in the queue,
       *     or if the one in the queue is active, take its place. */
      else if ((job->jobInfo->jobPriority <= Csm_GetQueue(i)->jobInfo->jobPriority) || (Csm_GetJobState(Csm_GetQueue(i)->jobId) == CSM_JOB_STATE_ACTIVE))
      {
        /* Place found in which job needs to be inserted */
        /* #13 Move all remaining jobs, which are already sorted, by one place to the right. */
        for (j = Csm_GetQueueIdxOfQueueState(queueIdx); j > i; j--)
        {
          Csm_SetQueue(j, Csm_GetQueue(j - 1u)); /* PRQA S 2841 */ /* MD_CSM_QUEUE_BY_QUEUEIDX */ /* SBSW_CSM_WRITE_TO_QUEUE_BY_LOCALIDX */
        }

        /* #14 Insert new job into the freed place. */
        Csm_SetQueue(i, job); /* SBSW_CSM_WRITE_TO_QUEUE_BY_LOCALIDX */
        break;
      }
      else
      {
        /* Priority of job to insert is higher, check next slot of queue. */
      }
    }

    Csm_SetJobState(job->jobId, CSM_JOB_STATE_QUEUED); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
    Csm_IncQueueIdxOfQueueState(queueIdx); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE */
  }
  else
  {
    retVal = CRYPTO_E_QUEUE_FULL;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Csm_DequeueJob()
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
CSM_LOCAL FUNC(void, CSM_CODE) Csm_DequeueJob(Csm_QueueInfoIterType queueIdx, uint32 jobId)
{
  Csm_QueueIterType i, j;

  /* #10 Iterate over enqueued jobs. */
  for (i = Csm_GetQueueIdxOfQueueState(queueIdx); i > Csm_GetQueueStartIdxOfQueueInfo(queueIdx); i--)
  {
    if (jobId == Csm_GetQueue(i - 1u)->jobId)
    {
      /* #20 Fill the possible gap by shifting remaining jobs to the left. */
      for (j = i; j < Csm_GetQueueIdxOfQueueState(queueIdx); j++)
      {
        Csm_SetQueue(j - 1u, Csm_GetQueue(j)); /* PRQA S 2841 */ /* MD_CSM_QUEUE_BY_QUEUEIDX */ /* SBSW_CSM_WRITE_TO_QUEUE_BY_LOCALIDX */
      }

      /* #21 Decrease queue size to finalize dequeuing. */
      Csm_DecQueueIdxOfQueueState(queueIdx); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID */

      break;
    }
  }
}

/**********************************************************************************************************************
 *  Csm_TriggerAsynchJobProcessing()
 *********************************************************************************************************************/
/*!
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
CSM_LOCAL FUNC(void, CSM_CODE) Csm_TriggerAsynchJobProcessing(Csm_QueueInfoIterType queueIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
  uint32 jobId;
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Check if queue has entries and processing is possible (queue not locked due to an active request): */
  if ((Csm_GetLockOfQueueState(queueIdx) == 0u) && (Csm_GetQueueIdxOfQueueState(queueIdx) != Csm_GetQueueStartIdxOfQueueInfo(queueIdx)))
  {
    job = Csm_GetQueue(Csm_GetQueueIdxOfQueueState(queueIdx) - 1u);
    jobId = job->jobId;

#  if (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON)
    Csm_SetRetriggeringOfQueueState(queueIdx, CSM_ASYNC_JOB_RETRIGGER_STATE_IDLE); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE */
#  endif

    if (jobId < Csm_GetSizeOfJobState())
    {
      /* #20 Lock queue temporarily and pass job to underlying crypto. */
      Csm_SetJobState(jobId, CSM_JOB_STATE_PROGRESSING); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
      Csm_IncLockOfQueueState(queueIdx); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE */
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
      retVal = Csm_HandleJobProcessing(Csm_GetChannelIdOfQueueInfo(queueIdx), job); /* SBSW_CSM_PASS_JOB */
      SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
      Csm_DecLockOfQueueState(queueIdx); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE */

      /* #31 If Job was successfully passed to lower layer remove it from queue. */
      if (retVal == E_OK)
      {
        Csm_DequeueJob(queueIdx, jobId);
        /* #32 Check if the job is still in state PROGRESSING or callback already occurred. */
        if (Csm_GetJobState(jobId) == CSM_JOB_STATE_PROGRESSING)
        {
          Csm_SetJobState(jobId, CSM_JOB_STATE_WAITING); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
        }
        else
        {
          /* Callback already occurred but was not ready to process the application callbacks - do that now */
          SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
          Csm_CallbackNotification(job, Csm_GetJobState(jobId) & CSM_JOB_STATE_CALLBACK_OCCURRED_CLEAR_MASK); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
          SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
        }
      }
      /* #33 If job cannot be processed because of busy driver or full queue. Let job in queue and try again later. */
      else if ((retVal == CRYPTO_E_BUSY) || (retVal == CRYPTO_E_QUEUE_FULL)) {
        Csm_SetJobState(jobId, CSM_JOB_STATE_QUEUED); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
      }
      else
      {
        /* #40 If lower layer returns an error remove job from queue and notify application. */
        Csm_DequeueJob(queueIdx, jobId);
        /* change to state waiting for correct handling in callback notification */
        Csm_SetJobState(jobId, CSM_JOB_STATE_WAITING); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
        SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
        Csm_CallbackNotification(job, retVal); /* SBSW_CSM_PASS_JOB_POINTER_LOCAL */
        SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
      }
    }
    else
    {
      /* job invalid - throw it away */
      Csm_DecQueueIdxOfQueueState(queueIdx); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE */
    }
  }
}

/**********************************************************************************************************************
 *  Csm_CancelAsynchronousJob()
 *********************************************************************************************************************/
/*!
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
 *
 *
 *
 */
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
CSM_LOCAL_INLINE FUNC(Std_ReturnType, CSM_CODE) Csm_CancelAsynchronousJob(P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job)
{
  uint32 jobId = job->jobId;
  Std_ReturnType retVal = E_NOT_OK;
  Csm_QueueInfoIterType queueInfoIdx = Csm_GetQueueRefIdxOfJobTable(jobId);

  boolean notifyByCallback = FALSE;

#  if (CSM_CANCELATION_DURING_PROCESSING == STD_ON)
  if (Csm_GetJobState(jobId) == CSM_JOB_STATE_WAITING)
  {
    /* #10 Object is in driver and could call Csm_Notification at any time. Even if lower layer returns,
     *     job can already have invoked the Csm_CallbackNotification or be on the way */

    Csm_SetJobState(jobId, CSM_JOB_STATE_CANCELING); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
    SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();

    /* #12 Cancel job in lower layer */
    retVal = CryIf_CancelJob(Csm_GetChannelIdOfQueueInfo(queueInfoIdx), job); /* SBSW_CSM_PASS_JOB */

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();

    if (Csm_GetJobState(jobId) == CSM_JOB_STATE_CANCELING)
    {
      if (retVal == E_OK)
      {
        notifyByCallback = TRUE;
      }
      else if (retVal == CRYPTO_E_JOB_CANCELED)
      {
        /* No immediate cancelation in lower layer possible, wait for next Csm_CallbackNotification */
        Csm_SetJobState(jobId, CSM_JOB_STATE_CANCELED); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
      }
      else
      {
        /* Canceling failed, restore state */
        Csm_SetJobState(jobId, CSM_JOB_STATE_WAITING); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
      }
    }
    else
    {
      /* Job State was modified in between so Csm_CallbackNotification already occurred - to late for cancel */
      retVal = E_NOT_OK;
    }
  }
  else
#  endif
    if (Csm_GetJobState(jobId) == CSM_JOB_STATE_QUEUED)
    {
      /* #21 If Job is not started yet, cancelation can be done in Csm only, otherwise cancelation in lower layer required. */
      if ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_START) == CRYPTO_OPERATIONMODE_START)
      {
        retVal = E_OK;
      }
      else
      {
        /* Job was already started and waits for input in driver - cancel it in lower layer too */
        /* #23 Pause processing when canceling in lower layer. Otherwise job could be passed to lower layer in between */
        Csm_IncLockOfQueueState(queueInfoIdx); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID */
        SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
        retVal = CryIf_CancelJob(Csm_GetChannelIdOfQueueInfo(queueInfoIdx), job); /* SBSW_CSM_PASS_JOB */
        SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
        Csm_DecLockOfQueueState(queueInfoIdx); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID */
      }

      if (retVal == E_OK)
      {
        /* #24 Remove job from queue in case of successful cancelation */
        Csm_DequeueJob(queueInfoIdx, jobId);
        notifyByCallback = TRUE;
      }
    }
    else if (Csm_GetJobState(jobId) == CSM_JOB_STATE_ACTIVE)
    {
      Csm_SetJobState(jobId, CSM_JOB_STATE_CANCELING); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();

      /* #32 Cancel job in lower layer */
      retVal = CryIf_CancelJob(Csm_GetChannelIdOfQueueInfo(queueInfoIdx), job); /* SBSW_CSM_PASS_JOB */

      SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
      if (retVal == E_OK)
      {
        /* Canceling successful - free job oject */
        notifyByCallback = TRUE;
      }
      else
      {
        /* #34 Abort cancelation and restore state if cancelation in lower layer fails */
        Csm_SetJobState(jobId, CSM_JOB_STATE_ACTIVE); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
        retVal = E_NOT_OK;
      }
    }
    else
    {
      /* Job is in mode which cannot be cancelled - retVal already set */
    }

  /* #40 If job was canceled, call configured callback notification function */
  if (notifyByCallback)
  {
    SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    Csm_CallbackNotification(job, CRYPTO_E_JOB_CANCELED); /* SBSW_CSM_FORWARDING_PTR */
    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
  }

  return retVal;
} /* Csm_CancelAsynchronousJob() */

/**********************************************************************************************************************
 *  Csm_HandleApplicationCallback()
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
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_HandleApplicationCallback(P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job, Std_ReturnType result, P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
  /* #10 Call configured user callback function */
#  if (CSM_CALLBACK_START_NOTIFICATION == STD_OFF)
  if ((((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)
          && (job->jobPrimitiveInfo->callbackUpdateNotification == TRUE))
      || ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)
      || (result == CRYPTO_E_JOB_CANCELED))
#  endif /* (CSM_CALLBACK_START_NOTIFICATION == STD_OFF) */
  {
#  if (CSM_CALLBACKFUNC43OFBSWCALLBACKS == STD_ON)
    if (Csm_IsBswCallbacksUsedOfJobTable(job->jobId) && (Csm_GetCallbackFunc43OfBswCallbacks(Csm_GetBswCallbacksIdxOfJobTable(job->jobId)) != NULL_PTR))
    {
      Csm_GetCallbackFunc43OfBswCallbacks(Csm_GetBswCallbacksIdxOfJobTable(job->jobId))(job, result); /* SBSW_CSM_FUNCTION_POINTER */
    }
    else
#  endif /* (CSM_CALLBACKFUNC43OFBSWCALLBACKS == STD_ON) */

#  if(CSM_CALLBACKFUNC44OFBSWCALLBACKS == STD_ON)
    if (Csm_IsBswCallbacksUsedOfJobTable(job->jobId) && (Csm_GetCallbackFunc44OfBswCallbacks(Csm_GetBswCallbacksIdxOfJobTable(job->jobId)) != NULL_PTR))
    {
      Csm_GetCallbackFunc44OfBswCallbacks(Csm_GetBswCallbacksIdxOfJobTable(job->jobId))(job->jobId, result); /* SBSW_CSM_FUNCTION_POINTER */
    }
    else
#  endif /* (CSM_CALLBACKFUNC44OFBSWCALLBACKS == STD_ON) */

#  if(CSM_CALLBACKFUNC45OFBSWCALLBACKS == STD_ON)
    /* Coming to this line Csm_GetCallbackFunc45OfBswCallbacks is always != NULL_PTR */
    if (Csm_IsBswCallbacksUsedOfJobTable(job->jobId))
    {
      Csm_GetCallbackFunc45OfBswCallbacks(Csm_GetBswCallbacksIdxOfJobTable(job->jobId))(job, result); /* SBSW_CSM_FUNCTION_POINTER */
    }
    else
#  endif /* (CSM_CALLBACKFUNC45OFBSWCALLBACKS == STD_ON) */

#  if (CSM_RTECALLBACK == STD_ON)
    if (Csm_IsRteCallbackUsedOfJobTable(job->jobId)) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    {
      Csm_SetRteResult(Csm_GetRteCallbackIdxOfJobTable(job->jobId), result); /* SBSW_CSM_WRITE_TO_RTERESULT_BY_JOBID */
      Csm_SetRteCallbackOccurred(TRUE); /* SBSW_CSM_CSL_VAR_ACCESS */
    }
    else
#  endif /* (CSM_RTECALLBACK == STD_ON) */

    {
      *errorId = CSM_E_PARAM_METHOD_INVALID; /* SBSW_WRITE_TO_PASSED_LOCAL_STACK_VARIABLE */
      /* Notification was called with an invalid job */
      CSM_DUMMY_STATEMENT(job); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
      CSM_DUMMY_STATEMENT(result); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
    }
  }
}

#  if (CSM_RTECALLBACK == STD_ON)
/**********************************************************************************************************************
 *  Csm_HandleRteCallbacks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_HandleRteCallbacks(void)
{
  /* Rte Notifications */
  if (Csm_IsRteCallbackOccurred())
  {
    Csm_RteCallbackIterType callbackIdx;
    Std_ReturnType retVal;

    Csm_SetRteCallbackOccurred(FALSE); /* SBSW_CSM_CSL_VAR_ACCESS */
    for (callbackIdx = 0u; callbackIdx < Csm_GetSizeOfRteCallback(); callbackIdx++)
    {
      retVal = Csm_GetRteResult(callbackIdx);
      if (retVal != CSM_CALLBACK_IDLE)
      {
        Csm_SetRteResult(callbackIdx, CSM_CALLBACK_IDLE); /* SBSW_CSM_WRITE_TO_RTERESULT_BY_SIZE */
        (void)Csm_GetRteCallback(callbackIdx)(retVal); /* SBSW_CSM_FUNCTION_POINTER */
      }
    }
  }
}
#  endif /* (CSM_RTECALLBACK == STD_ON) */

# endif /* (CSM_ASYNC_PROCESSING == STD_ON) */
#endif /* (CSM_JOB == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Csm_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, CSM_CODE) Csm_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
#if (CSM_DEV_ERROR_DETECT == STD_ON)
  Csm_IsInitialized = CSM_UNINIT;
#endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
} /* Csm_InitMemory() */

/**********************************************************************************************************************
 *  Csm_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6010 1 */ /* MD_MSR_STPTH */
FUNC(void, CSM_CODE) Csm_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state */
  if (Csm_IsInitialized == (uint8)CSM_INITIALIZED)
  {
    errorId = CSM_E_ALREADY_INITIALIZED;
  }
  else
#endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
#if (CSM_JOB == STD_ON)
    uint32_least index;
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Initialize job input/output buffer */
    for (index = 0u; index < Csm_GetSizeOfJob(); index++)
    {
      Csm_GetJob(index).jobState = CRYPTO_JOBSTATE_IDLE; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.inputPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.inputLength = 0u; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.secondaryInputPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.secondaryInputLength = 0u; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.tertiaryInputPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.tertiaryInputLength = 0u; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.outputPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.outputLengthPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.secondaryOutputPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.secondaryOutputLengthPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.verifyPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
# if (CSM_JOB_TYPE_LAYOUT_INPUT64 == STD_ON)
      Csm_GetJob(index).jobPrimitiveInputOutput.input64 = 0u; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
# endif
# if (CSM_JOB_TYPE_LAYOUT_OUTPUT64_PTR == STD_ON)
      Csm_GetJob(index).jobPrimitiveInputOutput.output64Ptr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
# endif
      Csm_GetJob(index).jobPrimitiveInputOutput.mode = 0u; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).cryptoKeyId = 0u; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobId = CSM_EMPTY_JOB; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
# if (CSM_JOB_TYPE_LAYOUT_ASR430_COMPATIBILITY == STD_ON)
      Csm_GetJob(index).PrimitiveInputOutput = Csm_GetJob(index).jobPrimitiveInputOutput; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).state = CRYPTO_JOBSTATE_IDLE; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
# endif
    }

    for (index = 0u; index < Csm_GetSizeOfJobTable(); index++)
    {
# if (CSM_JOBIDXOFJOBTABLE == STD_ON)
#  if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
      if (Csm_IsJobUsedOfJobTable(index))
#  endif
      {
        Csm_GetJob(Csm_GetJobIdxOfJobTable(index)).jobPrimitiveInfo = Csm_GetAddrJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(index)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
        Csm_GetJob(Csm_GetJobIdxOfJobTable(index)).jobInfo = Csm_GetAddrJobInfo(Csm_GetJobInfoIdxOfJobTable(index)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
        Csm_GetJob(Csm_GetJobIdxOfJobTable(index)).jobId = (uint32)index; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      }
# endif
      Csm_SetJobState(index, CSM_JOB_STATE_IDLE); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
    }

    /* #30 Init queues */
    for (index = 0u; index < Csm_GetSizeOfQueueInfo(); index++)
    {
      Csm_SetQueueIdxOfQueueState(index, Csm_GetQueueStartIdxOfQueueInfo(index)); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE */
      Csm_SetLockOfQueueState(index, 0u); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE */

# if ((CSM_ASYNC_PROCESSING == STD_ON) && (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON))
      if (Csm_IsTriggerAsynchJobsInCallbackOfQueueInfo(index)) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
      {
        Csm_SetRetriggeringOfQueueState(index, CSM_ASYNC_JOB_RETRIGGER_STATE_IDLE); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE */
      }
# endif
    }

# if (CSM_CALLOUT == STD_ON)
    /* Initialize Callout State */
    for (index = 0u; index < Csm_GetSizeOfCalloutState(); index++)
    {
      Csm_SetCalloutStateOfCalloutState(index, CSM_CALLOUT_STATE_IDLE); /* SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE */
    }
# endif /* (CSM_CALLOUT == STD_ON) */

# if (CSM_RTECALLBACK == STD_ON)
    Csm_SetRteCallbackOccurred(FALSE); /* SBSW_CSM_CSL_VAR_ACCESS */
    for (index = 0u; index < Csm_GetSizeOfRteCallback(); index++)
    {
      Csm_SetRteResult(index, CSM_CALLBACK_IDLE); /* SBSW_CSM_WRITE_TO_RTERESULT_BY_SIZE */
    }
# endif /* (CSM_RTECALLBACK == STD_ON) */

#endif /* (CSM_JOB == STD_ON) */

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    /* #40 Set CSM initialization state to initialized */
    Csm_IsInitialized = CSM_INITIALIZED;
#endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */

  }
  /* ----- Development Error Report --------------------------------------- */
#if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError(CSM_MODULE_ID, CSM_INSTANCE_ID, CSM_INIT_ID, errorId);
  }
#else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */
} /* Csm_Init() */

#if (CSM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Csm_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CSM_CODE) Csm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CSM_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  if (versioninfo == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  else
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID = (uint16)CSM_VENDOR_ID; /* SBSW_CSM_GETVERSIONINFO */
    versioninfo->moduleID = (uint8)CSM_MODULE_ID; /* SBSW_CSM_GETVERSIONINFO */
    versioninfo->sw_major_version = (uint8)CSM_SW_MAJOR_VERSION; /* SBSW_CSM_GETVERSIONINFO */
    versioninfo->sw_minor_version = (uint8)CSM_SW_MINOR_VERSION; /* SBSW_CSM_GETVERSIONINFO */
    versioninfo->sw_patch_version = (uint8)CSM_SW_PATCH_VERSION; /* SBSW_CSM_GETVERSIONINFO */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)CSM_GETVERSIONINFO_ID, errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */
}
#endif /* (CSM_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 *  Csm_MainFunction()
 *********************************************************************************************************************/
/*!
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CSM_CODE) Csm_MainFunction(void)
{
#if (CSM_JOB == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
# if (CSM_ASYNC_PROCESSING == STD_ON)

    Csm_QueueInfoIterType queueIdx;
#  if (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON)
    Csm_QueueIterType triggerCounter;
#  endif

    /* ----- Implementation ----------------------------------------------- */

    /* #20 Loop over configured queues and trigger asynchronous job processing */
    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    for (queueIdx = 0u; queueIdx < Csm_GetSizeOfQueueInfo(); queueIdx++)
    {
#  if (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON)
      if (Csm_IsTriggerAsynchJobsInCallbackOfQueueInfo(queueIdx)) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
      {
        /* #30 Check whether retriggering of asynchronous job processing has halted. If so, trigger it again. */
        /* The following difference is calculated from generated table values and assumed to be always greater or equal than 0 */
        for (triggerCounter = 0u; triggerCounter < ((Csm_QueueIterType)(Csm_GetQueueEndIdxOfQueueInfo(queueIdx)) - (Csm_QueueIterType)(Csm_GetQueueStartIdxOfQueueInfo(queueIdx))); triggerCounter++)
        {
          Csm_TriggerAsynchJobProcessing(queueIdx);

          if (Csm_GetRetriggeringOfQueueState(queueIdx) == CSM_ASYNC_JOB_RETRIGGER_STATE_IDLE)
          {
            break;
          }
        }
      }
      else
#  endif
      {
        Csm_TriggerAsynchJobProcessing(queueIdx);
      }
    }
    SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();

    /* #40 Handle callback notifications if processing has been aborted by callouts */
#  if (CSM_CALLOUT == STD_ON)
    {
      Csm_CalloutInfoIterType calloutInfoIdx;
      for (calloutInfoIdx = 0u; calloutInfoIdx < Csm_GetSizeOfCalloutInfo(); calloutInfoIdx++)
      {
        if (Csm_GetCalloutStateOfCalloutState(calloutInfoIdx) == CSM_CALLOUT_STATE_PROCESSING_ABORTED_BY_CALLOUT)
        {
          Csm_CallbackNotification(&Csm_GetJob(Csm_GetJobToObjMap(Csm_GetJobTableIdxOfCalloutInfo(calloutInfoIdx))), E_NOT_OK); /* SBSW_CSM_PASS_JOB_POINTER_CSL03 */
        }
      }
    }
#  endif

#  if (CSM_RTECALLBACK == STD_ON)
    Csm_HandleRteCallbacks();
#  endif

# endif /* (CSM_ASYNC_PROCESSING == STD_ON) */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_MAINFUNCTION_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */
#endif /* (CSM_JOB == STD_ON) */
} /* Csm_MainFunction() */

/**********************************************************************************************************************
 *  Csm_CallbackNotification()
 *********************************************************************************************************************/
/*!
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(void, CSM_CODE) Csm_CallbackNotification(P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job, /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                              Std_ReturnType result)
{
#if (CSM_JOB == STD_ON) && (CSM_ASYNC_PROCESSING == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CSM_E_NO_ERROR;
  Std_ReturnType resultCopy = result;
  boolean notify = TRUE;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (job == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  else if ((job->jobId >= Csm_GetSizeOfJobTable()) || ((resultCopy & CSM_JOB_STATE_CALLBACK_OCCURRED) != 0u))
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Keep jobId local to still have it after if it is cleared in case of a shared job object */
    uint32 jobId = job->jobId;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
# if (CSM_CANCELATION_DURING_PROCESSING == STD_ON)
    /* #21 Check if job was cancelled in between and update return value accordingly */
    if (Csm_GetJobState(jobId) == CSM_JOB_STATE_CANCELED)
    {
      resultCopy = CRYPTO_E_JOB_CANCELED;
      Csm_SetJobState(jobId, CSM_JOB_STATE_IDLE); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
    }
    else
# endif
    /* #22 If not yet in state waiting but in state progressing due to preemption,
     *     save result and call callback in initial call. */
    if (Csm_GetJobState(jobId) == CSM_JOB_STATE_PROGRESSING)
    {
      Csm_SetJobState(jobId, CSM_JOB_STATE_CALLBACK_OCCURRED | resultCopy); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
      notify = FALSE;
    }
    /* #23 Update state to accept new request directly triggered in callback of application.
           Set new state to idle or active depending on whether operation mode cotains operation mode finish. */
    else if ((resultCopy == E_OK) && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) != CRYPTO_OPERATIONMODE_FINISH))
    {
      Csm_SetJobState(jobId, CSM_JOB_STATE_ACTIVE); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
    }
    else
    {
      Csm_SetJobState(jobId, CSM_JOB_STATE_IDLE); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
    }

    SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();

    /* #24 Check wether user is already to be notified and shared object is to be freed */
    if (notify)
    {
# if (CSM_CALLOUT == STD_ON)
      Csm_CallPostJobCallout(job, &resultCopy); /* SBSW_CSM_PASS_JOB_AND_STACK */
# endif /* (CSM_CALLOUT == STD_ON) */

      /* #30 Call user callback function */
      Csm_HandleApplicationCallback(job, resultCopy, &errorId); /* SBSW_CSM_PASS_JOB_POINTER_API_REQUIREMENT_AND_STACK */

      /* #31 Free object if job finishes or is rejected if job has no dedicated object */
# if (CSM_JOBIDXOFJOBTABLE == STD_ON) && (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
      if (!(Csm_IsJobUsedOfJobTable(jobId)))
# endif
      {
# if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
        if ((resultCopy != E_OK) || ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))
        {
          job->jobId = CSM_EMPTY_JOB; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
        }
# endif
      }

# if (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON)
      {
        /* #40 Trigger new asynchronous job. Derive the retrigger state depending on whether getting queue lock succeeded
          *     or failed. */
        Csm_QueueInfoIterType queueIdx;
        queueIdx = Csm_GetQueueRefIdxOfJobTable(jobId);

        if (Csm_IsTriggerAsynchJobsInCallbackOfQueueInfo(queueIdx)) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
        {
          SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
          Csm_SetRetriggeringOfQueueState(queueIdx, CSM_ASYNC_JOB_RETRIGGER_STATE_PENDING); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE */
          Csm_TriggerAsynchJobProcessing(queueIdx); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE */
          SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
        }
      }
# endif /* (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON) */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_CALLBACKNOTIFICATION_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(job); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */
#else
  CSM_DUMMY_STATEMENT(job); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(result); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif /* (CSM_JOB == STD_ON) && (CSM_ASYNC_PROCESSING == STD_ON) */
}

/* Csm Key Management Functions */
/**********************************************************************************************************************
 *  Csm_KeyElementSet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementSet(uint32 keyId,
                                                 uint32 keyElementId,
                                                 P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) keyPtr,
                                                 uint32 keyLength)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (keyPtr == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_KeyElementSet function */
    retVal = CryIf_KeyElementSet(Csm_GetCryIfKeyIdOfKey(keyId), keyElementId, keyPtr, keyLength); /* SBSW_CSM_FORWARDING_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYELEMENTSET_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyElementId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT_CONST(keyPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_KeyElementSet() */

/**********************************************************************************************************************
 *  Csm_KeySetValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeySetValid(uint32 keyId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeySetValid function */
    retVal = CryIf_KeySetValid(Csm_GetCryIfKeyIdOfKey(keyId));
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYSETVALID_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_KeySetValid() */

/**********************************************************************************************************************
 *  Csm_KeyElementGet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementGet(uint32 keyId,
                                                 uint32 keyElementId,
                                                 P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) keyPtr,        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                                 P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) keyLengthPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if ((keyPtr == NULL_PTR)
         || (keyLengthPtr == NULL_PTR))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_KeyElementGet function */
    retVal = CryIf_KeyElementGet(Csm_GetCryIfKeyIdOfKey(keyId), keyElementId, keyPtr, keyLengthPtr); /* SBSW_CSM_FORWARDING_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYELEMENTGET_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyElementId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_KeyElementGet() */

/**********************************************************************************************************************
 *  Csm_KeyElementCopy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementCopy(uint32 keyId,
                                                  uint32 keyElementId,
                                                  uint32 targetKeyId,
                                                  uint32 targetKeyElementId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check key range */
  else if ((keyId >= Csm_GetSizeOfKey()) || (targetKeyId >= Csm_GetSizeOfKey()))
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeyElementCopy function */
    retVal = CryIf_KeyElementCopy(Csm_GetCryIfKeyIdOfKey(keyId), keyElementId, Csm_GetCryIfKeyIdOfKey(targetKeyId), targetKeyElementId);
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYELEMENTCOPY_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyElementId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(targetKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(targetKeyElementId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_KeyElementCopy() */

/**********************************************************************************************************************
 *  Csm_KeyElementCopyPartial()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementCopyPartial( uint32 keyId,
                                                          uint32 keyElementId,
                                                          uint32 keyElementSourceOffset,
                                                          uint32 keyElementTargetOffset,
                                                          uint32 keyElementCopyLength,
                                                          uint32 targetKeyId,
                                                          uint32 targetKeyElementId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check key range */
  else if ((keyId >= Csm_GetSizeOfKey()) || (targetKeyId >= Csm_GetSizeOfKey()))
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeyElementCopyPartial function */
    retVal = CryIf_KeyElementCopyPartial(Csm_GetCryIfKeyIdOfKey(keyId), keyElementId, keyElementSourceOffset, keyElementTargetOffset, keyElementCopyLength, Csm_GetCryIfKeyIdOfKey(targetKeyId), targetKeyElementId);
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYELEMENTCOPYPARTIAL_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyElementId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyElementSourceOffset); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyElementTargetOffset); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyElementCopyLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(targetKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(targetKeyElementId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
}

/**********************************************************************************************************************
 *  Csm_KeyCopy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyCopy(uint32 keyId,
                                           uint32 targetKeyId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check key range */
  else if ((keyId >= Csm_GetSizeOfKey()) || (targetKeyId >= Csm_GetSizeOfKey()))
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeyCopy function */
    retVal = CryIf_KeyCopy(Csm_GetCryIfKeyIdOfKey(keyId), Csm_GetCryIfKeyIdOfKey(targetKeyId));
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYCOPY_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(targetKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_KeyCopy() */

/**********************************************************************************************************************
 *  Csm_RandomSeed()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_RandomSeed(uint32 keyId,
                                              P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) seedPtr,
                                              uint32 seedLength)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (seedPtr == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_RandomSeed function */
    retVal = CryIf_RandomSeed(Csm_GetCryIfKeyIdOfKey(keyId), seedPtr, seedLength); /* SBSW_CSM_FORWARDING_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_RANDOMSEED_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT_CONST(seedPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(seedLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_RandomSeed() */

/**********************************************************************************************************************
 *  Csm_KeyGenerate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyGenerate(uint32 keyId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeyGenerate function */
    retVal = CryIf_KeyGenerate(Csm_GetCryIfKeyIdOfKey(keyId));
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYGENERATE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_KeyGenerate() */

/**********************************************************************************************************************
 *  Csm_KeyDerive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyDerive(uint32 keyId,
                                             uint32 targetKeyId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check key range */
  else if ((keyId >= Csm_GetSizeOfKey()) || (targetKeyId >= Csm_GetSizeOfKey()))
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeyDerive function */
    retVal = CryIf_KeyDerive(Csm_GetCryIfKeyIdOfKey(keyId), Csm_GetCryIfKeyIdOfKey(targetKeyId));
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYDERIVE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(targetKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_KeyDerive() */

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcPubVal()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcPubVal(uint32 keyId,
                                                         P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                                         P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if ((publicValuePtr == NULL_PTR)
         || (publicValueLengthPtr == NULL_PTR))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_KeyExchangeCalcPubVal function */
    retVal = CryIf_KeyExchangeCalcPubVal(Csm_GetCryIfKeyIdOfKey(keyId), publicValuePtr, publicValueLengthPtr); /* SBSW_CSM_FORWARDING_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYEXCHANGECALCPUBVAL_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(publicValuePtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(publicValueLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_KeyExchangeCalcPubVal() */

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcSecret()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcSecret(uint32 keyId,
                                                         P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
                                                         uint32 partnerPublicValueLength)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (partnerPublicValuePtr == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_KeyExchangeCalcSecret function */
    retVal = CryIf_KeyExchangeCalcSecret(Csm_GetCryIfKeyIdOfKey(keyId), partnerPublicValuePtr, partnerPublicValueLength); /* SBSW_CSM_FORWARDING_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYEXCHANGECALCSECRET_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(partnerPublicValuePtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(partnerPublicValueLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_KeyExchangeCalcSecret() */

/**********************************************************************************************************************
 *  Csm_CertificateParse()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateParse(uint32 keyId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_CertificateParse function */
    retVal = CryIf_CertificateParse(Csm_GetCryIfKeyIdOfKey(keyId));
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_CERTIFICATEPARSE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_CertificateParse() */

/**********************************************************************************************************************
 *  Csm_CertificateVerify()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateVerify(uint32 keyId,
                                                     uint32 verifyKeyId,
                                                     P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (verifyPtr == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if ((keyId >= Csm_GetSizeOfKey()) || (verifyKeyId >= Csm_GetSizeOfKey()))
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_CertificateVerify function */
    retVal = CryIf_CertificateVerify(Csm_GetCryIfKeyIdOfKey(keyId), Csm_GetCryIfKeyIdOfKey(verifyKeyId), verifyPtr); /* SBSW_CSM_FORWARDING_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_CERTIFICATEVERIFY_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(verifyKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(verifyPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_CertificateVerify() */

/**********************************************************************************************************************
 *  Csm_CancelJob()
 *********************************************************************************************************************/
/*!
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
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_CancelJob(uint32 jobId,
                                             Crypto_OperationModeType mode)
{
#if (CSM_JOB == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check job range */
  else if (jobId >= Csm_GetSizeOfJobTable())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();

    if (Csm_GetJobState(jobId) == CSM_JOB_STATE_IDLE)
    {
      /* #30 If Job is idle  - No canceling needed */
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
      retVal =  E_OK;
    }
    else
    {
      /* Job is running, object exists */
      P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job = Csm_GetAddrJob(Csm_GetJobToObjMap(jobId));

# if (CSM_ASYNC_PROCESSING == STD_ON)
      if (Csm_IsAsynchronousOfJobTable(jobId))
      {
        retVal = Csm_CancelAsynchronousJob(job); /* SBSW_CSM_PASS_JOB */
        SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
      }
      else
# endif
      if (Csm_GetJobState(jobId) == CSM_JOB_STATE_ACTIVE)
      {
        Csm_SetJobState(jobId, CSM_JOB_STATE_CANCELING); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
        SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();

        /* #55 Cancel Job in lower layer */
        retVal = CryIf_CancelJob(Csm_GetChannelIdOfQueueInfo(Csm_GetQueueRefIdxOfJobTable(jobId)), job); /* SBSW_CSM_PASS_JOB */

        if (retVal == E_OK)
        {
          /* #56 Canceling successful - Call Postcallout and free job oject */
# if (CSM_CALLOUT == STD_ON)
          /* retVal only acts as information for post job callot that job was cancelled.
           * Set it back to E_OK in each case as canceling was already performed. */
          retVal = CRYPTO_E_JOB_CANCELED;
          Csm_CallPostJobCallout(job, &retVal); /* SBSW_CSM_PASS_JOB */
          retVal = E_OK;
# endif /* (CSM_CALLOUT == STD_ON) */
# if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
          job->jobId = CSM_EMPTY_JOB; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
# endif
          Csm_SetJobState(jobId, CSM_JOB_STATE_IDLE); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
        }
        else
        {
          /* #57 Canceling failed, set active again */
          Csm_SetJobState(jobId, CSM_JOB_STATE_ACTIVE); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID */
        }
      }
      else
      {
        /* #60 Job cancelation is not possible as the job is currently in or on the way to the lower layer.
         *     If job is currently on the way to the lower layer, Crypto would return E_OK as it does not know the job yet
         *     Due to this, Csm cannot rely on the return value.
         */
        SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
        /* retVal already set to E_NOT_OK */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_CANCELJOB_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return retVal;
#else

  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_JOB == STD_ON) */
} /* Csm_CancelJob() */

/* Csm Service Functions */

/**********************************************************************************************************************
 *  Csm_Hash()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_Hash(uint32 jobId,
                                        Crypto_OperationModeType mode,
                                        P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                        uint32 dataLength,
                                        P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                        P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_HASH == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check job range */
  else if (jobId >= Csm_GetSizeOfJobTable())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service != CRYPTO_HASH)
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputPtr = resultPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_HASH_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif
} /* Csm_Hash() */

/**********************************************************************************************************************
 *  Csm_MacGenerate()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerate(uint32 jobId,
                                               Crypto_OperationModeType mode,
                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                               uint32 dataLength,
                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_MACGENERATE == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check job range */
  else if (jobId >= Csm_GetSizeOfJobTable())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service != CRYPTO_MACGENERATE)
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputPtr = macPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputLengthPtr = macLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_MACGENERATE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(macPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(macLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_MACGENERATE == STD_ON) */
} /* Csm_MacGenerate() */

/**********************************************************************************************************************
 *  Csm_MacVerify()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerify(uint32 jobId,
                                             Crypto_OperationModeType mode,
                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                             uint32 dataLength,
                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                             uint32 macLength,
                                             P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_MACVERIFY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check job range */
  else if (jobId >= Csm_GetSizeOfJobTable())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service != CRYPTO_MACVERIFY)
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.secondaryInputPtr = macPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.secondaryInputLength = macLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.verifyPtr = verifyPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_MACVERIFY_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(macPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(macLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(verifyPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_MACVERIFY == STD_ON) */
} /* Csm_MacVerify() */

/**********************************************************************************************************************
 *  Csm_Encrypt()
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
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_Encrypt(uint32 jobId,
                                           Crypto_OperationModeType mode,
                                           P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                           uint32 dataLength,
                                           P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                           P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_ENCRYPT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check job range */
  else if (jobId >= Csm_GetSizeOfJobTable())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service != CRYPTO_ENCRYPT)
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputPtr = resultPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_ENCRYPT_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_ENCRYPT == STD_ON) */
} /* Csm_Encrypt() */

/**********************************************************************************************************************
 *  Csm_Decrypt()
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
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_Decrypt(uint32 jobId,
                                           Crypto_OperationModeType mode,
                                           P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                           uint32 dataLength,
                                           P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                           P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_DECRYPT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check job range */
  else if (jobId >= Csm_GetSizeOfJobTable())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service != CRYPTO_DECRYPT)
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputPtr = resultPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_DECRYPT_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_DECRYPT == STD_ON) */
} /* Csm_Decrypt() */

/**********************************************************************************************************************
 *  Csm_AEADEncrypt()
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
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncrypt(uint32 jobId,
                                               Crypto_OperationModeType mode,
                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                               uint32 plaintextLength,
                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                               uint32 associatedDataLength,
                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,         /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,  /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,                /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr)         /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_AEADENCRYPT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check job range */
  else if (jobId >= Csm_GetSizeOfJobTable())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service != CRYPTO_AEADENCRYPT)
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputPtr = plaintextPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputLength = plaintextLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.secondaryInputPtr = associatedDataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.secondaryInputLength = associatedDataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputPtr = ciphertextPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputLengthPtr = ciphertextLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.secondaryOutputPtr = tagPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.secondaryOutputLengthPtr = tagLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_AEADENCRYPT_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(plaintextPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(plaintextLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(associatedDataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(associatedDataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(ciphertextPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(ciphertextLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(tagPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(tagLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_AEADENCRYPT == STD_ON) */
} /* Csm_AEADEncrypt() */

/**********************************************************************************************************************
 *  Csm_AEADDecrypt()
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
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecrypt(uint32 jobId,
                                               Crypto_OperationModeType mode,
                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                               uint32 ciphertextLength,
                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                               uint32 associatedDataLength,
                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                               uint32 tagLength,
                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,                /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) plaintextLengthPtr,         /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                               P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_AEADDECRYPT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check job range */
  else if (jobId >= Csm_GetSizeOfJobTable())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service != CRYPTO_AEADDECRYPT)
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputPtr = ciphertextPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputLength = ciphertextLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.secondaryInputPtr = associatedDataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.secondaryInputLength = associatedDataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.tertiaryInputPtr = tagPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.tertiaryInputLength = tagLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputPtr = plaintextPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputLengthPtr = plaintextLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.verifyPtr = verifyPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_AEADDECRYPT_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(ciphertextPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(ciphertextLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(associatedDataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(associatedDataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(tagPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(tagLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(plaintextPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(plaintextLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(verifyPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_AEADDECRYPT == STD_ON) */
} /* Csm_AEADDecrypt() */

/**********************************************************************************************************************
 *  Csm_SignatureGenerate()
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
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerate(uint32 jobId,
                                                     Crypto_OperationModeType mode,
                                                     P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                     uint32 dataLength,
                                                     P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                                     P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_SIGNATUREGENERATE == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check job range */
  else if (jobId >= Csm_GetSizeOfJobTable())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service != CRYPTO_SIGNATUREGENERATE)
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputPtr = resultPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_SIGNATUREGENERATE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_SIGNATUREGENERATE == STD_ON) */
} /* Csm_SignatureGenerate() */

/**********************************************************************************************************************
 *  Csm_SignatureVerify()
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
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerify(uint32 jobId,
                                                   Crypto_OperationModeType mode,
                                                   P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                   uint32 dataLength,
                                                   P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                   uint32 signatureLength,
                                                   P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_SIGNATUREVERIFY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check job range */
  else if (jobId >= Csm_GetSizeOfJobTable())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service != CRYPTO_SIGNATUREVERIFY)
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.secondaryInputPtr = signaturePtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.secondaryInputLength = signatureLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.verifyPtr = verifyPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_SIGNATUREVERIFY_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(signaturePtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(signatureLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(verifyPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_SIGNATUREVERIFY == STD_ON) */
} /* Csm_SignatureVerify() */

/**********************************************************************************************************************
 *  Csm_RandomGenerate()
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
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_RandomGenerate(uint32 jobId,
                                                  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                                  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_RANDOMGENERATE == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check job range */
  else if (jobId >= Csm_GetSizeOfJobTable())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service != CRYPTO_RANDOMGENERATE)
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SINGLECALL; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputPtr = resultPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_RANDOMGENERATE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_RANDOMGENERATE == STD_ON) */
} /* Csm_RandomGenerate() */

/**********************************************************************************************************************
 *  Csm_JobKeySetValid()
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
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobKeySetValid( uint32 jobId, uint32 keyId )
{
#if (CSM_EXIST_JOBKEYSETVALID == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check key and job range */
  else if ( ( keyId >= Csm_GetSizeOfKey() ) || ( jobId >= Csm_GetSizeOfJobTable() ) )
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service != CRYPTO_KEYSETVALID)
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SINGLECALL; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.cryIfKeyId = Csm_GetCryIfKeyIdOfKey( keyId ); /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError( (uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_JOBKEYSETVALID_ID), errorId );
  }
# else
  CSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT( jobId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( keyId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_JOBKEYSETVALID == STD_ON) */
} /* Csm_JobKeySetValid() */

/**********************************************************************************************************************
 *  Csm_JobRandomSeed()
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
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobRandomSeed( uint32 jobId, uint32 keyId, P2CONST( uint8, AUTOMATIC, CSM_APPL_VAR ) seedPtr, uint32 seedLength )
{
#if (CSM_EXIST_JOBRANDOMSEED == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check key and job range */
  else if ( ( keyId >= Csm_GetSizeOfKey() ) || ( jobId >= Csm_GetSizeOfJobTable() ) )
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service != CRYPTO_RANDOMSEED)
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SINGLECALL; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.cryIfKeyId = Csm_GetCryIfKeyIdOfKey( keyId ); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputPtr = seedPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputLength = seedLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError( (uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_JOBRANDOMSEED_ID), errorId );
  }
# else
  CSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT_CONST( jobId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( keyId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT_CONST( seedPtr ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( seedLength ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_JOBRANDOMSEED == STD_ON) */
} /* Csm_JobRandomSeed() */

/**********************************************************************************************************************
 *  Csm_JobKeyGenerate()
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
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobKeyGenerate( uint32 jobId, uint32 keyId )
{
#if (CSM_EXIST_JOBKEYGENERATE == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check key and job range */
  else if ((keyId >= Csm_GetSizeOfKey()) || (jobId >= Csm_GetSizeOfJobTable()))
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service != CRYPTO_KEYGENERATE)
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SINGLECALL; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.cryIfKeyId = Csm_GetCryIfKeyIdOfKey( keyId ); /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError( (uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_JOBKEYGENERATE_ID), errorId );
  }
# else
  CSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT( jobId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( keyId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_JOBKEYGENERATE == STD_ON) */
} /* Csm_JobKeyGenerate() */

/**********************************************************************************************************************
 *  Csm_JobKeyDerive()
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
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobKeyDerive( uint32 jobId, uint32 keyId, uint32 targetKeyId )
{
#if (CSM_EXIST_JOBKEYDERIVE == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check key, targetKey and job range */
  else if ( (keyId >= Csm_GetSizeOfKey()) || (targetKeyId >= Csm_GetSizeOfKey()) || (jobId >= Csm_GetSizeOfJobTable()) )
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service != CRYPTO_KEYDERIVE)
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SINGLECALL; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.cryIfKeyId = Csm_GetCryIfKeyIdOfKey( keyId ); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.targetCryIfKeyId = Csm_GetCryIfKeyIdOfKey( targetKeyId ); /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError( (uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_JOBKEYDERIVE_ID), errorId );
  }
# else
  CSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT( jobId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( keyId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( targetKeyId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_JOBKEYDERIVE == STD_ON) */
} /* Csm_JobKeyDerive() */

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcPubVal()
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
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobKeyExchangeCalcPubVal( uint32 jobId,
                                                               uint32 keyId,
                                                               P2VAR( uint8, AUTOMATIC, CSM_APPL_VAR ) publicValuePtr,         /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                                               P2VAR( uint32, AUTOMATIC, CSM_APPL_VAR ) publicValueLengthPtr ) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_JOBKEYEXCHANGECALCPUBVAL == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check key and job range */
  else if ((keyId >= Csm_GetSizeOfKey()) || (jobId >= Csm_GetSizeOfJobTable()))
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service != CRYPTO_KEYEXCHANGECALCPUBVAL)
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SINGLECALL; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.cryIfKeyId = Csm_GetCryIfKeyIdOfKey( keyId ); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputPtr = publicValuePtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputLengthPtr = publicValueLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError( (uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_JOBKEYEXCHANGECALCPUBVAL_ID), errorId );
  }
# else
  CSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT( jobId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( keyId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( publicValuePtr ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( publicValueLengthPtr ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_JOBKEYEXCHANGECALCPUBVAL == STD_ON) */
} /* Csm_JobKeyExchangeCalcPubVal() */

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcSecret()
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
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobKeyExchangeCalcSecret(  uint32 jobId,
                                                                uint32 keyId,
                                                                P2CONST( uint8, AUTOMATIC, CSM_APPL_VAR ) partnerPublicValuePtr,
                                                                uint32 partnerPublicValueLength )
{
#if (CSM_EXIST_JOBKEYEXCHANGECALCSECRET == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check key and job range */
  else if ((keyId >= Csm_GetSizeOfKey()) || (jobId >= Csm_GetSizeOfJobTable()))
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service != CRYPTO_KEYEXCHANGECALCSECRET)
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SINGLECALL; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.cryIfKeyId = Csm_GetCryIfKeyIdOfKey( keyId ); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputPtr = partnerPublicValuePtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputLength = partnerPublicValueLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError( (uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_JOBKEYEXCHANGECALCSECRET_ID), errorId );
  }
# else
  CSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT( jobId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( keyId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( partnerPublicValuePtr ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( partnerPublicValueLength ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_JOBKEYEXCHANGECALCSECRET == STD_ON) */
} /* Csm_JobKeyExchangeCalcSecret() */

/**********************************************************************************************************************
 *  Csm_SaveContextJob()
 *********************************************************************************************************************/
 /*!
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_SaveContextJob(  uint32 jobId,
                                                    P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) contextBufferPtr, /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                                    P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) contextBufferLengthPtr ) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_JOB == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check job range */
  else if (jobId >= Csm_GetSizeOfJobTable())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check for Null Pointer */
  else if ((contextBufferPtr == NULL_PTR) || (contextBufferLengthPtr == NULL_PTR))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #13 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service > CRYPTO_SIGNATUREVERIFY)
  {
    errorId = CSM_E_SERVICE_TYPE;
  }
  else
# endif
  /* #14 Check if the job state is active */
  if (Csm_GetJobState(jobId) == CSM_JOB_STATE_IDLE)
  {
    /* retVal is already set to E_NOT_OK */
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SAVE_CONTEXT; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputPtr = contextBufferPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.outputLengthPtr = contextBufferLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_SAVECONTEXTJOB_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(contextBufferPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(contextBufferLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_JOB == STD_ON) */
} /* Csm_SaveContextJob() */

/**********************************************************************************************************************
 *  Csm_RestoreContextJob()
 *********************************************************************************************************************/
 /*!
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_RestoreContextJob(  uint32 jobId,
                                                       P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) contextBufferPtr, /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                                       uint32 contextBufferLength )
{
#if (CSM_JOB == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #11 Check job range */
  else if (jobId >= Csm_GetSizeOfJobTable())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  /* #12 Check for Null Pointer */
  else if (contextBufferPtr == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #13 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service > CRYPTO_SIGNATUREVERIFY)
  {
    errorId = CSM_E_SERVICE_TYPE;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_RESTORE_CONTEXT; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputPtr = contextBufferPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobPrimitiveInputOutput.inputLength = contextBufferLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_RESTORECONTEXTJOB_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(contextBufferPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(contextBufferLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_JOB == STD_ON) */
} /* Csm_RestoreContextJob() */

#define CSM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Module specific MISRA deviations
 *********************************************************************************************************************/
/* Module specific MISRA deviations:
  MD_CSM_QUEUE_BY_QUEUEIDX:
    Reason:     If Queue variable has only a size of 1, accessing with any index + 1 causes this message.
    Risk:       Access could be executed causing an invalid memory access.
    Prevention: During runtime, loop is only entered if Queue is at least two since index is checked agains localQueueIndex. Covered by code review.

  MD_CSM_0316:
    Reason:     Cast is needed since upper layer Rte expects super type as parameter.
    Risk:       Wrong type is casted or size does not match.
    Prevention: Covered by code review.

  MD_CSM_CALLBACK_DECLARATIONS:
    Description:  Rule 8.5: An external object or function shall be declared once in one and only one file
    Reason:       MICROSAR Csm declares its callbacks and pre and post job callouts in its Cfg.h. Modules that also declares them see a double declaration.
    Risk:         Compile warning or error due to declaration mismatch.
    Prevention:   Not necessary
 */

/* SBSW_JUSTIFICATION_BEGIN

  \ID SBSW_CSM_GETVERSIONINFO
    \DESCRIPTION The function Csm_GetVersionInfo writes to the object referenced by parameter VersionInfoPtr
    \COUNTERMEASURE \N The caller ensures that the pointer passed to the parameter VersionInfoPtr is valid.

  \ID SBSW_CSM_PASS_JOB
    \DESCRIPTION    A job pointer is passed to the function. This pointer was originally determined by the given and
                    checked job id. This is done by accessing the job structure from the global job array via
                    indirection over JobTable or using one of the shared job objects. The shared job objects are
                    determined by accessing the QueueInfo via indirection over the JobTable. Having the QueueInfo index,
                    accesses to the global job strucure is done via indirection of QueueInfo. Therefore, pointer always
                    points to a valid job structure ready to also pass to asynchronous functions.
    \COUNTERMEASURE \N The job pointer is set using the passed Id and ComStackLib macros (CSL03).

  \ID SBSW_CSM_FUNCTION_POINTER
    \DESCRIPTION    The API call is made using a function pointer which is read using ComStackLib macros.
                    Pointer might be forwarded.
    \COUNTERMEASURE \N The function pointer is read using the passed Id and ComStackLib macros (CSL03).
                       The pointer are forwarded and assumed to be valid.

  \ID SBSW_CSM_FORWARDING_PTR
    \DESCRIPTION    Passing received pointer to called function.
    \COUNTERMEASURE \N The pointer is forwarded and assumed to be valid.

  \ID SBSW_CSM_PASS_JOB_AND_STACK
    \DESCRIPTION    The function passes a checked job pointer (see [SBSW_CSM_PASS_JOB]) and a pointer to a stack
                    variable of expected size to an synchronous function.
    \COUNTERMEASURE \N The function pointer is read using the passed Id and ComStackLib macros (CSL03).
                       [CM_CSM_STACK_VARIABLE]

  \ID SBSW_CSM_PASS_CHECKED_JOBID_AND_STACK_VARIABLE
    \DESCRIPTION    The function passes a checked job id and a pointer to a stack variable of expected size to an synchronous function.
    \COUNTERMEASURE \R The job id was checked if it is in range.
                       [CM_CSM_STACK_VARIABLE]

  \ID SBSW_CSM_PASS_JOB_POINTER_LOCAL
    \DESCRIPTION    The function passes a pointer referencing a job. The passed pointer has been initialized and points to a valid job object.
    \COUNTERMEASURE \N The pointer is initialized from a local queue. The queue is only filled with valid job objects and therefore always contains valid jobs except when it is empty. In this case, the function is not called.

  \ID SBSW_CSM_PASS_JOB_POINTER_API_REQUIREMENT
    \DESCRIPTION    The function passes a pointer referencing a job. The passed pointer has been passed by the caller. The API requires that the referenced job is valid.
    \COUNTERMEASURE \N The caller ensures that the pointer passed as parameter is valid and references a valid object.

  \ID SBSW_CSM_PASS_JOB_POINTER_API_REQUIREMENT_AND_STACK
    \DESCRIPTION    The function passes a pointer referencing a job. The passed pointer has been passed by the caller. The API requires that the referenced job is valid.
                    The function also passes a pointer to a stack variable of expected size to an synchronous function.
    \COUNTERMEASURE \N The caller ensures that the pointer passed as parameter is valid and references a valid object.

  \ID SBSW_CSM_PASS_JOB_POINTER_CSL03
    \DESCRIPTION    The function passes a pointer referencing a job. The passed pointer is valid since it is an indirection to Job via CalloutInfo.
    \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

  \ID SBSW_CSM_WRITE_TO_JOB_POINTER
    \DESCRIPTION    The function does a write access on the job pointer. See [SBSW_CSM_PASS_JOB] for checking validity of job pointer.
    \COUNTERMEASURE \R The pointer is checked when it was passed to the service.

  \ID SBSW_WRITE_TO_PASSED_LOCAL_STACK_VARIABLE
    \DESCRIPTION    The function writes to a pointer which is passed by local local caller and located on the stack.
    \COUNTERMEASURE \R The calling function ensures that the pointer passed as parameter is valid and references a valid object.

  \ID SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE
  \DESCRIPTION Access of CalloutState using ComStackLib
  \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib

  \ID SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE
    \DESCRIPTION Access of QueueState using ComStackLib
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib

  \ID SBSW_CSM_WRITE_TO_RTERESULT_BY_SIZE
    \DESCRIPTION Access of RteCallback and RteResult with same index using ComStackLib
    \COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

  \ID SBSW_CSM_WRITE_TO_RTERESULT_BY_JOBID
    \DESCRIPTION Access to RteResult via indirection over JobTable.
    \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

  \ID SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID
    \DESCRIPTION Access to QueueState via indirection over JobTable.
    \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

  \ID SBSW_CSM_WRITE_TO_QUEUE_BY_LOCALIDX
    \DESCRIPTION Access to Queue via localQueueIndex which is always checked against its size via indirection over QueueInfo.
    \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

  \ID SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID
    \DESCRIPTION Access of JobState and JobTable with same index using ComStackLib
    \COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

  \ID SBSW_WRITE_TO_OBJECTMAP_BY_JOBID
    \DESCRIPTION Access of ObjectMap and JobTable with same index using ComStackLib
    \COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

  \ID SBSW_CSM_CSL_VAR_ACCESS
    \DESCRIPTION A ComStackLib generated variable is accessed.
    \COUNTERMEASURE \N The variable is written using a generated macro of the CSL. It immediately dereferences the variables address without pointer arithmetic.

  \ID SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER
    \DESCRIPTION    The API call is forwarded to an existing API function.
                    The API is called with one or more pointers passed from the wrapper API function.
    \COUNTERMEASURE \N The called functions in the wrapper functions are API functions that are supposed to be silent.

SBSW_JUSTIFICATION_END */

/* COUNTERMEASURES_BEGIN

  \CM CM_CSM_STACK_VARIABLE The address of the stack variable is always valid for synchronous call context.

COUNTERMEASURES_END */

/* COV_JUSTIFICATION_BEGIN
  \ID COV_CSM_USER_DEFINES
    \ACCEPT TX
    \ACCEPT XF
    \REASON Ifndef is present for redefinition prevention. This allows the user or RTE to manually override this value
            while also ensuring that the constant is present if the user or RTE does not override it.
  \ID COV_CSM_LOCAL_DEFINE
    \ACCEPT TX
    \REASON CSM_LOCAL is not defined externally.
COV_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: CSM.C
 *********************************************************************************************************************/

