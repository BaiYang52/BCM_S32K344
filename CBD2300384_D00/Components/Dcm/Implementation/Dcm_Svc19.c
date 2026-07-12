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
/**        \file  Dcm_Svc19.c
 *         \unit  Svc19
 *        \brief  Contains the implementation of Service 0x19 unit.
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
#define DCM_SVC19_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc19.h"
#if (DCM_SVC_19_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Svc19Uti.h"
# include "Dcm_CfgWrap.h"
# include "Dcm_AuthMgr.h"

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
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc19Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x19 Handler.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc19SubFuncHandler()
 *********************************************************************************************************************/
/*! \brief          Service 0x19 SubFunction Handler.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19SubFuncHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc19ReqLengthGetter()
 *********************************************************************************************************************/
/*! \brief          Service 0x19 sub-function length getter.
 *  \details        Always returns the sub-function specific length.
 *  \param[in]      sfRef                The sub-function zero-based index.
 *  \return         Expected sub-function specific length.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc19ReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
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
 *  Dcm_Svc19Handler()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType                lStdResult;
  Dcm_DiagSubServiceRefOptType  lSubSvcRef;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_DiagVerifyServiceWithSubFunction(pMsgContext
                                                   ,Dcm_CfgSvc19SubFuncLookUpTable
                                                   ,Dcm_CfgSvc19SubFuncExecPrecondTable
                                                   ,Dcm_Svc19ReqLengthGetter
                                                   ,Dcm_DiagNoSequenceChecker
                                                   ,&lSubSvcRef
                                                   ,ErrorCode);                                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */

  pRepContext->DemClientId = Dcm_DiagGetDemClientId(pMsgContext);                                                                                    /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_GLOBAL_PTR_WRITE */ /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)
  {
    pRepContext->SubSvcRef = (Dcm_DiagSubServiceRefMemType)lSubSvcRef;                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */

    Dcm_RepeaterNextStep(pContext, DCM_SVC19_PROGRESS_SUBFUNCTION); /* delegate job */                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */

    lStdResult = DCM_E_LOOP;/* speed up processing */
  } /* else DCM_E_NOT_OK (ErrorCode is already set) */

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc19SubFuncHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19SubFuncHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                 lStdResult;
  Dcm_CfgSvc19SubFuncInfoPtrType pSubFuncInfo;

  lStdResult = Dcm_CfgWrapSvc19SubFuncInfoGetEntry(pRepContext->SubSvcRef, &pSubFuncInfo, ErrorCode);                                                /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lStdResult == DCM_E_OK)                                                                                                                        /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    lStdResult = pSubFuncInfo->SubSvcFunc(pContext, opStatus, pMsgContext, ErrorCode);                                                               /* SBSW_DCM_CALL_FUNCPTR_SVC19SUBFUNC */
  }

  return lStdResult;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc19ReqLengthGetter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc19ReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  )
{
  /* Return sub-function specific length */
  return (Dcm_CfgWrapSvc19SubFuncInfo(sfRef))->ReqLength;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Service19Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc19Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC19_PROGRESS_SUBFUNCTION:
    lStdResult = Dcm_Svc19SubFuncHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                   /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# if (DCM_SVC_19_UTI_COPY_LINEAR_DATA_ENABLED == STD_ON)
  case DCM_SVC19_PROGRESS_UTI_COPYLINEARDATA:
    lStdResult = Dcm_Svc19UtiCopyLinearData(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_DEMIF_SELECT_DTC_ENABLED == STD_ON)
  case DCM_SVC19_PROGRESS_DEMCHAIN_SELECT_DTC:
    lStdResult = Dcm_Svc19UtiDemSelectDTC(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                  /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_SVC_19_UTI_ANYABLERECORDUPDATE_ENABLED == STD_ON)
  case DCM_SVC19_PROGRESS_DEMCHAIN_DISABLERECORDUPDATE:
    lStdResult = Dcm_Svc19UtiDemDisableRecordUpdate(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                        /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_SVC_19_UTI_GETSTATUSOFDTC_ENABLED == STD_ON)
  case DCM_SVC19_PROGRESS_DEMCHAIN_GETSTATUSOFDTC:
    lStdResult = Dcm_Svc19UtiDemGetStatusOfDtc(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                             /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_DEMIF_SELECT_FFR_ENABLED == STD_ON)
  case DCM_SVC19_PROGRESS_DEMCHAIN_SELECTFFREC:
    lStdResult = Dcm_Svc19UtiDemSelectFFData(pContext, ErrorCode, pRepContext);                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_SVC_19_UTI_GETSIZEOFFFRECORD_ENABLED == STD_ON)
  case DCM_SVC19_PROGRESS_DEMCHAIN_GETSIZEOFFFREC:
    lStdResult = Dcm_Svc19UtiDemGetSizeOfFFRecord(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_DEMIF_SELECT_EDR_ENABLED == STD_ON)
  case DCM_SVC19_PROGRESS_DEMCHAIN_SELECTEXTDATAREC:
    lStdResult = Dcm_Svc19UtiDemSelectExtRecord(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                            /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_DEMIF_SVC_19_GETSIZEOFEXTRECORD_ENABLED == STD_ON)
  case DCM_SVC19_PROGRESS_DEMCHAIN_GETSIZEOFEXTDATAREC:
    lStdResult = Dcm_Svc19UtiDemGetSizeOfExtRecord(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                         /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_SVC_19_UTI_GETNUMFILTEREDDTC_ENABLED == STD_ON)
  case DCM_SVC19_PROGRESS_DEMCHAIN_GETNUMFLTRDDTC:
    lStdResult = Dcm_Svc19UtiDemGetNumFltrdDtc(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                             /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_SVC_19_08_SUPPORT_ENABLED == STD_ON)
  case DCM_SVC19_PROGRESS_DEMCHAIN_GETNEXTFLTRDSEVERITY:
    lStdResult = Dcm_Svc19UtiDemGetNxtFltrdSeverity(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                        /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_SVC_19_UTI_GETSEVERITYOFDTC_ENABLED == STD_ON)
  case DCM_SVC19_PROGRESS_DEMCHAIN_GETSEVERITYOFDTC:
    lStdResult = Dcm_Svc19UtiDemGetSeverityOfDtc(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                           /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_SVC_19_UTI_GETSEVERITYOFDTC_ENABLED == STD_ON)
  case DCM_SVC19_PROGRESS_DEMCHAIN_GETFUNCUNITOFDTC:
    lStdResult = Dcm_Svc19UtiDemGetFuncUnitOfDtc(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                           /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_DEMIF_SVC_19_GETNEXTFILTEREDDTC_ENABLED == STD_ON)
  case DCM_SVC19_PROGRESS_DEMCHAIN_GETNEXTFLTRDDTC:
    lStdResult = Dcm_Svc19UtiDemGetNxtFltrdDtc(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                             /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_SVC_19_UTI_READ_DTC_CNT_BY_STATUS_MASK_ENABLED == STD_ON)
  case DCM_SVC19_PROGRESS_DEMCHAIN_PUTNUMFLTRDDTC:
    lStdResult = Dcm_Svc19UtiPutNumFilteredDtc(opStatus, pMsgContext, ErrorCode, pRepContext);                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_SVC_19_14_SUPPORT_ENABLED == STD_ON)
  case DCM_SVC19_PROGRESS_DEMCHAIN_GETNEXTFLTRDFDC:
    lStdResult = Dcm_Svc19UtiDemGetNxtFltrdFDC(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                             /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_SVC_19_42_SUPPORT_ENABLED == STD_ON)
  case DCM_SVC19_PROGRESS_DEMCHAIN_WWHOBD_GETNEXTFLTRDSEVERITY:
    lStdResult = Dcm_Svc19UtiWwhObdDemGetNxtFltrdSeverity(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                  /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_SVC_19_UTI_CHAIN_END_ENABLED == STD_ON)
  case DCM_SVC19_PROGRESS_CHAIN_END:
    lStdResult = DCM_E_OK;
    break;
