/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Csm
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

#ifndef CSM_TYPES_H
#define CSM_TYPES_H

/**
*   @file
*
*   @internal
*   @addtogroup CSM
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Mcal.h"

/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CSM_VENDOR_ID_TYPES                       43
#define CSM_MODULE_ID_TYPES                       110
#define CSM_AR_RELEASE_MAJOR_VERSION_TYPES        4
#define CSM_AR_RELEASE_MINOR_VERSION_TYPES        4
#define CSM_AR_RELEASE_REVISION_VERSION_TYPES     0
#define CSM_SW_MAJOR_VERSION_TYPES                2
#define CSM_SW_MINOR_VERSION_TYPES                0
#define CSM_SW_PATCH_VERSION_TYPES                1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if header file and Mcal header file are of the same AutoSar version */
    #if ((CSM_AR_RELEASE_MAJOR_VERSION_TYPES != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (CSM_AR_RELEASE_MINOR_VERSION_TYPES != MCAL_AR_RELEASE_MINOR_VERSION)    \
        )
        #error "AutoSar Version Numbers of Csm_Types.h and Mcal.h are different"
    #endif
#endif

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/* AUTOSAR Key Element Index Definition */
/* MAC */
#define CRYPTO_KE_MAC_KEY                               ((uint32)1U)    /* Key Material */
#define CRYPTO_KE_MAC_PROOF                             ((uint32)2U)    /* Proof (SHE) */
/* Signature */
#define CRYPTO_KE_SIGNATURE_KEY                         ((uint32)1U)    /* Key Material */
/* Random */
#define CRYPTO_KE_RANDOM_SEED_STATE                     ((uint32)3U)    /* Seed State */
#define CRYPTO_KE_RANDOM_ALGORITHM                      ((uint32)4U)    /* Algorithm */
/* Cipher/AEAD */
#define CRYPTO_KE_CIPHER_KEY                            ((uint32)1U)    /* Key Material */
#define CRYPTO_KE_CIPHER_IV                             ((uint32)5U)    /* Init Vector */
#define CRYPTO_KE_CIPHER_PROOF                          ((uint32)6U)    /* Proof (SHE) */
#define CRYPTO_KE_CIPHER_2NDKEY                         ((uint32)7U)    /* 2nd Key Material */
/* Key Exchange */
#define CRYPTO_KE_KEYEXCHANGE_BASE                      ((uint32)8U)    /* Base */
#define CRYPTO_KE_KEYEXCHANGE_PRIVKEY                   ((uint32)9U)    /* Private Key */
#define CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY                 ((uint32)10U)   /* Own Public Key */
#define CYRPTO_KE_KEYEXCHANGE_SHAREDVALUE               ((uint32)1U)    /* Shared Value */
#define CRYPTO_KE_KEYEXCHANGE_ALGORITHM                 ((uint32)12U)   /* Algorithm */
/* Key Derivation */
#define CRYPTO_KE_KEYDERIVATION_PASSWORD                ((uint32)1U)    /* Password */
#define CRYPTO_KE_KEYDERIVATION_SALT                    ((uint32)13U)   /* Salt */
#define CRYPTO_KE_KEYDERIVATION_ITERATIONS              ((uint32)14U)   /* Iterations */
#define CRYPTO_KE_KEYDERIVATION_ALGORITHM               ((uint32)15U)   /* Algorithm */
/* Key Generate */
#define CRYPTO_KE_KEYGENERATE_KEY                       ((uint32)1U)    /* Algorithm */
#define CRYPTO_KE_KEYGENERATE_SEED                      ((uint32)16U)   /* Algorithm */
#define CRYPTO_KE_KEYGENERATE_ALGORITHM                 ((uint32)17U)   /* Algorithm */
/* Certificate Parsing */
#define CRYPTO_KE_CERTIFICATE_DATA                      ((uint32)0U)    /* Certificate */
#define CRYPTO_KE_CERTIFICATE_PARSING_FORMAT            ((uint32)18U)   /* Format */
#define CRYPTO_KE_CERTIFICATE_CURRENT_TIME              ((uint32)19U)   /* Current time */
#define CRYPTO_KE_CERTIFICATE_VERSION                   ((uint32)20U)   /* Version */
#define CRYPTO_KE_CERTIFICATE_SERIALNUMBER              ((uint32)21U)   /* Serial Number */
#define CRYPTO_KE_CERTIFICATE_SIGNATURE_ALGORITHM       ((uint32)22U)   /* Signature Algorithm */
#define CRYPTO_KE_CERTIFICATE_ISSUER                    ((uint32)23U)   /* Issuer */
#define CRYPTO_KE_CERTIFICATE_VALIDITY_NOT_BEFORE       ((uint32)24U)   /* Validity Start */
#define CRYPTO_KE_CERTIFICATE_VALIDITY_NOT_AFTER        ((uint32)25U)   /* Validity End */
#define CRYPTO_KE_CERTIFICATE_SUBJECT                   ((uint32)26U)   /* Subject */
#define CRYPTO_KE_CERTIFICATE_SUBJECT_PUBLIC_KEY        ((uint32)1U)    /* Subject Public Key */
#define CRYPTO_KE_CERTIFICATE_EXTENSIONS                ((uint32)27U)   /* Extensions */
#define CRYPTO_KE_CERTIFICATE_SIGNATURE                 ((uint32)28U)   /* Signature */

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/* Structure which contains job information (job ID and job priority). */
typedef struct
{
    const uint32 jobId;         /* The family of the algorithm */
    const uint32 jobPriority;   /* Specifies the importance of the job (the higher, the more important). */
} Crypto_JobInfoType;

