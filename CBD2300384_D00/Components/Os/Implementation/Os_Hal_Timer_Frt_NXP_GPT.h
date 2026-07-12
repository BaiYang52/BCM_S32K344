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
 *  \brief        This file contains hardware specific definition of the NXP GTM timer module.
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

#ifndef OS_HAL_TIMER_FRT_NXP_GPT_H
# define OS_HAL_TIMER_FRT_NXP_GPT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/* Os HAL dependencies */
# include "Os_Hal_Timer_NXP_GPTInt.h"
# include "Os_Hal_Interrupt.h"

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! High resolution timer configuration information for NXP_GPT timer module. */
struct Os_Hal_TimerFrtConfigType_Tag
{
  /*! The timer base address. */
  uint32 TimerBaseAddr;
  /*! The timer compare value. */
  uint32 CompareValue;
  /*! The timer interrupt source. */
  uint32 InterruptSource;
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
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerFrtInit,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))
{
  Os_Hal_TimerNxpGptRefType TimerRef = (Os_Hal_TimerNxpGptRefType)(TimerConfig->TimerBaseAddr);                         /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */

  /* #01 Disable GPT */
  TimerRef->TimerCR &= (uint32)(~OS_HAL_TIMER_NXP_GPT_REG_CR_BIT_EN_MASK);                                              /* SBSW_OS_HAL_FRT_NXP_GPT_REG_ACCESS */

  /* #02 Disable GPT interrupts */
  TimerRef->TimerIR = OS_HAL_TIMER_NXP_GPT_INTERRUPT_DISABLE;                                                           /* SBSW_OS_HAL_FRT_NXP_GPT_REG_ACCESS */
  
  /* #03 Configure Free-Run operating mode */
  TimerRef->TimerCR |= OS_HAL_TIMER_NXP_GPT_REG_CR_BIT_FRR_MASK;                                                        /* SBSW_OS_HAL_FRT_NXP_GPT_REG_ACCESS */

  /* #04 Write the ENMOD bit to reset the counter value to 0 once it's enabled */
  TimerRef->TimerCR |= OS_HAL_TIMER_NXP_GPT_REG_CR_BIT_ENMOD_MASK;                                                      /* SBSW_OS_HAL_FRT_NXP_GPT_REG_ACCESS */

  /* #05 Configure GPT counter to be disabled in Debug, Wait, Doze, Stop modes */
  TimerRef->TimerCR &= (uint32)(~(OS_HAL_TIMER_NXP_GPT_REG_CR_BIT_DBGEN_MASK |
                                  OS_HAL_TIMER_NXP_GPT_REG_CR_BIT_WAITEN_MASK |
                                  OS_HAL_TIMER_NXP_GPT_REG_CR_BIT_DOZEEN_MASK |
                                  OS_HAL_TIMER_NXP_GPT_REG_CR_BIT_STOPEN_MASK));                                        /* SBSW_OS_HAL_FRT_NXP_GPT_REG_ACCESS */

  /* #06 Disable GPT Capture feature */
  TimerRef->TimerCR &= (uint32)(~(OS_HAL_TIMER_NXP_GPT_REG_CR_FIELD_IM1_MASK |
                                  OS_HAL_TIMER_NXP_GPT_REG_CR_FIELD_IM2_MASK));                                         /* SBSW_OS_HAL_FRT_NXP_GPT_REG_ACCESS */

  /* #07 Disconnect output pins */
  TimerRef->TimerCR &= (uint32)(~(OS_HAL_TIMER_NXP_GPT_REG_CR_FIELD_OM1_MASK |
                                  OS_HAL_TIMER_NXP_GPT_REG_CR_FIELD_OM2_MASK |
                                  OS_HAL_TIMER_NXP_GPT_REG_CR_FIELD_OM3_MASK));                                         /* SBSW_OS_HAL_FRT_NXP_GPT_REG_ACCESS */

  /* #08 Clear GPT status register */
  TimerRef->TimerSR = OS_HAL_TIMER_NXP_GPT_CLEAR_STATUS_REG;                                                            /* SBSW_OS_HAL_FRT_NXP_GPT_REG_ACCESS */

  /* #09 Set compare value */
  TimerRef->TimerOCR1 = TimerConfig->CompareValue;                                                                      /* SBSW_OS_HAL_FRT_NXP_GPT_REG_ACCESS */
  
  /* #10 Enable Output Compare 1 Interrupt. [ Note: Only channel 1 is used ] */
  TimerRef->TimerIR = OS_HAL_TIMER_NXP_GPT_REG_IR_BIT_OF1IE_MASK;                                                       /* SBSW_OS_HAL_FRT_NXP_GPT_REG_ACCESS */

  /* #11 Enable interrupt source in the NVIC Interrupt Controller */
  Os_Hal_INTC_EnableNotMappedSource(TimerConfig->InterruptSource);

  /* #12 Start the timer by enabling it */
  TimerRef->TimerCR |= OS_HAL_TIMER_NXP_GPT_REG_CR_BIT_EN_MASK;                                                         /* SBSW_OS_HAL_FRT_NXP_GPT_REG_ACCESS */
}


/***********************************************************************************************************************
 *  Os_Hal_TimerFrtGetCounterValue()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
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
  /* #10 Return counter value. */
  return ((Os_Hal_TimerNxpGptRefType)TimerConfig->TimerBaseAddr)->TimerCNT;                                             /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
}


/***********************************************************************************************************************
 *  Os_Hal_TimerFrtGetCompareValue()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_Hal_TimerFrtTickType, OS_CODE, OS_ALWAYS_INLINE,                        /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerFrtGetCompareValue,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))
{
  /* #10 Return value of output compare register. */
  return ((Os_Hal_TimerNxpGptRefType)TimerConfig->TimerBaseAddr)->TimerOCR1;                                            /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
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
  /* #10 Set new compare value. */
  ((Os_Hal_TimerNxpGptRefType)TimerConfig->TimerBaseAddr)->TimerOCR1 = ExpirationTime;                                 /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_FRT_NXP_GPT_REG_ACCESS */
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
  /* #10 Clear Output Compare 1 event status flag by writing 1 to OF1 field */
  ((Os_Hal_TimerNxpGptRefType)TimerConfig->TimerBaseAddr)->TimerSR = OS_HAL_TIMER_NXP_GPT_REG_SR_BIT_OF1_MASK;         /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_FRT_NXP_GPT_REG_ACCESS */
}


/***********************************************************************************************************************
 *  Os_Hal_TimerFrtTriggerIrq()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerFrtTriggerIrq,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))
{
  /* #10 Trigger Irq. */
  Os_Hal_INTC_TriggerSoftwareInterrupt(TimerConfig->InterruptSource);
}

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* OS_HAL_TIMER_FRT_NXP_GPT_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Timer_Frt_NXP_GPT.h
 **********************************************************************************************************************/

