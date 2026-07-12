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
 *  \defgroup   Dem_Mad Dem_Mar
 *  \{
 *  \file       Dem_Mad_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Interface of the Mad subcomponent which manages the calculation of Mad.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MAD_INTERFACE_H)
# define DEM_MAD_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/* Own subcomponent header */
/* ------------------------------------------------------------------------- */
# include "Dem_Mad_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DEM_START_SEC_CODE
# include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Mar_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Mar_Mad_Init
 *****************************************************************************/
/*!
 * \brief         Initialize incrementation queued flag with FALSE
 *
 * \details       Initialize incrementation queued flag with FALSE
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Mad_Init(
  void
);

/* ****************************************************************************
 * Dem_Mar_Mad_GetDenominator
 *****************************************************************************/
/*!
 * \brief         Get the MAD value
 *
 * \details       Get the MAD value
 *
 * \return        MAD value
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Mad_Type, DEM_CODE)
Dem_Mar_Mad_GetDenominator(
  void
);

/* ****************************************************************************
 * Dem_Mar_Mad_QueueIncrement
 *****************************************************************************/
/*!
 * \brief         Queues the incrementation of the MAD
 *
 * \details       Queues the incrementation of the MAD
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Mad_QueueIncrement(
  void
);

/* ****************************************************************************
 * Dem_Mar_Mad_ResetDenominator
 *****************************************************************************/
/*!
 * \brief         Resets the Mad value to zero
 *
 * \details       Resets the Mad value to zero
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Mad_ResetDenominator(
  void
);

/* ****************************************************************************
 * Dem_Mar_Mad_IncrementDenominatorIfQueued
 *****************************************************************************/
/*!
 * \brief         Increments the denominator if incrementation is queued
 *
 * \details       Increments the denominator if incrementation is queued
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Mad_IncrementDenominatorIfQueued(
  void
);

/* ****************************************************************************
 * Dem_Mar_Mad_ResetIncrementFlag
 *****************************************************************************/
/*!
 * \brief         Reset the denominator incrementation queue flag.
 *
 * \details       Reset the denominator incrementation queue flag.
 *
 * \param[in]     -
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Mad_ResetIncrementFlag(
  void
);

/*!
 * \}
 */
# define DEM_STOP_SEC_CODE
# include "MemMap.h"

#endif /* DEM_MAD_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Mad_Interface.h
 *********************************************************************************************************************/
