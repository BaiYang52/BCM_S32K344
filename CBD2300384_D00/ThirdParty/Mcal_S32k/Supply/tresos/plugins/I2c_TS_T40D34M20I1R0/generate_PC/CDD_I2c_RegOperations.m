[!NOCODE!]
/**
    @file    CDD_I2c_RegOperations.m
    @version 2.0.1

    @brief   AUTOSAR I2c - Register operations.
    @details Register operations.

    Project RTD AUTOSAR 4.4
    Platform CORTEXM
    Peripheral LPI2C
    Dependencies none

    ARVersion 4.4.0
    ARRevision ASR_REL_4_4_REV_0000
    ARConfVariant
    SWVersion 2.0.1
    BuildVersion S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707

    (c) Copyright 2020 - 2022 NXP Semiconductors
    All Rights Reserved.
 */
/*==================================================================================================
==================================================================================================*/

[!/* *** multiple inclusion protection *** */!]
[!IF "not(var:defined('I2C_REG_OPERATIONS_M'))"!][!//
[!VAR "I2C_REG_OPERATIONS_M"="'true'"!][!//

[!MACRO "CheckDuplicates"!]
[!NOCODE!]
    [!LOOP "I2cGlobalConfig/*"!]
        [!VAR "ConfigIndexA" = "@name"!]
        [!LOOP "./I2cChannel/*"!]
            [!VAR "channelSettingMain" = "@name"!]
            [!LOOP "../../../../I2cGlobalConfig/*"!]
                [!VAR "ConfigIndexB" = "@name"!]
                [!IF "$ConfigIndexA!=$ConfigIndexB"!]
                    [!LOOP "./I2cChannel/*"!]
                        [!VAR "channelSettingSlave" = "@name"!]
                        [!IF "$channelSettingMain=$channelSettingSlave"!]
                            [!ERROR!]
    [!"$ConfigIndexA"!] contains I2cChannelConfig [!"$channelSettingMain"!] which is duplicated in [!"$ConfigIndexB"!]
                            [!ENDERROR!]
                        [!ENDIF!]
                    [!ENDLOOP!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ENDLOOP!]
    [!ENDLOOP!]
[!ENDNOCODE!]
[!ENDMACRO!][!//


[!MACRO "CheckFlexIOPinConsistency"!]
[!NOCODE!]
    [!LOOP "I2cGlobalConfig/*"!]
        [!VAR "ConfigIndexA" = "@name"!]
        [!"$ConfigIndexA"!]
        [!LOOP "./I2cChannel/*"!]
            [!IF "contains(I2cHwChannel,'FLEXIO')"!]
                [!VAR "channelSettingMain" = "@name"!]
                [!VAR "firstsdaPin" = "./I2cFlexIOConfiguration/I2cFlexIOSdaPin"!]
                [!VAR "firstsclPin" = "./I2cFlexIOConfiguration/I2cFlexIOSclPin"!]
                    [!LOOP "../../I2cChannel/*"!]
                    [!IF "contains(I2cHwChannel,'FLEXIO')"!]
                        [!VAR "channelSettingSlave" = "@name"!]
                        [!IF "$channelSettingSlave != $channelSettingMain"!]
                            [!VAR "secondsdaPin" = "./I2cFlexIOConfiguration/I2cFlexIOSdaPin"!]
                            [!VAR "secondsclPin" = "./I2cFlexIOConfiguration/I2cFlexIOSclPin"!]
                            [!IF "($firstsdaPin = $secondsdaPin) or ($firstsdaPin = $secondsclPin) or ($firstsclPin = $secondsdaPin) or ($firstsclPin = $secondsclPin)"!]
                                [!ERROR!]
        In [!"$ConfigIndexA"!] the channels [!"$channelSettingMain"!] and [!"$channelSettingSlave"!] have a pin conflict. A pin can be only used once.
                                [!ENDERROR!]
                            [!ENDIF!]
                        [!ENDIF!]
                    [!ENDIF!]
                [!ENDLOOP!]
            [!ENDIF!]
        [!ENDLOOP!]
    [!ENDLOOP!]
[!ENDNOCODE!]
[!ENDMACRO!][!//

[!MACRO "I2c_Dem_Config","Type"!][!//
[!NOCODE!]
    [!VAR "I2cDemEventParameter"!]I2C_E_TIMEOUT_FAILURE[!ENDVAR!]
    [!VAR "DemErrorCount" = "num:i(count(text:split($I2cDemEventParameter, ',')))"!]
[!CODE!]
#if (I2C_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF)
/**
* @brief        DEM error reporting configuration
* @details      Set the state and id for Mcal_DemErrorType.
*/
const I2c_DemConfigType I2c_DemConfig =
{
[!ENDCODE!]
    [!FOR "Index" = "1" TO "num:i($DemErrorCount)"!]
        [!VAR "ErrorName" = "normalize-space(text:split($I2cDemEventParameter, ',')[num:i($Index)])"!]
        [!IF "node:exists(I2cGlobalConfig/I2cDemEventParameterRefs)"!]
            [!VAR "DemRefNode" = "concat('I2cGlobalConfig/I2cDemEventParameterRefs/',$ErrorName)"!]
            [!IF "node:exists($DemRefNode)"!]
                [!IF "node:refvalid($DemRefNode)"!]
[!CODE!]
    /* DEM error reporting [!"$ErrorName"!] */
    {(uint32)STD_ON, DemConf_DemEventParameter_[!"node:ref(node:value($DemRefNode))/@name"!]}[!IF "$Index < $DemErrorCount"!],[!ENDIF!]
[!ENDCODE!]
                [!ELSE!]
                     [!ERROR "Invalid reference for $ErrorName"!]
                [!ENDIF!]
            [!ELSE!]
[!CODE!]
    /* DEM error reporting [!"$ErrorName"!] */
    {(uint32)STD_OFF, 0U}[!IF "$Index < $DemErrorCount"!],[!ENDIF!]
[!ENDCODE!]
            [!ENDIF!]
        [!ELSE!]
[!CODE!]
    /* DEM error reporting [!"$ErrorName"!] */
    {(uint32)STD_OFF, 0U}[!IF "$Index < $DemErrorCount"!],[!ENDIF!]
[!ENDCODE!]
        [!ENDIF!]
    [!ENDFOR!]
[!CODE!]
};
#endif /* (I2C_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF) */
[!ENDCODE!]
[!ENDNOCODE!]
[!ENDMACRO!]

[!MACRO "I2c_ChannelConfig","Type"!][!//
[!NOCODE!]
    [!SELECT "I2cGlobalConfig"!][!//
    
    [!IF "as:modconf('I2c')[1]/GeneralConfiguration/I2cFlexIOUsed='true'"!]
        [!VAR "flexioUsed" = "1"!]
    [!ENDIF!]
    [!IF "num:i(count(I2cChannel/*))>0"!]
        [!VAR "Channel"= "num:i(count(I2cChannel/*))-1"!]
    [!LOOP "I2cChannel/*"!]
        [!IF "contains(I2cHwChannel,'IIC')"!]
[!CODE!]
[!ENDCODE!]
        [!ENDIF!]
    [!ENDLOOP!]

[!CODE!]
/**
* @brief        Definition of I2c hardware channel structure.
* @details      Configures the hardware peripheral of an I2c channel.
*               Configuration set by calling I2c_Init() API.
*/
static const I2c_HwUnitConfigType I2c_ChannelConfig[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i(count(I2cChannel/*))"!]] =
{
[!ENDCODE!]
        [!LOOP "I2cChannel/*"!]
            [!VAR "PCindex" = "@index"!]
            [!IF "as:modconf('I2c')[1]/GeneralConfiguration/I2cMulticoreSupport='true'"!][!//
                [!IF "node:exists(I2cChannelEcucPartitionRef)"!][!//
                    [!VAR "I2cChPartShortName"="count(text:split(./I2cChannelEcucPartitionRef,'/'))"!]
                    [!VAR "I2cChPartShortName"="text:split(./I2cChannelEcucPartitionRef,'/')[num:i($I2cChPartShortName)]"!]
                    [!VAR "I2cChPartShortName" = "text:toupper($I2cChPartShortName)"!][!//
                [!ELSE!][!//
                    [!VAR "I2cChPartShortName" = "'UNALLOCATED'"!][!//
                [!ENDIF!][!//
            [!ELSE!][!//
                [!VAR "I2cChPartShortName" = "'SPURIOUS'"!][!//
            [!ENDIF!][!//
[!CODE!]
    /* Start of I2c_ChannelConfig[[!"@index"!]]*/
    {
        /* The hardware channel ID. */
        (I2c_HwUnitType) I2C_[!"node:value(I2cHwChannel)"!],

        /* The Partition Id */
        (I2c_PartCoreType)I2C_[!"$I2cChPartShortName"!]_CORE_ID,

        /* The master/slave mode of the channel. */
        I2C_[!"I2cMasterSlaveConfiguration"!],

        /* The hardware channel type */[!//
        [!IF "contains(I2cHwChannel,'LPI2C')"!]
        I2C_LPI2C_CHANNEL,
        [!ELSE!]
        I2C_FLEXIO_CHANNEL,
        [!ENDIF!]
        {[!//
            [!IF "as:modconf('I2c')[1]/GeneralConfiguration/I2cFlexIOUsed='false'"!]
                [!IF "contains(I2cHwChannel,'LPI2C')"!]
            &I2c_Ipw_ChannelConfig[!"num:i(@index)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!//
                [!ENDIF!]
            [!ELSE!]
                [!IF "contains(I2cHwChannel,'LPI2C')"!]
            &I2c_Ipw_ChannelConfig[!"num:i(@index)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
            NULL_PTR
                [!ELSE!]
            NULL_PTR,
            &Flexio_I2cMasterChannel[!"num:i(@index)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
                [!ENDIF!]            
            [!ENDIF!]
        }
    }[!IF "$PCindex < $Channel"!], [!ENDIF!]/* End of I2c_ChannelConfig[!"$Type"!][[!"@index"!]]. */
[!ENDCODE!]
        [!ENDLOOP!]
[!CODE!]
};
[!ENDCODE!]
        [!ENDIF!]
        [!ENDSELECT!]
[!ENDNOCODE!]
[!ENDMACRO!][!//

/* Macro pointer config of postBuildVariant when I2cMulticoreSupport disable */
[!MACRO "I2c_Config","Type"!][!//
[!NOCODE!]
    [!VAR "I2cChPartShortName"="'spurious_core'"!]
    [!VAR "I2cChPartUpperName" = "text:toupper($I2cChPartShortName)"!][!//
[!CODE!]
/**
* @brief          Initialization data for the I2c driver.
* @details        A structure is provided to the I2c initialization routines for configuration.
*                 The configuration structure for [!"$I2cChPartShortName"!].
*
*/
[!IF "(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)"!][!//
/**
* @violates @ref I2c_[!IF "$Type = 'PC'"!]Cfg[!ELSE!]PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ENDIF!]_c_REF_4 An object should be defined at block scope if its
* identifier only appears in a single function.
*/
[!ELSE!]
/**
* @violates @ref I2c_[!IF "$Type = 'PC'"!]Cfg[!ELSE!]PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ENDIF!]_c_REF_3 Functions and objects should not be defined with external
* linkage if they are referenced in only one translation unit.
*/
/**
* @violates @ref I2c_[!IF "$Type = 'PC'"!]Cfg[!ELSE!]PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ENDIF!]_c_REF_5 Function with external linkage is not visible.
*/
[!ENDIF!][!//
[!IF "(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)"!][!//
static const I2c_ConfigType I2c_PBCfgVariantPredefined_[!"$I2cChPartShortName"!] =
[!ELSE!]
const I2c_ConfigType I2c_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
[!ENDIF!][!//
{
    /* The Partition Core Id */
    (I2c_PartCoreType)I2C_[!"$I2cChPartUpperName"!]_ID,

#if (I2C_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF)
    /* Pointer to DEM error reporting configurations. */
    &I2c_DemConfig,
#endif

    /* Pointer to I2C Hardware Unit configuration. */
    &I2c_ChannelConfig[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
};
[!ENDCODE!]
[!ENDNOCODE!]
[!ENDMACRO!][!//

# MACROs returning a boolean are bugged. Return 0 for 'false' or > 0 for 'true'.
[!MACRO "IsPartitionUsed"!][!//
    [!VAR "IsUsed" = "num:i(0)"!]

    [!IF "node:containsValue(../../I2cChannel/*/I2cChannelEcucPartitionRef, node:value(.))"!]
        [!VAR "IsUsed" = "num:i(1)"!]
    [!ENDIF!]
    [!"$IsUsed"!]
[!ENDMACRO!][!//

# MACRO returning Max Core ID.
[!MACRO "MaxCoreIdUsed"!][!//
[!NOCODE!]
[!VAR "MaxCoreId"="0"!]
[!IF "as:modconf('I2c')[1]/GeneralConfiguration/I2cMulticoreSupport='true'"!][!//
    [!LOOP "as:modconf('EcuC')[1]/EcucHardware/*[1]/EcucCoreDefinition/*"!][!//
            [!IF "$MaxCoreId < node:value(./EcucCoreId)"!]
                [!VAR "MaxCoreId"="node:value(./EcucCoreId)"!]
            [!ENDIF!]
    [!ENDLOOP!][!//
[!ENDIF!]
[!ENDNOCODE!]
[!"$MaxCoreId"!]
[!ENDMACRO!][!//

/* Macro pointer config of postBuildVariant when I2cMulticoreSupport enable */
[!MACRO "I2c_Config_Multicore","Type"!][!//
[!NOCODE!]
    [!LOOP "I2cGlobalConfig/I2cEcucPartitionRef/*"!]
        [!VAR "I2cChPartRef" = "node:value(.)"!][!//
        [!VAR "I2cChPartShortName"="count(text:split($I2cChPartRef,'/'))"!]
        [!VAR "I2cChPartShortName"="text:split($I2cChPartRef,'/')[num:i($I2cChPartShortName)]"!]
        [!VAR "I2cChPartUpperName" = "text:toupper($I2cChPartShortName)"!][!//
        [!VAR "PartitionUsed"!][!CALL "IsPartitionUsed"!][!ENDVAR!]
        [!IF "num:i($PartitionUsed) > num:i(0)"!]
[!CODE!]
/**
* @brief          Initialization data for the I2c driver.
* @details        A structure is provided to the I2c initialization routines for configuration.
*                 The configuration structure for [!"$I2cChPartShortName"!].
*
*/
[!IF "(../../../IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)"!][!//
/**
* @violates @ref I2c_[!IF "$Type = 'PC'"!]Cfg[!ELSE!]PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ENDIF!]_c_REF_4 An object should be defined at block scope if its
* identifier only appears in a single function.
*/
[!ELSE!]
/**
* @violates @ref I2c_[!IF "$Type = 'PC'"!]Cfg[!ELSE!]PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ENDIF!]_c_REF_3 Functions and objects should not be defined with external
* linkage if they are referenced in only one translation unit.
*/
/**
* @violates @ref I2c_[!IF "$Type = 'PC'"!]Cfg[!ELSE!]PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ENDIF!]_c_REF_5 Function with external linkage is not visible.
*/
[!ENDIF!][!//
[!IF "(../../../IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)"!][!//
static const I2c_ConfigType I2c_PBCfgVariantPredefined_[!"$I2cChPartShortName"!] =
[!ELSE!]
const I2c_ConfigType I2c_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_[!"$I2cChPartShortName"!] =
[!ENDIF!][!//
{
    /* The Partition Core Id */
    (I2c_PartCoreType)I2C_[!"$I2cChPartUpperName"!]_CORE_ID,

#if (I2C_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF)
    /* Pointer to DEM error reporting configurations. */
    &I2c_DemConfig,
#endif

    /* Pointer to I2C Hardware Unit configuration. */
    &I2c_ChannelConfig[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
};
[!ENDCODE!]
        [!ENDIF!][!//num:i($PartitionUsed) > num:i(0)
    [!ENDLOOP!][!//I2cGlobalConfig/I2cEcucPartitionRef/*
[!ENDNOCODE!]
[!ENDMACRO!][!//

# MACRO generate core id used in each VS.
[!MACRO "I2cParititionName"!][!//
[!NOCODE!]
[!CODE!]
/**
* @brief          Paritition IDs.
* @details        Paritition IDs.
*
* @api
*/
[!ENDCODE!][!//
[!LOOP "as:modconf('I2c')[1]/I2cGlobalConfig/I2cEcucPartitionRef/*"!][!//
    [!VAR "I2cChPartRef" = "node:value(.)"!][!//
    [!VAR "I2cChPartShortName"="count(text:split($I2cChPartRef,'/'))"!]
    [!VAR "I2cChPartShortName"="text:split($I2cChPartRef,'/')[num:i($I2cChPartShortName)]"!]
    [!VAR "I2cChPartShortName" = "text:toupper($I2cChPartShortName)"!][!//
    [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
        [!IF "$I2cChPartRef = node:value(./OsAppEcucPartitionRef)"!][!//
            [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
                [!CODE!]
#define I2C_[!"$I2cChPartShortName"!]_CORE_ID       ([!"node:value(./EcucCoreId)"!]U)
                [!ENDCODE!]
            [!ENDSELECT!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDLOOP!][!//
[!ENDNOCODE!]
[!ENDMACRO!][!//

/* Macro pointer config of variant predefined */
[!MACRO "I2c_Config_Part_Predefined"!][!//
[!NOCODE!]
[!IF "(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)"!][!//
[!CODE!][!//
/**
* @brief          Initialization data for the I2c driver.
* @details        A pointer to such a structure is provided to the I2c initialization routines for configuration.
*
*/
/**
* @violates @ref I2c_[!IF "$Type = 'PC'"!]Cfg[!ELSE!]PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ENDIF!]_c_REF_4 An object should be defined at block scope if its
* identifier only appears in a single function.
*/
const I2c_ConfigType * const I2c_PBCfgVariantPredefined[I2C_MAX_CORE_ID] =
{[!//
    /** @brief  Pointer to configuration structure of I2c */
    &I2c_PBCfgVariantPredefined_[!"$I2cChPartShortName"!]
};
[!ENDCODE!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//


/* Macro pointer config of variant predefined support multicore */
[!MACRO "I2c_Config_Part_Predefined_Multicore"!][!//
[!NOCODE!]
[!IF "(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)"!][!//
    [!VAR "NumOfPartCfg" = "num:i(count(I2cGlobalConfig/I2cEcucPartitionRef/*))"!][!//
    [!VAR "NumOfCore"!] [!CALL "MaxCoreIdUsed"!][!ENDVAR!][!//
[!CODE!][!//
/**
* @brief          Initialization data for the I2c driver.
* @details        A pointer to such a structure is provided to the I2c initialization routines for configuration.
*
*/
/**
* @violates @ref I2c_[!IF "$Type = 'PC'"!]Cfg[!ELSE!]PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ENDIF!]_c_REF_4 An object should be defined at block scope if its
* identifier only appears in a single function.
*/
const I2c_ConfigType * const I2c_PBCfgVariantPredefined[I2C_MAX_CORE_ID] =
{[!//
[!ENDCODE!][!//
[!FOR "CurrentCoreId" = "0" TO "$NumOfCore"!]
    [!VAR "HasCore" = "0"!]
    [!VAR "HasPartition" = "0"!]
    [!VAR "foundPartition" = "0"!]

    [!LOOP "I2cGlobalConfig/I2cEcucPartitionRef/*"!]
        [!IF "num:i($foundPartition) = 0"!]
            [!VAR "I2cEcucPartitionRefName" = "node:value(.)"!]
            [!VAR "currentPartition" = "substring-after(substring-after(substring-after(substring-after(.,'/'),'/'),'/'),'/')"!][!//?

            [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
                [!IF "$I2cEcucPartitionRefName = node:value(./OsAppEcucPartitionRef)"!]
                    [!SELECT "node:ref(./OsApplicationCoreRef)"!]
                        [!VAR "OsCoreId" = "node:value(./EcucCoreId)"!]

                        [!IF "num:i($OsCoreId) = num:i($CurrentCoreId)"!]
                            [!VAR "HasCore" = "1"!]

                            [!LOOP "as:modconf('I2c')[1]/I2cGlobalConfig/I2cChannel/*/I2cChannelEcucPartitionRef"!]
                                [!VAR "I2cChannelEcucPartitionRefName" = "node:value(.)"!]

                                [!IF "$I2cChannelEcucPartitionRefName = $I2cEcucPartitionRefName"!]
                                    [!VAR "HasPartition" = "1"!]
                                    [!VAR "foundPartition" = "1"!]
                                [!ENDIF!]
                            [!ENDLOOP!]
                        [!ENDIF!]
                    [!ENDSELECT!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ENDIF!]
    [!ENDLOOP!]

    [!IF "$HasCore = 1"!]
        [!IF "$HasPartition = 1"!]
            [!CODE!]
    /** @brief  Pointer to configuration structure of I2c for [!"$currentPartition"!] refered by core [!"num:i($CurrentCoreId)"!] */
    &I2c_PBCfgVariantPredefined_[!"$currentPartition"!][!//
            [!ENDCODE!]
        [!ELSE!]
            [!CODE!]
    /** @brief  No configuration structure of I2c refered by core [!"num:i($CurrentCoreId)"!] */
    NULL_PTR[!//
            [!ENDCODE!]
        [!ENDIF!]
    [!ELSE!]
        [!CODE!]
    /** @brief  No configuration structure of I2c refered by core [!"num:i($CurrentCoreId)"!] */
    NULL_PTR[!//
        [!ENDCODE!]
    [!ENDIF!]
[!IF "$CurrentCoreId < $NumOfCore"!][!CODE!],[!ENDCODE!][!ENDIF!][!CR!]
[!ENDFOR!]
[!CODE!][!//
};
[!ENDCODE!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!ENDIF!][!// avoid multiple inclusion ENDIF
[!ENDNOCODE!]