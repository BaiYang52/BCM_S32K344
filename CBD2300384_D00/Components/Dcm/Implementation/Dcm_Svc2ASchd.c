/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Dcm_Svc2ASchd.c
 *         \unit  Svc2ASchd
 *        \brief  Contains the implementation of Service 0x2A Scheduler unit.
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#define DCM_SVC2ASCHD_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc2ASchd.h"
#if (DCM_SVC_2A_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Debug.h"
# include "Dcm_Uti.h"
# include "Dcm_Tmr.h"
# include "Dcm_Tsk.h"
# include "Dcm_Net.h"
# include "Dcm_RsrcMgr.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Consistent configuration checks */
# if (DCM_NET_PERIODIC_TX_ENABLED == STD_OFF)
#  error "Service 0x2A is enabled, but no periodic messages have been configured for Dcm. Please, refer to the Dcm TechRef for SID 0x2A configuration aspect."
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
# define Dcm_Svc2AGetSchedulingTimeOfEntry(pSchedTableEntry)         (Dcm_CfgSvc2ASchdRateTime[(pSchedTableEntry)->Rate & DCM_SVC2A_RATE_TYPE_MASK]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Context for handling of periodic transmision */
struct DCM_SVC2ASINGLETONCONTEXTTYPE_TAG
{
  Dcm_Svc2ASchedulerType Scheduler;                        /*!< Scheduler for periodic transmission */
};
typedef struct DCM_SVC2ASINGLETONCONTEXTTYPE_TAG Dcm_Svc2ASingletonContextType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* --- private members --------------------------------------------------------------------------------------------- */
/**********************************************************************************************************************
 *  Dcm_Svc2ASchdFindFreeOrInUseItemByDid()
 *********************************************************************************************************************/
/*! \brief          Tries to find already used DID entry slot. If not, returns the last free found.
 *  \details        -
 *  \param[in]      did           The did
 *  \param[in]      schdHandle    Handle of the found slot
 *  \return         TRUE          A slot was found
 *  \return         FALSE         No slot was found - schdHandle output parameter is invalid
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_Svc2ASchdFindFreeOrInUseItemByDid(
  uint16 did,
  P2VAR(Dcm_Svc2ASchdItemHandleOptType, AUTOMATIC, AUTOMATIC) schdHandle
  );

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdUpdateTimers()
 *********************************************************************************************************************/
/*! \brief          Updates all active scheduled item timers.
 *  \details        -
 *  \return         TRUE     Activate processor task
 *  \return         FALSE    Don't activate processor task
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_Svc2ASchdUpdateTimers(
  void
  );

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdNextEntryIdxInc()
 *********************************************************************************************************************/
/*! \brief          Scheduler item iterator increment.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc2ASchdNextEntryIdxInc(
  void
  );

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdGetEntry()
 *********************************************************************************************************************/
/*! \brief          Obtain a service 0x2A scheduler entry.
 *  \details        -
 *  \param[in]      schdHandle    The scheduler entry handle
 *  \return         Pointer to requested 2A scheduler entry - if handle is invalid, pointer to first entry
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_Svc2ASchdEntryPtrType, DCM_CODE) Dcm_Svc2ASchdGetEntry(
  Dcm_Svc2ASchdItemHandleOptType schdHandle
  );

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdInitDidMgrResources()
 *********************************************************************************************************************/
/*! \brief          Allocate DID manager resources for PDID
 *  \details        -
 *  \param[in,out]  pDataContext    Pointer to the data context
 *  \param[in]      did             The DID that is about to be read
 *  \return         DCM_E_OK        Continue processing
 *  \return         DCM_E_PENDING   DID is currently in use by another service
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2ASchdInitDidMgrResources(
  Dcm_DiagDataContextPtrType pDataContext,
  uint16 did
  );

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdProcessEntry()
 *********************************************************************************************************************/
/*! \brief          A single PDID processing step.
 *  \details        Performs PDID reading and sending on expired PDID slots.
 *  \param[in]      pSchedTableEntry    The scheduler entry to be processed
 *  \return         DCM_E_OK        Continue processing
 *  \return         DCM_E_PENDING   Processing was postponed
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2ASchdProcessEntry(
  Dcm_Svc2ASchdEntryPtrType pSchedTableEntry
  );
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc2ASchdCancelReadByHandle()
 *********************************************************************************************************************/
