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

#ifndef OS_HAL_DERIVATIVE_S32K2TVINT_H
# define OS_HAL_DERIVATIVE_S32K2TVINT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  S32K2TV
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
# define OS_HAL_INTC_MSCM_BASE     (0x40060000uL)

/*! MSCM_IRSPRC address. */
# define OS_HAL_MSCM_IRSPRC_ASM          0x40060880
# define OS_HAL_MSCM_IRSPRC_BASE        (0x40060880uL)
# define OS_HAL_MSCM_IRSPRC_CORE0_BITOFFFSET           (0x00000002uL)

/**********************************************************************************************************************
 *  Timer
 *********************************************************************************************************************/

/*! Defines whether FRT_UNSUPPORTED Timer Unit is supported (STD_ON) or not (STD_OFF). */
# define OS_HAL_TIMER_FRT_UNSUPPORTED     (STD_ON)

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



#endif /* OS_HAL_DERIVATIVE_S32K2TVINT_H */

/*!
 * \}
 */
/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Derivative_S32K2TVInt.h
 *********************************************************************************************************************/
