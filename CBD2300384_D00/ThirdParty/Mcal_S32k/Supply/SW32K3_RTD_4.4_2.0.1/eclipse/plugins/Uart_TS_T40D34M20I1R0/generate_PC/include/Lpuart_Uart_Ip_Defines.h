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

#ifndef LPUART_UART_IP_DEFINES_H
#define LPUART_UART_IP_DEFINES_H

/**
*   @file
*   @defgroup lpuart_uart_ip Lpuart UART IPL
*   @addtogroup  lpuart_uart_ip Lpuart UART IPL
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
[!NOCODE!][!// Include specific header file
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!VAR "GetDerivative" = "as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative"!][!//
    [!VAR "Derivative" = "substring-before($GetDerivative, '_')"!][!//

    [!IF "contains($Derivative, 's32k3') or contains($Derivative, 's32k1')"!][!//
        [!IF "contains($Derivative, 's32k341')"!][!//
[!CODE!][!WS "0"!]#include "S32K342_LPUART.h"[!CR!][!ENDCODE!][!//
        [!ELSE!][!//
            [!VAR "DerivativeUpper" = "text:toupper($Derivative)"!][!//
[!CODE!][!WS "0"!]#include "[!"$DerivativeUpper"!]_LPUART.h"[!CR!][!ENDCODE!][!//
        [!ENDIF!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define LPUART_UART_IP_DEFINES_VENDOR_ID                     43
#define LPUART_UART_IP_DEFINES_AR_RELEASE_MAJOR_VERSION      4
#define LPUART_UART_IP_DEFINES_AR_RELEASE_MINOR_VERSION      4
#define LPUART_UART_IP_DEFINES_AR_RELEASE_REVISION_VERSION   0
#define LPUART_UART_IP_DEFINES_SW_MAJOR_VERSION              2
#define LPUART_UART_IP_DEFINES_SW_MINOR_VERSION              0
#define LPUART_UART_IP_DEFINES_SW_PATCH_VERSION              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Mcal.h */
    #if ((LPUART_UART_IP_DEFINES_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (LPUART_UART_IP_DEFINES_AR_RELEASE_MINOR_VERSION != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Lpuart_Uart_Ip_CfgDefines.h and Mcal.h are different"
    #endif
#endif

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
 *                                     DEFINES AND MACROS
==================================================================================================*/

/* @brief Development error detection */
#define LPUART_UART_IP_DEV_ERROR_DETECT             ([!IF "GeneralConfiguration/UartDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* @brief Number of available hardware instances */
#define LPUART_UART_IP_NUMBER_OF_INSTANCES          ([!"num:i(ecu:get('Uart.UartGlobalConfig.UartLpuartChannel'))"!]U) /*!< Number of available hardware instances */

/* @brief Uart Osif source counter. This parameter is used to select between different OsIf counter implementation */
#define LPUART_UART_IP_TIMEOUT_TYPE                 ([!"GeneralConfiguration/UartTimeoutMethod"!])

/* @brief Number of loops before returning LPUART_STATUS_TIMEOUT.*/
#define LPUART_UART_IP_TIMEOUT_VALUE_US             ([!"GeneralConfiguration/UartTimeoutDuration"!]U)

/* @brief LPUART DMA support */
#define LPUART_UART_IP_HAS_DMA_ENABLED              ([!IF "GeneralConfiguration/UartDmaEnable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* @brief Support for User mode. If this parameter has been configured to TRUE, the Uart driver can be executed from both supervisor and user mode. */
#define LPUART_UART_IP_ENABLE_USER_MODE_SUPPORT     (STD_OFF)

[!NOCODE!]

[!VAR "StringLoopback" = "'{'"!][!//
[!VAR "TotalOfChannels" = "ecu:get('Uart.UartGlobalConfig.UartLpuartChannel')"!]
[!VAR "Found" = "0"!]
[!VAR "EnableLoopback" = "false()"!]
[!VAR "FeatureEnableLoopback" = "'(STD_OFF)'"!]

[!FOR "Id"="0" TO "$TotalOfChannels - 1"!]
    [!VAR "Found" = "0"!]
    [!LOOP "UartGlobalConfig/UartChannel/*"!]
        [!IF "UartHwUsing = 'LPUART_IP'"!][!//
            [!VAR "Channel" = "DetailModuleConfiguration/UartHwChannel"!]
            [!IF "contains($Channel, 'LPUART')"!]

                [!VAR "EnableLoopback" = "false()"!]

                [!VAR "HwChannel" = "num:i(substring-after($Channel ,'_'))"!]
                [!IF "$Id = num:i($HwChannel)"!]

                    [!VAR "Found" = "1"!]
                    [!IF "DetailModuleConfiguration/UartInternalLoopbackEnable"!]

                        [!VAR "EnableLoopback" = "true()"!]
                        [!VAR "FeatureEnableLoopback" = "'(STD_ON)'"!]
                    [!ENDIF!]
                [!BREAK!]
                [!ENDIF!]

            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
    [!IF "$Found  = num:i(1)"!]
        [!IF "$EnableLoopback"!]
            [!IF "$Id = num:i($TotalOfChannels) - 1"!]
                [!VAR "StringLoopback" = "concat($StringLoopback, '(boolean) TRUE }')"!]
            [!ELSE!]
                [!VAR "StringLoopback" = "concat($StringLoopback, '(boolean) TRUE,')"!]
            [!ENDIF!]
        [!ELSE!]
            [!IF "$Id = $TotalOfChannels"!]
                [!VAR "StringLoopback" = "concat($StringLoopback, '(boolean) FALSE }')"!]
            [!ELSE!]
                [!VAR "StringLoopback" = "concat($StringLoopback, '(boolean) FALSE,')"!]
            [!ENDIF!]
        [!ENDIF!]
    [!ELSE!]
        [!IF "$Id = $TotalOfChannels - 1"!]
            [!VAR "StringLoopback" = "concat($StringLoopback, '(boolean) FALSE }')"!]
        [!ELSE!]
            [!VAR "StringLoopback" = "concat($StringLoopback, '(boolean) FALSE,')"!]
        [!ENDIF!]
   [!ENDIF!]
[!ENDFOR!]
[!ENDNOCODE!]

/* @brief Support for Internal Loopback. If this parameter has been configured to TRUE, the Uart driver will be executed in Loopback mode. */
#define LPUART_UART_IP_ENABLE_INTERNAL_LOOPBACK  [!"$FeatureEnableLoopback"!]

#if (STD_ON == LPUART_UART_IP_ENABLE_INTERNAL_LOOPBACK)
    /* @brief Array of instances that have loopback mode enabled. */
    #define LPUART_UART_IP_ENABLE_INTERNAL_LOOPBACK_PER_INSTANCE  [!"$StringLoopback"!]
#endif

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

#endif /* LPUART_UART_IP_DEFINES_H */
