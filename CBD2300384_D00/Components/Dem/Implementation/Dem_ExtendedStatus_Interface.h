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
 *  \defgroup   Dem_ExtendedStatus ExtendedStatus
 *  \{
 *  \file       Dem_ExtendedStatus_Interface.h
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

#if !defined (DEM_EXTENDEDSTATUS_INTERFACE_H)
#define DEM_EXTENDEDSTATUS_INTERFACE_H

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

                                                  /* Own unit header */
/* ------------------------------------------------------------------------- */
#include "Dem_ExtendedStatus_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_ExtendedStatus_PublicProperties Public Properties
 * \{
 */

 /* ****************************************************************************
 * Dem_Event_ExtendedStatus_GetTPSLC
 *****************************************************************************/
 /*!
 * \brief         Test test passed since last clear state of extended status of event
 *
 * \details       -
 *
 * \param[in]     Status
 *                Current extended status of the event
 *
 * \return        TRUE
 *                The TPSLC bit is set
 * \return        FALSE
 *                The TPSLC bit is cleared
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_ExtendedStatus_GetTPSLC(
  CONST(Dem_Event_ExtendedStatusType, AUTOMATIC)  Status
);

/* ****************************************************************************
 * Dem_Event_ExtendedStatus_SetTPSLC
 *****************************************************************************/
/*!
 * \brief         Set test passed since last clear state of extended status of event
 *
 * \details       -
 *
 * \param[in]     Status
 *                Current extended status of the event
 *
 * \return        Event extended status with flag set for TPSLC.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Event_ExtendedStatusType, DEM_CODE)
Dem_Event_ExtendedStatus_SetTPSLC(
  CONST(Dem_Event_ExtendedStatusType, AUTOMATIC)  Status
);


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_ExtendedStatus_Public Public Methods
 * \{
 */

 /* ****************************************************************************
  * Dem_Event_ExtendedStatus_ProcessPassedEffectsTPSLC
  *****************************************************************************/
  /*!
  * \brief         Processes DTCs that are relevant for Readiness Completion calculation
  *
  * \details       Extended Status of DTC is set to 'passed since last clear'.
  *                For DTC with combined group status it is only set if all
  *                subevents are tested this cycle and no subevent has status
  *                failed.
  *
  * \param[in]     DTCContext
  *                DTC processing context
  *
  * \pre           -
  * \config        Feature Major Monitors and PID 01 is enabled
  *
  * \context       TASK|ISR2
  * \synchronous   TRUE
  * \reentrant     TRUE for different DTCContext
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_ExtendedStatus_ProcessPassedEffectsTPSLC(
  CONSTP2CONST(Dem_DTC_ContextType, AUTOMATIC, AUTOMATIC)  DTCContext
);


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_EXTENDEDSTATUS_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ExtendedStatus_Interface.h
 *********************************************************************************************************************/