/* Enumeration of the current job state. */
typedef enum
{
    CRYPTO_JOBSTATE_IDLE   = 0x00U,  /* Job is in the state "idle". This state is reached after Csm_Init() or when the "Finish" state is finished. */
    CRYPTO_JOBSTATE_ACTIVE = 0x01U   /* Job is in the state "active". There was already some input or there are intermediate results. This state is reached, when the "update" or "start" operation  finishes. */
} Crypto_JobStateType;

/* Enumeration of the algorithm mode */
typedef enum
{
    CRYPTO_ALGOMODE_NOT_SET           = 0x00U,  /* Algorithm key is not set */
    CRYPTO_ALGOMODE_ECB               = 0x01U,  /* Blockmode: Electronic Code Book */
    CRYPTO_ALGOMODE_CBC               = 0x02U,  /* Blockmode: Cipher Block Chaining */
    CRYPTO_ALGOMODE_CFB               = 0x03U,  /* Blockmode: Cipher Feedback Mode */
    CRYPTO_ALGOMODE_OFB               = 0x04U,  /* Blockmode: Output Feedback Mode */
    CRYPTO_ALGOMODE_CTR               = 0x05U,  /* Blockmode: Counter Modex */
    CRYPTO_ALGOMODE_GCM               = 0x06U,  /* Blockmode: Galois/Counter Mode */
    CRYPTO_ALGOMODE_XTS               = 0x07U,  /* XOR-encryption-based tweaked-codebook mode with ciphertext stealing */
    CRYPTO_ALGOMODE_RSAES_OAEP        = 0x08U,  /* RSA Optimal Asymmetric Encryption Padding */
    CRYPTO_ALGOMODE_RSAES_PKCS1_v1_5  = 0x09U,  /* RSA encryption/decryption with PKCS#1 v1.5 padding */
    CRYPTO_ALGOMODE_RSASSA_PSS        = 0x0AU,  /* RSA Probabilistic Signature Scheme */
    CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5 = 0x0BU,  /* RSA signature with PKCS#1 v1.5 */
    CRYPTO_ALGOMODE_8ROUNDS           = 0x0CU,  /* 8 rounds (e.g. ChaCha8) */
    CRYPTO_ALGOMODE_12ROUNDS          = 0x0DU,  /* 12 rounds (e.g. ChaCha12) */
    CRYPTO_ALGOMODE_20ROUNDS          = 0x0EU,  /* 20 rounds (e.g. ChaCha20) */
    CRYPTO_ALGOMODE_HMAC              = 0x0FU,  /* Hashed-based MAC */
    CRYPTO_ALGOMODE_CMAC              = 0x10U,  /* Cipher-based MAC */
    CRYPTO_ALGOMODE_GMAC              = 0x11U,  /* Galois MAC */
    CRYPTO_ALGOMODE_CTRDRBG           = 0x12U,  /* Counter-based Deterministic Random Bit Generator */
    CRYPTO_ALGOMODE_SIPHASH_2_4       = 0x13U,  /* Siphash-2-4 */
    CRYPTO_ALGOMODE_SIPHASH_4_8       = 0x14U,  /* Siphash-4-8 */
    CRYPTO_ALGOMODE_PXXXR1            = 0x15U,  /* ANSI R1 Curve */
    CRYPTO_ALGOMODE_CUSTOM            = 0xffU   /* Custom algorithm mode */
} Crypto_AlgorithmModeType;


