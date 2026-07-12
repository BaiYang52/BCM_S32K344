/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : FLEXIO
*   Dependencies         : 
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

#ifndef UART_IPW_CFG_H
#define UART_IPW_CFG_H

/**
*   @file
*   @internal
*   @addtogroup UART_IPW_DRIVER
*   @{
*/

#ifdef __cplusplus
extern "C"{
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
[!CODE!]#include "Uart_Ipw_[!"."!]_PBcfg.h"[!CR!][!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!]#include "Uart_Ipw_PBcfg.h"[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]
#include "Uart_Ipw_Types.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define UART_IPW_CFG_VENDOR_ID                    43
#define UART_IPW_CFG_AR_RELEASE_MAJOR_VERSION     4
#define UART_IPW_CFG_AR_RELEASE_MINOR_VERSION     4
#define UART_IPW_CFG_AR_RELEASE_REVISION_VERSION  0
#define UART_IPW_CFG_SW_MAJOR_VERSION             2
#define UART_IPW_CFG_SW_MINOR_VERSION             0
#define UART_IPW_CFG_SW_PATCH_VERSION             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!][!//
/* Checks against Uart_Ipw_[!"."!]_PBcfg.h */
#if (UART_IPW_CFG_VENDOR_ID != UART_IPW_PBCFG_[!"."!]_VENDOR_ID)
    #error "Uart_Ipw_Cfg.h and Uart_Ipw_[!"."!]_PBcfg.h have different vendor ids"
#endif
#if ((UART_IPW_CFG_AR_RELEASE_MAJOR_VERSION    != UART_IPW_PBCFG_[!"."!]_AR_RELEASE_MAJOR_VERSION) || \
     (UART_IPW_CFG_AR_RELEASE_MINOR_VERSION    != UART_IPW_PBCFG_[!"."!]_AR_RELEASE_MINOR_VERSION) || \
     (UART_IPW_CFG_AR_RELEASE_REVISION_VERSION != UART_IPW_PBCFG_[!"."!]_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Uart_Ipw_Cfg.h and Uart_Ipw_[!"."!]_PBcfg.h are different"
#endif
#if ((UART_IPW_CFG_SW_MAJOR_VERSION != UART_IPW_PBCFG_[!"."!]_SW_MAJOR_VERSION) || \
     (UART_IPW_CFG_SW_MINOR_VERSION != UART_IPW_PBCFG_[!"."!]_SW_MINOR_VERSION) || \
     (UART_IPW_CFG_SW_PATCH_VERSION != UART_IPW_PBCFG_[!"."!]_SW_PATCH_VERSION))
    #error "Software Version Numbers of Uart_Ipw_Cfg.h and Uart_Ipw_[!"."!]_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!][!//
/* Checks against Uart_Ipw_PBcfg.h */
#if (UART_IPW_CFG_VENDOR_ID != UART_IPW_PBCFG_VENDOR_ID)
    #error "Uart_Ipw_Cfg.h and Uart_Ipw_PBcfg.h have different vendor ids"
#endif
#if ((UART_IPW_CFG_AR_RELEASE_MAJOR_VERSION    != UART_IPW_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (UART_IPW_CFG_AR_RELEASE_MINOR_VERSION    != UART_IPW_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (UART_IPW_CFG_AR_RELEASE_REVISION_VERSION != UART_IPW_PBCFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Uart_Ipw_Cfg.h and Uart_Ipw_PBcfg.h are different"
#endif
#if ((UART_IPW_CFG_SW_MAJOR_VERSION != UART_IPW_PBCFG_SW_MAJOR_VERSION) || \
     (UART_IPW_CFG_SW_MINOR_VERSION != UART_IPW_PBCFG_SW_MINOR_VERSION) || \
     (UART_IPW_CFG_SW_PATCH_VERSION != UART_IPW_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Uart_Ipw_Cfg.h and Uart_Ipw_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]
/*Checks against Uart_Ipw_Types.h */
#if (UART_IPW_CFG_VENDOR_ID!= UART_IPW_TYPES_VENDOR_ID)
    #error "Uart_Ipw_Cfg.h and Uart_Ipw_Types.h have different vendor ids"
#endif
#if ((UART_IPW_CFG_AR_RELEASE_MAJOR_VERSION   != UART_IPW_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (UART_IPW_CFG_AR_RELEASE_MINOR_VERSION   != UART_IPW_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (UART_IPW_CFG_AR_RELEASE_REVISION_VERSION!= UART_IPW_TYPES_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Uart_Ipw_Cfg.h and Uart_Ipw_Types.h are different"
#endif
#if ((UART_IPW_CFG_SW_MAJOR_VERSION!= UART_IPW_TYPES_SW_MAJOR_VERSION) || \
     (UART_IPW_CFG_SW_MINOR_VERSION!= UART_IPW_TYPES_SW_MINOR_VERSION) || \
     (UART_IPW_CFG_SW_PATCH_VERSION!= UART_IPW_TYPES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Uart_Ipw_Cfg.h and Uart_Ipw_Types.h are different"
#endif
/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

[!NOCODE!]
[!CODE!]#define UART_IPW_CONFIG_EXT \[!CR!][!ENDCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!][!//
    [!CODE!][!WS "4"!]UART_IPW_CONFIG_[!"."!]_PB \[!CR!][!ENDCODE!]
[!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS "4"!]UART_IPW_CONFIG_PB[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]
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
[!NOCODE!]
[!IF "GeneralConfiguration/UartCallbackCapability"!]
    [!IF "(node:exists(GeneralConfiguration/UartCallback))"!]
        [!IF "node:value(GeneralConfiguration/UartCallback) != 'NULL_PTR'"!]
            [!CODE!]

/**
* @brief    Declare callback functions if any
*/

#define UART_START_SEC_CODE
#include "Uart_MemMap.h"

/* Define User Receive callback function */
extern void [!"GeneralConfiguration/UartCallback"!](uint8 HwInstance, Uart_EventType Event);

#define UART_STOP_SEC_CODE
#include "Uart_MemMap.h"

            [!ENDCODE!]
        [!ENDIF!]
    [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!]

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* UART_IPW_CFG_H */
