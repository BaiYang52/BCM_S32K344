/*==================================================================================================
*   Project              : M4_SRC_PROJECT_NAME
*   Platform             : M4_SRC_MCU_FAMILY
*   Peripheral           : M4_SRC_USED_PERIPHERAL
*   Dependencies         : M4_SRC_AR_MODULE_DEPENDENCY
*
*   Autosar Version      : M4_SRC_AR_SPEC_VERSION_MAJOR.M4_SRC_AR_SPEC_VERSION_MINOR.M4_SRC_AR_SPEC_VERSION_PATCH
*   Autosar Revision     : M4_SRC_AR_RELEASE_REVISION
*   Autosar Conf.Variant :
*   SW Version           : M4_SRC_SW_VERSION_MAJOR.M4_SRC_SW_VERSION_MINOR.M4_SRC_SW_VERSION_PATCH
*   Build Version        : M4_SRC_BUILD_ID
*
*   (c) Copyright M4_SRC_YEAR_ID M4_SRC_COPYRIGHTED_TO
*   All Rights Reserved.
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

/**
*   @file main.c
*
*   @addtogroup main_module main module documentation
*   @{
*/

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Emios_Ocu_Ip.h"
#include "Emios_Mcl_Ip.h"
#include "Emios_Mcl_Ip_Irq.h"
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
#define clockConfig &Clock_Ip_aClockConfig[0]
#define EMIOS_0       (0U)
#define CHANNEL_1     (1U)

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
/* Global flag updated in irq */
static volatile uint8 toggleLed = 0U;
/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

void OcuChannel_0_notification(void);
/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

/**
* @brief    Emios notification periodically called by the configured channel
* @details  Used to blink a led
*/
void OcuChannel_0_notification()
{
    static uint16 count = 0U;
    if(++count >= 100)
    {
        toggleLed = 1U;
        count = 0;
    }
}


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/**
* @brief        Main function of the example
* @details      Initializez the used drivers and uses the Ocu
*               and Siul2_Dio drivers to toggle a LED periodically
*/

int main(void)
{
    uint8 stateLed = 0;

    /* Init clock */
    Clock_Ip_Init(clockConfig);
    /* Initialize all pins using the Siul2_Port driver */
    Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS0,g_pin_mux_InitConfigArr0);

    /*set EMIOS interupt*/
    IntCtrl_Ip_Init(&IntCtrlConfig_0);
    IntCtrl_Ip_ConfigIrqRouting(&intRouteConfig);

    /*init eMios_Mcl for enable global counter bus*/
    Emios_Mcl_Ip_Init(EMIOS_0, &Emios_Mcl_Ip_0_Config_VS_0);
    
    /*init eMios_Ocu*/
    Emios_Ocu_Ip_Init(&Emios_Ocu_Ip_ModuleCfgPB_VS_0);
    /*Start eMios_Ocu Channel*/
    Emios_Ocu_Ip_StartChannel(EMIOS_0,CHANNEL_1);
    /*Enable eMios_Ocu channel notification*/
    Emios_Ocu_Ip_EnableNotification(EMIOS_0,CHANNEL_1);

    while (1)
    {
        /* Toggle the LED when the Ocu notification is called */
        if (1U == toggleLed)
        {
            toggleLed = 0;
            Siul2_Dio_Ip_WritePin(LED_PORT, LED_PIN, stateLed);
            stateLed = stateLed ^ 1U;
        }
    }
}

/** @} */