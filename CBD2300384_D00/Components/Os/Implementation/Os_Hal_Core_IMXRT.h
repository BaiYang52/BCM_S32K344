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
 *  \brief      iMXRT specific primitives.
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
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */

#ifndef OS_HAL_CORE_IMXRT_H
# define OS_HAL_CORE_IMXRT_H

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
/*! IOMUX Controller LPSR General Purpose #0 Register. */
# define OS_HAL_IOMUXC_LPSR_GPR_GPR0  (*(volatile uint32*) (OS_HAL_IOMUXC_LPSR_GPR_BASE))
/*! IOMUX Controller LPSR General Purpose #1 Register. */
# define OS_HAL_IOMUXC_LPSR_GPR_GPR1  (*(volatile uint32*) (OS_HAL_IOMUXC_LPSR_GPR_BASE + 0x04uL))
/*! System Reset Controller Control Register. */
# define OS_HAL_SRC_SCR  (*(volatile uint32*) (OS_HAL_SRC_BASE))

/*! Messaging Unit A/B Status Register. */
# define OS_HAL_MU_SR(n)  (*(volatile uint32*) (OS_HAL_CORE0_MU_A_BASE + (0x4000uL * n) + 0x20uL))
/*! Messaging Unit A/B Control Register. */
# define OS_HAL_MU_CR(n)  (*(volatile uint32*) (OS_HAL_CORE0_MU_A_BASE + (0x4000uL * n) + 0x24uL))

/*! Processor B-side Reset State bit. */
# define OS_HAL_MU_A_ASR_BRS  (0x01uL << 7u)
/*! Processor A MU Reset bit. */
# define OS_HAL_MU_A_ACR_MUR  (0x01uL << 5u)
/*! Processor A General Purpose Interrupt 0 Request bit. */
# define OS_HAL_MU_A_ACR_GIR  (0x08uL << 16u)
/*! Processor B General Interrupt 0 Request Pending bit. */
# define OS_HAL_MU_B_BSR_GIP  (0x08uL << 28u)
/*! Processor B General Purpose Interrupt 0 Enable bit. */
# define OS_HAL_MU_B_BCR_GIE  (0x08uL << 28u)

/*! Reset control register BT_RELEASE_M4 bit. */
# define OS_HAL_SRC_BT_RELEASE_M4  (0x01uL)
/*! Mask used to set lower bits of CM4 Vector table. */
# define OS_HAL_IOMUXC_LPSR_GPR_GPR0_CM4_INIT_VTOR_LOW_MASK   (0xFFF8uL)

/*! Arm defined CPU identification register. */
# define OS_HAL_CORTEXM_CPUID  (*(volatile uint32*) (0xE000ED00uL))

