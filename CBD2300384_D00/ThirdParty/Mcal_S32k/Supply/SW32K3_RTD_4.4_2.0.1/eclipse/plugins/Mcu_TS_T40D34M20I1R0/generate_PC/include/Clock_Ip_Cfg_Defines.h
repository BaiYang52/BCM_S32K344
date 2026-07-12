/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : 
*   Dependencies         : none
*
*   Autosar Version      : 4.4.0
*   Autosar Revision     : ASR_REL_4_4_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 2.0.1
*   Build Version        : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
*   (c) Copyright 2020 - 2022 NXP Semiconductors
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

#ifndef CLOCK_IP_CFG_DEFINES_H
#define CLOCK_IP_CFG_DEFINES_H

/**
*   @file       Clock_Ip_Cfg_Defines.h
*   @version    2.0.1
*
*   @brief   AUTOSAR Mcu - Post-Build(PB) configuration file code template.
*   @details Code template for Post-Build(PB) configuration file generation.
*
*   @addtogroup CLOCK_DRIVER_CONFIGURATION Clock Ip Driver
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
[!NOCODE!][!// Include specific header file 
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!// 
    [!IF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k312')"!][!// 
[!CODE!][!WS "0"!]#include "S32K312_MC_CGM.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K312_FIRC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K312_SIRC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K312_FXOSC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K312_SXOSC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K312_PLL.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K312_MC_ME.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K312_PRAMC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K312_FLASH.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K312_CMU_FC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K312_SYSTICK.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K312_EMIOS.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K312_RTC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K312_CONFIGURATION_GPR.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k314')"!][!//
[!CODE!][!WS "0"!]#include "S32K314_MC_CGM.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K314_FIRC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K314_SIRC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K314_FXOSC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K314_SXOSC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K314_PLL.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K314_MC_ME.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K314_PRAMC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K314_FLASH.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K314_CMU_FC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K314_SYSTICK.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K314_EMIOS.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K314_RTC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K314_CONFIGURATION_GPR.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k322')"!][!//
[!CODE!][!WS "0"!]#include "S32K322_MC_CGM.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K322_FIRC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K322_SIRC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K322_FXOSC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K322_SXOSC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K322_PLL.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K322_MC_ME.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K322_PRAMC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K322_FLASH.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K322_CMU_FC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K322_SYSTICK.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K322_EMIOS.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K322_RTC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K322_CONFIGURATION_GPR.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k324')"!][!//
[!CODE!][!WS "0"!]#include "S32K324_MC_CGM.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K324_FIRC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K324_SIRC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K324_FXOSC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K324_SXOSC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K324_PLL.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K324_MC_ME.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K324_PRAMC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K324_FLASH.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K324_CMU_FC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K324_SYSTICK.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K324_EMIOS.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K324_RTC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K324_CONFIGURATION_GPR.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k341')"!][!//
[!CODE!][!WS "0"!]#include "S32K342_MC_CGM.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_FIRC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_SIRC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_FXOSC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_SXOSC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_PLL.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_MC_ME.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_PRAMC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_FLASH.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_CMU_FC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_SYSTICK.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_EMIOS.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_RTC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_CONFIGURATION_GPR.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k342')"!][!//
[!CODE!][!WS "0"!]#include "S32K342_MC_CGM.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_FIRC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_SIRC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_FXOSC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_SXOSC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_PLL.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_MC_ME.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_PRAMC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_FLASH.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_CMU_FC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_SYSTICK.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_EMIOS.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_RTC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K342_CONFIGURATION_GPR.h"[!CR!][!ENDCODE!][!//
     [!ELSE!][!//
[!CODE!][!WS "0"!]#include "S32K344_MC_CGM.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K344_FIRC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K344_SIRC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K344_FXOSC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K344_SXOSC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K344_PLL.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K344_MC_ME.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K344_PRAMC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K344_FLASH.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K344_CMU_FC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K344_SYSTICK.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K344_EMIOS.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K344_RTC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32K344_CONFIGURATION_GPR.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!// 
[!ENDIF!][!// 
[!ENDNOCODE!][!//
/*==================================================================================================
                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CLOCK_IP_CFG_DEFINES_VENDOR_ID                       43
#define CLOCK_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION        4
#define CLOCK_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION        4
#define CLOCK_IP_CFG_DEFINES_AR_RELEASE_REVISION_VERSION     0
#define CLOCK_IP_CFG_DEFINES_SW_MAJOR_VERSION                2
#define CLOCK_IP_CFG_DEFINES_SW_MINOR_VERSION                0
#define CLOCK_IP_CFG_DEFINES_SW_PATCH_VERSION                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/**
* @brief            Derivative used.
*/
#define CLOCK_IP_DERIVATIVE_[!"ecu:get('MCU.Derivative')"!]

