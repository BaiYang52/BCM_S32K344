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
 *   @file          Pit_Ip_PBcfg.c
 *
 *   @addtogroup    pit_ip Pit IPL
 *
 *   @{
 */

 [!NOCODE!]
/*==================================================================================================
 *                              GENERATION MACRO DEFINTION
 *================================================================================================*/
[!// MACRO for creating the IP configuration structure(s)
[!MACRO "GENERATE_PIT_IP_CONFIGURATION","VARIANT"="'PB'"!][!//
[!NOCODE!]

[!SELECT "./GptChannelConfigSet"!]
[!IF "count(GptPit/*) > '0'"!]
    [!LOOP "GptPit/*"!]
        [!CODE!]const Pit_Ip_InstanceConfigType [!"node:value(./GptPitModule)"!]_InitConfig_[!"$VARIANT"!] = [!CR!]{[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]/** @brief PIT Standard Timer */[!CR!][!ENDCODE!]
        [!VAR "MacPitUsed"="0"!][!//
        [!VAR "MacPitRtiUsed"="0"!][!//
        [!LOOP "../../../GptHwConfiguration/*"!][!//
        [!IF "node:value(GptChannelIsUsed) = 'true' and contains(GptIsrHwId, 'PIT') and not(contains(GptIsrHwId, 'RTI'))"!][!//
            [!VAR "MacPitUsed"="1"!][!//
        [!ELSEIF "node:value(GptChannelIsUsed) = 'true' and contains(GptIsrHwId, 'RTI')"!][!//
            [!VAR "MacPitRtiUsed"="1"!][!//
        [!ENDIF!][!//
        [!ENDLOOP!]
        [!CODE!][!WS"4"!]#if((defined PIT_IP_MDIS_BIT_EXISTS) && (PIT_IP_MDIS_BIT_EXISTS == STD_ON))[!CR!][!ENDCODE!]
        [!CODE!][!WS"4"!]([!IF "$MacPitUsed=1"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!]), /* Enable standard timer */[!CR!][!ENDCODE!]
        [!CODE!][!WS"4"!]#endif[!CR!][!ENDCODE!]
        [!CODE!][!WS"4"!]#if(defined (PIT_IP_RTI_USED) && (PIT_IP_RTI_USED == STD_ON))[!CR!][!ENDCODE!]
        [!CODE!][!WS"4"!]([!IF "$MacPitRtiUsed=1"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!]), /* Enable real time interrupt timer */[!CR!][!ENDCODE!]
        [!CODE!][!WS"4"!]#endif[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]/** @brief PIT Debug Mode */[!CR!][!ENDCODE!]
        [!IF "PitFreezeEnable"!][!//
            [!CODE!][!WS"4"!](boolean)(TRUE) /* Enable Freeze Bit */[!CR!][!ENDCODE!]
        [!ELSE!][!//
            [!CODE!][!WS"4"!](boolean)(FALSE) /* Disable Freeze Bit */[!CR!][!ENDCODE!]
        [!ENDIF!][!//
[!CODE!]
};[!CR!][!CR!]
[!ENDCODE!]
    [!ENDLOOP!]
[!ENDIF!]
[!ENDSELECT!]

[!SELECT "./GptChannelConfigSet"!]
[!IF "count(GptPit/*) > '0'"!]
    [!LOOP "GptPit/*"!]
        [!CODE!]const Pit_Ip_ChannelConfigType [!"node:value(./GptPitModule)"!]_ChannelConfig_[!"$VARIANT"!][[!"num:i(count(GptPitChannels/*))"!]U] = [!ENDCODE!]
        [!CODE!][!CR!]{[!CR!][!ENDCODE!]
        [!VAR "comma" = "num:i(count(GptPitChannels/*))"!]
        [!VAR "ModuleName" = "node:value(./GptPitModule)"!]
            [!LOOP "GptPitChannels/*"!]
            [!CODE!][!WS "4"!]/** @brief [!"node:name(.)"!] */[!CR!][!ENDCODE!]
            [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
            [!CODE!][!WS "8"!]/** @brief PIT Channel Id */[!CR!][!ENDCODE!]
            [!VAR "channelName" = "node:value(./GptPitChannel)"!]
            [!IF "($channelName = 'CH_RTI')"!]
                [!CODE!][!WS "8"!][!"substring-after($channelName, '_')"!],[!CR!][!ENDCODE!]
            [!ELSE!]
                [!CODE!][!WS "8"!][!"substring-after($channelName, '_')"!]U,[!CR!][!ENDCODE!]
            [!ENDIF!]
            [!CODE!][!WS "8"!]/** @brief PIT Enable Interrupt */[!CR!][!ENDCODE!]
            [!LOOP "../../../../../GptHwConfiguration/*"!][!//
                [!IF "GptIsrHwId = concat($ModuleName,'_',$channelName)"!]
                        [!IF "(GptIsrEnable = 'true')"!]
                            [!CODE!][!WS"8"!](boolean)(TRUE), /* Interrupt enabled */[!CR!][!ENDCODE!]
                        [!ELSE!][!//
                            [!CODE!][!WS"8"!](boolean)(FALSE), /* Interrupt disabled */[!CR!][!ENDCODE!]
                        [!ENDIF!][!//
                [!ENDIF!][!//
            [!ENDLOOP!]
            [!CODE!][!WS "8"!]/** @brief PIT callback name */[!CR!][!ENDCODE!]
            [!CODE!][!WS "8"!]&Gpt_ProcessCommonInterrupt,[!CR!][!ENDCODE!]
            [!CODE!][!WS "8"!]/** @brief PIT callbackparam */[!CR!][!ENDCODE!]
            [!CODE!][!WS "8"!](uint8)[!"../../../../GptChannelConfiguration/*[as:path(node:ref(GptModuleRef)) = as:path(node:current())]/GptChannelId"!]U,[!CR!][!ENDCODE!]
            [!CODE!][!WS "8"!]/** @brief PIT channel mode */[!CR!][!ENDCODE!]
            [!VAR "channelMode" = "../../../../GptChannelConfiguration/*[as:path(node:ref(GptModuleRef)) = as:path(node:current())]/GptChannelMode"!]
            [!CODE!][!WS "8"!]PIT_IP_[!"substring-after($channelMode, '_')"!][!CR!][!ENDCODE!]
            [!CODE!][!WS "4"!]}[!CR!][!ENDCODE!]
            [!IF "(num:i($comma))>1"!][!CODE!][!WS "4"!],[!CR!][!ENDCODE!][!VAR "comma"="$comma - 1"!][!ENDIF!]
            [!ENDLOOP!]
        [!CODE!]};[!CR!][!CR!][!ENDCODE!]
    [!ENDLOOP!]
[!ENDIF!]
[!ENDSELECT!]

[!ENDNOCODE!]
[!ENDMACRO!][!// End of Macro GENERATE_PIT_IP_CONFIGURATION

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
#include "Pit_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h"
#include "Gpt_Irq.h"
/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 *================================================================================================*/
#define PIT_IP_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C                    43
#define PIT_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C     4
#define PIT_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C     4
#define PIT_IP_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C  0
#define PIT_IP_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C             2
#define PIT_IP_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C             0
#define PIT_IP_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C             1
/*==================================================================================================
 *                                      FILE VERSION CHECKS
 *================================================================================================*/
#if (PIT_IP_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != PIT_IP_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H)
    #error "Pit_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Pit_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h have different vendor ids"
#endif
/* Check if this header file and header file are of the same Autosar version */
#if ((PIT_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != PIT_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) || \
     (PIT_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != PIT_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) || \
     (PIT_IP_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != PIT_IP_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) \
    )
    #error "AutoSar Version Numbers of Pit_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Pit_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h are different"
#endif
/* Check if this header file and header file are of the same Software version */
#if ((PIT_IP_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != PIT_IP_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) || \
     (PIT_IP_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != PIT_IP_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) || \
     (PIT_IP_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != PIT_IP_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) \
    )
    #error "Software Version Numbers of Pit_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Pit_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h are different"
#endif

#if (PIT_IP_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_IRQ_VENDOR_ID)
    #error "Pit_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Gpt_Irq.h have different vendor ids"
#endif
/* Check if this header file and header file are of the same Autosar version */
#if ((PIT_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_IRQ_AR_RELEASE_MAJOR_VERSION) || \
     (PIT_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_IRQ_AR_RELEASE_MINOR_VERSION) || \
     (PIT_IP_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_IRQ_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Pit_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Gpt_Irq.h are different"
#endif
/* Check if this header file and header file are of the same Software version */
#if ((PIT_IP_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_IRQ_SW_MAJOR_VERSION) || \
     (PIT_IP_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_IRQ_SW_MINOR_VERSION) || \
     (PIT_IP_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_IRQ_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Pit_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Gpt_Irq.h are different"
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

[!CALL "GENERATE_PIT_IP_CONFIGURATION","VARIANT"="$PBCfg_Variant"!]
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
#endif /* PIT_IP_PBCFG_C*/

[!ENDCODE!]
/** @} */
