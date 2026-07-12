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
*   @file    Flexio_Sent_Ip_Defines.h
*
*   @version 2.0.1
*
*   @brief   AUTOSAR Sent - module interface
*
*   @addtogroup SENT_DRIVER
*   @{
*/


#ifndef FLEXIO_SENT_IP_DEFINES_H
#define FLEXIO_SENT_IP_DEFINES_H

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

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/*
* @file           Flexio_Sent_Ip_Defines.h
*/
#define FLEXIO_SENT_IP_DEFINES_VENDOR_ID                       43
#define FLEXIO_SENT_IP_DEFINES_AR_RELEASE_MAJOR_VERSION        4
#define FLEXIO_SENT_IP_DEFINES_AR_RELEASE_MINOR_VERSION        4
#define FLEXIO_SENT_IP_DEFINES_AR_RELEASE_REVISION_VERSION     0
#define FLEXIO_SENT_IP_DEFINES_SW_MAJOR_VERSION                2
#define FLEXIO_SENT_IP_DEFINES_SW_MINOR_VERSION                0
#define FLEXIO_SENT_IP_DEFINES_SW_PATCH_VERSION                1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and Mcal.h header file are of the same Autosar version */
    #if ((FLEXIO_SENT_IP_DEFINES_AR_RELEASE_MAJOR_VERSION  != MCAL_AR_RELEASE_MAJOR_VERSION) || \
           (FLEXIO_SENT_IP_DEFINES_AR_RELEASE_MINOR_VERSION  != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Flexio_Sent_Ip_Defines.h and Mcal.h are different"
    #endif
#endif
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/
/*
* @brief          Support for User Mode feature.
* @details        This parameter is enabled only in order to support the write access to some registers are protected in user mode.
*/
#define FEATURE_FLEXIO_ENABLE_USER_MODE_SUPPORT[!WS "4"!][!WS "1"!][!IF "SentGeneral/SentEnableUserModeSupport = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!][!CR!]

#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    #if (STD_ON == FEATURE_FLEXIO_ENABLE_USER_MODE_SUPPORT)
        #error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running Flexio in user mode the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined
    #endif /* (STD_ON == FEATURE_FLEXIO_ENABLE_USER_MODE_SUPPORT) */
#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT */

/*
* @brief          Switches the Development Error Detection and Notification ON or OFF.
* @details        Shall activate or deactivate the detection of all development errors.
*
*/
#define FEATURE_FLEXIO_DEV_ERROR_DETECT[!WS "4"!][!WS "1"!][!IF "SentGeneral/SentDevErrorDetect = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!][!CR!]

/*
* @brief          Switches the DMA Feature ON or OFF.
* @details        Shall activate or deactivate the DMA feature.
*
*/
#define FEATURE_FLEXIO_DMA_ENABLE[!WS "4"!][!WS "1"!][!IF "SentGeneral/SentDmaActivation = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!][!CR!]
/**
* @brief      Specifies the timeout for  this module instance.
*
*/
#define SENT_TIMEOUT[!WS "4"!](uint32)[!"SentGeneral/SentTimeout"!]U

/**
* @brief      TIMEOUT for sync transmissions
*/
#define SENT_TIMEOUT_TYPE[!WS "4"!]([!"SentGeneral/SentTimeoutMethod"!])

/**
* @brief      Expanding range mode for tick length
*/
#define FLEXIO_SENT_IP_EXPANDING_RANGE_MODE[!WS "4"!][!IF "SentGeneral/TickLengthExpandRange = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!][!CR!]

/*
* @brief         Frequency from Mcu
* @details       Unit: Mhz, This is clock provides for SENT_FLEXIO
*
*/
#define FLEXIO_TIMER_FREQ_MHZ[!WS "4"!][!"num:i(num:i(node:ref(SentGeneral/SentCpuClockRef)/McuClockReferencePointFrequency) div 1000000)"!]U /* Mhz */

/*
* @brief         Number of configured controllers
* @details       Number of configured controllers as defined by user in configuration tool
*
*/
#define FEATURE_FLEXIO_INSTANCE_COUNT[!WS "4"!][!"num:i(count(SentConfigSet/SentControllerConfig/*))"!]U

/*
* @brief         Max number of configured channels
* @details       Max number of configured channels as defined by user in configuration tool
*
*/
#define FEATURE_FLEXIO_CHANNEL_COUNT[!WS "4"!][!"num:i(count(SentConfigSet/SentControllerConfig/*/SentChannelConfig/*))"!]U


#define FEATURE_FLEXIO_MAX_TIMER_COUNT[!WS "4"!][!"ecu:get('Sent.SentConfigSet.SentControllerConfig.SentChannelConfig')"!]U

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* FLEXIO_SENT_IP_DEFINES_H */
