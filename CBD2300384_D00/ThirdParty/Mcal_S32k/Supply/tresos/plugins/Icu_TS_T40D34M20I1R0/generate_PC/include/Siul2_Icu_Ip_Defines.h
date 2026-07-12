[!CODE!][!//
[!AUTOSPACING!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Emios Siul2 Wkpu LpCmp
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

#ifndef SIUL2_ICU_IP_DEFINES_H
#define SIUL2_ICU_IP_DEFINES_H

/**
 *   @file       Siul2_Icu_Ip_Defines.h
 *   @version    2.0.1
 *
 *   @brief      AUTOSAR Icu - contains the data exported by the Icu module
 *   @details    Contains the information that will be exported by the module, as requested by Autosar.
 *
 *   @addtogroup siul2_icu_ip SIUL2 IPL
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif

 /*==================================================================================================
 *                                         INCLUDE FILES
 * 1) system and project includes
 * 2) needed interfaces from external units
 * 3) internal and external interfaces from this unit
 *================================================================================================*/
#include "StandardTypes.h"
[!NOCODE!][!// Include specific header file

[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!VAR "DerivativeName" = "text:toupper(substring-before(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative,'_'))"!]
    [!CODE!][!WS "0"!]/* Include platform header file. */[!CR!][!ENDCODE!][!//

    [!IF "contains($DerivativeName, 'S32K3')"!][!//
        [!IF "contains($DerivativeName, 'S32K341') or contains($DerivativeName, 'S32K342')"!][!//
            [!CODE!][!WS "0"!]#include "S32K342_SIUL2.h"[!CR!][!ENDCODE!][!//
        [!ELSE!]
            [!CODE!][!WS "0"!]#include "[!"$DerivativeName"!]_SIUL2.h"[!CR!][!ENDCODE!][!//
        [!ENDIF!][!//
    [!ENDIF!][!//

    [!IF "contains($DerivativeName, 'S32R')"!][!//
        [!CODE!][!WS "0"!]#include "[!"$DerivativeName"!]_SIUL2.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
    [!IF "contains($DerivativeName, 'SAF85')"!][!//
        [!CODE!][!WS "0"!]#include "SAF85XX_SIUL2.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
    [!IF "contains($DerivativeName, 'S32G2')"!][!//
        [!CODE!][!WS "0"!]#include "S32G274A_SIUL2.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
    [!IF "contains($DerivativeName, 'S32E27')"!][!//
        [!CODE!][!WS "0"!]#include "S32E27_SIUL2.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
    [!IF "contains($DerivativeName, 'S32S27')"!][!//
        [!CODE!][!WS "0"!]#include "S32S27_SIUL2.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
    [!IF "contains($DerivativeName, 'S32Z27')"!][!//
        [!CODE!][!WS "0"!]#include "S32Z27_SIUL2.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
    [!IF "contains($DerivativeName, 'S32G3')"!][!//
        [!CODE!][!WS "0"!]#include "S32G399A_SIUL2.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDIF!][!//

[!ENDNOCODE!][!//

/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 *================================================================================================*/
#define SIUL2_ICU_IP_DEFINES_VENDOR_ID                      43
#define SIUL2_ICU_IP_DEFINES_AR_RELEASE_MAJOR_VERSION       4
#define SIUL2_ICU_IP_DEFINES_AR_RELEASE_MINOR_VERSION       4
#define SIUL2_ICU_IP_DEFINES_AR_RELEASE_REVISION_VERSION    0
#define SIUL2_ICU_IP_DEFINES_SW_MAJOR_VERSION               2
#define SIUL2_ICU_IP_DEFINES_SW_MINOR_VERSION               0
#define SIUL2_ICU_IP_DEFINES_SW_PATCH_VERSION               1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if header file and StandardTypes.h file are of the same Autosar version */
    #if ((SIUL2_ICU_IP_DEFINES_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
         (SIUL2_ICU_IP_DEFINES_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Siul2_Icu_Ip_Defines.h and StandardTypes.h are different"
    #endif
#endif

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/** @brief Switches the Development Error Detection and Notification on or off.  */
#define SIUL2_ICU_IP_DEV_ERROR_DETECT        ([!IF "IcuGeneral/IcuDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief The number of channels available on each instance */
#define SIUL2_ICU_IP_NUM_OF_CHANNELS         ([!"ecu:get('Icu.Num_Siul2_Hw_Channels')"!]U)

/** @brief Adds or removes all services related to mode set functionality. */
#define SIUL2_ICU_IP_SET_MODE_API               ([!IF "IcuOptionalApis/IcuSetModeApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief Adds or removes all services related to dual clock edge functionality. */
#define SIUL2_ICU_IP_DUAL_CLOCK_MODE_API        ([!IF "IcuAutosarExt/IcuEnableDualClockMode"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief Add or remove all functions related to input state. */
#define SIUL2_ICU_IP_GET_INPUT_STATE_API        ([!IF "IcuOptionalApis/IcuGetInputStateApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief Support for User mode. If this parameter has been configured to 'STD_ON', the FTM driver 
 *         code can be executed from both supervisor and user mode. */
#define SIUL2_ICU_IP_ENABLE_USER_MODE_SUPPORT   ([!IF "IcuGeneral/IcuEnableUserModeSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Verification for user mode support. */
#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    #if (defined (SIUL2_ICU_IP_ENABLE_USER_MODE_SUPPORT) && (STD_ON == SIUL2_ICU_IP_ENABLE_USER_MODE_SUPPORT))
        #error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running Icu in user mode the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined
    #endif
#endif

#ifdef __cplusplus
}
#endif

/** @} */

#endif  /* SIUL2_ICU_IP_DEFINES_H */
[!ENDCODE!][!//
