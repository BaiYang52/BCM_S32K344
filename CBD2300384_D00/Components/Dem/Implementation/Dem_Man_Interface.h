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
 *  \defgroup   Dem_Man Dem_Mar
 *  \{
 *  \file       Dem_Man_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Interface of the Man subcomponent which manages the calculation of Man.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MAN_INTERFACE_H)
#define DEM_MAN_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/* Own subcomponent header */
#include "Dem_Man_Types.h"
/* ------------------------------------------------------------------------- */


/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Mar_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Mar_Man_MarkNumeratorToBeIncremented
 *****************************************************************************/
/*!
 * \brief         Marks the given MAN as to be incremented this Dcy
 *
 * \details       Marks the given MAN as to be incremented this Dcy
 *
 * \param[in]     ManIdx
 *                Unique handle of the MAN.
 *                The ManIdx must be in range [0..DEM_CFG_MAX_NUMBER_OF_EVENTS_WITH_MAR[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE, for different EventIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Man_MarkNumeratorToBeIncremented(
  CONST(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, AUTOMATIC) ManIdx
);


/* ****************************************************************************
 * Dem_Mar_Man_ResetAllNumerators
 *****************************************************************************/
/*!
 * \brief         Sets the value of all Mans to zero
 *
 * \details       Sets the value of all Mans to zero
 *
 * \param[in]     -
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Man_ResetAllNumerators(
  void
);

/* ****************************************************************************
 * Dem_Mar_Man_CalculateWorstNumerator
 *****************************************************************************/
/*!
 * \brief         Calculates the worst numerator of all MARs of a DTC
 *
 * \details       Calculates the worst numerator of all MARs of a DTC
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Value of MAN
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Man_Type, DEM_CODE)
Dem_Mar_Man_CalculateWorstNumerator(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
);


/* ****************************************************************************
 * Dem_Mar_Man_IncrementNumeratorsIfQueued
 *****************************************************************************/
/*!
 * \brief         Increments the numerators if incrementation is queued
 *
 * \details       Increments the numerators if incrementation is queued
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Man_IncrementNumeratorsIfQueued(
  void
);

/* ****************************************************************************
 * Dem_Mar_Man_ResetToBeIncrementedFlags
 *****************************************************************************/
/*!
 * \brief         Resets the flag of all MANs which indicates that the MAN is 
 *                already incremented in the current driving cycle.
 *
 * \details       Resets the flag of all MANs which indicates that the MAN is 
 *                already incremented in the current driving cycle.
 *
 * \param[in]     -
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Man_ResetToBeIncrementedFlags(
  void
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"

#endif /* DEM_MAN_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Man_Interface.h
 *********************************************************************************************************************/
