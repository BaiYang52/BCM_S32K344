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
/*! \addtogroup Dem_GlobalDiagnostics
 *  \{
 *  \file       Dem_GlobalDiagnostics_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Implementation of Global Diagnostics Logical Unit
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_GLOBALDIAGNOSTICS_IMPLEMENTATION_H)
#define DEM_GLOBALDIAGNOSTICS_IMPLEMENTATION_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/* Include implementation belonging to child units */
/* ------------------------------------------------------------------------- */
#include "Dem_GlobalDiagnosticsIF_SvcImplementation.h"
#include "Dem_GlobalDiagnosticsIF_Implementation.h"
#include "Dem_PidStatistics_Implementation.h"
#include "Dem_Conditions_Implementation.h"
#include "Dem_GlobalState_Implementation.h"
#include "Dem_Readiness_Implementation.h"
#include "Dem_Queue_Implementation.h"
#include "Dem_Pto_Implementation.h"

#endif /* DEM_GLOBALDIAGNOSTICS_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_GlobalDiagnostics_Implementation.h
 *********************************************************************************************************************/
