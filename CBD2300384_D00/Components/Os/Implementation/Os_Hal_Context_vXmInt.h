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
 *  \addtogroup Os_Hal_Context
 *  \{
 *
 *  \file
 *  \brief      Context definition for ARM-M.
 *  \details
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

#ifndef OS_HAL_CONTEXT_VXMINT_H
# define OS_HAL_CONTEXT_VXMINT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */

/* Os kernel module dependencies */

/* Os Hal dependencies */


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
/*! Number of permanent registers. */
# define OS_HAL_CONTEXT_NUMBER_OF_PERMANENT_REGISTERS  (8)
/*! Size of the basic frame. */
# define OS_HAL_BASIC_FRAME_SIZE                       (0x24uL)
/*! Size of the additional extended frame bytes to be added to the basic frame size. */
# define OS_HAL_EXTENDED_FRAME_ADDITIONAL_SIZE         (0x48uL)
/*! Number of additional FPU registers registers (not automatically saved by HW). */
#if (OS_CFG_THREAD_FPU_CONTEXT_ENABLED == STD_ON)
# define OS_HAL_CONTEXT_NUMBER_OF_ADDITIONAL_FPU_REGISTERS  (16)
#else
# define OS_HAL_CONTEXT_NUMBER_OF_ADDITIONAL_FPU_REGISTERS  (1)
#endif
/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
/*! \brief     Exception frame
 *  \details   Describes the memory layout of the register stored by the hardware at exception entry.
*/
struct Os_Hal_ExceptionFrame_Tag
{
  uint32 R0;                      /*!< R0. */
  uint32 R1;                      /*!< R1. */
  uint32 R2;                      /*!< R2. */
  uint32 R3;                      /*!< R3. */
  uint32 R12;                     /*!< R12. */
  uint32 Lr;                      /*!< Link Register (LR). */
  uint32 Pc;                      /*!< Program Counter (PC). */
  uint32 Psr;                     /*!< Program Status Register (PSR). */
};


/*! \brief     Minimal set of hardware registers to be able to suspend and resume a thread.
 *  \details   In case the core MPU is used for stack protection, the StackRegionInfo parameter includes the
 *             stack region size. In case the system MPU is used, the paramter includes the stack region end address.
*/
struct Os_Hal_ContextType_Tag
{
  uint32 Sp;                      /*!< Stack Pointer (SP). */
  uint32 Lr;                      /*!< Link Register (LR). */
  uint32 Primask;                 /*!< PRIMASK Register. */
  uint32 PermanentRegisters[OS_HAL_CONTEXT_NUMBER_OF_PERMANENT_REGISTERS]; /*!< Permanent registers (r4-r11). */
  uint32 Control;                 /*!< CONTROL Register (CONTROL). */
  uint32 Basepri;                 /*!< BASEPRI Register (BASEPRI). */
  uint32 StackRegionStart;        /*!< Stack region start (low address). */
  uint32 StackRegionInfo;         /*!< Additional Stack region information (region size or region end (high) address).*/
  Os_Hal_ExceptionFrame ExcFrame; /*!< Exception frame. */
};

/*! \brief    Set of floating point registers to be able to suspend and resume a thread.
  * \details  Not used because fpu registers are saved by hardware on exception entry, if they are used.
*/
struct Os_Hal_ContextFpuContextType_Tag
{
   uint32 FpuAdditionalRegisters[OS_HAL_CONTEXT_NUMBER_OF_ADDITIONAL_FPU_REGISTERS];                                    /* PRQA S 1039 */ /* MD_Os_Hal_Rule1.2_1039 */
};

/*! \brief     Information needed to initialize the context of a thread.
 *  \details   In case the core MPU is used for stack protection, the StackRegionInfo parameter includes the
 *             stack region size. In case the system MPU is used, the paramter includes the stack region end address.
*/
struct Os_Hal_ContextConfigType_Tag
{
  uint32 StackRegionEnd;          /*!< Stack regions end. */
  uint32 Control;                 /*!< CONTROL Register (CONTROL). */
  uint32 Basepri;                 /*!< BASEPRI Register (BASEPRI). */
  uint32 Entry;                   /*!< Entry function (initial program counter value). */
  uint32 ReturnAddress;           /*!< Initial link register value. */
  uint32 StackRegionStart;        /*!< Stack region start (low address). */
  uint32 StackRegionInfo;         /*!< Additional Stack region information (region size or region end address). */
};


/*! Information needed to perform a stack switch. */
struct Os_Hal_ContextStackConfigType_Tag
{
  uint32 StackRegionEnd;          /*!< Stack end address (high address) */
  uint32 StackRegionStart;        /*!< Stack start address (low address) */
  uint32 StackRegionSize;         /*!< Stack size. */
};


/*! Information needed to perform a context switch. */
typedef struct
{
  uint32 NextSprPtr;              /*!< Pointer to Sp, Lr. */
  uint32 PermRegsPtr;             /*!< Pointer to the permanent registers. */
  uint32 Basepri;                 /*!< New BASEPRI value. */
  uint32 SaveSprPtr;              /*!< Address to store the current SP and LR. */
  uint32 MpuStackRegionPtr;       /*!< Pointer to the MPU stack region configuration. */
} Os_Hal_ContextSwitchInfoType;


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

#endif /* OS_HAL_CONTEXT_VXMINT_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Context_vXmInt.h
 **********************************************************************************************************************/
