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
/*! \addtogroup Dem_SnapshotEntry
 *  \{
 *  \file       Dem_SnapshotEntry_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Encapsulates the list of snapshot record buffers stored in the memory entry
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_SNAPSHOTENTRY_IMPLEMENTATION_H)
#define DEM_SNAPSHOTENTRY_IMPLEMENTATION_H

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

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_SnapshotEntry_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Core_Interface.h"
#include "Dem_RecordReader_Interface.h"
#include "Dem_Memory_Interface.h"
#include "Dem_MemoryEntryIF_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_SNAPSHOTENTRY_IMPLEMENTATION_FILENAME "Dem_SnapshotEntry_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/


/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_SnapshotEntry_Private Private Methods
 * \{
 */
#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_SnapshotEntry_IteratorIsInvalidated
 *****************************************************************************/
/*!
 * \brief         Test if the snapshot entry is in state 'invalidated'
 *
 * \details       Test if the snapshot entry is in state 'invalidated'
 *
 * \param[in]     SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON &&
 *                DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_SnapshotEntry_IteratorIsInvalidated(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 * Dem_SnapshotEntry_IteratorIsCombinedSnapshotRecordStored
 *****************************************************************************/
/*!
 * \brief         Tests whether a combined snapshot record is currently stored.
 *
 * \details       Tests whether a combined snapshot record pointed to by the iterator
 *                is currently stored.
 *
 * \param[in]     SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \return        TRUE
 *                The snapshot record is stored
 * \return        FALSE
 *                The snapshot record is not stored
 *
 * \pre           SnapshotEntryIter->MemoryEntryId must be inside valid range
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *                && DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_SnapshotEntry_IteratorIsCombinedSnapshotRecordStored(
CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
);
#endif

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_IteratorIsInvalidated
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_SnapshotEntry_IteratorIsInvalidated(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  return (boolean)(SnapshotEntryIter->MemoryEntryId == DEM_MEMORYENTRY_HANDLE_INVALID);
}
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_IteratorIsCombinedSnapshotRecordStored
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
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_SnapshotEntry_IteratorIsCombinedSnapshotRecordStored(
CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
)
{
  boolean lSnapshotRecordIsStored;
  Dem_EventIdType lEventId;
  Dem_Memory_MemEntryIterType lMemEntryIter;
  Dem_Cfg_MemoryEntryHandleType lMemoryEntryId;

  lSnapshotRecordIsStored = FALSE;
  lMemoryEntryId = SnapshotEntryIter->MemoryEntryId;

  if (SnapshotEntryIter->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID)                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_SRecNumerationType lSrecNumerationType = Dem_Memory_GetSRecNumerationType(DEM_CFG_MEMORYID_PRIMARY);

    for (Dem_Memory_MemEntry_IterResume_FromEntryId(DEM_CFG_MEMORYID_PRIMARY, lMemoryEntryId, &lMemEntryIter);                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Memory_MemEntry_IterExists(&lMemEntryIter) == TRUE;                                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Memory_MemEntry_IterNext(&lMemEntryIter))                                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      lMemoryEntryId = Dem_Memory_MemEntry_IterGetEntryId(&lMemEntryIter);                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

      lEventId = Dem_MemoryEntry_GetEventId(lMemoryEntryId);
      if (Dem_Cfg_EventCombinedGroup(lEventId) == SnapshotEntryIter->CombinedDTCIndex)
      {
        if (Dem_Memory_UserDefinedMemory_IsSRecNumerationCalculated(lSrecNumerationType))
        {
          if (Dem_MemoryEntry_GetSnapshotHeader(lMemoryEntryId) > SnapshotEntryIter->SnapshotEntryIndex)
          {
            lSnapshotRecordIsStored = TRUE;
          }
        }
        else /* Uses configured records */
        {
          uint32 lMask;

          lMask = (uint32)1 << SnapshotEntryIter->CombinedDTCSnapshotEntryIndex;

          if ((Dem_MemoryEntry_GetCombinedDTCSnapshotHeader(lMemoryEntryId) & lMask) != 0u)
          {
            lSnapshotRecordIsStored = TRUE;
          }
        }

        if (lSnapshotRecordIsStored == TRUE)
        {
          break;
        }
      }
    }
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
  return lSnapshotRecordIsStored;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON) */

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
 * \addtogroup Dem_SnapshotEntry_Public
 * \{
 */

#if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)\
    && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 % Dem_SnapshotEntry_SRecIsSupportedByAvailableCombinedEvent
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
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_SnapshotEntry_SRecIsSupportedByAvailableCombinedEvent(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  CombinedDTCGroup,
  CONST(uint8, AUTOMATIC)  SRecId
  )
{
  boolean lMatch;
  lMatch = FALSE;

  if ( (Dem_RecordReader_0x1904_IsObdIIFreezeFrameInService19EnabledInVariant() == TRUE)
    && (SRecId == DEM_RECORDREADER_SNAPSHOTDATARECORD_OBD) )
  /* global OBD II freeze frame is always supported */
  {
    lMatch = TRUE;
  }
  else
  {
    Dem_Cfg_FreezeFrameRecordEventIterType lFreezeFrameRecordEventIter;

    for (Dem_Cfg_FreezeFrameRecordEventIterInit(&lFreezeFrameRecordEventIter, CombinedDTCGroup, SRecId);                         /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_FreezeFrameRecordEventIterExists(&lFreezeFrameRecordEventIter) == TRUE;                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_FreezeFrameRecordEventIterNext(&lFreezeFrameRecordEventIter))                                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_EventIdType lEventId;
      lEventId = Dem_Cfg_FreezeFrameRecordEventIterGet(&lFreezeFrameRecordEventIter);                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */

      if ((Dem_Cfg_EventAvailableByVariant(lEventId) == TRUE)
        && (Dem_EventInternalStatus_TestEventDisconnected(Dem_Event_GetInternalStatus(lEventId)) == FALSE))                      
      {
        lMatch = TRUE;
        break;
      }
    }
  }

  return lMatch;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) 
