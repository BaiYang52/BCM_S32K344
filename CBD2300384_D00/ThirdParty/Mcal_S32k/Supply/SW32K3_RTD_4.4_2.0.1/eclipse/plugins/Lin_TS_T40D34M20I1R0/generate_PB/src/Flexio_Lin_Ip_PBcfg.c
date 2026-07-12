/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : LPUART_FLEXIO
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
*   @file    Flexio_Lin_Ip_PBcfg.c
*
*   @addtogroup FLEXIO_IP
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Flexio_Lin_Ip_Cfg.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLEXIO_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_VENDOR_ID_C                     43
#define FLEXIO_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C      4
#define FLEXIO_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MINOR_VERSION_C      4
#define FLEXIO_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_REVISION_VERSION_C   0
#define FLEXIO_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MAJOR_VERSION_C              2
#define FLEXIO_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MINOR_VERSION_C              0
#define FLEXIO_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_PATCH_VERSION_C              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/* Checks against Flexio_Lin_Ip_Cfg.h */
#if (FLEXIO_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_VENDOR_ID_C != FLEXIO_LIN_IP_CFG_VENDOR_ID)
    #error "Flexio_Lin_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_Lin_Ip_Cfg.h have different vendor ids"
#endif
/* Check if current file and Flexio_Lin_Ip_Cfg header file are of the same Autosar version */
#if ((FLEXIO_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C    != FLEXIO_LIN_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (FLEXIO_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MINOR_VERSION_C    != FLEXIO_LIN_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (FLEXIO_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_REVISION_VERSION_C != FLEXIO_LIN_IP_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Flexio_Lin_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_Lin_Ip_Cfg.h are different"
#endif
/* Check if current file and Flexio_Lin_Ip_Cfg header file are of the same Software version */
#if ((FLEXIO_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MAJOR_VERSION_C != FLEXIO_LIN_IP_CFG_SW_MAJOR_VERSION) || \
     (FLEXIO_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MINOR_VERSION_C != FLEXIO_LIN_IP_CFG_SW_MINOR_VERSION) || \
     (FLEXIO_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_PATCH_VERSION_C != FLEXIO_LIN_IP_CFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Flexio_Lin_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_Lin_Ip_Cfg.h are different"
#endif

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                      BUFFER DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                   STATE STRUCTURE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
[!MACRO "FlexioLinBaudrateCalculator"!][!//
[!NOCODE!]
    [!SELECT "node:ref(LinClockRef)"!]
    [!VAR "ClockFrequencyFromMcu" = "McuClockReferencePointFrequency"!]
    [!VAR "ClockFrequency" = "num:i($ClockFrequencyFromMcu)"!]
    [!ENDSELECT!][!//
    [!VAR "FlexioTIMDEC" = "0"!]
    [!VAR "ActualBaudrate" = "0"!]
    [!VAR "ErrorValue" = "1000000"!]
    [!VAR "DivBest" = "255"!]
    [!VAR "RatioBest" = "num:i(ecu:list('Lin.LinFlexioClkDivRatio.List')[1])"!]
    [!FOR "RatioIndex" = "1" TO "count(ecu:list('Lin.LinFlexioClkDivRatio.List'))"!]
        [!VAR "CurrentRatioValue" = "num:i(ecu:list('Lin.LinFlexioClkDivRatio.List')[num:i($RatioIndex)])"!]
        [!FOR "DivCount" = "0" TO "255"!]
            [!VAR "CurrentBaudValue" = "$ClockFrequency div ($CurrentRatioValue * 2 * ($DivCount + 1))"!]
            [!IF "$CurrentBaudValue < $Baudrate"!]
                [!VAR "CurrentErrorValue" = "$Baudrate - $CurrentBaudValue"!]
            [!ELSE!]
                [!VAR "CurrentErrorValue" = "$CurrentBaudValue - $Baudrate"!]
            [!ENDIF!]
            [!IF "$CurrentErrorValue < $ErrorValue"!]
                [!VAR "ErrorValue" = "$CurrentErrorValue"!]
                [!VAR "DivBest" = "$DivCount"!]
                [!VAR "RatioBest" = "$CurrentRatioValue"!]
                [!VAR "ActualBaudrate" = "$CurrentBaudValue"!]
            [!ENDIF!]
        [!ENDFOR!]
    [!ENDFOR!]

    [!VAR "TIMCMPL" = "($ClockFrequency div $RatioBest div 2 div $Baudrate) - 1"!]
    [!IF "$TIMCMPL > 255"!]
        [!ERROR!] Lin baud rate divider = [!"$TIMCMPL"!], it should be <= 255, please select a lower MCU clock.[!ENDERROR!]
    [!ENDIF!]
    [!IF "$RatioBest = num:i(256)"!]
        [!VAR "FlexioTIMDEC" = "5"!]
    [!ELSEIF "$RatioBest = num:i(16)"!]
        [!VAR "FlexioTIMDEC" = "4"!]
    [!ENDIF!]
[!ENDNOCODE!]
[!ENDMACRO!][!//

[!NOCODE!]
[!VAR "NumHwChannelConfig" = "0"!]
[!LOOP "LinGlobalConfig/LinChannel/*"!][!//
[!IF "contains(node:value(LinHwChannel),'FLEXIO')"!][!//
    [!VAR "NumHwChannelConfig" = "$NumHwChannelConfig + 1"!]
[!ENDIF!]
[!ENDLOOP!][!//
[!ENDNOCODE!]
[!IF "$NumHwChannelConfig > 0"!]
[!CODE!]
#define LIN_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Lin_MemMap.h"

extern Flexio_Lin_Ip_StateStructType Flexio_Lin_Ip_StateStructure[FLEXIO_LIN_IP_NUMBER_OF_CHANNELS];

#define LIN_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Lin_MemMap.h"
[!ENDCODE!]
[!ENDIF!]

#define LIN_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Lin_MemMap.h"

[!VAR "CurrentChannel" = "0"!]
[!LOOP "LinGlobalConfig/LinChannel/*"!][!//
[!IF "node:exists(LinFlexioTxControllerRef) and node:exists(LinFlexioRxControllerRef)"!]

        [!CODE!]
/**
 * @brief   FlexIO Config parameters
 */

const Flexio_Lin_Ip_UserConfigType Flexio_Lin_Ip_pHwConfigPB_[!"LinChannelId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]=
{
    [!ENDCODE!]
    [!NOCODE!]
    [!VAR "RxShifterID" = "num:i(text:split((node:ref(LinFlexioRxControllerRef)/FlexioMclChannelId),'_')[2])"!][!//
    [!VAR "TxShifterID" = "num:i(text:split((node:ref(LinFlexioTxControllerRef)/FlexioMclChannelId),'_')[2])"!][!//
    [!VAR "RxTimerID"   = "num:i(text:split((node:ref(LinFlexioRxControllerRef)/FlexioMclChannelId),'_')[2])"!][!//
    [!VAR "TxTimerID"   = "num:i(text:split((node:ref(LinFlexioTxControllerRef)/FlexioMclChannelId),'_')[2])"!][!//
    [!VAR "TxPinID" = "num:i((text:split((node:ref(LinFlexioTxControllerRef)/FlexioMclPinId),'_')[2]))"!][!//
    [!VAR "RxPinID" = "num:i((text:split((node:ref(LinFlexioRxControllerRef)/FlexioMclPinId),'_')[2]))"!][!//
    [!VAR "Baudrate" = "LinChannelBaudRate"!]
    [!CALL "FlexioLinBaudrateCalculator"!]
    [!VAR "HeaderTimeoutValue" = "ceiling((1 div $ActualBaudrate) * LinHeaderTimeout * 1000000)"!]
    [!VAR "ResponseTimeoutValue" = "ceiling((1 div $ActualBaudrate) * (LinResponseTimeout div 10) * 1000000)"!]

    [!// Calculate byte to generate wakeup pulse.
    [!VAR "BitLengthInUs" = "1000000 div $ActualBaudrate"!]
    [!VAR "WakeupByteNumOfBits0" = "5000 div $BitLengthInUs"!][!// maximum duration 5000us of wakeup pulse
    [!IF "$WakeupByteNumOfBits0 > 9"!][!//maximum bits 0 in a frame 0x00 is 9(included 1 Start bit)
        [!VAR "WakeupByteNumOfBits0" = "8"!][!// For maximum baudrate 20kbps, 8bits 0 + 1 Start bit = 450us.
    [!ELSE!]
        [!VAR "WakeupByteNumOfBits0" = "floor($WakeupByteNumOfBits0 - 1)"!][!// For minimum baudrate 1kbps, number of bits 0 is lowest: 5 bits0 - 1 Start bit.
    [!ENDIF!]
    [!VAR "WakeupByte" = "bit:and(bit:shl(255,$WakeupByteNumOfBits0),255)"!]
    [!VAR "WakeupPulseDuration" = "($WakeupByteNumOfBits0 + 1) * $BitLengthInUs"!]

    [!// Calculate minimum number of consecutive bit0 in LSB of byte received to detect wakeup pulse.
    [!IF "BitLengthInUs > 150"!]
        [!VAR "MinimumBits0InWakeupByteDetect" = "0"!][!// Already have 1 bit0 of Start bit, wakeup byte received should have at least 0 bit0 (Ex:0xFF).
    [!ELSE!]
        [!VAR "MinimumBits0InWakeupByteDetect" = "floor((150 div $BitLengthInUs) - 1)"!][!// Already have 1 bit0 of Start bit, so number of bit0 - 1 Start bit.
    [!ENDIF!]
    [!// In driver, checking the result of (byte received & BaseWakeupByteDetectInverted)==0 to decide byte received is wakeup byte instead of just checking Rx falling edge detection.
    [!VAR "BaseWakeupByteDetectInverted" = "bit:and(bit:not(bit:shl(255,$MinimumBits0InWakeupByteDetect)),255)"!]
    [!VAR "WakeupDetectDuration" = "($MinimumBits0InWakeupByteDetect + 1) * $BitLengthInUs"!]

    [!VAR "NodeType" = "'(boolean)FLEXIO_LIN_IP_SLAVE'"!]
    [!IF "contains(node:value(LinNodeType), 'MASTER')"!]
        [!VAR "NodeType" = "'(boolean)FLEXIO_LIN_IP_MASTER'"!]
    [!ENDIF!]
    [!ENDNOCODE!]
    [!CODE!]
    (uint8)[!"num:i(substring-after(LinHwChannel, 'FLEXIO_IP_'))"!]U, /*!< Instance */
    (uint8)[!"$TxShifterID"!]U, /*!< TxShifterId */
    (uint8)[!"$TxTimerID"!]U, /*!< TxTimerId */
    (uint8)[!"$TxPinID"!]U, /*!< TxPinId */
    (uint8)[!"$RxShifterID"!]U, /*!< RxShifterId */
    (uint8)[!"$RxTimerID"!]U, /*!< RxTimerId */
    (uint8)[!"$RxPinID"!]U, /*!< RxPinId */
    (uint8)0U, /*!< u8FlexioHwInstance */
    (uint8)[!"num:i(substring-after(BreakLength, 'BL_'))"!]U, /*!< MasterBreakLength */
    (uint8)11U, /*!< SlaveSyncBreakLength */
    (uint8)[!"num:i($FlexioTIMDEC)"!],/* Timer clock source */
    (uint16)[!"$DivBest"!], /*!< Baudratedivider */
    [!"$NodeType"!], /*!< NodeFunction */
    (Flexio_Lin_Ip_CallbackType)Flexio_Lin_Ipw_Callback, /*!< Callback function to invoke after receiving a byte or transmitting a byte. */
    &Flexio_Lin_Ip_StateStructure[[!"num:i($CurrentChannel)"!]U], /*!< Runtime state structure refference/ */
    (uint32)[!"$Baudrate"!], /*!< Baudrate. ActualBaudrate: [!"$ActualBaudrate"!] */
#if (FLEXIO_LIN_IP_FRAME_TIMEOUT_DISABLE == STD_OFF)
#ifdef FLEXIO_LIN_IP_START_TIMER_NOTIFICATION
    (uint32)[!"num:i($HeaderTimeoutValue)"!], /*!< HeaderTimeoutValue in microseconds */
    (uint32)[!"num:i($ResponseTimeoutValue)"!], /*!< ResponseTimeoutValue in microseconds for 1 byte */
#endif
#endif /* (FLEXIO_LIN_IP_FRAME_TIMEOUT_DISABLE == STD_OFF) */
    (uint8)[!"num:i($WakeupByte)"!], /*!< Wakeup byte to generate wake up pulse [!"$WakeupPulseDuration"!]us */
    (uint8)[!"num:i($BaseWakeupByteDetectInverted)"!] /*!< Byte use to check a wake up pulse [!"$WakeupDetectDuration"!]us detection */
};
    [!ENDCODE!]
    [!VAR "CurrentChannel" = "$CurrentChannel + 1"!]

[!ENDIF!][!//
[!ENDLOOP!]

#define LIN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Lin_MemMap.h"
/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */
