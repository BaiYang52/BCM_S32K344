/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : GMAC
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

#ifndef ETH_IPW_TRUSTEDFUNCTIONS_H
#define ETH_IPW_TRUSTEDFUNCTIONS_H

/**
*   @file
*
*   @addtogroup ETH_DRIVER Ethernet Driver
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

#include "Platform_Types.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ETH_IPW_TRUSTEDFUNCTIONS_VENDOR_ID                    43
#define ETH_IPW_TRUSTEDFUNCTIONS_AR_RELEASE_MAJOR_VERSION     4
#define ETH_IPW_TRUSTEDFUNCTIONS_AR_RELEASE_MINOR_VERSION     4
#define ETH_IPW_TRUSTEDFUNCTIONS_AR_RELEASE_REVISION_VERSION  0
#define ETH_IPW_TRUSTEDFUNCTIONS_SW_MAJOR_VERSION             2
#define ETH_IPW_TRUSTEDFUNCTIONS_SW_MINOR_VERSION             0
#define ETH_IPW_TRUSTEDFUNCTIONS_SW_PATCH_VERSION             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Checks against Platform_Types.h */
#if ((ETH_IPW_TRUSTEDFUNCTIONS_AR_RELEASE_MAJOR_VERSION    != PLATFORM_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_IPW_TRUSTEDFUNCTIONS_AR_RELEASE_MINOR_VERSION    != PLATFORM_TYPES_AR_RELEASE_MINOR_VERSION))
    #error "AUTOSAR Version Numbers of Eth_Ipw_TrustedFunctions.h and Platform_Types.h are different"
#endif

#endif

/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/
#define ETH_START_SEC_CODE
#include "Eth_MemMap.h"

extern void Eth_Ipw_SelectPhyInterface(uint32 RmiiMiiSelBit);

#define ETH_STOP_SEC_CODE
#include "Eth_MemMap.h"


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* ETH_IPW_TRUSTEDFUNCTIONS_H */
