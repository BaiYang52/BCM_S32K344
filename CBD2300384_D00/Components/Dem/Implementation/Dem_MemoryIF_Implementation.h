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
/*! \addtogroup Dem_MemoryIF
 *  \{
 *  \file       Dem_MemoryIF_Implementation.h
 *  \brief      Abstraction for multiple event memories.
 *  \details    Implementation of MemoryIF subcomponent which is facade for all memory related tasks.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MEMORYIF_IMPLEMENTATION_H)
#define DEM_MEMORYIF_IMPLEMENTATION_H

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
#include "Dem_MemoryIF_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/ - units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_TimeSeries_Interface.h"
#include "Dem_Mem_Interface.h"
#include "Dem_MemAccess_Interface.h"
#include "Dem_MemObdFreezeFrame_Interface.h"
#include "Dem_MemoryRestoration_Interface.h"
#include "Dem_MemPermanent_Interface.h"
#include "Dem_MemState_Interface.h"
#include "Dem_MemStorageManager_Interface.h"
#include "Dem_UserDefinedMemory_Interface.h"

#include "Dem_Client_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_Satellite_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_DiagnosticObserver_Interface.h"
#include "Dem_MemoryEntry_Interface.h"
#include "Dem_Prestore_Interface.h"


/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_MEMORYIF_IMPLEMENTATION_FILENAME "Dem_MemoryIF_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

#define DEM_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! NvM requested initialization of admin data */
DEM_LOCAL VAR(boolean, DEM_VAR_NOINIT) Dem_Memory_InitAdminDataRequested;

#define DEM_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
/*!
 * \defgroup Dem_Memory_PrivateProperties Private Properties
 * \{
 */
#define DEM_START_SEC_CODE
#include "MemMap.h"
/* ****************************************************************************
 * Dem_Memory_GetInitAdminDataRequested
 *****************************************************************************/
/*!
 * \brief         Returns if admin data was initialized or not
 *
 * \details       Returns if admin data was initialized or not
 *
 * \return        TRUE: Admin data was initialized by NvM
 * \return        FALSE: Otherwise
 *
 * \pre           NvM must have restored NV mirrors.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_GetInitAdminDataRequested(
  void
  );

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/


/* ****************************************************************************
 % Dem_Memory_GetInitAdminDataRequested
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_GetInitAdminDataRequested(
  void
  )
{
  return Dem_Memory_InitAdminDataRequested;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Memory_PublicProperties
 * \{
 */



/* ****************************************************************************
 % Dem_Memory_SetInitAdminDataRequested
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_SetInitAdminDataRequested(
  CONST(boolean, AUTOMATIC)  InitRequested
  )
{
  Dem_Memory_InitAdminDataRequested = InitRequested;
}

#define DEM_STOP_SEC_CODE
#include "MemMap.h"

/*!
 * \}
 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Memory_Private Private Methods
 * \{
 */


/* ****************************************************************************
 * Dem_Memory_Init_RestoreMemory
 *****************************************************************************/