/* Enumeration of the result type of verification operations. */
typedef enum
{
    CRYPTO_E_VER_OK     = 0x00U,    /* The result of the verification is "true", i.e. the two compared elements are identical. This return code shall be given as value "0" */
    CRYPTO_E_VER_NOT_OK = 0x01U     /* The result of the verification is "false", i.e. the two compared elements are not identical. This return code shall be given as value "1". */
} Crypto_VerifyResultType;

/* Indicator of the mode(s)/operation(s) to be performed. Enumeration which operation shall be performed.
   This enumeration is constructed from a bit mask, where the first bit indicates "Start", the second "Update" and the third "Finish".*/
typedef enum
{
    CRYPTO_OPERATIONMODE_START       = 0x01U, /* Operation Mode is "Start". The job's state shall be reset, i.e. previous input data and intermediate results shall be deleted. */
    CRYPTO_OPERATIONMODE_UPDATE      = 0x02U, /* Operation Mode is "Update". Used to calculate intermediate results. */
    CRYPTO_OPERATIONMODE_STREAMSTART = 0x03U, /* Operation Mode is "Stream Start". Mixture of "Start" and "Update". Used for streaming. */
    CRYPTO_OPERATIONMODE_FINISH      = 0x04U, /* Operation Mode is "Finish". The calculations shall be finalized */
    CRYPTO_OPERATIONMODE_SINGLECALL  = 0x07U  /* Operation Mode is "Single Call". Mixture of "Start", "Update" and "Finish". */
} Crypto_OperationModeType;

/* Enumeration of the kind of the service. */
typedef enum
{
    CRYPTO_HASH                  = 0x00U,  /* Hash Service */
    CRYPTO_MACGENERATE           = 0x01U,  /* MacGenerate Service */
    CRYPTO_MACVERIFY             = 0x02U,  /* MacVerify Service */
    CRYPTO_ENCRYPT               = 0x03U,  /* Encrypt Service */
    CRYPTO_DECRYPT               = 0x04U,  /* Decrypt Service */
    CRYPTO_AEADENCRYPT           = 0x05U,  /* AEADEncrypt Service */
    CRYPTO_AEADDECRYPT           = 0x06U,  /* AEADDecrypt Service */
    CRYPTO_SIGNATUREGENERATE     = 0x07U,  /* SignatureGenerate Service */
    CRYPTO_SIGNATUREVERIFY       = 0x08U,  /* SignatureVerify Service */
    CRYPTO_RANDOMGENERATE        = 0x0BU,  /* RandomGenerate Service */
    CRYPTO_RANDOMSEED            = 0x0CU,  /* RandomSeed Service */
    CRYPTO_KEYGENERATE           = 0x0DU,  /* KeyGenerate Service */
    CRYPTO_KEYDERIVE             = 0x0EU,  /* KeyDerive Service */
    CRYPTO_KEYEXCHANGECALCPUBVAL = 0x0FU,  /* KeyExchangeCalcpubVal Service */
    CRYPTO_KEYEXCHANGECALCSECRET = 0x10U,  /* KeyExchangeCalcSecret Service */
    CRYPTO_CERTIFICATEPARSE      = 0x11U,  /* CertificateParse Service */
    CRYPTO_CERTIFICATEVERIFY     = 0x12U,  /* CertificateVerify Service */
    CRYPTO_KEYSETVALID           = 0x13U   /* KeySetValid Service */
} Crypto_ServiceInfoType;

