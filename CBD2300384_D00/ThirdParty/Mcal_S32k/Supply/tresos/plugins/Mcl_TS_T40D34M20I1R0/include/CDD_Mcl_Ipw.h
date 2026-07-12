/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : DMA,CACHE,TRGMUX,LCU,EMIOS,FLEXIO
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

/* Prevention from multiple including the same header */
#ifndef CDD_MCL_IPW_H_
#define CDD_MCL_IPW_H_

#ifdef __cplusplus
extern "C"{
#endif

/**
*   @file    CDD_Mcl_Ipw.h
*
*   @version 2.0.1
*
*   @brief   AUTOSAR Mcl - CDD MCL Ipw header file.
*   @details
*
*   @addtogroup MCL_DRIVER MCL IPW Driver
*   @{
*/

#include "Lcu_Ip.h"
#include "Lcu_Ip_Types.h"
/*==================================================================================================
*                                SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CDD_MCL_IPW_VENDOR_ID                       43
#define CDD_MCL_IPW_AR_RELEASE_MAJOR_VERSION        4
#define CDD_MCL_IPW_AR_RELEASE_MINOR_VERSION        4
#define CDD_MCL_IPW_AR_RELEASE_REVISION_VERSION     0
#define CDD_MCL_IPW_SW_MAJOR_VERSION                2
#define CDD_MCL_IPW_SW_MINOR_VERSION                0
#define CDD_MCL_IPW_SW_PATCH_VERSION                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if header file and Lcu_Ip.h file are of the same vendor */
#if (CDD_MCL_IPW_VENDOR_ID != LCU_IP_VENDOR_ID)
    #error "CDD_Mcl_Ipw.h and Lcu_Ip.h have different vendor ids"
#endif

/* Check if header file and Lcu_Ip.h file are of the same Autosar version */
#if ((CDD_MCL_IPW_AR_RELEASE_MAJOR_VERSION != LCU_IP_AR_RELEASE_MAJOR_VERSION) || \
     (CDD_MCL_IPW_AR_RELEASE_MINOR_VERSION != LCU_IP_AR_RELEASE_MINOR_VERSION) || \
     (CDD_MCL_IPW_AR_RELEASE_REVISION_VERSION != LCU_IP_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of CDD_Mcl_Ipw.h and Lcu_Ip.h are different"
#endif

/* Check if header file and Lcu_Ip.h file are of the same Software version */
#if ((CDD_MCL_IPW_SW_MAJOR_VERSION != LCU_IP_SW_MAJOR_VERSION) || \
     (CDD_MCL_IPW_SW_MINOR_VERSION != LCU_IP_SW_MINOR_VERSION) || \
     (CDD_MCL_IPW_SW_PATCH_VERSION != LCU_IP_SW_PATCH_VERSION))
    #error "Software Version Numbers of CDD_Mcl_Ipw.h and Lcu_Ip.h are different"
#endif

/* Check if header file and Lcu_Ip_Types.h file are of the same vendor */
#if (CDD_MCL_IPW_VENDOR_ID != LCU_IP_TYPES_VENDOR_ID)
    #error "CDD_Mcl_Ipw.h and Lcu_Ip_Types.h have different vendor ids"
#endif

/* Check if header file and Lcu_Ip_Types.h file are of the same Autosar version */
#if ((CDD_MCL_IPW_AR_RELEASE_MAJOR_VERSION != LCU_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (CDD_MCL_IPW_AR_RELEASE_MINOR_VERSION != LCU_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (CDD_MCL_IPW_AR_RELEASE_REVISION_VERSION != LCU_IP_TYPES_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of CDD_Mcl_Ipw.h and Lcu_Ip_Types.h are different"
#endif

/* Check if header file and Lcu_Ip_Types.h file are of the same Software version */
#if ((CDD_MCL_IPW_SW_MAJOR_VERSION != LCU_IP_TYPES_SW_MAJOR_VERSION) || \
     (CDD_MCL_IPW_SW_MINOR_VERSION != LCU_IP_TYPES_SW_MINOR_VERSION) || \
     (CDD_MCL_IPW_SW_PATCH_VERSION != LCU_IP_TYPES_SW_PATCH_VERSION))
    #error "Software Version Numbers of CDD_Mcl_Ipw.h and Lcu_Ip_Types.h are different"
#endif

/*===============================================================================================
                                           ENUMS
===============================================================================================*/

/*===============================================================================================
                                           STRUCTS
===============================================================================================*/

/*==================================================================================================
*                                   GLOBAL FUNCTION PROTOTYPES
==================================================================================================*/
#if (STD_ON == LCU_IP_IS_AVAILABLE)

#define MCL_START_SEC_CODE
/* @violates @ref CDD_MCL_IPW_H_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"

Lcu_Ip_ReturnType Mcl_Ipw_Lcu_Init(const Lcu_Ip_InitType * const pxLcuInit);

Lcu_Ip_ReturnType Mcl_Ipw_Lcu_Deinit(void);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuWriteProtect(const uint8 LogicInstance);

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuWriteProtect(const uint8 LogicInstance);


#if (STD_ON == LCU_IP_SYNC_FUNC_IS_AVAILABLE)

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncInputSwOverrideEnable(const Lcu_Ip_SyncInputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncInputSwOverrideValue(const Lcu_Ip_SyncInputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncInputMuxSelect(const Lcu_Ip_SyncInputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncInputSwSyncMode(const Lcu_Ip_SyncInputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputDebugMode(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputEnable(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputForceInputSensitivity(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputForceClearingMode(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputForceSyncSelect(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputPolarity(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputForceDma(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputForceInt(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputLutDma(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputLutInt(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputFallFilter(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputRiseFilter(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputLutControl(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_ClearLcuSyncOutputForceEvent(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuSyncLogicInput(Lcu_Ip_SyncInputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuSyncSwOverrideInput(Lcu_Ip_SyncInputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuSyncLogicOutput(Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuSyncForceOutput(Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuSyncForceStatus(Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuSyncCombineForceInput(Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension);
#endif /* #if (STD_ON == LCU_IP_SYNC_FUNC_IS_AVAILABLE) */

#if (STD_ON == LCU_IP_ASYNC_FUNC_IS_AVAILABLE)
Lcu_Ip_ReturnType Mcl_Ipw_SetLcuAsyncInputList(const uint8 LogicInput, const Lcu_Ip_AsyncInputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuAsyncOutputList(const uint8 LogicOutput, const Lcu_Ip_AsyncOutputValueType List[], const uint8 Dimension);

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuAsyncLogicInputInfo(const uint8 LogicInput, const Lcu_Ip_LogicInputInfoParamType Param, uint8 * const Value);

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuAsyncLogicOutputInfo(const uint8 LogicOutput, const Lcu_Ip_LogicOutputInfoParamType Param, uint8 * const Value);

#endif /* #if (STD_ON == LCU_IP_ASYNC_FUNC_IS_AVAILABLE) */

#define MCL_STOP_SEC_CODE
/* @violates @ref CDD_MCL_IPW_H_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"

#endif /* #if (STD_ON == LCU_IP_IS_AVAILABLE) */

#ifdef __cplusplus
}
#endif

/** @} */

#endif  /* #ifndef CDD_MCL_IPW_H_ */

/*==================================================================================================
 *                                        END OF FILE
==================================================================================================*/
