[!AUTOSPACING!]
[!NOCODE!]
    [!IF "var:defined('postBuildVariant')"!]
        [!VAR "VS_number" = "concat('_',$postBuildVariant)"!]
    [!ELSE!]
        [!VAR "VS_number" = "''"!]
    [!ENDIF!]
[!ENDNOCODE!]
[!CODE!]

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

/**
*   @file         Ocu[!"$VS_number"!]_PBcfg.c
*   @implements   Ocu_PBcfg.c_Artifact
*   @addtogroup   ocu_driver Ocu Driver
*   @brief        Ocu HLD Postbuild configuration source file.
*   @details      Postbuild configuration structure instances.
*
*   @{
*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

[!IF "var:defined('postBuildVariant')"!][!//
#include "Ocu[!"$VS_number"!]_PBcfg.h"
#include "Ocu_Ipw[!"$VS_number"!]_PBcfg.h"
[!ELSE!]
#include "Ocu_PBcfg.h"
#include "Ocu_Ipw_PBcfg.h"
[!ENDIF!]

/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define OCU[!"$VS_number"!]_PBCFG_VENDOR_ID_C                       43
#define OCU[!"$VS_number"!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C        4
#define OCU[!"$VS_number"!]_PBCFG_AR_RELEASE_MINOR_VERSION_C        4
#define OCU[!"$VS_number"!]_PBCFG_AR_RELEASE_REVISION_VERSION_C     0
#define OCU[!"$VS_number"!]_PBCFG_SW_MAJOR_VERSION_C                2
#define OCU[!"$VS_number"!]_PBCFG_SW_MINOR_VERSION_C                0
#define OCU[!"$VS_number"!]_PBCFG_SW_PATCH_VERSION_C                1
/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/
#if (OCU[!"$VS_number"!]_PBCFG_VENDOR_ID_C != OCU[!"$VS_number"!]_PBCFG_VENDOR_ID)
    #error "Ocu[!"$VS_number"!]_PBcfg.c and Ocu[!"$VS_number"!]_PBcfg.h have different vendor ids"
#endif

/* Check if  source file and Ocu[!"$VS_number"!]_PBcfg.h file are of the same Autosar version */
#if ((OCU[!"$VS_number"!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C != OCU[!"$VS_number"!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (OCU[!"$VS_number"!]_PBCFG_AR_RELEASE_MINOR_VERSION_C != OCU[!"$VS_number"!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (OCU[!"$VS_number"!]_PBCFG_AR_RELEASE_REVISION_VERSION_C != OCU[!"$VS_number"!]_PBCFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Ocu[!"$VS_number"!]_PBcfg.c and Ocu[!"$VS_number"!]_PBcfg.h are different"
#endif

