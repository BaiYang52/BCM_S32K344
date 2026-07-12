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
/**        \file  Dcm_Svc2A.c
 *         \unit  Svc2A
 *        \brief  Contains the implementation of Service 0x2A unit.
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
#define DCM_SVC2A_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc2A.h"
#if (DCM_SVC_2A_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Repeater.h"
# include "Dcm_Svc2ASchd.h"
# include "Dcm_Uti.h"
# include "Dcm_Net.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define DCM_SVC2A_PROGRESS_SCHEDULEDDID_LOOKUP                      ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u)) /*!< Look up DIDs */
# define DCM_SVC2A_PROGRESS_DID_CHECKCONDITION                       ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u)) /*!< Check conditions */
# define DCM_SVC2A_PROGRESS_DID_GETLENGTH                            ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 2u)) /*!< Obtain total length of the DID */
# define DCM_SVC2A_PROGRESS_STOPPED_DID_LOOKUP                       ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 3u)) /*!< Loop up DIDs and stop periodic transmission */

/*! Service 0x2A transmissionMode parameter */
# define DCM_SVC_2A_SEND_AT_SLOW_RATE                                ((Dcm_Svc2ATransmissionModeType)0x01u)  /*!< Server shall transmit at a slow rate */
# define DCM_SVC_2A_SEND_AT_MEDIUM_RATE                              ((Dcm_Svc2ATransmissionModeType)0x02u)  /*!< Server shall transmit at a medium rate */
# define DCM_SVC_2A_SEND_AT_FAST_RATE                                ((Dcm_Svc2ATransmissionModeType)0x03u)  /*!< Server shall transmit at a fast rate */
# define DCM_SVC_2A_STOP_SENDING                                     ((Dcm_Svc2ATransmissionModeType)0x04u)  /*!< Server stops transmitting periodically/repeatedly */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
# define Dcm_Svc2AMakePeriodicDid(didLoByte)                         (Dcm_UtiMake16Bit((uint8)0xF2u, (didLoByte)))                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_Svc2AMakeSchdRateFromUdsRate(udsRate)                   (Dcm_Svc2ASchedRateMemType)((udsRate)-1u)                                       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc2AScheduledDidLookUp()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of SID 0x2A.
 *  \details        Performs a lookup for a specific DID on scheduling requests.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_LOOP              Go to next step
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2AScheduledDidLookUp(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2ARepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2AStoppedDidLookUp()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of SID 0x2A.
 *  \details        Performs a lookup for a specific DID on stop DID requests.
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_LOOP              Go to next step
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2AStoppedDidLookUp(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2ARepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2ADidCheckCondition()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of SID 0x2A.
 *  \details        Checks the conditions of a requested DID.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2ADidCheckCondition(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2ARepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2ADidGetLength()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of SID 0x2A.
 *  \details        Returns the length of a requested DID.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Go on with next DID
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2ADidGetLength(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2ARepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2AIsUdsRateSupported()
 *********************************************************************************************************************/
/*! \brief          Checks whether the UDS rate is supported.
 *  \details        -
 *  \param[in]      udsRate                 Rate to be checked
 *  \return         TRUE                    Rate is supported
 *  \return         FALSE                   Rate is not supported
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_Svc2AIsUdsRateSupported(
  Dcm_Svc2ATransmissionModeType udsRate
  );

/**********************************************************************************************************************
 *  Dcm_Svc2ADoScheduleDids()
 *********************************************************************************************************************/
/*! \brief          Updates the scheduler content.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2ADoScheduleDids(
  Dcm_ContextPtrType pContext,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_Svc2AStopScheduledDids()
 *********************************************************************************************************************/
/*! \brief          Stops requested DIDs.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2AStopScheduledDids(
  Dcm_ContextPtrType pContext,
  Dcm_MsgContextPtrType pMsgContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2AHandler()
 *********************************************************************************************************************/
/*! \brief          Service 0x2A Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Operation failed
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2AHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2ARepeaterProxyContextPtrType pRepContext
  );
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc2ADidLookUp()
 *********************************************************************************************************************/
/*! \brief          A generic DID look-up for SID 0x2A.
 *  \details        Performs a lookup for a specific DID on any request.
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_PROCESSINGDONE    Send response
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_LOOP              Go to next step
 *  \return         DCM_E_OK                Found a DID, make next step
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2ADidLookUp(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2ARepeaterProxyContextPtrType pRepContext
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
 *  Dcm_Svc2AStoppedDidLookUp()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2AStoppedDidLookUp(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2ARepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdReturn;

  lStdReturn = Dcm_Svc2ADidLookUp(opStatus, pMsgContext, ErrorCode, pRepContext);                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdReturn == DCM_E_OK)
  {
    Dcm_Svc2ASchdStopItemByDid(pRepContext->DidInfoContext.Did);
    lStdReturn = DCM_E_LOOP;/* go on with the next DID  */
  }
  else if(lStdReturn == DCM_E_PROCESSINGDONE)
  {
    lStdReturn = DCM_E_OK;
  }
  else
  {
    /* DCM_E_PENDING, DCM_E_NOT_OK (ErrorCode already set), DCM_E_LOOP */
  }

  return lStdReturn;
}

/**********************************************************************************************************************
 *  Dcm_Svc2AScheduledDidLookUp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2AScheduledDidLookUp(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2ARepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdReturn;
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext = &(pRepContext->DidInfoContext);                                                                  /* PRQA S 3679 */ /* MD_Dcm_Design_3679 */

  lStdReturn = Dcm_Svc2ADidLookUp(opStatus, pMsgContext, ErrorCode, pRepContext);                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdReturn == DCM_E_OK)
  {
    lStdReturn = Dcm_DidMgrDynDidStateCheck(Dcm_NetGetConnHdlOfRxPduId(pMsgContext->rxPduId), pDidInfoContext, ErrorCode);                           /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */
    if(lStdReturn == DCM_E_OK)
    {
      /* Proceed with next validation step */
      Dcm_RepeaterNextStep(pContext, DCM_SVC2A_PROGRESS_DID_CHECKCONDITION); /* next job */                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdReturn = DCM_E_LOOP;/* speed up processing */
    } /* else DCM_E_NOT_OK (ErrorCode already set) */
  }
  else if(lStdReturn == DCM_E_PROCESSINGDONE)
  {
    lStdReturn = DCM_E_OK;
  }
  else
  {
    /* DCM_E_PENDING, DCM_E_NOT_OK (ErrorCode already set), DCM_E_LOOP */
  }

  return lStdReturn;
}

