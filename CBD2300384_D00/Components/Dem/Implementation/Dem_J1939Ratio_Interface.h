/* ********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                                  All rights reserved.
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
 *  \defgroup   Dem_J1939Ratio J1939Ratio
 *  \{
 *  \file       Dem_J1939Ratio_Interface.h
 *  \brief      Subcomponent for handling requests to filter Ratios.
 *  \details    Subcomponent for handling requests to filter Ratios for J1939 Diagnostic Message (DM) 20.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_J1939RATIO_INTERFACE_H)
#define DEM_J1939RATIO_INTERFACE_H

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

                                                  /* Own unit header */
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_J1939Ratio_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 *  Dem_FilterData_J1939Ratio_IsJ1939RatioFilteringEnabled
 *****************************************************************************/
/*!
 * \brief         Returns true if J1939 Ratio filtering is enabled
 * \details       Returns true if J1939 Ratio filtering is enabled
 *
 * \return        TRUE
 *                J1939 Ratio filtering is enabled
 * \return        FALSE
 *                J1939 Ratio filtering is not enabled
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FilterData_J1939Ratio_IsJ1939RatioFilteringEnabled(
  void
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_J1939Ratio_Public Public Methods
 * \{
 */

/* ****************************************************************************
* Dem_J1939Ratio_SpnIter_Init
*****************************************************************************/
/*!
 * \brief         Initialize the iterator
 *
 * \details       Initializes the iterator to enumerate SPNs.
 *
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_J1939Ratio_SpnIter_Init(
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
);

/* ****************************************************************************
 * Dem_FilterData_J1939Ratio_GetNextFilteredRatio
 *****************************************************************************/
/*!
 * \brief         Get the next matching ratio.
 *
 * \details       Get the next ratio matching the criteria of the passed 
 *                DTC filter.
 *
 * \param[in]     FilterId
 *                The filter to modify.
 * \param[out]    SPN
 *                Pointer to receive the SPN of the matching ratio.
 * \param[out]    Numerator
 *                Pointer to receive the numerator of the matching ratio.
 * \param[out]    Denominator
 *                Pointer to receive the denominator of the matching ratio.
 *
 * \return        E_OK
 *                Matching ratio found.
 *                SPN, Numerator and Denominator contain valid data.
 * \return        DEM_NO_SUCH_ELEMENT
 *                No matching ratio found. SPN, Numerator and Denominator
 *                not modified.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FilterData_J1939Ratio_GetNextFilteredRatio(
  Dem_FilterData_HandleType FilterId,
  CONSTP2VAR(uint32, AUTOMATIC, AUTOMATIC) SPN,
  CONSTP2VAR(uint16, AUTOMATIC, AUTOMATIC) Numerator,
  CONSTP2VAR(uint16, AUTOMATIC, AUTOMATIC) Denominator
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_J1939RATIO_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_J1939Ratio_Interface.h
 *********************************************************************************************************************/
