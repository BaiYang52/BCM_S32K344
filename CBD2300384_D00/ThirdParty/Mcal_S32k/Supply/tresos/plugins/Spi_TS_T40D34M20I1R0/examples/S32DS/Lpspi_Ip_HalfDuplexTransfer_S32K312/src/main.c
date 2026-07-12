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
#include "Siul2_Port_Ip.h"
#include "Lpspi_Ip.h"
#include "IntCtrl_Ip.h"
#include "check_example.h"
/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define NUMBER_OF_BYTES             (10u)
#define TIMEOUT                     ((uint32)1000000u)

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/
/* LPSPI 1 is used as master */
#define MASTER_EXTERNAL_DEVICE (Lpspi_Ip_DeviceAttributes_SpiExternalDevice_Master_Instance_0_BOARD_InitPeripherals)
#define SLAVE_EXTERNAL_DEVICE (Lpspi_Ip_DeviceAttributes_SpiExternalDevice_Slave_Instance_2_BOARD_InitPeripherals)

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
#define SPI_START_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Spi_MemMap.h"
uint8 TxMasterBuffer[NUMBER_OF_BYTES] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
uint8 RxSlaveBuffer[NUMBER_OF_BYTES];
#define SPI_STOP_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Spi_MemMap.h"
/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
/**
* @brief        Main function of the example
*/
int main(void)
{
    uint8 Index;
    uint32 Counter = 0xFFFFFF;
    Lpspi_Ip_HwStatusType Status;
    volatile boolean Passed = TRUE;

    /* Initalized clock */
    Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);
    /* Initalized pin */
    Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
    /* Initialize interrupt */
    IntCtrl_Ip_Init(&IntCtrlConfig_0);
    IntCtrl_Ip_ConfigIrqRouting(&intRouteConfig);

    /* Initalized spi using instance lpspi as a spi slave */
    Lpspi_Ip_Init(&Lpspi_Ip_PhyUnitConfig_SpiPhyUnit_0_Instance_0_BOARD_InitPeripherals);
    Lpspi_Ip_Init(&Lpspi_Ip_PhyUnitConfig_SpiPhyUnit_1_Instance_2_BOARD_InitPeripherals);
    /* Update Transfer Mode */
    Lpspi_Ip_UpdateTransferMode(SLAVE_EXTERNAL_DEVICE.Instance, LPSPI_IP_INTERRUPT);

    /* Slave set up and waiting active signal of Cs from master to process data reception */
    Lpspi_Ip_AsyncTransmitHalfDuplex(&SLAVE_EXTERNAL_DEVICE, RxSlaveBuffer, NUMBER_OF_BYTES, LPSPI_IP_HALF_DUPLEX_RECEIVE, NULL_PTR);

    /* Master transmits data in half-duplex mode by sync method */
    Lpspi_Ip_SyncTransmitHalfDuplex(&MASTER_EXTERNAL_DEVICE, TxMasterBuffer, NUMBER_OF_BYTES, LPSPI_IP_HALF_DUPLEX_TRANSMIT, TIMEOUT);

    do
    {
        Counter --;
        Status = Lpspi_Ip_GetStatus(SLAVE_EXTERNAL_DEVICE.Instance);
    }
    while ( (Counter > 0UL) && (Status != LPSPI_IP_IDLE) );

    if (0UL != Counter)
    {
        /* Checking data */
        for (Index = 0; Index < NUMBER_OF_BYTES; Index++)
        {
            if (TxMasterBuffer[Index] != RxSlaveBuffer[Index])
            {
                Passed = FALSE;
                break;
            }
        }
    }
    else
    {
        Passed = FALSE;
    }

    Exit_Example(Passed);

    return 0;
}
#ifdef __cplusplus
}
#endif

/** @} */
