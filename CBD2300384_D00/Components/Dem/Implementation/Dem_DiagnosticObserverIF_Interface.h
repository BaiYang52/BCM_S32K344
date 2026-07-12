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
 *  \defgroup   Dem_DiagnosticObserverIF DiagnosticObserverIF
 *  \{
 *  \file       Dem_DiagnosticObserverIF_Interface.h
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

#if !defined (DEM_DIAGNOSTICOBSERVERIF_INTERFACE_H)
#define DEM_DIAGNOSTICOBSERVERIF_INTERFACE_H

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
/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_DiagnosticObserverIF_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_DiagnosticObserver_ProcessOBDCycleRestart
 *****************************************************************************/
/*!
 * \brief         Process operations on OBD Driving Cycle Restart.
 *
 * \details       Process operations on OBD Driving Cycle Restart.
 *
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DiagnosticObserver_ProcessOBDCycleRestart(
  void
  );

/* ****************************************************************************
 * Dem_DiagnosticObserver_ProcessEventFailed
 *****************************************************************************/
/*!
 * \brief         Process event triggers on 'Failed' status report.
 *
 * \details       Process event's warning indicator requests and qualified 
 *                active event count
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
Dem_DiagnosticObserver_ProcessEventFailed(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
  );


/* ****************************************************************************
 * Dem_DiagnosticObserver_ProcessEventPassed
 *****************************************************************************/
/*!
 * \brief         Process event effects on 'Passed' status report.
 *
 * \details       Process event effects on 'Passed' status report if WwhObd 
 *                is enabled.
 *
 * \param[in]     EventContext
 *                Event processing context
 * \param[in]     DTCContext
 *                DTC processing context
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DiagnosticObserver_ProcessEventPassed(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext,
  CONSTP2CONST(Dem_DTC_ContextType, AUTOMATIC, AUTOMATIC) DTCContext
  );

/* ****************************************************************************
 * Dem_DiagnosticObserver_ProcessEventCleared
 *****************************************************************************/
/*!
 * \brief         Process event effects on Clear
 *
 * \details       Process Dtr values and warning indicator requests
 *
 * \param[in]     EventId
 * \param[in]     OldEventStatus
 *
 * \pre           Has to be called before qualification status is reset
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DiagnosticObserver_ProcessEventCleared(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_UdsStatusByteType, AUTOMATIC) OldEventStatus
);

/* ****************************************************************************
 * Dem_DiagnosticObserver_ProcessEventHealed
 *****************************************************************************/
/*!
 * \brief         Process healed event
 *
 * \details       Process reset of warning indicator requests
 *
 * \param[in]     EventId
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DiagnosticObserver_ProcessEventHealed(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
);

/* ****************************************************************************
 * Dem_DiagnosticObserver_ProcessDcyOfEventQualified
 *****************************************************************************/
/*!
 * \brief         Process OBD DCY qualification
 *
 * \details       Process qualification of CDTC and WIR bit
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
Dem_DiagnosticObserver_ProcessDcyOfEventQualified(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
  );

/* ****************************************************************************
 * Dem_DiagnosticObserver_ProcessOBDCycleEnd
 *****************************************************************************/
 /*!
  * \brief         Process end of Obd relevant operation cycle
  *
  * \details       Process end of Obd warm up or driving cycle
  *
  * \param[in]     CycleId
  *                Unique Id of the cycle
  *
  * \pre           -
  *
  * \context       TASK
  * \synchronous   TRUE
  * \reentrant     FALSE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DiagnosticObserver_ProcessOBDCycleEnd(
  CONST(uint8, AUTOMATIC)  CycleId
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_DIAGNOSTICOBSERVERIF_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_DiagnosticObserverIF_Interface.h
 *********************************************************************************************************************/
