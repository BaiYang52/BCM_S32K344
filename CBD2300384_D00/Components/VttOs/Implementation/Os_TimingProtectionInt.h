/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain wi:wth Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/**
 *  \ingroup      Os_Kernel
 *  \defgroup     Os_TimingProtection Timing Protection
 *  \brief        Timing protection mechanism
 *  \details
 *
 *  \{
 *
 *  \file
 *  \brief
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os.h.
 **********************************************************************************************************************/

#ifndef OS_TIMINGPROTECTIONINT_H
# define OS_TIMINGPROTECTIONINT_H
                                                                                                                         /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */
/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_TimingProtection_Cfg.h"
# include "Os_TimingProtection_Types.h"

/* Os module dependencies */
# include "Os_CommonInt.h"
# include "Os_ErrorInt.h"
# include "Os_Cfg.h"
# include "Os_CounterInt.h"
# include "Os_TimerInt.h"

/* Os Hal dependencies */
# include "Os_Hal_CompilerInt.h"
# include "Os_Hal_TimerInt.h"



/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/*! Type cast from Os_TpOwnerThreadConfigType to Os_TpThreadConfigType by use of base element addressing. */
#define OS_TP_CASTCONFIG_TPOWNERTHREAD_2_TPTHREAD(tpOwnerThread)  (&((tpOwnerThread).Base))

/*! Type cast from Os_TpThreadConfigType to Os_TpThreadConfigType* (for symmetry reasons and to ease generation. */
#define OS_TP_CASTCONFIG_TPTHREAD_2_TPTHREAD(tpThread)            (&((tpThread)))

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
/*! Data type for different thread related timing protection configurations types. */
typedef enum
{
  OS_TPTHREADKIND_OWNER,        /*!< The thread is monitored and has own budgets. */
  OS_TPTHREADKIND_HEIR,         /*!< The thread is monitored and uses the budgets of its host. */
  OS_TPTHREADKIND_TPISR         /*!< The thread is the timing protection interrupt. */
}Os_TpMonitoringKindType;


typedef uint8 Os_TpDelayLevelType;

/*! Dynamic information of a timing protection budget. */
struct Os_TpBudgetType_Tag
{
  /*! Remaining time until budget is exhausted. */
  Os_TickType Remaining;

  /*! Previous budget. */
  P2CONST(Os_TpBudgetConfigType, TYPEDEF, OS_CONST) Previous;
};


/*! Configured information about a timing protection budget. */
struct Os_TpBudgetConfigType_Tag
{
  /*! Dynamic data of a timing protection budget. */
  P2VAR(Os_TpBudgetType, TYPEDEF, OS_VAR_NOINIT) Dyn;

  /*! Specifies the type of this execution budget. */
  Os_TpMonitoringType BudgetKind;

  /*! Initial budget. */
  Os_TickType Budget;
};


/*! Dynamic timing protection data. */
struct Os_TpOwnerThreadType_Tag
{
  /*! Time stamp of the next arrival allowed. */
  Os_InterArrivalTimeType EarliestArrival;

  /*! The number of ongoing delay requests. */
  Os_TpDelayLevelType DelayLevel;

  /*! Stack of currently monitored budgets. Sorted by earliest deadline first.
   * Budgets which have a later deadline than budgets that are already monitored, are not monitored.
   * We can do this, because locks have to be acquired and released in a stack based manner. */
  P2CONST(Os_TpBudgetConfigType, TYPEDEF, OS_CONST) MonitoredBudgets;

  /*! Indicates whether at least one budget has already been expired.
   * (Info needed for delayed timing protection reporting). */
  Os_TpMonitoringType Violation;
};


/*! Configured Timing protection settings. */
struct Os_TpThreadConfigType_Tag
{
  /*! Defines the kind of monitoring that is applied to the thread. */
  Os_TpMonitoringKindType ThreadKind;
};


/*! Configuration information of a monitored thread.
 * \details
 * \extends Os_TpThreadConfigType_Tag
 */
struct Os_TpOwnerThreadConfigType_Tag
{
  /*! The thread attributes of this task.
   * This attribute must come first! */
  Os_TpThreadConfigType Base;

  /*! Minimum allowed time between successive activations. */
  Os_InterArrivalTimeType TimeFrame;

  /*! Execution budget. */
  Os_TpBudgetConfigType ExecutionBudget;

