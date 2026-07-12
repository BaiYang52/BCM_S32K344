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
/**        \file  Dcm_Svc85.c
 *         \unit  Svc85
 *        \brief  Contains the implementation of Service 0x85 unit.
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
#define DCM_SVC85_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc85.h"
#if (DCM_SVC_85_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Debug.h"
# include "Dcm_Uti.h"
# include "Dcm_Repeater.h"
# include "Dcm_Mode.h"
# include "Dcm_State.h"
# include "Dcm_CfgWrap.h"

# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Service 0x85 processing steps */
# define DCM_SVC85_PROGRESS_REPEATERPROXY                            ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))

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
 *  Dcm_Svc85Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x85 Handler.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc85Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc85RepeaterProxyContextPtrType pRepContext
  );
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc85ReqLengthGetter()
 *********************************************************************************************************************/
/*! \brief          Service 0x85 sub-function length getter.
 *  \details        Always returns length of 1 or 4 byte depending on whether the optional DTC group is supported.
 *  \param[in]      sfRef                The sub-function zero-based index.
 *  \return         Expected sub-function specific length.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc85ReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  );

/**********************************************************************************************************************
 *  Dcm_Svc85_XX_RepeaterProxy()
 *********************************************************************************************************************/
/*! \brief          Service 0x85 worker.
 *  \details        Calls the Enable/DisableDTCSetting function.
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc85_XX_RepeaterProxy(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc85RepeaterProxyContextPtrType pRepContext
  );
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
 *  Dcm_Svc85Handler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc85Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc85RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType                lStdResult;
  Dcm_DiagSubServiceRefOptType  lSubSvcRef;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_DiagVerifyServiceWithSubFunction(pMsgContext
                                                   ,Dcm_CfgSvc85SubFuncLookUpTable
                                                   ,Dcm_CfgSvc85SubFuncExecPrecondTable
                                                   ,Dcm_Svc85ReqLengthGetter
                                                   ,Dcm_DiagNoSequenceChecker
                                                   ,&lSubSvcRef
                                                   ,ErrorCode);                                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)
  {
    pRepContext->SubSvcRef   = (Dcm_DiagSubServiceRefMemType)lSubSvcRef;                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->ReqMode     = Dcm_CfgWrapSvc85SubFuncInfo(pRepContext->SubSvcRef)->Mode;                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->DemClientId = Dcm_DiagGetDemClientId(pMsgContext);                                                                                  /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_GLOBAL_PTR_WRITE */ /* SBSW_DCM_PARAM_PTR_FORWARD */

# if (DCM_SVC_85_DTC_GRP_ENABLED == STD_ON)
    Dcm_UtiConsumeReqDataAsU24(pMsgContext, &pRepContext->DTCGroup);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
#  if (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
    if (pRepContext->DTCGroup != DEM_DTC_GROUP_ALL_DTCS)
    {
      lStdResult = E_NOT_OK;
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    /* else: everything fine -> proceed with service processing */
#  endif
# else
    /* length = 0 -> nothing to extract */
    pRepContext->DTCGroup = DEM_DTC_GROUP_ALL_DTCS;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
# endif
# if (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
    if (lStdResult == DCM_E_OK)                                                                                                                      /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */
# endif
    {
      Dcm_RepeaterNextStep(pContext, DCM_SVC85_PROGRESS_REPEATERPROXY);                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_LOOP; /* speed up processing */
    }
  }
  return lStdResult;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc85ReqLengthGetter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc85ReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  )
{
  DCM_IGNORE_UNREF_PARAM(sfRef);                                                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  /* Return sub-function specific length (always the same constant) */
  return DCM_SVC_85_REQ_LENGTH;
}

/**********************************************************************************************************************
 *  Dcm_Svc85_XX_RepeaterProxy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc85_XX_RepeaterProxy(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc85RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                       lStdResult;
  Dcm_DemIfReturnControlDTCSettingType lDemResult;
  Dcm_CfgSvc85SubFuncInfoPtrType       pSubFuncInfo;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_CfgWrapSvc85SubFuncInfoGetEntry(pRepContext->SubSvcRef, &pSubFuncInfo, ErrorCode);                                                /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lStdResult == DCM_E_OK)                                                                                                                        /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
# if (DCM_DEMIF_421_ENABLED == STD_ON)
    lDemResult = pSubFuncInfo->DemFunc(pRepContext->DTCGroup, DEM_DTC_KIND_ALL_DTCS);                                                                /* SBSW_DCM_CALL_FUNCPTR_SVC85SUBFUNC */
# else
    lDemResult = pSubFuncInfo->DemFunc(pRepContext->DemClientId);                                                                                    /* SBSW_DCM_CALL_FUNCPTR_SVC85SUBFUNC */
