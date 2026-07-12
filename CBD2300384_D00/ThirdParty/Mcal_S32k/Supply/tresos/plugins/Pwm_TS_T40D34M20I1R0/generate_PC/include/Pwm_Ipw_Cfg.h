[!AUTOSPACING!]
[!CODE!]
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Emios Flexio
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

#ifndef PWM_IPW_CFG_H
#define PWM_IPW_CFG_H

/**
*   @file       Pwm_Ipw_Cfg.h
*
*   @addtogroup pwm_driver Pwm Driver
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif


[!NOCODE!]
    /* Include file Pwm_VersionCheck_Inc.m relative to plugin */
    [!INCLUDE "Pwm_VersionCheck_Inc.m"!]

    /* Check if Emios is used */
    [!IF "(node:exists(PwmChannelConfigSet/PwmEmios) and (count(PwmChannelConfigSet/PwmEmios/*) > 0)) = 'true'"!]
        [!VAR "EmiosIsUsed" = "'true'"!]
    [!ELSE!]
        [!VAR "EmiosIsUsed" = "'false'"!]
    [!ENDIF!]

    /* Check if Flexio is used */
    [!IF "(node:exists(PwmChannelConfigSet/PwmFlexio) and (count(PwmChannelConfigSet/PwmFlexio/*) > 0)) = 'true'"!]
        [!VAR "FlexioIsUsed" = "'true'"!]
    [!ELSE!]
        [!VAR "FlexioIsUsed" = "'false'"!]
    [!ENDIF!]
    /* Find the offset for hw to logic mapping for flexio channels */
    [!IF "ecu:has('Pwm.EmiosSupported') and (ecu:get('Pwm.EmiosSupported') = 'true')"!]
        [!VAR "FlexioChannelOffset" = "number(ecu:get('Pwm.EmiosInstanceCount')) * number(ecu:get('Pwm.EmiosChannelCount'))"!]
    [!ELSE!]
        [!VAR "FlexioChannelOffset" = "0"!]
    [!ENDIF!]

[!ENDNOCODE!]
/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "StandardTypes.h"
#include "Emios_Pwm_Ip_Cfg.h"

[!IF "$FlexioIsUsed = 'true'"!]
#include "Flexio_Pwm_Ip_Cfg.h"
[!ENDIF!]

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define PWM_IPW_CFG_VENDOR_ID                         43
#define PWM_IPW_CFG_MODULE_ID                         121
#define PWM_IPW_CFG_AR_RELEASE_MAJOR_VERSION          4
#define PWM_IPW_CFG_AR_RELEASE_MINOR_VERSION          4
#define PWM_IPW_CFG_AR_RELEASE_REVISION_VERSION       0
#define PWM_IPW_CFG_SW_MAJOR_VERSION                  2
#define PWM_IPW_CFG_SW_MINOR_VERSION                  0
#define PWM_IPW_CFG_SW_PATCH_VERSION                  1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if header file and StandardTypes.h are of the same AUTOSAR version */
    #if ((PWM_IPW_CFG_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
         (PWM_IPW_CFG_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Pwm_Ipw_Cfg.h and StandardTypes.h are different"
    #endif
#endif

/* Check if header file and Emios_Pwm_Ip_Cfg header file are of the same vendor */
#if (PWM_IPW_CFG_VENDOR_ID != EMIOS_PWM_IP_CFG_VENDOR_ID)
    #error "Vendor IDs of Pwm_Ipw_Cfg.h and Emios_Pwm_Ip_Cfg.h are different."
#endif

/* Check if header file and Emios_Pwm_Ip_Cfg header file are of the same AUTOSAR version */
#if ((PWM_IPW_CFG_AR_RELEASE_MAJOR_VERSION    != EMIOS_PWM_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (PWM_IPW_CFG_AR_RELEASE_MINOR_VERSION    != EMIOS_PWM_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (PWM_IPW_CFG_AR_RELEASE_REVISION_VERSION != EMIOS_PWM_IP_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR version numbers of Pwm_Ipw_Cfg.h and Emios_Pwm_Ip_Cfg.h are different."
#endif

/* Check if header file and Emios_Pwm_Ip_Cfg header file are of the same software version */
#if ((PWM_IPW_CFG_SW_MAJOR_VERSION != EMIOS_PWM_IP_CFG_SW_MAJOR_VERSION) || \
     (PWM_IPW_CFG_SW_MINOR_VERSION != EMIOS_PWM_IP_CFG_SW_MINOR_VERSION) || \
     (PWM_IPW_CFG_SW_PATCH_VERSION != EMIOS_PWM_IP_CFG_SW_PATCH_VERSION))
    #error "Software version numbers of Pwm_Ipw_Cfg.h and Emios_Pwm_Ip_Cfg.h are different."
