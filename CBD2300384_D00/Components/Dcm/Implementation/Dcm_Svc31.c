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
/**        \file  Dcm_Svc31.c
 *         \unit  Svc31
 *        \brief  Contains the implementation of Service 0x31 unit.
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
#define DCM_SVC31_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc31.h"
#if (DCM_SVC_31_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Repeater.h"
# include "Dcm_Debug.h"
# include "Dcm_Uti.h"
# include "Dcm_RidMgr.h"
# include "Dcm_State.h"
# include "Dcm_RsrcMgr.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Service 0x31 processing steps */
# define DCM_SVC31_PROGRESS_CHECKACCESS                              ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
# define DCM_SVC31_PROGRESS_EXECUTEOP                                ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u))

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Context for the handling of RIDs */
struct DCM_SVC31THREADCONTEXT_TAG
{
  uint8 IntegrityBuffer[DCM_SVC_31_INTEGRITY_BUFFER_SIZE];  /*!< Buffer for copy of input or output data for one Rid */
};
typedef struct DCM_SVC31THREADCONTEXT_TAG Dcm_Svc31ThreadContextType;
typedef P2VAR(Dcm_Svc31ThreadContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc31ThreadContextPtrType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc31GetThreadContext()
 *********************************************************************************************************************/
/*! \brief          Returns a Svc31 thread context element of the given index.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         Valid debug thread context.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_Svc31ThreadContextPtrType, DCM_CODE) Dcm_Svc31GetThreadContext(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_Svc31Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x31 Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc31Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc31RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc31CheckAccess()
 *********************************************************************************************************************/
/*! \brief          Gets the lock for 0x31 Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc31CheckAccess(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc31RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc31RedirectMsgContext()
 *********************************************************************************************************************/
/*! \brief          Redirects the buffer to local provided one.
 *  \details        -
 *  \param[in]      pMsgContext          Current message context
 *  \param[in]      pMsgContextCopied    Message context to be filled
 *  \param[in,out]  pRepContext          Current repeater proxy context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc31RedirectMsgContext(
  Dcm_ReadOnlyMsgContextPtrType pMsgContext,
  Dcm_MsgContextPtrType pMsgContextCopied,
  Dcm_Svc31RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc31ExecuteRoutine()
 *********************************************************************************************************************/
/*! \brief          Executes a routine operation.
 *  \details        Ensures that the request data stays availalbe during service processing.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Operation succeeded
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_NOT_OK            Operation failed, ErrorCode contains the NRC
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc31ExecuteRoutine(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc31RepeaterProxyContextPtrType pRepContext
  );
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Thread context of Svc31 unit */
DCM_LOCAL VAR(Dcm_Svc31ThreadContextType, DCM_VAR_NOINIT) Dcm_Svc31ThreadContext[DCM_NUM_THREADS];                                                   /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
# define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc31GetThreadContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_Svc31ThreadContextPtrType, DCM_CODE) Dcm_Svc31GetThreadContext(
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_Svc31ThreadContextPtrType pThreadContext;

  if (Dcm_DebugDetectRuntimeError(threadId >= DCM_NUM_THREADS))                                                                                      /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_UNIT_TEST XF */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    pThreadContext = &Dcm_Svc31ThreadContext[0];
  }
  else
  {
    pThreadContext = &Dcm_Svc31ThreadContext[threadId];
  }

  return pThreadContext;
}

