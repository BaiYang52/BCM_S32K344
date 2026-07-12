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
 *  \brief      S32X specific primitives.
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
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                       /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */

#ifndef OS_HAL_CORE_S32X_H
# define OS_HAL_CORE_S32X_H

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

/*! Processor X Number Register (CPUXNUM). */
# define OS_HAL_MSCM_CPUXNUM                         (*(volatile uint32*) (OS_HAL_INTC_MSCM_BASE + 0x04uL))
/*! Control Key Register.  */
# define OS_HAL_MC_ME_CTL_KEY                        (*(volatile uint32*) (0x40088000uL))

/*! XRDC Hardware Configuration 1 register. */
# define OS_HAL_XRDC_HWCFG1                          (*(volatile uint32*) (0x401A40F4uL))
/*! System SEMA42 lock value to get the spinlock. */
# define OS_HAL_SEMA42_LOCK_VALUE                    ((uint8)(OS_HAL_XRDC_HWCFG1) + 1u)
/*! System SEMA42 unlock value to release the spinlock. */
# define OS_HAL_SEMA42_UNLOCK_VALUE                  (uint8)0x0u
/*! System SEMA42 first reset half word (RSTGT[TSTGDP]). */
# define OS_HAL_SEMA42_RSTGT_RSTGDP_FIRST_VALUE      (uint16)0xE200u
/*! System SEMA42 second reset half word (RSTGT[TSTGDP]), and reset all gates. */
# define OS_HAL_SEMA42_RSTGT_RSTGDP_SECOND_VALUE     (uint16)0x1DFFu

/*! Mode entry partition 0 core x base address. */
#define OS_HAL_MC_ME_PRTN0_CORE_BASE(Id)     (OS_HAL_MC_ME_PRTN0_CORE0_BASE + (Id * 0x20u))
/*! Partition 0 core x process configuration register (PRTN0_COREx_PCONF). */
#define OS_HAL_MC_ME_PRTN0_CORE_PCONF(Id)    (*(volatile uint32*) (OS_HAL_MC_ME_PRTN0_CORE_BASE(Id)))
/*! Partition 0 core x process update register (PRTN0_COREx_PUPD). */
#define OS_HAL_MC_ME_PRTN0_CORE_PUPD(Id)     (*(volatile uint32*) (OS_HAL_MC_ME_PRTN0_CORE_BASE(Id) + 0x04uL))
/*! Partition 0 core x status register (PRTN0_COREx_STAT). */
#define OS_HAL_MC_ME_PRTN0_CORE_STAT(Id)     (*(volatile uint32*) (OS_HAL_MC_ME_PRTN0_CORE_BASE(Id) + 0x08uL))
/*! Partition 0 core x address register (PRTN0_COREx_ADDR). */
#define OS_HAL_MC_ME_PRTN0_CORE_ADDR(Id)     (*(volatile uint32*) (OS_HAL_MC_ME_PRTN0_CORE_BASE(Id) + 0x0CuL))

/*! System SEMA42 base address. */
# define OS_HAL_SEMA42_BASE        (0x40298000uL)
/*! First System SEMA42 gate register (SEMA42_GATE). */
# define OS_HAL_SEMA42_GATE        (*(volatile uint8*) (OS_HAL_SEMA42_BASE))
/*! System SEMA42 reset gate register (SEMA42_RSTGT). */
# define OS_HAL_SEMA42_RSTGT       (*(volatile uint16*) (OS_HAL_SEMA42_BASE + 0x42uL))

/*! Interrupt Router CPa Interrupt 0 Status Register (IRCPaISR0). */
# define OS_HAL_MSCM_IRCPSR(a)     (*(volatile uint32*) (OS_HAL_INTC_MSCM_BASE + (OS_HAL_MSCM_IRCPSR_BASE_OFFSET) + \
                                                        (OS_HAL_MSCM_IRCPSR_CORE_OFFSET * (a + 4u))))
/*! Interrupt Router CPa Interrupt 0 Generation Register (IRCPaIGR0). */
# define OS_HAL_MSCM_IRCPGR(a)     (*(volatile uint32*) (OS_HAL_INTC_MSCM_BASE + (OS_HAL_MSCM_IRCPGR_BASE_OFFSET) + \
                                                        (OS_HAL_MSCM_IRCPSR_CORE_OFFSET * (a + 4u))))

