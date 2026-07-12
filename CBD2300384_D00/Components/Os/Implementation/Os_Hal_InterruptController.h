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
 *  \addtogroup  Os_Hal_Interrupt
 *  \{
 *
 *  \file    Os_Hal_InterruptController.h
 *  \brief   HAL interface to work with the ARM Nested Vectored Interrupt Controllers.
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
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */

#ifndef OS_HAL_INTERRUPTCONTROLLER_H
# define OS_HAL_INTERRUPTCONTROLLER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"

# include "Os_Hal_Derivative.h"
# include "Os_Hal_Compiler.h"
# include "Os_Hal_Interrupt_Types.h"
# include "Os_Error.h"

# if defined (OS_HAL_INTC_NVIC_CYT2)                                                                                    /* COV_OS_DERIVATIVEBETAMODULE */
#  include "Os_Hal_InterruptController_NVIC_CYT2.h"
# elif defined (OS_HAL_INTC_NVIC_IMX8)                                                                                  /* COV_OS_DERIVATIVEBETAMODULE */
#  include "Os_Hal_InterruptController_NVIC_IMX8.h"
# elif defined (OS_HAL_INTC_NVIC_S32)                                                                                   /* COV_OS_DERIVATIVEBETAMODULE */
# include "Os_Hal_InterruptController_NVIC_S32.h"
# elif defined (OS_HAL_INTC_NVIC_CYT4)                                                                                  /* COV_OS_DERIVATIVEBETAMODULE */
# include "Os_Hal_InterruptController_NVIC_CYT4.h"
# elif defined (OS_HAL_INTC_NVIC)
#  include "Os_Hal_InterruptController_NVIC.h"
# else
#  error "Undefined or unsupported Interrupt controller"
# endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/*! Interrupt Set-Enable Registers (ISER). */
# define OS_HAL_INTC_NVIC_ISER(x)    (*(volatile uint32*) (OS_HAL_INTC_NVIC_BASE + 0x100uL + ((x) * 4uL)))

/*! Interrupt Clear-Enable Registers (ICER). */
# define OS_HAL_INTC_NVIC_ICER(x)    (*(volatile uint32*) (OS_HAL_INTC_NVIC_BASE + 0x180uL + ((x) * 4uL)))

/*! Interrupt Set-Pending Registers (ISPR). */
# define OS_HAL_INTC_NVIC_ISPR(x)    (*(volatile uint32*) (OS_HAL_INTC_NVIC_BASE + 0x200uL + ((x) * 4uL)))

/*! Interrupt Clear-Pending Registers (ICPR). */
# define OS_HAL_INTC_NVIC_ICPR(x)    (*(volatile uint32*) (OS_HAL_INTC_NVIC_BASE + 0x280uL + ((x) * 4uL)))

/*! Interrupt Priority Registers (IPR). */
# define OS_HAL_INTC_NVIC_IPR(x)     (*(volatile uint8*) (OS_HAL_INTC_NVIC_BASE + 0x400uL + (x)))

/*! Interrupt Control State Register (ICSR). */
# define OS_HAL_INTC_NVIC_ICSR       (*(volatile uint32*) (0xE000ED04uL))

/*! Vector Table Offset Register (VTOR). */
# define OS_HAL_INTC_SCB_VTOR        (*(volatile uint32*) (0xE000ED08uL))

/*! \brief    Logical interrupt level used for tasks.
 *  \details  The logically lowest interrupt level. */
# define OS_HAL_TASK_LEVEL           (0x0u)

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  Os_Hal_INTC_InitializeVectorTable()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_InitializeVectorTable,
(
  Os_Hal_VectorTableType VectorTable
))
{
  uint32 VectorTableStartAddress = (uint32)VectorTable;                                                                 /* PRQA S 0306 */ /* MD_Os_Hal_Rule11.4_0306 */

  /* #10 Enable write access to VTOR. */
  Os_Hal_INTC_EnableWriteAccess(VectorTable);                                                                           /* SBSW_OS_HAL_FC_CALLER */

  /* #20 Set VTOR Register to given base address. */
  Os_Assert(((VectorTableStartAddress & 0x7FuL) == 0uL));                                                               /* PRQA S 4404 */ /* MD_Os_C90BooleanCompatibility */
  OS_HAL_INTC_SCB_VTOR = VectorTableStartAddress;                                                                       /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_INTC_INITIALIZEVECTORTABLE_001 */

  /* #30 Disable interrupts. */
  Os_Hal_INTC_SetCurrentLevel(OS_CFG_HAL_GLOBAL_DISABLE_LEVEL);
}

/***********************************************************************************************************************
 *  Os_Hal_INTC_InitSource()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_InitSource,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
))
{
  /* #10 Set IPR register at given source to given level. */
  OS_HAL_INTC_NVIC_IPR(IsrConfig->CoreInterruptSource) = (uint8)IsrConfig->Level;                                       /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_INTC_NVIC_IPR */

  /* #20 Route the given source to the given running core. */
  OS_HAL_INTC_NVIC_RouteSource(IsrConfig);                                                                              /* SBSW_OS_HAL_FC_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_EnableMappedSource()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_EnableMappedSource,
(
  Os_Hal_IntSourceType SystemInterruptSource,
  Os_Hal_IntSourceType CoreInterruptSource
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  /* #10 Enable the interrupt handling for the given interrupt. */
  Os_Hal_INTC_NVIC_EnableMappedSource(SystemInterruptSource, CoreInterruptSource);
}

