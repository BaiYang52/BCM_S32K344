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
 * \ingroup Os_Kernel
 * \addtogroup Os_Core
 * \{
 * \file
 * \brief       Implementation of the core control module.
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os.h.
 **********************************************************************************************************************/

                                                                                                                        /* PRQA S 0777, 0779, 0828  EOF */ /* MD_MSR_Rule5.1, MD_MSR_Rule5.2, MD_MSR_Dir1.1 */


#define OS_CORE_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */
#include "Os_Core_Cfg.h"
#include "Os_Core_Lcfg.h"
#include "Os_Core.h"

/* Os kernel module dependencies */
#include "OsInt.h"
#include "Os_Lcfg.h"
#include "Os_Common_Types.h"
#include "Os_Interrupt.h"
#include "Os_Thread_Types.h"
#include "Os_Thread.h"
#include "Os_Task_Types.h"
#include "Os_Task.h"
#include "Os_Scheduler.h"
#include "Os_Resource.h"
#include "Os_Hook.h"
#include "Os_TimingProtection.h"
#include "Os_XSignal.h"
#include "Os_Ioc.h"
#include "Os.h"

/* Os HAL dependencies */
#include "Os_Hal_Core.h"
#include "Os_Hal_Context_Lcfg.h"

/***********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/

/* Check the version of OS header file */
#if (  (OS_SW_MAJOR_VERSION != (2u)) \
    || (OS_SW_MINOR_VERSION != (78u)) \
    || (OS_SW_PATCH_VERSION != (0u))   )
# error "Vendor specific version numbers of Os_Core.c and Os.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (OS_CFG_MAJOR_VERSION != (2u)) \
    || (OS_CFG_MINOR_VERSION != (73u)) )
# error "Version numbers of Os_Core.c and Os_Cfg.h are inconsistent!"
#endif


/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define OS_START_SEC_CODE
#include "Os_MemMap_OsCode.h"                                                                                           /* PRQA S 5087 */ /* MD_MSR_MemMap */
/***********************************************************************************************************************
 *  Os_CoreInit()
 **********************************************************************************************************************/
/*! \brief        Initialize a given core data structure (Init-Step4).
 *  \details      Initialize high level OS functionality, which is not available in PreStartTask.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          Local core is an AUTOSAR core.
 *  \pre          Supervisor mode.
 *
 *  \trace        SPEC-63724
 **********************************************************************************************************************/
FUNC(OS_LOCAL_INLINE void, OS_CODE) Os_CoreInit(void);


/***********************************************************************************************************************
 *  Os_CoreShutdown()
 **********************************************************************************************************************/
/*! \brief          System internal shutdown of the local core.
 *  \details        This function does not perform access right checks.
 *
 *  \param[in]      Error         Status code to report to the shutdown hooks.
 *  \param[in]      Synchronize   Tells whether the core shall synchronize with other AUTOSAR cores during shutdown
 *                                (TRUE) or not (FALSE).
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode.
 *
 *  \trace          SPEC-64036, SPEC-63859
 **********************************************************************************************************************/
OS_LOCAL OS_FUNC_ATTRIBUTE_DECLARATION(void, OS_CODE, OS_NORETURN, Os_CoreShutdown,
(
  StatusType Error,
  boolean Synchronize
));


/***********************************************************************************************************************
 *  Os_CoreDetermineStartStatus()
 **********************************************************************************************************************/
/*! \brief          Calculates and returns the requested start mode.
 *  \details        If multiple cores request a start, system is freezed.
 *
 *  \param[in,out]  Core    Core to query. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_LOCAL_INLINE FUNC(void, OS_CODE) Os_CoreDetermineStartStatus
(
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) Core
);


/***********************************************************************************************************************
 *  Os_CoreReducedInit()
 **********************************************************************************************************************/
/*! \brief          Reduced core initialization (Init-Step2).
 *  \details        Minimal initialization to be able to use the interrupt API.
 *
 *  \param[in,out]  Core    Core to initialize. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 *  \pre            Master core initialization already performed.
 **********************************************************************************************************************/
OS_LOCAL_INLINE FUNC(void, OS_CODE) Os_CoreReducedInit
(
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) Core
);


/***********************************************************************************************************************
 *  Os_CorePreStartInit()
 **********************************************************************************************************************/
/*! \brief          Initialization which has to be performed before entering the first task (Pre-Start or IdleTask).
 *  \details        This function is called in Init-Step3 and Init-Step4 in order to handle the situation that
 *                  Init-Step3 is optional.
 *
 *  \param[in]      InitialStackRegion  Stack configuration for memory protection mechanism. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access is prevented by caller.
 *  \pre            Current core is an AUTOSAR core.
 **********************************************************************************************************************/
OS_LOCAL FUNC(void, OS_CODE) Os_CorePreStartInit
(
  P2CONST(Os_Hal_ContextStackConfigType, AUTOMATIC, OS_VAR_NOINIT) InitialStackRegion
);


/***********************************************************************************************************************
 *  Os_CoreHooksInit()
 **********************************************************************************************************************/
/*! \brief          Initialize hooks of the given core.
 *  \details        The function is called in Init-Step3 for the kernel application and in Init-Step4 for all
 *                  applications.
 *
 *  \param[in,out]  Core     The core to initialize. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different applications.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_LOCAL FUNC(void, OS_CODE) Os_CoreHooksInit
(
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) Core
);


/***********************************************************************************************************************
 *  Os_SystemInit()
 **********************************************************************************************************************/
/*! \brief          Initialize objects which don't have any core association (Init-Step2).
 *  \details        --no details--
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
FUNC(OS_LOCAL_INLINE void, OS_CODE) Os_SystemInit
(
  void
);


/***********************************************************************************************************************
 *  Os_CoreCheckIdleMode()
 **********************************************************************************************************************/
/*! \brief          Returns whether the given idle mode is valid (Not OS_CHECK_FAILED) or not (OS_CHECK_FAILED).
 *  \details        In case that extended checks are disabled, the default result of \ref Os_ErrExtendedCheck() is
 *                  returned.
 *
 *  \param[in]      IdleMode        The mode to check.
 *
 *  \retval         Not OS_CHECK_FAILED   If the given mode is valid.
 *  \retval         OS_CHECK_FAILED       If the given mode is not valid.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_CoreCheckIdleMode,
(
  IdleModeType IdleMode
));


/***********************************************************************************************************************
 *  Os_CoreGetStartState()
 **********************************************************************************************************************/
/*! \brief          Returns the start state of a given core.
 *  \details        --no details--
 *
 *  \param[in]      Core    The core to check. Parameter must not be NULL.
 *
 *  \return         The core start state.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_CoreStartStateType, OS_CODE, OS_ALWAYS_INLINE, Os_CoreGetStartState,
(
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) Core
));


/***********************************************************************************************************************
 *  Os_CoreCheckCoreIsInactive()
 **********************************************************************************************************************/
/*! \brief          Checks whether the given core is not activated.
 *  \details        --no details--
 *
 *  \param[in]      Core        The core to check. Parameter must not be NULL.
 *
 *  \retval         Not OS_CHECK_FAILED   If the given core is inactive.
 *  \retval         OS_CHECK_FAILED       If the given core is active.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_CoreCheckCoreIsInactive,
(
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) Core
));


/***********************************************************************************************************************
 *  Os_CoreGetTrue()
 **********************************************************************************************************************/
/*! \brief          Return TRUE.
 *  \details        This function allows stubbing of internal values during test.
 *
 *  \return         TRUE
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE boolean, OS_CODE, OS_ALWAYS_INLINE, Os_CoreGetTrue, (void));


/***********************************************************************************************************************
 *  Os_CoreIdle()
 **********************************************************************************************************************/
/*! \brief          Kernel's idle behavior.
 *  \details        Performs an endless loop until it is interrupted.
 *
 *  \context        TASK|OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 *
 *  \note           Function is wrongly displayed in doxygen report (call tree and implementation steps are incorrect).
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_CoreIdle, (void));


/***********************************************************************************************************************
 *  Os_CoreGetStatus()
 **********************************************************************************************************************/
/*! \brief          Returns the status data of a core.
 *  \details        --no details--
 *
 *  \param[in]      Core        The core to query. Parameter must not be NULL.
 *
 *  \return         The core status object.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE P2VAR(volatile Os_CoreStatusType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE,
OS_ALWAYS_INLINE, Os_CoreGetStatus,
(
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) Core
));


/***********************************************************************************************************************
 *  Os_CoreCheckOsIsNotStarted()
 **********************************************************************************************************************/
/*! \brief          Checks whether the OS is not started on given core.
 *  \details        --no details--
 *
 *  \param[in]      Core        The core to check. Parameter must not be NULL.
 *
 *  \retval         Not OS_CHECK_FAILED   The OS is not started.
 *  \retval         OS_CHECK_FAILED       The OS is started.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreCheckOsIsNotStarted,
(
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) Core
));


/***********************************************************************************************************************
 *  Os_CoreCheckOsIsInitializing()
 **********************************************************************************************************************/
/*! \brief          Checks whether the OS is in initialization phase.
 *  \details        --no details--
 *
 *  \param[in]      Core        The core to check. Parameter must not be NULL.
 *
 *  \retval         Not OS_CHECK_FAILED   The OS is in initialization phase.
 *  \retval         OS_CHECK_FAILED       The OS is not in initialization phase.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreCheckOsIsInitializing,
(
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) Core
));


/***********************************************************************************************************************
 *  Os_CoreCheckOsHasPreStartTask()
 **********************************************************************************************************************/
/*! \brief          Returns whether the Os has any Pre-Start Task configured or not.
 *  \details        --no details--
 *
 *  \retval         Not OS_CHECK_FAILED   Pre-Start Task configured.
 *  \retval         OS_CHECK_FAILED       Pre-Start Task not configured.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreCheckOsHasPreStartTask,
(
  void
));


/***********************************************************************************************************************
 *  Os_CoreCheckHasPreStartTask()
 **********************************************************************************************************************/
/*! \brief          Returns whether the given core has a Pre-Start Task configured or not.
 *  \details        --no details--
 *
 *
 *  \param[in]      Core        The core to check. Parameter must not be NULL.
 *
 *  \retval         Not OS_CHECK_FAILED   Pre-Start Task configured.
 *  \retval         OS_CHECK_FAILED       Pre-Start Task not configured.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreCheckHasPreStartTask,
(
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) Core
));


/***********************************************************************************************************************
 *  Os_CoreAsrGetApplicationMode()
 **********************************************************************************************************************/
/*! \brief          Returns the active application mode.
 *  \details        --no details--
 *
 *  \param[in]      Core  The core to query. Parameter must not be NULL.
 *
 *  \return         The active application mode.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE AppModeType, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreAsrGetApplicationMode,
(
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) Core
));


/***********************************************************************************************************************
 *  Os_CoreAsrCombineApplicationMode()
 **********************************************************************************************************************/
/*! \brief          Combines the given application mode with the active application mode on this core.
 *  \details        --no details--
 *
 *  \param[in,out]  Core              The core to query. Parameter must not be NULL.
 *  \param[in]      ApplicationMode   The application mode to combine.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreAsrCombineApplicationMode,
(
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) Core,
  AppModeType ApplicationMode
));


/***********************************************************************************************************************
 *  Os_CoreCheckIsAppModeValid()
 **********************************************************************************************************************/
/*! \brief          Returns whether the given application mode is valid or not.
 *  \details        Application mode is valid if exactly one bit is set.
 *
 *  \param[in]      Mode        The app mode to check.
 *
 *  \retval         Not OS_CHECK_FAILED   Given mode is valid.
 *  \retval         OS_CHECK_FAILED       Given mode is invalid.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreCheckIsAppModeValid,
(
  AppModeType Mode
));


/***********************************************************************************************************************
 *  Os_SystemInterruptHandlingInit()
 **********************************************************************************************************************/
/*! \brief        This function initializes interrupt handling in hardware for the system wide interrupts.
 *  \details
 *  Initialization includes:
 *    - Binding of interrupt sources to cores.
 *    - Setting the interrupt priority of each configured ISR.
 *
 *  \param[in]    SystemCfg  The SystemConfiguration which contains the interrupt handling initialization information.
 *                           Parameter must not be NULL.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access is prevented by caller.
 **********************************************************************************************************************/
OS_LOCAL FUNC(void, OS_CODE) Os_SystemInterruptHandlingInit
(
  P2CONST(Os_SystemConfigType, AUTOMATIC, OS_CONST) SystemCfg
);


/***********************************************************************************************************************
 *  Os_CoreInterruptHandlingInit()
 **********************************************************************************************************************/
/*! \brief        This function initializes interrupt handling in hardware.
 *  \details
 *  Initialization includes:
 *    - Binding of interrupt sources to cores.
 *    - Setting the interrupt priority of each configured ISR.
 *
 *  \param[in]    Core  The core which contains the interrupt handling initialization information.
 *                      Parameter must not be NULL.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          Concurrent access is prevented by caller.
 **********************************************************************************************************************/
OS_LOCAL FUNC(void, OS_CODE) Os_CoreInterruptHandlingInit
(
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) Core
);


/***********************************************************************************************************************
 *  Os_CoreEmulateMaster()
 **********************************************************************************************************************/
/*! \brief        This function performs the boot synchronization on the master side.
 *  \details      For all auto-start cores a 2-stage synchronization approach is applied in series.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          Called on the master core
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_CoreEmulateMaster, (void));


/***********************************************************************************************************************
 *  Os_CoreEmulateSlaveWaitForHardwareInit()
 **********************************************************************************************************************/
/*! \brief        This function performs the boot synchronization on the slave side for hardware initialization.
 *  \details      Performs the echo until the running signal is received from the master core. For non-auto-started
 *                core there is nothing to be done.
 *
 *  \param[in]    Id  The Id of this core, which should emulates slave. The Id must smaller than OS_COREID_COUNT.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          Called on the slave core
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_CoreEmulateSlaveWaitForHardwareInit,
(
  CoreIdType Id
));


/***********************************************************************************************************************
 *  Os_CoreEmulateSlaveWaitForStartCore()
 **********************************************************************************************************************/
/*! \brief        This function performs the boot synchronization on the slave side for StartCore.
 *  \details      Performs a loop until the running signal is received from the master core. For non-auto-started
 *                core there is nothing to be done.
 *
 *  \param[in]    Id  The Id of this core, which should emulates slave. The Id must smaller than OS_COREID_COUNT.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          Called on the slave core
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_CoreEmulateSlaveWaitForStartCore,
(
  CoreIdType Id
));


/***********************************************************************************************************************
 *  Os_CoreGetInitHook()
 **********************************************************************************************************************/
/*! \brief          Returns the Init Hook of the core.
 *  \details        --no details--
 *
 *  \param[in]      Core    The core to query. Parameter must not be NULL.
 *
 *  \return         Init Hook of the core.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE P2CONST(Os_HookInitHookConfigType, AUTOMATIC, OS_CONST), OS_CODE,
OS_ALWAYS_INLINE, Os_CoreGetInitHook,
(
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) Core
));


/***********************************************************************************************************************
 *  Os_CoreCheckExceptionContextReadingSupported()
 **********************************************************************************************************************/
/*! \brief          Returns whether the HAL supports exception context reading (Not OS_CHECK_FAILED) or not
 *                  (OS_CHECK_FAILED).
 *  \details        In case that extended checks are disabled, the default result of \ref Os_ErrExtendedCheck() is
 *                  returned.
 *
 *  \retval         Not OS_CHECK_FAILED   If the HAL supports exception context reading.
 *  \retval         OS_CHECK_FAILED       If the HAL does not support exception context reading.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreCheckExceptionContextReadingSupported,
(
  void
));


/***********************************************************************************************************************
 *  Os_CoreCheckExceptionContextModificationSupported()
 **********************************************************************************************************************/
/*! \brief          Returns whether the HAL supports exception context modification (Not OS_CHECK_FAILED) or not
 *                  (OS_CHECK_FAILED).
 *  \details        In case that extended checks are disabled, the default result of \ref Os_ErrExtendedCheck() is
 *                  returned.
 *
 *  \retval         Not OS_CHECK_FAILED   If the HAL supports exception context modification.
 *  \retval         OS_CHECK_FAILED       If the HAL does not support exception context modification.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreCheckExceptionContextModificationSupported,
(
  void
));


/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Os_CoreReducedInit()
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
 *
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
FUNC(OS_LOCAL_INLINE void, OS_CODE) Os_CoreReducedInit
(
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) Core
)
{
  P2VAR(Os_CoreAsrType, AUTOMATIC, OS_VAR_NOINIT) coreDyn;
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) idleThread;
  P2CONST(Os_TpConfigType, AUTOMATIC, OS_CONST) tpConfig;
  P2CONST(Os_TpThreadConfigType, AUTOMATIC, OS_CONST) currentTp;
  P2VAR(volatile Os_CoreAsrStatusType, AUTOMATIC, OS_VAR_NOINIT) coreStatus;

  coreDyn =  Os_CoreAsrGetDyn(Core);                                                                                    /* SBSW_OS_FC_PRECONDITION */
  tpConfig = Os_CoreAsrGetTimingProtection(Core);                                                                       /* SBSW_OS_FC_PRECONDITION */
  idleThread = Os_TaskGetThread(Core->IdleTask);                                                                        /* SBSW_OS_CORE_TASKGETTHREAD_001 */
  currentTp = Os_ThreadGetTpConfig(idleThread);                                                                         /* SBSW_OS_FC_PRECONDITION */
  coreStatus = Os_CoreAsrGetAsrStatus(Core);                                                                            /* SBSW_OS_FC_PRECONDITION */

  /* #10 Initialize critical user section counter. */
  coreStatus->CriticalUserSection = 0;                                                                                  /* SBSW_OS_CORE_COREASR_STATUS_002 */

  /* #20 Initialize hardware specific stuff. */
  Os_Hal_CoreInit(Core->HwConfig);                                                                                      /* SBSW_OS_CORE_HAL_COREINIT_001 */

  /* #30 Initialize core variables to be able to handle cat2 ISRs.
   *     This is needed to handle the Timing Protection ISR. */
  coreDyn->InterruptedThreadsCounter = 0;                                                                               /* SBSW_OS_CORE_COREASR_DYN_001 */

  /* #40 Initialize timing protection. This is needed because interrupt API may check for blocking time. */
  Os_TpReducedInit(tpConfig, idleThread);                                                                               /* SBSW_OS_CORE_TPREDUCEDINIT_001 */
  Os_TpThreadInit(tpConfig, currentTp);                                                                                 /* SBSW_OS_CORE_TPTHREADINIT_001 */

  /* #50 Initialize interrupt API. */
  Os_InterruptInit(&(coreDyn->IntApiState));                                                                            /* SBSW_OS_CORE_INTERRUPTINIT_001 */

  /* #60 Set the current thread pointer to the idle task. */
  /*     So we have a valid thread reference for the interrupt API. */
  Os_CoreSetThread(idleThread);                                                                                         /* SBSW_OS_CORE_TASKSETTHREAD_001 */

  /* #70 Set the current stack pointer. */
  Os_CoreSetCurrentStack(idleThread->Stack, idleThread->Core);                                                          /* SBSW_OS_CORE_SETCURRENTSTACK_001 */

  /* #80 Call the respective initialization function of the idle thread. */
  Os_ThreadInit2(idleThread);                                                                                           /* SBSW_OS_CORE_THREADINIT2_001 */

  /* #90 Call the initialization function of the error subcomponent. */
  Os_ErrInit(&(coreDyn->LastError));                                                                                    /* SBSW_OS_CORE_ERRINIT_001 */

  /* #100 Initialize core variables to be able to use the Os_GetUnhandled*() API. */
  coreDyn->UnhandledIrqReported = OS_UNHANDLEDEVENTREPORT_NOTAVAILABLE;                                                 /* SBSW_OS_CORE_COREASR_DYN_001 */
  coreDyn->UnhandledExcReported = OS_UNHANDLEDEVENTREPORT_NOTAVAILABLE;                                                 /* SBSW_OS_CORE_COREASR_DYN_001 */
}


/***********************************************************************************************************************
 *  Os_CorePreStartInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
OS_LOCAL FUNC(void, OS_CODE) Os_CorePreStartInit
(
  P2CONST(Os_Hal_ContextStackConfigType, AUTOMATIC, OS_VAR_NOINIT) InitialStackRegion
)
{
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) core;

  core = Os_Core2AsrCore(Os_CoreGetCurrentCore());                                                                      /* SBSW_OS_CORE_CORE2ASRCORE_002 */

  /* #10 Initialize core's memory protection. */
  Os_MpCoreInit(core->MemoryProtection, InitialStackRegion);                                                            /* SBSW_OS_CORE_MPCOREINIT_001 */

  /* #20 Initialize system hooks. */
  Os_CoreHooksInit(core);                                                                                               /* SBSW_OS_CORE_COREHOOKSINIT_001 */

  /* #30 Initialize timing protection with the Idle Task. */
  Os_TpInit(Os_CoreAsrGetTimingProtection(core));                                                                       /* SBSW_OS_CORE_TPINIT_001 */ /* SBSW_OS_CORE_COREASRGETTIMINGPROTECTION_001 */


  /* #40 Initialize scheduler. */
  Os_SchedulerInit(core->Scheduler, core->IdleTask);                                                                    /* SBSW_OS_CORE_SCHEDULERINIT_001 */
}


