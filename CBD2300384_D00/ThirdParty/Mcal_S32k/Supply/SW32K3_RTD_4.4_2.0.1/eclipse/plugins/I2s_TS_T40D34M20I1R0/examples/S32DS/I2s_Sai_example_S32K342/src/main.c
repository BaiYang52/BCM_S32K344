/*
*   (c) Copyright 2021-2022 NXP
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
*   @addtogroup I2s
*   @{
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
#include "Port.h"
#include "Platform.h"
#include "check_example.h"
/*==================================================================================================
*                                      EXTERN DECLARATIONS
==================================================================================================*/
extern ISR(SAI0_IRQHandler);
extern ISR(SAI1_IRQHandler);

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define TIME_OUT  500000
#define BUFF_SIZE 100

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
uint16 LeftData[BUFF_SIZE];
uint16 RightData[BUFF_SIZE];
uint16* SendData[2U] = { LeftData, RightData };
uint16 RecvBuff[BUFF_SIZE * 2U];
uint16* RecvData[1U] = { RecvBuff };
volatile boolean TxFlag = FALSE;
volatile boolean RxFlag = FALSE;

/* I2s callback function for Master */
void I2s_TransmitterNotification(Sai_Ip_ReportType event)
{
    if (event == SAI_IP_TX_COMPLETE)
    {
        TxFlag = TRUE;
    }
}

/* I2s callback function for Slave */
void I2s_ReceiverNotification(Sai_Ip_ReportType event)
{
    if (event == SAI_IP_RX_COMPLETE)
    {
        RxFlag = TRUE;
    }
}


int main(void)
{
    volatile boolean bStatus = TRUE;
    Std_ReturnType Ret = 0U;
    volatile uint32 u32Timeout = 0U;
    I2s_RequestType * pRequest;
    I2s_StatusStructType transferStatus;
    uint32 i;

    I2s_RequestType I2s_TransmitRequest = {
        .bufferSize = BUFF_SIZE,
        .dataDirection = I2S_SEND_DATA,
        .pDataBuffer = (I2s_DataType (**))SendData
    };

    I2s_RequestType I2s_ReceiveRequest = {
        .bufferSize = BUFF_SIZE*2,
        .dataDirection = I2S_RECEIVE_DATA,
        .pDataBuffer = (I2s_DataType (**))RecvData
    };

    for (i = 0; i < BUFF_SIZE; i++)
    {
        LeftData[i] = i*2;
        RightData[i] = i*2+1;
        RecvBuff[i*2] = 0U;
        RecvBuff[i*2+1] = 0U;
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
    Platform_InstallIrqHandler(SAI0_IRQn, SAI0_IRQHandler, NULL_PTR);
    Platform_SetIrq(SAI0_IRQn, TRUE);
    Platform_InstallIrqHandler(SAI1_IRQn, SAI1_IRQHandler, NULL_PTR);
    Platform_SetIrq(SAI1_IRQn, TRUE);

    /* Initialize Port pin */
    Port_Init(&Port_Config_VS_0);

    /* Init Transmitter using mux line with 2 channels (Left and Right) */
    /* Init Receiver using mux disable */
    I2s_Init(&I2s_xConfig_VS_0);

    /* Start receiving data */
    pRequest = &I2s_ReceiveRequest;
    Ret = I2s_AsyncTransmit(I2S_SAI_RECEIVER, pRequest);

    if (E_NOT_OK == Ret)
    {
        bStatus = FALSE;
    }
    else
    {
        /* Start sending data */
        pRequest = &I2s_TransmitRequest;
        Ret = I2s_AsyncTransmit(I2S_SAI_TRANSMIT, pRequest);
    }

    if (E_NOT_OK == Ret)
    {
        bStatus = FALSE;
    }
    else
    {
        u32Timeout = TIME_OUT;
        /* Wait for the completion */
        do
        {
            I2s_GetStatus(I2S_SAI_RECEIVER, &transferStatus);
            u32Timeout--;
            if (u32Timeout == 0U)
            {
                bStatus = FALSE;
                break;
            }
        } while (I2S_STATUS_BUSY == transferStatus.status);

        if ((I2S_STATUS_COMPLETED != transferStatus.status) || (0U != transferStatus.byteRemainCount))
        {
            bStatus = FALSE;
        }

        I2s_GetStatus(I2S_SAI_TRANSMIT, &transferStatus);
        if ((I2S_STATUS_COMPLETED != transferStatus.status) || (0U != transferStatus.byteRemainCount))
        {
            bStatus = FALSE;
        }

        /* Notification was called after transfer */
        if ((TxFlag != TRUE) || (RxFlag != TRUE))
        {
            bStatus = FALSE;
        }
        /* Check the received data */
        for (i = 0U; i < BUFF_SIZE; i++)
        {
            if ((RecvBuff[i*2] != LeftData[i]) || (RecvBuff[i*2+1] != RightData[i]))
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