/*! \brief          Stops a single scheduled item.
 *  \details        -
 *  \param[in]      schdHandle    The handle
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_Svc2ASchdCancelReadByHandle(
  Dcm_Svc2ASchdItemHandleOptType schdHandle
  );

/**********************************************************************************************************************
*  Dcm_Svc2ASchdGetItemByDid()
*********************************************************************************************************************/
/*! \brief          Look up for a scheduled DID.
*  \details        -
*  \param[in]      did           The did
*  \param[in]      schdHandle    Handle of the found entry
*  \return         TRUE          Entry was found
*  \return         FALSE         Entry was not found - schdHandle output parameter is invalid
*  \context        TASK
*  \reentrant      TRUE
*  \pre            -
*********************************************************************************************************************/
DCM_LOCAL FUNC(boolean, DCM_CODE) Dcm_Svc2ASchdGetItemByDid(
  uint16 did,
  P2VAR(Dcm_Svc2ASchdItemHandleOptType, AUTOMATIC, AUTOMATIC) schdHandle
  );

/**********************************************************************************************************************
*  Dcm_Svc2ASchdFinishEntryProcessing()
*********************************************************************************************************************/
/*! \brief          Finalizes scheduler entry processing (with or without response).
*  \details        Resets all global states after a scheduler entry was processed. Initiates response transmission
*                  or releases UUDT message.
*  \param[in]      doSendResponse    Specifies whether a response shall be sent or not
*  \context        TASK
*  \reentrant      FALSE
*  \pre            -
*********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_Svc2ASchdFinishEntryProcessing(
  boolean doSendResponse
  );
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Singleton context of service 2A unit */
DCM_LOCAL VAR(Dcm_Svc2ASingletonContextType, DCM_VAR_NOINIT) Dcm_Svc2ASingletonContext;                                                              /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
# define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc2ASchdFindFreeOrInUseItemByDid()
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
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_Svc2ASchdFindFreeOrInUseItemByDid(
  uint16 did,
  P2VAR(Dcm_Svc2ASchdItemHandleOptType, AUTOMATIC, AUTOMATIC) schdHandle
  )
{
  Dcm_Svc2ASchdItemHandleOptType lSchdIter;
  boolean lSlotFound = FALSE;

  for(lSchdIter = 0; lSchdIter < DCM_SVC_2A_SCHEDULER_SIZE; ++lSchdIter)
  {
    Dcm_Svc2ASchdEntryConstPtrType pSchdEntry = Dcm_Svc2ASchdGetEntry(lSchdIter);

    if(pSchdEntry->Rate == DCM_SVC2A_RATE_TYPE_STOPPED)
    {
      if(lSlotFound == FALSE)
      {
        *schdHandle = lSchdIter; /* reserve first found free slot */                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
        lSlotFound = TRUE;
      }/* else - already set */
    }
    else if(did == pSchdEntry->DidContext.Did)
    {
      *schdHandle = lSchdIter; /* found already in used or just reserved DID -> use the same slot */                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
      lSlotFound = TRUE;
      break;
    }
    else
    {
      /* do nothing */
    }
  }
  return lSlotFound;
}

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdUpdateTimers()
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
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_Svc2ASchdUpdateTimers(
  void
  )
{
  boolean doActivateProcessorTask = FALSE;
  Dcm_Svc2ASchdItemHandleOptType lSchdIter;
# if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
  Dcm_Svc2ASchedRateOptType lFastestRate = DCM_SVC2A_RATE_TYPE_SLOW;
# endif

  for(lSchdIter = 0; lSchdIter < DCM_SVC_2A_SCHEDULER_SIZE; ++lSchdIter)
  {
    Dcm_Svc2ASchdEntryPtrType pSchdEntry = Dcm_Svc2ASchdGetEntry(lSchdIter);

    if(pSchdEntry->Timer != 0u)
    {
      --(pSchdEntry->Timer);                                                                                                                         /* PRQA S 3387 */ /* MD_Dcm_OptimizedInlineCode_3387 */ /* SBSW_DCM_POINTER_WRITE_2ASCHEDULERENTRY */
      if(pSchdEntry->Timer == 0u)
      {
        if(pSchdEntry->Rate < DCM_SVC2A_RATE_TYPE_STOPPED) /* is it scheduled ? */
        {
          doActivateProcessorTask = TRUE;
        }
      }
    }
# if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
    if(pSchdEntry->Rate < DCM_SVC2A_RATE_TYPE_STOPPED) /* is it scheduled ? */
    {
      if(pSchdEntry->Rate > lFastestRate)
      {
        lFastestRate = pSchdEntry->Rate;
      }
    }
# endif
  }
# if(DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
  Dcm_NetPeriodicMsgSetFastestRate(lFastestRate);
# endif
  return doActivateProcessorTask;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdNextEntryIdxInc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc2ASchdNextEntryIdxInc(
  void
  )
{
  ++Dcm_Svc2ASingletonContext.Scheduler.CurrentEntryIndex;
  if(Dcm_Svc2ASingletonContext.Scheduler.CurrentEntryIndex >= DCM_SVC_2A_SCHEDULER_SIZE)
  {
    /* Write always zero (also when only one scheduler element exists, to keep the safe aspect when the wrap-around is
    * detected */
    Dcm_Svc2ASingletonContext.Scheduler.CurrentEntryIndex = 0u;
  }
}

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdGetEntry()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_Svc2ASchdEntryPtrType, DCM_CODE) Dcm_Svc2ASchdGetEntry(
  Dcm_Svc2ASchdItemHandleOptType schdHandle
  )
{
  Dcm_Svc2ASchdEntryPtrType pSchdEntry;

  if(Dcm_DebugDetectRuntimeError(schdHandle >= DCM_SVC_2A_SCHEDULER_SIZE))                                                                           /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    pSchdEntry = &Dcm_Svc2ASingletonContext.Scheduler.Table[0];
  }
  else
  {
    pSchdEntry = &Dcm_Svc2ASingletonContext.Scheduler.Table[schdHandle];
  }
  return pSchdEntry;
}

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdInitDidMgrResources()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2ASchdInitDidMgrResources(
  Dcm_DiagDataContextPtrType pDataContext,
  uint16 did
  )
{
  Std_ReturnType lStdResult;

  lStdResult = Dcm_RsrcMgrGetDidLock(DCM_INITIAL, did, DCM_RSRCMGR_DIDLOCK_OWNER_SID2A);

  if(lStdResult == DCM_E_OK)
  {
    /* prepare first time reading */
    Dcm_Svc2ASingletonContext.Scheduler.OpStatus = DCM_INITIAL;
    Dcm_Svc2ASingletonContext.Scheduler.WriteIndex = 0u;
    Dcm_UtiProvideDataAsU8(pDataContext, Dcm_UtiGetLoByte(did));                                                                                     /* SBSW_DCM_POINTER_WRITE_DATA_CONTEXT */
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdProcessEntry()
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
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2ASchdProcessEntry(
  Dcm_Svc2ASchdEntryPtrType pSchedTableEntry
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;
  boolean doSendData = FALSE;

  /* allocate buffer for response data */
  /* still nothing reserved for this DID? */
  if(Dcm_Svc2ASingletonContext.Scheduler.MsgTxHandle == DCM_NET_INVALID_PTXOBJ_HANDLE)
  {
    Dcm_Svc2ASingletonContext.Scheduler.MsgTxHandle = Dcm_NetPeriodicMsgAllocate(pSchedTableEntry->DidContext.Did);
    if(Dcm_Svc2ASingletonContext.Scheduler.MsgTxHandle == DCM_NET_INVALID_PTXOBJ_HANDLE)
    {
      /* no free message buffer try again later */
      lStdResult = DCM_E_PENDING; /* exit loop and task */
    }
  }

  if(lStdResult == DCM_E_OK)
  {
    Dcm_DiagDataContextType lDataContext;

    /* Init data context */
    Dcm_UtiInitDataContext(&lDataContext
                          ,Dcm_NetPeriodicMsgGetTxBuffer(Dcm_Svc2ASingletonContext.Scheduler.MsgTxHandle)
                          ,(Dcm_MsgLenType)DCM_NET_PERIODIC_BUFFER_SIZE);                                                                            /* SBSW_DCM_POINTER_INIT_DATA_CONTEXT */

    /* Allocate DID manager resource */
    if (Dcm_RsrcMgrHasDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID2A) == FALSE)
    {
      lStdResult = Dcm_Svc2ASchdInitDidMgrResources(&lDataContext, pSchedTableEntry->DidContext.Did);                                                /* SBSW_DCM_POINTER_WRITE_DATA_CONTEXT */
    }

    if(lStdResult == DCM_E_OK)
    {
      Dcm_NegativeResponseCodeType lNrc; /* unused since no NR possible */

      /* Write index is initialized in Dcm_Svc2ASchdInitDidMgrResources and thus always valid */
      Dcm_UtiCommitData(&lDataContext, Dcm_Svc2ASingletonContext.Scheduler.WriteIndex);                                                              /* SBSW_DCM_POINTER_WRITE_DATA_CONTEXT */

      /* read the response data */
      lStdResult = Dcm_DidMgrReadDid(Dcm_Svc2ASingletonContext.Scheduler.OpStatus
                                    ,&lDataContext
                                    ,&(pSchedTableEntry->DidContext)
                                    ,&(Dcm_Svc2ASingletonContext.Scheduler.DidOpTypeContext)
                                    ,&lNrc);                                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */

      Dcm_Svc2ASingletonContext.Scheduler.WriteIndex = (Dcm_CfgNetBufferSizeMemType)lDataContext.Usage;

      switch (lStdResult)
      {
      case DCM_E_OK:
        doSendData = TRUE; /* now the response can be sent */
        break;
      case DCM_E_PENDING:
        Dcm_Svc2ASingletonContext.Scheduler.OpStatus = DCM_PENDING; /* prepare for next call */
        break;
      case DCM_E_NOT_OK:
        /* something went wrong - skip this DID */
        break;
      default:                                                                                                                                       /* COV_DCM_RTM_UNREACHABLE X */
        lStdResult = DCM_E_NOT_OK;
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
        break;
      }

      if (lStdResult != DCM_E_PENDING)
      {
        /* reload the period timer from now on (see ISO14229-1 SID 0x2A scheduler example) */
        /*
        * No critical section for split tasks needed since:
        * - The timer that just has been processed is already stopped and will not be decrement in the
        *   Dcm_Svc2ASchdUpdateTimers utility.
        * - This tasks is a low-priority one and cannot interrupt the timer task (Dcm_OnTimeoutSvc2AScheduler). So
        *   setting a non-zero value will not be an issue.
        */
        pSchedTableEntry->Timer = Dcm_Svc2AGetSchedulingTimeOfEntry(pSchedTableEntry);                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */

        /* Finish scheduled job */
        Dcm_Svc2ASchdFinishEntryProcessing(doSendData);
      }
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc2ASchdCancelReadByHandle()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_Svc2ASchdCancelReadByHandle(
  Dcm_Svc2ASchdItemHandleOptType schdHandle
  )
{
  if((Dcm_RsrcMgrHasDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID2A) == TRUE)
    && (schdHandle == Dcm_Svc2ASingletonContext.Scheduler.CurrentEntryIndex))
  {
    Dcm_NegativeResponseCodeType lNrc;/* not evaluated after the API call */
    Dcm_DiagDataContextType      lDataContext;

    Dcm_UtiInitDataContext(&lDataContext
                          ,&Dcm_NetPeriodicMsgGetTxBuffer(Dcm_Svc2ASingletonContext.Scheduler.MsgTxHandle)[1]
                          ,(Dcm_MsgLenType)(DCM_NET_PERIODIC_BUFFER_SIZE - 1u));                                                                     /* SBSW_DCM_POINTER_INIT_DATA_CONTEXT */

    /* cancel ongoing operation immediately  */
    (void)Dcm_DidMgrReadDid(DCM_CANCEL
                           ,&lDataContext
                           ,&(Dcm_Svc2ASchdGetEntry(schdHandle)->DidContext)
                           ,&(Dcm_Svc2ASingletonContext.Scheduler.DidOpTypeContext)
                           ,&lNrc);                                                                                                                  /* SBSW_DCM_COMB_PTR_FORWARD */

    /* Finish scheduled job */
    Dcm_Svc2ASchdFinishEntryProcessing(FALSE);
  }/* else - nothing to do */
}

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdGetItemByDid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(boolean, DCM_CODE) Dcm_Svc2ASchdGetItemByDid(
  uint16 did,
  P2VAR(Dcm_Svc2ASchdItemHandleOptType, AUTOMATIC, AUTOMATIC) schdHandle
  )
{
  boolean lItemFound = FALSE;
  Dcm_Svc2ASchdItemHandleOptType lSchdIter;

  if(Dcm_Svc2ASingletonContext.Scheduler.NumActiveEntries != 0u)
  {
    for (lSchdIter = 0; lSchdIter < DCM_SVC_2A_SCHEDULER_SIZE; ++lSchdIter)
    {
      Dcm_Svc2ASchdEntryConstPtrType pSchdEntry = Dcm_Svc2ASchdGetEntry(lSchdIter);
      if((pSchdEntry->Rate < DCM_SVC2A_RATE_TYPE_STOPPED)
        && (pSchdEntry->DidContext.Did == did))
      {
        *schdHandle = lSchdIter;                                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
        lItemFound = TRUE;
        break;
      }
    }
  }
  return lItemFound;
}

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdFinishEntryProcessing()
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
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_Svc2ASchdFinishEntryProcessing(
  boolean doSendResponse
  )
{
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj;

  pPeriodicTxObj = Dcm_NetGetPeriodicTxObject(Dcm_Svc2ASingletonContext.Scheduler.MsgTxHandle);

  if(doSendResponse == TRUE)
  {
    Dcm_Svc2ASchdEntryConstPtrType pSchdEntry =
    Dcm_Svc2ASchdGetEntry(Dcm_Svc2ASingletonContext.Scheduler.CurrentEntryIndex);
    Dcm_NetPeriodicMsgTransmit(pPeriodicTxObj, ((PduLengthType)pSchdEntry->DidContext.DidLength + 1u));                                              /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
  }
  else
  {
    /* release resource if not used */
    Dcm_NetPeriodicMsgRelease(pPeriodicTxObj);                                                                                                       /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
  }

  /* free for next DID */
  Dcm_Svc2ASingletonContext.Scheduler.MsgTxHandle = DCM_NET_INVALID_PTXOBJ_HANDLE;

  Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID2A);
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc2ASchdStopAll()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc2ASchdStopAll(
  void
  )
{
  if(Dcm_Svc2ASingletonContext.Scheduler.NumActiveEntries != 0u)
  {
    Dcm_Svc2ASchdItemHandleOptType lSchdIter;

    for(lSchdIter = 0; lSchdIter < DCM_SVC_2A_SCHEDULER_SIZE; ++lSchdIter)
    {
      Dcm_Svc2ASchdStopItemByHandle(lSchdIter);
    }
  }
}

# if (DCM_DIDMGR_PERIODICDYNDID_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2ASchdCancelReadByDid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc2ASchdCancelReadByDid(
  uint16 did
  )
{
  Dcm_Svc2ASchdItemHandleOptType lSchdHandle = 0u;

  if(Dcm_Svc2ASchdGetItemByDid(did, &lSchdHandle) == TRUE)                                                                                           /* SBSW_DCM_POINTER_FORWARD_STACK */
  {
    Dcm_Svc2ASchdCancelReadByHandle(lSchdHandle);
  }
}
# endif

# if (DCM_DIDMGR_PERIODICDYNDID_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2AGetConnectionOfDID()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_Svc2AGetConnectionOfDID(
  uint16 did
  )
{
  Dcm_NetConnRefMemType lConnHdl = DCM_NET_INVALID_CONNHDL;
  Dcm_Svc2ASchdItemHandleOptType lSchdHandle = 0u;

  if(Dcm_Svc2ASchdGetItemByDid(did, &lSchdHandle) == TRUE)                                                                                           /* SBSW_DCM_POINTER_FORWARD_STACK */
  {
    Dcm_Svc2ASchdEntryConstPtrType pSchdEntry = Dcm_Svc2ASchdGetEntry(lSchdHandle);
    if(pSchdEntry->Rate < DCM_SVC2A_RATE_TYPE_STOPPED)
    {
      lConnHdl = Dcm_NetPeriodicMsgGetConnection();
    }
  }
  return lConnHdl;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdStopItemByHandle()
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
FUNC(void, DCM_CODE) Dcm_Svc2ASchdStopItemByHandle(
  Dcm_Svc2ASchdItemHandleOptType schdHandle
  )
{
  Dcm_Svc2ASchdEntryPtrType pSchdEntry = Dcm_Svc2ASchdGetEntry(schdHandle);
  if(pSchdEntry->Rate < DCM_SVC2A_RATE_TYPE_STOPPED)
  {
    pSchdEntry->Rate = DCM_SVC2A_RATE_TYPE_STOPPED;                                                                                                  /* SBSW_DCM_POINTER_WRITE_2ASCHEDULERENTRY */
    /* must have at least one element (this one) */
    Dcm_DebugAssert((Dcm_Svc2ASingletonContext.Scheduler.NumActiveEntries > 0u)
      , DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                                                                                                      /* COV_DCM_RTM_DEV_DEBUG XF */

    --Dcm_Svc2ASingletonContext.Scheduler.NumActiveEntries; /* update statistic */

    Dcm_Svc2ASchdCancelReadByHandle(schdHandle);

    Dcm_NetPeriodicMsgStopDid(pSchdEntry->DidContext.Did);
    if(Dcm_Svc2ASingletonContext.Scheduler.NumActiveEntries == 0u)/* scheduler is empty */
    {
      Dcm_NetPeriodicMsgResetConnection(); /* allow other tester to use service 0x2A */
      Dcm_Svc2ASingletonContext.Scheduler.CurrentEntryIndex = 0u;
# if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
      /* stop timer updater */
      Dcm_TmrStopTimer(DCM_TMR_ID_SVC2A_SCHEDULER);
# else
      /* no need to deactivate the task - if the event was set, the task will deactivate itself after a single short
      * execution time activation */
# endif
    }
  }/* else - this item is not scheduled at all */
}

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdStopItemByDid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc2ASchdStopItemByDid(
  uint16 did
  )
{
  Dcm_Svc2ASchdItemHandleOptType lSchdHandle = 0u;

  if(Dcm_Svc2ASchdGetItemByDid(did, &lSchdHandle) == TRUE)                                                                                           /* SBSW_DCM_POINTER_FORWARD_STACK */
  {
    Dcm_Svc2ASchdStopItemByHandle(lSchdHandle);
  }
}

/**********************************************************************************************************************
 *  Dcm_Svc2ATaskSchdProcess()
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
 */
FUNC(void, DCM_CODE) Dcm_Svc2ATaskSchdProcess(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

# if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
  /* scheduler timer update is done in a dedicated task to achieve maximum timing accuracy */
# else
  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_SVC2A_SCHEDULER_TMR_UPD))
  {
    boolean doActivateProcessorTask;

    if(Dcm_Svc2ASingletonContext.Scheduler.NumActiveEntries != 0u)
    {
      doActivateProcessorTask = Dcm_Svc2ASchdUpdateTimers();
      if(doActivateProcessorTask == TRUE)
      {
        Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_SVC2A_SCHEDULER_PROCESS);/* let's check which did is in a timeout */                      /* SBSW_DCM_PARAM_PTR_WRITE */
      }
      /* still something to do - keep timer updater alive */
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_SVC2A_SCHEDULER_TMR_UPD);                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_SVC2A_SCHEDULER_PROCESS))
# endif
  {
    Dcm_Svc2ASchdItemHandleOptType lSchdIter;

    /* loop over all scheduled items (maximum once per task activation) */
    for(lSchdIter = 0; lSchdIter < DCM_SVC_2A_SCHEDULER_SIZE; ++lSchdIter)
    {
      Dcm_Svc2ASchdEntryPtrType pSchedTableEntry =
        Dcm_Svc2ASchdGetEntry(Dcm_Svc2ASingletonContext.Scheduler.CurrentEntryIndex);

      if((pSchedTableEntry->Timer == 0u)
        && ((pSchedTableEntry->Rate & DCM_SVC2A_RATE_TYPE_STOPPED) == 0u))
      {
        Std_ReturnType lStdReturn;

        lStdReturn = Dcm_Svc2ASchdProcessEntry(pSchedTableEntry);                                                                                    /* SBSW_DCM_POINTER_WRITE_2ASCHEDULERENTRY */
        if(lStdReturn == DCM_E_PENDING)
        {
          Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_SVC2A_SCHEDULER_PROCESS);                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
          break; /* leave the loop and the task */
        }
      }
      /* next entry to be processed */
      Dcm_Svc2ASchdNextEntryIdxInc();
    }
  }
}

