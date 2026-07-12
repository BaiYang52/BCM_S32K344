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
 *  \addtogroup Os_Hal_Compiler
 *  \{
 *
 *  \file
 *  \brief        Compiler abstraction for Texas Instruments compiler.
 *  \details
 *
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

# ifdef CDK_CHECK_MISRA
#  ifndef OS_STATIC_CODE_ANALYSIS
#    define OS_STATIC_CODE_ANALYSIS
#  endif
# endif




/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Os_Hal_Interrupt_Cfg.h"
# include "Os_Hal_Entry_Cfg.h"
# include "Os_Hal_CompilerInt.inc"
# include "Os_Hal_Core_Cfg.h"

  OS_HAL_ASM_CODE_SECTION(OS_CODE)
  OS_HAL_ASM_DUMMY_NOP
  OS_HAL_ASM_SECTION_END

# if defined(OS_CFG_COMPILER_TEXASINSTRUMENTS)
 # if !defined(OS_STATIC_CODE_ANALYSIS)
  OS_HAL_ASM_THUMB_CODE

  OS_HAL_ASM_EXPORT(Os_Hal_ReadStackPointer)
  OS_HAL_ASM_EXPORT(Os_Hal_ReadLinkRegister)
  OS_HAL_ASM_EXPORT(Os_Hal_ReadBasepri)
  OS_HAL_ASM_EXPORT(Os_Hal_ReadControl)
  OS_HAL_ASM_EXPORT(Os_Hal_DisableAndReturnPriorState)
  OS_HAL_ASM_EXPORT(Os_Hal_ReadPsr)

/***********************************************************************************************************************
 *  Os_Hal_ReadStackPointer()
 **********************************************************************************************************************/
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_ReadStackPointer)
  mov r0, sp
  bx  lr
  OS_HAL_ASM_FUNCTION_END(Os_Hal_ReadStackPointer)

/***********************************************************************************************************************
 *  Os_Hal_ReadLinkRegister()
**********************************************************************************************************************/
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_ReadLinkRegister)
  mov r0, lr
  bx  lr
  OS_HAL_ASM_FUNCTION_END(Os_Hal_ReadLinkRegister)



/***********************************************************************************************************************
 *  Os_Hal_ReadBasepri()
 **********************************************************************************************************************/
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_ReadBasepri)
  mrs r0, BASEPRI
  bx  lr
  OS_HAL_ASM_FUNCTION_END(Os_Hal_ReadBasepri)


/***********************************************************************************************************************
 *  Os_Hal_ReadControl()
 *********************************************************************************************************************/
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_ReadControl)
  mrs r0, CONTROL
  bx lr
  OS_HAL_ASM_FUNCTION_END(Os_Hal_ReadControl)


 /***********************************************************************************************************************
  *  Os_Hal_ReadPsr()
 **********************************************************************************************************************/
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_ReadPsr)
  mrs r0, XPSR
  bx lr
  OS_HAL_ASM_FUNCTION_END(Os_Hal_ReadPsr)


/***********************************************************************************************************************
 *  Os_Hal_DisableAndReturnPriorState()
 **********************************************************************************************************************/
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_DisableAndReturnPriorState)
  push {r1}
  mrs r0, basepri
  mov r1, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr basepri, r1
  pop {r1}
  bx lr
  OS_HAL_ASM_FUNCTION_END(Os_Hal_DisableAndReturnPriorState)

  #if defined(OS_CFG_THREAD_FPU_CONTEXT_ENABLED_ASM)

  OS_HAL_ASM_EXPORT(Os_Hal_StoreFpuRegisters)
  OS_HAL_ASM_EXPORT(Os_Hal_RestoreFpuRegisters)

/***********************************************************************************************************************
 *  Os_Hal_StoreFpuRegisters()
 *********************************************************************************************************************/
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_StoreFpuRegisters)
  vstm r0, {s16-s31}
  bx lr
  OS_HAL_ASM_FUNCTION_END(Os_Hal_StoreFpuRegisters)


 /***********************************************************************************************************************
  *  Os_Hal_RestoreFpuRegisters()
 **********************************************************************************************************************/
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_RestoreFpuRegisters)
  vldm r0, {s16-s31}
  bx lr
  OS_HAL_ASM_FUNCTION_END(Os_Hal_RestoreFpuRegisters)
  #endif /* OS_CFG_THREAD_FPU_CONTEXT_ENABLED == STD_ON */
 #endif /* !defined OS_STATIC_CODE_ANALYSIS */
#endif /* OS_CFG_COMPILER_TEXASINSTRUMENTS */

/*!
 * \}
 */
/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Compiler_TiInt.h
 *********************************************************************************************************************/

 OS_HAL_ASM_MODULE_END
