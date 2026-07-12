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
 *  \file    Os_Hal_Timer_NXP_GPTInt.h
 *  \brief   Data types and interfaces to work with the Timer.
 *  \details Timer module features:
 *              - Six timers : GPT1 .. GPT6.
 *              - HAL supports currently the FRT part. [Note: PIT support can be added in HAL in the future if needed]
 *
 *  Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_TIMER_FRT_NXP_GPTINT_H
# define OS_HAL_TIMER_FRT_NXP_GPTINT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os HAL dependencies */
# include"Os_Hal_TimerInt.h"


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/* CR register macros */
# define OS_HAL_TIMER_NXP_GPT_REG_CR_BIT_EN_MASK        (0x00000001uL)    /* Bit 0 */
# define OS_HAL_TIMER_NXP_GPT_REG_CR_BIT_ENMOD_MASK     (0x00000002uL)    /* Bit 1 */
# define OS_HAL_TIMER_NXP_GPT_REG_CR_BIT_DBGEN_MASK     (0x00000004uL)    /* Bit 2 */
# define OS_HAL_TIMER_NXP_GPT_REG_CR_BIT_WAITEN_MASK    (0x00000008uL)    /* Bit 3 */
# define OS_HAL_TIMER_NXP_GPT_REG_CR_BIT_DOZEEN_MASK    (0x00000010uL)    /* Bit 4 */
# define OS_HAL_TIMER_NXP_GPT_REG_CR_BIT_STOPEN_MASK    (0x00000020uL)    /* Bit 5 */
# define OS_HAL_TIMER_NXP_GPT_REG_CR_BIT_FRR_MASK       (0x00000200uL)    /* Bit 9  */
# define OS_HAL_TIMER_NXP_GPT_REG_CR_BIT_SWR_MASK       (0x00008000uL)    /* Bit 15 */
# define OS_HAL_TIMER_NXP_GPT_REG_CR_FIELD_IM1_MASK     (0x00030000uL)    /* Bits 16, 17 */
# define OS_HAL_TIMER_NXP_GPT_REG_CR_FIELD_IM2_MASK     (0x000C0000uL)    /* Bits 18, 19 */
# define OS_HAL_TIMER_NXP_GPT_REG_CR_FIELD_OM1_MASK     (0x00700000uL)    /* Bits 20-22 */
# define OS_HAL_TIMER_NXP_GPT_REG_CR_FIELD_OM2_MASK     (0x03800000uL)    /* Bits 23-25 */
# define OS_HAL_TIMER_NXP_GPT_REG_CR_FIELD_OM3_MASK     (0x1C000000uL)    /* Bits 26-28 */

/* IR register macros */
# define OS_HAL_TIMER_NXP_GPT_INTERRUPT_DISABLE      (0x0uL)         /* writing 0 to bits 0-5 disables all interrupts. */
# define OS_HAL_TIMER_NXP_GPT_REG_IR_BIT_OF1IE_MASK  (0x00000001uL)  /* Bit 0 */

/* SR register macros */
# define OS_HAL_TIMER_NXP_GPT_CLEAR_STATUS_REG       (0xFFFFFFFFuL)  /* writing 1 to bits 0-5 clears all status bits */
# define OS_HAL_TIMER_NXP_GPT_REG_SR_BIT_OF1_MASK    (0x00000001uL)  /* Bit 0 */

/* PR register macros */
# define OS_HAL_TIMER_NXP_GPT_REG_PR_FIELD_PRESCALER_MASK     (0x00000FFFuL)  /* Bits 0-11 */
# define OS_HAL_TIMER_NXP_GPT_REG_PR_FIELD_PRESCALER24M_MASK  (0x0000F000uL)  /* Bits 12-15 */

/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! Memory mapped NXP_GPT timer registers */
struct Os_Hal_TimerNxpGptType_Tag
{
  uint32 TimerCR;      /*! < +00h Control Register (CR) */
  uint32 TimerPR;      /*! < +04h Prescaler Register (PR) */
  uint32 TimerSR;      /*! < +08h Status Register (SR) */
  uint32 TimerIR;      /*! < +0Ch Interrupt Register (IR) */
  uint32 TimerOCR1;    /*! < +10h Output Compare Register 1 (OCR1) */
  uint32 TimerOCR2;    /*! < +14h Output Compare Register 2 (OCR2) */
  uint32 TimerOCR3;    /*! < +18h Output Compare Register 3 (OCR3) */
  uint32 TimerICR1;    /*! < +1Ch Input Capture Register 1 (ICR1) */
  uint32 TimerICR2;    /*! < +20h Input Capture Register 2 (ICR2) */
  uint32 TimerCNT;     /*! < +24h Counter Register (CNT) */
};

/*! Forward declaration of platform specific NXP_GPT timer type. */
typedef struct Os_Hal_TimerNxpGptType_Tag Os_Hal_TimerNxpGptType;

/*! Pointer to platform specific NXP_GPT timer type. */
typedef CONSTP2VAR(volatile Os_Hal_TimerNxpGptType, AUTOMATIC, OS_CONST) Os_Hal_TimerNxpGptRefType;

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
 
/* Timer function prototypes are located in the included file "Os_Hal_TimerInt.h" */


#endif /* OS_HAL_TIMER_FRT_NXP_GPTINT_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Timer_NXP_GPTInt.h
 **********************************************************************************************************************/