/* ****************************************************************************
 % Dem_SnapshotEntry_IteratorInvalidate
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_SnapshotEntry_IteratorInvalidate(
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter                                            /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  )
{
#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
  SnapshotEntryIter->MemoryEntryId = DEM_MEMORYENTRY_HANDLE_INVALID;                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SnapshotEntryIter)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_IteratorInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_SnapshotEntry_IteratorInit(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_Cfg_MemoryEntryHandleType, AUTOMATIC)  MemoryEntryId,
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  SnapshotEntryIter->EventId = EventId;                                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  SnapshotEntryIter->MemoryEntryId = MemoryEntryId;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  SnapshotEntryIter->SnapshotEntryIndex = 0u;                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  SnapshotEntryIter->SnapshotFifoEntryIndex = 0u;                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

  if (SnapshotEntryIter->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID)
  {
    SnapshotEntryIter->SnapshotIteratorIndex = Dem_Cfg_SRecNumStartIdx(EventId);                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

    if (Dem_Event_IsCalculatedFifoSRecsUsed(EventId) == TRUE)
    {
      if (Dem_MemoryEntry_GetSnapshotHeader(SnapshotEntryIter->MemoryEntryId) != 
          Dem_MemoryEntry_MemoryEntryIF_GetSnapshotHeaderFifo(SnapshotEntryIter->MemoryEntryId))
      {
        SnapshotEntryIter->SnapshotEntryIndex =
          Dem_MemoryEntry_MemoryEntryIF_GetSnapshotHeaderFifo(SnapshotEntryIter->MemoryEntryId);                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      }
    }
  }
  else
  {
    SnapshotEntryIter->SnapshotIteratorIndex = 0u;                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}
#endif /* (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_IteratorNext
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_SnapshotEntry_IteratorNext(
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  if (SnapshotEntryIter->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID)
  {
    SnapshotEntryIter->SnapshotIteratorIndex++;                                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    SnapshotEntryIter->SnapshotEntryIndex++;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    SnapshotEntryIter->SnapshotFifoEntryIndex++;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

    if (Dem_Event_IsCalculatedFifoSRecsUsed(SnapshotEntryIter->EventId) == TRUE)
    {
      if (Dem_MemoryEntry_GetSnapshotHeader(SnapshotEntryIter->MemoryEntryId) !=
        Dem_MemoryEntry_MemoryEntryIF_GetSnapshotHeaderFifo(SnapshotEntryIter->MemoryEntryId))
      {
        if (SnapshotEntryIter->SnapshotEntryIndex >= Dem_MemoryEntry_GetSnapshotHeader(SnapshotEntryIter->MemoryEntryId))
        {
          SnapshotEntryIter->SnapshotEntryIndex = 0u;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        }
      }
    }
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
}
#endif /* (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_IteratorExists
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_SnapshotEntry_IteratorExists(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  boolean lIteratorExists;
  if (SnapshotEntryIter->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID)
  {
    lIteratorExists = (boolean)(SnapshotEntryIter->SnapshotIteratorIndex <
      Dem_Cfg_SRecNumEndIdx(SnapshotEntryIter->EventId));
  }
  else
  {
    lIteratorExists = FALSE;
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
  return lIteratorExists;
}
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_CombinationOnRetrival_IteratorInit
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
Dem_SnapshotEntry_CombinationOnRetrival_IteratorInit(
CONST(Dem_EventIdType, AUTOMATIC)  EventId,
CONST(Dem_Cfg_MemoryEntryHandleType, AUTOMATIC)  MemoryEntryId,
CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
)
{
  SnapshotEntryIter->EventId = EventId;                                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  SnapshotEntryIter->MemoryEntryId = MemoryEntryId;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  SnapshotEntryIter->SnapshotEntryIndex = 0u;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  if (SnapshotEntryIter->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID)
  {
    SnapshotEntryIter->SnapshotIteratorIndex =
      Dem_Cfg_GetFreezeFrameNumTableStartIdxOfEventTable(EventId);                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

    SnapshotEntryIter->CombinedDTCIndex = Dem_Cfg_EventCombinedGroup(EventId);                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    if (SnapshotEntryIter->CombinedDTCIndex != DEM_CFG_COMBINED_GROUP_INVALID)
    {
      SnapshotEntryIter->CombinedDTCSnapshotEntryIndex = 0u;                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      Dem_Cfg_DTCFreezeFrameRecordIterInit(
        &(SnapshotEntryIter->CombinedDTCSnapShotIterator)                                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
        , SnapshotEntryIter->CombinedDTCIndex);
    }
  }
  else
  {
    SnapshotEntryIter->SnapshotIteratorIndex = 0u;                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON) */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_CombinationOnRetrival_IteratorNext
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
Dem_SnapshotEntry_CombinationOnRetrival_IteratorNext(
CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
)
{
  Dem_Cfg_SRecNumerationType lSrecNumerationType = Dem_Memory_GetSRecNumerationType(DEM_CFG_MEMORYID_PRIMARY);

  if (SnapshotEntryIter->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID)
  {
    if (SnapshotEntryIter->CombinedDTCIndex != DEM_CFG_COMBINED_GROUP_INVALID)                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    {
      if (Dem_Memory_UserDefinedMemory_IsSRecNumerationConfigured(lSrecNumerationType) == TRUE)
      {
        Dem_Cfg_DTCFreezeFrameRecordIterNext(&SnapshotEntryIter->CombinedDTCSnapShotIterator);                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      }
      else
      {
        SnapshotEntryIter->SnapshotEntryIndex++;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      }

      SnapshotEntryIter->CombinedDTCSnapshotEntryIndex++;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
    {
      SnapshotEntryIter->SnapshotIteratorIndex++;                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      SnapshotEntryIter->SnapshotEntryIndex++;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
}
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON) */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_CombinationOnRetrival_IteratorExists
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
Dem_SnapshotEntry_CombinationOnRetrival_IteratorExists(
CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
)
{
  boolean lIteratorExists;
  if (SnapshotEntryIter->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID)
  {
# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON) /* Configured */
    if (SnapshotEntryIter->CombinedDTCIndex != DEM_CFG_COMBINED_GROUP_INVALID)
    {
      lIteratorExists = Dem_Cfg_DTCFreezeFrameRecordIterExists(&SnapshotEntryIter->CombinedDTCSnapShotIterator);                 /* SBSW_DEM_CALL_ITERATOR_POINTER */
    }
    else
# endif
    {
      lIteratorExists = (boolean)(SnapshotEntryIter->SnapshotIteratorIndex <
        Dem_Cfg_SRecNumEndIdx(SnapshotEntryIter->EventId));
    }
  }
  else
  {
    lIteratorExists = FALSE;
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
  return lIteratorExists;
}
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_IteratorGetSnapshotRecordIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SRecIndexType, DEM_CODE)
Dem_SnapshotEntry_IteratorGetSnapshotRecordIndex(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  Dem_Cfg_SRecIndexType lIndex;
  if (SnapshotEntryIter->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID)
  {
    lIndex = SnapshotEntryIter->SnapshotIteratorIndex;
  }
  else
  {
    lIndex = 0u;
  }
  return lIndex;
}
#endif /* (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_IteratorGetSnapshotEntryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_SnapshotEntry_IteratorGetSnapshotEntryIndex(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  uint8 lIndex;
  if (SnapshotEntryIter->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID)
  {
    lIndex = SnapshotEntryIter->SnapshotEntryIndex;
  }
  else
  {
    lIndex = 0u;
  }
  return lIndex;
}
#endif 

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
% Dem_SnapshotEntry_IteratorGetSnapshotFifoEntryIndex
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_SnapshotEntry_IteratorGetSnapshotFifoEntryIndex(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
)
{
  return SnapshotEntryIter->SnapshotFifoEntryIndex;
}
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_IteratorGetSnapshotRecordDataConstPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_ConstSharedDataPtrType, DEM_CODE)
Dem_SnapshotEntry_IteratorGetSnapshotRecordDataConstPtr(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  if (SnapshotEntryIter->MemoryEntryId == DEM_MEMORYENTRY_HANDLE_INVALID)
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
  return Dem_MemoryEntry_GetSnapshotRecordDataConstPtr(SnapshotEntryIter->MemoryEntryId, SnapshotEntryIter->SnapshotEntryIndex);
}
#endif /* (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_IteratorIsSnapshotRecordStored
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
Dem_SnapshotEntry_IteratorIsSnapshotRecordStored(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  boolean lSnapshotRecordIsStored;

  lSnapshotRecordIsStored = FALSE;

  if (SnapshotEntryIter->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID)
  {
    if (Dem_Event_IsCalculatedFifoSRecsUsed(SnapshotEntryIter->EventId) == TRUE)
    {
      if (Dem_MemoryEntry_GetSnapshotHeader(SnapshotEntryIter->MemoryEntryId) > 
          Dem_SnapshotEntry_IteratorGetSnapshotFifoEntryIndex(SnapshotEntryIter))                                                /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      {
        lSnapshotRecordIsStored = TRUE;
      }
    }
    else if (Dem_Event_IsCalculatedSRecsUsed(SnapshotEntryIter->EventId) == TRUE)
    {
      if (Dem_MemoryEntry_GetSnapshotHeader(SnapshotEntryIter->MemoryEntryId) > SnapshotEntryIter->SnapshotEntryIndex)
      {
        lSnapshotRecordIsStored = TRUE;
      }
    }
    else /* Uses configured records */
    {
      uint8 lMask;

      lMask = (uint8)1 << SnapshotEntryIter->SnapshotEntryIndex;
      if ((Dem_MemoryEntry_GetSnapshotHeader(SnapshotEntryIter->MemoryEntryId) & lMask) != 0u)
      {
        lSnapshotRecordIsStored = TRUE;
      }
    }
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }

  return lSnapshotRecordIsStored;
}
#endif /* DEM_CFG_SUPPORT_SRECS == STD_ON */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_IteratorTestSnapshotRecordFifoStorageRequired
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_SnapshotEntry_IteratorTestSnapshotRecordFifoStorageRequired(                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
)
{
  boolean lNeedToStoreSnapshotRecord;
  uint8 lNextRecordPos;

  lNeedToStoreSnapshotRecord = FALSE;
  lNextRecordPos = Dem_MemoryEntry_MemoryEntryIF_GetSnapshotHeaderFifo(SnapshotEntryIter->MemoryEntryId);

  if (lNextRecordPos == SnapshotEntryIter->SnapshotEntryIndex)
  {
    lNeedToStoreSnapshotRecord = TRUE;
  }

  return lNeedToStoreSnapshotRecord;
}
#endif /* DEM_CFG_SUPPORT_SRECS == STD_ON */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_IteratorUpdateSnapshotEntry
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
Dem_SnapshotEntry_IteratorUpdateSnapshotEntry(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  if (SnapshotEntryIter->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID)
  {
    if (Dem_Event_IsCalculatedSRecsUsed(SnapshotEntryIter->EventId) == TRUE)
    {
      if (Dem_MemoryEntry_GetSnapshotHeader(SnapshotEntryIter->MemoryEntryId) == SnapshotEntryIter->SnapshotEntryIndex)
      { /* Only increment if a new snapshot is added, updates keep the current ID */
        Dem_MemoryEntry_SetSnapshotHeader(SnapshotEntryIter->MemoryEntryId,
          (uint8)(Dem_MemoryEntry_GetSnapshotHeader(SnapshotEntryIter->MemoryEntryId) + 1u));
      }

      /* For calculated fifo, also update the snapshot header fifo with wrapping */
      if (Dem_Event_IsCalculatedFifoSRecsUsed(SnapshotEntryIter->EventId) == TRUE)
      {
        uint8 lSnapshotHeaderFifo;
        lSnapshotHeaderFifo = (Dem_MemoryEntry_MemoryEntryIF_GetSnapshotHeaderFifo(SnapshotEntryIter->MemoryEntryId) + 1u) %
          Dem_Cfg_SRecNumConfigured(SnapshotEntryIter->EventId);

        Dem_MemoryEntry_MemoryEntryIF_SetSnapshotHeaderFifo(SnapshotEntryIter->MemoryEntryId, lSnapshotHeaderFifo);
      }
    }
    else /* Uses configured records */
    {
      uint8 lMask;

      lMask = (uint8)1 << SnapshotEntryIter->SnapshotEntryIndex;
      Dem_MemoryEntry_SetSnapshotHeader(
        SnapshotEntryIter->MemoryEntryId, (uint8)(Dem_MemoryEntry_GetSnapshotHeader(SnapshotEntryIter->MemoryEntryId) | lMask));
    }
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

# if ((DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)\
      && (DEM_CFG_SUPPORT_SRECS == STD_ON))
/* ****************************************************************************
 % Dem_SnapshotEntry_SetSRecStorageInDTCHeader
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_SnapshotEntry_SetSRecStorageInDTCHeader(
  CONST(uint8, AUTOMATIC)  SRecId,
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  CombinedDTCGroup,    
  CONST(Dem_Cfg_MemoryEntryHandleType, AUTOMATIC)  MemoryEntryId
  )
{
  if (MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID)
  {
    uint32 lMask;
    uint8 lMaskIndex;
    Dem_Cfg_SRecNumerationType lSRecNumerationType;
    Dem_EventIdType lEventId;

    lMask = 0x00U;

    lEventId = Dem_MemoryEntry_GetEventId(MemoryEntryId);
    lSRecNumerationType = Dem_Memory_GetSRecNumerationType(Dem_Cfg_EventDestination(lEventId));
    lMaskIndex = Dem_SnapshotEntry_FindSRecIndexInCombinedDTCSRecList(SRecId, lSRecNumerationType, CombinedDTCGroup);

    if (lMaskIndex < DEM_SNAPSHOTENTRY_CONFIGURED_SREC_INDEX_INVALID)
    {
      lMask = (uint32)1u << lMaskIndex;
      Dem_MemoryEntry_SetCombinedDTCSnapshotHeader(MemoryEntryId,
        (Dem_MemoryEntry_GetCombinedDTCSnapshotHeader(MemoryEntryId) | lMask));
    }
    else
    {
      /* Stored an SRec which was not present in the DTC SRec List */
      Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
    }
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
}
#endif 

