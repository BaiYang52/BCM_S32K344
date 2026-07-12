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
 *  \addtogroup Os_Hal_Context
 *  \trace      CREQ-1201
 *
 *  \{
 *
 *  \file       Os_Hal_Context.h
 *  \brief      Context switching primitives.
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388  EOF */ /* MD_MSR_Dir1.1 */
#ifndef OS_HAL_CONTEXT_H
# define OS_HAL_CONTEXT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module dependencies */
# include "Os_Hal_ContextInt.h"
# include "Os_Hal_Context_Lcfg.h"

/* Os kernel module dependencies */

/* Os Hal dependencies */
# include "Os_Hal_Compiler.h"
# include "Os_Hal_Interrupt.h"
# include "Os_Hal_Core.h"
# include "Os_Hal_EntryInt.h"


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


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
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  Os_Hal_ContextCANoeInit()
 **********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextCANoeInit,
(
  P2CONST(Os_Hal_CANoeTaskConfigType, AUTOMATIC, OS_CONST) Config,
  Os_Hal_ContextConfigTypeType Type
))
{
  /* #10 If the configuration is a task: */
  if (Type == Os_Hal_ContextConfigType_Task) {
    P2VAR(Os_Hal_CANoeNameType, AUTOMATIC, OS_VAR_NOINIT) taskName;

    taskName = (P2VAR(Os_Hal_CANoeNameType, AUTOMATIC, OS_VAR_NOINIT))(Config->Name);                                   /* PRQA S 0311 */ /* MD_Os_Hal_Rule11.8_0311 */

    /* #20 Configure the task name of the given context from its configuration. */
    CANoeEmuProcessor_ConfigureTaskName((sint32)(Config->Id), taskName);                                                /* SBSW_OS_HAL_FC_CALLER */
  }

  /* #30 Configure the task entry function of the given context from its configuration. */
  CANoeEmuProcessor_ConfigureTaskFunction((sint32)(Config->Id), Config->Main);                                          /* SBSW_OS_HAL_FC_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextResetCANoeTasks()
 **********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_MSR_Rule8.13 */
Os_Hal_ContextResetCANoeTasks,
(
    P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context
))
{
  sint32 result;

  /* #10 Reset the task. */
  result = CANoeEmuProcessor_RollbackStackOfTask((sint32)(Context->Config->CANoeTask.Id));
  Os_Assert((uint32)(result == CANOEEMUPROCESSOR_ROLLBACK));                                                            /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */

  /* #20 Reset the second Isrhost task. */
  result = CANoeEmuProcessor_RollbackStackOfTask((sint32)(Context->Config->CANoeIsrHost.Id));
  Os_Assert((uint32)(result == CANOEEMUPROCESSOR_ROLLBACK));                                                            /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextInit()
 **********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextInit,
(
  P2CONST(Os_Hal_ContextConfigType, AUTOMATIC, OS_CONST) Config,
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context
))
{
  /* #10 Initialize common fields of the given context from its configuration. */
  Context->Config = Config;                                                                                             /* SBSW_OS_HAL_PWA_CALLER */
  Context->CurrentInterruptState = Config->IntLockLevel;                                                                /* SBSW_OS_HAL_PWA_CALLER */
  Context->Stack = Config->Stack;                                                                                       /* SBSW_OS_HAL_PWA_CALLER */

  /* #20 Initialize both tasks(task and ISR host task) of the context. */
  Os_Hal_ContextCANoeInit(&(Config->CANoeTask), Config->Type);                                                          /* SBSW_OS_HAL_FC_CALLER */
  Os_Hal_ContextCANoeInit(&(Config->CANoeIsrHost), Config->Type);                                                       /* SBSW_OS_HAL_FC_CALLER */

  Context->ActiveCANoeTaskId = (Os_Hal_CANoeTaskIdType)(Config->CANoeTask.Id);                                          /* SBSW_OS_HAL_PWA_CALLER */
  Context->GlobalInterruptState = Os_Hal_ContextGlobalInterruptState_Enabled;                                           /* SBSW_OS_HAL_PWA_CALLER */
  Context->HostedIsr = NULL_PTR;                                                                                        /* SBSW_OS_HAL_PWA_CALLER */

  Context->ResetContext = TRUE;                                                                                         /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextReset()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_ContextReset,
(
  P2CONST(Os_Hal_ContextConfigType, AUTOMATIC, OS_CONST) Config,
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context
))
{
  /* #10 Reinitialize the common fields of the given context from its configuration. */
  Context->CurrentInterruptState = Config->IntLockLevel;                                                                /* SBSW_OS_HAL_PWA_CALLER */
  Context->GlobalInterruptState = Os_Hal_ContextGlobalInterruptState_Enabled;                                           /* SBSW_OS_HAL_PWA_CALLER */
  Context->ActiveCANoeTaskId = (Os_Hal_CANoeTaskIdType)(Config->CANoeTask.Id);                                          /* SBSW_OS_HAL_PWA_CALLER */

  /* #20 Reset the tasks in the current context. */
  Os_Hal_ContextResetCANoeTasks(Context);                                                                               /* SBSW_OS_HAL_FC_CALLER */
}

