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
/*! \addtogroup Dem_ERec
 *  \{
 *  \file       Dem_ERec_Types.h
 *  \brief      Diagnostic Event Manager (Dem) Type and Macro definition file
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_EREC_TYPES_H)
#define DEM_EREC_TYPES_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_Int.h"
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/
#define  DEM_DCM_EREC_DATACOLLECTIONTABLE_INVALID  0u    /*!< Value for an invalid DataCollectionTable index */
#define  DEM_EREC_OBDONUDS_ERECNUM_IUMPR           0x91u /*!< Extended data record number to request DTC-based IUMPR */
#define  DEM_EREC_OBDONUDS_ERECNUM_DTR             0x92u /*!< Extended data record number to request DTR data*/
#define  DEM_EREC_OBDONUDS_ERECNUM_MAR             0x93u /*!< Extended data record number to request MAR data*/
/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/


/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

 /*! This used for ExtendedDataIterator handles. */
typedef uint8  Dem_Dcm_ERec_0x1916_RecordNumberFilter_HandleType;

/*! This used for iteration over ERecs, upon the EventTable_ERec2EventIdInd table. */
typedef Dem_Cfg_ComplexIterType  Dem_Dcm_ERec_ERecNumberIterType;

/*! This used for iteration over ERecs, upon the DataCollection table. */
typedef Dem_Cfg_ComplexIterType  Dem_Dcm_ERec_DataCollectionERecIterType;

#endif /* DEM_EREC_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ERec_Types.h
 *********************************************************************************************************************/
