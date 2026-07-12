[!AUTOSPACING!][!//
[!CODE!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : FLEXIO_SENT_IP
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

#ifndef FLEXIO_SENT_IP_CFGDEFINES_H
#define FLEXIO_SENT_IP_CFGDEFINES_H

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
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLEXIO_SENT_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION     4
#define FLEXIO_SENT_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION     4
/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and Mcal.h header file are of the same Autosar version */
    #if ((FLEXIO_SENT_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
           (FLEXIO_SENT_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION  != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Flexio_Sent_Ip_CfgDefines.h and Mcal.h are different"
    #endif
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/



[!NOCODE!][!//
[!SELECT "SentConfigSet"!]
[!CODE!][!WS "0"!]/* Macros that indicate FLEXIO channels used by SENT */[!CR!][!ENDCODE!][!//
[!LOOP "node:order(SentControllerConfig/*/SentChannelConfig/*, 'node:value(SentChannelId)')"!]
    [!CODE!][!WS "0"!]#ifndef [!"node:ref(SentFlexioChannelRef)/../../FlexioMclInstances"!]_CH_[!"num:i(text:split((node:ref(SentFlexioChannelRef)/FlexioMclChannelId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#define [!"node:ref(SentFlexioChannelRef)/../../FlexioMclInstances"!]_CH_[!"num:i(text:split((node:ref(SentFlexioChannelRef)/FlexioMclChannelId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#else[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#error "[!"node:ref(SentFlexioChannelRef)/FlexioMclChannelId"!] cannot be used by SENT driver. Channel locked by another driver!"[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#endif[!CR!][!CR!][!ENDCODE!][!//
[!ENDLOOP!]
[!ENDSELECT!]
[!ENDNOCODE!][!//

[!NOCODE!][!//
[!SELECT "SentConfigSet"!]
[!CODE!][!WS "0"!]/* Macros that indicate FLEXIO pins used by SENT */[!CR!][!ENDCODE!][!//
[!LOOP "node:order(SentControllerConfig/*/SentChannelConfig/*, 'node:value(SentChannelId)')"!]
    [!CODE!][!WS "0"!]#ifndef [!"node:ref(SentFlexioChannelRef)/../../FlexioMclInstances"!]_PIN_[!"num:i((text:split((node:ref(SentFlexioChannelRef)/FlexioMclPinId),'_')[2]))"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#define [!"node:ref(SentFlexioChannelRef)/../../FlexioMclInstances"!]_PIN_[!"num:i((text:split((node:ref(SentFlexioChannelRef)/FlexioMclPinId),'_')[2]))"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#else[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#error "[!"node:ref(SentFlexioChannelRef)/FlexioMclPinId"!] cannot be used by SENT driver. Pin used by another driver!"[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#endif[!CR!][!CR!][!ENDCODE!][!//
[!ENDLOOP!]
[!ENDSELECT!]
[!ENDNOCODE!][!//

[!NOCODE!][!//
[!SELECT "SentConfigSet"!]
[!CODE!][!WS "0"!]/* Macros that indicate FLEXIO channels used by SENT */[!CR!][!ENDCODE!][!//
[!LOOP "node:order(SentControllerConfig/*/SentChannelConfig/*, 'node:value(SentChannelId)')"!]
    [!CODE!][!WS "0"!]#ifdef [!"node:ref(SentFlexioChannelRef)/../../FlexioMclInstances"!]_CH_[!"num:i(text:split((node:ref(SentFlexioChannelRef)/FlexioMclChannelId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#define SENT_CHANNEL_[!"num:i(text:split((node:ref(SentFlexioChannelRef)/FlexioMclChannelId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#else[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#error "[!"node:ref(SentFlexioChannelRef)/FlexioMclChannelId"!] cannot be used by SENT driver. Channel locked by other driver!"[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#endif[!CR!][!CR!][!ENDCODE!][!//
[!ENDLOOP!]
[!ENDSELECT!]
[!ENDNOCODE!][!//

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* FLEXIO_SENT_IP_CFGDEFINES_H */

[!ENDCODE!][!//
