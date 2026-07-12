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

#ifndef CRYIF_H
#define CRYIF_H

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
#include "Csm_Types.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CRYIF_VENDOR_ID                       43
#define CRYIF_MODULE_ID                       112
#define CRYIF_AR_RELEASE_MAJOR_VERSION        4
#define CRYIF_AR_RELEASE_MINOR_VERSION        4
#define CRYIF_AR_RELEASE_REVISION_VERSION     0
#define CRYIF_SW_MAJOR_VERSION                2
#define CRYIF_SW_MINOR_VERSION                0
#define CRYIF_SW_PATCH_VERSION                1

/*==================================================================================================
*                                    FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if CryIf header file and Csm types header file are of the same Autosar version */
    #if ((CRYIF_AR_RELEASE_MAJOR_VERSION != CSM_AR_RELEASE_MAJOR_VERSION_TYPES) || \
         (CRYIF_AR_RELEASE_MINOR_VERSION != CSM_AR_RELEASE_MINOR_VERSION_TYPES)    \
        )
        #error "AutoSar Version Numbers of CryIf.h and Csm_Types.h are different"
    #endif
#endif

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/**
*   @brief The service request failed because the service is still busy.
*/
#define CRYPTO_E_BUSY                       ((uint8)0x02U)

/**
* @brief The service request failed because the provided buffer is too small to store the result.
*/
#define CRYPTO_E_SMALL_BUFFER               ((uint8)0x03U)

/**
* @brief The service request failed because the entropy of the random number generator is exhausted.
*/
#define CRYPTO_E_ENTROPY_EXHAUSTION         ((uint8)0x04U)

/**
* @brief The service request failed because the queue is full.
*/
#define CRYPTO_E_QUEUE_FULL                 ((uint8)0x05U)

/**
* @brief   The service request failed because read access failed.
*/
#define CRYPTO_E_KEY_READ_FAIL              ((uint8)0x06U)

/**
* @brief The service request failed because write access failed.
*/
#define CRYPTO_E_KEY_WRITE_FAIL             ((uint8)0x07U)

/**
* @brief The service request failed because the key is not available.
*/
#define CRYPTO_E_KEY_NOT_AVAILABLE          ((uint8)0x08U)

/**
* @brief The service request failed because at least one needed key element is invalid.
*/
#define CRYPTO_E_KEY_NOT_VALID              ((uint8)0x09U)

/**
* @brief The service request failed because the key element is not partially accessible and the provided key element length is too short or too long for that key element.
*/
#define CRYPTO_E_KEY_SIZE_MISMATCH          ((uint8)0x0AU)

/**
* @brief The service request failed because the Job has been canceled.
*/
#define CRYPTO_E_JOB_CANCELED               ((uint8)0x0CU)

/**
* @brief The service request failed because of uninitialized source key element
*/
#define CRYPTO_E_KEY_EMPTY                  ((uint8)0x0DU)

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


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CRYIF_H */