/***********************************************************************************************************************
 *  Os_Hal_ContextSetParameter()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_ContextSetParameter,
(
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context,
  P2CONST(void, AUTOMATIC, OS_VAR_NOINIT) Parameter
))
{
  /* #10 Set the first parameter of the given context. */
  Context->Parameter = Parameter;                                                                                       /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextSetStack()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_MSR_Rule8.13 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_ContextSetStack,
(
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context,
  P2CONST(Os_Hal_ContextStackConfigType, AUTOMATIC, OS_VAR_NOINIT) Stack
))
{
  /* #10 Set the used core stack configuration data. */
  Context->Stack = Stack->Stack;                                                                                        /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextGetStack()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_MSR_Rule8.13 */
Os_Hal_ContextGetStack,
(
  P2CONST(Os_Hal_ContextConfigType, AUTOMATIC, OS_CONST) Context,
  P2VAR(Os_Hal_ContextStackConfigType, AUTOMATIC, OS_VAR_NOINIT) Stack
))
{
  /* #10 Get the stack according to the given context. */
  Stack->Stack = Context->Stack;                                                                                        /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextSetInterruptState()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextSetInterruptState,
(
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context,
  P2CONST(Os_Hal_IntStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  /* #10 Set the interrupt state of the given context. */
  Context->CurrentInterruptState = *InterruptState;                                                                     /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextSetUserMsrBits()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_MSR_Rule8.13 */
Os_Hal_ContextSetUserMsrBits,
(
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context
))
{
  /*!
   * Internal comment removed.
 *
 *
   */
  OS_IGNORE_UNREF_PARAM(Context);                                                                                       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextFirstResume()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextFirstResume,
(
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Next
))
{
  /* #10 Disable all maskable interrupts. */
  (void)CANoeEmuProcessor_DisableInterrupts();

  /* #20 Set the interrupt level of the next context. */
  Os_Hal_IntSetLevel(&(Next->CurrentInterruptState));                                                                   /* SBSW_OS_HAL_FC_CALLER */
  Os_Hal_ConsumeStack(Next->Stack);                                                                                     /* SBSW_OS_HAL_FC_CALLER */

  /* #30 Start first thread. */
  Os_Hal_ContextSetTaskToRunAndRestoreInterrupts(Next);                                                                 /* SBSW_OS_HAL_FC_CALLER */

  CANoeEmuProcessor_RollbackStack();
}


/***********************************************************************************************************************
 *  Os_Hal_ContextSwitch()
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextSwitch,
(
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Current,
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Next
))
{
  Os_Hal_IntLevelType nextInterruptState;

  /* #10 Disable global interrupts. */
  Os_Hal_ContextGlobalInterruptsDisable();

  /* #20 Store common context elements to current context. */
  Os_Hal_IntGetLevel(&(Current->CurrentInterruptState));                                                                /* SBSW_OS_HAL_FC_CALLER */

  /* #30 Restore common context elements from next context. */
  nextInterruptState = Next->Config->IntLockLevel;
  Os_Hal_IntSetLevel(&nextInterruptState);                                                                              /* SBSW_OS_HAL_FC_LOCAL_VAR */
  Os_Hal_ConsumeStack(Next->Stack);                                                                                     /* SBSW_OS_HAL_FC_CALLER */

  /* #40 Start or resume next thread. */
  Os_Hal_ContextSetTaskToRunAndRestoreInterrupts(Next);                                                                 /* SBSW_OS_HAL_FC_CALLER */

  /* #50 Restore global interrupts. */
  Os_Hal_ContextGlobalInterruptsEnable();

  /* #60 If context reset is required: */
  if(Current->ResetContext == TRUE)
  {
    /* #70 Rollback current stack. */
    CANoeEmuProcessor_RollbackStack();
  }
}


/***********************************************************************************************************************
 *  Os_Hal_ContextResetAndResume()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextResetAndResume,
(
  P2CONST(Os_Hal_ContextConfigType, AUTOMATIC, OS_CONST) CurrentConfig,
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Current,
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Next
))
{
  /* #10 Disable global interrupts. */
  Os_Hal_ContextGlobalInterruptsDisable();
  Current->CurrentInterruptState = CurrentConfig->IntLockLevel;                                                         /* SBSW_OS_HAL_PWA_CALLER */

  /* #20 Start or resume next thread. */
  (void)CANoeEmuProcessor_SetInterruptLevel(Next->Config->IntLockLevel);
  Os_Hal_ConsumeStack(Next->Stack);                                                                                     /* SBSW_OS_HAL_FC_CALLER */
  Os_Hal_ContextSetTaskToRunAndRestoreInterrupts(Next);                                                                 /* SBSW_OS_HAL_FC_CALLER */

  /* #30 Restore global interrupts. */
  Os_Hal_ContextGlobalInterruptsEnable();
  CANoeEmuProcessor_RollbackStack();
}


