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
/**        \file  Dcm_Svc14.c
 *         \unit  Svc14
 *        \brief  Contains the implementation of Service 0x14 unit.
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
#define DCM_SVC14_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc14.h"
#if (DCM_SVC_14_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Repeater.h"
# include "Dcm_Debug.h"
# include "Dcm_DemIf.h"
# include "Dcm_AuthMgr.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Service 0x14 processing steps */
# define DCM_SVC14_PROGRESS_REPEATERPROXY_SELECT_DTC                 ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
# define DCM_SVC14_PROGRESS_REPEATERPROXY_CHECK_SELECTION_RESULT     ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u))
# define DCM_SVC14_PROGRESS_REPEATERPROXY                            ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 2u))

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc14Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x14 Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Execute next repeater stage immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc14Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc14RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc14_XX_RepeaterProxy()
 *********************************************************************************************************************/
/*! \brief          Service 0x14 worker.
 *  \details        Calls the ClearDTC function.
 *  \param[in]      opStatus                The operations status
 *  \param[in]      pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send positive response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc14_XX_RepeaterProxy(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc14RepeaterProxyContextPtrType pRepContext
  );

# if (DCM_DEMIF_SELECT_DTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc14_XX_RepeaterProxySelectDTC()
 *********************************************************************************************************************/
/*! \brief          Service 0x14 worker.
 *  \details        Calls the SelectDTC function.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in]      pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_PROCESSINGDONE    Send response
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc14_XX_RepeaterProxySelectDTC(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc14RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_DEMIF_SELECT_DTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc14_XX_RepeaterProxyCheckSelectionResult()
 *********************************************************************************************************************/
/*! \brief          Service 0x14 worker.
 *  \details        Calls the GetDTCSelectionResult function.
 *  \param[in]      opStatus                The operations status
 *  \param[in]      pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_PROCESSINGDONE    Send response
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc14_XX_RepeaterProxyCheckSelectionResult(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc14RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc14Handler()
 *********************************************************************************************************************/
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
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc14Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc14RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  /* Store requested DTC for asynchronous processing */
  Dcm_UtiConsumeReqDataAsU24(pMsgContext, &(pRepContext->Dtc));                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(pMsgContext->reqDataLen == 0u)
  {
    pRepContext->UserMemId = DEM_DTC_ORIGIN_PRIMARY_MEMORY;                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
  }
# if (DCM_SVC_14_USER_MEM_ID_ENABLED == STD_ON)
  else if (pMsgContext->reqDataLen == 1u)
  {
    uint8 lUserMemIdWithoutOffset;

    /* Store Memory Id */
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &(lUserMemIdWithoutOffset));                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
#  if (DCM_SVC_14_USER_MEM_ID_CHECK_ENABLED == STD_ON)
    if (lUserMemIdWithoutOffset != DCM_SVC_14_USER_MEM_ID)
    {
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
       lStdResult = DCM_E_NOT_OK;
    }
    else
#  endif
#  if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
    if (Dcm_AuthMgrCheckMem(pContext, DCM_SVC_14_USERMEM_ID_PRECON_REF, lUserMemIdWithoutOffset) == DCM_E_NOT_OK)                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
    {
      *ErrorCode = DCM_E_AUTHENTICATIONREQUIRED;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
    else
#  endif
    {
      /* Add Offset to UserMemId */
      pRepContext->UserMemId = lUserMemIdWithoutOffset + 0x100u;                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
# endif
  else
  {
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  if(lStdResult == DCM_E_OK)
  {
    pRepContext->DemClientId = Dcm_DiagGetDemClientId(pMsgContext);                                                                                  /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_WRITE */ /* SBSW_DCM_PARAM_PTR_FORWARD */

# if (DCM_DEMIF_SELECT_DTC_ENABLED == STD_ON)
    Dcm_RepeaterNextStep(pContext, DCM_SVC14_PROGRESS_REPEATERPROXY_SELECT_DTC);                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
# else
    Dcm_RepeaterNextStep(pContext, DCM_SVC14_PROGRESS_REPEATERPROXY);                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
# endif

    lStdResult = DCM_E_LOOP;/* speed up processing */
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc14_XX_RepeaterProxy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc14_XX_RepeaterProxy(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc14RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType              lStdResult;
  Dcm_DemIfReturnClearDTCType lResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

# if (DCM_DEMIF_421_ENABLED == STD_ON)
  lResult = Dcm_DemIf(ClearDTC)(pRepContext->Dtc
                               ,DEM_DTC_FORMAT_UDS
                               ,DEM_DTC_ORIGIN_PRIMARY_MEMORY);
# else
  lResult = Dcm_DemIf(ClearDTC)(pRepContext->DemClientId);
# endif

  switch (lResult)
  {
  case DCM_DEMIF_RET_CLEAR_OK:
    lStdResult = DCM_E_OK;
    Dcm_DebugReportSecurityEvent(pMsgContext->threadId, DCM_DEBUG_SEV_CLEAR_DTC_SUCCESSFUL);
    break;
  case DCM_DEMIF_RET_CLEAR_PENDING:
    lStdResult = DCM_E_PENDING;
    break;
  case DCM_DEMIF_RET_CLEAR_MEMORY_ERROR:
    /* The only different NRC compared to service 0x04 */
    *ErrorCode = DCM_E_GENERALPROGRAMMINGFAILURE;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  default:
    *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(lResult, Dcm_DemIfNrcMapClearDTC);                                                                    /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }

  return lStdResult;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_DEMIF_SELECT_DTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc14_XX_RepeaterProxySelectDTC()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc14_XX_RepeaterProxySelectDTC(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc14RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;
  Std_ReturnType lDemResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lDemResult = Dcm_DemIf(SelectDTC)(pRepContext->DemClientId
                                   ,pRepContext->Dtc
                                   ,DEM_DTC_FORMAT_UDS
                                   ,pRepContext->UserMemId);

  switch (lDemResult)
  {
  case E_OK:
    lStdResult = DCM_E_LOOP;
    Dcm_RepeaterNextStep(pContext, DCM_SVC14_PROGRESS_REPEATERPROXY_CHECK_SELECTION_RESULT);                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_DEMIF_RET_SELECT_BUSY:
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  default:
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
    break;
  }

  return lStdResult;
}
# endif

# if (DCM_DEMIF_SELECT_DTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc14_XX_RepeaterProxyCheckSelectionResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc14_XX_RepeaterProxyCheckSelectionResult(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc14RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType              lStdResult;
  Dcm_DemIfReturnClearDTCType lResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lResult = Dcm_DemIf(GetDTCSelectionResult)(pRepContext->DemClientId);

  switch (lResult)
  {
  case DCM_DEMIF_RET_CLEAR_OK:
    lStdResult = DCM_E_LOOP;
    Dcm_RepeaterNextStep(pContext, DCM_SVC14_PROGRESS_REPEATERPROXY);                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_DEMIF_RET_CLEAR_PENDING:
    lStdResult = DCM_E_PENDING; /* try again later */
    break;
  case DCM_DEMIF_RET_CLEAR_W_DTC:
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  case DCM_DEMIF_RET_CLEAR_BUSY:
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  case DCM_DEMIF_RET_CLEAR_W_ORIGIN:
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  default:
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
    break;
  }

  return lStdResult;
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Service14Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service14Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc14RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc14;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc14Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# if (DCM_DEMIF_SELECT_DTC_ENABLED == STD_ON)
  case DCM_SVC14_PROGRESS_REPEATERPROXY_SELECT_DTC:
    lStdResult = Dcm_Svc14_XX_RepeaterProxySelectDTC(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                       /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC14_PROGRESS_REPEATERPROXY_CHECK_SELECTION_RESULT:
    lStdResult = Dcm_Svc14_XX_RepeaterProxyCheckSelectionResult(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                            /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
  case DCM_SVC14_PROGRESS_REPEATERPROXY:
    lStdResult = Dcm_Svc14_XX_RepeaterProxy(opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}

/**********************************************************************************************************************
*  Dcm_Service14Cancel()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service14Cancel(
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  Dcm_DemIfCancelOperation();
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_14_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc14.c
 *********************************************************************************************************************/