# endif
    switch(lDemResult)
    {
    case DCM_DEMIF_RET_CONTROL_DTC_SETTING_OK:
      /* success */
      lStdResult = DCM_E_OK;
      break;
# if defined (DCM_DEMIF_RET_CONTROL_DTC_SETTING_N_OK)
    case DCM_DEMIF_RET_CONTROL_DTC_SETTING_N_OK:
      *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      break;
# endif
# if defined (DCM_DEMIF_RET_CONTROL_DTC_WRONG_DTCGROUP)
    case DCM_DEMIF_RET_CONTROL_DTC_WRONG_DTCGROUP:
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      break;
# endif
# if defined (DCM_DEMIF_RET_CONTROL_DTC_SETTING_PENDING)
    case DCM_DEMIF_RET_CONTROL_DTC_SETTING_PENDING:
      lStdResult = DCM_E_PENDING;
      break;
# endif
    default:
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
      lStdResult = DCM_E_NOT_OK;
      break;
    }
  }

  return lStdResult;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc85DisableDtcSetting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc85DisableDtcSetting(
  Dem_DTCGroupType DTCGroup,
  uint8 DemClientId
  )
{
  Std_ReturnType                       lStdResult;
  Dcm_DemIfReturnControlDTCSettingType lDemResult;

  /* Disable the DTC setting for all DTCs */
#  if (DCM_DEMIF_421_ENABLED == STD_ON)
  lDemResult = Dcm_DemIf(DisableDTCSetting)(DTCGroup, DEM_DTC_KIND_ALL_DTCS);
#  else
  lDemResult = Dcm_DemIf(DisableDTCSetting)(DemClientId);
#  endif

  switch (lDemResult)
  {
    case DCM_DEMIF_RET_CONTROL_DTC_SETTING_OK:
      lStdResult = DCM_E_OK;
      Dcm_ModeSwitchDtcSetting(RTE_MODE_DcmControlDtcSetting_DISABLEDTCSETTING, DTCGroup, DemClientId);
      break;
#  if defined (DCM_DEMIF_RET_CONTROL_DTC_SETTING_PENDING)
    case DCM_DEMIF_RET_CONTROL_DTC_SETTING_PENDING:
      lStdResult = DCM_E_PENDING;
      break;
#  endif
    default:
      lStdResult = DCM_E_NOT_OK;
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
      break;
  }
  return lStdResult;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc85EnableDtcSetting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc85EnableDtcSetting(
  uint8 DemClientId
  )
{
  Std_ReturnType                       lStdResult;
  Dcm_DemIfReturnControlDTCSettingType lDemResult;

  if(Dcm_ModeGetControlDtcSettingMode() != RTE_MODE_DcmControlDtcSetting_ENABLEDTCSETTING)
  {
    /* Enable the DTC setting for all DTCs */
# if (DCM_DEMIF_421_ENABLED == STD_ON)
    lDemResult = Dcm_DemIf(EnableDTCSetting)(DEM_DTC_GROUP_ALL_DTCS, DEM_DTC_KIND_ALL_DTCS);
# else
    lDemResult = Dcm_DemIf(EnableDTCSetting)(DemClientId);
# endif

    switch (lDemResult)
    {
      case DCM_DEMIF_RET_CONTROL_DTC_SETTING_OK:
        lStdResult = DCM_E_OK;
        Dcm_ModeSwitchDtcSetting(RTE_MODE_DcmControlDtcSetting_ENABLEDTCSETTING, DEM_DTC_GROUP_ALL_DTCS, DemClientId);
        break;
# if defined (DCM_DEMIF_RET_CONTROL_DTC_SETTING_PENDING)
      case DCM_DEMIF_RET_CONTROL_DTC_SETTING_PENDING:
        lStdResult = DCM_E_PENDING;
        break;
# endif
      default:
        lStdResult = DCM_E_NOT_OK;
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
        break;
    }
  }
  else
  {
    lStdResult = DCM_E_OK;
  }
  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc85OnChangeSession()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc85OnChangeSession(
  void
  )
{
  sint16_least lSidRef = Dcm_UtiLookUpUint8(Dcm_CfgDiagSvcIdLookUpTable, 0x85u);                                                                     /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

  if ((Dcm_StateIsDefaultSessionActive())
     ||(!Dcm_StateIsSupportedInSession(Dcm_StateGetPreconditionStates(Dcm_CfgWrapDiagSvcIdExecPrecondTable(lSidRef)))))                              /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
  {
    /* Enable DTC setting */
    (void)Dcm_Svc85EnableDtcSetting(Dcm_ModeGetLastDisableDTCClientID());
  }
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Service85Processor()
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
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service85Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc85RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc85;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc85Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC85_PROGRESS_REPEATERPROXY:
    lStdResult = Dcm_Svc85_XX_RepeaterProxy(opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
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
 *  Dcm_Service85PostProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service85PostProcessor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_ConfirmationStatusType status
  )
{
  /* If a positive response was (successfully) sent: */
  if((status & DCM_DIAG_RES_NEG_ANY) == 0u)
  {
    if (pContext->Repeater.Context.Svc85.ReqMode == RTE_MODE_DcmControlDtcSetting_DISABLEDTCSETTING) /* Disable DTC */
    {
      Dcm_DebugReportSecurityEvent(pContext->ThreadId, DCM_DEBUG_SEV_DTC_SETTING_SWITCHED_OFF);
    }
    /* Trigger the requested mode switch */
    Dcm_ModeSwitchDtcSetting(pContext->Repeater.Context.Svc85.ReqMode
                            ,pContext->Repeater.Context.Svc85.DTCGroup
                            ,pContext->Repeater.Context.Svc85.DemClientId);
  }
}

/**********************************************************************************************************************
 *  Dcm_Service85Cancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service85Cancel(
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  Dcm_DemIfCancelOperation();
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_85_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc85.c
 *********************************************************************************************************************/
