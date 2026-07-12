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

#ifndef SYSTEM_IP_H_
#define SYSTEM_IP_H_

/**
*   @file
*
*   @defgroup   System_Ip System IP
*   @ingroup    Platform
*
*   @addtogroup System_Ip
*   @{
*/

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "System_Ip_Cfg.h"
#include "Devassert.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define PLATFORM_SYSTEM_IP_VENDOR_ID                         43
#define PLATFORM_SYSTEM_IP_AR_RELEASE_MAJOR_VERSION          4
#define PLATFORM_SYSTEM_IP_AR_RELEASE_MINOR_VERSION          4
#define PLATFORM_SYSTEM_IP_AR_RELEASE_REVISION_VERSION       0
#define PLATFORM_SYSTEM_IP_SW_MAJOR_VERSION                  2
#define PLATFORM_SYSTEM_IP_SW_MINOR_VERSION                  0
#define PLATFORM_SYSTEM_IP_SW_PATCH_VERSION                  1

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and System_Ip_Cfg header file are of the same vendor */
#if (PLATFORM_SYSTEM_IP_VENDOR_ID != PLATFORM_SYSTEM_IP_CFG_TYPES_VENDOR_ID)
    #error "System_Ip.h and System_Ip_Cfg.h have different vendor ids"
#endif

/* Check if current file and System_Ip_Cfg header file are of the same Software version */
#if ((PLATFORM_SYSTEM_IP_SW_MAJOR_VERSION != PLATFORM_SYSTEM_IP_CFG_SW_MAJOR_VERSION) || \
     (PLATFORM_SYSTEM_IP_SW_MINOR_VERSION != PLATFORM_SYSTEM_IP_CFG_SW_MINOR_VERSION) || \
     (PLATFORM_SYSTEM_IP_SW_PATCH_VERSION != PLATFORM_SYSTEM_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of System_Ip.h and System_Ip_Cfg.h are different"
#endif

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
#define PLATFORM_START_SEC_CODE
#include "Platform_MemMap.h"

/**
 * @brief         Selects the access priority on the AHBS port of the Cortex-M7.
 *
 * @details       This function is non-reentrant and configures the AHB slave priority.
 *
 * @param[in]     bPriority: FALSE - round-robin arbitration scheme,
 *                           TRUE - AHB-slave access has priority over a core access
 * @return        void
 *
 * @api
 *
 * @implements    System_Ip_SetAhbSlavePriority_Activity
 *
 * */
void System_Ip_SetAhbSlavePriority(boolean bPriority);

#if ((SYSTEM_IP_ARM_CORTEXM == STD_ON) && (SYSTEM_FPU_CFG == STD_ON)) 
/**
 * @brief         Enables/disables core-related interrupt exceptions.
 *
 * @details       This function is non-reentrant and configures core-related interrupt
 *                exceptions, as defined per each platform.
 *
 * @param[in]     eIrq: core-related interrupt event.
 * @param[in]     bEnable: FALSE - disable interrupt, TRUE - enable interrupt.
 * @return        void
 *
 * @api
 *
 * @implements    System_Ip_ConfigIrq_Activity
 *
 * */
void System_Ip_ConfigIrq(System_Ip_IrqType eIrq, boolean bEnable);
#endif
#ifdef MCM_HAS_WABS_SUPPORTED
/**
 * @brief         Clears Write Abort on Slave flag.
 *
 * @details       This function is reentrant and clears the flag indicating when a
 *                write abort has occurred on the AHBS interface.
 *
 * @return        void
 *
 * @api
 *
 * @implements    System_Ip_ClearWriteAbortFlag_Activity
 *
 * */
void System_Ip_ClearWriteAbortFlag(void);
#endif
#ifdef MCM_HAS_PLATFORM_REVISION
/**
 * @brief         Returns platform revision.
 *
 * @details       This function is reentrant and returns a software-visible revision number.
 *
 * @return        void
 *
 * @api
 *
 * @implements    System_Ip_GetPlatformRevision_Activity
 *
 * */
uint32 System_Ip_GetPlatformRevision(void);
#endif

#define PLATFORM_STOP_SEC_CODE
#include "Platform_MemMap.h"

#endif /* SYSTEM_IP_H_ */

/** @} */
