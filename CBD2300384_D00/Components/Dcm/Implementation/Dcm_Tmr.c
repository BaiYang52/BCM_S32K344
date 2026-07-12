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
/**        \file  Dcm_Tmr.c
 *         \unit  Tmr
 *        \brief  Contains the implementation of Timer Manager unit.
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
#define DCM_TMR_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Tmr.h"
#include "Dcm_Tsk.h"
#include "Dcm_Debug.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Timer thread ability types */
#define DCM_TMR_ATR_THREAD_SINGLE                                    ((Dcm_TmrTimerAttributeType)0x00u) /*!< Timer does not support multi threading */
#define DCM_TMR_ATR_THREAD_MULTI                                     ((Dcm_TmrTimerAttributeType)0x01u) /*!< Timer supports multi threading */

/*! Calculation of all timer context IDs */
#define DCM_TMR_CONTEXT_ID_P2                                        ((Dcm_TmrTimerIdMemType)(0u + 0u))                                                                 /*!< The timer context ID of the P2 timer */
#define DCM_TMR_CONTEXT_ID_S3                                        ((Dcm_TmrTimerIdMemType)(DCM_TMR_CONTEXT_ID_P2 + DCM_TMR_CONTEXT_NUM_P2))                          /*!< The timer context ID of the S3 timer */
#define DCM_TMR_CONTEXT_ID_FBLRES                                    ((Dcm_TmrTimerIdMemType)(DCM_TMR_CONTEXT_ID_S3 + DCM_TMR_CONTEXT_NUM_S3))                          /*!< The timer context ID of the ComM going to FullCommMode after ECU power-on/reset timer */
#define DCM_TMR_CONTEXT_ID_SECURITY                                  ((Dcm_TmrTimerIdMemType)(DCM_TMR_CONTEXT_ID_FBLRES + DCM_TMR_CONTEXT_NUM_FBLRES))                  /*!< The timer context ID of the diagnostic service 0x27 failed attempt delay base timer */
#define DCM_TMR_CONTEXT_ID_PERIODIC_TX                               ((Dcm_TmrTimerIdMemType)(DCM_TMR_CONTEXT_ID_SECURITY + DCM_TMR_CONTEXT_NUM_SECURITY))              /*!< The timer context ID of the periodic message transmission base timer */
#define DCM_TMR_CONTEXT_ID_SVC2A_SCHEDULER                           ((Dcm_TmrTimerIdMemType)(DCM_TMR_CONTEXT_ID_PERIODIC_TX + DCM_TMR_CONTEXT_NUM_PERIODIC_TX))        /*!< The timer context ID of the diagnostic service 0x2A periodic DID scheduler base timer */
#define DCM_TMR_CONTEXT_ID_SVC86_SCHEDULER                           ((Dcm_TmrTimerIdMemType)(DCM_TMR_CONTEXT_ID_SVC2A_SCHEDULER + DCM_TMR_CONTEXT_NUM_SVC2A_SCHEDULER)) /*!< The timer context ID of the diagnostic service 0x86 intermessage timer */
#define DCM_TMR_CONTEXT_ID_KEEP_ALIVE                                ((Dcm_TmrTimerIdMemType)(DCM_TMR_CONTEXT_ID_SVC86_SCHEDULER + DCM_TMR_CONTEXT_NUM_SVC86_SCHEDULER)) /*!< The timer context ID of the extended keep-alive timer */
#define DCM_TMR_CONTEXT_ID_AUTHENTICATION                            ((Dcm_TmrTimerIdMemType)(DCM_TMR_CONTEXT_ID_KEEP_ALIVE + DCM_TMR_CONTEXT_NUM_KEEP_ALIVE))          /*!< The timer context ID of the extended keep-alive timer */

/*
 * Calculation of the total number of system timers
 */
/*! A single timer for P2 monitoring is always needed and thread specific */
#define DCM_TMR_CONTEXT_NUM_P2                                       (DCM_TMR_NUM_P2 * DCM_NUM_THREADS)

/*! A single timer for S3 monitoring is needed */
#define DCM_TMR_CONTEXT_NUM_S3                                       (DCM_TMR_NUM_S3 * 1u)

/*! A single timer for monitoring the time between ECU start up and ComM going into FullComMode is needed */
#define DCM_TMR_CONTEXT_NUM_FBLRES                                   (DCM_TMR_NUM_FBLRES * 1u)

