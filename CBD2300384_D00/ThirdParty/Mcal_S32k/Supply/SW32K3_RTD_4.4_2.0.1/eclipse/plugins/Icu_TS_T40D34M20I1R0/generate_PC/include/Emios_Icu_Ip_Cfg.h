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

#ifndef EMIOS_ICU_IP_CFG_H
#define EMIOS_ICU_IP_CFG_H

/**
 *   @file    Emios_Icu_Ip_Cfg.h
 *   @version 2.0.1
 *
 *   @brief   AUTOSAR Icu - contains the data exported by the ICU module.
 *   @details Contains the information that will be exported by the module, as requested by AUTOSAR.
 *
 *   @addtogroup emios_icu_ip EMIOS IPL
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
/* Include all variants header files. */
[!NOCODE!][!//
[!IF "var:defined('postBuildVariant')"!][!//
    [!LOOP "variant:all()"!][!//
        [!CODE!][!WS "0"!]#include "Emios_Icu_Ip_[!"."!]_PBcfg.h"[!CR!][!ENDCODE!][!//
    [!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS "0"!]#include "Emios_Icu_Ip_PBcfg.h"[!CR!][!ENDCODE!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define EMIOS_ICU_IP_CFG_VENDOR_ID                    43
#define EMIOS_ICU_IP_CFG_AR_RELEASE_MAJOR_VERSION     4
#define EMIOS_ICU_IP_CFG_AR_RELEASE_MINOR_VERSION     4
#define EMIOS_ICU_IP_CFG_AR_RELEASE_REVISION_VERSION  0
#define EMIOS_ICU_IP_CFG_SW_MAJOR_VERSION             2
#define EMIOS_ICU_IP_CFG_SW_MINOR_VERSION             0
#define EMIOS_ICU_IP_CFG_SW_PATCH_VERSION             1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
[!IF "var:defined('postBuildVariant')"!][!//
    [!LOOP "variant:all()"!][!//
/* Check if source file and ICU configuration header file are of the same vendor */
#if (EMIOS_ICU_IP_CFG_VENDOR_ID != EMIOS_ICU_IP_[!"."!]_PBCFG_VENDOR_ID)
    #error "Emios_Icu_Ip_Cfg.h and Emios_Icu_Ip_[!"."!]_PBcfg.h have different vendor IDs"
#endif
    /* Check if header file and Icu configuration header file are of the same Autosar version */
#if ((EMIOS_ICU_IP_CFG_AR_RELEASE_MAJOR_VERSION != EMIOS_ICU_IP_[!"."!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_ICU_IP_CFG_AR_RELEASE_MINOR_VERSION != EMIOS_ICU_IP_[!"."!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_ICU_IP_CFG_AR_RELEASE_REVISION_VERSION != EMIOS_ICU_IP_[!"."!]_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
#error "AutoSar Version Numbers of Emios_Icu_Ip_Cfg.h and Emios_Icu_Ip_[!"."!]_PBcfg.h are different"
#endif
/* Check if header file and Icu configuration header file are of the same software version */
#if ((EMIOS_ICU_IP_CFG_SW_MAJOR_VERSION != EMIOS_ICU_IP_[!"."!]_PBCFG_SW_MAJOR_VERSION) || \
     (EMIOS_ICU_IP_CFG_SW_MINOR_VERSION != EMIOS_ICU_IP_[!"."!]_PBCFG_SW_MINOR_VERSION) || \
     (EMIOS_ICU_IP_CFG_SW_PATCH_VERSION != EMIOS_ICU_IP_[!"."!]_PBCFG_SW_PATCH_VERSION) \
    )
#error "Software Version Numbers of Emios_Icu_Ip_Cfg.h and Emios_Icu_Ip_[!"."!]_PBcfg.h are different"
#endif
    [!ENDLOOP!][!//
[!ELSE!][!//
/* Check if source file and ICU configuration header file are of the same vendor */
#if (EMIOS_ICU_IP_CFG_VENDOR_ID != EMIOS_ICU_IP_PBCFG_VENDOR_ID)
    #error "Emios_Icu_Ip_Cfg.h and Emios_Icu_Ip_PBcfg.h have different vendor IDs"
#endif
    /* Check if header file and Icu configuration header file are of the same Autosar version */
#if ((EMIOS_ICU_IP_CFG_AR_RELEASE_MAJOR_VERSION != EMIOS_ICU_IP_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_ICU_IP_CFG_AR_RELEASE_MINOR_VERSION != EMIOS_ICU_IP_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_ICU_IP_CFG_AR_RELEASE_REVISION_VERSION != EMIOS_ICU_IP_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
#error "AutoSar Version Numbers of Emios_Icu_Ip_Cfg.h and Emios_Icu_Ip_PBcfg.h are different"
#endif
/* Check if header file and Icu configuration header file are of the same software version */
#if ((EMIOS_ICU_IP_CFG_SW_MAJOR_VERSION != EMIOS_ICU_IP_PBCFG_SW_MAJOR_VERSION) || \
     (EMIOS_ICU_IP_CFG_SW_MINOR_VERSION != EMIOS_ICU_IP_PBCFG_SW_MINOR_VERSION) || \
     (EMIOS_ICU_IP_CFG_SW_PATCH_VERSION != EMIOS_ICU_IP_PBCFG_SW_PATCH_VERSION) \
    )
#error "Software Version Numbers of Emios_Icu_Ip_Cfg.h and Emios_Icu_Ip_PBcfg.h are different"
#endif
[!ENDIF!][!//

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
[!NOCODE!][!//

[!LOOP "IcuConfigSet/IcueMios/*"!][!// Assuming that in the icu.xdm file have check whether EMIOS instance used
    [!LOOP "IcueMiosChannels/*"!][!// Assuming that in the icu.xdm file have check whether EMIOS channel used
[!CODE!][!//
/** @brief Macros to check EMIOS channels used by ICU. */
#ifndef EMIOS_[!"node:value(../../IcueMiosModule)"!]_CH_[!"node:value(./IcueMiosChannel)"!]_USED
    #define EMIOS_[!"node:value(../../IcueMiosModule)"!]_CH_[!"node:value(./IcueMiosChannel)"!]_USED
#else
    #error "EMIOS_[!"node:value(../../IcueMiosModule)"!]_CH_[!"node:value(./IcueMiosChannel)"!] channel cannot be used by ICU. Instance locked by another driver!"
#endif
[!ENDCODE!][!//
    [!ENDLOOP!][!//
[!ENDLOOP!][!//

[!ENDNOCODE!][!//

[!IF "count(./IcuConfigSet/IcueMios/*) > '0'"!]
[!CODE!]#define EMIOS_ICU_CONFIG_EXT \[!CR!][!ENDCODE!][!//
[!IF "var:defined('postBuildVariant')"!][!//
    [!LOOP "variant:all()"!][!//
        [!CODE!][!WS "8"!]EMIOS_ICU_CONFIG_[!"."!]_PB \[!CR!][!ENDCODE!][!//
    [!ENDLOOP!][!//
    [!CODE!][!CR!][!CR!][!ENDCODE!][!//
[!ELSE!][!//
    [!CODE!][!WS "8"!]EMIOS_ICU_CONFIG_PB[!CR!][!ENDCODE!][!//
[!ENDIF!][!//
[!ENDIF!][!//



#ifdef __cplusplus
}
#endif

/** @} */

#endif /* EMIOS_ICU_IP_CFG_H */

[!ENDCODE!]