  /*! All interrupt lock time budget. */
  Os_TpBudgetConfigType AllInterruptLockBudget;

  /*! OS interrupt lock time budget. */
  Os_TpBudgetConfigType OsInterruptLockBudget;

  /*! Resource lock time budgets. */
  P2CONST(Os_TpBudgetConfigRefType, TYPEDEF, OS_CONST) ResourceLockBudgets;

  /*! Dynamic data of the timing protection settings.
   * \details As there is no Dyn object within the base class, the Dyn pointer is located here. */
  P2VAR(Os_TpOwnerThreadType, TYPEDEF, OS_VAR_NOINIT) Dyn;
};


/*! Dynamic data of timing protection object. */
struct Os_TpType_Tag
{
  /*! A snapshot of the range extended timing protection counter value.
   * Needed to check inter arrival time. */
  Os_InterArrivalTimeType Snapshot;

  /*! A snapshot of the underlying counter value. */
  Os_TickType LastCounterValue;

  /*! The currently monitored thread. */
  P2CONST(Os_ThreadConfigType, TYPEDEF, OS_CONST) MonitoredThread;
};


/*! Data type for timing protection timer configuration. */
struct Os_TpConfigType_Tag
{
  /*! Pointer to timer hardware configuration. */
  P2CONST(Os_TimerHrtConfigType, TYPEDEF, OS_CONST) Timer;

  /*! Dynamic data of timing protection object. */
  P2VAR(Os_TpType, TYPEDEF, OS_VAR_NOINIT) Dyn;
};



/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/***********************************************************************************************************************
 *  Os_TpThread2TpOwnerThread()
 **********************************************************************************************************************/
/*! \brief          Returns the monitored thread of a timing protected thread.
 *  \details        --no details--
 *
 *  \param[in]      TpThread    The TP thread to query. Parameter must not be NULL.
 *
 *  \return         Pointer to a monitored thread.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            TpThread is a TpOwnerThread.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
P2CONST(Os_TpOwnerThreadConfigType, AUTOMATIC, OS_CONST), OS_CODE, OS_ALWAYS_INLINE, Os_TpThread2TpOwnerThread,
(
  P2CONST(Os_TpThreadConfigType, AUTOMATIC, OS_CONST) TpThread
));


/***********************************************************************************************************************
 *  Os_TpGetCurrentTpOwnerThread()
 **********************************************************************************************************************/
/*! \brief          Returns the configuration of the currently monitored thread.
 *  \details        --no details--
 *
 *  \param[in]      Config    The timing protection configuration to query. Parameter must not be NULL.
 *
 *  \return         Pointer to a monitored thread.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            OwnerThread is set.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE P2CONST(Os_TpOwnerThreadConfigType, AUTOMATIC, OS_CONST), OS_CODE,
OS_ALWAYS_INLINE, Os_TpGetCurrentTpOwnerThread,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config
));


/***********************************************************************************************************************
 *  Os_TpThreadInit()
 **********************************************************************************************************************/
/*! \brief        Initialize given timing protection settings.
 *  \details      Reset thread budgets to configured ones. The function is called in Init-Step3 for the idle task and
 *                for all threads in Init-Step4.
 *
 *  \param[in,out]  Config   The Timing protection configuration.
 *                           Parameter may be NULL if timing protection is disabled for the owner core.
 *  \param[in,out]  Current  Timing protection budgets.
 *                           Parameter must not be NULL if parameter Config is non-NULL.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access to given object is prevented by caller.
 *  \pre          The timing protection of the owner core is initialized.
 *
 *  \trace        CREQ-49, CREQ-845
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TpThreadInit,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config,
  P2CONST(Os_TpThreadConfigType, AUTOMATIC, OS_CONST) Current
));


/***********************************************************************************************************************
 *  Os_TpThreadKill()
 **********************************************************************************************************************/
/*! \brief        Reset timing protection settings.
 *  \details      Cancel all budget monitoring and reset budgets to configured values.
 *                The EarliestArrival is not modified.
 *
 *  \param[in,out]  Thread  Timing protection budgets.
 *                          Parameter must not be NULL if timing protection is enabled on the thread's owner core.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access to given object is prevented by caller.
 *  \pre          The timing protection of the owner core is initialized.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TpThreadKill,
(
  P2CONST(Os_TpThreadConfigType, AUTOMATIC, OS_CONST) Thread
));


/***********************************************************************************************************************
 *  Os_TpReducedInit()
 **********************************************************************************************************************/
