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


#ifndef ETH_INTERNAL_H
#define ETH_INTERNAL_H

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

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ETH_INTERNAL_VENDOR_ID                    43
#define ETH_INTERNAL_MODULE_ID                    88
#define ETH_INTERNAL_AR_RELEASE_MAJOR_VERSION     4
#define ETH_INTERNAL_AR_RELEASE_MINOR_VERSION     4
#define ETH_INTERNAL_AR_RELEASE_REVISION_VERSION  0
#define ETH_INTERNAL_SW_MAJOR_VERSION             2
#define ETH_INTERNAL_SW_MINOR_VERSION             0
#define ETH_INTERNAL_SW_PATCH_VERSION             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
#define ETH_START_SEC_CODE
#include "Eth_MemMap.h"


void Eth_ReportTransmission(
                                const uint8 CtrlIdx, \
                                const uint8 FifoIdx \
                           );

 
#define ETH_STOP_SEC_CODE
#include "Eth_MemMap.h"


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* ETH_INTERNAL_H */
