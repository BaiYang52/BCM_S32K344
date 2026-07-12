/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**
 * \addtogroup Os_Timer
 * \{
 *
 * \file
 * \brief       Implementation of high level timer hardware handling.
 *
 *
 *********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os.h.
 **********************************************************************************************************************/

                                                                                                                        /* PRQA S 0777, 0779, 0828  EOF */ /* MD_MSR_Rule5.1, MD_MSR_Rule5.2, MD_MSR_Dir1.1 */


#define OS_TIMER_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */
#include "Os_Timer.h"

/* Os kernel module dependencies */
#include "Os_Counter_Types.h"
#include "Os_Counter.h"
#include "Os_Isr.h"

/* Os hal dependencies */
#include "Os_Hal_Timer.h"
#include "Os_Hal_Compiler.h"


/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define OS_START_SEC_CODE
#include "Os_MemMap_OsCode.h"                                                                                           /* PRQA S 5087 */ /* MD_MSR_MemMap */



/***********************************************************************************************************************
 *  Os_TimerThread2CounterIsr()
 **********************************************************************************************************************/
/*! \brief          Returns the timer ISR corresponding to a thread.
 *  \details        The caller has to ensure, that the thread actually belongs to a timer ISR.
 *
 *  \param[in]      Thread  Thread which shall be converted. Parameter must not be NULL.
 *
 *  \context        ISR2
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Given Thread is a Counter ISR.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE P2CONST(Os_TimerIsrConfigType, AUTOMATIC, OS_CONST), OS_CODE,
OS_ALWAYS_INLINE, Os_TimerThread2CounterIsr,
(
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) Thread
));


/***********************************************************************************************************************
 *  Os_TimerPitCheckNotPending()
 **********************************************************************************************************************/
/*! \brief          Returns whether the PIT timer interrupt is not pending.
 *  \details        --no details--
 *
 *  \param[in]      PitCounter  Counter, which interrupt is to be checked. Counter must be a PIT counter.
 *                              Parameter must not be NULL.
 *
 *  \retval         !0  The timer interrupt is not pending.
 *  \retval         0   The timer interrupt is pending.
 *
 *  \context        ISR2
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerPitCheckNotPending,
(
  P2CONST(Os_TimerPitConfigType, AUTOMATIC, OS_CONST) PitCounter
));


/***********************************************************************************************************************
 *  Os_TimerPfrtSetNextHardwareCompareValue()
 **********************************************************************************************************************/
/*! \brief          Updates the compare value of a PFRT for the next timer interrupt.
 *  \details        This function modifies the hardware timer of the PFRT.
 *
 *  \param[in,out]  Timer           The timer to modify. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION( OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerPfrtSetNextHardwareCompareValue,
(
  P2CONST(Os_TimerPfrtConfigType, AUTOMATIC, OS_CONST) Timer
));


/***********************************************************************************************************************
 *  Os_TimerPfrtGetDyn()
 **********************************************************************************************************************/
/*! \brief          Returns the dynamic data of a PFRT.
 *  \details        --no details--
 *
 *  \param[in]      Timer        Timer to query. Parameter must not be NULL.
 *
 *  \return         Pointer to the timer's dynamic data.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE P2VAR(Os_TimerPfrtType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE,
OS_ALWAYS_INLINE,
Os_TimerPfrtGetDyn,
(
  P2CONST(Os_TimerPfrtConfigType, AUTOMATIC, OS_CONST) Timer
));


/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  Os_TimerThread2CounterIsr()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE P2CONST(Os_TimerIsrConfigType, AUTOMATIC, OS_CONST), OS_CODE,
OS_ALWAYS_INLINE, Os_TimerThread2CounterIsr,
(
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) Thread
))
{
  return (P2CONST(Os_TimerIsrConfigType, AUTOMATIC, OS_CONST))Thread;                                                   /* PRQA S 0310 */ /* MD_Os_Rule11.3_0310 */
}


