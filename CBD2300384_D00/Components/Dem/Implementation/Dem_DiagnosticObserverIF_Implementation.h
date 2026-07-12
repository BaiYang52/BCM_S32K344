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
/*! \addtogroup Dem_DiagnosticObserverIF
 *  \{
 *  \file       Dem_DiagnosticObserverIF_Implementation.h
 *  \brief      Implementation of unit DiagnosticObserverIF.
 *  \details    Realizes the public interfaces of its parent logical unit, i.e. both Dem external service APIs and Dem internal APIs.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_DIAGNOSTICOBSERVERIF_IMPLEMENTATION_H)
#define DEM_DIAGNOSTICOBSERVERIF_IMPLEMENTATION_H

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

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_DiagnosticObserverIF_Interface.h"

                                                    /* Used subcomponent API */
/* ------------------------------------------------------------------------- */
#include "Dem_Dtr_Interface.h"
#include "Dem_Iumpr_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Core_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_RecordReader_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_DIAGNOSTICOBSERVERIF_IMPLEMENTATION_FILENAME "Dem_DiagnosticObserverIF_Implementation.h"

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_GlobalStateIF_Private Private Methods
 * \{
 */


/* ****************************************************************************
 * Dem_DiagnosticObserver_TriggerPermanentStorage
 *****************************************************************************/
 /*!
  * \brief         Trigger permanent DTC storage
  *
  * \details       Check preconditions depending on active OBD legislation and 
  *                trigger permanent DTC storage
  *
 * \param[in]      EventContext
 *                 Event processing context
  *
  * \pre           -
  *
  * \context       TASK
  * \synchronous   TRUE
  * \reentrant     TRUE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DiagnosticObserver_TriggerPermanentStorage(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/


/* ****************************************************************************
 % Dem_DiagnosticObserver_TriggerPermanentStorage
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
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DiagnosticObserver_TriggerPermanentStorage(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
)
{
  if (Dem_Core_Event_UpdateTrigger_TestWIR(EventContext->EventEffects) == TRUE)
  {
    if (Dem_DTC_TestEventStoresPermanentDTC(EventContext->EventId) == TRUE)
    {
      if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
      {
        if (Dem_GlobalState_ActivationMode_IsEventAndActivationMode4Active(EventContext->EventId) == TRUE)
        {
          Dem_Data_PermanentEntryAdd(EventContext->EventId);
        }
      }
      else
      {
        /* Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE */
#if (DEM_CFG_SUPPORT_PERMANENTDTC_ACTIVATION_DISTANCE == STD_ON)
        if (Dem_Mem_GetPermanentActivationState() == TRUE)
#endif
        {
          Dem_Data_PermanentEntryAdd(EventContext->EventId);
        }
      }
    }
  }
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"    

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_DiagnosticObserverIF_Public
 * \{
 */

/* ****************************************************************************
 % Dem_DiagnosticObserver_ProcessOBDCycleRestart
 *****************************************************************************/
/*!
 * Internal comment removed. *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DiagnosticObserver_ProcessOBDCycleRestart(
  void
  )
{
  Dem_GlobalDiagnostics_Readiness_SetPid41DisabledMask(0);
}

/* ****************************************************************************
 % Dem_DiagnosticObserver_ProcessEventFailed
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DiagnosticObserver_ProcessEventFailed(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
)
{
  if (EventContext->EventEffects != DEM_CFG_TRIGGER_NONE)
  {
    Dem_DiagnosticObserver_TriggerPermanentStorage(EventContext);                                                                /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

    Dem_GlobalDiagnostics_ProcessEventFailed(EventContext);                                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }

  Dem_Statistics_UpdateGlobalFailureStatistics(EventContext);                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
}

/* ****************************************************************************
 % Dem_DiagnosticObserver_ProcessEventPassed
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DiagnosticObserver_ProcessEventPassed(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext,
  CONSTP2CONST(Dem_DTC_ContextType, AUTOMATIC, AUTOMATIC) DTCContext                                                           /* PRQA S 3673 */ /* MD_DEM_8.13_Internal */
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DTCContext);                                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */

  Dem_GlobalState_ActivationMode_ProcessEventPassed(EventContext);                                                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
 
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    if ((EventContext->EventEffects & DEM_CFG_TRIGGER_PASSED) != DEM_CFG_TRIGGER_NONE)
    {
# if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
      if (Dem_DTC_TestEventStoresPermanentDTC(DTCContext->EventId) == TRUE)
      {
        if ((Dem_DTC_IsEventCombinationEnabled() == FALSE)
          || ((Dem_UDSStatus_Test_TF(DTCContext->OldDtcStatus) == TRUE)
            && (Dem_UDSStatus_Test_TF(DTCContext->NewDtcStatus) == FALSE)))
        {
          uint8 lPermanentIndex;

          /* If the event has a permanent DTC */
          lPermanentIndex = Dem_Data_PermanentEntryFind(Dem_Cfg_GetMasterEvent(DTCContext->EventId));
          if (lPermanentIndex != Dem_Cfg_GlobalPermanentMaxCount())
          {
            uint8 lPermanentState;
            lPermanentState = Dem_Mem_PermanentGetStatus(lPermanentIndex);

            if (DEM_ESM_GET_PERMANENT_STATE(lPermanentState) == DEM_ESM_PERMANENT_ACTIVE)
            {
              Dem_Data_PermanentEntryRemove(lPermanentIndex);
              Dem_Memory_PermanentFillUp();
            }
          }
        }
      }
# endif
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_DiagnosticObserver_ProcessEventCleared
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DiagnosticObserver_ProcessEventCleared(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_UdsStatusByteType, AUTOMATIC) OldEventStatus
)
{

  Dem_Dtr_ResetDtrsOfEvent(EventId);

  if (Dem_UDSStatus_Test_TF(OldEventStatus) == TRUE)
  {
    Dem_GlobalState_ActivationMode_ProcessEventCleared(EventId);
  }

  if (Dem_UDSStatus_Test_WIR(OldEventStatus) == TRUE)
  {
    Dem_GlobalDiagnostics_GlobalState_ProcessEventWirDisabled(EventId); 
  }

}

