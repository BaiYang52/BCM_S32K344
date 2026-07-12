/* ********************************************************************************************************************
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
 *  -------------------------------------------------------------------------------------------------------------------
 */
/*!
 *  \defgroup   Dem_GlobalDiagnosticsIF Dem GlobalDiagnosticsIF Functionality
 *  \{
 *  \file       Dem_GlobalDiagnosticsIF_SvcImplementation.h
 *  \brief      Service implementation of unit GlobalDiagnosticsIF.
 *  \details    Realizes Dem external service APIs.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_GLOBALDIAGNOSTICSIF_SVCIMPLEMENTATION_H)
#define DEM_GLOBALDIAGNOSTICSIF_SVCIMPLEMENTATION_H

/* ********************************************************************************************************************
 *  GLOBAL MISRA JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2880 EOF */ /* MD_DEM_2.1_UNREACHABLE_STATEMENT */
/* PRQA S 2982 EOF */ /* MD_DEM_2982 */
/* PRQA S 2983 EOF */ /* MD_DEM_2983 */
/* PRQA S 2985 EOF */ /* MD_DEM_2985 */
/* PRQA S 2986 EOF */ /* MD_DEM_2986 */
/* PRQA S 2987 EOF */ /* MD_DEM_2987 */
/* PRQA S 2991 EOF */ /* MD_DEM_2991 */
/* PRQA S 2992 EOF */ /* MD_DEM_2992 */
/* PRQA S 2995 EOF */ /* MD_DEM_2995 */
/* PRQA S 2996 EOF */ /* MD_DEM_2996 */
/* PRQA S 3415 EOF */ /* MD_DEM_13.5_cf */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                    /* Used subcomponent API */
/* ------------------------------------------------------------------------- */
#include "Dem_StorageCondition_Interface.h"
#include "Dem_Conditions_Interface.h"
#include "Dem_GlobalDiagnosticsIF_Interface.h"
#include "Dem_GlobalState_Interface.h"
#include "Dem_PidStatistics_Interface.h"
#include "Dem_Readiness_Interface.h"
#include "Dem_Pto_Interface.h"

#include "Dem_RecordReader_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Client_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_DiagnosticObserver_Interface.h"
#include "Dem_FaultMemory_Interface.h"

                                                   /* Subcomponents callbacks*/
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  FILE SPECIFIC MISRA VIOLATIONS
 *********************************************************************************************************************/
/* PRQA S 3406 DEM_API_IMPL */ /* MD_DEM_8.6 */


/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_GLOBALDIAGNOSTICSIF_SVCIMPLEMENTATION_FILENAME "Dem_GlobalDiagnosticsIF_SvcImplementation.h"

 /* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_GlobalDiagnosticsIF_Private Private Methods
 * \{
 */
#if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_WWHOBD == STD_ON) && (DEM_CFG_SUPPORT_PID91 == STD_ON))                                                                                         
/* ****************************************************************************
 * Dem_GlobalDiagnostics_TranslateActivationMode
 *****************************************************************************/
