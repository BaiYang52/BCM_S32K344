/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : I3C
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

[!CODE!][!//
#ifndef I3C_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_H
#define I3C_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_H
[!ENDCODE!][!//

/**
*   @file
*
*   @addtogroup i3c_ip_config I3c IPL Configuration
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "I3c_Ip_Types.h"

[!NOCODE!][!//
[!//INCLUDE "Adc_VersionCheck_Src.m"!][!// TODO: add version checking
[!INCLUDE "I3c_RegOperations.m"!][!//
[!ENDNOCODE!][!//
/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define  I3C_IP_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG                    43
#define  I3C_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG     4
#define  I3C_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG     4
#define  I3C_IP_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG  0
#define  I3C_IP_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG             2
#define  I3C_IP_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG             0
#define  I3C_IP_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG             1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and I3c_Ip_Types header file are of the same vendor */
#if (I3C_IP_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG != I3C_IP_VENDOR_ID_TYPES)
    #error "I3c_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and I3c_Ip_Types.h have different vendor ids"
#endif

/* Check if current file and I3c_Ip_Types header file are of the same Autosar version */
#if ((I3C_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG     != I3C_IP_AR_RELEASE_MAJOR_VERSION_TYPES) || \
     (I3C_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG     != I3C_IP_AR_RELEASE_MINOR_VERSION_TYPES) || \
     (I3C_IP_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG  != I3C_IP_AR_RELEASE_REVISION_VERSION_TYPES))
    #error "AutoSar Version Numbers of I3c_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and I3c_Ip_Types.h are different"
#endif

/* Check if current file and I3c_Ip_Types header file are of the same software version */
#if ((I3C_IP_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG != I3C_IP_SW_MAJOR_VERSION_TYPES) || \
     (I3C_IP_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG != I3C_IP_SW_MINOR_VERSION_TYPES) || \
     (I3C_IP_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG != I3C_IP_SW_PATCH_VERSION_TYPES))
    #error "Software Version Numbers of I3c_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and I3c_Ip_Types.h are different"
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
#define I3C_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "I3c_MemMap.h"
[!//
[!CALL "I3c_Ip_MasterConfig" , "MacroGenerationType" = "'declaration_config'"!][!//
[!CALL "I3c_Ip_SlaveConfig" , "MacroGenerationType" = "'declaration_config'"!][!//
[!//
#define I3C_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "I3c_MemMap.h"

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/
#define I3C_START_SEC_CODE
#include "I3c_MemMap.h"
[!//
[!CALL "I3c_Ip_MasterConfig" , "MacroGenerationType" = "'declaration_callback'"!][!//
[!CALL "I3c_Ip_SlaveConfig" , "MacroGenerationType" = "'declaration_callback'"!][!//
[!//
#define I3C_STOP_SEC_CODE
#include "I3c_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

[!CODE!][!//
#endif /* I3C_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_H */
[!ENDCODE!][!//
