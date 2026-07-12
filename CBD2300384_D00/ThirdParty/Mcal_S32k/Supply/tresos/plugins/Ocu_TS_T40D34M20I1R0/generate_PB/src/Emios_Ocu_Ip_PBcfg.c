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
*   @file          Emios_Ocu_Ip[!"$VS_number"!]_PBcfg.c
*                  
*   @addtogroup    ocu_ip Ocu IPL
*   @brief         Ocu IPL Postbuild configuration source file.
*   @details       Postbuild configuration structure instances.
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
#include "Emios_Ocu_Ip[!"$VS_number"!]_PBcfg.h"
[!ELSE!]
#include "Emios_Ocu_Ip_PBcfg.h"
[!ENDIF!]

/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_VENDOR_ID_C                       43
#define EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C        4
#define EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_AR_RELEASE_MINOR_VERSION_C        4
#define EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_AR_RELEASE_REVISION_VERSION_C     0
#define EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_SW_MAJOR_VERSION_C                2
#define EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_SW_MINOR_VERSION_C                0
#define EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_SW_PATCH_VERSION_C                1
/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/
#if (EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_VENDOR_ID_C != EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_VENDOR_ID)
    #error "Emios_Ocu_Ip[!"$VS_number"!]_PBcfg.c and Emios_Ocu_Ip[!"$VS_number"!]_PBcfg.h have different vendor ids"
#endif

/* Check if  source file and Emios_Ocu_Ip[!"$VS_number"!]_PBcfg.h file are of the same Autosar version */
#if ((EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C != EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_AR_RELEASE_MINOR_VERSION_C != EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_AR_RELEASE_REVISION_VERSION_C != EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Emios_Ocu_Ip[!"$VS_number"!]_PBcfg.c and Emios_Ocu_Ip[!"$VS_number"!]_PBcfg.h are different"
#endif

