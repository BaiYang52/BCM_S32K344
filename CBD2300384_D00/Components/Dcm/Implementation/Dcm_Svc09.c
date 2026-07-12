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
/**        \file  Dcm_Svc09.c
 *         \unit  Svc09
 *        \brief  Contains the implementation of Service 0x09 unit.
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
#define DCM_SVC09_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc09.h"
#if (DCM_SVC_09_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Repeater.h"
# include "Dcm_Uti.h"
# include "Dcm_RsrcMgr.h"
# include "Dcm_CfgWrap.h"
# include "Dcm_Debug.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define DCM_SVC09_PROGRESS_CHECKACCESS                              ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
# define DCM_SVC09_PROGRESS_REPEATERPROXY                            ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u))

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
 *  Dcm_Svc09Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x09 Handler.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc09Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc09RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc09CheckAccess()
 *********************************************************************************************************************/
/*! \brief          Checks the service 0x09 access.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc09CheckAccess(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc09RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc09ReadData()
 *********************************************************************************************************************/
/*! \brief          Reads the data of a specific VIT.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc09ReadData(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc09RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc09_XX_RepeaterProxy()
 *********************************************************************************************************************/
/*! \brief          Service 0x09 worker.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc09_XX_RepeaterProxy(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc09RepeaterProxyContextPtrType pRepContext
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
 *  Dcm_Svc09Handler()
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
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc09Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc09RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  pRepContext->ParserDesc.LookUpTable       = Dcm_CfgSvc09LookUpTable;                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
  pRepContext->ParserDesc.LookUpFilterTable = Dcm_CfgSvc09ExecPrecondTable;                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
  pRepContext->ParserDesc.MaskData          = Dcm_CfgSvc09SupportedIdMask;                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */

  if(Dcm_ObdIdMgrIsAvailabilityId(pMsgContext->reqData[0]))
  {
    lStdResult = Dcm_ObdIdMgrProcessSupportedIdRequest(pMsgContext, &(pRepContext->ParserDesc), ErrorCode);                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    if(pMsgContext->reqDataLen == 1u)
    {
      if(Dcm_ObdIdMgrIdLookUp(pMsgContext->reqData[0], &(pRepContext->ParserDesc)) == DCM_E_OK)                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
      {
        Dcm_RepeaterNextStep(pContext, DCM_SVC09_PROGRESS_CHECKACCESS);                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
        lStdResult = DCM_E_LOOP;
      }
      else
      {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }
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
 *  Dcm_Svc09CheckAccess()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc09CheckAccess(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc09RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;
  uint16         lVID = Dcm_UtiMake16Bit((uint8)0xF8u, pMsgContext->reqData[0]);

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_RsrcMgrGetDidLock(opStatus, lVID, DCM_RSRCMGR_DIDLOCK_OWNER_SID09);

  if(lStdResult == DCM_E_OK)
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC09_PROGRESS_REPEATERPROXY); /* delegate job */                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;/* speed up processing */
  }

  return lStdResult;
}
/**********************************************************************************************************************
 *  Dcm_Svc09ReadData()
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
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc09ReadData(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc09RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                  lStdResult;
  Dcm_CfgSvc09ServiceInfoPtrType  pServiceInfo;

  lStdResult = Dcm_CfgWrapSvc09ServiceInfoGetEntry(pRepContext->ParserDesc.Handle, &pServiceInfo, ErrorCode);                                        /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    lStdResult = Dcm_UtiMsgContextVerifySize(pMsgContext, pServiceInfo->ResLength, ErrorCode);                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */

    if(lStdResult == DCM_E_OK)                                                                                                                       /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
    {
      uint8 lVidDataLength = pServiceInfo->ResLength;

      /* compose response record */
      lStdResult = pServiceInfo->OpFunc(opStatus
                                       ,Dcm_UtiGetResData(pMsgContext)                                                                               /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
# if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_422)
                                       ,&lVidDataLength
# endif
                                       );                                                                                                            /* SBSW_DCM_COMB_PTR_FORWARD */ /* SBSW_DCM_CALL_FUNCPTR_SVC09SERVICEINFO */

      if(opStatus == DCM_CANCEL)
      {
        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }

      if (lStdResult == DCM_E_OK)
      {
# if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_422)
        /* Update final response length only in positive/final case */
        if (Dcm_DebugDetectError(lVidDataLength > pServiceInfo->ResLength))                                                                          /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
        {
          lStdResult = DCM_E_NOT_OK;
          *ErrorCode = DCM_E_PANIC_NRC;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
          Dcm_DebugReportError(DCM_SID_SVCPORT_DATASVCS_OP_READVID, DCM_E_INTERFACE_BUFFER_OVERFLOW);
        }
        else
# endif
        {
          /* VID reading finished with OK -> check for next */
          Dcm_UtiProvideResData(pMsgContext, lVidDataLength); /* data size */                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
        }
      }
      else if(lStdResult == DCM_E_PENDING)
      {
        /* Retry later */
      }
      else
      {
        Dcm_DebugAssert((lStdResult == DCM_E_NOT_OK), DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);

        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }
    }
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc09_XX_RepeaterProxy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc09_XX_RepeaterProxy(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc09RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                  lStdResult;

  if(opStatus == DCM_INITIAL)
  {
    /* Write the VID first, then the response data */
    Dcm_UtiProvideResDataAsU8(pMsgContext, pMsgContext->reqData[0]);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  lStdResult = Dcm_Svc09ReadData(opStatus, pMsgContext, ErrorCode, pRepContext);                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */


  if(lStdResult != DCM_E_PENDING)
  {
    Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID09);
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
 *  Dcm_Service09Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service09Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc09RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc09;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc09Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC09_PROGRESS_CHECKACCESS:
    lStdResult = Dcm_Svc09CheckAccess(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC09_PROGRESS_REPEATERPROXY:
    lStdResult = Dcm_Svc09_XX_RepeaterProxy(opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
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

#endif /* (DCM_SVC_09_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc09.c
 *********************************************************************************************************************/
