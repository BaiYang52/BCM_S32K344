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
 *  \addtogroup Os_Hal_Interrupt
 *  \{
 *
 *  \file  Os_Hal_Interrupt.h
 *  \brief  Functions for interrupt handling.
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */

#ifndef OS_HAL_INTERRUPT_H
# define OS_HAL_INTERRUPT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Hal_InterruptInt.h"
# include "Os_Hal_Interrupt_Lcfg.h"

/* Os kernel module declarations */
# include "Os_Cfg.h"
# include "Os_Common.h"
# include "Os_TimingProtection_Cfg.h"

/* Os Hal dependencies */
# include "Os_Hal_Derivative.h"
# include "Os_Hal_Compiler.h"
# include "Os_Hal_Core.h"
# include "Os_Hal_InterruptController.h"

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


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  Os_Hal_IntEnableGlobal()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntEnableGlobal, (void))
{
   /* #10 Set current BASEPRI value in order to enable all configured interrupts. */
   Os_Hal_INTC_SetCurrentLevel(OS_HAL_TASK_LEVEL);
}


/***********************************************************************************************************************
 *  Os_Hal_IntDisableGlobal()
 **********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntDisableGlobal, (void))
{
  /* #10 Set current BASEPRI value in order to disable all configured interrupts. */
  Os_Hal_INTC_SetCurrentLevel(OS_CFG_HAL_GLOBAL_DISABLE_LEVEL);
}


/***********************************************************************************************************************
 *  Os_Hal_IntDisableAndReturnGlobalState()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_Hal_IntGlobalStateType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntDisableAndReturnGlobalState,(void))
{
  /* #10 Get current BASEPRI value (no global flag available). */
  volatile Os_Hal_IntGlobalStateType previousLevel;
  previousLevel = Os_Hal_INTC_GetCurrentLevel();

  /* #20 Set current BASEPRI value in order to disable all configured interrupts. */
  Os_Hal_INTC_SetCurrentLevel(OS_CFG_HAL_GLOBAL_DISABLE_LEVEL);

  /* #30 Return the previously read BASEPRI value. */
  return previousLevel;
}


/***********************************************************************************************************************
  *  Os_Hal_IntSetGlobalState()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntSetGlobalState,
(
   P2CONST(Os_Hal_IntGlobalStateType, AUTOMATIC, OS_APPL_DATA) State
))
{
  /* #10 Write the given state to BASEPRI register (no global flag available). */
  Os_Hal_INTC_SetCurrentLevel(*State);
}


/***********************************************************************************************************************
 *  Os_Hal_IntGetLevel()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntGetLevel,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
))
{
  /* #10 Write current interrupt level to given pointer. */
  *Level = Os_Hal_INTC_GetCurrentLevel();                                                                               /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
  *  Os_Hal_IntSetLevel()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntSetLevel,
(
   P2CONST(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
))
{
  /* #10 Set interrupt level to given level. */
  Os_Hal_INTC_SetCurrentLevel(*Level);
}


/***********************************************************************************************************************
 *  Os_Hal_IntGetGlobalEnable()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntGetGlobalEnable,
(
  P2VAR(Os_Hal_IntGlobalStateType, AUTOMATIC, OS_APPL_DATA) State
))                                                                                                                      /* COV_OS_HALPLATFORMGLOBAL2LEVELSUPPORTED */
{
  /* #10 Return the BASEPRI value to enable all interrupts. */
  *State = OS_HAL_TASK_LEVEL;                                                                                           /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_IntGetCat2LockLevel()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntGetCat2LockLevel,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
))
{
  /* #10 Return the BASEPRI value to disable up to Cat2 ISR lock level. */
  *Level = OS_CFG_HAL_ISR2_DISABLE_LEVEL;                                                                               /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_IntGetCat1LockLevel()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntGetCat1LockLevel,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
))
{
  /* #10 Return the BASEPRI value to disable up to Cat1 ISR lock level. */
  *Level = OS_CFG_HAL_ISR1_DISABLE_LEVEL;                                                                               /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_IntGetTPLockLevel()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntGetTPLockLevel,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
))
{
  /* #10 Return the BASEPRI value to disable up to timing protection lock level. */
  *Level = OS_CFG_HAL_TP_DISABLE_LEVEL;                                                                                 /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
  *  Os_Hal_IntGetTaskLevel()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntGetTaskLevel,
(
   P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
))
{
  /* #10 Returns the interrupt priority level which is used to get back to task level. */
  *Level = OS_HAL_TASK_LEVEL;                                                                                           /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_IntDisableSource()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntDisableSource,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  /* #10 Disable given interrupt. */
  Os_Hal_INTC_DisableSource(IsrConfig->CoreInterruptSource);
}

