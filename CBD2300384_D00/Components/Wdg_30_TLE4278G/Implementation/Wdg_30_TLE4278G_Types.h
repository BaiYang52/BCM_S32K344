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
/*!        \file  Wdg_30_TLE4278G_Types.h
 *        \brief  File in which watchdog specific types are defined.
 *
 *      \details  This file contains definitions of watchdog specific types.
 *
 *********************************************************************************************************************/

#if (!defined WDG_30_TLE4278G_TYPES_H)
#define WDG_30_TLE4278G_TYPES_H

/**********************************************************************************************************************
 * DATA TYPES
 *********************************************************************************************************************/

typedef enum 
{
    WDG_30_TLE4278G_UNINIT = 0u,
    WDG_30_TLE4278G_IDLE = 1u,
    WDG_30_TLE4278G_BUSY = 2u
} Wdg_30_TLE4278G_StateType;

#endif /* WDG_30_TLE4278G_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: Wdg_30_TLE4278G_Types.h
 *********************************************************************************************************************/
