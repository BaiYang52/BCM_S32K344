[!AUTOSPACING!]
[!CODE!]
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : eMios
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

#ifndef OCU_IPW_CFG_H
#define OCU_IPW_CFG_H

/**
*   @file          Ocu_Ipw_Cfg.h
*
*   @internal
*   @addtogroup    Ipw Ocu Driver
*   @brief         Ocu IPW driver precompile configuration header file.
*   @details
*
*   @{
*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!]
#include "Ocu_Ipw_[!"."!]_PBcfg.h"
[!ENDLOOP!]
[!ELSE!]
#include "Ocu_Ipw_PBcfg.h"
[!ENDIF!]


/*==================================================================================================
*                               HEADER FILE VERSION INFORMATION
==================================================================================================*/
#define OCU_IPW_CFG_VENDOR_ID                       43
#define OCU_IPW_CFG_AR_RELEASE_MAJOR_VERSION        4
#define OCU_IPW_CFG_AR_RELEASE_MINOR_VERSION        4
#define OCU_IPW_CFG_AR_RELEASE_REVISION_VERSION     0
#define OCU_IPW_CFG_SW_MAJOR_VERSION                2
#define OCU_IPW_CFG_SW_MINOR_VERSION                0
#define OCU_IPW_CFG_SW_PATCH_VERSION                1
/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!]
#if (OCU_IPW_CFG_VENDOR_ID != OCU_IPW_[!"."!]_PBCFG_VENDOR_ID)
    #error "Ocu_Ipw_Cfg.h and Ocu_Ipw_[!"."!]_PBcfg.h have different vendor ids"
#endif

/* Check if  header file and Ocu_Ipw_[!"."!]_PBcfg.h file are of the same Autosar version */
#if ((OCU_IPW_CFG_AR_RELEASE_MAJOR_VERSION != OCU_IPW_[!"."!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (OCU_IPW_CFG_AR_RELEASE_MINOR_VERSION != OCU_IPW_[!"."!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (OCU_IPW_CFG_AR_RELEASE_REVISION_VERSION != OCU_IPW_[!"."!]_PBCFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Ocu_Ipw_Cfg.h and Ocu_Ipw_[!"."!]_PBcfg.h are different"
#endif

/* Check if header file and Ocu_Ipw_[!"."!]_PBcfg.h file are of the same Software version */
#if ((OCU_IPW_CFG_SW_MAJOR_VERSION != OCU_IPW_[!"."!]_PBCFG_SW_MAJOR_VERSION) || \
     (OCU_IPW_CFG_SW_MINOR_VERSION != OCU_IPW_[!"."!]_PBCFG_SW_MINOR_VERSION) || \
     (OCU_IPW_CFG_SW_PATCH_VERSION != OCU_IPW_[!"."!]_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Ocu_Ipw_Cfg.h and Ocu_Ipw_[!"."!]_PBcfg.h are different"
#endif
[!ENDLOOP!]
[!ELSE!]
#if (OCU_IPW_CFG_VENDOR_ID != OCU_IPW_PBCFG_VENDOR_ID)
    #error "Ocu_Ipw_Cfg.h and Ocu_Ipw_PBcfg.h have different vendor ids"
#endif

/* Check if  header file and Ocu_Ipw_PBcfg.h file are of the same Autosar version */
#if ((OCU_IPW_CFG_AR_RELEASE_MAJOR_VERSION != OCU_IPW_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (OCU_IPW_CFG_AR_RELEASE_MINOR_VERSION != OCU_IPW_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (OCU_IPW_CFG_AR_RELEASE_REVISION_VERSION != OCU_IPW_PBCFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Ocu_Ipw_Cfg.h and Ocu_Ipw_PBcfg.h are different"
#endif

/* Check if header file and Ocu_Ipw_PBcfg.h file are of the same Software version */
#if ((OCU_IPW_CFG_SW_MAJOR_VERSION != OCU_IPW_PBCFG_SW_MAJOR_VERSION) || \
     (OCU_IPW_CFG_SW_MINOR_VERSION != OCU_IPW_PBCFG_SW_MINOR_VERSION) || \
     (OCU_IPW_CFG_SW_PATCH_VERSION != OCU_IPW_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Ocu_Ipw_Cfg.h and Ocu_Ipw_PBcfg.h are different"
#endif
[!ENDIF!]

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
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

#endif  /* OCU_IPW_CFG_H */

[!ENDCODE!]
