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

#ifndef EMIOS_PWM_IP_CFG_H
#define EMIOS_PWM_IP_CFG_H

/**
*   @file       Emios_Pwm_Ip_Cfg.h
*
*   @addtogroup emios_pwm_ip Emios Pwm IPL
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

/* DevAssert implementation */
#include "Devassert.h"

/* Register definitions */
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!VAR "DerivativeName" = "text:toupper(substring-before(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative,'_'))"!]
    [!IF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k341')"!][!//
        [!CODE!][!WS "0"!]#include "S32K342_EMIOS.h"[!CR!][!ENDCODE!][!//
    [!ELSE!][!// 
        [!CODE!][!WS "0"!]#include "[!"$DerivativeName"!]_EMIOS.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDIF!][!//

/* Include all variants header files. */
[!IF "var:defined('postBuildVariant')"!]
    [!LOOP "variant:all()"!]
#include "Emios_Pwm_Ip_[!"."!]_PBcfg.h"
    [!ENDLOOP!]
[!ELSE!]
#include "Emios_Pwm_Ip_PBcfg.h"
[!ENDIF!]

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define EMIOS_PWM_IP_CFG_VENDOR_ID                    43
#define EMIOS_PWM_IP_CFG_MODULE_ID                    121
#define EMIOS_PWM_IP_CFG_AR_RELEASE_MAJOR_VERSION     4
#define EMIOS_PWM_IP_CFG_AR_RELEASE_MINOR_VERSION     4
#define EMIOS_PWM_IP_CFG_AR_RELEASE_REVISION_VERSION  0
#define EMIOS_PWM_IP_CFG_SW_MAJOR_VERSION             2
#define EMIOS_PWM_IP_CFG_SW_MINOR_VERSION             0
#define EMIOS_PWM_IP_CFG_SW_PATCH_VERSION             1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if header file and StandardTypes.h are of the same AUTOSAR version */
    #if ((EMIOS_PWM_IP_CFG_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
         (EMIOS_PWM_IP_CFG_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Emios_Pwm_Ip_Cfg.h and StandardTypes.h are different"
    #endif

    /* Check if header file and Devassert.h are of the same AUTOSAR version */
    #if ((EMIOS_PWM_IP_CFG_AR_RELEASE_MAJOR_VERSION != DEVASSERT_AR_RELEASE_MAJOR_VERSION) || \
         (EMIOS_PWM_IP_CFG_AR_RELEASE_MINOR_VERSION != DEVASSERT_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Emios_Pwm_Ip_Cfg.h and Devassert.h are different"
    #endif
#endif

[!IF "var:defined('postBuildVariant')"!]
    [!LOOP "variant:all()"!]
/* Check if header file and Emios_Pwm_Ip_[!"."!]_Pbcfg header file are of the same vendor */
#if (EMIOS_PWM_IP_CFG_VENDOR_ID != EMIOS_PWM_IP_[!"."!]_PB_CFG_VENDOR_ID)
    #error "Vendor IDs of Emios_Pwm_Ip_Cfg.h and Emios_Pwm_Ip_[!"."!]_Pbcfg.h are different."
#endif

/* Check if header file and Pwm_EnvCfg header file are of the same AUTOSAR version */
#if ((EMIOS_PWM_IP_CFG_AR_RELEASE_MAJOR_VERSION    != EMIOS_PWM_IP_[!"."!]_PB_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_PWM_IP_CFG_AR_RELEASE_MINOR_VERSION    != EMIOS_PWM_IP_[!"."!]_PB_CFG_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_PWM_IP_CFG_AR_RELEASE_REVISION_VERSION != EMIOS_PWM_IP_[!"."!]_PB_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR version numbers of Emios_Pwm_Ip_Cfg.h and Emios_Pwm_Ip_[!"."!]_Pbcfg.h are different."
#endif

/* Check if header file and Pwm_EnvCfg header file are of the same software version */
#if ((EMIOS_PWM_IP_CFG_SW_MAJOR_VERSION != EMIOS_PWM_IP_[!"."!]_PB_CFG_SW_MAJOR_VERSION) || \
     (EMIOS_PWM_IP_CFG_SW_MINOR_VERSION != EMIOS_PWM_IP_[!"."!]_PB_CFG_SW_MINOR_VERSION) || \
     (EMIOS_PWM_IP_CFG_SW_PATCH_VERSION != EMIOS_PWM_IP_[!"."!]_PB_CFG_SW_PATCH_VERSION))
    #error "Software version numbers of Emios_Pwm_Ip_Cfg.h and Emios_Pwm_Ip_[!"."!]_Pbcfg.h are different."
#endif
    [!ENDLOOP!]
[!ELSE!]
/* Check if header file and Pwm_EnvCfg header file are of the same vendor */
#if (EMIOS_PWM_IP_CFG_VENDOR_ID != EMIOS_PWM_IP_PB_CFG_VENDOR_ID)
    #error "Vendor IDs of Emios_Pwm_Ip_Cfg.h and Emios_Pwm_Ip_Pbcfg.h are different."
#endif

/* Check if header file and Pwm_EnvCfg header file are of the same AUTOSAR version */
#if ((EMIOS_PWM_IP_CFG_AR_RELEASE_MAJOR_VERSION    != EMIOS_PWM_IP_PB_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_PWM_IP_CFG_AR_RELEASE_MINOR_VERSION    != EMIOS_PWM_IP_PB_CFG_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_PWM_IP_CFG_AR_RELEASE_REVISION_VERSION != EMIOS_PWM_IP_PB_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR version numbers of Emios_Pwm_Ip_Cfg.h and Emios_Pwm_Ip_Pbcfg.h are different."
#endif

/* Check if header file and Pwm_EnvCfg header file are of the same software version */
#if ((EMIOS_PWM_IP_CFG_SW_MAJOR_VERSION != EMIOS_PWM_IP_PB_CFG_SW_MAJOR_VERSION) || \
     (EMIOS_PWM_IP_CFG_SW_MINOR_VERSION != EMIOS_PWM_IP_PB_CFG_SW_MINOR_VERSION) || \
     (EMIOS_PWM_IP_CFG_SW_PATCH_VERSION != EMIOS_PWM_IP_PB_CFG_SW_PATCH_VERSION))
    #error "Software version numbers of Emios_Pwm_Ip_Cfg.h and Emios_Pwm_Ip_Pbcfg.h are different."
#endif
[!ENDIF!]

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/** @brief      Switch to enable the development error detection. */
#define EMIOS_PWM_IP_DEV_ERROR_DETECT           ([!IF "PwmGeneral/PwmDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief      The number of Emios instances available on platform */
#define EMIOS_PWM_IP_INSTANCE_COUNT                ([!"ecu:get('Pwm.EmiosInstanceMacro')"!])
/** @brief      The number of channels available on each Emios instance */
#define EMIOS_PWM_IP_CHANNEL_COUNT                 ([!"ecu:get('Pwm.EmiosChannelMacro')"!])

/** @brief      The number of PWM modes implemented in hardware */
#define EMIOS_PWM_IP_MODES                         ([!"ecu:get('Pwm.EmiosPwmModesCount')"!]U)
/** @brief      Mapping between available channels and the PWM mode supported by each of them */
#define EMIOS_PWM_IP_CHANNEL_MODES                 {\
[!FOR "instance" = "0" TO "number(ecu:get('Pwm.EmiosInstanceCount')) - 1"!]
                                                    {[!"ecu:get(concat('Pwm.EmiosPwmModesMappingInst_', $instance))"!]},\
[!ENDFOR!]
                                                }
/** @brief Min counter value for PWM modes */
#define EMIOS_PWM_IP_MIN_CNT_VAL                   (0x0001UL)
/** @brief Max counter value for PWM modes */
#define EMIOS_PWM_IP_MAX_CNT_VAL                   (0xFFFFUL)

/** @brief      Counter bus channel number */
#define EMIOS_PWM_IP_COUNTER_BUS_A                 (23U)
#define EMIOS_PWM_IP_COUNTER_BUS_BCDE              (248U)
#define EMIOS_PWM_IP_COUNTER_BUS_F                 (22U)

#define EMIOS_PWM_IP_INITIAL_MODES \
{\
[!NOCODE!]
[!FOR "instance" = "0" TO "number(ecu:get('Pwm.EmiosInstanceCount')) - 1"!]
    [!CODE!][!WS "4"!]{\[!CR!][!ENDCODE!]
    [!FOR "channel" = "0" TO "number(ecu:get('Pwm.EmiosChannelCount')) - 1"!]
        [!CODE!][!WS "8"!]EMIOS_PWM_IP_MODE_NODEFINE[!ENDCODE!]
        [!IF "$channel < number(ecu:get('Pwm.EmiosChannelCount')) - 1"!][!CODE!],[!ENDCODE!][!ENDIF!][!CODE!]\[!CR!][!ENDCODE!]
    [!ENDFOR!]
    [!CODE!][!WS "4"!]}[!ENDCODE!]
    [!IF "$instance < number(ecu:get('Pwm.EmiosInstanceCount')) - 1"!][!CODE!],[!ENDCODE!][!ENDIF!][!CODE!]\[!CR!][!ENDCODE!]
[!ENDFOR!]
[!ENDNOCODE!]
}
/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/* Redefine eMIOS_Type from header file to comply with coding guidelines */
typedef eMIOS_Type Emios_Pwm_Ip_HwAddrType;

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

#endif /* EMIOS_PWM_IP_CFG_H */

[!ENDCODE!]
