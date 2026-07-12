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
*   @file CDD_Mcl.c
*
*   @version 2.0.1
*
*   @brief   AUTOSAR Mcl - MCL driver source file.
*   @details
*
*   @addtogroup MCL_DRIVER MCL Driver
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
#include "CDD_Mcl.h"
#include "CDD_Mcl_Irq.h"

/*==================================================================================================
 *                                      SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CDD_MCL_VENDOR_ID_C                       43
#define CDD_MCL_AR_RELEASE_MAJOR_VERSION_C        4
#define CDD_MCL_AR_RELEASE_MINOR_VERSION_C        4
#define CDD_MCL_AR_RELEASE_REVISION_VERSION_C     0
#define CDD_MCL_SW_MAJOR_VERSION_C                2
#define CDD_MCL_SW_MINOR_VERSION_C                0
#define CDD_MCL_SW_PATCH_VERSION_C                1

/*==================================================================================================
 *                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if CDD_Mcl.c file and CDD_Mcl.h file are of the same vendor */
#if (CDD_MCL_VENDOR_ID_C != CDD_MCL_VENDOR_ID)
    #error "CDD_Mcl.c and CDD_Mcl.h have different vendor ids"
#endif

/* Check if CDD_Mcl.c file and CDD_Mcl.h file are of the same Autosar version */
#if ((CDD_MCL_AR_RELEASE_MAJOR_VERSION_C != CDD_MCL_AR_RELEASE_MAJOR_VERSION) || \
     (CDD_MCL_AR_RELEASE_MINOR_VERSION_C != CDD_MCL_AR_RELEASE_MINOR_VERSION) || \
     (CDD_MCL_AR_RELEASE_REVISION_VERSION_C != CDD_MCL_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of CDD_Mcl.c and CDD_Mcl.h are different"
#endif

/* Check if CDD_Mcl.c file and CDD_Mcl.h file are of the same Software version */
#if ((CDD_MCL_SW_MAJOR_VERSION_C != CDD_MCL_SW_MAJOR_VERSION) || \
     (CDD_MCL_SW_MINOR_VERSION_C != CDD_MCL_SW_MINOR_VERSION) || \
     (CDD_MCL_SW_PATCH_VERSION_C != CDD_MCL_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Mcl.c and CDD_Mcl.h are different"
#endif

/* Check if CDD_Mcl.c file and CDD_Mcl_Irq.h file are of the same vendor */
#if (CDD_MCL_VENDOR_ID_C != CDD_MCL_IRQ_VENDOR_ID)
    #error "CDD_Mcl.c and CDD_Mcl_Irq.h have different vendor ids"
#endif

/* Check if CDD_Mcl.c file and CDD_Mcl_Irq.h file are of the same Autosar version */
#if ((CDD_MCL_AR_RELEASE_MAJOR_VERSION_C != CDD_MCL_IRQ_AR_RELEASE_MAJOR_VERSION) || \
     (CDD_MCL_AR_RELEASE_MINOR_VERSION_C != CDD_MCL_IRQ_AR_RELEASE_MINOR_VERSION) || \
     (CDD_MCL_AR_RELEASE_REVISION_VERSION_C != CDD_MCL_IRQ_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of CDD_Mcl.c and CDD_Mcl_Irq.h are different"
#endif

/* Check if CDD_Mcl.c file and CDD_Mcl_Irq.h file are of the same Software version */
#if ((CDD_MCL_SW_MAJOR_VERSION_C != CDD_MCL_IRQ_SW_MAJOR_VERSION) || \
     (CDD_MCL_SW_MINOR_VERSION_C != CDD_MCL_IRQ_SW_MINOR_VERSION) || \
     (CDD_MCL_SW_PATCH_VERSION_C != CDD_MCL_IRQ_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Mcl.c and CDD_Mcl_Irq.h are different"
#endif

/*==================================================================================================
 *                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
 *                                        LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
 *                                        LOCAL VARIABLES
==================================================================================================*/
#define MCL_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Mcl_MemMap.h"

static const Mcl_ConfigType * Mcl_pxConfig;

#define MCL_STOP_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Mcl_MemMap.h"


/*==================================================================================================
 *                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
 *                                        GLOBAL VARIABLES
==================================================================================================*/
#if (STD_ON == MCL_EMIOS_IS_AVAILABLE)
#define MCL_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Mcl_MemMap.h"

Mcl_EmiosInitConfigType * Mcl_pxEmiosCommonConfig;

#define MCL_STOP_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Mcl_MemMap.h"

#define MCL_START_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE
#include "Mcl_MemMap.h"

uint8 Mcl_au8EmiosLogicToHwInstance[eMIOS_INSTANCE_COUNT];

#define MCL_STOP_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE
#include "Mcl_MemMap.h"
#endif /* EMIOS_IS_AVAILABLE */

#if (STD_ON == MCL_FLEXIO_IS_AVAILABLE)
#define MCL_START_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE
#include "Mcl_MemMap.h"

uint8 Mcl_au8FlexioLogicToHwInstance[FLEXIO_INSTANCE_COUNT];

#define MCL_STOP_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE
#include "Mcl_MemMap.h"
#endif /* FLEXIO_IS_AVAILABLE */

/*==================================================================================================
 *                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#if (STD_ON == MCL_EMIOS_IS_AVAILABLE)
#define MCL_START_SEC_CODE
#include "Mcl_MemMap.h"

static inline void Mcl_Emios_Init(const Mcl_EmiosInitConfigType * const pConfig);
static inline void Mcl_Emios_Deinit(void);

#if (STD_ON == MCL_DET_IS_AVAILABLE)
static inline Std_ReturnType Mcl_Emios_ValidateLogicChannel(uint16 logicChannel);
#endif

#define MCL_STOP_SEC_CODE
#include "Mcl_MemMap.h"
#endif /* STD_ON == MCL_EMIOS_IS_AVAILABLE */

#define MCL_START_SEC_CODE
#include "Mcl_MemMap.h"

#if (STD_ON == MCL_DET_IS_AVAILABLE)
static Mcl_ReturnType Mcl_ValidatePtrInit(const Mcl_ConfigType * const ConfigPtr);
#endif /* STD_ON == MCL_DET_IS_AVAILABLE */

#if (STD_ON == MCL_DMA_IS_AVAILABLE)
static void Mcl_DmaReturnCheck(const uint8 Channel, const uint8 Function, const Dma_Ip_ReturnType RetStatus);
#endif /* #if (STD_ON == MCL_DMA_IS_AVAILABLE) */

#define MCL_STOP_SEC_CODE
#include "Mcl_MemMap.h"

/*==================================================================================================
 *                                        LOCAL FUNCTIONS
==================================================================================================*/
#define MCL_START_SEC_CODE
#include "Mcl_MemMap.h"

#if (STD_ON == MCL_DET_IS_AVAILABLE)
static Mcl_ReturnType Mcl_ValidatePtrInit(const Mcl_ConfigType * const ConfigPtr)
{
    Mcl_ReturnType Valid = MCL_STATUS_SUCCESS;

#if (MCL_PRECOMPILE_SUPPORT == STD_OFF)
    if (NULL_PTR == ConfigPtr)
    {
        Valid = MCL_STATUS_ERROR;
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_INIT, MCL_E_INIT_FAILED);
    }
    else
    {
        /** do nothing */
    }
#elif (STD_ON == MCL_PRECOMPILE_SUPPORT)
    if (NULL_PTR != ConfigPtr)
    {
        Valid = MCL_STATUS_ERROR;
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_INIT, MCL_E_INIT_FAILED);
    }
    else
    {
        /** do nothing */
    }
#endif /* MCL_PRECOMPILE_SUPPORT */

    return Valid;
}
#endif /* STD_ON == MCL_DET_IS_AVAILABLE */

#if (STD_ON == MCL_DMA_IS_AVAILABLE)
/* Check returned status of the IP-Layer DMA function */
static void Mcl_DmaReturnCheck(const uint8 Channel, const uint8 Function, const Dma_Ip_ReturnType RetStatus)
{
    switch(RetStatus)
    {
    #if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        case DMA_IP_STATUS_WRONG_CORE:
            #if (STD_ON == MCL_DET_IS_AVAILABLE)
                (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, Function, MCL_E_PARAM_CONFIG);
            #endif /* MCL_DET_IS_AVAILABLE == STD_ON */
        break;
    #endif /* (STD_ON == MCL_MULTICORE_IS_AVAILABLE) */

#if (STD_ON == MCL_DET_IS_AVAILABLE)
        case DMA_IP_STATUS_ERROR:
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, Function, MCL_E_INCONSISTENCY);
        break;
        case DMA_IP_STATUS_WRONG_STATE:
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, Function, MCL_E_INVALID_STATE);
        break;
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
        case DMA_IP_STATUS_SUCCESS:
            (void)Channel;
            (void)Function;
        break;
        default:
            /* Do nothing... */
        break;
    }
}
#endif /* #if (STD_ON == MCL_DMA_IS_AVAILABLE) */

