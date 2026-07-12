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
/**        \file  Dcm_ObdDtcMgr.h
 *         \unit  ObdDtcMgr
 *        \brief  Contains public inline function definitions of OBD DTC Manager unit.
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
#if !defined(DCM_OBDDTCMGR_H)
# define DCM_OBDDTCMGR_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_ObdDtcMgrTypes.h"
# if (DCM_OBDDTCMGR_SUPPORT_ENABLED == STD_ON)
#  include "Dcm_Repeater.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! OBD DTC manager temporary processing context */
struct DCM_OBDDTCMANAGERCONTEXTTYPE_TAG
{
  Dcm_RepeaterProgressType  DcmObdDtcManagerProgress; /*!< Repeater progress state */
  uint8                     DemClientId;              /*!< DEM ClientId */
};
typedef struct DCM_OBDDTCMANAGERCONTEXTTYPE_TAG Dcm_ObdDtcManagerContextType;
typedef P2VAR(Dcm_ObdDtcManagerContextType, TYPEDEF, DCM_VAR_NOINIT)  Dcm_ObdDtcManagerContextPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_ObdDtcMgrDemSetDTCFilter()
 *********************************************************************************************************************/
/*! \brief          Sets the DTC Filter in Dem.
 *  \details        -
 *  \param[in]      StatusMask              DTC Statusmask
 *  \param[in]      Origin                  DTC Origin
 *  \param[in,out]  pMsgContext             Current request context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdDtcMgrDemSetDTCFilter(
  uint8 StatusMask,
  Dem_DTCOriginType Origin,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_ObdDtcManagerContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_ObdDtcMgrReadDtcByStatusMask()
 *********************************************************************************************************************/
/*! \brief          Processes a DTC read by status mask service for OBD.
 *  \details        -
 *  \param[in]      opStatus                The operation status
 *  \param[in,out]  pMsgContext             Current request context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_STOP_REPEATER     Stop Repeater Proxy
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            DTC Filter set in Dem (Dcm_ObdDtcMgrDemSetDTCFilter)
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdDtcMgrReadDtcByStatusMask(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_ObdDtcManagerContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_ObdDtcMgrCpyDemGetNxtFltrdDtc()
 *********************************************************************************************************************/
/*! \brief          Data processor of service (0x03/0x07/0x0A).
 *  \details        Calls the GetNextFilteredDTC function.
 *  \param[in,out]  pDataContext                Pointer to the data context
 *  \param[out]     ErrorCode                   The NRC
 *  \param[in,out]  pRepContext                 Current repeater proxy context
 *  \return         DCM_E_OK                    All data copied
 *  \return         DCM_E_NOT_OK                Error occured
 *  \return         DCM_E_PENDING               Retry later
 *  \return         DCM_E_BUFFERTOOLOW          Retry later, because of no available space any more
 *  \return         DCM_E_DATA_READY_PADDING    Force padding of remaining bytes
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdDtcMgrCpyDemGetNxtFltrdDtc(
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_ObdDtcManagerContextPtrType pRepContext
  );
#  define DCM_STOP_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_OBDDTCMGR_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_OBDDTCMGR_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_ObdDtcMgr.h
 *********************************************************************************************************************/
