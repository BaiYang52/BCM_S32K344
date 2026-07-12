[!NOCODE!][!//
/**
  @file    Can_VersionCheck_Src_PB.m
  @version 2.0.1

  @brief   AUTOSAR Can - version check macro.
  @details Version checks.

  Project RTD AUTOSAR 4.4
  Platform CORTEXM
  Peripheral FLEXCAN
  Dependencies 

  ARVersion 4.4.0
  ARRevision ASR_REL_4_4_REV_0000
  ARConfVariant
  SWVersion 2.0.1
  BuildVersion S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707

  (c) Copyright 2020 - 2022 NXP Semiconductors
  All Rights Reserved.
*/
/*==================================================================================================
==================================================================================================*/

[!/* avoid multiple inclusion */!]
[!IF "not(var:defined('CAN_VERSION_CHECK_INC_M'))"!]
[!VAR "CAN_VERSION_CHECK_INC_M"="'true'"!]

[!VAR "CAN_AR_RELEASE_MAJOR_VERSION_TEMPLATE"="4"!][!//
[!VAR "CAN_AR_RELEASE_MINOR_VERSION_TEMPLATE"="4"!][!//
[!VAR "CAN_AR_RELEASE_REVISION_VERSION_TEMPLATE"="0"!][!//
[!VAR "CAN_SW_MAJOR_VERSION_TEMPLATE"="2"!][!//
[!VAR "CAN_SW_MINOR_VERSION_TEMPLATE"="0"!][!//
[!VAR "CAN_SW_PATCH_VERSION_TEMPLATE"="1"!][!//

[!SELECT "CommonPublishedInformation"!][!//
[!IF "not(num:i(ArReleaseMajorVersion) = num:i($CAN_AR_RELEASE_MAJOR_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "AUTOSAR major version number of the Basic Software Module Description file (Can.epd version [!"ArReleaseMajorVersion"!]) and the Code template file (Can_PBcfg.c version [!"num:i($CAN_AR_RELEASE_MAJOR_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!]
[!IF "not(num:i(ArReleaseMinorVersion) = num:i($CAN_AR_RELEASE_MINOR_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "AUTOSAR minor version number of the Basic Software Module Description file (Can.epd version [!"ArReleaseMinorVersion"!]) and the Code template file (Can_PBcfg.c version [!"num:i($CAN_AR_RELEASE_MINOR_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!]
[!IF "not(num:i(ArReleaseRevisionVersion) = num:i($CAN_AR_RELEASE_REVISION_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "AUTOSAR patch version number of the Basic Software Module Description file (Can.epd version [!"ArReleaseRevisionVersion"!]) and the Code template file (Can_PBcfg.c version [!"num:i($CAN_AR_RELEASE_REVISION_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!]
[!IF "not(num:i(SwMajorVersion) = num:i($CAN_SW_MAJOR_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "The software major number of the Basic Software Module Description file (Can.epd version [!"SwMajorVersion"!]) and the Code template file (Can_PBcfg.c version [!"num:i($CAN_SW_MAJOR_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!]
[!IF "not(num:i(SwMinorVersion) = num:i($CAN_SW_MINOR_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "The software minor number of the Basic Software Module Description file (Can.epd version [!"SwMinorVersion"!]) and the Code template file (Can_PBcfg.c version [!"num:i($CAN_SW_MINOR_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!]
[!IF "not(num:i(SwPatchVersion) = num:i($CAN_SW_PATCH_VERSION_TEMPLATE))"!]
    [!ERROR!]
        "The software patch number of the Basic Software Module Description file (Can.epd version [!"SwPatchVersion"!]) and the Code template file (Can_PBcfg.c version [!"num:i($CAN_SW_PATCH_VERSION_TEMPLATE)"!]) are different"
    [!ENDERROR!]
[!ENDIF!]
[!ENDSELECT!][!//

[!ENDIF!][!// avoid multiple inclusion ENDIF

[!ENDNOCODE!][!//