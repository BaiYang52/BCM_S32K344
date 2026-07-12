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
 *  \file       Os_Hal_Interrupt.h
 *  \brief
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388  EOF */ /* MD_MSR_Dir1.1 */
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
# include "Os_Hal_FeatureInt.h"

/* Os kernel module declarations */
# include "Os_Cfg.h"
# include "Os_TimingProtection_Cfg.h"
# include "Os_Common.h"

/* Os Hal dependencies */
# include "Os_Hal_Compiler.h"
# include "Os_Hal_Core.h"


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
 *  Os_Hal_IntEnableGlobal()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntEnableGlobal, (void))
{
  /* #10 Start atomic operation to avoid suspension. */
  CANoeAPI_AtomicBegin();

  /* #20 Enable global interrupt state via level. */
  (void)CANoeEmuProcessor_SetInterruptLevel(OS_HAL_INTERRUPT_LEVEL_TASK);

  /* #30 End atomic operation. */
  CANoeAPI_AtomicEnd();
}


/***********************************************************************************************************************
 *  Os_Hal_IntDisableGlobal()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntDisableGlobal, (void))
{
  /* #10 Start atomic operation to avoid suspension. */
  CANoeAPI_AtomicBegin();

  /* #20 Disable global interrupt state via level. */
  (void)CANoeEmuProcessor_SetInterruptLevel(OS_CFG_MAX_INT_LEVEL);

  /* #30 End atomic operation. */
  CANoeAPI_AtomicEnd();
}


/***********************************************************************************************************************
 *  Os_Hal_IntDisableAndReturnGlobalState()
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_Hal_IntGlobalStateType, OS_CODE, OS_ALWAYS_INLINE,                      /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntDisableAndReturnGlobalState, (void))
{
  Os_Hal_IntGlobalStateType retState;

  /* #10 Start atomic operation to avoid suspension. */
  CANoeAPI_AtomicBegin();

  /* #20 Get current interrupt level. */
  retState = CANoeEmuProcessor_GetInterruptLevel();

  /* #30 Disable global interrupt state via level. */
  (void)CANoeEmuProcessor_SetInterruptLevel(OS_CFG_MAX_INT_LEVEL);

  /* #40 End atomic operation. */
  CANoeAPI_AtomicEnd();

  return retState;
}


/***********************************************************************************************************************
 *  Os_Hal_IntSetGlobalState()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntSetGlobalState,
(
  P2CONST(Os_Hal_IntGlobalStateType, AUTOMATIC, OS_APPL_DATA) State
))
{
  /* #10 Start atomic operation to avoid suspension. */
  CANoeAPI_AtomicBegin();

  /* #20 Update the interrupt level register. */
  (void)CANoeEmuProcessor_SetInterruptLevel(*State);

  /* #30 End atomic operation. */
  CANoeAPI_AtomicEnd();
}


/***********************************************************************************************************************
 *  Os_Hal_IntGetLevel()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,                                                             /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_ALWAYS_INLINE, Os_Hal_IntGetLevel,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
))
{
  /* #10 Start atomic operation to avoid suspension. */
  CANoeAPI_AtomicBegin();

  /* #20 Read the interrupt level register. */
  *Level = CANoeEmuProcessor_GetInterruptLevel();                                                                       /* SBSW_OS_HAL_PWA_CALLER */

  /* #30 End atomic operation. */
  CANoeAPI_AtomicEnd();
}