/*!
 * \brief         Maps the activation mode state to DCM format
 *
 * \details       This function maps the given activation mode state to the
 *                respective DCM representation:
 *                DEM_INDICATOR_CONTINUOUS -> 0x03
 *                DEM_INDICATOR_SHORT      -> 0x02
 *                DEM_INDICATOR_ON_DEMAND  -> 0x01
 *                DEM_INDICATOR_OFF        -> 0x00
 *
 * \param[in]     ActivationMode
 *                The internal activation mode state
 *
 * \return        DCM encoded Activation Mode
 *
 * \pre           -
 * \config          ((DEM_CFG_SUPPORT_DCM == STD_ON) \
 *                && (DEM_CFG_SUPPORT_WWHOBD == STD_ON) \ 
 *                && (DEM_CFG_SUPPORT_PID91 == STD_ON))    
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_TranslateActivationMode(
  CONST(Dem_IndicatorStatusType, AUTOMATIC)  ActivationMode
  );
#endif

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

 #if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_WWHOBD == STD_ON) && (DEM_CFG_SUPPORT_PID91 == STD_ON))
/* ****************************************************************************
 % Dem_GlobalDiagnostics_TranslateActivationMode
 *****************************************************************************/
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
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_TranslateActivationMode(
  CONST(Dem_IndicatorStatusType, AUTOMATIC)  ActivationMode
  )
{
  uint8 lReturnValue;

  switch (ActivationMode)
  {
  case DEM_INDICATOR_CONTINUOUS:
    lReturnValue = 0x03U;
    break;
  case DEM_INDICATOR_SHORT:
    lReturnValue = 0x02U;
    break;
  case DEM_INDICATOR_ON_DEMAND:
    lReturnValue = 0x01U;
    break;
  case DEM_INDICATOR_OFF:
    lReturnValue = 0x00U;
    break;
  default:
    Dem_Error_ReportError(DEM_INTERNAL_APIID, DEM_E_INCONSISTENT_STATE);
    lReturnValue = 0x00U;
    break;
  }
  return lReturnValue;
}
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_GlobalDiagnosticsIF_Public
 * \{
 */
 
#if (DEM_CFG_SUPPORT_ENABLE_CONDITIONS == STD_ON)
/* ****************************************************************************
 % Dem_SetEnableCondition
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_SetEnableCondition(
  uint8  EnableConditionID,                                                                                                      /* PRQA S 1330 */ /* MD_DEM_8.3_rte */
  boolean  ConditionFulfilled
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;
  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_IsEnableConditionNotDtcSettingAndValid(EnableConditionID) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  if (Dem_APIChecks_IsBoolean(ConditionFulfilled) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    if (ConditionFulfilled == TRUE)
    {
      Dem_Conditions_EnableCondition_SetFulfilled(EnableConditionID);
    }
    else
    {
      Dem_Conditions_EnableCondition_ResetFulfilled(EnableConditionID);
    }

    lReturnValue = E_OK;
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_SETENABLECONDITION_APIID, Dem_Det_ErrorRegisterGet());
  }
  return lReturnValue;
}                                                                                                                                 /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_ENABLE_CONDITIONS == STD_ON) */

#if (DEM_CFG_SUPPORT_STORAGE_CONDITIONS == STD_ON)
/* ****************************************************************************
 % Dem_SetStorageCondition
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_SetStorageCondition(
  uint8  StorageConditionID,                                                                                                       /* PRQA S 1330 */ /* MD_DEM_8.3_rte */
  boolean  ConditionFulfilled
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;
  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_IsStorageConditionIDValid(StorageConditionID) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  if (Dem_APIChecks_IsBoolean(ConditionFulfilled) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    if (ConditionFulfilled == TRUE)
    {
      Dem_Conditions_StorageCondition_SetFulfilled(StorageConditionID);
    }
    else
    {
      Dem_Conditions_StorageCondition_ResetFulfilled(StorageConditionID);
    }

    lReturnValue = E_OK;
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_SETSTORAGECONDITION_APIID, Dem_Det_ErrorRegisterGet());
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_STORAGE_CONDITIONS == STD_ON) */


#if (DEM_CFG_SUPPORT_ENABLE_CONDITIONS == STD_ON)
/* ****************************************************************************
 % Dem_GetEventEnableCondition
 *****************************************************************************/
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
FUNC(Std_ReturnType, DEM_CODE)
Dem_GetEventEnableCondition(
  Dem_EventIdType  EventId,                                                                                                      /* PRQA S 1330, 3206 */ /* MD_DEM_8.3_rte, MD_DEM_3206 */
  P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA)  ConditionFulfilled
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;
  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

    /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_GlobalConfigPointerInState(DEM_CONFIGVALIDATION_CONFIGPTR_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_EventId(EventId) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  if (Dem_APIChecks_NullPointer(ConditionFulfilled) == FALSE)                                                                    /* SBSW_DEM_POINTER_NULLPTR_CHECKING */ /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    if ( (Dem_Event_TestEventAvailable(EventId) == TRUE) )
    {
      *ConditionFulfilled = Dem_Condition_EnableCondition_TestEnableConditionStateOfEvent(EventId);                                        /* SBSW_DEM_POINTER_WRITE_API */
      lReturnValue = E_OK;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_GETENABLECONDITION_APIID, Dem_Det_ErrorRegisterGet());
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_ENABLE_CONDITIONS == STD_ON) */


                                               /* Permanent Fault Code Cycle */
