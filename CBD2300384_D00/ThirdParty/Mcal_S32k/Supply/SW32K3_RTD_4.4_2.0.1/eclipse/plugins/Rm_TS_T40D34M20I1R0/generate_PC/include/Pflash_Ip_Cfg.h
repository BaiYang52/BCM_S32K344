/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : 
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

#ifndef PFLASH_IP_CFG_H
#define PFLASH_IP_CFG_H

/**
*   @file Pflash_Ip_Cfg.h
*
*   @addtogroup Pflash_Ip Pflash IPV Driver
*   @{
*/


#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Mcal.h"

[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!]#include "Pflash_Ip_[!"."!]_PBcfg.h"[!CR!][!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!]#include "Pflash_Ip_PBcfg.h"[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

#include "Pflash_Ip_Types.h"
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_PFLASH_IP_CFG_VENDOR_ID                    43
#define RM_PFLASH_IP_CFG_AR_RELEASE_MAJOR_VERSION     4
#define RM_PFLASH_IP_CFG_AR_RELEASE_MINOR_VERSION     4
#define RM_PFLASH_IP_CFG_AR_RELEASE_REVISION_VERSION  0
#define RM_PFLASH_IP_CFG_SW_MAJOR_VERSION             2
#define RM_PFLASH_IP_CFG_SW_MINOR_VERSION             0
#define RM_PFLASH_IP_CFG_SW_PATCH_VERSION             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!][!//
/* Checks against Pflash_Ip_[!"."!]_PBcfg.h */
#if (RM_PFLASH_IP_CFG_VENDOR_ID != RM_PFLASH_IP_PBCFG_[!"."!]_VENDOR_ID)
    #error "Pflash_Ip_Cfg.h and Pflash_Ip_[!"."!]_PBcfg.h have different vendor ids"
#endif
#if ((RM_PFLASH_IP_CFG_AR_RELEASE_MAJOR_VERSION    != RM_PFLASH_IP_PBCFG_[!"."!]_AR_RELEASE_MAJOR_VERSION) || \
     (RM_PFLASH_IP_CFG_AR_RELEASE_MINOR_VERSION    != RM_PFLASH_IP_PBCFG_[!"."!]_AR_RELEASE_MINOR_VERSION) || \
     (RM_PFLASH_IP_CFG_AR_RELEASE_REVISION_VERSION != RM_PFLASH_IP_PBCFG_[!"."!]_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Pflash_Ip_Cfg.h and Pflash_Ip_[!"."!]_PBcfg.h are different"
#endif
#if ((RM_PFLASH_IP_CFG_SW_MAJOR_VERSION != RM_PFLASH_IP_PBCFG_[!"."!]_SW_MAJOR_VERSION) || \
     (RM_PFLASH_IP_CFG_SW_MINOR_VERSION != RM_PFLASH_IP_PBCFG_[!"."!]_SW_MINOR_VERSION) || \
     (RM_PFLASH_IP_CFG_SW_PATCH_VERSION != RM_PFLASH_IP_PBCFG_[!"."!]_SW_PATCH_VERSION))
    #error "Software Version Numbers of Pflash_Ip_Cfg.h and Pflash_Ip_[!"."!]_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!][!//
/* Checks against Pflash_Ip_PBcfg.h */
#if (RM_PFLASH_IP_CFG_VENDOR_ID != RM_PFLASH_IP_PBCFG_VENDOR_ID)
    #error "Pflash_Ip_Cfg.h and Pflash_Ip_PBcfg.h have different vendor ids"
#endif
#if ((RM_PFLASH_IP_CFG_AR_RELEASE_MAJOR_VERSION    != RM_PFLASH_IP_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (RM_PFLASH_IP_CFG_AR_RELEASE_MINOR_VERSION    != RM_PFLASH_IP_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (RM_PFLASH_IP_CFG_AR_RELEASE_REVISION_VERSION != RM_PFLASH_IP_PBCFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Pflash_Ip_Cfg.h and Pflash_Ip_PBcfg.h are different"
#endif
#if ((RM_PFLASH_IP_CFG_SW_MAJOR_VERSION != RM_PFLASH_IP_PBCFG_SW_MAJOR_VERSION) || \
     (RM_PFLASH_IP_CFG_SW_MINOR_VERSION != RM_PFLASH_IP_PBCFG_SW_MINOR_VERSION) || \
     (RM_PFLASH_IP_CFG_SW_PATCH_VERSION != RM_PFLASH_IP_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Pflash_Ip_Cfg.h and Pflash_Ip_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Mcal.h */
    #if ((RM_PFLASH_IP_CFG_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (RM_PFLASH_IP_CFG_AR_RELEASE_MINOR_VERSION != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Pflash_Ip_Cfg.h and Mcal.h are different"
    #endif
#endif

/* Checks against Pflash_Ip_Types.h */
#if (RM_PFLASH_IP_CFG_VENDOR_ID != RM_PFLASH_IP_TYPES_VENDOR_ID)
    #error "Pflash_Ip_Cfg.h and Pflash_Ip_Types.h have different vendor ids"
#endif
#if ((RM_PFLASH_IP_CFG_AR_RELEASE_MAJOR_VERSION    != RM_PFLASH_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (RM_PFLASH_IP_CFG_AR_RELEASE_MINOR_VERSION    != RM_PFLASH_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (RM_PFLASH_IP_CFG_AR_RELEASE_REVISION_VERSION != RM_PFLASH_IP_TYPES_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Pflash_Ip_Cfg.h and Pflash_Ip_Types.h are different"
#endif
#if ((RM_PFLASH_IP_CFG_SW_MAJOR_VERSION != RM_PFLASH_IP_TYPES_SW_MAJOR_VERSION) || \
     (RM_PFLASH_IP_CFG_SW_MINOR_VERSION != RM_PFLASH_IP_TYPES_SW_MINOR_VERSION) || \
     (RM_PFLASH_IP_CFG_SW_PATCH_VERSION != RM_PFLASH_IP_TYPES_SW_PATCH_VERSION))
    #error "Software Version Numbers of Pflash_Ip_Cfg.h and Pflash_Ip_Types.h are different"
#endif
/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
[!IF "RmGeneral/RmPflashConfigurable"!]
/**
* @brief   Collection of all configuration structures declarations.
*/

[!NOCODE!]
[!CODE!]#define PFLASH_CONFIG_EXT \[!CR!][!ENDCODE!]
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "variantIndex"="0"!][!//
    [!VAR "variantNumber"="variant:size()"!][!//
[!LOOP "variant:all()"!][!//
    [!VAR "variantIndex"="$variantIndex + 1"!][!//
    [!CODE!][!WS "4"!]PFLASH_CONFIG_[!"."!]_PB[!IF "$variantIndex < $variantNumber"!] \[!CR!][!ENDIF!][!ENDCODE!]
[!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS "4"!]PFLASH_CONFIG_PB[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

#define  RM_IP_ENABLE_PFLASH                            ([!IF "RmGeneral/RmPflashConfigurable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#define  PFLASH_NUM_CONFIG_U32                          ((uint32)[!"num:i(count(RmConfigSet/Pflash_Configuration/PflashMasterProtection/*))"!]UL)

#define  PFLASH_IP_DEV_ERROR_DETECT                     ([!IF "RmConfigSet/Pflash_Configuration/PflashDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!ENDIF!]
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
                                       GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/


#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @} */

#endif /* PFLASH_IP_CFG_H */

