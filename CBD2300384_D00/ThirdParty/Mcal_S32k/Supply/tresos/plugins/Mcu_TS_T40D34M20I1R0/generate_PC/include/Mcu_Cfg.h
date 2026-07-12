/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : 
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

#ifndef MCU_CFG_H
#define MCU_CFG_H

/**
*   @file       Mcu_Cfg.h
*   @implements Mcu_Cfg.h_Artifact
*   @version    2.0.1
*
*   @brief      AUTOSAR Mcu - Configuration file for the driver.
*   @details    Precompile parameters and extern configuration.
*
*   @addtogroup MCU
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "Mcal.h"
#include "Clock_Ip_Cfg.h"
#include "Power_Ip_Cfg.h"
#include "Ram_Ip_Cfg.h"
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
#include "Mcu_[!"."!]_PBcfg.h"
[!ENDLOOP!][!//
[!ELSE!][!//
#include "Mcu_PBcfg.h"
[!ENDIF!][!//

/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define MCU_CFG_VENDOR_ID                       43
#define MCU_CFG_AR_RELEASE_MAJOR_VERSION        4
#define MCU_CFG_AR_RELEASE_MINOR_VERSION        4
#define MCU_CFG_AR_RELEASE_REVISION_VERSION     0
#define MCU_CFG_SW_MAJOR_VERSION                2
#define MCU_CFG_SW_MINOR_VERSION                0
#define MCU_CFG_SW_PATCH_VERSION                1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if header file and Mcal header file are of the same Autosar version */
#if ((MCU_CFG_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
     (MCU_CFG_AR_RELEASE_MINOR_VERSION != MCAL_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Mcu_Cfg.h and Mcal.h are different"
#endif
#endif

/* Check if header file and Clock_Ip_Cfg.h file are of the same vendor */
#if (MCU_CFG_VENDOR_ID != CLOCK_IP_CFG_VENDOR_ID)
    #error "Mcu_Cfg.h and Clock_Ip_Cfg.h have different vendor ids"
#endif

/* Check if header file and Clock_Ip_Cfg.h file are of the same Autosar version */
#if ((MCU_CFG_AR_RELEASE_MAJOR_VERSION != CLOCK_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (MCU_CFG_AR_RELEASE_MINOR_VERSION != CLOCK_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (MCU_CFG_AR_RELEASE_REVISION_VERSION != CLOCK_IP_CFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Mcu_Cfg.h and Clock_Ip_Cfg.h are different"
#endif

/* Check if header file and Clock_Ip_Cfg.h file are of the same Software version */
#if ((MCU_CFG_SW_MAJOR_VERSION != CLOCK_IP_CFG_SW_MAJOR_VERSION) || \
     (MCU_CFG_SW_MINOR_VERSION != CLOCK_IP_CFG_SW_MINOR_VERSION) || \
     (MCU_CFG_SW_PATCH_VERSION != CLOCK_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Mcu_Cfg.h and Clock_Ip_Cfg.h are different"
#endif

/* Check if header file and Power_Ip_Cfg.h file are of the same vendor */
#if (MCU_CFG_VENDOR_ID != POWER_IP_CFG_VENDOR_ID)
    #error "Mcu_Cfg.h and Power_Ip_Cfg.h have different vendor ids"
#endif

/* Check if header file and Power_Ip_Cfg.h file are of the same Autosar version */
#if ((MCU_CFG_AR_RELEASE_MAJOR_VERSION != POWER_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (MCU_CFG_AR_RELEASE_MINOR_VERSION != POWER_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (MCU_CFG_AR_RELEASE_REVISION_VERSION != POWER_IP_CFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Mcu_Cfg.h and Power_Ip_Cfg.h are different"
#endif

/* Check if header file and Power_Ip_Cfg.h file are of the same Software version */
#if ((MCU_CFG_SW_MAJOR_VERSION != POWER_IP_CFG_SW_MAJOR_VERSION) || \
     (MCU_CFG_SW_MINOR_VERSION != POWER_IP_CFG_SW_MINOR_VERSION) || \
     (MCU_CFG_SW_PATCH_VERSION != POWER_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Mcu_Cfg.h and Power_Ip_Cfg.h are different"
#endif

/* Check if header file and Ram_Ip_Cfg.h file are of the same vendor */
#if (MCU_CFG_VENDOR_ID != RAM_IP_CFG_VENDOR_ID)
    #error "Mcu_Cfg.h and Ram_Ip_Cfg.h have different vendor ids"
#endif

/* Check if header file and Ram_Ip_Cfg.h file are of the same Autosar version */
#if ((MCU_CFG_AR_RELEASE_MAJOR_VERSION != RAM_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (MCU_CFG_AR_RELEASE_MINOR_VERSION != RAM_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (MCU_CFG_AR_RELEASE_REVISION_VERSION != RAM_IP_CFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Mcu_Cfg.h and Ram_Ip_Cfg.h are different"
#endif

/* Check if header file and Ram_Ip_Cfg.h file are of the same Software version */
#if ((MCU_CFG_SW_MAJOR_VERSION != RAM_IP_CFG_SW_MAJOR_VERSION) || \
     (MCU_CFG_SW_MINOR_VERSION != RAM_IP_CFG_SW_MINOR_VERSION) || \
     (MCU_CFG_SW_PATCH_VERSION != RAM_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Mcu_Cfg.h and Ram_Ip_Cfg.h are different"
#endif

[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
/* Check if header file and Mcu_[!"."!]_PBcfg.h file are of the same vendor */
#if (MCU_CFG_VENDOR_ID != MCU_[!"."!]_PBCFG_VENDOR_ID)
    #error "Mcu_Cfg.h and Mcu_[!"."!]_PBcfg.h have different vendor ids"
#endif

/* Check if header file and Mcu_[!"."!]_PBcfg.h file are of the same Autosar version */
#if ((MCU_CFG_AR_RELEASE_MAJOR_VERSION != MCU_[!"."!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (MCU_CFG_AR_RELEASE_MINOR_VERSION != MCU_[!"."!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (MCU_CFG_AR_RELEASE_REVISION_VERSION != MCU_[!"."!]_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Mcu_Cfg.h and Mcu_[!"."!]_PBcfg.h are different"
#endif

/* Check if header file and Mcu_[!"."!]_PBcfg.h file are of the same Software version */
#if ((MCU_CFG_SW_MAJOR_VERSION != MCU_[!"."!]_PBCFG_SW_MAJOR_VERSION) || \
     (MCU_CFG_SW_MINOR_VERSION != MCU_[!"."!]_PBCFG_SW_MINOR_VERSION) || \
     (MCU_CFG_SW_PATCH_VERSION != MCU_[!"."!]_PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Mcu_Cfg.h and Mcu_[!"."!]_PBcfg.h are different"
#endif
[!ENDLOOP!][!//
[!ELSE!][!//
/* Check if header file and Mcu_PBcfg.h file are of the same vendor */
#if (MCU_CFG_VENDOR_ID != MCU_PBCFG_VENDOR_ID)
    #error "Mcu_Cfg.h and Mcu_PBcfg.h have different vendor ids"
#endif

/* Check if header file and Mcu_PBcfg.h file are of the same Autosar version */
#if ((MCU_CFG_AR_RELEASE_MAJOR_VERSION != MCU_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (MCU_CFG_AR_RELEASE_MINOR_VERSION != MCU_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (MCU_CFG_AR_RELEASE_REVISION_VERSION != MCU_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Mcu_Cfg.h and Mcu_PBcfg.h are different"
#endif

/* Check if header file and Mcu_PBcfg.h file are of the same Software version */
#if ((MCU_CFG_SW_MAJOR_VERSION != MCU_PBCFG_SW_MAJOR_VERSION) || \
     (MCU_CFG_SW_MINOR_VERSION != MCU_PBCFG_SW_MINOR_VERSION) || \
     (MCU_CFG_SW_PATCH_VERSION != MCU_PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Mcu_Cfg.h and Mcu_PBcfg.h are different"
#endif
[!ENDIF!][!//
/*==================================================================================================
*                                           DEFINES AND MACROS
==================================================================================================*/

[!IF "not((IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1))"!]
#define MCU_CONFIG_EXT \
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
    MCU_CONFIG_[!"."!]_PB \
[!ENDLOOP!][!//
[!ELSE!][!//
    MCU_CONFIG_PB
[!ENDIF!]
[!ENDIF!]

/**
* @brief      Specifies the InstanceId of this module instance.
*/

#define MCU_INSTANCE_ID                 ((uint8)0x0U)

/**
@{
* @brief            Service Ids for MCU APIs.
*/


#define MCU_INIT_ID                     ((uint8)0x00U)

#define MCU_INITRAMSECTION_ID           ((uint8)0x01U)

#define MCU_INITCLOCK_ID                ((uint8)0x02U)

#define MCU_DISTRIBUTEPLLCLOCK_ID       ((uint8)0x03U)

#define MCU_GETPLLSTATUS_ID             ((uint8)0x04U)

#define MCU_GETRESETREASON_ID           ((uint8)0x05U)

#define MCU_GETRESETRAWVALUE_ID         ((uint8)0x06U)

#define MCU_PERFORMRESET_ID             ((uint8)0x07U)

#define MCU_SETMODE_ID                  ((uint8)0x08U)

#define MCU_GETVERSIONINFO_ID           ((uint8)0x09U)

#define MCU_GETRAMSTATE_ID              ((uint8)0x0AU)

/**@}*/


/**
@{
* @brief            Service Ids for NonASR MCU APIs.
*/


#define MCU_GETPOWERDOMAIN_ID           ((uint8)0x0BU)

#define MCU_GETPERIPHERALSTATE_ID       ((uint8)0x0CU)

#define MCU_GETSYSTEMSTATE_ID           ((uint8)0x0DU)

#define MCU_GETPOWERMODESTATE_ID        ((uint8)0x0EU)

#define MCU_GETMEMCONFIG_ID             ((uint8)0x13U)

#define MCU_GETMIDRSTRUCTURE_ID         ((uint8)0x14U)

#define MCU_SSCMGETSTATUS_ID            ((uint8)0x15U)

#define MCU_SSCMGETUOPT_ID              ((uint8)0x16U)

#define MCU_DISABLECMU_ID               ((uint8)0x17U)

#define MCU_EMIOSCONFIGUREGPREN_ID      ((uint8)0x18U)

#define MCU_GETCLOCKFREQUENCY_ID        ((uint8)0x19U)

#define MCU_SLEEPONEXIT_ID              ((uint8)0x1AU)

#define MCU_SRAMRETENCONFIG_ID          ((uint8)0x1BU)


/**@}*/


/**
* @brief            Pre-processor switch for enabling the default error detection and reporting to the DET.
*                   The detection of default errors is configurable (ON / OFF) at pre-compile time.
*/
#define MCU_DEV_ERROR_DETECT   [!IF "McuGeneralConfiguration/McuDevErrorDetect = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief            Pre-processor switch to enable/disable the API to read out the modules version information.
*/
#define MCU_VERSION_INFO_API   [!IF "McuGeneralConfiguration/McuVersionInfoApi = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]


/**
* @brief            Pre-processor switch to enable/disable the API Mcu_GetRamState.
*/
#define MCU_GET_RAM_STATE_API           [!IF "McuGeneralConfiguration/McuGetRamStateApi = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]


/**
* @brief            If this parameter is set to FALSE, the clock initialization has to be disabled from the MCU driver.
*/
#define MCU_INIT_CLOCK   [!IF "McuGeneralConfiguration/McuInitClock = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]


/**
* @brief            This parameter shall be set True, if the H/W does not have a PLL or the PLL circuitry is enabled after the power on without S/W intervention.
*/
#define MCU_NO_PLL   [!IF "McuGeneralConfiguration/McuNoPll = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief            Support for Low Power mode.
*/
#define MCU_ENTER_LOW_POWER_MODE           POWER_IP_ENTER_LOW_POWER_MODE

/**
* @brief            Pre-processor switch to enable/disable the API Mcu_PerformReset.
*/
#define MCU_PERFORM_RESET_API               POWER_IP_PERFORM_RESET_API

/**
* @brief            define for interrupt CMU, PMC, SCG peripheral
*/
#define MCU_CMU_PMC_SCG_INTERRUPT  [!IF "num:i(ecu:get('MCU.CMU.UNITS')) > num:i(0)"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

#if (MCU_PERFORM_RESET_API == STD_ON)
/**
* @brief            The user callout reset is/isn't available (STD_ON/STD_OFF) - called by MCU right before Mcu_PerformReset().
*/
#define MCU_RESET_CALLOUT_USED   [!IF "(node:exists(McuGeneralConfiguration/McuCalloutBeforePerformReset)) and (McuGeneralConfiguration/McuCalloutBeforePerformReset = 'true')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#endif


[!IF "McuGeneralConfiguration/McuPerformResetApi = 'true'"!][!IF "McuGeneralConfiguration/McuCalloutBeforePerformReset = 'true'"!]
#if ( (MCU_PERFORM_RESET_API == STD_ON) && (MCU_RESET_CALLOUT_USED == STD_ON) )
/**
* @brief            The callout reset procedure configured by the user.
*/
#define MCU_RESET_CALLOUT   ([!"normalize-space(McuGeneralConfiguration/McuPerformResetCallout)"!])
#endif /* ( (MCU_PERFORM_RESET_API == STD_ON) && (MCU_RESET_CALLOUT_USED == STD_ON) ) */
[!ENDIF!][!ENDIF!]

[!IF "node:exists(McuGeneralConfiguration/McuCmuNotification)"!]
/**
* @brief            The callout configured by the user for CMU notifications.
*/

#define MCU_CMU_FCCU_NOTIFICATION(ClockName) ([!"normalize-space(McuGeneralConfiguration/McuCmuNotification)"!](ClockName))
[!ENDIF!]

[!IF "node:exists(McuGeneralConfiguration/McuCguNotification)"!]
/**
* @brief            The callout configured by the user for CGU notifications.
*/

#define MCU_CGU_NOTIFICATION(ClockName, StatusDetect) ([!"normalize-space(McuGeneralConfiguration/McuCguNotification)"!](ClockName, StatusDetect))
[!ENDIF!]


[!IF "node:exists(McuGeneralConfiguration/McuErrorIsrNotification)"!]
/**
* @brief            The callout configured by the user for CMU notifications.
*/

#define MCU_ERROR_ISR_NOTIFICATION(u8ErrorCode) ([!"normalize-space(McuGeneralConfiguration/McuErrorIsrNotification)"!](u8ErrorCode))
[!ENDIF!]

[!IF "node:exists(McuGeneralConfiguration/McuPrepareMemoryConfig)"!]
/**
* @brief            The callout configured by the user for preparing flash and ram controllers configuration.
*/

#define MCU_PREPARE_MEMORY_CONFIG(configStage) ([!"normalize-space(McuGeneralConfiguration/McuPrepareMemoryConfig)"!](configStage))
[!ENDIF!]

[!IF "node:exists(McuGeneralConfiguration/McuPmcNotification)"!]
/**
* @brief            The callout configured by the user for preparing flash and ram controllers configuration.
*/

#define MCU_PMC_NOTIFICATION(ePowerEvent)           ([!"normalize-space(McuGeneralConfiguration/McuPmcNotification)"!](ePowerEvent))
[!ENDIF!]

[!IF "node:exists(McuGeneralConfiguration/McuVeryLowPowerStopAbortNotification)"!]
/**
* @brief            The low power abort notification procedure configured by the user.
*/
#define MCU_VLPSA_NOTIFICATION   ([!"normalize-space(McuGeneralConfiguration/McuVeryLowPowerStopAbortNotification)"!])
[!ENDIF!]

/**
* @brief            ISR Mcu_CmuClockFail_ISR is/isn't available
*/
#define MCU_CMU_ERROR_ISR_USED                  [!IF "McuGeneralConfiguration/McuCmuErrorIsrUsed = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

[!IF "node:exists(McuGeneralConfiguration/McuCguDetectIsrUsed)"!]
/**
* @brief            ISR Mcu_CguClockDetect_ISR is/isn't available
*/
#define MCU_CGU_DETECT_ISR_USED                  [!IF "McuGeneralConfiguration/McuCguDetectIsrUsed = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
[!ELSE!]
/**
* @brief            ISR Mcu_CguClockDetect_ISR is/isn't available
*/
#define MCU_CGU_DETECT_ISR_USED                 (STD_OFF)
[!ENDIF!]


/**
* @brief            Enable the usage of Non-Autosar API Mcu_GetPeripheral_State() for getting infos about peripheral state from MC_ME module.
*/
#define MCU_GET_PERIPH_STATE_API   (STD_OFF)

/**
* @brief            Enable the usage of Non-Autosar API Mcu_GetPowerMode_State() for getting infos system platform configuration.
*/
#define MCU_POWERMODE_STATE_API   [!IF "McuDebugConfiguration/McuGetPowerModeStatetApi = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief            Enable the usage of Non-Autosar API Mcu_GetPowerDomain_Status. Get PMU state: operable or not.
*/
#define MCU_GET_POWER_DOMAIN_API   (STD_OFF)

/**
* @brief            Enable the usage of Non-Autosar API Mcu_GetSystem_State() for getting system platform configuration information. Get (SSCM_STATUS | SSCM_MEMCONFIG).
*/
#define MCU_GET_SYSTEM_STATE_API  (STD_OFF)

/**
* @brief            Enable the usage of Non-Autosar API Mcu_SscmGetMemConfig(). Get SSCM_MEMCONFIG.
*/
#define MCU_GET_MEM_CONFIG_API    (STD_OFF)

/**
* @brief            Enable the usage of Non-Autosar API Mcu_SscmGetStatus(). Get SSCM_STATUS.
*/
#define MCU_SSCM_GET_STATUS_API   (STD_OFF)

/**
* @brief            Enable the usage of Non-Autosar API Mcu_SscmGetUopt(). Get SSCM_UOPT.
*/
#define MCU_SSCM_GET_UOPT_API     (STD_OFF)

/**
* @brief            Enable the usage of Non-Autosar API  Mcu_GetMidrStructure(). Returns .
*/
#define MCU_GET_MIDR_API          [!IF "McuDebugConfiguration/McuGetMidrStructureApi = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief            Enable the usage of Non-Autosar API  Mcu_EmiosConfigureGpren().
*/
#define MCU_EMIOS_CONFIGURE_GPREN_API          [!IF "McuDebugConfiguration/McuEmiosConfigureGprenApi = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief            Enable the usage of Non-Autosar API  Mcu_Disable_CMU().
*/
#define MCU_DISABLE_CMU_API     [!IF "McuDebugConfiguration/McuDisableCmuApi = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief            Enable the usage of Non-Autosar API  Mcu_GetClockFrequency().
*/
#define MCU_GET_CLOCK_FREQUENCY_API     [!IF "McuDebugConfiguration/McuGetClockFrequencyApi = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief            Enable/Disable the API for reporting the Dem Error.
*/
#define MCU_DISABLE_DEM_REPORT_ERROR_STATUS     [!IF "(McuDebugConfiguration/McuDisableDemReportErrorStatus='true') or (num:i(count(McuModuleConfiguration/McuDemEventParameterRefs[node:exists(.)]))=0)"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

[!IF "num:i( count( ecu:list('MCU.Partitions.List') )) > num:i(0)"!]
/**
* @brief            Max number of MC_ME partitions.
*/
#define MCU_MAX_NUMBER_OF_PARTITIONS         ((uint8)[!"num:i( count( ecu:list('MCU.Partitions.List') ) )"!]U)
[!ENDIF!]

/**
* @brief            Enable the usage of Non-Autosar API  Mcu_SRAMRetentionConfig().
*/
#define MCU_SRAM_RETEN_CONFIG_API    [!IF "McuDebugConfiguration/McuSRAMRetentionConfigApi = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief           Enable/Disable the Ram Notification.
*/
#define MCU_DISABLE_RAM_CONFIG         [!IF "node:exists(McuGeneralConfiguration/McuDisableRamWaitStatesConfig)"!][!IF "McuGeneralConfiguration/McuDisableRamWaitStatesConfig = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!][!ELSE!](STD_ON)[!ENDIF!]

/**
* @brief           Enable/Disable the Flash Notification.
*/
#define MCU_DISABLE_FLASH_CONFIG       [!IF "node:exists(McuGeneralConfiguration/McuDisableFlashWaitStatesConfig)"!][!IF "McuGeneralConfiguration/McuDisableFlashWaitStatesConfig = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!][!ELSE!](STD_ON)[!ENDIF!]

/**
* @brief            ISR Mcu_VoltageError_ISR is/isn't available (STD_ON/STD_OFF)
*/
#if (MCU_CMU_PMC_SCG_INTERRUPT == STD_ON)
#define MCU_VOLTAGE_ERROR_ISR_USED   [!IF "McuModuleConfiguration/McuInterruptEvents/McuVoltageErrorEvent = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#endif
/**
* @brief            Maximum number of MCU Clock configurations.
*/[!NOCODE!]
[!VAR "MaxNoOfClkCfgs" = "0"!]
    [!IF "$MaxNoOfClkCfgs < num:i(count(McuModuleConfiguration/McuClockSettingConfig/*))"!]
        [!VAR "MaxNoOfClkCfgs" = "num:i(count(McuModuleConfiguration/McuClockSettingConfig/*))"!]
    [!ENDIF!]
[!ENDNOCODE!]
#define MCU_MAX_CLKCONFIGS   ((uint32)[!"num:i($MaxNoOfClkCfgs)"!]U)


/**
* @brief            Maximum number of MCU Mode configurations.
*/[!NOCODE!]
[!VAR "MaxNoOfModeCfgs" = "0"!]
    [!IF "$MaxNoOfModeCfgs < num:i(count(McuModuleConfiguration/McuModeSettingConf/*))"!]
        [!VAR "MaxNoOfModeCfgs" = "num:i(count(McuModuleConfiguration/McuModeSettingConf/*))"!]
    [!ENDIF!]
[!ENDNOCODE!]
#define MCU_MAX_MODECONFIGS   ((uint32)[!"num:i($MaxNoOfModeCfgs)"!]U)


/**
* @brief            Maximum number of MCU Ram configurations.
*/[!NOCODE!]
[!VAR "MaxNoOfRamCfgs" = "0"!]
    [!IF "$MaxNoOfRamCfgs < num:i(McuModuleConfiguration/McuRamSectors)"!]
        [!VAR "MaxNoOfRamCfgs" = "num:i(McuModuleConfiguration/McuRamSectors)"!]
    [!ENDIF!]
[!ENDNOCODE!]
#define MCU_MAX_RAMCONFIGS   ((uint32)[!"num:i($MaxNoOfRamCfgs)"!]U)
[!NOCODE!]
    [!IF "$MaxNoOfRamCfgs = 0"!]
    [!CODE!]#define MCU_MAX_NORAMCONFIGS[!ENDCODE!]
    [!ENDIF!]
[!ENDNOCODE!]


/**
* @brief            Pre-compile Support.
*/
#define MCU_PRECOMPILE_SUPPORT   [!IF "(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

#ifdef MCU_GET_MIDR_API
  #if (MCU_GET_MIDR_API == STD_ON)
/**
* @brief            Number of total available SIUL2 (SIUL2 + SIUL2_AE) units.
*/

#define MCU_SIUL2_TOTAL_UNITS                    ((uint8)[!"num:i(count(ecu:list('MCU.SIUL2.List')))"!]U)
  #endif
#endif

/**
* @brief            Number of Clock Muxes per CGM module
*/
[!NOCODE!]
  [!VAR "NumOfCgmModules" = "num:i(count(ecu:list('MCU.CGM.CgmModules.List')))"!]
  [!FOR "CgmCounter" = "1" TO "$NumOfCgmModules"!]
[!CODE!]
#define MCU_CGM_[!"substring-after(ecu:list('MCU.CGM.CgmModules.List')[number($CgmCounter)], 'Cgm')"!]_NUMBER_OF_CLK_MUX        ((uint8)[!"num:i(count( ecu:list(concat('MCU.', text:toupper(ecu:list('MCU.CGM.CgmModules.List')[number($CgmCounter)]), '.ClockMuxes.List')) ))"!]U)
[!ENDCODE!]
  [!ENDFOR!]
[!ENDNOCODE!]

/**
* @brief            Index of CGMs module
*/
[!NOCODE!]
    [!VAR "NumOfCgmModules" = "num:i(count(ecu:list('MCU.CGM.CgmModules.List')))"!]
    [!FOR "CgmCounter" = "1" TO "$NumOfCgmModules"!]
        [!VAR "CgmEffectiveIndex" = "substring-after(ecu:list('MCU.CGM.CgmModules.List')[number($CgmCounter)], 'Cgm')"!]
[!CODE!]

#define MCU_CGM_[!"$CgmEffectiveIndex"!]_INDEX_U8        ((uint8)[!"num:i($CgmCounter - 1)"!]U)
[!ENDCODE!]
  [!ENDFOR!]
[!ENDNOCODE!]

#ifdef MCU_ERROR_ISR_NOTIFICATION
/* This is used to clear CMU flags from the RGM interrupt */
#define MCU_CMU_CLEAR_CLOCK_IRQ_FLAG            (MCU_CMU_ERROR_ISR_USED)
#endif

#if !(defined (CLOCK_IP_S32K1) || defined (CLOCK_IP_SJA11))
/**
* @brief        Support for User mode.
*               If this parameter has been configured to 'TRUE' the Mcu driver can be executed from both supervisor and user mode.
*/
#define MCU_ENABLE_USER_MODE_SUPPORT  [!IF "McuGeneralConfiguration/McuEnableUserModeSupport = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/** Check the driver user mode is enabled only when the MCAL_ENABLE_USER_MODE_SUPPORT is enabled */
#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
  #if (MCU_ENABLE_USER_MODE_SUPPORT == STD_ON)
    #error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running Mcu in user mode the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined.
  #endif /* (MCU_ENABLE_USER_MODE_SUPPORT == STD_ON) */
#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT */

#endif /* !(defined (CLOCK_IP_S32K1) || defined (CLOCK_IP_SJA11)) */


[!IF "ecu:get('MCU.SLEEPONEXIT.SUPPORT') = 'true'"!]
/**
* @brief           This define controls the availability of function Mcu_SleepOnExit
*/
#define MCU_SLEEPONEXIT_SUPPORT     (STD_ON)
[!ENDIF!]
/*==================================================================================================
                                           DEFINES AND MACROS
==================================================================================================*/
#if (MCU_DEV_ERROR_DETECT == STD_ON)
/**
@{
* @brief            Default error values are of type uint8.
*                   The following errors and exceptions shall be detectable by the MCU module depending on its
*                   build version (Default/production mode).
*
*/

#define MCU_E_PARAM_CONFIG              ((uint8)0x0AU)

#if (MCU_INIT_CLOCK == STD_ON)
    #define MCU_E_PARAM_CLOCK               ((uint8)0x0BU)
#endif

#define MCU_E_PARAM_MODE                ((uint8)0x0CU)

#ifndef MCU_MAX_NORAMCONFIGS
    #define MCU_E_PARAM_RAMSECTION          ((uint8)0x0DU)
#endif

#if (MCU_INIT_CLOCK == STD_ON) && (MCU_NO_PLL == STD_OFF)
    #define MCU_E_PLL_NOT_LOCKED            ((uint8)0x0EU)
#endif

#define MCU_E_UNINIT                    ((uint8)0x0FU)

#if (MCU_VERSION_INFO_API == STD_ON) || (MCU_GET_MIDR_API == STD_ON)
    #define MCU_E_PARAM_POINTER             ((uint8)0x10U)
#endif

#define MCU_E_INIT_FAILED               ((uint8)0x11U)
#define MCU_E_ALREADY_INITIALIZED       ((uint8)0x13U)

#if (MCU_DISABLE_CMU_API == STD_ON)
    #define MCU_E_CMU_INDEX_OUT_OF_RANGE    ((uint8)0x22U)
#endif

/**
* @brief  The function Mcu_GetResetRawValue shall return an implementation specific value which does
*         not correspond to a valid value of the reset status register and is not equal to 0 if this
*         function is called prior to calling of the function Mcu_Init, and if supported by the
*         hardware.
*
*/
#define MCU_RAW_RESET_DEFAULT      ((uint32)0xFFFFFFFFUL)

/**@}*/
#endif /* (MCU_DEV_ERROR_DETECT == STD_ON) */


#ifdef MCU_ERROR_ISR_NOTIFICATION
/**
@{
* @brief            Error ISR values are of type uint8.
*                   The following error codes are reported by the error ISR.
*/
#if (MCU_CMU_ERROR_ISR_USED == STD_ON)
    #define MCU_E_ISR_CLOCK_FAILURE                 ((uint8)0x01U)
#endif

#endif /* MCU_ERROR_ISR_NOTIFICATION */

#ifdef MCU_CGU_NOTIFICATION
/**
@{
* @brief            Detection ISR values are of type uint8.
*                   The following Detection codes are reported by the Detection ISR.
*/
#if defined(MCU_CGU_DETECT_ISR_USED)
#if (MCU_CGU_DETECT_ISR_USED == STD_ON)
    #define MCU_ISR_CLOCK_ACTIVE                 ((uint8)0x01U)
    #define MCU_ISR_CLOCK_INACTIVE               ((uint8)0x02U)
#endif
#endif
#endif /* MCU_CGU_NOTIFICATION */

/**
@{
* @brief            Macro parameter input of function mcu_emiosconfiguregpren().
*/
#ifdef MCU_EMIOS_CONFIGURE_GPREN_API
#if (MCU_EMIOS_CONFIGURE_GPREN_API == STD_ON)

#ifdef IP_EMIOS_0_BASE
#define MCU_EMIOS_MODULE_0 (0x00U)
#endif
#ifdef IP_EMIOS_1_BASE
#define MCU_EMIOS_MODULE_1 (0x01U)
#endif
#ifdef IP_EMIOS_2_BASE
#define MCU_EMIOS_MODULE_2 (0x02U)
#endif

#define MCU_EMIOS_GPREN_BIT_ENABLE (0x01U)
#define MCU_EMIOS_GPREN_BIT_DISABLE (0x00U)

#define MCU_EMIOS_MCR_GPREN_MASK32 eMIOS_MCR_GPREN_MASK

#endif
#endif

/*==================================================================================================
*                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/**
* @brief            The Mcu_ModeType specifies the identification (ID) for a MCU mode, configured via configuration structure.
* @details          The type shall be uint8, uint16 or uint32.
*
* @implements     Mcu_ModeType_typedef
*
*/
typedef uint32 Mcu_ModeType;

/**
* @brief            The Mcu_RamSectionType specifies the identification (ID) for a RAM section, configured via the configuration structure.
*                   The type shall be uint8, uint16 or uint32, based on best performance.
* @implements Mcu_RamSectionType_typedef
*/
typedef uint32 Mcu_RamSectionType;

/**
* @brief            The Mcu_RamWriteSizeType specifies the RAM section write size.
*                   The type shall be uint8, uint16 or uint32, based on best performance.
*
*/
typedef uint32 Mcu_RamWriteSizeType;


#if (MCU_INIT_CLOCK == STD_ON)
/**
* @brief          Defines the identification (ID) for clock setting configured via the configuration structure.
* @details        The type shall be uint8, uint16 or uint32, depending on uC platform.
*
* @implements Mcu_ClockType_typedef
*/
typedef uint32 Mcu_ClockType;
#endif /* (MCU_INIT_CLOCK == STD_ON) */


/**
* @brief            The type Mcu_RawResetType specifies the reset reason in raw register format, read from a reset status register.
* @details          The type shall be uint8, uint16 or uint32 based on best performance.
*
* @implements Mcu_RawResetType_typedef
*
*/
typedef uint32 Mcu_RawResetType;   /**< @brief Destructive and Functional Reset Events Log. */

/**
* @brief            DEM error reporting configuration.
* @details          This structure contains information DEM error reporting
*/
typedef struct
{
    Mcal_DemErrorType ErrorTimeoutFailureCfg;
    Mcal_DemErrorType ErrorInvalidFxoscConfigCfg;
    Mcal_DemErrorType ErrorClockMuxSwitchFailureCfg;
    Mcal_DemErrorType ErrorClockFailureCfg;
    Mcal_DemErrorType ErrorSwitchModeFailureCfg;
} Mcu_DemConfigType;

#if (MCU_POWERMODE_STATE_API == STD_ON)
/**
* @brief            State of the current power mode.
* @details          Used to store the value of register value.
*/
typedef uint32 Mcu_PowerModeStateType;
#endif /* (MCU_POWERMODE_STATE_API == STD_ON) */
/*==================================================================================================
*                                      CONSTANTS
==================================================================================================*/
#if (MCU_INIT_CLOCK == STD_ON)
/**
* @brief            Create defines with the IDs assigned to Mcu Clock configurations.
*                   These IDs will be transmitted as input parameters for Mcu_InitClock() API.
*/
[!LOOP "McuModuleConfiguration/McuClockSettingConfig/*"!]

#define [!"name(.)"!]   ((Mcu_ClockType)[!"num:i( num:i(McuClockSettingId) )"!]U)

#define McuConf_McuClockSettingConfig_[!"name(.)"!]   ((Mcu_ClockType)[!"num:i( num:i(McuClockSettingId) )"!]U)
[!ENDLOOP!]

#endif

/**
* @brief            Create defines with the IDs assigned to Mcu Mode configurations.
*                   These IDs will be transmitted as input parameters for Mcu_SetMode() API.
*/
[!LOOP "McuModuleConfiguration/McuModeSettingConf/*"!]

#define [!"name(.)"!]   ((Mcu_ModeType)[!"num:i( num:i(McuMode) )"!]U)

#define McuConf_McuModeSettingConf_[!"name(.)"!]   ((Mcu_ModeType)[!"num:i( num:i(McuMode) )"!]U)
[!ENDLOOP!]

/**
* @brief            Create defines with the IDs assigned to Mcu RAM Section configurations.
*                   These IDs will be transmitted as input parameters for Mcu_InitRamSection() API.
*/
[!LOOP "McuModuleConfiguration/McuRamSectorSettingConf/*"!]

#define [!"name(.)"!]   ((Mcu_RamSectionType)[!"num:i(McuRamSectorId)"!]U)

#define McuConf_McuRamSectorSettingConf_[!"name(.)"!]   ((Mcu_RamSectionType)[!"num:i(McuRamSectorId)"!]U)
[!ENDLOOP!]


/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/**
* @brief            Type of the return value of the function Mcu_GetPllStatus.
* @details          The type of Mcu_PllStatusType is an enumeration with the following values:
*                       MCU_PLL_LOCKED, MCU_PLL_UNLOCKED, MCU_PLL_STATUS_UNDEFINED.
*
* @implements     Mcu_PllStatusType_Enumeration
*
*/
typedef enum
{
    MCU_PLL_LOCKED = 0x33U,            /**< @brief PLL is locked.         */
    MCU_PLL_UNLOCKED = 0xCCU,          /**< @brief PLL is unlocked.       */
    MCU_PLL_STATUS_UNDEFINED = 0x5AU   /**< @brief PLL Status is unknown. */

} Mcu_PllStatusType;


/**
* @brief            The type Mcu_ResetType, represents the different reset that a specified MCU can have.
* @details          The MCU module shall provide at least the values MCU_POWER_ON_RESET and MCU_RESET_UNDEFINED for the enumeration Mcu_ResetType.
*
* @implements Mcu_ResetType_Enumeration
*/
typedef Power_Ip_ResetType Mcu_ResetType;

#if (MCU_GET_RAM_STATE_API == STD_ON)
/**
* @brief            Ram State of the microcontroller.
* @details          This is the Ram State data type returned by the function Mcu_GetRamState() of the Mcu module.
* @implements Mcu_RamStateType_Enumeration
*/
typedef enum
{
    MCU_RAMSTATE_INVALID = 0x99U,   /**< @brief RAM content is not valid or unknown (default). */
    MCU_RAMSTATE_VALID   = 0x66U    /**< @brief RAM content is valid. */

} Mcu_RamStateType;
#endif /* (MCU_GET_RAM_STATE_API == STD_ON) */

#ifdef MCU_PREPARE_MEMORY_CONFIG
/**
* @brief            The stage of the flash and ram controllers configuration.
* @details          This is used to specify the entry and exit point of the flash and ram controllers configuration.
*/
typedef enum
{
    MCU_RAM_MEMORY_CONFIG_ENTRY_POINT      = 0x77U,
    MCU_RAM_MEMORY_CONFIG_EXIT_POINT       = 0x88U,
    MCU_FLASH_MEMORY_CONFIG_ENTRY_POINT    = 0x89U,
    MCU_FLASH_MEMORY_CONFIG_EXIT_POINT     = 0x90U

} Mcu_MemoryConfigStageType;
#endif

#if (defined(MCU_PMC_NOTIFICATION) && (MCU_ENTER_LOW_POWER_MODE == STD_ON))
/**
* @brief            Power management controller events.
* @details          The various events triggered by the power management controller.
*/
typedef enum
{
    MCU_LAST_MILE_REGULATOR_DISABLED = 0x55U

} Mcu_PowerManagementEventType;
#endif

#ifdef MCU_SLEEPONEXIT_SUPPORT
  #if (MCU_SLEEPONEXIT_SUPPORT == STD_ON)
typedef enum
{
    MCU_SLEEP_ON_EXIT_DISABLED = 0U,   /**< @brief Disable SLEEPONEXIT bit (default). */
    MCU_SLEEP_ON_EXIT_ENABLED   /**< @brief Enable SLEEPONEXIT bit. */

} Mcu_SleepOnExitType;
  #endif
#endif

#ifdef MCU_SRAM_RETEN_CONFIG_API
#if (MCU_SRAM_RETEN_CONFIG_API == STD_ON)
/**
* @brief            Type of parameter value of the function Mcu_SRAMRetentionConfig.
* @details          The type of Mcu_SRAMRetenConfigType is an enumeration with the following values:
*                       MCU_SRAML_RETEN, MCU_SRAMU_RETEN, MCU_SRAMLU_RETEN, MCU_NO_SRAMLU_RETEN.
*/
typedef Power_Ip_SRAMRetenConfigType Mcu_SRAMRetenConfigType;
#endif
#endif
/*==================================================================================================
                                           DEFINES AND MACROS
==================================================================================================*/
/**
* @brief            This macro is used to define the position of the first reset reason.
*
*/
#define MCU_FIRST_RESET_REASON_POS_U32      ((uint32)MCU_POWER_ON_RESET)

/*==================================================================================================
*                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

#if (MCU_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF)
#define MCU_START_SEC_VAR_CLEARED_UNSPECIFIED

#include "Mcu_MemMap.h"

/**
* @brief            Local copy of the pointer to the configuration data
*/
extern const Mcu_DemConfigType * Mcu_pDemCfgPtr;

#define MCU_STOP_SEC_VAR_CLEARED_UNSPECIFIED

#include "Mcu_MemMap.h"


#define MCU_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "Mcu_MemMap.h"

extern const Mcu_DemConfigType Mcu_DemConfig;

#define MCU_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "Mcu_MemMap.h"
#endif /* (MCU_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF) */


/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
#define MCU_START_SEC_CODE

#include "Mcu_MemMap.h"

[!IF "node:exists(McuGeneralConfiguration/McuCmuNotification)"!]
/**
* @brief            The callout configured by the user for CMU notifications.
*/
extern void MCU_CMU_FCCU_NOTIFICATION(Clock_Ip_NameType ClockName);
[!ENDIF!]

[!IF "node:exists(McuGeneralConfiguration/McuCguNotification)"!]
/**
* @brief            The callout configured by the user for CGU notifications.
*/
extern void MCU_CGU_NOTIFICATION(Clock_Ip_NameType ClockName, uint8 StatusDetect);
[!ENDIF!]


[!IF "node:exists(McuGeneralConfiguration/McuErrorIsrNotification)"!]
/**
* @brief            The callout configured by the user for error ISR notifications.
*/
extern void MCU_ERROR_ISR_NOTIFICATION(uint8 u8ErrorCode);
[!ENDIF!]

[!IF "node:exists(McuGeneralConfiguration/McuPmcNotification)"!]
/**
* @brief            The callout configured by the user for PMC notifications.
*/
extern void MCU_PMC_NOTIFICATION(Mcu_PowerManagementEventType ePowerEvent);
[!ENDIF!]

[!IF "node:exists(McuGeneralConfiguration/McuVeryLowPowerStopAbortNotification)"!]
/**
* @brief            The low power abort notification procedure configured by the user.
*/
extern void MCU_VLPSA_NOTIFICATION(void);
[!ENDIF!]

[!IF "node:exists(McuGeneralConfiguration/McuPrepareMemoryConfig)"!]
/**
* @brief            The callout configured by the user for Memory notifications.
*/
extern void MCU_PREPARE_MEMORY_CONFIG(Mcu_MemoryConfigStageType ConfigStage);
[!ENDIF!]

[!IF "McuGeneralConfiguration/McuPerformResetApi = 'true'"!][!IF "McuGeneralConfiguration/McuCalloutBeforePerformReset = 'true'"!]
#if ( (MCU_PERFORM_RESET_API == STD_ON) && (MCU_RESET_CALLOUT_USED == STD_ON) )
/**
* @brief            The callout reset procedure configured by the user.
*/
extern void MCU_RESET_CALLOUT(void);
#endif /* ( (MCU_PERFORM_RESET_API == STD_ON) && (MCU_RESET_CALLOUT_USED == STD_ON) ) */
[!ENDIF!][!ENDIF!]

#define MCU_STOP_SEC_CODE

#include "Mcu_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
#endif /* #ifndef MCU_CFG_H */

