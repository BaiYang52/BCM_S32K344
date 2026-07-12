/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Emios Siul2 Wkpu LpCmp
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

#ifndef WKPU_IP_IRQ_H
#define WKPU_IP_IRQ_H

/**
 *     @file
 *
 *     @addtogroup wkpu_icu_ip WKPU IPL
 *     @{
 */

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                          INCLUDE FILES
*  1) system and project includes
*  2) needed interfaces from external units
*  3) internal and external interfaces from this unit
==================================================================================================*/
#include "OsIf.h"
#include "Wkpu_Ip_Cfg.h"
#include "Wkpu_Ip_Types.h"

/*==================================================================================================
*                                SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define WKPU_IP_IRQ_VENDOR_ID                      43

#define WKPU_IP_IRQ_AR_RELEASE_MAJOR_VERSION       4
#define WKPU_IP_IRQ_AR_RELEASE_MINOR_VERSION       4
#define WKPU_IP_IRQ_AR_RELEASE_REVISION_VERSION    0

#define WKPU_IP_IRQ_SW_MAJOR_VERSION               2
#define WKPU_IP_IRQ_SW_MINOR_VERSION               0
#define WKPU_IP_IRQ_SW_PATCH_VERSION               1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* TO DO Check if header file and OsIf.h file are of the same Autosar version */

/* Check if source file and ICU header file are of the same vendor */
#if (WKPU_IP_IRQ_VENDOR_ID != WKPU_IP_CFG_VENDOR_ID)
    #error "Wkpu_Ip_Irq.h and Wkpu_Ip_Cfg.h have different vendor IDs"
#endif
/* Check if source file and ICU header file are of the same AutoSar version */
#if ((WKPU_IP_IRQ_AR_RELEASE_MAJOR_VERSION  != WKPU_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (WKPU_IP_IRQ_AR_RELEASE_MINOR_VERSION  != WKPU_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (WKPU_IP_IRQ_AR_RELEASE_REVISION_VERSION   != WKPU_IP_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Wkpu_Ip_Irq.h and Wkpu_Ip_Cfg.h are different"
#endif
/* Check if source file and ICU header file are of the same Software version */
#if ((WKPU_IP_IRQ_SW_MAJOR_VERSION  != WKPU_IP_CFG_SW_MAJOR_VERSION) || \
     (WKPU_IP_IRQ_SW_MINOR_VERSION  != WKPU_IP_CFG_SW_MINOR_VERSION) || \
     (WKPU_IP_IRQ_SW_PATCH_VERSION  != WKPU_IP_CFG_SW_PATCH_VERSION))
#error "Software Version Numbers of Wkpu_Ip_Irq.h and Wkpu_Ip_Cfg.h are different"
#endif

/* Check if source file and ICU header file are of the same vendor */
#if (WKPU_IP_IRQ_VENDOR_ID != WKPU_IP_TYPES_VENDOR_ID)
    #error "Wkpu_Ip_Irq.h and Wkpu_Ip_Types.h have different vendor IDs"
#endif
/* Check if source file and ICU header file are of the same AutoSar version */
#if ((WKPU_IP_IRQ_AR_RELEASE_MAJOR_VERSION  != WKPU_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (WKPU_IP_IRQ_AR_RELEASE_MINOR_VERSION  != WKPU_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (WKPU_IP_IRQ_AR_RELEASE_REVISION_VERSION   != WKPU_IP_TYPES_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Wkpu_Ip_Irq.h and Wkpu_Ip_Types.h are different"
#endif
/* Check if source file and ICU header file are of the same Software version */
#if ((WKPU_IP_IRQ_SW_MAJOR_VERSION  != WKPU_IP_TYPES_SW_MAJOR_VERSION) || \
     (WKPU_IP_IRQ_SW_MINOR_VERSION  != WKPU_IP_TYPES_SW_MINOR_VERSION) || \
     (WKPU_IP_IRQ_SW_PATCH_VERSION  != WKPU_IP_TYPES_SW_PATCH_VERSION))
#error "Software Version Numbers of Wkpu_Ip_Irq.h and Wkpu_Ip_Types.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if this header file and OsIf.h file are of the same Autosar version */
    #if ((WKPU_IP_IRQ_AR_RELEASE_MAJOR_VERSION != OSIF_AR_RELEASE_MAJOR_VERSION) || \
        (WKPU_IP_IRQ_AR_RELEASE_MINOR_VERSION != OSIF_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Wkpu_Ip_Irq.h and OsIf.h are different"
    #endif
#endif
/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        DEFINES AND MACROS
==================================================================================================*/
#ifndef WKPU_IP_DERIVATIVE_NOT_SUPPORT_WKPU

#if ((defined ICU_WKPU_CH_0_ISR_USED) || (defined ICU_WKPU_CH_1_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_2_ISR_USED) || (defined ICU_WKPU_CH_3_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_4_ISR_USED) || (defined ICU_WKPU_CH_5_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_6_ISR_USED) || (defined ICU_WKPU_CH_7_ISR_USED))
/**
*@brief WKPU External Interrupt Channels defines 0-7
*/
#define ICU_WKPU_0    ((uint8) 0U)
#endif

#if ((defined ICU_WKPU_CH_8_ISR_USED) || (defined ICU_WKPU_CH_9_ISR_USED)   ||  \
     (defined ICU_WKPU_CH_10_ISR_USED) || (defined ICU_WKPU_CH_11_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_12_ISR_USED) || (defined ICU_WKPU_CH_13_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_14_ISR_USED) || (defined ICU_WKPU_CH_15_ISR_USED))
