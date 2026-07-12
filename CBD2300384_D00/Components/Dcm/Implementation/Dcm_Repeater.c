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
/**        \file  Dcm_Repeater.c
 *         \unit  Repeater
 *        \brief  Contains the implementation of Repeater unit.
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
#define DCM_REPEATER_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Repeater.h"
#include "Dcm_Uti.h"
#include "Dcm_CfgWrap.h"
#include "Dcm_RsrcMgr.h"

#include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RepeaterNextSubStep()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_RepeaterNextSubStep(                                                                                                        /* PRQA S 3219 */ /* MD_MSR_Unreachable */
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_RepeaterProgressType nextSubStep
  )
{
  pContext->Repeater.SubProgress = nextSubStep;                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_RepeaterResetOpStatus(pContext);                                                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
}

/**********************************************************************************************************************
 *  Dcm_RepeaterNextStep()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_RepeaterNextStep(                                                                                                           /* PRQA S 3219 */ /* MD_MSR_Unreachable */
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_RepeaterProgressType nextStep
  )
{
  pContext->Repeater.Progress = nextStep;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_RepeaterNextSubStep(pContext, DCM_REPEATER_PROGRESS_INITIAL);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
}

/**********************************************************************************************************************
 *  Dcm_RepeaterSetCallee()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_RepeaterSetCallee(
  Dcm_ContextPtrType pContext,
  uint8 serviceProcessorIdx
  )
{
  pContext->Repeater.CalleeIdx = serviceProcessorIdx;                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_RepeaterNextStep(pContext, DCM_REPEATER_PROGRESS_INITIAL);                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
}

/**********************************************************************************************************************
 *  Dcm_RepeaterExecute()
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
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_RepeaterExecute(
  Dcm_ContextPtrType pContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;                                                                                                          /* PRQA S 2981 */ /* MD_Dcm_RetVal */

  /* Make a backup in case the processor uses the Dcm_RepeaterSetCallee API */
  Dcm_OpStatusType opStatusBckp = pContext->Repeater.OpStatus;

  Dcm_UtiResetRuntimeLimiter();

  DCM_UTI_LOOP_FOR_EVER                                                                                                                              /* PRQA S 0771 */ /* MD_Dcm_Optimize_0771 */
  {
    Dcm_CfgDiagServiceInfoPtrType pServiceInfo;

    lStdResult = Dcm_CfgWrapDiagServiceInfoGetEntry(pContext->Repeater.CalleeIdx, &pServiceInfo, ErrorCode);                                         /* SBSW_DCM_COMB_PTR_FORWARD */

    if (lStdResult == DCM_E_OK)                                                                                                                      /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
    {
#if (DCM_DIAG_SERVICE_DISPATCHER_ENABLED == STD_ON)
      if(pContext->Diag.ProcessServiceExtern == TRUE)
      {
        lStdResult = pServiceInfo->AltProcFunc(pContext
                                              ,pContext->Repeater.OpStatus
                                              ,&(pContext->Diag.MsgContext)
                                              ,ErrorCode);                                                                                           /* SBSW_DCM_CALL_FUNCPTR_SERVICEINFO */
      }
      else
#endif
      {
        lStdResult = pServiceInfo->ProcFunc(pContext
                                           ,pContext->Repeater.OpStatus
                                           ,&(pContext->Diag.MsgContext)
                                           ,ErrorCode);                                                                                              /* SBSW_DCM_CALL_FUNCPTR_SERVICEINFO */
      }

      if(opStatusBckp == DCM_CANCEL)
      {
        Dcm_DiagSvcCancelFuncType lCancelFunc = Dcm_DiagGetCancelFunc(pServiceInfo->CancelFuncRef);

        lCancelFunc(pContext);                                                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_CALL_FUNCPTR_SVCCANCELLERS */

        *ErrorCode = DCM_DIAG_CANCEL_OP_NRC;/* always return the cancellation NRC! */                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;/* enforce finalization */
      }
    }

    if(lStdResult != DCM_E_LOOP)
    {
      break;
    }/* else - just loop again */

    Dcm_RepeaterResetOpStatus(pContext);                                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */

    if (Dcm_UtiRuntimeLimitReached() == TRUE)
    {
      lStdResult = DCM_E_PENDING_LIMITER;
      break;
    }
  }

  return lStdResult;
}

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterCheckAndGetAccess()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_RepeaterCheckAndGetAccess(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;                                                                                                              /* PRQA S 2981 */ /* MD_Dcm_RetVal */

  if ((Dcm_CfgWrapDiagServiceInfo(pContext->Diag.SidIndex).Props & DCM_DIAG_SVC_CFG_PROP_NEEDS_LOCK) != 0u)
  {
    if (!Dcm_RsrcMgrHasServiceLock(pContext->ThreadId))
    {
      lStdResult = Dcm_RsrcMgrGetServiceLock(pContext->Repeater.OpStatus, pContext->Diag.SidIndex, pContext->ThreadId);

      switch (lStdResult)
      {
      case DCM_E_OK:
        /* set opStatus as initial, because now the Dcm_RepeaterExecute will be called the first time */
        pContext->Repeater.OpStatus = DCM_INITIAL;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
        break;
      case DCM_E_NOT_OK:
        *ErrorCode = DCM_DIAG_CANCEL_OP_NRC; /* always return the cancellation NRC! */                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
        break;
      default:
        /* DCM_E_PENDING do nothing */
        break;
      }
    }
  }
  return lStdResult;
}
#endif

/**********************************************************************************************************************
 *  Dcm_RepeaterDeadEnd()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_RepeaterDeadEnd(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return DCM_E_STOP_REPEATER;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Repeater.c
 *********************************************************************************************************************/
