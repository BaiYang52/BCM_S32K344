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
/**        \file  Dcm_Svc36.c
 *         \unit  Svc36
 *        \brief  Contains the implementation of Service 0x36 unit.
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
#define DCM_SVC36_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc36.h"
#if (DCM_SVC_36_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Int.h"
# include "Dcm_Debug.h"
# include "Dcm_Uti.h"
# include "Dcm_Repeater.h"

# include "Rte_Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Service 0x36 processing steps */
# define DCM_SVC36_PROGRESS_EXECUTEOP                                ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! ProcessRequestDownoad is called */
# define Dcm_Svc36ProcessTransferDataWriteWrapper                    DCM_PROCESS_TRANSFER_DATA_WRITE                                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc36HandlerCheckSequenceCounter()
 *********************************************************************************************************************/
/*! \brief          Service 0x36 Check Sequence Counter.
 *  \details        -
 *  \param[in]      downloadState           Download state
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Check is ok
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc36HandlerCheckSequenceCounter(
  Dcm_FblMgrDownloadStateType downloadState,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc36Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x36 Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[in,out]  pDownloadData           Download data context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pDownloadData           Download data context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc36Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc36FinalizeProcessing()
 *********************************************************************************************************************/
/*! \brief          Finalize Processing for Service 0x36 Tranfer Data.
 *  \details        -
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[in,out]  pDownloadData           Download data context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc36FinalizeProcessing(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc36ProcessTransferDataWrite()
 *********************************************************************************************************************/
/*! \brief          Service 0x36 Tranfer Data.
 *  \details        -
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pDownloadData           Download data context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_OK                Send positive response
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_FORCE_RCRRP       Enforce a RCR-RP transmission
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc36ProcessTransferDataWrite(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext
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
 *  Dcm_Svc36HandlerCheckSequenceCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc36HandlerCheckSequenceCounter(
  Dcm_FblMgrDownloadStateType downloadState,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;

  if (downloadState == DCM_FBL_DOWNLOAD_STATE_READY_FOR_TRANSFER)
  {
    if (pRepContext->BlockSequenceCounterNew == 1u)
    {
      lStdResult = DCM_E_OK;
    }
    else
    {
      *ErrorCode = DCM_E_WRONGBLOCKSEQUENCECOUNTER;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
  else
  {
    Dcm_FblMgrBlockSequenceCounterType lBlockSequenceCounterOld = Dcm_FblMgrGetBlockSequenceCounter();

    if ((pRepContext->BlockSequenceCounterNew == lBlockSequenceCounterOld) /* a frame positive response is not received correctly by client */
      || (pRepContext->BlockSequenceCounterNew == ((Dcm_FblMgrBlockSequenceCounterType)(lBlockSequenceCounterOld + 1u)))) /* considers also wrap around case */
    {
      lStdResult = DCM_E_OK;
    }
    else
    {
      *ErrorCode = DCM_E_WRONGBLOCKSEQUENCECOUNTER;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc36Handler()
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
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc36Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;
  Dcm_FblMgrDownloadStateType lDownloadState;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = DCM_E_NOT_OK;

  /* dispatcher guarantees for minimum length of 1 byte (blockSequenceCounter) */
  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &pRepContext->BlockSequenceCounterNew);                                                                     /* SBSW_DCM_COMB_PTR_FORWARD */

  lDownloadState = Dcm_FblMgrGetDownloadState();

  if ((lDownloadState == DCM_FBL_DOWNLOAD_STATE_READY_FOR_TRANSFER)
    || (lDownloadState == DCM_FBL_DOWNLOAD_STATE_TRANSFER_IN_PROGRESS))
  {
    if (pMsgContext->reqDataLen >= 1u) /* at least 1 byte (transferRequestParameterRecord) available */
    {
      if (pMsgContext->reqDataLen <= Dcm_FblMgrGetMaxBlockLength())
      {
        lStdResult = Dcm_Svc36HandlerCheckSequenceCounter(lDownloadState, ErrorCode, pRepContext);                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
      else
      {
        *ErrorCode = DCM_E_TRANSFERDATASUSPENDED;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
      }
    }
    else
    {
      *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
  else
  {
    *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  if (lStdResult == DCM_E_OK)
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC36_PROGRESS_EXECUTEOP);                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;/* speed up processing */
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc36FinalizeProcessing()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc36FinalizeProcessing(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_FblMgrSetDownloadState(DCM_FBL_DOWNLOAD_STATE_TRANSFER_IN_PROGRESS);

  Dcm_FblMgrSetBlockSequenceCounter(pRepContext->BlockSequenceCounterNew);
  Dcm_FblMgrGetMemoryBlock()->Address = Dcm_FblMgrGetMemoryBlock()->Address + pMsgContext->reqDataLen;                                               /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_UtiProvideResDataAsU8(pMsgContext, pRepContext->BlockSequenceCounterNew);                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
}

/**********************************************************************************************************************
 *  Dcm_Svc36ProcessTransferDataWrite()
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
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc36ProcessTransferDataWrite(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_ReturnWriteMemoryType opResult;
  Std_ReturnType  lStdResult = DCM_E_OK;
  boolean lUnknown = FALSE;

  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* If a TransferData request to download data is correctly received and processed in the server but the positive
  * response message does not reach the client.
  */
  if (pRepContext->BlockSequenceCounterNew == Dcm_FblMgrGetBlockSequenceCounter())
  {
    Dcm_UtiProvideResDataAsU8(pMsgContext, Dcm_FblMgrGetBlockSequenceCounter());                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    opResult = Dcm_Svc36ProcessTransferDataWriteWrapper(opStatus
                                                       ,Dcm_FblMgrGetMemoryBlock()->Mid
                                                       ,Dcm_FblMgrGetMemoryBlock()->Address
                                                       ,pMsgContext->reqDataLen
                                                       ,Dcm_UtiGetReqData(pMsgContext)                                                               /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                       ,ErrorCode);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */

    lStdResult = Dcm_UtiConvMemoryOpResult(opResult, opStatus, &lUnknown, ErrorCode);                                                                /* SBSW_DCM_COMB_PTR_FORWARD */

    if (lUnknown == TRUE)
    {
      *ErrorCode = DCM_E_GENERALPROGRAMMINGFAILURE;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      Dcm_DebugReportError(DCM_SID_PROCESSTRANSFERDATAWRITE, DCM_E_INTERFACE_RETURN_VALUE);
    }

    if (lStdResult == DCM_E_OK)
    {
      Dcm_Svc36FinalizeProcessing(pMsgContext, pRepContext);                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
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
 *  Dcm_Service36Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service36Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc36RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc36;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc36Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC36_PROGRESS_EXECUTEOP:
    lStdResult = Dcm_Svc36ProcessTransferDataWrite(opStatus, pMsgContext, ErrorCode, pRepContext);                                                   /* SBSW_DCM_COMB_PTR_FORWARD */
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

#endif /* (DCM_SVC_36_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc36.c
 *********************************************************************************************************************/
