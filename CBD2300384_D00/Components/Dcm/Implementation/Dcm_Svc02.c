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
/**        \file  Dcm_Svc02.c
 *         \unit  Svc02
 *        \brief  Contains the implementation of Service 0x02 unit.
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
#define DCM_SVC02_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc02.h"
#if (DCM_SVC_02_SUPPORT_ENABLED == STD_ON)
# include "Dcm_DemIf.h"
# include "Dcm_Repeater.h"
# include "Dcm_Debug.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Service 0x02 processing steps */
# define DCM_SVC02_PROGRESS_REPEATERPROXY                            ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))

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
 *  Dcm_Svc02Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x02 Handler.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc02Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc02RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc02_XX_RepeaterProxy()
 *********************************************************************************************************************/
/*! \brief          Service 0x02 worker.
 *  \details        -
 *  \param[in]      opStatus                The operations status
 *  \param[in]      pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send positive response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc02_XX_RepeaterProxy(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc02RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc02ReadPidDataCommon()
 *********************************************************************************************************************/
/*! \brief          Reads a single valid PID data via callout function.
 *  \details        -
 *  \param[in]      pid          The PID which data shall be read
 *  \param[in,out]  pMsgContext  The message context
 *  \return         DCM_E_OK        Send positive response
 *  \return         DCM_E_NOT_OK    Send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            PID != 2
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc02ReadPidDataCommon(
  uint8 pid,
  Dcm_MsgContextPtrType pMsgContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc02ReadPidData()
 *********************************************************************************************************************/