/*! \brief        Performs reduced initialization of timing protection module.
 *  \details      This function enables to call the timing protection API. API calls have no effect before calling
 *                \ref Os_TpInit().
 *                This function is called in Init-Step2.
 *
 *  \param[in,out]  Config          The timing protection configuration of the current core.
 *                                  Parameter may be NULL if timing protection is disabled for the owner core.
 *  \param[in,out]  InitialThread   The initial monitored thread.
 *                                  Parameter must not be NULL if parameter Config is non-NULL.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_TpReducedInit
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config,
  P2CONST(Os_ThreadConfigType, TYPEDEF, OS_CONST) InitialThread
);


/***********************************************************************************************************************
 *  Os_TpInit()
 **********************************************************************************************************************/
/*! \brief        Initialize timing protection module and hardware timer.
 *  \details      This function is called in Init-Step3 and Init-Step4 in order to handle the situation that
 *                Init-Step3 is optional.
 *
 *  \param[in,out]  Config          The timing protection configuration of the current core.
 *                                  Parameter may be NULL if timing protection is disabled for the owner core.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_TpInit
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config
);


/***********************************************************************************************************************
 *  Os_TpStart()
 **********************************************************************************************************************/
/*! \brief        Start the timing protection hardware timer.
 *  \details      --no details--
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_TpStart(void);


/***********************************************************************************************************************
 *  Os_TpResume()
 **********************************************************************************************************************/
/*! \brief        Let the timing protection interrupt occur in RemainingTime ticks.
 *  \details        --no details--
 *
 *  \param[in,out]  Config            The timing protection configuration of the current core.
 *                                    Parameter must not be NULL.
 *  \param[in]      Now               The current point in time.
 *  \param[in]      RemainingTime     The time of the next expiration relative to now. RemainingTime must be <=
 *                                    Config->Timer->Counter.CounterMaxAllowedValue.MaxAllowedValue.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TpResume,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config,
  Os_TickType Now,
  Os_TickType RemainingTime
));


/***********************************************************************************************************************
 *  Os_TpSuspend()
 **********************************************************************************************************************/
/*! \brief        Update the remaining time of the current thread.
 *  \details        --no details--
 *
 *  \param[in,out]  Config            The timing protection configuration of the current core.
 *                                    Parameter must not be NULL.
 *  \param[in,out]  Current           Budgets of current thread.
 *                                    Parameter must not be NULL.
 *  \param[in]      Now               The current point in time.
 *  \param[in]      Deadline          The absolute deadline of the current budget.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TpSuspend,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config,
  P2CONST(Os_TpOwnerThreadConfigType, AUTOMATIC, OS_CONST) Current,
  Os_TickType Now,
  Os_TickType Deadline
));


/***********************************************************************************************************************
 *  Os_TpSub()
 **********************************************************************************************************************/
/*! \brief        Calculate the difference between Value and Sub.
 *  \details        --no details--
 *
 *  \param[in,out]  Config            The timing protection configuration of the current core.
 *                                    Parameter must not be NULL.
 *  \param[in]      Value             A timer value.
 *                                    Value has to be in range 0 <= Value <= MaxCountingValue.
 *  \param[in]      Sub               The counter value to be subtracted.
 *                                    Value has to be in range 0 <= Sub   <= MaxAllowedValue.
 *
 *  \return       The difference between Value and Sub.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TpSub,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config,
  Os_TickType Value,
  Os_TickType Sub
));


/***********************************************************************************************************************
 *  Os_TpAdd()
 **********************************************************************************************************************/
/*! \brief        Calculate the sum of Value and Add.
 *  \details        --no details--
 *
 *  \param[in,out]  Config            The timing protection configuration of the current core.
 *                                    Parameter must not be NULL.
 *  \param[in]      Value             A timer value.
 *                                    Value has to be in range 0 <= Value <= MaxCountingValue.
 *  \param[in]      Add               The counter value to be added.
 *                                    Value has to be in range 0 <= Add   <= MaxAllowedValue.
 *
 *  \return       The sum of Value and Add.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TpAdd,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config,
  Os_TickType Value,
  Os_TickType Add
));


/***********************************************************************************************************************
 *  Os_TpGetArrivalTimeStamp()
 **********************************************************************************************************************/