/***********************************************************************************************************************
 *  Os_Hal_IntSetLevel()
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
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntSetLevel,
(
  P2CONST(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
))
{
  /* #10 If interrupt levels are supported */
  if (Os_Hal_IntIsLevelSupported() != 0u)                                                                               /* PRQA S 2991, 2995 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule2.2_2995 */
  {
    /* #20 Start atomic operation to avoid suspension. */
    CANoeAPI_AtomicBegin();
    /* #30 Update the interrupt level register. */
    (void)CANoeEmuProcessor_SetInterruptLevel(*Level);

    /* #40 End atomic operation. */
    CANoeAPI_AtomicEnd();
  }
  /* #50 Otherwise */
  else
  {
    /* #60 If given interrupt level is greater than task interrupt level */
    if (*Level > OS_HAL_INTERRUPT_LEVEL_TASK)
    {
      /* #70 Disable global interrupts. */
      Os_Hal_IntDisableGlobal();
    }
    /* #80 Otherwise */
    else
    {
      /* #90 Enable global interrupts. */
      Os_Hal_IntEnableGlobal();
    }
  }
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
))
{
  /* #10 Return Autosar task interrupt level. */
  *State = OS_HAL_INTERRUPT_LEVEL_TASK;                                                                                 /* SBSW_OS_HAL_PWA_CALLER */
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
  /* #10 Return highest Cat2 interrupt level. */
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
  /* #10 Return highest Cat1 interrupt level. */
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
  /* #10 Return the level to disable all timing protection ISRs. */
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntGetTaskLevel,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
))
{
  /* #10 Return task level.  */
  *Level = OS_HAL_INTERRUPT_LEVEL_TASK;                                                                                 /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_IntGetInterruptState()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,                                                             /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_ALWAYS_INLINE, Os_Hal_IntGetInterruptState,
(
  P2VAR(Os_Hal_IntStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  /* #10 Start atomic operation to avoid suspension. */
  CANoeAPI_AtomicBegin();

  /* #20 Read the interrupt level register. */
  *InterruptState = CANoeEmuProcessor_GetInterruptLevel();                                                              /* SBSW_OS_HAL_PWA_CALLER */

  /* #30 End atomic operation. */
  CANoeAPI_AtomicEnd();
}


/***********************************************************************************************************************
 *  Os_Hal_IntDisableSource()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntDisableSource,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
))
{
  /* #10 Mask the passed interrupt source in CANoe. */
  (void)CANoeEmuProcessor_MaskInterrupt(IsrConfig->Source);
}


/***********************************************************************************************************************
 *  Os_Hal_IntDisableSourceMapped()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntDisableSourceMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
))
{
  OS_IGNORE_UNREF_PARAM(IsrMapConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* #10 Mask the passed interrupt source in CANoe. */
  Os_Hal_IntDisableSource(IsrConfig);                                                                                   /* SBSW_OS_HAL_FC_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_IntEnableSource()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntEnableSource,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
))
{
  /* #10 Unmask the passed interrupt source in CANoe */
  (void)CANoeEmuProcessor_UnmaskInterrupt(IsrConfig->Source);
}


/***********************************************************************************************************************
 *  Os_Hal_IntEnableSourceMapped()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntEnableSourceMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
))
{
  OS_IGNORE_UNREF_PARAM(IsrMapConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* #10 Unmask the passed interrupt source in CANoe */
  Os_Hal_IntEnableSource(IsrConfig);                                                                                    /* SBSW_OS_HAL_FC_CALLER */
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
Os_Hal_IntIsClearPendingSupported,(void))
{
  /* #10 Return clear pending interrupts is supported. */
  return (Os_StdReturnType)(Os_Hal_IsFeatureActive(OS_HAL_TST_CLEARPENDINGINTERRUPTSUPPORT));                           /* PRQA S 2995, 4304, 4558 */ /* MD_Os_Rule2.2_2995, MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}


/***********************************************************************************************************************
 *  Os_Hal_IntClearPending()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntClearPending,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
))
{
  /* #10 Clear the passed interrupt source */
  CANoeEmuProcessor_ClearInterrupt(IsrConfig->Source);
}


/***********************************************************************************************************************
 *  Os_Hal_IntClearPendingMapped()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntClearPendingMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
))
{
  OS_IGNORE_UNREF_PARAM(IsrMapConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* #10 Clear the passed interrupt source */
  Os_Hal_IntClearPending(IsrConfig);                                                                                    /* SBSW_OS_HAL_FC_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptSourceEnabled()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsInterruptSourceEnabled,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
))
{
  /* #10 Return the interrupt mask state of the passed source */
  return (Os_StdReturnType)(CANoeEmuProcessor_IsInterruptMasked(IsrConfig->Source) == 0);                               /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */
}


/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptSourceEnabledMapped()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsInterruptSourceEnabledMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
))
{
  OS_IGNORE_UNREF_PARAM(IsrMapConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Os_Hal_IntIsInterruptSourceEnabled(IsrConfig);                                                                 /* SBSW_OS_HAL_FC_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptPending()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsInterruptPending,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
))
{
  /* #10 Return the interrupt pending state of the passed source */
  return (Os_StdReturnType)(CANoeEmuProcessor_IsInterruptRequested(IsrConfig->Source));
}


/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptPendingMapped()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsInterruptPendingMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
))
{
  OS_IGNORE_UNREF_PARAM(IsrMapConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* #10 Return the interrupt pending state of the passed source */
  return Os_Hal_IntIsInterruptPending(IsrConfig);                                                                       /* SBSW_OS_HAL_FC_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_IntEndOfInterrupt()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntEndOfInterrupt,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
))
{
  OS_IGNORE_UNREF_PARAM(IsrConfig);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_IntEndOfInterruptMapped()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntEndOfInterruptMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
))
{
  OS_IGNORE_UNREF_PARAM(IsrConfig);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(IsrMapConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_IntIsrLevelIsLe(()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType,                                                          /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_CODE, OS_ALWAYS_INLINE, Os_Hal_IntIsrLevelIsLe,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntLevelType, AUTOMATIC, OS_CONST) Level
))
{
  return (Os_StdReturnType)(IsrConfig->Level <= (*Level));                                                              /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */
}


/***********************************************************************************************************************
 *  Os_Hal_IntLevelIsGt()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE,                                                 /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_ALWAYS_INLINE, Os_Hal_IntLevelIsGt,
(
  P2CONST(Os_Hal_IntLevelType, AUTOMATIC, OS_CONST) LevelX,
  P2CONST(Os_Hal_IntLevelType, AUTOMATIC, OS_CONST) LevelY
))
{
  return (Os_StdReturnType)((*LevelX) > (*LevelY));                                                                     /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */
}