/***********************************************************************************************************************
 *  Os_CoreHooksInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_LOCAL FUNC(void, OS_CODE) Os_CoreHooksInit
(
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) Core
)
{
  P2VAR(Os_CoreAsrType, AUTOMATIC, OS_VAR_NOINIT) coreDyn;

  coreDyn = Os_CoreAsrGetDyn(Core);                                                                                     /* SBSW_OS_FC_PRECONDITION */

  /* #10 Initialize hook nesting state of the given core. */
  Os_HookInfoInit(&(coreDyn->HookInfo));                                                                                /* SBSW_OS_CORE_HOOKINFOINIT_001 */

  {
    Os_ObjIdx_IteratorType hookIdx;

    /* #20 For each hook on the given core: */
    for(hookIdx = 0; hookIdx < (Os_ObjIdx_IteratorType) Core->HookCount; ++hookIdx)                                     /* FETA_OS_CONSTANTASFUNCTIONPARAMETER */
    {
      /* #30 Initialize the hook. */
      Os_HookInit(Core->HookRefs[hookIdx]);                                                                             /* SBSW_OS_CORE_HOOKINIT_001 */
    }
  }
}


/***********************************************************************************************************************
 *  Os_SystemInit()
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
 *
 */
/* PRQA S 6050,  6080 1 */ /* MD_MSR_STCAL */ /* MD_MSR_STMIF */
FUNC(OS_LOCAL_INLINE void, OS_CODE) Os_SystemInit
(
  void
)
{
  Os_ObjIdx_IteratorType spinlockId;
  P2CONST(Os_SystemConfigType, AUTOMATIC, OS_CONST) systemCfg;

  systemCfg = Os_SystemGet();

  /* #10 Initialize the user spinlocks. */
  for(spinlockId = 0; spinlockId < (Os_ObjIdx_IteratorType) systemCfg->SpinlockCount; ++spinlockId)                     /* FETA_OS_CONSTDATA */
  {
    Os_SpinlockInit(systemCfg->SpinlockRefs[spinlockId]);                                                               /* SBSW_OS_CORE_SPINLOCKINIT_001 */
  }

  /* #20 For each core: */
  {
    CoreIdType coreId;
    for(coreId = (CoreIdType)0; coreId < OS_COREID_COUNT; coreId++)                                                     /* PRQA S 4332,  4527 */ /* MD_Os_Rule10.5_4332,  MD_Os_Rule10.1_4527 */ /* FETA_OS_CONSTDATA */
    {
      CoreIdType statusId;
      P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) coreIt;
      P2VAR(volatile Os_CoreStatusType, AUTOMATIC, OS_VAR_NOINIT) status;

      coreIt = Os_CoreId2Core(coreId);
      status = Os_CoreGetStatus(coreIt);                                                                                /* SBSW_OS_CORE_COREGETSTATUS_001 */

      /* #30 Set initial core state.*/
      status->ActivationState = OS_CORESTATE_INACTIVE;                                                                  /* SBSW_OS_CORE_COREGETSTATUS_002 */

      for(statusId = (CoreIdType)0; statusId < OS_COREID_COUNT; ++statusId)                                             /* PRQA S 4332,  4527 */ /* MD_Os_Rule10.5_4332,  MD_Os_Rule10.1_4527 */
      {
        status->CoreStartRequests[statusId] = OS_CORESTATE_INACTIVE;                                                    /* SBSW_OS_CORE_COREGETSTATUS_002 */
      }

      /* #40 If the core is an AUTOSAR core: */
      if(Os_CoreIsAsrCore(coreIt) != 0u)                                                                                /* SBSW_OS_CORE_COREISASRCORE_001 */
      {
        P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) asrCore;
        P2VAR(Os_CoreAsrStatusType volatile, AUTOMATIC, OS_VAR_NOINIT) asrStatus;
        Os_ObjIdx_IteratorType appIterator;

        asrCore = Os_Core2AsrCore(coreIt);                                                                              /* SBSW_OS_CORE_CORE2ASRCORE_001 */
        asrStatus = Os_CoreAsrGetAsrStatus(asrCore);                                                                    /* SBSW_OS_CORE_COREASR_STATUS_001 */

        /* #50 Set initial AUTOSAR core status. */
        asrStatus->OsState = OS_COREOSSTATE_INITIALZE;                                                                  /* SBSW_OS_CORE_COREASR_STATUS_002 */
        asrStatus->ApplicationMode = DONOTCARE;                                                                         /* SBSW_OS_CORE_COREASR_STATUS_002 */

        /* #60 Initialize core barrier. */
        Os_BarrierInit(asrCore->Barrier);                                                                               /* SBSW_OS_CORE_BARRIERINIT_001 */

        /* #70 If user configured barriers exist */
        if (Os_BarrierIsEnabled() != 0u)                                                                                /* PRQA S 2991, 2995 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule2.2_2995 */
        {
          Os_ObjIdx_IteratorType index;

          /* #80 For all configured barriers */
          for (index = 0; index < (Os_ObjIdx_IteratorType) asrCore->BarrierCount; index++)                              /* FETA_OS_CONSTDATA */
          {
            /* #90 If barrier reference is a non NULL_PTR (core participates in the indexed barrier) */
            if (asrCore->BarrierRefs[index] != NULL_PTR)
            {
              /* #100 Initialize the barrier */
              Os_BarrierInit(asrCore->BarrierRefs[index]);                                                              /* SBSW_OS_CORE_BARRIERINIT_002 */
            }
          }
        }

        Os_AppSetNotAccessible(asrCore->KernelApp);                                                                     /* SBSW_OS_CORE_APPSETNOTACCESSIBLE_001 */

        /* #110 Initialize the OS state of each application AUTOSAR core as 'not started'. */
        for(appIterator = 0; appIterator < (Os_ObjIdx_IteratorType) asrCore->AppCount; appIterator++)                   /* FETA_OS_CONSTDATA */
        {
          Os_AppSetNotAccessible(asrCore->AppRefs[appIterator]);                                                        /* SBSW_OS_CORE_APPSETNOTACCESSIBLE_002 */
        }
      }
    }
  }
}


/***********************************************************************************************************************
 *  Os_CoreInit()
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
 *
 *
 *
 *
 *
 */
/* PRQA S 6010, 6050 1 */ /* MD_MSR_STPTH, MD_MSR_STCAL */
FUNC(OS_LOCAL_INLINE void, OS_CODE) Os_CoreInit(void)
{
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) core;
  P2VAR(Os_CoreAsrType, AUTOMATIC, OS_VAR_NOINIT) coreDyn;
  P2VAR(volatile Os_CoreAsrStatusType, AUTOMATIC, OS_VAR_NOINIT) coreStatus;
  Os_ObjIdx_IteratorType index;

  core = Os_ThreadGetCore(Os_CoreGetThread());                                                                          /* SBSW_OS_CORE_THREADGETCORE_001 */
  coreDyn = Os_CoreAsrGetDyn(core);                                                                                     /* SBSW_OS_CORE_COREASRGETDYN_001 */
  coreStatus = Os_CoreAsrGetAsrStatus(core);                                                                            /* SBSW_OS_CORE_COREASR_STATUS_003 */

  /* #10 Initialize dynamic core attributes. */
  coreStatus->OsState = OS_COREOSSTATE_STARTED;                                                                         /* SBSW_OS_CORE_COREASR_STATUS_002 */

  coreDyn->IdleMode = IDLE_NO_HALT;                                                                                     /* SBSW_OS_CORE_COREASR_DYN_001 */

  /* #20 Initialize Resources. */
  for(index = 0; index < (Os_ObjIdx_IteratorType) core->ResourceCount; index++)                                         /* FETA_OS_CONSTDATA */
  {
    Os_ResourceInit(core->ResourceRefs[index]);                                                                         /* SBSW_OS_CORE_RESOURCEINIT_001 */
  }

  /* #30 Initialize core IOCs. */
  for(index = 0; index < (Os_ObjIdx_IteratorType) core->IocCount; index++)                                              /* FETA_OS_CONSTDATA */
  {
    Os_IocInit(core->IocRefs[index]);                                                                                   /* SBSW_OS_CORE_IOCINIT */
  }

  /* #40 Initialize Applications. */
  Os_AppInit(core->KernelApp, coreStatus->ApplicationMode);                                                             /* SBSW_OS_CORE_APPINIT_002 */

  for(index = 0; index < (Os_ObjIdx_IteratorType) core->AppCount; index++)                                              /* FETA_OS_CONSTDATA */
  {
    Os_AppInit(core->AppRefs[index], coreStatus->ApplicationMode);                                                      /* SBSW_OS_CORE_APPINIT_001 */
  }

  /* #50 Initialize the XSignal module. */
  Os_XSigInit(core->XSignal);                                                                                           /* SBSW_OS_CORE_XSIGINIT_001 */

  /* #60 If user configured barriers exist */
  if(Os_BarrierIsEnabled() != 0u)                                                                                       /* PRQA S 2991, 2995 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule2.2_2995 */
  {
    /* #70 For all configured barriers */
    for(index = 0; index < (Os_ObjIdx_IteratorType) core->BarrierCount; index++)                                        /* FETA_OS_CONSTDATA */
    {
      /* #80 If barrier reference is a non NULL_PTR (core participates in the indexed barrier) */
      if(core->BarrierRefs[index] != NULL_PTR)
      {
        /* #90 Attach to the barrier */
        Os_BarrierAttachUserBarrier(core->BarrierRefs[index]);                                                          /* SBSW_OS_CORE_BARRIERATTACHUSERBARRIER_001 */
      }
    }
  }

  /* #100 Start timing protection. */
  Os_TpStart();

  /* #110 Synchronize with started AUTOSAR cores. */
  Os_BarrierSynchronizeInternal(core->Barrier);                                                                         /* SBSW_OS_CORE_BARRIERSYNCHRONIZE_001 */

  /* #120 Call system startup hook. */
  (void)Os_HookCallCallback(core->StartupHookRef);                                                                      /* SBSW_OS_CORE_HOOKCALLCALLBACK_001 */

  /* #130 Call application specific startup hooks. */
  for(index = 0; index < (Os_ObjIdx_IteratorType) core->AppCount; index++)                                              /* FETA_OS_CONSTDATA */
  {
    (void)Os_HookCallCallback(Os_AppGetStartupHook(core->AppRefs[index]));                                              /* SBSW_OS_CORE_HOOKCALLCALLBACK_002 */ /* SBSW_OS_CORE_APPGETSTARTUPHOOK_001 */
  }

  /* #140 Start all applications. */
  Os_AppStart(core->KernelApp);                                                                                         /* SBSW_OS_CORE_APPSTART_002 */

  for(index = 0; index < (Os_ObjIdx_IteratorType) core->AppCount; index++)                                              /* FETA_OS_CONSTDATA */
  {
    Os_AppStart(core->AppRefs[index]);                                                                                  /* SBSW_OS_CORE_APPSTART_001 */
  }

  /* #150 Synchronize with started AUTOSAR cores. */
  Os_BarrierSynchronizeInternal(core->Barrier);                                                                         /* SBSW_OS_CORE_BARRIERSYNCHRONIZE_001 */
}


/***********************************************************************************************************************
 *  Os_CoreShutdown()
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
 *
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
OS_LOCAL OS_FUNC_ATTRIBUTE_DEFINITION(void, OS_CODE, OS_NORETURN, Os_CoreShutdown,
(
  StatusType Error,
  boolean Synchronize
))
{
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) currentThread;
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) core;
  Os_ObjIdx_IteratorType index;

  currentThread = Os_CoreGetThread();
  core = Os_ThreadGetCore(currentThread);                                                                               /* SBSW_OS_CORE_THREADGETCORE_001 */

  /* #10 Disable Interrupts. */
  Os_IntDisable();

  /* #20 If we already are in a shutdown Hook: */
  if(OS_UNLIKELY(Os_ThreadIsShutdownHook(currentThread) != 0u))                                                         /* SBSW_OS_CORE_THREADISSHUTDOWNHOOK_001 */ /* COV_OS_INVSTATE */
  {
    /* #30 Kernel panic. */
    Os_ErrKernelPanic();
  }

  /* #40 If StartOS() has been called: */
  if(OS_LIKELY(Os_CoreOsIsStarted(core) != 0u))                                                                         /* SBSW_OS_CORE_COREOSISSTARTED_001 */
  {
    /* #50 Kill all user applications on this core, but not the kernel application. */
    for(index = 0; index < (Os_ObjIdx_IteratorType) core->AppCount; index++)                                            /* FETA_OS_CONSTDATA */
    {
      Os_AppKillNoSuicide(core->AppRefs[index]);                                                                        /* SBSW_OS_CORE_APPKILLNOSUICIDE_001 */
    }

    /* #60 Call application specific shutdown hooks. */
    for(index = 0; index < (Os_ObjIdx_IteratorType) core->AppCount; index++)                                            /* FETA_OS_CONSTDATA */
    {
      Os_HookCallStatusHook(Os_AppGetShutdownHook(core->AppRefs[index]), Error);                                        /* SBSW_OS_CORE_HOOKCALLSTATUSHOOK_001 */ /* SBSW_OS_CORE_APPGETSHUTDOWNHOOK_001 */
    }
  }

  /* #70 If "synchronization" is requested: */
  if(Synchronize == TRUE)
  {
    /* #80 Synchronize with started AUTOSAR cores. */
    Os_BarrierSynchronizeInternal(core->Barrier);                                                                       /* SBSW_OS_CORE_BARRIERSYNCHRONIZE_001 */
  }
  else
  {
    /* #90 Otherwise, decrement shutdown barrier. */
    Os_BarrierDetach(core->Barrier);                                                                                    /* SBSW_OS_CORE_BARRIERDETACH_001 */
  }

  /* #100 Call system shutdown hook. */
  Os_HookCallStatusHook(core->ShutdownHookRef, Error);                                                                  /* SBSW_OS_CORE_HOOKCALLSTATUSHOOK_002 */

  /* #110 Kernel panic. */
  Os_ErrKernelPanic();
}


/***********************************************************************************************************************
 *  Os_CoreDetermineStartStatus()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_LOCAL_INLINE FUNC(void, OS_CODE) Os_CoreDetermineStartStatus                                                         /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
(
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) Core
)
{
  Os_CoreStateType requestedState;
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) coreIt;
  CoreIdType callerCoreId;
  CoreIdType coreIdIt;

  requestedState = OS_CORESTATE_INACTIVE;                                                                               /* PRQA S 2982 */ /* MD_Os_Rule2.2_2982 */
  callerCoreId = Core->Id;

  /* #10 Find the first core which requested a startup. */
  for(coreIdIt = (CoreIdType)0; coreIdIt < OS_COREID_COUNT; ++coreIdIt)                                                 /* PRQA S 4332, 4527 */ /* MD_Os_Rule10.5_4332, MD_Os_Rule10.1_4527 */ /* COV_OS_CORESTARTUPSTATUS */
  {
    coreIt = Os_CoreId2Core(coreIdIt);
    requestedState = Os_CoreGetStatus(coreIt)->CoreStartRequests[callerCoreId];                                         /* PRQA S 2842 */ /* MD_Os_Rule18.1_2842 */ /* SBSW_OS_CORE_COREGETSTATUS_001 */

    if(requestedState != OS_CORESTATE_INACTIVE)
    {
      break;
    }
  }

  /* #20 If there is no request enter kernel panic. */
  if(requestedState == OS_CORESTATE_INACTIVE)                                                                           /* COV_OS_CORESTARTUPSTATUS */
  {
    Os_ErrKernelPanic();
  }

  /* #30 If there is a second request from another core enter kernel panic. */
  for(coreIdIt++; coreIdIt < OS_COREID_COUNT; ++coreIdIt)                                                               /* PRQA S 2994, 2996, 4527 */ /* MD_Os_Rule14.3_2994, MD_Os_Rule2.2_2996, MD_Os_Rule10.1_4527 */ /* COMP_WARN_OS_CW_005 */ /* FETA_OS_CONSTDATA */
  {
    coreIt = Os_CoreId2Core(coreIdIt);                                                                                  /* PRQA S 2880 */ /* MD_Os_Rule2.1_2880 */

    if(Os_CoreGetStatus(coreIt)->CoreStartRequests[callerCoreId] != OS_CORESTATE_INACTIVE)                              /* PRQA S 2842 */ /* MD_Os_Rule18.1_2842 */ /* SBSW_OS_CORE_COREGETSTATUS_001 */ /* COV_OS_CORESTARTUPSTATUS */
    {
      Os_ErrKernelPanic();
    }
  }

  Os_CoreGetStatus(Core)->ActivationState = requestedState;                                                             /* SBSW_OS_FC_PRECONDITION */ /* SBSW_OS_CORE_COREGETSTATUS_002 */
}


/***********************************************************************************************************************
 *  Os_CoreShutdownAllLocal()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(void, OS_CODE, OS_NORETURN, Os_CoreShutdownAllLocal,                                       /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
(
  StatusType Error
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  /* #10 Shutdown local core with shutdown synchronization. */
  Os_CoreShutdown(Error, TRUE);
}


/***********************************************************************************************************************
 *  Os_SystemShutdown()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
OS_FUNC_ATTRIBUTE_DEFINITION(void, OS_CODE, OS_NORETURN, Os_SystemShutdown,
(
  StatusType Error
))
{
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) currentThread;
  CoreIdType coreIdx;
  CoreIdType currentCoreId;

  /* #10 Disable interrupts. */
  Os_IntDisable();

  currentThread = Os_CoreGetThread();                                                                                   /* PRQA S 2983 */ /* MD_Os_Rule2.2_2983_Config */
  currentCoreId = Os_CoreGetId(Os_CoreGetCurrentCore());                                                                /* SBSW_OS_CORE_COREGETID_001 */

  /* #20 Tell other started AUTOSAR cores to shutdown. */
  for(coreIdx = (CoreIdType)0; coreIdx < OS_COREID_COUNT; ++coreIdx)                                                    /* PRQA S 4332,  4527 */ /* MD_Os_Rule10.5_4332,  MD_Os_Rule10.1_4527 */
  {
    if(coreIdx != currentCoreId)                                                                                        /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */ /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
    {
      P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;

      core = Os_CoreId2Core(coreIdx);

      if(core->Status->ActivationState == OS_CORESTATE_ACTIVATED_ASR)                                                   /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
      {
        (void)Os_XSigSend_ShutdownAllCores(Os_Core2AsrCore(core), currentThread, Error);                                /* SBSW_OS_CORE_XSIGSEND_001 */ /* SBSW_OS_CORE_CORE2ASRCORE_001 */
      }
    }
  }

  /* #30 Shutdown local core with shutdown synchronization. */
  Os_CoreShutdown(Error, TRUE);
}


/***********************************************************************************************************************
 *  Os_CoreCheckIdleMode()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreCheckIdleMode,
(
  IdleModeType IdleMode
))
{

  return Os_ErrExtendedCheck(Os_ErrIsValueLo((uint32)IdleMode, (uint32)OS_IDLEMODE_COUNT));
}


/***********************************************************************************************************************
 *  Os_CoreGetStartState()
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
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_CoreStartStateType, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreGetStartState,
(
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) Core
))
{
  Os_CoreStateType coreState;
  Os_CoreStartStateType result;
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) coreIt;
  CoreIdType coreItId;
  CoreIdType coreId;

  coreId = Core->Id;
  result = OS_CORESTARTSTATE_START_UNREQUESTED;

  /* #10 Read the cores activation state. */
  coreState = Os_CoreGetStatus(Core)->ActivationState;                                                                  /* SBSW_OS_FC_PRECONDITION */

  /* #20 If the core is an already started AUTOSAR core: */
  if (coreState == OS_CORESTATE_ACTIVATED_ASR)                                                                          /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
  {
    /* #30 Update the result to OS_CORESTARTSTATE_STARTED_ASR. */
    result = OS_CORESTARTSTATE_STARTED_ASR;
  }
  /* #40 Otherwise if the core is an already started Non-AUTOSAR master core: */
  else if (coreState == OS_CORESTATE_ACTIVATED_NONASR)                                                                  /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
  {
    /* #50 Update the result to OS_CORESTARTSTATE_STARTED_NONASR. */
    result = OS_CORESTARTSTATE_STARTED_NONASR;
  }
  /* #60 Otherwise if the activation state is inactive: */
  else if (coreState == OS_CORESTATE_INACTIVE)
  {
    /* #70 Check if there is a start request from any other core. */
    for(coreItId = (CoreIdType)0; coreItId < OS_COREID_COUNT; ++coreItId)                                               /* PRQA S 4332,  4527 */ /* MD_Os_Rule10.5_4332,  MD_Os_Rule10.1_4527 */
    {
      coreIt = Os_CoreId2Core(coreItId);
      coreState = Os_CoreGetStatus(coreIt)->CoreStartRequests[coreId];                                                  /* SBSW_OS_CORE_COREGETSTATUS_001 */
      if(coreState != OS_CORESTATE_INACTIVE){                                                                           /* COV_OS_NOFUNCCALL_XF */
        break;
      }
    }

    /* #80 If there is a start request by StartCore() API: */
    if(coreState == OS_CORESTATE_ACTIVATED_ASR)                                                                         /* COV_OS_NOFUNCCALL_XF */
    {
      /* #90 Update the result to OS_CORESTARTSTATE_START_REQUESTED_ASR. */
      result = OS_CORESTARTSTATE_START_REQUESTED_ASR;
    }
    /* #100 Otherwise if there is a start request by StartNonAutosarCore() API: */
    else if (coreState == OS_CORESTATE_ACTIVATED_NONASR)                                                                /* COV_OS_NOFUNCCALL_XF */
    {
      /* #110 Update the result to OS_CORESTARTSTATE_START_REQUESTED_NONASR. */
      result = OS_CORESTARTSTATE_START_REQUESTED_NONASR;
    }
    else{ /* Intentionally empty. */}
  }
  else
  {
    Os_ErrKernelPanic();
  }

  return result;
}


