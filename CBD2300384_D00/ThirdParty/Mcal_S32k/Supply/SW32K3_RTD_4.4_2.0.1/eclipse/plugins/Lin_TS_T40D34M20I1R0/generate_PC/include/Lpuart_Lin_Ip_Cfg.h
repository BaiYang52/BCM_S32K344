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
 
#ifndef LPUART_LIN_IP_CFG_H
#define LPUART_LIN_IP_CFG_H

/**
*   @file
*
*   @addtogroup LPUART_LIN
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
* 4) user callback header files
==================================================================================================*/
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!]#include "Lpuart_Lin_Ip_[!"."!]_PBcfg.h"[!CR!][!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!]#include "Lpuart_Lin_Ip_PBcfg.h"[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define LPUART_LIN_IP_CFG_VENDOR_ID                    43
#define LPUART_LIN_IP_CFG_AR_RELEASE_MAJOR_VERSION     4
#define LPUART_LIN_IP_CFG_AR_RELEASE_MINOR_VERSION     4
#define LPUART_LIN_IP_CFG_AR_RELEASE_REVISION_VERSION  0
#define LPUART_LIN_IP_CFG_SW_MAJOR_VERSION             2
#define LPUART_LIN_IP_CFG_SW_MINOR_VERSION             0
#define LPUART_LIN_IP_CFG_SW_PATCH_VERSION             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!][!//
/* Checks against Lpuart_Lin_Ip_[!"."!]_PBcfg.h */
#if (LPUART_LIN_IP_CFG_VENDOR_ID != LPUART_LIN_IP_[!"."!]_PBCFG_VENDOR_ID)
    #error "Lpuart_Lin_Ip_Cfg.h and Lpuart_Lin_Ip_[!"."!]_PBcfg.h have different vendor ids"
#endif
#if ((LPUART_LIN_IP_CFG_AR_RELEASE_MAJOR_VERSION      != LPUART_LIN_IP_[!"."!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (LPUART_LIN_IP_CFG_AR_RELEASE_MINOR_VERSION      != LPUART_LIN_IP_[!"."!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (LPUART_LIN_IP_CFG_AR_RELEASE_REVISION_VERSION   != LPUART_LIN_IP_[!"."!]_PBCFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Lpuart_Lin_Ip_Cfg.h and Lpuart_Lin_Ip_[!"."!]_PBcfg.h are different"
#endif
#if ((LPUART_LIN_IP_CFG_SW_MAJOR_VERSION != LPUART_LIN_IP_[!"."!]_PBCFG_SW_MAJOR_VERSION) || \
     (LPUART_LIN_IP_CFG_SW_MINOR_VERSION != LPUART_LIN_IP_[!"."!]_PBCFG_SW_MINOR_VERSION) || \
     (LPUART_LIN_IP_CFG_SW_PATCH_VERSION != LPUART_LIN_IP_[!"."!]_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Lpuart_Lin_Ip_Cfg.h and Lpuart_Lin_Ip_[!"."!]_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!][!//
/* Checks against Lpuart_Lin_Ip_PBcfg.h */
#if (LPUART_LIN_IP_CFG_VENDOR_ID != LPUART_LIN_IP_PBCFG_VENDOR_ID)
    #error "Lpuart_Lin_Ip_Cfg.h and Lpuart_Lin_Ip_PBcfg.h have different vendor ids"
#endif
#if ((LPUART_LIN_IP_CFG_AR_RELEASE_MAJOR_VERSION      != LPUART_LIN_IP_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (LPUART_LIN_IP_CFG_AR_RELEASE_MINOR_VERSION      != LPUART_LIN_IP_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (LPUART_LIN_IP_CFG_AR_RELEASE_REVISION_VERSION   != LPUART_LIN_IP_PBCFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Lpuart_Lin_Ip_Cfg.h and Lpuart_Lin_Ip_PBcfg.h are different"
#endif
#if ((LPUART_LIN_IP_CFG_SW_MAJOR_VERSION != LPUART_LIN_IP_PBCFG_SW_MAJOR_VERSION) || \
     (LPUART_LIN_IP_CFG_SW_MINOR_VERSION != LPUART_LIN_IP_PBCFG_SW_MINOR_VERSION) || \
     (LPUART_LIN_IP_CFG_SW_PATCH_VERSION != LPUART_LIN_IP_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Lpuart_Lin_Ip_Cfg.h and Lpuart_Lin_Ip_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDIF!] 
[!ENDNOCODE!]

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
 *                                     DEFINES AND MACROS
==================================================================================================*/
[!NOCODE!]
[!CODE!]#define LPUART_LIN_IP_CONFIG_EXT \[!CR!][!ENDCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!][!//
    [!CODE!][!WS "4"!]LPUART_LIN_IP_CONFIG_[!"."!]_PB \[!CR!][!ENDCODE!]
[!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS "4"!]LPUART_LIN_IP_CONFIG_PB[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]
/*==================================================================================================
*                                            ENUMS
==================================================================================================*/

/*==================================================================================================
*                               STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

#define LIN_START_SEC_CODE
#include "Lin_MemMap.h"

[!NOCODE!]
[!IF "node:fallback(AutosarExt/LinFrameTimeoutDisable, 'false') = 'false'"!]
    [!IF "node:exists(AutosarExt/LinLpuartStartTimerNotification)"!]
        [!IF "node:value(AutosarExt/LinLpuartStartTimerNotification) != 'NULL_PTR'"!]
            [!CODE!]

extern void [!"AutosarExt/LinLpuartStartTimerNotification"!](uint8 Instance, uint32 MicroSeconds);

[!ENDCODE!]
        [!ENDIF!]
    [!ENDIF!]
[!IF "node:exists(AutosarExt/LinLpuartStopTimerNotification)"!]
        [!IF "node:value(AutosarExt/LinLpuartStopTimerNotification) != 'NULL_PTR'"!]
            [!CODE!]

extern void [!"AutosarExt/LinLpuartStopTimerNotification"!](uint8 Instance);

[!ENDCODE!]
        [!ENDIF!]
    [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!]

extern void Lin_Ipw_Callback(const uint8 Instance, const Lpuart_Lin_Ip_StateStructType *LpuartStateStruct);

#define LIN_STOP_SEC_CODE
#include "Lin_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

#endif 
