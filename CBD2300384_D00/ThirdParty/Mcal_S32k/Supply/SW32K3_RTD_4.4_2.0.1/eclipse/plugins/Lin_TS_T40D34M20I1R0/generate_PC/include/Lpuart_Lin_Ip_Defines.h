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
 
#ifndef LPUART_LIN_IP_DEFINES_H
#define LPUART_LIN_IP_DEFINES_H

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
#include "Mcal.h"
[!NOCODE!][!// Include specific header file
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!VAR "DerivativeName" = "text:toupper(substring-before(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative,'_'))"!]
    [!IF "contains($DerivativeName, 'S32K3') or contains($DerivativeName, 'S32K1')"!][!//
        [!IF "contains($DerivativeName, 'S32K341')"!][!//
[!CODE!][!WS "0"!]
#include "S32K342_LPUART.h"
[!CR!][!ENDCODE!][!//
        [!ELSE!][!//
[!CODE!][!WS "0"!]
#include "[!"$DerivativeName"!]_LPUART.h"
[!CR!][!ENDCODE!][!//        
        [!ENDIF!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define LPUART_LIN_IP_DEFINES_VENDOR_ID                    43
#define LPUART_LIN_IP_DEFINES_AR_RELEASE_MAJOR_VERSION     4
#define LPUART_LIN_IP_DEFINES_AR_RELEASE_MINOR_VERSION     4
#define LPUART_LIN_IP_DEFINES_AR_RELEASE_REVISION_VERSION  0
#define LPUART_LIN_IP_DEFINES_SW_MAJOR_VERSION             2
#define LPUART_LIN_IP_DEFINES_SW_MINOR_VERSION             0
#define LPUART_LIN_IP_DEFINES_SW_PATCH_VERSION             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and Mcal.h header file are of the same Autosar version */
    #if ((LPUART_LIN_IP_DEFINES_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (LPUART_LIN_IP_DEFINES_AR_RELEASE_MINOR_VERSION != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Lpuart_Lin_Ip_Defines.h and Mcal.h are different"
    #endif
#endif /* DISABLE_MCAL_INTERMODULE_ASR_CHECK */
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
 *                                     DEFINES AND MACROS
==================================================================================================*/
[!NOCODE!]
[!VAR "NumHwChannelConfig" = "0"!]
[!LOOP "LinGlobalConfig/LinChannel/*"!][!//
[!IF "contains(node:value(LinHwChannel),'LPUART')"!][!//
    [!VAR "NumHwChannelConfig" = "$NumHwChannelConfig + 1"!]
[!ENDIF!]
[!IF "$NumHwChannelConfig = 0"!][!//When Lpuart is not configured, NumHwChannelConfig should be set to 1 to avoid build failures when the number of array elements is zero.
    [!VAR "NumHwChannelConfig" = "1"!]
[!ENDIF!]
[!ENDLOOP!][!//
[!ENDNOCODE!]
/**
* @brief   Switches the Development Error Detection and Notification ON or OFF.
*
* 
*/
#define LPUART_LIN_IP_DEV_ERROR_DETECT               ([!IF "LinGeneral/LinDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]) /*!< Development error detection */

/**
* @brief   No of Channels configured for Lpuart
*
* 
*/
#define LPUART_LIN_IP_NUMBER_OF_INSTANCES         ([!"num:i($NumHwChannelConfig)"!]U)


/**
* @brief          Lin Osif source counter
* @details        This parameter is used to select between different OsIf counter implementation
*
* 
*/
#define  LPUART_LIN_IP_TIMEOUT_TYPE       ([!"LinGeneral/LinTimeoutMethod"!])

/**
* @brief   Number of loops before returning LPUART_STATUS_TIMEOUT.
*
* 
*/
#define LPUART_LIN_IP_TIMEOUT_VALUE_US    ([!"LinGeneral/LinTimeoutDuration"!]U)

/**
* @brief   Enable/Disable Autobaud feature. This feature only support for IPV layer
* So HLD layer always disable for this feature.
*
*
*/
#define LPUART_LIN_IP_AUTO_BAUD           (STD_OFF)

/**
* @brief   Enable/Disable timeout feature.
*
*
*/
#define LPUART_LIN_IP_FRAME_TIMEOUT_DISABLE        [!IF "AutosarExt/LinFrameTimeoutDisable = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

[!NOCODE!]
[!IF "node:fallback(AutosarExt/LinFrameTimeoutDisable, 'false') = 'false'"!]
    [!IF "node:exists(AutosarExt/LinLpuartStartTimerNotification)"!]
        [!IF "node:value(AutosarExt/LinLpuartStartTimerNotification) != 'NULL_PTR'"!]
            [!CODE!]
/**
* @brief   Notification function to start timeout counter.
*
*
*/
#define LPUART_LIN_IP_START_TIMER_NOTIFICATION       [!"AutosarExt/LinLpuartStartTimerNotification"!]
[!ENDCODE!]
        [!ENDIF!]
    [!ENDIF!]

    [!IF "node:exists(AutosarExt/LinLpuartStopTimerNotification)"!]
        [!IF "node:value(AutosarExt/LinLpuartStopTimerNotification) != 'NULL_PTR'"!]
            [!CODE!]
/**
* @brief   Notification function to end timeout counter.
*
*
*/
#define LPUART_LIN_IP_STOP_TIMER_NOTIFICATION         [!"AutosarExt/LinLpuartStopTimerNotification"!]
[!ENDCODE!]
        [!ENDIF!]
    [!ENDIF!]
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
#ifdef __cplusplus
}
#endif

/** @} */

#endif 
