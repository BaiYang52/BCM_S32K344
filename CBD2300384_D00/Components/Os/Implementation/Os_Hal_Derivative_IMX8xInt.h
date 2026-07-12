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
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 *********************************************************************************************************************/

#ifndef OS_HAL_DERIVATIVE_IMX8XINT_H
# define OS_HAL_DERIVATIVE_IMX8XINT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  IMX8x
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

/*! Defines whether NVIC_IMX8 interrupt controller is supported (STD_ON) or not (STD_OFF). */
# define OS_HAL_INTC_NVIC_IMX8     (STD_ON)

/*! NVIC, interrupt controller base address. */
# define OS_HAL_INTC_NVIC_BASE     (0xE000E000uL)

/*! STEER, interrupt controller base address. */
# define OS_HAL_INTC_STEER_BASE     (0x51070000uL)

/*! STEER_CHN_STATUS address. */
# define OS_HAL_STEER_CHN_STATUS_ASM          0x51070084
# define OS_HAL_STEER_CHN_STATUS_BASE        (0x51070084uL)

/*! STEER_CHN_MASK address. */
# define OS_HAL_STEER_CHN_MASK_ASM          0x51070004
# define OS_HAL_STEER_CHN_MASK_BASE        (0x51070004uL)

# define OS_HAL_STEER_CHN_MINTDIS_REG_OFFSET     (0x000000C4uL)

# define OS_HAL_STEER_NUMBER_OF_CHANNELS     (15u)

/**********************************************************************************************************************
 *  Timer
 *********************************************************************************************************************/
/*! Defines TPM Timer Unit Base Address. */
# define OS_HAL_TIMER_TPM_BASE     (0x41200000uL)

/*! Defines whether FRT_TPM Timer Unit is supported (STD_ON) or not (STD_OFF). */
# define OS_HAL_TIMER_FRT_TPM     (STD_ON)

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



#endif /* OS_HAL_DERIVATIVE_IMX8XINT_H */

/*!
 * \}
 */
/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Derivative_IMX8xInt.h
 *********************************************************************************************************************/
