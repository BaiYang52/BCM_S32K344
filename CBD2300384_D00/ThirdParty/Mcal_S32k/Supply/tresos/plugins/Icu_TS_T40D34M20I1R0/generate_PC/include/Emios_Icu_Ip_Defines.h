[!CODE!][!//
[!AUTOSPACING!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Emios Siul2 Wkpu LpCmp
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
#ifndef EMIOS_ICU_IP_DEFINES_H
#define EMIOS_ICU_IP_DEFINES_H

/**
 *   @file
 *
 *   @addtogroup emios_icu_ip EMIOS IPL
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif

 /*==================================================================================================
 *                                         INCLUDE FILES
 * 1) system and project includes
 * 2) needed interfaces from external units
 * 3) internal and external interfaces from this unit
 *================================================================================================*/
#include "StandardTypes.h"
[!NOCODE!][!// Include specific header file
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!VAR "DerivativeName" = "text:toupper(substring-before(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative,'_'))"!]
    [!CODE!][!WS "0"!]/* Include platform header file. */[!CR!][!ENDCODE!][!//

    [!IF "contains($DerivativeName, 'S32K3')"!][!//
        [!IF "contains($DerivativeName, 'S32K341') or contains($DerivativeName, 'S32K342')"!][!//
            [!CODE!][!WS "0"!]#include "S32K342_EMIOS.h"[!CR!][!ENDCODE!][!//
        [!ELSE!]
            [!CODE!][!WS "0"!]#include "[!"$DerivativeName"!]_EMIOS.h"[!CR!][!ENDCODE!][!//
        [!ENDIF!][!//
    [!ENDIF!][!//

    [!IF "contains($DerivativeName, 'S32E27')"!][!//
        [!CODE!][!WS "0"!]#include "S32E27_EMIOS.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
    [!IF "contains($DerivativeName, 'S32S27')"!][!//
        [!CODE!][!WS "0"!]#include "S32S27_EMIOS.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
    [!IF "contains($DerivativeName, 'S32Z27')"!][!//
        [!CODE!][!WS "0"!]#include "S32Z27_EMIOS.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 *================================================================================================*/
#define EMIOS_ICU_IP_DEFINES_VENDOR_ID                       43
#define EMIOS_ICU_IP_DEFINES_AR_RELEASE_MAJOR_VERSION        4
#define EMIOS_ICU_IP_DEFINES_AR_RELEASE_MINOR_VERSION        4
#define EMIOS_ICU_IP_DEFINES_AR_RELEASE_REVISION_VERSION     0
#define EMIOS_ICU_IP_DEFINES_SW_MAJOR_VERSION                2
#define EMIOS_ICU_IP_DEFINES_SW_MINOR_VERSION                0
#define EMIOS_ICU_IP_DEFINES_SW_PATCH_VERSION                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if header file and Std_Types.h file are of the same Autosar version */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((EMIOS_ICU_IP_DEFINES_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
         (EMIOS_ICU_IP_DEFINES_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Emios_Icu_Ip_Defines.h and StandardTypes.h are different"
    #endif
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
#define EMIOS_ICU_IP_CHANNEL_24_USED             ([!IF "ecu:get('Icu.Num_Emios_Hw_Channels') > 24"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief The number of EMIOS instances available on platform */
#define EMIOS_ICU_IP_INSTANCE_COUNT               ([!"ecu:get('Icu.Num_Emios_Hw_Modules')"!]U)

/** @brief The number of channels available on each EMIOS instance */
#define EMIOS_ICU_IP_NUM_OF_CHANNELS               ([!"ecu:get('Icu.Num_Emios_Hw_Channels')"!]U)

/** @brief Switches the Development Error Detection and Notification on or off.  */
#define EMIOS_ICU_IP_DEV_ERROR_DETECT             ([!IF "IcuGeneral/IcuDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
#define EMIOS_ICU_IP_VALIDATE_GLOBAL_CALL         (EMIOS_ICU_IP_DEV_ERROR_DETECT)

/** @brief Adds or removes all services related to the timestamp functionality. */
#define EMIOS_ICU_IP_TIMESTAMP_API                ([!IF "IcuOptionalApis/IcuTimestampApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief Adds or removes all services related to the edge detect functionality. */
#define EMIOS_ICU_IP_EDGE_DETECT_API              ([!IF "IcuOptionalApis/IcuEdgeDetectApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief Adds or removes all services related to the signal mesurement functionality. */
#define EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API       ([!IF "IcuOptionalApis/IcuSignalMeasurementApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief Adds or removes all services related to the input level. */
#define EMIOS_ICU_IP_GET_INPUT_LEVEL_API          ([!IF "node:exists(IcuAutosarExt/IcuGetInputLevelApi)"!][!IF "IcuAutosarExt/IcuGetInputLevelApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ELSE!]STD_OFF[!ENDIF!])

/** @brief define SAIC mode if any channels not supporting IPWM or IPM mode is configured. */
#define EMIOS_ICU_IP_SIGNAL_MEASUREMENT_USES_SAIC_MODE            ([!IF "node:exists(IcuAutosarExt/IcuSupportSAICModeApi)"!][!IF "IcuAutosarExt/IcuSupportSAICModeApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ELSE!]STD_OFF[!ENDIF!])

/** @brief Adds or removes all services related to the deinitialization functionality. */
#define EMIOS_ICU_IP_DEINIT_API                   ([!IF "IcuOptionalApis/IcuDeInitApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief Adds or removes all services related to mode set functionality. */
#define EMIOS_ICU_IP_SET_MODE_API                 ([!IF "IcuOptionalApis/IcuSetModeApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief Adds or removes all services related to edge count functionality. */
#define EMIOS_ICU_IP_EDGE_COUNT_API               ([!IF "IcuOptionalApis/IcuEdgeCountApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief Adds or removes all services related to input state functionality. */
#define EMIOS_ICU_IP_GET_INPUT_STATE_API          ([!IF "IcuOptionalApis/IcuGetInputStateApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief Adds or removes all services related to dual clock edge functionality. */
#define EMIOS_ICU_IP_DUAL_CLOCK_MODE_API          ([!IF "IcuAutosarExt/IcuEnableDualClockMode"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief Support for User mode.
 *  If this parameter has been configured to STD_ON, the EMIOS driver code
 *  can be executed from both supervisor and user mode. */
#define EMIOS_ICU_IP_ENABLE_USER_MODE_SUPPORT     ([!IF "IcuGeneral/IcuEnableUserModeSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#define EMIOS_ICU_IP_CAPTURERGISTER_API           ([!IF "node:exists(IcuAutosarExt/IcuGetCaptureRegisterValueApi)"!][!IF "IcuAutosarExt/IcuGetCaptureRegisterValueApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ELSE!]STD_OFF[!ENDIF!])

/** @brief Adds or Removes the code related to overflow notification */
#define EMIOS_ICU_IP_OVERFLOW_NOTIFICATION_API    ([!IF "IcuAutosarExt/IcuOverflowNotificationApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** @brief    Adds or removes the support  measurement with DMA*/
[!VAR "TimestampDmaEnable" = "0"!][!/*
*/!][!VAR "MeasurementDmaEnable" = "0"!][!/*
*/!][!SELECT "IcuConfigSet"!][!/*
    */!][!LOOP "./IcuChannel/*"!][!/*
    */!][!IF "IcuDMAChannelEnable = 'true'"!][!/*
        */!][!IF "IcuMeasurementMode = 'ICU_MODE_SIGNAL_MEASUREMENT'"!][!/*
            */!][!VAR "MeasurementDmaEnable" = "$MeasurementDmaEnable + 1"!][!/*
        */!][!ELSEIF "IcuMeasurementMode='ICU_MODE_TIMESTAMP'"!][!/*
            */!][!VAR "TimestampDmaEnable" = "$TimestampDmaEnable + 1"!][!/*
        */!][!ENDIF!][!/*
    */!][!ENDIF!][!/*
    */!][!ENDLOOP!][!/*
 */!][!ENDSELECT!]
#define EMIOS_ICU_IP_SIGNALMEASUREMENT_USES_DMA   ([!IF "$MeasurementDmaEnable != 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
#define EMIOS_ICU_IP_TIMESTAMP_USES_DMA           ([!IF "$TimestampDmaEnable != 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#define EMIOS_ICU_IP_GET_PULSE_WIDTH_API          ([!IF "num:i(0) != num:i(ecu:get('Icu.Num_Emios_Hw_Modules'))"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Verification for user mode support. */
#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    #if (defined (EMIOS_ICU_IP_ENABLE_USER_MODE_SUPPORT) && (STD_ON == EMIOS_ICU_IP_ENABLE_USER_MODE_SUPPORT))
        #error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running Icu in user mode the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined
    #endif
#endif

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

/** @implements PwmExclusivAccessMacro_Activity */
[!MACRO "PwmExclusivAccess"!][!//


[!IF "node:exists(as:modconf('Mcl')[1]/MclConfig/EmiosCommon)"!][!//

    [!LOOP "IcuConfigSet/IcueMios/*"!] [!// Assuming that in the icu.xdm file have check whether EMIOS instance used
        [!VAR "emiosInstance" = "node:value(./IcueMiosModule)"!][!//
        [!LOOP "IcueMiosChannels/*"!] [!// Assuming that in the icu.xdm file have check whether EMIOS channel used
        
            [!IF "contains(node:value(./IcuEmiosBusSelect), 'EMIOS_ICU_BUS_A')"!][!//
                [!LOOP "as:modconf('Mcl')[1]/MclConfig/EmiosCommon/*"!][!//
                    [!IF "text:split(node:value(EmiosMclInstances), '_')[2] = $emiosInstance"!][!//
                        [!LOOP "EmiosMclMasterBus/*"!][!//
                            [!IF "(node:value(./EmiosMclMasterBusNumber) = '23') and node:value(./EmiosMclPwmExclusiveAccess) = 'true' "!][!//
                                [!ERROR!]
                                    EMIOS_ICU_BUS_A(or 23 channel in MCL) is used exclusiv by PWM driver! Please recheck MCL configuration for EMIOS!
                                [!ENDERROR!][!//
                            [!ENDIF!][!//
                        [!ENDLOOP!][!//
                    [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ENDIF!][!//

            [!IF "contains(node:value(./IcuEmiosBusSelect), 'EMIOS_ICU_BUS_F')"!][!//
                [!LOOP "as:modconf('Mcl')[1]/MclConfig/EmiosCommon/*"!][!//
                    [!IF "text:split(node:value(EmiosMclInstances), '_')[2] = $emiosInstance"!][!//
                        [!LOOP "EmiosMclMasterBus/*"!][!//
                            [!IF "(node:value(./EmiosMclMasterBusNumber) = '22') and node:value(./EmiosMclPwmExclusiveAccess) = 'true' "!][!//
                                [!ERROR!]
                                    EMIOS_ICU_BUS_F(or 22 channel in MCL) is used exclusiv by PWM driver! Please recheck MCL configuration for EMIOS!
                                [!ENDERROR!][!//
                            [!ENDIF!][!//
                        [!ENDLOOP!][!//
                    [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ENDIF!][!//

            [!IF "contains(node:value(./IcuEmiosBusSelect), 'EMIOS_ICU_BUS_DIVERSE')"!][!//
                [!IF "num:i(node:value(./IcueMiosChannel)) < 8"!]
                    [!LOOP "as:modconf('Mcl')[1]/MclConfig/EmiosCommon/*"!][!//
                        [!IF "text:split(node:value(EmiosMclInstances), '_')[2] = $emiosInstance"!][!//
                            [!LOOP "EmiosMclMasterBus/*"!][!//
                                [!IF "(node:value(./EmiosMclMasterBusNumber) = '0') and node:value(./EmiosMclPwmExclusiveAccess) = 'true' "!][!//
                                    [!ERROR!]
                                        EMIOS_ICU_BUS_B(or 0 channel in MCL) is used exclusiv by PWM driver! Please recheck MCL configuration for EMIOS!
                                    [!ENDERROR!][!//
                                [!ENDIF!][!//
                            [!ENDLOOP!][!//
                        [!ENDIF!][!//
                    [!ENDLOOP!][!//
                [!ENDIF!]

                [!IF "num:i(node:value(./IcueMiosChannel)) > 8 and num:i(node:value(./IcueMiosChannel)) < 16"!]
                    [!LOOP "as:modconf('Mcl')[1]/MclConfig/EmiosCommon/*"!][!//
                        [!IF "text:split(node:value(EmiosMclInstances), '_')[2] = $emiosInstance"!][!//
                            [!LOOP "EmiosMclMasterBus/*"!][!//
                                [!IF "(node:value(./EmiosMclMasterBusNumber) = '8') and node:value(./EmiosMclPwmExclusiveAccess) = 'true' "!][!//
                                    [!ERROR!]
                                        EMIOS_ICU_BUS_C(or 8 channel in MCL) is used exclusiv by PWM driver! Please recheck MCL configuration for EMIOS!
                                    [!ENDERROR!][!//
                                [!ENDIF!][!//
                            [!ENDLOOP!][!//
                        [!ENDIF!][!//
                    [!ENDLOOP!][!//
                [!ENDIF!]

                [!IF "num:i(node:value(./IcueMiosChannel)) > 16"!]
                    [!LOOP "as:modconf('Mcl')[1]/MclConfig/EmiosCommon/*"!][!//
                        [!IF "text:split(node:value(EmiosMclInstances), '_')[2] = $emiosInstance"!][!//
                            [!LOOP "EmiosMclMasterBus/*"!][!//
                                [!IF "(node:value(./EmiosMclMasterBusNumber) = '16') and node:value(./EmiosMclPwmExclusiveAccess) = 'true' "!][!//
                                    [!ERROR!]
                                        EMIOS_ICU_BUS_D(or 16 channel in MCL) is used exclusiv by PWM driver! Please recheck MCL configuration for EMIOS!
                                    [!ENDERROR!][!//
                                [!ENDIF!][!//
                            [!ENDLOOP!][!//
                        [!ENDIF!][!//
                    [!ENDLOOP!][!//
                [!ENDIF!]
            [!ENDIF!][!//

        [!ENDLOOP!][!//
    [!ENDLOOP!][!//

[!ENDIF!][!//

[!ENDMACRO!][!//

[!CALL "PwmExclusivAccess"!][!//


#ifdef __cplusplus
}
#endif

/** @} */

#endif  /* EMIOS_ICU_IP_DEFINES_H */

[!ENDCODE!][!//