/**
* @brief            Max number of internal oscillators
*/
#define CLOCK_IP_IRCOSCS_COUNT       ([!"num:i( count( ecu:list('MCU.IRCOSC.List') ) )"!]U)

/**
* @brief            Max number of external oscillators
*/
#define CLOCK_IP_XOSCS_COUNT       ([!"num:i( count( ecu:list('MCU.XOSC.List') ) )"!]U)

/**
* @brief            Max number of pll devices
*/
#define CLOCK_IP_PLLS_COUNT       ([!"num:i( count( ecu:list('MCU.PLL.List') ) )"!]U)

/**
* @brief            Max number of selectors
*/
#define CLOCK_IP_SELECTORS_COUNT       ([!"num:i( count( ecu:list('MCU.SELECTOR.Names.List') ) )"!]U)

/**
* @brief            Max number of dividers
*/
#define CLOCK_IP_DIVIDERS_COUNT       ([!"num:i( count( ecu:list('MCU.DIVIDER.Names.List') ) )"!]U)

/**
* @brief            Max number of divider triggers
*/
#define CLOCK_IP_DIVIDER_TRIGGERS_COUNT       ([!"num:i( count( ecu:list('MCU.DIVIDER.TRIGGER.Names.List') ) )"!]U)

/**
* @brief            Max number of fractional dividers
*/
#define CLOCK_IP_FRACTIONAL_DIVIDERS_COUNT       ([!"num:i( count( ecu:list('MCU.ClockDFS.List') ) )"!]U)

/**
* @brief            Max number of external clocks
*/
#define CLOCK_IP_EXT_CLKS_COUNT       ([!"num:i( count( ecu:list('MCU.ExtClock.List') ) )"!]U)

/**
* @brief            Max number of pcfs
*/
#define CLOCK_IP_PCFS_COUNT       ([!"num:i( count( ecu:list('MCU.CGM.CgmPcfs.List') ))"!]U)

/**
* @brief            Max number of clock gates
*/
#define CLOCK_IP_GATES_COUNT       ([!"num:i( count( ecu:list('MCU.GATE.Names.List') ) )"!]U)

/**
* @brief            Max number of clock monitoring units
*/
#define CLOCK_IP_CMUS_COUNT       ([!"num:i( count( ecu:list('MCU.CMU_FC.List') ) )"!]U)

/**
* @brief            Max number of configured frequencies values
*/
#define CLOCK_IP_CONFIGURED_FREQUENCIES_COUNT       ([!"num:i(1 + num:i( count( ecu:list('MCU.ConfiguredFrequency.List') ) ) )"!]U)

/**
* @brief            Max number of specific peripheral (eMIOS) units
*/
#define CLOCK_IP_SPECIFIC_PERIPH_COUNT       ([!"num:i( count( ecu:list('MCU.SPECIFIC_PERIPHERAL_PRAMs.List') ) )"!]U)

/**
* @brief            Supported power mode.
*/
#define CLOCK_IP_HAS_RUN_MODE                0U

/**
* @brief            Clock ip supports clock frequency
*/
#define CLOCK_IP_GET_FREQUENCY_API              [!IF "McuDebugConfiguration/McuGetClockFrequencyApi = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief            Firc frequency
*/
#define CLOCK_IP_FIRC_FREQUENCY              ([!"(num:i(McuModuleConfiguration/McuClockSettingConfig/*[1]/McuFIRC/McuFIRC_Frequency))"!]U)

/**
* @brief            Sirc frequency
*/
#define CLOCK_IP_SIRC_FREQUENCY              32000U

