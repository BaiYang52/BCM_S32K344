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
*   @internal
*   @addtogroup UART_IPW_DRIVER
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
#include "Uart_Ipw_Cfg.h"
#ifdef UART_IPW_LPUART_HW_USING
#include "Lpuart_Uart_Ip_Cfg.h"
#endif
#ifdef UART_IPW_FLEXIO_HW_USING
#include "Flexio_Uart_Ip_Cfg.h"
#endif
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C                     43
#define UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C      4
#define UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C      4
#define UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C   0
#define UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C              2
#define UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C              0
#define UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Uart_Ipw_Cfg.h */
#if (UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != UART_IPW_CFG_VENDOR_ID)
    #error "Uart_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Uart_Ipw_Cfg.h have different vendor ids"
#endif
#if ((UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != UART_IPW_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != UART_IPW_CFG_AR_RELEASE_MINOR_VERSION) || \
     (UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != UART_IPW_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Uart_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Uart_Ipw_Cfg.h are different"
#endif
#if ((UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != UART_IPW_CFG_SW_MAJOR_VERSION) || \
     (UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != UART_IPW_CFG_SW_MINOR_VERSION) || \
     (UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != UART_IPW_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Uart_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Uart_Ipw_Cfg.h are different"
#endif

#ifdef UART_IPW_LPUART_HW_USING
/* Checks against Lpuart_Uart_Ip_Cfg.h */
#if (UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != LPUART_UART_IP_CFG_VENDOR_ID)
    #error "Uart_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lpuart_Uart_Ip_Cfg.h have different vendor ids"
#endif
#if ((UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != LPUART_UART_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != LPUART_UART_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != LPUART_UART_IP_CFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Uart_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lpuart_Uart_Ip_Cfg.h are different"
#endif
#if ((UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != LPUART_UART_IP_CFG_SW_MAJOR_VERSION) || \
     (UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != LPUART_UART_IP_CFG_SW_MINOR_VERSION) || \
     (UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != LPUART_UART_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Uart_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lpuart_Uart_Ip_Cfg.h are different"
#endif
#endif

#ifdef UART_IPW_FLEXIO_HW_USING
/* Checks against Flexio_Uart_Ip_Cfg.h */
#if (UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != FLEXIO_UART_IP_CFG_VENDOR_ID)
    #error "Uart_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_Uart_Ip_Cfg.h have different vendor ids"
#endif
#if ((UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != FLEXIO_UART_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != FLEXIO_UART_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != FLEXIO_UART_IP_CFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Uart_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_Uart_Ip_Cfg.h are different"
#endif
#if ((UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != FLEXIO_UART_IP_CFG_SW_MAJOR_VERSION) || \
     (UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != FLEXIO_UART_IP_CFG_SW_MINOR_VERSION) || \
     (UART_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != FLEXIO_UART_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Uart_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_Uart_Ip_Cfg.h are different"
#endif
#endif
/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
#define UART_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Uart_MemMap.h"

#ifdef UART_IPW_LPUART_HW_USING
/**
* @brief   Export Lpuart Uart configurations.
*/
LPUART_UART_IP_CONFIG_EXT
#endif

#ifdef UART_IPW_FLEXIO_HW_USING
/**
* @brief   Export Flexio Uart configurations.
*/
FLEXIO_UART_IP_CONFIG_EXT
#endif
#define UART_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Uart_MemMap.h"
/*==================================================================================================
*                         LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                  LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
#define UART_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Uart_MemMap.h"
[!NOCODE!]
[!VAR "Callback" = "'NULL_PTR'"!]
[!IF "GeneralConfiguration/UartCallbackCapability"!]
    [!IF "(node:exists(GeneralConfiguration/UartCallback))"!]
        [!VAR "CallbackName"="GeneralConfiguration/UartCallback"!]
        [!VAR "Callback" = "concat("(Uart_CallbackType)", $CallbackName)"!]
    [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!]

[!LOOP "UartGlobalConfig/UartChannel/*"!][!//
[!NOCODE!]
[!VAR "HwUsing" = "UartHwUsing"!]
[!IF "$HwUsing = 'LPUART_IP'"!]
    [!VAR "Uart_Hw_Channel" = "substring-after(string(DetailModuleConfiguration/UartHwChannel),'_')"!]
[!ELSE!]
    [!SELECT "node:ref(FlexioModuleConfiguration/UartHwChannelRef)"!]
        [!VAR "Uart_Hw_Channel" = "substring-after(string(FlexioMclChannelId),'_')"!]
    [!ENDSELECT!]
[!ENDIF!]

[!ENDNOCODE!]
[!CODE!]
/**
* @brief   The Ip configuration structure pointer
*/
const Uart_Ipw_IpConfigType Uart_Ipw_IpChnConfigPB_[!"UartChannelId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
[!IF "$HwUsing = 'LPUART_IP'"!]
    /* Lpuart Ip configuration structure  */
    &Lpuart_Uart_Ip_xHwConfigPB_[!"UartChannelId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
[!ELSE!]
    /* Not use Lpuart Ip configuration structure  */
    NULL_PTR,
[!ENDIF!]
[!IF "$HwUsing != 'LPUART_IP'"!]
    /* Flexio Uart Ip configuration structure  */
    &Flexio_Uart_Ip_xHwConfigPB_[!"UartChannelId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
[!ELSE!]
    /* Not use Flexio Uart Ip configuration structure  */
    NULL_PTR
[!ENDIF!]
};

/**
* @brief    Hardware configuration for Uart Hardware - Configuration.
*/

const Uart_Ipw_HwConfigType Uart_Ipw_xHwConfigPB_[!"UartChannelId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    /* Uart Hardware Channel.*/
    [!"$Uart_Hw_Channel"!]U,
    /* Type of Hardware interface configured.*/
    [!"$HwUsing"!],
    /* User Callback */
    [!"$Callback"!],
    /* Pointer to the Ip configuration structure.*/
    &Uart_Ipw_IpChnConfigPB_[!"UartChannelId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
};
[!ENDCODE!]
[!ENDLOOP!][!//
#define UART_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Uart_MemMap.h"
/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


#ifdef __cplusplus
}

/** @} */

#endif
