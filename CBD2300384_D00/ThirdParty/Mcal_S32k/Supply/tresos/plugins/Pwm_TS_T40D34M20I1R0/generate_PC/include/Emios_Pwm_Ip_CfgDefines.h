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

#ifndef EMIOS_PWM_IP_CFGDEFINES_H
#define EMIOS_PWM_IP_CFGDEFINES_H

/**
*   @file       EMIOS_Pwm_Ip_CfgDefines.h
*
*   @addtogroup emios_pwm_ip EMIOS Pwm IPL
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
/* Standard Types */
#include "StandardTypes.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define EMIOS_PWM_IP_CFGDEFINES_VENDOR_ID                    43
#define EMIOS_PWM_IP_CFGDEFINES_MODULE_ID                    121
#define EMIOS_PWM_IP_CFGDEFINES_AR_RELEASE_MAJOR_VERSION     4
#define EMIOS_PWM_IP_CFGDEFINES_AR_RELEASE_MINOR_VERSION     4
#define EMIOS_PWM_IP_CFGDEFINES_AR_RELEASE_REVISION_VERSION  0
#define EMIOS_PWM_IP_CFGDEFINES_SW_MAJOR_VERSION             2
#define EMIOS_PWM_IP_CFGDEFINES_SW_MINOR_VERSION             0
#define EMIOS_PWM_IP_CFGDEFINES_SW_PATCH_VERSION             1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if header file and StandardTypes.h are of the same AUTOSAR version */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((EMIOS_PWM_IP_CFGDEFINES_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
         (EMIOS_PWM_IP_CFGDEFINES_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR version numbers of EMIOS_Pwm_Ip_CfgDefines.h and StandardTypes.h are different."
    #endif
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/** @brief        Switch to enable the notifications feature */
#define EMIOS_PWM_IP_NOTIFICATION_SUPPORT             ([!IF "PwmGeneral/PwmNotificationSupported"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief        Macros to indicate eMIOS channels used. */
[!LOOP "PwmChannelConfigSet/PwmEmios/*/PwmEmiosChannels/*"!]
#ifndef [!"text:toupper(../../PwmHwInstance)"!]_[!"EmiosChId"!]_USED
    #define [!"text:toupper(../../PwmHwInstance)"!]_[!"EmiosChId"!]_USED
#else
    #error "[!"text:toupper(../../PwmHwInstance)"!]_[!"EmiosChId"!] channel cannot be used by Pwm. Channel locked by another driver!"
#endif
[!ENDLOOP!]

[!NOCODE!]
    [!VAR "EmiosInterruptUsed" = "0"!]

    [!IF "(PwmGeneral/PwmNotificationSupported = 'true')"!]
        [!LOOP "PwmChannelConfigSet/PwmEmios/*/PwmEmiosChannels/*"!]
            [!IF "EmiosChInterrupt = 'EMIOS_PWM_IP_INTERRUPT_REQUEST'"!]
                [!VAR "EmiosInterruptUsed" = "1"!]
            [!ENDIF!]
        [!ENDLOOP!]
    [!ENDIF!]

    [!IF "$EmiosInterruptUsed != 0"!]
        [!CODE!]
/**
* @brief        Macros used to enable ISR for eMIOS channel
*
*/
            [!LOOP "PwmChannelConfigSet/PwmEmios/*/PwmEmiosChannels/*"!]
                [!IF "EmiosChInterrupt = 'EMIOS_PWM_IP_INTERRUPT_REQUEST'"!]
#define PWM_[!"text:toupper(../../PwmHwInstance)"!]_[!"EmiosChId"!]_ISR_USED
[!CR!]
#ifndef [!"text:toupper(../../PwmHwInstance)"!]_[!"EmiosChId"!]_ISR_USED
    #define [!"text:toupper(../../PwmHwInstance)"!]_[!"EmiosChId"!]_ISR_USED
#endif
[!CR!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ENDCODE!]
    [!ENDIF!]
[!ENDNOCODE!]

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

#endif /* EMIOS_PWM_IP_CFGDEFINES_H */

[!ENDCODE!]
