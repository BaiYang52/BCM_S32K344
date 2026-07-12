/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : eMios
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
=====================================================================================================*/

/**
*   @file           Ocu_Ipw.c
*
*   @internal
*   @addtogroup    ocu_ipw Ocu IPW
*   @brief         Ocu IPW API implementation.
*   @details       OCU IWP API implementation.
*
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*===============================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
===============================================================================================*/

#include "Ocu_Ipw.h"
/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define OCU_IPW_VENDOR_ID_C                       43
#define OCU_IPW_AR_RELEASE_MAJOR_VERSION_C        4
#define OCU_IPW_AR_RELEASE_MINOR_VERSION_C        4
#define OCU_IPW_AR_RELEASE_REVISION_VERSION_C     0
#define OCU_IPW_SW_MAJOR_VERSION_C                2
#define OCU_IPW_SW_MINOR_VERSION_C                0
#define OCU_IPW_SW_PATCH_VERSION_C                1
/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/
#if (OCU_IPW_VENDOR_ID_C != OCU_IPW_VENDOR_ID)
    #error "Ocu_Ipw.c and Ocu_Ipw.h have different vendor ids"
#endif

/* Check if  source file and Ocu_Ipw.h file are of the same Autosar version */
#if ((OCU_IPW_AR_RELEASE_MAJOR_VERSION_C != OCU_IPW_AR_RELEASE_MAJOR_VERSION) || \
     (OCU_IPW_AR_RELEASE_MINOR_VERSION_C != OCU_IPW_AR_RELEASE_MINOR_VERSION) || \
     (OCU_IPW_AR_RELEASE_REVISION_VERSION_C != OCU_IPW_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Ocu_Ipw.c and Ocu_Ipw.h are different"
#endif

/* Check if source file and Ocu_Ipw.h file are of the same Software version */
#if ((OCU_IPW_SW_MAJOR_VERSION_C != OCU_IPW_SW_MAJOR_VERSION) || \
     (OCU_IPW_SW_MINOR_VERSION_C != OCU_IPW_SW_MINOR_VERSION) || \
     (OCU_IPW_SW_PATCH_VERSION_C != OCU_IPW_SW_PATCH_VERSION))
    #error "Software Version Numbers of Ocu_Ipw.c and Ocu_Ipw.h are different"
#endif

/*===============================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
===============================================================================================*/

/*===============================================================================================
*                                       CONSTANTS
===============================================================================================*/

#define OCU_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Ocu_MemMap.h"

/**
* @brief    Array of pointers to the IP specific configuration structure - the pointer is valid only when the driver is in initialized state for a given partition
*/
static const Ocu_Ipw_IpConfigType * Ocu_Ipw_pGlobalConfig[OCU_MAX_PARTITIONS];

#define OCU_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Ocu_MemMap.h"

/*===============================================================================================
*                                       LOCAL VARIABLES
===============================================================================================*/

/*===============================================================================================
*                                       GLOBAL CONSTANTS
===============================================================================================*/

/*===============================================================================================
*                                       GLOBAL VARIABLES
===============================================================================================*/

/*===============================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
===============================================================================================*/

/*===============================================================================================
*                                       LOCAL FUNCTIONS
===============================================================================================*/

/*===============================================================================================
*                                       GLOBAL FUNCTIONS
===============================================================================================*/

#define OCU_START_SEC_CODE
#include "Ocu_MemMap.h"

/*================================================================================================*/
/**
 * @brief   This function call the supported IP initialization functions.
 *
 */
void Ocu_Ipw_Init(const Ocu_Ipw_IpConfigType * const pIpConfig)
{
    uint8 CoreId = (uint8)Ocu_GetCoreId();
    if ((NULL_PTR != pIpConfig) && (NULL_PTR == Ocu_Ipw_pGlobalConfig[CoreId]))
    {
        if (NULL_PTR != pIpConfig->pEmiosIpConfig)
        {
            /* Call eMIOS IP init function */
            Ocu_Ipw_pGlobalConfig[CoreId] = pIpConfig;
            Emios_Ocu_Ip_Init(Ocu_Ipw_pGlobalConfig[CoreId]->pEmiosIpConfig);
        }
    }
}

/*===============================================================================================*/
#if (STD_ON == OCU_DEINIT_API)
/**
 * @brief   This function call the supported IP de-initialization functions.
 *
 */
