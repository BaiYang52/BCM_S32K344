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
 * \addtogroup  Os_Hal_Timer
 * \{
 *
 * \file
 * \brief    This file contains hardware specific definition of the FTM module.
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
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_TIMER_FRT_FTM_H
# define OS_HAL_TIMER_FRT_FTM_H

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

/*! Offset of FTM Module Count Regsiter (CNT) */
# define OS_HAL_TIMER_FTM_COUNTREGISTER_OFFSET      (0x04u)

/*! Offset of FTM Channel 0 Regsiter Set (C0SC) */
# define OS_HAL_TIMER_FTM_CHANNEL0_OFFSET           (0x0Cu)
/*! Offset of FTM Channel 2 Regsiter Set (C2SC) */
# define OS_HAL_TIMER_FTM_CHANNEL2_OFFSET           (0x1Cu)
/*! Offset of FTM Channel 4 Regsiter Set (C4SC) */
# define OS_HAL_TIMER_FTM_CHANNEL4_OFFSET           (0x2Cu)
/*! Offset of FTM Channel 6 Regsiter Set (C6SC) */
# define OS_HAL_TIMER_FTM_CHANNEL6_OFFSET           (0x3Cu)

/*! Offset of FTM Channel Compare Value Register (CxV) */
# define OS_HAL_TIMER_FTM_CHANNEL_CMP_OFFSET        (0x04u)

/*! Bit mask for FTM Module Control Register to select external clock. */
# define OS_HAL_TIMER_FTM_SC_CLKS_MASK              (0x00000018uL)

/*! Bit mask for FTM Module Control Register Prescaler. */
# define OS_HAL_TIMER_FTM_SC_PRESCALE_MASK(x)       ((x) & 0x00000007uL)

/*! Value for FTM module initialization (Prescaler = 32). */
# define OS_HAL_TIMER_FTM_SC_VALUE       (OS_HAL_TIMER_FTM_SC_CLKS_MASK | OS_HAL_TIMER_FTM_SC_PRESCALE_MASK(0x05uL))

/*! Bit mask for FTM Channel Control Register to enable the interrupt and set to output compare mode. */
# define OS_HAL_TIMER_FTM_CHANNEL_VALUE             (0x00000050uL)

/*! Bit mask for FTM Channel Control Register to clear an interrupt. */
# define OS_HAL_TIMER_FTM_CHANNEL_CHIE_CLEAR_MASK   (0xFFFFFF7FuL)

/*! Bit mask for FTM Channel Control Register to check if an interrupt is pending. */
# define OS_HAL_TIMER_FTM_CHANNEL_CHF_MASK          (0x00000080uL)


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! High resolution timer configuration information. */
struct Os_Hal_TimerFrtConfigType_Tag
{
  /*! Adress of the corresponding FTM Module Control Register */
  uint32 ModuleControlRegisterAddress;
  /*! Adress of the corresponding FTM Module Count Register */
  uint32 ModuleCountRegisterAddress;
  /*! Adress of the corresponding FTM Channel Control Register */
  uint32 ChannelControlRegisterAddress;
  /*! Adress of the corresponding FTM Channel Compare Register */
  uint32 ChannelCompareRegisterAddress;
  /*! Interrupt Source Number of the corresponding FTM Channel */
  P2CONST(Os_Hal_IntIsrConfigType, OS_CONST, OS_CONST) InterruptConfig;
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
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
Os_Hal_TimerFrtInit,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))                                                                                                                      /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
{
  /* #10 Clear any pending interrupt. */
  Os_Hal_TimerFrtAcknowledge(TimerConfig);                                                                              /* SBSW_OS_HAL_FC_CALLER */

  /* #20 Configure the Counter Control Register. */
  *(volatile uint32*)(TimerConfig->ModuleControlRegisterAddress) = OS_HAL_TIMER_FTM_SC_VALUE;                           /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TIMER_CONFIG */

  /* #30 Enable the interrupt generation and set to output compare mode. */
  *(volatile uint32*)(TimerConfig->ChannelControlRegisterAddress) = OS_HAL_TIMER_FTM_CHANNEL_VALUE;                     /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TIMER_CONFIG */

  /* #40 Activate interrupt source. */
  Os_Hal_IntEnableSource(TimerConfig->InterruptConfig);                                                                 /* SBSW_OS_HAL_FC_CALLER */
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
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
OS_LOCAL_INLINE Os_Hal_TimerFrtTickType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_TimerFrtGetCounterValue,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))                                                                                                                      /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
{
  /* #10 Perform dsb and isb instructions in order to get actual value of counter register. */
  Os_Hal_InstructionSyncBarrier();
  Os_Hal_DataSyncBarrier();

  /* #20 Return value of counter register. */
  return *(volatile Os_Hal_TimerFrtTickType*)(TimerConfig->ModuleCountRegisterAddress);                                 /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
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
))                                                                                                                      /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
{
  /* #10 Perform dsb and isb instructions in order to get actual value of counter register. */
  Os_Hal_InstructionSyncBarrier();
  Os_Hal_DataSyncBarrier();

  /* #20 Return value of compare register. */
  return *(volatile Os_Hal_TimerFrtTickType*)(TimerConfig->ChannelCompareRegisterAddress);                              /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
}


/***********************************************************************************************************************
 *  Os_Hal_TimerFrtSetCompareValue()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
Os_Hal_TimerFrtSetCompareValue,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig,
  Os_Hal_TimerFrtTickType ExpirationTime
))                                                                                                                      /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
{
  /* #10 Set value of compare register. */
  *(volatile Os_Hal_TimerFrtTickType*)(TimerConfig->ChannelCompareRegisterAddress) = ExpirationTime;                    /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TIMER_CONFIG */
}


/***********************************************************************************************************************
 *  Os_Hal_TimerFrtAcknowledge()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
Os_Hal_TimerFrtAcknowledge,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))                                                                                                                      /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
{
  /* #10 Clear the corresponding interrupt flag bit. */
  *(volatile uint32*)(TimerConfig->ChannelControlRegisterAddress) &= OS_HAL_TIMER_FTM_CHANNEL_CHIE_CLEAR_MASK;          /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TIMER_CONFIG */
}


/***********************************************************************************************************************
 *  Os_Hal_TimerFrtTriggerIrq()
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerFrtTriggerIrq,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))                                                                                                                      /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
{
  /* #10 Calculate the new compare value by adding an appropriate amount of ticks to the current counter value. */
  Os_Hal_TimerFrtTickType newCompare = Os_Hal_TimerFrtGetCounterValue(TimerConfig) + 2u;                                /* SBSW_OS_HAL_FC_CALLER */

  /* #20 Update the hardware compare register with the calculated compare value. */
  Os_Hal_TimerFrtSetCompareValue(TimerConfig, newCompare);                                                              /* SBSW_OS_HAL_FC_CALLER */

  /* #30 Assure that the interrupt is pending before leaving the function. */
  while(((*(volatile uint32*)(TimerConfig->ChannelControlRegisterAddress)) & OS_HAL_TIMER_FTM_CHANNEL_CHF_MASK) == 0u)  /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
  {
    /* Do nothing. */
  }
}


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* OS_HAL_TIMER_FRT_FTM_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Timer_Frt_FTM.h
 **********************************************************************************************************************/