/***********************************************************************************************************************
 *  Os_Hal_IntTrapRestore()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,                                                             /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_ALWAYS_INLINE, Os_Hal_IntTrapRestore,
(
  P2CONST(Os_Hal_IntTrapStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  OS_IGNORE_UNREF_PARAM(InterruptState);                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_IntTrapUpdate()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,                                                             /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_MSR_Rule8.13 */
OS_ALWAYS_INLINE, Os_Hal_IntTrapUpdate,
(
  P2VAR(Os_Hal_IntTrapStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  OS_IGNORE_UNREF_PARAM(InterruptState);                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_IntInterruptSourceInit()
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,                                                             /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_ALWAYS_INLINE, Os_Hal_IntInterruptSourceInit,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) InterruptSource
))
{
  /* #10 Set the interrupt core binding. */
  CANoeEmuProcessor_SetInterruptRoutingMask(InterruptSource->Source, InterruptSource->RoutingMask);

  /* #20 Configure the interrupt handler. */
  CANoeEmuProcessor_ConfigureISR(InterruptSource->Source,  (void (*)(void))InterruptSource->Handler);                   /* SBSW_OS_HAL_FC_CALLER */

  /* #30 Configure the interrupt level. */
  CANoeEmuProcessor_ConfigureInterruptPriority(InterruptSource->Source, InterruptSource->Level);

  /* #40 Set the CANoe interrupt name for the given source. */
  CANoeEmuProcessor_ConfigureInterruptName(InterruptSource->Source, InterruptSource->Name);                             /* SBSW_OS_HAL_FC_CALLER */

  /* #50 Mask the passed interrupt source in CANoe. */
  (void)CANoeEmuProcessor_MaskInterrupt(InterruptSource->Source);
}


/***********************************************************************************************************************
 *  Os_Hal_IntInterruptSourceInitMapped()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,                                                             /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_ALWAYS_INLINE, Os_Hal_IntInterruptSourceInitMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) InterruptSource,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
))
{
  OS_IGNORE_UNREF_PARAM(IsrMapConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  Os_Hal_IntInterruptSourceInit(InterruptSource);                                                                       /* SBSW_OS_HAL_FC_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_IntIsGlobal2LevelSwitchSupported()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsGlobal2LevelSwitchSupported, (void))
{
  /* #10 Only level lock supported. */
  return (Os_StdReturnType)(Os_Hal_IsFeatureActive(OS_HAL_TST_GLOBAL2LEVELSWITCHSUPPORT));                              /* PRQA S 2996, 4304 */ /* MD_Os_Rule2.2_2996, MD_Os_C90BooleanCompatibility */
}

/***********************************************************************************************************************
 *  Os_Hal_IntIsPeripheralInterruptTriggerSupported()
 **********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsPeripheralInterruptTriggerSupported, (void))
{
  /* #10 Return !0 because the peripheral interrupts can be triggered. */
  return (Os_StdReturnType)(Os_Hal_IsFeatureActive(OS_HAL_TST_PERIPHERALINTERRUPTTRIGGERSUPPORT));                      /* PRQA S 2995, 4304, 4558 */ /* MD_Os_Rule2.2_2995, MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}


/***********************************************************************************************************************
 *  Os_Hal_IntIsLevelSupported()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsLevelSupported, (void))
{
  return (Os_StdReturnType)(Os_Hal_IsFeatureActive(OS_HAL_TST_LEVELSUPPORT));                                           /* PRQA S 2995, 4304, 4558 */ /* MD_Os_Rule2.2_2995, MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}


/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptMappingSupported()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsInterruptMappingSupported, ( void ))
{
  return !0u;                                                                                                           /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}


/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptPostActionSupported()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_IntIsInterruptPostActionSupported, ( void ))
{
  return !0u;                                                                                                           /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}


/***********************************************************************************************************************
 *  Os_Hal_IntPostAction()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
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
