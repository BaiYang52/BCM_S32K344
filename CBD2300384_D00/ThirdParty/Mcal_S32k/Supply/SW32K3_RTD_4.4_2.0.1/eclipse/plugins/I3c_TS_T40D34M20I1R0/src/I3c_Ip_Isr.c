/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : I3C
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
*   @addtogroup i3c_ip
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "I3c_Ip.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define I3C_IP_VENDOR_ID_ISR_C                      43
#define I3C_IP_AR_RELEASE_MAJOR_VERSION_ISR_C       4
#define I3C_IP_AR_RELEASE_MINOR_VERSION_ISR_C       4
#define I3C_IP_AR_RELEASE_REVISION_VERSION_ISR_C    0
#define I3C_IP_SW_MAJOR_VERSION_ISR_C               2
#define I3C_IP_SW_MINOR_VERSION_ISR_C               0
#define I3C_IP_SW_PATCH_VERSION_ISR_C               1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and I3c_Ip header file are of the same vendor */
#if (I3C_IP_VENDOR_ID_ISR_C != I3C_IP_VENDOR_ID)
    #error "I3c_Ip_Isr.c and I3c_Ip.h have different vendor ids"
#endif

/* Check if current file and I3c_Ip header file are of the same Autosar version */
#if ((I3C_IP_AR_RELEASE_MAJOR_VERSION_ISR_C     != I3C_IP_AR_RELEASE_MAJOR_VERSION) || \
     (I3C_IP_AR_RELEASE_MINOR_VERSION_ISR_C     != I3C_IP_AR_RELEASE_MINOR_VERSION) || \
     (I3C_IP_AR_RELEASE_REVISION_VERSION_ISR_C  != I3C_IP_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of I3c_Ip_Isr.c and I3c_Ip.h are different"
#endif

/* Check if current file and I3c_Ip header file are of the same software version */
#if ((I3C_IP_SW_MAJOR_VERSION_ISR_C != I3C_IP_SW_MAJOR_VERSION) || \
     (I3C_IP_SW_MINOR_VERSION_ISR_C != I3C_IP_SW_MINOR_VERSION) || \
     (I3C_IP_SW_PATCH_VERSION_ISR_C != I3C_IP_SW_PATCH_VERSION))
    #error "Software Version Numbers of I3c_Ip_Isr.c and I3c_Ip.h are different"
#endif

/*==================================================================================================
*                                           CODE
==================================================================================================*/
#define I3C_START_SEC_CODE
#include "I3c_MemMap.h"

extern void I3c_Ip_IRQHandler(const uint8 Instance);

#if (I3C_IP_0_AVAILABLE == STD_ON)
/* IAR requires prototypes */
ISR(I3c0_Isr);
/* Implementation of I3c0 IRQ Handler */
ISR(I3c0_Isr)
{
    I3c_Ip_IRQHandler(0U);
}
#endif /* I3C_IP_0_AVAILABLE == STD_ON */

#if (I3C_IP_1_AVAILABLE == STD_ON)
/* IAR requires prototypes */
ISR(I3c1_Isr);
/* Implementation of I3c1 IRQ Handler */
ISR(I3c1_Isr)
{
    I3c_Ip_IRQHandler(1U);
}
#endif /* I3C_IP_1_AVAILABLE == STD_ON */


#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
extern void I3c_Ip_SlaveTxDmaCompleteTransfer(const uint8 Instance);

#if (I3C_IP_0_AVAILABLE == STD_ON)
/* IAR requires prototypes */
void I3c_0_Ip_SlaveTxDmaCompleteCallback(void);
/* Implementation of I3c0 Tx callback function for Dma interrupt. */
void I3c_0_Ip_SlaveTxDmaCompleteCallback(void)
{
    I3c_Ip_SlaveTxDmaCompleteTransfer(0U);
}
#endif /* I3C_IP_0_AVAILABLE == STD_ON */

#if (I3C_IP_1_AVAILABLE == STD_ON)
/* IAR requires prototypes */
void I3c_1_Ip_SlaveTxDmaCompleteCallback(void);

/* Implementation of I3c1 Tx callback function for Dma interrupt. */
void I3c_1_Ip_SlaveTxDmaCompleteCallback(void)
{
    I3c_Ip_SlaveTxDmaCompleteTransfer(0U);
}
#endif /* I3C_IP_1_AVAILABLE == STD_ON */
#endif /* I3C_IP_DMA_FEATURE_AVAILABLE */

#define I3C_STOP_SEC_CODE
#include "I3c_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
