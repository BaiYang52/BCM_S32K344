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
 *  \addtogroup Os_Hal_Core
 *  \{
 *
 *  \file
 *  \brief      CYT4Bx specific primitives.
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
 *
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */

#ifndef OS_HAL_CORE_CYT4Bx_H
# define OS_HAL_CORE_CYT4Bx_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Common.h"

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/*! Mask using to read processor ID. */
# define OS_HAL_CPUSS_IDENTITY_MASK                   (0x1uL)
/*! SUCCESS bit offset inside IPC_STRUCT_ACQUIRE register. */
# define OS_HAL_IPC_STRUCT_ACQUIRE_SUCCESS_BIT        (31uL)
/*! Value using to release the IPC channel. */
# define OS_HAL_IPC_STRUCT_RELEASE_VALUE              (0x0uL)
/* Mask to read core Id bit 0. */
# define OS_HAL_CORE_ID_MASK                          (1uL)
/* Offset value to notify bits inside IPC_INTR_STRUCT_INTR structure. */
# define OS_HAL_IPC_INTR_STRUCT_INTR_NOTIFY_OFFSET    (16uL)
/* Offset value to mask the IPC interrupt mask. */
# define OS_HAL_IPC_INTR_STRUCT_INTR_MASK_OFFSET      (16uL)

/*! Inter-Processor Communication channel 7 base address (reserved for the spinlock). */
# define OS_HAL_IPC_STRUCT_7_BASE                (0x402200E0uL)
/*! IPC channel 7 lock acquire register. */
# define OS_HAL_IPC_STRUCT_7_ACQUIRE             (*(volatile uint32*) (OS_HAL_IPC_STRUCT_7_BASE))
/*! IPC channel 7 lock release register. */
# define OS_HAL_IPC_STRUCT_7_RELEASE             (*(volatile uint32*) (OS_HAL_IPC_STRUCT_7_BASE + 0x04uL))

/*! Inter-Processor Communication channel base address (used for the xsignals). */
# define OS_HAL_IPC_STRUCT_BASE                  (0x40220000uL)
/*! IPC notification register of channel x. */
# define OS_HAL_IPC_STRUCT_NOTIFY(x)             (*(volatile uint32*) (OS_HAL_IPC_STRUCT_BASE + 0x08uL + ((x) * 0x20uL)))

/* IPC interrupt structure base register. */
# define OS_HAL_IPC_INTR_STRUCT_BASE             (0x40221000uL)
/* IPC interrupt status register of interrupt x. */
# define OS_HAL_IPC_INTR_STRUCT_INTR(x)          (*(volatile uint32*) (OS_HAL_IPC_INTR_STRUCT_BASE + ((x) * 0x20uL)))
/* IPC interrupt mask register of interrupt x. */
# define OS_HAL_IPC_INTR_STRUCT_INTR_MASK(x)     (*(volatile uint32*) (OS_HAL_IPC_INTR_STRUCT_BASE + ((x) * 0x20uL) + 0x08uL))

/*! CPU Subsystem identity register (CPUSS_IDENTITY). */
# define OS_HAL_CPUSS_IDENTITY                   (*(volatile uint32*) (0x40200000uL))


 /***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! XSignal interrupt configuration. */
struct Os_Hal_XSigInterruptConfigType_Tag
{
  Os_Hal_IntSourceType CoreInterruptSource;                      /*!< Core interrupt source of XSignal. */
  Os_Hal_IntSourceType SystemInterruptSource;                    /*!< System interrupt source of XSignal. */
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
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

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
  /* #10 Return not readable in user mode. */
  return 0u;                                                                                                           /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}

/***********************************************************************************************************************
 *  Os_Hal_CoreIsCoreIdUserReadable()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* COV_OS_HALPLATFORMCOREIDNOTREADABLE */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreIsCoreIdUserReadable,
(
  void
))                                                                                                                      /* COV_OS_HALPLATFORMCOREIDNOTREADABLE */
{
  /* #10 Return not readable in user mode. */
  return 0u;                                                                                                           /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}

/***********************************************************************************************************************
 *  Os_Hal_CoreGetId()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */

OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_CorePhysicalIdType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_CoreGetId,
(
  void
))
{
  /* #10 Return physical core ID. */
  return (Os_CorePhysicalIdType) ((OS_HAL_CPUSS_IDENTITY >> 8u) & OS_HAL_CPUSS_IDENTITY_MASK);                          /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
}

