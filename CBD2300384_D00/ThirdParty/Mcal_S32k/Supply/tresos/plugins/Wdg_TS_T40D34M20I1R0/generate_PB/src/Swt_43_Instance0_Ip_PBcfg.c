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

/**
* @file
*
* @addtogroup  Swt
* @{
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

#include "Swt_43_Instance0_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h"

 [!INCLUDE "Wdg_VersionCheck_Src_PB.m"!][!//
 [!INCLUDE "Wdg_PluginMacros.m"!][!//
/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define SWT_43_INSTANCE0_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C                     43
#define SWT_43_INSTANCE0_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C      4
#define SWT_43_INSTANCE0_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C      4
#define SWT_43_INSTANCE0_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C   0
#define SWT_43_INSTANCE0_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C              2
#define SWT_43_INSTANCE0_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C              0
#define SWT_43_INSTANCE0_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C              1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and Swt_43_Instance0_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg header file are of the same vendor */
#if (SWT_43_INSTANCE0_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C != SWT_43_INSTANCE0_IP_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG)
#error "Swt_43_Instance0_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Swt_43_Instance0_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h have different vendor ids"
#endif

/* Check if current file and Swt_43_Instance0_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg header file are of the same Autosar version */
#if ((SWT_43_INSTANCE0_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C    != SWT_43_INSTANCE0_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG   ) || \
     (SWT_43_INSTANCE0_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C    != SWT_43_INSTANCE0_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG   ) || \
     (SWT_43_INSTANCE0_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C != SWT_43_INSTANCE0_IP_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG))
#error "AutoSar Version Numbers of Swt_43_Instance0_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Swt_43_Instance0_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h are different"
#endif

/* Check if current file and Swt_43_Instance0_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg header file are of the same software version */
#if ((SWT_43_INSTANCE0_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C != SWT_43_INSTANCE0_IP_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG) || \
     (SWT_43_INSTANCE0_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C != SWT_43_INSTANCE0_IP_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG) || \
     (SWT_43_INSTANCE0_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C != SWT_43_INSTANCE0_IP_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG))
#error "Software Version Numbers of Swt_43_Instance0_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Swt_43_Instance0_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h are different"
#endif



/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/

#define WDG_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "Wdg_MemMap.h"

[!AUTOSPACING!]
[!VAR "xPathModule" = "concat('ASPathDataOfSchema:/','TS_T40D34M20I1R0/Wdg')"!][!//
[!IF "node:exists($xPathModule)"!][!//
[!IF "(normalize-space(node:value(concat($xPathModule,'/WdgGeneral/WdgCallbackNotification'))) !='NULL_PTR') and (normalize-space(node:value(concat($xPathModule,'/WdgGeneral/WdgCallbackNotification'))) !='NULL')"!]
/**
 *   @brief External Notifications for Wdg Interrupt
 */[!CR!]
extern void [!"node:value(concat($xPathModule,'/WdgGeneral/WdgCallbackNotification'))"!](void);
[!VAR "WdgCbkFunc" = "node:value(concat($xPathModule,'/WdgGeneral/WdgCallbackNotification'))"!]
[!ELSE!]
[!VAR "WdgCbkFunc" = "'NULL_PTR'"!]
[!ENDIF!]
[!ENDIF!]

[!IF "node:exists($xPathModule)"!][!//
const Swt_Ip_ConfigType Swt_Ip_OffModeSettings_0[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]=
{
    (uint8)           SWT_IP_ALL_MAP_ENABLE, /* u8MapEnBitmask */
    (boolean)         FALSE,        /* bEnResetOnInvalidAccess */
#if (defined(SWT_IP_HAS_STOP_MODE) && (SWT_IP_HAS_STOP_MODE == 1U))
    (boolean)         FALSE,        /* bEnRunInStopMode */
#endif
    (boolean)         FALSE,        /* bEnRunInDebugMode */
    (boolean)         FALSE,        /* bEnWindow */
    (boolean)         FALSE,        /* bEnInterrupt */
    (Swt_Ip_ServiceModeType)     SWT_IP_FS_SEQ_MODE,        /* eServiceMode */
    (uint32)          0x100,    /* u32TimeoutValue */
    (uint32)          0x0,        /* u32WindowValue */
    (uint16)          0x0,        /* u16InitialKey */
    (Swt_Ip_LockType) [!IF "node:value(concat($xPathModule,'/WdgSettingsConfig/WdgSettingsOff/WdgSoftLockConfiguration'))"!]SWT_IP_SOFTLOCK[!/*
                      */!][!ELSEIF "node:value(concat($xPathModule,'/WdgSettingsConfig/WdgSettingsOff/WdgHardLockConfiguration'))"!]SWT_IP_HARDLOCK[!/*
                      */!][!ELSE!]SWT_IP_UNLOCK[!ENDIF!], /* lockConfig */
    NULL_PTR     /* pfSwtCallback */
};

const Swt_Ip_ConfigType Swt_Ip_SlowModeSettings_0[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]=
{
[!CALL "WdgModeConfig","WdgMode" = "'WdgSettingsSlow'","WdgNo" = "$xPathModule"!]
    [!IF "$WdgCbkFunc != 'NULL_PTR'"!]
    (Swt_CallbackPtrType) &[!"$WdgCbkFunc"!] /* pfSwtCallback */
    [!ELSE!]
    NULL_PTR         /* pfSwtCallback */
    [!ENDIF!]
};

const Swt_Ip_ConfigType Swt_Ip_FastModeSettings_0[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]=
{
[!CALL "WdgModeConfig","WdgMode" = "'WdgSettingsFast'","WdgNo" = "$xPathModule"!]
    [!IF "$WdgCbkFunc != 'NULL_PTR'"!]
    (Swt_CallbackPtrType) &[!"$WdgCbkFunc"!] /* pfSwtCallback */
    [!ELSE!]
    NULL_PTR        /* pfSwtCallback */
    [!ENDIF!]
};

[!ENDIF!][!//


#define WDG_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "Wdg_MemMap.h"

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

