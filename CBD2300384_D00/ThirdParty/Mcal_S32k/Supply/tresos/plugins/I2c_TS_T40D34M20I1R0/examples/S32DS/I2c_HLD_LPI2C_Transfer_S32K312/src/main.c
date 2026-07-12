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
#include "Mcu.h"
#include "Platform.h"
#include "CDD_I2c.h"
#include "sys_init.h"
#include "Lpi2c_Ip_Irq.h"
#include "Port.h"
#include "check_example.h"
#include "OsIf.h"

volatile int exit_code = 0;

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/**
* @brief        Define transfer size
*/
#define TRANSFER_SIZE 8U

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

I2c_DataType rxBufferSlave[TRANSFER_SIZE] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
I2c_DataType txBuffer[TRANSFER_SIZE] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7};
I2c_RequestType pRequestSend = 
{
    /* Slave address */
    0x32, 
    
    /*10 bit address*/
    FALSE,
    
    /*high speed */
    FALSE,
    
    /* expect Nack */
    FALSE, 
    
    /*repeated start */
    FALSE, 
    
    /*buffer size */
    8U, 
    
    /*Data direction */
    I2C_SEND_DATA,
    
    /*Buffer*/
    txBuffer    
};

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/**
* @brief        Slave callback for i2c
* @details      Prepare slave's buffer for transfer
*/
void I2c_Callback(uint8 u8Event, uint8 u8Channel)
{
    switch(u8Event)
    {
        case I2C_EVENT_RX_REQ_SLAVE:
            I2c_PrepareSlaveBuffer(u8Channel, TRANSFER_SIZE, rxBufferSlave);
            break;
        
        case I2C_EVENT_TX_REQ_SLAVE:
            I2c_PrepareSlaveBuffer(u8Channel, TRANSFER_SIZE, txBuffer);
            break;
    
        default:
            (void) u8Channel;
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
    uint32 timeout = 0xFFFF;
    boolean checkOk = TRUE;
    uint8 i;
    I2c_StatusType status;
    
    /* Init system clock */
    Mcu_Init(NULL_PTR);

    /* Initialize the clock tree and apply PLL as system clock */
    Mcu_InitClock(McuClockSettingConfig_0);
    while ( MCU_PLL_LOCKED != Mcu_GetPllStatus() )
    {
      /* Busy wait until the System PLL is locked */
    }

    Mcu_DistributePllClock();
    Mcu_SetMode(McuModeSettingConf_0);

    /* Initialize Port driver */
    Port_Init(NULL_PTR);

    /* Initialize Interrupt */
    Platform_Init(NULL_PTR);

    /* Init master and slave */
    I2c_Init(NULL_PTR);

    I2c_StartListening(1U);
    /* Master send data */
    I2c_AsyncTransmit(0U, &pRequestSend);

    /* Wait until master is ready */
    while(((status = I2c_GetStatus(0U)) == I2C_CH_SEND) && (timeout > 0))
    {
        timeout--;
    }

    /* Wait until slave is ready */
    while(((status = I2c_GetStatus(1U)) == I2C_CH_RECEIVE) && (timeout > 0))
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
