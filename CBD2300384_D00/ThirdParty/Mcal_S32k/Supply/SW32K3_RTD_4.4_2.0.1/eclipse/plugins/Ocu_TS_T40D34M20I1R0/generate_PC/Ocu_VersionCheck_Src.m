[!NOCODE!][!//

/*!
*   @file    Ocu_VersionCheck_Src.m
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
[!IF "not(var:defined('OCU_VERSION_CHECK_SRC_M'))"!]
[!VAR "OCU_VERSION_CHECK_SRC_M"="'true'"!]


[!VAR "OCU_AR_MAJOR_VERSION_TEMPLATE"="4"!][!//
[!VAR "OCU_AR_MINOR_VERSION_TEMPLATE"="4"!][!//
[!VAR "OCU_AR_REVISION_VERSION_TEMPLATE"="0"!][!//
[!VAR "OCU_SW_MAJOR_VERSION_TEMPLATE"="2"!][!//
[!VAR "OCU_SW_MINOR_VERSION_TEMPLATE"="0"!][!//

[!SELECT "CommonPublishedInformation"!][!//
[!ASSERT "ArReleaseMajorVersion   = num:i($OCU_AR_MAJOR_VERSION_TEMPLATE)"!]
        **** AUTOSAR major version number of the Basic Software Module Description file (Ocu.epd version [!"ArReleaseMajorVersion"!]) and the Code template files (Ocu_Cfg.c version [!"num:i($OCU_AR_MAJOR_VERSION_TEMPLATE)"!]) are different ****
[!ENDASSERT!][!//
[!ASSERT "ArReleaseMinorVersion  = num:i($OCU_AR_MINOR_VERSION_TEMPLATE)"!]
        **** AUTOSAR minor version number of the Basic Software Module Description file (Ocu.epd version [!"ArReleaseMinorVersion"!]) and the Code template files ( Ocu_Cfg.c version [!"num:i($OCU_AR_MINOR_VERSION_TEMPLATE)"!]) are different ****
[!ENDASSERT!][!//
[!IF "not(num:i(ArReleaseRevisionVersion) = num:i($OCU_AR_REVISION_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "AUTOSAR release revision version number of the Basic Software Module Description file (Pwm.epd version [!"ArReleaseRevisionVersion"!]) and the Code template files (Pwm_Cfg.h version [!"num:i($PWM_AR_REVISION_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!ASSERT "SwMajorVersion = num:i($OCU_SW_MAJOR_VERSION_TEMPLATE)"!]
        **** The software major number of the Basic Software Module Description file (Ocu.epd version [!"SwMajorVersion"!]) and the Code template files (Ocu_Cfg.c version [!"num:i($OCU_SW_MAJOR_VERSION_TEMPLATE)"!]) are different ****
[!ENDASSERT!][!//
[!ASSERT "SwMinorVersion = num:i($OCU_SW_MINOR_VERSION_TEMPLATE)"!]
        **** The software minor number of the Basic Software Module Description file (Ocu.epd version [!"SwMinorVersion"!]) and the Code template files ( Ocu_Cfg.c version [!"num:i($OCU_SW_MINOR_VERSION_TEMPLATE)"!]) are different ****
[!ENDASSERT!][!//
[!ENDSELECT!][!//
[!NOCODE!][!//
[!ENDNOCODE!][!//


[!ENDIF!][!// avoid multiple inclusion ENDIF