/*! A single timer for monitoring all security access delay times is needed */
#define DCM_TMR_CONTEXT_NUM_SECURITY                                 (DCM_TMR_NUM_SECURITY * 1u)

/*! A single timer for monitoring all periodic message transmission times is needed */
#define DCM_TMR_CONTEXT_NUM_PERIODIC_TX                              (DCM_TMR_NUM_PERIODIC_TX * 1u)

/*! A single timer for monitoring periodic DID scheduler timers is needed */
#define DCM_TMR_CONTEXT_NUM_SVC2A_SCHEDULER                          (DCM_TMR_NUM_SVC2A_SCHEDULER * 1u)

/*! A single timer for managing the RoE transmission intermessage time is needed */
#define DCM_TMR_CONTEXT_NUM_SVC86_SCHEDULER                          (DCM_TMR_NUM_SVC86_SCHEDULER * 1u)

/*! A single timer for monitoring the extended keep-alive delay time is needed */
#define DCM_TMR_CONTEXT_NUM_KEEP_ALIVE                               (DCM_TMR_NUM_KEEP_ALIVE * 1u)

/*! A single timer for monitoring all authentication session time is needed */
#define DCM_TMR_CONTEXT_NUM_AUTHENTICATION                           (DCM_TMR_NUM_AUTHENTICATION * 1u)

/*! Pre-compile total number of timer contexts calculation.
    Note: Do not cast the sum, since it is used within a pre-processor arithmetic expression (refer to Dcm_TmrMaskMemType definition section) */
#define DCM_TMR_NUM_TIMER_CONTEXTS                                   ( DCM_TMR_CONTEXT_NUM_P2 \
                                                                     + DCM_TMR_CONTEXT_NUM_S3 \
                                                                     + DCM_TMR_CONTEXT_NUM_FBLRES \
                                                                     + DCM_TMR_CONTEXT_NUM_SECURITY \
                                                                     + DCM_TMR_CONTEXT_NUM_PERIODIC_TX \
                                                                     + DCM_TMR_CONTEXT_NUM_SVC2A_SCHEDULER \
                                                                     + DCM_TMR_CONTEXT_NUM_SVC86_SCHEDULER \
                                                                     + DCM_TMR_CONTEXT_NUM_KEEP_ALIVE \
                                                                     + DCM_TMR_CONTEXT_NUM_AUTHENTICATION)

/*! The casted version of the DCM_TMR_NUM_TIMER_CONTEXTS within the C-code */
#define DCM_TMR_NUM_TIMER_CONTEXTS_CASTED                            ((Dcm_TmrTimerIdMemType) DCM_TMR_NUM_TIMER_CONTEXTS)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8 Dcm_TmrTimerAttributeType;

/*! Function pointer prototype for a system timer related time-out notification */
typedef P2FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE, Dcm_TmrTimeoutFuncType) (
  Dcm_ThreadIdMemType threadId  /* IN: Active thread */
  );

/*! Single timer configuration data */
struct DCM_TMRTIMERINFOTYPE_TAG
{
  Dcm_TmrTimeoutFuncType     OnTimeoutFunc;    /*!< Pointer to the timeout notification handler of kind Dcm_OnTimeout<TimerName>() (e.g. Dcm_OnTimeoutP2()) */
  Dcm_TmrTimerAttributeType  Attributes;       /*!< Configuration attributes (Valid values: DCM_TMR_ATR_*) */
  Dcm_TmrTimerIdMemType      TimerContextRef;  /*!< Reference to Dcm_TmrSingletonContext.TimerContext[] */
};
typedef struct DCM_TMRTIMERINFOTYPE_TAG Dcm_TmrTimerInfoType;
typedef P2CONST(Dcm_TmrTimerInfoType, TYPEDEF, DCM_CONST) Dcm_TmrTimerInfoPtrType;

#if (DCM_TMR_NUM_TIMER_CONTEXTS <= 8u)
/*! Base data type (memory consumption and runtime optimized) for a timer-pool bitmap when no more than 8 system timers are needed */
typedef uint8        Dcm_TmrMaskMemType;
typedef uint8_least  Dcm_TmrMaskOptType;
#elif (DCM_TMR_NUM_TIMER_CONTEXTS <= 16u)                                                                                                            /* COV_DCM_SUPPORT_ALWAYS TX */
/*! Base data type (memory consumption and runtime optimized) for a timer-pool bitmap when no more than 16 system timers are needed */
typedef uint16        Dcm_TmrMaskMemType;
typedef uint16_least  Dcm_TmrMaskOptType;
#elif (DCM_TMR_NUM_TIMER_CONTEXTS <= 32u)
/*! Base data type (memory consumption and runtime optimized) for a timer-pool bitmap when no more than 32 system timers are needed */
typedef uint32        Dcm_TmrMaskMemType;
typedef uint32_least  Dcm_TmrMaskOptType;
#else
# error "Too many system timers!"
#endif