/* Check if source file and Emios_Ocu_Ip[!"$VS_number"!]_PBcfg.h file are of the same Software version */
#if ((EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_SW_MAJOR_VERSION_C != EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_SW_MAJOR_VERSION) || \
     (EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_SW_MINOR_VERSION_C != EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_SW_MINOR_VERSION) || \
     (EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_SW_PATCH_VERSION_C != EMIOS_OCU_IP[!"$VS_number"!]_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Emios_Ocu_Ip[!"$VS_number"!]_PBcfg.c and Emios_Ocu_Ip[!"$VS_number"!]_PBcfg.h are different"
#endif

/*===============================================================================================
*                                       LOCAL MACROS
===============================================================================================*/
[!NOCODE!]
    [!VAR "numOcuChannels" = "num:i(count(OcuConfigSet/OcuChannel/*))"!]
    [!VAR "numOcuModules" = "num:i(count(OcuConfigSet/OcuHWSpecificSettings/*))"!]
    
    [!VAR "max_number_ocu_cfg" = "num:i(count(./OcuGeneral/OcuEcucPartitionRef/*))"!]

    [!VAR "MacNumEmiosChannels"="0"!]
    /* Loop on all channels inside the same configuration Set */
    [!LOOP "OcuConfigSet/OcuChannel/*"!][!//
        [!IF "contains(HwChannel, 'EMIOS')"!][!//
            [!VAR "MacNumEmiosChannels"="$MacNumEmiosChannels + 1"!]
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDNOCODE!]

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL FUNCTION PROTOTYPES
==================================================================================================*/
#define OCU_START_SEC_CODE
#include "Ocu_MemMap.h"

#if (OCU_NOTIFICATION_SUPPORTED == STD_ON)
extern void Ocu_ProcessChannelNotification(uint16 Channel);
#endif

#define OCU_STOP_SEC_CODE
#include "Ocu_MemMap.h"

/*==================================================================================================
*                                       GLOBAL CONSTANTS
==================================================================================================*/

#define OCU_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Ocu_MemMap.h"

[!NOCODE!]

[!SELECT "OcuConfigSet"!]

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

/** @brief counter direction */
[!IF "node:exists(OcuCountdirection)"!]
    [!IF "(OcuCountdirection = 'OCU_EMIOS_DOWNCOUNTING')"!]
        [!VAR "EmiosCountDir" = "'((uint32)OCU_EMIOS_DOWNCOUNTING << OCU_EMIOS_COUNT_DIR_SHIFT)'"!]
    [!ELSE!]
        [!VAR "EmiosCountDir" = "'((uint32)OCU_EMIOS_UPCOUNTING << OCU_EMIOS_COUNT_DIR_SHIFT)'"!]
    [!ENDIF!]
[!ELSE!]
    [!VAR "EmiosCountDir" = "'((uint32)OCU_EMIOS_UPCOUNTING << OCU_EMIOS_COUNT_DIR_SHIFT)'"!]
[!ENDIF!]

/* =====================================================================*/
[!MACRO "GENERATE_CHANNELCONFIG", "OcuEcucPartitionName", "OcuPartitionId"!]

[!IF "$max_number_ocu_cfg != 0"!]
    [!VAR "OcuPartitionIndex" = "concat('_P',$OcuPartitionId)"!]
[!ELSE!]
    [!VAR "OcuPartitionIndex" = "''"!]
[!ENDIF!]

[!IF "(num:i($MacNumEmiosChannels)) != 0"!]

[!CODE!][!CR!]
/***************************** Configuration for eMios ********************/

/** @brief    Configurations for OCU channels using eMios */
static const Emios_Ocu_Ip_ChannelConfigType Emios_Ocu_Ip_ChannelConfig_PB[!"$VS_number"!][!"$OcuPartitionIndex"!][NUMBER_CHANNEL[!"$VS_number"!][!"$OcuPartitionIndex"!]] = {
[!ENDCODE!]

[!VAR "comma" = "1"!]

[!LOOP "../../OcuConfigSet/OcuChannel/*"!]  

    [!VAR "OcuEcucPartitionChannel" = "''"!]
    [!IF "$max_number_ocu_cfg != 0"!]
        [!VAR "OcuEcucPartitionChannel" = "node:value(OcuChannelEcucPartitionRef/*[1])"!]
    [!ENDIF!]


    [!VAR "MacChannelId" = "num:i(./OcuChannelId)"!]
    [!VAR "LocalHwChannel" = "node:value(./HwChannel)"!]
    [!LOOP "../../../OcuGeneral/OcuHwResourceConfig/*"!][!//
        [!IF "((node:value(OcuChannelIsUsed) = 'true') and contains(OcuHwResourceId, 'EMIOS')) and (not(OcuIsrEnable = 'true'))"!][!//
            [!IF "node:value(./OcuHwResourceId) = $LocalHwChannel"!]
                [!VAR "MacChannelId" = "num:i(255)"!]
            [!ENDIF!]
        [!ENDIF!][!//
    [!ENDLOOP!][!//

    [!VAR "DefaultThreshold" = "num:i(OcuDefaultThreshold)"!]
    [!VAR "MaxCounterValue" = "num:i(OcuMaxCounterValue)"!]
    [!VAR "EmiosMasterBus" = "'EMIOS_OCU_BUS_INTERNAL_COUNTER'"!]
    [!VAR "EmiosHardwareChannel" = "HwChannel"!]
    
    [!LOOP "../../OcuHWSpecificSettings/*"!]
        [!IF "contains(OcuHardwareElements,'EMIOS')"!]
            [!IF "contains(OcuHardwareElements,$EmiosHardwareChannel)"!]

                /* get hw information: prescaler */
                [!IF "(node:exists('OcuPrescale'))"!]
                    [!VAR "Prescaler" = "concat('((uint32)EMIOS_PRESCALER_DIVIDE',substring-after(OcuPrescale,'_'),' << OCU_EMIOS_CLOCK_DIV_SHIFT)')"!]
                [!ELSE!]
                    [!VAR "Prescaler" = "'((uint32)EMIOS_PRESCALER_DIVIDE1 << OCU_EMIOS_CLOCK_DIV_SHIFT)'"!]
                [!ENDIF!]

                /* get hw information: alternate prescaler */
                [!IF "(node:exists('OcuPrescale_Alternate'))"!]
                    [!VAR "AltPrescaler" = "concat('((uint32)EMIOS_PRESCALER_DIVIDE',substring-after(OcuPrescale_Alternate,'_'),' << OCU_EMIOS_CLOCK_DIV_SHIFT)')"!]
                [!ELSE!]
                    [!VAR "AltPrescaler" = "'((uint32)EMIOS_PRESCALER_DIVIDE1 << OCU_EMIOS_CLOCK_DIV_SHIFT)'"!]
                [!ENDIF!]

                /* get hw information: debug information */
                [!IF "(OcuEmiosFreeze = 'true')"!]
                    [!VAR "DebugCfg" = "'(uint32)(1UL << OCU_EMIOS_DBG_INFO_SHIFT)'"!]
                [!ELSE!]
                    [!VAR "DebugCfg" = "'(uint32)(0UL << OCU_EMIOS_DBG_INFO_SHIFT)'"!]
                [!ENDIF!]

                /* get hw information: master bus information */
                [!IF "(node:exists('OcuEmiosBusSelect'))"!]
                    [!IF "OcuEmiosBusSelect = 'EMIOS_OCU_BUS_INTERNAL_COUNTER'"!]
                        [!VAR "EmiosMasterBus" = "OcuEmiosBusSelect"!]
                        [!VAR "MasterBusChannel" = "OcuHardwareElements"!]
                    [!ELSE!]
                        [!VAR "EmiosMasterBus" = "OcuEmiosBusSelect"!]
                        [!VAR "MasterBusChannel" = "concat(substring-before(OcuHardwareElements,'_CH'),'_CH_', ecu:get(concat('Ocu.',OcuEmiosBusSelect)))"!]
                    [!ENDIF!]
                [!ELSE!]
                    [!VAR "MasterBusChannel" = "OcuHardwareElements"!]
                [!ENDIF!]

                [!BREAK!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
    
    [!VAR "PinUsed" = "OcuOuptutPinUsed"!]
    [!VAR "PinState" = "'((uint32)OCU_EMIOS_OUTPUT_DISABLED << OCU_EMIOS_OUTPIN_USED_SHIFT)'"!]
    [!VAR "PinAction" = "'((uint32)OCU_EMIOS_SET_DISABLE << OCU_EMIOS_PIN_ACTION_SHIFT)'"!]

    [!IF "($PinUsed = 'true')"!]
        [!IF "(node:exists('OcuOutputPinDefaultState'))"!]
            [!IF "(OcuOutputPinDefaultState = 'OCU_HIGH')"!]
                [!VAR "PinState"= "'((uint32)OCU_EMIOS_OUTPUT_ENABLE << OCU_EMIOS_OUTPIN_USED_SHIFT) | ((uint32)(OCU_EMIOS_HIGH) << OCU_EMIOS_PIN_STATE_SHIFT)'"!]
            [!ELSE!]
                [!VAR "PinState"= "'((uint32)OCU_EMIOS_OUTPUT_ENABLE << OCU_EMIOS_OUTPIN_USED_SHIFT)'"!]
            [!ENDIF!]
        [!ELSE!]
            [!VAR "PinState"= "'((uint32)OCU_EMIOS_OUTPUT_ENABLE << OCU_EMIOS_OUTPIN_USED_SHIFT)'"!]
        [!ENDIF!]

        [!IF "(node:exists('OcuOutputPinAction'))"!]
            [!IF "(OcuOutputPinAction = 'OCU_SET_HIGH')"!]
                [!VAR "PinAction"= "'((uint32)OCU_EMIOS_SET_HIGH << OCU_EMIOS_PIN_ACTION_SHIFT)'"!]
            [!ELSEIF "(OcuOutputPinAction = 'OCU_TOGGLE')"!]
                [!VAR "PinAction"= "'((uint32)OCU_EMIOS_SET_TOGGLE << OCU_EMIOS_PIN_ACTION_SHIFT)'"!]
            [!ELSEIF "(OcuOutputPinAction = 'OCU_DISABLE')"!]
                [!VAR "PinAction"= "'((uint32)OCU_EMIOS_SET_DISABLE << OCU_EMIOS_PIN_ACTION_SHIFT)'"!]
            [!ELSE!]
                 [!VAR "PinAction"= "'((uint32)OCU_EMIOS_SET_LOW << OCU_EMIOS_PIN_ACTION_SHIFT)'"!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDIF!]

[!IF "$OcuEcucPartitionName = $OcuEcucPartitionChannel"!]
[!CODE!]
    /* [!"node:name(.)"!] -> [!"HwChannel"!] */
    {
        /** @brief  eMios hardware channel */
        (uint16)[!"HwChannel"!],
        /** @brief Ocu Default treshold: [!"$DefaultThreshold"!] */
        (uint16)[!"num:inttohex($DefaultThreshold,8)"!]U,
        /** @brief Ocu max counter value: [!"$MaxCounterValue"!]  */
        (uint16)[!"num:inttohex(num:i($MaxCounterValue),8)"!]U,
[!ENDCODE!]
        [!IF "(../../../OcuGeneral/OcuEnableDualClockMode = 'true')"!]
[!CODE!]
#if (OCU_SET_CLOCK_MODE_API == STD_ON)
        /** @brief eMios alternative Control Value */
        (uint32)[!"$AltPrescaler"!],
#endif
[!ENDCODE!]
        [!ENDIF!]
[!CODE!]
        /** @brief eMios Control Value */
        (uint32)
        (
        [!WS "12"!][!"'(uint32)('"!][!"$EmiosMasterBus"!][!"' << EMIOS_OCU_BUS_SELECT_PARAM_SHIFT)'"!] | /* Counter select bits for the given channel */
        [!WS "12"!][!"$PinState"!] |    /* Output Pin Enable and default state */
        [!WS "12"!][!"$PinAction"!] |   /* Output Pin default action */
        [!WS "12"!][!"$DebugCfg"!] |    /* Timer debug config */
        [!WS "12"!][!"$Prescaler"!] /* Timer clock prescaler configuration */
        ),
        /** @brief Ocu Hw Channel of counter BUS */
        [!"$MasterBusChannel"!],
#if (OCU_NOTIFICATION_SUPPORTED == STD_ON)
        [!WS "8"!]/** @brief eMios callback function name */[!CR!]
        [!WS "8"!]&Ocu_ProcessChannelNotification,[!CR!]
        [!WS "8"!]/** @brief eMios callback param */[!CR!]
        [!WS "8"!](uint16)[!"$MacChannelId"!]U[!CR!]
#endif
    }[!IF "(num:i($comma))<(num:i($numOcuChannels))"!],[!ENDIF!][!CR!][!VAR "comma"="$comma + 1"!]
[!ENDCODE!]
[!ENDIF!]
[!ENDLOOP!]

[!CODE!]
};
[!ENDCODE!]

[!CODE!][!CR!]
/** @brief    eMios IP configuration */
const Emios_Ocu_Ip_ModuleConfigType Emios_Ocu_Ip_ModuleCfgPB[!"$VS_number"!][!"$OcuPartitionIndex"!] = {
    /** @brief  Number of eMios channels in the OCU configuration */
    (uint8)NUMBER_CHANNEL[!"$VS_number"!][!"$OcuPartitionIndex"!],
    /** @brief  Pointer to the array of eMios channel enabled OCU channel configurations */
    &Emios_Ocu_Ip_ChannelConfig_PB[!"$VS_number"!][!"$OcuPartitionIndex"!],[!CR!]
};
[!ENDCODE!]

[!ELSE!]
[!CODE!]
    /** @brief No eMios channel configured */
[!ENDCODE!]
[!ENDIF!]

[!ENDMACRO!]
/*=========================================================================*/

[!IF "$max_number_ocu_cfg = 0"!]
    [!SELECT "../OcuGeneral/OcuEcucPartitionRef"!]
    [!CALL "GENERATE_CHANNELCONFIG", "OcuEcucPartitionName" = "''", "OcuPartitionId"="''"!]
    [!ENDSELECT!]
[!ENDIF!]

[!VAR "OcuPartitionId_temp" = "num:i(0)"!]
[!LOOP "../OcuGeneral/OcuEcucPartitionRef/*"!]
    [!VAR "OcuEcucPartitionRefName" = "node:value(.)"!]
    [!SELECT "../."!]   
    [!CALL "GENERATE_CHANNELCONFIG", "OcuEcucPartitionName" = "$OcuEcucPartitionRefName", "OcuPartitionId"="$OcuPartitionId_temp"!]
    [!VAR "OcuPartitionId_temp" = "num:i($OcuPartitionId_temp + 1)"!]
    [!ENDSELECT!]
[!ENDLOOP!]

/*======================================================================*/

[!ENDSELECT!]
[!ENDNOCODE!]


#define OCU_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Ocu_MemMap.h"

/*==================================================================================================
*                                       GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL FUNCTION PROTOTYPES
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
