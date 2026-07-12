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
 *  \brief        HAL interface to work with the Timer/Pulse Width Modulation (TPM) module as a free running timer.
 *  \details
 *  TPM module features:
 *  - 32-bit counter used as a free-running counter.
 *  - Interrupt on Alarm or Counter Overflow.
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
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_TIMER_FRT_TPM_H
# define OS_HAL_TIMER_FRT_TPM_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os HAL dependencies */
# include "Os_Hal_Interrupt.h"

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
/*! Clock mode field of the TPM counter. */
#define OS_HAL_TIMER_TPM_FRT_SC_CMOD_MASK    (0x18uL)

/*! Select TPM counter clock. */
#define OS_HAL_TIMER_TPM_FRT_SC_CMOD_SET     (1uL << 3u)

/*! Prescale factor selection value. */
#define OS_HAL_TIMER_TPM_FRT_SC_PS           (0x7uL)

/*! Initialize the counter register. */
#define OS_HAL_TIMER_TPM_FRT_CNT_INIT        (0x0uL)

/*! Timer overflow interrupt enable bit. */
#define OS_HAL_TIMER_TPM_FRT_SC_TOIE         (1uL << 6u)

/*! Center aligned PWM select bit. */
#define OS_HAL_TIMER_TPM_FRT_SC_CPWMS        (1uL << 5u)

/*! Clear Channel #0 flag bit. */
#define OS_HAL_TIMER_TPM_FRT_C0SC_CHF_CLEAR  (1uL << 7u)

/*! Reset the configuration register. */
#define OS_HAL_TIMER_TPM_FRT_CONF_RESET      (0x0uL)

/*! Modulo value of the TPM counter. */
#define OS_HAL_TIMER_TPM_FRT_MOD_INIT        (0xFFFFFFFFuL)

/*! Match value for the output compare mode. */
#define OS_HAL_TIMER_TPM_FRT_C0V_INIT        (0x0uL)

/*! Quadrature decoder control and status mode field. */
#define OS_HAL_TIMER_TPM_FRT_QDCTRL_QUADEN   (0x1uL)

/*! Channel #0 interrupt enable bit. */
#define OS_HAL_TIMER_TPM_FRT_C0SC_CHIE       (1uL << 6u)

/*! Channel #0 status and control ELSA field. */
#define OS_HAL_TIMER_TPM_FRT_C0SC_ELSA       (1uL << 2u)
/*! Channel #0 status and control ELSB field. */
#define OS_HAL_TIMER_TPM_FRT_C0SC_ELSB       (1uL << 3u)
/*! Channel #0 status and control MSA field. */
#define OS_HAL_TIMER_TPM_FRT_C0SC_MSA        (1uL << 4u)
/*! Channel #0 status and control MSB field. */
#define OS_HAL_TIMER_TPM_FRT_C0SC_MSB        (1uL << 5u)

/*! TPM Timer Status and Control Register. */
#define OS_HAL_TIMER_TPM_FRT_SC              (*(volatile uint32*)(OS_HAL_TIMER_TPM_BASE + 0x10uL))

/*! TPM Timer Counter Register. */
#define OS_HAL_TIMER_TPM_FRT_CNT             (*(volatile uint32*)(OS_HAL_TIMER_TPM_BASE + 0x14uL))

/*! TPM Timer Modulo Register. */
#define OS_HAL_TIMER_TPM_FRT_MOD             (*(volatile uint32*)(OS_HAL_TIMER_TPM_BASE + 0x18uL))

/*! TPM Timer Channel#0 Status and Control Register. */
#define OS_HAL_TIMER_TPM_FRT_C0SC            (*(volatile uint32*)(OS_HAL_TIMER_TPM_BASE + 0x20uL))

/*! TPM Timer Channel#0 Value Register. */
#define OS_HAL_TIMER_TPM_FRT_C0V             (*(volatile uint32*)(OS_HAL_TIMER_TPM_BASE + 0x24uL))

/*! TPM Timer Quadrature Decoder Control and Status Register. */
#define OS_HAL_TIMER_TPM_FRT_QDCTRL          (*(volatile uint32*)(OS_HAL_TIMER_TPM_BASE + 0x80uL))

