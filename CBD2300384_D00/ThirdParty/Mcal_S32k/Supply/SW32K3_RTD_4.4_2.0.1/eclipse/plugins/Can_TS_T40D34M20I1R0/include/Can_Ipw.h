/**
*   @file    Can_Ipw.h
*   @version 2.0.1
*
*   @brief   AUTOSAR Can - module interface
*   @details Main header file - can include different IPV models.
*
*   @addtogroup CAN_DRIVER
*   @{
*/
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

#ifndef CAN_IPW_H
#define CAN_IPW_H

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
* @section Can_Ipw_h_REF_1
* Violates MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents
* of a header file being included more than once. This comes from the order of includes in the .c file
* and from include dependencies. As a safe approach, any file must include all its dependencies.
* Header files are already protected against double inclusions. The inclusion of Can_Memmap.h is as
* per AUTOSAR requirement (SWS_MemMap_00003)
*
*/

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "FlexCAN_Ip.h"
#include "Can_Ipw_Cfg.h"
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/**
* @{
*
*/
#define CAN_IPW_VENDOR_ID_H                    43
#define CAN_IPW_MODULE_ID                      80
#define CAN_IPW_AR_RELEASE_MAJOR_VERSION_H     4
#define CAN_IPW_AR_RELEASE_MINOR_VERSION_H     4
#define CAN_IPW_AR_RELEASE_REVISION_VERSION_H  0
#define CAN_IPW_SW_MAJOR_VERSION_H             2
#define CAN_IPW_SW_MINOR_VERSION_H             0
#define CAN_IPW_SW_PATCH_VERSION_H             1
/**@}*/

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and FlexCAN_Ip header file are of the same vendor */
#if (CAN_IPW_VENDOR_ID_H != FLEXCAN_IP_VENDOR_ID_H)
    #error "Can_Ipw.h and FlexCAN_Ip.h have different vendor ids"
#endif
/* Check if current file and FlexCAN_Ip header file are of the same Autosar version */
#if ((CAN_IPW_AR_RELEASE_MAJOR_VERSION_H    != FLEXCAN_IP_AR_RELEASE_MAJOR_VERSION_H) || \
     (CAN_IPW_AR_RELEASE_MINOR_VERSION_H    != FLEXCAN_IP_AR_RELEASE_MINOR_VERSION_H) || \
     (CAN_IPW_AR_RELEASE_REVISION_VERSION_H != FLEXCAN_IP_AR_RELEASE_REVISION_VERSION_H))
    #error "AutoSar Version Numbers of Can_Ipw.h and FlexCAN_Ip.h are different"
#endif
/* Check if current file and FlexCAN_Ip header file are of the same Software version */
#if ((CAN_IPW_SW_MAJOR_VERSION_H != FLEXCAN_IP_SW_MAJOR_VERSION_H) || \
     (CAN_IPW_SW_MINOR_VERSION_H != FLEXCAN_IP_SW_MINOR_VERSION_H) || \
     (CAN_IPW_SW_PATCH_VERSION_H != FLEXCAN_IP_SW_PATCH_VERSION_H))
    #error "Software Version Numbers of Can_Ipw.h and FlexCAN_Ip.h are different"
#endif

/* Check if current file and Can_Ipw_Cfg header file are of the same vendor */
#if (CAN_IPW_VENDOR_ID_H != CAN_IPW_CFG_VENDOR_ID)
    #error "Can_Ipw.h and Can_Ipw_Cfg.h have different vendor ids"