/***********************************************************************************************************************
 *  Os_Hal_ContextAbort()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextAbort,
(
  P2CONST(Os_Hal_ContextConfigType, AUTOMATIC, OS_CONST) Config,
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context
))
{
  Context->CurrentInterruptState = Config->IntLockLevel;                                                                /* SBSW_OS_HAL_PWA_CALLER */

  /* #10 Set execution of the given context to the return function, and reset task. */
  CANoeEmuProcessor_ConfigureTaskFunction(Context->ActiveCANoeTaskId, Os_Hal_EntryISRAbort);                            /* SBSW_OS_HAL_FC_CALLER */

  Os_Hal_ContextResetCANoeTasks(Context);                                                                               /* SBSW_OS_HAL_FC_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextCall()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_MSR_Rule8.13 */
Os_Hal_ContextCall,
(
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Current,
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Next
))
{
  Os_Hal_IntGetLevel(&(Current->CurrentInterruptState));                                                                /* SBSW_OS_HAL_FC_CALLER */
  Os_Hal_IntSetLevel(&(Next->CurrentInterruptState));                                                                   /* SBSW_OS_HAL_FC_CALLER */

  /* #10 setjmp to the current context and call the entry function to the next config. */
  if(setjmp(Current->Call.JmpBuf) == 0u)                                                                                /* PRQA S 3437, 5137 */ /* MD_Os_Hal_Rule1.3_3437, MD_Os_Hal_Rule21.4_5137 */ /* SBSW_OS_HAL_FC_CALLER */
  {
    Next->Config->Entry(Next->Parameter);                                                                               /* SBSW_OS_HAL_FC_CALLER */
  }
}


/***********************************************************************************************************************
 *  Os_Hal_ContextCallOnStack()
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
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_MSR_Rule8.13 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_ContextCallOnStack,
(
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Current,
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Next
))
{
  /* #10 Store the current interrupt state. */
  Os_Hal_IntGetLevel(&(Current->CurrentInterruptState));                                                                /* SBSW_OS_HAL_FC_CALLER */

  /* #20 Set the interrupt state for the next context. */
  Os_Hal_IntSetLevel(&(Next->CurrentInterruptState));                                                                   /* SBSW_OS_HAL_FC_CALLER */

  /* #30 Consume some stack for the next context. */
  Os_Hal_ConsumeStack(Next->Stack);                                                                                     /* SBSW_OS_HAL_FC_CALLER */

  /* #40 if next context is a non-trusted function: */
  if(Next->Config->Type == Os_Hal_ContextConfigType_NontrustedFunction)
  {
    /* #50 setjmp to the current context and call the entry function to the next config. */
    if(setjmp(Current->Call.JmpBuf) == 0u)                                                                              /* PRQA S 3437, 5137 */ /* MD_Os_Hal_Rule1.3_3437, MD_Os_Hal_Rule21.4_5137 */ /* SBSW_OS_HAL_FC_CALLER */
    {
      Next->Config->Entry(Next->Parameter);                                                                             /* SBSW_OS_HAL_FC_CALLER */
    }
  }
  /* #60 else: */
  else
  {
    /* #70 start or resume next task. */
    Os_Hal_ContextSetTaskToRunAndRestoreInterrupts(Next);                                                               /* SBSW_OS_HAL_FC_CALLER */
  }
}


