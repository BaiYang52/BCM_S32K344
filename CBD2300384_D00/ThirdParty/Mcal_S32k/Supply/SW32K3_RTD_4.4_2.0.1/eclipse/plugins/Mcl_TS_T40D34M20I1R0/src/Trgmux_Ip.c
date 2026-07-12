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
*   @file    Trgmux_Ip.c
*
*   @version 2.0.1
*
*   @brief   AUTOSAR Mcl - Trgmux Ip driver source file.
*   @details
*
*   @addtogroup TRGMUX_IP_DRIVER TRGMUX IP Driver
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Trgmux_Ip.h"
#include "Trgmux_Ip_HwAcc.h"

#include "SchM_Mcl.h"

/*==================================================================================================
                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define TRGMUX_IP_VENDOR_ID_C                       43
#define TRGMUX_IP_AR_RELEASE_MAJOR_VERSION_C        4
#define TRGMUX_IP_AR_RELEASE_MINOR_VERSION_C        4
#define TRGMUX_IP_AR_RELEASE_REVISION_VERSION_C     0
#define TRGMUX_IP_SW_MAJOR_VERSION_C                2
#define TRGMUX_IP_SW_MINOR_VERSION_C                0
#define TRGMUX_IP_SW_PATCH_VERSION_C                1

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/

/* Check if Trgmux_Ip.c file and Trgmux_Ip.h file are of the same vendor */
#if (TRGMUX_IP_VENDOR_ID_C != TRGMUX_IP_VENDOR_ID)
    #error "Trgmux_Ip.c and Trgmux_Ip.h have different vendor ids"
#endif

