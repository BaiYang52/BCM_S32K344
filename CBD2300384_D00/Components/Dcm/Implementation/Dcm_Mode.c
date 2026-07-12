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
/**        \file  Dcm_Mode.c
 *         \unit  Mode
 *        \brief  Contains the implementation of Mode Manager unit.
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
#define DCM_MODE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Mode.h"
#include "Dcm_Tsk.h"
#include "BswM_Dcm.h"
#include "Rte_Dcm.h"
#include "Dcm_CfgWrap.h"
#include "Dcm_Svc28.h"
#include "Dcm_Svc85.h"

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
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_MODE_CTRLDTCSETTING_MONITOR_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeMonitorControlDtcSetting()
 *********************************************************************************************************************/
/*! \brief          Verifies that the DTCsetting auto-recovery condition is not yet met.
 *  \details        -
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_ModeMonitorControlDtcSetting(
  Dcm_TskEventContextPtrType pEventContext
  );
#endif

#if (DCM_MODE_COMMCTRL_MONITOR_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeMonitorCommunicationControl()
 *********************************************************************************************************************/
/*! \brief          Verifies that the CommunicationControl auto-recovery condition is not yet met.
 *  \details        -
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_ModeMonitorCommunicationControl(
  Dcm_TskEventContextPtrType pEventContext
  );
#endif

#if (DCM_MODE_ECU_RESET_ENABLED == STD_ON)   || \
    (DCM_MODE_RPD_SHTDWN_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSwitchAckResultInterpreter()
 *********************************************************************************************************************/
/*! \brief          Converts a ModeSwitchAck API return value to a DCM standard result.
 *  \details        -
 *  \param[in]      modeSwitchAckResult    The ModeSwitchAck API result
 *  \return         The DCM standard result
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ModeSwitchAckResultInterpreter(
  Std_ReturnType modeSwitchAckResult
  );
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Singleton context of ModeMgr unit */
DCM_LOCAL VAR(Dcm_ModeMgrSingletonContextType, DCM_VAR_NOINIT) Dcm_ModeMgrSingletonContext;
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CONST_8
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_MODE_COMMCTRL_ENABLED == STD_ON)
/*! Converts DCMs implementation type definition values into the RTE Mode symbolic name values */
DCM_LOCAL CONST(Dcm_CommunicationModeType, DCM_CONST) Dcm_ModeComCtrlModeMap[12]=                                                                    /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
   RTE_MODE_DcmCommunicationControl_DCM_ENABLE_RX_TX_NORM
  ,RTE_MODE_DcmCommunicationControl_DCM_ENABLE_RX_DISABLE_TX_NORM
  ,RTE_MODE_DcmCommunicationControl_DCM_DISABLE_RX_ENABLE_TX_NORM
  ,RTE_MODE_DcmCommunicationControl_DCM_DISABLE_RX_TX_NORMAL
  ,RTE_MODE_DcmCommunicationControl_DCM_ENABLE_RX_TX_NM
  ,RTE_MODE_DcmCommunicationControl_DCM_ENABLE_RX_DISABLE_TX_NM
  ,RTE_MODE_DcmCommunicationControl_DCM_DISABLE_RX_ENABLE_TX_NM
  ,RTE_MODE_DcmCommunicationControl_DCM_DISABLE_RX_TX_NM
  ,RTE_MODE_DcmCommunicationControl_DCM_ENABLE_RX_TX_NORM_NM
  ,RTE_MODE_DcmCommunicationControl_DCM_ENABLE_RX_DISABLE_TX_NORM_NM
  ,RTE_MODE_DcmCommunicationControl_DCM_DISABLE_RX_ENABLE_TX_NORM_NM
  ,RTE_MODE_DcmCommunicationControl_DCM_DISABLE_RX_TX_NORM_NM
};
#endif /* (DCM_MODE_COMMCTRL_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CONST_8
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_MODE_CTRLDTCSETTING_MONITOR_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeMonitorControlDtcSetting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_ModeMonitorControlDtcSetting(
  Dcm_TskEventContextPtrType pEventContext
  )
{
  Dcm_NegativeResponseCodeType lNrc;/* the value is not used, just the standard API requires it */
  Dcm_ModeRuleFuncType         lModeRuleFunc;
  Std_ReturnType               lResult;

  lResult = Dcm_CfgWrapModeMgrRulesGetEntry(DCM_MODE_CTRLDTCSETTING_MONITOR_MODERULE_REF, &lModeRuleFunc, &lNrc);                                    /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lResult == DCM_E_OK)                                                                                                                           /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    if (lModeRuleFunc(&lNrc) == TRUE)                                                                                                                /* SBSW_DCM_CALL_FUNCPTR_MODEMGRRULES */
    {
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_MODE_MON_SVC85);                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      /* fire and forget */
      (void)Dcm_Svc85EnableDtcSetting(Dcm_ModeMgrSingletonContext.CtrlDtcSetting.LastDisableDTCClientID);
    }
  }
}
#endif