/***********************************************************************************************************************
 *  Os_Hal_IntDisableSourceMapped()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntDisableSourceMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  OS_IGNORE_UNREF_PARAM(IsrConfig);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  /* #10 Disable given mapped interrupt. */
  Os_Hal_INTC_DisableMappedSource(IsrMapConfig->SystemInterruptSource);
}


/***********************************************************************************************************************
 *  Os_Hal_IntEnableSource()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntEnableSource,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  Os_Hal_INTC_EnableNotMappedSource(IsrConfig->CoreInterruptSource);
}

/***********************************************************************************************************************
 *  Os_Hal_IntEnableSourceMapped()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntEnableSourceMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  /* #10 Enable given mapped interrupt. */
  Os_Hal_INTC_EnableMappedSource(IsrMapConfig->SystemInterruptSource, IsrConfig->CoreInterruptSource);
}


/***********************************************************************************************************************
 *  Os_Hal_IntIsClearPendingSupported()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsClearPendingSupported, ( void ))
{
  /* #10 Returns the interrupt controller specific value. */
  return Os_Hal_INTC_IsClearPendingSupported();
}


/***********************************************************************************************************************
 *  Os_Hal_IntClearPending()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* COV_OS_HALPLATFORMCLEARPENDINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntClearPending,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
))                                                                                                                      /* COV_OS_HALPLATFORMCLEARPENDINGAVAILABILITY */
{
  /* #10 Signal end of interrupt for the given interrupt. */
  Os_Hal_INTC_EndOfInterrupt(IsrConfig);                                                                                /* SBSW_OS_HAL_FC_CALLER */
}

/***********************************************************************************************************************
 *  Os_Hal_IntClearPendingMapped()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* COV_OS_HALPLATFORMCLEARSYSTEMPENDINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntClearPendingMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
))                                                                                                                      /* COV_OS_HALPLATFORMCLEARSYSTEMPENDINGAVAILABILITY */
{
  OS_IGNORE_UNREF_PARAM(IsrMapConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  /* #10 Signal end of interrupt for the given interrupt. */
  Os_Hal_INTC_EndOfInterrupt(IsrConfig);                                                                                /* SBSW_OS_HAL_FC_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptSourceEnabled()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsInterruptSourceEnabled,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  /* #10 Return whether given interrupt is enabled. */
  return Os_Hal_INTC_IsInterruptSourceEnabled(IsrConfig->CoreInterruptSource);
}

/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptSourceEnabledMapped()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsInterruptSourceEnabledMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  /* #10 Return whether given mapped interrupt is enabled. */
  return Os_Hal_INTC_IsMappedInterruptSourceEnabled(IsrConfig->CoreInterruptSource, IsrMapConfig->SystemInterruptSource);
}

/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptPending()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* COV_OS_HALPLATFORMCLEARPENDINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsInterruptPending,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
))                                                                                                                      /* COV_OS_HALPLATFORMCLEARPENDINGAVAILABILITY */
{
  /* #10 Return whether given interrupt is pending. */
  return Os_Hal_INTC_IsInterruptPending(IsrConfig);                                                                     /* SBSW_OS_HAL_FC_CALLER */
}

/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptPendingMapped()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* COV_OS_HALPLATFORMCLEARSYSTEMPENDINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsInterruptPendingMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
))                                                                                                                      /* COV_OS_HALPLATFORMCLEARSYSTEMPENDINGAVAILABILITY */
{
  OS_IGNORE_UNREF_PARAM(IsrMapConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  /* #10 Return whether given mapped interrupt is pending. */
  return Os_Hal_INTC_IsInterruptPending(IsrConfig);                                                                     /* SBSW_OS_HAL_FC_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_IntEndOfInterrupt()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntEndOfInterrupt,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
))
{
  /* #10 Call the corresponding interrupt controller specific end of interrupt function. */
  Os_Hal_INTC_EndOfInterrupt(IsrConfig);                                                                                /* SBSW_OS_HAL_FC_CALLER */
}

/***********************************************************************************************************************
 *  Os_Hal_IntEndOfInterruptMapped()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntEndOfInterruptMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  OS_IGNORE_UNREF_PARAM(IsrMapConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  /* #10 Call the corresponding interrupt controller specific end of interrupt function. */
  Os_Hal_INTC_EndOfInterrupt(IsrConfig);                                                                                /* SBSW_OS_HAL_FC_CALLER */
}