/**
* @brief            Default fxosc frequency
*/
#define CLOCK_IP_DEFAULT_FXOSC_FREQUENCY     16000000U

/**
* @brief            Default sxosc frequency
*/
#define CLOCK_IP_DEFAULT_SXOSC_FREQUENCY     32768U

/**
* @brief            Clock ip supports ram wait states
*/
[!IF "McuGeneralConfiguration/McuDisableRamWaitStatesConfig = 'false'"!]
#define CLOCK_IP_HAS_RAM_WAIT_STATES
[!ENDIF!]

/**
* @brief            Clock ip supports flash wait states
*/
[!IF "McuGeneralConfiguration/McuDisableFlashWaitStatesConfig = 'false'"!]
#define CLOCK_IP_HAS_FLASH_WAIT_STATES
[!ENDIF!]

/**
* @brief            Clock ip supports to disable FIRC in STDBY mode
*/
#define CLOCK_IP_HAS_FIRC_STDBY_CLOCK_DISABLE

/**
* @brief            Clock ip supports to enable FIRC in STDBY mode
*/
#define CLOCK_IP_HAS_FIRC_STDBY_CLOCK_ENABLE

/**
* @brief            Clock ip supports to disable SIRC in STDBY mode
*/
#define CLOCK_IP_HAS_SIRC_STDBY_CLOCK_DISABLE

/**
* @brief            Clock ip supports to enable SIRC in STDBY mode
*/
#define CLOCK_IP_HAS_SIRC_STDBY_CLOCK_ENABLE

[!IF "McuGeneralConfiguration/McuRegisterValuesOptimization = 'ENABLED'"!][!//
[!VAR "Mac_NumberOfCmus" = "0"!][!//
[!LOOP "McuModuleConfiguration/McuClockSettingConfig/*"!][!//
    [!VAR "Mac_NumOfClkMonitor" = "0"!][!//
    [!IF "node:exists("McuClkMonitor")"!][!//
        [!LOOP "./McuClkMonitor/*"!][!//
            [!IF "node:value(./McuClockMonitorUnderMcuControl)"!][!//
                [!VAR "Mac_NumOfClkMonitor" = "$Mac_NumOfClkMonitor + 1"!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
    [!ENDIF!][!//
    [!VAR "Mac_NumberOfCmus" = "$Mac_NumberOfCmus + $Mac_NumOfClkMonitor"!][!//
[!ENDLOOP!][!// "McuModuleConfiguration/McuClockSettingConfig/*"
[!IF "$Mac_NumberOfCmus > num:i(0)"!][!//
/**
* @brief            Clock ip number of register values when register value optimization is enabled
*/
#define CLOCK_IP_REGISTER_VALUES_COUNT       ([!"num:i($Mac_NumberOfCmus * 4)"!]U)
[!ENDIF!][!//
[!ENDIF!][!//

/**
* @brief            Supported clocks.
*/
[!FOR "Index" = "1" TO "num:i( count( ecu:list('MCU.ProducerClocks.List') ) )"!][!//
#define CLOCK_IP_HAS_[!"ecu:list('MCU.ProducerClocks.List')[num:i($Index)]"!]         [!"num:i($Index)"!]U
[!ENDFOR!][!//
#define CLOCK_IP_FEATURE_PRODUCERS_NO         [!"num:i(1 + num:i( count( ecu:list('MCU.ProducerClocks.List') ) ) )"!]U
[!FOR "Index" = "1" TO "num:i( count( ecu:list('MCU.ConsumerClocks.List') ) )"!][!//
#define CLOCK_IP_HAS_[!"ecu:list('MCU.ConsumerClocks.List')[num:i($Index)]"!]         [!"num:i(num:i($Index) + num:i(count(ecu:list('MCU.ProducerClocks.List'))) + 1)"!]U
[!ENDFOR!][!//
#define CLOCK_IP_FEATURE_NAMES_NO         [!"num:i(1 + num:i(count(ecu:list('MCU.ProducerClocks.List'))) + num:i(count(ecu:list('MCU.ConsumerClocks.List'))) + 1)"!]U

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/



#ifdef __cplusplus
}
#endif

/** @} */
#endif /* #ifndef CLOCK_IP_CFG_DEFINES_H */

