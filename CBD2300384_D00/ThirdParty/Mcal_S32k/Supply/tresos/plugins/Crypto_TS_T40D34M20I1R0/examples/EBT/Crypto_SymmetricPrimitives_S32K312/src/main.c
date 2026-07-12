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
#define App_SetSuccessStatus(value)         (u32NumFailedOperations += ((value) ? 0U : 1U))
#define App_GetSuccessStatus()              (0U == u32NumFailedOperations)

/* Identifier of the KeyElement that contains the key material */
#define KEY_MATERIAL_ELEMENT_ID_U32         ((uint32)1U)

/* Take the generated value of the Crypto Driver Object Id used for symmetric primitives */
#define APP_SYMMETRIC_CDO_ID                   (CryptoConf_CryptoDriverObject_CDO_Symmetric)

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL MACROS related to the 'AES128 encrypt/decrypt' part of the example                     */
/* ----------------------------------------------------------------------------------------------- */
/* Take the generated value of the AES128 Encrypt/Decrypt key id from the configuration file */
#define APP_AES128_KEY_ID                   (CryptoConf_CryptoKey_Crypto_Key_AES128)
/* Size in bytes of the AES128 Encrypt/Decrypt key */
#define APP_AES128_KEY_SIZE                 (16U)
/* Size in bytes of the 2 plain texts that will be used to test encryption */
#define APP_AES128_ECB_PLAIN_TEXT_SIZE_1    (16U)
#define APP_AES128_ECB_PLAIN_TEXT_SIZE_2    (64U)
/* Size in bytes of the 2 cipher texts that will be used to test decryption */
#define APP_AES128_ECB_CIPHER_TEXT_SIZE_1   (16U)
#define APP_AES128_ECB_CIPHER_TEXT_SIZE_2   (64U)
/* Size in bytes of the buffer where the Crypto driver will place the result of the encrypt/decrypt operations
   In this sample app, should be large enough to fit encryption/decryption of a 16 byte and respectively 64 bytes blocks of data */
#define APP_AES128_ECB_RESULT_SIZE          (64U)

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL MACROS related to the 'CMAC generate/verify' part of the example                       */
/* ----------------------------------------------------------------------------------------------- */
/* Take the generated value of the CMAC generate/verify key id from the configuration file */
#define APP_CMAC_KEY_ID                      (CryptoConf_CryptoKey_Crypto_Key_AES128)
/* Size in bytes of the CMAC generate/verify key */
#define APP_CMAC_KEY_SIZE                    (16U)
/* Size of the CMAC resulted tag */
#define APP_CMAC_TAG_SIZE                    (16U)
/* Size of the data used to compute CMAC over */
#define APP_CMAC_DATA_SIZE_1                 (16U)
#define APP_CMAC_DATA_SIZE_2                 (64U)

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL MACROS related to the 'Hash' part of the example                                       */
/* ----------------------------------------------------------------------------------------------- */
/* Size of the data used to compute HASH over */
#define APP_HASH_DATA_SIZE                   (64U)
/* Size of the SHA-1 digest */
#define APP_HASH_SHA_1_DIGEST_SIZE              (20U)
/* Size of the SHA-256 digest */
#define APP_HASH_SHA_256_DIGEST_SIZE            (32U)
/* Max size of the digests in this test application */
#define APP_MAX_DIGEST_SIZE                     (32U)

/*==================================================================================================
*                                      LOCAL CONSTANTS
===================================================================================================*/
/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL CONSTANTS related to the 'AES128 encrypt/decrypt' part of the example                  */
/* ----------------------------------------------------------------------------------------------- */
/* ---- Crypto job sub-structures for AES 128 ECB Encrypt ---------------------------------------- */
static const Crypto_PrimitiveInfoType App_Aes128EcbEncryptPrimitiveInfo =
{
    APP_AES128_ECB_RESULT_SIZE,         /* resultLength                - Contains the result length in bytes. */
    CRYPTO_ENCRYPT,                     /* service                     - Contains the enum of the used service, e.g. Encrypt */
    {
        CRYPTO_ALGOFAM_AES,             /* family                      - The family of the algorithm */
        CRYPTO_ALGOFAM_NOT_SET,         /* secondaryFamily             - The secondary family of the algorithm  */
        APP_AES128_KEY_SIZE,            /* keyLength                   - The key length in bits to be used with that algorithm */
        CRYPTO_ALGOMODE_ECB             /* mode                        - The operation mode to be used with that algorithm */
    }
};

static const Crypto_JobInfoType App_JobAes128EcbEncryptInfo =
{
    0U,                                 /* jobId                       - The identifier of the job */
    0U                                  /* jobPriority                 - Specifies the importance of the job (the higher, the more important) */
};

/* ---- Crypto job sub-structures for AES 128 ECB Decrypt ---------------------------------------- */
static const Crypto_PrimitiveInfoType App_Aes128EcbDecryptPrimitiveInfo =
{
    APP_AES128_ECB_RESULT_SIZE,         /* resultLength                - Contains the result length in bytes. */
    CRYPTO_DECRYPT,                     /* service                     - Contains the enum of the used service, e.g. Encrypt */
    {
        CRYPTO_ALGOFAM_AES,             /* family                      - The family of the algorithm */
        CRYPTO_ALGOFAM_NOT_SET,         /* secondaryFamily             - The secondary family of the algorithm  */
        APP_AES128_KEY_SIZE,            /* keyLength                   - The key length in bits to be used with that algorithm */
        CRYPTO_ALGOMODE_ECB             /* mode                        - The operation mode to be used with that algorithm */
    }
};

static const Crypto_JobInfoType App_JobAes128EcbDecryptInfo =
{
    0U,                                 /* jobId                       - The identifier of the job */
    0U                                  /* jobPriority                 - Specifies the importance of the job (the higher, the more important) */
};

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL CONSTANTS related to the 'CMAC generate/verify' part of the example                    */
/* ----------------------------------------------------------------------------------------------- */
/* ---- Crypto job sub-structures for CMAC Generate ---------------------------------------------- */
static const Crypto_PrimitiveInfoType App_CmacGeneratePrimitiveInfo =
{
    APP_CMAC_TAG_SIZE,                   /* resultLength                - Contains the result length in bytes. */
    CRYPTO_MACGENERATE,                  /* service                     - Contains the enum of the used service, e.g. Encrypt */
    {
        CRYPTO_ALGOFAM_AES,              /* family                      - The family of the algorithm */
        CRYPTO_ALGOFAM_NOT_SET,          /* secondaryFamily             - The secondary family of the algorithm  */
        APP_CMAC_KEY_SIZE,               /* keyLength                   - The key length in bits to be used with that algorithm */
        CRYPTO_ALGOMODE_CMAC             /* mode                        - The operation mode to be used with that algorithm */
    }
};

static const Crypto_JobInfoType App_JobCmacGenerateInfo =
{
    0U,                                  /* jobId                      - The family of the algorithm */
    0U                                   /* jobPriority                - Specifies the importance of the job (the higher, the more important). */
};

/* ---- Crypto job sub-structures for CMAC Verify ------------------------------------------------ */
static const Crypto_PrimitiveInfoType App_CmacVerifyPrimitiveInfo =
{
    APP_CMAC_TAG_SIZE,                   /* resultLength                - Contains the result length in bytes. */
    CRYPTO_MACVERIFY,                    /* service                     - Contains the enum of the used service, e.g. Encrypt */
    {
        CRYPTO_ALGOFAM_AES,              /* family                      - The family of the algorithm */
        CRYPTO_ALGOFAM_NOT_SET,          /* secondaryFamily             - The secondary family of the algorithm  */
        APP_CMAC_KEY_SIZE,               /* keyLength                   - The key length in bits to be used with that algorithm */
        CRYPTO_ALGOMODE_CMAC             /* mode                        - The operation mode to be used with that algorithm */
    }
};

