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
/**        \file  Dcm_DemIf.c
 *         \unit  DemIf
 *        \brief  Contains the implementation of Dem Interface unit.
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
#define DCM_DEMIF_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_DemIf.h"
#include "Dcm_Debug.h"

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

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CONST_8
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_SVC_19_SUPPORT_ENABLED == STD_ON) || \
    (DCM_OBDDTCMGR_SUPPORT_ENABLED == STD_ON)
/*! Dem GetNumberOfFilteredDTC API return value to NRC mapping */
CONST(uint8, DCM_CONST) Dcm_DemIfNrcMapGetNumberOfFilteredDTC[] =
{
# if (DCM_DEMIF_421_ENABLED == STD_ON)
  DEM_NUMBER_FAILED, DCM_E_CONDITIONSNOTCORRECT,
# endif
  E_OK /* end marker  - E_OK shall never be used as an error code */
};
#endif

#if (DCM_SVC_19_SUPPORT_ENABLED == STD_ON) || \
    (DCM_OBDDTCMGR_SUPPORT_ENABLED == STD_ON)

/*! Dem SetDTCFilter API return value to NRC mapping */
CONST(uint8, DCM_CONST) Dcm_DemIfNrcMapSetDTCFilter[3] =
{
  DCM_DEMIF_RET_WRONG_FILTER, DCM_E_REQUESTOUTOFRANGE,
  E_OK /* end marker  - E_OK shall never be used as an error code */
};
#endif

#if (DCM_DEMIF_SELECT_DTC_ENABLED == STD_ON)
/*! Dem SelectDTC API return value to NRC mapping */
CONST(uint8, DCM_CONST) Dcm_DemIfNrcMapSelectDTC[3] =
{
  DCM_DEMIF_RET_SELECT_BUSY, DCM_E_CONDITIONSNOTCORRECT,
  E_OK /* end marker  - E_OK shall never be used as an error code */
};
#endif

#if (DCM_DEMIF_CLEAR_DTC_ENABLED == STD_ON)
/*! Dem ClearDTC API return value to NRC mapping */
CONST(uint8, DCM_CONST) Dcm_DemIfNrcMapClearDTC[] =
{
  DCM_DEMIF_RET_CLEAR_W_DTC,        DCM_E_REQUESTOUTOFRANGE,
  DCM_DEMIF_RET_CLEAR_FAILED,       DCM_E_CONDITIONSNOTCORRECT,
  DCM_DEMIF_RET_CLEAR_CLEAR_BUSY,   DCM_E_CONDITIONSNOTCORRECT,
# if defined(DCM_DEMIF_RET_CLEAR_BUSY)
  DCM_DEMIF_RET_CLEAR_BUSY,         DCM_E_CONDITIONSNOTCORRECT,
# endif
  DCM_DEMIF_RET_CLEAR_W_ORIGIN,     DCM_E_REQUESTOUTOFRANGE,
  E_OK /* end marker  - E_OK shall never be used as an error code */
};
#endif
#define DCM_STOP_SEC_CONST_8
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_DEMIF_RETVAL_TO_NRC_MAP_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DemIfGetNrcForDemRetValue()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Dcm_NegativeResponseCodeType, DCM_CODE) Dcm_DemIfGetNrcForDemRetValue(
  uint8 demResult,
  Dcm_Uint8ConstDataPtrType demResult2NrcMap
  )
{
  Dcm_NegativeResponseCodeType lNrcResult = DCM_E_PANIC_NRC;
  Dcm_Uint8ConstDataPtrType lDemResult2NrcMap = demResult2NrcMap;

  /* Iterate over the "DEM result to NRC" using increment by two to visit each <key,value> pair */
  while (lDemResult2NrcMap[0] != E_OK)
  {
    /* If a match is found, return the associated NRC */
    if (lDemResult2NrcMap[0] == demResult)
    {
      lNrcResult = lDemResult2NrcMap[1];                                                                                                             /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
      break;
    }
    /* Otherwise return the panic NRC */

    /* Increment by two for next map entry */
    lDemResult2NrcMap = &lDemResult2NrcMap[2];                                                                                                       /* PRQA S 2931 */ /* MD_Dcm_ComputingInvalidPointer */
  }

  /* Check if the DEM has returned an unexpected value (i.e. default case in a switch-case dispatcher) */
  if (Dcm_DebugDetectError(lDemResult2NrcMap[0] == E_OK))                                                                                            /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
  }

  /* Return final result (concrete or panic NRC) */
  return lNrcResult;
}
#endif

#if (DCM_DEMIF_GETNEXTFILTEREDITEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DemIfConvDemRsltGetNextFilteredItem()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DemIfConvDemRsltGetNextFilteredItem(
  Dcm_DemIfReturnGetNextFilteredItemType demResult,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lDcmResult;
  switch (demResult)
  {
  case DCM_DEMIF_RET_FILTERED_NO_ITEMS:
    /* If no more items to be reported, signal data ready with eventual data padding */
    lDcmResult = DCM_E_DATA_READY_PADDING; /* signal end of list */
    break;
  case DCM_DEMIF_RET_FILTERED_PENDING:
    /* If Dem cannot provide next filtered item immediately, signal a retry */
    lDcmResult = DCM_E_PENDING;/* signal end of list */
    break;
  case DCM_DEMIF_RET_FILTERED_BUFFER_TOO_SMALL:
    /* If Dem cannot provide the data due to a too low buffer allocation, signal buffer overrun */
    lDcmResult = DCM_E_BUFFERTOOLOW;
    break;
  default:
    /* Any other Dem result is treated as an error, thus signal job closing with panic NRC */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);

    lDcmResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  }
  return lDcmResult;
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_DemIf.c
 *********************************************************************************************************************/
