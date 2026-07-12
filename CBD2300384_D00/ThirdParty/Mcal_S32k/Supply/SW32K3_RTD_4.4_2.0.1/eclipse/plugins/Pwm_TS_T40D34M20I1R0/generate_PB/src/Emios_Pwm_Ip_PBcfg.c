[!AUTOSPACING!]
[!NOCODE!]
    [!// Section for macros used in this file
    [!MACRO "getParamAsr", "instance", "channel"!]
        [!NOCODE!]
            [!VAR "varPeriodMax" = "65534"!]
            [!VAR "valuePeriod" = "''"!]
            [!VAR "valueDutyCycle" = "''"!]
            [!VAR "valuePolarity" = "''"!]
            [!VAR "checkValid" = "0"!]
            [!LOOP "../../../../PwmChannel/*"!]
                [!IF "contains(PwmHwChannel,'Emios')"!]
                    [!VAR "instanceIdx" = "num:i(substring-after(node:ref(PwmHwChannel)/../../PwmHwInstance, '_'))"!]
                    [!VAR "channelIdx" = "num:i(substring-after(node:ref(PwmHwChannel)/EmiosChId, '_'))"!]
                    [!VAR "clockPrescaler" = "num:i(substring-after(node:ref(PwmHwChannel)/EmiosChPrescaler, 'EMIOS_PWM_IP_CLOCK_DIV_'))"!]
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
                            [!VAR "valuePolarity" = "concat('EMIOS_PWM_IP_ACTIVE_', $valuePolarity)"!]

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
                [!ERROR!] "EMIOS_[!"$instance"!]_CH_[!"$channel"!] did not reference by any channels in PwmChannel list, Please remove this channel in EmiosChannels list"[!ENDERROR!]
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
*   @file       Emios_Pwm_Ip_PBcfg.c
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
#include "Emios_Pwm_Ip_Cfg.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define EMIOS_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_VENDOR_ID_C                       43
#define EMIOS_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_AR_RELEASE_MAJOR_VERSION_C        4
#define EMIOS_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_AR_RELEASE_MINOR_VERSION_C        4
#define EMIOS_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_AR_RELEASE_REVISION_VERSION_C     0
#define EMIOS_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_SW_MAJOR_VERSION_C                2
#define EMIOS_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_SW_MINOR_VERSION_C                0
#define EMIOS_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_SW_PATCH_VERSION_C                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if source file and Emios_Pwm_Ip_Cfg.h file are of the same vendor */
#if (EMIOS_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_VENDOR_ID_C != EMIOS_PWM_IP_CFG_VENDOR_ID)
    #error "Vendor IDs of Emios_Pwm_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Emios_Pwm_Ip_Cfg.h are different."
#endif

/* Check if source file and Emios_Pwm_Ip_Cfg.h file are of the same AUTOSAR version */
#if ((EMIOS_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_AR_RELEASE_MAJOR_VERSION_C    != EMIOS_PWM_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_AR_RELEASE_MINOR_VERSION_C    != EMIOS_PWM_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_AR_RELEASE_REVISION_VERSION_C != EMIOS_PWM_IP_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR version numbers of Emios_Pwm_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Emios_Pwm_Ip_Cfg.h are different."
#endif

/* Check if source file and Emios_Pwm_Ip_Cfg.h file are of the same Software version */
#if ((EMIOS_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_SW_MAJOR_VERSION_C != EMIOS_PWM_IP_CFG_SW_MAJOR_VERSION)  || \
     (EMIOS_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_SW_MINOR_VERSION_C != EMIOS_PWM_IP_CFG_SW_MINOR_VERSION)  || \
     (EMIOS_PWM_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PB_CFG_SW_PATCH_VERSION_C != EMIOS_PWM_IP_CFG_SW_PATCH_VERSION))
    #error "Software version numbers of Emios_Pwm_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Emios_Pwm_Ip_Cfg.h are different."
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
[!VAR "numEmiosChannelsPerModule" = "ecu:get('Pwm.EmiosChannelCount')"!]
/** @brief  Prototypes of Emios channels User Notifications */
extern void Pwm_Notification(uint8 Channel);
[!ENDIF!]