/*! \brief        Returns the current time of the counter for inter-arrival time measurement.
 *  \details        --no details--
 *
 *  \param[in,out]  Config            The timing protection configuration of the current core.
 *                                    Parameter must not be NULL.
 *
 *  \return       Current The current time of the counter for inter-arrival time measurement.
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access to given object is prevented by caller.
 *  \pre          Function has to be called at least one time in a hardware timer cycle.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_InterArrivalTimeType, OS_CODE,
OS_ALWAYS_INLINE, Os_TpGetArrivalTimeStamp,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config
));


/***********************************************************************************************************************
 *  Os_TpSwitch()
 **********************************************************************************************************************/
/*! \brief        Switch timing protection budgets.
 *  \details      Restore monitoring budgets of next thread.
 *                Preempt remaining time of currently monitored budget and store it into timing protection settings
 *                of current thread.
 *
 *  \param[in,out]  Config      The timing protection configuration of the current core.
 *                              Parameter must not be NULL if timing protection is enabled on the current core.
 *  \param[in,out]  Current     Budgets of current thread.
 *                              Parameter must not be NULL if timing protection is enabled on the current core.
 *  \param[in,out]  Next        Budgets of next thread.
 *                              Parameter must not be NULL if timing protection is enabled on the current core.
 *  \param[in]      NextThread  The next thread to monitor.
 *                              Parameter must not be NULL if timing protection is enabled on the current core.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access to given object is prevented by caller.
 *  \pre          Given Current thread is an owner thread, if Next is an owner thread.
 *
 *  \trace        CREQ-49, CREQ-845
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                          /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TpSwitch,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config,
  P2CONST(Os_TpThreadConfigType, AUTOMATIC, OS_CONST) Current,
  P2CONST(Os_TpThreadConfigType, AUTOMATIC, OS_CONST) Next,
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) NextThread
));


/***********************************************************************************************************************
 *  Os_TpResetAndResume()
 **********************************************************************************************************************/
/*! \brief        Reset timing protection budgets and resume given budgets.
 *  \details      Stops and resets the current execution budget. So when the budget is resumed the next time,
 *                the full time is available again. Resumes the execution and lock budget. Reprogram hardware timer.
 *
 *  \param[in,out]  Config      The timing protection configuration of the current core.
 *                              Parameter must not be NULL if timing protection is enabled on the current core.
 *  \param[in,out]  Current     Budgets of current thread.
 *                              Parameter must not be NULL if timing protection is enabled on the current core.
 *  \param[in,out]  Next        Budgets of next thread.
 *                              Parameter must not be NULL if timing protection is enabled on the current core.
 *  \param[in]      NextThread  The next thread to monitor.
 *                              Parameter must not be NULL if timing protection is enabled on the current core.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access to given object is prevented by caller.
 *  \pre          Next and NextThread belong together.
 *
 *  \trace        CREQ-49, CREQ-845
 *  \trace        SPEC-64030, SPEC-64061
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TpResetAndResume,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config,
  P2CONST(Os_TpThreadConfigType, AUTOMATIC, OS_CONST) Current,
  P2CONST(Os_TpThreadConfigType, AUTOMATIC, OS_CONST) Next,
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) NextThread
));


/***********************************************************************************************************************
 *  Os_TpResetAfterWait()
 **********************************************************************************************************************/
/*! \brief        Reset timing protection budgets and resume given budgets after leaving the wait state.
 *  \details      Stops and resets the current execution budget after a task leaves the WAITING state.
 *                Reprograms the hardware timer. see /ref Os_TaskWait().
 *
 *  \param[in,out]  Config      The timing protection configuration of the current core.
 *                              Parameter must not be NULL if timing protection is enabled on the current core.
 *  \param[in,out]  Current     Budgets of current thread.
 *                              Parameter must not be NULL if timing protection is enabled on the current core.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access to given object is prevented by caller.
 *
 *  \trace        CREQ-49, CREQ-845
 *  \trace        SPEC-64030
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TpResetAfterWait,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config,
  P2CONST(Os_TpThreadConfigType, AUTOMATIC, OS_CONST) Current
));


/***********************************************************************************************************************
 *  Os_TpCheckInterArrival()
 **********************************************************************************************************************/