/***********************************************************************************************************************
 *  Os_TimerPitCheckNotPending()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* COV_OS_HALPLATFORMPITUNSUPPORTED_XX */
Os_TimerPitCheckNotPending,
(
  P2CONST(Os_TimerPitConfigType, AUTOMATIC, OS_CONST) PitCounter
))                                                                                                                      /* COV_OS_HALPLATFORMPITUNSUPPORTED_XX */
{
  return Os_ErrExtendedCheck((Os_StdReturnType)(Os_Hal_TimerPitIsPending(PitCounter->HwConfig) == 0u));                 /* PRQA S 2995, 4304 */ /* MD_Os_Rule2.2_2995, MD_Os_C90BooleanCompatibility */ /* SBSW_OS_FC_PRECONDITION */
}


/***********************************************************************************************************************
 *  Os_TimerPfrtSetNextHardwareCompareValue()
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
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMPFRTUNSUPPORTED_XX */
Os_TimerPfrtSetNextHardwareCompareValue,
(
  P2CONST(Os_TimerPfrtConfigType, AUTOMATIC, OS_CONST) Timer
))                                                                                                                      /* COV_OS_HALPLATFORMPFRTUNSUPPORTED_XX */
{
  P2VAR(Os_TimerPfrtType, AUTOMATIC, OS_VAR_NOINIT) dyn;
  Os_TickType compare;
  Os_TickType now;

  dyn = Os_TimerPfrtGetDyn(Timer);                                                                                      /* SBSW_OS_FC_PRECONDITION */

  /* #10 Calculate and store the next Compare value. */
  compare = Os_TimerAdd(Timer->MaxCountingValue,
                        dyn->Compare,
                        Timer->Period);
  dyn->Compare = compare;                                                                                               /* SBSW_OS_TIMER_TIMERGETDYN_001 */

  /* #20 Write the compare value to hardware. */
  Os_TimerFrtSetCompareValue(Timer->HwConfig, compare);                                                                 /* SBSW_OS_TIMER_TIMERFRTSETCOMPAREVALUE_001 */

  /* #30 Get the current hardware counter value. */
  now = Os_TimerFrtTickType2TickType(Os_Hal_TimerFrtGetCounterValue(Timer->HwConfig));                                  /* SBSW_OS_TIMER_HAL_TIMERFRTGETCOUNTERVALUE_001 */

  /* #40 If new compare value is already in the past or now: */
  if(OS_UNLIKELY(                                                                                                       /* PRQA S 2991, 2995 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule2.2_2995 */
      Os_CounterIsFutureValueInternal(Timer->MaxCountingValue, Timer->MaxDifferentialValue, compare, now) == 0u))
  {
    /* #50 If the platform supports peripheral interrupt trigger: */
    if (Os_Hal_IntIsPeripheralInterruptTriggerSupported() != 0u)                                                        /* COV_OS_HALPLATFORMPERIPHERALINTTRIGGERSUPPORTED */ /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
    {
      /* #60 Trigger interrupt, so the violation is handled after the thread switch is completed. */
      Os_Hal_TimerFrtTriggerIrq(Timer->HwConfig);                                                                       /* SBSW_OS_TIMER_HAL_TIMERFRTTRIGGERIRQ_001 */
    }
    /* #70 Else: */
    else
    {
      /* #80 Set the compare value into the near future. */
      Os_TimerFrtSetFutureCompareValue(Timer->HwConfig,                                                                 /* SBSW_OS_TIMER_TIMERFRTSETFUTURECOMPAREVALUE_001 */
                                       Timer->MaxCountingValue,
                                       Timer->MaxDifferentialValue);
    }
  }
}


/***********************************************************************************************************************
 *  Os_TimerPfrtGetDyn()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE P2VAR(Os_TimerPfrtType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE,                /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMPFRTUNSUPPORTED_XX */
