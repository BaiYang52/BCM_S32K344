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
/*! \addtogroup Dem_ClientIF
 *  \{
 *  \file       Dem_ClientIF_Implementation.h
 *  \brief      Implementation of unit ClientIF.
 *  \details    Realizes the public interfaces of its parent logical unit, i.e. both Dem external service APIs and Dem internal APIs.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_CLIENTIF_IMPLEMENTATION_H)
#define DEM_CLIENTIF_IMPLEMENTATION_H

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
#include "Dem_ClientIF_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_ClientAccess_Interface.h"
#include "Dem_DTCReporting_Interface.h"

#include "Dem_Core_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_FaultMemory_Interface.h"


/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_CLIENTIF_IMPLEMENTATION_FILENAME "Dem_ClientIF_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_ClientIF_Public
 * \{
 */
#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_GetStatusOfSelectedDTC
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_GetStatusOfSelectedDTC(
  uint8  ClientId,
  P2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  DTCStatus
  )
{
  Std_ReturnType lReturnValue;
  uint16 lTranslatedMemoryId;

  lReturnValue = E_NOT_OK;
  lTranslatedMemoryId = Dem_Client_ClientAccess_GetSelectedMemoryId(ClientId);
 
  if (Dem_FaultMemory_Memory_IsEventDestination(lTranslatedMemoryId) == TRUE)
  {
    Dem_EventIdType lEventId;
    lEventId = Dem_Client_ClientAccess_GetSelectedEventId(ClientId);

    if (lEventId != DEM_EVENT_INVALID)
    {
      if (Dem_DTC_TestSuppressionOnDtcRetrieval(lEventId) == TRUE)
      {
        lReturnValue = DEM_WRONG_DTC;
      }
      else
      {
        *DTCStatus = Dem_DTC_GetExternalDTCStatus(lEventId);                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        lReturnValue = E_OK;
      }
    }
  }
  else
  if (lTranslatedMemoryId == DEM_CFG_MEMORYID_PERMANENT)                                                                         /* COV_DEM_MISRA_BRANCH */
  {
    lReturnValue = DEM_NO_SUCH_ELEMENT;
  }
  else
  {
    /* MISRA */
  }

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_GetSeverityOfSelectedDTC
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_GetSeverityOfSelectedDTC(
  CONST(uint8, AUTOMATIC)  ClientId,
  P2VAR(Dem_DTCSeverityType, AUTOMATIC, DEM_DCM_DATA)  DTCSeverity
  )
{
  Std_ReturnType lReturnValue;
  Dem_EventIdType lEventId;

  lEventId = Dem_Client_ClientAccess_GetSelectedEventId(ClientId);

  if (lEventId != DEM_EVENT_INVALID)
  {
    if (Dem_DTC_TestSuppressionOnDtcRetrieval(lEventId) == TRUE)
    {
      lReturnValue = DEM_WRONG_DTC;
    }
    else
    {
      if ( (Dem_DTCReporting_TestObdHideOccurrences() == TRUE)
        && (Dem_Cfg_EventSignificance(lEventId) == DEM_CFG_EVENT_SIGNIFICANCE_OCCURRENCE)
        && (Dem_Core_DTC_IsObdRelated(lEventId) == TRUE) )
      { /* Special case - skip Dependent OBD DTCs */
        lReturnValue = DEM_WRONG_DTC;
      }
      else
      {
        *DTCSeverity = Dem_Cfg_EventSeverity(lEventId);                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        lReturnValue = E_OK;
      }
    }
  }
  else
  {
    lReturnValue = E_NOT_OK;
  }

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_GetFunctionalUnitOfSelectedDTC
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_GetFunctionalUnitOfSelectedDTC(
  CONST(uint8, AUTOMATIC)  ClientId,
  P2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  DTCFunctionalUnit
  )
{
  Std_ReturnType lReturnValue;
  Dem_EventIdType lEventId;

  lEventId = Dem_Client_ClientAccess_GetSelectedEventId(ClientId);

  if (lEventId != DEM_EVENT_INVALID)
  {
    if (Dem_DTC_TestSuppressionOnDtcRetrieval(lEventId) == TRUE)
    {
      lReturnValue = DEM_WRONG_DTC;
    }
    else
    {
      if ( (Dem_DTCReporting_TestObdHideOccurrences() == TRUE)
        && (Dem_Cfg_EventSignificance(lEventId) == DEM_CFG_EVENT_SIGNIFICANCE_OCCURRENCE)
        && (Dem_Core_DTC_IsObdRelated(lEventId) == TRUE) )
      { /* Special case - skip Dependent OBD DTCs */
        lReturnValue = DEM_WRONG_DTC;
      }
      else
      {
        *DTCFunctionalUnit = Dem_Cfg_EventFunctionalUnit(lEventId);                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        lReturnValue = E_OK;
      }
    }
  }
  else
  {
    lReturnValue = E_NOT_OK;
  }

  return lReturnValue;
}
#endif

/* ****************************************************************************
 % Dem_Client_GetEventIdOfSelectedDTC
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_GetEventIdOfSelectedDTC(
  CONST(uint8, AUTOMATIC)  ClientId,
  P2VAR(Dem_EventIdType, AUTOMATIC, DEM_APPL_DATA)  EventId
  )
{
  Std_ReturnType lReturnValue;
  Dem_EventIdType lEventId;

  lEventId = Dem_Client_ClientAccess_GetSelectedEventId(ClientId);

  if (lEventId != DEM_EVENT_INVALID)
  {
    *EventId = lEventId;                                                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    lReturnValue = E_OK;
  }
  else
  {
    lReturnValue = E_NOT_OK;
  }

  return lReturnValue;
}
/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_CLIENTIF_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ClientIF_Implementation.h
 *********************************************************************************************************************/
