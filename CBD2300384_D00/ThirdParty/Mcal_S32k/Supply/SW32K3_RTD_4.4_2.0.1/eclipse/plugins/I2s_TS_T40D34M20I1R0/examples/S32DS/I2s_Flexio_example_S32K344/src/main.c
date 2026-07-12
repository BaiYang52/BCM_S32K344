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
#include "CDD_I2s.h"
#include "Mcu.h"
#include "Mcl.h"
#include "Port.h"
#include "Platform.h"
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
#define TIME_OUT            (300000U)
#define BUFF_SIZE           (100U)

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
#define I2S_START_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE
#include "I2s_MemMap.h"
uint16 SendData1[BUFF_SIZE];
uint16* SendData[1U] = { SendData1 };
uint16 RecvBuff1[BUFF_SIZE];
uint16* RecvData[1U] = { RecvBuff1 };
#define I2S_STOP_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE
#include "I2s_MemMap.h"

volatile uint8 MasterTransferCount = 0;
volatile uint8 SlaveTransferCount = 0;

/* I2s callback function for Master */
void I2s_MasterNotification(Flexio_I2s_Ip_EventType Event, void *UserData)
{
    if (Event == FLEXIO_I2S_IP_EVENT_END_TRANSFER)
    {
        MasterTransferCount += (int)UserData;
    }
}
/* I2s callback function for Slave */
void I2s_SlaveNotification(Flexio_I2s_Ip_EventType Event, void *UserData)
{
    if (Event == FLEXIO_I2S_IP_EVENT_END_TRANSFER)
    {
        SlaveTransferCount += (int)UserData;
    }
}

