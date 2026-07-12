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
 *  \file    Os_Hal_InterruptController_NVIC_CYT2.h
 *  \brief   HAL interface to work with the Traveo2 Interrupt Architecture.
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

#ifndef OS_HAL_INTERRUPTCONTROLLER_NVIC_CYT2_H
# define OS_HAL_INTERRUPTCONTROLLER_NVIC_CYT2_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define OS_HAL_INTC_PERIPHERAL_TRIGGER_SUPPORTED     (0u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/*! Interrupt Set-Enable Registers (ISER). */
# define OS_HAL_INTC_NVIC_ISER(x)    (*(volatile uint32*) (OS_HAL_INTC_NVIC_BASE + 0x100uL + ((x) * 4uL)))

/*! Interrupt Clear-Enable Registers (ICER). */
# define OS_HAL_INTC_NVIC_ICER(x)    (*(volatile uint32*) (OS_HAL_INTC_NVIC_BASE + 0x180uL + ((x) * 4uL)))

/*! System Cortex-M4 Interrupt Control Register (CPUSS_CM4_SYSTEM_INT_CTL). */
# define OS_HAL_CPUSS_CM4_SYSTEM_INT_CTL(x)     (*(volatile uint32*) (OS_HAL_INTC_CPUSS_CM4_SYSTEM_BASE + ((x) * 4UL)))

/*! \brief  SSYSTEM_INT_CTL Regiter: ENABLED flag mask.
 *  \details
 *  1 = set enabled bit
 *  0 = do not set enabled bit.
 */
# define OS_HAL_INTC_SYSTEM_INT_CL_ENABLED_BIT  (1uL << 31)

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
 *  Os_Hal_INTC_EnableWriteAccess()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_EnableWriteAccess,
(
  Os_Hal_VectorTableType VectorTable
))
{
  /* #10 No action for NVIC_CYT2 necessary. */
  OS_IGNORE_UNREF_PARAM(VectorTable);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  OS_HAL_INTC_NVIC_RouteSource()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_HAL_INTC_NVIC_RouteSource,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
))
{
  /* #10 No action for NVIC_CYT2 necessary. */
  OS_IGNORE_UNREF_PARAM(IsrConfig);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_NVIC_EnableMappedSource()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_NVIC_EnableMappedSource,
(
  Os_Hal_IntSourceType SystemInterruptSource,
  Os_Hal_IntSourceType CoreInterruptSource
))
{
  /* #10 Set enable bit in System Interrupt control register and map the system interrupt to the core internal one. */
  OS_HAL_CPUSS_CM4_SYSTEM_INT_CTL(SystemInterruptSource) =  OS_HAL_INTC_SYSTEM_INT_CL_ENABLED_BIT | CoreInterruptSource;/* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_INTC_NVIC_CPUSS */

  /* #20 Set enable bit in ISER register for the core interrupt source. */
  OS_HAL_INTC_NVIC_ISER(CoreInterruptSource >> 5) = (1uL << (CoreInterruptSource & 31uL));                              /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_INTC_NVIC_ISER */
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_NVIC_DisableMappedSource()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_NVIC_DisableMappedSource,
(
  Os_Hal_IntSourceType Source
))
{
  /* #10 Disable system interrupt in System Interrupt control register. */
  OS_HAL_CPUSS_CM4_SYSTEM_INT_CTL(Source) &= ~OS_HAL_INTC_SYSTEM_INT_CL_ENABLED_BIT;                                    /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_INTC_NVIC_CPUSS */
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_NVIC_IsMappedInterruptSourceEnabled()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE uint8, OS_CODE, OS_ALWAYS_INLINE,                                          /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_NVIC_IsMappedInterruptSourceEnabled,
(
  Os_Hal_IntSourceType CoreInterruptSource,
  Os_Hal_IntSourceType SystemInterruptSource
))
{
  uint32 isExternalInterruptEnabled;
  uint32 isInternalInterruptEnabled;
  uint32 isInterruptSourceEnabled;

  /* #10 Read enable bit on NVIC ICER register for the mapped core interrupt source. */
  isInternalInterruptEnabled = ((OS_HAL_INTC_NVIC_ICER(CoreInterruptSource >> 5u) &                                     /* PRQA S 4404, 0303 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Hal_Rule11.4_0303 */
                                 (1uL << (CoreInterruptSource & 31uL))) !=  0u);

  /* #20 Read enable bit on SYS_INT_CTL register for the given system interrupt source. */
  isExternalInterruptEnabled = OS_HAL_CPUSS_CM4_SYSTEM_INT_CTL(SystemInterruptSource)                                   /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
      & OS_HAL_INTC_SYSTEM_INT_CL_ENABLED_BIT;

  isInterruptSourceEnabled = ((isInternalInterruptEnabled != 0uL) && (isExternalInterruptEnabled != 0uL));              /* PRQA S 4404 */ /* MD_Os_C90BooleanCompatibility */

  /* #30 Return !0 if both interrupts are enabled, 0 otherwise. */
  return (uint8)isInterruptSourceEnabled;
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_NVIC_IsClearPendingSupported()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_NVIC_IsClearPendingSupported, ( void ))
{
  /* #10 Return clear pending is not supported. */
  return 0u;                                                                                                            /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_NVIC_IntIsInterruptMappingSupported()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_NVIC_IntIsInterruptMappingSupported, ( void ))
{
  /* #10 Return !0 because the hierarchical interrupt controllers are supported. */
  return !0u;                                                                                                           /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* OS_HAL_INTERRUPTCONTROLLER_NVIC_CYT2_H */

/*!
 * \}
 */
/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_InterruptController_NVIC_CYT2.h
 *********************************************************************************************************************/

