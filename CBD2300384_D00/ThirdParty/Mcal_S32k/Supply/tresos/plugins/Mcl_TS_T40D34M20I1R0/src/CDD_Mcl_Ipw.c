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
#ifdef __cplusplus
extern "C"{
#endif
/**
*   @file    CDD_Mcl_Ipw.c
*
*   @version 2.0.1
*
*   @brief   AUTOSAR Mcl - CDD Mcl Ipw driver source file.
*   @details
*
*   @addtogroup CDD_MCL_IPW_DRIVER MCL IPW Driver
*   @{
*/

#include "CDD_Mcl_Ipw.h"
#include "StandardTypes.h"
#include "Mcal.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define CDD_MCL_IPW_VENDOR_ID_C                       43
#define CDD_MCL_IPW_AR_RELEASE_MAJOR_VERSION_C        4
#define CDD_MCL_IPW_AR_RELEASE_MINOR_VERSION_C        4
#define CDD_MCL_IPW_AR_RELEASE_REVISION_VERSION_C     0
#define CDD_MCL_IPW_SW_MAJOR_VERSION_C                2
#define CDD_MCL_IPW_SW_MINOR_VERSION_C                0
#define CDD_MCL_IPW_SW_PATCH_VERSION_C                1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/* Check if CDD_Mcl_Ipw.c file and CDD_Mcl_Ipw.h file are of the same vendor */
#if (CDD_MCL_IPW_VENDOR_ID_C != CDD_MCL_IPW_VENDOR_ID)
    #error "CDD_Mcl_Ipw.c and CDD_Mcl_Ipw.h have different vendor ids"
#endif

/* Check if CDD_Mcl_Ipw.c file and CDD_Mcl_Ipw.h file are of the same Autosar version */
#if ((CDD_MCL_IPW_AR_RELEASE_MAJOR_VERSION_C != CDD_MCL_IPW_AR_RELEASE_MAJOR_VERSION) || \
     (CDD_MCL_IPW_AR_RELEASE_MINOR_VERSION_C != CDD_MCL_IPW_AR_RELEASE_MINOR_VERSION) || \
     (CDD_MCL_IPW_AR_RELEASE_REVISION_VERSION_C != CDD_MCL_IPW_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of CDD_Mcl_Ipw.c and CDD_Mcl_Ipw.h are different"
#endif

/* Check if CDD_Mcl_Ipw.c file and CDD_Mcl_Ipw.h file are of the same Software version */
#if ((CDD_MCL_IPW_SW_MAJOR_VERSION_C != CDD_MCL_IPW_SW_MAJOR_VERSION) || \
     (CDD_MCL_IPW_SW_MINOR_VERSION_C != CDD_MCL_IPW_SW_MINOR_VERSION) || \
     (CDD_MCL_IPW_SW_PATCH_VERSION_C != CDD_MCL_IPW_SW_PATCH_VERSION))
    #error "Software Version Numbers of CDD_Mcl_Ipw.c and CDD_Mcl_Ipw.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if CDD_Mcl_Ipw.c file and Mcal header file are of the same Autosar version */
#if ((CDD_MCL_IPW_AR_RELEASE_MAJOR_VERSION_C != MCAL_AR_RELEASE_MAJOR_VERSION) || \
     (CDD_MCL_IPW_AR_RELEASE_MINOR_VERSION_C != MCAL_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of CDD_Mcl_Ipw.c and Mcal.h are different"
#endif

/* Check if CDD_Mcl_Ipw.c file and StandardTypes header file are of the same Autosar version */
#if ((CDD_MCL_IPW_AR_RELEASE_MAJOR_VERSION_C != STD_AR_RELEASE_MAJOR_VERSION) || \
     (CDD_MCL_IPW_AR_RELEASE_MINOR_VERSION_C != STD_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of CDD_Mcl_Ipw.c and StandardTypes.h are different"
#endif
#endif /* DISABLE_MCAL_INTERMODULE_ASR_CHECK */

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
/*================================================================================================*/
#if (STD_ON == LCU_IP_IS_AVAILABLE)

#define MCL_START_SEC_CODE
/* @violates @ref CDD_MCL_IPW_C_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"

/* Initializes the LCU IP Driver */
Lcu_Ip_ReturnType Mcl_Ipw_Lcu_Init(const Lcu_Ip_InitType * const pxLcuInit)
{
    /* Return result value */
    return Lcu_Ip_Init(pxLcuInit);
}

Lcu_Ip_ReturnType Mcl_Ipw_Lcu_Deinit(void)
{
    /* Return result value */
    return Lcu_Ip_Deinit();
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuWriteProtect(const uint8 LogicInstance)
{
    /* Return result value */
    return Lcu_Ip_SetWriteProtect(LogicInstance);
}

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuWriteProtect(const uint8 LogicInstance)
{
    /* Return result value */
    return Lcu_Ip_GetWriteProtect(LogicInstance);
}

#if (STD_ON == LCU_IP_SYNC_FUNC_IS_AVAILABLE)

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncInputSwOverrideEnable(const Lcu_Ip_SyncInputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetSyncInputSwOverrideEnable(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncInputSwOverrideValue(const Lcu_Ip_SyncInputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetSyncInputSwOverrideValue(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncInputMuxSelect(const Lcu_Ip_SyncInputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetSyncInputMuxSelect(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncInputSwSyncMode(const Lcu_Ip_SyncInputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetSyncInputSwSyncMode(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputDebugMode(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetSyncOutputDebugMode(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputEnable(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetSyncOutputEnable(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputForceInputSensitivity(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetSyncOutputForceInputSensitivity(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputForceClearingMode(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetSyncOutputForceClearingMode(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputForceSyncSelect(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetSyncOutputForceSyncSelect(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputPolarity(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetSyncOutputPolarity(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputForceDma(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetSyncOutputForceDma(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputForceInt(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetSyncOutputForceInt(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputLutDma(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetSyncOutputLutDma(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputLutInt(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetSyncOutputLutInt(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputFallFilter(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetSyncOutputFallFilter(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputRiseFilter(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetSyncOutputRiseFilter(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuSyncOutputLutControl(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetSyncOutputLutControl(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_ClearLcuSyncOutputForceEvent(const Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_ClearSyncOutputForceEvent(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuSyncLogicInput(Lcu_Ip_SyncInputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_GetSyncLogicInput(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuSyncSwOverrideInput(Lcu_Ip_SyncInputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_GetSyncSwOverrideInput(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuSyncLogicOutput(Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_GetSyncLogicOutput(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuSyncForceOutput(Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_GetSyncForceOutput(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuSyncForceStatus(Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_GetSyncForceStatus(List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuSyncCombineForceInput(Lcu_Ip_SyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_GetSyncCombineForceInput(List, Dimension);
}
#endif /* #if (STD_ON == LCU_IP_SYNC_FUNC_IS_AVAILABLE) */

#if (STD_ON == LCU_IP_ASYNC_FUNC_IS_AVAILABLE)
Lcu_Ip_ReturnType Mcl_Ipw_SetLcuAsyncInputList(const uint8 LogicInput, const Lcu_Ip_AsyncInputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetAsyncInputList(LogicInput, List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_SetLcuAsyncOutputList(const uint8 LogicOutput, const Lcu_Ip_AsyncOutputValueType List[], const uint8 Dimension)
{
    /* Return result value */
    return Lcu_Ip_SetAsyncOutputList(LogicOutput, List, Dimension);
}

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuAsyncLogicInputInfo(const uint8 LogicInput, const Lcu_Ip_LogicInputInfoParamType Param, uint8 * const Value)
{
    /* Return result value */
    return Lcu_Ip_GetAsyncLogicInputInfo(LogicInput, Param, Value);
}

Lcu_Ip_ReturnType Mcl_Ipw_GetLcuAsyncLogicOutputInfo(const uint8 LogicOutput, const Lcu_Ip_LogicOutputInfoParamType Param, uint8 * const Value)
{
    /* Return result value */
    return Lcu_Ip_GetAsyncLogicOutputInfo(LogicOutput, (Lcu_Ip_LogicOutputInfoParamType)Param, Value);
}

#endif /* #if (STD_ON == LCU_IP_ASYNC_FUNC_IS_AVAILABLE) */

#define MCL_STOP_SEC_CODE
/* @violates @ref CDD_MCL_IPW_C_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"

#endif /* if(STD_ON == LCU_IP_IS_AVAILABLE) */

#ifdef __cplusplus
}
/** @} */
#endif/*GPT_IPW_C*/


