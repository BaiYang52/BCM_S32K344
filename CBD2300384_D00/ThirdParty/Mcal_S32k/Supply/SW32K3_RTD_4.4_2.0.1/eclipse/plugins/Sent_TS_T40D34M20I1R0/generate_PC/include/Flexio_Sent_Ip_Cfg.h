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


/**
*   @file    Flexio_Sent_Ip_Cfg.h
*
*   @version 2.0.1
*
*   @brief   AUTOSAR Sent - module interface
*
*   @addtogroup SENT_DRIVER
*   @{
*/


#ifndef FLEXIO_SENT_IP_CFG_H
#define FLEXIO_SENT_IP_CFG_H

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Mcal.h"
#include "Flexio_Sent_Ip_Types.h"
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!]#include "Flexio_Sent_Ip_[!"."!]_PBcfg.h"[!CR!][!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!]#include "Flexio_Sent_Ip_PBcfg.h"[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

#include "[!"ecu:get('Sent.SentHeaderNameFLEXIO')"!].h"
[!AUTOSPACING!]

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/*
* @file           Flexio_Sent_Ip_Cfg.h
*/
#define FLEXIO_SENT_IP_CFG_VENDOR_ID                       43
#define FLEXIO_SENT_IP_CFG_AR_RELEASE_MAJOR_VERSION        4
#define FLEXIO_SENT_IP_CFG_AR_RELEASE_MINOR_VERSION        4
#define FLEXIO_SENT_IP_CFG_AR_RELEASE_REVISION_VERSION     0
#define FLEXIO_SENT_IP_CFG_SW_MAJOR_VERSION                2
#define FLEXIO_SENT_IP_CFG_SW_MINOR_VERSION                0
#define FLEXIO_SENT_IP_CFG_SW_PATCH_VERSION                1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!][!//
/* Checks against Flexio_Sent_Ip_[!"."!]_PBcfg.h */
#if (FLEXIO_SENT_IP_CFG_VENDOR_ID != FLEXIO_SENT_IP_PBCFG_[!"."!]_VENDOR_ID)
    #error "Flexio_Sent_Ip_Cfg.h and Flexio_Sent_Ip_[!"."!]_PBcfg.h have different vendor ids"
#endif
#if ((FLEXIO_SENT_IP_CFG_AR_RELEASE_MAJOR_VERSION    != FLEXIO_SENT_IP_PBCFG_[!"."!]_AR_RELEASE_MAJOR_VERSION) || \
     (FLEXIO_SENT_IP_CFG_AR_RELEASE_MINOR_VERSION    != FLEXIO_SENT_IP_PBCFG_[!"."!]_AR_RELEASE_MINOR_VERSION) || \
     (FLEXIO_SENT_IP_CFG_AR_RELEASE_REVISION_VERSION != FLEXIO_SENT_IP_PBCFG_[!"."!]_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Flexio_Sent_Ip_Cfg.h and Flexio_Sent_Ip_[!"."!]_PBcfg.h are different"
#endif
#if ((FLEXIO_SENT_IP_CFG_SW_MAJOR_VERSION != FLEXIO_SENT_IP_PBCFG_[!"."!]_SW_MAJOR_VERSION) || \
     (FLEXIO_SENT_IP_CFG_SW_MINOR_VERSION != FLEXIO_SENT_IP_PBCFG_[!"."!]_SW_MINOR_VERSION) || \
     (FLEXIO_SENT_IP_CFG_SW_PATCH_VERSION != FLEXIO_SENT_IP_PBCFG_[!"."!]_SW_PATCH_VERSION))
    #error "Software Version Numbers of Flexio_Sent_Ip_Cfg.h and Flexio_Sent_Ip_[!"."!]_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!][!//
/* Checks against Flexio_Sent_Ip_PBcfg.h */
#if (FLEXIO_SENT_IP_CFG_VENDOR_ID != FLEXIO_SENT_IP_PBCFG_VENDOR_ID)
    #error "Flexio_Sent_Ip_Cfg.h and Flexio_Sent_Ip_PBcfg.h have different vendor ids"
