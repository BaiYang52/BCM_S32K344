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
#include "CDD_I3c.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define I3C_VENDOR_ID_CFG_C                      43
#define I3C_AR_RELEASE_MAJOR_VERSION_CFG_C       4
#define I3C_AR_RELEASE_MINOR_VERSION_CFG_C       4
#define I3C_AR_RELEASE_REVISION_VERSION_CFG_C    0
#define I3C_SW_MAJOR_VERSION_CFG_C               2
#define I3C_SW_MINOR_VERSION_CFG_C               0
#define I3C_SW_PATCH_VERSION_CFG_C               1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and CDD_I3c header file are of the same vendor */
#if (I3C_VENDOR_ID_CFG_C != I3C_VENDOR_ID)
    #error "CDD_I3c_Cfg.c and CDD_I3c.h have different vendor IDs"
#endif

/* Check if current file and CDD_I3c header file are of the same Autosar version */
#if ((I3C_AR_RELEASE_MAJOR_VERSION_CFG_C    != I3C_AR_RELEASE_MAJOR_VERSION) || \
     (I3C_AR_RELEASE_MINOR_VERSION_CFG_C    != I3C_AR_RELEASE_MINOR_VERSION) || \
     (I3C_AR_RELEASE_REVISION_VERSION_CFG_C != I3C_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of CDD_I3c_Cfg.c and CDD_I3c.h are different"
#endif

/* Check if current file and CDD_I3c header file are of the same software version */
#if ((I3C_SW_MAJOR_VERSION_CFG_C != I3C_SW_MAJOR_VERSION) || \
     (I3C_SW_MINOR_VERSION_CFG_C != I3C_SW_MINOR_VERSION) || \
     (I3C_SW_PATCH_VERSION_CFG_C != I3C_SW_PATCH_VERSION))
    #error "Software Version Numbers of CDD_I3c_Cfg.c and CDD_I3c.h are different"
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
