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
 *  \file    Os_Hal_InterruptController_NVIC_IMX8.h
 *  \brief   HAL interface to work with the Cortex-M4 subsystem Interrupt Controller on the IMX8x platform,
 *  which comprises of three parts
 *  --A Core local Nested Vector Interrupt Controller (NVIC)
 *  --A SystemLevel Generic Interrupt Controller (GIC)
 *  --A STEER module to distribute the interrupts from the 512 GIC sources to other subsystems(including the M4),
 *  on the M4 these sources are mapped as 8 External Interrupt sources, as described below. Only the STEER module
 *  needs to be handled by the M4. The nummber of sources and STEER channels vary between derivatives.
 * +--------------------------+-------------------------------+-------------------+
 * | Interrupt Group GIC SPI  |    External Interrupt Number  |    Cortex-M4 IRQ   |
 * +--------------------------+-------------------------------+-------------------+
 * |       0 - 63            |          0                     |   48              |
 * |       64 - 127          |          1                     |   49              |
 * |      128 - 191          |          2                     |   50              |
 * |      192 - 255          |          3                     |   51              |
 * |      256 - 319          |          4                     |   52              |
 * |      320 - 383          |          5                     |   53              |
 * |      384 - 447          |          6                     |   54              |
 * |      448 - 511          |          7                     |   55              |
 * +--------------------------+-------------------------------+-------------------+
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

#ifndef OS_HAL_INTERRUPTCONTROLLER_NVIC_IMX8_H
# define OS_HAL_INTERRUPTCONTROLLER_NVIC_IMX8_H

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

/*! STEER Module coreid based Base Address (x = CoreID). */
# define OS_HAL_INTC_STEER_CORE_BASE(x)           (OS_HAL_INTC_STEER_BASE + ((x) * 0x10000uL))

/*! STEER Channel Interrupt mask register (CHN_MASKy) (x = CoreID, y = Register Index). */
# define OS_HAL_INTC_STEER_CHN_MASK(x,y)  (*(volatile uint32*) (OS_HAL_INTC_STEER_CORE_BASE(x) + 0x4uL + ((y) * 4uL)))

/*! STEER Channel master interrupt disable register (CHn_MINTDIS) (x = CoreID). */
# define OS_HAL_INTC_STEER_CHn_MINTDIS(x)         (*(volatile uint32*) (OS_HAL_INTC_STEER_CORE_BASE(x)                  \
                                                                                + OS_HAL_STEER_CHN_MINTDIS_REG_OFFSET))

/* STEER register index calculation (x = System Interrupt Source) */
# define OS_HAL_STEER_CALCULATE_REGISTER_VALUE(x) (OS_HAL_STEER_NUMBER_OF_CHANNELS - ((x) / 32u))

/* STEER bit offset calculation (x = System Interrupt Source) */
# define OS_HAL_STEER_CALCULATE_BIT_OFFSET(x)     ((x) - ((OS_HAL_STEER_NUMBER_OF_CHANNELS                              \
                                                                  - (OS_HAL_STEER_CALCULATE_REGISTER_VALUE(x))) * 32u))

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
  /* #10 No action for NVIC_IMX8 necessary. */
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
  /* #10 No action for NVIC_IMX8 necessary. */
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
  const Os_CorePhysicalIdType CoreId = Os_Hal_CoreGetId();

  /* #10 Configure the STEER module to enable the routing of the system interrupt. */
  OS_HAL_INTC_STEER_CHN_MASK(CoreId, OS_HAL_STEER_CALCULATE_REGISTER_VALUE(SystemInterruptSource)) |=                   /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_INTC_NVIC_STEER */
    (0x1uL  << OS_HAL_STEER_CALCULATE_BIT_OFFSET(SystemInterruptSource));
  OS_HAL_INTC_STEER_CHn_MINTDIS(CoreId) &= ~(0x1uL << (SystemInterruptSource / 64uL));                                  /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_INTC_NVIC_STEER */

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
  const Os_CorePhysicalIdType CoreId = Os_Hal_CoreGetId();

  /* #10 Disable system interrupt in System Interrupt mask register. */
  OS_HAL_INTC_STEER_CHN_MASK(CoreId, OS_HAL_STEER_CALCULATE_REGISTER_VALUE(Source)) &=                                  /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_INTC_NVIC_STEER */
    (~(0x1uL  << OS_HAL_STEER_CALCULATE_BIT_OFFSET(Source)));
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
  const Os_CorePhysicalIdType CoreId = Os_Hal_CoreGetId();

  /* #10 Read enable bit on NVIC ICER register for the mapped core interrupt source. */
  isInternalInterruptEnabled = ((OS_HAL_INTC_NVIC_ICER(CoreInterruptSource >> 5) &                                      /* PRQA S 4404, 0303 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Hal_Rule11.4_0303 */
        (1uL << (CoreInterruptSource & 31uL))) != 0u);

  /* #20 Read enable bit on STEER_CHN_MASK register for the given system interrupt source. */
  isExternalInterruptEnabled = (((OS_HAL_INTC_STEER_CHN_MASK(                                                           /* PRQA S 4404, 0303 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Hal_Rule11.4_0303 */
                                  CoreId,
                                  OS_HAL_STEER_CALCULATE_REGISTER_VALUE(SystemInterruptSource))) &
                                  (0x1uL  << OS_HAL_STEER_CALCULATE_BIT_OFFSET(SystemInterruptSource))) != 0u);

  /* #30 Return !0 if both interrupts are enabled, 0 otherwise. */
  isInterruptSourceEnabled = (((isInternalInterruptEnabled) & (isExternalInterruptEnabled)) != 0u);                     /* PRQA S 4404 */ /* MD_Os_C90BooleanCompatibility */

  return (uint8)(isInterruptSourceEnabled);
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

#endif /* OS_HAL_INTERRUPTCONTROLLER_NVIC_IMX8_H */

/*!
 * \}
 */
/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_InterruptController_IMX8x.h
 *********************************************************************************************************************/

