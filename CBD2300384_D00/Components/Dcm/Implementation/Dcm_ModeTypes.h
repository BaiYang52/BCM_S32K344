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
/**        \file  Dcm_ModeTypes.h
 *         \unit  Mode
 *        \brief  Contains native types and forward declarations of Mode Manager unit.
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
#if !defined(DCM_MODETYPES_H)
# define DCM_MODETYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Types.h"
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
# if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
/*! DTC setting control states */
struct DCM_MODECTRLDTCSETTINGCONTEXTTYPE_TAG
{
#  if (DCM_SVC_85_DTC_GRP_ENABLED == STD_ON)
  uint32                             DTCGroup;               /*!< The DTC group associated with the disabled DTC setting state */
#  endif
  Rte_ModeType_DcmControlDtcSetting  Mode;                   /*!< Current DTC setting state (ENABLEDTCSETTING, DISABLEDTCSETTING) */
  uint8                              LastDisableDTCClientID; /*!< ClientID that was last used to disable DTC setting */
};
typedef struct DCM_MODECTRLDTCSETTINGCONTEXTTYPE_TAG Dcm_ModeCtrlDtcSettingContextType;
# endif

/*! Control data of the mode manager sub-component */
struct DCM_MODEMGRSINGLETONCONTEXTTYPE_TAG
{
# if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
  Dcm_ModeCtrlDtcSettingContextType CtrlDtcSetting; /*!< DTC setting mode management control data */
# else
  uint8 spare;                                      /*!< Reserved unused place for future extensions */
# endif
};
typedef struct DCM_MODEMGRSINGLETONCONTEXTTYPE_TAG Dcm_ModeMgrSingletonContextType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#endif /* !defined(DCM_MODETYPES_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_ModeTypes.h
 *********************************************************************************************************************/
