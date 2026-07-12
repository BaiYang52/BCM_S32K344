/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/**
 * \addtogroup Os_Trace
 *
 * \{
 *
 * \file
 * \brief       The tracing inline implementation.
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os.h.
 **********************************************************************************************************************/

#ifndef OS_TRACE_H
# define OS_TRACE_H
                                                                                                                        /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */
/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_TraceInt.h"
# include "Os_Trace_Cfg.h"


/* Os kernel module dependencies */
# include "Os_Cfg.h"
# include "Os_Task.h"
# include "Os_Common.h"
# include "Os_Core.h"

/* Os hal dependencies */
# include "Os_Hal_Compiler.h"

# if(OS_CFG_ORTI == STD_ON)
#   include "Os_Orti.h"
# endif


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


#if (OS_IS_DEFINED_OS_VTH_SCHEDULE == STD_OFF)
/***********************************************************************************************************************
 *  OS_VTH_SCHEDULE()
 **********************************************************************************************************************/
/*! \brief          Trace thread-switches, where thread stands for task or ISR
 *  \details        This hook routine allows external tools to trace all context switches from task to ISR and back as
 *                  well as between tasks and between ISRs. So external tools may visualize the information or measure
 *                  the execution time of tasks and ISRs.
 *                  Mind that measured execution time values may not reflect the worst case, which would be necessary
 *                  for schedulability analysis.
 *
 *                  Callers:
 *                    - Os_TraceThreadSwitch()
 *                    - Os_TraceThreadResetAndResume()
 *                    - Os_TraceThreadCleanupAndResume()
 *                    - Os_TraceThreadSuspendAndStart()
 *
 *  \param[in]      FromThreadId     The ID of the thread which has run until the switch
 *  \param[in]      FromThreadReason Represents the reason why the thread is no longer running:
 *                                   - \ref OS_VTHP_TASK_TERMINATION
 *                                   - \ref OS_VTHP_ISR_END
 *                                   - \ref OS_VTHP_TASK_WAITEVENT
 *                                   - \ref OS_VTHP_TASK_WAITSEMA
 *                                   - \ref OS_VTHP_THREAD_PREEMPT
 *  \param[in]      ToThreadId       The ID of the thread which will run from now on
 *  \param[in]      ToThreadReason   Represents the reason why the thread runs from now on:
 *                                   - \ref OS_VTHP_TASK_ACTIVATION
 *                                   - \ref OS_VTHP_ISR_START
 *                                   - \ref OS_VTHP_TASK_SETEVENT
 *                                   - \ref OS_VTHP_TASK_GOTSEMA
 *                                   - \ref OS_VTHP_THREAD_RESUME
 *                                   - \ref OS_VTHP_THREAD_CLEANUP
 *  \param[in]      CallerCoreId     The ID of the core where the thread switch occurs
 *
 *  \context        OS internal
 *
 *  \reentrant      TRUE for different cores.
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 *
 **********************************************************************************************************************/
# define OS_VTH_SCHEDULE(FromThreadId,FromThreadReason,ToThreadId,ToThreadReason,CallerCoreId) \
   OS_IGNORE_UNREF_PARAM(FromThreadId)     ;            \
   OS_IGNORE_UNREF_PARAM(FromThreadReason) ;            \
   OS_IGNORE_UNREF_PARAM(ToThreadId)       ;            \
   OS_IGNORE_UNREF_PARAM(ToThreadReason)   ;            \
   OS_IGNORE_UNREF_PARAM(CallerCoreId)     ;
#endif

#if (OS_IS_DEFINED_OS_VTH_FORCED_TERMINATION == STD_OFF)
/***********************************************************************************************************************
 *  OS_VTH_FORCED_TERMINATION()
 **********************************************************************************************************************/
/*! \brief          Trace the forcible termination of a thread (task or ISR)
 *  \details        This hook routine allows external tools to trace the forcible termination of tasks and ISRs.
 *                  Forcible termination may take place as a reaction on a detected protection error or as result of
 *                  a call to the API function TerminateApplication.
 *                  Mind that forcibly terminated threads change their internal state without further notice. However,
 *                  in some situations a return to the thread is necessary for OS internal cleanup. In this case the
 *                  OS_VTH_SCHEDULE macro is called with ToThreadReason OS_VTHP_THREAD_CLEANUP. That cleanup will
 *                  not return to the application code of the thread but only be operating system internal.
 *
 *                  Callers:
 *                    - Os_TraceIsrKill()
 *                    - Os_TraceTaskKill()
 *
 *  \param[in]      ThreadId      The ID of the thread which is forcibly terminated
 *  \param[in]      CallerCoreId  The ID of the core where the forcible termination occurs
 *
 *  \context        OS internal
 *
 *  \reentrant      TRUE for different cores.
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 **********************************************************************************************************************/
# define OS_VTH_FORCED_TERMINATION(ThreadId,CallerCoreId) \
   OS_IGNORE_UNREF_PARAM(ThreadId)    ;            \
   OS_IGNORE_UNREF_PARAM(CallerCoreId);
#endif

#if (OS_IS_DEFINED_OS_VTH_ACTIVATION == STD_OFF)
/***********************************************************************************************************************
 *  OS_VTH_ACTIVATION()
 **********************************************************************************************************************/
/*! \brief          Trace the activation of a task.
 *  \details        This hook is called on the caller core when that core has successfully performed the activation of
 *                  TaskId on the destination core. As this OS implementation always performs task activation on the
 *                  destination core, DestCoreId and CallerCoreId are always identical.
 *
 *                  Callers:
 *                    - Os_TraceTaskActivate()
 *
 *  \param[in]      TaskId       The ID of the task which is activated
 *  \param[in]      DestCoreId   The ID of the core where the task will be executed
 *  \param[in]      CallerCoreId The ID of the core where this hook is called
 *
 *  \context        OS internal
 *
 *  \reentrant      TRUE for different caller cores.
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 **********************************************************************************************************************/
# define OS_VTH_ACTIVATION(TaskId, DestCoreId, CallerCoreId) \
   OS_IGNORE_UNREF_PARAM(TaskId)      ;                      \
   OS_IGNORE_UNREF_PARAM(DestCoreId)  ;                      \
   OS_IGNORE_UNREF_PARAM(CallerCoreId);
#endif

#if (OS_IS_DEFINED_OS_VTH_ACTIVATION_LIMIT == STD_OFF)
/***********************************************************************************************************************
 *  OS_VTH_ACTIVATION_LIMIT()
 **********************************************************************************************************************/
/*! \brief          Trace the activation of a task.
 *  \details        This hook is called on the caller core when that core has failed to perform the activation of
 *                  TaskId on the destination core because number of activations has reached the limit.
 *                  As this OS implementation always performs task activation on the destination core, DestCoreId
 *                  and CallerCoreId are always identical.
 *
 *                  Callers:
 *                    - Os_TraceTaskActivateLimit()
 *
 *  \param[in]      TaskId       The ID of the task which is activated
 *  \param[in]      DestCoreId   The ID of the core where the task will be executed
 *  \param[in]      CallerCoreId The ID of the core where this hook is called
 *
 *  \context        OS internal
 *
 *  \reentrant      TRUE for different caller cores.
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 **********************************************************************************************************************/
# define OS_VTH_ACTIVATION_LIMIT(TaskId, DestCoreId, CallerCoreId) \
   OS_IGNORE_UNREF_PARAM(TaskId)      ;                            \
   OS_IGNORE_UNREF_PARAM(DestCoreId)  ;                            \
   OS_IGNORE_UNREF_PARAM(CallerCoreId);
#endif

#if (OS_IS_DEFINED_OS_VTH_SETEVENT == STD_OFF)
/***********************************************************************************************************************
 *  OS_VTH_SETEVENT()
 **********************************************************************************************************************/
/*! \brief          Trace the event setting on a task.
 *  \details        This hook is called on the CallerCore when that core has successfully performed the event
 *                  setting on the destination core. As this OS implementation always performs event setting on the
 *                  destination core, DestCoreId and CallerCoreId are always identical.
 *
 *                  Callers:
 *                    - Os_TraceTaskSetEvent()
 *
 *  \param[in]      TaskId       The ID of the task which receives this event
 *  \param[in]      EventMask    A bit mask with the events which have been set
 *  \param[in]      StateChanged
 *                   - !0: The task state has changed from WAITING to READY
 *                   -  0: The task state has not changed
 *  \param[in]      DestCoreId   The ID of the core where the task will be executed
 *  \param[in]      CallerCoreId The ID of the core where this hook is called
 *
 *  \context        OS internal
 *
 *  \reentrant      TRUE for different caller cores.
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 **********************************************************************************************************************/
# define OS_VTH_SETEVENT(TaskId, EventMask, StateChanged, DestCoreId, CallerCoreId) \
   OS_IGNORE_UNREF_PARAM(TaskId)      ;                                             \
   OS_IGNORE_UNREF_PARAM(EventMask)   ;                                             \
   OS_IGNORE_UNREF_PARAM(StateChanged);                                             \
   OS_IGNORE_UNREF_PARAM(DestCoreId)  ;                                             \
   OS_IGNORE_UNREF_PARAM(CallerCoreId);
#endif


#if (OS_IS_DEFINED_OS_VTH_WAITEVENT_NOWAIT == STD_OFF)
/***********************************************************************************************************************
 *  OS_VTH_WAITEVENT_NOWAIT()
 **********************************************************************************************************************/
/*! \brief          Trace the event waiting on a task.
 *  \details        This hook is called on the CallerCore when that core has successfully performed the wait event
 *                  on the destination core but the event is already set.
 *                  As this OS implementation always performs wait for event on the destination core, DestCoreId
 *                  and CallerCoreId are always identical.
 *
 *                  Callers:
 *                    - Os_TraceTaskWaitEventNoWait()
 *
 *  \param[in]      TaskId       The ID of the task which receives this event
 *  \param[in]      EventMask    A bit mask of the events for which the task is waiting for
 *  \param[in]      DestCoreId   The ID of the core where the task will be executed
 *  \param[in]      CallerCoreId The ID of the core where this hook is called
 *
 *  \context        OS internal
 *
 *  \reentrant      TRUE for different caller cores.
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 *
 **********************************************************************************************************************/
# define OS_VTH_WAITEVENT_NOWAIT(TaskId, EventMask, DestCoreId, CallerCoreId) \
   OS_IGNORE_UNREF_PARAM(TaskId)      ;                                       \
   OS_IGNORE_UNREF_PARAM(EventMask)   ;                                       \
   OS_IGNORE_UNREF_PARAM(DestCoreId)  ;                                       \
   OS_IGNORE_UNREF_PARAM(CallerCoreId);
#endif


#if (OS_IS_DEFINED_OS_VTH_GOT_RES == STD_OFF)
/***********************************************************************************************************************
 *  OS_VTH_GOT_RES()
 **********************************************************************************************************************/
/*! \brief          Trace resource occupation (get)
 *  \details        The OS calls this hook on a successful call of the API function GetResource. The priority of the
 *                  calling task or ISR has been increased so that other tasks and ISRs on the same core may need to
 *                  wait until they can be executed.
 *
 *                  Callers:
 *                    - Os_TraceResourceTaken()
 *
 *  \param[in]      ResId        The ID of the resource which has been taken
 *  \param[in]      CallerCoreId The ID of the core where this hook is called
 *
 *  \context        OS internal
 *
 *  \reentrant      TRUE for different caller cores.
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 *
 **********************************************************************************************************************/
# define OS_VTH_GOT_RES(ResId, CallerCoreId)\
   OS_IGNORE_UNREF_PARAM(ResId)       ;     \
   OS_IGNORE_UNREF_PARAM(CallerCoreId);
#endif


#if (OS_IS_DEFINED_OS_VTH_REL_RES == STD_OFF)
/***********************************************************************************************************************
 *  OS_VTH_REL_RES()
 **********************************************************************************************************************/
/*! \brief          Trace resource occupation (release)
 *  \details        The OS calls this hook on a successful call of the API function ReleaseResource. The priority of
 *                  the calling task or ISR has been decreased so that other tasks and ISRs on the same core may become
 *                  running as a result.
 *
 *                  Callers:
 *                    - Os_TraceResourceReleased()
 *
 *  \param[in]      ResId        The ID of the resource which has been released
 *  \param[in]      CallerCoreId The ID of the core where this hook is called
 *
 *  \context        OS internal
 *
 *  \reentrant      TRUE for different caller cores.
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 *
 **********************************************************************************************************************/
