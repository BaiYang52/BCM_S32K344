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
/*! \addtogroup Dem_Data
 *  \{
 *  \file       Dem_Data_Types.h
 *  \brief      Diagnostic Event Manager (Dem) header file
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

#if !defined (DEM_DATA_TYPES_H)
#define DEM_DATA_TYPES_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Int.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

                                                           /* Data Provision */
/* ------------------------------------------------------------------------- */

/*! Start value for occurrence counter */
# define DEM_DATA_OCCURRENCE_INIT                (0U)

#if (DEM_CFG_DATA_OCCCTR_2BYTE == STD_ON)
/*! Maximum value for 2 Byte occurrence counter */
# define DEM_DATA_OCCURRENCE_MAX                 (0xffffU)
#else
/*! Maximum value for 1 byte occurrence counter */
# define DEM_DATA_OCCURRENCE_MAX                 (0xffU)
#endif

/*!
 * \defgroup  Dem_Data_ChangeState  Macros encoding changed data while updating DTCs
 * Macros encoding changed data while updating DTCs
 * \{
 */
#define DEM_DATA_NO_CHANGES                      (0x00U)  /*!< No data changed */
#define DEM_DATA_USERDATA_CHANGED                (0x01U)  /*!< User data changed, update NV ram and call data change notification */
#define DEM_DATA_STATISTICS_CHANGED              (0x02U)  /*!< Internal data changed, update NV ram but no data change notification */
/*!
 * \}
 */


/*!
 * \defgroup  Dem_Data_OBD_FF_Update  Macros encoding freeze frame updates
 * Macros encoding freeze frame updates
 * \{
 */
#define DEM_DATA_FF_UPDATE_DATA                  (0x01U)  /*!< Store new freeze frame data */
#define DEM_DATA_FF_UPDATE_VISIBILITY            (0x02U)  /*!< Update freeze frame visibility */
#define DEM_DATA_FF_UPDATE_TIMESTAMP             (0x04U)  /*!< Update freeze frame order */
#define DEM_DATA_FF_UPDATE_REQUIRED     (DEM_DATA_FF_UPDATE_DATA |DEM_DATA_FF_UPDATE_VISIBILITY|DEM_DATA_FF_UPDATE_TIMESTAMP) /* freeze frame slot needs update */
/*!
 * \}
 */

/*! Maximum value for OBD odometer */
#define DEM_DATA_MAX_OBD_ODOMETER                (0xffffU)



/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/
/*! Test userdata changed flag of data changed state */
#define DEM_DATA_TEST_USERDATA_CHANGED(Status)   (((Status) & DEM_DATA_USERDATA_CHANGED) != 0u)
/*! Set userdata changed flag of data changed state */
#define DEM_DATA_SET_USERDATA_CHANGED(Status)    ((uint8)((Status) | DEM_DATA_USERDATA_CHANGED))

/*! Test statistics changed flag of data changed state */
#define DEM_DATA_TEST_STATISTICS_CHANGED(Status)  (((Status) & DEM_DATA_STATISTICS_CHANGED) != 0u)
/*! Set statistics changed flag of data changed state */
#define DEM_DATA_SET_STATISTICS_CHANGED(Status)  ((uint8)((Status) | DEM_DATA_STATISTICS_CHANGED))

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*! Structure for Destination Buffer */
struct Dem_Data_DestinationBuffer_s
{
  Dem_SharedDataPtrType             Buffer;      /*!< Destination buffer receiving the DID data */
  uint16                            BufferSize;  /*!< Buffer size of the destination buffer */
  uint16                            WriteIndex;  /*!< Current write index in the destination buffer */
};

/*! Type for Destination Buffer */
typedef struct Dem_Data_DestinationBuffer_s Dem_Data_DestinationBuffer;
/*! Pointer to type for Destination Buffer */
typedef P2VAR(Dem_Data_DestinationBuffer, TYPEDEF, AUTOMATIC) Dem_Data_DestinationBufferPtrType;
typedef P2CONST(Dem_Data_DestinationBuffer, TYPEDEF, AUTOMATIC) Dem_Data_ConstDestinationBufferPtrType;

#endif /* DEM_DATA_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Data_Types.h
 *********************************************************************************************************************/