OS_ALWAYS_INLINE, Os_TimerPfrtGetDyn,
(
  P2CONST(Os_TimerPfrtConfigType, AUTOMATIC, OS_CONST) Timer
))                                                                                                                      /* COV_OS_HALPLATFORMPFRTUNSUPPORTED_XX */
{
  return (P2VAR(Os_TimerPfrtType, AUTOMATIC, OS_VAR_NOINIT))((Timer->SwCounter).Dyn);                                   /* PRQA S 0310, 3305 */ /* MD_Os_Rule11.3_0310, MD_Os_Rule11.3_3305 */
}


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  Os_TimerSwIncrement()
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
 *
 *
 *
 *
 */
FUNC(void, OS_CODE) Os_TimerSwIncrement
(
  P2CONST(Os_TimerSwConfigType, AUTOMATIC, OS_CONST) Timer
)
{
  Os_StdReturnType collectionIsList = Os_CounterCollectionIsList(&(Timer->Counter));                                    /* SBSW_OS_FC_PRECONDITION */ /* PRQA S 2983 */ /* MD_Os_Rule2.2_2983_Config */
  Os_StdReturnType collectionIsQueue = Os_CounterCollectionIsQueue(&(Timer->Counter));                                  /* SBSW_OS_FC_PRECONDITION */ /* PRQA S 2983 */ /* MD_Os_Rule2.2_2983_Config */

  /* #10 Perform assertions. */
  Os_Assert((Os_StdReturnType)(Timer->Dyn->Value <= Timer->Counter.Characteristics.MaxCountingValue));                  /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */

  /* #20 If the counter reached its maximum value, reset the counter to zero. */
  if(OS_UNLIKELY(Timer->Dyn->Value == Timer->Counter.Characteristics.MaxCountingValue))
  {
    Timer->Dyn->Value = 0;                                                                                              /* SBSW_OS_TIMER_TIMERGETDYN_001 */
  }
  /* #30 Otherwise, increment the counter. */
  else
  {
    (Timer->Dyn->Value)++;                                                                                              /* SBSW_OS_TIMER_TIMERGETDYN_001 */
  }

  /* #40 If the collection of the counter is an array list: */
  if ((Os_CounterHasList() != 0u) && (collectionIsList != 0u))                                                          /* PRQA S 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
  {
    /* #50 Work off expired jobs. */
    Os_CounterWorkJobs(&(Timer->Counter));                                                                              /* SBSW_OS_FC_PRECONDITION */
  }
  /* #60 Otherwise, if the collection is a priority queue: */
  else if ((Os_CounterHasQueue() != 0u) && (collectionIsQueue != 0u))                                                   /* PRQA S 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
  {
    /* #70 If the compare value (time stamp of the next job) has been reached: */
    if(OS_UNLIKELY(Timer->Dyn->Value == Timer->Dyn->Compare))
    {
      /* #80 Work off expired jobs. */
      Os_CounterWorkJobs(&(Timer->Counter));                                                                            /* SBSW_OS_FC_PRECONDITION */
    }
  }
  /* #90 Otherwise: */
  else
  {
    /* #100 Enter KernelPanic. */
    Os_ErrKernelPanic();
  }
}


/***********************************************************************************************************************
 *  Os_TimerPfrtStart()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, OS_CODE) Os_TimerPfrtStart                                                                                   /* COV_OS_HALPLATFORMPFRTUNSUPPORTED_XX */
