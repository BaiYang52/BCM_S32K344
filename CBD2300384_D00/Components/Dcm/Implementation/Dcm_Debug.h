/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Dcm_Debug.h
 *         \unit  Debug
 *        \brief  Contains public inline function definitions of Debug unit.
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined(DCM_DEBUG_H)
# define DCM_DEBUG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_DebugTypes.h"
# include "Dcm.h"
# include "Dcm_UtiTypes.h"

# if (DCM_DEV_ERROR_REPORT == STD_ON)
#  include "Det.h"
# endif

# if (DCM_DEBUG_SECURITY_EVENT_REPORT_ENABLED == STD_ON)
#  include "IdsM.h"
# endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Debug sub-component initialization states */
# define DCM_UNINIT                                                  (0x00u) /*!< DCM is not or partially initialized. In this state DCM is completely isolated from the other BSWs/SW-C in the ECU */
# define DCM_INIT                                                    (0x01u) /*!< DCM is initialized. In this state DCM is fully operable */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_DebugDeInit()
 *********************************************************************************************************************/
/*! \brief          Puts DCM into uninitialized state (isolate from the rest of the world)
 *  \details        -
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_GLOBAL_INLINE FUNC(void, DCM_CODE) Dcm_DebugDeInit(
  void
  );

/**********************************************************************************************************************
 *  Dcm_DebugIsModuleInitialized()
 *********************************************************************************************************************/
/*! \brief          Checks whether DCM is initialized.
 *  \details        -
 *  \return         TRUE         DCM is initialized
 *  \return         FALSE        DCM is not initialized
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_GLOBAL_INLINE FUNC(boolean, DCM_CODE) Dcm_DebugIsModuleInitialized(
  void
  );

/**********************************************************************************************************************
 *  Dcm_DebugIsDevErrorDetectEnabled()
 *********************************************************************************************************************/
/*! \brief          Returns DCM_DEV_ERROR_DETECT as boolean.
 *  \details        -
 *  \return         TRUE         DCM_DEV_ERROR_DETECT is STD_ON
 *  \return         FALSE        DCM_DEV_ERROR_DETECT is STD_OFF
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_GLOBAL_INLINE FUNC(boolean, DCM_CODE) Dcm_DebugIsDevErrorDetectEnabled(
  void
  );

/**********************************************************************************************************************
 *  Dcm_DebugIsDevRuntimeChecksEnabled()
 *********************************************************************************************************************/
/*! \brief          Returns DCM_DEV_RUNTIME_CHECKS as boolean.
 *  \details        -
 *  \return         TRUE         DCM_DEV_RUNTIME_CHECKS is STD_ON
 *  \return         FALSE        DCM_DEV_RUNTIME_CHECKS is STD_OFF
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_GLOBAL_INLINE FUNC(boolean, DCM_CODE) Dcm_DebugIsDevRuntimeChecksEnabled(
  void
  );

/**********************************************************************************************************************
 *  Dcm_DebugDetectError()
 *********************************************************************************************************************/
/*! \brief          Checks for Development Error Detection.
 *  \details        -
 *  \param[in]      condition    Condition to evaluate
 *  \return         TRUE         Condition is true and Error Detection is active
 *  \return         FALSE        Condition is false and/or Error Detection is disabled
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_GLOBAL_INLINE FUNC(boolean, DCM_CODE) Dcm_DebugDetectError(
  boolean condition
  );

/**********************************************************************************************************************
 *  Dcm_DebugDetectRuntimeError()
 *********************************************************************************************************************/
/*! \brief          Checks for Runtime Error Detection.
 *  \details        -
 *  \param[in]      condition    Condition to evaluate
 *  \return         TRUE         Condition is true and Runtime Error Detection is active
 *  \return         FALSE        Condition is false and/or Runtime Error Detection is disabled
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_GLOBAL_INLINE FUNC(boolean, DCM_CODE) Dcm_DebugDetectRuntimeError(
  boolean condition
  );

/**********************************************************************************************************************
 *  Dcm_DebugReportError()
 *********************************************************************************************************************/
/*! \brief          Checks for Development Error Report.
 *  \details        -
 *  \param[in]      sid          API Service ID
 *  \param[in]      errorId      Error ID
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_GLOBAL_INLINE FUNC(void, DCM_CODE) Dcm_DebugReportError(
  uint8 sid,
  uint8 errorId
  );

/**********************************************************************************************************************
 *  Dcm_DebugAssert()
 *********************************************************************************************************************/
/*! \brief          Supports DCM internal debugging for additional, deeper analysis
 *  \details        -
 *  \param[in]      sid          API Service ID
 *  \param[in]      errorId      Error ID
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_GLOBAL_INLINE FUNC(void, DCM_CODE) Dcm_DebugAssert(
  boolean condition,
  uint8 sid,
  uint8 errorId
  );
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_DebugInit()
 *********************************************************************************************************************/
/*! \brief          Initializes the debug sub-component.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DebugInit(
  void
  );

/**********************************************************************************************************************
 *  Dcm_DebugReportSecurityEvent()
 *********************************************************************************************************************/
/*! \brief          Reports the security events to the Idsm.
 *  \details        -
 *  \param[in]      threadId           The active thread identifier
 *  \param[in]      idsmEvent          IdsM security event
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DebugReportSecurityEvent(
  Dcm_ThreadIdOptType threadId,
  uint16 idsmEvent
  );

/**********************************************************************************************************************
 *  Dcm_DebugResetSecurityEventStatus()
 *********************************************************************************************************************/
