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
/**        \file  Dcm_ObdDtcMgr.c
 *         \unit  ObdDtcMgr
 *        \brief  Contains the implementation of OBD DTC Manager unit.
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
#define DCM_OBDDTCMGR_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_ObdDtcMgr.h"
#if (DCM_OBDDTCMGR_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Uti.h"
# include "Dcm_Debug.h"
# include "Dcm_DemIf.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! OBD DTC manager processing steps */
# define DCM_OBDDTCMGR_PROGRESS_INITAL                               ((Dcm_RepeaterProgressType)0)
# define DCM_OBDDTCMGR_PROGRESS_GETNUMFLTRDDTC                       ((Dcm_RepeaterProgressType)1)
# define DCM_OBDDTCMGR_PROGRESS_COPYLINEARDATA                       ((Dcm_RepeaterProgressType)2)

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
 *  Dcm_ObdDtcMgrSetNextStep()
 *********************************************************************************************************************/
/*! \brief          Set next progress step.
 *  \details        -
 *  \param[in]      NextStep         Next step to be processed by local Repeater
 *  \param[in,out]  pRepContext      Current repeater proxy context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_ObdDtcMgrSetNextStep(
  Dcm_RepeaterProgressType NextStep,
  Dcm_ObdDtcManagerContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_ObdDtcMgrGetNextStep()
 *********************************************************************************************************************/
/*! \brief          Get next progress step.
 *  \details        -
 *  \return         Next step to be processed by local Repeater
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_RepeaterProgressType, DCM_CODE) Dcm_ObdDtcMgrGetNextStep(
  Dcm_ObdDtcManagerContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_ObdDtcMgrDemGetNumFltrdDtc()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of readDtcInfo.
 *  \details        Calls the GetNumberOfFilteredDTC function.
 *  \param[in]      opStatus                The operation status
 *  \param[in,out]  pMsgContext             Current request context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_STOP_REPEATER     Stop Repeater Proxy
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdDtcMgrDemGetNumFltrdDtc(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_ObdDtcManagerContextPtrType pRepContext
  );

# if (DCM_DEMIF_PAGED_BUFFER_ENABLED == STD_OFF)
/**********************************************************************************************************************
 *  Dcm_ObdDtcMgrCopyLinearData()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of readDtcInfo.
 *  \details        Copies some linear data.
 *  \param[in]      opStatus                The operation status
 *  \param[in,out]  pMsgContext             Current request context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdDtcMgrCopyLinearData(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_ObdDtcManagerContextPtrType pRepContext
  );
# endif
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
 *  Dcm_ObdDtcMgrSetNextStep()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_ObdDtcMgrSetNextStep(
  Dcm_RepeaterProgressType NextStep,
  Dcm_ObdDtcManagerContextPtrType pRepContext
  )
{
  pRepContext->DcmObdDtcManagerProgress = NextStep;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
}

/**********************************************************************************************************************
 *  Dcm_ObdDtcMgrGetNextStep()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_RepeaterProgressType, DCM_CODE) Dcm_ObdDtcMgrGetNextStep(
  Dcm_ObdDtcManagerContextPtrType pRepContext                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  return pRepContext->DcmObdDtcManagerProgress;
}

/**********************************************************************************************************************
 *  Dcm_ObdDtcMgrDemGetNumFltrdDtc()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdDtcMgrDemGetNumFltrdDtc(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_ObdDtcManagerContextPtrType pRepContext                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                            lStdResult;
  Dcm_DemIfReturnGetNumberOfFilteredDTCType lDemResult;
  uint16                                    lNumDtc;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

# if (DCM_DEMIF_421_ENABLED == STD_ON)
  lDemResult = Dcm_DemIf(GetNumberOfFilteredDTC)(&lNumDtc);                                                                                          /* SBSW_DCM_POINTER_FORWARD_STACK */
# else
  lDemResult = Dcm_DemIf(GetNumberOfFilteredDTC)(pRepContext->DemClientId
                                                ,&lNumDtc);                                                                                          /* SBSW_DCM_POINTER_FORWARD_STACK */
# endif

  switch (lDemResult)
  {
  case DCM_DEMIF_RET_NUMBER_OK:
    if (lNumDtc <= 0xFFu)
    {
      Dcm_UtiProvideResDataAsU8(pMsgContext, Dcm_UtiGetLoByte(lNumDtc));                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */ /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

# if (DCM_DEMIF_PAGED_BUFFER_ENABLED == STD_ON)
      Dcm_PagedBufferStart(pMsgContext, (Dcm_MsgLenType)lNumDtc * 2u);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_STOP_REPEATER;
# else
      Dcm_ObdDtcMgrSetNextStep(DCM_OBDDTCMGR_PROGRESS_COPYLINEARDATA, pRepContext);                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_LOOP;
# endif
    }
    else
    {
      /* else - NRC 0x22 */
      *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
    break;
  case DCM_DEMIF_RET_NUMBER_PENDING:
    lStdResult = DCM_E_PENDING;
    break;
  default:/* DEM_NUMBER_FAILED or unknown */
    *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(lDemResult, Dcm_DemIfNrcMapGetNumberOfFilteredDTC);                                                   /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }
  return lStdResult;
}

