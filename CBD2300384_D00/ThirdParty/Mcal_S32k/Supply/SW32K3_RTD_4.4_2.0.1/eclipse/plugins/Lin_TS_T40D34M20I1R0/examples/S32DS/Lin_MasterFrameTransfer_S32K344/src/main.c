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
#include "Lin.h"
#include "Port.h"
#include "Platform.h"
#include "OsIf.h"
#if (LIN_FLEXIO_USED == STD_ON)
#include "Mcl.h"
#endif
#include "check_example.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define LIN_MASTER_RESPONSE LIN_FRAMERESPONSE_TX
#define T_LIN_TEST_NUM_CHANNELS ((uint8)2U)
#define T_LIN_TEST_FRM_NUM      ((uint8)1U)
#define T_LIN_TIME_OUT 400000

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/
uint8 T_Lin_Data[3] = {0x01, 0x02, 0x03};
const Lin_PduType T_Lin_Frame[T_LIN_TEST_FRM_NUM] =
{
    {0x1A, LIN_ENHANCED_CS, LIN_MASTER_RESPONSE, 3u, T_Lin_Data}
};
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
/**
* @brief        Main function of the example
* @details      Initializez the used drivers and uses the Gpt
*               and Dio drivers to toggle a LED periodically
*/
int main(void) 
{
    static uint8 linSdu[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    volatile Lin_StatusType T_LinStatus;
    volatile uint32 TimeOut;
    uint8 Index_frame = 0;
    Lin_PduType linFrame;
    uint8* linSduPtr = linSdu;
    volatile uint8 T_result = 0;
    uint8 Channel_Index = 0;
    volatile Std_ReturnType wakeUpEvent;

    /* Initialize all pins */
    Port_Init(NULL_PTR);
    /* Setup Clocks */
    /* Initialize Mcu module */
    Mcu_Init(NULL_PTR);

    /* Initialize Mcu clock */
    Mcu_InitClock(McuClockSettingConfig_0);

    while (Mcu_GetPllStatus() != MCU_PLL_LOCKED){};

    /* Use PLL clock */
    Mcu_DistributePllClock();

    Mcu_SetMode(McuModeSettingConf_0);
#if (LIN_FLEXIO_USED == STD_ON)
    Mcl_Init(NULL_PTR);
#endif
    /* Initialize Mcu module to enable interrupts and install handler */
    Platform_Init(NULL_PTR);

    /* Init osif */
    OsIf_Init(NULL_PTR);

    /* Initialize LIN module */
    Lin_Init(NULL_PTR);

    for (Channel_Index = 0; Channel_Index < T_LIN_TEST_NUM_CHANNELS; Channel_Index++)
    {
        /* Wakeup LIN channels for ASR 4.3.1 */
        Lin_WakeupInternal(Channel_Index);
        /* Check Lin channel wakeup */
        wakeUpEvent = Lin_CheckWakeup(Channel_Index);
        
        /* Get frame data */
        linFrame = T_Lin_Frame[Index_frame];

        /* Send LIN frame */
        Lin_SendFrame(Channel_Index, &linFrame);

        /* Wait for the transmission done */
        TimeOut = 4*T_LIN_TIME_OUT;
        do
        {
            T_LinStatus = Lin_GetStatus(Channel_Index, &linSduPtr);
        } while (LIN_TX_OK != T_LinStatus && 0 != TimeOut--);

        /* Verify that the LIN frame is sent successfully */
        if (LIN_TX_OK == T_LinStatus)
        {
            T_result = T_result + 1;
        }
    }

    Exit_Example((E_OK == wakeUpEvent)&&((T_LIN_TEST_FRM_NUM * T_LIN_TEST_NUM_CHANNELS) == T_result));

    return (0U);
}


#ifdef __cplusplus
}
#endif

/** @} */