#define MCL_STOP_SEC_CODE
#include "Mcl_MemMap.h"

/*==================================================================================================
 *                                        GLOBAL FUNCTIONS
==================================================================================================*/
#define MCL_START_SEC_CODE
#include "Mcl_MemMap.h"

void Mcl_Init(const Mcl_ConfigType * const ConfigPtr)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    #if (STD_ON == MCL_DMA_IS_AVAILABLE)
    Dma_Ip_ReturnType DmaIpStatus;
    #endif
    #if (STD_ON == MCL_TRGMUX_IS_AVAILABLE)
    Trgmux_Ip_StatusType TrgmuxIpStatus;
    #endif
    #if (STD_ON == MCL_LCU_IS_AVAILABLE)
    Lcu_Ip_ReturnType LcuIpStatus;
    #endif
    Mcl_ReturnType ValidPtr;

    ValidPtr = Mcl_ValidatePtrInit(ConfigPtr);
    if(MCL_STATUS_SUCCESS == ValidPtr)
    {
#endif /* STD_ON == MCL_DET_IS_AVAILABLE */
        if(NULL_PTR == Mcl_pxConfig)
        {
            #if (STD_ON == MCL_PRECOMPILE_SUPPORT)
                Mcl_pxConfig = &Mcl_Config;
                (void)ConfigPtr;
            #else
                Mcl_pxConfig = ConfigPtr;
            #endif /* STD_ON == MCL_PRECOMPILE_SUPPORT */
                if(NULL_PTR != Mcl_pxConfig)
                {
                #if (STD_ON == MCL_DMA_IS_AVAILABLE)
                    if(NULL_PTR != Mcl_pxConfig->pxDmaInitType)
                    {
                    #if (STD_ON == MCL_DET_IS_AVAILABLE)
                        DmaIpStatus = Dma_Ip_Init(Mcl_pxConfig->pxDmaInitType);
                        if(DMA_IP_STATUS_SUCCESS != DmaIpStatus)
                        {
                            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)0U, MCL_DET_INIT, MCL_E_UNINIT);
                        }
                    #else
                        (void)Dma_Ip_Init(Mcl_pxConfig->pxDmaInitType);
                    #endif
                    }
                #endif /* #if (STD_ON == MCL_DMA_IS_AVAILABLE) */
                #if (STD_ON == MCL_TRGMUX_IS_AVAILABLE)
                    if(NULL_PTR != Mcl_pxConfig->pxTrgmuxInitType)
                    {
                    #if (STD_ON == MCL_DET_IS_AVAILABLE)
                        TrgmuxIpStatus = Trgmux_Ip_Init(Mcl_pxConfig->pxTrgmuxInitType);
                        if(TRGMUX_IP_STATUS_SUCCESS != TrgmuxIpStatus)
                        {
                            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)0U, MCL_DET_INIT, MCL_E_UNINIT);
                        }
                    #else
                        (void)Trgmux_Ip_Init(Mcl_pxConfig->pxTrgmuxInitType);
                    #endif
                    }
                #endif /* MCL_TRGMUX_IS_AVAILABLE */
                #if(STD_ON == MCL_EMIOS_IS_AVAILABLE)
                    if(NULL_PTR != Mcl_pxConfig->pEmiosInitType)
                    {
                        (void)Mcl_Emios_Init(Mcl_pxConfig->pEmiosInitType);
                    }
                #endif /* MCL_EMIOS_IS_AVAILABLE */
                #if(STD_ON == MCL_FLEXIO_IS_AVAILABLE)
                    if(NULL_PTR != Mcl_pxConfig->pFlexioInitType)
                    {
                        (void)Flexio_Mcl_Ip_InitDevice(Mcl_pxConfig->pFlexioInitType);
                    }
                #endif /* MCL_FLEXIO_IS_AVAILABLE */
                #if(STD_ON == MCL_LCU_IS_AVAILABLE)
                    if(NULL_PTR != Mcl_pxConfig->pxLcuInitType)
                    {
                    #if (STD_ON == MCL_DET_IS_AVAILABLE)
                        LcuIpStatus = Mcl_Ipw_Lcu_Init(Mcl_pxConfig->pxLcuInitType);
                        if (LCU_IP_STATUS_SUCCESS != LcuIpStatus)
                        {
                            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)0U, MCL_DET_INIT, MCL_E_UNINIT);
                        }
                    #else /* MCL_DET_IS_AVAILABLE */
                        (void)Mcl_Ipw_Lcu_Init(Mcl_pxConfig->pxLcuInitType);
                    #endif
                    }
                #endif /* MCL_LCU_IS_AVAILABLE */
                }
            }
        else
        {
#if (STD_ON == MCL_DET_IS_AVAILABLE)
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)0U, MCL_DET_INIT, MCL_E_UNINIT);
#else /* STD_ON == MCL_DET_IS_AVAILABLE */
            /* Do Nothing */
