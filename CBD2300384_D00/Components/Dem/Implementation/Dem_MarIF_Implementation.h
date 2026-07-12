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
/*! \addtogroup Dem_Mar_MarIF
 *  \{
 *  \file       Dem_MarIF_Implementation.h
 *  \brief      Mar MarIF subcomponent.
 *  \details    Implementation of Smar subcomponent which provides the functionality to calculate and retrieve MARs.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MARIF_IMPLEMENTATION_H)
#define DEM_MARIF_IMPLEMENTATION_H

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
#include "Dem_MarIF_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_Man_Interface.h"
#include "Dem_Mad_Interface.h"
#include "Dem_Smar_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_MARIF_IMPLEMENTATION_FILENAME "Dem_MarIF_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

#define DEM_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Number of currently OBD related events which are pending */
DEM_LOCAL VAR(uint16, DEM_VAR_NOINIT) Dem_Mar_NumOfPendingEvents;

#define DEM_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Mar_MarIF_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Mar_TestNoObdRelevantEventPending
 *****************************************************************************/
/*!
 * \brief         Checks if any OBD relevant event is pending
 *
 * \details       Checks if any OBD relevant event is pending
 *
 * \return        TRUE:  MAN/MAD can be incremented.
 *                FALSE: MAN/MAD can not be incremented.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Mar_TestNoObdRelevantEventPending(
  void
);

/* ****************************************************************************
 * Dem_Mar_TransferManToSmar
 *****************************************************************************/
/*!
 * \brief         Transfers all MANs to the corresponding SMAR
 *
 * \details       Transfers all MANS to the corresponding SMAR 
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_TransferManToSmar(
  void
);

/* ****************************************************************************
 * Dem_Mar_IncrementPendingEvents
 *****************************************************************************/
/*!
 * \brief         Increments the counter which counts the number of pending
 *                MIL illuminating events
 *
 * \details       Increments the counter which counts the number of pending
 *                MIL illuminating events
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_IncrementPendingEvents(void);

/* ****************************************************************************
 * Dem_Mar_DecrementPendingEvents
 *****************************************************************************/
/*!
 * \brief         Decrements the counter which counts the number of pending
 *                MIL illuminating events
 *
 * \details       Decrements the counter which counts the number of pending
 *                MIL illuminating events
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_DecrementPendingEvents(void);


/* ****************************************************************************
 * Dem_Mar_ProcessNumeratorToBeIncrementedFlags
 *****************************************************************************/
/*!
 * \brief         Process the to be incremented flags for numerators
 *
 * \details       Increments the numerators if incrementation is allowed and queued
 *                and reset the flags
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_ProcessNumeratorToBeIncrementedFlags(
  void
);

/* ****************************************************************************
 * Dem_Mar_ProcessDenominatorIncrementFlag
 *****************************************************************************/
/*!
 * \brief         Process the increment flag for denominator
 *
 * \details       Increments the denominator if incrementation is allowed and queued then
 *                reset the flag
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_ProcessDenominatorIncrementFlag(
  void
);


/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Mar_TestNoObdRelevantEventPending
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Mar_TestNoObdRelevantEventPending(
  void
)
{
  return (boolean)(Dem_Mar_NumOfPendingEvents == 0u);
}

/* ****************************************************************************
 % Dem_Mar_TransferManToSmar
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
Dem_Mar_TransferManToSmar(
  void
)
{
  Dem_EventIdType lEventId;

  /* Iterate over primary memory events. */
  for (lEventId = Dem_MemState_MemoryGetFirstEvent(DEM_CFG_MEMORYID_PRIMARY);
       lEventId <= Dem_MemState_MemoryGetLastEvent(DEM_CFG_MEMORYID_PRIMARY);
       lEventId++)
  {
    /* If the event has a MAR ratio assigned */
    if (Dem_Event_GetManIdx(lEventId) != DEM_CFG_MONITOR_ACTIVITY_RATIO_INDEX_INVALID)
    {
      /* if event is single event or master of a combined group */
      if (lEventId == Dem_Cfg_GetMasterEvent(lEventId))
      {
        /* Set SMAR to the worst numerator value of the event (group) */
        Dem_Smar_Type lSmar;
        lSmar = (Dem_Smar_Type)Dem_Mar_Man_CalculateWorstNumerator(lEventId);
        Dem_Mar_Smar_SetSmarByEventId(lEventId, lSmar);
      }
    }
  }
}

/* ****************************************************************************
 % Dem_Mar_IncrementPendingEvents
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_IncrementPendingEvents(void)
{
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_Mar_NumOfPendingEvents >= Dem_Cfg_GlobalEventCount())
  {
    Dem_Error_RunTimeCheckFailed(DEM_MARIF_IMPLEMENTATION_FILENAME, __LINE__);                                                   /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#endif
  {
    ++Dem_Mar_NumOfPendingEvents;
  }
}

/* ****************************************************************************
 % Dem_Mar_DecrementPendingEvents
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_DecrementPendingEvents(void)
{
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_Mar_NumOfPendingEvents == (uint16)0)
  {
    Dem_Error_RunTimeCheckFailed(DEM_MARIF_IMPLEMENTATION_FILENAME, __LINE__);                                                   /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#endif
  {
    --Dem_Mar_NumOfPendingEvents;
  }
}


/* ****************************************************************************
 % Dem_Mar_ProcessNumeratorToBeIncrementedFlags
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_ProcessNumeratorToBeIncrementedFlags(
  void
)
{
  if (Dem_Mar_TestNoObdRelevantEventPending() == TRUE)
  {
    Dem_Mar_Man_IncrementNumeratorsIfQueued();
  }
    
  Dem_Mar_Man_ResetToBeIncrementedFlags();
}

/* ****************************************************************************
 % Dem_Mar_ProcessDenominatorIncrementFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_ProcessDenominatorIncrementFlag(
  void
)
{
  if (Dem_Mar_TestNoObdRelevantEventPending() == TRUE)
  {
    Dem_Mar_Mad_IncrementDenominatorIfQueued();
  }
  Dem_Mar_Mad_ResetIncrementFlag();
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
 * \addtogroup Dem_Mar_MarIF_PublicProperties
 * \{
 */


