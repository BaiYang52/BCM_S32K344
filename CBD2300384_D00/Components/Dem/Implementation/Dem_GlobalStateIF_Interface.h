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
 *  \defgroup   Dem_GlobalStateIF GlobalStateIF
 *  \{
 *  \file       Dem_GlobalStateIF_Interface.h
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

#if !defined (DEM_GLOBALSTATEIF_INTERFACE_H)
#define DEM_GLOBALSTATEIF_INTERFACE_H

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

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_GlobalStateIF_PublicProperties Public Properties
 * \{
 */

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
 * \defgroup Dem_GlobalStateIF_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GlobalState_ProcessEventWirEnabled
 *****************************************************************************/
/*!
 * \brief        Trigger processing of an event WIR bit ON trigger 
 *
 * \details      -
 *
 * \param[in]    EventId
 *               Unique handle of the Event.
 *               The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_GlobalState_ProcessEventWirEnabled(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GlobalState_ProcessEventWirDisabled
 *****************************************************************************/
/*!
 * \brief         Trigger processing of an event WIR bit OFF trigger
 *
 * \details       -
 *
 * \param[in]    EventId
 *               Unique handle of the Event.
 *               The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.        
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_GlobalState_ProcessEventWirDisabled(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GlobalState_Init
 *****************************************************************************/
/*!
 * \brief       Initialize all units in this logical unit  
 *
 * \details     -       
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_GlobalState_Init(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GlobalState_ProcessMilStateEnabled
 *****************************************************************************/
/*!
 * \brief         Hook function called when the MIL is activated
 *
 * \details       If the Mil status has changed from Off to On the current
 *                Odometer value (absolute) is set as base value and the
 *                Engine Run Time is set to 0
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_GlobalState_ProcessMilStateEnabled(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GlobalState_ProcessMilStateDisabled
 *****************************************************************************/
/*!
 * \brief         Hook function called when the MIL is deactivated
 *
 * \details       The Mil status has changed from On to Off, the current
 *                Odometer value (relative) may be reset
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_GlobalState_ProcessMilStateDisabled(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GlobalState_ProcessAfterClear
 *****************************************************************************/
/*!
 * \brief         Reset global states after clear
 *
 * \details       Reset global states if the MIL has turned OFF after clear
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_GlobalState_ProcessAfterClear(
  void
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_GLOBALSTATEIF_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_GlobalStateIF_Interface.h
 *********************************************************************************************************************/
