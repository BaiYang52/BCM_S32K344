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
/*! \addtogroup Dem_MemState
 *  \{
 *  \file       Dem_MemState_Implementation.h
 *  \brief      Encapsulate global data stored for an event memory.
 *  \details    Implementation file for MemState subcomponent which encapsulate global data stored for an event memory.
 *              This includes handling chronology for event entries, tracking occupancy, overflow status, update state
 *              etc.
 *********************************************************************************************************************/
  
/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MEMSTATE_IMPLEMENTATION_H)
#define DEM_MEMSTATE_IMPLEMENTATION_H

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
#include "Dem_MemState_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_TimeSeries_Interface.h"
#include "Dem_MemoryEntry_Interface.h"
#include "Dem_MemPermanent_Interface.h"
#include "Dem_UserDefinedMemory_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_MEMSTATE_IMPLEMENTATION_FILENAME "Dem_MemState_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/
#define DEM_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DEM_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DEM_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Current time stamp, used to establish a chronological ordering */
DEM_LOCAL VAR(uint32, DEM_VAR_NOINIT)         Dem_MemState_CurrentTimestamp;

#define DEM_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */     

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */     


#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_MemState_PrivateProperties Private Properties
 * \{
 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

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
 * \addtogroup Dem_MemState_PublicProperties
 * \{
 */
/* ****************************************************************************
 % Dem_MemState_SetCurrentTimestamp
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemState_SetCurrentTimestamp(                                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint32, AUTOMATIC)  CurrentTimeStamp
  )
{
  Dem_MemState_CurrentTimestamp = CurrentTimeStamp;
}

/* ****************************************************************************
 % Dem_MemState_GetCurrentTimestamp
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_MemState_GetCurrentTimestamp(                                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  return Dem_MemState_CurrentTimestamp;
}

/* ****************************************************************************
 % Dem_MemState_MemoryGetChronology
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_MemState_MemoryGetChronology(
  uint16 TranslatedMemoryId,
  CONST(uint8, AUTOMATIC)  ChronoIndex
  )
{
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ChronoIndex)                                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return (Dem_Cfg_GetChronoPtrOfMemoryInfoTable(TranslatedMemoryId))[ChronoIndex];
}

/* ****************************************************************************
 % Dem_MemState_MemorySetChronology
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemState_MemorySetChronology(
  uint16 TranslatedMemoryId,
  CONST(uint8, AUTOMATIC)  ChronoIndex,
  CONST(Dem_Cfg_MemoryIndexType, AUTOMATIC)  MemoryIndex
  )
{
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if ( (TranslatedMemoryId >= Dem_Cfg_GetSizeOfMemoryInfoTable())
    || (ChronoIndex >= Dem_MemState_MemoryGetMaxSize(TranslatedMemoryId))
    || (Dem_Cfg_GetChronoPtrOfMemoryInfoTable(TranslatedMemoryId) == NULL_PTR))
  {
    Dem_Error_RunTimeCheckFailed(DEM_MEMSTATE_IMPLEMENTATION_FILENAME, __LINE__);                                                /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#endif
  {
    (Dem_Cfg_GetChronoPtrOfMemoryInfoTable(TranslatedMemoryId))[ChronoIndex] = MemoryIndex;                                      /* SBSW_DEM_ARRAY_WRITE_MEMORYCHRONOLOGY */
    DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                               /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  }
}

/* ****************************************************************************
 % Dem_MemState_MemoryGetFirstIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_MemState_MemoryGetFirstIndex(
  uint16 TranslatedMemoryId
  )
{
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return Dem_Cfg_GetFirstMemoryIndexOfMemoryInfoTable(TranslatedMemoryId);
}

/* ****************************************************************************
 % Dem_MemState_MemoryGetEndIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_MemState_MemoryGetEndIndex(
  uint16 TranslatedMemoryId
  )
{
#if(DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  Dem_Internal_AssertReturnValue(TranslatedMemoryId != DEM_CFG_MEMORYID_PERMANENT, DEM_E_INCONSISTENT_STATE, Dem_MemState_MemoryGetFirstIndex(TranslatedMemoryId));
#endif
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return (Dem_Cfg_MemoryIndexType)(Dem_MemState_MemoryGetFirstIndex(TranslatedMemoryId)
                                   + Dem_MemState_MemoryGetMaxSize(TranslatedMemoryId));
}

/* ****************************************************************************
 % Dem_MemState_MemoryGetMaxSize
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemState_MemoryGetMaxSize(
  uint16 TranslatedMemoryId
  )
{
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return Dem_Cfg_GetMaxSizeOfMemoryInfoTable(TranslatedMemoryId);
}

/* ****************************************************************************
 % Dem_MemState_MemoryGetCurrentSize
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemState_MemoryGetCurrentSize(
  uint16 TranslatedMemoryId
  )
{
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return Dem_Cfg_GetMemoryCurrentCount(TranslatedMemoryId);                                                                      /* PRQA S 2841 */ /* MD_DEM_Dir4.1_ReadOperation */
}

