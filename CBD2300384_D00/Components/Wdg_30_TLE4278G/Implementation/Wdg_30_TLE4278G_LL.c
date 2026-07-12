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
/*!        \file  Wdg_30_TLE4278G_LL.c
 *        \brief  Implementation of sub-module Wdg_30_TLE4278G_LL
 *         
 *
 *      \details  This sub-module abstracts hardware access to the external watchdog peripheral by providing APIs to 
 *                higher-level sub-modules. Due to it's high adaptability via configuration it supports a large number 
 *                of external watchdog peripherals.
 *********************************************************************************************************************/

#define WDG_30_TLE4278G_SOURCE

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Wdg_30_TLE4278G_LL.h"
#include "Wdg_30_TLE4278G_TrgCnd.h"
#include "Wdg_30_TLE4278G_Mode.h"


/*!
 * Internal comment removed. *
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

/**********************************************************************************************************************
 *  LOCAL MACROS
 *********************************************************************************************************************/


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
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define WDG_30_TLE4278G_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (defined (WDG_30_TLE4278G_MODE_FAST_IDX_MAX) || \
     defined (WDG_30_TLE4278G_MODE_SLOW_IDX_MAX) || \
     defined (WDG_30_TLE4278G_MODE_OFF_IDX_MAX))

/**********************************************************************************************************************
  Wdg_30_TLE4278G_LL_setModeDioLevels()
**********************************************************************************************************************/
/*! \brief         Sets the physical levels to configure external watchdog device.
 *
 *  \details       Sets the physical levels of the watchdog mode to configure the behaviour of external watchdog device. 
 * 
 *  \param[in]     DioLevelAssignments  Pointer to the array which stores assigned Dio levels for a specific mode.
 *  \param[in]     maxIdx     Index of the last element in DioLevelAssignments.
 *
 *  \return        -
 * 
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        (defined (WDG_30_TLE4278G_MODE_FAST_IDX_MAX) || defined (WDG_30_TLE4278G_MODE_FAST_IDX_MAX) 
 *                 || defined (WDG_30_TLE4278G_MODE_FAST_IDX_MAX))
 *  \pre           Module is initialized
 *  \note          Gets called from Wdg_30_TLE4278G_LL_SetHwMode in case ModeLevelAssignments are configured for a 
 *                 specific mode.
 */
LOCAL_INLINE FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_LL_setModeDioLevels(
  P2CONST(Wdg_30_TLE4278G_ModeDioLevelAssignmentType, AUTOMATIC, WDG_30_TLE4278G_CONST) DioLevelAssignments,
  uint16 maxIdx
);


/**********************************************************************************************************************
 * LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
  Wdg_30_TLE4278G_LL_setModeDioLevels()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_LL_setModeDioLevels(
  P2CONST(Wdg_30_TLE4278G_ModeDioLevelAssignmentType, AUTOMATIC, WDG_30_TLE4278G_CONST) DioLevelAssignments,
  uint16 maxIdx
)
{
  uint16 i;

  for(i = 0; i < maxIdx; i++)
  {
    Wdg_30_TLE4278G_DioWriteChannel(
      DioLevelAssignments[i].channel, 
      DioLevelAssignments[i].level
    );
  }
}
#endif
/**********************************************************************************************************************
 * GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_LL_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_LL_Init(void)
{
  /* #10 Enable Gpt Notifications */
  Gpt_EnableNotification(Wdg_30_TLE4278G_ConfigPtr->TriggerTimer);

  /* 20 Set initial level at WDI pin */
  Wdg_30_TLE4278G_DioWriteChannel(Wdg_30_TLE4278G_ConfigPtr->DioPinWdiChannel,
    Wdg_30_TLE4278G_ConfigPtr->DioPinWdiInitLevel);

#if WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF)
  /* #30 If WDGIF_OFF_MODE is supported synchronize hardware to module (WDGIF_OFF_MODE is initial mode) */
  return Wdg_30_TLE4278G_LL_SetHwMode(WDGIF_OFF_MODE);
#else
  return E_OK;
#endif


}