(
  P2CONST(Os_TimerPfrtConfigType, AUTOMATIC, OS_CONST) Timer
)
{
  P2VAR(Os_TimerPfrtType, AUTOMATIC, OS_VAR_NOINIT) dyn;

  if(Os_CounterHasPfrtCounter() == 0u)                                                                                  /* COV_OS_INVSTATE */ /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
  {
    Os_ErrKernelPanic();
  }

  /* #10 Start the software part of the timer. */
  Os_TimerSwStart(&(Timer->SwCounter));                                                                                 /* SBSW_OS_FC_PRECONDITION */

  /* #20 Initialize the FRT hardware. */
  Os_Hal_TimerFrtInit(Timer->HwConfig);                                                                                 /* SBSW_OS_TIMER_HAL_TIMERFRTINIT_001 */

  /* #30 Set the compare value stored in software to now. */
  dyn = Os_TimerPfrtGetDyn(Timer);                                                                                      /* SBSW_OS_FC_PRECONDITION */
  dyn->Compare = (Os_TickType)Os_Hal_TimerFrtGetCounterValue(Timer->HwConfig);                                          /* SBSW_OS_TIMER_TIMERGETDYN_001 */ /* SBSW_OS_TIMER_HAL_TIMERFRTGETCOUNTERVALUE_001 */

  /* #40 Move compare value one period into the future. */
  Os_TimerPfrtSetNextHardwareCompareValue(Timer);                                                                       /* SBSW_OS_FC_PRECONDITION */
}


/***********************************************************************************************************************
 *  ISR(Os_TimerPitIsr)
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
ISR(Os_TimerPitIsr)                                                                                                     /* COV_OS_HALPLATFORMPITUNSUPPORTED_XX */
{
  P2CONST(Os_TimerIsrConfigType, AUTOMATIC, OS_CONST) me;
  P2CONST(Os_TimerPitConfigType, AUTOMATIC, OS_CONST) pitCounter;

  if(Os_CounterHasPitCounter() == 0u)                                                                                   /* COV_OS_INVSTATE */ /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
  {
    Os_ErrKernelPanic();
  }

  me = Os_TimerThread2CounterIsr(Os_CoreGetThread());                                                                   /* SBSW_OS_TIMER_TIMERTHREAD2COUNTERISR_001 */

  if(me->Counter->DriverType != OS_TIMERTYPE_PERIODIC_TICK)                                                             /* COV_OS_INVSTATE */
  {
    Os_ErrKernelPanic();
  }

  pitCounter = (P2CONST(Os_TimerPitConfigType, AUTOMATIC, OS_CONST)) me->Counter;                                       /* PRQA S 0310 */ /* MD_Os_Rule11.3_0310 */

  /* #10 Acknowledge and reload the PIT hardware. */
  Os_Hal_TimerPitAckAndReload(pitCounter->HwConfig);                                                                    /* SBSW_OS_TIMER_HAL_TIMERPITACKANDRELOAD_001 */

  /* #20 Increment software counter. If there are expired jobs, work them off. */
  Os_TimerSwIncrement(&(pitCounter->SwCounter));                                                                        /* SBSW_OS_TIMER_TIMERSWINCREMENT_001 */

  /* #30 (EXTENDED status:) If there is already an interrupt pending (unlikely): */
  if(OS_UNLIKELY(Os_TimerPitCheckNotPending(pitCounter) == OS_CHECK_FAILED))                                            /* SBSW_OS_TIMER_HAL_TIMERPITISPENDING_001 */
  {
    /* #40 Report job overload. */
    Os_ErrReportTimerIsr(OS_STATUS_OVERLOAD);
  }
}


