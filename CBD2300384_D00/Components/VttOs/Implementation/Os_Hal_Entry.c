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
 *  \addtogroup Os_Hal_Entry
 *  \{
 *
 *  \file       Os_Hal_Entry.c
 *  \brief      This component handles hardware exceptions and interrupts.
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

                                                                                                                        /* PRQA S 0777 EOF */ /* MD_MSR_Rule5.1 */


#define OS_HAL_ENTRY_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */

/* Os module dependencies */
#include "Os_Cfg.h"
#include "Os_Kernel.h"

/* Os HAL dependencies */
#include "Os_Hal_Compiler.h"


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
 *  Os_Exc_MemProt()
 **********************************************************************************************************************/
/*! \brief    This handler is triggered, if a memory fault is detected.
 *  \details  In this handler it is not safe to use the current stack pointer any longer, because the memory fault
 *            may be a result of an invalid stack pointer. For this reason the handler has to switch to the kernel
 *            stack before using any stack. Scratch registers are allowed to be destroyed here, because this function
 *            never returns.
 *
 *            This handler shall call the kernel's Os_MemFault() function.
 *            Os_MemFault() expects the following parameters:
 *              <INSTRUCTION-ADDRESS>  The address of the causing instruction.
 *              <CAUSER-PRIVILEGED>    The privilege state during the violation:
 *                                       !0 During the violation, the core was privileged.
 *                                       0  During the violation, the core was not privileged.
 *
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          Supervisor mode.
 *  \pre          Interrupts disabled.
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_Exc_MemProt(void)
{
  Os_Hal_ConsumeStack(Os_CoreGetKernelStack(Os_Core2AsrCore(Os_CoreGetCurrentCore())));                                 /* SBSW_OS_HAL_FC_LOCAL_VAR */ /* SBSW_OS_HAL_FC_LOCAL_VAR */ /* SBSW_OS_HAL_FC_LOCAL_VAR */
  /* The passed address is never inside a code section and we are alwas privileged. */
  Os_MemFault((Os_AddressOfConstType)0, !0u);                                                                           /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */ /* SBSW_OS_HAL_FC_STATICPOINTER */
}


/***********************************************************************************************************************
 *  Os_Hal_EntryIrqRun()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_EntryIrqRun,
(
  P2CONST(Os_IsrConfigType, AUTOMATIC, OS_CONST) Isr
))
{
  Os_Hal_ContextSwitchToIsrHost(Isr);                                                                                   /* SBSW_OS_HAL_FC_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_EntryIsrRun()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_EntryIsrRun, (void))
{
  P2CONST(Os_Hal_ContextType, AUTOMATIC, AUTOMATIC) currentContext;

  for(;;)                                                                                                               /* FETA_OS_HALVTTENTRYISRRUN */
  {
    currentContext = Os_Hal_CoreGetContext();
    Os_IsrRun(currentContext->HostedIsr);                                                                               /* SBSW_OS_HAL_FC_LOCAL_VAR */
    Os_Hal_ContextSwitchToTask();
  }
}


/***********************************************************************************************************************
 *  Os_Hal_EntryExecuteThread()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_EntryExecuteThread, (void))
{
  P2VAR(Os_Hal_ContextType, AUTOMATIC, AUTOMATIC) currentContext;

  currentContext = Os_Hal_CoreGetContext();

  currentContext->ResetContext = FALSE;                                                                                 /* SBSW_OS_HAL_PWA_COREGETCONTEXT */

  currentContext->Config->Entry(NULL_PTR);                                                                              /* SBSW_OS_HAL_FC_COREGETCONTEXT */

  currentContext->Config->ReturnAddress();                                                                              /* SBSW_OS_HAL_FC_COREGETCONTEXT */
}


/***********************************************************************************************************************
 *  Os_Hal_EntryExecuteThreadWithParam()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_EntryExecuteThreadWithParam, (void))
{
  P2VAR(Os_Hal_ContextType, AUTOMATIC, AUTOMATIC) currentContext;

  currentContext = Os_Hal_CoreGetContext();

  currentContext->ResetContext = FALSE;                                                                                 /* SBSW_OS_HAL_PWA_COREGETCONTEXT */

  currentContext->Config->Entry(currentContext->Parameter);                                                             /* SBSW_OS_HAL_FC_COREGETCONTEXT */

  currentContext->Config->ReturnAddress();                                                                              /* SBSW_OS_HAL_FC_COREGETCONTEXT */
}


/***********************************************************************************************************************
 *  Os_Hal_EntryISRAbort()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_EntryISRAbort, (void))
{
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) currentContext;

  currentContext = Os_Hal_CoreGetContext();

  CANoeEmuProcessor_ConfigureTaskFunction((sint32)(currentContext->Config->CANoeTask.Id),                               /* SBSW_OS_HAL_FC_CALLER */
                                          (void (*)(void))currentContext->Config->CANoeTask.Main);

  CANoeEmuProcessor_ConfigureTaskFunction((sint32)(currentContext->Config->CANoeIsrHost.Id),                            /* SBSW_OS_HAL_FC_CALLER */
                                          (void (*)(void))currentContext->Config->CANoeIsrHost.Main);

  currentContext->Config->ReturnAddress();                                                                              /* SBSW_OS_HAL_FC_COREGETCONTEXT */
}

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Entry.c
 **********************************************************************************************************************/

