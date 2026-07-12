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
/**        \file  Dcm_Svc3D.c
 *         \unit  Svc3D
 *        \brief  Contains the implementation of Service 0x3D unit.
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
#define DCM_SVC3D_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc3D.h"
#if (DCM_SVC_3D_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Repeater.h"
# include "Dcm_MemMgr.h"
# include "Dcm_RsrcMgr.h"
# include "Dcm_Debug.h"
# include "Dcm_Uti.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define DCM_SVC3D_PROGRESS_CHECKACCESS                              ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
# define DCM_SVC3D_PROGRESS_WRITEMEMORY                              ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u))

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
 *  Dcm_Svc3DHandler()
 *********************************************************************************************************************/
/*! \brief          Service 0x3D Handler.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc3DHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc3DRepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc3DCheckAccess()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of SID 0x3D.
 *  \details        Checks the service 0x3D access.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus         Current operation status
 *  \param[in]      pMsgContext      Pointer to current message context
 *  \param[out]     ErrorCode        The NRC
 *  \param[in]      pRepContext      Current repeater proxy context
 *  \return         DCM_E_LOOP       Repeat immediately
 *  \return         DCM_E_PENDING    Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc3DCheckAccess(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_ReadOnlyMsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc3DRepeaterProxyContextConstPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc3DWriteMemory()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of SID 0x3D.
 *  \details        Writes the data to the memory.
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in]      pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_FORCE_RCRRP       Force RCR-RP
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc3DWriteMemory(
  Dcm_OpStatusType opStatus,
  Dcm_ReadOnlyMsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc3DRepeaterProxyContextConstPtrType pRepContext
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
 *  Dcm_Svc3DHandler()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc3DHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc3DRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  uint8          lAlfid;
  uint8          lBlockLength;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* dispatcher guarantees for minimum length of 4 bytes (ALFID + MEM + SIZE + DATA) available */
  lStdResult = Dcm_MemMgrValidateAndGetAlfid(pMsgContext, &lAlfid, &lBlockLength, ErrorCode);                                                        /* SBSW_DCM_COMB_PTR_FORWARD */ /* SBSW_DCM_POINTER_FORWARD_STACK */
  if (lStdResult == DCM_E_OK)
  {
    /* check for exact length match */
    Dcm_UtiProvideResData(pMsgContext, ((Dcm_MsgLenType)lBlockLength + 1u)); /* (1 byte ALFID + MEM + SIZE) */                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_MemMgrParseMemStream(lAlfid, pMsgContext, &pRepContext->MemBlock);                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */

    if(pRepContext->MemBlock.Length == pMsgContext->reqDataLen)
    {
      lStdResult = Dcm_MemMgrCheckMemBlock(&pRepContext->MemBlock, DCM_MEMMGR_OP_WRITE, ErrorCode);                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
      if (lStdResult == DCM_E_OK)
      {
        Dcm_RepeaterNextStep(pContext, DCM_SVC3D_PROGRESS_CHECKACCESS);                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
        lStdResult = DCM_E_LOOP;/* speed up processing */
      }/* else DCM_E_NOT_OK (ErrorCode already set) */
    }
    else
    {
      *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  }/* else DCM_E_NOT_OK (ErrorCode already set) */

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc3DCheckAccess()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc3DCheckAccess(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_ReadOnlyMsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc3DRepeaterProxyContextConstPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_RsrcMgrGetMemLock(opStatus, DCM_RSRCMGR_MEM_ACCESS_REQ_EXT);

  if (lStdResult == DCM_E_OK)
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC3D_PROGRESS_WRITEMEMORY); /* delegate job */                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP; /* speed up processing */
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc3DWriteMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc3DWriteMemory(
  Dcm_OpStatusType opStatus,
  Dcm_ReadOnlyMsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc3DRepeaterProxyContextConstPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;

  lStdResult = Dcm_MemMgrWriteMemory(opStatus
                                    ,&(pRepContext->MemBlock)
                                    ,Dcm_UtiGetReqData(pMsgContext)                                                                                  /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                    ,ErrorCode);                                                                                                     /* SBSW_DCM_COMB_PTR_FORWARD */

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
 *  Dcm_Service3DProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service3DProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc3DRepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc3D;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc3DHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC3D_PROGRESS_CHECKACCESS:
    lStdResult = Dcm_Svc3DCheckAccess(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC3D_PROGRESS_WRITEMEMORY:
    lStdResult = Dcm_Svc3DWriteMemory(opStatus, pMsgContext, ErrorCode, pRepContext);                                                                /* SBSW_DCM_COMB_PTR_FORWARD */
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

#endif /* (DCM_SVC_3D_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc3D.c
 *********************************************************************************************************************/
