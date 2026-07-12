/*
*   (c) Copyright 2022 NXP
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

#ifdef __cplusplus
extern "C" {
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Clock_Ip.h"
#include "IntCtrl_Ip.h"
#include "Siul2_Port_Ip.h"
#include "Flexio_I2s_Ip.h"
#include "Flexio_Mcl_Ip.h"
#include "Dma_Ip.h"
#include "check_example.h"

/*==================================================================================================
*                                      EXTERN DECLARATIONS
==================================================================================================*/
extern void MCL_FLEXIO_ISR(void);
#if defined(S32K312)
extern void Dma0_Ch6_IRQHandler(void);
extern void Dma0_Ch7_IRQHandler(void);
#else /* S32K342 and S32K344 */
extern void Dma0_Ch16_IRQHandler(void);
extern void Dma0_Ch17_IRQHandler(void);
#endif

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define BUFF_SIZE           (100U)

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
#define I2S_START_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE
#include "I2s_MemMap.h"
uint16 SendData[BUFF_SIZE];
uint16 RecvBuff[BUFF_SIZE];
#define I2S_STOP_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE
#include "I2s_MemMap.h"

volatile uint8 MasterTransferCount = 0;
volatile uint8 SlaveTransferCount = 0;

/* Flexio_I2s callback function for Master */
void Flexio_I2s_MasterNotification(Flexio_I2s_Ip_EventType Event, void *UserData)
{
    if (Event == FLEXIO_I2S_IP_EVENT_END_TRANSFER)
    {
        MasterTransferCount += (int)UserData;
    }
}
/* Flexio_I2s callback function for Slave */
void Flexio_I2s_SlaveNotification(Flexio_I2s_Ip_EventType Event, void *UserData)
{
    if (Event == FLEXIO_I2S_IP_EVENT_END_TRANSFER)
    {
        SlaveTransferCount += (int)UserData;
    }
}


int main(void)
{
    uint32 i;
    uint32 bytesRemaining = 0U;
    volatile boolean bStatus = TRUE;
    Clock_Ip_StatusType clockStatus = CLOCK_IP_SUCCESS;

    /* Initialize the clock tree */
    clockStatus = Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);
    while (clockStatus != CLOCK_IP_SUCCESS)
    {
        clockStatus = Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);
    }

    /* Enable the Dma and FLEXIO interrupt vector */
    IntCtrl_Ip_InstallHandler(FLEXIO_IRQn, MCL_FLEXIO_ISR, NULL_PTR);
    IntCtrl_Ip_EnableIrq(FLEXIO_IRQn);
#if defined(S32K312)
    IntCtrl_Ip_InstallHandler(DMATCD6_IRQn, &Dma0_Ch6_IRQHandler, NULL_PTR);
    IntCtrl_Ip_EnableIrq(DMATCD6_IRQn);
    IntCtrl_Ip_InstallHandler(DMATCD7_IRQn, &Dma0_Ch7_IRQHandler, NULL_PTR);
    IntCtrl_Ip_EnableIrq(DMATCD7_IRQn);
#else /* S32K342 and S32K344 */
    IntCtrl_Ip_InstallHandler(DMATCD16_IRQn, &Dma0_Ch16_IRQHandler, NULL_PTR);
    IntCtrl_Ip_EnableIrq(DMATCD16_IRQn);
    IntCtrl_Ip_InstallHandler(DMATCD17_IRQn, &Dma0_Ch17_IRQHandler, NULL_PTR);
    IntCtrl_Ip_EnableIrq(DMATCD17_IRQn);
