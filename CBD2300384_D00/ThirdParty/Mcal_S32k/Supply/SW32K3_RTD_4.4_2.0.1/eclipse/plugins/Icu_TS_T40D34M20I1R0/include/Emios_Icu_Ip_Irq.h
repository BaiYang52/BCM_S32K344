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

#ifndef EMIOS_ICU_IRQ_H
#define EMIOS_ICU_IRQ_H

/**
 *   @file Emios_Icu_Ip_Irq.h
 *
 *   @addtogroup emios_icu_ip EMIOS IPL
 *   @{
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
#include "Emios_Icu_Ip_Cfg.h"
#include "Emios_Icu_Ip_Types.h"

/*==================================================================================================
*                                SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define EMIOS_ICU_IP_IRQ_VENDOR_ID                       43
#define EMIOS_ICU_IP_IRQ_AR_RELEASE_MAJOR_VERSION        4
#define EMIOS_ICU_IP_IRQ_AR_RELEASE_MINOR_VERSION        4
#define EMIOS_ICU_IP_IRQ_AR_RELEASE_REVISION_VERSION     0
#define EMIOS_ICU_IP_IRQ_SW_MAJOR_VERSION                2
#define EMIOS_ICU_IP_IRQ_SW_MINOR_VERSION                0
#define EMIOS_ICU_IP_IRQ_SW_PATCH_VERSION                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
#if (EMIOS_ICU_IP_IRQ_VENDOR_ID != EMIOS_ICU_IP_TYPES_VENDOR_ID)
    #error "Emios_Icu_Ip_Irq.h and Emios_Icu_Ip_Types.h have different vendor ids"
#endif

/* Check if  header file and Emios_Icu_Ip_Types.h file are of the same Autosar version */
#if ((EMIOS_ICU_IP_IRQ_AR_RELEASE_MAJOR_VERSION != EMIOS_ICU_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_ICU_IP_IRQ_AR_RELEASE_MINOR_VERSION != EMIOS_ICU_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_ICU_IP_IRQ_AR_RELEASE_REVISION_VERSION != EMIOS_ICU_IP_TYPES_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Emios_Icu_Ip_Irq.h and Emios_Icu_Ip_Types.h are different"
#endif

/* Check if header file and Emios_Icu_Ip_Types.h file are of the same Software version */
#if ((EMIOS_ICU_IP_IRQ_SW_MAJOR_VERSION != EMIOS_ICU_IP_TYPES_SW_MAJOR_VERSION) || \
     (EMIOS_ICU_IP_IRQ_SW_MINOR_VERSION != EMIOS_ICU_IP_TYPES_SW_MINOR_VERSION) || \
     (EMIOS_ICU_IP_IRQ_SW_PATCH_VERSION != EMIOS_ICU_IP_TYPES_SW_PATCH_VERSION))
    #error "Software Version Numbers of Emios_Icu_Ip_Irq.h and Emios_Icu_Ip_Types.h are different"
#endif

#if (EMIOS_ICU_IP_IRQ_VENDOR_ID != EMIOS_ICU_IP_CFG_VENDOR_ID)
    #error "Emios_Icu_Ip_Irq.h and Emios_Icu_Ip_Cfg.h have different vendor ids"
#endif

/* Check if  header file and Emios_Icu_Ip_Cfg.h file are of the same Autosar version */
#if ((EMIOS_ICU_IP_IRQ_AR_RELEASE_MAJOR_VERSION != EMIOS_ICU_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_ICU_IP_IRQ_AR_RELEASE_MINOR_VERSION != EMIOS_ICU_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_ICU_IP_IRQ_AR_RELEASE_REVISION_VERSION != EMIOS_ICU_IP_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Emios_Icu_Ip_Irq.h and Emios_Icu_Ip_Cfg.h are different"
#endif

/* Check if header file and Emios_Icu_Ip_Cfg.h file are of the same Software version */
#if ((EMIOS_ICU_IP_IRQ_SW_MAJOR_VERSION != EMIOS_ICU_IP_CFG_SW_MAJOR_VERSION) || \
     (EMIOS_ICU_IP_IRQ_SW_MINOR_VERSION != EMIOS_ICU_IP_CFG_SW_MINOR_VERSION) || \
     (EMIOS_ICU_IP_IRQ_SW_PATCH_VERSION != EMIOS_ICU_IP_CFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Emios_Icu_Ip_Irq.h and Emios_Icu_Ip_Cfg.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if this header file and OsIf.h file are of the same Autosar version */
    #if ((EMIOS_ICU_IP_IRQ_AR_RELEASE_MAJOR_VERSION != OSIF_AR_RELEASE_MAJOR_VERSION) || \
        (EMIOS_ICU_IP_IRQ_AR_RELEASE_MINOR_VERSION != OSIF_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Emios_Icu_Ip_Irq.h and OsIf.h are different"
    #endif
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/

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

/**
* @brief      Icu driver function that handles the interrupt of eMIOS channel.
* @details    This function:
*              - Reads the status register
*              - Clears the pending interrupt
*              - Processes interrupt for corresponding eMIOS channel
*
* @param[in]  channel - eMIOS hardware channel
*
*
*/
void Emios_Icu_Ip_IrqHandler(uint8 instance, uint8 channel);

#if (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON)
/**
* @brief      Icu driver function that handles the signal measurement type interrupt.
* @details    This service is  called when an  interrupt is recognized  as a Signal  Measurement
*             type. There are two branch depending on  the sub-function selected: Duty Cycle or
*             OTHER. Duty Cycle requires  an extra  variable, because  three values  are required:
*             two flanks for active signal time and another flank for the end of the pulse. For
*             calculating high, low and period is enough with the HW registers.
*
* @param[in]  instance  - eMIOS module index
* @param[in]  hwChannel - eMIOS Channel index
* @param[in]  bOverflow        Parameter that indicates the source of report is an overflow
*/
void Emios_Icu_Ip_SignalMeasurementHandler(const uint8 instance, const uint8 hwChannel, boolean bOverflow);
#endif

#define ICU_STOP_SEC_CODE
#include "Icu_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

#endif  /* EMIOS_ICU_IP_IRQ_H */
