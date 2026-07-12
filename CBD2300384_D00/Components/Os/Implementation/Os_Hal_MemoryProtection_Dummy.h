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
 *  \addtogroup Os_Hal_MemoryProtection
 *  \{
 *
 *  \file
 *  \brief      Dummy memory protection primitives. Needed if the hardware does not support a memory protection unit.
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

#ifndef OS_HAL_MEMORYPROTECTION_DUMMY_H
# define OS_HAL_MEMORYPROTECTION_DUMMY_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os kernel module dependencies */
# include "Os_Common.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

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

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Os_Hal_MpSystemInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,                                                             /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_ALWAYS_INLINE, Os_Hal_MpSystemInit,
(
  P2CONST(Os_Hal_MpSystemConfigType, TYPEDEF, OS_CONST) SystemConfig
))
{
  /* #10 No system MPU supported. Do nothing. */
  OS_IGNORE_UNREF_PARAM(SystemConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_MpCoreInit()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_MpCoreInit,
(
  P2CONST(Os_Hal_MpCoreConfigType , TYPEDEF, OS_CONST) CoreConfig,
  P2CONST(Os_Hal_ContextStackConfigType, AUTOMATIC, OS_VAR_NOINIT) InitialStackRegion
))
{
  /* #10 No core MPU supported. Do nothing. */
  OS_IGNORE_UNREF_PARAM(CoreConfig);                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(InitialStackRegion);                                                                            /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_MpAppSwitch()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
Os_Hal_MpAppSwitch,
(
  P2CONST(Os_Hal_MpAppConfigType, TYPEDEF, OS_CONST) AppConfig
))
{
  /* #10 No MPU supported. Do nothing. */
  OS_IGNORE_UNREF_PARAM(AppConfig);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_MpThreadSwitch()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
Os_Hal_MpThreadSwitch,
(
  P2CONST(Os_Hal_MpThreadConfigType, TYPEDEF, OS_CONST) ThreadConfig
))
{
  /* #10 No MPU supported. Do nothing. */
  OS_IGNORE_UNREF_PARAM(ThreadConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_MpGetStackRegion()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,                                                             /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_ALWAYS_INLINE, Os_Hal_MpGetStackRegion,
(
  P2VAR(Os_Hal_ContextStackConfigType, TYPEDEF, OS_VAR_NOINIT) StackConfig
))
{
  /* #10 No MPU supported. Do nothing. */
  OS_IGNORE_UNREF_PARAM(StackConfig);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* OS_HAL_MEMORYPROTECTION_DUMMY_H */

/*!
 * \}
 */
/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_MemoryProtection_Dummy.h
 *********************************************************************************************************************/
