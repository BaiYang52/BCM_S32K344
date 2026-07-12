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
/**        \file  Dcm_Mode.h
 *         \unit  Mode
 *        \brief  Contains public inline function definitions of Mode Manager unit.
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
#if !defined(DCM_MODE_H)
# define DCM_MODE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_ModeTypes.h"
# include "Dcm_Net.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# if (DCM_MODE_CTRLDTCSETTING_MONITOR_ENABLED == STD_ON) || \
     (DCM_MODE_COMMCTRL_MONITOR_ENABLED == STD_ON)
#  define DCM_MODEMGR_MONITORING_ENABLED                             STD_ON
# else
#  define DCM_MODEMGR_MONITORING_ENABLED                             STD_OFF
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! Configuration dependent getter/setter access to the communication control state */
# if (DCM_MODE_COMMCTRL_ENABLED == STD_ON)
#  define Dcm_ModeGetCommControlState(networkRef)                    (Dcm_PbRamNetComCtrlChannels[(networkRef)])                                     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_ModeSetCommControlState(networkRef, mode)              (Dcm_NetSetComControlChannelState((networkRef), (mode)))                        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# else
#  define Dcm_ModeGetCommControlState(networkRef)                    /* not used */
#  define Dcm_ModeSetCommControlState(networkRef, mode)              /* not used */
# endif

/*! Configuration dependent getter/setter access to the DTC setting state */
# if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
#  define Dcm_ModeGetControlDtcSettingMode()                         (Dcm_ModeGetDTCMode())                                                          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_ModeSetControlDtcSettingMode(newMode)                  (Dcm_ModeSetDTCMode(newMode))                                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# else
#  define Dcm_ModeGetControlDtcSettingMode()                         /* not used */
#  define Dcm_ModeSetControlDtcSettingMode(newMode)                  /* not used */
# endif

/*! Configuration dependent getter/setter access to the DTC group associated to the already set DTC setting's state */
# if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON) && \
     (DCM_SVC_85_DTC_GRP_ENABLED == STD_ON)