/* Check if Trgmux_Ip.c file and Trgmux_Ip.h file are of the same Autosar version */
#if ((TRGMUX_IP_AR_RELEASE_MAJOR_VERSION_C != TRGMUX_IP_AR_RELEASE_MAJOR_VERSION) || \
     (TRGMUX_IP_AR_RELEASE_MINOR_VERSION_C != TRGMUX_IP_AR_RELEASE_MINOR_VERSION) || \
     (TRGMUX_IP_AR_RELEASE_REVISION_VERSION_C != TRGMUX_IP_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Trgmux_Ip.c and Trgmux_Ip.h are different"
#endif

/* Check if Trgmux_Ip.c file and Trgmux_Ip.h file are of the same Software version */
#if ((TRGMUX_IP_SW_MAJOR_VERSION_C != TRGMUX_IP_SW_MAJOR_VERSION) || \
     (TRGMUX_IP_SW_MINOR_VERSION_C != TRGMUX_IP_SW_MINOR_VERSION) || \
     (TRGMUX_IP_SW_PATCH_VERSION_C != TRGMUX_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Trgmux_Ip.c and Trgmux_Ip.h are different"
#endif

/* Check if Trgmux_Ip.c file and Trgmux_Ip_HwAcc.h file are of the same vendor */
#if (TRGMUX_IP_VENDOR_ID_C != TRGMUX_IP_HWACC_VENDOR_ID)
    #error "Trgmux_Ip.c and Trgmux_Ip_HwAcc.h have different vendor ids"
#endif

/* Check if Trgmux_Ip.c file and Trgmux_Ip_HwAcc.h file are of the same Autosar version */
#if ((TRGMUX_IP_AR_RELEASE_MAJOR_VERSION_C != TRGMUX_IP_HWACC_AR_RELEASE_MAJOR_VERSION) || \
     (TRGMUX_IP_AR_RELEASE_MINOR_VERSION_C != TRGMUX_IP_HWACC_AR_RELEASE_MINOR_VERSION) || \
     (TRGMUX_IP_AR_RELEASE_REVISION_VERSION_C != TRGMUX_IP_HWACC_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Trgmux_Ip.c and Trgmux_Ip_HwAcc.h are different"
#endif

/* Check if Trgmux_Ip.c file and Trgmux_Ip_HwAcc.h file are of the same Software version */
#if ((TRGMUX_IP_SW_MAJOR_VERSION_C != TRGMUX_IP_HWACC_SW_MAJOR_VERSION) || \
     (TRGMUX_IP_SW_MINOR_VERSION_C != TRGMUX_IP_HWACC_SW_MINOR_VERSION) || \
     (TRGMUX_IP_SW_PATCH_VERSION_C != TRGMUX_IP_HWACC_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Trgmux_Ip.c and Trgmux_Ip_HwAcc.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if Trgmux_Ip.c file and SchM_Mcl header file are of the same Autosar version */
#if ((TRGMUX_IP_AR_RELEASE_MAJOR_VERSION_C != SCHM_MCL_AR_RELEASE_MAJOR_VERSION) || \
     (TRGMUX_IP_AR_RELEASE_MINOR_VERSION_C != SCHM_MCL_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Trgmux_Ip.c and SchM_Mcl.h are different"
#endif
#endif

#if (STD_ON == TRGMUX_IP_IS_AVAILABLE)
/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/
#define MCL_START_SEC_CODE
#include "Mcl_MemMap.h"

#ifdef MCAL_ENABLE_USER_MODE_SUPPORT
    #if (STD_ON == TRGMUX_IP_USER_MODE_SUPPORT_IS_AVAILABLE)
        #define Trgmux_Ip_HwAcc_Init(pTrgmux) \
                (OsIf_Trusted_Call_Return1param((uint32)hwAcc_Init, pTrgmux))
        #define Trgmux_Ip_HwAcc_SetInputForOutput(pTrgmux, Input, Output) \
                (OsIf_Trusted_Call3params(hwAcc_SetInputForOutput, pTrgmux, Input, Output))
        #define Trgmux_Ip_HwAcc_SetLockForOutput(pTrgmux, Output) \
                (OsIf_Trusted_Call2params(hwAcc_SetLockForOutput, pTrgmux, Output))
        #define Trgmux_Ip_HwAcc_GetLockForOutput(pTrgmux, Output) \
                (OsIf_Trusted_Call_Return2param((uint32)hwAcc_GetLockForOutput, pTrgmux, Output))
    #else
        #define Trgmux_Ip_HwAcc_Init(pTrgmux) \
                (hwAcc_Init(pTrgmux))
        #define Trgmux_Ip_HwAcc_SetInputForOutput(pTrgmux, Input, Output) \
                (hwAcc_SetInputForOutput(pTrgmux, Input, Output))
        #define Trgmux_Ip_HwAcc_SetLockForOutput(pTrgmux, Output) \
                (hwAcc_SetLockForOutput(pTrgmux, Output))
        #define Trgmux_Ip_HwAcc_GetLockForOutput(pTrgmux, Output) \
                (hwAcc_GetLockForOutput(pTrgmux, Output))
    #endif /* STD_ON == TRGMUX_IP_USER_MODE_SUPPORT_IS_AVAILABLE */
#else
    #define Trgmux_Ip_HwAcc_Init(pTrgmux) \
            (hwAcc_Init(pTrgmux))
    #define Trgmux_Ip_HwAcc_SetInputForOutput(pTrgmux, Input, Output) \
            (hwAcc_SetInputForOutput(pTrgmux, Input, Output))
    #define Trgmux_Ip_HwAcc_SetLockForOutput(pTrgmux, Output) \
            (hwAcc_SetLockForOutput(pTrgmux, Output))
    #define Trgmux_Ip_HwAcc_GetLockForOutput(pTrgmux, Output) \
            (hwAcc_GetLockForOutput(pTrgmux, Output))
#endif /* MCAL_ENABLE_USER_MODE_SUPPORT */

#define MCL_STOP_SEC_CODE
#include "Mcl_MemMap.h"

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
#define MCL_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Mcl_MemMap.h"

static TRGMUX_Type * const Trgmux_Ip_paxBaseInst[TRGMUX_INSTANCE_COUNT] = IP_TRGMUX_BASE_PTRS;

#define MCL_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Mcl_MemMap.h"

#define MCL_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Mcl_MemMap.h"

/* Pointer to the TRGMUX Initialization Configuration. Based on this configuration pointer,
   the TRGMUX Driver obtains all information for the Logic Triggers and corresponding
   Triggers Configurations.
   The Pointer is loaded when Trgmux_Ip_Init() is called. */
static const Trgmux_Ip_InitType * Trgmux_Ip_pxInit;

#define MCL_STOP_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Mcl_MemMap.h"

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#define MCL_START_SEC_CODE
#include "Mcl_MemMap.h"

#if (TRGMUX_IP_MULTICORE_IS_AVAILABLE == STD_ON)
static Trgmux_Ip_StatusType Static_Trgmux_Ip_ValidateMultiCoreChannelCall(uint8 CoreId);
#endif /* TRGMUX_IP_MULTICORE_IS_AVAILABLE == STD_ON */

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/
#if (STD_ON == TRGMUX_IP_MULTICORE_IS_AVAILABLE)
static Trgmux_Ip_StatusType Static_Trgmux_Ip_ValidateMultiCoreChannelCall(uint8 CoreId)
{
    Trgmux_Ip_StatusType Valid = TRGMUX_IP_STATUS_WRONG_CORE;
    uint8 CurrentCoreId = (uint8)OsIf_GetCoreID();

    /* Check that the core can access Channel Trgmux */
    if (CurrentCoreId == CoreId)
    {
        Valid = TRGMUX_IP_STATUS_SUCCESS;
    }
    else
    {
        /* Do nothing */
    }
    return Valid;
}
#endif /* STD_ON == TRGMUX_IP_MULTICORE_IS_AVAILABLE */

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/
Trgmux_Ip_StatusType Trgmux_Ip_Init(const Trgmux_Ip_InitType * const pxTrgmuxInit)
{
    TRGMUX_Type * LocTrgmuxBase;
    uint32 LocTriggerIdx;
    uint32 LocInput;
    uint32 LocOutput;
    uint8 LocInst;
    boolean LocLock;
    Trgmux_Ip_StatusType Status = TRGMUX_IP_STATUS_SUCCESS;

    TRGMUX_IP_DEV_ASSERT(NULL_PTR != pxTrgmuxInit);

    /* Clear register */
    LocTrgmuxBase = Trgmux_Ip_paxBaseInst[TRGMUX_IP_HW_INST_0];
    /* Enter exclusive area to protect TRGMUXn */
    SchM_Enter_Mcl_MCL_EXCLUSIVE_AREA_19();

#ifdef MCAL_ENABLE_USER_MODE_SUPPORT
    #if (STD_ON == TRGMUX_IP_USER_MODE_SUPPORT_IS_AVAILABLE)
        if((Std_ReturnType)TRGMUX_IP_STATUS_SUCCESS != Trgmux_Ip_HwAcc_Init(LocTrgmuxBase))
    #else
        if(TRGMUX_IP_STATUS_SUCCESS != Trgmux_Ip_HwAcc_Init(LocTrgmuxBase))
    #endif
#else
        if(TRGMUX_IP_STATUS_SUCCESS != Trgmux_Ip_HwAcc_Init(LocTrgmuxBase))
#endif
        {
            Status =  TRGMUX_IP_STATUS_LOCKED;
        }
        else
        {
            /* Do nothing */
        }

    if(TRGMUX_IP_STATUS_SUCCESS == Status)
    {
        /* Set register with configured value */
        for(LocTriggerIdx = 0U; LocTriggerIdx < TRGMUX_IP_NOF_CFG_LOGIC_TRIGGERS; LocTriggerIdx++)
        {
            LocInst = pxTrgmuxInit->paxLogicTrigger[LocTriggerIdx]->HwInstId;
            LocOutput = pxTrgmuxInit->paxLogicTrigger[LocTriggerIdx]->Output;
            LocInput = pxTrgmuxInit->paxLogicTrigger[LocTriggerIdx]->Input;
            LocTrgmuxBase = Trgmux_Ip_paxBaseInst[LocInst];
            Trgmux_Ip_HwAcc_SetInputForOutput(LocTrgmuxBase, LocInput, LocOutput);
        }

        /* Set register lock */
        for(LocTriggerIdx = 0U; LocTriggerIdx < TRGMUX_IP_NOF_CFG_LOGIC_TRIGGERS; LocTriggerIdx++)
        {
            LocLock = pxTrgmuxInit->paxLogicTrigger[LocTriggerIdx]->Lock;
            if(TRUE == LocLock)
            {
                LocOutput = pxTrgmuxInit->paxLogicTrigger[LocTriggerIdx]->Output;
                Trgmux_Ip_HwAcc_SetLockForOutput(LocTrgmuxBase, LocOutput);
            }
        }
        /* Exit exclusive area to protect TRGMUXn */
        SchM_Exit_Mcl_MCL_EXCLUSIVE_AREA_19();

        Trgmux_Ip_pxInit = pxTrgmuxInit;
    }
    else
    {
        /* Do nothing*/
    }

    return Status;
}

Trgmux_Ip_StatusType Trgmux_Ip_SetInput(const uint32 LogicTrigger, const uint32 Input)
{
    uint32 LocHwInst = Trgmux_Ip_pxInit->paxLogicTrigger[LogicTrigger]->HwInstId;
    uint32 LocOutput = Trgmux_Ip_pxInit->paxLogicTrigger[LogicTrigger]->Output;
#if (STD_ON == TRGMUX_IP_MULTICORE_IS_AVAILABLE)
    uint8 CoreId = Trgmux_Ip_pxInit->paxLogicTrigger[LogicTrigger]->CoreId;
#endif /* STD_ON == TRGMUX_IP_MULTICORE_IS_AVAILABLE */
    TRGMUX_Type * LocTrgmuxBase = Trgmux_Ip_paxBaseInst[LocHwInst];
    Trgmux_Ip_StatusType Status = TRGMUX_IP_STATUS_SUCCESS;

#if (STD_ON == TRGMUX_IP_MULTICORE_IS_AVAILABLE)
    Status = Static_Trgmux_Ip_ValidateMultiCoreChannelCall(CoreId);
    if(TRGMUX_IP_STATUS_SUCCESS == Status)
    {
#endif /* STD_ON == TRGMUX_IP_MULTICORE_IS_AVAILABLE */
        if(FALSE != Trgmux_Ip_HwAcc_GetLockForOutput(LocTrgmuxBase, LocOutput))
        {
            Status = TRGMUX_IP_STATUS_LOCKED;
        }

        if(TRGMUX_IP_STATUS_LOCKED != Status)
        {
            /* Enter exclusive area to protect TRGMUXn */
            SchM_Enter_Mcl_MCL_EXCLUSIVE_AREA_20();
            Trgmux_Ip_HwAcc_SetInputForOutput(LocTrgmuxBase, Input, LocOutput);
            /* Exit exclusive area to protect TRGMUXn */
            SchM_Exit_Mcl_MCL_EXCLUSIVE_AREA_20();
        }
        else
        {
            /* Do nothing */
        }
#if (STD_ON == TRGMUX_IP_MULTICORE_IS_AVAILABLE)
    }
#endif /* STD_ON == TRGMUX_IP_MULTICORE_IS_AVAILABLE */

    return Status;
}

Trgmux_Ip_StatusType Trgmux_Ip_SetLock(const uint32 LogicTrigger)
{
    uint32 LocHwInst = Trgmux_Ip_pxInit->paxLogicTrigger[LogicTrigger]->HwInstId;
    uint32 LocOutput = Trgmux_Ip_pxInit->paxLogicTrigger[LogicTrigger]->Output;
#if (STD_ON == TRGMUX_IP_MULTICORE_IS_AVAILABLE)
    uint8 CoreId = Trgmux_Ip_pxInit->paxLogicTrigger[LogicTrigger]->CoreId;
#endif /* STD_ON == TRGMUX_IP_MULTICORE_IS_AVAILABLE */
    TRGMUX_Type * LocTrgmuxBase = Trgmux_Ip_paxBaseInst[LocHwInst];
    Trgmux_Ip_StatusType Status = TRGMUX_IP_STATUS_SUCCESS;

#if (STD_ON == TRGMUX_IP_MULTICORE_IS_AVAILABLE)
    Status = Static_Trgmux_Ip_ValidateMultiCoreChannelCall(CoreId);
    if(TRGMUX_IP_STATUS_SUCCESS == Status)
    {
#endif /* STD_ON == TRGMUX_IP_MULTICORE_IS_AVAILABLE */
        if(FALSE != Trgmux_Ip_HwAcc_GetLockForOutput(LocTrgmuxBase, LocOutput))
        {
            Status = TRGMUX_IP_STATUS_LOCKED;
        }

        if(TRGMUX_IP_STATUS_LOCKED != Status)
        {
            /* Enter exclusive area to protect TRGMUXn */
            SchM_Enter_Mcl_MCL_EXCLUSIVE_AREA_21();
            Trgmux_Ip_HwAcc_SetLockForOutput(LocTrgmuxBase, LocOutput);
            /* Exit exclusive area to protect TRGMUXn */
            SchM_Exit_Mcl_MCL_EXCLUSIVE_AREA_21();
        }
        else
        {
            /*Do nothing*/
        }
#if (STD_ON == TRGMUX_IP_MULTICORE_IS_AVAILABLE)
    }
#endif /* STD_ON == TRGMUX_IP_MULTICORE_IS_AVAILABLE */

    return Status;
}

#define MCL_STOP_SEC_CODE
#include "Mcl_MemMap.h"

#endif /* TRGMUX_IP_IS_AVAILABLE */

#ifdef __cplusplus
}
#endif

/** @} */