/***********************************************************************************************************************
 *  Os_CoreCheckCoreIsInactive()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
Os_CoreCheckCoreIsInactive,
(
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) Core
))                                                                                                                      /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
{
  return Os_ErrExtendedCheck(Os_CoreGetStartState(Core) == OS_CORESTARTSTATE_START_UNREQUESTED);                        /* PRQA S 4404 */ /* MD_Os_C90BooleanCompatibility */ /* SBSW_OS_FC_PRECONDITION */
}


/***********************************************************************************************************************
 *  Os_CoreGetTrue()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE boolean, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreGetTrue, (void))
{
  return TRUE;
}


/***********************************************************************************************************************
 *  Os_CoreIdle()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreIdle, (void))
{
  /* #10 Call HAL Idle in an endless loop. */
  while(Os_CoreGetTrue() == TRUE)                                                                                       /* PRQA S 2871, 2990 */ /* MD_Os_Dir4.1_2871, MD_Os_Rule14.3_2990 */ /* COV_OS_COREIDLELOOP */ /* FETA_OS_COREIDLE */
  {
    Os_Hal_CoreIdle();                                                                                                  /* PRQA S 2987 */ /* MD_Os_Rule2.2_2987 */
  }
}


/***********************************************************************************************************************
 *  Os_CoreGetStatus()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE P2VAR(volatile Os_CoreStatusType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE,
OS_ALWAYS_INLINE, Os_CoreGetStatus,
(
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) Core
))
{
  return Core->Status;
}


/***********************************************************************************************************************
 *  Os_CoreCheckOsIsNotStarted()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreCheckOsIsNotStarted,
(
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) Core
))
{
  return Os_ErrExtendedCheck(!Os_CoreOsIsStarted(Core));                                                                /* PRQA S 4116, 4404, 4558 */  /* MD_Os_Rule10.1_4116, MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */ /* SBSW_OS_FC_PRECONDITION */
}


/***********************************************************************************************************************
 *  Os_CoreCheckOsIsInitializing()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE,
OS_ALWAYS_INLINE, Os_CoreCheckOsIsInitializing,
(
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) Core
))
{
  return Os_ErrExtendedCheck((Os_StdReturnType)(Os_CoreAsrGetAsrStatus(Core)->OsState == OS_COREOSSTATE_INITIALZE));    /* PRQA S 1881, 4304 */ /* MD_Os_Rule10.4_1881, MD_Os_C90BooleanCompatibility */ /* SBSW_OS_FC_PRECONDITION */
}


/***********************************************************************************************************************
 *  Os_CoreCheckOsHasPreStartTask()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreCheckOsHasPreStartTask,
(
  void
))
{
  return (Os_StdReturnType)(OS_CFG_OS_PRESTARTTASK == STD_ON);                                                          /* PRQA S 2995, 4304 */ /* MD_Os_Rule2.2_2995, MD_Os_C90BooleanCompatibility */
}


/***********************************************************************************************************************
 *  Os_CoreCheckHasPreStartTask()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreCheckHasPreStartTask,
(
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) Core
))
{
  return Os_ErrExtendedCheck((Os_StdReturnType)(Core->PreStartTask != NULL_PTR));                                       /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */
}


/***********************************************************************************************************************
 *  Os_CoreAsrGetApplicationMode()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE AppModeType, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreAsrGetApplicationMode,
(
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) Core
))
{
  return Os_CoreAsrGetAsrStatus(Core)->ApplicationMode;                                                                 /* SBSW_OS_FC_PRECONDITION */
}


/***********************************************************************************************************************
 *  Os_CoreAsrCombineApplicationMode()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreAsrCombineApplicationMode,
(
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) Core,
  AppModeType ApplicationMode
))
{
  Os_CoreAsrGetAsrStatus(Core)->ApplicationMode |= ApplicationMode;                                                     /* SBSW_OS_FC_PRECONDITION */ /* SBSW_OS_CORE_COREASR_STATUS_002 */
}


/***********************************************************************************************************************
 *  Os_CoreCheckIsAppModeValid()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_CoreCheckIsAppModeValid,
(
    AppModeType Mode
))
{
  /* #10 If there is a single bit set exactly, return !OS_CHECK_FAILED, otherwise OS_CHECK_FAILED. */
  /*
   * Check that one bit is set at most: ((Mode - 1) & Mode) == 0
   *
   * If only one app mode bit is set, this bit will be toggled by subtracting '1', so the
   * result of the '&' operation is '0' ==> check succeeded.
   * If more than one bit is set, the upper bit will not be changed by subtracting '1',
   * so the result of the '&' operation is not '0'  ==> check failed.
   */
  return (Os_StdReturnType)((((Mode - 1u) & Mode) == 0u) && (Mode != 0u));                                              /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */
}


/***********************************************************************************************************************
 *  Os_Task_Os_IdleTask()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TASK(Os_IdleTask)
{
  /* #10 Enter idle state. */
  Os_CoreIdle();
}


/***********************************************************************************************************************
 *  Os_CoreGetInitHook()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE
P2CONST(Os_HookInitHookConfigType, AUTOMATIC, OS_CONST), OS_CODE, OS_ALWAYS_INLINE, Os_CoreGetInitHook,
(
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) Core
))
{
  return Core->InitHookRef;
}


/***********************************************************************************************************************
 *  Os_CoreCheckExceptionContextReadingSupported()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_CoreCheckExceptionContextReadingSupported,
(
  void
))
{
  return Os_ErrExtendedCheck(Os_CoreIsExceptionContextReadingSupported());
}


/***********************************************************************************************************************
 *  Os_CoreCheckExceptionContextModificationSupported()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_CoreCheckExceptionContextModificationSupported,
(
  void
))
{
  return Os_ErrExtendedCheck(Os_CoreIsExceptionContextModificationSupported());
}


/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Os_Api_GetVersionInfo()
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_Api_GetVersionInfo
(
  Std_VersionInfoType *versioninfo
)
{
  *versioninfo = Os_SystemGet()->VersionInfo;                                                                           /* SBSW_OS_PWA_PRECONDITION */
}


/***********************************************************************************************************************
 *  Os_Api_GetExceptionContext()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Os_StatusType, OS_CODE) Os_Api_GetExceptionContext
(
  Os_ExceptionContextRefType Context
)
{
  Os_StatusType status;

  /* #10 Perform error checks */
  if(OS_UNLIKELY(Os_CoreCheckExceptionContextReadingSupported() == OS_CHECK_FAILED))                                    /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */ /* COV_OS_HALPLATFORMGETEXCEPTIONCONTEXTAVAILABILITY */
  {
    status = OS_STATUS_UNIMPLEMENTED_EXCEPTIONCONTEXT;
  }
  else
  {
    P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) currentThread;

    currentThread = Os_CoreGetThread();

    if(OS_UNLIKELY(Os_ThreadCheckCallContext(currentThread, OS_APICONTEXT_GETEXCEPTIONCONTEXT) == OS_CHECK_FAILED))     /* SBSW_OS_CORE_THREADCHECKCALLCONTEXT_001 */
    {
      status = OS_STATUS_CALLEVEL;
    }
    else
    {
      /* #20 Get context from HAL */
      *Context = *OsCfg_Hal_Context_ExceptionContextRef[Os_Hal_CoreGetId()];                                            /* SBSW_OS_PWA_PRECONDITION */
      status = OS_STATUS_OK;
    }
  }

  return status;
}


/***********************************************************************************************************************
 *  Os_Api_SetExceptionContext()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Os_StatusType, OS_CODE) Os_Api_SetExceptionContext
(
  P2CONST(Os_ExceptionContextType, AUTOMATIC, OS_CONST) Context
)
{
  Os_StatusType status;

  /* #10 Perform error checks */
  if(OS_UNLIKELY(Os_CoreCheckExceptionContextModificationSupported() == OS_CHECK_FAILED))                               /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */ /* COV_OS_HALPLATFORMSETEXCEPTIONCONTEXTAVAILABILITY */
  {
    status = OS_STATUS_UNIMPLEMENTED_EXCEPTIONCONTEXT;
  }
  else
  {
    P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) currentThread;

    currentThread = Os_CoreGetThread();

    if(OS_UNLIKELY(Os_ThreadCheckCallContext(currentThread, OS_APICONTEXT_SETEXCEPTIONCONTEXT) == OS_CHECK_FAILED))     /* SBSW_OS_CORE_THREADCHECKCALLCONTEXT_001 */
    {
      status = OS_STATUS_CALLEVEL;
    }
    else
    {
      /* #20 Set HAL context */
      *OsCfg_Hal_Context_ExceptionContextRef[Os_Hal_CoreGetId()] =  *Context;                                           /* SBSW_OS_CORE_SETEXCEPTIONCONTEXT_001 */
      status = OS_STATUS_OK;
    }
  }

  return status;
}


/***********************************************************************************************************************
 *  Os_Api_GetExceptionAddress()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Os_AddressOfConstType, OS_CODE) Os_Api_GetExceptionAddress
(
  void
)
{
  Os_AddressOfConstType address;
  P2CONST(Os_ErrorType, AUTOMATIC, OS_CONST) lastError;

  /* #10 Read the exception instruction address from the core local error data. */
  lastError = &(Os_CoreAsrGetDyn(Os_Core2AsrCore(Os_CoreGetCurrentCore()))->LastError);                                 /* SBSW_OS_CORE_COREASRGETDYN_002 */ /* SBSW_OS_CORE_CORE2ASRCORE_002 */
  address = Os_ErrGetExceptionInstructionAddress(lastError);                                                            /* SBSW_OS_CORE_ERRGETEXCEPTIONINSTRUCTIONADDRESS_001 */

  return address;
}


/***********************************************************************************************************************
 *  Os_Api_InitMemory()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, OS_CODE) Os_Api_InitMemory
(
  void
)
{
  /* #10 There is no OS data within init sections, so nothing to be done here. */
  /* nop */
}


/***********************************************************************************************************************
 *  Os_SystemInterruptHandlingInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_LOCAL FUNC(void, OS_CODE) Os_SystemInterruptHandlingInit
(
  P2CONST(Os_SystemConfigType, AUTOMATIC, OS_CONST) SystemCfg
)
{
  Os_ObjIdx_IteratorType intSrcIdx;

  /* #10 For each interrupt source object on the given system configuration: */
  for(intSrcIdx = 0; intSrcIdx < (Os_ObjIdx_IteratorType) SystemCfg->InterruptSourceCount; intSrcIdx++)                 /* COV_OS_HALPLATFORMSYSTEMINTERRUPTSUNSUPPORTED */ /* FETA_OS_CONSTANTASFUNCTIONPARAMETER */
  {
    /* #20 Call low level initialization function. */
    Os_IsrInterruptSourceInit(SystemCfg->InterruptSourceRefs[intSrcIdx]);                                               /* SBSW_OS_CORE_ISRINTERRUPTSOURCEINIT_001 */
  }
}


/***********************************************************************************************************************
 *  Os_CoreInterruptHandlingInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_LOCAL FUNC(void, OS_CODE) Os_CoreInterruptHandlingInit
(
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) Core
)
{
  Os_ObjIdx_IteratorType intSrcIdx;

  /* #10 For each interrupt source object on the given core: */
  for(intSrcIdx = 0; intSrcIdx < (Os_ObjIdx_IteratorType) Core->InterruptSourceCount; intSrcIdx++)                      /* COV_OS_HALPLATFORMCOREINTERRUPTSUNSUPPORTED */ /* FETA_OS_CONSTANTASFUNCTIONPARAMETER */
  {
    /* #20 Call low level initialization function. */
    Os_IsrInterruptSourceInit(Core->InterruptSourceRefs[intSrcIdx]);                                                    /* SBSW_OS_CORE_ISRINTERRUPTSOURCEINIT_001 */
  }
}


/***********************************************************************************************************************
 *  Os_CoreEmulateMaster()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_CoreEmulateMaster, (void))             /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  CoreIdType coreId;

  /* #10 For all auto-started slave cores. */
  for(coreId = (CoreIdType)0; coreId < OS_COREID_COUNT; coreId++)                                                       /* PRQA S 4332, 4527 */ /* MD_Os_Rule10.5_4332, MD_Os_Rule10.1_4527 */
  {
    if(coreId != OS_CORE_ID_MASTER)                                                                                     /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */ /* COMP_WARN_OS_CW_005 */
    {
      P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;

      core = Os_CoreId2Core(coreId);                                                                                    /* PRQA S 2880 */ /* MD_Os_Rule2.1_2880 */

      if(core->IsAutostart == TRUE)                                                                                     /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
      {
        /* #20 Perform the first stage echo. */
        do                                                                                                              /* FETA_OS_COREINIT */
        {
          OsCfg_CoreBootBarrierRefs[coreId]->EchoIn = OS_CORE_BOOTBARRIER_ECHOPATTERN1;                                 /* SBSW_OS_CORE_COREBOOTBARRIERREFS_001 */
          Os_Hal_CoreNop();
        }while(OsCfg_CoreBootBarrierRefs[coreId]->EchoOut != OS_CORE_BOOTBARRIER_ECHOPATTERN1);                         /* COV_OS_COREBOOTBARRIER */

        /* #30 Perform the second stage echo. */
        do                                                                                                              /* FETA_OS_COREINIT */
        {
          OsCfg_CoreBootBarrierRefs[coreId]->EchoIn = OS_CORE_BOOTBARRIER_ECHOPATTERN2;                                 /* SBSW_OS_CORE_COREBOOTBARRIERREFS_001 */
          Os_Hal_CoreNop();
        }while(OsCfg_CoreBootBarrierRefs[coreId]->EchoOut != OS_CORE_BOOTBARRIER_ECHOPATTERN2);                         /* COV_OS_COREBOOTBARRIER */
      }
    }
  }

  /* #40 First synchronization barrier is done. Trigger all auto-start cores to initialize their hardware and
   *  waits until they are done. */
  for(coreId = (CoreIdType)0; coreId < OS_COREID_COUNT; coreId++)                                                       /* PRQA S 4332, 4527 */ /* MD_Os_Rule10.5_4332, MD_Os_Rule10.1_4527 */
  {
    if(coreId != OS_CORE_ID_MASTER)                                                                                     /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */ /* COMP_WARN_OS_CW_005 */
    {
      P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;

      core = Os_CoreId2Core(coreId);                                                                                    /* PRQA S 2880 */ /* MD_Os_Rule2.1_2880 */

      if(core->IsAutostart == TRUE)                                                                                     /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
      {
        OsCfg_CoreBootBarrierRefs[coreId]->WaitingSign = OS_CORE_BOOTBARRIER_INITHARDWARE;                              /* SBSW_OS_CORE_COREBOOTBARRIERREFS_001 */

        while(OsCfg_CoreBootBarrierRefs[coreId]->WaitingSign != OS_CORE_BOOTBARRIER_INITHARDWARE_COMPLETED)             /* COV_OS_HALPLATFORMTESTEDASMULTICORE */ /* FETA_OS_COREINIT */
        {
          Os_Hal_CoreNop();
        }
      }
    }
  }
}


/***********************************************************************************************************************
 *  Os_CoreEmulateSlaveWaitForHardwareInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_CoreEmulateSlaveWaitForHardwareInit,   /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
(
  CoreIdType Id
))                                                                                                                      /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
{
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;

  Os_Assert(Os_ErrIsValueLo((uint32)Id, (uint32)OS_COREID_COUNT));
  core = Os_CoreId2Core(Id);

  /* #10 if this core is an auto-start core. */
  if(core->IsAutostart == TRUE)
  {
    /* #20 Set the waiting signal. */
    OsCfg_CoreBootBarrierRefs[Id]->WaitingSign = OS_CORE_BOOTBARRIER_WAITINGSIGN;                                       /* SBSW_OS_CORE_COREBOOTBARRIERREFS_001 */

    /* #30 Perform echo until the waiting signal is set by the master. */
    while(OsCfg_CoreBootBarrierRefs[Id]->WaitingSign != OS_CORE_BOOTBARRIER_INITHARDWARE)                               /* FETA_OS_COREINIT */
    {
      OsCfg_CoreBootBarrierRefs[Id]->EchoOut = OsCfg_CoreBootBarrierRefs[Id]->EchoIn;                                   /* SBSW_OS_CORE_COREBOOTBARRIERREFS_001 */
      Os_Hal_CoreNop();
    }
  }
}


/***********************************************************************************************************************
 *  Os_CoreEmulateSlaveWaitForStartCore()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_CoreEmulateSlaveWaitForStartCore,      /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
(
  CoreIdType Id
))                                                                                                                      /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
{
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;

  Os_Assert(Os_ErrIsValueLo((uint32)Id, (uint32)OS_COREID_COUNT));
  core = Os_CoreId2Core(Id);

  /* #10 if this core is an auto-start core. */
  if(core->IsAutostart == TRUE)                                                                                         /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
  {
    /* #20 Tells the master that it has completed the hardware initialization. */
    OsCfg_CoreBootBarrierRefs[Id]->WaitingSign = OS_CORE_BOOTBARRIER_INITHARDWARE_COMPLETED;                            /* SBSW_OS_CORE_COREBOOTBARRIERREFS_001 */

    /* #30 Wait until the waiting signal is set by the master. */
    while(OsCfg_CoreBootBarrierRefs[Id]->WaitingSign != OS_CORE_BOOTBARRIER_STARTCORE)                                  /* COV_OS_HALPLATFORMTESTEDASMULTICORE */ /* FETA_OS_COREINIT */
    {
      Os_Hal_CoreNop();
    }
  }
}


