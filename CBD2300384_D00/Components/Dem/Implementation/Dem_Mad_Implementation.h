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
/*! \addtogroup Dem_Mar_Mad
 *  \{
 *  \file       Dem_Mad_Implementation.h
 *  \brief      Mar Mad subcomponent.
 *  \details    Implementation of MAD subcomponent which provides the functionality to calculate the Mad.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MAD_IMPLEMENTATION_H)
# define DEM_MAD_IMPLEMENTATION_H

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
/* PRQA S 0759 EOF */ /* MD_MSR_Union */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
# include "Dem_Mad_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
# define DEM_MAD_IMPLEMENTATION_FILENAME "Dem_Mad_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

# define DEM_START_SEC_VAR_NOINIT_8BIT
# include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Flag indicating if any numerator needs to be incremented */
DEM_LOCAL volatile VAR(boolean, DEM_VAR_NOINIT)  Dem_Mad_IncrementationQueued;

# define DEM_STOP_SEC_VAR_NOINIT_8BIT
# include "MemMap.h" 

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
# define DEM_START_SEC_CODE
# include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Mar_Mad_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Mar_Mad_SetDenominator
 *****************************************************************************/
/*!
 * \brief         Sets the MAD to the given value
 *
 * \details       Sets the MAD to the given value
 *
 * \param[in]     Mad
 *                MAD value which shall be set. 
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Mad_SetDenominator(
  CONST(Dem_Mad_Type, AUTOMATIC) Mad
);

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Mar_Mad_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Mad_SetDenominator(
  CONST(Dem_Mad_Type, AUTOMATIC) Mad
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Mad)

# if (DEM_CFG_DATA_MONITOR_ACTIVITY_DATA == STD_ON)
    Dem_Cfg_ObdIumprData.MonitorActivityDenominator = (uint8) Mad;
# endif
}


/*!
 * \}
 */
# define DEM_STOP_SEC_CODE
# include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

# define DEM_START_SEC_CODE
# include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Mar_Mad_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_Mar_Mad_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Mad_Init(
  void
)
{
  Dem_Mad_IncrementationQueued = FALSE;
  Dem_Mar_Mad_ResetDenominator();
}

/* ****************************************************************************
 % Dem_Mar_Mad_GetDenominator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Mad_Type, DEM_CODE)
Dem_Mar_Mad_GetDenominator(
  void
)
{
# if (DEM_CFG_DATA_MONITOR_ACTIVITY_DATA == STD_ON)
  return (Dem_Mad_Type) Dem_Cfg_ObdIumprData.MonitorActivityDenominator;                                                                            /* PRQA S 2843 */ /* MD_DEM_Dir4.1_ReadOperation */
 #else
  return (Dem_Mad_Type)0;
# endif
}

/* ****************************************************************************
 % Dem_Mar_Mad_QueueIncrement
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Mad_QueueIncrement(
  void
)
{
  Dem_Mad_IncrementationQueued = TRUE;
}

/* ****************************************************************************
 % Dem_Mar_Mad_ResetDenominator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Mad_ResetDenominator(
  void
)
{ 
  Dem_Mar_Mad_SetDenominator(0u);
}

/* ****************************************************************************
 % Dem_Mar_Mad_IncrementDenominatorIfQueued
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Mad_IncrementDenominatorIfQueued(
  void
)
{
  if (Dem_Mad_IncrementationQueued == TRUE)
  {
    Dem_Mad_Type lMad = Dem_Mar_Mad_GetDenominator();
    Dem_Mar_Mad_SetDenominator(lMad + 1u);
  }
}

/* ****************************************************************************
 % Dem_Mar_Mad_ResetIncrementFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Mad_ResetIncrementFlag(
  void
)
{
  Dem_Mad_IncrementationQueued = FALSE;
}

/*!
 * \}
 */
# define DEM_STOP_SEC_CODE
# include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_MAD_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Mad_Implementation.h
 *********************************************************************************************************************/