/**********************************************************************************************************************
 *  Dcm_Svc2ADidCheckCondition()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2ADidCheckCondition(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2ARepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult = DCM_E_LOOP;

  if(Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->DidInfoContext))
                                 ,DCM_DIDMGR_OPTYPE_READCHKCOND))
  {
    DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                             /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

    /* check condition on this DID */
    Dcm_DidMgrInitOpClassInfo(&(pRepContext->DidInfoContext), DCM_DIDMGR_OPTYPE_READCHKCOND);                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = Dcm_DidMgrReadCheckCond(opStatus
                                        ,&(pRepContext->DidInfoContext)
                                        ,&(pRepContext->DidOpTypeContext)
                                        ,ErrorCode);                                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

    switch (lStdResult)
    {
    case DCM_E_OK:
      lStdResult = DCM_E_LOOP;
      break; /* go on with next DID */
    case DCM_E_PENDING:
      break;
    default: /* DCM_E_NOT_OK */
      break; /* finish service processing immediately */
    }
  }/* else - not supported operation - go on with getLength */

  if (lStdResult == DCM_E_LOOP)
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC2A_PROGRESS_DID_GETLENGTH);                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  return lStdResult; /* speed up processing */
}

/**********************************************************************************************************************
 *  Dcm_Svc2ADidGetLength()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2ADidGetLength(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2ARepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdReturn = DCM_E_OK;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if(Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->DidInfoContext))
                                 ,DCM_DIDMGR_OPTYPE_READLENGTH))
  {
    Dcm_DidMgrInitOpClassInfo(&(pRepContext->DidInfoContext), DCM_DIDMGR_OPTYPE_READLENGTH);                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* Calculate DID data length if applicable */
    lStdReturn = Dcm_DidMgrReadLength(opStatus
                                     ,&(pRepContext->DidInfoContext)
                                     ,&(pRepContext->DidOpTypeContext)
                                     ,ErrorCode);                                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  if(lStdReturn == DCM_E_OK)
  {
    /* check for buffer overrun - uudt frame needs to be large enough for the periodic message (1 Byte DID + data) */
    if(pRepContext->DidInfoContext.DidLength >= Dcm_PbCfgNetGetConnectionInfo(pRepContext->ConnHdl).UudtFrameSize)
    {
      /* the configured buffer will not be able to hold all of the data -> notify the client */
      *ErrorCode = DCM_E_RESPONSETOOLONG;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK; /* finish service processing immediately */
    }
    else
    {
      /* Add scheduler job - set the operation now since not changed later */
      Dcm_DidMgrInitOpClassInfo(&(pRepContext->DidInfoContext), DCM_DIDMGR_OPTYPE_READ);                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */

      Dcm_Svc2ASchdAddItemByDid(&pRepContext->DidInfoContext);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */

      Dcm_RepeaterNextStep(pContext, DCM_SVC2A_PROGRESS_SCHEDULEDDID_LOOKUP);                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdReturn = DCM_E_LOOP;/* speed up processing */
    }
  }/* else - DCM_E_PENDING, DCM_E_NOT_OK */

  return lStdReturn;
}