# define OS_VTH_REL_RES(ResId, CallerCoreId)\
   OS_IGNORE_UNREF_PARAM(ResId)       ;     \
   OS_IGNORE_UNREF_PARAM(CallerCoreId);
#endif


#if (OS_IS_DEFINED_OS_VTH_REQ_SPINLOCK == STD_OFF)
/***********************************************************************************************************************
 *  OS_VTH_REQ_SPINLOCK()
 **********************************************************************************************************************/
/*! \brief          Trace attempts to get a spinlock
 *  \details        The OS calls this hook before it enters a busy waiting loop on a spinlock. Tasks or ISRs of lower
 *                  priority have to wait until this task or ISR has taken and released the spinlock.
 *
 *                  Callers:
 *                    - Os_TraceSpinlockRequested()
 *
 *  \param[in]      SpinlockId   The ID of the spinlock, the caller is waiting for
 *  \param[in]      CallerCoreId The ID of the core where this hook is called
 *
 *  \context        OS internal
 *
 *  \reentrant      TRUE for different caller cores.
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 *
 **********************************************************************************************************************/
# define OS_VTH_REQ_SPINLOCK(SpinlockId, CallerCoreId)\
   OS_IGNORE_UNREF_PARAM(SpinlockId)  ;               \
   OS_IGNORE_UNREF_PARAM(CallerCoreId);
#endif


#if (OS_IS_DEFINED_OS_VTH_GOT_SPINLOCK == STD_OFF)
/***********************************************************************************************************************
 *  OS_VTH_GOT_SPINLOCK()
 **********************************************************************************************************************/
/*! \brief          Trace spinlock occupations (get)
 *  \details        The OS calls this hook whenever a spinlock has successfully been taken. If the task or ISR was not
 *                  successful immediately (entered busy waiting state), this hook means that it leaves the busy waiting
 *                  state. From now on no other task or ISR may get the spinlock until the current task or ISR has
 *                  released it.
 *
 *                  Callers:
 *                    - Os_TraceSpinlockTaken()
 *
 *  \param[in]      SpinlockId   The ID of the spinlock which was taken
 *  \param[in]      CallerCoreId The ID of the core where this hook is called
 *
 *  \context        OS internal
 *
 *  \reentrant      TRUE for different caller cores.
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 *
 **********************************************************************************************************************/
# define OS_VTH_GOT_SPINLOCK(SpinlockId, CallerCoreId)\
   OS_IGNORE_UNREF_PARAM(SpinlockId)  ;               \
   OS_IGNORE_UNREF_PARAM(CallerCoreId);
#endif


#if (OS_IS_DEFINED_OS_VTH_REL_SPINLOCK == STD_OFF)
/***********************************************************************************************************************
 *  OS_VTH_REL_SPINLOCK()
 **********************************************************************************************************************/
/*! \brief          Trace spinlock occupations (release)
 *  \details        The OS calls this hook on a release of a spinlock. Other tasks and ISR may take the spinlock now.
 *                  Callers:
 *                    - Os_TraceSpinlockReleased()
 *
 *  \param[in]      SpinlockId   The ID of the spinlock which was released
 *  \param[in]      CallerCoreId The ID of the core where this hook is called
 *
 *  \context        OS internal
 *
 *  \reentrant      TRUE for different caller cores.
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 *
 **********************************************************************************************************************/
# define OS_VTH_REL_SPINLOCK(SpinlockId, CallerCoreId)\
   OS_IGNORE_UNREF_PARAM(SpinlockId)  ;               \
   OS_IGNORE_UNREF_PARAM(CallerCoreId);
#endif


#if (OS_IS_DEFINED_OS_VTH_REQ_ISPINLOCK == STD_OFF)
/***********************************************************************************************************************
 *  OS_VTH_REQ_ISPINLOCK()
 **********************************************************************************************************************/
/*! \brief          Trace attempts to get an internal spinlock
 *  \details        The OS calls this hook before it enters a busy waiting loop on a spinlock. Tasks or ISRs of lower
 *                  priority have to wait until this task or ISR has taken and released the spinlock.
 *
 *                  Callers:
 *                    - Os_TraceSpinlockRequested()
 *
 *  \param[in]      SpinlockId   The ID of the spinlock, the caller is waiting for
 *  \param[in]      CallerCoreId The ID of the core where this hook is called
 *
 *  \context        OS internal
 *
 *  \reentrant      TRUE for different caller cores.
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 *
 **********************************************************************************************************************/
# define OS_VTH_REQ_ISPINLOCK(SpinlockId, CallerCoreId)\
   OS_IGNORE_UNREF_PARAM(SpinlockId)  ;               \
   OS_IGNORE_UNREF_PARAM(CallerCoreId);
#endif


#if (OS_IS_DEFINED_OS_VTH_GOT_ISPINLOCK == STD_OFF)
/***********************************************************************************************************************
 *  OS_VTH_GOT_ISPINLOCK()
 **********************************************************************************************************************/
/*! \brief          Trace internal spinlock occupations (get)
 *  \details        The OS calls this hook whenever a spinlock has successfully been taken. If the task or ISR was not
 *                  successful immediately (entered busy waiting state), this hook means that it leaves the busy waiting
 *                  state. From now on no other task or ISR may get the spinlock until the current task or ISR has
 *                  released it.
 *
 *                  Callers:
 *                    - Os_TraceSpinlockTaken()
 *
 *  \param[in]      SpinlockId   The ID of the spinlock which was taken
 *  \param[in]      CallerCoreId The ID of the core where this hook is called
 *
 *  \context        OS internal
 *
 *  \reentrant      TRUE for different caller cores.
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 *
 **********************************************************************************************************************/
# define OS_VTH_GOT_ISPINLOCK(SpinlockId, CallerCoreId)\
   OS_IGNORE_UNREF_PARAM(SpinlockId)  ;               \
   OS_IGNORE_UNREF_PARAM(CallerCoreId);
#endif


#if (OS_IS_DEFINED_OS_VTH_REL_ISPINLOCK == STD_OFF)
/***********************************************************************************************************************
 *  OS_VTH_REL_ISPINLOCK()
 **********************************************************************************************************************/
/*! \brief          Trace internal spinlock occupations (release)
 *  \details        The OS calls this hook on a release of a spinlock. Other tasks and ISR may take the spinlock now.
 *                  Callers:
 *                    - Os_TraceSpinlockReleased()
 *
 *  \param[in]      SpinlockId   The ID of the spinlock which was released
 *  \param[in]      CallerCoreId The ID of the core where this hook is called
 *
 *  \context        OS internal
 *
 *  \reentrant      TRUE for different caller cores.
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 *
 **********************************************************************************************************************/
# define OS_VTH_REL_ISPINLOCK(SpinlockId, CallerCoreId)\
   OS_IGNORE_UNREF_PARAM(SpinlockId)  ;               \
   OS_IGNORE_UNREF_PARAM(CallerCoreId);
#endif


#if (OS_IS_DEFINED_OS_VTH_DISABLEDINT == STD_OFF)
/***********************************************************************************************************************
 *  OS_VTH_DISABLEDINT()
 **********************************************************************************************************************/
/*! \brief          Trace interrupt locks (disable)
 *  \details        The OS calls this hook if the application has called an API function to disable interrupts. The
 *                  parameter IntLockId describes whether category 1 interrupts may still occur.
 *
 *                  Mind that the two types of interrupt locking (as described by the IntLockId) are independent from
 *                  each other so that the hook may be called twice before the hook OS_VTH_ENABLEDINT is called,
 *                  dependent on the application.
 *
 *
 *                  Callers:
 *                    - Os_TraceInterruptsGlobalDisabled()
 *                    - Os_TraceInterruptsLevelDisabled()
 *
 *  \param[in]      IntLockId:
 *                    - \ref OS_VTHP_CAT2INTERRUPTS
 *                    - \ref OS_VTHP_ALLINTERRUPTS
 *  \param[in]      CallerCoreId The ID of the core where this hook is called
 *
 *  \context        OS internal
 *
 *  \reentrant      TRUE for different caller cores.
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 *
 **********************************************************************************************************************/
# define OS_VTH_DISABLEDINT(IntLockId, CallerCoreId)\
   OS_IGNORE_UNREF_PARAM(IntLockId)   ;             \
   OS_IGNORE_UNREF_PARAM(CallerCoreId);
#endif


#if (OS_IS_DEFINED_OS_VTH_ENABLEDINT == STD_OFF)
/***********************************************************************************************************************
 *  OS_VTH_ENABLEDINT()
 **********************************************************************************************************************/
/*! \brief          Trace interrupt locks (enable)
 *  \details        The OS calls this hook if the application has called an API function to enable interrupts.
 *                  Mind that the two types of interrupt locking (as described by the IntLockId) are independent from
 *                  each other so that interrupts may still be disabled by means of the other locking type after this
 *                  hook has returned.
 *
 *                  Callers:
 *                    - Os_TraceInterruptsGlobalEnabled()
 *                    - Os_TraceInterruptsLevelEnabled()
 *
 *  \param[in]      IntLockId:
 *                    - \ref OS_VTHP_CAT2INTERRUPTS
 *                    - \ref OS_VTHP_ALLINTERRUPTS
 *  \param[in]      CallerCoreId The ID of the core where this hook is called
 *
 *  \context        OS internal
 *
 *  \reentrant      TRUE for different caller cores.
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 *
 **********************************************************************************************************************/
# define OS_VTH_ENABLEDINT(IntLockId, CallerCoreId)\
   OS_IGNORE_UNREF_PARAM(IntLockId)   ;            \
   OS_IGNORE_UNREF_PARAM(CallerCoreId);
#endif

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  Os_TraceIsTimingHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TraceIsTimingHookEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_CFG_OSTIMINGHOOKS == STD_ON);                                                            /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}


/***********************************************************************************************************************
 *  Os_TraceIsScheduleTimingHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TraceIsScheduleTimingHookEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_IS_DEFINED_OS_VTH_SCHEDULE == STD_ON);                                                   /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}

/***********************************************************************************************************************
 *  Os_TraceIsArtiTaskHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TraceIsArtiTaskHookEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_CFG_ARTI_HOOKS_TASK == STD_ON);                                                           /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}

/***********************************************************************************************************************
 *  Os_TraceIsArtiCat2IsrHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TraceIsArtiCat2IsrHookEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_CFG_ARTI_HOOKS_CAT2ISR == STD_ON);                                                        /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}


/***********************************************************************************************************************
 *  Os_TraceIsForcedTerminationTimingHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TraceIsForcedTerminationTimingHookEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_IS_DEFINED_OS_VTH_FORCED_TERMINATION == STD_ON);                                         /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}


/***********************************************************************************************************************
 *  Os_TraceIsActivationTimingHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TraceIsActivationTimingHookEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_IS_DEFINED_OS_VTH_ACTIVATION == STD_ON);                                                 /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}


/***********************************************************************************************************************
 *  Os_TraceIsActivationLimitTimingHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TraceIsActivationLimitTimingHookEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_IS_DEFINED_OS_VTH_ACTIVATION_LIMIT == STD_ON);                                           /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}


/***********************************************************************************************************************
 *  Os_TraceIsSetEventTimingHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TraceIsSetEventTimingHookEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_IS_DEFINED_OS_VTH_SETEVENT == STD_ON);                                                   /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}


/***********************************************************************************************************************
 *  Os_TraceIsWaitEventTimingHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TraceIsWaitEventTimingHookEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_IS_DEFINED_OS_VTH_WAITEVENT_NOWAIT == STD_ON);                                           /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}


/***********************************************************************************************************************
 *  Os_TraceIsGotResourceTimingHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TraceIsGotResourceTimingHookEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_IS_DEFINED_OS_VTH_GOT_RES == STD_ON);                                                    /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}


/***********************************************************************************************************************
 *  Os_TraceIsRelResourceTimingHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TraceIsRelResourceTimingHookEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_IS_DEFINED_OS_VTH_REL_RES == STD_ON);                                                    /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}


/***********************************************************************************************************************
 *  Os_TraceIsReqSpinlockTimingHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TraceIsReqSpinlockTimingHookEnabled,
( void ))
{
  return (Os_StdReturnType)((OS_IS_DEFINED_OS_VTH_REQ_SPINLOCK == STD_ON) ||                                            /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
                            (OS_IS_DEFINED_OS_VTH_REQ_ISPINLOCK == STD_ON));                                            /* PRQA S 2996 */ /* MD_Os_Rule2.2_2996 */
}


