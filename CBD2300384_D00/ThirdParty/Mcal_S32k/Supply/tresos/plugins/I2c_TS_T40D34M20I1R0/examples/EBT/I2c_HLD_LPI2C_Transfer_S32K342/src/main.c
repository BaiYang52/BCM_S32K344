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
#include "Mcu.h"
#include "Platform.h"
#include "Port.h"
#include "CDD_I2c.h"
#include "sys_init.h"
#include "Lpi2c_Ip_Irq.h"

#include "check_example.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define TRANSFER_SIZE 8U

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


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
int main(void);

/*==================================================================================================
*                                       LOCAL FUNCTIONS
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
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


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

void Init_Pins(void)
{
    /* LPI2C0_SDA - PTC9 */

    IP_SIUL2->MSCR[73] = SIUL2_MSCR_OBE(1)|SIUL2_MSCR_IBE(1)|SIUL2_MSCR_SSS_0(1)|SIUL2_MSCR_SSS_1(0)|SIUL2_MSCR_SSS_2(0);
    IP_SIUL2->IMCR[214] = 1;



    /* LPI2C0_SCL - PTC8 */
    IP_SIUL2->MSCR[72] = SIUL2_MSCR_OBE(1)|SIUL2_MSCR_IBE(1)|SIUL2_MSCR_SSS_0(1)|SIUL2_MSCR_SSS_1(0)|SIUL2_MSCR_SSS_2(0);
    IP_SIUL2->IMCR[212] = 1;
    /* LPI2C1_SDA - PTC 29 */
    IP_SIUL2->MSCR[93] = SIUL2_MSCR_OBE(1)|SIUL2_MSCR_IBE(1)|SIUL2_MSCR_SSS_0(1)|SIUL2_MSCR_SSS_1(0)|SIUL2_MSCR_SSS_2(1);
    IP_SIUL2->IMCR[219] = 3;



    /* LPI2C1_SCL PTC 28 */
    IP_SIUL2->MSCR[92] = SIUL2_MSCR_OBE(1)|SIUL2_MSCR_IBE(1)|SIUL2_MSCR_SSS_0(1)|SIUL2_MSCR_SSS_1(0)|SIUL2_MSCR_SSS_2(1);
    IP_SIUL2->IMCR[217] = 4;

}
/**
* @brief        Main function of the example
* @details      Initializez the LPI2C driver to asynchronous data.
*/
int main(void) 
{
    volatile uint32 timeout = 0xFFFF;
    boolean checkOk = TRUE;
    uint8 i;
    volatile I2c_StatusType status;

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
    I2c_Init(&I2c_Config_VS_0);
    
    /* Slave recieve data */
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

    Exit_Example(checkOk);

    return (0U);
}


#ifdef __cplusplus
}
#endif

/** @} */
