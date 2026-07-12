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

/**
*   @file   Lin_Ipw_PBcfg.c
*   @implements Lin_Ipw_PBcfg.c_Artifact
*   
*   @internal 
*   @addtogroup LIN_IPW
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
#include "Lin_Ipw_Cfg.h"
#include "Lpuart_Lin_Ip_Cfg.h"
#if (STD_ON == LIN_FLEXIO_USED)
#include "Flexio_Lin_Ip_Cfg.h"
#endif


/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_VENDOR_ID_C                     43
#define LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C      4
#define LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MINOR_VERSION_C      4
#define LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_REVISION_VERSION_C   0
#define LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MAJOR_VERSION_C              2
#define LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MINOR_VERSION_C              0
#define LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_PATCH_VERSION_C              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Lin_Ipw_Cfg.h */
#if (LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_VENDOR_ID_C != LIN_IPW_CFG_VENDOR_ID)
    #error "Lin_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lin_Ipw_Cfg.h have different vendor ids"
#endif
#if ((LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C    != LIN_IPW_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MINOR_VERSION_C    != LIN_IPW_CFG_AR_RELEASE_MINOR_VERSION) || \
     (LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_REVISION_VERSION_C != LIN_IPW_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Lin_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lin_Ipw_Cfg.h are different"
#endif
#if ((LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MAJOR_VERSION_C != LIN_IPW_CFG_SW_MAJOR_VERSION) || \
     (LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MINOR_VERSION_C != LIN_IPW_CFG_SW_MINOR_VERSION) || \
     (LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_PATCH_VERSION_C != LIN_IPW_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Lin_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lin_Ipw_Cfg.h are different"
#endif

/* Checks against Lpuart_Lin_Ip_Cfg.h */
#if (LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_VENDOR_ID_C != LPUART_LIN_IP_CFG_VENDOR_ID)
    #error "Lin_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lpuart_Lin_Ip_Cfg.h have different vendor ids"
#endif
#if ((LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C    != LPUART_LIN_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MINOR_VERSION_C    != LPUART_LIN_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_REVISION_VERSION_C != LPUART_LIN_IP_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Lin_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lpuart_Lin_Ip_Cfg.h are different"
#endif
#if ((LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MAJOR_VERSION_C != LPUART_LIN_IP_CFG_SW_MAJOR_VERSION) || \
     (LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MINOR_VERSION_C != LPUART_LIN_IP_CFG_SW_MINOR_VERSION) || \
     (LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_PATCH_VERSION_C != LPUART_LIN_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Lin_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lpuart_Lin_Ip_Cfg.h are different"
#endif

#if (STD_ON == LIN_FLEXIO_USED)
/* Checks against Flexio_Lin_Ip_Cfg.h */
#if (LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_VENDOR_ID_C != FLEXIO_LIN_IP_CFG_VENDOR_ID)
    #error "Lin_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_Lin_Ip_Cfg.h have different vendor ids"
#endif
#if ((LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C    != FLEXIO_LIN_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MINOR_VERSION_C    != FLEXIO_LIN_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_REVISION_VERSION_C != FLEXIO_LIN_IP_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Lin_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_Lin_Ip_Cfg.h are different"
#endif
#if ((LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MAJOR_VERSION_C != FLEXIO_LIN_IP_CFG_SW_MAJOR_VERSION) || \
     (LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MINOR_VERSION_C != FLEXIO_LIN_IP_CFG_SW_MINOR_VERSION) || \
     (LIN_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_PATCH_VERSION_C != FLEXIO_LIN_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Lin_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_Lin_Ip_Cfg.h are different"
#endif
#endif

/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/
#define LIN_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Lin_MemMap.h"

/**
* @brief   Export LPUART_LIN configurations.
*/
LPUART_LIN_IP_CONFIG_EXT

#if (STD_ON == LIN_FLEXIO_USED)
/**
* @brief   Export FLEXIO_LIN configurations.
*/
FLEXIO_LIN_IP_CONFIG_EXT
#endif
/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/
[!NOCODE!]
[!VAR "DualClockEnabled"= "'false'"!]
[!LOOP "LinGlobalConfig/LinChannel/*"!]
    [!IF "(node:exists(LinClockRef_Alternate))"!]
        [!VAR "DualClockEnabled"= "'true'"!]
    [!ENDIF!]
[!ENDLOOP!]

[!LOOP "LinGlobalConfig/LinChannel/*"!][!//
    [!IF "not(node:exists(LinFlexioTxControllerRef) and node:exists(LinFlexioRxControllerRef))"!]

        [!IF "$DualClockEnabled = 'true'"!]
            [!VAR "LinBaudrate" = "LinChannelBaudRate"!]
            [!IF "node:exists(LinClockRef_Alternate)"!]
                [!SELECT "node:ref(LinClockRef_Alternate)"!]
                    [!INCLUDE "Lin_BaudRate_Comp.m"!]
                [!ENDSELECT!]
            [!ELSE!]
                [!SELECT "node:ref(LinClockRef)"!]
                    [!INCLUDE "Lin_BaudRate_Comp.m"!]
                [!ENDSELECT!]
            [!ENDIF!]
        [!ENDIF!]
    [!ELSE!]
        [!IF "$DualClockEnabled = 'true'"!]
            [!VAR "Baudrate" = "LinChannelBaudRate"!]
            [!IF "node:exists(LinClockRef_Alternate)"!]
                [!SELECT "node:ref(LinClockRef_Alternate)"!]
                    [!VAR "ClockFrequencyFromMcu" = "McuClockReferencePointFrequency"!]
                    [!VAR "ClockFrequency" = "num:i($ClockFrequencyFromMcu)"!]
                [!ENDSELECT!]
            [!ELSE!]
                [!SELECT "node:ref(LinClockRef)"!]
                    [!VAR "ClockFrequencyFromMcu" = "McuClockReferencePointFrequency"!]
                    [!VAR "ClockFrequency" = "num:i($ClockFrequencyFromMcu)"!]
                [!ENDSELECT!]
            [!ENDIF!]
            
            [!VAR "FlexioTIMDEC" = "0"!]
            [!VAR "ActualBaudrate" = "0"!]
            [!VAR "ErrorValue" = "1000000"!]
            [!VAR "DivBest" = "255"!]
            [!VAR "RatioBest" = "num:i(ecu:list('Lin.LinFlexioClkDivRatio.List')[1])"!]
            [!FOR "RatioIndex" = "1" TO "count(ecu:list('Lin.LinFlexioClkDivRatio.List'))"!]
                [!VAR "CurrentRatioValue" = "num:i(ecu:list('Lin.LinFlexioClkDivRatio.List')[num:i($RatioIndex)])"!]
                [!FOR "DivCount" = "0" TO "255"!]
                    [!VAR "CurrentBaudValue" = "$ClockFrequency div ($CurrentRatioValue * 2 * ($DivCount + 1))"!]
                    [!IF "$CurrentBaudValue < $Baudrate"!]
                        [!VAR "CurrentErrorValue" = "$Baudrate - $CurrentBaudValue"!]
                    [!ELSE!]
                        [!VAR "CurrentErrorValue" = "$CurrentBaudValue - $Baudrate"!]
                    [!ENDIF!]
                    [!IF "$CurrentErrorValue < $ErrorValue"!]
                        [!VAR "ErrorValue" = "$CurrentErrorValue"!]
                        [!VAR "DivBest" = "$DivCount"!]
                        [!VAR "RatioBest" = "$CurrentRatioValue"!]
                        [!VAR "ActualBaudrate" = "$CurrentBaudValue"!]
                    [!ENDIF!]
                [!ENDFOR!]
            [!ENDFOR!]
        
            [!VAR "TIMCMPL" = "($ClockFrequency div $RatioBest div 2 div $Baudrate) - 1"!]
            [!IF "$TIMCMPL > 255"!]
                [!ERROR!] Lin baud rate divider = [!"$TIMCMPL"!], it should be <= 255, please select a lower MCU clock.[!ENDERROR!]
            [!ENDIF!]
            [!IF "$RatioBest = num:i(256)"!]
                [!VAR "FlexioTIMDEC" = "5"!]
            [!ELSEIF "$RatioBest = num:i(16)"!]
                [!VAR "FlexioTIMDEC" = "4"!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDIF!]
[!CODE!][!//
/**
* 
* @brief          Hardware configuration for Lin Hardware Instance [!"LinHwChannel"!] - Configuration:
*
* 
*/
const Lin_HwConfigType Lin_Ipw_pHwConfigPB_[!"LinChannelId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    [!"LinHwChannel"!],
#if (LIN_WAKEUP_DETECTION == STD_ON)
[!IF "LinChannelWakeupSupport"!][!//
    (boolean)TRUE, /* Wakeup support enabled */ 
[!ELSE!][!//
    (boolean)FALSE, /* Wakeup support disabled */
[!ENDIF!][!//
[!IF "(node:exists(LinChannelEcuMWakeupSource) = 'true') and (node:value(LinChannelEcuMWakeupSource) != '')"!][!//
    (EcuM_WakeupSourceType)((uint32)1UL << [!"as:ref(LinChannelEcuMWakeupSource)/EcuMWakeupSourceId"!]U), /* Wakeup Source transmitted to the Ecu State Manager (used only when Wakeup Support is true) */  
[!ELSE!][!//
    (EcuM_WakeupSourceType)LIN_NONE_ECUM_WAKEUP_SOURCE_REF, /* None Wakeup Source was referred */ 
[!ENDIF!]
#endif
[!IF "contains(node:value(LinNodeType), 'MASTER')"!][!//
    LIN_MASTER_NODE, /* Lin Node Type */
[!ELSE!][!//
    LIN_SLAVE_NODE, /* Lin Node Type */
[!ENDIF!][!//
    [!IF "node:exists(LinFlexioTxControllerRef) and node:exists(LinFlexioRxControllerRef)"!]
    LIN_FLEXIO_CHANNEL, /*Hardware type*/ 
    {   
        NULL_PTR,
        &Flexio_Lin_Ip_pHwConfigPB_[!"LinChannelId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
    }
    [!ELSE!]
    LIN_LPUART_CHANNEL, /*Hardware type*/ 
    {
        &Lpuart_Lin_Ip_pHwConfigPB_[!"LinChannelId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
    #if (STD_ON == LIN_FLEXIO_USED)
        ,
        NULL_PTR
    #endif
    }
    [!ENDIF!]
[!IF "not(node:exists(LinFlexioTxControllerRef) and node:exists(LinFlexioRxControllerRef))"!]
[!IF "$DualClockEnabled = 'true'"!][!//
    ,(uint32)[!"$BaudrateConf_Value"!]U
#if (STD_ON == LIN_FLEXIO_USED)
    ,(uint8)0U /* Timer clock source */
#endif
[!ENDIF!][!//
[!ELSE!]
[!IF "$DualClockEnabled = 'true'"!][!//
    ,(uint16)[!"$DivBest"!] /*!< Baudratedivider */
#if (STD_ON == LIN_FLEXIO_USED)
    ,(uint8)[!"num:i($FlexioTIMDEC)"!] /* Timer clock source */
#endif
[!ENDIF!][!//
[!ENDIF!]
};
[!ENDCODE!]
[!ENDLOOP!][!//
[!ENDNOCODE!]

#define LIN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Lin_MemMap.h"
/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/

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
#endif

/** @} */
