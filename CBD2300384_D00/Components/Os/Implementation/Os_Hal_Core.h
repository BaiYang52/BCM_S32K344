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
 * \addtogroup  Os_Hal_Core
 * \{
 *
 * \file
 * \brief   Core and multi-core related primitives.
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */

#ifndef OS_HAL_CORE_H
# define OS_HAL_CORE_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Common.h"
# include "Os_Error.h"

/* Os HAL dependencies */
# include "Os_Hal_CoreInt.h"
# include "Os_Hal_MemoryProtectionInt.h"
# include "Os_Hal_Interrupt.h"
# include "Os_Hal_Compiler.h"
# include "Os_Hal_Context.h"

# if defined(OS_CFG_DERIVATIVEGROUP_TDA2X)                                                                              /* COV_OS_DERIVATIVEBETAMODULE */
# include "Os_Hal_Core_Jacinto.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_S32X)                                                                             /* COV_OS_DERIVATIVEBETAMODULE */
# include "Os_Hal_Core_S32x.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_S32G3X)                                                                           /* COV_OS_DERIVATIVEBETAMODULE */
# include "Os_Hal_Core_S32x.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_S32K3XX)                                                                          /* COV_OS_DERIVATIVEBETAMODULE */
# include "Os_Hal_Core_S32K3x.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_CYT4BFX)                                                                          /* COV_OS_DERIVATIVEBETAMODULE */
# include "Os_Hal_Core_CYT4Bx.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_CYT4BBX)                                                                          /* COV_OS_DERIVATIVEBETAMODULE */
# include "Os_Hal_Core_CYT4Bx.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_CYT4DNX)                                                                          /* COV_OS_DERIVATIVEBETAMODULE */
# include "Os_Hal_Core_CYT4Bx.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_IMX8XDUALCORE)                                                                    /* COV_OS_DERIVATIVEBETAMODULE */
# include "Os_Hal_Core_IMX8.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_IMXRT117X)                                                                        /* COV_OS_DERIVATIVEBETAMODULE */
# include "Os_Hal_Core_IMXRT.h"
# elif defined(OS_HAL_ARCH_V8M)                                                                                         /* COV_OS_DERIVATIVEBETAMODULE */
# include "Os_Hal_Core_v8m.h"
# elif defined(OS_HAL_ARCH_V7M)
# include "Os_Hal_Core_v7m.h"
# else
# error "Undefined or unsupported platform"
# endif


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
 *  Os_Hal_CoreSetThread()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_CoreSetThread,
(
  P2CONST(Os_Hal_Core2ThreadConfigType, AUTOMATIC, OS_CONST) Core2Thread,
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) Thread
))
{
  /* #10 Set thread pointer in memory. */
  Core2Thread->Core2Thread[Os_Hal_CoreGetId()]->Thread = Thread;                                                        /* SBSW_OS_HAL_CORESETTHREAD_001 */
}


/***********************************************************************************************************************
 *  Os_Hal_CoreGetThread()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE                                                                            /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST), OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_CoreGetThread,
(
  P2CONST(Os_Hal_Core2ThreadConfigType, AUTOMATIC, OS_CONST) Core2Thread
))
{
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) threadConfig;

  /* #10 Initialize thread pointer from memory. */
  threadConfig = Core2Thread->Core2Thread[Os_Hal_CoreGetId()]->Thread;

  /* #20 Return thread pointer from memory. */
  return threadConfig;
}


/***********************************************************************************************************************
 *  Os_Hal_CoreIsPrivilegedModeUserReadable()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreIsPrivilegedModeUserReadable,
(
  void
))
{
  /* #10 Return !0 (Privileges readable in user mode). */
  return !0u;                                                                                                           /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}


/***********************************************************************************************************************
 *  Os_Hal_CoreGetPrivilegedMode()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreGetPrivilegedMode,
(
  void
))
{
  /* This function must only be called in Thread mode. */
  Os_Assert(((uint32)Os_Hal_ReadPsr() & OS_HAL_PSR_IPSR_MASK) == 0u);                                                   /* PRQA S 4404 */ /* MD_Os_C90BooleanCompatibility */

  /* #10 Return whether Thread mode runs privileged or unprivileged. */
  return (Os_StdReturnType)(((uint32)Os_Hal_ReadControl() & OS_HAL_CONTROL_PRIVILEGED_MASK) ==                          /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */ \
                              OS_HAL_CONTROL_PRIVILEGED);
}


/***********************************************************************************************************************
 *  Os_Hal_CountLeadingZeros()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE uint8, OS_CODE, OS_ALWAYS_INLINE,                                          /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CountLeadingZeros,
(
  Os_Hal_BitFieldType BitField
))
{
  /* #10 Execute CLZ instruction. */
  return (uint8)Os_Hal_CLZ(BitField);
}


