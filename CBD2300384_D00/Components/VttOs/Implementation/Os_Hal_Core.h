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
 * \file    Os_Hal_Core.h
 * \brief   Core and multi-core related primitives.
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388  EOF */ /* MD_MSR_Dir1.1 */
#ifndef OS_HAL_CORE_H
# define OS_HAL_CORE_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Hal_CoreInt.h"
# include "Os_Hal_Core_Lcfg.h"

/* Os kernel module declarations */
# include "Os_Common.h"
# include "Os_Thread.h"
# include "Os_Core.h"
# include "Os_Lcfg.h"

/* Os HAL dependencies */
# include "Os_Hal_Compiler.h"
# include "Os_Hal_Vtt_LCfg.h"


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

extern P2VAR(Os_Hal_ContextType, AUTOMATIC, AUTOMATIC) Os_Hal_CoreId2CurrentContext[OS_CFG_COREPHYSICALID_COUNT];

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Os_Hal_CoreGetId()
 **********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_CorePhysicalIdType, OS_CODE, OS_ALWAYS_INLINE,                          /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreGetId,
(
  void
))
{
  /* #10 Return the simulated CoreId. */
  return (Os_CorePhysicalIdType)CANoeEmuProcessor_GetCoreNumber();
}

/***********************************************************************************************************************
 *  Os_Hal_CoreIsThreadPointerUserReadable()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreIsThreadPointerUserReadable,
(
  void
))
{
  /* #10 Thread pointer can be read in user mode -> return !0. */
  return (Os_StdReturnType)(Os_Hal_IsFeatureActive(OS_HAL_TST_THREADPOINTERUSERREADABLE));                              /* PRQA S 2995, 4304, 4558 */ /* MD_Os_Rule2.2_2995, MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}

/***********************************************************************************************************************
 *  Os_Hal_CoreIsCoreIdUserReadable()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreIsCoreIdUserReadable,
(
  void
))
{
  /* #10 CoreId can be read in user mode -> return !0. */
  return (Os_StdReturnType)(Os_Hal_IsFeatureActive(OS_HAL_TST_COREIDUSERREADABLE));                                     /* PRQA S 2995, 4304, 4558 */ /* MD_Os_Rule2.2_2995, MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}


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
  Os_CorePhysicalIdType coreId = Os_Hal_CoreGetId();

  /* #10 Store the current thread pointer on the local core. */
  Core2Thread->Core2Thread[coreId]->Thread = Thread;                                                                    /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_CoreGetThread()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
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
  Os_CorePhysicalIdType coreId = Os_Hal_CoreGetId();

  OS_IGNORE_UNREF_PARAM(Core2Thread);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* #10 Returns the current thread pointer of the local core. */
  return (Core2Thread->Core2Thread[coreId]->Thread);
}


/***********************************************************************************************************************
 *  Os_Hal_CoreIsPrivilegedModeUserReadable()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreIsPrivilegedModeUserReadable,
(
  void
))
{
  return (Os_StdReturnType)(Os_Hal_IsFeatureActive(OS_HAL_TST_PRIVILEGEDMODEUSERREADABLE));                             /* PRQA S 2995, 4304, 4558 */ /* MD_Os_Rule2.2_2995, MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}


/***********************************************************************************************************************
 *  Os_Hal_CoreGetPrivilegedMode()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreGetPrivilegedMode,
(
  void
))
{
  /* Return User-Mode */
  return (Os_StdReturnType)(0u);                                                                                        /* PRQA S 4304, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
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
  uint8 result;
  uint8 BitFieldWidth = (sizeof(Os_Hal_BitFieldType) * 8u) ;

  uint8 indexOffset;
  Os_Hal_BitFieldType index;
  static CONST(uint8, OS_CONST) countLeadingZeros4BitMap[16]=
  {
     4,     /* 0000 */
     3,     /* 0001 */
     2,     /* 0010 */
     2,     /* 0011 */
     1,     /* 0100 */
     1,     /* 0101 */
     1,     /* 0110 */
     1,     /* 0111 */
     0,     /* 1000 */
     0,     /* 1001 */
     0,     /* 1010 */
     0,     /* 1011 */
     0,     /* 1100 */
     0,     /* 1101 */
     0,     /* 1110 */
     0      /* 1111 */
  };

  index = BitField;
  indexOffset = 0;

  if(BitFieldWidth >= 32u)                                                                                              /* PRQA S 2991, 2995 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule2.2_2995 */
  {
    if((index & 0xFFFF0000u) != 0u)
    {
      index >>= (Os_Hal_BitFieldType)16;
    }
    else
    {
      indexOffset += (uint8)16;
    }
  }

  if(BitFieldWidth >= 16u)                                                                                              /* PRQA S 2991, 2995 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule2.2_2995 */
  {
    if((index & 0xFF00u) != 0u)
    {
      index >>= (Os_Hal_BitFieldType)8;
    }
    else
    {
      indexOffset += (uint8)8;
    }
  }

  if((index & 0xF0u) != 0u)
  {
    index >>= (Os_Hal_BitFieldType)4;
  }
  else
  {
    indexOffset += (uint8)4;
  }

  result = indexOffset + countLeadingZeros4BitMap[index];

  return result;
}



/***********************************************************************************************************************
 *  Os_Hal_StackEndGet()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE,                /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_MSR_Rule8.13 */
