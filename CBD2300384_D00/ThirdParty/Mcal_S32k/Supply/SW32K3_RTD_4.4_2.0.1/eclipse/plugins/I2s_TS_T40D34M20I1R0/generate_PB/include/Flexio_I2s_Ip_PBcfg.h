/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : I2S
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
#ifndef FLEXIO_I2S_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_H
#define FLEXIO_I2S_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_H
[!ENDCODE!][!//

/**
*   @file
*
*   @addtogroup Flexio_I2s_ip_config Flexio_I2s IPL Configuration
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Flexio_I2s_Ip_Types.h"

[!NOCODE!][!//
[!//INCLUDE "I2s_VersionCheck_Src.m"!][!// TODO: add version checking
[!INCLUDE "I2s_RegOperations.m"!][!//
[!ENDNOCODE!][!//
/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLEXIO_I2S_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_VENDOR_ID                    43
#define FLEXIO_I2S_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION     4
#define FLEXIO_I2S_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION     4
#define FLEXIO_I2S_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION  0
#define FLEXIO_I2S_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION             2
#define FLEXIO_I2S_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION             0
#define FLEXIO_I2S_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and Flexio_I2s_Ip_Types header file are of the same vendor */
#if (FLEXIO_I2S_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_VENDOR_ID != FLEXIO_I2S_IP_TYPES_VENDOR_ID)
#error "Flexio_I2s_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Flexio_I2s_Ip_Types.h have different vendor ids"
#endif

/* Check if current file and Flexio_I2s_Ip_Types header file are of the same Autosar version */
#if ((FLEXIO_I2S_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION     != FLEXIO_I2S_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (FLEXIO_I2S_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION     != FLEXIO_I2S_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (FLEXIO_I2S_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION  != FLEXIO_I2S_IP_TYPES_AR_RELEASE_REVISION_VERSION))
#error "AutoSar Version Numbers of Flexio_I2s_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Flexio_I2s_Ip_Types.h are different"
#endif

/* Check if current file and Flexio_I2s_Ip_Types header file are of the same software version */
#if ((FLEXIO_I2S_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION != FLEXIO_I2S_IP_TYPES_SW_MAJOR_VERSION) || \
     (FLEXIO_I2S_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION != FLEXIO_I2S_IP_TYPES_SW_MINOR_VERSION) || \
     (FLEXIO_I2S_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION != FLEXIO_I2S_IP_TYPES_SW_PATCH_VERSION))
#error "Software Version Numbers of Flexio_I2s_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Flexio_I2s_Ip_Types.h are different"
#endif

/*==================================================================================================
*                         LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
#if (STD_ON == FLEXIO_I2S_IP_ENABLE)

#define I2S_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "I2s_MemMap.h"

#if (FLEXIO_I2S_IP_NUMBER_OF_CHANNEL != 0U)
extern Flexio_I2s_Ip_StateType Flexio_I2s_Ip_MasterSlaveState[FLEXIO_I2S_IP_NUMBER_OF_CHANNEL];
#endif

#define I2S_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "I2s_MemMap.h"

#define I2S_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "I2s_MemMap.h"

[!CALL "Flexio_I2s_Ip_Config" , "MacroGenerationType" = "'declaration'"!][!//

#define I2S_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "I2s_MemMap.h"

/*==================================================================================================
*                                  LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL FUNCTIONS
==================================================================================================*/
#define I2S_START_SEC_CODE
#include "I2s_MemMap.h"

[!CALL "Flexio_I2s_Ip_ConfigCallbackDeclaration"!][!//

#define I2S_STOP_SEC_CODE
#include "I2s_MemMap.h"

#endif /* (STD_ON == FLEXIO_I2S_IP_ENABLE) */

#ifdef __cplusplus
}
#endif

/** @} */

[!CODE!][!//
#endif /* FLEXIO_I2S_IP_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_H */
[!ENDCODE!][!//
