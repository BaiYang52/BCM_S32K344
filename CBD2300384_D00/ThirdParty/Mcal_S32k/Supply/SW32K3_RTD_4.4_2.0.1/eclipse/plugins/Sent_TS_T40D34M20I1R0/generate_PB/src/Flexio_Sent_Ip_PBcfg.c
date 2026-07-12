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
*   @file    Flexio_Sent_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c
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

#ifdef FEATURE_FLEXIO_DMA_ENABLE
#if(STD_ON == FEATURE_FLEXIO_DMA_ENABLE)
    #include "Dma_Ip_Cfg.h"
#endif
#endif

[!AUTOSPACING!][!//
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/*
* @file           Flexio_Sent_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c
*/
#define FLEXIO_SENT_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C                        43
#define FLEXIO_SENT_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C         4
#define FLEXIO_SENT_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C         4
#define FLEXIO_SENT_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C      0
#define FLEXIO_SENT_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C                 2
#define FLEXIO_SENT_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C                 0
#define FLEXIO_SENT_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C                 1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/* Check if current file and SENT configuration header file are of the same vendor */
#if (FLEXIO_SENT_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != FLEXIO_SENT_IP_CFG_VENDOR_ID)
#error "Flexio_Sent_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_Sent_Ip_Cfg.h have different vendor ids"
#endif
/* Check if current file and SENT header file are of the same Autosar version */
#if ((FLEXIO_SENT_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != FLEXIO_SENT_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (FLEXIO_SENT_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != FLEXIO_SENT_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (FLEXIO_SENT_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != FLEXIO_SENT_IP_CFG_AR_RELEASE_REVISION_VERSION) \
    )
  #error "AutoSar Version Numbers of Flexio_Sent_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_Sent_Ip_Cfg.h are different"
#endif
/* Check if current file and SENT header file are of the same Software version */
#if ((FLEXIO_SENT_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != FLEXIO_SENT_IP_CFG_SW_MAJOR_VERSION) || \
     (FLEXIO_SENT_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != FLEXIO_SENT_IP_CFG_SW_MINOR_VERSION) || \
     (FLEXIO_SENT_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != FLEXIO_SENT_IP_CFG_SW_PATCH_VERSION) \
    )
       #error "Software Version Numbers of Flexio_Sent_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_Sent_Ip_Cfg.h are different"
#endif
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Dma_Ip_Cfg.h */
    #ifdef FEATURE_FLEXIO_DMA_ENABLE
        #if(STD_ON == FEATURE_FLEXIO_DMA_ENABLE)

            #if ((FLEXIO_SENT_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C != DMA_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
                 (FLEXIO_SENT_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C != DMA_IP_CFG_AR_RELEASE_MINOR_VERSION) \
                )
                #error "AutoSar Version Numbers of Flexio_Sent_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Dma_Ip_Cfg.h are different"
            #endif
        #endif
    #endif
#endif

/*==================================================================================================
*                                   STATE STRUCTURE DECLARATIONS
==================================================================================================*/
#define SENT_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Sent_MemMap.h"

extern Flexio_Sent_Ip_StateType Flexio_Sent_Ip_axStateStructure[FEATURE_FLEXIO_INSTANCE_COUNT];

#define SENT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Sent_MemMap.h"

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

[!SELECT "SentConfigSet"!]
    [!NOCODE!]
        [!VAR "SENT_CONTROLLER_CONFIG_CNT" = "num:i(0)"!]
        [!VAR "SENT_CHANNEL_CONFIG_CNT" = "num:i(0)"!]
        [!VAR "SENT_CONTROLLER_CONFIGURED" = "num:i(count(SentControllerConfig/*))"!]
        [!VAR "SENT_CHANNEL_CONFIG_CONFIGURED" = "num:i(count(SentControllerConfig/*/SentChannelConfig/*))"!]
    [!ENDNOCODE!]
#define SENT_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Sent_MemMap.h"

