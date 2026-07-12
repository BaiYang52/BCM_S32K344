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

#ifndef OCU_CFG_H
#define OCU_CFG_H

/**
*   @file         Ocu_Cfg.h
*
*   @addtogroup   ocu_driver Ocu Driver
*   @brief        Ocu HLD Precompile configuration header file
*   @details      Precompile parameters and extern configuration.
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
#include "Ocu_[!"."!]_PBcfg.h"
[!ENDLOOP!]
[!ELSE!]
#include "Ocu_PBcfg.h"
[!ENDIF!]

/*==================================================================================================
*                               HEADER FILE VERSION INFORMATION
==================================================================================================*/
#define OCU_CFG_VENDOR_ID                       43
#define OCU_CFG_AR_RELEASE_MAJOR_VERSION        4
#define OCU_CFG_AR_RELEASE_MINOR_VERSION        4
#define OCU_CFG_AR_RELEASE_REVISION_VERSION     0
#define OCU_CFG_SW_MAJOR_VERSION                2
#define OCU_CFG_SW_MINOR_VERSION                0
#define OCU_CFG_SW_PATCH_VERSION                1
/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/

[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
#if (OCU_CFG_VENDOR_ID != OCU_[!"."!]_PBCFG_VENDOR_ID)
    #error "Ocu_Cfg.h and Ocu_[!"."!]_PBcfg.h have different vendor ids"
#endif

/* Check if  header file and Ocu_[!"."!]_PBcfg.h file are of the same Autosar version */
#if ((OCU_CFG_AR_RELEASE_MAJOR_VERSION != OCU_[!"."!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (OCU_CFG_AR_RELEASE_MINOR_VERSION != OCU_[!"."!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (OCU_CFG_AR_RELEASE_REVISION_VERSION != OCU_[!"."!]_PBCFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Ocu_Cfg.h and Ocu_[!"."!]_PBcfg.h are different"
#endif

/* Check if header file and Ocu_[!"."!]_PBcfg.h file are of the same Software version */
#if ((OCU_CFG_SW_MAJOR_VERSION != OCU_[!"."!]_PBCFG_SW_MAJOR_VERSION) || \
     (OCU_CFG_SW_MINOR_VERSION != OCU_[!"."!]_PBCFG_SW_MINOR_VERSION) || \
     (OCU_CFG_SW_PATCH_VERSION != OCU_[!"."!]_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Ocu_Cfg.h and Ocu_[!"."!]_PBcfg.h are different"
#endif

[!ENDLOOP!][!//
[!ELSE!][!//
#if (OCU_CFG_VENDOR_ID != OCU_PBCFG_VENDOR_ID)
    #error "Ocu_Cfg.h and Ocu_PBcfg.h have different vendor ids"
#endif

/* Check if  header file and Ocu_PBcfg.h file are of the same Autosar version */
#if ((OCU_CFG_AR_RELEASE_MAJOR_VERSION != OCU_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (OCU_CFG_AR_RELEASE_MINOR_VERSION != OCU_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (OCU_CFG_AR_RELEASE_REVISION_VERSION != OCU_PBCFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Ocu_Cfg.h and Ocu_PBcfg.h are different"
#endif

/* Check if header file and Ocu_PBcfg.h file are of the same Software version */
#if ((OCU_CFG_SW_MAJOR_VERSION != OCU_PBCFG_SW_MAJOR_VERSION) || \
     (OCU_CFG_SW_MINOR_VERSION != OCU_PBCFG_SW_MINOR_VERSION) || \
     (OCU_CFG_SW_PATCH_VERSION != OCU_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Ocu_Cfg.h and Ocu_PBcfg.h are different"
#endif
[!ENDIF!][!//

/*==================================================================================================
                                           CONSTANTS
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
/**
* @brief        Symbolic Names for configured channels
*
*/
[!LOOP "OcuConfigSet/OcuChannel/*"!]
#define [!"node:name(.)"!]                      ((Ocu_ChannelType)[!"OcuChannelId"!]U)
[!ENDLOOP!]

/**
* @brief        Symbolic Names for configured channels - ecuc 2108 compliant.
*
*/
[!LOOP "OcuConfigSet/OcuChannel/*"!]
#define  OcuConf_OcuConfigSet_[!"node:name(.)"!]            ((Ocu_ChannelType)[!"OcuChannelId"!]U)
[!ENDLOOP!]

/**
* @brief        Hardware resource configured for Ocu channels
*
*/
[!NOCODE!]
[!LOOP "OcuGeneral/OcuHwResourceConfig/*"!]
    [!IF "(OcuChannelIsUsed  = 'true')"!]
        [!CODE!]
#define OCU_[!"OcuHwResourceId"!]_USED
        [!ENDCODE!]
    [!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!]


[!NOCODE!][!//
[!IF "not((IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1))"!][!//
    [!CODE!][!CR!][!//
#define OCU_CONFIG_EXT \
    [!ENDCODE!][!//
    [!IF "var:defined('postBuildVariant')"!][!//
        [!VAR "variantIndex"="0"!][!//
        [!VAR "variantNumber"="variant:size()"!][!//
        [!LOOP "variant:all()"!]
            [!VAR "variantIndex"="$variantIndex + 1"!][!//
            [!CODE!][!WS "4"!]OCU_CONFIG_[!"."!]_PB [!IF "$variantIndex < $variantNumber"!]\[!ENDIF!][!CR!][!ENDCODE!]
        [!ENDLOOP!][!//
    [!ELSE!][!//
        [!CODE!][!WS "4"!]OCU_CONFIG_PB[!CR!][!ENDCODE!]
    [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!][!//

/*==================================================================================================
 *                                    FUNCTION PROTOTYPES
 ==================================================================================================*/


#ifdef __cplusplus
}
#endif

/** @} */

#endif    /* OCU_CFG_H */

[!ENDCODE!]
