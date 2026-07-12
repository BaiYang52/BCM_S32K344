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
/**        \file  Dcm_Debug.c
 *         \unit  Debug
 *        \brief  Contains the implementation of Debug unit.
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
#define DCM_DEBUG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Debug.h"
/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
#if (DCM_DEBUG_SECURITY_EVENT_REPORT_ENABLED == STD_ON)
/*! Debug Unit thread processing context */
struct DCM_DEBUGTHREADCONTEXTTYPE_TAG
{
  boolean  IdsMEventTriggered;  /*!< IdsM event triggered status */
};
typedef struct DCM_DEBUGTHREADCONTEXTTYPE_TAG Dcm_DebugThreadContextType;
typedef P2VAR(Dcm_DebugThreadContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_DebugThreadContextPtrType;
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_DEBUG_SECURITY_EVENT_REPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DebugGetThreadContext()
 *********************************************************************************************************************/
/*! \brief          Returns a debug thread context element of the given index.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         Valid debug thread context.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_DebugThreadContextPtrType, DCM_CODE) Dcm_DebugGetThreadContext(
  Dcm_ThreadIdOptType threadId
  );
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Dedicated RAM allocation for IdsM event state per thread */
#if (DCM_DEBUG_SECURITY_EVENT_REPORT_ENABLED == STD_ON)
DCM_LOCAL VAR(Dcm_DebugThreadContextType, DCM_VAR_NOINIT) Dcm_DebugThreadContext[DCM_NUM_THREADS];                                                   /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
#endif
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_VAR_INIT_8
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Dedicated RAM allocation for modules initialization state */
VAR(uint8, DCM_VAR_INIT) Dcm_DebugInitState = DCM_UNINIT;
#define DCM_STOP_SEC_VAR_INIT_8
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_DEBUG_SECURITY_EVENT_REPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DebugGetThreadContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_DebugThreadContextPtrType, DCM_CODE) Dcm_DebugGetThreadContext(
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_DebugThreadContextPtrType pThreadContext;

  if (Dcm_DebugDetectRuntimeError(threadId >= DCM_NUM_THREADS))                                                                                      /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    pThreadContext = &Dcm_DebugThreadContext[0];
  }
  else
  {
    pThreadContext = &Dcm_DebugThreadContext[threadId];
  }

  return pThreadContext;
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_DebugInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DebugInit(
  void
  )
{
  Dcm_ThreadIdOptType lThreadIter;

  Dcm_DebugInitState = DCM_INIT;

  /* Reset the security event status */
  for(lThreadIter = 0; lThreadIter < DCM_NUM_THREADS; ++lThreadIter)
  {
    Dcm_DebugResetSecurityEventStatus(lThreadIter);
  }
}

/**********************************************************************************************************************
 *  Dcm_DebugReportSecurityEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DebugReportSecurityEvent(
  Dcm_ThreadIdOptType threadId,
  uint16 idsmEvent
  )
{
#if (DCM_DEBUG_SECURITY_EVENT_REPORT_ENABLED == STD_ON)
  Dcm_DebugThreadContextPtrType pThreadContext;

  pThreadContext = Dcm_DebugGetThreadContext(threadId);

  if (pThreadContext->IdsMEventTriggered == FALSE)
  {
    if (idsmEvent != DCM_DEBUG_SEV_INVALID)
    {
      IdsM_SetSecurityEvent(idsmEvent);
      pThreadContext->IdsMEventTriggered = TRUE;                                                                                                     /* SBSW_DCM_POINTER_WRITE_DEBUGTHREADCONTEXT */
    }
  }
#else
  DCM_IGNORE_UNREF_PARAM(threadId);
  DCM_IGNORE_UNREF_PARAM(idsmEvent);
#endif
}

/**********************************************************************************************************************
 *  Dcm_DebugResetSecurityEventStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DebugResetSecurityEventStatus(
  Dcm_ThreadIdOptType threadId
  )
{
#if (DCM_DEBUG_SECURITY_EVENT_REPORT_ENABLED == STD_ON)
  Dcm_DebugThreadContextPtrType pThreadContext;

  pThreadContext = Dcm_DebugGetThreadContext(threadId);

  pThreadContext->IdsMEventTriggered = FALSE;                                                                                                        /* SBSW_DCM_POINTER_WRITE_DEBUGTHREADCONTEXT */
#else
  DCM_IGNORE_UNREF_PARAM(threadId);
#endif
}

/**********************************************************************************************************************
 *  Dcm_DebugReportSecurityEventNRC()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DebugReportSecurityEventNRC(
  Dcm_ThreadIdOptType threadId,
  Dcm_NegativeResponseCodeType ErrorCode
  )
{
#if (DCM_DEBUG_SECURITY_EVENT_REPORT_ENABLED == STD_ON)
  switch (ErrorCode)
  {
  case DCM_E_SERVICENOTSUPPORTED:
  case DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION:
    Dcm_DebugReportSecurityEvent(threadId, DCM_DEBUG_SEV_SERVICE_NOT_SUPPORTED);
    break;

  case DCM_E_SUBFUNCTIONNOTSUPPORTED:
  case DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION:
    Dcm_DebugReportSecurityEvent(threadId, DCM_DEBUG_SEV_SUBFUNCTION_NOT_SUPPORTED);
    break;

  case DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT:
    Dcm_DebugReportSecurityEvent(threadId, DCM_DEBUG_SEV_INCORRECT_MESSAGE_LENGTH_OR_FORMAT);
    break;

  case DCM_E_REQUESTSEQUENCEERROR:
    Dcm_DebugReportSecurityEvent(threadId, DCM_DEBUG_SEV_REQUEST_SEQUENCE_ERROR);
    break;

  case DCM_E_REQUESTOUTOFRANGE:
    Dcm_DebugReportSecurityEvent(threadId, DCM_DEBUG_SEV_REQUEST_OUT_OF_RANGE);
    break;

  case DCM_E_SECURITYACCESSDENIED:
    Dcm_DebugReportSecurityEvent(threadId, DCM_DEBUG_SEV_SECURITY_ACCESS_DENIED);
    break;

  case DCM_E_INVALIDKEY:
    Dcm_DebugReportSecurityEvent(threadId, DCM_DEBUG_SEV_SECURITY_ACCESS_INVALID_KEY);
    break;

  case DCM_E_EXCEEDNUMBEROFATTEMPTS:
    Dcm_DebugReportSecurityEvent(threadId, DCM_DEBUG_SEV_SECURITY_ACCESS_NUMBER_OF_ATTEMPTS_EXCEEDED);
    break;

  case DCM_E_REQUIREDTIMEDELAYNOTEXPIRED:
    Dcm_DebugReportSecurityEvent(threadId, DCM_DEBUG_SEV_SECURITY_ACCESS_REQUIRED_TIME_DELAY_NOT_EXPIRED);
    break;

  case DCM_E_AUTHENTICATIONREQUIRED:
    Dcm_DebugReportSecurityEvent(threadId, DCM_DEBUG_SEV_REQUESTED_ACTIONS_REQUIRES_AUTHENTICATION);
    break;

  default:
    /* Otherwise - proceed with negative response transmission */
    break;
  }
#else
  DCM_IGNORE_UNREF_PARAM(threadId);
  DCM_IGNORE_UNREF_PARAM(ErrorCode);
#endif
}                                                                                                                                                    /* PRQA S 6030 */ /* MD_MSR_STCYC */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Debug.c
 *********************************************************************************************************************/
