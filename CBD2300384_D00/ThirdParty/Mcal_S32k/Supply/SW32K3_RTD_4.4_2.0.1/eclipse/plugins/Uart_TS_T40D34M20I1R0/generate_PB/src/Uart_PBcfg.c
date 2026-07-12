/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : FLEXIO
*   Dependencies         : 
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
*   @file
*
*   @defgroup uart_driver UART Driver
*   @addtogroup  uart_driver UART Driver
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

#include "StandardTypes.h"
#include "Uart_Cfg.h"
#include "Uart_Ipw_Cfg.h"
#include "Uart_Defines.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C                     43
#define UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C      4
#define UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C      4
#define UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C   0
#define UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C              2
#define UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C              0
#define UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Uart_Cfg.h */
#if (UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != UART_CFG_VENDOR_ID)
    #error "Uart[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Uart_Cfg.h have different vendor ids"
#endif
#if ((UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != UART_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != UART_CFG_AR_RELEASE_MINOR_VERSION) || \
     (UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != UART_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Uart[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Uart_Cfg.h are different"
#endif
#if ((UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != UART_CFG_SW_MAJOR_VERSION) || \
     (UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != UART_CFG_SW_MINOR_VERSION) || \
     (UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != UART_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Uart[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Uart_Cfg.h are different"
#endif

/* Checks against Uart_Ipw_Cfg.h */
#if (UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != UART_IPW_CFG_VENDOR_ID)
    #error "Uart[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Uart_Ipw_Cfg.h have different vendor ids"
#endif
#if ((UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != UART_IPW_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != UART_IPW_CFG_AR_RELEASE_MINOR_VERSION) || \
     (UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != UART_IPW_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Uart[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Uart_Ipw_Cfg.h are different"
#endif
#if ((UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != UART_IPW_CFG_SW_MAJOR_VERSION) || \
     (UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != UART_IPW_CFG_SW_MINOR_VERSION) || \
     (UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != UART_IPW_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Uart[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Uart_Ipw_Cfg.h are different"
#endif

/* Checks against Uart_Defines.h */
#if (UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != UART_DEFINES_VENDOR_ID_CFG)
    #error "Uart[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Uart_Defines.h have different vendor ids"
#endif
#if ((UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != UART_DEFINES_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != UART_DEFINES_AR_RELEASE_MINOR_VERSION_CFG) || \
     (UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != UART_DEFINES_AR_RELEASE_REVISION_VERSION_CFG))
    #error "AUTOSAR Version Numbers of Uart[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Uart_Defines.h are different"
#endif
#if ((UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != UART_DEFINES_SW_MAJOR_VERSION_CFG) || \
     (UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != UART_DEFINES_SW_MINOR_VERSION_CFG) || \
     (UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != UART_DEFINES_SW_PATCH_VERSION_CFG) \
    )
    #error "Software Version Numbers of Uart[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Uart_Defines.h are different"
#endif
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK

/* Check if current file and StandardTypes.h header file are of the same Autosar version */
    #if ((UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C != STD_AR_RELEASE_MAJOR_VERSION) || \
            (UART_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C != STD_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Uart[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and StandardTypes.h are different"
    #endif
#endif

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
#define UART_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Uart_MemMap.h"

/**
* @brief   Export IPW configurations.
*/
UART_IPW_CONFIG_EXT

#define UART_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Uart_MemMap.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/
[!NOCODE!]
[!IF "as:modconf('Uart')[1]/GeneralConfiguration/UartMulticoreSupport = 'true'"!]
    [!VAR "MaxCoreId"="0"!]
    [!LOOP "as:modconf('EcuC')[1]/EcucHardware/*[1]/EcucCoreDefinition/*"!][!//
            [!IF "$MaxCoreId < node:value(./EcucCoreId)"!]
                [!VAR "MaxCoreId"="node:value(./EcucCoreId)"!]
            [!ENDIF!] 
    [!ENDLOOP!][!//
[!ENDIF!]

[!VAR "PartitionName"="''"!]
[!VAR "UartConfiguredPartitions" = "num:i((count(GeneralConfiguration/UartEcucPartitionRef/*)))"!][!//
[!IF "$UartConfiguredPartitions > 0"!][!//

    [!LOOP "GeneralConfiguration/UartEcucPartitionRef/*"!][!//
        [!VAR "PartitionRef"="node:value(.)"!][!//
        [!VAR "PartitionName"="count(text:split($PartitionRef,'/'))"!]
        [!VAR "PartitionName"="text:split($PartitionRef,'/')[num:i($PartitionName)]"!]
        [!VAR "PartitionNameUpper"="text:toupper($PartitionName)"!]
        [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
            [!IF "$PartitionRef = node:value(./OsAppEcucPartitionRef)"!][!//
                [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
                    [!CODE!]
#define UART_[!"$PartitionNameUpper"!]_CORE_ID     ((uint32)[!"node:value(./EcucCoreId)"!]U)
                    [!ENDCODE!]
                [!ENDSELECT!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
    [!ENDLOOP!][!//
[!ELSE!]
[!CODE!]
#define UART_[!"$PartitionName"!]CORE_ID     ((uint32)0U)
[!ENDCODE!]

[!ENDIF!]

[!ENDNOCODE!]

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/

#define UART_START_SEC_CONFIG_DATA_UNSPECIFIED
/**
* @violates @ref Uart[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg_c_REF_1 <MA>_MemMap.h is included
                 after each section define in order to set the current memory section as defined by AUTOSAR.
* @violates @ref Uart[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg_c_REF_2 This violation is not fixed
                  since the inclusion of <MA>_MemMap.h is as per AUTOSAR requirement [SWS_MemMap_00003].
*/
#include "Uart_MemMap.h"

[!NOCODE!]
[!LOOP "UartGlobalConfig/UartChannel/*"!][!//

[!IF "as:modconf('Uart')[1]/GeneralConfiguration/UartMulticoreSupport = 'true'"!]
    [!VAR "PartitionRef"="node:value(./UartChannelEcucPartitionRef)"!][!//
    [!VAR "PartitionName"="count(text:split($PartitionRef,'/'))"!]
    [!VAR "PartitionName"="text:split($PartitionRef,'/')[num:i($PartitionName)]"!]
    [!VAR "PartitionNameUpper"="text:toupper($PartitionName)"!]
    [!VAR "PartitionNameUpper" = "concat('_',$PartitionNameUpper)"!][!//

[!ELSE!]
    [!VAR "PartitionNameUpper"="''"!]
[!ENDIF!]

[!SELECT "node:ref(UartClockRef)"!][!//
[!VAR "ClockFrequencyFromMcu" = "McuClockReferencePointFrequency"!]
[!ENDSELECT!][!//

[!CODE!]
/**
* @brief          Configuration for Uart Channel [!"UartChannelId"!]
*
*
*/
const Uart_ChannelConfigType Uart_ChannelConfig_[!"UartChannelId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
[!ENDCODE!][!//
[!CODE!]    [!"UartChannelId"!], /*Uart Channel Id*/[!CR!][!ENDCODE!]
[!CODE!]
#if (UART_MULTICORE_SUPPORT == STD_ON)
    UART[!"$PartitionNameUpper"!]_CORE_ID, /*Uart Channel Core Id*/
#endif
[!ENDCODE!]
[!CODE!]    [!"num:i($ClockFrequencyFromMcu)"!]U, /* Clock frequency */[!CR!][!ENDCODE!]
[!CODE!]    &Uart_Ipw_xHwConfigPB_[!"UartChannelId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!] [!ENDIF!] /* Uart Hardware config */
};
[!ENDCODE!][!//
[!ENDLOOP!][!//
[!ENDNOCODE!]

[!VAR "PartitionName"="''"!]
[!VAR "UartConfiguredPartitions" = "num:i((count(GeneralConfiguration/UartEcucPartitionRef/*)))"!][!//
[!IF "$UartConfiguredPartitions > 0"!][!//
    [!LOOP "GeneralConfiguration/UartEcucPartitionRef/*"!][!//
        [!VAR "PartitionRef"="node:value(.)"!][!//
        [!VAR "PartitionName"="count(text:split($PartitionRef,'/'))"!]
        [!VAR "PartitionName"="text:split($PartitionRef,'/')[num:i($PartitionName)]"!]
        [!VAR "PartitionNameUpper"="text:toupper($PartitionName)"!]
        [!IF "not(../../../IMPLEMENTATION_CONFIG_VARIANT !='VariantPostBuild' and (variant:size()<=1))"!][!//
            [!IF "var:defined('postBuildVariant')"!][!//
                [!VAR "PostBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!][!//
            [!ELSE!][!//
                [!VAR "PostBuildVariantNameUnderscore" = "string(null)"!][!//
            [!ENDIF!][!//
        [!ENDIF!][!//

        [!VAR "FinalPartitionName" = "''"!]
        [!VAR "FinalPartitionName" = "concat('_',$PartitionName)"!][!//
        [!IF "(../../../IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)"!]
[!CODE!]
/**
* @brief          Uart Configuration data for [!"$PartitionName"!]
*
*
*/
static const Uart_ConfigType Uart_xConfig[!"$FinalPartitionName"!] =
{
[!ENDCODE!][!//
        [!ELSE!][!//
[!CODE!]
/**
* @brief          Uart Configuration data for [!"$PartitionName"!]
*
*
*/
const Uart_ConfigType Uart_xConfig[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!"$FinalPartitionName"!] =
{
[!ENDCODE!][!//
        [!ENDIF!][!//
[!CODE!][!//

#if (UART_MULTICORE_SUPPORT == STD_ON)
    UART[!"text:toupper($FinalPartitionName)"!]_CORE_ID,
#endif
    {
[!ENDCODE!][!//
[!NOCODE!]
        [!VAR "MaxChannelNr" = "num:i(count(../../../UartGlobalConfig/UartChannel/*))"!]
        [!VAR "Idx" = "$MaxChannelNr"!][!//
        [!LOOP "../../../UartGlobalConfig/UartChannel/*"!]
            [!IF "as:modconf('Uart')[1]/GeneralConfiguration/UartMulticoreSupport = 'true'"!]
                [!VAR "PartitionChannelRef"="node:value(./UartChannelEcucPartitionRef)"!][!//
                [!VAR "PartitionNameChannel"="count(text:split($PartitionChannelRef,'/'))"!]
                [!VAR "PartitionNameChannel"="text:split($PartitionChannelRef,'/')[num:i($PartitionNameChannel)]"!]
                [!IF "$PartitionName=$PartitionNameChannel"!]

[!CODE!]    &Uart_ChannelConfig_[!"UartChannelId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ENDCODE!][!//
                [!ELSE!][!//
[!CODE!]    NULL_PTR[!ENDCODE!][!//
                [!ENDIF!][!//
            [!ENDIF!][!//
            [!VAR "Idx" = "$Idx - 1"!][!//
            [!IF "$Idx > 0"!][!//
                [!CODE!],[!ENDCODE!][!CR!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
[!ENDNOCODE!][!//
[!CODE!][!//
[!CR!][!//
    }
};
[!ENDCODE!]
    [!ENDLOOP!]
[!ELSE!]
    [!IF "not(IMPLEMENTATION_CONFIG_VARIANT !='VariantPostBuild' and (variant:size()<=1))"!][!//
        [!IF "var:defined('postBuildVariant')"!][!//
            [!VAR "PostBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!][!//
        [!ELSE!][!//
            [!VAR "PostBuildVariantNameUnderscore" = "string(null)"!][!//
        [!ENDIF!][!//
    [!ENDIF!][!//
    [!IF "(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)"!][!//
[!CODE!]
/**
* @brief          Configuration for Uart driver
*
*
*/
static const Uart_ConfigType Uart_xConfig =
{
[!ENDCODE!]
    [!ELSE!][!//
[!CODE!]
/**
* @brief          Configuration for Uart driver
*
*
*/
const Uart_ConfigType Uart_xConfig[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
[!ENDCODE!]
    [!ENDIF!][!//
[!CODE!]
#if (UART_MULTICORE_SUPPORT == STD_ON)
    UART_CORE_ID,
#endif
    {
[!ENDCODE!]
[!NOCODE!]
    [!VAR "MaxChannelNr" = "num:i(count(UartGlobalConfig/UartChannel/*))"!][!//
    [!VAR "Idx" = "0"!][!//
    [!LOOP "UartGlobalConfig/UartChannel/*"!]
[!CODE!]
        &Uart_ChannelConfig_[!"UartChannelId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
[!ENDCODE!]
        [!IF "$Idx < $MaxChannelNr - 1"!]
            [!CODE!]
    ,
            [!ENDCODE!][!//
        [!ENDIF!][!//
        [!VAR "Idx" = "$Idx + 1"!][!//
    [!ENDLOOP!]
[!ENDNOCODE!]
[!CODE!]
[!CR!]
    }
};
[!ENDCODE!]
[!ENDIF!]
[!NOCODE!]

[!IF "(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)"!][!//
[!CODE!]
/**
* @brief          Uart Configuration data for Uart_pxPBcfgVariantPredefined.
*
*/
/**
* @violates @ref Uart[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg_c_REF_3 Function with external linkage is not visible.
* @violates @ref Uart[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg_c_REF_4 These functions
                 represent the API of the driver. External linkage is needed to "export" the API.
* @violates @ref Uart[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg_c_REF_5 This violation is due to exporting
                 the externally configured initialization structure used by the "<MA>_Init" function.
*/
const Uart_ConfigType* const Uart_pxPBcfgVariantPredefined[UART_MAX_PARTITIONS] =
{
[!ENDCODE!]
[!IF "as:modconf('Uart')[1]/GeneralConfiguration/UartMulticoreSupport = 'true'"!]
    [!VAR "UartNumOfPars"="count(GeneralConfiguration/UartEcucPartitionRef/*)"!]

    [!VAR "FirstCore"="1"!]
    [!FOR "UartCountCoreId"="0" TO "$MaxCoreId"!]
        [!VAR "TempCoreIdUpdated"="'false'"!]
        [!LOOP "GeneralConfiguration/UartEcucPartitionRef/*"!][!//
            [!VAR "PartitionRef"="node:value(.)"!]
            [!VAR "PartitionName"="count(text:split($PartitionRef,'/'))"!]
            [!VAR "PartitionName"="text:split($PartitionRef,'/')[num:i($PartitionName)]"!]
            [!VAR "NumOfChannelInPar"="count(../../../UartGlobalConfig/UartChannel/*[contains(./UartChannelEcucPartitionRef,$PartitionRef)])"!]

            [!IF "$NumOfChannelInPar > 0"!]

                [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//

                    [!IF "$PartitionRef = node:value(./OsAppEcucPartitionRef)"!][!//
                        [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//

                            [!IF "($UartCountCoreId = ./EcucCoreId)"!]
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
    &Uart_xConfig_[!"$PartitionNameUpdated"!]
[!ENDCODE!]
        [!ELSE!]
[!CODE!]
    NULL_PTR
[!ENDCODE!]
        [!ENDIF!]
    [!ENDFOR!]
[!ELSE!][!//UartGeneral/UartMulticoreSupport = 'false'
    [!CODE!]
    &Uart_xConfig
    [!ENDCODE!]
[!ENDIF!][!//UartGeneral/UartMulticoreSupport = 'true'
[!CODE!][!//
};
[!ENDCODE!]
[!ENDIF!][!//(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)
[!ENDNOCODE!]

#define UART_STOP_SEC_CONFIG_DATA_UNSPECIFIED
/**
* @violates @ref Uart[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg_c_REF_1 <MA>_MemMap.h is included
                 after each section define in order to set the current memory section as defined by AUTOSAR.
* @violates @ref Uart[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg_c_REF_2 This violation is not fixed
                  since the inclusion of <MA>_MemMap.h is as per AUTOSAR requirement [SWS_MemMap_00003].
*/
#include "Uart_MemMap.h"

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

/** @} */

#endif
