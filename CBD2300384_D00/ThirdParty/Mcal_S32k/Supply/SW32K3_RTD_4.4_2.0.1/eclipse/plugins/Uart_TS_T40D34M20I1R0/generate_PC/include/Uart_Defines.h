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

#ifndef UART_CFG_DEFINES_H
#define UART_CFG_DEFINES_H

/**
*   @file
*
*   @defgroup uart_driver UART Driver
*   @addtogroup  uart_driver UART Driver
*   @{
*/

#ifdef __cplusplus
extern "C"
{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Mcal.h"
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define UART_DEFINES_VENDOR_ID_CFG                     43
#define UART_DEFINES_AR_RELEASE_MAJOR_VERSION_CFG      4
#define UART_DEFINES_AR_RELEASE_MINOR_VERSION_CFG      4
#define UART_DEFINES_AR_RELEASE_REVISION_VERSION_CFG   0
#define UART_DEFINES_SW_MAJOR_VERSION_CFG              2
#define UART_DEFINES_SW_MINOR_VERSION_CFG              0
#define UART_DEFINES_SW_PATCH_VERSION_CFG              1
/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Mcal.h */
    #if ((UART_DEFINES_AR_RELEASE_MAJOR_VERSION_CFG != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (UART_DEFINES_AR_RELEASE_MINOR_VERSION_CFG != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Uart_Defines.h and Mcal.h are different"
    #endif
#endif
/*==================================================================================================
*                                     DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
/* @brief Enables / Disables multicore support */
#define UART_MULTICORE_SUPPORT          [!IF "GeneralConfiguration/UartMulticoreSupport"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/* @brief Enables / Disables PreCompile support when there is a single configuration */
#define UART_PRECOMPILE_SUPPORT         ([!IF "(IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* @brief Enables / Disables DET development errors detection and reporting */
#define UART_DEV_ERROR_DETECT           ([!IF "GeneralConfiguration/UartDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* @brief Enables / Disables Runtime errors detection and reporting */
#define UART_RUNTIME_ERROR_DETECT       ([!IF "GeneralConfiguration/DisableUartRuntimeErrorDetect"!]STD_OFF[!ELSE!]STD_ON[!ENDIF!])

/* @brief Number of Channels configured. */
#define UART_CH_MAX_CONFIG              ([!"num:i(count(UartGlobalConfig/UartChannel/*))"!]U)

/* @brief UART Osif source counter. This parameter is used to select between different OsIf counter implementation */
#define UART_TIMEOUT_TYPE               ([!"GeneralConfiguration/UartTimeoutMethod"!])

/* @brief Number of loops before returning Timeout status */
#define UART_TIMEOUT_VALUE_US           ([!"GeneralConfiguration/UartTimeoutDuration"!]U)

/* @brief Switches the Uart_GetVersionInfo() API ON or OFF. Support for version info API. */
#define UART_VERSION_INFO_API           [!IF "GeneralConfiguration/UartVersionInfoApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/* @brief Uart has feature DMA enable. */
#define UART_HAS_DMA_ENABLED            [!IF "GeneralConfiguration/UartDmaEnable"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/* @brief Number of available hardware instances */
#define UART_NUMBER_OF_INSTANCES             ([!"num:i(ecu:get('Uart.UartGlobalConfig.UartChannel'))"!]U)

/* @brief UART Hardware instances mapping initialize value */
#define UART_HW_CHANNELS_MAPPING_FOR_INIT       {[!FOR "idx" = "1" TO "num:i(ecu:get('Uart.UartGlobalConfig.UartChannel'))"!]-1[!IF "$idx < num:i(ecu:get('Uart.UartGlobalConfig.UartChannel'))"!], [!ENDIF!][!ENDFOR!]}

/* @brief Declare callback parameters if any */
#define UART_CALLBACK_DECLARED            [!IF "GeneralConfiguration/UartCallbackCapability"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief        All CoreIDs are supported by Uart driver.
*/
[!NOCODE!]
[!VAR "MaxCoreId"="0"!]
[!IF "node:value(GeneralConfiguration/UartMulticoreSupport)='true'"!]
    [!LOOP "as:modconf('EcuC')[1]/EcucHardware/*[1]/EcucCoreDefinition/*"!][!//
            [!IF "$MaxCoreId < node:value(./EcucCoreId)"!]
                [!VAR "MaxCoreId"="node:value(./EcucCoreId)"!]
            [!ENDIF!] 
    [!ENDLOOP!]
[!ENDIF!]
[!VAR "MaxCoreId" = "$MaxCoreId + 1"!][!//
[!ENDNOCODE!]
#define UART_MAX_PARTITIONS             ([!"num:i($MaxCoreId)"!]U)

/* Api to retrieve the core id */
#if (STD_ON == UART_MULTICORE_SUPPORT)
    #define Uart_GetCoreID() (OsIf_GetCoreID())
#else
    #define Uart_GetCoreID() ((uint32)0x0U)
#endif /* (STD_ON == UART_MULTICORE_SUPPORT) */

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

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

/*==================================================================================================
*                                  EXTERNAL CONSTANTS
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* UART_CFG_DEFINES_H */