/*! Control data of the timer sub-component */
struct DCM_TMRSINGLETONCONTEXTTYPE_TAG
{
  volatile Dcm_TmrMaskMemType       ControlMask;                               /*!< Control bit mask for all configured system timers */
           Dcm_TmrTimerContextType  TimerContext[DCM_TMR_NUM_TIMER_CONTEXTS];  /*!< Runtime data of each available timer */
};
typedef struct DCM_TMRSINGLETONCONTEXTTYPE_TAG Dcm_TmrSingletonContextType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_TmrGetTimerInfo()
 *********************************************************************************************************************/
/*! \brief          Returns a timer info element of the given index.
 *  \details        -
 *  \param[in]      index    Unique handle to a timer info
 *  \return         The timer info element corresponding to the index
 *  \return         NULL_PTR      On invalid array boundaries (if runtime checks enabled)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_TmrTimerInfoPtrType, DCM_CODE) Dcm_TmrGetTimerInfo(
  Dcm_TmrTimerIdOptType index
  );

/**********************************************************************************************************************
 *  Dcm_TmrSetTimerStopped()
 *********************************************************************************************************************/
/*! \brief         Stops the timer with the given ID.
 *  \details        -
 *  \param[in]      timerContextId      The timer ID
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_TmrSetTimerStopped(
  Dcm_TmrTimerIdOptType timerId
  );

/**********************************************************************************************************************
 *  Dcm_TmrSetTimerRunning()
 *********************************************************************************************************************/
/*! \brief         Sets the timer with the given ID to running.
 *  \details        -
 *  \param[in]      timerContextId      The timer ID
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_TmrSetTimerRunning(
  Dcm_TmrTimerIdOptType timerId
  );

/**********************************************************************************************************************
 *  Dcm_TmrIsTimerRunningContext()
 *********************************************************************************************************************/
/*! \brief          Checks whether the timer with the given Context ID is running.
 *  \details        -
 *  \param[in]      timerContextId      The timer Context ID
 *  \return         TRUE                Timer is running
 *  \return         FALSE               Timer is not running
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_TmrIsTimerRunningContext(
  Dcm_TmrTimerIdOptType timerContextId
  );

/**********************************************************************************************************************
 *  Dcm_TmrGetTimerContext()
 *********************************************************************************************************************/
/*! \brief          Returns a timer context element of the given index.
 *  \details        -
 *  \param[in]      index    Unique handle to a timer context
 *  \return         The timer context element corresponding to the index
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_TmrTimerContextPtrType, DCM_CODE) Dcm_TmrGetTimerContext(
  Dcm_TmrTimerIdOptType index
  );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Singleton context of TimerMgr unit */
DCM_LOCAL VAR(Dcm_TmrSingletonContextType, DCM_VAR_NOINIT) Dcm_TmrSingletonContext;
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
DCM_LOCAL CONST(Dcm_TmrTimerInfoType, DCM_CONST) Dcm_TmrTimerInfo[DCM_TMR_NUM_TIMERS] =                                                              /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  {Dcm_OnTimeoutP2,                       DCM_TMR_ATR_THREAD_MULTI,  DCM_TMR_CONTEXT_ID_P2 }
 ,{Dcm_OnTimeoutS3,                       DCM_TMR_ATR_THREAD_SINGLE, DCM_TMR_CONTEXT_ID_S3 }
#if (DCM_TMR_NUM_FBLRES > 0u)
 ,{Dcm_OnTimeoutFblRes,                   DCM_TMR_ATR_THREAD_SINGLE, DCM_TMR_CONTEXT_ID_FBLRES }
#endif
#if (DCM_TMR_NUM_SECURITY > 0u)
 ,{Dcm_OnTimeoutSecurityAccess,           DCM_TMR_ATR_THREAD_SINGLE, DCM_TMR_CONTEXT_ID_SECURITY }
