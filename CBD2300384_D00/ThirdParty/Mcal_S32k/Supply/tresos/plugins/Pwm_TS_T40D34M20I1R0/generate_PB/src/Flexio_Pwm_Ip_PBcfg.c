[!AUTOSPACING!]
[!NOCODE!]
    [!// Section for macros used in this file
    [!MACRO "getParamAsr", "instance", "channel"!]
        [!NOCODE!]
            [!VAR "varPeriodMax" = "512"!]
            [!VAR "valuePeriod" = "''"!]
            [!VAR "valueDutyCycle" = "''"!]
            [!VAR "valuePolarity" = "''"!]
            [!VAR "checkValid" = "0"!]
            [!LOOP "../../../../PwmChannel/*"!]
                [!IF "contains(PwmHwChannel,'Flexio')"!]
                    [!VAR "instanceIdx" = "num:i(substring-after(node:ref(PwmHwChannel)/../../PwmHwInstance, '_'))"!]
                    [!VAR "channelIdx" = "num:i(substring-after(node:ref(PwmHwChannel)/FlexioChId, '_'))"!]
                    [!VAR "clockPrescaler" = "num:i(substring-after(node:ref(PwmHwChannel)/FlexioChPrescaler, 'FLEXIO_PWM_IP_CLK_DIV_'))"!]
                    [!VAR "mcuClockRef" = "num:i(node:value(node:ref(./PwmMcuClockReferencePoint)/McuClockReferencePointFrequency))"!]
                    [!IF "($instance = $instanceIdx) and ($channel = $channelIdx)"!]
                            [!VAR "checkValid" = "1"!]
                            [!IF "PwmPeriodInTicks"!]
                                [!VAR "valuePeriod" = "num:i(node:value(PwmPeriodDefault))"!]
                            [!ELSE!]
                                [!VAR "valuePeriod" = "num:i(node:value(PwmPeriodDefault) * ($mcuClockRef div $clockPrescaler))"!]
                            [!ENDIF!]
                            [!VAR "valueDutyCycle" = "num:i(node:value(PwmDutycycleDefault))"!]
                            [!VAR "valueDutyCycle" = "num:i(($valuePeriod * $valueDutyCycle) div 32768)"!]

                            [!VAR "valuePolarity" = "substring-after(node:value(PwmPolarity), '_')"!]
                            [!VAR "valuePolarity" = "concat('FLEXIO_PWM_IP_ACTIVE_', $valuePolarity)"!]

                            [!IF "num:i($valuePeriod) > $varPeriodMax"!]
                                [!ERROR!]
[Error in variant: [!"variant:name()"!]] The configured period/ticks for PwmChannelId '[!"PwmChannelId"!]' is [!"$valuePeriod"!] ticks which is larger than the maximum allowed value for the period, which is [!"num:i($varPeriodMax)"!] ticks
The plugin performs the conversion from Period to ticks if requested.
To solve this problem change the configured Period/ticks or change the Pwm clock tree settings.
                                [!ENDERROR!]
                            [!ENDIF!]

                    [!ENDIF!]
                [!ENDIF!]
            [!ENDLOOP!]
            [!IF "$checkValid = 0"!]
                [!ERROR!] "Flexio_[!"$instance"!]_CH_[!"$channel"!] did not reference by any channels in PwmChannel list, Please remove this channel in FlexioChannels list"[!ENDERROR!]
            [!ENDIF!]
        [!ENDNOCODE!]
    [!ENDMACRO!]
[!ENDNOCODE!]
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

/**
*   @file Flexio_Pwm_Ip_PBcfg.c
*
*   @addtogroup flexio_pwm_ip FlexIO Pwm IPL
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
#include "Flexio_Pwm_Ip_Types.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLEXIO_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_VENDOR_ID_C                      43
#define FLEXIO_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C       4
#define FLEXIO_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MINOR_VERSION_C       4
#define FLEXIO_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_REVISION_VERSION_C    0
#define FLEXIO_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MAJOR_VERSION_C               2
#define FLEXIO_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MINOR_VERSION_C               0
#define FLEXIO_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_PATCH_VERSION_C               1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if header file and Flexio_Pwm_Ip_Types.h header file are of the same vendor */
#if (FLEXIO_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_VENDOR_ID_C != FLEXIO_PWM_IP_TYPES_VENDOR_ID)
    #error "Vendor IDs of Flexio_Pwm_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Flexio_Pwm_Ip_Types.h are different."
#endif

/* Check if header file and Flexio_Pwm_Ip_Types.h header file are of the same AUTOSAR version */
#if ((FLEXIO_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C    != FLEXIO_PWM_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (FLEXIO_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MINOR_VERSION_C    != FLEXIO_PWM_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (FLEXIO_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_REVISION_VERSION_C != FLEXIO_PWM_IP_TYPES_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR version numbers of Flexio_Pwm_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Flexio_Pwm_Ip_Types.h are different."
#endif

/* Check if header file and Flexio_Pwm_Ip_Types.h header file are of the same software version */
#if ((FLEXIO_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MAJOR_VERSION_C != FLEXIO_PWM_IP_TYPES_SW_MAJOR_VERSION) || \
     (FLEXIO_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MINOR_VERSION_C != FLEXIO_PWM_IP_TYPES_SW_MINOR_VERSION) || \
     (FLEXIO_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_PATCH_VERSION_C != FLEXIO_PWM_IP_TYPES_SW_PATCH_VERSION))
    #error "Software version numbers of Flexio_Pwm_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Flexio_Pwm_Ip_Types.h are different."
