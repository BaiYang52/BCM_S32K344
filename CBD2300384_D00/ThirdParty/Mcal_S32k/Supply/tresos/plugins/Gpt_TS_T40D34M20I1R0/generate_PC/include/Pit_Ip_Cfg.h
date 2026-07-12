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

#ifndef PIT_IP_CFG_H
#define PIT_IP_CFG_H

/**
*   @file           Pit_Ip_Cfg.h
*
*   @addtogroup     pit_ip Pit IPL
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
#include "Pit_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define PIT_IP_VENDOR_ID_CFG                    43
#define PIT_IP_AR_RELEASE_MAJOR_VERSION_CFG     4
#define PIT_IP_AR_RELEASE_MINOR_VERSION_CFG     4
#define PIT_IP_AR_RELEASE_REVISION_VERSION_CFG  0
#define PIT_IP_SW_MAJOR_VERSION_CFG             2
#define PIT_IP_SW_MINOR_VERSION_CFG             0
#define PIT_IP_SW_PATCH_VERSION_CFG             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#if (PIT_IP_VENDOR_ID_CFG != PIT_IP_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H)
    #error "Pit_Ip_Cfg.h and Pit_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h have different vendor ids"
#endif
/* Check if this header file and header file are of the same Autosar version */
#if ((PIT_IP_AR_RELEASE_MAJOR_VERSION_CFG != PIT_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) || \
     (PIT_IP_AR_RELEASE_MINOR_VERSION_CFG != PIT_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) || \
     (PIT_IP_AR_RELEASE_REVISION_VERSION_CFG != PIT_IP_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) \
    )
    #error "AutoSar Version Numbers of Pit_Ip_Cfg.h and Pit_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h are different"
#endif
/* Check if this header file and header file are of the same Software version */
#if ((PIT_IP_SW_MAJOR_VERSION_CFG != PIT_IP_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) || \
     (PIT_IP_SW_MINOR_VERSION_CFG != PIT_IP_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) || \
     (PIT_IP_SW_PATCH_VERSION_CFG != PIT_IP_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H) \
    )
    #error "Software Version Numbers of Pit_Ip_Cfg.h and Pit_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h are different"
#endif
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
/**
* @brief Dev error detect switch
*/
#define PIT_IP_DEV_ERROR_DETECT ([!IF "GptDriverConfiguration/GptDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*==================================================================================================*/
/**
* @brief    PIT_IP_CHAIN_MODE switch
* @details  Enable/disable API for Chain Mode.
*/
#define PIT_IP_CHAIN_MODE ([!IF "GptAutosarExt/ChainModeApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*================================================================================================*/
/**
* @brief    PIT_IP_CHANGE_NEXT_TIMEOUT_VALUE switch
* @details  Enable/disable support for changing timeout value during timer running
*/
#define PIT_IP_CHANGE_NEXT_TIMEOUT_VALUE     ([!IF "GptAutosarExt/GptChangeNextTimeoutValueApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*================================================================================================*/
/**
* @brief    PIT_IP_ENABLE_USER_MODE_SUPPORT switch
* @details  Enable/disable support usermode.If this parameter has been configured to 'TRUE' the GPT driver code can be executed from both supervisor and user mode.
*/
#define PIT_IP_ENABLE_USER_MODE_SUPPORT    ([!IF "GptAutosarExt/GptEnableUserModeSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    #ifdef PIT_IP_ENABLE_USER_MODE_SUPPORT
        #if (STD_ON == PIT_IP_ENABLE_USER_MODE_SUPPORT)
            #error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running Gpt in user mode the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined.
        #endif /* (STD_ON == PIT_IP_ENABLE_USER_MODE_SUPPORT) */
    #endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT */
