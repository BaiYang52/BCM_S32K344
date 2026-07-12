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
/*! \addtogroup Dem_RecordReaderIF
 *  \{
 *  \file       Dem_RecordReaderIF_Types.h
 *  \brief      Implementation of unit RecordReaderIF.
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

#if !defined (DEM_RECORDREADERIF_TYPES_H)
#define DEM_RECORDREADERIF_TYPES_H

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
#include "Dem_Int.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/
 /*!
 * \defgroup  Dem_0x1904_SnapshotDataRecord_Value Snapshot data Records
 * List of special snapshot data record values
 * \{
 */
#define DEM_RECORDREADER_SNAPSHOTDATARECORD_OBD                     (0x00u)     /*! OBD record */
#define DEM_RECORDREADER_SNAPSHOTDATARECORD_TIMESERIES_POWERTRAIN   (0x30u)     /*! Powertrain TimeSeries record */
#define DEM_RECORDREADER_SNAPSHOTDATARECORD_STANDARD_FIRST          (0x01u)     /*! The first valid standard snapshot data record */
#define DEM_RECORDREADER_SNAPSHOTDATARECORD_OBDONUDS_UPDATEABLE     (0xF0u)     /*! OBD on UDS updateable record */
#define DEM_RECORDREADER_SNAPSHOTDATARECORD_STANDARD_LAST           (0xFEu)     /*! The last valid standard snapshot data record */
#define DEM_RECORDREADER_SNAPSHOTDATARECORD_ALL                     (0xFFu)     /*! All snapshot data records */
/*!
 * \}
 */

 /* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#endif /* DEM_RECORDREADERIF_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_RecordReaderIF_Types.h
 *********************************************************************************************************************/