/***********************************************************************************************************************
 *  Os_TraceIsGotSpinlockTimingHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TraceIsGotSpinlockTimingHookEnabled,
( void ))
{
  return (Os_StdReturnType)((OS_IS_DEFINED_OS_VTH_GOT_SPINLOCK == STD_ON) ||                                            /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
                            (OS_IS_DEFINED_OS_VTH_GOT_ISPINLOCK == STD_ON));                                            /* PRQA S 2996 */ /* MD_Os_Rule2.2_2996 */
}


/***********************************************************************************************************************
 *  Os_TraceIsRelSpinlockTimingHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TraceIsRelSpinlockTimingHookEnabled,
( void ))
{
  return (Os_StdReturnType)((OS_IS_DEFINED_OS_VTH_REL_SPINLOCK == STD_ON) ||                                            /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
                            (OS_IS_DEFINED_OS_VTH_REL_ISPINLOCK == STD_ON));                                            /* PRQA S 2996 */ /* MD_Os_Rule2.2_2996 */
}


/***********************************************************************************************************************
 *  Os_TraceIsDisabledIntTimingHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TraceIsDisabledIntTimingHookEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_IS_DEFINED_OS_VTH_DISABLEDINT == STD_ON);                                                /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}


/***********************************************************************************************************************
 *  Os_TraceIsEnabledIntTimingHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_TraceIsEnabledIntTimingHookEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_IS_DEFINED_OS_VTH_ENABLEDINT == STD_ON);                                                 /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}


/***********************************************************************************************************************
 *  Os_TraceIsOrtiServiceTracingEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_TraceIsOrtiServiceTracingEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_CFG_ORTI_ADDITIONAL == STD_ON);                                                          /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}


/***********************************************************************************************************************
 *  Os_TraceIsOrtiTaskIsrTracingEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_TraceIsOrtiTaskIsrTracingEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_CFG_ORTI == STD_ON);                                                                     /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}

/***********************************************************************************************************************
 *  Os_TraceIsArtiApplHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_TraceIsArtiApplHookEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_CFG_ARTI_HOOKS_APPLICATION == STD_ON);                                                   /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}

/***********************************************************************************************************************
 *  Os_TraceIsArtiSpinlockHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_TraceIsArtiSpinlockHookEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_CFG_ARTI_HOOKS_SPINLOCK == STD_ON);                                                      /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}

/***********************************************************************************************************************
 *  Os_TraceIsArtiProtHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_TraceIsArtiProtHookEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_CFG_ARTI_HOOKS_PROTECTIONHOOK == STD_ON);                                                /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}

/***********************************************************************************************************************
 *  Os_TraceIsArtiServiceHookEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_TraceIsArtiServiceHookEnabled,
( void ))
{
  return (Os_StdReturnType)(OS_CFG_ARTI_HOOKS_SERVICECALLS == STD_ON);                                                  /* PRQA S 4304, 2996 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule2.2_2996 */
}

/***********************************************************************************************************************
 *  Os_TraceThreadInit()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TraceThreadInit,
(
  P2CONST(Os_TraceThreadConfigType, AUTOMATIC, OS_CONST) Thread
))
{
  /* - #10 Check if timing hooks are enabled. */
  if(Os_TraceIsTimingHookEnabled() != 0u)                                                                               /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    /* - #20 Set the initial next run reason for the respective thread type. */
    switch(Thread->Type)
    {
    case OS_TRACE_THREAD_TYPE_TASK:
      Thread->Dyn->NextRunReason = OS_TRACE_TASK_ACTIVATION;                                                            /* SBSW_OS_TRACE_GETDYN_001 */
      break;
    case OS_TRACE_THREAD_TYPE_ISR:
      Thread->Dyn->NextRunReason = OS_TRACE_ISR_START;                                                                  /* SBSW_OS_TRACE_GETDYN_001 */
      break;
    default:
      Thread->Dyn->NextRunReason = OS_TRACE_THREAD_MISC;                                                                /* SBSW_OS_TRACE_GETDYN_001 */
      break;
    }
  }
}


/***********************************************************************************************************************
 *  Os_TraceThreadInitAndStart()
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
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TraceThreadInitAndStart,
(
  P2CONST(Os_TraceThreadConfigType, AUTOMATIC, OS_CONST) Task
))
{
  /* #10 If the timing hook for scheduling or the ARTI task hook is enabled: */
  if((Os_TraceIsScheduleTimingHookEnabled() != 0u) || (Os_TraceIsArtiTaskHookEnabled() != 0u))                          /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;

    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* - #20 Assert that passed parameter is valid. */
    Os_Assert((Os_StdReturnType)(Task->Type == OS_TRACE_THREAD_TYPE_TASK));                                             /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */

    /* - #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* - #40 Check if timing hook for scheduling is enabled. */
    if(Os_TraceIsScheduleTimingHookEnabled() != 0u)                                                                     /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
    {
      /* - #50 Call the Vector timing hook (user code) for scheduling: OS_VTH_SCHEDULE(). */
      /* variable values are casted in order to assure that they cannot be written in the hook */
      OS_VTH_SCHEDULE((OS_TRACE_INVALID_THREAD),                                                                        /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */
                      (OS_VTHP_THREAD_PREEMPT),
                      (Task->Id),
                      (OS_VTHP_TASK_ACTIVATION),
                      ((CoreIdType) Os_CoreGetId(core)))

      /* No initialization of NextRunReason necessary as it is anyhow setup when the task is left and
       * not needed before it gets active for the next time. */
    }
    /* #60 If the ARTI task hook is enabled: */
    if (Os_TraceIsArtiTaskHookEnabled() != 0u)
    {
      /* #70 Call the respective ARTI hook for the first task. */
      OS_CALL_ARTI_TRACE(OSARTI_TASK, ((CoreIdType) Os_CoreGetId(core)), OsTask_Start, Task->Id)
    }
    /* - #80 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
}

/***********************************************************************************************************************
 *  Os_TraceThreadSwitch()
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
 */
/* PRQA S 6010, 6030, 6050, 6080 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceThreadSwitch,
(
  TaskStateType CurrentTaskState,
  P2CONST(Os_TraceThreadConfigType, AUTOMATIC, OS_CONST) Current,
  P2CONST(Os_TraceThreadConfigType, AUTOMATIC, OS_CONST) Next
))
{
  /* #10 If the timing hook for scheduling or the ARTI task hook is enabled: */
  if((Os_TraceIsScheduleTimingHookEnabled() != 0u) || (Os_TraceIsArtiTaskHookEnabled() != 0u))                          /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;

    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #20 Set current tasks next run reason in accordance to its task state */
    if(CurrentTaskState == WAITING)
    {
      Current->Dyn->NextRunReason = OS_TRACE_TASK_SETEVENT;                                                             /* SBSW_OS_TRACE_GETDYN_001 */
    }
    else if(CurrentTaskState == READY)
    {
      Current->Dyn->NextRunReason = OS_TRACE_THREAD_RESUME;                                                             /* SBSW_OS_TRACE_GETDYN_001 */
    }
    else
    {
      Os_ErrKernelPanic(); /* called with no other task state */                                                        /* COV_OS_INVSTATE */
    }

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 If the timing hook for scheduling is enabled: */
    if (Os_TraceIsScheduleTimingHookEnabled() != 0u)
    {
      /* #50 Call the Vector timing hook (user code) for scheduling: OS_VTH_SCHEDULE(). */
      /* variable values are casted in order to assure that they cannot be written in the hook */
      OS_VTH_SCHEDULE( (Current->Id),                                                                                   /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */
                       ((uint8)(Current->Dyn->NextRunReason)),
                       (Next->Id),
                       ((uint8)(Next->Dyn->NextRunReason)),
                       ((CoreIdType) Os_CoreGetId(core)) )
    }
    /* #60 If the ARTI task hook is enabled: */
    if (Os_TraceIsArtiTaskHookEnabled() != 0u)
    {
      /* #70 Call the respective ARTI hook for the current task according to the current task state. */
      if(CurrentTaskState == WAITING)
      {
        OS_CALL_ARTI_TRACE(OSARTI_TASK, ((CoreIdType) Os_CoreGetId(core)), OsTask_Wait, Current->Id)
      }
      else if(CurrentTaskState == READY)                                                                                /* COMP_WARN_OS_CW_005 */
      {
        OS_CALL_ARTI_TRACE(OSARTI_TASK, ((CoreIdType) Os_CoreGetId(core)), OsTask_Preempt, Current->Id)
      }
      else
      {
        /* Intentionally empty. Unreachable code. */
      }

      /* #80 Call the respective ARTI hook for the next task according to the next run reason. */
      if(Next->Dyn->NextRunReason == OS_TRACE_TASK_SETEVENT)
      {
        OS_CALL_ARTI_TRACE(OSARTI_TASK, ((CoreIdType) Os_CoreGetId(core)), OsTask_Continue, Next->Id)
      }
      else if(Next->Dyn->NextRunReason == OS_TRACE_THREAD_RESUME)
      {
        OS_CALL_ARTI_TRACE(OSARTI_TASK, ((CoreIdType) Os_CoreGetId(core)), OsTask_Resume, Next->Id)
      }
      else if(Next->Dyn->NextRunReason == OS_TRACE_TASK_ACTIVATION)
      {
        OS_CALL_ARTI_TRACE(OSARTI_TASK, ((CoreIdType) Os_CoreGetId(core)), OsTask_Start, Next->Id)
      }
      else
      {
        Os_ErrKernelPanic(); /* called with no other run reason */                                                      /* COV_OS_INVSTATE */
      }
    }
    /* #90 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }

  /* #100 Store the currently running task if Orti task tracing is enabled. */
  if (Os_TraceIsOrtiTaskIsrTracingEnabled() != 0u)                                                                      /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    core = Os_CoreGetCurrentCore();
    Os_CoreGetTrace(core)->CurrentTask   = Next;                                                                        /* SBSW_OS_TRACE_COREGETTRACE_001 */ /* SBSW_OS_TRACE_COREGETTRACE_002 */
    Os_CoreGetTrace(core)->CurrentThread = Next;                                                                        /* SBSW_OS_TRACE_COREGETTRACE_001 */ /* SBSW_OS_TRACE_COREGETTRACE_002 */
  }
}


/***********************************************************************************************************************
 *  Os_TraceThreadResetAndResume()
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
 */
/* PRQA S 6050, 6080 1 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceThreadResetAndResume,
(
  P2CONST(Os_TraceThreadConfigType, AUTOMATIC, OS_CONST) Current,
  P2CONST(Os_TraceThreadConfigType, AUTOMATIC, OS_CONST) Next
))
{
  /* #10 If the timing hook for scheduling or the ARTI task hook is enabled: */
  if((Os_TraceIsScheduleTimingHookEnabled() != 0u) || (Os_TraceIsArtiTaskHookEnabled() != 0u))                          /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;

    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #20 Set current tasks next run reason */
    Current->Dyn->NextRunReason = OS_TRACE_TASK_ACTIVATION;                                                             /* SBSW_OS_TRACE_GETDYN_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */


    /* #40 If the timing hook for scheduling is enabled: */
    if (Os_TraceIsScheduleTimingHookEnabled() != 0u)
    {
      /* #50 Call the Vector timing hook (user code) for scheduling: OS_VTH_SCHEDULE(). */
      /* variable values are casted in order to assure that they cannot be written in the hook */
      OS_VTH_SCHEDULE( (Current->Id),                                                                                   /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */
                       ((uint8)(Current->Dyn->NextRunReason)),
                       (Next->Id),
                       ((uint8)(Next->Dyn->NextRunReason)),
                       ((CoreIdType) Os_CoreGetId(core)) )
    }

    /* #60 If the ARTI task hook is enabled: */
    if (Os_TraceIsArtiTaskHookEnabled() != 0u)
    {
      /* #70 Call the ARTI hook for task termination for the current task. */
      OS_CALL_ARTI_TRACE(OSARTI_TASK, ((CoreIdType) Os_CoreGetId(core)), OsTask_Terminate, Current->Id)

      /* #80 Call the respective ARTI hook for the next task according to the next run reason. */
      if (Next->Dyn->NextRunReason == OS_TRACE_TASK_SETEVENT)
      {
        OS_CALL_ARTI_TRACE(OSARTI_TASK, ((CoreIdType) Os_CoreGetId(core)), OsTask_Continue, Next->Id)
      }
      else if (Next->Dyn->NextRunReason == OS_TRACE_THREAD_RESUME)
      {
        OS_CALL_ARTI_TRACE(OSARTI_TASK, ((CoreIdType) Os_CoreGetId(core)), OsTask_Resume, Next->Id)
      }
      else if (Next->Dyn->NextRunReason == OS_TRACE_TASK_ACTIVATION)
      {
        OS_CALL_ARTI_TRACE(OSARTI_TASK, ((CoreIdType) Os_CoreGetId(core)), OsTask_Start, Next->Id)
      }
      else
      {
        Os_ErrKernelPanic(); /* called with no other run reason */
      }
    }

    /* #90 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
  /* #100 Store the currently running task if Orti task tracing is enabled. */
  if (Os_TraceIsOrtiTaskIsrTracingEnabled() != 0u)                                                                      /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    core = Os_CoreGetCurrentCore();
    Os_CoreGetTrace(core)->CurrentTask   = Next;                                                                        /* SBSW_OS_TRACE_COREGETTRACE_001 */ /* SBSW_OS_TRACE_COREGETTRACE_002 */
    Os_CoreGetTrace(core)->CurrentThread = Next;                                                                        /* SBSW_OS_TRACE_COREGETTRACE_001 */ /* SBSW_OS_TRACE_COREGETTRACE_002 */
  }
}