/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_LL_TriggerHw()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_LL_TriggerHw(void)
{

  /* #10 Trigger the watchdog hardware */
  /* PRQA S 4304 1 */ /* MD_MSR_AutosarBoolean */
  Dio_LevelType toggledTriggerLvl = (Dio_LevelType)(Wdg_30_TLE4278G_DioReadChannel(Wdg_30_TLE4278G_ConfigPtr->DioPinWdiChannel) == 0u);
  Wdg_30_TLE4278G_DioWriteChannel(Wdg_30_TLE4278G_ConfigPtr->DioPinWdiChannel, toggledTriggerLvl);
  

#if (defined WDG_30_TLE4278G_ASYNC_DUTY_CYCLE)
  /* #20 If asynchronous duty-cycle is enabled restart trigger timer */
  Wdg_30_TLE4278G_LL_StopTriggerTimer();
  Wdg_30_TLE4278G_LL_StartTriggerTimer(Wdg_30_TLE4278G_Mode_GetMode());
#endif

}

/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_LL_SetHwMode()
 *********************************************************************************************************************/
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
FUNC(Std_ReturnType, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_LL_SetHwMode(WdgIf_ModeType Mode)
{
  /* PRQA S 2981 1 */ /* MD_MSR_RetVal */
  Std_ReturnType retVal = E_NOT_OK;

  switch(Mode) /* PRQA S 2002 1 */ /* MD_MSR_EmptyClause */
  {
    
    case WDGIF_FAST_MODE:
    /* ----------------- WDGIF_FAST_MODE --------------------------------------------------------------------------- */
    /* #10 If requested mode is WDGIF_FAST_MODE */
#if defined (WDG_30_TLE4278G_MODE_FAST_IDX_MAX)
      /* #20 Set external hardware to WDGIF_FAST_MODE */
      Wdg_30_TLE4278G_LL_setModeDioLevels(Wdg_30_TLE4278G_ConfigPtr->DioModeLevelsFast, /* SBSW_WDG_30_TLE4278G_01 */
        WDG_30_TLE4278G_MODE_FAST_IDX_MAX);
#endif
      /* #30 Return E_OK */
      retVal = E_OK;
      break;


#if defined (WDG_30_TLE4278G_MODE_SLOW_IDX_MAX)
    /* ----------------- WDGIF_SLOW_MODE --------------------------------------------------------------------------- */
      /* #50 If requested mode is WDGIF_SLOW_MODE */
    case WDGIF_SLOW_MODE:
      /* #60 Set external hardware to WDGIF_SLOW_MODE */
      Wdg_30_TLE4278G_LL_setModeDioLevels(Wdg_30_TLE4278G_ConfigPtr->DioModeLevelsSlow, /* SBSW_WDG_30_TLE4278G_01 */
        WDG_30_TLE4278G_MODE_SLOW_IDX_MAX);

      /* #70 Return E_OK */
      retVal = E_OK;
      break;
#endif


#if WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF)
    /* ----------------- WDGIF_OFF_MODE ---------------------------------------------------------------------------- */
    default:
    /* #100 If requested mode is WDGIF_OFF_MODE */
#if defined (WDG_30_TLE4278G_ENABLE_IRREVERSIBLE)
      /* #110 If ENABLE_IRREVERSIBLE deny WDGIF_OFF_MODE requests once the watchdog was activated */
      if(Wdg_30_TLE4278G_Mode_GetMode() != WDGIF_OFF_MODE) 
      {
        /* #120 Return E_NOT_OK */
        retVal = E_NOT_OK;
      }
      else
# endif
      {
#if defined (WDG_30_TLE4278G_MODE_OFF_IDX_MAX)
        /* #130 Set external hardware to WDGIF_OFF_MODE */
        Wdg_30_TLE4278G_LL_setModeDioLevels(Wdg_30_TLE4278G_ConfigPtr->DioModeLevelsOff, /* SBSW_WDG_30_TLE4278G_01 */
          WDG_30_TLE4278G_MODE_OFF_IDX_MAX);
#endif
        /* #140 Return E_OK */
        retVal = E_OK;
      }
      break;
#endif
  } /* PRQA S 2002 */ /* MD_Wdg_30_TLE4278G_2002 */

  return retVal;
}

/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_LL_StartTriggerTimer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_LL_StartTriggerTimer(WdgIf_ModeType Mode)
{ 
  Gpt_ValueType tickValue;

#if (defined WDG_30_TLE4278G_ASYNC_DUTY_CYCLE)
  if(Wdg_30_TLE4278G_DioReadChannel(Wdg_30_TLE4278G_ConfigPtr->DioPinWdiChannel) 
    != Wdg_30_TLE4278G_ConfigPtr->DioPinWdiInitLevel)
  {
    /* PRQA S 3382 3 */ /* MD_Wdg_30_TLE4278G_3382 */
    /* PRQA S 3689 2 */ /* MD_Wdg_30_TLE4278G_3689 */
    tickValue = Wdg_30_TLE4278G_ConfigPtr->RecoverTimerCounter[WDG_30_TLE4278G_MODE_INDEX(Mode)];
  }
  else
#endif
  {
    /* #25 Start trigger timer with conventional trigger value */
    /* PRQA S 3382 3 */ /* MD_Wdg_30_TLE4278G_3382 */
    /* PRQA S 3689 2 */ /* MD_Wdg_30_TLE4278G_3689 */
    tickValue = Wdg_30_TLE4278G_ConfigPtr->TriggerTimerCounter[WDG_30_TLE4278G_MODE_INDEX(Mode)];
  }
 
  Gpt_StartTimer(Wdg_30_TLE4278G_ConfigPtr->TriggerTimer, tickValue);
}

#if ((WDG_30_TLE4278G_SUPPORTED_MODES != WDG_30_TLE4278G_MODES_FAST) \
       || (defined WDG_30_TLE4278G_ASYNC_DUTY_CYCLE))
