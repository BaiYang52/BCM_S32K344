[!CODE!][!//
[!AUTOSPACING!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Emios Siul2 Wkpu LpCmp
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

#ifndef CMP_IP_CFG_H
#define CMP_IP_CFG_H

/**
 *   @file    Cmp_Ip_Cfg.h
 *   @version 2.0.1
 *
 *   @brief   AUTOSAR Icu - contains the data exported by the Icu module
 *   @details Contains the information that will be exported by the module, as requested by Autosar.
 *
 *   @addtogroup cmp_icu_ip CMP IPL
 *   @{
 */
 
#ifdef __cplusplus
extern "C"{
#endif

 /*==================================================================================================
 *                                         INCLUDE FILES
 * 1) system and project includes
 * 2) needed interfaces from external units
 * 3) internal and external interfaces from this unit
 *================================================================================================*/
#include "StandardTypes.h"
[!NOCODE!][!//
[!IF "var:defined('postBuildVariant')"!][!//
    [!LOOP "variant:all()"!][!//
        [!CODE!][!WS "0"!]#include "Cmp_Ip_[!"."!]_PBcfg.h"[!CR!][!ENDCODE!][!//
    [!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS "0"!]#include "Cmp_Ip_PBcfg.h"[!CR!][!ENDCODE!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//

/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 *================================================================================================*/
#define CMP_IP_CFG_VENDOR_ID                    43
#define CMP_IP_CFG_AR_RELEASE_MAJOR_VERSION     4
#define CMP_IP_CFG_AR_RELEASE_MINOR_VERSION     4
#define CMP_IP_CFG_AR_RELEASE_REVISION_VERSION  0
#define CMP_IP_CFG_SW_MAJOR_VERSION             2
#define CMP_IP_CFG_SW_MINOR_VERSION             0
#define CMP_IP_CFG_SW_PATCH_VERSION             1

/*==================================================================================================
 *                                      FILE VERSION CHECKS
 *================================================================================================*/

/* Check if header file and StandardTypes.h file are of the same Autosar version */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((CMP_IP_CFG_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
         (CMP_IP_CFG_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Cmp_Ip_Cfg.h and StandardTypes.h are different"
    #endif
#endif

/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/

[!IF "count(./IcuConfigSet/IcuLpCmp/*) > '0'"!]
[!CODE!]#define CMP_CONFIG_EXT \[!CR!][!ENDCODE!][!//
[!IF "var:defined('postBuildVariant')"!][!//
    [!LOOP "variant:all()"!][!//
        [!CODE!][!WS4!]CMP_CONFIG_[!"."!]_PB \[!CR!][!ENDCODE!]
    [!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS4!]CMP_CONFIG_PB[!CR!][!ENDCODE!]
[!ENDIF!][!//
[!ENDIF!]

#ifdef __cplusplus
}
#endif

/** @} */

#endif  /* CMP_IP_CFG_H */
[!ENDCODE!]