#endif

    /* Init buffer data */
    for (i = 0; i < BUFF_SIZE; i++)
    {
        SendData[i] = i + 1;
        RecvBuff[i] = 0U;
    }

    /* Initialize port */
    Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
    /* Initialize DMA IP Driver */
    (void)Dma_Ip_Init(&Dma_Ip_xDmaInitPB);
    /* Initialize Flexio_I2s module */
    Flexio_Mcl_Ip_InitDevice(&Flexio_Ip_xFlexioInit);
    Flexio_I2s_Ip_SlaveInit(FLEXIO_I2S_VS_0_INSTANCE, SLAVE_VS_0_CHANNEL, &Flexio_I2s_Ip_xInitConfig_0_VS_0);
    Flexio_I2s_Ip_MasterInit(FLEXIO_I2S_VS_0_INSTANCE, MASTER_VS_0_CHANNEL, &Flexio_I2s_Ip_xInitConfig_1_VS_0);

    /* Part1: Master sends data / Slave receives data */
    /* Slave starts receiving data */
    (void)Flexio_I2s_Ip_SlaveReceiveData(FLEXIO_I2S_VS_0_INSTANCE, SLAVE_VS_0_CHANNEL, (uint8*)RecvBuff, BUFF_SIZE);

    /* Master starts sending data */
    (void)Flexio_I2s_Ip_MasterSendDataBlocking(FLEXIO_I2S_VS_0_INSTANCE, MASTER_VS_0_CHANNEL, (const uint8*)SendData, BUFF_SIZE, FLEXIO_I2S_IP_INTERNAL_TIMEOUT_VALUE);

    /* Wait for transfer complete */
    while (Flexio_I2s_Ip_MasterGetStatus(FLEXIO_I2S_VS_0_INSTANCE, MASTER_VS_0_CHANNEL, &bytesRemaining) != FLEXIO_I2S_IP_STATUS_SUCCESS)
    {
    }
    if (bytesRemaining != 0U)
    {
        bStatus = FALSE;
    }

    while (Flexio_I2s_Ip_MasterGetStatus(FLEXIO_I2S_VS_0_INSTANCE, SLAVE_VS_0_CHANNEL, &bytesRemaining) != FLEXIO_I2S_IP_STATUS_SUCCESS)
    {
    }
    if (bytesRemaining != 0U)
    {
        bStatus = FALSE;
    }

    /* Notification was called 1 time with exact value after transfer */
    if ((1U != MasterTransferCount) || (2U != SlaveTransferCount))
    {
        bStatus = FALSE;
    }
    /* Check the received data */
    for (i = 0U; i < BUFF_SIZE; i++)
    {
        if (RecvBuff[i] != SendData[i])
        {
            bStatus = FALSE;
            break;
        }
    }

    /* Part2: Master receives data / Slave sends data */
    /* Reset Status and Recv buffer for next transfer */
    MasterTransferCount = 0U;
    SlaveTransferCount = 0U;
    for (i = 0; i < BUFF_SIZE; i++)
    {
        RecvBuff[i] = 0U;
    }

    /* Prepare to send data */
    (void)Flexio_I2s_Ip_SlaveSendData(FLEXIO_I2S_VS_0_INSTANCE, SLAVE_VS_0_CHANNEL, (const uint8*)SendData, BUFF_SIZE);

    /* Master starts receiving data */
    (void)Flexio_I2s_Ip_MasterReceiveData(FLEXIO_I2S_VS_0_INSTANCE, MASTER_VS_0_CHANNEL, (uint8*)RecvBuff, BUFF_SIZE);

    /* Wait for transfer complete */
    while (Flexio_I2s_Ip_MasterGetStatus(FLEXIO_I2S_VS_0_INSTANCE, MASTER_VS_0_CHANNEL, &bytesRemaining) != FLEXIO_I2S_IP_STATUS_SUCCESS)
    {
    }
    if (bytesRemaining != 0U)
    {
        bStatus = FALSE;
    }

    while (Flexio_I2s_Ip_MasterGetStatus(FLEXIO_I2S_VS_0_INSTANCE, SLAVE_VS_0_CHANNEL, &bytesRemaining) != FLEXIO_I2S_IP_STATUS_SUCCESS)
    {
    }
    if (bytesRemaining != 0U)
    {
        bStatus = FALSE;
    }

    /* Notification was called 1 time with exact value after transfer */
    if ((1U != MasterTransferCount) || (2U != SlaveTransferCount))
    {
        bStatus = FALSE;
    }
    /* Check the received data */
    for (i = 0U; i < BUFF_SIZE; i++)
    {
        if (RecvBuff[i] != SendData[i])
        {
            bStatus = FALSE;
            break;
        }
    }

    /* De-initial Flexio_I2s*/
    Flexio_I2s_Ip_MasterDeinit(FLEXIO_I2S_VS_0_INSTANCE, MASTER_VS_0_CHANNEL);
    Flexio_I2s_Ip_SlaveDeinit(FLEXIO_I2S_VS_0_INSTANCE, SLAVE_VS_0_CHANNEL);

    Exit_Example(bStatus);

    return 0;
}


#ifdef __cplusplus
}
#endif

/** @} */
