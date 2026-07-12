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
/*! \addtogroup Dem_UserDefinedMemory
 *  \{
 *  \file       Dem_UserDefinedMemory_Types.h
 *  \brief      Type Definitions for User Defined Memory Unit.
 *  \details    Type Definitions for User Defined Memory Unit.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_USERDEFINEDMEMORY_TYPES_H)
#define DEM_USERDEFINEDMEMORY_TYPES_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Int.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/
/*!
* \defgroup  Dem_Memory_FaultMemory_UserDefinedMemory_Range  Macros encoding valid range of user defined memories
* \{
*/

#define DEM_MEMORY_USERDEFINEDMEMORY_DTCORIGIN_LOWER_LIMIT               (0x0100U)  /*!< Lower limit of user defined memory origin */
#define DEM_MEMORY_USERDEFINEDMEMORY_DTCORIGIN_UPPER_LIMIT               (0x01FFU)  /*!< Upper limit of user defined memory origin */

/*!
* \}
*/

/*! User defined Memory translation offset*/
#define DEM_MEMORY_USERDEFINEDMEMORY_TRANSLATION_OFFSET             (uint16)0x100U

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


#endif /* DEM_USERDEFINEDMEMORY_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_UserDefinedMemory_Types.h
 *********************************************************************************************************************/