int main(void)
{
    volatile boolean bStatus = TRUE;
    Std_ReturnType Ret = 0U;
    volatile uint32 u32Timeout = 0U;
    I2s_RequestType * pRequest;
    I2s_StatusStructType transferStatus;
    I2s_RequestType I2s_MasterRequest;
    I2s_RequestType I2s_SlaveRequest;
    uint32 i;

    /* Create Master/Slave request */
    I2s_MasterRequest.bufferSize = BUFF_SIZE;
    I2s_MasterRequest.dataDirection = I2S_SEND_DATA;
    I2s_MasterRequest.pDataBuffer = (I2s_DataType (**))SendData;

    I2s_SlaveRequest.bufferSize = BUFF_SIZE;
    I2s_SlaveRequest.dataDirection = I2S_RECEIVE_DATA;
    I2s_SlaveRequest.pDataBuffer = (I2s_DataType (**))RecvData;

    /* Init buffer data */
    for (i = 0; i < BUFF_SIZE; i++)
    {
        SendData1[i] = i + 1;
        RecvBuff1[i] = 0U;
    }

    /* Initialize Mcu Driver */
    Mcu_Init(NULL_PTR);
    Mcu_InitClock(McuClockSettingConfig_0);
    while (MCU_PLL_LOCKED != Mcu_GetPllStatus())
    {
        /* Busy wait until the System PLL is locked */
    }
    Mcu_DistributePllClock();
    Mcu_SetMode(McuModeSettingConf_0);

    /* Initialize OsIf */
    OsIf_Init(NULL_PTR);

    /* Initialize Platform Driver */
    Platform_Init(NULL_PTR);
    Platform_InstallIrqHandler(FLEXIO_IRQn, &MCL_FLEXIO_ISR, NULL_PTR);
    Platform_SetIrq(FLEXIO_IRQn, TRUE);
#if defined(S32K312)
    Platform_InstallIrqHandler(DMATCD6_IRQn, &Dma0_Ch6_IRQHandler, NULL_PTR);
    Platform_SetIrq(DMATCD6_IRQn, TRUE);
    Platform_InstallIrqHandler(DMATCD7_IRQn, &Dma0_Ch7_IRQHandler, NULL_PTR);
    Platform_SetIrq(DMATCD7_IRQn, TRUE);
#else /* S32K342 and S32K344 */
    Platform_InstallIrqHandler(DMATCD16_IRQn, &Dma0_Ch16_IRQHandler, NULL_PTR);
    Platform_SetIrq(DMATCD16_IRQn, TRUE);
    Platform_InstallIrqHandler(DMATCD17_IRQn, &Dma0_Ch17_IRQHandler, NULL_PTR);
    Platform_SetIrq(DMATCD17_IRQn, TRUE);
#endif

    /* Initialize Port pin and Mcl */
    Port_Init(&Port_Config_VS_0);
    Mcl_Init(&Mcl_Config_VS_0);
    /* Init I2S over SAI as Master and FLEXIO_I2S as Slave */
    I2s_Init(&I2s_xConfig_VS_0);

    /* Part1: Master sends data / Slave receives data */
    /* Start receiving data */
    pRequest = &I2s_SlaveRequest;
    Ret = I2s_AsyncTransmit(I2S_FLEXIO_SLAVE, pRequest);

    if (E_NOT_OK == Ret)
    {
        bStatus = FALSE;
    }
    else
    {
        /* Start sending data */
        pRequest = &I2s_MasterRequest;
        Ret = I2s_SyncTransmit(I2S_FLEXIO_MASTER, pRequest);
    }

    if (E_NOT_OK == Ret)
    {
        bStatus = FALSE;
    }
    else
    {
        I2s_GetStatus(I2S_FLEXIO_SLAVE, &transferStatus);
        if ((I2S_STATUS_COMPLETED != transferStatus.status) || (0U != transferStatus.byteRemainCount))
        {
            bStatus = FALSE;
        }
        I2s_GetStatus(I2S_FLEXIO_MASTER, &transferStatus);
        if ((I2S_STATUS_COMPLETED != transferStatus.status) || (0U != transferStatus.byteRemainCount))
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
            if (RecvBuff1[i] != SendData1[i])
            {
                bStatus = FALSE;
                break;
            }
        }
    }

    /* Part2: Master receives data / Slave sends data */
    /* Update Master/Slave request */
    I2s_MasterRequest.bufferSize = BUFF_SIZE;
    I2s_MasterRequest.dataDirection = I2S_RECEIVE_DATA;
    I2s_MasterRequest.pDataBuffer = (I2s_DataType (**))RecvData;

    I2s_SlaveRequest.bufferSize = BUFF_SIZE;
    I2s_SlaveRequest.dataDirection = I2S_SEND_DATA;
    I2s_SlaveRequest.pDataBuffer = (I2s_DataType (**))SendData;

    /* Reset Status and Recv buffer for next transfer */
    MasterTransferCount = 0U;
    SlaveTransferCount = 0U;
    for (i = 0; i < BUFF_SIZE; i++)
    {
        RecvBuff1[i] = 0U;
    }

    /* Prepare to send data */
    pRequest = &I2s_SlaveRequest;
    Ret = I2s_AsyncTransmit(I2S_FLEXIO_SLAVE, pRequest);

    if (E_NOT_OK == Ret)
    {
        bStatus = FALSE;
    }
    else
    {
        /* Start receiving data */
        pRequest = &I2s_MasterRequest;
        Ret = I2s_AsyncTransmit(I2S_FLEXIO_MASTER, pRequest);
    }

    if (E_NOT_OK == Ret)
    {
        bStatus = FALSE;
    }
    else
    {
        /* Wait for Master receives all data */
        u32Timeout = TIME_OUT;
        do
        {
            I2s_GetStatus(I2S_FLEXIO_MASTER, &transferStatus);
            u32Timeout--;
            if (u32Timeout == 0U)
            {
                break;
            }
        } while (I2S_STATUS_BUSY == transferStatus.status);
        if ((I2S_STATUS_COMPLETED != transferStatus.status) || (0U != transferStatus.byteRemainCount))
        {
            bStatus = FALSE;
        }
        I2s_GetStatus(I2S_FLEXIO_SLAVE , &transferStatus);
        if ((I2S_STATUS_COMPLETED != transferStatus.status) || (0U != transferStatus.byteRemainCount))
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
            if (RecvBuff1[i] != SendData1[i])
            {
                bStatus = FALSE;
                break;
            }
        }
    }

    I2s_DeInit();

    Exit_Example(bStatus);

    return 0;
}


#ifdef __cplusplus
}
#endif

/** @} */
