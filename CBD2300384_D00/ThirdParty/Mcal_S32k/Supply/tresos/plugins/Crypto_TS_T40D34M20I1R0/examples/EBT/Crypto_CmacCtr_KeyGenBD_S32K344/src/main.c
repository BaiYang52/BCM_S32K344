/*
*   (c) Copyright 2021 NXP
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be used strictly
*   in accordance with the applicable license terms.  By expressly accepting
*   such terms or by downloading, installing, activating and/or otherwise using
*   the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms.  If you do not agree to
*   be bound by the applicable license terms, then you may not retain,
*   install, activate or otherwise use the software.
*
*   This file contains sample code only. It is not part of the production code deliverables.
*/

#ifdef __cplusplus
extern "C" {
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Crypto.h"
#include "OsIf.h"
#include "check_example.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
/* Function-like macros to set and get the status of the example run */
#define App_SetSuccessStatus(value)                 (u32NumFailedOperations += ((value) ? 0U : 1U))
#define App_GetSuccessStatus()                      (0U == u32NumFailedOperations)

/* Identifier of the KeyElement that contains the key material */
#define KEY_MATERIAL_ELEMENT_ID_U32                 ((uint32)1U)

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL MACROS related to the 'CMAC with counter' part of the example                          */
/* ----------------------------------------------------------------------------------------------- */
/* Take the generated value of the CMAC Counter key id from the configuration file */
#define APP_CMAC_KEY_ID                             (CryptoConf_CryptoKey_Crypto_Key_CMAC_Counter)
/* Size in bytes of the CMAC generate/verify key */
#define APP_CMAC_KEY_SIZE                           (16U)
/* Size of the CMAC resulted tag */
#define APP_CMAC_TAG_SIZE                           (16U)
/* Size of the data used to compute CMAC over */
#define APP_CMAC_DATA_SIZE                          (16U)

/* Defines related to the monotonic counter used for CMAC Generate */
#define APP_CMAC_GENERATE_CTR_IDX                   (0U)
#define APP_CMAC_GENERATE_CTR_RP_NUM_BITS           (40U)
#define APP_CMAC_GENERATE_CTR_MAX_VOLATILE_VALUE    (0xFFFFFFU)

/* Defines related to the monotonic counter used for CMAC Verify */
#define APP_CMAC_VERIFY_CTR_IDX                     (1U)
#define APP_CMAC_VERIFY_CTR_RP_NUM_BITS             (40U)
#define APP_CMAC_VERIFY_CTR_INCREMENT_VALUE         (0x1000000U)
#define APP_CMAC_VERIFY_CTR_RP_OFFSET               (0x0U)
#define APP_CMAC_VERIFY_CTR_INIT_VOLATILE_VALUE     (50000U)

/* HSE Key Handle identifier for the key AES128 key that will be used for CMAC generation/verification
   It is the HSE key handle of the key material element of the Crypto key CryptoConf_CryptoKey_Crypto_Key_CMAC_Counter */
#define APP_CMAC_KEY_HANDLE                         GET_KEY_HANDLE(HSE_KEY_CATALOG_ID_RAM, 0, 0)

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL MACROS related to the 'Burmester-Desmedt key generation' part of the example           */
/* ----------------------------------------------------------------------------------------------- */
/* Handles of the HSE keys used in descriptors requesting various services from HSE FW */
#define APP_ECC_KEY_PAIR_HANDLE                     (GET_KEY_HANDLE(HSE_KEY_CATALOG_ID_RAM, 2, 0))
#define APP_ECC_KEY_PUB_HANDLE                      (GET_KEY_HANDLE(HSE_KEY_CATALOG_ID_RAM, 3, 0))
#define APP_SHARED_SECRET_HANDLE                    (GET_KEY_HANDLE(HSE_KEY_CATALOG_ID_RAM, 4, 0))
#define APP_HMAC_KEY_HANDLE                         (GET_KEY_HANDLE(HSE_KEY_CATALOG_ID_RAM, 5, 0))

/* ID of the Crypto Driver Object used when calling Crypto_ProcessJob() service */
#define APP_CDO_ID                                  (CryptoConf_CryptoDriverObject_CDO_Symmetric)

/* IDs of the Crypto keys used when calling Crypto services */
#define APP_CRYPTO_HMAC_KEY_ID                      (CryptoConf_CryptoKey_Crypto_Key_HMAC)
#define APP_CRYPTO_EXPORT_HMAC_KEY_ID               (CryptoConf_CryptoKey_Crypto_Key_To_Export_HMAC_Key)
#define APP_CRYPTO_DECRYPT_HMAC_KEY_ID              (CryptoConf_CryptoKey_Crypto_Key_To_Decrypt_HMAC_Key)

/* Size of an AES256 key in bytes */
#define APP_AES256_KEY_SIZE                         (32U)

/* Size of an ECC P244 curve elements in bits */
#define APP_CURVE_P224_BIT_SIZE                     (224U)

/* Sizes of the encrypted and plain shared secret, in bytes */
#define APP_ENCRYPTED_SHARED_SECRET_SIZE            (64U)
#define APP_PLAIN_SHARED_SECRET_SIZE                (56U)

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/
/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL CONSTANTS related to the 'CMAC with counter' part of the example                       */
/* ----------------------------------------------------------------------------------------------- */

/* Payload of the AES128 key that will be used for CMAC generation/verification */
static const uint8 App_au8CmacKey[APP_CMAC_KEY_SIZE] __attribute__((aligned)) =
{
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};

/* Data used to compute the CMAC tag over */
static const uint8 App_au8CmacData[APP_CMAC_DATA_SIZE] =
{
    0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a
};

/* Expected value of the tag obtained after performing CMAC generate over the data in App_au8CmacData */
static const uint8 App_au8ExpectedCmacGenerateTag[APP_CMAC_TAG_SIZE] =
{
    0x6f, 0xe9, 0xde, 0x94, 0x09, 0x62, 0x76, 0x12, 0x1B, 0x59, 0x2f, 0xe3, 0x8e, 0xe3, 0x9c, 0xdd
};

/* Value of the tag sent to CMAC verify to compare with the CMAC tag performed over the data in App_au8CmacData */
static const uint8 App_au8CmacTagVerify[APP_CMAC_TAG_SIZE] =
{
    0x62, 0x34, 0x01, 0x27, 0xcb, 0xb9, 0xc3, 0x55, 0xa4, 0x07, 0x34, 0xb8, 0xc1, 0x06, 0x67, 0xff
};

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL CONSTANTS related to the 'Burmester-Desmedt key generation' part of the example        */
/* ----------------------------------------------------------------------------------------------- */
/* Elliptic curve prime p */
static const uint8_t App_au8Ecc_SECP224R1_Prime_P[] =
{
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
};

/* Elliptic curve coefficient a */
static const uint8_t App_au8Ecc_SECP224R1_CurveParam_A[] =
{
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE
};

/* Elliptic curve coefficient b */
static const uint8_t App_au8Ecc_SECP224R1_CurveParam_B[] =
{
    0xB4, 0x05, 0x0A, 0x85, 0x0C, 0x04, 0xB3, 0xAB, 0xF5, 0x41, 0x32, 0x56, 0x50, 0x44, 0xB0, 0xB7,
    0xD7, 0xBF, 0xD8, 0xBA, 0x27, 0x0B, 0x39, 0x43, 0x23, 0x55, 0xFF, 0xB4
};

/* Elliptic curve base point G(x,y) */
static const uint8_t App_au8Ecc_SECP224R1_Basepoint[] =
{
    0xB7, 0x0E, 0x0C, 0xBD, 0x6B, 0xB4, 0xBF, 0x7F, 0x32, 0x13, 0x90, 0xB9, 0x4A, 0x03, 0xC1, 0xD3,
    0x56, 0xC2, 0x11, 0x22, 0x34, 0x32, 0x80, 0xD6, 0x11, 0x5C, 0x1D, 0x21, 0xBD, 0x37, 0x63, 0x88,
    0xB5, 0xF7, 0x23, 0xFB, 0x4C, 0x22, 0xDF, 0xE6, 0xCD, 0x43, 0x75, 0xA0, 0x5A, 0x07, 0x47, 0x64,
    0x44, 0xD5, 0x81, 0x99, 0x85, 0x00, 0x7E, 0x34
};

/* Elliptic curve order n */
static const uint8_t App_au8Ecc_SECP224R1_Order_N[] =
{
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x16, 0xA2,
    0xE0, 0xB8, 0xF0, 0x3E, 0x13, 0xDD, 0x29, 0x45, 0x5C, 0x5C, 0x2A, 0x3D
};

/* Private key of node 1 */
static const uint8_t App_au8A1[] =
{
    0x33, 0xb5, 0x8b, 0xf6, 0xc8, 0x4a, 0xb9, 0x7e, 0xa4, 0xd6, 0xde, 0x9a, 0x59, 0x25, 0x25, 0xdf,
    0xa2, 0xf7, 0x24, 0x50, 0x23, 0x64, 0x2c, 0xe6, 0xf4, 0xd4, 0x90, 0xbb
};

/* Private key of node 2 */
static const uint8_t App_au8A2[] =
{
    0x0c, 0x24, 0xfe, 0xa3, 0xe5, 0x04, 0x95, 0xd2, 0xb4, 0x13, 0xdb, 0xac, 0xfe, 0x88, 0xd2, 0x13,
    0x89, 0x4f, 0x9e, 0x70, 0x98, 0xca, 0x17, 0x91, 0xad, 0xde, 0xca, 0xc7
};

/* Private key of node 3 */
static const uint8_t App_au8A3[] =
{
    0x33, 0xb5, 0xbc, 0x34, 0x6f, 0x4a, 0xa4, 0x51, 0x33, 0x7a, 0x6e, 0x34, 0xed, 0x5a, 0xd0, 0x32,
    0x96, 0xd4, 0xda, 0xb1, 0xbe, 0x44, 0x93, 0x65, 0xac, 0xfe, 0xab, 0x58
};

/* First public key of node 1 */
static const uint8_t App_au8Z1[] =
{
    0x1E, 0xC5, 0x91, 0xD3, 0x19, 0xD9, 0x87, 0x2E, 0x2F, 0x62, 0xC6, 0x9D, 0xC1, 0xC4, 0xCA, 0xE6,
    0x7D, 0x3E, 0xBD, 0x03, 0x19, 0x20, 0xB9, 0xBB, 0x7D, 0x06, 0xE9, 0x94, 0x51, 0xEC, 0x05, 0xC4,
    0x1E, 0x12, 0x53, 0xB4, 0x28, 0x29, 0xA5, 0x13, 0xA8, 0x30, 0x17, 0x45, 0x2A, 0x96, 0x91, 0xBB,
    0xE2, 0x3C, 0xB3, 0x97, 0x0D, 0x71, 0xD9, 0xC3
};

/* First public key of node 2 */
static const uint8_t App_au8Z2[] =
{
    0x0E, 0x82, 0x0B, 0x7F, 0x49, 0x2E, 0x06, 0x41, 0x4A, 0x66, 0xED, 0xB8, 0x2B, 0x7C, 0x1B, 0xAB,
    0xB5, 0xEC, 0xB3, 0x67, 0x30, 0xCC, 0xBD, 0x69, 0x29, 0x99, 0xD5, 0x33, 0x82, 0xE8, 0x95, 0x98,
    0x01, 0x69, 0x27, 0x30, 0xB2, 0x5C, 0xD3, 0x5F, 0x71, 0xC8, 0x94, 0x2E, 0x5C, 0x4D, 0x5A, 0xCA,
    0x80, 0x2B, 0x3A, 0x13, 0xEC, 0x92, 0x84, 0x6C
};

/* First public key of node 3 */
static const uint8_t App_au8Z3[] =
{
    0xC0, 0x83, 0x55, 0x21, 0x78, 0xB0, 0xAC, 0x17, 0x3E, 0xF5, 0xCB, 0xF9, 0xF5, 0xB6, 0x16, 0xD7,
    0x70, 0x64, 0x49, 0x28, 0xC2, 0x01, 0xF1, 0xBB, 0x7A, 0xA3, 0xDA, 0x1C, 0x89, 0x6C, 0x52, 0x0F,
    0x3C, 0x4D, 0x94, 0xEB, 0xA4, 0xC8, 0xAD, 0xCB, 0x35, 0x33, 0x6E, 0x55, 0x58, 0x25, 0xCD, 0x8A,
    0xB3, 0x10, 0xF1, 0x6A, 0x7A, 0xC7, 0xDB, 0xAC
};

/* Second public key of node 1 */
static const uint8_t App_au8X1[] =
{
    0x85, 0xF2, 0x74, 0x78, 0xDF, 0x00, 0x7E, 0x15, 0xD1, 0x01, 0xDB, 0x07, 0xE3, 0x59, 0x2E, 0x0A,
    0xEC, 0x73, 0xCF, 0xEC, 0x96, 0x0C, 0x16, 0x70, 0xD0, 0xA3, 0xDE, 0xF2, 0xBB, 0xC2, 0xE9, 0x14,
    0x5F, 0xB3, 0x46, 0xDC, 0xA9, 0xDB, 0xBB, 0xE0, 0x4A, 0x22, 0x9C, 0x9E, 0x12, 0x84, 0xBB, 0x67,
    0x0C, 0x3F, 0x5D, 0x5B, 0xA9, 0x64, 0x73, 0xE8
};

/* Second public key of node 2 */
static const uint8_t App_au8X2[] =
{
    0x8E, 0xCC, 0xBB, 0x99, 0x0D, 0xF9, 0x42, 0x99, 0x09, 0x2A, 0xD4, 0xF9, 0x96, 0x4F, 0x17, 0x0E,
    0xE3, 0xBE, 0x6A, 0x79, 0xA8, 0xEA, 0xA2, 0xCC, 0x5B, 0xD0, 0x1D, 0x06, 0xAE, 0x98, 0xC1, 0xDB,
    0xF4, 0x85, 0x17, 0xA8, 0x1B, 0xF9, 0xB0, 0x44, 0x31, 0x2A, 0x42, 0x1B, 0x2B, 0x5C, 0x1A, 0x2B,
    0xE6, 0xD4, 0xA1, 0x23, 0x0A, 0x9C, 0xB8, 0x77
};

/* Second public key of node 3 */
static const uint8_t App_au8X3[] =
{
    0x65, 0x8F, 0x92, 0xEB, 0xBD, 0x00, 0x3F, 0x33, 0x02, 0x76, 0xB3, 0x6D, 0xA5, 0x26, 0xF8, 0x29,
    0x07, 0x17, 0x0B, 0xEF, 0x76, 0x64, 0xF0, 0xD9, 0xF3, 0xD6, 0x20, 0x24, 0xC4, 0x82, 0x0C, 0xAB,
    0x8B, 0x3C, 0xBF, 0x84, 0xFF, 0xFF, 0xC8, 0x3A, 0x21, 0x57, 0xFD, 0xE4, 0x49, 0x21, 0x47, 0x85,
    0xFF, 0xBB, 0x24, 0x85, 0x72, 0x26, 0x19, 0x76
};

/* Expected value of the shared secret computed by any of the 3 nodes */
static const uint8_t App_au8ExpectedSharedSecret[] =
{
    0xA4, 0xF7, 0xCE, 0x86, 0xB4, 0x8B, 0xB1, 0xBF, 0x59, 0xC3, 0xE0, 0x69, 0xE6, 0x89, 0x9B, 0x5C,
    0xD0, 0x1B, 0xF9, 0xF7, 0xE7, 0xBC, 0xCC, 0x0A, 0x3C, 0xB3, 0x37, 0x17, 0x3E, 0xAC, 0x91, 0x38,
    0x2D, 0xCC, 0xA4, 0xCE, 0xF6, 0x4F, 0x2B, 0x2E, 0x92, 0xDD, 0x44, 0x6E, 0x50, 0x50, 0x52, 0xBF,
    0x44, 0x33, 0x0F, 0xE8, 0x0F, 0x78, 0x91, 0x2F
};

/* AES 256 key used to export encrypted and authenticated the HMAC key containing the copied shared secret, and also used for decrypting it */
static const uint8 App_au8Aes256KeyToExportAndDecryptHMAC[APP_AES256_KEY_SIZE] =
{
    0x95, 0xd8, 0xaf, 0xb8, 0xa4, 0xb7, 0x24, 0x5c, 0xe7, 0x9f, 0x9f, 0x9c, 0x5d, 0xdd, 0x40, 0xde,
    0x61, 0xb3, 0x59, 0x05, 0xdc, 0xb6, 0x38, 0xf2, 0xb8, 0x75, 0x40, 0x4a, 0x98, 0x5b, 0x3f, 0x7a
};

/* Crypto job sub-structure for AES 256 ECB decrypt of the encrypted shared secret */
static const Crypto_PrimitiveInfoType App_DecryptSharedSecretPrimitiveInfo =
{
    APP_PLAIN_SHARED_SECRET_SIZE,       /* resultLength                - Contains the result length in bytes. */
    CRYPTO_DECRYPT,                     /* service                     - Contains the enum of the used service, e.g. Encrypt */
    {
        CRYPTO_ALGOFAM_AES,             /* family                      - The family of the algorithm */
        CRYPTO_ALGOFAM_NOT_SET,         /* secondaryFamily             - The secondary family of the algorithm  */
        APP_AES256_KEY_SIZE,            /* keyLength                   - The key length in bits to be used with that algorithm */
        CRYPTO_ALGOMODE_ECB             /* mode                        - The operation mode to be used with that algorithm */
    }
};

/* Crypto job sub-structure for AES 256 ECB decrypt of the encrypted shared secret */
static const Crypto_JobInfoType App_JobDecryptSharedSecretInfo =
{
    0U,                                 /* jobId                       - The identifier of the job */
    0U                                  /* jobPriority                 - Specifies the importance of the job (the higher, the more important) */
};

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
/* Variable storing the number of failed operations in the example */
static uint32 u32NumFailedOperations = 0U;

#define CRYPTO_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Crypto_MemMap.h"

/* Descriptor for creating a synchronous request to HSE using Csm_KeyElementGet() API with keyElementId set to 0xFEEDDE5C */
static hseSrvDescriptor_t Hse_SrvDescriptor;

static hseKeyInfo_t Hse_KeyInfo;

#define CRYPTO_STOP_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Crypto_MemMap.h"

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL VARIABLES related to the 'CMAC with counter' part of the example                       */
/* ----------------------------------------------------------------------------------------------- */
/* The following RAM variables need to be put in non-cacheable memory in order to be correctly seen by HSE */
#define CRYPTO_START_SEC_VAR_CLEARED_32_NO_CACHEABLE
#include "Crypto_MemMap.h"

/* Variable storing the volatile part of the monotonic counter used in CMAC generate operation */
static uint32 App_u32CmacGenerateCtrVolatileVal;

/* Variable storing the volatile part of the monotonic counter used in CMAC verify operation */
static uint32 App_u32CmacVerifyCtrVolatileVal;

#define CRYPTO_STOP_SEC_VAR_CLEARED_32_NO_CACHEABLE
#include "Crypto_MemMap.h"


#define CRYPTO_START_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Crypto_MemMap.h"

/* Buffer where HSE FW will upload the tag of the CMAC generate operation */
static uint8  App_au8CmacGenerateTagBuffer[APP_CMAC_TAG_SIZE];

#define CRYPTO_STOP_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Crypto_MemMap.h"

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL VARIABLES related to the 'Burmester-Desmedt key generation' part of the example        */
/* ----------------------------------------------------------------------------------------------- */
/* The following RAM variables need to be put in non-cacheable memory in order to be correctly seen by HSE */
#define CRYPTO_START_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Crypto_MemMap.h"

/* Buffer where the encrypted shared secret will be exported from HSE */
static uint8 App_au8EncryptedSharedSecret[APP_ENCRYPTED_SHARED_SECRET_SIZE];
/* Buffer where the plain shared secret will be decrypted by HSE */
static uint8 App_au8PlainSharedSecret[APP_PLAIN_SHARED_SECRET_SIZE];

#define CRYPTO_STOP_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Crypto_MemMap.h"


#define CRYPTO_START_SEC_VAR_INIT_32_NO_CACHEABLE
#include "Crypto_MemMap.h"

/* In/Out variable containing the size of the encrypted shared secret */
static uint32 App_u32EncryptedSharedSecretSize = APP_ENCRYPTED_SHARED_SECRET_SIZE;
/* In/Out variable containing the size of the plain shared secret */
static uint32 App_u32PlainSharedSecretSize     = APP_ENCRYPTED_SHARED_SECRET_SIZE;

#define CRYPTO_STOP_SEC_VAR_INIT_32_NO_CACHEABLE
#include "Crypto_MemMap.h"


/* Crypto job sub-structure for AES 256 ECB decrypt of the encrypted shared secret */
static Crypto_JobPrimitiveInfoType App_JobDecryptSharedSecretPrimitiveInfo =
{
    0U,                                    /* callbackId                 - Identifier of the callback function, to be called, if the configured service finished. */
    &App_DecryptSharedSecretPrimitiveInfo, /* primitiveInfo              - Pointer to a structure containing further configuration of the crypto primitives */
    APP_CRYPTO_DECRYPT_HMAC_KEY_ID,        /* cryIfKeyId                 - Identifier of the CryIf key. */
    CRYPTO_PROCESSING_SYNC,                /* processingType             - Determines the synchronous or asynchronous behavior. */
    (boolean)FALSE                         /* callbackUpdateNotification - Indicates, whether the callback function shall be called, if the UPDATE operation has finished. */
};

/* Structure of the job to be passed to Crypto driver, requesting AES 256 ECB decrypt of the encrypted shared secret */
static Crypto_JobType App_JobDecryptSharedSecret =
{
    0U,                                      /* jobId                      - Identifier for the job structure */
    CRYPTO_JOBSTATE_IDLE,                    /* jobState                   - Determines the current job state */
    {
        App_au8EncryptedSharedSecret,        /* inputPtr                   - Pointer to the input data. */
        APP_ENCRYPTED_SHARED_SECRET_SIZE,    /* inputLength                - Contains the input length in bytes. */
        NULL_PTR,                            /* secondaryInputPtr          - Pointer to the secondary input data (for MacVerify, SignatureVerify). */
        0U,                                  /* secondaryInputLength       - Contains the secondary input length in bytes. */
        NULL_PTR,                            /* tertiaryInputPtr           - Pointer to the tertiary input data (for MacVerify, SignatureVerify). */
        0U,                                  /* tertiaryInputLength        - Contains the tertiary input length in bytes. */
        App_au8PlainSharedSecret,            /* outputPtr                  - Pointer to the output data. */
        &App_u32PlainSharedSecretSize,       /* outputLengthPtr            - Holds a pointer to a memory location containing the output length in bytes. */
        NULL_PTR,                            /* secondaryOutputPtr         - Pointer to the secondary output data. */
        NULL_PTR,                            /* secondaryOutputLengthPtr   - Holds a pointer to a memory location containing the secondary output length in bytes. */
        0U,                                  /* input64                    - Versatile input parameter */
        NULL_PTR,                            /* verifyPtr                  - Output pointer to a memory location holding a Crypto_VerifyResultType */
        NULL_PTR,                            /* output64Ptr                - Output pointer to a memory location holding an uint64. */
        CRYPTO_OPERATIONMODE_SINGLECALL,     /* mode                       - Indicator of the mode(s)/operation(s) to be performed */
        0U,                                  /* cryIfKeyId                 - Holds the CryIf key id for key operation services. */
        0U,                                  /* targetCryIfKeyId           - Holds the target CryIf key id for key operation services. */
    },
    &App_JobDecryptSharedSecretPrimitiveInfo, /* jobPrimitiveInfo           - Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    &App_JobDecryptSharedSecretInfo,          /* jobInfo                    - Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    NULL_PTR,                                 /* jobRedirectionInfoRef      - Pointer to a structure containing further information on the usage of keys as input and output for jobs. */
    0U                                        /* cryptoKeyId                - Identifier of the Crypto Driver key. The identifier shall be written by the Crypto Interface */
};

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
static void App_CmacWithCounterExample(void);
static void App_BurmesterDesmedtKeyGenerationExample(void);

/* Utility functions */
static boolean Util_Memcmp
(
    uint8* pSource,
    uint8* pDest,
    uint32 u32Size
);

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
/**
* @brief          Performs a CMAC generate with counter followed by a CMAC verify with counter operations
*
* @details        Exercises the CMAC with counter Generate/Verify features of the HSE firwmare using the Crypto_KeyElementGet() API of the Cryto driver
*
*/
static void App_CmacWithCounterExample(void)
{
    Std_ReturnType         RetVal;
    /* Variable storing the response from Hse after feeding a Hse descriptor using Crypto_KeyElementGet() API with a keyElementId set to 0xFEEDDE5C */
    hseSrvResponse_t       HseResponse;

    /* =============================================================================================================================== */
    /*    Key management                                                                                                               */
    /* =============================================================================================================================== */
    /* Load the CMAC key */
    RetVal = Crypto_KeyElementSet(APP_CMAC_KEY_ID, KEY_MATERIAL_ELEMENT_ID_U32, App_au8CmacKey, APP_CMAC_KEY_SIZE);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* =============================================================================================================================== */
    /*    Generate CMAC with counter                                                                                                   */
    /* =============================================================================================================================== */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Configure the monotonic counter                                                                                              */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /* Prepare the HSE descriptor */
    Hse_SrvDescriptor.srvId                                = HSE_SRV_ID_CONFIG_COUNTER;
    Hse_SrvDescriptor.hseSrv.configSecCounter.counterIndex = APP_CMAC_GENERATE_CTR_IDX;
    Hse_SrvDescriptor.hseSrv.configSecCounter.RPBitSize    = APP_CMAC_GENERATE_CTR_RP_NUM_BITS;
    /* Send the HSE descriptor to the firmware for synchronous processing */
    RetVal = Crypto_KeyElementGet(CryptoConf_CryptoKey_Crypto_Key_FEEDDE5C,      /* uint32  keyId           */
                                  0xFEEDDE5CU,                                   /* uint32  keyElementId    */
                                  (uint8*)&Hse_SrvDescriptor,                    /* uint8*  resultPtr       */
                                  (uint32*)&HseResponse                          /* uint32* resultLengthPtr */
                                 );
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Increment the monotonic counter to the maximum value of the volatile part                                                    */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /* Prepare the HSE descriptor */
    Hse_SrvDescriptor.srvId                             = HSE_SRV_ID_INCREMENT_COUNTER;
    Hse_SrvDescriptor.hseSrv.incCounterReq.counterIndex = APP_CMAC_GENERATE_CTR_IDX;
    Hse_SrvDescriptor.hseSrv.incCounterReq.value        = APP_CMAC_GENERATE_CTR_MAX_VOLATILE_VALUE;
    /* Send the HSE descriptor to the firmware for synchronous processing */
    RetVal = Crypto_KeyElementGet(CryptoConf_CryptoKey_Crypto_Key_FEEDDE5C,      /* uint32  keyId           */
                                  0xFEEDDE5CU,                                   /* uint32  keyElementId    */
                                  (uint8*)&Hse_SrvDescriptor,                    /* uint8*  resultPtr       */
                                  (uint32*)&HseResponse                          /* uint32* resultLengthPtr */
                                 );
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Generate CMAC with counter                                                                                                   */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /* Initialize the variable used by HSE to return the volatile part of the used monotonic counter to some random value */
    App_u32CmacGenerateCtrVolatileVal = 0x7654U;
    /* Prepare the HSE descriptor */
    Hse_SrvDescriptor.srvId                                      = HSE_SRV_ID_CMAC_WITH_COUNTER;
    Hse_SrvDescriptor.hseSrv.cmacWithCounterReq.authDir          = HSE_AUTH_DIR_GENERATE;
    Hse_SrvDescriptor.hseSrv.cmacWithCounterReq.keyHandle        = APP_CMAC_KEY_HANDLE;
    Hse_SrvDescriptor.hseSrv.cmacWithCounterReq.counterIdx       = APP_CMAC_GENERATE_CTR_IDX;
    Hse_SrvDescriptor.hseSrv.cmacWithCounterReq.inputBitLength   = HSE_BYTES_TO_BITS(APP_CMAC_DATA_SIZE);
    Hse_SrvDescriptor.hseSrv.cmacWithCounterReq.pInput           = HSE_PTR_TO_HOST_ADDR(App_au8CmacData);
    Hse_SrvDescriptor.hseSrv.cmacWithCounterReq.tagBitLength     = HSE_BYTES_TO_BITS(APP_CMAC_TAG_SIZE);
    Hse_SrvDescriptor.hseSrv.cmacWithCounterReq.pTag             = HSE_PTR_TO_HOST_ADDR(App_au8CmacGenerateTagBuffer);
    Hse_SrvDescriptor.hseSrv.cmacWithCounterReq.pVolatileCounter = HSE_PTR_TO_HOST_ADDR(&App_u32CmacGenerateCtrVolatileVal);
    /* Send the HSE descriptor to the firmware for synchronous processing */
    RetVal = Crypto_KeyElementGet(CryptoConf_CryptoKey_Crypto_Key_FEEDDE5C,      /* uint32  keyId           */
                                  0xFEEDDE5CU,                                   /* uint32  keyElementId    */
                                  (uint8*)&Hse_SrvDescriptor,                    /* uint8*  resultPtr       */
                                  (uint32*)&HseResponse                          /* uint32* resultLengthPtr */
                                 );
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);
    App_SetSuccessStatus(Util_Memcmp(App_au8CmacGenerateTagBuffer,(uint8*)App_au8ExpectedCmacGenerateTag, APP_CMAC_TAG_SIZE));
    App_SetSuccessStatus(App_u32CmacGenerateCtrVolatileVal == 0U);

    /* =============================================================================================================================== */
    /*    Verify CMAC with counter                                                                                                     */
    /* =============================================================================================================================== */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Configure the monotonic counter                                                                                              */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /* Prepare the HSE descriptor */
    Hse_SrvDescriptor.srvId                                = HSE_SRV_ID_CONFIG_COUNTER;
    Hse_SrvDescriptor.hseSrv.configSecCounter.counterIndex = APP_CMAC_VERIFY_CTR_IDX;
    Hse_SrvDescriptor.hseSrv.configSecCounter.RPBitSize    = APP_CMAC_VERIFY_CTR_RP_NUM_BITS;
    /* Send the HSE descriptor to the firmware for synchronous processing */
    RetVal = Crypto_KeyElementGet(CryptoConf_CryptoKey_Crypto_Key_FEEDDE5C,      /* uint32  keyId           */
                                  0xFEEDDE5CU,                                   /* uint32  keyElementId    */
                                  (uint8*)&Hse_SrvDescriptor,                    /* uint8*  resultPtr       */
                                  (uint32*)&HseResponse                          /* uint32* resultLengthPtr */
                                 );
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Increment the monotonic counter                                                                                              */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /* Prepare the HSE descriptor */
    Hse_SrvDescriptor.srvId                             = HSE_SRV_ID_INCREMENT_COUNTER;
    Hse_SrvDescriptor.hseSrv.incCounterReq.counterIndex = APP_CMAC_VERIFY_CTR_IDX;
    Hse_SrvDescriptor.hseSrv.incCounterReq.value        = APP_CMAC_VERIFY_CTR_INCREMENT_VALUE;
    /* Send the HSE descriptor to the firmware for synchronous processing */
    RetVal = Crypto_KeyElementGet(CryptoConf_CryptoKey_Crypto_Key_FEEDDE5C,      /* uint32  keyId           */
                                  0xFEEDDE5CU,                                   /* uint32  keyElementId    */
                                  (uint8*)&Hse_SrvDescriptor,                    /* uint8*  resultPtr       */
                                  (uint32*)&HseResponse                          /* uint32* resultLengthPtr */
                                 );
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Verify CMAC with counter                                                                                                     */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    App_u32CmacVerifyCtrVolatileVal = APP_CMAC_VERIFY_CTR_INIT_VOLATILE_VALUE;
    /* Prepare the HSE descriptor */
    Hse_SrvDescriptor.srvId                                      = HSE_SRV_ID_CMAC_WITH_COUNTER;
    Hse_SrvDescriptor.hseSrv.cmacWithCounterReq.authDir          = HSE_AUTH_DIR_VERIFY;
    Hse_SrvDescriptor.hseSrv.cmacWithCounterReq.keyHandle        = APP_CMAC_KEY_HANDLE;
    Hse_SrvDescriptor.hseSrv.cmacWithCounterReq.counterIdx       = APP_CMAC_VERIFY_CTR_IDX;
    Hse_SrvDescriptor.hseSrv.cmacWithCounterReq.RPOffset         = APP_CMAC_VERIFY_CTR_RP_OFFSET;
    Hse_SrvDescriptor.hseSrv.cmacWithCounterReq.inputBitLength   = HSE_BYTES_TO_BITS(APP_CMAC_DATA_SIZE);
    Hse_SrvDescriptor.hseSrv.cmacWithCounterReq.pInput           = HSE_PTR_TO_HOST_ADDR(App_au8CmacData);
    Hse_SrvDescriptor.hseSrv.cmacWithCounterReq.tagBitLength     = HSE_BYTES_TO_BITS(APP_CMAC_TAG_SIZE);
    Hse_SrvDescriptor.hseSrv.cmacWithCounterReq.pTag             = HSE_PTR_TO_HOST_ADDR(App_au8CmacTagVerify);
    Hse_SrvDescriptor.hseSrv.cmacWithCounterReq.pVolatileCounter = HSE_PTR_TO_HOST_ADDR(&App_u32CmacVerifyCtrVolatileVal);
    /* Send the HSE descriptor to the firmware for synchronous processing */
    RetVal = Crypto_KeyElementGet(CryptoConf_CryptoKey_Crypto_Key_FEEDDE5C,      /* uint32  keyId           */
                                  0xFEEDDE5CU,                                   /* uint32  keyElementId    */
                                  (uint8*)&Hse_SrvDescriptor,                    /* uint8*  resultPtr       */
                                  (uint32*)&HseResponse                          /* uint32* resultLengthPtr */
                                 );
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);
}

