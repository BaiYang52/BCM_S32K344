[!AUTOSPACING!]
[!NOCODE!]
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
[!/* Avoid multiple inclusion */!]
[!IF "not(var:defined('GPT_IPW_PLUGIN_MACRO_M'))"!][!//
[!VAR "GPT_IPW_PLUGIN_MACRO_M" = "'true'"!][!//

[!VAR "GptEcucPartitionRefCount" = "num:i(count(as:modconf('Gpt')[1]/GptDriverConfiguration/GptEcucPartitionRef/*))"!]
[!IF "$GptEcucPartitionRefCount != 0 "!]
    [!VAR "GptEcucPartitionRefNum" = "$GptEcucPartitionRefCount"!] 
[!ELSE!]
    [!VAR "GptEcucPartitionRefNum" = "1"!] 
[!ENDIF!]
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "PBCfg_Variant" = "concat('PB_', $postBuildVariant)"!]
[!ELSE!]
    [!VAR "PBCfg_Variant" = "'PB'"!]
[!ENDIF!]

[!// MACRO for creating the mapping between hardware channels and logical channels
[!// need to be called in the context of each partition where $GptEcucPartitionRefName is available 
[!MACRO "GPT_VARIABLES"!][!//
[!NOCODE!]
    [!VAR "MaximumCoreId" = "0"!]
    [!VAR "CurrentCoreId" = "0"!]
    [!VAR "OutRespectiveCoreId" = "0"!]
    [!VAR "OutNumGptChannelsUsingPit" = "0"!]
    [!VAR "OutNumGptChannelsUsingStm"  = "0"!]
    [!VAR "OutNumGptChannelsUsingRtc"  = "0"!]
    [!VAR "OutNumGptChannelsUsingEmios"  = "0"!]
    [!VAR "OutNumGptChannels" = "num:i(count(GptChannelConfigSet/GptChannelConfiguration/*))"!]
    [!VAR "OutNumGptChannelsConfiguration" = "num:i(count(GptChannelConfiguration/*))"!]
    [!VAR "OutNumChannelInPartition"  = "0"!]   
    [!VAR "OutNumInstancesInPartition"  = "0"!]
    [!VAR "OutNumInstances"  = "0"!]   
    [!VAR "OutNumInstancesPit"  = "0"!]   
    [!VAR "OutNumInstancesStm"  = "0"!]   
    [!VAR "OutNumInstancesRtc"  = "0"!]        
    [!// Count channels and instances in each module that is using for current partition
    [!IF "$GptEcucPartitionRefCount != 0 "!]
        [!// Count instances
        [!LOOP "GptPit/*"!]
            [!IF "node:exists(//GptChannelConfigSet/GptChannelConfiguration/*[(node:refs(GptModuleRef)/../../GptPitModule = node:current()/GptPitModule) and (node:value(./GptChannelEcucPartitionRef/*[1]) = $GptEcucPartitionRefName)])"!]
                [!VAR "OutNumInstancesInPartition" = "$OutNumInstancesInPartition + 1"!]
            [!ENDIF!]
        [!ENDLOOP!]
        [!LOOP "GptStm/*"!]
            [!IF "node:exists(//GptChannelConfigSet/GptChannelConfiguration/*[(node:refs(GptModuleRef)/../../GptStmModule = node:current()/GptStmModule) and (node:value(./GptChannelEcucPartitionRef/*[1]) = $GptEcucPartitionRefName)])"!]
                [!VAR "OutNumInstancesInPartition" = "$OutNumInstancesInPartition + 1"!]
            [!ENDIF!]
        [!ENDLOOP!]
        [!LOOP "GptEmios/*"!]
            [!IF "node:exists(//GptChannelConfigSet/GptChannelConfiguration/*[(node:refs(GptModuleRef)/../../GptEmiosModule = node:current()/GptEmiosModule) and (node:value(./GptChannelEcucPartitionRef/*[1]) = $GptEcucPartitionRefName)])"!]
                [!VAR "OutNumInstancesInPartition" = "$OutNumInstancesInPartition + 1"!]
            [!ENDIF!]
        [!ENDLOOP!]

        [!IF "node:exists(//GptChannelConfigSet/GptChannelConfiguration/*[(GptHwIp = 'RTC') and (node:value(./GptChannelEcucPartitionRef/*[1]) = $GptEcucPartitionRefName)])"!]
            [!VAR "OutNumInstancesInPartition" = "$OutNumInstancesInPartition + 1"!]
        [!ENDIF!]

        
        [!// Count channels
        [!LOOP "GptChannelConfiguration/*"!]
            [!VAR "HardwareModule" = "./GptHwIp"!]
            [!LOOP "./GptChannelEcucPartitionRef/*"!]
                [!IF "node:value(.) = $GptEcucPartitionRefName"!]
                    [!//Update variables storing number of Gpt channels using either Pit, Stm
                    [!IF "contains($HardwareModule,'PIT')"!]
                        [!VAR "OutNumGptChannelsUsingPit" = "$OutNumGptChannelsUsingPit + 1"!]
                    [!ELSEIF "contains($HardwareModule,'STM')"!]
                        [!VAR "OutNumGptChannelsUsingStm" = "$OutNumGptChannelsUsingStm + 1"!]
                    [!ELSEIF "contains($HardwareModule,'RTC')"!]
                        [!VAR "OutNumGptChannelsUsingRtc" = "$OutNumGptChannelsUsingRtc + 1"!]
                    [!ELSEIF "contains($HardwareModule,'EMIOS')"!]
                        [!VAR "OutNumGptChannelsUsingEmios" = "$OutNumGptChannelsUsingEmios + 1"!]
                    [!ENDIF!]
                    [!VAR "OutNumChannelInPartition" = "$OutNumChannelInPartition + 1"!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ENDLOOP!]
        [!LOOP "../GptDriverConfiguration/GptEcucPartitionRef/*"!]
            [!VAR "GptPartRefName" = "node:value(.)"!]
            [!VAR "GptPartRefShortName" = "substring-after($GptPartRefName, '/')"!]
            [!VAR "GptPartRefShortName" = "substring-after($GptPartRefShortName, '/')"!]
            [!VAR "GptPartRefShortName" = "substring-after($GptPartRefShortName, '/')"!]
            [!VAR "GptPartRefShortName" = "substring-after($GptPartRefShortName, '/')"!]
            [!VAR "GptPartRefShortName" = "text:toupper($GptPartRefShortName)"!]
            [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
                [!IF "$GptPartRefName = node:value(OsAppEcucPartitionRef)"!]
                    [!SELECT "node:ref(OsApplicationCoreRef)"!]
                        [!IF "$GptPartRefName = $GptEcucPartitionRefName"!]
                            [!VAR "OutRespectiveCoreId" = "node:value(./EcucCoreId)"!]
                        [!ENDIF!]
                        [!VAR "CurrentCoreId" = "node:value(./EcucCoreId)"!]
                        [!IF "num:i($MaximumCoreId) < num:i($CurrentCoreId)"!]
                            [!VAR "MaximumCoreId" = "$CurrentCoreId"!]
                        [!ENDIF!]
                    [!ENDSELECT!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ENDLOOP!]
    [!ELSE!]
        [!// TODO: Count instances
        
        
    [!LOOP "GptChannelConfigSet/GptPit/*"!]
    [!VAR "HardwareModule" = "./GptPitModule"!]
    [!VAR "chUsed"  = "0"!]
    [!LOOP "../../../GptChannelConfigSet/GptChannelConfiguration/*"!]
        [!IF "contains(GptHwIp,'PIT')"!]
            [!IF "node:value(node:ref(./GptModuleRef)/../../GptPitModule) = $HardwareModule"!] 
                    [!IF "$chUsed = 0"!] 
                        [!VAR "OutNumInstancesPit"  = "$OutNumInstancesPit + 1"!]
                        [!VAR "chUsed"  = "1"!]
                    [!ENDIF!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
    [!ENDLOOP!]
    
    [!VAR "OutNumInstancesStm"  = "0"!]
    [!LOOP "GptChannelConfigSet/GptStm/*"!]
    [!VAR "HardwareModule" = "./GptStmModule"!]
    [!VAR "chUsed"  = "0"!]
    [!LOOP "../../../GptChannelConfigSet/GptChannelConfiguration/*"!]        
        [!IF "contains(GptHwIp,'STM')"!]                    
            [!IF "node:value(node:ref(./GptModuleRef)/../../GptStmModule) = $HardwareModule"!]
                    [!IF "$chUsed = 0"!] 
                        [!VAR "OutNumInstancesStm"  = "$OutNumInstancesStm + 1"!]
                        [!VAR "chUsed"  = "1"!]
                    [!ENDIF!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
    [!ENDLOOP!]
    
    [!VAR "OutNumInstancesEmios"  = "0"!]
    [!LOOP "GptChannelConfigSet/GptEmios/*"!]
    [!VAR "HardwareModule" = "./GptEmiosModule"!]
    [!VAR "chUsed"  = "0"!]
    [!LOOP "../../../GptChannelConfigSet/GptChannelConfiguration/*"!]        
        [!IF "contains(GptHwIp,'EMIOS')"!]                    
            [!IF "node:value(node:ref(./GptModuleRef)/../../GptEmiosModule) = $HardwareModule"!]
                    [!IF "$chUsed = 0"!] 
                        [!VAR "OutNumInstancesEmios"  = "$OutNumInstancesEmios + 1"!]
                        [!VAR "chUsed"  = "1"!]
                    [!ENDIF!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
    [!ENDLOOP!]
    
    
    [!LOOP "GptChannelConfigSet/GptRtc/*"!]
    [!VAR "HardwareModule" = "./GptRtcModule"!]
    [!VAR "chUsed"  = "0"!]
    [!LOOP "../../../GptChannelConfigSet/GptChannelConfiguration/*"!]
        [!IF "contains(GptHwIp,'RTC')"!]
            [!IF "node:value(node:ref(./GptModuleRef)/GptRtcModule) = $HardwareModule"!] 
                    [!IF "$chUsed = 0"!] 
                        [!VAR "OutNumInstancesRtc"  = "$OutNumInstancesRtc + 1"!]
                        [!VAR "chUsed"  = "1"!]
                    [!ENDIF!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
    [!ENDLOOP!]
    
    [!VAR "OutNumInstances" = "$OutNumInstancesPit + $OutNumInstancesStm + $OutNumInstancesRtc + $OutNumInstancesEmios"!]
    
        
            [!LOOP "GptPit/*"!]
            [!IF "node:exists(//GptChannelConfigSet/GptChannelConfiguration/*[(node:refs(GptModuleRef)/../../GptPitModule = node:current()/GptPitModule)])"!]
                [!VAR "OutNumInstancesInPartition" = "$OutNumInstancesInPartition + 1"!]
            [!ENDIF!]
            [!ENDLOOP!]
            [!LOOP "GptStm/*"!]
            [!IF "node:exists(//GptChannelConfigSet/GptChannelConfiguration/*[(node:refs(GptModuleRef)/../../GptStmModule = node:current()/GptStmModule)])"!]
                [!VAR "OutNumInstancesInPartition" = "$OutNumInstancesInPartition + 1"!]
            [!ENDIF!]
            [!ENDLOOP!]
            [!LOOP "GptEmios/*"!]
            [!IF "node:exists(//GptChannelConfigSet/GptChannelConfiguration/*[(node:refs(GptModuleRef)/../../GptEmiosModule = node:current()/GptEmiosModule)])"!]
                [!VAR "OutNumInstancesInPartition" = "$OutNumInstancesInPartition + 1"!]
            [!ENDIF!]
            [!ENDLOOP!]
            
            [!IF "node:exists(//GptChannelConfigSet/GptChannelConfiguration/*[(GptHwIp = 'RTC')])"!]
                [!VAR "OutNumInstancesInPartition" = "$OutNumInstancesInPartition + 1"!]
            [!ENDIF!]

        [!VAR "OutNumChannelInPartition" = "num:i(count(GptChannelConfiguration/*))"!]
        [!// Count channels and instances
        [!LOOP "GptChannelConfiguration/*"!]
            [!IF "contains(GptHwIp,'PIT')"!]
                [!VAR "OutNumGptChannelsUsingPit" = "$OutNumGptChannelsUsingPit + 1"!]
            [!ELSEIF "contains(GptHwIp,'STM')"!]
                [!VAR "OutNumGptChannelsUsingStm" = "$OutNumGptChannelsUsingStm + 1"!]
            [!ELSEIF "contains(GptHwIp,'RTC')"!]
                [!VAR "OutNumGptChannelsUsingRtc" = "$OutNumGptChannelsUsingRtc + 1"!]
            [!ELSEIF "contains(GptHwIp,'EMIOS')"!]
                [!VAR "OutNumGptChannelsUsingEmios" = "$OutNumGptChannelsUsingEmios + 1"!]    
            [!ENDIF!]
        [!ENDLOOP!]
    [!ENDIF!]
[!ENDNOCODE!]
[!ENDMACRO!][!// End of Macro GPT_VARIABLES

[!ENDIF!] /* Avoid multiple inclusion ENDIF */
[!ENDNOCODE!] /*Gpt_Ipw_PluginMacros.m*/
/** @} */