/***********************************************************************************************************************
 *  Os_TraceThreadCleanupAndResume()
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
 *
 */
/* PRQA S 6010, 6030, 6050, 6080 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceThreadCleanupAndResume,
(
  P2CONST(Os_TraceThreadConfigType, AUTOMATIC, OS_CONST) Current,
  P2CONST(Os_TraceThreadConfigType, AUTOMATIC, OS_CONST) Next
))
{
  /* #10 If the timing hook for scheduling, the ARTI task hook or the ARTI Cat2 ISR hook is enabled: */
  if((Os_TraceIsScheduleTimingHookEnabled() != 0u) ||                                                                   /* PRQA S 2992, 2996 2 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
     (Os_TraceIsArtiTaskHookEnabled()       != 0u) ||
     (Os_TraceIsArtiCat2IsrHookEnabled()    != 0u))
  {
    /* #20 If the current thead is a ISR (no hook): */
    /*!
     * Internal comment removed.
 *
 *
 *
 *
     */
    if(OS_LIKELY(Current->Type == OS_TRACE_THREAD_TYPE_ISR))
    {

      P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
      P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;

      core = Os_CoreGetCurrentCore();
      coreAsr = Os_Core2AsrCore(core);                                                                                  /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

      /* #30 Set current threads next run reason. */
      Current->Dyn->NextRunReason = OS_TRACE_ISR_START;                                                                 /* SBSW_OS_TRACE_GETDYN_001 */

      /* #40 Enter critical user section. */
      Os_CoreCriticalUserSectionEnter(coreAsr);                                                                         /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

      /* #50 If the timing hook for scheduling is enabled: */
      if (Os_TraceIsScheduleTimingHookEnabled() != 0u)
      {
        /* #60 Call the Vector timing hook (user code) for scheduling: OS_VTH_SCHEDULE(). */
        /* variable values are casted in order to assure that they cannot be written in the hook */
        OS_VTH_SCHEDULE( (Current->Id),                                                                                 /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */
                         ((uint8)(Current->Dyn->NextRunReason)),
                         (Next->Id),
                         ((uint8)(Next->Dyn->NextRunReason)),
                         ((CoreIdType) Os_CoreGetId(core)) )
      }
      /* #70 If the ARTI Cat2 ISR hook is enabled: */
      if (Os_TraceIsArtiCat2IsrHookEnabled() != 0u)
      {
        /* #80 Call the ARTI hook for stopping Cat2 ISRs for the current ISR. */
        OS_CALL_ARTI_TRACE(OS_CAT2ISR, ((CoreIdType) Os_CoreGetId(core)), OsCat2Isr_Stop, Current->Id)
      }

      /* #90 If the next thread is a task: */
      if(Next->Type == OS_TRACE_THREAD_TYPE_TASK)                                                                       /* COV_OS_HALPLATFORMISRNESTINGSUPPORT_TX */
      {
        /* #95 If the ARTI task hook is enabled: */
        if (Os_TraceIsArtiTaskHookEnabled() != 0u)
        {
          /* #100 Call the respective ARTI hook for the next task according to the next run reason. */
          if (Next->Dyn->NextRunReason == OS_TRACE_TASK_SETEVENT)
          {
            OS_CALL_ARTI_TRACE(OSARTI_TASK, ((CoreIdType) Os_CoreGetId(core)), OsTask_Continue, Next->Id)
          }
          else if (Next->Dyn->NextRunReason == OS_TRACE_THREAD_RESUME)
          {
            OS_CALL_ARTI_TRACE(OSARTI_TASK, ((CoreIdType) Os_CoreGetId(core)), OsTask_Resume, Next->Id)
          }
          else if (Next->Dyn->NextRunReason == OS_TRACE_TASK_ACTIVATION)
          {
            OS_CALL_ARTI_TRACE(OSARTI_TASK, ((CoreIdType) Os_CoreGetId(core)), OsTask_Start, Next->Id)
          }
          else
          {
            Os_ErrKernelPanic(); /* called with no other run reason */                                                  /* COV_OS_INVSTATE */
          }
        }
      }
      /* #110 Otherwise, if the next thread is an ISR: */
      else if(Next->Type == OS_TRACE_THREAD_TYPE_ISR)
      {
        /* #115 If the ARTI CAT2 ISR hook is enabled: */
        if (Os_TraceIsArtiCat2IsrHookEnabled() != 0u)
        {
          /* #120 Call the ARTI hook for starting Cat2 ISRs for the next ISR. */
          OS_CALL_ARTI_TRACE(OS_CAT2ISR, ((CoreIdType) Os_CoreGetId(core)), OsCat2Isr_Start, Next->Id)
        }
      }
      else
      {
        Os_ErrKernelPanic(); /* called with no other thread type */                                                     /* COV_OS_INVSTATE */
      }
      /* #130 Leave critical user section. */
      Os_CoreCriticalUserSectionLeave(coreAsr);                                                                         /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
    }
  }

  /* #140 Store the currently running ISR if Orti ISR tracing is enabled. */
  if (Os_TraceIsOrtiTaskIsrTracingEnabled() != 0u)                                                                      /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2VAR(Os_TraceCoreType, TYPEDEF, OS_VAR_NOINIT) trace;

    core = Os_CoreGetCurrentCore();
    trace = Os_CoreGetTrace(core);                                                                                      /* SBSW_OS_TRACE_COREGETTRACE_002 */

    trace->CurrentThread = Next;                                                                                        /* SBSW_OS_TRACE_COREGETTRACE_001 */

    if (Next->Type == OS_TRACE_THREAD_TYPE_ISR)                                                                         /* COV_OS_HALPLATFORMISRNESTINGSUPPORT_XF */
    {
      trace->CurrentIsr    = Next;                                                                                      /* SBSW_OS_TRACE_COREGETTRACE_001 */
    }
    else
    {
      trace->CurrentIsr    = NULL_PTR;                                                                                  /* SBSW_OS_TRACE_COREGETTRACE_001 */
      trace->CurrentTask   = Next;                                                                                      /* SBSW_OS_TRACE_COREGETTRACE_001 */
    }
  }
}


/***********************************************************************************************************************
 *  Os_TraceThreadSuspendAndStart()
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
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceThreadSuspendAndStart,
(
  P2CONST(Os_TraceThreadConfigType, AUTOMATIC, OS_CONST) Current,
  P2CONST(Os_TraceThreadConfigType, AUTOMATIC, OS_CONST) Next
))
{
  /* #10 If the timing hook for scheduling, the ARTI task hook or the ARTI Cat2 ISR hook is enabled: */
  if((Os_TraceIsScheduleTimingHookEnabled() != 0u) ||                                                                   /* PRQA S 2992, 2996 2 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
     (Os_TraceIsArtiTaskHookEnabled()       != 0u) ||
     (Os_TraceIsArtiCat2IsrHookEnabled()    != 0u))
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;

    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #20 Set current threads next run reason. */
    Current->Dyn->NextRunReason = OS_TRACE_THREAD_RESUME;                                                               /* SBSW_OS_TRACE_GETDYN_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 If the timing hook for scheduling is enabled: */
    if (Os_TraceIsScheduleTimingHookEnabled() != 0u)
    {
      /* #50 Call the Vector timing hook (user code) for scheduling: OS_VTH_SCHEDULE(). */
      /* variable values are casted in order to assure that they cannot be written in the hook */
      OS_VTH_SCHEDULE( (Current->Id),                                                                                   /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */
                       ((uint8)(Current->Dyn->NextRunReason)),
                       (Next->Id),
                       ((uint8)(Next->Dyn->NextRunReason)),
                       ((CoreIdType) Os_CoreGetId(core)) )
    }
    /* #60 If the current thread is a task and the ARTI task hook is enabled: */
    if(Current->Type == OS_TRACE_THREAD_TYPE_TASK)                                                                      /* COV_OS_HALPLATFORMISRNESTINGSUPPORT_TX */
    {
      if (Os_TraceIsArtiTaskHookEnabled() != 0u)
      {
        /* #70 Call the ARTI hook for task preemption for the current task. */
        OS_CALL_ARTI_TRACE(OSARTI_TASK, ((CoreIdType) Os_CoreGetId(core)), OsTask_Preempt, Current->Id)
      }
    }
    /* #80 Otherwise, if the current thread is an ISR and the ARTI Cat2 ISR hook is enabled: */
    else if(Current->Type == OS_TRACE_THREAD_TYPE_ISR)
    {
      if (Os_TraceIsArtiCat2IsrHookEnabled() != 0u)
      {
        /* #90 Call the ARTI hook for stopping Cat2 ISRs for the current ISR. */
        OS_CALL_ARTI_TRACE(OS_CAT2ISR, ((CoreIdType) Os_CoreGetId(core)), OsCat2Isr_Stop, Current->Id)
      }
    }
    else
    {
      Os_ErrKernelPanic(); /* called with no other thread type */                                                       /* COV_OS_INVSTATE */
    }
    /* #100 If the ARTI Cat2 ISR hook is enabled, call the ARTI hook for starting Cat2 ISRs for the next thread. */
    if (Os_TraceIsArtiCat2IsrHookEnabled() != 0u)
    {
      OS_CALL_ARTI_TRACE(OS_CAT2ISR, ((CoreIdType) Os_CoreGetId(core)), OsCat2Isr_Start, Next->Id)
    }
    /* #100 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }

  /* #110 Store the currently running ISR if Orti ISR tracing is enabled. */
  if (Os_TraceIsOrtiTaskIsrTracingEnabled() != 0u)                                                                      /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    core = Os_CoreGetCurrentCore();
    Os_CoreGetTrace(core)->CurrentIsr    = Next;                                                                        /* SBSW_OS_TRACE_COREGETTRACE_001 */ /* SBSW_OS_TRACE_COREGETTRACE_002 */
    Os_CoreGetTrace(core)->CurrentThread = Next;                                                                        /* SBSW_OS_TRACE_COREGETTRACE_001 */ /* SBSW_OS_TRACE_COREGETTRACE_002 */
  }
}


/***********************************************************************************************************************
 *  Os_TraceIsrKill()
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceIsrKill,
(
  P2CONST(Os_TraceThreadConfigType, AUTOMATIC, OS_CONST) KilledIsr
))
{
  /* - #10 Check if timing hook for forcible termination is enabled. */
  if(Os_TraceIsForcedTerminationTimingHookEnabled() != 0u)                                                              /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    coreAsr = Os_Core2AsrCore(Os_CoreGetCurrentCore());                                                                 /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* - #20 Set Isrs next run reason */
    KilledIsr->Dyn->NextRunReason = OS_TRACE_THREAD_CLEANUP;                                                            /* SBSW_OS_TRACE_GETDYN_001 */

    /* - #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* - #40 Call the Vector timing hook for killing: OS_VTH_FORCED_TERMINATION(). */
    OS_VTH_FORCED_TERMINATION((KilledIsr->Id), (Os_CoreGetId(Os_CoreGetCurrentCore())))                                 /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */

    /* - #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
}


/***********************************************************************************************************************
 *  Os_TraceTaskKill()
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
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceTaskKill,
(
  P2CONST(Os_TraceThreadConfigType, AUTOMATIC, OS_CONST) KilledTask,
  Os_StdReturnType IsTaskSuspended
))
{
  /* - #10 Check if timing hook for forcible termination is enabled. */
  if(Os_TraceIsForcedTerminationTimingHookEnabled() != 0u)                                                              /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    /* - #20 Set tasks next run reason (dependent on currently running thread) */
    if(KilledTask == Os_ThreadGetTrace(Os_CoreGetThread()))                                                             /* SBSW_OS_TRACE_THREADGETTRACE_001 */ /* COMP_WARN_OS_CW_005 */
    {
      KilledTask->Dyn->NextRunReason = OS_TRACE_THREAD_CLEANUP;                                                         /* SBSW_OS_TRACE_GETDYN_001 */
    }
    else
    {
      KilledTask->Dyn->NextRunReason = OS_TRACE_TASK_ACTIVATION;                                                        /* SBSW_OS_TRACE_GETDYN_001 */
    }

    /* - #30 Check if task is not suspended */
    if(IsTaskSuspended == 0u)
    {
      P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
      coreAsr = Os_Core2AsrCore(Os_CoreGetCurrentCore());                                                               /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

      /* - #40 Enter critical user section. */
      Os_CoreCriticalUserSectionEnter(coreAsr);                                                                         /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

      /* - #50 Call the Vector timing hook for killing OS_VTH_FORCED_TERMINATION() */
      OS_VTH_FORCED_TERMINATION((KilledTask->Id), (Os_CoreGetId(Os_CoreGetCurrentCore())))                              /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */

      /* - #60 Leave critical user section. */
      Os_CoreCriticalUserSectionLeave(coreAsr);                                                                         /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
    }
  }
}


