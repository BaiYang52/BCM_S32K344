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
/*! \addtogroup Dem_Mar_Man
 *  \{
 *  \file       Dem_Man_Implementation.h
 *  \brief      Mar Man subcomponent.
 *  \details    Implementation of Man subcomponent which provides the functionality to calculate the MAN.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MAN_IMPLEMENTATION_H)
#define DEM_MAN_IMPLEMENTATION_H

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
#include "Dem_Man_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"
/* ------------------------------------------------------------------------- */


/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_MAN_IMPLEMENTATION_FILENAME "Dem_Man_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

#define DEM_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define DEM_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"    

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Mar_Man_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Mar_Man_GetNumerator
 *****************************************************************************/
/*!
 * \brief         Get value of MAN
 *
 * \details       Get value of MAN
 *
 * \param[in]     ManIdx
 *                Index of MAN which shall be retrieved.
 *
 * \return        MAN value
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Man_Type, DEM_CODE)
Dem_Mar_Man_GetNumerator(
  CONST(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, AUTOMATIC) ManIdx
);

/* ****************************************************************************
 * Dem_Mar_Man_GetNumeratorByEventId
 *****************************************************************************/
/*!
 * \brief         Get value of MAN by id of corresponding event
 *
 * \details       Get value of MAN by id of corresponding event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        MAN value
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Man_Type, DEM_CODE)
Dem_Mar_Man_GetNumeratorByEventId(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
);

/* ****************************************************************************
 * Dem_Mar_Man_SetNumerator
 *****************************************************************************/
/*!
 * \brief         Set value of MAN.
 *
 * \details       Set value of MAN.
 *
 * \param[in]     ManIdx
 *                Index of MAN which shall be stored.
 *
 *                Man
 *                Value of MAN which shall be stored.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Man_SetNumerator(
  CONST(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, AUTOMATIC) ManIdx,
  CONST(Dem_Man_Type, AUTOMATIC) Man
);

/* ****************************************************************************
 * Dem_Mar_Man_GetSizeOfIncrementedFlagTable
 *****************************************************************************/
/*!
 * \brief         Returns the size of the array which stores the MAN increment
 *                flags.
 *
 * \details       Returns the size of the array which stores the flags which
 *                indicate if the MAN is already incremented in the current 
 *                driving cycle.
 *
 * \return        Size of increment flag table.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, DEM_CODE)
Dem_Mar_Man_GetSizeOfIncrementedFlagTable(
  void
);

/* ****************************************************************************
 * Dem_Mar_Man_GetSizeOfManTable
 *****************************************************************************/
/*!
 * \brief         Returns the size of the array which stores the MANs
 *
 * \details       Returns the size of the array which stores the MANs
 *
 * \return        Size of MAN table
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, DEM_CODE)
Dem_Mar_Man_GetSizeOfManTable(
  void
);

/* ****************************************************************************
 * Dem_Mar_Man_SetManToBeIncrementFlagBitField
 *****************************************************************************/
/*!
 * \brief         Writes incremented flag array entry in the array
 *
 * \details       Writes incremented flag array entry in the array
 *
 * \param[in]     ManIncrementedIdx
 *                Index of incremented flag array
 *
 * \param[in]     ManIncremented
 *                Entry of incremented flag Array
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Man_SetManToBeIncrementFlagBitField(
  CONST(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, AUTOMATIC) ManIncrementFlagArrayIdx,
  CONST(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, AUTOMATIC) ManIncremented
);

/* ****************************************************************************
 * Dem_Mar_Man_TestManToBeIncrementedThisDcy
 *****************************************************************************/
/*!
 * \brief          Checks if given MAN was already incremented in the current
 *                 driving cycle.
 *
 * \details        Checks if given MAN was already incremented in the current
 *                 driving cycle.
 *
 * \param[in]      ManIdx
 *                 Index of MAN which shall be checked.
 *
 * \return         TRUE:  MAN of event is already incremented this driving cycle
 *                 FALSE: MAN of event is not incremented this driving cycle
 *
 * \pre            MAR is enabled
 *
 * \context        TASK
 * \synchronous    TRUE
 * \reentrant      TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Mar_Man_TestManToBeIncrementedThisDcy(
  CONST(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, AUTOMATIC) ManIdx
);

/* ****************************************************************************
 * Dem_Mar_Man_SetManToBeIncrementedThisDcy
 *****************************************************************************/
