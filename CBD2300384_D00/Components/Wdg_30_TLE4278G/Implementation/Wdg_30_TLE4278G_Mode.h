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
/*!        \file  Wdg_30_TLE4278G_Mode.h
 *        \brief  Header file of Wdg_30_TLE4278G_Mode
 *
 *      \details  This sub-component provides mode-specific services to other sub-component.
 *                This is an internal include file - it shall not be included by other modules than Wdg_30_TLE4278G.
 *
 *********************************************************************************************************************/

#if !(defined WDG_30_TLE4278G_MODE_H)
#define WDG_30_TLE4278G_MODE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Wdg_30_TLE4278G_Cfg.h"
#include "Wdg_30_TLE4278G_LL.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define WDG_30_TLE4278G_MODE_SUPPORT_FLAG (0x1u)

 /*********************************************************************************************************************
 * FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 1 */ /* MD_MSR_FctLikeMacro */
#define Wdg_30_TLE4278G_Mode_IsModeAllowed(modeIdx) ((boolean)(((uint8)(((WDG_30_TLE4278G_SUPPORTED_MODES) >> ((uint8)(modeIdx))) & (WDG_30_TLE4278G_MODE_SUPPORT_FLAG))) == WDG_30_TLE4278G_MODE_SUPPORT_FLAG))

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define WDG_30_TLE4278G_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * Wdg_30_TLE4278G_Mode_GetMode()
 *********************************************************************************************************************/
/*! \brief       Returns the current watchdog mode.
 *  \pre         -
 *  \return      Current mode of the of the watchdog.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \details     The function returns the current mode of the module. Depending on the configuration this could be:\n
 *               -WDGIF_OFF_MODE
 *               -WDGIF_SLOW_MODE
 *               -WDGIF_FAST_MODE
 *  \trace       CREQ-105560, DSGN-Wdg22902
 *********************************************************************************************************************/
FUNC(WdgIf_ModeType, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Mode_GetMode(void);

/**********************************************************************************************************************
 * Wdg_30_TLE4278G_Mode_SetMode()
 *********************************************************************************************************************/
/*! \brief       Coordinates a mode change.
 *  \pre         -
 *  \param[in]   Mode   Valid mode to which the watchdog shall switch.
 *  \return      E_OK on success
 *               E_NOT_OK otherwise
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \details     This function sets the watchdog hardware to the correct mode and resets the timing setup (which
 *               includes watchdog triggering and trigger condition) properly.
 *  \trace       CREQ-105560, DSGN-Wdg22902, DSGN-Wdg22983
 *********************************************************************************************************************/
FUNC(Std_ReturnType, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Mode_SetMode(WdgIf_ModeType Mode);


#define WDG_30_TLE4278G_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* WDG_30_TLE4278G_MODE_H */

/**********************************************************************************************************************
 *  END OF FILE: Wdg_30_TLE4278G_Mode.h
 *********************************************************************************************************************/