/* The family of the algorithm */
typedef enum
{
    CRYPTO_ALGOFAM_NOT_SET                      = 0x00U,
    CRYPTO_ALGOFAM_SHA1                         = 0x01U,
    CRYPTO_ALGOFAM_SHA2_224                     = 0x02U,
    CRYPTO_ALGOFAM_SHA2_256                     = 0x03U,
    CRYPTO_ALGOFAM_SHA2_384                     = 0x04U,
    CRYPTO_ALGOFAM_SHA2_512                     = 0x05U,
    CRYPTO_ALGOFAM_SHA2_512_224                 = 0x06U,
    CRYPTO_ALGOFAM_SHA2_512_256                 = 0x07U,
    CRYPTO_ALGOFAM_SHA3_224                     = 0x08U,
    CRYPTO_ALGOFAM_SHA3_256                     = 0x09U,
    CRYPTO_ALGOFAM_SHA3_384                     = 0x0AU,
    CRYPTO_ALGOFAM_SHA3_512                     = 0x0BU,
    CRYPTO_ALGOFAM_SHAKE128                     = 0x0CU,
    CRYPTO_ALGOFAM_SHAKE256                     = 0x0DU,
    CRYPTO_ALGOFAM_RIPEMD160                    = 0x0EU,
    CRYPTO_ALGOFAM_BLAKE_1_256                  = 0x0FU,
    CRYPTO_ALGOFAM_BLAKE_1_512                  = 0x10U,
    CRYPTO_ALGOFAM_BLAKE_2s_256                 = 0x11U,
    CRYPTO_ALGOFAM_BLAKE_2s_512                 = 0x12U,
    CRYPTO_ALGOFAM_3DES                         = 0x13U,
    CRYPTO_ALGOFAM_AES                          = 0x14U,
    CRYPTO_ALGOFAM_CHACHA                       = 0x15U,
    CRYPTO_ALGOFAM_RSA                          = 0x16U,
    CRYPTO_ALGOFAM_ED25519                      = 0x17U,
    CRYPTO_ALGOFAM_BRAINPOOL                    = 0x18U,
    CRYPTO_ALGOFAM_ECCNIST                      = 0x19U,
    CRYPTO_ALGOFAM_RNG                          = 0x1BU,
    CRYPTO_ALGOFAM_SIPHASH                      = 0x1CU,
    CRYPTO_ALGOFAM_ECIES                        = 0x1DU,
    CRYPTO_ALGOFAM_ECCANSI                      = 0x1EU,
    CRYPTO_ALGOFAM_ECCSEC                       = 0x1FU,
    CRYPTO_ALGOFAM_DRBG                         = 0x20U,
    CRYPTO_ALGOFAM_FIPS186                      = 0x21U,
    CRYPTO_ALGOFAM_PADDING_PKCS7                = 0x22U,
    CRYPTO_ALGOFAM_PADDING_ONEWITHZEROS         = 0x23U,
    CRYPTO_ALGOFAM_PBKDF2                       = 0x24U,
    CRYPTO_ALGOFAM_KDFX963                      = 0x25U,
    CRYPTO_ALGOFAM_DH                           = 0x26U,
    CRYPTO_ALGOFAM_CUSTOM                       = 0xFFU
} Crypto_AlgorithmFamilyType;

/* Structure containing input and output information depending on the job and the crypto primitive. */
typedef struct
{
    const uint8*             inputPtr;                  /* Pointer to the input data. */
    uint32                   inputLength;               /* Contains the input length in bytes. */
    const uint8*             secondaryInputPtr;         /* Pointer to the secondary input data (for MacVerify, SignatureVerify). */
    uint32                   secondaryInputLength;      /* Contains the secondary input length in bytes. */
    const uint8*             tertiaryInputPtr;          /* Pointer to the tertiary input data (for MacVerify, SignatureVerify). */
    uint32                   tertiaryInputLength;       /* Contains the tertiary input length in bytes. */
    uint8*                   outputPtr;                 /* Pointer to the output data. */
    uint32*                  outputLengthPtr;           /* Holds a pointer to a memory location containing the output length in bytes. */
    uint8*                   secondaryOutputPtr;        /* Pointer to the secondary output data. */
    uint32*                  secondaryOutputLengthPtr;  /* Holds a pointer to a memory location containing the secondary output length in bytes. */
    uint64                   input64;                   /* Versatile input parameter */
    Crypto_VerifyResultType* verifyPtr;                 /* Output pointer to a memory location holding a Crypto_VerifyResultType */
    uint64*                  output64Ptr;               /* Output pointer to a memory location holding an uint64. */
    Crypto_OperationModeType mode;                      /* Indicator of the mode(s)/operation(s) to be performed */
    uint32                   cryIfKeyId;                /* Holds the CryIf key id for key operation services. */
    uint32                   targetCryIfKeyId;          /* Holds the target CryIf key id for key operation services. */
} Crypto_JobPrimitiveInputOutputType;

/* Structure which determines the exact algorithm. Note, not every algorithm needs to specify all fields. AUTOSAR shall only allow valid combinations. */
typedef struct
{
    Crypto_AlgorithmFamilyType family;             /* The family of the algorithm */
    Crypto_AlgorithmFamilyType secondaryFamily;    /* The secondary family of the algorithm  */
    uint32                     keyLength;          /* The key length in bits to be used with that algorithm */
    Crypto_AlgorithmModeType   mode;               /* The operation mode to be used with that algorithm */
} Crypto_AlgorithmInfoType;

