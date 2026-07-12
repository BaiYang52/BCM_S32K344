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
/**        \file  Dcm_Svc34.c
 *         \unit  Svc34
 *        \brief  Contains the implementation of Service 0x34 unit.
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
#define DCM_SVC34_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc34.h"
#if (DCM_SVC_34_SUPPORT_ENABLED == STD_ON)
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
/*! Service 0x34 processing steps */
# define DCM_SVC34_PROGRESS_EXECUTEOP                                ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! ProcessRequestDownoad is called */
# define Dcm_Svc34ProcessRequestDownloadWrapper                      DCM_PROCESS_REQUEST_DOWNLOAD                                                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

# define DCM_FBL_DATA_IDENTIFIER                                     (0x40u)   /* parameter maxNumberOfBlockLength in the response of service 0x34 always has a size of 4 bytes independent of the value */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc34Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x34 Handler.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc34Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc34RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc34FinalizeProcessing()
 *********************************************************************************************************************/
/*! \brief          Finalize Processing for Service 0x34 Request Download.
 *  \details        -
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc34FinalizeProcessing(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Svc34RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc34ProcessRequestDownload()
 *********************************************************************************************************************/
/*! \brief          Service 0x34 Request Download.
 *  \details        -
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/

DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc34ProcessRequestDownload(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc34RepeaterProxyContextPtrType pRepContext
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
 *  Dcm_Svc34Handler()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc34Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc34RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType         lStdResult;
  uint8                  lAlfid;
  uint8                  lBlockLength = 0;
  Dcm_FblMgrMemBlockType lMemoryBlock;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* dispatcher guarantees for minimum length of 4 bytes (DataFormatIdentifier, ALFID + MEM + SIZE) available */
  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &pRepContext->DataFormatId);                                                                                /* SBSW_DCM_COMB_PTR_FORWARD */

  lStdResult = Dcm_FblMgrValidateAndGetAlfid(pMsgContext, &lAlfid, &lBlockLength, ErrorCode);                                                        /* SBSW_DCM_COMB_PTR_FORWARD */ /* SBSW_DCM_POINTER_FORWARD_STACK */
  if (lStdResult == DCM_E_OK)
  {
    if (lBlockLength != pMsgContext->reqDataLen)
    {
      *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  }

  if (lStdResult == DCM_E_OK)
  {
    lStdResult = Dcm_FblMgrParseMemStream(lAlfid, pMsgContext, &lMemoryBlock, ErrorCode);                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */

    if (lStdResult == DCM_E_OK)
    {
      pRepContext->MemoryBlock = lMemoryBlock;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */

      Dcm_RepeaterNextStep(pContext, DCM_SVC34_PROGRESS_EXECUTEOP);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_LOOP;/* speed up processing */
    }/* else DCM_E_NOT_OK (ErrorCode already set) */
  }/* else DCM_E_NOT_OK (ErrorCode already set) */

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc34FinalizeProcessing()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc34FinalizeProcessing(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Svc34RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_DebugReportSecurityEvent(pMsgContext->threadId, DCM_DEBUG_SEV_REQUEST_DOWNLOAD);

  Dcm_FblMgrSetMaxBlockLength(pRepContext->MaxBlockLength);
  Dcm_FblMgrSetMemoryBlock(&pRepContext->MemoryBlock);                                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
  Dcm_FblMgrSetDownloadState(DCM_FBL_DOWNLOAD_STATE_READY_FOR_TRANSFER);
  Dcm_FblMgrSetBlockSequenceCounter(0u);

  Dcm_UtiProvideResDataAsU8(pMsgContext, DCM_FBL_DATA_IDENTIFIER);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
  Dcm_UtiProvideResDataAsU32(pMsgContext, (pRepContext->MaxBlockLength + 2u)); /* SID and BSC */                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
}

/**********************************************************************************************************************
 *  Dcm_Svc34ProcessRequestDownload()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc34ProcessRequestDownload(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc34RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;
  Dcm_FblMgrBlockLengthType lBlockLength;

  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  pRepContext->MaxBlockLength = (Dcm_FblMgrBlockLengthType)(pMsgContext->resMaxDataLen - 1u); /* SID and BSC are not part of the data */             /* SBSW_DCM_PARAM_PTR_WRITE */
  lBlockLength = pRepContext->MaxBlockLength;

  lStdResult = Dcm_Svc34ProcessRequestDownloadWrapper(opStatus
                                                     ,pRepContext->DataFormatId
                                                     ,pRepContext->MemoryBlock.Mid
                                                     ,pRepContext->MemoryBlock.Address
                                                     ,pRepContext->MemoryBlock.Length
                                                     ,&pRepContext->MaxBlockLength
                                                     ,ErrorCode);                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */

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
      if (pRepContext->MaxBlockLength > lBlockLength)
      {
        lStdResult = DCM_E_NOT_OK;
        *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
        Dcm_DebugReportError(DCM_SID_PROCESSREQUESTDOWNLOAD, DCM_E_INTERFACE_BUFFER_OVERFLOW);
      }
      else
      {
        Dcm_Svc34FinalizeProcessing(pMsgContext, pRepContext);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
    }
    break;
  case DCM_E_NOT_OK:
  case DCM_E_PENDING:
    break;
  default:
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_PROCESSREQUESTDOWNLOAD, DCM_E_INTERFACE_RETURN_VALUE);
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
 *  Dcm_Service34Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service34Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc34RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc34;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc34Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC34_PROGRESS_EXECUTEOP:
    lStdResult = Dcm_Svc34ProcessRequestDownload(opStatus, pMsgContext, ErrorCode, pRepContext);                                                     /* SBSW_DCM_COMB_PTR_FORWARD */
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

#endif /* (DCM_SVC_34_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc34.c
 *********************************************************************************************************************/