/***********************************************************************************************************************
 *  Os_Hal_ContextReturn()
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
Os_Hal_ContextReturn,
(
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Current,
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Next
))
{
  /* #10 if current context is a non-trusted or a trusted function: */
  if(((Current->Config->Type) == Os_Hal_ContextConfigType_TrustedFunction)
      || ((Current->Config->Type) == Os_Hal_ContextConfigType_NontrustedFunction))
  {
    Os_Hal_IntSetLevel(&(Next->CurrentInterruptState));                                                                 /* SBSW_OS_HAL_FC_CALLER */

    /* #20 longjmp to the next context. */
    /* Function longjmp never returns. */
    longjmp(((P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT))Next)->Call.JmpBuf, 1);                               /* PRQA S 0311, 5137 */ /* MD_Os_Hal_Rule11.8_0311, MD_Os_Hal_Rule21.4_5137 */ /* SBSW_OS_HAL_FC_CALLER */
  }
  /* #30 else: */
  else
  {
    /* #40 start or resume next task. */
    Os_Hal_ContextGlobalInterruptsDisable();
    Os_Hal_IntSetLevel(&(Next->CurrentInterruptState));                                                                 /* SBSW_OS_HAL_FC_CALLER */
    Os_Hal_ContextSetTaskToRunAndRestoreInterrupts(Next);                                                               /* SBSW_OS_HAL_FC_CALLER */
  }

  CANoeEmuProcessor_RollbackStack();
}


/***********************************************************************************************************************
 *  Os_Hal_ContextIsExceptionContextReadingSupported()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextIsExceptionContextReadingSupported,
(
  void
))
{
  /* #10 Return 0 as exception context cannot be read. */
  return (Os_Hal_IsFeatureActive(OS_HAL_TST_GETEXCEPTIONCONTEXTSUPPORT));                                               /* PRQA S 2996, 4404 */ /* MD_Os_Rule2.2_2996, MD_Os_C90BooleanCompatibility */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextIsExceptionContextModificationSupported()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextIsExceptionContextModificationSupported,
(
  void
))
{
  /* #10 Return 0 as exception context cannot be modified. */
  return (Os_Hal_IsFeatureActive(OS_HAL_TST_SETEXCEPTIONCONTEXTSUPPORT));                                               /* PRQA S 2996, 4404 */ /* MD_Os_Rule2.2_2996, MD_Os_C90BooleanCompatibility */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextSetTaskToRunAndRestoreInterrupts()
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
Os_Hal_ContextSetTaskToRunAndRestoreInterrupts,
(
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Next
))
{
  Os_Hal_CoreSetContext(Next);                                                                                          /* SBSW_OS_HAL_FC_CALLER */

  /* #10 if Interrupts are globally enabled: */
  if(Next->GlobalInterruptState == Os_Hal_ContextGlobalInterruptState_Enabled)
  {
    /* #20 Enable enable interrupts and run the task. */
    CANoeEmuProcessor_SetTaskToRunAndEnableInterrupts(Next->ActiveCANoeTaskId);
  }
  /* #30 else: */
  else
  {
    /* #40 Run the task. */
    CANoeEmuProcessor_SetTaskToRun(Next->ActiveCANoeTaskId);
  }
}

/***********************************************************************************************************************
 *  Os_Hal_ContextGlobalInterruptsDisable()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextGlobalInterruptsDisable, (void))
{
  P2VAR(Os_Hal_ContextType, AUTOMATIC, AUTOMATIC) currentContext;

  (void)CANoeEmuProcessor_DisableInterrupts();

  /* #10 Disable interrupt globally in the current context. */
  currentContext = Os_Hal_CoreGetContext();
  currentContext->GlobalInterruptState = Os_Hal_ContextGlobalInterruptState_Disabled;                                   /* SBSW_OS_HAL_PWA_COREGETCONTEXT */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextGlobalInterruptsEnable()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextGlobalInterruptsEnable, (void))
{
  P2VAR(Os_Hal_ContextType, AUTOMATIC, AUTOMATIC) currentContext;

  /* #10 Enable interrupt globally in the current context. */
  currentContext = Os_Hal_CoreGetContext();
  currentContext->GlobalInterruptState = Os_Hal_ContextGlobalInterruptState_Enabled;                                    /* SBSW_OS_HAL_PWA_COREGETCONTEXT */

  (void)CANoeEmuProcessor_EnableInterrupts();
}


