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

#ifndef I2S_IPW_CFG_H
#define I2S_IPW_CFG_H

/**
*   @file
*
*   @addtogroup i2s_ipw_config I2s IPW Configuration
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
[!NOCODE!][!//
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!]#include "I2s_Ipw_[!"."!]_PBcfg.h"[!CR!][!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!]#include "I2s_Ipw_PBcfg.h"[!CR!][!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!][!//

[!NOCODE!][!//
[!//INCLUDE "Adc_VersionCheck_Src.m"!][!// TODO: add version checking
[!INCLUDE "I2s_RegOperations.m"!][!//
[!ENDNOCODE!][!//

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define I2S_IPW_CFG_VENDOR_ID                       43
#define I2S_IPW_CFG_AR_RELEASE_MAJOR_VERSION        4
#define I2S_IPW_CFG_AR_RELEASE_MINOR_VERSION        4
#define I2S_IPW_CFG_AR_RELEASE_REVISION_VERSION     0
#define I2S_IPW_CFG_SW_MAJOR_VERSION                2
#define I2S_IPW_CFG_SW_MINOR_VERSION                0
#define I2S_IPW_CFG_SW_PATCH_VERSION                1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
/* Check if current file and I2s_Ipw_[!"."!]_PBcfg.h file are of the same vendor */
#if (I2S_IPW_CFG_VENDOR_ID != I2S_IPW_[!"text:toupper(.)"!]_PBCFG_VENDOR_ID)
#error "I2s_Ipw_Cfg.h and I2s_Ipw_[!"."!]_PBcfg.h have different vendor IDs"
#endif

/* Check if current file and I2s_Ipw_[!"."!]_PBcfg.h file are of the same Autosar version */
#if ((I2S_IPW_CFG_AR_RELEASE_MAJOR_VERSION     != I2S_IPW_[!"text:toupper(.)"!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (I2S_IPW_CFG_AR_RELEASE_MINOR_VERSION     != I2S_IPW_[!"text:toupper(.)"!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (I2S_IPW_CFG_AR_RELEASE_REVISION_VERSION  != I2S_IPW_[!"text:toupper(.)"!]_PBCFG_AR_RELEASE_REVISION_VERSION))
#error "AutoSar Version Numbers of I2s_Ipw_Cfg.h and I2s_Ipw_[!"."!]_PBcfg.h are different"
#endif

/* Check if current file and I2s_Ipw_[!"."!]_PBcfg.h file are of the same software version */
#if ((I2S_IPW_CFG_SW_MAJOR_VERSION != I2S_IPW_[!"text:toupper(.)"!]_PBCFG_SW_MAJOR_VERSION) || \
     (I2S_IPW_CFG_SW_MINOR_VERSION != I2S_IPW_[!"text:toupper(.)"!]_PBCFG_SW_MINOR_VERSION) || \
     (I2S_IPW_CFG_SW_PATCH_VERSION != I2S_IPW_[!"text:toupper(.)"!]_PBCFG_SW_PATCH_VERSION))
#error "Software Version Numbers of I2s_Ipw_Cfg.h and I2s_Ipw_[!"."!]_PBcfg.h are different"
#endif
[!ENDLOOP!][!//
[!ELSE!][!//
/* Check if current file and I2s_Ipw_PBcfg.h file are of the same vendor */
#if (I2S_IPW_CFG_VENDOR_ID != I2S_IPW_PBCFG_VENDOR_ID)
    #error "I2s_Ipw_Cfg.h and I2s_Ipw_PBcfg.h have different vendor IDs"
#endif

/* Check if current file and I2s_Ipw_PBcfg.h file are of the same Autosar version */
#if ((I2S_IPW_CFG_AR_RELEASE_MAJOR_VERSION != I2S_IPW_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (I2S_IPW_CFG_AR_RELEASE_MINOR_VERSION != I2S_IPW_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (I2S_IPW_CFG_AR_RELEASE_REVISION_VERSION != I2S_IPW_PBCFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of I2s_Ipw_Cfg.h and I2s_Ipw_PBcfg.h are different"
#endif

/* Check if current file and I2s_Ipw_PBcfg.h file are of the same software version */
#if ((I2S_IPW_CFG_SW_MAJOR_VERSION != I2S_IPW_PBCFG_SW_MAJOR_VERSION) || \
     (I2S_IPW_CFG_SW_MINOR_VERSION != I2S_IPW_PBCFG_SW_MINOR_VERSION) || \
     (I2S_IPW_CFG_SW_PATCH_VERSION != I2S_IPW_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of I2s_Ipw_Cfg.h and I2s_Ipw_PBcfg.h are different"
#endif
[!ENDIF!]

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

/*==================================================================================================
*                                  LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL FUNCTIONS
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* I2S_IPW_CFG_H */