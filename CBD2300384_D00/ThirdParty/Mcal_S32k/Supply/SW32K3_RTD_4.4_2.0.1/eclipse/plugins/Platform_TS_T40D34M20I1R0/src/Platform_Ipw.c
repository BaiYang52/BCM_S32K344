/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : 
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
*   @file
*
*   @internal
*   @addtogroup Platform_Ipw
*   @{
*/

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#include "Platform_TypesDef.h"
#include "Platform_Ipw.h"
#include "Mcal.h"

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
#define PLATFORM_START_SEC_CODE
#include "Platform_MemMap.h"

#if ((STD_ON == PLATFORM_ENABLE_USER_MODE_SUPPORT) && (defined (MCAL_ENABLE_USER_MODE_SUPPORT)))
    #define Call_System_Ip_ConfigIrq(eIrq,bEnable)          OsIf_Trusted_Call2params(System_Ip_ConfigIrq,(eIrq),(bEnable))
    #define Call_System_Ip_SetAhbSlavePriority(bPriority)   OsIf_Trusted_Call1param(System_Ip_SetAhbSlavePriority,(bPriority))
#else
    #define Call_System_Ip_ConfigIrq(eIrq,bEnable)          System_Ip_ConfigIrq((eIrq),(bEnable))
    #define Call_System_Ip_SetAhbSlavePriority(bPriority)   System_Ip_SetAhbSlavePriority((bPriority))
#endif



static Std_ReturnType Platform_Ipw_InitIntCtrl(const IntCtrl_Ip_CtrlConfigType *pIntCtrlCtrlConfig)
{
    uint32 irqIdx;
    
    for (irqIdx = 0; irqIdx < pIntCtrlCtrlConfig->u32ConfigIrqCount; irqIdx++)
    {
        /* clears the pending flag for the interrupt request. */
        IntCtrl_Ip_ClearPending(pIntCtrlCtrlConfig->aIrqConfig[irqIdx].eIrqNumber);
        /* interrupt number for which the priority is se */
        IntCtrl_Ip_SetPriority(pIntCtrlCtrlConfig->aIrqConfig[irqIdx].eIrqNumber,
                               pIntCtrlCtrlConfig->aIrqConfig[irqIdx].u8IrqPriority);
        /* Enable interrupt */
        if (pIntCtrlCtrlConfig->aIrqConfig[irqIdx].bIrqEnabled)
        {
            /* enables the interrupt request at interrupt controller level. */
            IntCtrl_Ip_EnableIrq(pIntCtrlCtrlConfig->aIrqConfig[irqIdx].eIrqNumber);
        }
        else
        {
            /* disables the interrupt request at interrupt controller level.*/
            IntCtrl_Ip_DisableIrq(pIntCtrlCtrlConfig->aIrqConfig[irqIdx].eIrqNumber);
        }
    }

    return E_OK;
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/**
 * @brief         Initializes the platform settings based on user configuration.
 */
void Platform_Ipw_Init(const Platform_Ipw_ConfigType *pConfig)
{
#if (PLATFORM_SYS_CFG == STD_ON)
    uint8 irqIdx;
#endif
    Std_ReturnType ret=(Std_ReturnType)E_OK;
#if (PLATFORM_INT_MONITOR == STD_ON)
    uint8 idx = 0u;
#endif

    /* Initialize interrupts at interrupt controller level */
    if (NULL_PTR != pConfig->pIntCtrlConfig)
    {
        ret = Platform_Ipw_InitIntCtrl(pConfig->pIntCtrlConfig);
    }

    /* Configure core-related interrupt requests */
    if (ret ==(Std_ReturnType)E_OK)
    {
#if (PLATFORM_SYS_CFG == STD_ON)
        if (pConfig->aSystemConfig != NULL_PTR)
        {
            for (irqIdx = 0u; irqIdx < (pConfig->aSystemConfig)->u8SysIrqsCount; irqIdx++)
            {
                Call_System_Ip_ConfigIrq((pConfig->aSystemConfig)->aSystemIrqs[irqIdx].eIrq, (pConfig->aSystemConfig)->aSystemIrqs[irqIdx].bEnable);
            }
            Call_System_Ip_SetAhbSlavePriority((pConfig->aSystemConfig)->bAhbSlavePriority);
        }
#endif
    }
#if (PLATFORM_INT_MONITOR == STD_ON)
    /* Configure interrupt monitors */
    for (idx = 0u; idx < (pConfig->pIntMonitor)->u8NumChannel; idx++)
    {
        Intm_Ip_SelectMonitoredIrq(pConfig->pIntMonitor->aIntmChannelConfig[idx].eMonitor, pConfig->pIntMonitor->aIntmChannelConfig[idx].eMonitoredIrq);
        Intm_Ip_SetLatency(pConfig->pIntMonitor->aIntmChannelConfig[idx].eMonitor, pConfig->pIntMonitor->aIntmChannelConfig[idx].u32AcceptedLatency);
    }
    if (pConfig->pIntMonitor->pIntmGenericConfig != NULL_PTR)
    {
        if (pConfig->pIntMonitor->pIntmGenericConfig->bEnableIntMonitor)
        {
            Intm_Ip_EnableMonitor();
        }
        else
        {
            Intm_Ip_DisableMonitor();
        }
    }
#endif
}

#if (PLATFORM_MSCM_CFG == STD_ON)
/**
 * @brief         Initializes the platform settings for non-core elements  based on user configuration.
 */

void Platform_Ipw_InitNonCore(const Platform_Ipw_NonCoreConfigType *pConfig)
{
    uint32 irqIdx;
    if (pConfig -> pIntRouteConfig != NULL_PTR)
    {
        for (irqIdx = 0; irqIdx < (pConfig -> pIntRouteConfig) -> u32ConfigIrqCount ; irqIdx++)
        {           
            IntCtrl_Ip_SetTargetCores(pConfig -> pIntRouteConfig->aIrqConfig[irqIdx].eIrqNumber,
                                    pConfig -> pIntRouteConfig->aIrqConfig[irqIdx].u8TargetCores);
            IntCtrl_Ip_InstallHandler(pConfig -> pIntRouteConfig->aIrqConfig[irqIdx].eIrqNumber,
                                  pConfig -> pIntRouteConfig->aIrqConfig[irqIdx].pfHandler,
                                  NULL_PTR);
        }
    }
}
#endif
#if (PLATFORM_INT_MONITOR == STD_ON)
/**
 * @internal
 * @brief         Enables/disables interrupt monitoring feature by calling the Interrupt Monitor IP layer.
 */
void Platform_Ipw_SetIrqMonitor(boolean bEnable)
{    
    if (bEnable == TRUE)
    {
        Intm_Ip_EnableMonitor();
    }
    else
    {
        Intm_Ip_DisableMonitor();
    }
}


/**
 * @internal
 * @brief         Selects an interrupt to be monitored by calling the Interrupt Monitor IP layer.
 */
void Platform_Ipw_SelectMonitoredIrq(const Platform_Ipw_ConfigType *pConfig,
                                                uint8 u8Channel,
                                                IRQn_Type eIrqNumber)
{
    uint8 u8ChannelIdxInPartition;
    Intm_Ip_MonitorType eHwChannel;
    
    u8ChannelIdxInPartition = (*((pConfig->pIntMonitor)->Platform_IndexChannelMap))[u8Channel];
    eHwChannel = pConfig->pIntMonitor->aIntmChannelConfig[u8ChannelIdxInPartition].eMonitor;
    Intm_Ip_SelectMonitoredIrq(eHwChannel, eIrqNumber);
}                                                

/**
 * @internal
 * @brief         Selects an accepted latency for a monitored interrupt
 *                by calling the Interrupt Monitor IP layer.
 */
void Platform_Ipw_SetMonitoredIrqLatency(const Platform_Ipw_ConfigType *pConfig,
                                                    uint8 u8Channel,
                                                    uint32 u32Latency)
{
    uint8 u8ChannelIdxInPartition;
    Intm_Ip_MonitorType eHwChannel;
    
    u8ChannelIdxInPartition = (*((pConfig->pIntMonitor)->Platform_IndexChannelMap))[u8Channel];
    eHwChannel = pConfig->pIntMonitor->aIntmChannelConfig[u8ChannelIdxInPartition].eMonitor;
    Intm_Ip_SetLatency(eHwChannel, u32Latency);
}


/**
 * @internal
 * @brief         Resets an interrupt monitor timer by calling the Interrupt Monitor IP layer.
 */
void Platform_Ipw_ResetIrqMonitorTimer(const Platform_Ipw_ConfigType *pConfig,
                                                    uint8 u8Channel)
{
    uint8 u8ChannelIdxInPartition;
    Intm_Ip_MonitorType eHwChannel;
    
    u8ChannelIdxInPartition = (*((pConfig->pIntMonitor)->Platform_IndexChannelMap))[u8Channel];
    eHwChannel = pConfig->pIntMonitor->aIntmChannelConfig[u8ChannelIdxInPartition].eMonitor;
    Intm_Ip_ResetTimer(eHwChannel);
}


/**
 * @internal
 * @brief         Returns the status of an interrupt monitor by calling the Interrupt Monitor IP layer.
 */
boolean Platform_Ipw_GetIrqMonitorStatus(const Platform_Ipw_ConfigType *pConfig,
                                                        uint8 u8Channel)
{
    boolean RetVal = TRUE;
    uint8 u8ChannelIdxInPartition;
    Intm_Ip_MonitorType eHwChannel;
    
    u8ChannelIdxInPartition = (*((pConfig->pIntMonitor)->Platform_IndexChannelMap))[u8Channel];
    eHwChannel = pConfig->pIntMonitor->aIntmChannelConfig[u8ChannelIdxInPartition].eMonitor;
    if (Intm_Ip_GetStatus(eHwChannel) == 0U)
    {
        RetVal = FALSE;
    }
    return RetVal;
}

/**
 * @internal
 * @brief         Returns the validation result for given logical channel.
 */
Std_ReturnType Platform_Ipw_ValidateChannelIntm(const Platform_Ipw_ConfigType *pConfig,
                                                    uint8 u8Channel)
{
    Std_ReturnType RetVal = E_NOT_OK;
    uint8 u8ChannelIdxInPartition;
    if(((pConfig->pIntMonitor)->Platform_IndexChannelMap) != NULL_PTR)
    {
        u8ChannelIdxInPartition = (*((pConfig->pIntMonitor)->Platform_IndexChannelMap))[u8Channel];
        if (u8ChannelIdxInPartition <= (pConfig->pIntMonitor->u8NumChannel))
        {
            RetVal = E_OK;
        }
    }   
    return RetVal;
}

/**
 * @internal
 * @brief         Returns the the possibility of setting interrupt monitor enabling.
 */
Std_ReturnType Platform_Ipw_ValidateSetIrqMonitor(const Platform_Ipw_ConfigType *pConfig)
{
    Std_ReturnType RetVal = E_NOT_OK;
    
    if (pConfig->pIntMonitor->pIntmGenericConfig != NULL_PTR)
    {
        RetVal = E_OK;
    }
    return RetVal;
}
#endif

#define PLATFORM_STOP_SEC_CODE
#include "Platform_MemMap.h"
