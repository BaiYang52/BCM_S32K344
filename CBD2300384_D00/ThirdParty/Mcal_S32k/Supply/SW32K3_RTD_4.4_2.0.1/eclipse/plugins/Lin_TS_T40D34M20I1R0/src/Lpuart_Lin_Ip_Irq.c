/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : LPUART_FLEXIO
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
*   @file   Lpuart_Lin_Ip_Irq.c
*   @implements Lpuart_Lin_Ip_Irq.c_Artifact
*
*   @addtogroup LPUART_LIN_IP
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
#include "Lpuart_Lin_Ip.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define LPUART_LIN_IP_IRQ_VENDOR_ID_C                      43
#define LPUART_LIN_IP_IRQ_AR_RELEASE_MAJOR_VERSION_C       4
#define LPUART_LIN_IP_IRQ_AR_RELEASE_MINOR_VERSION_C       4
#define LPUART_LIN_IP_IRQ_AR_RELEASE_REVISION_VERSION_C    0
#define LPUART_LIN_IP_IRQ_SW_MAJOR_VERSION_C               2
#define LPUART_LIN_IP_IRQ_SW_MINOR_VERSION_C               0
#define LPUART_LIN_IP_IRQ_SW_PATCH_VERSION_C               1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and Lpuart_Lin_Ip header file are of the same Autosar version */
#if (LPUART_LIN_IP_IRQ_VENDOR_ID_C != LPUART_LIN_IP_VENDOR_ID)
    #error "Lpuart_Lin_Ip_Irq.c and Lpuart_Lin_Ip.h have different vendor ids"
#endif
#if ((LPUART_LIN_IP_IRQ_AR_RELEASE_MAJOR_VERSION_C    != LPUART_LIN_IP_AR_RELEASE_MAJOR_VERSION) || \
     (LPUART_LIN_IP_IRQ_AR_RELEASE_MINOR_VERSION_C    != LPUART_LIN_IP_AR_RELEASE_MINOR_VERSION) || \
     (LPUART_LIN_IP_IRQ_AR_RELEASE_REVISION_VERSION_C != LPUART_LIN_IP_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Lpuart_Lin_Ip_Irq.c and Lpuart_Lin_Ip.h are different"
#endif
/* Check if current file and Lpuart_Lin_Ip header file are of the same Software version */
#if ((LPUART_LIN_IP_IRQ_SW_MAJOR_VERSION_C != LPUART_LIN_IP_SW_MAJOR_VERSION) || \
     (LPUART_LIN_IP_IRQ_SW_MINOR_VERSION_C != LPUART_LIN_IP_SW_MINOR_VERSION) || \
     (LPUART_LIN_IP_IRQ_SW_PATCH_VERSION_C != LPUART_LIN_IP_SW_PATCH_VERSION) )
    #error "Software Version Numbers of Lpuart_Lin_Ip_Irq.c and Lpuart_Lin_Ip.h are different"
#endif

/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#define LIN_START_SEC_CODE
#include "Lin_MemMap.h"

#if (LPUART_INSTANCE_COUNT > 0U)
ISR(LPUART0_LIN_IP_RxTx_IRQHandler);

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART0_LIN_IP_ERR_IRQHandler);

#endif
#endif /* if (LPUART_INSTANCE_COUNT > 0U) */

#if (LPUART_INSTANCE_COUNT > 1U)
ISR(LPUART1_LIN_IP_RxTx_IRQHandler);


#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART1_LIN_IP_ERR_IRQHandler);

#endif
#endif /* if (LPUART_INSTANCE_COUNT > 1U) */

#if (LPUART_INSTANCE_COUNT > 2U)
ISR(LPUART2_LIN_IP_RxTx_IRQHandler);

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART2_LIN_IP_ERR_IRQHandler);

#endif
#endif /* if (LPUART_INSTANCE_COUNT > 2U) */

#if (LPUART_INSTANCE_COUNT > 3U)
ISR(LPUART3_LIN_IP_RxTx_IRQHandler);

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART3_LIN_IP_ERR_IRQHandler);

#endif
#endif /* if (LPUART_INSTANCE_COUNT > 3U) */

#if (LPUART_INSTANCE_COUNT > 4U)
ISR(LPUART4_LIN_IP_RxTx_IRQHandler);


