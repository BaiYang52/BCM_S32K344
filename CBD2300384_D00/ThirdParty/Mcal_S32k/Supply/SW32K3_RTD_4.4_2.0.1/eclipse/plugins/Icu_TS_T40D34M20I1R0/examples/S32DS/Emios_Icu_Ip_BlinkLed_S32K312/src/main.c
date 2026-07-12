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
#include "Siul2_Port_Ip.h"
#include "Siul2_Dio_Ip.h"
#include "Emios_Icu_Ip.h"
#include "IntCtrl_Ip.h"
#include "Clock_Ip.h"


/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/


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
volatile uint8 UserCountIrqCH0;
volatile uint8 BlinkCount;

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
extern ISR(EMIOS0_5_IRQ);
void delay(void)
{
    volatile uint32 count = 4200000U;
    while(count > 0)
    {
        count--;
    }
}
void User_EdgeDetect0(void)
{
    /* increment IRQ counter */
    UserCountIrqCH0++;
    if(UserCountIrqCH0 % 2 == 0)
    {
        Siul2_Dio_Ip_WritePin(Digital_Output_LED_RED_PORT, Digital_Output_LED_RED_PIN, 1);
    }
    else
    {
        Siul2_Dio_Ip_WritePin(Digital_Output_LED_RED_PORT, Digital_Output_LED_RED_PIN, 0);
    }
}
/**
* @brief        Main function of the example
* @details      Initializez the used drivers and uses the Icu
*               and Dio drivers to toggle a LED on a push button
*/
int main(void) 
{   
    /* Initialize clock */
    Clock_Ip_InitClock(&Clock_Ip_aClockConfig[0]);

    Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Install Emios ISR */
    IntCtrl_Ip_InstallHandler(EMIOS0_5_IRQn, &EMIOS0_5_IRQ, NULL_PTR);
    IntCtrl_Ip_EnableIrq(EMIOS0_5_IRQn);

    /* Initialize the Icu driver */
    Emios_Icu_Ip_Init(0,&eMios_Icu_Ip_0_Config_PB_VS_0);
    UserCountIrqCH0 = 0U;
    BlinkCount = 0U;
    
    Emios_Icu_Ip_EnableEdgeDetection(0,1);
    Emios_Icu_Ip_EnableNotification(0,1);

    while (TRUE)
    {
        if(BlinkCount % 2 == 0)
        {
            Siul2_Dio_Ip_WritePin(Port_pin_InputForIcuChannel0_PORT, Port_pin_InputForIcuChannel0_PIN, 1);
            delay();
        }
        else
        {
            Siul2_Dio_Ip_WritePin(Port_pin_InputForIcuChannel0_PORT, Port_pin_InputForIcuChannel0_PIN, 0);
            delay();
        }
        BlinkCount++;
    }

    
    return (0U);
}


#ifdef __cplusplus
}
#endif

/** @} */
