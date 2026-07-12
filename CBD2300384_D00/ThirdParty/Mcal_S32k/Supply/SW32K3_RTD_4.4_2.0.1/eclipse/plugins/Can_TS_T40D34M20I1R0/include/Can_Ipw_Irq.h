/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : FLEXCAN
*   Dependencies         : 
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
/*==================================================================================================
==================================================================================================*/

#ifndef CAN_IPW_IRQ_H
#define CAN_IPW_IRQ_H

/**
*   @file    Can_Ipw_Irq.h
*   @version 2.0.1
*
*   @brief   AUTOSAR Can - module interface.
*   @details API header for CAN driver.
*
*   @addtogroup CAN_DRIVER
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif
/*
* @page misra_violations MISRA-C:2012 violations
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.1, External identifiers shall be distinct.
* The used compilers use more than 31 chars for identifiers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.2, Identifiers declared in the same scope and name space shall be distinct.
* The used compilers use more than 31 chars for identifiers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.4, Macro identifiers shall be distinct.
* The used compilers use more than 31 chars for identifiers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.5, Identifiers shall be distinct from macro names.
* The used compilers use more than 31 chars for identifiers.
*
* @section Can_Ipw_Irq_h_REF_1
* Violates MISRA 2012 Advisory Rule 4.9, A function should be used in preference to a function-like macro where they are interchangeable.
* Function like macro are used to reduce code complexity.
*
* @section Can_Ipw_Irq_h_REF_2
* Violates MISRA 2012 Advisory Rule 2.5, A project should not contain unused macro declarations.
* The macro is used as depending on configuration.
*/
/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Can_Irq.h"
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CAN_IPW_IRQ_VENDOR_ID                   43
#define CAN_IPW_IRQ_AR_RELEASE_MAJOR_VERSION    4
#define CAN_IPW_IRQ_AR_RELEASE_MINOR_VERSION    4
#define CAN_IPW_IRQ_AR_RELEASE_REVISION_VERSION 0
#define CAN_IPW_IRQ_SW_MAJOR_VERSION            2
#define CAN_IPW_IRQ_SW_MINOR_VERSION            0
#define CAN_IPW_IRQ_SW_PATCH_VERSION            1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and Can_Irq.h are of the same vendor */
#if (CAN_IPW_IRQ_VENDOR_ID != CAN_IRQ_VENDOR_ID)
#error "Can_IPW_Irq.h and Can_Irq.h have different vendor ids"
#endif
/* Check if current file and Can_Irq.h are of the same Autosar version */
#if ((CAN_IPW_IRQ_AR_RELEASE_MAJOR_VERSION != CAN_IRQ_AR_RELEASE_MAJOR_VERSION) || \
     (CAN_IPW_IRQ_AR_RELEASE_MINOR_VERSION != CAN_IRQ_AR_RELEASE_MINOR_VERSION) || \
     (CAN_IPW_IRQ_AR_RELEASE_REVISION_VERSION != CAN_IRQ_AR_RELEASE_REVISION_VERSION) \
    )
  #error "AutoSar Version Numbers of Can_IPW_Irq.h and Can_Irq.h are different"
#endif

/* Check if current file and Can_Irq.h are of the same software version */
#if ((CAN_IPW_IRQ_SW_MAJOR_VERSION != CAN_IRQ_SW_MAJOR_VERSION) || \
     (CAN_IPW_IRQ_SW_MINOR_VERSION != CAN_IRQ_SW_MINOR_VERSION) || \
     (CAN_IPW_IRQ_SW_PATCH_VERSION != CAN_IRQ_SW_PATCH_VERSION) \
    )
  #error "Software Version Numbers of Can_IPW_Irq.h and Can_Irq.h are different"
#endif
/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
/* @violates @ref Can_Ipw_Irq_h_REF_1 MISRA 2012 Advisory Rule 4.9 */
/* @violates @ref Can_Ipw_Irq_h_REF_2 MISRA 2012 Advisory Rule 2.5 */
#define Can_Ipw_ProcessMesgBufferCommonInterrupt(u8CtrlOffset, u8FirstMbIdx, u8LastMbIdx)    Can_ProcessMesgBufferCommonInterrupt((u8CtrlOffset), (u8FirstMbIdx), (u8LastMbIdx))
/* @violates @ref Can_Ipw_Irq_h_REF_1 MISRA 2012 Advisory Rule 4.9 */
/* @violates @ref Can_Ipw_Irq_h_REF_2 MISRA 2012 Advisory Rule 2.5 */
#define Can_Ipw_ProcessBusOffInterrupt(u8CtrlOffset)    Can_ProcessBusOffInterrupt(u8CtrlOffset)
/* @violates @ref Can_Ipw_Irq_h_REF_1 MISRA 2012 Advisory Rule 4.9 */
/* @violates @ref Can_Ipw_Irq_h_REF_2 MISRA 2012 Advisory Rule 2.5 */
#define Can_Ipw_ProcessErrorInterrupt(u8CtrlOffset, bIsErrFast)     Can_ProcessErrorInterrupt(u8CtrlOffset, bIsErrFast)
/* @violates @ref Can_Ipw_Irq_h_REF_1 MISRA 2012 Advisory Rule 4.9 */
/* @violates @ref Can_Ipw_Irq_h_REF_2 MISRA 2012 Advisory Rule 2.5 */
#define Can_Ipw_ProcessEnhancedFiFoInterrupt(u8CtrlOffset)     Can_ProcessEnhancedFiFoInterrupt(u8CtrlOffset)

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CAN_IPW_IRQ_H */