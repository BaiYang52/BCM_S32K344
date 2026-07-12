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
 *  \defgroup   Dem_0x191A 0x191A
 *  \{
 *  \file       Dem_0x191A_Interface.h
 *  \brief      Subcomponent for handling Service 0x19 Subfunction 0x1A.
 *  \details    Subcomponent for handling Service 0x19 Subfunction 0x1A, which filters DTCs by a user defined record number.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_0X191A_INTERFACE_H)
#define DEM_0X191A_INTERFACE_H

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
#include "Dem_0x191A_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_0x191A_Public Public Methods
 * \{
 */
 /* ****************************************************************************
 *  Dem_FilterData_0x191A_IsService19_1AEnabled
*****************************************************************************/
/*!
 * \brief         Returns true if service 19 1A is enabled
 * \details       Returns true if service 19 subfunction 1A is enabled
 *
 * \return        TRUE
 *                Support for service 19 subfunction 1A
 * \return        FALSE
 *                No support for service 19 1A
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FilterData_0x191A_IsService19_1AEnabled(
  void
);

/* ****************************************************************************
 *  Dem_FilterData_0x191A_SetDTCFilterByExtendedDataRecordNumber
*****************************************************************************/
/*!
 * \brief         Initialize the DTC filter for extended data record number filtering.
 *
 * \details       Initialize the DTC filter for extended data record number filtering.
 *
 * \param[in]     DTCFilterId
 *                The DTC filter handle.
 * \param[in]     ExtendedDataRecordNumber
 *                Number of the extended data record.
 *
 * \pre           Feature Service 0x19 1A is enabled and DCM support is enabled
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different DTCFilterIds
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FilterData_0x191A_SetDTCFilterByExtendedDataRecordNumber(
  Dem_FilterData_HandleType DTCFilterId,
  uint8 ExtendedDataRecordNumber
);

/* ****************************************************************************
 *  Dem_FilterData_0x191A_GetNumberOfFilteredDTCsByERecNumber
 *****************************************************************************/
/*!
 * \brief         Calculate number of DTCs with a certain extended data record
 *
 * \details       Calculate number of DTCs with a certain extended data record
 *
 * \param[in]     DataCollectionTableIdx
 *                Index of the Data Collection Table.
 *
 * \return        Number of DTCs
 *
 * \pre           Feature Service 0x19 1A is enabled and DCM support is enabled
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_FilterData_0x191A_GetNumberOfFilteredDTCsByERecNumber(
  Dem_Cfg_ERecIndexType DataCollectionTableIdx
);

/* ****************************************************************************
 * Dem_FilterData_0x191A_FilterDTCByERecNumber
 *****************************************************************************/
/*!
 * \brief         Iterates over events with a certain extended data record.
 *
 * \details       Iterates over events with a certain extended data record.
 *                The events with a certain extended data record is specified in DTC Filter.
 *                
 *
 * \param[in]     DTCFilterPtr
 *                Pointer to a DTC filter.
 *
 * \return        EventId of the next event matching the extended data record filter criteria.
 *
 * \pre           Feature Service 0x19 1A is enabled and DCM support is enabled
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different DTCFilterIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_FilterData_0x191A_FilterDTCByERecNumber(
  CONSTP2VAR(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC)  DTCFilterPtr
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_0X191A_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_0x191A_Interface.h
 *********************************************************************************************************************/