/**
* @brief          Exercises the ECC variant Burmester-Desmedt protocol to compute a shared secret
*
* @details        The Burmester-Desmedt Protocol protocol is an extension to the Diffie-Hellman key-agreement protocol.
*                 It allows establishing a shared secret key for a number of participants organized in a "ring".
*                 There are 3 nodes in the ring used by this example, numbered as node 1, node 2 and node 3.
*                 The example goes through all the steps needed to get the shared secret on node 1, assuming that the public keys and the second public keys
*                 of the other 2 nodes are known.
*
*/
static void App_BurmesterDesmedtKeyGenerationExample(void)
{
    Std_ReturnType         RetVal;
    /* Variable storing the response from Hse after feeding a Hse descriptor using Crypto_KeyElementGet() API with a keyElementId set to 0xFEEDDE5C */
    hseSrvResponse_t       HseResponse;

    /* =============================================================================================================================== */
    /*    Preparing the setup                                                                                                          */
    /* =============================================================================================================================== */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Load P224 curve into HSE                                                                                                     */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /* Prepare the HSE descriptor */
    Hse_SrvDescriptor.srvId                             = HSE_SRV_ID_LOAD_ECC_CURVE;
    Hse_SrvDescriptor.hseSrv.loadEccCurveReq.eccCurveId = HSE_EC_USER_CURVE1;
    Hse_SrvDescriptor.hseSrv.loadEccCurveReq.nBitLen    = APP_CURVE_P224_BIT_SIZE;
    Hse_SrvDescriptor.hseSrv.loadEccCurveReq.pBitLen    = APP_CURVE_P224_BIT_SIZE;
    Hse_SrvDescriptor.hseSrv.loadEccCurveReq.pA         = HSE_PTR_TO_HOST_ADDR(App_au8Ecc_SECP224R1_CurveParam_A);
    Hse_SrvDescriptor.hseSrv.loadEccCurveReq.pB         = HSE_PTR_TO_HOST_ADDR(App_au8Ecc_SECP224R1_CurveParam_B);
    Hse_SrvDescriptor.hseSrv.loadEccCurveReq.pG         = HSE_PTR_TO_HOST_ADDR(App_au8Ecc_SECP224R1_Basepoint);
    Hse_SrvDescriptor.hseSrv.loadEccCurveReq.pN         = HSE_PTR_TO_HOST_ADDR(App_au8Ecc_SECP224R1_Order_N);
    Hse_SrvDescriptor.hseSrv.loadEccCurveReq.pP         = HSE_PTR_TO_HOST_ADDR(App_au8Ecc_SECP224R1_Prime_P);
    /* Send the HSE descriptor to the firmware for synchronous processing */
    RetVal = Crypto_KeyElementGet(CryptoConf_CryptoKey_Crypto_Key_FEEDDE5C,      /* uint32  keyId           */
                                  0xFEEDDE5CU,                                   /* uint32  keyElementId    */
                                  (uint8*)&Hse_SrvDescriptor,                    /* uint8*  resultPtr       */
                                  (uint32*)&HseResponse                          /* uint32* resultLengthPtr */
                                 );
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Configure the key that will be used to export encrypted and authenticated the HMAC key containing the shared secret          */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    RetVal = Crypto_KeyElementSet(APP_CRYPTO_EXPORT_HMAC_KEY_ID, KEY_MATERIAL_ELEMENT_ID_U32, App_au8Aes256KeyToExportAndDecryptHMAC, APP_AES256_KEY_SIZE);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Configure the key that will be used to decrypt the shared secret                                                             */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    RetVal = Crypto_KeyElementSet(APP_CRYPTO_DECRYPT_HMAC_KEY_ID, KEY_MATERIAL_ELEMENT_ID_U32, App_au8Aes256KeyToExportAndDecryptHMAC, APP_AES256_KEY_SIZE);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    /* Mark the key as valid, in order to be able to be used in a Crypto_ProcessJob() request */
    RetVal = Crypto_KeySetValid(APP_CRYPTO_DECRYPT_HMAC_KEY_ID);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* =============================================================================================================================== */
    /*    Compute the shared secret on node 1                                                                                          */
    /* =============================================================================================================================== */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Load node 1's device key                                                                                                     */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /* Fill the HSE keyInfo structure first */
    Hse_KeyInfo.keyType             = HSE_KEY_TYPE_ECC_PAIR;
    Hse_KeyInfo.keyFlags            = HSE_KF_USAGE_EXCHANGE | HSE_KF_ACCESS_EXPORTABLE;
    Hse_KeyInfo.specific.eccCurveId = HSE_EC_USER_CURVE1;
    Hse_KeyInfo.keyBitLen           = APP_CURVE_P224_BIT_SIZE;
    Hse_KeyInfo.keyCounter          = 0U;
    /* Prepare the HSE descriptor */
    Hse_SrvDescriptor.srvId                                          = HSE_SRV_ID_IMPORT_KEY;
    Hse_SrvDescriptor.hseSrv.importKeyReq.pKeyInfo                   = HSE_PTR_TO_HOST_ADDR(&Hse_KeyInfo);
    Hse_SrvDescriptor.hseSrv.importKeyReq.pKey[0]                    = HSE_PTR_TO_HOST_ADDR(App_au8Z1);
    Hse_SrvDescriptor.hseSrv.importKeyReq.keyLen[0]                  = HSE_BITS_TO_BYTES(APP_CURVE_P224_BIT_SIZE) * 2U;
    Hse_SrvDescriptor.hseSrv.importKeyReq.pKey[2]                    = HSE_PTR_TO_HOST_ADDR(App_au8A1);
    Hse_SrvDescriptor.hseSrv.importKeyReq.keyLen[2]                  = HSE_BITS_TO_BYTES(APP_CURVE_P224_BIT_SIZE);
    Hse_SrvDescriptor.hseSrv.importKeyReq.targetKeyHandle            = APP_ECC_KEY_PAIR_HANDLE;
    Hse_SrvDescriptor.hseSrv.importKeyReq.cipher.cipherKeyHandle     = HSE_INVALID_KEY_HANDLE;
    Hse_SrvDescriptor.hseSrv.importKeyReq.keyContainer.authKeyHandle = HSE_INVALID_KEY_HANDLE;
    /* Send the HSE descriptor to the firmware for synchronous processing */
    RetVal = Crypto_KeyElementGet(CryptoConf_CryptoKey_Crypto_Key_FEEDDE5C,      /* uint32  keyId           */
                                  0xFEEDDE5CU,                                   /* uint32  keyElementId    */
                                  (uint8*)&Hse_SrvDescriptor,                    /* uint8*  resultPtr       */
                                  (uint32*)&HseResponse                          /* uint32* resultLengthPtr */
                                 );
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Load Z3 (node 1's predecessor's first public key) in slot 0                                                                  */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /* Fill the HSE keyInfo structure first */
    Hse_KeyInfo.keyType             = HSE_KEY_TYPE_ECC_PUB;
    Hse_KeyInfo.keyFlags            = HSE_KF_USAGE_EXCHANGE | HSE_KF_ACCESS_EXPORTABLE;
    Hse_KeyInfo.specific.eccCurveId = HSE_EC_USER_CURVE1;
    Hse_KeyInfo.keyBitLen           = APP_CURVE_P224_BIT_SIZE;
    Hse_KeyInfo.keyCounter          = 0U;
    /* Prepare the HSE descriptor */
    Hse_SrvDescriptor.srvId                                          = HSE_SRV_ID_IMPORT_KEY;
    Hse_SrvDescriptor.hseSrv.importKeyReq.pKeyInfo                   = HSE_PTR_TO_HOST_ADDR(&Hse_KeyInfo);
    Hse_SrvDescriptor.hseSrv.importKeyReq.pKey[0]                    = HSE_PTR_TO_HOST_ADDR(App_au8Z3);
    Hse_SrvDescriptor.hseSrv.importKeyReq.keyLen[0]                  = HSE_BITS_TO_BYTES(APP_CURVE_P224_BIT_SIZE) * 2U;
    Hse_SrvDescriptor.hseSrv.importKeyReq.pKey[2]                    = HSE_PTR_TO_HOST_ADDR(NULL_PTR);
    Hse_SrvDescriptor.hseSrv.importKeyReq.keyLen[2]                  = 0U;
    Hse_SrvDescriptor.hseSrv.importKeyReq.targetKeyHandle            = APP_ECC_KEY_PUB_HANDLE;
    Hse_SrvDescriptor.hseSrv.importKeyReq.cipher.cipherKeyHandle     = HSE_INVALID_KEY_HANDLE;
    Hse_SrvDescriptor.hseSrv.importKeyReq.keyContainer.authKeyHandle = HSE_INVALID_KEY_HANDLE;
    /* Send the HSE descriptor to the firmware for synchronous processing */
    RetVal = Crypto_KeyElementGet(CryptoConf_CryptoKey_Crypto_Key_FEEDDE5C,      /* uint32  keyId           */
                                  0xFEEDDE5CU,                                   /* uint32  keyElementId    */
                                  (uint8*)&Hse_SrvDescriptor,                    /* uint8*  resultPtr       */
                                  (uint32*)&HseResponse                          /* uint32* resultLengthPtr */
                                 );
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Load Z2 (node 1's successor's first public key) in slot 1                                                                    */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /* Fill the HSE keyInfo structure first */
    Hse_KeyInfo.keyType             = HSE_KEY_TYPE_ECC_PUB;
    Hse_KeyInfo.keyFlags            = HSE_KF_USAGE_EXCHANGE | HSE_KF_ACCESS_EXPORTABLE;
    Hse_KeyInfo.specific.eccCurveId = HSE_EC_USER_CURVE1;
    Hse_KeyInfo.keyBitLen           = APP_CURVE_P224_BIT_SIZE;
    Hse_KeyInfo.keyCounter          = 0U;
    /* Prepare the HSE descriptor */
    Hse_SrvDescriptor.srvId                                          = HSE_SRV_ID_IMPORT_KEY;
    Hse_SrvDescriptor.hseSrv.importKeyReq.pKeyInfo                   = HSE_PTR_TO_HOST_ADDR(&Hse_KeyInfo);
    Hse_SrvDescriptor.hseSrv.importKeyReq.pKey[0]                    = HSE_PTR_TO_HOST_ADDR(App_au8Z2);
    Hse_SrvDescriptor.hseSrv.importKeyReq.keyLen[0]                  = HSE_BITS_TO_BYTES(APP_CURVE_P224_BIT_SIZE) * 2U;
    Hse_SrvDescriptor.hseSrv.importKeyReq.pKey[2]                    = HSE_PTR_TO_HOST_ADDR(NULL_PTR);
    Hse_SrvDescriptor.hseSrv.importKeyReq.keyLen[2]                  = 0U;
    Hse_SrvDescriptor.hseSrv.importKeyReq.targetKeyHandle            = APP_ECC_KEY_PUB_HANDLE + 1U;
    Hse_SrvDescriptor.hseSrv.importKeyReq.cipher.cipherKeyHandle     = HSE_INVALID_KEY_HANDLE;
    Hse_SrvDescriptor.hseSrv.importKeyReq.keyContainer.authKeyHandle = HSE_INVALID_KEY_HANDLE;
    /* Send the HSE descriptor to the firmware for synchronous processing */
    RetVal = Crypto_KeyElementGet(CryptoConf_CryptoKey_Crypto_Key_FEEDDE5C,      /* uint32  keyId           */
                                  0xFEEDDE5CU,                                   /* uint32  keyElementId    */
                                  (uint8*)&Hse_SrvDescriptor,                    /* uint8*  resultPtr       */
                                  (uint32*)&HseResponse                          /* uint32* resultLengthPtr */
                                 );
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Perform Burmester-Desmedt step 1                                                                                             */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /* Prepare the HSE descriptor */
    Hse_SrvDescriptor.srvId                                      = HSE_SRV_ID_BURMESTER_DESMEDT;
    Hse_SrvDescriptor.hseSrv.burmesterDesmedtReq.bdStep          = HSE_BD_STEP_COMPUTE_SECOND_PUBLIC_KEY;
    Hse_SrvDescriptor.hseSrv.burmesterDesmedtReq.deviceKeyHandle = APP_ECC_KEY_PAIR_HANDLE;
    Hse_SrvDescriptor.hseSrv.burmesterDesmedtReq.pubKeyHandle    = APP_ECC_KEY_PUB_HANDLE;
    /* Send the HSE descriptor to the firmware for synchronous processing */
    RetVal = Crypto_KeyElementGet(CryptoConf_CryptoKey_Crypto_Key_FEEDDE5C,      /* uint32  keyId           */
                                  0xFEEDDE5CU,                                   /* uint32  keyElementId    */
                                  (uint8*)&Hse_SrvDescriptor,                    /* uint8*  resultPtr       */
                                  (uint32*)&HseResponse                          /* uint32* resultLengthPtr */
                                 );
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Load X2 in slot 3                                                                                                            */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /* Fill the HSE keyInfo structure first */
    Hse_KeyInfo.keyType             = HSE_KEY_TYPE_ECC_PUB;
    Hse_KeyInfo.keyFlags            = HSE_KF_USAGE_EXCHANGE | HSE_KF_ACCESS_EXPORTABLE;
    Hse_KeyInfo.specific.eccCurveId = HSE_EC_USER_CURVE1;
    Hse_KeyInfo.keyBitLen           = APP_CURVE_P224_BIT_SIZE;
    Hse_KeyInfo.keyCounter          = 0U;
    /* Prepare the HSE descriptor */
    Hse_SrvDescriptor.srvId                                          = HSE_SRV_ID_IMPORT_KEY;
    Hse_SrvDescriptor.hseSrv.importKeyReq.pKeyInfo                   = HSE_PTR_TO_HOST_ADDR(&Hse_KeyInfo);
    Hse_SrvDescriptor.hseSrv.importKeyReq.pKey[0]                    = HSE_PTR_TO_HOST_ADDR(App_au8X2);
    Hse_SrvDescriptor.hseSrv.importKeyReq.keyLen[0]                  = HSE_BITS_TO_BYTES(APP_CURVE_P224_BIT_SIZE) * 2U;
    Hse_SrvDescriptor.hseSrv.importKeyReq.pKey[2]                    = HSE_PTR_TO_HOST_ADDR(NULL_PTR);
    Hse_SrvDescriptor.hseSrv.importKeyReq.keyLen[2]                  = 0U;
    Hse_SrvDescriptor.hseSrv.importKeyReq.targetKeyHandle            = APP_ECC_KEY_PUB_HANDLE + 3U;
    Hse_SrvDescriptor.hseSrv.importKeyReq.cipher.cipherKeyHandle     = HSE_INVALID_KEY_HANDLE;
    Hse_SrvDescriptor.hseSrv.importKeyReq.keyContainer.authKeyHandle = HSE_INVALID_KEY_HANDLE;
    /* Send the HSE descriptor to the firmware for synchronous processing */
    RetVal = Crypto_KeyElementGet(CryptoConf_CryptoKey_Crypto_Key_FEEDDE5C,      /* uint32  keyId           */
                                  0xFEEDDE5CU,                                   /* uint32  keyElementId    */
                                  (uint8*)&Hse_SrvDescriptor,                    /* uint8*  resultPtr       */
                                  (uint32*)&HseResponse                          /* uint32* resultLengthPtr */
                                 );
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Perform Burmester-Desmedt step 2                                                                                             */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /* Prepare the HSE descriptor */
    Hse_SrvDescriptor.srvId                                            = HSE_SRV_ID_BURMESTER_DESMEDT;
    Hse_SrvDescriptor.hseSrv.burmesterDesmedtReq.bdStep                = HSE_BD_STEP_COMPUTE_SHARED_SECRET;
    Hse_SrvDescriptor.hseSrv.burmesterDesmedtReq.deviceKeyHandle       = APP_ECC_KEY_PAIR_HANDLE;
    Hse_SrvDescriptor.hseSrv.burmesterDesmedtReq.pubKeyHandle          = APP_ECC_KEY_PUB_HANDLE;
    Hse_SrvDescriptor.hseSrv.burmesterDesmedtReq.sharedSecretKeyHandle = APP_SHARED_SECRET_HANDLE;
    Hse_SrvDescriptor.hseSrv.burmesterDesmedtReq.numParticipants       = 3U;
    /* Send the HSE descriptor to the firmware for synchronous processing */
    RetVal = Crypto_KeyElementGet(CryptoConf_CryptoKey_Crypto_Key_FEEDDE5C,      /* uint32  keyId           */
                                  0xFEEDDE5CU,                                   /* uint32  keyElementId    */
                                  (uint8*)&Hse_SrvDescriptor,                    /* uint8*  resultPtr       */
                                  (uint32*)&HseResponse                          /* uint32* resultLengthPtr */
                                 );
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Check that the length of the generated shared secret is correct                                                              */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /* Prepare the HSE descriptor */
    Hse_SrvDescriptor.srvId                          = HSE_SRV_ID_GET_KEY_INFO;
    Hse_SrvDescriptor.hseSrv.getKeyInfoReq.keyHandle = APP_SHARED_SECRET_HANDLE;
    Hse_SrvDescriptor.hseSrv.getKeyInfoReq.pKeyInfo  = HSE_PTR_TO_HOST_ADDR(&Hse_KeyInfo);
    /* Send the HSE descriptor to the firmware for synchronous processing */
    RetVal = Crypto_KeyElementGet(CryptoConf_CryptoKey_Crypto_Key_FEEDDE5C,      /* uint32  keyId           */
                                  0xFEEDDE5CU,                                   /* uint32  keyElementId    */
                                  (uint8*)&Hse_SrvDescriptor,                    /* uint8*  resultPtr       */
                                  (uint32*)&HseResponse                          /* uint32* resultLengthPtr */
                                 );
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);
    App_SetSuccessStatus(Hse_KeyInfo.keyBitLen == (2 * APP_CURVE_P224_BIT_SIZE));

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Copy the shared secret to an exportable HMAC slot                                                                            */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /* Fill the HSE keyInfo structure first */
    Hse_KeyInfo.keyType             = HSE_KEY_TYPE_HMAC;
    Hse_KeyInfo.keyFlags            = HSE_KF_USAGE_SIGN | HSE_KF_USAGE_VERIFY | HSE_KF_ACCESS_EXPORTABLE;
    Hse_KeyInfo.keyBitLen           = 448;
    /* Prepare the HSE descriptor */
    Hse_SrvDescriptor.srvId                                      = HSE_SRV_ID_KEY_DERIVE_COPY;
    Hse_SrvDescriptor.hseSrv.keyDeriveCopyKeyReq.keyHandle       = APP_SHARED_SECRET_HANDLE;
    Hse_SrvDescriptor.hseSrv.keyDeriveCopyKeyReq.startOffset     = 0U;
    Hse_SrvDescriptor.hseSrv.keyDeriveCopyKeyReq.targetKeyHandle = APP_HMAC_KEY_HANDLE;
    Hse_SrvDescriptor.hseSrv.keyDeriveCopyKeyReq.keyInfo         = Hse_KeyInfo;
    /* Send the HSE descriptor to the firmware for synchronous processing */
    RetVal = Crypto_KeyElementGet(CryptoConf_CryptoKey_Crypto_Key_FEEDDE5C,      /* uint32  keyId           */
                                  0xFEEDDE5CU,                                   /* uint32  keyElementId    */
                                  (uint8*)&Hse_SrvDescriptor,                    /* uint8*  resultPtr       */
                                  (uint32*)&HseResponse                          /* uint32* resultLengthPtr */
                                 );
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Export the shared secret, encrypted and authenticated                                                                        */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    RetVal = Crypto_KeyElementGet(APP_CRYPTO_HMAC_KEY_ID, KEY_MATERIAL_ELEMENT_ID_U32, App_au8EncryptedSharedSecret, &App_u32EncryptedSharedSecretSize);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(App_u32EncryptedSharedSecretSize == APP_ENCRYPTED_SHARED_SECRET_SIZE);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Decrypt the shared secret                                                                                                    */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    RetVal = Crypto_ProcessJob(APP_CDO_ID, &App_JobDecryptSharedSecret);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    /* Compare the decryted shared secret with the expected result */
    App_SetSuccessStatus(Util_Memcmp(App_au8PlainSharedSecret,(uint8*)App_au8ExpectedSharedSecret, APP_PLAIN_SHARED_SECRET_SIZE));

    /* The shared secret can be computed in a similar way on the other 2 nodes (node 2 and node 3)
       Ignore for now compiler warnings for not used variables */
    (void)App_au8X1;
    (void)App_au8X3;
    (void)App_au8A2;
    (void)App_au8A3;
}