/***********************************************************************************************************************
 *  ISR(Os_TimerHrtIsr)
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
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
ISR(Os_TimerHrtIsr)                                                                                                     /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
{
  P2CONST(Os_TimerIsrConfigType, AUTOMATIC, OS_CONST) me;
  P2CONST(Os_TimerHrtConfigType, AUTOMATIC, OS_CONST) hrtCounter;
  Os_IntStateType interruptState;
  Os_StdReturnType isLogicalCompareValueReached;

  if(Os_CounterHasHrtCounter() == 0u)                                                                                   /* COV_OS_INVSTATE */ /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
  {
    Os_ErrKernelPanic();
  }

  me = Os_TimerThread2CounterIsr(Os_CoreGetThread());                                                                   /* SBSW_OS_TIMER_TIMERTHREAD2COUNTERISR_001 */

  if(me->Counter->DriverType != OS_TIMERTYPE_HIGH_RESOLUTION)                                                           /* COV_OS_INVSTATE */
  {
    Os_ErrKernelPanic();
  }

  hrtCounter = (P2CONST(Os_TimerHrtConfigType, AUTOMATIC, OS_CONST)) me->Counter;                                       /* PRQA S 0310 */ /* MD_Os_Rule11.3_0310 */

  /* #10 Acknowledge the timer hardware, so that new timer interrupts can be triggered. */
  Os_Hal_TimerFrtAcknowledge(hrtCounter->HwConfig);                                                                     /* SBSW_OS_TIMER_HAL_TIMERFRTACKANDRELOAD_001 */

  /* #20 Suspend interrupts. */
  Os_IntSuspend(&interruptState);                                                                                       /* SBSW_OS_FC_POINTER2LOCAL */

  /* #30 Get the value which is needed to check if the requested time span has elapsed (critical). */
  isLogicalCompareValueReached = Os_TimerHrtIsLogicalCompareValueReached(hrtCounter);                                   /* SBSW_OS_TIMER_TIMERISLOGICALCOMPAREVALUEREACHED */

  /* #40 Resume interrupts. */
  Os_IntResume(&interruptState);                                                                                        /* SBSW_OS_FC_POINTER2LOCAL */

  /* #50 If the requested time span has elapsed: */
  if(isLogicalCompareValueReached != 0u)                                                                                /* COV_OS_HALPLATFORMFRT16BITAVAILABILITY_TX */ /* COMP_WARN_OS_CW_005 */ /* PRQA S 2991, 2995 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule2.2_2995 */
  {
    /* #60 Work off expired jobs. */
    Os_CounterWorkJobs(&hrtCounter->Counter);                                                                           /* SBSW_OS_TIMER_COUNTERWORKJOBS_001 */
  /* #70 Otherwise: */
  }else
  {
    /* #80 Update the hardware compare value in accordance to the logical compare value. */
    Os_TimerHrtSetCompareValue(hrtCounter, hrtCounter->Dyn->Compare);                                                   /* SBSW_OS_TIMER_TIMERHRTSETCOMPAREVALUE */
  }
}


/***********************************************************************************************************************
 *  ISR(Os_TimerPfrtIsr)
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ISR(Os_TimerPfrtIsr)                                                                                                    /* COV_OS_HALPLATFORMPFRTUNSUPPORTED_XX */
{
  P2CONST(Os_TimerIsrConfigType, AUTOMATIC, OS_CONST) me;
  P2CONST(Os_TimerPfrtConfigType, AUTOMATIC, OS_CONST) pfrtTimer;

  if(Os_CounterHasPfrtCounter() == 0u)                                                                                  /* COV_OS_INVSTATE */ /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
  {
    Os_ErrKernelPanic();
  }

  me = Os_TimerThread2CounterIsr(Os_CoreGetThread());                                                                   /* SBSW_OS_TIMER_TIMERTHREAD2COUNTERISR_001 */

  if(me->Counter->DriverType != OS_TIMERTYPE_PERIODIC_FREE_RUNNING_TIMER)                                               /* COV_OS_INVSTATE */
  {
    Os_ErrKernelPanic();
  }

  pfrtTimer = (P2CONST(Os_TimerPfrtConfigType, AUTOMATIC, OS_CONST)) me->Counter;                                       /* PRQA S 0310 */ /* MD_Os_Rule11.3_0310 */

  /* #10 Acknowledge the timer hardware, so that new timer interrupts can be triggered. */
  Os_Hal_TimerFrtAcknowledge(pfrtTimer->HwConfig);                                                                      /* SBSW_OS_TIMER_HAL_TIMERFRTACKANDRELOAD_001 */

  /* #20 Work off expired jobs. */
  Os_TimerSwIncrement(&(pfrtTimer->SwCounter));                                                                         /* SBSW_OS_TIMER_TIMERSWINCREMENT_001 */

  /* #30 Set compare value for next timer interrupt. */
  Os_TimerPfrtSetNextHardwareCompareValue(pfrtTimer);                                                                   /* SBSW_OS_FC_PRECONDITION */
}