#if (DCM_MODE_COMMCTRL_MONITOR_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeMonitorCommunicationControl()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_ModeMonitorCommunicationControl(
  Dcm_TskEventContextPtrType pEventContext
  )
{
  Dcm_NegativeResponseCodeType lNrc;/* the value is not used, just the standard API requires it */
  Dcm_ModeRuleFuncType         lModeRuleFunc;
  Std_ReturnType               lResult;

  lResult = Dcm_CfgWrapModeMgrRulesGetEntry(DCM_MODE_COMMCTRL_MONITOR_MODERULE_REF, &lModeRuleFunc, &lNrc);                                          /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lResult == DCM_E_OK)                                                                                                                           /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    if (lModeRuleFunc(&lNrc) == TRUE)                                                                                                                /* SBSW_DCM_CALL_FUNCPTR_MODEMGRRULES */
    {
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_MODE_MON_SVC28);                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      Dcm_Svc28EnableCommunication();
    }
  }
}
#endif

#if (DCM_MODE_ECU_RESET_ENABLED == STD_ON)   || \
    (DCM_MODE_RPD_SHTDWN_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSwitchAckResultInterpreter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ModeSwitchAckResultInterpreter(
  Std_ReturnType modeSwitchAckResult
  )
{
  Std_ReturnType lStdResult;

  switch (modeSwitchAckResult)
  {
  case RTE_E_TRANSMIT_ACK:
    /* switch accomplished */
    lStdResult = DCM_E_OK;
    break;
  case RTE_E_NO_DATA:
    lStdResult = DCM_E_PENDING;
    break;
  default: /* any error situation */
    lStdResult = DCM_E_NOT_OK;
    break;
  }
  return lStdResult;
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_MODE_COMMCTRL_ENABLED == STD_ON) || \
    (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_ModeInit(
  void
  )
{
  /* Initialize ControlDTCsetting internal mode */
  Dcm_ModeSetControlDtcSettingGroup(DEM_DTC_GROUP_ALL_DTCS);
  Dcm_ModeSetControlDtcSettingMode(RTE_MODE_DcmControlDtcSetting_ENABLEDTCSETTING);

# if (DCM_MODE_COMMCTRL_ENABLED == STD_ON)
  /* Initialize CommunicationControl internal mode */
  {
    uint8_least channelIter;
#  if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
    /* Get the current active communication variant */
    Dcm_CfgVarMgrComVariantType pComVariant = Dcm_NetGetActiveComVariant();
#  endif

    for (channelIter = 0; channelIter < Dcm_NetCfgGetNumAllComMChannels(pComVariant); ++channelIter)
    {
      Dcm_ModeSetCommControlState(channelIter, DCM_ENABLE_RX_TX_NORM_NM);
    }
  }
# endif
# if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
  Dcm_ModeMgrSingletonContext.CtrlDtcSetting.LastDisableDTCClientID = DCM_NET_DEFAULT_DEM_CLIENTID;
# endif
}
#endif

#if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeGetLastDisableDTCClientID()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, DCM_CODE) Dcm_ModeGetLastDisableDTCClientID(
  void
  )
{
  return Dcm_ModeMgrSingletonContext.CtrlDtcSetting.LastDisableDTCClientID;
}
#endif

#if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSetDTCMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_ModeSetDTCMode(
  Rte_ModeType_DcmControlDtcSetting mode
  )
{
  Dcm_ModeMgrSingletonContext.CtrlDtcSetting.Mode = mode;
}
#endif