/***********************************************************************************************************************
 *  Os_Hal_ContextSwitchToIsrHost()
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
Os_Hal_ContextSwitchToIsrHost,
(
  P2CONST(Os_IsrConfigType, AUTOMATIC, OS_CONST) Isr
))
{
  P2CONST(Os_Hal_ContextConfigType, AUTOMATIC, OS_CONST) config;
  P2VAR(Os_Hal_ContextType, AUTOMATIC, AUTOMATIC) currentContext;

  currentContext = Os_Hal_CoreGetContext();
  config = currentContext->Config;

  /* #10 Disable interrupts. */
  currentContext->GlobalInterruptStateHostBuffer = currentContext->GlobalInterruptState;                                /* SBSW_OS_HAL_PWA_COREGETCONTEXT */
  Os_Hal_ContextGlobalInterruptsDisable();

  /* #20 Set the ISR which is executed by the host. */
  currentContext->HostedIsr = Isr;                                                                                      /* SBSW_OS_HAL_PWA_COREGETCONTEXT */
  currentContext->ActiveCANoeTaskId = (Os_Hal_CANoeTaskIdType)(config->CANoeIsrHost.Id);                                /* SBSW_OS_HAL_PWA_COREGETCONTEXT */

  /* #30 Reset IsrHost of current context. */
  Isr->Thread.Context->ResetContext = TRUE;                                                                             /* SBSW_OS_HAL_PWA_CALLER */

  /* #40 Switch from the current context to its ISR host context. */
  Os_Hal_ContextSetTaskToRunAndRestoreInterrupts(currentContext);                                                       /* SBSW_OS_HAL_FC_LOCAL_VAR */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextSwitchToTask()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextSwitchToTask, (void))
{
  P2CONST(Os_Hal_ContextConfigType, AUTOMATIC, OS_CONST) config;
  P2VAR(Os_Hal_ContextType, AUTOMATIC, AUTOMATIC) currentContext;

  /* #10 Disable interrupts. */
  Os_Hal_ContextGlobalInterruptsDisable();

  currentContext = Os_Hal_CoreGetContext();
  config = currentContext->Config;

  /* #20 Update active CANoe task. */
  currentContext->ActiveCANoeTaskId = (Os_Hal_CANoeTaskIdType)(config->CANoeTask.Id);                                   /* SBSW_OS_HAL_PWA_COREGETCONTEXT */
  currentContext->GlobalInterruptState = currentContext->GlobalInterruptStateHostBuffer;                                /* SBSW_OS_HAL_PWA_COREGETCONTEXT */

  /* #30 Resume task context. */
  Os_Hal_ContextSetTaskToRunAndRestoreInterrupts(currentContext);                                                       /* SBSW_OS_HAL_FC_LOCAL_VAR */
}

/***********************************************************************************************************************
 *  Os_Hal_ContextFetchInitialFpuContext()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_MSR_Rule8.13 */
Os_Hal_ContextFetchInitialFpuContext,
(
  P2VAR(Os_Hal_ContextFpuContextType, AUTOMATIC, OS_VAR_NOINIT) InitialContext
))
{
  /* #10 Stub implementation: fpu registers are already saved in the context switch mechanism. */
  OS_IGNORE_UNREF_PARAM(InitialContext);                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextInitFpu()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextInitFpu,
(
  P2CONST(Os_Hal_ContextFpuContextType, AUTOMATIC, OS_VAR_NOINIT) InitialContext
))
{
  /* Stub implementation: fpu registers are already saved in the context switch mechanism. */
  OS_IGNORE_UNREF_PARAM(InitialContext);                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextInitFpuContext()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_MSR_Rule8.13 */
Os_Hal_ContextInitFpuContext,
(
  P2VAR(Os_Hal_ContextFpuContextType, AUTOMATIC, OS_VAR_NOINIT) Context,
  P2CONST(Os_Hal_ContextFpuContextType, AUTOMATIC, OS_VAR_NOINIT) InitialContext
))
{
  /* #10 Stub implementation: fpu registers are already saved in the context switch mechanism. */
  OS_IGNORE_UNREF_PARAM(Context);                                                                                       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(InitialContext);                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextStoreFpuContext()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_MSR_Rule8.13 */
Os_Hal_ContextStoreFpuContext,
(
  P2VAR(Os_Hal_ContextFpuContextType, AUTOMATIC, OS_VAR_NOINIT) Context
))
{
  /* #10 Stub implementation: fpu context is saved during the context switch mechanism. */
  OS_IGNORE_UNREF_PARAM(Context);                                                                                       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

/***********************************************************************************************************************
 *  Os_Hal_ContextRestoreFpuContext()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextRestoreFpuContext,
(
  P2CONST(Os_Hal_ContextFpuContextType, AUTOMATIC, OS_VAR_NOINIT) Context
))
{
  /* #10 Stub implementation: fpu context is restored during the context switch mechanism. */
  OS_IGNORE_UNREF_PARAM(Context);                                                                                       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* OS_HAL_CONTEXT_H */


/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Context.h
 **********************************************************************************************************************/