/*!
 * \brief         Test and restore corrupted primary/user defined memory.
 *
 * \details       This function verifies the integrity of memory entries and
 *                links them up into the runtime control structures.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of the Event memory.
 *
 * \pre           NvM must have restored NV mirrors.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_Init_RestoreMemory(
  uint16 TranslatedMemoryId
  );

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Memory_Init_RestoreMemory
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_Init_RestoreMemory(
  uint16 TranslatedMemoryId
  )
{ /* No reinitialization - restore primary stack */
  Dem_Memory_MemEntryIterType MemEntryIter;

  Dem_MemState_MemorySetCurrentSize(TranslatedMemoryId, 0);

  /* Iterate through all memory entries. The iteration order must be identical to Dem_MemAccess_MemoryFindIndex. */
  for (Dem_Memory_MemEntry_IterInit(TranslatedMemoryId, &MemEntryIter);                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Memory_MemEntry_IterExists(&MemEntryIter) == TRUE;                                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Memory_MemEntry_IterNext(&MemEntryIter))                                                                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_MemoryIndexType lMemoryIndex;
    Dem_Cfg_MemoryEntryHandleType lMemoryEntryId;
    Dem_EventIdType lEventId;

    lMemoryIndex = Dem_Memory_MemEntry_IterGetMemoryIndex(&MemEntryIter);                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */

    /* Check and fix problems with internal corruption of the memory entry record: */
    if (Dem_MemoryRestoration_FixCorruptMemoryEntry(TranslatedMemoryId, lMemoryIndex) == TRUE)
    { /* Continue with next memory entry */
      continue;                                                                                                                  
    }

    /* If the memory entry is allocated to an event that already has a memory entry: */
    if (Dem_MemoryRestoration_FixDoubleAllocation(TranslatedMemoryId, lMemoryIndex) == TRUE)
    { /* Continue with next memory entry */
      continue;                                                                                                                  
    }

    lMemoryEntryId = Dem_MemoryEntry_GetId(lMemoryIndex);
    lEventId = Dem_MemoryEntry_GetEventId(lMemoryEntryId);
    if (lEventId != DEM_EVENT_INVALID)
    {
      uint32 lTimestamp;
      lTimestamp = Dem_MemoryEntry_GetTimestamp(lMemoryEntryId);

      Dem_StatusIndicator_UpdateOnMemoryInitRestoreMemory(lEventId, lMemoryIndex, lMemoryEntryId);

      Dem_MemoryRestoration_RestoreEventStatus(lMemoryEntryId, lMemoryIndex, lEventId);
      Dem_MemoryRestoration_RestoreTripCountFromCFCC(lMemoryEntryId, lEventId);

      Dem_Event_InitUdsFromStatusIndicator(lEventId, lMemoryEntryId);

      /* Synchronize the global timestamp with the timestamp stored in the memory entry */
      if (lTimestamp > Dem_MemState_GetCurrentTimestamp())
      {
        Dem_MemState_SetCurrentTimestamp(lTimestamp);
      }

      /* Sort the memory entry into the chronological list */
      Dem_MemState_SortEntryIntoChronologicalList(TranslatedMemoryId, lMemoryIndex, lTimestamp);

      /* Update the number of occupied memory entries */
      Dem_MemState_MemorySetCurrentSize(TranslatedMemoryId,
        (uint8)(Dem_MemState_MemoryGetCurrentSize(TranslatedMemoryId) + 1u));
    }
  }

  /* Update the global timestamp */
  Dem_MemState_SetCurrentTimestamp(Dem_MemState_GetCurrentTimestamp() + 1u);;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Memory_Public
 * \{
 */

/* ****************************************************************************
 % Dem_FaultMemory_Memory_TestDTCOriginValid
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
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_FaultMemory_Memory_TestDTCOriginValid(
 Dem_DTCOriginType  DTCOrigin
 )
{
  boolean lDTCOriginValid;
  lDTCOriginValid = FALSE;

  if ((DTCOrigin != 0u) && (DTCOrigin < DEM_DTC_ORIGIN_INVALID))
  {
    lDTCOriginValid = TRUE;
  }
  else if (Dem_Memory_UserDefinedMemory_isUserDefinedMemoryValid(DTCOrigin) == TRUE)
  {
    lDTCOriginValid = TRUE;
  }
  else
  {
    /* MISRA */
  }

  return lDTCOriginValid;
}

