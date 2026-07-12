/*==================================================================================================
* Project : RTD AUTOSAR 4.4
* Platform : CORTEXM
* Peripheral : S32K3XX
* Dependencies : none
*
* Autosar Version : 4.4.0
* Autosar Revision : ASR_REL_4_4_REV_0000
* Autosar Conf.Variant :
* SW Version : 2.0.1
* Build Version : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
* (c) Copyright 2020 - 2022 NXP Semiconductors
* All Rights Reserved.
*
* NXP Confidential. This software is owned or controlled by NXP and may only be
* used strictly in accordance with the applicable license terms. By expressly
* accepting such terms or by downloading, installing, activating and/or otherwise
* using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms. If you do not agree to be
* bound by the applicable license terms, then you may not retain, install,
* activate or otherwise use the software.
==================================================================================================*/
/**
*   @file    Can_DrvGeneralTypes.h
*   @version 2.0.1
*
*   @brief   AUTOSAR CAN Driver - General types
*   @details This file provides types for the CAN Driver module.
*            This file contains sample code only. It is not part of the production code deliverables
*
*   @addtogroup BASE_COMPONENT
*   @{
*/

#ifndef CAN_DRV_GENERAL_TYPES_H
#define CAN_DRV_GENERAL_TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                         INCLUDE FILES
==================================================================================================*/
#include "ComStack_Types.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/**
* @brief CAN Controller State
*/
typedef enum
{
    CAN_CS_UNINIT = 0,
    CAN_CS_STARTED,
    CAN_CS_STOPPED,
    CAN_CS_SLEEP
} Can_ControllerStateType;

/**
* @brief CAN Error State
*/
typedef enum
{
    CAN_ERRORSTATE_OK = 0,
    CAN_ERRORSTATE_PASSIVE,
    CAN_ERRORSTATE_BUSOFF
} Can_ErrorStateType;

/*==================================================================================================
*                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                     FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* CAN_DRV_GENERAL_TYPES_H */

/** @} */