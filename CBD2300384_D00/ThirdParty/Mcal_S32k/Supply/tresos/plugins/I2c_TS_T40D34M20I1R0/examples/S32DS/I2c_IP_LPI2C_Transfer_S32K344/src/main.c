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


#ifdef __cplusplus
extern "C" {
#endif

/* Including necessary configuration files. */
#include "Mcal.h"
#include "Clock_Ip.h"
#include "IntCtrl_Ip.h"
#include "Siul2_Port_Ip.h"
#include "Siul2_Port_Ip_Cfg.h"
#include "Lpi2c_Ip.h"
#if defined(S32K312)
#include "S32K312_SIUL2.h"
#endif
#if defined(S32K342)
#include "S32K342_SIUL2.h"
#endif
#if defined(S32K344)
#include "S32K344_SIUL2.h"
#endif
#include "Lpi2c_Ip_Irq.h"
#include "Lpi2c_Ip_VS_0_PBcfg.h"
#include "check_example.h"

volatile int exit_code = 0;


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/**
* @brief        Define transfer size
*/
#define TRANSFER_SIZE 8U

/**
* @brief        Define I2C instances
*/
#define I2C_SLAVE    1U
#define I2C_MASTER   0U

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/**
* @brief        Define data buffers
*/
uint8 rxBufferSlave[TRANSFER_SIZE] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
uint8 txBuffer[TRANSFER_SIZE] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7};

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    uint32 timeout = 0xFFFF;
	boolean checkOk = TRUE;
    uint8 i;

    /* Init clock  */
    Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);
	
	/* Enable LPI2C interrupts */
    IntCtrl_Ip_Init(&IntCtrlConfig_0);
    IntCtrl_Ip_ConfigIrqRouting(&intRouteConfig);
	
	/* Init Pins */
	Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
	
	/* Init master module */
	Lpi2c_Ip_MasterInit(I2C_MASTER, &I2c_Lpi2cMasterChannel0_VS_0);
	
	/* Init slave module */
	Lpi2c_Ip_SlaveInit(I2C_SLAVE, &I2c_Lpi2cSlaveChannel1_VS_0);

	/* Set slave buffer */
	Lpi2c_Ip_SlaveSetBuffer(I2C_SLAVE, rxBufferSlave, TRANSFER_SIZE);
	
	/* Send data non-blocking */
	Lpi2c_Ip_MasterSendData(I2C_MASTER, txBuffer, TRANSFER_SIZE, TRUE);

	/* Wait master transfer to finish */
	while(((Lpi2c_Ip_MasterGetTransferStatus(I2C_MASTER, NULL_PTR)) == LPI2C_IP_BUSY_STATUS) && (timeout > 0))
	{
		timeout--;
	}
	
	/* Wait slave module to finish */
	while(((Lpi2c_Ip_SlaveGetTransferStatus(I2C_SLAVE, NULL_PTR)) == LPI2C_IP_BUSY_STATUS) && (timeout > 0))
	{
		timeout--;
	}

    /* Verify data received */
    for (i = 0; i < TRANSFER_SIZE; i ++)
    {
        if( txBuffer[i] != rxBufferSlave[i])
        {
        	checkOk = FALSE;
        }
    }

    /* Check if transfer was successful */
    Exit_Example(checkOk);


    for(;;)
    {
        if(exit_code != 0)
        {
            break;
        }
    }
    return exit_code;
}

#ifdef __cplusplus
}
#endif


/** @} */
