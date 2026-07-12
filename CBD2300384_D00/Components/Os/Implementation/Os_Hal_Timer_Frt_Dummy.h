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
 *  \addtogroup  Os_Hal_Timer
 *  \{
 *
 *  \file
 *  \brief        Dummy Implementation for FRT.
 *  \details
 *  Provides empty dummy functions for derivatives without an FRT timer unit.
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_TIMER_FRT_DUMMY_H
# define OS_HAL_TIMER_FRT_DUMMY_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

 /*! High resolution timer configuration dummy type */
struct Os_Hal_TimerFrtConfigType_Tag
{
  uint32 Dummy;
};

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
 *  Os_Hal_TimerFrtInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerFrtInit,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))                                                                                                                      /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
{
  /* #10 Stub: do nothing. */
  OS_IGNORE_UNREF_PARAM(TimerConfig);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_TimerFrtGetCounterValue()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_Hal_TimerFrtTickType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_TimerFrtGetCounterValue,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))                                                                                                                      /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
{
  /* #10 Stub: do nothing. */
  OS_IGNORE_UNREF_PARAM(TimerConfig);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return 0;
}


/***********************************************************************************************************************
 *  Os_Hal_TimerFrtGetCompareValue()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_Hal_TimerFrtTickType, OS_CODE, OS_ALWAYS_INLINE,                        /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerFrtGetCompareValue,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))                                                                                                                      /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
{
  /* #10 Stub: do nothing. */
  OS_IGNORE_UNREF_PARAM(TimerConfig);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return 0;
}


/***********************************************************************************************************************
 *  Os_Hal_TimerFrtSetCompareValue()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerFrtSetCompareValue,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig,
  Os_Hal_TimerFrtTickType ExpirationTime
))                                                                                                                      /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
{
  /* #10 Stub: do nothing. */
  OS_IGNORE_UNREF_PARAM(TimerConfig);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(ExpirationTime);                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_TimerFrtAcknowledge()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerFrtAcknowledge,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))                                                                                                                      /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
{
  /* #10 Stub: do nothing. */
  OS_IGNORE_UNREF_PARAM(TimerConfig);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_TimerFrtTriggerIrq()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_TimerFrtTriggerIrq,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig
))                                                                                                                      /* COV_OS_HALPLATFORMFRTUNSUPPORTED_XX */
{
  /* #10 Stub: do nothing. */
  OS_IGNORE_UNREF_PARAM(TimerConfig);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* OS_HAL_TIMER_FRT_DUMMY_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Timer_Frt_Dummy.h
 **********************************************************************************************************************/