/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! XSignal interrupt configuration. */
struct Os_Hal_XSigInterruptConfigType_Tag
{
  Os_Hal_IntSourceType CoreInterruptSource;  /*!< Core interrupt source of XSignal. */
  Os_Hal_IntSourceType InterruptFlagMask;    /*!< Interrupt flag mask. */
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
  /* #10 Return unreadable in user mode. */
  return 0u;                                                                                                            /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
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
  /* #10 Return unreadable in user mode. */
  return 0u;                                                                                                            /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
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

OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_CorePhysicalIdType, OS_CODE, OS_ALWAYS_INLINE,                          /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreGetId,
(
  void
))
{
  /* #10 Return physical core ID. */
  return (Os_CorePhysicalIdType)(0x01uL ^ ((OS_HAL_CORTEXM_CPUID >> 0x04uL) & 0x01uL));                                 /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
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
  /* #10 Set given spinlock to 0. */
  *Spinlock = 0u;                                                                                                       /* SBSW_OS_HAL_PWA_CALLER */
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
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_Hal_SpinlockResultType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_SpinTryLock,
(
  P2VAR(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
))
{
  Os_Hal_SpinlockResultType result = !OS_HAL_SPINLOCKSUCCEEDED;                                                         /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */

  /* #10 If spinlock is free (use load exclusive instruction here): */
  if( !Os_Hal_LoadExclusive(Spinlock) )                                                                                 /* PRQA S 4116, 4558 */ /* MD_Os_Rule10.1_4116, MD_Os_Rule10.1_4558 */ /* SBSW_OS_HAL_FC_CALLER */
  {
    /* #20 Try to get the spinlock (use store exclusive instruction here). */
    result = (Os_Hal_SpinlockResultType)Os_Hal_StoreExclusive(Spinlock, 1);                                             /* SBSW_OS_HAL_FC_CALLER */

    /* #30 If we got the spinlock: */
    if( !result )                                                                                                       /* PRQA S 4116, 4558 */ /* MD_Os_Rule10.1_4116, MD_Os_Rule10.1_4558 */
    {
      /* #40 Memory barrier. */
      Os_Hal_DataMemoryBarrier();

      /* #50 Return succes. */
    }
    /* #60 Otherwise: Return no success. */
  }
  /* #70 Otherwise: Return no success. */

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
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_XSigInit,
(
  P2CONST(Os_Hal_XSigInterruptConfigType, AUTOMATIC, OS_CONST) XSigConfig
))
{
  const Os_CorePhysicalIdType CoreId = Os_Hal_CoreGetId();

  /* #10 Assert Processor A MU reset. */
  OS_HAL_MU_CR(CoreId) |= OS_HAL_MU_A_ACR_MUR;                                                                          /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MU_CR */

  /* #20 Wait as long as Processor B-side of the MU in reset. */
  while((OS_HAL_MU_SR(CoreId) & (uint32)OS_HAL_MU_A_ASR_BRS) != 0uL)                                                    /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* COV_OS_HALPLATFORMINVSTATE */ /* FETA_OS_HAL_XSIGNAL_INIT */
  {
    /* Do nothing. */
  }

  /* #30 Enable Processor B General interrupt 0. */
  OS_HAL_MU_CR(((Os_CorePhysicalIdType)(CoreId ^ 0x01uL))) |= OS_HAL_MU_B_BCR_GIE;                                      /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MU_CR */

  /* #40 Setup the interrupt controller. */
  Os_Hal_INTC_EnableNotMappedSource(XSigConfig->CoreInterruptSource);
}


/***********************************************************************************************************************
 *  Os_Hal_XSigTrigger()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_XSigTrigger,
(
  P2CONST(Os_Hal_XSigInterruptConfigType, AUTOMATIC, OS_CONST) XSigConfig
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  const Os_CorePhysicalIdType CoreId = Os_Hal_CoreGetId();

  /* #10 Processor A requests General Interrupt 0 to Processor B. */
  OS_HAL_MU_CR(CoreId) |= OS_HAL_MU_A_ACR_GIR;                                                                          /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MU_CR */

  OS_IGNORE_UNREF_PARAM(XSigConfig);                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_XSigAcknowledge()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_XSigAcknowledge,
(
  P2CONST(Os_Hal_XSigInterruptConfigType, AUTOMATIC, OS_CONST) XSigConfig
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  const Os_CorePhysicalIdType CoreId = Os_Hal_CoreGetId();

  /* #10 Clear pending general purpose interrupt 0 of Processor B. */
  OS_HAL_MU_SR(CoreId) |= OS_HAL_MU_B_BSR_GIP;                                                                          /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MU_BSR */

  OS_IGNORE_UNREF_PARAM(XSigConfig);                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

/***********************************************************************************************************************
 *  Os_Hal_CoreStart()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
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
  /* #10 Set lower bits of the vector table base address. */
  OS_HAL_IOMUXC_LPSR_GPR_GPR0 = ((uint32)((uint32)(Core->CoreStartAddress))                                             /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_IOMUXC_LPSR_GPR_GPR */
                                 & OS_HAL_IOMUXC_LPSR_GPR_GPR0_CM4_INIT_VTOR_LOW_MASK);

  /* #20 Set higher bits of the vector table base address. */
  OS_HAL_IOMUXC_LPSR_GPR_GPR1 = ((uint32)((Core->CoreStartAddress) >> 16u ));                                           /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_IOMUXC_LPSR_GPR_GPR */

  /* #30 Release Cortex-M4 core reset. */
  OS_HAL_SRC_SCR |= OS_HAL_SRC_BT_RELEASE_M4;                                                                           /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_SRC_SCR */
}

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* OS_HAL_CORE_IMXRT_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Core_IMXRT.h
 **********************************************************************************************************************/

