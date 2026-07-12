/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : CryIf
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

#ifndef CRYIF_CBK_H
#define CRYIF_CBK_H

/**
*   @file
*
*   @addtogroup CRYIF
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
==================================================================================================*/
#include "CryIf.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CRYIF_CBK_VENDOR_ID                       43
#define CRYIF_CBK_MODULE_ID                       112
#define CRYIF_CBK_AR_RELEASE_MAJOR_VERSION        4
#define CRYIF_CBK_AR_RELEASE_MINOR_VERSION        4
#define CRYIF_CBK_AR_RELEASE_REVISION_VERSION     0
#define CRYIF_CBK_SW_MAJOR_VERSION                2
#define CRYIF_CBK_SW_MINOR_VERSION                0
#define CRYIF_CBK_SW_PATCH_VERSION                1

/*==================================================================================================
*                                    FILE VERSION CHECKS
==================================================================================================*/
/* Check if CryIf callback header file and CryIf header file are of the same vendor */
#if (CRYIF_CBK_VENDOR_ID != CRYIF_VENDOR_ID)
    #error "CryIf_Cbk.h and CryIf.h have different vendor ids"
#endif

/* Check if CryIf callbacks header file and CryIf header file are of the same Autosar version */
#if ((CRYIF_CBK_AR_RELEASE_MAJOR_VERSION != CRYIF_AR_RELEASE_MAJOR_VERSION) || \
     (CRYIF_CBK_AR_RELEASE_MINOR_VERSION != CRYIF_AR_RELEASE_MINOR_VERSION) || \
     (CRYIF_CBK_AR_RELEASE_REVISION_VERSION != CRYIF_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of CryIf_Cbk.h and CryIf.h are different"
#endif

/* Check if CryIf callback header file and CryIf header file are of the same Software version */
#if ((CRYIF_CBK_SW_MAJOR_VERSION != CRYIF_SW_MAJOR_VERSION) || \
     (CRYIF_CBK_SW_MINOR_VERSION != CRYIF_SW_MINOR_VERSION) || \
     (CRYIF_CBK_SW_PATCH_VERSION != CRYIF_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CryIf_Cbk.h and CryIf.h are different"
#endif

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                     FUNCTION PROTOTYPES
==================================================================================================*/
void CryIf_CallbackNotification(Crypto_JobType* job, Std_ReturnType result);


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CRYIF_CBK_H */