/***********************************************************************************************************************
 *  Os_Hal_IntIsrLevelIsLe()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntIsrLevelIsLe,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntLevelType, AUTOMATIC, OS_CONST) Level
))
{
  /* #10 Delegate the decision to the interrupt controller specific function. */
  return !(Os_Hal_INTC_IntLevelIsGt(IsrConfig->Level, *Level));                                                         /* PRQA S 4116, 4404, 4558 */ /* MD_Os_Rule10.1_4116, MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}


/***********************************************************************************************************************
 *  Os_Hal_IntLevelIsGt()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntLevelIsGt,
(
  P2CONST(Os_Hal_IntLevelType, AUTOMATIC, OS_CONST) LevelX,
  P2CONST(Os_Hal_IntLevelType, AUTOMATIC, OS_CONST) LevelY
))
{
  /* #10 Delegate the decision to the interrupt controller specific function. */
  return Os_Hal_INTC_IntLevelIsGt(*LevelX, *LevelY);
}


/***********************************************************************************************************************
 *  Os_Hal_IntTrapRestore()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,                                                             /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_ALWAYS_INLINE, Os_Hal_IntTrapRestore,
(
  P2CONST(Os_Hal_IntTrapStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  /* #10 Restore given interrupt state. */
  Os_Hal_WriteBasepri(*InterruptState);
}


/***********************************************************************************************************************
 *  Os_Hal_IntInterruptSourceInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntInterruptSourceInit,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) InterruptSource
))
{
  /* #10 Call the corresponding interrupt controller specific initialization function. */
  Os_Hal_INTC_InitSource(InterruptSource);                                                                              /* SBSW_OS_HAL_FC_CALLER */
}

/***********************************************************************************************************************
 *  Os_Hal_IntInterruptSourceInitMapped()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntInterruptSourceInitMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  /* #10 Call the corresponding interrupt controller specific initialization function. */
  Os_Hal_INTC_InitSource(IsrConfig);                                                                                    /* SBSW_OS_HAL_FC_CALLER */
  OS_IGNORE_UNREF_PARAM(IsrMapConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_IntIsGlobal2LevelSwitchSupported()
 **********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE,                                                 /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_ALWAYS_INLINE, Os_Hal_IntIsGlobal2LevelSwitchSupported, (void))
{
  /* #10 Return 0 because the platform does not support interrupt global flag. */
  return 0u;
}

/***********************************************************************************************************************
 *  Os_Hal_IntTrapUpdate()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,                                                             /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_ALWAYS_INLINE, Os_Hal_IntTrapUpdate,
(
  P2VAR(Os_Hal_IntTrapStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  /* #10 Copy interrupt level from current BASEPRI to given interrupt state variable. */
  *InterruptState = Os_Hal_INTC_GetCurrentLevel();                                                                      /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_IntGetInterruptState()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,                                                             /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_ALWAYS_INLINE, Os_Hal_IntGetInterruptState,
(
  P2VAR(Os_Hal_IntStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  /* #10 Store interrupt level into given interrupt state. */
  InterruptState->InterruptLevel = Os_Hal_INTC_GetCurrentLevel();                                                       /* SBSW_OS_HAL_PWA_CALLER */
}

/***********************************************************************************************************************
 *  Os_Hal_IntIsPeripheralInterruptTriggerSupported()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsPeripheralInterruptTriggerSupported, (void))                                                                /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
{
  return OS_HAL_INTC_PERIPHERAL_TRIGGER_SUPPORTED;
}


/***********************************************************************************************************************
 *  Os_Hal_IntIsLevelSupported()
 **********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsLevelSupported, (void))
{
  /* #10 Return !0 because the level triggered interrupts are supported. */
  return !0u;                                                                                                           /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}

/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptMappingSupported()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsInterruptMappingSupported, ( void ))
{
  /* #10 Delegate the decision to the interrupt controller specific function. */
  return Os_Hal_INTC_IntIsInterruptMappingSupported();
}


/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptPostActionSupported()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsInterruptPostActionSupported, ( void ))
{
  /* #10 Return 0 as the hardware does not support interrupt post action handling. */
  return 0u;
}


/***********************************************************************************************************************
 *  Os_Hal_IntPostAction()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* COV_OS_HALPLATFORMISRPOSTACTIONAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntPostAction,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
))                                                                                                                      /* COV_OS_HALPLATFORMISRPOSTACTIONAVAILABILITY */
{
  /* #10 Nothing to do on this hardware. */
  OS_IGNORE_UNREF_PARAM(IsrConfig);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_IntPostActionMapped()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* COV_OS_HALPLATFORMISRPOSTACTIONAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntPostActionMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
))                                                                                                                      /* COV_OS_HALPLATFORMISRPOSTACTIONAVAILABILITY */
{
  /* #10 Nothing to do on this hardware. */
  OS_IGNORE_UNREF_PARAM(IsrConfig);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(IsrMapConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* OS_HAL_INTERRUPT_H */

/*!
 * \}
 */

/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Interrupt.h
 *********************************************************************************************************************/