# endif
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6030, 6050 */ /* MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Dcm_Service19PostProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service19PostProcessor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_ConfirmationStatusType status
  )
{
# if (DCM_SVC_19_UTI_ANYABLERECORDUPDATE_ENABLED == STD_ON)
#  if (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
  Dcm_Svc19RepeaterProxyContextConstPtrType pRepContext = &pContext->Repeater.Context.Svc19;
#  endif
# endif

# if (DCM_SVC_19_UTI_ANYABLERECORDUPDATE_ENABLED == STD_ON)
#  if (DCM_DEMIF_421_ENABLED == STD_ON)
  (void)Dcm_DemIf(EnableDTCRecordUpdate)();
#  else
  (void)Dcm_DemIf(EnableDTCRecordUpdate)(pRepContext->DemClientId);
#  endif
# endif
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(status);                                                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

# if (DCM_DEMIF_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19Updater()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19Updater(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiCopyData(pContext, pDataContext, ErrorCode, pRepContext);                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_01Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_01Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiReportNumDtcByStatusMaskData(pContext
                                                 ,DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_01
                                                 ,0
                                                 ,pMsgContext
                                                 ,ErrorCode
                                                 ,pRepContext);                                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_02Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_02Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiReportDtcByStatusMaskData(pContext
                                              ,DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_01
                                              ,DCM_SVC_19_UTI_USE_MASK_REQ
                                              ,pMsgContext
                                              ,ErrorCode
                                              ,pRepContext);                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_03Processor()
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_03Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType               lStdResult;
  Dcm_DemIfReturnSetFilterType demResult;
  uint16                       numOfRecords;
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  demResult = Dcm_DemIf(SetFreezeFrameRecordFilter)(
#  if (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
                                                    pRepContext->DemClientId,
#  endif
                                                    DEM_DTC_FORMAT_UDS,
                                                    &numOfRecords);                                                                                  /* SBSW_DCM_POINTER_FORWARD_STACK */
  if (demResult == DCM_DEMIF_RET_FILTER_ACCEPTED)
  {
    lStdResult =  Dcm_Svc19UtiStartReporting(pContext
                                           ,pMsgContext
                                           ,DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_RECORD
                                           ,((Dcm_MsgLenType)numOfRecords * 4u)
                                           ,pRepContext);                                                                                            /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
  }
  else
  { /* Any DEM error */
    *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(demResult, Dcm_DemIfNrcMapSetDTCFilter);                                                              /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_19_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_04Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_04Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  )
{
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiReportDtcFFRByDtc(pContext, DEM_DTC_ORIGIN_PRIMARY_MEMORY, pMsgContext, pRepContext);                                           /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_05_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_05Processor()
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_05Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdReturn;
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &(pRepContext->RecordNum));                                                                                 /* SBSW_DCM_COMB_PTR_FORWARD */

  if( (pRepContext->RecordNum == 0x00u)
    ||(pRepContext->RecordNum == DCM_SVC_19_UTI_RECNUM_ALL) )
  {
    uint16 recordLength;
    Dcm_MsgType freezeFrameDataBuffer;
    Dcm_DemIfReturnGetFreezeFrameDataByRecordType demResult;

    /* Report immediately the only possible SSRN (0x00) */
    Dcm_UtiProvideResDataAsU8(pMsgContext, 0x00u);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    /* Calculate remaining length for SSRN data provision by DEM
     * (i.e. subtract the DTC (3bytes) and status mask (1 Byte) from the current available response buffer, that will be written later)
     */
    recordLength = (uint16)Dcm_UtiGetRemainingResLenWithOffset(pMsgContext, 4u);                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
    freezeFrameDataBuffer = Dcm_UtiGetResDataRel(pMsgContext, 4u);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */

    pRepContext->Origin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;                                                                                             /* SBSW_DCM_GLOBAL_PTR_WRITE */
    /* Gather the SSRN data (with offset of 4 bytes to the current position) and DTC number */
    demResult = Dcm_DemIfGetOBDFreezeFrameData(0x00u                                                                                                 /* SBSW_DCM_COMB_PTR_FORWARD */
                                              ,pRepContext->Origin
                                              ,&pRepContext->Dtc
                                              ,freezeFrameDataBuffer
                                              ,&recordLength);
    switch(demResult)
    {
    case DCM_DEMIF_RET_FFBYRECORD_OK:
      /* Commit the reported SSRN data */
      Dcm_UtiUpdateResLength(pMsgContext, recordLength);                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
      /* Delegate the service processing to the "DTCAndStatusRecord" reporting utility (i.e. writes DTC and its status into the response) */
      lStdReturn = Dcm_Svc19UtiChainStart(pContext, Dcm_Svc19UtiDemChainGetDtcStatusEnd, pRepContext);                                               /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
      break;
    case DCM_DEMIF_RET_FFBYRECORD_NO_DTC:
      /* Otherwise, if the DEM has found no matching DTC, just return the SSRN with no further data */
      lStdReturn = DCM_E_OK;
      break;
    default:
      *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(demResult, Dcm_Svc19UtiNrcMapGetOBDFreezeFrameData);                                                /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK;
      break;
    }
  }
  else
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }
  return lStdReturn;
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */
# endif

