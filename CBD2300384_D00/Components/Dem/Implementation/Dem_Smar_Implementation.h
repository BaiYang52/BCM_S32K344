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
/*! \addtogroup Dem_Mar_Smar
 *  \{
 *  \file       Dem_Smar_Implementation.h
 *  \brief      Mar Smar subcomponent.
 *  \details    Implementation of Smar subcomponent which provides the functionality to store the SMAR.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_SMAR_IMPLEMENTATION_H)
#define DEM_SMAR_IMPLEMENTATION_H

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
#include "Dem_Smar_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_SMAR_IMPLEMENTATION_FILENAME "Dem_Smar_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/


/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Mar_Smar_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Mar_Smar_GetNumberOfSmars
 *****************************************************************************/
/*!
 * \brief        Get number of SMARs.
 *
 * \details      Get number of SMARs.
 *
 * \return       Number of SMARs.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SMARIndexOfMonitorActivityRatioTableType, DEM_CODE)
Dem_Mar_Smar_GetNumberOfSmars(
  void
);

/* ****************************************************************************
 * Dem_Mar_Smar_SetSmar
 *****************************************************************************/
/*!
 * \brief         Set SMAR value by SMAR index.
 *
 * \details       Set SMAR value by SMAR index.
 *
 * \param[in]     SmarIdx
 *                SMAR index.
 *
 * \param[in]     Smar
 *                SMAR value.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Smar_SetSmar(
  CONST(Dem_Cfg_SMARIndexOfMonitorActivityRatioTableType, AUTOMATIC) SmarIdx,
  CONST(Dem_Smar_Type, AUTOMATIC) Smar
);

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Mar_Smar_GetNumberOfSmars
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SMARIndexOfMonitorActivityRatioTableType, DEM_CODE)
Dem_Mar_Smar_GetNumberOfSmars(
  void
)
{
#if (DEM_CFG_DATA_MONITOR_ACTIVITY_DATA == STD_ON)
  return (Dem_Cfg_SMARIndexOfMonitorActivityRatioTableType)DEM_CFG_MAX_NUMBER_OF_STORED_MONITOR_ACTIVITY_RATIO;
#else
  return (Dem_Cfg_SMARIndexOfMonitorActivityRatioTableType)0;
#endif
}

/* ****************************************************************************
 % Dem_Mar_Smar_SetSmar
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Smar_SetSmar(
  CONST(Dem_Cfg_SMARIndexOfMonitorActivityRatioTableType, AUTOMATIC) SmarIdx,
  CONST(Dem_Smar_Type, AUTOMATIC) Smar
)
{
  if ((Dem_Error_isDevRunTimeChecksEnabled() == TRUE)
    && (SmarIdx >= Dem_Mar_Smar_GetNumberOfSmars()))
  {
    Dem_Error_RunTimeCheckFailed(DEM_SMAR_IMPLEMENTATION_FILENAME, __LINE__);                                                    /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
  {
#if (DEM_CFG_DATA_MONITOR_ACTIVITY_DATA == STD_ON)
    Dem_Cfg_ObdIumprData.StoredMonitorActivityRatio[SmarIdx] = Smar;                                                            /* SBSW_DEM_ARRAY_WRITE_STOREDMONITORACTIVITYRATIO */
#else
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(SmarIdx)
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(Smar)
#endif
  }
}


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
 * \addtogroup Dem_Mar_Smar_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_Mar_Smar_SetSmarByEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Smar_SetSmarByEventId(
  CONST(Dem_EventIdType, AUTOMATIC) EventId,
  CONST(Dem_Smar_Type, AUTOMATIC) Smar
)
{
#if (DEM_CFG_DATA_MONITOR_ACTIVITY_DATA == STD_ON)
  Dem_Cfg_SMARIndexOfMonitorActivityRatioTableType lSmarIdx;
  lSmarIdx = Dem_Event_GetSmarIdx(EventId);
  Dem_Mar_Smar_SetSmar(lSmarIdx, Smar);
#endif

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Smar)
}

/* ****************************************************************************
 % Dem_Mar_Smar_ResetAllSmars
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mar_Smar_ResetAllSmars(
  void
)
{
  Dem_Cfg_SMARIndexOfMonitorActivityRatioTableType lSmarIdx;
  Dem_Cfg_SMARIndexOfMonitorActivityRatioTableType lNumberOfSmars;

  lNumberOfSmars = Dem_Mar_Smar_GetNumberOfSmars();

  for (lSmarIdx = 0u;
    lSmarIdx < lNumberOfSmars;                                                                                                   /* PRQA S 2994 */ /* MD_DEM_2994 */
    ++lSmarIdx)
  {
    Dem_Mar_Smar_SetSmar(lSmarIdx, 0u);
  }
}

/* ****************************************************************************
 % Dem_Mar_Smar_GetSmarByEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Smar_Type, DEM_CODE)
Dem_Mar_Smar_GetSmarByEventId(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
)
{
  Dem_Smar_Type lSmar;

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)

  lSmar = 0;

#if (DEM_CFG_DATA_MONITOR_ACTIVITY_DATA == STD_ON)
  {
    Dem_Cfg_SMARIndexOfMonitorActivityRatioTableType lSmarIdx;
    lSmarIdx = Dem_Event_GetSmarIdx(EventId);
    lSmar = (Dem_Smar_Type)Dem_Cfg_ObdIumprData.StoredMonitorActivityRatio[lSmarIdx];
  }
#endif

  return lSmar;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_SMAR_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Smar_Implementation.h
 *********************************************************************************************************************/
