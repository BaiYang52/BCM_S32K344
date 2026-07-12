[!AUTOSPACING!]
[!NOCODE!]
    [!IF "var:defined('postBuildVariant')"!]
        [!VAR "VS_number" = "concat('_',$postBuildVariant)"!]
    [!ELSE!]
        [!VAR "VS_number" = "''"!]
    [!ENDIF!]
[!ENDNOCODE!]
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : eMios
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

#ifndef OCU_IPW[!"$VS_number"!]_PBCFG_H
#define OCU_IPW[!"$VS_number"!]_PBCFG_H

/**
*   @file           Ocu_Ipw[!"$VS_number"!]_PBcfg.h
*
*   @internal
*   @addtogroup     ocu_ipw Ocu IPW
*   @brief          Ocu IPW Postbuild configuration header file.
*   @details
*
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*===============================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
===============================================================================================*/

#include "Ocu_Ipw_Types.h"
#include "Emios_Ocu_Ip[!"$VS_number"!]_PBcfg.h"

/*==================================================================================================
*                               HEADER FILE VERSION INFORMATION
==================================================================================================*/
#define OCU_IPW[!"$VS_number"!]_PBCFG_VENDOR_ID                       43
#define OCU_IPW[!"$VS_number"!]_PBCFG_AR_RELEASE_MAJOR_VERSION        4
#define OCU_IPW[!"$VS_number"!]_PBCFG_AR_RELEASE_MINOR_VERSION        4
#define OCU_IPW[!"$VS_number"!]_PBCFG_AR_RELEASE_REVISION_VERSION     0
#define OCU_IPW[!"$VS_number"!]_PBCFG_SW_MAJOR_VERSION                2
#define OCU_IPW[!"$VS_number"!]_PBCFG_SW_MINOR_VERSION                0
#define OCU_IPW[!"$VS_number"!]_PBCFG_SW_PATCH_VERSION                1
/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/
#if (OCU_IPW[!"$VS_number"!]_PBCFG_VENDOR_ID != EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_VENDOR_ID)
    #error "Ocu_Ipw[!"$VS_number"!]_PBcfg.h and Emios_Ocu_Ip[!"$VS_number"!]_PBcfg.h have different vendor ids"
#endif

/* Check if  header file and Emios_Ocu_Ip[!"$VS_number"!]_PBcfg.h file are of the same Autosar version */
#if ((OCU_IPW[!"$VS_number"!]_PBCFG_AR_RELEASE_MAJOR_VERSION != EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (OCU_IPW[!"$VS_number"!]_PBCFG_AR_RELEASE_MINOR_VERSION != EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (OCU_IPW[!"$VS_number"!]_PBCFG_AR_RELEASE_REVISION_VERSION != EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Ocu_Ipw[!"$VS_number"!]_PBcfg.h and Emios_Ocu_Ip[!"$VS_number"!]_PBcfg.h are different"
#endif

/* Check if header file and Emios_Ocu_Ip[!"$VS_number"!]_PBcfg.h file are of the same Software version */
#if ((OCU_IPW[!"$VS_number"!]_PBCFG_SW_MAJOR_VERSION != EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_SW_MAJOR_VERSION) || \
     (OCU_IPW[!"$VS_number"!]_PBCFG_SW_MINOR_VERSION != EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_SW_MINOR_VERSION) || \
     (OCU_IPW[!"$VS_number"!]_PBCFG_SW_PATCH_VERSION != EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Ocu_Ipw[!"$VS_number"!]_PBcfg.h and Emios_Ocu_Ip[!"$VS_number"!]_PBcfg.h are different"
#endif

#if (OCU_IPW[!"$VS_number"!]_PBCFG_VENDOR_ID != OCU_IPW_TYPES_VENDOR_ID)
    #error "Ocu_Ipw[!"$VS_number"!]_PBcfg.h and Ocu_Ipw_Types.h have different vendor ids"
#endif

/* Check if  header file and Ocu_Ipw_Types.h file are of the same Autosar version */
#if ((OCU_IPW[!"$VS_number"!]_PBCFG_AR_RELEASE_MAJOR_VERSION != OCU_IPW_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (OCU_IPW[!"$VS_number"!]_PBCFG_AR_RELEASE_MINOR_VERSION != OCU_IPW_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (OCU_IPW[!"$VS_number"!]_PBCFG_AR_RELEASE_REVISION_VERSION != OCU_IPW_TYPES_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Ocu_Ipw[!"$VS_number"!]_PBcfg.h and Ocu_Ipw_Types.h are different"
#endif

/* Check if header file and Ocu_Ipw_Types.h file are of the same Software version */
#if ((OCU_IPW[!"$VS_number"!]_PBCFG_SW_MAJOR_VERSION != OCU_IPW_TYPES_SW_MAJOR_VERSION) || \
     (OCU_IPW[!"$VS_number"!]_PBCFG_SW_MINOR_VERSION != OCU_IPW_TYPES_SW_MINOR_VERSION) || \
     (OCU_IPW[!"$VS_number"!]_PBCFG_SW_PATCH_VERSION != OCU_IPW_TYPES_SW_PATCH_VERSION))
    #error "Software Version Numbers of Ocu_Ipw[!"$VS_number"!]_PBcfg.h and Ocu_Ipw_Types.h are different"
#endif

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL CONSTANTS
==================================================================================================*/

#define OCU_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Ocu_MemMap.h"

[!NOCODE!]
[!VAR "OcuConfiguredPartitions" = "num:i((count(OcuGeneral/OcuEcucPartitionRef/*)))"!][!//
[!VAR "IndexPartitions" = "0"!][!//
[!IF "$OcuConfiguredPartitions > 0"!][!//
    [!LOOP "OcuGeneral/OcuEcucPartitionRef/*"!][!//
        [!VAR "concatCurrentPartition" = "concat('_P',num:i($IndexPartitions))"!]
        [!CODE!][!WS "0"!]extern const Ocu_Ipw_IpChannelConfigType Ocu_Ipw_IpChCfgPB[!"$VS_number"!][!"$concatCurrentPartition"!][OCU_CONF_CHANNELS_PB]; [!IF "$IndexPartitions < ($OcuConfiguredPartitions - 1)"!]\[!ENDIF!][!CR!][!ENDCODE!]
        [!VAR "IndexPartitions" = "$IndexPartitions + 1"!][!//
    [!ENDLOOP!][!//
    [!ELSE!][!//
    [!CODE!][!WS "0"!]extern const Ocu_Ipw_IpChannelConfigType Ocu_Ipw_IpChCfgPB[!"$VS_number"!][OCU_CONF_CHANNELS_PB];[!CR!][!ENDCODE!][!//
[!ENDIF!][!//
[!ENDNOCODE!]

#define OCU_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Ocu_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

#endif  /* OCU_IPW[!"$VS_number"!]_PBCFG_H */
