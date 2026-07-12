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
#ifndef WKPU_IP_DEFINES_H
#define WKPU_IP_DEFINES_H

/**
 *   @file    Wkpu_Ip_Defines.h
 *   @version 2.0.1
 *
 *   @brief   AUTOSAR Icu - contains the data exported by the Icu module
 *   @details Contains the information that will be exported by the module, as requested by Autosar.
 *
 *   @addtogroup wkpu_icu_ip WKPU IPL
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
            [!CODE!][!WS "0"!]#include "S32K342_WKPU.h"[!CR!][!ENDCODE!][!//
        [!ELSE!]
            [!CODE!][!WS "0"!]#include "[!"$DerivativeName"!]_WKPU.h"[!CR!][!ENDCODE!][!//
        [!ENDIF!][!//
    [!ENDIF!][!//

    [!IF "contains($DerivativeName, 'S32R')"!][!//
        [!CODE!][!WS "0"!]#include "[!"$DerivativeName"!]_WKPU.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
    [!IF "contains($DerivativeName, 'S32G2')"!][!//
        [!CODE!][!WS "0"!]#include "S32G274A_WKPU.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
    [!IF "contains($DerivativeName, 'S32G3')"!][!//
        [!CODE!][!WS "0"!]#include "S32G399A_WKPU.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDIF!][!//

[!ENDNOCODE!][!//

/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 *================================================================================================*/

#define WKPU_IP_DEFINES_VENDOR_ID                    43
#define WKPU_IP_DEFINES_AR_RELEASE_MAJOR_VERSION     4
#define WKPU_IP_DEFINES_AR_RELEASE_MINOR_VERSION     4
#define WKPU_IP_DEFINES_AR_RELEASE_REVISION_VERSION  0
#define WKPU_IP_DEFINES_SW_MAJOR_VERSION             2
#define WKPU_IP_DEFINES_SW_MINOR_VERSION             0
#define WKPU_IP_DEFINES_SW_PATCH_VERSION             1

/*==================================================================================================
 *                                      FILE VERSION CHECKS
 *================================================================================================*/
