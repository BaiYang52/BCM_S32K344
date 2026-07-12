[!NOCODE!][!//
/**
    @file    LIN_VersionCheck_Inc.m
    @version 2.0.1

    @brief   AUTOSAR Lin - Brief description.
    @details Version check variables vor Lin_PBcfg.c

    Project RTD AUTOSAR 4.4
    Patform CORTEXM
    Peripheral LPUART_FLEXIO
    Dependencies none

    ARVersion 4.4.0
    ARRevision ASR_REL_4_4_REV_0000
    ARConfVariant
    SWVersion 2.0.1
    BuildVersion S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707

    (c) Copyright 2020 - 2022 NXP Semiconductors. 
    All Rights Reserved.
*/
/*==================================================================================================
==================================================================================================*/
[!/* avoid multiple inclusion */!]
[!IF "not(var:defined('LIN_VERSION_CHECK_INC_PB_M'))"!]
[!VAR "LIN_VERSION_CHECK_INC_PB_M"="'true'"!]
[!ENDIF!]

[!VAR "LIN_AR_MAJOR_VERSION_TEMPLATE"="4"!][!//
[!VAR "LIN_AR_MINOR_VERSION_TEMPLATE"="4"!][!//
[!VAR "LIN_AR_PATCH_VERSION_TEMPLATE"="0"!][!//
[!VAR "LIN_SW_MAJOR_VERSION_TEMPLATE"="2"!][!//
[!VAR "LIN_SW_MINOR_VERSION_TEMPLATE"="0"!][!//
[!VAR "LIN_SW_PATCH_VERSION_TEMPLATE"="1"!][!//
[!ENDNOCODE!][!//

[!SELECT "CommonPublishedInformation"!][!//
[!IF "num:i(ArReleaseMajorVersion) != num:i($LIN_AR_MAJOR_VERSION_TEMPLATE)"!]
    [!ERROR!]
          **** AUTOSAR major version number of the Basic Software Module Description file (Lin.epd version [!"ArReleaseMajorVersion"!]) and the Code template file (Lin_[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!]PBCfg.h  version [!"num:i($LIN_AR_MAJOR_VERSION_TEMPLATE)"!]) are different ****
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "num:i(ArReleaseMinorVersion)  != num:i($LIN_AR_MINOR_VERSION_TEMPLATE)"!]
    [!ERROR!]
          **** AUTOSAR minor version number of the Basic Software Module Description file (Lin.epd version [!"ArReleaseMinorVersion"!]) and the Code template file (Lin_[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!]PBCfg.h  version [!"num:i($LIN_AR_MINOR_VERSION_TEMPLATE)"!]) are different ****
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "num:i(ArReleaseRevisionVersion)  != num:i($LIN_AR_PATCH_VERSION_TEMPLATE)"!]
    [!ERROR!]
          **** AUTOSAR patch version number of the Basic Software Module Description file (Lin.epd version [!"ArReleaseRevisionVersion"!]) and the Code template file (Lin_[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!]PBCfg.h version [!"num:i($LIN_AR_PATCH_VERSION_TEMPLATE)"!]) are different ****
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "num:i(SwMajorVersion) != num:i($LIN_SW_MAJOR_VERSION_TEMPLATE)"!]
    [!ERROR!]
          **** The software major number of the Basic Software Module Description file (Lin.epd version [!"SwMajorVersion"!]) and the Code template file (Lin_[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!]PBCfg.h version [!"num:i($LIN_SW_MAJOR_VERSION_TEMPLATE)"!]) are different ****
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "num:i(SwMinorVersion) != num:i($LIN_SW_MINOR_VERSION_TEMPLATE)"!]
    [!ERROR!]
          **** The software minor number of the Basic Software Module Description file (Lin.epd version [!"SwMinorVersion"!]) and the Code template file (Lin_[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!]PBCfg.h version [!"num:i($LIN_SW_MINOR_VERSION_TEMPLATE)"!]) are different ****
    [!ENDERROR!]
[!ENDIF!][!//
[!IF "not(num:i(SwPatchVersion) = num:i($LIN_SW_PATCH_VERSION_TEMPLATE))"!]
    [!ERROR!]
      **** The software patch number of the Basic Software Module Description file (Lin.epd version [!"SwPatchVersion"!]) and the Code template file (Lin_[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!]PBCfg.h version [!"num:i($LIN_SW_PATCH_VERSION_TEMPLATE)"!]) are different ****
    [!ENDERROR!]
[!ENDIF!]
[!ENDSELECT!][!//
[!NOCODE!][!//
[!ENDNOCODE!][!//