/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : FLEXCAN
*   Dependencies         : 
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
/*==================================================================================================
==================================================================================================*/

#ifndef CAN_EXTERNALS_H
#define CAN_EXTERNALS_H

/**
*   @file    Can_Externals.h
*   @version 2.0.1
*
*   @brief   AUTOSAR Can - module interface.
*   @details API header for CAN driver.
*
*   @addtogroup CAN_DRIVER
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define CAN_EXTERNALS_VENDOR_ID_H                       43
#define CAN_EXTERNALS_MODULE_ID_H                       80
#define CAN_EXTERNALS_AR_RELEASE_MAJOR_VERSION_H        4
#define CAN_EXTERNALS_AR_RELEASE_MINOR_VERSION_H        4
#define CAN_EXTERNALS_AR_RELEASE_REVISION_VERSION_H     0
#define CAN_EXTERNALS_SW_MAJOR_VERSION_H                2
#define CAN_EXTERNALS_SW_MINOR_VERSION_H                0
#define CAN_EXTERNALS_SW_PATCH_VERSION_H                1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
#define CAN_START_SEC_CODE
#include "Can_MemMap.h"
[!NOCODE!][!INDENT "0"!]
[!SELECT "CanGeneral"!]
    [!IF "node:exists(./CanLPduReceiveCalloutFunction)"!]
        [!IF "(normalize-space(./CanLPduReceiveCalloutFunction) != 'NULL_PTR')"!]
            [!CODE!]boolean [!"normalize-space(./CanLPduReceiveCalloutFunction)"!](uint8 Hrh, Can_IdType CanId, uint8 CanDataLegth, const uint8* CanSduPtr);[!CR!][!ENDCODE!]
        [!ENDIF!]
    [!ENDIF!]
[!ENDSELECT!]
[!ENDINDENT!][!ENDNOCODE!]
#define CAN_STOP_SEC_CODE
#include "Can_MemMap.h"
#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CAN_EXTERNALS_H */