static const Crypto_JobInfoType App_JobCmacVerifyInfo =
{
    0U,                                  /* jobId                      - The family of the algorithm */
    0U                                   /* jobPriority                - Specifies the importance of the job (the higher, the more important). */
};

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL CONSTANTS related to the 'HASH' part of the example                                    */
/* ----------------------------------------------------------------------------------------------- */
/* ---- Crypto job sub-structures for HASH SHA-1 ------------------------------------------------- */
static const Crypto_PrimitiveInfoType App_HashSha1PrimitiveInfo =
{
    APP_HASH_SHA_1_DIGEST_SIZE,             /* resultLength                - Contains the result length in bytes. */
    CRYPTO_HASH,                            /* service                     - Contains the enum of the used service, e.g. Encrypt */
    {
        CRYPTO_ALGOFAM_SHA1,                /* family                      - The family of the algorithm */
        CRYPTO_ALGOFAM_NOT_SET,             /* secondaryFamily             - The secondary family of the algorithm  */
        0,                                  /* keyLength                   - The key length in bits to be used with that algorithm */
        CRYPTO_ALGOMODE_NOT_SET             /* mode                        - The operation mode to be used with that algorithm */
    }
};

static const Crypto_JobInfoType App_JobHashSha1Info =
{
    0U,                                     /* jobId                      - The family of the algorithm */
    0U                                      /* jobPriority                - Specifies the importance of the job (the higher, the more important). */
};

/* ---- Crypto job sub-structures for HASH SHA2-256 ---------------------------------------------- */
static const Crypto_PrimitiveInfoType App_HashSha256PrimitiveInfo =
{
    APP_HASH_SHA_256_DIGEST_SIZE,           /* resultLength                - Contains the result length in bytes. */
    CRYPTO_HASH,                            /* service                     - Contains the enum of the used service, e.g. Encrypt */
    {
        CRYPTO_ALGOFAM_SHA2_256,            /* family                      - The family of the algorithm */
        CRYPTO_ALGOFAM_NOT_SET,             /* secondaryFamily             - The secondary family of the algorithm  */
        0,                                  /* keyLength                   - The key length in bits to be used with that algorithm */
        CRYPTO_ALGOMODE_NOT_SET             /* mode                        - The operation mode to be used with that algorithm */
    }
};

static const Crypto_JobInfoType App_JobHashSha256Info =
{
    0U,                                     /* jobId                      - The family of the algorithm */
    0U                                      /* jobPriority                - Specifies the importance of the job (the higher, the more important). */
};

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
/* Variable storing the number of failed operations in the example */
static uint32 u32NumFailedOperations = 0U;

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL VARIABLES related to the 'AES128 encrypt/decrypt' part of the example                  */
/* ----------------------------------------------------------------------------------------------- */
/* --- The following RAM variables need to be put in non-cacheable memory in order to be correctly seen by HSE --- */
#define CRYPTO_START_SEC_VAR_INIT_8_NO_CACHEABLE
#include "Crypto_MemMap.h"

static uint8 App_au8Aes128EcbKey_1[APP_AES128_KEY_SIZE] __attribute__((aligned)) =
{
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
};

static uint8 App_au8Aes128EcbKey_2[APP_AES128_KEY_SIZE] __attribute__((aligned)) =
{
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f
};

static uint8 App_au8Aes128EcbPlaintext_1[APP_AES128_ECB_PLAIN_TEXT_SIZE_1] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

static uint8 App_au8Aes128EcbPlaintext_2[APP_AES128_ECB_PLAIN_TEXT_SIZE_2] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
};

static uint8 App_au8Aes128EcbCiphertext_1[APP_AES128_ECB_CIPHER_TEXT_SIZE_1] =
{
    0x9c, 0x54, 0xd5, 0x71, 0x70, 0x2c, 0xfa, 0x0f, 0x03, 0xf3, 0x62, 0x15, 0x67, 0x6b, 0xab, 0x78
};

static uint8 App_au8Aes128EcbCiphertext_2[APP_AES128_ECB_CIPHER_TEXT_SIZE_2] =
{
    0xdb, 0x7c, 0xe6, 0x7a, 0xf1, 0x3d, 0xe5, 0x7a, 0x95, 0xd9, 0x22, 0xe5, 0x32, 0x5a, 0xbf, 0x13,
    0x77, 0x10, 0x98, 0xa6, 0xb7, 0x8c, 0xb4, 0x5c, 0x02, 0x9c, 0xf1, 0xc0, 0xdd, 0xee, 0x0f, 0x1b,
    0xcc, 0x0d, 0xd1, 0x5b, 0x23, 0xe6, 0x82, 0x9e, 0x05, 0xf6, 0xe4, 0x17, 0xf4, 0x64, 0xb6, 0x99,
    0xa7, 0xad, 0xee, 0xae, 0xcc, 0xa5, 0x48, 0x89, 0xb8, 0x62, 0x1b, 0xec, 0x18, 0x52, 0x70, 0x44
};

#define CRYPTO_STOP_SEC_VAR_INIT_8_NO_CACHEABLE
#include "Crypto_MemMap.h"


#define CRYPTO_START_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Crypto_MemMap.h"

static uint8  App_au8Aes128EcbResult[APP_AES128_ECB_RESULT_SIZE];

#define CRYPTO_STOP_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Crypto_MemMap.h"


#define CRYPTO_START_SEC_VAR_CLEARED_32_NO_CACHEABLE
#include "Crypto_MemMap.h"

static uint32 App_u32Aes128EcbResultSize;

#define CRYPTO_STOP_SEC_VAR_CLEARED_32_NO_CACHEABLE
#include "Crypto_MemMap.h"

/* ---- Crypto job sub-structures for AES 128 ECB Encrypt ---------------------------------------- */
static Crypto_JobPrimitiveInfoType App_JobAes128EcbEncryptPrimitiveInfo =
{
    0U,                                  /* callbackId                 - Identifier of the callback function, to be called, if the configured service finished. */
    &App_Aes128EcbEncryptPrimitiveInfo,  /* primitiveInfo              - Pointer to a structure containing further configuration of the crypto primitives */
    0U,                                  /* cryIfKeyId                 - Identifier of the CryIf key. */
    CRYPTO_PROCESSING_SYNC,              /* processingType             - Determines the synchronous or asynchronous behavior. */
    (boolean)FALSE                       /* callbackUpdateNotification - Indicates, whether the callback function shall be called, if the UPDATE operation has finished. */
};

/* ---- Crypto job sub-structures for AES 128 ECB Decrypt ---------------------------------------- */
static Crypto_JobPrimitiveInfoType App_JobAes128EcbDecryptPrimitiveInfo =
{
    0U,                                  /* callbackId                 - Identifier of the callback function, to be called, if the configured service finished. */
    &App_Aes128EcbDecryptPrimitiveInfo,  /* primitiveInfo              - Pointer to a structure containing further configuration of the crypto primitives */
    0U,                                  /* cryIfKeyId                 - Identifier of the CryIf key. */
    CRYPTO_PROCESSING_SYNC,              /* processingType             - Determines the synchronous or asynchronous behavior. */
    (boolean)FALSE                       /* callbackUpdateNotification - Indicates, whether the callback function shall be called, if the UPDATE operation has finished. */
};