/**********************************************************************************************************************
 *  Dcm_Svc2AIsUdsRateSupported()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_Svc2AIsUdsRateSupported(
  Dcm_Svc2ATransmissionModeType udsRate
  )
{
  sint16_least lLookUpResult;
  boolean      lResult = TRUE;

  lLookUpResult = Dcm_UtiLookUpUint8(Dcm_CfgSvc2ASupportedRatesLookUpTable, udsRate);                                                                /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

  if(lLookUpResult < 0)
  {
    lResult = FALSE;
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc2AStopScheduledDids()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2AStopScheduledDids(
  Dcm_ContextPtrType pContext,
  Dcm_MsgContextPtrType pMsgContext                                                                                                                  /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdReturn = DCM_E_OK;
  if(pMsgContext->reqDataLen == 0u)
  {
    Dcm_Svc2ASchdStopAll();
  }
  else
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC2A_PROGRESS_STOPPED_DID_LOOKUP); /* delegate job */                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdReturn = DCM_E_LOOP;/* speed up processing */
  }

  return lStdReturn;
}

/**********************************************************************************************************************
 *  Dcm_Svc2ADoScheduleDids()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2ADoScheduleDids(
  Dcm_ContextPtrType pContext,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdReturn = DCM_E_NOT_OK;

  if(pMsgContext->reqDataLen > Dcm_Svc2ASchdGetFreeSpace())
  {
    /*
     * not enough space in the scheduler table
     * (don't consider duplicates and unsupported DIDs in the request -> apply KISS)
     */
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC2A_PROGRESS_SCHEDULEDDID_LOOKUP); /* delegate job */                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdReturn = DCM_E_LOOP;/* speed up processing */
  }

  /* Reaching this point means diagnostic error was found */
  return lStdReturn;
}

/**********************************************************************************************************************
 *  Dcm_Svc2AHandler()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2AHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2ARepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType                     lStdReturn;
  Dcm_NetTransportObjectConstPtrType pTranspObj = Dcm_DiagGetTranspObj(pContext);

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  pRepContext->ConnHdl = pTranspObj->ConnHdl;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */

# if (DCM_NET_PROTECT_SESSION_OF_CLIENT_ENABLED == STD_ON)
  /* protected by the session connection ownership */