/* ****************************************************************************
 % Dem_Mar_InitNv
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
Dem_Mar_InitNv(
  void
)
{
  if (Dem_Mar_IsEnabled() == TRUE)
  {
    Dem_Mar_Man_ResetAllNumerators();
    Dem_Mar_Man_ResetToBeIncrementedFlags();
    Dem_Mar_Mad_Init();
    Dem_Mar_Smar_ResetAllSmars();
  }
}

/* ****************************************************************************
 % Dem_Mar_MarkNumeratorToBeIncremented
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
Dem_Mar_MarkNumeratorToBeIncremented(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
)
{
  if (Dem_Mar_IsEnabled() == TRUE)
  {
    Dem_Cfg_MARIndexOfMonitorActivityRatioTableType lManIdx;

    lManIdx = Dem_Event_GetManIdx(EventId);

    if (lManIdx != DEM_CFG_MONITOR_ACTIVITY_RATIO_INDEX_INVALID)
    {
      Dem_Mar_Man_MarkNumeratorToBeIncremented(lManIdx);
    }
  }
}


/* ****************************************************************************
 % Dem_Mar_CountPendingEvents
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_CountPendingEvents(
  void
)
{
  if (Dem_Mar_IsEnabled() == TRUE)
  {
    Dem_EventIdType lEventId;
    uint16 lEventCounter;

    lEventCounter = 0;

    for (lEventId = Dem_MemState_MemoryGetFirstEvent(DEM_CFG_MEMORYID_PRIMARY);
         lEventId <= Dem_MemState_MemoryGetLastEvent(DEM_CFG_MEMORYID_PRIMARY);
         lEventId++)
    {
      if ((Dem_Core_DTC_IsObdRelated(lEventId) == TRUE)
        && (Dem_Core_Event_TestMilAssigned(lEventId) == TRUE)
        && (Dem_UDSStatus_Test_PDTC(Dem_Event_GetUDSStatus(lEventId)) == TRUE))
      {
        ++lEventCounter;
      }
    }

    Dem_Mar_NumOfPendingEvents = lEventCounter;
  }
}




/* ****************************************************************************
 % Dem_Mar_IncrementPendingEventsIfNeeded
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_IncrementPendingEventsIfNeeded(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
)
{
  if (Dem_Mar_IsEnabled() == TRUE)
  {
    if ((Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
      && (Dem_Core_Event_TestMilAssigned(EventId) == TRUE)
      && (Dem_UDSStatus_Test_PDTC(Dem_Event_GetUDSStatus(EventId)) == FALSE))
    {
      Dem_Mar_IncrementPendingEvents();
    }
  }
}


/* ****************************************************************************
 % Dem_Mar_DecrementPendingEventsIfNeeded
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_DecrementPendingEventsIfNeeded(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
)
{
  if (Dem_Mar_IsEnabled() == TRUE)
  {
    if ((Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
      && (Dem_Core_Event_TestMilAssigned(EventId) == TRUE)
      && (Dem_UDSStatus_Test_PDTC(Dem_Event_GetUDSStatus(EventId)) == TRUE))
    {
      Dem_Mar_DecrementPendingEvents();
    }
  }
}

/* ****************************************************************************
 % Dem_Mar_QueueDenominatorIncrementation
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_QueueDenominatorIncrementation(
  void
) 
{
  if (Dem_Mar_IsEnabled() == TRUE)
  {
    Dem_Mar_Mad_QueueIncrement();
  }
}

/* ****************************************************************************
 % Dem_Mar_RestartDcy
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
Dem_Mar_RestartDcy(
  void
)
{
  if (Dem_Mar_IsEnabled() == TRUE)
  {
    Dem_Mad_Type lMad;

    Dem_Mar_ProcessNumeratorToBeIncrementedFlags();
    Dem_Mar_ProcessDenominatorIncrementFlag();

    lMad = Dem_Mar_Mad_GetDenominator();
    if (lMad == DEM_MAD_MAX_VALUE)
    {
      Dem_Mar_TransferManToSmar();
      Dem_Mar_Man_ResetAllNumerators();
      Dem_Mar_Mad_ResetDenominator();
    }
  }
}

/* ****************************************************************************
 % Dem_Mar_IsEnabled()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Mar_IsEnabled(void)
{
  return (boolean)(DEM_CFG_DATA_MONITOR_ACTIVITY_DATA == STD_ON);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_MARIF_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_MarIF_Implementation.h
 *********************************************************************************************************************/
