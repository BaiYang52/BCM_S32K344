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
 *  \defgroup   Dem_GlobalDiagnostics Dem Global Diagnostics
 *  \{
 *  \file       Dem_GlobalDiagnostics_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Public Interface of Global Diagnostics Logical Unit
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_GLOBALDIAGNOSTICS_INTERFACE_H)
#define DEM_GLOBALDIAGNOSTICS_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/* Includes to expose public functions belonging to Sub-(logical) units to be exposed outwards */
/* ------------------------------------------------------------------------- */
#include "Dem_GlobalDiagnosticsIF_Interface.h"
#include "Dem_Conditions_Interface.h"
#include "Dem_Queue_Interface.h"
#include "Dem_Readiness_Interface.h"
#include "Dem_GlobalState_Interface.h"
#include "Dem_PidStatistics_Interface.h"
#include "Dem_Pto_Interface.h"

#endif /* DEM_GLOBALDIAGNOSTICS_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_GlobalDiagnostics_Interface.h
 *********************************************************************************************************************/