# else
  /* In case of generic connections, a connection can be used by a pool of testers. Tester address has to be checked as well. */
  if((Dcm_NetPeriodicMsgGetConnection() != DCM_NET_INVALID_CONNHDL)
    && ((Dcm_NetPeriodicMsgGetConnection() != pRepContext->ConnHdl)                                                                                  /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
#  if(DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
    || ((Dcm_NetCfgIsGenericConnection(Dcm_NetGetActiveComVariant(), Dcm_NetPeriodicMsgGetConnection()))                                             /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
    && (pTranspObj->ClientSrcAddr != Dcm_NetPeriodicMsgGetClientSrcAddr()))                                                                          /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
#  endif
    ))
  {
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT; /* the 2A service is currently in use by another client */                                              /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
# endif
    if(!Dcm_NetPeriodicMsgSupportedFor(Dcm_DiagGetTranspObj(pContext)->ConnHdl))
    {
      *ErrorCode = DCM_E_PANIC_NRC; /* current client does not support periodic messages */                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK;
    }
    else
    {
      pRepContext->HasAnySupportedDids = FALSE;                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      Dcm_UtiConsumeReqDataAsU8(pMsgContext, &(pRepContext->UdsRate));                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */

      /* Maximum number of DIDs per single request service shall not be exceeded.
      * Check minimum length for rates excluding stopSending */
      if((pMsgContext->reqDataLen > DCM_SVC_2A_MAX_DID_LIST_LEN)
        || ((pMsgContext->reqDataLen == 0u) && (pRepContext->UdsRate != DCM_SVC_2A_STOP_SENDING)))
      {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdReturn = DCM_E_NOT_OK;
      }
      else
      {
        if(!Dcm_Svc2AIsUdsRateSupported(pRepContext->UdsRate))
        {
          *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdReturn = DCM_E_NOT_OK;
        }
        else
        {
          if(pRepContext->UdsRate == DCM_SVC_2A_STOP_SENDING)
          {
            lStdReturn = Dcm_Svc2AStopScheduledDids(pContext, pMsgContext);                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
          }
          else
          {
            lStdReturn = Dcm_Svc2ADoScheduleDids(pContext, pMsgContext, ErrorCode);                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
          }
        }
      }
    }

  return lStdReturn;
}                                                                                                                                                    /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc2ADidLookUp()
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
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2ADidLookUp(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2ARepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdReturn;
  boolean doProcessDid = TRUE;

  if(opStatus == DCM_INITIAL)
  {
    if(pMsgContext->reqDataLen != 0u)
    {
      uint8 perDidLoByte;
      Dcm_UtiConsumeReqDataAsU8(pMsgContext, &perDidLoByte);                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */

      pRepContext->DidInfoContext.Did = Dcm_Svc2AMakePeriodicDid(perDidLoByte);                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      doProcessDid = FALSE;
    }
  }

  if(doProcessDid == TRUE)
  {
    lStdReturn = Dcm_DidMgrGetDidInfo(opStatus
                                     ,&(pRepContext->DidInfoContext)
                                     ,&(pRepContext->DidOpTypeContext)
                                     ,DCM_DIDMGR_OP_READ);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
    if(lStdReturn == DCM_E_OK)
    {
      pRepContext->HasAnySupportedDids = TRUE; /* register a supported DID */                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
      /* go on with the DID condition check - DCM_E_OK */
    }
    else if(lStdReturn == DCM_E_PENDING)
    {
      /* already set */
    }
    else
    {
      /* Just skip this DID - DCM_E_NOT_OK - request a new client loop */
      lStdReturn = DCM_E_LOOP;
    }
  }
  else
  {
    /* empty run loop ended without any DID to be processed (for this activation of the repeater proxy) */
    /* all DIDs processed */
    if (pRepContext->HasAnySupportedDids == FALSE)
    {
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK;
    }
    else
    {
      lStdReturn = DCM_E_PROCESSINGDONE;
    }
  }

  return lStdReturn;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Service2AInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Service2AInit(
  void
  )
{
  Dcm_Service2ASchedulerInit();
}

/**********************************************************************************************************************
 *  Dcm_Service2AProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service2AProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc2ARepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc2A;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc2AHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC2A_PROGRESS_SCHEDULEDDID_LOOKUP:
    lStdResult = Dcm_Svc2AScheduledDidLookUp(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                               /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC2A_PROGRESS_DID_CHECKCONDITION:
    lStdResult = Dcm_Svc2ADidCheckCondition(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC2A_PROGRESS_DID_GETLENGTH:
    lStdResult = Dcm_Svc2ADidGetLength(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                     /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC2A_PROGRESS_STOPPED_DID_LOOKUP:
    lStdResult = Dcm_Svc2AStoppedDidLookUp(opStatus, pMsgContext, ErrorCode, pRepContext);                                                           /* SBSW_DCM_COMB_PTR_FORWARD */
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
 *  Dcm_Service2APostProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service2APostProcessor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_ConfirmationStatusType status
  )
{
  if(pContext->Repeater.Context.Svc2A.UdsRate != DCM_SVC_2A_STOP_SENDING)
  {
    if(status == DCM_RES_POS_OK)
    {
      Dcm_Svc2ASchdCommit(Dcm_Svc2AMakeSchdRateFromUdsRate(pContext->Repeater.Context.Svc2A.UdsRate));
      Dcm_NetPeriodicMsgSetConnection(pContext->Repeater.Context.Svc2A.ConnHdl);
      Dcm_NetPeriodicMsgSetClientSrcAddr(Dcm_DiagGetTranspObj(pContext)->ClientSrcAddr);
    }
    else
    {
      Dcm_Svc2ASchdDiscard();
    }
  }/* else - nothing */
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_2A_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc2A.c
 *********************************************************************************************************************/
