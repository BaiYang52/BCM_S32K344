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
/*!        \file  Wdg_30_TLE4278G_Timer.c
 *        \brief  Implementation of sub-module Wdg_30_TLE4278G_Timer.c
 *
 *      \details  This sub-module provides timing services to other sub-modules.
 *
 *********************************************************************************************************************/

#define WDG_30_TLE4278G_SOURCE

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Wdg_30_TLE4278G_Timer.h"
#include "Wdg_30_TLE4278G.h"

#include "Wdg_30_TLE4278G_TrgCnd.h"

#include "Wdg_30_TLE4278G_TrgCnd_Cbk.h"

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
#define WDG_30_TLE4278G_START_SEC_VAR_ZERO_INIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Keeps track how often the watchdog hardware may be triggered until TriggerCondition gets invalid */
WDG_30_TLE4278G_LOCAL VAR(uint32, WDG_30_TLE4278G_VAR_INIT) Wdg_30_TLE4278G_Timer_permissionCounter = 0;

#define WDG_30_TLE4278G_STOP_SEC_VAR_ZERO_INIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if(((WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_SLOW)) \
  || WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF) && WDG_30_TLE4278G_DISABLE_ALLOWED == STD_ON))

#define WDG_30_TLE4278G_START_SEC_VAR_ZERO_INIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Stores the current timing mode. */
WDG_30_TLE4278G_LOCAL VAR(WdgIf_ModeType, WDG_30_TLE4278G_VAR_INIT) Wdg_30_TLE4278G_Timer_currentMode = WDGIF_OFF_MODE;

#define WDG_30_TLE4278G_STOP_SEC_VAR_ZERO_INIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if (WDG_30_TLE4278G_MODE_CHANGE_ASYNC == STD_ON)

# define WDG_30_TLE4278G_START_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Indicates whether a mode change should be initiated after next trigger. */
WDG_30_TLE4278G_LOCAL VAR(boolean, WDG_30_TLE4278G_VAR_INIT) Wdg_30_TLE4278G_Timer_pendingModeFlag = FALSE;

# define WDG_30_TLE4278G_STOP_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define WDG_30_TLE4278G_START_SEC_VAR_NOINIT_32BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Stores the mode to which should be changed after next trigger. */
WDG_30_TLE4278G_LOCAL VAR(WdgIf_ModeType, WDG_30_TLE4278G_VAR_INIT) Wdg_30_TLE4278G_Timer_pendingMode;

# define WDG_30_TLE4278G_STOP_SEC_VAR_NOINIT_32BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (WDG_30_TLE4278G_MODE_CHANGE_ASYNC == STD_ON) */

#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define WDG_30_TLE4278G_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_SLOW) \
  || (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF) && WDG_30_TLE4278G_DISABLE_ALLOWED == STD_ON))
/**********************************************************************************************************************
 Wdg_30_TLE4278G_Timer_DoTimingChange()
 *********************************************************************************************************************/
/*! \brief      Performs change of trigger timing
 *  \details    This function performs the changes of trigger timing that can be (depending on the watchdog hardware)
 *              performed synchronous or asynchronous. The function is called either instantly during the mode change
 *              request  (synchronous mode change) or during trigger timer callback (asynchronous mode change).
 *  \param[in]  targetMode                   Mode to which the trigger timing should be adapted.
 *  \config     (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_SLOW) ||
 *              (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF) && WDG_30_TLE4278G_DISABLE_ALLOWED == STD_ON)
 *  \pre        -
 *********************************************************************************************************************/
WDG_30_TLE4278G_LOCAL_INLINE FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Timer_DoTimingChange(WdgIf_ModeType targetMode);
#endif

#if (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF) && WDG_30_TLE4278G_DISABLE_ALLOWED == STD_ON)
/**********************************************************************************************************************
 Wdg_30_TLE4278G_Timer_SetTimingOff()
 *********************************************************************************************************************/
/*! \brief      Switches off trigger timing.
 *  \details    This function stops the trigger timer. The function only is required if watchdog can be set to OFF mode.
 *  \config     WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF) && WDG_30_TLE4278G_DISABLE_ALLOWED == STD_ON
 *  \pre        -
 *********************************************************************************************************************/
WDG_30_TLE4278G_LOCAL_INLINE FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Timer_SetTimingOff(void);
#endif

#if (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_SLOW))
/**********************************************************************************************************************
 Wdg_30_TLE4278G_Timer_SwitchTimingMode()
 *********************************************************************************************************************/
/*! \brief      Adapts the trigger timing between running modes.
 *  \details    This function restarts the trigger timer according to the requested mode and recalculates the per-
 *              mission counter. (The permission counter represents the value how often the timer callback function can
 *              be called until the trigger condition has to be set to invalid (timeout expires). As the number of
 *              calls for a fix timeout depends on the set mode (trigger interval) the value has to be converted.)
 *              The function is only required if both running modes are available.
 *  \param[in]  targetMode        Mode to which the trigger timing should be adapted.
 *  \config     WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_SLOW)
 *  \pre        -
 *********************************************************************************************************************/
