/*
*   (c) Copyright 2020 NXP
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be used strictly
*   in accordance with the applicable license terms. By expressly accepting
*   such terms or by downloading, installing, activating and/or otherwise using
*   the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to
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
#include "Flexio_Pwm_Ip.h"
#include "Flexio_Mcl_Ip.h"
#include "Siul2_Port_Ip.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define NUM_BLINK_LED                   (uint32)10U
#define DELAY_TIMER                     (uint32)5000000U
#define INSTANCE_0                      (uint8)0U
#define FLEXIO_PWM_IP_CHANNEL_0         (uint8)0U
/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/

extern const Flexio_Ip_InstanceConfigType Flexio_Ip_xFlexioInit;
/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

void TestDelay(uint32 delay);
/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
void TestDelay(uint32 delay)
{
    static volatile uint32 DelayTimer = 0;
    while(DelayTimer<delay)
    {
        DelayTimer++;
    }
    DelayTimer=0;
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/**
* @brief        Main function of the example
* @details      Initialize the used drivers and uses the Pwm driver to control the led brightness
*               and blink a LED
*/
int main (void)
{
    uint8 num_blink = 0U;
    /* Initialize clock () */
    Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);

    /* Initialize all pins using the Port driver */
    Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Initialize Flexio_Mcl driver */
    Flexio_Mcl_Ip_InitDevice(&Flexio_Ip_xFlexioInit);

    /* Initialize Flexio_Pwm_Ip driver */
    Flexio_Pwm_Ip_InitChannel(FLEXIO_PWM_IP_VS_0_I0_CH0_CFG, &Flexio_Pwm_Ip_VS_0_I0_Ch0);

    /* Setup new period and duty */
    Flexio_Pwm_Ip_UpdatePeriodDuty(INSTANCE_0, FLEXIO_PWM_IP_CHANNEL_0, 240U, 240U);

    /* Setup new duty cycle to decreasing LED brightness */
    Flexio_Pwm_Ip_UpdatePeriodDuty(INSTANCE_0, FLEXIO_PWM_IP_CHANNEL_0, 240U, 40U);

    /* Setup new duty cycle to increasing LED brightness */
    Flexio_Pwm_Ip_UpdatePeriodDuty(INSTANCE_0, FLEXIO_PWM_IP_CHANNEL_0, 240U, 100U);

    /* Using duty 0% and 100% to blink led */
    while(num_blink < NUM_BLINK_LED)
    {
        /* Led on when duty cycle is 100% */
        Flexio_Pwm_Ip_UpdatePeriodDuty(INSTANCE_0, FLEXIO_PWM_IP_CHANNEL_0, 240U, 240U);
        TestDelay(DELAY_TIMER);

        /* Led off when duty cycle is 0% */
        Flexio_Pwm_Ip_UpdatePeriodDuty(INSTANCE_0, FLEXIO_PWM_IP_CHANNEL_0, 240U, 0U);
        TestDelay(DELAY_TIMER);
        num_blink++;
    }

    /* De-Initialize flexio_Pwm_Ip driver */
    Flexio_Pwm_Ip_DeInitChannel(INSTANCE_0, FLEXIO_PWM_IP_CHANNEL_0);
    
    return 0U;
}

#ifdef __cplusplus
}
#endif

/** @} */