/*! Mask used to trigger a core-to-core interrupt in IRCPIGR register. */
# define OS_HAL_MSCM_IRCPGR_TRIGGER_MASK             (0x1uL)


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! XSignal interrupt configuration. */
struct Os_Hal_XSigInterruptConfigType_Tag
{
  Os_Hal_IntSourceType Source;                                   /*!< Interrupt Source. */
  uint32 TargetCoreId;                                           /*!< Target physical core id. */
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
  /* #10 Return readable in user mode. */
  return !0u;                                                                                                           /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
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
  /* #10 Return readable in user mode. */
  return !0u;                                                                                                           /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
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
  /* #10 Return logical core ID. */
  return (Os_CorePhysicalIdType)(OS_HAL_MSCM_CPUXNUM & 0x03uL);                                                         /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
}

 /***********************************************************************************************************************
  *  Os_Hal_SpinInit()
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
 Os_Hal_SpinInit,
 (
   P2VAR(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
 ))
 {
   /* #10 Perform first part of gate reset. */
   OS_HAL_SEMA42_RSTGT = OS_HAL_SEMA42_RSTGT_RSTGDP_FIRST_VALUE;                                                         /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_SPINLOCK_SEMA42_RSTGT */

   /* #20 Perform second part of gate reset. */
   OS_HAL_SEMA42_RSTGT = OS_HAL_SEMA42_RSTGT_RSTGDP_SECOND_VALUE;                                                        /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_SPINLOCK_SEMA42_RSTGT */

   /* #30 Set given spinlock to 0. */
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
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_Os_Hal_Rule8.13_3673_NotUsed */
OS_LOCAL_INLINE Os_Hal_SpinlockResultType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_SpinTryLock,
(
  P2VAR(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
))
{
  Os_Hal_SpinlockResultType result;                                                                                     /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
  uint32 interruptLevel;

  /* #10 Disable global interrupt handling and store prior state. */
  interruptLevel = Os_Hal_DisableAndReturnPriorState();

  /* #20 Wait until hardware spinlock is free and than lock it.  */
  do                                                                                                                    /* FETA_OS_HAL_SPINLOCK_AQUIRE */
  {
      OS_HAL_SEMA42_GATE = (uint8)OS_HAL_SEMA42_LOCK_VALUE;                                                             /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_SPINLOCK_SEMA42_GATE */
  }
  while(OS_HAL_SEMA42_GATE != (uint8)OS_HAL_SEMA42_LOCK_VALUE);                                                         /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* COV_OS_HALPLATFORMSPINLOCKTRYTOLOCKCONDITION */

  /* #30 If the spinlock variable is already locked. */
  if(*Spinlock == OS_HAL_SPINLOCK_LOCKED)
  {
    /* #40 Set the result value to not succeeded. */
    result = !(OS_HAL_SPINLOCKSUCCEEDED);                                                                               /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
  }
  /* #50 Otherwise (spinlock variable is unlocked). */
  else
  {
    /* #60 Lock the spinlock variable. */
    *Spinlock = OS_HAL_SPINLOCK_LOCKED;                                                                                 /* SBSW_OS_HAL_PWA_CALLER */
    /* #70 Set the result value to succeeded. */
    result = OS_HAL_SPINLOCKSUCCEEDED;
  }

  /* #80 Wait till the memory write operation has finished. */
  Os_Hal_DataSyncBarrier();

  /* #90 Release the hardware spinlock. */
  OS_HAL_SEMA42_GATE = OS_HAL_SEMA42_UNLOCK_VALUE;                                                                      /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_SPINLOCK_SEMA42_GATE */

  /* #100 Restore previous interrupt state. */
  Os_Hal_WriteBasepri(interruptLevel);

  /* #110 Return the result value. */
  return result;
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
  /* #10 Enable the corresponding interrupt source. */
  Os_Hal_INTC_EnableNotMappedSource(XSigConfig->Source);
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
  /* #10 Trigger interrupt in the IRCPGR specific register. */
  OS_HAL_MSCM_IRCPGR(XSigConfig->TargetCoreId) |= (uint32)OS_HAL_MSCM_IRCPGR_TRIGGER_MASK;                              /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_XSIGNAL_MSCM_IRCPGR */
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
  /* #10 Acknowledge interrupt in the IRCPSR specific register. */
  OS_HAL_MSCM_IRCPSR(XSigConfig->TargetCoreId) = (uint32)OS_HAL_MSCM_IRCPSR_ACKNOWLEDGE_MASK;                           /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_XSIGNAL_MSCM_IRCPSR */
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
  /* #10 Offset used to map each core to its peripheral reset (PERIPH_n_RST, n = 0,1,2,6 for Cortex-M7 0,1,2,3). */
  uint32 coreIdLoc = (((Core->CoreId) ^ 0x03uL) == 0x00uL) ? 0x03uL : 0x00uL;                                           /* COV_OS_HALPLATFORMS32XPERIPHERAL_RESET_XF */

  /* #20 Set the core boot address and enable the clock. */
  OS_HAL_MC_ME_PRTN0_CORE_ADDR(Core->CoreId) = (Core->CoreStartAddress);                                                /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_CORECONFIG_MC_ME_PRTN0_COREX_ADDR */
  OS_HAL_MC_ME_PRTN0_CORE_PCONF(Core->CoreId) |= 1u;                                                                    /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_CORECONFIG_MC_ME_PRTN0_COREX_PCONF */
  OS_HAL_MC_ME_PRTN0_CORE_PUPD(Core->CoreId) |= 1u;                                                                     /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_CORECONFIG_MC_ME_PRTN0_COREX_PUPD */

  /* #30 Start the hardware prosess for core x. */
  OS_HAL_MC_ME_CTL_KEY = (uint32)0x5AF0u;                                                                               /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_CORECONFIG_MC_ME_CTL_KEY */
  OS_HAL_MC_ME_CTL_KEY = (uint32)0xA50Fu;                                                                               /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_CORECONFIG_MC_ME_CTL_KEY */

  /* #40 Wait Until clock is enabled. */
  while ((OS_HAL_MC_ME_PRTN0_CORE_STAT(Core->CoreId) & (uint32)0x01u) != 1u)                                            /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* COV_OS_HALPLATFORMINVSTATE */ /* FETA_OS_HAL_MCME_CLK */
  {
    /* Do nothing. */
  }

  /* #50 Release the core reset. */
  *(volatile uint32*)(OS_HAL_RGM_PRST_BASE) &= ~(uint32)(0x1uL << (coreIdLoc + (Core->CoreId)));                        /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_CORECONFIG_MC_RGM_PRST_BASE */

  /* #60 Wait until reset has been deasserted. */
  while (((*(volatile uint32*)(OS_HAL_RGM_PSTAT_BASE)) & (0x1uL << (coreIdLoc + (Core->CoreId)))) == 1u)                /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* COV_OS_HALPLATFORMINVSTATE */ /* FETA_OS_HAL_RGM */
  {
    /* Do nothing. */
  }
}

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* OS_HAL_CORE_S32X_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Core_S32x.h
 **********************************************************************************************************************/

