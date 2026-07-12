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
 *  \file    Os_Hal_InterruptController_NVIC_S32.h
 *  \brief   HAL interface to work with the S32x Interrupt Controller architecture.
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

#ifndef OS_HAL_INTERRUPTCONTROLLER_NVIC_S32X_H
# define OS_HAL_INTERRUPTCONTROLLER_NVIC_S32X_H

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

/*! Interrupt Router Shared Peripheral Routing Control Register (MSCM_IRSPRC). */
# define OS_HAL_MSCM_IRSPRC(x)       (*(volatile uint16*) (OS_HAL_MSCM_IRSPRC_BASE + ((x) * 2uL)))

/*! Macro to get the index inside MSCM_IRSPRC, given the physical core id parameter. */
# define OS_HAL_MSCM_IRSPRC_COREID2FIELD(x)       (x + OS_HAL_MSCM_IRSPRC_CORE0_BITOFFFSET)

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
  /* #10 No action for NVIC_S32x necessary. */
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
  Os_CorePhysicalIdType CoreId = Os_Hal_CoreGetId();

  /* #10 Set IRSPRC register to route the given source to given running core. */
  OS_HAL_MSCM_IRSPRC(IsrConfig->CoreInterruptSource) |= (uint16)(1UL << OS_HAL_MSCM_IRSPRC_COREID2FIELD(CoreId));       /* PRQA S 0303, 2985 */ /* MD_Os_Hal_Rule11.4_0303, MD_Os_Rule2.2_2996 */ /* SBSW_OS_HAL_INTC_NVIC_IPR */
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_NVIC_EnableMappedSource()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_NVIC_EnableMappedSource,
(
  Os_Hal_IntSourceType SystemInterruptSource,
  Os_Hal_IntSourceType CoreInterruptSource
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  /* #10 No action for NVIC_S32x necessary. */
  OS_IGNORE_UNREF_PARAM(SystemInterruptSource);                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(CoreInterruptSource);                                                                           /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_NVIC_DisableMappedSource()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_NVIC_DisableMappedSource,
(
  Os_Hal_IntSourceType Source
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  /* #10 No action for NVIC_S32x necessary. */
  OS_IGNORE_UNREF_PARAM(Source);                                                                                        /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
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
  /* #10 Return clear pending is supported. */
  return !0u;                                                                                                            /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}


/***********************************************************************************************************************
 *  Os_Hal_INTC_NVIC_IsMappedInterruptSourceEnabled()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE uint8, OS_CODE, OS_ALWAYS_INLINE,                                          /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_INTC_NVIC_IsMappedInterruptSourceEnabled,
(
  Os_Hal_IntSourceType CoreInterruptSource,
  Os_Hal_IntSourceType SystemInterruptSource
))                                                                                                                      /* COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY */
{
  OS_IGNORE_UNREF_PARAM(CoreInterruptSource);                                                                           /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(SystemInterruptSource);                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  /* #10 Return 0 because the hierarchical interrupt controllers are not supported. */
  return 0u;
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
  /* #10 Return 0 because the hierarchical interrupt controllers are not supported. */
  return 0u;                                                                                                            /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* OS_HAL_INTERRUPTCONTROLLER_NVIC_S32X_H */

/*!
 * \}
 */
/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_InterruptController_NVIC_S32x.h
 *********************************************************************************************************************/

