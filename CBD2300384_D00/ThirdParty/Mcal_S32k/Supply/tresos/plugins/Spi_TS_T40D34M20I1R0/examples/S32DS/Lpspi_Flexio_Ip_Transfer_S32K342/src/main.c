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
#include "Flexio_Mcl_Ip.h"
#include "Flexio_Spi_Ip.h"
#include "Dma_Ip.h"
#include "IntCtrl_Ip.h"
#include "check_example.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
/* Time to transfer all frame data */
#define TIMEOUT             ((uint32)1000000UL)
#define NUMBER_OF_BYTES     (10)

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/
#define SLAVE_EXTERNAL_DEVICE          	 (Lpspi_Ip_DeviceAttributes_SpiExternalDevice_Slave_Instance_2_BOARD_InitPeripherals)
#define MASTER_EXTERNAL_DEVICE           (Flexio_Spi_Ip_DeviceAttributes_SpiExternalDevice_Master_Instance_0_BOARD_InitPeripherals)

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
#define SPI_START_SEC_VAR_INIT_8_NO_CACHEABLE
#include "Spi_MemMap.h"
uint8 TxSlaveBuffer[NUMBER_OF_BYTES] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19};
uint8 RxSlaveBuffer[NUMBER_OF_BYTES];
#define SPI_STOP_SEC_VAR_INIT_8_NO_CACHEABLE
#include "Spi_MemMap.h"

#define SPI_START_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Spi_MemMap.h"
uint8 TxMasterBuffer[NUMBER_OF_BYTES] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
uint8 RxMasterBuffer[NUMBER_OF_BYTES];
#define SPI_STOP_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Spi_MemMap.h"

/**
* @brief        Main function of the example
*/
int main(void)
{
    /* Write your code here */
    uint8 Count = 0;
    boolean Passed = TRUE;

    /* Initialize clock */
    Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);

    /* Configure pins */
    Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Initialize Dma */
    Dma_Ip_Init(&Dma_Ip_xDmaInitPB);
    /* Initialize interrupt */
    IntCtrl_Ip_Init(&IntCtrlConfig_0);
    IntCtrl_Ip_ConfigIrqRouting(&intRouteConfig);

    Flexio_Mcl_Ip_InitDevice(&Flexio_Ip_xFlexioInit);

    /* Initialize each Spi hardware units using*/
    Lpspi_Ip_Init(&Lpspi_Ip_PhyUnitConfig_SpiPhyUnit_0_Instance_2_BOARD_InitPeripherals);
    Flexio_Spi_Ip_Init(&Flexio_Spi_Ip_PhyUnitConfig_SpiPhyUnit_0_Instance_0_BOARD_InitPeripherals);

    /* Update Framesize */
    Lpspi_Ip_UpdateFrameSize(&SLAVE_EXTERNAL_DEVICE, 8U);
    Flexio_Spi_Ip_UpdateFrameSize(&MASTER_EXTERNAL_DEVICE, 8U);

    /* Update Transfered Bit order */
    Lpspi_Ip_UpdateLsb(&SLAVE_EXTERNAL_DEVICE, FALSE);
    Flexio_Spi_Ip_UpdateLsb(&MASTER_EXTERNAL_DEVICE, FALSE);

    /* Update Transfer Mode */
    Lpspi_Ip_UpdateTransferMode(SLAVE_EXTERNAL_DEVICE.Instance, LPSPI_IP_INTERRUPT);

    /* Slave starts transfer, synchronous method is used */
    Lpspi_Ip_AsyncTransmit(&SLAVE_EXTERNAL_DEVICE, TxSlaveBuffer, RxSlaveBuffer, NUMBER_OF_BYTES, NULL_PTR);

    /* Master starts transfer, async method is used */
    Flexio_Spi_Ip_SyncTransmit(&MASTER_EXTERNAL_DEVICE, TxMasterBuffer, RxMasterBuffer, NUMBER_OF_BYTES, TIMEOUT);

    for (Count = 0u; Count < 10u; Count++)
    {
        if (TxMasterBuffer[Count] != RxSlaveBuffer[Count])
        {
            Passed = FALSE;
            break;
        }

        if (RxMasterBuffer[Count] != TxSlaveBuffer[Count])
        {
            Passed = FALSE;
            break;
        }
    }

    Exit_Example(Passed);

    return 0;
}

#ifdef __cplusplus
}
#endif

/** @} */