# if (DCM_TMR_NUM_SVC2A_SCHEDULER > 0u)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutSvc2AScheduler()
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
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutSvc2AScheduler(
  Dcm_ThreadIdMemType threadId
  )
{
  Dcm_TmrTimerCntrMemType lReturn = 0;

  DCM_IGNORE_UNREF_PARAM(threadId);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if(Dcm_Svc2ASingletonContext.Scheduler.NumActiveEntries != 0u)
  {
    boolean doActivateProcessorTask;

    doActivateProcessorTask = Dcm_Svc2ASchdUpdateTimers();

    if(doActivateProcessorTask == TRUE)
    {
      Dcm_TskSetEvent(DCM_TSK_ID_SVC2A_SCHEDULER, DCM_TSK_EV_SVC2A_SCHEDULER_PROCESS);/* at least one job to do */
    }
    lReturn = 1u;/* immediate timeout in next slot */
  }/* else - a NOP activate - will be the last one */
  return lReturn;/* no reload */
}
# endif

/**********************************************************************************************************************
 *  Dcm_Service2ASchedulerInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Service2ASchedulerInit(
  void
  )
{
  Dcm_Svc2ASingletonContext.Scheduler.NumActiveEntries = 0u;
  Dcm_Svc2ASingletonContext.Scheduler.CurrentEntryIndex = 0u;
  Dcm_Svc2ASingletonContext.Scheduler.MsgTxHandle = DCM_NET_INVALID_PTXOBJ_HANDLE;

  {
    Dcm_Svc2ASchdItemHandleOptType lSchdIter;

    for(lSchdIter = 0; lSchdIter < DCM_SVC_2A_SCHEDULER_SIZE; ++lSchdIter)
    {
      Dcm_Svc2ASchdEntryPtrType pSchdEntry = Dcm_Svc2ASchdGetEntry(lSchdIter);
      pSchdEntry->Timer = 0u;                                                                                                                        /* SBSW_DCM_POINTER_WRITE_2ASCHEDULERENTRY */
      pSchdEntry->Rate = DCM_SVC2A_RATE_TYPE_STOPPED;                                                                                                /* SBSW_DCM_POINTER_WRITE_2ASCHEDULERENTRY */
    }
  }
}

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdAddItemByDid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc2ASchdAddItemByDid(
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext                                                                                                    /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_Svc2ASchdItemHandleOptType lSchdHandle = DCM_SVC_2A_SCHEDULER_SIZE;
  Dcm_Svc2ASchdEntryPtrType pSchdEntry;

  if(Dcm_Svc2ASchdFindFreeOrInUseItemByDid(pDidInfoContext->Did, &lSchdHandle) == TRUE)                                                              /* SBSW_DCM_POINTER_FORWARD_STACK */
  {
    pSchdEntry = Dcm_Svc2ASchdGetEntry((Dcm_Svc2ASchdItemHandleOptType)lSchdHandle);

    pSchdEntry->DidContext = *pDidInfoContext;                                                                                                       /* SBSW_DCM_POINTER_WRITE_2ASCHEDULERENTRY */
    pSchdEntry->Rate |= DCM_SVC2A_RATE_TYPE_RESERVED;                                                                                                /* SBSW_DCM_POINTER_WRITE_2ASCHEDULERENTRY */
  }
  else
  {
    /* there shall always be free space when this function is called, otherwise check the SID 0x2A processor */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
  }
}

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdCommit()
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
 */
