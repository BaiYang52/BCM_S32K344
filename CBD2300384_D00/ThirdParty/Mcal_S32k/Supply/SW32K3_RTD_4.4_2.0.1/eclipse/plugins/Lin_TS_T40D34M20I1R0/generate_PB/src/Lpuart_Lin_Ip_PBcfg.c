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
*   @file   Lpuart_Lin_Ip_PBcfg.c
*   @implements Lpuart_Lin_Ip_PBcfg.c_Artifact
*
*   @addtogroup LPUART_LIN
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

#include "Lpuart_Lin_Ip_Cfg.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define LPUART_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_VENDOR_ID_C                     43
#define LPUART_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C      4
#define LPUART_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MINOR_VERSION_C      4
#define LPUART_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_REVISION_VERSION_C   0
#define LPUART_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MAJOR_VERSION_C              2
#define LPUART_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MINOR_VERSION_C              0
#define LPUART_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_PATCH_VERSION_C              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/* Checks against Lpuart_Lin_Ip_Cfg.h */
#if (LPUART_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_VENDOR_ID_C != LPUART_LIN_IP_CFG_VENDOR_ID)
    #error "Lpuart_Lin_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lpuart_Lin_Ip_Cfg.h have different vendor ids"
#endif
/* Check if current file and Lpuart_Lin_Ip_Cfg header file are of the same Autosar version */
#if ((LPUART_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MAJOR_VERSION_C    != LPUART_LIN_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (LPUART_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MINOR_VERSION_C    != LPUART_LIN_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (LPUART_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_REVISION_VERSION_C != LPUART_LIN_IP_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Lpuart_Lin_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lpuart_Lin_Ip_Cfg.h are different"
#endif
/* Check if current file and Lpuart_Lin_Ip_Cfg header file are of the same Software version */
#if ((LPUART_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MAJOR_VERSION_C != LPUART_LIN_IP_CFG_SW_MAJOR_VERSION) || \
     (LPUART_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MINOR_VERSION_C != LPUART_LIN_IP_CFG_SW_MINOR_VERSION) || \
     (LPUART_LIN_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_PATCH_VERSION_C != LPUART_LIN_IP_CFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Lpuart_Lin_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Lpuart_Lin_Ip_Cfg.h are different"
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
[!NOCODE!]
[!VAR "NumHwChannelConfig" = "0"!]
[!LOOP "LinGlobalConfig/LinChannel/*"!]
[!IF "contains(node:value(LinHwChannel),'LPUART')"!][!//
    [!VAR "NumHwChannelConfig" = "$NumHwChannelConfig + 1"!]
[!ENDIF!]
[!ENDLOOP!][!//
[!ENDNOCODE!]
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

[!IF "$NumHwChannelConfig != 0"!]
[!CODE!]
#define LIN_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Lin_MemMap.h"

extern Lpuart_Lin_Ip_StateStructType Lpuart_Lin_Ip_axStateStructure[LPUART_LIN_IP_NUMBER_OF_INSTANCES];

#define LIN_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Lin_MemMap.h"
[!ENDCODE!]
[!ENDIF!]

#define LIN_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Lin_MemMap.h"
[!NOCODE!]
[!VAR "CurrentChannel" = "0"!]
[!LOOP "LinGlobalConfig/LinChannel/*"!]
[!IF "contains(node:value(LinHwChannel),'LPUART')"!][!//
    [!VAR "Baudrate" = "LinChannelBaudRate"!]

    [!VAR "LinHeaderTimeoutValue" = "ceiling(LinHeaderTimeout * 1000000 * (1 div $Baudrate))"!]
    [!VAR "LinResponseTimeoutValue" = "ceiling((LinResponseTimeout div 10) * 1000000 * (1 div $Baudrate))"!]

    [!// Calculate byte to generate wakeup pulse.
    [!VAR "BitLengthInUs" = "1000000 div $Baudrate"!]
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

[!CODE!]
/**
* @brief          Hardware configuration for Lin Hardware Instance [!"LinHwChannel"!] - Configuration:
*
*
*/
const Lpuart_Lin_Ip_UserConfigType Lpuart_Lin_Ip_pHwConfigPB_[!"LinChannelId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
[!ENDCODE!]
    [!VAR "LinBaudrate" = "LinChannelBaudRate"!]
    [!SELECT "node:ref(LinClockRef)"!]
        [!INCLUDE "Lin_BaudRate_Comp.m"!][!//
    [!ENDSELECT!][!//
[!CODE!]
    [!"num:i(substring-after(LinHwChannel, 'LPUART_IP_'))"!]U, /*!< Hardware instance */

    [!"$BaudrateConf_Value"!]U, /*!< Baudrate divisor */

    16U, /*!< Baudrate OSR */
[!ENDCODE!]
    [!IF "contains(node:value(LinNodeType), 'MASTER')"!]
[!CODE!]
    (boolean)LPUART_LIN_IP_MASTER, /*!< Master node */
[!ENDCODE!]
    [!ELSE!]
[!CODE!]
    (boolean)LPUART_LIN_IP_SLAVE, /*!< Slave node */
[!ENDCODE!]
    [!ENDIF!], /*!< Node function as Master or Slave */
[!CODE!]
    LPUART_LIN_IP_BREAK_CHAR_13_BIT_MINIMUM_U8, /*!< Break character has [!"substring-after(BreakLength, '_')"!] bits */
[!ENDCODE!]
[!CODE!]
    (uint8)1U,    /*!< Detect break character at [!"substring-after(DetectedBreakLength, '_')"!] bits */
    
    (Lpuart_Lin_Ip_CallbackType)Lin_Ipw_Callback, /*!< Callback function to invoke after receiving a byte or transmitting a byte. */

    &Lpuart_Lin_Ip_axStateStructure[[!"num:i($CurrentChannel)"!]U], /*!< Runtime state structure refference/ */
#if (LPUART_LIN_IP_FRAME_TIMEOUT_DISABLE == STD_OFF)
#ifdef LPUART_LIN_IP_START_TIMER_NOTIFICATION
    [!"num:i($LinHeaderTimeoutValue)"!]U,           /*!< HeaderTimeoutValue in microseconds */

    [!"num:i($LinResponseTimeoutValue)"!]U,         /*!< ResponseTimeoutValue in microseconds for 1 byte */
#endif
#endif /* (LPUART_LIN_IP_START_TIMER_NOTIFICATION == STD_OFF) */
    (uint8)[!"num:i($WakeupByte)"!], /*!< Wakeup byte to generate wake up pulse [!"$WakeupPulseDuration"!]us */

    (uint8)[!"num:i($BaseWakeupByteDetectInverted)"!], /*!< Byte use to check a wake up pulse [!"$WakeupDetectDuration"!]us detection */

    [!"num:i($ClockFrequencyFromMcu)"!]U            /*!< Channel clock */
};
[!ENDCODE!]
    [!VAR "CurrentChannel" = "$CurrentChannel + 1"!]
[!ENDIF!]
[!ENDLOOP!][!//
[!ENDNOCODE!]
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