WDG_30_TLE4278G_LOCAL_INLINE FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Timer_SwitchTimingMode(WdgIf_ModeType targetMode);
#endif
/**********************************************************************************************************************
 Wdg_30_TLE4278G_Timer_StartTriggerTimer()
 *********************************************************************************************************************/
/*! \brief      Starts the trigger timer.
 *  \details    This function is used by other functions to start the trigger timer.
 *  \param[in]  Mode              Mode for which the trigger timing should be started.
 *  \pre        -
 *********************************************************************************************************************/
WDG_30_TLE4278G_LOCAL_INLINE FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Timer_StartTriggerTimer(WdgIf_ModeType Mode);

#if ((WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF) && (WDG_30_TLE4278G_DISABLE_ALLOWED == STD_ON)) \
  || (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_SLOW)))
/**********************************************************************************************************************
 Wdg_30_TLE4278G_Timer_StopTriggerTimer()
 *********************************************************************************************************************/
/*! \brief      Stops the trigger timer.
 *  \details    This function is used by other functions to stop the trigger timer.
 *              The function is necessary if SLOW mode is supported (a reset consists of stopping and starting the
 *              trigger timer), or OFF mode is supported and disable is allowed.
 *  \config     (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_SLOW) ||
 *              (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF) && (WDG_30_TLE4278G_DISABLE_ALLOWED == STD_ON))
 *  \pre        -
 *********************************************************************************************************************/
WDG_30_TLE4278G_LOCAL_INLINE FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Timer_StopTriggerTimer(void);
#endif

/**********************************************************************************************************************
 * GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_Timer_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Timer_Init(void)
{
  /* #10 Call Wdg_30_TLE4278G_LL_Init */
  return Wdg_30_TLE4278G_LL_Init();
}

/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_Timer_SetTimeout()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Timer_SetTimeout(uint16 timeout, WdgIf_ModeType targetMode)
{
  /* #10 Calculate Wdg_30_TLE4278G_Timer_permissionCounter */

  Wdg_30_TLE4278G_Timer_permissionCounter = (uint32)timeout / Wdg_30_TLE4278G_ConfigPtr->TriggerCycleDuration[WDG_30_TLE4278G_MODE_INDEX(targetMode)];
}

/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_Timer_TriggerCbk()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Timer_TriggerCbk(void)
{

  /* #10 Invalidate trigger condition if permission counter is 0 otherwise decrement permission counter */
  if (Wdg_30_TLE4278G_Timer_permissionCounter == 0u)
  {
    Wdg_30_TLE4278G_TrgCnd_InvalidateCondition();
  }
  else
  {
    Wdg_30_TLE4278G_Timer_permissionCounter--;
  }

  /* #20 Trigger the hardware watchdog if trigger condition is true */
  if (Wdg_30_TLE4278G_TrgCnd_CheckCondition() == TRUE)
  {
    Wdg_30_TLE4278G_LL_TriggerHw();
  }

#if((WDG_30_TLE4278G_MODE_CHANGE_ASYNC == STD_ON) && ((WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_SLOW) \
  || (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF) && WDG_30_TLE4278G_DISABLE_ALLOWED == STD_ON))))
  /* #30 Reset timers if an asynchronous mode change is pending */
  if (Wdg_30_TLE4278G_Timer_pendingModeFlag == TRUE)
  {
    Wdg_30_TLE4278G_Timer_DoTimingChange(Wdg_30_TLE4278G_Timer_pendingMode);
  }
#endif

}

/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_Timer_SetTriggerTiming()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 * 
*/
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Timer_SetTriggerTiming(WdgIf_ModeType targetMode)
{

#if (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_SLOW) \
  || (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF) && WDG_30_TLE4278G_DISABLE_ALLOWED == STD_ON))
  /* #10 If timing should switch from a running mode do switch or set pending mode  */
  if (Wdg_30_TLE4278G_Timer_currentMode != WDGIF_OFF_MODE)
  {
# if (WDG_30_TLE4278G_MODE_CHANGE_ASYNC == STD_ON)
    Wdg_30_TLE4278G_Timer_pendingModeFlag = TRUE;
    Wdg_30_TLE4278G_Timer_pendingMode = targetMode;
# else
    Wdg_30_TLE4278G_Timer_DoTimingChange(targetMode);
# endif
  }
  /* #20 If timing should switch from an off mode to running mode, perform the request directly */
  else 
#endif
  {
    Wdg_30_TLE4278G_TrgCnd_SetCondition(WDG_30_TLE4278G_INITIAL_TIMEOUT, targetMode);
    Wdg_30_TLE4278G_Timer_StartTriggerTimer(targetMode);

#if(((WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_SLOW)) \
  || WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF) && WDG_30_TLE4278G_DISABLE_ALLOWED == STD_ON))
    Wdg_30_TLE4278G_Timer_currentMode = targetMode;
#endif
  }
}

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_SLOW) \
  || (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF) && WDG_30_TLE4278G_DISABLE_ALLOWED == STD_ON))