/* Check if header file and StandardTypes.h file are of the same Autosar version */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((WKPU_IP_DEFINES_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
         (WKPU_IP_DEFINES_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Wkpu_Ip_Defines.h and StandardTypes.h are different"
    #endif
#endif

/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/

[!NOCODE!]
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!IF "contains($DerivativeName, 'SAF85')"!][!//
        [!CODE!][!WS "0"!]#define WKPU_IP_DERIVATIVE_NOT_SUPPORT_WKPU[!CR!][!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDIF!][!//

[!CODE!][!WS "0"!]#ifndef WKPU_IP_DERIVATIVE_NOT_SUPPORT_WKPU[!CR!][!ENDCODE!][!//

[!VAR "CoreCnt" = "num:i(0)"!]
[!SELECT "IcuConfigSet"!][!//
    [!IF "count(IcuWkpu/*) > '0'"!]
        [!LOOP "IcuWkpu/*"!]
            [!VAR "CoreCnt" = "num:i(count(IcuWkpuNMIConfiguration/*))"!]
        [!ENDLOOP!]
    [!ENDIF!]
[!ENDSELECT!]
[!CODE!]#define WKPU_IP_NMI_API               [!IF "(num:i($CoreCnt))!= (num:i(0))"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!][!CR!][!ENDCODE!]
[!VAR "NumCores" = "num:i(count(ecu:list('Icu.IcuConfigSet.IcuChannel.IcuHwChannel.IcuWkpuCoresSupport')) - 1)"!]
[!CODE!]/** @brief The number core support for WKPU module */[!CR!][!ENDCODE!]
[!CODE!]#if (defined (WKPU_IP_NMI_API) && (STD_ON == WKPU_IP_NMI_API))[!CR!][!ENDCODE!]
[!CODE!]#define WKPU_IP_NMI_NUM_CORES                  ([!"$NumCores"!]U)[!CR!][!ENDCODE!]
[!CODE!]#endif[!CR!][!ENDCODE!]
[!CODE!]#if (defined (WKPU_IP_NMI_API) && (STD_ON == WKPU_IP_NMI_API))[!CR!][!ENDCODE!]
[!CODE!]/** @brief The WKPU core array */[!CR!][!ENDCODE!]
[!CODE!]#define WKPU_IP_CORE_ARRAY \[!CR!][!ENDCODE!]
[!CODE!]{                          \[!CR!][!ENDCODE!]
[!FOR "CoreIdx" = "1" TO "$NumCores"!]
    [!VAR "CoreID" = "num:i(ecu:list('Icu.IcuConfigSet.IcuChannel.IcuHwChannel.IcuWkpuCoresSupport')[num:i($CoreIdx)])"!]
    [!CODE!][!WS "4"!]WKPU_CORE[!"$CoreID"!]    /*!< Core [!"$CoreID"!] */[!ENDCODE!]
    [!IF "(num:i($CoreIdx))<(num:i($NumCores))"!][!CODE!],[!ENDCODE!][!ENDIF!][!CODE!]\[!CR!][!ENDCODE!]
[!ENDFOR!]
[!CODE!]}[!CR!][!ENDCODE!]
[!CODE!]#endif[!CR!][!ENDCODE!]
[!IF "(num:i($CoreCnt))!= (num:i(0))"!]
    [!CODE!]#define WKPU_IP_NMI_CORE_CNT          ([!"$CoreCnt"!]U)[!CR!][!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

#if (defined (WKPU_IP_NMI_API) && (STD_ON == WKPU_IP_NMI_API))
/*! @brief The distance between cores */
#define     WKPU_IP_CORE_OFFSET_SIZE            (8U)
#define     WKPU_IP_SUPPORT_NONE_REQUEST
#define     WKPU_IP_SUPPORT_NON_MASK_INT
#endif
#define     WKPU_IP_SUPPORT_INTERRUPT_REQUEST
#define     WKPU_IP_NUM_OF_CHANNELS             ([!"ecu:get('Icu.Num_Wkpu_Hw_Channels')"!]U)

[!IF "num:i(ecu:get('Icu.Num_Wkpu_Hw_Channels')) > '32'"!]
    [!CODE!][!//
#define     WKPU_IP_NUM_OF_CHANNELS_IN_ONE_REG  (32U)
    [!ENDCODE!][!//
[!ELSE!]
    [!CODE!][!//
#define     WKPU_IP_NUM_OF_CHANNELS_IN_ONE_REG  ([!"ecu:get('Icu.Num_Wkpu_Hw_Channels')"!]U)
    [!ENDCODE!][!//
[!ENDIF!]

[!IF "node:exists(IcuAutosarExt/IcuWkpuStandbyWakeupSupport)"!][!//
/** @brief Switch for enabling Standby wakeup support  on or off */
#define     WKPU_IP_STANDBY_WAKEUP_SUPPORT      ([!IF "node:exists(IcuAutosarExt/IcuWkpuStandbyWakeupSupport)"!][!IF "IcuAutosarExt/IcuWkpuStandbyWakeupSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ELSE!]STD_OFF[!ENDIF!])
[!ENDIF!][!//

[!// Define used to see if S32R45 or S32R41 derivative is used.
[!IF "((ecu:get('Icu.Derivative')) = 'S32R45') or ((ecu:get('Icu.Derivative')) = 'S32R41')"!][!//
    [!CODE!][!WS "0"!]#define WKPU_DERIVATIVE_SUPPORT_ONLY_NMI        (STD_ON)[!CR!][!ENDCODE!][!//
[!ENDIF!][!//
#endif /* WKPU_IP_DERIVATIVE_NOT_SUPPORT_WKPU */

#ifdef __cplusplus
}
#endif

/** @} */

#endif  /* WKPU_IP_DEFINES_H */
[!ENDCODE!][!//
