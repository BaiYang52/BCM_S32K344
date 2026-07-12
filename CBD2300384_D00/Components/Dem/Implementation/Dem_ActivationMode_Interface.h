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
 *  \defgroup   Dem_ActivationMode ActivationMode
 *  \{
 *  \file       Dem_ActivationMode_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Manages the User and Special indicator states
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_ACTIVATIONMODE_INTERFACE_H)
#define DEM_ACTIVATIONMODE_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Dem_Int.h"
                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_ActivationMode_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_ActivationMode_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_ActivationMode_B1Counter_GetValue
 *****************************************************************************/
/*!
 * \brief          Get the WWH OBD B1 counter value
 *
 * \details        -
 *
 * \return         B1 counter value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_ActivationMode_B1Counter_GetValue(
  void
);

/* ****************************************************************************
 * Dem_ActivationMode_ContinuousMiCounter_GetValue
 *****************************************************************************/
/*!
 * \brief        Get the WWH OBD continuous Mi counter value
 *
 * \details      -
 *
 * \return       Continuous Mi counter value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_ActivationMode_ContinuousMiCounter_GetValue(
  void
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_ActivationMode_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalState_ActivationMode_Init
 *****************************************************************************/
/*!
 * \brief         Initializes Unit ActivationMode 
 *
 * \details       Initialize the activation mode of the ECU based on the status
 *                of all events available in this configuration variant.
 *
 *
 * \pre           Event status has been restored before
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_ActivationMode_Init(
  void
  );

/* ****************************************************************************
 * Dem_GlobalState_ActivationMode_InitAdminData
 *****************************************************************************/
/*!
 * \brief         Initializes activation mode counter stored in AdminData block
 *
 * \details       Initialize the B1Counter and ContinuousMI counter and their
 *                respective Healing and Aging counters
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_ActivationMode_InitAdminData(
  void
);

/* ****************************************************************************
 * Dem_ActivationMode_ContinuousMiCounter_InitCounters
 *****************************************************************************/
/*!
 * \brief         Initializes continuous Mi counters
 *
 * \details       Initializes continuous Mi counter related counters
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ActivationMode_ContinuousMiCounter_InitCounters(
  void
);
/* ****************************************************************************
 * Dem_GlobalState_ActivationMode_ProcessEngineRunTime
 *****************************************************************************/
/*!
 * \brief         Process EngineRunTime update
 *
 * \details       If WwhObd is enabled, update B1 counter and continuous Mi 
 *                counter and process activation mode change
 *
 * \param[in]     EngineRunTimeTicks
 *                Current engine run time since engine start
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_ActivationMode_ProcessEngineRunTime(
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
);

/* ****************************************************************************
 * Dem_GlobalState_ActivationMode_IsEventAndActivationMode4Active
 *****************************************************************************/
/*!
 * \brief         Tests if with currently processed event activation mode 4 
 *                gets active
 *
 * \details       Tests if given event has active state and activation mode 4 
 *                is active
 *
 * \param[in]    EventId
 *               Unique handle of the event
 *
 * \return        TRUE: The given event is active and either this leads to 
 *                      activation mode 4 being active or activation mode 4 
 *                      is active because of the B1Counter exceeding 200h
 *                FALSE: - Event is not active or 
 *                       - Event is active but neither is of Severity Class A 
 *                         nor is the B1 counter exceeding 200h
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalState_ActivationMode_IsEventAndActivationMode4Active(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
);

/* ****************************************************************************
 * Dem_GlobalState_ActivationMode_GetMILStatus
 *****************************************************************************/
/*!
 * \brief         Gets the current indicator status.
 *
 * \details       Gets the indicator status resulting from current Activation 
 *                Mode.
 *
 * \return        The current indicator status
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_GlobalState_ActivationMode_GetMILStatus(
  void
  );

/* ****************************************************************************
 * Dem_GlobalState_ActivationMode_RestoreCounters
 *****************************************************************************/
/*!
 * \brief         Restore WWHOBD counters
 *
 * \details       Restore WWHOBD counters during initialization
 *
 * \pre           May be called only during module initialization
 *                Must be called after Dem_GlobalState_ActivationMode_Init
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_ActivationMode_RestoreCounters(
  void
);

/* ****************************************************************************
 * Dem_GlobalState_ActivationMode_ProcessWirEnabled
 *****************************************************************************/
/*!
 * \brief         Updates WWH OBD statistics on an event WIR bit ON trigger
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique identifier of the event   
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_ActivationMode_ProcessWirEnabled(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_GlobalState_ActivationMode_ProcessWirDisabled
 *****************************************************************************/
/*!
 * \brief         Updates WWH OBD statistics on an event WIR bit OFF trigger
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique identifier of the event   
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_ActivationMode_ProcessWirDisabled(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_GlobalState_ActivationMode_ProcessQualifiedActiveEvent
 *****************************************************************************/
/*!
 * \brief         Processes the increment of the qualified active event count
 *
 * \details       Increments of the qualified active event count and processes 
 *                a resulting transition to activation mode 4
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventId
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalState_ActivationMode_ProcessQualifiedActiveEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_GlobalState_ActivationMode_ProcessDCYEnd
 *****************************************************************************/
/*!
 * \brief         Processes a driving cycle stop trigger
 *
 * \details       Updates WWH OBD statistics on a driving cycle stop trigger
 *
 * \pre           Event specific trigger on Operation cycle end are already 
 *                processed
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_ActivationMode_ProcessDCYEnd(
  void
);

/* ****************************************************************************
 * Dem_GlobalState_ActivationMode_ProcessWUCEnd
 *****************************************************************************/
/*!
 * \brief         Processes a warm up cycle stop trigger
 *
 * \details       Updates WWH OBD statistics on a warm up cycle stop trigger
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_ActivationMode_ProcessWUCEnd(
  void
);

/* ****************************************************************************
 * Dem_GlobalState_ActivationMode_ProcessEventPassed
 *****************************************************************************/
/*!
 * \brief         Processes reset of TF bit
 *
 * \details       Updates qualified active event counts and processes 
 *                a resulting activation mode change
 *
 * \param[in]     EventContext
 *                Event processing context
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_ActivationMode_ProcessEventPassed(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
  );

/* ****************************************************************************
 * Dem_GlobalState_ActivationMode_ProcessEventCleared
 *****************************************************************************/
/*!
 * \brief         Processes reset of Event status
 *
 * \details       Updates status related event counts and processes a 
 *                resulting activation mode change
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_ActivationMode_ProcessEventCleared(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
);

/* ****************************************************************************
 * Dem_ActivationMode_B1Counter_ProcessMalfunctionDetected
 *****************************************************************************/
/*!
 * \brief         Processes possible malfunction for an event
 *
 * \details       Marks this DCY with malfunction detected, if the event is 
 *                a WWHOBD Class B1 Event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ActivationMode_B1Counter_ProcessMalfunctionDetected(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
);

/* ****************************************************************************
 * Dem_GlobalState_ActivationMode_ProcessAfterClearAll
 *****************************************************************************/
/*!
 * \brief         Reset activation mode data after clear all
 *
 * \details       Reset activation mode data after clear all
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_ActivationMode_ProcessAfterClearAll(
  void
);


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_ACTIVATIONMODE_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ActivationMode_Interface.h
 *********************************************************************************************************************/