#  define Dcm_ModeGetControlDtcSettingGroup()                        (Dcm_ModeGetDTCGroup())                                                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_ModeSetControlDtcSettingGroup(newGroup)                (Dcm_ModeSetDTCGroup(newGroup))                                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# else
#  define Dcm_ModeGetControlDtcSettingGroup()                        /* not used */
#  define Dcm_ModeSetControlDtcSettingGroup(newGroup)                /* not used */
# endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_MODE_COMMCTRL_ENABLED == STD_ON) || \
     (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeInit()
 *********************************************************************************************************************/
/*! \brief          Initializes DCM mode manager sub-module.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Global interrupts must be disabled.
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ModeInit(
  void
  );
# endif

# if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeGetLastDisableDTCClientID()
 *********************************************************************************************************************/
/*! \brief          Get a Mode manager last used ClientID that disabled DTC setting.
 *  \details        -
 *  \return         Valid Mode manager ClientID.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(uint8, DCM_CODE) Dcm_ModeGetLastDisableDTCClientID(
  void
  );
# endif

# if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSetDTCMode()
 *********************************************************************************************************************/
/*! \brief          Set a Mode manager Current DTC setting state.
 *  \details        -
 *  \param[in]      mode     Mode manager Current DTC setting state.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ModeSetDTCMode(
  Rte_ModeType_DcmControlDtcSetting mode
  );
# endif

# if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeGetDTCMode()
 *********************************************************************************************************************/
/*! \brief          Returns a Mode manager Current DTC setting state.
 *  \details        -
 *  \return         Valid Mode manager DTC setting state.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUEv
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Rte_ModeType_DcmControlDtcSetting, DCM_CODE) Dcm_ModeGetDTCMode(
  void
  );
# endif

# if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON) && \
     (DCM_SVC_85_DTC_GRP_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSetDTCGroup()
 *********************************************************************************************************************/
/*! \brief         Set Mode manager DTC group associated with the disabled DTC setting state.
 *  \details        -
 *  \param[in]      DTCGroup     DTC group associated with the disabled DTC setting state.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ModeSetDTCGroup(
  uint32 DTCGroup
  );
# endif

# if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON) && \
     (DCM_SVC_85_DTC_GRP_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeGetDTCGroup()
 *********************************************************************************************************************/
/*! \brief          Get Mode manager DTC group associated with the disabled DTC setting state.
 *  \details        -
 *  \return         Valid Mode manager DTC group.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(uint32, DCM_CODE) Dcm_ModeGetDTCGroup(
  void
  );
# endif

# if (DCM_MODEMGR_CHECK_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeCheck()
 *********************************************************************************************************************/
/*! \brief          Performs a mode rule check.
 *  \details        -
 *  \param[in]      modeRuleRef    A reference to a mode rule
 *  \param[out]     ErrorCode      The NRC
 *  \return         DCM_E_OK       Mode rule check was successful
 *  \return         DCM_E_NOT_OK   Mode rule check was not successful
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_ModeCheck(
  Dcm_CfgModeMgrRuleRefOptType modeRuleRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_MODE_ECU_RESET_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSwitchAckEcuReset()
 *********************************************************************************************************************/
/*! \brief          Mode switch acknowledgment for an ECU reset command.
 *  \details        -
 *  \return         The operation result
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_ModeSwitchAckEcuReset(
  void
  );
# endif

# if (DCM_MODE_ECU_RESET_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSwitchEcuReset()
 *********************************************************************************************************************/
/*! \brief          Mode switching for an ECU reset command.
 *  \details        -
 *  \param[in]      mode    The mode
 *  \return         The operation result
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_ModeSwitchEcuReset(
  Rte_ModeType_DcmEcuReset mode
  );
# endif

# if (DCM_MODE_RPD_SHTDWN_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSwitchAckRapidShutDown()
 *********************************************************************************************************************/
/*! \brief          Mode switch acknowledgment for a rapid power shutdown command.
 *  \details        -
 *  \return         The operation result
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_ModeSwitchAckRapidShutDown(
  void
  );
# endif

# if (DCM_MODE_RPD_SHTDWN_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSwitchRapidShutDown()
 *********************************************************************************************************************/
/*! \brief          Mode switching for an ECU reset rapid shut down command.
 *  \details        -
 *  \param[in]      mode    The mode
 *  \return         The operation result
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_ModeSwitchRapidShutDown(
  Rte_ModeType_DcmModeRapidPowerShutDown mode
  );
# endif

# if (DCM_MODE_ROE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSwitchRoE()
 *********************************************************************************************************************/
/*! \brief          Mode switching for RoE activation state change.
 *  \details        -
 *  \param[in]      eventId    The id used for triggering RoE events
 *  \param[in]      mode       The mode
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ModeSwitchRoE(
  uint8 eventId,
  Rte_ModeType_DcmResponseOnEvent mode
  );
# endif

# if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSwitchDtcSetting()
 *********************************************************************************************************************/
/*! \brief          Mode management on control DTC setting.
 *  \details        -
 *  \param[in]      newMode       The mode
 *  \param[in]      DTCGroup      The affected DTC group of changed setting
 *  \param[in]      DemClientId   The DEM client id
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ModeSwitchDtcSetting(
  Rte_ModeType_DcmControlDtcSetting newMode,
  uint32 DTCGroup,
  uint8 DemClientId
  );
# endif

# if (DCM_MODE_COMMCTRL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSwitchCommControl()
 *********************************************************************************************************************/
/*! \brief          Mode management on communication control.
 *  \details        -
 *  \param[in]      networkRef The network handle
 *  \param[in]      newMode    The mode
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ModeSwitchCommControl(
  Dcm_CfgNetNetIdRefOptType networkRef,
  Dcm_CommunicationModeType newMode
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#endif /* !defined(DCM_MODE_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Mode.h
 *********************************************************************************************************************/