/**
*@brief WKPU External Interrupt Channels defines 8-15
*/
#define ICU_WKPU_8    ((uint8) 8U)
#endif

#if ((defined ICU_WKPU_CH_16_ISR_USED) || (defined ICU_WKPU_CH_17_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_18_ISR_USED) || (defined ICU_WKPU_CH_19_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_20_ISR_USED) || (defined ICU_WKPU_CH_21_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_22_ISR_USED) || (defined ICU_WKPU_CH_23_ISR_USED))
/**
*@brief WKPU External Interrupt Channels defines 15-23
*/
#define ICU_WKPU_16   ((uint8)16U)
#endif

#if ((defined ICU_WKPU_CH_24_ISR_USED) || (defined ICU_WKPU_CH_25_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_26_ISR_USED) || (defined ICU_WKPU_CH_27_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_28_ISR_USED) || (defined ICU_WKPU_CH_29_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_30_ISR_USED) || (defined ICU_WKPU_CH_31_ISR_USED))
/**
*@brief WKPU External Interrupt Channels defines 23-31
*/
#define ICU_WKPU_24   ((uint8)24U)
#endif

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/


/*==================================================================================================
*                                      FUNCTION PROTOTYPES
==================================================================================================*/
#define ICU_START_SEC_CODE
#include "Icu_MemMap.h"

#ifdef WKPU_IP_SUPPORT_INTERRUPT_REQUEST

#if (defined(ICU_WKPU_SINGLE_INTERRUPT) && (ICU_WKPU_SINGLE_INTERRUPT == STD_ON))

/**
 * @brief     Interrupt handler for WKPU channels: 0 to 31.
 * @details   Process the interrupt of WKPU channels 0 to 31 for platforms with only one interrupt line.
 *
 * @remarks   This will be defined only if any of WKPU channels 0 to 31 are configured.
 */
ISR(WKPU_EXT_IRQ_SINGLE_ISR);

#else

#if ((defined ICU_WKPU_CH_0_ISR_USED) || (defined ICU_WKPU_CH_1_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_2_ISR_USED) || (defined ICU_WKPU_CH_3_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_4_ISR_USED) || (defined ICU_WKPU_CH_5_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_6_ISR_USED) || (defined ICU_WKPU_CH_7_ISR_USED))

/**
 * @brief     Interrupt handler for WKPU channels: 0 to 7.
 * @details   Process the interrupt of WKPU channels 0 to 7.
 *
 * @remarks   This will be defined only if any of WKPU channels 0 to 7 is configured.
 */
ISR(WKPU_EXT_IRQ_0_7_ISR);

#endif

#if ((defined ICU_WKPU_CH_8_ISR_USED)  || (defined ICU_WKPU_CH_9_ISR_USED)   || \
     (defined ICU_WKPU_CH_10_ISR_USED) || (defined ICU_WKPU_CH_11_ISR_USED)  || \
     (defined ICU_WKPU_CH_12_ISR_USED) || (defined ICU_WKPU_CH_13_ISR_USED)  || \
     (defined ICU_WKPU_CH_14_ISR_USED) || (defined ICU_WKPU_CH_15_ISR_USED))

/**
 * @brief     Interrupt handler for WKPU channels: 8 to 15.
 * @details   Process the interrupt of WKPU channels 8 to 15.
 *
 * @remarks   This will be defined only if any of WKPU channels 8 to 15 is configured.
 */
ISR(WKPU_EXT_IRQ_8_15_ISR);

#endif

#if ((defined ICU_WKPU_CH_16_ISR_USED) || (defined ICU_WKPU_CH_17_ISR_USED)  || \
     (defined ICU_WKPU_CH_18_ISR_USED) || (defined ICU_WKPU_CH_19_ISR_USED)  || \
     (defined ICU_WKPU_CH_20_ISR_USED) || (defined ICU_WKPU_CH_21_ISR_USED)  || \
     (defined ICU_WKPU_CH_22_ISR_USED) || (defined ICU_WKPU_CH_23_ISR_USED))

/**
 * @brief     Interrupt handler for WKPU channels: 16 to 23.
 * @details   Process the interrupt of WKPU channels 16 to 23.
 *
 * @remarks   This will be defined only if any of WKPU channels 16 to 23 is configured.
 */
ISR(WKPU_EXT_IRQ_16_23_ISR);

#endif

#if ((defined ICU_WKPU_CH_24_ISR_USED) || (defined ICU_WKPU_CH_25_ISR_USED)  || \
     (defined ICU_WKPU_CH_26_ISR_USED) || (defined ICU_WKPU_CH_27_ISR_USED)  || \
     (defined ICU_WKPU_CH_28_ISR_USED) || (defined ICU_WKPU_CH_29_ISR_USED)  || \
     (defined ICU_WKPU_CH_30_ISR_USED) || (defined ICU_WKPU_CH_31_ISR_USED))

/**
 * @brief     Interrupt handler for WKPU channels: 24 to 31.
 * @details   Process the interrupt of WKPU channels 24 to 31.
 *
 * @remarks   This will be defined only if any of WKPU channels 24 to 31 is configured.
 */
ISR(WKPU_EXT_IRQ_24_31_ISR);

#endif
#endif /*defined ICU_WKPU_SINGLE_INTERRUPT*/

#endif /* #ifdef WKPU_IP_SUPPORT_INTERRUPT_REQUEST */

#define ICU_STOP_SEC_CODE
#include "Icu_MemMap.h"

#endif /* WKPU_IP_DERIVATIVE_NOT_SUPPORT_WKPU */

#ifdef __cplusplus
}
#endif

/**@}*/

#endif  /* WKPU_IP_IRQ_H */
