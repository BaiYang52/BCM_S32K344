[!AUTOSPACING!][!//
[!CODE!][!//
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
#ifndef CDD_RM_CFG_H
#define CDD_RM_CFG_H

/**
*   @file CDD_Rm_Cfg.h
*
*   @addtogroup RM_MODULE RM Driver 
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
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!]
#include "CDD_Rm_[!"."!]_PBcfg.h"
[!ENDLOOP!]
[!ELSE!][!//
#include "CDD_Rm_PBcfg.h"
[!ENDIF!]
#include "CDD_Rm_Ipw_Cfg.h"
/*==================================================================================================
                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_CFG_VENDOR_ID                       43
#define RM_CFG_AR_RELEASE_MAJOR_VERSION        4
#define RM_CFG_AR_RELEASE_MINOR_VERSION        4
#define RM_CFG_AR_RELEASE_REVISION_VERSION     0
#define RM_CFG_SW_MAJOR_VERSION                2
#define RM_CFG_SW_MINOR_VERSION                0
#define RM_CFG_SW_PATCH_VERSION                1
/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!][!//
/* Checks against CDD_Rm_[!"."!]_PBcfg.h */
#if (RM_CFG_VENDOR_ID != RM_PBCFG_[!"."!]_VENDOR_ID)
    #error "CDD_Rm_Cfg.h and CDD_Rm_[!"."!]_PBcfg.h have different vendor ids"
#endif
#if ((RM_CFG_AR_RELEASE_MAJOR_VERSION    != RM_PBCFG_[!"."!]_AR_RELEASE_MAJOR_VERSION) || \
     (RM_CFG_AR_RELEASE_MINOR_VERSION    != RM_PBCFG_[!"."!]_AR_RELEASE_MINOR_VERSION) || \
     (RM_CFG_AR_RELEASE_REVISION_VERSION != RM_PBCFG_[!"."!]_AR_RELEASE_REVISION_VERSION) \
    )
     #error "AUTOSAR Version Numbers of CDD_Rm_Cfg.h and CDD_Rm_[!"."!]_PBcfg.h are different"
#endif
#if ((RM_CFG_SW_MAJOR_VERSION != RM_PBCFG_[!"."!]_SW_MAJOR_VERSION) || \
     (RM_CFG_SW_MINOR_VERSION != RM_PBCFG_[!"."!]_SW_MINOR_VERSION) || \
     (RM_CFG_SW_PATCH_VERSION != RM_PBCFG_[!"."!]_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Cfg.h and CDD_Rm_[!"."!]_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!][!//
/* Checks against CDD_Rm_PBcfg.h */
#if (RM_CFG_VENDOR_ID != RM_PBCFG_VENDOR_ID)
    #error "CDD_Rm_Cfg.h and CDD_Rm_PBcfg.h have different vendor ids"
#endif
#if ((RM_CFG_AR_RELEASE_MAJOR_VERSION    != RM_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (RM_CFG_AR_RELEASE_MINOR_VERSION    != RM_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (RM_CFG_AR_RELEASE_REVISION_VERSION != RM_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
     #error "AUTOSAR Version Numbers of CDD_Rm_Cfg.h and CDD_Rm_PBcfg.h are different"
#endif
#if ((RM_CFG_SW_MAJOR_VERSION != RM_PBCFG_SW_MAJOR_VERSION) || \
     (RM_CFG_SW_MINOR_VERSION != RM_PBCFG_SW_MINOR_VERSION) || \
     (RM_CFG_SW_PATCH_VERSION != RM_PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Cfg.h and CDD_Rm_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Mcal.h */
    #if ((RM_CFG_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (RM_CFG_AR_RELEASE_MINOR_VERSION != MCAL_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AUTOSAR Version Numbers of CDD_Rm_Cfg.h and Mcal.h are different"
    #endif
#endif

/* Checks against CDD_Rm_Ipw_Cfg.h */
#if (RM_CFG_VENDOR_ID != RM_IPW_CFG_VENDOR_ID)
    #error "CDD_Rm_Cfg.h and CDD_Rm_Ipw_Cfg.h have different vendor ids"
#endif
#if ((RM_CFG_AR_RELEASE_MAJOR_VERSION    != RM_IPW_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (RM_CFG_AR_RELEASE_MINOR_VERSION    != RM_IPW_CFG_AR_RELEASE_MINOR_VERSION) || \
     (RM_CFG_AR_RELEASE_REVISION_VERSION != RM_IPW_CFG_AR_RELEASE_REVISION_VERSION) \
    )
     #error "AUTOSAR Version Numbers of CDD_Rm_Cfg.h and CDD_Rm_Ipw_Cfg.h are different"
#endif
#if ((RM_CFG_SW_MAJOR_VERSION != RM_IPW_CFG_SW_MAJOR_VERSION) || \
     (RM_CFG_SW_MINOR_VERSION != RM_IPW_CFG_SW_MINOR_VERSION) || \
     (RM_CFG_SW_PATCH_VERSION != RM_IPW_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Cfg.h and CDD_Rm_Ipw_Cfg.h are different"
#endif

/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/


/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/
/**
* @brief   Collection of all configuration structures declarations.
*/
#define RM_CONFIG_EXT \
[!IF "var:defined('postBuildVariant')"!][!//
    [!VAR "variantIndex"="0"!][!//
    [!VAR "variantNumber"="variant:size()"!][!//
[!LOOP "variant:all()"!][!//
    [!VAR "variantIndex"="$variantIndex + 1"!][!//
    RM_CONFIG_[!"."!]_PB[!IF "$variantIndex < $variantNumber"!] \[!CR!][!ENDIF!]
[!ENDLOOP!][!//
[!ELSE!][!//
    RM_CONFIG_PB
[!ENDIF!]


/* Rm Pre Compile Switch */
#define RM_PRECOMPILE_SUPPORT                   ([!IF "(IMPLEMENTATION_CONFIG_VARIANT ='VariantPreCompile') and (variant:size()<=1)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
*   @brief  Switches the Development Error Detection and Notification on or off.
*        STD_ON: Enabled. STD_OFF: Disabled.
*
*/
#define RM_DEV_ERROR_DETECT                     ([!IF "RmGeneral/RmDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
* @{
*   @brief Configuration of Optional API's
*/
/**
*   @brief  Adds or removes the service Rm_GetVersionInfo() from the code.
*        STD_ON: Rm_GetVersionInfo() can be used. STD_OFF: Rm_GetVersionInfo() can not be used.
*
*/
#define RM_GET_VERSION_INFO_API                 ([!IF "RmGeneral/Rm_VersionInfoApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
* @brief            Support for User mode.
*                   STD_ON: the Rm driver can be executed from both supervisor and user mode.
*                   STD_OFF: the Rm driver can be executed only from supervisor mode.
*/
#define RM_ENABLE_USER_MODE_SUPPORT             ([!IF "RmGeneral/RmEnableUserModeSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    #if (STD_ON == RM_ENABLE_USER_MODE_SUPPORT)
        #error "MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running RM in user mode the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined"
    #endif /* (STD_ON == RM_ENABLE_USER_MODE_SUPPORT) */
#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT */

#if (RM_ENABLE_USER_MODE_SUPPORT == STD_ON)
/**
* @brief            Protected Register in User mode
*                   Macro use to enable if IP hw need to configure Reg_Prot to access from user mode
*/
#define RM_SEMA4_REG_PROT_AVAILABLE           ([!IF "node:containsValue(ecu:list('Rm.Ipv.RegProt.Available.List'),'SEMA4')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
#endif


[!VAR "rmFeatureEnabled"="0"!]
/*********************************** XRDC SECTION *********************************/
#define RM_ENABLE_XRDC                                ([!IF "RmGeneral/RmEnableXRDCSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!IF "RmGeneral/RmEnableXRDCSupport"!][!VAR "rmFeatureEnabled"="1"!][!ENDIF!]
/*********************************** SEMA42 SECTION *******************************/
#define RM_ENABLE_SEMA42                              ([!IF "RmGeneral/RmEnableSema42Support"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!IF "RmGeneral/RmEnableSema42Support"!][!VAR "rmFeatureEnabled"="1"!][!ENDIF!]
/*********************************** MPU_M7 SECTION *******************************/
#define RM_ENABLE_MPU_M7                              ([!IF "RmGeneral/RmMpuM7Configurable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!IF "RmGeneral/RmMpuM7Configurable"!][!VAR "rmFeatureEnabled"="1"!][!ENDIF!]
/*********************************** PFLASH SECTION *******************************/
#define RM_ENABLE_PFLASH                              ([!IF "RmGeneral/RmPflashConfigurable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!IF "RmGeneral/RmPflashConfigurable"!][!VAR "rmFeatureEnabled"="1"!][!ENDIF!]
/*********************************** AXBS SECTION *********************************/
#define RM_ENABLE_AXBS                                ([!IF "RmGeneral/RmCrossbarConfigurable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!IF "RmGeneral/RmCrossbarConfigurable"!][!VAR "rmFeatureEnabled"="1"!][!ENDIF!]
/*********************************** XBIC SECTION *********************************/
#define RM_ENABLE_XBIC                                ([!IF "RmGeneral/RmXbicConfigurable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!IF "RmGeneral/RmXbicConfigurable"!][!VAR "rmFeatureEnabled"="1"!][!ENDIF!]
/*********************************** VIRT_WRAPPER SECTION *************************/
#define RM_ENABLE_VIRT_WRAPPER                        ([!IF "RmGeneral/RmVirtWrapperConfigurable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!IF "RmGeneral/RmVirtWrapperConfigurable"!][!VAR "rmFeatureEnabled"="1"!][!ENDIF!]
/*********************************** MULTICORE SECTION ****************************/
#define RM_MPU_M7_MULTICORE_SUPPORT                   ([!IF "RmGeneral/RmEnableMultiCore"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

[!IF "$rmFeatureEnabled=0"!][!//
   [!ERROR!] [!//
         "At least one RM feature must be enabled(XRDC, SEMA42, MPU, VIRT_WRAPPER, XBIC, XBAR, PFLASH or CAN_HUB). "[!//
   [!ENDERROR!][!//
[!ENDIF!][!//
/*==================================================================================================
                                             ENUMS
==================================================================================================*/


/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
* @brief This type is used to group all multiple configurations.
*/
typedef struct
{
    const Rm_Ipw_ConfigType *Rm_pIpwConfig;
} Rm_ConfigType;

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif    /* CDD_RM_CFG_H */

[!ENDCODE!][!//