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
 *  \defgroup   Dem_J1939DTC J1939DTC
 *  \{
 *  \file       Dem_J1939DTC_Interface.h
 *  \brief      Subcomponent for handling requests to filter J1939 DTCs.
 *  \details    Subcomponent for handling requests to filter J1939 DTCs for J1939 Diagnostic Messages.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_J1939DTC_INTERFACE_H)
#define DEM_J1939DTC_INTERFACE_H

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
 *  SUBCOMPONENT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_J1939DTC_PublicProperties Public Properties
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_J1939DTC_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_FilterData_J1939DTC_TestJ1939DTCFilterSupported()
 *****************************************************************************/
/*!
 * \brief         Test if filter criteria are supported for a
 *                'filter J1939 DTC' request.
 *
 * \details       Test if the given filter criteria are supported when setting
 *                up a J1939 DTC filter.
 *
 * \param[in]     DTCStatusFilter
 *                DTC status to filter.
 * \param[in]     DTCKind
 *                DTC kind to filter.
 * \param[in]     DTCOrigin
 *                Destination of the DTC.
 *
 * \return        TRUE
 *                The filter settings are supported.
 * \return        FALSE
 *                The filter settings are not supported.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FilterData_J1939DTC_TestJ1939DTCFilterSupported(
  Dem_J1939DcmDTCStatusFilterType DTCStatusFilter,
  Dem_DTCKindType DTCKind,
  Dem_DTCOriginType DTCOrigin
);

/* ****************************************************************************
 * Dem_J1939DTC_Filter_Init()
 *****************************************************************************/
/*!
 * \brief         Initialize a filter for a 'filter J1939 DTC' request.
 *
 * \details       Initialize a filter for a 'filter J1939 DTC' request based
 *                on the passed filter information.
 *
 * \param[in]     DTCFilterPtr
 *                Pointer to the DTC filter to initialize.
 * \param[in]     DTCStatusFilter
 *                The DTC status filter type.
 * \param[in]     DTCOrigin
 *                Destination of the DTC.
 *
 * \return        E_OK
 *                The filter was initialized.
 * \return        E_NOT_OK
 *                The filter was not initialized due to invalid parameters.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DTC_Filter_Init(
  CONSTP2VAR(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC)  DTCFilterPtr,
  Dem_J1939DcmDTCStatusFilterType DTCStatusFilter,
  Dem_DTCOriginType DTCOrigin
);

/* ****************************************************************************
 * Dem_FilterData_J1939DTC_GetNextFilteredJ1939DTC()
 *****************************************************************************/
/*!
 * \brief         Get the next matching event.
 *
 * \details       Get the next event matching the criteria of the passed 
 *                DTC filter.
 *
 * \param[in]     DTCFilterPtr
 *                Pointer to the DTC filter.
 * \param[in]     FilterEmissionRelevantDTCs
 *                TRUE: Filter only for emission relevant DTCs
 *                FALSE: Filter for all DTCs
 * \param[out]    EventId
 *                Event Id of the next matching event.
 *
 * \return        E_OK
 *                Matching event returned in EventId.
 * \return        DEM_NO_SUCH_ELEMENT
 *                No more matching events, EventId not modified.
 * \return        DEM_PENDING
 *                Filtering is currently in progress and runtime limit
 *                was reached, EventId not modified.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FilterData_J1939DTC_GetNextFilteredJ1939DTC(
  CONSTP2VAR(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr,
  boolean FilterEmissionRelevantDTCs,
  CONSTP2VAR(Dem_EventIdType, AUTOMATIC, AUTOMATIC) EventId
);

/* ****************************************************************************
 * Dem_FilterData_J1939DTC_GetNumberOfFilteredJ1939DTC()
 *****************************************************************************/
/*!
 * \brief         Calculate the number of events matching the J1939 DTC filter.
 *
 * \details       Calculate the number of events matching the J1939 DTC filter.
 *
 * \param[in]     DTCFilterPtr
 *                Pointer to the DTC filter.
 *
 * \return        Number of DTCs matching the J1939 DTC filter criteria.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_FilterData_J1939DTC_GetNumberOfFilteredJ1939DTC(
  CONSTP2CONST(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC)  DTCFilterPtr,
  boolean FilterEmissionRelevantDTCs
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_J1939DTC_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_J1939DTC_Interface.h
 *********************************************************************************************************************/
