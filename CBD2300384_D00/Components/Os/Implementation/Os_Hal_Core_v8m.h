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
 *  \addtogroup Os_Hal_Core
 *  \{
 *
 *  \file
 *  \brief      ARMv8-M related primitives.
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
 *
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */

#ifndef OS_HAL_CORE_V8M_H
# define OS_HAL_CORE_V8M_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Common.h"

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! Dummy XSignal interrupt configuration, in case of single core Cortex-M derivatives. */
struct Os_Hal_XSigInterruptConfigType_Tag
{
  uint32 Dummy;                       /*!< Dummy variable. */
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
 *  Os_Hal_CoreIsThreadPointerUserReadable()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreIsThreadPointerUserReadable,
(
  void
))
{
  /* #10 Return not readable in user mode. */
  return 0u;                                                                                                           /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}


/***********************************************************************************************************************
 *  Os_Hal_CoreIsCoreIdUserReadable()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* COV_OS_HALPLATFORMCOREIDNOTREADABLE */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreIsCoreIdUserReadable,
(
  void
))                                                                                                                      /* COV_OS_HALPLATFORMCOREIDNOTREADABLE */
{
  /* #10 Return not readable in user mode. */
  return 0u;                                                                                                            /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
}

/***********************************************************************************************************************
 *  Os_Hal_CoreGetId()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */

OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_CorePhysicalIdType, OS_CODE, OS_ALWAYS_INLINE,                          /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreGetId,
(
  void
))
{
  /* #10 Return physical core ID. */
  return (Os_CorePhysicalIdType)0;
}

/***********************************************************************************************************************
 *  Os_Hal_SpinInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_SpinInit,
(
  P2VAR(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
))
{

  /* #10 Set given spinlock to 0. */
  *Spinlock = 0;                                                                                                        /* SBSW_OS_HAL_PWA_CALLER */
}

/***********************************************************************************************************************
 *  Os_Hal_SpinTryLock()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_Hal_SpinlockResultType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_SpinTryLock,
(
  P2VAR(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
))
{
  Os_Hal_SpinlockResultType result = !OS_HAL_SPINLOCKSUCCEEDED;                                                         /* PRQA S 4404, 4558 */ /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */

  /* #10 If spinlock is free (use load exclusive instruction here): */
  if( !Os_Hal_LoadExclusive(Spinlock) )                                                                                 /* PRQA S 4116, 4558 */ /* MD_Os_Rule10.1_4116, MD_Os_Rule10.1_4558 */ /* SBSW_OS_HAL_FC_CALLER */
  {
    /* #20 Try to get the spinlock (use store exclusive instruction here). */
    result = (Os_Hal_SpinlockResultType)Os_Hal_StoreExclusive(Spinlock, 1);                                             /* SBSW_OS_HAL_FC_CALLER */

    /* #30 If we got the spinlock: */
    if( !result )                                                                                                       /* PRQA S 4116, 4558 */ /* MD_Os_Rule10.1_4116, MD_Os_Rule10.1_4558 */
    {
      /* #40 Memory barrier. */
      Os_Hal_DataMemoryBarrier();

      /* #50 Return succes. */
    }
    /* #60 Otherwise: Return no success. */
  }
  /* #70 Otherwise: Return no success. */

  return result;
}

/***********************************************************************************************************************
 *  Os_Hal_XSigInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
Os_Hal_XSigInit,
(
  P2CONST(Os_Hal_XSigInterruptConfigType, AUTOMATIC, OS_CONST) XSigConfig
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  /* #10 Stub implementation: Not needed in single core case. */
  OS_IGNORE_UNREF_PARAM(XSigConfig);                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_XSigTrigger()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_XSigTrigger,
(
  P2CONST(Os_Hal_XSigInterruptConfigType, AUTOMATIC, OS_CONST) XSigConfig
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  /* #10 Stub implementation: Not needed in single core case. */
  OS_IGNORE_UNREF_PARAM(XSigConfig);                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_XSigAcknowledge()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_XSigAcknowledge,
(
  P2CONST(Os_Hal_XSigInterruptConfigType, AUTOMATIC, OS_CONST) XSigConfig
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  /* #10 Stub implementation: Not needed in single core case. */
  OS_IGNORE_UNREF_PARAM(XSigConfig);                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

/***********************************************************************************************************************
 *  Os_Hal_CoreStart()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */ /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Hal_CoreStart,
(
  P2CONST(Os_Hal_CoreConfigType, AUTOMATIC, OS_CONST) Core
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  /* #10 Stub implementation: Currently not needed. */
  OS_IGNORE_UNREF_PARAM(Core);                                                                                          /* PRQA S 3112 */ /* MD_Os_Rule2.2_3112 */
}

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* OS_HAL_CORE_V8M_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Core_v8m.h
 **********************************************************************************************************************/

