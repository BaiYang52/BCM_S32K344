/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : LPUART_FLEXIO
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
*   @file    Lin_PBcfg.c
*   @implements Lin_PBcfg.c_Artifact
*
*   @defgroup lin_driver LIN Driver
*   @addtogroup  lin_driver LIN Driver
*   @{
*/

#ifdef __cplusplus
extern "C"
{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "StandardTypes.h"
#include "Lin_Cfg.h"
#include "Lin_Ipw_Cfg.h"

#if (STD_OFF == LIN_DISABLE_DEM_REPORT_ERROR_STATUS)
#include "Dem.h"
#endif /* STD_OFF == LIN_DISABLE_DEM_REPORT_ERROR_STATUS */

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_VENDOR_ID_C                     43
#define LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C      4
#define LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MINOR_VERSION_C      4
#define LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_REVISION_VERSION_C   0
#define LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MAJOR_VERSION_C              2
#define LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MINOR_VERSION_C              0
#define LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_PATCH_VERSION_C              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Lin_Cfg.h */
#if (LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_VENDOR_ID_C != LIN_CFG_VENDOR_ID)
    #error "Lin[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lin_Cfg.h have different vendor ids"
#endif
#if ((LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C    != LIN_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MINOR_VERSION_C    != LIN_CFG_AR_RELEASE_MINOR_VERSION) || \
     (LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_REVISION_VERSION_C != LIN_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Lin[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lin_Cfg.h are different"
#endif
#if ((LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MAJOR_VERSION_C != LIN_CFG_SW_MAJOR_VERSION) || \
     (LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MINOR_VERSION_C != LIN_CFG_SW_MINOR_VERSION) || \
     (LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_PATCH_VERSION_C != LIN_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Lin[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lin_Cfg.h are different"
#endif

/* Checks against Lin_Ipw_Cfg.h */
#if (LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_VENDOR_ID_C != LIN_IPW_CFG_VENDOR_ID)
    #error "Lin[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lin_Ipw_Cfg.h have different vendor ids"
#endif
#if ((LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C    != LIN_IPW_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MINOR_VERSION_C    != LIN_IPW_CFG_AR_RELEASE_MINOR_VERSION) || \
     (LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_REVISION_VERSION_C != LIN_IPW_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Lin[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lin_Ipw_Cfg.h are different"
#endif
#if ((LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MAJOR_VERSION_C != LIN_IPW_CFG_SW_MAJOR_VERSION) || \
     (LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MINOR_VERSION_C != LIN_IPW_CFG_SW_MINOR_VERSION) || \
     (LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_PATCH_VERSION_C != LIN_IPW_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Lin[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lin_Ipw_Cfg.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if (STD_OFF == LIN_DISABLE_DEM_REPORT_ERROR_STATUS)
        /* Checks against Dem.h */
        #if ((LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C != DEM_AR_RELEASE_MAJOR_VERSION) || \
             (LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MINOR_VERSION_C != DEM_AR_RELEASE_MINOR_VERSION) \
            )
            #error "AUTOSAR Version Numbers of Lin[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Dem.h are different"
        #endif
    #endif
    /* Check if current file and StandardTypes.h header file are of the same Autosar version */
    #if ((LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C != STD_AR_RELEASE_MAJOR_VERSION) || \
         (LIN[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MINOR_VERSION_C != STD_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AutoSar Version Numbers of Lin[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and StandardTypes.h are different"
    #endif
#endif

/*==================================================================================================
*                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
#define LIN_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Lin_MemMap.h"

/**
* @brief   Export IPW configurations.
*/
LIN_IPW_CONFIG_EXT

#define LIN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Lin_MemMap.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/
/**
* @brief        All CoreIDs are supported by LIN driver.
*/
[!NOCODE!]
[!LOOP "./LinGeneral/LinEcucPartitionRef/*"!][!//
    [!VAR "PartitionRef"="node:value(.)"!][!//
    [!VAR "PartitionName"="count(text:split($PartitionRef,'/'))"!]
    [!VAR "PartitionName"="text:split($PartitionRef,'/')[num:i($PartitionName)]"!]
    [!VAR "PartitionName"="text:toupper($PartitionName)"!]
    [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
        [!IF "$PartitionRef = node:value(./OsAppEcucPartitionRef)"!][!//
            [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
                [!CODE!]
#define LIN_[!"$PartitionName"!]_CORE_ID     ((uint32)[!"node:value(./EcucCoreId)"!]U)
                [!ENDCODE!]
            [!ENDSELECT!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDLOOP!][!//
[!ENDNOCODE!]
/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/
#define LIN_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Lin_MemMap.h"

[!NOCODE!]
[!VAR "DualClockEnabled"= "'false'"!]
[!LOOP "LinGlobalConfig/LinChannel/*"!]
    [!IF "(node:exists(LinClockRef_Alternate))"!]
        [!VAR "DualClockEnabled"= "'true'"!]
    [!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!]

[!NOCODE!]
[!VAR "NodeName" = "as:name(./LinGlobalConfig)"!][!//
[!LOOP "LinGlobalConfig/LinChannel/*"!][!//
    [!IF "node:exists(./LinChannelEcucPartitionRef)"!]
        [!VAR "PartitionName"="count(text:split(./LinChannelEcucPartitionRef,'/'))"!]
        [!VAR "PartitionName"="text:split(./LinChannelEcucPartitionRef,'/')[num:i($PartitionName)]"!]
    [!ELSE!]
        [!VAR "PartitionName"="'UnAllocatedPar'"!]
    [!ENDIF!]

[!CODE!]
/**
* @brief          Configuration for Lin Channel [!"LinChannelId"!] - Configuration:
*                 [!"$NodeName"!].
*
* 
*/
const Lin_ChannelConfigType Lin_[!"node:name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_[!"$PartitionName"!] =
{
    (uint8)[!"LinChannelId"!]U, /* Lin Channel ID */
[!ENDCODE!]
[!CODE!] 
    &Lin_Ipw_pHwConfigPB_[!"LinChannelId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!], /* Pointer to the hardware configuration*/

    LIN_[!"text:toupper($PartitionName)"!]_CORE_ID,         /* Partition core id of channel */

[!IF "as:modconf('Lin')[1]/LinGeneral/LinMulticoreSupport = 'true'"!][!//
    (boolean)[!IF "$PartitionName = 'UnAllocatedPar'"!]FALSE[!ELSE!]TRUE[!ENDIF!]
[!ELSE!][!//
    (boolean)TRUE[!//
[!ENDIF!][!//
[!ENDCODE!]
[!CODE!]
};
[!ENDCODE!]
[!ENDLOOP!][!//
[!ENDNOCODE!]

[!NOCODE!][!//
[!VAR "NodeName" = "as:name(./LinGlobalConfig)"!][!//
[!IF "as:modconf('Lin')[1]/LinGeneral/LinMulticoreSupport = 'true'"!]
    [!VAR "LinNumOfPars"="count(LinGeneral/LinEcucPartitionRef/*)"!]
[!ELSE!]
    [!VAR "LinNumOfPars"="1"!]
[!ENDIF!]
[!FOR "LinCountPars"="0" TO "$LinNumOfPars - 1"!]
    [!IF "as:modconf('Lin')[1]/LinGeneral/LinMulticoreSupport = 'true'"!]
        [!VAR "PartitionRef"="LinGeneral/LinEcucPartitionRef/*[number($LinCountPars + 1)]"!]
        [!VAR "PartitionName"="count(text:split($PartitionRef,'/'))"!]
        [!VAR "PartitionName"="text:split($PartitionRef,'/')[num:i($PartitionName)]"!]
        [!VAR "PartitionNameUnderScore"="concat('_',$PartitionName)"!]
        [!VAR "NumOfChannelInPar"="count(LinGlobalConfig/LinChannel/*[contains(./LinChannelEcucPartitionRef,$PartitionRef)])"!]
    [!ELSE!]
        [!VAR "PartitionName"="'UnAllocatedPar'"!]
        [!VAR "PartitionNameUnderScore"="string(null)"!]
        [!VAR "NumOfChannelInPar"="1"!]
    [!ENDIF!]
    [!IF "$NumOfChannelInPar > 0"!]
[!CODE!]
/**
* @brief          Lin Configuration data for [!"$NodeName"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!"$PartitionNameUnderScore"!].
*
*/
[!IF "(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)"!][!//
static const Lin_ConfigType Lin_Config[!"$PartitionNameUnderScore"!] =
[!ELSE!][!//
const Lin_ConfigType Lin_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!"$PartitionNameUnderScore"!] =
[!ENDIF!][!//
{
    LIN_[!"text:toupper($PartitionName)"!]_CORE_ID,            /** Core Id */

    {[!//
[!ENDCODE!][!//
    [!VAR "Count"="0"!]
    [!VAR "MaxChannelNr" = "num:i(count(LinGlobalConfig/LinChannel/*))"!]
    [!FOR "Count" = "0" TO "$MaxChannelNr - 1"!]
        [!LOOP "LinGlobalConfig/LinChannel/*"!]
            [!IF "$Count = num:i(LinChannelId)"!]
                 [!IF "node:exists(./LinChannelEcucPartitionRef)"!]
                    [!VAR "ChPartitionName"="count(text:split(./LinChannelEcucPartitionRef,'/'))"!]
                    [!VAR "ChPartitionName"="text:split(./LinChannelEcucPartitionRef,'/')[num:i($ChPartitionName)]"!]
                [!ELSE!]
                    [!VAR "ChPartitionName"="'UnAllocatedPar'"!]
                [!ENDIF!]
[!CODE!]
        /**
        * @brief   Configuration for Lin Channel [!"LinChannelId"!].
        */
        &Lin_[!"node:name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_[!"$ChPartitionName"!][!IF "num:i($Count + 1) < $MaxChannelNr"!],[!ENDIF!][!//
[!ENDCODE!][!//
            [!ENDIF!]
        [!ENDLOOP!]
    [!ENDFOR!]
[!CODE!]
    }
};[!ENDCODE!]
    [!ENDIF!][!//$NumOfChannelInPar > 0
[!ENDFOR!]
[!ENDNOCODE!]
[!NOCODE!]
[!IF "as:modconf('Lin')[1]/LinGeneral/LinMulticoreSupport = 'true'"!]
    [!VAR "MaxCoreId"="0"!]
    [!LOOP "as:modconf('EcuC')[1]/EcucHardware/*[1]/EcucCoreDefinition/*"!][!//
            [!IF "$MaxCoreId < node:value(./EcucCoreId)"!]
                [!VAR "MaxCoreId"="node:value(./EcucCoreId)"!]
            [!ENDIF!]  
    [!ENDLOOP!][!// 
[!ENDIF!]
[!ENDNOCODE!]
[!NOCODE!]
[!IF "(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)"!][!//
[!CODE!][!//
/**
* @brief          Lin Configuration data for Lin_PBCfgVariantPredefined[LIN_MAX_PARTITIONS].
*
*/
const Lin_ConfigType * const Lin_PBCfgVariantPredefined[LIN_MAX_PARTITIONS] =
{
[!ENDCODE!][!//
[!IF "as:modconf('Lin')[1]/LinGeneral/LinMulticoreSupport = 'true'"!]
    [!VAR "LinNumOfPars"="count(LinGeneral/LinEcucPartitionRef/*)"!]
    [!VAR "FirstCore"="1"!]
    [!FOR "LinCountCoreId"="0" TO "$MaxCoreId"!]
        [!VAR "TempCoreIdUpdated"="'false'"!]
        [!LOOP "./LinGeneral/LinEcucPartitionRef/*"!][!//
            [!VAR "PartitionRef"="node:value(.)"!]
            [!VAR "PartitionName"="count(text:split($PartitionRef,'/'))"!]
            [!VAR "PartitionName"="text:split($PartitionRef,'/')[num:i($PartitionName)]"!]
            [!VAR "NumOfChannelInPar"="count(../../../LinGlobalConfig/LinChannel/*[contains(./LinChannelEcucPartitionRef,$PartitionRef)])"!]
            [!IF "$NumOfChannelInPar > 0"!]
                [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
                    [!IF "$PartitionRef = node:value(./OsAppEcucPartitionRef)"!][!//
                        [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
                            [!IF "($LinCountCoreId = ./EcucCoreId)"!]
                                [!VAR "TempCoreIdUpdated"="'true'"!]
                                [!VAR "PartitionNameUpdated"="$PartitionName"!]
                            [!ENDIF!]
                        [!ENDSELECT!]
                    [!ENDIF!]
                [!ENDLOOP!]
            [!ENDIF!]
        [!ENDLOOP!]
        [!CODE!]
    [!IF "$FirstCore = 0"!],[!ENDIF!][!VAR "FirstCore"="0"!]
        [!ENDCODE!]
        [!IF "$TempCoreIdUpdated = 'true'"!]
            [!CODE!]
    &Lin_Config_[!"$PartitionNameUpdated"!]
            [!ENDCODE!]
        [!ELSE!]
            [!CODE!]
    NULL_PTR
            [!ENDCODE!]
        [!ENDIF!]
    [!ENDFOR!]
[!ELSE!][!//LinGeneral/LinMulticoreSupport = 'false'
    [!CODE!]
    &Lin_Config
    [!ENDCODE!]
[!ENDIF!][!//LinGeneral/LinMulticoreSupport = 'true'
[!CODE!][!//
};
[!ENDCODE!]
[!ENDIF!][!//(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)
[!ENDNOCODE!]
#define LIN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Lin_MemMap.h"

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */
