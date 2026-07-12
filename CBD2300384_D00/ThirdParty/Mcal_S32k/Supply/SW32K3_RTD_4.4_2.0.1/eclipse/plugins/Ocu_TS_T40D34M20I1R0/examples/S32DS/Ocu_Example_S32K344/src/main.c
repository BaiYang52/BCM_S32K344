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
#include "Ocu.h"
#include "Port.h"
#include "Dio.h"
#include "Mcu.h"
#include "Platform.h"
#include "Mcl.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

#define FXOSC_CLOCK_FREQ   16000000U

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
#if (STD_ON == OCU_NOTIFICATION_SUPPORTED)
void OcuChannel_0_notification(void);
#endif /* OCU_NOTIFICATION_SUPPORTED */

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
#if (STD_ON == OCU_NOTIFICATION_SUPPORTED)
    
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

#endif /* OCU_NOTIFICATION_SUPPORTED */
/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/


/**
* @brief        Main function of the example
* @details      Initializez the used drivers and uses the Ocu
*               and Dio drivers to toggle a LED periodically
*/

int main(void)
{
    uint8 stateLed = 0;

    /* Init clock */
#if (STD_ON == MCU_PRECOMPILE_SUPPORT)
    Mcu_Init(NULL_PTR);
#else
    Mcu_Init(&Mcu_Config_VS_0);
#endif

#if (STD_ON == MCU_INIT_CLOCK)
    /* Initialize the clock tree and apply PLL as system clock */
    Mcu_InitClock(McuClockSettingConfig_0);

#if (STD_OFF == MCU_NO_PLL)
    while ( MCU_PLL_LOCKED != Mcu_GetPllStatus() )
    {
        /* Busy wait until the System PLL is locked */
    }
    Mcu_DistributePllClock();
#endif

    Mcu_SetMode(McuModeSettingConf_0);

#else
    #error "The Mcu Init Clock API should be enabled from the Mcu driver"
#endif

    /* if (FXOSC_CLOCK_FREQ == Mcu_GetClockFrequency(FXOSC_CLK) ) */
    {
        /* Initialize all pins using the Port driver */
#if (STD_ON == PORT_PRECOMPILE_SUPPORT)
        Port_Init(NULL_PTR);
#else
        Port_Init(&Port_Config_VS_0);
#endif

        /* set eMios interupt */
        Platform_Init(NULL_PTR);

        /* init eMios Mcl for enable global counter bus */
        Mcl_Init(&Mcl_Config_VS_0);

        /* Initialize the high level configuration structure of Ocu driver */
#if (STD_ON == OCU_PRECOMPILE_SUPPORT)
        Ocu_Init(NULL_PTR);
#else
        Ocu_Init(&Ocu_Config_VS_0);
#endif

        /* Start eMios Ocu Channel */
        Ocu_StartChannel(OcuChannel_0);
        
        /* Enable eMios Ocu channel notification */
        Ocu_EnableNotification(OcuChannel_0);

        while (1)
        {
            /* Toggle the LED when the Ocu notification is called */
            if (1U == toggleLed)
            {
                toggleLed = 0;
                Dio_WriteChannel(DioConf_DioChannel_DioChannel_0, stateLed);
                stateLed = stateLed ^ 1U;
            }
        }

        /* Exit_Example(TRUE); */
    }

    /* Exit_Example(FALSE); */
}

/** @} */