#define OS_STOP_SEC_CODE
#include "Os_MemMap_OsCode.h"                                                                                           /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* module specific MISRA deviations:
 */

/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_OS_TIMER_TIMERGETDYN_001
 \DESCRIPTION    Write access to the dynamic data of a Timer.  The Timer pointer is passed as argument to the
                 caller function. Precondition ensures that the argument pointer is valid. The compliance of the
                 precondition is checked during review.
 \COUNTERMEASURE \N [CM_OS_TIMERGETDYN_N]

\ID SBSW_OS_TIMER_HAL_TIMERPITINIT_001
 \DESCRIPTION    Os_Hal_TimerPitInit is called with a HwConfig value derived from a pointer passed as argument to the
                 caller function. Precondition ensures that the argument pointer is valid. The compliance of the
                 precondition is checked during review.
 \COUNTERMEASURE \M [CM_OS_TIMERPITHWCONFIG_M]

\ID SBSW_OS_TIMER_HAL_TIMERFRTINIT_001
 \DESCRIPTION    Os_Hal_TimerFrtInit is called with a HwConfig value derived from a pointer passed as argument to the
                 caller function. Precondition ensures that the argument pointer is valid. The compliance of the
                 precondition is checked during review.
 \COUNTERMEASURE \M [CM_OS_TIMERHRTHWCONFIG_M]

\ID SBSW_OS_TIMER_HAL_TIMERFRTTRIGGERIRQ_001
 \DESCRIPTION    Os_Hal_TimerFrtTriggerIrq is called with a HwConfig value derived from a pointer passed as
                 argument to the caller function. Precondition ensures that the argument pointer is valid. The
                 compliance of the precondition is checked during review.
 \COUNTERMEASURE \M [CM_OS_TIMERHRTHWCONFIG_M]

\ID SBSW_OS_TIMER_TIMERFRTSETCOMPAREVALUE_001
 \DESCRIPTION    Os_TimerFrtSetCompareValue is called with a HwConfig value derived from a pointer passed as
                 argument to the caller function. Precondition ensures that the argument pointer is valid. The
                 compliance of the precondition is checked during review.
 \COUNTERMEASURE \M [CM_OS_TIMERHRTHWCONFIG_M]

\ID SBSW_OS_TIMER_TIMERFRTSETFUTURECOMPAREVALUE_001
 \DESCRIPTION    Os_TimerFrtSetFutureCompareValue is called with a HwConfig value derived from a pointer passed as
                 argument to the caller function. Precondition ensures that the argument pointer is valid. The
                 compliance of the precondition is checked during review.
 \COUNTERMEASURE \M [CM_OS_TIMERHRTHWCONFIG_M]

\ID SBSW_OS_TIMER_HAL_TIMERFRTACKNOWLEDGE_001
 \DESCRIPTION    Os_Hal_TimerFrtAcknowledge is called with a HwConfig value derived from a pointer passed as
                 argument to the caller function. Precondition ensures that the argument pointer is valid. The
                 compliance of the precondition is checked during review.
 \COUNTERMEASURE \M [CM_OS_TIMERHRTHWCONFIG_M]

\ID SBSW_OS_TIMER_HAL_TIMERFRTGETCOUNTERVALUE_001
 \DESCRIPTION    Os_Hal_TimerFrtGetCounterValue is called with a HwConfig value derived from a pointer passed as
                 argument to the caller function. Precondition ensures that the argument pointer is valid. The
                 compliance of the precondition is checked during review.
 \COUNTERMEASURE \M [CM_OS_TIMERHRTHWCONFIG_M]