/***********************************************************************************************************************
 *  Os_Api_Init()
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
/* PRQA S 6030, 6050, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(void, OS_CODE) Os_Api_Init(void)
{
  /* #05 If the OS is not configured for the interrupt only use case: */
  if(Os_CoreIsInterruptOnlyOs() == 0u )                                                                                 /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_SystemConfigType, AUTOMATIC, OS_CONST) systemCfg;

    core = Os_CoreGetCurrentCore();
    systemCfg = Os_SystemGet();

    /* #10 If called on the hardware initialization core. */
    if(core->IsHardwareInitCore == TRUE)                                                                                /* PRQA S 2991, 2995 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule2.2_2995 */
    {
      /* #20 Perform hardware specific initialization on the Hardware Initialization Core. */
      Os_Hal_CoreInitHardware(systemCfg->HwConfig);                                                                     /* SBSW_OS_CORE_HAL_COREINITHARDWARE_001 */
      /* #30 Initialize system wide interrupt handling */
      Os_SystemInterruptHandlingInit(systemCfg);                                                                        /* SBSW_OS_CORE_SYSTEMINTERRUPTHANDLINGINIT_001 */
      /* #40 Initialize system's memory protection. */
      Os_MpSystemInit(systemCfg->MemoryProtection);                                                                     /* SBSW_OS_CORE_MPSYSTEMINIT_001 */
    }

    /* #45 In a single core system, only initialize the system data. */
    if(Os_CoreIsSingleCoreOs() != 0u)                                                                                   /* COV_OS_TESTSUITERESTRICTIONCORECOUNT */ /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
    {
      Os_SystemInit();
    }
    else
    {
      /* #50 Otherwise, if called on master core. */
      if(core->Id == OS_CORE_ID_MASTER)                                                                                 /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
      {
        /* #60 Perform the boot synchronization. */
        Os_CoreEmulateMaster();

        /* #70 Initialize the system data. */
        Os_SystemInit();
      }
      /* #80 Otherwise: */
      else                                                                                                              /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
      {
        /* At most one core is configured with IsMasterStartAllowed = TRUE */
        /* #90 Start the master core, if it is not an auto-start core and the current core is allowed to do that. */
        if(core->IsMasterStartAllowed == TRUE)                                                                          /* COV_OS_HALPLATFORMMASTERSTARTALLOWED */
        {
          P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) masterCore;

          masterCore = Os_CoreId2Core(OS_CORE_ID_MASTER);

          if(masterCore->IsAutostart == FALSE)
          {
            Os_Hal_CoreStart(masterCore->HwConfig);                                                                     /* SBSW_OS_CORE_HAL_CORESTART_001 */
          }
        }

        /* #100 Perform the boot synchronization on this slave core. */
        Os_CoreEmulateSlaveWaitForHardwareInit(core->Id);

        /* #110 Perform the boot synchronization on this slave core until StartCore is called by the master. */
        Os_CoreEmulateSlaveWaitForStartCore(core->Id);

        /* #120 Determine the startup mode. */
        Os_CoreDetermineStartStatus(core);                                                                              /* SBSW_OS_CORE_COREDETERMINESTARTSTATUS_001 */
      }
    }

    /* #130 Initialize core specific interrupt handling. */
    Os_CoreInterruptHandlingInit(core);                                                                                 /* SBSW_OS_CORE_COREINTERRUPTHANDLINGINIT_001 */

    /* #140 If we are an AUTOSAR core: */
    if(core->IsAsrCore == TRUE)                                                                                         /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
    {
      P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) asrCore;

      asrCore = Os_Core2AsrCore(core);                                                                                  /* SBSW_OS_CORE_CORE2ASRCORE_001 */

      /* #150 Initialize thread class object */
      Os_ThreadClassInit(asrCore->ThreadClass);                                                                         /* SBSW_OS_CORE_THREADCLASSINIT_001 */


      /* #160 If we are the master core. */
      if(core->Id == OS_CORE_ID_MASTER)
      {
        /* #170 Attach to the barrier. */
        Os_BarrierAttach(asrCore->Barrier);                                                                             /* SBSW_OS_CORE_BARRIERATTACH_001 */

        Os_CoreGetStatus(core)->ActivationState = OS_CORESTATE_ACTIVATED_ASR;                                           /* SBSW_OS_CORE_COREGETSTATUS_001 */ /* SBSW_OS_CORE_COREGETSTATUS_002 */
      }

      /* #180 Perform reduced initialization that allows usage of interrupt API. */
      Os_CoreReducedInit(asrCore);                                                                                      /* SBSW_OS_CORE_COREREDUCEDINIT_001 */

      /* #190 Initialize stacks. */
      {
        Os_ObjIdx_IteratorType stackIdx;

        for(stackIdx = 0; stackIdx < (Os_ObjIdx_IteratorType) asrCore->StackCount; ++stackIdx)                          /* FETA_OS_CONSTDATA */
        {
          Os_StackInit(asrCore->StackRefs[stackIdx]);                                                                   /* SBSW_OS_CORE_STACKINIT_001 */
        }
      }
    }
    /* #200 Otherwise if this is the master core, update its state. */
    else if(core->Id == OS_CORE_ID_MASTER)                                                                              /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
    {
      Os_CoreGetStatus(core)->ActivationState = OS_CORESTATE_ACTIVATED_NONASR;                                          /* SBSW_OS_CORE_COREGETSTATUS_001 */ /* SBSW_OS_CORE_COREGETSTATUS_002 */
    }
    else
    {
      /* Nothing to be done for non master, non autosar cores.  */
    }
  }
  /* #210 Else ignore the call. */
}


/***********************************************************************************************************************
 *  Os_Api_InitInterruptOnly()
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
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
FUNC(void, OS_CODE) Os_Api_InitInterruptOnly(void)
{
  /* #10 If the OS is configured for the interrupt only use case: */
  if(Os_CoreIsInterruptOnlyOs() != 0u )                                                                                 /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_SystemConfigType, AUTOMATIC, OS_CONST) systemCfg;
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) asrCore;

    core = Os_CoreGetCurrentCore();
    systemCfg = Os_SystemGet();
    asrCore = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_CORE_CORE2ASRCORE_002 */

    /* #20 Perform hardware specific initialization on the Hardware Initialization Core. */
    Os_Hal_CoreInitHardware(systemCfg->HwConfig);                                                                       /* SBSW_OS_CORE_HAL_COREINITHARDWARE_001 */

    /* #30 Initialize system wide interrupt handling. */
    Os_SystemInterruptHandlingInit(systemCfg);                                                                          /* SBSW_OS_CORE_SYSTEMINTERRUPTHANDLINGINIT_001 */

    /* #40 Initialize core specific interrupt handling. */
    Os_CoreInterruptHandlingInit(core);                                                                                 /* SBSW_OS_CORE_COREINTERRUPTHANDLINGINIT_001 */

    /* #50 Initialize hardware specific stuff. */
    Os_Hal_CoreInit(asrCore->HwConfig);                                                                                 /* SBSW_OS_CORE_HAL_COREINIT_002 */
  }
  /* #60 Else ignore the call. */
}


/***********************************************************************************************************************
 *  Os_Api_EnterPreStartTask()
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
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
FUNC(void, OS_CODE) Os_Api_EnterPreStartTask(void)
{
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;

  core = Os_CoreGetCurrentCore();

  /* #10 If no Pre-Start Task is configured: KernelPanic. */
  if(Os_CoreCheckOsHasPreStartTask() == 0u)                                                                             /* COV_OS_INVSTATE */ /* PRQA S 2992, 2996 */ /* MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2996 */
  {
    Os_ErrKernelPanic();                                                                                                /* PRQA S 2880 */ /* MD_Os_Rule2.1_2880 */
  }

  /* #15 Check that the core has been started as an AUTOSAR core. */
  if(OS_UNLIKELY(Os_CoreCheckStartedAsAsr(core) == OS_CHECK_FAILED))                                                    /* COV_OS_INVSTATE */ /* SBSW_OS_CORE_CORECHECKSTARTEDASASR_001 */
  {
    Os_ErrKernelPanic();
  }
  else
  {
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_CORE_CORE2ASRCORE_002 */

    /* #20 Check that the core has a pre-start task configured. */
    if(OS_UNLIKELY(Os_CoreCheckHasPreStartTask(coreAsr) == OS_CHECK_FAILED))                                            /* COV_OS_INVSTATE */ /* SBSW_OS_CORE_CORECHECKHASPRESTARTTASK_001 */
    {
      Os_ErrKernelPanic();
    }
    /* #30 Check that StartOS() and Os_Api_EnterPreStartTask has not been called before. */
    else if(OS_UNLIKELY(Os_CoreCheckOsIsInitializing(coreAsr) == OS_CHECK_FAILED))                                      /* COV_OS_INVSTATE */ /* SBSW_OS_CORE_CORECHECKOSISINITIALIZING_001 */
    {
      Os_ErrKernelPanic();
    }
    else
    {
      /* #40 Initialize Pre-Start Task. */
      Os_TaskInit(coreAsr->PreStartTask, 0);                                                                            /* SBSW_OS_CORE_TASKINIT_001 */

      /* #50 Set core state to "in Pre-Start Task". */
      Os_CoreAsrGetAsrStatus(coreAsr)->OsState = OS_COREOSSTATE_PRESTARTTASK;                                           /* SBSW_OS_CORE_COREASR_STATUS_001 */ /* SBSW_OS_CORE_COREASR_STATUS_002 */

      /* #60 Task switch to Pre-Start Task. */
      Os_TaskPreStartTaskEnter(coreAsr->PreStartTask);                                                                  /* SBSW_OS_CORE_TASKPRESTARTTASKENTER_001 */
    }
  }
}


/***********************************************************************************************************************
 *  Os_CorePreStartTaskWrapper()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
OS_FUNC_ATTRIBUTE_DEFINITION(void, OS_CODE, OS_NORETURN, Os_CorePreStartTaskWrapper,
(
  void
))
{
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
  P2CONST(Os_Hal_ContextConfigType, TYPEDEF, OS_CONST) context;
  Os_Hal_ContextStackConfigType stack;

  context = Os_ThreadGetContext(Os_CoreGetThread());                                                                    /* SBSW_OS_CORE_THREADGETCONTEXT_001 */
  coreAsr = Os_Core2AsrCore(Os_CoreGetCurrentCore());                                                                   /* SBSW_OS_CORE_CORE2ASRCORE_002 */

  /* #10 Read the current stack from the context. */
  Os_Hal_ContextGetStack(context, &stack);                                                                              /* SBSW_OS_FC_POINTER2LOCAL */

  /* #20 Call core Pre-Start initialization. */
  Os_CorePreStartInit(&stack);                                                                                          /* SBSW_OS_FC_POINTER2LOCAL */

  /* #30 Initialize all non-trusted functions. */
  Os_AppServicesInit(coreAsr->KernelApp);                                                                               /* SBSW_OS_CORE_APPSERVICESINIT_001 */
  {
    Os_ObjIdx_IteratorType index;

    for(index = 0; index < (Os_ObjIdx_IteratorType) coreAsr->AppCount; index++)                                         /* FETA_OS_CONSTDATA */
    {
      Os_AppServicesInit(coreAsr->AppRefs[index]);                                                                      /* SBSW_OS_CORE_APPSERVICESINIT_002 */
    }
  }

  /* #40 Call the Pre-Start Task. */
  coreAsr->PreStartTaskCallback();                                                                                      /* SBSW_OS_CORE_PRESTARTTASK_CALLBACK_001 */

  /* #50 If the Pre-Start Task Callback returns: KernelPanic. */
  Os_ErrKernelPanic();
}


/***********************************************************************************************************************
 *  Os_Api_StartCore()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050, 6080 1 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(void, OS_CODE) Os_Api_StartCore                                                                                    /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
(
  CoreIdType CoreID,
  StatusType *Status
)
{
  *Status = E_OK;                                                                                                       /* PRQA S 2982 */ /* MD_Os_Rule2.2_2982_Config */ /* SBSW_OS_PWA_PRECONDITION */

  /* #10 Perform error checks. */
  if(Os_CoreIsSingleCoreOs() != 0u)                                                                                     /* COV_OS_TESTSUITERESTRICTIONCORECOUNT */ /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
  {
    *Status = E_OS_ID;                                                                                                  /* SBSW_OS_PWA_PRECONDITION */
  }
  else if(OS_UNLIKELY(Os_CoreCheckId(CoreID) == OS_CHECK_FAILED ))
  {
    *Status = E_OS_ID;                                                                                                  /* SBSW_OS_PWA_PRECONDITION */
  }
  else
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) currentCore;

    core = Os_CoreId2Core(CoreID);
    currentCore = Os_CoreGetCurrentCore();

    if(OS_UNLIKELY(Os_CoreCheckIsAsrCore(core) == OS_CHECK_FAILED))                                                     /* SBSW_OS_CORE_CORECHECKISASRCORE_001 */
    {
      *Status = E_OS_ID;                                                                                                /* SBSW_OS_PWA_PRECONDITION */
    }
    else
    {
      if(OS_UNLIKELY(Os_CoreCheckCoreIsInactive(core) == OS_CHECK_FAILED))                                              /* SBSW_OS_CORE_CORECHECKCOREISINACTIVE_001 */
      {
        *Status = E_OS_STATE;                                                                                           /* SBSW_OS_PWA_PRECONDITION */
      }
      else
      {
        if(Os_CoreIsAsrCore(currentCore) != 0u)                                                                         /* SBSW_OS_CORE_COREISASRCORE_002 */
        {
          P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) currentCoreAsr;

          currentCoreAsr = Os_Core2AsrCore(currentCore);                                                                /* SBSW_OS_CORE_CORE2ASRCORE_002 */

          if(OS_UNLIKELY(Os_CoreCheckOsIsNotStarted(currentCoreAsr) == OS_CHECK_FAILED))                                /* SBSW_OS_CORE_CORECHECKOSISNOTSTARTED_001 */
          {
            *Status = E_OS_ACCESS;                                                                                      /* SBSW_OS_PWA_PRECONDITION */
          }
        }
      }

      if(OS_LIKELY(*Status == E_OK))
      {
        P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;

        /* #20 Set the core activation request. */
        Os_CoreGetStatus(currentCore)->CoreStartRequests[CoreID] = OS_CORESTATE_ACTIVATED_ASR;                          /* PRQA S 2842 */ /* MD_Os_Rule18.1_2842_Check */ /* SBSW_OS_CORE_COREGETSTATUS_003 */ /* SBSW_OS_CORE_CORESTARTREQUESTS_001 */

        coreAsr = Os_Core2AsrCore(core);                                                                                /* SBSW_OS_CORE_CORE2ASRCORE_001 */

        /* #30 Attach to synchronization barrier. */
        Os_BarrierAttach(coreAsr->Barrier);                                                                             /* SBSW_OS_CORE_BARRIERATTACH_001 */

        if(core->IsAutostart == TRUE)                                                                                   /* COV_OS_HALPLATFORMNONAUTOSTARTCORE */
        {
          /* #40 If the core is an auto-start core (already booted), just set the waiting signal. */
          OsCfg_CoreBootBarrierRefs[CoreID]->WaitingSign = OS_CORE_BOOTBARRIER_STARTCORE;                               /* SBSW_OS_CORE_COREBOOTBARRIERREFS_001 */
        }
        else
        {
          /* #50 Otherwise start the core through hardware registers. */
          Os_Hal_CoreStart(core->HwConfig);                                                                             /* SBSW_OS_CORE_HAL_CORESTART_001 */
        }
      }
    }
  }
}


/***********************************************************************************************************************
 *  Os_Api_StartNonAutosarCore()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, OS_CODE) Os_Api_StartNonAutosarCore                                                                          /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
(
  CoreIdType CoreID,
  StatusType *Status
)
{
  /* #10 Perform error checks. */
  if(Os_CoreIsSingleCoreOs() != 0u)                                                                                     /* COV_OS_TESTSUITERESTRICTIONCORECOUNT */ /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
  {
    *Status = E_OS_ID;                                                                                                  /* SBSW_OS_PWA_PRECONDITION */
  }
  else if(OS_UNLIKELY(Os_CoreCheckId(CoreID) == OS_CHECK_FAILED ))
  {
    *Status = E_OS_ID;                                                                                                  /* SBSW_OS_PWA_PRECONDITION */
  }
  else
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;

    core = Os_CoreId2Core(CoreID);

    if(OS_UNLIKELY(Os_CoreCheckCoreIsInactive(core) == OS_CHECK_FAILED))                                                /* SBSW_OS_CORE_CORECHECKCOREISINACTIVE_001 */
    {
      *Status = E_OS_STATE;                                                                                             /* SBSW_OS_PWA_PRECONDITION */
    }
    else
    {
      P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) currentCore;

      currentCore = Os_CoreGetCurrentCore();

      /* #20 Set the core activation request. */
      Os_CoreGetStatus(currentCore)->CoreStartRequests[CoreID] = OS_CORESTATE_ACTIVATED_NONASR;                         /* PRQA S 2842 */ /* MD_Os_Rule18.1_2842_Check */ /* SBSW_OS_CORE_COREGETSTATUS_003 */ /* SBSW_OS_CORE_COREGETSTATUS_002 */
      *Status = E_OK;                                                                                                   /* SBSW_OS_PWA_PRECONDITION */
    }

    /* Performed without lock. */
    if(OS_LIKELY(*Status == E_OK))
    {
      if(core->IsAutostart == TRUE)                                                                                     /* COV_OS_HALPLATFORMNONAUTOSTARTCORE */
      {
        /* #30 If the core is an auto-start core (already booted), just set the waiting signal. */
        OsCfg_CoreBootBarrierRefs[CoreID]->WaitingSign = OS_CORE_BOOTBARRIER_STARTCORE;                                 /* SBSW_OS_CORE_COREBOOTBARRIERREFS_001 */
      }
      else
      {
        /* #40 Otherwise start the core through hardware registers. */
        Os_Hal_CoreStart(core->HwConfig);                                                                               /* SBSW_OS_CORE_HAL_CORESTART_001 */
      }
    }
  }
}


/***********************************************************************************************************************
 *  Os_Api_GetCoreID()
 **********************************************************************************************************************/
FUNC(CoreIdType, OS_CODE) Os_Api_GetCoreID
(
  void
)
{
  return (CoreIdType)Os_CoreGetId(Os_CoreGetCurrentCore());                                                             /* SBSW_OS_CORE_COREGETID_001 */
}


/***********************************************************************************************************************
 *  Os_Api_GetNumberOfActivatedCores()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint32, OS_CODE) Os_Api_GetNumberOfActivatedCores                                                                  /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
(
  void
)
{
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) coreIt;
  CoreIdType numberOfCores;
  CoreIdType it;

  numberOfCores = (CoreIdType)0;                                                                                        /* PRQA S 4332 */ /* MD_Os_Rule10.5_4332 */

  for(it = (CoreIdType)0; it < OS_COREID_COUNT; ++it)                                                                   /* PRQA S 4332,  4527 */ /* MD_Os_Rule10.5_4332,  MD_Os_Rule10.1_4527 */
  {
    coreIt = Os_CoreId2Core(it);

    if(Os_CoreIsAsrCore(coreIt) != 0u)                                                                                  /* SBSW_OS_CORE_COREISASRCORE_001 */
    {
      if(Os_CoreAsrGetAsrStatus(Os_Core2AsrCore(coreIt))->OsState == OS_COREOSSTATE_STARTED)                            /* SBSW_OS_CORE_COREASRGETASRSTATUS_001 */ /* SBSW_OS_CORE_CORE2ASRCORE_001 */
      {
        numberOfCores++;                                                                                                /* PRQA S 4527 */ /*  MD_Os_Rule10.1_4527 */
      }
    }
  }

  return (uint32)numberOfCores;
}


/***********************************************************************************************************************
 *  Os_Api_GetActiveApplicationMode()
 **********************************************************************************************************************/
FUNC(AppModeType, OS_CODE) Os_Api_GetActiveApplicationMode
(
  void
)
{
  return Os_CoreAsrGetApplicationMode(Os_ThreadGetCore(Os_CoreGetThread()));                                            /* SBSW_OS_CORE_COREASRGETAPPLICATIONMODE_002 */ /* SBSW_OS_CORE_THREADGETCORE_001 */
}


/***********************************************************************************************************************
 *  Os_Api_StartOS()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
FUNC(void, OS_CODE) Os_Api_StartOS(AppModeType Mode)
{
  /* #10 If the OS is not configured for the interrupt only use case: */
  if(Os_CoreIsInterruptOnlyOs() == 0u )                                                                                 /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;

    core = Os_CoreGetCurrentCore();

    /* #10 Check that the core has been started as an AUTOSAR core. */
    if(OS_UNLIKELY(Os_CoreCheckStartedAsAsr(core) == OS_CHECK_FAILED))                                                  /* SBSW_OS_CORE_CORECHECKSTARTEDASASR_001 */ /* COV_OS_INVSTATE */
    {
      Os_ErrKernelPanic();
    }
    else
    {
      P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
      coreAsr = Os_Core2AsrCore(core);                                                                                  /* SBSW_OS_CORE_CORE2ASRCORE_002 */

      /* #20 Check that StartOS() was not called before. */
      if(OS_UNLIKELY(Os_CoreCheckOsIsNotStarted(coreAsr) == OS_CHECK_FAILED))                                           /* SBSW_OS_CORE_CORECHECKOSISNOTSTARTED_001 */ /* COV_OS_INVSTATE */
      {
        Os_ErrKernelPanic();
      }
      else
      {
        /* #30 Enter the core's Init Hook. */
        Os_HookCallOs_CoreInitHook(Os_CoreGetInitHook(coreAsr), Mode);                                                  /* SBSW_OS_CORE_COREGETINITHOOK_001 */ /* SBSW_OS_CORE_HOOKCALLOS_COREINITHOOK_001 */
      }
    }
  }
  /* #50 Else ignore the call. */
}