/* ****************************************************************************
 % Dem_FaultMemory_Memory_TranslateDTCOrigin
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
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_FaultMemory_Memory_TranslateDTCOrigin(
  Dem_DTCOriginType  DTCOrigin,
  CONSTP2VAR(uint16, AUTOMATIC, AUTOMATIC)  TranslatedMemoryId
  )
{
  Std_ReturnType lTranslationStatus;
  lTranslationStatus = E_NOT_OK;

  switch (DTCOrigin)
  {
  case DEM_DTC_ORIGIN_PRIMARY_MEMORY:
    *TranslatedMemoryId = DEM_CFG_MEMORYID_PRIMARY;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    lTranslationStatus = E_OK;
    break;

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  case DEM_DTC_ORIGIN_PERMANENT_MEMORY:
    if (Dem_Cfg_IsPermanentSupportedInVariant() == TRUE)
    {
      *TranslatedMemoryId = DEM_CFG_MEMORYID_PERMANENT;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      lTranslationStatus = E_OK;
    }
    break;
#endif

  default:
    if ((Dem_Memory_UserDefinedMemory_isUserDefinedMemoryEnabled() == TRUE)
        && (Dem_Memory_UserDefinedMemory_isUserDefinedMemoryValid(DTCOrigin) == TRUE))
    {
      uint16 lTranslatedUserDefinedMemoryId;
      lTranslatedUserDefinedMemoryId = Dem_Memory_UserDefinedMemory_TranslateDTCOrigin(DTCOrigin);
      if (lTranslatedUserDefinedMemoryId != DEM_CFG_MEMORYID_PRIMARY)
      {
        *TranslatedMemoryId = lTranslatedUserDefinedMemoryId;                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        lTranslationStatus = E_OK;
      }
    }
    break;
  }

    return lTranslationStatus;
}

/* ****************************************************************************
 % Dem_FaultMemory_Memory_IsEventDestination
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FaultMemory_Memory_IsEventDestination(
  uint16 TranslatedMemoryId
  )
{
  boolean lEventDestination;
  lEventDestination = FALSE;
  
  if ((TranslatedMemoryId == DEM_CFG_MEMORYID_PRIMARY)
    || (TranslatedMemoryId <= Dem_Memory_UserDefinedMemory_GetNumberOfUserDefinedMemories()))
  {
    lEventDestination = TRUE;
  }

  return lEventDestination;
}

/* ****************************************************************************
 % Dem_FaultMemory_Memory_TestEntryforEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FaultMemory_Memory_TestEntryforEvent(
  uint16 TranslatedMemoryId,
  Dem_Cfg_MemoryIndexType MemoryIndex,
  Dem_EventIdType EventId
  )
{
  boolean lRetVal;
  lRetVal = FALSE;

  if (Dem_FaultMemory_Memory_IsEventDestination(TranslatedMemoryId) == TRUE)
  {
    lRetVal = Dem_MemoryEntry_TestEntryForEvent(MemoryIndex, EventId);
  }
  else 
  if (TranslatedMemoryId == DEM_CFG_MEMORYID_TIMESERIES)
  {
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) 
    lRetVal = Dem_Memory_TimeSeries_TestEntryForEvent(MemoryIndex, EventId);
#endif
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
  
  return lRetVal;
}

/* ****************************************************************************
 % Dem_FaultMemory_Memory_FreeEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_FaultMemory_Memory_FreeEntry(
  uint16 TranslatedMemoryId,
  Dem_Cfg_MemoryIndexType MemoryIndex
  )
{
  if (Dem_FaultMemory_Memory_IsEventDestination(TranslatedMemoryId) == TRUE)
  {
    Dem_MemoryEntry_FreeEntry(MemoryIndex);
  }
  else
  if (TranslatedMemoryId == DEM_CFG_MEMORYID_TIMESERIES)
  {
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) 
    Dem_Memory_TimeSeries_FreeEntry(MemoryIndex);
#endif
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
}

/* ****************************************************************************
 % Dem_Memory_FindIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Memory_FindIndex(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  return Dem_MemAccess_MemoryFindIndex(Dem_Cfg_EventDestination(EventId), EventId);
}

/* ****************************************************************************
 % Dem_Memory_FindTimeSeriesIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Memory_FindTimeSeriesIndex(
  Dem_EventIdType EventId
  )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  return Dem_MemAccess_MemoryFindIndex(DEM_CFG_MEMORYID_TIMESERIES, EventId);
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return DEM_MEM_INVALID_MEMORY_INDEX;
#endif
}


/* ****************************************************************************
 % Dem_Memory_FindMemoryIndex_StoredEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Memory_FindMemoryIndex_StoredEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  return Dem_MemAccess_FindMemoryIndex_StoredEvent(Dem_Cfg_EventDestination(EventId), EventId);
}

/* ****************************************************************************
 % Dem_Memory_FreeEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_FreeEvent(
  CONST(Dem_Cfg_MemoryIndexType, AUTOMATIC)  MemoryIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_MemAccess_MemoryFreeIndex(Dem_Cfg_EventDestination(EventId), MemoryIndex);
}

/* ****************************************************************************
 % Dem_Memory_GetFirstMemoryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Memory_GetFirstMemoryIndex(                                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint16  TranslatedMemoryId
  )
{
  return Dem_MemState_MemoryGetFirstIndex(TranslatedMemoryId);
}

/* ****************************************************************************
 % Dem_Memory_AllocateIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Memory_AllocateIndex(
CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  return Dem_MemAccess_MemoryAllocateIndex(Dem_Cfg_EventDestination(EventId), EventId);
}

#if ((DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON) && (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF))
/* ****************************************************************************
 % Dem_Memory_AllocateIndexAging
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Memory_AllocateIndexAging(
CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  Dem_Cfg_MemoryIndexType lMemoryIndex;
  lMemoryIndex = Dem_MemAccess_MemoryAllocateAging(Dem_Cfg_EventDestination(EventId), EventId);
  return lMemoryIndex;
}
#endif

#if (DEM_FEATURE_NEED_OVFLIND == STD_ON)
/* ****************************************************************************
 % Dem_FaultMemory_Memory_GetOverflow
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FaultMemory_Memory_GetOverflow(
  CONST(Dem_DTCOriginType, AUTOMATIC)  DTCOrigin,
  P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA)  OverflowIndication
  )
{
  Std_ReturnType lReturnValue;
  uint16 lTranslatedMemoryId;

  if (Dem_FaultMemory_Memory_TranslateDTCOrigin(DTCOrigin, &lTranslatedMemoryId) == E_OK)                                        /* SBSW_DEM_POINTER_FORWARD_STACK */
  {
    *OverflowIndication = Dem_Memory_MemState_GetOverflow(lTranslatedMemoryId);                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    lReturnValue = E_OK;
  }
  else
  {
    lReturnValue = E_NOT_OK;
  }

  return lReturnValue;
}
#endif