/*! \brief        Check inter-arrival time, if violation detected report protection violation.
 *  \details      If an inter-arrival time violation is detected (OS_CHECK_FAILED), the protection hook is called.
 *                If there is no inter-arrival time violation (Not OS_CHECK_FAILED), the arrival time is recorded for
 *                the next time.
 *
 *  \param[in,out]  Config      The timing protection configuration of the current core.
 *                              Parameter must not be NULL if timing protection is enabled on the current core.
 *  \param[in,out]  TpThread    The timing protection configuration to check.
 *                              Parameter must not be NULL if timing protection is enabled on the current core.
 *
 *  \retval       OS_CHECK_FAILED      Inter-arrival time violation.
 *  \retval       Not OS_CHECK_FAILED  Arrival allowed.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access to given object is prevented by caller.
 *
 *  \trace        CREQ-85, CREQ-846
 *  \trace        SPEC-63968, SPEC-64009, SPEC-63848, SPEC-63775, SPEC-63851, SPEC-64022, SPEC-63997
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_TpCheckInterArrival,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config,
  P2CONST(Os_TpThreadConfigType, AUTOMATIC, OS_CONST) TpThread
));


/***********************************************************************************************************************
 *  Os_TpUpdateViolation()
 **********************************************************************************************************************/
/*! \brief        Sets the current violation to the given violation, if there has been no budget expiration before.
 *  \details        --no details--
 *
 *  \param[in,out]  Current     Budgets of the current thread. Parameter must not be NULL.
 *  \param[in]      Violation   The violation.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TpUpdateViolation,
(
  P2CONST(Os_TpOwnerThreadConfigType, AUTOMATIC, OS_CONST) Current,
  Os_TpMonitoringType Violation
));


/***********************************************************************************************************************
 *  Os_TpStartResource()
 **********************************************************************************************************************/
/*! \brief        Start locking time monitoring for given resource.
 *  \see          Os_TpBudgetStart()
 *  \details        --no details--
 *
 *  \param[in,out]  Config    The timing protection configuration of the current core.
 *                            Parameter must not be NULL if timing protection is enabled on the current core.
 *  \param[in]      Resource  Resource ID the locking time monitoring shall be started for.
 *                            Resource must be < OS_RESOURCEID_COUNT.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access to given object is prevented by caller.
 *
 *  \trace        CREQ-847, CREQ-31
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TpStartResource,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config,
  ResourceType Resource
));


/***********************************************************************************************************************
 *  Os_TpGetResourceBudget()
 **********************************************************************************************************************/
/*! \brief        Return the lock budget of the given resource corresponding to the given thread.
 *  \details        --no details--
 *
 *  \param[in,out]  Current   Timing protection settings which shall be used. Parameter must not be NULL
 *  \param[in]      Resource  The ID of the resource. Resource must be < OS_RESOURCEID_COUNT.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 *
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE P2CONST(Os_TpBudgetConfigType, TYPEDEF, OS_CONST), OS_CODE,
OS_ALWAYS_INLINE, Os_TpGetResourceBudget,
(
  P2CONST(Os_TpOwnerThreadConfigType, AUTOMATIC, OS_CONST) Current,
  ResourceType Resource
));


/***********************************************************************************************************************
 *  Os_TpStopResource()
 **********************************************************************************************************************/
/*! \brief        Stop locking time monitoring for last occupied resource.
 *  \details        --no details--
 *  \see          Os_TpBudgetStop()
 *
 *  \param[in,out]  Config    The timing protection configuration of the current core.
 *                            Parameter must not be NULL if timing protection is enabled on the current core.
 *  \param[in,out]  TpThread  The TP thread, which releases the resource.
 *                            Parameter must not be NULL if timing protection is enabled on the current core.
 *  \param[in]      Resource  Resource ID the locking time monitoring shall be stopped for.
 *                            Resource must be < OS_RESOURCEID_COUNT.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access to given object is prevented by caller.
 *  \pre          The given TpThread is an owner thread.
 *
 *  \trace        CREQ-847, CREQ-31
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TpStopResource,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config,
  P2CONST(Os_TpThreadConfigType, AUTOMATIC, OS_CONST) TpThread,
  ResourceType Resource
));


/***********************************************************************************************************************
 *  Os_TpStartDelay()
 **********************************************************************************************************************/