/* ------------------------------------------------------------------------- */
#if (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON) && (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_SetPfcCycleQualified
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_SetPfcCycleQualified(
  void
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;
  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsObdIIOrObdOnUdsSupportedInVariant() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant())
    {
    /* ----- Implementation ------------------------------------------------ */
/* >>>> -------------------------------- Enter Critical Section: StateManager */
      Dem_EnterCritical_StateManager();

      Dem_OperationCycle_SetObdCycleStates(DEM_OPERATIONCYCLE_SET_PFC_CYCLE(Dem_OperationCycle_GetObdCycleStates()));

      Dem_LeaveCritical_StateManager();
/* <<<< -------------------------------- Leave Critical Section: StateManager */

      lReturnValue = E_OK;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_SETPFCCYCLEQUALIFIED_APIID, Dem_Det_ErrorRegisterGet());
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif


                                                               /* B1 Counter */
/* ------------------------------------------------------------------------- */
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
/* ****************************************************************************
 % Dem_GetB1Counter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_GetB1Counter(
  P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA)  B1Counter                                                                             /* PRQA S 1330 */ /* MD_DEM_8.3_rte */
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;
  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsWwhObdSupportedInVariant() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_NullPointer(B1Counter) == FALSE)                                                                             /* SBSW_DEM_POINTER_NULLPTR_CHECKING */  /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    if (Dem_Cfg_IsWwhObdSupportedInVariant())
    {
      /* internal resolution of 1min/bit must be converted to 1h/bit */
      *B1Counter = (uint16)(Dem_ActivationMode_B1Counter_GetValue()/60u);                                                                         /* SBSW_DEM_POINTER_WRITE_API */
      lReturnValue = E_OK;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_GETB1COUNTER_APIID, Dem_Det_ErrorRegisterGet());
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DisableDTCSetting
 *****************************************************************************/
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
FUNC(Std_ReturnType, DEM_CODE)
Dem_DisableDTCSetting(
  uint8  ClientId
  )
{
  Std_ReturnType lReturnValue;

  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_ClientId(ClientId) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else  
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dem_Conditions_EnableCondition_ResetFulfilled(DEM_ENABLECONDITION_CONTROLDTCSETTING);

    Dem_Notifications_ControlDtcSettingChanged(FALSE);

    lReturnValue = E_OK;
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_DISABLEDTCSETTING_APIID, Dem_Det_ErrorRegisterGet());
  }

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_EnableDTCSetting
 *****************************************************************************/
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
FUNC(Std_ReturnType, DEM_CODE)
Dem_EnableDTCSetting(
  uint8  ClientId
  )
{
  Std_ReturnType lReturnValue;

  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_ClientId(ClientId) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else  
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dem_Conditions_EnableCondition_SetFulfilled(DEM_ENABLECONDITION_CONTROLDTCSETTING);

    Dem_Notifications_ControlDtcSettingChanged(TRUE);
    lReturnValue = E_OK;
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_ENABLEDTCSETTING_APIID, Dem_Det_ErrorRegisterGet());
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_PID1C == STD_ON)
/* ****************************************************************************
 % Dem_DcmReadDataOfPID1C
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmReadDataOfPID1C(
  P2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  PID1Cvalue                                                                              /* PRQA S 1330 */ /* MD_DEM_8.3_dcm */
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsObdSupportedInVariant() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_NullPointer(PID1Cvalue) == FALSE)                                                                       /* SBSW_DEM_POINTER_NULLPTR_CHECKING */  /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else  
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
    {
      *PID1Cvalue = Dem_Cfg_GlobalObdRequirements();                                                                               /* SBSW_DEM_POINTER_WRITE_API */
      lReturnValue = E_OK;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_DCMREADDATAOFPID1C_APIID, Dem_Det_ErrorRegisterGet());
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_PID1C == STD_ON) */


