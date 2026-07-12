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
/*! \addtogroup Dem_UserDefinedMemory
 *  \{
 *  \file       Dem_UserDefinedMemory_Implementation.h
 *  \brief      Implementation for User Defined Memory Unit.
 *  \details    Implementation for User Defined Memory Unit.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_USERDEFINEDMEMORY_IMPLEMENTATION_H)
#define DEM_USERDEFINEDMEMORY_IMPLEMENTATION_H

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
#include "Dem_UserDefinedMemory_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/ - units to be used by this Unit */
/* ------------------------------------------------------------------------- */



/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_USERDEFINEDMEMORY_IMPLEMENTATION_FILENAME "Dem_UserDefinedMemory_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/


/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
/*!
 * \defgroup Dem_Memory_PrivateProperties Private Properties
 * \{
 */
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Memory_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_Memory_UserDefinedMemory_isUserDefinedMemoryEnabled()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_UserDefinedMemory_isUserDefinedMemoryEnabled(
  void
  )
{
  return (boolean)(DEM_CFG_SUPPORT_USERDEFINEDMEMORY == STD_ON);
}

/* ****************************************************************************
% Dem_Memory_UserDefinedMemory_isUserDefinedMemoryValid()
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_UserDefinedMemory_isUserDefinedMemoryValid(
  Dem_DTCOriginType DTCOrigin
  )
{
    return (boolean)((DTCOrigin >= DEM_MEMORY_USERDEFINEDMEMORY_DTCORIGIN_LOWER_LIMIT)
                    && (DTCOrigin <= DEM_MEMORY_USERDEFINEDMEMORY_DTCORIGIN_UPPER_LIMIT));
}

/* ****************************************************************************
 % Dem_Memory_UserDefinedMemory_GetNumberOfUserDefinedMemories()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memory_UserDefinedMemory_GetNumberOfUserDefinedMemories(
  void
  )
{
#if (DEM_CFG_SUPPORT_USERDEFINEDMEMORY == STD_ON)
  return (uint16)DEM_CFG_MAX_NUM_USER_DEFINED_MEMORIES;
#else
  return (uint16)0x00U;
#endif
}

/* ****************************************************************************
 % Dem_Memory_UserDefinedMemory_TranslateDTCOrigin
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memory_UserDefinedMemory_TranslateDTCOrigin(
  Dem_DTCOriginType  DTCOrigin
  )
{
  uint16 lTranslatedMemoryId;
  lTranslatedMemoryId = DEM_CFG_MEMORYID_PRIMARY;

#if (DEM_CFG_SUPPORT_USERDEFINEDMEMORY == STD_ON)
  {
    uint16 lUserDefinedMemoryId;
    lUserDefinedMemoryId = (DTCOrigin - DEM_MEMORY_USERDEFINEDMEMORY_TRANSLATION_OFFSET);
    if (Dem_Cfg_IsMemoryInfoTableUsedOfUserDefinedOriginTable(lUserDefinedMemoryId) == TRUE)
    {
      lTranslatedMemoryId = (uint16)Dem_Cfg_GetMemoryInfoTableIdxOfUserDefinedOriginTable(lUserDefinedMemoryId);
    }
  }
#endif

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DTCOrigin)
  return lTranslatedMemoryId;
}

/* ****************************************************************************
% Dem_Memory_UserDefinedMemory_IsSRecNumerationCalculated
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_UserDefinedMemory_IsSRecNumerationCalculated(
  Dem_Cfg_SRecNumerationType NumerationType
  )
{
  return (boolean)((NumerationType == DEM_CFG_MEMORY_FF_RECNUM_CALCULATED)
    || (NumerationType == DEM_CFG_MEMORY_FF_RECNUM_CALCULATED_FIFO));
}

/* ****************************************************************************
% Dem_Memory_UserDefinedMemory_IsSRecNumerationCalculatedFiFo
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_UserDefinedMemory_IsSRecNumerationCalculatedFiFo(
  Dem_Cfg_SRecNumerationType NumerationType
  )
{
  return (boolean)(NumerationType == DEM_CFG_MEMORY_FF_RECNUM_CALCULATED_FIFO);
}

/* ****************************************************************************
% Dem_Memory_UserDefinedMemory_IsSRecNumerationConfigured
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memory_UserDefinedMemory_IsSRecNumerationConfigured(
  Dem_Cfg_SRecNumerationType NumerationType
  )
{
  return (boolean)(NumerationType == DEM_CFG_MEMORY_FF_RECNUM_CONFIGURED);
}

#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*!
 * \}
 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Memory_Private Private Methods
 * \{
 */
                                                                                                                                 /* PRQA S 6050 */ /* MD_MSR_STCAL */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Memory_Public
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_USERDEFINEDMEMORY_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_UserDefinedMemory_Implementation.h
 *********************************************************************************************************************/
