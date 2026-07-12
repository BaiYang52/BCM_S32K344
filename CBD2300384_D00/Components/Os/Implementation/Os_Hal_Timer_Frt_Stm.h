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
 * \brief    This file contains hardware specific definition of the STM module.
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

#ifndef OS_HAL_TIMER_FRT_STM_H
# define OS_HAL_TIMER_FRT_STM_H

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

/*! Bit mask for STM Channel interrupt register to access pending interrupt flag. */
# define OS_HAL_TIMER_STM_CHANNEL_ISR_MASK          (0x00000001uL)

/*! Offset value to add to count register value to trigger an interrupt. */
# define OS_HAL_TIMER_STM_ISR_TRIG_OFFSET           (0x10uL)

/*! Bit mask for STM Control Register initialization, set prescaler to 1 and enable debug mode. */
# define OS_HAL_TIMER_STM_CR_INIT_MASK              (0x00000002uL)

/*! Bit mask for STM Control Register to set counter enable bit. */
# define OS_HAL_TIMER_STM_CR_EN_MASK                (0x00000001uL)

/*! Bit mask for STM Channel Control Register to enable channel. */
# define OS_HAL_STM_CCRN_EN_MASK                    (0x00000001uL)


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! High resolution timer configuration information. */
struct Os_Hal_TimerFrtConfigType_Tag
{
  /*! The timer base address. */
  uint32 TimerBaseAddr;
  /*! The timer compare value. */
  uint32 CompareValue;
  /*! Interrupt Source Number of the corresponding STM Channel */
  P2CONST(Os_Hal_IntIsrConfigType, OS_CONST, OS_CONST) InterruptConfig;
};

/*! Memory mapped Timer STM registers corresponding to the memory layout */
struct Os_Hal_TimerSTMType_Tag
{
  uint32 STMCR;      /*!< +00h Control register. (32 bit read/write) */
  uint32 STMCNT;     /*!< +04h Count register. (32 bit read/write) */
  uint32 STMUNUSED1; /*! unused */
  uint32 STMUNUSED2; /*! unused */
  uint32 STMCCR0;    /*!< +10h Channel control register (32 bit read/write) */
  uint32 STMCIR0;    /*!< +14h Channel interrupt register (32 bit write 1 to clear) */
  uint32 STMCMP0;    /*!< +18h Channel compare register (32 bit read write) */
};


/*! Forward declaration of platform specific STM timer type. */
typedef struct Os_Hal_TimerSTMType_Tag Os_Hal_TimerSTMType;

/*! Pointer to platform specific STM timer type. */
typedef P2VAR(volatile Os_Hal_TimerSTMType, AUTOMATIC, OS_CONST) Os_Hal_TimerSTMRefType;


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
 *
 *
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
Os_Hal_TimerFrtInit,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))                                                                                                                      /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
{
  Os_Hal_TimerSTMRefType TimerRef = (Os_Hal_TimerSTMRefType)(TimerConfig->TimerBaseAddr);                               /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */

  /* #10 Clear any pending interrupt. */
  Os_Hal_TimerFrtAcknowledge(TimerConfig);                                                                              /* SBSW_OS_HAL_FC_CALLER */

  /* #20 Configure the Counter Control Register. */
  TimerRef->STMCR = OS_HAL_TIMER_STM_CR_INIT_MASK;                                                                      /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TIMER_CONFIG */

  /* #30 Set compare value. */
  TimerRef->STMCMP0 = TimerConfig->CompareValue;                                                                        /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TIMER_CONFIG */

  /* #40 Enable timer channel. */
  TimerRef->STMCCR0 = OS_HAL_STM_CCRN_EN_MASK;                                                                          /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TIMER_CONFIG */

  /* #50 Enable timer - start counting. */
  TimerRef->STMCR |= OS_HAL_TIMER_STM_CR_EN_MASK;                                                                       /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TIMER_CONFIG */

  /* #60 Activate interrupt source. */
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
  return ((Os_Hal_TimerSTMRefType)TimerConfig->TimerBaseAddr)->STMCNT;                                                  /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
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

  /* #20 Return value of channel compare register. */
  return ((Os_Hal_TimerSTMRefType)TimerConfig->TimerBaseAddr)->STMCMP0;                                                 /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
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
  /* #10 Set value of channel compare register. */
  ((Os_Hal_TimerSTMRefType)TimerConfig->TimerBaseAddr)->STMCMP0 = ExpirationTime;                                       /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TIMER_CONFIG */
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
  ((Os_Hal_TimerSTMRefType)TimerConfig->TimerBaseAddr)->STMCIR0 = OS_HAL_TIMER_STM_CHANNEL_ISR_MASK;                    /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TIMER_CONFIG */
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

#endif /* OS_HAL_TIMER_FRT_STM_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Timer_Frt_STM.h
 **********************************************************************************************************************/