/*! TPM Timer Configuration Register. */
#define OS_HAL_TIMER_TPM_FRT_CONF            (*(volatile uint32*)(OS_HAL_TIMER_TPM_BASE + 0x84uL))


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

 /*! High resolution timer configuration dummy type */
struct Os_Hal_TimerFrtConfigType_Tag
{
  uint32 InterruptSource;                    /*!< Interrupt source. */
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
 *  Os_Hal_TimerFrtInit()
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
 *
 *
 *
 *
 *
 *
 */

OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerFrtInit,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))
{
  /* #10 Disable the TPM counter. */
  OS_HAL_TIMER_TPM_FRT_SC &= ~(OS_HAL_TIMER_TPM_FRT_SC_CMOD_MASK);                                                      /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_FRT_SC */

  /* #20 Wait till the counter is disabled. */
  while ((uint32)0 != (OS_HAL_TIMER_TPM_FRT_SC & OS_HAL_TIMER_TPM_FRT_SC_CMOD_MASK))                                    /* COV_OS_HALPLATFORMINVSTATE */ /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
  {
    /* Wait till reading zero. */
  }

  /* #30 Set the clock prescale factor to divide by 1. */
  OS_HAL_TIMER_TPM_FRT_SC &= ~(OS_HAL_TIMER_TPM_FRT_SC_PS);                                                             /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_FRT_SC */

  /* #40 Setup the counter configuration. */
  OS_HAL_TIMER_TPM_FRT_CONF = OS_HAL_TIMER_TPM_FRT_CONF_RESET;                                                          /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_FRT_CONF */

  /* #50 Disable the quadrature decoder mode for channel #0. */
  OS_HAL_TIMER_TPM_FRT_QDCTRL &= ~OS_HAL_TIMER_TPM_FRT_QDCTRL_QUADEN;                                                   /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_FRT_QDCTRL */

  /* #60 Disable center aligned PWM. */
  OS_HAL_TIMER_TPM_FRT_SC &= ~OS_HAL_TIMER_TPM_FRT_SC_CPWMS;                                                            /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_FRT_SC */

  /* #70 Disable channel #0 before setting the mode. */
  OS_HAL_TIMER_TPM_FRT_C0SC &= ~(OS_HAL_TIMER_TPM_FRT_C0SC_ELSA | OS_HAL_TIMER_TPM_FRT_C0SC_ELSB                        /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_FRT_C0SC */
    | OS_HAL_TIMER_TPM_FRT_C0SC_MSA | OS_HAL_TIMER_TPM_FRT_C0SC_MSB);

  /* #80 Wait till channel #0 is disabled. */
  while ((uint32)0 != (OS_HAL_TIMER_TPM_FRT_C0SC & (OS_HAL_TIMER_TPM_FRT_C0SC_ELSA | OS_HAL_TIMER_TPM_FRT_C0SC_ELSB     /* COV_OS_HALPLATFORMINVSTATE */ /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
    | OS_HAL_TIMER_TPM_FRT_C0SC_MSA | OS_HAL_TIMER_TPM_FRT_C0SC_MSB)))                                                  /* COV_OS_HALPLATFORMINVSTATE */
  {
    /* Wait. */
  }

  /* #90 Set the output compare mode to toggle output on a match. */
  OS_HAL_TIMER_TPM_FRT_C0SC |= (OS_HAL_TIMER_TPM_FRT_C0SC_ELSA | OS_HAL_TIMER_TPM_FRT_C0SC_MSA);                        /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_FRT_C0SC */

  /* #100 Set the compare value. */
  OS_HAL_TIMER_TPM_FRT_C0V = OS_HAL_TIMER_TPM_FRT_C0V_INIT;                                                             /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_FRT_C0V */

  /* #110 Wait till the output compare is acknowledged. */
  while ((uint32)0 == (OS_HAL_TIMER_TPM_FRT_C0SC & (OS_HAL_TIMER_TPM_FRT_C0SC_ELSA | OS_HAL_TIMER_TPM_FRT_C0SC_MSA)))   /* COV_OS_HALPLATFORMINVSTATE */ /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
  {
    /* Wait. */
  }

  /* #120 Reset the counter register. */
  OS_HAL_TIMER_TPM_FRT_CNT = OS_HAL_TIMER_TPM_FRT_CNT_INIT;                                                             /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_FRT_CNT */

  /* #130 Reset the timer period. */
  OS_HAL_TIMER_TPM_FRT_MOD = OS_HAL_TIMER_TPM_FRT_MOD_INIT;                                                             /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_FRT_MOD */

  /* #140 Disable the timer overflow interrupt. */
  OS_HAL_TIMER_TPM_FRT_SC &= ~OS_HAL_TIMER_TPM_FRT_SC_TOIE;                                                             /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_FRT_SC */

  /* #150 Enable channel #0 timer interrupt. */
  OS_HAL_TIMER_TPM_FRT_C0SC |= OS_HAL_TIMER_TPM_FRT_C0SC_CHIE;                                                          /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_FRT_C0SC */

  /* #160 Enable the interrupt source. */
  Os_Hal_INTC_EnableNotMappedSource(TimerConfig->InterruptSource);

  /* #170 Start the timer. */
  OS_HAL_TIMER_TPM_FRT_SC |= OS_HAL_TIMER_TPM_FRT_SC_CMOD_SET;                                                          /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_FRT_SC */
}