/*!
 * \brief         Sets flag which indicates that MAN was already incremented
 *                in this driving cycle.
 *
 * \details       Sets flag which indicates that MAN was already incremented
 *                in this driving cycle.
 * 
 * \param[in]     ManIdx
 *                Index the MAN where the incrementation flag shall be set.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Man_SetManToBeIncrementedThisDcy(
  CONST(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, AUTOMATIC) ManIdx
);

/* ****************************************************************************
 * Dem_Mar_Man_CalculateWorstNumeratorOfCombinedGroup
 *****************************************************************************/
/*!
 * \brief         Calculates the worst numerator of the combined group
 *
 * \details       Calculates the worst (minimal) numerator of the combined group
 *
 * \param[in]     GroupID
 *                Combined group identifier
 *
 * \return        MAN value
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Man_Type, DEM_CODE)
Dem_Mar_Man_CalculateWorstNumeratorOfCombinedGroup(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC) GroupId
);

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Mar_Man_GetNumerator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */ 
DEM_LOCAL_INLINE FUNC(Dem_Man_Type, DEM_CODE)
Dem_Mar_Man_GetNumerator(
  CONST(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, AUTOMATIC) ManIdx
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ManIdx)

#if (DEM_CFG_DATA_MONITOR_ACTIVITY_DATA == STD_ON)
  return (Dem_Man_Type)Dem_Cfg_ObdIumprData.MonitorActivityNumerator[ManIdx];                                            /* PRQA S 2843 */ /* MD_DEM_Dir4.1_ReadOperation */
#else
  return (Dem_Man_Type)0;
#endif
}


/* ****************************************************************************
 % Dem_Mar_Man_GetNumeratorByEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Man_Type, DEM_CODE)
Dem_Mar_Man_GetNumeratorByEventId(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
)
{

  Dem_Man_Type lMan;

  lMan = 0;

#if (DEM_CFG_DATA_MONITOR_ACTIVITY_DATA == STD_ON)
  {
    Dem_Cfg_MARIndexOfMonitorActivityRatioTableType lManIdx;
    lManIdx = Dem_Event_GetManIdx(EventId);
    lMan = (Dem_Man_Type)Dem_Cfg_ObdIumprData.MonitorActivityNumerator[lManIdx];                                                /* PRQA S 2843 */ /* MD_DEM_Dir4.1_ReadOperation */
  }
#endif

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)

  return lMan;
}

/* ****************************************************************************
 % Dem_Mar_Man_SetNumerator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Man_SetNumerator(
  CONST(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, AUTOMATIC) ManIdx,
  CONST(Dem_Man_Type, AUTOMATIC) Man
)
{
    if ((Dem_Error_isDevRunTimeChecksEnabled() == TRUE)
      && (ManIdx >= Dem_Mar_Man_GetSizeOfManTable()))
    {
      Dem_Error_RunTimeCheckFailed(DEM_MAN_IMPLEMENTATION_FILENAME, __LINE__);                                             /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    }
    else
    {
#if (DEM_CFG_DATA_MONITOR_ACTIVITY_DATA == STD_ON)
      Dem_Cfg_ObdIumprData.MonitorActivityNumerator[ManIdx] = (uint8)Man;                                                         /* SBSW_DEM_ARRAY_WRITE_MONITORACTIVITYNUMERATOR */
#else
      DEM_IGNORE_UNUSED_CONST_ARGUMENT(ManIdx)
      DEM_IGNORE_UNUSED_CONST_ARGUMENT(Man)
#endif
    }
}

/* ****************************************************************************
 % Dem_Mar_Man_GetSizeOfIncrementedFlagTable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, DEM_CODE)
Dem_Mar_Man_GetSizeOfIncrementedFlagTable(
  void
)
{
#if (DEM_CFG_DATA_MONITOR_ACTIVITY_DATA == STD_ON)                           
  return (Dem_Cfg_MARIndexOfMonitorActivityRatioTableType)DEM_CFG_MAX_NUMBER_OF_MAN_TO_BE_INCREMENTED_ENTRIES;                   /* PRQA S 2843 */ /* MD_DEM_Dir4.1_ReadOperation */
#else
  return (Dem_Cfg_MARIndexOfMonitorActivityRatioTableType)0;
#endif
}

