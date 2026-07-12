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
 *  \brief        HAL interface to work with the Timer/Pulse Width Modulation (TCPWM) module as a free running timer.
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

#ifndef OS_HAL_TIMER_FRT_TCPWM_H
# define OS_HAL_TIMER_FRT_TCPWM_H

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

/* Counter Set Enable Bit. */
# define OS_HAL_TCPWM_GRP_CNT_CTRL_ENABLE_BIT           31uL
# define OS_HAL_TCPWM_GRP_CNT_CTRL_DBGFREEZE_BIT        30uL
# define OS_HAL_TCPWM_GRP_CNT_CTRL_UPDOWN_BIT           16uL

/* Counter Set Mode Bit. */
# define OS_HAL_TCPWM_GRP_CNT_CTRL_MODE_BIT             24uL

/* Counter Trigger Command Bits. */
# define OS_HAL_TCPWM_GRP_CNT_TR_CMD_START_BIT          4uL
# define OS_HAL_TCPWM_GRP_CNT_TR_CMD_CAPTURE0_BIT       0uL

/* Counter Interupt Mask Bits. */
# define OS_HAL_TCPWM_GRP_CNT_INTR_CC0_MASK_BIT         1uL

# define OS_HAL_TCPWM_GRP_CNT_INTR_CC0_MATCH_BIT        1uL

# define OS_HAL_TCPWM_COUNTER_INIT                      0uL

/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
typedef enum
{
  OS_HAL_TCPWM_COUNTER_0 = 0u,      /*! TCPWM Counter Number 0. */
  OS_HAL_TCPWM_COUNTER_1 = 1u,      /*! TCPWM Counter Number 1. */
  OS_HAL_TCPWM_COUNTER_2 = 2u,      /*! TCPWM Counter Number 2. */
  OS_HAL_TCPWM_COUNTER_3 = 3u,      /*! TCPWM Counter Number 3. */
  OS_HAL_TCPWM_COUNTER_COUNT = 4u   /*! TCPWM Counter Number count. */
}Os_Hal_TcpwmCounterNumType;

 /*! High resolution timer configuration type */
struct Os_Hal_TimerFrtConfigType_Tag
{
  uint32 TimerBaseAddr;                      /*! The timer base address. */
  uint32 CoreInterruptSource;                /*!< Core Interrupt source. */
  uint32 SystemInterruptSource;              /*!< System Interrupt source. */
  Os_Hal_TcpwmCounterNumType CounterIndex;   /*!< Counter Index. */
};

typedef struct {
  uint32 Ctrl;                           /*!< Offset: 0x00         Counter control Register. */
  uint32 Status;                         /*!< Offset: 0x04         Counter status Register. */
  uint32 Counter;                        /*!< Offset: 0x08         Counter count Register. */
  uint32 Reserved0;                      /*!< Offset: 0x0C         Reserved. */
  uint32 CC0;                            /*!< Offset: 0x10         Counter compare/capture 0 Register. */
  uint32 CC0_Buff;                       /*!< Offset: 0x14         Counter buffered compare/capture 0 Register. */
  uint32 CC1;                            /*!< Offset: 0x18         Counter compare/capture 1 Register. */
  uint32 CC1_Buff;                       /*!< Offset: 0x1C         Counter buffered compare/capture 1 Register. */
  uint32 Period;                         /*!< Offset: 0x20         Counter period Register. */
  uint32 Period_Buff;                    /*!< Offset: 0x24         Counter buffered period Register. */
  uint32 Line_sel;                       /*!< Offset: 0x28         Counter line selection Register. */
  uint32 Line_sel_Buff;                  /*!< Offset: 0x2C         Counter buffered line selection Register. */
  uint32 Dt;                             /*!< Offset: 0x30         Counter PWM dead time Register. */
  uint32 Reserved1[3];                   /*!< Offset: 0x34-0x03C   Reserved. */
  uint32 Tr_Cmd;                         /*!< Offset: 0x40         Counter trigger command Register. */
  uint32 Reserved2[11];                  /*!< Offset: 0x44-0x06C   Reserved. */
  uint32 Intr;                           /*!< Offset: 0x70         Interrupt request Register. */
  uint32 Intr_set;                       /*!< Offset: 0x74         Interrupt set request Register. */
  uint32 Intr_Mask;                      /*!< Offset: 0x78         Interrupt mask Register. */
  uint32 Intr_Masked;                    /*!< Offset: 0x7C         Interrupt masked request register. */
} Os_Hal_TcpwmCounterType;

/*! TCPWM module Layout. */
typedef struct
{
  Os_Hal_TcpwmCounterType Counter[OS_HAL_TCPWM_COUNTER_COUNT];    /*!< Offset: 0x0000-0x0180 Counter Registers. */
} Os_Hal_TcpwmModuleType;

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
 */

OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerFrtInit,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))
{

  CONSTP2VAR(volatile Os_Hal_TcpwmModuleType, AUTOMATIC, OS_CONST) tcpwm =
                                                  (Os_Hal_TcpwmModuleType*)(TimerConfig->TimerBaseAddr);                /* PRQA S 0306 */ /* MD_Os_Hal_Rule11.4_0306 */

  const Os_Hal_TcpwmCounterNumType counterIndex = TimerConfig->CounterIndex;

  /* #10 Disable the timer. */
  tcpwm->Counter[counterIndex].Ctrl &= ~(1uL << OS_HAL_TCPWM_GRP_CNT_CTRL_ENABLE_BIT);                                  /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TCPWM_MODULETYPE */ /* SBSW_OS_HAL_TCPWM_COUNTER */

  /* #20 Set the timer Control register to default values. */
  tcpwm->Counter[counterIndex].Ctrl &= (1uL << OS_HAL_TCPWM_GRP_CNT_CTRL_ENABLE_BIT);                                   /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TCPWM_MODULETYPE */ /* SBSW_OS_HAL_TCPWM_COUNTER */

  /* #30 Enable interrupt request for compare0. */
  tcpwm->Counter[counterIndex].Intr_Mask |= (1uL << OS_HAL_TCPWM_GRP_CNT_INTR_CC0_MASK_BIT);                            /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TCPWM_MODULETYPE */ /* SBSW_OS_HAL_TCPWM_COUNTER */

  /* #40 Reset the counter register. */
  tcpwm->Counter[counterIndex].Counter = OS_HAL_TCPWM_COUNTER_INIT;                                                     /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TCPWM_MODULETYPE */ /* SBSW_OS_HAL_TCPWM_COUNTER */

  /* #50 Enable the interrupt source. */
  Os_Hal_INTC_EnableMappedSource(TimerConfig->SystemInterruptSource, TimerConfig->CoreInterruptSource);

  /* #60 Enable the timer. */
  tcpwm->Counter[counterIndex].Ctrl |= (1uL << OS_HAL_TCPWM_GRP_CNT_CTRL_ENABLE_BIT);                                   /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TCPWM_MODULETYPE */ /* SBSW_OS_HAL_TCPWM_COUNTER */

  /* #70 Start the timer. */
  tcpwm->Counter[counterIndex].Tr_Cmd |= (1uL << OS_HAL_TCPWM_GRP_CNT_TR_CMD_START_BIT);                                /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TCPWM_MODULETYPE */ /* SBSW_OS_HAL_TCPWM_COUNTER */
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
  CONSTP2CONST(volatile Os_Hal_TcpwmModuleType, AUTOMATIC, OS_CONST) tcpwm =
                                                  (Os_Hal_TcpwmModuleType*)(TimerConfig->TimerBaseAddr);                /* PRQA S 0306 */ /* MD_Os_Hal_Rule11.4_0306 */

  /* #10 Perform dsb instruction in order to get actual value of counter register. */
  Os_Hal_DataSyncBarrier();

  /* #20 Return the TCPWM counter value. */
  return tcpwm->Counter[TimerConfig->CounterIndex].Counter;                                                             /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TCPWM_MODULETYPE */ /* SBSW_OS_HAL_TCPWM_COUNTER */
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
  CONSTP2CONST(volatile Os_Hal_TcpwmModuleType, AUTOMATIC, OS_CONST) tcpwm =
                                                  (Os_Hal_TcpwmModuleType*)(TimerConfig->TimerBaseAddr);                /* PRQA S 0306 */ /* MD_Os_Hal_Rule11.4_0306 */

  /* #10 Perform dsb instruction in order to get actual value of counter register. */
  Os_Hal_DataSyncBarrier();

  /* #20 Return value of counter compare/capture register. */
  return tcpwm->Counter[TimerConfig->CounterIndex].CC0;                                                                 /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
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
   CONSTP2VAR(volatile Os_Hal_TcpwmModuleType, AUTOMATIC, OS_CONST) tcpwm =
                                                  (Os_Hal_TcpwmModuleType*)(TimerConfig->TimerBaseAddr);                /* PRQA S 0306 */ /* MD_Os_Hal_Rule11.4_0306 */

  /* #10 Set the compare value. */
  tcpwm->Counter[TimerConfig->CounterIndex].CC0 = ExpirationTime;                                                       /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TCPWM_MODULETYPE */ /* SBSW_OS_HAL_TCPWM_COUNTER */
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
   CONSTP2VAR(volatile Os_Hal_TcpwmModuleType, AUTOMATIC, OS_CONST) tcpwm =
                                                  (Os_Hal_TcpwmModuleType*)(TimerConfig->TimerBaseAddr);                /* PRQA S 0306 */ /* MD_Os_Hal_Rule11.4_0306 */

  /* #10 Clear the corresponding interrupt flag bit. */
  tcpwm->Counter[TimerConfig->CounterIndex].Intr |= (1uL << OS_HAL_TCPWM_GRP_CNT_INTR_CC0_MATCH_BIT);                   /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_TCPWM_MODULETYPE */ /* SBSW_OS_HAL_TCPWM_COUNTER */
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
  OS_IGNORE_UNREF_PARAM(TimerConfig);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* OS_HAL_TIMER_FRT_TCPWM_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Timer_Frt_Tcpwm.h
 **********************************************************************************************************************/
