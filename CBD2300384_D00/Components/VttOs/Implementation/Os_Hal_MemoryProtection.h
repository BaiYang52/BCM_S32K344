/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/**
 *  \addtogroup Os_Hal_MemoryProtection
 *  \{
 *
 *  \file       Os_Hal_MemoryProtection.h
 *  \brief
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388  EOF */ /* MD_MSR_Dir1.1 */
#ifndef OS_HAL_MEMORYPROTECTION_H
# define OS_HAL_MEMORYPROTECTION_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Hal_MemoryProtectionInt.h"
# include "Os_Hal_Context_Types.h"

/* Os kernel module dependencies */
# include "Os_Kernel.h"
# include "Os_Common.h"

/* Os Hal dependencies */
# include "Os_Hal_Compiler.h"
# include "Os_Hal_Core.h"


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

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
  OS_IGNORE_UNREF_PARAM(SystemConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_MpCoreInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_MpCoreInit,
(
  P2CONST(Os_Hal_MpCoreConfigType, TYPEDEF, OS_CONST) CoreConfig,
  P2CONST(Os_Hal_ContextStackConfigType, AUTOMATIC, OS_VAR_NOINIT) InitialStackRegion
))
{
  OS_IGNORE_UNREF_PARAM(CoreConfig);                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(InitialStackRegion);                                                                            /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_MpAppSwitch()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_MpAppSwitch,
(
  P2CONST(Os_Hal_MpAppConfigType, TYPEDEF, OS_CONST) AppConfig
))
{
  OS_IGNORE_UNREF_PARAM(AppConfig);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_MpThreadSwitch()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_MpThreadSwitch,
(
  P2CONST(Os_Hal_MpThreadConfigType, TYPEDEF, OS_CONST) ThreadConfig
))
{
  OS_IGNORE_UNREF_PARAM(ThreadConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* OS_HAL_MEMORYPROTECTION_H */


/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_MemoryProtection.h
 **********************************************************************************************************************/
