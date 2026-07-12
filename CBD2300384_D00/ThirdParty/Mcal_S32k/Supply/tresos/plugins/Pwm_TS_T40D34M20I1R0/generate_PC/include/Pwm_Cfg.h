[!AUTOSPACING!]
[!CODE!]
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Emios Flexio
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

#ifndef PWM_CFG_H
#define PWM_CFG_H

/**
*   @file       Pwm_Cfg.h
*
*   @addtogroup pwm_driver Pwm Driver
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
[!NOCODE!]

    /* TODO: revert include file to Pwm_VersionCheck_Inc.m */
    [!INCLUDE "Pwm_VersionCheck_Inc.m"!]

    [!VAR "numChannels" = "0"!]

    [!IF "ecu:has('Pwm.FtmSupported') and (ecu:get('Pwm.FtmSupported') = 'true')"!]
        [!VAR "numModules" = "ecu:get('Pwm.FtmInstanceCount')"!]
        [!VAR "numChannelsPerModule" = "ecu:get('Pwm.FtmChannelsCount')"!]
        [!VAR "numChannels" = "$numChannels + ($numModules * $numChannelsPerModule)"!]
        [!VAR "pwmFtmSupported" = "'true'"!]
    [!ENDIF!]

    [!IF "ecu:has('Pwm.EmiosSupported') and (ecu:get('Pwm.EmiosSupported') = 'true')"!]
        [!VAR "numModules" = "ecu:get('Pwm.EmiosInstanceCount')"!]
        [!VAR "numChannelsPerModule" = "ecu:get('Pwm.EmiosChannelCount')"!]
        [!VAR "numChannels" = "$numChannels + ($numModules * $numChannelsPerModule)"!]
        [!VAR "pwmEmiosSupported" = "'true'"!]
    [!ENDIF!]

    [!IF "ecu:has('Pwm.FlexioSupported') and (ecu:get('Pwm.FlexioSupported') = 'true')"!]
        [!VAR "numModules" = "ecu:get('Pwm.FlexioInstanceCount')"!]
        [!VAR "numChannelsPerModule" = "ecu:get('Pwm.FlexioChannelCount')"!]
        [!VAR "numChannels" = "$numChannels + ($numModules * $numChannelsPerModule)"!]
        [!VAR "pwmFlexioSupported" = "'true'"!]
    [!ENDIF!]

[!ENDNOCODE!]
#include "StandardTypes.h"

/* Include IPW Cfg header file */
#include "Pwm_Ipw_Cfg.h"

/* Include all variants header files. */
[!IF "var:defined('postBuildVariant')"!]
    [!LOOP "variant:all()"!]
#include "Pwm_[!"."!]_PBcfg.h"
    [!ENDLOOP!]
