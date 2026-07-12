[!CODE!][!//
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

#ifndef SAI_IP_CFG_H
#define SAI_IP_CFG_H

/**
*   @file
*
*   @addtogroup sai_ip_config Sai IPL Configuration
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
#include "StandardTypes.h"
[!NOCODE!][!//
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!]#include "Sai_Ip_[!"."!]_PBcfg.h"[!CR!][!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!]#include "Sai_Ip_PBcfg.h"[!CR!][!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!][!//

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define SAI_IP_CFG_VENDOR_ID                       43
#define SAI_IP_CFG_AR_RELEASE_MAJOR_VERSION        4
#define SAI_IP_CFG_AR_RELEASE_MINOR_VERSION        4
#define SAI_IP_CFG_AR_RELEASE_REVISION_VERSION     0
#define SAI_IP_CFG_SW_MAJOR_VERSION                2
#define SAI_IP_CFG_SW_MINOR_VERSION                0
#define SAI_IP_CFG_SW_PATCH_VERSION                1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if current file and StandardTypes header file are of the same Autosar version */
#if ((SAI_IP_CFG_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
     (SAI_IP_CFG_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
#error "AutoSar Version Numbers of Sai_Ip_Cfg.h and StandardTypes.h are different"
#endif
#endif /* DISABLE_MCAL_INTERMODULE_ASR_CHECK */

[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
/* Check if current file and Sai_Ip_[!"."!]_PBcfg.h file are of the same vendor */
#if (SAI_IP_CFG_VENDOR_ID != SAI_IP_[!"text:toupper(.)"!]_PBCFG_VENDOR_ID)
#error "Sai_Ip_Cfg.h and Sai_Ip_[!"."!]_PBcfg.h have different vendor ids"
#endif

/* Check if current file and Sai_Ip_[!"."!]_PBcfg.h file are of the same Autosar version */
#if ((SAI_IP_CFG_AR_RELEASE_MAJOR_VERSION     != SAI_IP_[!"text:toupper(.)"!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (SAI_IP_CFG_AR_RELEASE_MINOR_VERSION     != SAI_IP_[!"text:toupper(.)"!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (SAI_IP_CFG_AR_RELEASE_REVISION_VERSION  != SAI_IP_[!"text:toupper(.)"!]_PBCFG_AR_RELEASE_REVISION_VERSION))
#error "AutoSar Version Numbers of Sai_Ip_Cfg.h and Sai_Ip_[!"."!]_PBcfg.h are different"
#endif

/* Check if current file and Sai_Ip_[!"."!]_PBcfg.h file are of the same software version */
#if ((SAI_IP_CFG_SW_MAJOR_VERSION != SAI_IP_[!"text:toupper(.)"!]_PBCFG_SW_MAJOR_VERSION) || \
     (SAI_IP_CFG_SW_MINOR_VERSION != SAI_IP_[!"text:toupper(.)"!]_PBCFG_SW_MINOR_VERSION) || \
     (SAI_IP_CFG_SW_PATCH_VERSION != SAI_IP_[!"text:toupper(.)"!]_PBCFG_SW_PATCH_VERSION))
#error "Software Version Numbers of Sai_Ip_Cfg.h and Sai_Ip_[!"."!]_PBcfg.h are different"
#endif
[!ENDLOOP!][!//
[!ELSE!][!//
/* Check if current file and Sai_Ip_PBcfg.h file are of the same vendor */
#if (SAI_IP_CFG_VENDOR_ID != SAI_IP_PBCFG_VENDOR_ID)
    #error "Sai_Ip_Cfg.h and Sai_Ip_PBcfg.h have different vendor IDs"
#endif

/* Check if current file and Sai_Ip_PBcfg.h file are of the same Autosar version */
#if ((SAI_IP_CFG_AR_RELEASE_MAJOR_VERSION != SAI_IP_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (SAI_IP_CFG_AR_RELEASE_MINOR_VERSION != SAI_IP_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (SAI_IP_CFG_AR_RELEASE_REVISION_VERSION != SAI_IP_PBCFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Sai_Ip_Cfg.h and Sai_Ip_PBcfg.h are different"
#endif

/* Check if current file and Sai_Ip_PBcfg.h file are of the same software version */
#if ((SAI_IP_CFG_SW_MAJOR_VERSION != SAI_IP_PBCFG_SW_MAJOR_VERSION) || \
     (SAI_IP_CFG_SW_MINOR_VERSION != SAI_IP_PBCFG_SW_MINOR_VERSION) || \
     (SAI_IP_CFG_SW_PATCH_VERSION != SAI_IP_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Sai_Ip_Cfg.h and Sai_Ip_PBcfg.h are different"
#endif
[!ENDIF!]

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

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
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* SAI_IP_CFG_H */
[!ENDCODE!][!//