/*! \brief        Tell the timing protection to delay timing protection reporting until call of Os_TpStopDelay().
 *  \details      Delay requests can be done multiple times.
 *
 *  \param[in,out]  Config   The timing protection of the current core.
 *                           Parameter must not be NULL if timing protection is enabled on the current core.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access to given object is prevented by caller.
 *
 *  \trace        SPEC-64022, SPEC-63997
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TpStartDelay,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config
));


/***********************************************************************************************************************
 *  Os_TpStopDelay()
 **********************************************************************************************************************/
/*! \brief        Tell the timing protection that a delay request is done.
 *  \details        --no details--
 *
 *  \param[in,out]  Config   The timing protection of the current core.
 *                           Parameter must not be NULL if timing protection is enabled on the current core.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access to given object is prevented by caller.
 *  \pre          \ref Os_TpStartDelay() was at least called once more than \ref Os_TpStopDelay().
 *
 *  \trace        SPEC-64022, SPEC-63997
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TpStopDelay,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config
));


/***********************************************************************************************************************
 *  Os_TpStartAllInterrupt()
 **********************************************************************************************************************/
/*! \brief        Start locking time monitoring for "all-interrupt" lock.
 *  \details      Same behavior as in Os_TpLockingStartResource but for "all-interrupt" lock.
 *
 *  \param[in,out]  Config     The timing protection configuration of the current core.
 *                             Parameter must not be NULL if timing protection is enabled on the current core.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          -
 *
 *  \trace        CREQ-847, CREQ-31
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TpStartAllInterrupt,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config
));


/***********************************************************************************************************************
 *  Os_TpStopAllInterrupt()
 **********************************************************************************************************************/
/*! \brief        Stop locking time monitoring for "all-interrupt" lock.
 *  \details      Same behavior as in Os_TpLockingStopResource but for "all-interrupt" lock.
 *
 *  \param[in,out]  Config     The timing protection configuration of the current core.
 *                             Parameter must not be NULL if timing protection is enabled on the current core.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          -
 *
 *  \trace        CREQ-847, CREQ-31
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TpStopAllInterrupt,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config
));


/***********************************************************************************************************************
 *  Os_TpStartOsInterrupt()
 **********************************************************************************************************************/
/*! \brief        Start locking time monitoring for "OS-interrupt" lock.
 *  \details      Same behavior as in Os_TpLockingStartResource but for "OS-interrupt" lock.
 *
 *  \param[in,out]  Config     The timing protection configuration of the current core.
 *                             Parameter must not be NULL if timing protection is enabled on the current core.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          -
 *
 *  \trace        CREQ-847, CREQ-31
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TpStartOsInterrupt,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config
));


/***********************************************************************************************************************
 *  Os_TpStopOsInterrupt()
 **********************************************************************************************************************/
/*! \brief        Stop locking time monitoring for "OS-interrupt" lock.
 *  \details      Same behavior as in Os_TpLockingStopResource but for "OS-interrupt" lock.
 *
 *  \param[in,out]  Config     The timing protection configuration of the current core.
 *                             Parameter must not be NULL if timing protection is enabled on the current core.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          -
 *
 *  \trace        CREQ-847, CREQ-31
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TpStopOsInterrupt,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config
));


/***********************************************************************************************************************
 *  Os_TpBudgetStart()
 **********************************************************************************************************************/
/*! \brief        Start monitoring given budget.
 *  \details      If there has been further monitoring on going before, the remaining budgets are compared
 *                and the smaller one is used to program the hardware timer.
 *
 *  \param[in,out]  Config     The timing protection configuration of the current core.
 *                             Parameter must not be NULL if timing protection is enabled on the current core.
 *  \param[in,out]  Current    Timing protection settings which shall be used.
 *                             Parameter must not be NULL if timing protection is enabled on the current core.
 *  \param[in,out]  Budget     Budget to monitor.
 *                             Parameter must not be NULL if timing protection is enabled on the current core.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Current is the budget owner.
 *  \pre          Budget is not of kind OS_TPMONITORINGTYPE_NONE.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TpBudgetStart,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config,
  P2CONST(Os_TpOwnerThreadConfigType, AUTOMATIC, OS_CONST) Current,
  P2CONST(Os_TpBudgetConfigType, AUTOMATIC, OS_CONST) Budget
));


/***********************************************************************************************************************
 *  Os_TpBudgetStop()
 **********************************************************************************************************************/