/* --- Structure of the job to be passed to Crypto driver, requesting Aes128 ECB Encrypt --------- */
static Crypto_JobType App_JobAes128EcbEncrypt =
{
    1U,                                     /* jobId                      - Identifier for the job structure */
    CRYPTO_JOBSTATE_IDLE,                   /* jobState                   - Determines the current job state */
    {
        NULL_PTR,                           /* inputPtr                   - Pointer to the input data. */
        0,                                  /* inputLength                - Contains the input length in bytes. */
        NULL_PTR,                           /* secondaryInputPtr          - Pointer to the secondary input data (for MacVerify, SignatureVerify). */
        0U,                                 /* secondaryInputLength       - Contains the secondary input length in bytes. */
        NULL_PTR,                           /* tertiaryInputPtr           - Pointer to the tertiary input data (for MacVerify, SignatureVerify). */
        0U,                                 /* tertiaryInputLength        - Contains the tertiary input length in bytes. */
        NULL_PTR,                           /* outputPtr                  - Pointer to the output data. */
        NULL_PTR,                           /* outputLengthPtr            - Holds a pointer to a memory location containing the output length in bytes. */
        NULL_PTR,                           /* secondaryOutputPtr         - Pointer to the secondary output data. */
        NULL_PTR,                           /* secondaryOutputLengthPtr   - Holds a pointer to a memory location containing the secondary output length in bytes. */
        0U,                                 /* input64                    - Versatile input parameter */
        NULL_PTR,                           /* verifyPtr                  - Output pointer to a memory location holding a Crypto_VerifyResultType */
        NULL_PTR,                           /* output64Ptr                - Output pointer to a memory location holding an uint64. */
        CRYPTO_OPERATIONMODE_SINGLECALL,    /* mode                       - Indicator of the mode(s)/operation(s) to be performed */
        0U,                                 /* cryIfKeyId                 - Holds the CryIf key id for key operation services. */
        0U,                                 /* targetCryIfKeyId           - Holds the target CryIf key id for key operation services. */
    },
    &App_JobAes128EcbEncryptPrimitiveInfo,  /* jobPrimitiveInfo           - Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    &App_JobAes128EcbEncryptInfo,           /* jobInfo                    - Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    NULL_PTR,                               /* jobRedirectionInfoRef      - Pointer to a structure containing further information on the usage of keys as input and output for jobs. */
    0U                                      /* cryptoKeyId                - Identifier of the Crypto Driver key. The identifier shall be written by the Crypto Interface */
};

/* --- Structure of the job to be passed to Crypto driver, requesting Aes128 ECB Encrypt --------- */
static Crypto_JobType App_JobAes128EcbDecrypt =
{
    2U,                                     /* jobId                      - Identifier for the job structure */
    CRYPTO_JOBSTATE_IDLE,                   /* jobState                   - Determines the current job state */
    {
        NULL_PTR,                           /* inputPtr                   - Pointer to the input data. */
        0,                                  /* inputLength                - Contains the input length in bytes. */
        NULL_PTR,                           /* secondaryInputPtr          - Pointer to the secondary input data (for MacVerify, SignatureVerify). */
        0U,                                 /* secondaryInputLength       - Contains the secondary input length in bytes. */
        NULL_PTR,                           /* tertiaryInputPtr           - Pointer to the tertiary input data (for MacVerify, SignatureVerify). */
        0U,                                 /* tertiaryInputLength        - Contains the tertiary input length in bytes. */
        NULL_PTR,                           /* outputPtr                  - Pointer to the output data. */
        NULL_PTR,                           /* outputLengthPtr            - Holds a pointer to a memory location containing the output length in bytes. */
        NULL_PTR,                           /* secondaryOutputPtr         - Pointer to the secondary output data. */
        NULL_PTR,                           /* secondaryOutputLengthPtr   - Holds a pointer to a memory location containing the secondary output length in bytes. */
        0U,                                 /* input64                    - Versatile input parameter */
        NULL_PTR,                           /* verifyPtr                  - Output pointer to a memory location holding a Crypto_VerifyResultType */
        NULL_PTR,                           /* output64Ptr                - Output pointer to a memory location holding an uint64. */
        CRYPTO_OPERATIONMODE_SINGLECALL,    /* mode                       - Indicator of the mode(s)/operation(s) to be performed */
        0U,                                 /* cryIfKeyId                 - Holds the CryIf key id for key operation services. */
        0U,                                 /* targetCryIfKeyId           - Holds the target CryIf key id for key operation services. */
    },
    &App_JobAes128EcbDecryptPrimitiveInfo,  /* jobPrimitiveInfo           - Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    &App_JobAes128EcbDecryptInfo,           /* jobInfo                    - Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    NULL_PTR,                               /* jobRedirectionInfoRef      - Pointer to a structure containing further information on the usage of keys as input and output for jobs. */
    0U                                      /* cryptoKeyId                - Identifier of the Crypto Driver key. The identifier shall be written by the Crypto Interface */
};

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL VARIABLES related to the 'CMAC generate/verify' part of the example                    */
/* ----------------------------------------------------------------------------------------------- */
/* --- The following RAM variables need to be put in non-cacheable memory in order to be correctly seen by HSE --- */
#define CRYPTO_START_SEC_VAR_INIT_8_NO_CACHEABLE
#include "Crypto_MemMap.h"

static uint8 App_au8CmacKey_1[APP_CMAC_KEY_SIZE] __attribute__((aligned)) =
{
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
};

static uint8 App_au8CmacKey_2[APP_CMAC_KEY_SIZE] __attribute__((aligned)) =
{
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f
};

static uint8 App_au8CmacData_1[APP_CMAC_DATA_SIZE_1] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

static uint8 App_au8CmacData_2[APP_CMAC_DATA_SIZE_2] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
};

static uint8 App_au8CmacTag_1[APP_CMAC_TAG_SIZE] =
{
    0x94, 0xb0, 0xe7, 0xf3, 0x47, 0xfc, 0x6e, 0xda, 0x53, 0x8d, 0x27, 0x0c, 0xa8, 0x3b, 0x5c, 0x74
};

static uint8 App_au8CmacTag_2[APP_CMAC_TAG_SIZE] =
{
    0xE6, 0x77, 0x37, 0xDA, 0xD4, 0x9, 0x25, 0xAF, 0x1C, 0x2C, 0xF9, 0x3A, 0xDA, 0x38, 0xEF, 0x1F
};

#define CRYPTO_STOP_SEC_VAR_INIT_8_NO_CACHEABLE
#include "Crypto_MemMap.h"


#define CRYPTO_START_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Crypto_MemMap.h"

static uint8  App_au8CmacResult[APP_CMAC_TAG_SIZE];

#define CRYPTO_STOP_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Crypto_MemMap.h"


#define CRYPTO_START_SEC_VAR_INIT_32_NO_CACHEABLE
#include "Crypto_MemMap.h"

static uint32 App_u32CmacResultSize = APP_CMAC_TAG_SIZE;

#define CRYPTO_STOP_SEC_VAR_INIT_32_NO_CACHEABLE
#include "Crypto_MemMap.h"

/* ---- Crypto job sub-structures for CMAC Generate ---------------------------------------------- */
static Crypto_JobPrimitiveInfoType App_JobCmacGeneratePrimitiveInfo =
{
    0U,                                  /* callbackId                 - Identifier of the callback function, to be called, if the configured service finished. */
    &App_CmacGeneratePrimitiveInfo,      /* primitiveInfo              - Pointer to a structure containing further configuration of the crypto primitives */
    0U,                                  /* cryIfKeyId                 - Identifier of the CryIf key. */
    CRYPTO_PROCESSING_SYNC,              /* processingType             - Determines the synchronous or asynchronous behavior. */
    (boolean)FALSE                       /* callbackUpdateNotification - Indicates, whether the callback function shall be called, if the UPDATE operation has finished. */
};

/* ---- Crypto job sub-structures for CMAC Verify ------------------------------------------------ */
static Crypto_JobPrimitiveInfoType App_JobCmacVerifyPrimitiveInfo =
{
    0U,                                  /* callbackId                 - Identifier of the callback function, to be called, if the configured service finished. */
    &App_CmacVerifyPrimitiveInfo,        /* primitiveInfo              - Pointer to a structure containing further configuration of the crypto primitives */
    0U,                                  /* cryIfKeyId                 - Identifier of the CryIf key. */
    CRYPTO_PROCESSING_SYNC,              /* processingType             - Determines the synchronous or asynchronous behavior. */
    (boolean)FALSE                       /* callbackUpdateNotification - Indicates, whether the callback function shall be called, if the UPDATE operation has finished. */
};


#define CRYPTO_START_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE
#include "Crypto_MemMap.h"