[!ELSE!]
#include "Pwm_PBcfg.h"
[!ENDIF!]

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define PWM_CFG_VENDOR_ID                         43
#define PWM_CFG_MODULE_ID                         121
#define PWM_CFG_AR_RELEASE_MAJOR_VERSION          4
#define PWM_CFG_AR_RELEASE_MINOR_VERSION          4
#define PWM_CFG_AR_RELEASE_REVISION_VERSION       0
#define PWM_CFG_SW_MAJOR_VERSION                  2
#define PWM_CFG_SW_MINOR_VERSION                  0
#define PWM_CFG_SW_PATCH_VERSION                  1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if header file and StandardTypes.h are of the same AUTOSAR version */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((PWM_CFG_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
         (PWM_CFG_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR version numbers of Pwm_Cfg.h and StandardTypes.h are different."
    #endif
#endif

/* Check if header file and PWM configuration header file are of the same vendor */
#if (PWM_CFG_VENDOR_ID != PWM_IPW_CFG_VENDOR_ID)
    #error "Pwm_Cfg.h and Pwm_Ipw_Cfg.h have different vendor IDs"
#endif
    /* Check if header file and Pwm configuration header file are of the same Autosar version */
#if ((PWM_CFG_AR_RELEASE_MAJOR_VERSION != PWM_IPW_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (PWM_CFG_AR_RELEASE_MINOR_VERSION != PWM_IPW_CFG_AR_RELEASE_MINOR_VERSION) || \
     (PWM_CFG_AR_RELEASE_REVISION_VERSION != PWM_IPW_CFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Pwm_Cfg.h and Pwm_Ipw_Cfg.h are different"
#endif
/* Check if header file and Pwm configuration header file are of the same software version */
#if ((PWM_CFG_SW_MAJOR_VERSION != PWM_IPW_CFG_SW_MAJOR_VERSION) || \
     (PWM_CFG_SW_MINOR_VERSION != PWM_IPW_CFG_SW_MINOR_VERSION) || \
     (PWM_CFG_SW_PATCH_VERSION != PWM_IPW_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Pwm_Cfg.h and Pwm_Ipw_Cfg.h are different"
#endif

[!IF "var:defined('postBuildVariant')"!]
    [!LOOP "variant:all()"!]
/* Check if header file and PWM configuration header file are of the same vendor */
#if (PWM_CFG_VENDOR_ID != PWM_[!"."!]_PB_CFG_VENDOR_ID)
    #error "Pwm_Cfg.h and Pwm_[!"."!]_PBcfg.h have different vendor IDs"
#endif
    /* Check if header file and Pwm configuration header file are of the same Autosar version */
#if ((PWM_CFG_AR_RELEASE_MAJOR_VERSION != PWM_[!"."!]_PB_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (PWM_CFG_AR_RELEASE_MINOR_VERSION != PWM_[!"."!]_PB_CFG_AR_RELEASE_MINOR_VERSION) || \
     (PWM_CFG_AR_RELEASE_REVISION_VERSION != PWM_[!"."!]_PB_CFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Pwm_Cfg.h and Pwm_[!"."!]_PBcfg.h are different"
#endif
/* Check if header file and Pwm configuration header file are of the same software version */
#if ((PWM_CFG_SW_MAJOR_VERSION != PWM_[!"."!]_PB_CFG_SW_MAJOR_VERSION) || \
     (PWM_CFG_SW_MINOR_VERSION != PWM_[!"."!]_PB_CFG_SW_MINOR_VERSION) || \
     (PWM_CFG_SW_PATCH_VERSION != PWM_[!"."!]_PB_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Pwm_Cfg.h and Pwm_[!"."!]_PBcfg.h are different"
#endif
    [!ENDLOOP!]
[!ELSE!]
/* Check if header file and PWM configuration header file are of the same vendor */
#if (PWM_CFG_VENDOR_ID != PWM_PB_CFG_VENDOR_ID)
    #error "Pwm_Cfg.h and PWM_PBcfg.h have different vendor IDs"
#endif
    /* Check if header file and Pwm configuration header file are of the same Autosar version */
#if ((PWM_CFG_AR_RELEASE_MAJOR_VERSION != PWM_PB_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (PWM_CFG_AR_RELEASE_MINOR_VERSION != PWM_PB_CFG_AR_RELEASE_MINOR_VERSION) || \
     (PWM_CFG_AR_RELEASE_REVISION_VERSION != PWM_PB_CFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Pwm_Cfg.h and Pwm_PBcfg.h are different"
#endif
/* Check if header file and Pwm configuration header file are of the same software version */
#if ((PWM_CFG_SW_MAJOR_VERSION != PWM_PB_CFG_SW_MAJOR_VERSION) || \
     (PWM_CFG_SW_MINOR_VERSION != PWM_PB_CFG_SW_MINOR_VERSION) || \
     (PWM_CFG_SW_PATCH_VERSION != PWM_PB_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Pwm_Cfg.h and Pwm_PBcfg.h are different"
#endif
[!ENDIF!]

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/
/**
* @brief        Specifies the InstanceId of this module instance.
*               If only one instance is present it shall have the Id 0.
* @note         Not used in the current implementation
*/
#define PWM_INDEX                               ([!"PwmGeneral/PwmIndex "!]U)

[!VAR "logicChannelCount" = "num:i(count(PwmChannelConfigSet/PwmChannel/*))"!]
/** @brief      Total number of Pwm logic channels configured. */
#define PWM_CONFIG_LOGIC_CHANNELS               ([!"num:i($logicChannelCount)"!]U)

#define PWM_HW_CHANNEL_NO                       ([!"num:i($numChannels)"!]U)

[!NOCODE!]
[!IF "./PwmGeneral/PwmMulticoreEnabled = 'true'"!]
        [!VAR "MaximumCoreId" = "0"!]

        [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
            [!SELECT "node:ref(./OsApplicationCoreRef)"!]
                [!IF "$MaximumCoreId < node:value(./EcucCoreId)"!]
                    [!VAR "MaximumCoreId" = "node:value(./EcucCoreId)"!]
                [!ENDIF!]
            [!ENDSELECT!]
        [!ENDLOOP!]
        [!VAR "MaximumCoreId" = "$MaximumCoreId + 1"!]
[!ELSE!]
    [!VAR "MaximumCoreId" = "1"!]
[!ENDIF!]
[!ENDNOCODE!]
/** @brief        Maximum number of partitions declared in Os. */
#define PWM_MAX_PARTITION_NO                    ([!"num:i($MaximumCoreId)"!]U)

/** @brief      Macro used to initialize the driver state structure */
#define PWM_DRIVER_STATE_INITIALIZATION         {\
[!FOR "coreId" = "1" TO "num:i($MaximumCoreId)"!]
                                                    {\
         [!IF "PwmGeneral/PwmDevErrorDetect"!][!WS"56"!]PWM_STATE_UNINIT,\[!CR!][!ENDIF!]
                                                        NULL_PTR,\
                                                        PWM_NODEFINE_POWER,\
                                                        PWM_NODEFINE_POWER,\
                                                        {\
                                                        [!FOR "channelId" = "1" TO "num:i($logicChannelCount)"!]
                                                            NULL_PTR[!IF "number($channelId) < num:i($logicChannelCount)"!],[!ENDIF!]\
                                                        [!ENDFOR!]
                                                        },\
                                                        {\
                                                        [!FOR "channelId" = "1" TO "num:i($logicChannelCount)"!]
                                                            (boolean)FALSE[!IF "number($channelId) < num:i($logicChannelCount)"!],[!ENDIF!]\
                                                        [!ENDFOR!]
                                                        }\
                                                    }[!IF "number($coreId) < num:i($MaximumCoreId)"!],[!ENDIF!]\
[!ENDFOR!]
                                                }

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/**
* @brief        Pre-compile configuration constants
*
*/
#define PWM_PRECOMPILE_SUPPORT                  ([!IF "(IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_SetClockMode API is supported */
#define PWM_VERSION_INFO_API                    ([!IF "PwmConfigurationOfOptApiServices/PwmVersionInfoApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
* @brief        Switch to indicate that Pwm_ForceOutputToZero API is supported
* @note         In current implementation, this option is not supported
*/
#define PWM_FORCE_OUTPUT_TO_ZERO_API            ([!IF "node:when(node:exists(PwmConfigurationOfOptApiServices/PwmForceOutputToZeroApi), PwmConfigurationOfOptApiServices/PwmForceOutputToZeroApi, false())"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_SetPhaseShift API is supported */
#define PWM_SET_PHASE_SHIFT_API                 ([!IF "node:when(node:exists(PwmConfigurationOfOptApiServices/PwmSetPhaseShift), PwmConfigurationOfOptApiServices/PwmSetPhaseShift, false())"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_SetDutyPhaseShift API is supported */
#define PWM_SET_DUTY_PHASE_SHIFT_API            ([!IF "node:when(node:exists(PwmConfigurationOfOptApiServices/PwmSetDutyPhaseShift), PwmConfigurationOfOptApiServices/PwmSetDutyPhaseShift, false())"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_SyncUpdate API is supported */
#define PWM_SYNC_UPDATE_API                     ([!IF "PwmGeneral/PwmMultiChannelSync"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_SetDutyCycle_NoUpdate API is supported */
#define PWM_SET_DUTY_CYCLE_NO_UPDATE_API        ([!IF "(PwmConfigurationOfOptApiServices/PwmSetDutyCycle_NoUpdate = 'true') and (PwmGeneral/PwmMultiChannelSync = 'true' )"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_SetPeriodAndDuty_NoUpdate API is supported */
#define PWM_SET_PERIOD_AND_DUTY_NO_UPDATE_API   ([!IF "(PwmConfigurationOfOptApiServices/PwmSetPeriodAndDuty_NoUpdate = 'true') and (PwmGeneral/PwmMultiChannelSync = 'true' )"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_SetPhaseShift_NoUpdate API is supported */
#define PWM_SET_PHASE_SHIFT_NO_UPDATE_API       ([!IF "(PwmConfigurationOfOptApiServices/PwmSetPhaseShift_NoUpdate = 'true') and (PwmGeneral/PwmMultiChannelSync = 'true' )"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_EnableTrigger API is supported */
#define PWM_ENABLE_TRIGGER_API                  ([!IF "node:when(node:exists(PwmConfigurationOfOptApiServices/PwmEnableTrigger), PwmConfigurationOfOptApiServices/PwmEnableTrigger, false())"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_DisableTrigger API is supported */
#define PWM_DISABLE_TRIGGER_API                 ([!IF "node:when(node:exists(PwmConfigurationOfOptApiServices/PwmDisableTrigger), PwmConfigurationOfOptApiServices/PwmDisableTrigger, false())"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_SwResetCounter API is supported */
#define PWM_RESET_COUNTER_API                   ([!IF "node:when(node:exists(PwmConfigurationOfOptApiServices/PwmResetCounter), PwmConfigurationOfOptApiServices/PwmResetCounter, false())"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
* @brief        Switch to indicate that Pwm_SetChannelForceOut API is supported
* @note         In current implementation, this function is not supported for FTM
*/
#define PWM_SET_CHANNEL_FORCE_OUT_API           (STD_OFF)

/* @brief Add/remove the service Pwm_SetChannelDeadTime() from the code.*/
#define PWM_SET_CHANNEL_DEAD_TIME_API           ([!IF "PwmConfigurationOfOptApiServices/PwmSetChannelDeadTime"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to enable the update of the duty cycle parameter at the end of the current period. */
#define PWM_DUTYCYCLE_UPDATED_ENDPERIOD         ([!IF "PwmGeneral/PwmDutycycleUpdatedEndperiod"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to enable the update of the period parameter at the end of the current period */
#define PWM_DUTY_PERIOD_UPDATED_ENDPERIOD       ([!IF "PwmGeneral/PwmPeriodUpdatedEndperiod"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to enable the mask output feature (Pwm_MaskOutput and Pwm_UnMaskOutput API) */
#define PWM_ENABLE_MASKING_OPERATIONS           ([!IF "node:when(node:exists(PwmGeneral/PwmEnableMaskOutputs), PwmGeneral/PwmEnableMaskOutputs, false())"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to enable update synchronization feature */
#define PWM_UPDATE_DUTY_SYNCHRONOUS             ([!IF "PwmGeneral/PwmMultiChannelSync"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])


/**
* @brief        Support for User mode.
*               If this parameter has been configured to 'TRUE' the PWM driver code can be executed from both supervisor and user mode.
*/
#define PWM_ENABLE_USER_MODE_SUPPORT            ([!IF "PwmGeneral/PwmEnableUserModeSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    #ifdef PWM_ENABLE_USER_MODE_SUPPORT
        #if (STD_ON == PWM_ENABLE_USER_MODE_SUPPORT)
            #error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running PWM in user mode the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined
        #endif
    #endif
#endif

/** @brief        Switch to indicate that the Reload notifications are supported */
#define PWM_RELOAD_NOTIF_SUPPORTED              (STD_OFF)

/**
* @brief        Switch to enable the fault functionality
* @note         In current implementation, this option is not supported
*/
#define PWM_FAULT_SUPPORTED                     (STD_OFF)

/**
* @brief        Switch to indicate that Pwm_SelectCommonTimebase API is supported
* @note         In current implementation, this option is not supported
*/
#define PWM_SELECT_COMMON_TIMEBASE_API          (STD_OFF)

/** @brief        Enable/Disable multicore feature for PWM driver */
#define PWM_MULTICORE_ENABLED                   ([!IF "./PwmGeneral/PwmMulticoreEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])



/**
* @brief        Symbolic Names for configured channels - legacy
*
*/
    [!LOOP "PwmChannelConfigSet/PwmChannel/*"!]
#define [!"node:name(.)"!]                            ((Pwm_ChannelType)[!"PwmChannelId"!]U)
    [!ENDLOOP!]

/**
* @brief        Symbolic name for configured channels - TPS_ECUC_02108 compliant.
*
*/
    [!LOOP "PwmChannelConfigSet/PwmChannel/*"!]
#define PwmConf_PwmChannelConfigSet_[!"node:name(.)"!]          ((Pwm_ChannelType)[!"PwmChannelId"!]U)
    [!ENDLOOP!]

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
#if (PWM_PRECOMPILE_SUPPORT == STD_OFF)
#define PWM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Pwm_MemMap.h"

/** @brief          Post Build configuration constants */
#define PWM_CONFIG_EXTERNAL \
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "variantCount" = "num:i(count(variant:all()))"!]
    [!VAR "variantId" = "1"!]
    [!LOOP "variant:all()"!]
                PWM_CONFIG_[!"."!]_PB[!IF "number($variantId) < number($variantCount)"!] \[!ENDIF!][!CR!]
        [!VAR "variantId" = "number($variantId) + 1"!]
    [!ENDLOOP!]
[!ELSE!][!WS"16"!]PWM_CONFIG_PB
[!ENDIF!]

#define PWM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Pwm_MemMap.h"
#endif

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/
#define PWM_START_SEC_CODE
#include "Pwm_MemMap.h"

#define PWM_STOP_SEC_CODE
#include "Pwm_MemMap.h"


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* PWM_CFG_H */

[!ENDCODE!]
