/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : 
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

#ifndef ECUM_CFG_H
#define ECUM_CFG_H

/**
*   @file
*
*   @addtogroup ECUM_MODULE EcuM Module
*   @{
*/

#ifdef __cplusplus
extern "C" {
#endif
/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/


/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ECUM_CFG_VENDOR_ID                      43
#define ECUM_CFG_MODULE_ID                      10
#define ECUM_CFG_AR_RELEASE_MAJOR_VERSION       4
#define ECUM_CFG_AR_RELEASE_MINOR_VERSION       4
#define ECUM_CFG_AR_RELEASE_REVISION_VERSION    0
#define ECUM_CFG_SW_MAJOR_VERSION               2
#define ECUM_CFG_SW_MINOR_VERSION               0
#define ECUM_CFG_SW_PATCH_VERSION               1

/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/


/*==================================================================================================
 *                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/
/* ========= user defined wakeup sources ========= */
[!LOOP "EcuMConfiguration"!]
[!LOOP "EcuMCommonConfiguration/EcuMWakeupSource/*"!]
#if (defined EcuMConf_EcuMWakeupSource_[!"node:name('.')"!]) /* to prevent double declaration */
#error EcuMConf_EcuMWakeupSource_[!"node:name('.')"!] already defined
#endif
/** @brief Identifier for wakeup source [!"node:name('.')"!]*/
#define EcuMConf_EcuMWakeupSource_[!"node:name('.')"!] ((uint32)((uint32)1U<<[!"node:value('./EcuMWakeupSourceId')"!]))
[!ENDLOOP!]
[!ENDLOOP!]

/* ======== Sleep modes ======== */
[!LOOP "EcuMConfiguration"!]
[!LOOP "EcuMCommonConfiguration/EcuMSleepMode/*"!]
#if (defined EcuMConf_EcuMSleepMode_[!"node:name('.')"!]) /* to prevent double declaration */
#error EcuMConf_EcuMSleepMode_[!"node:name('.')"!] already defined
#endif
/** @brief Identifier for wakeup source [!"node:name('.')"!]*/
#define EcuMConf_EcuMSleepMode_[!"node:name('.')"!] [!"num:inttohex(node:value('./EcuMSleepModeId'))"!]U
[!ENDLOOP!]
[!ENDLOOP!]
/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
typedef uint32 EcuM_WakeupSourceType; /**< @brief variable type for ECU Manager wakeup sources*/

/*==================================================================================================
*                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
#ifdef __cplusplus
}
#endif

/** @} */

#endif /* ECUM_CFG_H */
