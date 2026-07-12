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
/**        \file  Dcm_Svc22.c
 *         \unit  Svc22
 *        \brief  Contains the implementation of Service 0x22 unit.
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
#define DCM_SVC22_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc22.h"
#if (DCM_SVC_22_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Repeater.h"
# include "Dcm_Net.h"
# include "Dcm_Debug.h"
# include "Dcm_Uti.h"
# include "Dcm_RsrcMgr.h"
# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON)
#  include "Dcm_PagedBuffer.h"
# endif
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define DCM_SVC22_PROGRESS_DIDLOOKUP                                ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
# define DCM_SVC22_PROGRESS_CHECKCONDITION                           ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u))
# define DCM_SVC22_PROGRESS_GETLENGTH                                ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 2u))
# define DCM_SVC22_PROGRESS_READDATA                                 ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 3u))

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
 *  Dcm_Svc22GetDidInfoContext()
 *********************************************************************************************************************/
/*! \brief          Returns a pointer to a DidInfoContext of the index given by the repeater proxy context.
 *  \details        -
 *  \param[in,out]  pRepContext    Current repeater proxy context
 *  \return         A valid pointer to a DidInfoContext
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_DidMgrDidInfoContextPtrType, DCM_CODE) Dcm_Svc22GetDidInfoContext(
  Dcm_Svc22RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc22Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x22 Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc22Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc22RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc22ReadDid()
 *********************************************************************************************************************/
/*! \brief          Reads a single paged DID for service 0x22.
 *  \details        -
 *  \param[in]      opStatus                The operation status
 *  \param[in,out]  pDataContext            The pointer to the data context
 *  \param[in,out]  pDidInfoContext         The DID configuration context
 *  \param[in,out]  pDidOpTypeContext       The DID configuration operation context
 *  \param[out]     ErrorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                All data has been copied
 *  \return         DCM_E_PENDING           No result yet, retry later
 *  \return         DCM_E_BUFFERTOOLOW      Some data copied but no place, retry later on TP buffer under-run
 *  \return         DCM_E_FORCE_RCRRP       Enforce a RCR-RP transmission
 *  \return         DCM_E_NOT_OK            The DID reading has failed for some reason. NRC is already set.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc22ReadDid(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_Svc22UtiReadData()
 *********************************************************************************************************************/
/*! \brief          Reads the requested DIDs for service 0x22.
 *  \details        This is shared utility function of the Repeater and PagedBuffer.
 *  \param[in]      opStatus                Current operation status
 *  \param[in,out]  pDataContext            Pointer to the data context
 *  \param[out]     ErrorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \param[in,out]  pRepContext             Service 0x22 repeater context
 *  \return         DCM_E_OK                All data has been copied
 *  \return         DCM_E_PENDING           No result yet, retry later
 *  \return         DCM_E_BUFFERTOOLOW      Some data copied but no place, retry later on TP buffer under-run
 *  \return         DCM_E_FORCE_RCRRP       Enforce a RCR-RP transmission
 *  \return         DCM_E_NOT_OK            The DID reading has failed for some reason. NRC is already set.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc22UtiReadData(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc22RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc22DidLookUp()
 *********************************************************************************************************************/