OS_ALWAYS_INLINE, Os_Hal_StackEndGet,
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
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_MSR_Rule8.13 */
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
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_StackTopGet,
(
  void
))
{
  return (Os_Hal_StackType*)0u;
}


/***********************************************************************************************************************
 *  Os_Hal_StackPointerMoveDown()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_MSR_Rule8.13 */
OS_LOCAL_INLINE P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_StackPointerMoveDown,
(
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) StackPointer
))
{
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) stackPointer = StackPointer + 1u;
  return stackPointer;
}


/***********************************************************************************************************************
 *  Os_Hal_StackCalculateUsage()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE uint32, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_StackCalculateUsage,
(
  P2CONST(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) StackBottom,
  P2CONST(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) StackTop
))
{
  return (uint32)(StackBottom - StackTop);                                                                              /* PRQA S 0488 */ /* MD_Os_Hal_Rule18.4_0488 */
}

/***********************************************************************************************************************
 *  Os_Hal_CoreStart()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreStart,
(
  P2CONST(Os_Hal_CoreConfigType, AUTOMATIC, OS_CONST) Core
))
{
  /* #10 Set start address. */
  CANoeEmuProcessor_SetStartAddr((sint32)Core->CANoeEmuCoreId, (Os_Hal_CoreEntryFunc)CORE_START_FUNC);                  /* SBSW_OS_HAL_FC_MIXED_GENDATA_VAR */


  /* #20 Start core. */
  CANoeEmuProcessor_StartCore((sint32)Core->CANoeEmuCoreId);
}


/***********************************************************************************************************************
 *  Os_Hal_SpinInit()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_SpinInit,
(
  P2VAR(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
))
{
  /* #10 Set Spinlock to unlocked. */
  *Spinlock = OS_HAL_SPINLOCKUNLOCKED;                                                                                  /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_SpinIsLocked()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
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
  /* #10 Prevent CANoe from detecting an infinite loop. */
  CANoeAPI_ConsumeTicks(1);

  /* #20 Return 0 if Spinlock is unlocked, !0 otherwise. */
  return (Os_StdReturnType)(*Spinlock != OS_HAL_SPINLOCKUNLOCKED);                                                      /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */
}


/***********************************************************************************************************************
 *  Os_Hal_SpinTryLock()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_MSR_Rule8.13 */
OS_LOCAL_INLINE Os_Hal_SpinlockResultType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_SpinTryLock,
(
  P2VAR(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
))
{
  /* #10 Prevent CANoe from detecting an infinite loop. */
  CANoeAPI_ConsumeTicks(1);

  /* #20 Try to get the Spinlock. */
  return CANoeEmuProcessor_InterlockedExchange(Spinlock, OS_HAL_SPINLOCKLOCKED);                                        /* SBSW_OS_HAL_FC_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_SpinUnlock()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_SpinUnlock,
(
  P2VAR(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
))
{
  /* #10 Set Spinlock to unlocked. */
  *Spinlock = OS_HAL_SPINLOCKUNLOCKED;                                                                                  /* SBSW_OS_HAL_PWA_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_XSigInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_XSigInit,
(
  P2CONST(Os_Hal_XSigInterruptConfigType, AUTOMATIC, OS_CONST) XSigConfig
))
{
  /* #10 Unmask the x signal interrupt source in CANoe */
  (void)CANoeEmuProcessor_UnmaskInterrupt(XSigConfig->InterruptSource);
}


/***********************************************************************************************************************
 *  Os_Hal_XSigTrigger()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_XSigTrigger,
(
  P2CONST(Os_Hal_XSigInterruptConfigType, AUTOMATIC, OS_CONST) XSigConfig
))
{
  /* #10 Trigger XSignal interrupt. */
  CANoeEmuProcessor_RequestInterruptOnCore((sint32)XSigConfig->DestinationCore, XSigConfig->InterruptSource);
}


/***********************************************************************************************************************
 *  Os_Hal_XSigAcknowledge()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_XSigAcknowledge,
(
  P2CONST(Os_Hal_XSigInterruptConfigType, AUTOMATIC, OS_CONST) XSigConfig
))
{
  /* #10 Do nothing. */
  OS_IGNORE_UNREF_PARAM(XSigConfig);                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
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
  /* #10 Run idle task. */
  CANoeEmuProcessor_SetTaskToRun(CANOEEMUPROCESSOR_IDLE);
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
  /* #10 Perform NOP using CANoeAPI. */
  CANoeAPI_ConsumeTicks(1);
}


/***********************************************************************************************************************
 *  Os_Hal_CoreGetContext()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE P2VAR(Os_Hal_ContextType, AUTOMATIC, AUTOMATIC), OS_CODE,                  /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_ALWAYS_INLINE, Os_Hal_CoreGetContext, (void))
{
  /* #10 Return the current context pointer of the local core. */
  return Os_Hal_CoreId2CurrentContext[Os_Hal_CoreGetId()];
}


/***********************************************************************************************************************
 *  Os_Hal_CoreSetContext()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreSetContext,
(
  P2CONST(Os_Hal_ContextType, AUTOMATIC, AUTOMATIC) Context
))
{
  /* #10 Set the current context pointer of the local core. */
  Os_Hal_CoreId2CurrentContext[Os_Hal_CoreGetId()] = (P2VAR(Os_Hal_ContextType, AUTOMATIC, AUTOMATIC))Context;          /* PRQA S 0311 */ /* MD_Os_Hal_Rule11.8_0311 */ /* SBSW_OS_HAL_AWA_COREGETID */
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

