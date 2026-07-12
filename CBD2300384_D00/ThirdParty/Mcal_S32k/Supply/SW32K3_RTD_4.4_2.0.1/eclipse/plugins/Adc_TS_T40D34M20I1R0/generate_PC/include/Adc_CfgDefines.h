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

#ifndef ADC_CFGDEFINES_H
#define ADC_CFGDEFINES_H

/**
*   @file
*
*   @addtogroup adc_driver_config Adc Driver Configuration
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
[!NOCODE!][!//
[!INCLUDE "Adc_VersionCheck_Inc.m"!][!//
[!INCLUDE "Adc_RegOperations.m"!][!//
[!ENDNOCODE!][!//

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ADC_VENDOR_ID_CFGDEFINES                      43
#define ADC_AR_RELEASE_MAJOR_VERSION_CFGDEFINES       4
#define ADC_AR_RELEASE_MINOR_VERSION_CFGDEFINES       4
#define ADC_AR_RELEASE_REVISION_VERSION_CFGDEFINES    0
#define ADC_SW_MAJOR_VERSION_CFGDEFINES               2
#define ADC_SW_MINOR_VERSION_CFGDEFINES               0
#define ADC_SW_PATCH_VERSION_CFGDEFINES               1
/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/**
* @brief          Enables or disables Multicore feature in the ADC driver ((STD_ON)/(STD_OFF)).
*/
#define ADC_MULTICORE_SUPPORT               [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

