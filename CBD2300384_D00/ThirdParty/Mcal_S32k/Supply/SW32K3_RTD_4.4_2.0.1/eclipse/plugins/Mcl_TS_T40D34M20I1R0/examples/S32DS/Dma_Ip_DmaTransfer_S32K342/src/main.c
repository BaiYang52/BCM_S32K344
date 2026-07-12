/*
*   (c) Copyright 2020 NXP
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be used strictly
*   in accordance with the applicable license terms.  By expressly accepting
*   such terms or by downloading, installing, activating and/or otherwise using
*   the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms.  If you do not agree to
*   be bound by the applicable license terms, then you may not retain,
*   install, activate or otherwise use the software.
*
*   This file contains sample code only. It is not part of the production code deliverables.
*/

/**
*   @file main.c
*
*   @addtogroup main_module main module documentation
*   @{
*/

/* Including necessary configuration files. */
#include "Mcal.h"

#include "Osif.h"

#include "Clock_Ip.h"

#include "IntCtrl_Ip.h"

#include "Dma_Ip.h"

#include "Cache_Ip.h"

#include <string.h>

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/
#define DMA_CH0_BUFFER_DIMENSION           ((uint32)24U)
#define DMA_CH0_CONFIG_LIST_DIMENSION      ((uint32)2U)

#define DMA_CH1_ELEMENT0_DIMENSION         ((uint32)16U)
#define DMA_CH1_ELEMENT1_DIMENSION         ((uint32)32U)
#define DMA_CH1_ELEMENT2_DIMENSION         ((uint32)64U)
#define DMA_CH1_CONFIG_LIST_DIMENSION      ((uint32)2U)

#define DMA_CH2_BUFFER_DIMENSION           ((uint32)24U)
#define DMA_CH2_CONFIG_LIST_DIMENSION      ((uint32)9U)
#define DMA_CH2_GLOBAL_LIST_DIMENSION      ((uint32)3U)

#define DMA_IRQ_NOT_TRIGGERED              ((uint32)0U)
#define DMA_IRQ_TRIGGERED_ONCE             ((uint32)1U)

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
/* DMA Logic Channel 0 Buffers */
__attribute__(( aligned(32) )) static uint8 DmaCh0_SourceBuffer[DMA_CH0_BUFFER_DIMENSION] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};
__attribute__(( aligned(32) )) static uint8 DmaCh0_DestinationBuffer[DMA_CH0_BUFFER_DIMENSION];
/* DMA Logic Channel 0 Address Configuration */
Dma_Ip_LogicChannelTransferListType DmaCh0_TransferList[DMA_CH0_CONFIG_LIST_DIMENSION] =
{
    {DMA_IP_CH_SET_SOURCE_ADDRESS, (uint32)&DmaCh0_SourceBuffer[0U]},
    {DMA_IP_CH_SET_DESTINATION_ADDRESS, (uint32)&DmaCh0_DestinationBuffer[0U]},
};

/* DMA Logic Channel 1 Buffers */
__attribute__(( aligned(32) )) static uint8 DmaCh1_SourceBuffer0[DMA_CH1_ELEMENT0_DIMENSION] = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
__attribute__(( aligned(32) )) static uint8 DmaCh1_DestinationBuffer0[DMA_CH1_ELEMENT0_DIMENSION];
__attribute__(( aligned(32) )) static uint8 DmaCh1_SourceBuffer1[DMA_CH1_ELEMENT1_DIMENSION] = {0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7};
__attribute__(( aligned(32) )) static uint8 DmaCh1_DestinationBuffer1[DMA_CH1_ELEMENT1_DIMENSION];
__attribute__(( aligned(32) )) static uint8 DmaCh1_SourceBuffer2[DMA_CH1_ELEMENT2_DIMENSION] = {0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,100,100,100,100,99,99,99,99,98,98,98,98,97,97,97,97,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,96,96,96,96,95,95,95,95,94,94,94,94,93,93,93,93};
__attribute__(( aligned(32) )) static uint8 DmaCh1_DestinationBuffer2[DMA_CH1_ELEMENT2_DIMENSION];
/* DMA Logic Channel 1 Address Configuration */
Dma_Ip_LogicChannelScatterGatherListType DmaCh1_Element0_List[DMA_CH1_CONFIG_LIST_DIMENSION] =
{
    {DMA_IP_CH_SET_SOURCE_ADDRESS, (uint32)&DmaCh1_SourceBuffer0[0U]},
    {DMA_IP_CH_SET_DESTINATION_ADDRESS, (uint32)&DmaCh1_DestinationBuffer0[0U]},
};
Dma_Ip_LogicChannelScatterGatherListType DmaCh1_Element1_List[DMA_CH1_CONFIG_LIST_DIMENSION] =
{
    {DMA_IP_CH_SET_SOURCE_ADDRESS, (uint32)&DmaCh1_SourceBuffer1[0U]},
    {DMA_IP_CH_SET_DESTINATION_ADDRESS, (uint32)&DmaCh1_DestinationBuffer1[0U]},
};
Dma_Ip_LogicChannelScatterGatherListType DmaCh1_Element2_List[DMA_CH1_CONFIG_LIST_DIMENSION] =
{
    {DMA_IP_CH_SET_SOURCE_ADDRESS, (uint32)&DmaCh1_SourceBuffer2[0U]},
    {DMA_IP_CH_SET_DESTINATION_ADDRESS, (uint32)&DmaCh1_DestinationBuffer2[0U]},
};

