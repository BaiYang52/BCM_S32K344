/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : HSE
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

#ifndef CRYPTO_TYPES_H
#define CRYPTO_TYPES_H

/**
*   @file
*
*   @internal
*   @addtogroup CRYPTO
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
#include "StandardTypes.h"
#include "Csm_Types.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CRYPTO_VENDOR_ID_TYPES                       43
#define CRYPTO_AR_RELEASE_MAJOR_VERSION_TYPES        4
#define CRYPTO_AR_RELEASE_MINOR_VERSION_TYPES        4
#define CRYPTO_AR_RELEASE_REVISION_VERSION_TYPES     0
#define CRYPTO_SW_MAJOR_VERSION_TYPES                2
#define CRYPTO_SW_MINOR_VERSION_TYPES                0
#define CRYPTO_SW_PATCH_VERSION_TYPES                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if header file and StandardTypes header file are of the same AutoSar version */
    #if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_TYPES != STD_AR_RELEASE_MAJOR_VERSION) || \
         (CRYPTO_AR_RELEASE_MINOR_VERSION_TYPES != STD_AR_RELEASE_MINOR_VERSION)    \
        )
        #error "AutoSar Version Numbers of Crypto_Types.h and StandardTypes.h are different"
    #endif
    
    /* Check if header file and Csm types header header file are of the same AutoSar version */
    #if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_TYPES != CSM_AR_RELEASE_MAJOR_VERSION_TYPES) || \
         (CRYPTO_AR_RELEASE_MINOR_VERSION_TYPES != CSM_AR_RELEASE_MINOR_VERSION_TYPES)    \
        )
        #error "AutoSar Version Numbers of Crypto_Types.h and Csm_Types.h are different"
    #endif
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/
typedef enum
{
    CRYPTO_WA_DENIED        = 0x01U,
    CRYPTO_WA_INTERNAL_COPY = 0x02U,
    CRYPTO_WA_ALLOWED       = 0x03U,
    CRYPTO_WA_ENCRYPTED     = 0x04U
} Crypto_KeyElementWriteAccessType;

typedef enum
{
    CRYPTO_RA_DENIED        = 0x01U,
    CRYPTO_RA_INTERNAL_COPY = 0x02U,
    CRYPTO_RA_ALLOWED       = 0x03U,
    CRYPTO_RA_ENCRYPTED     = 0x04U
} Crypto_KeyElementReadAccessType;

typedef enum
{
    CRYPTO_KE_FORMAT_BIN_OCTET                       = 0x01U,
    CRYPTO_KE_FORMAT_BIN_SHEKEYS                     = 0x02U,
    CRYPTO_KE_FORMAT_BIN_IDENT_PRIVATEKEY_PKCS8      = 0x03U,
    CRYPTO_KE_FORMAT_BIN_IDENT_PUBLICKEY             = 0x04U,
    CRYPTO_KE_FORMAT_BIN_RSA_PRIVATEKEY              = 0x05U,
    CRYPTO_KE_FORMAT_BIN_RSA_PUBLICKEY               = 0x06U,
    CRYPTO_KE_FORMAT_BIN_CERT_X509_V3                = 0x07U,
    CRYPTO_KE_FORMAT_BIN_CERT_CVC                    = 0x08U
} Crypto_KeyFormatType;

/**
* @brief Enum containing the possible states of the Crypto driver
*/
typedef enum
{
    CRYPTO_DRIVER_UNINIT      = 0x00U,
    CRYPTO_DRIVER_INITIALIZED = 0x01U
} Crypto_DriverStateType;

/**
* @brief Enum containing the possible primitives supported by the Crypto driver
*/
typedef enum
{
    MAC_VERIFY              = 0x01U,   /* Mac Verify Service */
    MAC_GENERATE            = 0x02U,   /* Mac Generate Service */
    SIGNATURE_VERIFY        = 0x03U,   /* Signature Verify Service */
    SIGNATURE_GENERATE      = 0x04U,   /* Signature Generate Service */
    ENCRYPT                 = 0x05U,   /* Encrypt Service */
    DECRYPT                 = 0x06U,   /* Decrypt Service */
    AEAD_ENCRYPT            = 0x07U,   /* AEAD Encrypt Service*/
    AEAD_DECRYPT            = 0x08U,   /* AEAD Decrypt Service */
    HASH                    = 0x09U,   /* Hash Service */
    RANDOM                  = 0x0AU    /* Random Service */
} Crypto_PrimitiveServiceType;

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
typedef struct
{
    const Crypto_PrimitiveServiceType eService;
    const uint8                       u8AlgoFamily;
    const uint8                       u8AlgoMode;
    const uint8                       u8SecondaryAlgoFamily;
} Crypto_PrimitiveType;

typedef struct
{
    uint32          u32Next;
    Crypto_JobType* pJob;
} Crypto_QueueElementType;

/* Structure storing information about the elements in the queue of a Crypto Driver Object */
typedef struct
{
    uint32 u32HeadOfFreeJobs;
    uint32 u32HeadOfQueuedJobs;
}Crypto_ObjectQueueType;

typedef struct
{
    const uint32        u32NumCryptoKeyElements;
    const uint32* const pCryptoKeyElementList;
} Crypto_KeyType;

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CRYPTO_TYPES_H */
