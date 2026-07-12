/*==================================================================================================
* Project : RTD AUTOSAR 4.4
* Platform : CORTEXM
* Peripheral : Stm_Pit_Rtc_Emios
* Dependencies : none
*
* Autosar Version : 4.4.0
* Autosar Revision : ASR_REL_4_4_REV_0000
* Autosar Conf.Variant :
* SW Version : 2.0.1
* Build Version : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
* (c) Copyright 2020 - 2022 NXP Semiconductors
* All Rights Reserved.
*
* NXP Confidential. This software is owned or controlled by NXP and may only be
* used strictly in accordance with the applicable license terms. By expressly
* accepting such terms or by downloading, installing, activating and/or otherwise
* using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms. If you do not agree to be
* bound by the applicable license terms, then you may not retain, install,
* activate or otherwise use the software.
==================================================================================================*/

#ifndef PIT_IP_CFG_DEFINES_H
#define PIT_IP_CFG_DEFINES_H

/**
*   @file       Pit_Ip_Cfg_Defines.h
*
*   @addtogroup pit_ip Pit IPL
*
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
[!NOCODE!][!// Include specific header file
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!VAR "DerivativeName" = "text:toupper(substring-before(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative,'_'))"!][!//
    [!IF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k341')"!][!//
        [!CODE!][!WS "0"!]#include "S32K342_PIT.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 'saf85')"!][!//
        [!CODE!][!WS "0"!]#include "SAF85XX_PIT.h"[!CR!][!ENDCODE!][!//
    [!ELSE!][!//
        [!CODE!][!WS "0"!]#include "[!"$DerivativeName"!]_PIT.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ELSE!][!//
    [!ERROR "The Resource plugins or ResourceSubderivative field is not available for header file inclusion."!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define PIT_IP_DEFINES_VENDOR_ID_CFG                    43
#define PIT_IP_DEFINES_AR_RELEASE_MAJOR_VERSION_CFG     4
#define PIT_IP_DEFINES_AR_RELEASE_MINOR_VERSION_CFG     4
#define PIT_IP_DEFINES_AR_RELEASE_REVISION_VERSION_CFG  0
#define PIT_IP_DEFINES_SW_MAJOR_VERSION_CFG             2
#define PIT_IP_DEFINES_SW_MINOR_VERSION_CFG             0
#define PIT_IP_DEFINES_SW_PATCH_VERSION_CFG             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if header file and StandardTypes.h file are of the same Autosar version */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((PIT_IP_DEFINES_AR_RELEASE_MAJOR_VERSION_CFG != STD_AR_RELEASE_MAJOR_VERSION) || \
         (PIT_IP_DEFINES_AR_RELEASE_MINOR_VERSION_CFG != STD_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Pit_Ip_Cfg_Defines.h and StandardTypes.h are different"
    #endif
#endif
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
[!NOCODE!][!//
[!VAR "MacPitUsed"="0"!][!//
[!VAR "MacPitRtiUsed"="0"!][!//
    // Loop on all channels inside the same Configuration Set
    [!LOOP "GptHwConfiguration/*"!][!//
        [!IF "node:value(GptChannelIsUsed) = 'true' and contains(GptIsrHwId, 'PIT') and not(contains(GptIsrHwId, 'RTI'))"!][!//
            [!VAR "MacPitUsed"="1"!][!//
        [!ELSEIF "node:value(GptChannelIsUsed) = 'true' and contains(GptIsrHwId, 'RTI')"!][!//
            [!VAR "MacPitRtiUsed"="1"!][!//
            [!VAR "MacPitUsed"="1"!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDNOCODE!][!//
/**
* @brief These defines indicate that at least one channel from each module is used in all configurations.
*/
#define PIT_IP_USED ([!IF "$MacPitUsed=1"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
#define PIT_IP_RTI_USED ([!IF "$MacPitRtiUsed=1"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

[!NOCODE!][!//
[!VAR "MacRtiExists"="0"!][!//
    // Loop on all channels inside the same Configuration Set
    [!LOOP "GptHwConfiguration/*"!][!//
        [!IF " contains(GptIsrHwId, 'RTI')"!][!//
            [!VAR "MacRtiExists"="1"!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!IF "$MacRtiExists=1"!]
    [!CODE!]
#define PIT_IP_RTI_CHANNEL_EXISTS
    [!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!][!//
/**
* @brief This define is used to select between interrupt on each channel and source interrupt
*        on entire module sources hardware implementations.
*
*/
#define PIT_IP_MODULE_SINGLE_INTERRUPT [!IF "ecu:get('Gpt.GptConfig.Gpt_PIT_ModuleSingleInterrupt')='STD_ON'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief This defines
*
*/
#define PIT_IP_PECULIAR_INSTANCES [!IF "ecu:get('Gpt.GptConfig.Gpt_PIT_PeculiarInstances')='STD_ON'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
*
* @brief IRQ Defines for each channel used
*/
[!NOCODE!][!//
[!VAR "AlreadyParsedModule"!][!ENDVAR!][!//
[!LOOP "GptHwConfiguration/*"!][!//
    [!IF "(GptChannelIsUsed = 'true') and (GptIsrEnable = 'true')"!][!//
        [!IF "contains(GptIsrHwId,"CE_PIT") or contains(GptIsrHwId,"RTU_PIT")"!][!//
            [!VAR "ModuleUsed"="concat(concat(text:split(GptIsrHwId,'_')[1], '_', text:split(GptIsrHwId,'_')[2]), '_' , text:split(GptIsrHwId,'_')[3])"!][!//
            [!IF "contains($AlreadyParsedModule, $ModuleUsed) = false()"!][!//
                [!CODE!][!//
#define [!"$ModuleUsed"!]_ISR_USED
                [!ENDCODE!][!//
                [!VAR "AlreadyParsedModule"="concat($AlreadyParsedModule,$ModuleUsed,'|')"!][!//
            [!ENDIF!][!//
        [!ELSEIF "contains(GptIsrHwId,"PIT")"!][!//
            [!VAR "ModuleUsed"="concat(text:split(GptIsrHwId,'_')[1], '_', text:split(GptIsrHwId,'_')[2])"!][!//
            [!IF "contains($AlreadyParsedModule,$ModuleUsed) = false()"!][!//
                [!CODE!][!//
#define [!"$ModuleUsed"!]_ISR_USED
                [!ENDCODE!][!//
                [!VAR "AlreadyParsedModule"="concat(text:split(GptIsrHwId,'_')[1], '_', text:split(GptIsrHwId,'_')[2])"!][!//
            [!ENDIF!][!//
        [!ENDIF!][!//
    [!ENDIF!]//
[!ENDLOOP!][!//
[!ENDNOCODE!][!//

/**
*
* @brief Module Disable for PIT bit exists on current platform
*/
[!NOCODE!][!// Include specific header file
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!IF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32e27')"!][!//
        [!CODE!][!WS "0"!]#define PIT_IP_MDIS_BIT_EXISTS     (STD_OFF)[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32z27')"!][!//
        [!CODE!][!WS "0"!]#define PIT_IP_MDIS_BIT_EXISTS     (STD_OFF)[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r41') or contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 'saf8')"!][!//
        [!CODE!][!WS "0"!]#define PIT_IP_INSTANCE_GAP_EXISTS (STD_ON)[!CR!][!ENDCODE!][!//
        [!CODE!][!WS "0"!]#define PIT_IP_MDIS_BIT_EXISTS     (STD_ON)[!CR!][!ENDCODE!][!//
    [!ELSE!]
        [!CODE!][!WS "0"!]#define PIT_IP_INSTANCE_GAP_EXISTS (STD_OFF)[!CR!][!ENDCODE!][!//
        [!CODE!][!WS "0"!]#define PIT_IP_MDIS_BIT_EXISTS     (STD_ON)[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//

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

#ifdef __cplusplus
}
#endif

/** @} */
#endif  /* Pit_Ip_Cfg_Defines.h */