#endif
#if ((FLEXIO_SENT_IP_CFG_AR_RELEASE_MAJOR_VERSION    != FLEXIO_SENT_IP_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (FLEXIO_SENT_IP_CFG_AR_RELEASE_MINOR_VERSION    != FLEXIO_SENT_IP_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (FLEXIO_SENT_IP_CFG_AR_RELEASE_REVISION_VERSION != FLEXIO_SENT_IP_PBCFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Flexio_Sent_Ip_Cfg.h and Flexio_Sent_Ip_PBcfg.h are different"
#endif
#if ((FLEXIO_SENT_IP_CFG_SW_MAJOR_VERSION != FLEXIO_SENT_IP_PBCFG_SW_MAJOR_VERSION) || \
     (FLEXIO_SENT_IP_CFG_SW_MINOR_VERSION != FLEXIO_SENT_IP_PBCFG_SW_MINOR_VERSION) || \
     (FLEXIO_SENT_IP_CFG_SW_PATCH_VERSION != FLEXIO_SENT_IP_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Flexio_Sent_Ip_Cfg.h and Flexio_Sent_Ip_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and Mcal.h header file are of the same Autosar version */
    #if ((FLEXIO_SENT_IP_CFG_AR_RELEASE_MAJOR_VERSION  != MCAL_AR_RELEASE_MAJOR_VERSION) || \
           (FLEXIO_SENT_IP_CFG_AR_RELEASE_MINOR_VERSION  != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Flexio_Sent_Ip_Cfg.h and Mcal.h are different"
    #endif
#endif
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/
[!INDENT "0"!]
[!NOCODE!]
[!CODE!]#define FLEXIO_SENT_IP_CONFIG_EXT \[!CR!][!ENDCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!][!//
    [!CODE!][!WS "4"!]FLEXIO_SENT_IP_CONFIG_[!"."!]_PB \[!CR!][!ENDCODE!]
[!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS "4"!]FLEXIO_SENT_IP_CONFIG_PB[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

[!SELECT "SentConfigSet"!]
    [!LOOP "SentControllerConfig/*"!]
        [!IF "SentControllerActivation = 'false'"!]
            /*
            * @brief          Define SENT_FLEXIO_NULL_OFFSET_U8 when Sent Controller Activation disable
            *
            */
            #define SENT_FLEXIO_NULL_OFFSET_U8[!WS "4"!]((uint8)255U)[!CR!]
        [!ENDIF!]
    [!ENDLOOP!]
[!ENDSELECT!]

/*
* @brief          Generate ISR defines for each controller Combined
* @details        Generate ISR defines for all controllers with all channels.
*
*/
[!NOCODE!]
    [!SELECT "SentConfigSet"!]
        [!VAR "ChnlIsrFlag" = "'false'"!]
        [!LOOP "SentControllerConfig/*"!]
            [!IF "SentProcessing = 'INTERRUPT'"!]
                [!LOOP "SentChannelConfig/*"!]
                    [!IF "(SentFastNotificationEnable = 'true')"!]
                        [!IF "(SentFastNotification != 'NULL_PTR')"!]
                            [!VAR "ChnlIsrFlag" = "'true'"!] [!BREAK!]
                        [!ENDIF!]
                    [!ENDIF!]
                    [!IF "(SentSlowNotificationEnable = 'true')"!]
                        [!IF "(SentSlowNotification != 'NULL_PTR')"!]
                            [!VAR "ChnlIsrFlag" = "'true'"!] [!BREAK!]
                        [!ENDIF!]
                    [!ENDIF!]
                [!ENDLOOP!]
                [!IF "$ChnlIsrFlag = 'true'"!]
                    [!CODE!]#define FXIO_ISR_PROCESS_MESSAGE_HANDLER_CTRL[!"text:split((node:ref(SentHwControllerRef)/FlexioMclInstances),'_')[2]"!]_COMBINED[!CR!][!ENDCODE!]
                [!ENDIF!]
            [!ENDIF!]
            [!VAR "ChnlPollingFlag" = "'false'"!]
            [!IF "SentProcessing = 'POLLING'"!]
                [!LOOP "SentChannelConfig/*"!]
                    [!IF "(SentFastNotificationEnable = 'true')"!]
                        [!IF "(SentFastNotification != 'NULL_PTR')"!]
                            [!VAR "ChnlPollingFlag" = "'true'"!] [!BREAK!]
                        [!ENDIF!]
                    [!ENDIF!]
                    [!IF "(SentSlowNotificationEnable = 'true')"!]
                        [!IF "(SentSlowNotification != 'NULL_PTR')"!]
                            [!VAR "ChnlPollingFlag" = "'true'"!] [!BREAK!]
                        [!ENDIF!]
                    [!ENDIF!]
                [!ENDLOOP!]
                [!IF "$ChnlPollingFlag = 'true'"!]
                    [!CODE!]#define FXIO_ISR_PROCESS_TIMER_CTRL[!"text:split((node:ref(SentHwControllerRef)/FlexioMclInstances),'_')[2]"!][!CR!][!ENDCODE!]
                [!ENDIF!]
            [!ENDIF!]
        [!ENDLOOP!]
    [!ENDSELECT!]
[!ENDNOCODE!]

[!ENDINDENT!]


/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                            ENUMS
==================================================================================================*/

/*==================================================================================================
*                               STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

[!SELECT "SentConfigSet"!]
    [!NOCODE!]
        [!VAR "SENT_CONTROLLER_CONFIG_CNT" = "num:i(0)"!]
        [!VAR "SENT_CHANNEL_CONFIG_CNT" = "num:i(0)"!]
        [!VAR "SENT_CONTROLLER_CONFIGURED" = "num:i(count(SentControllerConfig/*))"!]
        [!VAR "SENT_CHANNEL_CONFIG_CONFIGURED" = "num:i(count(SentControllerConfig/*/SentChannelConfig/*))"!]
[!ENDNOCODE!]
[!IF "../SentGeneral/SentDmaActivation = 'true'"!]
#define SENT_START_SEC_VAR_CLEARED_32_NO_CACHEABLE
#include "Sent_MemMap.h"

#if(STD_ON == FEATURE_FLEXIO_DMA_ENABLE)
[!LOOP "node:order(SentControllerConfig/*/SentChannelConfig/*, 'node:value(SentChannelId)')"!]
[!IF "SentSyncAsyncSelection = 'SENT_PAUSE_PULSE_DISABLED'"!]
    [!VAR "PausePulseEnable" = "0"!]
[!ELSE!]
    [!VAR "PausePulseEnable" = "1"!]
[!ENDIF!]   
[!VAR "DmaBufferLength" = "num:i(num:i(DataLength + 3 + $PausePulseEnable)) * num:i(DmaBufferDepth)"!]
extern uint32 Flexio_Sent_Ip_DmaTimerValue_[!"../../SentControllerId"!]_[!"num:i(SentChannelId)"!][[!"num:i($DmaBufferLength)"!]U];
[!ENDLOOP!]
#endif

#define SENT_STOP_SEC_VAR_CLEARED_32_NO_CACHEABLE
#include "Sent_MemMap.h"
[!ENDIF!]
[!ENDSELECT!]
/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
[!INDENT "0"!]

[!VAR "AllFastCallbackFunctionsArray" = "concat('', ' ')"!]
    [!LOOP "SentConfigSet/SentControllerConfig/*/SentChannelConfig/*"!]
        [!IF "(SentFastNotificationEnable = 'true')"!]
            [!IF "(SentFastNotification != 'NULL_PTR')"!]
                [!VAR "EachFastCallbackFunctionsArray" = "SentFastNotification"!]
                [!VAR "EachFastCallbackFunctionsArray" = "concat($EachFastCallbackFunctionsArray, ' ')"!]
                [!VAR "AllFastCallbackFunctionsArray" = "concat($AllFastCallbackFunctionsArray, $EachFastCallbackFunctionsArray)"!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
[!NOCODE!]
    [!LOOP "SentConfigSet/SentControllerConfig/*/SentChannelConfig/*"!]
        [!IF "(SentFastNotificationEnable = 'true')"!]
            [!IF "(SentFastNotification != 'NULL_PTR')"!]
                [!VAR "EachFastCallbackFunctionsArray" = "SentFastNotification"!]
                [!VAR "verify" = "text:uniq(text:split($AllFastCallbackFunctionsArray), $EachFastCallbackFunctionsArray)"!]
                [!VAR "verifyContains" = "text:contains(text:split($AllFastCallbackFunctionsArray), $EachFastCallbackFunctionsArray)"!]
                [!IF "$verifyContains"!]
                    [!IF "$verify"!]
[!CODE!]
#define SENT_START_SEC_CODE
#include "Sent_MemMap.h"
extern void [!"SentFastNotification"!] (Flexio_Sent_Ip_StatusType Event, Flexio_Sent_FastMsgType * SentFastMsg);
#define SENT_STOP_SEC_CODE
#include "Sent_MemMap.h"
[!ENDCODE!]
                    [!ELSE!]
[!CODE!]
#define SENT_START_SEC_CODE
#include "Sent_MemMap.h"
extern void [!"SentFastNotification"!] (Flexio_Sent_Ip_StatusType Event, Flexio_Sent_FastMsgType * SentFastMsg);
#define SENT_STOP_SEC_CODE
#include "Sent_MemMap.h"
[!ENDCODE!]
                    [!VAR "AllFastCallbackFunctionsArray" = "text:replaceAll($AllFastCallbackFunctionsArray, $EachFastCallbackFunctionsArray, ' ')"!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
[!ENDNOCODE!]

[!VAR "AllSlowCallbackFunctionsArray" = "concat('', ' ')"!]
    [!LOOP "SentConfigSet/SentControllerConfig/*/SentChannelConfig/*"!]
        [!IF "(SentSlowNotificationEnable = 'true')"!]
            [!IF "(SentSlowNotification != 'NULL_PTR')"!]
                [!VAR "EachSlowCallbackFunctionsArray" = "SentSlowNotification"!]
                [!VAR "EachSlowCallbackFunctionsArray" = "concat($EachSlowCallbackFunctionsArray, ' ')"!]
                [!VAR "AllSlowCallbackFunctionsArray" = "concat($AllSlowCallbackFunctionsArray, $EachSlowCallbackFunctionsArray)"!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
[!NOCODE!]
    [!LOOP "SentConfigSet/SentControllerConfig/*/SentChannelConfig/*"!]
        [!IF "(SentSlowNotificationEnable = 'true')"!]
            [!IF "(SentSlowNotification != 'NULL_PTR')"!]
                [!VAR "EachSlowCallbackFunctionsArray" = "SentSlowNotification"!]
                [!VAR "verify" = "text:uniq(text:split($AllSlowCallbackFunctionsArray), $EachSlowCallbackFunctionsArray)"!]
                [!VAR "verifyContains" = "text:contains(text:split($AllSlowCallbackFunctionsArray), $EachSlowCallbackFunctionsArray)"!]
                [!IF "$verifyContains"!]
                    [!IF "$verify"!]
[!CODE!]
#define SENT_START_SEC_CODE
#include "Sent_MemMap.h"
extern void [!"SentSlowNotification"!] (Flexio_Sent_Ip_StatusType Event, Flexio_Sent_SerialMsgType * SentSerialMsg);
#define SENT_STOP_SEC_CODE
#include "Sent_MemMap.h"
[!ENDCODE!]
                    [!ELSE!]
[!CODE!]
#define SENT_START_SEC_CODE
#include "Sent_MemMap.h"
extern void [!"SentSlowNotification"!] (Flexio_Sent_Ip_StatusType Event, Flexio_Sent_SerialMsgType * SentSerialMsg);
#define SENT_STOP_SEC_CODE
#include "Sent_MemMap.h"
[!ENDCODE!]
                        [!VAR "AllSlowCallbackFunctionsArray" = "text:replaceAll($AllSlowCallbackFunctionsArray, $EachSlowCallbackFunctionsArray, ' ')"!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
[!ENDNOCODE!]

[!VAR "AllSlowCrcCallbackFunctionsArray" = "concat('', ' ')"!]
    [!LOOP "SentConfigSet/SentControllerConfig/*/SentChannelConfig/*"!]
        [!IF "(SentSlowCRCErrorNotificationEnable = 'true')"!]
            [!IF "(SentSlowCRCErrorNotification != 'NULL_PTR')"!]
                [!VAR "EachSlowCRCCallbackFunctionsArray" = "SentSlowCRCErrorNotification"!]
                [!VAR "EachSlowCRCCallbackFunctionsArray" = "concat($EachSlowCRCCallbackFunctionsArray, ' ')"!]
                [!VAR "AllSlowCrcCallbackFunctionsArray" = "concat($AllSlowCrcCallbackFunctionsArray, $EachSlowCRCCallbackFunctionsArray)"!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
[!NOCODE!]
    [!LOOP "SentConfigSet/SentControllerConfig/*/SentChannelConfig/*"!]
        [!IF "(SentSlowCRCErrorNotificationEnable = 'true')"!]
            [!IF "(SentSlowCRCErrorNotification != 'NULL_PTR')"!]
                [!VAR "EachSlowCRCCallbackFunctionsArray" = "SentSlowCRCErrorNotification"!]
                [!VAR "verify" = "text:uniq(text:split($AllSlowCrcCallbackFunctionsArray), $EachSlowCRCCallbackFunctionsArray)"!]
                [!VAR "verifyContains" = "text:contains(text:split($AllSlowCrcCallbackFunctionsArray), $EachSlowCRCCallbackFunctionsArray)"!]
                [!IF "$verifyContains"!]
                    [!IF "$verify"!]
[!CODE!]
#define SENT_START_SEC_CODE
#include "Sent_MemMap.h"
extern void [!"SentSlowCRCErrorNotification"!](Flexio_Sent_Ip_ErrorStatusType ErrorEvent);
#define SENT_STOP_SEC_CODE
#include "Sent_MemMap.h"
[!ENDCODE!]
                    [!ELSE!]
[!CODE!]
#define SENT_START_SEC_CODE
#include "Sent_MemMap.h"
extern void [!"SentSlowCRCErrorNotification"!](Flexio_Sent_Ip_ErrorStatusType ErrorEvent);
#define SENT_STOP_SEC_CODE
#include "Sent_MemMap.h"
[!ENDCODE!]
                    [!VAR "AllSlowCrcCallbackFunctionsArray" = "text:replaceAll($AllSlowCrcCallbackFunctionsArray, $EachSlowCRCCallbackFunctionsArray, ' ')"!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
[!ENDNOCODE!]

[!VAR "AllFastCrcCallbackFunctionsArray" = "concat('', ' ')"!]
    [!LOOP "SentConfigSet/SentControllerConfig/*/SentChannelConfig/*"!]
        [!IF "(SentFastCRCErrorNotificationEnable = 'true')"!]
            [!IF "(SentFastCRCErrorNotification != 'NULL_PTR')"!]
                [!VAR "EachFastCRCCallbackFunctionsArray" = "SentFastCRCErrorNotification"!]
                [!VAR "EachFastCRCCallbackFunctionsArray" = "concat($EachFastCRCCallbackFunctionsArray, ' ')"!]
                [!VAR "AllFastCrcCallbackFunctionsArray" = "concat($AllFastCrcCallbackFunctionsArray, $EachFastCRCCallbackFunctionsArray)"!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
[!NOCODE!]
    [!LOOP "SentConfigSet/SentControllerConfig/*/SentChannelConfig/*"!]
        [!IF "(SentFastCRCErrorNotificationEnable = 'true')"!]
            [!IF "(SentFastCRCErrorNotification != 'NULL_PTR')"!]
                [!VAR "EachFastCRCCallbackFunctionsArray" = "SentFastCRCErrorNotification"!]
                [!VAR "verify" = "text:uniq(text:split($AllFastCrcCallbackFunctionsArray), $EachFastCRCCallbackFunctionsArray)"!]
                [!VAR "verifyContains" = "text:contains(text:split($AllFastCrcCallbackFunctionsArray), $EachFastCRCCallbackFunctionsArray)"!]
                [!IF "$verifyContains"!]
                    [!IF "$verify"!]
[!CODE!]
#define SENT_START_SEC_CODE
#include "Sent_MemMap.h"
extern void [!"SentFastCRCErrorNotification"!](Flexio_Sent_Ip_ErrorStatusType ErrorEvent);
#define SENT_STOP_SEC_CODE
#include "Sent_MemMap.h"
[!ENDCODE!]
                    [!ELSE!]
[!CODE!]
#define SENT_START_SEC_CODE
#include "Sent_MemMap.h"
extern void [!"SentSlowCRCErrorNotification"!](Flexio_Sent_Ip_ErrorStatusType ErrorEvent);
#define SENT_STOP_SEC_CODE
#include "Sent_MemMap.h"
[!ENDCODE!]
                    [!VAR "AllFastCrcCallbackFunctionsArray" = "text:replaceAll($AllFastCrcCallbackFunctionsArray, $EachFastCRCCallbackFunctionsArray, ' ')"!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
[!ENDNOCODE!]
[!ENDINDENT!]
#ifdef __cplusplus
}
#endif

/** @} */

#endif /* FLEXIO_SENT_IP_CFG_H */
