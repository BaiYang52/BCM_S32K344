[!AUTOSPACING!]
[!NOCODE!]
[!NOCODE!]
    [!IF "var:defined('postBuildVariant')"!]
        [!VAR "VS_number" = "concat('_',$postBuildVariant)"!]
    [!ELSE!]
        [!VAR "VS_number" = "''"!]
    [!ENDIF!]
[!ENDNOCODE!]
/**
*   @file    Gpt_PluginMacros.m
*   @version 2.0.1
*
*   @brief   AUTOSAR Gpt - Macros used to generate code template for GPT driver.
*   @details Macros used to generate code template for GPT driver.
*
*   @addtogroup gpt Gpt Driver
*   @{
*/
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Stm_Pit_Rtc_Emios
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
==================================================================================================*/
/*==================================================================================================
==================================================================================================*/
[!/* Avoid multiple inclusion */!]
[!IF "not(var:defined('GPT_PLUGIN_MACRO_M'))"!][!//
[!VAR "GPT_PLUGIN_MACRO_M" = "'true'"!][!//

[!//This macros is used to build the configuration structure for a config set.
[!MACRO "BuildConfigSet" , "Partition"!]
[!VAR "MacFirstMember" = "string('TRUE')"!]
[!VAR "Index" = "0"!][!//
[!FOR "x" = "0" TO "num:i(count(//GptChannelConfigSet/GptChannelConfiguration/*))"!][!//
    [!SELECT "//GptChannelConfigSet/GptChannelConfiguration/*/GptChannelId[.=$x]/.."!][!//
    [!IF "(contains(GptChannelEcucPartitionRef/*,$Partition)) or ($Partition = string(null))"!]
    [!IF "$MacFirstMember = string('TRUE')"!][!VAR "MacFirstMember" = "string('FALSE')"!][!ELSE!][!CODE!],[!CR!][!ENDCODE!][!ENDIF!]
    [!VAR "Index" = "$Index + 1"!]
        [!CODE!][!WS"4"!]{   /*[!"node:name(.)"!] configuration data*/[!CR!][!ENDCODE!]
        [!IF "GptEnableWakeup"!][!//
            [!CODE!][!WS"8"!](boolean)TRUE, /* Wakeup capability */[!CR!][!ENDCODE!]
        [!ELSE!][!//
            [!CODE!][!WS"8"!](boolean)FALSE, /* Wakeup capability */[!CR!][!ENDCODE!]
        [!ENDIF!]
        [!IF "node:exists(GptNotification)"!][!//
            [!IF "(GptNotification != '') and (GptNotification != 'NULL_PTR') and (GptNotification != 'NULL')"!][!//
                [!CODE!][!WS"8"!]&[!"GptNotification"!], /* Channel notification */[!CR!][!ENDCODE!]
            [!ELSE!][!//
                [!CODE!][!WS"8"!]NULL_PTR, /* Channel notification */[!CR!][!ENDCODE!]
            [!ENDIF!][!//
        [!ELSE!][!//
            [!CODE!][!WS"8"!]NULL_PTR, /* Channel notification */[!CR!][!ENDCODE!]
        [!ENDIF!]
        
        [!CODE!]#if ((GPT_WAKEUP_FUNCTIONALITY_API == STD_ON) && (GPT_REPORT_WAKEUP_SOURCE == STD_ON))[!CR!][!ENDCODE!]
        
        [!IF "node:exists(GptWakeupConfiguration)"!][!//
            [!CODE!][!WS"8"!](EcuM_WakeupSourceType)((uint32)((uint32)1 << [!"as:ref(GptWakeupConfiguration/GptWakeupSourceRef)/EcuMWakeupSourceId"!])), /* Wakeup information */[!CR!][!ENDCODE!]
        [!ELSE!][!//
            [!CODE!][!WS"8"!](EcuM_WakeupSourceType)0U, /* Wakeup information */[!CR!][!ENDCODE!]
        [!ENDIF!]
        
        [!CODE!]#endif[!CR!][!ENDCODE!]
        
        [!CODE!][!WS"8"!](Gpt_ValueType)([!"num:i(GptChannelTickValueMax)"!]U), /* Maximum ticks value*/
        ([!"GptChannelMode"!]), /* Timer mode:continous/one-shot */[!CR!][!ENDCODE!]
        
        [!CODE!][!WS"8"!]&Gpt_Ipw_ChannelConfig_PB[!"$VS_number"!][!IF "$GptEcucPartitionRefCount != 0 "!]_P_[!"$Partition"!][!ELSE!][!"$Partition"!][!ENDIF!][[!"num:i($Index -1)"!]U][!CR!][!ENDCODE!]
        
    [!CODE!][!WS"4"!]}[!CR!][!ENDCODE!][!//
    [!ENDIF!]
    [!ENDSELECT!][!//
[!ENDFOR!][!//
[!ENDMACRO!][!//

[!// need to be called in the context of each partition $GptEcucPartitionRefName is available 
[!MACRO "GPT_GET_NUMBER_INSTANCE", "GptEcucPartitionRefName"!][!//
[!NOCODE!][!//
[!VAR "GptcountInstance" = "0"!]
[!VAR "GptcountInstanceRtc" = "0"!]
[!VAR "GptcountInstanceRestIp" = "0"!]
[!VAR "GptListInstance" = "''"!]
        [!LOOP "../../../GptChannelConfigSet/GptChannelConfiguration/*[./GptChannelEcucPartitionRef/*[1] = $GptEcucPartitionRefName]/GptModuleRef"!]
        [!VAR "HardwareModule" = "node:ref(.)/*[1]"!]
        [!IF "contains($HardwareModule,'RTC')"!]
        [!VAR "GptcountInstanceRtc" = "num:i($GptcountInstanceRtc +1)"!]
        [!ENDIF!]
            [!VAR "Gpt_Instance" = "node:ref(.)/*[1]/../../../*[1]"!]
                [!IF "not(contains($GptListInstance,$Gpt_Instance))"!]
                     [!VAR "GptListInstance" = "concat($GptListInstance,concat($Gpt_Instance,','))"!]
                     [!VAR "GptcountInstanceRestIp" = "num:i($GptcountInstanceRestIp +1)"!]
                [!ENDIF!] 
        [!ENDLOOP!]
        [!VAR "GptcountInstance" = "num:i($GptcountInstanceRestIp + $GptcountInstanceRtc)"!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//


[!MACRO "PredefTimerStmConsistencyCheck"!]
[!VAR "MacCurrentModule" = "substring-before(GptHwChannel,'_PREDEF')"!]
[!VAR "MacGptChannelClkSrcRef" = "GptChannelClkSrcRef"!]
[!VAR "MacGptChannelPrescaler" = "GptChannelPrescaler"!]
[!VAR "MacGptFreezeEnable" = "GptFreezeEnable"!]
[!LOOP "../*"!]
     [!IF "substring-before(GptHwChannel,'_PREDEF') = $MacCurrentModule"!]
        [!IF "GptChannelClkSrcRef != $MacGptChannelClkSrcRef"!]
            [!ERROR!]
                GptChannelClkSrcRef of predeftimer [!"$MacCurrentModule"!] is not consistency with other predeftimer channel
            [!ENDERROR!]
        [!ENDIF!]
        [!IF "GptChannelPrescaler != $MacGptChannelPrescaler"!]
            [!ERROR!]
                GptChannelPrescaler of predeftimer [!"$MacCurrentModule"!] is not consistency with other predeftimer channel
            [!ENDERROR!]
        [!ENDIF!]
        [!IF "GptFreezeEnable != $MacGptFreezeEnable"!]
            [!ERROR!]
                GptFreezeEnable of predeftimer [!"$MacCurrentModule"!] is not consistency with other predeftimer channel
            [!ENDERROR!]
        [!ENDIF!]
    [!ENDIF!]
[!ENDLOOP!]
[!LOOP "../../GptChannelConfigSet/GptChannelConfiguration/*"!]
    [!IF "contains(GptHwIp,'STM')"!]
       [!IF "node:value(node:ref(./GptModuleRef)/../../GptStmModule) = $MacCurrentModule"!]
            [!IF "GptChannelClkSrcRef != $MacGptChannelClkSrcRef"!]
                [!ERROR!]
                    GptChannelClkSrcRef of predeftimer [!"$MacCurrentModule"!] is not consistency with Gpt channel [!"GptChannelId"!]
                [!ENDERROR!]
            [!ENDIF!]
            [!IF "node:value(node:ref(./GptModuleRef)/../../GptStmPrescaler) != $MacGptChannelPrescaler"!]
                [!ERROR!]
                    GptChannelPrescaler of predeftimer [!"$MacCurrentModule"!] is not consistency with Gpt channel [!"GptChannelId"!]
                [!ENDERROR!]
            [!ENDIF!]
            [!IF "node:value(node:ref(./GptModuleRef)/../../StmFreezeEnable) != $MacGptFreezeEnable"!]
                [!ERROR!]
                    GptFreezeEnable of predeftimer [!"$MacCurrentModule"!] is not consistency with Gpt channel [!"GptChannelId"!]
                [!ENDERROR!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDIF!]
[!ENDLOOP!]
[!ENDMACRO!]

[!//This macros is used to build the Predef timer configuration structure.
[!MACRO "BuildHwchannelPredefTimer", "predefType"!]
[!NOCODE!]

[!VAR "path_temp" = "'GptPredefTimer_1us_16Bit'"!][!//
[!IF "$predefType = 'PREDEF_TIMER_1US_16BIT'"!][!//
    [!VAR "path_temp" = "'GptPredefTimer_1us_16Bit'"!][!//
[!ELSEIF "$predefType = 'PREDEF_TIMER_1US_24BIT'"!][!//
    [!VAR "path_temp" = "'GptPredefTimer_1us_24Bit'"!][!//
[!ELSEIF "$predefType = 'PREDEF_TIMER_1US_32BIT'"!][!//
    [!VAR "path_temp" = "'GptPredefTimer_1us_32Bit'"!][!//
[!ELSE!][!//
    [!VAR "path_temp" = "'GptPredefTimer_100us_32Bit'"!][!//
[!ENDIF!][!//

[!VAR "path_predef" = "concat('GptPredefTimerConfiguration/',$path_temp)"!]
[!SELECT "node:ref($path_predef)"!]
    [!IF "contains(GptHwChannel,'STM')"!][!//
        [!CALL "PredefTimerStmConsistencyCheck"!][!//
    [!ENDIF!][!//
    [!CODE!][!WS"4"!](GPT_[!"substring-before(GptHwChannel, '_')"!]_MODULE), /* hardware module type */[!CR!][!ENDCODE!]
    [!CODE!][!WS"4"!](uint8)([!"substring-before(substring-after(GptHwChannel, '_'),'_PREDEF')"!]U), /* hw instance no. */[!CR!][!ENDCODE!]
    [!CODE!][!WS"4"!](uint8)0U, /* GPT physical channel no. */[!CR!][!ENDCODE!]
    [!IF "contains(GptHwChannel, 'FTM')"!][!//
        [!IF "GptFtmChannelClkSrc = 'FTM_GPT_IP_CLOCK_SOURCE_NONE'"!]
            [!CODE!][!WS"4"!](Gpt_ClockSourceType)0U, /* FTM uses CLOCK_SOURCE_NONE*/[!CR!][!ENDCODE!]
        [!ELSEIF "GptFtmChannelClkSrc = 'FTM_GPT_IP_CLOCK_SOURCE_SYSTEMCLK'"!]
            [!CODE!][!WS"4"!](Gpt_ClockSourceType)1U, /* FTM uses CLOCK_SOURCE_SYSTEMCLK*/[!CR!][!ENDCODE!]
        [!ELSEIF "GptFtmChannelClkSrc = 'FTM_GPT_IP_CLOCK_SOURCE_FIXED_FREQUENCY'"!]
            [!CODE!][!WS"4"!](Gpt_ClockSourceType)2U, /* FTM uses SIRC CLOCK_SOURCE_FIXED_FREQUENCY*/[!CR!][!ENDCODE!]
        [!ELSEIF "GptFtmChannelClkSrc = 'FTM_GPT_IP_CLOCK_SOURCE_EXTERNALCLK'"!]
            [!CODE!][!WS"4"!](Gpt_ClockSourceType)3U, /* FTM uses CLOCK_SOURCE_EXTERNALCLK*/[!CR!][!ENDCODE!]
        [!ENDIF!]
    [!ELSEIF "contains(GptHwChannel, 'STM')"!][!//
    [!CODE!][!WS"4"!](Gpt_ClockSourceType)0U, [!CR!][!ENDCODE!]
    [!ENDIF!]
    [!IF "GptFreezeEnable"!][!//
        [!CODE!][!WS"4"!](boolean)TRUE, /* Freeze Enable */[!CR!][!ENDCODE!]
    [!ELSE!][!//
        [!CODE!][!WS"4"!](boolean)(FALSE), /* Freeze Disnable */[!CR!][!ENDCODE!]
    [!ENDIF!][!//
    [!IF "contains(GptHwChannel, 'STM')"!][!//
        [!CODE!][!WS"4"!](Gpt_PrescalerType)([!"num:i(number(GptChannelPrescaler) - 1)"!]U), /* STM Clock divider */[!CR!][!ENDCODE!]
    [!ELSEIF "contains(GptHwChannel, 'FTM')"!][!//
        [!FOR "macIndex" = "0" TO "7"!][!//
                [!VAR "macTemp" = "bit:shr(num:i(number(GptChannelPrescaler)), num:i($macIndex))"!][!//
                [!IF "$macTemp < 2"!]
                    [!CODE!][!WS"4"!](Gpt_PrescalerType)([!"num:i(number($macIndex))"!]U), /* FTM Clock divider */[!CR!][!ENDCODE!][!//
                    [!BREAK!][!//
                [!ENDIF!][!//
        [!ENDFOR!][!//
    [!ENDIF!][!//
        
[!ENDSELECT!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!ENDIF!] /* Avoid multiple inclusion ENDIF */
[!ENDNOCODE!]