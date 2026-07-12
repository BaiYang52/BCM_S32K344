/**********************************************************************************************************************
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
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Wdg_30_TLE4278G_Mode.c
 *        \brief  Implementation of sub-module Wdg_30_TLE4278G_Mode.
 *
 *      \details  This sub-module provides mode-specific services to other sub-modules
 *
 *********************************************************************************************************************/

#define WDG_30_TLE4278G_SOURCE

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Wdg_30_TLE4278G.h"
#include "Wdg_30_TLE4278G_Mode.h"

#include "Wdg_30_TLE4278G_LL.h"
#include "Wdg_30_TLE4278G_Timer.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (WDG_30_TLE4278G_LOCAL) /* COV_WDG_30_TLE4278G_COMPATIBILITY */
# define WDG_30_TLE4278G_LOCAL static
#endif

#if !defined (WDG_30_TLE4278G_LOCAL_INLINE) /* COV_WDG_30_TLE4278G_COMPATIBILITY */
# define WDG_30_TLE4278G_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 * LOCAL VARIABLES
 *********************************************************************************************************************/
#define WDG_30_TLE4278G_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Stores the current mode of the module */
WDG_30_TLE4278G_LOCAL VAR(WdgIf_ModeType, WDG_30_TLE4278G_VAR_INIT) Wdg_30_TLE4278G_Mode_currentMode = WDGIF_OFF_MODE;

#define WDG_30_TLE4278G_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define WDG_30_TLE4278G_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * Wdg_30_TLE4278G_Mode_DoModeChange()
 *********************************************************************************************************************/
/*! \brief      Perform transition from current mode to a another mode
 *  \pre        -
 *  \details    This function performs the transition to another mode. This includes setting the hardware and adapting
 *              the trigger timing.
 *  \return     E_OK - Transition to WDGIF_OFF_MODE was successful.
 *  \return     E_NOT_OK - Transition to WDGIF_OFF_MODE failed.
 *********************************************************************************************************************/
WDG_30_TLE4278G_LOCAL_INLINE FUNC(Std_ReturnType, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Mode_DoModeChange(WdgIf_ModeType Mode);


/**********************************************************************************************************************
 * GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_Mode_GetMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(WdgIf_ModeType, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Mode_GetMode(void)
{
  /* #10 Return value of Wdg_30_TLE4278G_Mode_currentMode */
  return Wdg_30_TLE4278G_Mode_currentMode;
}

/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_Mode_SetMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Mode_SetMode(WdgIf_ModeType Mode)
{
  Std_ReturnType retVal;
  /* #10 If requested mode equals current mode */
  if(Wdg_30_TLE4278G_Mode_currentMode == Mode)
  {
    retVal = E_OK;
  }
  /* #20 Otherwise do the transition to the requested running mode */
  else 
  {
    Wdg_30_TLE4278G_EnterCritical(WDG_30_TLE4278G_EXCLUSIVE_AREA_0);
    /* >>> CRITICAL AREA START >>> */

    retVal = Wdg_30_TLE4278G_Mode_DoModeChange(Mode);

    /* <<< CRITICAL AREA END <<< */ 
    Wdg_30_TLE4278G_ExitCritical(WDG_30_TLE4278G_EXCLUSIVE_AREA_0);
  }

  return retVal;
}


/**********************************************************************************************************************
 * LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Wdg_30_TLE4278G_Mode_DoModeChange()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
WDG_30_TLE4278G_LOCAL_INLINE FUNC(Std_ReturnType, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Mode_DoModeChange(WdgIf_ModeType Mode)
{
  Std_ReturnType retVal = E_NOT_OK;
  
  /* #10 Set hardware to requested mode and if it succeeds adapt timing and return E_OK */
  if (Wdg_30_TLE4278G_LL_SetHwMode(Mode) == E_OK)
  {
    Wdg_30_TLE4278G_Timer_SetTriggerTiming(Mode);
    Wdg_30_TLE4278G_Mode_currentMode = Mode;
    retVal = E_OK;
  }
  
  /* #20 Otherwise report DEM error EModeFaild */
  else
  {
    Dem_ReportErrorStatus(Wdg_30_TLE4278G_ConfigPtr->EModeFailed, DEM_EVENT_STATUS_FAILED);
  }
  
  return retVal;
}

#define WDG_30_TLE4278G_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* START_COVERAGE_JUSTIFICATION

 \ID COV_WDG_30_TLE4278G_COMPATIBILITY
 \ACCEPT TX
 \REASON [COV_MSR_COMPATIBILITY]

 END_COVERAGE_JUSTIFICATION */

/**********************************************************************************************************************
 *  END OF FILE: Wdg_30_TLE4278G_Mode.c
 *********************************************************************************************************************/