#if ((DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 % Dem_SnapshotEntry_GetSRecStorageStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_SnapshotEntry_GetSRecStorageStatus(
  CONST(uint8, AUTOMATIC)  SRecId,
  CONST(uint8, AUTOMATIC)  SnapshotHeaderBitMaskIndex,
  CONST(Dem_Cfg_MemoryEntryHandleType, AUTOMATIC)  MemoryEntryId
  )
{
  boolean lSnapshotRecordIsStored;
  Dem_EventIdType lEventId;

  lSnapshotRecordIsStored = FALSE;
  lEventId = Dem_MemoryEntry_GetEventId(MemoryEntryId);

  if (Dem_Event_IsCalculatedSRecsUsed(lEventId) == TRUE)
  {
    if (Dem_MemoryEntry_GetSnapshotHeader(MemoryEntryId) >= SRecId)
    {
      lSnapshotRecordIsStored = TRUE;
    }
  }
  else /* Uses configured records */
  {
    uint32 lMask;

    lMask = 0x00U;
    if (SnapshotHeaderBitMaskIndex < DEM_SNAPSHOTENTRY_CONFIGURED_SREC_INDEX_INVALID)
    {
      lMask = (uint32)1u << SnapshotHeaderBitMaskIndex;
      if ((Dem_MemoryEntry_GetCombinedDTCSnapshotHeader(MemoryEntryId) & lMask) != 0u)
      {
        lSnapshotRecordIsStored = TRUE;
      }
    }
    else
    {
      Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
    }
  }

  return lSnapshotRecordIsStored;
}
#endif 