#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_PID21 == STD_ON)
/* ****************************************************************************
 % Dem_DcmReadDataOfPID21
 *****************************************************************************/
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
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmReadDataOfPID21(
  P2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  PID21value                                                                              /* PRQA S 1330 */ /* MD_DEM_8.3_dcm */
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsObdSupportedInVariant() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_NullPointer(PID21value) == FALSE)                                                                            /* SBSW_DEM_POINTER_NULLPTR_CHECKING */  /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else  
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dem_Data_DestinationBuffer lDestinationBuffer;
    Dem_Data_InitDestinationBuffer(&lDestinationBuffer, &(PID21value[0]), 0x02);                                                 /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
    lReturnValue = Dem_GlobalDiagnostics_PidStatistics_ReadDataOfPID21(&lDestinationBuffer);                                     /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_DCMREADDATAOFPID21_APIID, Dem_Det_ErrorRegisterGet());
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON)  && (DEM_CFG_SUPPORT_PID21 == STD_ON) */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_PID30 == STD_ON)
/* ****************************************************************************
 % Dem_DcmReadDataOfPID30
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmReadDataOfPID30(
  P2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  PID30value                                                                              /* PRQA S 1330 */ /* MD_DEM_8.3_dcm */
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsObdSupportedInVariant() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_NullPointer(PID30value) == FALSE)                                                                       /* SBSW_DEM_POINTER_NULLPTR_CHECKING */  /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else  
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
    {
      *PID30value = Dem_Mem_GetWarmUpCountSinceClear();                                                                            /* SBSW_DEM_POINTER_WRITE_API */
      lReturnValue = E_OK;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_DCMREADDATAOFPID30_APIID, Dem_Det_ErrorRegisterGet());
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_PID30 == STD_ON) */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_PID31 == STD_ON)
/* ****************************************************************************
 % Dem_DcmReadDataOfPID31
 *****************************************************************************/
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
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmReadDataOfPID31(
  P2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  PID31value                                                                              /* PRQA S 1330 */ /* MD_DEM_8.3_dcm */
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsObdSupportedInVariant() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_NullPointer(PID31value) == FALSE)                                                                            /* SBSW_DEM_POINTER_NULLPTR_CHECKING */  /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else  
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dem_Data_DestinationBuffer lDestinationBuffer;
    Dem_Data_InitDestinationBuffer(&lDestinationBuffer, &(PID31value[0]), 0x02);                                                 /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
    lReturnValue = Dem_GlobalDiagnostics_PidStatistics_ReadDataOfPID31(&lDestinationBuffer);                                     /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_DCMREADDATAOFPID31_APIID, Dem_Det_ErrorRegisterGet());
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_PID31 == STD_ON) */


#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_PID4D == STD_ON)
/* ****************************************************************************
 % Dem_DcmReadDataOfPID4D
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmReadDataOfPID4D(
  P2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  PID4Dvalue                                                                              /* PRQA S 1330 */ /* MD_DEM_8.3_dcm */
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsObdSupportedInVariant() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_NullPointer(PID4Dvalue) == FALSE)                                                                            /* SBSW_DEM_POINTER_NULLPTR_CHECKING */  /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else  
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
    {
      uint16 lPid4D;

      lPid4D = Dem_Mem_GetEngineTimeMilOn();

      PID4Dvalue[0] = Dem_GetHiByte(lPid4D);                                                                                     /* SBSW_DEM_POINTER_WRITE_API */
      PID4Dvalue[1] = Dem_GetLoByte(lPid4D);                                                                                     /* SBSW_DEM_POINTER_WRITE_API */

      lReturnValue = E_OK;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_DCMREADDATAOFPID4D_APIID, Dem_Det_ErrorRegisterGet());
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_PID4D == STD_ON) */


