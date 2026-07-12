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
/*! \addtogroup Dem_Indicator
 *  \{
 *  \file       Dem_Indicator_Implementation.h
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

#if !defined (DEM_INDICATOR_IMPLEMENTATION_H)
#define DEM_INDICATOR_IMPLEMENTATION_H

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
#include "Dem_Indicator_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_GlobalStateIF_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_INDICATOR_IMPLEMENTATION_FILENAME "Dem_Indicator_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

#define DEM_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DEM_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 /* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_GlobalState_Indicator_PrivateProperties Private Properties
 * \{
 */

 /* ****************************************************************************
 * Dem_GlobalState_Indicator_IsIndicatorSupportEnabled
 *****************************************************************************/
 /*!
 * \brief         Tests if indicators are supported.
 *
 * \details       Tests if indicators are supported.
 *
 * \return        TRUE
 *                Indicators are supported.
 * \return        FALSE
 *                Indicators are not supported.
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalState_Indicator_IsIndicatorSupportEnabled(
  void
);

/* ****************************************************************************
 * Dem_GlobalState_Indicator_GetIndicatorEventCounters
 *****************************************************************************/
/*!
 * \brief         Get the current event counter for indicator
 *
 * \details       Read the stored value from IndicatorEventCounters table
 *
 * \param[in]     Index in IndicatorEventCounters table
 *
 * \return        Value at given index
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalState_Indicator_GetIndicatorEventCounters(
  CONST(uint16, AUTOMATIC)  Index
);

/* ****************************************************************************
 * Dem_GlobalState_Indicator_IncrementIndicatorEventCounter
 *****************************************************************************/
/*!
 * \brief         Increment the event counter for an indicator
 *
 * \details       Increment the event counter for the indicator at given index 
 *
 * \param[in]     Index of indicator in Indicator table and 
 *                IndicatorEventCounter table
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_Indicator_IncrementIndicatorEventCounter(
  CONST(uint16, AUTOMATIC)  Index
  );

/* ****************************************************************************
 * Dem_GlobalState_Indicator_DecrementIndicatorEventCounter
 *****************************************************************************/
/*!
 * \brief         Decrement the event counter for an indicator
 *
 * \details       Decrement the event counter for the indicator at given index 
 *
 * \param[in]     Index of indicator in Indicator table and 
 *                IndicatorEventCounter table
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_Indicator_DecrementIndicatorEventCounter(
  CONST(uint16, AUTOMATIC)  Index
  );

/* ****************************************************************************
 * Dem_GlobalState_Indicator_GetIndicatorIdOfIndicatorCounterTable
 *****************************************************************************/
/*!
 * \brief         Get the indicator ID of an indicator
 *
 * \details       Get the indicator ID for the indicator at given index 
 *
 * \param[in]     Index of indicator in Indicator counter table
 *
 * \return        Indicator ID
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalState_Indicator_GetIndicatorIdOfIndicatorCounterTable(
  CONST(Dem_GlobalState_Indicator_IndicatorCounterTableIndexType, AUTOMATIC)  Index
);

/* ****************************************************************************
 * Dem_GlobalState_Indicator_GetNodeIndexOfIndicatorTable
 *****************************************************************************/