/***********************************************************************************************************************
 *  Os_CoreInitHook()
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
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
FUNC(void, OS_CODE) Os_CoreInitHook
(
  AppModeType Mode
)
{
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) coreIt;
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsrIt;
  CoreIdType it;

  coreAsr = Os_Core2AsrCore(Os_CoreGetCurrentCore());                                                                   /* SBSW_OS_CORE_CORE2ASRCORE_002 */

  /* #10 Update the core's app mode. */
  Os_CoreAsrCombineApplicationMode(coreAsr, Mode);                                                                      /* SBSW_OS_CORE_COREASRCOMBINEAPPLICATIONMODE_001 */

  /* #20 Wait until all cores have updated their app mode. */
  Os_BarrierSynchronizeInternal(coreAsr->Barrier);                                                                      /* SBSW_OS_CORE_BARRIERSYNCHRONIZE_002 */

  /* #30 Calculate the overall app mode. */
  for(it = (CoreIdType)0; it < OS_COREID_COUNT; ++it)                                                                   /* PRQA S 4332,  4527 */ /* MD_Os_Rule10.5_4332,  MD_Os_Rule10.1_4527 */
  {
    coreIt = Os_CoreId2Core(it);
    if(Os_CoreIsAsrCore(coreIt) != 0u)                                                                                  /* SBSW_OS_CORE_COREISASRCORE_001 */
    {
      coreAsrIt = Os_Core2AsrCore(coreIt);                                                                              /* SBSW_OS_CORE_CORE2ASRCORE_001 */
      Os_CoreAsrCombineApplicationMode(coreAsr, Os_CoreAsrGetApplicationMode(coreAsrIt));                               /* SBSW_OS_CORE_COREASRCOMBINEAPPLICATIONMODE_001 */ /* SBSW_OS_CORE_COREASRGETAPPLICATIONMODE_001 */
    }
  }

  /* #40 Freeze if the overall app mode is invalid. */
  if(OS_UNLIKELY(Os_CoreCheckIsAppModeValid(Os_CoreAsrGetApplicationMode(coreAsr)) == OS_CHECK_FAILED))                 /* SBSW_OS_CORE_COREASRGETAPPLICATIONMODE_001 */
  {
    Os_ErrKernelPanic();
  }

  /* #50 If the Pre-Start task has not been started. */
  if(Os_CoreAsrGetAsrStatus(coreAsr)->OsState != OS_COREOSSTATE_PRESTARTTASK)                                           /* SBSW_OS_CORE_COREASR_STATUS_001 */
  {
    P2CONST(Os_Hal_ContextConfigType, TYPEDEF, OS_CONST) context;
    Os_Hal_ContextStackConfigType stack;

    context = Os_ThreadGetContext(Os_CoreGetThread());                                                                  /* SBSW_OS_CORE_THREADGETCONTEXT_001 */
    Os_Hal_ContextGetStack(context, &stack);                                                                            /* SBSW_OS_FC_POINTER2LOCAL */

    /* #60 Perform pre-start initialization. */
    Os_CorePreStartInit(&stack);                                                                                        /* SBSW_OS_FC_POINTER2LOCAL */
  }

  /* #70 Perform rest of initialization. */
  Os_CoreInit();

  /* The switch to idle task is done in Os_HookReturn -> Os_TaskBeginScheduling */
}


/***********************************************************************************************************************
 *  Os_Api_ShutdownOS()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, OS_CODE) Os_Api_ShutdownOS
(
  StatusType Error
)
{
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) currentThread;

  currentThread = Os_CoreGetThread();

  if(OS_UNLIKELY(Os_ThreadCheckCallContext(currentThread, OS_APICONTEXT_SHUTDOWNOS) == OS_CHECK_FAILED))                /* SBSW_OS_CORE_THREADCHECKCALLCONTEXT_001 */
  {
    Os_ErrProtectionError(OS_STATUS_CALLEVEL);
  }
  else
  {
    P2CONST(Os_AppConfigType, AUTOMATIC, OS_CONST) currentApp;

    currentApp = Os_ThreadGetCurrentApplication(currentThread);                                                         /* SBSW_OS_CORE_THREADGETCURRENTAPPLICATION_001 */

    /* #10 If the current application is trusted: */
    if(OS_UNLIKELY(Os_AppIsTrusted(currentApp) !=  0u))                                                                 /* SBSW_OS_CORE_APPISTRUSTED_001 */
    {
      /* #20 Shutdown local core without shutdown synchronization. */
      Os_CoreShutdown(Error, FALSE);
    }
  }
  /* #30 Else ignore the call. */
}


/***********************************************************************************************************************
 *  Os_Api_ShutdownAllCores()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, OS_CODE) Os_Api_ShutdownAllCores
(
  StatusType Error
)
{
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) currentThread;

  currentThread = Os_CoreGetThread();

  if(OS_UNLIKELY(Os_ThreadCheckCallContext(currentThread, OS_APICONTEXT_SHUTDOWNALLCORES) == OS_CHECK_FAILED))          /* SBSW_OS_CORE_THREADCHECKCALLCONTEXT_001 */ /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
  {
    Os_ErrProtectionError(OS_STATUS_CALLEVEL);
  }
  else
  {
    P2CONST(Os_AppConfigType, AUTOMATIC, OS_CONST) currentApp;

    currentApp = Os_ThreadGetCurrentApplication(currentThread);                                                         /* SBSW_OS_CORE_THREADGETCURRENTAPPLICATION_001 */

    /* #10 If the current application is trusted: */
    if(OS_UNLIKELY(Os_AppIsTrusted(currentApp) !=  0u))                                                                 /* SBSW_OS_CORE_APPISTRUSTED_001 */
    {
      /* #20 Shutdown the system. */
      Os_SystemShutdown(Error);
    }
  }
  /* #30 Else ignore the call. */
}


/***********************************************************************************************************************
 *  Os_Api_ControlIdle()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050, 6080 1 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(Os_StatusType, OS_CODE) Os_Api_ControlIdle
(
  CoreIdType CoreID,
  IdleModeType IdleMode
)
{
  Os_StatusType status;
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) currentThread;

  currentThread = Os_CoreGetThread();

  /* #10 Perform error checks. */
  if(OS_UNLIKELY(Os_CoreCheckId(CoreID) == OS_CHECK_FAILED))                                                            /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
  {
    status = OS_STATUS_ID_1;
  }
  else if(OS_UNLIKELY(Os_CoreCheckIdleMode(IdleMode) == OS_CHECK_FAILED))                                               /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
  {
    status = OS_STATUS_ID_2;
  }
  else if(OS_UNLIKELY(Os_ThreadCheckCallContext(currentThread, OS_APICONTEXT_CONTROLIDLE) == OS_CHECK_FAILED))          /* COV_OS_HALPLATFORMTESTEDASMULTICORE */ /* SBSW_OS_CORE_THREADCHECKCALLCONTEXT_001 */
  {
    status = OS_STATUS_CALLEVEL;
  }
  else if(OS_UNLIKELY(Os_ThreadCheckAreInterruptsEnabled(currentThread) == OS_CHECK_FAILED))                            /* SBSW_OS_CORE_THREADCHECKAREINTERRUPTSENABLED_001 */ /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
  {
    status = OS_STATUS_DISABLEDINT;
  }
  else
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) targetCore;

    targetCore = Os_CoreId2Core(CoreID);

    if(OS_UNLIKELY(Os_CoreCheckIsAsrCore(targetCore) == OS_CHECK_FAILED))                                               /* SBSW_OS_CORE_COREISASRCORE_001 */ /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
    {
      status = OS_STATUS_CORE;
    }
    else
    {
      P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) targetAsrCore;

      targetAsrCore = Os_Core2AsrCore(targetCore);                                                                      /* SBSW_OS_CORE_CORE2ASRCORE_001 */

      /* #20 If the target core is the local core, set the idle mode. */
      if(OS_LIKELY(Os_CoreAsrIsCoreLocal(targetAsrCore) != 0u))                                                         /* SBSW_OS_CORE_COREASRISCORELOCAL_001 */ /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */ /* PRQA S 2991, 2995 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule2.2_2995 */
      {
        status = Os_CoreControlIdle(targetAsrCore, IdleMode);                                                           /* SBSW_OS_CORE_CORECONTROLIDLE_001 */
      }
      /* #30 Otherwise, send a cross core request. */
      else
      {
        status = Os_XSigSend_ControlIdle(targetAsrCore, currentThread, CoreID, IdleMode);                               /* SBSW_OS_CORE_XSIGSEND_001 */
      }
    }
  }

  return status;
}


/***********************************************************************************************************************
 *  Os_UnhandledIrq()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(void, OS_CODE, OS_NORETURN, Os_UnhandledIrq,
(
  Os_InterruptSourceIdType InterruptSource,
  Os_AddressOfConstType ExceptionInstructionAddress,
  boolean ExceptionCauserPrivileged
))
{
  /* #10 If the OS is not configured for the interrupt only use case: */
  if(Os_CoreIsInterruptOnlyOs() == 0u )                                                                                 /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) core;
    P2VAR(Os_CoreAsrType, AUTOMATIC, OS_VAR_NOINIT) coreDyn;

    core = Os_Core2AsrCore(Os_CoreGetCurrentCore());                                                                    /* SBSW_OS_CORE_CORE2ASRCORE_002 */
    coreDyn = Os_CoreAsrGetDyn(core);                                                                                   /* SBSW_OS_CORE_COREASRGETDYN_002 */

    /* #20 Set last unhandled interrupt source ID to given ID. */
    coreDyn->LastUnhandledIrq = InterruptSource;                                                                        /* SBSW_OS_CORE_COREASR_DYN_001 */
    coreDyn->UnhandledIrqReported = OS_UNHANDLEDEVENTREPORT_AVAILABLE;                                                  /* SBSW_OS_CORE_COREASR_DYN_001 */

    /* #30 Report exception to error handling. */
    Os_ErrProtectionException(OS_STATUS_PROTECTION_IRQ, ExceptionInstructionAddress, ExceptionCauserPrivileged);        /* SBSW_OS_CORE_PROTECTIONEXCEPTION_001 */
  }

  /* #40 Enter Kernel Panic. */
  Os_ErrKernelPanic();
}


/***********************************************************************************************************************
 *  Os_Api_GetUnhandledIrq()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Os_StatusType, OS_CODE) Os_Api_GetUnhandledIrq
(
  Os_InterruptSourceIdRefType InterruptSource
)
{
  Os_StatusType status;
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;

  core = Os_CoreGetCurrentCore();

  /* #10 Perform checks. */
  if(OS_UNLIKELY(Os_CoreCheckIsAsrCore(core) == OS_CHECK_FAILED))                                                       /* SBSW_OS_CORE_CORECHECKISASRCORE_003 */ /* COV_OS_NOFUNCCALL */
  {
    status = OS_STATUS_CORE;
  }
  else
  {
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    P2CONST(Os_CoreAsrType, AUTOMATIC, OS_VAR_NOINIT) coreAsrDyn;

    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_CORE_CORE2ASRCORE_002 */
    coreAsrDyn = Os_CoreAsrGetDyn(coreAsr);                                                                             /* SBSW_OS_CORE_COREASRGETDYN_002 */

    if(OS_UNLIKELY(coreAsrDyn->UnhandledIrqReported != OS_UNHANDLEDEVENTREPORT_AVAILABLE))                              /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
    {
      status = OS_STATUS_STATE_1;
    }
    else
    {
      /* #20 Set out parameter InterruptSource to unhandled interrupt source ID. */
      *InterruptSource = coreAsrDyn->LastUnhandledIrq;                                                                  /* SBSW_OS_PWA_PRECONDITION */
      status = OS_STATUS_OK;
    }
  }

  return status;
}


/***********************************************************************************************************************
 *  Os_UnhandledExc()
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
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
FUNC(void, OS_CODE) Os_UnhandledExc
(
  Os_ExceptionSourceIdType ExceptionSource,
  Os_AddressOfConstType ExceptionInstructionAddress,
  boolean ExceptionCauserPrivileged
)
{
  /* #05 If the OS is not configured for the interrupt only use case: */
  if(Os_CoreIsInterruptOnlyOs() == 0u )                                                                                 /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
  {
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) core;
    P2VAR(Os_CoreAsrType, AUTOMATIC, OS_VAR_NOINIT) coreDyn;

    /* #10 switch from global interrupt lock to level lock if needed. */
    Os_IntDisable();

    if(Os_InterruptLockOnLevelIsNeeded() != 0u)                                                                         /* COV_OS_HALPLATFORMLEVELSUPPORT */ /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
    {
      if(Os_IntIsGlobal2LevelSwitchSupported() != 0u)                                                                   /* COV_OS_HALPLATFORMGLOBAL2LEVELSUPPORTED */ /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
      {
        Os_IntEnableGlobal();
      }
    }

    core = Os_Core2AsrCore(Os_CoreGetCurrentCore());                                                                    /* SBSW_OS_CORE_CORE2ASRCORE_002 */
    coreDyn = Os_CoreAsrGetDyn(core);                                                                                   /* SBSW_OS_CORE_COREASRGETDYN_002 */

    /* #20 Inform the local core about the stack switch performed in HAL. */
    if(Os_StackIsMonitoringEnabled() != 0u)                                                                             /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
    {
      Os_CoreSetCurrentStack(Os_CoreGetKernelStack(core), core);                                                        /* SBSW_OS_CORE_GETKERNELSTACK_001 */ /* SBSW_OS_CORE_SETCURRENTSTACK_002 */
    }

    /* #30 Set last unhandled exception source ID to given ID. */
    coreDyn->LastUnhandledExc = ExceptionSource;                                                                        /* SBSW_OS_CORE_COREASR_DYN_001 */
    coreDyn->UnhandledExcReported = OS_UNHANDLEDEVENTREPORT_AVAILABLE;                                                  /* SBSW_OS_CORE_COREASR_DYN_001 */

    /* #40 Report exception to error handling. */
    Os_ErrProtectionException(OS_STATUS_PROTECTION_EXCEPTION, ExceptionInstructionAddress, ExceptionCauserPrivileged);  /* SBSW_OS_CORE_PROTECTIONEXCEPTION_001 */
  }else{
    /* #50 Otherwise, enter Kernel Panic. */
    Os_ErrKernelPanic();
  }

}


/***********************************************************************************************************************
 *  Os_Api_GetUnhandledExc()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Os_StatusType, OS_CODE) Os_Api_GetUnhandledExc
(
  Os_ExceptionSourceIdRefType ExceptionSource
)
{
  Os_StatusType status;
  P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;

  core = Os_CoreGetCurrentCore();

  /* #10 Perform checks. */
  if(OS_UNLIKELY(Os_CoreCheckIsAsrCore(core) == OS_CHECK_FAILED))                                                       /* SBSW_OS_CORE_CORECHECKISASRCORE_003 */ /* COV_OS_NOFUNCCALL */
  {
    status = OS_STATUS_CORE;
  }
  else
  {
    P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) coreAsr;
    P2CONST(Os_CoreAsrType, AUTOMATIC, OS_VAR_NOINIT) coreAsrDyn;

    coreAsr = Os_Core2AsrCore(core);                                                                                    /* SBSW_OS_CORE_CORE2ASRCORE_002 */
    coreAsrDyn = Os_CoreAsrGetDyn(coreAsr);                                                                             /* SBSW_OS_CORE_COREASRGETDYN_002 */

    if(OS_UNLIKELY(coreAsrDyn->UnhandledExcReported != OS_UNHANDLEDEVENTREPORT_AVAILABLE))                              /* COV_OS_HALPLATFORMTESTEDASMULTICORE */
    {
      status = OS_STATUS_STATE_1;
    }
    else
    {
      /* #20 Set out parameter ExceptionSource to unhandled exception source ID. */
      *ExceptionSource = coreAsrDyn->LastUnhandledExc;                                                                  /* SBSW_OS_PWA_PRECONDITION */
      status = OS_STATUS_OK;
    }
  }

  return status;
}


/***********************************************************************************************************************
 *  Os_Api_GetCoreStartState()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, OS_CODE) Os_Api_GetCoreStartState
(
  CoreIdType CoreID,
  Os_CoreStartStateType *CoreState,
  StatusType *Status
)
{
  /* #10 Perform error checks. */
  if(OS_UNLIKELY(Os_CoreCheckId(CoreID) == OS_CHECK_FAILED ))
  {
    *Status = E_OS_ID;                                                                                                  /* SBSW_OS_PWA_PRECONDITION */
  }
  else
  {
    P2CONST(Os_CoreConfigType, AUTOMATIC, OS_CONST) core;
    core = Os_CoreId2Core(CoreID);

    *Status = E_OK;                                                                                                     /* SBSW_OS_PWA_PRECONDITION */

    /* #20 Read the core startup state. */
    *CoreState = Os_CoreGetStartState(core);                                                                            /* SBSW_OS_PWA_PRECONDITION */ /* SBSW_OS_CORE_COREGETSTARTSTATE_001 */
  }
}


/***********************************************************************************************************************
 *  Os_UnhandledSysCall()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(void, OS_CODE, OS_NORETURN, Os_UnhandledSysCall,                                           /* COV_OS_HALPLATFORMUNHANDLEDSYSCALL */
(
  Os_AddressOfConstType ExceptionInstructionAddress,
  boolean ExceptionCauserPrivileged
))                                                                                                                      /* COV_OS_HALPLATFORMUNHANDLEDSYSCALL */
{
  /* #10 If the OS is not configured for the interrupt only use case: */
  if(Os_CoreIsInterruptOnlyOs() == 0u )                                                                                 /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
  {
    /* #20 switch from global interrupt lock to level lock if needed. */
    Os_IntDisable();

    if(Os_InterruptLockOnLevelIsNeeded() != 0u)                                                                         /* COV_OS_HALPLATFORMLEVELSUPPORT */ /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
    {
      if(Os_IntIsGlobal2LevelSwitchSupported() != 0u)                                                                   /* COV_OS_HALPLATFORMGLOBAL2LEVELSUPPORTED */ /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule14.3_2992, MD_Os_Rule2.2_2995, MD_Os_Rule2.2_2996 */
      {
        Os_IntEnableGlobal();
      }
    }

    /* #30 Report exception to error handling. */
    Os_ErrProtectionException(OS_STATUS_PROTECTION_SYSCALL, ExceptionInstructionAddress, ExceptionCauserPrivileged);    /* SBSW_OS_CORE_PROTECTIONEXCEPTION_001 */
  }

  /* #40 Enter Kernel Panic. */
  Os_ErrKernelPanic();
}


/***********************************************************************************************************************
 *  Os_CoreControlIdle()
 **********************************************************************************************************************/
FUNC(Os_StatusType, OS_CODE) Os_CoreControlIdle
(
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) Core,
  IdleModeType IdleMode
)
{
  Os_CoreAsrGetDyn(Core)->IdleMode = IdleMode;                                                                          /* SBSW_OS_FC_PRECONDITION */ /* SBSW_OS_CORE_COREASRGETDYN_003 */

  return OS_STATUS_OK;
}


#define OS_STOP_SEC_CODE
#include "Os_MemMap_OsCode.h"                                                                                           /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* module specific MISRA deviations:
 */

/*
 * Review Helper - Naming convention:
 * Start every justification with SWSW_OS_<Message>_<Name>.
 * <Message>:
 *  - AWA: Array write access
 *  - AI:  Assembler instruction
 *  - FC:  Function call
 *  - FPC: Function pointer call
 *  - PWA: Pointer write access
 *
 * <Name>:
 *  A short name which refers to the justification counter measure.
 */

/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_OS_MEMAP_UNDEF
 \DESCRIPTION    Undefine of memory start/end section.
 \COUNTERMEASURE \N No measure required. Undefine is required by AUTOSAR and has no influence on review helper results.

\ID SBSW_OS_HAL_EXAMPLE
 \DESCRIPTION    Function call is used as an example.
 \COUNTERMEASURE \N No measure required, as the sample call is either replaced by the HAL implementation or justified.

\ID SBSW_OS_PWA_LOCAL
 \DESCRIPTION    Write access to a local array.
 \COUNTERMEASURE \N No measure required, as the used index is static and in range.


\ID SBSW_OS_FC_PRECONDITION_AND_CHECK_IF_NEEDED
 \DESCRIPTION    The pointer passed to the called function is derived from an argument passed to the caller function.
                 As the caller of the caller function is enforced by preconditions to pass valid arguments only or
                 NULL_PTR if allowed by global config, the pointer is correct. Preconditions are checked during code
                 review.
 \COUNTERMEASURE \N [CM_OS_PRECONDITION_AND_CHECK_IF_NEEDED_N]

\ID SBSW_OS_FC_PRECONDITION_AND_CHECK
 \DESCRIPTION    The pointer passed to the called function is derived from an argument passed to the caller function.
                 As the caller of the caller function is enforced by preconditions to pass valid arguments only or
                 NULL_PTR, the pointer is correct. Preconditions are checked during code review.
 \COUNTERMEASURE \N [CM_OS_PRECONDITION_AND_CHECK_N]

\ID SBSW_OS_PWA_PRECONDITION
 \DESCRIPTION    Write access to a function argument. Function design requires the caller to pass a valid pointer.
 \COUNTERMEASURE \N [CM_OS_PRECONDITION_N]

\ID SBSW_OS_FC_PRECONDITION
 \DESCRIPTION    The pointer passed to the called function is derived from an argument passed to the caller function.
                 As the caller of the caller function is enforced by preconditions or by parameter limitations to pass
                 valid arguments only, the pointer is correct. Preconditions and parameter limitations are checked
                 during code review.
 \COUNTERMEASURE \N None, as the pointer validity is guaranteed.

\ID SBSW_OS_PWA_RETURNEDPOINTER
 \DESCRIPTION    Write access to a pointer returned by a function.
 \COUNTERMEASURE \N No measure required, as the pointer is returned by a function which returns valid pointer only.

\ID SBSW_OS_FC_RETURNEDPOINTER
 \DESCRIPTION    The passed pointer is derived from a function, which guarantees that the pointer is valid.
 \COUNTERMEASURE \N No measure required, as the pointer is returned by a function which returns valid pointer only.