# if (DCM_SVC_19_06_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_06Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_06Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiReportExtDataRecordByDtcNum(pContext
                                                ,DEM_DTC_ORIGIN_PRIMARY_MEMORY
                                                ,pMsgContext
                                                ,ErrorCode
                                                ,pRepContext);                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_07Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_07Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  uint8 reqDtcSeverityMask;
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &reqDtcSeverityMask);                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */

  return Dcm_Svc19UtiReportNumDtcByStatusMaskData(pContext
                                                 ,DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_02
                                                 ,reqDtcSeverityMask
                                                 ,pMsgContext
                                                 ,ErrorCode
                                                 ,pRepContext);                                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_08_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_08Processor()
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_08Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                    lStdResult;
  uint8                             dtcStatusMask;
  uint8                             dtcSeverityMask;
  uint8                             dtcSAM;
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* save the requested masks */
  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &dtcSeverityMask);                                                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &dtcStatusMask);                                                                                            /* SBSW_DCM_COMB_PTR_FORWARD */

  /* Gather the configured DEM Status Availability Mask */
  lStdResult = Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask(&dtcSAM, ErrorCode, pRepContext);                                                         /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == E_OK)
  {
    Dcm_UtiProvideResDataAsU8(pMsgContext, dtcSAM);                                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */

    Dcm_Svc19UtiPatchDtcStatusSetForFilter(&dtcStatusMask);                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */

    /* Form the relevant status bits */
    dtcStatusMask &= dtcSAM;

    if(dtcStatusMask == 0u)
    {
      lStdResult = DCM_E_OK;/* no DTCs to report */
    }
    else
    {
      /* filter the DTCs */
      lStdResult = Dcm_Svc19UtiDemSetDTCFilter(ErrorCode
                                              ,DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_02
                                              ,dtcStatusMask
                                              ,dtcSeverityMask
                                              ,DCM_DEMIF_FILTER_FOR_FDC_NO
                                              ,pRepContext);                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
      if(lStdResult == DCM_E_OK)
      {
        lStdResult = Dcm_Svc19UtiChainStart(pContext, Dcm_Svc19Uti19_08DemChain, pRepContext);                                                       /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
      }/* Otherwise, Already set by Dcm_Svc19UtiDemSetDTCFilter */
    }
  }/* Otherwise, Already set by Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask */

  return lStdResult;
}
# endif