#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART4_LIN_IP_ERR_IRQHandler);

#endif
#endif /* if (LPUART_INSTANCE_COUNT > 4U) */

#if (LPUART_INSTANCE_COUNT > 5U)
ISR(LPUART5_LIN_IP_RxTx_IRQHandler);


#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART5_LIN_IP_ERR_IRQHandler);

#endif
#endif /* if (LPUART_INSTANCE_COUNT > 5U) */

#if (LPUART_INSTANCE_COUNT > 6U)
ISR(LPUART6_LIN_IP_RxTx_IRQHandler);

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART6_LIN_IP_ERR_IRQHandler);

#endif
#endif /* if (LPUART_INSTANCE_COUNT > 6U) */

#if (LPUART_INSTANCE_COUNT > 7U)
ISR(LPUART7_LIN_IP_RxTx_IRQHandler);


#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART7_LIN_IP_ERR_IRQHandler);

#endif
#endif /* if (LPUART_INSTANCE_COUNT > 7U) */

#if (LPUART_INSTANCE_COUNT > 8U)
ISR(LPUART8_LIN_IP_RxTx_IRQHandler);

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART8_LIN_IP_ERR_IRQHandler);

#endif
#endif /* if (LPUART_INSTANCE_COUNT > 8U) */

#if (LPUART_INSTANCE_COUNT > 9U)
ISR(LPUART9_LIN_IP_RxTx_IRQHandler);

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART9_LIN_IP_ERR_IRQHandler);

#endif
#endif /* if (LPUART_INSTANCE_COUNT > 9U) */


#if (LPUART_INSTANCE_COUNT > 10U)
ISR(LPUART10_LIN_IP_RxTx_IRQHandler);

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART10_LIN_IP_ERR_IRQHandler);

#endif
#endif /* if (LPUART_INSTANCE_COUNT > 10U) */


#if (LPUART_INSTANCE_COUNT > 11U)
ISR(LPUART11_LIN_IP_RxTx_IRQHandler);

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART11_LIN_IP_ERR_IRQHandler);

#endif
#endif /* if (LPUART_INSTANCE_COUNT > 11U) */


#if (LPUART_INSTANCE_COUNT > 12U)
ISR(LPUART12_LIN_IP_RxTx_IRQHandler);


#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART12_LIN_IP_ERR_IRQHandler);

#endif
#endif /* if (LPUART_INSTANCE_COUNT > 12U) */


#if (LPUART_INSTANCE_COUNT > 13U)
ISR(LPUART13_LIN_IP_RxTx_IRQHandler);


#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART13_LIN_IP_ERR_IRQHandler);

#endif
#endif /* if (LPUART_INSTANCE_COUNT > 13U) */


#if (LPUART_INSTANCE_COUNT > 14U)
ISR(LPUART14_LIN_IP_RxTx_IRQHandler);


#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART14_LIN_IP_ERR_IRQHandler);

#endif
#endif /* if (LPUART_INSTANCE_COUNT > 14U) */


#if (LPUART_INSTANCE_COUNT > 15U)
ISR(LPUART15_LIN_IP_RxTx_IRQHandler);

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART15_LIN_IP_ERR_IRQHandler);
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 15U) */

#define LIN_STOP_SEC_CODE
#include "Lin_MemMap.h"

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/
#define LIN_START_SEC_CODE
#include "Lin_MemMap.h"

#if (LPUART_INSTANCE_COUNT > 0U)
ISR(LPUART0_LIN_IP_RxTx_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(0U);
    
    EXIT_INTERRUPT();
}

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART0_LIN_IP_ERR_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(0U);
    
    EXIT_INTERRUPT();
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 0U) */

#if (LPUART_INSTANCE_COUNT > 1U)
ISR(LPUART1_LIN_IP_RxTx_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(1U);
    
    EXIT_INTERRUPT();
}

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART1_LIN_IP_ERR_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(1U);
    
    EXIT_INTERRUPT();
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 1U) */

#if (LPUART_INSTANCE_COUNT > 2U)
ISR(LPUART2_LIN_IP_RxTx_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(2U);
    
    EXIT_INTERRUPT();
}

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART2_LIN_IP_ERR_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(2U);
    
    EXIT_INTERRUPT();
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 2U) */