[!IF "(IMPLEMENTATION_CONFIG_VARIANT !='VariantPostBuild' and (variant:size()<=1))"!][!//
[!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
/**
* @brief          Maximum number of configured partitions.
*/
#define ADC_MAX_PARTITIONS_CFG              ([!"num:i(count(AdcGeneral/AdcEcucPartitionRef/*))"!]U)
[!ELSE!][!//
/**
* @brief          Maximum number of configured partitions.
*/
#define ADC_MAX_PARTITIONS_CFG              (1U)

[!ENDIF!][!//
[!ENDIF!][!//
[!NOCODE!][!//
[!VAR "maxPartition" = "num:i(0)"!]
[!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
[!IF "node:exists(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition)"!]
    [!VAR "maxPartition" = "num:i(count(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition/*))"!]
[!ENDIF!]
[!ENDIF!]
[!IF "$maxPartition = 0"!][!VAR "maxPartition" = "num:i(1)"!][!ENDIF!]
[!ENDNOCODE!][!//
/**
* @brief          Number of configured partitions.
*/
#define ADC_MAX_PARTITIONS                  ([!"num:i($maxPartition)"!]U)

/**
* @brief          Averaging functionality is supported
*/
#define ADC_AVERAGING_FUNCTIONALITY_SUPPORT

/**
* @brief           Adds/removes ((STD_ON)/(STD_OFF)) the services Adc_EnableHardwareTrigger() and Adc_DisableHardwareTrigger() from the code.
*/
#define ADC_HW_TRIGGER_API                  [!IF "AdcGeneral/AdcHwTriggerApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief           AdcSetHwUnitPowerMode api enabled/disabled ((STD_ON)/(STD_OFF)).
*/
#define ADC_SET_HW_UNIT_POWER_MODE_API                    [!IF "AutosarExt/AdcSetHwUnitPowerModeApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief           Enables or disables limit checking feature in the ADC driver.
*/
#define ADC_ENABLE_LIMIT_CHECK              [!IF "AdcGeneral/AdcEnableLimitCheck"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief           Group notification mechanism is/is not ((STD_ON)/(STD_OFF)) available at runtime.
*/
#define ADC_GRP_NOTIF_CAPABILITY            [!IF "AdcGeneral/AdcGrpNotifCapability"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief           Type of prioritization mechanism ADC_PRIORITY_HW, ADC_PRIORITY_HW_SW and ADC_PRIORITY_NONE.
*/
/* SWS_Adc_00522 */
#define ADC_PRIORITY_HW                     (0U)
#define ADC_PRIORITY_HW_SW                  (1U)
#define ADC_PRIORITY_NONE                   (2U)

/**
* @brief           Priority implementation.
*/
#define ADC_PRIORITY_IMPLEMENTATION         ([!"AdcGeneral/AdcPriorityImplementation"!])

/**
* @brief           Type of alignment mechanism ADC_ALIGN_RIGHT, ADC_ALIGN_LEFT.
*/
/* SWS_Adc_00525 */
#define ADC_ALIGN_RIGHT                     (0U)
#define ADC_ALIGN_LEFT                      (1U)

/**
* @brief           Result Alignment.
*/
#define ADC_RESULT_ALIGNMENT                ([!"AdcGeneral/AdcResultAlignment"!])

/**
* @brief         This is a timeout value which is used to wait until
*                  - the conversion is not aborted.
*                  - ADC hardware is not entered in power down state.
*                  - ADC hardware is not entered in idle state.
*/
#define ADC_TIMEOUT_COUNTER                 ([!"num:i(AutosarExt/AdcTimeoutVal)"!]UL)

/**
* @brief           Specifies for reducing the number of interrupts for streaming groups
*/
#define ADC_OPTIMIZE_DMA_STREAMING_GROUPS         [!IF "AutosarExt/AdcOptimizeDmaStreamingGroups"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief           Specifies to optimize conversions for hw triggered, one shot group.
*/
#define ADC_OPTIMIZE_ONESHOT_HW_TRIGGER     [!IF "AutosarExt/AdcOptimizeOneShotHwTriggerConversions"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief           Specifies to configure BCTU/CTU list at initialization for ctu hardware trigger mode groups.
*/
#define ADC_CTU_HW_TRIGGER_OPTIMIZATION     [!IF "AutosarExt/AdcCtuHardwareTriggerOptimization"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!][!//

[!NOCODE!][!//
[!VAR "DMA_ENABLED"="0"!][!//
[!IF "AutosarExt/AdcEnableDmaTransferMode  = 'true'"!][!//
[!VAR "DMA_ENABLED"="1"!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!IF "$DMA_ENABLED = 1"!]
/**
* @brief           Active DMA.
*/
#define ADC_DMA_SUPPORTED
[!ENDIF!][!//
[!CALL "CheckUniqueDmaChannel"!][!//

/**
* @brief           Active DMA transfer notification.
*/
[!FOR "x" = "0" TO "ecu:get('Adc.AdcConfigSet.AdcHwUnit')-1"!][!//
[!IF "AdcConfigSet/AdcHwUnit/*[AdcHwUnitId = concat('ADC',string($x))]/AdcTransferType = 'ADC_DMA'"!][!//
#define ADC_UNIT_[!"$x"!]_DMA_TRANSFER_USED
[!ENDIF!][!//
[!ENDFOR!][!//

/**
 * @brief   Define the number of maximum hardware unit configured
 */
#define ADC_MAX_HW_UNITS_CFG                ADC_IPW_MAX_HW_UNITS_CFG

/**
* @brief           Adc transfer type.
*/
#ifdef ADC_DMA_SUPPORTED
#define ADC_DMA                             (0U)
#endif /* ADC_DMA_SUPPORTED */
#define ADC_INTERRUPT                       (1U)

/**
* @brief           Add/Remove Autosar Extension Adc_EnableChannel() and Adc_DisableChannel() api.
*/
#define ADC_ENABLE_CH_DISABLE_CH_NONAUTO_API      [!IF "AutosarExt/AdcEnableChDisableChApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief           Get injected conversion status api enabled/disabled (STD_ON/STD_OFF).
*/
#define ADC_GET_INJECTED_CONVERSION_STATUS_API    [!IF "AutosarExt/AdcGetInjectedConvStatusApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief          Add/Remove Autosar Extension Adc_SetClockMode().
*/
#define ADC_DUAL_CLOCK_MODE                       [!IF "AutosarExt/AdcEnableDualClockMode"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief          Add/remove ((STD_ON)/(STD_OFF)) the service Adc_Calibration() from the code.
*/
#define ADC_CALIBRATION                           [!IF "AutosarExt/AdcEnableCalibration"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief          Add/remove ((STD_ON)/(STD_OFF)) the service Adc_SetChannel() from the code.
*/
#define ADC_SETCHANNEL_API                        [!IF "AutosarExt/AdcEnableSetChannel"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief          Extra group notification mechanism is/is not ((STD_ON)/(STD_OFF)) available at runtime.
*/
#define ADC_ENABLE_INITIAL_NOTIFICATION           [!IF "AutosarExt/AdcEnableInitialNotification"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief          Add/remove ((STD_ON)/(STD_OFF)) the service Adc_SelfTest() from the code.
*/
#define ADC_SELF_TEST                             [!IF "AutosarExt/AdcEnableSelfTest"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief           ADC Configure Threshold enabled/disabled (STD_ON/STD_OFF).
*/
#define ADC_ENABLE_CONFIGURE_THRESHOLD_NONAUTO_API    [!IF "AutosarExt/AdcEnableThresholdConfigurationApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief           Switch to enable/disable the API for the CPR_RTD_00056, CPR_RTD_00057, CPR_RTD_00058, CPR_RTD_00059 and CPR_RTD_00060.
*/
#define ADC_CTU_CONTROL_MODE_EXTRA_APIS           [!IF "AutosarExt/AdcCtuControlModeExtraApis ='true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief           This switch is used to enable the hardware trigger Autosar Extension API for the CPR_RTD_00037, CPR_RTD_00038, CPR_RTD_00039, CPR_RTD_00040, CPR_RTD_00041.
*/
#define ADC_ENABLE_CTUTRIG_NONAUTO_API            [!IF "AutosarExt/AdcEnableCtuTrigAutosarExtApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief           Switch to enable/disable the API for the CPR-MCAL-726, CPR-MCAL-727, CPR-MCAL-728.
*/
#define ADC_ENABLE_CTU_CONTROL_MODE_API           [!IF "AutosarExt/AdcEnableCtuControlModeApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief           Switch to enable/disable the API for Tempsense.
*/
#define ADC_ENABLE_TEMPSENSE_API                  [!IF "AutosarExt/AdcSarEnableTempsenseApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
[!IF "AutosarExt/AdcSarEnableTempsenseApi"!][!//
#define ADC_POWER_ON_TEMPSENSE                    [!IF "AutosarExt/AdcSarPowerOnTempSense"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
[!ELSE!][!//
#define ADC_POWER_ON_TEMPSENSE                    (STD_OFF)
[!ENDIF!][!//

/**
* @brief           Enable the read of raw data API (CPR_RTD_00508).
*/
[!IF "AutosarExt/AdcEnableCtuControlModeApi"!][!//
#define ADC_ENABLE_READ_RAW_DATA_API              [!IF "AutosarExt/AdcEnableReadRawDataApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
[!ELSE!][!//
#define ADC_ENABLE_READ_RAW_DATA_API              (STD_OFF)
[!ENDIF!][!//

/**
* @brief           Enable Group Streaming Result as multiple set.
*/
#define ADC_ENABLE_GROUP_STREAMING_RESULTS_REORDER [!IF "AutosarExt/AdcEnableGroupStreamingResultReorder"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

[!NOCODE!][!//
/**
* @brief           Lookup for WDG functionality activation.
*/
[!VAR "WDG_FUNCTIONALITY_FOUND" = "0"!]
[!IF "AutosarExt/AdcEnableWatchdogApi  = 'true'"!]
  [!VAR "WDG_FUNCTIONALITY_FOUND" = "1"!]
[!ENDIF!]
[!ENDNOCODE!][!//
[!IF "$WDG_FUNCTIONALITY_FOUND = 1"!][!//
/**
* @brief           WDG functionality activated.
*/
#define ADC_WDG_SUPPORTED                         ADC_SAR_IP_WDG_ENABLED

[!ENDIF!][!//
/**
* @brief IRQ definition
*/
[!FOR "x" = "0" TO "ecu:get('Adc.AdcConfigSet.AdcHwUnit')-1"!][!//
[!IF "AdcInterrupt/*[AdcInterruptSource=concat(concat('ADC',string($x)),'_EOC')]/AdcInterruptEnable='true'"!][!//
#define ADC_UNIT_[!"$x"!]_ISR_USED
[!ENDIF!][!//
[!ENDFOR!][!//

[!NOCODE!][!//
[!IF "$WDG_FUNCTIONALITY_FOUND = 1"!][!//
  [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
    [!VAR "AdcPhysicalId" = "substring-after(AdcHwUnitId,'ADC')"!][!//
    [!LOOP "AdcChannel/*[AdcEnableThresholds = 'true']"!][!//
[!CODE!][!//
#define ADC_UNIT_[!"$AdcPhysicalId"!]_WDG_ISR_USED
[!ENDCODE!][!//
      [!BREAK!][!//
    [!ENDLOOP!][!//
  [!ENDLOOP!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//

[!NOCODE!][!//
[!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
  [!VAR "AdcPhysicalId" = "substring-after(AdcHwUnitId,'ADC')"!][!//
  [!IF "count(AdcGroup/*[(AdcGroupTriggSrc='ADC_TRIGG_SRC_HW') and (AdcWithoutInterrupts = 'false') and not(contains((node:ref(AdcGroupHwTriggerSource)/AdcHwTrigSrc), 'EXT_TRIG'))]) > 0"!][!//
  [!// The current ADC Hw unit has at least 1 Group is HW triggered which trigger source comes from BCTU
    [!IF "node:value(./AdcTransferType)='ADC_INTERRUPT'"!][!//
[!CODE!][!//
#define ADC_UNIT_[!"$AdcPhysicalId"!]_BCTU_LIST_ISR_USED
[!ENDCODE!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDNOCODE!][!//

/**
* @brief End Of Chain Notification
*/
[!NOCODE!][!//
[!LOOP "AdcConfigSet/AdcHwUnit/*"!]
  [!VAR "AdcPhysicalId" = "substring-after(AdcHwUnitId,'ADC')"!][!//
  [!IF "AdcTransferType = 'ADC_INTERRUPT'"!]
    [!LOOP "AdcGroup/*[AdcWithoutInterrupts = 'false']"!]
      [!IF "AdcGroupConversionType = 'ADC_CONV_TYPE_NORMAL'"!]
[!CODE!][!//
#define ADC_UNIT_[!"$AdcPhysicalId"!]_NORMAL_ISR_USED
[!ENDCODE!][!//
        [!BREAK!]
      [!ENDIF!]
    [!ENDLOOP!]

    [!LOOP "AdcGroup/*[AdcWithoutInterrupts = 'false']"!]
      [!IF "AdcGroupConversionType = 'ADC_CONV_TYPE_INJECTED'"!]
[!CODE!][!//
#define ADC_UNIT_[!"$AdcPhysicalId"!]_INJECTED_ISR_USED
[!ENDCODE!][!//
        [!BREAK!]
      [!ENDIF!]
    [!ENDLOOP!]
  [!ENDIF!][!// End of checking TransferType is ADC_INTERRUPT
[!ENDLOOP!][!// End of loop all HW Units configured
[!ENDNOCODE!][!//

[!NOCODE!][!//
[!VAR "maxGrp" = "num:i(count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*))"!]
[!VAR "tempVal" = "0"!][!//
[!VAR "maxGrpPerUnit" = "0"!][!//
[!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
 [!VAR "tempVal" = "num:i(count(AdcGroup/*))"!]
  [!IF "num:i($tempVal) > num:i($maxGrpPerUnit)"!][!VAR "maxGrpPerUnit" = "$tempVal"!][!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDNOCODE!][!//
/**
* @brief           max queue depth configured across all configset.
*/
#define ADC_QUEUE_MAX_DEPTH_MAX             ([!"num:i(AdcGeneral/AdcPriorityQueueMaxDepth )"!]U)

/**
* @brief           max number of groups configured across all configset.
*/
#define ADC_MAX_GROUPS                      ([!"num:i($maxGrp)"!]U)

/**
* @brief           Invalid Hardware group ID to determine there is no ongoing hardware group
*/
#define ADC_INVALID_HW_GROUP_ID             (0xFFFFU)

[!VAR "maxVal" = "1"!][!//
[!VAR "tempVal" = "0"!][!//
[!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
[!VAR "tempVal" = "num:i(count(AdcChannel/*))"!][!//
[!IF "num:i($tempVal) > num:i($maxVal)"!][!VAR "maxVal" = "$tempVal"!][!ENDIF!][!//
[!ENDLOOP!][!//
#if defined(ADC_DMA_SUPPORTED) || (ADC_HW_TRIGGER_API == STD_ON)
/**
* @brief           Maximum number of channels across all hardware units.
*/
#define ADC_MAX_CHANNEL_PER_HW_UNIT         ([!"num:i($maxVal)"!]U)
#endif /* defined(ADC_DMA_SUPPORTED) || ...  */

/**
* @brief           Software injected conversion use
*/
#define ADC_SOFTWARE_INJECTED_CONVERSIONS_USED    [!IF "AutosarExt/AdcUseSoftwareInjectedGroups = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief           Hardware normal conversion use
*/
#define ADC_HARDWARE_NORMAL_CONVERSIONS_USED      [!IF "AutosarExt/AdcUseHardwareNormalGroups = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief          Configuration Precompile variant.
* @details        Configuration Precompile variant.
*/
#define ADC_PRECOMPILE_SUPPORT                    [!IF "(IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1)"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief          Development error detection enabled/disabled ((STD_ON)/(STD_OFF)).
*/
#define ADC_DEV_ERROR_DETECT                      [!IF "AdcGeneral/AdcDevErrorDetect"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief          Add/remove ((STD_ON)/(STD_OFF)) the service Adc_GetVersionInfo() from the code.
*/
#define ADC_VERSION_INFO_API                      [!IF "AdcGeneral/AdcVersionInfoApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief          Add/remove ((STD_ON)/(STD_OFF)) the service Adc_DeInit() from the code.
*/
#define ADC_DEINIT_API                            [!IF "AdcGeneral/AdcDeInitApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief          Add/remove ((STD_ON)/(STD_OFF)) the services Adc_StartGroupConversion()and Adc_StopGroupConversion() from the code.
*/
#define ADC_ENABLE_START_STOP_GROUP_API           [!IF "AdcGeneral/AdcEnableStartStopGroupApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief          Add/Remove the services Adc_ReadGroup() from the code.
*/
#define ADC_READ_GROUP_API                        [!IF "AdcGeneral/AdcReadGroupApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief          Add/Remove One time setting of Conversion time registers from Init() function.
*/
#define ADC_SET_ADC_CONV_TIME_ONCE                [!IF "AutosarExt/AdcConvTimeOnce"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief   Switch to enable that power state mode is supported
*
*/
#define ADC_POWER_STATE_SUPPORTED                 [!IF "node:exists(AdcGeneral/AdcLowPowerStatesSupport) and (AdcGeneral/AdcLowPowerStatesSupport = 'true')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

#if (ADC_POWER_STATE_SUPPORTED == STD_ON)
/**
* @brief   Enable/Disable the asynchronous power mode
*
*/
#define ADC_POWER_STATE_ASYNCH_MODE_SUPPORTED     [!IF "node:exists(AdcGeneral/AdcPowerStateAsynchTransitionMode) and (AdcGeneral/AdcPowerStateAsynchTransitionMode = 'true')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#endif /* (ADC_POWER_STATE_SUPPORTED == STD_ON) */

/**
* @brief           This switch is used to enable the queue.
*/
[!IF "AdcGeneral/AdcPriorityImplementation = 'ADC_PRIORITY_NONE'"!][!//
#define ADC_ENABLE_QUEUING                        [!IF "AdcGeneral/AdcEnableQueuing = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
[!ELSE!][!//
#define ADC_ENABLE_QUEUING                        (STD_ON)
[!ENDIF!][!//

/**
* @brief           This switch is used to enable the Autosar Extension ADC performance optimization feature.
*/
#define ADC_BYPASS_ABORT_CHAIN_CHECK              [!IF "AutosarExt/AdcBypassAbortChainCheck"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

[!NOCODE!][!//
[!IF "(AutosarExt/AdcEnableChDisableChApi = 'true')"!][!//
[!VAR "AdcGroupCounter" = "0"!][!//
[!LOOP "AdcConfigSet/AdcHwUnit/*/AdcGroup/*"!][!//
  [!IF "(AdcEnableChDisableChGroup = 'true')"!][!//
    [!VAR "AdcGroupCounter" = "$AdcGroupCounter + 1"!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//
[!IF "$AdcGroupCounter = 0"!][!// Adc_axRuntimeChansEnMask array must have at least 1 element to avoid failing at build, resize to 1 if AdcEnableChDisableChGroup is enabled but no group uses it
[!VAR "AdcGroupCounter" = "1"!][!//
[!ENDIF!][!//
[!CODE!][!//

/**
* @brief           Max number of groups supports the individual group channel enable or disable support.
*/
#define  ADC_NO_OF_ENABLE_CH_DISABLE_CH_GROUPS    [!"num:i($AdcGroupCounter)"!]
[!ENDCODE!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//

/**
* @brief           Symbolic names of ADC Hardware units.
* @details         Values generated are the ADC Logical Unit ID selected from configurator.
*                  These defines are recommended to be used with any ADC driver API that takes as input parameter Adc Unit.
*/
[!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
#define [!"name(.)"!]                               ([!"AdcLogicalUnitId"!]U)
[!ENDLOOP!][!//

/**
* @brief          Adc channel id.
*/
#define ADC_ID_CHANNEL_U8(Id)                     (Id)

/**
* @brief          macros to simplify access to structures.
* @details        Streaming number of samples
*/
#define ADC_STREAMING_NUM_SAMPLES(num)            (num)

/**
* @brief          Adc group priority.
*/
#define ADC_GROUP_PRIORITY(Priority)              (Priority)

/**
* @brief           AUTOSAR Symbolic names of channels on all HW units with encoded value.
* @details         Bit fields [12-15]:  HW unit physical id
*                  Bit fields [0-11]:   Logical id of channel in HW unit (used in Adc_EnableChannel()/Adc_DisableChannel()/Adc_EnableWdgNotification()/Adc_DisableWdgNotification())
*/
[!NOCODE!][!//
[!FOR "logicUnit" = "0" TO "count(AdcConfigSet/AdcHwUnit/*)-1"!][!//
    [!LOOP "AdcConfigSet/AdcHwUnit/*"!][!//
        [!IF "$logicUnit = AdcLogicalUnitId"!][!//
            [!FOR "logicCh" = "0" TO "count(AdcChannel/*)-1"!][!//
                [!LOOP "AdcChannel/*"!][!//
                    [!IF "$logicCh = AdcLogicalChannelId"!][!//
                        [!VAR "AdcPhysicalId" = "num:i(substring-after(../../AdcHwUnitId,'ADC'))"!][!//
[!CODE!][!//
#define [!"name(.)"!]                       ([!"num:inttohex(num:i(AdcLogicalChannelId) + bit:shl($AdcPhysicalId,12))"!]U)
[!ENDCODE!][!//
                        [!BREAK!]
                    [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ENDFOR!][!//
            [!BREAK!]
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDFOR!][!//
[!ENDNOCODE!][!//

/**
* @brief           Symbolic names of threshold control register with encoded value.
* @details         Bit fields [12-15]:  The ADC Logical Unit Id
*                  Bit fields [0-11]:   Logical id of threshold control in HW unit (used in Adc_ConfigureThreshold())
*/
[!CALL "AdcWdgThresholdDefineNamesMacro"!][!//

/**
* @brief          Definitions used for extracting the ADC Logical Unit Id and logical id of channels in each unit from equivalent channel symbolic name
*/
#if (defined(ADC_WDG_SUPPORTED) || (ADC_ENABLE_CH_DISABLE_CH_NONAUTO_API == STD_ON) || (ADC_ENABLE_CONFIGURE_THRESHOLD_NONAUTO_API == STD_ON))
#define ADC_CHANNEL_SYMBOLIC_NAME_MASK_CHANNEL_ID_U16           (0xFFFU)
#endif /* (defined(ADC_WDG_SUPPORTED) || (ADC_ENABLE_CH_DISABLE_CH_NONAUTO_API == STD_ON) || (ADC_ENABLE_CONFIGURE_THRESHOLD_NONAUTO_API == STD_ON)) */
#if (defined(ADC_WDG_SUPPORTED) || (ADC_ENABLE_CONFIGURE_THRESHOLD_NONAUTO_API == STD_ON))
#define ADC_CHANNEL_SYMBOLIC_NAME_SHIFT_HW_UNIT_ID_U16          (12U)
#endif /* (defined(ADC_WDG_SUPPORTED) || (ADC_ENABLE_CONFIGURE_THRESHOLD_NONAUTO_API == STD_ON)) */

/**
* @brief          Definitions used for extracting the ADC Logical Unit Id and threshold controls in each unit from equivalent threshold control symbolic name
*/
#if (ADC_ENABLE_CONFIGURE_THRESHOLD_NONAUTO_API == STD_ON)
#define ADC_THRESHOLD_SYMBOLIC_NAME_MASK_THRESHOLD_ID_U16       ADC_CHANNEL_SYMBOLIC_NAME_MASK_CHANNEL_ID_U16
#define ADC_THRESHOLD_SYMBOLIC_NAME_SHIFT_HW_UNIT_ID_U16        ADC_CHANNEL_SYMBOLIC_NAME_SHIFT_HW_UNIT_ID_U16
#endif /* (ADC_ENABLE_CONFIGURE_THRESHOLD_NONAUTO_API == STD_ON) */

[!IF "AutosarExt/AdcEnableGroupDependentChannelNames = 'true'"!][!//
[!VAR "already_parsed_names"!][!ENDVAR!][!//
/**
* @brief           Autosar Extension symbolic names of channels per groups, on all HW units, with value set to channel index in the group.
*/
[!FOR "x" = "0" TO "count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*)-1"!][!//
[!LOOP "AdcConfigSet/AdcHwUnit/*/AdcGroup/*"!][!//
[!IF "$x = AdcGroupId"!][!//
[!VAR "Groupname" = "name(.)"!][!//
[!VAR "index" = "0"!][!//
[!LOOP "AdcGroupDefinition/*"!][!//
[!VAR "CurrAdcChannel" = "."!][!//
[!VAR "CurrAdcChannel1" = "substring-after(substring-after(substring-after(substring-after(substring-after($CurrAdcChannel,'/'),'/'),'/'),'/'),'/')"!][!//
[!LOOP "../../../../AdcChannel/*"!][!//
[!IF "@name = $CurrAdcChannel1"!][!//
[!IF "contains($already_parsed_names, concat($Groupname, '_', node:name(.), '|')) = false()"!][!//
#define [!"$Groupname"!]_[!"name(.)"!]                   ([!"num:i($index)"!]U)
[!VAR "already_parsed_names"="concat($already_parsed_names, $Groupname, '_', node:name(.), '|')"!][!//
[!VAR "index" = "$index + 1"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDLOOP!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDFOR!][!//
[!ENDIF!][!//

/**
* @brief           Symbolic names of groups.
*/
[!FOR "x" = "0" TO "count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*)-1"!][!//
[!LOOP "AdcConfigSet/AdcHwUnit/*/AdcGroup/*"!][!//
[!IF "$x = AdcGroupId"!]
#define [!"name(.)"!]                                ([!"AdcGroupId"!]U)[!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDFOR!]

/**
* @brief           Symbolic names of groups - ecuc 2108 compliant.
*/
[!FOR "x" = "0" TO "count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*)-1"!][!//
[!LOOP "AdcConfigSet/AdcHwUnit/*/AdcGroup/*"!][!//
[!IF "$x = AdcGroupId"!]
#define AdcConf_AdcGroup_[!"name(.)"!]               ([!"AdcGroupId"!]U)[!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDFOR!]

/**
* @brief           Adc Autosar Extension Enable/Disable invalid group index.
*/
#if (ADC_ENABLE_CH_DISABLE_CH_NONAUTO_API == STD_ON)
#define ADC_ENABLE_CH_DISABLE_CH_INVALID_GROUP_INDEX   ((Adc_GroupType)0xFFFF)
#endif /* ADC_ENABLE_CH_DISABLE_CH_NONAUTO_API == STD_ON */
[!// Generate Hardware Trigger Sources Configured.
[!CALL "AdcHwTriggerSourceDefinesMacro"!][!//

/**
* @brief          ADC Global Validation.
*/
#define ADC_VALIDATE_GLOBAL_CALL         (ADC_DEV_ERROR_DETECT)
#define ADC_VALIDATE_CALL_AND_GROUP      (ADC_DEV_ERROR_DETECT)
#define ADC_VALIDATE_CALL_AND_UNIT       (ADC_DEV_ERROR_DETECT)
#define ADC_VALIDATE_PARAMS              (ADC_DEV_ERROR_DETECT)
#define ADC_VALIDATE_NOTIFY_CAPABILITY   (ADC_DEV_ERROR_DETECT)

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/


/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/


/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/


/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* ADC_CFGDEFINES_H */