# if (DCM_SVC_19_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_09Processor()
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_09Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  uint8 dtcSAM;

  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  Dcm_UtiConsumeReqDataAsU24(pMsgContext, &(pRepContext->Dtc));                                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */

  /* Gather the configured DEM Status Availability Mask */
  lStdResult = Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask(&dtcSAM, ErrorCode, pRepContext);                                                         /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == E_OK)
  {
    Dcm_UtiProvideResDataAsU8(pMsgContext, dtcSAM);                                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */

    pRepContext->Origin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;                                                                                             /* SBSW_DCM_GLOBAL_PTR_WRITE */
    lStdResult = Dcm_Svc19UtiChainStart(pContext, Dcm_Svc19Uti19_09DemChain, pRepContext);                                                           /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
  }/* Otherwise, Already set by Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask */
  return lStdResult;
}
# endif

# if (DCM_SVC_19_0A_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_0AProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_0AProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiReportDtcByStatusMaskData(pContext
                                              ,DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_01
                                              ,DCM_SVC_19_UTI_USE_MASK_ALL
                                              ,pMsgContext
                                              ,ErrorCode
                                              ,pRepContext);                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_0B_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_0BProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_0BProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiReportNumDtcByOccurenceTime(pContext
                                                ,DEM_FIRST_FAILED_DTC
                                                ,pMsgContext
                                                ,ErrorCode
                                                ,pRepContext);                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_0C_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_0CProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_0CProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiReportNumDtcByOccurenceTime(pContext
                                                ,DEM_FIRST_DET_CONFIRMED_DTC
                                                ,pMsgContext
                                                ,ErrorCode
                                                ,pRepContext);                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_0D_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_0DProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_0DProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiReportNumDtcByOccurenceTime(pContext
                                                ,DEM_MOST_RECENT_FAILED_DTC
                                                ,pMsgContext
                                                ,ErrorCode
                                                ,pRepContext);                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_0E_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_0EProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_0EProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiReportNumDtcByOccurenceTime(pContext
                                                ,DEM_MOST_REC_DET_CONFIRMED_DTC
                                                ,pMsgContext
                                                ,ErrorCode
                                                ,pRepContext);                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_0F_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_0FProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_0FProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiReportDtcByStatusMaskData(pContext
                                              ,DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_03
                                              ,DCM_SVC_19_UTI_USE_MASK_REQ
                                              ,pMsgContext
                                              ,ErrorCode
                                              ,pRepContext);                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_10_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_10Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_10Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiReportExtDataRecordByDtcNum(pContext
                                                ,DEM_DTC_ORIGIN_MIRROR_MEMORY
                                                ,pMsgContext
                                                ,ErrorCode
                                                ,pRepContext);                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_11_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_11Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_11Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiReportNumDtcByStatusMaskData(pContext
                                                 ,DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_03
                                                 ,0
                                                 ,pMsgContext
                                                 ,ErrorCode
                                                 ,pRepContext);                                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif
# if (DCM_SVC_19_12_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_12Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_12Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiReportNumDtcByStatusMaskData(pContext
                                                 ,DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_04
                                                 ,0
                                                 ,pMsgContext
                                                 ,ErrorCode
                                                 ,pRepContext);                                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_13_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_13Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_13Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiReportDtcByStatusMaskData(pContext
                                              ,DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_04
                                              ,DCM_SVC_19_UTI_USE_MASK_REQ
                                              ,pMsgContext
                                              ,ErrorCode
                                              ,pRepContext);                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif
# if (DCM_SVC_19_14_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_14Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_14Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType           lStdResult;
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_Svc19UtiDemSetDTCFilter(ErrorCode
                                          ,DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_01
                                          ,0x00u
                                          ,0u /* not used */
                                          ,DCM_DEMIF_FILTER_FOR_FDC_YES
                                          ,pRepContext);                                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */
  if(lStdResult == DCM_E_OK)
  {
    lStdResult = Dcm_Svc19UtiChainStart(pContext, Dcm_Svc19Uti19_14DemChain, pRepContext);                                                           /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
  }
  else
  {
    /* Already set by Dcm_Svc19UtiDemSetDTCFilter */
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_19_15_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_15Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_15Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiReportDtcByStatusMaskData(pContext
                                              ,DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_05
                                              ,DCM_SVC_19_UTI_USE_MASK_ALL
                                              ,pMsgContext
                                              ,ErrorCode
                                              ,pRepContext);                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_16_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_16Processor()
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_16Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                       lStdResult;
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &(pRepContext->RecordNum));                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

  if( (pRepContext->RecordNum == DCM_SVC_19_UTI_RECNUM_ALL)
    ||(pRepContext->RecordNum == DCM_SVC_19_UTI_RECNUM_OBD_ALL) )
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
  {
    Dcm_UtiProvideResDataAsU8(pMsgContext, pRepContext->RecordNum);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */

    lStdResult = Dcm_Svc19UtiDemSetExtDataRecordFilter(ErrorCode, pRepContext);                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */

    if (lStdResult == DCM_E_OK)
    {
      Std_ReturnType lDemResult;
      uint16         lNumRecords;
      uint16         lRecordSizeSum;

      /* The record size for a specific record number is constant but varies for different record numbers */
      lDemResult = Dem_GetSizeOfFilteredExtendedDataRecords(pRepContext->DemClientId
                                                           ,&lNumRecords
                                                           ,&lRecordSizeSum);                                                                        /* SBSW_DCM_POINTER_FORWARD_STACK */

      if (lDemResult == E_OK)
      {
        if (lRecordSizeSum != 0u) /* consider only filtered records */
        {
          /* Response size = (NumRecords * DTCAndStatusRecord) + DTCExtDataRecordSum) */
          lStdResult = Dcm_Svc19UtiStartReporting(pContext
                                                ,pMsgContext
                                                ,DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_EXT_RECORD
                                                ,((lNumRecords * (Dcm_MsgLenType)4u) + lRecordSizeSum)
                                                ,pRepContext);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
        }
        else
        {
          /* Otherwise, proceed with response transmission with no further data */
          lStdResult = DCM_E_OK;
        }
      }
      else
      {
        *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
      }
    } /* Otherwise, ErrorCode already set by Dcm_Svc19UtiDemSetExtDataRecordFilter */
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_19_17_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_17Processor()
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_17Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext;
  uint8 lUserMemId;

  lUserMemId = Dcm_UtiGetReqDataAsU8Rel(pMsgContext, 1u);
  pRepContext = &pContext->Repeater.Context.Svc19;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_SVC_19_USER_MEM_ID_CHECK_ENABLED == STD_ON)
  if(lUserMemId != DCM_SVC_19_USER_MEM_ID)
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
#   if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
  if(Dcm_AuthMgrCheckMem(pContext, DCM_SVC_19_USERMEM_ID_PRECON_REF, lUserMemId) == DCM_E_NOT_OK)                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
    *ErrorCode = DCM_E_AUTHENTICATIONREQUIRED;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
