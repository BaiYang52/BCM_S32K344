[!NOCODE!]
/**
    @file    Mcu_RegOperations.m
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

[!MACRO "Mcu_CheckDuplicates"!]
[!NOCODE!]
        [!VAR "Mac_ConfigIndexA" = "@name"!]
        [!LOOP "McuModuleConfiguration/McuClockSettingConfig/*"!]
            [!VAR "Mac_clockSettingMain" = "@name"!]
                [!VAR "Mac_ConfigIndexB" = "@name"!]
                [!IF "$Mac_ConfigIndexA!=$Mac_ConfigIndexB"!]
                    [!LOOP "McuModuleConfiguration/McuClockSettingConfig/*"!]
                        [!VAR "Mac_clockSettingSlave" = "@name"!]
                        [!IF "$Mac_clockSettingMain=$Mac_clockSettingSlave"!]
                            [!ERROR!]
    [!"$Mac_ConfigIndexA"!] contains McuClockSettingConfig [!"$Mac_clockSettingMain"!] which is duplicated in [!"$Mac_ConfigIndexB"!]
                            [!ENDERROR!]
                        [!ENDIF!]
                    [!ENDLOOP!]
                [!ENDIF!]
        [!ENDLOOP!]

        [!VAR "Mac_ConfigIndexA" = "@name"!]
        [!LOOP "McuModuleConfiguration/McuModeSettingConf/*"!]
            [!VAR "Mac_modeSettingMain" = "@name"!]
                [!VAR "Mac_ConfigIndexB" = "@name"!]
                [!IF "$Mac_ConfigIndexA!=$Mac_ConfigIndexB"!]
                    [!LOOP "McuModuleConfiguration/McuModeSettingConf/*"!]
                        [!VAR "Mac_modeSettingSlave" = "@name"!]
                        [!IF "$Mac_modeSettingMain=$Mac_modeSettingSlave"!]
                            [!ERROR!]
    [!"$Mac_ConfigIndexA"!] contains McuModeSettingConf [!"$Mac_modeSettingMain"!] which is duplicated in [!"$Mac_ConfigIndexB"!]
                            [!ENDERROR!]
                        [!ENDIF!]
                    [!ENDLOOP!]
                [!ENDIF!]
        [!ENDLOOP!]

        [!VAR "Mac_ConfigIndexA" = "@name"!]
        [!LOOP "McuModuleConfiguration/McuRamSectorSettingConf/*"!]
            [!VAR "Mac_ramSettingMain" = "@name"!]
                [!VAR "Mac_ConfigIndexB" = "@name"!]
                [!IF "$Mac_ConfigIndexA!=$Mac_ConfigIndexB"!]
                    [!LOOP "McuModuleConfiguration/McuRamSectorSettingConf/*"!]
                        [!VAR "Mac_ramSettingSlave" = "@name"!]
                        [!IF "$Mac_ramSettingMain=$Mac_ramSettingSlave"!]
                            [!ERROR!]
    [!"$Mac_ConfigIndexA"!] contains McuRamSectorSettingConf [!"$Mac_ramSettingMain"!] which is duplicated in [!"$Mac_ConfigIndexB"!]
                            [!ENDERROR!]
                        [!ENDIF!]
                    [!ENDLOOP!]
                [!ENDIF!]
        [!ENDLOOP!]
[!ENDNOCODE!]
[!ENDMACRO!][!//


[!MACRO "Mcu_Dem_Config"!][!//
[!NOCODE!]
    [!VAR "Mac_McuDemEventParameter"!]MCU_E_TIMEOUT_FAILURE,MCU_E_INVALIDFXOSC_CONFIG,MCU_E_CLOCKMUXSWITCH_FAILURE,MCU_E_CLOCK_FAILURE,MCU_E_SWITCHMODE_FAILURE[!ENDVAR!]
    [!VAR "Mac_DemErrorCount" = "num:i(count(text:split($Mac_McuDemEventParameter, ',')))"!]
[!CODE!]
#if (MCU_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF)
/**
* @brief        DEM error reporting configuration
* @details      Set the state and id for Mcal_DemErrorType.
*/
const Mcu_DemConfigType Mcu_DemConfig =
{
[!ENDCODE!]
    [!FOR "Index" = "1" TO "num:i($Mac_DemErrorCount)"!]
        [!VAR "Mac_ErrorName" = "normalize-space(text:split($Mac_McuDemEventParameter, ',')[num:i($Index)])"!]
        [!IF "node:exists(McuModuleConfiguration/McuDemEventParameterRefs)"!]
            [!VAR "Mac_DemRefNode" = "concat('McuModuleConfiguration/McuDemEventParameterRefs/',$Mac_ErrorName)"!]
            [!IF "node:exists($Mac_DemRefNode)"!]
                [!IF "node:refvalid($Mac_DemRefNode)"!]
[!CODE!]
    /* DEM error reporting [!"$Mac_ErrorName"!] */
    {(uint32)STD_ON, DemConf_DemEventParameter_[!"node:ref(node:value($Mac_DemRefNode))/@name"!]}[!IF "$Index < $Mac_DemErrorCount"!],[!ENDIF!]
[!ENDCODE!]
                [!ELSE!]
                     [!ERROR "Invalid reference for $Mac_ErrorName"!]
                [!ENDIF!]
            [!ELSE!]
[!CODE!]
    /* DEM error reporting [!"$Mac_ErrorName"!] */
    {(uint32)STD_OFF, 0U}[!IF "$Index < $Mac_DemErrorCount"!],[!ENDIF!]
[!ENDCODE!]
            [!ENDIF!]
        [!ELSE!]
[!CODE!]
    /* DEM error reporting [!"$Mac_ErrorName"!] */
    {(uint32)STD_OFF, 0U}[!IF "$Index < $Mac_DemErrorCount"!],[!ENDIF!]
[!ENDCODE!]
        [!ENDIF!]
    [!ENDFOR!]
[!CODE!]
};
#endif /* (MCU_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF) */
[!ENDCODE!]
[!ENDNOCODE!]
[!ENDMACRO!]


