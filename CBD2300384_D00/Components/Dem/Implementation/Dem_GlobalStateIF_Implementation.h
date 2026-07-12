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
/*! \addtogroup Dem_GlobalStateIF
 *  \{
 *  \file       Dem_GlobalStateIF_Implementation.h
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

#if !defined (DEM_GLOBALSTATEIF_IMPLEMENTATION_H)
#define DEM_GLOBALSTATEIF_IMPLEMENTATION_H

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
#include "Dem_GlobalStateIF_Interface.h"

                                                    /* Used unit API */
/* ------------------------------------------------------------------------- */
#include "Dem_ActivationMode_Interface.h"
#include "Dem_Indicator_Interface.h"
#include "Dem_Core_Interface.h"

                                                   /* unit callbacks*/
/* ------------------------------------------------------------------------- */


/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_GLOBALSTATEIF_IMPLEMENTATION_FILENAME "Dem_GlobalStateIF_Implementation.h"

 /* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/


/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_GlobalStateIF_PrivateProperties Private Properties
 * \{
 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_GlobalStateIF_PublicProperties
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_GlobalStateIF_Private Private Methods
 * \{
 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_GlobalStateIF_Public
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GlobalState_ProcessEventWirEnabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_GlobalState_ProcessEventWirEnabled(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_GlobalState_Indicator_ProcessEventWirEnabled(EventId);
  Dem_GlobalState_ActivationMode_ProcessWirEnabled(EventId);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GlobalState_ProcessEventWirDisabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_GlobalState_ProcessEventWirDisabled(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  if ((Dem_Cfg_IsObdSupportedInVariant() == FALSE)
    || (Dem_EventInternalStatus_Test_QualifyStatus_WIR(Dem_Event_GetQualifyStatus(EventId)) == TRUE))
  {
    Dem_GlobalState_Indicator_ProcessEventWirDisabled(EventId);
    Dem_GlobalState_ActivationMode_ProcessWirDisabled(EventId);
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GlobalState_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_GlobalState_Init(
  void
  )
{
  Dem_GlobalState_Indicator_Init();
  Dem_GlobalState_ActivationMode_Init();
  Dem_GlobalState_ActivationMode_RestoreCounters();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GlobalState_ProcessMilStateDisabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_GlobalState_ProcessMilStateDisabled(
  void
  )
{
#if (DEM_FEATURE_NEED_PID21 == STD_ON)
  /* If the Indicator state changes from on to off, the current Odometer value is set to invalid */
/* >>>> -------------------------------- Enter Critical Section: NonAtomic32bit */
  Dem_EnterCritical_NonAtomic32bit();

  Dem_Mem_SetOdometerMilOn(DEM_MEM_SET_ODOMETER_INVALID(Dem_Mem_GetOdometerMilOn()));

  Dem_LeaveCritical_NonAtomic32bit();
/* <<<< -------------------------------- Leave Critical Section: NonAtomic32bit */
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GlobalState_ProcessMilStateEnabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_GlobalState_ProcessMilStateEnabled(
  void
  )
{
  /* The MIL Indicator state has changed from FALSE to TRUE, the current Odometer value shall be fetched */
# if (DEM_FEATURE_NEED_PID21 == STD_ON)
/* >>>> -------------------------------- Enter Critical Section: NonAtomic32bit */
  Dem_EnterCritical_NonAtomic32bit();

  Dem_Mem_SetOdometerMilOn(DEM_MEM_SET_ODOMETER_INVALID(0u));
  Dem_LeaveCritical_NonAtomic32bit();
/* <<<< -------------------------------- Leave Critical Section: NonAtomic32bit */
# endif
# if (DEM_FEATURE_NEED_PID4D == STD_ON)
  Dem_Mem_SetEngineTimeMilOn(0);
# endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GlobalState_ProcessAfterClear
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_GlobalState_ProcessAfterClear(
  void
)
{

#if (DEM_FEATURE_NEED_GLOBAL_MIL_STATE == STD_ON)
  if (Dem_GlobalState_Indicator_TestMilReportedActive() == FALSE)
  {
# if (DEM_FEATURE_NEED_PID21 == STD_ON)
    /* >>>> -------------------------------- Enter Critical Section: NonAtomic32bit */
    Dem_EnterCritical_NonAtomic32bit();
    /* Clear Odometer for MIL on */
    Dem_Mem_SetOdometerMilOn(0);
    Dem_LeaveCritical_NonAtomic32bit();
    /* <<<< -------------------------------- Leave Critical Section: NonAtomic32bit */
# endif
# if (DEM_FEATURE_NEED_PID4D == STD_ON)
    Dem_Mem_SetEngineTimeMilOn(0);
# endif
  }
#endif
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_GLOBALSTATEIF_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_GlobalStateIF_Implementation.h
 *********************************************************************************************************************/