/***********************************************************************************************************************
 *  Os_TraceTaskActivate()
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceTaskActivate,
(
  P2CONST(Os_TraceThreadConfigType, AUTOMATIC, OS_CONST) Task
))
{
  /* #10 If the timing hook for task activation or the ARTI task hook is enabled: */
  if((Os_TraceIsActivationTimingHookEnabled() != 0u) || (Os_TraceIsArtiTaskHookEnabled() != 0u))                        /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;

    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #20 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #30 If the timing hook for task activation is enabled: */
    if (Os_TraceIsActivationTimingHookEnabled() != 0u)                                                                  /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
    {
      /* #40 Call the vector timing hook for task activation: OS_VTH_ACTIVATION(). */
      /* variable values are casted in order to assure that they cannot be written in the hook */
      OS_VTH_ACTIVATION((Task->Id), ((CoreIdType) Os_CoreGetId(core)), ((CoreIdType) Os_CoreGetId(core)))               /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */ /* SBSW_OS_TRACE_COREGETID_001 */
    }
    /* #50 If the ARTI task hook is enabled, call the ARTI hook for task activation. */
    if (Os_TraceIsArtiTaskHookEnabled() != 0u)                                                                          /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
    {
      OS_CALL_ARTI_TRACE(OSARTI_TASK, ((CoreIdType) Os_CoreGetId(core)), OsTask_Activate, (Task->Id))
    }

    /* #60 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
}

/***********************************************************************************************************************
 *  Os_TraceTaskActivateLimit()
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceTaskActivateLimit,
(
  P2CONST(Os_TraceThreadConfigType, AUTOMATIC, OS_CONST) Task
))
{
  /* - #10 Check if timing hook for a failed task activation is enabled. */
  if(Os_TraceIsActivationLimitTimingHookEnabled() != 0u)                                                                /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;

    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* - #20 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* - #30 Call the vector timing hook for failed task activation: OS_VTH_ACTIVATION_LIMIT(). */
    /* variable values are casted in order to assure that they cannot be written in the hook */
    OS_VTH_ACTIVATION_LIMIT((Task->Id), ((CoreIdType) Os_CoreGetId(core)), ((CoreIdType) Os_CoreGetId(core)))           /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */ /* SBSW_OS_TRACE_COREGETID_001 */

    /* - #40 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
}


/***********************************************************************************************************************
 *  Os_TraceTaskSetEvent()
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceTaskSetEvent,
(
  P2CONST(Os_TraceThreadConfigType, AUTOMATIC, OS_CONST) Task,
  EventMaskType EventMask,
  boolean StateChanged
))
{
  /* #10 If the timing hook for event setting or the ARTI task hook is enabled: */
  if((Os_TraceIsSetEventTimingHookEnabled() != 0u)  || (Os_TraceIsArtiTaskHookEnabled() != 0u))                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;

    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #20 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #30 If the timing hook for event setting is enabled: */
    if (Os_TraceIsSetEventTimingHookEnabled() != 0u)
    {
      /* #40 Call the vector timing hook for event setting: OS_VTH_SETEVENT(). */
      /* variable values are casted in order to assure that they cannot be written in the hook */
      OS_VTH_SETEVENT((Task->Id),                                                                                       /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */ /* SBSW_OS_TRACE_COREGETID_001 */
                     ((EventMaskType) EventMask),
                     ((boolean) StateChanged),
                     ((CoreIdType) Os_CoreGetId(core)),
                     ((CoreIdType) Os_CoreGetId(core)) )
    }
    /* #50 If the ARTI task hook is enabled, call the ARTI hook for event setting. */
    if (Os_TraceIsArtiTaskHookEnabled() != 0u)
    {
      OS_CALL_ARTI_TRACE(OSARTI_TASK, ((CoreIdType) Os_CoreGetId(core)), OsTask_Release, (Task->Id))
    }
    /* #60 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
}


/***********************************************************************************************************************
 *  Os_TraceTaskWaitEventNoWait()
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceTaskWaitEventNoWait,
(
  P2CONST(Os_TraceThreadConfigType, AUTOMATIC, OS_CONST) Task,
  EventMaskType EventMask
))
{
  /* - #10 Check if timing hook for event waiting is enabled. */
  if(Os_TraceIsWaitEventTimingHookEnabled() != 0u)                                                                      /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;

    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* - #20 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* - #30 Call the vector timing hook for task not waiting: OS_VTH_WAITEVENT_NOWAIT() */
    /* variable values are casted in order to assure that they cannot be written in the hook */
    OS_VTH_WAITEVENT_NOWAIT((Task->Id),                                                                                 /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */ /* SBSW_OS_TRACE_COREGETID_001 */
                           ((EventMaskType) EventMask),
                           ((CoreIdType) Os_CoreGetId(core)),
                           ((CoreIdType) Os_CoreGetId(core)) )

    /* - #40 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceResourceTaken()
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceResourceTaken,
(
    ResourceType ResId
))
{
  /* - #10 Check if timing hook for resource allocation is enabled. */
  if(Os_TraceIsGotResourceTimingHookEnabled() != 0u)                                                                    /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    coreAsr = Os_Core2AsrCore(Os_CoreGetCurrentCore());                                                                 /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* - #20 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* - #30 Call the vector timing hook for resource occupation: OS_VTH_GOT_RES(). */
    OS_VTH_GOT_RES((ResId), (Os_CoreGetId(Os_CoreGetCurrentCore())))                                                    /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */

    /* - #40 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceResourceReleased()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceResourceReleased,
(
    ResourceType ResId
))
{
  /* - #10 Check if timing hook for releasing a resource is enabled. */
  if(Os_TraceIsRelResourceTimingHookEnabled() != 0u)                                                                    /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    coreAsr = Os_Core2AsrCore(Os_CoreGetCurrentCore());                                                                 /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* - #20 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* - #30 Call the vector timing hook for resource occupation: OS_VTH_REL_RES(). */
    OS_VTH_REL_RES((ResId), (Os_CoreGetId(Os_CoreGetCurrentCore())))                                                    /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */

    /* - #40 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceSpinlockRequested()
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
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceSpinlockRequested,
(
    P2CONST(Os_TraceSpinlockConfigType, AUTOMATIC, OS_CONST) Spinlock
))
{
  /* - #10 Check if timing hook for requesting a spinlock is enabled. */
  if(Os_TraceIsReqSpinlockTimingHookEnabled() != 0u)                                                                    /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    Os_IntStateType interruptState;
    coreAsr = Os_Core2AsrCore(Os_CoreGetCurrentCore());                                                                 /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* No NULL_PTR expected by this function */
    Os_Assert((Os_StdReturnType)(Spinlock != NULL_PTR));                                                                /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */

    /* - #20 Suspend interrupts. */
    Os_IntSuspend(&interruptState);                                                                                     /* SBSW_OS_FC_POINTER2LOCAL */

    /* - #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    if (Spinlock->SpinlockId < Os_TraceGetNumberOfConfiguredSpinlocks())                                                /* COV_OS_HALPLATFORMTESTEDASMULTICORE */ /* COMP_WARN_OS_CW_005 */
    {
      /* - #40 For configured spinlocks call: OS_VTH_REQ_SPINLOCK(). */
      OS_VTH_REQ_SPINLOCK((Spinlock->SpinlockId), (Os_CoreGetId(Os_CoreGetCurrentCore())))                              /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */
    }
    else
    {
      /* - #50 For internal spinlocks call: OS_VTH_REQ_ISPINLOCK(). */
      OS_VTH_REQ_ISPINLOCK((Spinlock->SpinlockId), (Os_CoreGetId(Os_CoreGetCurrentCore())))                             /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */
    }

    /* - #60 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */

    /* - #70 Resume interrupts. */
    Os_IntResume(&interruptState);                                                                                      /* SBSW_OS_FC_POINTER2LOCAL */
  }
}

/***********************************************************************************************************************
 * Os_TraceSpinlockTaken
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
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceSpinlockTaken,
(
    P2CONST(Os_TraceSpinlockConfigType, AUTOMATIC, OS_CONST) Spinlock
))
{
  /* #10 If the timing hook for spinlock allocation or the ARTI spinlock hook is enabled: */
  if((Os_TraceIsGotSpinlockTimingHookEnabled() != 0u) || (Os_TraceIsArtiSpinlockHookEnabled() != 0u))                    /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    Os_IntStateType interruptState;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* No NULL_PTR expected by this function */
    Os_Assert((Os_StdReturnType)(Spinlock != NULL_PTR));                                                                /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */

    /* #20 Suspend interrupts. */
    Os_IntSuspend(&interruptState);                                                                                     /* SBSW_OS_FC_POINTER2LOCAL */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 If the timing hook for spinlock allocation is enabled: */
    if(Os_TraceIsGotSpinlockTimingHookEnabled() != 0u)
    {
      if (Spinlock->SpinlockId < Os_TraceGetNumberOfConfiguredSpinlocks())                                              /* COV_OS_HALPLATFORMTESTEDASMULTICORE */ /* COMP_WARN_OS_CW_005 */
      {
        /* #50 For configured spinlocks call: OS_VTH_GOT_SPINLOCK(). */
        OS_VTH_GOT_SPINLOCK((Spinlock->SpinlockId), (Os_CoreGetId(core)))                                               /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */
      }
      else
      {
        /* #60 For internal   spinlocks call: OS_VTH_GOT_ISPINLOCK(). */
        OS_VTH_GOT_ISPINLOCK((Spinlock->SpinlockId), (Os_CoreGetId(core)))                                              /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */
      }
    }
    /* #70 If the ARTI spinlock hook is enabled, call the ARTI hook for allocating spinlocks. */
    if(Os_TraceIsArtiSpinlockHookEnabled() != 0u)
    {
      OS_CALL_ARTI_TRACE(OS_SPINLOCK, (Os_CoreGetId(core)), OsSpinlock_Locked, Spinlock->SpinlockId)
    }

    /* #80 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */

    /* #90 Resume interrupts. */
    Os_IntResume(&interruptState);                                                                                      /* SBSW_OS_FC_POINTER2LOCAL */
  }
}

/***********************************************************************************************************************
 * Os_TraceSpinlockReleased
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
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceSpinlockReleased,
(
    P2CONST(Os_TraceSpinlockConfigType, AUTOMATIC, OS_CONST) Spinlock
))
{
  /* #10 If the timing hook for spinlock release or the ARTI spinlock hook is enabled: */
  if((Os_TraceIsRelSpinlockTimingHookEnabled() != 0u) || (Os_TraceIsArtiSpinlockHookEnabled() != 0u))                   /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    Os_IntStateType interruptState;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* No NULL_PTR expected by this function */
    Os_Assert((Os_StdReturnType)(Spinlock != NULL_PTR));                                                                /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */

    /* #20 Suspend interrupts. */
    Os_IntSuspend(&interruptState);                                                                                     /* SBSW_OS_FC_POINTER2LOCAL */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 If the timing hook for spinlock release is enabled: */
    if (Os_TraceIsRelSpinlockTimingHookEnabled() != 0u)
    {
      if (Spinlock->SpinlockId < Os_TraceGetNumberOfConfiguredSpinlocks())                                              /* COV_OS_HALPLATFORMTESTEDASMULTICORE */ /* COMP_WARN_OS_CW_005 */
     {
        /* #50 For configured spinlocks call: OS_VTH_REL_SPINLOCK(). */
        OS_VTH_REL_SPINLOCK((Spinlock->SpinlockId), (Os_CoreGetId(core)))                                               /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */
      }
      else
      {
        /* #60 For internal spinlocks call: OS_VTH_REL_ISPINLOCK(). */
        OS_VTH_REL_ISPINLOCK((Spinlock->SpinlockId), (Os_CoreGetId(core)))                                              /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */
      }
    }
    /* #70 If the ARTI spinlock hook is enabled, call the ARTI hook for releasing spinlocks. */
    if (Os_TraceIsArtiSpinlockHookEnabled() != 0u)
    {
      OS_CALL_ARTI_TRACE(OS_SPINLOCK, (Os_CoreGetId(core)), OsSpinlock_Released, Spinlock->SpinlockId)
    }

    /* #80 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */

    /* #90 Resume interrupts. */
    Os_IntResume(&interruptState);                                                                                      /* SBSW_OS_FC_POINTER2LOCAL */
  }
}


