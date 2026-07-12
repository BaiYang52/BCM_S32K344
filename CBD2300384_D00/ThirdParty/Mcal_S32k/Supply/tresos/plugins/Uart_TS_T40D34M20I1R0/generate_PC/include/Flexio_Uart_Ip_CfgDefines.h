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

#ifndef FLEXIO_UART_IP_CFG_DEFINES_H
#define FLEXIO_UART_IP_CFG_DEFINES_H

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
#include "Mcal.h"
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLEXIO_UART_IP_CFG_DEFINES_VENDOR_ID                     43
#define FLEXIO_UART_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION      4
#define FLEXIO_UART_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION      4
#define FLEXIO_UART_IP_CFG_DEFINES_AR_RELEASE_REVISION_VERSION   0
#define FLEXIO_UART_IP_CFG_DEFINES_SW_MAJOR_VERSION              2
#define FLEXIO_UART_IP_CFG_DEFINES_SW_MINOR_VERSION              0
#define FLEXIO_UART_IP_CFG_DEFINES_SW_PATCH_VERSION              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Mcal.h */
    #if ((FLEXIO_UART_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (FLEXIO_UART_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Flexio_Uart_Ip_CfgDefines.h and Mcal.h are different"
    #endif
#endif

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
 *                                     DEFINES AND MACROS
==================================================================================================*/
[!NOCODE!][!//
[!CODE!][!WS "0"!]/* Macros that indicate FLEXIO channels used by UART */[!CR!][!ENDCODE!][!//
[!LOOP "UartGlobalConfig/UartChannel/*"!]
[!IF "UartHwUsing = 'FLEXIO_IP'"!]
    [!CODE!][!WS "0"!]#ifndef [!"node:ref(FlexioModuleConfiguration/UartHwChannelRef)/../../FlexioMclInstances"!]_CH_[!"num:i(text:split((node:ref(FlexioModuleConfiguration/UartHwChannelRef)/FlexioMclChannelId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#define [!"node:ref(FlexioModuleConfiguration/UartHwChannelRef)/../../FlexioMclInstances"!]_CH_[!"num:i(text:split((node:ref(FlexioModuleConfiguration/UartHwChannelRef)/FlexioMclChannelId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#else[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#error "[!"node:ref(FlexioModuleConfiguration/UartHwChannelRef)/FlexioMclChannelId"!] cannot be used by UART driver. Channel locked by another driver!"[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#endif[!CR!][!CR!][!ENDCODE!][!//
[!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!][!//

[!NOCODE!][!//
[!CODE!][!WS "0"!]/* Macros that indicate FLEXIO pins used by UART */[!CR!][!ENDCODE!][!//
[!LOOP "UartGlobalConfig/UartChannel/*"!]
[!IF "UartHwUsing = 'FLEXIO_IP'"!]
    [!CODE!][!WS "0"!]#ifndef [!"node:ref(FlexioModuleConfiguration/UartHwChannelRef)/../../FlexioMclInstances"!]_PIN_[!"num:i((text:split((node:ref(FlexioModuleConfiguration/UartHwChannelRef)/FlexioMclPinId),'_')[2]))"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#define [!"node:ref(FlexioModuleConfiguration/UartHwChannelRef)/../../FlexioMclInstances"!]_PIN_[!"num:i((text:split((node:ref(FlexioModuleConfiguration/UartHwChannelRef)/FlexioMclPinId),'_')[2]))"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#else[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#error "[!"node:ref(FlexioModuleConfiguration/UartHwChannelRef)/FlexioMclPinId"!] cannot be used by UART driver. Pin used by another driver!"[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#endif[!CR!][!CR!][!ENDCODE!][!//
[!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!][!//

[!NOCODE!][!//
[!CODE!][!WS "0"!]/* Macros that indicate FLEXIO channels used by UART */[!CR!][!ENDCODE!][!//
[!LOOP "UartGlobalConfig/UartChannel/*"!]
[!IF "UartHwUsing = 'FLEXIO_IP'"!]
    [!CODE!][!WS "0"!]#ifdef [!"node:ref(FlexioModuleConfiguration/UartHwChannelRef)/../../FlexioMclInstances"!]_CH_[!"num:i(text:split((node:ref(FlexioModuleConfiguration/UartHwChannelRef)/FlexioMclChannelId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#define UART_CHANNEL_[!"num:i(text:split((node:ref(FlexioModuleConfiguration/UartHwChannelRef)/FlexioMclChannelId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#else[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#error "[!"node:ref(FlexioModuleConfiguration/UartHwChannelRef)/FlexioMclChannelId"!] cannot be used by UART driver. Channel locked by other driver!"[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#endif[!CR!][!CR!][!ENDCODE!][!//
[!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!][!//

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



#ifdef __cplusplus
}
#endif

/** @} */

#endif /* FLEXIO_UART_IP_CFG_DEFINES_H */
