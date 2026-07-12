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
/**        \file  Dcm_Svc28.c
 *         \unit  Svc28
 *        \brief  Contains the implementation of Service 0x28 unit.
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
#define DCM_SVC28_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc28.h"
#if (DCM_SVC_28_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Repeater.h"
# include "Dcm_Net.h"
# include "Dcm_Debug.h"
# include "Dcm_Uti.h"
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
/*! Service 0x28 processing steps */
# define DCM_SVC28_PROGRESS_SUBFUNCTION                              ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))

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
 *  Dcm_Svc28Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x28 Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc28Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc28RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc28SubFuncHandler()
 *********************************************************************************************************************/
/*! \brief          Service 0x28 SubFunction Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc28SubFuncHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc28RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc28ApplyCommControl()
 *********************************************************************************************************************/
/*! \brief          Apply concrete operation on network channel(s).
 *  \details        -
 *  \param[in]      networkRef The network handle
 *  \param[in]      mode       The communication mode
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc28ApplyCommControl(
  Dcm_CfgNetNetIdRefMemType networkRef,
  Dcm_CommunicationModeType mode
  );
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc28ReqLengthGetter()
 *********************************************************************************************************************/
/*! \brief          Service 0x28 sub-function length getter.
 *  \details        Always returns the sub-function specific length.
 *  \param[in]      sfRef                The sub-function zero-based index.
 *  \return         Expected sub-function specific length.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc28ReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
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
 *  Dcm_Svc28Handler()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc28Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc28RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType                lStdResult;
  Dcm_DiagSubServiceRefOptType  lSubSvcRef;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Prepare all generic part (independently of the SF validation result!) */
  pRepContext->IsInternallyProcessed = FALSE;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
  pRepContext->SubServiceId = Dcm_UtiGetReqDataAsU8(pMsgContext);                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */ /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

  lStdResult = Dcm_DiagVerifyServiceWithSubFunction(pMsgContext
                                                   ,Dcm_CfgSvc28SubFuncLookUpTable
                                                   ,Dcm_CfgSvc28SubFuncExecPrecondTable
                                                   ,Dcm_Svc28ReqLengthGetter
                                                   ,Dcm_DiagNoSequenceChecker
                                                   ,&lSubSvcRef
                                                   ,ErrorCode);                                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */

  pRepContext->SubSvcRef = (Dcm_DiagSubServiceRefMemType)lSubSvcRef;                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */

  if (lStdResult == DCM_E_OK)
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC28_PROGRESS_SUBFUNCTION); /* delegate job */                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;/* speed up processing */
  } /* else DCM_E_NOT_OK (ErrorCode is already set) */

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc28SubFuncHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc28SubFuncHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc28RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                 lStdResult;
  Dcm_CfgSvc28SubFuncInfoPtrType pSubFuncInfo;

  lStdResult = Dcm_CfgWrapSvc28SubFuncInfoGetEntry(pRepContext->SubSvcRef, &pSubFuncInfo, ErrorCode);                                                /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lStdResult == DCM_E_OK)                                                                                                                        /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    lStdResult = pSubFuncInfo->SubSvcFunc(pContext, opStatus, pMsgContext, ErrorCode);                                                               /* SBSW_DCM_CALL_FUNCPTR_SVC28SUBFUNC */
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc28ApplyCommControl()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc28ApplyCommControl(
  Dcm_CfgNetNetIdRefMemType networkRef,
  Dcm_CommunicationModeType mode
  )
{
  DCM_IGNORE_UNREF_PARAM(networkRef);                                                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

# if (DCM_SVC_28_CURNET_SUPPORT_ENABLED == STD_ON) && \
     (DCM_NET_COMCTRL_ALLNET_SUPPORT_ENABLED == STD_ON)
  if (networkRef == DCM_SVC_28_NETWORK_ALL)
# endif
  {
# if (DCM_NET_COMCTRL_ALLNET_SUPPORT_ENABLED == STD_ON)
    uint8_least lChannelIter;

    for (lChannelIter = Dcm_PbCfgNetGetComCtrlChannelListAll(0); lChannelIter != 0u; --lChannelIter)
    {
      Dcm_ModeSwitchCommControl(Dcm_PbCfgNetGetComCtrlChannelListAll(lChannelIter), mode);
    }
# endif
  }
# if (DCM_SVC_28_CURNET_SUPPORT_ENABLED == STD_ON) && \
     (DCM_NET_COMCTRL_ALLNET_SUPPORT_ENABLED == STD_ON)
  else
# endif
  {
# if (DCM_SVC_28_CURNET_SUPPORT_ENABLED == STD_ON)
    Dcm_ModeSwitchCommControl(networkRef, mode);
# endif
  }
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc28ReqLengthGetter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc28ReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  )
{
  /* Return sub-function specific length */
  return (Dcm_CfgWrapSvc28SubFuncInfo(sfRef))->ReqLength;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc28EnableCommunication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc28EnableCommunication(
  void
  )
{
  uint8_least channelIter = Dcm_PbCfgNetGetNumAllComMChannels();

  while (channelIter != 0u) /* Use top-down counting to be consistent with the Dcm_Svc28ApplyCommControl all-channel calls */
  {
    --channelIter;
    if (Dcm_PbRamNetGetComCtrlChannels(channelIter) != DCM_ENABLE_RX_TX_NORM_NM)
    {
      Dcm_ModeSwitchCommControl(channelIter, DCM_ENABLE_RX_TX_NORM_NM);
    }
  }
}

/**********************************************************************************************************************
 *  Dcm_Svc28OnChangeSession()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc28OnChangeSession(
  void
  )
{
  sint16_least lSidRef = Dcm_UtiLookUpUint8(Dcm_CfgDiagSvcIdLookUpTable, 0x28u);                                                                     /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

  if ((Dcm_StateIsDefaultSessionActive())
    || (!Dcm_StateIsSupportedInSession(Dcm_StateGetPreconditionStates(Dcm_CfgWrapDiagSvcIdExecPrecondTable(lSidRef)))))                              /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
  {
    Dcm_Svc28EnableCommunication();
  }
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Service28_XXProcessor()
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
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service28_XXProcessor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;
  uint8   msgType;
  boolean validParam = TRUE;
  Dcm_Svc28RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc28;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Extract and validate the requested network type (i.e. request data byte[0], high nibble) */
  {
    sint16_least lookUpResult;
    lookUpResult = Dcm_UtiLookUpUint8(Dcm_CfgSvc28SubNetIdLookUp, (uint8)(Dcm_UtiGetReqDataAsU8(pMsgContext) >> 4u));                                /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

    if (lookUpResult >= 0)
    {
      pRepContext->NetworkRef = Dcm_CfgWrapSvc28SubNetIdMap(lookUpResult);                                                                           /* SBSW_DCM_GLOBAL_PTR_WRITE */
# if (DCM_SVC_28_CURNET_SUPPORT_ENABLED == STD_ON)
      if (pRepContext->NetworkRef == DCM_SVC_28_NETWORK_CURRENT)
      {
        pRepContext->NetworkRef = Dcm_PbCfgNetGetConnComMChannelMap(Dcm_PbCfgNetGetConnectionInfo(Dcm_DiagGetTranspObj(pContext)->ConnHdl).NetworkIdRef); /* SBSW_DCM_GLOBAL_PTR_WRITE */
      }
# endif
    }
    else
    {
      validParam = FALSE;
    }
  }

  /* Extract and validate the requested message type (i.e. request data byte[0], low nibble) */
  {
    msgType = (uint8)(Dcm_UtiGetReqDataAsU8(pMsgContext) & 0x0Fu);                                                                                   /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

    if (Dcm_UtiLookUpUint8(Dcm_CfgSvc28MessageTypeLookUpTable, msgType) < 0)                                                                         /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
    {
      validParam = FALSE;
    }
  }

  if (validParam == TRUE)
  {
    /*--------------------------------------------------------------
    Message types      |   Operations                |ComParamValue
    (Affected = 1,     |                             |
    Not affected = 0)  | (Enable = 0, Disable = 1)   |
    ------+------------+-----------------+-----------+
      NM  |    Appl    |    RxPath       |  TxPath   |
    ------+------------+-----------------+-----------+--------------
      0   |     0      |        x        |    x      |    -
      0   |     1      |        0        |    0      |    0
      0   |     1      |        0        |    1      |    1
      0   |     1      |        1        |    0      |    2
      0   |     1      |        1        |    1      |    3
      1   |     0      |        0        |    0      |    4
      1   |     0      |        0        |    1      |    5
      1   |     0      |        1        |    0      |    6
      1   |     0      |        1        |    1      |    7
      1   |     1      |        0        |    0      |    8
      1   |     1      |        0        |    1      |    9
      1   |     1      |        1        |    0      |    10
      1   |     1      |        1        |    1      |    11
    ------+------------+-----------------+-----------+--------------
    => ModeValue := ((msgType) & 0x03) << 2) | ((RxPath) & 0x01) << 1) | ((TxPath) & 0x01) ) ) - 4
    SubFunc == ((RxPath) & 0x01) << 1) | ((TxPath) & 0x01);
    => ModeValue := ((msgType) & 0x03) << 2) | (SubFunc) ) - 4
    */
    pRepContext->ComMode = (Dcm_CommunicationModeType)((((msgType & 0x03u) << 2u) | (pRepContext->SubServiceId & DCM_SVC_28_CTRL_OP_MASK)) - 4u);    /* SBSW_DCM_GLOBAL_PTR_WRITE */

    /* Mark current service execution as internally handled to perform the post-processing step later on */
    pRepContext->IsInternallyProcessed = TRUE;                                                                                                       /* SBSW_DCM_GLOBAL_PTR_WRITE */
  }
  else
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Service28Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service28Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                lStdResult;
  Dcm_Svc28RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc28;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc28Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC28_PROGRESS_SUBFUNCTION:
    lStdResult = Dcm_Svc28SubFuncHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                   /* SBSW_DCM_COMB_PTR_FORWARD */
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
*  Dcm_Service28PostProcessor()
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
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service28PostProcessor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_ConfirmationStatusType status
  )
{
  if (status == DCM_RES_POS_OK)
  {
    if ((pContext->Repeater.Context.Svc28.SubServiceId == 0x01u)
      || (pContext->Repeater.Context.Svc28.SubServiceId == 0x02u)
      || (pContext->Repeater.Context.Svc28.SubServiceId == 0x03u))
    {
      Dcm_DebugReportSecurityEvent(pContext->ThreadId, DCM_DEBUG_SEV_COMMUNICATION_CONTROL_SWITCHED_OFF);
    }

    if (pContext->Repeater.Context.Svc28.IsInternallyProcessed == TRUE)
    {
      Dcm_Svc28ApplyCommControl(pContext->Repeater.Context.Svc28.NetworkRef,
        pContext->Repeater.Context.Svc28.ComMode);
    }
  }
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_28_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc28.c
 *********************************************************************************************************************/