/* --- Structure of the job to be passed to Crypto driver, requesting CMAC Generate -------------- */
static Crypto_JobType App_JobCmacGenerate =
{
    1U,                                     /* jobId                      - Identifier for the job structure */
    CRYPTO_JOBSTATE_IDLE,                   /* jobState                   - Determines the current job state */
    {
        NULL_PTR,                           /* inputPtr                   - Pointer to the input data. */
        0,                                  /* inputLength                - Contains the input length in bytes. */
        NULL_PTR,                           /* secondaryInputPtr          - Pointer to the secondary input data (for MacVerify, SignatureVerify). */
        0U,                                 /* secondaryInputLength       - Contains the secondary input length in bytes. */
        NULL_PTR,                           /* tertiaryInputPtr           - Pointer to the tertiary input data (for MacVerify, SignatureVerify). */
        0U,                                 /* tertiaryInputLength        - Contains the tertiary input length in bytes. */
        NULL_PTR,                           /* outputPtr                  - Pointer to the output data. */
        NULL_PTR,                           /* outputLengthPtr            - Holds a pointer to a memory location containing the output length in bytes. */
        NULL_PTR,                           /* secondaryOutputPtr         - Pointer to the secondary output data. */
        NULL_PTR,                           /* secondaryOutputLengthPtr   - Holds a pointer to a memory location containing the secondary output length in bytes. */
        0U,                                 /* input64                    - Versatile input parameter */
        NULL_PTR,                           /* verifyPtr                  - Output pointer to a memory location holding a Crypto_VerifyResultType */
        NULL_PTR,                           /* output64Ptr                - Output pointer to a memory location holding an uint64. */
        CRYPTO_OPERATIONMODE_SINGLECALL,    /* mode                       - Indicator of the mode(s)/operation(s) to be performed */
        0U,                                 /* cryIfKeyId                 - Holds the CryIf key id for key operation services. */
        0U,                                 /* targetCryIfKeyId           - Holds the target CryIf key id for key operation services. */
    },
    &App_JobCmacGeneratePrimitiveInfo,      /* jobPrimitiveInfo           - Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    &App_JobCmacGenerateInfo,               /* jobInfo                    - Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    NULL_PTR,                               /* jobRedirectionInfoRef      - Pointer to a structure containing further information on the usage of keys as input and output for jobs. */
    0U                                      /* cryptoKeyId                - Identifier of the Crypto Driver key. The identifier shall be written by the Crypto Interface */
};

/* --- Structure of the job to be passed to Crypto driver, requesting CMAC Verify ---------------- */
static Crypto_JobType App_JobCmacVerify =
{
    1U,                                     /* jobId                      - Identifier for the job structure */
    CRYPTO_JOBSTATE_IDLE,                   /* jobState                   - Determines the current job state */
    {
        NULL_PTR,                           /* inputPtr                   - Pointer to the input data. */
        0,                                  /* inputLength                - Contains the input length in bytes. */
        NULL_PTR,                           /* secondaryInputPtr          - Pointer to the secondary input data (for MacVerify, SignatureVerify). */
        0U,                                 /* secondaryInputLength       - Contains the secondary input length in bytes. */
        NULL_PTR,                           /* tertiaryInputPtr           - Pointer to the tertiary input data (for MacVerify, SignatureVerify). */
        0U,                                 /* tertiaryInputLength        - Contains the tertiary input length in bytes. */
        NULL_PTR,                           /* outputPtr                  - Pointer to the output data. */
        NULL_PTR,                           /* outputLengthPtr            - Holds a pointer to a memory location containing the output length in bytes. */
        NULL_PTR,                           /* secondaryOutputPtr         - Pointer to the secondary output data. */
        NULL_PTR,                           /* secondaryOutputLengthPtr   - Holds a pointer to a memory location containing the secondary output length in bytes. */
        0U,                                 /* input64                    - Versatile input parameter */
        NULL_PTR,                           /* verifyPtr                  - Output pointer to a memory location holding a Crypto_VerifyResultType */
        NULL_PTR,                           /* output64Ptr                - Output pointer to a memory location holding an uint64. */
        CRYPTO_OPERATIONMODE_SINGLECALL,    /* mode                       - Indicator of the mode(s)/operation(s) to be performed */
        0U,                                 /* cryIfKeyId                 - Holds the CryIf key id for key operation services. */
        0U,                                 /* targetCryIfKeyId           - Holds the target CryIf key id for key operation services. */
    },
    &App_JobCmacVerifyPrimitiveInfo,        /* jobPrimitiveInfo           - Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    &App_JobCmacVerifyInfo,                 /* jobInfo                    - Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    NULL_PTR,                               /* jobRedirectionInfoRef      - Pointer to a structure containing further information on the usage of keys as input and output for jobs. */
    0U                                      /* cryptoKeyId                - Identifier of the Crypto Driver key. The identifier shall be written by the Crypto Interface */
};

#define CRYPTO_STOP_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE
#include "Crypto_MemMap.h"


/* Variable storing the result of the CMAC Verify operation */
static Crypto_VerifyResultType App_CmacVerifyResult = CRYPTO_E_VER_NOT_OK;

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL VARIABLES related to the 'HASH' part of the example                                    */
/* ----------------------------------------------------------------------------------------------- */
/* --- The following RAM variables need to be put in non-cacheable memory in order to be correctly seen by HSE --- */
#define CRYPTO_START_SEC_VAR_INIT_8_NO_CACHEABLE
#include "Crypto_MemMap.h"

static uint8 App_au8HashData[APP_HASH_DATA_SIZE] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
};


static uint8 App_au8HashSha1Digest[APP_HASH_SHA_1_DIGEST_SIZE] =
{
    0xc6, 0x13, 0x8d, 0x51, 0x4f, 0xfa, 0x21, 0x35, 0xbf, 0xce, 0x0e, 0xd0, 0xb8, 0xfa, 0xc6, 0x56,
    0x69, 0x91, 0x7e, 0xc7
};

static uint8 App_au8HashSha256Digest[APP_HASH_SHA_256_DIGEST_SIZE] =
{
    0xfd, 0xea, 0xb9, 0xac, 0xf3, 0x71, 0x03, 0x62, 0xbd, 0x26, 0x58, 0xcd, 0xc9, 0xa2, 0x9e, 0x8f,
    0x9c, 0x75, 0x7f, 0xcf, 0x98, 0x11, 0x60, 0x3a, 0x8c, 0x44, 0x7c, 0xd1, 0xd9, 0x15, 0x11, 0x08
};

#define CRYPTO_STOP_SEC_VAR_INIT_8_NO_CACHEABLE
#include "Crypto_MemMap.h"


#define CRYPTO_START_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Crypto_MemMap.h"

static uint8  App_au8HashResult[APP_MAX_DIGEST_SIZE];

#define CRYPTO_STOP_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Crypto_MemMap.h"


#define CRYPTO_START_SEC_VAR_CLEARED_32_NO_CACHEABLE
#include "Crypto_MemMap.h"

static uint32 App_u32HashResultSize;

#define CRYPTO_STOP_SEC_VAR_CLEARED_32_NO_CACHEABLE
#include "Crypto_MemMap.h"


/* ---- Crypto job sub-structures for HASH SHA-1 ------------------------------------------------- */
static Crypto_JobPrimitiveInfoType App_JobHashSha1PrimitiveInfo =
{
    0U,                                     /* callbackId                 - Identifier of the callback function, to be called, if the configured service finished. */
    &App_HashSha1PrimitiveInfo,             /* primitiveInfo              - Pointer to a structure containing further configuration of the crypto primitives */
    0U,                                     /* cryIfKeyId                 - Identifier of the CryIf key. */
    CRYPTO_PROCESSING_SYNC,                 /* processingType             - Determines the synchronous or asynchronous behavior. */
    (boolean)FALSE                          /* callbackUpdateNotification - Indicates, whether the callback function shall be called, if the UPDATE operation has finished. */
};

/* ---- Crypto job sub-structures for HASH SHA2-256 ---------------------------------------------- */
static Crypto_JobPrimitiveInfoType App_JobHashSha256PrimitiveInfo =
{
    0U,                                     /* callbackId                 - Identifier of the callback function, to be called, if the configured service finished. */
    &App_HashSha256PrimitiveInfo,           /* primitiveInfo              - Pointer to a structure containing further configuration of the crypto primitives */
    0U,                                     /* cryIfKeyId                 - Identifier of the CryIf key. */
    CRYPTO_PROCESSING_SYNC,                 /* processingType             - Determines the synchronous or asynchronous behavior. */
    (boolean)FALSE                          /* callbackUpdateNotification - Indicates, whether the callback function shall be called, if the UPDATE operation has finished. */
};

