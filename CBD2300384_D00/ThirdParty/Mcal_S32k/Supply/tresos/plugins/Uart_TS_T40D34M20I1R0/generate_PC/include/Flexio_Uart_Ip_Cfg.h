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

#ifndef FLEXIO_UART_IP_CFG_H
#define FLEXIO_UART_IP_CFG_H

/**
*   @file
*   @defgroup flexio_uart_ip Flexio UART IPL
*   @addtogroup  flexio_uart_ip Flexio UART IPL
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
[!CODE!]#include "Flexio_Uart_Ip_[!"."!]_PBCfg.h"[!CR!][!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!]#include "Flexio_Uart_Ip_PBcfg.h"[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]
#include "StandardTypes.h"
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLEXIO_UART_IP_CFG_VENDOR_ID                     43
#define FLEXIO_UART_IP_CFG_AR_RELEASE_MAJOR_VERSION      4
#define FLEXIO_UART_IP_CFG_AR_RELEASE_MINOR_VERSION      4
#define FLEXIO_UART_IP_CFG_AR_RELEASE_REVISION_VERSION   0
#define FLEXIO_UART_IP_CFG_SW_MAJOR_VERSION              2
#define FLEXIO_UART_IP_CFG_SW_MINOR_VERSION              0
#define FLEXIO_UART_IP_CFG_SW_PATCH_VERSION              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!][!//
/* Checks against Flexio_Uart_Ip_[!"."!]_PBCfg.h */
#if (FLEXIO_UART_IP_CFG_VENDOR_ID != FLEXIO_UART_IP_PBCFG_[!"."!]_VENDOR_ID)
    #error "Flexio_Uart_Ip_Cfg.h and Flexio_Uart_Ip_[!"."!]_PBCfg.h have different vendor ids"
#endif
#if ((FLEXIO_UART_IP_CFG_AR_RELEASE_MAJOR_VERSION    != FLEXIO_UART_IP_PBCFG_[!"."!]_AR_RELEASE_MAJOR_VERSION) || \
     (FLEXIO_UART_IP_CFG_AR_RELEASE_MINOR_VERSION    != FLEXIO_UART_IP_PBCFG_[!"."!]_AR_RELEASE_MINOR_VERSION) || \
     (FLEXIO_UART_IP_CFG_AR_RELEASE_REVISION_VERSION != FLEXIO_UART_IP_PBCFG_[!"."!]_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Flexio_Uart_Ip_Cfg.h and Flexio_Uart_Ip_[!"."!]_PBCfg.h are different"
#endif
#if ((FLEXIO_UART_IP_CFG_SW_MAJOR_VERSION != FLEXIO_UART_IP_PBCFG_[!"."!]_SW_MAJOR_VERSION) || \
     (FLEXIO_UART_IP_CFG_SW_MINOR_VERSION != FLEXIO_UART_IP_PBCFG_[!"."!]_SW_MINOR_VERSION) || \
     (FLEXIO_UART_IP_CFG_SW_PATCH_VERSION != FLEXIO_UART_IP_PBCFG_[!"."!]_SW_PATCH_VERSION))
    #error "Software Version Numbers of Flexio_Uart_Ip_Cfg.h and Flexio_Uart_Ip_[!"."!]_PBCfg.h are different"
#endif
[!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!][!//
/* Checks against Flexio_Uart_Ip_PBcfg.h */
#if (FLEXIO_UART_IP_CFG_VENDOR_ID != FLEXIO_UART_IP_PBCFG_VENDOR_ID)
    #error "Flexio_Uart_Ip_Cfg.h and Flexio_Uart_Ip_PBcfg.h have different vendor ids"
#endif
#if ((FLEXIO_UART_IP_CFG_AR_RELEASE_MAJOR_VERSION    != FLEXIO_UART_IP_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (FLEXIO_UART_IP_CFG_AR_RELEASE_MINOR_VERSION    != FLEXIO_UART_IP_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (FLEXIO_UART_IP_CFG_AR_RELEASE_REVISION_VERSION != FLEXIO_UART_IP_PBCFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Flexio_Uart_Ip_Cfg.h and Flexio_Uart_Ip_PBcfg.h are different"
#endif
#if ((FLEXIO_UART_IP_CFG_SW_MAJOR_VERSION != FLEXIO_UART_IP_PBCFG_SW_MAJOR_VERSION) || \
     (FLEXIO_UART_IP_CFG_SW_MINOR_VERSION != FLEXIO_UART_IP_PBCFG_SW_MINOR_VERSION) || \
     (FLEXIO_UART_IP_CFG_SW_PATCH_VERSION != FLEXIO_UART_IP_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Flexio_Uart_Ip_Cfg.h and Flexio_Uart_Ip_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and StandardTypes.h header file are of the same Autosar version */
    #if ((FLEXIO_UART_IP_CFG_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
            (FLEXIO_UART_IP_CFG_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Flexio_Uart_Ip_Cfg.h and StandardTypes.h are different"
    #endif
#endif

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
 *                                     DEFINES AND MACROS
==================================================================================================*/
[!NOCODE!]
[!CODE!]#define FLEXIO_UART_IP_CONFIG_EXT \[!CR!][!ENDCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!][!//
    [!CODE!][!WS "4"!]FLEXIO_UART_IP_CONFIG_[!"."!]_PB \[!CR!][!ENDCODE!]
[!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS "4"!]FLEXIO_UART_IP_CONFIG_PB[!ENDCODE!]
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
[!NOCODE!]
[!IF "GeneralConfiguration/UartCallbackCapability"!]
    [!IF "(node:exists(GeneralConfiguration/UartCallback))"!]
        [!IF "node:value(GeneralConfiguration/UartCallback) != 'NULL_PTR'"!]
[!CODE!]
/**
* @brief    Declare callback function if it is used by user
*/

#define UART_START_SEC_CODE
#include "Uart_MemMap.h"

/* Define User callback function */
extern void Uart_Ipw_FlexioTransferCallback(const uint8 HwInstance, const Flexio_Uart_Ip_EventType Event, void* UserData);

#define UART_STOP_SEC_CODE
#include "Uart_MemMap.h"

[!ENDCODE!]
        [!ENDIF!]
    [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!][!//
#ifdef __cplusplus
}
#endif

/** @} */

#endif /* FLEXIO_UART_IP_CFG_H */