\ID SBSW_OS_FC_RETURNEDPOINTERWITHCHECK
 \DESCRIPTION    The passed pointer is derived from a function, which guarantees that the pointer is valid or a
                 NULL_PTR.
 \COUNTERMEASURE \N No measure required, as the pointer is returned by a function which returns valid pointer only.

\ID SBSW_OS_FC_POINTER2LOCAL
 \DESCRIPTION    Pass a reference to a local variable.
 \COUNTERMEASURE \N No measure required, as the passed pointer refers to a local variable and the called function
                    allows that the existence of the local variable ends on return.

\ID SBSW_OS_FC_POINTER2ARGUMENT
 \DESCRIPTION    Pass a reference to a variable which is passed as argument to the caller.
 \COUNTERMEASURE \N No measure required, as the passed pointer refers to an argument and the called function
                    allows that the existence of the local variable ends on return.

\ID SBSW_OS_FC_POINTER2GLOBAL
 \DESCRIPTION    Pass a reference to a global variable.
 \COUNTERMEASURE \N No measure required, as the passed pointer refers to a global variable.

\ID SBSW_OS_FC_ITERATOR2POINTER
 \DESCRIPTION    A pointer, which is derived from an iterator, is passed to a function.
 \COUNTERMEASURE \N No measure required, as the pointer is return by a function, which ensures by preconditions, that
                    the returned pointer is valid.

\ID SBSW_OS_PWA_ABSTRACT_DYN
 \DESCRIPTION    Write access to Dyn object of a abstract class.
 \COUNTERMEASURE \N [CM_OS_ABSTRACT_DYN_N]

\ID SBSW_OS_CORE_SYSTEMGETDYN_001
 \DESCRIPTION    Os_SystemGetDyn is called with the return value of Os_SystemGet.
 \COUNTERMEASURE \N [CM_OS_SYSTEMGET_N]

\ID SBSW_OS_CORE_HAL_COREINIT_001
 \DESCRIPTION    Os_Hal_CoreInit is called with the HwConfig derived from a caller argument.
 \COUNTERMEASURE \M [CM_OS_COREASR_HWCONFIG_M]

\ID SBSW_OS_CORE_HAL_COREINIT_002
 \DESCRIPTION    Os_Hal_CoreInit is called with the HwConfig derived from the return value of Os_Core2AsrCore.
                 See [CM_OS_CORE2ASRCORE_N].
 \COUNTERMEASURE \M [CM_OS_COREASR_HWCONFIG_M]

\ID SBSW_OS_CORE_HAL_CORESTART_001
 \DESCRIPTION    Os_Hal_CoreStart is called with the HwConfig derived from the return value of Os_CoreId2Core.
                 See [CM_OS_COREID2CORE_M].
 \COUNTERMEASURE \M [CM_OS_CORE_HWCONFIG_M]

\ID SBSW_OS_CORE_INTERRUPTINIT_001
 \DESCRIPTION    Os_InterruptInit is called with a pointer derived from the return value of Os_CoreAsrGetDyn.
 \COUNTERMEASURE \M [CM_OS_COREASRGETDYN_M]

\ID SBSW_OS_CORE_HOOKINFOINIT_001
 \DESCRIPTION    Os_HookInfoInit is called with a pointer derived from the return value of Os_CoreAsrGetDyn.
 \COUNTERMEASURE \M [CM_OS_COREASRGETDYN_M]

\ID SBSW_OS_CORE_ERRINIT_001
 \DESCRIPTION    Os_ErrInit is called with a pointer derived from the return value of Os_CoreAsrGetDyn.
 \COUNTERMEASURE \M [CM_OS_COREASRGETDYN_M]

\ID SBSW_OS_CORE_ERRGETEXCEPTIONINSTRUCTIONADDRESS_001
 \DESCRIPTION    Os_ErrGetExceptionInstructionAddress is called with a pointer derived from the return value of
                 Os_CoreAsrGetDyn.
 \COUNTERMEASURE \M [CM_OS_COREASRGETDYN_M]

\ID SBSW_OS_CORE_TASKGETTHREAD_001
 \DESCRIPTION    Os_TaskGetThread is called with the IdleTask derived from the caller argument Core.
                 Precondition ensures that the Core pointer is valid. The compliance of the precondition is check during
                 review.
 \COUNTERMEASURE \M [CM_OS_COREASR_IDLETASK_M]

\ID SBSW_OS_CORE_TASKSETTHREAD_001
 \DESCRIPTION    Os_CoreSetThread is called with the return value of Os_TaskGetThread.
 \COUNTERMEASURE \N [CM_OS_TASKGETTHREAD_N]

\ID SBSW_OS_CORE_THREADINIT2_001
 \DESCRIPTION    Os_ThreadInit2 is called with the return value of Os_TaskGetThread.
 \COUNTERMEASURE \N [CM_OS_TASKGETTHREAD_N]

\ID SBSW_OS_CORE_CORE2ASRCORE_001
 \DESCRIPTION    Os_Core2AsrCore is called with the return value of Os_CoreId2Core.
 \COUNTERMEASURE \M [CM_OS_COREID2CORE_M]

\ID SBSW_OS_CORE_CORE2ASRCORE_002
 \DESCRIPTION    Os_Core2AsrCore is called with the return value of Os_CoreGetCurrentCore.
 \COUNTERMEASURE \S [CM_OS_COREGETCURRENTCORE_S]

\ID SBSW_OS_CORE_COREGETLASTERROR_001
 \DESCRIPTION    Os_CoreGetLastError is called with the return value of Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]

\ID SBSW_OS_CORE_STACKINIT_001
 \DESCRIPTION    Os_StackInit is called with an entry of StackRefs. The used StackRefs index is limited to StackCount.
                 StackRefs is derived from a core object returned from Os_Core2AsrCore.
                 Precondition of Os_Core2AsrCore ensures that the returned Core pointer is valid. The compliance of the
                 precondition is check during review. [CM_OS_CORE2ASRCORE_N]
 \COUNTERMEASURE \M [CM_OS_COREASR_STACKREFS_M]

\ID SBSW_OS_CORE_ISRINTERRUPTSOURCEINIT_001
 \DESCRIPTION    Os_IsrInterruptSourceInit is called with an entry of InterruptSourceRefs. The used
                 InterruptSourceRefs index is limited to InterruptSourceCount. Precondition ensures that
                 the parameter pointer is valid. The compliance of the precondition is check during review.
 \COUNTERMEASURE \M [CM_OS_COREASR_INTERRUPTSOURCEREFS_M]
                 \M [CM_OS_SYSTEM_INTERRUPTSOURCEREFS_M]

\ID SBSW_OS_CORE_MPSYSTEMINIT_001
 \DESCRIPTION    Os_MpSystemInit is called with the MemoryProtection of the system object returned by Os_SystemGet.
 \COUNTERMEASURE \M [CM_OS_SYSTEM_MEMORYPROTECTION_M]

\ID SBSW_OS_CORE_MPCOREINIT_001
 \DESCRIPTION    Os_MpCoreInit is called with the MemoryProtection of a core object returned by Os_Core2AsrCore.
                 Precondition of Os_Core2AsrCore ensures that the returned Core pointer is valid. The compliance of the
                 precondition is check during review.
 \COUNTERMEASURE \M [CM_OS_COREASR_MEMORYPROTECTION_M]

\ID SBSW_OS_CORE_TPREDUCEDINIT_001
 \DESCRIPTION    Os_TpReducedInit is called with the return value of Os_CoreAsrGetTimingProtection and Os_TaskGetThread.
                 Precondition ensures the return value of Os_TaskGetThread.
 \COUNTERMEASURE \M [CM_OS_COREASRGETTIMINGPROTECTION1_M]

\ID SBSW_OS_CORE_TPINIT_001
 \DESCRIPTION    Os_TpInit is called with the return value of Os_CoreAsrGetTimingProtection.
 \COUNTERMEASURE \M [CM_OS_COREASRGETTIMINGPROTECTION1_M]

\ID SBSW_OS_CORE_COREASRGETTIMINGPROTECTION_001
 \DESCRIPTION    Os_CoreAsrGetTimingProtection is called with the return value of Os_Core2AsrCore.
 \COUNTERMEASURE \N [CM_OS_CORE2ASRCORE_N]

\ID SBSW_OS_CORE_TPTHREADINIT_001
 \DESCRIPTION    Os_TpThreadInit is called with the return values of Os_CoreAsrGetTimingProtection and
                 Os_ThreadGetTpConfig.
 \COUNTERMEASURE \M [CM_OS_COREASRGETTIMINGPROTECTION1_M]
                 \M [CM_OS_THREADGETTPCONFIG1_M]

\ID SBSW_OS_CORE_COREGETINITHOOK_001
 \DESCRIPTION    Os_CoreGetInitHook is called with the return value of Os_Core2AsrCore.
 \COUNTERMEASURE \N [CM_OS_CORE2ASRCORE_N]

\ID SBSW_OS_CORE_SCHEDULERINIT_001
 \DESCRIPTION    Os_SchedulerInit is called with the Scheduler and the IdleTask of a core object returned by
                 Os_Core2AsrCore.
                 Precondition of Os_Core2AsrCore ensures that the returned Core pointer is valid. The compliance of the
                 precondition is check during review.
 \COUNTERMEASURE \M [CM_OS_COREGETSCHEDULER_M]
                 \M [CM_OS_COREASR_IDLETASK_M]

\ID SBSW_OS_CORE_SPINLOCKINIT_001
 \DESCRIPTION    Os_SpinlockInit is called with an entry of SpinlokRefs. The used SpinlokRefs index is limited to
                 SpinlockCount. SpinlockRefs is derived from a system object given as argument.
 \COUNTERMEASURE \M [CM_OS_SYSTEM_SPINLOCKREFS_M]

\ID SBSW_OS_CORE_THREADGETCORE_001
 \DESCRIPTION    Os_ThreadGetCore is called with the return value of Os_CoreGetThread.
 \COUNTERMEASURE \N [CM_OS_COREGETTHREAD_N]

\ID SBSW_OS_CORE_THREADGETCONTEXT_001
 \DESCRIPTION    Os_ThreadGetContext is called with the return value of Os_CoreGetThread.
 \COUNTERMEASURE \N [CM_OS_COREGETTHREAD_N]

\ID SBSW_OS_CORE_THREADISANYINITIALCONTEXT_001
 \DESCRIPTION    Os_ThreadIsAnyInitialContext is called with the return value of Os_CoreGetThread.
 \COUNTERMEASURE \N [CM_OS_COREGETTHREAD_N]

\ID SBSW_OS_CORE_THREADISANYINITIALCONTEXT_002
 \DESCRIPTION    Os_ThreadIsAnyInitialContext is called with the return value of Os_CoreInterruptedThreadsGet.
 \COUNTERMEASURE \R [CM_COREINTERRUPTEDTHREADSGET_R]

\ID SBSW_OS_CORE_COREGETSTATUS_001
 \DESCRIPTION    Os_CoreGetStatus is called with the return value of Os_CoreId2Core.
 \COUNTERMEASURE \M [CM_OS_COREID2CORE_M]

\ID SBSW_OS_CORE_COREGETSTATUS_002
 \DESCRIPTION    Write access to the return value of Os_CoreGetStatus.
 \COUNTERMEASURE \M [CM_OS_COREGETSTATUS_M]

\ID SBSW_OS_CORE_COREGETSTATUS_003
 \DESCRIPTION    Os_CoreGetStatus is called with the return value of Os_CoreGetCurrentCore.
 \COUNTERMEASURE \S [CM_OS_COREGETCURRENTCORE_S]

\ID SBSW_OS_CORE_HAL_CONTEXTGETSTACK_001
 \DESCRIPTION    Os_Hal_ContextGetStack is called with return value of Os_ThreadGetContext.
 \COUNTERMEASURE \M [CM_OS_THREADGETCONTEXT_M]

\ID SBSW_OS_CORE_SYSTEMINTERRUPTHANDLINGINIT_001
 \DESCRIPTION    Os_SystemInterruptHandlingInit is called with the return value of Os_SystemGet.
                 Return value of Os_SystemGet is always correct.
 \COUNTERMEASURE \N [CM_OS_SYSTEMGET_N]

\ID SBSW_OS_CORE_COREINTERRUPTHANDLINGINIT_001
 \DESCRIPTION    Os_CoreInterruptHandlingInit is called with the return value of Os_CoreGetCurrentCore.
 \COUNTERMEASURE \S [CM_OS_COREGETCURRENTCORE_S]

\ID SBSW_OS_CORE_COREASRGETDYN_001
 \DESCRIPTION    Os_CoreAsrGetDyn is called with the return value of Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]

\ID SBSW_OS_CORE_COREASRGETDYN_002
 \DESCRIPTION    Os_CoreAsrGetDyn is called with the return value of Os_Core2AsrCore.
 \COUNTERMEASURE \N [CM_OS_CORE2ASRCORE_N]

\ID SBSW_OS_CORE_COREASRGETDYN_003
 \DESCRIPTION    Write access to the return value of Os_CoreAsrGetDyn.
 \COUNTERMEASURE \M [CM_OS_COREASRGETDYN_M]

\ID SBSW_OS_CORE_COREASR_DYN_001
 \DESCRIPTION    Write access to the return value of Os_CoreAsrGetDyn.
 \COUNTERMEASURE \M [CM_OS_COREASRGETDYN_M]

\ID SBSW_OS_CORE_COREASR_STATUS_001
 \DESCRIPTION    Os_CoreAsrGetAsrStatus is called with the return value of Os_Core2AsrCore.
 \COUNTERMEASURE \N [CM_OS_CORE2ASRCORE_N]

\ID SBSW_OS_CORE_COREASR_STATUS_002
 \DESCRIPTION    Write access to the return value of Os_CoreAsrGetAsrStatus.
 \COUNTERMEASURE \M [CM_OS_COREASRGETSTATUS_M]

\ID SBSW_OS_CORE_COREASR_STATUS_003
 \DESCRIPTION    Os_CoreAsrGetAsrStatus is called with the return value of Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]

\ID SBSW_OS_CORE_RESOURCEINIT_001
 \DESCRIPTION    Os_ResourceInit is called with an entry of ResourceRefs. The used ResourceRefs index is limited to
                 ResourceCount. ResourceCount and ResourceRefs are derived from a core returned from Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]
                 \M [CM_OS_COREASR_RESOURCEREFS_M]

\ID SBSW_OS_CORE_IOCINIT
 \DESCRIPTION    Os_IocInit is called with an entry of IocRefs. The used IocRefs index is limited to
                 IocCount. IocCount and IocRefs are derived from a core returned from Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]
                 \M [CM_OS_COREASR_IOCREFS_M]

\ID SBSW_OS_CORE_APPINIT_001
 \DESCRIPTION    Os_AppInit is called with an entry of AppRefs. The used AppRefs index is limited to
                 AppCount. AppCount and AppRefs are derived from a core returned from Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]
                 \M [CM_OS_COREASR_APPREFS_M]

\ID SBSW_OS_CORE_APPINIT_002
 \DESCRIPTION    Os_AppInit is called with the KernelApp derived from the return value of Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]
                 \M [CM_OS_COREASR_KERNELAPP_M]

\ID SBSW_OS_CORE_XSIGINIT_001
 \DESCRIPTION    Os_XSigInit is called with the XSignal derived from the return value of Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]
                 \M [CM_OS_COREASR_XSIGNAL_M]

\ID SBSW_OS_CORE_BARRIERATTACH_001
 \DESCRIPTION    Os_BarrierAttach is called with the Barrier object derived from the return value of
                 Os_Core2AsrCore.
 \COUNTERMEASURE \M [CM_OS_CORE_BARRIER_M]
                 \R [CM_OS_CORE2ASRCORE_R]

\ID SBSW_OS_CORE_BARRIERATTACHUSERBARRIER_001
 \DESCRIPTION    Os_BarrierAttachUserBarrier is called with the Barrier references object derived from the return value of
                 Os_ThreadGetCore.
 \COUNTERMEASURE \R [CM_OS_CORE_BARRIERREF_R]
                 \M [CM_OS_COREASR_BARRIERREFS_M]
                 \M [CM_OS_THREADGETCORE_M]

\ID SBSW_OS_CORE_BARRIERDETACH_001
 \DESCRIPTION    Os_BarrierDetach is called with the Barrier object derived from the return value of
                 Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_CORE_BARRIER_M]
                 \M [CM_OS_THREADGETCORE_M]

\ID SBSW_OS_CORE_BARRIERSYNCHRONIZE_001
 \DESCRIPTION    Os_BarrierSynchronizeInternal is called with the Barrier object derived from the return value of
                 Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_CORE_BARRIER_M]
                 \M [CM_OS_THREADGETCORE_M]

\ID SBSW_OS_CORE_BARRIERSYNCHRONIZE_002
 \DESCRIPTION    Os_BarrierSynchronizeInternal is called with the Barrier object derived from the return value of
                 Os_Core2AsrCore.
 \COUNTERMEASURE \M [CM_OS_CORE_BARRIER_M]
                 \R [CM_OS_CORE2ASRCORE_R]

\ID SBSW_OS_CORE_HOOKCALLCALLBACK_001
 \DESCRIPTION    Os_HookCallCallback is called with the StartupHookRef derived from the return value of
                 Os_ThreadGetCore. Os_HookCallCallback allows NULL_PTR arguments.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]

\ID SBSW_OS_CORE_HOOKCALLCALLBACK_002
 \DESCRIPTION    Os_HookCallCallback is called with the return value of Os_AppGetStartupHook.
 \COUNTERMEASURE \N [CM_OS_APPGETSTARTUPHOOK_N]

\ID SBSW_OS_CORE_APPGETSTARTUPHOOK_001
 \DESCRIPTION    Os_AppGetStartupHook is called with an entry of AppRefs. The used AppRefs index is limited to
                 AppCount. AppCount and AppRefs are derived from a core returned from Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]
                 \M [CM_OS_COREASR_APPREFS_M]

\ID SBSW_OS_CORE_APPSTART_001
 \DESCRIPTION    Os_AppStart is called with an entry of AppRefs. The used AppRefs index is limited to
                 AppCount. AppCount and AppRefs are derived from a core returned from Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]
                 \M [CM_OS_COREASR_APPREFS_M]

\ID SBSW_OS_CORE_APPSTART_002
 \DESCRIPTION    Os_AppStart is called with the KernelApp derived from the return value of Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]
                 \M [CM_OS_COREASR_KERNELAPP_M]

\ID SBSW_OS_CORE_THREADISSHUTDOWNHOOK_001
 \DESCRIPTION    Os_ThreadIsShutdownHook is called with the return value of Os_CoreGetThread.
 \COUNTERMEASURE \N [CM_OS_COREGETTHREAD_N]

\ID SBSW_OS_CORE_COREOSISSTARTED_001
 \DESCRIPTION    Os_CoreOsIsStarted is called with the return value of Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]

\ID SBSW_OS_CORE_APPKILLNOSUICIDE_001
 \DESCRIPTION    Os_AppKillNoSuicide is called with an entry of AppRefs. The used AppRefs index is limited to
                 AppCount. AppCount and AppRefs are derived from a core returned from Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]
                 \M [CM_OS_COREASR_APPREFS_M]

\ID SBSW_OS_CORE_APPGETSHUTDOWNHOOK_001
 \DESCRIPTION    Os_AppGetShutdownHook is called with an entry of AppRefs. The used AppRefs index is limited to
                 AppCount. AppCount and AppRefs are derived from a core returned from Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]
                 \M [CM_OS_COREASR_APPREFS_M]

\ID SBSW_OS_CORE_HOOKCALLSTATUSHOOK_001
 \DESCRIPTION    Os_HookCallStatusHook is called with the return value of Os_AppGetStartupHook.
 \COUNTERMEASURE \N [CM_OS_APPGETSHUTDOWNHOOK_N]

\ID SBSW_OS_CORE_HOOKCALLSTATUSHOOK_002
 \DESCRIPTION    Os_HookCallStatusHook is called with a ShutdownHook derived from the return value of
                 Os_ThreadGetCore. No measure for ShutdownHookRef. See CM_OS_COREGETSHUTDOWNHOOK_N
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]

\ID SBSW_OS_CORE_XSIGSEND_001
 \DESCRIPTION    A cross core send function is called, with the return values of Os_Core2AsrCore and
                 Os_CoreGetThread. Correctness of Os_CoreGetThread is ensured by preconditions.
 \COUNTERMEASURE \N [CM_OS_CORE2ASRCORE_N]. [CM_OS_COREGETTHREAD_N].

\ID SBSW_OS_CORE_BARRIERINIT_001
 \DESCRIPTION    Os_BarrierInit is called with the Barrier object derived from the return value of
                 Os_Core2AsrCore.
 \COUNTERMEASURE \M [CM_OS_CORE_BARRIER_M]
                 \R [CM_OS_CORE2ASRCORE_R]

