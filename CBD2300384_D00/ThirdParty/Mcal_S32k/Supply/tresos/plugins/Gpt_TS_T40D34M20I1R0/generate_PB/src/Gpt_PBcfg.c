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
*   @file           Gpt_PBcfg.c
*
*   @addtogroup     gpt Gpt Driver
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
#include "Gpt_Cfg.h"
#include "Gpt.h"
#include "Gpt_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h"

[!NOCODE!][!//
[!INCLUDE "Gpt_VersionCheck_Src_PB.m"!][!//
[!ENDNOCODE!][!//
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define GPT_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C                      43
#define GPT_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C       4
#define GPT_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C       4
#define GPT_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C    0
#define GPT_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C               2
#define GPT_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C               0
#define GPT_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C               1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#if ( GPT_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_VENDOR_ID_CFG)
    #error "Gpt_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Gpt_Cfg.h have different vendor ids"
#endif
/* Check if the header files are of the same Autosar version */
#if ((GPT_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (GPT_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_AR_RELEASE_MINOR_VERSION_CFG) || \
     (GPT_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_AR_RELEASE_REVISION_VERSION_CFG) \
    )
    #error "AutoSar Version Numbers of Gpt_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Gpt_Cfg.h are different"
#endif
/* Check if the header files are of the same Software version */
#if ((GPT_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_SW_MAJOR_VERSION_CFG) || \
     (GPT_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_SW_MINOR_VERSION_CFG) || \
     (GPT_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_SW_PATCH_VERSION_CFG)    \
    )
    #error "Software Version Numbers of Gpt_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Gpt_Cfg.h are different"
#endif

/* Check if Gpt_PBcfg.c file and Gpt.h header file are of the same vendor */
#if (GPT_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_VENDOR_ID)
    #error "Gpt_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Gpt.h have different vendor ids"
#endif
/* Check if Gpt_PBcfg.c file and Gpt.h header file are of the same Autosar version */
#if ((GPT_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_AR_RELEASE_MAJOR_VERSION) || \
     (GPT_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_AR_RELEASE_MINOR_VERSION) || \
     (GPT_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Gpt_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Gpt.h are different"
#endif
/* Check if Gpt_PBcfg.c file and Gpt.h header file are of the same Software version */
#if ((GPT_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_SW_MAJOR_VERSION) || \
     (GPT_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_SW_MINOR_VERSION) || \
     (GPT_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != GPT_SW_PATCH_VERSION))
    #error "Software Version Numbers of Gpt_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Gpt.h are different"
#endif

#if (GPT_IPW_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H != GPT_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C)
    #error "Gpt_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h and Gpt_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c have different vendor ids"
#endif
/* Check if the header files are of the same Autosar version */
#if ((GPT_IPW_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H != GPT_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C) || \
     (GPT_IPW_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H != GPT_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C) || \
     (GPT_IPW_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H != GPT_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C) \
    )
    #error "AutoSar Version Numbers of Gpt_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h and Gpt_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c are different"
#endif
/* Check if the header files are of the same Software version */
#if ((GPT_IPW_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H != GPT_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C) || \
     (GPT_IPW_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H != GPT_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C) || \
     (GPT_IPW_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H != GPT_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C) \
    )
    #error "Software Version Numbers of Gpt_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h and Gpt_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c are different"
