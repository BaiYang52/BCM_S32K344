/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : LPUART_FLEXIO
*   Dependencies         : none
*
*   Autosar Version      : 4.4.0
*   Autosar Revision     : ASR_REL_4_4_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 2.0.1
*   Build Version        : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
*   (c) Copyright 2020 - 2022 NXP Semiconductors
*   All Rights Reserved.
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

#ifndef LIN_ASREXT_H
#define LIN_ASREXT_H

/**
*   @file Lin_ASRExt.h
*
*   @addtogroup LIN
*   @{
*/

#ifdef __cplusplus
extern "C"
{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Lin_Defines.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define LIN_ASREXT_VENDOR_ID                    43
#define LIN_ASREXT_AR_RELEASE_MAJOR_VERSION     4
#define LIN_ASREXT_AR_RELEASE_MINOR_VERSION     4
#define LIN_ASREXT_AR_RELEASE_REVISION_VERSION  0
#define LIN_ASREXT_SW_MAJOR_VERSION             2
#define LIN_ASREXT_SW_MINOR_VERSION             0
#define LIN_ASREXT_SW_PATCH_VERSION             1

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
#if (LIN_ASREXT_VENDOR_ID != LIN_DEFINES_VENDOR_ID)
    #error "Lin_ASRExt.h and Lin_Defines.h have different vendor ids"
#endif
/* Check if current file and Lin_Defines header file are of the same Autosar version */
#if ((LIN_ASREXT_AR_RELEASE_MAJOR_VERSION    != LIN_DEFINES_AR_RELEASE_MAJOR_VERSION) || \
     (LIN_ASREXT_AR_RELEASE_MINOR_VERSION    != LIN_DEFINES_AR_RELEASE_MINOR_VERSION) || \
     (LIN_ASREXT_AR_RELEASE_REVISION_VERSION != LIN_DEFINES_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Lin_ASRExt.h and Lin_Defines.h are different"
#endif
/* Check if current file and Lin_Defines header file are of the same Software version */
#if ((LIN_ASREXT_SW_MAJOR_VERSION != LIN_DEFINES_SW_MAJOR_VERSION) || \
     (LIN_ASREXT_SW_MINOR_VERSION != LIN_DEFINES_SW_MINOR_VERSION) || \
     (LIN_ASREXT_SW_PATCH_VERSION != LIN_DEFINES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Lin_ASRExt.h and Lin_Defines.h are different"
#endif
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#if (LIN_DUAL_CLOCK_MODE == STD_ON)
#if (LIN_DEV_ERROR_DETECT == STD_ON)
/**
* @brief            API service ID for Lin_SetClockMode() function.
* @details          Parameters used when raising an error or exception.
*
*
*/
#define LIN_SETCLOCKMODE_ID       ((uint8)0x0CU)
#endif /* #if (LIN_DEV_ERROR_DETECT == STD_ON) */
#endif /* #if LIN_DUAL_CLOCK_MODE == STD_ON */

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/
#if (LIN_DUAL_CLOCK_MODE == STD_ON)
/**
* @brief          Clock modes.
* @pre            LIN_DUAL_CLOCK_MODE must be defined and its value must be STD_ON.
*
* @note           Possible clock modes: LIN_NORMAL (normal execution mode),
*                 LIN_ALTERNATE (low power mode).
*
*/
typedef enum
{
    LIN_NORMAL     =   (uint8)0x01U, /**< @brief
                                               LIN_NORMAL mode.*/
    LIN_ALTERNATE  =   (uint8)0x02U  /**< @brief
                                               LIN_ALTERNATE mode.*/
} Lin_ClockModesType;

#endif /* #if LIN_DUAL_CLOCK_MODE == STD_ON */

/*==================================================================================================
*                                STRUCTURES AND OTHERS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
#define LIN_START_SEC_CODE
#include "Lin_MemMap.h"

#if (LIN_DUAL_CLOCK_MODE == STD_ON)
/**
* @brief          Lin_SetClockMode.
* @details        Switch the clock mode to the alternate clock mode on
*                 all the Lin channels.
*
* @param[in]      ClockMode   New clock mode.
*
* @return                     The result of the switching clock operation.
* @retval E_OK                The switching operation was OK.
* @retval E_NOT_OK            The switching operation has failed caused by the
*                             wrong driver state.
*
* @api            Switch the clock mode to the alternate clock mode on all
*                 the Lin channels.
*
* @pre            LIN_DUAL_CLOCK_MODE must be defined and its value must be STD_ON.
*
* @note           Switch the clock mode to the alternate clock mode on
*                 all the Lin channels.
*/
Std_ReturnType Lin_SetClockMode(Lin_ClockModesType ClockMode);
#endif

#define LIN_STOP_SEC_CODE
#include "Lin_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* LIN_ASREXT_H */
