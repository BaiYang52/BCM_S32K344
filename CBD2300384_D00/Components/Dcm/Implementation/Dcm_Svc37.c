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
/**        \file  Dcm_Svc37.c
 *         \unit  Svc37
 *        \brief  Contains the implementation of Service 0x37 unit.
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
#define DCM_SVC37_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc37.h"
#if (DCM_SVC_37_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Int.h"
# include "Dcm_Debug.h"
# include "Dcm_Uti.h"
# include "Dcm_FblMgr.h"
# include "Dcm_Repeater.h"

# include "Rte_Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Service 0x37 processing steps */
# define DCM_SVC37_PROGRESS_EXECUTEOP                                ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! ProcessRequestDownoad is called */
# define Dcm_Svc37ProcessRequestTransferExitWrapper                  DCM_PROCESS_REQUEST_TRANSFER_EXIT                                               /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc37Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x37 Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc37Handler(
  Dcm_ContextPtrType pContext,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc37RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc37ProcessRequestTransferExit()
 *********************************************************************************************************************/
/*! \brief         Service 0x37 Tranfer Exit.
 *  \details        -
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_OK                Send positive response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc37ProcessRequestTransferExit(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc37RepeaterProxyContextPtrType pRepContext
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
 *  Dcm_Svc37Handler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc37Handler(
  Dcm_ContextPtrType pContext,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc37RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType  lStdResult = DCM_E_NOT_OK;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if (Dcm_FblMgrGetDownloadState() == DCM_FBL_DOWNLOAD_STATE_IDLE)
  {
    *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC37_PROGRESS_EXECUTEOP);                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc37ProcessRequestTransferExit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc37ProcessRequestTransferExit(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc37RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType  lStdResult;
  Dcm_MsgLenType  lAvailBufferSize;

  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lAvailBufferSize = pMsgContext->resMaxDataLen;

  lStdResult = Dcm_Svc37ProcessRequestTransferExitWrapper(opStatus,
                                                          Dcm_UtiGetReqData(pMsgContext),                                                            /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                          pMsgContext->reqDataLen,
                                                          Dcm_UtiGetResData(pMsgContext),                                                            /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
                                                          &lAvailBufferSize,
                                                          ErrorCode);                                                                                /* SBSW_DCM_CALL_PROCESSREQUESTTRANSFEREXIT */

  switch (lStdResult)
  {
  case DCM_E_OK:
    if (opStatus == DCM_CANCEL)
    {
      lStdResult = DCM_E_NOT_OK;
      *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      Dcm_UtiProvideResData(pMsgContext, lAvailBufferSize);                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_FblMgrReset();
    }
    break;
  case DCM_E_NOT_OK:
  case DCM_E_PENDING:
    break;
  default:
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_PROCESSREQUESTTRANSFEREXIT, DCM_E_INTERFACE_RETURN_VALUE);
    break;
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
 *  Dcm_Service37Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service37Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc37RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc37;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc37Handler(pContext, pMsgContext, ErrorCode, pRepContext);                                                                    /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC37_PROGRESS_EXECUTEOP:
    lStdResult = Dcm_Svc37ProcessRequestTransferExit(opStatus, pMsgContext, ErrorCode, pRepContext);                                                 /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_37_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc37.c
 *********************************************************************************************************************/