/***********************************************************************************************************************
 * Os_TraceInterruptsGlobalDisabled
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceInterruptsGlobalDisabled,
(
    void
))
{
  /* - #10 Check if timings hooks are enabled. */
  if(Os_TraceIsDisabledIntTimingHookEnabled() != 0u)                                                                    /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    coreAsr = Os_Core2AsrCore(Os_CoreGetCurrentCore());                                                                 /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* - #20 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* - #30 Call the vector timing hook for interrupt locks: OS_VTH_DISABLEDINT(). */
    OS_VTH_DISABLEDINT((OS_VTHP_ALLINTERRUPTS), (Os_CoreGetId(Os_CoreGetCurrentCore())))                                /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */

    /* - #40 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceInterruptsLevelDisabled
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceInterruptsLevelDisabled,
(
    void
))
{
  /* - #10 Check if timing hook for interrupt disabling is enabled. */
  if(Os_TraceIsDisabledIntTimingHookEnabled() != 0u)                                                                    /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    coreAsr = Os_Core2AsrCore(Os_CoreGetCurrentCore());                                                                 /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* - #20 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* - #30 Call the vector timing hook for interrupt locks: OS_VTH_DISABLEDINT(). */
    OS_VTH_DISABLEDINT((OS_VTHP_CAT2INTERRUPTS), (Os_CoreGetId(Os_CoreGetCurrentCore())))                               /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */

    /* - #40 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceInterruptsGlobalEnabled
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceInterruptsGlobalEnabled,
(
    void
))
{
  /* - #10 Check if timing hook for interrupt enabling is enabled. */
  if(Os_TraceIsEnabledIntTimingHookEnabled() != 0u)                                                                     /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    coreAsr = Os_Core2AsrCore(Os_CoreGetCurrentCore());                                                                 /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* - #20 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* - #30 Call the vector timing hook for interrupt locks: OS_VTH_ENABLEDINT(). */
    OS_VTH_ENABLEDINT((OS_VTHP_ALLINTERRUPTS), (Os_CoreGetId(Os_CoreGetCurrentCore())))                                 /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */

    /* - #40 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceInterruptsLevelEnabled
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceInterruptsLevelEnabled,
(
    void
))
{
  /* #10 Check if timing hooks are enabled: */
  if(Os_TraceIsEnabledIntTimingHookEnabled() != 0u)                                                                     /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    coreAsr = Os_Core2AsrCore(Os_CoreGetCurrentCore());                                                                 /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* - #20 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* - #30 Call the vector timing hook for interrupt locks: OS_VTH_ENABLEDINT(). */
    OS_VTH_ENABLEDINT((OS_VTHP_CAT2INTERRUPTS), (Os_CoreGetId(Os_CoreGetCurrentCore())))                                /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112_Hook */ /* SBSW_OS_TRACE_COREGETID_001 */

    /* - #40 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceOrtiApiEntry
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TraceOrtiApiEntry,
(
    OsOrtiApiIdType ServiceId
))
{
  /* - #10 Log the start of the currently running OS Api if Orti is enabled. */
  if (Os_TraceIsOrtiServiceTracingEnabled() != 0u)                                                                      /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    core = Os_CoreGetCurrentCore();
    Os_CoreGetTrace(core)->OrtiCurrentServiceId = ((uint8) ServiceId) + (uint8)1;                                       /* SBSW_OS_TRACE_COREGETTRACE_001 */ /* SBSW_OS_TRACE_COREGETTRACE_002 */
  }

}