/* ****************************************************************************
 % Dem_Memory_AllocMemEntriesChrono_IterInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_AllocMemEntriesChrono_IterInit(                                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint16, AUTOMATIC)  TranslatedMemoryId,
  CONSTP2VAR(Dem_Memory_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  Dem_Internal_AssertReturnVoid(TranslatedMemoryId != DEM_CFG_MEMORYID_PERMANENT, DEM_E_INCONSISTENT_STATE)

  IterPtr->mIdx = 0u;                                                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_MemState_MemoryGetCurrentSize(TranslatedMemoryId);                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mTranslatedMemoryId = TranslatedMemoryId;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_Memory_AllocMemEntriesChrono_IterResume
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_AllocMemEntriesChrono_IterResume(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint16, AUTOMATIC)  TranslatedMemoryId,
  CONST(Dem_Cfg_MemoryIndexType, AUTOMATIC)  StoredChronoIter,
  CONSTP2VAR(Dem_Memory_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  Dem_Internal_AssertReturnVoid(TranslatedMemoryId != DEM_CFG_MEMORYID_PERMANENT, DEM_E_INCONSISTENT_STATE)
  IterPtr->mIdx = StoredChronoIter;                                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_MemState_MemoryGetCurrentSize(TranslatedMemoryId);                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mTranslatedMemoryId = TranslatedMemoryId;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_Memory_AllocMemEntriesChrono_IterNext
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_AllocMemEntriesChrono_IterNext(                                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2VAR(Dem_Memory_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  IterPtr->mIdx += 1u;                                                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_Memory_AllocMemEntriesChrono_IterExists
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_AllocMemEntriesChrono_IterExists(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(Dem_Memory_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  return (boolean)(IterPtr->mIdx < IterPtr->mEndIdx);
}


/* ****************************************************************************
 % Dem_Memory_AllocMemEntriesChrono_IterGetMemoryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Memory_AllocMemEntriesChrono_IterGetMemoryIndex(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(Dem_Memory_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  return Dem_MemState_MemoryGetChronology(IterPtr->mTranslatedMemoryId,
         (uint8)((IterPtr->mEndIdx - 1u) - IterPtr->mIdx ));
}

/* ****************************************************************************
 % Dem_Memory_AllocMemEntriesChrono_IterGet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Memory_AllocMemEntriesChrono_IterGet(                                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(Dem_Memory_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  return (Dem_Cfg_MemoryIndexType)IterPtr->mIdx;
}

/* ****************************************************************************
 % Dem_Memory_AllEventMemEntries_IterInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_AllEventMemEntries_IterInit(                                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2VAR(Dem_Memory_AllEventMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  IterPtr->mIdx = Dem_MemState_MemoryGetFirstIndex(DEM_CFG_MEMORYID_PRIMARY);                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  /*Assign number of stored events*/
  IterPtr->mEndIdx = Dem_MemState_GetFirstInvalidEventBlockIndex();                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_Memory_AllEventMemEntries_IterNext
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_AllEventMemEntries_IterNext(                                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2VAR(Dem_Memory_AllEventMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  IterPtr->mIdx += 1u;                                                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_Memory_AllEventMemEntries_IterExists
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_AllEventMemEntries_IterExists(                                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(Dem_Memory_AllEventMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  return (boolean)(IterPtr->mIdx < IterPtr->mEndIdx);
}


/* ****************************************************************************
 % Dem_Memory_AllEventMemEntries_IterGetMemoryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Memory_AllEventMemEntries_IterGetMemoryIndex(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(Dem_Memory_AllEventMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  return (Dem_Cfg_MemoryIndexType)IterPtr->mIdx;
}


/* ****************************************************************************
 % Dem_Memory_MemEntry_IterInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_MemEntry_IterInit(                                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint16 TranslatedMemoryId,
  CONSTP2VAR(Dem_Memory_MemEntryIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  IterPtr->mIdx = Dem_MemState_MemoryGetFirstIndex(TranslatedMemoryId);                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_MemState_MemoryGetEndIndex(TranslatedMemoryId);                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_Memory_MemEntry_IterResume_FromIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_MemEntry_IterResume_FromIndex(                                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint16  TranslatedMemoryId,
  CONST(Dem_Cfg_MemoryIndexType, AUTOMATIC) MemoryIndex,
  CONSTP2VAR(Dem_Memory_MemEntryIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  IterPtr->mIdx = MemoryIndex;                                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_MemState_MemoryGetEndIndex(TranslatedMemoryId);                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */ 
}

/* ****************************************************************************
 % Dem_Memory_MemEntry_IterResume_FromEntryId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_MemEntry_IterResume_FromEntryId(                                                                                      /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
uint16  TranslatedMemoryId,
CONST(Dem_Cfg_MemoryEntryHandleType, AUTOMATIC) MemoryEntryId,
CONSTP2VAR(Dem_Memory_MemEntryIterType, AUTOMATIC, AUTOMATIC) IterPtr
)
{
  IterPtr->mIdx = Dem_MemState_MemoryGetFirstIndex(TranslatedMemoryId) + MemoryEntryId;                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_MemState_MemoryGetEndIndex(TranslatedMemoryId);                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}


/* ****************************************************************************
 % Dem_Memory_MemEntry_IterNext
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_MemEntry_IterNext(                                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2VAR(Dem_Memory_MemEntryIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  IterPtr->mIdx += 1u;                                                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_Memory_MemEntry_IterExists
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_MemEntry_IterExists(                                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(Dem_Memory_MemEntryIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  return (boolean)(IterPtr->mIdx < IterPtr->mEndIdx);
}


/* ****************************************************************************
 % Dem_Memory_MemEntry_IterGetMemoryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Memory_MemEntry_IterGetMemoryIndex(                                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(Dem_Memory_MemEntryIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  return (Dem_Cfg_MemoryIndexType)IterPtr->mIdx;
}


/* ****************************************************************************
 % Dem_Memory_MemEntry_IterGetEntryId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryEntryHandleType, DEM_CODE)
Dem_Memory_MemEntry_IterGetEntryId(                                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(Dem_Memory_MemEntryIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  return Dem_MemoryEntry_GetId((Dem_Cfg_MemoryIndexType)IterPtr->mIdx);
}


/* ****************************************************************************
 % Dem_Memory_GetNumAllocatedMemoryEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memory_GetNumAllocatedMemoryEntry(                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint16  TranslatedMemoryId
  )
{
  return Dem_MemState_MemoryGetCurrentSize(TranslatedMemoryId);
}

/* ****************************************************************************
 % Dem_Memory_ResetOverflow
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_ResetOverflow(
  CONST(uint16, AUTOMATIC)  MemoryId
  )
{
  Dem_Memory_MemState_ResetOverflow(MemoryId);
}

/* ****************************************************************************
 % Dem_Memory_UpdateChronology
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_UpdateChronology(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_Cfg_MemoryIndexType, AUTOMATIC)  MemoryIndex
  )
{
  Dem_MemState_MemoryUpdateIndex(Dem_Cfg_EventDestination(EventId), MemoryIndex);
}

/* ****************************************************************************
 % Dem_Memory_TriggerWWHOBDFreezeFrameStorage
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memory_TriggerWWHOBDFreezeFrameStorage(
CONST(Dem_EventIdType, AUTOMATIC)  EventId,
CONST(Dem_Cfg_StorageTriggerType, AUTOMATIC)  Effects,
CONST(Dem_Cfg_MemoryIndexType, AUTOMATIC)  MemoryIndex
)
{
  uint8 lReturnValue;

  lReturnValue = DEM_DATA_NO_CHANGES;
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    lReturnValue = Dem_MemStorageManager_TriggerWwhObdFreezeFrameStorage(EventId, Effects, MemoryIndex);
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(MemoryIndex)                                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Effects)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Memory_TriggerObdFreezeFrameStorage
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_TriggerObdFreezeFrameStorage(
  CONSTP2VAR(Dem_DTC_ContextType, AUTOMATIC, AUTOMATIC) DTCContext                                                               /* PRQA S 3673 */ /* MD_DEM_8.13_Internal */
)
{
  if (Dem_Cfg_IsObdOnUdsSupportedInVariant() == TRUE)
  {
    Dem_FaultMemory_MemObdFreezeFrame_TriggerObdOnUdsFreezeFrameUpdate(DTCContext);                                              /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
  else if (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)
  {
    Dem_MemStorageManager_TriggerObdIIFreezeFrameUpdate(DTCContext);                                                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
  else
  {
    /* MISRA Case */
  }
  
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DTCContext)                                                                                   /* PRQA S 3112 */ /* MD_DEM_14.2 */

}

#if ((DEM_CFG_SUPPORT_TRIGGER_PASSED == STD_ON) && (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF))
/* ****************************************************************************
 % Dem_Memory_UpdateEventEntryOnPassed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memory_UpdateEventEntryOnPassed(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,                                                                                    /* PRQA S 3206 */ /* MD_DEM_3206 */
  CONST(Dem_Cfg_StorageTriggerType, AUTOMATIC)  UpdateFlags,
  CONST(Dem_Cfg_MemoryIndexType, AUTOMATIC)  MemoryIndex
  )
{
  return Dem_MemStorageManager_UpdateEventEntryOnPassed(EventId, UpdateFlags, MemoryIndex);
}
#endif

/* ****************************************************************************
 % Dem_Memory_UpdateEventEntryOnFailed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memory_UpdateEventEntryOnFailed(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,                                                                                    /* PRQA S 3206 */ /* MD_DEM_3206 */
  CONST(Dem_Cfg_StorageTriggerType, AUTOMATIC)  UpdateFlags,
  CONST(Dem_Cfg_MemoryIndexType, AUTOMATIC)  MemoryIndex
  )
{
  return Dem_MemStorageManager_UpdateEventEntryOnFailed(EventId, UpdateFlags, MemoryIndex);
}

