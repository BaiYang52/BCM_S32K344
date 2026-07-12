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
/**        \file  Dcm_ObdIdMgr.c
 *         \unit  ObdIdMgr
 *        \brief  Contains the implementation of OBD ID Manager unit.
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
#define DCM_OBDIDMGR_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_ObdIdMgr.h"
#if (DCM_OBDIDMGR_SUPPORT_ENABLED == STD_ON) || (DCM_OBDUDSIDMGR_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Uti.h"
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
#  if (DEM_CFG_SUPPORT_DTR == STD_ON)
#   if (DCM_OBDUDSIDMGR_UDSMID_SUPPORT_BY_DCM_ENABLED == STD_ON)
#    error "DCM configured to handle OBD DID MIDs via DCM configuration, but MID handling is done by DEM."
#   endif
#  else
#   if (DCM_OBDUDSIDMGR_UDSMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
#    error "DCM configured to handle OBD DID MIDs via DEM configuration, but no MID handling is done by DEM."
#   endif
#  endif
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
# define Dcm_ObdIdMgrGetObdIdMaskBit(obdId)                          ((uint32)(0x01UL << (31u - (((obdId)-1u) & 0x1Fu))))                            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_ObdIdMgrIsIdSupported(availMask, obdId)                 (Dcm_UtiBitOpTest(uint32, (availMask), Dcm_ObdIdMgrGetObdIdMaskBit(obdId)))     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_ObdIdMgrGetAvailabilityIdIdx(obdId)                     ((uint8)(((uint8)(obdId)) >> 5u))                                               /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_ObdIdMgrGetAvailabilityId(id)                           ((uint8)((id) & 0xE0))                                                          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_ObdIdMgrGetMaskValue(obdAidId, pMaskData)               ((pMaskData)[Dcm_ObdIdMgrGetAvailabilityIdIdx(obdAidId)])                       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_OBDUDSIDMGR_UDSMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ObdIdMgr_F6XX_ReadDidData_SupportedId()
 *********************************************************************************************************************/
/*! \brief          Read operation for a Supported OBD MID Mask.
 *  \details        -
 *  \param[in]      ObdMID         Supported OBD Monitor Id
 *  \param[in,out]  pDataContext   The pointer to the data context
 *  \return         DCM_E_OK       Read operation was successful
 *  \return         DCM_E_NOT_OK   Read operation was not successful
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Obd MID is a supported MID (0x00, 0x20, 0x40,..)
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdIdMgr_F6XX_ReadDidData_SupportedId(
  uint8 ObdMID,
  Dcm_DiagDataContextPtrType pDataContext
  );
# endif

# if (DCM_OBDUDSIDMGR_UDSMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ObdIdMgr_F6XX_ReadDidData_DataId()
 *********************************************************************************************************************/
/*! \brief          Read operation for a OBD MID.
 *  \details        Reads test result for given OBD MID
 *  \param[in]      ObdMID         OBD Monitor Id
 *  \param[in,out]  pDataContext   The pointer to the data context
 *  \return         DCM_E_OK       Read operation was successful
 *  \return         DCM_E_NOT_OK   Read operation was not successful
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Obd MID is not a supported MID (0x00, 0x20, 0x40,..)
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdIdMgr_F6XX_ReadDidData_DataId(
  uint8 ObdMID,
  Dcm_DiagDataContextPtrType pDataContext
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
# if (DCM_OBDUDSIDMGR_UDSMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ObdIdMgr_F6XX_ReadDidData_SupportedId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdIdMgr_F6XX_ReadDidData_SupportedId(
  uint8 ObdMID,
  Dcm_DiagDataContextPtrType pDataContext
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;
  uint32 lObdMidAvailMask;

  /* If request of MID Mask value from Dem is successful */
  if (Dem_DcmGetAvailableOBDMIDs(ObdMID, &lObdMidAvailMask) == E_OK)                                                                                 /* SBSW_DCM_POINTER_FORWARD_STACK */
  {
    /* Copy MID Mask value to data context */
    Dcm_UtiProvideDataAsU32(pDataContext, lObdMidAvailMask);                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}
# endif

