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
 *  \addtogroup Dem_RecordReader
 *  \{
 *  \file       Dem_0x1904_Types.h
 *  \brief      Diagnostic Event Manager (Dem) Type and Macro definition file
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

#if !defined (DEM_0x1904_TYPES_H)
#define DEM_0x1904_TYPES_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_Int.h"
/* ------------------------------------------------------------------------- */
#include "Dem_RecordReaderIF_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/
/*!
 * \defgroup  Dem__0x1904_ExtendedDataRecord_Value Extended Data Records
 * List of special extended data record values
 * \{
 */
#define DEM_DCM_EXTENDEDDATARECORD_INVALID         (0x00u)     /*! Invalid extended record */
#define DEM_DCM_EXTENDEDDATARECORD_FIRST           (0x01u)     /*! The first valid extended record */
#define DEM_DCM_EXTENDEDDATARECORD_FIRST_OBD       (0x90u)     /*! The first valid OBD extended record */
#define DEM_DCM_EXTENDEDDATARECORD_LAST            (0xEFu)     /*! The last valid extended record */
#define DEM_DCM_EXTENDEDDATARECORD_FIRST_RESERVED  (0xF0u)     /*! The first reserved extended record */
#define DEM_DCM_EXTENDEDDATARECORD_OBD             (0xFEu)     /*! Select all OBD records */
#define DEM_DCM_EXTENDEDDATARECORD_ALL             (0xFFu)     /*! Select all extended records */
/*!
 * \}
 */
 
/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
 /*! OBD on UDS FreezeFrame Type */
typedef enum
{
  Dem_RecordReader_0x1904_OBDonUDSFF_0x00 = 0u,  /*! Non updateable 0x00 OBD on UDS FreezeFrame */
  Dem_RecordReader_0x1904_OBDonUDSFF_0xF0 = 1u   /*! Updateable 0x00 OBD on UDS FreezeFrame */
} Dem_1904_ObdOnUdsFreezeFrameType;
#endif /* DEM_0x1904_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_0x1904_Types.h
 *********************************************************************************************************************/
