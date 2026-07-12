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

#ifndef LIN_IPW_CFG_H
#define LIN_IPW_CFG_H

/**
*   @file
*   
*   @internal 
*   @addtogroup LIN_IPW
*   @{
*/

#ifdef __cplusplus
extern "C"
{
#endif

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/

[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!]#include "Lin_Ipw_[!"."!]_PBcfg.h"[!CR!][!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!]#include "Lin_Ipw_PBcfg.h"[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]
#include "Lin_Ipw_Types.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define LIN_IPW_CFG_VENDOR_ID                    43
#define LIN_IPW_CFG_AR_RELEASE_MAJOR_VERSION     4
#define LIN_IPW_CFG_AR_RELEASE_MINOR_VERSION     4
#define LIN_IPW_CFG_AR_RELEASE_REVISION_VERSION  0
#define LIN_IPW_CFG_SW_MAJOR_VERSION             2
#define LIN_IPW_CFG_SW_MINOR_VERSION             0
#define LIN_IPW_CFG_SW_PATCH_VERSION             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!][!//
/* Checks against Lin_Ipw_[!"."!]_PBcfg.h */
#if (LIN_IPW_CFG_VENDOR_ID != LIN_IPW_[!"."!]_PBCFG_VENDOR_ID)
    #error "Lin_Ipw_Cfg.h and Lin_Ipw_[!"."!]_PBcfg.h have different vendor ids"
#endif
#if ((LIN_IPW_CFG_AR_RELEASE_MAJOR_VERSION    != LIN_IPW_[!"."!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (LIN_IPW_CFG_AR_RELEASE_MINOR_VERSION    != LIN_IPW_[!"."!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (LIN_IPW_CFG_AR_RELEASE_REVISION_VERSION != LIN_IPW_[!"."!]_PBCFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Lin_Ipw_Cfg.h and Lin_Ipw_[!"."!]_PBcfg.h are different"
#endif
#if ((LIN_IPW_CFG_SW_MAJOR_VERSION != LIN_IPW_[!"."!]_PBCFG_SW_MAJOR_VERSION) || \
     (LIN_IPW_CFG_SW_MINOR_VERSION != LIN_IPW_[!"."!]_PBCFG_SW_MINOR_VERSION) || \
     (LIN_IPW_CFG_SW_PATCH_VERSION != LIN_IPW_[!"."!]_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Lin_Ipw_Cfg.h and Lin_Ipw_[!"."!]_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!][!//
/* Checks against Lin_Ipw_PBcfg.h */
#if (LIN_IPW_CFG_VENDOR_ID != LIN_IPW_PBCFG_VENDOR_ID)
    #error "Lin_Ipw_Cfg.h and Lin_Ipw_PBcfg.h have different vendor ids"
#endif
#if ((LIN_IPW_CFG_AR_RELEASE_MAJOR_VERSION    != LIN_IPW_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (LIN_IPW_CFG_AR_RELEASE_MINOR_VERSION    != LIN_IPW_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (LIN_IPW_CFG_AR_RELEASE_REVISION_VERSION != LIN_IPW_PBCFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Lin_Ipw_Cfg.h and Lin_Ipw_PBcfg.h are different"
#endif
#if ((LIN_IPW_CFG_SW_MAJOR_VERSION != LIN_IPW_PBCFG_SW_MAJOR_VERSION) || \
     (LIN_IPW_CFG_SW_MINOR_VERSION != LIN_IPW_PBCFG_SW_MINOR_VERSION) || \
     (LIN_IPW_CFG_SW_PATCH_VERSION != LIN_IPW_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Lin_Ipw_Cfg.h and Lin_Ipw_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

/* Checks against Lin_Ipw_Types.h */
#if (LIN_IPW_CFG_VENDOR_ID != LIN_IPW_TYPES_VENDOR_ID)
    #error "Lin_Ipw_Cfg.h and Lin_Ipw_Types.h have different vendor ids"
#endif
#if ((LIN_IPW_CFG_AR_RELEASE_MAJOR_VERSION    != LIN_IPW_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (LIN_IPW_CFG_AR_RELEASE_MINOR_VERSION    != LIN_IPW_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (LIN_IPW_CFG_AR_RELEASE_REVISION_VERSION != LIN_IPW_TYPES_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Lin_Ipw_Cfg.h and Lin_Ipw_Types.h are different"
#endif
#if ((LIN_IPW_CFG_SW_MAJOR_VERSION != LIN_IPW_TYPES_SW_MAJOR_VERSION) || \
     (LIN_IPW_CFG_SW_MINOR_VERSION != LIN_IPW_TYPES_SW_MINOR_VERSION) || \
     (LIN_IPW_CFG_SW_PATCH_VERSION != LIN_IPW_TYPES_SW_PATCH_VERSION))
    #error "Software Version Numbers of Lin_Ipw_Cfg.h and Lin_Ipw_Types.h are different"
#endif
/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
[!NOCODE!]
[!CODE!]#define LIN_IPW_CONFIG_EXT \[!CR!][!ENDCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!][!//
    [!CODE!][!WS "4"!]LIN_IPW_CONFIG_[!"."!]_PB \[!CR!][!ENDCODE!]
[!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS "4"!]LIN_IPW_CONFIG_PB[!ENDCODE!]
[!ENDIF!]
[!VAR "AllLinChannelEnableEcuMWakeupSource" = "'true'"!]
[!LOOP "LinGlobalConfig/LinChannel/*"!]
    [!IF "not(node:exists(LinChannelEcuMWakeupSource))"!]
        [!VAR "AllLinChannelEnableEcuMWakeupSource" = "'false'"!]
    [!ENDIF!]    
[!ENDLOOP!]
[!IF "$AllLinChannelEnableEcuMWakeupSource = 'false'"!]
[!CODE!]
#if (LIN_WAKEUP_DETECTION == STD_ON)

/**
* @brief          None EcuMWakeUpSource was referrd when LinChannelWakeupSupport is disable
*/
#define LIN_NONE_ECUM_WAKEUP_SOURCE_REF     (0UL)
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]
/**
* 
* @internal 
* @brief          Link Lin channels symbolic names with Lin hardware channel IDs.
* @details        Link Lin channels symbolic names with Lin hardware channel IDs.
*
* 
*/
[!FOR "index" = "0" TO "num:i(ecu:get('Lin.LinGlobalConfig.LinChannel'))-1"!][!//
    [!VAR "MaxHwChannels" = "string((ecu:list('Lin.LinGlobalConfig.LinChannel.LinHwChannel'))[$index+1])"!][!//
    [!VAR "ChannelFound" = "0"!][!//
    [!LOOP "LinGlobalConfig/LinChannel/*"!][!//
        [!VAR "HardwareChannel" = "string(LinHwChannel)"!][!//
        [!IF "$MaxHwChannels = $HardwareChannel"!][!//
            [!IF "$ChannelFound = 0"!][!//

#define [!"$MaxHwChannels"!]    ((uint8)[!"substring-after(LinHwChannel,'IP_')"!]U)[!//

                [!VAR "ChannelFound" = "1"!][!//
            [!ENDIF!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDFOR!][!//

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
*                                       GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* LIN_IPW_CFG_H */
