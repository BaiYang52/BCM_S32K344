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

#ifndef LIN_DEFINES_H
#define LIN_DEFINES_H

/**
*   @file
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
#include "Mcal.h"
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define LIN_DEFINES_VENDOR_ID                     43
#define LIN_DEFINES_AR_RELEASE_MAJOR_VERSION      4
#define LIN_DEFINES_AR_RELEASE_MINOR_VERSION      4
#define LIN_DEFINES_AR_RELEASE_REVISION_VERSION   0
#define LIN_DEFINES_SW_MAJOR_VERSION              2
#define LIN_DEFINES_SW_MINOR_VERSION              0
#define LIN_DEFINES_SW_PATCH_VERSION              1
/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Mcal.h */
    #if ((LIN_DEFINES_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (LIN_DEFINES_AR_RELEASE_MINOR_VERSION != MCAL_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AUTOSAR Version Numbers of Lin_Defines.h and Mcal.h are different"
    #endif
#endif

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/
[!NOCODE!]
[!// check for duplicated channel id, or hw channel id
    [!VAR "tmp_ch_id" = "-1"!]
    [!VAR "tmp_hw_ch" = "-1"!]
    [!VAR "tmp_channel_index" = "0"!]
    [!VAR "max_channel_nr" = "count(LinChannel/*)"!]
    [!VAR "LinMasterNodeUsed"= "'false'"!]

    [!LOOP "LinChannel/*"!]
        [!VAR "i" = "0"!]
        [!VAR "tmp_channel_index" = "$tmp_channel_index + 1"!]
        [!VAR "tmp_ch_id" = "LinChannelId"!]
        [!VAR "tmp_hw_ch" = "substring-after(LinHwChannel,'_')"!]
        [!FOR "i" = "$tmp_channel_index + 1" TO "$max_channel_nr"!]
            [!IF "$tmp_ch_id = ../*[num:i($i)]/LinChannelId"!]
                [!ERROR!] Duplicate LinChannelId.[!ENDERROR!]
            [!ENDIF!]
            [!IF "$tmp_hw_ch = substring-after(../*[num:i($i)]/LinHwChannel,'_')"!]
                [!ERROR!] Duplicate LinHwChannel IDs.[!ENDERROR!]
            [!ENDIF!]
        [!ENDFOR!]
    [!ENDLOOP!]

    [!LOOP "LinGlobalConfig/LinChannel/*"!][!//
        [!IF "contains(node:value(LinNodeType),'MASTER')"!]
            [!VAR "LinMasterNodeUsed"= "'true'"!]
        [!ENDIF!]
    [!ENDLOOP!][!//

[!// end check for duplicated channel id, or hw channel id
[!ENDNOCODE!][!//
/**
* @brief          Pre-compile Support.
*
*
*/
#define LIN_PRECOMPILE_SUPPORT [!IF "(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile' or IMPLEMENTATION_CONFIG_VARIANT='VariantLinkTime') and (variant:size() <= 1)"!] (STD_ON) [!ELSE!] (STD_OFF) [!ENDIF!]

/**
* @brief   No of Channels configured.
*
*
*/
#define LIN_HW_MAX_MODULES ([!"num:i(count(LinGlobalConfig/LinChannel/*))"!]U)

/**
* @brief   Total number of available hardware lin channels.
*
*
*/
#define LIN_HW_MAX_AVAILABLE_MODULES ([!"num:i(ecu:get('Lin.LinGlobalConfig.LinChannel'))"!]U)

/**
* @brief   Switches the Development Error Detection and Notification ON or OFF.
*
*
*/
#define LIN_DEV_ERROR_DETECT [!IF "LinGeneral/LinDevErrorDetect"!](STD_ON)  /* Enable Development Error Detection */[!ELSE!](STD_OFF)   /* Disable Development Error Detection */[!ENDIF!]

/**
* @brief   Switches the Production Error Detection and Notification OFF
*
*
*/
[!NOCODE!]
[!VAR "LinDemErrorEnable" = "'false'"!]
[!IF "AutosarExt/LinDisableDemReportErrorStatus ='false'"!][!//
    [!IF "node:exists(LinDemEventParameterRefs)"!]
        [!VAR "LinDemErrorEnable" = "'true'"!]
    [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!][!//
#define LIN_DISABLE_DEM_REPORT_ERROR_STATUS [!IF "$LinDemErrorEnable"!] (STD_OFF) /* Enable Production Error Detection */ [!ELSE!] (STD_ON) /* Disable Production Error Detection */ [!ENDIF!]

/**
* @brief          Lin Master Node Used
* @details        When LinGlobalConfig/LinChannel/LinNodeType contains at least one MASTER channel.
*
*
*/
#define LIN_MASTER_NODE_USED [!IF "$LinMasterNodeUsed = 'true'"!] (STD_ON) /* Used Master Node */[!ELSE!] (STD_OFF) /* Not Used Master Node */[!ENDIF!]

/**
* @brief          Support for version info API.
* @details        Switches the Lin_GetVersionInfo() API ON or OFF.
*
*
*/
#define LIN_VERSION_INFO_API [!IF "LinGeneral/LinVersionInfoApi"!](STD_ON)  /* Enable API Lin_GetVersionInfo      */[!ELSE!](STD_OFF) /* Disable API Lin_GetVersionInfo     */[!ENDIF!]

/**
* @brief        All CoreIDs are supported by LIN driver.
*/
[!NOCODE!]
[!VAR "MaxCoreId"="0"!]
[!IF "node:value(LinGeneral/LinMulticoreSupport)='true'"!]
    [!LOOP "as:modconf('EcuC')[1]/EcucHardware/*[1]/EcucCoreDefinition/*"!][!//
            [!IF "$MaxCoreId < node:value(./EcucCoreId)"!]
                [!VAR "MaxCoreId"="node:value(./EcucCoreId)"!]
            [!ENDIF!] 
    [!ENDLOOP!]
[!ENDIF!]
[!ENDNOCODE!]
#define LIN_MAX_PARTITIONS     ((uint32)[!"num:i($MaxCoreId+1)"!]U)

/**
* @brief          Multicore is enabled or not
*/
#define LIN_MULTICORE_SUPPORT   [!IF "node:fallback(as:modconf('Lin')[1]/LinGeneral/LinMulticoreSupport, 'false') = 'false'"!](STD_OFF) /* Multicore is disabled */[!ELSE!](STD_ON) /* Multicore is enabled */[!ENDIF!]

/**
* @brief          Enable Non-Autosar API for Dual-Clock support.
* @details        Enable/Disable API Lin_SetClockMode() to set the
*                 clock to be used by the LIN driver (Normal clock: default mode;
*                 Alternate clock: when the driver is in Low-Power mode).
*                 This can be set to STD_ON only if it is activated from xdm file:
*                 LinClockRef_Alternate is enable.
*
* @api
*/
[!NOCODE!]
[!VAR "DualClockEnabled"= "'false'"!]
[!LOOP "LinGlobalConfig/LinChannel/*"!]
    [!IF "(node:exists(LinClockRef_Alternate))"!]
        [!VAR "DualClockEnabled"= "'true'"!]
    [!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!]
#define LIN_DUAL_CLOCK_MODE [!IF "$DualClockEnabled = 'true'"!] (STD_ON) /* Enable API Lin_SetClockMode */ [!ELSE!] (STD_OFF) /* Disable API Lin_SetClockMode */ [!ENDIF!]

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define LIN_UNALLOCATEDPAR_CORE_ID  [!IF "as:modconf('Lin')[1]/LinGeneral/LinMulticoreSupport = 'true'"!]((uint32)0xFFU)[!ELSE!]((uint32)0x0U)[!ENDIF!]

#if (STD_ON == LIN_MULTICORE_SUPPORT)
    #define Lin_GetCoreID() (OsIf_GetCoreID())
#else
    #define Lin_GetCoreID() (LIN_UNALLOCATEDPAR_CORE_ID)
#endif /* (STD_ON == LIN_MULTICORE_SUPPORT) */

/**
* @brief          LIN driver status initialization variable.
*/
#define LIN_UNINIT_ARRAY  {[!FOR "idx" = "0" TO "$MaxCoreId"!]LIN_UNINIT[!IF "$idx < $MaxCoreId"!], [!ENDIF!][!ENDFOR!]}
[!NOCODE!]
[!VAR "HasFlexio" = "'STD_OFF'"!]
[!VAR "WakeupDetectionSupport" = "'STD_OFF'"!]
[!LOOP "LinGlobalConfig/LinChannel/*"!][!//
    [!IF "node:exists(LinFlexioTxControllerRef) and node:exists(LinFlexioRxControllerRef)"!]
        [!VAR "HasFlexio" = "'STD_ON'"!]
    [!ENDIF!]
    [!IF "node:exists(LinChannelWakeupSupport) and LinChannelWakeupSupport = 'true'"!]
        [!VAR "WakeupDetectionSupport" = "'STD_ON'"!]
    [!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!]
/**
*
* @internal
* @brief          Lin FlexIO Used
* @details        When LinGlobalConfig/LinChannel/LinHwChannel contains FLEXIO.
*
*
*/
#define LIN_FLEXIO_USED ([!"$HasFlexio"!]) /* [!IF "$HasFlexio = 'STD_ON'"!]Flexio Used[!ELSE!]No Used Flexio[!ENDIF!] */
/**
*
* @internal
* @brief          Lin Wakeup detection feature 
* @details        It is set to STD_ON when LinGlobalConfig/LinChannel/WakeupDetectionSupport is enabled.
*
*
*/
#define LIN_WAKEUP_DETECTION ([!"$WakeupDetectionSupport"!]) /* Support wakeup detection */
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

/*==================================================================================================
*                                  EXTERNAL CONSTANTS
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* LIN_DEFINES_H */