#endif

[!IF "$FlexioIsUsed = 'true'"!]
/* Check if source file and Flexio_Pwm_Ip_Cfg.h header file are of the same vendor */
#if (PWM_IPW_CFG_VENDOR_ID != FLEXIO_PWM_IP_CFG_VENDOR_ID)
    #error "Vendor IDs of Pwm_Ipw_Cfg.h and Flexio_Pwm_Ip_Cfg.h are different."
#endif

/* Check if source file and Flexio_Pwm_Ip_Cfg.h header file are of the same AUTOSAR version */
#if ((PWM_IPW_CFG_AR_RELEASE_MAJOR_VERSION    != FLEXIO_PWM_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (PWM_IPW_CFG_AR_RELEASE_MINOR_VERSION    != FLEXIO_PWM_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (PWM_IPW_CFG_AR_RELEASE_REVISION_VERSION != FLEXIO_PWM_IP_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR version numbers of Pwm_Ipw_Cfg.h and Flexio_Pwm_Ip_Cfg.h are different."
#endif

/* Check if source file and Flexio_Pwm_Ip_Cfg.h header file are of the same software version */
#if ((PWM_IPW_CFG_SW_MAJOR_VERSION != FLEXIO_PWM_IP_CFG_SW_MAJOR_VERSION) || \
     (PWM_IPW_CFG_SW_MINOR_VERSION != FLEXIO_PWM_IP_CFG_SW_MINOR_VERSION) || \
     (PWM_IPW_CFG_SW_PATCH_VERSION != FLEXIO_PWM_IP_CFG_SW_PATCH_VERSION))
    #error "Software version numbers of Pwm_Ipw_Cfg.h and Flexio_Pwm_Ip_Cfg.h are different."
#endif
[!ENDIF!]

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/** @brief        Switch to enable the development error detection. */
#define PWM_DEV_ERROR_DETECT                    ([!IF "PwmGeneral/PwmDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_DeInit API is supported */
#define PWM_DE_INIT_API                         ([!IF "PwmConfigurationOfOptApiServices/PwmDeInitApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_GetOutputState API is supported */
#define PWM_GET_OUTPUT_STATE_API                ([!IF "PwmConfigurationOfOptApiServices/PwmGetOutputState"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_SetDutyCycle API is supported */
#define PWM_SET_DUTY_CYCLE_API                  ([!IF "PwmConfigurationOfOptApiServices/PwmSetDutyCycle"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_SetPeriodAndDuty API is supported */
#define PWM_SET_PERIOD_AND_DUTY_API             ([!IF "PwmConfigurationOfOptApiServices/PwmSetPeriodAndDuty"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_SetOutputToIdle API is supported */
#define PWM_SET_OUTPUT_TO_IDLE_API              ([!IF "PwmConfigurationOfOptApiServices/PwmSetOutputToIdle"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_GetChannelState API is supported */
#define PWM_GET_CHANNEL_STATE_API               ([!IF "PwmConfigurationOfOptApiServices/PwmGetChannelStateApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that the notifications are supported */
#define PWM_NOTIFICATION_SUPPORTED              ([!IF "PwmGeneral/PwmNotificationSupported"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to enable the dual clock feature (Pwm_SetClockMode API) */
#define PWM_SET_CLOCK_MODE_API                  ([!IF "PwmGeneral/PwmEnableDualClockMode"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_SetCounterBus API is supported */
#define PWM_SET_COUNTER_BUS_API                 ([!IF "PwmConfigurationOfOptApiServices/PwmSetCounterBusApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_SetChannelOutput API is supported */
#define PWM_SET_CHANNEL_OUTPUT_API              ([!IF "PwmConfigurationOfOptApiServices/PwmSetChannelOutputApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Pwm_SetTriggerDelay API is supported */
#define PWM_SET_TRIGGER_DELAY_API               ([!IF "PwmConfigurationOfOptApiServices/PwmSetTriggerDelayApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to enable that power state mode is supported */
#define PWM_POWER_STATE_SUPPORTED               ([!IF "node:exists(PwmGeneral/PwmLowPowerStatesSupport)"!][!IF "PwmGeneral/PwmLowPowerStatesSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to enable that power state mode is supported */
#define PWM_POWER_STATE_ASYNCH_MODE_SUPPORTED   ([!IF "node:exists(PwmGeneral/PwmPowerStateAsynchTransitionMode)"!][!IF "PwmGeneral/PwmPowerStateAsynchTransitionMode"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that platform-specific validations of the notification function are supported */
#define PWM_NOTIFICATION_PLAUSABILITY           (STD_ON)

