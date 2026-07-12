[!CODE!][!//
[!AUTOSPACING!][!//
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

/**
 *   @file       Icu_PBCfg.c
 *   @version    2.0.1
 *
 *   @brief      AUTOSAR Icu - contains the data exported by the Icu module
 *   @details    Contains the information that will be exported by the module, as requested by Autosar.
 *
 *   @addtogroup icu Icu Driver
 *   @{
 */
 
#ifdef __cplusplus
extern "C"{
#endif

 /*==================================================================================================
 *                                         INCLUDE FILES
 * 1) system and project includes
 * 2) needed interfaces from external units
 * 3) internal and external interfaces from this unit
 *================================================================================================*/
#include "StandardTypes.h"
#include "Icu.h"
#include "Icu_Ipw_Types.h"
#include "Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h"

#if ((ICU_SIGNALMEASUREMENT_USES_DMA == STD_ON) || (ICU_TIMESTAMP_USES_DMA == STD_ON))
#include "CDD_Mcl.h"
#endif

[!NOCODE!]
[!INCLUDE "Icu_VersionCheck_Src_PB.m"!]
[!ENDNOCODE!][!//
/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 *================================================================================================*/
#define ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C                   43
#define ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C    4
#define ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C    4
#define ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C 0
#define ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C            2
#define ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C            0
#define ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C            1

/*==================================================================================================
 *                                      FILE VERSION CHECKS
 *================================================================================================*/
/* Check if header file and StandardTypes.h file are of the same Autosar version */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C != STD_AR_RELEASE_MAJOR_VERSION) || \
         (ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C != STD_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Icu_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and StandardTypes.h are different"
    #endif
    
    #if ((ICU_SIGNALMEASUREMENT_USES_DMA == STD_ON) || (ICU_TIMESTAMP_USES_DMA == STD_ON))
        #if ((ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C != CDD_MCL_AR_RELEASE_MAJOR_VERSION) || \
             (ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C != CDD_MCL_AR_RELEASE_MINOR_VERSION))
            #error "AutoSar Version Numbers of Icu_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and CDD_Mcl.h are different"
        #endif
    #endif    
#endif

/* Check if source file and ICU header file are of the same vendor */
#if (ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C != ICU_VENDOR_ID)
    #error "Icu_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Icu.h have different vendor IDs"
#endif
/* Check if source file and ICU header file are of the same AutoSar version */
#if ((ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C != ICU_AR_RELEASE_MAJOR_VERSION) || \
     (ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C != ICU_AR_RELEASE_MINOR_VERSION) || \
     (ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C != ICU_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Icu_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Icu.h are different"
#endif
/* Check if source file and ICU header file are of the same Software version */
#if ((ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C != ICU_SW_MAJOR_VERSION) || \
     (ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C != ICU_SW_MINOR_VERSION) || \
     (ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C != ICU_SW_PATCH_VERSION))
    #error "Software Version Numbers of Icu_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Icu.h are different"
#endif

/* Check if source file and ICU header file are of the same vendor */
#if (ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C != ICU_IPW_TYPES_VENDOR_ID)
    #error "Icu_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Icu_Ipw_Types.h have different vendor IDs"
#endif
/* Check if source file and ICU header file are of the same AutoSar version */
#if ((ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C != ICU_IPW_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C != ICU_IPW_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C != ICU_IPW_TYPES_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Icu_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Icu_Ipw_Types.h are different"
#endif
/* Check if source file and ICU header file are of the same Software version */
#if ((ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C != ICU_IPW_TYPES_SW_MAJOR_VERSION) || \
     (ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C != ICU_IPW_TYPES_SW_MINOR_VERSION) || \
     (ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C != ICU_IPW_TYPES_SW_PATCH_VERSION))
    #error "Software Version Numbers of Icu_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Icu_Ipw_Types.h are different"
#endif

/* Check if source file and ICU header file are of the same vendor */
#if (ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C != ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID)
    #error "Icu_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h have different vendor IDs"
#endif
/* Check if source file and ICU header file are of the same AutoSar version */
#if ((ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C != ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C != ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C != ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Icu_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h are different"
#endif
/* Check if source file and ICU header file are of the same Software version */
#if ((ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C != ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION) || \
     (ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C != ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION) || \
     (ICU_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C != ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Icu_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h are different"
#endif

/*==================================================================================================
 *                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 *================================================================================================*/

/*==================================================================================================
 *                                       LOCAL MACROS
 *================================================================================================*/

/*==================================================================================================
 *                                      LOCAL CONSTANTS
 *================================================================================================*/

/*==================================================================================================
 *                                       LOCAL VARIABLES
 *================================================================================================*/

/*==================================================================================================
 *                                       GLOBAL CONSTANTS
 *================================================================================================*/
[!INCLUDE "Icu_PluginMacros.m"!][!//
[!INCLUDE "Icu_Ipw_PluginMacros.m"!][!//

#define ICU_START_SEC_CODE
#include "Icu_MemMap.h"
[!CALL "ICU_GENERATE_NOTIFICATION_FUNCTIONS_PROTOTYPE"!]
#define ICU_STOP_SEC_CODE
#include "Icu_MemMap.h"

#define ICU_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Icu_MemMap.h"

[!NOCODE!][!//
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "PBCfg_Variant" = "concat('PB_', $postBuildVariant)"!]
[!ELSE!]
    [!VAR "PBCfg_Variant" = "'PB'"!]
[!ENDIF!]
[!ENDNOCODE!][!//

[!CALL "ICU_GENERATE_CHANNEL_INDEX_MAPPING","VARIANT"="$PBCfg_Variant"!]
[!CALL "ICU_GENERATE_CONFIGURATION","VARIANT"="$PBCfg_Variant"!]

[!FOR "PartitionIdx" = "1" TO "$IcuEcucPartitionRefNum"!]
    [!IF "$IcuEcucPartitionRefCount != 0 "!]
        [!VAR "PartitionIndex" = "node:name(node:value(IcuGeneral/IcuEcucPartitionRef/*[num:i($PartitionIdx)]))"!]
        [!VAR "IcuEcucPartitionRefName" = "node:value(IcuGeneral/IcuEcucPartitionRef/*[num:i($PartitionIdx)])"!]
    [!ENDIF!]
    
    [!SELECT "IcuConfigSet"!]
        [!CALL "ICU_VARIABLES"!]

[!IF "((../IMPLEMENTATION_CONFIG_VARIANT !='VariantPostBuild' and (variant:size()<=1)) or (../IMPLEMENTATION_CONFIG_VARIANT ='VariantPostBuild' and (variant:size() <= 0)))"!]
    [!IF "(../IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1)"!]
/*
*   @brief Pre-Compile Default Configuration for each Partition
*/
#ifdef ICU_PRECOMPILE_SUPPORT
    [!ENDIF!]
const Icu_ConfigType Icu_Config[!IF "$IcuEcucPartitionRefCount != 0 "!]_[!"$PartitionIndex"!][!ENDIF!] =
[!ELSE!]
/*
*   @brief Postbuild Configuration for each Partition
*/
const Icu_ConfigType Icu_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "$IcuEcucPartitionRefCount != 0 "!]_[!"$PartitionIndex"!][!ENDIF!] =
[!ENDIF!]
{
    (uint8)[!"num:i($OutNumChannelInPartition)"!], 
    /** @brief The number of channels configured*/
    &Icu_ChannelConfig_PB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "$IcuEcucPartitionRefCount != 0 "!]_P_[!"$PartitionIndex"!][!ENDIF!], 
    /** @brief Icu Channel Configuration Pointer */
    (uint8)[!"num:i($OutNumInstancesInPartition)"!], /* The number of IP instances configured*/
    /** @brief Icu Instance Configuration Pointer */
    &Icu_Ipw_IpConfig_PB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "$IcuEcucPartitionRefCount != 0 "!]_P_[!"$PartitionIndex"!][!ENDIF!],
    /** @brief Index of channel in each partition map table*/
    &Icu_ChIndexMap_PB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!IF "$IcuEcucPartitionRefCount != 0 "!]_P_[!"$PartitionIndex"!][!ENDIF!],
    /** @brief core index*/
    (uint8)[!"num:i($OutRespectiveCoreId)"!]U[!CR!]
};
     [!IF "(../IMPLEMENTATION_CONFIG_VARIANT !='VariantPostBuild' and (variant:size()<=1))"!]
#endif /*ICU_PRECOMPILE_SUPPORT*/
    [!ENDIF!]
    [!ENDSELECT!]
[!ENDFOR!]

[!IF "$IcuEcucPartitionRefCount != 0 "!]
[!CALL "ECUC_MAX_PARTITION_NUMBER"!]
[!SELECT "IcuConfigSet"!]
[!IF "(../IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1)"!]
#ifdef ICU_PRECOMPILE_SUPPORT
const Icu_ConfigType* const Icu_Config[ICU_MAX_PARTITIONS] =
{
[!NOCODE!]
[!FOR "CurrentCoreId" = "0" TO "num:i($maxPartition)"!]
    [!VAR "HasCore" = "0"!]
    [!VAR "HasPartition" = "0"!]
    [!VAR "foundPartition" = "0"!]  
    [!LOOP "as:modconf('Icu')[1]/IcuGeneral/IcuEcucPartitionRef/*"!]
        [!IF "num:i($foundPartition) = 0"!]
            [!VAR "IcuEcucPartitionRefName" = "node:value(.)"!]
            [!VAR "PartitionIndex" = "node:name(node:value(.))"!]
            [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
                [!IF "node:refvalid(./OsAppEcucPartitionRef)"!]
                    [!IF "$IcuEcucPartitionRefName = node:value(./OsAppEcucPartitionRef)"!]
                        [!IF "node:refvalid(./OsApplicationCoreRef)"!]
                            [!SELECT "node:ref(./OsApplicationCoreRef)"!]
                                [!VAR "OsCoreId" = "node:value(./EcucCoreId)"!]
                                [!IF "num:i($OsCoreId) = num:i($CurrentCoreId)"!]
                                    [!VAR "HasCore" = "1"!]
                                    [!LOOP "as:modconf('Icu')[1]/IcuConfigSet/IcuChannel/*"!]
                                        [!LOOP "./IcuChannelEcucPartitionRef/*"!]
                                            [!VAR "IcuChannelEcucPartitionRefName" = "node:value(.)"!]
                                            [!IF "$IcuChannelEcucPartitionRefName = $IcuEcucPartitionRefName"!]
                                                [!VAR "HasPartition" = "1"!]
                                                [!VAR "foundPartition" = "1"!]
                                            [!ENDIF!]
                                        [!ENDLOOP!]
                                    [!ENDLOOP!]
                                [!ENDIF!]
                            [!ENDSELECT!]
                        [!ENDIF!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ENDIF!]
    [!ENDLOOP!]
    
    [!IF "$HasCore = 1"!]
        [!IF "$HasPartition = 1"!]
            [!CODE!][!WS "4"!]/** @brief  Pointer to configuration structure of ICU for partition [!"$PartitionIndex"!] refered by core [!"num:i($CurrentCoreId)"!] */[!CR!][!ENDCODE!]
            [!CODE!][!WS "4"!]&Icu_Config_[!"$PartitionIndex"!][!CR!][!ENDCODE!]
        [!ELSE!]
            [!CODE!][!WS "4"!]/** @brief  No configuration structure of ICU refered by core [!"num:i($CurrentCoreId)"!] */[!CR!][!ENDCODE!]
            [!CODE!][!WS "4"!]NULL_PTR[!CR!][!ENDCODE!]
        [!ENDIF!]
    [!ELSE!]
        [!CODE!][!WS "4"!]/** @brief  No configuration structure of ICU refered by core [!"num:i($CurrentCoreId)"!] */[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!] NULL_PTR[!CR!][!ENDCODE!]
    [!ENDIF!]
    
    [!IF "$CurrentCoreId < num:i($maxPartition)"!][!CODE!],[!ENDCODE!][!ENDIF!][!CR!]
[!ENDFOR!]
[!ENDNOCODE!]
};
#endif /*ICU_PRECOMPILE_SUPPORT*/
[!ENDIF!]
[!ENDSELECT!]
[!ENDIF!]

#define ICU_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Icu_MemMap.h"
/*==================================================================================================
 *                                       GLOBAL VARIABLES
 *================================================================================================*/

/*==================================================================================================
 *                                   LOCAL FUNCTION PROTOTYPES
 *================================================================================================*/

/*==================================================================================================
 *                                       LOCAL FUNCTIONS
 *================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */
[!ENDCODE!]
