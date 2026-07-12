[!NOCODE!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : HSE
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

[!VAR "CryptoArMajorVersionTemplate"="4"!][!//
[!VAR "CryptoArMinorVersionTemplate"="4"!][!//
[!VAR "CryptoArPatchVersionTemplate"="0"!][!//
[!VAR "CryptoSwMajorVersionTemplate"="2"!][!//     
[!VAR "CryptoSwMinorVersionTemplate"="0"!][!//     

[!ENDNOCODE!][!//
[!SELECT "CommonPublishedInformation"!][!//
[!/*
[!IF "not(num:i(ArMajorVersion) = num:i($CryptoArMajorVersionTemplate))"!]
    [!ERROR!]
        "AUTOSAR major version number of the Basic Software Module Description file (Crypto.epd version [!"ArMajorVersion"!]) and the Code template file (Crypto_Cfg.h version [!"num:i($CryptoArMajorVersionTemplate)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(ArMinorVersion)  = num:i($CryptoArMinorVersionTemplate))"!]
     [!ERROR!]
        "AUTOSAR minor version number of the Basic Software Module Description file (Crypto.epd version [!"ArMinorVersion"!]) and the Code template file (Crypto_Cfg.h version [!"num:i($CryptoArMinorVersionTemplate)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(ArPatchVersion)  = num:i($CryptoArPatchVersionTemplate))"!]
    [!ERROR!]
        "AUTOSAR patch version number of the Basic Software Module Description file (Crypto.epd version [!"ArPatchVersion"!]) and the Code template file (Crypto_Cfg.h version [!"num:i($CryptoArPatchVersionTemplate)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
*/!]
[!IF "not(num:i(SwMajorVersion) = num:i($CryptoSwMajorVersionTemplate))"!]
    [!ERROR!]
        "The software major number of the Basic Software Module Description file (Crypto.epd version [!"SwMajorVersion"!]) and the Code template file (Crypto_Cfg.h version [!"num:i($CryptoSwMajorVersionTemplate)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(SwMinorVersion) = num:i($CryptoSwMinorVersionTemplate))"!]
    [!ERROR!]   
        "The software minor number of the Basic Software Module Description file (Crypto.epd version [!"SwMinorVersion"!]) and the Code template file (Crypto_Cfg.h version [!"num:i($CryptoSwMinorVersionTemplate)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!ENDSELECT!][!//
[!NOCODE!][!//

[!ENDNOCODE!][!//