/* ****************************************************************************
 % Dem_MemState_MemorySetCurrentSize
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemState_MemorySetCurrentSize(
  uint16 TranslatedMemoryId,
  uint8 Size
  )
{
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (TranslatedMemoryId >= Dem_Cfg_GetSizeOfMemoryCurrentCount())
  {
    Dem_Error_RunTimeCheckFailed(DEM_MEMSTATE_IMPLEMENTATION_FILENAME, __LINE__);                                                /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
  if (Size > Dem_MemState_MemoryGetMaxSize(TranslatedMemoryId))
  {
    Dem_Error_RunTimeCheckFailed(DEM_MEMSTATE_IMPLEMENTATION_FILENAME, __LINE__);                                                /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    Dem_Cfg_SetMemoryCurrentCount(TranslatedMemoryId, Dem_MemState_MemoryGetMaxSize(TranslatedMemoryId));                        /* SBSW_DEM_ARRAY_WRITE_MEMORYCURRENTCOUNT */
  }
  else
#endif
  {
    Dem_Cfg_SetMemoryCurrentCount(TranslatedMemoryId, Size);                                                                     /* SBSW_DEM_ARRAY_WRITE_MEMORYCURRENTCOUNT */
  }
}

/* ****************************************************************************
 % Dem_MemState_MemoryGetFirstEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_MemState_MemoryGetFirstEvent(
  uint16 TranslatedMemoryId
  )
{
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return Dem_Cfg_GetFirstEventOfMemoryInfoTable(TranslatedMemoryId);                                                             /* PRQA S 2841 */ /* MD_DEM_Dir4.1_ReadOperation */
}

/* ****************************************************************************
 % Dem_MemState_MemoryGetLastEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_MemState_MemoryGetLastEvent(
  uint16 TranslatedMemoryId
  )
{
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return (Dem_EventIdType)Dem_Cfg_GetLastEventOfMemoryInfoTable(TranslatedMemoryId);                                             /* PRQA S 2841 */ /* MD_DEM_Dir4.1_ReadOperation */
}

/* ****************************************************************************
 % Dem_MemState_GetFirstInvalidEventBlockIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_MemState_GetFirstInvalidEventBlockIndex(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  uint16 lMemoryIndex;
  Dem_Cfg_MemoryIndexType lInvalidEventBlockIndex;

  lInvalidEventBlockIndex = Dem_MemState_MemoryGetFirstIndex(DEM_CFG_MEMORYID_PRIMARY);

  /* Primary Memory has Index 0, User Defined Memories range from 1 to Max Number */
  for (lMemoryIndex = 0;
       lMemoryIndex <= Dem_Memory_UserDefinedMemory_GetNumberOfUserDefinedMemories();
       lMemoryIndex++)
  {
    lInvalidEventBlockIndex = lInvalidEventBlockIndex + Dem_MemState_MemoryGetMaxSize(lMemoryIndex);
  }

  return lInvalidEventBlockIndex;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_MemState_Private Private Methods
 * \{
 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/
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
 * \addtogroup Dem_MemState_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Memory_MemState_InitOverflow
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_MemState_InitOverflow(
  void
  )
{
#if (DEM_FEATURE_NEED_OVFLIND == STD_ON)
  Dem_MemSet((Dem_DataPtrType) &(Dem_Cfg_AdminData.MemoryOverflow[0]),                                                           /* PRQA S 0315, 0602 */ /* MD_MSR_VStdLibCopy, MD_DEM_20.2 */ /* SBSW_DEM_MEMSET_ADMIN_DATA_MEMORY_OVERFLOW */
    0x00,
    DEM_CFG_MAX_SIZE_OVERFLOWINDICATOR * sizeof(Dem_Cfg_AdminData.MemoryOverflow[0]));
#endif
}

/* ****************************************************************************
 % Dem_Memory_MemState_SetOverflow
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_MemState_SetOverflow(
  uint16 TranslatedMemoryId
  )
{
#if (DEM_FEATURE_NEED_OVFLIND == STD_ON)
  Dem_Com_SetBitInBitfield(&(Dem_Cfg_AdminData.MemoryOverflow[0]), DEM_CFG_MAX_SIZE_OVERFLOWINDICATOR, TranslatedMemoryId);      /* SBSW_DEM_POINTER_FORWARD_ADMIN_DATA_MEMORY_OVERFLOW */
#endif
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
}