/* ****************************************************************************
 % Dem_Memory_TestEventEntryUpdatable
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_TestEventEntryUpdatable(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_Cfg_MemoryIndexType, AUTOMATIC)  MemoryIndex
  )
{
  boolean  lUpdateable;
  Dem_Cfg_MemoryEntryHandleType lMemoryEntryId;

  lMemoryEntryId = Dem_MemoryEntry_GetId(MemoryIndex);
  if (
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
# if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON)
     (Dem_DTCInternalStatus_GetStoredStatus(Dem_DTC_GetInternalStatus(EventId)) == Dem_DTCInternalStatus_StoredStatus_Aged)
      ||
# endif
#endif
      (Dem_MemoryEntry_TestAgingOnlyEntry(lMemoryEntryId) == TRUE)                                                               /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
     )
  {
    lUpdateable = FALSE;
  }
  else
  {
    lUpdateable = TRUE;
  }
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return lUpdateable;
}

/* ****************************************************************************
 % Dem_Memory_UpdateEventsCycleCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_UpdateEventsCycleCounter(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  CycleStatus
)
{
#if ( (DEM_CFG_DATA_CYCLES_TESTED_SINCE_FIRST_FAILED == STD_ON) \
   || (DEM_CFG_DATA_CONSECUTIVE_FAILED_CYCLES == STD_ON) \
   || ((DEM_CFG_DATA_FAULT_PENDING_COUNTER == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_OFF)))
  boolean lBlockModification;
  Dem_Cfg_MemoryIndexType lMemoryIndex;
  Dem_Cfg_MemoryEntryHandleType lMemoryEntryId;

  lMemoryIndex = Dem_Memory_FindMemoryIndex_StoredEvent(EventId);
  lMemoryEntryId = Dem_MemoryEntry_GetId(lMemoryIndex);

  lBlockModification = Dem_MemStorageManager_UpdateCycleCounter(EventId, lMemoryEntryId, CycleStatus);
  if (lBlockModification == TRUE)
  {
    Dem_Nvm_SetEventMemoryBlockState(lMemoryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
  }
#else
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                    /* PRQA S 3112 */ /* MD_DEM_14.2 */
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(CycleStatus)                                                                                /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}

