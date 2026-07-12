/*!
** Copyright 2019 NXP
** @file main.c
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */


/* Including necessary configuration files. */
#include "Mcal.h"
#include "Clock_Ip.h"
#include "FlexCAN_Ip.h"
#include "IntCtrl_Ip.h"

#define MSG_ID 20u
#define RX_MB_IDX 1U
#define TX_MB_IDX 0U
volatile int exit_code = 0;
/* User includes */
uint8 dummyData[8] = {1,2,3,4,5,6,7};
/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
extern void CAN0_ORED_0_31_MB_IRQHandler(void);

int main(void)
{
    /* Write your code here */
    Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);

    IntCtrl_Ip_EnableIrq(FlexCAN0_1_IRQn);
    IntCtrl_Ip_InstallHandler(FlexCAN0_1_IRQn, CAN0_ORED_0_31_MB_IRQHandler, NULL_PTR);

    Flexcan_Ip_DataInfoType rx_info = {
            .msg_id_type = FLEXCAN_MSG_ID_STD,
            .data_length = 8u,
            .is_polling = TRUE,
            .is_remote = FALSE
    };
    Flexcan_Ip_MsgBuffType rxData;
    FlexCAN_Ip_Init(INST_FLEXCAN_0, &FlexCAN_State0, &FlexCAN_Config0);
    FlexCAN_Ip_SetStartMode(INST_FLEXCAN_0);
    FlexCAN_Ip_ConfigRxMb(INST_FLEXCAN_0, RX_MB_IDX, &rx_info, MSG_ID);
    rx_info.is_polling = FALSE;
    FlexCAN_Ip_Send(INST_FLEXCAN_0, TX_MB_IDX, &rx_info, MSG_ID, (uint8 *)&dummyData);
    FlexCAN_Ip_Receive(INST_FLEXCAN_0, RX_MB_IDX, &rxData, TRUE);
    while(FlexCAN_Ip_GetTransferStatus(INST_FLEXCAN_0, RX_MB_IDX) != FLEXCAN_STATUS_SUCCESS)
    { FlexCAN_Ip_MainFunctionRead(INST_FLEXCAN_0, RX_MB_IDX); }
    FlexCAN_Ip_SetStopMode(INST_FLEXCAN_0);
    FlexCAN_Ip_Deinit(INST_FLEXCAN_0);
    return 0;
}

/* END main */
/*!
** @}
*/