# if (DCM_DEMIF_PAGED_BUFFER_ENABLED == STD_OFF)
/**********************************************************************************************************************
 *  Dcm_ObdDtcMgrCopyLinearData()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdDtcMgrCopyLinearData(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_ObdDtcManagerContextPtrType pRepContext
  )
{
  Std_ReturnType           lStdResult;
  Dcm_DiagDataContextType  lDataContext;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  Dcm_UtiInitDataContext(&lDataContext, Dcm_UtiGetResData(pMsgContext), pMsgContext->resMaxDataLen);                                                 /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_POINTER_INIT_DATA_CONTEXT */

  lStdResult = Dcm_ObdDtcMgrCpyDemGetNxtFltrdDtc(&lDataContext, ErrorCode, pRepContext);                                                             /* SBSW_DCM_COMB_PTR_FORWARD */

  /* Commit copied data */
  Dcm_UtiProvideResData(pMsgContext, lDataContext.Usage);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */

  switch (lStdResult)
  {
  case DCM_E_BUFFERTOOLOW:
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;/* for OBD send (suppress with) NRC 0x31 since 0x14 (ResponseTooLong is not defined!) */                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  case DCM_E_DATA_READY_PADDING:
    lStdResult = DCM_E_OK;
    break;
  default:
    /* ErrorCode already set */
    break;
  }

  return lStdResult;
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_ObdDtcMgrDemSetDTCFilter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdDtcMgrDemSetDTCFilter(
  uint8 StatusMask,
  Dem_DTCOriginType Origin,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_ObdDtcManagerContextPtrType pRepContext
  )
{
  Std_ReturnType lStdReturn;
  Dcm_DemIfReturnSetFilterType resultSetDtcFilter;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  resultSetDtcFilter = Dcm_DemIf(SetDTCFilter)(
# if (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
                                               pRepContext->DemClientId,
# endif
                                               StatusMask
# if (DCM_DEMIF_421_ENABLED == STD_ON)
                                              ,DEM_DTC_KIND_EMISSION_REL_DTCS
# endif
                                              ,DEM_DTC_FORMAT_OBD
                                              ,Origin
                                              ,DCM_DEMIF_FILTER_WITH_SEVERITY_NO
                                              ,DEM_SEVERITY_NO_SEVERITY
                                              ,DCM_DEMIF_FILTER_FOR_FDC_NO);

  if (resultSetDtcFilter == DCM_DEMIF_RET_FILTER_ACCEPTED)
  {
    Dcm_ObdDtcMgrSetNextStep(DCM_OBDDTCMGR_PROGRESS_GETNUMFLTRDDTC, pRepContext);                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdReturn = DCM_E_LOOP;
  }
  else
  {/* else - set NRC */
    *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(resultSetDtcFilter, Dcm_DemIfNrcMapSetDTCFilter);                                                     /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }

  return lStdReturn;
}

/**********************************************************************************************************************
 *  Dcm_ObdDtcMgrReadDtcByStatusMask()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE)Dcm_ObdDtcMgrReadDtcByStatusMask(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_ObdDtcManagerContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;

  switch (Dcm_ObdDtcMgrGetNextStep(pRepContext))                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
  case DCM_OBDDTCMGR_PROGRESS_GETNUMFLTRDDTC:
    lStdResult = Dcm_ObdDtcMgrDemGetNumFltrdDtc(opStatus, pMsgContext, ErrorCode, pRepContext);                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
# if (DCM_DEMIF_PAGED_BUFFER_ENABLED == STD_OFF)
  case DCM_OBDDTCMGR_PROGRESS_COPYLINEARDATA:
    lStdResult = Dcm_ObdDtcMgrCopyLinearData(opStatus, pMsgContext, ErrorCode, pRepContext);                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
# endif
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}
/**********************************************************************************************************************
 *  Dcm_ObdDtcMgrCpyDemGetNxtFltrdDtc()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdDtcMgrCpyDemGetNxtFltrdDtc(
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_ObdDtcManagerContextPtrType pRepContext                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  uint32                                  lDtc;
  uint8                                   dtcStatus;
  Dcm_DemIfReturnGetNextFilteredItemType  demResult;
  Std_ReturnType                          lStdResult;

  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if (pDataContext->AvailLen >= 2u)
  {
    demResult = Dcm_DemIf(GetNextFilteredDTC)(
# if (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
                                              pRepContext->DemClientId,
# endif
                                              &lDtc, &dtcStatus);                                                                                    /* SBSW_DCM_POINTER_FORWARD_STACK */

    if (demResult == DCM_DEMIF_RET_FILTERED_OK)
    {
      Dcm_UtiProvideDataAsU8(pDataContext, Dcm_UtiGetHiLoByte(lDtc));                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideDataAsU8(pDataContext, Dcm_UtiGetLoHiByte(lDtc));                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_LOOP;
    }
    else
    {
      lStdResult = Dcm_DemIfConvDemRsltGetNextFilteredItem(demResult, ErrorCode);                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
  }
  else
  {
    lStdResult = DCM_E_BUFFERTOOLOW;
  }

  return lStdResult;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_OBDDTCMGR_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_ObdDtcMgr.c
 *********************************************************************************************************************/