#if (LPUART_INSTANCE_COUNT > 3U)
ISR(LPUART3_LIN_IP_RxTx_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(3U);
    
    EXIT_INTERRUPT();
}

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART3_LIN_IP_ERR_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(3U);
    
    EXIT_INTERRUPT();
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 3U) */

#if (LPUART_INSTANCE_COUNT > 4U)
ISR(LPUART4_LIN_IP_RxTx_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(4U);
    
    EXIT_INTERRUPT();
}

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART4_LIN_IP_ERR_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(4U);
    
    EXIT_INTERRUPT();
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 4U) */

#if (LPUART_INSTANCE_COUNT > 5U)
ISR(LPUART5_LIN_IP_RxTx_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(5U);
    
    EXIT_INTERRUPT();
}

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART5_LIN_IP_ERR_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(5U);
    
    EXIT_INTERRUPT();
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 5U) */

#if (LPUART_INSTANCE_COUNT > 6U)
ISR(LPUART6_LIN_IP_RxTx_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(6U);
    
    EXIT_INTERRUPT();
}

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART6_LIN_IP_ERR_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(6U);
    
    EXIT_INTERRUPT();
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 6U) */

#if (LPUART_INSTANCE_COUNT > 7U)
ISR(LPUART7_LIN_IP_RxTx_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(7U);
    
    EXIT_INTERRUPT();
}

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART7_LIN_IP_ERR_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(7U);
    
    EXIT_INTERRUPT();
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 7U) */

#if (LPUART_INSTANCE_COUNT > 8U)
ISR(LPUART8_LIN_IP_RxTx_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(8U);
    
    EXIT_INTERRUPT();
}

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART8_LIN_IP_ERR_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(8U);
    
    EXIT_INTERRUPT();
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 8U) */

#if (LPUART_INSTANCE_COUNT > 9U)
ISR(LPUART9_LIN_IP_RxTx_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(9U);
    
    EXIT_INTERRUPT();
}

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART9_LIN_IP_ERR_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(9U);
    
    EXIT_INTERRUPT();
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 9U) */


#if (LPUART_INSTANCE_COUNT > 10U)
ISR(LPUART10_LIN_IP_RxTx_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(10U);
    
    EXIT_INTERRUPT();
}

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART10_LIN_IP_ERR_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(10U);
    
    EXIT_INTERRUPT();
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 10U) */


#if (LPUART_INSTANCE_COUNT > 11U)
ISR(LPUART11_LIN_IP_RxTx_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(11U);
    
    EXIT_INTERRUPT();
}

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART11_LIN_IP_ERR_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(11U);
    
    EXIT_INTERRUPT();
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 11U) */


#if (LPUART_INSTANCE_COUNT > 12U)
ISR(LPUART12_LIN_IP_RxTx_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(12U);
    
    EXIT_INTERRUPT();
}

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART12_LIN_IP_ERR_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(12U);
    
    EXIT_INTERRUPT();
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 12U) */


#if (LPUART_INSTANCE_COUNT > 13U)
ISR(LPUART13_LIN_IP_RxTx_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(13U);
    
    EXIT_INTERRUPT();
}

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART13_LIN_IP_ERR_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(13U);
    
    EXIT_INTERRUPT();
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 13U) */


#if (LPUART_INSTANCE_COUNT > 14U)
ISR(LPUART14_LIN_IP_RxTx_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(14U);
    
    EXIT_INTERRUPT();
}

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART14_LIN_IP_ERR_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(14U);
    
    EXIT_INTERRUPT();
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 14U) */


#if (LPUART_INSTANCE_COUNT > 15U)
ISR(LPUART15_LIN_IP_RxTx_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(15U);
    
    EXIT_INTERRUPT();
}

#ifdef LPUART_LIN_IP_ERR_IRQS
ISR(LPUART15_LIN_IP_ERR_IRQHandler)
{
    Lpuart_Lin_Ip_IRQHandler(15U);
    
    EXIT_INTERRUPT();
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 15U) */

#define LIN_STOP_SEC_CODE
#include "Lin_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
