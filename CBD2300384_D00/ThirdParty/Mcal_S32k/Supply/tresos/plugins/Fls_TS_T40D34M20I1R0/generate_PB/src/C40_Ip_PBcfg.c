[!CODE!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : C40_IP IPV_QSPI
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
*   @file C40_Ip_PBcfg.c
*
*   @addtogroup FLS
*   @{
*/

/* implements C40_Ip_PBcfg.c_Artifact */

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "C40_Ip.h"


/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLS_C40_IP_VENDOR_ID_CFG_C                          43
#define FLS_C40_IP_AR_RELEASE_MAJOR_VERSION_CFG_C           4
#define FLS_C40_IP_AR_RELEASE_MINOR_VERSION_CFG_C           4
#define FLS_C40_IP_AR_RELEASE_REVISION_VERSION_CFG_C        0
#define FLS_C40_IP_SW_MAJOR_VERSION_CFG_C                   2
#define FLS_C40_IP_SW_MINOR_VERSION_CFG_C                   0
#define FLS_C40_IP_SW_PATCH_VERSION_CFG_C                   1


/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and C40_Ip header file are of the same vendor */
#if (FLS_C40_IP_VENDOR_ID_CFG_C != C40_IP_VENDOR_ID_H)
    #error "C40_Ip_PBCfg.c and C40_Ip.h have different vendor ids"
#endif
/* Check if current file and C40_Ip header file are of the same Autosar version */
#if ((FLS_C40_IP_AR_RELEASE_MAJOR_VERSION_CFG_C    != C40_IP_AR_RELEASE_MAJOR_VERSION_H) || \
     (FLS_C40_IP_AR_RELEASE_MINOR_VERSION_CFG_C    != C40_IP_AR_RELEASE_MINOR_VERSION_H) || \
     (FLS_C40_IP_AR_RELEASE_REVISION_VERSION_CFG_C != C40_IP_AR_RELEASE_REVISION_VERSION_H) \
    )
    #error "AutoSar Version Numbers of C40_Ip_PBCfg.c and C40_Ip.h are different"
#endif
/* Check if current file and C40_Ip header file are of the same Software version */
#if ((FLS_C40_IP_SW_MAJOR_VERSION_CFG_C != C40_IP_SW_MAJOR_VERSION_H) || \
     (FLS_C40_IP_SW_MINOR_VERSION_CFG_C != C40_IP_SW_MINOR_VERSION_H) || \
     (FLS_C40_IP_SW_PATCH_VERSION_CFG_C != C40_IP_SW_PATCH_VERSION_H) \
    )
    #error "Software Version Numbers of C40_Ip_PBCfg.c and C40_Ip.h are different"
#endif


/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/
[!INDENT "0"!] [!//
[!IF "node:exists(FlsConfigSet/FlsStartFlashAccessNotif) or node:exists(FlsConfigSet/FlsFinishedFlashAccessNotif)"!][!//
    #define FLS_START_SEC_CODE
    #include "Fls_MemMap.h"
    [!LOOP "text:order(text:split(normalize-space(text:join(FlsConfigSet/FlsStartFlashAccessNotif | FlsConfigSet/FlsFinishedFlashAccessNotif))))"!][!//
        /* Declaration of Fls notification function ([!"."!]())*/
        extern void [!"."!]( void );
    [!ENDLOOP!][!//
    #define FLS_STOP_SEC_CODE
    #include "Fls_MemMap.h"
[!ENDIF!][!//
[!ENDINDENT!][!//


/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/
[!INDENT "0"!][!//
[!VAR "configName" = "as:name(FlsConfigSet)"!][!//

#define FLS_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fls_MemMap.h"
const C40_ConfigType [!"$configName"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_InitCfg =
{
    [!WS "4"!][!IF "node:exists(FlsConfigSet/FlsStartFlashAccessNotif)"!]&[!"FlsConfigSet/FlsStartFlashAccessNotif"!],[!ELSE!]NULL_PTR,[!ENDIF!]  /* FlsStartFlashAccessNotif */[!CR!][!//
    [!WS "4"!][!IF "node:exists(FlsConfigSet/FlsFinishedFlashAccessNotif)"!]&[!"FlsConfigSet/FlsFinishedFlashAccessNotif"!][!ELSE!]NULL_PTR[!ENDIF!]  /* FlsFinishedFlashAccessNotif */[!CR!][!//
};
#define FLS_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fls_MemMap.h"

[!ENDINDENT!][!//
[!ENDCODE!][!//
#ifdef __cplusplus
}
#endif

/** @}*/

