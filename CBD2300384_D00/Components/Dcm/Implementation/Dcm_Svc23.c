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
/**        \file  Dcm_Svc23.c
 *         \unit  Svc23
 *        \brief  Contains the implementation of Service 0x23 unit.
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
#define DCM_SVC23_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc23.h"
#if (DCM_SVC_23_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Repeater.h"
# include "Dcm_Debug.h"
# include "Dcm_Uti.h"
# include "Dcm_MemMgr.h"
# include "Dcm_RsrcMgr.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Service 0x23 processing steps */
# define DCM_SVC23_PROGRESS_CHECKACCESS                              ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
# define DCM_SVC23_PROGRESS_READMEMORY                               ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u))

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
 *  Dcm_Svc23Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x23 Handler.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc23Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc23RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc23CheckAccess()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of SID 0x23.
 *  \details        Checks the service 0x23 access.
 *  \param[in]      pContext         Pointer to the context
 *  \param[in]      opStatus         Current operation status
 *  \param[in]      pMsgContext      Pointer to current message context
 *  \param[out]     ErrorCode        The NRC
 *  \param[in,out]  pRepContext      Current repeater proxy context
 *  \return         DCM_E_LOOP       Repeat immediately
 *  \return         DCM_E_PENDING    Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc23CheckAccess(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc23RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc23ReadMemory()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of SID 0x23.
 *  \details        Reads the data from the memory.
 *  \param[in]      opStatus                Current operation status
 *  \param[in,out]  pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in]      pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_FORCE_RCRRP       Force RCR-RP
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc23ReadMemory(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc23RepeaterProxyContextConstPtrType pRepContext
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
 *  Dcm_Svc23Handler()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc23Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc23RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  uint8          lAlfid;
  uint8          lBlockLength;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* dispatcher guarantees for minimum length of 3 bytes (ALFID + MEM + SIZE) available */
  lStdResult = Dcm_MemMgrValidateAndGetAlfid(pMsgContext, &lAlfid, &lBlockLength, ErrorCode);                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
  if(lStdResult == DCM_E_OK)
  {
    /* check for exact length match */
    if(pMsgContext->reqDataLen == lBlockLength)
    {
      Dcm_MemMgrParseMemStream(lAlfid
                              ,pMsgContext
                              ,&pRepContext->MemBlock);                                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */

      lStdResult = Dcm_MemMgrCheckMemBlock(&pRepContext->MemBlock, DCM_MEMMGR_OP_READ, ErrorCode);                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
      if(lStdResult == DCM_E_OK)
      {
        /* Check for overflow */
        if(pRepContext->MemBlock.Length <= pMsgContext->resMaxDataLen)
        {
          Dcm_RepeaterNextStep(pContext, DCM_SVC23_PROGRESS_CHECKACCESS);                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
          lStdResult = DCM_E_LOOP;/* speed up processing */
        }
        else
        {
          *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
        }
      }/* else DCM_E_NOT_OK - ErrorCode is already set */
    }
    else
    {
      *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  }/* else DCM_E_NOT_OK - ErrorCode is already set */

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc23CheckAccess()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc23CheckAccess(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc23RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_RsrcMgrGetMemLock(opStatus, DCM_RSRCMGR_MEM_ACCESS_REQ_EXT);

  if (lStdResult == DCM_E_OK)
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC23_PROGRESS_READMEMORY); /* delegate job */                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;/* speed up processing */
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc23ReadMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc23ReadMemory(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc23RepeaterProxyContextConstPtrType pRepContext
  )
{
  Std_ReturnType          lStdResult;
  Dcm_DiagDataContextType lDataContext;

  Dcm_UtiInitDataContext(&lDataContext, Dcm_UtiGetResData(pMsgContext), pMsgContext->resMaxDataLen);                                                 /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_POINTER_INIT_DATA_CONTEXT */

  lStdResult = Dcm_MemMgrReadMemory(opStatus
                                   ,&(pRepContext->MemBlock)
                                   ,&lDataContext
                                   ,ErrorCode);                                                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)
  {
    Dcm_UtiProvideResData(pMsgContext, pRepContext->MemBlock.Length);                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  /*  Otherwise - any other result to be delegated to the core
   *  No DET monitoring needed since the Dcm_MemMgrReadMemory has already one!
   */
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
 *  Dcm_Service23Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service23Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc23RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc23;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc23Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC23_PROGRESS_CHECKACCESS:
    lStdResult = Dcm_Svc23CheckAccess(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC23_PROGRESS_READMEMORY:
    lStdResult = Dcm_Svc23ReadMemory(opStatus, pMsgContext, ErrorCode, pRepContext);                                                                 /* SBSW_DCM_COMB_PTR_FORWARD */
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

#endif /* (DCM_SVC_23_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc23.c
 *********************************************************************************************************************/