/* Check if source file and Ocu[!"$VS_number"!]_PBcfg.h file are of the same Software version */
#if ((OCU[!"$VS_number"!]_PBCFG_SW_MAJOR_VERSION_C != OCU[!"$VS_number"!]_PBCFG_SW_MAJOR_VERSION) || \
     (OCU[!"$VS_number"!]_PBCFG_SW_MINOR_VERSION_C != OCU[!"$VS_number"!]_PBCFG_SW_MINOR_VERSION) || \
     (OCU[!"$VS_number"!]_PBCFG_SW_PATCH_VERSION_C != OCU[!"$VS_number"!]_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Ocu[!"$VS_number"!]_PBcfg.c and Ocu[!"$VS_number"!]_PBcfg.h are different"
#endif

#if (OCU[!"$VS_number"!]_PBCFG_VENDOR_ID_C != OCU_IPW[!"$VS_number"!]_PBCFG_VENDOR_ID)
    #error "Ocu[!"$VS_number"!]_PBcfg.c and Ocu_Ipw[!"$VS_number"!]_PBcfg.h have different vendor ids"
#endif

/* Check if  source file and Ocu_Ipw[!"$VS_number"!]_PBcfg.h file are of the same Autosar version */
#if ((OCU[!"$VS_number"!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C != OCU_IPW[!"$VS_number"!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (OCU[!"$VS_number"!]_PBCFG_AR_RELEASE_MINOR_VERSION_C != OCU_IPW[!"$VS_number"!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (OCU[!"$VS_number"!]_PBCFG_AR_RELEASE_REVISION_VERSION_C != OCU_IPW[!"$VS_number"!]_PBCFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Ocu[!"$VS_number"!]_PBcfg.c and Ocu_Ipw[!"$VS_number"!]_PBcfg.h are different"
#endif

/* Check if source file and Ocu_Ipw[!"$VS_number"!]_PBcfg.h file are of the same Software version */
#if ((OCU[!"$VS_number"!]_PBCFG_SW_MAJOR_VERSION_C != OCU_IPW[!"$VS_number"!]_PBCFG_SW_MAJOR_VERSION) || \
     (OCU[!"$VS_number"!]_PBCFG_SW_MINOR_VERSION_C != OCU_IPW[!"$VS_number"!]_PBCFG_SW_MINOR_VERSION) || \
     (OCU[!"$VS_number"!]_PBCFG_SW_PATCH_VERSION_C != OCU_IPW[!"$VS_number"!]_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Ocu[!"$VS_number"!]_PBcfg.c and Ocu_Ipw[!"$VS_number"!]_PBcfg.h are different"
#endif



/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

[!NOCODE!][!//​

/* Get max number partition is used */
[!VAR "maxPartition" = "num:i(0)"!]
[!IF "node:exists(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition)"!]
    [!VAR "maxPartition" = "num:i(count(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition/*))"!] 
[!ENDIF!]
[!IF "$maxPartition = 0"!][!VAR "maxPartition" = "num:i(1)"!][!ENDIF!]

[!VAR "max_number_ocu_cfg" = "num:i(count(./OcuGeneral/OcuEcucPartitionRef/*))"!]
[!VAR "max_number_partition" = "num:i($maxPartition)"!]

[!VAR "numOcuChannels" = "num:i(count(OcuConfigSet/OcuChannel/*))"!]

[!VAR "MacNumFtmChannels"="0"!]
[!VAR "MacNumEtimerChannels"="0"!]
[!VAR "MacNumEmiosChannels"="0"!]

// Loop on all channels inside the same Configuration Set
[!LOOP "OcuConfigSet/OcuChannel/*"!][!//
    [!IF "contains(HwChannel, 'ETIMER')"!][!//
        [!VAR "MacNumEtimerChannels"="$MacNumEtimerChannels + 1"!]
    [!ELSEIF "contains(HwChannel, 'FTM')"!][!//
        [!VAR "MacNumFtmChannels"="$MacNumFtmChannels + 1"!]
    [!ELSEIF "contains(HwChannel, 'EMIOS')"!][!//
        [!VAR "MacNumEmiosChannels"="$MacNumEmiosChannels + 1"!]
    [!ENDIF!][!//
[!ENDLOOP!][!//

/* Variable storing number of maximum core_id of partitions using in configuration */
[!VAR "numPartition" = "0"!]
[!VAR "CurrentCoreId" = "0"!]

[!LOOP "OcuGeneral/OcuEcucPartitionRef/*"!]
    [!VAR "OcuEcucPartitionRefName" = "node:value(.)"!]
    [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
        [!IF "$OcuEcucPartitionRefName = node:value(./OsAppEcucPartitionRef)"!]
            [!SELECT "node:ref(./OsApplicationCoreRef)"!]
            [!VAR "CurrentCoreId" = "node:value(./EcucCoreId)"!]
            [!IF "num:i($numPartition) < num:i($CurrentCoreId)"!]
                [!VAR "numPartition" = "$CurrentCoreId"!]
            [!ENDIF!]
            [!ENDSELECT!]
        [!ENDIF!]
    [!ENDLOOP!]
[!ENDLOOP!]

[!VAR "numPartition" = "$numPartition + 1"!]

[!CODE!]
/*===============================================================================================
*                                       LOCAL MACROS
===============================================================================================*/

/*==================================================================================================
*                                       LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                     FUNCTION PROTOTYPES
==================================================================================================*/

#define OCU_START_SEC_CODE
#include "Ocu_MemMap.h"
[!CR!][!ENDCODE!]

[!SELECT "OcuConfigSet"!]

[!CODE!][!CR!]
#if (OCU_NOTIFICATION_SUPPORTED == STD_ON)
[!CR!][!ENDCODE!]

[!LOOP "OcuChannel/*"!]
    [!IF "node:exists('OcuNotification')"!]
        [!IF "(OcuNotification = '') or (OcuNotification = 'NULL_PTR') or (OcuNotification = 'NULL')"!]
        [!ELSE!]
[!CODE!]extern void [!"OcuNotification"!](void);[!CR!][!ENDCODE!]
        [!ENDIF!]
    [!ENDIF!]
[!ENDLOOP!]

[!CODE!][!CR!]
#endif
[!CR!][!ENDCODE!]

[!CODE!][!CR!]

#define OCU_STOP_SEC_CODE
#include "Ocu_MemMap.h"

/*==================================================================================================
 *                                       GLOBAL CONSTANTS
 ==================================================================================================*/

#define OCU_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Ocu_MemMap.h"

[!ENDCODE!][!CR!]

/*=========================================================================*/

[!VAR "numOcuChannelsInCfg" = "num:i(count(OcuChannel/*))"!]
[!IF "$max_number_ocu_cfg = 0"!]
[!CODE!]
/** @brief Number of configured Ocu channels */
#define NUMBER_CHANNEL[!"$VS_number"!] [!"$numOcuChannelsInCfg"!]U
[!CR!][!ENDCODE!]
[!ENDIF!]

[!VAR "OcuPartitionId_temp" = "num:i(0)"!]

[!LOOP "../OcuGeneral/OcuEcucPartitionRef/*"!]
    [!VAR "OcuEcucPartitionRefName" = "node:value(.)"!]
    [!VAR "channelMatched" = "0"!]

    [!LOOP "../../../OcuConfigSet/OcuChannel/*"!]
        [!VAR "OcuEcucPartitionChannel" = "node:value(OcuChannelEcucPartitionRef/*[1])"!]

        [!IF "($OcuEcucPartitionRefName = $OcuEcucPartitionChannel)"!]
            [!VAR "channelMatched"="$channelMatched + 1"!]
        [!ENDIF!]
    [!ENDLOOP!]

[!CODE!]
/** @brief  Number of configured Ocu channels of Partition [!"$OcuPartitionId_temp"!] */
#define NUMBER_CHANNEL[!"$VS_number"!]_P[!"$OcuPartitionId_temp"!]  [!"num:i($channelMatched)"!]U
[!CR!][!ENDCODE!]

    [!VAR "OcuPartitionId_temp" = "num:i($OcuPartitionId_temp + 1)"!]
[!ENDLOOP!]

/*============================================================
 * This MACRO will return CoreId and OcuPartitionId if it used
 * ============================================================*/
[!VAR "CoreIdDefault" = "num:i(0)"!]
 [!MACRO "Check_CoreID_Used", "Core_index"!]
     [!VAR "CoreId" = "255"!]
     [!VAR "OcuPartitionId" = "255"!]
     [!VAR "OcuPartitionId_temp" = "0"!]
[!IF "$max_number_ocu_cfg != 0"!]
     [!LOOP "../OcuGeneral/OcuEcucPartitionRef/*"!]
     [!VAR "currentPartition_temp" = "substring-after(substring-after(substring-after(substring-after(.,'/'),'/'),'/'),'/')"!][!//
        [!VAR "OcuEcucPartitionRefName" = "node:value(.)"!]
            [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
            [!IF "$OcuEcucPartitionRefName = node:value(./OsAppEcucPartitionRef)"!]
                [!SELECT "node:ref(./OsApplicationCoreRef)"!]
                [!IF "num:i(node:value(./EcucCoreId)) = num:i($Core_index)"!]
                    [!VAR "CoreId" = "node:value(./EcucCoreId)"!]
                    [!VAR "OcuPartitionId" = "num:i($OcuPartitionId_temp)"!]
                    [!VAR "currentPartition" = "concat('_',$currentPartition_temp)"!]
                 [!ENDIF!]
                [!ENDSELECT!]
            [!ENDIF!]
            [!ENDLOOP!]
    [!VAR "OcuPartitionId_temp" = "num:i($OcuPartitionId_temp + 1)"!]    
    [!ENDLOOP!]
[!ELSE!]
    [!IF "$Core_index = $CoreIdDefault"!]
            [!VAR "CoreId" = "$CoreIdDefault"!]
            [!VAR "OcuPartitionId" = "num:i(0)"!]
            [!VAR "currentPartition" = "''"!]
    [!ENDIF!]
[!ENDIF!]
 [!ENDMACRO!]

/*============================================================*/
/*  Macro used to generate the correspondence (translation) table between all available
    channels and the indexes of channels in the array of Ocu channels configured to use current IP */
[!MACRO "GenerateHwToLogicMap", "nameIpv", "number_channel", "numberHWmodule"!]
[!FOR "idx" = "0" TO "$numberHWmodule - 1"!]
    [!VAR "maxNumChannels"="$number_channel - 1"!]
    [!CODE!]
    /* Idx of channels 0 - [!"num:i($maxNumChannels)"!] of [!"$nameIpv"!] Module [!"num:i($idx)"!], in the array of logic Ocu channels */
    [!ENDCODE!]
    [!NOCODE!]
    /* Loop through all channels of current IP received as parameter */
    [!FOR "MacCurModChannel" = "0" TO "num:i($maxNumChannels)"!]

        [!VAR "MacChannelId" = "num:i(255)"!]

        /* Loop through all Ocu channels configured to use current IP */
        [!LOOP "OcuChannel/*"!]
            [!IF "(contains(HwChannel, $nameIpv))"!]

                /* Build the comment for the current Module */
                
                /* Get indexes of current IP and Channel of current looped Ocu channel */
                [!VAR "MacIpModule"  = "num:i(substring-before(substring-after(HwChannel,concat( $nameIpv ,'_')),'_CH_'))"!]
                [!VAR "MacIpChannel" = "num:i(substring-after(HwChannel,'_CH_'))"!]

                /* Check the match with the module received as parameter */
                [!IF "$MacIpModule = $idx"!]
                    /* Check the match with the channel loop var */
                    [!IF "num:i($MacIpChannel) = $MacCurModChannel"!]
                        /* Keep the index of the channel in the array of configured Ocu channels */
                        [!VAR "MacChannelId" = "num:i(./OcuChannelId)"!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDIF!]
        [!ENDLOOP!]

        [!CODE!]
        [!WS "4"!](Ocu_ChannelType)[!"$MacChannelId"!]U,  /* [!"$nameIpv"!]_[!"$idx"!]_CH_[!"$MacCurModChannel"!] */
        [!ENDCODE!]

    [!ENDFOR!]
    [!ENDNOCODE!]
[!ENDFOR!]
[!ENDMACRO!]

/*======================================================================*/
[!CODE!][!CR!]
/** @brief Array of configured Ocu channels */
static const Ocu_ChannelConfigType Ocu_ConfiguredChannels_PB[!"$VS_number"!][OCU_CONF_CHANNELS_PB] = {[!CR!]
[!ENDCODE!]

[!VAR "comma" = "1"!]

[!FOR "varCounter" = "0" TO "$numOcuChannels"!]
    [!LOOP "OcuChannel/*"!]
        [!IF "OcuChannelId = $varCounter"!]
            [!CODE!]
            [!WS "4"!]{
            [!WS "8"!]/** @brief [!"@name"!]  OuptutPinUsed */
            [!IF "OcuOuptutPinUsed = 'true'"!]
                [!WS "8"!](boolean)TRUE,
            [!ELSE!]
                [!WS "8"!](boolean)FALSE,
            [!ENDIF!]
#if (OCU_NOTIFICATION_SUPPORTED == STD_ON)
            [!WS "8"!]/** @brief  Ocu notification function */
            [!ENDCODE!]

            [!IF "node:exists('OcuNotification')"!]
                [!IF "(OcuNotification = '') or (OcuNotification = 'NULL_PTR') or (OcuNotification = 'NULL')"!]
                    [!CODE!][!WS "8"!]NULL_PTR[!CR!][!ENDCODE!]
                [!ELSE!]
                    [!CODE!]
                    [!WS "8"!]&[!"OcuNotification"!]
                    [!ENDCODE!]
                [!ENDIF!]
            [!ELSE!]
                [!CODE!][!WS "8"!]NULL_PTR[!CR!][!ENDCODE!]
            [!ENDIF!]

            [!CODE!]
#endif
            [!WS "4"!]}[!IF "(num:i($comma))<(num:i($numOcuChannels))"!],[!ENDIF!][!VAR "comma"="$comma + 1"!][!CR!]
            [!ENDCODE!]
        [!ENDIF!]
    [!ENDLOOP!]
[!ENDFOR!]

[!CODE!]
};[!CR!][!CR!]
[!ENDCODE!]

/*======================================================================*/     
[!SELECT "../OcuGeneral/OcuEcucPartitionRef"!]
[!FOR "OcuPartitionId_temp" = "0" TO "$max_number_ocu_cfg"!]

    [!IF "$max_number_ocu_cfg != 0"!]
        [!IF "$OcuPartitionId_temp = $max_number_ocu_cfg"!] [!BREAK!] [!ENDIF!] 
        [!VAR "OcuEcucPartitionRefName" = "node:value(./*[$OcuPartitionId_temp + 1])"!]
        [!VAR "OcuPartitionIndex" = "concat('_P',$OcuPartitionId_temp)"!]
    [!ELSE!]
        [!VAR "OcuEcucPartitionRefName" = "''"!]
        [!VAR "OcuPartitionIndex" = "''"!]
    [!ENDIF!] 
    
[!CODE!][!CR!]
/** @brief    List of Ocu channel arrays used for partition [!"$OcuPartitionId_temp"!] */
static const Ocu_ChannelType Ocu_Channel_Use[!"$VS_number"!][!"$OcuPartitionIndex"!][OCU_CONF_CHANNELS_PB] = {[!CR!]
[!ENDCODE!]
    [!VAR "channelMatched" = "0"!]
    [!FOR "varCounter" = "0" TO "$numOcuChannels"!]
        [!LOOP "../../OcuConfigSet/OcuChannel/*"!]
            [!IF "OcuChannelId = $varCounter"!]
                [!VAR "channelMatched"="$channelMatched + 1"!]
                [!VAR "OcuEcucPartitionChannel" = "''"!]
                [!IF "$max_number_ocu_cfg != 0"!]
                    [!VAR "OcuEcucPartitionChannel" = "node:value(OcuChannelEcucPartitionRef/*[1])"!]
                [!ENDIF!]

                [!CODE!][!WS "4"!]/** @brief [!"@name"!] */[!CR!][!ENDCODE!]

                [!IF "$OcuEcucPartitionRefName = $OcuEcucPartitionChannel"!]
                    [!CODE!][!WS "4"!](Ocu_ChannelType)[!"HwChannel"!],[!CR!][!ENDCODE!]
                [!ELSE!]
                    [!CODE!][!WS "4"!](Ocu_ChannelType)OCU_INVALID_CHANNEL_NUM,[!CR!][!ENDCODE!]
                [!ENDIF!]
            [!ENDIF!]
        [!ENDLOOP!]
    [!ENDFOR!]
[!CODE!]
};[!CR!][!CR!]
[!ENDCODE!]

[!ENDFOR!]
[!ENDSELECT!]
/* ==================================================================*/

[!CODE!][!CR!]
#if (OCU_NOTIFICATION_SUPPORTED == STD_ON)
/** @brief  Hw to logic channel map. Array containing a number of elements
            equal to total number of available channels on Ftm, eTimer, eMios */
static const Ocu_ChannelType Ocu_InitHWMap_PB[!"$VS_number"!][OCU_NUM_HW_CHANNELS] = {[!CR!]
    [!CALL "GenerateHwToLogicMap", "nameIpv" = "'FTM'", "number_channel" = "ecu:get('Ocu.Num_Ftm_Hw_Channels')", "numberHWmodule" = "ecu:get('Ocu.Num_Ftm_Hw_Modules')"!]
    [!CALL "GenerateHwToLogicMap", "nameIpv" = "'ETIMER'", "number_channel" = "ecu:get('Ocu.Num_Etimer_Hw_Channels')", "numberHWmodule" = "ecu:get('Ocu.Num_Etimer_Hw_Modules')"!]
    [!CALL "GenerateHwToLogicMap", "nameIpv" = "'EMIOS'", "number_channel" = "ecu:get('Ocu.Num_Emios_Hw_Channels')", "numberHWmodule" = "ecu:get('Ocu.Num_Emios_Hw_Modules')"!]
};
#endif
[!ENDCODE!]

/*=================================================================*/

[!FOR "XCoreId" = "0" TO "$max_number_partition - 1"!]
    [!CALL "Check_CoreID_Used", "Core_index" = "$XCoreId"!]
    [!IF "num:i($CoreId) != num:i(255)"!]

        [!IF "$max_number_ocu_cfg != 0"!]
            [!VAR "OcuPartitionId_generate" = "concat('_P',$OcuPartitionId)"!]
        [!ELSE!]
            [!VAR "OcuPartitionId_generate" = "''"!]
        [!ENDIF!]

        [!CODE!][!CR!][!CR!]
[!WS "0"!]/** @brief   Ocu high level configuration structure for partition [!"$OcuPartitionId"!] */[!CR!]
        [!ENDCODE!]

        [!NOCODE!]
        [!IF "(../IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)"!]
    [!CODE!][!CR!]
static const Ocu_ConfigType Ocu_Config_Pc[!"$OcuPartitionId_generate"!] = {[!CR!]
    [!ENDCODE!]
        [!ELSE!]
    [!CODE!][!CR!]
const Ocu_ConfigType Ocu_Config[!"$VS_number"!][!"$currentPartition"!] = {[!CR!]
    [!ENDCODE!]
        [!ENDIF!]
        [!ENDNOCODE!]
    [!CODE!][!CR!]
    /** @brief Number of configured Ocu channels */
    (Ocu_ChannelType)NUMBER_CHANNEL[!"$VS_number"!][!"$OcuPartitionId_generate"!],
    /** @brief Pointer to array of Ocu channels */
    &Ocu_ConfiguredChannels_PB[!"$VS_number"!],[!CR!]
    /** @brief IP specific configuration */
    {[!CR!]
    [!NOCODE!]
        [!IF "(num:i($MacNumFtmChannels)) != 0"!]
            [!CODE!][!WS "8"!]/** @brief Pointer to the structure containing Ftm configuration  */[!CR!][!ENDCODE!]
            [!CODE!][!WS "8"!]&Ftm_Ocu_Ip_ModuleCfgPB[!"$VS_number"!][!"$OcuPartitionId_generate"!],[!CR!][!ENDCODE!]
        [!ELSEIF "(num:i($MacNumEmiosChannels)) != 0"!]
            [!CODE!][!WS "8"!]/** @brief Pointer to the structure containing eMios configuration  */[!CR!][!ENDCODE!]
            [!CODE!][!WS "8"!]&Emios_Ocu_Ip_ModuleCfgPB[!"$VS_number"!][!"$OcuPartitionId_generate"!],[!CR!][!ENDCODE!]
        [!ELSEIF "(num:i($MacNumEtimerChannels)) != 0"!]
            [!CODE!][!WS "8"!]/** @brief Pointer to the structure containing eTimer configuration  */[!CR!][!ENDCODE!]
            [!CODE!][!WS "8"!]&Etimer_Ocu_Ip_ModuleCfgPB[!"$VS_number"!][!"$OcuPartitionId_generate"!],[!CR!][!ENDCODE!]
        [!ELSE!]
            [!CODE!][!WS "8"!]/** @brief No channel configured  */[!CR!][!ENDCODE!]
            [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
        [!ENDIF!]
        [!CODE!][!WS "8"!]/** @brief Pointer to Array containing channe IP related information */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]&Ocu_Ipw_IpChCfgPB[!"$VS_number"!][!"$OcuPartitionId_generate"!],[!CR!][!ENDCODE!]
    [!ENDNOCODE!]
    },
        [!IF "node:exists(OcuCountdirection)"!]
#if (STD_ON == OCU_DOWNCOUNTING_SUPPORED)
    /** @brief counter direction */
            [!IF "OcuCountdirection = 'OCU_DOWNCOUNTING'"!]
    OCU_DOWNCOUNTING,
            [!ELSE!]
    OCU_UPCOUNTING,
            [!ENDIF!]
#endif
        [!ENDIF!]
#if (OCU_NOTIFICATION_SUPPORTED == STD_ON)
    &Ocu_InitHWMap_PB[!"$VS_number"!],
#endif  
    &Ocu_Channel_Use[!"$VS_number"!][!"$OcuPartitionId_generate"!],
    /** @brief Index Core [!"$CoreId"!]*/
    (uint8)[!"$CoreId"!],
};[!CR!][!CR!]

/*======================= End Partition [!"$OcuPartitionId"!] ===========================*/[!CR!][!CR!]
    [!ENDCODE!]
    [!ELSE!]
        [!CODE!][!WS "4"!]/** @brief  No configuration structure of OCU refered by core [!"num:i($XCoreId)"!] */[!CR!][!ENDCODE!]
    [!ENDIF!]
[!ENDFOR!]

/*====================================================*/

[!IF "(../IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)"!]
[!CODE!][!CR!][!CR!]
/** @brief   Array of pointers to Ocu high level config structures */
const Ocu_ConfigType * const Ocu_gapcxConfig[OCU_MAX_PARTITIONS] = {[!CR!]
[!ENDCODE!]

[!FOR "XCoreId" = "0" TO "$max_number_partition - 1"!]
    [!CALL "Check_CoreID_Used", "Core_index" = "$XCoreId"!]
    [!IF "num:i($CoreId) != num:i(255)"!]
        [!IF "$max_number_ocu_cfg != 0"!]
            [!VAR "OcuPartitionId_generate" = "concat('_P',$OcuPartitionId)"!]
        [!ELSE!]
            [!VAR "OcuPartitionId_generate" = "''"!]
        [!ENDIF!]
        [!CODE!][!WS "4"!]/** @brief  Pointer to configuration structure of Ocu for partition reference to core [!"num:i($XCoreId)"!] */[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]&Ocu_Config_Pc[!"$OcuPartitionId_generate"!],[!CR!][!ENDCODE!]
    [!ELSE!]
        [!CODE!][!WS "4"!]/** @brief  No configuration structure of OCU refered by core [!"num:i($XCoreId)"!] */
        [!WS "4"!]NULL_PTR,[!CR!][!ENDCODE!]
    [!ENDIF!]
[!ENDFOR!]

[!CODE!][!CR!]
};
[!ENDCODE!]

[!ENDIF!]


[!ENDSELECT!]
[!ENDNOCODE!]

#define OCU_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Ocu_MemMap.h"

/*==================================================================================================
*                                       GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                        LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */
[!ENDCODE!]
