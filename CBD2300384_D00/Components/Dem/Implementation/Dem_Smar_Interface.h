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
 *  \defgroup   Dem_Smar Dem_Mar
 *  \{
 *  \file       Dem_Smar_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Interface of the Mar subcomponent which manages storage of the SMAR.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_SMAR_INTERFACE_H)
#define DEM_SMAR_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/* Own subcomponent header */
#include "Dem_Smar_Types.h"
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
 * Dem_Mar_Smar_GetSmarByEventId
 *****************************************************************************/
/*!
 * \brief         Get value of SMAR
 *
 * \details       Get value of SMAR
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        SMAR value
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Smar_Type, DEM_CODE)
Dem_Mar_Smar_GetSmarByEventId(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
);

/* ****************************************************************************
 * Dem_Mar_Smar_SetSmarByEventId
 *****************************************************************************/
/*!
 * \brief         Set value of SMAR by EventId
 *
 * \details       Set value of SMAR By EventId
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \param[in]     Smar
 *                SMAR value which shall be set
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE, for different EventIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Smar_SetSmarByEventId(
  CONST(Dem_EventIdType, AUTOMATIC) EventId,
  CONST(Dem_Smar_Type, AUTOMATIC) Smar
);

/* ****************************************************************************
 * Dem_Mar_Smar_ResetAllSmars
 *****************************************************************************/
/*!
 * \brief         Rest value of all SMARs to zero
 *
 * \details       Rest value of all SMARs to zero
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Smar_ResetAllSmars(
  void
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"

#endif /* DEM_SMAR_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Smar_Interface.h
 *********************************************************************************************************************/