\ID SBSW_OS_CORE_BARRIERINIT_002
 \DESCRIPTION    Os_BarrierInit is called with the Barrier references object derived from the return value of
                 Os_Core2AsrCore. Check against NULL_PTR exists.
 \COUNTERMEASURE \R [CM_OS_CORE_BARRIERREF_R]
                 \M [CM_OS_COREASR_BARRIERREFS_M]
                 \R [CM_OS_CORE2ASRCORE_R]

\ID SBSW_OS_CORE_COREISASRCORE_001
 \DESCRIPTION    Os_CoreIsAsrCore is called with the return value of Os_CoreId2Core.
 \COUNTERMEASURE \M [CM_OS_COREID2CORE_M]

\ID SBSW_OS_CORE_COREISASRCORE_002
 \DESCRIPTION    Os_CoreIsAsrCore is called with the return value of Os_CoreGetCurrentCore.
 \COUNTERMEASURE \S [CM_OS_COREGETCURRENTCORE_S]

\ID SBSW_OS_CORE_APPSETNOTACCESSIBLE_001
 \DESCRIPTION    Os_AppSetNotAccessible is called with the KernelApp derived from the return value of Os_Core2AsrCore.
                 Precondition of Os_Core2AsrCore ensures that the returned Core pointer is valid. The compliance of the
                 precondition is check during review. CM_OS_CORE2ASRCORE_N
 \COUNTERMEASURE \M [CM_OS_COREASR_KERNELAPP_M]
                 \R [CM_OS_CORE2ASRCORE_R]

\ID SBSW_OS_CORE_APPSETNOTACCESSIBLE_002
 \DESCRIPTION    Os_AppSetNotAccessible is called with an entry of AppRefs. The used AppRefs index is limited to
                 AppCount. AppCount and AppRefs are derived from the return value of Os_Core2AsrCore.
                 Precondition of Os_Core2AsrCore ensures that the returned Core pointer is valid. The compliance of the
                 precondition is check during review. CM_OS_CORE2ASRCORE_N
 \COUNTERMEASURE \M [CM_OS_COREASR_APPREFS_M]

\ID SBSW_OS_CORE_HAL_COREINITHARDWARE_001
 \DESCRIPTION    Os_Hal_CoreInitHardware is called with the HwConfig derived from the return value of
                 Os_SystemGet. Return value of Os_SystemGet is always correct.
 \COUNTERMEASURE \M [CM_OS_SYSTEM_HWCONFIG_M]

\ID SBSW_OS_CORE_COREREDUCEDINIT_001
 \DESCRIPTION    Os_CoreReducedInit is called with the return value of Os_Core2AsrCore.
 \COUNTERMEASURE \N [CM_OS_CORE2ASRCORE_N]

\ID SBSW_OS_CORE_CORECHECKSTARTEDASASR_001
 \DESCRIPTION    Os_CoreCheckStartedAsAsr is called with the return values of Os_CoreGetCurrentCore.
 \COUNTERMEASURE \S [CM_OS_COREGETCURRENTCORE_S]

\ID SBSW_OS_CORE_CORECHECKHASPRESTARTTASK_001
 \DESCRIPTION    Os_CoreCheckHasPreStartTask is called with the return values of Os_Core2AsrCore.
 \COUNTERMEASURE \N [CM_OS_CORE2ASRCORE_N]

\ID SBSW_OS_CORE_CORECHECKOSISNOTSTARTED_001
 \DESCRIPTION    Os_CoreCheckOsIsNotStarted is called with the return values of Os_Core2AsrCore.
 \COUNTERMEASURE \N [CM_OS_CORE2ASRCORE_N]

\ID SBSW_OS_CORE_CORECHECKOSISINITIALIZING_001
 \DESCRIPTION    Os_CoreCheckOsIsInitializing is called with the return values of Os_Core2AsrCore.
 \COUNTERMEASURE \N [CM_OS_CORE2ASRCORE_N]

\ID SBSW_OS_CORE_APPSERVICESINIT_001
 \DESCRIPTION    Os_AppServicesInit is called with the KernelApp derived from the return value of Os_Core2AsrCore.
                 Precondition of Os_Core2AsrCore ensures that the returned Core pointer is valid. The compliance of the
                 precondition is check during review. CM_OS_CORE2ASRCORE_N
 \COUNTERMEASURE \M [CM_OS_COREASR_KERNELAPP_M]

\ID SBSW_OS_CORE_APPSERVICESINIT_002
 \DESCRIPTION    Os_AppServicesInit is called with an entry of AppRefs. The used AppRefs index is limited to
                 AppCount. AppCount and AppRefs are derived from the return value of Os_Core2AsrCore.
                 Precondition of Os_Core2AsrCore ensures that the returned Core pointer is valid. The compliance of the
                 precondition is check during review. CM_OS_CORE2ASRCORE_N
 \COUNTERMEASURE \M [CM_OS_COREASR_APPREFS_M]

\ID SBSW_OS_CORE_TASKINIT_001
 \DESCRIPTION    Os_TaskInit is called with the PreStartTask derived from Os_Core2AsrCore().
                 Precondition ensures that the Core pointer is valid. The compliance of the precondition is check during
                 review. CM_OS_CORE2ASRCORE_N
 \COUNTERMEASURE \R [CM_OS_COREASR_PRESTARTTASK_R]

\ID SBSW_OS_CORE_TASKPRESTARTTASKENTER_001
 \DESCRIPTION    Os_TaskPreStartTaskEnter is called with the PreStartTask Os_Core2AsrCore().
                 Precondition ensures that the Core pointer is valid. The compliance of the precondition is check during
                 review. CM_OS_CORE2ASRCORE_N
 \COUNTERMEASURE \R [CM_OS_COREASR_PRESTARTTASK_R]

\ID SBSW_OS_CORE_CORECHECKISASRCORE_001
 \DESCRIPTION    Os_CoreCheckIsAsrCore is called with the return value of Os_CoreId2Core.
 \COUNTERMEASURE \M [CM_OS_COREID2CORE_M]

\ID SBSW_OS_CORE_CORECHECKISASRCORE_002
 \DESCRIPTION    Os_CoreCheckIsAsrCore is called with the return values of Os_ThreadGetCore and Os_Core2AsrCore.
                 Precondition ensures that the Core pointer is valid. The compliance of the precondition is check during
                 review.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]

\ID SBSW_OS_CORE_CORECHECKISASRCORE_003
 \DESCRIPTION    Os_CoreCheckIsAsrCore is called with the return value of Os_CoreGetCurrentCore.
 \COUNTERMEASURE \S [CM_OS_COREGETCURRENTCORE_S]

\ID SBSW_OS_CORE_COREGETID_001
 \DESCRIPTION    Os_CoreGetId is called with the return value of Os_CoreGetCurrentCore.
 \COUNTERMEASURE \S [CM_OS_COREGETCURRENTCORE_S]

\ID SBSW_OS_CORE_COREGETSTARTSTATE_001
 \DESCRIPTION    Os_CoreGetStartState is called with the return value of Os_CoreId2Core.
 \COUNTERMEASURE \M [CM_OS_COREID2CORE_M]

\ID SBSW_OS_CORE_CORECHECKCOREISINACTIVE_001
 \DESCRIPTION    Os_CoreCheckCoreIsInactive is called with the return value of Os_CoreId2Core.
 \COUNTERMEASURE \M [CM_OS_COREID2CORE_M]

\ID SBSW_OS_CORE_THREADGETCURRENTAPPLICATION_001
 \DESCRIPTION    Os_ThreadGetCurrentApplication is called with the return value of Os_CoreGetThread.
 \COUNTERMEASURE \N [CM_OS_COREGETTHREAD_N]

\ID SBSW_OS_CORE_THREADCHECKCALLCONTEXT_001
 \DESCRIPTION    Os_ThreadCheckCallContext is called with the return value of Os_CoreGetThread.
 \COUNTERMEASURE \N [CM_OS_COREGETTHREAD_N]

\ID SBSW_OS_CORE_THREADCHECKAREINTERRUPTSENABLED_001
 \DESCRIPTION    Os_ThreadCheckAreInterruptsEnabled is called with the return value of Os_CoreGetThread.
 \COUNTERMEASURE \N [CM_OS_COREGETTHREAD_N]

\ID SBSW_OS_CORE_COREASRISCORELOCAL_001
 \DESCRIPTION    Os_CoreAsrIsCoreLocal is called with the return value of Os_Core2AsrCore.
                 Precondition ensures that the Core pointer is valid. The compliance of the precondition is check during
                 review.
 \COUNTERMEASURE \N [CM_OS_CORE2ASRCORE_N]

\ID SBSW_OS_CORE_COREASR_INTERRUPTEDTHREADS_001
 \DESCRIPTION    Write access to an element of InterruptedThreads derived from the return value of Os_CoreAsrGetDyn.
 \COUNTERMEASURE \R [CM_OS_COREASR_INTERRUPTEDTHREADS_R]
                 \M [CM_OS_COREASRGETDYN_M]

\ID SBSW_OS_CORE_SYSTEMGETCORE2THREAD_001
 \DESCRIPTION    Os_SystemGetCore2Thread is called with the return value of Os_SystemGet.
 \COUNTERMEASURE \N [CM_OS_SYSTEMGET_N]

\ID SBSW_OS_CORE_HAL_CORESETTHREAD_001
 \DESCRIPTION    Os_Hal_CoreSetThread is called with the return value of Os_SystemGetCore2Thread and a caller
                 argument. The correctness of the caller argument is ensured by precondition. The compliance of the
                 precondition is check during review.
 \COUNTERMEASURE \M [CM_OS_SYSTEM_CORE2THREAD_M]

\ID SBSW_OS_CORE_HAL_COREGETTHREAD_001
 \DESCRIPTION    Os_Hal_CoreGetThread is called with the return value of Os_SystemGetCore2Thread.
 \COUNTERMEASURE \M [CM_OS_SYSTEM_CORE2THREAD_M]

\ID SBSW_OS_CORE_APPISTRUSTED_001
 \DESCRIPTION    Os_AppIsTrusted is called with the return value of Os_ThreadGetCurrentApplication.
 \COUNTERMEASURE \N [CM_OS_THREADGETCURRENTAPPLICATION_N]

\ID SBSW_OS_CORE_CORECONTROLIDLE_001
 \DESCRIPTION    Os_CoreControlIdle is called with the return value  of Os_Core2AsrCore.
 \COUNTERMEASURE \N [CM_OS_CORE2ASRCORE_N]

\ID SBSW_OS_CORE_SYSTEMGETAPPLICATIONMODE_001
 \DESCRIPTION    Os_SystemGetApplicationMode is called with the return value  of Os_SystemGet.
 \COUNTERMEASURE \N [CM_OS_SYSTEMGET_N]

\ID SBSW_OS_CORE_COREDETERMINESTARTSTATUS_001
 \DESCRIPTION    Os_CoreDetermineStartStatus is called with the return value of Os_CoreGetCurrentCore.
 \COUNTERMEASURE \S [CM_OS_COREGETCURRENTCORE_S]

\ID SBSW_OS_CORE_CORESTARTREQUESTS_001
 \DESCRIPTION    Write access with an index to the CoreStartRequests member of a status object returned from
                 Os_CoreGetStatus.
 \COUNTERMEASURE \M [CM_OS_COREGETSTATUS_M]
                 \R [CM_OS_COREID_R]

\ID SBSW_OS_CORE_COREASRGETASRSTATUS_001
 \DESCRIPTION    Os_CoreAsrGetAsrStatus is called with the return value of Os_Core2AsrCore.
 \COUNTERMEASURE \R [CM_OS_CORE2ASRCORE_R]

\ID SBSW_OS_CORE_COREASRGETAPPLICATIONMODE_001
 \DESCRIPTION    Os_CoreAsrGetApplicationMode is called with the return value of Os_Core2AsrCore.
 \COUNTERMEASURE \R [CM_OS_CORE2ASRCORE_R]

\ID SBSW_OS_CORE_COREASRGETAPPLICATIONMODE_002
 \DESCRIPTION    Os_CoreAsrGetApplicationMode is called with the return value of Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]

\ID SBSW_OS_CORE_COREASRCOMBINEAPPLICATIONMODE_001
 \DESCRIPTION    Os_CoreAsrCombineApplicationMode is called with the return value of Os_Core2AsrCore.
 \COUNTERMEASURE \R [CM_OS_CORE2ASRCORE_R]

\ID SBSW_OS_CORE_COREHOOKSINIT_001
 \DESCRIPTION    Os_CoreHooksInit is called with the return value of Os_Core2AsrCore.
 \COUNTERMEASURE \R [CM_OS_CORE2ASRCORE_R]

\ID SBSW_OS_CORE_HOOKINIT_001
 \DESCRIPTION    Os_HookInit
 \COUNTERMEASURE \M [CM_OS_COREASR_HOOKREFS_M]

\ID SBSW_OS_CORE_COREBOOTBARRIERREFS_001
 \DESCRIPTION    The written pointer is coming from the array OsCfg_CoreBootBarrierRefs.
 \COUNTERMEASURE \M [CM_OS_CORE_COREBOOTBARRIERREFS_M]

\ID SBSW_OS_CORE_HOOKCALLOS_COREINITHOOK_001
 \DESCRIPTION    Os_HookCallOs_CoreInitHook is called with the return value of Os_CoreGetInitHook.
 \COUNTERMEASURE \N [CM_OS_COREGETINITHOOK_PRE_N]

\ID SBSW_OS_CORE_SETEXCEPTIONCONTEXT_001
 \DESCRIPTION    The pointer is from the array OsCfg_Hal_Context_ExceptionContextRef, which shall be checked by user.
 \COUNTERMEASURE \S [CM_OS_COREEXCEPTIONCONTEXT_S]

\ID SBSW_OS_CORE_THREADCLASSINIT_001
 \DESCRIPTION    Os_ThreadClassInit is called with an object derived from the return value of
                 Os_Core2AsrCore.
 \COUNTERMEASURE \M [CM_OS_COREASR_THREADCLASS_M]

\ID SBSW_OS_CORE_PROTECTIONEXCEPTION_001
 \DESCRIPTION    Os_ErrProtectionException is called with a pointer parameter passed from the caller function.
 \COUNTERMEASURE \N The parameter is declared as pointer to const to prevent invalid write access.

\ID SBSW_OS_CORE_SETCURRENTSTACK_001
 \DESCRIPTION    Os_CoreSetCurrentStack is called with the return value of Os_TaskGetThread.
 \COUNTERMEASURE \N [CM_OS_TASKGETTHREAD_N]

\ID SBSW_OS_CORE_GETKERNELSTACK_001
 \DESCRIPTION    Os_CoreGetKernelStack is called with the return value of Os_Core2AsrCore.
 \COUNTERMEASURE \N [CM_OS_CORE2ASRCORE_N]

\ID SBSW_OS_CORE_SETCURRENTSTACK_002
 \DESCRIPTION    Os_CoreSetCurrentStack is called with the return value of Os_CoreGetKernelStack
                 and the return value of Os_Core2AsrCore.
 \COUNTERMEASURE \M [CM_OS_COREASR_KERNELSTACK_M]
                    [CM_OS_CORE2ASRCORE_N]

\ID SBSW_OS_FC_CORE_ISRINITIALENABLE_001
 \DESCRIPTION    Os_IsrInitialEnable is called with an ISR reference, taken from array.
 \COUNTERMEASURE \M [CM_OS_CORE_ISRREFS_M]

\ID SBSW_OS_CORE_PRESTARTTASK_CALLBACK_001
 \DESCRIPTION    The Pre-Start Task callback of a core object is used as function pointer. The function is valid,
                 if a Pre-Start Task is configured for the core.
 \COUNTERMEASURE \M [CM_OS_COREASR_PRESTARTTASK_CALLBACK_M]

SBSW_JUSTIFICATION_END */

/* FETA_JUSTIFICATION_BEGIN

\ID FETA_OS_CONSTANTASFUNCTIONPARAMETER
 \DESCRIPTION    The loop condition depends on a constant value which is passed as a function parameter.
 \COUNTERMEASURE \N None as the parameter is a constant.

\ID FETA_OS_CONSTDATA
 \DESCRIPTION    The termination condition of the loop depends only on a generated constant data.
 \COUNTERMEASURE \N None as the data is constant.

\ID FETA_OS_MAXINTERRUPTEDTHREADS
 \DESCRIPTION    The loop iterates over all interrupted threads. The maximum number of interrupted threads is limited
                 by the OS_MAX_INTERRUPTEDTHREADS macro.
 \COUNTERMEASURE \R The boundary is checked in Os_CoreInterruptedThreadsPush.

\ID FETA_OS_DEQUEDELETE
 \DESCRIPTION    The loop iterates until the local read index matches the deque write index. Both indices are limited
                 by the Size of the deque.
 \COUNTERMEASURE \D As stated in the CDD a precondition of using this function is to prevent concurrent access
                 to the given object. The maximum number of iterations are then fixed.

\ID FETA_OS_EXTERNALTERMINATIONSCHT
 \DESCRIPTION    The loop termination depends on function results of the called functions. If a final expiry point
                 is processed (i.e., CYCLIC or SINGLE_SHOT) and there is a next schedule table then the loop will
                 iterate a second time and process the first expiry point of the next schedule table. Otherwise the
                 loop only iterates once.
 \COUNTERMEASURE \D Schedule tables and expiry points are described in DSGN-OsScheduleTable and the maximum number
                 of iterations is fixed.

\ID FETA_OS_XSIGCHANNELINDEX
 \DESCRIPTION    The loop, in the worst case, iterates until the next send element is equal to the result index. Both
                 indices are limited by the generated BufferSize.
 \COUNTERMEASURE \N None as the maximum number of iterations is fixed.

\ID FETA_OS_XSIGHANDLERESULTS
 \DESCRIPTION    The loop iterates while there are results in the buffer. The size of the buffer is limited by the
                 BufferSize of the SendChannel. The function is called before a new send in Os_XSigSendInternal(),
                 thus the buffer can not be filled with additional messages.
 \COUNTERMEASURE \N None as the maximum number of iterations is fixed.

\ID FETA_OS_SERVICECALLLIST
 \DESCRIPTION    The loop iterates over the linked list of service function until OS_SERVICECALL_LIST_END. The linked
                 list is empty at startup. The list grows by one for each call of a service function and shrinks
                 by one on return of that function.
 \COUNTERMEASURE \N None as the maximum number of iterations is fixed.

\ID FETA_OS_ITERATEOVERSTACK
 \DESCRIPTION    The loop iterates over a preconfigured stack.
 \COUNTERMEASURE \N None as the maximum number of iterations is fixed.

\ID FETA_OS_LOCKLIST
 \DESCRIPTION    The list iterates over the linked list of locks. The maximum number of iteration is limited by
                 the sum of OS_RESOURCEID_COUNT and OS_SPINLOCKID_COUNT.
 \COUNTERMEASURE \N None as the maximum number of iterations is fixed.

\ID FETA_OS_BARRIERSYNCHRONIZE
 \DESCRIPTION    The loop iterates until all attendees of the given barrier are synchronized.
 \COUNTERMEASURE \S The user of MICROSAR Safe shall ensure, that the number of calls to the API
                 Os_BarrierSynchronize is identical for all participating tasks of the referenced barrier. (SMI-309405)

\ID FETA_OS_COREINIT
 \DESCRIPTION    The loop iterates until the variable is set to a specific value by a corresponding foreign core
                 during startup. The boot barriers are used to synchronize the cores before the OS is started.
                 In case one of the attending cores has a malfunction, the loop may iterate endlessly.
 \COUNTERMEASURE \N None as freedom from interference with respect to timing is not violated since the OS has not been
                 started and no other components are running.

\ID FETA_OS_XSIGNALISR
 \DESCRIPTION    The loop iterates until there are no messages available anymore. New messages can also be
                 sent (by other cores) during the processing of previous messages.
 \COUNTERMEASURE \S The user of MICROSAR Safe shall ensure that frequent cross core API calls from cores
                 with lower diagnostic coverage to cores with higher diagnostic coverage do not interfere
                 with the application design. (SMI-483)

\ID FETA_OS_XSIGNALSYNC
 \DESCRIPTION    The loop idles until the submitted request is handled by the corresponding foreign core.
                 If the called core does not finish the caller will wait forever.
 \COUNTERMEASURE \S The user of MICROSAR Safe shall ensure that synchronous cross core API calls from a
                 core with higher diagnostic coverage to a core with lower diagnostic coverage do not
                 violate the safety goals in case the API calls never return. (SMI-484)

\ID FETA_OS_SPINLOCK
 \DESCRIPTION    The loop performs a busy wait until the spinlock is successfully acquired.
 \COUNTERMEASURE \N None as the busy wait is the intended behaviour.

\ID FETA_OS_COREIDLE
 \DESCRIPTION    The loop is the OS core idle loop. It is used to implement the Os_IdleTask.
 \COUNTERMEASURE \N None as the loop is endless by requirement.

\ID FETA_OS_QUEUEUSAGE
 \DESCRIPTION    The loop iterates over the queue that has Usage elements. The maximum number of elements of the
                 queue is limited by the Size.
 \COUNTERMEASURE \D As stated in the CDD a precondition of using this function is to prevent concurrent access
                 to the given object. The maximum number of iterations are then fixed.

\ID FETA_OS_LISTUSAGE
 \DESCRIPTION    The loop iterates over the list that has Usage elements. The maximum number of elements of the
                 list is limited by the Size.
 \COUNTERMEASURE \D As stated in the CDD a precondition of using this function is to prevent concurrent access
                 to the given object. The maximum number of iterations are then fixed.

\ID FETA_OS_SETNEXTCOMPAREVALUE
 \DESCRIPTION    The loop iterates until a compare value, that is in the future, can be set for the hardware timer.
                 This can take some iterations since the loop can be interrupted and set operation may require time.
 \COUNTERMEASURE \R A runtime check is implemented that limits the number of iterations to
                    OS_TIMER_SET_FRT_COMPARE_VALUE_TIMEOUT-1.

\ID FETA_OS_SETHALCOMPAREVALUE
 \DESCRIPTION    The loop iterates until the written compare value is read back correctly from the hardware.
                 This can take some iterations since it can take some time until the value arrives in the periphery.
 \COUNTERMEASURE \R A runtime check is implemented that limits the number of iterations to
                    OS_TIMER_SET_FRT_COMPARE_VALUE_TIMEOUT-1.

\ID FETA_OS_BARRIERATTACH
 \DESCRIPTION    The loop iterates until no other core attached to the barrier during the attach step.
                 The maximum number of iterations is limited by the number of configured AUTOSAR cores.
                 Hint: The counter value of a barrier is changed in the function Os_BarrierSynchronizeInternal() at each
                 synchronization point.
 \COUNTERMEASURE \N None as the maximum number of iterations is fixed.

\ID FETA_OS_COUNTERWORKJOBSQUEUE
 \DESCRIPTION    The loop iterates over all jobs in the counter's queue. The queue size is limited by the maximum number
                 of jobs configured for the counter. Only jobs with expired expiration times are removed from the
                 queue. New jobs can be added by cross core events or by higher priority category 2 interrupts.
                 Only if the expiration times of the jobs are sufficiently small and the jobs are requested cyclically
                 within this small period, the loop may iterate endlessly.
 \COUNTERMEASURE \R Interrupt handling is opened after MAX_JOB_EXECS_PER_LOCK jobs such that higher priority interrupts
                 can be executed and an overload situation can be handled.

\ID FETA_OS_COUNTERWORKJOBSLIST
 \DESCRIPTION    The loop iterates over all jobs in the counter's list. The list size is limited by the maximum number
                 of jobs configured for the counter. Only jobs with expired expiration times are removed from the
                 list. New jobs can be added by cross core events or by higher priority category 2 interrupts.
                 As High Resolution Timer is excluded for an array list, physical value of the counter does not change
                 during a counter cycle. Thus newly added jobs cannot expire.
 \COUNTERMEASURE \N None as the maximum number of iterations is fixed.

\ID FETA_OS_EVENTGETLOCAL
 \DESCRIPTION    The loop iterates as long as the event mask changed during loop execution. The event mask may be
                 changed during an interrupt or by a cross core service request. Events for foreign tasks can only be
                 set and only the owner task of an event can clear an event. There is no situation where an event gets
                 set and cleared periodically so that the loop iterates endlessly. The number of events for a task and
                 thus the number of changes of the event mask is limited (EventMaskType).
 \COUNTERMEASURE \N None as the maximum number of iterations is fixed.

FETA_JUSTIFICATION_END */