#if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeGetDTCMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Rte_ModeType_DcmControlDtcSetting, DCM_CODE) Dcm_ModeGetDTCMode(
  void
  )
{
  return Dcm_ModeMgrSingletonContext.CtrlDtcSetting.Mode;
}
#endif

#if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON) && \
    (DCM_SVC_85_DTC_GRP_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSetDTCGroup()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_ModeSetDTCGroup(
  uint32 DTCGroup
  )
{
  Dcm_ModeMgrSingletonContext.CtrlDtcSetting.DTCGroup = DTCGroup;
}
#endif

#if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON) && \
    (DCM_SVC_85_DTC_GRP_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeGetDTCGroup()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint32, DCM_CODE) Dcm_ModeGetDTCGroup(
  void
  )
{
  return Dcm_ModeMgrSingletonContext.CtrlDtcSetting.DTCGroup;
}
#endif

#if (DCM_MODEMGR_CHECK_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeCheck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_ModeCheck(
  Dcm_CfgModeMgrRuleRefOptType modeRuleRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType       lStdReturn;
  Dcm_ModeRuleFuncType lModeRuleFunc;

  /* Retrieve mode rule verification handler */
  lStdReturn = Dcm_CfgWrapModeMgrRulesGetEntry(modeRuleRef, &lModeRuleFunc, ErrorCode);                                                              /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lStdReturn == DCM_E_OK)                                                                                                                        /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    Dcm_NegativeResponseCodeType lNrc = DCM_E_CONDITIONSNOTCORRECT;/* default NRC in case no rule specifies any NRC */

    /* Execute the mode-rule handler and return the result */
    if (lModeRuleFunc(&lNrc) == FALSE)                                                                                                               /* SBSW_DCM_CALL_FUNCPTR_MODEMGRRULES */
    {
      *ErrorCode = lNrc;                                                                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK;
    }
  }

  return lStdReturn;
}
#endif

#if (DCM_MODE_ECU_RESET_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSwitchAckEcuReset()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_ModeSwitchAckEcuReset(
  void
  )
{
  Std_ReturnType lStdResult;

  lStdResult = Rte_SwitchAck_DcmEcuReset_DcmEcuReset();

  return Dcm_ModeSwitchAckResultInterpreter(lStdResult);
}
#endif

#if (DCM_MODE_ECU_RESET_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSwitchEcuReset()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_ModeSwitchEcuReset(
  Rte_ModeType_DcmEcuReset mode
  )
{
  Std_ReturnType lStdResult;

  lStdResult = Rte_Switch_DcmEcuReset_DcmEcuReset(mode);

  return lStdResult;
}
#endif

#if (DCM_MODE_RPD_SHTDWN_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSwitchAckRapidShutDown()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_ModeSwitchAckRapidShutDown(
  void
  )
{
  Std_ReturnType lStdResult;

  lStdResult = Rte_SwitchAck_DcmModeRapidPowerShutDown_DcmModeRapidPowerShutDown();

  return Dcm_ModeSwitchAckResultInterpreter(lStdResult);
}
#endif

#if (DCM_MODE_RPD_SHTDWN_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSwitchRapidShutDown()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_ModeSwitchRapidShutDown(
  Rte_ModeType_DcmModeRapidPowerShutDown mode
  )
{
  Std_ReturnType lStdResult;

  lStdResult = Rte_Switch_DcmModeRapidPowerShutDown_DcmModeRapidPowerShutDown(mode);

  return lStdResult;
}
#endif

