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
#include "Sai_Ip.h"
#include "check_example.h"

/*==================================================================================================
*                                      EXTERN DECLARATIONS
==================================================================================================*/
extern void SAI0_IRQHandler(void);
extern void SAI1_IRQHandler(void);

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define BUFF_SIZE           (100U)

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

/* Sai callback function for Master */
void Sai_TransmitterNotification(Sai_Ip_ReportType event)
{
    if (event == SAI_IP_TX_COMPLETE)
    {
        TxFlag = TRUE;
    }
}

/* Sai callback function for Slave */
void Sai_ReceiverNotification(Sai_Ip_ReportType event)
{
    if (event == SAI_IP_RX_COMPLETE)
    {
        RxFlag = TRUE;
    }
}


int main(void)
{
    uint32 i;
    volatile boolean bStatus = TRUE;
    Clock_Ip_StatusType clockStatus = CLOCK_IP_SUCCESS;

    /* Initialize the clock tree */
    clockStatus = Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);
    while (clockStatus != CLOCK_IP_SUCCESS)
    {
        clockStatus = Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);
    }

    /*Enable the SAI interrupt vector */
    IntCtrl_Ip_InstallHandler(SAI0_IRQn, SAI0_IRQHandler, NULL_PTR);
    IntCtrl_Ip_EnableIrq(SAI0_IRQn);
    IntCtrl_Ip_InstallHandler(SAI1_IRQn, SAI1_IRQHandler, NULL_PTR);
    IntCtrl_Ip_EnableIrq(SAI1_IRQn);

    /* Init buffer data */
    for (i = 0; i < BUFF_SIZE; i++)
    {
        LeftData[i] = i*2;
        RightData[i] = i*2+1;
    }

    /* Initialize port */
    Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
    /* Initialize SAI module */

    Sai_Ip_RxInit(SAI_RECEIVER_VS_0_INSTANCE, &Sai_Ip_xInitConfig_1_VS_0);
    Sai_Ip_TxInit(SAI_TRANSMIT_VS_0_INSTANCE, &Sai_Ip_xInitConfig_0_VS_0);

    /* Call receive data first because rx is slave */
    Sai_Ip_Receive(SAI_RECEIVER_VS_0_INSTANCE, (uint8_t**) RecvData, BUFF_SIZE * 2U);
    /* Send data from SAI0 to SAI1 */
    Sai_Ip_Send(SAI_TRANSMIT_VS_0_INSTANCE, (const uint8_t**) SendData, BUFF_SIZE);
    /* Wait for transfer complete */
    while (Sai_Ip_GetReceivingStatus(SAI_RECEIVER_VS_0_INSTANCE, NULL_PTR) == SAI_IP_STATUS_BUSY)
    {
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

    Sai_Ip_TxDeInit(SAI_TRANSMIT_VS_0_INSTANCE);
    Sai_Ip_RxDeInit(SAI_RECEIVER_VS_0_INSTANCE);

    Exit_Example(bStatus);

    return 0;
}


#ifdef __cplusplus
}
#endif

/** @} */