#   endif
#  endif
  {
    Dcm_DiagBufferIndexType resUserMemIdx = pMsgContext->resIndex;

    /* Set the UserMid in response now */
    Dcm_UtiProvideResData(pMsgContext, 1u);                                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */

#  if (DCM_DEMIF_431_ENABLED == STD_ON)
    pRepContext->Origin = lUserMemId + 0x0100u;                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
#  endif

    lStdResult = Dcm_Svc19UtiReportDtcByStatusMaskData(pContext
                                                      ,DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_06
                                                      ,DCM_SVC_19_UTI_USE_MASK_REQ
                                                      ,pMsgContext
                                                      ,ErrorCode
                                                      ,pRepContext);                                                                                 /* SBSW_DCM_COMB_PTR_FORWARD */
    /* place the UserMid to the response */
    Dcm_UtiSetResDataAsU8At(pMsgContext, resUserMemIdx, lUserMemId);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_19_18_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_18Processor()
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_18Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  )
{
  Std_ReturnType lStdResult;
  uint8 lUserMemId;
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext;

  pRepContext = &pContext->Repeater.Context.Svc19;
  lUserMemId = Dcm_UtiGetReqDataAsU8Rel(pMsgContext, 4u);

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_SVC_19_USER_MEM_ID_CHECK_ENABLED == STD_ON)
  if (lUserMemId != DCM_SVC_19_USER_MEM_ID)
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
#   if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
  if(Dcm_AuthMgrCheckMem(pContext, DCM_SVC_19_USERMEM_ID_PRECON_REF, lUserMemId) == DCM_E_NOT_OK)                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
    *ErrorCode = DCM_E_AUTHENTICATIONREQUIRED;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
#   endif
#  endif
  {
    /* save UserMid response location */
    Dcm_DiagBufferIndexType resUserMemIdx = pMsgContext->resIndex;
    /* Set the UserMid in response now */
    Dcm_UtiProvideResData(pMsgContext, 1u);                                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */

    lStdResult = Dcm_Svc19UtiReportDtcFFRByDtc(pContext
#  if (DCM_DEMIF_431_ENABLED == STD_ON)
                                              ,lUserMemId + 0x0100u
#  else
                                              ,DEM_DTC_ORIGIN_SECONDARY_MEMORY
#  endif
                                              ,pMsgContext
                                              ,pRepContext);                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
    /* place the UserMid to the response */
    Dcm_UtiSetResDataAsU8At(pMsgContext, resUserMemIdx, lUserMemId);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_19_19_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_19Processor()
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_19Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  uint8 lUserMemId;
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext;

  pRepContext = &pContext->Repeater.Context.Svc19;
  lUserMemId = Dcm_UtiGetReqDataAsU8Rel(pMsgContext, 4u);

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_SVC_19_USER_MEM_ID_CHECK_ENABLED == STD_ON)
  if (lUserMemId != DCM_SVC_19_USER_MEM_ID)
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
#   if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
  if(Dcm_AuthMgrCheckMem(pContext, DCM_SVC_19_USERMEM_ID_PRECON_REF, lUserMemId) == DCM_E_NOT_OK)                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
    *ErrorCode = DCM_E_AUTHENTICATIONREQUIRED;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
#   endif
#  endif
  {
    /* save UserMid response location */
    Dcm_DiagBufferIndexType resUserMemIdx = pMsgContext->resIndex;
    /* skip the UserMid in response now */
    Dcm_UtiProvideResData(pMsgContext, 1u);                                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */

    lStdResult = Dcm_Svc19UtiReportExtDataRecordByDtcNum(pContext
#  if (DCM_DEMIF_431_ENABLED == STD_ON)
                                                        ,lUserMemId + 0x0100u
#  else
                                                        ,DEM_DTC_ORIGIN_SECONDARY_MEMORY
#  endif
                                                        ,pMsgContext
                                                        ,ErrorCode
                                                        ,pRepContext);                                                                               /* SBSW_DCM_COMB_PTR_FORWARD */
    /* place the UserMid to the response */
    Dcm_UtiSetResDataAsU8At(pMsgContext, resUserMemIdx, lUserMemId);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_19_1A_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_1AProcessor()
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_1AProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
)
{
  Std_ReturnType lStdResult;
  uint8          lExtDataRecNumber;

  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lExtDataRecNumber);                                                                                        /* SBSW_DCM_COMB_PTR_FORWARD */

  if ((lExtDataRecNumber == DCM_SVC_19_UTI_RECNUM_ALL)
    || (lExtDataRecNumber == DCM_SVC_19_UTI_RECNUM_OBD_ALL))
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
  {
    uint8 dtcSAM;
    /* Gather the configured DEM Status Availability Mask */
    lStdResult = Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask(&dtcSAM, ErrorCode, pRepContext);                                                       /* SBSW_DCM_COMB_PTR_FORWARD */

    if (lStdResult == E_OK)
    {
      Dcm_UtiProvideResDataAsU8(pMsgContext, dtcSAM);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideResDataAsU8(pMsgContext, lExtDataRecNumber);                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */

      /* Set DEM reporting filter */
      lStdResult = Dcm_Svc19UtiDemSetDTCFilterByExtendedDataRecordNumber(lExtDataRecNumber, ErrorCode, pRepContext);                                 /* SBSW_DCM_COMB_PTR_FORWARD */

      if (lStdResult == DCM_E_OK)
      {
        /* Delegate the service processing to the "DTCAndStatusRecord" reporting utility (i.e. writes DTC and its status into the response) */
        lStdResult = Dcm_Svc19UtiChainStart(pContext, Dcm_Svc19UtiDemChainReadDtcByStatus, pRepContext);                                             /* SBSW_DCM_COMB_PTR_FORWARD */
      }/* Otherwise, Already set by Dcm_Svc19UtiDemSetDTCFilterByExtendedDataRecordNumber */
    } /* Otherwise, Already set by Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask */
  }
  return lStdResult;
}
# endif /* DCM_SVC_19_1A_SUPPORT_ENABLED == STD_ON */

