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
/**        \file  Dcm_Svc06.c
 *         \unit  Svc06
 *        \brief  Contains the implementation of Service 0x06 unit.
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
#define DCM_SVC06_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc06.h"
#if (DCM_SVC_06_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Repeater.h"
# include "Dcm_Uti.h"
# include "Dcm_Debug.h"
# if (DCM_OBDIDMGR_OBDMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
#  include "Dcm_DemIf.h"
# endif
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Verify consistency between DCM and DEM configurations.
* In case of error, please refer to the DCM technical reference (search for the error text) for more information and solution.
*/
# if defined(DEM_CFG_SUPPORT_DTR)                                                                                                                    /* COV_DCM_UNSUPPORTED_ERROR_DETECTION XF */
#  if (DCM_OBDIDMGR_OBDMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
#   if (DEM_CFG_SUPPORT_DTR == STD_OFF)
#    error "DCM configured to handle OBD MIDs via DEM configuration, but no MID handling is done by DEM."
#   endif
#  endif

#  if (DCM_OBDIDMGR_OBDMID_SUPPORT_BY_DCM_ENABLED == STD_ON)
#   if (DEM_CFG_SUPPORT_DTR == STD_ON)
#    error "DCM configured to handle OBD MIDs via DCM configuration, but MID handling is done by DEM."
#   endif
#  endif
# endif

/*! Service 0x06 processing step */
# define DCM_SVC06_PROGRESS_REPEATERPROXY                            ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))

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
 *  Dcm_Svc06Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x06 Handler.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc06Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc06RepeaterProxyContextPtrType pRepContext
  );

# if (DCM_OBDIDMGR_OBDMID_SUPPORT_BY_DCM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc06_XX_RepeaterProxy()
 *********************************************************************************************************************/
/*! \brief          Service 0x06 worker.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc06_XX_RepeaterProxy(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc06RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_OBDIDMGR_OBDMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc06_XX_RepeaterProxy()
 *********************************************************************************************************************/
/*! \brief          Service 0x06 worker.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc06_XX_RepeaterProxy(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc06RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_OBDIDMGR_OBDMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc06ProcessSupportedMidRequest()
 *********************************************************************************************************************/
/*! \brief          Check and process if every MID in request is an availability MID (0x00, 0x20, 0x40...).
 *  \details        -
 *  \param[in,out]  pMsgContext             The current request context
 *  \param[out]     ErrorCode               Negative response code
 *  \return         DCM_E_OK                Send positive response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc06ProcessSupportedMidRequest(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_OBDIDMGR_OBDMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc06ProcessSupportedMid()
 *********************************************************************************************************************/
