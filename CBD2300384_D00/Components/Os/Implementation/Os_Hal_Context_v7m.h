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
 *  \{
 *
 *  \file
 *  \brief      Context switching primitives for ARMv7-M.
 *  \details
 *  Internal comment removed.
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
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */

#ifndef OS_HAL_CONTEXT_V7M_H
# define OS_HAL_CONTEXT_V7M_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

# include "Os_Hal_Derivative.h"

/* Os kernel module dependencies */
# include "Os_Common.h"

/* Os Hal dependencies */
# include "Os_Hal_ContextInt.h"
# include "Os_Hal_Compiler.h"
# include "Os_Hal_CoreInt.h"
# include "Os_Hal_MemoryProtection.h"

/* dependency to service functions */
# include "Os_ServiceFunction_Types.h"

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  Os_Hal_ContextSuspend()
 *********************************************************************************************************************/
/*! \brief        Save current context to given address and its PC to given value.
 *  \details      --no details--
 *
 *  \param[out]   CURRENT        Address where to store current register values.
 *                               Parameter must not be NULL.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
# define Os_Hal_ContextSuspend(CURRENT) \
{                                                                                              \
  Os_Hal_ContextStackConfigType StackConfig;                                                   \
                                                                                               \
  /* #10 Global lock flag is deactivated. */                                                   \
  (CURRENT)->Primask = OS_HAL_PRIMASK_INTERRUPTS_NOT_MASKED;                                   \
                                                                                               \
  /* #20 Save BASEPRI, CONTROL, stack region, permanent registers. */                          \
  (CURRENT)->Basepri = (uint8)Os_Hal_ReadBasepri();                                            \
  (CURRENT)->Control = (uint8)(Os_Hal_ReadControl() & (~OS_HAL_CONTROL_FPCA_MASK));            \
                                                                                               \
  Os_Hal_MpGetStackRegion(&StackConfig);                                                       \
  if(Os_MpIsEnabled() != 0u)                                                                   \
  {                                                                                            \
    (CURRENT)->StackRegionStart = StackConfig.StackRegionStart;                                \
    (CURRENT)->StackRegionInfo = OS_HAL_MPU_GETSTACKREGIONINFO(StackConfig);                   \
  }                                                                                            \
                                                                                               \
  Os_Hal_StorePermanentRegisters((uint32)((CURRENT)->PermanentRegisters));                     \
}


/***********************************************************************************************************************
 *  Os_Hal_ContextResume()
 **********************************************************************************************************************/
/*! \brief        Restore context from given address.
 *  \details      --no details--
 *
 *  \param[in]    NEXT        Address where to load register values from. Parameter must not be NULL.
 *  \param[in]    SPRADDR     Address where to store the previous Sp and Lr. May be zero to skip the saving.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
# define Os_Hal_ContextResume(NEXT, SPRADDR) \
{                                                                                          \
  Os_Hal_ContextSwitchInfoType contextSwitchInfo;                                          \
  /* #10 Restore CONTROL leaving the FPCA bit unchanged. */                                \
  Os_Hal_WriteControl((NEXT)->Control | (Os_Hal_ReadControl() & OS_HAL_CONTROL_FPCA_MASK));\
                                                                                           \
  /* #20 Prepare the context switch structure for the assembly part. */                    \
  contextSwitchInfo.NextSprPtr        = (uint32)(&((NEXT)->Sp));                           \
  contextSwitchInfo.PermRegsPtr       = (uint32)((NEXT)->PermanentRegisters);              \
  contextSwitchInfo.Basepri           = (uint32)((NEXT)->Basepri);                         \
  contextSwitchInfo.SaveSprPtr        = (uint32)(SPRADDR);                                 \
  contextSwitchInfo.MpuStackRegionPtr = (uint32)(&((NEXT)->StackRegionStart));             \
                                                                                           \
  /* #30 Call SvCall exception with context switch sub-function. */                        \
  Os_Hal_Trigger_Context_SvCall((uint32)(&contextSwitchInfo));                             \
}


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
 *  Os_Hal_ContextInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_ContextInit,