#if (DEM_FEATURE_NEED_CYCLE_COUNTERS == STD_ON)
/* ****************************************************************************
 % Dem_Memory_LatchCycleCounters
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_LatchCycleCounters(
  CONST(Dem_Cfg_MemoryIndexType, AUTOMATIC)  MemoryIndex,
  CONST(uint16, AUTOMATIC)  CycleCounter
)
{
  boolean lDirty;
  lDirty = Dem_MemStorageManager_TestCycleCounterLatched(Dem_MemoryEntry_GetId(MemoryIndex), CycleCounter);
  if (lDirty == TRUE)
  {
    Dem_Nvm_SetEventMemoryBlockState(MemoryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
  }
}
#endif

/* ****************************************************************************
 % Dem_Memory_GetFirstEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Memory_GetFirstEvent(
  CONST(uint16, AUTOMATIC)  TranslatedMemoryId
  )
{
  return Dem_MemState_MemoryGetFirstEvent(TranslatedMemoryId);
}

/* ****************************************************************************
 % Dem_Memory_GetLastEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Memory_GetLastEvent(
  CONST(uint16, AUTOMATIC)  TranslatedMemoryId
  )
{
  return Dem_MemState_MemoryGetLastEvent(TranslatedMemoryId);
}

/* ****************************************************************************
 % Dem_Memory_Init
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_Init(
  void
  )
{
  boolean lResetStoredData;
  Dem_EventIdType lEventId;
  uint16 lDebounceNvIndex = 0u;


  for (lEventId = DEM_EVENT_FIRST; lEventId < Dem_Cfg_GlobalEventCount(); ++lEventId)
  { /* Only reset stored status, since debouncing can have changed since pre-initialization */
    Dem_Event_SetInternalStatus(lEventId,
      Dem_DTCInternalStatus_SetStoredStatus(Dem_Event_GetInternalStatus(lEventId), Dem_DTCInternalStatus_StoredStatus_None));

    /* If NV storage of debounce values is configured, and the event uses that feature */
    /* It's safe to do it here since supporting events are not allowed to report before Dem_Init() */
    if (Dem_Event_TestDebounceCounterStoredInNv(lEventId) == TRUE)
    {
      sint16 lDebounceValue;
      boolean lFailedThresholdReached;

      Dem_Internal_AssertReturnVoid(lDebounceNvIndex < Dem_Cfg_GlobalDebounceStorageEventCount(), DEM_E_INCONSISTENT_STATE)

      lDebounceValue = Dem_Mem_DebounceGetStoredValue(lDebounceNvIndex);
      lFailedThresholdReached = Dem_UDSStatus_Test_TF(Dem_Event_GetUDSStatus(lEventId));

      Dem_SatelliteCore_RestoreDebounceValue(
        Dem_Cfg_EventSatelliteId(lEventId),
        Dem_Cfg_EventSatelliteEventId(lEventId),
        lDebounceValue,
        lFailedThresholdReached);

      ++lDebounceNvIndex;
    }
  }

#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  {
    Dem_Cfg_CombinedGroupIndexType lGroupId;
    for (lGroupId = 0u; lGroupId < Dem_Cfg_GlobalCombinedGroupCount(); ++lGroupId)
    {
      Dem_Event_InternalStatusType lInternalStatus;
      Dem_Cfg_CombinedGroupIterType lSubEventIter;

      /* Initialize the combination group internal status */
      lInternalStatus = 0x00u;

      /* For all combination groups, test if at least one sub-events is available by configuration.
         Initialization in PreInit and Init can have different settings for
         event availability, so repeat the calculation for combined DTCs */
      for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lSubEventIter);                                                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
           Dem_Cfg_CombinedGroupIterExists(&lSubEventIter) == TRUE;                                                              /* PRQA S 2990 */ /* MD_DEM_2990 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
           Dem_Cfg_CombinedGroupIterNext(&lSubEventIter))                                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        if (Dem_Cfg_EventAvailableByVariant(Dem_Cfg_CombinedGroupIterGet(&lSubEventIter)) == TRUE)                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
        { /* If so, set the availability in the combination status. */
          lInternalStatus = Dem_EventInternalStatus_SetAvailableInVariant(lInternalStatus);
          break;
        }
      }
      Dem_DTCInternalStatus_CombinedGroupSetInternalStatus(lGroupId, lInternalStatus);
    }
  }
