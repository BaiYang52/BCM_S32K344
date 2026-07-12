/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : LPUART_FLEXIO
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

#ifndef LIN_CFG_H
#define LIN_CFG_H

/**
*   @file
*
*   @defgroup lin_driver LIN Driver
*   @addtogroup  lin_driver LIN Driver
*   @{
*/

#ifdef __cplusplus
extern "C"
{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Mcal.h"
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
#include "Lin_[!"."!]_PBcfg.h"
[!ENDLOOP!][!//
[!ELSE!][!//
#include "Lin_PBcfg.h"
[!ENDIF!][!//

[!NOCODE!][!//
[!INCLUDE "Lin_VersionCheck_Inc.m"!][!//
[!ENDNOCODE!][!//
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define LIN_CFG_VENDOR_ID                    43
#define LIN_CFG_AR_RELEASE_MAJOR_VERSION     4
#define LIN_CFG_AR_RELEASE_MINOR_VERSION     4
#define LIN_CFG_AR_RELEASE_REVISION_VERSION  0
#define LIN_CFG_SW_MAJOR_VERSION             2
#define LIN_CFG_SW_MINOR_VERSION             0
#define LIN_CFG_SW_PATCH_VERSION             1
/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!][!//
/* Checks against Lin_[!"."!]_PBcfg.h */
#if (LIN_CFG_VENDOR_ID != LIN_[!"."!]_PBCFG_VENDOR_ID)
    #error "Lin_Cfg.h and Lin_[!"."!]_PBcfg.h have different vendor ids"
#endif
#if ((LIN_CFG_AR_RELEASE_MAJOR_VERSION    != LIN_[!"."!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (LIN_CFG_AR_RELEASE_MINOR_VERSION    != LIN_[!"."!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (LIN_CFG_AR_RELEASE_REVISION_VERSION != LIN_[!"."!]_PBCFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Lin_Cfg.h and Lin_[!"."!]_PBcfg.h are different"
#endif
#if ((LIN_CFG_SW_MAJOR_VERSION != LIN_[!"."!]_PBCFG_SW_MAJOR_VERSION) || \
     (LIN_CFG_SW_MINOR_VERSION != LIN_[!"."!]_PBCFG_SW_MINOR_VERSION) || \
     (LIN_CFG_SW_PATCH_VERSION != LIN_[!"."!]_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Lin_Cfg.h and Lin_[!"."!]_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!][!//
/* Checks against Lin_PBcfg.h */
#if (LIN_CFG_VENDOR_ID != LIN_PBCFG_VENDOR_ID)
    #error "Lin_Cfg.h and Lin_PBcfg.h have different vendor ids"
#endif
#if ((LIN_CFG_AR_RELEASE_MAJOR_VERSION    != LIN_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (LIN_CFG_AR_RELEASE_MINOR_VERSION    != LIN_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (LIN_CFG_AR_RELEASE_REVISION_VERSION != LIN_PBCFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Lin_Cfg.h and Lin_PBcfg.h are different"
#endif
#if ((LIN_CFG_SW_MAJOR_VERSION != LIN_PBCFG_SW_MAJOR_VERSION) || \
     (LIN_CFG_SW_MINOR_VERSION != LIN_PBCFG_SW_MINOR_VERSION) || \
     (LIN_CFG_SW_PATCH_VERSION != LIN_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Lin_Cfg.h and Lin_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Mcal.h */
    #if ((LIN_CFG_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (LIN_CFG_AR_RELEASE_MINOR_VERSION != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Lin_Cfg.h and Mcal.h are different"
    #endif
#endif
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
[!IF "not(IMPLEMENTATION_CONFIG_VARIANT !='VariantPostBuild' and (variant:size()<=1))"!][!//
#define LIN_CONFIG_EXT \
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
    LIN_CONFIG_[!"."!]_PB \
[!ENDLOOP!][!//
[!ELSE!][!//
    LIN_CONFIG_PB
[!ENDIF!][!//
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
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                     EXTERNAL CONSTANTS
==================================================================================================*/
#define LIN_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Lin_MemMap.h"

#if (LIN_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF)
extern const Mcal_DemErrorType Lin_E_TimeoutCfg;
#endif /* LIN_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF */

#define LIN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Lin_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* LIN_CFG_H */
