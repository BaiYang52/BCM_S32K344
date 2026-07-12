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

#ifndef CDD_I3C_CFG_H
#define CDD_I3C_CFG_H

/**
*   @file
*
*   @addtogroup i3c_driver_config I3c Driver Configuration
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
#include "StandardTypes.h"
[!NOCODE!][!//
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!]#include "CDD_I3c_[!"."!]_PBcfg.h"[!CR!][!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!]#include "CDD_I3c_PBcfg.h"[!CR!][!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!][!//

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define I3C_VENDOR_ID_CFG                       43
#define I3C_AR_RELEASE_MAJOR_VERSION_CFG        4
#define I3C_AR_RELEASE_MINOR_VERSION_CFG        4
#define I3C_AR_RELEASE_REVISION_VERSION_CFG     0
#define I3C_SW_MAJOR_VERSION_CFG                2
#define I3C_SW_MINOR_VERSION_CFG                0
#define I3C_SW_PATCH_VERSION_CFG                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if current file and StandardTypes header file are of the same Autosar version */
#if ((I3C_AR_RELEASE_MAJOR_VERSION_CFG     != STD_AR_RELEASE_MAJOR_VERSION) || \
     (I3C_AR_RELEASE_MINOR_VERSION_CFG     != STD_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of CDD_I3c_Cfg.h and StandardTypes.h are different"
#endif
#endif /* DISABLE_MCAL_INTERMODULE_ASR_CHECK */

[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
/* Check if current file and CDD_I3c_[!"."!]_PBcfg header file are of the same vendor */
#if (I3C_VENDOR_ID_CFG != I3C_VENDOR_ID_[!"text:toupper(.)"!]_PBCFG)
    #error "CDD_I3c_Cfg.h and CDD_I3c_[!"."!]_PBcfg.h have different vendor IDs"
#endif

/* Check if current file and CDD_I3c_[!"."!]_PBcfg header file are of the same Autosar version */
#if ((I3C_AR_RELEASE_MAJOR_VERSION_CFG     != I3C_AR_RELEASE_MAJOR_VERSION_[!"text:toupper(.)"!]_PBCFG) || \
     (I3C_AR_RELEASE_MINOR_VERSION_CFG     != I3C_AR_RELEASE_MINOR_VERSION_[!"text:toupper(.)"!]_PBCFG) || \
     (I3C_AR_RELEASE_REVISION_VERSION_CFG  != I3C_AR_RELEASE_REVISION_VERSION_[!"text:toupper(.)"!]_PBCFG))
    #error "AutoSar Version Numbers of CDD_I3c_Cfg.h and CDD_I3c_[!"."!]_PBcfg.h are different"
#endif

/* Check if current file and CDD_I3c_[!"."!]_PBcfg header file are of the same software version */
#if ((I3C_SW_MAJOR_VERSION_CFG != I3C_SW_MAJOR_VERSION_[!"text:toupper(.)"!]_PBCFG) || \
     (I3C_SW_MINOR_VERSION_CFG != I3C_SW_MINOR_VERSION_[!"text:toupper(.)"!]_PBCFG) || \
     (I3C_SW_PATCH_VERSION_CFG != I3C_SW_PATCH_VERSION_[!"text:toupper(.)"!]_PBCFG))
    #error "Software Version Numbers of CDD_I3c_Cfg.h and CDD_I3c_[!"."!]_PBcfg.h are different"
#endif
[!ENDLOOP!][!//
[!ELSE!][!//
/* Check if current file and CDD_I3c_PBcfg header file are of the same vendor */
#if (I3C_VENDOR_ID_CFG != I3C_VENDOR_ID_PBCFG)
    #error "CDD_I3c_Cfg.h and CDD_I3c_PBcfg.h have different vendor IDs"
#endif

/* Check if current file and CDD_I3c_PBcfg header file are of the same Autosar version */
#if ((I3C_AR_RELEASE_MAJOR_VERSION_CFG    != I3C_AR_RELEASE_MAJOR_VERSION_PBCFG) || \
     (I3C_AR_RELEASE_MINOR_VERSION_CFG    != I3C_AR_RELEASE_MINOR_VERSION_PBCFG) || \
     (I3C_AR_RELEASE_REVISION_VERSION_CFG != I3C_AR_RELEASE_REVISION_VERSION_PBCFG))
    #error "AutoSar Version Numbers of CDD_I3c_Cfg.h and CDD_I3c_PBcfg.h are different"
#endif

/* Check if current file and CDD_I3c_PBcfg header file are of the same software version */
#if ((I3C_SW_MAJOR_VERSION_CFG != I3C_SW_MAJOR_VERSION_PBCFG) || \
     (I3C_SW_MINOR_VERSION_CFG != I3C_SW_MINOR_VERSION_PBCFG) || \
     (I3C_SW_PATCH_VERSION_CFG != I3C_SW_PATCH_VERSION_PBCFG))
    #error "Software Version Numbers of CDD_I3c_Cfg.h and CDD_I3c_PBcfg.h are different"
#endif
[!ENDIF!][!//

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
[!CODE!]
#define I3C_MAX_CORE_ID     (1U)
#define I3C_NULL_PTR        {NULL_PTR}

#if (STD_ON == I3C_MULTICORE_SUPPORT)
    #define I3c_GetCoreID()            OsIf_GetCoreID()
#else
    #define I3c_GetCoreID()            ((uint32)0UL)
#endif /* (STD_ON == I3C_MULTICORE_SUPPORT) */
[!ENDCODE!][!//

[!IF "not((IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1))"!][!//
#define I3C_CONFIG_EXT \
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
I3C_CONFIG_[!"."!]_PB \
[!ENDLOOP!][!//
[!ELSE!][!//
I3C_CONFIG_PB
[!ENDIF!][!// Support for Post Build only
[!ENDIF!][!//
/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CDD_I3C_CFG_H */
