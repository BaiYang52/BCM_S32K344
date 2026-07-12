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
/**        \file  Dcm_Svc11.c
 *         \unit  Svc11
 *        \brief  Contains the implementation of Service 0x11 unit.
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
#define DCM_SVC11_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc11.h"
#if (DCM_SVC_11_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Repeater.h"
# include "Dcm_Net.h"
# include "Dcm_CfgWrap.h"
# include "Dcm_Debug.h"
# include "Dcm_Uti.h"
# include "Dcm_Mode.h"
# if (DCM_SVC_11_RST2BOOT_HIS_ENABLED == STD_ON)
#  include "Dcm_Tmr.h"
# endif
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Service 0x11 processing steps */
# define DCM_SVC11_PROGRESS_SUBFUNCTION                              ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
# define DCM_SVC11_PROGRESS_ECURST_WAITFORACK                        ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u))
# define DCM_SVC11_RAPPWRDWN_WAITFORACK                              ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 2u))
# define DCM_SVC11_PROGRESS_ECURST_RCRRP_ACK                         ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 3u))
# define DCM_SVC11_PROGRESS_ECURST_SET_PRGCOND                       ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 4u))

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
 *  Dcm_Svc11Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x11 Handler.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc11Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc11RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc11SubFuncHandler()
 *********************************************************************************************************************/
/*! \brief          Service 0x11 SubFunction Handler.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc11SubFuncHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc11RepeaterProxyContextPtrType pRepContext
  );

# if (DCM_SVC_11_ECU_RESET_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc11_EcuRst_WaitForAck()
 *********************************************************************************************************************/
/*! \brief          Service 0x11 repeater.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in]      pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc11_EcuRst_WaitForAck(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc11RepeaterProxyContextPtrType pRepContext
  );
# endif

# if ((DCM_SVC_11_ECU_RESET_ENABLED == STD_ON) && (DCM_SVC_11_RST2BOOT_HIS_ENABLED == STD_ON))
/**********************************************************************************************************************
 *  Dcm_Svc11_WaitForRcrRpAck()
 *********************************************************************************************************************/
/*! \brief          Wait for RCR-RP acknowledgment.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in]      pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_LOOP              Execute next repeater stage immediately
 *  \return         DCM_E_NOT_OK            Operation failed, send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc11_WaitForRcrRpAck(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc11RepeaterProxyContextPtrType pRepContext
  );
# endif

# if ((DCM_SVC_11_ECU_RESET_ENABLED == STD_ON) && (DCM_SVC_11_RST2BOOT_HIS_ENABLED == STD_ON))
/**********************************************************************************************************************
 *  Dcm_Svc11_SetProgConditions()
 *********************************************************************************************************************/
/*! \brief          Sets programming conditions.
 *  \details        -
 *  \param[in]      opStatus                The operations status
 *  \param[in]      pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_STOP_REPEATER     Trigger to stop repeater (programming conditions set)
 *  \return         DCM_E_NOT_OK            Operation failed, send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc11_SetProgConditions(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc11RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_11_RAPID_SHTDWN_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc11_RapPwrDwn_WaitForAck()
 *********************************************************************************************************************/
/*! \brief          Service 0x11 repeater.
 *  \details        -
 *  \param[in]      opStatus                The operations status
 *  \param[in]      pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc11_RapPwrDwn_WaitForAck(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc11RepeaterProxyContextPtrType pRepContext
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_SVC_11_ECU_RESET_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc11_ProcessEcuReset()
 *********************************************************************************************************************/
/*! \brief          Service 0x11 EcuReset mode switching management.
 *  \details        Manages the mode switches for service 0x11. If something goes wrong, an appropriate NRC will be sent.
 *  \param[in]      pContext            Pointer to the context
 *  \param[in]      mode                The ECU reset mode to be switched into
 *  \param[out]     ErrorCode           The NRC
 *  \param[in,out]  pRepContext         Current repeater proxy context
 *  \return         DCM_E_NOT_OK        Send response (negative)
 *  \return         DCM_E_LOOP          Process immediately with next step
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc11_ProcessEcuReset(
  Dcm_ContextPtrType pContext,
  Rte_ModeType_DcmEcuReset mode,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc11RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_11_RAPID_SHTDWN_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc11_ProcessRapPwrDwn()
 *********************************************************************************************************************/