/***********************************************************************************************************************
 *  Os_Hal_TimerFrtGetCounterValue()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_Hal_TimerFrtTickType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_TimerFrtGetCounterValue,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))
{
  OS_IGNORE_UNREF_PARAM(TimerConfig);                                                                                   /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112 */

  /* #10 Perform dsb and isb instructions in order to get actual value of counter register. */
  Os_Hal_InstructionSyncBarrier();
  Os_Hal_DataSyncBarrier();

  /* #20 Return the TPM counter value. */
  return OS_HAL_TIMER_TPM_FRT_CNT;                                                                                      /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
}


/***********************************************************************************************************************
 *  Os_Hal_TimerFrtGetCompareValue()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_Hal_TimerFrtTickType, OS_CODE, OS_ALWAYS_INLINE,                        /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerFrtGetCompareValue,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))
{
  OS_IGNORE_UNREF_PARAM(TimerConfig);                                                                                   /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112 */

  /* #10 Perform dsb and isb instructions in order to get actual value of counter register. */
  Os_Hal_InstructionSyncBarrier();
  Os_Hal_DataSyncBarrier();

  /* #20 Return value of TPM timer channel#0 value register. */
  return OS_HAL_TIMER_TPM_FRT_C0V;                                                                                      /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
}


/***********************************************************************************************************************
 *  Os_Hal_TimerFrtSetCompareValue()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerFrtSetCompareValue,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig,
  Os_Hal_TimerFrtTickType ExpirationTime
))
{
  OS_IGNORE_UNREF_PARAM(TimerConfig);                                                                                   /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112 */

  /* #10 Set the compare value. */
  OS_HAL_TIMER_TPM_FRT_C0V = ExpirationTime;                                                                            /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_FRT_C0V */
}


/***********************************************************************************************************************
 *  Os_Hal_TimerFrtAcknowledge()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerFrtAcknowledge,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))
{
  OS_IGNORE_UNREF_PARAM(TimerConfig);                                                                                   /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112 */

  /* #10 Clear the corresponding interrupt flag bit. */
  OS_HAL_TIMER_TPM_FRT_C0SC |= OS_HAL_TIMER_TPM_FRT_C0SC_CHF_CLEAR;                                                     /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_FRT_C0SC */
}


/***********************************************************************************************************************
 *  Os_Hal_TimerFrtTriggerIrq()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* COV_OS_HALPLATFORMPERIPHERALINTTRIGGERSUPPORTED */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerFrtTriggerIrq,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))                                                                                                                      /* COV_OS_HALPLATFORMPERIPHERALINTTRIGGERSUPPORTED */
{
  /* #10 Stub implementation: Not supported. */
  OS_IGNORE_UNREF_PARAM(TimerConfig);                                                                                   /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112 */
}

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* OS_HAL_TIMER_FRT_TPM_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Timer_Frt_Tpm.h
 **********************************************************************************************************************/
