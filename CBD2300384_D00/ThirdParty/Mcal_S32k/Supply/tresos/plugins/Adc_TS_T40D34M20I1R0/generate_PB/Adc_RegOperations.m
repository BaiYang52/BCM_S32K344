[!/* *** multiple inclusion protection *** */!]
[!IF "not(var:defined('ADC_REGOPERATIONS_M'))"!]
[!VAR "ADC_REGOPERATIONS_M"="'true'"!]
[!NOCODE!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : ADC_SAR
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

/**
*   @file
*
*   @addtogroup adc_driver_config Adc Driver Configuration
*   @{
*/

[!ENDNOCODE!][!//
[!AUTOSPACING!]

[!MACRO "AdcWdgThresholdDefineNamesMacro"!][!//
[!NOCODE!][!//
[!VAR "MaxUnitCfg"="num:i(count(AdcConfigSet/AdcHwUnit/*)-1)"!][!//
    [!IF "node:value(AutosarExt/AdcEnableThresholdConfigurationApi)='true'"!]
        [!FOR "Unit" = "0" TO "num:i($MaxUnitCfg)"!][!//
            [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
                [!IF "count(AdcChannel/*[AdcEnableThresholds='true']) > 0"!]
                    [!IF "num:i(count(AdcThresholdControl/*)) > 0"!]
                        [!IF "AdcLogicalUnitId = $Unit"!][!//
                            [!LOOP "AdcThresholdControl/*"!]
[!CODE!][!//
#define [!"@name"!]             ([!"concat(num:inttohex(num:i(@index + bit:shl($Unit,12))) ,'U')"!])
[!ENDCODE!][!//
                            [!ENDLOOP!][!//
                        [!ENDIF!][!//
                    [!ENDIF!][!//
                [!ENDIF!][!//
            [!ENDLOOP!][!//
        [!ENDFOR!][!//
    [!ENDIF!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "AdcWdgThresholdConfigMacro"!][!//
[!IF "node:value(AutosarExt/AdcEnableWatchdogApi)='true'"!]
[!NOCODE!][!//
    [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
        [!VAR "MaxPartitionCfg"="num:i(count(AdcGeneral/AdcEcucPartitionRef/*))"!][!//
    [!ELSE!][!//
        [!VAR "MaxPartitionCfg"="1"!][!//
    [!ENDIF!][!//

    [!FOR "Partition" = "0" TO "num:i($MaxPartitionCfg) - 1"!][!//
        [!VAR "MaxHWUnitInPartition"="0"!][!//
        [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
            [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcGeneral/AdcEcucPartitionRef/*[@index = $Partition],'/'),'/'),'/'),'/')"!][!//
            [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
                [!IF "substring-after(substring-after(substring-after(substring-after(AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/') = $CurrentPartition"!]
                    [!VAR "MaxHWUnitInPartition"="$MaxHWUnitInPartition + 1"!][!//
            [!ENDIF!][!//
            [!ENDLOOP!][!//
        [!ELSE!][!//
            [!VAR "MaxHWUnitInPartition"="num:i(count(AdcConfigSet/AdcHwUnit/*))"!][!//
        [!ENDIF!][!//
        [!VAR "MaxUnitCfg"="num:i(count(AdcConfigSet/AdcHwUnit/*)-1)"!][!//
        [!FOR "Unit" = "0" TO "num:i($MaxUnitCfg)"!][!//
            [!VAR "LoopHWUnitPartition"="0"!][!//
            [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
                [!IF "count(AdcChannel/*[AdcEnableThresholds='true']) > 0"!]
                    [!VAR "HWUnitPartition"="0"!][!//
                    [!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!]
                        [!IF "substring-after(substring-after(substring-after(substring-after(AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/') = $CurrentPartition"!][!//
                            [!VAR "HWUnitPartition"="1"!][!//
                        [!ENDIF!][!//
                    [!ELSE!][!//
                        [!VAR "HWUnitPartition"="2"!][!//
                    [!ENDIF!][!//
                    [!IF "num:i(count(AdcThresholdControl/*)) > 0"!]
                        [!IF "(AdcLogicalUnitId = $Unit) and ($HWUnitPartition > 0)"!][!//
                            [!VAR "LoopHWUnitPartition" = "$LoopHWUnitPartition + 1"!][!//
[!CODE!][!//
/**
* @brief          Watchdog threshold control for Logical ID [!"AdcLogicalUnitId"!] corresponding to the [!"AdcHwUnitId"!] configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!] [!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*
*/
static const Adc_Sar_Ip_WdgThresholdType Adc_ThresholdCfg_[!"AdcLogicalUnitId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][[!"num:i(count(AdcThresholdControl/*))"!]] =
{
[!ENDCODE!][!//
                                [!VAR "MaxNum"="count(AdcThresholdControl/*)"!]
                                [!VAR "CrtNum"="0"!]
                                [!FOR "CrtNum" = "0" TO "$MaxNum"!]

                                    [!LOOP "AdcThresholdControl/*"!]
                                        [!IF "@index = $CrtNum"!][!//
                                            [!VAR "WdgThreshRegIndexString" = "'0'"!]
                                            [!VAR "WdgThreshRegIndexString" = "substring-after(AdcThresholdControlRegister, 'ADC_THRESHOLD_REG_')"!]
                                            [!VAR "WdgLowThresholdString" = "'0'"!]
                                            [!VAR "WdgHighThresholdString" = "'0'"!]
                                            [!VAR "WdgLowThresholdEnString" = "'FALSE'"!]
                                            [!VAR "WdgHighThresholdEnString" = "'FALSE'"!]
                                            [!IF "node:exists(AdcLowThreshold)"!]
                                                [!VAR "WdgLowThresholdString" = "AdcLowThreshold"!]
                                                [!VAR "WdgLowThresholdEnString" = "'TRUE'"!]
                                            [!ENDIF!]
                                            [!IF "node:exists(AdcHighThreshold)"!]
                                                [!VAR "WdgHighThresholdString" = "AdcHighThreshold"!]
                                                [!VAR "WdgHighThresholdEnString" = "'TRUE'"!]
                                            [!ENDIF!]
[!CODE!][!//
    {
        [!"$WdgThreshRegIndexString"!]U, /* WdgIndex */
        [!"$WdgLowThresholdString"!]U, /* LowThreshold */
        [!"$WdgHighThresholdString"!]U, /* HighThreshold */
        (boolean)[!"$WdgLowThresholdEnString"!], /* LowThresholdIntEn */
        (boolean)[!"$WdgHighThresholdEnString"!] /* HighThresholdIntEn */
[!IF "$CrtNum < $MaxNum - 1"!][!//
    },
[!ELSE!][!//
    }
[!ENDIF!][!//
[!ENDCODE!][!//
                                [!ENDIF!]
                            [!ENDLOOP!][!//
                        [!ENDFOR!]
[!CODE!][!//
};

[!ENDCODE!][!//
                        [!ENDIF!][!//
                    [!ENDIF!][!//
                [!ENDIF!][!//
            [!ENDLOOP!][!//
        [!ENDFOR!][!//
    [!ENDFOR!][!//
[!ENDNOCODE!]
[!ENDIF!][!//
[!ENDMACRO!][!//



[!MACRO "AdcGroupConfigMacro"!][!//
[!NOCODE!][!//
    [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
        [!VAR "MaxPartitionCfg"="num:i(count(AdcGeneral/AdcEcucPartitionRef/*))"!][!//
    [!ELSE!][!//
        [!VAR "MaxPartitionCfg"="1"!][!//
    [!ENDIF!][!//

    [!VAR "CtuTrigListPos" = "0"!][!// Current position in BCTU/CTU list of group
    [!VAR "FirstCtuTrigGroup" = "0"!][!// First CTU/BCTU trigger mode groups encounterred
    [!FOR "Partition" = "0" TO "num:i($MaxPartitionCfg) - 1"!][!//
        [!VAR "MaxGroupInPartition"="0"!][!//
        [!VAR "LoopGroupPartition"="0"!][!//
        [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
            [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcGeneral/AdcEcucPartitionRef/*[@index = $Partition],'/'),'/'),'/'),'/')"!][!//
            [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
                [!IF "substring-after(substring-after(substring-after(substring-after(AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/') = $CurrentPartition"!]
                    [!VAR "MaxGroupInPartition"="$MaxGroupInPartition + num:i(count(AdcGroup/*))"!][!//
                [!ENDIF!][!//
            [!ENDLOOP!][!//
        [!ELSE!][!//
            [!VAR "MaxGroupInPartition"="$MaxGroupInPartition + num:i(count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*))"!][!//
        [!ENDIF!][!//
        [!VAR "EnableCHDisableCHIndex" ="0"!][!//
        [!CODE!][!//
/**
* @brief          Definition of all ADC groups for configuration variant[!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*/
static const Adc_GroupConfigurationType Adc_GroupsCfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][] =
{
[!ENDCODE!][!//
        [!VAR "NumGroup" = "num:i(count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*)-1)"!][!//
        [!FOR "LoopVar" = "0" TO "$NumGroup"!][!//
            [!LOOP "AdcConfigSet/AdcHwUnit/*/AdcGroup/*"!][!//
                [!VAR "HWUnitPartition"="0"!][!//
                [!IF "node:value(../../../../../AutosarExt/AdcMulticoreSupport)='true'"!]
                    [!IF "substring-after(substring-after(substring-after(substring-after(../../AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/') = $CurrentPartition"!][!//
                        [!VAR "HWUnitPartition"="1"!][!//
                    [!ENDIF!][!//
                [!ELSE!][!//
                    [!VAR "HWUnitPartition"="2"!][!//
                [!ENDIF!][!//

                [!IF "(AdcGroupId = $LoopVar) and ($HWUnitPartition > 0)"!][!//
                    [!VAR "LoopGroupPartition" = "$LoopGroupPartition + 1"!][!//
                    [!VAR "AdcTempCh" = "0"!]
                    [!VAR "AdcChanGroup0" = "0"!]
                    [!VAR "AdcChanGroup1" = "0"!]
                    [!VAR "AdcChanGroup2" = "0"!]
                    [!VAR "FirstDmaCh" = "255"!]
                    [!VAR "LastDmaCh" = "0"!]
                    [!VAR "PreFlag" = "0"!]

                    [!LOOP "AdcGroupDefinition/*"!]
                        [!VAR "CurrAdcChannel" = "."!]
                        [!VAR "CurrAdcChannel1" = "substring-after(substring-after(substring-after(substring-after(substring-after($CurrAdcChannel,'/'),'/'),'/'),'/'),'/')"!]
                        [!LOOP "../../../../AdcChannel/*"!]

                            [!IF "$CurrAdcChannel1 = @name"!]
                                [!IF "contains(AdcChannelName,'ChanNum')"!]
                                    [!VAR "AdcTempCh" = "text:split(AdcChannelName, 'ChanNum')[last()]"!]

                                    [!IF "$AdcTempCh >= $LastDmaCh"!]
                                        [!VAR "LastDmaCh" = "$AdcTempCh"!]
                                    [!ENDIF!]

                                    [!IF "$AdcTempCh <= $FirstDmaCh"!]
                                        [!VAR "FirstDmaCh" = "$AdcTempCh"!]
                                    [!ENDIF!]

                                    [!IF "$AdcTempCh < 32"!]
                                        [!VAR "AdcChanGroup0" = "bit:or($AdcChanGroup0,bit:shl(1,$AdcTempCh))"!]
                                    [!ELSEIF "$AdcTempCh < 64"!]
                                        [!VAR "AdcChanGroup1" = "bit:or($AdcChanGroup1,bit:shl(1,num:i($AdcTempCh mod 32)))"!]
                                    [!ELSEIF "$AdcTempCh < 96"!]
                                        [!VAR "AdcChanGroup2" = "bit:or($AdcChanGroup2,bit:shl(1,num:i($AdcTempCh mod 32)))"!]
                                    [!ENDIF!]

                                [!ENDIF!]
                            [!ENDIF!]
                        [!ENDLOOP!]
                    [!ENDLOOP!]

                    [!VAR "AssignedChannelMask" = "concat('{ { ', text:replace(text:toupper(num:inttohex($AdcChanGroup0)), 'X', 'x'), 'U' )"!]
                    [!IF "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) > 1"!][!//
                        [!VAR "AssignedChannelMask" = "concat($AssignedChannelMask, ', ', text:replace(text:toupper(num:inttohex($AdcChanGroup1)), 'X', 'x'), 'U' )"!]
                    [!ENDIF!]
                    [!IF "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) > 2"!][!//
                        [!VAR "AssignedChannelMask" = "concat($AssignedChannelMask, ', ', text:replace(text:toupper(num:inttohex($AdcChanGroup2)), 'X', 'x'), 'U' )"!]
                    [!ENDIF!]
                    [!VAR "AssignedChannelMask" = "concat($AssignedChannelMask, ' } }' )"!]
                    [!IF "(../../../../../AdcGeneral/AdcGrpNotifCapability = 'true') and (node:exists(AdcNotification))"!][!//
                        [!VAR "AdcNotificationString" = "node:value(AdcNotification)"!]
                    [!ELSE!]
                        [!VAR "AdcNotificationString" = "'NULL_PTR'"!]
                    [!ENDIF!][!//

                    [!VAR "TempCtuTrigListPos" = "0"!]
                    [!IF "(../../../../../AutosarExt/AdcCtuHardwareTriggerOptimization = 'true') and (AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))"!][!//
                        [!IF "$FirstCtuTrigGroup = 0"!][!//
                            [!VAR "FirstCtuTrigGroup" = "1"!]
                            [!VAR "CtuTrigListPos" = "count(AdcGroupDefinition/*)"!]
                        [!ELSE!][!//
                            [!VAR "TempCtuTrigListPos" = "$CtuTrigListPos"!]
                            [!VAR "CtuTrigListPos" = "$CtuTrigListPos + count(AdcGroupDefinition/*)"!]
                        [!ENDIF!][!//
                    [!ENDIF!][!//

                    [!IF "(../../../../../AutosarExt/AdcEnableInitialNotification = 'true')"!][!//
                        [!VAR "AdcExtraNotificationString" = "node:value(AdcExtraNotification)"!]
                    [!ELSE!]
                        [!VAR "AdcExtraNotificationString" = "'NULL_PTR'"!]
                    [!ENDIF!][!//

                    [!IF "AdcGroupTriggSrc = 'ADC_TRIGG_SRC_HW'"!]
                        [!VAR "HwTriggerString"!][!"node:ref(AdcGroupHwTriggerSource)/@name"!][!ENDVAR!]
                    [!ELSE!]
                        [!VAR "HwTriggerString" = "'0U'"!]
                    [!ENDIF!]
[!CODE!][!//
    /**< @brief Group[!"AdcGroupId"!] -- Logical Unit Id [!"../../AdcLogicalUnitId"!] -- Hardware Unit [!"../../AdcHwUnitId"!] */
    {
        /**< @brief Index of group */
        [!"AdcGroupId"!]U, /* GroupId */
        /**< @brief ADC Logical Unit Id that the group belongs to */
        (Adc_HwUnitType)[!"../../AdcLogicalUnitId"!], /* AdcLogicalUnitId */
        /**< @brief Access mode */
        [!"AdcGroupAccessMode"!], /* AccessMode */
        /**< @brief Conversion mode */
        [!"AdcGroupConversionMode"!], /* Mode */
        /**< @brief Conversion type */
        [!"AdcGroupConversionType"!], /* Type */
#if (ADC_PRIORITY_IMPLEMENTATION != ADC_PRIORITY_NONE)
        /**< @brief Priority configured */
        [!IF "(../../../../../AdcGeneral/AdcPriorityImplementation != 'ADC_PRIORITY_NONE') and (node:exists(AdcGroupPriority))"!](Adc_GroupPriorityType)ADC_GROUP_PRIORITY([!"AdcGroupPriority"!]),[!ELSE!](Adc_GroupPriorityType)ADC_GROUP_PRIORITY(0),[!ENDIF!] /* Priority */
#endif /* ADC_PRIORITY_IMPLEMENTATION != ADC_PRIORITY_NONE */
        /**< @brief Replacement mode */
        ADC_GROUP_REPL_ABORT_RESTART, /* ReplacementMode */
        /**< @brief Trigger source configured */
        [!"AdcGroupTriggSrc"!], /* TriggerSource */
#if (STD_ON == ADC_HW_TRIGGER_API)
        /**< @brief Hardware trigger source for the group */
        [!"$HwTriggerString"!], /* HwTriggerSource */
        /**< @brief Hardware trigger signal */
        [!IF "(../../../../../AdcGeneral/AdcHwTriggerApi = 'true') and (AdcGroupTriggSrc = 'ADC_TRIGG_SRC_HW') and (node:exists(AdcHwTrigSignal))"!][!"AdcHwTrigSignal"!],[!ELSE!]ADC_HW_TRIG_RISING_EDGE,[!ENDIF!] /* TriggerEdge */
#endif /* (STD_ON == ADC_HW_TRIGGER_API) */
#if (STD_ON == ADC_GRP_NOTIF_CAPABILITY)
        /**< @brief Notification function */
        [!"$AdcNotificationString"!], /* Notification */
#endif /* (STD_ON == ADC_GRP_NOTIF_CAPABILITY) */
#if (ADC_ENABLE_INITIAL_NOTIFICATION == STD_ON)
        /**< @brief Extra notification function */
        [!"$AdcExtraNotificationString"!], /* ExtraNotification */
#endif /* (ADC_ENABLE_INITIAL_NOTIFICATION == STD_ON) */
        /**< @brief Group Streaming Buffer Mode */
        [!"AdcStreamingBufferMode"!], /* BufferMode */
#if (ADC_ENABLE_CH_DISABLE_CH_NONAUTO_API == STD_ON)
        /**< @brief Index of the group with AdcEnableChDisableChGroup enabled */
        (Adc_GroupType)[!IF "node:fallback(AdcEnableChDisableChGroup,'false') = 'true'"!][!"num:i($EnableCHDisableCHIndex)"!][!VAR "EnableCHDisableCHIndex" = "$EnableCHDisableCHIndex + 1"!][!ELSE!]ADC_ENABLE_CH_DISABLE_CH_INVALID_GROUP_INDEX[!ENDIF!], /* EnableChDisableChGroupIndex */
#endif /* (ADC_ENABLE_CH_DISABLE_CH_NONAUTO_API == STD_ON) */
        /**< @brief Number of streaming samples */
        (Adc_StreamNumSampleType)ADC_STREAMING_NUM_SAMPLES([!"AdcStreamingNumSamples"!]), /* NumSamples */
#if (ADC_ENABLE_GROUP_STREAMING_RESULTS_REORDER == STD_ON)
        /**< @brief Arrange the adc results as multiple sets of group result buffer */
        (boolean)[!IF "(../../../../../AutosarExt/AdcEnableGroupStreamingResultReorder = 'true') and (AdcStreamResultGroup = 'true') and (AdcGroupAccessMode = 'ADC_ACCESS_MODE_STREAMING')"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* StreamResultGroupMultiSets */
#endif /* (ADC_ENABLE_GROUP_STREAMING_RESULTS_REORDER == STD_ON) */
        /**< @brief Channels assigned to the group*/
        Adc_Group[!"AdcGroupId"!]_Assignment_[!"num:i(substring-after(../../AdcHwUnitId, 'ADC'))"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!], /* AssignmentPtr */
        /**< @brief Number of channels in the group*/
        (Adc_ChannelIndexType)ADC_CFGSET[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_GROUP_[!"AdcGroupId"!]_CHANNELS, /* AssignedChannelCount */
        /**< @brief Last Channel configured */
        (Adc_ChannelType)(ADC_ID_CHANNEL_U8([!"num:i($LastDmaCh)"!])), /* LastCh */
        /**< @brief First Channel configured */
        (Adc_ChannelType)(ADC_ID_CHANNEL_U8([!"num:i($FirstDmaCh)"!])), /* FirstCh */
        /**< @brief Enables or Disables the ADC and DMA interrupts */
        (uint8)[!IF "AdcWithoutInterrupts"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!], /* AdcWithoutInterrupt */
        /**< @brief Enables or Disables configuring external DMA channel in the group */
        (uint8)[!IF "AdcExtDMAChanEnable"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!], /* AdcExtDMAChanEnable */
        /**< @brief When true, disables completely DMA configuration done by ADC driver for the group */
        (boolean)[!IF "AdcWithoutDma"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* AdcWithoutDma */
#if (STD_ON == ADC_OPTIMIZE_DMA_STREAMING_GROUPS)
        /**< @brief Enables or Disables the ADC optimize DMA streaming groups feature */
        (boolean)[!IF "AdcEnableOptimizeDmaStreamingGroups"!](TRUE)[!ELSE!](FALSE)[!ENDIF!], /* AdcOptimizeDmaStream */
        /**< @brief Enable/disable half interrupt for group with optimize DMA streaming groups */
        (boolean)[!IF "AdcEnableHalfInterrupt"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* HalfInterrupt */
#endif /* (STD_ON == ADC_OPTIMIZE_DMA_STREAMING_GROUPS) */
#if (ADC_ENABLE_LIMIT_CHECK == STD_ON)
        /**< @brief Enables or disables the usage of limit checking for an ADC group. */
        (boolean)[!IF "node:exists(node:refs(AdcGroupDefinition/*)/AdcChannelLimitCheck) and (node:refs(AdcGroupDefinition/*)/AdcChannelLimitCheck = 'true')"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* AdcGroupLimitcheck */
#endif /* (STD_ON == ADC_ENABLE_LIMIT_CHECK) */
        [!"$AssignedChannelMask"!], /* AssignedChannelMask */
#if (ADC_CTU_HW_TRIGGER_OPTIMIZATION == STD_ON)
        /**< @brief First position of channel in BCTU channel list or CTU command list. */
        [!"num:i($TempCtuTrigListPos)"!], /* CtuTrigListPos */
#endif /* (ADC_CTU_HW_TRIGGER_OPTIMIZATION == STD_ON) */
[!IF "(../../../../../AutosarExt/AdcConvTimeOnce = 'false')"!][!//
#ifdef ADC_AVERAGING_FUNCTIONALITY_SUPPORT
#if (ADC_SET_ADC_CONV_TIME_ONCE == STD_OFF)
        &AdcIpwGroupConfig_[!"AdcGroupId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(../../../../../AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!] /* AdcIpwGroupConfigPtr */
#endif /* (ADC_SET_ADC_CONV_TIME_ONCE == STD_OFF) */
#endif /* ADC_AVERAGING_FUNCTIONALITY_SUPPORT */
[!ENDIF!][!//
    }[!IF "$LoopGroupPartition < num:i($MaxGroupInPartition)"!],[!ENDIF!]
[!ENDCODE!][!//
                [!ENDIF!][!//
            [!ENDLOOP!][!//
        [!ENDFOR!][!//
        [!CODE!][!//
};

[!ENDCODE!][!//
    [!ENDFOR!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//



[!MACRO "AdcGroupIdToIndexMapConfigMacro"!][!//
/**
* @brief         Array of index for mapping the groups.
* @details       Array of index for mapping the groups in each partition.
*
*/
static const uint16 Adc_au16GroupIdToIndexMap[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][ADC_MAX_GROUPS] =
{
[!NOCODE!][!//
[!VAR "MaxCfgGroup" = "count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*)"!][!//
[!FOR "OuterIndex" = "0" TO "num:i($MaxCfgGroup)-1"!][!//
[!VAR "GroupIndex" = "0"!][!//
    [!LOOP "AdcConfigSet/AdcHwUnit/*/AdcGroup/*"!][!//
        [!IF "AdcGroupId = $OuterIndex"!][!//
            [!IF "node:value(../../../../../AutosarExt/AdcMulticoreSupport)='true'"!]
                [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(../../AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/')"!][!//
                    [!FOR "InnerIndex" = "0" TO "num:i($MaxCfgGroup)-1"!][!//
                        [!LOOP "../../../../../AdcConfigSet/AdcHwUnit/*/AdcGroup/*"!][!//
                            [!IF "AdcGroupId = $InnerIndex"!][!//
                                [!IF "substring-after(substring-after(substring-after(substring-after(../../AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/') = $CurrentPartition"!][!//
                                    [!IF "$OuterIndex = $InnerIndex"!][!//
                                        [!IF "$OuterIndex < num:i($MaxCfgGroup)-1"!][!//
                                            [!VAR "Comma" = "','"!]
                                        [!ELSE!]
                                            [!VAR "Comma" = "' '"!]
                                        [!ENDIF!]
[!CODE!][!//
    [!"num:i($GroupIndex)"!][!"$Comma"!] /* Group [!"num:i($OuterIndex)"!] parameters can be found at Adc_GroupsCfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_[!"$CurrentPartition"!][[!"num:i($GroupIndex)"!]] */
[!ENDCODE!][!//
                                [!ELSE!][!//
                                    [!VAR "GroupIndex" = "$GroupIndex + 1"!][!//
                                [!ENDIF!][!//
                            [!ENDIF!][!//
                        [!ENDIF!][!//
                    [!ENDLOOP!][!//
                [!ENDFOR!][!//
            [!ELSE!][!//
[!CODE!][!//
    [!"num:i($OuterIndex)"!][!IF "$OuterIndex < num:i($MaxCfgGroup)-1"!],[!ELSE!][!ENDIF!]
[!ENDCODE!][!//
            [!ENDIF!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDFOR!][!//
[!ENDNOCODE!][!//
};
[!ENDMACRO!][!//

[!MACRO "AdcEcucPartitionRefConfigMacro"!][!//
[!IF "(IMPLEMENTATION_CONFIG_VARIANT !='VariantPostBuild' and (variant:size()<=1))"!][!//
/**
* @brief         Array contains coreId of Partition.
* @details       Array contains coreId of Partition.
*
*/
static const uint8 Adc_u8AdcEcucPartition[ADC_MAX_PARTITIONS_CFG] =
[!ELSE!][!//
/**
* @brief         Array contains coreId used for [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!].
* @details       Array contains coreId used for [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!].
*
*/
[!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
static const uint8 Adc_Partition_Assignment[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][ADC_CFGSET[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PARTITION] =
[!ELSE!][!//
static const uint8 Adc_Partition_Assignment[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][1] =
[!ENDIF!][!//
[!ENDIF!][!//
{
[!NOCODE!][!//
    [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
        [!VAR "MaxCount" = "count(AdcGeneral/AdcEcucPartitionRef/*)-1"!][!//
        [!VAR "CrtCount" = "0"!][!//
        [!LOOP "AdcGeneral/AdcEcucPartitionRef/*"!][!//
            [!VAR "AdcCrtPart" = "node:value(.)"!][!//
            [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
                [!IF "$AdcCrtPart = node:value(./OsAppEcucPartitionRef)"!][!//
                    [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
[!CODE!][!//
    (uint8)([!"node:value(./EcucCoreId)"!]UL)[!IF "$CrtCount < $MaxCount"!],[!ELSE!][!ENDIF!]
[!ENDCODE!][!//
                    [!ENDSELECT!][!//
                [!ENDIF!][!//
            [!ENDLOOP!][!//
            [!VAR "CrtCount" = "$CrtCount + 1"!][!//
        [!ENDLOOP!][!//
    [!ELSE!][!//
[!CODE!][!//
    (uint8)(0UL)
[!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDNOCODE!][!//
};
[!ENDMACRO!][!//


[!MACRO "AdcStructureConfigMacro"!][!//
[!NOCODE!][!//
    [!VAR "VariantsNo" = "variant:size()"!]
    [!IF "var:defined('postBuildVariant')"!]
        [!VAR "PostBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!]
    [!ELSE!]
        [!VAR "PostBuildVariantNameUnderscore" = "string(null)"!]
    [!ENDIF!]

    [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
        [!VAR "AdcConfiguredPartitions" = "num:i(count(AdcGeneral/AdcEcucPartitionRef/*))"!]
    [!ELSE!][!//
        [!VAR "AdcConfiguredPartitions" = "num:i(1)"!][!//
    [!ENDIF!][!//
[!ENDNOCODE!][!//
[!FOR "Partition" = "0" TO "num:i($AdcConfiguredPartitions) - 1"!][!//
[!INDENT "0"!]
    [!IF "(IMPLEMENTATION_CONFIG_VARIANT !='VariantPostBuild' and ($VariantsNo <= 1))"!][!//
        /* VariantPreCompile/LinkTime and at most 1 configured variant */
        [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!][!//
            /* Existing ADC EcucPartition References */
            [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcGeneral/AdcEcucPartitionRef/*[@index = $Partition],'/'),'/'),'/'),'/')"!][!//
static const Adc_ConfigType Adc_Config_[!"$CurrentPartition"!] =
        [!ELSE!][!//
/* No ADC configured Ecuc Partitions */
const Adc_ConfigType Adc_Config =
        [!ENDIF!][!//
    [!ELSE!][!//
        [!IF "$VariantsNo > 0"!][!//
            /* VariantPostBuild or more than 1 configured variant */
            [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
                /* Existing ADC EcucPartition References */
                [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcGeneral/AdcEcucPartitionRef/*[@index = $Partition],'/'),'/'),'/'),'/')"!]
/**
* @brief          ADC driver Configuration structure [!"$postBuildVariant"!].
*/
const Adc_ConfigType Adc_Config[!"$PostBuildVariantNameUnderscore"!]_[!"$CurrentPartition"!] =
            [!ELSE!][!//
/* No ADC configured Ecuc Partitions */
/**
* @brief          ADC driver Configuration structure [!"$postBuildVariant"!].
*/
const Adc_ConfigType Adc_Config[!"$PostBuildVariantNameUnderscore"!] =
            [!ENDIF!][!//
        [!ELSE!][!//
            /* PB with no variants (VARIANT_NO := 0) or no ECUC */
            [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
                /* Existing ADC EcucPartition References */
                [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcGeneral/AdcEcucPartitionRef/*[@index = $Partition],'/'),'/'),'/'),'/')"!]
/**
* @brief          ADC driver Configuration structure for [!"$CurrentPartition"!].
*/
const Adc_ConfigType Adc_Config_[!"$CurrentPartition"!] =
            [!ELSE!][!//
/* No ADC configured Ecuc Partitions */
const Adc_ConfigType Adc_Config =
            [!ENDIF!][!//
        [!ENDIF!][!//
    [!ENDIF!][!//
    [!NOCODE!][!//
    [!VAR "MaxUnit"="num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit'))-1"!][!//
    [!ENDNOCODE!][!//
{
[!ENDINDENT!][!//
    &AdcIpwCfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!], /* AdcIpwConfigPtr */
    /** @brief Group configurations */
    Adc_GroupsCfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!], /* GroupsPtr */
    ADC_GROUPS[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"text:toupper($CurrentPartition)"!][!ENDIF!], /* GroupCount */
    /**< @brief Array for mapping the groups */
    Adc_au16GroupIdToIndexMap[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!], /* GroupIdToIndexMapPtr */
[!NOCODE!][!//
    [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
        [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
            [!IF "$CurrentPartition = substring-after(substring-after(substring-after(substring-after(node:value(./OsAppEcucPartitionRef),'/'),'/'),'/'),'/')"!][!//
                [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
[!CODE!][!//
    /** @brief Configuration CoreID */
    (uint32)([!"node:value(./EcucCoreId)"!]UL), /* CoreId */
[!ENDCODE!][!//
                [!ENDSELECT!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
    [!ELSE!][!//
[!CODE!][!//
    /** @brief Configuration CoreID */
    (uint32)(0UL), /* CoreId */
[!ENDCODE!]
    [!ENDIF!][!//
[!ENDNOCODE!][!//
[!IF "(IMPLEMENTATION_CONFIG_VARIANT !='VariantPostBuild' and (variant:size()<=1))"!][!//
    /**< @brief Array contains CoreId */
    Adc_u8AdcEcucPartition, /* AssignmentPartitionPtr */
    /**< @brief Number of partitions */
    (uint8)ADC_MAX_PARTITIONS_CFG /* AssignedPartitionCount */
[!ELSE!][!//
    /**< @brief Array contains CoreId */
    Adc_Partition_Assignment[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!], /* AssignmentPartitionPtr */
    /**< @brief Number of partitions */
    [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!](uint8)ADC_CFGSET[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PARTITION[!ELSE!](uint8)1UL[!ENDIF!] /* AssignedPartitionCount */
[!ENDIF!][!//
};

[!ENDFOR!][!//
[!ENDMACRO!][!//

[!MACRO "AdcGlobalPointerMacro"!][!//
[!NOCODE!]
    [!VAR "VariantsNo" = "variant:size()"!]
    [!IF "(IMPLEMENTATION_CONFIG_VARIANT !='VariantPostBuild' and ($VariantsNo <= 1))"!]
        [!VAR "MaxPartition" = "num:i(0)"!]
        [!IF "node:exists(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition)"!]
            [!VAR "MaxPartition" = "num:i(count(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition/*))"!]
        [!ENDIF!]
        [!IF "$MaxPartition = 0"!][!VAR "MaxPartition" = "num:i(1)"!][!ENDIF!][!//
[!CODE!][!//
const Adc_ConfigType * const Adc_ConfigVariantPredefined[ADC_MAX_PARTITIONS] =
{
[!ENDCODE!]
        [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
            [!FOR "index" = "0" TO "num:i($MaxPartition) - 1"!][!//
                [!VAR "PartitionExist"="0"!][!//
                [!LOOP "AdcGeneral/AdcEcucPartitionRef/*"!][!//
                    [!VAR "AdcCrtPart" = "node:value(.)"!][!//
                    [!VAR "AdcCrtPartShortName" = "substring-after(substring-after($AdcCrtPart, node:name(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1])), "/")"!][!//
                    [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
                        [!IF "$AdcCrtPart = node:value(./OsAppEcucPartitionRef)"!][!//
                            [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
                                [!IF "$index = node:value(./EcucCoreId)"!][!//
                                    [!VAR "CurrentPartition" = "$AdcCrtPartShortName"!][!//
                                    [!VAR "PartitionExist"="1"!][!//
                                [!ENDIF!][!//
                            [!ENDSELECT!][!//
                        [!ENDIF!][!//
                    [!ENDLOOP!][!//
                [!ENDLOOP!][!//
                [!IF "$PartitionExist = 1"!][!//
[!CODE!]    &Adc_Config_[!"$CurrentPartition"!][!IF "$index < ($MaxPartition - 1)"!],[!ELSE!][!ENDIF!]
[!ENDCODE!]
                [!ELSE!][!//
[!CODE!]    NULL_PTR[!IF "$index < ($MaxPartition - 1)"!],[!ELSE!][!ENDIF!]
[!ENDCODE!]
                [!ENDIF!][!//
            [!ENDFOR!][!//
        [!ELSE!][!//
[!CODE!]    &Adc_Config
[!ENDCODE!]
        [!ENDIF!]
[!CODE!]};[!ENDCODE!]
    [!ENDIF!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//



[!MACRO "AdcGroupDefineMacro"!][!//
[!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
    [!LOOP "AdcGeneral/AdcEcucPartitionRef/*"!][!//
        [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(.,'/'),'/'),'/'),'/')"!][!//
/**
* @brief          Total number of groups in partition [!"$CurrentPartition"!] .
*
*/
#define ADC_GROUPS[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_[!"text:toupper($CurrentPartition)"!]       ([!"num:i(count(../../../AdcConfigSet/AdcHwUnit/*[substring-after(substring-after(substring-after(substring-after(AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/') = $CurrentPartition]/AdcGroup/*))"!]U)

    [!ENDLOOP!][!//
[!ELSE!][!//
/**
* @brief          Total number of groups in Config.
*
*/
#define ADC_GROUPS[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]                       ([!"num:i(count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*))"!]U)
[!ENDIF!][!//
[!ENDMACRO!]



[!MACRO "AdcGroupAssignmentsMacro"!][!//
[!VAR "MaxUnit"="num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit'))-1"!][!//
[!FOR "Unit" = "0" TO "num:i($MaxUnit)"!][!//
[!FOR "x" = "0" TO "count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*)-1"!][!//
[!LOOP "AdcConfigSet/AdcHwUnit/*/AdcGroup/*"!][!//
[!IF "$x = AdcGroupId"!][!//
[!IF "../../AdcHwUnitId = concat('ADC',string($Unit))"!][!//
/**
* @brief          Group Assignments on ADC[!"$Unit"!][!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!].
*
*/
static const Adc_ChannelType Adc_Group[!"AdcGroupId"!]_Assignment_[!"$Unit"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][ADC_CFGSET[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_GROUP_[!"AdcGroupId"!]_CHANNELS] =
{
[!INDENT "4"!][!//
    [!VAR "MaxCount" = "count(AdcGroupDefinition/*)-1"!][!//
    [!VAR "CrtCount" = "0"!][!//
    [!LOOP "AdcGroupDefinition/*"!][!//
        [!VAR "CurrAdcChannel" = "."!][!//
        [!VAR "CurrAdcChannel1" = "substring-after(substring-after(substring-after(substring-after(substring-after($CurrAdcChannel,'/'),'/'),'/'),'/'),'/')"!][!//
        [!VAR "Idx" = "0"!][!//
        [!LOOP "../../../../AdcChannel/*"!][!//
            [!IF "@name = $CurrAdcChannel1"!][!//
                [!CODE!][!//
                [!"AdcLogicalChannelId"!]U[!IF "$CrtCount < $MaxCount"!],[!ELSE!][!ENDIF!]
                [!ENDCODE!][!//
            [!ENDIF!][!//
            [!VAR "Idx" = "$Idx + 1"!][!//
        [!ENDLOOP!][!//
        [!VAR "CrtCount" = "$CrtCount + 1"!][!//
    [!ENDLOOP!][!//
[!ENDINDENT!][!//
};

[!ENDIF!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDFOR!][!//
[!ENDFOR!][!//
[!ENDMACRO!][!//


[!MACRO "AdcSarIpChansConfigMacro"!][!//
[!NOCODE!][!// the logicalId is unique among all partitions, no need to generate following number of partitions
    [!VAR "MaxUnitCfg"="num:i(count(AdcConfigSet/AdcHwUnit/*)-1)"!][!//
    [!FOR "Unit" = "0" TO "num:i($MaxUnitCfg)"!][!//
        [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
            [!IF "(AdcLogicalUnitId = $Unit)"!][!//
                [!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!]
                    [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcHwUnitEcucPartitionRef/*[@index = "0"],'/'),'/'),'/'),'/')"!][!//
                [!ENDIF!][!//
[!CODE!][!//
/**
* @brief          ADC SAR Ip List of Channels Configuration for Logical ID [!"AdcLogicalUnitId"!] corresponding to the [!"AdcHwUnitId"!] configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!] [!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*/
static const Adc_Sar_Ip_ChanConfigType AdcSarIpChansConfig_[!"AdcLogicalUnitId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][[!"num:i(count(AdcChannel/*))"!]] =
{
[!ENDCODE!][!//
                [!VAR "MaxNum"="count(AdcChannel/*)"!]
                [!VAR "CrtNum"="0"!]
                [!FOR "CrtNum" = "0" TO "$MaxNum"!][!// This array should be generated order by Logical Channel Id (AdcLogicalChannelId) instead of @index
                    [!LOOP "AdcChannel/*"!]
                        [!IF "AdcLogicalChannelId = $CrtNum"!][!//
                            [!VAR "WdgThreshRegIndexString" = "'0'"!]
                            [!IF "AdcEnableThresholds = 'true' and node:exists(AdcThresholdRegister) and node:value(../../../../../AutosarExt/AdcEnableWatchdogApi)='true'"!]
                                [!VAR "WdgThreshRegIndexString" = "substring-after(node:ref(AdcThresholdRegister)/AdcThresholdControlRegister, 'ADC_THRESHOLD_REG_')"!]
                            [!ENDIF!]
[!CODE!][!//
    {
        [!"text:split(AdcChannelName, 'ChanNum')[last()]"!]U, /* ChanIndex */
        (boolean)[!"text:toupper(AdcEnablePresampling)"!], /* PresamplingEnable */
    #if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
        [!"$WdgThreshRegIndexString"!]U, /* WdgThreshRegIndex */
        (boolean)[!IF "AdcEnableThresholds = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* WdgNotificationEn */
    #endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */
    #if (STD_ON == ADC_SAR_IP_EOC_ENABLED)
        (boolean)FALSE, /* EndOfConvNotification */[!// always set to FALSE, because not used by ADC driver */
    #endif /* (STD_ON == ADC_SAR_IP_EOC_ENABLED) */
        (boolean)FALSE /* EndOfConvDmaEnable */[!// always set to FALSE, because not used by ADC driver */
[!IF "$CrtNum < $MaxNum - 1"!][!//
    },
[!ELSE!][!//
    }
[!ENDIF!][!//
[!ENDCODE!][!//
                        [!ENDIF!]
                    [!ENDLOOP!]
                [!ENDFOR!]
[!CODE!][!//
};

[!ENDCODE!]
            [!ENDIF!]
        [!ENDLOOP!]
    [!ENDFOR!]
[!ENDNOCODE!]
[!ENDMACRO!]



[!MACRO "AdcIpwNotificationMacro"!][!//
[!NOCODE!][!//
    [!LOOP "AdcConfigSet/AdcHwUnit/*"!]
        [!VAR "AdcPhysicalId" = "substring-after(AdcHwUnitId,'ADC')"!][!//
        [!IF "AdcTransferType = 'ADC_INTERRUPT'"!]
            [!LOOP "AdcGroup/*[AdcWithoutInterrupts = 'false']"!]
                [!IF "AdcGroupConversionType = 'ADC_CONV_TYPE_NORMAL'"!]
[!CODE!][!//
void Adc_Ipw_Adc[!"$AdcPhysicalId"!]EndNormalChainNotification(void);
[!ENDCODE!][!//
                    [!BREAK!]
                [!ENDIF!]
            [!ENDLOOP!]
            [!LOOP "AdcGroup/*[AdcWithoutInterrupts = 'false']"!]
                [!IF "AdcGroupConversionType = 'ADC_CONV_TYPE_INJECTED'"!]
[!CODE!][!//
void Adc_Ipw_Adc[!"$AdcPhysicalId"!]EndInjectedChainNotification(void);
[!ENDCODE!][!//
                    [!BREAK!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ENDIF!]
        [!LOOP "AdcChannel/*"!]
            [!IF "AdcEnableThresholds = 'true'"!]
[!CODE!][!//
void Adc_Ipw_Adc[!"$AdcPhysicalId"!]WdgThresholdNotification(const uint16 PhysicalChanId, const uint8 Flags);
[!ENDCODE!][!//
                [!BREAK!]
            [!ENDIF!]
        [!ENDLOOP!]
    [!ENDLOOP!]
[!ENDNOCODE!][!//
[!ENDMACRO!]




[!MACRO "AdcIpwAltClockConfigMacro"!][!//
[!NOCODE!][!//
    [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
        [!VAR "MaxPartitionCfg"="num:i(count(AdcGeneral/AdcEcucPartitionRef/*))"!][!//
    [!ELSE!][!//
        [!VAR "MaxPartitionCfg"="1"!][!//
    [!ENDIF!][!//
    [!VAR "CrtCount" = "0"!][!//
    [!VAR "MaxCount" = "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan'))"!][!//
    [!IF "((AutosarExt/AdcEnableDualClockMode) = 'true')"!][!//
[!CODE!][!//
#if (ADC_DUAL_CLOCK_MODE == STD_ON)
[!ENDCODE!][!//
    [!FOR "Partition" = "0" TO "num:i($MaxPartitionCfg) - 1"!][!//
        [!VAR "MaxHWUnitInPartition"="0"!][!//
        [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
            [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcGeneral/AdcEcucPartitionRef/*[@index = $Partition],'/'),'/'),'/'),'/')"!][!//
            [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
                [!IF "substring-after(substring-after(substring-after(substring-after(AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/') = $CurrentPartition"!]
                    [!VAR "MaxHWUnitInPartition"="$MaxHWUnitInPartition + 1"!][!//
            [!ENDIF!][!//
            [!ENDLOOP!][!//
        [!ELSE!][!//
            [!VAR "MaxHWUnitInPartition"="num:i(count(AdcConfigSet/AdcHwUnit/*))"!][!//
        [!ENDIF!][!//
        [!VAR "MaxUnitCfg"="num:i(count(AdcConfigSet/AdcHwUnit/*)-1)"!][!//
        [!FOR "Unit" = "0" TO "num:i($MaxUnitCfg)"!][!//
            [!VAR "LoopHWUnitPartition"="0"!][!//
            [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
                [!VAR "HWUnitPartition"="0"!][!//
                [!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!]
                    [!IF "substring-after(substring-after(substring-after(substring-after(AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/') = $CurrentPartition"!][!//
                        [!VAR "HWUnitPartition"="1"!][!//
                    [!ENDIF!][!//
                [!ELSE!][!//
                    [!VAR "HWUnitPartition"="2"!][!//
                [!ENDIF!][!//
                [!IF "(AdcLogicalUnitId = $Unit) and ($HWUnitPartition > 0)"!][!//
                    [!VAR "AdcHwAvgEnableAltString" = "'FALSE'"!]
                    [!VAR "AdcHwAvgSelectAltString" = "'ADC_SAR_IP_AVG_4_CONV'"!]
                    [!VAR "SampleTimeString" = "'{ '"!]
                    [!VAR "SampleTimeString0" = "'ADC_SAR_IP_DEF_SAMPLE_TIME'"!]
                    [!VAR "SampleTimeString1" = "'ADC_SAR_IP_DEF_SAMPLE_TIME'"!]
                    [!VAR "SampleTimeString2" = "'ADC_SAR_IP_DEF_SAMPLE_TIME'"!]

                    [!VAR "ClkSelectString" = "'ADC_SAR_IP_CLK_FULL_BUS'"!]
                    [!IF "node:exists(AdcAltPrescale)"!]
                        [!IF "num:i(AdcAltPrescale) = 2"!]
                            [!VAR "ClkSelectString" = "'ADC_SAR_IP_CLK_HALF_BUS'"!]
                        [!ELSEIF "num:i(AdcAltPrescale) = 4"!]
                            [!VAR "ClkSelectString" = "'ADC_SAR_IP_CLK_QUARTER_BUS'"!]
                        [!ENDIF!]
                    [!ENDIF!]
                    [!VAR "AdcHighSpeedEnableString" = "'FALSE'"!]
                    [!IF "node:exists(AdcAltHighSpeedEnable)"!]
                        [!VAR "AdcHighSpeedEnableString" = "text:toupper(AdcAltHighSpeedEnable)"!]
                    [!ENDIF!]

                    [!IF "(node:exists(AdcAlternateConvTimings) and (../../../AutosarExt/AdcConvTimeOnce = 'true'))"!][!//
                        [!IF "node:exists(AdcAlternateConvTimings/AdcHardwareAverageEnableAlternate)"!]
                            [!VAR "AdcHwAvgEnableAltString" = "text:toupper(AdcAlternateConvTimings/AdcHardwareAverageEnableAlternate)"!]
                        [!ENDIF!]
                        [!IF "node:exists(AdcAlternateConvTimings/AdcHardwareAverageSelectAlternate)"!]
                            [!VAR "AdcHwAvgSelectAltString" = "concat('ADC_SAR_IP_AVG_', substring-after(AdcAlternateConvTimings/AdcHardwareAverageSelectAlternate, 'SAMPLES_'),'_CONV')"!]
                        [!ENDIF!]

                        [!VAR "SampleTimeString0" = "concat(node:value(AdcAlternateConvTimings/AdcSamplingDurationAlt0), 'U')"!]
                        [!IF "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) > 1"!][!//
                            [!VAR "SampleTimeString1" = "concat(node:value(AdcAlternateConvTimings/AdcSamplingDurationAlt1), 'U' )"!]
                        [!ENDIF!]
                        [!IF "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) > 2"!][!//
                            [!VAR "SampleTimeString2" = "concat(node:value(AdcAlternateConvTimings/AdcSamplingDurationAlt2), 'U' )"!]
                        [!ENDIF!]
                    [!ENDIF!][!//

                    [!VAR "SampleTimeString" = "concat($SampleTimeString, $SampleTimeString0)"!]
                    [!IF "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) > 1"!][!//
                        [!VAR "SampleTimeString" = "concat($SampleTimeString, ', ', $SampleTimeString1)"!]
                    [!ENDIF!]
                    [!IF "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) > 2"!][!//
                        [!VAR "SampleTimeString" = "concat($SampleTimeString, ', ', $SampleTimeString2)"!]
                    [!ENDIF!]
                    [!VAR "SampleTimeString" = "concat($SampleTimeString, ' }' )"!]
[!CODE!][!//
/**
* @brief          Definition of Alternate Clock Config for Logical ID [!"AdcLogicalUnitId"!] corresponding to the [!"AdcHwUnitId"!] configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!] [!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*/
static const Adc_Sar_Ip_ClockConfigType AdcIpwAltClockConfig_[!"AdcLogicalUnitId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!] =
{
    [!"$ClkSelectString"!], /* ClkSelect */
#if FEATURE_ADC_HAS_HIGH_SPEED_ENABLE
    (boolean)[!"$AdcHighSpeedEnableString"!], /* HighSpeedConvEn */
#endif /* FEATURE_ADC_HAS_HIGH_SPEED_ENABLE */
#if FEATURE_ADC_HAS_CONVERSION_TIMING
    [!"$SampleTimeString"!], /* SampleTimeArr */
#endif /* FEATURE_ADC_HAS_CONVERSION_TIMING */
    [!"num:i(AdcAltPowerDownDelay)"!]U, /* PowerDownDelay */
#if FEATURE_ADC_HAS_AVERAGING
    (boolean)[!"$AdcHwAvgEnableAltString"!], /* AvgEn */
    [!"$AdcHwAvgSelectAltString"!] /* AvgSel */
#endif /* FEATURE_ADC_HAS_AVERAGING */
};

[!ENDCODE!][!//
                [!ENDIF!][!//
            [!ENDLOOP!][!//
        [!ENDFOR!]
    [!ENDFOR!]
[!CODE!][!//
#endif /* (ADC_DUAL_CLOCK_MODE == STD_ON) */
[!ENDCODE!][!//
    [!ENDIF!][!// AdcEnableDualClockMode = 'true'
[!ENDNOCODE!]
[!ENDMACRO!]


[!MACRO "AdcIpwChannelConfigMacro"!][!//
[!NOCODE!][!//
    [!FOR "Unit" = "0" TO "num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit')) - 1"!][!// START FOR 1
        [!VAR "HwUnitExist" = "0"!][!// To check which Hardware Unit will be generated.
        [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!// LOOP 1
            [!IF "AdcLogicalUnitId = $Unit"!][!//
                [!VAR "HwUnitExist" = "$HwUnitExist+1"!][!//
                [!VAR "AdcHwUnit" = "concat('AdcConfigSet/AdcHwUnit/',@name)"!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!// END LOOP 1

        [!IF "$HwUnitExist = 1"!][!// If the unit exist, its configuration will be generated.
            [!VAR "LoopVar"="0"!][!//
            [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
                [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcConfigSet/AdcHwUnit/*[AdcLogicalUnitId = $Unit]/AdcHwUnitEcucPartitionRef/*[@index = "0"],'/'),'/'),'/'),'/')"!][!//
            [!ENDIF!]
[!CODE!][!//
/**
* @brief          Definition of Adc Ipw Channel for Logical Unit Id [!"node:ref(concat($AdcHwUnit,'/AdcLogicalUnitId'))"!] configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*/
static const Adc_Ipw_ChannelConfig AdcIpwChannelConfig_[!"node:ref(concat($AdcHwUnit,'/AdcLogicalUnitId'))"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][] =
{
[!ENDCODE!][!//
            [!VAR "numChannel" = "num:i(count(AdcConfigSet/AdcHwUnit/*[AdcLogicalUnitId = $Unit]/AdcChannel/*)-1)"!][!//
            [!FOR "x" = "0" TO "$numChannel"!][!// START FOR 2
                [!LOOP "AdcConfigSet/AdcHwUnit/*[AdcLogicalUnitId = $Unit]/AdcChannel/*"!][!// LOOP 2
                    [!VAR "Loop"="num:i($LoopVar)"!][!//
                    [!IF "AdcLogicalChannelId = $Loop"!][!//
                        [!VAR "WdgNotificationString" = "'NULL_PTR'"!]
                        [!VAR "LogicalThresholdIdString" = "'0'"!]
                        [!VAR "ChannelRangeString" = "'ADC_RANGE_ALWAYS'"!]
                        [!VAR "ChannelHighLimitString" = "'4095'"!]
                        [!VAR "ChannelLowLimitString" = "'0'"!]
                        [!VAR "ChannelLimitEnableString" = "'FALSE'"!]
                        [!IF "AdcEnableThresholds = 'true'"!][!//
                            [!IF "AdcEnableThresholds = 'true'"!][!//
                                [!VAR "WdgNotificationString" = "AdcWdogNotification"!]
                                [!VAR "LogicalThresholdIdString" = "num:i(node:ref(AdcThresholdRegister)/@index)"!]
                            [!ENDIF!]
                        [!ENDIF!]
                        [!IF "node:exists(AdcChannelLimitCheck) and AdcChannelLimitCheck ='true'"!][!//
                            [!VAR "ChannelLimitEnableString" = "'TRUE'"!]
                            [!IF "node:exists(AdcChannelRangeSelect)"!][!//
                                [!VAR "ChannelRangeString" = "AdcChannelRangeSelect"!]
                            [!ENDIF!][!//
                            [!IF "node:exists(AdcChannelHighLimit)"!][!//
                                [!VAR "ChannelHighLimitString" = "AdcChannelHighLimit"!]
                            [!ENDIF!][!//
                            [!IF "node:exists(AdcChannelLowLimit)"!][!//
                                [!VAR "ChannelLowLimitString" = "AdcChannelLowLimit"!]
                            [!ENDIF!][!//
                        [!ENDIF!][!//
[!CODE!][!//
    /**< @brief Channel logical Id [!"AdcLogicalChannelId"!] */
    {
        [!"$WdgNotificationString"!], /* WdgNotification */
        [!"$LogicalThresholdIdString"!]U, /* LogicalThresholdId */
#if (ADC_ENABLE_LIMIT_CHECK == STD_ON)
        /* ChannelLimitCheckingConfig */
        {
            /**< @brief limit check enabled */
            (boolean)[!"$ChannelLimitEnableString"!], /* ChannelLimitCheckEnabled */
            /**< @brief range for limit check */
            [!"$ChannelRangeString"!], /* ChannelRange */
            /**< @brief high limit value */
            [!"$ChannelHighLimitString"!]U, /* ChannelHighLimit */
            /**< @brief low limit value */
            [!"$ChannelLowLimitString"!]U /* ChannelLowLimit */
        }
#endif /* (ADC_ENABLE_LIMIT_CHECK == STD_ON) */
[!IF "$x < $numChannel"!][!//
    },[!ELSE!][!//
    }[!ENDIF!]
[!ENDCODE!][!//
                    [!ENDIF!][!//
                [!ENDLOOP!][!// END LOOP 2
                [!VAR "LoopVar"="$LoopVar + 1"!][!//
            [!ENDFOR!][!// END FOR 2
[!CODE!][!//
};

[!ENDCODE!][!//
        [!ENDIF!][!//
    [!ENDFOR!][!// END FOR 1
[!ENDNOCODE!]
[!ENDMACRO!]



[!MACRO "AdcIpwConfigMacro" , "MacroGenerationType"!][!// GenerationType='definition' or 'declaration'
[!NOCODE!][!//
    [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
        [!VAR "MaxPartitionCfg"="num:i(count(AdcGeneral/AdcEcucPartitionRef/*))"!][!//
    [!ELSE!][!//
        [!VAR "MaxPartitionCfg"="1"!][!//
    [!ENDIF!][!//

    [!FOR "Partition" = "0" TO "num:i($MaxPartitionCfg) - 1"!][!// Start of For Loop 1
        [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcGeneral/AdcEcucPartitionRef/*[@index = $Partition],'/'),'/'),'/'),'/')"!][!//
        [!VAR "CtuListStartIndexTemp" = "0"!][!//
        [!VAR "CtuListMaxNumElemsTemp" = "0"!][!//
        [!VAR "MaxCfgNumTrigModeHWUnit"="0"!][!// Total number of HW units have BCTU trigger mode groups (all partitions)
        [!VAR "MinTrigModePhyHWUnitId"="num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit')) - 1"!][!// HW unit has smallest physical ID (all partitions)
        [!VAR "TriggerModeEnabledInPartition"="0"!][!//
        [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
            [!IF "count(AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]) > 0 "!][!//
                [!VAR "MaxCfgNumTrigModeHWUnit"="$MaxCfgNumTrigModeHWUnit + 1"!][!//
                [!IF "num:i(substring-after(AdcHwUnitId, 'ADC')) < $MinTrigModePhyHWUnitId "!][!//
                    [!VAR "MinTrigModePhyHWUnitId" = "num:i(substring-after(AdcHwUnitId, 'ADC'))"!][!//
                [!ENDIF!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
        [!IF "$MacroGenerationType = 'declaration'"!]
[!CODE!][!//
/**
* @brief          Adc Ipw Config configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*/
extern const Adc_Ipw_Config AdcIpwCfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!];

[!ENDCODE!][!//
        [!ELSE!]
[!CODE!][!//
/**
* @brief          Adc Ipw Config configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*/
const Adc_Ipw_Config AdcIpwCfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!] =
{
[!ENDCODE!][!//
            [!VAR "AdcConfigArrayString" = "'{ '"!][!//
            [!VAR "AdcPhysicalChannelIdArrayString" = "'{ '"!][!//
            [!VAR "AdcAltClockConfigArrayString" = "'{ '"!][!//
            [!VAR "AdcIpwChannelConfigArrayString" = "'{ '"!][!//
            [!VAR "AdcUnitSupportCtuControlModeArrayString" = "'{ '"!][!//
            [!VAR "TransferTypeArrayString" = "'{ '"!][!//
            [!VAR "GroupCountArrayString" = "'{ '"!][!//
            [!VAR "ChannelCountArrayString" = "'{ '"!][!//
            [!VAR "ChannelLogicalIdArrayString0" = "'{ 0U }, /* Unit not used */'"!][!//
            [!IF "(2 < num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit')))"!]
                [!VAR "ChannelLogicalIdArrayString1" = "'{ 0U }, /* Unit not used */'"!][!//
                [!VAR "ChannelLogicalIdArrayString2" = "'{ 0U } /* Unit not used */'"!][!//
            [!ELSE!]
                [!VAR "ChannelLogicalIdArrayString1" = "'{ 0U } /* Unit not used */'"!][!//
            [!ENDIF!]
            [!VAR "DmaChannelArrayString" = "'{ '"!][!//
            [!VAR "DmaNumSgaElementArrayString" = "'{ '"!][!//
            [!VAR "CountingDmaChannelArrayString" = "'{ '"!][!//
            [!VAR "HwUnitEnableArrayString" = "'{ '"!][!//
            [!VAR "HwLogicalIdArrayString" = "'{ '"!][!//
            [!VAR "CtuMaxNumElemsInList"="num:i(ecu:get('BCTUNumberOfCmdListChan'))"!][!//
            [!VAR "CtuListStartIndexArrayString" = "'{ '"!][!//
            [!VAR "CtuListMaxNumElemsArrayString" = "'{ '"!][!//
            [!FOR "Unit" = "0" TO "num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit')) - 1"!][!// Start of For Loop 2
                [!IF "($Unit < num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit')) - 1)"!][!//
                    [!VAR "Comma" = "', '"!]
                [!ELSE!]
                    [!VAR "Comma" = "' '"!]
                [!ENDIF!]
                [!VAR "AdcHwUnit" = "'____________'"!][!//
                [!LOOP "AdcConfigSet/AdcHwUnit/*[AdcLogicalUnitId = $Unit]"!][!//
                    [!VAR "AdcHwUnit" = "node:path(.)"!][!//
                [!ENDLOOP!][!//
                [!VAR "AdcMaxHwCfg"="0"!][!//
                [!IF "node:exists($AdcHwUnit)"!]
                    [!VAR "HWUnitPartition"="0"!][!// Indicate the Multicore but the current Unit did not assign to current partition.
                    [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
                        [!IF "substring-after(substring-after(substring-after(substring-after(node:value(concat($AdcHwUnit,'/AdcHwUnitEcucPartitionRef/*[1]')),'/'),'/'),'/'),'/') = $CurrentPartition"!][!//
                            [!VAR "HWUnitPartition"="1"!][!// Indicate the Multicore.
                        [!ENDIF!][!//
                    [!ELSE!][!//
                        [!VAR "HWUnitPartition"="2"!][!// Indicate the Single Core.
                    [!ENDIF!][!//
                    [!VAR "AdcLogicalUnitId"="node:value(concat($AdcHwUnit,'/AdcLogicalUnitId'))"!][!//
                    [!VAR "AdcPhysicalId" = "substring-after(node:value(concat($AdcHwUnit,'/AdcHwUnitId')),'ADC')"!][!//
                    [!IF "($HWUnitPartition > 0)"!][!//
                        [!IF "(node:value(concat($AdcHwUnit,'/AdcTransferType')) = 'ADC_DMA') and node:exists(concat($AdcHwUnit,'/AdcDmaChannelId'))"!]
                            [!VAR "DmaChannelArrayString" = "concat($DmaChannelArrayString, as:ref(node:ref(concat($AdcHwUnit,'/AdcDmaChannelId')))/dmaLogicChannel_LogicName, $Comma)"!][!// Get Dma Channel Id from Mcl
                            [!IF "as:ref(node:ref(concat($AdcHwUnit,'/AdcDmaChannelId')))/dmaLogicChannel_EnableScatterGather"!][!//
                                [!VAR "DmaNumSgaElementArrayString" = "concat($DmaNumSgaElementArrayString, as:ref(node:ref(concat($AdcHwUnit,'/AdcDmaChannelId')))/dmaLogicChannel_LogicName, '_NOF_CFG_SGA_ELEMENTS',$Comma)"!][!// Get Dma Channel Id from Mcl
                            [!ELSE!][!//
                                [!VAR "DmaNumSgaElementArrayString" = "concat($DmaNumSgaElementArrayString, '0U', $Comma)"!][!// Tempo value because Scatter/Gather is disabled
                            [!ENDIF!][!//
                            [!VAR "AdcCountingDmaChannelEnabled" = "'FALSE'"!][!//
                            [!LOOP "AdcConfigSet/AdcHwUnit/*[AdcLogicalUnitId = $Unit]"!][!//
                                [!IF "(num:i(count(./AdcGroup/*[(((AdcEnableOptimizeDmaStreamingGroups = 'true') or ((AdcWithoutInterrupts = 'true') and (AdcGroupAccessMode = 'ADC_ACCESS_MODE_STREAMING') and (AdcStreamResultGroup = 'true'))) and (count(AdcGroupDefinition/*) > 1)) or ((AdcWithoutInterrupts = 'true') and (AdcGroupAccessMode = 'ADC_ACCESS_MODE_STREAMING') and (count(AdcGroupDefinition/*) = 1))])) > 0)"!]
                                    [!IF "node:exists(concat($AdcHwUnit,'/AdcCountingDmaChannelId'))"!][!//
                                        [!VAR "AdcCountingDmaChannelEnabled" = "'TRUE'"!][!//
                                    [!ENDIF!]
                                [!ENDIF!]
                            [!ENDLOOP!][!//
                            [!IF "($AdcCountingDmaChannelEnabled = 'TRUE') and not(node:empty(concat($AdcHwUnit,'/AdcCountingDmaChannelId')))"!]
                                [!VAR "CountingDmaChannelArrayString" = "concat($CountingDmaChannelArrayString, as:ref(node:ref(concat($AdcHwUnit,'/AdcCountingDmaChannelId')))/dmaLogicChannel_LogicName, $Comma)"!][!// Get Dma Channel Id from Mcl
                            [!ELSE!]
                                [!VAR "CountingDmaChannelArrayString" = "concat($CountingDmaChannelArrayString, 'ADC_IPW_INVALID_DMA_CHANNEL_ID', $Comma)"!][!// INVALID DMA Channel ID
                            [!ENDIF!]
                        [!ELSE!]
                            [!VAR "DmaChannelArrayString" = "concat($DmaChannelArrayString, 'ADC_IPW_INVALID_DMA_CHANNEL_ID', $Comma)"!][!// INVALID DMA Channel ID
                            [!VAR "DmaNumSgaElementArrayString" = "concat($DmaNumSgaElementArrayString, '0U', $Comma)"!][!// INVALID DMA Channel ID
                            [!VAR "CountingDmaChannelArrayString" = "concat($CountingDmaChannelArrayString, 'ADC_IPW_INVALID_DMA_CHANNEL_ID', $Comma)"!][!// INVALID DMA Channel ID
                        [!ENDIF!]
                        [!VAR "AdcConfigArraySubString"!]&AdcSarIpConfig_[!"$AdcLogicalUnitId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][!ENDVAR!]
                        [!VAR "AdcConfigArrayString" = "concat($AdcConfigArrayString, $AdcConfigArraySubString, $Comma)"!]
                        [!IF "(AutosarExt/AdcEnableDualClockMode = 'true')"!][!//
                            [!VAR "AdcAltClockConfigArraySubString"!]&AdcIpwAltClockConfig_[!"$AdcLogicalUnitId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][!ENDVAR!]
                        [!ELSE!][!//
                            [!VAR "AdcAltClockConfigArraySubString" = "'NULL_PTR'"!]
                        [!ENDIF!][!//
                        [!VAR "AdcAltClockConfigArrayString" = "concat($AdcAltClockConfigArrayString, $AdcAltClockConfigArraySubString, $Comma)"!]
                        [!VAR "AdcIpwChannelConfigArraySubString"!]AdcIpwChannelConfig_[!"$AdcLogicalUnitId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][!ENDVAR!]
                        [!VAR "AdcIpwChannelConfigArrayString" = "concat($AdcIpwChannelConfigArrayString, $AdcIpwChannelConfigArraySubString, $Comma)"!]
                        [!VAR "AdcPhysicalChannelIdArrayString" = "concat($AdcPhysicalChannelIdArrayString, text:replace(node:value(concat($AdcHwUnit,'/AdcHwUnitId')),'ADC',''), 'U', $Comma)"!]
                        [!VAR "TransferTypeArrayString" = "concat($TransferTypeArrayString, node:ref(concat($AdcHwUnit,'/AdcTransferType')), $Comma)"!]
                        [!VAR "GroupCountArrayString" = "concat($GroupCountArrayString, num:i(count(node:ref(concat($AdcHwUnit,'/AdcGroup'))/*)), 'U', $Comma)"!]
                        [!VAR "ChannelCountArrayString" = "concat($ChannelCountArrayString, num:i(count(node:ref(concat($AdcHwUnit,'/AdcChannel'))/*)), 'U', $Comma)"!]
                        [!VAR "ChannelLogicalIdArrayString" = "'{ '"!][!//
                        [!FOR "PhysicalChannelIndex" = "0" TO "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.MaxPhysicalChanIdx'))"!][!// Start of For Loop 3
                            [!VAR "LogicalChannelIndex" = "0"!][!//
                            [!LOOP "node:ref(concat($AdcHwUnit,'/AdcChannel'))/*"!][!//
                                [!IF "num:i(text:split(AdcChannelName, 'ChanNum')[last()]) = $PhysicalChannelIndex"!][!//
                                    [!VAR "LogicalChannelIndex" = "num:i(AdcLogicalChannelId)"!][!//
                                    [!BREAK!][!//
                                [!ENDIF!][!//
                            [!ENDLOOP!][!//
                            [!IF "($PhysicalChannelIndex < num:i(ecu:get('Adc.AdcConfigSet.AdcSar.MaxPhysicalChanIdx')))"!][!//
                                [!VAR "ChannelLogicalIdArrayString" = "concat($ChannelLogicalIdArrayString, num:i($LogicalChannelIndex),'U, ')"!][!//
                            [!ELSE!]
                                [!VAR "ChannelLogicalIdArrayString" = "concat($ChannelLogicalIdArrayString, num:i($LogicalChannelIndex),'U ')"!][!//
                            [!ENDIF!]
                        [!ENDFOR!][!// End of For Loop 3
                        [!VAR "CommaWithoutSpace" = "''"!]
                        [!IF "($Unit < num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit')) - 1)"!]
                            [!VAR "CommaWithoutSpace" = "','"!]
                        [!ENDIF!]
                        [!VAR "ChannelLogicalIdArrayString" = "concat($ChannelLogicalIdArrayString, '}', $CommaWithoutSpace)"!][!//
                        [!IF "($Unit = 0)"!][!//
                            [!VAR "ChannelLogicalIdArrayString0" = "$ChannelLogicalIdArrayString"!][!//
                        [!ELSEIF "($Unit = 1)"!][!//
                            [!VAR "ChannelLogicalIdArrayString1" = "$ChannelLogicalIdArrayString"!][!//
                        [!ELSEIF "($Unit = 2)"!][!//
                            [!VAR "ChannelLogicalIdArrayString2" = "$ChannelLogicalIdArrayString"!][!//
                        [!ENDIF!][!//
                        [!VAR "HwUnitEnableArrayString" = "concat($HwUnitEnableArrayString, 'STD_ON', $Comma)"!][!//
                        [!VAR "NumChanOfAllCtuTrigGroup" = "0"!][!//
                        [!IF "count(AdcConfigSet/AdcHwUnit/*[AdcLogicalUnitId = num:i($AdcLogicalUnitId)]/AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]) > 0"!]
                            [!VAR "CtuListStartIndexTemp" = "$CtuListStartIndexTemp + $CtuListMaxNumElemsTemp"!][!// Next start index = previous start index + previous max number elements
                            [!IF "(num:i($CtuMaxNumElemsInList mod $MaxCfgNumTrigModeHWUnit)!=0) and (num:i(substring-after(node:value(concat($AdcHwUnit,'/AdcHwUnitId')), 'ADC')))=num:i($MinTrigModePhyHWUnitId)"!][!//
                                [!VAR "CtuListMaxNumElemsTemp" = "num:i($CtuMaxNumElemsInList div $MaxCfgNumTrigModeHWUnit) + num:i($CtuMaxNumElemsInList mod $MaxCfgNumTrigModeHWUnit)"!][!//
                            [!ELSE!][!//
                                [!VAR "CtuListMaxNumElemsTemp" = "num:i($CtuMaxNumElemsInList div $MaxCfgNumTrigModeHWUnit)"!][!//
                            [!ENDIF!][!//
                            [!VAR "CtuListStartIndexArrayString" = "concat($CtuListStartIndexArrayString, num:i($CtuListStartIndexTemp), 'U', $Comma)"!][!//
                            [!VAR "CtuListMaxNumElemsArrayString" = "concat($CtuListMaxNumElemsArrayString, num:i($CtuListMaxNumElemsTemp), 'U', $Comma)"!][!//
                        [!ELSE!][!//
                            [!VAR "CtuListStartIndexArrayString" = "concat($CtuListStartIndexArrayString, '0U', $Comma)"!][!//
                            [!VAR "CtuListMaxNumElemsArrayString" = "concat($CtuListMaxNumElemsArrayString, '0U', $Comma)"!][!//
                        [!ENDIF!][!//

                        [!IF "count(AdcConfigSet/AdcHwUnit/*/AdcLogicalUnitId[.=$Unit]/../AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]) > 0"!]
                            [!VAR "TriggerModeEnabledInPartition"="1"!][!//
                        [!ENDIF!]
                    [!ENDIF!][!//
                    [!VAR "AdcUnitSupportCtuControlModeArrayString" = "concat($AdcUnitSupportCtuControlModeArrayString, '1U', $Comma)"!][!//
                [!ENDIF!][!//
                [!IF "(node:exists($AdcHwUnit) != 'true') or (num:i($HWUnitPartition) = num:i(0))"!] [!// If Unit is not configured OR does not belong to current partition
                    [!VAR "AdcConfigArrayString" = "concat($AdcConfigArrayString, 'NULL_PTR', $Comma)"!][!//
                    [!VAR "AdcPhysicalChannelIdArrayString" = "concat($AdcPhysicalChannelIdArrayString, 'ADC_IPW_INVALID_LOGICAL_UNIT_ID', $Comma)"!][!//
                    [!VAR "AdcAltClockConfigArrayString" = "concat($AdcAltClockConfigArrayString, 'NULL_PTR', $Comma)"!][!//
                    [!VAR "AdcIpwChannelConfigArrayString" = "concat($AdcIpwChannelConfigArrayString, 'NULL_PTR', $Comma)"!][!//
                    [!VAR "TransferTypeArrayString" = "concat($TransferTypeArrayString, 'ADC_IPW_INVALID_TRANSFER_TYPE /* Unit not used */', $Comma)"!][!//
                    [!VAR "GroupCountArrayString" = "concat($GroupCountArrayString, '0U', $Comma)"!][!//
                    [!VAR "ChannelCountArrayString" = "concat($ChannelCountArrayString, '0U', $Comma)"!][!//
                    [!VAR "HwUnitEnableArrayString" = "concat($HwUnitEnableArrayString, 'STD_OFF', $Comma)"!][!//
                    [!VAR "CtuListStartIndexArrayString" = "concat($CtuListStartIndexArrayString, '0U', $Comma)"!][!//
                    [!VAR "CtuListMaxNumElemsArrayString" = "concat($CtuListMaxNumElemsArrayString, '0U', $Comma)"!][!//
                    [!VAR "DmaChannelArrayString" = "concat($DmaChannelArrayString, 'ADC_IPW_INVALID_DMA_CHANNEL_ID', $Comma)"!][!// INVALID DMA Channel ID
                    [!VAR "DmaNumSgaElementArrayString" = "concat($DmaNumSgaElementArrayString, '0U', $Comma)"!][!// INVALID DMA Channel ID
                    [!VAR "CountingDmaChannelArrayString" = "concat($CountingDmaChannelArrayString, 'ADC_IPW_INVALID_DMA_CHANNEL_ID', $Comma)"!][!// INVALID DMA Channel ID
                [!ENDIF!][!//
                [!IF "(node:exists($AdcHwUnit) != 'true')"!] [!// If Unit is not configured
                    [!VAR "AdcUnitSupportCtuControlModeArrayString" = "concat($AdcUnitSupportCtuControlModeArrayString, '0U', $Comma)"!][!//
                [!ENDIF!][!//
                [!// This loop will generate the mapping between Physical Id and Logical Id, ordered by AdcHwUnitId.
                [!// For example: Max Unit is 3. ADC0 = Logical 1 and ADC2 = Logical 0 => HwLogicalId = {1, INVALID, 0}
                [!// So in driver, we can get Logical from Physical like that: LogicalAdcUnit = Adc_apxCfgPtr[CoreId]->AdcIpwConfigPtr->Mapping.HwLogicalId[PhysicalHwUnitId]
                [!VAR "bFoundUnit"="0"!][!// '0' means curent Physical unit is unused.
                [!LOOP "AdcConfigSet/AdcHwUnit/*[AdcHwUnitId = concat('ADC',$Unit)]"!][!//
                    [!VAR "HWUnitPartition"="0"!][!// '0' means Multicore was enabled but the current Unit did not assign to current partition.
                    [!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!]
                        [!IF "substring-after(substring-after(substring-after(substring-after(AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/') = $CurrentPartition"!][!//
                            [!VAR "HWUnitPartition"="1"!][!// '1' means Multicore was enabled but the current Unit is assigned to current partition.
                        [!ENDIF!][!//
                    [!ELSE!][!//
                        [!VAR "HWUnitPartition"="2"!][!//[!// '2' means Multicore was disabled.
                    [!ENDIF!][!//
                    [!IF "($HWUnitPartition > 0)"!][!//
                        [!VAR "HwLogicalIdArrayString" = "concat($HwLogicalIdArrayString, node:value(AdcLogicalUnitId) , 'U', $Comma)"!][!//
                    [!ELSE!]
                        [!VAR "HwLogicalIdArrayString" = "concat($HwLogicalIdArrayString, 'ADC_IPW_INVALID_LOGICAL_UNIT_ID', $Comma)"!][!//
                    [!ENDIF!]
                    [!VAR "bFoundUnit"="1"!][!// '1' means curent Physical unit is used in configuration.
                [!ENDLOOP!]
                [!IF "($bFoundUnit = 0)"!][!//
                    [!VAR "HwLogicalIdArrayString" = "concat($HwLogicalIdArrayString, 'ADC_IPW_INVALID_LOGICAL_UNIT_ID', $Comma)"!][!//
                [!ENDIF!]
            [!ENDFOR!][!// End of For Loop 2
            [!VAR "AdcConfigArrayString" = "concat($AdcConfigArrayString, '}')"!][!//
            [!VAR "AdcPhysicalChannelIdArrayString" = "concat($AdcPhysicalChannelIdArrayString, '}')"!][!//
            [!VAR "AdcAltClockConfigArrayString" = "concat($AdcAltClockConfigArrayString, '}')"!][!//
            [!VAR "AdcIpwChannelConfigArrayString" = "concat($AdcIpwChannelConfigArrayString, '}')"!][!//
            [!VAR "AdcUnitSupportCtuControlModeArrayString" = "concat($AdcUnitSupportCtuControlModeArrayString, '}')"!][!//

            [!VAR "TransferTypeArrayString" = "concat($TransferTypeArrayString, '}')"!][!//
            [!VAR "GroupCountArrayString" = "concat($GroupCountArrayString, '}')"!][!//
            [!VAR "ChannelCountArrayString" = "concat($ChannelCountArrayString, '}')"!][!//
            [!VAR "DmaChannelArrayString" = "concat($DmaChannelArrayString, '}')"!][!//
            [!VAR "DmaNumSgaElementArrayString" = "concat($DmaNumSgaElementArrayString, '}')"!][!//
            [!VAR "CountingDmaChannelArrayString" = "concat($CountingDmaChannelArrayString, '}')"!][!//
            [!VAR "HwUnitEnableArrayString" = "concat($HwUnitEnableArrayString, '}')"!][!//
            [!VAR "HwLogicalIdArrayString" = "concat($HwLogicalIdArrayString, '}')"!][!//
            [!VAR "CtuListStartIndexArrayString" = "concat($CtuListStartIndexArrayString, '}')"!][!//
            [!VAR "CtuListMaxNumElemsArrayString" = "concat($CtuListMaxNumElemsArrayString, '}')"!][!//

            [!VAR "MaxCfgHw" = "0"!][!//
            [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
                [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
                    [!IF "substring-after(substring-after(substring-after(substring-after(AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/') = $CurrentPartition"!]
                        [!VAR "MaxCfgHw" = "$MaxCfgHw + 1"!][!//
                    [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ELSE!][!//
                [!VAR "MaxCfgHw" = "num:i(count(AdcConfigSet/AdcHwUnit/*))"!][!//
            [!ENDIF!][!//

            [!VAR "BctuIpConfigTriggerModeString" = "'NULL_PTR'"!]
            [!VAR "BctuIpConfigControlModeString" = "'NULL_PTR'"!]
            [!IF "$TriggerModeEnabledInPartition = 1"!]
                [!VAR "BctuIpConfigTriggerModeString"!]&BctuIpConfigTriggerMode[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][!ENDVAR!]
            [!ENDIF!]
            [!IF "AutosarExt/AdcEnableCtuControlModeApi"!]
                [!VAR "BctuIpConfigControlModeString"!]&BctuIpConfigControlMode[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][!ENDVAR!]
            [!ENDIF!]

            [!VAR "CurrentPartitionHwUnit" = "substring-after(substring-after(substring-after(substring-after(AdcConfigSet/AdcHwUnit/*/AdcHwUnitId[.=concat('ADC',$Unit)]/../AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/')"!][!//

            [!IF "AutosarExt/AdcEnableCtuTrigAutosarExtApi"!]
                [!VAR "HwTriggIdArrayString" = "'{ '"!][!//
                [!VAR "CtuMaxTriggerId"="num:i(ecu:get('AdcMaxCtuTriggerEventId'))"!]
                [!VAR "HwTriggSourceArray" = "''"!][!//
                [!VAR "TriggComma" = "'U, '"!]
                [!LOOP "AdcConfigSet/AdcHwTrigger/*"!]
                    [!VAR "HwTriggSourceArray" = "concat($HwTriggSourceArray, AdcHwTrigSrc, ',')"!][!//
                    [!IF "contains(AdcHwTrigSrc,'BCTU_EMIOS_0')"!]
                        [!VAR "HwTriggIndex" = "num:i(substring-after(AdcHwTrigSrc, 'BCTU_EMIOS_0_'))"!][!//
                        [!VAR "HwTriggIdArrayString" = "concat($HwTriggIdArrayString, $HwTriggIndex, $TriggComma)"!][!//
                    [!ELSEIF "contains(AdcHwTrigSrc,'BCTU_EMIOS_1')"!]
                        [!VAR "HwTriggIndex" = "num:i(num:i(substring-after(AdcHwTrigSrc, 'BCTU_EMIOS_1_')) + 24)"!][!//
                        [!VAR "HwTriggIdArrayString" = "concat($HwTriggIdArrayString, $HwTriggIndex, $TriggComma)"!][!//
                    [!ELSEIF "contains(AdcHwTrigSrc,'BCTU_EMIOS_2')"!]
                        [!VAR "HwTriggIndex" = "num:i(num:i(substring-after(AdcHwTrigSrc, 'BCTU_EMIOS_2_')) + 48)"!][!//
                        [!VAR "HwTriggIdArrayString" = "concat($HwTriggIdArrayString, $HwTriggIndex, $TriggComma)"!][!//
                    [!ELSEIF "contains(AdcHwTrigSrc,'BCTU_Trg23')"!]
                        [!VAR "HwTriggIdArrayString" = "concat($HwTriggIdArrayString, '23', $TriggComma)"!][!//
                    [!ELSEIF "contains(AdcHwTrigSrc,'BCTU_Trg47')"!]
                        [!VAR "HwTriggIdArrayString" = "concat($HwTriggIdArrayString, '47', $TriggComma)"!][!//
                    [!ELSEIF "contains(AdcHwTrigSrc,'BCTU_Trg71')"!]
                        [!VAR "HwTriggIdArrayString" = "concat($HwTriggIdArrayString, '71', $TriggComma)"!][!//
                    [!ELSEIF "contains(AdcHwTrigSrc,'EXT_AND_AUX_EXT_TRIG')"!][!// Do not change order condition for external trigger
                        [!VAR "HwTriggIdArrayString" = "concat($HwTriggIdArrayString, $CtuMaxTriggerId + 3, $TriggComma)"!][!//
                    [!ELSEIF "contains(AdcHwTrigSrc,'AUX_EXT_TRIG')"!]
                        [!VAR "HwTriggIdArrayString" = "concat($HwTriggIdArrayString, $CtuMaxTriggerId + 2, $TriggComma)"!][!//
                    [!ELSEIF "contains(AdcHwTrigSrc,'EXT_TRIG')"!]
                        [!VAR "HwTriggIdArrayString" = "concat($HwTriggIdArrayString, $CtuMaxTriggerId + 1, $TriggComma)"!][!//
                    [!ENDIF!]
                [!ENDLOOP!]
                [!VAR "HwTriggIdArrayString" = "concat($HwTriggIdArrayString, 'Dummy')"!][!// Add Dummy text to remove "," at the end of string
                [!VAR "HwTriggIdArrayString" = "text:replace($HwTriggIdArrayString, ', Dummy', ' }')"!][!//
            [!ENDIF!]
 [!CODE!][!//
    [!"$AdcConfigArrayString"!], /* AdcConfigPtrArr */
    [!"$AdcPhysicalChannelIdArrayString"!], /* AdcPhysicalIdArr */
#if (ADC_DUAL_CLOCK_MODE == STD_ON)
    [!"$AdcAltClockConfigArrayString"!], /* AdcAltClockConfig */
#endif /* (ADC_DUAL_CLOCK_MODE == STD_ON) */
    [!"$AdcIpwChannelConfigArrayString"!], /* AdcIpwChannelConfig */
    [!"$BctuIpConfigTriggerModeString"!], /* CtuConfigTriggerMode */
    [!"$BctuIpConfigControlModeString"!], /* CtuConfigControlMode */
#if ((ADC_IPW_CTU_TRIGGER_MODE_SUPPORTED == STD_ON) && ((ADC_IPW_BCTU_AVAILABLE == STD_ON) || (ADC_IPW_CTU_AVAILABLE == STD_ON)))
    [!"$CtuListStartIndexArrayString"!], /* CtuListStartIndex */
    [!"$CtuListMaxNumElemsArrayString"!], /* CtuListMaxNumElems */
#endif /* ((ADC_IPW_CTU_TRIGGER_MODE_SUPPORTED == STD_ON) && ((ADC_IPW_BCTU_AVAILABLE == STD_ON) || (ADC_IPW_CTU_AVAILABLE == STD_ON))) */
    [!"$AdcUnitSupportCtuControlModeArrayString"!], /* AdcUnitSupportCtuControlMode */
    /* Mapping */
    {
        [!"$TransferTypeArrayString"!], /* AdcDmaInterruptSoftware */
        /**< @brief number of the maximum hardware units in the current configuration */
        [!"num:i($MaxCfgHw)"!]U, /* AdcMaxHwCfg */
        /**< @brief number of groups per hw unit > */
        [!"$GroupCountArrayString"!], /* AdcGroups */
        /**< @brief number of channels per hw unit > */
        [!"$ChannelCountArrayString"!], /* AdcChannels */
        /* ChannelLogicalId */
        {
            [!"$ChannelLogicalIdArrayString0"!]
            [!"$ChannelLogicalIdArrayString1"!]
[!IF "(2 < num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit')))"!][!//
            [!"$ChannelLogicalIdArrayString2"!]
[!ENDIF!][!//
        },
        [!"$DmaChannelArrayString"!], /* AdcDmaChannelLogicId */
        [!"$DmaNumSgaElementArrayString"!], /* DmaNumSgaElement */
#if (STD_ON == ADC_OPTIMIZE_DMA_STREAMING_GROUPS) || (ADC_ENABLE_GROUP_STREAMING_RESULTS_REORDER == STD_ON)
        [!"$CountingDmaChannelArrayString"!], /* AdcCountingDmaChanLogicId */
#endif /* (STD_ON == ADC_OPTIMIZE_DMA_STREAMING_GROUPS) || (ADC_ENABLE_GROUP_STREAMING_RESULTS_REORDER == STD_ON) */
        [!"$HwUnitEnableArrayString"!], /* AdcHwUnitArr */
        [!"$HwLogicalIdArrayString"!], /* HwLogicalId */
[!ENDCODE!][!//
[!IF "(node:value(AutosarExt/AdcEnableCtuTrigAutosarExtApi)='true')"!]
[!CODE!][!//
#if (ADC_ENABLE_CTUTRIG_NONAUTO_API == STD_ON)
        [!"$HwTriggIdArrayString"!] /* HwTriggIndex */
#endif /* (ADC_ENABLE_CTUTRIG_NONAUTO_API == STD_ON) */
[!ENDCODE!][!//
[!ENDIF!]
[!CODE!][!//
    }
};

[!ENDCODE!][!//
        [!ENDIF!]
    [!ENDFOR!][!// End of For Loop 1
[!ENDNOCODE!][!//
[!ENDMACRO!][!//


[!// This macro should be run only when "Adc Conversion Time Once" is OFF
[!MACRO "AdcIpwGroupConfigMacro" , "MacroGenerationType"!][!// GenerationType='definition' or 'declaration'
[!IF "node:value(AutosarExt/AdcConvTimeOnce)='false'"!][!//
#ifdef ADC_AVERAGING_FUNCTIONALITY_SUPPORT
#if (ADC_SET_ADC_CONV_TIME_ONCE == STD_OFF)
[!NOCODE!][!//  the groupId is unique among all partitions, no need to generate following number of partitions
    [!VAR "LoopGroupPartition"="0"!][!//
    [!VAR "NumGroup" = "num:i(count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*)-1)"!][!//
    [!FOR "LoopVar" = "0" TO "$NumGroup"!][!//
        [!LOOP "AdcConfigSet/AdcHwUnit/*/AdcGroup/*"!][!//
            [!IF "(AdcGroupId = $LoopVar)"!][!//
                [!IF "node:value(../../../../../AutosarExt/AdcMulticoreSupport)='true'"!]
                    [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(../../AdcHwUnitEcucPartitionRef/*[@index = "0"],'/'),'/'),'/'),'/')"!][!//
                [!ENDIF!][!//
                [!IF "$MacroGenerationType = 'declaration'"!]
[!CODE!][!//
/**
* @brief          ADC Ipw Group [!"AdcGroupId"!] Config [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!][!IF "node:value(../../../../../AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*/
extern const Adc_Ipw_GroupConfig AdcIpwGroupConfig_[!"AdcGroupId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(../../../../../AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!];

[!ENDCODE!][!//
                [!ELSE!]
[!CODE!][!//
/**
* @brief          ADC Ipw Group [!"AdcGroupId"!] Config [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!][!IF "node:value(../../../../../AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*/
const Adc_Ipw_GroupConfig AdcIpwGroupConfig_[!"AdcGroupId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(../../../../../AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!] =
{
[!ENDCODE!][!//
                    [!IF "(../../../../../AutosarExt/AdcConvTimeOnce = 'false')"!][!//
                        [!VAR "SampleTimeString" = "concat('{ ', node:value(AdcGroupConversionConfiguration/AdcSamplingDuration0), 'U' )"!]
                        [!IF "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) > 1"!][!//
                            [!VAR "SampleTimeString" = "concat($SampleTimeString, ', ', node:value(AdcGroupConversionConfiguration/AdcSamplingDuration1), 'U' )"!]
                        [!ENDIF!]
                        [!IF "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) > 2"!][!//
                            [!VAR "SampleTimeString" = "concat($SampleTimeString, ', ', node:value(AdcGroupConversionConfiguration/AdcSamplingDuration2), 'U' )"!]
                        [!ENDIF!]
                        [!VAR "SampleTimeString" = "concat($SampleTimeString, ' }' )"!]
                        [!IF "(../../../../../AutosarExt/AdcEnableDualClockMode)"!][!//
                            [!VAR "AltSampleTimeString" = "concat('{ ', node:value(AdcAlternateGroupConvTimings/AdcAltGroupSamplingDuration0), 'U' )"!]
                            [!IF "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) > 1"!][!//
                                [!VAR "AltSampleTimeString" = "concat($AltSampleTimeString, ', ', node:value(AdcAlternateGroupConvTimings/AdcAltGroupSamplingDuration1), 'U' )"!]
                            [!ENDIF!]
                            [!IF "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) > 2"!][!//
                                [!VAR "AltSampleTimeString" = "concat($AltSampleTimeString, ', ', node:value(AdcAlternateGroupConvTimings/AdcAltGroupSamplingDuration2), 'U' )"!]
                            [!ENDIF!]
                            [!VAR "AltSampleTimeString" = "concat($AltSampleTimeString, ' }' )"!]
                        [!ELSE!][!//
                            [!IF "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) > 1"!][!//
                                [!VAR "AltSampleTimeString" = "'{ ADC_SAR_IP_DEF_SAMPLE_TIME, ADC_SAR_IP_DEF_SAMPLE_TIME }'"!]
                            [!ENDIF!]
                            [!IF "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) > 2"!][!//
                                [!VAR "AltSampleTimeString" = "'{ ADC_SAR_IP_DEF_SAMPLE_TIME, ADC_SAR_IP_DEF_SAMPLE_TIME, ADC_SAR_IP_DEF_SAMPLE_TIME }'"!]
                            [!ENDIF!]
                        [!ENDIF!]
                        [!IF "AdcGroupConversionConfiguration/AdcGroupHardwareAverageEnable = 'true'"!][!VAR "GroupAvgEnableString" = "'STD_ON'"!][!ELSE!][!VAR "GroupAvgEnableString" = "'STD_OFF'"!][!ENDIF!]
                        [!VAR "GroupAvgSelectString" = "concat('ADC_SAR_IP_AVG_', substring-after(AdcGroupConversionConfiguration/AdcGroupHardwareAverageSelect, 'SAMPLES_'),'_CONV')"!]
                    [!ELSE!][!//
                        [!IF "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) > 1"!][!//
                            [!VAR "SampleTimeString" = "'{ ADC_SAR_IP_DEF_SAMPLE_TIME, ADC_SAR_IP_DEF_SAMPLE_TIME }'"!]
                            [!VAR "AltSampleTimeString" = "'{ ADC_SAR_IP_DEF_SAMPLE_TIME, ADC_SAR_IP_DEF_SAMPLE_TIME }'"!]
                        [!ENDIF!]
                        [!IF "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) > 2"!][!//
                            [!VAR "SampleTimeString" = "'{ ADC_SAR_IP_DEF_SAMPLE_TIME, ADC_SAR_IP_DEF_SAMPLE_TIME, ADC_SAR_IP_DEF_SAMPLE_TIME }'"!]
                            [!VAR "AltSampleTimeString" = "'{ ADC_SAR_IP_DEF_SAMPLE_TIME, ADC_SAR_IP_DEF_SAMPLE_TIME, ADC_SAR_IP_DEF_SAMPLE_TIME }'"!]
                        [!ENDIF!]
                        [!VAR "GroupAvgEnableString" = "'STD_OFF'"!]
                        [!VAR "GroupAvgSelectString" = "'ADC_SAR_IP_AVG_4_CONV'"!]
                    [!ENDIF!][!//

                    [!VAR "GroupAvgEnableAlternateString" = "'STD_OFF'"!]
                    [!VAR "GroupAvgSelectAlternateString" = "'ADC_SAR_IP_AVG_4_CONV'"!]
                    [!IF "(../../../../../AutosarExt/AdcEnableDualClockMode)"!][!//
                        [!IF "AdcAlternateGroupConvTimings/AdcGroupAltHardwareAverageEnable = 'true'"!][!VAR "GroupAvgEnableAlternateString" = "'STD_ON'"!][!ELSE!][!VAR "GroupAvgEnableAlternateString" = "'STD_OFF'"!][!ENDIF!]
                        [!VAR "GroupAvgSelectAlternateString" = "concat('ADC_SAR_IP_AVG_', substring-after(AdcAlternateGroupConvTimings/AdcGroupAltHardwareAverageSelect, 'SAMPLES_'),'_CONV')"!]
                    [!ENDIF!][!//
[!CODE!][!//
    /**< @brief Main Average enable status of group */
    [!"$GroupAvgEnableString"!], /* GroupAvgEnable */
    /**< @brief Main Average selection of group */
    [!"$GroupAvgSelectString"!], /* GroupAvgSelect */
#if (ADC_DUAL_CLOCK_MODE == STD_ON)
    /**< @brief Alternate Average enable status of group */
    [!"$GroupAvgEnableAlternateString"!], /* GroupAvgEnableAlternate */
    /**< @brief Alternate Average selection of group */
    [!"$GroupAvgSelectAlternateString"!], /* GroupAvgSelectAlternate */
#endif /* (ADC_DUAL_CLOCK_MODE == STD_ON) */
    [!"$SampleTimeString"!], /* ConvTime */
#if (ADC_DUAL_CLOCK_MODE == STD_ON)
    [!"$AltSampleTimeString"!] /* AlternateConvTime */
#endif /* (ADC_DUAL_CLOCK_MODE == STD_ON) */
};

[!ENDCODE!][!//
                [!ENDIF!][!//
                [!BREAK!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
    [!ENDFOR!][!//
[!ENDNOCODE!][!//
#endif /* (ADC_SET_ADC_CONV_TIME_ONCE == STD_OFF) */
#endif /* ADC_AVERAGING_FUNCTIONALITY_SUPPORT */
[!ENDIF!][!//
[!ENDMACRO!][!//




[!MACRO "AdcSarIpConfigMacro" , "MacroGenerationType"!][!// GenerationType='definition' or 'declaration'
[!NOCODE!][!// the logicalId is unique among all partitions, no need to generate following number of partitions
    [!VAR "MaxUnitCfg"="num:i(count(AdcConfigSet/AdcHwUnit/*)-1)"!][!//
    [!FOR "Unit" = "0" TO "num:i($MaxUnitCfg)"!][!//
        [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
            [!IF "(AdcLogicalUnitId = $Unit)"!][!//
                [!VAR "AdcPhysicalIdArr" = "substring-after(AdcHwUnitId,'ADC')"!][!//
                [!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!]
                    [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcHwUnitEcucPartitionRef/*[@index = "0"],'/'),'/'),'/'),'/')"!][!//
                [!ENDIF!][!//
                [!IF "$MacroGenerationType = 'declaration'"!]
[!CODE!][!//
/**
* @brief          ADC SAR Ip Config for Logical ID [!"AdcLogicalUnitId"!] corresponding to the [!"AdcHwUnitId"!] configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!] [!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*/
extern const Adc_Sar_Ip_ConfigType AdcSarIpConfig_[!"AdcLogicalUnitId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!];

[!ENDCODE!][!//
                [!ELSE!]
[!CODE!][!//
/**
* @brief          ADC SAR Ip Config for Logical ID [!"AdcLogicalUnitId"!] corresponding to the [!"AdcHwUnitId"!] configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!] [!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*/
const Adc_Sar_Ip_ConfigType AdcSarIpConfig_[!"AdcLogicalUnitId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!] =
{
[!ENDCODE!][!//
                    [!VAR "ClockSelectString" = "'ADC_SAR_IP_CLK_FULL_BUS'"!]
                    [!IF "node:exists(AdcPrescale)"!]
                        [!IF "num:i(AdcPrescale) = 2"!]
                            [!VAR "ClockSelectString" = "'ADC_SAR_IP_CLK_HALF_BUS'"!]
                        [!ELSEIF "num:i(AdcPrescale) = 4"!]
                            [!VAR "ClockSelectString" = "'ADC_SAR_IP_CLK_QUARTER_BUS'"!]
                        [!ENDIF!]
                    [!ENDIF!]
                    [!VAR "CalibrationClockSelectString" = "'ADC_SAR_IP_CLK_HALF_BUS'"!]
                    [!IF "(../../../AutosarExt/AdcEnableCalibration)"!]
                        [!IF "num:i(AdcCalibrationPrescale) = 1"!]
                            [!VAR "CalibrationClockSelectString" = "'ADC_SAR_IP_CLK_FULL_BUS'"!]
                        [!ELSEIF "num:i(AdcCalibrationPrescale) = 4"!]
                            [!VAR "CalibrationClockSelectString" = "'ADC_SAR_IP_CLK_QUARTER_BUS'"!]
                        [!ENDIF!]
                    [!ENDIF!]
                    [!VAR "AdcHighSpeedEnableString" = "'FALSE'"!]
                    [!IF "node:exists(AdcHighSpeedEnable)"!]
                        [!VAR "AdcHighSpeedEnableString" = "text:toupper(AdcHighSpeedEnable)"!]
                    [!ENDIF!]

                    [!VAR "AdcHwAvgEnableString" = "'FALSE'"!]
                    [!VAR "AdcHwAvgSelectString" = "'ADC_SAR_IP_AVG_4_CONV'"!]
                    [!VAR "SampleTimeString" = "'{ ADC_SAR_IP_DEF_SAMPLE_TIME,'"!]
                    [!IF "(../../../AutosarExt/AdcConvTimeOnce = 'true') and (node:exists(AdcNormalConvTimings))"!]
                        [!IF "node:exists(AdcNormalConvTimings/AdcHardwareAverageEnable)"!]
                            [!VAR "AdcHwAvgEnableString" = "text:toupper(AdcNormalConvTimings/AdcHardwareAverageEnable)"!]
                        [!ENDIF!]
                        [!IF "node:exists(AdcNormalConvTimings/AdcHardwareAverageSelect)"!]
                            [!VAR "AdcHwAvgSelectString" = "concat('ADC_SAR_IP_AVG_', substring-after(AdcNormalConvTimings/AdcHardwareAverageSelect, 'SAMPLES_'),'_CONV')"!]
                        [!ENDIF!]
                        [!VAR "SampleTimeString" = "concat('{ ', node:value(AdcNormalConvTimings/AdcSamplingDurationNormal0), 'U, ' )"!]
                    [!ENDIF!]
                    [!IF "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) > 1"!][!//
                        [!IF "(../../../AutosarExt/AdcConvTimeOnce = 'true') and (node:exists(AdcNormalConvTimings))"!]
                            [!VAR "SampleTimeString" = "concat($SampleTimeString, node:value(AdcNormalConvTimings/AdcSamplingDurationNormal1), 'U, ' )"!]
                        [!ELSE!]
                            [!VAR "SampleTimeString" = "concat($SampleTimeString,' ADC_SAR_IP_DEF_SAMPLE_TIME, ')"!]
                        [!ENDIF!]
                    [!ENDIF!]
                    [!IF "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) > 2"!][!//
                        [!IF "(../../../AutosarExt/AdcConvTimeOnce = 'true') and (node:exists(AdcNormalConvTimings))"!]
                            [!VAR "SampleTimeString" = "concat($SampleTimeString, node:value(AdcNormalConvTimings/AdcSamplingDurationNormal2), 'U' )"!]
                        [!ELSE!]
                            [!VAR "SampleTimeString" = "concat($SampleTimeString,'ADC_SAR_IP_DEF_SAMPLE_TIME')"!]
                        [!ENDIF!]
                    [!ENDIF!]
                    [!VAR "SampleTimeString" = "concat($SampleTimeString, ' }' )"!]

                    [!VAR "CrtNum"="0"!]
                    [!VAR "MaxNum"="num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) - 1"!]
                    [!VAR "PresamplingSourceString" = "'{ '"!]
                    [!FOR "RegIdx" = "0" TO "num:i(ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')) - 1"!]
                        [!VAR "RegConfig" = "concat('AdcPresamplingInternalSignal', $RegIdx)"!]
                        [!IF "node:value($RegConfig) = 'VREFL' "!]
                            [!IF "$CrtNum<$MaxNum"!][!VAR "PresamplingSourceString" = "concat($PresamplingSourceString, 'ADC_SAR_IP_PRESAMPLE_VREFL, ')"!][!ELSE!][!VAR "PresamplingSourceString" = "concat($PresamplingSourceString, 'ADC_SAR_IP_PRESAMPLE_VREFL ')"!][!ENDIF!]
                        [!ELSE!]
                            [!IF "node:value($RegConfig) = 'VREFH' "!]
                                [!IF "$CrtNum<$MaxNum"!][!VAR "PresamplingSourceString" = "concat($PresamplingSourceString, 'ADC_SAR_IP_PRESAMPLE_VREFH, ')"!][!ELSE!][!VAR "PresamplingSourceString" = "concat($PresamplingSourceString, 'ADC_SAR_IP_PRESAMPLE_VREFH ')"!][!ENDIF!]
                            [!ENDIF!]
                        [!ENDIF!]
                    [!VAR "CrtNum"="$CrtNum+1"!]
                    [!ENDFOR!]
                    [!VAR "PresamplingSourceString" = "concat($PresamplingSourceString,'}')"!]

                    [!VAR "DecodeDelayString" = "'0U'"!]
                    [!IF "node:exists(AdcMuxDelay)"!]
                        [!VAR "DecodeDelayString" = "num:i(AdcMuxDelay)"!]
                    [!ENDIF!]

                    [!VAR "EndOfNormalChainNotificationString" = "'NULL_PTR'"!]
                    [!VAR "EndOfInjectedChainNotificationString" = "'NULL_PTR'"!]
                    [!VAR "EndOfWdgOutOfRangeNotificationString" = "'NULL_PTR'"!]
                    [!IF "AdcTransferType = 'ADC_INTERRUPT'"!]
                        [!LOOP "AdcGroup/*[AdcWithoutInterrupts = 'false']"!]
                            [!IF "AdcGroupConversionType = 'ADC_CONV_TYPE_NORMAL'"!]
                                [!VAR "EndOfNormalChainNotificationString" = "concat('Adc_Ipw_Adc', $AdcPhysicalIdArr, 'EndNormalChainNotification')"!]
                                [!BREAK!]
                            [!ENDIF!]
                        [!ENDLOOP!]
                        [!LOOP "AdcGroup/*[AdcWithoutInterrupts = 'false']"!]
                            [!IF "AdcGroupConversionType = 'ADC_CONV_TYPE_INJECTED'"!]
                                [!VAR "EndOfInjectedChainNotificationString" = "concat('Adc_Ipw_Adc', $AdcPhysicalIdArr, 'EndInjectedChainNotification')"!]
                                [!BREAK!]
                            [!ENDIF!]
                        [!ENDLOOP!]
                        [!VAR "AdcHwUnitDmaClearSourceString" = "'ADC_SAR_IP_DMA_REQ_CLEAR_ON_ACK'"!]
                    [!ELSE!]
                        [!VAR "AdcHwUnitDmaClearSourceString" = "concat('ADC_SAR_IP_', node:value(AdcHwUnitDmaClearSource))"!]
                    [!ENDIF!]
                    [!LOOP "AdcChannel/*"!]
                        [!IF "AdcEnableThresholds = 'true'"!]
                            [!VAR "EndOfWdgOutOfRangeNotificationString" = "concat('Adc_Ipw_Adc', $AdcPhysicalIdArr, 'WdgThresholdNotification')"!]
                            [!BREAK!]
                        [!ENDIF!]
                    [!ENDLOOP!]
                    [!VAR "NumWdgThresholdsString" = "'0'"!]
                    [!VAR "WdgThresholdsString" = "'NULL_PTR'"!]
                    [!IF "count(AdcChannel/*[AdcEnableThresholds='true']) > 0"!]
                        [!VAR "NumWdgThresholdsString" = "num:i(count(AdcThresholdControl/*))"!]
                        [!IF "$NumWdgThresholdsString > 0"!]
                            [!VAR "WdgThresholdsString"!]Adc_ThresholdCfg_[!"AdcLogicalUnitId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][!ENDVAR!]
                        [!ENDIF!]
                    [!ENDIF!]

                    [!VAR "CtuModeString" = "'ADC_SAR_IP_CTU_MODE_DISABLED'"!]
                    [!IF "count(AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains(node:value(concat('../../../../AdcHwTrigger/',substring-after(AdcGroupHwTriggerSource,'AdcConfigSet/'),'/AdcHwTrigSrc')), 'EXT_TRIG'))]) > 0"!]
                        [!VAR "CtuModeString" = "'ADC_SAR_IP_CTU_MODE_TRIGGER'"!]
                    [!ENDIF!]
[!CODE!][!//
    ADC_SAR_IP_CONV_MODE_ONESHOT, /* ConvMode */[!// Set default to ONESHOT because gets overwritten by group param .Mode
[!ENDCODE!][!//
[!IF "ecu:get('AdcSetResolutionAvailable') = 'TRUE'"!]
[!CODE!][!//
#if (ADC_SAR_IP_SET_RESOLUTION == STD_ON)
    ADC_SAR_IP_[!"AdcHwUnitResolution"!], /* AdcResolution */
    (boolean)[!"text:toupper(AdcHwUnitBypassResolution)"!], /* BypassResolution */
#endif /* (ADC_SAR_IP_SET_RESOLUTION == STD_ON) */
[!ENDCODE!][!//
[!ENDIF!]
[!CODE!][!//
    [!"$ClockSelectString"!], /* ClkSelect */
    [!"$CalibrationClockSelectString"!], /* CalibrationClkSelect */
#if FEATURE_ADC_HAS_HIGH_SPEED_ENABLE
    (boolean)[!"$AdcHighSpeedEnableString"!], /* HighSpeedConvEn */
#endif /* FEATURE_ADC_HAS_HIGH_SPEED_ENABLE */
#if FEATURE_ADC_HAS_CTU
    [!"$CtuModeString"!], /* CtuMode */
#endif /* FEATURE_ADC_HAS_CTU */
#if FEATURE_ADC_HAS_INJ_EXT_TRIGGER
    ADC_SAR_IP_EXT_TRIG_EDGE_DISABLED, /* InjectedEdge */[!// Set default to disabled because gets overwritten by group param .Mode
#endif /* FEATURE_ADC_HAS_INJ_EXT_TRIGGER */
#if FEATURE_ADC_HAS_EXT_TRIGGER
    ADC_SAR_IP_EXT_TRIG_EDGE_DISABLED, /* ExtTrigger */[!// Set default to disabled because gets overwritten by group param .Mode
    (boolean)FALSE, /* NormalExtTrgEn */[!// Set default to FALSE because gets overwritten at group config
#if FEATURE_ADC_HAS_AUX_EXT_TRIGGER
    (boolean)FALSE, /* NormalAuxExtTrgEn */[!// Set default to FALSE because gets overwritten at group config
#endif /* FEATURE_ADC_HAS_AUX_EXT_TRIGGER */
#endif /* FEATURE_ADC_HAS_EXT_TRIGGER */
#if FEATURE_ADC_HAS_CONVERSION_TIMING
    [!"$SampleTimeString"!], /* SampleTimeArr */
#endif /* FEATURE_ADC_HAS_CONVERSION_TIMING */
#if FEATURE_ADC_HAS_PRESAMPLING
    (boolean)[!"text:toupper(AdcBypassSampling)"!], /* BypassSampling */
    [!"$PresamplingSourceString"!], /* PresamplingSourceArr */
#endif /* FEATURE_ADC_HAS_PRESAMPLING */
    (boolean)[!"text:toupper(AdcAutoClockOff)"!], /* AutoClockOff */
    (boolean)[!"text:toupper(AdcHwUnitOverwriteEn)"!], /* OverwriteEnable */
    [!IF "../../../AdcGeneral/AdcResultAlignment = 'ADC_ALIGN_RIGHT'"!]ADC_SAR_IP_DATA_ALIGNED_RIGHT[!ELSE!]ADC_SAR_IP_DATA_ALIGNED_LEFT[!ENDIF!], /* DataAlign */
#if FEATURE_ADC_SAR_DECODE_DELAY
    [!"$DecodeDelayString"!]U, /* DecodeDelay */
#endif /* FEATURE_ADC_SAR_DECODE_DELAY */
    [!"AdcPowerDownDelay"!]U, /* PowerDownDelay */
#if FEATURE_ADC_HAS_AVERAGING
    (boolean)[!"$AdcHwAvgEnableString"!], /* AvgEn */
    [!"$AdcHwAvgSelectString"!], /* AvgSel */
#endif /* FEATURE_ADC_HAS_AVERAGING */
    [!"AdcHwUnitUsrOffset"!]U, /* UsrOffset */
    [!"AdcHwUnitUsrGain"!]U, /* UsrGain */
    (boolean)FALSE, /* DmaEnable */[!// unused by HLD
    [!"$AdcHwUnitDmaClearSourceString"!], /* DmaClearSource */
    { 0U, 0U, 0U }, /* ChanMaskNormal */
    { 0U, 0U, 0U }, /* ChanMaskInjected */
    [!"num:i(count(AdcChannel/*))"!]U, /* NumChannels */
    AdcSarIpChansConfig_[!"AdcLogicalUnitId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!], /* ChannelConfigsPtr */
#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
    [!"$NumWdgThresholdsString"!]U, /* NumWdgThresholds */
    [!"$WdgThresholdsString"!], /* WdgThresholds */
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */
#if (STD_ON == ADC_SAR_IP_ECH_ENABLED)
    [!"$EndOfNormalChainNotificationString"!], /* EndOfNormalChainNotification */
#endif /* (STD_ON == ADC_SAR_IP_ECH_ENABLED) */
#if (STD_ON == ADC_SAR_IP_JECH_ENABLED)
    [!"$EndOfInjectedChainNotificationString"!], /* EndOfInjectedChainNotification */
#endif /* (STD_ON == ADC_SAR_IP_JECH_ENABLED) */
#if FEATURE_ADC_HAS_CTU
#if (STD_ON == ADC_SAR_IP_EOCTU_ENABLED)
    NULL_PTR, /* EndOfCtuConversionNotification */
#endif /* (STD_ON == ADC_SAR_IP_EOCTU_ENABLED) */
#endif /* FEATURE_ADC_HAS_CTU */
#if (STD_ON == ADC_SAR_IP_EOC_ENABLED)
    NULL_PTR, /* EndOfConvNotification */
#endif /* (STD_ON == ADC_SAR_IP_EOC_ENABLED) */
#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
    [!"$EndOfWdgOutOfRangeNotificationString"!] /* WdgOutOfRangeNotification */
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */
};

[!ENDCODE!][!//
                [!ENDIF!]
            [!ENDIF!]
        [!ENDLOOP!]
    [!ENDFOR!]
[!ENDNOCODE!]
[!ENDMACRO!]

[!MACRO "CheckUniqueDmaChannel"!][!//
[!NOCODE!][!//
[!VAR "DmaChnArr" = "''"!][!//
[!VAR "CheckingStatus" = "'TRUE'"!][!//
    [!LOOP "AdcConfigSet/AdcHwUnit/*[AdcTransferType = 'ADC_DMA']"!][!//
        [!IF "node:exists(AdcDmaChannelId)"!][!//
            [!IF "(text:indexOf(text:split($DmaChnArr, ','), node:value(AdcDmaChannelId)) = -1)"!][!//
                [!VAR "DmaChnArr" = "concat($DmaChnArr, AdcDmaChannelId, ',')"!][!//
            [!ELSE!]
                [!VAR "CheckingStatus" = "'FALSE'"!][!//
                [!BREAK!][!//
            [!ENDIF!][!//
        [!ENDIF!][!//
        [!LOOP "AdcGroup/*[((AdcEnableOptimizeDmaStreamingGroups = 'true') and (count(AdcGroupDefinition/*) > 1)) or ((AdcWithoutInterrupts = 'true') and (AdcGroupAccessMode = 'ADC_ACCESS_MODE_STREAMING'))]"!][!//
            [!IF "node:exists(../../AdcCountingDmaChannelId)"!][!//
                [!IF "(text:indexOf(text:split($DmaChnArr, ','), node:value(../../AdcCountingDmaChannelId)) = -1)"!][!//
                    [!VAR "DmaChnArr" = "concat($DmaChnArr, ../../AdcCountingDmaChannelId, ',')"!][!//
                [!ELSE!]
                    [!VAR "CheckingStatus" = "'FALSE'"!][!//
                [!ENDIF!][!//
                [!BREAK!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
    [!ENDLOOP!][!//
    [!IF "AutosarExt/CtuEnableDmaTransferMode = 'true'"!]
        [!LOOP "AdcConfigSet/BctuHwUnit/*/BctuResultFifos/*[BctuFifoDmaEnable = 'true' and node:exists(BctuFifoDmaChannelId)]"!][!//
            [!IF "(text:indexOf(text:split($DmaChnArr, ','), node:value(BctuFifoDmaChannelId)) = -1)"!][!//
                [!VAR "DmaChnArr" = "concat($DmaChnArr, BctuFifoDmaChannelId, ',')"!][!//
            [!ELSE!]
                [!VAR "CheckingStatus" = "'FALSE'"!][!//
                [!BREAK!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
    [!ENDIF!]
    [!IF "($CheckingStatus = 'FALSE')"!]
        [!ERROR!][!//
            [!"'DMA Channel cannot be shared among Adc configurations and Ctu Fifo configuration.'"!][!//
        [!ENDERROR!][!//
    [!ENDIF!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "CheckNumOfChansTriggerMode"!][!//
[!NOCODE!][!//
    [!VAR "CtuMaxNumElemsInList"="num:i(ecu:get('BCTUNumberOfCmdListChan'))"!][!//
    [!VAR "CtuListMaxNumElemsTemp"="0"!][!//
    [!IF "(AutosarExt/AdcCtuHardwareTriggerOptimization = 'true')"!]
        [!IF "count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]/AdcGroupDefinition/*) > $CtuMaxNumElemsInList"!]
            [!ERROR!][!//
                [!"'Configured channels in BCTU trigger mode groups are larger than available resource of BCTU channel list'"!][!//
            [!ENDERROR!][!//
        [!ENDIF!][!//
    [!ELSE!]
        [!VAR "MaxCfgNumTrigModeHWUnit"="0"!]
        [!VAR "MinTrigModePhyHWUnitId"="num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit')) - 1"!]
        [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
            [!IF "count(AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]) > 0 "!][!//
                [!IF "num:i(substring-after(AdcHwUnitId, 'ADC')) < $MinTrigModePhyHWUnitId "!][!//
                    [!VAR "MinTrigModePhyHWUnitId" = "num:i(substring-after(AdcHwUnitId, 'ADC'))"!][!//
                [!ENDIF!][!//
                [!VAR "MaxCfgNumTrigModeHWUnit"="$MaxCfgNumTrigModeHWUnit + 1"!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
        [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
            [!IF "count(AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]) > 0 "!][!//
                [!IF "(num:i($CtuMaxNumElemsInList mod $MaxCfgNumTrigModeHWUnit)!=0) and (num:i(substring-after(AdcHwUnitId, 'ADC')))=num:i($MinTrigModePhyHWUnitId)"!][!//
                    [!VAR "CtuListMaxNumElemsTemp" = "num:i($CtuMaxNumElemsInList div $MaxCfgNumTrigModeHWUnit) + num:i($CtuMaxNumElemsInList mod $MaxCfgNumTrigModeHWUnit)"!][!//
                [!ELSE!][!//
                    [!VAR "CtuListMaxNumElemsTemp" = "num:i($CtuMaxNumElemsInList div $MaxCfgNumTrigModeHWUnit)"!][!//
                [!ENDIF!][!//
            [!ENDIF!][!//
            [!LOOP "AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]"!][!//
                [!IF "num:i(count(AdcGroupDefinition/*)) > $CtuListMaxNumElemsTemp"!][!//
                    [!ERROR!][!//
                        [!"'Configured channels in BCTU trigger mode groups are larger than available resource of BCTU channel list'"!][!//
                    [!ENDERROR!][!//
                [!ENDIF!][!//
            [!ENDLOOP!][!//
        [!ENDLOOP!][!//
    [!ENDIF!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "CheckCTUFeatureMultiCore" , "MacroGenerationType"!][!// GenerationType='triggerMode' or 'controlMode'
[!NOCODE!][!//
[!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!][!// Only check when MultiCore feature is enabled.
    [!IF "$MacroGenerationType = 'triggerMode'"!][!// This part check configuration for Trigger Mode
        [!IF "count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]) > 0"!]
            [!VAR "TriggerModeEnabledInPartition"="0"!][!//
            [!FOR "Partition" = "0" TO "num:i(count(AdcGeneral/AdcEcucPartitionRef/*)) - 1"!][!// Macro Start For Loop Partition 1
                [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcGeneral/AdcEcucPartitionRef/*[@index = $Partition],'/'),'/'),'/'),'/')"!][!//
                [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
                    [!IF "substring-after(substring-after(substring-after(substring-after(AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/') = $CurrentPartition"!]
                        [!IF "count(AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]) > 0"!]
                            [!VAR "TriggerModeEnabledInPartition"="$TriggerModeEnabledInPartition + 1"!][!//
                            [!BREAK!][!//
                        [!ENDIF!]
                    [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ENDFOR!][!// Macro End For Loop Partition 1
            [!IF "$TriggerModeEnabledInPartition > 1"!]
                [!ERROR!][!//
                    [!"'Partition must be the same for all ADC groups that are configured to be triggered by source from BCTU. Please update the partition of the HW Units that has these groups to be same OR choose another source trigger (not come from BCTU) for all ADC Hardware Group.'"!][!//
                [!ENDERROR!][!//
            [!ENDIF!]
        [!ENDIF!]
    [!ELSE!][!// This part checkes configuration for Control Mode
        [!IF "AutosarExt/AdcEnableCtuControlModeApi"!][!//
            [!VAR "HwUnitPartitionArray" = "''"!]
            [!// Example of HwUnitPartitionArray: "ADC0:EcucPartition_0;ADC1:X;ADC2:EcucPartition_0;", where X corresponds to NotFound
            [!FOR "Unit" = "0" TO "num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit')) - 1"!]
                [!VAR "CurrentPartition" = "'X'"!][!//
                [!VAR "Check" = "0"!][!//
                [!VAR "HwUnitPartitionArray" = "concat($HwUnitPartitionArray, 'ADC', $Unit,':')"!]
                [!LOOP "AdcConfigSet/BctuHwUnit/*[1]/BctuInternalTrigger/*"!][!// Only 1 BCTU instance is currently supported
                    [!IF "bit:getbit(BctuAdcTargetMask,$Unit)"!]
                        [!VAR "Check" = "1"!][!//
                        [!BREAK!]
                    [!ENDIF!]
                [!ENDLOOP!][!//
                [!IF "$Check != 0"!]
                    [!LOOP "AdcConfigSet/AdcHwUnit/*[AdcHwUnitId = concat('ADC', $Unit)]"!]
                        [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/')"!][!//
                    [!ENDLOOP!][!//
                [!ENDIF!]
                [!VAR "HwUnitPartitionArray" = "concat($HwUnitPartitionArray, $CurrentPartition, ';')"!]
            [!ENDFOR!]

            [!VAR "CtuMultiPartition" = "'NO'"!][!//
            [!FOR "Index1" = "0" TO "num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit')) - 1"!]
                [!VAR "PartitionTemp1" = "substring-before(substring-after($HwUnitPartitionArray, concat('ADC',$Index1,':')),';')"!]
                [!IF "$PartitionTemp1 != 'X'"!]
                    [!FOR "Index2" = "$Index1 + 1" TO "num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit')) - 1"!]
                        [!VAR "PartitionTemp2" = "substring-before(substring-after($HwUnitPartitionArray, concat('ADC',$Index2,':')),';')"!]
                        [!IF "$PartitionTemp2 != 'X' and $PartitionTemp1 != $PartitionTemp2"!]
                            [!VAR "CtuMultiPartition" = "'YES'"!][!//
                            [!BREAK!]
                        [!ENDIF!]
                    [!ENDFOR!]
                [!ENDIF!]
            [!ENDFOR!]
            [!IF "$CtuMultiPartition = 'YES'"!]
                [!ERROR!][!//
                    [!"'Partition must be the same for all ADC channels that are configured to be triggered by Bctu Control Mode. Please update the partition of the HW Units that has these channels to be same.'"!][!//
                [!ENDERROR!][!//
            [!ENDIF!]
        [!ENDIF!]
    [!ENDIF!]
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "AdcNotificationConfigMacro"!][!//
[!NOCODE!][!//
[!VAR "AdcNotifArr" = "''"!][!//
[!FOR "x" = "0" TO "count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*)-1"!][!//
    [!LOOP "AdcConfigSet/AdcHwUnit/*/AdcGroup/*[$x = AdcGroupId]"!][!//
        [!IF "(../../../../../AdcGeneral/AdcGrpNotifCapability = 'true') and (node:exists(AdcNotification) and (AdcNotification != 'NULL_PTR') and (AdcNotification != 'NULL') and (AdcNotification != ''))"!][!//
            [!IF "(text:indexOf(text:split($AdcNotifArr, ','), node:value(AdcNotification)) = -1)"!][!//
                [!VAR "AdcNotifArr" = "concat($AdcNotifArr, AdcNotification, ',')"!][!//
            [!ENDIF!][!//
        [!ENDIF!][!//
        [!IF "(../../../../../AutosarExt/AdcEnableInitialNotification = 'true') and (AdcExtraNotification != 'NULL_PTR') and (AdcExtraNotification != 'NULL') and (AdcExtraNotification != '')"!][!//
            [!IF "(text:indexOf(text:split($AdcNotifArr, ','), node:value(AdcExtraNotification)) = -1)"!][!//
                [!VAR "AdcNotifArr" = "concat($AdcNotifArr, AdcExtraNotification, ',')"!][!//
            [!ENDIF!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDFOR!][!//

[!IF "($AdcNotifArr != '')"!]
[!CODE!][!//
/**
* @brief         ADC Notification functions.
* @details       ADC Notification functions defined by user.
*
*/
[!ENDCODE!][!//
    [!LOOP "text:split($AdcNotifArr, ',')"!][!//
[!CODE!][!//
void [!"."!](void);
[!ENDCODE!][!//
    [!ENDLOOP!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "AdcWdgNotificationConfigMacro"!][!//
[!NOCODE!][!//
[!VAR "WdgThresholdNotifArr" = "''"!][!//
[!VAR "adcLogChanIdArr" = "''"!][!//
[!LOOP "AdcConfigSet/AdcHwUnit/*/AdcChannel/*"!][!//
    [!IF "(AdcEnableThresholds = 'true') and (AdcWdogNotification != 'NULL_PTR') and (AdcWdogNotification != 'NULL') and (AdcWdogNotification != '')"!]
        [!IF "(text:indexOf(text:split($WdgThresholdNotifArr, ','), node:value(AdcWdogNotification)) = -1)"!]
            [!VAR "WdgThresholdNotifArr" = "concat($WdgThresholdNotifArr, AdcWdogNotification, ',')"!][!//
            [!VAR "adcLogChanIdArr" = "concat($adcLogChanIdArr, AdcLogicalChannelId, ',')"!][!//
        [!ENDIF!][!//
    [!ENDIF!][!//
[!ENDLOOP!][!//
[!IF "($WdgThresholdNotifArr != '')"!]
[!CODE!][!//
/**
* @brief          ADC Watchdog notification functions.
* @details        ADC Watchdog notification functions defined inside the Plugin.
*                 Watchdog notifications have a uint8 paramter(Flags) that shows which
*                 flags(low/high threshold) triggered the interrupt. It should be
*                 interpreted using ADC_WDG_... defines.
*
*/
[!ENDCODE!][!//
    [!VAR "ArrIndex" = "1"!][!//
    [!LOOP "text:split($WdgThresholdNotifArr, ',')"!][!//
[!CODE!][!//
void [!"."!](const uint8 Flags); /**< @brief for Channel - [!"text:split($adcLogChanIdArr, ',')[num:i($ArrIndex)]"!] */
[!ENDCODE!][!//
    [!VAR "ArrIndex" = "$ArrIndex + 1"!][!//
    [!ENDLOOP!][!//
[!ENDIF!][!//
[!ENDNOCODE!]
[!ENDMACRO!][!//

[!MACRO "GetHwResourceMacro" , "Source"!][!//
  [!NOCODE!]
  [!VAR "CtuMaxTriggerId"="num:i(ecu:get('AdcMaxCtuTriggerEventId'))"!]
      [!IF "contains($Source,'BCTU_EMIOS_0')"!]
          [!CODE!][!"num:i(substring-after($Source, 'BCTU_EMIOS_0_'))"!][!ENDCODE!]
      [!ELSEIF "contains($Source,'BCTU_EMIOS_1')"!]
          [!CODE!][!"num:i(num:i(substring-after($Source, 'BCTU_EMIOS_1_')) + 24)"!][!ENDCODE!]
      [!ELSEIF "contains($Source,'BCTU_EMIOS_2')"!]
          [!CODE!][!"num:i(num:i(substring-after($Source, 'BCTU_EMIOS_2_')) + 48)"!][!ENDCODE!]
      [!ELSEIF "contains($Source,'BCTU_Trg23')"!]
          [!CODE!][!"23"!][!ENDCODE!]
      [!ELSEIF "contains($Source,'BCTU_Trg47')"!]
          [!CODE!][!"47"!][!ENDCODE!]
      [!ELSEIF "contains($Source,'BCTU_Trg71')"!]
          [!CODE!][!"71"!][!ENDCODE!]
      [!ELSEIF "contains($Source,'EXT_AND_AUX_EXT_TRIG')"!]
          [!CODE!][!"$CtuMaxTriggerId + 3"!][!ENDCODE!]
      [!ELSEIF "contains($Source,'AUX_EXT_TRIG')"!]
          [!CODE!][!"$CtuMaxTriggerId + 2"!][!ENDCODE!]
      [!ELSEIF "contains($Source,'EXT_TRIG')"!]
          [!CODE!][!"$CtuMaxTriggerId + 1"!][!ENDCODE!]
      [!ENDIF!]
  [!ENDNOCODE!]
[!ENDMACRO!]

[!MACRO "AdcHwTriggerSourceDefinesMacro"!][!//
[!NOCODE!][!//
    [!IF "count(AdcConfigSet/AdcHwTrigger/*) > 0"!]
[!CODE!][!//
/**
* @brief          Hardware trigger sources configured.
*
*/
[!ENDCODE!]
        [!LOOP "AdcConfigSet/AdcHwTrigger/*"!]
            [!VAR "HwTrgSrc" = "AdcHwTrigSrc"!]
            [!VAR "TriggerValue"!][!CALL "GetHwResourceMacro" , "Source" = "$HwTrgSrc"!][!ENDVAR!]
[!CODE!][!//
#define [!"@name"!]                            ([!"concat(num:i($TriggerValue),'U')"!]) /* [!"$HwTrgSrc"!] */
[!ENDCODE!]
        [!ENDLOOP!]
    [!ENDIF!]
[!ENDNOCODE!]
[!ENDMACRO!]


[!MACRO "BctuIpConfigTriggerModeMacro" , "MacroGenerationType"!][!// GenerationType='definition', 'declaration', 'notificationDeclaration'
[!NOCODE!][!//
    [!// Trigger Mode is enabled if at least 1 Group is HW triggered, and trigger is not EXT_TRIG
    [!IF "count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]) > 0"!]
        [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
            [!VAR "MaxPartitionCfg"="num:i(count(AdcGeneral/AdcEcucPartitionRef/*))"!][!//
        [!ELSE!][!//
            [!VAR "MaxPartitionCfg"="1"!][!//
        [!ENDIF!][!//
        [!FOR "Partition" = "0" TO "num:i($MaxPartitionCfg) - 1"!][!// Macro Start FOR 1
            [!VAR "MaxHWUnitInPartition"="0"!][!//
            [!VAR "MaxTriggerModeGroupInPartition"="0"!][!//
            [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
                [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcGeneral/AdcEcucPartitionRef/*[@index = $Partition],'/'),'/'),'/'),'/')"!][!//
                [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
                    [!IF "substring-after(substring-after(substring-after(substring-after(AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/') = $CurrentPartition"!]
                        [!VAR "MaxHWUnitInPartition"="$MaxHWUnitInPartition + 1"!][!//
                        [!VAR "MaxTriggerModeGroupInPartition"="$MaxTriggerModeGroupInPartition + num:i(count(AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]))"!][!//
                    [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ELSE!][!//
                [!VAR "MaxHWUnitInPartition"="num:i(count(AdcConfigSet/AdcHwUnit/*))"!][!//
                [!VAR "MaxTriggerModeGroupInPartition"="num:i(count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]))"!][!//
            [!ENDIF!][!//
            [!IF "$MaxTriggerModeGroupInPartition > 0"!]
                [!IF "$MacroGenerationType = 'declaration'"!]
[!CODE!][!//
/**
* @brief     Bctu Ip Config for Triggered Mode configuration variant[!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*/
extern const Bctu_Ip_ConfigType BctuIpConfigTriggerMode[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!];
[!ENDCODE!][!//
            [!ELSE!]
                [!IF "$MacroGenerationType = 'definition'"!]
[!CODE!][!//
/**
* @brief     Bctu Ip Config for Triggered Mode variant[!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*/
const Bctu_Ip_ConfigType BctuIpConfigTriggerMode[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!] =
{
[!ENDCODE!][!//
                [!ENDIF!][!//
                [!VAR "AdcNotifications" = "'{ '"!][!//
                [!VAR "COUNT" = "0"!][!//
                [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
                        [!IF "(count(AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]) > 0) and $COUNT < 1"!][!//
                        [!ENDIF!][!//
                [!VAR "COUNT" = "$COUNT + 1"!][!//
                [!ENDLOOP!][!//
[!IF "$MacroGenerationType = 'notificationDeclaration'"!][!//
[!CODE!][!//
/**
* @brief     Bctu Notification for Ctu Trigger Mode.
*/
[!ENDCODE!][!//
[!ENDIF!][!//
                [!FOR "Index" = "0" TO "num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit')) - 1"!][!//
                    [!IF "($Index < num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit')) - 1)"!][!//
                        [!VAR "Comma" = "', '"!]
                    [!ELSE!]
                        [!VAR "Comma" = "' '"!]
                    [!ENDIF!]
                    [!VAR "BctuListNotification" = "'NULL_PTR'"!][!//

                    [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
                        [!IF "substring-after(AdcHwUnitId,'ADC') = $Index"!][!//
                            [!IF "count(AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and (AdcWithoutInterrupts = 'false') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]) > 0"!][!//
                                [!// BctuListNotification must be configured if for the current ADC Hw unit at least 1 Group is HW triggered, and trigger is not EXT_TRIG
                                [!IF "node:value(./AdcTransferType)='ADC_INTERRUPT'"!][!//
                                    [!VAR "BctuListNotification" = "concat('Adc_Ipw_BctuEndListNotificationAdc', $Index)"!][!//
                                [!ENDIF!]
                                [!IF "$MacroGenerationType = 'notificationDeclaration'"!][!//
[!CODE!][!//
[!IF "node:value(./AdcTransferType)='ADC_INTERRUPT'"!][!//
[!"concat('void Adc_Ipw_BctuEndListNotificationAdc', $Index, '(void);')"!]
[!ENDIF!][!//
[!ENDCODE!][!//
                                [!ENDIF!]

                            [!ENDIF!]
                        [!ENDIF!]
                    [!ENDLOOP!]
                    [!VAR "AdcNotifications" = "concat($AdcNotifications, '{ NULL_PTR, NULL_PTR, ', $BctuListNotification, ' }', $Comma)"!]
                [!ENDFOR!]
                [!VAR "AdcNotifications" = "concat($AdcNotifications, '}')"!]

                [!IF "$MacroGenerationType = 'definition'"!]
                    [!VAR "NumBctuTrigGroupChan"="0"!][!//
                    [!VAR "BctuIpInternalTriggersString"="'NULL_PTR'"!][!//
                    [!VAR "BctuIpListItemsString"="'NULL_PTR'"!][!//
                    [!IF "(AutosarExt/AdcCtuHardwareTriggerOptimization = 'true')"!]
                        [!// Loop for all CTU trigger mode groups and get sum of channels configured in these groups
                        [!LOOP "AdcConfigSet/AdcHwUnit/*/AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]"!]
                            [!VAR "NumBctuTrigGroupChan"="$NumBctuTrigGroupChan + num:i(count(AdcGroupDefinition/*))"!]
                        [!ENDLOOP!]
                        [!IF "($NumBctuTrigGroupChan > 0)"!]
                            [!VAR "BctuIpInternalTriggersString"!]BctuIpTriggerModeInternalTriggers[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][!ENDVAR!]
                            [!VAR "BctuIpListItemsString"!]BctuIpTriggerModeListItems[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][!ENDVAR!]
                        [!ENDIF!]
                    [!ENDIF!]
[!CODE!][!//
    (boolean)FALSE, /* LowPowerModeEn */
    (boolean)TRUE, /* GlobalHwTriggersEn */
    0U, /* NewDataDmaEnMask */
    NULL_PTR, /* TriggerNotification */[!// always set to this value in Triggered Mode, because uses ADC notifications
    [!"$AdcNotifications"!], /* AdcNotifications */[!// aAdcNotification is ordered by HW Unit. For example: {{Pointers of ADC0},{Pointers of ADC1},{Pointers of ADC2}}...
    [!"num:i($MaxTriggerModeGroupInPartition)"!]U, /* NumTrigConfigs */[!// Depend on number of channels in the group. The trigger list will be created in Adc_Ipw_EnableHardwareTrigger or generated directly to be used in Bctu_Ip_Init() when AdcCtuHardwareTriggerOptimization = true
    [!"$BctuIpInternalTriggersString"!], /* TrigConfigs */[!// Depend on number of channels in the group. The trigger list will be created in Adc_Ipw_EnableHardwareTrigger or generated directly to be used in Bctu_Ip_Init() when AdcCtuHardwareTriggerOptimization = true
    [!"num:i($NumBctuTrigGroupChan)"!]U, /* NumListItems */[!// Depend on number of channels in the group. The command list will be created in Adc_Ipw_EnableHardwareTrigger or generated directly to be used in Bctu_Ip_Init() when AdcCtuHardwareTriggerOptimization = true
    [!"$BctuIpListItemsString"!], /* ListItemConfigs */[!// Depend on number of channels in the group. The command list will be created in Adc_Ipw_EnableHardwareTrigger or generated directly to be used in Bctu_Ip_Init() when AdcCtuHardwareTriggerOptimization = true
    0U, /* NumFifoConfigs */[!// not used in trigger mode
    NULL_PTR /* FifoConfigs */[!// not used in trigger mode
};
[!ENDCODE!][!//
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDIF!]
        [!ENDFOR!][!// Macro End FOR 1
    [!ENDIF!]
[!ENDNOCODE!][!//
[!ENDMACRO!]

[!MACRO "BctuIpConfigArraysTriggerModeMacro"!]
[!NOCODE!][!//
    [!VAR "numBctuTriggerGroups" = "count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))])"!][!//
    [!IF "(AutosarExt/AdcCtuHardwareTriggerOptimization = 'true') and ($numBctuTriggerGroups > 0)"!]
        [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
            [!VAR "MaxPartitionCfg"="num:i(count(AdcGeneral/AdcEcucPartitionRef/*))"!][!//
        [!ELSE!][!//
            [!VAR "MaxPartitionCfg"="1"!][!//
        [!ENDIF!][!//

        [!VAR "CtuTrigListPos" = "0"!][!// Current position in BCTU/CTU list of group
        [!VAR "FirstCtuTrigGroup" = "0"!][!// First CTU/BCTU trigger mode groups encounterred
        [!FOR "Partition" = "0" TO "num:i($MaxPartitionCfg) - 1"!]
            [!VAR "LoopGroupPartition"="0"!][!//
            [!VAR "MaxTriggerModeGroupInPartition"="0"!][!//
            [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
                [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcGeneral/AdcEcucPartitionRef/*[@index = $Partition],'/'),'/'),'/'),'/')"!][!//
                [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
                    [!IF "substring-after(substring-after(substring-after(substring-after(AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/') = $CurrentPartition"!]
                        [!VAR "MaxTriggerModeGroupInPartition"="$MaxTriggerModeGroupInPartition + num:i(count(AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]))"!][!//
                    [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ELSE!][!//
                [!VAR "MaxTriggerModeGroupInPartition"="$MaxTriggerModeGroupInPartition + num:i(count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]))"!][!//
            [!ENDIF!][!//
            [!IF "$MaxTriggerModeGroupInPartition > 0"!]
[!CODE!][!//
/**
* @brief     Bctu Ip Internal Triggers Config for Trigger Mode configuration variant[!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*/
static const Bctu_Ip_TrigConfigType BctuIpTriggerModeInternalTriggers[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][] =
{
[!ENDCODE!]
                [!VAR "NumGroup" = "num:i(count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*)-1)"!][!//
                [!FOR "LoopVar" = "0" TO "$NumGroup"!][!//
                    [!LOOP "AdcConfigSet/AdcHwUnit/*/AdcGroup/*[AdcGroupId = $LoopVar]"!][!//
                        [!VAR "HWUnitPartition"="0"!][!//
                        [!IF "node:value(../../../../../AutosarExt/AdcMulticoreSupport)='true'"!]
                            [!IF "substring-after(substring-after(substring-after(substring-after(../../AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/') = $CurrentPartition"!][!//
                                [!VAR "HWUnitPartition"="1"!][!//
                            [!ENDIF!][!//
                        [!ELSE!][!//
                            [!VAR "HWUnitPartition"="2"!][!//
                        [!ENDIF!][!//

                        [!IF "(AdcGroupId = $LoopVar) and ($HWUnitPartition > 0)"!][!//
                            [!VAR "TempCtuTrigListPos" = "0"!]
                            [!IF "(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))"!][!//
                                [!VAR "TriggerValue"!][!CALL "GetHwResourceMacro" , "Source" = "node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc"!][!ENDVAR!]
                                [!IF "$FirstCtuTrigGroup = 0"!][!//
                                    [!VAR "FirstCtuTrigGroup" = "1"!]
                                [!ELSE!][!//
                                    [!VAR "CtuTrigListPos" = "$CtuTrigListPos + count(AdcGroupDefinition/*)"!]
                                    [!VAR "TempCtuTrigListPos" = "$CtuTrigListPos"!]
                                [!ENDIF!][!//
[!CODE!][!//
    {
        [!"normalize-space(num:i($TriggerValue))"!]U, /* TrigIndex - [!"node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc"!] */
        (boolean)FALSE, /* LoopEn */
        BCTU_IP_DATA_DEST_ADC_DATA_REG, /* DataDest */
        (boolean)FALSE, /* HwTriggersEn */
        BCTU_IP_TRIG_TYPE_LIST, /* TrigType */
        [!"num:i(bit:shl(1, substring-after(../../AdcHwUnitId, 'ADC')))"!]U, /* AdcTargetMask */
        [!"num:i($TempCtuTrigListPos)"!]U /* AdcChanOrListStart - Start index in BCTU LIST, because trigger is configured in LIST mode */
    }[!IF "($LoopGroupPartition < $MaxTriggerModeGroupInPartition - 1)"!],[!ENDIF!]
[!ENDCODE!]
                                [!VAR "LoopGroupPartition"="$LoopGroupPartition + 1"!]
                            [!ENDIF!][!//
                        [!ENDIF!][!//
                    [!ENDLOOP!][!//
                [!ENDFOR!][!//
[!CODE!][!//
};
[!ENDCODE!][!//

[!CODE!][!//

/**
* @brief     Bctu Ip Bctu List Items Config for Trigger Mode configuration variant[!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*/
static const Bctu_Ip_ListItemConfigType BctuIpTriggerModeListItems[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][] =
{
[!ENDCODE!]
                [!VAR "NumGroup" = "num:i(count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*)-1)"!][!//
                [!VAR "NumBctuTrigGroupChan"="0"!][!//
                [!VAR "ChannelListIndex"="0"!][!//
                [!// Loop for all CTU trigger mode groups and get sum of channels configured in these groups
                [!LOOP "AdcConfigSet/AdcHwUnit/*/AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]"!]
                    [!VAR "NumBctuTrigGroupChan"="$NumBctuTrigGroupChan + num:i(count(AdcGroupDefinition/*))"!]
                [!ENDLOOP!]
                [!FOR "LoopVar" = "0" TO "$NumGroup"!][!//
                    [!// Loop for all CTU trigger mode groups
                    [!LOOP "AdcConfigSet/AdcHwUnit/*/AdcGroup/*[AdcGroupId = $LoopVar]"!]
                        [!IF "(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))"!]
                            [!// Get channels of group into a string variable
                            [!VAR "ChanListString"="text:split(node:refs(AdcGroupDefinition/*[@index = 1])/AdcChannelName, 'ChanNum')[last()]"!][!//
                            [!LOOP "AdcGroupDefinition/*[@index != 1]"!][!//
                                    [!VAR "ChanListString"="concat($ChanListString,' ', text:split(node:refs(.)/AdcChannelName, 'ChanNum')[last()])"!][!//
                            [!ENDLOOP!]
                            [!// Max value channel ID is last element
                            [!VAR "MaxVal"="num:max(text:split($ChanListString))"!]
                            [!// Loop in ascending order of channel ids
                            [!FOR "Idx" = "1" TO "num:i(count(AdcGroupDefinition/*))"!][!//
                            [!VAR "CurSortedChanId"="num:order(text:split($ChanListString))[position() = $Idx]"!]
                                [!LOOP "AdcGroupDefinition/*"!]
                                [!VAR "CurChanId"="num:i(text:split(node:refs(.)/AdcChannelName, 'ChanNum')[last()])"!][!//
                                    [!IF "$CurChanId=$CurSortedChanId"!][!//
                                        [!VAR "LastChanString"="'FALSE'"!]
                                        [!IF "($CurChanId = $MaxVal)"!]
                                            [!VAR "LastChanString"="'TRUE'"!]
                                        [!ENDIF!][!//
[!CODE!][!//
    { /* Channel List Index: [!"num:i($ChannelListIndex)"!] */
        (boolean)FALSE, /* NextChanWaitOnTrig */
        [!"$CurChanId"!]U, /* AdcChanIndex */
        (boolean)[!"$LastChanString"!] /* LastChanInList */
    }[!IF "($ChannelListIndex < $NumBctuTrigGroupChan - 1)"!],[!ENDIF!]
[!ENDCODE!]
                                    [!ENDIF!][!//
                                [!ENDLOOP!][!//LOOP "AdcGroupDefinition/*"!][!//
                            [!VAR "ChannelListIndex"="$ChannelListIndex + 1"!]
                            [!ENDFOR!][!//FOR "index" = "1" TO "num:i(count(AdcGroupDefinition/*))"!][!//
                        [!ENDIF!][!//IF "(AdcGroupId = $LoopVar) and (AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))"!]
                    [!ENDLOOP!][!//LOOP "AdcConfigSet/AdcHwUnit/*/AdcGroup/*"!]
                [!ENDFOR!][!//FOR "LoopVar" = "0" TO "$NumGroup"!][!//
[!CODE!][!//
};
[!ENDCODE!]
            [!ENDIF!][!//
        [!ENDFOR!]
    [!ENDIF!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!// Macro for BCTU Control Mode
[!MACRO "BctuIpConfigControlModeMacro" , "MacroGenerationType"!][!// GenerationType='definition' or 'declaration'
[!NOCODE!][!//
    [!IF "AutosarExt/AdcEnableCtuControlModeApi"!]
        [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
            [!VAR "MaxPartitionCfg"="num:i(count(AdcGeneral/AdcEcucPartitionRef/*))"!][!//
        [!ELSE!][!//
            [!VAR "MaxPartitionCfg"="1"!][!//
        [!ENDIF!][!//

        [!FOR "Partition" = "0" TO "num:i($MaxPartitionCfg) - 1"!][!//
            [!VAR "MaxHWUnitInPartition"="0"!][!//
            [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
                [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcGeneral/AdcEcucPartitionRef/*[@index = $Partition],'/'),'/'),'/'),'/')"!][!//
                [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
                    [!IF "substring-after(substring-after(substring-after(substring-after(AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/') = $CurrentPartition"!]
                        [!VAR "MaxHWUnitInPartition"="$MaxHWUnitInPartition + 1"!][!//
                    [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ELSE!][!//
                [!VAR "MaxHWUnitInPartition"="num:i(count(AdcConfigSet/AdcHwUnit/*))"!][!//
            [!ENDIF!][!//
            [!IF "$MacroGenerationType = 'declaration'"!][!//
[!CODE!][!//
/**
* @brief     Bctu Ip Config for Control Mode configuration variant[!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!] [!"$CurrentPartition"!][!ENDIF!].
*/
extern const Bctu_Ip_ConfigType BctuIpConfigControlMode[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!];
[!ENDCODE!][!//
            [!ELSE!]
[!CODE!][!//
/**
* @brief     Bctu Ip Config for Control Mode variant[!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!] [!"$CurrentPartition"!][!ENDIF!].
*/
const Bctu_Ip_ConfigType BctuIpConfigControlMode[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!] =
{
[!ENDCODE!][!//
                [!LOOP "AdcConfigSet/BctuHwUnit/*"!][!// Only 1 BCTU instance is currently supported
                    [!VAR "ArrayLikeString" = "''"!]
                    [!FOR "Unit" = "0" TO "num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit')) - 1"!]
                        [!VAR "ArrayLikeString" = "concat($ArrayLikeString, 'ADC', $Unit,':position')"!]
                        [!VAR "Position" = "'X'"!]
                        [!LOOP "BctuAdcNotifications/*"!]
                            [!IF "node:ref(BctuAdcNotificationsAdcIndex)/AdcHwUnitId = concat('ADC',$Unit)"!]
                                [!VAR "Position" = "@index"!]
                            [!ENDIF!]
                        [!ENDLOOP!]
                        [!VAR "ArrayLikeString" = "concat($ArrayLikeString, $Position, '; ')"!]
                    [!ENDFOR!]
                    [!// Example of ArrayLikeString: "ADC0:position1; ADC1:positionX; ADC2:position0;", where X corresponds to NotFound
                    [!VAR "AdcNotificationsString" = "'{ '"!]
                    [!FOR "HwUnitIndex" = "0" TO "num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit')) - 1"!]
                        [!IF "($HwUnitIndex < num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit')) - 1)"!][!//
                            [!VAR "Comma" = "', '"!]
                        [!ELSE!]
                            [!VAR "Comma" = "' '"!]
                        [!ENDIF!]
                        [!VAR "AdcNotificationsSubstring" = "'{ NULL_PTR, NULL_PTR, NULL_PTR }'"!]
                        [!VAR "Position" = "substring-before(substring-after($ArrayLikeString, concat('ADC',$HwUnitIndex,':position')),';')"!]
                        [!IF "$Position != 'X'"!]
                            [!LOOP "BctuAdcNotifications/*"!]
                                [!IF "node:ref(BctuAdcNotificationsAdcIndex)/AdcHwUnitId = concat('ADC',$HwUnitIndex)"!]
                                    [!VAR "AdcNotificationsSubstring" = "concat('{ ', BctuAdcNewDataNotification, ', ', BctuDataOverrunNotification, ', ', BctuListLastConversionNotification, ' }')"!]
                                [!ENDIF!]
                            [!ENDLOOP!]
                        [!ENDIF!]
                        [!VAR "AdcNotificationsString" = "concat($AdcNotificationsString, $AdcNotificationsSubstring, $Comma)"!]
                    [!ENDFOR!]
                    [!VAR "AdcNotificationsString" = "concat($AdcNotificationsString, '}')"!]

                    [!IF "count(BctuInternalTrigger/*) > 0"!][!//
                        [!VAR "BctuIpInternalTriggersString"!]BctuIpControlModeInternalTriggers[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][!ENDVAR!]
                    [!ELSE!]
                        [!VAR "BctuIpInternalTriggersString" = "'NULL_PTR'"!][!//
                    [!ENDIF!]
                    [!IF "count(BctuListItems/*) > 0"!][!//
                        [!VAR "BctuIpListItemsString"!]BctuIpControlModeListItems[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][!ENDVAR!]
                    [!ELSE!]
                        [!VAR "BctuIpListItemsString" = "'NULL_PTR'"!][!//
                    [!ENDIF!]
                    [!IF "count(BctuResultFifos/*) > 0"!][!//
                        [!VAR "BctuIpResultFifosString"!]BctuIpResultFifos[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(../../../AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][!ENDVAR!]
                    [!ELSE!]
                        [!VAR "BctuIpResultFifosString" = "'NULL_PTR'"!][!//
                    [!ENDIF!]
  [!CODE!][!//
    (boolean)[!"text:toupper(node:value(BctuLowPowerMode))"!], /* LowPowerModeEn */
    (boolean)[!"text:toupper(node:value(BctuGlobalHwTriggers))"!], /* GlobalHwTriggersEn */
    [!"BctuNewDataDMAEnableMask"!]U, /* NewDataDmaEnMask */
    [!"BctuTriggerNotification"!], /* TriggerNotification */
    [!"$AdcNotificationsString"!], /* AdcNotifications */
    [!"num:i(count(BctuInternalTrigger/*))"!]U, /* NumTrigConfigs */
    [!"$BctuIpInternalTriggersString"!], /* TrigConfigs */
    [!"num:i(count(BctuListItems/*))"!]U, /* NumListItems */
    [!"$BctuIpListItemsString"!], /* ListItemConfigs */
    [!"num:i(count(BctuResultFifos/*))"!]U, /* NumFifoConfigs */
    [!"$BctuIpResultFifosString"!] /* FifoConfigs */
};
[!ENDCODE!][!//
                [!ENDLOOP!]
            [!ENDIF!]
        [!ENDFOR!]
    [!ENDIF!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//


[!MACRO "BctuIpConfigArraysControlModeMacro"!]
[!NOCODE!][!//
    [!IF "AutosarExt/AdcEnableCtuControlModeApi"!]
        [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
            [!VAR "MaxPartitionCfg"="num:i(count(AdcGeneral/AdcEcucPartitionRef/*))"!][!//
        [!ELSE!][!//
            [!VAR "MaxPartitionCfg"="1"!][!//
        [!ENDIF!][!//

        [!FOR "Partition" = "0" TO "num:i($MaxPartitionCfg) - 1"!][!//
            [!VAR "MaxHWUnitInPartition"="0"!][!//
            [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
                [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcGeneral/AdcEcucPartitionRef/*[@index = $Partition],'/'),'/'),'/'),'/')"!][!//
                [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
                    [!IF "substring-after(substring-after(substring-after(substring-after(AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/') = $CurrentPartition"!]
                        [!VAR "MaxHWUnitInPartition"="$MaxHWUnitInPartition + 1"!][!//
                [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ELSE!][!//
                [!VAR "MaxHWUnitInPartition"="num:i(count(AdcConfigSet/AdcHwUnit/*))"!][!//
            [!ENDIF!][!//
            [!IF "count(AdcConfigSet/BctuHwUnit/*/BctuInternalTrigger/*) > 0"!][!//
[!CODE!][!//
/**
* @brief     Bctu Ip Internal Triggers Config for Control Mode configuration variant[!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*/
static const Bctu_Ip_TrigConfigType BctuIpControlModeInternalTriggers[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][] =
{
[!ENDCODE!]
                [!LOOP "AdcConfigSet/BctuHwUnit/*"!][!// Only 1 BCTU instance is currently supported
                    [!VAR "MaxNum"="count(BctuInternalTrigger/*)"!]
                    [!VAR "CrtNum"="0"!]
                    [!FOR "CrtNum" = "0" TO "$MaxNum"!]
                        [!LOOP "BctuInternalTrigger/*"!]
                            [!IF "@index = $CrtNum"!][!//
[!CODE!][!//
    {
[!ENDCODE!]
                        [!VAR "TriggerValue"!][!CALL "GetHwResourceMacro" , "Source" = "node:ref(BctuTriggerSource)/AdcHwTrigSrc"!][!ENDVAR!]
                        [!VAR "TriggerSourceName" = "substring-after(substring-after(substring-after(substring-after(BctuTriggerSource,'/'),'/'),'/'),'/')"!]
                        [!IF "BctuTriggerConversionMode = 'SINGLE'"!]
                            [!VAR "AdcChanOrListStartString" = "num:i(text:split(node:ref(BctuAdcChannelSingle)/AdcChannelName, 'ChanNum')[last()])"!]
                            [!VAR "AdcChanOrListStartStringComment" = "'/* AdcChanOrListStart - Adc channel ID, because trigger is configured in single mode */'"!]
                        [!ELSE!]
                            [!VAR "AdcChanOrListStartString" = "BctuConversionListStartIndex"!]
                            [!VAR "AdcChanOrListStartStringComment" = "'/* AdcChanOrListStart - Start index in BCTU LIST, because trigger is configured in LIST mode */'"!]
                        [!ENDIF!]
[!CODE!][!//
        [!"normalize-space(num:i($TriggerValue))"!]U, /* TrigIndex - [!"$TriggerSourceName"!] */
        (boolean)[!"text:toupper(BctuTriggerLoop)"!], /* LoopEn */
        [!"concat('BCTU_IP_DATA_DEST_', substring-after(BctuDataDestination, 'BCTU_'))"!], /* DataDest */
        (boolean)[!"text:toupper(BctuHwTriggerEnable)"!], /* HwTriggersEn */
        [!"concat('BCTU_IP_TRIG_TYPE_', BctuTriggerConversionMode)"!], /* TrigType */
        [!"BctuAdcTargetMask"!]U, /* AdcTargetMask */
        [!"$AdcChanOrListStartString"!]U [!"$AdcChanOrListStartStringComment"!]
[!IF "$CrtNum < $MaxNum - 1"!]    },[!//
[!ELSE!]    }[!//
[!ENDIF!]
[!ENDCODE!]
                            [!ENDIF!]
                        [!ENDLOOP!]
                    [!ENDFOR!]
                [!ENDLOOP!]
[!CODE!][!//
};
[!ENDCODE!][!//
            [!ENDIF!]
            [!IF "count(AdcConfigSet/BctuHwUnit/*/BctuListItems/*) > 0"!][!//
[!CODE!][!//

/**
* @brief     Bctu Ip Bctu List Items Config for Control Mode configuration variant[!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*/
static const Bctu_Ip_ListItemConfigType BctuIpControlModeListItems[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][] =
{
[!ENDCODE!]
                [!LOOP "AdcConfigSet/BctuHwUnit/*"!][!// Only 1 BCTU instance is currently supported
                    [!VAR "MaxNum"="count(BctuListItems/*)"!]
                    [!VAR "CrtNum"="0"!]
                    [!FOR "CrtNum" = "0" TO "$MaxNum"!][!//
                        [!LOOP "BctuListItems/*"!]
                            [!IF "@index = $CrtNum"!][!//
[!CODE!][!//
    { /* Command List Index: [!"@index"!] - [!"@name"!] */
        (boolean)[!"text:toupper(BctuNextChannelWaitOnTrig)"!], /* NextChanWaitOnTrig */
        [!"num:i(text:split(BctuAdcChannelList, 'ChanNum')[last()])"!]U, /* AdcChanIndex */
        (boolean)[!"text:toupper(BctuLastChannel)"!] /* LastChanInList */
[!IF "$CrtNum < $MaxNum - 1"!]    },[!//
[!ELSE!]    }[!//
[!ENDIF!]
[!ENDCODE!]
                                [!BREAK!][!//
                            [!ENDIF!]
                        [!ENDLOOP!]
                    [!ENDFOR!]
                [!ENDLOOP!]
[!CODE!][!//
};
[!ENDCODE!]
            [!ENDIF!]
            [!IF "count(AdcConfigSet/BctuHwUnit/*/BctuResultFifos/*) > 0"!][!//
[!CODE!][!//

/**
* @brief     Bctu Ip Bctu Fifos Config for Control Mode configuration variant[!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!][!"$CurrentPartition"!][!ENDIF!].
*/
static const Bctu_Ip_FifoConfigType BctuIpResultFifos[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]_[!"$CurrentPartition"!][!ENDIF!][] =
{
[!ENDCODE!]
                [!LOOP "AdcConfigSet/BctuHwUnit/*"!][!// Only 1 BCTU instance is currently supported
                    [!VAR "MaxNum"="count(BctuResultFifos/*)"!]
                    [!VAR "CrtNum"="0"!]
                    [!FOR "CrtNum" = "0" TO "$MaxNum"!]
                        [!LOOP "BctuResultFifos/*"!]
                            [!VAR "FifoDmaChannelString" = "'BCTU_IP_INVALID_DMA_CHANNEL_ID'"!]
                            [!VAR "UserFifoBufferString" = "'NULL_PTR'"!]
                            [!IF "node:refs('ASPathDataOfSchema:/TS_T40D34M20I1R0/Adc')/AutosarExt/CtuEnableDmaTransferMode = 'true'"!]
                                [!IF "BctuFifoDmaEnable = 'true' and node:exists(BctuFifoDmaChannelId) and node:refvalid(BctuFifoDmaChannelId)"!]
                                    [!VAR "FifoDmaChannelString" = "node:ref(BctuFifoDmaChannelId)/dmaLogicChannel_LogicName"!][!//Get Dma Channel Id from Mcl
                                    [!VAR "UserFifoBufferString" = "BctuFifoDmaBuffer"!]
                                [!ENDIF!]
                            [!ENDIF!]
                            [!IF "@index = $CrtNum"!][!//
[!CODE!][!//
    /* Configuration for [!"BctuResultFifoIndex"!] */
    {
        [!"num:i(num:i(substring-after(BctuResultFifoIndex,'FIFO_')) - 1)"!]U, /* FifoIndex */
        [!"BctuWatermarkValue"!]U, /* WatermarkValue */
        (boolean)[!"text:toupper(BctuFifoDmaEnable)"!], /* FifoDmaEnable */
        [!IF "(BctuFifoDmaEnable = 'true')"!][!"$FifoDmaChannelString"!][!ELSE!]0U[!ENDIF!], /* FifoDmaChannel */
        [!IF "(BctuFifoDmaEnable = 'true')"!][!"$UserFifoBufferString"!][!ELSE!]NULL_PTR[!ENDIF!], /* UserFifoBuffer */
        (boolean)[!"text:toupper(BctuFifoNotificationsEnable)"!], /* bEnNotif */
        [!IF "(BctuFifoNotificationsEnable = 'true' or BctuFifoDmaEnable = 'true')"!][!"BctuWatermarkNotification"!][!ELSE!]NULL_PTR[!ENDIF!], /* WatermarkNotification */
        [!IF "(BctuFifoNotificationsEnable = 'true')"!][!"BctuUnderrunNotification"!][!ELSE!]NULL_PTR[!ENDIF!], /* UnderrunNotification */
        [!IF "(BctuFifoNotificationsEnable = 'true')"!][!"BctuOverrunNotification"!][!ELSE!]NULL_PTR[!ENDIF!] /* OverrunNotification */
[!IF "$CrtNum < $MaxNum - 1"!]    },[!//
[!ELSE!]    }[!//
[!ENDIF!]
[!ENDCODE!][!//
                            [!ENDIF!]
                        [!ENDLOOP!]
                    [!ENDFOR!]
                [!ENDLOOP!]
[!CODE!][!//
};
[!ENDCODE!]
            [!ENDIF!]
        [!ENDFOR!]
    [!ENDIF!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//




[!MACRO "BctuIpUserBuffersMacro", "MacroGenerationType"!][!//
[!NOCODE!][!//
  [!IF "AutosarExt/AdcEnableCtuControlModeApi"!][!// If Control Mode is enabled
    [!IF "num:i(count(AdcConfigSet/BctuHwUnit/*/BctuResultFifos/*[BctuFifoDmaEnable = 'true'])) > 0"!][!// Generate only when have at least one FIFO use DMA.
    [!LOOP "AdcConfigSet/BctuHwUnit/*"!][!// Only 1 CTU instance is currently supported
      [!VAR "BufferType" = "'uint16'"!][!//
      [!IF "BctuFifoDmaRawData"!]
        [!VAR "BufferType" = "'uint32'"!][!//
      [!ENDIF!]
    [!ENDLOOP!]
    [!IF "$BufferType = 'uint32'"!]
[!CODE!][!//
#define ADC_START_SEC_VAR_CLEARED_32_NO_CACHEABLE
#include "Adc_MemMap.h"
[!ENDCODE!][!//
    [!ELSE!]
[!CODE!][!//
#define ADC_START_SEC_VAR_CLEARED_16_NO_CACHEABLE
#include "Adc_MemMap.h"
[!ENDCODE!][!//
    [!ENDIF!]
[!CODE!][!//
/*
* @brief        Bctu User Dma Fifo Buffers.
* @details      Dma will transfer data from Fifo to this buffer.
                This is Raw Data from Fifo, user should mask to get expected value.
*/
[!ENDCODE!][!//
      [!LOOP "AdcConfigSet/BctuHwUnit/*"!][!// Only 1 BCTU instance is currently supported
        [!LOOP "BctuResultFifos/*"!][!//
[!CODE!][!//
[!IF "BctuFifoDmaEnable = 'true'"!][!//
[!IF "$MacroGenerationType = 'declaration'"!]extern [!ENDIF!][!"$BufferType"!] [!"BctuFifoDmaBuffer"!][[!"num:i(BctuWatermarkValue + 1)"!]]; /* Uses to store raw data from [!"BctuResultFifoIndex"!] */
[!ENDIF!][!//
[!ENDCODE!][!//
        [!ENDLOOP!]
      [!ENDLOOP!]
      [!IF "$BufferType = 'uint32'"!]
[!CODE!][!//
#define ADC_STOP_SEC_VAR_CLEARED_32_NO_CACHEABLE
#include "Adc_MemMap.h"
[!ENDCODE!][!//
    [!ELSE!]
[!CODE!][!//
#define ADC_STOP_SEC_VAR_CLEARED_16_NO_CACHEABLE
#include "Adc_MemMap.h"
[!ENDCODE!][!//
      [!ENDIF!]
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDNOCODE!]
[!ENDMACRO!][!//



[!MACRO "BctuControlModeNotificationsMacro"!]
[!NOCODE!][!//
    [!IF "AutosarExt/AdcEnableCtuControlModeApi"!]
        [!VAR "MaxPartitionCfg"="1"!][!//

        [!FOR "Partition" = "0" TO "num:i($MaxPartitionCfg) - 1"!][!//
            [!VAR "MaxHWUnitInPartition"="0"!][!//
            [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
                [!VAR "CurrentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcGeneral/AdcEcucPartitionRef/*[@index = $Partition],'/'),'/'),'/'),'/')"!][!//
                [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
                    [!IF "substring-after(substring-after(substring-after(substring-after(AdcHwUnitEcucPartitionRef/*[1],'/'),'/'),'/'),'/') = $CurrentPartition"!]
                        [!VAR "MaxHWUnitInPartition"="$MaxHWUnitInPartition + 1"!][!//
                [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ELSE!][!//
                [!VAR "MaxHWUnitInPartition"="num:i(count(AdcConfigSet/AdcHwUnit/*))"!][!//
            [!ENDIF!][!//

            [!VAR "BctuControlNotifArr" = "''"!][!//
            [!LOOP "AdcConfigSet/BctuHwUnit/*"!]

                [!IF "(BctuTriggerNotification != 'NULL_PTR') and (BctuTriggerNotification != 'NULL') and (BctuTriggerNotification != '')"!]
                    [!IF "(text:indexOf(text:split($BctuControlNotifArr, ','), node:value(BctuTriggerNotification)) = -1)"!]
                        [!VAR "BctuControlNotifArr" = "concat($BctuControlNotifArr, BctuTriggerNotification, ',')"!][!//
                    [!ENDIF!][!//
                [!ENDIF!]

                [!LOOP "BctuAdcNotifications/*"!]
                    [!IF "(BctuAdcNewDataNotification != 'NULL_PTR') and (BctuAdcNewDataNotification != 'NULL') and (BctuAdcNewDataNotification != '')"!]
                        [!IF "(text:indexOf(text:split($BctuControlNotifArr, ','), node:value(BctuAdcNewDataNotification)) = -1)"!]
                            [!VAR "BctuControlNotifArr" = "concat($BctuControlNotifArr, BctuAdcNewDataNotification, ',')"!][!//
                        [!ENDIF!][!//
                    [!ENDIF!]

                    [!IF "(BctuDataOverrunNotification != 'NULL_PTR') and (BctuDataOverrunNotification != 'NULL') and (BctuDataOverrunNotification != '')"!]
                        [!IF "(text:indexOf(text:split($BctuControlNotifArr, ','), node:value(BctuDataOverrunNotification)) = -1)"!]
                            [!VAR "BctuControlNotifArr" = "concat($BctuControlNotifArr, BctuDataOverrunNotification, ',')"!][!//
                        [!ENDIF!][!//
                    [!ENDIF!]

                    [!IF "(BctuListLastConversionNotification != 'NULL_PTR') and (BctuListLastConversionNotification != 'NULL') and (BctuListLastConversionNotification != '')"!]
                        [!IF "(text:indexOf(text:split($BctuControlNotifArr, ','), node:value(BctuListLastConversionNotification)) = -1)"!]
                            [!VAR "BctuControlNotifArr" = "concat($BctuControlNotifArr, BctuListLastConversionNotification, ',')"!][!//
                        [!ENDIF!][!//
                    [!ENDIF!]
                [!ENDLOOP!]

                [!LOOP "BctuResultFifos/*"!]
                    [!IF "((BctuWatermarkNotification != 'NULL_PTR') and (BctuWatermarkNotification != 'NULL') and (BctuWatermarkNotification != '') and (BctuFifoNotificationsEnable = 'true' or BctuFifoDmaEnable = 'true'))"!]
                        [!IF "(text:indexOf(text:split($BctuControlNotifArr, ','), node:value(BctuWatermarkNotification)) = -1)"!]
                            [!VAR "BctuControlNotifArr" = "concat($BctuControlNotifArr, BctuWatermarkNotification, ',')"!][!//
                        [!ENDIF!][!//
                    [!ENDIF!]
                    [!IF "((BctuUnderrunNotification != 'NULL_PTR') and (BctuUnderrunNotification != 'NULL') and (BctuUnderrunNotification != '') and (BctuFifoNotificationsEnable = 'true'))"!]
                        [!IF "(text:indexOf(text:split($BctuControlNotifArr, ','), node:value(BctuUnderrunNotification)) = -1)"!]
                            [!VAR "BctuControlNotifArr" = "concat($BctuControlNotifArr, BctuUnderrunNotification, ',')"!][!//
                        [!ENDIF!][!//
                    [!ENDIF!]
                    [!IF "((BctuOverrunNotification != 'NULL_PTR') and (BctuOverrunNotification != 'NULL') and (BctuOverrunNotification != '') and (BctuFifoNotificationsEnable = 'true'))"!]
                        [!IF "(text:indexOf(text:split($BctuControlNotifArr, ','), node:value(BctuOverrunNotification)) = -1)"!]
                            [!VAR "BctuControlNotifArr" = "concat($BctuControlNotifArr, BctuOverrunNotification, ',')"!][!//
                        [!ENDIF!][!//
                    [!ENDIF!][!//
                [!ENDLOOP!][!//

            [!ENDLOOP!][!//

            [!IF "($BctuControlNotifArr != '')"!]
[!CODE!][!//
/**
* @brief     Bctu Control Mode Notification function prototypes.
*/
[!ENDCODE!]
                [!LOOP "text:split($BctuControlNotifArr, ',')"!][!//
[!CODE!][!//
void [!"."!](void);
[!ENDCODE!][!//
                [!ENDLOOP!][!//
            [!ENDIF!][!//

        [!ENDFOR!][!//
    [!ENDIF!][!//

[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetAdcVoltageReferencesMacro"!][!// Get Adc voltage references for TempSense
[!NOCODE!][!//
    [!VAR "AdcVoltageReference"="''"!]
    [!FOR "Unit" = "0" TO "num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit'))-1"!][!//
        [!VAR "AdcVoltageReferenceFound"="0"!]
        [!LOOP "AdcConfigSet/AdcHwUnit/*"!]
            [!IF "AdcHwUnitId = concat('ADC',string($Unit))"!]
                [!VAR "AdcVoltageReference" = "concat($AdcVoltageReference, node:value(AdcSarHwUnitVref), 'U')"!]
                [!VAR "AdcVoltageReferenceFound"="1"!]
            [!ENDIF!]
        [!ENDLOOP!]
        [!IF "$AdcVoltageReferenceFound = 0"!]
            [!VAR "AdcVoltageReference" = "concat($AdcVoltageReference, '0U')"!]
        [!ENDIF!]
        [!IF "$Unit < (num:i(ecu:get('Adc.AdcConfigSet.AdcHwUnit'))-1)"!]
            [!VAR "AdcVoltageReference" = "concat($AdcVoltageReference, ', ')"!]
        [!ENDIF!]
    [!ENDFOR!]
    [!CODE!]const uint16 AdcVoltageRefs[ADC_INSTANCE_COUNT] = { [!"$AdcVoltageReference"!] };
[!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!]

[!MACRO "AdcSarIpCfgDefinesMacro"!][!//
[!NOCODE!][!//
[!VAR "EnableEndOfNormalChainString" = "'STD_OFF'"!]
[!VAR "EnableEndOfInjectedChainString" = "'STD_OFF'"!]
[!LOOP "AdcConfigSet/AdcHwUnit/*"!]
  [!VAR "AdcPhysicalId" = "substring-after(AdcHwUnitId,'ADC')"!][!//
  [!IF "AdcTransferType = 'ADC_INTERRUPT'"!]
    [!LOOP "AdcGroup/*[AdcWithoutInterrupts = 'false']"!]
      [!IF "AdcGroupConversionType = 'ADC_CONV_TYPE_NORMAL'"!]
        [!VAR "EnableEndOfNormalChainString" = "'STD_ON'"!]
        [!BREAK!]
      [!ENDIF!]
    [!ENDLOOP!]
    [!LOOP "AdcGroup/*[AdcWithoutInterrupts = 'false']"!]
      [!IF "AdcGroupConversionType = 'ADC_CONV_TYPE_INJECTED'"!]
        [!VAR "EnableEndOfInjectedChainString" = "'STD_ON'"!]
        [!BREAK!]
      [!ENDIF!]
    [!ENDLOOP!]
  [!ENDIF!][!// End of checking TransferType is ADC_INTERRUPT
[!ENDLOOP!][!// End of loop all HW Units configured
[!CODE!][!//
#define ADC_SAR_IP_ECH_ENABLED                ([!"$EnableEndOfNormalChainString"!])
#define ADC_SAR_IP_JECH_ENABLED               ([!"$EnableEndOfInjectedChainString"!])
[!ENDCODE!][!//
[!ENDNOCODE!][!//
#define ADC_SAR_IP_EOCTU_ENABLED              [!IF "node:value(AutosarExt/AdcEnableCtuTrigAutosarExtApi)='true' and node:value(AdcGeneral/AdcHwTriggerApi)='true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define ADC_SAR_IP_EOC_ENABLED                (STD_OFF)
#define ADC_SAR_IP_WDG_ENABLED                [!IF "AutosarExt/AdcEnableWatchdogApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define ADC_SAR_IP_DEV_ERROR_DETECT           [!IF "AutosarExt/AdcSarIpDevErrorDetect"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define ADC_SAR_IP_TIMEOUT_TYPE               ([!"AutosarExt/AdcTimeoutMethod"!])
#define ADC_SAR_IP_TIMEOUT_VAL                ([!"AutosarExt/AdcTimeoutVal"!]UL)
[!IF "AutosarExt/AdcEnableUserModeSupport"!][!//
#define ADC_SAR_IP_ENABLE_USER_MODE_SUPPORT   [!IF "ecu:get('AdcAdcSarRegProtAvailable') = 'TRUE'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
[!ELSE!][!//
#define ADC_SAR_IP_ENABLE_USER_MODE_SUPPORT   (STD_OFF)
[!ENDIF!][!//
#define ADC_SAR_IP_SET_RESOLUTION             [!IF "ecu:get('AdcSetResolutionAvailable') = 'TRUE'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define ADC_SAR_IP_TEMPSENSE_ENABLED          [!IF "AutosarExt/AdcSarEnableTempsenseApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define ADC_SAR_IP_TEMPSENSE_VOLTAGE_SUPPLY   ([!"AutosarExt/AdcSarTempSenseVsupply"!]U)

#define FEATURE_ADC_HAS_HIGH_SPEED_ENABLE     [!IF "ecu:get('AdcHasHighSpeedEnable') = 'TRUE'"!](1U)[!ELSE!](0U)[!ENDIF!]
#define FEATURE_ADC_HAS_EXT_TRIGGER           [!IF "ecu:get('AdcHasExtTrig') = 'TRUE'"!](1U)[!ELSE!](0U)[!ENDIF!]
#define FEATURE_ADC_HAS_INJ_EXT_TRIGGER       [!IF "ecu:get('AdcHasExtInjTrig') = 'TRUE'"!](1U)[!ELSE!](0U)[!ENDIF!]
#define FEATURE_ADC_HAS_AUX_EXT_TRIGGER       [!IF "ecu:get('AdcHasAuxExtTrig') = 'TRUE'"!](1U)[!ELSE!](0U)[!ENDIF!]
#define FEATURE_ADC_HAS_CLOCK_DIVIDER         [!IF "ecu:get('AdcClockDividerModuleAvailable') = 'TRUE'"!](1U)[!ELSE!](0U)[!ENDIF!]
#define FEATURE_ADC_HAS_CALIBRATION           [!IF "ecu:get('AdcCalibrationAvailable') = 'TRUE'"!](1U)[!ELSE!](0U)[!ENDIF!]
#define FEATURE_ADC_HAS_PRESAMPLING           [!IF "ecu:get('AdcPresamplingAvailable') = 'TRUE'"!](1U)[!ELSE!](0U)[!ENDIF!]
#define FEATURE_ADC_HAS_CONVERSION_TIMING     [!IF "ecu:get('AdcConvTimingAvailable') = 'TRUE'"!](1U)[!ELSE!](0U)[!ENDIF!]
[!ENDMACRO!][!//


[!MACRO "BctuIpCfgDefinesMacro"!][!//
#define BCTU_IP_DEV_ERROR_DETECT                 [!IF "AutosarExt/BctuIpDevErrorDetect"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define BCTU_IP_TIMEOUT_TYPE                     ([!"AutosarExt/AdcTimeoutMethod"!])
#define BCTU_IP_TIMEOUT_VAL                      ([!"AutosarExt/AdcTimeoutVal"!]UL)
[!ENDMACRO!][!//

/** @} */
[!ENDIF!]
