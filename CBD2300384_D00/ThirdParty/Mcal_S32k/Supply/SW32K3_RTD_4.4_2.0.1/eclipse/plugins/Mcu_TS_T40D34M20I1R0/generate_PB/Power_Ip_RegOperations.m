[!NOCODE!]
/**
    @file    Power_Ip_RegOperations.m
    @version 2.0.1

    @brief   AUTOSAR Mcu - Register operations.
    @details Register operations.

    Project RTD AUTOSAR 4.4
    Platform CORTEXM
    Peripheral 
    Dependencies none

    ARVersion 4.4.0
    ARRevision ASR_REL_4_4_REV_0000
    ARConfVariant
    SWVersion 2.0.1
    BuildVersion S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707

    (c) Copyright 2020 - 2022 NXP Semiconductors
    All Rights Reserved.
 */


[!MACRO "NumberOfCores", "ParamPrtnIndex"!][!//
    [!VAR "Mac_ResourceName" = "concat('MCU.MC_ME.Partition', $ParamPrtnIndex, '.Cores.List')"!]
    [!VAR "Mac_Count" = "num:i(count(ecu:list($Mac_ResourceName)))"!]
    [!"$Mac_Count"!]
[!ENDMACRO!][!//NumberOfCores


[!MACRO "Power_Ip_LinkerSymbols","Type"!][!//
[!NOCODE!][!//
    [!VAR "Mac_SymbolTable" = "''"!]
    [!VAR "Mac_NumOfPartitions" = "count(ecu:list('MCU.Partitions.List'))"!]
    [!LOOP "McuModuleConfiguration/McuModeSettingConf/*"!]
        [!FOR "PartitionCounter" = "1" TO "$Mac_NumOfPartitions"!]
            [!VAR "Mac_PartitionEffectiveIndex" = "num:i(substring-after(ecu:list('MCU.Partitions.List')[number($PartitionCounter)], 'Partition'))"!]
            [!VAR "Mac_CurrentPartitionPrefixStr" = "concat('McuPartitionConfiguration/McuPartition', $Mac_PartitionEffectiveIndex, 'Config')"!]
            [!SELECT "node:ref($Mac_CurrentPartitionPrefixStr)"!]
                [!VAR "Mac_PartitionCoresListResName" = "concat('MCU.MC_ME.Partition', $Mac_PartitionEffectiveIndex ,'.Cores.List')"!]
                [!VAR "Mac_NumOfCoresInPartition" = "count(ecu:list($Mac_PartitionCoresListResName))"!]
                [!FOR "CoreCounter" = "1" TO "$Mac_NumOfCoresInPartition"!]
                    [!VAR "Mac_CurrentCoreEffectiveIndex" = "substring-after(ecu:list($Mac_PartitionCoresListResName)[num:i($CoreCounter)], 'CORE_')"!]
                    [!VAR "Mac_CurrentCorePrefixStr" = "concat('McuCore', $Mac_CurrentCoreEffectiveIndex, 'Configuration')"!]
                    [!SELECT "node:ref($Mac_CurrentCorePrefixStr)"!]
                        [!IF "node:exists(McuCoreBootAddressLinkerSym) and (McuCoreBootAddressLinkerSym != '')"!]
                            [!VAR "Mac_Extern_Core_BAL" = "concat('extern uint32 ', McuCoreBootAddressLinkerSym, '[1U];')"!]
                            [!IF "not(text:contains(text:split($Mac_SymbolTable), McuCoreBootAddressLinkerSym))"!]
[!CODE!][!//
[!"$Mac_Extern_Core_BAL"!]
[!ENDCODE!][!//
                            [!VAR "Mac_SymbolTable" = "concat($Mac_SymbolTable, ' ', McuCoreBootAddressLinkerSym)"!]
                            [!ENDIF!]
                        [!ENDIF!]
                    [!ENDSELECT!][!//node:ref($Mac_CurrentCorePrefixStr)
                [!ENDFOR!][!//CoreCounter
            [!ENDSELECT!][!//node:ref($Mac_CurrentPartitionPrefixStr)
        [!ENDFOR!][!//PartitionCounter
    [!ENDLOOP!][!//McuModuleConfiguration/McuModeSettingConf/*
[!ENDNOCODE!][!//
[!ENDMACRO!][!//


[!MACRO "Power_Ip_MC_ME_Core_Config","Type"!][!//
[!NOCODE!]
    [!VAR "Mac_NumOfPartitions" = "count(ecu:list('MCU.Partitions.List'))"!]
    [!FOR "PartitionCounter" = "1" TO "$Mac_NumOfPartitions"!]
        [!VAR "Mac_PartitionEffectiveIndex" = "num:i(substring-after(ecu:list('MCU.Partitions.List')[number($PartitionCounter)], 'Partition'))"!]
        [!VAR "Mac_CurrentPartitionPrefixStr" = "concat('McuPartition', $Mac_PartitionEffectiveIndex, 'Config')"!]
        [!VAR "Mac_CoreResourcePath" = "concat('MCU.MC_ME.Partition', $Mac_PartitionEffectiveIndex, '.Cores.List')"!]
        [!LOOP "McuModuleConfiguration/McuModeSettingConf/*"!]
            [!VAR "Mac_ModeConfigIndex" = "@index"!]
            [!VAR "Mac_NumOfCores"!][!CALL "NumberOfCores", "ParamPrtnIndex" = "$Mac_PartitionEffectiveIndex"!][!ENDVAR!]
            [!IF "num:i($Mac_NumOfCores) > 0"!]
                [!VAR "Mac_FirstCore" = "1"!]
[!CODE!][!//
static const Power_Ip_MC_ME_CoreConfigType Power_Ip_MC_ME_aPartition[!"$Mac_PartitionEffectiveIndex"!]CoreConfig[!"$Type"!]_[!"num:i($Mac_ModeConfigIndex)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($Mac_NumOfCores)"!]U] =
{
[!ENDCODE!]
    [!LOOP "./McuPartitionConfiguration/*"!]
        [!IF "contains(@name, $Mac_CurrentPartitionPrefixStr)"!]
            [!FOR "CoreCounter" = "1" TO "$Mac_NumOfCores"!]
                [!VAR "Mac_CoreEffectiveIndex" = "num:i(substring-after(ecu:list($Mac_CoreResourcePath)[number($CoreCounter)], 'CORE_'))"!]
                [!VAR "Mac_CurrentCorePrefixStr" = "concat('McuCore', $Mac_CoreEffectiveIndex, 'Configuration')"!]
                [!LOOP "./*"!]
                    [!IF "contains(@name, $Mac_CurrentCorePrefixStr) = 'true'"!]
                        [!VAR "Mac_CoreUnderMcuCtr"    = "node:value(McuCoreUnderMcuControl)"!]
                        [!VAR "Mac_CoreBootAddress"    = "node:value(McuCoreBootAddress)"!]
                        [!VAR "Mac_CoreBootAddressSym" = "node:value(McuCoreBootAddressLinkerSym)"!]
                        [!VAR "Mac_CoreClockEnable"    = "node:value(McuCoreClockEnable)"!]
[!CODE!][!//
[!IF "num:i($Mac_FirstCore)=0"!],[!ENDIF!][!VAR "Mac_FirstCore"="0"!]
    /* The configuration structure for Partition [!"$Mac_PartitionEffectiveIndex"!] Core [!"$Mac_CoreEffectiveIndex"!]. */
    {
        /* Specifies whether the given core is under MCU control. */
        [!IF "text:tolower($Mac_CoreUnderMcuCtr) = 'true'"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!],

        /* The index of the core within the partition. */
        (uint8)[!"num:i($Mac_CoreEffectiveIndex)"!]U,

        /* The boot address of the core. */
        (uint32 *)[!IF "$Mac_CoreBootAddressSym = ''"!][!"num:inttohex($Mac_CoreBootAddress,8)"!]U[!ELSE!][!"$Mac_CoreBootAddressSym"!][!ENDIF!],

        /* The process configuration register value of the core. */
        MC_ME_PRTN[!"$Mac_PartitionEffectiveIndex"!]_CORE[!"num:i($Mac_CoreEffectiveIndex)"!]_PCONF_CCE
        (
            [!IF "text:tolower($Mac_CoreClockEnable) = 'true'"!]MC_ME_PRTNX_COREX_PCONF_CCE_EN_U32[!ELSE!]MC_ME_PRTNX_COREX_PCONF_CCE_DIS_U32[!ENDIF!]
        )
    }[!//
[!ENDCODE!]
                    [!ENDIF!]
                [!ENDLOOP!]
            [!ENDFOR!][!//CoreCounter
        [!ENDIF!]
    [!ENDLOOP!]
[!CODE!]
};
[!ENDCODE!]
            [!ENDIF!][!//num:i($Mac_NumOfCores) > 0
        [!ENDLOOP!][!//McuModeSettingConf/*
    [!ENDFOR!][!//PartitionCounter
[!ENDNOCODE!]
[!ENDMACRO!][!//Power_Ip_MC_ME_Core_Config


[!MACRO "NumberOfCofbs", "ParamPrtnIndex"!][!//
    [!VAR "Mac_ResourceName" = "concat('MCU.MC_ME.Partition', $ParamPrtnIndex, '.COFBs.List')"!]
    [!VAR "Mac_Count" = "num:i(count(ecu:list($Mac_ResourceName)))"!]
    [!"$Mac_Count"!]
[!ENDMACRO!][!//NumberOfCofbs


[!MACRO "GetEnabledPeripherals","ParamPrtnIndex","ParamCofbIndex"!][!//
[!NOCODE!]
    [!CODE!]((uint32)0x00000000U)[!ENDCODE!]
    [!LOOP "../McuPeripheral/*"!]
        [!IF "text:toupper(McuModeEntrySlot) != 'NONE'"!]
            [!VAR "Mac_PrtnIndex" = "num:i(substring(McuModeEntrySlot, (string-length('PRTN') + 1), 1))"!]
            [!IF "num:i($Mac_PrtnIndex) = num:i($ParamPrtnIndex)"!]
                [!VAR "Mac_CofbIndex" = "num:i(substring(McuModeEntrySlot, (string-length(concat('PRTN', num:i($ParamPrtnIndex), '_COFB')) + 1), 1))"!]
                [!IF "num:i($Mac_CofbIndex) = num:i($ParamCofbIndex)"!]
                    [!VAR "Mac_ReqIndex" = "num:i(substring-after(McuModeEntrySlot, concat('PRTN', num:i($ParamPrtnIndex), '_COFB', num:i($ParamCofbIndex), '_REQ')))"!]
                    [!IF "McuPeripheralClockEnable = 'true'"!]
                [!CODE!] | MC_ME_PRTN[!"$Mac_PrtnIndex"!]_COFB[!"$Mac_CofbIndex"!]_CLKEN_REQ[!"$Mac_ReqIndex"!]_MASK[!ENDCODE!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
[!ENDNOCODE!]
[!ENDMACRO!][!//


[!MACRO "Power_Ip_MC_ME_COFB_Config","Type"!][!//
[!NOCODE!]
    [!VAR "Mac_NumOfPartitions" = "count(ecu:list('MCU.Partitions.List'))"!]
    [!FOR "PartitionCounter" = "1" TO "$Mac_NumOfPartitions"!]
        [!VAR "Mac_PartitionEffectiveIndex" = "num:i(substring-after(ecu:list('MCU.Partitions.List')[number($PartitionCounter)], 'Partition'))"!]
        [!VAR "Mac_CurrentPartitionPrefixStr" = "concat('McuPartition', num:i($Mac_PartitionEffectiveIndex), 'Config')"!]
        [!LOOP "McuModuleConfiguration/McuModeSettingConf/*"!]
            [!VAR "Mac_ModeConfigIndex" = "@index"!]
            [!VAR "Mac_NumOfCofbs"!][!CALL "NumberOfCofbs", "ParamPrtnIndex" = "num:i($Mac_PartitionEffectiveIndex)"!][!ENDVAR!]
            [!IF "num:i($Mac_NumOfCofbs) > 0"!]
                [!VAR "Mac_FirstCofb" = "1"!]
[!CODE!]
static const Power_Ip_MC_ME_CofbConfigType Power_Ip_MC_ME_aPartition[!"$Mac_PartitionEffectiveIndex"!]CofbConfig[!"$Type"!]_[!"num:i($Mac_ModeConfigIndex)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($Mac_NumOfCofbs)"!]U] =
{
[!ENDCODE!]
    [!LOOP "./McuPartitionConfiguration/*"!]
        [!IF "contains(@name, $Mac_CurrentPartitionPrefixStr)"!]
            [!FOR "CofbCounter" = "1" TO "$Mac_NumOfCofbs"!]
                [!VAR "Mac_CurrentPrtnCofbListResName" = "concat('MCU.MC_ME.Partition', $Mac_PartitionEffectiveIndex, '.COFBs.List')"!]
                [!VAR "Mac_CofbEffectiveIndex" = "num:i(substring-after(ecu:list($Mac_CurrentPrtnCofbListResName)[number($CofbCounter)], 'COFB_'))"!]
                [!VAR "Mac_CofbUnderMcuCtr"    = "node:value(concat('McuPrtnCofb', $Mac_CofbEffectiveIndex, 'UnderMcuControl'))"!]
[!CODE!][!//
[!IF "num:i($Mac_FirstCofb)=0"!],[!ENDIF!][!VAR "Mac_FirstCofb"="0"!]
    /* The configuration structure for Partition [!"$Mac_PartitionEffectiveIndex"!] COFB [!"$Mac_CofbEffectiveIndex"!]. */
    {
        /* Specifies whether the given COFB set is under MCU control. */
        [!IF "text:tolower($Mac_CofbUnderMcuCtr) = 'true'"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!],

        /* The index of the COFB set within the partition. */
        (uint8)[!"num:i($Mac_CofbEffectiveIndex)"!]U,

        /* The clock enable register value of the COFB set. */
        MC_ME_PRTN[!"$Mac_PartitionEffectiveIndex"!]_COFB[!"$Mac_CofbEffectiveIndex"!]_CLKEN
        (
            [!CALL "GetEnabledPeripherals", "ParamPrtnIndex" = "$Mac_PartitionEffectiveIndex", "ParamCofbIndex" = "$Mac_CofbEffectiveIndex"!][!//
        ),

        /* Mask containing the COFB blocks to be updated. */
        MC_ME_PRTN[!"$Mac_PartitionEffectiveIndex"!]_COFB[!"$Mac_CofbEffectiveIndex"!]_CLKEN_RWBITS_MASK
    }
[!ENDCODE!]
            [!ENDFOR!]
        [!ENDIF!]
    [!ENDLOOP!]
[!CODE!][!//
};
[!ENDCODE!]
            [!ENDIF!][!//num:i($Mac_NumOfCofbs) > 0
        [!ENDLOOP!][!//McuModeSettingConf/*
    [!ENDFOR!][!//PartitionCounter
[!ENDNOCODE!]
[!ENDMACRO!][!//


[!MACRO "Power_Ip_MC_ME_Partition_Config","Type"!][!//
[!NOCODE!]
    [!LOOP "McuModuleConfiguration/McuModeSettingConf/*"!]
        [!VAR "Mac_ModeConfigIndex" = "@index"!]
        [!VAR "Mac_NumOfPartitions" = "count(ecu:list('MCU.Partitions.List'))"!]
        [!VAR "Mac_FirstPartition" = "1"!]
[!CODE!][!//
static const Power_Ip_MC_ME_PartitionConfigType Power_Ip_MC_ME_aPartitionConfig[!"$Type"!]_[!"num:i($Mac_ModeConfigIndex)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($Mac_NumOfPartitions)"!]U] =
{
[!ENDCODE!]
    [!FOR "PartitionCounter" = "1" TO "$Mac_NumOfPartitions"!]
        [!VAR "Mac_PartitionEffectiveIndex" = "num:i(substring-after(ecu:list('MCU.Partitions.List')[number($PartitionCounter)], 'Partition'))"!]
        [!VAR "Mac_NumOfCofbs"!][!CALL "NumberOfCofbs", "ParamPrtnIndex" = "num:i($Mac_PartitionEffectiveIndex)"!][!ENDVAR!]
        [!VAR "Mac_NumOfCores"!][!CALL "NumberOfCores", "ParamPrtnIndex" = "num:i($Mac_PartitionEffectiveIndex)"!][!ENDVAR!]
        [!VAR "Mac_PartitionUnderMcuCtr"      = "node:value(concat('McuPartitionConfiguration/McuPartition', $Mac_PartitionEffectiveIndex, 'Config/McuPartitionUnderMcuControl'))"!]
        [!VAR "Mac_PartitionPowerUnderMcuCtr" = "node:value(concat('McuPartitionConfiguration/McuPartition', $Mac_PartitionEffectiveIndex, 'Config/McuPartitionPowerUnderMcuControl'))"!]
        [!VAR "Mac_PartitionClockEnable"      = "node:value(concat('McuPartitionConfiguration/McuPartition', $Mac_PartitionEffectiveIndex, 'Config/McuPartitionClockEnable'))"!]
[!CODE!][!//
[!IF "num:i($Mac_FirstPartition)=0"!],[!ENDIF!][!VAR "Mac_FirstPartition"="0"!]
    /* The configuration structure for Partition [!"$Mac_PartitionEffectiveIndex"!]. */
    {
        /* Specifies whether the given partition is under MCU control. */
        [!IF "text:tolower($Mac_PartitionUnderMcuCtr) = 'true'"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!],

        /* Specifies whether the given partition's power management is under MCU control. */
        [!IF "text:tolower($Mac_PartitionPowerUnderMcuCtr) = 'true'"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!],

        /* Partition Index */
        (uint8)[!"num:i($Mac_PartitionEffectiveIndex)"!]U,

        /* The process configuration register value of the partition. */
        (
            [!IF "text:tolower(ecu:get('MCU.MC_ME.HasOutputSafeState')) = 'true'"!]
            [!IF "text:tolower($Mac_PartitionClockEnable) = 'true'"!]MC_ME_PRTNX_PCONF_PCE_EN_U32[!ELSE!]MC_ME_PRTNX_PCONF_PCE_DIS_U32[!ENDIF!] |

            [!IF "text:tolower($Mac_PartitionClockEnable) = 'true'"!]MC_ME_PRTNX_PCONF_OSSE_DIS_U32[!ELSE!]MC_ME_PRTNX_PCONF_OSSE_EN_U32[!ENDIF!]
            [!ELSE!]
            [!IF "text:tolower($Mac_PartitionClockEnable) = 'true'"!]MC_ME_PRTNX_PCONF_PCE_EN_U32[!ELSE!]MC_ME_PRTNX_PCONF_PCE_DIS_U32[!ENDIF!]
            [!ENDIF!]
        ),

        /* Mask containing the Partition triggers (PCE/OSSE/etc) to be updated. */
        MC_ME_PRTN[!"$Mac_PartitionEffectiveIndex"!]_PUPD_RWBITS_MASK,

        /* Number of COFBs within Partition [!"num:i($Mac_PartitionEffectiveIndex)"!] */
        (uint8)[!"num:i($Mac_NumOfCofbs)"!]U,
        /* Configuration for the COFBs within Partition [!"num:i($Mac_PartitionEffectiveIndex)"!]. */
        [!IF "num:i($Mac_NumOfCofbs) != 0"!]&Power_Ip_MC_ME_aPartition[!"$Mac_PartitionEffectiveIndex"!]CofbConfig[!"$Type"!]_[!"num:i($Mac_ModeConfigIndex)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ELSE!](NULL_PTR)[!ENDIF!],

        /* Number of Cores within Partition [!"num:i($Mac_PartitionEffectiveIndex)"!] */
        (uint8)[!"num:i($Mac_NumOfCores)"!]U,
        /* Configuration for the Cores within Partition [!"num:i($Mac_PartitionEffectiveIndex)"!]. */
        [!IF "num:i($Mac_NumOfCores) != 0"!]&Power_Ip_MC_ME_aPartition[!"$Mac_PartitionEffectiveIndex"!]CoreConfig[!"$Type"!]_[!"num:i($Mac_ModeConfigIndex)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ELSE!](NULL_PTR)[!ENDIF!]
    }[!//
[!ENDCODE!]
    [!ENDFOR!]
[!CODE!]
};
[!ENDCODE!]
    [!ENDLOOP!][!//McuModeSettingConf/*
[!ENDNOCODE!]
[!ENDMACRO!][!//

[!MACRO "Power_Ip_MC_ME_Mode_Config","Type"!][!//
[!LOOP "McuModuleConfiguration/McuModeSettingConf/*"!][!//
[!NOCODE!][!//
[!CODE!][!//
/**
* @brief            Definition of a MCU mode section in the configuration structure.
* @details          Specifies the system behaviour during the selected target mode.
*                   Data set and configured by Mcu_SetMode call.
*/
const Power_Ip_MC_ME_ModeConfigType Power_Ip_MC_ME_ModeConfig[!"$Type"!]_[!"num:i(@index)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
#if (defined(POWER_IP_ENTER_LOW_POWER_MODE) && (POWER_IP_ENTER_LOW_POWER_MODE == STD_ON))
    /* MC_ME Main Core ID register */
    MC_ME_MAIN_COREID_U32
    (
[!IF "(McuPowerMode = 'SOC_STANDBY') or (McuPowerMode = 'STANDBY') or (McuPowerMode = 'SOC_FAST_STANDBY') or (McuPowerMode = 'FAST_STANDBY')"!][!//
        MC_ME_MAIN_COREID_PIDX_[!"McuMainCoreSelect"!]_U32 |
        MC_ME_MAIN_COREID_CIDX_[!"McuMainCoreSelect"!]_U32
[!ELSE!][!//
        MC_ME_MAIN_COREID_NOT_USED_U32
[!ENDIF!][!//
    ),
#endif /* (POWER_IP_ENTER_LOW_POWER_MODE == STD_ON) */
    /* MC_ME Mode Partition Settings. */
    &Power_Ip_MC_ME_aPartitionConfig[!"$Type"!]_[!"num:i(@index)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
};
[!ENDCODE!]
[!ENDNOCODE!]
[!ENDLOOP!][!//McuModeSettingConf/*
[!ENDMACRO!][!//

[!MACRO "Power_Ip_MC_RGM_Core_Config","Type"!][!//
[!NOCODE!]
    [!VAR "Mac_NumOfDomains" = "count(ecu:list('MCU.Partitions.List'))"!]
    [!FOR "DomainCounter" = "1" TO "$Mac_NumOfDomains"!]
        [!VAR "Mac_DomainEffectiveIndex" = "num:i(substring-after(ecu:list('MCU.Partitions.List')[number($DomainCounter)], 'Partition'))"!]
        [!VAR "Mac_CurrentDomainPrefixStr" = "concat('McuPartition', $Mac_DomainEffectiveIndex, 'Config')"!]
        [!VAR "Mac_CoreResourcePath" = "concat('MCU.MC_ME.Partition', $DomainEffectiveIndex, '.Cores.List')"!]
        [!LOOP "McuModuleConfiguration/McuModeSettingConf/*"!]
            [!VAR "Mac_ModeConfigIndex" = "@index"!]
            [!VAR "Mac_NumOfCores"!][!CALL "NumberOfCores", "ParamPrtnIndex" = "$Mac_DomainEffectiveIndex"!][!ENDVAR!]
            [!IF "num:i($Mac_NumOfCores) > 0"!]
                [!VAR "Mac_FirstCore" = "1"!]
[!CODE!]
static const MC_RGM_CoreConfigType Power_Ip_MC_RGM_aDomain[!"$Mac_DomainEffectiveIndex"!]CoreConfig[!"$Type"!]_[!"num:i($Mac_ModeConfigIndex)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($Mac_NumOfCores)"!]U] =
{
[!ENDCODE!]
    [!LOOP "./McuPartitionConfiguration/*"!]
        [!IF "contains(@name, $Mac_CurrentDomainPrefixStr)"!]
            [!FOR "CoreCounter" = "1" TO "$NumOfCores"!]
                [!VAR "Mac_CoreEffectiveIndex" = "num:i(substring-after(ecu:list($Mac_CoreResourcePath)[number($CoreCounter)], 'CORE_'))"!]
                [!VAR "Mac_CurrentCorePrefixStr" = "concat('McuCore', $Mac_CoreEffectiveIndex, 'Configuration')"!]
                [!LOOP "./*"!]
                    [!IF "contains(@name, $Mac_CurrentCorePrefixStr) = 'true'"!]
                        [!VAR "Mac_CoreUnderMcuCtr"    = "node:value(McuCoreUnderMcuControl)"!]
                        [!VAR "Mac_CoreResetEnable"    = "node:value(McuCoreResetEnable)"!]
[!CODE!][!//
    [!IF "num:i($Mac_FirstCore)=0"!],[!ENDIF!][!VAR "Mac_FirstCore"="0"!]
    /* The configuration structure for Domain [!"$Mac_DomainEffectiveIndex"!] Core [!"$Mac_CoreEffectiveIndex"!]. */
    {
        /* Specifies whether the given core is under MCU control. */
        [!IF "text:tolower($Mac_CoreUnderMcuCtr) = 'true'"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!],

        /* The index of the core within the domain. */
        (uint8)[!"num:i($Mac_CoreEffectiveIndex)"!]U,

        /* The reset enable register value of the core. */
        MC_RGM_PRST[!"$Mac_DomainEffectiveIndex"!]_COFB0_RSTEN
        (
            ((uint32)0x00000000U)
            [!IF "text:tolower($Mac_CoreResetEnable) = 'true'"!]| MC_RGM_PRST[!"$Mac_DomainEffectiveIndex"!]_COFB0_RSTEN_CORES_MASK([!"$Mac_CoreEffectiveIndex"!]U)[!ENDIF!]
        ),

        /* Mask containing the Core blocks to be updated. */
        MC_RGM_PRST[!"$Mac_DomainEffectiveIndex"!]_COFB0_RSTEN_CORES_MASK([!"$Mac_CoreEffectiveIndex"!]U)
    }
[!ENDCODE!][!//
                    [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ENDFOR!]
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!CODE!][!//
};
[!ENDCODE!][!//
            [!ENDIF!][!//num:i($Mac_NumOfCores) > 0
        [!ENDLOOP!][!//McuModeSettingConf/*
    [!ENDFOR!][!//PartitionCounter
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "NumberOfRstCofbs", "ParamDomainIndex"!][!//
    [!VAR "Mac_ResourceName" = "concat('MCU.MC_RGM.Partition', $ParamDomainIndex, '.COFBs.List')"!][!//
    [!VAR "Mac_Count" = "num:i(count(ecu:list($Mac_ResourceName)))"!][!//
    [!"$Mac_Count"!][!//
[!ENDMACRO!][!//

[!MACRO "GetResettablePeripherals","ParamDomainIndex","ParamCofbIndex"!][!//
[!NOCODE!][!//
    [!CODE!]((uint32)0x00000000U) [!ENDCODE!][!//
    [!LOOP "../McuPeripheral/*"!][!//
        [!IF "text:toupper(McuResetGenerationSlot) != 'NONE'"!]
            [!VAR "Mac_PrstIndex" = "num:i(substring-after(McuResetGenerationSlot, 'PRST'))"!]
            [!IF "num:i($Mac_PrstIndex) = num:i($ParamDomainIndex)"!]
                [!VAR "Mac_CofbIndex" = "num:i(substring-after(McuResetGenerationSlot, concat('PRST', $ParamDomainIndex, '_COFB')))"!]
                [!IF "num:i($Mac_CofbIndex) = num:i($ParamCofbIndex)"!]
                    [!VAR "Mac_ReqIndex" = "num:i(substring-after(McuResetGenerationSlot, concat('PRST', $ParamDomainIndex, '_COFB', $ParamCofbIndex, '_PERIPH_')))"!]
                    [!//[!VAR "Mac_ReqIndex" = "num:i(num:mod($Mac_ReqIndex, 32))"!]
                    [!IF "McuPeripheralResetEnable = 'true'"!]
                [!CODE!]| MC_RGM_PRST_[!"$Mac_PrstIndex"!]_PERIPH_[!"$Mac_ReqIndex"!]_RST_MASK [!ENDCODE!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
[!ENDNOCODE!]
[!ENDMACRO!][!//


[!MACRO "Power_Ip_MC_RGM_COFB_Config","Type"!][!//
[!NOCODE!][!//
    [!VAR "Mac_NumOfDomains" = "count(ecu:list('MCU.Partitions.List'))"!]
    [!FOR "DomainCounter" = "1" TO "$Mac_NumOfDomains"!]
        [!VAR "Mac_DomainEffectiveIndex" = "num:i(substring-after(ecu:list('MCU.Partitions.List')[number($DomainCounter)], 'Partition'))"!]
        [!VAR "Mac_CurrentDomainPrefixStr" = "concat('McuPartition', num:i($Mac_DomainEffectiveIndex), 'Config')"!]
        [!LOOP "McuModuleConfiguration/McuModeSettingConf/*"!]
            [!VAR "Mac_ModeConfigIndex" = "@index"!]
            [!VAR "Mac_NumOfCofbs"!][!CALL "NumberOfRstCofbs", "ParamDomainIndex" = "num:i($Mac_DomainEffectiveIndex)"!][!ENDVAR!]
            [!IF "num:i($Mac_NumOfCofbs) > 0"!]
                [!VAR "Mac_FirstCofb" = "1"!]
[!CODE!][!//
static const MC_RGM_CofbConfigType Power_Ip_MC_RGM_aDomain[!"$Mac_DomainEffectiveIndex"!]CofbConfig[!"$Type"!]_[!"num:i($Mac_ModeConfigIndex)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($NumOfCofbs)"!]U] =
{
[!ENDCODE!][!//
    [!LOOP "./McuPartitionConfiguration/*"!]
        [!IF "contains(@name, $Mac_CurrentDomainPrefixStr)"!]
            [!FOR "CofbCounter" = "1" TO "$Mac_NumOfCofbs"!]
                [!VAR "Mac_CurrentDomainCofbListResName" = "concat('MCU.MC_RGM.Partition', $Mac_DomainEffectiveIndex, '.COFBs.List')"!]
                [!VAR "Mac_CofbEffectiveIndex"   = "num:i(substring-after(ecu:list($Mac_CurrentDomainCofbListResName)[number($CofbCounter)], 'COFB_'))"!]
                [!VAR "Mac_CofbUnderMcuCtr"      = "node:value(concat('McuPrstCofb', $Mac_CofbEffectiveIndex, 'UnderMcuControl'))"!]
[!CODE!][!//
    [!IF "num:i($Mac_FirstCofb)=0"!],[!ENDIF!][!VAR "Mac_FirstCofb"="0"!]
    /* The configuration structure for Domain [!"$Mac_DomainEffectiveIndex"!] COFB [!"$Mac_CofbEffectiveIndex"!] (i.e. PRST[!"$Mac_DomainEffectiveIndex"!]_[!"$Mac_CofbEffectiveIndex"!]). */
    {
        /* Specifies whether the given COFB set is under MCU control. */
        [!IF "text:tolower($Mac_CofbUnderMcuCtr) = 'true'"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!],

        /* The index of the COFB set within the domain. */
        (uint8)[!"num:i($Mac_CofbEffectiveIndex)"!]U,

        /* The reset enable register value of the COFB set. */
        MC_RGM_PRST[!"$Mac_DomainEffectiveIndex"!]_COFB[!"$Mac_CofbEffectiveIndex"!]_RSTEN
        (
            [!CALL "GetResettablePeripherals", "ParamDomainIndex" = "$Mac_DomainEffectiveIndex", "ParamCofbIndex" = "$Mac_CofbEffectiveIndex"!][!//
        ),

        /* Mask containing the COFB blocks to be updated. */
        MC_RGM_PRST[!"$Mac_DomainEffectiveIndex"!]_COFB[!"$Mac_CofbEffectiveIndex"!]_RSTEN_PERIPH_MASK
    }
[!ENDCODE!][!//
            [!ENDFOR!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!CODE!][!//
};
[!ENDCODE!][!//
            [!ENDIF!][!//num:i($Mac_NumOfCofbs) > 0
        [!ENDLOOP!][!//McuModeSettingConf/*
    [!ENDFOR!][!//DomainCounter
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "Power_Ip_MC_RGM_Domain_Config","Type"!][!//
[!NOCODE!]
    [!LOOP "McuModuleConfiguration/McuModeSettingConf/*"!][!//
        [!VAR "Mac_ModeConfigIndex" = "@index"!][!//
        [!VAR "Mac_NumOfDomains" = "count(ecu:list('MCU.Partitions.List'))"!][!//
        [!VAR "Mac_FirstDomain" = "1"!][!//
[!CODE!][!//
static const Power_Ip_MC_RGM_DomainConfigType Power_Ip_MC_RGM_aDomainConfig[!"$Type"!]_[!"num:i($Mac_ModeConfigIndex)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($Mac_NumOfDomains)"!]U] =
{
[!ENDCODE!]
    [!FOR "DomainCounter" = "1" TO "$Mac_NumOfDomains"!]
        [!VAR "Mac_DomainEffectiveIndex" = "num:i(substring-after(ecu:list('MCU.Partitions.List')[number($DomainCounter)], 'Partition'))"!]
        [!VAR "Mac_NumOfCofbs"!][!CALL "NumberOfRstCofbs", "ParamDomainIndex" = "num:i($Mac_DomainEffectiveIndex)"!][!ENDVAR!]
        [!VAR "Mac_NumOfCores"!][!CALL "NumberOfCores", "ParamPrtnIndex" = "$Mac_DomainEffectiveIndex"!][!ENDVAR!]
        [!VAR "Mac_DomainUnderMcuCtr"      = "node:value(concat('McuPartitionConfiguration/McuPartition', $Mac_DomainEffectiveIndex, 'Config/McuPartitionUnderMcuControl'))"!]
        [!VAR "Mac_DomainPowerUnderMcuCtr" = "node:value(concat('McuPartitionConfiguration/McuPartition', $Mac_DomainEffectiveIndex, 'Config/McuPartitionPowerUnderMcuControl'))"!]
        [!VAR "Mac_PartitionResetEnable"   = "node:value(concat('McuPartitionConfiguration/McuPartition', $Mac_DomainEffectiveIndex, 'Config/McuPartitionResetEnable'))"!]
[!CODE!]
    [!IF "num:i($Mac_FirstDomain)=0"!],[!ENDIF!][!VAR "Mac_FirstDomain"="0"!]
    /* The configuration structure for Domain [!"$Mac_DomainEffectiveIndex"!]. */
    {
        /* Specifies whether the given domain is under MCU control. */
        [!IF "text:tolower($Mac_DomainUnderMcuCtr) = 'true'"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!],

        /* Specifies whether the given domain's power management is under MCU control. */
        [!IF "text:tolower($Mac_DomainPowerUnderMcuCtr) = 'true'"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!],

        /* Domain Index */
        (uint8)[!"num:i($Mac_DomainEffectiveIndex)"!]U,

        /* The process configuration register value of the domain. */
        [!IF "text:tolower($Mac_PartitionResetEnable) = 'false'"!]RDC_RDX_CTRL_REG_ICON_INTF_ENABLE[!ELSE!]RDC_RDX_CTRL_REG_ICON_INTF_DISABLE[!ENDIF!],

        /* Number of COFBs within Domain [!"num:i($Mac_DomainEffectiveIndex)"!] */
        (uint8)[!"num:i($Mac_NumOfCofbs)"!]U,
        /* Configuration for the COFBs within Domain [!"num:i($Mac_DomainEffectiveIndex)"!]. */
        [!IF "num:i($Mac_NumOfCofbs) != 0"!]&Power_Ip_MC_RGM_aDomain[!"$Mac_DomainEffectiveIndex"!]CofbConfig[!"$Type"!]_[!"num:i($Mac_ModeConfigIndex)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ELSE!](NULL_PTR)[!ENDIF!],

        /* Number of Cores within Domain [!"num:i($Mac_DomainEffectiveIndex)"!] */
        (uint8)[!"num:i($Mac_NumOfCores)"!]U,
        /* Configuration for the Cores within Domain [!"num:i($Mac_DomainEffectiveIndex)"!]. */
        [!IF "num:i($Mac_NumOfCores) != 0"!]&Power_Ip_MC_RGM_aDomain[!"$Mac_DomainEffectiveIndex"!]CoreConfig[!"$Type"!]_[!"num:i($Mac_ModeConfigIndex)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ELSE!](NULL_PTR)[!ENDIF!]
    }
[!ENDCODE!]
    [!ENDFOR!]
[!CODE!]
};
[!ENDCODE!]
    [!ENDLOOP!][!//McuModeSettingConf/*
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "Power_Ip_MC_RGM_Mode_Config","Type"!][!//
[!LOOP "McuModuleConfiguration/McuModeSettingConf/*"!][!//
[!NOCODE!][!//
[!CODE!][!//
/**
* @brief            Definition of a MCU mode section in the configuration structure.
* @details          Specifies the system behaviour during the selected target mode.
*                   Data set and configured by Mcu_SetMode call.
*/
static const Power_Ip_MC_RGM_ModeConfigType Power_Ip_MC_RGM_ModeConfig[!"$Type"!]_[!"num:i(@index)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    /* MC_RGM Mode Domain Settings. */
    &Power_Ip_MC_RGM_aDomainConfig[!"$Type"!]_[!"num:i(@index)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
};
[!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDLOOP!][!//McuModeSettingConf/*
[!ENDMACRO!][!//

[!MACRO "GetControlDCMRWF5"!][!//
[!NOCODE!][!//
[!IF "node:value('McuPowerMode') = 'SOC_PREPARE_FAST_STANDBY' or node:value('McuPowerMode') = 'SOC_FAST_STANDBY' or node:value('McuPowerMode') = 'FAST_STANDBY'"!][!//
[!CODE!][!//
/* Boot Mode */
    (
        (uint8)1U
    ),
[!ENDCODE!][!//
[!ELSE!][!//
[!CODE!][!//
/* Boot Mode */
    (
        (uint8)0U
    ),
[!ENDCODE!][!//
[!ENDIF!][!//
[!CODE!][!//
    /* Cortex-M7_0 base address of vector table. */
    (
        ((uint32)[!"num:inttohex(McuDcmGprConfiguration/McuBootBaseAddress)"!]U)
    ),
[!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetControlDCMRWF2"!][!//
[!NOCODE!]
    [!CODE!]((uint32)0x00000000U) [!ENDCODE!]
    [!VAR "Mac_NumOfDCMRWF2" = "count(ecu:list('MCU.DCM.Config.List'))"!]
    [!FOR "DCMRWF2Counter" = "1" TO "$Mac_NumOfDCMRWF2"!]
        [!VAR "Mac_DCMRWF2Node"  = "concat('McuDcmGprConfiguration/Mcu', ecu:list('MCU.DCM.Config.List')[num:i($DCMRWF2Counter)], 'Control')"!]
        [!VAR "Mac_DCMRWF2Value" = "node:value($Mac_DCMRWF2Node)"!]
        [!IF "text:tolower($Mac_DCMRWF2Value) = 'true'"!]
        [!CODE!]| DCM_GPR_DCMRWF2_[!"ecu:list('MCU.DCM.Config.List')[num:i($DCMRWF2Counter)]"!]_MASK [!ENDCODE!]
    [!ENDIF!]
    [!ENDFOR!]
[!ENDNOCODE!]
[!ENDMACRO!][!//

[!MACRO "Power_Ip_DCM_GPR_Config","Type"!][!//
[!NOCODE!]
[!LOOP "McuModuleConfiguration/McuModeSettingConf/*"!][!//
[!CODE!]
/**
* @brief          Initialization data for DCM_GPR hw IP.
* @details        Data set and configured by Mcu_SetMode call.
*
*/
static const Power_Ip_DCM_GPR_ConfigType Power_Ip_DCM_GPR_Config[!"$Type"!]_[!"num:i(@index)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
[!ENDCODE!]
    [!VAR "Mac_DcmGprUnderMcuCtr" = "node:value('McuDcmGprConfiguration/McuDcmGprUnderMcuControl')"!]
    [!VAR "Mac_McuGlobalPadkeepingEnable" = "node:value('McuDcmGprConfiguration/McuGlobalPadkeepingEnable')"!]
[!CODE!]
    /* Specifies whether the given domain is under MCU control. */
    [!IF "text:tolower($Mac_DcmGprUnderMcuCtr) = 'true'"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!],
    [!CALL "GetControlDCMRWF5"!][!//
[!IF "count(ecu:list('MCU.DCM.Config.List')) > num:i(0)"!][!//
    /* DCM_GPR Configuration Register (DCMRWF2). */
    (
        [!CALL "GetControlDCMRWF2"!][!//
    ),
[!ENDIF!][!//
    /* Global Padkeeping configuration. */
    [!IF "text:tolower($Mac_McuGlobalPadkeepingEnable) = 'true'"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!]
};
[!ENDCODE!]
[!ENDLOOP!][!//McuModeSettingConf/*
[!ENDNOCODE!]
[!ENDMACRO!][!//

[!MACRO "Power_Ip_Mode_Config","Type"!][!//
[!NOCODE!][!//
[!CODE!][!//
/**
* @brief          Initialization mode data.
* @details        Static configuration realized by calling Mcu_SetMode() API.
*
*/
const Power_Ip_ModeConfigType Power_Ip_aModeConfig[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i(count(McuModuleConfiguration/McuModeSettingConf/*))"!]U] =
{
[!ENDCODE!][!//
    [!IF "num:i(count(McuModuleConfiguration/McuModeSettingConf/*))>0"!]
        [!VAR "Mac_ModeConfigs"= "num:i(count(McuModuleConfiguration/McuModeSettingConf/*))-1"!]
        [!LOOP "McuModuleConfiguration/McuModeSettingConf/*"!]
[!CODE!][!//
    /* Start of Mcu_aModeConfig[[!"@index"!]] */
    {
[!ENDCODE!][!//
        [!VAR "Mac_PCindex" = "@index"!]
[!CODE!][!//
        /* Mode Configuration ID. */
        (Power_Ip_ModeType)[!"num:i(McuMode)"!]U,

        /* The Power Mode name (code). */
        POWER_IP_[!"McuPowerMode"!]_MODE,

        /* The Sleep On Exit configuration */
        [!IF "McuEnableSleepOnExit='true'"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!],

        /* MC_ME IP Mode settings. */
        &Power_Ip_MC_ME_ModeConfig[!"$Type"!]_[!"num:i(@index)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
[!IF "count(ecu:list('MCU.MC_RGM.Partition0.COFBs.List')) > 0"!][!//
        /* MC_RGM IP Mode settings. */
        &Power_Ip_MC_RGM_ModeConfig[!"$Type"!]_[!"num:i(@index)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
[!ENDIF!][!//

[!IF "count(ecu:list('MCU.DCM.Config.List')) > 0"!][!//
        /* Configuration for DCM_GPR (Device Configuration Module General-Purpose Registers) hardware IP. */
        &Power_Ip_DCM_GPR_Config[!"$Type"!]_[!"num:i(@index)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
[!ENDIF!][!//

    }[!IF "$Mac_PCindex < $Mac_ModeConfigs"!],[!ENDIF!] /* End of Mcu_aModeConfig[[!"@index"!]] */
[!ENDCODE!][!//
        [!ENDLOOP!][!//McuModeSettingConf/*
    [!ENDIF!][!//
[!CODE!][!//
};
[!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//


[!MACRO "Power_Ip_MC_RGM_Config","Type"!][!//
[!NOCODE!]
[!CODE!]
/**
* @brief          Initialization data for MC_RGM hardware IP.
* @details        Static configuration assigned at startup by calling Mcu_Init() API.
*
*/
static const Power_Ip_MC_RGM_ConfigType Power_Ip_MC_RGM_Config[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
#if (POWER_IP_PERFORM_RESET_API == STD_ON)
    /* MC_ResetType: Functional vs Destructive. */
    ([!IF "McuModuleConfiguration/McuResetConfig/McuResetType = 'FunctionalReset'"!]MCU_FUNC_RESET[!ELSE!]MCU_DEST_RESET[!ENDIF!]),
#endif /* (POWER_IP_PERFORM_RESET_API == STD_ON) */

[!ENDCODE!]
[!IF "num:i(count(McuModuleConfiguration/McuResetConfig/McuResetSourcesConfig/*)) != num:i(0)"!]
[!CODE!]
#if (defined(POWER_IP_FUNCTIONAL_RESET_DISABLE_SUPPORT) && (POWER_IP_FUNCTIONAL_RESET_DISABLE_SUPPORT == STD_ON))
    /* Func_ResetOpt: FERD Register Configuration. */
    (
[!ENDCODE!]
    [!LOOP "McuModuleConfiguration/McuResetConfig/McuResetSourcesConfig/*"!]
        [!VAR "Mac_ResetSource" = "substring-before( substring-after( @name,'Mcu' ),'_ResetSource' )"!]
        [!IF "node:value(McuDisableReset)"!]
[!CODE!][!//
        MC_RGM_FERD_D_[!"$Mac_ResetSource"!]_MASK |
[!ENDCODE!][!//
        [!ENDIF!]
    [!ENDLOOP!]
[!CODE!][!//
        0U
    ),
#endif /* POWER_IP_FUNCTIONAL_RESET_DISABLE_SUPPORT */
[!ENDCODE!]
[!ENDIF!]
[!CODE!]

    /* Func_Threshold_Reset: RGM_FRET Register Configuration. */
    MC_RGM_FRET_FRET((uint32)[!"num:i(McuModuleConfiguration/McuResetConfig/McuFuncResetEscThreshold)"!]U),

    /* Dest_Threshold_Reset: RGM_DRET Register Configuration. */
    MC_RGM_DRET_DRET((uint32)[!"num:i(McuModuleConfiguration/McuResetConfig/McuDestResetEscThreshold)"!]U)
};
[!ENDCODE!]
[!ENDNOCODE!]
[!ENDMACRO!][!//


[!MACRO "GetEnabledNCSPD"!][!//
[!NOCODE!]
    [!CODE!]((uint32)0x00000000U) [!ENDCODE!]
    [!VAR "Mac_NumOfNCSPD" = "count(ecu:list('MCU.PMC.NonCriticalSPD.List'))"!]
    [!FOR "NCSPDCounter" = "1" TO "$Mac_NumOfNCSPD"!]
        [!VAR "Mac_NCSPDNode"  = "concat('Mcu', ecu:list('MCU.PMC.NonCriticalSPD.List')[num:i($NCSPDCounter)], 'NonCriticalFlag')"!]
        [!VAR "Mac_NCSPDValue" = "node:value($Mac_NCSPDNode)"!]
        [!IF "text:tolower($Mac_NCSPDValue) = 'true'"!]
        [!CODE!]| PMC_NCSPD_CTL_NCSPD_CTL[!"ecu:list('MCU.PMC.NonCriticalSPD.Indices.List')[num:i($NCSPDCounter)]"!]_MASK [!ENDCODE!]
    [!ENDIF!]
    [!ENDFOR!]
[!ENDNOCODE!]
[!ENDMACRO!][!//

[!MACRO "GetEnabledCONFIG"!][!//
[!NOCODE!]
    [!CODE!]((uint32)0x00000000U) [!ENDCODE!]
    [!VAR "Mac_NumOfCONFIG" = "count(ecu:list('MCU.PMC.Config.List'))"!]
    [!FOR "CONFIGCounter" = "1" TO "$Mac_NumOfCONFIG"!]
        [!VAR "Mac_CONFIGNode"  = "concat('Mcu', ecu:list('MCU.PMC.Config.List')[num:i($CONFIGCounter)], 'Enable')"!]
        [!VAR "Mac_CONFIGValue" = "node:value($Mac_CONFIGNode)"!]
        [!IF "text:tolower($Mac_CONFIGValue) = 'true'"!]
        [!CODE!]| PMC_CONFIG_[!"ecu:list('MCU.PMC.Config.List')[num:i($CONFIGCounter)]"!]_MASK [!ENDCODE!]
    [!ENDIF!]
    [!ENDFOR!]
[!ENDNOCODE!]
[!ENDMACRO!][!//

[!MACRO "Power_Ip_PMC_Config","Type"!][!//
[!NOCODE!]
[!SELECT "McuModuleConfiguration/McuPowerControl/McuPMC_Config"!]
[!CODE!]
/**
* @brief          Initialization data for PMC hw IP.
* @details        Static configuration realized at statup by calling Mcu_Init() API.
*
*/
static const Power_Ip_PMC_ConfigType Power_Ip_PMC_Config[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
[!IF "count(ecu:list('MCU.PMC.NonCriticalSPD.List')) > num:i(0)"!][!//
    /* Non-Critical Supply Presence Detector Control Register (NCSPD_CTL). */
    (
        [!CALL "GetEnabledNCSPD"!][!//
    )
[!ENDIF!][!//
[!IF "count(ecu:list('MCU.PMC.Config.List')) > num:i(0)"!][!//
    /* PMC Configuration Register (CONFIG). */
    (
        [!CALL "GetEnabledCONFIG"!][!//
    )
[!ENDIF!][!//
};
[!ENDCODE!]
[!ENDSELECT!][!//McuModuleConfiguration/McuPowerControl/McuPMC_Config
[!ENDNOCODE!]
[!ENDMACRO!][!//

[!MACRO "Power_Ip_HwIPs_Config","Type"!][!//
[!NOCODE!]
[!CODE!]
/**
* @brief          Initialization data for different hw IPs.
* @details        Static configuration realized at startup by calling Mcu_Init() API.
*
*/
const Power_Ip_HwIPsConfigType Power_Ip_HwIPsConfig[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    /* Configuration for MC_RGM (Reset Generation Module) hardware IP. */
    &Power_Ip_MC_RGM_Config[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],

    /* Configuration for PMU (Power Management Unit) hardware IP, part of PCU. */
    &Power_Ip_PMC_Config[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],

};
[!ENDCODE!]
[!ENDNOCODE!]
[!ENDMACRO!][!//

[!ENDNOCODE!][!//


