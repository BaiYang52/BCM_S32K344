[!AUTOSPACING!][!//
[!CODE!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Emios Siul2 Wkpu LpCmp
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

#ifndef ICU_CFG_H
#define ICU_CFG_H

/**
*   @file Icu_Cfg.h
*   @version 2.0.1
*
*   @brief   AUTOSAR Icu - contains the configuration data of the ICU driver
*   @details Contains the configuration data of the ICU driver
*
*   @addtogroup icu Icu Driver
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
[!NOCODE!][!//
[!INCLUDE "Icu_VersionCheck_Inc.m"!][!//
[!ENDNOCODE!][!//
#include "Icu_EnvCfg.h"
#include "StandardTypes.h"
[!IF "var:defined('postBuildVariant')"!][!//
    [!LOOP "as:modconf('EcuC')[1]/EcucPostBuildVariants/EcucPostBuildVariantRef/*"!][!//
#include "Icu_[!"substring-after(substring-after(substring-after(node:value(.),'/'),'/'),'/')"!]_PBcfg.h"
    [!ENDLOOP!][!//
[!ELSE!][!//
#include "Icu_PBcfg.h"
[!ENDIF!][!//

[!INCLUDE "Icu_PluginMacros.m"!][!//

/*==================================================================================================
                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ICU_CFG_VENDOR_ID                       43
#define ICU_CFG_AR_RELEASE_MAJOR_VERSION        4
#define ICU_CFG_AR_RELEASE_MINOR_VERSION        4
#define ICU_CFG_AR_RELEASE_REVISION_VERSION     0
#define ICU_CFG_SW_MAJOR_VERSION                2
#define ICU_CFG_SW_MINOR_VERSION                0
#define ICU_CFG_SW_PATCH_VERSION                1

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
/* Check if source file and Icu_EnvCfg header file are of the same vendor */
#if (ICU_CFG_VENDOR_ID != ICU_ENVCFG_VENDOR_ID)
    #error "Icu_Cfg.h and Icu_EnvCfg.h have different vendor IDs"
#endif
/* Check if source file and Icu_EnvCfg header file are of the same AutoSar version */
#if ((ICU_CFG_AR_RELEASE_MAJOR_VERSION != ICU_ENVCFG_AR_RELEASE_MAJOR_VERSION) || \
     (ICU_CFG_AR_RELEASE_MINOR_VERSION != ICU_ENVCFG_AR_RELEASE_MINOR_VERSION) || \
     (ICU_CFG_AR_RELEASE_REVISION_VERSION != ICU_ENVCFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Icu_Cfg.h and Icu_EnvCfg.h are different"
#endif
/* Check if source file and Icu_EnvCfg header file are of the same Software version */
#if ((ICU_CFG_SW_MAJOR_VERSION != ICU_ENVCFG_SW_MAJOR_VERSION) || \
     (ICU_CFG_SW_MINOR_VERSION != ICU_ENVCFG_SW_MINOR_VERSION) || \
     (ICU_CFG_SW_PATCH_VERSION != ICU_ENVCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Icu_Cfg.h and Icu_EnvCfg.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((ICU_CFG_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
         (ICU_CFG_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Icu_Cfg.h and StandardTypes.h are different"
    #endif
#endif

[!IF "var:defined('postBuildVariant')"!][!//
    [!LOOP "variant:all()"!][!//
/* Check if source file and ICU configuration header file are of the same vendor */
#if (ICU_[!"."!]_PBCFG_VENDOR_ID != ICU_CFG_VENDOR_ID)
    #error "Icu_[!"."!]_PBcfg.h and Icu_Cfg.h have different vendor IDs"
#endif
/* Check if header file and Icu configuration header file are of the same Autosar version */
#if ((ICU_[!"."!]_PBCFG_AR_RELEASE_MAJOR_VERSION != ICU_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (ICU_[!"."!]_PBCFG_AR_RELEASE_MINOR_VERSION != ICU_CFG_AR_RELEASE_MINOR_VERSION) || \
     (ICU_[!"."!]_PBCFG_AR_RELEASE_REVISION_VERSION != ICU_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Icu_[!"."!]_PBcfg.h and Icu_Cfg.h are different"
#endif
/* Check if header file and Icu configuration header file are of the same software version */
#if ((ICU_[!"."!]_PBCFG_SW_MAJOR_VERSION != ICU_CFG_SW_MAJOR_VERSION) || \
     (ICU_[!"."!]_PBCFG_SW_MINOR_VERSION != ICU_CFG_SW_MINOR_VERSION) || \
     (ICU_[!"."!]_PBCFG_SW_PATCH_VERSION != ICU_CFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Icu_[!"."!]_PBcfg.h and Icu_Cfg.h are different"
#endif
    [!ENDLOOP!][!//
[!ELSE!][!//
/* Check if source file and Icu configuration header file are of the same vendor */
#if (ICU_PBCFG_VENDOR_ID != ICU_CFG_VENDOR_ID)
    #error "Icu_PBcfg.h and Icu_Cfg.h have different vendor IDs"
#endif
    /* Check if header file and Icu configuration header file are of the same Autosar version */
#if ((ICU_PBCFG_AR_RELEASE_MAJOR_VERSION != ICU_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (ICU_PBCFG_AR_RELEASE_MINOR_VERSION != ICU_CFG_AR_RELEASE_MINOR_VERSION) || \
     (ICU_PBCFG_AR_RELEASE_REVISION_VERSION != ICU_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Icu_PBcfg.h and Icu_Cfg.h are different"
#endif
/* Check if header file and Icu configuration header file are of the same software version */
#if ((ICU_PBCFG_SW_MAJOR_VERSION != ICU_CFG_SW_MAJOR_VERSION) || \
     (ICU_PBCFG_SW_MINOR_VERSION != ICU_CFG_SW_MINOR_VERSION) || \
     (ICU_PBCFG_SW_PATCH_VERSION != ICU_CFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Icu_PBcfg.h and Icu_Cfg.h are different"
#endif
[!ENDIF!][!//

/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/
[!IF "(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPostBuild' or (variant:size()>1))"!][!//
#define ICU_CONFIG_EXT \
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
      ICU_CONFIG_[!"."!]_PB \
[!ENDLOOP!][!//
[!ELSE!][!//
       ICU_CONFIG_PB
[!ENDIF!][!//
[!ENDIF!][!//

[!IF "IMPLEMENTATION_CONFIG_VARIANT='VariantPreCompile' or IMPLEMENTATION_CONFIG_VARIANT='VariantLinkTime' "!][!//
    [!IF "variant:size()<=1"!][!//
/** @brief Pre-compile configuration constants. */
#define ICU_PRECOMPILE_SUPPORT
    [!ENDIF!][!//
[!ENDIF!][!//

/** @brief Maximum number of ICU channels configured. */
[!NOCODE!][!//
    [!VAR "MaxNoOfCh" = "0"!][!//
    [!SELECT "IcuConfigSet"!][!//
        [!IF "$MaxNoOfCh < IcuMaxChannel"!][!//
            [!VAR "MaxNoOfCh" = "IcuMaxChannel"!][!//
        [!ENDIF!][!//
    [!ENDSELECT!][!//
[!ENDNOCODE!][!//
#define ICU_MAX_CHANNEL     ((Icu_ChannelType)[!"num:i($MaxNoOfCh)"!]U)

/**
*   @brief  Switches the Development Error Detection and Notification on or off.
*        STD_ON: Enabled.
*        STD_OFF: Disabled.
*   @implements ICU_DEV_ERROR_DETECT_define
*/
#define ICU_DEV_ERROR_DETECT            ([!IF "IcuGeneral/IcuDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
*   @brief  Switch for enabling Wakeup source reporting.
*        STD_ON: Report Wakeup source.
*        STD_OFF: Do not report Wakeup source.
*
*/
#define ICU_REPORT_WAKEUP_SOURCE        ([!IF "IcuGeneral/IcuReportWakeupSource"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
* @brief Support for User mode.
*        If this parameter has been configured to 'STD_ON', the ICU driver code can be executed from both supervisor and user mode.
*/
[!IF "(IcuGeneral/IcuEnableUserModeSupport)"!][!//
#define ICU_ENABLE_USER_MODE_SUPPORT   (STD_ON)
/**
 * @brief Support for REG_PROT in SIUL2 IP.
 * If the current platform implements REG_PROT for SIUL2 IP, this parameter will be defined, and will 
 * enable REG_PROT configuration for SIUL2 IP in ICU drvier.
 */
#define ICU_SIUL2_REG_PROT_AVAILABLE
[!ELSE!][!//
#define ICU_ENABLE_USER_MODE_SUPPORT   (STD_OFF)
[!ENDIF!][!//

#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
 #ifdef ICU_ENABLE_USER_MODE_SUPPORT
  #if (STD_ON == ICU_ENABLE_USER_MODE_SUPPORT)
   #error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running Icu in user mode the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined
  #endif /* (STD_ON == ICU_ENABLE_USER_MODE_SUPPORT) */
 #endif /* ifdef ICU_ENABLE_USER_MODE_SUPPORT*/
#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT */

/** @brief Configuration of Optional API's. */
/**
*   @brief  Adds or removes the service Icu_GetVersionInfo() from the code.
*        STD_ON: Icu_GetVersionInfo() can be used.
*        STD_OFF: Icu_GetVersionInfo() can not be used.
*
*/
#define ICU_GET_VERSION_INFO_API        ([!IF "IcuOptionalApis/IcuGetVersionInfoApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
*   @brief  Adds or removes the service Icu_DeInit() from the code.
*        STD_ON: Icu_DeInit() can be used. STD_OFF: Icu_DeInit() can not be used.
*   @implements ICU_DE_INIT_API_define
*/
#define ICU_DE_INIT_API                 ([!IF "IcuOptionalApis/IcuDeInitApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
*   @brief  Adds or removes the service Icu_SetMode() from the code.
*        STD_ON: Icu_SetMode() can be used.
*        STD_OFF: Icu_SetMode() can not be used.
*   @implements ICU_SET_MODE_API_define
*/
#define ICU_SET_MODE_API                ([!IF "IcuOptionalApis/IcuSetModeApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
*   @brief  Adds or removes the service Icu_DisableWakeup() from the code.
*        STD_ON: Icu_DisableWakeup() can be used.
*        STD_OFF: Icu_DisableWakeup() can not be used.
*   @implements ICU_DISABLE_WAKEUP_API_define
*/
#define ICU_DISABLE_WAKEUP_API          ([!IF "IcuOptionalApis/IcuDisableWakeupApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
*   @brief  Adds or removes the service Icu_EnableWakeup() from the code.
*        STD_ON: Icu_EnableWakeup() can be used.
*        STD_OFF: Icu_EnableWakeup() can not be used.
*   @implements ICU_ENABLE_WAKEUP_API_define
*/
#define ICU_ENABLE_WAKEUP_API           ([!IF "IcuOptionalApis/IcuEnableWakeupApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
*   @brief  Adds or removes all services related to the timestamping functionality as listed
*        below from the code: Icu_StartTimestamp(), Icu_StopTimestamp(), Icu_GetTimestampIndex().
*        STD_ON: The services listed above can be used.
*        STD_OFF: The services listed above can not be used.
*   @implements ICU_TIMESTAMP_API_define
*/
#define ICU_TIMESTAMP_API               ([!IF "IcuOptionalApis/IcuTimestampApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
*   @brief  Adds or removes all services related to the edge counting functionality as listed below,
*           from the code: Icu_ResetEdgeCount(), Icu_EnableEdgeCount(), Icu_DisableEdgeCount(),
*           Icu_GetEdgeNumbers().
*        STD_ON: The services listed above can be used.
*        STD_OFF: The services listed above can not be used.
*   @implements ICU_EDGE_COUNT_API_define
*/
#define ICU_EDGE_COUNT_API              ([!IF "IcuOptionalApis/IcuEdgeCountApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
*   @brief  Adds or removes the service Icu_GetTimeElapsed() from the code.
*        STD_ON: Icu_GetTimeElapsed() can be used.
*        STD_OFF: Icu_GetTimeElapsed() can not be used.
*   @implements ICU_GET_TIME_ELAPSED_API_define
*/
#define ICU_GET_TIME_ELAPSED_API        ([!IF "IcuOptionalApis/IcuGetTimeElapsedApi"!][!IF "IcuOptionalApis/IcuSignalMeasurementApi"!]STD_ON[!ELSE!][!ERROR "If IcuSignalMeasurementApi == OFF this switch is shall also be set to OFF"!][!ENDIF!][!ELSE!]STD_OFF[!ENDIF!])

/**
*   @brief  Adds or removes the service Icu_GetDutyCycleValues() from the code.
*        STD_ON: Icu_GetDutyCycleValues() can be used.
*        STD_OFF: Icu_GetDutyCycleValues() can not be used.
*   @implements ICU_GET_DUTY_CYCLE_VALUES_API_define
*/
#define ICU_GET_DUTY_CYCLE_VALUES_API   ([!IF "IcuOptionalApis/IcuGetDutyCycleValuesApi"!][!IF "IcuOptionalApis/IcuSignalMeasurementApi"!]STD_ON[!ELSE!][!ERROR "If IcuSignalMeasurementApi == OFF this switch is shall also be set to OFF"!][!ENDIF!][!ELSE!]STD_OFF[!ENDIF!])

/**
*   @brief  Adds or removes the service Icu_GetInputState() from the code.
*        STD_ON: Icu_GetInputState() can be used.
*        STD_OFF: Icu_GetInputState() can not be used.
*   @implements ICU_GET_INPUT_STATE_API_define
*/
#define ICU_GET_INPUT_STATE_API         ([!IF "IcuOptionalApis/IcuGetInputStateApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
*   @brief  Adds or removes the services Icu_StartSignalMeasurement() and
*          Icu_StopSignalMeasurement() from the code.
*        STD_ON: Icu_StartSignalMeasurement() and Icu_StopSignalMeasurement() can be used.
*        STD_OFF: Icu_StartSignalMeasurement() and Icu_StopSignalMeasurement() can not be used.
*   @implements ICU_SIGNAL_MEASUREMENT_API_define
*/
#define ICU_SIGNAL_MEASUREMENT_API      ([!IF "IcuOptionalApis/IcuSignalMeasurementApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
*   @brief  Adds or removes the service Icu_CheckWakeup() from the code.
*        STD_ON: Icu_CheckWakeup() can be used.
*        STD_OFF: Icu_CheckWakeup() can not be used.
*   @implements ICU_WAKEUP_FUNCTIONALITY_API_define
*/
#define ICU_WAKEUP_FUNCTIONALITY_API      ([!IF "IcuOptionalApis/IcuWakeupFunctionalityApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
*   @brief  Adds or removes the services Icu_EnableEdgeDetection() and Icu_DisableEdgeDetection()
*           from the code.
*        STD_ON: Icu_EnableEdgeDetection() and Icu_DisableEdgeDetection() can be used.
*        STD_OFF: Icu_EnableEdgeDetection() and Icu_DisableEdgeDetection() can not be used.
*
*   @implements  ICU_EDGE_DETECT_API_define
*/
#define ICU_EDGE_DETECT_API      ([!IF "IcuOptionalApis/IcuEdgeDetectApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
*   @brief  Implementation specific parameter
*        Adds or Removes the code related to overflow notification
*        STD_ON: Overflow notification function will be called if overflow occurs
*        STD_OFF: Overflow notification function will not be called though overflow occurs
*
*/
#define ICU_OVERFLOW_NOTIFICATION_API      ([!IF "IcuAutosarExt/IcuOverflowNotificationApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
*   @brief   Adds or removes the service Icu_GetInputLevel() from the code.
*         STD_ON:  Icu_GetInputLevel() can be used.
*         STD_OFF: Icu_GetInputLevel() can not be used.
*
*
*/
#define ICU_GET_INPUT_LEVEL_API             ([!IF "node:exists(IcuAutosarExt/IcuGetInputLevelApi)"!][!IF "IcuAutosarExt/IcuGetInputLevelApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ELSE!]STD_OFF[!ENDIF!])

[!IF "node:exists(IcuAutosarExt/IcuWkpuStandbyWakeupSupport)"!][!//
/**
 * @brief  Switch for enabling Standby wakeup support  on or off.
 *         STD_ON:  Enabled.
 *         STD_OFF: Disabled.
 */
#define ICU_WKPU_STANDBY_WAKEUP_SUPPORT    ([!IF "IcuAutosarExt/IcuWkpuStandbyWakeupSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!ENDIF!][!//

/**
*   @brief   Adds / removes the service Icu_GetPulseWidth() from the code.
*         STD_ON:  Icu_GetPulseWidth() can be used.
*         STD_OFF: Icu_GetPulseWidth() can not be used.
*/
#define ICU_GET_PULSE_WIDTH_API             ([!IF "(ecu:has('Icu.Num_Emios_Hw_Modules')) and (num:i(0) != num:i(ecu:get('Icu.Num_Emios_Hw_Modules')))"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
 * @brief  Adds or removes the support  measurement with DMA
 *         STD_ON:  DMA measurement can be used.
 *         STD_OFF: DMA measurement can not be used.
 */
[!NOCODE!][!//
    [!VAR "TimestampDmaEnable" = "0"!][!//
    [!VAR "MeasurementDmaEnable" = "0"!][!//
    [!SELECT "IcuConfigSet"!][!//
        [!LOOP "./IcuChannel/*"!][!//
        [!IF "IcuDMAChannelEnable = 'true'"!][!//
            [!IF "IcuMeasurementMode='ICU_MODE_TIMESTAMP'"!][!//
                [!VAR "TimestampDmaEnable" = "$TimestampDmaEnable + 1"!][!//
            [!ENDIF!][!//
            
            [!IF "IcuMeasurementMode='ICU_MODE_SIGNAL_MEASUREMENT'"!][!//
                [!VAR "MeasurementDmaEnable" = "$MeasurementDmaEnable + 1"!][!//
            [!ENDIF!][!//
        [!ENDIF!][!//
        [!ENDLOOP!][!//
    [!ENDSELECT!][!//
[!ENDNOCODE!][!//

/**
 * @brief Adds or removes the for support Signal Measurement with DMA.
 *        STD_ON:  DMA in SM measurement can be used.
 *        STD_OFF: DMA in SM measurement can not be used.
 */
#define ICU_SIGNALMEASUREMENT_USES_DMA   ([!IF "$MeasurementDmaEnable != 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
 * @brief Adds or removes the support for TimeStamp Measurement with DMA.
 *        STD_ON:  DMA in TS measurement can be used.
 *        STD_OFF: DMA in TS measurement can not be used.
 */
#define ICU_TIMESTAMP_USES_DMA           ([!IF "$TimestampDmaEnable != 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#if ((ICU_SIGNALMEASUREMENT_USES_DMA == STD_ON) || (ICU_TIMESTAMP_USES_DMA == STD_ON))
/**
 * @brief Implementation specific.
 *        Each channel provides a subset of the functionality available in the unified channel,
 *        at a resolution of 32 bits.
 */
#define ICU_CAPTURE_REGISTER_MASK   (((Icu_ValueType)0xFFFFU))

/**
 * @brief Implementation specific.
 *        Each channel provides a DMA resolution of 16 bits.
 */
#define ICU_DMA_SIZE                (DMA_IP_TRANSFER_SIZE_2_BYTE)

/**
 * @brief Implementation specific.
 *        DMA number of bytes transfer on a minor loop is 2 bytes - for 16 bits hw registers.
 */
#define ICU_DMA_NUM_BYTES           (2U)

/**
 * @brief Implementation specific.
 *        DMA offset is 16 bits HW registers.
 */
#define ICU_DMA_OFFSET              (2U)

/** @brief Define when no MCL DMA channel is used. */
#define NoMclDmaChannel             ((Mcl_ChannelType)0xFF)

#endif /* ((ICU_SIGNALMEASUREMENT_USES_DMA == STD_ON) || (ICU_TIMESTAMP_USES_DMA == STD_ON)) */

/**
*   @brief   Adds / removes the service Icu_SetClockMode() from the code.
*         STD_ON:  Icu_SetClockMode() can be used.
*         STD_OFF: Icu_SetClockMode() can not be used.
*
*/
#define ICU_DUAL_CLOCK_MODE_API     ([!IF "IcuAutosarExt/IcuEnableDualClockMode"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
*   @brief   Adds / removes the service Icu_GetCaptureRegisterValue() from the code.
*         STD_ON:  Icu_GetCaptureRegisterValue() can be used.
*         STD_OFF: Icu_GetCaptureRegisterValue() can not be used.
*/
#define ICU_CAPTURERGISTER_API      ([!IF "node:exists(IcuAutosarExt/IcuGetCaptureRegisterValueApi)"!][!IF "IcuAutosarExt/IcuGetCaptureRegisterValueApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ELSE!]STD_OFF[!ENDIF!])

[!INDENT "0"!]
[!NOCODE!]
    [!VAR "OuterLoopCounter" = "0"!]
    [!VAR "InnerLoopCounter" = "0"!]
    [!LOOP "IcuConfigSet/IcuChannel/*"!]
        [!VAR "OuterLoopCounter" = "$OuterLoopCounter + 1"!]
        [!VAR "InnerLoopCounter" = "0"!]
        [!VAR "Matchcounter" = "0"!]
        [!VAR "Name" = "node:name(.)"!]
        [!LOOP "../../IcuChannel/*"!]
            [!VAR "InnerLoopCounter" = "$InnerLoopCounter + 1"!]
            [!IF "$OuterLoopCounter >= $InnerLoopCounter"!]
            [!IF "($Name = node:name(.))"!]
                    [!VAR "Matchcounter" = "$Matchcounter + 1"!]
                [!ENDIF!]
            [!ENDIF!]
        [!ENDLOOP!]
        [!IF "$Matchcounter = 1"!]
                [!CODE!]
                 #define [!"node:name(.)"!]                     (IcuConf_IcuChannel_[!"node:name(.)"!])[!CR!]
                 #define IcuConf_IcuChannel_[!"node:name(.)"!]  ((Icu_ChannelType)[!"IcuChannelId"!]U) [!CR!]
                [!ENDCODE!]
        [!ENDIF!]
    [!ENDLOOP!]
[!ENDNOCODE!]

[!//[!CALL "ICU_GENERATE_USED_ISR"!]

[!NOCODE!]
    [!VAR "MaximumCoreId" = "0"!]
    [!VAR "CurrentCoreId" = "0"!]
    [!LOOP "./IcuGeneral/IcuEcucPartitionRef/*"!]
        [!VAR "IcuPartRefName" = "node:value(.)"!]    
        [!VAR "IcuPartRefShortName" = "substring-after($IcuPartRefName, '/')"!]
        [!VAR "IcuPartRefShortName" = "substring-after($IcuPartRefShortName, '/')"!]
        [!VAR "IcuPartRefShortName" = "substring-after($IcuPartRefShortName, '/')"!]
        [!VAR "IcuPartRefShortName" = "substring-after($IcuPartRefShortName, '/')"!]
        [!VAR "IcuPartRefShortName" = "text:toupper($IcuPartRefShortName)"!]
        [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
            [!IF "node:refvalid(./OsAppEcucPartitionRef)"!]
                [!IF "$IcuPartRefName = node:value(./OsAppEcucPartitionRef)"!]
                    [!IF "node:refvalid(./OsApplicationCoreRef)"!]
                        [!VAR "CurrentCoreId" = "node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!]
                        [!IF "num:i($MaximumCoreId) < num:i($CurrentCoreId)"!]
                            [!VAR "MaximumCoreId" = "$CurrentCoreId"!]
                        [!ENDIF!]
[!CODE!][!//
/**
*   @brief Definition represents for mapping of EcucPartitions to CoreId
*   @details
*/
#define [!"$IcuPartRefShortName"!]_CORE_ID      ([!"node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!]U)
[!ENDCODE!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDIF!]
        [!ENDLOOP!]
    [!ENDLOOP!]
    [!VAR "MaximumCoreId" = "$MaximumCoreId + 1"!]
[!ENDNOCODE!]

[!CALL "ECUC_MAX_PARTITION_NUMBER"!]
/**
* @brief          Number of configured partitions.
*/
#define ICU_MAX_PARTITIONS                    ([!"num:i($maxPartition + 1)"!]U)

#define ICU_MULTICORE_SUPPORT             ([!IF "node:value(IcuGeneral/IcuMulticoreSupport)='true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])


[!MACRO "IcuExportDriverConfiguration"!]
[!NOCODE!][!//
[!VAR "configName" = "as:name(IcuConfigSet)"!][!//

[!CODE!][!//
/**
*   @brief export configuration icu driver
*/
#define ICU_CONF_PB \
[!ENDCODE!][!//
[!IF "var:defined('postBuildVariant')"!][!//
    [!VAR "variantIndex"="0"!][!//
    [!VAR "variantNumber"="variant:size()"!][!//
    [!LOOP "variant:all()"!][!VAR "variantIndex"="$variantIndex + 1"!][!//
    [!//
        [!CODE!][!WS4!]extern const Icu_ConfigType * [!"$configName"!]_[!"."!][ICU_MAX_PARTITIONS];[!IF "$variantIndex < $variantNumber"!] \[!ENDIF!][!CR!][!ENDCODE!]
    [!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS4!]extern const Icu_ConfigType * [!"$configName"!][ICU_MAX_PARTITIONS];[!CR!][!ENDCODE!]
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!]

[!ENDINDENT!]
/*==================================================================================================
                                             ENUMS
==================================================================================================*/


/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
*   @brief Implementation specific. This type shall be chosen in order to have the most efficient
*       implementation on a specific microcontroller platform.
*       Range: 0  to width of the timer register.
*       Description: Width of the buffer for timestamp ticks and measured elapsed timeticks
*/
typedef uint16 Icu_TimerRegisterWidthType;

#if (STD_ON == ICU_TIMESTAMP_API)
/**
*   @brief Implementation specific. This type shall be chosen in order to have the most efficient
*       implementation on a specific microcontroller platform.
*       Description: Type, to abstract the return value of the service Icu_GetTimestampIndex().
*/
typedef uint16 Icu_HwSpecificIndexType;
#endif /* (STD_ON == ICU_TIMESTAMP_API) */

#if (STD_ON == ICU_EDGE_COUNT_API)
/**
*   @brief Implementation specific. This type shall be chosen in order to have the most efficient
*       implementation on a specific microcontroller platform.
*       Description: Type, to abstract the return value of the service Icu_GetEdgeNumbers().
*/
typedef uint16 Icu_HwSpecificEdgeNumberType;
#endif /* (STD_ON == ICU_EDGE_COUNT_API) */

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif    /* ICU_CFG_H */

/** @} */
[!ENDCODE!]