/**********************************************************************************************************************
 *  Dcm_Svc31Handler()
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
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc31Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc31RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  uint8          lRoutineOp;
  uint16         lRid;

  /* Search for the RID in the database */
  lStdResult = Dcm_RidMgrRidLookUp(opStatus
                                  ,Dcm_UtiGetReqDataAsU16Rel(pMsgContext, 1u)                                                                        /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                  ,&(pRepContext->RidInfoIdx));                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
  /* If RID found: */
  if(lStdResult == DCM_E_OK)
  {
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lRoutineOp); /* Extract SF */                                                                            /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &lRid);      /* Extract RID for use in RID WL Check */                                                   /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_UtiProvideResData(pMsgContext, 3u); /* Commit SF and RID in response */                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */

    pRepContext->Rid = lRid;                                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */

    lStdResult = Dcm_StateCheckRID(pContext, lRid, lRoutineOp, Dcm_CfgRidMgrRidInfoExecCondRef(pRepContext->RidInfoIdx), ErrorCode);                 /* SBSW_DCM_COMB_PTR_FORWARD */

    /* If the preconditions for the requested RID are fulfilled: */
    if(lStdResult == DCM_E_OK)
    {
      lStdResult = Dcm_RidMgrGetOpInfo(pRepContext->RidInfoIdx
                                      ,Dcm_RidMgrConvOpOfSubFunc(lRoutineOp)
                                      ,&(pRepContext->RidOpInfoIdx));                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

      /* If the requested sub-function is supported: */
      if(lStdResult == DCM_E_OK)
      {
        /* If the length of the request is valid: */
        if( (pMsgContext->reqDataLen >= Dcm_CfgRidMgrOpInfoReqMinLength(pRepContext->RidOpInfoIdx))
          &&(pMsgContext->reqDataLen <= Dcm_CfgRidMgrOpInfoReqMaxLength(pRepContext->RidOpInfoIdx)) )
        {
          if (Dcm_CfgRidMgrOpInfoParamIntegrity(pRepContext->RidOpInfoIdx) == DCM_RIDMGR_INTEGRITY_IN)
          {
            Dcm_Svc31ThreadContextPtrType pThreadContext;

            pThreadContext = Dcm_Svc31GetThreadContext(pMsgContext->threadId);
            /* Copy the the request data to the integrity buffer */
            Dcm_UtiMemCopySafe(Dcm_UtiGetReqData(pMsgContext)                                                                                        /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                              ,pThreadContext->IntegrityBuffer                                                                                       /* SBSW_DCM_POINTER_WRITE_SVC31THREADCONTEXT */
                              ,0u
                              ,DCM_SVC_31_INTEGRITY_BUFFER_SIZE
                              ,pMsgContext->reqDataLen);                                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */
          }
          Dcm_RepeaterNextStep(pContext, DCM_SVC31_PROGRESS_CHECKACCESS);                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
          lStdResult = DCM_E_LOOP;
        }
        else
        {
          *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
        }
      }
      else
      {
        *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }
    }/* else DCM_E_NOT_OK - ErrorCode already assigned */
  }
  /* Otherwise, if RID look-up needs more time: */
  else if (lStdResult == DCM_E_PENDING)
  {
    /* lStdResult is already set */
  }
  /* Otherwise i.e. RID not found, return NRC 0x31 */
  else
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_Svc31CheckAccess()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc31CheckAccess(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc31RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_RsrcMgrGetRidLock(opStatus, pRepContext->Rid, pMsgContext->threadId);

  if (lStdResult == DCM_E_OK)
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC31_PROGRESS_EXECUTEOP);                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;
  }
  return lStdResult;
}                                                                                                                                                    /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_Svc31RedirectMsgContext()
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
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc31RedirectMsgContext(
  Dcm_ReadOnlyMsgContextPtrType pMsgContext,
  Dcm_MsgContextPtrType pMsgContextCopied,
  Dcm_Svc31RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_Svc31ThreadContextPtrType pThreadContext;

  /* Copy all values as default */
  *pMsgContextCopied = *pMsgContext;                                                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
  pThreadContext = Dcm_Svc31GetThreadContext(pMsgContext->threadId);

  switch (Dcm_CfgRidMgrOpInfoParamIntegrity(pRepContext->RidOpInfoIdx))
  {
    case DCM_RIDMGR_INTEGRITY_IN:
      {
        /* Modify the request values */
        pMsgContextCopied->reqData    = pThreadContext->IntegrityBuffer;                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */ /* SBSW_DCM_POINTER_WRITE_SVC31THREADCONTEXT */
        pMsgContextCopied->reqIndex   = 0;                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
        pMsgContextCopied->reqBufSize = DCM_SVC_31_INTEGRITY_BUFFER_SIZE;                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
      }
      break;
    case DCM_RIDMGR_INTEGRITY_OUT:
      {
        /* Modify the response values */
        pMsgContextCopied->resData       = pThreadContext->IntegrityBuffer;                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */ /* SBSW_DCM_POINTER_WRITE_SVC31THREADCONTEXT */
        pMsgContextCopied->resIndex      = 0;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
        pMsgContextCopied->resDataLen    = 0;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
        pMsgContextCopied->resBufSize    = DCM_SVC_31_INTEGRITY_BUFFER_SIZE;                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
        pMsgContextCopied->resMaxDataLen = DCM_SVC_31_INTEGRITY_BUFFER_SIZE;                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
      }
      break;
    default: /* DCM_RIDMGR_INTEGRITY_NONE */
      /* Nothing to do */
      break;
  }
}