#endif
/* Check if current file and Can_Ipw_Cfg header file are of the same Autosar version */
#if ((CAN_IPW_AR_RELEASE_MAJOR_VERSION_H    != CAN_IPW_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (CAN_IPW_AR_RELEASE_MINOR_VERSION_H    != CAN_IPW_CFG_AR_RELEASE_MINOR_VERSION) || \
     (CAN_IPW_AR_RELEASE_REVISION_VERSION_H != CAN_IPW_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Can_Ipw.h and Can_Ipw_Cfg.h are different"
#endif
/* Check if current file and Can_Ipw_Cfg header file are of the same Software version */
#if ((CAN_IPW_SW_MAJOR_VERSION_H != CAN_IPW_CFG_SW_MAJOR_VERSION) || \
     (CAN_IPW_SW_MINOR_VERSION_H != CAN_IPW_CFG_SW_MINOR_VERSION) || \
     (CAN_IPW_SW_PATCH_VERSION_H != CAN_IPW_CFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Can_Ipw.h and Can_Ipw_Cfg.h are different"
#endif
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
/* @violates @ref Can_Flexcan_h_REF_2 MISRA 2012 Advisory Rule 4.9 */
    #define Call_Can_FlexCan_Init(instance, status, Can_pControllerConfig)         FlexCAN_Ip_Init(instance, status, Can_pControllerConfig)
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
#define CAN_START_SEC_CODE
/* @violates @ref Can_Ipw_h_REF_1 MISRA 2012 Required Directive 4.10. */
#include "Can_MemMap.h"
/* Initialize Controller */
Std_ReturnType Can_Ipw_Init(const Can_ControllerConfigType * Can_pControllerConfig);

/* De-Initialize Controller */
void Can_Ipw_DeInit(const Can_ControllerConfigType * Can_pControllerConfig);

#if (CAN_SET_BAUDRATE_API == STD_ON)
/* Set Baud Rate of Controller */
Std_ReturnType Can_Ipw_SetBaudrate
    (
        const Can_ControllerConfigType * Can_pControllerConfig,
        uint16 BaudRateConfigID
    );
#endif
/* Set Controller to participate the CAN network */
Std_ReturnType Can_Ipw_SetControllerToStartMode(const Can_ControllerConfigType * Can_pControllerConfig);

/* Set Controller to stop participating the CAN network */
Std_ReturnType Can_Ipw_SetControllerToStopMode(const Can_ControllerConfigType * Can_pControllerConfig);

/* Disable Can Controller Interrupt */
void Can_Ipw_DisableControllerInterrupts(const Can_ControllerConfigType * Can_pControllerConfig);

/* Enable Can Controller Interrupt */
void Can_Ipw_EnableControllerInterrupts(const Can_ControllerConfigType * Can_pControllerConfig);

/* Get Controller Error State */
uint8 Can_Ipw_GetControllerErrorState(const Can_ControllerConfigType * Can_pControllerConfig);

/* Get Controller Rx Error Counter */
uint8 Can_Ipw_GetControllerRxErrorCounter(const Can_ControllerConfigType * Can_pControllerConfig);

/* Get Controller Tx Error Counter */
uint8 Can_Ipw_GetControllerTxErrorCounter(const Can_ControllerConfigType * Can_pControllerConfig);

#if (CAN_LISTEN_ONLY_MODE == STD_ON)
/* Enable or disable Listen Only Mode */
Std_ReturnType Can_Ipw_ListenOnlyMode(const Can_ControllerConfigType * Can_pControllerConfig, const Can_ListenOnlyType State);
#endif /* (CAN_LISTEN_ONLY_MODE == STD_ON) */

/* Write Pduinfo to Hw Buffer and request transmission */
Std_ReturnType Can_Ipw_Write
(
    const Can_ControllerConfigType * Can_pControllerConfig,
    const Can_HwObjectConfigType * Can_pHwObjectConfig,
    const Can_PduType * PduInfo
);

#if (CAN_ABORT_MB_API == STD_ON)
    /* Cancel of a pending CAN transmission */
    void Can_Ipw_AbortMb
    (
        const Can_ControllerConfigType * Can_pControllerConfig,
        const Can_HwObjectConfigType * Can_pHwObjectConfig
    );
#endif

#if (CAN_DUAL_CLOCK_MODE == STD_ON)
    /* Switch to another clock by changing controller's prescaler */
    void Can_Ipw_SetClockMode
    (
        const Can_ControllerConfigType * Can_pControllerConfig,
        uint16 Can_u16BaudrateID,
        Can_ClockModeType Can_eClkMode
    );
#endif
/* Polling Tx Confirmation */
void Can_Ipw_MainFunction_Write
(
    const Can_ControllerConfigType * Can_pControllerConfig,
    const Can_HwObjectConfigType * Can_pHwObjectConfig
);

/* Polling Rx Indication */
void Can_Ipw_MainFunction_Read
(
    const Can_ControllerConfigType * Can_pControllerConfig,
    const Can_HwObjectConfigType * Can_pHwObjectConfig
);

/* Polling Bus Off */
void Can_Ipw_MainFunction_BusOff(const Can_ControllerConfigType * Can_pControllerConfig);

/* Polling Controller Mode Transitions */
void Can_Ipw_MainFunction_Mode
(
    const Can_ControllerConfigType * Can_pControllerConfig,
    Can_ControllerStateType * Can_pControllerState
);

#if (CAN_MB_INTERRUPT_SUPPORT == STD_ON)
    /* Process Tx Interrupt */
    void Can_Ipw_ProcessTxMesgBuffer
    (
        const Can_ControllerConfigType * Can_pControllerConfig,
        const Can_HwObjectConfigType * Can_pHwObjectConfig,
        uint8 u8MbIdx
    );

    /* Process Rx Interrupt */
    void Can_Ipw_ProcessRxMesgBuffer
    (
        const Can_ControllerConfigType * Can_pControllerConfig,
        const Can_HwObjectConfigType * Can_pHwObjectConfig,
        uint8 u8MbIdx
    );
    #if (FLEXCAN_IP_FEATURE_HAS_DMA_ENABLE == STD_ON)
        void Can_Ipw_ProcessRxLegacyDma
        (
            const Can_ControllerConfigType * Can_pControllerConfig,
            const Can_HwObjectConfigType * Can_pHwObjectConfig,
            uint8 u8Event
        );
    #endif
    #if (CAN_ENHANCED_FIFO_ENABLED == STD_ON)
        /* Process Rx Enhance FIFO Interrupt */
        void Can_Ipw_ProcessRxEnhance
        (
            const Can_ControllerConfigType * Can_pControllerConfig,
            const Can_HwObjectConfigType * Can_pHwObjectConfig,
            uint8 u8Event
        );
    #endif

#endif

#if (CAN_TIMESTAMP_ENABLE == STD_ON)
void Can_Ipw_GetCurrentTime(Can_TimeStampType* timeStampPtr);

#if (CAN_TS_EXTERNAL_TIMER_USED == STD_ON)
void Can_Ipw_SetCurrentTime(const Can_TimeStampType* timeStampPtr);
#endif /* (CAN_TS_EXTERNAL_TIMER_USED == STD_ON) */

void Can_Ipw_EnableEgressTimeStamp
(
    const Can_ControllerConfigType * Can_pControllerConfig,
    const Can_HwObjectConfigType * Can_pHwObjectConfig
);

Std_ReturnType Can_Ipw_GetEgressTimeStamp
(
    const Can_ControllerConfigType * Can_pControllerConfig,
    const Can_HwObjectConfigType * Can_pHwObjectConfig,
    const PduIdType TxPduId,
    Can_TimeStampType* timeStampPtr
);

void Can_Ipw_GetIngressTimeStamp
(
    const Can_ControllerConfigType * Can_pControllerConfig,
    const Can_HwObjectConfigType * Can_pHwObjectConfig,
    Can_TimeStampType* timeStampPtr
);
#endif /* (CAN_TIMESTAMP_ENABLE == STD_ON) */

#define CAN_STOP_SEC_CODE
/* @violates @ref Can_Ipw_h_REF_1 MISRA 2012 Required Directive 4.10. */
#include "Can_MemMap.h"

#ifdef __cplusplus
}
#endif

#endif /*TEMPLATE_H*/

/** @} */