/*!
 * \brief         Get the J1939 node of an indicator
 *
 * \details       Get the index of the J1939 node for which the indicator at 
 *                given index is configured
 *
 * \param[in]     Index of indicator in Indicator table
 *
 * \return        Index in J1939NodeId array
 *
 * \pre           -           
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalState_Indicator_GetNodeIndexOfIndicatorTable(
  CONST(uint16_least, AUTOMATIC)  Index
);

 /* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

 /* ****************************************************************************
 % Dem_GlobalState_Indicator_IsIndicatorSupportEnabled
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalState_Indicator_IsIndicatorSupportEnabled(
  void
)
{
  return (DEM_CFG_SUPPORT_INDICATORS == STD_ON);
}

 /* ****************************************************************************
 % Dem_GlobalState_Indicator_GetIndicatorEventCounters
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalState_Indicator_GetIndicatorEventCounters(
  CONST(uint16, AUTOMATIC)  Index
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)                                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return Dem_Cfg_GetIndicatorEventCounters(Index);
}

 /* ****************************************************************************
 % Dem_GlobalState_Indicator_IncrementIndicatorEventCounter
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_Indicator_IncrementIndicatorEventCounter(
  CONST(uint16, AUTOMATIC)  Index
  )
{
  if ((Dem_Error_isDevRunTimeChecksEnabled() == TRUE)
    && (Index >= Dem_Cfg_GetSizeOfIndicatorEventCounters()))                                                                                     
  {
    Dem_Error_RunTimeCheckFailed(DEM_INDICATOR_IMPLEMENTATION_FILENAME, __LINE__);                                               /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
  {
    Dem_Cfg_IncIndicatorEventCounters(Index);                                                                                    /* SBSW_DEM_ARRAY_WRITE_INDICATOREVENTCOUNTERS */
  }
}

 /* ****************************************************************************
 % Dem_GlobalState_Indicator_DecrementIndicatorEventCounter
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_Indicator_DecrementIndicatorEventCounter(
  CONST(uint16, AUTOMATIC)  Index
  )
{
  if ((Dem_Error_isDevRunTimeChecksEnabled() == TRUE)
    && (Index >= Dem_Cfg_GetSizeOfIndicatorEventCounters()))                                                                                     
  {
    Dem_Error_RunTimeCheckFailed(DEM_INDICATOR_IMPLEMENTATION_FILENAME, __LINE__);                                               /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
  {
    Dem_Cfg_DecIndicatorEventCounters(Index);                                                                                    /* SBSW_DEM_ARRAY_WRITE_INDICATOREVENTCOUNTERS */
  }
}

 /* ****************************************************************************
 % Dem_GlobalState_Indicator_GetIndicatorIdOfIndicatorCounterTable
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalState_Indicator_GetIndicatorIdOfIndicatorCounterTable(
  CONST(Dem_GlobalState_Indicator_IndicatorCounterTableIndexType, AUTOMATIC)  Index
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)                                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return Dem_Cfg_GetIndicatorIdOfIndicatorCounterTable(Index);
}

 /* ****************************************************************************
 % Dem_GlobalState_Indicator_GetNodeIndexOfIndicatorTable
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalState_Indicator_GetNodeIndexOfIndicatorTable(
  CONST(uint16_least, AUTOMATIC)  Index
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)                                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
  return Dem_Cfg_FindJ1939NodeIndexFromNmNodeId(Dem_Cfg_GetJ1939NodeIdOfIndicatorTable(Index));
#else
  return Dem_Cfg_GetInvalidJ1939NodeIndex();
#endif
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
 * \addtogroup Dem_GlobalState_Indicator_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalState_Indicator_GetGlobalMilIndicatorId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalState_Indicator_GetGlobalMilIndicatorId(                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  uint8 lResult;

#if (DEM_CFG_SUPPORT_MIL == STD_ON)
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
    lResult = DEM_CFG_GLOBAL_MIL_INDICATOR;
  }
  else
#endif
  {
    lResult = DEM_CFG_GLOBAL_INDICATOR_COUNT;
  }
  return lResult;
}


/* ****************************************************************************
 % Dem_GlobalState_Indicator_GetGlobalAwlIndicatorId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalState_Indicator_GetGlobalAwlIndicatorId(                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  void
  )
{
# if (DEM_CFG_SUPPORT_AWL == STD_ON)
  return DEM_CFG_GLOBAL_AWL_INDICATOR;
# else
  return DEM_CFG_GLOBAL_INDICATOR_COUNT;
# endif
}

/* ****************************************************************************
 % Dem_GlobalState_Indicator_GetGlobalPlIndicatorId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalState_Indicator_GetGlobalPlIndicatorId(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  void
  )
{
# if (DEM_CFG_SUPPORT_PL == STD_ON)
  return DEM_CFG_GLOBAL_PL_INDICATOR;
# else
  return DEM_CFG_GLOBAL_INDICATOR_COUNT;
# endif
}


/* ****************************************************************************
 % Dem_GlobalState_Indicator_GetGlobalRslIndicatorId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalState_Indicator_GetGlobalRslIndicatorId(                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  void
  )
{
# if (DEM_CFG_SUPPORT_RSL == STD_ON)
  return DEM_CFG_GLOBAL_RSL_INDICATOR;
# else
  return DEM_CFG_GLOBAL_INDICATOR_COUNT;
# endif
}

/* ****************************************************************************
 % Dem_GlobalState_Indicator_EventIndicatorIterGetIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_IndicatorIndexType, DEM_CODE)
Dem_GlobalState_Indicator_EventIndicatorIterGetIndex(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr                                                           /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return (Dem_Cfg_IndicatorIndexType)Dem_Cfg_GetIndicatorTableInd(IterPtr->mIdx);
}

 /* ****************************************************************************
 % Dem_GlobalState_Indicator_GetIndicatorIdOfIndicatorTable
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalState_Indicator_GetIndicatorIdOfIndicatorTable(
  CONST(uint16, AUTOMATIC)  Index
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)                                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return Dem_Cfg_GetIdOfIndicatorTable(Index);
}

 /* ****************************************************************************
 % Dem_GlobalState_Indicator_GetBehaviorOfIndicatorTable
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_GlobalState_Indicator_GetBehaviorOfIndicatorTable(
  CONST(uint16, AUTOMATIC)  Index
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)                                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return Dem_Cfg_GetBehaviorOfIndicatorTable(Index);
}

/* ****************************************************************************
 % Dem_GlobalState_Indicator_IsIndicatorIdValid
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalState_Indicator_IsIndicatorIdValid(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint8 IndicatorId
  )
{
  boolean lIsIndicatorIdValid = FALSE;
  if ( Dem_GlobalState_Indicator_IsIndicatorSupportEnabled() 
   && (IndicatorId < DEM_CFG_GLOBAL_INDICATOR_COUNT))
  {
    lIsIndicatorIdValid = TRUE;
  }
  return lIsIndicatorIdValid;
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
 * \defgroup Dem_GlobalState_Indicator_Private Private Methods
 * \{
 */