# if (DCM_SVC_19_42_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_42Processor()
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
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_42Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                lStdResult;
  uint8                         dtcStatusMask;
  uint8                         dtcSeverityMask;
  uint8                         functionalId;
  uint8                         dtcSAM;
  uint8                         dtcSevAM;
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &functionalId);                                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */

  if (functionalId == 0x33u)
  {
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &dtcStatusMask);                                                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &dtcSeverityMask);                                                                                        /* SBSW_DCM_COMB_PTR_FORWARD */

    /* Echo the requested functional group (do it here, otherwise the above extracted data could be overwritten in case of shared Rx-Tx-Buffers) */
    Dcm_UtiProvideResDataAsU8(pMsgContext, functionalId);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* Gather the configured DEM Status Availability Mask */
    lStdResult = Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask(&dtcSAM, ErrorCode, pRepContext);                                                       /* SBSW_DCM_COMB_PTR_FORWARD */

    if(lStdResult == E_OK)
    {
      /* Report configured DEM SAM */
      Dcm_UtiProvideResDataAsU8(pMsgContext, dtcSAM);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

#  if (DCM_DEMIF_421_ENABLED == STD_ON)
      lStdResult = Dcm_DemIf(GetDTCSeverityAvailabilityMask)(&dtcSevAM);                                                                             /* SBSW_DCM_POINTER_FORWARD_STACK */
#  else
      lStdResult = Dcm_DemIf(GetDTCSeverityAvailabilityMask)(pRepContext->DemClientId
                                                             ,&dtcSevAM);                                                                            /* SBSW_DCM_POINTER_FORWARD_STACK */
#  endif

      if(lStdResult == E_OK)
      {
        Dcm_UtiProvideResDataAsU8(pMsgContext, dtcSevAM);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
        /* Report the configured in DEM DTC format identifier */
#  if (DCM_DEMIF_421_ENABLED == STD_ON)
        Dcm_UtiProvideResDataAsU8(pMsgContext, Dcm_DemIf(GetTranslationType)());                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
#  else
        Dcm_UtiProvideResDataAsU8(pMsgContext, Dcm_DemIf(GetTranslationType)(pRepContext->DemClientId));                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
#  endif

        Dcm_Svc19UtiPatchDtcStatusSetForFilter(&dtcStatusMask);                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */

        /* Form the relevant status bits */
        dtcStatusMask &= dtcSAM;

        if(dtcStatusMask == 0u)
        {
          lStdResult = DCM_E_OK;/* no DTCs to report */
        }
        else
        {
          /* filter the DTCs */
          lStdResult = Dcm_Svc19UtiDemSetDTCFilter(ErrorCode
                                                  ,DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_07
                                                  ,dtcStatusMask
                                                  ,dtcSeverityMask
                                                  ,DCM_DEMIF_FILTER_FOR_FDC_NO
                                                  ,pRepContext);                                                                                     /* SBSW_DCM_COMB_PTR_FORWARD */

          if(lStdResult == DCM_E_OK)
          {
            lStdResult = Dcm_Svc19UtiChainStart(pContext, Dcm_Svc19Uti19_42DemChain, pRepContext);                                                   /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
          }/* Otherwise, Already set by Dcm_Svc19UtiDemSetDTCFilter */
        }
      }
      else
      {
        *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(lStdResult, Dcm_Svc19UtiNrcMapStdReturnType);                                                     /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }
    }/* Otherwise, Already set by Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask */
  }
  else
  {
    /* Invalid functional group */
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  return lStdResult;
}                                                                                                                                                    /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
# endif