/***********************************************************************************************************************
 *  Os_Hal_SpinInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_SpinInit,
(
  P2VAR(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
))
{
  /* #10 Set given spinlock to 0. */
  *Spinlock = !OS_HAL_SPINLOCK_LOCKED;                                                                                  /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */ /* SBSW_OS_HAL_PWA_CALLER */
}

/***********************************************************************************************************************
 *  Os_Hal_SpinTryLock()
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
OS_FUNC_ATTRIBUTE_DEFINITION(
OS_LOCAL_INLINE Os_Hal_SpinlockResultType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_SpinTryLock,
(
  P2VAR(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
))
{
  Os_Hal_SpinlockResultType result = !(OS_HAL_SPINLOCKSUCCEEDED);                                                       /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
  uint32 interruptLevel;

  /* #10 Disable global interrupt handling and store prior state. */
  interruptLevel = Os_Hal_DisableAndReturnPriorState();

  /* #20 Check if the IPC channel is locked.  */
  if((OS_HAL_IPC_STRUCT_7_ACQUIRE & (1uL << OS_HAL_IPC_STRUCT_ACQUIRE_SUCCESS_BIT)) != 0u)                              /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
  {
    /* #30 If the spinlock variable is already locked. */
    if(*Spinlock == OS_HAL_SPINLOCK_LOCKED)
    {
      /* #40 Set the result value to not succeeded. */
      result = !(OS_HAL_SPINLOCKSUCCEEDED);                                                                             /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
    }
    /* #50 Otherwise (spinlock variable is unlocked). */
    else
    {
      /* #60 Lock the spinlock variable. */
      *Spinlock = OS_HAL_SPINLOCK_LOCKED;                                                                               /* SBSW_OS_HAL_PWA_CALLER */
      /* #70 Set the result value to succeeded. */
      result = OS_HAL_SPINLOCKSUCCEEDED;
    }

    /* #80 Release the IPC channel. */
    OS_HAL_IPC_STRUCT_7_RELEASE = OS_HAL_IPC_STRUCT_RELEASE_VALUE;                                                      /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_IPC_STRUCT_RELEASE */
  }

  /* #90 Restore previous interrupt state. */
  Os_Hal_WriteBasepri(interruptLevel);

  /* #100 Return the result value. */
  return result;
}

/***********************************************************************************************************************
 *  Os_Hal_XSigInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_XSigInit,
(
  P2CONST(Os_Hal_XSigInterruptConfigType, AUTOMATIC, OS_CONST) XSigConfig
))
{
  const Os_CorePhysicalIdType otherCoreId = ~(Os_Hal_CoreGetId()) & OS_HAL_CORE_ID_MASK;

  /* #10 Enable IPC interrupt mask. */
  OS_HAL_IPC_INTR_STRUCT_INTR_MASK(XSigConfig->SystemInterruptSource) |=                                                /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_IPC_INTR_STRUCT_INTR_MASK */
    (uint32)(1uL << (otherCoreId + OS_HAL_IPC_INTR_STRUCT_INTR_MASK_OFFSET));

  /* #20 Enable the corresponding interrupt source. */
  Os_Hal_INTC_EnableMappedSource(XSigConfig->SystemInterruptSource, XSigConfig->CoreInterruptSource);
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
  const Os_CorePhysicalIdType coreId = Os_Hal_CoreGetId();

  /* #10 Generate a notification event to the IPC interrupt structure. */
  OS_HAL_IPC_STRUCT_NOTIFY(coreId) |= (uint32)(1uL << XSigConfig->SystemInterruptSource);                               /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_IPC_STRUCT_NOTIFY */
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
  const Os_CorePhysicalIdType otherCoreId = ~(Os_Hal_CoreGetId()) & OS_HAL_CORE_ID_MASK;

  /* #10 Clear the interrupt. */
  OS_HAL_IPC_INTR_STRUCT_INTR(XSigConfig->SystemInterruptSource) |=                                                     /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_IPC_INTR_STRUCT_INTR */
    (uint32)(1uL << (otherCoreId + OS_HAL_IPC_INTR_STRUCT_INTR_NOTIFY_OFFSET));

}

 /***********************************************************************************************************************
 *  Os_Hal_CoreStart()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreStart,
(
  P2CONST(Os_Hal_CoreConfigType, AUTOMATIC, OS_CONST) Core
))
{
  /* #10 Stub implementation: Currently not needed. */
  OS_IGNORE_UNREF_PARAM(Core);                                                                                          /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112 */
}

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* OS_HAL_CORE_CYT4Bx_H */

/*!
 * \}
 */
/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Derivative_CYT4BXInt.h
 *********************************************************************************************************************/