#endif /* STD_ON == MCL_DET_IS_AVAILABLE */
        }

#if (STD_ON == MCL_DET_IS_AVAILABLE)
    }
#endif /* STD_ON == MCL_DET_IS_AVAILABLE */
}


void Mcl_DeInit(void)
{
#if (STD_ON == MCL_DMA_IS_AVAILABLE)
    Dma_Ip_ReturnType RetStatus;
#endif /* #if (STD_ON == MCL_DMA_IS_AVAILABLE) */
#if (STD_ON == MCL_LCU_IS_AVAILABLE)
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    Lcu_Ip_ReturnType LcuRetStatus;
#endif /* STD_ON == MCL_DET_IS_AVAILABLE */
#endif /* #if (STD_ON == MCL_LCU_IS_AVAILABLE) */

    if(NULL_PTR != Mcl_pxConfig)
    {
#if (STD_ON == MCL_DMA_IS_AVAILABLE)
        if(NULL_PTR != Mcl_pxConfig->pxDmaInitType)
        {
            RetStatus = Dma_Ip_Deinit();
            Mcl_DmaReturnCheck(0U, MCL_DET_DEINIT, RetStatus);
        }
#endif /* #if (STD_ON == MCL_DMA_IS_AVAILABLE) */

#if (STD_ON == MCL_LCU_IS_AVAILABLE)
        if(NULL_PTR != Mcl_pxConfig->pxLcuInitType)
        {
#if (STD_ON == MCL_DET_IS_AVAILABLE)
            LcuRetStatus = Mcl_Ipw_Lcu_Deinit();
            if (LCU_IP_STATUS_SUCCESS != LcuRetStatus)
                {
                    (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)0U, MCL_DET_DEINIT, MCL_E_PARAM_CONFIG);
                }
#else /* MCL_DET_IS_AVAILABLE */
            (void)Mcl_Ipw_Lcu_Deinit();
#endif /* MCL_DET_IS_AVAILABLE */
        }
#endif /* #if (STD_ON == MCL_LCU_IS_AVAILABLE) */

#if(STD_ON == MCL_EMIOS_IS_AVAILABLE)
        if(NULL_PTR != Mcl_pxConfig->pEmiosInitType)
        {
            Mcl_Emios_Deinit();
        }
#endif /* MCL_EMIOS_IS_AVAILABLE */

#if(STD_ON == MCL_FLEXIO_IS_AVAILABLE)
        if(NULL_PTR != Mcl_pxConfig->pFlexioInitType)
        {
            Flexio_Mcl_Ip_DeinitDevice(Mcl_pxConfig->pFlexioInitType->instance);
        }
#endif /* MCL_FLEXIO_IS_AVAILABLE */

        Mcl_pxConfig = NULL_PTR;
    }
    else
    {
#if (STD_ON == MCL_DET_IS_AVAILABLE)
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)0U, MCL_DET_DEINIT, MCL_E_UNINIT);
#else /* STD_ON == MCL_DET_IS_AVAILABLE */
        /* Do Nothing */
#endif /* STD_ON == MCL_DET_IS_AVAILABLE */
    }
}


#if (STD_ON == MCL_DMA_IS_AVAILABLE)
void Mcl_SetDmaInstanceCommand(const uint32 Instance, const Mcl_DmaInstanceCmdType Command)
{
    Dma_Ip_ReturnType RetStatus;
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    if(DMA_IP_NOF_CFG_LOGIC_INSTANCES < Instance)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Instance, MCL_DET_DMA_INSTANCE_COMMAND, MCL_E_INVALID_INSTANCE);
    }
    else
    {
        if(MCL_DMA_INST_RESUME < Command)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Instance, MCL_DET_DMA_INSTANCE_COMMAND, MCL_E_INVALID_COMMAND);
        }
        else
        {
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
            RetStatus = Dma_Ip_SetLogicInstanceCommand(Instance, (Dma_Ip_LogicInstanceCmdType)Command);
            Mcl_DmaReturnCheck((uint8)Instance, MCL_DET_DMA_INSTANCE_COMMAND, RetStatus);
#if (STD_ON == MCL_DET_IS_AVAILABLE)
        }
    }
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
}

void Mcl_GetDmaInstanceStatus(const uint32 Instance, Mcl_DmaInstanceStatusType * const Status)
{
    Dma_Ip_ReturnType RetStatus;
    Dma_Ip_LogicInstanceStatusType LocInstStatus = {0U, 0U, FALSE};
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    if(DMA_IP_NOF_CFG_LOGIC_INSTANCES < Instance)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Instance, MCL_DET_DMA_INSTANCE_STATUS, MCL_E_INVALID_INSTANCE);
    }
    else
    {
        if (NULL_PTR == Status)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Instance, MCL_DET_DMA_INSTANCE_STATUS, MCL_E_PARAM_POINTER);
        }
        else
        {
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
            RetStatus = Dma_Ip_GetLogicInstanceStatus(Instance, &LocInstStatus);
            Status->Errors   = LocInstStatus.Errors;
            Status->ActiveId = LocInstStatus.ActiveId;
            Status->Active   = LocInstStatus.Active;
            Mcl_DmaReturnCheck((uint8)Instance, MCL_DET_DMA_INSTANCE_STATUS, RetStatus);
#if (STD_ON == MCL_DET_IS_AVAILABLE)
        }
    }
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
}

void Mcl_SetDmaChannelCommand(const uint32 Channel, const Mcl_DmaChannelCmdType Command)
{
    Dma_Ip_ReturnType RetStatus;
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    if(DMA_IP_NOF_CFG_LOGIC_CHANNELS < Channel)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, MCL_DET_DMA_CHANNEL_COMMAND, MCL_E_INVALID_CHANNEL);
    }
    else
    {
        if(MCL_DMA_CH_ACK_ERROR < Command)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, MCL_DET_DMA_CHANNEL_COMMAND, MCL_E_INVALID_COMMAND);
        }
        else
        {
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
            RetStatus = Dma_Ip_SetLogicChannelCommand(Channel, (Dma_Ip_LogicChannelCmdType)Command);
            Mcl_DmaReturnCheck((uint8)Channel, MCL_DET_DMA_CHANNEL_COMMAND, RetStatus);
#if (STD_ON == MCL_DET_IS_AVAILABLE)
        }
    }
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
}

