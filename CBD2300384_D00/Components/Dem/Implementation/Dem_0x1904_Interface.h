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
 *  \defgroup   Dem_0x1904 DEM 0x19 04 Interface
 *  \{
 *  \file       Dem_0x1904_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) Interface declaration file
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

#if !defined (DEM_0x1904_INTERFACE_H)
#define DEM_0x1904_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_0x1904_Types.h"
/* ------------------------------------------------------------------------- */

#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
* \defgroup Dem_0x1904_PublicProperties Public Properties 
* \{
*/

/* ****************************************************************************
 * Dem_RecordReader_0x1904_GlobalSnapshotRecordsEnabled
 *****************************************************************************/
/*!
 * \brief         Returns true if feature Global Snapshot Records is enabled.
 *
 * \details       -
 *
 * \return        TRUE
 *                Global Snapshot Records is enabled
 * \return        FALSE
 *                Global Snapshot Records is not enabled
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_RecordReader_0x1904_GlobalSnapshotRecordsEnabled(
  void 
  );

#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_0x1904_Public Public Methods
 * \{
 */

 /* ****************************************************************************
 * Dem_RecordReader_0x1904_IsObdIIFreezeFrameInService19EnabledInVariant
 *****************************************************************************/
 /*!
 * \brief         Return if OBDII freeze frame is supported in service 19 in variant.
 * \details       -
 *
 * \return        TRUE
 *                OBD-II freeze frame supported in service 19 
 *                and OBD II is active in variant.
 * \return        FALSE
 *                Either OBD-II freeze frame not supported in service 19 
 *                or OBD II is not active in variant.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_RecordReader_0x1904_IsObdIIFreezeFrameInService19EnabledInVariant(
  void
  );

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_RecordReader_0x1904_SelectFirstSource
 *****************************************************************************/
/*!
 * \brief         Invokes the correct function to select the first snapshot
 *                record source based on the selected DTC.
 *
 * \details       Invokes the correct function to select the first snapshot
 *                record source based on the selected DTC.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \param[in]     RecordNumber
 *                The record number to select
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_RecordReader_0x1904_SelectFirstSource(
  CONST(uint8, AUTOMATIC) ClientId,
  CONST(uint8, AUTOMATIC)  RecordNumber
  );
#endif


# if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_RecordReader_0x1904_ReadNextRecord
 *****************************************************************************/
/*!
 * \brief         Validates the request to copy snapshot record in UDS format
 *                and invokes the relevant functions to copy data to the given 
 *                buffer
 *
 * \details       Validates the request to copy snapshot record in UDS format
 *                and invokes the relevant functions to copy data to the given 
 *                buffer
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[out]    DestBuffer
 *                Pointer to the destination buffer
 * \param[in,out] BufSize
 *                In: Available space in the receiving buffer
 *                Out: Number of written bytes.
 *
 * \return        DEM_NO_SUCH_ELEMENT
 *                Selection function is not called.
 *
 * \return        DEM_BUFFER_TOO_SMALL
 *                provided buffer size too small.
 *
 * \return        DEM_NO_SUCH_ELEMENT
 *                Record number is not supported by configuration and
 *                therefore invalid
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *              
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds.
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_RecordReader_0x1904_ReadNextRecord(
  CONST(uint8, AUTOMATIC) ClientId,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  DestBuffer,
  CONSTP2VAR(uint16, AUTOMATIC, DEM_APPL_DATA)  BufSize
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_RecordReader_0x1904_GetSize
 *****************************************************************************/
/*!
 * \brief         Invoke the correct function to retrieve size of selected
 *                freeze frame data.
 *
 * \details       Invoke the correct function to retrieve size of selected
 *                freeze frame data.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[out]    SizeOfFreezeFrame
 *                Pointer to receive the size in bytes
 *
 * \return        E_OK
 *                The requested data was returned
 * \return        E_NOT_OK
 *                Selection function is not called.
 * \return        DEM_NO_SUCH_ELEMENT
 *                The requested record is not available
 * \return        DEM_PENDING
 *                Not used by this implementation
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2, from the master partition only
 * \synchronous   FALSE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_RecordReader_0x1904_GetSize(
  CONST(uint8, AUTOMATIC)  ClientId,
  CONSTP2VAR(uint16, AUTOMATIC, DEM_DCM_DATA)  SizeOfFreezeFrame
  );
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* DEM_0x1904_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_0x1904_Interface.h
 *********************************************************************************************************************/
