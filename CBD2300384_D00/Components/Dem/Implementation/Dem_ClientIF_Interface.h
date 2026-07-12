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
 *  \defgroup   Dem_ClientIF ClientIF
 *  \{
 *  \file       Dem_ClientIF_Interface.h
 *  \brief      Implementation of unit ClientIF.
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

#if !defined (DEM_CLIENTIF_INTERFACE_H)
#define DEM_CLIENTIF_INTERFACE_H

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

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_ClientIF_PublicProperties Public Properties
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
 * \defgroup Dem_ClientIF_Public Public Methods
 * \{
 */
#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_GetStatusOfSelectedDTC
 *****************************************************************************/
/*!
 * \brief         Get the current Uds status of a selected DTC
 *
 * \details       Get the current Uds status of a selected DTC
 *
 * \param[in]     ClientId
 *                The client to query.
 *
 * \param[out]    DTCStatus
 *                Pointer to receive the Uds status
 *
 * \return        E_OK
 *                The requested status was stored in DTCStatus
 * \return        DEM_WRONG_DTC
 *                DTC is suppressed
 * \return        E_NOT_OK
 *                EventId of selection is invalid
 * \return        DEM_NO_SUCH_ELEMENT
 *                The selected DTC does not support a status
 *                (in case that permanent memory was selected as origin)
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           Requires a preceding DTC selection by Dem_SelectDTC
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_GetStatusOfSelectedDTC(
  uint8  ClientId,
  P2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  DTCStatus
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_GetSeverityOfSelectedDTC
 *****************************************************************************/
/*!
 * \brief         Get the severity of a selected DTC
 *
 * \details       Get the severity of a selected DTC
 *
 * \param[in]     ClientId
 *                The client to query.
 *
 * \param[out]    DTCSeverity
 *                Pointer to receive the severity
 *
 * \return        E_OK
 *                The requested severity was stored in DTCSeverity
 * \return        DEM_WRONG_DTC
 *                DTC is suppressed or a hidden dependent Secondary ECU DTC
 * \return        E_NOT_OK
 *                EventId of selection is invalid
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           Requires a preceding DTC selection by Dem_SelectDTC
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_GetSeverityOfSelectedDTC(
  CONST(uint8, AUTOMATIC)  ClientId,
  P2VAR(Dem_DTCSeverityType, AUTOMATIC, DEM_DCM_DATA)  DTCSeverity
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_GetFunctionalUnitOfSelectedDTC
 *****************************************************************************/
/*!
 * \brief         Get the functional unit of a selected DTC
 *
 * \details       Get the functional unit of a selected DTC
 *
 * \param[in]     ClientId
 *                The client to query.
 *
 * \param[out]    DTCFunctionalUnit
 *                Pointer to receive the functional unit
 *
 * \return        E_OK
 *                The requested functional unit was stored in DTCFunctionalUnit
 * \return        DEM_WRONG_DTC
 *                DTC is suppressed or a hidden dependent Secondary ECU DTC
 * \return        E_NOT_OK
 *                EventId of selection is invalid
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           Requires a preceding DTC selection by Dem_SelectDTC
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_GetFunctionalUnitOfSelectedDTC(
  CONST(uint8, AUTOMATIC)  ClientId,
  P2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  DTCFunctionalUnit
  );
#endif

/* ****************************************************************************
 * Dem_Client_GetEventIdOfSelectedDTC
 *****************************************************************************/
/*!
 * \brief         Get the EventId of a selected DTC
 *
 * \details       Get the EventId of a selected DTC
 *
 * \param[in]     ClientId
 *                The client to query.
 *
 * \param[out]    EventId
 *                Pointer to receive the EventId
 *
 * \return        E_OK
 *                The requested EventId was stored in EventId
 * \return        E_NOT_OK
 *                EventId of selection is invalid
 *
 * \pre           Requires a preceding DTC selection by Dem_SelectDTC
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_GetEventIdOfSelectedDTC(
  CONST(uint8, AUTOMATIC)  ClientId,
  P2VAR(Dem_EventIdType, AUTOMATIC, DEM_APPL_DATA)  EventId
  );
/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_CLIENTIF_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ClientIF_Interface.h
 *********************************************************************************************************************/