/*! \brief        Stop monitoring given budget.
 *  \details      If there has been further monitoring on going before, the previous monitoring is continued.
 *
 *  \param[in,out]  Config      The timing protection configuration of the current core.
 *                              Parameter must not be NULL if timing protection is enabled on the current core.
 *  \param[in,out]  Current     Timing protection settings which shall be used.
 *                              Parameter must not be NULL if timing protection is enabled on the current core.
 *  \param[in,out]  Budget      Budget to monitor.
 *                              Parameter must not be NULL if timing protection is enabled on the current core.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE for different objects.
 *  \synchronous  TRUE
 *
 *  \pre          Current is the budget owner.
 *  \pre          The given Budget is either not in the list or the Budget is on top of the list of monitored budgets.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TpBudgetStop,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config,
  P2CONST(Os_TpOwnerThreadConfigType, AUTOMATIC, OS_CONST) Current,
  P2CONST(Os_TpBudgetConfigType, AUTOMATIC, OS_CONST) Budget
));


/***********************************************************************************************************************
 *  Os_TpGetGlobalConfigValue()
 **********************************************************************************************************************/
/*! \brief        Returns the value of OS_CFG_TIMING_PROTECTION
 *  \details      -
 *
 *  \retval       OS_CFG_TP_NO_CORE    0 No timing protection configured on any ASR core.
 *  \retval       OS_CFG_TP_SOME_CORES 1 Timing protection configured on some ASR cores.
 *  \retval       OS_CFG_TP_ALL_CORES  2 Timing protection configured for all ASR cores.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE uint32, OS_CODE, OS_ALWAYS_INLINE, Os_TpGetGlobalConfigValue, (void));


/***********************************************************************************************************************
 *  Os_TpIsEnabled()
 **********************************************************************************************************************/
/*! \brief        Returns whether Timing Protection is enabled (!0) or not (0).
 *  \details      Timing protection is considered to be enabled if there is any task or ISR with timing protection on
 *                any core.
 *
 *  \retval       !0  Timing Protection enabled.
 *  \retval       0   Timing Protection disabled.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TpIsEnabled, (void));


/***********************************************************************************************************************
 *  Os_TpIsEnabledByCoreTpConfig()
 **********************************************************************************************************************/
/*! \brief          Returns whether Timing Protection is enabled (!0) or not (0) for the respective core.
 *  \details        Timing protection is considered to be enabled if there is any task or ISR with timing protection
 *                  configured and the given TP configuration pointer is non-NULL (there is a task or ISR on the given
 *                  core with timing protection configured).
 *
 *  \param[in]      Config      The timing protection configuration of the current core.
 *                              Parameter shall be NULL if no timing protection is configured for the current core.
 *
 *  \retval         !0  Timing Protection enabled.
 *  \retval         0   Timing Protection disabled.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TpIsEnabledByCoreTpConfig,
(
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) Config
));

/***********************************************************************************************************************
 *  Os_TpIsEnabledByThreadTpConfig()
 **********************************************************************************************************************/
/*! \brief          Returns whether Timing Protection is enabled (!0) or not (0) for the respective core.
 *  \details        Timing protection is considered to be enabled if there is any task or ISR with timing protection
 *                  configured and the given TP configuration pointer is non-NULL (there is a task or ISR on the given
 *                  core with timing protection configured).
 *
 *  \param[in]      Config      The timing protection configuration of the current thread.
 *                              Parameter shall be NULL if no timing protection is configured for the current core.
 *
 *  \retval         !0  Timing Protection enabled.
 *  \retval         0   Timing Protection disabled.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_TpIsEnabledByThreadTpConfig,
(
  P2CONST(Os_TpThreadConfigType, AUTOMATIC, OS_CONST) Config
));


/***********************************************************************************************************************
 *  ISR(Os_TpIsr)
 **********************************************************************************************************************/
/*! \brief        Timing protection ISR which handles timing protection violations.
 *  \details      This function handles execution time violations and locking time violations.
 *
 *  \context      ISR2|OS_INTERNAL
 *
 *  \reentrant    TRUE, for different timers.
 *  \synchronous  TRUE
 *
 *  \pre          Timer belongs to the local core.
 *
 *  \trace        CREQ-31, CREQ-845
 *  \trace        SPEC-63906, SPEC-63806, SPEC-63990, SPEC-63867, SPEC-64022, SPEC-63997
 **********************************************************************************************************************/
ISR(Os_TpIsr);



/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* OS_TIMINGPROTECTIONINT_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_TimingProtectionInt.h
 **********************************************************************************************************************/
