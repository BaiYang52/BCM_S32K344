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

/**
 *   @file       Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c
 *   @version    2.0.1
 *
 *   @brief      AUTOSAR Icu - contains the data exported by the Icu module
 *   @details    Contains the information that will be exported by the module, as requested by Autosar.
 *
 *   @addtogroup icu_ipw
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
[!NOCODE!][!//

[!INCLUDE "Icu_Ipw_PluginMacros.m"!][!//

[!ENDNOCODE!][!//
#include "StandardTypes.h"
#include "Icu_Ipw_Types.h"
#include "Siul2_Icu_Ip.h"
#include "Wkpu_Ip.h"
#include "Cmp_Ip.h"
#include "Emios_Icu_Ip.h"

/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 *================================================================================================*/

#define ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C                   43
#define ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C    4
#define ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C    4
#define ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C 0
#define ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C            2
#define ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C            0
#define ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C            1

/*==================================================================================================
 *                                      FILE VERSION CHECKS
 *================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if header file and StandardTypes.h file are of the same Autosar version */
    #if ((ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C != STD_AR_RELEASE_MAJOR_VERSION) || \
         (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C != STD_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c and StandardTypes.h are different"
    #endif
#endif

/* Check if source file and ICU header file are of the same vendor */
#if (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C != ICU_IPW_TYPES_VENDOR_ID)
    #error "Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c and Icu_Ipw_Types.h have different vendor IDs"
#endif
/* Check if source file and ICU header file are of the same AutoSar version */
#if ((ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C  != ICU_IPW_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C  != ICU_IPW_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C   != ICU_IPW_TYPES_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c and Icu_Ipw_Types.h are different"
#endif
/* Check if source file and ICU header file are of the same Software version */
#if ((ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C  != ICU_IPW_TYPES_SW_MAJOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C  != ICU_IPW_TYPES_SW_MINOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C  != ICU_IPW_TYPES_SW_PATCH_VERSION))
#error "Software Version Numbers of Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c and Icu_Ipw_Types.h are different"
#endif

#if (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C != SIUL2_ICU_IP_VENDOR_ID)
    #error "Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c and Siul2_Icu_Ip.h have different vendor ids"
#endif

/* Check if  source file and Siul2_Icu_Ip.h file are of the same Autosar version */
#if ((ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C != SIUL2_ICU_IP_AR_RELEASE_MAJOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C != SIUL2_ICU_IP_AR_RELEASE_MINOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C != SIUL2_ICU_IP_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c and Siul2_Icu_Ip.h are different"
#endif

/* Check if source file and Siul2_Icu_Ip.h file are of the same Software version */
#if ((ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C != SIUL2_ICU_IP_SW_MAJOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C != SIUL2_ICU_IP_SW_MINOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C != SIUL2_ICU_IP_SW_PATCH_VERSION))
    #error "Software Version Numbers of Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c and Siul2_Icu_Ip.h are different"
#endif

#if (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C != EMIOS_ICU_IP_VENDOR_ID)
    #error "Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c and Emios_Icu_Ip.h have different vendor ids"
#endif

/* Check if  source file and Emios_Icu_Ip.h file are of the same Autosar version */
#if ((ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C != EMIOS_ICU_IP_AR_RELEASE_MAJOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C != EMIOS_ICU_IP_AR_RELEASE_MINOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C != EMIOS_ICU_IP_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c and Emios_Icu_Ip.h are different"
#endif

/* Check if source file and Emios_Icu_Ip.h file are of the same Software version */
#if ((ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C != EMIOS_ICU_IP_SW_MAJOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C != EMIOS_ICU_IP_SW_MINOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C != EMIOS_ICU_IP_SW_PATCH_VERSION))
    #error "Software Version Numbers of Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c and Emios_Icu_Ip.h are different"
#endif

#if (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C != WKPU_IP_VENDOR_ID)
    #error "Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c and Wkpu_Ip.h have different vendor ids"
#endif

/* Check if  source file and Wkpu_Ip.h file are of the same Autosar version */
#if ((ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C != WKPU_IP_AR_RELEASE_MAJOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C != WKPU_IP_AR_RELEASE_MINOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C != WKPU_IP_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c and Wkpu_Ip.h are different"
#endif

/* Check if source file and Wkpu_Ip.h file are of the same Software version */
#if ((ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C != WKPU_IP_SW_MAJOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C != WKPU_IP_SW_MINOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C != WKPU_IP_SW_PATCH_VERSION))
    #error "Software Version Numbers of Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c and Wkpu_Ip.h are different"
#endif

#if (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C != CMP_IP_VENDOR_ID)
    #error "Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c and Cmp_Ip.h have different vendor ids"
#endif

/* Check if  source file and Cmp_Ip.h file are of the same Autosar version */
#if ((ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C != CMP_IP_AR_RELEASE_MAJOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C != CMP_IP_AR_RELEASE_MINOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C != CMP_IP_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c and Cmp_Ip.h are different"
#endif

/* Check if source file and Cmp_Ip.h file are of the same Software version */
#if ((ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C != CMP_IP_SW_MAJOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C != CMP_IP_SW_MINOR_VERSION) || \
     (ICU_IPW_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C != CMP_IP_SW_PATCH_VERSION))
    #error "Software Version Numbers of Icu_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCfg.c and Cmp_Ip.h are different"
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

