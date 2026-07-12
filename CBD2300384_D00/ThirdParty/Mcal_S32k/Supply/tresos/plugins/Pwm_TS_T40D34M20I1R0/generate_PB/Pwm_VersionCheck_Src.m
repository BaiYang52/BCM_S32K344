[!NOCODE!][!//

/*!
*   @file       Pwm_VersionCheck_Src_PB.m
*   @version    2.0.1
*
*   @brief      AUTOSAR Pwm macro to check for notification config, variant post-build
*   @details    Macro to check for notification config, variant post-build
*
*   @addtogroup PWM_MODULE
*   @{
*/
/*!==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Emios Flexio
*   Dependencies         : none
*
*   AUTOSAR Version      : 4.4.0
*   AUTOSAR Revision     : ASR_REL_4_4_REV_0000
*   AUTOSAR Conf.Variant :
*   SW Version           : 2.0.1
*   Build Version        : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
*   (c) Copyright 2020 - 2022 NXP Semiconductors
*   All Rights Reserved.
==================================================================================================*/

[!ENDNOCODE!][!//


[!/* avoid multiple inclusion */!]
[!IF "not(var:defined('PWM_VERSION_CHECK_SRC_PB_M'))"!]
[!VAR "PWM_VERSION_CHECK_SRC_PB_M"="'true'"!]


[!VAR "PWM_AR_MAJOR_VERSION_TEMPLATE"="4"!][!//
[!VAR "PWM_AR_MINOR_VERSION_TEMPLATE"="4"!][!//
[!VAR "PWM_AR_REVISION_VERSION_TEMPLATE"="0"!][!//
[!VAR "PWM_SW_MAJOR_VERSION_TEMPLATE"="2"!][!//
[!VAR "PWM_SW_MINOR_VERSION_TEMPLATE"="0"!][!//
[!VAR "PWM_SW_PATCH_VERSION_TEMPLATE"="1"!][!//

[!SELECT "CommonPublishedInformation"!][!//

[!IF "not(num:i(ArReleaseMajorVersion) = num:i($PWM_AR_MAJOR_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "AUTOSAR major version number of the Basic Software Module Description file (Pwm.epd version [!"ArReleaseMajorVersion"!]) and the Code template file (Pwm_PBcfg.c  version [!"num:i($PWM_AR_MAJOR_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(ArReleaseMinorVersion) = num:i($PWM_AR_MINOR_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "AUTOSAR release minor version number of the Basic Software Module Description file (Pwm.epd version [!"ArReleaseMinorVersion"!]) and the Code template files (Pwm_PBcfg.c  version [!"num:i($PWM_AR_MINOR_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(ArReleaseRevisionVersion) = num:i($PWM_AR_REVISION_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "AUTOSAR release revision version number of the Basic Software Module Description file (Pwm.epd version [!"ArReleaseRevisionVersion"!]) and the Code template files (Pwm_PBcfg.c  version [!"num:i($PWM_AR_REVISION_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(SwMajorVersion) = num:i($PWM_SW_MAJOR_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "The software major number of the Basic Software Module Description file (Pwm.epd version [!"SwMajorVersion"!]) and the Code template files (Pwm_PBcfg.c  version [!"num:i($PWM_SW_MAJOR_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(SwMinorVersion) = num:i($PWM_SW_MINOR_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "The software minor number of the Basic Software Module Description file (Pwm.epd version [!"SwMinorVersion"!]) and the Code template files (Pwm_PBcfg.c  version [!"num:i($PWM_SW_MINOR_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(SwPatchVersion) = num:i($PWM_SW_PATCH_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "The software patch number of the Basic Software Module Description file (Pwm.epd version [!"SwPatchVersion"!]) and the Code template files (Pwm_PBcfg.c  version [!"num:i($PWM_SW_PATCH_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!]


[!ENDSELECT!][!//
[!NOCODE!][!//
[!ENDNOCODE!][!//


[!ENDIF!][!// avoid multiple inclusion ENDIF