void Mcl_GetDmaChannelStatus(const uint32 Channel, Mcl_DmaChannelStatusType * const Status)
{
    Dma_Ip_ReturnType RetStatus;
    Dma_Ip_LogicChannelStatusType LocChStatus;
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    if(DMA_IP_NOF_CFG_LOGIC_CHANNELS < Channel)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, MCL_DET_DMA_CHANNEL_STATUS, MCL_E_INVALID_CHANNEL);
    }
    else
    {
        if (NULL_PTR == Status)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, MCL_DET_DMA_CHANNEL_STATUS, MCL_E_PARAM_POINTER);
        }
        else
        {
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
            RetStatus = Dma_Ip_GetLogicChannelStatus(Channel, &LocChStatus);
            Status->ChannelState = (Mcl_DmaChannelStateType)LocChStatus.ChStateValue;
            Status->Errors = LocChStatus.Errors;
            Status->Active = LocChStatus.Active;
            Status->Done   = LocChStatus.Done;
            Mcl_DmaReturnCheck((uint8)Channel, MCL_DET_DMA_CHANNEL_STATUS, RetStatus);
#if (STD_ON == MCL_DET_IS_AVAILABLE)
        }
    }
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
}

void Mcl_SetDmaChannelGlobalList(const uint32 Channel, const Mcl_DmaChannelGlobalListType List[], const uint32 ListDimension)
{
    Dma_Ip_ReturnType RetStatus;
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Idx;
    boolean DetErrorCheck = FALSE;

    if(DMA_IP_NOF_CFG_LOGIC_CHANNELS < Channel)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, MCL_DET_DMA_GLOBAL, MCL_E_INVALID_CHANNEL);
        DetErrorCheck = TRUE;
    }
    if(((uint32)MCL_DMA_CH_SET_DIS_PREEMPT_PRIORITY + 1U) < ListDimension)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, MCL_DET_DMA_GLOBAL, MCL_E_INVALID_PARAMETER);
        DetErrorCheck = TRUE;
    }
    for(Idx = 0; Idx < ListDimension; Idx++)
    {
        if(MCL_DMA_CH_SET_DIS_PREEMPT_PRIORITY < List[Idx].Param)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, MCL_DET_DMA_GLOBAL, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }
    if(FALSE == DetErrorCheck)
    {
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
        RetStatus = Dma_Ip_SetLogicChannelGlobalList(Channel, (const Dma_Ip_LogicChannelGlobalListType*)List, ListDimension);
        Mcl_DmaReturnCheck((uint8)Channel, MCL_DET_DMA_GLOBAL, RetStatus);
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    }
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
}

void Mcl_SetDmaChannelTransferList(const uint32 Channel, const Mcl_DmaChannelTransferListType List[], const uint32 ListDimension)
{
    Dma_Ip_ReturnType RetStatus;
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Idx;
    boolean DetErrorCheck = FALSE;

    if(DMA_IP_NOF_CFG_LOGIC_CHANNELS < Channel)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, MCL_DET_DMA_TRANSFER, MCL_E_INVALID_CHANNEL);
        DetErrorCheck = TRUE;
    }
    if(((uint32)MCL_DMA_CH_SET_CONTROL_BANDWIDTH + 1U) < ListDimension)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, MCL_DET_DMA_TRANSFER, MCL_E_INVALID_PARAMETER);
        DetErrorCheck = TRUE;
    }
    for(Idx = 0; Idx < ListDimension; Idx++)
    {
        if(MCL_DMA_CH_SET_CONTROL_BANDWIDTH < List[Idx].Param)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, MCL_DET_DMA_TRANSFER, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }
    if(FALSE == DetErrorCheck)
    {
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
        RetStatus = Dma_Ip_SetLogicChannelTransferList(Channel, (const Dma_Ip_LogicChannelTransferListType*)List, ListDimension);
        Mcl_DmaReturnCheck((uint8)Channel, MCL_DET_DMA_TRANSFER, RetStatus);
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    }
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
}

void Mcl_SetDmaChannelScatterGatherList(const uint32 Channel, const uint32 Element, const Mcl_DmaChannelScatterGatherListType List[], const uint32 ListDimension)
{
    Dma_Ip_ReturnType RetStatus;
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Idx;
    boolean DetErrorCheck = FALSE;

    if(DMA_IP_NOF_CFG_LOGIC_CHANNELS < Channel)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, MCL_DET_DMA_SCATTER_GATHER_LIST, MCL_E_INVALID_CHANNEL);
        DetErrorCheck = TRUE;
    }
    if(((uint32)MCL_DMA_CH_SET_CONTROL_BANDWIDTH + 1U) < ListDimension)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, MCL_DET_DMA_SCATTER_GATHER_LIST, MCL_E_INVALID_PARAMETER);
        DetErrorCheck = TRUE;
    }
    for(Idx = 0; Idx < ListDimension; Idx++)
    {
        if(MCL_DMA_CH_SET_CONTROL_BANDWIDTH < List[Idx].Param)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, MCL_DET_DMA_SCATTER_GATHER_LIST, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }
    if(FALSE == DetErrorCheck)
    {
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
        RetStatus = Dma_Ip_SetLogicChannelScatterGatherList(Channel, Element, (const Dma_Ip_LogicChannelScatterGatherListType*)List, ListDimension);
        Mcl_DmaReturnCheck((uint8)Channel, MCL_DET_DMA_SCATTER_GATHER_LIST, RetStatus);
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    }
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
}

void Mcl_GetDmaChannelParam(const uint32 Channel, const Mcl_DmaChannelInfoParamType Param, uint32 * const Value)
{
    Dma_Ip_ReturnType RetStatus;
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    if(DMA_IP_NOF_CFG_LOGIC_CHANNELS < Channel)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, MCL_DET_DMA_INFORMATION, MCL_E_INVALID_CHANNEL);
    }
    else
    {
        if(MCL_DMA_CH_GET_HALF_MAJOR_INTERRUPT < Param)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, MCL_DET_DMA_INFORMATION, MCL_E_INVALID_PARAMETER);
        }
        else
        {
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
            RetStatus = Dma_Ip_GetLogicChannelParam(Channel, (Dma_Ip_LogicChannelInfoParamType)Param, Value);
            Mcl_DmaReturnCheck((uint8)Channel, MCL_DET_DMA_INFORMATION, RetStatus);
#if (STD_ON == MCL_DET_IS_AVAILABLE)
        }
    }
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
}

void Mcl_SetDmaChannelScatterGatherConfig(const uint32 Channel, const uint32 Element)
{
    Dma_Ip_ReturnType RetStatus;
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    if(DMA_IP_NOF_CFG_LOGIC_CHANNELS < Channel)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)Channel, MCL_DET_DMA_SCATTER_GATHER_CONFIG, MCL_E_INVALID_CHANNEL);
    }
    else
    {
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
        RetStatus = Dma_Ip_SetLogicChannelScatterGatherConfig(Channel, Element);
        Mcl_DmaReturnCheck((uint8)Channel, MCL_DET_DMA_SCATTER_GATHER_CONFIG, RetStatus);
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    }
#endif /* #if (STD_ON == MCL_DET_IS_AVAILABLE) */
}


