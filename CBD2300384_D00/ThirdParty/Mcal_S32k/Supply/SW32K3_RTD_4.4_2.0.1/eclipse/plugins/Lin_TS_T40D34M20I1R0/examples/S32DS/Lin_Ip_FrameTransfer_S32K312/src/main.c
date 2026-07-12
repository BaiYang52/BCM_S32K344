/*==================================================================================================
*   Project              : M4_SRC_PROJECT_NAME
*   Platform             : M4_SRC_MCU_FAMILY
*   Peripheral           : M4_SRC_USED_PERIPHERAL
*   Dependencies         : M4_SRC_AR_MODULE_DEPENDENCY
*
*   Autosar Version      : M4_SRC_AR_SPEC_VERSION_MAJOR.M4_SRC_AR_SPEC_VERSION_MINOR.M4_SRC_AR_SPEC_VERSION_PATCH
*   Autosar Revision     : M4_SRC_AR_RELEASE_REVISION
*   Autosar Conf.Variant :
*   SW Version           : M4_SRC_SW_VERSION_MAJOR.M4_SRC_SW_VERSION_MINOR.M4_SRC_SW_VERSION_PATCH
*   Build Version        : M4_SRC_BUILD_ID
*
   (c) Copyright M4_SRC_YEAR_ID M4_SRC_COPYRIGHTED_TO
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
*   @file main.c
*
*   @addtogroup main_module main module documentation
*   @{
*/

/* Including necessary configuration files. */
#include "Mcal.h"


/* User includes */
#include "OsIf.h"
#include "Flexio_Lin_Ip.h"
#include "Lpuart_Lin_Ip.h"
#include "Flexio_Mcl_Ip.h"
#include "Clock_Ip.h"
#include "IntCtrl_Ip.h"
#include "Siul2_Port_Ip.h"
#include "Siul2_Icu_Ip.h"
#include "Pit_Ip.h"
#include "check_example.h"

/* Global Macros */
#define BUFFER_SIZE             (3U)
#define T_LIN_TIME_OUT          (400000U)
#define FLEXIO_PID              (0x1A)
#define LPUART_PID              (0x1A)
#define MASTER_ROLE             (0U)
#define SLAVE_ROLE              (1U)

#define FLEXIO_INSTANCE         (0U)

/* PIT instance used - 0 */
#define PIT_INST_0              0U
/* PIT channel used - 0 */
#define PIT_CH_0                0U

#define PIT_PERIOD              0xFFFFFFFFU

/* PIT clock in MHz */
#define PIT_CLK                 40U

/* Siul2 Icu instance used - 0 */
#define SIUL2_ICU_INST_0        0U
/* Siul2 Icu channel used - 14 */
#define SIUL2_ICU_CHN_14        14U

/* Global Variables */
volatile uint64 TmpCountValue = 0;
volatile uint64 CountValue = 0;
volatile uint32 BitTimeNs = 0;
volatile uint8 Count = 0;
uint8 FlexioTxBuff[BUFFER_SIZE] = {0x01, 0x02, 0x03};
uint8 LpuartTxBuff[BUFFER_SIZE] = {0x04, 0x05, 0x06};
//uint8 SduBuf[3U];

Lpuart_Lin_Ip_PduType LinLpuartPdu[] =
{
    {
                .Pid = (uint8)LPUART_PID,
                .Cs = LPUART_LIN_IP_ENHANCED_CS,
                .SduPtr = LpuartTxBuff,
                .Drc = LPUART_LIN_IP_FRAMERESPONSE_TX,
                .Dl = (uint8)BUFFER_SIZE
    },
    {
                .Pid = (uint8)LPUART_PID,
                .Cs = LPUART_LIN_IP_ENHANCED_CS,
                .SduPtr = NULL_PTR,
                .Drc = LPUART_LIN_IP_FRAMERESPONSE_RX,
                .Dl = (uint8)BUFFER_SIZE
    }
};