(
  P2CONST(Os_Hal_ContextConfigType, AUTOMATIC, OS_CONST) Config,
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context
))
{
  /* #10 Reset given context. */
  Os_Hal_ContextReset(Config, Context);                                                                                 /* SBSW_OS_HAL_FC_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextReset()
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
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_ContextReset,
(
  P2CONST(Os_Hal_ContextConfigType, AUTOMATIC, OS_CONST) Config,
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context
))
{
  /* #10 Set SP to the address of the execution frame. */
  Context->Sp = (uint32)(&Context->ExcFrame);                                                                           /* SBSW_OS_HAL_PWA_CALLER */ /* PRQA S 0306 */ /* MD_Os_Hal_Rule11.4_0306 */

  /* #20 Set LR in order to perform the exception return to Thread Mode using the Main Stack. */
  Context->Lr = 0xFFFFFFF9u;                                                                                            /* SBSW_OS_HAL_PWA_CALLER */

  /* #30 Prepare the stack region using the configured values. */
  Context->StackRegionStart = Config->StackRegionStart;                                                                 /* SBSW_OS_HAL_PWA_CALLER */
  Context->StackRegionInfo = Config->StackRegionInfo;                                                                   /* SBSW_OS_HAL_PWA_CALLER */

  /* #40 Set CONTROL register to privileged value. */
  Context->Control = OS_HAL_CONTROL_PRIVILEGED;                                                                         /* SBSW_OS_HAL_PWA_CALLER */

  /* #50 Set BASEPRI to the configured value. */
  Context->Basepri = Config->Basepri;                                                                                   /* SBSW_OS_HAL_PWA_CALLER */

  /* #60 Set PRIMASK to disable all interrupts during trampoline execution. */
  Context->Primask = OS_HAL_PRIMASK_INTERRUPTS_MASKED;                                                                  /* SBSW_OS_HAL_PWA_CALLER */

  /* #70 Prepare the exception frame program counter for the first execution. */
  Context->ExcFrame.Pc = (uint32)(&Os_Hal_StartNewThread);                                                              /* SBSW_OS_HAL_PWA_CALLER */ /* PRQA S 0305 */ /* MD_Os_Hal_Rule11.1_0305 */

  /* #80 Save in the exception frame configuration values to be used in the Os_Hal_StartNewThread. */
  Context->ExcFrame.R1  = Config->StackRegionEnd;                                                                       /* SBSW_OS_HAL_PWA_CALLER */
  Context->ExcFrame.R3  = Config->Entry;                                                                                /* SBSW_OS_HAL_PWA_CALLER */
  Context->ExcFrame.R12 = Config->Control;                                                                              /* SBSW_OS_HAL_PWA_CALLER */

  /* #90 Prepare the exception frame Psr register for the first execution with thumb mode bit set. */
  Context->ExcFrame.Psr = OS_HAL_PSR_THUMB_BIT;                                                                         /* SBSW_OS_HAL_PWA_CALLER */

  /* #100 Prepare the exception frame link register with the configured return address. */
  Context->ExcFrame.Lr  = Config->ReturnAddress;                                                                        /* SBSW_OS_HAL_PWA_CALLER */

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
  /* #10 Set parameter register of given context to given value. */
  Context->ExcFrame.R0 = (uint32)Parameter;                                                                               /* PRQA S 0306, 0326 */ /* MD_Os_Hal_Rule11.4_0306, MD_Os_Rule11.6_0326 */ /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextSetStack()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_ContextSetStack,
(
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context,
  P2CONST(Os_Hal_ContextStackConfigType, AUTOMATIC, OS_VAR_NOINIT) Stack
))
{
  /* #10 Set SP and stack region of given context to given stack configuration. */
  Context->ExcFrame.R1 = Stack->StackRegionEnd;                                                                         /* SBSW_OS_HAL_PWA_CALLER */
  Context->StackRegionStart = Stack->StackRegionStart;                                                                  /* SBSW_OS_HAL_PWA_CALLER */
  Context->StackRegionInfo = OS_HAL_MPU_GETSTACKREGIONINFO((*Stack));                                                   /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextGetStack()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextGetStack,
(
  P2CONST(Os_Hal_ContextConfigType, AUTOMATIC, OS_CONST) Context,
  P2VAR(Os_Hal_ContextStackConfigType, AUTOMATIC, OS_VAR_NOINIT) Stack
))
{
   /* #10 Get stack config from given context config. */
   Stack->StackRegionStart = Context->StackRegionStart;                                                                 /* SBSW_OS_HAL_PWA_CALLER */
   Stack->StackRegionSize = Context->StackRegionInfo;                                                                   /* SBSW_OS_HAL_PWA_CALLER */
   Stack->StackRegionEnd = 0;                                                                                           /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextSetInterruptState()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_ContextSetInterruptState,
(
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context,
  P2CONST(Os_Hal_IntStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  /* #10 Set interrupt level to given level. */
  Context->Basepri  = (uint8)InterruptState->InterruptLevel;                                                            /* SBSW_OS_HAL_PWA_CALLER */

}


/***********************************************************************************************************************
 *  Os_Hal_ContextSetUserMsrBits()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_Os_Hal_Rule8.13_3673_NotUsed */
Os_Hal_ContextSetUserMsrBits,
(
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context
))
{
  /* #10 Nothing to do here. All processor status bits are controlled by the OS. */
  OS_IGNORE_UNREF_PARAM(Context);                                                                                       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextFirstResume()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_ContextFirstResume,
(
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Next
))
{
  /* #10 Resume Next. */
  Os_Hal_ContextResume(Next, 0);                                                                                        /* PRQA S 0306, 2983 */ /* MD_Os_Hal_Rule11.4_0306, MD_Os_Rule2.2_2983 */ /* COV_OS_HALPLATFORMCONTEXTDOWHILENORETURN */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextResetAndResume()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_ContextResetAndResume,
(
  P2CONST(Os_Hal_ContextConfigType, AUTOMATIC, OS_CONST) CurrentConfig,
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Current,
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Next
))
{
  /* #10 Reset given Current context. */
  Os_Hal_ContextReset(CurrentConfig, Current);                                                                          /* SBSW_OS_HAL_FC_CALLER */

  /* #20 Resume Next. */
  Os_Hal_ContextResume(Next, 0);                                                                                        /* PRQA S 0306, 2983 */ /* MD_Os_Hal_Rule11.4_0306, MD_Os_Rule2.2_2983 */ /* COV_OS_HALPLATFORMCONTEXTDOWHILENORETURN */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextAbort()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_ContextAbort,
(
  P2CONST(Os_Hal_ContextConfigType, AUTOMATIC, OS_CONST) Config,
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context
))
{
  /* #10 Reset given context. */
  Os_Hal_ContextReset(Config, Context);                                                                                 /* SBSW_OS_HAL_FC_CALLER */

  /* #20 Set PC to configured return address. */
  Context->ExcFrame.R3 = Config->ReturnAddress;                                                                         /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextReturn()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_ContextReturn,
(
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Current,
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Next
))
{
  OS_IGNORE_UNREF_PARAM(Current);                                                                                       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* #10 Resume Next. */
  Os_Hal_ContextResume(Next, 0);                                                                                        /* PRQA S 0306, 2983 */ /* MD_Os_Hal_Rule11.4_0306, MD_Os_Rule2.2_2983 */ /* COV_OS_HALPLATFORMCONTEXTDOWHILENORETURN */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextIsExceptionContextReadingSupported()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextIsExceptionContextReadingSupported,
(
  void
))
{
  return !0u;                                                                                                           /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}


/***********************************************************************************************************************
 *  Os_Hal_ContextIsExceptionContextModificationSupported()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextIsExceptionContextModificationSupported,
(
  void
))
{
  return 0;
}


/***********************************************************************************************************************
 *  Os_Hal_ContextFetchInitialFpuContext()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_Os_Hal_Rule8.13_3673_NotUsed */
Os_Hal_ContextFetchInitialFpuContext,
(
  P2VAR(Os_Hal_ContextFpuContextType, AUTOMATIC, OS_VAR_NOINIT) InitialContext
))
{
  /* #10 Stub implementation: fpu context is saved by hardware on exception entry. */
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
  /* #10 Stub implementation: fpu context is saved by hardware on exception entry. */
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_Os_Hal_Rule8.13_3673_NotUsed */
Os_Hal_ContextInitFpuContext,
(
  P2VAR(Os_Hal_ContextFpuContextType, AUTOMATIC, OS_VAR_NOINIT) Context,
  P2CONST(Os_Hal_ContextFpuContextType, AUTOMATIC, OS_VAR_NOINIT) InitialContext
))
{
  /* #10 Stub implementation: fpu context is saved by hardware on exception entry. */
  OS_IGNORE_UNREF_PARAM(Context);                                                                                       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(InitialContext);                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

/***********************************************************************************************************************
 *  Os_Hal_ContextStoreFpuContext()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_Os_Hal_Rule8.13_3673_NotUsed */
Os_Hal_ContextStoreFpuContext,
(
  P2VAR(Os_Hal_ContextFpuContextType, AUTOMATIC, OS_VAR_NOINIT) Context
))
{
#if (OS_CFG_THREAD_FPU_CONTEXT_ENABLED == STD_ON)
  Os_Hal_StoreFpuRegisters((uint32)&(Context->FpuAdditionalRegisters));
#else
  OS_IGNORE_UNREF_PARAM(Context);                                                                                       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#endif
}

/***********************************************************************************************************************
 *  Os_Hal_ContextRestoreFpuContext()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_ContextRestoreFpuContext,
(
  P2CONST(Os_Hal_ContextFpuContextType, AUTOMATIC, OS_VAR_NOINIT) Context
))
{
#if (OS_CFG_THREAD_FPU_CONTEXT_ENABLED == STD_ON)
  Os_Hal_RestoreFpuRegisters((uint32)&(Context->FpuAdditionalRegisters));
#else
  OS_IGNORE_UNREF_PARAM(Context);                                                                                       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#endif
}

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* OS_HAL_CONTEXT_V7M_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Context_v7m.h
 **********************************************************************************************************************/