/* ****************************************************************************
 % Dem_Memory_MemState_ResetOverflow
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memory_MemState_ResetOverflow(
  uint16 TranslatedMemoryId
  )
{
#if (DEM_FEATURE_NEED_OVFLIND == STD_ON)
  Dem_Com_ResetBitInBitfield(&(Dem_Cfg_AdminData.MemoryOverflow[0]), DEM_CFG_MAX_SIZE_OVERFLOWINDICATOR, TranslatedMemoryId);    /* SBSW_DEM_POINTER_FORWARD_ADMIN_DATA_MEMORY_OVERFLOW */ 
#endif
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
}

/* ****************************************************************************
 % Dem_Memory_MemState_GetOverflow
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_MemState_GetOverflow(                                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint16 TranslatedMemoryHandle
  )
{
#if (DEM_FEATURE_NEED_OVFLIND == STD_ON)
  return Dem_Com_TestBitInBitfield(&(Dem_Cfg_AdminData.MemoryOverflow[0]), DEM_CFG_MAX_SIZE_OVERFLOWINDICATOR,                   /* SBSW_DEM_POINTER_FORWARD_ADMIN_DATA_MEMORY_OVERFLOW */ 
                                    TranslatedMemoryHandle); 
#else
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryHandle)                                                                             /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return FALSE;
#endif
}

/* ****************************************************************************
 % Dem_MemState_MemoryUpdateIndex
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
Dem_MemState_MemoryUpdateIndex(
  uint16 TranslatedMemoryId,
  CONST(Dem_Cfg_MemoryIndexType, AUTOMATIC)  MemoryIndex
  )
{
  uint8 lChronoIterator;
  Dem_Cfg_MemoryIndexType lTempIndex;

  /* Store the global timestamp to the entry, and update it */
  Dem_MemoryEntry_SetTimestamp(Dem_MemoryEntry_GetId(MemoryIndex), Dem_MemState_GetCurrentTimestamp());
  Dem_MemState_SetCurrentTimestamp(Dem_MemState_GetCurrentTimestamp() + 1u);

  /* move downwards the chronology moving all entries, until the updated element is overwritten */
  lChronoIterator = (uint8)(Dem_MemState_MemoryGetCurrentSize(TranslatedMemoryId) - 1u);
  lTempIndex = Dem_MemState_MemoryGetChronology(TranslatedMemoryId, lChronoIterator); 
  while (lTempIndex != MemoryIndex)
  {
    Dem_Cfg_MemoryIndexType lSwap;

    lSwap = lTempIndex;
    --lChronoIterator;
    lTempIndex = Dem_MemState_MemoryGetChronology(TranslatedMemoryId, lChronoIterator);
    Dem_MemState_MemorySetChronology(TranslatedMemoryId, lChronoIterator, lSwap); 

#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
    if ((lChronoIterator == 0u) && (lTempIndex != MemoryIndex))
    {
      /* chronology is inconsistent */
      Dem_Error_RunTimeCheckFailed(DEM_MEMSTATE_IMPLEMENTATION_FILENAME, __LINE__);                                              /* SBSW_DEM_POINTER_RUNTIME_CHECK */
      break;
    }
#endif
  }

  Dem_MemState_MemorySetChronology(TranslatedMemoryId, (uint8)(Dem_MemState_MemoryGetCurrentSize(TranslatedMemoryId) - 1u), MemoryIndex); 
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_MemState_SortEntryIntoChronologicalList
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
Dem_MemState_SortEntryIntoChronologicalList(
  uint16 TranslatedMemoryId,
  CONST(Dem_Cfg_MemoryIndexType, AUTOMATIC)    MemoryIndex,
  CONST(uint32, AUTOMATIC)                     Timestamp
  )
{
  uint8 lSortedIndex;
  for (lSortedIndex = Dem_MemState_MemoryGetCurrentSize(TranslatedMemoryId);
        (lSortedIndex > 0u)  && (Dem_MemoryEntry_GetTimestamp(Dem_MemoryEntry_GetId(                                             /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
                                Dem_MemState_MemoryGetChronology(TranslatedMemoryId, (uint8)(lSortedIndex - 1u))))  > Timestamp);
       --lSortedIndex
    )
  {
    Dem_MemState_MemorySetChronology(
      TranslatedMemoryId,
      lSortedIndex,
      Dem_MemState_MemoryGetChronology(TranslatedMemoryId, (uint8)(lSortedIndex - 1u)));
  }

  Dem_MemState_MemorySetChronology(TranslatedMemoryId, lSortedIndex, MemoryIndex);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_MEMSTATE_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_MemState_Implementation.h
 *********************************************************************************************************************/
