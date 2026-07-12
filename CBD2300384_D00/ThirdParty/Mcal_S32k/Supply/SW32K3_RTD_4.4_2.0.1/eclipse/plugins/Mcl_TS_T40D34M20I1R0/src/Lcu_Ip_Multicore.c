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

/**
*   @file    Lcu_Ip_Multicore.c
*
*   @version 2.0.1
*
*   @brief   AUTOSAR Mcl - Lcu Ip driver source file.
*   @details
*
*   @addtogroup LCU_IP_DRIVER LCU IP Driver
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

#include "Lcu_Ip_Multicore.h"

/*==================================================================================================
                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define LCU_IP_MULTICORE_VENDOR_ID_C                       43
#define LCU_IP_MULTICORE_AR_RELEASE_MAJOR_VERSION_C        4
#define LCU_IP_MULTICORE_AR_RELEASE_MINOR_VERSION_C        4
#define LCU_IP_MULTICORE_AR_RELEASE_REVISION_VERSION_C     0
#define LCU_IP_MULTICORE_SW_MAJOR_VERSION_C                2
#define LCU_IP_MULTICORE_SW_MINOR_VERSION_C                0
#define LCU_IP_MULTICORE_SW_PATCH_VERSION_C                1

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
/* Check if Lcu_Ip_Multicore.c file and Lcu_Ip_Multicore.h file are of the same vendor */
#if (LCU_IP_MULTICORE_VENDOR_ID_C != LCU_IP_MULTICORE_VENDOR_ID)
    #error "Lcu_Ip_Multicore.c and Lcu_Ip_Multicore.h have different vendor ids"
#endif

/* Check if Lcu_Ip_Multicore.c file and Lcu_Ip_Multicore.h file are of the same Autosar version */
#if ((LCU_IP_MULTICORE_AR_RELEASE_MAJOR_VERSION_C != LCU_IP_MULTICORE_AR_RELEASE_MAJOR_VERSION) || \
     (LCU_IP_MULTICORE_AR_RELEASE_MINOR_VERSION_C != LCU_IP_MULTICORE_AR_RELEASE_MINOR_VERSION) || \
     (LCU_IP_MULTICORE_AR_RELEASE_REVISION_VERSION_C != LCU_IP_MULTICORE_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Lcu_Ip_Multicore.c and Lcu_Ip_Multicore.h are different"
#endif

/* Check if Lcu_Ip_Multicore.c file and Lcu_Ip_Multicore.h file are of the same Software version */
#if ((LCU_IP_MULTICORE_SW_MAJOR_VERSION_C != LCU_IP_MULTICORE_SW_MAJOR_VERSION) || \
     (LCU_IP_MULTICORE_SW_MINOR_VERSION_C != LCU_IP_MULTICORE_SW_MINOR_VERSION) || \
     (LCU_IP_MULTICORE_SW_PATCH_VERSION_C != LCU_IP_MULTICORE_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Lcu_Ip_Multicore.c and Lcu_Ip_Multicore.h are different"
#endif

#if (STD_ON == LCU_IP_IS_AVAILABLE)

#if (STD_ON == LCU_IP_MULTICORE_IS_AVAILABLE)
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
#define MCL_START_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Mcl_MemMap.h"

static uint8 Lcu_Ip_au8CoreRef[LCU_INSTANCE_COUNT][LCU_LC_COUNT];

#define MCL_STOP_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Mcl_MemMap.h"

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/
#define MCL_START_SEC_CODE
#include "Mcl_MemMap.h"

void Lcu_Ip_SetCoreRef(uint8 LcuId, uint8 LcId, uint8 CoreRef)
{
    Lcu_Ip_au8CoreRef[LcuId][LcId] = CoreRef;
}

Lcu_Ip_ReturnType Lcu_Ip_ValidateCore(uint8 LcuId, uint8 LcId)
{
    Lcu_Ip_ReturnType ReturnValue = LCU_IP_STATUS_WRONG_CORE;
    uint8 CurrentCoreId = (uint8)OsIf_GetCoreID();
    uint8 LcCoreRef = Lcu_Ip_au8CoreRef[LcuId][LcId];

    if (CurrentCoreId == LcCoreRef)
    {
        ReturnValue = LCU_IP_STATUS_SUCCESS;
    }
    else
    {
        /* Do Nothing */
    }
    return ReturnValue;
}

Lcu_Ip_ReturnType Lcu_Ip_OutputValidateCore(uint8 LcuId, uint8 HwLcOutputId)
{
    uint8 LcId = LCU_IP_MAPPING_OUTPUT_TO_LC(HwLcOutputId);
    return Lcu_Ip_ValidateCore(LcuId, LcId);
}

Lcu_Ip_ReturnType Lcu_Ip_InputValidateCore(uint8 LcuId, uint8 HwLcInputId)
{
    uint8 LcId = LCU_IP_MAPPING_INPUT_TO_LC(HwLcInputId);
    return Lcu_Ip_ValidateCore(LcuId, LcId);
}

Lcu_Ip_ReturnType Lcu_Ip_ValidateCoreInit(const Lcu_Ip_InitType * const ConfigPtr)
{
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_WRONG_CORE;
    uint8 CurrentCoreId = (uint8)OsIf_GetCoreID();
    uint8 LocLcuCoreRef = ConfigPtr->LcuCoreRef;

    if (CurrentCoreId == LocLcuCoreRef)
    {
        LocStatus = LCU_IP_STATUS_SUCCESS;
    }
    else
    {
        /* Do Nothing */
    }
    return LocStatus;
}

#define MCL_STOP_SEC_CODE
#include "Mcl_MemMap.h"

#endif /* STD_ON == LCU_IP_MULTICORE_IS_AVAILABLE */

#endif /* #if (STD_ON == LCU_IP_IS_AVAILABLE) */

#ifdef __cplusplus
}
#endif

/** @} */
