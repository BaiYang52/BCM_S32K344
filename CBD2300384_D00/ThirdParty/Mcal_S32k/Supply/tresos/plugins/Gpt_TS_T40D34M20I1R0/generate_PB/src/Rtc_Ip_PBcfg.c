[!CODE!][!//
[!AUTOSPACING!]
/*==================================================================================================
* Project : RTD AUTOSAR 4.4
* Platform : CORTEXM
* Peripheral : Stm_Pit_Rtc_Emios
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
 *   @file          Rtc_Ip_PBcfg.c
 *
 *   @addtogroup    rtc_ip Rtc IPL
 *
 *   @{
 */

 [!NOCODE!]
/*==================================================================================================
 *                              GENERATION MACRO DEFINTION
 *================================================================================================*/
[!// MACRO for creating the IP configuration structure(s)
[!MACRO "GENERATE_RTC_IP_CONFIGURATION","VARIANT"="'PB'"!][!//
[!NOCODE!]

[!SELECT "./GptChannelConfigSet"!]
[!IF "count(GptRtc/*) > '0'"!]
    [!LOOP "GptRtc/*"!]
        [!CODE!]const Rtc_Ip_ConfigType [!"substring-before(node:value(./GptRtcModule),'_CH')"!]_InitConfig_[!"$VARIANT"!][[!"num:i(count(../../GptRtc/*))"!]U] = [!CR!]{[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]/**< @brief RTC Debug Mode */[!CR!][!ENDCODE!]
        [!IF "RtcFreezeEnable"!][!//
            [!CODE!][!WS"4"!](boolean)(TRUE), /**< @brief Enable Freeze Bit */[!CR!][!ENDCODE!]
        [!ELSE!][!//
            [!CODE!][!WS"4"!](boolean)(FALSE), /**< @brief Disable Freeze Bit */[!CR!][!ENDCODE!]
        [!ENDIF!][!//
         [!CODE!][!WS "4"!]/**< @brief DivBy512 Prescaler enable */[!CR!][!ENDCODE!]
        [!IF "DivBy512"!][!//
            [!CODE!][!WS"4"!](boolean)(TRUE), /**< @brief Enable */[!CR!][!ENDCODE!]
        [!ELSE!][!//
            [!CODE!][!WS"4"!](boolean)(FALSE), /**< @brief Disable */[!CR!][!ENDCODE!]
        [!ENDIF!][!//
         [!CODE!][!WS "4"!]/**< @brief DivBy32 Prescaler enable */[!CR!][!ENDCODE!]
        [!IF "DivBy32"!][!//
            [!CODE!][!WS"4"!](boolean)(TRUE), /**< @brief Enable */[!CR!][!ENDCODE!]
        [!ELSE!][!//
            [!CODE!][!WS"4"!](boolean)(FALSE), /**< @brief Disable */[!CR!][!ENDCODE!]
        [!ENDIF!][!//
        [!CODE!][!WS "4"!]/**< @brief Rtc clock source  */[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!][!"node:value(./GptRtcChannelClkSrc)"!],[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]/**< @brief RTC callback name */[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]&Gpt_ProcessCommonInterrupt,[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]/** @brief RTC callbackparam */[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!](uint8)[!"../../GptChannelConfiguration/*[as:path(node:ref(GptModuleRef)) = as:path(node:current())]/GptChannelId"!]U,[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]/** @brief RTC channel mode */[!CR!][!ENDCODE!]
        [!VAR "channelMode" = "../../GptChannelConfiguration/*[as:path(node:ref(GptModuleRef)) = as:path(node:current())]/GptChannelMode"!]
        [!CODE!][!WS "4"!]RTC_IP_[!"substring-after($channelMode, '_')"!][!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]}[!CR!][!ENDCODE!]
        [!CODE!]};[!CR!][!ENDCODE!]
    [!ENDLOOP!]
[!ENDIF!]
[!ENDSELECT!]
[!ENDNOCODE!]
[!ENDMACRO!][!// End of Macro GENERATE_RTC_IP_CONFIGURATION
[!ENDNOCODE!][!//

#ifdef __cplusplus
extern "C"{
#endif

 /*==================================================================================================
 *                                         INCLUDE FILES
 * 1) system and project includes
 * 2) needed interfaces from external units
 * 3) internal and external interfaces from this unit
 *================================================================================================*/
#include "Rtc_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h"
#include "Gpt_Irq.h"
/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 *================================================================================================*/
#define RTC_IP_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C                    43
#define RTC_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C     4
#define RTC_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C     4
#define RTC_IP_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C  0
#define RTC_IP_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C             2
#define RTC_IP_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C             0
#define RTC_IP_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C             1
/*==================================================================================================
 *                                      FILE VERSION CHECKS
 *================================================================================================*/
#if (RTC_IP_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != RTC_IP_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H)
    #error "Rtc_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Rtc_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h have different vendor ids"
#endif
/* Check if this header file and header file are of the same Autosar version */
#if ((RTC_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != RTC_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) || \
     (RTC_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != RTC_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) || \
     (RTC_IP_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != RTC_IP_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) \
    )
    #error "AutoSar Version Numbers of Rtc_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Rtc_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h are different"
#endif
/* Check if this header file and header file are of the same Software version */
#if ((RTC_IP_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != RTC_IP_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) || \
     (RTC_IP_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != RTC_IP_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) || \
     (RTC_IP_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != RTC_IP_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) \
    )
    #error "Software Version Numbers of Rtc_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Rtc_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h are different"
#endif

#if (RTC_IP_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_IRQ_VENDOR_ID)
    #error "Rtc_Ip_PBcfg.c and Gpt_Irq.h have different vendor ids"
#endif
/* Check if this header file and header file are of the same Autosar version */
#if ((RTC_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_IRQ_AR_RELEASE_MAJOR_VERSION) || \
     (RTC_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_IRQ_AR_RELEASE_MINOR_VERSION) || \
     (RTC_IP_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_IRQ_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Rtc_Ip_PBcfg.c and Gpt_Irq.h are different"
#endif
/* Check if this header file and header file are of the same Software version */
#if ((RTC_IP_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_IRQ_SW_MAJOR_VERSION) || \
     (RTC_IP_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_IRQ_SW_MINOR_VERSION) || \
     (RTC_IP_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_IRQ_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Rtc_Ip_PBcfg.c and Gpt_Irq.h are different"
#endif

/*================================================================================================*/

/*==================================================================================================
 *                                       GLOBAL VARIABLES
 *================================================================================================*/

/*==================================================================================================
 *                                       GLOBAL CONSTANTS
 *================================================================================================*/
#define GPT_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Gpt_MemMap.h"
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "PBCfg_Variant" = "concat('PB_', $postBuildVariant)"!]
[!ELSE!]
    [!VAR "PBCfg_Variant" = "'PB'"!]
[!ENDIF!]

[!CALL "GENERATE_RTC_IP_CONFIGURATION","VARIANT"="$PBCfg_Variant"!]
[!ENDNOCODE!]
#define GPT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Gpt_MemMap.h"
/*==================================================================================================
 *                                       GLOBAL FUNCTIONS
 *================================================================================================*/

/*==================================================================================================
 *                                       LOCAL VARIABLES
 *================================================================================================*/

/*==================================================================================================
 *                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 *================================================================================================*/

/*==================================================================================================
 *                                       LOCAL MACROS
 *================================================================================================*/

/*==================================================================================================
 *                                      LOCAL CONSTANTS
 *================================================================================================*/

/*==================================================================================================
 *                                       LOCAL FUNCTIONS
 *================================================================================================*/

/*==================================================================================================
 *                                   LOCAL FUNCTION PROTOTYPES
 *================================================================================================*/

#ifdef __cplusplus
}
#endif/* RTC_IP_PBCFG_C*/

[!ENDCODE!]
/** @} */