#if (DCM_MODE_ROE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSwitchRoE()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_ModeSwitchRoE(
  uint8 eventId,
  Rte_ModeType_DcmResponseOnEvent mode
  )
{
  Std_ReturnType                           lStdReturn;
  Dcm_CfgModeRoEEventModeSwitchInfoPtrType pModeSwitchInfo;

  lStdReturn = Dcm_CfgWrapModeRoEEventModeSwitchGetEntry(eventId, &pModeSwitchInfo);                                                                 /* SBSW_DCM_COMB_PTR_FORWARD */
  if (lStdReturn == DCM_E_OK)                                                                                                                        /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    (void)pModeSwitchInfo->OpFunc(mode);                                                                                                             /* SBSW_DCM_CALL_FUNCPTR_ROEMODESWITCH */
  }
}
#endif

#if (DCM_MODEMGR_MONITORING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeTaskMonitor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_ModeTaskMonitor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

# if (DCM_MODE_CTRLDTCSETTING_MONITOR_ENABLED == STD_ON)
  /* If activated, monitor DTCsetting related mode-rule fulfillment */
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_MODE_MON_SVC85))
  {
    Dcm_ModeMonitorControlDtcSetting(pEventContext);                                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
# endif

# if (DCM_MODE_COMMCTRL_MONITOR_ENABLED == STD_ON)
  /* If activated, monitor CommunicationControl related mode-rule fulfillment */
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_MODE_MON_SVC28))
  {
    Dcm_ModeMonitorCommunicationControl(pEventContext);                                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
# endif
}
#endif

#if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSwitchDtcSetting()
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
FUNC(void, DCM_CODE) Dcm_ModeSwitchDtcSetting(
  Rte_ModeType_DcmControlDtcSetting newMode,
  uint32 DTCGroup,
  uint8 DemClientId
  )
{
  /* execute transition */
  (void)Rte_Switch_DcmControlDtcSetting_DcmControlDtcSetting(newMode);

  DCM_IGNORE_UNREF_PARAM(DTCGroup);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(DemClientId);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Update local states */
  Dcm_ModeSetControlDtcSettingGroup(DTCGroup);
  Dcm_ModeSetControlDtcSettingMode(newMode);

# if (DCM_MODE_CTRLDTCSETTING_MONITOR_ENABLED == STD_ON)
  if (newMode == RTE_MODE_DcmControlDtcSetting_DISABLEDTCSETTING)
  {
    Dcm_ModeMgrSingletonContext.CtrlDtcSetting.LastDisableDTCClientID = DemClientId;
    Dcm_TskSetEvent(DCM_TSK_ID_MODE_MONITOR, DCM_TSK_EV_MODE_MON_SVC85);
  }
  else
  {
    Dcm_TskClrEvent(DCM_TSK_ID_MODE_MONITOR, DCM_TSK_EV_MODE_MON_SVC85);
  }
# endif
}
#endif

#if (DCM_MODE_COMMCTRL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ModeSwitchCommControl()
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
FUNC(void, DCM_CODE) Dcm_ModeSwitchCommControl(
  Dcm_CfgNetNetIdRefOptType networkRef,
  Dcm_CommunicationModeType newMode
  )
{
  NetworkHandleType lNetwork = Dcm_PbCfgNetAllComMChannelMapGetEntry(networkRef);

  BswM_Dcm_CommunicationMode_CurrentState(lNetwork, newMode);/* notify BswM */
  Dcm_ModeOnComControlModeChange(lNetwork, Dcm_ModeComCtrlModeMap[newMode]); /* notify the application */

  Dcm_ModeSetCommControlState(networkRef, newMode);

# if (DCM_MODE_COMMCTRL_MONITOR_ENABLED == STD_ON)
  if ((newMode & DCM_SVC_28_CTRL_OP_MASK) != 0u)/* operation is with any disabling */
  {
    Dcm_TskSetEvent(DCM_TSK_ID_MODE_MONITOR, DCM_TSK_EV_MODE_MON_SVC28);
  }
  else
  {
    Dcm_TskClrEvent(DCM_TSK_ID_MODE_MONITOR, DCM_TSK_EV_MODE_MON_SVC28);
  }
# endif
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Mode.c
 *********************************************************************************************************************/