\ID SBSW_OS_TIMER_HAL_TIMERPITACKANDRELOAD_001
 \DESCRIPTION    Os_Hal_TimerPitAckAndReload is called with a HwConfig value derived from the return value of
                 Os_TimerThread2CounterIsr.
 \COUNTERMEASURE \R [CM_OS_TIMERTHREAD2COUNTERISR_R]
                 \M [CM_OS_TIMERPITHWCONFIG_M]

\ID SBSW_OS_TIMER_HAL_TIMERFRTACKANDRELOAD_001
 \DESCRIPTION    Os_Hal_TimerFrtAcknowledge is called with a HwConfig value derived from the return value of
                 Os_TimerThread2CounterIsr.
 \COUNTERMEASURE \R [CM_OS_TIMERTHREAD2COUNTERISR_R]
                 \M [CM_OS_TIMERHRTHWCONFIG_M]

\ID SBSW_OS_TIMER_TIMERSWINCREMENT_001
 \DESCRIPTION    Os_TimerSwIncrement is called with a value derived from the return value of
                 Os_TimerThread2CounterIsr.
 \COUNTERMEASURE \R [CM_OS_TIMERTHREAD2COUNTERISR_R]

\ID SBSW_OS_TIMER_COUNTERWORKJOBS_001
 \DESCRIPTION    Os_CounterWorkJobs is called with a value derived from the return value of
                 Os_TimerThread2CounterIsr.
 \COUNTERMEASURE \R [CM_OS_TIMERTHREAD2COUNTERISR_R]

\ID SBSW_OS_TIMER_HAL_TIMERPITISPENDING_001
 \DESCRIPTION    Os_Hal_TimerPitIsPending is called with a HwConfig value derived from the return value of
                 Os_TimerThread2CounterIsr.
 \COUNTERMEASURE \R [CM_OS_TIMERTHREAD2COUNTERISR_R]
                 \M [CM_OS_TIMERPITHWCONFIG_M]

\ID SBSW_OS_TIMER_TIMERISLOGICALCOMPAREVALUEREACHED
 \DESCRIPTION    Os_TimerHrtIsLogicalCompareValueReached is called with a value derived from the return value of
                 Os_TimerThread2CounterIsr.
 \COUNTERMEASURE \R [CM_OS_TIMERTHREAD2COUNTERISR_R]

\ID SBSW_OS_TIMER_TIMERHRTSETCOMPAREVALUE
 \DESCRIPTION    Os_TimerSetCompareValue is called with a value derived from the return value of
                 Os_TimerThread2CounterIsr.
 \COUNTERMEASURE \R [CM_OS_TIMERTHREAD2COUNTERISR_R]

\ID SBSW_OS_TIMER_TIMERTHREAD2COUNTERISR_001
 \DESCRIPTION    Os_TimerThread2CounterIsr is called with the return value of Os_CoreGetThread.
 \COUNTERMEASURE \N [CM_OS_COREGETTHREAD_N]

SBSW_JUSTIFICATION_END */

/*
\CM CM_OS_TIMERTHREAD2COUNTERISR_R
      Ensured by implementation the the returned value is valid.

\CM CM_OS_TIMERHRTGETCOUNTER_N
      Precondition ensures that the pointer is valid. The compliance of the precondition is checked during review.

\CM CM_OS_TIMERPITHWCONFIG_M
      Verify that the HwConfig reference in each PIT instance is a non NULL_PTR.

\CM CM_OS_TIMERHRTHWCONFIG_M
      Verify that the HwConfig reference in each HRT instance is a non NULL_PTR.

\CM CM_OS_TIMERGETDYN_N
      As each object of an abstract class is always part of a concrete class, the concrete class
      guarantees, that the Dyn pointer is valid.

 */
/*!
 * \}
 */

/**********************************************************************************************************************
 *  END OF FILE: Os_Timer.c
 **********************************************************************************************************************/
