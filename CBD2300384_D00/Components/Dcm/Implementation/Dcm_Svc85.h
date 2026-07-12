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
/**        \file  Dcm_Svc85.h
 *         \unit  Svc85
 *        \brief  Contains public inline function definitions of Service 0x85 unit.
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
#if !defined(DCM_SVC85_H)
# define DCM_SVC85_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Svc85Types.h"
# if (DCM_SVC_85_SUPPORT_ENABLED == STD_ON)
#  include "Dcm_DiagTypes.h"
#  include "Rte_Dcm_Type.h"
#  include "Dcm_DemIf.h"

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
/*! Service 0x85 temporary processing context */
struct DCM_SVC85_REPEATERPROXYCONTEXTTYPE_TAG
/*! Service 0x85 temporary processing context */
{
  uint32                             DTCGroup;    /*!< Requested DTC for the ModeSwitch operation during the post-processing */
  Rte_ModeType_DcmControlDtcSetting  ReqMode;     /*!< Requested mode for the ModeSwitch operation during the post-processing */
  Dcm_DiagSubServiceRefOptType       SubSvcRef;   /*!< Reference to Dcm_CfgSvc85SubFuncInfo[] entry */
  uint8                              DemClientId; /*!< DEM ClientId */
};
typedef struct DCM_SVC85_REPEATERPROXYCONTEXTTYPE_TAG Dcm_Svc85RepeaterProxyContextType;
typedef P2VAR(Dcm_Svc85RepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc85RepeaterProxyContextPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
#  if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc85DisableDtcSetting()
 *********************************************************************************************************************/
/*! \brief          Service 0x85 disabler.
 *  \details        Calls the DisableDtcSetting function.
 *  \param[in]      DTCGroup               The DTC Group to be disabled
 *  \param[in]      DemClientId            The DEM ClientId
 *  \return         DCM_E_OK               Operation is done positive
 *  \return         DCM_E_NOT_OK           Operation is done negative
 *  \return         DCM_E_PENDING          Final result is pending, retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc85DisableDtcSetting(
  Dem_DTCGroupType DTCGroup,
  uint8 DemClientId
  );
#  endif

/**********************************************************************************************************************
 *  Dcm_Svc85EnableDtcSetting()
 *********************************************************************************************************************/
/*! \brief          Service 0x85 enabler.
 *  \details        Calls the EnableDtcSetting function.
 *  \param[in]      DemClientId            The DEM ClientId
 *  \return         DCM_E_OK               Operation is done positive
 *  \return         DCM_E_NOT_OK           Operation is done negative
 *  \return         DCM_E_PENDING          Final result is pending, retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc85EnableDtcSetting(
  uint8 DemClientId
  );

/**********************************************************************************************************************
 *  Dcm_Svc85OnChangeSession()
 *********************************************************************************************************************/
/*! \brief          Service 0x85 utility function about a session change.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc85OnChangeSession(
  void
  );
#  define DCM_STOP_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_SVC_85_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_SVC85_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc85.h
 *********************************************************************************************************************/