void Ocu_Ipw_DeInit(const Ocu_Ipw_IpConfigType * const pIpConfig)
{
    uint8 CoreId = (uint8)Ocu_GetCoreId();
    if ((NULL_PTR != pIpConfig) && (pIpConfig == Ocu_Ipw_pGlobalConfig[CoreId]))
    {
        if (NULL_PTR != pIpConfig->pEmiosIpConfig)
        {
            /* Call eMIOS IP DeInit function */
            Emios_Ocu_Ip_DeInit();
            Ocu_Ipw_pGlobalConfig[CoreId] = NULL_PTR;
        }
    }
}
#endif /* OCU_DEINIT_API */


/*===============================================================================================*/
/**
 * @brief   Call StartChannel() function for the eMIOS Ocu channel
 *
 */
void Ocu_Ipw_StartChannel(uint16 NumChannel)
{
    const Emios_Ocu_Ip_ModuleConfigType * pEmiosIpChConfig;
    const Emios_Ocu_Ip_ChannelConfigType * pEmiosChannelCfg;

    /* eMIOS channel and instance numbers */
    uint8 InstNum;
    uint8 ChNum;

    uint8 CoreId = (uint8)Ocu_GetCoreId();

    /* Channel Index in the eMIOS specific configuration table */
    uint16 ChannelIdx = (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mIdxChannelConfig;

    if (OCU_EMIOS_CHANNEL == (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mChannelIpType)
    {
        pEmiosIpChConfig = Ocu_Ipw_pGlobalConfig[CoreId]->pEmiosIpConfig;
        pEmiosChannelCfg = &(*pEmiosIpChConfig->pcxChannelsConfig)[ChannelIdx];

        /* Get eMIOS channel and instance numbers from Hw Channel Id */
        InstNum = Emios_Ocu_Ip_GetInstanceNum(pEmiosChannelCfg->HwChannel);
        ChNum   = Emios_Ocu_Ip_GetChannelNum(pEmiosChannelCfg->HwChannel);
        Emios_Ocu_Ip_StartChannel(InstNum, ChNum);
    }
}


/*===============================================================================================*/
/**
 * @brief   Call StopChannel() function for the eMIOS Ocu channel
 *
 */
void Ocu_Ipw_StopChannel(uint16 NumChannel)
{
    const Emios_Ocu_Ip_ModuleConfigType * pEmiosIpChConfig;
    const Emios_Ocu_Ip_ChannelConfigType * pEmiosChannelCfg;

    /* eMIOS channel and instance numbers */
    uint8 InstNum;
    uint8 ChNum;

    uint8 CoreId = (uint8)Ocu_GetCoreId();

    /* Channel Index in the eMIOS specific configuration table */
    uint16 ChannelIdx = (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mIdxChannelConfig;

    if (OCU_EMIOS_CHANNEL == (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mChannelIpType)
    {
        pEmiosIpChConfig = Ocu_Ipw_pGlobalConfig[CoreId]->pEmiosIpConfig;
        pEmiosChannelCfg = &(*pEmiosIpChConfig->pcxChannelsConfig)[ChannelIdx];

        /* Get eMIOS channel and instance numbers from Hw Channel Id */
        InstNum = Emios_Ocu_Ip_GetInstanceNum(pEmiosChannelCfg->HwChannel);
        ChNum   = Emios_Ocu_Ip_GetChannelNum(pEmiosChannelCfg->HwChannel);
        Emios_Ocu_Ip_StopChannel(InstNum, ChNum);
    }
}

/*===============================================================================================*/
#if (STD_ON == OCU_SET_PIN_STATE_API)
/**
 * @brief   Call SetPinState() function for the eMIOS Ocu channel
 *
 */
void Ocu_Ipw_SetPinState(uint16 NumChannel, OCU_PIN_STATE_TYPE PinState)
{
    const Emios_Ocu_Ip_ModuleConfigType * pEmiosIpChConfig;
    const Emios_Ocu_Ip_ChannelConfigType * pEmiosChannelCfg;

    /* eMIOS channel and instance numbers */
    uint8 InstNum;
    uint8 ChNum;

    uint8 CoreId = (uint8)Ocu_GetCoreId();

    /* Channel Index in the eMIOS specific configuration table */
    uint16 ChannelIdx = (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mIdxChannelConfig;

    if (OCU_EMIOS_CHANNEL == (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mChannelIpType)
    {
        pEmiosIpChConfig = Ocu_Ipw_pGlobalConfig[CoreId]->pEmiosIpConfig;
        pEmiosChannelCfg = &(*pEmiosIpChConfig->pcxChannelsConfig)[ChannelIdx];

        /* Get eMIOS channel and instance numbers from Hw Channel Id */
        InstNum = Emios_Ocu_Ip_GetInstanceNum(pEmiosChannelCfg->HwChannel);
        ChNum   = Emios_Ocu_Ip_GetChannelNum(pEmiosChannelCfg->HwChannel);
        Emios_Ocu_Ip_SetPinState(InstNum, ChNum, PinState);
    }
}
#endif

/*===============================================================================================*/
#if (STD_ON == OCU_SET_PIN_ACTION_API)
/**
 * @brief   Call SetPinAction() function for the eMIOS Ocu channel
 *
 */
void Ocu_Ipw_SetPinAction(uint16 NumChannel, OCU_PIN_ACTION_TYPE PinAction)
{
    const Emios_Ocu_Ip_ModuleConfigType * pEmiosIpChConfig;
    const Emios_Ocu_Ip_ChannelConfigType * pEmiosChannelCfg;

    /* eMIOS channel and instance numbers */
    uint8 InstNum;
    uint8 ChNum;

    uint8 CoreId = (uint8)Ocu_GetCoreId();

    /* Channel Index in the eMIOS specific configuration table */
    uint16 ChannelIdx = (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mIdxChannelConfig;

    if (OCU_EMIOS_CHANNEL == (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mChannelIpType)
    {
        pEmiosIpChConfig = Ocu_Ipw_pGlobalConfig[CoreId]->pEmiosIpConfig;
        pEmiosChannelCfg = &(*pEmiosIpChConfig->pcxChannelsConfig)[ChannelIdx];

        /* Get eMIOS channel and instance numbers from Hw Channel Id */
        InstNum = Emios_Ocu_Ip_GetInstanceNum(pEmiosChannelCfg->HwChannel);
        ChNum   = Emios_Ocu_Ip_GetChannelNum(pEmiosChannelCfg->HwChannel);
        Emios_Ocu_Ip_SetPinAction(InstNum, ChNum, PinAction);
    }
}
#endif

#if (STD_ON == OCU_GET_COUNTER_API)
/*===============================================================================================*/
/**
 * @brief   Call GetCounter() function for the eMIOS Ocu channel
 *
 */
uint16 Ocu_Ipw_GetCounter(uint16 NumChannel)
{
    uint16 RetCntVal;

    const Emios_Ocu_Ip_ModuleConfigType * pEmiosIpChConfig;
    const Emios_Ocu_Ip_ChannelConfigType * pEmiosChannelCfg;

    /* eMIOS channel and instance numbers */
    uint8 InstNum;
    uint8 ChNum;

    uint8 CoreId = (uint8)Ocu_GetCoreId();

    /* Channel Index in the eMIOS specific configuration table */
    uint16 ChannelIdx = (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mIdxChannelConfig;

    if (OCU_EMIOS_CHANNEL == (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mChannelIpType)
    {
        pEmiosIpChConfig = Ocu_Ipw_pGlobalConfig[CoreId]->pEmiosIpConfig;
        pEmiosChannelCfg = &(*pEmiosIpChConfig->pcxChannelsConfig)[ChannelIdx];

        /* Get eMIOS channel and instance numbers from Hw Channel Id */
        InstNum = Emios_Ocu_Ip_GetInstanceNum(pEmiosChannelCfg->HwChannel);
        ChNum   = Emios_Ocu_Ip_GetChannelNum(pEmiosChannelCfg->HwChannel);
        RetCntVal = (uint16)Emios_Ocu_Ip_GetCounter(InstNum, ChNum);
    }
    else
    {
        /*do nothing*/
        RetCntVal = 0U;
    }
    return RetCntVal;
}

#endif /* OCU_GET_COUNTER_API */

#if (STD_ON == OCU_SET_ABSOLUTE_THRESHOLD_API)
/*===============================================================================================*/
/**
 * @brief   This function will change the compare match interval for the given timer channel
 *
 */
OCU_RETURN_TYPE Ocu_Ipw_SetAbsoluteThreshold(uint16 NumChannel, uint16 ReferenceValue, uint16 AbsoluteValue)
{
    OCU_RETURN_TYPE RetVal;

    const Emios_Ocu_Ip_ModuleConfigType * pEmiosIpChConfig;
    const Emios_Ocu_Ip_ChannelConfigType * pEmiosChannelCfg;

    /* eMIOS channel and instance numbers */
    uint8 InstNum;
    uint8 ChNum;

    uint8 CoreId = (uint8)Ocu_GetCoreId();

    /* Channel Index in the eMIOS specific configuration table */
    uint16 ChannelIdx = (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mIdxChannelConfig;

    if (OCU_EMIOS_CHANNEL == (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mChannelIpType)
    {
        pEmiosIpChConfig = Ocu_Ipw_pGlobalConfig[CoreId]->pEmiosIpConfig;
        pEmiosChannelCfg = &(*pEmiosIpChConfig->pcxChannelsConfig)[ChannelIdx];

        /* Get eMIOS channel and instance numbers from Hw Channel Id */
        InstNum = Emios_Ocu_Ip_GetInstanceNum(pEmiosChannelCfg->HwChannel);
        ChNum   = Emios_Ocu_Ip_GetChannelNum(pEmiosChannelCfg->HwChannel);
        RetVal  = Emios_Ocu_Ip_SetAbsoluteThreshold(InstNum, ChNum, (Emios_Ocu_Ip_ValueType)ReferenceValue, (Emios_Ocu_Ip_ValueType)AbsoluteValue);
    }
    else
    {
        /*do nothing*/
        RetVal = OCU_EMIOS_CM_IN_REF_INTERVAL;
    }
    return RetVal;
}
#endif /* OCU_SET_ABSOLUTE_THRESHOLD_API */

#if (STD_ON == OCU_SET_RELATIVE_THRESHOLD_API)
/*===============================================================================================*/
/**
 * @brief   This function will change the compare match interval for the given timer channel
 *
 */
OCU_RETURN_TYPE Ocu_Ipw_SetRelativeThreshold(uint16 NumChannel, uint16 RelativeValue)
{
    OCU_RETURN_TYPE RetVal;

    const Emios_Ocu_Ip_ModuleConfigType * pEmiosIpChConfig;
    const Emios_Ocu_Ip_ChannelConfigType * pEmiosChannelCfg;

    /* eMIOS channel and instance numbers */
    uint8 InstNum;
    uint8 ChNum;

    uint8 CoreId = (uint8)Ocu_GetCoreId();

    /* Channel Index in the eMIOS specific configuration table */
    uint16 ChannelIdx = (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mIdxChannelConfig;

    if (OCU_EMIOS_CHANNEL == (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mChannelIpType)
    {
        pEmiosIpChConfig = Ocu_Ipw_pGlobalConfig[CoreId]->pEmiosIpConfig;
        pEmiosChannelCfg = &(*pEmiosIpChConfig->pcxChannelsConfig)[ChannelIdx];

        /* Get eMIOS channel and instance numbers from Hw Channel Id */
        InstNum = Emios_Ocu_Ip_GetInstanceNum(pEmiosChannelCfg->HwChannel);
        ChNum   = Emios_Ocu_Ip_GetChannelNum(pEmiosChannelCfg->HwChannel);
        RetVal  = Emios_Ocu_Ip_SetRelativeThreshold(InstNum, ChNum, (Emios_Ocu_Ip_ValueType)RelativeValue);
    }
    else
    {
        /*do nothing*/
        RetVal = OCU_EMIOS_CM_IN_REF_INTERVAL;
    }
    return RetVal;
}
#endif /* OCU_SET_RELATIVE_THRESHOLD_API */

/*===============================================================================================*/
#if (STD_ON == OCU_SET_CLOCK_MODE_API)
/**
 * @brief   Call SetClockMode() function of all configured Hw IPs
 *
 */
void Ocu_Ipw_SetClockMode(OCU_PIN_SELECT_PRESCALER_TYPE Prescaler)
{
    /* Call eMIOS IP function */
    Emios_Ocu_Ip_SetClockMode(Prescaler);
}
#endif

/*===============================================================================================*/
#if ((STD_ON == OCU_SET_RELATIVE_THRESHOLD_API) || (STD_ON == OCU_SET_ABSOLUTE_THRESHOLD_API))
/**
 * @brief   This function will return max counter value for given channel
 *
 */
uint16 Ocu_Ipw_GetMaxCounterValue(uint16 NumChannel)
{
    uint16  RetMaxCntVal;

    const Emios_Ocu_Ip_ModuleConfigType * pEmiosIpChConfig;
    const Emios_Ocu_Ip_ChannelConfigType * pEmiosChannelCfg;

    /* eMIOS channel and instance numbers */
    uint8 InstNum;
    uint8 ChNum;

    uint8 CoreId = (uint8)Ocu_GetCoreId();

    /* Channel Index in the eMIOS specific configuration table */
    uint16 ChannelIdx = (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mIdxChannelConfig;

    if (OCU_EMIOS_CHANNEL == (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mChannelIpType)
    {
        pEmiosIpChConfig = Ocu_Ipw_pGlobalConfig[CoreId]->pEmiosIpConfig;
        pEmiosChannelCfg = &(*pEmiosIpChConfig->pcxChannelsConfig)[ChannelIdx];

        /* Get eMIOS channel and instance numbers from Hw Channel Id */
        InstNum = Emios_Ocu_Ip_GetInstanceNum(pEmiosChannelCfg->HwChannel);
        ChNum   = Emios_Ocu_Ip_GetChannelNum(pEmiosChannelCfg->HwChannel);
        RetMaxCntVal = Emios_Ocu_Ip_GetMaxCounterValue(InstNum, ChNum);
    }
    else
    {
        /*do nothing*/
        RetMaxCntVal = 0U;
    }
    return RetMaxCntVal;
}
#endif


#if ((STD_ON == OCU_NOTIFICATION_SUPPORTED) && (STD_ON == OCU_ENABLE_ISR_BY_EN_DIS_NOTIFICATION))
/**
 * @brief   This function is used to disable the OCU compare match notification.
 *
 */
void Ocu_Ipw_DisableNotification(uint16 NumChannel)
{
    const Emios_Ocu_Ip_ModuleConfigType * pEmiosIpChConfig;
    const Emios_Ocu_Ip_ChannelConfigType * pEmiosChannelCfg;

    /* eMIOS channel and instance numbers */
    uint8 InstNum;
    uint8 ChNum;

    uint8 CoreId = (uint8)Ocu_GetCoreId();

    /* Channel Index in the eMIOS specific configuration table */
    uint16 ChannelIdx = (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mIdxChannelConfig;

    if (OCU_EMIOS_CHANNEL == (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mChannelIpType)
    {
        pEmiosIpChConfig = Ocu_Ipw_pGlobalConfig[CoreId]->pEmiosIpConfig;
        pEmiosChannelCfg = &(*pEmiosIpChConfig->pcxChannelsConfig)[ChannelIdx];

        /* Get eMIOS channel and instance numbers from Hw Channel Id */
        InstNum = Emios_Ocu_Ip_GetInstanceNum(pEmiosChannelCfg->HwChannel);
        ChNum   = Emios_Ocu_Ip_GetChannelNum(pEmiosChannelCfg->HwChannel);
        Emios_Ocu_Ip_DisableNotification(InstNum, ChNum);
    }
}

/**
 * @brief   This function is used to enable the OCU compare match notification.
 *
 */
void Ocu_Ipw_EnableNotification(uint16 NumChannel)
{
    const Emios_Ocu_Ip_ModuleConfigType * pEmiosIpChConfig;
    const Emios_Ocu_Ip_ChannelConfigType * pEmiosChannelCfg;

    /* eMIOS channel and instance numbers */
    uint8 InstNum;
    uint8 ChNum;

    uint8 CoreId = (uint8)Ocu_GetCoreId();

    /* Channel Index in the eMIOS specific configuration table */
    uint16 ChannelIdx = (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mIdxChannelConfig;

    if (OCU_EMIOS_CHANNEL == (*Ocu_Ipw_pGlobalConfig[CoreId]->pIpChannelsConfig)[NumChannel].mChannelIpType)
    {
        pEmiosIpChConfig = Ocu_Ipw_pGlobalConfig[CoreId]->pEmiosIpConfig;
        pEmiosChannelCfg = &(*pEmiosIpChConfig->pcxChannelsConfig)[ChannelIdx];

        /* Get eMIOS channel and instance numbers from Hw Channel Id */
        InstNum = Emios_Ocu_Ip_GetInstanceNum(pEmiosChannelCfg->HwChannel);
        ChNum   = Emios_Ocu_Ip_GetChannelNum(pEmiosChannelCfg->HwChannel);
        Emios_Ocu_Ip_EnableNotification(InstNum, ChNum);
    }
}
#endif

#define OCU_STOP_SEC_CODE
#include "Ocu_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