/**
* @brief          Description of individual SENT SRX controllers and Top level structure containing all Sent driver configurations
* @details        Description of individual SENT SRX controllers and Top level structure containing all Sent driver configurations
*
*/
static const Flexio_Sent_Ip_UserConfigType Flexio_aChnlUserConfig[[!"$SENT_CHANNEL_CONFIG_CONFIGURED"!]] =
{
    [!INDENT "4"!]
        [!LOOP "node:order(SentControllerConfig/*/SentChannelConfig/*, 'node:value(SentChannelId)')"!]
            [!VAR "SentClockRef" = "num:i(num:i(node:ref(../../../../../SentGeneral/SentCpuClockRef)/McuClockReferencePointFrequency) div 1000000)"!]
            
            [!IF "../../../../../SentGeneral/TickLengthExpandRange = 'false'"!]
            [!VAR "SentTickLengthMax" = "num:i((65535 div (num:i($SentClockRef)*56)))"!]
                [!IF "num:i(SentTickLength) >= num:i($SentTickLengthMax)"!]
                    [!ERROR!]
                        SentTickLength should be less than [!"$SentTickLengthMax"!]. The maximum timer value (flexio timer compare register) is 65535 (16 bit register), so the maximum of sent's nibble pulse period (56 tick time of Calibration/Synchronization Pulse) need to less than maximum timer value, on order to allocate to flexio timer compare register.
                    [!ENDERROR!]
                [!ENDIF!]
            [!ELSE!]
            [!VAR "SentTickLengthMax" = "num:i(((65535 * 1000) div (num:i($SentClockRef)*56)))"!]
                [!IF "num:i(SentTickLengthExpand) >= num:i($SentTickLengthMax)"!]
                    [!ERROR!]
                        SentTickLength should be less than [!"$SentTickLengthMax"!]. The maximum timer value (flexio timer compare register) is 65535 (16 bit register), so the maximum of sent's nibble pulse period (56 tick time of Calibration/Synchronization Pulse) need to less than maximum timer value, on order to allocate to flexio timer compare register.
                    [!ENDERROR!]
                [!ENDIF!]
            [!ENDIF!]
            [!VAR "SENT_CHANNEL_CONFIG_CNT" = "$SENT_CHANNEL_CONFIG_CNT + 1"!]
            {
                [!INDENT "8"!]
                   /*Controller ID of channel*/
                    (uint8)[!"../../SentControllerId"!],
                    /*Controller HW offset of channel*/
                    (uint8)[!"num:i(text:split((node:ref(SentFlexioChannelRef)/../../FlexioMclInstances),'_')[2])"!]U,
                    /* Channel ID */
                    (uint8)[!"SentChannelId"!],
                    /* Channel HW offset */
                    (uint8)[!"num:i(text:split((node:ref(SentFlexioChannelRef)/FlexioMclChannelId),'_')[2])"!]U,
                    /* Flexio pin to use as SENT pin */
                    (uint8)[!"num:inttohex((text:split((node:ref(SentFlexioChannelRef)/FlexioMclPinId),'_')[2]))"!]U,
                    /* Enable/Disable Pause Pulse */
                    [!"SentSyncAsyncSelection"!],
                    /* Number of nibbles */
                    (uint8)[!"DataLength"!]U,
                    /* Tick Time prescaler */
                    [!IF "../../../../../SentGeneral/TickLengthExpandRange = 'false'"!]
                    (uint32)[!"SentTickLength"!],
                    [!ELSE!]
                    (uint32)[!"SentTickLengthExpand"!],
                    [!ENDIF!]
            [!INDENT "4"!]
                #if(STD_ON == FEATURE_FLEXIO_DMA_ENABLE)
            [!ENDINDENT!]
                    /* DMA channel */
                    [!IF "../../../../../SentGeneral/SentDmaActivation = 'true'"!]
                    [!IF "node:exists(SentDmaChannelRef)"!]
                        (uint8)[!"normalize-space(node:ref(SentDmaChannelRef)/dmaLogicChannel_LogicName)"!],
                    [!ELSE!]
                        (uint8)0U,
                    [!ENDIF!]
                        /* Dma buffer depth */
                        (uint8)[!"DmaBufferDepth"!]U,
                        /* Dma buffer */
                    Flexio_Sent_Ip_DmaTimerValue_[!"../../SentControllerId"!]_[!"num:i(SentChannelId)"!],
                    [!ELSE!]
                        (uint8)0U,
                        /* Dma buffer depth */
                        (uint8)1U,
                        /* Dma buffer */
                        NULL_PTR,
                    [!ENDIF!]
            [!INDENT "4"!]
                #endif
            [!ENDINDENT!]
                    /* Sent Fast Channel CRC Type */
                    [!"SentFastChannelCRCType"!],
                    /* Sent Slow Channel CRC Type */
                    [!"SentSlowChannelCRCType"!],
                    /* CRC Fast notification */
                    [!IF "(SentFastCRCErrorNotificationEnable = 'true') and (SentFastCRCErrorNotification != 'NULL_PTR')"!]
                        &[!"normalize-space(SentFastCRCErrorNotification)"!],
                    [!ELSE!]
                        NULL_PTR,
                    [!ENDIF!]
                    /* CRC Slow notification */
                    [!IF "(SentSlowCRCErrorNotificationEnable = 'true') and (SentSlowCRCErrorNotification != 'NULL_PTR')"!]
                        &[!"normalize-space(SentSlowCRCErrorNotification)"!],
                    [!ELSE!]
                        NULL_PTR,
                    [!ENDIF!]
                    /* Call back function for Fast Notification. */
                    [!IF "(SentFastNotificationEnable = 'true') and (SentFastNotification != 'NULL_PTR')"!]
                        &[!"normalize-space(SentFastNotification)"!],
                    [!ELSE!]
                        NULL_PTR,
                    [!ENDIF!]
                    /* Call back function for Slow Notification. */
                    [!IF "(SentSlowNotificationEnable = 'true') and (SentSlowNotification != 'NULL_PTR')"!]
                        &[!"normalize-space(SentSlowNotification)"!]
                    [!ELSE!]
                        NULL_PTR
                    [!ENDIF!]
                [!ENDINDENT!]
            }[!IF "$SENT_CHANNEL_CONFIG_CNT < $SENT_CHANNEL_CONFIG_CONFIGURED"!],[!ENDIF!][!CR!]
        [!ENDLOOP!]
    [!ENDINDENT!]

};