#endif /* ifdef MCAL_ENABLE_USER_MODE_SUPPORT*/
/*================================================================================================*/
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
[!IF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k3')"!][!//
/**
*   @brief      PIT_IP_TIMEOUT_COUNTER
*   @details    This is a timeout value which is used to wait till PIT_RTI_LDVAL is synchronized into the RTI clock domain
*/
[!WS "0"!]#define PIT_IP_TIMEOUT_COUNTER  ([!"num:i(GptDriverConfiguration/GptTimeoutDuration)"!]UL)
[!WS "0"!]#define PIT_IP_TIMEOUT_TYPE     ([!"(GptDriverConfiguration/GptTimeoutMethod)"!])[!CR!]
[!ENDIF!][!//
[!ENDIF!][!//

[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
[!IF "(contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r41')) or (contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 'saf85'))"!][!//
[!CODE!][!WS"0"!]#if(defined (PIT_IP_INSTANCE_GAP_EXISTS) && (PIT_IP_INSTANCE_GAP_EXISTS == STD_ON))[!CR!][!ENDCODE!]
[!CODE!][!WS"0"!]#define PIT_INSTANCE_COUNT_ALT (PIT_INSTANCE_COUNT + 1U)[!CR!][!ENDCODE!][!//
[!CODE!][!WS"0"!]#endif[!CR!][!ENDCODE!]
[!ENDIF!][!//
[!ENDIF!][!//
/*================================================================================================*/
[!NOCODE!]
[!MACRO "CalculateModulesNumber","Module"!]
    [!IF "$Module = 'PIT_0'"!][!//
        [!VAR "ChannelsNumberPit0" = "count(text:grep(ecu:list('Gpt.GptChannelConfigSet.GptChannelConfiguration.GptHwChannel'),concat($Module,'_CH','.*')))"!][!//
    [!ELSEIF "$Module = 'PIT_1'"!][!//
        [!VAR "ChannelsNumberPit1" = "count(text:grep(ecu:list('Gpt.GptChannelConfigSet.GptChannelConfiguration.GptHwChannel'),concat($Module,'_CH','.*')))"!][!//
    [!ELSEIF "$Module = 'PIT_2'"!][!//
        [!VAR "ChannelsNumberPit2" = "count(text:grep(ecu:list('Gpt.GptChannelConfigSet.GptChannelConfiguration.GptHwChannel'),concat($Module,'_CH','.*')))"!][!//
    [!ELSEIF "$Module = 'PIT_3'"!][!//
        [!VAR "ChannelsNumberPit3" = "count(text:grep(ecu:list('Gpt.GptChannelConfigSet.GptChannelConfiguration.GptHwChannel'),concat($Module,'_CH','.*')))"!][!//
    [!ELSEIF "$Module = 'PIT_4'"!][!//
        [!VAR "ChannelsNumberPit4" = "count(text:grep(ecu:list('Gpt.GptChannelConfigSet.GptChannelConfiguration.GptHwChannel'),concat($Module,'_CH','.*')))"!][!//
    [!ELSEIF "$Module = 'PIT_5'"!][!//
        [!VAR "ChannelsNumberPit5" = "count(text:grep(ecu:list('Gpt.GptChannelConfigSet.GptChannelConfiguration.GptHwChannel'),concat($Module,'_CH','.*')))"!][!//
    [!ELSEIF "$Module = 'CE_PIT_0'"!][!//
        [!VAR "ChannelsNumberPitCE0" = "count(text:grep(ecu:list('Gpt.GptChannelConfigSet.GptChannelConfiguration.GptHwChannel'),concat($Module,'_CH','.*')))"!][!//
    [!ELSEIF "$Module = 'CE_PIT_1'"!][!//
        [!VAR "ChannelsNumberPitCE1" = "count(text:grep(ecu:list('Gpt.GptChannelConfigSet.GptChannelConfiguration.GptHwChannel'),concat($Module,'_CH','.*')))"!][!//
    [!ELSEIF "$Module = 'CE_PIT_2'"!][!//
        [!VAR "ChannelsNumberPitCE2" = "count(text:grep(ecu:list('Gpt.GptChannelConfigSet.GptChannelConfiguration.GptHwChannel'),concat($Module,'_CH','.*')))"!][!//
    [!ELSEIF "$Module = 'CE_PIT_3'"!][!//
        [!VAR "ChannelsNumberPitCE3" = "count(text:grep(ecu:list('Gpt.GptChannelConfigSet.GptChannelConfiguration.GptHwChannel'),concat($Module,'_CH','.*')))"!][!//
    [!ELSEIF "$Module = 'CE_PIT_4'"!][!//
        [!VAR "ChannelsNumberPitCE4" = "count(text:grep(ecu:list('Gpt.GptChannelConfigSet.GptChannelConfiguration.GptHwChannel'),concat($Module,'_CH','.*')))"!][!//
    [!ELSEIF "$Module = 'CE_PIT_5'"!][!//
        [!VAR "ChannelsNumberPitCE5" = "count(text:grep(ecu:list('Gpt.GptChannelConfigSet.GptChannelConfiguration.GptHwChannel'),concat($Module,'_CH','.*')))"!][!//
    [!ELSEIF "$Module = 'RTU_PIT_0'"!][!//
        [!VAR "ChannelsNumberPitRTU0" = "count(text:grep(ecu:list('Gpt.GptChannelConfigSet.GptChannelConfiguration.GptHwChannel'),concat($Module,'_CH','.*')))"!][!//
    [!ELSEIF "$Module = 'RTU_PIT_1'"!][!//
        [!VAR "ChannelsNumberPitRTU1" = "count(text:grep(ecu:list('Gpt.GptChannelConfigSet.GptChannelConfiguration.GptHwChannel'),concat($Module,'_CH','.*')))"!][!//
    [!ENDIF!]
[!ENDMACRO!]
[!ENDNOCODE!]
/*================================================================================================*/
[!NOCODE!]
[!CALL "CalculateModulesNumber","Module"="string('PIT_0')"!][!//
[!IF "num:i($ChannelsNumberPit0) > 0"!][!//
[!CODE!][!//
/** Channels number instance PIT_0 */
#define PIT_0_IP_CHANNELS_NUMBER  ([!"num:i($ChannelsNumberPit0)"!]U)
#define PIT_0_IP_EXISTS
[!ENDCODE!][!//
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!IF "(contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32e27')) or (contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32z27'))"!][!//
        [!CODE!][!WS "0"!]#define PIT_0_IP_INSTANCE_NUMBER     (6U)[!CR!][!ENDCODE!][!//
    [!ELSE!][!//
        [!CODE!][!WS "0"!]#define PIT_0_IP_INSTANCE_NUMBER     (0U)[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!CALL "CalculateModulesNumber","Module"="string('PIT_1')"!][!//
[!IF "num:i($ChannelsNumberPit1) > 0"!][!//
[!CODE!][!//
/** Channels number instance PIT_1 */
#define PIT_1_IP_CHANNELS_NUMBER  ([!"num:i($ChannelsNumberPit1)"!]U)
#define PIT_1_IP_EXISTS
[!ENDCODE!][!//
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!IF "(contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32e27')) or (contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32z27'))"!][!//
        [!CODE!][!WS "0"!]#define PIT_1_IP_INSTANCE_NUMBER     (7U)[!CR!][!ENDCODE!][!//
    [!ELSE!][!//
        [!CODE!][!WS "0"!]#define PIT_1_IP_INSTANCE_NUMBER     (1U)[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!CALL "CalculateModulesNumber","Module"="string('PIT_2')"!][!//
[!IF "num:i($ChannelsNumberPit2) > 0"!][!//
[!CODE!][!//
/** Channels number instance PIT_2 */
#define PIT_2_IP_CHANNELS_NUMBER  ([!"num:i($ChannelsNumberPit2)"!]U)
#define PIT_2_IP_EXISTS
#define PIT_2_IP_INSTANCE_NUMBER    (2U)
[!ENDCODE!][!//
[!ENDIF!][!//
[!CALL "CalculateModulesNumber","Module"="string('PIT_3')"!][!//
[!IF "num:i($ChannelsNumberPit3) > 0"!][!//
[!CODE!][!//
/** Channels number instance PIT_3 */
#define PIT_3_IP_CHANNELS_NUMBER  ([!"num:i($ChannelsNumberPit3)"!]U)
#define PIT_3_IP_EXISTS
#define PIT_3_IP_INSTANCE_NUMBER    (3U)
[!ENDCODE!][!//
[!ENDIF!][!//
[!CALL "CalculateModulesNumber","Module"="string('PIT_4')"!][!//
[!IF "num:i($ChannelsNumberPit4) > 0"!][!//
[!CODE!][!//
/** Channels number instance PIT_4 */
#define PIT_4_IP_CHANNELS_NUMBER  ([!"num:i($ChannelsNumberPit4)"!]U)
#define PIT_4_IP_EXISTS
[!ENDCODE!][!//
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!IF "(contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32e27')) or (contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32z27'))"!][!//
        [!CODE!][!WS "0"!]#define PIT_4_IP_INSTANCE_NUMBER     (8U)[!CR!][!ENDCODE!][!//
    [!ELSE!][!//
        [!CODE!][!WS "0"!]#define PIT_4_IP_INSTANCE_NUMBER     (4U)[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!CALL "CalculateModulesNumber","Module"="string('PIT_5')"!][!//
[!IF "num:i($ChannelsNumberPit5) > 0"!][!//
[!CODE!][!//
/** Channels number instance PIT_5 */
#define PIT_5_IP_CHANNELS_NUMBER  ([!"num:i($ChannelsNumberPit5)"!]U)
#define PIT_5_IP_EXISTS
[!ENDCODE!][!//
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!IF "(contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32e27')) or (contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32z27'))"!][!//
        [!CODE!][!WS "0"!]#define PIT_5_IP_INSTANCE_NUMBER     (9U)[!CR!][!ENDCODE!][!//
    [!ELSE!][!//
        [!CODE!][!WS "0"!]#define PIT_5_IP_INSTANCE_NUMBER     (5U)[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!CALL "CalculateModulesNumber","Module"="string('CE_PIT_0')"!][!//
[!IF "num:i($ChannelsNumberPitCE0) > 0"!][!//
[!CODE!][!//
/** Channels number instance CE_PIT_0 */
#define CE_PIT_0_IP_CHANNELS_NUMBER  ([!"num:i($ChannelsNumberPitCE0)"!]U)
#define CE_PIT_0_IP_EXISTS
#define CE_PIT_0_IP_INSTANCE_NUMBER    (0U)
[!ENDCODE!][!//
[!ENDIF!][!//
[!CALL "CalculateModulesNumber","Module"="string('CE_PIT_1')"!][!//
[!IF "num:i($ChannelsNumberPitCE1) > 0"!][!//
[!CODE!][!//
/** Channels number instance CE_PIT_1 */
#define CE_PIT_1_IP_CHANNELS_NUMBER  ([!"num:i($ChannelsNumberPitCE1)"!]U)
#define CE_PIT_1_IP_EXISTS
#define CE_PIT_1_IP_INSTANCE_NUMBER    (1U)
[!ENDCODE!][!//
[!ENDIF!][!//
[!CALL "CalculateModulesNumber","Module"="string('CE_PIT_2')"!][!//
[!IF "num:i($ChannelsNumberPitCE2) > 0"!][!//
[!CODE!][!//
/** Channels number instance CE_PIT_2 */
#define CE_PIT_2_IP_CHANNELS_NUMBER  ([!"num:i($ChannelsNumberPitCE2)"!]U)
#define CE_PIT_2_IP_EXISTS
#define CE_PIT_2_IP_INSTANCE_NUMBER    (2U)
[!ENDCODE!][!//
[!ENDIF!][!//
[!CALL "CalculateModulesNumber","Module"="string('CE_PIT_3')"!][!//
[!IF "num:i($ChannelsNumberPitCE3) > 0"!][!//
[!CODE!][!//
/** Channels number instance CE_PIT_3 */
#define CE_PIT_3_IP_CHANNELS_NUMBER  ([!"num:i($ChannelsNumberPitCE3)"!]U)
#define CE_PIT_3_IP_EXISTS
#define CE_PIT_3_IP_INSTANCE_NUMBER    (3U)
[!ENDCODE!][!//
[!ENDIF!][!//
[!CALL "CalculateModulesNumber","Module"="string('CE_PIT_4')"!][!//
[!IF "num:i($ChannelsNumberPitCE4) > 0"!][!//
[!CODE!][!//
/** Channels number instance CE_PIT_4 */
#define CE_PIT_4_IP_CHANNELS_NUMBER  ([!"num:i($ChannelsNumberPitCE4)"!]U)
#define CE_PIT_4_IP_EXISTS
#define CE_PIT_4_IP_INSTANCE_NUMBER    (4U)
[!ENDCODE!][!//
[!ENDIF!][!//
[!CALL "CalculateModulesNumber","Module"="string('CE_PIT_5')"!][!//
[!IF "num:i($ChannelsNumberPitCE5) > 0"!][!//
[!CODE!][!//
/** Channels number instance CE_PIT_5 */
#define CE_PIT_5_IP_CHANNELS_NUMBER  ([!"num:i($ChannelsNumberPitCE5)"!]U)
#define CE_PIT_5_IP_EXISTS
#define CE_PIT_5_IP_INSTANCE_NUMBER    (5U)
[!ENDCODE!][!//
[!ENDIF!][!//
[!CALL "CalculateModulesNumber","Module"="string('RTU_PIT_0')"!][!//
[!IF "num:i($ChannelsNumberPitRTU0) > 0"!][!//
[!CODE!][!//
/** Channels number instance RTU_PIT_0 */
#define RTU_PIT_0_IP_CHANNELS_NUMBER  ([!"num:i($ChannelsNumberPitRTU0)"!]U)
#define RTU_PIT_0_IP_EXISTS
#define RTU_PIT_0_IP_INSTANCE_NUMBER    (10U)
[!ENDCODE!][!//
[!ENDIF!][!//
[!CALL "CalculateModulesNumber","Module"="string('RTU_PIT_1')"!][!//
[!IF "num:i($ChannelsNumberPitRTU1) > 0"!][!//
[!CODE!][!//
/** Channels number instance RTU_PIT_1 */
#define RTU_PIT_1_IP_CHANNELS_NUMBER  ([!"num:i($ChannelsNumberPitRTU1)"!]U)
#define RTU_PIT_1_IP_EXISTS
#define RTU_PIT_1_IP_INSTANCE_NUMBER    (11U)
[!ENDCODE!][!//
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
#endif  /* PIT_IP_CFG_H */