/* --- Structure of the job to be passed to Crypto driver, requesting Hash with SHA1 ------------- */
static Crypto_JobType App_JobHashSha1 =
{
    1U,                                     /* jobId                      - Identifier for the job structure */
    CRYPTO_JOBSTATE_IDLE,                   /* jobState                   - Determines the current job state */
    {
        App_au8HashData,                    /* inputPtr                   - Pointer to the input data. */
        APP_HASH_DATA_SIZE,                 /* inputLength                - Contains the input length in bytes. */
        NULL_PTR,                           /* secondaryInputPtr          - Pointer to the secondary input data (for MacVerify, SignatureVerify). */
        0U,                                 /* secondaryInputLength       - Contains the secondary input length in bytes. */
        NULL_PTR,                           /* tertiaryInputPtr           - Pointer to the tertiary input data (for MacVerify, SignatureVerify). */
        0U,                                 /* tertiaryInputLength        - Contains the tertiary input length in bytes. */
        App_au8HashResult,                  /* outputPtr                  - Pointer to the output data. */
        &App_u32HashResultSize,             /* outputLengthPtr            - Holds a pointer to a memory location containing the output length in bytes. */
        NULL_PTR,                           /* secondaryOutputPtr         - Pointer to the secondary output data. */
        NULL_PTR,                           /* secondaryOutputLengthPtr   - Holds a pointer to a memory location containing the secondary output length in bytes. */
        0U,                                 /* input64                    - Versatile input parameter */
        NULL_PTR,                           /* verifyPtr                  - Output pointer to a memory location holding a Crypto_VerifyResultType */
        NULL_PTR,                           /* output64Ptr                - Output pointer to a memory location holding an uint64. */
        CRYPTO_OPERATIONMODE_SINGLECALL,    /* mode                       - Indicator of the mode(s)/operation(s) to be performed */
        0U,                                 /* cryIfKeyId                 - Holds the CryIf key id for key operation services. */
        0U,                                 /* targetCryIfKeyId           - Holds the target CryIf key id for key operation services. */
    },
    &App_JobHashSha1PrimitiveInfo,          /* jobPrimitiveInfo           - Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    &App_JobHashSha1Info,                   /* jobInfo                    - Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    NULL_PTR,                               /* jobRedirectionInfoRef      - Pointer to a structure containing further information on the usage of keys as input and output for jobs. */
    0U                                      /* cryptoKeyId                - Identifier of the Crypto Driver key. The identifier shall be written by the Crypto Interface */
};