#endif

  Dem_Memory_TimeSeries_Init();

  lResetStoredData = (boolean)( (Dem_Cfg_AdminData.ImplementationVersion != Dem_Make16Bit(DEM_SW_MAJOR_VERSION, DEM_SW_MINOR_VERSION))
                           || (Dem_Cfg_AdminData.ConfigurationId != Dem_Cfg_GlobalConfigurationId())                             
                           || (Dem_Memory_GetInitAdminDataRequested() == TRUE));                                                 

  if (lResetStoredData == TRUE)
  {
    /* If the NV data is not compatible to the configuration or could not be restored by NvM */
    /* Re-initialize all NV data content and reset the global time stamp */
    Dem_Cfg_MemoryIndexType lMemoryIndex;
    Dem_Memory_AllEventMemEntriesIterType lEventMemEntryIter;

    Dem_Nvm_SetAllBlocksDirty();

    /* Do not reinitialize an already initialized Admin data block */
    if (Dem_Memory_GetInitAdminDataRequested() == FALSE)
    {
      (void)Dem_NvM_InitAdminData();
    }

    /* Reset flag to prevent re-initialization on Shutdown-MasterInit*/
    Dem_Memory_SetInitAdminDataRequested(FALSE);

    (void)Dem_NvM_InitStatusData();

    if (Dem_Mem_IsDebounceNvEnabled() == TRUE)
    {
      (void)Dem_NvM_InitDebounceData();
    }

    if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
    {
      (void)Dem_NvM_InitObdFreezeFrameData();
    }

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
    if ( (Dem_Cfg_IsPermanentSupportedInVariant() == TRUE)
      && (Dem_Mem_PermanentGetInitPattern() != DEM_MEM_PERMANENT_INIT_PATTERN) )
    { /* only initialize permanent memory if it was not initialized before */
      (void)Dem_NvM_InitObdPermanentData();
    }
#endif
    if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
    {
      Dem_Iumpr_InitNv();

#if (DEM_CFG_SUPPORT_DTR == STD_ON)
      (void)Dem_NvM_InitDtrData();
#endif
    }

#if (DEM_FEATURE_NEED_AGING_DATA == STD_ON)
    (void)Dem_NvM_InitAgingData();
#endif
    Dem_DTC_InitMemIndependentCycleCounterDataNv();

#if (DEM_CFG_SUPPORT_EVENTAVAILABLE_NV == STD_ON)
    (void)Dem_NvM_InitEventAvailableData();
#endif

    Dem_MemState_SetCurrentTimestamp(1U);

    {
      /* Translated memory index for user defined memories range from 0x01 - Number of configured memories 
         Memory Index 0x00 is reserved for Primary */

      uint16 lTranslatedMemoryIndex;
   
      for (lTranslatedMemoryIndex = 0;
          lTranslatedMemoryIndex <= Dem_Memory_UserDefinedMemory_GetNumberOfUserDefinedMemories();
          lTranslatedMemoryIndex++)
      {
        Dem_MemState_MemorySetCurrentSize((lTranslatedMemoryIndex), 0);
      }
    }

    if (Dem_Cfg_IsPermanentSupportedInVariant() == TRUE)
    {
      Dem_MemState_MemorySetCurrentSize(DEM_CFG_MEMORYID_PERMANENT, 0);
    }

    for (Dem_Memory_AllEventMemEntries_IterInit(&lEventMemEntryIter);                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Memory_AllEventMemEntries_IterExists(&lEventMemEntryIter) == TRUE;                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Memory_AllEventMemEntries_IterNext(&lEventMemEntryIter))                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      lMemoryIndex = Dem_Memory_AllEventMemEntries_IterGetMemoryIndex(&lEventMemEntryIter);                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_MemoryEntry_Init(Dem_MemoryEntry_GetId(lMemoryIndex));
    }
    Dem_Memory_TimeSeries_InitNvData();
  }
  /* Otherwise */
  else
  {
    Dem_MemState_SetCurrentTimestamp(0U);

    {
      /* Translated memory index for user defined memories range from 0x01 - Number of configured memories
      Memory Index 0x00 is reserved for Primary */

      uint16 lTranslatedMemoryIndex;

      for (lTranslatedMemoryIndex = 0;
           lTranslatedMemoryIndex <= Dem_Memory_UserDefinedMemory_GetNumberOfUserDefinedMemories();
           lTranslatedMemoryIndex++)
      {
        Dem_Memory_Init_RestoreMemory(lTranslatedMemoryIndex);
      }
    }
  }
#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
  if (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)
  {
    /* In OBD configurations, initialize the visible OBD freeze frame */
    Dem_Mem_FreezeFrameIndexMode02 = Dem_MemObdFreezeFrame_SelectHighestPriorFreezeFrame();
  }
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
  /* If prestore freeze frame is enabled, initialize freeze frame prestorage */
  Dem_Prestore_Init();
#endif

  Dem_StatusIndicator_Init();
}                                                                                                                                /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_Memory_InitConfirmedChronology
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memory_InitConfirmedChronology(
  void
  )
{
  Dem_MemPermanent_ConfirmedChronologyInit();
}
#endif

