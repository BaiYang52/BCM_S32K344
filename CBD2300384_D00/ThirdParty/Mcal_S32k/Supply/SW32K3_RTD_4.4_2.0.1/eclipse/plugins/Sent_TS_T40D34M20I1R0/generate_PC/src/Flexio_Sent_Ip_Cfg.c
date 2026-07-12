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
*   @file    Flexio_Sent_Ip_Cfg.c
*
*   @version 2.0.1
*
*   @brief   AUTOSAR Sent - module interface
*   @details Configuration Structures for PostBuild.
*
*   @addtogroup SENT_DRIVER
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
#include "Flexio_Sent_Ip_Cfg.h"
#include "Flexio_Sent_Ip_Types.h"

[!AUTOSPACING!][!//
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/*
* @file           Flexio_Sent_Ip_Cfg.c
*/
#define FLEXIO_SENT_IP_CFG_VENDOR_ID_C                        43
#define FLEXIO_SENT_IP_CFG_AR_RELEASE_MAJOR_VERSION_C         4
#define FLEXIO_SENT_IP_CFG_AR_RELEASE_MINOR_VERSION_C         4
#define FLEXIO_SENT_IP_CFG_AR_RELEASE_REVISION_VERSION_C      0
#define FLEXIO_SENT_IP_CFG_SW_MAJOR_VERSION_C                 2
#define FLEXIO_SENT_IP_CFG_SW_MINOR_VERSION_C                 0
#define FLEXIO_SENT_IP_CFG_SW_PATCH_VERSION_C                 1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and SENT configuration header file are of the same vendor */
#if (FLEXIO_SENT_IP_CFG_VENDOR_ID_C != FLEXIO_SENT_IP_CFG_VENDOR_ID)
#error "Flexio_Sent_Ip_Cfg.c and Flexio_Sent_Ip_Cfg.h have different vendor ids"
#endif
/* Check if current file and SENT header file are of the same Autosar version */
#if ((FLEXIO_SENT_IP_CFG_AR_RELEASE_MAJOR_VERSION_C    != FLEXIO_SENT_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (FLEXIO_SENT_IP_CFG_AR_RELEASE_MINOR_VERSION_C    != FLEXIO_SENT_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (FLEXIO_SENT_IP_CFG_AR_RELEASE_REVISION_VERSION_C != FLEXIO_SENT_IP_CFG_AR_RELEASE_REVISION_VERSION) \
    )
  #error "AutoSar Version Numbers of Flexio_Sent_Ip_Cfg.c and Flexio_Sent_Ip_Cfg.h are different"
#endif
/* Check if current file and SENT header file are of the same Software version */
#if ((FLEXIO_SENT_IP_CFG_SW_MAJOR_VERSION_C != FLEXIO_SENT_IP_CFG_SW_MAJOR_VERSION) || \
     (FLEXIO_SENT_IP_CFG_SW_MINOR_VERSION_C != FLEXIO_SENT_IP_CFG_SW_MINOR_VERSION) || \
     (FLEXIO_SENT_IP_CFG_SW_PATCH_VERSION_C != FLEXIO_SENT_IP_CFG_SW_PATCH_VERSION) \
    )
       #error "Software Version Numbers of Flexio_Sent_Ip_Cfg.c and Flexio_Sent_Ip_Cfg.h are different"
#endif

/* Check if current file and SENT configuration header file are of the same vendor */
#if (FLEXIO_SENT_IP_CFG_VENDOR_ID_C != FLEXIO_SENT_IP_TYPES_VENDOR_ID)
#error "Flexio_Sent_Ip_Cfg.c and Flexio_Sent_Ip_Types.h have different vendor ids"
#endif
/* Check if current file and SENT header file are of the same Autosar version */
#if ((FLEXIO_SENT_IP_CFG_AR_RELEASE_MAJOR_VERSION_C    != FLEXIO_SENT_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (FLEXIO_SENT_IP_CFG_AR_RELEASE_MINOR_VERSION_C    != FLEXIO_SENT_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (FLEXIO_SENT_IP_CFG_AR_RELEASE_REVISION_VERSION_C != FLEXIO_SENT_IP_TYPES_AR_RELEASE_REVISION_VERSION) \
    )
  #error "AutoSar Version Numbers of Flexio_Sent_Ip_Cfg.c and Flexio_Sent_Ip_Types.h are different"
#endif
/* Check if current file and SENT header file are of the same Software version */
#if ((FLEXIO_SENT_IP_CFG_SW_MAJOR_VERSION_C != FLEXIO_SENT_IP_TYPES_SW_MAJOR_VERSION) || \
     (FLEXIO_SENT_IP_CFG_SW_MINOR_VERSION_C != FLEXIO_SENT_IP_TYPES_SW_MINOR_VERSION) || \
     (FLEXIO_SENT_IP_CFG_SW_PATCH_VERSION_C != FLEXIO_SENT_IP_TYPES_SW_PATCH_VERSION) \
    )
       #error "Software Version Numbers of Flexio_Sent_Ip_Cfg.c and Flexio_Sent_Ip_Types.h are different"
#endif


/*==================================================================================================
*                                   GLOBAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
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
uint32 Flexio_Sent_Ip_DmaTimerValue_[!"../../SentControllerId"!]_[!"num:i(SentChannelId)"!][[!"num:i($DmaBufferLength)"!]U];
[!ENDLOOP!]
#endif

#define SENT_STOP_SEC_VAR_CLEARED_32_NO_CACHEABLE
#include "Sent_MemMap.h"
[!ENDIF!]  
[!ENDSELECT!]
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
#endif

/** @} */