#endif
/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
/**
* 
* @brief The callback functions defined by the user to be called as channel notifications
*/
[!VAR "AlreadyParsedNotification"!][!ENDVAR!][!//
[!LOOP "GptChannelConfigSet/*/*/GptNotification"!][!//
[!IF "(. != 'NULL_PTR') and (. != 'NULL')"!][!//
[!IF "contains($AlreadyParsedNotification, concat(.,'|')) = false()"!][!//
extern void [!"."!](void);
[!VAR "AlreadyParsedNotification"="concat($AlreadyParsedNotification,.,'|')"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//

[!NOCODE!]
[!VAR "GptConfiguredPartitions" = "num:i((count(GptDriverConfiguration/GptEcucPartitionRef/*)))"!][!//
[!VAR "GptEcucPartitionRefCount" = "num:i(count(as:modconf('Gpt')[1]/GptDriverConfiguration/GptEcucPartitionRef/*))"!]
[!IF "$GptEcucPartitionRefCount != 0 "!]
    [!VAR "GptEcucPartitionRefNum" = "$GptEcucPartitionRefCount"!] 
[!ELSE!]
    [!VAR "GptEcucPartitionRefNum" = "1"!] 
[!ENDIF!]

    [!IF "var:defined('postBuildVariant')"!]
        [!VAR "VS_number" = "concat('_',$postBuildVariant)"!]
    [!ELSE!]
        [!VAR "VS_number" = "''"!]
    [!ENDIF!]
[!ENDNOCODE!]
/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/
#define GPT_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Gpt_MemMap.h"

[!INCLUDE "Gpt_PluginMacros.m"!]
[!INCLUDE "Gpt_Ipw_PluginMacros.m"!]
[!VAR "GptConfiguredPartitions" = "num:i((count(GptDriverConfiguration/GptEcucPartitionRef/*)))"!][!//
[!IF "$GptConfiguredPartitions > 0"!][!//
[!LOOP "GptDriverConfiguration/GptEcucPartitionRef/*"!][!// 
[!VAR "currentPartition" = "node:name(node:value(.))"!]
/* Number of channels per partition */
#define GPT_CONF_CHANNELS_PB[!"$VS_number"!]_P_[!"$currentPartition"!][!WS"4"!][!"num:i(count(//GptChannelConfigSet/GptChannelConfiguration/*[contains(./GptChannelEcucPartitionRef/*,$currentPartition)]))"!]U
[!ENDLOOP!][!// 
[!ELSE!]
/* Number of channels per variant without partitions */
#define GPT_CONF_CHANNELS_PB[!"$VS_number"!][!WS"4"!][!"num:i(count(GptChannelConfigSet/GptChannelConfiguration/*))"!]U
[!ENDIF!][!//
#define GPT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Gpt_MemMap.h"

#define GPT_START_SEC_CONST_UNSPECIFIED
#include "Gpt_MemMap.h"
#if(GPT_PREDEFTIMER_FUNCTIONALITY_API == STD_ON)
/* Predefined timers channels configuration */
[!NOCODE!]
[!VAR "flag_1us_16bit" = "'false'"!]
[!VAR "flag_1us_16bit_24bit" = "'false'"!]
[!VAR "flag_1us_16bit_24bit_32bit" = "'false'"!]
[!VAR "flag_100us_32bit" = "'false'"!]
[!IF "GptConfigurationOfOptApiServices/GptPredefTimerFunctionalityApi"!]
[!SELECT "GptDriverConfiguration"!]
    [!IF "(GptPredefTimer1usEnablingGrade = 'GPT_PREDEF_TIMER_1US_16BIT_ENABLED')"!]
        [!VAR "flag_1us_16bit"="'true'"!]
    [!ENDIF!]
    [!IF "(GptPredefTimer1usEnablingGrade = 'GPT_PREDEF_TIMER_1US_16_24BIT_ENABLED')"!]
        [!VAR "flag_1us_16bit_24bit"="'true'"!]
    [!ENDIF!]
    [!IF "(GptPredefTimer1usEnablingGrade = 'GPT_PREDEF_TIMER_1US_16_24_32BIT_ENABLED')"!]
        [!VAR "flag_1us_16bit_24bit_32bit"="'true'"!]
    [!ENDIF!]
    [!IF "(GptPredefTimer100us32bitEnable = 'true')"!]
        [!VAR "flag_100us_32bit"="'true'"!]
    [!ENDIF!]    
[!ENDSELECT!]
[!ENDIF!]
[!CODE!][!//
[!IF "($flag_1us_16bit = 'true') or ($flag_1us_16bit_24bit = 'true') or ($flag_1us_16bit_24bit_32bit = 'true')"!][!//
static const Gpt_HwPredefChannelConfigType Gpt_pInitPredefTimer_1us_16bitPB[!"$VS_number"!]=
{
[!NOCODE!][!//
  [!CALL "BuildHwchannelPredefTimer","predefType"="string('PREDEF_TIMER_1US_16BIT')"!]
[!ENDNOCODE!][!//
};
[!ENDIF!][!//
[!//
[!IF "($flag_1us_16bit_24bit = 'true')or ($flag_1us_16bit_24bit_32bit = 'true')"!][!//
static const Gpt_HwPredefChannelConfigType Gpt_pInitPredefTimer_1us_24bitPB[!"$VS_number"!]=
{
[!NOCODE!][!//
  [!CALL "BuildHwchannelPredefTimer","predefType"="string('PREDEF_TIMER_1US_24BIT')"!]
[!ENDNOCODE!][!//
};
[!ENDIF!][!//
[!//
[!IF "($flag_1us_16bit_24bit_32bit = 'true')"!][!//
static const Gpt_HwPredefChannelConfigType Gpt_pInitPredefTimer_1us_32bitPB[!"$VS_number"!]=
{
[!NOCODE!][!//
  [!CALL "BuildHwchannelPredefTimer","predefType"="string('PREDEF_TIMER_1US_32BIT')"!]
[!ENDNOCODE!][!//
};
[!ENDIF!][!//
[!//
[!IF "$flag_100us_32bit = 'true'"!][!//
static const Gpt_HwPredefChannelConfigType Gpt_pInitPredefTimer_100us_32bitPB[!"$VS_number"!]=
{
[!NOCODE!][!//
  [!CALL "BuildHwchannelPredefTimer","predefType"="string('PREDEF_TIMER_100US_32BIT')"!]
[!ENDNOCODE!][!//
};
[!ENDIF!][!//
[!ENDCODE!][!//
[!ENDNOCODE!][!//

/*Predefined timer configuration structure.*/
static const Gpt_HwPredefChannelConfigType *const Gpt_pInitPredefTimerChannelPB[!"$VS_number"!][GPT_HW_PREDEFTIMER_NUM]=
{
[!IF "($flag_1us_16bit = 'true') or ($flag_1us_16bit_24bit = 'true')or ($flag_1us_16bit_24bit_32bit = 'true')"!][!//
    &Gpt_pInitPredefTimer_1us_16bitPB[!"$VS_number"!],
[!ELSE!][!//
    NULL_PTR,
[!ENDIF!][!//
[!IF "($flag_1us_16bit_24bit = 'true')or ($flag_1us_16bit_24bit_32bit = 'true')"!][!//
    &Gpt_pInitPredefTimer_1us_24bitPB[!"$VS_number"!],
[!ELSE!][!//
    NULL_PTR,
[!ENDIF!][!//
[!IF "($flag_1us_16bit_24bit_32bit = 'true')"!][!//
    &Gpt_pInitPredefTimer_1us_32bitPB[!"$VS_number"!],
[!ELSE!][!//
    NULL_PTR,
[!ENDIF!][!//
[!IF "($flag_100us_32bit = 'true')"!][!//
    &Gpt_pInitPredefTimer_100us_32bitPB[!"$VS_number"!]
[!ELSE!][!//
    NULL_PTR
[!ENDIF!][!//
};
#endif /*GPT_PREDEFTIMER_FUNCTIONALITY_API == STD_ON*/

/* Channels configuration set.*/
[!SELECT "./GptDriverConfiguration/GptEcucPartitionRef"!]
[!NOCODE!][!//
[!FOR "GptPartitionId_temp" = "0" TO "$GptConfiguredPartitions"!]
    [!IF "$GptConfiguredPartitions!= 0 "!]
        [!IF "$GptPartitionId_temp = $GptConfiguredPartitions"!] [!BREAK!] [!ENDIF!] 
        [!VAR "GptEcucPartitionRefName" = "text:split(./*[$GptPartitionId_temp + 1],'/')[5]"!]
        [!VAR "GptPartitionIndex" = "concat('_P_',$GptEcucPartitionRefName)"!]
    [!ELSE!]
        [!VAR "GptEcucPartitionRefName" = "''"!]
        [!VAR "GptPartitionIndex" = "''"!]
    [!ENDIF!] 
[!CODE!]
static const uint8 u8GptChannelIdToIndexMap[!"$VS_number"!][!"$GptPartitionIndex"!][GPT_NUM_CONFIG] =
{[!CR!][!ENDCODE!][!//
[!VAR "GptChannelNumber" = "num:i(count(../../GptChannelConfigSet/GptChannelConfiguration/*))"!]
[!VAR "ChannelIndex" = "0"!][!//
[!VAR "Index" = "0"!][!//
[!IF "$GptConfiguredPartitions!= 0 "!]
[!FOR "outerIndex" = "0" TO "num:i($GptChannelNumber)-1"!][!//
    [!VAR "Index" = "$Index + 1"!]
    [!IF "$Index < num:i($GptChannelNumber)"!]
        [!VAR "comma" = "','"!]
    [!ELSE!]
        [!VAR "comma" = "' '"!]
    [!ENDIF!]
    [!SELECT "//GptChannelConfigSet/GptChannelConfiguration/*[GptChannelId=$outerIndex]"!][!//
            [!VAR "currentPartitionNumber" = "node:value(GptChannelEcucPartitionRef/*)"!][!//            
                [!IF "contains($currentPartitionNumber,$GptEcucPartitionRefName)"!][!//
                [!VAR "ChannelIndex" = "$ChannelIndex + 1"!][!//
                [!CODE!][!WS "4"!][!"num:i($ChannelIndex - 1)"!][!"$comma"!][!WS "8"!]/* Logical Channel [!"node:name(.)"!] */[!CR!][!ENDCODE!]
                [!ELSE!]
                [!CODE!][!WS "4"!]255[!"$comma"!][!WS "6"!]/* No configuration structure of GPT for [!"$GptEcucPartitionRefName"!] */[!CR!][!ENDCODE!][!//
                [!ENDIF!][!// 
    [!ENDSELECT!][!//
[!ENDFOR!]
[!ELSE!]
    [!FOR "x" = "0" TO "num:i($GptChannelNumber)-1"!][!//
    [!VAR "Index" = "$Index + 1"!]
    [!IF "$Index < num:i($GptChannelNumber)"!]
        [!VAR "comma" = "','"!]
    [!ELSE!]
        [!VAR "comma" = "' '"!]
    [!ENDIF!]    
    [!SELECT "//GptChannelConfigSet/GptChannelConfiguration/*/GptChannelId[.=$x]"!][!//
        [!CODE!][!WS "4"!][!"num:i($x)"!][!"$comma"!] /* Logical Channel [!"node:name(//GptChannelConfigSet/GptChannelConfiguration/*/GptChannelId[.=$x]/..)"!]*/[!CR!][!ENDCODE!]
    [!ENDSELECT!][!//
    [!ENDFOR!][!//
[!ENDIF!][!//
[!CODE!]};[!ENDCODE!]
[!ENDFOR!]
[!ENDNOCODE!][!//
[!ENDSELECT!][!CR!]
#define GPT_STOP_SEC_CONST_UNSPECIFIED
#include "Gpt_MemMap.h"

#define GPT_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Gpt_MemMap.h"
[!VAR "GptConfiguredPartitions" = "num:i((count(GptDriverConfiguration/GptEcucPartitionRef/*)))"!][!//
[!IF "$GptConfiguredPartitions > 0"!][!//
[!LOOP "GptDriverConfiguration/GptEcucPartitionRef/*"!][!//
[!VAR "currentPartitionNumber" = "substring-after(substring-after(substring-after(substring-after(.,'/'),'/'),'/'),'/')"!][!//
static const Gpt_ChannelConfigType Gpt_InitChannelPB[!"$VS_number"!]_P_[!"$currentPartitionNumber"!][GPT_CONF_CHANNELS_PB[!"$VS_number"!]_P_[!"node:name(node:value(.))"!]] =
{
[!NOCODE!][!//
    [!CALL "BuildConfigSet", "Partition" = "$currentPartitionNumber"!][!//
[!ENDNOCODE!][!//
};
[!ENDLOOP!][!//
[!ELSE!][!//
static const Gpt_ChannelConfigType Gpt_InitChannelPB[!"$VS_number"!][GPT_CONF_CHANNELS_PB[!"$VS_number"!]] =
{
[!NOCODE!][!//
    [!CALL "BuildConfigSet", "Partition" = "string(null)"!][!//
[!ENDNOCODE!][!//
};
[!ENDIF!][!//

[!VAR "VariantsNo" = "variant:size()"!][!//
[!IF "var:defined('postBuildVariant')"!][!//
    [!VAR "postBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!][!//
[!ELSE!][!//
    [!VAR "postBuildVariantNameUnderscore" = "string(null)"!][!//
[!ENDIF!][!//
[!CALL "GPT_VARIABLES"!]
[!VAR "GptConfiguredPartitions" = "num:i((count(GptDriverConfiguration/GptEcucPartitionRef/*)))"!][!//
[!IF "((IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size()<=1))"!][!//
[!CODE!][!//
/* VariantPreCompile/LinkTime and at most 1 configured variant */
[!ENDCODE!][!//
[!IF "$GptConfiguredPartitions > 0"!][!//
[!LOOP "GptDriverConfiguration/GptEcucPartitionRef/*"!][!//
[!VAR "currentPartitionName"="node:value(.)"!]
[!CALL "GPT_GET_NUMBER_INSTANCE","GptEcucPartitionRefName"="$currentPartitionName"!]


[!VAR "currentPartitionNumber" = "substring-after(substring-after(substring-after(substring-after(.,'/'),'/'),'/'),'/')"!][!//
[!VAR "MacPartitionName" = "substring-after(node:value('.'), 'EcucPartition_')"!][!//
[!CODE!][!//
/**
* @brief        Gpt configuration type existing GptEcucPartitionRef 
* @details      This is the type of the data structure including the configuration
*               set required for initializing the GPT driver.
*
*/

const Gpt_ConfigType Gpt_Config_[!"$currentPartitionNumber"!] =
{
    /** @brief Number of GPT channels (configured in tresos plugin builder)*/
    (Gpt_ChannelType)[!"num:i(count(//GptChannelConfigSet/GptChannelConfiguration/*[contains(./GptChannelEcucPartitionRef/*,$currentPartitionNumber)]))"!]U,
    /** @brief Pointer to the GPT channel configuration */
    &Gpt_InitChannelPB[!"$VS_number"!]_P_[!"$currentPartitionNumber"!],
    /** @brief Number of GPT instances (configured in tresos plugin builder)*/
    [!"$GptcountInstance"!]U,
    /** @brief Pointer to the GPT instance configuration */    
    &Gpt_Ipw_HwInstanceConfig_PB[!"$VS_number"!]_P_[!"$currentPartitionNumber"!],
    /** @brief GPT_PREDEFTIMER_FUNCTIONALITY_API == STD_ON  */
#if(GPT_PREDEFTIMER_FUNCTIONALITY_API == STD_ON)
    /** @brief Pointer configuration of PredefTimer  */
    Gpt_pInitPredefTimerChannelPB[!"$VS_number"!],
#endif
    /** @brief channel index in each partition map table*/
    &u8GptChannelIdToIndexMap[!"$VS_number"!]_P_[!"$currentPartitionNumber"!]
};
[!ENDCODE!][!//
[!ENDLOOP!]
    [!ELSE!][!//
[!CODE!][!//
/**
* @brief        Gpt configuration type does not existing GptEcucPartitionRef 
* @details      This is the type of the data structure including the configuration
*               set required for initializing the GPT driver.
*
*/
const Gpt_ConfigType Gpt_Config =
{
    /** @brief Number of GPT channels (configured in tresos plugin builder)*/
    (Gpt_ChannelType)[!"num:i(count(GptChannelConfigSet/GptChannelConfiguration/*))"!]U,
    /** @brief Pointer to the GPT channel configuration */
    &Gpt_InitChannelPB[!"$VS_number"!],
    /** @brief Number of GPT instances (configured in tresos plugin builder)*/
    [!"num:i($OutNumInstances)"!]U,
    /** @brief Pointer to the GPT instance configuration */    
    &Gpt_Ipw_HwInstanceConfig_PB[!"$VS_number"!],
#if(GPT_PREDEFTIMER_FUNCTIONALITY_API == STD_ON)
    /** @brief Pointer configuration of PredefTimer  */ 
    Gpt_pInitPredefTimerChannelPB[!"$VS_number"!],
#endif
    /** @brief channel index in each partition map table*/    
    &u8GptChannelIdToIndexMap[!"$VS_number"!]
};
[!ENDCODE!][!//
    [!ENDIF!][!//  
[!ELSE!][!//
[!CODE!][!//
/* VariantPostBuild or more than 1 configured variant */
[!ENDCODE!][!// 
    [!IF "$GptConfiguredPartitions > 0"!][!//
[!LOOP "GptDriverConfiguration/GptEcucPartitionRef/*"!][!//
[!VAR "currentPartitionName"="node:value(.)"!]
[!CALL "GPT_GET_NUMBER_INSTANCE","GptEcucPartitionRefName"="$currentPartitionName"!]

[!VAR "currentPartitionNumber" = "substring-after(substring-after(substring-after(substring-after(.,'/'),'/'),'/'),'/')"!][!//
[!VAR "MacPartitionName" = "substring-after(node:value('.'), 'EcucPartition_')"!][!//
[!CODE!][!//
/**
* @brief        Gpt configuration type existing GptEcucPartitionRef and VariantPostBuild or more than 1 configured variant.
* @details      This is the type of the data structure including the configuration
*               set required for initializing the GPT driver.
*
*/
const Gpt_ConfigType Gpt_Config[!"$postBuildVariantNameUnderscore"!]_[!"$currentPartitionNumber"!] =
{
    /** @brief Number of GPT channels (configured in tresos plugin builder)*/
    (Gpt_ChannelType)[!"num:i(count(//GptChannelConfigSet/GptChannelConfiguration/*[contains(./GptChannelEcucPartitionRef/*,$currentPartitionNumber)]))"!]U,
    /** @brief Pointer to the GPT channel configuration */
    &Gpt_InitChannelPB[!"$VS_number"!]_P_[!"$currentPartitionNumber"!],
    /** @brief Number of GPT instances (configured in tresos plugin builder)*/
    [!"$GptcountInstance"!]U,
    /** @brief Pointer to the GPT instance configuration */    
    &Gpt_Ipw_HwInstanceConfig_PB[!"$VS_number"!]_P_[!"$currentPartitionNumber"!],
#if(GPT_PREDEFTIMER_FUNCTIONALITY_API == STD_ON)
    /** @brief Pointer configuration of PredefTimer  */
    Gpt_pInitPredefTimerChannelPB[!"$VS_number"!],
#endif
    /** @brief channel index in each partition map table*/ 
    &u8GptChannelIdToIndexMap[!"$VS_number"!]_P_[!"$currentPartitionNumber"!]
};
[!ENDCODE!][!//
[!ENDLOOP!]
    [!ELSE!][!//
[!CODE!][!//
/**
* @brief        Gpt configuration type does not existing GptEcucPartitionRef and VariantPostBuild or more than 1 configured variant.
* @details      This is the type of the data structure including the configuration
*               set required for initializing the GPT driver.
*
*/
const Gpt_ConfigType Gpt_Config[!"$postBuildVariantNameUnderscore"!] =
{
    /** @brief Number of GPT channels (configured in tresos plugin builder)*/
    (Gpt_ChannelType)[!"num:i(count(GptChannelConfigSet/GptChannelConfiguration/*))"!]U,
    /** @brief Pointer to the GPT channel configuration */
    &Gpt_InitChannelPB[!"$VS_number"!],
    /** @brief Number of GPT instances (configured in tresos plugin builder)*/
    [!"num:i($OutNumInstances)"!]U,
    /** @brief Pointer to the GPT instance configuration */    
    &Gpt_Ipw_HwInstanceConfig_PB[!"$VS_number"!],
#if(GPT_PREDEFTIMER_FUNCTIONALITY_API == STD_ON)
    /** @brief Pointer configuration of PredefTimer  */
    Gpt_pInitPredefTimerChannelPB[!"$VS_number"!],
#endif
    /** @brief channel index in each partition map table*/ 
    &u8GptChannelIdToIndexMap[!"$VS_number"!]
};
[!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDIF!][!//

[!VAR "GptConfiguredPartitions" = "num:i((count(GptDriverConfiguration/GptEcucPartitionRef/*)))"!][!//
[!IF "$GptConfiguredPartitions!=0"!][!//
[!NOCODE!][!//
[!VAR "maxPartition" = "num:i(0)"!][!//
[!IF "node:value(GptDriverConfiguration/GptMulticoreSupport) = 'true'"!][!//
[!IF "node:exists(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition)"!][!//
    [!VAR "maxPartition" = "num:i(count(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition/*))"!][!// 
[!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!ENDIF!][!//
[!NOCODE!][!//
[!SELECT "as:modconf('Gpt')[1]/GptChannelConfigSet"!][!//
[!IF "(../IMPLEMENTATION_CONFIG_VARIANT !='VariantPostBuild' and (variant:size()<=1))"!][!// 
[!IF "$GptConfiguredPartitions!=0"!][!//
    [!CODE!][!//
const Gpt_ConfigType* const Gpt_Config[GPT_MAX_PARTITIONS] =
{
    [!ENDCODE!][!//
    [!FOR "CurrentCoreId" = "0" TO "num:i($maxPartition) - 1"!][!//
        [!VAR "HasCore" = "0"!][!//
        [!VAR "HasPartition" = "0"!][!//
        [!VAR "foundPartition" = "0"!]  [!//
        [!LOOP "as:modconf('Gpt')[1]/GptDriverConfiguration/GptEcucPartitionRef/*"!][!//
            [!IF "num:i($foundPartition) = 0"!][!//
                [!VAR "GptEcucPartitionRefName" = "node:value(.)"!][!//
                [!VAR "PartitionIndex" = "node:name(node:value(.))"!][!//
                [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
                [!IF "$GptEcucPartitionRefName = node:value(OsAppEcucPartitionRef)"!][!//
                    [!SELECT "node:ref(OsApplicationCoreRef)"!][!//
                    [!VAR "OsCoreId" = "node:value(./EcucCoreId)"!][!//
                        [!IF "num:i($OsCoreId) = num:i($CurrentCoreId)"!][!//
                            [!VAR "HasCore" = "1"!][!//
                            [!LOOP "//GptChannelConfigSet/GptChannelConfiguration/*[1]"!][!//
                            [!LOOP "//GptChannelEcucPartitionRef/*"!][!//
                            [!VAR "GptChannelEcucPartitionRefName" = "node:value(.)"!] [!//
                            [!IF "$GptChannelEcucPartitionRefName = $GptEcucPartitionRefName"!][!//
                                [!VAR "HasPartition" = "1"!][!//
                                [!VAR "foundPartition" = "1"!][!//
                            [!ENDIF!][!//
                            [!ENDLOOP!][!//
                            [!ENDLOOP!][!//
                        [!ENDIF!][!//
                    [!ENDSELECT!][!//
                [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
        [!IF "$HasCore = 1"!][!//
            [!IF "$HasPartition = 1"!][!//
[!CODE!]
    /** @brief  Pointer to configuration structure of GPT for partition [!"$PartitionIndex"!] referred by core [!"num:i($CurrentCoreId)"!] */
    &Gpt_Config_[!"$PartitionIndex"!][!//
[!ENDCODE!]
            [!ELSE!][!//
[!CODE!]
    /** @brief  No configuration structure of GPT referred by core [!"num:i($CurrentCoreId)"!] */
    NULL_PTR[!//
[!ENDCODE!]
            [!ENDIF!]
        [!ELSE!][!//
[!CODE!]
    /** @brief  No configuration structure of GPT referred by core [!"num:i($CurrentCoreId)"!] */
    NULL_PTR[!//
[!ENDCODE!]
        [!ENDIF!][!//
[!IF "$CurrentCoreId < num:i($maxPartition) - 1"!][!CODE!],[!ENDCODE!][!ENDIF!][!CR!]
    [!ENDFOR!]
[!CODE!]
};
[!ENDCODE!]
[!ENDIF!]
[!ENDIF!] /* precompile */
[!ENDSELECT!]
[!ENDNOCODE!]
#define GPT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Gpt_MemMap.h"
/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

#ifdef __cplusplus
}
#endif /* GPT_PBCFG_C */
/** @} */