/*
\CM CM_OS_ABSTRACT_DYN_N
      As each object of an abstract class is always part of a concrete class, the concrete class
      guarantees, that the Dyn pointer is valid.

\CM CM_OS_COREGETSCHEDULER_M
      Verify that the Scheduler in each core object is a non NULL_PTR.

\CM CM_OS_PRECONDITION_N
      Preconditions ensures, that the used pointer(s) is/are correct. The compliance of the precondition is check
      during review.

\CM CM_OS_PRECONDITION_AND_CHECK_N
      The caller has to ensure, that the given pointer is correct. NULL_PTR is checked before usage.

\CM CM_OS_PRECONDITION_AND_CHECK_IF_NEEDED_N
      The caller has to ensure, that the given pointer is correct. NULL_PTR is checked before usage where the
      global configuration allows NULL_PTR.

\CM CM_OS_COREGETTHREAD_N
      Precondition ensures that the pointer is valid. The compliance of the precondition is check during review.

\CM CM_COREINTERRUPTEDTHREADSGET_R
      Runtime check ensures that the pointer is valid.

\CM CM_OS_SYSTEMGET_N
      The returned value of Os_SystemGet is derived from a static value and therefor valid always.

\CM CM_OS_COREASR_HWCONFIG_M
      Verify that each autosar core's HwConfig pointer in OsCfg_CoreRefs is no NULL_PTR (this is relevant
      for asr core of type Os_CoreAsrConfigType, member type Os_Hal_CoreAseConfigType*).

\CM CM_OS_COREGETSTATUS_M
      Verify that each Status pointer of each core object is a non NULL_PTR.

\CM CM_OS_COREASRGETSTATUS_M
      Verify that each Status pointer of each core which is an AUTOSAR core, is derived from an AUTOSAR core status
      object.

\CM CM_OS_COREASRGETDYN_M
      Verify that each Autosar core Dyn pointer is a non NULL_PTR.

\CM CM_OS_COREASR_IDLETASK_M
      Verify that each Autosar core's IdleTask pointer is a non NULL_PTR.

\CM CM_OS_COREGETLASTERROR_M
      Verify that each Autosar core's Dyn pointer is derived from an Autosar dyn object.

\CM CM_OS_CORE_HWCONFIG_M
      Verify that each core's HwConfig pointer is a non NULL_PTR (this is relevant for core type Os_CoreConfigType
      incl. derived types, member type Os_Hal_CoreConfigType*).

\CM CM_OS_SYSTEM_HWCONFIG_M
      Verify that the system's HwConfig pointer is a non NULL_PTR.

\CM CM_OS_COREINTERRUPTEDTHREADSPOP_T
      TCASE345812 ensures that pointers passed via Os_CoreInterruptedThreadsPush are returned in LIFO order by
      Os_CoreInterruptedThreadsPop.

\CM CM_OS_COREASR_BARRIERREFS_M
      Verify that:
        1. each Autosar core's BarrierRefs pointer is no NULL_PTR,
        2. each Autosar core's BarrierRefs size is equal to BarrierCount and
        3. the last element in BarrierRefs must be NULL_PTR.

\CM CM_OS_COREASR_STACKREFS_M
      Verify that:
        1. each Autosar core's StackRefs pointer is no NULL_PTR,
        2. each Autosar core's StackRefs size is equal to the core's StackCount and
        3. each element in StackRefs except the last one is no NULL_PTR, the last one must be NULL_PTR.

\CM CM_OS_COREASR_INTERRUPTSOURCEREFS_M
      Verify that:
        1. each Autosar core's InterruptSourceRefs pointer is no NULL_PTR,
        2. each Autosar core's InterruptSourceRefs size is equal to the core's InterruptSourceCount and
        3. each element in InterruptSourceRefs except the last one is no NULL_PTR, the last one must be
           NULL_PTR.

\CM CM_OS_SYSTEM_INTERRUPTSOURCEREFS_M
      Verify that:
        1. Verify that the system InterruptSourceRefs pointer is not NULL_PTR,
        2. verify that the system InterruptSourceRefs size is equal to the systems InterruptSourceCount and
        3. verify that each element in InterruptSourceRefs except the last one is no NULL_PTR, the last one must
           be NULL_PTR.

\CM CM_OS_CORE2ASRCORE_N
      Precondition ensures that the pointer is valid. The compliance of the precondition is checked during review.

\CM CM_OS_CORE2ASRCORE_R
      Correctness of return value is ensured by runtime check.

\CM CM_OS_COREASR2CORE_N
      Precondition ensures that the pointer is valid. The compliance of the precondition is check during review.

\CM CM_OS_SYSTEM_MEMORYPROTECTION_M
      Verify that the systems memory protection pointer is valid, if memory protection is enabled.

\CM CM_OS_COREASR_MEMORYPROTECTION_M
      Verify that the core memory protection pointer is valid, if memory protection is enabled.

\CM CM_OS_COREGETSTARTUPHOOK_N
      The called function is able to handle NULL_PTR.

\CM CM_OS_COREGETSTARTUPHOOK_PRE_N
      Precondition ensures, that the returned value is a non NULL_PTR.

\CM CM_OS_COREGETERRORHOOK_N
      The called function is able to handle NULL_PTR.

\CM CM_OS_COREGETSHUTDOWNHOOK_N
      The called function is able to handle NULL_PTR.

\CM CM_OS_COREGETERRORHOOK_PRE_N
      Precondition ensures, that the returned value is a non NULL_PTR.

\CM CM_OS_COREGETSHUTDOWNHOOK_PRE_N
      Precondition ensures, that the returned value is a non NULL_PTR.

\CM CM_OS_COREGETPROTECTIONHOOK_N
      The called function is able to handle NULL_PTR.

\CM CM_OS_COREGETPROTECTIONHOOK_PRE_N
      Precondition ensures, that the returned value is a non NULL_PTR.

\CM CM_OS_COREGETINITHOOK_PRE_N
      Precondition ensures, that the returned value is a non NULL_PTR.

\CM CM_OS_COREASRGETTIMINGPROTECTION1_M
      Verify that the core timing protection pointer is a non NULL_PTR, if timing protection is enabled for all cores
      and is a NULL_PTR if no timing protection is enabled at all. As a result, the core timing protection pointer
      must be a non NULL_PTR if OS_CFG_TIMING_PROTECTION is defined to OS_CFG_TP_ALL_CORES and
      must be a NULL_PTR if OS_CFG_TIMING_PROTECTION is defined to OS_CFG_TP_NO_CORE.

\CM CM_OS_COREGETTRACE_M
      Verify that the Trace pointer of each Core is valid, when ORTI tracing is enabled.

\CM CM_OS_COREASR_RESOURCEREFS_M
      Verify that:
        1. each Autosar core's ResourceRefs pointer is no NULL_PTR,
        2. each Autosar core's ResourceRefs size is equal to the core's ResourceCount and
        3. each element in ResourceRefs except the last one is no NULL_PTR, the last one must be NULL_PTR.

\CM CM_OS_COREASR_IOCREFS_M
      Verify that:
        1. each Autosar core's IocRefs pointer is no NULL_PTR,
        2. each Autosar core's IocRefs size is equal to the core's IocCount and
        3. each element in IocRefs except the last one is no NULL_PTR, the last one must be NULL_PTR.

\CM CM_OS_COREASR_APPREFS_M
      Verify that:
        1. each Autosar core's AppRefs pointer is no NULL_PTR,
        2. each Autosar core's AppRefs size is equal to the core's AppCount and
        3. each element in AppRefs except the last one is no NULL_PTR, the last one must be NULL_PTR.

\CM CM_OS_COREASR_HOOKREFS_M
      Verify that:
        1. each Autosar core's HookRefs pointer is no NULL_PTR,
        2. each Autosar core's HookRefs size is equal to the core's HookCount and
        3. each element in HookRefs except the last one is no NULL_PTR, the last one must be NULL_PTR.

\CM CM_OS_COREASR_INTERRUPTEDTHREADS_R
      InterruptedThreads has the size OS_MAX_INTERRUPTEDTHREADS before any write to the array when retrieving a pointer,
      the used index is check against OS_MAX_INTERRUPTEDTHREADS.

\CM CM_OS_COREASR_XSIGNAL_M
      Verify that the core XSignal pointer of each autosar core is a non NULL_PTR if XSIGNAL is enabled.

\CM CM_OS_CORE_BARRIER_M
      Verify that each core's Barrier pointer is a non NULL_PTR.

\CM CM_OS_SYSTEM_SPINLOCKREFS_M
      Verify that:
        1. each system's SpinlockRefs pointer is no NULL_PTR,
        2. each system's SpinlockRefs size is equal to the systems's SpinlockCount and
        3. each element in SpinlockRefs except the last one is no NULL_PTR, the last one must be NULL_PTR.

\CM CM_OS_COREID2CORE_M
      Verify that:
        1. each Core in OsCfg_CoreRefs except the last is a non NULL_PTR,
        2. OsCfg_CoreRefs has a size equal to OS_COREID_COUNT + 1 and
        3. each element in CoreIdType has a value lower than OS_COREID_COUNT.

\CM CM_OS_CORE_COREBOOTBARRIERREFS_M
      Verify that:
        1. each entry in OsCfg_CoreBootBarrierRefs except the last is a non NULL_PTR and
        2. OsCfg_CoreBootBarrierRefs has a size equal to OS_COREID_COUNT + 1.

\CM CM_OS_COREASR_PRESTARTTASK_R
      The caller ensures, that the pointer is no NULL_PTR.

\CM CM_OS_SYSTEM_CORE2THREAD_M
      Verify that the system's Core2Thread pointer is a non NULL_PTR.

\CM CM_OS_COREASR_KERNELSTACK_M
      Verify that the core's KernelStack pointer is a non NULL_PTR.

\CM CM_OS_CORETHREADGETBYCONTEXT_R
      The caller has to ensure, that the given pointer is correct. NULL_PTR is checked before usage.

\CM CM_OS_COREASR_HOOKINFO_N
      Precondition ensures, that the returned value is a non NULL_PTR.

\CM CM_OS_COREASR_THREADCLASS_M
      Verify that the core's ThreadClass pointer is a non NULL_PTR.

\CM CM_OS_COREASR_KERNELAPP_M
      Verify that the KernelApp reference of each autosar core is a non NULL_PTR.

\CM CM_OS_COREID_R
      Core ID is used as index. Correctness of ID is ensured by implementation.

\CM CM_OS_COREGETCURRENTCORE_S
      Verify that:
        The array OsCfg_CorePhysicalRefs contains all physical cores.
        For each existing physical hardware core identifier there shall be one corresponding entry inside the array
        which is indexed by the physical hardware core identifier provided directly by the hardware registers.
        SMI-2900

\CM CM_OS_COREEXCEPTIONCONTEXT_S
      Verify that:
        The array OsCfg_Hal_Context_ExceptionContextRef contains all physical cores.
        For each existing physical hardware core identifier, which is also an Autosar core, there shall be one
        corresponding entry (not NULL_PTR) inside the array which is indexed by the physical hardware core identifier
        provided directly by the hardware registers.
        SMI-39288

\CM CM_OS_CORE_BARRIERREF_R
      Runtime check ensures that the pointer is valid.

\CM CM_OS_CORE_ISRREFS_M
      Verify that:
        1. each AsrCores IsrRefs pointer points to an array of cat2 ISRs.
        2. each AsrCores IsrRefs size is equal to the AsrCores IsrCount+1 and
        3. each element in IsrRefs except the last one is no NULL_PTR, the last one must be NULL_PTR.

\CM CM_OS_COREASR_PRESTARTTASK_CALLBACK_M
      Verify that the Pre-Start Task Callback reference of an autosar core is a non NULL_PTR, when a Pre-Start Task
      is configured for that autosar core.
 */

/* START_COVERAGE_JUSTIFICATION
 *
\ID COV_OS_INVSTATE
   \ACCEPT XX
   \ACCEPT XF
   \ACCEPT TX
   \REASON [COV_MSR_INV_STATE]

\ID COV_OS_NOFUNCCALL
   \ACCEPT XF
   \ACCEPT XX
   \ACCEPT TX
   \REASON As only function call coverage is required and there is no function call in the uncovered code block,
           it is acceptable to leave this code block uncovered.

\ID COV_OS_NOFUNCCALL_XF
   \ACCEPT XF
   \REASON As only function call coverage is required and there is no function call in the uncovered code block,
           it is acceptable to leave this code block uncovered.

\ID COV_OS_NOFUNCCALL_TX
   \ACCEPT TX
   \REASON As only function call coverage is required and there is no function call in the uncovered code block,
           it is acceptable to leave this code block uncovered.

\ID COV_OS_NOFUNCCALL_XX
   \ACCEPT XX
   \REASON As only function call coverage is required and there is no function call in the uncovered code block,
           it is acceptable to leave this code block uncovered.

\ID COV_OS_COREIDLELOOP
   \ACCEPT TX
   \REASON Intentional endless loop. The idle loop is left only by means of interruption. Therefore, the loop condition
           will never become false.

\ID COV_OS_CORESTARTUPSTATUS
   \ACCEPT TX
   \ACCEPT XF
   \REASON [COV_MSR_INV_STATE]

\ID COV_OS_COREBOOTBARRIER
   \ACCEPT XF
   \ACCEPT TF
   \REASON In certain circumstances it might possible that the repetition of the do-while loop is not covered. This
           might be either an initialization problem (the echo variable has already the expected value - due to this
           it is written twice with different values) or a timing problem (the other core has already written the echo
           variable before it is checked). Furthermore it is implemented as a do-while loop so the code is always
           excecuted at least once. The risk of keeping the code is low as it is unit tested and reviewed.

\ID COV_OS_TESTSUITERESTRICTIONCAT2ISRS_XF
   \ACCEPT XF
   \REASON Currently the testsuite does not support configurations with no CAT2 ISRs. Therefore the check that no
           CAT2 ISRs are present will never become true. Unit test and code inspection have not found any risk in
           keeping this code.

\ID COV_OS_TESTSUITEOSC4998
   \ACCEPT XX
   \ACCEPT XF
   \ACCEPT TX
   \REASON Currently the User-Barrier-Feature is only tested on multi core derivatives. The feature can be configured on
           single core systems as well.
           Even such a configuration does not make much sense, the behavior should be tested. (OSC-4998)
           Unit test and code inspection have not found any risk in using the code on single core systems.

\ID COV_OS_TESTSUITERESTRICTIONCORECOUNT
   \ACCEPT TX
   \ACCEPT XF
   \REASON Currently, the testsuite expects to run with a core count identical to the number of physically available
           cores on the derivative. This is a result of the used linker scripts for each derivative.
           For multicore derivatives, it is currently not possible to reach code paths for single core derivatives
           and vice versa. However, each derivative can only reach one path or the other here. Unit test and code
           inspection have not found any risk in this code.

\ID COV_OS_OSC7103_XF
   \ACCEPT XF
   \REASON Missing test case (see OSC-7103). Same sequence is used in function Os_ThreadCallContextOnStack.
           Unit test and code inspection have not found any risk in keeping this code.

\ID COV_OS_TIMEOUT_XF
   \ACCEPT XF
   \REASON The code is used to introduce a termination criteria for a potentially endless loop. It is not possible to
           reach the true case in a component test. The sequence in the condition has been tested on several other
           places in the OS code.
           Unit test and code inspection have not found any risk in using the code on single core systems.

\ID COV_OS_NOTTESTABLE_XX
   \ACCEPT XX
   \REASON It is not possible to reach the functionality in a component test.
           The sequence has been tested on several other places in the OS code.
           Unit test and code inspection have not found any risk in keeping this code.


Justifications for Variant Coverage:

\ID COV_OS_DERIVATIVEGROUP
   \ACCEPT TX
   \ACCEPT XF
   \ACCEPT XX
   \REASON The condition is used to identify derivative specific module configurations.
           All uncovered derivatives contain only modules which are tested by a derivative under regression.
           The compliance with a specific derivative under test is assured by the Equivalence Hardware Architecture Check.

\ID COV_OS_DERIVATIVEDUMMYMODULE
   \ACCEPT TX
   \REASON The uncovered code only contains empty dummy implementations to avoid compile errors for specific derivative
           groups.

\ID COV_OS_DERIVATIVEBETAMODULE
   \ACCEPT TX
   \ACCEPT XF
   \ACCEPT XX
   \REASON The uncovered implementation only contains code which is in BETA state. The BETA state is documented in an
           according Issue-Report on the underlying subpackage.

\ID COV_OS_CDKMISRA
   \ACCEPT XF
   \REASON The condition is used to provide compiler and hardware specific information to the MISRA checker.
           The risk in not covering this code is low, as the code is never executed and only used for static code analysis.

\ID COV_OS_STATICCODEANALYSIS
   \ACCEPT TX
   \ACCEPT XF
   \ACCEPT XX
   \REASON The condition is used to provide compiler and hardware specific information to static code analysis tools.
           The risk in not covering this code is low, as the code is never executed and only used for static code analysis.

\ID COV_OS_COMPILERKEYWORD
   \ACCEPT TX
   \REASON [COV_MSR_COMPATIBILITY]

\ID COV_OS_TRACEOSTRACE
   \ACCEPT TX
   \REASON [COV_MSR_COMPATIBILITY]

\ID COV_OS_MEMMAPERROR
   \ACCEPT TX
   \ACCEPT XF
   \REASON [COV_MSR_CONFIG]

END_COVERAGE_JUSTIFICATION */


/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Core.c
 **********************************************************************************************************************/