/* ****************************************************************************
 % Dem_Mar_Man_GetSizeOfManTable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, DEM_CODE)
Dem_Mar_Man_GetSizeOfManTable(
  void
)
{
#if (DEM_CFG_DATA_MONITOR_ACTIVITY_DATA == STD_ON)
  return (Dem_Cfg_MARIndexOfMonitorActivityRatioTableType)DEM_CFG_MAX_NUMBER_OF_EVENTS_WITH_MAR;                                 /* PRQA S 2843 */ /* MD_DEM_Dir4.1_ReadOperation */
#else
  return (Dem_Cfg_MARIndexOfMonitorActivityRatioTableType)0;
#endif
}

/* ****************************************************************************
 % Dem_Mar_Man_SetManToBeIncrementFlagBitField
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Man_SetManToBeIncrementFlagBitField(
  CONST(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, AUTOMATIC) ManIncrementFlagArrayIdx,
  CONST(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, AUTOMATIC) ManIncremented
)
{
  if ((Dem_Error_isDevRunTimeChecksEnabled() == TRUE)
    && (ManIncrementFlagArrayIdx >= Dem_Mar_Man_GetSizeOfIncrementedFlagTable()))
  {
    Dem_Error_RunTimeCheckFailed(DEM_MAN_IMPLEMENTATION_FILENAME, __LINE__);                                             /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
  {
#if (DEM_CFG_DATA_MONITOR_ACTIVITY_DATA == STD_ON)
    Dem_Cfg_ObdIumprData.MonitorActivityNumeratorToBeIncrementedFlag[ManIncrementFlagArrayIdx] = (uint8)ManIncremented;          /* SBSW_DEM_ARRAY_WRITE_MONITORACTIVITYNUMERATORINCREMENTFLAG */
#else
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(ManIncrementFlagArrayIdx)                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(ManIncremented)                                                                               /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
  }
}

/* ****************************************************************************
 % Dem_Mar_Man_TestManToBeIncrementedThisDcy
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Mar_Man_TestManToBeIncrementedThisDcy(
  CONST(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, AUTOMATIC) ManIdx
)
{
  boolean lIsManAlreadyIncremented;

  lIsManAlreadyIncremented = FALSE;

#if (DEM_CFG_DATA_MONITOR_ACTIVITY_DATA == STD_ON)    
  lIsManAlreadyIncremented = Dem_Com_TestBitInBitfield(                                                                                                      
      &Dem_Cfg_ObdIumprData.MonitorActivityNumeratorToBeIncrementedFlag[0],                                                      /* SBSW_DEM_POINTER_FORWARD_STACK */
      Dem_Mar_Man_GetSizeOfIncrementedFlagTable(),
      (Dem_HandleType)ManIdx
    );
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ManIdx)
#endif

  return lIsManAlreadyIncremented;
}

/* ****************************************************************************
 % Dem_Mar_Man_SetManToBeIncrementedThisDcy
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Man_SetManToBeIncrementedThisDcy(
  CONST(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, AUTOMATIC) ManIdx
)
{
#if (DEM_CFG_DATA_MONITOR_ACTIVITY_DATA == STD_ON)    
  Dem_Com_SyncSetBitInBitfield(
    &Dem_Cfg_ObdIumprData.MonitorActivityNumeratorToBeIncrementedFlag[0],                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    Dem_Mar_Man_GetSizeOfIncrementedFlagTable(),
    (Dem_HandleType)ManIdx
  );
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ManIdx)
#endif
}

/* ****************************************************************************
 % Dem_Mar_Man_CalculateWorstNumeratorOfCombinedGroup
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
DEM_LOCAL_INLINE FUNC(Dem_Man_Type, DEM_CODE)
Dem_Mar_Man_CalculateWorstNumeratorOfCombinedGroup(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC) GroupId
)
{
  Dem_Cfg_CombinedGroupIterType lCombinedIter;
  Dem_Man_Type lMan;
  boolean lFirstAssignmentDone;


  lMan = 0u;
  lFirstAssignmentDone = FALSE;

  for (Dem_Cfg_CombinedGroupIterInit(GroupId, &lCombinedIter);                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                                     /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_EventIdType lSubEventId;

    lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */

    if (Dem_Event_TestEventUnavailable(lSubEventId) == FALSE)
    {
      Dem_Man_Type lManOfSubEvent;

      lManOfSubEvent = Dem_Mar_Man_GetNumeratorByEventId(lSubEventId);

      /* Calculate the minimum */
      if ((lFirstAssignmentDone == FALSE)
        || (lManOfSubEvent < lMan))
      {
        lMan = lManOfSubEvent;
        lFirstAssignmentDone = TRUE;
      }
    }
  }

  return lMan;
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
 * \addtogroup Dem_Mar_Smar_Ratio_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_Mar_Man_MarkNumeratorToBeIncremented
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Man_MarkNumeratorToBeIncremented(
  CONST(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, AUTOMATIC) ManIdx
)
{
  if ((Dem_Mar_Man_TestManToBeIncrementedThisDcy(ManIdx) == FALSE))
  {
    Dem_Mar_Man_SetManToBeIncrementedThisDcy(ManIdx);
  }
}

