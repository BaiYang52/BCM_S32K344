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
 *  \addtogroup  Os_Hal_Timer
 *  \{
 *
 *  \file
 *  \brief        Primitives to work with Cortex M SysTick timer hardware.
 *  \details
 *  The ARMv7-M architecture provides a system timer (SysTick):
 *  - 24-bit
 *  - Decrementing
 *  - Wrap-on-zero
 *
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

#ifndef OS_HAL_TIMER_PIT_SYSTICK_H
# define OS_HAL_TIMER_PIT_SYSTICK_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
# include "Std_Types.h"
# include "Os_Common.h"
# include "Os_Hal_Timer_Types.h"

# include "Os_Hal_Interrupt.h"


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
/*! SysTick base address. */
# define OS_HAL_TIMER_SYSTICK_BASE                       (0xE000E010uL)

/*! SysTick Control and Status Register (SYST_CSR). */
# define OS_HAL_TIMER_SYSTICK_SYST_CSR                   (*(volatile uint32*)(OS_HAL_TIMER_SYSTICK_BASE + 0x0uL))
/*! SYST_CSR.ENABLE bit. */
# define OS_HAL_TIMER_SYSTICK_SYST_CSR_ENABLE            (1uL << 0)
/*! SYST_CSR.TICKINT bit. */
# define OS_HAL_TIMER_SYSTICK_SYST_CSR_TICKINT           (1uL << 1)
/*! SYST_CSR.CLKSOURCE bit. */
# define OS_HAL_TIMER_SYSTICK_SYST_CSR_CLKSOURCE         (1uL << 2)
/*! SYST_CSR.COUNTFLAG bit. */
# define OS_HAL_TIMER_SYSTICK_SYST_CSR_COUNTFLAG         (1uL << 16)

/*! SysTick Reload Value Register (SYST_RVR). */
# define OS_HAL_TIMER_SYSTICK_SYST_RVR                   (*(volatile uint32*)(OS_HAL_TIMER_SYSTICK_BASE + 0x4uL))

/*! SysTick Current Value Register (SYST_CVR). */
# define OS_HAL_TIMER_SYSTICK_SYST_CVR                   (*(volatile uint32*)(OS_HAL_TIMER_SYSTICK_BASE + 0x8uL))

/*! SysTick Calibration Value Register (SYST_CALIB). */
# define OS_HAL_TIMER_SYSTICK_SYST_CALIB                 (*(volatile uint32*)(OS_HAL_TIMER_SYSTICK_BASE + 0xCuL))

/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
/*! Programmable interval timer configuration information for Systick timer module. */
struct Os_Hal_TimerPitConfigType_Tag
{
  uint32 ReloadValue;      /*!< Reload value. */
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
 *  Os_Hal_TimerPitInit()
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerPitInit,
(
  P2CONST(Os_Hal_TimerPitConfigType, AUTOMATIC, OS_CONST) TimerConfig
))
{
  /* #10 Disable the timer. */
  OS_HAL_TIMER_SYSTICK_SYST_CSR = 0uL;                                                                                  /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TIMER_SYSTICK_SYST_CSR */

  /* #20 Set SYST_RVR to configured reload value. */
  OS_HAL_TIMER_SYSTICK_SYST_RVR = TimerConfig->ReloadValue;                                                             /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TIMER_SYSTICK_SYST_RVR */

  /* #30 Set current counter value (SYST_CVR) to 0. */
  OS_HAL_TIMER_SYSTICK_SYST_CVR = 0uL;                                                                                  /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TIMER_SYSTICK_SYST_CVR */

  /* #40 Configure timer to generate interrupts and use CPU as clock source. */
  /* #50 Enable the timer. */
  OS_HAL_TIMER_SYSTICK_SYST_CSR = OS_HAL_TIMER_SYSTICK_SYST_CSR_ENABLE |                                                /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TIMER_SYSTICK_SYST_CSR */
                                  OS_HAL_TIMER_SYSTICK_SYST_CSR_TICKINT |
                                  OS_HAL_TIMER_SYSTICK_SYST_CSR_CLKSOURCE;
}


/***********************************************************************************************************************
 *  Os_Hal_TimerPitAckAndReload()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerPitAckAndReload,
(
  P2CONST(Os_Hal_TimerPitConfigType, AUTOMATIC, OS_CONST) TimerConfig
))
{
  volatile uint32 forceRead;                                                                                            /* COMP_WARN_OS_READ_IO_REG */ /* COMP_WARN_UNUSED_VARIABLE */

  /* #10 Read SYST_CSR to clear COUNTFLAG. */
  forceRead = OS_HAL_TIMER_SYSTICK_SYST_CSR;                                                                            /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */

  OS_IGNORE_UNREF_PARAM(TimerConfig);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_TimerPitIsPending()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE uint32, OS_CODE, OS_ALWAYS_INLINE,                                         /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerPitIsPending,
(
  P2CONST(Os_Hal_TimerPitConfigType, AUTOMATIC, OS_CONST) TimerConfig
))
{
  uint32 isPending;

  /* #10 Return value of SYST_CSR.COUNTFLAG. */
  isPending = OS_HAL_TIMER_SYSTICK_SYST_CSR & OS_HAL_TIMER_SYSTICK_SYST_CSR_COUNTFLAG;                                  /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */

  OS_IGNORE_UNREF_PARAM(TimerConfig);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return isPending;
}


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* OS_HAL_TIMER_PIT_SYSTICK_H */

/*!
 * \}
 */
/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_HwTimer_Pit_SysTick.h
 *********************************************************************************************************************/