/* Structure which contains basic information about the crypto primitive. */
typedef struct
{
    const uint32 resultLength;                  /* Contains the result length in bytes. */
    const Crypto_ServiceInfoType service;       /* Contains the enum of the used service, e.g. Encrypt */
    const Crypto_AlgorithmInfoType algorithm;   /* Contains the information of the used algorithm */
} Crypto_PrimitiveInfoType;

/* Enumeration of the processing type. */
typedef enum
{
    CRYPTO_PROCESSING_ASYNC = 0x00U,    /* Asynchronous job processing */
    CRYPTO_PROCESSING_SYNC  = 0x01U     /* Synchronous job processing */
} Crypto_ProcessingType;

/* Structure which contains further information, which depends on the job and the crypto primitive. */
typedef struct
{
    uint32                          callbackId;                    /* Identifier of the callback function, to be called, if the configured service finished. */
    const Crypto_PrimitiveInfoType* primitiveInfo;                 /* Pointer to a structure containing further configuration of the crypto primitives */
    uint32                          cryIfKeyId;                    /* Identifier of the CryIf key. */
    Crypto_ProcessingType           processingType;                /* Determines the synchronous or asynchronous behavior. */
    boolean                         callbackUpdateNotification;    /* Indicates, whether the callback function shall be called, if the UPDATE operation has finished. */
} Crypto_JobPrimitiveInfoType;

/* Defines which of the input/output parameters are re-directed to a key element. The values can be combined to define a bit field. */
typedef enum
{
    CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT    = 0x01,
    CRYPTO_REDIRECT_CONFIG_SECONDARY_INPUT  = 0x02,
    CRYPTO_REDIRECT_CONFIG_TERTIARY_INPUT   = 0x04,
    CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT   = 0x10,
    CRYPTO_REDIRECT_CONFIG_SECONDARY_OUTPUT = 0x20
} Crypto_InputOutputRedirectionConfigType;

/* Structure which holds the identifiers of the keys and key elements which shall be used as input and output for a job and a bit structure which indicates which buffers shall be redirected to those key elements. */
typedef struct
{
    uint8  redirectionConfig;                  /* Bit structure which indicates which buffer shall be redirected to a key element. Values from Crypto_InputOutputRedirectionConfigType can be used and combined with unary OR operation. */
    uint32 inputKeyId;                         /* Identifier of the key which shall be used as input */
    uint32 inputKeyElementId;                  /* Identifier of the key element which shall be used as input */
    uint32 secondaryInputKeyId;                /* Identifier of the key which shall be used as secondary input */
    uint32 secondaryInputKeyElementId;         /* Identifier of the key element which shall be used as secondary input */
    uint32 tertiaryInputKeyId;                 /* Identifier of the key which shall be used as tertiary input */
    uint32 tertiaryInputKeyElementId;          /* Identifier of the key element which shall be used as tertiary input */
    uint32 outputKeyId;                        /* Identifier of the key which shall be used as output */
    uint32 outputKeyElementId;                 /* Identifier of the key element which shall be used as output */
    uint32 secondaryOutputKeyId;               /* Identifier of the key which shall be used as secondary output */
    uint32 secondaryOutputKeyElementId;        /* Identifier of the key element which shall be used as secondary output */
} Crypto_JobRedirectionInfoType;

/* Structure which contains further information, which depends on the job and the crypto primitive  */
typedef struct
{
    uint32                             jobId;                       /* Identifier for the job structure */
    Crypto_JobStateType                jobState;                    /* Determines the current job state */
    Crypto_JobPrimitiveInputOutputType jobPrimitiveInputOutput;     /* Structure containing input and output information depending on the job and the crypto primitive */
    const Crypto_JobPrimitiveInfoType* jobPrimitiveInfo;            /* Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    const Crypto_JobInfoType*          jobInfo;                     /* Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    Crypto_JobRedirectionInfoType*     jobRedirectionInfoRef;       /* Pointer to a structure containing further information on the usage of keys as input and output for jobs. */
    uint32                             cryptoKeyId;                 /* Identifier of the Crypto Driver key. The identifier shall be written by the Crypto Interface */
} Crypto_JobType;

/*==================================================================================================
*                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                     FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CSM_TYPES_H */