/* ****************************************************************************
 * Dem_GlobalState_Indicator_GetCounterIndexFromId
 *****************************************************************************/
/*!
 * \brief         Get index in indicator counter table 
 *
 * \details       Get index in indicator counter table for given indicator Id
 *
 * \param[in]     Indicator Id
 *
 * \param[in,out] CounterIndex
 *                Index in IndicatorCounter table referring to this indicator Id
 *
 * \return        TRUE
 *                Indicator Id was found in IndicatorCounter table
 *                
 * \return        FALSE
 *                No entry was found in IndicatorCounter table for this 
 *                Indicator Id
 *                
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalState_Indicator_GetCounterIndexFromId(
  CONST(uint8, AUTOMATIC)  IndicatorId,
  CONSTP2VAR(Dem_GlobalState_Indicator_IndicatorCounterTableIndexType, AUTOMATIC, AUTOMATIC) CounterIndex
  );


/* ****************************************************************************
 * Dem_GlobalState_Indicator_IndicatorCounterIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate special indicator descriptors
 *                configured for an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_Indicator_IndicatorCounterIterInit(
  CONST(uint8, AUTOMATIC)  IndicatorId,                                                                                          /* PRQA S 3206 */ /* MD_DEM_3206 */
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr                                                             /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_fp */
);

/* ****************************************************************************
 * Dem_GlobalState_Indicator_IsMilStateActive
 *****************************************************************************/
/*!
 * \brief         Decide whether Mil state is considered as active
 *
 * \details       Decide whether a given Mil state is considered as active
 *                according to configuration option
 *
 * \param[in]     MilState
 *                
 * \param[out]    TRUE: Mil state is considered as active
 *                FALSE: Mil state is considered as inactive
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalState_Indicator_IsMilStateActive(
  CONST(Dem_IndicatorStatusType, AUTOMATIC)  MilState
  );

/* ****************************************************************************
 * Dem_GlobalState_Indicator_IncrementAllCountersAssignedToEvent
 *****************************************************************************/
/*!
 * \brief        Increment all counters assigned to the event
 *
 * \details      -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_Indicator_IncrementAllCountersAssignedToEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
);

/* ****************************************************************************
 * Dem_GlobalState_Indicator_ResetAllEventCounters
 *****************************************************************************/