#endif

/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#define PWM_START_SEC_CODE
#include "Pwm_MemMap.h"

[!IF "PwmGeneral/PwmNotificationSupported"!]
[!VAR "numFlexioChannelsPerModule" = "ecu:get('Pwm.FlexioChannelCount')"!]
/** @brief  Prototypes of Pwm notification callback */
extern void Pwm_Ipw_Notification(uint8 Channel);
[!ENDIF!]

#define PWM_STOP_SEC_CODE
#include "Pwm_MemMap.h"

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/
#define PWM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Pwm_MemMap.h"

[!LOOP "./PwmChannelConfigSet/PwmFlexio/*/PwmFlexioChannels/*"!]
    [!VAR "instanceId" = "substring-after(../../PwmHwInstance, '_')"!]
    [!VAR "instanceCfgName" = "name(../..)"!]
    [!VAR "channelId" = "substring-after(FlexioChId, '_')"!]
    [!VAR "channelCfgName" = "name(.)"!]
    [!VAR "pinId" = "substring-after(FlexioPinId, '_')"!]
/** @brief [!"$channelCfgName"!] of [!"$instanceCfgName"!] Pwm using Timer [!"$channelId"!] and Pin [!"$pinId"!] */
const Flexio_Pwm_Ip_ChannelConfigType Flexio_Pwm_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_I[!"$instanceId"!]_Ch[!"$channelId"!] =
{
    /* TimerId */   [!"$channelId"!]U,
    /* PinId */     [!"$pinId"!]U,
#if (defined(FLEXIO_PWM_IP_HAS_PRESCALER) && (FLEXIO_PWM_IP_HAS_PRESCALER == STD_ON))
    /* Prescaler */ [!"node:value(./FlexioChPrescaler)"!],
#endif
[!CALL "getParamAsr", "instance" = "$instanceId", "channel" = "$channelId"!]
    /* Period */    [!"$valuePeriod"!]U,
    /* DutyCycle */ [!"$valueDutyCycle"!]U,
#if (defined(FLEXIO_PWM_IP_HAS_LOW_MODE) && (FLEXIO_PWM_IP_HAS_LOW_MODE == STD_ON))
    /* Polarity */  [!"$valuePolarity"!],
#endif
    /* IrqMode */   [!"node:value(./FlexioChInterrupt)"!],
    /* Callback */  {
        /* CbFunction */    NULL_PTR,
        /* CbParameter */   NULL_PTR
                    },
    /* Callback */  {
    [!IF "node:value(./FlexioChInterrupt) != ecu:list('Pwm.FlexioIrqType')[1]"!]
        /* CbFunction */    &Pwm_Ipw_Notification,
        /* CbParameter */   (uint8)[!"num:i($instanceId * $numFlexioChannelsPerModule + $channelId)"!]
    [!ELSE!]
        /* CbFunction */    NULL_PTR,
        /* CbParameter */   0   
    [!ENDIF!]
                    }
};

[!ENDLOOP!]
#define PWM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Pwm_MemMap.h"

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/


#ifdef __cplusplus
}
#endif

/** @} */

[!ENDCODE!]
