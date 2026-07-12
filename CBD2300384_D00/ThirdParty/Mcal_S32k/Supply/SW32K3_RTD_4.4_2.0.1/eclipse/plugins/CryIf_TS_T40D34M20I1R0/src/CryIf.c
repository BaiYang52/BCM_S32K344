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
#include "CryIf_Cbk.h"

/*==================================================================================================
*                                       SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CRYIF_VENDOR_ID_C                     43
#define CRYIF_AR_RELEASE_MAJOR_VERSION_C      4
#define CRYIF_AR_RELEASE_MINOR_VERSION_C      4
#define CRYIF_AR_RELEASE_REVISION_VERSION_C   0
#define CRYIF_SW_MAJOR_VERSION_C              2
#define CRYIF_SW_MINOR_VERSION_C              0
#define CRYIF_SW_PATCH_VERSION_C              1

/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/

/* Check if CryIf source file and CryIf callback header file are of the same vendor */
#if (CRYIF_VENDOR_ID_C != CRYIF_CBK_VENDOR_ID)
    #error "CryIf.c and CryIf_Cbk.h have different vendor ids"
#endif

/* Check if CryIf source file and CryIf callback header file are of the same Autosar version */
#if ((CRYIF_AR_RELEASE_MAJOR_VERSION_C != CRYIF_CBK_AR_RELEASE_MAJOR_VERSION) || \
     (CRYIF_AR_RELEASE_MINOR_VERSION_C != CRYIF_CBK_AR_RELEASE_MINOR_VERSION) || \
     (CRYIF_AR_RELEASE_REVISION_VERSION_C != CRYIF_CBK_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of CryIf.c and CryIf_Cbk.h are different"
#endif

/* Check if CryIf source file and CryIf callback header file are of the same Software version */
#if ((CRYIF_SW_MAJOR_VERSION_C != CRYIF_CBK_SW_MAJOR_VERSION) || \
     (CRYIF_SW_MINOR_VERSION_C != CRYIF_CBK_SW_MINOR_VERSION) || \
     (CRYIF_SW_PATCH_VERSION_C != CRYIF_CBK_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CryIf.c and CryIf_Cbk.h are different"
#endif

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
void CryIf_CallbackNotification(Crypto_JobType* job, Std_ReturnType result)
{
    (void)job;
    (void)result;
}



#ifdef __cplusplus
}
#endif

/** @} */