/*! \brief          A generic DID look-up for SID 0x22.
 *  \details        Performs a lookup for a specific DID on any request.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_LOOP              Go to next step
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc22DidLookUp(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc22RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc22CheckCondition()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of SID 0x22.
 *  \details        Checks the conditions of a requested DID.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc22CheckCondition(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc22RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc22GetLength()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of SID 0x22.
 *  \details        Reads the length of a requested DID.
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc22GetLength(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc22RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc22ReadData()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of SID 0x22.
 *  \details        Reads the data of a requested DID.
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_FORCE_RCRRP       Force RCR-RP
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc22ReadData(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc22RepeaterProxyContextPtrType pRepContext
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
 *  Dcm_Svc22GetDidInfoContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_DidMgrDidInfoContextPtrType, DCM_CODE) Dcm_Svc22GetDidInfoContext(
  Dcm_Svc22RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_DidMgrDidInfoContextPtrType lResult;

  if (Dcm_DebugDetectRuntimeError(pRepContext->DidInfoContextIdx >= DCM_SVC_22_MAX_DID_LIST_LEN))                                                    /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &(pRepContext->DidInfoContextList[0]);
  }
  else
  {
    lResult = &(pRepContext->DidInfoContextList[pRepContext->DidInfoContextIdx]);
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc22Handler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc22Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc22RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* If the request length is even (SID is already extracted) and does not exceed the capacity of the ECU: */
  if ((Dcm_UtiIsEven(pMsgContext->reqDataLen))
    && (pMsgContext->reqDataLen <= (Dcm_MsgLenType)(DCM_SVC_22_MAX_DID_LIST_LEN * 2u)))
  {
    pRepContext->DidInfoContextIdx = 0u;                                                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->NumDids = 0u;                                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->DidIter = 0u;                                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->ActualLength = 0u;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->ReservedLength = 0u;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */

    Dcm_RepeaterNextStep(pContext, DCM_SVC22_PROGRESS_DIDLOOKUP);                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;
  }
  else
  {
    *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc22ReadDid()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc22ReadDid(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  /* Try to read the DID data */
  lStdResult = Dcm_DidMgrReadDid(opStatus
                                ,pDataContext
                                ,pDidInfoContext
                                ,pDidOpTypeContext
                                ,ErrorCode);                                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

  switch (lStdResult)
  {
  case DCM_E_PENDING:
    /* Try it the next task cycle */
    break;
# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON)
  case DCM_E_BUFFERTOOLOW:
    /* Notify the page buffer and try it later */
    break;
# endif
# if (DCM_SVC_2C_02_SUPPORT_ENABLED == STD_ON)
  case DCM_E_FORCE_RCRRP:
    break;
# endif
  case DCM_E_OK:
    /* Go on with next DID */
    Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID22);
    break;
  default: /* DCM_E_NOT_OK */
    /* Finish service processing immediately by sending the appropriate NRC */
    Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID22);
    break;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc22UtiReadData()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc22UtiReadData(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc22RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType                   lStdResult;
  Dcm_DidMgrDidInfoContextPtrType  pDidInfoContext;

  pDidInfoContext = Dcm_Svc22GetDidInfoContext(pRepContext);                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

  if (Dcm_RsrcMgrHasDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID22) == FALSE)
  {
# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON)
    if (pDataContext->AvailLen < 2u)
    {
      lStdResult = DCM_E_BUFFERTOOLOW;
    }
    else
# endif
    {
      lStdResult = Dcm_RsrcMgrGetDidLock(opStatus, pDidInfoContext->Did, DCM_RSRCMGR_DIDLOCK_OWNER_SID22);

      if (lStdResult == DCM_E_OK)
      {
        Dcm_UtiProvideDataAsU16(pDataContext, pDidInfoContext->Did);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
        Dcm_DidMgrInitOpClassInfo(pDidInfoContext, DCM_DIDMGR_OPTYPE_READ);                                                                          /* SBSW_DCM_POINTER_WRITE_DIDINFOCONTEXT */
        lStdResult = DCM_E_LOOP;
      }
    }
  }
  else
  {
    /*
    *  Action/Return value | DCM_E_PENDING | DCM_E_BUFFERTOOLOW | DCM_E_OK
    * ---------------------+---------------+--------------------+----------
    *  call again          |      yes      |        yes         |   no
    *  data committed      |      no       |        yes         |   yes
    *  ready               |      no       |        no          |   yes
    */

    /* Try to read the DID data */
    lStdResult = Dcm_Svc22ReadDid(opStatus
                                 ,pDataContext
                                 ,pDidInfoContext
                                 ,&(pRepContext->DidOpTypeContext)
                                 ,ErrorCode);                                                                                                        /* SBSW_DCM_COMB_PTR_FORWARD */

    if (lStdResult == DCM_E_OK)
    {
      --pRepContext->NumDids;                                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
      if (pRepContext->NumDids != 0u)
      {
        /* Read next Did */
        pRepContext->DidInfoContextIdx++;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */

        lStdResult = DCM_E_LOOP;
      }
    }
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc22DidLookUp()
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
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc22DidLookUp(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc22RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType                   lStdReturn;
  Dcm_DidMgrDidInfoContextPtrType  pDidInfoContext;

  pDidInfoContext = Dcm_Svc22GetDidInfoContext(pRepContext);                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

  if (opStatus == DCM_INITIAL)
  {
    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &(pDidInfoContext->Did));                                                                                /* SBSW_DCM_COMB_PTR_FORWARD */
  }

  lStdReturn = Dcm_DidMgrGetDidInfo(opStatus
                                   ,pDidInfoContext
                                   ,&(pRepContext->DidOpTypeContext)
                                   ,DCM_DIDMGR_OP_READ);                                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */

  switch (lStdReturn)
  {
  case DCM_E_OK:
    ++(pRepContext->NumDids); /* register a supported DID */                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */

    lStdReturn = Dcm_DidMgrDynDidStateCheck(Dcm_NetGetConnHdlOfRxPduId(pMsgContext->rxPduId), pDidInfoContext, ErrorCode);                           /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_COMB_PTR_FORWARD */
    if (lStdReturn == DCM_E_OK)
    {
      ++(pRepContext->DidInfoContextIdx); /* next DID */                                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_LOOP; /* proceed with next DID */
    } /* else DCM_E_NOT_OK (ErrorCode already set) */
    break;
  case DCM_E_PENDING:
    /* Try it the next task cycle */
    break;
  default: /* DCM_E_NOT_OK */
    /* Just skip this DID - request a new look-up loop */
    lStdReturn = DCM_E_LOOP;
    break;
  }

  /* If next DID look-up is requested: */
  if (lStdReturn == DCM_E_LOOP)
  {
    /* If all DIDs are validated: */
    if (pMsgContext->reqDataLen == 0u)
    {
      if (pRepContext->NumDids != 0u) /* any DID to process? */
      {
        pRepContext->DidIter = pRepContext->NumDids;                                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
        pRepContext->DidInfoContextIdx = 0u;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */

        Dcm_RepeaterNextStep(pContext, DCM_SVC22_PROGRESS_CHECKCONDITION); /* delegate job */                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
      else
      {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdReturn = DCM_E_NOT_OK;
      }
    }
  }

  return lStdReturn;
}

/**********************************************************************************************************************
 *  Dcm_Svc22CheckCondition()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc22CheckCondition(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc22RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                   lStdResult = DCM_E_OK;
  Dcm_DidMgrDidInfoContextPtrType  pDidInfoContext;

  pDidInfoContext = Dcm_Svc22GetDidInfoContext(pRepContext);                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if (Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(pDidInfoContext), DCM_DIDMGR_OPTYPE_READCHKCOND))
  {
    Dcm_DidMgrInitOpClassInfo(pDidInfoContext, DCM_DIDMGR_OPTYPE_READCHKCOND);                                                                       /* SBSW_DCM_POINTER_WRITE_DIDINFOCONTEXT */

    /* check conditions */
    lStdResult = Dcm_DidMgrReadCheckCond(opStatus
                                        ,pDidInfoContext
                                        ,&(pRepContext->DidOpTypeContext)
                                        ,ErrorCode);                                                                                                 /* SBSW_DCM_COMB_PTR_FORWARD */
  }/* else - not supported operation - go on with getLength */

  if (lStdResult == DCM_E_OK)
  {
    /* DCM_E_OK - go on with next DID */
    Dcm_RepeaterNextStep(pContext, DCM_SVC22_PROGRESS_GETLENGTH); /* next job */                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc22GetLength()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc22GetLength(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc22RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType                   lStdReturn = DCM_E_OK;
  Dcm_DidMgrDidInfoContextPtrType  pDidInfoContext;

  pDidInfoContext = Dcm_Svc22GetDidInfoContext(pRepContext);                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if (Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(pDidInfoContext), DCM_DIDMGR_OPTYPE_READLENGTH))
  {
    Dcm_DidMgrInitOpClassInfo(pDidInfoContext, DCM_DIDMGR_OPTYPE_READLENGTH);                                                                        /* SBSW_DCM_POINTER_WRITE_DIDINFOCONTEXT */

    lStdReturn = Dcm_DidMgrReadLength(opStatus
                                     ,pDidInfoContext
                                     ,&(pRepContext->DidOpTypeContext)
                                     ,ErrorCode);                                                                                                    /* SBSW_DCM_COMB_PTR_FORWARD */
  }

  if (lStdReturn == DCM_E_OK)
  {
    Dcm_MsgLenType lMaxLength;

    pRepContext->ActualLength += 2u; /* cumulate DID length + DID itself */                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->ActualLength += pDidInfoContext->DidLength;                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */

    pRepContext->ReservedLength += 2u;                                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->ReservedLength += pDidInfoContext->ReservedLength;                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */

# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON)
    /* Note: Protocol->MaxLength includes the SID, so subtract it */
    lMaxLength = (Dcm_MsgLenType)(Dcm_NetGetProtObjOfTranspObj(Dcm_DiagGetTranspObj(pContext))->MaxLength) - 1u;                                     /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */
# else
    /* Note: pMsgContext->resMaxDataLen excludes the SID already */
    lMaxLength = pMsgContext->resMaxDataLen;
# endif

    /* Check for buffer overrun */
    if (pRepContext->ReservedLength <= lMaxLength)
    {
      pRepContext->DidIter--;                                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
      if (pRepContext->DidIter == 0u)
      {
        /* All DIDs processed without any errors. Prepare for reading
        * Now pRepContext->NumDids will be used as counter -> didIter not needed anymore!
        * pRepContext->DidIter = pRepContext->NumDids;
        */
        pRepContext->DidInfoContextIdx = 0u;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */

        Dcm_RepeaterNextStep(pContext, DCM_SVC22_PROGRESS_READDATA); /* delegate job */                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
      else
      {
        pRepContext->DidInfoContextIdx++; /* next DID */                                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */

        Dcm_RepeaterNextStep(pContext, DCM_SVC22_PROGRESS_CHECKCONDITION); /* start again with next DID  */                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
      lStdReturn = DCM_E_LOOP; /* speed up processing */
    }
    else
    {
      /* the configured buffer will not be able to hold all of the data -> notify the client */
      *ErrorCode = DCM_E_RESPONSETOOLONG;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK; /* finish service processing immediately */
    }
  }/* else - DCM_E_PENDING, DCM_E_NOT_OK (ErrorCode already set) */

  return lStdReturn;
}

/**********************************************************************************************************************
 *  Dcm_Svc22ReadData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc22ReadData(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc22RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON)
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  Dcm_DebugAssert((pMsgContext->resDataLen == 0u), DCM_SID_INTERNAL, DCM_E_PARAM);                                                                   /* COV_DCM_RTM_DEV_DEBUG XF */

  /* Start the paged-buffer to copy the DID data */
  Dcm_PagedBufferStart(pMsgContext, pRepContext->ActualLength);                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
  return DCM_E_STOP_REPEATER;
# else
  Std_ReturnType           lStdResult;
  Dcm_DiagDataContextType  lDataContext;

  Dcm_UtiInitDataContext(&lDataContext, Dcm_UtiGetResData(pMsgContext), pMsgContext->resMaxDataLen);                                                 /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_POINTER_INIT_DATA_CONTEXT */

  lStdResult = Dcm_Svc22UtiReadData(opStatus, &lDataContext, ErrorCode, pRepContext);                                                                /* SBSW_DCM_COMB_PTR_FORWARD */

  /* Commit copied data */
  Dcm_UtiProvideResData(pMsgContext, lDataContext.Usage);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */

  return lStdResult;
# endif
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Service22Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service22Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc22RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc22;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc22Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC22_PROGRESS_DIDLOOKUP:
    lStdResult = Dcm_Svc22DidLookUp(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                        /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC22_PROGRESS_CHECKCONDITION:
    lStdResult = Dcm_Svc22CheckCondition(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                   /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC22_PROGRESS_GETLENGTH:
    lStdResult = Dcm_Svc22GetLength(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                        /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC22_PROGRESS_READDATA:
    lStdResult = Dcm_Svc22ReadData(opStatus, pMsgContext, ErrorCode, pRepContext);                                                                   /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE XX */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}

# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service22Updater()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service22Updater(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc22RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc22;

  return Dcm_Svc22UtiReadData(opStatus, pDataContext, ErrorCode, pRepContext);                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif

/**********************************************************************************************************************
 *  Dcm_Service22Cancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service22Cancel(
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON)
  if (pContext->PagedBuffer.State != DCM_PAGEDBUFFER_STATE_INACTIVE)
  {
    Dcm_NegativeResponseCodeType         lNrc;
    Dcm_DiagDataContextType              lDataContext;
    Dcm_PbCfgNetBufferInfoPtrType        lBufferInfo;
    Dcm_Svc22RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc22;

    /* Initialize the data context */
    lBufferInfo = Dcm_NetGetBufferInfo(Dcm_DiagGetTranspObj(pContext)->BufferHdl);

    Dcm_UtiInitDataContext(&lDataContext, lBufferInfo->BufferPtr, lBufferInfo->Size);                                                                /* SBSW_DCM_POINTER_INIT_DATA_CONTEXT */

    /* Notify the application about the cancellation */
    (void)Dcm_Svc22UtiReadData(DCM_CANCEL, &lDataContext, &lNrc, pRepContext);                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
  }
# endif

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_22_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc22.c
 *********************************************************************************************************************/
