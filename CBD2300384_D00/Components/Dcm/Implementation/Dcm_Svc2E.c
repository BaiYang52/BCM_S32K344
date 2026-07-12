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
/**        \file  Dcm_Svc2E.c
 *         \unit  Svc2E
 *        \brief  Contains the implementation of Service 0x2E unit.
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
#define DCM_SVC2E_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc2E.h"
#if (DCM_SVC_2E_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Repeater.h"
# include "Dcm_RsrcMgr.h"
# include "Dcm_Debug.h"
# include "Dcm_Uti.h"
# include "Dcm_Net.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define DCM_SVC2E_PROGRESS_CHECKACCESS                              ((Dcm_RepeaterProgressType)1u)
# define DCM_SVC2E_PROGRESS_WRITEDATA                                ((Dcm_RepeaterProgressType)2u)

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
 *  Dcm_Svc2EHandler()
 *********************************************************************************************************************/
/*! \brief          Handles Service 0x2E.
 *  \details        Checks if Did information is correct and either ready to write data or pending.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2EHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2ERepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2ECheckAccess()
 *********************************************************************************************************************/
/*! \brief          Checks the service 0x2E access.
 *  \details        Tries to lock a given Did and react accordingly.
 *  \param[in]      pContext         Pointer to the context
 *  \param[in]      opStatus         Current operation status
 *  \param[in]      pMsgContext      Pointer to current message context
 *  \param[in]      pRepContext      Current repeater proxy context
 *  \param[out]     ErrorCode        The NRC
 *  \return         DCM_E_LOOP       Repeat immediately
 *  \return         DCM_E_PENDING    Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2ECheckAccess(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2ERepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2EWriteData()
 *********************************************************************************************************************/
/*! \brief          Executes service 0x2E operation.
 *  \details        Writes the intended data and releases the lock if the operation result is not pending.
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2EWriteData(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2ERepeaterProxyContextPtrType pRepContext
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
 *  Dcm_Svc2EHandler()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2EHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2ERepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  Dcm_CfgStateRefOptType lStateRef;

  if (opStatus == DCM_INITIAL)
  {
    /* Min Length already checked in DiagDispatcher and expected to be 3 !!! */
    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &(pRepContext->DidInfoContext.Did));                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  lStdResult = Dcm_DidMgrGetDidInfo(opStatus
                                   ,&(pRepContext->DidInfoContext)
                                   ,&(pRepContext->DidOpTypeContext)
                                   ,DCM_DIDMGR_OP_WRITE);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */

  if (lStdResult == DCM_E_OK)
  {
    Dcm_UtiProvideResData(pMsgContext, 2u);/* return the DID (shared Rx-Tx buffer) */                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* Check DID specific length */
    if ( (pMsgContext->reqDataLen >= Dcm_CfgDidMgrGetDidInfo(&(pRepContext->DidInfoContext))->MinLength)
       && (pMsgContext->reqDataLen <= Dcm_CfgDidMgrGetDidInfo(&(pRepContext->DidInfoContext))->MaxLength) )
    {
      lStateRef = Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->DidInfoContext))->ExecCondRef;

      lStdResult = Dcm_DidMgrStaticDidStateCheck(Dcm_NetGetConnHdlOfRxPduId(pMsgContext->rxPduId)                                                    /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_COMB_PTR_FORWARD */
                                                ,pRepContext->DidInfoContext.Did
                                                ,DCM_DIDMGR_OP_WRITE
                                                ,lStateRef
                                                ,ErrorCode);
      if (lStdResult == DCM_E_OK)
      {
        Dcm_DidMgrInitOpClassInfo(&(pRepContext->DidInfoContext), DCM_DIDMGR_OPTYPE_WRITE);                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */

        Dcm_RepeaterNextStep(pContext, DCM_SVC2E_PROGRESS_CHECKACCESS); /* delegate job */                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */

        lStdResult = DCM_E_LOOP;/* speed up processing */
      } /* else DCM_E_NOT_OK - ErrorCode already assigned */
    }
    else
    {
      *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  }
  else if (lStdResult == DCM_E_PENDING)
  {
    /* DCM_E_PENDING -> try again */
  }
  else
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc2ECheckAccess()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2ECheckAccess(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc2ERepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_RsrcMgrGetDidLock(opStatus, pRepContext->DidInfoContext.Did, DCM_RSRCMGR_DIDLOCK_OWNER_SID2E);

  if(lStdResult == DCM_E_OK)
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC2E_PROGRESS_WRITEDATA); /* delegate job */                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;/* speed up processing */
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc2EWriteData()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2EWriteData(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2ERepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;

# if (DCM_DIDMGR_SECURE_CODING_ENABLED == STD_ON)
  /* Special handling if this Did is part of Secure Coding */
  if (Dcm_DidMgrIsOpSupported(Dcm_CfgDidMgrGetDidInfo(&(pRepContext->DidInfoContext)), DCM_DIDMGR_OP_CODING))
  {
    lStdResult = Dcm_DidMgrWriteSecured(opStatus
                                       ,pMsgContext
                                       ,&(pRepContext->DidInfoContext)
                                       ,&(pRepContext->DidOpTypeContext)
                                       ,ErrorCode);                                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */

    Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID2E);
  }
  else
# endif
  {
    lStdResult = Dcm_DidMgrWrite(opStatus
                                ,pMsgContext
                                ,&(pRepContext->DidInfoContext)
                                ,&(pRepContext->DidOpTypeContext)
                                ,ErrorCode);                                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

    switch (lStdResult)
    {
    case DCM_E_PENDING:
      /* Try it the next task cycle */
      break;
    case DCM_E_OK:
      Dcm_DebugReportSecurityEvent(pMsgContext->threadId, DCM_DEBUG_SEV_WRITE_DATA);
      Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID2E);
      break;
    default: /* DCM_E_NOT_OK */
      if (*ErrorCode == DCM_E_REQUESTOUTOFRANGE)
      {
        Dcm_DebugReportSecurityEvent(pMsgContext->threadId, DCM_DEBUG_SEV_WRITE_INV_DATA);
      }

      Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID2E);
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
/**********************************************************************************************************************
 *  Dcm_Service2EProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service2EProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc2ERepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc2E;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc2EHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC2E_PROGRESS_CHECKACCESS:
    lStdResult = Dcm_Svc2ECheckAccess(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC2E_PROGRESS_WRITEDATA:
    lStdResult = Dcm_Svc2EWriteData(opStatus, pMsgContext, ErrorCode, pRepContext);                                                                  /* SBSW_DCM_COMB_PTR_FORWARD */
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
 *  Dcm_Service2ECancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service2ECancel(
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID2E);
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_2E_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc2E.c
 *********************************************************************************************************************/
