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
==================================================================================================*/

#include "Lpuart_Uart_Ip_Types.h"
#include "Lpuart_Uart_Ip_Cfg.h"
#include "StandardTypes.h"
#if (LPUART_UART_IP_HAS_DMA_ENABLED == STD_ON)
#include "Dma_Ip.h"
#endif
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C                     43
#define LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C      4
#define LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C      4
#define LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C   0
#define LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C              2
#define LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C              0
#define LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/* Checks against Lpuart_Uart_Ip_Types.h */
#if (LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != LPUART_UART_IP_TYPES_VENDOR_ID)
    #error "Lpuart_Uart_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lpuart_Uart_Ip_Types.h have different vendor ids"
#endif
#if ((LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != LPUART_UART_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != LPUART_UART_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != LPUART_UART_IP_TYPES_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Lpuart_Uart_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lpuart_Uart_Ip_Types.h are different"
#endif
#if ((LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != LPUART_UART_IP_TYPES_SW_MAJOR_VERSION) || \
     (LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != LPUART_UART_IP_TYPES_SW_MINOR_VERSION) || \
     (LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != LPUART_UART_IP_TYPES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Lpuart_Uart_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lpuart_Uart_Ip_Types.h are different"
#endif

/* Checks against Lpuart_Uart_Ip_Cfg.h */
#if (LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != LPUART_UART_IP_CFG_VENDOR_ID)
    #error "Lpuart_Uart_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lpuart_Uart_Ip_Cfg.h have different vendor ids"
#endif
#if ((LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != LPUART_UART_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != LPUART_UART_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != LPUART_UART_IP_CFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Lpuart_Uart_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lpuart_Uart_Ip_Cfg.h are different"
#endif
#if ((LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != LPUART_UART_IP_CFG_SW_MAJOR_VERSION) || \
     (LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != LPUART_UART_IP_CFG_SW_MINOR_VERSION) || \
     (LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != LPUART_UART_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Lpuart_Uart_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lpuart_Uart_Ip_Cfg.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and StandardTypes.h header file are of the same Autosar version */
    #if ((LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C != STD_AR_RELEASE_MAJOR_VERSION) || \
         (LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C != STD_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Lpuart_Uart_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and StandardTypes.h are different"
    #endif
    /* Checks against Dma_Ip.h */
    #if (LPUART_UART_IP_HAS_DMA_ENABLED == STD_ON)
        #if ((LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C != DMA_IP_AR_RELEASE_MAJOR_VERSION) || \
             (LPUART_UART_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C != DMA_IP_AR_RELEASE_MINOR_VERSION))
            #error "AutoSar Version Numbers of Lpuart_Uart_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Dma_Ip.h are different"
        #endif
    #endif
#endif
/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                   STATE STRUCTURE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

#define UART_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Uart_MemMap.h"
[!NOCODE!]
[!VAR "NumHwChannelConfig" = "0"!]
[!LOOP "UartGlobalConfig/UartChannel/*"!][!//

[!IF "UartHwUsing = 'LPUART_IP'"!]
    [!VAR "NumHwChannelConfig" = "$NumHwChannelConfig + 1"!]
[!ENDIF!]
[!ENDLOOP!][!//
[!IF "$NumHwChannelConfig != 0"!]
[!CODE!]extern Lpuart_Uart_Ip_StateStructureType Lpuart_Uart_Ip_apStateStructure[LPUART_UART_IP_NUMBER_OF_INSTANCES];[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]
#define UART_STOP_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Uart_MemMap.h"

#define UART_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Uart_MemMap.h"

[!NOCODE!]
[!VAR "Callback"  = "'NULL_PTR'"!]
[!VAR "CallbackParam" = "'NULL_PTR'"!]
[!IF "GeneralConfiguration/UartCallbackCapability"!]
        [!IF "(node:exists(GeneralConfiguration/UartCallback))"!]
            [!IF "node:value(GeneralConfiguration/UartCallback) != 'NULL_PTR'"!]
                [!VAR "Callback" = "'(Lpuart_Uart_Ip_CallbackType)Uart_Ipw_LpuartCallback'"!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDIF!]
[!ENDNOCODE!]

[!NOCODE!]
[!VAR "NumChannelUser"  = "0"!]
[!LOOP "UartGlobalConfig/UartChannel/*"!][!//

[!IF "UartHwUsing = 'LPUART_IP'"!]

[!CODE!]
/**
* @brief          Hardware configuration for Uart Hardware - Configuration:
*
* @api
*/
const Lpuart_Uart_Ip_UserConfigType Lpuart_Uart_Ip_xHwConfigPB_[!"UartChannelId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
[!ENDCODE!]
[!IF "(node:exists(DetailModuleConfiguration))"!]
    [!VAR "LpuartBaudrate" = "num:i(substring-after(DetailModuleConfiguration/DesireBaudrate, 'LPUART_UART_BAUDRATE_'))"!]
    [!SELECT "node:ref(UartClockRef)"!]
    [!INCLUDE "Uart_BaudRate_Comp.m"!][!//
[!ENDSELECT!][!//

[!CODE!]
    /*!< Baud rate in hertz */
    [!"$ActualBaudrate"!]U,
    /* Baud clock divisor*/
    [!"$BaudRateDivisor"!]U,
    /* Over sampling ratio*/
    [!"$BaudOverSamplingRatio"!]U,
    /* Parity type */
    [!"DetailModuleConfiguration/UartParityType"!],
    /* Number of stop bits, 1 stop bit (default) or 2 stop bits */
    [!"DetailModuleConfiguration/UartStopBitNumber"!],
    /* Number of bits per transmitted/received word */
    [!"DetailModuleConfiguration/UartWordLength"!],
    /* Type of UART transfer (interrupt/dma based) */
    [!"DetailModuleConfiguration/UartInteruptDmaMethod"!],
    /* Callback to invoke for Uart event.*/
    [!"$Callback"!],
    /* User callback parameter pointer.*/
    [!"$CallbackParam"!],
#if (LPUART_UART_IP_HAS_DMA_ENABLED == STD_ON)
    /* DMA channel number for DMA-based rx. */
[!IF "(node:exists(DetailModuleConfiguration/UartDmaRxChannelRef))"!][!//
[!SELECT "node:ref(DetailModuleConfiguration/UartDmaRxChannelRef)"!][!//
    [!"dmaLogicChannel_LogicName"!],
[!ENDSELECT!][!//
[!ELSE!][!//
    255,
[!ENDIF!][!//
    /* DMA channel number for DMA-based tx. */
[!IF "(node:exists(DetailModuleConfiguration/UartDmaTxChannelRef))"!][!//
[!SELECT "node:ref(DetailModuleConfiguration/UartDmaTxChannelRef)"!][!//
    [!"dmaLogicChannel_LogicName"!],
[!ENDSELECT!][!//
[!ELSE!][!//
    255,
[!ENDIF!][!//
#endif
    /* Runtime state structure refference */
    &Lpuart_Uart_Ip_apStateStructure[[!"num:i($NumChannelUser)"!]]
[!ENDCODE!]
[!ENDIF!][!//    /* exists(DetailModuleConfiguration) */
[!CODE!]
};
[!ENDCODE!]
[!VAR "NumChannelUser" = "$NumChannelUser + 1"!]
[!ENDIF!]
[!ENDLOOP!][!//
[!ENDNOCODE!]
#define UART_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Uart_MemMap.h"
/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/


#ifdef __cplusplus
}

/** @} */

#endif
