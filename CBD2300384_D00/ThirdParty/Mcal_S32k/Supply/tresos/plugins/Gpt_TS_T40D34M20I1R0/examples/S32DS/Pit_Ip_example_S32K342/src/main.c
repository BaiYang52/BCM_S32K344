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
#include "Pit_Ip.h"
#include "Clock_Ip.h"
#include "IntCtrl_Ip.h"
#include "Siul2_Port_Ip.h"
#include "Siul2_Dio_Ip.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
/* PIT instance used - 0 */
#define PIT_INST_0 0U
/* PIT Channel used - 0 */
#define CH_0 0U
/* PIT time-out period - equivalent to 1s */
#define PIT_PERIOD 40000000

#define clockConfig &Clock_Ip_aClockConfig[0]

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
/* Global flag updated in interrupt */
volatile uint8 toggleLed = 0U;

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
* @brief        Pit notification called by the configured channel periodically
* @details      Used to blink a led
*/
void PitNotification(void)
{
    toggleLed = 1U;
}


/**
* @brief        Main function of the example
* @details      Initialize the used drivers and uses the Pit
*               and Dio drivers to toggle a LED periodically
*/
int main (void)
{
    /* Initial Clock */
    Clock_Ip_Init(clockConfig);
    /*Initial Pin */
    Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
    /*Initial ISR*/
    IntCtrl_Ip_Init(&IntCtrlConfig_0);

    IntCtrl_Ip_ConfigIrqRouting(&intRouteConfig);
    /*Initial PIT instance 0 - Channel 0*/
    Pit_Ip_Init(PIT_INST_0, &PIT_0_InitConfig_PB);
    /*Initial channel 0 */
    Pit_Ip_InitChannel(PIT_INST_0, PIT_0_CH_0);
    /*Enable channel interrupt PIT_0 - CH_0*/
    Pit_Ip_EnableChannelInterrupt(PIT_INST_0, CH_0);
    /*Start channel CH_0*/
    Pit_Ip_StartChannel(PIT_INST_0, CH_0, PIT_PERIOD);
    /*Waiting for Interrupt occurred */
    while (1)
    {
        /* Toggle the LED when the Pit notification is called */
        if (toggleLed == 1U)
        {
            /*Led toggle - blink a led*/
            Siul2_Dio_Ip_TogglePins(LED_PORT, (1<<LED_PIN));

            toggleLed = 0U;
        }
    }
    return 0;
}


#ifdef __cplusplus
}
#endif

/** @} */
