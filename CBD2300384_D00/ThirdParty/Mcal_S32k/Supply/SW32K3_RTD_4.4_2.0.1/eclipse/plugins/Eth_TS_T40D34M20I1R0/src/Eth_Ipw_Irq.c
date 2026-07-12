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
#include "Eth_Ipw.h"

#include "Gmac_Ip_Types.h"

#if (STD_ON == ETH_DEM_EVENT_DETECT)
#include "Dem.h"
#endif

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ETH_IPW_IRQ_VENDOR_ID_C                      43
#define ETH_IPW_IRQ_MODULE_ID_C                      88
#define ETH_IPW_IRQ_AR_RELEASE_MAJOR_VERSION_C       4
#define ETH_IPW_IRQ_AR_RELEASE_MINOR_VERSION_C       4
#define ETH_IPW_IRQ_AR_RELEASE_REVISION_VERSION_C    0
#define ETH_IPW_IRQ_SW_MAJOR_VERSION_C               2
#define ETH_IPW_IRQ_SW_MINOR_VERSION_C               0
#define ETH_IPW_IRQ_SW_PATCH_VERSION_C               1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Eth_Ipw.h */
#if (ETH_IPW_IRQ_VENDOR_ID_C != ETH_IPW_VENDOR_ID)
    #error "Eth_Ipw_Irq.c and Eth_Ipw.h have different vendor ids"
#endif
#if ((ETH_IPW_IRQ_AR_RELEASE_MAJOR_VERSION_C    != ETH_IPW_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_IPW_IRQ_AR_RELEASE_MINOR_VERSION_C    != ETH_IPW_AR_RELEASE_MINOR_VERSION) || \
     (ETH_IPW_IRQ_AR_RELEASE_REVISION_VERSION_C != ETH_IPW_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Eth_Ipw_Irq.c and Eth_Ipw.h are different"
#endif
#if ((ETH_IPW_IRQ_SW_MAJOR_VERSION_C != ETH_IPW_SW_MAJOR_VERSION) || \
     (ETH_IPW_IRQ_SW_MINOR_VERSION_C != ETH_IPW_SW_MINOR_VERSION) || \
     (ETH_IPW_IRQ_SW_PATCH_VERSION_C != ETH_IPW_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Eth_Ipw_Irq.c and Eth_Ipw.h are different"
#endif

/* Checks against Gmac_Ip_Types.h */
#if (ETH_IPW_IRQ_VENDOR_ID_C != GMAC_IP_TYPES_VENDOR_ID)
    #error "Eth_Ipw_Irq.c and Gmac_Ip_Types.h have different vendor ids"
#endif
#if ((ETH_IPW_IRQ_AR_RELEASE_MAJOR_VERSION_C    != GMAC_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_IPW_IRQ_AR_RELEASE_MINOR_VERSION_C    != GMAC_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (ETH_IPW_IRQ_AR_RELEASE_REVISION_VERSION_C != GMAC_IP_TYPES_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Eth_Ipw_Irq.c and Gmac_Ip_Types.h are different"
#endif
#if ((ETH_IPW_IRQ_SW_MAJOR_VERSION_C != GMAC_IP_TYPES_SW_MAJOR_VERSION) || \
     (ETH_IPW_IRQ_SW_MINOR_VERSION_C != GMAC_IP_TYPES_SW_MINOR_VERSION) || \
     (ETH_IPW_IRQ_SW_PATCH_VERSION_C != GMAC_IP_TYPES_SW_PATCH_VERSION))
    #error "Software Version Numbers of Eth_Ipw_Irq.c and Gmac_Ip_Types.h are different"
#endif

#if (STD_ON == ETH_DEM_EVENT_DETECT)
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Dem.h */
    #if ((ETH_IPW_IRQ_AR_RELEASE_MAJOR_VERSION_C != DEM_AR_RELEASE_MAJOR_VERSION) || \
         (ETH_IPW_IRQ_AR_RELEASE_MINOR_VERSION_C != DEM_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Eth_Ipw_Irq.c and Dem.h are different"
    #endif
#endif
#endif
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


/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
#define ETH_START_SEC_CODE
#include "Eth_MemMap.h"


void Eth_Ipw_SafetyIrqCallback(const uint8 CtrlIdx, const Gmac_Ip_SafetyErrorType Error);
void Eth_Ipw_SafetyIrqCallback(const uint8 CtrlIdx, const Gmac_Ip_SafetyErrorType Error)
{
    /* If Dem event enabled for safety */
#if (STD_ON == ETH_DEM_EVENT_DETECT)
    if ((uint32)STD_ON == (uint32)(Eth_Ipw_apxInternalCfg[CtrlIdx]->Eth_Ipw_DemEventsList.ETH_IPW_E_SAFETY_Cfg.state))
    {
        (void)Dem_SetEventStatus((Dem_EventIdType)(Eth_Ipw_apxInternalCfg[CtrlIdx]->Eth_Ipw_DemEventsList.ETH_IPW_E_SAFETY_Cfg.id), DEM_EVENT_STATUS_FAILED);
    }
#endif
    (void)CtrlIdx;
    (void)Error;
}


#define ETH_STOP_SEC_CODE
#include "Eth_MemMap.h"


#ifdef __cplusplus
}
#endif

/** @} */
