[!NOCODE!][!//

/*!
*   @file    Ocu_VersionCheck_Inc_PB.m
*   @version 2.0.1
*
*   @brief   AUTOSAR Ocu macro to check for notification config, variant post-build
*   @details    macro to check for notification config, variant pre-compile
*
*   @addtogroup ocu_driver Ocu Driver
*   @brief      Ocu HLD
*   @{
*/
/*!==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : eMios
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

[!ENDNOCODE!][!//

[!/* avoid multiple inclusion */!]
[!IF "not(var:defined('OCU_VERSION_CHECK_INC_M'))"!]
[!VAR "OCU_VERSION_CHECK_INC_M"="'true'"!]


[!VAR "OCU_AR_MAJOR_VERSION_TEMPLATE"="4"!][!//
[!VAR "OCU_AR_MINOR_VERSION_TEMPLATE"="4"!][!//
[!VAR "OCU_AR_REVISION_VERSION_TEMPLATE"="0"!][!//
[!VAR "OCU_SW_MAJOR_VERSION_TEMPLATE"="2"!][!//
[!VAR "OCU_SW_MINOR_VERSION_TEMPLATE"="0"!][!//
[!VAR "OCU_SW_PATCH_VERSION_TEMPLATE"="1"!][!//

[!SELECT "CommonPublishedInformation"!][!//

[!IF "not(num:i(ArReleaseMajorVersion) = num:i($OCU_AR_MAJOR_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "AUTOSAR major version number of the Basic Software Module Description file (Ocu.epd version [!"ArReleaseMajorVersion"!]) and the Code template file (Ocu_PBcfg.h version [!"num:i($OCU_AR_MAJOR_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(ArReleaseMinorVersion) = num:i($OCU_AR_MINOR_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "AUTOSAR release minor version number of the Basic Software Module Description file (Ocu.epd version [!"ArReleaseMinorVersion"!]) and the Code template files (Ocu_PBcfg.h version [!"num:i($OCU_AR_MINOR_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(ArReleaseRevisionVersion) = num:i($OCU_AR_REVISION_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "AUTOSAR release revision version number of the Basic Software Module Description file (Ocu.epd version [!"ArReleaseRevisionVersion"!]) and the Code template files (Ocu_PBcfg.h version [!"num:i($OCU_AR_REVISION_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(SwMajorVersion) = num:i($OCU_SW_MAJOR_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "The software major number of the Basic Software Module Description file (Ocu.epd version [!"SwMajorVersion"!]) and the Code template files (Ocu_PBcfg.h version [!"num:i($OCU_SW_MAJOR_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(SwMinorVersion) = num:i($OCU_SW_MINOR_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "The software minor number of the Basic Software Module Description file (Ocu.epd version [!"SwMinorVersion"!]) and the Code template files (Ocu_PBcfg.h version [!"num:i($OCU_SW_MINOR_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(SwPatchVersion) = num:i($OCU_SW_PATCH_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "The software patch number of the Basic Software Module Description file (Ocu.epd version [!"SwPatchVersion"!]) and the Code template files (Ocu_PBcfg.h version [!"num:i($OCU_SW_PATCH_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!]


[!ENDSELECT!][!//
[!NOCODE!][!//
[!ENDNOCODE!][!//

[!ENDIF!][!// avoid multiple inclusion ENDIF

