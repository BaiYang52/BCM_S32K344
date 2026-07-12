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
/*!        \file  Wdg_30_TLE4278G_Cbk.h
 *        \brief  Declaration of callback routines
 *
 *      \details  This file declares callback functions Wdg_30_TLE4278G_Cbk_TriggerNotification and Wdg_30_TLE4278G_Cbk_PermissionNotification
 *
 *********************************************************************************************************************/

#if !defined (WDG_30_TLE4278G_CBK_H)
# define WDG_30_TLE4278G_CBK_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Wdg_30_TLE4278G_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define WDG_30_TLE4278G_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * Wdg_30_TLE4278G_Cbk_GptNotificationTrigger()
 *********************************************************************************************************************/
/*! \brief       This callback function triggers the watchdog hardware.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \details     The main purpose of this function is to trigger the watchdog hardware.
 *               Additionally it triggers the update of the trigger condition and initiates a reset of the timing
 *               if the watchdog hardware requires an asynchronous mode change.
 *   \trace      CREQ-105563, DSGN-Wdg22903
 *********************************************************************************************************************/
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Cbk_GptNotificationTrigger(void);

#define WDG_30_TLE4278G_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif /* WDG_30_TLE4278G_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: Wdg_30_TLE4278G_Cbk.h
 *********************************************************************************************************************/