/* ****************************************************************************
 % Dem_Memory_ConfirmedChronologyResetOverflown
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_ConfirmedChronologyResetOverflown(
  void
  )
{
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  Dem_MemPermanent_ConfirmedChronologyResetOverflown();
#endif
}

/* ****************************************************************************
 % Dem_Memory_ConfirmedChronoAddEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_ConfirmedChronoAddEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  MasterEventId
  )
{
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  Dem_MemPermanent_ConfirmedChronoAddEvent(MasterEventId, FALSE);
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(MasterEventId)                                                                                /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}

/* ****************************************************************************
 % Dem_Memory_PermanentFillUp
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memory_PermanentFillUp(void)
{
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  Dem_MemPermanent_FillUp();
#endif
}

/* ****************************************************************************
 % Dem_Memory_UpdatePermanentMemoryOnEnteringActivationMode4
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_UpdatePermanentMemoryOnEnteringActivationMode4(                                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  void
)
{
  if (Dem_Cfg_IsPermanentSupportedInVariant() == TRUE)
  {
    Dem_MemPermanent_UpdatePermanentMemoryOnEnteringActivationMode4();
  }
}

/* ****************************************************************************
 % Dem_Memory_ConfirmedChronoFreeEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_ConfirmedChronoFreeEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  Dem_MemPermanent_ConfirmedChronoFreeEvent(EventId);
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}

/* ****************************************************************************
 % Dem_Memory_UpdatePermanentEntryVisibility
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_UpdatePermanentEntryVisibility(                                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
# if (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON) && (DEM_CFG_SUPPORT_PERMANENTDTC_IMMEDIATE_VISIBLE == STD_ON)
  Dem_MemPermanent_UpdateEntryVisibility(EventId);
# else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
# endif
}


/* ****************************************************************************
 % Dem_Memory_GetSRecNumerationType
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SRecNumerationType, DEM_CODE)
Dem_Memory_GetSRecNumerationType(
  uint16 TranslatedMemoryId
)
{
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
    return Dem_Cfg_GetSRecNumTypeOfMemoryInfoTable(TranslatedMemoryId);
}

/* ****************************************************************************
 % Dem_Memory_TestEventStorageTriggerIsOnConfirmed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_TestEventStorageTriggerIsOnConfirmed(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId
  )
{
  boolean lStorageTriggerFulfilled;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  lStorageTriggerFulfilled = (boolean)(Dem_Cfg_GetEventMemoryEntryStorageTriggerOfMemoryInfoTable(
                                        Dem_Cfg_EventDestination(EventId)) == DEM_MEMORY_STORAGE_TRIGGER_CONFIRMED);
  return lStorageTriggerFulfilled;
}

/* ****************************************************************************
 % Dem_Memory_TestEventStorageTriggerIsOnFailed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_TestEventStorageTriggerIsOnFailed(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId
  )
{
  boolean lStorageTriggerFulfilled;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  lStorageTriggerFulfilled = (boolean)(Dem_Cfg_GetEventMemoryEntryStorageTriggerOfMemoryInfoTable(
                               Dem_Cfg_EventDestination(EventId)) == DEM_MEMORY_STORAGE_TRIGGER_FAILED);
  return lStorageTriggerFulfilled;
}

/* ****************************************************************************
 % Dem_Memory_TestEventStorageTriggerIsOnPending
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_TestEventStorageTriggerIsOnPending(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId
  )
{
  boolean lStorageTriggerFulfilled;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  lStorageTriggerFulfilled = (boolean)(Dem_Cfg_GetEventMemoryEntryStorageTriggerOfMemoryInfoTable(
                               Dem_Cfg_EventDestination(EventId)) == DEM_MEMORY_STORAGE_TRIGGER_PENDING);
  return lStorageTriggerFulfilled;
}

/* ****************************************************************************
 % Dem_Memory_TestEventStorageTriggerIsOnFdc
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_TestEventStorageTriggerIsOnFdc(                                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId
  )
{
  boolean lStorageTriggerFulfilled;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  lStorageTriggerFulfilled = (boolean)(Dem_Cfg_GetEventMemoryEntryStorageTriggerOfMemoryInfoTable(
                               Dem_Cfg_EventDestination(EventId)) == DEM_MEMORY_STORAGE_TRIGGER_FDC);
  return lStorageTriggerFulfilled;
}

/* ****************************************************************************
 % Dem_Memory_TestEventStorageTriggerIsFulfilled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_TestEventStorageTriggerIsFulfilled(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId,
  Dem_Cfg_StorageTriggerType DtcEffects
)
{
  boolean lStorageTriggerFulfilled;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  lStorageTriggerFulfilled = (boolean)((DtcEffects & Dem_Cfg_GetEventMemoryEntryStorageTriggerOfMemoryInfoTable(
                                        Dem_Cfg_EventDestination(EventId))) != 0u);
  return lStorageTriggerFulfilled;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_MEMORYIF_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_MemoryIF_Implementation.h
 *********************************************************************************************************************/