/* DMA Logic Channel 2 Buffers */
__attribute__(( aligned(32) )) static uint8 DmaCh2_SourceBuffer[DMA_CH2_BUFFER_DIMENSION] = {23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
__attribute__(( aligned(32) )) static uint8 DmaCh2_DestinationBuffer[DMA_CH2_BUFFER_DIMENSION];
/* DMA Logic Channel 0 Address Configuration */
Dma_Ip_LogicChannelTransferListType DmaCh2_TransferList[DMA_CH2_CONFIG_LIST_DIMENSION] =
{
    {DMA_IP_CH_SET_SOURCE_ADDRESS, (uint32)&DmaCh2_SourceBuffer[0U]},
    {DMA_IP_CH_SET_SOURCE_SIGNED_OFFSET, 2U},
    {DMA_IP_CH_SET_SOURCE_TRANSFER_SIZE, DMA_IP_TRANSFER_SIZE_2_BYTE},
    {DMA_IP_CH_SET_DESTINATION_ADDRESS, (uint32)&DmaCh2_DestinationBuffer[0U]},
    {DMA_IP_CH_SET_DESTINATION_SIGNED_OFFSET, 2U},
    {DMA_IP_CH_SET_DESTINATION_TRANSFER_SIZE, DMA_IP_TRANSFER_SIZE_2_BYTE},
    {DMA_IP_CH_SET_MINORLOOP_SIZE, 24U},
    {DMA_IP_CH_SET_MAJORLOOP_COUNT, 1U},
    {DMA_IP_CH_SET_CONTROL_EN_MAJOR_INTERRUPT, TRUE},
};
Dma_Ip_LogicChannelGlobalListType DmaCh2_GlobalList[DMA_CH2_GLOBAL_LIST_DIMENSION] =
{
#if (STD_ON == DMA_IP_DMAMUX_IS_AVAILABLE)
    {DMA_IP_CH_SET_EN_MUX_SOURCE_REQ, TRUE},
    {DMA_IP_CH_SET_MUX_SOURCE_REQ, DMA_IP_REQ_MUX0_ALWAYS_ON0},
#endif
    {DMA_IP_CH_SET_EN_ERROR_INTERRUPT, TRUE},
};

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
uint32 g_DmaCh0_CallbackCounter = 0U;
uint32 g_DmaCh0_ErrorCallbackCounter = 0U;
uint32 g_DmaCh1_CallbackCounter = 0U;
uint32 g_DmaCh1_ErrorCallbackCounter = 0U;
uint32 g_DmaCh2_CallbackCounter = 0U;
uint32 g_DmaCh2_ErrorCallbackCounter = 0U;

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
Std_ReturnType CHECK_Uint8_BufferCompare(uint8 * pSource, uint8 * pDestination, uint32 Length);

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
void DmaCh0_Callback(void)
{
    g_DmaCh0_CallbackCounter++;
}

void DmaCh0_ErrorCallback(void)
{
    g_DmaCh0_ErrorCallbackCounter++;
}

void DmaCh1_Callback(void)
{
    g_DmaCh1_CallbackCounter++;
}

void DmaCh1_ErrorCallback(void)
{
    g_DmaCh1_ErrorCallbackCounter++;
}

void DmaCh2_Callback(void)
{
    g_DmaCh2_CallbackCounter++;
}

void DmaCh2_ErrorCallback(void)
{
    g_DmaCh2_ErrorCallbackCounter++;
}

Std_ReturnType CHECK_Uint8_BufferCompare(uint8 * pSource, uint8 * pDestination, uint32 Length)
{
    uint32 Index = 0U;

    for(Index = 0U; Index < Length; Index++)
    {
        if(pSource[Index] != pDestination[Index])
        {
            return E_NOT_OK;
        }
    }
    return E_OK;
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
int main(void)
{
    Std_ReturnType StdStatus = E_OK;
    boolean Status = TRUE;
    Dma_Ip_LogicChannelStatusType t_GetChannel0Status;
    Dma_Ip_LogicChannelStatusType t_GetChannel1Status;
    Dma_Ip_LogicChannelStatusType t_GetChannel2Status;

    /* Initialize Clock */
    Clock_Ip_InitClock(&Clock_Ip_aClockConfig[0]);

    /* Initialize Interrupt */
    IntCtrl_Ip_Init(&IntCtrlConfig_0);
#if defined(S32K3XX)
    IntCtrl_Ip_InstallHandler(DMATCD0_IRQn, Dma0_Ch0_IRQHandler, NULL_PTR);
    IntCtrl_Ip_InstallHandler(DMATCD1_IRQn, Dma0_Ch1_IRQHandler, NULL_PTR);
    IntCtrl_Ip_InstallHandler(DMATCD2_IRQn, Dma0_Ch2_IRQHandler, NULL_PTR);
#elif (defined(S32G2XX) || defined(S32G3XX) || defined(S32R45) || defined(S32R41) || defined(SAF8544))
    IntCtrl_Ip_InstallHandler(DMA0_0_15_IRQn, Dma0_Ch0_Ch15_IrqHandler, NULL_PTR);
    IntCtrl_Ip_InstallHandler(DMA0_ERR0_IRQn, Dma0_Error_IrqHandler, NULL_PTR);
#elif (defined(SJA1110) || defined(S32K1XX))
    IntCtrl_Ip_InstallHandler(DMA0_IRQn, Dma0_Ch0_IRQHandler, NULL_PTR);
    IntCtrl_Ip_InstallHandler(DMA1_IRQn, Dma0_Ch1_IRQHandler, NULL_PTR);
    IntCtrl_Ip_InstallHandler(DMA2_IRQn, Dma0_Ch2_IRQHandler, NULL_PTR);
    IntCtrl_Ip_InstallHandler(DMA_Error_IRQn, Dma0_Error_IrqHandler, NULL_PTR);
#endif

    /* Initialize DMA IP} Driver */
    (void)Dma_Ip_Init(&Dma_Ip_xDmaInitPB);

    /* Set Logic Channel 0 -> Source and Destination Address */
    Dma_Ip_SetLogicChannelTransferList(DMA_LOGIC_CH_0, DmaCh0_TransferList, DMA_CH0_CONFIG_LIST_DIMENSION);

    /* Set Logic Channel 1 -> Source and Destination Address for each Scatter/Gather Element */
    Dma_Ip_SetLogicChannelScatterGatherList(DMA_LOGIC_CH_1, DMA_LOGIC_CH_1_SGA_ELEMENT_0, DmaCh1_Element0_List, DMA_CH1_CONFIG_LIST_DIMENSION);
    Dma_Ip_SetLogicChannelScatterGatherList(DMA_LOGIC_CH_1, DMA_LOGIC_CH_1_SGA_ELEMENT_1, DmaCh1_Element1_List, DMA_CH1_CONFIG_LIST_DIMENSION);
    Dma_Ip_SetLogicChannelScatterGatherList(DMA_LOGIC_CH_1, DMA_LOGIC_CH_1_SGA_ELEMENT_2, DmaCh1_Element2_List, DMA_CH1_CONFIG_LIST_DIMENSION);
    Dma_Ip_SetLogicChannelScatterGatherConfig(DMA_LOGIC_CH_1, DMA_LOGIC_CH_1_SGA_ELEMENT_0);

    /* Set Logic Channel 2 */
    Dma_Ip_SetLogicChannelGlobalList(DMA_LOGIC_CH_2, DmaCh2_GlobalList, DMA_CH2_GLOBAL_LIST_DIMENSION);
    Dma_Ip_SetLogicChannelTransferList(DMA_LOGIC_CH_2, DmaCh2_TransferList, DMA_CH2_CONFIG_LIST_DIMENSION);

    /* Start transfer for logic Channel 0 */
    Dma_Ip_SetLogicChannelCommand(DMA_LOGIC_CH_0, DMA_IP_CH_SET_SOFTWARE_REQUEST);
    Dma_Ip_GetLogicChannelStatus(DMA_LOGIC_CH_0, &t_GetChannel0Status);
    while(t_GetChannel0Status.Done != TRUE)
    {
        Dma_Ip_GetLogicChannelStatus(DMA_LOGIC_CH_0, &t_GetChannel0Status);
    }

    /* Start transfer for logic Channel 1 */
    Dma_Ip_SetLogicChannelCommand(DMA_LOGIC_CH_1, DMA_IP_CH_SET_SOFTWARE_REQUEST);
    Dma_Ip_GetLogicChannelStatus(DMA_LOGIC_CH_1, &t_GetChannel1Status);
    while(t_GetChannel1Status.Done != TRUE)
    {
        Dma_Ip_GetLogicChannelStatus(DMA_LOGIC_CH_1, &t_GetChannel1Status);
    }

    /* Start transfer for logic Channel 2 */
    Dma_Ip_SetLogicChannelCommand(DMA_LOGIC_CH_2, DMA_IP_CH_SET_SOFTWARE_REQUEST);
    Dma_Ip_GetLogicChannelStatus(DMA_LOGIC_CH_2, &t_GetChannel2Status);
    while(t_GetChannel2Status.Done != TRUE)
    {
        Dma_Ip_GetLogicChannelStatus(DMA_LOGIC_CH_2, &t_GetChannel2Status);
    }

#if (STD_ON == CACHE_IP_IS_AVAILABLE)
    /* Invalidate Cache */
    Cache_Ip_InvalidateByAddr(CACHE_IP_DATA, (uint32)&DmaCh0_DestinationBuffer[0U], DMA_CH0_BUFFER_DIMENSION);
    Cache_Ip_InvalidateByAddr(CACHE_IP_DATA, (uint32)&DmaCh1_DestinationBuffer0[0U], DMA_CH1_ELEMENT0_DIMENSION);
    Cache_Ip_InvalidateByAddr(CACHE_IP_DATA, (uint32)&DmaCh1_DestinationBuffer1[0U], DMA_CH1_ELEMENT1_DIMENSION);
    Cache_Ip_InvalidateByAddr(CACHE_IP_DATA, (uint32)&DmaCh1_DestinationBuffer2[0U], DMA_CH1_ELEMENT2_DIMENSION);
    Cache_Ip_InvalidateByAddr(CACHE_IP_DATA, (uint32)&DmaCh2_DestinationBuffer[0U], DMA_CH2_BUFFER_DIMENSION);
#endif

    if(DMA_IRQ_TRIGGERED_ONCE != g_DmaCh0_CallbackCounter)
    {
        Status = FALSE;
    }
    if(DMA_IRQ_NOT_TRIGGERED != g_DmaCh0_ErrorCallbackCounter)
    {
        Status = FALSE;
    }
    if(DMA_IRQ_TRIGGERED_ONCE != g_DmaCh1_CallbackCounter)
    {
        Status = FALSE;
    }
    if(DMA_IRQ_NOT_TRIGGERED != g_DmaCh1_ErrorCallbackCounter)
    {
        Status = FALSE;
    }
    if(DMA_IRQ_TRIGGERED_ONCE != g_DmaCh2_CallbackCounter)
    {
        Status = FALSE;
    }
    if(DMA_IRQ_NOT_TRIGGERED != g_DmaCh2_ErrorCallbackCounter)
    {
        Status = FALSE;
    }

    StdStatus = CHECK_Uint8_BufferCompare(DmaCh0_SourceBuffer, DmaCh0_DestinationBuffer, DMA_CH0_BUFFER_DIMENSION);
    if(E_OK != StdStatus)
    {
        Status = FALSE;
    }
    StdStatus = CHECK_Uint8_BufferCompare(DmaCh1_SourceBuffer0, DmaCh1_DestinationBuffer0, DMA_CH1_ELEMENT0_DIMENSION);
    if(E_OK != StdStatus)
    {
        Status = FALSE;
    }StdStatus = CHECK_Uint8_BufferCompare(DmaCh1_SourceBuffer1, DmaCh1_DestinationBuffer1, DMA_CH1_ELEMENT1_DIMENSION);
    if(E_OK != StdStatus)
    {
        Status = FALSE;
    }
    StdStatus = CHECK_Uint8_BufferCompare(DmaCh1_SourceBuffer2, DmaCh1_DestinationBuffer2, DMA_CH1_ELEMENT2_DIMENSION);
    if(E_OK != StdStatus)
    {
        Status = FALSE;
    }
    StdStatus = CHECK_Uint8_BufferCompare(DmaCh2_SourceBuffer, DmaCh2_DestinationBuffer, DMA_CH2_BUFFER_DIMENSION);
    if(E_OK != StdStatus)
    {
        Status = FALSE;
    }

    DevAssert(Status);

    return (0U);
}

/* END main */
/*!
** @}
*/