[!LOOP "node:order(SentControllerConfig/*, 'node:value(SentControllerId)')"!]
[!VAR "ChannelCount" = "num:i(count(SentChannelConfig/*))"!]
/**
*   @brief  Sent Channel Object's pointer structure of Controller [!"SentControllerId"!]
*/
static Flexio_Sent_Ip_UserConfigType const * const Flexio_apChnlConfigCtrl_[!"SentControllerId"!][[!"$ChannelCount"!]]=
{
    [!INDENT "4"!]
    [!VAR "CtrIdx" = "0"!]
    [!LOOP "node:order(SentChannelConfig/*, './SentChannelId')"!]
        [!VAR "CtrIdx" = "num:i($CtrIdx + 1)"!]
                &Flexio_aChnlUserConfig[[!"num:i(./SentChannelId)"!]U][!IF "$CtrIdx < $ChannelCount"!],[!ENDIF!][!CR!]
    [!ENDLOOP!]
    [!ENDINDENT!]
};
[!ENDLOOP!]
#define SENT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Sent_MemMap.h"
/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/
#define SENT_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Sent_MemMap.h"

const Flexio_CtrlConfigType Flexio_aCtrlConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"$SENT_CONTROLLER_CONFIGURED"!]U] =
{
    [!INDENT "4"!]
        [!LOOP "node:order(SentControllerConfig/*, 'node:value(SentControllerId)')"!]
            [!VAR "ChannelCount" = "num:i(count(SentChannelConfig/*))"!]
            [!VAR "SENT_CONTROLLER_CONFIG_CNT" = "$SENT_CONTROLLER_CONFIG_CNT + 1"!]
            {
                [!INDENT "8"!]
                    /* Controller HWoffset of [!"node:ref(SentHwControllerRef)/FlexioMclInstances"!]*/
                    [!IF "SentControllerActivation = 'true'"!]
                        (uint8)[!"num:i(text:split((node:ref(SentHwControllerRef)/FlexioMclInstances),'_')[2])"!]U,
                    [!ELSE!]
                        (uint8)SENT_FLEXIO_NULL_OFFSET_U8,
                    [!ENDIF!]
                    /* Controller ID of [!"node:ref(SentHwControllerRef)/FlexioMclInstances"!] */
                    (uint8)[!"num:i(SentControllerId)"!]U,
                    /* Number of configured channels of Sent Controller Unit [!"node:ref(SentHwControllerRef)/FlexioMclInstances"!]*/
                    (uint8)[!"num:i($ChannelCount)"!]U,
                    /* Controller processing type */
                    [!IF "SentProcessing = 'INTERRUPT'"!]
                        FLEXIO_DRIVER_TYPE_INTERRUPTS,
                    [!ENDIF!]
                    [!IF "SentProcessing = 'DMA'"!]
                        FLEXIO_DRIVER_TYPE_DMA,
                    [!ENDIF!]
                    [!IF "SentProcessing = 'POLLING'"!]
                        FLEXIO_DRIVER_TYPE_POLLING,
                    [!ENDIF!]
                    /* Array of pointer point to Channel config of this Controller*/
                    Flexio_apChnlConfigCtrl_[!"SentControllerId"!],
                    /* Array of pointer point to controller state of this Controller*/
                    &Flexio_Sent_Ip_axStateStructure[[!"SentControllerId"!]]
                [!ENDINDENT!]
            }[!IF "$SENT_CONTROLLER_CONFIG_CNT < $SENT_CONTROLLER_CONFIGURED"!],[!ENDIF!][!CR!]
        [!ENDLOOP!]
    [!ENDINDENT!]
[!ENDSELECT!]
};

#define SENT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Sent_MemMap.h"
/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

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