/* ****************************************************************************
 % Dem_DiagnosticObserver_ProcessEventHealed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DiagnosticObserver_ProcessEventHealed(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
    Dem_GlobalDiagnostics_GlobalState_ProcessEventWirDisabled(EventId);
}

/* ****************************************************************************
 % Dem_DiagnosticObserver_ProcessDcyOfEventQualified
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DiagnosticObserver_ProcessDcyOfEventQualified(                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
  )
{
  Dem_GlobalDiagnostics_ProcessDcyOfEventQualified(EventContext);                                                                /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

  Dem_DiagnosticObserver_TriggerPermanentStorage(EventContext);                                                                  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
}

/* ****************************************************************************
 % Dem_DiagnosticObserver_ProcessOBDCycleEnd
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DiagnosticObserver_ProcessOBDCycleEnd(
  CONST(uint8, AUTOMATIC)  CycleId
)
{
  if (CycleId == Dem_Cfg_GlobalObdDrivingCycleId())
  {
# if (DEM_FEATURE_NEED_GLOBAL_MIL_STATE == STD_ON)
    /* Reset PID aging counter if at least one DTC triggers the MIL */
    if (Dem_GlobalState_Indicator_TestMilReportedActive() == TRUE)
    {
      Dem_Mem_SetPidAgingCounter(DEM_MEM_PID_AGING_COUNTER_THRESHOLD);
    }
# endif
    Dem_GlobalState_ActivationMode_ProcessDCYEnd();
  }

  if ((Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
    && (CycleId == Dem_Cfg_GlobalWarmupCycleId()))
  {
    Dem_GlobalState_ActivationMode_ProcessWUCEnd();
  }
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_DIAGNOSTICOBSERVERIF_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_DiagnosticObserverIF_Implementation.h
 *********************************************************************************************************************/