#endif /* #if (STD_ON == MCL_DMA_IS_AVAILABLE) */

#if (STD_ON == MCL_TRGMUX_IS_AVAILABLE)

void Mcl_SetTrgMuxInput(const uint32 Trigger, const uint32 Input)
{
#if ((STD_ON == MCL_MULTICORE_IS_AVAILABLE) && (STD_ON == MCL_DET_IS_AVAILABLE))

    Trgmux_Ip_StatusType LocStatus = Trgmux_Ip_SetInput(Trigger, Input);

    if (TRGMUX_IP_STATUS_WRONG_CORE == LocStatus)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_TRGMUX_SET_INPUT, MCL_E_PARAM_CONFIG);
    }
    else
    {
        /* Do Nothing */
    }

#else

    (void)Trgmux_Ip_SetInput(Trigger, Input);

#endif /* MCL_DET_IS_AVAILABLE and MCL_MULTICORE_IS_AVAILABLE */

}

void Mcl_SetTrgMuxLock(const uint32 Trigger)
{
#if ((STD_ON == MCL_MULTICORE_IS_AVAILABLE) && (STD_ON == MCL_DET_IS_AVAILABLE))

    /* Set Lock */
    Trgmux_Ip_StatusType LocStatus = Trgmux_Ip_SetLock(Trigger);

    if (TRGMUX_IP_STATUS_WRONG_CORE == LocStatus)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_TRGMUX_SET_LOC, MCL_E_PARAM_CONFIG);
    }
    else
    {
        /* Do Nothing */
    }

#else

    /* Set Lock */
    (void)Trgmux_Ip_SetLock(Trigger);

#endif /* MCL_DET_IS_AVAILABLE and MCL_MULTICORE_IS_AVAILABLE */
}


#endif /* MCL_TRGMUX_IS_AVAILABLE */

#if (STD_ON == MCL_EMIOS_IS_AVAILABLE)

static inline void Mcl_Emios_Init(const Mcl_EmiosInitConfigType *const pConfig)
{
    uint8 CurrentInstance;

    for (CurrentInstance = 0; CurrentInstance < pConfig->MclEmiosNumInstances; CurrentInstance++)
    {
        Mcl_au8EmiosLogicToHwInstance[CurrentInstance] = (*pConfig->pMclEmiosInstanceConfig)[CurrentInstance].instance;
        /* Init Emios */
        (void)Emios_Mcl_Ip_Init((*pConfig->pMclEmiosInstanceConfig)[CurrentInstance].instance,
                          (*pConfig->pMclEmiosInstanceConfig)[CurrentInstance].emiosInstanceConfig);
    }
}

static inline void Mcl_Emios_Deinit(void)
{
    uint8 CurrentInstance;

    for (CurrentInstance = 0; CurrentInstance < eMIOS_INSTANCE_COUNT; CurrentInstance++)
    {
        if (255U != Mcl_au8EmiosLogicToHwInstance[CurrentInstance])
        {
            /* De-init Emios */
            (void)Emios_Mcl_Ip_Deinit(Mcl_au8EmiosLogicToHwInstance[CurrentInstance]);
        }
    }
}

#if (STD_ON == MCL_DET_IS_AVAILABLE)
static inline Std_ReturnType Mcl_Emios_ValidateLogicChannel(uint16 logicChannel)
{
    uint8 HwChannel  = (uint8)(255U & logicChannel);
    uint8 HwInstance = (uint8)(logicChannel >> 8);
    Std_ReturnType RetStatus = (Std_ReturnType)E_NOT_OK;

    /* Validate channel */
    if (TRUE == Emios_Mcl_Ip_ValidateChannel(HwInstance, HwChannel))
    {
        RetStatus = (Std_ReturnType)E_OK;
    }
    else
    {
        /* Do nothing */
    }

    return RetStatus;
}
#endif

/** @implements Mcl_Emios_SetReloadInterval_Activity */
void Mcl_Emios_SetReloadInterval(uint16 logicChannel, uint8 interval)
{
    uint8 HwChannel  = (uint8)(255U & logicChannel);
    uint8 HwInstance = (uint8)(logicChannel >> 8);

#if (STD_ON == MCL_DET_IS_AVAILABLE)
    Std_ReturnType valid = Mcl_Emios_ValidateLogicChannel(logicChannel);

    if ((Std_ReturnType)E_NOT_OK == valid)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)logicChannel, MCL_DET_EMIOS_E_INVALID_CHANNEL, MCL_E_INVALID_CHANNEL);
    }
    else
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        if (TRUE == Emios_Mcl_Ip_ValidateMultiCoreInit(HwInstance))
        {
#endif

            (void)Emios_Mcl_Ip_SetReloadInterval(HwInstance, HwChannel, interval);

#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        }
        else
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_EMIOS_E_INVALID_CHANNEL, MCL_E_PARAM_CONFIG);
        }
#endif
    }
#else /* MCL_DET_IS_AVAILABLE == STDOFF */
    (void)Emios_Mcl_Ip_SetReloadInterval(HwInstance, HwChannel, interval);
#endif /* MCL_DET_IS_AVAILABLE */
}

/** @implements Mcl_Emios_SetCounterBusPeriod_Activity */
void Mcl_Emios_SetCounterBusPeriod(uint16 logicChannel, uint16 period, boolean syncUpdate)
{
    uint8  HwChannel   = (uint8)(255U & logicChannel);
    uint8  HwInstance  = (uint8)(logicChannel >> 8U);
    uint32 ChannelMask = (uint32)((uint32)1U << (uint32)HwChannel);

#if (STD_ON == MCL_DET_IS_AVAILABLE)
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    if (Emios_Mcl_Ip_ValidateMultiCoreInit(HwInstance))
    {
#endif /* STD_ON == MCL_MULTICORE_IS_AVAILABLE */
        Emios_Ip_CommonStatusType validSet;
        Std_ReturnType valid = Mcl_Emios_ValidateLogicChannel(logicChannel);

        if ((Std_ReturnType)E_NOT_OK == valid)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)logicChannel, MCL_DET_EMIOS_E_INVALID_CHANNEL, MCL_E_INVALID_CHANNEL);
        }
        else
        {
            if (syncUpdate == TRUE)
            {
                Emios_Mcl_Ip_ComparatorTransferDisable(HwInstance, ChannelMask);
            }
            else
            {
                /* Do nothing. */
            }
            validSet = Emios_Mcl_Ip_SetCounterBusPeriod(HwInstance, HwChannel, period);
            if (EMIOS_IP_COMMON_STATUS_FAIL == validSet)
            {
                (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, (uint8)logicChannel, MCL_DET_EMIOS_E_INVALID_SET, MCL_E_INVALID_PARAMETER);
            }
        }