/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_Timer_DoTimingChange()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 * 
*/
WDG_30_TLE4278G_LOCAL_INLINE FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Timer_DoTimingChange(WdgIf_ModeType targetMode)
{
  /* PRQA S 2002 2 */ /* MD_Wdg_30_TLE4278G_2002 */
  /* PRQA S 3315 1 */ /* MD_Wdg_30_TLE4278G_3315 */
  switch(targetMode)
  {
#if (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF) && WDG_30_TLE4278G_DISABLE_ALLOWED == STD_ON)
    /* #10 If requested mode is off mode, forward request do SetTimingOff */
    case WDGIF_OFF_MODE:
      Wdg_30_TLE4278G_Timer_SetTimingOff();
      break;
#endif
#if (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_SLOW))
    /* #20 If requested mode is running mode, forward request to SwitchTimingMode */
    default: /* WDGIF_SLOW_MODE, WDGIF_FAST_MODE */
      Wdg_30_TLE4278G_Timer_SwitchTimingMode(targetMode);
      break;
#endif
  }

  /* #30 Updated current mode of sub-component */
  Wdg_30_TLE4278G_Timer_currentMode = targetMode;

  /* Delete pending mode for asynchronous mode changes */
# if (WDG_30_TLE4278G_MODE_CHANGE_ASYNC == STD_ON)
  Wdg_30_TLE4278G_Timer_pendingModeFlag = FALSE;
# endif
  }
#endif

#if (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF) && WDG_30_TLE4278G_DISABLE_ALLOWED == STD_ON)
/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_Timer_SetTimingOff()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
WDG_30_TLE4278G_LOCAL_INLINE FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Timer_SetTimingOff(void)
{
  /* #10 Stop trigger timer */
  Wdg_30_TLE4278G_Timer_StopTriggerTimer();

  /* #20 Set permission counter to 0 */
  Wdg_30_TLE4278G_Timer_permissionCounter = 0;

  /* #30 Reset the trigger condition */
  Wdg_30_TLE4278G_TrgCnd_ResetCondition();
}
#endif

#if (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_SLOW))
/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_Timer_SwitchTimingMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
WDG_30_TLE4278G_LOCAL_INLINE FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Timer_SwitchTimingMode(WdgIf_ModeType targetMode)
{
  /* #10 Stop trigger timer */
  Wdg_30_TLE4278G_Timer_StopTriggerTimer();

  /* #20 Recalculate value of PermissionCounter */
  Wdg_30_TLE4278G_Timer_permissionCounter = (Wdg_30_TLE4278G_Timer_permissionCounter
    * Wdg_30_TLE4278G_ConfigPtr->TriggerTimerCounter[WDG_30_TLE4278G_MODE_INDEX(Wdg_30_TLE4278G_Timer_currentMode)])
    / Wdg_30_TLE4278G_ConfigPtr->TriggerTimerCounter[WDG_30_TLE4278G_MODE_INDEX(targetMode)];

  /* #30 Restart the trigger timer */
  Wdg_30_TLE4278G_Timer_StartTriggerTimer(targetMode);
}
#endif

/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_Timer_StartTriggerTimer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
WDG_30_TLE4278G_LOCAL_INLINE FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Timer_StartTriggerTimer(WdgIf_ModeType Mode)
{
  /* #10 Start GptChannel TriggerTimer */
  Wdg_30_TLE4278G_LL_StartTriggerTimer(Mode);
}

#if ((WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF) && (WDG_30_TLE4278G_DISABLE_ALLOWED == STD_ON)) \
|| (WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_SLOW)))
/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_Timer_StopTriggerTimer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
WDG_30_TLE4278G_LOCAL_INLINE FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Timer_StopTriggerTimer(void)
{
  /*  #10 Forward call to sub-component Wdg_30_TLE4278G_LL */
  Wdg_30_TLE4278G_LL_StopTriggerTimer();
}
#endif

#define WDG_30_TLE4278G_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:

 MD_Wdg_30_TLE4278G_2002:
 Misra Rule 15.3
 Reason: Depending on the module configuration (hardware support) the default case is removed by preprocessor.
 Risk: None, as only valid parameters are passed to this function/switch-statement.
 Prevention: Testing ensures the correct behaviour of this function.

 MD_Wdg_30_TLE4278G_3315:
 Misra Rule 15.5
 Reason: Depending on the module configuration (hardware support) other cases of the switch statement is removed
 by preprocessor.
 Risk: None, as only valid parameters are passed to this function/switch-statement.
 Prevention: Testing ensures the correct behaviour of this function.

 */

/* START_COVERAGE_JUSTIFICATION

 \ID COV_WDG_30_TLE4278G_COMPATIBILITY
 \ACCEPT TX
 \REASON [COV_MSR_COMPATIBILITY]

 END_COVERAGE_JUSTIFICATION */

/**********************************************************************************************************************
 *  END OF FILE: Wdg_30_TLE4278G_Timer.c
 *********************************************************************************************************************/
