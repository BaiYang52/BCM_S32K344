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
 *  \defgroup   Dem_UserDefinedMemory Dem UserDefinedMemory
 *  \{
 *  \file       Dem_UserDefinedMemory_Interface.h
 *  \brief      Interface for User Defined Memory Unit.
 *  \details    Interface for User Defined Memory Unit.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_USERDEFINEDMEMORY_INTERFACE_H)
#define DEM_USERDEFINEDMEMORY_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_UserDefinedMemory_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Memory_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Memory_UserDefinedMemory_isUserDefinedMemoryEnabled
 *****************************************************************************/
/*!
 * \brief         Gets the enablement status of user defined memory feature.
 *
 * \details       Gets the enablement status of user defined memory feature.
 *
 * \return        TRUE
 *                User Defined Memory is enabled.
 * \return        FALSE
 *                User Defined Memory is disabled.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_UserDefinedMemory_isUserDefinedMemoryEnabled(
  void
  );

/* ****************************************************************************
 * Dem_Memory_UserDefinedMemory_isUserDefinedMemoryValid
 *****************************************************************************/
/*!
 * \brief         Returns if the input user defined memory origin is valid or not.
 *
 * \details       Returns if the input user defined memory origin is valid or not.
 *
 * \param[in]     DTCOrigin
 *                The event memory of the requested DTC or group of DTC.
 * 
 * \return        TRUE
 *                User Defined Memory Origin is valid.
 * \return        FALSE
 *                User Defined Memory Origin is invalid.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_UserDefinedMemory_isUserDefinedMemoryValid(
  Dem_DTCOriginType DTCOrigin
  );

/* ****************************************************************************
* Dem_Memory_UserDefinedMemory_GetNumberOfUserDefinedMemories
*****************************************************************************/
/*!
* \brief         Return the number of configured user defined memories.
*
* \details       Return the number of configured user defined memories.
*
* \return        Number of configured user defined memories.
*
* \pre           -
*
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memory_UserDefinedMemory_GetNumberOfUserDefinedMemories(
  void
  );

/* **************************************************************************** 
 * Dem_Memory_UserDefinedMemory_TranslateDTCOrigin
 *****************************************************************************/
/*!
 * \brief         Return index of the User Defined Memory Id to the Memory Info table
 *
 * \details       Return index of the User Defined Memory Id to the Memory Info table
 *
 * \param[in]     DTCOrigin
 *                The event memory of the requested DTC or group of DTC.
 *
 * \return        Index to the Memory Info table.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memory_UserDefinedMemory_TranslateDTCOrigin(
  Dem_DTCOriginType  DTCOrigin
  );


/* ****************************************************************************
* Dem_Memory_UserDefinedMemory_IsSRecNumerationCalculated
*****************************************************************************/
/*!
* \brief         Check if a Snapshot Enumeration type is either Calculated or Calculated FIFO
*
* \details       Check if a Snapshot Enumeration type is either Calculated or Calculated FIFO
*
* \param[in]     NumerationType
*                The Snapshot numeration type
*
* \return        TRUE
*                The enumeration type indicates either Calculated or Calculated FIFO Snapshot Records
* \return        FALSE
*                The enumeration type does not indicate Calculated Snapshot records
*
* \pre           -
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_UserDefinedMemory_IsSRecNumerationCalculated(
  Dem_Cfg_SRecNumerationType NumerationType
  );

/* ****************************************************************************
* Dem_Memory_UserDefinedMemory_IsSRecNumerationCalculatedFiFo
*****************************************************************************/
/*!
* \brief         Check if a Snapshot Enumeration type is Calculated FIFO
*
* \details       Check if a Snapshot Enumeration type is Calculated FIFO
*
* \param[in]     NumerationType
*                The Snapshot numeration type
*
* \return        TRUE
*                The enumeration type indicates Calculated FIFO Snapshot Records
* \return        FALSE
*                The enumeration type does not indicate Calculated FIFO Snapshot Records
*
* \pre           -
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_UserDefinedMemory_IsSRecNumerationCalculatedFiFo(
  Dem_Cfg_SRecNumerationType NumerationType
  );

/* ****************************************************************************
* Dem_Memory_UserDefinedMemory_IsSRecNumerationConfigured
*****************************************************************************/
/*!
* \brief         Check if a Snapshot Enumeration type is Configured
*
* \details       Check if a Snapshot Enumeration type is Configured
*
* \param[in]     NumerationType
*                The Snapshot numeration type
*
* \return        TRUE
*                The enumeration type indicates Configured Snapshot Records
* \return        FALSE
*                The enumeration type does not indicate Configured Snapshot Records
*
* \pre           -
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_UserDefinedMemory_IsSRecNumerationConfigured(
  Dem_Cfg_SRecNumerationType NumerationType
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Memory_Public Public Methods
 * \{
 */


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_USERDEFINEDMEMORY_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_UserDefinedMemory_Interface.h
 *********************************************************************************************************************/
