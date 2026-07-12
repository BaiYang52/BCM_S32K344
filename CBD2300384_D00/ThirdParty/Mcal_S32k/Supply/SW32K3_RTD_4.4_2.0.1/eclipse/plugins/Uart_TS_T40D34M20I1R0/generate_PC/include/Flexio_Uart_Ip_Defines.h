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

#ifndef FLEXIO_UART_IP_DEFINES_H
#define FLEXIO_UART_IP_DEFINES_H

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

[!NOCODE!][!// Include specific header file
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!VAR "GetDerivative" = "as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative"!][!//
    [!VAR "Derivative" = "substring-before($GetDerivative, '_')"!][!//

    [!IF "contains($Derivative, 's32k3') or contains($Derivative, 's32k1')"!][!//
        [!IF "contains($Derivative, 's32k341')"!][!//
[!CODE!][!WS "0"!]#include "S32K342_FLEXIO.h"[!CR!][!ENDCODE!][!//
        [!ELSE!][!//
            [!VAR "DerivativeUpper" = "text:toupper($Derivative)"!][!//
[!CODE!][!WS "0"!]#include "[!"$DerivativeUpper"!]_FLEXIO.h"[!CR!][!ENDCODE!][!//
        [!ENDIF!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLEXIO_UART_IP_DEFINES_VENDOR_ID                     43
#define FLEXIO_UART_IP_DEFINES_AR_RELEASE_MAJOR_VERSION      4
#define FLEXIO_UART_IP_DEFINES_AR_RELEASE_MINOR_VERSION      4
#define FLEXIO_UART_IP_DEFINES_AR_RELEASE_REVISION_VERSION   0
#define FLEXIO_UART_IP_DEFINES_SW_MAJOR_VERSION              2
#define FLEXIO_UART_IP_DEFINES_SW_MINOR_VERSION              0
#define FLEXIO_UART_IP_DEFINES_SW_PATCH_VERSION              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Mcal.h */
    #if ((FLEXIO_UART_IP_DEFINES_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (FLEXIO_UART_IP_DEFINES_AR_RELEASE_MINOR_VERSION != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Flexio_Uart_Ip_Defines.h and Mcal.h are different"
    #endif
#endif

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
 *                                     DEFINES AND MACROS
==================================================================================================*/
[!NOCODE!][!//
[!VAR "UsingFlexio" = "0"!]
[!LOOP "UartGlobalConfig/UartChannel/*"!]
    [!IF "UartHwUsing = 'FLEXIO_IP'"!]
        [!VAR "UsingFlexio" = "1"!]
        [!BREAK!][!//
    [!ENDIF!]
[!ENDLOOP!]
[!CODE!]
/* @brief Using FLEXIO */
#define FLEXIO_UART_IP_IS_USING                         ([!IF "$UsingFlexio = 1"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!ENDCODE!][!//
[!ENDNOCODE!][!//
/* @brief Development error detection */
#define FLEXIO_UART_IP_DEV_ERROR_DETECT                 ([!IF "GeneralConfiguration/UartDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* @brief FLEXIO DMA support */
#define FLEXIO_UART_IP_HAS_DMA_ENABLED                  ([!IF "GeneralConfiguration/UartDmaEnable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* @brief Number of available hardware shifter and timer */
#define FLEXIO_UART_IP_NUMBER_OF_SHIFTER_AND_TIMER      ([!"num:i(ecu:get('Uart.UartGlobalConfig.UartFlexioChannel'))"!]U)

/* @brief Uart Osif source counter. This parameter is used to select between different OsIf counter implementation */
#define FLEXIO_UART_IP_TIMEOUT_TYPE                     ([!"GeneralConfiguration/UartTimeoutMethod"!])

/* @brief Number of loops before returning FLEXIO_STATUS_TIMEOUT.*/
#define FLEXIO_UART_IP_TIMEOUT_VALUE_US                 ([!"GeneralConfiguration/UartTimeoutDuration"!]U)

/* @brief Support for User mode. If this parameter has been configured to TRUE, the Uart driver can be executed from both supervisor and user mode. */
#define FLEXIO_UART_IP_ENABLE_USER_MODE_SUPPORT         (STD_OFF)

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

#endif /* FLEXIO_UART_IP_DEFINES_H */