/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_LL_StopTriggerTimer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_LL_StopTriggerTimer(void)
{
  Gpt_StopTimer(Wdg_30_TLE4278G_ConfigPtr->TriggerTimer);
}
#endif

#define WDG_30_TLE4278G_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* Justification for module-specific MISRA deviations:

MD_Wdg_30_TLE4278G_2002:
  Misra Rule 15.5
  Reason: No default case for switch-statement in case WDGIF_OFF_MODE_is not supported.
  Risk: None, as only valid parameters are passed to this function/switch-statement.
  Prevention: Testing ensures the correct behaviour of this function.

MD_Wdg_30_TLE4278G_3382:
  Misra Rule 21.1
  Reason: '#define WDG_30_TLE4278G_MODE_INDEX(X) (uint8)(X)-1u' is used to calculate a corresponding array index
  for a passed mode. If the macro would be used with X = WDGIF_OFF_MODE (0) a wrap-around would happen.
  As the macro only is used with X = WDGIF_SLOW_MODE (1) or WDGIF_FAST_MODE (2) this wrap-around never
  will  happen.
  Risk: No risk.
  Prevention: Code inspection.

MD_Wdg_30_TLE4278G_3689:
  Misra Rule 21.1
  Reason: '#define WDG_30_TLE4278G_MODE_INDEX(X) (uint8)(X)-1u' is used to calculate a corresponding array index
  for a passed mode. If the macro would be used with X = WDGIF_OFF_MODE (0) an access outside the array
  boundaries would happen. As the macro only is used with X = WDGIF_SLOW_MODE (1) or WDGIF_FAST_MODE (2)
  the access will always be inside the array boundaries.
  Risk: No risk.
  Prevention: Code inspection.

*/

/* SBSW_JUSTIFICATION_BEGIN

  \ID SBSW_WDG_30_TLE4278G_01
  \DESCRIPTION Function is called with pointer as a parameter.
  \COUNTERMEASURE \N Function uses the parameter for read-only access.

SBSW_JUSTIFICATION_END */

/* START_COVERAGE_JUSTIFICATION

  \ID COV_WDG_30_TLE4278G_COMPATIBILITY
  \ACCEPT TX
  \REASON [COV_MSR_COMPATIBILITY]

END_COVERAGE_JUSTIFICATION */ 


/**********************************************************************************************************************
 *  END OF FILE: Wdg_30_TLE4278G_LL.c
 *********************************************************************************************************************/