/*! \brief          Service 0x11 RapidPowerShutDown mode switching management.
 *  \details        Manages the mode switches for service 0x11. If something goes wrong, an appropriate NRC will be sent.
 *  \param[in]      pContext            Pointer to the context
 *  \param[in]      mode                The ECU reset mode to be switched into
 *  \param[out]     ErrorCode           The NRC
 *  \return         DCM_E_NOT_OK        Send response (negative)
 *  \return         DCM_E_LOOP          Process immediately with next step
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc11_ProcessRapPwrDwn(
  Dcm_ContextPtrType pContext,
  Rte_ModeType_DcmModeRapidPowerShutDown mode,
  Dcm_NegativeResponseCodePtrType ErrorCode
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
 *  Dcm_Svc11Handler()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc11Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc11RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  Dcm_DiagSubServiceRefOptType  lSubSvcRef;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  pRepContext->SubSvcId = Dcm_UtiGetReqDataAsU8(pMsgContext);                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */ /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

  /* Min Length already checked in DiagDispatcher and expected to be 1 !!! */
  lStdResult = Dcm_DiagVerifyServiceWithSubFunction(pMsgContext
                                                   ,Dcm_CfgSvc11SubFuncLookUpTable
                                                   ,Dcm_CfgSvc11SubFuncExecPrecondTable
                                                   ,Dcm_DiagSvcWithOnlySubFuncReqLengthGetter
                                                   ,Dcm_DiagNoSequenceChecker
                                                   ,&lSubSvcRef
                                                   ,ErrorCode);                                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)
  {
    pRepContext->SubSvcRef = (Dcm_DiagSubServiceRefMemType)lSubSvcRef;                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */

    Dcm_RepeaterNextStep(pContext, DCM_SVC11_PROGRESS_SUBFUNCTION); /* delegate job */                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;/* speed up processing */
  } /* else DCM_E_NOT_OK (ErrorCode is already set) */

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc11SubFuncHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc11SubFuncHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc11RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                 lStdResult;
  Dcm_CfgSvc11SubFuncInfoPtrType pSubFuncInfo;

  lStdResult = Dcm_CfgWrapSvc11SubFuncInfoGetEntry(pRepContext->SubSvcRef, &pSubFuncInfo, ErrorCode);                                                /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lStdResult == DCM_E_OK)                                                                                                                        /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    lStdResult = pSubFuncInfo->SubSvcFunc(pContext, opStatus, pMsgContext, ErrorCode);                                                               /* SBSW_DCM_CALL_FUNCPTR_SVC11SUBFUNC */
  }

  return lStdResult;
}

# if (DCM_SVC_11_ECU_RESET_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc11_EcuRst_WaitForAck()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc11_EcuRst_WaitForAck(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc11RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_ModeSwitchAckEcuReset();
  switch(lStdResult)
  {
  case DCM_E_OK:
#  if (DCM_SVC_11_RST2BOOT_HIS_ENABLED == STD_ON)
    if(Dcm_CfgWrapSvc11SubFuncInfo(pRepContext->SubSvcRef)->ResponseType == DCM_SVC11_RESPONSE_AFTER_RESET)
    {
      Dcm_RepeaterNextStep(pContext, DCM_SVC11_PROGRESS_ECURST_RCRRP_ACK);                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
      if(Dcm_NetGetHasRespPendOnBoot(pContext) == TRUE)                                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
      {
        lStdResult = DCM_E_FORCE_RCRRP;
      }
      else
      {
        lStdResult = DCM_E_LOOP;
      }
    }
#  endif
    break;
  case DCM_E_PENDING:                                                                                                                                /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    /* Result already set
      lStdResult = DCM_E_PENDING; */
    break;
  default: /* any error situation */
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_11_RST2BOOT_HIS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc11_WaitForRcrRpAck()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc11_WaitForRcrRpAck(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc11RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;

  if(opStatus == DCM_FORCE_RCRRP_NOT_OK)
  {
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
  {
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    Dcm_SplitTaskEnterCS();/* avoid interrupt from the Dcm_DiagTaskTx */
    /* If positive response suppressed and no RCRRP sent */
    if(pMsgContext->msgAddInfo.suppressPosResponse != 0u)
    {
      pRepContext->ProgConditions.ResponseRequired = FALSE;                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      /* Stop to avoid inconsistency between P2 timer and the SetProgCond asynchronous API's "ResponseRequired" value */
      Dcm_TmrStopTimerByThread(DCM_TMR_ID_P2, pContext->ThreadId);
    }
    else
    {
      pRepContext->ProgConditions.ResponseRequired = TRUE;                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    Dcm_SplitTaskLeaveCS();/* avoid interrupt from the Dcm_DiagTaskTx */
    /*=================================*
      END CRITICAL SECTION
     *=================================*/

    Dcm_RepeaterNextStep(pContext, DCM_SVC11_PROGRESS_ECURST_SET_PRGCOND);                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_11_RST2BOOT_HIS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc11_SetProgConditions()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc11_SetProgConditions(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc11RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_SetProgConditions(&pRepContext->ProgConditions);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
  switch(lStdResult)
  {
  case DCM_E_OK:
    /* Reset instantly and send response afterwards */
    lStdResult = Dcm_ModeSwitchEcuReset(RTE_MODE_DcmEcuReset_EXECUTE);
    if(lStdResult == DCM_E_OK) /* fire and forget */                                                                                                 /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */
    {
      /* just wait for reset */
      lStdResult = DCM_E_STOP_REPEATER;
    }
    else
    {
      /* This code shall not be reached, since the programming conditions are already set! */
      *ErrorCode = DCM_E_PANIC_NRC; /* Use panic NRC since the first mode switch has succeeded, and this one shall too */                            /* SBSW_DCM_PARAM_PTR_WRITE */ /* PRQA S 2880 */ /* MD_MSR_Unreachable */
      lStdResult = DCM_E_NOT_OK;
    }
    break; /* send final response and if applicable - do reset */
  case DCM_E_PENDING:
    /* already set */
    break;
  case DCM_E_NOT_OK:
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  default:
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }
  return lStdResult;
}                                                                                                                                                    /* PRQA S 6030, 6050 */ /* MD_MSR_STCYC, MD_MSR_STCAL */
# endif

# if (DCM_SVC_11_RAPID_SHTDWN_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc11_RapPwrDwn_WaitForAck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc11_RapPwrDwn_WaitForAck(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc11RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_ModeSwitchAckRapidShutDown();
  switch(lStdResult)
  {
  case DCM_E_OK:
    /* switch accomplished */
    break;
  case DCM_E_PENDING:                                                                                                                                /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    /* Result already set
      lStdResult = DCM_E_PENDING; */
    break;
  default: /* any error situation */
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }
  return lStdResult;
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_SVC_11_ECU_RESET_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc11_ProcessEcuReset()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc11_ProcessEcuReset(
  Dcm_ContextPtrType pContext,
  Rte_ModeType_DcmEcuReset mode,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc11RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lResult;

  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_SVC_11_RST2BOOT_HIS_ENABLED == STD_ON)
  {
    Dcm_NetTransportObjectConstPtrType pTranspObj = Dcm_DiagGetTranspObj(pContext);

    /* Prepare programming conditions */
    pRepContext->ProgConditions.ProtocolId = Dcm_NetGetProtObjOfTranspObj(pTranspObj)->Id;                                                           /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */ /* SBSW_DCM_PARAM_PTR_WRITE */
#   if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
    pRepContext->ProgConditions.ConnectionId = Dcm_PbCfgNetGetConnectionInfo(pTranspObj->ConnHdl).ConnectionId;                                      /* SBSW_DCM_PARAM_PTR_WRITE */
#   endif
    pRepContext->ProgConditions.TesterSourceAddr = pTranspObj->ClientSrcAddr;                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->ProgConditions.Sid = 0x11u;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->ProgConditions.SubFuncId = pRepContext->SubSvcId;                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->ProgConditions.ReprogrammingRequest = FALSE;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->ProgConditions.ApplUpdated = FALSE;                                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
  }
#  endif

  /* Try performing mode switch on ECU reset */
  if(Dcm_ModeSwitchEcuReset(mode) == DCM_E_OK)                                                                                                       /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */
  {
    /* If succeeded, go on with waiting for acknowledge */
    Dcm_RepeaterNextStep(pContext, DCM_SVC11_PROGRESS_ECURST_WAITFORACK);                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
    lResult = DCM_E_LOOP;/* speed up processing */
  }
  else
  {
    /* If this attempt fails, send back NRC 0x22 to notify diagnostic client */
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */ /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    lResult = DCM_E_NOT_OK;
  }
  return lResult;
}
# endif

# if (DCM_SVC_11_RAPID_SHTDWN_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc11_ProcessRapPwrDwn()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc11_ProcessRapPwrDwn(
  Dcm_ContextPtrType pContext,
  Rte_ModeType_DcmModeRapidPowerShutDown mode,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lResult;

  /* Try performing mode switch on ECU reset */
  if(Dcm_ModeSwitchRapidShutDown(mode) == DCM_E_OK)                                                                                                  /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */
  {
    /* If succeeded, go on with waiting for acknowledge */
    Dcm_RepeaterNextStep(pContext, DCM_SVC11_RAPPWRDWN_WAITFORACK);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
    lResult = DCM_E_LOOP;/* speed up processing */
  }
  else
  {
    /* If this attempt fails, send back NRC 0x22 to notify diagnostic client */
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */ /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    lResult = DCM_E_NOT_OK;
  }

  return lResult;
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
 *  Dcm_Service11Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service11Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc11RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc11;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc11Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC11_PROGRESS_SUBFUNCTION:
    lStdResult = Dcm_Svc11SubFuncHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                   /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# if (DCM_SVC_11_ECU_RESET_ENABLED == STD_ON)
  case DCM_SVC11_PROGRESS_ECURST_WAITFORACK:
    lStdResult = Dcm_Svc11_EcuRst_WaitForAck(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                               /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
#  if (DCM_SVC_11_RST2BOOT_HIS_ENABLED == STD_ON)
  case DCM_SVC11_PROGRESS_ECURST_RCRRP_ACK:
    lStdResult = Dcm_Svc11_WaitForRcrRpAck(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                 /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC11_PROGRESS_ECURST_SET_PRGCOND:
    lStdResult = Dcm_Svc11_SetProgConditions(opStatus, pMsgContext, ErrorCode, pRepContext);                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
#  endif
# endif
# if (DCM_SVC_11_RAPID_SHTDWN_ENABLED == STD_ON)
  case DCM_SVC11_RAPPWRDWN_WAITFORACK:
    lStdResult = Dcm_Svc11_RapPwrDwn_WaitForAck(opStatus, pMsgContext, ErrorCode, pRepContext);                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Service11PostProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service11PostProcessor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_ConfirmationStatusType status
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(status);                                                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

# if (DCM_SVC_11_ECU_RESET_ENABLED == STD_ON)
  if(status == DCM_RES_POS_OK)
  {
    if(pContext->Repeater.Context.Svc11.SubSvcId < 0x04u)
    {
      Dcm_DebugReportSecurityEvent(pContext->ThreadId, DCM_DEBUG_SEV_ECU_RESET);

      /* reset only on 0x01 (hard), 0x02 (soft), 0x03 (keyOnOff) */
      (void)Dcm_ModeSwitchEcuReset(RTE_MODE_DcmEcuReset_EXECUTE);
    }
  }
# endif
}

/**********************************************************************************************************************
 *  Dcm_Service11FastPostProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service11FastPostProcessor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_ConfirmationStatusType status
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(status);                                                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

# if (DCM_SVC_11_ECU_RESET_ENABLED == STD_ON)
  if(status == DCM_RES_POS_OK)
  {
    if(pContext->Repeater.Context.Svc11.SubSvcId < 0x04u)
    {
      Dcm_NetSetRxAllowed(FALSE);
    }
  }
# endif
}

# if (DCM_SVC_11_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service11_01Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service11_01Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc11RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc11;

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Delegate job to the concrete mode manager */
  return Dcm_Svc11_ProcessEcuReset(pContext, RTE_MODE_DcmEcuReset_HARD, ErrorCode, pRepContext);                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
}
# endif
# if (DCM_SVC_11_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service11_02Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service11_02Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc11RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc11;

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Delegate job to the concrete mode manager */
  return Dcm_Svc11_ProcessEcuReset(pContext, RTE_MODE_DcmEcuReset_KEYONOFF, ErrorCode, pRepContext);                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
}
# endif
# if (DCM_SVC_11_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service11_03Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service11_03Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc11RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc11;

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Delegate job to the concrete mode manager */
  return Dcm_Svc11_ProcessEcuReset(pContext, RTE_MODE_DcmEcuReset_SOFT, ErrorCode, pRepContext);                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
}
# endif
# if (DCM_SVC_11_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service11_04Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service11_04Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Prepare for a positive response */
  Dcm_UtiProvideResDataAsU8(pMsgContext, DCM_SVC_11_POWERDOWNTIME);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* Delegate job to the concrete mode manager */
  return Dcm_Svc11_ProcessRapPwrDwn(pContext, RTE_MODE_DcmModeRapidPowerShutDown_ENABLE_RAPIDPOWERSHUTDOWN, ErrorCode);                              /* SBSW_DCM_PARAM_PTR_FORWARD */
}
# endif
# if (DCM_SVC_11_05_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service11_05Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service11_05Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Delegate job to the concrete mode manager */
  return Dcm_Svc11_ProcessRapPwrDwn(pContext, RTE_MODE_DcmModeRapidPowerShutDown_DISABLE_RAPIDPOWERSHUTDOWN, ErrorCode);                             /* SBSW_DCM_PARAM_PTR_FORWARD */
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_11_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc11.c
 *********************************************************************************************************************/