# if (DCM_SVC_19_55_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_55Processor()
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_55Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  uint8          lFunctionalId;

  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lFunctionalId);                                                                                            /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lFunctionalId == 0x33u)
  {
    uint8 dtcSAM;

    /* Echo the requested functional group (do it here, otherwise the above extracted data could be overwritten in case of shared Rx-Tx-Buffers) */
    Dcm_UtiProvideResDataAsU8(pMsgContext, lFunctionalId);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* Gather the configured DEM Status Availability Mask */
    lStdResult = Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask(&dtcSAM, ErrorCode, pRepContext);                                                       /* SBSW_DCM_COMB_PTR_FORWARD */

    if(lStdResult == E_OK)
    {
      uint8 lDTCFormatId;

      /* Report configured DEM SAM */
      Dcm_UtiProvideResDataAsU8(pMsgContext, dtcSAM);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

      /* Report the configured in DEM DTC format identifier */
      lDTCFormatId = Dcm_DemIf(GetTranslationType)(pRepContext->DemClientId);

      if((lDTCFormatId == 0x02u) || (lDTCFormatId == 0x04u))
      {
        Dcm_UtiProvideResDataAsU8(pMsgContext, lDTCFormatId);                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */

        /* Set DEM reporting filter */
        lStdResult = Dcm_Svc19UtiDemSetDTCFilter(ErrorCode
                                                ,DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_05
                                                ,0u /* DTC Status Mask must be equal to 0x00 according to AUTOSAR 4.3 */
                                                ,0u /* Severity mask is not relevant */
                                                ,DCM_DEMIF_FILTER_FOR_FDC_NO
                                                ,pRepContext);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */

        if(lStdResult == DCM_E_OK)
        {
          /* Delegate the service processing to the "DTCAndStatusRecord" reporting utility (i.e. writes DTC and its status into the response) */
          lStdResult = Dcm_Svc19UtiChainStart(pContext, Dcm_Svc19UtiDemChainReadDtcByStatus, pRepContext);                                           /* SBSW_DCM_COMB_PTR_FORWARD */
        }/* Otherwise, Already set by Dcm_Svc19UtiDemSetDTCFilter */
      }
      else
      {
        /* Invalid DTC format identifier */
        *ErrorCode = DCM_E_GENERALREJECT;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }
    }/* Otherwise, Already set by Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask */
  }
  else
  {
    /* Invalid functional group */
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_19_56_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service19_56Processor()
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service19_56Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  uint8          lFunctionalId;
  uint8          lReadinessId;

  Dcm_Svc19RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc19;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lFunctionalId);                                                                                            /* SBSW_DCM_COMB_PTR_FORWARD */
  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lReadinessId);                                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lFunctionalId == 0x33u)
  {
    uint8 dtcSAM;

    /* Echo the requested functional group (do it here, otherwise the above extracted data could be overwritten in case of shared Rx-Tx-Buffers) */
    Dcm_UtiProvideResDataAsU8(pMsgContext, lFunctionalId);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* Gather the configured DEM Status Availability Mask */
    lStdResult = Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask(&dtcSAM, ErrorCode, pRepContext);                                                       /* SBSW_DCM_COMB_PTR_FORWARD */

    if (lStdResult == E_OK)
    {
      uint8 lDTCFormatId;

      /* Report configured DEM SAM */
      Dcm_UtiProvideResDataAsU8(pMsgContext, dtcSAM);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

      /* Report the configured in DEM DTC format identifier */
      lDTCFormatId = Dcm_DemIf(GetTranslationType)(pRepContext->DemClientId);

      Dcm_UtiProvideResDataAsU8(pMsgContext, lDTCFormatId);                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideResDataAsU8(pMsgContext, lReadinessId);                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */

      /* Set DEM reporting filter */
      lStdResult = Dcm_Svc19UtiDemSetReadinessGroupIdentifierFilter(lReadinessId, ErrorCode, pRepContext);                                           /* SBSW_DCM_COMB_PTR_FORWARD */

      if (lStdResult == DCM_E_OK)
      {
        /* Delegate the service processing to the "DTCAndStatusRecord" reporting utility (i.e. writes DTC and its status into the response) */
        lStdResult = Dcm_Svc19UtiChainStart(pContext, Dcm_Svc19UtiDemChainReadDtcByStatus, pRepContext);                                             /* SBSW_DCM_COMB_PTR_FORWARD */
      }/* Otherwise, Already set by Dcm_Svc19UtiDemSetReadinessGroupIdentifierFilter */
    }/* Otherwise, Already set by Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask */
  }
  else
  {
    /* Invalid functional group */
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  return lStdResult;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Service19Cancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service19Cancel(
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  Dcm_DemIfCancelOperation();
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_19_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc19.c
 *********************************************************************************************************************/