[!MACRO "Mcu_Config","Type"!][!//
[!NOCODE!]
[!CODE!]
/**
* @brief          Initialization data for the MCU driver.
* @details        A pointer to such a structure is provided to the MCU initialization routines for configuration.
*
*/

const Mcu_ConfigType [!IF "(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)"!]Mcu_PreCompileConfig[!ELSE!]Mcu_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ENDIF!] =
{
#if (MCU_INIT_CLOCK == STD_ON)
    /* Globally enable/disable SCM, loss of clock and loss of lock notification. */
    [!IF "McuModuleConfiguration/McuClockSrcFailureNotification = 'ENABLED'"!]MCU_CLK_NOTIF_EN[!ELSE!]MCU_CLK_NOTIF_DIS[!ENDIF!],
#endif

#if (MCU_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF)
    /* Pointer to DEM error reporting configurations. */
    &Mcu_DemConfig,
#endif
    /* Number of RAM Sections configurations. */
    (Mcu_RamSectionType)[!"num:i(count(McuModuleConfiguration/McuRamSectorSettingConf/*))"!]U,

    /* Number of Power Modes configurations. */
    (Mcu_ModeType)[!"num:i(count(McuModuleConfiguration/McuModeSettingConf/*))"!]U,

#if (MCU_INIT_CLOCK == STD_ON)
    /* Number of Clock Setting configurations. */
    (Mcu_ClockType)[!"num:i(count(McuModuleConfiguration/McuClockSettingConfig/*))"!]U,
#endif /* (MCU_INIT_CLOCK == STD_ON) */

#ifndef MCU_MAX_NORAMCONFIGS
    /* Pointer to RAM Section configurations. */
    [!IF "num:i(count(McuModuleConfiguration/McuRamSectorSettingConf/*))>0"!]&Mcu_aRamConfig[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],[!ELSE!](NULL_PTR),[!ENDIF!]
#endif

    /* Pointer to Power Mode configurations. */
    [!IF "num:i(count(McuModuleConfiguration/McuModeSettingConf/*))>0"!]&Power_Ip_aModeConfig[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],[!ELSE!](NULL_PTR),[!ENDIF!]

#if (MCU_INIT_CLOCK == STD_ON)
    /* Pointer to Clock configurations. */
    [!IF "num:i(count(McuModuleConfiguration/McuClockSettingConfig/*))>0"!]&Mcu_aClockConfig[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],[!ELSE!](NULL_PTR),[!ENDIF!]
#endif /* (MCU_INIT_CLOCK == STD_ON) */

    /* Pointer to Low Level Mcu driver configuration. */
    &Power_Ip_HwIPsConfig[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
};
[!ENDCODE!]
[!ENDNOCODE!]
[!ENDMACRO!][!//


[!ENDNOCODE!]
