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
#include "Eth.h"
#include "Eth_Internal.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ETH_IRQ_VENDOR_ID_C                      43
#define ETH_IRQ_MODULE_ID_C                      88
#define ETH_IRQ_AR_RELEASE_MAJOR_VERSION_C       4
#define ETH_IRQ_AR_RELEASE_MINOR_VERSION_C       4
#define ETH_IRQ_AR_RELEASE_REVISION_VERSION_C    0
#define ETH_IRQ_SW_MAJOR_VERSION_C               2
#define ETH_IRQ_SW_MINOR_VERSION_C               0
#define ETH_IRQ_SW_PATCH_VERSION_C               1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Eth.h */
#if (ETH_IRQ_VENDOR_ID_C != ETH_VENDOR_ID)
    #error "Eth_Irq.c and Eth.h have different vendor ids"
#endif
#if ((ETH_IRQ_AR_RELEASE_MAJOR_VERSION_C    != ETH_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_IRQ_AR_RELEASE_MINOR_VERSION_C    != ETH_AR_RELEASE_MINOR_VERSION) || \
     (ETH_IRQ_AR_RELEASE_REVISION_VERSION_C != ETH_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Eth_Irq.c and Eth.h are different"
#endif
#if ((ETH_IRQ_SW_MAJOR_VERSION_C != ETH_SW_MAJOR_VERSION) || \
     (ETH_IRQ_SW_MINOR_VERSION_C != ETH_SW_MINOR_VERSION) || \
     (ETH_IRQ_SW_PATCH_VERSION_C != ETH_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Eth_Irq.c and Eth.h are different"
#endif

/* Checks against Eth_Internal.h */
#if (ETH_IRQ_VENDOR_ID_C != ETH_INTERNAL_VENDOR_ID)
    #error "Eth_Irq.c and Eth_Internal.h have different vendor ids"
#endif
#if ((ETH_IRQ_AR_RELEASE_MAJOR_VERSION_C    != ETH_INTERNAL_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_IRQ_AR_RELEASE_MINOR_VERSION_C    != ETH_INTERNAL_AR_RELEASE_MINOR_VERSION) || \
     (ETH_IRQ_AR_RELEASE_REVISION_VERSION_C != ETH_INTERNAL_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Eth_Irq.c and Eth_Internal.h are different"
#endif
#if ((ETH_IRQ_SW_MAJOR_VERSION_C != ETH_INTERNAL_SW_MAJOR_VERSION) || \
     (ETH_IRQ_SW_MINOR_VERSION_C != ETH_INTERNAL_SW_MINOR_VERSION) || \
     (ETH_IRQ_SW_PATCH_VERSION_C != ETH_INTERNAL_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Eth_Irq.c and Eth_Internal.h are different"
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


#if (STD_ON == ETH_RX_IRQ_ENABLED)
void Eth_RxIrqCallback(const uint8 CtrlIdx, const uint8 DMAChannel);
void Eth_RxIrqCallback(const uint8 CtrlIdx, const uint8 DMAChannel)
{
    Eth_RxStatusType RxStatus = ETH_NOT_RECEIVED;

    /* Driver initialization and spurious IRQ verification is done at the IP level */

    do
    {
        /* Assume there's a 1:1 mapping between Rx Queues and Rx DMA Channels */
        Eth_Receive(CtrlIdx, DMAChannel, &RxStatus);
    } while (ETH_NOT_RECEIVED != RxStatus);

    /* Do we need a timeout mechanism here? */
}
#endif


#if (STD_ON == ETH_TX_IRQ_ENABLED)
void Eth_TxIrqCallback(const uint8 CtrlIdx, const uint8 DMAChannel);
void Eth_TxIrqCallback(const uint8 CtrlIdx, const uint8 DMAChannel)
{
    /* Driver initialization and spurious IRQ verification is done at the IP level */

    /* Assume there's a 1:1 mapping between Tx Queues and Tx DMA Channels */
    Eth_ReportTransmission(CtrlIdx, DMAChannel);
}
#endif


#define ETH_STOP_SEC_CODE
#include "Eth_MemMap.h"

#ifdef __cplusplus
}
#endif
/** @} */
