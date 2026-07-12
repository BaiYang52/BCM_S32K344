/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : DMA,CACHE,TRGMUX,LCU,EMIOS,FLEXIO
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

#ifndef MCL_H_
#define MCL_H_

/**
*   @file Mcl.h
*
*   @version 2.0.1
*
*   @brief   AUTOSAR Mcl - MCL driver header file.
*   @details 
*
*   @addtogroup MCL_DRIVER MCL Driver
*   @{
*/

#ifdef __cplusplus
extern "C"
{
#endif


/*==================================================================================================
*                                          INCLUDE FILES
*  1) system and project includes
*  2) needed interfaces from external units
*  3) internal and external interfaces from this unit
==================================================================================================*/
#include "CDD_Mcl.h"

/*==================================================================================================
*                                SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define MCL_MODULE_ID                       255
#define MCL_VENDOR_ID                       43
#define MCL_AR_RELEASE_MAJOR_VERSION        4
#define MCL_AR_RELEASE_MINOR_VERSION        4
#define MCL_AR_RELEASE_REVISION_VERSION     0
#define MCL_SW_MAJOR_VERSION                2
#define MCL_SW_MINOR_VERSION                0
#define MCL_SW_PATCH_VERSION                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if header file and CDD_Mcl.h file are of the same vendor */
#if (MCL_VENDOR_ID != CDD_MCL_VENDOR_ID)
    #error "Mcl.h and CDD_Mcl.h have different vendor ids"
#endif

/* Check if header file and CDD_Mcl.h file are of the same Autosar version */
#if ((MCL_AR_RELEASE_MAJOR_VERSION != CDD_MCL_AR_RELEASE_MAJOR_VERSION) || \
     (MCL_AR_RELEASE_MINOR_VERSION != CDD_MCL_AR_RELEASE_MINOR_VERSION) || \
     (MCL_AR_RELEASE_REVISION_VERSION != CDD_MCL_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Mcl.h and CDD_Mcl.h are different"
#endif

/* Check if header file and CDD_Mcl.h file are of the same Software version */
#if ((MCL_SW_MAJOR_VERSION != CDD_MCL_SW_MAJOR_VERSION) || \
     (MCL_SW_MINOR_VERSION != CDD_MCL_SW_MINOR_VERSION) || \
     (MCL_SW_PATCH_VERSION != CDD_MCL_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Mcl.h and CDD_Mcl.h are different"
#endif
 
/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        DEFINES AND MACROS
==================================================================================================*/

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
*                                      FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif  /* MCL_H_ */

/*==================================================================================================
 *                                        END OF FILE
==================================================================================================*/

