[!CODE!][!//
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

#ifndef ADC_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_H
#define ADC_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_H

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

#include "Adc_Types.h"

[!NOCODE!][!//
[!INCLUDE "Adc_VersionCheck_Src.m"!][!//
[!INCLUDE "Adc_RegOperations.m"!][!//
[!ENDNOCODE!][!//
[!//
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ADC_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG                      43
#define ADC_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG       4
#define ADC_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG       4
#define ADC_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG    0
#define ADC_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG               2
#define ADC_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG               0
#define ADC_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG               1
/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/* Check if Adc Configuration header file and Adc Types header file are of the same vendor */
#if (ADC_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG != ADC_VENDOR_ID_TYPES)
    #error "Adc[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Adc_Types.h have different vendor ids"
#endif

/* Check if Adc Configuration header file and Adc Types header file are of the same Autosar version */
#if ((ADC_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG != ADC_AR_RELEASE_MAJOR_VERSION_TYPES) || \
     (ADC_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG != ADC_AR_RELEASE_MINOR_VERSION_TYPES) || \
     (ADC_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG != ADC_AR_RELEASE_REVISION_VERSION_TYPES) \
    )
    #error "AutoSar Version Numbers of Adc[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Adc_Types.h are different"
#endif

/* Check if Adc Configuration header file and Adc Types header file are of the same Software version */
#if ((ADC_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG != ADC_SW_MAJOR_VERSION_TYPES) || \
     (ADC_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG != ADC_SW_MINOR_VERSION_TYPES) || \
     (ADC_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG != ADC_SW_PATCH_VERSION_TYPES) \
    )
  #error "Software Version Numbers of Adc[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Adc_Types.h are different"
#endif
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

[!NOCODE!]
[!IF "not((IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1))"!][!//
    [!VAR "VariantsNo" = "variant:size()"!]
    [!IF "var:defined('postBuildVariant')"!][!//
        [!VAR "postBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!][!//
        [!CODE!]#define ADC_CONFIG_[!"$postBuildVariant"!]_PB \[!CR!][!ENDCODE!]
    [!ELSE!][!//
        [!VAR "postBuildVariantNameUnderscore" = "string(null)"!][!//
        [!CODE!]#define ADC_CONFIG_PB \[!CR!][!ENDCODE!]
    [!ENDIF!][!//
    [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!][!//
        [!VAR "AdcConfiguredPartitions" = "num:i(count(AdcGeneral/AdcEcucPartitionRef/*))"!][!//
    [!ELSE!][!//
        [!VAR "AdcConfiguredPartitions" = "num:i(1)"!][!//
    [!ENDIF!][!//
    [!FOR "Partition" = "0" TO "num:i($AdcConfiguredPartitions) - 1"!][!//
        [!IF "$VariantsNo > 0"!][!//
            [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!][!//
                [!VAR "currentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcGeneral/AdcEcucPartitionRef/*[@index = $Partition],'/'),'/'),'/'),'/')"!][!//
                [!CODE!][!WS4!]extern const Adc_ConfigType Adc_Config[!"$postBuildVariantNameUnderscore"!]_[!"$currentPartition"!];\[!CR!][!ENDCODE!]
            [!ELSE!][!//
                [!CODE!][!WS4!]extern const Adc_ConfigType Adc_Config[!"$postBuildVariantNameUnderscore"!];\[!CR!][!ENDCODE!]
            [!ENDIF!][!//
        [!ELSE!][!//
            [!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!][!//
                [!VAR "currentPartition" = "substring-after(substring-after(substring-after(substring-after(AdcGeneral/AdcEcucPartitionRef/*[@index = $Partition],'/'),'/'),'/'),'/')"!]
                [!CODE!][!WS4!]extern const Adc_ConfigType Adc_Config_[!"$currentPartition"!];\[!CR!][!ENDCODE!]
            [!ELSE!][!//
                [!CODE!][!WS4!]extern const Adc_ConfigType Adc_Config;\[!ENDCODE!]
            [!ENDIF!][!//
        [!ENDIF!][!//
    [!ENDFOR!][!//
[!ENDIF!][!// Support for Post Build only
[!ENDNOCODE!][!//

/**
* @brief           Number of channels configured for each group.
*
*/
[!FOR "x" = "0" TO "count(AdcConfigSet/AdcHwUnit/*/AdcGroup/*)-1"!][!//
[!LOOP "AdcConfigSet/AdcHwUnit/*/AdcGroup/*"!][!//
[!IF "$x = AdcGroupId"!]
#define ADC_CFGSET[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_GROUP_[!"AdcGroupId"!]_CHANNELS      ([!"num:i(count(AdcGroupDefinition/*))"!]U)[!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDFOR!][!//

[!IF "node:value(AutosarExt/AdcMulticoreSupport)='true'"!]
/**
* @brief           Number of partitions configured for Configset[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!].
*/
#define ADC_CFGSET[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PARTITION             ([!"num:i(count(AdcGeneral/AdcEcucPartitionRef/*))"!]U)[!//
[!ENDIF!][!//

[!CALL "AdcGroupDefineMacro"!]

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

#define ADC_START_SEC_CODE
#include "Adc_MemMap.h"

[!CALL "AdcNotificationConfigMacro"!][!//

#define ADC_STOP_SEC_CODE
#include "Adc_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* ADC_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_H */
[!ENDCODE!]