/* --- Structure of the job to be passed to Crypto driver, requesting Hash with SHA256 ----------- */
static Crypto_JobType App_JobHashSha256 =
{
    1U,                                     /* jobId                      - Identifier for the job structure */
    CRYPTO_JOBSTATE_IDLE,                   /* jobState                   - Determines the current job state */
    {
        App_au8HashData,                    /* inputPtr                   - Pointer to the input data. */
        APP_HASH_DATA_SIZE,                 /* inputLength                - Contains the input length in bytes. */
        NULL_PTR,                           /* secondaryInputPtr          - Pointer to the secondary input data (for MacVerify, SignatureVerify). */
        0U,                                 /* secondaryInputLength       - Contains the secondary input length in bytes. */
        NULL_PTR,                           /* tertiaryInputPtr           - Pointer to the tertiary input data (for MacVerify, SignatureVerify). */
        0U,                                 /* tertiaryInputLength        - Contains the tertiary input length in bytes. */
        App_au8HashResult,                  /* outputPtr                  - Pointer to the output data. */
        &App_u32HashResultSize,             /* outputLengthPtr            - Holds a pointer to a memory location containing the output length in bytes. */
        NULL_PTR,                           /* secondaryOutputPtr         - Pointer to the secondary output data. */
        NULL_PTR,                           /* secondaryOutputLengthPtr   - Holds a pointer to a memory location containing the secondary output length in bytes. */
        0U,                                 /* input64                    - Versatile input parameter */
        NULL_PTR,                           /* verifyPtr                  - Output pointer to a memory location holding a Crypto_VerifyResultType */
        NULL_PTR,                           /* output64Ptr                - Output pointer to a memory location holding an uint64. */
        CRYPTO_OPERATIONMODE_SINGLECALL,    /* mode                       - Indicator of the mode(s)/operation(s) to be performed */
        0U,                                 /* cryIfKeyId                 - Holds the CryIf key id for key operation services. */
        0U,                                 /* targetCryIfKeyId           - Holds the target CryIf key id for key operation services. */
    },
    &App_JobHashSha256PrimitiveInfo,        /* jobPrimitiveInfo           - Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    &App_JobHashSha256Info,                 /* jobInfo                    - Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    NULL_PTR,                               /* jobRedirectionInfoRef      - Pointer to a structure containing further information on the usage of keys as input and output for jobs. */
    0U                                      /* cryptoKeyId                - Identifier of the Crypto Driver key. The identifier shall be written by the Crypto Interface */
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
/* Utility functions prototypes */
static boolean Util_Memcmp
(
    uint8* pSource,
    uint8* pDest,
    uint32 u32Size
);

static void Util_Memset
(
    void*  pDest,
    uint8  u8Value,
    uint32 u32Size
);

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL FUNCTION PROTOTYPES related to the 'AES128 encrypt/decrypt' part of the example        */
/* ----------------------------------------------------------------------------------------------- */
static void App_PrepareAes128EcbEncryptJob
(
    uint32  u32KeyId,
    uint8*  pPlainText,
    uint32  u32PlainTextSize,
    uint8*  pCipherText,
    uint32* pu32CipherTextSize
);

static void App_PrepareAes128EcbDecryptJob
(
    uint32  u32KeyId,
    uint8*  pCipherText,
    uint32  u32CipherTextSize,
    uint8*  pPlainText,
    uint32* pu32PlainTextSize
);

static void App_Aes128EncryptDecryptExample(void);

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL FUNCTION PROTOTYPES related to the 'CMAC generate/verify' part of the example          */
/* ----------------------------------------------------------------------------------------------- */
static void App_PrepareCmacGenerateJob
(
    uint32  u32KeyId,
    uint8*  pData,
    uint32  u32DataSize,
    uint8*  pTag,
    uint32* pu32TagLength
);

static void App_PrepareCmacVerifyJob
(
    uint32                   u32KeyId,
    uint8*                   pData,
    uint32                   u32DataSize,
    uint8*                   pTag,
    uint32                   u32TagLength,
    Crypto_VerifyResultType* pVerifyResult
);

static void App_CmacGenerateVerifyExample(void);

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL FUNCTION PROTOTYPES related to the 'HASH' part of the example                          */
/* ----------------------------------------------------------------------------------------------- */
static void App_HashExample(void);

/*==================================================================================================
*                                       LOCAL FUNCTIONS
===================================================================================================*/
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

static void Util_Memset
(
    void*  pDest,
    uint8  u8Value,
    uint32 u32Size
)
{
    uint32 u32Counter;
    uint8* ptr = (uint8*)pDest;

    for (u32Counter = 0U; u32Counter < u32Size; u32Counter++)
    {
        *ptr = u8Value;
        ptr++;
    }
}

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL FUNCTIONS related to the 'AES128 encrypt/decrypt' part of the example                  */
/* ----------------------------------------------------------------------------------------------- */
static void App_PrepareAes128EcbEncryptJob
(
    uint32  u32KeyId,             /* IN     - Identifier of the used Crypto key                                                */
    uint8*  pPlainText,           /* IN     - Pointer to the buffer storing the plain text                                     */
    uint32  u32PlainTextSize,     /* IN     - Size of the plain text                                                           */
    uint8*  pCipherText,          /* IN     - Pointer to the buffer where the cipherText will be stored                        */
    uint32* pu32CipherTextSize    /* IN/OUT - IN:  Contains the max size of the app buffer where the cipher text will be stored
                                              OUT: Is updated by the Crypto driver with the actual size of the cipher text     */
)
{
    App_JobAes128EcbEncryptPrimitiveInfo.cryIfKeyId                 = u32KeyId;
    App_JobAes128EcbEncrypt.jobPrimitiveInputOutput.inputPtr        = pPlainText;
    App_JobAes128EcbEncrypt.jobPrimitiveInputOutput.inputLength     = u32PlainTextSize;
    App_JobAes128EcbEncrypt.jobPrimitiveInputOutput.outputPtr       = pCipherText;
    App_JobAes128EcbEncrypt.jobPrimitiveInputOutput.outputLengthPtr = pu32CipherTextSize;
}

static void App_PrepareAes128EcbDecryptJob
(
    uint32  u32KeyId,            /* IN     - Identifier of the used Crypto key                                                 */
    uint8*  pCipherText,         /* IN     - Pointer to the buffer storing the cipher text                                     */
    uint32  u32CipherTextSize,   /* IN     - Size of the cipher text                                                           */
    uint8*  pPlainText,          /* IN     - Pointer to the buffer where the plain text will be stored                         */
    uint32* pu32PlainTextSize    /* IN/OUT - IN:  Contains the max size of the app buffer where the plain text will be stored
                                             OUT: Is updated by the Crypto driver with the actual size of the plain text       */
)
{
    App_JobAes128EcbDecryptPrimitiveInfo.cryIfKeyId                 = u32KeyId;
    App_JobAes128EcbDecrypt.jobPrimitiveInputOutput.inputPtr        = pCipherText;
    App_JobAes128EcbDecrypt.jobPrimitiveInputOutput.inputLength     = u32CipherTextSize;
    App_JobAes128EcbDecrypt.jobPrimitiveInputOutput.outputPtr       = pPlainText;
    App_JobAes128EcbDecrypt.jobPrimitiveInputOutput.outputLengthPtr = pu32PlainTextSize;
}

/**
* @brief          Exercises the AES128 ECB Encryption and Decryption features of Cryto driver
*
* @details        Performs 2 synchronous encryption/decryption operations with 2 different values for the key and 2 different plain/cipher texts
*                 Assumes that both the values of the key and the data to encrypt/decrypt can change at run-time, so they are stored in RAM
*
*                 Xdm configuration:
*                   - HSE Key catalogs should be configured with at least one group of AES128 keys
*                   - Because the value(Key Material) of the AES128 key that is used is changed at run-time, the key element that stores the key must be stored
*                 in the HSE RAM Catalog so it can be updated for as many times as needed.
*                   - The HseKeyFlags of the Key Element storing the HSE AES128 key must have the HSE_KF_USAGE_ENCRYPT and HSE_KF_USAGE_DECRYPT flags, in order
*                 to use the key for encrypt/decrypt operations
*                   - The Crypto Driver Object that is used to process the jobs (CDO_Symmetric) should have the AES128_Encrypt and AES128_Decrypt primitives set
*/
static void App_Aes128EncryptDecryptExample(void)
{
    Std_ReturnType RetVal;

    /* =============================================================================================================================== */
    /*    Encryption Example 1: Using first key to encrypt 16 bytes of data                                                            */
    /* =============================================================================================================================== */

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Key management                                                                                                               */
    /* ------------------------------------------------------------------------------------------------------------------------------- */

    /* Load the value of the first AES128 key into HSE */
    RetVal = Crypto_KeyElementSet(APP_AES128_KEY_ID, KEY_MATERIAL_ELEMENT_ID_U32, App_au8Aes128EcbKey_1, APP_AES128_KEY_SIZE);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* Mark the key as valid, so it can be used by Crypto driver in future job requests */
    RetVal = Crypto_KeySetValid(APP_AES128_KEY_ID);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    AES128 ECB Encryption                                                                                                        */
    /* ------------------------------------------------------------------------------------------------------------------------------- */

    /* This variable will be used to inform Crypto driver about the max length in bytes of the buffer where it can put the result of the encryption */
    App_u32Aes128EcbResultSize = APP_AES128_ECB_RESULT_SIZE;

    /* Clear the result buffer, in order to be able to check the successful result of encryption */
    Util_Memset(App_au8Aes128EcbResult, 0U, APP_AES128_ECB_RESULT_SIZE);

    /* Prepare the information in the job to be sent to Crypto driver */
    App_PrepareAes128EcbEncryptJob(APP_AES128_KEY_ID, App_au8Aes128EcbPlaintext_1, APP_AES128_ECB_PLAIN_TEXT_SIZE_1, App_au8Aes128EcbResult, &App_u32Aes128EcbResultSize);

    /* Request Crypto driver to perform AES128 Encryption */
    RetVal = Crypto_ProcessJob(APP_SYMMETRIC_CDO_ID, &App_JobAes128EcbEncrypt);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(App_u32Aes128EcbResultSize == APP_AES128_ECB_CIPHER_TEXT_SIZE_1);
    App_SetSuccessStatus(Util_Memcmp(App_au8Aes128EcbResult, App_au8Aes128EcbCiphertext_1, APP_AES128_ECB_CIPHER_TEXT_SIZE_1));

    /* =============================================================================================================================== */
    /*    Encryption Example 2: Using second key to encrypt 64 bytes of data                                                           */
    /* =============================================================================================================================== */

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Key management                                                                                                               */
    /* ------------------------------------------------------------------------------------------------------------------------------- */

    /* Load the value of the second AES128 key into HSE */
    RetVal = Crypto_KeyElementSet(APP_AES128_KEY_ID, KEY_MATERIAL_ELEMENT_ID_U32, App_au8Aes128EcbKey_2, APP_AES128_KEY_SIZE);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* Mark the key as valid, so it can be used by Crypto driver in future job requests */
    RetVal = Crypto_KeySetValid(APP_AES128_KEY_ID);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    AES128 ECB Encryption                                                                                                        */
    /* ------------------------------------------------------------------------------------------------------------------------------- */

    /* This variable will be used to inform Crypto driver about the max length in bytes of the buffer where it can put the result of the encryption */
    App_u32Aes128EcbResultSize = APP_AES128_ECB_RESULT_SIZE;

    /* Clear the result buffer, in order to be able to check the successful result of encryption */
    Util_Memset(App_au8Aes128EcbResult, 0U, APP_AES128_ECB_RESULT_SIZE);

    /* Prepare the information in the job to be sent to Crypto driver */
    App_PrepareAes128EcbEncryptJob(APP_AES128_KEY_ID, App_au8Aes128EcbPlaintext_2, APP_AES128_ECB_PLAIN_TEXT_SIZE_2, App_au8Aes128EcbResult, &App_u32Aes128EcbResultSize);

    /* Request Crypto driver to perform AES128 Encryption */
    RetVal = Crypto_ProcessJob(APP_SYMMETRIC_CDO_ID, &App_JobAes128EcbEncrypt);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(App_u32Aes128EcbResultSize == APP_AES128_ECB_CIPHER_TEXT_SIZE_2);
    App_SetSuccessStatus(Util_Memcmp(App_au8Aes128EcbResult, App_au8Aes128EcbCiphertext_2, APP_AES128_ECB_CIPHER_TEXT_SIZE_2));

    /* =============================================================================================================================== */
    /*    Decryption Example 1: Using first key to decrypt 16 bytes of data                                                            */
    /* =============================================================================================================================== */

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Key management                                                                                                               */
    /* ------------------------------------------------------------------------------------------------------------------------------- */

    /* Load the value of the first AES128 key into HSE */
    RetVal = Crypto_KeyElementSet(APP_AES128_KEY_ID, KEY_MATERIAL_ELEMENT_ID_U32, App_au8Aes128EcbKey_1, APP_AES128_KEY_SIZE);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* Mark the key as valid, so it can be used by Crypto driver in future job requests */
    RetVal = Crypto_KeySetValid(APP_AES128_KEY_ID);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    AES128 ECB Decryption                                                                                                        */
    /* ------------------------------------------------------------------------------------------------------------------------------- */

    /* This variable will be used to inform Crypto driver about the max length in bytes of the buffer where it can put the result of the decryption */
    App_u32Aes128EcbResultSize = APP_AES128_ECB_RESULT_SIZE;

    /* Clear the result buffer, in order to be able to check the successful result of decryption */
    Util_Memset(App_au8Aes128EcbResult, 0U, APP_AES128_ECB_RESULT_SIZE);

    /* Prepare the information in the job to be sent to Crypto driver */
    App_PrepareAes128EcbDecryptJob(APP_AES128_KEY_ID, App_au8Aes128EcbCiphertext_1, APP_AES128_ECB_CIPHER_TEXT_SIZE_1, App_au8Aes128EcbResult, &App_u32Aes128EcbResultSize);

    /* Request Crypto driver to perform AES128 Decryption */
    RetVal = Crypto_ProcessJob(APP_SYMMETRIC_CDO_ID, &App_JobAes128EcbDecrypt);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(App_u32Aes128EcbResultSize == APP_AES128_ECB_PLAIN_TEXT_SIZE_1);
    App_SetSuccessStatus(Util_Memcmp(App_au8Aes128EcbResult, App_au8Aes128EcbPlaintext_1, APP_AES128_ECB_PLAIN_TEXT_SIZE_1));

    /* =============================================================================================================================== */
    /*    Decryption Example 2: Using second key to decrypt 64 bytes of data                                                           */
    /* =============================================================================================================================== */

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Key management                                                                                                               */
    /* ------------------------------------------------------------------------------------------------------------------------------- */

    /* Load the value of the second AES128 key into HSE */
    RetVal = Crypto_KeyElementSet(APP_AES128_KEY_ID, KEY_MATERIAL_ELEMENT_ID_U32, App_au8Aes128EcbKey_2, APP_AES128_KEY_SIZE);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* Mark the key as valid, so it can be used by Crypto driver in future job requests */
    RetVal = Crypto_KeySetValid(APP_AES128_KEY_ID);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    AES128 ECB Decryption                                                                                                        */
    /* ------------------------------------------------------------------------------------------------------------------------------- */

    /* This variable will be used to inform Crypto driver about the max length in bytes of the buffer where it can put the result of the decryption */
    App_u32Aes128EcbResultSize = APP_AES128_ECB_RESULT_SIZE;

    /* Clear the result buffer, in order to be able to check the successful result of decryption */
    Util_Memset(App_au8Aes128EcbResult, 0U, APP_AES128_ECB_RESULT_SIZE);

    /* Prepare the information in the job to be sent to Crypto driver */
    App_PrepareAes128EcbDecryptJob(APP_AES128_KEY_ID, App_au8Aes128EcbCiphertext_2, APP_AES128_ECB_CIPHER_TEXT_SIZE_2, App_au8Aes128EcbResult, &App_u32Aes128EcbResultSize);

    /* Request Crypto driver to perform AES128 Decryption */
    RetVal = Crypto_ProcessJob(APP_SYMMETRIC_CDO_ID, &App_JobAes128EcbDecrypt);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(App_u32Aes128EcbResultSize == APP_AES128_ECB_PLAIN_TEXT_SIZE_2);
    App_SetSuccessStatus(Util_Memcmp(App_au8Aes128EcbResult, App_au8Aes128EcbPlaintext_2, APP_AES128_ECB_PLAIN_TEXT_SIZE_2));
}

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL FUNCTIONS related to the 'CMAC generate/verify' part of the example                    */
/* ----------------------------------------------------------------------------------------------- */
static void App_PrepareCmacGenerateJob
(
    uint32  u32KeyId,        /* IN     - Identifier of the used Crypto key                                                */
    uint8*  pData,           /* IN     - Pointer to the buffer storing the data to perform CMAC over                      */
    uint32  u32DataSize,     /* IN     - Size of the data to perform CMAC over                                            */
    uint8*  pTag,            /* IN     - Pointer to the buffer where the CMAC tag will be stored                          */
    uint32* pu32TagLength    /* IN/OUT - IN:  Contains the max size of the app buffer where the CMAC tag will be stored
                                                                            OUT: Is updated by the Crypto driver with the actual size of the CMAC tag        */
)
{
    App_JobCmacGeneratePrimitiveInfo.cryIfKeyId                 = u32KeyId;
    App_JobCmacGenerate.jobPrimitiveInputOutput.inputPtr        = pData;
    App_JobCmacGenerate.jobPrimitiveInputOutput.inputLength     = u32DataSize;
    App_JobCmacGenerate.jobPrimitiveInputOutput.outputPtr       = pTag;
    App_JobCmacGenerate.jobPrimitiveInputOutput.outputLengthPtr = pu32TagLength;
}

static void App_PrepareCmacVerifyJob
(
    uint32                   u32KeyId,        /* IN  - Identifier of the used Crypto key                                                */
    uint8*                   pData,           /* IN  - Pointer to the buffer storing the data to perform CMAC over                      */
    uint32                   u32DataSize,     /* IN  - Size of the data to perform CMAC over                                            */
    uint8*                   pTag,            /* IN  - Pointer to the buffer containing the CMAC to be verified                         */
    uint32                   u32TagLength,    /* IN  - Size of the MAC to be verified                                                   */
    Crypto_VerifyResultType* pVerifyResult    /* OUT - Is updated by the Crypto driver with the status of the CMAC Verify operation     */
)
{
    App_JobCmacVerifyPrimitiveInfo.cryIfKeyId                      = u32KeyId;
    App_JobCmacVerify.jobPrimitiveInputOutput.inputPtr             = pData;
    App_JobCmacVerify.jobPrimitiveInputOutput.inputLength          = u32DataSize;
    App_JobCmacVerify.jobPrimitiveInputOutput.secondaryInputPtr    = pTag;
    App_JobCmacVerify.jobPrimitiveInputOutput.secondaryInputLength = u32TagLength;
    App_JobCmacVerify.jobPrimitiveInputOutput.verifyPtr            = pVerifyResult;
}

/**
* @brief          Exercises the CMAC Generate/Verify features of Cryto driver
*
* @details        Performs 2 synchronous CMAC generate/verify operations with 2 different values for the key and 2 different blocks of data
*                 Assumes that both the values of the key and the data to generate/verify CMAC over can change at run-time, so they are stored in RAM
*
*                 Xdm configuration:
*                   - HSE Key catalogs should be configured with at least one group of AES128 keys
*                   - Because the value(Key Material) of the AES128 key that is used is changed at run-time, the key element that stores the key must be stored
*                 in the HSE RAM Catalog so it can be updated for as many times as needed.
*                   - The HseKeyFlags of the Key Element storing the HSE AES128 key must have the HSE_KF_USAGE_SIGN and HSE_KF_USAGE_VERIFY flags, in order
*                 to use the key for CMAC generate/verify operations
*                   - The Crypto Driver Object that is used to process the jobs (CDO_Symmetric) should have the CMAC_Generate and CMAC_Verify primitives set
*/
static void App_CmacGenerateVerifyExample(void)
{
    Std_ReturnType RetVal;

    /* =============================================================================================================================== */
    /*    CMAC Generation Example 1: Using first key to perform a CMAC over 16 bytes of data                                           */
    /* =============================================================================================================================== */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Key management                                                                                                               */
    /* ------------------------------------------------------------------------------------------------------------------------------- */

    /* Load the first CMAC key */
    RetVal = Crypto_KeyElementSet(APP_CMAC_KEY_ID, KEY_MATERIAL_ELEMENT_ID_U32, App_au8CmacKey_1, APP_CMAC_KEY_SIZE);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* Mark the key as valid */
    RetVal = Crypto_KeySetValid(APP_CMAC_KEY_ID);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    CMAC Generation                                                                                                              */
    /* ------------------------------------------------------------------------------------------------------------------------------- */

    /* This variable will be used to inform Crypto driver about the max length in bytes of the buffer where it can put the result of the CMAC Generate operation */
    App_u32CmacResultSize = APP_CMAC_TAG_SIZE;

    /* Clear the result buffer, in order to be able to check the successful result of CMAC */
    Util_Memset(App_au8CmacResult, 0U, APP_CMAC_TAG_SIZE);

    /* Prepare the information in the job to be sent to Crypto driver */
    App_PrepareCmacGenerateJob(APP_CMAC_KEY_ID, App_au8CmacData_1, APP_CMAC_DATA_SIZE_1, App_au8CmacResult, &App_u32CmacResultSize);

    /* Request Crypto driver to perform CMAC Generate */
    RetVal = Crypto_ProcessJob(APP_SYMMETRIC_CDO_ID, &App_JobCmacGenerate);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(App_u32CmacResultSize == APP_CMAC_TAG_SIZE);
    App_SetSuccessStatus(Util_Memcmp(App_au8CmacResult, App_au8CmacTag_1, APP_CMAC_TAG_SIZE));

    /* =============================================================================================================================== */
    /*    CMAC Generation Example 2: Using second key to perform a CMAC over 64 bytes of data                                          */
    /* =============================================================================================================================== */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Key management                                                                                                               */
    /* ------------------------------------------------------------------------------------------------------------------------------- */

    /* Load the second CMAC key */
    RetVal = Crypto_KeyElementSet(APP_CMAC_KEY_ID, KEY_MATERIAL_ELEMENT_ID_U32, App_au8CmacKey_2, APP_CMAC_KEY_SIZE);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* Mark the key as valid */
    RetVal = Crypto_KeySetValid(APP_CMAC_KEY_ID);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    CMAC Generation                                                                                                              */
    /* ------------------------------------------------------------------------------------------------------------------------------- */

    /* This variable will be used to inform Crypto driver about the max length in bytes of the buffer where it can put the result of the CMAC Generate operation */
    App_u32CmacResultSize = APP_CMAC_TAG_SIZE;

    /* Clear the result buffer, in order to be able to check the successful result of CMAC */
    Util_Memset(App_au8CmacResult, 0U, APP_CMAC_TAG_SIZE);

    /* Prepare the information in the job to be sent to Crypto driver */
    App_PrepareCmacGenerateJob(APP_CMAC_KEY_ID, App_au8CmacData_2, APP_CMAC_DATA_SIZE_2, App_au8CmacResult, &App_u32CmacResultSize);

    /* Request Crypto driver to perform CMAC Generate */
    RetVal = Crypto_ProcessJob(APP_SYMMETRIC_CDO_ID, &App_JobCmacGenerate);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(App_u32CmacResultSize == APP_CMAC_TAG_SIZE);
    App_SetSuccessStatus(Util_Memcmp(App_au8CmacResult, App_au8CmacTag_2, APP_CMAC_TAG_SIZE));


    /* =============================================================================================================================== */
    /*    CMAC Verification Example 1: Using first key to perform a CMAC verify over 16 bytes of data                                  */
    /* =============================================================================================================================== */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Key management                                                                                                               */
    /* ------------------------------------------------------------------------------------------------------------------------------- */

    /* Load the first CMAC key */
    RetVal = Crypto_KeyElementSet(APP_CMAC_KEY_ID, KEY_MATERIAL_ELEMENT_ID_U32, App_au8CmacKey_1, APP_CMAC_KEY_SIZE);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* Mark the key as valid */
    RetVal = Crypto_KeySetValid(APP_CMAC_KEY_ID);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    CMAC Verification                                                                                                            */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /* Mark the result of the CMAC verify operation as invalid */
    App_CmacVerifyResult = CRYPTO_E_VER_NOT_OK;

    /* Prepare the information in the job to be sent to Crypto driver */
    App_PrepareCmacVerifyJob(APP_CMAC_KEY_ID, App_au8CmacData_1, APP_CMAC_DATA_SIZE_1, App_au8CmacTag_1, APP_CMAC_TAG_SIZE, &App_CmacVerifyResult);

    /* Request Crypto driver to perform CMAC Verify */
    RetVal = Crypto_ProcessJob(APP_SYMMETRIC_CDO_ID, &App_JobCmacVerify);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(CRYPTO_E_VER_OK == App_CmacVerifyResult);

    /* =============================================================================================================================== */
    /*    CMAC Verification Example 2: Using second key to perform a CMAC verify over 64 bytes of data                                 */
    /* =============================================================================================================================== */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    Key management                                                                                                               */
    /* ------------------------------------------------------------------------------------------------------------------------------- */

    /* Load the second CMAC key */
    RetVal = Crypto_KeyElementSet(APP_CMAC_KEY_ID, KEY_MATERIAL_ELEMENT_ID_U32, App_au8CmacKey_2, APP_CMAC_KEY_SIZE);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* Mark the key as valid */
    RetVal = Crypto_KeySetValid(APP_CMAC_KEY_ID);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);

    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /*    CMAC Verification                                                                                                            */
    /* ------------------------------------------------------------------------------------------------------------------------------- */
    /* Mark the result of the CMAC verify operation as invalid */
    App_CmacVerifyResult = CRYPTO_E_VER_NOT_OK;

    /* Prepare the information in the job to be sent to Crypto driver */
    App_PrepareCmacVerifyJob(APP_CMAC_KEY_ID, App_au8CmacData_2, APP_CMAC_DATA_SIZE_2, App_au8CmacTag_2, APP_CMAC_TAG_SIZE, &App_CmacVerifyResult);

    /* Request Crypto driver to perform CMAC Verify */
    RetVal = Crypto_ProcessJob(APP_SYMMETRIC_CDO_ID, &App_JobCmacVerify);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(CRYPTO_E_VER_OK == App_CmacVerifyResult);
}

