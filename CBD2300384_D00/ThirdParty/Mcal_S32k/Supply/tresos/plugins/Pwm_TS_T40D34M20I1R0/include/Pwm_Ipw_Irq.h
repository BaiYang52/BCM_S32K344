/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Emios Flexio
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

#ifndef PWM_IPW_IRQ_H
#define PWM_IPW_IRQ_H

/**
*   @file       Pwm_Ipw_Irq.h
*
*   @addtogroup pwm_driver Pwm Driver
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
#include "Pwm.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define PWM_IPW_IRQ_VENDOR_ID                       43
#define PWM_IPW_IRQ_MODULE_ID                       121
#define PWM_IPW_IRQ_AR_RELEASE_MAJOR_VERSION        4
#define PWM_IPW_IRQ_AR_RELEASE_MINOR_VERSION        4
#define PWM_IPW_IRQ_AR_RELEASE_REVISION_VERSION     0
#define PWM_IPW_IRQ_SW_MAJOR_VERSION                2
#define PWM_IPW_IRQ_SW_MINOR_VERSION                0
#define PWM_IPW_IRQ_SW_PATCH_VERSION                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if header file and Pwm header file are of the same vendor */
#if (PWM_IPW_IRQ_VENDOR_ID != PWM_VENDOR_ID)
    #error "Vendor IDs Pwm_Ipw_Irq.h and Pwm.h are different."
#endif

/* Check if header file and Pwm header file are of the same AUTOSAR version */
#if ((PWM_IPW_IRQ_AR_RELEASE_MAJOR_VERSION    != PWM_AR_RELEASE_MAJOR_VERSION) || \
     (PWM_IPW_IRQ_AR_RELEASE_MINOR_VERSION    != PWM_AR_RELEASE_MINOR_VERSION) || \
     (PWM_IPW_IRQ_AR_RELEASE_REVISION_VERSION != PWM_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR version numbers of Pwm_Ipw_Irq.h and Pwm.h are different."
#endif

/* Check if header file and Pwm_Ipw_Irq header file are of the same software version */
#if ((PWM_IPW_IRQ_SW_MAJOR_VERSION != PWM_SW_MAJOR_VERSION) || \
     (PWM_IPW_IRQ_SW_MINOR_VERSION != PWM_SW_MINOR_VERSION) || \
     (PWM_IPW_IRQ_SW_PATCH_VERSION != PWM_SW_PATCH_VERSION))
    #error "Software version numbers of Pwm_Ipw_Irq.h and Pwm.h are different."
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
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
*                                       FUNCTION PROTOTYPES
==================================================================================================*/
#define PWM_START_SEC_CODE
#include "Pwm_MemMap.h"

#if (PWM_NOTIFICATION_SUPPORTED == STD_ON)
void Pwm_Ipw_Notification(uint8 Channel);
#endif /* PWM_NOTIFICATION_SUPPORTED */

#define PWM_STOP_SEC_CODE
#include "Pwm_MemMap.h"


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* PWM_IPW_IRQ_H */
