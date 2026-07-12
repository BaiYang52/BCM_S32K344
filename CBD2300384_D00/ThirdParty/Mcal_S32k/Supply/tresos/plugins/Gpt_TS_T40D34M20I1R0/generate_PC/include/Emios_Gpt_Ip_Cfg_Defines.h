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

#ifndef EMIOS_GPT_IP_CFG_DEFINE_H
#define EMIOS_GPT_IP_CFG_DEFINE_H

/**
*   @file           Emios_Gpt_Ip_Cfg_Defines.h
*
*   @addtogroup     emios_ip Emios IPL
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
    [!CODE!][!WS "0"!]/* Include platform header file. */[!CR!][!ENDCODE!][!//
    [!IF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k341')"!][!//
        [!CODE!][!WS "0"!]#include "S32K342_EMIOS.h"[!CR!][!ENDCODE!][!//
    [!ELSE!][!//
        [!CODE!][!WS "0"!]#include "[!"$DerivativeName"!]_EMIOS.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ELSE!][!//
    [!ERROR "The Resource plugins or ResourceSubderivative field is not available for header file inclusion."!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define EMIOS_GPT_IP_CFG_DEFINES_VENDOR_ID                    43
#define EMIOS_GPT_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION     4
#define EMIOS_GPT_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION     4
#define EMIOS_GPT_IP_CFG_DEFINES_AR_RELEASE_REVISION_VERSION  0
#define EMIOS_GPT_IP_CFG_DEFINES_SW_MAJOR_VERSION             2
#define EMIOS_GPT_IP_CFG_DEFINES_SW_MINOR_VERSION             0
#define EMIOS_GPT_IP_CFG_DEFINES_SW_PATCH_VERSION             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if header file and StandardTypes.h file are of the same Autosar version */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((EMIOS_GPT_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
         (EMIOS_GPT_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Emios_Gpt_Ip_Cfg_Defines.h and StandardTypes.h are different"
    #endif
#endif
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
[!NOCODE!][!//
[!VAR "MacEmiosUsed"="0"!][!//
    // Loop on all channels inside the same Configuration Set
    [!LOOP "GptHwConfiguration/*"!][!//
        [!IF "node:value(GptChannelIsUsed) = 'true' and contains(GptIsrHwId, 'EMIOS')"!][!//
            [!VAR "MacEmiosUsed"="1"!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDNOCODE!][!//

/**
* @brief These defines indicate that at least one channel from each module is used in all configurations.
*/
#define EMIOS_GPT_IP_USED ([!IF "$MacEmiosUsed=1"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
/*================================================================================================*/
/**
* @brief    EMIOS_GPT_IP_SET_CLOCK_MODE switch
* @details  Enable/disable API for Dual Mode support.
*/
#define EMIOS_GPT_IP_SET_CLOCK_MODE           ([!IF "GptAutosarExt/GptEnableDualClockMode"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
/**
*
* @brief IRQ Defines for each channel used
*/
[!NOCODE!][!//
[!LOOP "GptHwConfiguration/*"!][!//
    [!IF "(GptChannelIsUsed = 'true') and (GptIsrEnable = 'true')"!][!//
        [!IF "contains(GptIsrHwId,"EMIOS")"!][!//
                [!CODE!][!//
#define GPT_[!"GptIsrHwId"!]_ISR_USED
                [!ENDCODE!][!//
        [!ENDIF!][!//
    [!ENDIF!]//
[!ENDLOOP!][!//
[!ENDNOCODE!][!//

[!NOCODE!][!//
[!CODE!]/* Macros that indicate EMIOS interrupts used by GPT. */[!CR!][!ENDCODE!][!//
    [!LOOP "GptHwConfiguration/*"!][!//
        [!IF "(node:value(./GptIsrEnable) = 'true') and (text:split(node:value(./GptIsrHwId), '_')[1] = 'EMIOS')"!][!//
            [!CODE!][!WS "4"!]#ifndef [!"node:value(./GptIsrHwId)"!]_ISR_USED[!CR!][!ENDCODE!][!//
            [!CODE!][!WS "8"!]#define GPT_[!"node:value(./GptIsrHwId)"!]_ISR_USED[!CR!][!ENDCODE!][!//
            [!CODE!][!WS "8"!]#define [!"node:value(./GptIsrHwId)"!]_ISR_USED[!CR!][!ENDCODE!][!//
            [!CODE!][!WS "4"!]#else[!CR!][!ENDCODE!][!//
            [!CODE!][!WS "8"!]#error "[!"node:value(./GptIsrHwId)"!] interrupt cannot be used by Gpt driver. Channel locked by other driver!"[!CR!][!ENDCODE!][!//
            [!CODE!][!WS "4"!]#endif[!CR!][!CR!][!ENDCODE!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDNOCODE!][!//

/*================================================================================================*/

/*================================================================================================*/

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

#endif  /* EMIOS_GPT_IP_CFG_DEFINE_H */
