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
/*! \addtogroup Dem_Esm
 *  \{
 *  \file       Dem_Esm_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_ESM_IMPLEMENTATION_H)
#define DEM_ESM_IMPLEMENTATION_H

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
/* PRQA S 0759 EOF */ /* MD_MSR_Union */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Esm_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_DiagnosticObserver_Interface.h"
#include "Dem_RecordReader_Interface.h"
#include "Dem_OperationCycle_Interface.h"
#include "Dem_Event_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_ESM_IMPLEMENTATION_FILENAME "Dem_Esm_Implementation.h"

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
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Esm_Public
 * \{
 */

#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
/* ****************************************************************************
 % Dem_Esm_AddAgingTime
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Esm_AddAgingTime(
  CONST(uint32, AUTOMATIC)  CurrentTime,
  CONST(uint16, AUTOMATIC)  DeltaValue
  )
{
  uint32 lTargetTime;

  lTargetTime = (uint32)(CurrentTime + DeltaValue);

  return lTargetTime;
}
#endif
                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (DEM_FEATURE_NEED_OBD == STD_ON) && (DEM_CFG_SUPPORT_RESTART_DCY_ON_CLEAR_DTC == STD_ON)
/* ****************************************************************************
 % Dem_Esm_PreOnClear_AllDtc
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Esm_PreOnClear_AllDtc(
  CONST(uint16, AUTOMATIC)  MemoryId
  )
{
  if (MemoryId == DEM_CFG_MEMORYID_PRIMARY)
  {
    Dem_OperationCycle_CycleUpdate(Dem_Cfg_GlobalObdDrivingCycleId(), DEM_OPERATIONCYCLE_CYCLE_RESTARTED);
  }
}
#endif

/* ****************************************************************************
 % Dem_Esm_PostOnClear_AllDtc
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
Dem_Esm_PostOnClear_AllDtc(
  CONST(uint16, AUTOMATIC)  MemoryId
  )
{
  Dem_Memory_ResetOverflow(MemoryId);

  if (MemoryId == DEM_CFG_MEMORYID_PRIMARY)
  {
    Dem_Statistics_SetFirstFailedEvent(DEM_EVENT_INVALID);
    Dem_Statistics_SetFirstConfirmedEvent(DEM_EVENT_INVALID);
    Dem_Statistics_SetMostRecentFailedEvent(DEM_EVENT_INVALID);
    Dem_Statistics_SetMostRecentConfirmedEvent(DEM_EVENT_INVALID);
    Dem_Dtr_ResetDtrsOfEvent(DEM_EVENT_INVALID);
    
    Dem_GlobalDiagnostics_ProcessAfterClearAll();
  }
}

/* ****************************************************************************
 % Dem_Esm_PostOnClear_Always
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Esm_PostOnClear_Always(
  CONST(uint16, AUTOMATIC)  MemoryId                                                                                              /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
    if (MemoryId == DEM_CFG_MEMORYID_PRIMARY)
    {
      Dem_Iumpr_ResetPendingFids();

#if (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON)
      if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
      {
        /* >>>> -------------------------------- Enter Critical Section: StateManager */
        Dem_EnterCritical_StateManager();
        Dem_OperationCycle_SetObdCycleStates(DEM_OPERATIONCYCLE_RESET_PFC_CYCLE(Dem_OperationCycle_GetObdCycleStates()));
        Dem_LeaveCritical_StateManager();
        /* <<<< -------------------------------- Leave Critical Section: StateManager */
      }

#endif

      Dem_GlobalDiagnostics_ProcessAfterClear();
    }
  }

  /* copy debounce values from debounce array to nvram mirror */
  Dem_Mem_CopyDataDebounceNv(DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);

  Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_ADMIN),
    DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);

#if (DEM_FEATURE_NEED_AGING_DATA == STD_ON)
  Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_AGINGDATA),
    DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_ESM_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Esm_Implementation.h
 *********************************************************************************************************************/
