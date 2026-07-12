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
/*! \addtogroup Dem_GlobalDiagnosticsIF
 *  \{
 *  \file       Dem_GlobalDiagnosticsIF_Implementation.h
 *  \brief      Implementation of unit GlobalDiagnosticsIF.
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

#if !defined (DEM_GLOBALDIAGNOSTICSIF_IMPLEMENTATION_H)
#define DEM_GLOBALDIAGNOSTICSIF_IMPLEMENTATION_H

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
#include "Dem_GlobalDiagnosticsIF_Interface.h"

                                                    /* Used subcomponent API */
/* ------------------------------------------------------------------------- */
#include "Dem_Readiness_Interface.h"
#include "Dem_Conditions_Interface.h"
#include "Dem_GlobalState_Interface.h"
#include "Dem_Pto_Interface.h"
#include "Dem_PidStatistics_Interface.h"

                                                   /* Subcomponents callbacks*/
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_GLOBALDIAGNOSTICSIF_IMPLEMENTATION_FILENAME "Dem_GlobalDiagnosticsIF_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_GlobalDiagnosticsIF_Public
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_PreInit
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_PreInit(
  void
)
{
  Dem_GlobalDiagnostics_Conditions_PreInit();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_InitPreCore
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_InitPreCore(
  void
)
{
  Dem_GlobalDiagnostics_Conditions_Init();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_InitPostCore
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_InitPostCore(
  void
)
{
  Dem_GlobalDiagnostics_GlobalState_Init();
  Dem_GlobalDiagnostics_Pto_Init();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessDcyOfEventQualified
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
Dem_GlobalDiagnostics_ProcessDcyOfEventQualified(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
  )
{
  if (Dem_Core_Event_UpdateTrigger_TestConfirmed(EventContext->EventEffects) == TRUE)
  {
    Dem_Readiness_Completion_UpdateCompletionStatus(Dem_Core_Event_GetEventReadinessGroup(EventContext->EventId), TRUE);

    if (Dem_UDSStatus_Test_TF(EventContext->NewEventStatus) == TRUE)
    {
      Dem_GlobalState_ActivationMode_ProcessQualifiedActiveEvent(EventContext->EventId);
    }
  }

  if (Dem_Core_Event_UpdateTrigger_TestWIR(EventContext->EventEffects) == TRUE)
  { 
    /* Process the indicators */
    Dem_GlobalDiagnostics_GlobalState_ProcessEventWirEnabled(EventContext->EventId);
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessEventFailed
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventFailed(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
  )
{
  if (Dem_Core_Event_UpdateTrigger_TestWIR(EventContext->EventEffects) == TRUE)
  {
    /* Suppress the Indicators if the driving cycle is not qualified yet */
    if ((Dem_Cfg_IsObdSupportedInVariant() == FALSE)
      || (Dem_EventInternalStatus_Test_QualifyStatus_WIR(EventContext->QualifyStatus) == TRUE))
    {
      /* Process the indicators */
      Dem_GlobalDiagnostics_GlobalState_ProcessEventWirEnabled(EventContext->EventId);
    }
  }

  /* increment counter only if event initially becomes confirmed or
     if it is still confirmed and test failed changes from 0 to 1 */
  if (((Dem_Core_Event_UpdateTrigger_TestConfirmed(EventContext->EventEffects) == TRUE)
    && (Dem_EventInternalStatus_Test_QualifyStatus_CDTC(EventContext->QualifyStatus) == TRUE))
    || ((Dem_Core_Event_UpdateTrigger_TestFailed(EventContext->EventEffects) == TRUE)
      && (Dem_UDSStatus_Test_CDTC(EventContext->OldExternalEventStatus) == TRUE)))
  {
    Dem_GlobalState_ActivationMode_ProcessQualifiedActiveEvent(EventContext->EventId);
  }

  Dem_ActivationMode_B1Counter_ProcessMalfunctionDetected(EventContext->EventId);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessAfterClear
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
Dem_GlobalDiagnostics_ProcessAfterClear(
  void
  )
{
#if (DEM_CFG_SUPPORT_PID30 == STD_ON)
  /* Clear WarmUpCycle since last clear counter */
  Dem_Mem_SetWarmUpCountSinceClear(0x00U);
#endif
#if (DEM_FEATURE_NEED_PID31 == STD_ON)
  /* >>>> -------------------------------- Enter Critical Section: NonAtomic32bit */
  Dem_EnterCritical_NonAtomic32bit();

  /* Set current Odometer value to invalid */
  Dem_Mem_SetOdometerAtClear(DEM_MEM_SET_ODOMETER_INVALID(0u));
  Dem_LeaveCritical_NonAtomic32bit();
  /* <<<< -------------------------------- Leave Critical Section: NonAtomic32bit */
#endif
#if (DEM_FEATURE_NEED_PID4E == STD_ON)
  Dem_Mem_SetEngineTimeSinceClear(0x0000U);
#endif

  Dem_Readiness_Completion_UpdateCompletionStatesAfterClear();
  Dem_GlobalDiagnostics_GlobalState_ProcessAfterClear();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessAfterClearAll
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessAfterClearAll(
  void
  )
{
  Dem_GlobalState_ActivationMode_ProcessAfterClearAll();
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_GLOBALDIAGNOSTICSIF_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_GlobalDiagnosticsIF_Implementation.h
 *********************************************************************************************************************/