#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    }
    else
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_EMIOS_E_INVALID_CHANNEL, MCL_E_PARAM_CONFIG);
    }
#endif /* STD_ON == MCL_MULTICORE_IS_AVAILABLE */

#else
    if (syncUpdate == TRUE)
    {
        Emios_Mcl_Ip_ComparatorTransferDisable(HwInstance, ChannelMask);
    }
    else
    {
        /* Do nothing. */
    }
    (void)Emios_Mcl_Ip_SetCounterBusPeriod(HwInstance, HwChannel, period);
#endif /* MCL_DET_IS_AVAILABLE */

}

#endif /* STD_ON == MCL_EMIOS_IS_AVAILABLE */



#if (STD_ON == MCL_LCU_IS_AVAILABLE)

void Mcl_SetLcuWriteProtect(const uint8 LogicInstance)
{
    #if (STD_ON == MCL_DET_IS_AVAILABLE)
        Lcu_Ip_ReturnType ReturnValue = Mcl_Ipw_GetLcuWriteProtect(LogicInstance);
        if (LCU_IP_STATUS_PROTECTED == ReturnValue)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, LogicInstance, MCL_DET_LCU_SET_WRITE_PROTECT, MCL_E_PROTECTED);
        }
        else
        {
            (void)Mcl_Ipw_SetLcuWriteProtect(LogicInstance);
        }
    #else
        (void)Mcl_Ipw_SetLcuWriteProtect(LogicInstance);
    #endif /* MCL_DET_IS_AVAILABLE */
}