/* Utility functions */
static boolean Util_Memcmp
(
    uint8* pSource,
    uint8* pDest,
    uint32 u32Size
)
{
    boolean bResult      = (boolean)TRUE;
    uint32  u32InputSize = u32Size;

    while (0U < u32InputSize--)
    {
        if (*pSource == *pDest)
        {
            pSource++;
            pDest++;
        }
        else
        {
            bResult = (boolean)FALSE;
            break;
        }
    }
    return bResult;
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
int main(void)
{
    Std_ReturnType         RetVal;

    /* =============================================================================================================================== */
    /*    Initialization                                                                                                               */
    /* =============================================================================================================================== */
    /* Initialize OsIf component. It will be used to measure timeouts while waiting for HSE to respond to synchronous requests */
    OsIf_Init(NULL_PTR);
    /* Initialize Crypto driver */
    Crypto_Init(NULL_PTR);

    /* Format HSE key catalogs (RAM + NVM) */
    RetVal = Crypto_Exts_FormatKeyCatalogs();
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* =============================================================================================================================== */
    /*    Run the Cmac generate/verify with counter example                                                                            */
    /* =============================================================================================================================== */
    App_CmacWithCounterExample();

    /* =============================================================================================================================== */
    /*    Run the Burmester-Desmedt key generation example                                                                             */
    /* =============================================================================================================================== */
    App_BurmesterDesmedtKeyGenerationExample();

    /* =============================================================================================================================== */
    /*    Finish application execution, signaling the status                                                                           */
    /* =============================================================================================================================== */
    Exit_Example(App_GetSuccessStatus());
    return (0U);
}


#ifdef __cplusplus
}
#endif

/** @} */
