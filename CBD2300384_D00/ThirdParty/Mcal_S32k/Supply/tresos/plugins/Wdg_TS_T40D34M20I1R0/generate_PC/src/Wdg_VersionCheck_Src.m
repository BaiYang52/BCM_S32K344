[!NOCODE!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Swt
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
*   @file
*
*   @addtogroup Wdg
*   @{
*/

[!VAR "WdgArReleaseMajorVersionTemplate"="4"!][!//
[!VAR "WdgArReleaseMinorVersionTemplate"="4"!][!//
[!VAR "WdgArReleasePatchVersionTemplate"="0"!][!//
[!VAR "WdgSwMajorVersionTemplate"="2"!][!//     
[!VAR "WdgSwMinorVersionTemplate"="0"!][!//
[!VAR "WdgSwPatchVersionTemplate"="1"!][!//   
[!ENDNOCODE!][!//
[!SELECT "CommonPublishedInformation"!][!//
[!IF "not(num:i(ArReleaseMajorVersion) = num:i($WdgArReleaseMajorVersionTemplate))"!]
    [!ERROR!]
        "AUTOSAR major version number of the Basic Software Module Description file (Wdg.epd version [!"ArReleaseMajorVersion"!]) and the Code template file (Wdg_Cfg.c version [!"num:i($WdgArReleaseMajorVersionTemplate)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(ArReleaseMinorVersion)  = num:i($WdgArReleaseMinorVersionTemplate))"!]
    [!ERROR!]
        "AUTOSAR minor version number of the Basic Software Module Description file (Wdg.epd version [!"ArReleaseMinorVersion"!]) and the Code template file (Wdg_Cfg.c version [!"num:i($WdgArReleaseMinorVersionTemplate)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(ArReleaseRevisionVersion)  = num:i($WdgArReleasePatchVersionTemplate))"!]
    [!ERROR!]
        "AUTOSAR patch version number of the Basic Software Module Description file (Wdg.epd version [!"ArReleaseRevisionVersion"!]) and the Code template file (Wdg_Cfg.c version [!"num:i($WdgArReleasePatchVersionTemplate)"!]) are different
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(SwMajorVersion) = num:i($WdgSwMajorVersionTemplate))"!]
    [!ERROR!]
        "The software major number of the Basic Software Module Description file (Wdg.epd version [!"SwMajorVersion"!]) and the Code template file (Wdg_Cfg.c version [!"num:i($WdgSwMajorVersionTemplate)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(SwMinorVersion) = num:i($WdgSwMinorVersionTemplate))"!]
    [!ERROR!]
        "The software minor number of the Basic Software Module Description file (Wdg.epd version [!"SwMinorVersion"!]) and the Code template file (Wdg_Cfg.c version [!"num:i($WdgSwMinorVersionTemplate)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(SwPatchVersion) = num:i($WdgSwPatchVersionTemplate))"!]
    [!ERROR!]
        "The software patch number of the Basic Software Module Description file (Wdg.epd version [!"SwPatchVersion"!]) and the Code template file (Wdg_Cfg.c version [!"num:i($WdgSwPatchVersionTemplate)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!ENDSELECT!][!//
[!NOCODE!][!//
[!ENDNOCODE!][!//