#if (STD_ON == MCL_LCU_SYNC_FUNC_IS_AVAILABLE)
void Mcl_SetLcuSyncInputSwOverrideEnable(const Mcl_LcuSyncInputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicInputId >= LCU_IP_NOF_CFG_LOGIC_INPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicInputId, MCL_DET_LCU_SYNC_SET_INPUT_SW_OVERRIDE_EN, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }

    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_SetLcuSyncInputSwOverrideEnable(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_INPUT_SW_OVERRIDE_EN, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_SetLcuSyncInputSwOverrideEnable(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */

    }
#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_SetLcuSyncInputSwOverrideEnable(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_SetLcuSyncInputSwOverrideValue(const Mcl_LcuSyncInputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicInputId >= LCU_IP_NOF_CFG_LOGIC_INPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicInputId, MCL_DET_LCU_SYNC_SET_INPUT_SW_OVERRIDE_VALUE, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }

    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_SetLcuSyncInputSwOverrideValue(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_INPUT_SW_OVERRIDE_VALUE, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */
        (void)Mcl_Ipw_SetLcuSyncInputSwOverrideValue(List, Dimension);
#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }
#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_SetLcuSyncInputSwOverrideValue(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_SetLcuSyncInputMuxSelect(const Mcl_LcuSyncInputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;

#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicInputId >= LCU_IP_NOF_CFG_LOGIC_INPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicInputId, MCL_DET_LCU_SYNC_SET_INPUT_MUX_SEL, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }

    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_SetLcuSyncInputMuxSelect(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_INPUT_MUX_SEL, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_SetLcuSyncInputMuxSelect(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_SetLcuSyncInputMuxSelect(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_SetLcuSyncInputSwSyncMode(const Mcl_LcuSyncInputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicInputId >= LCU_IP_NOF_CFG_LOGIC_INPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicInputId, MCL_DET_LCU_SYNC_SET_INPUT_SW_SYNC_MODE, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }

    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_SetLcuSyncInputSwSyncMode(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_INPUT_SW_SYNC_MODE, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_SetLcuSyncInputSwSyncMode(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else
    (void)Mcl_Ipw_SetLcuSyncInputSwSyncMode(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_SetLcuSyncOutputDebugMode(const Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_SET_OUTPUT_DEBUG_MODE_EN, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }
    if (FALSE == DetErrorCheck)
    {
    #if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_SetLcuSyncOutputDebugMode(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_OUTPUT_DEBUG_MODE_EN, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_SetLcuSyncOutputDebugMode(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */

    }
#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_SetLcuSyncOutputDebugMode(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_SetLcuSyncOutputEnable(const Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_SET_OUTPUT_ENABLE, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }
    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_SetLcuSyncOutputEnable(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_OUTPUT_ENABLE, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_SetLcuSyncOutputEnable(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }
#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_SetLcuSyncOutputEnable(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_SetLcuSyncOutputForceInputSensitivity(const Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_SET_OUTPUT_FORCE_SENSITIVITY, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }
    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_SetLcuSyncOutputForceInputSensitivity(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_OUTPUT_FORCE_SENSITIVITY, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_SetLcuSyncOutputForceInputSensitivity(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else
    (void)Mcl_Ipw_SetLcuSyncOutputForceInputSensitivity(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_SetLcuSyncOutputForceClearingMode(const Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_SET_OUTPUT_FORCE_CLEAR_MODE, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }
    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_SetLcuSyncOutputForceClearingMode(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_OUTPUT_FORCE_CLEAR_MODE, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_SetLcuSyncOutputForceClearingMode(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_SetLcuSyncOutputForceClearingMode(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_SetLcuSyncOutputForceSyncSelect(const Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_SET_OUTPUT_FORCE_SYNC_SEL, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }
    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_SetLcuSyncOutputForceSyncSelect(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_OUTPUT_FORCE_SYNC_SEL, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_SetLcuSyncOutputForceSyncSelect(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else
    (void)Mcl_Ipw_SetLcuSyncOutputForceSyncSelect(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_SetLcuSyncOutputPolarity(const Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_SET_OUTPUT_POLARITY, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }

    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_SetLcuSyncOutputPolarity(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_OUTPUT_POLARITY, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_SetLcuSyncOutputPolarity(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_SetLcuSyncOutputPolarity(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_SetLcuSyncOutputForceDma(const Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_SET_OUTPUT_FORCE_DMA, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }
    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_SetLcuSyncOutputForceDma(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_OUTPUT_FORCE_DMA, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_SetLcuSyncOutputForceDma(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_SetLcuSyncOutputForceDma(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_SetLcuSyncOutputForceInt(const Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_SET_OUTPUT_FORCE_INT, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }

    if (FALSE == DetErrorCheck)
    {
        LocStatus = Mcl_Ipw_SetLcuSyncOutputForceInt(List, Dimension);
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        if (LCU_IP_STATUS_WRONG_CORE == LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_OUTPUT_FORCE_INT, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */
        if (LCU_IP_STATUS_ERROR == LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_OUTPUT_FORCE_INT, MCL_E_INVALID_STATE);
        }
        else
        {
            /* Do Nothing */
        }
#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_SetLcuSyncOutputForceInt(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}
void Mcl_SetLcuSyncOutputLutDma(const Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_SET_OUTPUT_LUT_DMA, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }
    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_SetLcuSyncOutputLutDma(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_OUTPUT_LUT_DMA, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_SetLcuSyncOutputLutDma(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_SetLcuSyncOutputLutDma(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_SetLcuSyncOutputLutInt(const Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_SET_OUTPUT_LUT_INT, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }
    if (FALSE == DetErrorCheck)
    {
        LocStatus = Mcl_Ipw_SetLcuSyncOutputLutInt(List, Dimension);
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        if (LCU_IP_STATUS_WRONG_CORE == LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_OUTPUT_LUT_INT, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */
        if (LCU_IP_STATUS_ERROR == LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_OUTPUT_LUT_INT, MCL_E_INVALID_STATE);
        }
        else
        {
            /* Do Nothing */
        }
#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else /* MCL_DET_IS_AVAILABLE */
   (void)Mcl_Ipw_SetLcuSyncOutputLutInt(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_SetLcuSyncOutputFallFilter(const Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_SET_OUTPUT_FALL_FILTER, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }
    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_SetLcuSyncOutputFallFilter(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_OUTPUT_FALL_FILTER, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_SetLcuSyncOutputFallFilter(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_SetLcuSyncOutputFallFilter(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */

}

void Mcl_SetLcuSyncOutputRiseFilter(const Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_SET_OUTPUT_RISE_FILTER, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }
    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_SetLcuSyncOutputRiseFilter(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_OUTPUT_RISE_FILTER, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_SetLcuSyncOutputRiseFilter(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else
    (void)Mcl_Ipw_SetLcuSyncOutputRiseFilter(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_SetLcuSyncOutputLutControl(const Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_SET_OUTPUT_LUT_CONTROL, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }
    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_SetLcuSyncOutputLutControl(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_SET_OUTPUT_LUT_CONTROL, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_SetLcuSyncOutputLutControl(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_SetLcuSyncOutputLutControl(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_ClearLcuSyncOutputForceEvent(const Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_CLEAR_OUTPUT_FORCE_EVENT, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }

    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_ClearLcuSyncOutputForceEvent(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_CLEAR_OUTPUT_FORCE_EVENT, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_ClearLcuSyncOutputForceEvent(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_ClearLcuSyncOutputForceEvent(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_GetLcuSyncLogicInput(Mcl_LcuSyncInputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicInputId >= LCU_IP_NOF_CFG_LOGIC_INPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicInputId, MCL_DET_LCU_SYNC_GET_LOGIC_INPUT, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }

    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_GetLcuSyncLogicInput(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_GET_LOGIC_INPUT, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_GetLcuSyncLogicInput(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_GetLcuSyncLogicInput(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_GetLcuSyncSwOverrideInput(Mcl_LcuSyncInputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicInputId >= LCU_IP_NOF_CFG_LOGIC_INPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicInputId, MCL_DET_LCU_SYNC_GET_SW_OVERRIDE_INPUT, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }

    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_GetLcuSyncSwOverrideInput(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_GET_SW_OVERRIDE_INPUT, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_GetLcuSyncSwOverrideInput(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_GetLcuSyncSwOverrideInput(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_GetLcuSyncLogicOutput(Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_GET_LOGIC_OUTPUT, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }

    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_GetLcuSyncLogicOutput(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_GET_LOGIC_OUTPUT, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_GetLcuSyncLogicOutput(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_GetLcuSyncLogicOutput(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_GetLcuSyncForceOutput(Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_GET_FORCE_OUTPUT, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }

    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_GetLcuSyncForceOutput(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_GET_FORCE_OUTPUT, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_GetLcuSyncForceOutput(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_GetLcuSyncForceOutput(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_GetLcuSyncForceStatus(Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_GET_FORCE_STATUS, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }

    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_GetLcuSyncForceStatus(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_GET_FORCE_STATUS, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_GetLcuSyncForceStatus(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_GetLcuSyncForceStatus(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_GetLcuSyncCombineForceInput(Mcl_LcuSyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)
    uint8 Index;
    boolean DetErrorCheck = FALSE;
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    for(Index = 0; Index < Dimension; Index++)
    {
        if(List[Index].LogicOutputId >= LCU_IP_NOF_CFG_LOGIC_OUTPUTS)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, List[Index].LogicOutputId, MCL_DET_LCU_SYNC_GET_COMBINE_FORCE_INPUT, MCL_E_INVALID_PARAMETER);
            DetErrorCheck = TRUE;
            break;
        }
    }

    if (FALSE == DetErrorCheck)
    {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        LocStatus = Mcl_Ipw_GetLcuSyncCombineForceInput(List, Dimension);
        if (LCU_IP_STATUS_SUCCESS != LocStatus)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, 0U, MCL_DET_LCU_SYNC_GET_COMBINE_FORCE_INPUT, MCL_E_PARAM_CONFIG);
        }
        else
        {
            /* Do Nothing */
        }
#else /* MCL_MULTICORE_IS_AVAILABLE */

        (void)Mcl_Ipw_GetLcuSyncCombineForceInput(List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
    }

#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_GetLcuSyncCombineForceInput(List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

#endif /* #if (STD_ON == MCL_LCU_SYNC_FUNC_IS_AVAILABLE) */

#if (STD_ON == MCL_LCU_ASYNC_FUNC_IS_AVAILABLE)
void Mcl_SetLcuAsyncInputList(const uint8 LogicInput, const Mcl_LcuAsyncInputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)

#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    if (LCU_IP_NOF_CFG_LOGIC_INPUTS <= LogicInput)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, LogicInput, MCL_DET_LCU_ASYNC_SET_INPUT, MCL_E_INVALID_PARAMETER);
    }
    else
    {
        if (((uint8)MCL_LCU_IP_INPUT_SET_SW_VALUE + 1U) < Dimension)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, LogicInput, MCL_DET_LCU_ASYNC_SET_INPUT, MCL_E_INVALID_PARAMETER);
        }
        else
        {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
            LocStatus = Mcl_Ipw_SetLcuAsyncInputList(LogicInput, List, Dimension);
            if (LCU_IP_STATUS_SUCCESS != LocStatus)
            {
                (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, LogicInput, MCL_DET_LCU_ASYNC_SET_INPUT, MCL_E_PARAM_CONFIG);
            }
            else
            {
                /* Do Nothing */
            }
#else /* MCL_MULTICORE_IS_AVAILABLE */

            (void)Mcl_Ipw_SetLcuAsyncInputList(LogicInput, List, Dimension);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
        }
    }
#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_SetLcuAsyncInputList(LogicInput, List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_SetLcuAsyncOutputList(const uint8 LogicOutput, const Mcl_LcuAsyncOutputValueType List[], const uint8 Dimension)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)

    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;

    if (LCU_IP_NOF_CFG_LOGIC_OUTPUTS <= LogicOutput)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, LogicOutput, MCL_DET_LCU_ASYNC_SET_OUTPUT, MCL_E_INVALID_PARAMETER);
    }
    else
    {
        if (((uint8)MCL_LCU_IP_OUTPUT_CLEAR_FORCE_STS + 1U) < Dimension)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, LogicOutput, MCL_DET_LCU_ASYNC_SET_OUTPUT, MCL_E_INVALID_PARAMETER);
        }
        else
        {
            LocStatus = Mcl_Ipw_SetLcuAsyncOutputList(LogicOutput, List, Dimension);
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
            if (LCU_IP_STATUS_WRONG_CORE == LocStatus)
            {
                (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, LogicOutput, MCL_DET_LCU_ASYNC_SET_OUTPUT, MCL_E_PARAM_CONFIG);
            }
            else
            {
                /* Do Nothing */
            }
#else /* MCL_MULTICORE_IS_AVAILABLE */

            if (LCU_IP_STATUS_ERROR == LocStatus)
            {
                (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, LogicOutput, MCL_DET_LCU_ASYNC_SET_OUTPUT, MCL_E_INVALID_STATE);
            }
            else
            {
                /* Do Nothing */
            }

#endif /* MCL_MULTICORE_IS_AVAILABLE */
        }
    }
#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_SetLcuAsyncOutputList(LogicOutput, (Lcu_Ip_AsyncOutputValueType*)List, Dimension);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_GetLcuAsyncLogicInputInfo(const uint8 LogicInput, const Mcl_LcuInputInfoParamType Param, uint8 * const Value)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)

#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
        Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;
#endif /* MCL_MULTICORE_IS_AVAILABLE */

    if (LCU_IP_NOF_CFG_LOGIC_INPUTS <= LogicInput)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, LogicInput, MCL_DET_LCU_ASYNC_GET_INPUT_INFO, MCL_E_INVALID_PARAMETER);
    }
    else
    {
        if (MCL_LCU_IP_INPUT_GET_SW_OVERRIDE_STATE < Param)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, LogicInput, MCL_DET_LCU_ASYNC_GET_INPUT_INFO, MCL_E_INVALID_PARAMETER);
        }
        else
        {
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
            LocStatus = Mcl_Ipw_GetLcuAsyncLogicInputInfo(LogicInput, (Lcu_Ip_LogicInputInfoParamType)Param, Value);
            if (LCU_IP_STATUS_SUCCESS != LocStatus)
            {
                (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, LogicInput, MCL_DET_LCU_ASYNC_GET_INPUT_INFO, MCL_E_PARAM_CONFIG);
            }
            else
            {
                /* Do Nothing */
            }
#else /* MCL_MULTICORE_IS_AVAILABLE */

            (void)Mcl_Ipw_GetLcuAsyncLogicInputInfo(LogicInput, (Lcu_Ip_LogicInputInfoParamType)Param, Value);

#endif /* MCL_MULTICORE_IS_AVAILABLE */
        }
    }
#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_GetLcuAsyncLogicInputInfo(LogicInput, (Lcu_Ip_LogicInputInfoParamType)Param, Value);
#endif /* MCL_DET_IS_AVAILABLE */
}

void Mcl_GetLcuAsyncLogicOutputInfo(const uint8 LogicOutput, const Mcl_LcuOutputInfoParamType Param, uint8 * const Value)
{
#if (STD_ON == MCL_DET_IS_AVAILABLE)

    Lcu_Ip_ReturnType LocStatus = LCU_IP_STATUS_SUCCESS;

    if (LCU_IP_NOF_CFG_LOGIC_OUTPUTS < LogicOutput)
    {
        (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, LogicOutput, MCL_DET_LCU_ASYNC_GET_OUTPUT_INFO, MCL_E_INVALID_PARAMETER);
    }
    else
    {
        if (MCL_LCU_IP_OUTPUT_GET_COMBINE_FORCE_INPUT < Param)
        {
            (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, LogicOutput, MCL_DET_LCU_ASYNC_GET_OUTPUT_INFO, MCL_E_INVALID_PARAMETER);
        }
        else
        {
            LocStatus = Mcl_Ipw_GetLcuAsyncLogicOutputInfo(LogicOutput, (Lcu_Ip_LogicOutputInfoParamType)Param, Value);
#if (STD_ON == MCL_MULTICORE_IS_AVAILABLE)
            if (LCU_IP_STATUS_WRONG_CORE == LocStatus)
            {
                (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, LogicOutput, MCL_DET_LCU_ASYNC_GET_OUTPUT_INFO, MCL_E_PARAM_CONFIG);
            }
            else
            {
                /* Do Nothing */
            }
#else /* MCL_MULTICORE_IS_AVAILABLE */
            if (LCU_IP_STATUS_ERROR == LocStatus)
            {
                (void)Det_ReportError((uint16)CDD_MCL_MODULE_ID, LogicOutput, MCL_DET_LCU_ASYNC_GET_OUTPUT_INFO, MCL_E_PARAM_CONFIG);
            }
            else
            {
                /* Do Nothing */
            }
#endif /* MCL_MULTICORE_IS_AVAILABLE */
        }
    }
#else /* MCL_DET_IS_AVAILABLE */
    (void)Mcl_Ipw_GetLcuAsyncLogicOutputInfo(LogicOutput, (Lcu_Ip_LogicOutputInfoParamType)Param, Value);
#endif /* MCL_DET_IS_AVAILABLE */
}

#endif /* #if (STD_ON == MCL_LCU_ASYNC_FUNC_IS_AVAILABLE) */

#endif /* STD_ON == MCL_LCU_IS_AVAILABLE */

#if (STD_ON == MCL_VERSION_INFO_API_IS_AVAILABLE)
void Mcl_GetVersionInfo(Std_VersionInfoType * const VersionInfo)
{
    /* Save the version information into VersionInfo pointer */
    (VersionInfo)->vendorID = (uint16)CDD_MCL_VENDOR_ID;
    (VersionInfo)->moduleID = (uint16)CDD_MCL_MODULE_ID;
    (VersionInfo)->sw_major_version = (uint8)CDD_MCL_SW_MAJOR_VERSION;
    (VersionInfo)->sw_minor_version = (uint8)CDD_MCL_SW_MINOR_VERSION;
    (VersionInfo)->sw_patch_version = (uint8)CDD_MCL_SW_PATCH_VERSION;
}
#endif /* STD_ON == MCL_VERSION_INFO_API_IS_AVAILABLE */

#define MCL_STOP_SEC_CODE
#include "Mcl_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

/*==================================================================================================
 *                                        END OF FILE
==================================================================================================*/

