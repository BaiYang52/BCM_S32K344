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

#ifndef CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H
#define CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H

/**
 *   @file    Cmp_Ip_PBcfg.h
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
*================================================================================================*/
#include "Cmp_Ip_Types.h"

/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 *================================================================================================*/
#define CMP_IP_PBCFG_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]VENDOR_ID                    43
#define CMP_IP_PBCFG_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]AR_RELEASE_MAJOR_VERSION     4
#define CMP_IP_PBCFG_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]AR_RELEASE_MINOR_VERSION     4
#define CMP_IP_PBCFG_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]AR_RELEASE_REVISION_VERSION  0
#define CMP_IP_PBCFG_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]SW_MAJOR_VERSION             2
#define CMP_IP_PBCFG_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]SW_MINOR_VERSION             0
#define CMP_IP_PBCFG_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]SW_PATCH_VERSION             1

/*==================================================================================================
 *                                      FILE VERSION CHECKS
 *================================================================================================*/
/* Check if source file and ICU header file are of the same vendor */
#if (CMP_IP_PBCFG_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]VENDOR_ID != CMP_IP_TYPES_VENDOR_ID)
    #error "Cmp_Ip_PBcfg.h and Cmp_Ip_Types.h have different vendor IDs"
#endif
/* Check if source file and ICU header file are of the same AutoSar version */
#if ((CMP_IP_PBCFG_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]AR_RELEASE_MAJOR_VERSION != CMP_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (CMP_IP_PBCFG_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]AR_RELEASE_MINOR_VERSION != CMP_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (CMP_IP_PBCFG_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]AR_RELEASE_REVISION_VERSION != CMP_IP_TYPES_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Cmp_Ip_PBcfg.h and Cmp_Ip_Types.h are different"
#endif
/* Check if source file and ICU header file are of the same Software version */
#if ((CMP_IP_PBCFG_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]SW_MAJOR_VERSION != CMP_IP_TYPES_SW_MAJOR_VERSION) || \
     (CMP_IP_PBCFG_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]SW_MINOR_VERSION != CMP_IP_TYPES_SW_MINOR_VERSION) || \
     (CMP_IP_PBCFG_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]SW_PATCH_VERSION != CMP_IP_TYPES_SW_PATCH_VERSION))
#error "Software Version Numbers of Cmp_Ip_PBcfg.h and Cmp_Ip_Types.h are different"
#endif
/*==================================================================================================
 *                                       DEFINES AND MACROS
 *================================================================================================*/

[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "PBCfg_Variant" = "concat('PB_', $postBuildVariant)"!]
    [!IF "count(./IcuConfigSet/IcuLpCmp/*) > '0'"!]
        [!CODE!]#define CMP_CONFIG_[!"$postBuildVariant"!]_PB \[!CR!][!ENDCODE!]
    [!ENDIF!]
[!ELSE!]
    [!VAR "PBCfg_Variant" = "'PB'"!]
    [!IF "count(./IcuConfigSet/IcuLpCmp/*) > '0'"!]
        [!CODE!]#define CMP_CONFIG_PB \[!CR!][!ENDCODE!]
    [!ENDIF!]
[!ENDIF!]

[!SELECT "./IcuConfigSet"!]
[!IF "count(IcuLpCmp/*) > '0'"!]
    [!LOOP "IcuLpCmp/*"!]
            [!CODE!]extern const Cmp_Ip_ConfigType Cmp_Ip_[!"node:value(IcuCmpInstanceNumber)"!]_Config_[!"$PBCfg_Variant"!]; \[!CR!][!ENDCODE!]
    [!ENDLOOP!]
[!ENDIF!]
[!ENDSELECT!]
[!ENDNOCODE!]


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H */
[!ENDCODE!]
