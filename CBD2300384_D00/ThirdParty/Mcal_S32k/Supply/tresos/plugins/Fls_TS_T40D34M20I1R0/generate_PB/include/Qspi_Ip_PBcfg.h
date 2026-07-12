/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : C40_IP IPV_QSPI
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
[!VAR "variantName" = "''"!][!//
[!IF "var:defined('postBuildVariant')"!][!//
[!VAR "variantName" = "concat($postBuildVariant, '_')"!][!//
[!ENDIF!][!//

#ifndef QSPI_IP_[!"$variantName"!]PBCFG_H
#define QSPI_IP_[!"$variantName"!]PBCFG_H

/**
*   @file Qspi_Ip_[!"$variantName"!]PBcfg.h
*
*   @addtogroup FLS
*   @{
*/

/* implements Qspi_Ip_PBcfg.h_Artifact */

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Qspi_Ip_Types.h"

/*==================================================================================================
                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define QSPI_IP_[!"$variantName"!]PBCFG_VENDOR_ID                    43

#define QSPI_IP_[!"$variantName"!]PBCFG_AR_RELEASE_MAJOR_VERSION     4
#define QSPI_IP_[!"$variantName"!]PBCFG_AR_RELEASE_MINOR_VERSION     4
#define QSPI_IP_[!"$variantName"!]PBCFG_AR_RELEASE_REVISION_VERSION  0

#define QSPI_IP_[!"$variantName"!]PBCFG_SW_MAJOR_VERSION             2
#define QSPI_IP_[!"$variantName"!]PBCFG_SW_MINOR_VERSION             0
#define QSPI_IP_[!"$variantName"!]PBCFG_SW_PATCH_VERSION             1

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and Qspi_Ip_Types.h header file are of the same vendor */
#if (QSPI_IP_[!"$variantName"!]PBCFG_VENDOR_ID != FLS_QSPI_TYPES_VENDOR_ID)
    #error "Qspi_Ip_[!"$variantName"!]PBcfg.h and Qspi_Ip_Types.h have different vendor ids"
#endif
/* Check if current file and Qspi_Ip_Types.h header file are of the same Autosar version */
#if ((QSPI_IP_[!"$variantName"!]PBCFG_AR_RELEASE_MAJOR_VERSION    != FLS_QSPI_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (QSPI_IP_[!"$variantName"!]PBCFG_AR_RELEASE_MINOR_VERSION    != FLS_QSPI_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (QSPI_IP_[!"$variantName"!]PBCFG_AR_RELEASE_REVISION_VERSION != FLS_QSPI_TYPES_AR_RELEASE_REVISION_VERSION) \
    )
#error "AutoSar Version Numbers of Qspi_Ip_[!"$variantName"!]PBcfg.h and Qspi_Ip_Types.h are different"
#endif
/* Check if current file and Qspi_Ip_Types.h header file are of the same software version */
#if ((QSPI_IP_[!"$variantName"!]PBCFG_SW_MAJOR_VERSION != FLS_QSPI_TYPES_SW_MAJOR_VERSION) || \
     (QSPI_IP_[!"$variantName"!]PBCFG_SW_MINOR_VERSION != FLS_QSPI_TYPES_SW_MINOR_VERSION) || \
     (QSPI_IP_[!"$variantName"!]PBCFG_SW_PATCH_VERSION != FLS_QSPI_TYPES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Qspi_Ip_[!"$variantName"!]PBcfg.h and Qspi_Ip_Types.h are different"
#endif

/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/


/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!][!//
  [!VAR "postBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!][!//
[!ELSE!][!//
  [!VAR "postBuildVariantNameUnderscore" = "string(null)"!][!//
[!ENDIF!][!//
[!//
[!VAR "LUTSeqDefAlignment" = "80"!][!//
[!VAR "PaddingSpaces" = "'                                                                  '"!][!//
[!MACRO "PrintLUTSequence", "LutName" = "", "LutPos" = ""!][!//
    [!VAR "LUTSequenceOut" = "concat('#define ', $LutName)"!][!// Add the LUT name
    [!VAR "LUTSequenceOut" = "concat($LUTSequenceOut, $PaddingSpaces)"!][!// Padding spaces for alignment
    [!VAR "LUTSequenceOut" = "substring($LUTSequenceOut, 1, $LUTSeqDefAlignment - string-length($LutPos))"!][!// Perform the alignment
    [!VAR "LUTSequenceOut" = "concat($LUTSequenceOut, ' ', $LutPos, 'U')"!][!// Add the LUT position
    [!CODE!][!"$LUTSequenceOut"!][!CR!][!ENDCODE!][!// Print the output string
[!ENDMACRO!][!//
[!//
[!INDENT "0"!] [!//
[!LOOP "FlsConfigSet"!][!//
    [!IF "num:i(count(FlsExternalDriver/MemCfg/*)) > 0"!][!//
        [!LOOP "FlsExternalDriver/MemCfg/*"!][!// Iterate through memory configurations
            [!VAR "FlsLutIdx" = "0"!][!//
            [!CODE!][!WS "0"!]/* Defines for direct access to the virtual LUT table */[!CR!][!ENDCODE!]
            [!CODE!][!WS "0"!]/* Configuration: [!"node:name(.)"!] */[!CR!][!ENDCODE!]
            [!VAR "FlsLutName" = "concat(node:name(.),$postBuildVariantNameUnderscore,'_LUT_SEQUENCE_')"!][!//
            [!LOOP "node:order(./FlsLUT/*, 'node:value(./FlsLUTIndex)')"!][!// Iterate through the Fls LUT Index
                [!CALL "PrintLUTSequence", "LutName" = "text:toupper(concat($FlsLutName,node:name(.)))", "LutPos" = "num:i($FlsLutIdx)"!]
                [!VAR "FlsLutIdx" = "$FlsLutIdx + 1 + num:i(count(./FlsInstructionOperandPair/*))"!][!//
            [!ENDLOOP!][!//
        [!ENDLOOP!][!//
    [!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDINDENT!][!//
[!ENDNOCODE!]

/*==================================================================================================
                                             ENUMS
==================================================================================================*/


/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/


/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

#define FLS_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fls_MemMap.h"

[!NOCODE!][!//
[!VAR "configName" = "as:name(FlsConfigSet)"!][!//
[!VAR "ControllerCfgCount"="num:i(count(FlsConfigSet/FlsExternalDriver/ControllerCfg/*))"!][!//
[!VAR "MemCfgCount"="num:i(count(FlsConfigSet/FlsExternalDriver/MemCfg/*))"!][!//
[!VAR "FlsMemCount"="num:i(count(FlsConfigSet/FlsExternalDriver/FlsMem/*))"!][!//
[!IF "var:defined('postBuildVariant')"!][!//
    [!CODE!][!CR!][!ENDCODE!][!//
    [!IF "$ControllerCfgCount > 0"!][!//
        [!CODE!]/* Controller connections */[!CR!][!ENDCODE!][!//
        [!CODE!]extern const Qspi_Ip_ControllerConfigType [!"$configName"!]_[!"$postBuildVariant"!]_paQspiUnitCfg[[!"$ControllerCfgCount"!]U];[!CR!][!ENDCODE!][!//
        [!CODE!][!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
    [!IF "$MemCfgCount > 0"!][!//
        [!CODE!]/* Memory configurations */[!CR!][!ENDCODE!][!//
        [!CODE!]extern const Qspi_Ip_MemoryConfigType [!"$configName"!]_[!"$postBuildVariant"!]_paFlashCfg[[!"$MemCfgCount"!]U];[!CR!][!ENDCODE!][!//
        [!CODE!][!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
    [!IF "$FlsMemCount > 0"!][!//
        [!CODE!]/* Memory-controller connections */[!CR!][!ENDCODE!][!//
        [!CODE!]extern const Qspi_Ip_MemoryConnectionType [!"$configName"!]_[!"$postBuildVariant"!]_paFlashConnectionCfg[[!"$FlsMemCount"!]U];[!CR!][!ENDCODE!][!//
        [!CODE!][!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ELSE!][!//
    [!CODE!][!CR!][!ENDCODE!][!//
    [!IF "$ControllerCfgCount > 0"!][!//
        [!CODE!]/* Controller connections */[!CR!][!ENDCODE!][!//
        [!CODE!]extern const Qspi_Ip_ControllerConfigType [!"$configName"!]_paQspiUnitCfg[[!"$ControllerCfgCount"!]U];[!CR!][!ENDCODE!][!//
        [!CODE!][!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
    [!IF "$MemCfgCount > 0"!][!//
        [!CODE!]/* Memory configurations */[!CR!][!ENDCODE!][!//
        [!CODE!]extern const Qspi_Ip_MemoryConfigType [!"$configName"!]_paFlashCfg[[!"$MemCfgCount"!]U];[!CR!][!ENDCODE!][!//
        [!CODE!][!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
    [!IF "$FlsMemCount > 0"!][!//
        [!CODE!]/* Memory-controller connections */[!CR!][!ENDCODE!][!//
        [!CODE!]extern const Qspi_Ip_MemoryConnectionType [!"$configName"!]_paFlashConnectionCfg[[!"$FlsMemCount"!]U];[!CR!][!ENDCODE!][!//
        [!CODE!][!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//

#define FLS_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fls_MemMap.h"

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/**@}*/

#endif    /* #ifndef QSPI_IP_[!"$variantName"!]PBCFG_H */