#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_PID4E == STD_ON)
/* ****************************************************************************
 % Dem_DcmReadDataOfPID4E
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmReadDataOfPID4E(
  P2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  PID4Evalue                                                                              /* PRQA S 1330 */ /* MD_DEM_8.3_dcm */
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsObdSupportedInVariant() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_NullPointer(PID4Evalue) == FALSE)                                                                            /* SBSW_DEM_POINTER_NULLPTR_CHECKING */  /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else  
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
    {
      uint16 lPid4E;

      /* ----- Implementation ------------------------------------------------ */
      lPid4E = Dem_Mem_GetEngineTimeSinceClear();

      PID4Evalue[0] = Dem_GetHiByte(lPid4E);                                                                                     /* SBSW_DEM_POINTER_WRITE_API */
      PID4Evalue[1] = Dem_GetLoByte(lPid4E);                                                                                     /* SBSW_DEM_POINTER_WRITE_API */

      lReturnValue = E_OK;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_DCMREADDATAOFPID4E_APIID, Dem_Det_ErrorRegisterGet());
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_PID4E == STD_ON) */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_WWHOBD == STD_ON) && (DEM_CFG_SUPPORT_PID91 == STD_ON)
/* ****************************************************************************
 % Dem_DcmReadDataOfPID91
 *****************************************************************************/
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
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmReadDataOfPID91(
  P2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  PID91value                                                                              /* PRQA S 1330 */ /* MD_DEM_8.3_dcm */
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_NullPointer(PID91value) == FALSE)                                                                            /* SBSW_DEM_POINTER_NULLPTR_CHECKING */  /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else  
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    if(Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
    {
      PID91value[0] = Dem_GlobalDiagnostics_TranslateActivationMode(Dem_GlobalState_ActivationMode_GetMILStatus());              /* SBSW_DEM_POINTER_WRITE_API */
      
      PID91value[1] = 0x00; /* continuous MI counter is latched at 255 hours */                                                  /* SBSW_DEM_POINTER_WRITE_API */
      /* internal resolution of 1min/bit must be converted to 1h/bit */
      PID91value[2] = (uint8)(Dem_ActivationMode_ContinuousMiCounter_GetValue()/60u);                                            /* SBSW_DEM_POINTER_WRITE_API */
      
      PID91value[3] = 0x00U; /* B1 counter is latched at 255 hours */                                                            /* SBSW_DEM_POINTER_WRITE_API */
      /* internal resolution of 1min/bit must be converted to 1h/bit */
      PID91value[4] = (uint8)(Dem_ActivationMode_B1Counter_GetValue()/60u);                                                      /* SBSW_DEM_POINTER_WRITE_API */
      
      lReturnValue = E_OK;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_DCMREADDATAOFPID91_APIID, Dem_Det_ErrorRegisterGet());
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6080, 6050 */ /* MD_MSR_STMIF, MD_MSR_STCAL */
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_WWHOBD == STD_ON) && (DEM_CFG_SUPPORT_PID91 == STD_ON) */

                                                   /* Dcm <-> Dem : OBD PIDs */
/* ------------------------------------------------------------------------- */
#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_PID01 == STD_ON)
/* ****************************************************************************
 % Dem_DcmReadDataOfPID01
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmReadDataOfPID01(
  P2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  PID01value                                                                              /* PRQA S 1330 */ /* MD_DEM_8.3_dcm */
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsObdSupportedInVariant() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_NullPointer(PID01value) == FALSE)                                                                            /* SBSW_DEM_POINTER_NULLPTR_CHECKING */  /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else  
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    lReturnValue = Dem_GlobalDiagnostics_Readiness_ReadDataOfPID01(PID01value, DEM_READINESS_PID01_SIZE);                        /* SBSW_DEM_POINTER_FORWARD_API_ARGUMENT_BUFFER */
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_DCMREADDATAOFPID01_APIID, Dem_Det_ErrorRegisterGet());
  }

  return lReturnValue;
}
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_PID01 == STD_ON) */



                                                                  /* PID Data */