/* ****************************************************************************
 % Dem_Mar_Man_ResetAllNumerators
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Man_ResetAllNumerators(
  void
)
{
  Dem_Cfg_MARIndexOfMonitorActivityRatioTableType lManIndex;
  Dem_Cfg_MARIndexOfMonitorActivityRatioTableType lSizeOfManTable;

  lSizeOfManTable = Dem_Mar_Man_GetSizeOfManTable();

  for (lManIndex = 0u;
    lManIndex < lSizeOfManTable;                                                                                                 /* PRQA S 2994 */ /* MD_DEM_2994 */
    lManIndex++)
  {
    Dem_Mar_Man_SetNumerator(lManIndex, 0);
  }
}

/* ****************************************************************************
 % Dem_Mar_Man_CalculateWorstNumerator
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
DEM_LOCAL_INLINE FUNC(Dem_Man_Type, DEM_CODE)
Dem_Mar_Man_CalculateWorstNumerator(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
)
{
  Dem_Cfg_CombinedGroupIndexType lGroupId;
  Dem_Man_Type lMan;

  lGroupId = Dem_Cfg_EventCombinedGroup(EventId);

  if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    lMan = Dem_Mar_Man_CalculateWorstNumeratorOfCombinedGroup(lGroupId);
  }
  else
  {
    if (Dem_Event_TestEventUnavailable(EventId) == FALSE)
    {
      lMan = Dem_Mar_Man_GetNumeratorByEventId(EventId);
    }
    else
    {
      lMan = 0;
    }
  }

  return lMan;
}


/* ****************************************************************************
 % Dem_Mar_Man_IncrementNumeratorsIfQueued
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Man_IncrementNumeratorsIfQueued(
  void
)
{
  Dem_Cfg_MARIndexOfMonitorActivityRatioTableType lManIndex;
  Dem_Cfg_MARIndexOfMonitorActivityRatioTableType lSizeOfManTable;

  lSizeOfManTable = Dem_Mar_Man_GetSizeOfManTable();

  for (lManIndex = 0u;
    lManIndex < lSizeOfManTable;                                                                                                 /* PRQA S 2994 */ /* MD_DEM_2994 */
    lManIndex++)
  {
    Dem_Man_Type lMan;
    lMan = Dem_Mar_Man_GetNumerator(lManIndex);

    if ((Dem_Mar_Man_TestManToBeIncrementedThisDcy(lManIndex) == TRUE)
      && (lMan < DEM_MAN_MAX_VALUE))
    {
      Dem_Mar_Man_SetNumerator(lManIndex, lMan + 1u);
    }
  }
}

/* ****************************************************************************
 % Dem_Mar_Man_ResetToBeIncrementedFlags
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Man_ResetToBeIncrementedFlags(
  void
)
{
  Dem_Cfg_MARIndexOfMonitorActivityRatioTableType lManIncrementedTableIndex;
  Dem_Cfg_MARIndexOfMonitorActivityRatioTableType lSizeOfIncrementedFlagTable;

  lSizeOfIncrementedFlagTable = Dem_Mar_Man_GetSizeOfIncrementedFlagTable();

  for (lManIncrementedTableIndex = 0;
    lManIncrementedTableIndex < lSizeOfIncrementedFlagTable;                                                                     /* PRQA S 2994 */ /* MD_DEM_2994 */
    ++lManIncrementedTableIndex)
  {
    Dem_Mar_Man_SetManToBeIncrementFlagBitField(lManIncrementedTableIndex, 0);
  }
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_MAN_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Man_Implementation.h
 *********************************************************************************************************************/