#endif
#if (DCM_TMR_NUM_PERIODIC_TX > 0u)
 ,{Dcm_OnTimeoutPeriodicTx,               DCM_TMR_ATR_THREAD_SINGLE, DCM_TMR_CONTEXT_ID_PERIODIC_TX }
#endif
#if (DCM_TMR_NUM_SVC2A_SCHEDULER > 0u)
 ,{Dcm_OnTimeoutSvc2AScheduler,           DCM_TMR_ATR_THREAD_SINGLE, DCM_TMR_CONTEXT_ID_SVC2A_SCHEDULER }
#endif
#if (DCM_TMR_NUM_SVC86_SCHEDULER > 0u)
 ,{Dcm_ExtOnTimeoutSvc86Scheduler, DCM_TMR_ATR_THREAD_SINGLE, DCM_TMR_CONTEXT_ID_SVC86_SCHEDULER }
#endif
#if (DCM_TMR_NUM_KEEP_ALIVE > 0u)
 ,{Dcm_OnTimeoutKeepAlive,                DCM_TMR_ATR_THREAD_SINGLE, DCM_TMR_CONTEXT_ID_KEEP_ALIVE }
#endif
#if (DCM_TMR_NUM_AUTHENTICATION > 0u)
 ,{Dcm_OnTimeoutAuthentication,           DCM_TMR_ATR_THREAD_SINGLE, DCM_TMR_CONTEXT_ID_AUTHENTICATION }
#endif
};
#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_TmrSetTimerStopped()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_TmrSetTimerStopped(
  Dcm_TmrTimerIdOptType timerId
  )
{
  Dcm_UtiBitOpClr(Dcm_TmrMaskMemType, Dcm_TmrSingletonContext.ControlMask, Dcm_UtiGetBitFromIndex(Dcm_TmrMaskMemType, timerId));
}

/**********************************************************************************************************************
 *  Dcm_TmrSetTimerRunning()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_TmrSetTimerRunning(
  Dcm_TmrTimerIdOptType timerId
  )
{
  Dcm_UtiBitOpSet(Dcm_TmrMaskMemType, Dcm_TmrSingletonContext.ControlMask, Dcm_UtiGetBitFromIndex(Dcm_TmrMaskMemType, timerId));
}

/**********************************************************************************************************************
 *  Dcm_TmrIsTimerRunningContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_TmrIsTimerRunningContext(
  Dcm_TmrTimerIdOptType timerContextId
  )
{
  return Dcm_UtiBitOpTest(Dcm_TmrMaskMemType, Dcm_TmrSingletonContext.ControlMask, Dcm_UtiGetBitFromIndex(Dcm_TmrMaskMemType, timerContextId));
}

/**********************************************************************************************************************
 *  Dcm_TmrGetTimerInfo()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_TmrTimerInfoPtrType, DCM_CODE) Dcm_TmrGetTimerInfo(
  Dcm_TmrTimerIdOptType index
  )
{
  Dcm_TmrTimerInfoPtrType lResult;

  if(Dcm_DebugDetectRuntimeError(index >= DCM_TMR_NUM_TIMERS_CASTED))                                                                                /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &Dcm_TmrTimerInfo[0];
  }
  else
  {
    lResult = &Dcm_TmrTimerInfo[index];
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_TmrGetTimerContext()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_TmrTimerContextPtrType, DCM_CODE) Dcm_TmrGetTimerContext(
  Dcm_TmrTimerIdOptType index
  )
{
  Dcm_TmrTimerContextPtrType lResult;

  if(Dcm_DebugDetectRuntimeError(index >= DCM_TMR_NUM_TIMER_CONTEXTS_CASTED))                                                                        /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &Dcm_TmrSingletonContext.TimerContext[0];
  }
  else
  {
    lResult = &Dcm_TmrSingletonContext.TimerContext[index];
  }

  return lResult;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_TmrInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_TmrInit(
  void
  )
{
  Dcm_TmrTimerIdMemType  lTimerInfoIter;
  Dcm_TmrTimerIdOptType  lTimerContextIter;

  lTimerInfoIter    = DCM_TMR_NUM_TIMERS_CASTED - 1u;
  lTimerContextIter = DCM_TMR_NUM_TIMER_CONTEXTS_CASTED;

  do
  {
    --lTimerContextIter;

    Dcm_TmrSingletonContext.TimerContext[lTimerContextIter].TimerInfoRef = lTimerInfoIter;                                                           /* SBSW_DCM_LOOP */

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
    if(Dcm_TmrTimerInfo[lTimerInfoIter].TimerContextRef == lTimerContextIter)