/***********************************************************************************************************************
 *  Os_Hal_StackEndGet()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_Os_Hal_Rule8.13_3673_NotUsed */
OS_LOCAL_INLINE P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_StackEndGet,
(
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) LowAddress,
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) HighAddress
))
{
  OS_IGNORE_UNREF_PARAM(HighAddress);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* #10 Return LowAddress. */
  return LowAddress;
}


/***********************************************************************************************************************
 *  Os_Hal_StackBottomGet()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_Os_Hal_Rule8.13_3673_NotUsed */
OS_LOCAL_INLINE P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_StackBottomGet,
(
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) LowAddress,
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) HighAddress
))
{
  OS_IGNORE_UNREF_PARAM(LowAddress);                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* #10 Return HighAddress. */
  return HighAddress;
}


/***********************************************************************************************************************
 *  Os_Hal_StackTopGet()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_StackTopGet,
(
  void
))
{
  /* #10 Return SP value. */
  return (P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT))Os_Hal_ReadStackPointer();                                  /* PRQA S 0306 */ /* MD_Os_Hal_Rule11.4_0306 */
}


/***********************************************************************************************************************
 *  Os_Hal_StackPointerMoveDown()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_StackPointerMoveDown,
(
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) StackPointer
))
{
  /* #10 Increment given address. */
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT)localStackPointer=StackPointer;
  localStackPointer++;
  return localStackPointer;
}


/***********************************************************************************************************************
 *  Os_Hal_StackCalculateUsage()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE uint32, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_StackCalculateUsage,
(
  P2CONST(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) StackBottom,
  P2CONST(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) StackTop
))
{
  /* #10 Return (Bottom-Top)*SizeOf(Element). */
  return ((uint32)StackBottom - (uint32)StackTop);                                                                      /* PRQA S 0306 */ /* MD_Os_Hal_Rule11.4_0306 */
}


/***********************************************************************************************************************
 *  Os_Hal_SpinIsLocked()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_SpinIsLocked,
(
  P2CONST(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
))
{
  /* #10 Return spinlock value. */
  return (Os_StdReturnType)(*Spinlock);
}
/***********************************************************************************************************************
 *  Os_Hal_SpinUnlock()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_SpinUnlock,
(
  P2VAR(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
))
{
  /* #10 Memory barrier. */
  Os_Hal_DataMemoryBarrier();

  /* #20 Set spinlock to 0. */
  *Spinlock = 0;                                                                                                        /* SBSW_OS_HAL_PWA_CALLER */
}
/***********************************************************************************************************************
 *  Os_Hal_CoreIdle()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreIdle, (void))
{
  /* #10 Do nothing. */
  Os_Hal_CoreNop();
}


/***********************************************************************************************************************
 *  Os_Hal_CoreNop()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreNop, (void))
{
  /* #10 Do nothing. */
  Os_Hal_Nop();
}


/***********************************************************************************************************************
 *  Os_Hal_Core_InitExceptions()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_Core_InitExceptions,
(
  P2CONST(Os_Hal_CoreAsrConfigType, AUTOMATIC, OS_CONST) Config
))
{
  P2VAR(volatile Os_Hal_CoreV7MSystemHandlerType, AUTOMATIC, OS_VAR_NOINIT) sysHandler
                      = (P2VAR(Os_Hal_CoreV7MSystemHandlerType, AUTOMATIC, OS_VAR_NOINIT)) OS_HAL_SCS_SH_BASE_ADDRESS;  /* PRQA S 0306 */ /* MD_Os_Hal_Rule11.4_0306 */

  /* #10 Initialize the System Handler priorities. */
  sysHandler->Shpr1 = Config->ExceptionPriority[0];                                                                     /* SBSW_OS_HAL_CORE_INITEXCEPTIONS_001 */
  sysHandler->Shpr2 = Config->ExceptionPriority[1];                                                                     /* SBSW_OS_HAL_CORE_INITEXCEPTIONS_001 */
  sysHandler->Shpr3 = Config->ExceptionPriority[2];                                                                     /* SBSW_OS_HAL_CORE_INITEXCEPTIONS_001 */

  /* #20 Activate the additional fault System Handlers to differentiate between fault causes. */
  sysHandler->Shcsr |= (OS_HAL_SCS_SH_SHCSR_USGFAULTENA | OS_HAL_SCS_SH_SHCSR_BUSFAULTENA |                             /* SBSW_OS_HAL_CORE_INITEXCEPTIONS_001 */
                        OS_HAL_SCS_SH_SHCSR_MEMFAULTENA);

  /* #30 Allow for exception handlers to return to thread state at any nesting level. */
  Os_Hal_SetsysHandlerCcr();                                                                         /* SBSW_OS_HAL_CORE_INITEXCEPTIONS_001 */
}

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* OS_HAL_CORE_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Core.h
 **********************************************************************************************************************/