/* ------------------------------------------------------------------------- */
#if (DEM_CFG_SUPPORT_PID01 == STD_ON)
/* ****************************************************************************
 % Dem_ReadDataOfPID01
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_ReadDataOfPID01(
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  PID01value                                                                             /* PRQA S 1330 */ /* MD_DEM_8.3_rte */
)
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsObdSupportedInVariant() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_NullPointer(PID01value) == FALSE)                                                                            /* SBSW_DEM_POINTER_NULLPTR_CHECKING */  /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    lReturnValue = Dem_GlobalDiagnostics_Readiness_ReadDataOfPID01(PID01value, DEM_READINESS_PID01_SIZE);                        /* SBSW_DEM_POINTER_FORWARD_API_ARGUMENT_BUFFER */
  }
  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_READDATAOFPID01_APIID, Dem_Det_ErrorRegisterGet());
  }
  return lReturnValue;
}
#endif /* (DEM_CFG_SUPPORT_PID01 == STD_ON) */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_PID41 == STD_ON)
/* ****************************************************************************
 % Dem_DcmReadDataOfPID41
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmReadDataOfPID41(
  P2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  PID41value                                                                              /* PRQA S 1330 */ /* MD_DEM_8.3_dcm */
)
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsObdSupportedInVariant() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_NullPointer(PID41value) == FALSE)                                                                            /* SBSW_DEM_POINTER_NULLPTR_CHECKING */  /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    lReturnValue = Dem_GlobalDiagnostics_Readiness_ReadDataOfPID41(PID41value, DEM_READINESS_PID41_SIZE);                        /* SBSW_DEM_POINTER_FORWARD_API_ARGUMENT_BUFFER */
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_DCMREADDATAOFPID41_APIID, Dem_Det_ErrorRegisterGet());
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_PID41 == STD_ON) */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) 
/* ****************************************************************************
 % Dem_DcmReadDataOfPIDF501
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmReadDataOfPIDF501(
  P2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  PIDF501value                                                                              /* PRQA S 1330 */ /* MD_DEM_8.3_dcm */
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsObdOnUdsSupportedInVariant() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_NullPointer(PIDF501value) == FALSE)                                                                          /* SBSW_DEM_POINTER_NULLPTR_CHECKING */  /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else  
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    lReturnValue = Dem_GlobalDiagnostics_Readiness_ReadDataOfDIDF501(PIDF501value, DEM_READINESS_DIDF501_SIZE);                  /* SBSW_DEM_POINTER_FORWARD_API_ARGUMENT_BUFFER */
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_DCMREADDATAOFPIDF501_APIID, Dem_Det_ErrorRegisterGet());
  }

  return lReturnValue;
}
#endif

/* ****************************************************************************
 % Dem_GetIndicatorStatus
 *****************************************************************************/
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
FUNC(Std_ReturnType, DEM_CODE)
Dem_GetIndicatorStatus(
  uint8  IndicatorId,                                                                                                            /* PRQA S 1330, 3206 */ /* MD_DEM_8.3_rte, MD_DEM_3206 */
  P2VAR(Dem_IndicatorStatusType, AUTOMATIC, DEM_APPL_DATA)  IndicatorStatus                                                      /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13 */
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;
  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_GlobalState_Indicator_IsIndicatorIdValid(IndicatorId) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  if (Dem_APIChecks_NullPointer(IndicatorStatus) == FALSE)                                                                       /* SBSW_DEM_POINTER_NULLPTR_CHECKING */ /* PRQA S 0315 */ /* MD_DEM_Dir1.1 */
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_POINTER)
  }
  else
#endif
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */

  if ( (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
    && (Dem_GlobalState_Indicator_GetGlobalMilIndicatorId() == IndicatorId) )
  {
    *IndicatorStatus = Dem_GlobalState_ActivationMode_GetMILStatus();                                                    /* SBSW_DEM_POINTER_WRITE_API */
  }
  else
  {
    *IndicatorStatus = Dem_GlobalState_Indicator_GetIndicatorState(IndicatorId, 0, TRUE);                                    /* SBSW_DEM_POINTER_WRITE_API */
  }

    lReturnValue = E_OK;
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_GETINDICATORSTATUS_APIID, Dem_Det_ErrorRegisterGet());
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */


#if (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON) || (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
/* ****************************************************************************
 % Dem_SetPtoStatus
 *****************************************************************************/
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
FUNC(Std_ReturnType, DEM_CODE)
Dem_SetPtoStatus(
  boolean  PtoStatus
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;
  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_IsMasterPartition() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  if (Dem_APIChecks_IsBoolean(PtoStatus) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  if (Dem_APIChecks_IsObdSupportedInVariant() == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_WRONG_CONDITION)
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
    {
      lReturnValue = Dem_GlobalDiagnostics_Pto_SetPtoStatus(PtoStatus);
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_SETPTOSTATUS_APIID, Dem_Det_ErrorRegisterGet());
  }
  return lReturnValue;
}                                                                                                                                 /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  FILE SPECIFIC MISRA VIOLATIONS
 *********************************************************************************************************************/
/* PRQA L:DEM_API_IMPL */


#endif /* DEM_GLOBALDIAGNOSTICSIF_SVCIMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_GlobalDiagnosticsIF_SvcImplementation.h
 *********************************************************************************************************************/