Flexio_Lin_Ip_PduType LinFlexioPdu[] =
{
    {
                .Pid = (uint8)FLEXIO_PID,
                .Cs = FLEXIO_LIN_IP_ENHANCED_CS,
                .SduPtr = FlexioTxBuff,
                .Drc = FLEXIO_LIN_IP_FRAMERESPONSE_TX,
                .Dl = (uint8)BUFFER_SIZE
    },
    {
                .Pid = (uint8)FLEXIO_PID,
                .Cs = FLEXIO_LIN_IP_ENHANCED_CS,
                .SduPtr = NULL_PTR,
                .Drc = FLEXIO_LIN_IP_FRAMERESPONSE_RX,
                .Dl = (uint8)BUFFER_SIZE
    }
};

boolean CheckReceiveBuffer(uint8 *OriginalBuffer, uint8 * ReceiveBuffer)
{
    uint8 Index;
    volatile boolean RetVal = TRUE;
    for (Index = 0; Index < BUFFER_SIZE; Index++)
    {
        if (OriginalBuffer[Index] != *(ReceiveBuffer + Index))
        {
            RetVal = FALSE;
        }
    }
    return RetVal;
}

void Siul2Callback(void)
{
    if (!Count)
    {
        TmpCountValue = Pit_Ip_GetCurrentTimer(PIT_INST_0, PIT_CH_0);
        Count++;
    }
    else
    {
        CountValue = Pit_Ip_GetCurrentTimer(PIT_INST_0, PIT_CH_0);
        BitTimeNs = 1000 * (TmpCountValue - CountValue) / PIT_CLK;
        TmpCountValue = CountValue;

        (void)Lpuart_Lin_Ip_AutoBaudCapture(Lpuart_Lin_Ip_Sa_pHwConfigPB_1_VS_0.Instance);
    }
}

void AutobaudCallback(const uint8 Instance, uint32 *NanoSeconds)
{
    (void)Instance;
    *NanoSeconds = BitTimeNs;
}

void LpuartSlaveCallback (uint8 Instance, const  Lpuart_Lin_Ip_StateStructType *LpuartStateStruct)
{
    switch (LpuartStateStruct->CurrentEventId)
    {
        /* Baudrate adjusted */
        case LPUART_LIN_IP_BAUDRATE_ADJUSTED:

            (void)Siul2_Icu_Ip_DeInit(SIUL2_ICU_INST_0);
            /* Re-configure PTB28 as LPUART_RX */
            Siul2_Port_Ip_SetInputBuffer(PORTB_H_HALF, 12, FALSE, 161, PORT_INPUT_MUX_ALT0);
            Siul2_Port_Ip_SetInputBuffer(PORTB_H_HALF, 12, TRUE, 192, PORT_INPUT_MUX_ALT4);
            Pit_Ip_StopChannel(PIT_INST_0, PIT_CH_0);
            Count = 0;
            break;

        /* Header transmission ok */
        case LPUART_LIN_IP_RECV_HEADER_OK:
            if (LpuartStateStruct->CurrentPid == LinLpuartPdu[SLAVE_ROLE].Pid)
            {
                /* Master start send header */
                (void)Lpuart_Lin_Ip_SendFrame(Instance, (const Lpuart_Lin_Ip_PduType *)&LinLpuartPdu[SLAVE_ROLE]);
            }
            break;
        default:
            /* do nothing */
            break;
    }
}