/* ----------------------------------------------------------------------------------------------- */
/*    LOCAL FUNCTIONS related to the 'HASH' part of the example                                    */
/* ----------------------------------------------------------------------------------------------- */
/**
* @brief          Exercises the Hash feature of Cryto driver
*
* @details        Performs 2 synchronous Hash operations (SHA-1, SHA2-256) over a 64 bytes block of data
*                 Assumes that the value of the data to hash can change at run-time, so it is stored in RAM
*
*                 Xdm configuration:
*                   - The Crypto Driver Object that is used to process the jobs (CDO_Symmetric) should have the HASH_SHA1 and HASH_SHA2_256 primitives set
*/
static void App_HashExample(void)
{
    Std_ReturnType RetVal;

    /* =============================================================================================================================== */
    /*    HASH Generation Example 1: SHA-1 over 64 bytes of data                                                                       */
    /* =============================================================================================================================== */

    /* This variable will be used to inform Crypto driver about the max length in bytes of the buffer where it can put the result of the HASH */
    App_u32HashResultSize = APP_MAX_DIGEST_SIZE;
    /* Clear the result buffer, in order to be able to check the successful result of the HASH operation */
    Util_Memset(App_au8HashResult, 0U, APP_MAX_DIGEST_SIZE);

    /* Request Crypto driver to perform a HASH operation */
    RetVal = Crypto_ProcessJob(APP_SYMMETRIC_CDO_ID, &App_JobHashSha1);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(Util_Memcmp(App_au8HashResult, App_au8HashSha1Digest, APP_HASH_SHA_1_DIGEST_SIZE));

    /* =============================================================================================================================== */
    /*    HASH Generation Example 1: SHA-256 over 64 bytes of data                                                                     */
    /* =============================================================================================================================== */

    /* This variable will be used to inform Crypto driver about the max length in bytes of the buffer where it can put the result of the HASH */
    App_u32HashResultSize = APP_MAX_DIGEST_SIZE;
    /* Clear the result buffer, in order to be able to check the successful result of the HASH operation */
    Util_Memset(App_au8HashResult, 0U, APP_MAX_DIGEST_SIZE);

    /* Request Crypto driver to perform a HASH operation */
    RetVal = Crypto_ProcessJob(APP_SYMMETRIC_CDO_ID, &App_JobHashSha256);
    App_SetSuccessStatus((Std_ReturnType)E_OK == RetVal);
    App_SetSuccessStatus(Util_Memcmp(App_au8HashResult, App_au8HashSha256Digest, APP_HASH_SHA_256_DIGEST_SIZE));
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
int main(void)
{
    Std_ReturnType RetVal;

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
    /*    Run the AES128 encrypt/decrypt example                                                                                       */
    /* =============================================================================================================================== */
    App_Aes128EncryptDecryptExample();

    /* =============================================================================================================================== */
    /*    Run the CMAC generate/verify example                                                                                         */
    /* =============================================================================================================================== */
    App_CmacGenerateVerifyExample();

    /* =============================================================================================================================== */
    /*    Run the HASH example                                                                                                         */
    /* =============================================================================================================================== */
    App_HashExample();

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
