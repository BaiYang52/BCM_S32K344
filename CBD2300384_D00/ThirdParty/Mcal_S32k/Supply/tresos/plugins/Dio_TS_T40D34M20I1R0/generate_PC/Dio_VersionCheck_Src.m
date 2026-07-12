[!NOCODE!][!//
/**
*   @file    Dio_VersionCheck_Src.m
*   @version 2.0.1
*
*   @brief   AUTOSAR Dio - version check macro.
*   @details Version checks.
*
*   @addtogroup DIO
*   @{
*/
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : SIUL2
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
==================================================================================================*/
[!/* avoid multiple inclusion */!]
[!IF "not(var:defined('DIO_VERSIONCHECK_SRC_M'))"!]
[!VAR "DIO_VERSIONCHECK_SRC_M"="'true'"!]
[!VAR "DIO_AR_RELEASE_MAJOR_VERSION_TEMPLATE"="4"!][!//
[!VAR "DIO_AR_RELEASE_MINOR_VERSION_TEMPLATE"="4"!][!//
[!VAR "DIO_AR_RELEASE_REVISION_VERSION_TEMPLATE"="0"!][!//
[!VAR "DIO_SW_MAJOR_VERSION_TEMPLATE"="2"!][!//
[!VAR "DIO_SW_MINOR_VERSION_TEMPLATE"="0"!][!//
[!VAR "DIO_SW_PATCH_VERSION_TEMPLATE"="1"!][!//

[!SELECT "CommonPublishedInformation"!][!//

[!IF "not(num:i(ArReleaseMajorVersion) = num:i($DIO_AR_RELEASE_MAJOR_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "AUTOSAR release major version number of the Basic Software Module Description file (Dio.epd version [!"ArReleaseMajorVersion"!]) and the Code template file (Dio_Cfg.c version [!"num:i($DIO_AR_RELEASE_MAJOR_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!]
[!IF "not(num:i(ArReleaseMinorVersion)  = num:i($DIO_AR_RELEASE_MINOR_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "AUTOSAR release minor version number of the Basic Software Module Description file (Dio.epd version [!"ArReleaseMinorVersion"!]) and the Code template file (Dio_Cfg.c version [!"num:i($DIO_AR_RELEASE_MINOR_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!]
[!IF "not(num:i(ArReleaseRevisionVersion)  = num:i($DIO_AR_RELEASE_REVISION_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "AUTOSAR release revision version number of the Basic Software Module Description file (Dio.epd version [!"ArReleaseRevisionVersion"!]) and the Code template file (Dio_Cfg.c version [!"num:i($DIO_AR_RELEASE_REVISION_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!]
[!IF "not(num:i(SwMajorVersion) = num:i($DIO_SW_MAJOR_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "The software major number of the Basic Software Module Description file (Dio.epd version [!"SwMajorVersion"!]) and the Code template file (Dio_Cfg.c version [!"num:i($DIO_SW_MAJOR_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!]
[!IF "not(num:i(SwMinorVersion) = num:i($DIO_SW_MINOR_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "The software minor number of the Basic Software Module Description file (Dio.epd version [!"SwMinorVersion"!]) and the Code template file (Dio_Cfg.c version [!"num:i($DIO_SW_MINOR_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!]
[!IF "not(num:i(SwPatchVersion) = num:i($DIO_SW_PATCH_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "The software patch number of the Basic Software Module Description file (Dio.epd version [!"SwPatchVersion"!]) and the Code template file (Dio_Cfg.c version [!"num:i($DIO_SW_PATCH_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!]
[!ENDSELECT!][!//

[!ENDIF!][!// avoid multiple inclusion ENDIF
[!ENDNOCODE!][!//