/*! \brief          Reads a single valid PID data.
 *  \details        -
 *  \param[in]      pid          The PID which data shall be read
 *  \param[in,out]  pMsgContext  The message context
 *  \param[in,out]  pRepContext  Current repeater proxy context
 *  \return         DCM_E_OK        Send positive response
 *  \return         DCM_E_NOT_OK    Send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc02ReadPidData(
  uint8 pid,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Svc02RepeaterProxyContextPtrType pRepContext
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
 *  Dcm_Svc02Handler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc02Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc02RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* At maximum 3 PID + 3 FF can be requested */
  if ((pMsgContext->reqDataLen <= 6u)
    && (Dcm_UtiIsEven(pMsgContext->reqDataLen)))
  {
    pRepContext->ParserDesc.LookUpTable = Dcm_CfgSvc02LookUpTable;                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->ParserDesc.LookUpFilterTable = Dcm_CfgSvc02ExecPrecondTable;                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->ParserDesc.MaskData = Dcm_CfgSvc02SupportedIdMask;                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->DemClientId = Dcm_DiagGetDemClientId(pMsgContext);                                                                                  /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_WRITE */ /* SBSW_DCM_PARAM_PTR_FORWARD */

    Dcm_RepeaterNextStep(pContext, DCM_SVC02_PROGRESS_REPEATERPROXY);                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;
  }
  else
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc02_XX_RepeaterProxy()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc02_XX_RepeaterProxy(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc02RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  Dcm_ObdIdMgrHandleOptType lPidIter = (Dcm_ObdIdMgrHandleOptType)pMsgContext->reqDataLen;
  uint8_least lPidTypeMask = 0x00u;
  uint8 lPidList[6];
  uint8 lFreezeFrameId;
  uint8 lPid;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Store the requested data */
  Dcm_UtiConsumeReqDataAsUN(pMsgContext, lPidList, 6u, pMsgContext->reqDataLen);                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */

  do
  {
    /* OBD specification does not specify the order of PIDs in the response, thus it is ok to scan the requested PIDs backwards */
    --lPidIter;      /* navigate to the FF */
    lFreezeFrameId = lPidList[lPidIter];
    --lPidIter;      /* navigate to the PID */
    lPid = lPidList[lPidIter];

    lPidTypeMask |= (uint8)(Dcm_ObdIdMgrIsAvailabilityId(lPid) ? 0x01u : 0x02u);  /* mark availability/DATA PID found (never matter whether it is supported or not!) */

    if (lPidTypeMask != 0x03u)
    {
      if ((Dcm_ObdIdMgrIdLookUp(lPid, &(pRepContext->ParserDesc)) == DCM_E_OK)                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
        && (lFreezeFrameId == 0x00u)) /* Accept only FF = 0 */
      {
        if (Dcm_Svc02ReadPidData(lPid, pMsgContext, pRepContext) == DCM_E_OK)                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
        {
          lStdResult = DCM_E_OK;
        }
      }/* else:
       - FreezeFrame != 0 --> ignore
       - PID not supported --> ignore */
    }
    else
    {
      /* Otherwise return NRC 0x31 (Mix of Data and Availability PIDs is not allowed) */
      lStdResult = DCM_E_NOT_OK;
      break;
    }
  } while (lPidIter != 0u);

  /* If no PID was read send NRC 0x31 */
  if (lStdResult == DCM_E_NOT_OK)
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc02ReadPidDataCommon()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc02ReadPidDataCommon(
  uint8 pid,
  Dcm_MsgContextPtrType pMsgContext
  )
{
  Std_ReturnType lStdResult;

  Dcm_MsgLenType lBufSize;
  Dcm_MsgType    lFreezeFrameDataBuffer;

  lBufSize = Dcm_UtiGetRemainingResLenWithOffset(pMsgContext, 2u);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
  lFreezeFrameDataBuffer = Dcm_UtiGetResDataRel(pMsgContext, 2u);                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */

  lStdResult = Dcm_DemIfReadDataOfOBDFreezeFrame(pid
                                                ,0x00u /* DCM does not know the data index, so 0x00 is used here */
                                                ,lFreezeFrameDataBuffer
                                                ,&lBufSize);                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */

  if ((lStdResult == E_OK)
    && (lBufSize > 0u))
  {
    Dcm_UtiProvideResDataAsU8(pMsgContext, pid);                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_UtiProvideResDataAsU8(pMsgContext, 0x00u); /* Only FF 0x00 is supported */                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* PID have been successfully read */
    Dcm_UtiProvideResData(pMsgContext, lBufSize);                                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    /* else - PID can not be read or no data stored --> ignore */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc02ReadPidData()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc02ReadPidData(
  uint8 pid,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Svc02RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  /*
  * Check for sufficent buffer - It is ensured by the buffer size validation,
  * that the buffer is large enough to hold three availability PIDs plus data in the response
  */
  if (pMsgContext->resMaxDataLen < 4u)
  {
    lStdResult = DCM_E_NOT_OK;
  }
  else if (Dcm_ObdIdMgrIsAvailabilityId(pid))
  {
    Dcm_UtiProvideResDataAsU8(pMsgContext, pid);                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_UtiProvideResDataAsU8(pMsgContext, 0x00u); /* Only FF 0x00 is supported */                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_UtiProvideResDataAsU32(pMsgContext, Dcm_ObdIdMgrGetAvailabilityMask(pid, &(pRepContext->ParserDesc)));                                       /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else if (pid == 0x02u)
  {
    uint32 lDtc;

    Dcm_UtiProvideResDataAsU8(pMsgContext, 0x02u);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_UtiProvideResDataAsU8(pMsgContext, 0x00u); /* Only FF 0x00 is supported */                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */

    if (Dcm_DemIfGetDTCOfOBDFreezeFrame(0x00u, &lDtc, DEM_DTC_FORMAT_OBD) != E_OK)                                                                   /* SBSW_DCM_POINTER_FORWARD_STACK */
    {
      lDtc = 0x0000u;
    }

    /* PID 0x02: DTC that stored the freeze frame */
    Dcm_UtiProvideResDataAsU8(pMsgContext, Dcm_UtiGetHiLoByte(lDtc));                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_UtiProvideResDataAsU8(pMsgContext, Dcm_UtiGetLoHiByte(lDtc));                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    /* other PID: get data from freeze frame */
    lStdResult = Dcm_Svc02ReadPidDataCommon(pid, pMsgContext);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
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
 *  Dcm_Service02Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service02Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType  lStdResult;

  Dcm_Svc02RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc02;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc02Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC02_PROGRESS_REPEATERPROXY:
    lStdResult = Dcm_Svc02_XX_RepeaterProxy(opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
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

#endif /* (DCM_SVC_02_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc02.c
 *********************************************************************************************************************/