/** @brief        Switch to indicate that platform-specific validation of the offset (phase shift) of duty-cycle is supported */
#define PWM_OFFSET_PLAUSABILITY                 (STD_OFF)

/** @brief        Switch to indicate that platform-specific validation of the period is supported */
#define PWM_MAX_PERIOD_PLAUSABILITY             (STD_OFF)

/** @brief       Switch to indicate that platform-specific validation of the Pwm_SetOutputToIdle is supported */
#define PWM_SETOUTPUTTOIDLE_PLAUSABILITY        (STD_OFF)

/** @brief        Switch to indicate that Hw instance common configuration done by the PWM driver */
#define PWM_HW_INSTANCE_USED                    (STD_OFF)

/** @brief        Maximum number of all hardware Emios modules available on this platform */
#define PWM_HW_MODULE_EMIOS_NO                   ((uint8)[!IF "$EmiosIsUsed = 'true'"!][!"ecu:get('Pwm.EmiosInstanceCount')"!][!ELSE!]0[!ENDIF!]U)

/** @brief        Maximum number of all hardware FlexIO modules available on this platform */
#define PWM_HW_MODULE_FLEXIO_NO                  ((uint8)[!IF "$FlexioIsUsed = 'true'"!][!"ecu:get('Pwm.FlexioInstanceCount')"!][!ELSE!]0[!ENDIF!]U)

/** @brief        Maximum number of all hardware modules available on this platform */
#define PWM_HW_MODULE_NO                         (PWM_HW_MODULE_EMIOS_NO + PWM_HW_MODULE_FLEXIO_NO)

/** @brief        Switch to indicate that eMIOS is used */
#define PWM_EMIOS_USED                          ([!IF "$EmiosIsUsed = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Switch to indicate that Flexio is used */
#define PWM_FLEXIO_USED                         ([!IF "$FlexioIsUsed = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Offset to indicate Flexio channel location in hw to logic channel mapping */
#define PWM_IPW_FLEXIO_CHANNEL_OFFSET           ([!"num:i($FlexioChannelOffset)"!]U)

[!IF "$EmiosIsUsed = 'true'"!]
/** @brief        ID of all EMIOS hardware modules available on this platform */
/*                The macros will be used in Pwm_SyncUpdate() api */
[!FOR "instance" = "0" TO "number(ecu:get('Pwm.EmiosInstanceCount')) - 1"!]
#define PWM_EMIOS_INSTANCE_[!"$instance"!]                (uint8)([!"$instance"!]U)
[!ENDFOR!]
[!ENDIF!]

[!IF "$FlexioIsUsed = 'true'"!]
/** @brief        ID of all FLEXIO hardware modules available on this platform */
/*                The macros will be used in Pwm_SyncUpdate() api */
[!FOR "instance" = "0" TO "number(ecu:get('Pwm.FlexioInstanceCount')) - 1"!]
#define PWM_FLEXIO_INSTANCE_[!"$instance"!]              (PWM_HW_MODULE_EMIOS_NO + (uint8)[!"$instance"!]U)
[!ENDFOR!]
[!ENDIF!]

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
* @brief        PWM Period type (the value of the period is platform dependent and thus configurable)
*/
typedef [!"ecu:get('Pwm.TimerPrecision')"!] Pwm_Ipw_PeriodType;

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/


/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* PWM_IPW_CFG_H */

[!ENDCODE!]