#define PWM_STOP_SEC_CODE
#include "Pwm_MemMap.h"

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/
#ifndef EMIOS_PWM_IP_PRECOMPILE_SUPPORT
#define PWM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Pwm_MemMap.h"

[!LOOP "./PwmChannelConfigSet/PwmEmios/*"!]
    [!VAR "instanceId" = "substring-after(./PwmHwInstance, '_')"!]
    [!VAR "instanceCfgName" = "name(.)"!]
/*******************************************************************************
 * [!"./PwmHwInstance"!] initialization configuration structures
 ******************************************************************************/
    [!LOOP "./PwmEmiosChannels/*"!]
        [!NOCODE!]
            [!VAR "channelId" = "substring-after(./EmiosChId, '_')"!]
            [!VAR "channelCfgName" = "name(.)"!]
            [!// Get the Flag mode for the pwm mode selected. For OPWMT mode no flag suffix is needed
            [!IF "contains(./EmiosChMode, 'OPWMT')"!]
                [!VAR "channelFlagMode" = "''"!]
            [!ELSEIF "./EmiosChFlagGeneration = 'Trailing_Edge'"!]
                [!VAR "channelFlagMode" = "'_FLAG'"!]
            [!ELSE!]
                [!VAR "channelFlagMode" = "'_FLAG_BOTH'"!]
            [!ENDIF!]
        [!ENDNOCODE!]
[!CR!]
/* Emios PWM mode configuration structure for [!"$instanceCfgName"!]/[!"$channelCfgName"!] */
const Emios_Pwm_Ip_ChannelConfigType Emios_Pwm_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_I[!"$instanceId"!]_Ch[!"$channelId"!] =
{
    /* .ChannelId */            [!"$channelId"!]U,
    /* .Mode */                 [!"node:value(./EmiosChMode)"!][!"$channelFlagMode"!],
    /* .Timebase */             [!"node:value(./EmiosChCounterBus)"!],
    /* .DebugMode */            (boolean)[!"text:toupper(node:value(./EmiosChFreeze))"!],
    /* .OutputDisableSource */  [!"node:value(./EmiosChOutputDisable)"!],
    /* .InternalPs */           [!"node:value(./EmiosChPrescaler)"!],
    /* .InternalPsAlternate */  [!"node:value(./EmiosChPrescalerAlternate)"!],
    /* .InternalPsSrc */        [!"node:value(./EmiosChPrescalerSource)"!],
[!CALL "getParamAsr", "instance" = "$instanceId", "channel" = "$channelId"!]
    /* .OutputPolarity */       [!"$valuePolarity"!],
    /* .IrqMode */              [!"node:value(./EmiosChInterrupt)"!],
[!IF "../../../../../PwmGeneral/PwmNotificationSupported= 'true'"!]
    /* .UserCallback */         {
            /* .CbFunction */       Pwm_Notification,
            /* .CbParameter */      (uint8)[!"num:i($instanceId * $numEmiosChannelsPerModule + $channelId)"!]U
                                },
[!ELSE!][!//
    /* .UserCallback */         {
            /* .CbFunction */       [!"node:value(./EmiosChIrqCallback/EmiosChIrqFunctionCallback)"!],
            /* .CbParameter */      [!"node:value(./EmiosChIrqCallback/EmiosChIrqParameterCallback)"!]
                                },
[!ENDIF!]
    /* .PeriodCount */          [!"$valuePeriod"!]U,
    /* .DutyCycle */            [!"$valueDutyCycle"!]U,
    /* .PhaseShift */           [!"node:value(./EmiosChPhaseShift)"!]U,
    /* .DeadTime */             [!"node:value(./EmiosChDeadtime)"!]U,
    /* .TriggerPosition */      [!"node:value(./EmiosChTrigger)"!]U
};
    [!ENDLOOP!]
[!CR!]
[!ENDLOOP!]
[!//
#define PWM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Pwm_MemMap.h"
#endif  /* EMIOS_PWM_IP_PRECOMPILE_SUPPORT */

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
