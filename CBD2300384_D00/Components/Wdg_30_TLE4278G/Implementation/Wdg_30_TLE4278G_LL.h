/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Wdg_30_TLE4278G_LL.h
 *        \brief  Header file of Wdg_30_TLE4278G_LL
 *
 *      \details  This sub-module abstracts hardware access to the external watchdog peripheral by providing APIs to 
 *                higher-level sub-modules. Due to it's high adaptability via configuration it supports a large number 
 *                of external watchdog peripherals.
 *********************************************************************************************************************/

#if !(defined WDG_30_TLE4278G_LL_H)
#define WDG_30_TLE4278G_LL_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Wdg_30_TLE4278G_Cfg.h"

/**********************************************************************************************************************
 * GLOBAL MACROS
 *********************************************************************************************************************/
/*! Bitmasks of possible supported mode sets  */
#define WDG_30_TLE4278G_MODES_ALL       (0x7u)
#define WDG_30_TLE4278G_MODES_FAST_SLOW (0x6u)
#define WDG_30_TLE4278G_MODES_FAST_OFF  (0x5u)
#define WDG_30_TLE4278G_MODES_FAST      (0x4u)

/*! Mode bitmasks */
#define WDG_30_TLE4278G_MODE_FAST       (0x4u)
#define WDG_30_TLE4278G_MODE_SLOW       (0x2u)
#define WDG_30_TLE4278G_MODE_OFF        (0x1u)

/* PRQA S 3453 1 */ /* MD_MSR_FctLikeMacro */
#define WDG_30_TLE4278G_IS_MODE_SUPPORTED(mode) ((WDG_30_TLE4278G_SUPPORTED_MODES & (mode)) == (mode))


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define WDG_30_TLE4278G_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * Wdg_30_TLE4278G_LL_Init()
 *********************************************************************************************************************/
/*! \brief       Function to initialize the sub-component Wdg_30_TLE4278G_LL.
 *  \return      E_OK on success.
 *               E_NOT_OK otherwise.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \details     This function initializes the sub-component Wdg_30_TLE4278G_LL. It enables notifications for the 
 *               referenced Gpt channel and  sets the initial physical level for triggering Dio channel (WDI). If
 *               supported, the connected peripheral gets disabled to be synchronized to the modules initial mode
 *               (WDGIF_OFF_MODE).
 *  \trace       DSGN-Wdg22909
 *********************************************************************************************************************/
FUNC(Std_ReturnType, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_LL_Init(void);


/**********************************************************************************************************************
 * Wdg_30_TLE4278G_LL_TriggerHw()
 *********************************************************************************************************************/
/*! \brief       Function that triggers the watchdog hardware
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \details     This function triggers the watchdog hardware by toggling the output level of the referenced DioChannel.
 *               It is called cyclically until the trigger condition is valid.
 *               For DeviceType TLE7368 it additionally recovers the trigger output after initialization procedure and 
 *               adapts trigger timing.
 *  \trace       DSGN-Wdg22903
 *********************************************************************************************************************/
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_LL_TriggerHw(void);


/**********************************************************************************************************************
 * Wdg_30_TLE4278G_LL_SetHwMode()
 *********************************************************************************************************************/
/*! \brief       Function that sets the mode to watchdog to passed mode.
 *  \param[in]   Mode   Mode to which the watchdog hardware should be set
 *  \return      E_OK in case the mode change request was executed successfully.
 *               E_NOT_OK in case the mode change request was not successful.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \details     This function sets the watchdog hardware to the passed mode by setting output levels to respective 
 *               DioChannels.
 *  \trace       DSGN-Wdg22902
 *********************************************************************************************************************/
FUNC(Std_ReturnType, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_LL_SetHwMode(WdgIf_ModeType Mode);


/**********************************************************************************************************************
 * Wdg_30_TLE4278G_LL_StartTriggerTimer()
 *********************************************************************************************************************/
/*! \brief       This function starts the timer that is used for watchdog trigger timing.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \details     This function starts the rewrapping timer that is used for watchdog trigger timing by using the 
 *               Gpt module. 
 *               In case an asynchronous duty-cycle is used for triggering/recovering the timer is restarted with
 *               alternating timing values dependent on the current physical level at the trigger pin.
 *  \trace       DSGN-Wdg22903
 *********************************************************************************************************************/
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_LL_StartTriggerTimer(WdgIf_ModeType Mode);

#if ((WDG_30_TLE4278G_SUPPORTED_MODES != WDG_30_TLE4278G_MODES_FAST) \
       || (defined WDG_30_TLE4278G_ASYNC_DUTY_CYCLE))
/**********************************************************************************************************************
 * Wdg_30_TLE4278G_LL_StopTriggerTimer()
 *********************************************************************************************************************/
/*! \brief       This function stops the timer that is used for watchdog trigger timing.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      (WDG_30_TLE4278G_SUPPORTED_MODES != WDG_30_TLE4278G_MODES_FAST)
 *                 || (defined WDG_30_TLE4278G_ASYNC_DUTY_CYCLE)
 *  \details     This function starts the rewrapping timer that is used for watchdog trigger timing by using the 
 *               Gpt module.
 *  \trace       DSGN-Wdg22903
 *********************************************************************************************************************/
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_LL_StopTriggerTimer(void);
#endif

#define WDG_30_TLE4278G_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* WDG_30_TLE4278G_LL_H */

/**********************************************************************************************************************
 *  END OF FILE: Wdg_30_TLE4278G_LL.h
 *********************************************************************************************************************/