# if ((DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 % Dem_SnapshotEntry_FindSRecIndexInCombinedDTCSRecList
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
Dem_SnapshotEntry_FindSRecIndexInCombinedDTCSRecList(
  CONST(uint8, AUTOMATIC)  SRecId,
  CONST(Dem_Cfg_SRecNumerationType, AUTOMATIC)  SRecNumerationType,
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  CombinedDTCGroup
  )
{
  uint8 lMaskIndex;
  boolean lRecordFound;
  Dem_Cfg_DTCFreezeFrameIterType lDTCFreezeFrameIter;

  lRecordFound = FALSE;
  lMaskIndex = 0x00U;

  if (Dem_Memory_UserDefinedMemory_IsSRecNumerationCalculated(SRecNumerationType) == TRUE)
  {
    /* If the DTC at least has one freeze frame configured */
    Dem_Cfg_DTCFreezeFrameRecordIterInit(&lDTCFreezeFrameIter, CombinedDTCGroup);                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
    if (Dem_Cfg_DTCFreezeFrameRecordIterExists(&lDTCFreezeFrameIter) == TRUE)                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      /* Initialize to end of list. If the last SRec Id is greater than requested SRec Id,
         the SRec exists in list since the record numbers are sequential */
      if (Dem_Cfg_GetMaxFreezeFrameIdCombinedEvent(CombinedDTCGroup) >= SRecId)
      {
        lRecordFound = TRUE; 
      }
    }
  }
  else /* Uses configured records */
  {
    for (Dem_Cfg_DTCFreezeFrameRecordIterInit(&lDTCFreezeFrameIter, CombinedDTCGroup);                                           /* PRQA S 0771 */ /* MD_DEM_15.4_opt */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
         (Dem_Cfg_DTCFreezeFrameRecordIterExists(&lDTCFreezeFrameIter) == TRUE);                                                 /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_DTCFreezeFrameRecordIterNext(&lDTCFreezeFrameIter))                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      if (Dem_Cfg_DTCFreezeFrameRecordIterGet(&lDTCFreezeFrameIter) == SRecId)                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        lRecordFound = TRUE; 
        break;
      }

      if (Dem_Cfg_DTCFreezeFrameRecordIterGet(&lDTCFreezeFrameIter) > SRecId)                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
      { /* In an ordered list, no point to search for entries greater the SRec Id. 
           This means the combined DTC does not have the freeze frame configured */
        break;
      }

      lMaskIndex++;
    }
  }

  /* Return invalid index if record is not found */
  if (lRecordFound == FALSE)
  {
    lMaskIndex = DEM_SNAPSHOTENTRY_CONFIGURED_SREC_INDEX_INVALID;
  }

  return lMaskIndex;
}
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_IteratorCopySnapshotEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_SnapshotEntry_IteratorCopySnapshotEntry(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter,
  CONST(uint8, AUTOMATIC)  EntryIndex
  )
{
  if ( (SnapshotEntryIter->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID)
    && (EntryIndex < Dem_Cfg_GlobalSRecMaxCount()) )                                                                             
  {
    Dem_MemCpy((Dem_MemoryEntry_GetSnapshotRecordDataPtr(SnapshotEntryIter->MemoryEntryId, SnapshotEntryIter->SnapshotEntryIndex)), /* PRQA S 0315, 0602 */ /* MD_MSR_VStdLibCopy, MD_DEM_20.2 */  /* SBSW_DEM_POINTER_SNAPSHOT_BUFFER */
               (Dem_MemoryEntry_GetSnapshotRecordDataPtr(SnapshotEntryIter->MemoryEntryId, EntryIndex)),
                Dem_Cfg_EventSRecMaxRawSize());
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
}
#endif /* (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_IteratorFindLastStoredEntry
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
Dem_SnapshotEntry_IteratorFindLastStoredEntry(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_Cfg_MemoryEntryHandleType, AUTOMATIC)  MemoryEntryId,
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  for (Dem_SnapshotEntry_IteratorInit(EventId, MemoryEntryId, SnapshotEntryIter);                                                /* PRQA S 2467, 2476, 2477 */ /* MD_DEM_14.2_LoopControl, MD_DEM_14.2_LoopInitialisation, MD_DEM_14.2_LoopControl */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
       Dem_SnapshotEntry_IteratorExists(SnapshotEntryIter) == TRUE;                                                              /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
       Dem_SnapshotEntry_IteratorNext(SnapshotEntryIter))                                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    if ( (Dem_MemoryEntry_GetSnapshotHeader(MemoryEntryId) > 0u)
      && (Dem_MemoryEntry_GetSnapshotHeader(MemoryEntryId) <= Dem_Cfg_GlobalSRecMaxCount()) )                                    /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    {
      boolean lFound = FALSE;

      if (Dem_Event_IsCalculatedFifoSRecsUsed(EventId) == TRUE)
      {
        /* Calculated fifo snapshot records */
        if ((Dem_MemoryEntry_GetSnapshotHeader(MemoryEntryId) - 1u) == 
            Dem_SnapshotEntry_IteratorGetSnapshotFifoEntryIndex(SnapshotEntryIter))                                              /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        {
          lFound = TRUE;
        }
      }
      else
      {
        /* Calculated snapshot records */
        if ((Dem_MemoryEntry_GetSnapshotHeader(MemoryEntryId) - 1u) == SnapshotEntryIter->SnapshotEntryIndex)
        {
          lFound = TRUE;
        }
      }

      if (lFound == TRUE)
      {
        break;
      }
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_IteratorFindEntryWithRecordNumber
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
Dem_SnapshotEntry_IteratorFindEntryWithRecordNumber(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  RecordNumber,
  CONST(Dem_Cfg_MemoryEntryHandleType, AUTOMATIC)  MemoryEntryId,
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  for (Dem_SnapshotEntry_IteratorInit(EventId, MemoryEntryId, SnapshotEntryIter);                                                /* PRQA S 2467, 2476, 2477 */ /* MD_DEM_14.2_LoopControl, MD_DEM_14.2_LoopInitialisation, MD_DEM_14.2_LoopControl */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
       Dem_SnapshotEntry_IteratorExists(SnapshotEntryIter) == TRUE;                                                              /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
       Dem_SnapshotEntry_IteratorNext(SnapshotEntryIter))                                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_Cfg_SRecIndexType lCfgSRecIndex;

    lCfgSRecIndex = Dem_SnapshotEntry_IteratorGetSnapshotRecordIndex(SnapshotEntryIter);                                         /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

    if (RecordNumber == Dem_Cfg_SRecId(lCfgSRecIndex))
    {
      if (Dem_SnapshotEntry_IteratorIsSnapshotRecordStored(SnapshotEntryIter) == TRUE)                                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      {
        break;
      }
    }
  }
}
#endif /* (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_SearchStoredEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_SnapshotEntry_SearchStoredEntry(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_Cfg_MemoryEntryHandleType, AUTOMATIC)  MemoryEntryId,
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter                                            /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  )
{
  boolean lFoundEntry;
  lFoundEntry = FALSE;

  if(Dem_SnapshotEntry_IteratorIsInvalidated(SnapshotEntryIter) == TRUE)                                                         /* SBSW_DEM_POINTER_FORWARD_STACK */
  {
#  if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
    Dem_SnapshotEntry_CombinationOnRetrival_IteratorInit(EventId, MemoryEntryId, SnapshotEntryIter);                            /* SBSW_DEM_POINTER_FORWARD_STACK */
#  else
    Dem_SnapshotEntry_IteratorInit(EventId, MemoryEntryId, SnapshotEntryIter);                                                  /* SBSW_DEM_POINTER_FORWARD_STACK */
#  endif
  }

  if (Dem_Event_IsConfiguredSRecsUsed(EventId) == TRUE)
  {
    while ((Dem_SnapshotEntry_CombinationOnRetrival_IteratorExists(SnapshotEntryIter) == TRUE) && (lFoundEntry == FALSE))        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    {
  #  if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
      if (SnapshotEntryIter->CombinedDTCIndex != DEM_CFG_COMBINED_GROUP_INVALID)
      {
        if (Dem_SnapshotEntry_IteratorIsCombinedSnapshotRecordStored(SnapshotEntryIter) == TRUE)                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        {
          lFoundEntry = TRUE;
        }
        else
        {
          Dem_SnapshotEntry_CombinationOnRetrival_IteratorNext(SnapshotEntryIter);                                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        }
      }
      else
  #  endif
      {
        if (Dem_SnapshotEntry_IteratorIsSnapshotRecordStored(SnapshotEntryIter) == TRUE)                                         /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        {
          lFoundEntry = TRUE;
        }
        else
        {
          Dem_SnapshotEntry_IteratorNext(SnapshotEntryIter);                                                                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        }
      }
    }
  }
  else /* Uses calculated snapshot records */
  {
#  if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
    if (SnapshotEntryIter->CombinedDTCIndex != DEM_CFG_COMBINED_GROUP_INVALID)
    {
      if (Dem_SnapshotEntry_IteratorIsCombinedSnapshotRecordStored(SnapshotEntryIter) == TRUE)                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      {
        lFoundEntry = TRUE;
      }
    }
    else
#  endif
    {
      if (Dem_SnapshotEntry_IteratorIsSnapshotRecordStored(SnapshotEntryIter) == TRUE)                                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      {
        lFoundEntry = TRUE;
      }
    }
  }

  return lFoundEntry;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) */