/*==================================================================================================
 *                                       GLOBAL VARIABLES
 *================================================================================================*/
#define ICU_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Icu_MemMap.h"
/*
 *   @brief This index relates the Hardware channels with the respective ICU channel. *
 *   When an normal interrupt is asserted this index is used to locate the corresponding ICU channel
 */[!//

[!NOCODE!]

[!CALL "VALIDATE_EMIOS_CHANNEL_MODE"!]

[!FOR "PartitionIdx" = "1" TO "$IcuEcucPartitionRefNum"!]
    [!IF "$IcuEcucPartitionRefCount != 0 "!]
        [!VAR "PartitionIndex" = "node:name(node:value(IcuGeneral/IcuEcucPartitionRef/*[num:i($PartitionIdx)]))"!]
        [!VAR "IcuEcucPartitionRefName" = "node:value(IcuGeneral/IcuEcucPartitionRef/*[num:i($PartitionIdx)])"!]
    [!ENDIF!]
    [!SELECT "IcuConfigSet"!][!//
        [!CALL "ICU_VARIABLES"!]
        [!IF "num:i($OutNumChannelInPartition)!=0"!]
            [!CODE!]/** [!CR!][!ENDCODE!]
            [!CODE!] * @brief   Icu channels IP related configuration array[!CR!][!ENDCODE!]
            [!CODE!] */[!CR!][!ENDCODE!]
            [!CODE!]const Icu_Ipw_ChannelConfigType Icu_Ipw_IpChannelConfig_[!"$PBCfg_Variant"!][!IF "$IcuEcucPartitionRefCount != 0 "!]_P_[!"$PartitionIndex"!][!ENDIF!][[!"num:i($OutNumChannelInPartition)"!]U] =[!CR!]{[!CR!][!ENDCODE!]
            [!VAR "CurIcuChannelUsingeMios"  = "0"!]
            [!VAR "CurIcuChannelUsingSiul2"  = "0"!]
            [!VAR "CurIcuChannelUsingWkpu"   = "0"!]
            [!VAR "comma" = "1"!]
            [!VAR "NumIcuChannels" = "num:i(count(IcuChannel/*))"!]
            [!FOR "Idx" = "0" TO "$NumIcuChannels - 1"!]
                [!LOOP "IcuChannel/*"!]
                    [!IF "num:i($Idx)=num:i(IcuChannelId)"!]
                    [!IF "(($IcuEcucPartitionRefCount = 0) or (node:value(./IcuChannelEcucPartitionRef/*[1]) = $IcuEcucPartitionRefName)) "!]
                        [!CODE!][!WS "4"!]/** @brief [!"@name"!] */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
                        [!IF "contains(node:path(node:ref(IcuChannelRef)),'IcueMios')"!]
                            [!CODE!][!WS "8"!]/** @brief IP type of this channel */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]ICU_EMIOS_MODULE,[!CR!][!ENDCODE!]
                            
                            [!CODE!][!WS "8"!]/** @brief Instance number */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!](uint8)[!"node:value(node:ref(IcuChannelRef)/../../IcueMiosModule)"!],[!CR!][!ENDCODE!]
                            
                            [!CODE!][!WS "8"!]/** @brief Emios IP channel pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]&eMios_Icu_Ip_[!"node:ref(IcuChannelRef)/../../IcueMiosModule"!]_ChannelConfig_[!"$PBCfg_Variant"!][[!"node:ref(IcuChannelRef)/@index"!]U],[!CR!][!ENDCODE!]                        
                            [!CODE!][!WS "8"!]/** @brief Wkpu IP channel pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                            [!CODE!][!WS "4"!]#if (defined(ICU_WKPU_NMI_API_USED))[!CR!][!ENDCODE!][!//
                            [!CODE!][!WS "8"!]/** @brief Wkpu NMI pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                            [!CODE!][!WS "4"!]#endif[!CR!][!ENDCODE!][!//
                            [!CODE!][!WS "8"!]/** @brief Siul2 IP channel pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]                        
                            [!CODE!][!WS "8"!]/** @brief Cmp IP channel pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]  

                            [!CODE!][!WS "8"!]/** @brief Index in the configuration table of the eMIos channels */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!](uint8)[!"num:i($CurIcuChannelUsingeMios)"!]U[!CR!][!ENDCODE!]
                            [!VAR "CurIcuChannelUsingeMios" = "$CurIcuChannelUsingeMios + 1"!]
                        
                        [!ELSEIF "contains(as:path(node:ref(IcuChannelRef)), 'IcuSiul2')"!]
                            [!CODE!][!WS "8"!]/** @brief IP type of this channel */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]ICU_SIUL2_MODULE,[!CR!][!ENDCODE!]

                            [!CODE!][!WS "8"!]/** @brief Instance number */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!](uint8)[!"node:value(node:ref(IcuChannelRef)/../../IcuSiul2Instance)"!]U,[!CR!][!ENDCODE!]

                            [!CODE!][!WS "8"!]/** @brief Emios IP channel pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]/** @brief Wkpu IP channel pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!] 
                            [!CODE!][!WS "4"!]#if (defined(ICU_WKPU_NMI_API_USED))[!CR!][!ENDCODE!][!//
                            [!CODE!][!WS "8"!]/** @brief Wkpu NMI pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                            [!CODE!][!WS "4"!]#endif[!CR!][!ENDCODE!][!//                            
                            [!CODE!][!WS "8"!]/** @brief Siul2 IP channel pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]&Siul2_Icu_Ip_[!"node:value(node:ref(IcuChannelRef)/../../IcuSiul2Instance)"!]_ChannelConfig_[!"$PBCfg_Variant"!][[!"node:ref(IcuChannelRef)/@index"!]U],[!CR!][!ENDCODE!]                              
                            [!CODE!][!WS "8"!]/** @brief Cmp IP channel pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]  
                            
                            [!CODE!][!WS "8"!]/** @brief Index in the configuration table of the Siul2 channels */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!](uint8)[!"num:i($CurIcuChannelUsingSiul2)"!][!CR!][!ENDCODE!]
                            [!VAR "CurIcuChannelUsingSiul2" = "$CurIcuChannelUsingSiul2 + 1"!]
                        [!ELSEIF "contains(node:path(node:ref(IcuChannelRef)), 'IcuWkpu')"!]
                            [!CODE!][!WS "8"!]/** @brief IP type of this channel */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]ICU_WKPU_MODULE,[!CR!][!ENDCODE!]

                            [!CODE!][!WS "8"!]/** @brief Instance number */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]0U,[!CR!][!ENDCODE!]
                            
                            [!CODE!][!WS "8"!]/** @brief Emios IP channel pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]                             
                          [!IF "contains(node:path(node:ref(IcuChannelRef)),'IcuWkpuNMIConfiguration')"!]
                            [!CODE!][!WS "8"!]/** @brief Wkpu IP channel pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                            [!CODE!][!WS "4"!]#if (defined(ICU_WKPU_NMI_API_USED))[!CR!][!ENDCODE!][!//
                            [!CODE!][!WS "8"!]/** @brief Wkpu NMI pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]&Wkpu_Ip_NMIConfig_[!"$PBCfg_Variant"!][[!"node:ref(IcuChannelRef)/@index"!]U],[!CR!][!ENDCODE!]
                            [!CODE!][!WS "4"!]#endif[!CR!][!ENDCODE!][!//
                          [!ELSE!]
                            [!CODE!][!WS "8"!]/** @brief Wkpu IP channel pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]&Wkpu_Ip_ChannelConfig_[!"$PBCfg_Variant"!][[!"node:ref(IcuChannelRef)/@index"!]U],[!CR!][!ENDCODE!]
                            [!CODE!][!WS "4"!]#if (defined(ICU_WKPU_NMI_API_USED))[!CR!][!ENDCODE!][!//
                            [!CODE!][!WS "8"!]/** @brief Wkpu NMI pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                            [!CODE!][!WS "4"!]#endif[!CR!][!ENDCODE!][!//
                          [!ENDIF!]
                            [!CODE!][!WS "8"!]/** @brief Siul2 IP channel pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]/** @brief Cmp IP channel pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]                             
                            
                            [!CODE!][!WS "8"!]/** @brief Index in the configuration table of the Wkpu channels */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!](uint8)[!"num:i($CurIcuChannelUsingWkpu)"!]U[!CR!][!ENDCODE!]
                            [!VAR "CurIcuChannelUsingWkpu" = "$CurIcuChannelUsingWkpu + 1"!]
                        [!ELSEIF "contains(node:path(node:ref(IcuChannelRef)), 'IcuLpCmp')"!]
                            [!CODE!][!WS "8"!]/** @brief IP type of this channel */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]ICU_CMP_MODULE,[!CR!][!ENDCODE!]

                            [!CODE!][!WS "8"!]/** @brief Instance number */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!](uint8)[!"node:value(node:ref(IcuChannelRef)/IcuCmpInstanceNumber)"!],[!CR!][!ENDCODE!]
                            
                            [!CODE!][!WS "8"!]/** @brief Emios IP channel pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]                             
                            [!CODE!][!WS "8"!]/** @brief Wkpu IP channel pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]  
                            [!CODE!][!WS "4"!]#if (defined(ICU_WKPU_NMI_API_USED))[!CR!][!ENDCODE!][!//
                            [!CODE!][!WS "8"!]/** @brief Wkpu NMI pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                            [!CODE!][!WS "4"!]#endif[!CR!][!ENDCODE!][!//
                            [!CODE!][!WS "8"!]/** @brief Siul2 IP channel pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!] 
                            [!CODE!][!WS "8"!]/** @brief Cmp IP channel pointer */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!]&Cmp_Ip_[!"node:value(node:ref(IcuChannelRef)/IcuCmpInstanceNumber)"!]_Config_[!"$PBCfg_Variant"!],[!CR!][!ENDCODE!] 
                            
                            [!CODE!][!WS "8"!]/** @brief Index in the configuration table of the Cmp */[!CR!][!ENDCODE!]
                            [!CODE!][!WS "8"!](uint8)0U[!CR!][!ENDCODE!]
                        [!ENDIF!]
                        
                        [!CODE!][!WS "4"!]}[!ENDCODE!]
        
                        [!IF "(num:i($comma))<(num:i($OutNumChannelInPartition))"!]
                            [!CODE!],[!CR!][!ENDCODE!]
                        [!ELSE!]
                            [!CODE!][!CR!][!ENDCODE!]
                        [!ENDIF!]
                        [!VAR "comma"="$comma + 1"!]
                    [!ENDIF!]
                    [!ENDIF!]
                [!ENDLOOP!]
                [!ENDFOR!]
            [!CODE!][!CR!]};[!CR!][!CR!][!ENDCODE!]
        [!ENDIF!]
    [!ENDSELECT!]
