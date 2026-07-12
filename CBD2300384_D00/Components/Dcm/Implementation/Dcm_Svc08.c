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
/**        \file  Dcm_Svc08.c
 *         \unit  Svc08
 *        \brief  Contains the implementation of Service 0x08 unit.
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
#define DCM_SVC08_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc08.h"
#if (DCM_SVC_08_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Repeater.h"
# include "Dcm_Uti.h"
# include "Dcm_CfgWrap.h"
# include "Dcm_Debug.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define DCM_SVC08_PROGRESS_REPEATERPROXY                            ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))

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
 *  Dcm_Svc08Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x08 Handler.
 *  \details        -
 *  \param[in]      pContext        Pointer to the context
 *  \param[in]      opStatus        The operations status
 *  \param[in,out]  pMsgContext     Current message context
 *  \param[out]     ErrorCode       The NRC
 *  \param[in,out]  pRepContext     Current repeater proxy context
 *  \return         DCM_E_NOT_OK    Send negative response
 *  \return         DCM_E_LOOP      Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc08Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc08RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc08_XX_RepeaterProxy()
 *********************************************************************************************************************/
/*! \brief          Service 0x08 worker.
 *  \details        -
 *  \param[in]      opStatus         The operations status
 *  \param[in]      pMsgContext      Current message context
 *  \param[out]     ErrorCode        The NRC
 *  \param[in,out]  pRepContext      Current repeater proxy context
 *  \return         DCM_E_OK         Send response
 *  \return         DCM_E_NOT_OK     Send negative response
 *  \return         DCM_E_PENDING    Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc08_XX_RepeaterProxy(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc08RepeaterProxyContextPtrType pRepContext
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
 *  Dcm_Svc08Handler()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc08Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc08RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  pRepContext->ParserDesc.LookUpTable       = Dcm_CfgSvc08LookUpTable;                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
  pRepContext->ParserDesc.LookUpFilterTable = Dcm_CfgSvc08ExecPrecondTable;                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
  pRepContext->ParserDesc.MaskData          = Dcm_CfgSvc08SupportedIdMask;                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */

  if(Dcm_ObdIdMgrIsAvailabilityId(pMsgContext->reqData[0]))
  {
    lStdResult = Dcm_ObdIdMgrProcessSupportedIdRequest(pMsgContext, &(pRepContext->ParserDesc), ErrorCode);                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    if(Dcm_ObdIdMgrIdLookUp(pMsgContext->reqData[0], &(pRepContext->ParserDesc)) == DCM_E_OK)                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
    {
      Dcm_RepeaterNextStep(pContext, DCM_SVC08_PROGRESS_REPEATERPROXY);                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_LOOP;
    }
    else
    {
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc08_XX_RepeaterProxy()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc08_XX_RepeaterProxy(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc08RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                  lStdResult;
  Dcm_CfgSvc08ServiceInfoPtrType  pServiceInfo;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_CfgWrapSvc08ServiceInfoGetEntry(pRepContext->ParserDesc.Handle, &pServiceInfo, ErrorCode);                                        /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    /* Move pointer to request and response buffer to correct position */
    Dcm_UtiConsumeReqData(pMsgContext, 1u);                                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_UtiProvideResData(pMsgContext, 1u);                                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */

    lStdResult = Dcm_UtiMsgContextVerifySize(pMsgContext, pServiceInfo->ResLength, ErrorCode);                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */

    if(lStdResult == DCM_E_OK)                                                                                                                       /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
    {
      /* Data TID -> only one possible */
      if(pMsgContext->reqDataLen == pServiceInfo->ReqLength)
      {
        lStdResult = pServiceInfo->OpFunc(Dcm_UtiGetResData(pMsgContext)                                                                             /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                         ,Dcm_UtiGetReqData(pMsgContext));                                                                           /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_CALL_FUNCPTR_SVC08SERVICEINFO */

        if(lStdResult == DCM_E_OK)
        {
          /* Prepare for positive response with specific length of the data (TID is already provided) */
          Dcm_UtiProvideResData(pMsgContext, pServiceInfo->ResLength);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
        }
        else
        {
          *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
        }
      }
      else
      {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }
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
/**********************************************************************************************************************
 *  Dcm_Service08Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service08Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc08RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc08;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc08Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC08_PROGRESS_REPEATERPROXY:
    lStdResult = Dcm_Svc08_XX_RepeaterProxy(opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_08_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc08.c
 *********************************************************************************************************************/
