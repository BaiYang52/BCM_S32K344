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
*   @file Fls_Cfg.c
*
*   @addtogroup FLS
*   @{
*/

/* implements Fls_Cfg.c_Artifact */

#ifdef __cplusplus
extern "C"{
#endif

[!INDENT "0"!]
/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Fls.h"
[!ENDINDENT!][!//

#ifdef FLS_PRECOMPILE_SUPPORT

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLS_VENDOR_ID_PCFG_C                      43
#define FLS_AR_RELEASE_MAJOR_VERSION_PCFG_C       4
#define FLS_AR_RELEASE_MINOR_VERSION_PCFG_C       4
#define FLS_AR_RELEASE_REVISION_VERSION_PCFG_C    0
#define FLS_SW_MAJOR_VERSION_PCFG_C               2
#define FLS_SW_MINOR_VERSION_PCFG_C               0
#define FLS_SW_PATCH_VERSION_PCFG_C               1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and Fls header file are of the same vendor */
#if (FLS_VENDOR_ID_PCFG_C != FLS_VENDOR_ID)
    #error "Fls_Cfg.c and Fls.h have different vendor ids"
#endif
/* Check if current file and Fls header file are of the same Autosar version */
#if ((FLS_AR_RELEASE_MAJOR_VERSION_PCFG_C    != FLS_AR_RELEASE_MAJOR_VERSION) || \
     (FLS_AR_RELEASE_MINOR_VERSION_PCFG_C    != FLS_AR_RELEASE_MINOR_VERSION) || \
     (FLS_AR_RELEASE_REVISION_VERSION_PCFG_C != FLS_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Fls_Cfg.c and Fls.h are different"
#endif
/* Check if current file and Fls header file are of the same Software version */
#if ((FLS_SW_MAJOR_VERSION_PCFG_C != FLS_SW_MAJOR_VERSION) || \
     (FLS_SW_MINOR_VERSION_PCFG_C != FLS_SW_MINOR_VERSION) || \
     (FLS_SW_PATCH_VERSION_PCFG_C != FLS_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Fls_Cfg.c and Fls.h are different"
#endif

[!INCLUDE "Fls_VersionCheck.m"!][!//

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/


[!ENDCODE!]


#endif /* FLS_PRECOMPILE_SUPPORT */
#ifdef __cplusplus
}
#endif

/** @}*/
