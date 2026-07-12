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
/*!        \file  Wdg_30_TLE4278G_TrgCnd_Cbk.h
 *        \brief  Declaration of Cbk routines
 *
 *      \details  This file declares callback functions of sub-component Wdg_30_TLE4278G_TrgCnd.
 *
 *********************************************************************************************************************/

#if !(defined WDG_30_TLE4278G_TRGCND_CBK_H)
#define WDG_30_TLE4278G_TRGCND_CBK_H

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define WDG_30_TLE4278G_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * Wdg_30_TLE4278G_TrgCnd_InvalidateCondition()
 *********************************************************************************************************************/
/*! \brief       Invalidates the trigger condition.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \details     The function is used to invalidate the trigger condition. Possible reasons are expiration of timeout
 *               or a call Wdg_30_TLE4278G_SetTriggerCondition with timeout parameter 0.
 *  \trace       CREQ-105561, DSGN-Wdg22900
 *********************************************************************************************************************/
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_TrgCnd_InvalidateCondition(void);

#define WDG_30_TLE4278G_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* WDG_30_TLE4278G_TRGCND_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: Wdg_30_TLE4278G_TrgCnd_Cbk.h
 *********************************************************************************************************************/