#endif
    {
      --lTimerInfoIter;
    }
  }
  while(lTimerContextIter != 0u);

  Dcm_TmrSingletonContext.ControlMask = 0; /* mark all timers as not running */
}

/**********************************************************************************************************************
 *  Dcm_TmrSetTimerByThread()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_TmrSetTimerByThread(
  Dcm_TmrTimerIdOptType timerId,
  Dcm_TmrTimerCntrMemType timeTicks,
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_TmrTimerIdOptType      lTimerContextId;
  Dcm_TmrTimerContextPtrType pTimerContext;

  lTimerContextId = (Dcm_TmrTimerIdOptType)(Dcm_TmrGetTimerInfo(timerId)->TimerContextRef + threadId);
  pTimerContext   = Dcm_TmrGetTimerContext(lTimerContextId);

  /* Update the corresponding timer counter */
  pTimerContext->Timer = timeTicks;                                                                                                                  /* SBSW_DCM_POINTER_WRITE_TIMERCONTEXT */
}

/**********************************************************************************************************************
 *  Dcm_TmrStartTimerByThread()
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
 */
FUNC(void, DCM_CODE) Dcm_TmrStartTimerByThread(
  Dcm_TmrTimerIdOptType timerId,
  Dcm_TmrTimerCntrMemType timeTicks,
  Dcm_ThreadIdOptType threadId
  )
{
  if (timeTicks != 0u)
  {
    Dcm_TmrTimerIdOptType      lTimerContextId;
    Dcm_TmrTimerContextPtrType pTimerContext;

    lTimerContextId = (Dcm_TmrTimerIdOptType)(Dcm_TmrGetTimerInfo(timerId)->TimerContextRef + threadId);
    pTimerContext = Dcm_TmrGetTimerContext(lTimerContextId);

    /* Enter critical section (Reason: A timer can be modified from an ISR or higher priority task) */
    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
      *=================================*/
    /* Update the corresponding timer counter */
    pTimerContext->Timer = timeTicks;                                                                                                                /* SBSW_DCM_POINTER_WRITE_TIMERCONTEXT */

    /* Mark timer as running */
    Dcm_TmrSetTimerRunning(lTimerContextId);
    /*=================================*
      END CRITICAL SECTION
      *=================================*/
    Dcm_UtiLeaveCriticalSection();
    /* Signal the Dcm_TmrTaskTimer() that at least one timer is active */
    Dcm_TskSetEvent(DCM_TSK_ID_TIMER, DCM_TSK_EV_TIMER_ACTIVE);
  }
}

/**********************************************************************************************************************
 *  Dcm_TmrStopTimerByThread()
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
FUNC(void, DCM_CODE) Dcm_TmrStopTimerByThread(
  Dcm_TmrTimerIdOptType timerId,
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_TmrTimerIdOptType lTimerContextId;

  lTimerContextId = (Dcm_TmrTimerIdOptType)(Dcm_TmrGetTimerInfo(timerId)->TimerContextRef + threadId);

  /* Enter critical section (Reason: A timer can be modified from an ISR or higher priority task) */
  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
  /* Mark timer as not running */
  Dcm_TmrSetTimerStopped(lTimerContextId);
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  Dcm_UtiLeaveCriticalSection();
}

/**********************************************************************************************************************
 *  Dcm_TmrIsTimerRunningByThread()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_TmrIsTimerRunningByThread(
  Dcm_TmrTimerIdOptType timerId,
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_TmrTimerIdOptType lTimerContextId;
  boolean lReturnValue;

  lTimerContextId = (Dcm_TmrTimerIdOptType)(Dcm_TmrGetTimerInfo(timerId)->TimerContextRef + threadId);

  /* Enter critical section (Reason: A timer can be modified from an ISR or higher priority task) */
  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
  /* Mark timer as not running */
  lReturnValue = Dcm_TmrIsTimerRunningContext(lTimerContextId);
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  Dcm_UtiLeaveCriticalSection();
  return lReturnValue;
}

