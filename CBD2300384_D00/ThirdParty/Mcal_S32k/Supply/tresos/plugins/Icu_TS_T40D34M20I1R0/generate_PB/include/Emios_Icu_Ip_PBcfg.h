[!AUTOSPACING!][!//
[!CODE!][!//
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

#ifndef EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H
#define EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H

/**
 *   @file    Emios_Icu_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c
 *   @version 2.0.1
 *
 *   @brief   AUTOSAR Icu - contains the data exported by the ICU module.
 *   @details Contains the information that will be exported by the module, as requested by Autosar.
 *
 *   @addtogroup emios_icu_ip EMIOS IPL
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

/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 *================================================================================================*/
#define EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID                    43
#define EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION     4
#define EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION     4
#define EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION  0
#define EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION             2
#define EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION             0
#define EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION             1

/*==================================================================================================
 *                                      FILE VERSION CHECKS
 *================================================================================================*/

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
[!NOCODE!]
[!VAR "OuterLoopCounter" = "0"!]
[!VAR "InnerLoopCounter" = "0"!]
[!LOOP "IcuConfigSet/IcueMios/*"!] [!// Assuming that in the icu.xdm file have check whether EMIOS instance used
[!LOOP "IcueMiosChannels/*"!] [!// Assuming that in the icu.xdm file have check whether EMIOS channel used

[!CODE!]
#ifndef ICU_EMIOS_[!"node:value(../../IcueMiosModule)"!]_CH_[!"node:value(./IcueMiosChannel)"!]_ISR_USED
    /** @brief Macros for indicate EMIOS interrupts used by ICU. */
    #define ICU_EMIOS_[!"node:value(../../IcueMiosModule)"!]_CH_[!"node:value(./IcueMiosChannel)"!]_ISR_USED    (STD_ON)
#endif

#ifndef EMIOS_[!"node:value(../../IcueMiosModule)"!]_CH_[!"node:value(./IcueMiosChannel)"!]_ISR_USED
    /** @brief Macros for indicate EMIOS interrupts used. */
    #define EMIOS_[!"node:value(../../IcueMiosModule)"!]_CH_[!"node:value(./IcueMiosChannel)"!]_ISR_USED        (STD_ON)
#endif
[!ENDCODE!]
[!IF "((node:value(./IcuEmiosBusSelect) != 'EMIOS_ICU_BUS_INTERNAL_COUNTER') and (node:value(../../../../../IcuAutosarExt/IcuOverflowNotificationApi) = 'true') and (node:value(../../../../../IcuConfigSet/IcuChannel/*[text:concat(text:split(as:path(node:ref(./IcuChannelRef)), '/')[4],text:split(as:path(node:ref(./IcuChannelRef)), '/')[5]) =  text:concat(text:split(as:path(node:current()), '/')[4],text:split(as:path(node:current()), '/')[5])]/IcuOverflowNotification) != 'NULL_PTR'))"!]
[!IF "(node:value(./IcuEmiosBusSelect) = 'EMIOS_ICU_BUS_A')"!]
    [!VAR "icueMiosMaster" = "23"!]
[!ELSEIF "(node:value(./IcuEmiosBusSelect) = 'EMIOS_ICU_BUS_F')"!]
    [!VAR "icueMiosMaster" = "22"!]
[!ELSEIF "(node:value(./IcuEmiosBusSelect) = 'EMIOS_ICU_BUS_DIVERSE')"!]
    [!IF "(num:i(node:value(./IcueMiosChannel)) < 8)"!][!//
        [!VAR "icueMiosMaster" = "0"!]
    [!ELSEIF "(num:i(node:value(./IcueMiosChannel)) < 16)"!]
        [!VAR "icueMiosMaster" = "8"!]
    [!ELSEIF "(num:i(node:value(./IcueMiosChannel)) < 24)"!]
        [!VAR "icueMiosMaster" = "16"!]
    [!ELSE!]
        [!VAR "icueMiosMaster" = "24"!]
    [!ENDIF!]
[!ENDIF!]
[!CODE!]
#ifndef ICU_EMIOS_[!"node:value(../../IcueMiosModule)"!]_CH_[!"num:i($icueMiosMaster)"!]_ISR_USED
    /** @brief Macros for check EMIOS channels used by MCL. */
    #define ICU_EMIOS_[!"node:value(../../IcueMiosModule)"!]_CH_[!"num:i($icueMiosMaster)"!]_ISR_USED          (STD_ON)
#endif
#ifndef EMIOS_[!"node:value(../../IcueMiosModule)"!]_CH_[!"num:i($icueMiosMaster)"!]_ISR_USED
    /** @brief Macros for indicate EMIOS interrupts used. */
    #define EMIOS_[!"node:value(../../IcueMiosModule)"!]_CH_[!"num:i($icueMiosMaster)"!]_ISR_USED          (STD_ON)
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDLOOP!]
[!ENDLOOP!]
[!ENDNOCODE!]

[!NOCODE!]

[!IF "var:defined('postBuildVariant')"!]
    [!VAR "PBCfg_Variant" = "concat('PB_', $postBuildVariant)"!]
    [!IF "count(./IcuConfigSet/IcueMios/*) > '0'"!]
        [!CODE!][!CR!]#define EMIOS_ICU_CONFIG_[!"$postBuildVariant"!]_PB \[!CR!][!ENDCODE!]
    [!ENDIF!]
[!ELSE!]
    [!VAR "PBCfg_Variant" = "'PB'"!]
    [!IF "count(./IcuConfigSet/IcueMios/*) > '0'"!]
        [!CODE!][!CR!]#define EMIOS_ICU_CONFIG_PB \[!CR!][!ENDCODE!]
    [!ENDIF!]
[!ENDIF!]

[!SELECT "./IcuConfigSet"!]
[!IF "count(IcueMios/*) > '0'"!]
    [!LOOP "IcueMios/*"!][!//
      [!CODE!][!WS "8"!]extern const eMios_Icu_Ip_ChannelConfigType  eMios_Icu_Ip_[!"node:value(./IcueMiosModule)"!]_ChannelConfig_[!"$PBCfg_Variant"!][[!"num:i(count(IcueMiosChannels/*))"!]U]; \[!CR!][!ENDCODE!][!//
      [!//[!CODE!][!WS "0"!]extern const eMios_Icu_Ip_InstanceConfigType eMios_Icu_Ip_[!"node:value(./IcueMiosModule)"!]_InstanceConfig_[!"$PBCfg_Variant"!]; \[!CR!][!ENDCODE!][!//
      [!CODE!][!WS "8"!]extern const eMios_Icu_Ip_ConfigType         eMios_Icu_Ip_[!"node:value(./IcueMiosModule)"!]_Config_[!"$PBCfg_Variant"!]; \[!CR!][!ENDCODE!][!//
    [!ENDLOOP!][!//
[!ENDIF!][!//
[!ENDSELECT!][!//

[!ENDNOCODE!][!//



/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H */

[!ENDCODE!][!//
