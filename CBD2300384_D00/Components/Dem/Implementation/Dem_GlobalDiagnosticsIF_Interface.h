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
 *  \defgroup   Dem_GlobalDiagnosticsIF GlobalDiagnosticsIF
 *  \{
 *  \file       Dem_GlobalDiagnosticsIF_Interface.h
 *  \brief      Implementation of unit GlobalDiagnosticsIF.
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

#if !defined (DEM_GLOBALDIAGNOSTICSIF_INTERFACE_H)
#define DEM_GLOBALDIAGNOSTICSIF_INTERFACE_H

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
 * \defgroup Dem_GlobalDiagnosticsIF_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_PreInit
 *****************************************************************************/
 /*!
 * \brief         Performs preinitalization for the Global Diagnostic 
 *                logical unit.
 *
 * \details       Performs preinitalization for the Global Diagnostic 
 *                logical unit.
 *
 * \pre           Can only be called during Dem preinitialization.
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_PreInit(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_InitPreCore
 *****************************************************************************/
 /*!
 * \brief         Performs initialization for the Global Diagnostic logical unit.
 *
 * \details       Performs initialization for the Global Diagnostic logical unit.
 *
 * \pre           Can only be called during Dem initialization.
 *                Must be called before Event_Init()
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_InitPreCore(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_InitPostCore
 *****************************************************************************/
 /*!
 * \brief         Performs initialization for the Global Diagnostic logical unit.
 *
 * \details       Performs initialization for the Global Diagnostic logical unit.
 *
 * \pre           Can only be called during Dem initialization.
 *                Must be called after Event_Init()
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_InitPostCore(
  void
);


/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessDcyOfEventQualified
 *****************************************************************************/
/*!
 * \brief         Process OBD DCY qualification
 *
 * \details       Process OBD DCY qualification for single event
 *
 * \param[in]     EventContext
 *                Event processing context
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessDcyOfEventQualified(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessEventFailed
 *****************************************************************************/
/*!
 * \brief         Process Event Failed report
 *
 * \details       Process triggers resulting of an Event Failed report
 *
 * \param[in]     EventContext
 *                Event processing context
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventFailed(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessAfterClear
 *****************************************************************************/
/*!
 * \brief         Reset global diagnostics after clear
 *
 * \details       Reset global diagnostics after clear
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessAfterClear(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessAfterClearAll
 *****************************************************************************/
/*!
 * \brief         Reset global diagnostics after clear all
 *
 * \details       Reset global diagnostics after clear all
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessAfterClearAll(
  void
);


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_GLOBALDIAGNOSTICSIF_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_GlobalDiagnosticsIF_Interface.h
 *********************************************************************************************************************/
