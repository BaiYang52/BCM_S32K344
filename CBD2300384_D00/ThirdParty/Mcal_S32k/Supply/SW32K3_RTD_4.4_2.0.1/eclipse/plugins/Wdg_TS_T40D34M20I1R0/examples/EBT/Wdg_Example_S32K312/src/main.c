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
#include "Gpt.h"
#include "Wdg_43_Instance0.h"
#include "Platform.h"

#include "check_example.h"

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
#define NUM_OF_NOTIFICATIONS         10U
#define NUM_OF_SETTRIGGER         10U
/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
volatile uint8 u8NotificationCount = 0U;
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
* @brief        Wdg callback notification called by the configured channel periodically
* @details      Increments the counter
*/
void Wdg_CallBackNotification0(void)
{
    u8NotificationCount = 1U;
}

/**
* @brief        Delay function
* @details      Delay function to test the Wdg Trigger
*/
void delay(uint32 timeout)
{
    volatile uint32 i = timeout;
    
    while(i--);
}

/**
* @brief        PIT_0 Interrupt handler
* @details      Defined in Pit_Ip.c
*/
extern void PIT_0_ISR(void);

/**
* @brief        SWT_0 Interrupt handler
* @details      Defined in Swt_Ip_Irq.c.c
*/
extern void Swt_Ip_Swt0_Isr(void);

/**
* @brief        Main function of the example
* @details      Initializez the used drivers and uses Wdg driver
*/

int main(void)
{
    uint8 count = 0U;

    /* Initialize the Mcu driver */
    Mcu_Init(NULL_PTR);
    /* Initialize the clock tree and apply PLL as system clock */
    Mcu_InitClock(McuClockSettingConfig_0);
    while (MCU_PLL_LOCKED != Mcu_GetPllStatus())
    {
        /* Busy wait until the System PLL is locked */
    }
    Mcu_DistributePllClock();
    Mcu_SetMode(McuModeSettingConf_0);

    /* Initialize Platform driver */
    Platform_Init(NULL_PTR);
    /* Install PIT0 ISR */
    Platform_InstallIrqHandler(PIT0_IRQn, &PIT_0_ISR, NULL_PTR);
    Platform_SetIrq(PIT0_IRQn, TRUE);
    /* Install SWT0 ISR */
    Platform_InstallIrqHandler(SWT0_IRQn, &Swt_Ip_Swt0_Isr, NULL_PTR);
    Platform_SetIrq(SWT0_IRQn, TRUE);
    
    /* Initialize the Gpt driver */
    Gpt_Init(NULL_PTR);
    /* Enable the Gpt notification to periodically service the Wdg */
    Gpt_EnableNotification(GptConf_GptChannelConfiguration_GptChannelConfiguration_0);

    /* Initialize the Wdg driver */
    Wdg_43_Instance0_Init(&Wdg_Config_0_VS_0);
    /* Set the Wdg Trigger Condition in order to periodically service the Wdg */
    Wdg_43_Instance0_SetTriggerCondition(0xFD0);
    /*Change the Wdg mode of operation */
    Wdg_43_Instance0_SetMode(WDGIF_FAST_MODE);
    /* Set the Wdg Trigger Condition in order to periodically service the Wdg */
    Wdg_43_Instance0_SetTriggerCondition(0xFD0);

    /* Test that the Wdg will not expire while it is services */
    for (uint8 index = 0; index < NUM_OF_SETTRIGGER; index++)
    {
        delay(50000U);
        Wdg_43_Instance0_SetTriggerCondition(0xFD0);
    }

    while (count < NUM_OF_NOTIFICATIONS)
    {
        /* Set the Operation Mode and the Trigger Condition after the Wdg notification is called */
        if (u8NotificationCount == 1U)
        {
            /* Change the Wdg mode of operation in order to restart the timer in charge of servicing the Wdg */
            Wdg_43_Instance0_SetMode(WDGIF_FAST_MODE);
            Wdg_43_Instance0_SetTriggerCondition(0x4D0);
            count++;
            u8NotificationCount = 0;
        }
    }

    /* Set the Wdg Trigger Condition in order to periodically service the Wdg */
    Wdg_43_Instance0_SetTriggerCondition(0xFD0);
    /* Disable the Wdg driver by changing the mode of operation to OFF_MODE */
    Wdg_43_Instance0_SetMode(WDGIF_OFF_MODE);
    
    Exit_Example(TRUE);
    
    return (0U);
}


#ifdef __cplusplus
}
#endif

/** @} */
