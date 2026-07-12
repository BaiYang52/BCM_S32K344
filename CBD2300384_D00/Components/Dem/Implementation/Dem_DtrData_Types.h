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
/*! \addtogroup Dem_Dtr
 *  \{
 *  \file       Dem_DtrData_Types.h
 *  \brief      Diagnostic Event Manager (Dem) Type and Macro definition file
 *  \details    Type definitions of DtrData subcomponent
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_DTRDATA_TYPES_H)
#define DEM_DTRDATA_TYPES_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_Int.h"
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* type definition for the unique DTR Handle */
typedef uint16 DTRIdType;


# if (DEM_CFG_SUPPORT_DTR == STD_ON)
typedef float32 Dem_DtrData_FloatType;
#  define DEM_DTRDATA_FLOATZERO 0.0f
#  define DEM_DTRDATA_FLOATONEHALF 0.5f
# else
/* Prevent usage of float where not needed --> no linking of floating point library needed. */
typedef sint32 Dem_DtrData_FloatType;
#  define DEM_DTRDATA_FLOATZERO 0
#  define DEM_DTRDATA_FLOATONEHALF 0
# endif

#endif /* DEM_DTRDATA_TYPES_H */
/*!
 * \}
 */

/* ********************************************************************************************************************
 *  END OF FILE: Dem_DtrData_Types.h
 *********************************************************************************************************************/