/**********************************************************************************************************************
 *  Dcm_Svc31ExecuteRoutine()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc31ExecuteRoutine(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc31RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType                       lStdResult;
  Dcm_MsgContextType                   lMsgContext;
  Dcm_CfgRidMgrRoutineInfoByteMemType  lRoutineInfoByte;

  lRoutineInfoByte = Dcm_CfgRidMgrRidInfoRoutineInfoByte(pRepContext->RidInfoIdx);

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /*
   * If the RID supports routine info byte, reserve 1 byte in response buffer by moving response buffer pointer
   * Note: Do not write the routine info byte yet since this would overwrite the request data (IN-parameter)
   *       Provide the routine info byte as a placeholder before checking the remaining response buffer size
   */
  if( (opStatus == DCM_INITIAL)
    &&(lRoutineInfoByte != DCM_CFGRIDMGR_INVALID_ROUTINEINFOBYTE) )
  {
    Dcm_UtiProvideResData(pMsgContext, 1u);                                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  Dcm_Svc31RedirectMsgContext(pMsgContext, &lMsgContext, pRepContext);                                                                               /* SBSW_DCM_POINTER_SVC31_REDIRECT_MESSAGE_CONTEXT */

  lStdResult = Dcm_RidMgrExecuteRoutine(opStatus, &lMsgContext, ErrorCode, pRepContext);                                                             /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lStdResult == DCM_E_OK)
  {
    /* Place the routine info byte, if applicable */
    if (lRoutineInfoByte != DCM_CFGRIDMGR_INVALID_ROUTINEINFOBYTE)
    {
      Dcm_UtiSetResDataAsU8At(pMsgContext, DCM_RIDMGR_ROUTINEINFOBYTE_IDX, (uint8)lRoutineInfoByte);                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
    }

    if (Dcm_CfgRidMgrOpInfoParamIntegrity(pRepContext->RidOpInfoIdx) == DCM_RIDMGR_INTEGRITY_OUT)
    {
      /* Copy the data from integrity buffer back to original response buffer */
      Dcm_UtiProvideResDataAsUN(pMsgContext, lMsgContext.resData, lMsgContext.resDataLen);                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
      /* Just commit the written data */
      Dcm_UtiProvideResData(pMsgContext, (Dcm_MsgLenType)(lMsgContext.resDataLen - pMsgContext->resDataLen));                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
  }

  if ((lStdResult == DCM_E_OK) || (lStdResult == DCM_E_NOT_OK))
  {
    Dcm_RsrcMgrReleaseRidLock(pMsgContext->threadId);
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
 *  Dcm_Service31Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service31Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc31RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc31;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc31Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC31_PROGRESS_CHECKACCESS:
    lStdResult = Dcm_Svc31CheckAccess(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC31_PROGRESS_EXECUTEOP:
    lStdResult = Dcm_Svc31ExecuteRoutine(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                   /* SBSW_DCM_COMB_PTR_FORWARD */
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

#endif /* (DCM_SVC_31_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc31.c
 *********************************************************************************************************************/