/**********************************************************************************************************************
 *  Dcm_TmrResumeTimerByThread()
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
FUNC(void, DCM_CODE) Dcm_TmrResumeTimerByThread(
  Dcm_TmrTimerIdOptType timerId,
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_TmrTimerIdOptType           lTimerContextId;
  Dcm_TmrTimerContextConstPtrType pTimerContext;

  lTimerContextId = (Dcm_TmrTimerIdOptType)(Dcm_TmrGetTimerInfo(timerId)->TimerContextRef + threadId);
  pTimerContext   = Dcm_TmrGetTimerContext(lTimerContextId);

  /* Enter critical section (Reason: A timer can be modified from an ISR or higher priority task) */
  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
  if(pTimerContext->Timer != 0u)
  {
    /* Mark timer as running */
    Dcm_TmrSetTimerRunning(lTimerContextId);
  }
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  Dcm_UtiLeaveCriticalSection();
  /* Signal the Dcm_TmrTaskTimer() that at least one timer is active */
  Dcm_TskSetEvent(DCM_TSK_ID_TIMER, DCM_TSK_EV_TIMER_ACTIVE);
}

/**********************************************************************************************************************
 *  Dcm_TmrTaskTimer()
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
 */
FUNC(void, DCM_CODE) Dcm_TmrTaskTimer(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  Dcm_TmrMaskOptType    expiredTimers = 0u;
  Dcm_TmrTimerIdOptType timerIter;

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* The only event here is the DCM_TSK_EV_TIMER_ACTIVE. If not set, this task will not be called -> no additional event check is necessary */

  /* Enter critical section (Reason: A timer can be modified from an ISR or higher priority task) */
  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
  /* Iterate over all system timers: */
  for(timerIter = 0; timerIter < DCM_TMR_NUM_TIMER_CONTEXTS_CASTED; ++timerIter)
  {
    Dcm_TmrTimerContextPtrType pTimerContext = Dcm_TmrGetTimerContext(timerIter);

    expiredTimers <<=1; /* next timer */
    /* If a timer is active (not yet expired or stopped), update its counter (decrement by one) */
    if(Dcm_TmrIsTimerRunningContext(timerIter))
    {
      pTimerContext->Timer--;                                                                                                                        /* PRQA S 3387 */ /* MD_Dcm_OptimizedInlineCode_3387 */ /* SBSW_DCM_POINTER_WRITE_TIMERCONTEXT */

      if(pTimerContext->Timer == 0u)
      {
        /* If the concrete timer did expire with the above step, register the timerId for later processing */
        expiredTimers |= 0x01u;
        Dcm_TmrSetTimerStopped(timerIter);
      }
      else
      {
        /* Otherwise, signal reactivation of this task again at next main-function cycle, since at least one timer still active */
        pEventContext->PostEv |= DCM_TSK_EV_TIMER_ACTIVE;                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
      }
    }
  }
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  /* Leave critical section */
  Dcm_UtiLeaveCriticalSection();

  /* Iterate over all expired timers only: */
  DCM_UTI_LOOP_BIT_SCAN(expiredTimers)
  {
    --timerIter; /* pre-decrement since the iterator equals at first DCM_TMR_NUM_TIMERS_CASTED */
    if(Dcm_UtiBitOpTest(Dcm_TmrMaskOptType, expiredTimers, 0x01u))
    {
      Dcm_TmrTimerIdOptType   lTimerInfoId;
      Dcm_TmrTimerInfoPtrType lTimerInfo;

      lTimerInfoId = Dcm_TmrGetTimerContext(timerIter)->TimerInfoRef;
      lTimerInfo   = Dcm_TmrGetTimerInfo(lTimerInfoId);

      {
        /* Invoke the associated with the timer "Dcm_OnTimeout*" handler function */
        Dcm_TmrTimerCntrMemType reloadTicks = lTimerInfo->OnTimeoutFunc((Dcm_ThreadIdMemType)(timerIter - lTimerInfo->TimerContextRef));             /* SBSW_DCM_CALL_FUNCPTR_TIMERINFO */
        if(reloadTicks != 0u)                                                                                                                        /* COV_DCM_RTM_DESIGN_LOWER_LAYER XF */
        {
          /* If the handler requires an immediate reload of the timer, activate the timer immediately */
          Dcm_TmrStartTimerByThread(lTimerInfoId, reloadTicks, (Dcm_ThreadIdMemType)(timerIter - lTimerInfo->TimerContextRef));
        }
      }
    }
  }
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Tmr.c
 *********************************************************************************************************************/