/*! \brief          Reading supported MIDs.
 *  \details        -
 *  \param[in,out]  pMsgContext             The current request context
 *  \param[out]     ErrorCode               Negative response code
 *  \return         DCM_E_OK                Send positive response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc06ProcessSupportedMid(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
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
# if (DCM_OBDIDMGR_OBDMID_SUPPORT_BY_DCM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc06Handler()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc06Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc06RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  pRepContext->ParserDesc.LookUpTable = Dcm_CfgSvc06LookUpTable;                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
  pRepContext->ParserDesc.LookUpFilterTable = Dcm_CfgSvc06ExecPrecondTable;                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
  pRepContext->ParserDesc.MaskData = Dcm_CfgSvc06SupportedIdMask;                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */

  if (Dcm_ObdIdMgrIsAvailabilityId(pMsgContext->reqData[0]))
  {
    lStdResult = Dcm_ObdIdMgrProcessSupportedIdRequest(pMsgContext, &(pRepContext->ParserDesc), ErrorCode);                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    if (pMsgContext->reqDataLen == 1u)
    {
      if (Dcm_ObdIdMgrIdLookUp(pMsgContext->reqData[0], &(pRepContext->ParserDesc)) == DCM_E_OK)                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
      {
        Dcm_RepeaterNextStep(pContext, DCM_SVC06_PROGRESS_REPEATERPROXY);                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
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
# endif

# if (DCM_OBDIDMGR_OBDMID_SUPPORT_BY_DCM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc06_XX_RepeaterProxy()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc06_XX_RepeaterProxy(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc06RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                 lStdResult = DCM_E_OK;
  uint16                         lTestValue;
  uint16                         lMinLimit;
  uint16                         lMaxLimit;
  Dcm_DTRStatusType              lStatus;
  Dcm_CfgSvc06Mid2TidRefOptType  lHandle = Dcm_CfgWrapSvc06Mid2TidRefMap(pRepContext->ParserDesc.Handle);
  Dcm_CfgSvc06Mid2TidRefOptType  lMaxHandle = Dcm_CfgWrapSvc06Mid2TidRefMap(pRepContext->ParserDesc.Handle + 1u);
  Dcm_CfgSvc06MidTidInfoPtrType  pMidTidInfo;

  for (; lHandle<lMaxHandle; lHandle++)
  {
    lStdResult = Dcm_CfgWrapSvc06MidTidInfoGetEntry(lHandle, &pMidTidInfo, ErrorCode);                                                               /* SBSW_DCM_COMB_PTR_FORWARD */

    if (lStdResult == DCM_E_OK)                                                                                                                      /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
    {
      if (pMidTidInfo->OpFunc(opStatus, &lTestValue, &lMinLimit, &lMaxLimit, &lStatus) == DCM_E_OK)                                                  /* SBSW_DCM_POINTER_FORWARD_STACK */ /* SBSW_DCM_CALL_FUNCPTR_SVC06MIDTIDINFO */
      {
        if (lStatus == DCM_DTRSTATUS_INVISIBLE)
        {
          lTestValue = 0;
          lMinLimit = 0;
          lMaxLimit = 0;
        }

        /* Copy data */
        Dcm_UtiProvideResDataAsU8(pMsgContext, pMsgContext->reqData[0]);                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
        Dcm_UtiProvideResDataAsU8(pMsgContext, pMidTidInfo->Tid);                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
        Dcm_UtiProvideResDataAsU8(pMsgContext, pMidTidInfo->Uasid);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
        Dcm_UtiProvideResDataAsU16(pMsgContext, lTestValue);                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
        Dcm_UtiProvideResDataAsU16(pMsgContext, lMinLimit);                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
        Dcm_UtiProvideResDataAsU16(pMsgContext, lMaxLimit);                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
      else
      {
        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }
    }

    if (lStdResult != DCM_E_OK)
    {
      break;
    }
  }
  return lStdResult;
}
# endif

# if (DCM_OBDIDMGR_OBDMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc06Handler()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc06Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc06RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;
  uint8 lObdMid = pMsgContext->reqData[0];

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if (Dcm_ObdIdMgrIsAvailabilityId(lObdMid))
  {
    lStdResult = Dcm_Svc06ProcessSupportedMidRequest(pMsgContext, ErrorCode);                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else if (pMsgContext->reqDataLen == 1u)
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC06_PROGRESS_REPEATERPROXY);                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;
  }
  else
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}
# endif

# if (DCM_OBDIDMGR_OBDMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc06_XX_RepeaterProxy()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc06_XX_RepeaterProxy(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc06RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;
  uint8 lObdMid = pMsgContext->reqData[0];
  uint8 lNumOfTIDs;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Request number of TIDs for MID from Dem */
  if (Dem_DcmGetNumTIDsOfOBDMID(lObdMid, &lNumOfTIDs) == E_OK)                                                                                       /* SBSW_DCM_POINTER_FORWARD_STACK */
  {
    /* #10 If request is successful */
    uint8 lTidIter;
    uint8 lTIDValue;
    uint8 lUaSID;
    uint16 lTestValue;
    uint16 lTestValueMin;
    uint16 lTestValueMax;

    /* Request Test Result of each TID from Dem */
    for (lTidIter = 0; lTidIter < lNumOfTIDs; lTidIter++)
    {
      if (Dem_DcmGetDTRData(lObdMid
                           ,lTidIter
                           ,&lTIDValue
                           ,&lUaSID
                           ,&lTestValue
                           ,&lTestValueMin
                           ,&lTestValueMax) == E_OK)                                                                                                 /* SBSW_DCM_POINTER_FORWARD_STACK */
      {

        /* #20 If request is successful for a TID */

        /* #30 If response data buffer has not enough space */
        if (pMsgContext->resMaxDataLen < 9u)
        {
          /* Set NRC DCM_E_REQUESTOUTOFRANGE */
          *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
        }
        /* Otherwise copy response data to response buffer */
        else
        {
          Dcm_UtiProvideResDataAsU8(pMsgContext, lObdMid);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */

          Dcm_UtiProvideResDataAsU8(pMsgContext, lTIDValue);                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

          Dcm_UtiProvideResDataAsU8(pMsgContext, lUaSID);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */

          Dcm_UtiProvideResDataAsU16(pMsgContext, lTestValue);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
          Dcm_UtiProvideResDataAsU16(pMsgContext, lTestValueMin);                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
          Dcm_UtiProvideResDataAsU16(pMsgContext, lTestValueMax);                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
        }
      }
      else
      {
        /* Otherwise set NRC DCM_E_CONDITIONSNOTCORRECT */
        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }

      /* #40 If any unexpected error occurred */
      if (lStdResult != DCM_E_OK)
      {
        /* Exit service processing */
        break;
      }
    }
  }
  else
  {
    /* Otherwise set NRC DCM_E_REQUESTOUTOFRANGE */
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}
# endif

# if (DCM_OBDIDMGR_OBDMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc06ProcessSupportedMidRequest()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc06ProcessSupportedMidRequest(
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;
  uint8_least lMidIter;
  uint8_least lNumberOfMids = (uint8)pMsgContext->reqDataLen;

  if (pMsgContext->reqDataLen <= DCM_OBDIDMGR_MAX_NUMBER_OF_IDS) /* Up to 6 IDs possible */
  {
    /* Check if any MID in request is not an availability MID (0x00, 0x20, 0x40...) */
    for (lMidIter = 0; lMidIter < lNumberOfMids; lMidIter++)
    {
      if (!Dcm_ObdIdMgrIsAvailabilityId(pMsgContext->reqData[lMidIter]))
      {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
        break;
      }
    }

    if (lStdResult == DCM_E_OK)
    {
      lStdResult = Dcm_Svc06ProcessSupportedMid(pMsgContext, ErrorCode);                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
  }
  else
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}
# endif

# if (DCM_OBDIDMGR_OBDMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc06ProcessSupportedMid()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc06ProcessSupportedMid(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;
  uint8_least lMidIter;
  uint8_least lNumberOfMids = (uint8)pMsgContext->reqDataLen;

  uint8 lMidList[DCM_OBDIDMGR_MAX_NUMBER_OF_IDS];
  /* Copy all requested IDs to local list. This allows to write into response buffer during parsing. */
  Dcm_UtiConsumeReqDataAsUN(pMsgContext, lMidList, DCM_OBDIDMGR_MAX_NUMBER_OF_IDS, pMsgContext->reqDataLen);                                         /* SBSW_DCM_COMB_PTR_FORWARD */

  /* Request from Dem value of each MID */
  for (lMidIter = 0; lMidIter < lNumberOfMids; lMidIter++)
  {
    uint32 lObdMidAvailMask;
    if (Dem_DcmGetAvailableOBDMIDs((lMidList[lMidIter]), &lObdMidAvailMask) == E_OK)                                                                 /* SBSW_DCM_POINTER_FORWARD_STACK */
    {
      /* If response data buffer has not enough space */
      if (pMsgContext->resMaxDataLen < 5u)                                                                                                           /* COV_DCM_RTM_UNIT_TEST XF */
      {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
        break;
      }
      /* Otherwise copy MID and MID value to response buffer */
      else
      {
        Dcm_UtiProvideResDataAsU8(pMsgContext, (lMidList[lMidIter]));                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
        Dcm_UtiProvideResDataAsU32(pMsgContext, lObdMidAvailMask);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
    }
    /* else NRC will be set later if response length equal 0 -> none of the OBD MIDs could be read */
  }

  /* If response data buffer is empty  */
  if (pMsgContext->resDataLen == 0u)
  {
    /* return NRC DCM_E_REQUESTOUTOFRANGE */
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
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
*  Dcm_Service06Processor()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service06Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc06RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc06;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc06Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC06_PROGRESS_REPEATERPROXY:
    lStdResult = Dcm_Svc06_XX_RepeaterProxy(opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                                                                                     /* COV_DCM_RTM_DEV_DEBUG XF */
    break;
  }

  return lStdResult;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_06_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc06.c
 *********************************************************************************************************************/
