/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : FLEXIO
*   Dependencies         : 
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

[!VAR "UART_AR_MAJOR_VERSION_TEMPLATE"="4"!][!//
[!VAR "UART_AR_MINOR_VERSION_TEMPLATE"="4"!][!//
[!VAR "UART_AR_PATCH_VERSION_TEMPLATE"="0"!][!//
[!VAR "UART_SW_MAJOR_VERSION_TEMPLATE"="2"!][!//
[!VAR "UART_SW_MINOR_VERSION_TEMPLATE"="0"!][!//
[!VAR "UART_SW_PATCH_VERSION_TEMPLATE"="1"!][!//
[!ENDNOCODE!][!//
[!SELECT "CommonPublishedInformation"!][!//
[!IF "num:i(ArReleaseMajorVersion) != num:i($UART_AR_MAJOR_VERSION_TEMPLATE)"!]
[!ERROR!]
      **** AUTOSAR major version number of the Basic Software Module Description file (Uart.epd version [!"ArReleaseMajorVersion"!]) and the Code template file (Uart_PBcfg.c  version [!"num:i($UART_AR_MAJOR_VERSION_TEMPLATE)"!]) are different ****
[!ENDERROR!]
[!ENDIF!][!//
[!IF "num:i(ArReleaseMinorVersion)  != num:i($UART_AR_MINOR_VERSION_TEMPLATE)"!]
[!ERROR!]
      **** AUTOSAR minor version number of the Basic Software Module Description file (Uart.epd version [!"ArReleaseMinorVersion"!]) and the Code template file (Uart_PBcfg.c  version [!"num:i($UART_AR_MINOR_VERSION_TEMPLATE)"!]) are different ****
[!ENDERROR!]
[!ENDIF!][!//
[!IF "num:i(ArReleaseRevisionVersion)  != num:i($UART_AR_PATCH_VERSION_TEMPLATE)"!]
[!ERROR!]
      **** AUTOSAR patch version number of the Basic Software Module Description file (Uart.epd version [!"ArReleaseRevisionVersion"!]) and the Code template file (Uart_PBcfg.c  version [!"num:i($UART_AR_PATCH_VERSION_TEMPLATE)"!]) are different ****
[!ENDERROR!]
[!ENDIF!][!//
[!IF "num:i(SwMajorVersion) != num:i($UART_SW_MAJOR_VERSION_TEMPLATE)"!]
[!ERROR!]
      **** The software major number of the Basic Software Module Description file (Uart.epd version [!"SwMajorVersion"!]) and the Code template file (Uart_PBcfg.c  version [!"num:i($UART_SW_MAJOR_VERSION_TEMPLATE)"!]) are different ****
[!ENDERROR!]
[!ENDIF!][!//
[!IF "num:i(SwMinorVersion) != num:i($UART_SW_MINOR_VERSION_TEMPLATE)"!]
[!ERROR!]
      **** The software minor number of the Basic Software Module Description file (Uart.epd version [!"SwMinorVersion"!]) and the Code template file (Uart_PBcfg.c  version [!"num:i($UART_SW_MINOR_VERSION_TEMPLATE)"!]) are different ****
[!ENDERROR!]
[!ENDIF!][!//
[!IF "num:i(SwPatchVersion) != num:i($UART_SW_PATCH_VERSION_TEMPLATE)"!]
[!ERROR!]
      **** The software patch number of the Basic Software Module Description file (Uart.epd version [!"SwPatchVersion"!]) and the Code template file (Uart_PBcfg.c  version [!"num:i($UART_SW_PATCH_VERSION_TEMPLATE)"!]) are different ****
[!ENDERROR!]
[!ENDIF!][!//
[!ENDSELECT!][!//
[!NOCODE!][!//
[!ENDNOCODE!][!//