/***********************************************************************************************************************
 * Os_TraceOrtiApiExit
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TraceOrtiApiExit,
(
    OsOrtiApiIdType ServiceId
))
{
  /* #10 Log the end of the currently running OS Api if Orti is enabled. */
  if (Os_TraceIsOrtiServiceTracingEnabled() != 0u)                                                                      /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    core = Os_CoreGetCurrentCore();
    Os_CoreGetTrace(core)->OrtiCurrentServiceId = ((uint8) ServiceId);                                                  /* SBSW_OS_TRACE_COREGETTRACE_001 */ /* SBSW_OS_TRACE_COREGETTRACE_002 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetApplicationID
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetApplicationID,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetApplicationID);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetApplicationID(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetApplicationID_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetApplicationID
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetApplicationID,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetApplicationID);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetApplicationID(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetApplicationID_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}



/***********************************************************************************************************************
 * Os_TraceApiEntryGetISRID
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetISRID,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetISRID);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetISRID(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetISRID_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetISRID
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetISRID,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetISRID);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetISRID(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetISRID_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryCallTrustedFunction
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryCallTrustedFunction,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdCallTrustedFunction);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function CallTrustedFunction(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_CallTrustedFunction_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitCallTrustedFunction
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitCallTrustedFunction,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdCallTrustedFunction);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function CallTrustedFunction(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_CallTrustedFunction_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryCheckISRMemoryAccess
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryCheckISRMemoryAccess,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdCheckISRMemoryAccess);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function CheckISRMemoryAccess(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_CheckISRMemoryAccess_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitCheckISRMemoryAccess
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitCheckISRMemoryAccess,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdCheckISRMemoryAccess);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function CheckISRMemoryAccess(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_CheckISRMemoryAccess_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryCheckTaskMemoryAccess
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryCheckTaskMemoryAccess,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdCheckTaskMemoryAccess);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function CheckTaskMemoryAccess(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_CheckTaskMemoryAccess_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitCheckTaskMemoryAccess
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitCheckTaskMemoryAccess,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdCheckTaskMemoryAccess);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function CheckTaskMemoryAccess(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_CheckTaskMemoryAccess_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryCheckObjectAccess
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryCheckObjectAccess,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdCheckObjectAccess);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function CheckObjectAccess(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_CheckObjectAccess_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitCheckObjectAccess
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitCheckObjectAccess,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdCheckObjectAccess);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function CheckObjectAccess(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_CheckObjectAccess_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryCheckObjectOwnership
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryCheckObjectOwnership,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdCheckObjectOwnership);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function CheckObjectOwnership(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_CheckObjectOwnership_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitCheckObjectOwnership
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitCheckObjectOwnership,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdCheckObjectOwnership);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function CheckObjectOwnership(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_CheckObjectOwnership_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryStartScheduleTableRel
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryStartScheduleTableRel,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdStartScheduleTableRel);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function StartScheduleTableRel(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_StartScheduleTableRel_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitStartScheduleTableRel
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitStartScheduleTableRel,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdStartScheduleTableRel);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function StartScheduleTableRel(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_StartScheduleTableRel_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryStartScheduleTableAbs
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryStartScheduleTableAbs,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdStartScheduleTableAbs);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function StartScheduleTableAbs(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_StartScheduleTableAbs_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitStartScheduleTableAbs
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitStartScheduleTableAbs,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdStartScheduleTableAbs);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function StartScheduleTableAbs(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_StartScheduleTableAbs_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryStopScheduleTable
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryStopScheduleTable,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdStopScheduleTable);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function StopScheduleTable(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_StopScheduleTable_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitStopScheduleTable
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitStopScheduleTable,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdStopScheduleTable);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function StopScheduleTable(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_StopScheduleTable_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}




/***********************************************************************************************************************
 * Os_TraceApiEntryNextScheduleTable
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryNextScheduleTable,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdNextScheduleTable);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function NextScheduleTable(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_NextScheduleTable_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitNextScheduleTable
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitNextScheduleTable,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdNextScheduleTable);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function NextScheduleTable(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_NextScheduleTable_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryStartScheduleTableSynchron
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryStartScheduleTableSynchron,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdStartScheduleTableSynchron);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function StartScheduleTableSynchron(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_StartScheduleTableSynchron_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitStartScheduleTableSynchron
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitStartScheduleTableSynchron,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdStartScheduleTableSynchron);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function StartScheduleTableSynchron(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_StartScheduleTableSynchron_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntrySyncScheduleTable
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntrySyncScheduleTable,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdSyncScheduleTable);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function SyncScheduleTable(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_SyncScheduleTable_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitSyncScheduleTable
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitSyncScheduleTable,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdSyncScheduleTable);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function SyncScheduleTable(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_SyncScheduleTable_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntrySetScheduleTableAsync
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntrySetScheduleTableAsync,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdSetScheduleTableAsync);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function SetScheduleTableAsync(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_SetScheduleTableAsync_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitSetScheduleTableAsync
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitSetScheduleTableAsync,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdSetScheduleTableAsync);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function SetScheduleTableAsync(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_SetScheduleTableAsync_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetScheduleTableStatus
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetScheduleTableStatus,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetScheduleTableStatus);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetScheduleTableStatus(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetScheduleTableStatus_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetScheduleTableStatus
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetScheduleTableStatus,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetScheduleTableStatus);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetScheduleTableStatus(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetScheduleTableStatus_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryIncrementCounter
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryIncrementCounter,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdIncrementCounter);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function IncrementCounter(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_IncrementCounter_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitIncrementCounter
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitIncrementCounter,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdIncrementCounter);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function IncrementCounter(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_IncrementCounter_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetCounterValue
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetCounterValue,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetCounterValue);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetCounterValue(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetCounterValue_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetCounterValue
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetCounterValue,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetCounterValue);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetCounterValue(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetCounterValue_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetElapsedValue
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetElapsedValue,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetElapsedValue);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetElapsedValue(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetElapsedValue_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetElapsedValue
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetElapsedValue,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetElapsedValue);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetElapsedValue(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetElapsedValue_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryTerminateApplication
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryTerminateApplication,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdTerminateApplication);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function TerminateApplication(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_TerminateApplication_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitTerminateApplication
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitTerminateApplication,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdTerminateApplication);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function TerminateApplication(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_TerminateApplication_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryAllowAccess
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryAllowAccess,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdAllowAccess);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function AllowAccess(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_AllowAccess_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitAllowAccess
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitAllowAccess,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdAllowAccess);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function AllowAccess(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_AllowAccess_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}




/***********************************************************************************************************************
 * Os_TraceApiEntryGetApplicationState
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetApplicationState,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetApplicationState);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetApplicationState(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetApplicationState_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetApplicationState
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetApplicationState,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetApplicationState);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetApplicationState(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetApplicationState_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetNumberOfActivatedCores
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetNumberOfActivatedCores,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetNumberOfActivatedCores);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetNumberOfActivatedCores(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetNumberOfActivatedCores_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetNumberOfActivatedCores
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetNumberOfActivatedCores,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetNumberOfActivatedCores);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetNumberOfActivatedCores(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetNumberOfActivatedCores_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetCoreID
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetCoreID,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetCoreID);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetCoreID(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetCoreID_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetCoreID
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetCoreID,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetCoreID);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetCoreID(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetCoreID_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryStartCore
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryStartCore,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdStartCore);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function StartCore(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_StartCore_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitStartCore
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitStartCore,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdStartCore);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function StartCore(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_StartCore_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryStartNonAutosarCore
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryStartNonAutosarCore,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdStartNonAutosarCore);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function StartNonAutosarCore(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_StartNonAutosarCore_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitStartNonAutosarCore
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitStartNonAutosarCore,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdStartNonAutosarCore);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function StartNonAutosarCore(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_StartNonAutosarCore_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetSpinlock
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetSpinlock,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetSpinlock);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetSpinlock(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetSpinlock_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetSpinlock
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetSpinlock,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetSpinlock);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetSpinlock(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetSpinlock_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryReleaseSpinlock
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryReleaseSpinlock,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdReleaseSpinlock);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function ReleaseSpinlock(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ReleaseSpinlock_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitReleaseSpinlock
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitReleaseSpinlock,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdReleaseSpinlock);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function ReleaseSpinlock(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ReleaseSpinlock_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryTryToGetSpinlock
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryTryToGetSpinlock,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdTryToGetSpinlock);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function TryToGetSpinlock(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_TryToGetSpinlock_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitTryToGetSpinlock
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitTryToGetSpinlock,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdTryToGetSpinlock);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function TryToGetSpinlock(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_TryToGetSpinlock_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryShutdownAllCores
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryShutdownAllCores,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdShutdownAllCores);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function ShutdownAllCores(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ShutdownAllCores_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitShutdownAllCores
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitShutdownAllCores,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdShutdownAllCores);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function ShutdownAllCores(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ShutdownAllCores_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryControlIdle
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryControlIdle,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdControlIdle);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function ControlIdle(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ControlIdle_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitControlIdle
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitControlIdle,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdControlIdle);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function ControlIdle(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ControlIdle_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetCurrentApplicationID
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetCurrentApplicationID,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetCurrentApplicationID);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetCurrentApplicationID(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetCurrentApplicationID_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetCurrentApplicationID
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetCurrentApplicationID,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetCurrentApplicationID);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetCurrentApplicationID(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetCurrentApplicationID_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}




/***********************************************************************************************************************
 * Os_TraceApiEntryCallNonTrustedFunction
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryCallNonTrustedFunction,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdCallNonTrustedFunction);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function CallNonTrustedFunction(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_CallNonTrustedFunction_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitCallNonTrustedFunction
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitCallNonTrustedFunction,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdCallNonTrustedFunction);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function CallNonTrustedFunction(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_CallNonTrustedFunction_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetTaskStackUsage
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetTaskStackUsage,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetTaskStackUsage);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetTaskStackUsage(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetTaskStackUsage_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetTaskStackUsage
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetTaskStackUsage,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetTaskStackUsage);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetTaskStackUsage(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetTaskStackUsage_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetISRStackUsage
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetISRStackUsage,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetISRStackUsage);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetISRStackUsage(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetISRStackUsage_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetISRStackUsage
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetISRStackUsage,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetISRStackUsage);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetISRStackUsage(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetISRStackUsage_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetKernelStackUsage
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetKernelStackUsage,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetKernelStackUsage);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetKernelStackUsage(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetKernelStackUsage_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetKernelStackUsage
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetKernelStackUsage,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetKernelStackUsage);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetKernelStackUsage(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetKernelStackUsage_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetStartupHookStackUsage
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetStartupHookStackUsage,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetStartupHookStackUsage);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetStartupHookStackUsage(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetStartupHookStackUsage_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetStartupHookStackUsage
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetStartupHookStackUsage,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetStartupHookStackUsage);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetStartupHookStackUsage(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetStartupHookStackUsage_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetErrorHookStackUsage
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetErrorHookStackUsage,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetErrorHookStackUsage);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetErrorHookStackUsage(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetErrorHookStackUsage_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetErrorHookStackUsage
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetErrorHookStackUsage,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetErrorHookStackUsage);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetErrorHookStackUsage(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetErrorHookStackUsage_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetShutdownHookStackUsage
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetShutdownHookStackUsage,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetShutdownHookStackUsage);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetShutdownHookStackUsage(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetShutdownHookStackUsage_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetShutdownHookStackUsage
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetShutdownHookStackUsage,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetShutdownHookStackUsage);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetShutdownHookStackUsage(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetShutdownHookStackUsage_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetProtectionHookStackUsage
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetProtectionHookStackUsage,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetProtectionHookStackUsage);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetProtectionHookStackUsage(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetProtectionHookStackUsage_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetProtectionHookStackUsage
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetProtectionHookStackUsage,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetProtectionHookStackUsage);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetProtectionHookStackUsage(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetProtectionHookStackUsage_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryStartOS
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryStartOS,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdStartOS);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function StartOS(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_StartOS_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}



/***********************************************************************************************************************
 * Os_TraceApiEntryGetActiveApplicationMode
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetActiveApplicationMode,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetActiveApplicationMode);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetActiveApplicationMode(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetActiveApplicationMode_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetActiveApplicationMode
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetActiveApplicationMode,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetActiveApplicationMode);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetActiveApplicationMode(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetActiveApplicationMode_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryDisableAllInterrupts
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryDisableAllInterrupts,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdDisableAllInterrupts);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function DisableAllInterrupts(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_DisableAllInterrupts_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitDisableAllInterrupts
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitDisableAllInterrupts,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdDisableAllInterrupts);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function DisableAllInterrupts(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_DisableAllInterrupts_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryEnableAllInterrupts
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryEnableAllInterrupts,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdEnableAllInterrupts);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function EnableAllInterrupts(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_EnableAllInterrupts_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitEnableAllInterrupts
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitEnableAllInterrupts,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdEnableAllInterrupts);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function EnableAllInterrupts(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_EnableAllInterrupts_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntrySuspendAllInterrupts
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntrySuspendAllInterrupts,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdSuspendAllInterrupts);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function SuspendAllInterrupts(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_SuspendAllInterrupts_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitSuspendAllInterrupts
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitSuspendAllInterrupts,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdSuspendAllInterrupts);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function SuspendAllInterrupts(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_SuspendAllInterrupts_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryResumeAllInterrupts
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryResumeAllInterrupts,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdResumeAllInterrupts);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function ResumeAllInterrupts(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ResumeAllInterrupts_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitResumeAllInterrupts
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitResumeAllInterrupts,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdResumeAllInterrupts);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function ResumeAllInterrupts(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ResumeAllInterrupts_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntrySuspendOSInterrupts
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntrySuspendOSInterrupts,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdSuspendOSInterrupts);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function SuspendOSInterrupts(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_SuspendOSInterrupts_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitSuspendOSInterrupts
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitSuspendOSInterrupts,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdSuspendOSInterrupts);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function SuspendOSInterrupts(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_SuspendOSInterrupts_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryResumeOSInterrupts
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryResumeOSInterrupts,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdResumeOSInterrupts);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function ResumeOSInterrupts(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ResumeOSInterrupts_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitResumeOSInterrupts
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitResumeOSInterrupts,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdResumeOSInterrupts);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function ResumeOSInterrupts(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ResumeOSInterrupts_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryActivateTask
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryActivateTask,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdActivateTask);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function ActivateTask(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ActivateTask_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitActivateTask
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitActivateTask,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdActivateTask);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function ActivateTask(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ActivateTask_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryTerminateTask
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryTerminateTask,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdTerminateTask);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function TerminateTask(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_TerminateTask_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitTerminateTask
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitTerminateTask,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdTerminateTask);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function TerminateTask(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_TerminateTask_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}




/***********************************************************************************************************************
 * Os_TraceApiEntryChainTask
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryChainTask,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdChainTask);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function ChainTask(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ChainTask_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitChainTask
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitChainTask,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdChainTask);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function ChainTask(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ChainTask_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetTaskID
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetTaskID,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetTaskID);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetTaskID(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetTaskID_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetTaskID
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetTaskID,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetTaskID);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetTaskID(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetTaskID_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetTaskState
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetTaskState,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetTaskState);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetTaskState(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetTaskState_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetTaskState
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetTaskState,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetTaskState);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetTaskState(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetTaskState_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntrySchedule
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntrySchedule,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdSchedule);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function Schedule(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_Schedule_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitSchedule
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitSchedule,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdSchedule);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function Schedule(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_Schedule_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntrySetEvent
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntrySetEvent,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdSetEvent);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function SetEvent(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_SetEvent_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitSetEvent
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitSetEvent,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdSetEvent);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function SetEvent(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_SetEvent_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryClearEvent
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryClearEvent,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdClearEvent);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function ClearEvent(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ClearEvent_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitClearEvent
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitClearEvent,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdClearEvent);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function ClearEvent(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ClearEvent_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetEvent
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetEvent,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetEvent);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetEvent(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetEvent_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetEvent
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetEvent,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetEvent);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetEvent(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetEvent_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryWaitEvent
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryWaitEvent,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdWaitEvent);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function WaitEvent(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_WaitEvent_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitWaitEvent
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitWaitEvent,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdWaitEvent);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function WaitEvent(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_WaitEvent_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetAlarmBase
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetAlarmBase,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetAlarmBase);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetAlarmBase(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetAlarmBase_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetAlarmBase
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetAlarmBase,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetAlarmBase);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetAlarmBase(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetAlarmBase_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetAlarm
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetAlarm,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetAlarm);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetAlarm(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetAlarm_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetAlarm
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetAlarm,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetAlarm);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetAlarm(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetAlarm_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}




/***********************************************************************************************************************
 * Os_TraceApiEntrySetRelAlarm
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntrySetRelAlarm,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdSetRelAlarm);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function SetRelAlarm(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_SetRelAlarm_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitSetRelAlarm
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitSetRelAlarm,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdSetRelAlarm);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function SetRelAlarm(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_SetRelAlarm_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntrySetAbsAlarm
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntrySetAbsAlarm,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdSetAbsAlarm);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function SetAbsAlarm(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_SetAbsAlarm_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitSetAbsAlarm
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitSetAbsAlarm,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdSetAbsAlarm);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function SetAbsAlarm(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_SetAbsAlarm_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryCancelAlarm
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryCancelAlarm,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdCancelAlarm);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function CancelAlarm(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_CancelAlarm_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitCancelAlarm
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitCancelAlarm,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdCancelAlarm);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function CancelAlarm(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_CancelAlarm_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetResource
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetResource,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetResource);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetResource(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetResource_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetResource
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetResource,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetResource);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetResource(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetResource_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryReleaseResource
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryReleaseResource,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdReleaseResource);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function ReleaseResource(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ReleaseResource_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitReleaseResource
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitReleaseResource,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdReleaseResource);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function ReleaseResource(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ReleaseResource_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryEnterPreStartTask
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryEnterPreStartTask,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdEnterPreStartTask);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function EnterPreStartTask(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_EnterPreStartTask_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}



/***********************************************************************************************************************
 * Os_TraceApiEntryShutdownOS
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryShutdownOS,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdShutdownOS);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function ShutdownOS(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ShutdownOS_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitShutdownOS
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitShutdownOS,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdShutdownOS);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function ShutdownOS(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ShutdownOS_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetUnhandledIrq
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetUnhandledIrq,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetUnhandledIrq);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetUnhandledIrq(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetUnhandledIrq_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetUnhandledIrq
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetUnhandledIrq,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetUnhandledIrq);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetUnhandledIrq(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetUnhandledIrq_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetUnhandledExc
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetUnhandledExc,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetUnhandledExc);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetUnhandledExc(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetUnhandledExc_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetUnhandledExc
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetUnhandledExc,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetUnhandledExc);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetUnhandledExc(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetUnhandledExc_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}




/***********************************************************************************************************************
 * Os_TraceApiEntryDisableOSInterrupts
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryDisableOSInterrupts,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdDisableOSInterrupts);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function DisableOSInterrupts(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_DisableOSInterrupts_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitDisableOSInterrupts
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitDisableOSInterrupts,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdDisableOSInterrupts);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function DisableOSInterrupts(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_DisableOSInterrupts_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryEnableOSInterrupts
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryEnableOSInterrupts,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdEnableOSInterrupts);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function EnableOSInterrupts(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_EnableOSInterrupts_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitEnableOSInterrupts
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitEnableOSInterrupts,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdEnableOSInterrupts);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function EnableOSInterrupts(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_EnableOSInterrupts_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryReadPeripheral8
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryReadPeripheral8,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdReadPeripheral8);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function ReadPeripheral8(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ReadPeripheral8_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitReadPeripheral8
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitReadPeripheral8,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdReadPeripheral8);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function ReadPeripheral8(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ReadPeripheral8_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryReadPeripheral16
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryReadPeripheral16,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdReadPeripheral16);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function ReadPeripheral16(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ReadPeripheral16_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitReadPeripheral16
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitReadPeripheral16,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdReadPeripheral16);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function ReadPeripheral16(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ReadPeripheral16_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryReadPeripheral32
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryReadPeripheral32,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdReadPeripheral32);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function ReadPeripheral32(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ReadPeripheral32_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitReadPeripheral32
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitReadPeripheral32,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdReadPeripheral32);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function ReadPeripheral32(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ReadPeripheral32_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryWritePeripheral8
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryWritePeripheral8,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdWritePeripheral8);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function WritePeripheral8(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_WritePeripheral8_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitWritePeripheral8
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitWritePeripheral8,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdWritePeripheral8);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function WritePeripheral8(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_WritePeripheral8_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryWritePeripheral16
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryWritePeripheral16,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdWritePeripheral16);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function WritePeripheral16(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_WritePeripheral16_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitWritePeripheral16
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitWritePeripheral16,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdWritePeripheral16);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function WritePeripheral16(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_WritePeripheral16_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryWritePeripheral32
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryWritePeripheral32,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdWritePeripheral32);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function WritePeripheral32(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_WritePeripheral32_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitWritePeripheral32
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitWritePeripheral32,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdWritePeripheral32);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function WritePeripheral32(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_WritePeripheral32_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryModifyPeripheral8
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryModifyPeripheral8,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdModifyPeripheral8);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function ModifyPeripheral8(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ModifyPeripheral8_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitModifyPeripheral8
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitModifyPeripheral8,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdModifyPeripheral8);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function ModifyPeripheral8(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ModifyPeripheral8_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryModifyPeripheral16
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryModifyPeripheral16,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdModifyPeripheral16);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function ModifyPeripheral16(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ModifyPeripheral16_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitModifyPeripheral16
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitModifyPeripheral16,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdModifyPeripheral16);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function ModifyPeripheral16(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ModifyPeripheral16_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}




/***********************************************************************************************************************
 * Os_TraceApiEntryModifyPeripheral32
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryModifyPeripheral32,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdModifyPeripheral32);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function ModifyPeripheral32(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ModifyPeripheral32_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitModifyPeripheral32
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitModifyPeripheral32,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdModifyPeripheral32);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function ModifyPeripheral32(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ModifyPeripheral32_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetDetailedError
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetDetailedError,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetDetailedError);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetDetailedError(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetDetailedError_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetDetailedError
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetDetailedError,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetDetailedError);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetDetailedError(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetDetailedError_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryOSErrorGetServiceId
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryOSErrorGetServiceId,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdOSErrorGetServiceId);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function OSErrorGetServiceId(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_OSErrorGetServiceId_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitOSErrorGetServiceId
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitOSErrorGetServiceId,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdOSErrorGetServiceId);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function OSErrorGetServiceId(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_OSErrorGetServiceId_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryDisableInterruptSource
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryDisableInterruptSource,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdDisableInterruptSource);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function DisableInterruptSource(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_DisableInterruptSource_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitDisableInterruptSource
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitDisableInterruptSource,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdDisableInterruptSource);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function DisableInterruptSource(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_DisableInterruptSource_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryEnableInterruptSource
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryEnableInterruptSource,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdEnableInterruptSource);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function EnableInterruptSource(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_EnableInterruptSource_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitEnableInterruptSource
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitEnableInterruptSource,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdEnableInterruptSource);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function EnableInterruptSource(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_EnableInterruptSource_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryEnableInterruptsPreStart
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryEnableInterruptsPreStart,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdEnableInterruptsPreStart);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function EnableInterruptsPreStart(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_EnableInterruptsPreStart_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitEnableInterruptsPreStart
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitEnableInterruptsPreStart,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdEnableInterruptsPreStart);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function EnableInterruptsPreStart(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_EnableInterruptsPreStart_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryClearPendingInterrupt
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryClearPendingInterrupt,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdClearPendingInterrupt);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function ClearPendingInterrupt(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ClearPendingInterrupt_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitClearPendingInterrupt
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitClearPendingInterrupt,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdClearPendingInterrupt);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function ClearPendingInterrupt(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_ClearPendingInterrupt_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryIsInterruptSourceEnabled
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryIsInterruptSourceEnabled,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdIsInterruptSourceEnabled);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function IsInterruptSourceEnabled(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_IsInterruptSourceEnabled_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitIsInterruptSourceEnabled
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitIsInterruptSourceEnabled,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdIsInterruptSourceEnabled);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function IsInterruptSourceEnabled(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_IsInterruptSourceEnabled_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryIsInterruptPending
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryIsInterruptPending,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdIsInterruptPending);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function IsInterruptPending(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_IsInterruptPending_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitIsInterruptPending
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitIsInterruptPending,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdIsInterruptPending);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function IsInterruptPending(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_IsInterruptPending_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryCallFastTrustedFunction
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryCallFastTrustedFunction,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdCallFastTrustedFunction);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function CallFastTrustedFunction(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_CallFastTrustedFunction_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitCallFastTrustedFunction
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitCallFastTrustedFunction,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdCallFastTrustedFunction);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function CallFastTrustedFunction(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_CallFastTrustedFunction_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryBarrierSynchronize
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryBarrierSynchronize,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdBarrierSynchronize);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function BarrierSynchronize(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_BarrierSynchronize_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitBarrierSynchronize
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitBarrierSynchronize,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdBarrierSynchronize);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function BarrierSynchronize(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_BarrierSynchronize_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}




/***********************************************************************************************************************
 * Os_TraceApiEntryGetExceptionContext
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetExceptionContext,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetExceptionContext);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetExceptionContext(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetExceptionContext_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetExceptionContext
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetExceptionContext,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetExceptionContext);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetExceptionContext(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetExceptionContext_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntrySetExceptionContext
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntrySetExceptionContext,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdSetExceptionContext);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function SetExceptionContext(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_SetExceptionContext_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitSetExceptionContext
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitSetExceptionContext,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdSetExceptionContext);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function SetExceptionContext(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_SetExceptionContext_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryInitialEnableInterruptSources
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryInitialEnableInterruptSources,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdInitialEnableInterruptSources);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function InitialEnableInterruptSources(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_InitialEnableInterruptSources_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitInitialEnableInterruptSources
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitInitialEnableInterruptSources,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdInitialEnableInterruptSources);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function InitialEnableInterruptSources(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_InitialEnableInterruptSources_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetCoreStartState
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetCoreStartState,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetCoreStartState);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetCoreStartState(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetCoreStartState_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetCoreStartState
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetCoreStartState,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetCoreStartState);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetCoreStartState(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetCoreStartState_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetExceptionAddress
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetExceptionAddress,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetExceptionAddress);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetExceptionAddress(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetExceptionAddress_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetExceptionAddress
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetExceptionAddress,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetExceptionAddress);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetExceptionAddress(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetExceptionAddress_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 * Os_TraceApiEntryGetNonTrustedFunctionStackUsage
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiEntryGetNonTrustedFunctionStackUsage,
(
    void
))
{
  /* #10 Call the trace ORTI entry function. */
  Os_TraceOrtiApiEntry(OsOrtiApiIdGetNonTrustedFunctionStackUsage);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service start hook for API function GetNonTrustedFunctionStackUsage(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetNonTrustedFunctionStackUsage_Start, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}