[!ENDFOR!]

[!FOR "PartitionIdx" = "1" TO "$IcuEcucPartitionRefNum"!]
    [!IF "$IcuEcucPartitionRefCount != 0 "!]
        [!VAR "PartitionIndex" = "node:name(node:value(IcuGeneral/IcuEcucPartitionRef/*[num:i($PartitionIdx)]))"!]
        [!VAR "IcuEcucPartitionRefName" = "node:value(IcuGeneral/IcuEcucPartitionRef/*[num:i($PartitionIdx)])"!]
    [!ENDIF!]
    [!SELECT "IcuConfigSet"!][!//
        [!CALL "ICU_VARIABLES"!]
        
        [!CODE!]const Icu_Ipw_IpConfigType Icu_Ipw_IpConfig_[!"$PBCfg_Variant"!][!IF "$IcuEcucPartitionRefCount != 0 "!]_P_[!"$PartitionIndex"!][!ENDIF!][[!"num:i($OutNumInstancesInPartition)"!]U] =[!CR!]{[!CR!][!ENDCODE!]
      
        [!IF "$OutNumIcuChannelsUsingeMios > 0"!]
            [!FOR "Instance" = "0" TO "num:i(count(ecu:list('Icu.eMios_Hw_Modules'))) - 1"!]
                [!IF "($IcuEcucPartitionRefCount = 0) and node:exists(IcueMios/*/IcueMiosModule[contains(., num:i($Instance))])"!]
                    [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief IP type */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]ICU_EMIOS_MODULE,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief IP instance number */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!][!"$Instance"!]U,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief pointer to Emios HW configuration of instance */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]&eMios_Icu_Ip_[!"$Instance"!]_Config_[!"$PBCfg_Variant"!],[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief pointer to Wkpu HW configuration of instance */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief pointer to Siul2 HW configuration of instance */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]                    
                    [!CODE!][!WS "8"!]/** @brief pointer to LpCmp HW configuration of instance */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]NULL_PTR[!CR!][!ENDCODE!]
                    [!CODE!][!WS "4"!]}[!ENDCODE!]
                    [!IF "num:i($OutNumInstancesInPartition) > 1"!][!CODE!],[!CR!][!ENDCODE!][!VAR "OutNumInstancesInPartition" = "$OutNumInstancesInPartition - 1"!][!ENDIF!]
                [!ELSE!]
                    [!IF "node:exists(IcueMios/*/IcueMiosModule[contains(., num:i($Instance))]) and 
                        (node:exists(IcuChannel/*[(node:refs(IcuChannelRef)/../../IcueMiosModule = num:i($Instance)) and
                        (node:value(./IcuChannelEcucPartitionRef/*[1]) = $IcuEcucPartitionRefName)]))"!]
                        [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]/** @brief IP type */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]ICU_EMIOS_MODULE,[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]/** @brief IP instance number */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!][!"$Instance"!]U,[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]/** @brief pointer to Emios HW configuration of instance */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]&eMios_Icu_Ip_[!"$Instance"!]_Config_[!"$PBCfg_Variant"!],[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]/** @brief pointer to Wkpu HW configuration of instance */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]/** @brief pointer to Siul2 HW configuration of instance */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!] 
                        [!CODE!][!WS "8"!]/** @brief pointer to LpCmp HW configuration of instance */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]NULL_PTR[!CR!][!ENDCODE!]
                        [!CODE!][!WS "4"!]}[!ENDCODE!]
                        [!IF "num:i($OutNumInstancesInPartition) > 1"!][!CODE!],[!CR!][!ENDCODE!][!VAR "OutNumInstancesInPartition" = "$OutNumInstancesInPartition - 1"!][!ENDIF!]
                        [!ENDIF!]
                [!ENDIF!]
            [!ENDFOR!]
        [!ENDIF!]

        [!IF "num:i($OutNumIcuChannelsUsingSiul2) > 0"!]
            [!FOR "Instance" = "0" TO "num:i(count(ecu:list('Icu.Num_Siul2_Hw_Modules'))) - 1"!]
                [!IF "($IcuEcucPartitionRefCount = 0) and node:exists(IcuSiul2/*/IcuSiul2Instance[contains(., num:i($Instance))])"!]
                    [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief IP type */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]ICU_SIUL2_MODULE,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief IP instance number */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!](uint8)[!"$Instance"!]U,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief pointer to Emios HW configuration of instance */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief pointer to Wkpu HW configuration of instance */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!] 
                    [!CODE!][!WS "8"!]/** @brief pointer to Siul2 HW configuration of instance */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]&Siul2_Icu_Ip_[!"$Instance"!]_Config_[!"$PBCfg_Variant"!],[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief pointer to LpCmp HW configuration of instance */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]NULL_PTR[!CR!][!ENDCODE!]
                    [!CODE!][!WS "4"!]}[!CR!][!ENDCODE!]
                    [!IF "num:i($OutNumInstancesInPartition) > 1"!][!CODE!],[!CR!][!ENDCODE!][!VAR "OutNumInstancesInPartition" = "$OutNumInstancesInPartition - 1"!][!ENDIF!]
                [!ELSE!]
                    [!IF "node:exists(IcuSiul2/*/IcuSiul2Instance[contains(., num:i($Instance))]) and 
                          (node:exists(IcuChannel/*[(node:refs(IcuChannelRef)/../../IcuSiul2Instance = num:i($Instance)) and
                          (node:value(./IcuChannelEcucPartitionRef/*[1]) = $IcuEcucPartitionRefName)]))"!]
                        [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]/** @brief IP type */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]ICU_SIUL2_MODULE,[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]/** @brief IP instance number */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!](uint8)[!"$Instance"!]U,[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]/** @brief pointer to Emios HW configuration of instance */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]/** @brief pointer to Wkpu HW configuration of instance */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!] 
                        [!CODE!][!WS "8"!]/** @brief pointer to Siul2 HW configuration of instance */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]&Siul2_Icu_Ip_[!"$Instance"!]_Config_[!"$PBCfg_Variant"!],[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]/** @brief pointer to LpCmp HW configuration of instance */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]NULL_PTR[!CR!][!ENDCODE!]
                        [!CODE!][!WS "4"!]}[!CR!][!ENDCODE!]
                        [!IF "num:i($OutNumInstancesInPartition) > 1"!][!CODE!],[!CR!][!ENDCODE!][!VAR "OutNumInstancesInPartition" = "$OutNumInstancesInPartition - 1"!][!ENDIF!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDFOR!]
        [!ENDIF!]
        
        [!IF "num:i($OutNumIcuChannelsUsingWkpu) > 0"!]
            [!IF "($IcuEcucPartitionRefCount = 0)"!]
                [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]/** @brief IP type */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]ICU_WKPU_MODULE,[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]/** @brief IP instance number */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]0U,[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]/** @brief pointer to Emios HW configuration of instance */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]/** @brief pointer to Wkpu HW configuration of instance */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]&Wkpu_Ip_Config_[!"$PBCfg_Variant"!],[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]/** @brief pointer to Siul2 HW configuration of instance */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!] 
                [!CODE!][!WS "8"!]/** @brief pointer to LpCmp HW configuration of instance */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]NULL_PTR[!CR!][!ENDCODE!]
                [!CODE!][!WS "4"!]}[!ENDCODE!]
                [!IF "num:i($OutNumInstancesInPartition) > 1"!][!CODE!],[!CR!][!ENDCODE!][!VAR "OutNumInstancesInPartition" = "$OutNumInstancesInPartition - 1"!][!ENDIF!]
            [!ELSE!]
                [!IF "node:exists(IcuChannel/*/IcuChannelEcucPartitionRef/*[1][node:value(.) = $IcuEcucPartitionRefName])"!]
                    [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief IP type */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]ICU_WKPU_MODULE,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief IP instance number */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]0U,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief pointer to Emios HW configuration of instance */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief pointer to Wkpu HW configuration of instance */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]&Wkpu_Ip_Config_[!"$PBCfg_Variant"!],[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief pointer to Siul2 HW configuration of instance */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief pointer to LpCmp HW configuration of instance */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]NULL_PTR[!CR!][!ENDCODE!]
                    [!CODE!][!WS "4"!]}[!ENDCODE!]
                    [!IF "num:i($OutNumInstancesInPartition) > 1"!][!CODE!],[!CR!][!ENDCODE!][!VAR "OutNumInstancesInPartition" = "$OutNumInstancesInPartition - 1"!][!ENDIF!]
                [!ENDIF!]
            [!ENDIF!]   
        [!ENDIF!]

        [!IF "$OutNumIcuChannelsUsingCmp > 0"!]
            [!FOR "Instance" = "0" TO "num:i('3') - 1"!]
                [!IF "($IcuEcucPartitionRefCount = 0) and node:exists(IcuLpCmp/*/IcuCmpInstanceNumber[. = num:i($Instance)])"!]
                    [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief IP type */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]ICU_CMP_MODULE,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief IP instance number */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!][!"$Instance"!]U,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief pointer to Emios HW configuration of instance */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief pointer to Wkpu HW configuration of instance */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief pointer to Siul2 HW configuration of instance */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief pointer to LpCmp HW configuration of instance */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]&Cmp_Ip_[!"$Instance"!]_Config_[!"$PBCfg_Variant"!][!CR!][!ENDCODE!]
                    [!CODE!][!WS "4"!]}[!ENDCODE!]
                    [!IF "num:i($OutNumInstancesInPartition) > 1"!][!CODE!],[!CR!][!ENDCODE!][!VAR "OutNumInstancesInPartition" = "$OutNumInstancesInPartition - 1"!][!ENDIF!]
                [!ELSE!]
                    [!IF "node:exists(IcuLpCmp/*/IcuCmpInstanceNumber[. = num:i($Instance)]) and 
                        (node:exists(IcuChannel/*[(node:refs(IcuChannelRef)/IcuCmpInstanceNumber = num:i($Instance)) and
                        (node:value(./IcuChannelEcucPartitionRef/*[1]) = $IcuEcucPartitionRefName)]))"!]
                        [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]/** @brief IP type */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]ICU_CMP_MODULE,[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]/** @brief IP instance number */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!][!"$Instance"!]U,[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]/** @brief pointer to Emios HW configuration of instance */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]/** @brief pointer to Wkpu HW configuration of instance */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]/** @brief pointer to Siul2 HW configuration of instance */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]/** @brief pointer to LpCmp HW configuration of instance */[!CR!][!ENDCODE!]
                        [!CODE!][!WS "8"!]&Cmp_Ip_[!"$Instance"!]_Config_[!"$PBCfg_Variant"!][!CR!][!ENDCODE!]
                        [!CODE!][!WS "4"!]}[!ENDCODE!]
                        [!IF "num:i($OutNumInstancesInPartition) > 1"!][!CODE!],[!CR!][!ENDCODE!][!VAR "OutNumInstancesInPartition" = "$OutNumInstancesInPartition - 1"!][!ENDIF!]
                        [!ENDIF!]
                [!ENDIF!]
            [!ENDFOR!]
        [!ENDIF!]

        [!CODE!]};[!CR!][!CR!][!ENDCODE!]
    [!ENDSELECT!]
[!ENDFOR!]

[!ENDNOCODE!]

#define ICU_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Icu_MemMap.h"

/*==================================================================================================
 *                                   LOCAL FUNCTION PROTOTYPES
 *================================================================================================*/

/*==================================================================================================
 *                                       LOCAL FUNCTIONS
 *================================================================================================*/

/*==================================================================================================
 *                                       GLOBAL FUNCTIONS
 *================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */
[!ENDCODE!]
