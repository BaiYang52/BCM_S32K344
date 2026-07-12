/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Swt
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

#ifndef WDG_43_INSTANCE2_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H
#define WDG_43_INSTANCE2_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H

/**
*   @file
*
*   @addtogroup Wdg
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
* 4) user callback header files
==================================================================================================*/
#include "Wdg_ChannelTypes.h"
#include "Wdg_43_Instance2_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h"
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define WDG_43_INSTANCE2_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG                    43
#define WDG_43_INSTANCE2_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG     4
#define WDG_43_INSTANCE2_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG     4
#define WDG_43_INSTANCE2_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG  0
#define WDG_43_INSTANCE2_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG             2
#define WDG_43_INSTANCE2_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG             0
#define WDG_43_INSTANCE2_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/* Check if current file and Wdg_ChannelTypes header file are of the same vendor */
#if (WDG_43_INSTANCE2_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG != WDG_CHANNEL_TYPES_VENDOR_ID)
#error "Wdg_43_Instance2_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h and Wdg_ChannelTypes.h have different vendor ids"
#endif

/* Check if current file and Wdg_ChannelTypes header file are of the same Autosar version */
#if ((WDG_43_INSTANCE2_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG     != WDG_CHANNEL_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (WDG_43_INSTANCE2_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG     != WDG_CHANNEL_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (WDG_43_INSTANCE2_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG  != WDG_CHANNEL_TYPES_AR_RELEASE_REVISION_VERSION))
#error "AutoSar Version Numbers of Wdg_43_Instance2_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h and Wdg_ChannelTypes.h are different"
#endif

/* Check if current file and Wdg_ChannelTypes header file are of the same software version */
#if ((WDG_43_INSTANCE2_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG != WDG_CHANNEL_TYPES_SW_MAJOR_VERSION) || \
     (WDG_43_INSTANCE2_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG != WDG_CHANNEL_TYPES_SW_MINOR_VERSION) || \
     (WDG_43_INSTANCE2_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG != WDG_CHANNEL_TYPES_SW_PATCH_VERSION))
#error "Software Version Numbers of Wdg_43_Instance2_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h and Wdg_ChannelTypes.h are different"
#endif

/* Check if current file and Wdg_43_Instance2_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg configuration header file are of the same vendor */
#if (WDG_43_INSTANCE2_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG != WDG_43_INSTANCE2_IPW_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG)
#error "Wdg_43_Instance2_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h and Wdg_43_Instance2_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h have different vendor ids"
#endif

#if ((WDG_43_INSTANCE2_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG    != WDG_43_INSTANCE2_IPW_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG) || \
     (WDG_43_INSTANCE2_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG    != WDG_43_INSTANCE2_IPW_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG) || \
     (WDG_43_INSTANCE2_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG != WDG_43_INSTANCE2_IPW_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG))
#error "AutoSar Version Numbers of Wdg_43_Instance2_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h and Wdg_43_Instance2_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h are different"
#endif

#if ((WDG_43_INSTANCE2_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG != WDG_43_INSTANCE2_IPW_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG) || \
     (WDG_43_INSTANCE2_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG != WDG_43_INSTANCE2_IPW_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG) || \
     (WDG_43_INSTANCE2_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG != WDG_43_INSTANCE2_IPW_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG))
#error "Software Version Numbers of Wdg_43_Instance2_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h and Wdg_43_Instance2_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h are different"
#endif


/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                            ENUMS
==================================================================================================*/

/*==================================================================================================
*                               STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

[!NOCODE!]
[!INDENT "0"!]
[!VAR "xPathModule" = "concat('ASPathDataOfSchema:/Wdg_43_Instance','2','_','TS_T40D34M20I1R0/Wdg')"!][!//
[!IF "node:exists($xPathModule)"!][!//
    [!IF "node:refs(concat($xPathModule,'/WdgGeneral/WdgEnableMultiCoreSupport'))"!]
        [!VAR "PartitionName" = "text:split(node:value(concat($xPathModule,'/WdgGeneral/WdgEcucPartitionRef')),'/')[5]"!]
    [!ENDIF!]

    [!IF "var:defined('postBuildVariant')"!]
        [!VAR "postBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!]
       [!CODE!]#define WDG_43_INSTANCE2_CONFIG_[!"$postBuildVariant"!]_PB \[!ENDCODE!][!CR!]
    [!ELSE!]
    [!VAR "postBuildVariantNameUnderscore" = "string(null)"!]
        [!CODE!]#define WDG_43_INSTANCE2_CONFIG_PB \[!ENDCODE!][!CR!]
    [!ENDIF!]

    [!CODE!]extern const Wdg_ConfigType Wdg_Config_2[!"$postBuildVariantNameUnderscore"!][!IF "var:defined('PartitionName')"!]_[!"$PartitionName"!][!ENDIF!];[!ENDCODE!]
[!ENDIF!]

[!ENDINDENT!]
[!ENDNOCODE!]

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* WDG_43_INSTANCE'2`_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H */