FUNC(void, DCM_CODE) Dcm_Svc2ASchdCommit(
  Dcm_Svc2ASchedRateMemType rate
  )
{
  Dcm_Svc2ASchdItemHandleOptType lSchdIter;

  Dcm_SplitTaskEnterCS();/* protect against timer task update */

  for(lSchdIter = 0; lSchdIter < DCM_SVC_2A_SCHEDULER_SIZE; ++lSchdIter)
  {
    Dcm_Svc2ASchdEntryPtrType pSchdEntry = Dcm_Svc2ASchdGetEntry(lSchdIter);
    if((pSchdEntry->Rate & DCM_SVC2A_RATE_TYPE_RESERVED) != 0u)
    {
      if((pSchdEntry->Rate & DCM_SVC2A_RATE_TYPE_STOPPED) != 0u)
      {
        /* update statistic of first activation of this DID */
        ++Dcm_Svc2ASingletonContext.Scheduler.NumActiveEntries;
      }
      /* must not exceed the scheduler size */
      Dcm_DebugAssert((Dcm_Svc2ASingletonContext.Scheduler.NumActiveEntries <= DCM_SVC_2A_SCHEDULER_SIZE)
                     ,DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                                                                                        /* COV_DCM_RTM_DEV_DEBUG XF */

      pSchdEntry->Rate = rate; /* set the new rate */                                                                                                /* SBSW_DCM_POINTER_WRITE_2ASCHEDULERENTRY */
      pSchdEntry->Timer = 0u; /* enforce immediate timeout */                                                                                        /* SBSW_DCM_POINTER_WRITE_2ASCHEDULERENTRY */
    }
  }
  Dcm_SplitTaskLeaveCS();

  if(Dcm_Svc2ASingletonContext.Scheduler.NumActiveEntries != 0u)
  {
# if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
    Dcm_TmrStartTimer(DCM_TMR_ID_SVC2A_SCHEDULER, 1u);/* start timer updater */
# endif
    /* start updating the timers/processing new DIDs */
    Dcm_TskSetEvent(DCM_TSK_ID_SVC2A_SCHEDULER
                   ,(Dcm_TskTaskEvMemType)(DCM_TSK_EV_SVC2A_SCHEDULER_TMR_UPD | DCM_TSK_EV_SVC2A_SCHEDULER_PROCESS));
  }
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
*  Dcm_Svc2ASchdDiscard()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
FUNC(void, DCM_CODE) Dcm_Svc2ASchdDiscard(
  void
  )
{
  Dcm_Svc2ASchdItemHandleOptType lSchdIter;

  for(lSchdIter = 0; lSchdIter < DCM_SVC_2A_SCHEDULER_SIZE; ++lSchdIter)
  {
    Dcm_Svc2ASchdEntryPtrType pSchdEntry = Dcm_Svc2ASchdGetEntry(lSchdIter);
    Dcm_UtiBitOpClr(Dcm_Svc2ASchedRateMemType, pSchdEntry->Rate, DCM_SVC2A_RATE_TYPE_RESERVED);                                                      /* SBSW_DCM_POINTER_WRITE_2ASCHEDULERENTRY */
  }
}

# if (DCM_DIDMGR_PERIODICDID_CLR_ON_STATE_CHG_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2AOnStateChanged()
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
FUNC(void, DCM_CODE) Dcm_Svc2AOnStateChanged(
  void
  )
{
  if(Dcm_Svc2ASingletonContext.Scheduler.NumActiveEntries != 0u)
  {
    Dcm_NegativeResponseCodeType   lNrc;
    Dcm_Svc2ASchdItemHandleOptType lSchdIter;
    Std_ReturnType lStdReturn;

    for(lSchdIter = 0; lSchdIter < DCM_SVC_2A_SCHEDULER_SIZE; ++lSchdIter)
    {
      Dcm_Svc2ASchdEntryPtrType pSchdEntry = Dcm_Svc2ASchdGetEntry(lSchdIter);                                                                       /* PRQA S 3679 */ /* MD_Dcm_Design_3679 */
      if(pSchdEntry->Rate < DCM_SVC2A_RATE_TYPE_STOPPED)/* is still an active entry? */
      {
        lStdReturn = Dcm_DidMgrDynDidStateCheck(DCM_NET_INVALID_CONNHDL, &(pSchdEntry->DidContext), &lNrc);                                          /* SBSW_DCM_COMB_PTR_FORWARD */
        if(lStdReturn == DCM_E_NOT_OK)
        {
          Dcm_Svc2ASchdStopItemByHandle(lSchdIter);
        }
      }
    }
  }
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdGetFreeSpace()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc2ASchdItemHandleMemType, DCM_CODE) Dcm_Svc2ASchdGetFreeSpace(
  void
  )
{
  return (Dcm_Svc2ASchdItemHandleMemType)(DCM_SVC_2A_SCHEDULER_SIZE - Dcm_Svc2ASingletonContext.Scheduler.NumActiveEntries);
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_2A_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc2ASchd.c
 *********************************************************************************************************************/
