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

#ifndef PLATFORM_IPW_H_
#define PLATFORM_IPW_H_

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

#include "Platform_Ipw_Cfg.h"
#include "IntCtrl_Ip.h"
#include "System_Ip.h"
#include "Intm_Ip.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define PLATFORM_PLATFORM_IPW_VENDOR_ID                    43
#define PLATFORM_PLATFORM_IPW_AR_RELEASE_MAJOR_VERSION     4
#define PLATFORM_PLATFORM_IPW_AR_RELEASE_MINOR_VERSION     4
#define PLATFORM_PLATFORM_IPW_AR_RELEASE_REVISION_VERSION  0
#define PLATFORM_PLATFORM_IPW_SW_MAJOR_VERSION             2
#define PLATFORM_PLATFORM_IPW_SW_MINOR_VERSION             0
#define PLATFORM_PLATFORM_IPW_SW_PATCH_VERSION             1

/*==================================================================================================
*                                       GLOBAL VARIABLES
==================================================================================================*/
extern uint32 VTABLE[((uint32)(INT_CTRL_IP_IRQ_MAX)) + 16U + 1U];


/*==================================================================================================
*                                      FUNCTION PROTOTYPES
==================================================================================================*/
#define PLATFORM_START_SEC_CODE
#include "Platform_MemMap.h"

/**
 * @internal
 * @brief         Initializes the platform settings based on user configuration.
 */
void Platform_Ipw_Init(const Platform_Ipw_ConfigType *pConfig);

#if (PLATFORM_MSCM_CFG == STD_ON)
/**
 * @internal
 * @brief         Initializes the platform settings for non-core elements based on user configuration.
 */
void Platform_Ipw_InitNonCore(const Platform_Ipw_NonCoreConfigType *pConfig);
#endif

/**
 * @internal
 * @brief         Enables/disables an interrupt by calling the Interrupt Controller IP layer.
 */
static inline void Platform_Ipw_SetIrq(IRQn_Type eIrqNumber, boolean bEnable)
{
    if (bEnable)
    {
        IntCtrl_Ip_EnableIrq(eIrqNumber);
    }
    else
    {
        IntCtrl_Ip_DisableIrq(eIrqNumber);
    }
}

/**
 * @internal
 * @brief         Sets the priority of an interrupt by calling the Interrupt Controller IP layer.
 */
static inline void Platform_Ipw_SetIrqPriority(IRQn_Type eIrqNumber, uint8 u8Priority)
{
    IntCtrl_Ip_SetPriority(eIrqNumber, u8Priority);
}

/**
 * @internal
 * @brief         Returns the priority of an interrupt by calling the Interrupt Controller IP layer.
 */
static inline uint8 Platform_Ipw_GetIrqPriority(IRQn_Type eIrqNumber)
{
    return IntCtrl_Ip_GetPriority(eIrqNumber);
}

/**
 * @internal
 * @brief         Installs a new interrupt handler by calling the Interrupt Controller IP layer.
 */
static inline void Platform_Ipw_InstallIrqHandler(IRQn_Type eIrqNumber,
                                                  const IntCtrl_Ip_IrqHandlerType pfNewHandler,
                                                  IntCtrl_Ip_IrqHandlerType* const pfOldHandler)
{
    IntCtrl_Ip_InstallHandler(eIrqNumber, pfNewHandler, pfOldHandler);
}
#if (PLATFORM_INT_MONITOR == STD_ON)
/**
 * @internal
 * @brief         Enables/disables interrupt monitoring feature by calling the Interrupt Monitor IP layer.
 */
void Platform_Ipw_SetIrqMonitor(boolean bEnable);


/**
 * @internal
 * @brief         Acknowledges a monitored interrupt by calling the Interrupt Monitor IP layer.
 */
#define Platform_Ipw_AckIrq(eIrqNumber) Intm_Ip_AckIrq(eIrqNumber)


/**
 * @internal
 * @brief         Selects an interrupt to be monitored by calling the Interrupt Monitor IP layer.
 */
void Platform_Ipw_SelectMonitoredIrq(const Platform_Ipw_ConfigType *pConfig,
                                                uint8 u8Channel,
                                                IRQn_Type eIrqNumber);

/**
 * @internal
 * @brief         Selects an accepted latency for a monitored interrupt
 *                by calling the Interrupt Monitor IP layer.
 */
void Platform_Ipw_SetMonitoredIrqLatency(const Platform_Ipw_ConfigType *pConfig,
                                                    uint8 u8Channel,
                                                    uint32 u32Latency);


/**
 * @internal
 * @brief         Resets an interrupt monitor timer by calling the Interrupt Monitor IP layer.
 */
void Platform_Ipw_ResetIrqMonitorTimer(const Platform_Ipw_ConfigType *pConfig,
                                                    uint8 u8Channel);

/**
 * @internal
 * @brief         Returns the status of an interrupt monitor by calling the Interrupt Monitor IP layer.
 */
boolean Platform_Ipw_GetIrqMonitorStatus(const Platform_Ipw_ConfigType *pConfig,
                                                        uint8 u8Channel);

/**
 * @internal
 * @brief         Returns the validation result for given logical channel.
 */
Std_ReturnType Platform_Ipw_ValidateChannelIntm(const Platform_Ipw_ConfigType *pConfig,
                                                    uint8 u8Channel);
/**
 * @internal
 * @brief         Returns the the possibility of setting interrupt monitor enabling.
 */
    
Std_ReturnType Platform_Ipw_ValidateSetIrqMonitor(const Platform_Ipw_ConfigType *pConfig);

#endif

#define PLATFORM_STOP_SEC_CODE
#include "Platform_MemMap.h"


#endif /* PLATFORM_IPW_H_ */
