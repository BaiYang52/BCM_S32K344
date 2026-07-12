[!AUTOSPACING!][!//
[!CODE!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Emios Siul2 Wkpu LpCmp
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

#ifndef WKPU_IP_CFG_H
#define WKPU_IP_CFG_H

/**
 *   @file    Wkpu_Ip_Cfg.h
 *   @version 2.0.1
 *
 *   @brief   AUTOSAR Icu - contains the data exported by the Icu module
 *   @details Contains the information that will be exported by the module, as requested by Autosar.
 *
 *   @addtogroup wkpu_icu_ip WKPU IPL
 *   @{
 */
 
#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
*================================================================================================*/
[!NOCODE!][!//
[!IF "var:defined('postBuildVariant')"!][!//
    [!LOOP "variant:all()"!][!//
        [!CODE!][!WS "0"!]#include "Wkpu_Ip_[!"."!]_PBcfg.h"[!CR!][!ENDCODE!][!//
    [!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS "0"!]#include "Wkpu_Ip_PBcfg.h"[!CR!][!ENDCODE!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//

/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 *================================================================================================*/
#define WKPU_IP_CFG_VENDOR_ID                       43
#define WKPU_IP_CFG_AR_RELEASE_MAJOR_VERSION        4
#define WKPU_IP_CFG_AR_RELEASE_MINOR_VERSION        4
#define WKPU_IP_CFG_AR_RELEASE_REVISION_VERSION     0
#define WKPU_IP_CFG_SW_MAJOR_VERSION                2
#define WKPU_IP_CFG_SW_MINOR_VERSION                0
#define WKPU_IP_CFG_SW_PATCH_VERSION                1

/*==================================================================================================
 *                                      FILE VERSION CHECKS
 *================================================================================================*/
/* Check if header file and StandardTypes.h file are of the same Autosar version */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((WKPU_IP_CFG_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
         (WKPU_IP_CFG_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Wkpu_Ip_Cfg.h and StandardTypes.h are different."
    #endif
#endif

[!IF "var:defined('postBuildVariant')"!][!//
    [!LOOP "variant:all()"!][!//
/* Check if source file and ICU configuration header file are of the same vendor */
#if (WKPU_IP_CFG_VENDOR_ID != WKPU_IP_[!"."!]_PBCFG_VENDOR_ID)
    #error "Wkpu_Ip_Cfg.h and Wkpu_Ip_[!"."!]_PBcfg.h have different vendor IDs"
#endif
    /* Check if header file and Icu configuration header file are of the same Autosar version */
#if ((WKPU_IP_CFG_AR_RELEASE_MAJOR_VERSION != WKPU_IP_[!"."!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (WKPU_IP_CFG_AR_RELEASE_MINOR_VERSION != WKPU_IP_[!"."!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (WKPU_IP_CFG_AR_RELEASE_REVISION_VERSION != WKPU_IP_[!"."!]_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
#error "AutoSar Version Numbers of Wkpu_Ip_Cfg.h and Wkpu_Ip_[!"."!]_PBcfg.h are different"
#endif
/* Check if header file and Icu configuration header file are of the same software version */
#if ((WKPU_IP_CFG_SW_MAJOR_VERSION != WKPU_IP_[!"."!]_PBCFG_SW_MAJOR_VERSION) || \
     (WKPU_IP_CFG_SW_MINOR_VERSION != WKPU_IP_[!"."!]_PBCFG_SW_MINOR_VERSION) || \
     (WKPU_IP_CFG_SW_PATCH_VERSION != WKPU_IP_[!"."!]_PBCFG_SW_PATCH_VERSION) \
    )
#error "Software Version Numbers of Wkpu_Ip_Cfg.h and Wkpu_Ip_[!"."!]_PBcfg.h are different"
#endif
    [!ENDLOOP!][!//
[!ELSE!][!//
/* Check if source file and ICU configuration header file are of the same vendor */
#if (WKPU_IP_CFG_VENDOR_ID != WKPU_IP_PBCFG_VENDOR_ID)
    #error "Wkpu_Ip_Cfg.h and Wkpu_Ip_PBcfg.h have different vendor IDs"
#endif
    /* Check if header file and Icu configuration header file are of the same Autosar version */
#if ((WKPU_IP_CFG_AR_RELEASE_MAJOR_VERSION != WKPU_IP_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (WKPU_IP_CFG_AR_RELEASE_MINOR_VERSION != WKPU_IP_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (WKPU_IP_CFG_AR_RELEASE_REVISION_VERSION != WKPU_IP_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
#error "AutoSar Version Numbers of Wkpu_Ip_Cfg.h and Wkpu_Ip_PBcfg.h are different"
#endif
/* Check if header file and Icu configuration header file are of the same software version */
#if ((WKPU_IP_CFG_SW_MAJOR_VERSION != WKPU_IP_PBCFG_SW_MAJOR_VERSION) || \
     (WKPU_IP_CFG_SW_MINOR_VERSION != WKPU_IP_PBCFG_SW_MINOR_VERSION) || \
     (WKPU_IP_CFG_SW_PATCH_VERSION != WKPU_IP_PBCFG_SW_PATCH_VERSION) \
    )
#error "Software Version Numbers of Wkpu_Ip_Cfg.h and Wkpu_Ip_PBcfg.h are different"
#endif
[!ENDIF!][!//
/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/
#ifndef WKPU_IP_DERIVATIVE_NOT_SUPPORT_WKPU
#define WKPU_IP_DEV_ERROR_DETECT        ([!IF "IcuGeneral/IcuDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#define WKPU_IP_SET_MODE_API           ([!IF "IcuOptionalApis/IcuSetModeApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

[!IF "num:i(ecu:get('Icu.Num_Wkpu_Hw_Channels')) > '32'"!]
    [!CODE!][!//
#define WKPU_IP_64_CH_USED          (STD_ON)
    [!ENDCODE!][!//
[!ENDIF!]

[!NOCODE!][!//
    [!VAR "UsingWkpuInterrupt" = "0"!][!//
    [!LOOP "IcuConfigSet/IcuHwInterruptConfigList/*"!][!//
    [!IF "(IcuIsrEnable = 'true') and (contains(IcuIsrHwId, 'WKPU'))"!][!//
        [!VAR "UsingWkpuInterrupt"="$UsingWkpuInterrupt + 1"!][!//
    [!ENDIF!][!//
    [!ENDLOOP!][!//
      [!CODE!][!//
#define ICU_WKPU_SINGLE_INTERRUPT           ([!IF "$UsingWkpuInterrupt != 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
      [!ENDCODE!][!//
[!ENDNOCODE!][!//

[!IF "count(./IcuConfigSet/IcuWkpu/*) > '0'"!]
[!VAR "WkpuNoChannels" = "num:i(0)"!][!//
[!VAR "WkpuNoNMIs" = "num:i(0)"!][!//
[!LOOP "./IcuConfigSet/IcuWkpu/*"!]
    [!VAR "WkpuNoChannels"="$WkpuNoChannels + num:i(count(IcuWkpuChannels/*))"!][!//
    [!VAR "WkpuNoNMIs"="$WkpuNoNMIs + num:i(count(IcuWkpuNMIConfiguration/*))"!][!//
[!ENDLOOP!][!//
[!IF "($WkpuNoChannels > '0') or ($WkpuNoNMIs > '0')"!]
[!CODE!]/** @brief Macro used to export generated configuration. */[!CR!][!ENDCODE!]
[!CODE!]#define WKPU_CONFIG_EXT \[!CR!][!ENDCODE!][!//
[!IF "var:defined('postBuildVariant')"!][!//
    [!LOOP "variant:all()"!][!//
        [!CODE!][!WS "8"!]WKPU_CONFIG_[!"."!]_PB \[!CR!][!ENDCODE!]
    [!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS "8"!]WKPU_CONFIG_PB[!CR!][!ENDCODE!]
[!ENDIF!][!//
[!ENDIF!][!//
[!ENDIF!]

#endif /* WKPU_IP_DERIVATIVE_NOT_SUPPORT_WKPU */

#ifdef __cplusplus
}
#endif

/** @} */

#endif  /* WKPU_IP_CFG_H */
[!ENDCODE!][!//