/*!
 * \brief         Reset all event counters
 *
 * \details       Reset all event counters
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_Indicator_ResetAllEventCounters(void);

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_GlobalState_Indicator_GetCounterIndexFromId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalState_Indicator_GetCounterIndexFromId(
  CONST(uint8, AUTOMATIC)  IndicatorId,
  CONSTP2VAR(Dem_GlobalState_Indicator_IndicatorCounterTableIndexType, AUTOMATIC, AUTOMATIC) CounterIndex
  )
{
  boolean lIndicatorIdFound = FALSE;
  Dem_GlobalState_Indicator_IndicatorCounterTableIndexType lIndex;

  for (lIndex = 0; lIndex < Dem_Cfg_GetSizeOfIndicatorCounterTable(); lIndex++)                                                  /* PRQA S 2994 */ /* MD_DEM_2994 */
  {
    if (Dem_GlobalState_Indicator_GetIndicatorIdOfIndicatorCounterTable(lIndex) == IndicatorId)
    {
      *CounterIndex = lIndex;                                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      lIndicatorIdFound = TRUE;
      break;
    }
  }
  return lIndicatorIdFound;
}


/* ****************************************************************************
 % Dem_GlobalState_Indicator_IndicatorCounterIterInit
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
Dem_GlobalState_Indicator_IndicatorCounterIterInit(
  CONST(uint8, AUTOMATIC)  IndicatorId,                                                                                         
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr                                                            
  )
{
  Dem_GlobalState_Indicator_IndicatorCounterTableIndexType lIndicatorIndex = 0;
  if (Dem_GlobalState_Indicator_GetCounterIndexFromId(IndicatorId, &lIndicatorIndex) == TRUE)                                    /* SBSW_DEM_POINTER_FORWARD_STACK */
  {
    Dem_Cfg_ComplexIterInit(IterPtr, Dem_Cfg_GetIndicatorTableStartIdxOfIndicatorCounterTable(lIndicatorIndex),                  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
       Dem_Cfg_GetIndicatorTableEndIdxOfIndicatorCounterTable(lIndicatorIndex));
  }
  else
  {
    Dem_Cfg_ComplexIterInit(IterPtr, 0,0);                                                                                       /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_GlobalState_Indicator_IsMilStateActive
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalState_Indicator_IsMilStateActive(
  CONST(Dem_IndicatorStatusType, AUTOMATIC)  MilState
  )
{
  boolean lReturnValue;
  lReturnValue = FALSE;

# if (DEM_CFG_REPORT_BLINKING_MIL_AS_ACTIVE == STD_ON)
  if ((MilState != DEM_INDICATOR_OFF))
#else
  if ((MilState == DEM_INDICATOR_CONTINUOUS) || (MilState == DEM_INDICATOR_BLINK_CONT))
# endif
  {
    lReturnValue = TRUE;
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalState_Indicator_IncrementAllCountersAssignedToEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_Indicator_IncrementAllCountersAssignedToEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Cfg_ComplexIterType lIndicatorIter;

  for (Dem_Core_Event_IndicatorIterInit(EventId, &lIndicatorIter);                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterExists(&lIndicatorIter) == TRUE;                                                                        /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterNext(&lIndicatorIter))                                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_GlobalState_Indicator_IncrementIndicatorEventCounter(
      Dem_GlobalState_Indicator_EventIndicatorIterGetIndex(&lIndicatorIter));                                      /* SBSW_DEM_POINTER_FORWARD_STACK */
  }
}

/* ****************************************************************************
 % Dem_GlobalState_Indicator_ResetAllEventCounters
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_Indicator_ResetAllEventCounters(void)
{
  /* If indicators are configured, initialize all indicator state counters */
  Dem_Cfg_IndicatorTableIndType lIndicatorIndex;
  for (lIndicatorIndex = 0u; lIndicatorIndex < Dem_Cfg_GetSizeOfIndicatorTable(); ++lIndicatorIndex)                             /* PRQA S 2994 */ /* MD_DEM_2994 */
  {
    Dem_Cfg_SetIndicatorEventCounters(lIndicatorIndex, 0);                                                                       /* SBSW_DEM_ARRAY_WRITE_INDICATOREVENTCOUNTERS */
  }
}

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
 * \addtogroup Dem_GlobalState_Indicator_Public
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalState_Indicator_GetIndicatorState
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
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_GlobalState_Indicator_GetIndicatorState(
  CONST(uint8, AUTOMATIC)  IndicatorId,
  CONST(uint8, AUTOMATIC)  NodeIndex,
  CONST(boolean, AUTOMATIC) NodeIndependent
  )
{
  Dem_IndicatorStatusType lIndicatorState;
  lIndicatorState = DEM_INDICATOR_OFF;

  if (Dem_GlobalState_Indicator_IsIndicatorSupportEnabled())
  {
    Dem_Cfg_ComplexIterType lCounterIter;

    for (Dem_GlobalState_Indicator_IndicatorCounterIterInit(IndicatorId, &lCounterIter);                                         /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIterExists(&lCounterIter) == TRUE;                                                                          /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIterNext(&lCounterIter))                                                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      if ((Dem_GlobalState_Indicator_GetNodeIndexOfIndicatorTable(lCounterIter.mIdx) == NodeIndex) || (NodeIndependent == TRUE))
      {
        if (Dem_GlobalState_Indicator_GetIndicatorEventCounters((uint16)(lCounterIter.mIdx)) > 0u)
        {
          if (lIndicatorState == DEM_INDICATOR_OFF)
          {
            lIndicatorState = Dem_GlobalState_Indicator_GetBehaviorOfIndicatorTable((uint16)(lCounterIter.mIdx));
          }
          else if ((lIndicatorState == DEM_INDICATOR_CONTINUOUS)                                                                 /* COV_DEM_GETINDICATORSTATE_ROBUSTNESS */
                && (Dem_Cfg_GetBehaviorOfIndicatorTable(lCounterIter.mIdx) == DEM_INDICATOR_BLINKING))
          {
            lIndicatorState = DEM_INDICATOR_BLINK_CONT;
          }
          else
          {
            /* Do nothing, Misra case */
          }

          if (lIndicatorState != DEM_INDICATOR_CONTINUOUS)
          {
            /* The counters are sorted by the gravity of their behavior, so the first
              positive counter found determines the behavior of the indicator lamp,
              unless it is CONTINUOUS, in this case we need to continue the search to make
              sure there is no counter with BLINKING behavior, because this results in
              BLINK_CONT. */
            break;
          }
        }
      }
    }

  } /* IsIndicatorSupportEnabled */
  return lIndicatorState;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_GlobalState_Indicator_ProcessEventWirEnabled
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalState_Indicator_ProcessEventWirEnabled(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  if (Dem_GlobalState_Indicator_IsIndicatorSupportEnabled())                                                                     /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    Dem_Cfg_ComplexIterType lIndicatorIter;
    boolean lOldMilIsActive;
    lOldMilIsActive = FALSE;

    for (Dem_Core_Event_IndicatorIterInit(EventId, &lIndicatorIter);                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIterExists(&lIndicatorIter) == TRUE;                                                                        /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIterNext(&lIndicatorIter))                                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      if (Dem_GlobalState_Indicator_GetIndicatorIdOfIndicatorTable(
           Dem_GlobalState_Indicator_EventIndicatorIterGetIndex((&lIndicatorIter)))                                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
          == Dem_GlobalState_Indicator_GetGlobalMilIndicatorId())                   
      {
        if (Dem_GlobalState_Indicator_GetIndicatorEventCounters(
              Dem_GlobalState_Indicator_EventIndicatorIterGetIndex(&lIndicatorIter)) == 0u)                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
        {
          /* counter will increment from 0->1, this is a possible state change */
          lOldMilIsActive = Dem_GlobalState_Indicator_TestMilReportedActive();
        }
      }

      Dem_GlobalState_Indicator_IncrementIndicatorEventCounter(
        Dem_GlobalState_Indicator_EventIndicatorIterGetIndex(&lIndicatorIter));                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */

      if (Dem_GlobalState_Indicator_GetIndicatorIdOfIndicatorTable(
           Dem_GlobalState_Indicator_EventIndicatorIterGetIndex((&lIndicatorIter))) 
          == Dem_GlobalState_Indicator_GetGlobalMilIndicatorId())                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        if ((Dem_GlobalState_Indicator_GetIndicatorEventCounters(
             Dem_GlobalState_Indicator_EventIndicatorIterGetIndex(&lIndicatorIter)) == 1u)                                       /* SBSW_DEM_CALL_ITERATOR_POINTER */
          && (lOldMilIsActive != Dem_GlobalState_Indicator_TestMilReportedActive()))
        {
          /* counter has incremented from 0->1, process the state change */
          Dem_GlobalDiagnostics_GlobalState_ProcessMilStateEnabled();
        }
      }
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalState_Indicator_ProcessEventWirDisabled
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalState_Indicator_ProcessEventWirDisabled(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  if (Dem_GlobalState_Indicator_IsIndicatorSupportEnabled())
  {
    Dem_Cfg_ComplexIterType lIndicatorIter;
    boolean lOldMilIsActive;
    lOldMilIsActive = FALSE;

    for (Dem_Core_Event_IndicatorIterInit(EventId, &lIndicatorIter);                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIterExists(&lIndicatorIter) == TRUE;                                                                        /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIterNext(&lIndicatorIter))                                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      if (Dem_GlobalState_Indicator_GetIndicatorIdOfIndicatorTable(
           Dem_GlobalState_Indicator_EventIndicatorIterGetIndex((&lIndicatorIter)))                                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
          == Dem_GlobalState_Indicator_GetGlobalMilIndicatorId())
      {
        if (Dem_GlobalState_Indicator_GetIndicatorEventCounters(
            Dem_GlobalState_Indicator_EventIndicatorIterGetIndex(&lIndicatorIter)) == 1u)                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
        {
          /* counter will increment from 1->0, this is a possible state change */
          lOldMilIsActive = Dem_GlobalState_Indicator_TestMilReportedActive();
        }
      }
      
      
      Dem_GlobalState_Indicator_DecrementIndicatorEventCounter(
        Dem_GlobalState_Indicator_EventIndicatorIterGetIndex(&lIndicatorIter));                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */

      if (Dem_GlobalState_Indicator_GetIndicatorIdOfIndicatorTable(
        Dem_GlobalState_Indicator_EventIndicatorIterGetIndex((&lIndicatorIter)))                                                 /* SBSW_DEM_CALL_ITERATOR_POINTER */
        == Dem_GlobalState_Indicator_GetGlobalMilIndicatorId())
      {
        if ((Dem_GlobalState_Indicator_GetIndicatorEventCounters(
          Dem_GlobalState_Indicator_EventIndicatorIterGetIndex(&lIndicatorIter)) == 0u)                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
          && (lOldMilIsActive != Dem_GlobalState_Indicator_TestMilReportedActive()))
        {
          /* counter has incremented from 0->1, process the state change */
          Dem_GlobalDiagnostics_GlobalState_ProcessMilStateDisabled();
        }
      }
    }
  }
}                                                                                                                                 /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalState_Indicator_Init
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
Dem_GlobalState_Indicator_Init(void)
{
  if (Dem_GlobalState_Indicator_IsIndicatorSupportEnabled())
  {
    Dem_EventIdType lEventId;

    Dem_GlobalState_Indicator_ResetAllEventCounters();

    for (lEventId = DEM_EVENT_FIRST; lEventId < Dem_Cfg_GlobalEventCount(); ++lEventId)
    {
      if (Dem_Event_TestEventAvailable(lEventId) == TRUE)
      {
        Dem_UDSStatus_Type lExternalEventStatus = Dem_Event_GetExternalUDSStatus(lEventId, Dem_Event_GetUDSStatus(lEventId));

        if (Dem_UDSStatus_Test_WIR(lExternalEventStatus) == TRUE)
        {
          Dem_GlobalState_Indicator_IncrementAllCountersAssignedToEvent(lEventId);
        }
      }
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


/* ****************************************************************************
 % Dem_GlobalState_Indicator_TestMilReportedActive
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalState_Indicator_TestMilReportedActive(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  boolean lReturnValue;
  lReturnValue = FALSE;

  if (Dem_GlobalState_Indicator_IsIndicatorSupportEnabled())                                                                     /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    lReturnValue = Dem_GlobalState_Indicator_IsMilStateActive(Dem_GlobalState_Indicator_GetIndicatorState(Dem_GlobalState_Indicator_GetGlobalMilIndicatorId(), 0, TRUE));
  }

  return lReturnValue;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_INDICATOR_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Indicator_Implementation.h
 *********************************************************************************************************************/