/***********************************************************************************************************************
 *  Os_Hal_INTC_EnableNotMappedSource()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_EnableNotMappedSource,
(
  Os_Hal_IntSourceType CoreInterruptSource
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  /* #10 Set enable bit in ISER register for given source. */
  OS_HAL_INTC_NVIC_ISER(CoreInterruptSource >> 5) = (1uL << (CoreInterruptSource & 31uL));                              /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_INTC_NVIC_ISER */
}

/***********************************************************************************************************************
 *  Os_Hal_INTC_DisableSource()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_DisableSource,
(
  Os_Hal_IntSourceType Source
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  /* #10 Set disable bit in ICER register for given source. */
  OS_HAL_INTC_NVIC_ICER(Source >> 5) = (1uL << (Source & 31uL));                                                        /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_INTC_NVIC_ICER */
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_DisableMappedSource()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_DisableMappedSource,
(
  Os_Hal_IntSourceType Source
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  /* #10 Disable the interrupt handling for the given mapped interrupt source number. */
  Os_Hal_INTC_NVIC_DisableMappedSource(Source);
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_SetCurrentLevel()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_SetCurrentLevel,
(
  Os_Hal_IntLevelType Level
))
{
  /* #10 Set BASEPRI to given level. */
  Os_Hal_WriteBasepri(Level);
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_GetCurrentLevel()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_Hal_IntLevelType, OS_CODE, OS_ALWAYS_INLINE,                            /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_GetCurrentLevel,
(
  void
))
{
  /* #10 Return BASEPRI. */
  return Os_Hal_ReadBasepri();
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_IsClearPendingSupported()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_IsClearPendingSupported, ( void ))
{
  /* #10 Returns whether the Interrupt controller supports clearing of a pending interrupt. */
  return Os_Hal_INTC_NVIC_IsClearPendingSupported();
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_EndOfInterrupt()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_EndOfInterrupt,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
))
{
  /* #10 Set clear bit in ICPR register for given source. */
  OS_HAL_INTC_NVIC_ICPR(IsrConfig->CoreInterruptSource >> 5) = (1uL << (IsrConfig->CoreInterruptSource & 31uL));        /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_INTC_NVIC_ICPR */
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_TriggerSoftwareInterrupt()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_TriggerSoftwareInterrupt,
(
  Os_Hal_IntSourceType Source
))                                                                                                                      /* COV_OS_HALPLATFORMUNUSED */
{
  /* #10 Set pending bit in ISPR register for given source. */
  OS_HAL_INTC_NVIC_ISPR(Source >> 5) = (1uL << (Source & 31uL));                                                        /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_INTC_NVIC_ISPR */
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_IsInterruptSourceEnabled()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE uint8, OS_CODE, OS_ALWAYS_INLINE,                                          /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_IsInterruptSourceEnabled,
(
  Os_Hal_IntSourceType Source
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  /* #10 Return clear-enable bit in ICER register for given source. */
  return (uint8)((OS_HAL_INTC_NVIC_ICER(Source >> 5u) & (1uL << (Source & 31uL))) != 0u);                               /* PRQA S 0303, 4304 */ /* MD_Os_Hal_Rule11.4_0303, MD_Os_C90BooleanCompatibility  */
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_IsMappedInterruptSourceEnabled()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE uint8, OS_CODE, OS_ALWAYS_INLINE,                                          /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_IsMappedInterruptSourceEnabled,
(
  Os_Hal_IntSourceType CoreInterruptSource,
  Os_Hal_IntSourceType SystemInterruptSource
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  /* #10 Returns whether the given mapped interrupt source number is enabled. */
  return Os_Hal_INTC_NVIC_IsMappedInterruptSourceEnabled(CoreInterruptSource, SystemInterruptSource);
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_IsInterruptPending()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE uint8, OS_CODE, OS_ALWAYS_INLINE,                                          /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_IsInterruptPending,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  /* #10 Return clear-pending bit in ICPR for given source. */
  return (uint8)((OS_HAL_INTC_NVIC_ICPR(IsrConfig->CoreInterruptSource >> 5u)                                           /* PRQA S 0303, 4304 */ /* MD_Os_Hal_Rule11.4_0303, MD_Os_C90BooleanCompatibility  */
      & (1uL << (IsrConfig->CoreInterruptSource & 31uL))) != 0u);
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_IntLevelIsGt()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_IntLevelIsGt,
(
  CONST(Os_Hal_IntLevelType, OS_CONST) LevelX,
  CONST(Os_Hal_IntLevelType, OS_CONST) LevelY
))
{
  /* #10 Return !0 if LevelX is smaller than LevelY. */
  return (Os_StdReturnType)((LevelX - 1u) < (LevelY - 1u));                                                             /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_IntIsInterruptMappingSupported()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_IntIsInterruptMappingSupported, ( void ))
{
  /* #10 Returns whether the hardware supports interrupt mapping. */
  return Os_Hal_INTC_NVIC_IntIsInterruptMappingSupported();
}


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* OS_HAL_INTERRUPTCONTROLLER_H */

/*!
 * \}
 */
/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_InterruptController.h
 *********************************************************************************************************************/