/*! \brief          Set Idsm event status.
 *  \details        -
 *  \param[in]      threadId         Active thread
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DebugResetSecurityEventStatus(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_DebugReportSecurityEventNRC()
 *********************************************************************************************************************/
/*! \brief          Reports Idsm event depending on the given error code.
 *  \details        -
 *  \param[in]      threadId        Active thread
 *  \param[in]      ErrorCode       The error code
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DebugReportSecurityEventNRC(
  Dcm_ThreadIdOptType threadId,
  Dcm_NegativeResponseCodeType ErrorCode
  );
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_VAR_INIT_8
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Dedicated RAM allocation for modules initialization state */
extern VAR(uint8, DCM_VAR_INIT) Dcm_DebugInitState;
# define DCM_STOP_SEC_VAR_INIT_8
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if !defined(DCM_NOUNIT_SWITCHES)                                                                                                                   /* COV_DCM_NOUNIT TX */
/**********************************************************************************************************************
 *  Dcm_DebugIsDevErrorDetectEnabled()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_GLOBAL_INLINE FUNC(boolean, DCM_CODE) Dcm_DebugIsDevErrorDetectEnabled(                                                                          /* PRQA S 3219, 1527 */ /* MD_MSR_Unreachable, MD_MSR_Unreachable */
  void
  )
{
  return (DCM_DEV_ERROR_DETECT == STD_ON);                                                                                                           /* PRQA S 2995, 2996 */ /* MD_Dcm_ConstExpr */
}

/**********************************************************************************************************************
 *  Dcm_DebugIsDevRuntimeChecksEnabled()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_GLOBAL_INLINE FUNC(boolean, DCM_CODE) Dcm_DebugIsDevRuntimeChecksEnabled(                                                                        /* PRQA S 3219, 1527 */ /* MD_MSR_Unreachable */
  void
  )
{
  return (DCM_DEV_RUNTIME_CHECKS == STD_ON);                                                                                                         /* PRQA S 2995, 2996 */ /* MD_Dcm_ConstExpr */
}
# endif /* !defined(DCM_NOUNIT_SWITCHES) */

# if !defined(DCM_NOUNIT_DEBUG)                                                                                                                      /* COV_DCM_NOUNIT TX */
/**********************************************************************************************************************
 *  Dcm_DebugDeInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_GLOBAL_INLINE FUNC(void, DCM_CODE) Dcm_DebugDeInit(                                                                                              /* PRQA S 3219 */ /* MD_MSR_Unreachable */
  void
  )
{
  Dcm_DebugInitState = DCM_UNINIT;
}

/**********************************************************************************************************************
 *  Dcm_DebugIsModuleInitialized()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_GLOBAL_INLINE FUNC(boolean, DCM_CODE) Dcm_DebugIsModuleInitialized(                                                                              /* PRQA S 3219 */ /* MD_MSR_Unreachable */
  void
  )
{
  return (Dcm_DebugInitState == DCM_INIT);
}

/**********************************************************************************************************************
 *  Dcm_DebugDetectError()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_GLOBAL_INLINE FUNC(boolean, DCM_CODE) Dcm_DebugDetectError(                                                                                      /* PRQA S 3219, 1527 */ /* MD_MSR_Unreachable */
  boolean condition
  )
{
  DCM_IGNORE_UNREF_PARAM(condition);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return (Dcm_DebugIsDevErrorDetectEnabled() && condition);
}

/**********************************************************************************************************************
 *  Dcm_DebugDetectRuntimeError()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_GLOBAL_INLINE FUNC(boolean, DCM_CODE) Dcm_DebugDetectRuntimeError(                                                                               /* PRQA S 3219, 1527 */ /* MD_MSR_Unreachable */
  boolean condition
  )
{
  DCM_IGNORE_UNREF_PARAM(condition);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return (Dcm_DebugIsDevRuntimeChecksEnabled() && condition);
}

/**********************************************************************************************************************
 *  Dcm_DebugReportError()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_GLOBAL_INLINE FUNC(void, DCM_CODE) Dcm_DebugReportError(                                                                                         /* PRQA S 3219, 1527 */ /* MD_MSR_Unreachable */
  uint8 sid,
  uint8 errorId
  )
{
#  if (DCM_DEV_ERROR_REPORT == STD_ON)
  if(errorId != DCM_E_NO_ERROR)
  {
    (void)Det_ReportError(DCM_MODULE_ID, 0u, sid, errorId);
  }
#  else
  DCM_IGNORE_UNREF_PARAM(sid);                                                                                                                       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(errorId);                                                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#  endif
}

/**********************************************************************************************************************
 *  Dcm_DebugAssert()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_GLOBAL_INLINE FUNC(void, DCM_CODE) Dcm_DebugAssert(                                                                                              /* PRQA S 3219, 1527 */ /* MD_MSR_Unreachable */
  boolean condition,
  uint8 sid,
  uint8 errorId
  )
{
  if(!condition)                                                                                                                                     /* COV_DCM_RTM_UNREACHABLE XF */
  {
    Dcm_DebugReportError(sid, errorId);
  }
}
# endif /* !defined(DCM_NOUNIT_DEBUG) */
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* !defined(DCM_DEBUG_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Debug.h
 *********************************************************************************************************************/
