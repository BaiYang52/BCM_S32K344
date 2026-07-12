[!AUTOSPACING!]
[!NOCODE!]
    [!IF "ecu:has('Pwm.ExclusiveHwInstance') and (ecu:get('Pwm.ExclusiveHwInstance') = 'true')"!]
        [!VAR "instanceCount" = "'0'"!]
    [!ENDIF!]

    [!IF "ecu:has('Pwm.FtmSupported') and (ecu:get('Pwm.FtmSupported') = 'true')"!]
        [!VAR "numModules" = "ecu:get('Pwm.FtmInstanceCount')"!]
        [!VAR "numChannelsPerModule" = "ecu:get('Pwm.FtmChannelsCount')"!]
        [!VAR "numChannels" = "($numModules * $numChannelsPerModule)"!]
        [!VAR "pwmFtmSupported" = "'true'"!]
        [!IF "var:defined('instanceCount')"!][!VAR "instanceCount" = "num:i($instanceCount) + count(PwmChannelConfigSet/PwmFtm/*)"!][!ENDIF!]
    [!ENDIF!]

    /* Variable indicate multicore feature enabled */
    [!VAR "MulticoreEnabled" = "0"!]

    [!IF "./PwmGeneral/PwmMulticoreEnabled = 'true'"!]
        [!VAR "MulticoreEnabled" = "1"!]
    [!ENDIF!]

        /* Variable storing number of maximum partitions using in configuration */
    [!VAR "numPartition" = "0"!]

    [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
        [!SELECT "node:ref(./OsApplicationCoreRef)"!]
            [!IF "$numPartition < node:value(./EcucCoreId)"!]
                [!VAR "numPartition" = "node:value(./EcucCoreId)"!]
            [!ENDIF!]
        [!ENDSELECT!]
    [!ENDLOOP!]

    [!VAR "numPartition" = "$numPartition + 1"!]
[!ENDNOCODE!]
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

#ifndef PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_H
#define PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_H

/**
*   @file       Pwm_PBcfg.h
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
#include "StandardTypes.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_VENDOR_ID                    43
#define PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_MODULE_ID                    121
#define PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_AR_RELEASE_MAJOR_VERSION     4
#define PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_AR_RELEASE_MINOR_VERSION     4
#define PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_AR_RELEASE_REVISION_VERSION  0
#define PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_SW_MAJOR_VERSION             2
#define PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_SW_MINOR_VERSION             0
#define PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_SW_PATCH_VERSION             1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if source file and StandardTypes.h are of the same AUTOSAR version */
    #if ((PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
         (PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR version numbers of Pwm[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and StandardTypes.h are different."
    #endif
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
[!IF "$MulticoreEnabled = 1"!]
    [!IF "(./../IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1)"!]
/**
* @brief        Total number of partition configured
*
*/
#define PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CONF_PARTITIONS         ((uint8)[!"num:i($numPartition)"!]U)

    [!ENDIF!]
[!ENDIF!]
[!//
/**
* @brief        Total number of configured PWM channels
*/
#define PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_CHANNELS_COUNT           ((uint8)[!"num:i(count(PwmChannelConfigSet/PwmChannel/*))"!]U)

[!IF "ecu:has('Pwm.ExclusiveHwInstance') and (ecu:get('Pwm.ExclusiveHwInstance') = 'true')"!]
/**
* @brief        Total number of configured PWM instances
*/
#define PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_INSTANCES_COUNT          ((uint8)[!IF "$instanceCount > 0"!][!"num:i($instanceCount)"!]U[!ELSE!]1U[!ENDIF!])

[!ENDIF!]
[!//
[!IF "$MulticoreEnabled = 1"!]
    [!LOOP "as:modconf('Pwm')[1]/PwmGeneral/PwmEcucPartitionRef/*"!]
        [!NOCODE!]
            [!VAR "PartitionIndex" = "@index"!]

            [!VAR "PwmEcucPartitionRefName" = "node:value(.)"!]
            [!VAR "PwmEcucPartitionShortName" = "substring-after($PwmEcucPartitionRefName, '/')"!]
            [!VAR "PwmEcucPartitionShortName" = "substring-after($PwmEcucPartitionShortName, '/')"!]
            [!VAR "PwmEcucPartitionShortName" = "substring-after($PwmEcucPartitionShortName, '/')"!]
            [!VAR "PwmEcucPartitionShortName" = "substring-after($PwmEcucPartitionShortName, '/')"!]

            [!VAR "NumChannelInPartition" = "0"!]
            [!VAR "NumModuleInPartition" = "0"!]

            /* Count channels in each module that is using for current partition */
            [!LOOP "as:modconf('Pwm')[1]/PwmChannelConfigSet/PwmChannel/*"!]

                [!LOOP "./PwmChannelEcucPartitionRef/*"!]
                    [!VAR "PwmChannelEcucPartitionRefName" = "node:value(.)"!]

                    [!IF "$PwmChannelEcucPartitionRefName = $PwmEcucPartitionRefName"!]
                        [!VAR "NumChannelInPartition" = "$NumChannelInPartition + 1"!]
                    [!ENDIF!]
                [!ENDLOOP!]
            [!ENDLOOP!]

            [!IF "(num:i($NumChannelInPartition)) > 0"!]
                [!VAR "NumModuleInPartition" = "0"!]

                [!LOOP "as:modconf('Pwm')[1]/PwmChannelConfigSet/PwmFtm/*"!]
                    [!VAR "HardwareModule" = "./PwmHwInstance"!]
                    [!VAR "HasOccurred" = "0"!]

                    [!LOOP "./../../PwmChannel/*"!]
                        [!IF "$HasOccurred = 0"!]
                            [!LOOP "./PwmChannelEcucPartitionRef/*"!]
                                [!VAR "PwmChannelEcucPartitionRefName" = "node:value(.)"!]

                                [!IF "$PwmChannelEcucPartitionRefName = $PwmEcucPartitionRefName"!]
                                     [!IF "node:value(node:ref(./../../PwmHwChannel)/../../PwmHwInstance) = $HardwareModule"!]
                                        [!VAR "NumModuleInPartition" = "$NumModuleInPartition + 1"!]
                                        [!VAR "HasOccurred" = "1"!]
                                    [!ENDIF!]
                                [!ENDIF!]
                            [!ENDLOOP!]
                        [!ENDIF!]
                    [!ENDLOOP!]
                [!ENDLOOP!]
            [!ENDIF!]
        [!ENDNOCODE!]
        [!IF "(num:i($NumChannelInPartition)) > 0"!]
/*================================== Define in [!"$PwmEcucPartitionShortName"!] ==================================*/

/**
* @brief        Number of configured PWM channels in [!"$PwmEcucPartitionShortName"!]
*/
#define PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_CHANNELS_COUNT_[!"text:toupper($PwmEcucPartitionShortName)"!]    ((uint8)[!"num:i($NumChannelInPartition)"!]U)

/**
* @brief        Total number of configured PWM instances in [!"$PwmEcucPartitionShortName"!]
*/
#define PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_INSTANCES_COUNT_[!"text:toupper($PwmEcucPartitionShortName)"!]    ((uint8)[!IF "$NumModuleInPartition > 0"!][!"num:i($NumModuleInPartition)"!]U[!ELSE!]1U[!ENDIF!])

        [!ENDIF!]
    [!ENDLOOP!]
[!ENDIF!]
[!//
/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
#define PWM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Pwm_MemMap.h"

[!IF "not((IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1))"!]
    [!VAR "VariantsNo" = "variant:size()"!]
    [!VAR "PwmConfiguredPartitions" = "num:i((count(PwmGeneral/PwmEcucPartitionRef/*)))"!]
    [!VAR "IndexPartitions" = "0"!]
    [!VAR "NumPartitionsUsed" = "0"!]
[!//
    [!IF "var:defined('postBuildVariant')"!]
        [!VAR "postBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!]
#define PWM_CONFIG_[!"$postBuildVariant"!]_PB \
    [!ELSE!]
        [!VAR "postBuildVariantNameUnderscore" = "string(null)"!]
#define PWM_CONFIG_PB \
    [!ENDIF!]
[!//
    [!IF "$PwmConfiguredPartitions > 0"!]
        [!LOOP "PwmGeneral/PwmEcucPartitionRef/*"!]
            [!VAR "currentPartition" = "substring-after(substring-after(substring-after(substring-after(.,'/'),'/'),'/'),'/')"!]
            [!VAR "channelConfigured" = "num:i(count(//PwmChannelConfigSet/PwmChannel/*[contains(./PwmChannelEcucPartitionRef/*,$currentPartition)]))"!]
            [!IF "$channelConfigured > 0"!]
                [!VAR "NumPartitionsUsed" = "$NumPartitionsUsed + 1"!]
            [!ENDIF!]
        [!ENDLOOP!]
[!//
        [!LOOP "PwmGeneral/PwmEcucPartitionRef/*"!]
            [!VAR "currentPartition" = "substring-after(substring-after(substring-after(substring-after(.,'/'),'/'),'/'),'/')"!]
            [!VAR "channelConfigured" = "num:i(count(//PwmChannelConfigSet/PwmChannel/*[contains(./PwmChannelEcucPartitionRef/*,$currentPartition)]))"!]
            [!IF "$channelConfigured > 0"!]
        extern const Pwm_ConfigType Pwm_Config[!"$postBuildVariantNameUnderscore"!]_[!"$currentPartition"!]; [!IF "$IndexPartitions < ($NumPartitionsUsed - 1)"!]\[!ENDIF!][!CR!]
            [!VAR "IndexPartitions" = "$IndexPartitions + 1"!]
            [!ENDIF!]
        [!ENDLOOP!]
    [!ELSE!]
        extern const Pwm_ConfigType Pwm_Config[!"$postBuildVariantNameUnderscore"!];
    [!ENDIF!]
[!ENDIF!]

#define PWM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Pwm_MemMap.h"

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

#endif /* PWM[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_H */

[!ENDCODE!]
