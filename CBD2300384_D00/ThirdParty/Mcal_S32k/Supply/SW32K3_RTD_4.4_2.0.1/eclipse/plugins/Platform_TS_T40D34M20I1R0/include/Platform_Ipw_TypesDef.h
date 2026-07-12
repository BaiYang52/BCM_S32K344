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

#ifndef IPW_PLATFORM_IPW_TYPESDEF_H_
#define IPW_PLATFORM_IPW_TYPESDEF_H_

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

#include "Platform_CfgDefines.h"
#if (PLATFORM_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif
#include "IntCtrl_Ip_TypesDef.h"
#include "System_Ip_CfgDefines.h"
#include "Intm_Ip_CfgDefines.h"

/*==================================================================================================
*                                SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define PLATFORM_IPW_TYPESDEF_VENDOR_ID                       43
#define PLATFORM_IPW_TYPESDEF_AR_RELEASE_MAJOR_VERSION        4
#define PLATFORM_IPW_TYPESDEF_AR_RELEASE_MINOR_VERSION        4
#define PLATFORM_IPW_TYPESDEF_AR_RELEASE_REVISION_VERSION     0
#define PLATFORM_IPW_TYPESDEF_SW_MAJOR_VERSION                2
#define PLATFORM_IPW_TYPESDEF_SW_MINOR_VERSION                0
#define PLATFORM_IPW_TYPESDEF_SW_PATCH_VERSION                1


/*==================================================================================================
 *                              TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/**
 * @internal
 * @brief          Configuration structure core-related interrupt requests.
 */
typedef struct
{
    System_Ip_IrqType eIrq;
    boolean bEnable;
}Platform_SystemIrqConfigType;

/**
 * @internal
 * @brief          Configuration structure system settings.
 */
typedef struct
{
    const Platform_SystemIrqConfigType *aSystemIrqs;
    uint8 u8SysIrqsCount;
    boolean bAhbSlavePriority;
}Platform_SystemConfigType;

/**
 * @internal
 * @brief          Configuration structure system settings.
 */
typedef struct
{
    /** @brief Target cores for the interrupt */
    const IntCtrl_Ip_GlobalRouteConfigType *pIntRouteConfig;
}Platform_Ipw_NonCoreConfigType;

/**
 * @internal
 * @brief          Configuration structure channel interrupt monitors.
 */
typedef struct
{
    Intm_Ip_MonitorType eMonitor;
    IRQn_Type eMonitoredIrq;
    uint32 u32AcceptedLatency;
}Platform_IntmChannelConfigType;

/**
 * @internal
 * @brief          Configuration structure generic interrupt monitors.
 */
typedef struct
{
    boolean bEnableIntMonitor;
}Platform_IntmGenericConfigType;

/**
 * @internal
 * @brief          Configuration structure interrupt monitors.
 */
typedef struct
{
    /** @brief number of aIntmChannelConfig */
    uint8 u8NumChannel;
    /** @brief configuration for a logic channel */
    const Platform_IntmChannelConfigType *aIntmChannelConfig;
    /** @brief configuration generic part, just en/disable */
    const Platform_IntmGenericConfigType *pIntmGenericConfig;
    /** @brief channel index in each partition map table*/
    const uint8                 (*Platform_IndexChannelMap)[];
    
}Platform_IntmConfigType;

/**
 * @internal
 * @brief          Configuration structure for PLATFORM IP wrapper.
 */
typedef struct
{
    const IntCtrl_Ip_CtrlConfigType *pIntCtrlConfig;
    const Platform_SystemConfigType *aSystemConfig;
    const Platform_IntmConfigType * pIntMonitor;
} Platform_Ipw_ConfigType;

#endif /* IPW_PLATFORM_IPW_TYPESDEF_H_ */