void FlexioSlaveCallback (uint8 Instance, const  Flexio_Lin_Ip_StateStructType *FlexioStateStruct)
{
    if (FlexioStateStruct->CurrentEventId == FLEXIO_LIN_IP_RECV_HEADER_OK)
    {
        (void)Flexio_Lin_Ip_SendFrame(Instance, (const Flexio_Lin_Ip_PduType *)&LinFlexioPdu[SLAVE_ROLE]);
    }
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{

    uint8 *RecvBuffer[2];
    uint8 *DummyBuffer;
    uint32 TimeoutValue = 0u;
    volatile boolean FlexioCheckData;
    volatile boolean LpuartCheckData;
    volatile Flexio_Lin_Ip_TransferStatusType FlexioSlaveStatus = FLEXIO_LIN_IP_STATUS_OPERATIONAL;
    volatile Flexio_Lin_Ip_TransferStatusType FlexioMasterStatus = FLEXIO_LIN_IP_STATUS_OPERATIONAL;
    volatile Lpuart_Lin_Ip_TransferStatusType LpuartSlaveStatus = LPUART_LIN_IP_STATUS_OPERATIONAL;
    volatile Lpuart_Lin_Ip_TransferStatusType LpuartMasterStatus = LPUART_LIN_IP_STATUS_OPERATIONAL;

    /* Init clock  */
    Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);

    /* Timer Init */
    Pit_Ip_Init(PIT_INST_0, &PIT_0_InitConfig_PB);
    Pit_Ip_InitChannel(PIT_INST_0, &PIT_0_ChannelConfig_PB[0]);

    /*Enable Flexio module */
    /* Init Flexio common Mcl  */
    Flexio_Mcl_Ip_InitDevice(FLEXIO_INSTANCE);

    /* Initialize all pins */
    Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Initialize IRQs */
    IntCtrl_Ip_Init(&IntCtrlConfig_0);
    IntCtrl_Ip_ConfigIrqRouting(&intRouteConfig);

    /* Initialize Icu */
    Siul2_Icu_Ip_Init(SIUL2_ICU_INST_0, &Siul2_Icu_Ip_0_Config_PB_VS_0);
    Siul2_Icu_Ip_EnableInterrupt(SIUL2_ICU_INST_0, SIUL2_ICU_CHN_14);
    Siul2_Icu_Ip_EnableNotification(SIUL2_ICU_INST_0, SIUL2_ICU_CHN_14);

    /* Example for Flexio_Lin */
    /*Intialize Flexio_Lin */
    Flexio_Lin_Ip_Init(Flexio_Lin_Ip_Sa_pHwConfigPB_0_VS_0.Instance, &Flexio_Lin_Ip_Sa_pHwConfigPB_0_VS_0);
    Flexio_Lin_Ip_Init(Flexio_Lin_Ip_Sa_pHwConfigPB_1_VS_0.Instance, &Flexio_Lin_Ip_Sa_pHwConfigPB_1_VS_0);

    /* Start of the sending frame from Flexio Master*/
    (void)Flexio_Lin_Ip_SendFrame(Flexio_Lin_Ip_Sa_pHwConfigPB_0_VS_0.Instance, &LinFlexioPdu[MASTER_ROLE]);

    /*Wait for the transmission done on Master*/
    TimeoutValue = 4*T_LIN_TIME_OUT;
    do
    {
        FlexioMasterStatus = Flexio_Lin_Ip_GetStatus(Flexio_Lin_Ip_Sa_pHwConfigPB_0_VS_0.Instance, (uint8 **)&DummyBuffer);
    }
    while ((FLEXIO_LIN_IP_STATUS_TX_OK != FlexioMasterStatus) && (TimeoutValue-- > 1));

    /*Wait for the transmission done on Slave*/
    TimeoutValue = 4*T_LIN_TIME_OUT;
    do
    {
        FlexioSlaveStatus = Flexio_Lin_Ip_GetStatus(Flexio_Lin_Ip_Sa_pHwConfigPB_1_VS_0.Instance, (uint8 **)&RecvBuffer[0]);
    }
    while ((FLEXIO_LIN_IP_STATUS_RX_OK != FlexioSlaveStatus) && (TimeoutValue-- > 1));

    FlexioCheckData = CheckReceiveBuffer(LinFlexioPdu[MASTER_ROLE].SduPtr, RecvBuffer[0]);

    /* Flexio over Lin driver de-initialization*/
    Flexio_Lin_Ip_Deinit(Flexio_Lin_Ip_Sa_pHwConfigPB_0_VS_0.Instance);
    Flexio_Lin_Ip_Deinit(Flexio_Lin_Ip_Sa_pHwConfigPB_1_VS_0.Instance);

    /* Disable Flexio Module*/
    Flexio_Mcl_Ip_DeinitDevice(FLEXIO_INSTANCE);

    /* Example for Lpuart_Lin */
    
    /* Set mode Pins for Master which uses Lpuart7 - PTB14 (RX) and PTB15 (TX) */
    /* PTB15 */
    Siul2_Port_Ip_SetOutputBuffer(PORTB_L_HALF, 15, TRUE, PORT_MUX_ALT6);
    /* PTB14 */
    Siul2_Port_Ip_SetInputBuffer(PORTB_L_HALF, 14, FALSE, 175, PORT_INPUT_MUX_ALT0);
    Siul2_Port_Ip_SetInputBuffer(PORTB_L_HALF, 14, TRUE, 194, PORT_INPUT_MUX_ALT1);

    /* Set mode Pins for Slave which uses Lpuart5 - PTB28 (RX) and PTB27 (TX) */
    /* PTB27 (TX) */
    Siul2_Port_Ip_SetOutputBuffer(PORTB_H_HALF, 11, TRUE, PORT_MUX_ALT1);
    /* PTB28 (RX)*/
    Siul2_Port_Ip_SetInputBuffer(PORTB_H_HALF, 12, FALSE, 161, PORT_INPUT_MUX_ALT0);
    Siul2_Port_Ip_SetInputBuffer(PORTB_H_HALF, 12, TRUE, 30, PORT_INPUT_MUX_ALT2);

    /*Intialize Lpuart_Lin */
    Lpuart_Lin_Ip_Init(Lpuart_Lin_Ip_Sa_pHwConfigPB_0_VS_0.Instance, &Lpuart_Lin_Ip_Sa_pHwConfigPB_0_VS_0);
    Lpuart_Lin_Ip_Init(Lpuart_Lin_Ip_Sa_pHwConfigPB_1_VS_0.Instance, &Lpuart_Lin_Ip_Sa_pHwConfigPB_1_VS_0);
    /* Start timer */
    Pit_Ip_StartChannel(PIT_INST_0, PIT_CH_0, PIT_PERIOD);

    /* Start of the sending frame from Lpuart Master*/
    (void)Lpuart_Lin_Ip_SendFrame(Lpuart_Lin_Ip_Sa_pHwConfigPB_0_VS_0.Instance, &LinLpuartPdu[MASTER_ROLE]);

    /*Wait for the transmission done */
    TimeoutValue = 4*T_LIN_TIME_OUT;
    do
    {
        LpuartMasterStatus = Lpuart_Lin_Ip_GetStatus(Lpuart_Lin_Ip_Sa_pHwConfigPB_0_VS_0.Instance, (uint8 **)&DummyBuffer);
    }
    while ((LPUART_LIN_IP_STATUS_TX_OK != LpuartMasterStatus) && (TimeoutValue-- > 1));

    /*Wait for the transmission done */
    TimeoutValue = 4*T_LIN_TIME_OUT;
    do
    {
        LpuartSlaveStatus = Lpuart_Lin_Ip_GetStatus(Lpuart_Lin_Ip_Sa_pHwConfigPB_1_VS_0.Instance, (uint8 **)&RecvBuffer[1]);
    }
    while ((LPUART_LIN_IP_STATUS_RX_OK != LpuartSlaveStatus) && (TimeoutValue-- > 1));

    LpuartCheckData = CheckReceiveBuffer(LinLpuartPdu[MASTER_ROLE].SduPtr, RecvBuffer[1]);

    Pit_Ip_Deinit(PIT_INST_0);

    /* Lpuart over Lin driver de-initialization*/
    Lpuart_Lin_Ip_Deinit(Lpuart_Lin_Ip_Sa_pHwConfigPB_0_VS_0.Instance);
    Lpuart_Lin_Ip_Deinit(Lpuart_Lin_Ip_Sa_pHwConfigPB_1_VS_0.Instance);

    /* Check status */
    Exit_Example(FLEXIO_LIN_IP_STATUS_TX_OK == FlexioMasterStatus && FLEXIO_LIN_IP_STATUS_RX_OK == FlexioSlaveStatus && \
                 LPUART_LIN_IP_STATUS_TX_OK == LpuartMasterStatus && LPUART_LIN_IP_STATUS_RX_OK == LpuartSlaveStatus && \
                 TRUE == FlexioCheckData && TRUE == LpuartCheckData);

    return (0);

}

/** @} */