# if (DCM_OBDUDSIDMGR_UDSMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ObdIdMgr_F6XX_ReadDidData_DataId()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdIdMgr_F6XX_ReadDidData_DataId(
  uint8 ObdMID,
  Dcm_DiagDataContextPtrType pDataContext
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;
  uint8 lNumOfTIDs;

  /* Request number of TIDs for DID from Dem */
  if (Dem_DcmGetNumTIDsOfOBDMID(ObdMID, &lNumOfTIDs) == E_OK)                                                                                        /* SBSW_DCM_POINTER_FORWARD_STACK */
  {
    /* If request is successful */
    uint8 lTidIter;
    uint16 lTestValue;
    uint16 lTestValuMin;
    uint16 lTestValueMax;
    uint8 lTidValue;
    uint8 lUaSid;

    /* Request Test Result of each TID from Dem */
    for (lTidIter = 0; lTidIter < lNumOfTIDs; lTidIter++)
    {
      if (Dem_DcmGetDTRData(ObdMID, lTidIter, &lTidValue, &lUaSid, &lTestValue, &lTestValuMin, &lTestValueMax) == E_OK)                              /* SBSW_DCM_POINTER_FORWARD_STACK */
      {
        /* If request is successful for a TID */
        /* Copy response data to data context */
        Dcm_UtiProvideDataAsU8(pDataContext, lTidValue);                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */

        Dcm_UtiProvideDataAsU8(pDataContext, lUaSid);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

        Dcm_UtiProvideDataAsU16(pDataContext, lTestValue);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */

        Dcm_UtiProvideDataAsU16(pDataContext, lTestValuMin);                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

        Dcm_UtiProvideDataAsU16(pDataContext, lTestValueMax);                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
      else
      {
        lStdResult = DCM_E_NOT_OK;
        break;
      }
    }
  }
  else
  {
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
# if (DCM_OBDIDMGR_REQ_PARSER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ObdIdMgrProcessSupportedIdRequest()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdIdMgrProcessSupportedIdRequest(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_ObdIdMgrDescPtrType pParserDesc,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;
  uint8_least    lNumberOfIds = (uint8_least)pMsgContext->reqDataLen;

  /* If maximal 6 OBD availability IDs are requested: */
  if (lNumberOfIds <= DCM_OBDIDMGR_MAX_NUMBER_OF_IDS)
  {
    uint8        lIdList[DCM_OBDIDMGR_MAX_NUMBER_OF_IDS];
    uint8_least  lIdIter;

    /* Copy all requested IDs to local list. This allows to write into response buffer during parsing. */
    Dcm_UtiConsumeReqDataAsUN(pMsgContext, lIdList, DCM_OBDIDMGR_MAX_NUMBER_OF_IDS, pMsgContext->reqDataLen);                                        /* SBSW_DCM_COMB_PTR_FORWARD */

    /* If any ID in request is not an availability ID (0x00, 0x20, 0x40...): */
    for (lIdIter = 0; lIdIter < lNumberOfIds; lIdIter++)
    {
      if (!Dcm_ObdIdMgrIsAvailabilityId(lIdList[lIdIter]))
      {
        /* Mix of Data and Availability IDs is not allowed */
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
        break;
      }
    }

    if (lStdResult == DCM_E_OK)
    {
      for (lIdIter = 0; lIdIter < lNumberOfIds; lIdIter++)
      {
        uint8 lId = lIdList[lIdIter];

        /* If the given OBD identifier is supported: */
        if (Dcm_ObdIdMgrIdLookUp(lId, pParserDesc) == DCM_E_OK)                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
        {
          Dcm_UtiProvideResDataAsU8(pMsgContext, lId);                                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
          Dcm_UtiProvideResDataAsU32(pMsgContext, Dcm_ObdIdMgrGetAvailabilityMask(lId, pParserDesc));                                                /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_PARAM_PTR_FORWARD */
        }
      }

      /* If no requested ID was supported: */
      if (pMsgContext->resDataLen == 0u)
      {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }
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

# if (DCM_SVC_22_OBD_CALIBRATION_ENABLED == STD_ON) || \
     (DCM_SVC_31_OBD_CALIBRATION_ENABLED == STD_ON) || \
     (DCM_OBDUDSIDMGR_CALIBRATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ObdIdMgrIsIdEnabled()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_ObdIdMgrIsIdEnabled(
  uint8 obdId,
  P2CONST(Dcm_ObdIdMgrMaskValueTableType, AUTOMATIC, DCM_CONST) pMaskData
  )
{
  boolean lResult;

  if (obdId == 0x00u)
  {
    lResult = (boolean)(pMaskData[0] != 0u); /* ID 0x00 is only available if any other IDs are supported too */                                      /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
  }
  else
  {
    lResult = (boolean)Dcm_ObdIdMgrIsIdSupported(pMaskData[Dcm_ObdIdMgrGetAvailabilityIdIdx(obdId - 1u)], obdId); /* else - another ID (i.e. [0x01-0xFF]) is requested */ /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
  }

  return lResult;
}
# endif

/**********************************************************************************************************************
 *  Dcm_ObdIdMgrGetAvailabilityMask()
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
FUNC(uint32, DCM_CODE) Dcm_ObdIdMgrGetAvailabilityMask(
  uint8 id,
  Dcm_ObdIdMgrDescPtrType pParserDesc
  )
{
  uint32       lResultMask;
  sint16_least lLookUpIter;

  lResultMask = 0;
  lLookUpIter = (sint16_least)Dcm_UtiLookUpUint8(pParserDesc->LookUpTable, id);                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* Skip the availability Id */
  ++lLookUpIter;

  /* Iterate within the range of the given AID or end of table and calculate bit mask  */
  for (; lLookUpIter < (sint16_least)pParserDesc->LookUpTable[0]; ++lLookUpIter)
  {
    uint8 lId = pParserDesc->LookUpTable[lLookUpIter + 1]; /* Consider the leading-size-byte offset in the lookup table */

    if (Dcm_ObdIdMgrIdLookUp(lId, pParserDesc) == DCM_E_OK)                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
    {
      Dcm_UtiBitOpSet(uint32, lResultMask, Dcm_ObdIdMgrGetObdIdMaskBit(lId));
    }

    /* Iterate until the next AID is reached. This AID shall be reported in bit0 of the mask too, therefore use post-condition-check */
    if (Dcm_ObdIdMgrIsAvailabilityId(lId))
    {
      break;
    }
  }

  return lResultMask;
}

/**********************************************************************************************************************
 *  Dcm_ObdIdMgrIdLookUp()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdIdMgrIdLookUp(
  uint8 id,
  Dcm_ObdIdMgrDescPtrType pParserDesc
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  sint16_least   lLookUpResult;

  lLookUpResult = Dcm_DiagVariantLookUpFilter(pParserDesc->LookUpTable, pParserDesc->LookUpFilterTable, id);                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

  if (lLookUpResult >= 0)
  {
# if (DCM_OBDUDSIDMGR_CALIBRATION_ENABLED == STD_ON)
    if (Dcm_ObdIdMgrIsIdEnabled(id, pParserDesc->MaskData) == TRUE)                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
# endif
    {
      pParserDesc->Handle = (Dcm_ObdIdMgrHandleMemType)((uint16)lLookUpResult - Dcm_ObdIdMgrGetAvailabilityIdIdx(id) - 1u);                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_OK;
    }
  }

  return lStdResult;
}

# if (DCM_OBDUDSIDMGR_UDSMID_SUPPORT_BY_DCM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ObdIdMgrReadUdsMid()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdIdMgrReadUdsMid(
  Dcm_ObdIdMgrHandleMemType handle,
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext
  )
{
  Std_ReturnType                 lStdResult = DCM_E_OK; /* if no data to be returned -> positive response */
  uint16                         lTestValue;
  uint16                         lMinLimit;
  uint16                         lMaxLimit;
  Dcm_DTRStatusType              lStatus;
  Dcm_CfgSvc06Mid2TidRefOptType  lHandle = Dcm_CfgWrapSvc06Mid2TidRefMap(handle);
  Dcm_CfgSvc06Mid2TidRefOptType  lMaxHandle = Dcm_CfgWrapSvc06Mid2TidRefMap(handle + 1u);
  Dcm_CfgSvc06MidTidInfoPtrType  pMidTidInfo;
  Dcm_NegativeResponseCodeType   lNrc;

  for (; lHandle<lMaxHandle; lHandle++)
  {
    lStdResult = Dcm_CfgWrapSvc06MidTidInfoGetEntry(lHandle, &pMidTidInfo, &lNrc);                                                                   /* SBSW_DCM_COMB_PTR_FORWARD */

    if (lStdResult == DCM_E_OK)                                                                                                                      /* COV_DCM_RTM_RUNTIME_CHECK TX */ /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */
    {
      lStdResult = pMidTidInfo->OpFunc(opStatus, &lTestValue, &lMinLimit, &lMaxLimit, &lStatus);                                                     /* SBSW_DCM_POINTER_FORWARD_STACK */ /* SBSW_DCM_CALL_FUNCPTR_SVC06MIDTIDINFO */

      if (lStdResult == DCM_E_OK)
      {
        if (lStatus == DCM_DTRSTATUS_INVISIBLE)
        {
          lTestValue = 0;
          lMinLimit = 0;
          lMaxLimit = 0;
        }

        /* Copy data */
        Dcm_UtiProvideDataAsU8(pDataContext, pMidTidInfo->Tid);                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
        Dcm_UtiProvideDataAsU8(pDataContext, pMidTidInfo->Uasid);                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
        Dcm_UtiProvideDataAsU16(pDataContext, lTestValue);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
        Dcm_UtiProvideDataAsU16(pDataContext, lMinLimit);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
        Dcm_UtiProvideDataAsU16(pDataContext, lMaxLimit);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
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

# if (DCM_OBDUDSIDMGR_UDSMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ObdIdMgr_F6XX_IsDidAvailable()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdIdMgr_F6XX_IsDidAvailable(
  uint16 DID,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_DidSupportedType, AUTOMATIC, DCM_VAR_NOINIT) supported
  )
{
  uint8 lOBDMid = Dcm_UtiGetLoByte(DID);

  DCM_IGNORE_UNREF_PARAM(OpStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  *supported = DCM_DID_NOT_SUPPORTED;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */

  /* Check kind of DID (Availability ID or Data ID) */
  if (Dcm_ObdIdMgrIsAvailabilityId(DID))
  {
    /* If DID is a availability ID  */
    uint32 lObdMidAvailMask;

    /* Request value of availabity ID from Dem */
    if (Dem_DcmGetAvailableOBDMIDs(lOBDMid, &lObdMidAvailMask) == E_OK)                                                                              /* SBSW_DCM_POINTER_FORWARD_STACK */
    {
      /* If request ist successful */
      /* Report to caller that DID is available */
      *supported = DCM_DID_SUPPORTED;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
  else
  {
    /* If DID is Data ID */
    uint8 lNumOfTIDs;

    /* Request number of TIDs for Data ID */
    if (Dem_DcmGetNumTIDsOfOBDMID(lOBDMid, &lNumOfTIDs) == E_OK)                                                                                     /* SBSW_DCM_POINTER_FORWARD_STACK */
    {
      /* If request is successful */
      /* Report to caller that DID is available */
      *supported = DCM_DID_SUPPORTED;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }

  return DCM_E_OK;
}
# endif

# if (DCM_OBDUDSIDMGR_UDSMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ObdIdMgr_F6XX_ReadDidRangeDataLength()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdIdMgr_F6XX_ReadDidRangeDataLength(
  uint16 DID,
  Dcm_OpStatusType OpStatus,
  P2VAR(uint16, AUTOMATIC, DCM_VAR_NOINIT) DataLength
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  DCM_IGNORE_UNREF_PARAM(OpStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Check kind of DID (Availability ID or Data ID) */
  if (Dcm_ObdIdMgrIsAvailabilityId(DID))
  {
    /* If DID is an availability ID  */
    /* Set data length to 4 */
    *DataLength = 4u;                                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
  {
    /* Otherwise DID is a Data ID */
    uint8 lObdMid = Dcm_UtiGetLoByte(DID);
    uint8 lNumOfTIDs;

    /* Request number of TIDs for Data ID */
    if (Dem_DcmGetNumTIDsOfOBDMID(lObdMid, &lNumOfTIDs) == E_OK)                                                                                     /* SBSW_DCM_POINTER_FORWARD_STACK */
    {
      /* If request is successful calculate total response data length */
      *DataLength = (uint16)lNumOfTIDs * 8u;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      /* Otherwise something went wrong - report back */
      lStdResult = DCM_E_NOT_OK;
    }
  }

  return lStdResult;
}
# endif

# if (DCM_OBDUDSIDMGR_UDSMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ObdIdMgr_F6XX_ReadDidData()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdIdMgr_F6XX_ReadDidData(
  uint16 DID,
  Dcm_OpStatusType OpStatus,
  Dcm_DiagDataContextPtrType pDataContext
  )
{
  Std_ReturnType lStdResult;
  uint8 lObdMid = Dcm_UtiGetLoByte(DID);

  DCM_IGNORE_UNREF_PARAM(OpStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Check kind of DID (Availability ID or Data ID) */
  if (Dcm_ObdIdMgrIsAvailabilityId(DID))
  {
    /* If DID is a availability ID */
    /* Read value of availabiltity Id */
    lStdResult = Dcm_ObdIdMgr_F6XX_ReadDidData_SupportedId(lObdMid, pDataContext);                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  /* Otherwise */
  else
  {
    /* Read Value of Data Id */
    lStdResult = Dcm_ObdIdMgr_F6XX_ReadDidData_DataId(lObdMid, pDataContext);                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  return lStdResult;
}
# endif

# if (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON) && \
     (DCM_DIDMGR_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ObdIdMgrGetDidAvailabilityMask()
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
FUNC(uint32, DCM_CODE) Dcm_ObdIdMgrGetDidAvailabilityMask(
  uint16 DidLookUpRefStart,
  uint16 DidLookUpRefEnd,
  Dcm_ObdIdMgrMaskValueTableType maskValue
  )
{
  uint32 lResultMask;
  uint16_least lLookUpIter;

  DCM_IGNORE_UNREF_PARAM(maskValue);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lResultMask = 0;

  /* Iterate over all (WWH-)OBD DIDs related to the AID */
  for (lLookUpIter = DidLookUpRefStart;
    lLookUpIter <= DidLookUpRefEnd;
    ++lLookUpIter)
  {
    Dcm_DidMgrDidInfoContextType lDidInfoContext;
    uint8 lDidLowByte;

    /* Consider only the DID low-byte since the AID reports the availability mask only for a certain
    * DID range (e.g 0xF4XX).
    */
    lDidLowByte = Dcm_UtiGetLoByte(Dcm_CfgWrapDidMgrDidLookUpTable(lLookUpIter));

    lDidInfoContext.Descriptor.DidInfoIdx = (Dcm_CfgDidMgrDidInfoRefType)(lLookUpIter - 1u);
    Dcm_DidMgrInitOpInfo(&lDidInfoContext, DCM_DIDMGR_OP_READ);                                                                                      /* SBSW_DCM_POINTER_FORWARD_STACK */

    /* If the DID is (still) supported: */
    if (Dcm_DiagIsEnabledInActiveVariants(Dcm_CfgDidMgrGetDidOpInfo(&lDidInfoContext)->ExecCondRef) == TRUE)                                         /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    {
      /* Set the corresponding bit in the availability bitmap */
      Dcm_UtiBitOpSet(uint32, lResultMask, Dcm_ObdIdMgrGetObdIdMaskBit(lDidLowByte));
    }
  }

#  if (DCM_OBDUDSIDMGR_CALIBRATION_ENABLED == STD_ON)
  /* Consider any DIDs additionally suppressed by calibration procedure */
  lResultMask &= maskValue;
#  endif

  return lResultMask;
}
# endif

# if (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON) && \
     (DCM_RIDMGR_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ObdIdMgrGetRidAvailabilityMask()
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
FUNC(uint32, DCM_CODE) Dcm_ObdIdMgrGetRidAvailabilityMask(
  uint16 RidLookUpRefStart,
  uint16 RidLookUpRefEnd,
  Dcm_ObdIdMgrMaskValueTableType maskValue
  )
{
  uint32 lResultMask;
  uint16_least lLookUpIter;

  DCM_IGNORE_UNREF_PARAM(maskValue);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lResultMask = 0;
  /* Iterate over all (WWH-)OBD RIDs related to the AID */
  for (lLookUpIter = RidLookUpRefStart;
    lLookUpIter <= RidLookUpRefEnd;
    ++lLookUpIter)
  {
    uint8 lRidLowByte;
    /* Consider only the RID low-byte since the AID reports the availability mask only for a certain
    * RID range (e.g 0xE0XX).
    */
    lRidLowByte = Dcm_UtiGetLoByte(Dcm_CfgWrapRidMgrRidLookUpTable(lLookUpIter));

    /* If the RID is (still) supported: */
    if (Dcm_DiagIsEnabledInActiveVariants(Dcm_CfgWrapRidMgrRidInfo(lLookUpIter - 1u).ExecCondRef) == TRUE)                                           /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    {
      /* Set the corresponding bit in the availability bitmap */
      Dcm_UtiBitOpSet(uint32, lResultMask, Dcm_ObdIdMgrGetObdIdMaskBit(lRidLowByte));
    }
  }

#  if (DCM_OBDUDSIDMGR_CALIBRATION_ENABLED == STD_ON)
  /* Consider any RIDs additionally suppressed by calibration procedure */
  lResultMask &= maskValue;
#  endif

  return lResultMask;
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_OBDIDMGR_SUPPORT_ENABLED == STD_ON) || (DCM_OBDUDSIDMGR_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_ObdIdMgr.c
 *********************************************************************************************************************/
