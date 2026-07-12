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
/**        \file  Dcm_Svc0A.c
 *         \unit  Svc0A
 *        \brief  Contains the implementation of Service 0x0A unit.
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
#define DCM_SVC0A_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc0A.h"
#if (DCM_SVC_0A_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Repeater.h"
# include "Dcm_Debug.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Service 0x0A processing steps */
# define DCM_SVC0A_PROGRESS_SETDTCFILTER                             ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
# define DCM_SVC0A_PROGRESS_READDTCBYSTATUSMASK                      ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u))

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
 *  Dcm_Svc0AHandler()
 *********************************************************************************************************************/
/*! \brief          Service 0x0A Handler.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc0AHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_ObdDtcManagerContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc0AUtiDemSetDTCFilter()
 *********************************************************************************************************************/
/*! \brief          Wrapper function for Dem_SetDTCFilter API.
 *  \details        Sets the DTC filter using the respective DEM API.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current request context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc0AUtiDemSetDTCFilter(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_ObdDtcManagerContextPtrType pRepContext
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
 *  Dcm_Svc0AHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc0AHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_ObdDtcManagerContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if (pMsgContext->reqDataLen == 0u)
  {
    pRepContext->DemClientId = Dcm_DiagGetDemClientId(pMsgContext);                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */ /* SBSW_DCM_PARAM_PTR_FORWARD */

    Dcm_RepeaterNextStep(pContext, DCM_SVC0A_PROGRESS_SETDTCFILTER);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
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
 *  Dcm_Svc0AUtiDemSetDTCFilter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc0AUtiDemSetDTCFilter(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_ObdDtcManagerContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_ObdDtcMgrDemSetDTCFilter(DCM_DTC_BIT_DONT_CARE
                                           ,DEM_DTC_ORIGIN_PERMANENT_MEMORY
                                           ,pMsgContext
                                           ,ErrorCode
                                           ,pRepContext);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */

  Dcm_RepeaterNextStep(pContext, DCM_SVC0A_PROGRESS_READDTCBYSTATUSMASK);                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */

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
 *  Dcm_Service0AProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service0AProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType  lStdResult;

  Dcm_ObdDtcManagerContextPtrType pRepContext = &pContext->Repeater.Context.Svc0A.ReadDtcContext;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc0AHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC0A_PROGRESS_SETDTCFILTER:
    lStdResult = Dcm_Svc0AUtiDemSetDTCFilter(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                               /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC0A_PROGRESS_READDTCBYSTATUSMASK:
    lStdResult = Dcm_ObdDtcMgrReadDtcByStatusMask(opStatus, pMsgContext, ErrorCode, pRepContext);                                                    /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}

# if (DCM_DEMIF_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service0AUpdater()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service0AUpdater(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_ObdDtcMgrCpyDemGetNxtFltrdDtc(pDataContext, ErrorCode, &pContext->Repeater.Context.Svc0A.ReadDtcContext);                               /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_0A_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc0A.c
 *********************************************************************************************************************/
