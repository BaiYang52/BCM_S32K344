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
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

#ifndef TSPC_PORT_IP_CFG_H
#define TSPC_PORT_IP_CFG_H

/**
*   @file    Tspc_Port_Ip_Cfg.h
*
*   @addtogroup Port_CFG
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
#include "Tspc_Port_Ip_Types.h"
[!INDENT "0"!][!//
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!IF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k312')"!][!//
#include "S32K312_TSPC.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k314')"!][!//
#include "S32K314_TSPC.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k324')"!][!//
#include "S32K324_TSPC.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k342')"!][!//
#include "S32K342_TSPC.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k341')"!][!//
/* In conclusion, S32K341 will use the header files from S32K342 */
#include "S32K342_TSPC.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k322')"!][!//
#include "S32K322_TSPC.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k344')"!][!//
#include "S32K344_TSPC.h"
    [!ENDIF!][!//
[!ELSE!][!//
    [!ERROR "The Resource plugin or ResourceSubderivative field is not available for header file inclusion."!][!//
[!ENDIF!][!//
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
#include "Tspc_Port_Ip_[!"."!]_PBcfg.h"
[!ENDLOOP!][!//
[!ELSE!][!//
#include "Tspc_Port_Ip_PBcfg.h"
[!ENDIF!][!//
[!ENDINDENT!]
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define TSPC_PORT_IP_VENDOR_ID_CFG_H                       43
#define TSPC_PORT_IP_AR_RELEASE_MAJOR_VERSION_CFG_H        4
#define TSPC_PORT_IP_AR_RELEASE_MINOR_VERSION_CFG_H        4
#define TSPC_PORT_IP_AR_RELEASE_REVISION_VERSION_CFG_H     0
#define TSPC_PORT_IP_SW_MAJOR_VERSION_CFG_H                2
#define TSPC_PORT_IP_SW_MINOR_VERSION_CFG_H                0
#define TSPC_PORT_IP_SW_PATCH_VERSION_CFG_H                1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if the files Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_Types.h are of the same version */
#if (TSPC_PORT_IP_VENDOR_ID_CFG_H != TSPC_PORT_IP_TYPES_VENDOR_ID_H)
    #error "Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_Types.h have different vendor ids"
#endif
/* Check if Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_Types.h are of the same Autosar version */
#if ((TSPC_PORT_IP_AR_RELEASE_MAJOR_VERSION_CFG_H    != TSPC_PORT_IP_TYPES_AR_RELEASE_MAJOR_VERSION_H) || \
     (TSPC_PORT_IP_AR_RELEASE_MINOR_VERSION_CFG_H    != TSPC_PORT_IP_TYPES_AR_RELEASE_MINOR_VERSION_H) || \
     (TSPC_PORT_IP_AR_RELEASE_REVISION_VERSION_CFG_H != TSPC_PORT_IP_TYPES_AR_RELEASE_REVISION_VERSION_H) \
    )
    #error "AutoSar Version Numbers of Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_Types.h are different"
#endif
/* Check if Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_Types.h are of the same Software version */
#if ((TSPC_PORT_IP_SW_MAJOR_VERSION_CFG_H != TSPC_PORT_IP_TYPES_SW_MAJOR_VERSION_H) || \
     (TSPC_PORT_IP_SW_MINOR_VERSION_CFG_H != TSPC_PORT_IP_TYPES_SW_MINOR_VERSION_H) || \
     (TSPC_PORT_IP_SW_PATCH_VERSION_CFG_H != TSPC_PORT_IP_TYPES_SW_PATCH_VERSION_H)    \
    )
    #error "Software Version Numbers of Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_Types.h are different"
#endif

[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
/* Check if the files Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_PBcfg.h are of the same version */
#if (TSPC_PORT_IP_VENDOR_ID_CFG_H != TSPC_PORT_IP_VENDOR_ID_[!"."!]_PBCFG_H)
    #error "Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_[!"."!]_PBcfg.h have different vendor IDs"
#endif
 /* Check if the files Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_PBcfg.h are of the same Autosar version */
#if ((TSPC_PORT_IP_AR_RELEASE_MAJOR_VERSION_CFG_H != TSPC_PORT_IP_AR_RELEASE_MAJOR_VERSION_[!"."!]_PBCFG_H) ||   \
     (TSPC_PORT_IP_AR_RELEASE_MINOR_VERSION_CFG_H != TSPC_PORT_IP_AR_RELEASE_MINOR_VERSION_[!"."!]_PBCFG_H) ||   \
     (TSPC_PORT_IP_AR_RELEASE_REVISION_VERSION_CFG_H != TSPC_PORT_IP_AR_RELEASE_REVISION_VERSION_[!"."!]_PBCFG_H) \
    )
    #error "AutoSar Version Numbers of Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_[!"."!]_PBcfg.h are different"
#endif
/* Check if the files Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_PBcfg.h are of the same software version */
#if ((TSPC_PORT_IP_SW_MAJOR_VERSION_CFG_H != TSPC_PORT_IP_SW_MAJOR_VERSION_[!"."!]_PBCFG_H) || \
     (TSPC_PORT_IP_SW_MINOR_VERSION_CFG_H != TSPC_PORT_IP_SW_MINOR_VERSION_[!"."!]_PBCFG_H) || \
     (TSPC_PORT_IP_SW_PATCH_VERSION_CFG_H != TSPC_PORT_IP_SW_PATCH_VERSION_[!"."!]_PBCFG_H)     \
    )
    #error "Software Version Numbers of Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_[!"."!]_PBcfg.h are different"
#endif
[!ENDLOOP!]
[!ELSE!]
/* Check if the files Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_PBcfg.h are of the same version */
#if (TSPC_PORT_IP_VENDOR_ID_CFG_H != TSPC_PORT_IP_VENDOR_ID_PBCFG_H)
    #error "Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_PBcfg.h have different vendor IDs"
#endif
 /* Check if the files Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_PBcfg.h are of the same Autosar version */
#if ((TSPC_PORT_IP_AR_RELEASE_MAJOR_VERSION_CFG_H != TSPC_PORT_IP_AR_RELEASE_MAJOR_VERSION_PBCFG_H) ||   \
     (TSPC_PORT_IP_AR_RELEASE_MINOR_VERSION_CFG_H != TSPC_PORT_IP_AR_RELEASE_MINOR_VERSION_PBCFG_H) ||   \
     (TSPC_PORT_IP_AR_RELEASE_REVISION_VERSION_CFG_H != TSPC_PORT_IP_AR_RELEASE_REVISION_VERSION_PBCFG_H) \
    )
    #error "AutoSar Version Numbers of Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_PBcfg.h are different"
#endif
/* Check if the files Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_PBcfg.h are of the same software version */
#if ((TSPC_PORT_IP_SW_MAJOR_VERSION_CFG_H != TSPC_PORT_IP_SW_MAJOR_VERSION_PBCFG_H) || \
     (TSPC_PORT_IP_SW_MINOR_VERSION_CFG_H != TSPC_PORT_IP_SW_MINOR_VERSION_PBCFG_H) || \
     (TSPC_PORT_IP_SW_PATCH_VERSION_CFG_H != TSPC_PORT_IP_SW_PATCH_VERSION_PBCFG_H)     \
    )
#error "Software Version Numbers of Tspc_Port_Ip_Cfg.h and Tspc_Port_Ip_PBcfg.h are different"
#endif
[!ENDIF!]
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
/* Pre-processor switch to enable/disable Touch Sense support */
#define FEATURE_TSPC_PORT_IP_SUPPORT      ([!IF "PortGeneral/PortTspcSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*! @brief User number of configured pins */
[!SELECT "PortConfigSet"!][!//
#define NUM_OF_OBE_GROUP_CONFIG [!"num:i(count(PortContainer/*/PortPin/*))"!]
[!ENDSELECT!][!//

#define PORT_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Port_MemMap.h"

[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
TSPC_PORT_CONFIG_[!"."!]_PB \
[!ENDLOOP!][!//
[!ELSE!][!//
TSPC_PORT_CONFIG_PB
[!ENDIF!]

#define PORT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Port_MemMap.h"
/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* TSPC_PORT_IP_CFG_H */