/***********************************************************************************************************************
 * Os_TraceApiExitGetNonTrustedFunctionStackUsage
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceApiExitGetNonTrustedFunctionStackUsage,
(
    void
))
{
  /* #10 Call the trace ORTI exit function. */
  Os_TraceOrtiApiExit(OsOrtiApiIdGetNonTrustedFunctionStackUsage);

  /* #20 If the ARTI service hook is enabled: */
  if (Os_TraceIsArtiServiceHookEnabled() != 0u)                                                                         /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #30 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #40 Call the ARTI service return hook for API function GetNonTrustedFunctionStackUsage(). */
    OS_CALL_ARTI_TRACE(OS_SERVICECALLS, ((CoreIdType) Os_CoreGetId(core)),
          OsServiceCall_GetNonTrustedFunctionStackUsage_Return, (0u))

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */
  }
}


/***********************************************************************************************************************
 *  Os_TraceGetNumberOfConfiguredSpinlocks()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_TraceSpinlockIdType, OS_CODE, OS_ALWAYS_INLINE,                         /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceGetNumberOfConfiguredSpinlocks, ( void ))                                                                       /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
{
  return OS_TRACE_NUMBER_OF_CONFIGURED_SPINLOCKS;
}

/***********************************************************************************************************************
 * Os_TraceAppStart
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceAppStart,
(
    P2CONST(Os_TraceAppConfigType, AUTOMATIC, OS_CONST) App
))
{
  /* #10 If the ARTI application hook is enabled: */
  if (Os_TraceIsArtiApplHookEnabled() != 0u)                                                                            /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #20 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #30 Call the ARTI application start hook. */
    OS_CALL_ARTI_TRACE(OS_APPLICATION, ((CoreIdType) Os_CoreGetId(core)), OsApplication_Start, (App->Id))

    /* #40 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
  else
  {
    OS_IGNORE_UNREF_PARAM(App);
  }

}

/***********************************************************************************************************************
 * Os_TraceAppAllowAccess
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceAppAllowAccess,
(
    P2CONST(Os_TraceAppConfigType, AUTOMATIC, OS_CONST) App
))
{
  /* #10 If the ARTI application hook is enabled: */
  if (Os_TraceIsArtiApplHookEnabled() != 0u)                                                                            /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #20 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #30 Call the ARTI application access allowance hook. */
    OS_CALL_ARTI_TRACE(OS_APPLICATION, ((CoreIdType) Os_CoreGetId(core)), OsApplication_AllowAccess, (App->Id))

    /* #40 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
  else
  {
    OS_IGNORE_UNREF_PARAM(App);
  }

}

/***********************************************************************************************************************
 * Os_TraceAppKill
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceAppKill,
(
    P2CONST(Os_TraceAppConfigType, AUTOMATIC, OS_CONST) App,
    CONST(boolean,AUTOMATIC) Restarting
))
{
  /* #10 If the ARTI application hook is enabled: */
  if (Os_TraceIsArtiApplHookEnabled() != 0u)                                                                            /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #20 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #30 If the app is restarted, call the ARTI application restart hook. */
    if(Restarting == TRUE)
    {
      OS_CALL_ARTI_TRACE(OS_APPLICATION, ((CoreIdType) Os_CoreGetId(core)), OsApplication_Restart, (App->Id))
    }
    else
    {
      /* #40 Otherwise, call the ARTI application termination hook. */
      OS_CALL_ARTI_TRACE(OS_APPLICATION, ((CoreIdType) Os_CoreGetId(core)), OsApplication_Terminate, (App->Id))
    }

    /* #50 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
  else
  {
    OS_IGNORE_UNREF_PARAM(App);
    OS_IGNORE_UNREF_PARAM(Restarting);
  }
}


/***********************************************************************************************************************
 * Os_TraceProtHookEntry
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceProtHookEntry,
(
    CONST(StatusType, AUTOMATIC) Status
))
{
  /* #10 If the ARTI protection hook is enabled: */
  if (Os_TraceIsArtiProtHookEnabled() != 0u)                                                                            /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #20 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #30 Call the ARTI protection hook start hook. */
    OS_CALL_ARTI_TRACE(OS_PROTECTIONHOOK, ((CoreIdType) Os_CoreGetId(core)), ProtectionHookStart, Status)

    /* #40 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
  else
  {
    OS_IGNORE_UNREF_PARAM(Status);
  }

}
/***********************************************************************************************************************
 * Os_TraceProtHookExit
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TraceProtHookExit,
(
    CONST(ProtectionReturnType, AUTOMATIC) ProtRet
))
{
  /* #10 If the ARTI protection hook is enabled: */
  if (Os_TraceIsArtiProtHookEnabled() != 0u)                                                                            /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    core = Os_CoreGetCurrentCore();
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_TRACE_CORE2ASRCORE_001 */

    /* #20 Enter critical user section. */
    Os_CoreCriticalUserSectionEnter(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONENTER_001 */

    /* #30 Call the ARTI protection hook return hook. */
    OS_CALL_ARTI_TRACE(OS_PROTECTIONHOOK, ((CoreIdType) Os_CoreGetId(core)), ProtectionHookReturn, ProtRet)

    /* #40 Leave critical user section. */
    Os_CoreCriticalUserSectionLeave(coreAsr);                                                                           /* SBSW_OS_TRACE_CORECRITICALUSERSECTIONLEAVE_001 */
  }
  else
  {
    OS_IGNORE_UNREF_PARAM(ProtRet);
  }
}


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* OS_TRACE_H */

/* module specific MISRA deviations:
 */


/*!
 * \}
 */
/***********************************************************************************************************************
 *  END OF FILE: Os_Trace.h
 **********************************************************************************************************************/