#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_IteratorGetStoredRecordNumber
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
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_SnapshotEntry_IteratorGetStoredRecordNumber(
CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
)
{
  uint8 lRecordNumber; 

  if (Dem_Event_IsCalculatedSRecsUsed(SnapshotEntryIter->EventId) == TRUE)
  {
    if (Dem_Event_IsCalculatedFifoSRecsUsed(SnapshotEntryIter->EventId) == TRUE)
    {
      lRecordNumber = (uint8)(Dem_SnapshotEntry_IteratorGetSnapshotFifoEntryIndex(SnapshotEntryIter) + 1u);                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
    else
    {
      lRecordNumber = (uint8)(Dem_SnapshotEntry_IteratorGetSnapshotEntryIndex(SnapshotEntryIter) + 1u);                            /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }
  else /* Uses configured records */
  {
# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
    if (SnapshotEntryIter->CombinedDTCIndex != DEM_CFG_COMBINED_GROUP_INVALID)
    {
      lRecordNumber = Dem_Cfg_DTCFreezeFrameRecordIterGet(&(SnapshotEntryIter->CombinedDTCSnapShotIterator));                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
    else
# endif
    {
      lRecordNumber = Dem_Cfg_SRecId(Dem_SnapshotEntry_IteratorGetSnapshotRecordIndex(SnapshotEntryIter));                         /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }

  return lRecordNumber;
}
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_GetNumberOfStoredSnapshotRecords
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
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_SnapshotEntry_GetNumberOfStoredSnapshotRecords(
  CONST(Dem_Cfg_MemoryEntryHandleType, AUTOMATIC)  MemoryEntryId
  )
{
  uint8 lNumberOfRecords;

  if (Dem_Event_IsCalculatedSRecsUsed(Dem_MemoryEntry_GetEventId(MemoryEntryId)) == TRUE)
  {
    lNumberOfRecords = Dem_MemoryEntry_GetSnapshotHeader(MemoryEntryId);
  }
  else
  {
    uint8 lSnapshotHeader;
    lNumberOfRecords = 0u;
    lSnapshotHeader = Dem_MemoryEntry_GetSnapshotHeader(MemoryEntryId);
    while (lSnapshotHeader != 0u)
    {
      lNumberOfRecords = (uint8)(lNumberOfRecords + (lSnapshotHeader & 0x01U));
      lSnapshotHeader = (uint8)(lSnapshotHeader >> 0x01U);
    }
  }

  return lNumberOfRecords;
}
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_SnapshotEntry_GetNumberOfStoredCombinedSnapshotRecords
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
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_SnapshotEntry_GetNumberOfStoredCombinedSnapshotRecords(
CONST(Dem_Memory_MemEntryIterType, AUTOMATIC)  MemEntryIter,
CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC) GroupId
)
{
  
  Dem_EventIdType lEventId;
  Dem_Memory_MemEntryIterType lMemEntryIter;
  Dem_Cfg_MemoryEntryHandleType lMemoryId;
  uint8 lCrntSnapshotHeader;
  uint8 lNumberOfRecords = 0u;
  uint32 lCombinedSnapshotHeader = 0u;
  Dem_Cfg_SRecNumerationType lSrecNumerationType = Dem_Memory_GetSRecNumerationType(DEM_CFG_MEMORYID_PRIMARY);

  for (lMemEntryIter = MemEntryIter;
    Dem_Memory_MemEntry_IterExists(&lMemEntryIter) == TRUE;                                                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Memory_MemEntry_IterNext(&lMemEntryIter))                                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {

    lMemoryId = Dem_Memory_MemEntry_IterGetEntryId(&lMemEntryIter);                                                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
    lEventId = Dem_MemoryEntry_GetEventId(lMemoryId);
   
    if ((Dem_Event_TestValidHandle(lEventId) == TRUE)
      && (Dem_Cfg_EventCombinedGroup(lEventId) == GroupId))                                                                      
    {
      if (Dem_Memory_UserDefinedMemory_IsSRecNumerationCalculated(lSrecNumerationType))
      {
        lCrntSnapshotHeader = Dem_MemoryEntry_GetSnapshotHeader(lMemoryId);
        if (lNumberOfRecords < lCrntSnapshotHeader)
        {
          lNumberOfRecords = lCrntSnapshotHeader;
        }
      }
      else /* Uses configured records */
      {
        lCombinedSnapshotHeader |= Dem_MemoryEntry_GetCombinedDTCSnapshotHeader(lMemoryId);
      }
    }
  }

  /* Count configured records */
  while (lCombinedSnapshotHeader != 0u)                                                                                          /* PRQA S 2994 */ /* MD_DEM_2994 */
  {
    lNumberOfRecords = (uint8)(lNumberOfRecords + (lCombinedSnapshotHeader & 0x01U));
    lCombinedSnapshotHeader = (uint32)(lCombinedSnapshotHeader >> 0x01U);
  }

  return lNumberOfRecords;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON) */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_SNAPSHOTENTRY_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_SnapshotEntry_Implementation.h
 *********************************************************************************************************************/
