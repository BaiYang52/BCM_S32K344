/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**
 * \addtogroup  Os_Hal_Derivative
 * \{
 *
 * \file
 * \brief  This file contains derivative specific information.
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 *********************************************************************************************************************/

#ifndef OS_HAL_DERIVATIVE_S32G3XINT_H
# define OS_HAL_DERIVATIVE_S32G3XINT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  S32G3x
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Core
 *********************************************************************************************************************/
/*! Defines whether ARMv7 CortexM Architecture is supported (STD_ON) or not (STD_OFF). */
# define OS_HAL_ARCH_V7M     (STD_ON)

/*! Defines whether V7M Exceptions Handling is supported (STD_ON) or not (STD_OFF). */
# define OS_HAL_EXCEPTIONS_V7M     (STD_ON)


/**********************************************************************************************************************
 *  Interrupt Controller
 *********************************************************************************************************************/

/*! Defines whether NVIC_S32 interrupt controller is supported (STD_ON) or not (STD_OFF). */
# define OS_HAL_INTC_NVIC_S32     (STD_ON)

/*! NVIC, interrupt controller base address. */
# define OS_HAL_INTC_NVIC_BASE     (0xE000E000uL)


/*! MSCM, interrupt controller base address. */
# define OS_HAL_INTC_MSCM_BASE     (0x40198000uL)

/*! MSCM_IRSPRC address. */
# define OS_HAL_MSCM_IRSPRC_ASM          0x40198880
# define OS_HAL_MSCM_IRSPRC_BASE        (0x40198880uL)
# define OS_HAL_MSCM_IRSPRC_CORE0_BITOFFFSET           (0x00000001uL)
# define OS_HAL_MSCM_IRCPSR_BASE_OFFSET          (0xA60uL)
# define OS_HAL_MSCM_IRCPGR_BASE_OFFSET          (0xA64uL)
# define OS_HAL_MSCM_IRCPSR_CORE_OFFSET          (0x70uL)
# define OS_HAL_MSCM_IRCPSR_ACKNOWLEDGE_MASK          (0x0FFFuL)

/**********************************************************************************************************************
 *  Timer
 *********************************************************************************************************************/
/*! Defines STM timer Unit Base Address. */
# define OS_HAL_TIMER_STM0_BASE    (0x4011C000uL)
/*! Defines STM timer Unit Base Address. */
# define OS_HAL_TIMER_STM1_BASE    (0x40120000uL)
/*! Defines STM timer Unit Base Address. */
# define OS_HAL_TIMER_STM2_BASE    (0x40124000uL)
/*! Defines STM timer Unit Base Address. */
# define OS_HAL_TIMER_STM3_BASE    (0x40128000uL)
/*! Defines STM timer Unit Base Address. */
# define OS_HAL_TIMER_STM4_BASE    (0x4021C000uL)
/*! Defines STM timer Unit Base Address. */
# define OS_HAL_TIMER_STM5_BASE    (0x40220000uL)
/*! Defines STM timer Unit Base Address. */
# define OS_HAL_TIMER_STM6_BASE    (0x40224000uL)
/*! Defines STM timer Unit Base Address. */
# define OS_HAL_TIMER_STM7_BASE    (0x40228000uL)
/*! Defines STM timer Unit Base Address. */
# define OS_HAL_TIMER_STM8_BASE    (0x40520000uL)
/*! Defines STM timer Unit Base Address. */
# define OS_HAL_TIMER_STM9_BASE    (0x40524000uL)
/*! Defines STM timer Unit Base Address. */
# define OS_HAL_TIMER_STM10_BASE    (0x40528000uL)
/*! Defines STM timer Unit Base Address. */
# define OS_HAL_TIMER_STM11_BASE    (0x4052C000uL)

/*! Defines whether FRT_STM Timer Unit is supported (STD_ON) or not (STD_OFF). */
# define OS_HAL_TIMER_FRT_STM     (STD_ON)

/*! Defines whether PIT_SYSTICK Timer Unit is supported (STD_ON) or not (STD_OFF). */
# define OS_HAL_TIMER_PIT_SYSTICK     (STD_ON)


/**********************************************************************************************************************
 *  Memory Protection
 *********************************************************************************************************************/
/*! Defines whether PMSAV7M Memory Protection Architecture is supported (STD_ON) or not (STD_OFF). */
# define OS_HAL_MEMORY_PROTECTION_PMSAV7M         (STD_ON)
# define OS_HAL_MPU_BASE_ADDRESS_ASM              0xE000ED90
# define OS_HAL_MPU_BASE_ADDRESS                  (0xE000ED90uL)

/**********************************************************************************************************************
 *  Physical Core Modules
 *********************************************************************************************************************/
/*! RGM_PRST address. */
# define OS_HAL_RGM_PRST_BASE        (0x40078040uL)
/*! RGM_PSTAT address. */
# define OS_HAL_RGM_PSTAT_BASE        (0x40078140uL)
/*! MC_ME_PRTN0_CORE0 address. */
# define OS_HAL_MC_ME_PRTN0_CORE0_BASE        (0x40088140uL)
/*! MC_ME_PRTN0_CORE1 address. */
# define OS_HAL_MC_ME_PRTN0_CORE1_BASE        (0x40088160uL)
/*! MC_ME_PRTN0_CORE2 address. */
# define OS_HAL_MC_ME_PRTN0_CORE2_BASE        (0x40088180uL)
/*! MC_ME_PRTN0_CORE3 address. */
# define OS_HAL_MC_ME_PRTN0_CORE3_BASE        (0x400881A0uL)



#endif /* OS_HAL_DERIVATIVE_S32G3XINT_H */

/*!
 * \}
 */
/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Derivative_S32G3xInt.h
 *********************************************************************************************************************/
