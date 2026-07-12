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

/**
*   @file
*
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
#include "Crypto.h"
#include "hse_interface.h"
#include "Crypto_Hse.h"
#include "Crypto_Private.h"
#include "Crypto_Util.h"
#include "Hse_Ip.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CRYPTO_VENDOR_ID_HSE_C                    43
#define CRYPTO_AR_RELEASE_MAJOR_VERSION_HSE_C     4
#define CRYPTO_AR_RELEASE_MINOR_VERSION_HSE_C     4
#define CRYPTO_AR_RELEASE_REVISION_VERSION_HSE_C  0
#define CRYPTO_SW_MAJOR_VERSION_HSE_C             2
#define CRYPTO_SW_MINOR_VERSION_HSE_C             0
#define CRYPTO_SW_PATCH_VERSION_HSE_C             1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if Crypto Hse source file and Crypto header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_HSE_C != CRYPTO_VENDOR_ID)
    #error "Crypto_Hse.c and Crypto.h have different vendor ids"
#endif

/* Check if Crypto Hse source file and Crypto header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_HSE_C    != CRYPTO_AR_RELEASE_MAJOR_VERSION) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_HSE_C    != CRYPTO_AR_RELEASE_MINOR_VERSION) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_HSE_C != CRYPTO_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Crypto_Hse.c and Crypto.h are different"
#endif

/* Check if Crypto Hse source file and Crypto header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_HSE_C != CRYPTO_SW_MAJOR_VERSION) || \
     (CRYPTO_SW_MINOR_VERSION_HSE_C != CRYPTO_SW_MINOR_VERSION) || \
     (CRYPTO_SW_PATCH_VERSION_HSE_C != CRYPTO_SW_PATCH_VERSION)    \
    )
    #error "Software Version Numbers of Crypto_Hse.c and Crypto.h are different"
#endif


/* Check if Crypto Hse source file and Crypto Hse header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_HSE_C != CRYPTO_VENDOR_ID_HSE_H)
    #error "Crypto_Hse.c and Crypto_Hse.h have different vendor ids"
#endif

/* Check if Crypto Hse source file and Crypto Hse header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_HSE_C    != CRYPTO_AR_RELEASE_MAJOR_VERSION_HSE_H) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_HSE_C    != CRYPTO_AR_RELEASE_MINOR_VERSION_HSE_H) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_HSE_C != CRYPTO_AR_RELEASE_REVISION_VERSION_HSE_H) \
    )
    #error "AutoSar Version Numbers of Crypto_Hse.c and Crypto_Hse.h are different"
#endif

/* Check if Crypto Hse source file and Crypto Hse header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_HSE_C != CRYPTO_SW_MAJOR_VERSION_HSE_H) || \
     (CRYPTO_SW_MINOR_VERSION_HSE_C != CRYPTO_SW_MINOR_VERSION_HSE_H) || \
     (CRYPTO_SW_PATCH_VERSION_HSE_C != CRYPTO_SW_PATCH_VERSION_HSE_H)    \
    )
    #error "Software Version Numbers of Crypto_Hse.c and Crypto_Hse.h are different"
#endif


/* Check if Crypto Hse source file and Crypto Private header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_HSE_C != CRYPTO_VENDOR_ID_PRIVATE)
    #error "Crypto_Hse.c and Crypto_Private.h have different vendor ids"
#endif

/* Check if Crypto Hse source file and Crypto Private header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_HSE_C    != CRYPTO_AR_RELEASE_MAJOR_VERSION_PRIVATE) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_HSE_C    != CRYPTO_AR_RELEASE_MINOR_VERSION_PRIVATE) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_HSE_C != CRYPTO_AR_RELEASE_REVISION_VERSION_PRIVATE) \
    )
    #error "AutoSar Version Numbers of Crypto_Hse.c and Crypto_Private.h are different"
#endif

/* Check if Crypto Hse source file and Crypto Private header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_HSE_C != CRYPTO_SW_MAJOR_VERSION_PRIVATE) || \
     (CRYPTO_SW_MINOR_VERSION_HSE_C != CRYPTO_SW_MINOR_VERSION_PRIVATE) || \
     (CRYPTO_SW_PATCH_VERSION_HSE_C != CRYPTO_SW_PATCH_VERSION_PRIVATE)    \
    )
    #error "Software Version Numbers of Crypto_Hse.c and Crypto_Private.h are different"
#endif


/* Check if Crypto Hse source file and Crypto_Util header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_HSE_C != CRYPTO_UTIL_VENDOR_ID)
    #error "Crypto_Hse.c and Crypto_Util.h have different vendor ids"
#endif

/* Check if Crypto Hse source file and Crypto_Util header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_HSE_C    != CRYPTO_UTIL_AR_RELEASE_MAJOR_VERSION) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_HSE_C    != CRYPTO_UTIL_AR_RELEASE_MINOR_VERSION) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_HSE_C != CRYPTO_UTIL_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Crypto_Hse.c and Crypto_Util.h are different"
#endif

/* Check if Crypto Hse source file and Crypto_Util header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_HSE_C != CRYPTO_UTIL_SW_MAJOR_VERSION) || \
     (CRYPTO_SW_MINOR_VERSION_HSE_C != CRYPTO_UTIL_SW_MINOR_VERSION) || \
     (CRYPTO_SW_PATCH_VERSION_HSE_C != CRYPTO_UTIL_SW_PATCH_VERSION)    \
    )
    #error "Software Version Numbers of Crypto_Hse.c and Crypto_Util.h are different"
#endif


/* Check if Crypto Hse source file and Hse_Ip header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_HSE_C != HSE_IP_VENDOR_ID_H)
    #error "Crypto_Hse.c and Hse_Ip.h have different vendor ids"
#endif

/* Check if source file and Hse_Ip header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_HSE_C != HSE_IP_SW_MAJOR_VERSION_H) || \
     (CRYPTO_SW_MINOR_VERSION_HSE_C != HSE_IP_SW_MINOR_VERSION_H) || \
     (CRYPTO_SW_PATCH_VERSION_HSE_C != HSE_IP_SW_PATCH_VERSION_H)    \
    )
    #error "Software Version Numbers of Crypto_Hse.c and Hse_Ip.h are different"
#endif

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/
/* This is needed in order to have the CancelJob work correctly even when trying to launch a Crypto_CancelJob() for a job with jobId = 0 that was never launched */
#define CRYPTO_INVALID_VAL_FOR_JOBID_U32                ((uint32)0xA5AA5A55U)

/* Value used to mark that there is no stream available */
#define CRYPTO_INVALID_STREAM_ID_U8                     ((uint8)0xFFU)

#if (STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT)
    /* Curve ID was not configured by user */
    #define CRYPTO_KE_INVALID_CURVE_ID_U8               ((uint8)0xFFU)
#endif /* (STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT) */

#if (STD_ON == CRYPTO_KEYS_EXIST)
    #if (STD_ON == CRYPTO_SPT_RSA)
        #define RSA_MOD_LEN                                 (256U)
    #endif /* (STD_ON == CRYPTO_SPT_RSA) */

    #if (STD_ON == CRYPTO_SPT_ECC)
        /* Size of Ecc Nist 256 OID */
        #define CRYPTO_NIST_SECP256_SIZE_U8                 ((uint8)0x08)

        /* Size of Ecc Nist 384 and 521 OID */
        #define CRYPTO_NIST_SECP_384_521_SIZE_U8            ((uint8)0x05)

        /* Size of Brainpool OID Version  */
        #define CRYPTO_BRAINPOOL_OID_VERSION_SIZE_U8        ((uint8)0x08)

        /* Size of Brainpool OID Object identifier */
        #define CRYPTO_BRAINPOOL_OID_OBJ_ID_SIZE_U8         ((uint8)0x09)

        /* Object identifier for brainpoolP256r1 */
        #define CRYPTO_BRAINPOOLP256R1_OBJECT_ID_U8         ((uint8)0x07U)

        /* Object identifier for brainpoolP320r1 */
        #define CRYPTO_BRAINPOOLP320R1_OBJECT_ID_U8         ((uint8)0x09U)

        /* Object identifier for brainpoolP384r1 */
        #define CRYPTO_BRAINPOOLP384R1_OBJECT_ID_U8         ((uint8)0x0BU)

        /* Object identifier for brainpoolP512r1 */
        #define CRYPTO_BRAINPOOLP512R1_OBJECT_ID_U8         ((uint8)0x0DU)

        /* Size of Ecc ED25519 OID */
        #define CRYPTO_25519_ED25519_SIZE_U8                ((uint8)0x09U)

        /* Size of Ecc CURVE25519 OID */
        #define CRYPTO_25519_CURVE25519_SIZE_U8             ((uint8)0x0AU)

        /* Size of Ecc ED448 and CURVE448 OID */
        #define CRYPTO_448_ED448_CURVE448_SIZE_U8           ((uint8)0x03U)
    #endif /* (STD_ON == CRYPTO_SPT_ECC) */
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

#if (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT)
    /* Size of the TLS1.2 master key in bytes */
    #define CRYPTO_TLS12_MASTER_KEY_SIZE                (48U)
    /* Size of the TLS1.2 'master secret' label in bytes */
    #define CRYPTO_TLS12_LABEL_MASTER_SECRET_SIZE       (13U)
    /* Size of the TLS1.2 'key expansion' label in bytes */
    #define CRYPTO_TLS12_LABEL_KEY_EXPANSION_SIZE       (13U)
    /* Size of the TLS1.2 'client finished' label in bytes */
    #define CRYPTO_TLS12_LABEL_CLIENT_FINISHED_SIZE     (15U)
    /* Size of the TLS1.2 'server finished' label in bytes */
    #define CRYPTO_TLS12_LABEL_SERVER_FINISHED_SIZE     (15U)
#endif /* (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT) */
#if (STD_ON == CRYPTO_SPT_KEY_DERIVE)
    /* Index of the byte storing the Secondary Algorithm value in the buffer of the CRYPTO_KE_KEYDERIVATION_ALGORITHM_SECONDARY key element */
    #define CRYPTO_ALGORITHM_SECONDARY_IDX        (0U)
#endif /* (STD_ON == CRYPTO_SPT_KEY_DERIVE) */

#define Crypto_IsBitSet(var, bit)                       (0U != ((var) & (1U << (bit))))

#if ((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC))
    /* Function like macro for computing the number of bytes needed to store a given number of bits */
    #define Crypto_GetByteLenFromBitLen(len)            ((uint32)(((len) + (uint32)7U) / (uint32)8U))
#endif /* ((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC)) */

#if ((STD_ON == CRYPTO_SPT_SYM_KEY_IMPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT) || (STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_KEY_GENERATE) || (STD_ON == CRYPTO_SPT_KEY_DERIVE))
    /* Function like macro for converting byte length into bit length */
    #define Crypto_GetBitLenFromByteLen(len)            ((len) << 3U)
#endif /* ((STD_ON == CRYPTO_SPT_SYM_KEY_IMPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT) || (STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_KEY_GENERATE) || (STD_ON == CRYPTO_SPT_KEY_DERIVE)) */

/* Channel 0 of the Messaging Unit instance */
#define CRYPTO_CHANNEL_0_U8                             ((uint8)0U)

/* Mask for FINISH operation mode flag */
#define CRYPTO_OPERATIONMODE_FINISH_MASK_U8             ((uint8)0x04U)

#if (STD_ON == CRYPTO_SPT_SHE)
    /* Key handle of the SHE MASTER ECU key */
    #define CRYPTO_SHE_MASTER_ECU_KEY_HANDLE_U32        ((uint32)0x00010000U)
#endif /* (STD_ON == CRYPTO_SPT_SHE) */


/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
/* State structure for Crypto_Hse layer storing variables that should be kept per MU instance */
typedef struct
{
    Hse_Ip_MuStateType  Hse_Ip_MuState;                                  /* Structure storing HseIp internal variables */
    Hse_Ip_ReqType      Hse_Ip_aRequest[HSE_NUM_OF_CHANNELS_PER_MU];     /* Array of requests to Hse Ip layer that must be kept available between asynchronous calls */
    uint32              au32MuChannelJobId[HSE_NUM_OF_CHANNELS_PER_MU];  /* Array containing the jobIds of each job that runs on a MU channel */
    uint32              au32MuChannelCDOIdx[HSE_NUM_OF_CHANNELS_PER_MU]; /* Array storing the index of the CDO where the job came from */
    uint32              u32HseSyncRequestsTimeout;                       /* Variable storing the timeout for synchronous requests sent to HSE in ticks */
    hseSrvDescriptor_t* pHseSrvDescriptor;                               /* Pointer where the statically allocated memory for Hse service descriptors placed in SHARED RAM begins */
    uint32              aStreamJobId[HSE_STREAM_COUNT];                  /* Array containing each job id per stream */
    uint8               u8StreamBusyBitMap;                              /* Stores the stream status */
#if (STD_ON == CRYPTO_KEYS_EXIST)
    hseKeyInfo_t        HseKeyInfoStorage;                               /* Structure containing key properties used by functions to get or provide the key information when requesting services from HSE */
    uint32              au32OutLength[HSE_NUM_OF_CHANNELS_PER_MU];       /* Array that will be filled with the length information from user output */
    #if (STD_ON == CRYPTO_SPT_ECC)
    uint8               au8KeyBigEndForm[200];                           /* Temporary buffer used for endianness conversion */
    #endif /* (STD_ON == CRYPTO_SPT_ECC) */
    #if (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT)
    uint16              au16AuthResultSize[2U];                          /* Size of the authentication tag or signature given by HSE */
        #if (STD_ON == CRYPTO_SPT_RSA)
    uint32              au32SizeRsa[2U];                                 /* RSA key size */
        #endif /* (STD_ON == CRYPTO_SPT_RSA) */
    #endif /* (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) */
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */
} Crypto_Hse_MuStateType;

#if (STD_ON == CRYPTO_SPT_HASH)
    /* Structure used to store information about Hash algo */
    typedef struct
    {
        hseHashAlgo_t HseHashAlgo;
        uint32        u32HashLength;
    } Crypto_Hse_HashConfigType;
#endif /* (STD_ON == CRYPTO_SPT_HASH) */

#if (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT)
    typedef struct
    {
        hseKeyInfo_t HseKeyInfoContainer;
        uint8        aKey[512U];
    } Crypto_ContainerType;
#endif /* (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) */

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/
#define CRYPTO_START_SEC_CONST_8
#include "Crypto_MemMap.h"

#if (STD_ON == CRYPTO_KEYS_EXIST)
    #if (STD_ON == CRYPTO_SPT_KEY_DERIVE)
        #if (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT)
            /* Array of bytes storing the TLS1.2 'master secret' label */
            static const uint8 Crypto_au8Tls12LabelMasterSecret[CRYPTO_TLS12_LABEL_MASTER_SECRET_SIZE]     = { 'm', 'a', 's', 't', 'e', 'r', ' ', 's', 'e', 'c', 'r', 'e', 't' };
            /* Array of bytes storing the TLS1.2 'key expansion' label */
            static const uint8 Crypto_au8Tls12LabelKeyExpansion[CRYPTO_TLS12_LABEL_KEY_EXPANSION_SIZE]     = { 'k', 'e', 'y', ' ', 'e', 'x', 'p', 'a', 'n', 's', 'i', 'o', 'n' };
            /* Array of bytes storing the TLS1.2 'client finished' label */
            static const uint8 Crypto_au8Tls12LabelClientFinished[CRYPTO_TLS12_LABEL_CLIENT_FINISHED_SIZE] = { 'c', 'l', 'i', 'e', 'n', 't', ' ', 'f', 'i', 'n', 'i', 's', 'h', 'e', 'd' };
            /* Array of bytes storing the TLS1.2 'server finished' label */
            static const uint8 Crypto_au8Tls12LabelServerFinished[CRYPTO_TLS12_LABEL_SERVER_FINISHED_SIZE] = { 's', 'e', 'r', 'v', 'e', 'r', ' ', 'f', 'i', 'n', 'i', 's', 'h', 'e', 'd' };
        #endif /* (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT) */
    #endif /* (STD_ON == CRYPTO_SPT_KEY_DERIVE) */
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

#define CRYPTO_STOP_SEC_CONST_8
#include "Crypto_MemMap.h"

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/
#define CRYPTO_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Crypto_MemMap.h"

/* Array storing the Crypto_Hse layer per MU state machine structures */
static Crypto_Hse_MuStateType Crypto_aCryptoHseMuState[CRYPTO_NUMBER_PARTITIONS_ALLOWED_TO_RUN_IN_U8];

#define CRYPTO_STOP_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Crypto_MemMap.h"


#define CRYPTO_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_MemMap.h"

/* Array of pointers to Crypto_Hse layer state structures, one per MU instance */
static Crypto_Hse_MuStateType* Crypto_Hse_apMuState[HSE_NUM_OF_MU_INSTANCES];

#define CRYPTO_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_MemMap.h"


#define CRYPTO_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_MemMap.h"

/* Array containing one pointer per Crypto Driver Object, storing the currently processing synchronous job */
static Crypto_JobType* Crypto_apSyncJobInProgress[CRYPTO_NUMBER_OF_DRIVER_OBJECTS_U32];

#define CRYPTO_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_MemMap.h"


#define CRYPTO_START_SEC_VAR_SHARED_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Crypto_MemMap.h"

static hseSrvDescriptor_t Crypto_aHseSrvDescriptor[CRYPTO_NUMBER_PARTITIONS_ALLOWED_TO_RUN_IN_U8][HSE_NUM_OF_CHANNELS_PER_MU];

#define CRYPTO_STOP_SEC_VAR_SHARED_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Crypto_MemMap.h"

#define CRYPTO_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Crypto_MemMap.h"

#if (STD_ON == CRYPTO_KEYS_EXIST)
    #if (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT)
        static Crypto_ContainerType Crypto_xContainer;
    #endif /* (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) */
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

#define CRYPTO_STOP_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Crypto_MemMap.h"



/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#define CRYPTO_START_SEC_CODE
#include "Crypto_MemMap.h"

static void Crypto_Hse_FillVerifyPtr
(
    const Crypto_JobType* pCryptoJob,
    const Std_ReturnType  StdRetVal
);
#if ((STD_ON == CRYPTO_KEYS_EXIST) || (STD_ON == CRYPTO_SPT_HASH))

static inline hseAccessMode_t Crypto_Hse_ConfigureAccessMode
(
    const Crypto_OperationModeType eAccessMode
);
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) || (STD_ON == CRYPTO_SPT_HASH)) */

#if (STD_ON == CRYPTO_KEYS_EXIST)
#if (STD_ON == CRYPTO_SPT_CMAC) || (STD_ON == CRYPTO_SPT_XCBCMAC)
static inline Std_ReturnType Crypto_Hse_CmacXcbcMacGenerate
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
);

static inline Std_ReturnType Crypto_Hse_CmacXcbcMacVerify
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
);
#endif /* (STD_ON == CRYPTO_SPT_CMAC) || (STD_ON == CRYPTO_SPT_XCBCMAC) */

#if (STD_ON == CRYPTO_SPT_FAST_CMAC)
static inline Std_ReturnType Crypto_Hse_FastCmac
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    hseAuthDir_t          HseAuthDir,
    boolean*              pbReqSentOverChannel
);
#endif /* (STD_ON == CRYPTO_SPT_FAST_CMAC) */

#if (STD_ON == CRYPTO_SPT_GMAC)
static inline Std_ReturnType Crypto_Hse_GmacGenerate
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
);

static inline Std_ReturnType Crypto_Hse_GmacVerify
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
);
#endif /* (STD_ON == CRYPTO_SPT_GMAC) */

#if (STD_ON == CRYPTO_SPT_AEAD)
static inline Std_ReturnType Crypto_Hse_AeadEncDec
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    hseCipherDir_t        CipherDir,
    boolean*              pbReqSentOverChannel
);
#endif /* (STD_ON == CRYPTO_SPT_AEAD) */

static inline Std_ReturnType Crypto_Hse_EncDec
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    hseCipherDir_t        u8CipherDir,
    boolean*              pbReqSentOverChannel
);

#if (STD_ON == CRYPTO_SPT_HMAC)
static inline Std_ReturnType Crypto_Hse_HmacGen
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    hseHashAlgo_t         HashAlgo,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
);

static inline Std_ReturnType Crypto_Hse_HmacVerify
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    hseHashAlgo_t         HashAlgo,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
);
#endif /* (STD_ON == CRYPTO_SPT_HMAC) */

static inline hseCipherBlockMode_t Crypto_Hse_ConfigureAlgoMode
(
    uint8 u8AlgoMode
);

#if (STD_ON == CRYPTO_SPT_RSA)
static inline Std_ReturnType Crypto_Hse_RSAEncDec
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    hseCipherDir_t        CipherDir,
    boolean*              pbReqSentOverChannel
);
#endif /* (STD_ON == CRYPTO_SPT_RSA) */

#if (STD_ON == CRYPTO_SPT_SYM_KEY_IMPORT)
static inline Std_ReturnType Crypto_Hse_LoadSimKey
(
    uint8               u8MuInstance,
    uint8               u8MuChannel,
    hseSrvDescriptor_t* pHseSrvDescriptor,
    uint32              u32KeyLength,
    const uint8*        pKeyMaterial,
    uint32              u32KeyMaterialKeyElemIdx
);
#endif /* (STD_ON == CRYPTO_SPT_SYM_KEY_IMPORT) */

#if (STD_ON == CRYPTO_SPT_SHE_KEY_IMPORT)
static inline Std_ReturnType Crypto_Hse_SheLoadPlainKey
(
    uint8               u8MuInstance,
    uint8               u8MuChannel,
    hseSrvDescriptor_t* pHseSrvDescriptor,
    const uint8*        pKeyMaterial
);

static inline Std_ReturnType Crypto_Hse_SheLoadKey
(
    uint8               u8MuInstance,
    uint8               u8MuChannel,
    hseSrvDescriptor_t* pHseSrvDescriptor,
    const uint8*        pKeyMaterial,
    uint32              u32MacProofKeyElemIdx,
    uint32              u32CipherProofKeyElemIdx,
    uint32              u32KeyMaterialKeyElemIdx
);
#endif /* (STD_ON == CRYPTO_SPT_SHE_KEY_IMPORT) */

#if ((STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT))
static inline Std_ReturnType Crypto_Hse_ExportSymPrivAsymPub
(
    uint8               u8MuInstance,
    uint8               u8MuChannel,
    hseSrvDescriptor_t* pHseSrvDescriptor,
    uint32              u32KeyMaterialKeyElemIdx,
    uint32              u32KeyExportKeyElemIdx,
    uint8* const        pResult,
#if (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT)
    uint32*             pResultLength
#else
    const uint32*       pResultLength
#endif /* (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) */
);
#endif /* ((STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT)) */

#if ((STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) && (STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT))
static inline void Crypto_Hse_ExportCipherScheme
(
    hseCipherScheme_t*                   pCipherScheme,
    const Crypto_ExportCipherSchemeType* pExportCipherScheme
);
#endif /* ((STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) && (STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT)) */

#if ((STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) && ((STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT)))
static inline void Crypto_Hse_ExportAuthScheme
(
    hseExportKeySrv_t* pExportKeyReq,
    uint32             u32KeyMaterialKeyElemIdx,
    uint32             u32KeyExportKeyElemIdx,
    uint8              u8MuInstance
);
#endif /* ((STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) && ((STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT))) */

#if ((STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT))
static inline Std_ReturnType Crypto_Hse_ExportResultAddr
(
    hseExportKeySrv_t*    pExportKeyReq,
    uint8                 u8HseKeyType,
    const uint8* const    pResult,
    const uint32*         pResultLength
#if (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) && (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT)
    , const uint32       u32KeyHandle,
    uint8                u8MuInstance
#endif /* (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) && (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) */
);
#endif /* ((STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT)) */


#if (STD_ON == CRYPTO_SPT_SHE_KEY_EXPORT)
static inline Std_ReturnType Crypto_Hse_ExportRamSheKey
(
    uint8               u8MuInstance,
    uint8               u8MuChannel,
    hseSrvDescriptor_t* pHseSrvDescriptor,
    const uint8*        pResult
);
#endif /* (STD_ON == CRYPTO_SPT_SHE_KEY_EXPORT) */

#if ((STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_PAIR_GEN))
static inline uint16 Crypto_Hse_EccKeyBitLen
(
    uint16 u16CurveId
);
#endif /* ((STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_PAIR_GEN)) */

#if (STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT)
static inline Std_ReturnType Crypto_Hse_EccLoadPlainPubKey
(
    uint8               u8MuInstance,
    uint8               u8MuChannel,
    hseSrvDescriptor_t* pHseSrvDescriptor,
    const uint8*        pKey,
    uint32              u32KeyLength,
    uint32              u32KeyMaterialKeyElemIdx,
    uint8               u8CurveId
);

static inline Std_ReturnType Crypto_Hse_EccLoadPlainPairKey
(
    uint8               u8MuInstance,
    uint8               u8MuChannel,
    hseSrvDescriptor_t* pHseSrvDescriptor,
    const               uint8* pKey,
    uint32              u32KeyLength,
    uint32              u32KeyMaterialKeyElemIdx,
    uint8               u8CurveId
);
#endif /* (STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT) */

#if ((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC))
static inline Std_ReturnType Crypto_Hse_SignRequest
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
);

static inline boolean Crypto_Hse_FillSignRequestDescriptor
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSignSrv_t*         pSignSrv,
    const Crypto_JobType* pJob,
    const uint32          u32SignKeyElemIdx
);
#endif /* ((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC)) */

#if (STD_ON == CRYPTO_SPT_KEY_GENERATE)
static Std_ReturnType Crypto_Hse_KeyGenerateRequest
(
    const uint8           u8MuInstance,
    const uint8           u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    const uint32          u32CryptoKeyIdx,
    boolean*              pbReqSentOverChannel
);
#endif /* (STD_ON == CRYPTO_SPT_KEY_GENERATE) */

#if (STD_ON == CRYPTO_SPT_KEY_DERIVE)
static Std_ReturnType Crypto_Hse_KeyDeriveRequest
(
    const uint8           u8MuInstance,
    const uint8           u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    const uint32          u32CryptoKeyIdx,
    const uint32          u32TargetKeyIdx,
    boolean*              pbReqSentOverChannel
);
#endif /* (STD_ON == CRYPTO_SPT_KEY_DERIVE) */

#if (((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC)) && (STD_ON == CRYPTO_SPT_COMPUTE_DH))
static Std_ReturnType Crypto_Hse_CalcSecretRequest
(
    const uint8           u8MuInstance,
    const uint8           u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    const uint32          u32CryptoKeyIdx,
    const uint8* const    pPartnerPublicValue,
    const uint32          u32PartnerPublicValueLength
);
#endif /* (((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC)) && (STD_ON == CRYPTO_SPT_COMPUTE_DH)) */

#if (STD_ON == CRYPTO_SPT_ECC_KEY_PAIR_GEN)
static inline uint8 Crypto_Hse_GetEccCurveId
(
    const uint32 u32EccCurveOidKeyElemIdx,
    const uint8  u8EccCurveAlgoFam
);

static inline uint8 Crypto_Hse_ConvertNistOidToCurveId
(
    const uint8* pCurveOid,
    const uint32 u32CurveOidLength
);

static inline uint8 Crypto_Hse_ConvertBrainPoolOidToCurveId
(
    const uint8* pCurveOid,
    const uint32 u32CurveOidLength
);

static inline uint8 Crypto_Hse_ConvertEd25519OidToCurveId
(
    const uint8* pCurveOid,
    const uint32 u32CurveOidLength
);

static inline uint8 Crypto_Hse_ConvertEd448OidToCurveId
(
    const uint8* pCurveOid,
    const uint32 u32CurveOidLength
);
#endif /* (STD_ON == CRYPTO_SPT_ECC_KEY_PAIR_GEN) */

#if ((STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT))
static inline Std_ReturnType Crypto_Hse_GetKeyInfo
(
    const uint8 u8MuInstance,
    uint32      u32HseKeyHandle
);
#endif /* ((STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT)) */

#if ((STD_ON == CRYPTO_SPT_SYM_KEY_IMPORT) || (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT))
static inline boolean Crypto_Hse_AuthCipherModeIsCcmOrGcm
(
    const uint8 u8AuthCipherMode
);
#endif /* ((STD_ON == CRYPTO_SPT_SYM_KEY_IMPORT) || (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT)) */
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

static inline Std_ReturnType Crypto_Hse_OperationModeControl
(
    Crypto_JobType* pJob
);

static inline uint8 Crypto_Hse_AllocStream
(
    const uint8           u8MuInstance,
    const Crypto_JobType* pJob
);

static inline void Crypto_Hse_FreeStream
(
    const uint8 u8MuInstance,
    uint32      u32JobId
);

#if (STD_ON == CRYPTO_SPT_HASH)
static inline Crypto_Hse_HashConfigType Crypto_Hse_ConfigureHashAlgo
(
    uint8 u8AlgoFamily
);

static inline Std_ReturnType Crypto_Hse_Hash
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
);
#endif /* (STD_ON == CRYPTO_SPT_HASH) */

#if ((STD_ON == CRYPTO_SPT_SIPHASH) && (STD_ON == CRYPTO_KEYS_EXIST))
static inline Std_ReturnType Crypto_Hse_SipHash
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    hseAuthDir_t          HseAuthDir,
    boolean*              pbReqSentOverChannel
);
#endif /* ((STD_ON == CRYPTO_SPT_SIPHASH) && (STD_ON == CRYPTO_KEYS_EXIST)) */

static inline Std_ReturnType Crypto_Hse_Random
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob
);

static inline Std_ReturnType Crypto_Hse_TranslateHseResponse
(
    hseSrvResponse_t HseResponse
);

static Std_ReturnType Crypto_Hse_ProcessOperation
(
    const uint8     u8MuInstance,
    uint32          u32ObjectIdx,
    Crypto_JobType* pJob
);


static inline Std_ReturnType Crypto_Hse_SendCancelJobReq
(
    uint8               u8MuInstance,
    uint8               u8MuChannel,
    hseSrvDescriptor_t* pHseSrvDescriptor,
    uint8               u8MuChannelToCancel
);

static void Crypto_Hse_ProcessMuChannelResponse
(
    uint8            u8MuInstance,
    uint8            u8MuChannel,
    hseSrvResponse_t HseResponse,
    void*            pCallbackParam
);

static Std_ReturnType Crypto_Hse_SendMsg
(
    const uint8                 u8MuInstance,
    const uint8                 u8MuChannel,
    hseSrvDescriptor_t* const   pHseSrvDescriptor,
    Crypto_JobType* const       pJob
);

static void Crypto_Hse_PushJobsFromQueuesToHse
(
    const uint8 u8MuInstance
);

static inline Std_ReturnType Crypto_Hse_ServiceRequest
(
    const uint8           u8MuInstance,
    const uint8           u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
);

#if ((STD_ON == CRYPTO_SPT_MAC) && (STD_ON == CRYPTO_KEYS_EXIST))
static inline Std_ReturnType Crypto_Hse_MacServiceRequest
(
    const uint8           u8MuInstance,
    const uint8           u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
);
#endif /* ((STD_ON == CRYPTO_SPT_MAC) && (STD_ON == CRYPTO_KEYS_EXIST)) */

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/
/**
* @brief           Fills the verifyPtr for Signature Verification, MAC Verify and AEAD Decrypt jobs
* @details         Fills the verifyPtr (CRYPTO_E_VER_OK or CRYPTO_E_VER_NOT_OK) for Signature Verification, MAC Verify and AEAD Decrypt jobs. This function is needed because the HSE descriptors do not provide such field (verifyPtr).
*
* @param[in]       pCryptoJob:   Pointer to Crypto job
* @param[in]       StdRetVal:    Std_ReturnType translated response received from HSE
*
* @returns         void
*/
static void Crypto_Hse_FillVerifyPtr
(
    const Crypto_JobType* pCryptoJob,
    const Std_ReturnType  StdRetVal
)
{
    Crypto_ServiceInfoType eCryptoService = pCryptoJob->jobPrimitiveInfo->primitiveInfo->service;

    if (
        (CRYPTO_MACVERIFY       == eCryptoService)
#if ((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC))
        ||
        (CRYPTO_SIGNATUREVERIFY == eCryptoService)
#endif /* ((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC)) */
#if (STD_ON == CRYPTO_SPT_AEAD)
        ||
        (CRYPTO_AEADDECRYPT     == eCryptoService)
#endif /* (STD_ON == CRYPTO_SPT_AEAD) */
       )
    {
        if ((Std_ReturnType)E_OK == StdRetVal)
        {
            *pCryptoJob->jobPrimitiveInputOutput.verifyPtr = CRYPTO_E_VER_OK;
        }
        else
        {
            *pCryptoJob->jobPrimitiveInputOutput.verifyPtr = CRYPTO_E_VER_NOT_OK;
        }
    }
}
#if ((STD_ON == CRYPTO_KEYS_EXIST) || (STD_ON == CRYPTO_SPT_HASH))

/**
* @brief           Configures the mode for HSE (start, update, finish, singlecall)
* @details         Configures the mode for HSE (start, update, finish, singlecall).
*
* @param[in]       eAccessMode: Mode needed by the job
*
* @returns         Corresponding mode of HSE
*                   HSE_ACCESS_MODE_START    - start mode configured
*                   HSE_ACCESS_MODE_UPDATE   - update mode configured
*                   HSE_ACCESS_MODE_FINISH   - finish mode configured
*                   HSE_ACCESS_MODE_ONE_PASS - singlecall operation configured
*/
static inline hseAccessMode_t Crypto_Hse_ConfigureAccessMode
(
    const Crypto_OperationModeType eAccessMode
)
{
    hseAccessMode_t RetVal;

    switch (eAccessMode)
    {
        case CRYPTO_OPERATIONMODE_SINGLECALL:
            RetVal = HSE_ACCESS_MODE_ONE_PASS;
            break;

        case CRYPTO_OPERATIONMODE_START:
            RetVal = HSE_ACCESS_MODE_START;
            break;

        case CRYPTO_OPERATIONMODE_UPDATE:
        /* fall-through */
        case CRYPTO_OPERATIONMODE_STREAMSTART:
            RetVal = HSE_ACCESS_MODE_UPDATE;
            break;

        case CRYPTO_OPERATIONMODE_FINISH:
            RetVal = HSE_ACCESS_MODE_FINISH;
            break;

        default:
            RetVal = HSE_ACCESS_MODE_ONE_PASS;
            break;
    }

    return RetVal;
}
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) || (STD_ON == CRYPTO_SPT_HASH)) */

/**
* @brief           Function that allocates job id to stream
* @details         Assign jobId to stream if it is available
*
* @param[in]       u8MuInstance:  MU instance used to send the request to HSE
* @param[in]       pJob:          Pointer to the job to be processed
*
* @returns         uint8: The index of the available stream or CRYPTO_INVALID_STREAM_ID_U8 if there are no available streams
*/
static inline uint8 Crypto_Hse_AllocStream
(
    const uint8           u8MuInstance,
    const Crypto_JobType* pJob
)
{
    uint8  u8Count;
    uint8  u8StreamId = CRYPTO_INVALID_STREAM_ID_U8;

    /* Crypto_Hse_apMuState[u8MuInstance]->u8StreamBusyBitMap is a global variable that is unique per each MU instance.
       For this reason we need a critical section in order to make atomic the operation of read-modify-write for this variable */
    SchM_Enter_Crypto_CRYPTO_EXCLUSIVE_AREA_03();

    for (u8Count = 0U; u8Count < HSE_STREAM_COUNT; u8Count++)
    {
        /* If there is any empty slot or if there is a slot with the same job id and the mode is START get the stream */
        if (!(Crypto_IsBitSet(Crypto_Hse_apMuState[u8MuInstance]->u8StreamBusyBitMap,u8Count))||
            (pJob->jobId == Crypto_Hse_apMuState[u8MuInstance]->aStreamJobId[u8Count])
           )
        {
            Crypto_Hse_apMuState[u8MuInstance]->aStreamJobId[u8Count] = (uint32)pJob->jobId;
            u8StreamId = u8Count;
            Crypto_Hse_apMuState[u8MuInstance]->u8StreamBusyBitMap |= ((uint8)1 << u8Count);
            break;
        }
    }

    SchM_Exit_Crypto_CRYPTO_EXCLUSIVE_AREA_03();

    return u8StreamId;
}

/**
* @brief           Function that clear stream slot status
* @details         Set stream slot status to not used
*
* @param[in]       u8MuInstance: MU instance used to send the request to HSE
* @param[in]       u32JobId:     Configured JobId
*
* @returns         void
*/
static inline void Crypto_Hse_FreeStream
(
    const uint8 u8MuInstance,
    uint32      u32JobId
)
{
    uint8 u8Count;

    /* Crypto_Hse_apMuState[u8MuInstance]->u8StreamBusyBitMap is a global variable that is unique per each MU instance.
       For this reason we need a critical section in order to make atomic the operation of read-modify-write for this variable */
    SchM_Enter_Crypto_CRYPTO_EXCLUSIVE_AREA_04();

    for (u8Count = 0U; u8Count < HSE_STREAM_COUNT; u8Count++)
    {
        if ((Crypto_IsBitSet(Crypto_Hse_apMuState[u8MuInstance]->u8StreamBusyBitMap,u8Count)) && ((uint32)u32JobId == Crypto_Hse_apMuState[u8MuInstance]->aStreamJobId[u8Count]))
        {
            Crypto_Hse_apMuState[u8MuInstance]->u8StreamBusyBitMap &=  (uint8)~((uint8)1 << u8Count);
            break;
        }
    }

    SchM_Exit_Crypto_CRYPTO_EXCLUSIVE_AREA_04();
}

#if (STD_ON == CRYPTO_KEYS_EXIST)
#if (STD_ON == CRYPTO_SPT_ECC_KEY_PAIR_GEN)
/**
* @brief           Convert Ecc Nist OID to Curve Id
* @details         This function will parse OID data and return the Ecc Curve Id
*
* @param[in]       pCurveOid         : Curve OID element array
* @param[in]       u32CurveOidLength : Curve OID Length
*
* @returns         uint8             : Curve Id value
*/
static inline uint8 Crypto_Hse_ConvertNistOidToCurveId
(
    const uint8* pCurveOid,
    const uint32 u32CurveOidLength
)
{
    uint8   u8RetEccCurveId = HSE_EC_CURVE_NONE;
    boolean bCompareResult;
#if (STD_ON == CRYPTO_SPT_SECP256R1)
    /*OID bytes for NIST P-256 standard curves */
    static const uint8 Crypto_au8EccNistSecp256Oid[CRYPTO_NIST_SECP256_SIZE_U8]        = {0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x07};
#endif /* (STD_ON == CRYPTO_SPT_SECP256R1) */

#if (STD_ON == CRYPTO_SPT_SECP384R1)
    /*OID bytes for NIST P-384 standard curves */
    static const uint8 Crypto_au8EccNistSecp384Oid[CRYPTO_NIST_SECP_384_521_SIZE_U8]   = {0x2B, 0x81, 0x04, 0x00,0x22};
#endif /* (STD_ON == CRYPTO_SPT_SECP384R1) */

#if (STD_ON == CRYPTO_SPT_SECP521R1)
    /*OID bytes for NIST P-521 standard curves */
    static const uint8 Crypto_au8EccNistSecp521Oid[CRYPTO_NIST_SECP_384_521_SIZE_U8]   = {0x2B, 0x81, 0x04, 0x00,0x22};
#endif /* (STD_ON == CRYPTO_SPT_SECP521R1) */

    switch (u32CurveOidLength)
    {
#if (STD_ON == CRYPTO_SPT_SECP256R1)
        case CRYPTO_NIST_SECP256_SIZE_U8:
            /* Check if the OID configured by user is equal to Nist Secp 256 OID */
            bCompareResult = Crypto_Util_MemCmp(pCurveOid, Crypto_au8EccNistSecp256Oid,u32CurveOidLength);

            if (TRUE == bCompareResult)
            {
                u8RetEccCurveId = HSE_EC_SEC_SECP256R1;
            }
            break;
#endif /* (STD_ON == CRYPTO_SPT_SECP256R1) */

#if (STD_ON == CRYPTO_SPT_SECP384R1) || (STD_ON == CRYPTO_SPT_SECP521R1)
        /* Assumption is that if SECP521R1 is enabled, SECP384R1 is for sure enabled */
        case CRYPTO_NIST_SECP_384_521_SIZE_U8:
            /* Check if the OID configured by user is equal to Nist Secp 384 OID */
            bCompareResult = Crypto_Util_MemCmp(pCurveOid, Crypto_au8EccNistSecp384Oid,u32CurveOidLength);

            if (TRUE == bCompareResult)
            {
                u8RetEccCurveId = HSE_EC_SEC_SECP384R1;
            }
#if (STD_ON == CRYPTO_SPT_SECP521R1)
            else
            {
                /* Check if the OID configured by user is equal to Nist Secp 521 OID */
                bCompareResult = Crypto_Util_MemCmp(pCurveOid, Crypto_au8EccNistSecp521Oid,u32CurveOidLength);

                if (TRUE == bCompareResult)
                {
                    u8RetEccCurveId = HSE_EC_SEC_SECP521R1;
                }
            }
#endif /* (STD_ON == CRYPTO_SPT_SECP521R1) */
            break;
#endif /* (STD_ON == CRYPTO_SPT_SECP384R1) || (STD_ON == CRYPTO_SPT_SECP521R1) */

        default:
            u8RetEccCurveId = HSE_EC_CURVE_NONE;
            break;
    }

    /* Avoid compiler warnings */
    (void)bCompareResult;

    return u8RetEccCurveId;
}

/**
* @brief           Convert Ecc BrainPool OID to Curve Id
* @details         This function will parse OID data and return the Ecc Curve Id
*
* @param[in]       pCurveOid         : Curve OID element array
* @param[in]       u32CurveOidLength : Curve OID Length
*
* @returns         uint8             : Curve Id value
*/
static inline uint8 Crypto_Hse_ConvertBrainPoolOidToCurveId
(
    const uint8* pCurveOid,
    const uint32 u32CurveOidLength
)
{
    uint8   u8RetEccCurveId = HSE_EC_CURVE_NONE;
#if ((STD_ON == CRYPTO_SPT_BRAINPOOLP256R1) || (STD_ON == CRYPTO_SPT_BRAINPOOLP320R1) || (STD_ON == CRYPTO_SPT_BRAINPOOLP384R1) || (STD_ON == CRYPTO_SPT_BRAINPOOLP512R1))
    boolean bCompareResult;
    uint8   u8VersionId;
    /* OID bytes for ECC Branipool standard curves */
    static const uint8 Crypto_au8EccBrainpoolOid[CRYPTO_BRAINPOOL_OID_VERSION_SIZE_U8] = {0x2B, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01};

    if (CRYPTO_BRAINPOOL_OID_OBJ_ID_SIZE_U8 == u32CurveOidLength)
    {
        /* Compare the first common 8 bytes from Brainpool OID */
        bCompareResult = Crypto_Util_MemCmp(pCurveOid, Crypto_au8EccBrainpoolOid, CRYPTO_BRAINPOOL_OID_VERSION_SIZE_U8);

        if (TRUE == bCompareResult)
        {
            /* Take last byte from Oid and set curve id */
            u8VersionId = pCurveOid[CRYPTO_BRAINPOOL_OID_VERSION_SIZE_U8];

            switch (u8VersionId)
            {
#if (STD_ON == CRYPTO_SPT_BRAINPOOLP256R1)
                case CRYPTO_BRAINPOOLP256R1_OBJECT_ID_U8:
                    u8RetEccCurveId = HSE_EC_BRAINPOOL_BRAINPOOLP256R1;
                    break;
#endif /* (STD_ON == CRYPTO_SPT_BRAINPOOLP256R1) */

#if (STD_ON == CRYPTO_SPT_BRAINPOOLP320R1)
                case CRYPTO_BRAINPOOLP320R1_OBJECT_ID_U8:
                    u8RetEccCurveId = HSE_EC_BRAINPOOL_BRAINPOOLP320R1;
                    break;
#endif /* (STD_ON == CRYPTO_SPT_BRAINPOOLP320R1) */

#if (STD_ON == CRYPTO_SPT_BRAINPOOLP384R1)
                case CRYPTO_BRAINPOOLP384R1_OBJECT_ID_U8:
                    u8RetEccCurveId = HSE_EC_BRAINPOOL_BRAINPOOLP384R1;
                    break;
#endif /* (STD_ON == CRYPTO_SPT_BRAINPOOLP384R1) */

#if (STD_ON == CRYPTO_SPT_BRAINPOOLP512R1)
                case CRYPTO_BRAINPOOLP512R1_OBJECT_ID_U8:
                    u8RetEccCurveId = HSE_EC_BRAINPOOL_BRAINPOOLP512R1;
                    break;
#endif /* (STD_ON == CRYPTO_SPT_BRAINPOOLP512R1) */

                default:
                    u8RetEccCurveId = HSE_EC_CURVE_NONE;
                    break;
            }
        }
    }
#else
     /* Avoid compiler warnings */
    (void)pCurveOid;
    (void)u32CurveOidLength;
#endif /* ((STD_ON == CRYPTO_SPT_BRAINPOOLP256R1) || (STD_ON == CRYPTO_SPT_BRAINPOOLP320R1) || (STD_ON == CRYPTO_SPT_BRAINPOOLP384R1) || (STD_ON == CRYPTO_SPT_BRAINPOOLP512R1)) */

    return u8RetEccCurveId;
}

/**
* @brief           Convert Ecc ED25519 OID to Curve Id
* @details         This function will parse OID data and return the Ecc Curve Id
*
* @param[in]       pCurveOid         : Curve OID element array
* @param[in]       u32CurveOidLength : Curve OID Length
*
* @returns         uint8             : Curve Id value
*/
static inline uint8 Crypto_Hse_ConvertEd25519OidToCurveId
(
    const uint8* pCurveOid,
    const uint32 u32CurveOidLength
)
{
    uint8   u8RetEccCurveId = HSE_EC_CURVE_NONE;
#if ((STD_ON == CRYPTO_SPT_EC25519_ED25519) || (STD_ON == CRYPTO_SPT_EC25519_CURVE25519))
    boolean bCompareResult;
#if (STD_ON == CRYPTO_SPT_EC25519_ED25519)
    /*OID bytes for ED25519 standard curves */
    static const uint8 Crypto_au8EccEd25519Oid[CRYPTO_25519_ED25519_SIZE_U8]       = {0x2B, 0x06, 0x01, 0x04, 0x01, 0xDA, 0x47, 0x0F, 0x01};
#endif /* (STD_ON == CRYPTO_SPT_EC25519_ED25519) */

#if (STD_ON == CRYPTO_SPT_EC25519_CURVE25519)
    /*OID bytes for CURVE25519 standard curves */
    static const uint8 Crypto_au8EccCurve25519Oid[CRYPTO_25519_CURVE25519_SIZE_U8] = {0x2B, 0x06, 0x01, 0x04, 0x01, 0x97, 0x55, 0x01, 0x05, 0x01};
#endif /* (STD_ON == CRYPTO_SPT_EC25519_CURVE25519) */

    switch (u32CurveOidLength)
    {
#if (STD_ON == CRYPTO_SPT_EC25519_ED25519)
        case CRYPTO_25519_ED25519_SIZE_U8:
            /* Check if the OID configured by user is equal to ED25519 OID */
            bCompareResult = Crypto_Util_MemCmp(pCurveOid, Crypto_au8EccEd25519Oid, u32CurveOidLength);

            if (TRUE == bCompareResult)
            {
                u8RetEccCurveId = HSE_EC_25519_ED25519;
            }
            break;
#endif /* (STD_ON == CRYPTO_SPT_EC25519_ED25519) */

#if (STD_ON == CRYPTO_SPT_EC25519_CURVE25519)
        case CRYPTO_25519_CURVE25519_SIZE_U8:
            /* Check if the OID configured by user is equal to CURVE25519 OID */
            bCompareResult = Crypto_Util_MemCmp(pCurveOid, Crypto_au8EccCurve25519Oid, u32CurveOidLength);

            if (TRUE == bCompareResult)
            {
                u8RetEccCurveId = HSE_EC_25519_CURVE25519;
            }
            break;
#endif /* (STD_ON == CRYPTO_SPT_EC25519_CURVE25519) */

        default:
            u8RetEccCurveId = HSE_EC_CURVE_NONE;
            break;
    }

    /* Avoid compiler warnings */
    (void)bCompareResult;
#else
    /* Avoid compiler warnings */
    (void)pCurveOid;
    (void)u32CurveOidLength;
#endif /* (STD_ON == CRYPTO_SPT_EC25519_ED25519) || (STD_ON == CRYPTO_SPT_EC25519_CURVE25519) */

    return u8RetEccCurveId;
}

/**
* @brief           Convert Ecc ED448 OID to Curve Id
* @details         This function will parse OID data and return the Ecc Curve Id
*
* @param[in]       pCurveOid         : Curve OID element array
* @param[in]       u32CurveOidLength : Curve OID Length
*
* @returns         uint8             : Curve Id value
*/
static inline uint8 Crypto_Hse_ConvertEd448OidToCurveId
(
    const uint8* pCurveOid,
    const uint32 u32CurveOidLength
)
{
    uint8   u8RetEccCurveId = HSE_EC_CURVE_NONE;
#if ((STD_ON == CRYPTO_SPT_EC448_ED448) || (STD_ON == CRYPTO_SPT_EC448_CURVE448))
    boolean bCompareResult;
#if (STD_ON == CRYPTO_SPT_EC448_ED448)
    /*OID bytes for ED448 standard curves */
    static const uint8 Crypto_au8EccEd448Oid[CRYPTO_448_ED448_CURVE448_SIZE_U8]       = {0x2B, 0x65, 0x71};
#endif /* (STD_ON == CRYPTO_SPT_EC448_ED448) */

#if (STD_ON == CRYPTO_SPT_EC448_CURVE448)
    /*OID bytes for CURVE448 standard curves */
    static const uint8 Crypto_au8EccCurve448Oid[CRYPTO_448_ED448_CURVE448_SIZE_U8] = {0x2B, 0x65, 0x6F};
#endif /* (STD_ON == CRYPTO_SPT_EC448_CURVE448) */

    if (CRYPTO_448_ED448_CURVE448_SIZE_U8 == u32CurveOidLength)
    {
#if (STD_ON == CRYPTO_SPT_EC448_ED448)
        /* Check if the OID configured by user is equal to ED448 OID */
        bCompareResult = Crypto_Util_MemCmp(pCurveOid, Crypto_au8EccEd448Oid, u32CurveOidLength);

        if (TRUE == bCompareResult)
        {
            u8RetEccCurveId = HSE_EC_448_ED448;
        }
#endif /* (STD_ON == CRYPTO_SPT_EC448_ED448) */

#if (STD_ON == CRYPTO_SPT_EC448_CURVE448)
        if (FALSE == bCompareResult)
        {
            /* Check if the OID configured by user is equal to CURVE448 OID */
            bCompareResult = Crypto_Util_MemCmp(pCurveOid, Crypto_au8EccCurve448Oid, u32CurveOidLength);

            if (TRUE == bCompareResult)
            {
                u8RetEccCurveId = HSE_EC_448_CURVE448;
            }
        }
#endif /* (STD_ON == CRYPTO_SPT_EC448_CURVE448) */
    }
#else
    /* Avoid compiler warnings */
    (void)pCurveOid;
    (void)u32CurveOidLength;
#endif /* ((STD_ON == CRYPTO_SPT_EC448_ED448) || (STD_ON == CRYPTO_SPT_EC448_CURVE448)) */

    return u8RetEccCurveId;
}

/**
* @brief           Get ECC curve id
* @details         This function will return the curve id from OID
*
* @param[in]       u32EccCurveOidKeyElemIdx: Curve Oid element index
* @param[in]       u8EccCurveAlgoFam       : Curve Oid Algorithm Family
*
* @returns         uint8                   : Curve Id value
*/
static inline uint8 Crypto_Hse_GetEccCurveId
(
    const uint32 u32EccCurveOidKeyElemIdx,
    const uint8  u8EccCurveAlgoFam
)
{
    const uint8* pCurveOid         = Crypto_aKeyElementList[u32EccCurveOidKeyElemIdx].pCryptoElementArray;
    const uint32 u32CurveOidLength = *Crypto_aKeyElementList[u32EccCurveOidKeyElemIdx].pu32CryptoElementActualSize;
    uint8        u8RetEccCurveId   = HSE_EC_CURVE_NONE;

    switch (u8EccCurveAlgoFam)
    {
        case (uint8)CRYPTO_ALGOFAM_ECCNIST:
            u8RetEccCurveId = Crypto_Hse_ConvertNistOidToCurveId(pCurveOid,u32CurveOidLength);
            break;

        case (uint8)CRYPTO_ALGOFAM_BRAINPOOL:
            u8RetEccCurveId = Crypto_Hse_ConvertBrainPoolOidToCurveId(pCurveOid, u32CurveOidLength);
            break;

        case (uint8)CRYPTO_ALGOFAM_ED25519:
            u8RetEccCurveId = Crypto_Hse_ConvertEd25519OidToCurveId(pCurveOid, u32CurveOidLength);
            break;

        case (uint8)CRYPTO_ALGOFAM_ED448:
            u8RetEccCurveId = Crypto_Hse_ConvertEd448OidToCurveId(pCurveOid, u32CurveOidLength);
            break;

        default:
            u8RetEccCurveId = HSE_EC_CURVE_NONE;
            break;
    }

    return u8RetEccCurveId;
}
#endif /* (STD_ON == CRYPTO_SPT_ECC_KEY_PAIR_GEN) */

#if ((STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_PAIR_GEN))
/**
* @brief           Function that returns bit length of key
* @details         Returns bit length of a key based on the elliptic curve ID
*
* @param[in]       u16CurveId: ID of elliptic curve
*
* @returns         uint16:     Length of key
*/
static inline uint16 Crypto_Hse_EccKeyBitLen
(
    uint16 u16CurveId
)
{
    uint16 u16RetKeyBitLen;

    switch (u16CurveId)
    {
#if (STD_ON == CRYPTO_SPT_EC25519_ED25519)
        case HSE_EC_25519_ED25519:
            u16RetKeyBitLen = 256U;
            break;
#endif /* (STD_ON == CRYPTO_SPT_EC25519_ED25519) */

#if (STD_ON == CRYPTO_SPT_EC25519_CURVE25519)
        case HSE_EC_25519_CURVE25519:
            u16RetKeyBitLen = 256U;
            break;
#endif /* (STD_ON == CRYPTO_SPT_EC25519_CURVE25519) */

#if (STD_ON == CRYPTO_SPT_EC448_ED448)
        case HSE_EC_448_ED448:
            u16RetKeyBitLen = 456U;
            break;
#endif /* (STD_ON == CRYPTO_SPT_EC448_ED448) */

#if (STD_ON == CRYPTO_SPT_EC448_CURVE448)
        case HSE_EC_448_CURVE448:
            u16RetKeyBitLen = 448U;
            break;
#endif /* (STD_ON == CRYPTO_SPT_EC448_CURVE448) */

#if (STD_ON == CRYPTO_SPT_SECP256R1)
        case HSE_EC_SEC_SECP256R1:
            u16RetKeyBitLen = 256U;
            break;
#endif /* (STD_ON == CRYPTO_SPT_SECP256R1) */

#if (STD_ON == CRYPTO_SPT_SECP384R1)
        case HSE_EC_SEC_SECP384R1:
            u16RetKeyBitLen = 384U;
            break;
#endif /* (STD_ON == CRYPTO_SPT_SECP384R1) */

#if (STD_ON == CRYPTO_SPT_SECP521R1)
        case HSE_EC_SEC_SECP521R1:
            u16RetKeyBitLen = 521U;
            break;
#endif /* (STD_ON == CRYPTO_SPT_SECP521R1) */

#if (STD_ON == CRYPTO_SPT_BRAINPOOLP256R1)
        case HSE_EC_BRAINPOOL_BRAINPOOLP256R1:
            u16RetKeyBitLen = 256U;
            break;
#endif /* (STD_ON == CRYPTO_SPT_BRAINPOOLP256R1) */

#if (STD_ON == CRYPTO_SPT_BRAINPOOLP320R1)
        case HSE_EC_BRAINPOOL_BRAINPOOLP320R1:
            u16RetKeyBitLen =  320U;
            break;
#endif /* (STD_ON == CRYPTO_SPT_BRAINPOOLP320R1) */

#if (STD_ON == CRYPTO_SPT_BRAINPOOLP384R1)
        case HSE_EC_BRAINPOOL_BRAINPOOLP384R1:
            u16RetKeyBitLen =  384U;
            break;
#endif /* (STD_ON == CRYPTO_SPT_BRAINPOOLP384R1) */

#if (STD_ON == CRYPTO_SPT_BRAINPOOLP512R1)
        case HSE_EC_BRAINPOOL_BRAINPOOLP512R1:
            u16RetKeyBitLen =  512U;
            break;
#endif /* (STD_ON == CRYPTO_SPT_BRAINPOOLP512R1) */

        default:
            u16RetKeyBitLen = 0U;
            break;
    }

    return u16RetKeyBitLen;
}
#endif /* ((STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_PAIR_GEN)) */

/**
* @brief           Configure algorithm mode
* @details         Map autosar algorithm mode to HSE data structure
*
* @param[in]       uint8: Autosar job algorithm mode
*
* @returns         hseCipherBlockMode_t: HSE block mode
*/
static inline hseCipherBlockMode_t Crypto_Hse_ConfigureAlgoMode
(
    uint8 u8AlgoMode
)
{
    hseCipherBlockMode_t RetVal;

    switch (u8AlgoMode)
    {
#if (STD_ON == CRYPTO_SPT_AES_ECB)
        case (uint8)CRYPTO_ALGOMODE_ECB:
            RetVal = HSE_CIPHER_BLOCK_MODE_ECB;
            break;
#endif /* (STD_ON == CRYPTO_SPT_AES_ECB) */

#if (STD_ON == CRYPTO_SPT_AES_CBC)
        case (uint8)CRYPTO_ALGOMODE_CBC:
            RetVal = HSE_CIPHER_BLOCK_MODE_CBC;
            break;
#endif /* (STD_ON == CRYPTO_SPT_AES_CBC) */

#if (STD_ON == CRYPTO_SPT_AES_CFB)
        case (uint8)CRYPTO_ALGOMODE_CFB:
            RetVal = HSE_CIPHER_BLOCK_MODE_CFB;
            break;
#endif /* (STD_ON == CRYPTO_SPT_AES_CFB) */

#if (STD_ON == CRYPTO_SPT_AES_CTR)
        case (uint8)CRYPTO_ALGOMODE_CTR:
            RetVal = HSE_CIPHER_BLOCK_MODE_CTR;
            break;
#endif /* (STD_ON == CRYPTO_SPT_AES_CTR) */

#if (STD_ON == CRYPTO_SPT_AES_OFB)
        case (uint8)CRYPTO_ALGOMODE_OFB:
            RetVal = HSE_CIPHER_BLOCK_MODE_OFB;
            break;
#endif /* (STD_ON == CRYPTO_SPT_AES_OFB) */

        default:
            RetVal = HSE_CIPHER_BLOCK_MODE_NULL;
            break;
    }

    return RetVal;
}

#if (STD_ON == CRYPTO_SPT_HMAC)
/**
* @brief           Fills the HMAC generate descriptor
* @details         Configures the HSE HMAC generate descriptor
*
* @param[in]       u8MuInstance:         MU instance where the job will be issued
* @param[in]       u8MuChannel:          MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       pJob:                 Pointer to the job
* @param[in]       HashAlgo:             Translates AUTOSAR hash Algo into available HSE hash algo
* @param[in]       u8StreamId:           Identifier of the HSE stream used for the job
* @param[inout]    pbReqSentOverChannel: Pointer to a boolean storing channel usage
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: keyId not existent, IV (if needed)
*                   CRYPTO_E_SMALL_BUFFER: provided buffer is to small
*/
static inline Std_ReturnType Crypto_Hse_HmacGen
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    hseHashAlgo_t         HashAlgo,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
)
{
    hseMacSrv_t*   pMacGenCmd      = &(pHseSrvDescriptor->hseSrv.macReq);
    Std_ReturnType RetVal          = (Std_ReturnType)E_NOT_OK;
    uint32         u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(Crypto_Util_GetJobKey(pJob));
    uint32         u32ResultLen    = pJob->jobPrimitiveInfo->primitiveInfo->resultLength;
    uint32         u32KeyMaterialKeyElemIdx;
    const uint8*   pInput          = NULL_PTR;
    uint32         u32inputLength  = 0U;
    const uint8*   pOutputPtr      = NULL_PTR;
    uint32*        pOutputLength   = NULL_PTR;
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    uint32         u32RedirectedInputKeyIdx;
    uint32         u32RedirectedInputKeyElemIdx;
    uint32         u32RedirectedOutputKeyIdx;
    uint32         u32RedirectedOutputKeyElemIdx;
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))  */

#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    /* Check if input is redirected */
    if ((NULL_PTR != pJob->jobRedirectionInfoRef) &&
        (Crypto_Util_IsRedirectionSet(pJob->jobRedirectionInfoRef->redirectionConfig, CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT)))
    {
        /* Take the key index and key element index that was already checked in upper layer */
        u32RedirectedInputKeyIdx     = Crypto_Util_GetKeyIdx(pJob->jobRedirectionInfoRef->inputKeyId);
        u32RedirectedInputKeyElemIdx = Crypto_Util_GetKeyElementIdx (u32RedirectedInputKeyIdx, pJob->jobRedirectionInfoRef->inputKeyElementId);

        /* Check if the u32RedirectedInputKeyElemIdx is valid */
        if(CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32RedirectedInputKeyElemIdx)
        {
            pInput         = Crypto_aKeyElementList[u32RedirectedInputKeyElemIdx].pCryptoElementArray;
            u32inputLength = *Crypto_aKeyElementList[u32RedirectedInputKeyElemIdx].pu32CryptoElementActualSize;
        }
    }
    else
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))  */
    {
        pInput         = pJob->jobPrimitiveInputOutput.inputPtr;
        u32inputLength = pJob->jobPrimitiveInputOutput.inputLength;
    }
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    /* Check if output is redirected */
    if ((NULL_PTR != pJob->jobRedirectionInfoRef) &&
        (Crypto_Util_IsRedirectionSet(pJob->jobRedirectionInfoRef->redirectionConfig, CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT)))
    {
        /* Take the key index and key element index that was already checked in upper layer */
        u32RedirectedOutputKeyIdx     = Crypto_Util_GetKeyIdx(pJob->jobRedirectionInfoRef->outputKeyId);
        u32RedirectedOutputKeyElemIdx = Crypto_Util_GetKeyElementIdx (u32RedirectedOutputKeyIdx, pJob->jobRedirectionInfoRef->outputKeyElementId);

        /* Check if the u32RedirectedOutputKeyElemIdx is valid */
        if(CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32RedirectedOutputKeyElemIdx)
        {
            pOutputPtr    = Crypto_aKeyElementList[u32RedirectedOutputKeyElemIdx].pCryptoElementArray;
            pOutputLength = Crypto_aKeyElementList[u32RedirectedOutputKeyElemIdx].pu32CryptoElementActualSize;
        }
    }
    else
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))  */
    {
        pOutputPtr    = pJob->jobPrimitiveInputOutput.outputPtr;
        pOutputLength = pJob->jobPrimitiveInputOutput.outputLengthPtr;
    }

    u32KeyMaterialKeyElemIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KEY_MATERIAL_U32);

    if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32KeyMaterialKeyElemIdx)
    {
        /* [SWS_Crypto_00071] In case of START and UPDATE the output length should not be checked as the length is required only for FINISH and SINGLE CALL */
        if (((uint8)CRYPTO_OPERATIONMODE_FINISH != (uint8)(((uint8)(pJob->jobPrimitiveInputOutput.mode)) & CRYPTO_OPERATIONMODE_FINISH_MASK_U8)) || (*pOutputLength >= u32ResultLen))
        {
            pHseSrvDescriptor->srvId                = HSE_SRV_ID_MAC;
            pMacGenCmd->sgtOption                   = HSE_SGT_OPTION_NONE;
            pMacGenCmd->streamId                    = u8StreamId;
            pMacGenCmd->macScheme.macAlgo           = HSE_MAC_ALGO_HMAC;
            pMacGenCmd->macScheme.sch.hmac.hashAlgo = HashAlgo;
            pMacGenCmd->accessMode                  = Crypto_Hse_ConfigureAccessMode (pJob->jobPrimitiveInputOutput.mode);
            pMacGenCmd->authDir                     = HSE_AUTH_DIR_GENERATE;
            pMacGenCmd->keyHandle                   = (hseKeyHandle_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseKeyHandle;
            pMacGenCmd->inputLength                 = u32inputLength;
            pMacGenCmd->pInput                      = HSE_PTR_TO_HOST_ADDR(pInput);
            /* [SWS_Crypto_00071] Output buffer length and output buffer are available only at finish step */
            if ((uint8)CRYPTO_OPERATIONMODE_FINISH == (uint8)(((uint8)(pJob->jobPrimitiveInputOutput.mode)) & CRYPTO_OPERATIONMODE_FINISH_MASK_U8))
            {
                pMacGenCmd->pTag                    = HSE_PTR_TO_HOST_ADDR(pOutputPtr);
                pMacGenCmd->pTagLength              = HSE_PTR_TO_HOST_ADDR(pOutputLength);
                /* [SWS_Crypto_00065] If job->jobPrimitiveInfo->primitiveInfo->service is set to CRYPTO_HASH or CRYPTO_MACGENERATE, the parameter job->jobPrimitiveInfo->primitiveInfo->resultLength is required.*/
                /* Load the value of resultLength in pOutputLength, which is send to the firmware, because the user may want the result truncated to a smaller length */
                *pOutputLength = u32ResultLen;
            }
            RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
            /* Mark the request as sent */
            *pbReqSentOverChannel = TRUE;
        }
        else
        {
            /* [SWS_Crypto_00136] Output buffer is not big enough to store the result */
            RetVal = CRYPTO_E_SMALL_BUFFER;
        }
    }

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_SPT_HMAC) */

#if (STD_ON == CRYPTO_SPT_CMAC) || (STD_ON == CRYPTO_SPT_XCBCMAC)
/**
* @brief           Fills the CMAC or XCBCMAC generate descriptor
* @details         Configures the HSE CMAC or XCBCMAC generate descriptor
*
* @param[in]       u8MuInstance:         MU instance where the job will be issued
* @param[in]       u8MuChannel:          MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       pJob:                 Pointer to the job
* @param[in]       u8StreamId:           Identifier of the HSE stream used for the job
* @param[inout]    pbReqSentOverChannel: Pointer to a boolean storing channel usage
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: keyId not existent, IV(if needed)
*                   CRYPTO_E_SMALL_BUFFER: provided buffer is to small
*/
static inline Std_ReturnType Crypto_Hse_CmacXcbcMacGenerate
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
)
{
    uint32         u32KeyMaterialKeyElemIdx;
    hseMacSrv_t*   pMacGenCmd      = &(pHseSrvDescriptor->hseSrv.macReq);
    Std_ReturnType RetVal          = (Std_ReturnType)E_NOT_OK;
    uint32         u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(Crypto_Util_GetJobKey(pJob));

    u32KeyMaterialKeyElemIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KEY_MATERIAL_U32);

    if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32KeyMaterialKeyElemIdx)
    {
        /* [SWS_Crypto_00071] In case of START and UPDATE the output length should not be checked as the length is required only for FINISH and SINGLE CALL */
        if (((uint8)CRYPTO_OPERATIONMODE_FINISH != (((uint8)(pJob->jobPrimitiveInputOutput.mode)) & CRYPTO_OPERATIONMODE_FINISH_MASK_U8)) ||(*pJob->jobPrimitiveInputOutput.outputLengthPtr >= pJob->jobPrimitiveInfo->primitiveInfo->resultLength))
        {
            pHseSrvDescriptor->srvId = HSE_SRV_ID_MAC;

            /* Choosing CMAC or XCBCMAC */
            if (CRYPTO_ALGOMODE_CMAC == pJob->jobPrimitiveInfo->primitiveInfo->algorithm.mode)
            {
                pMacGenCmd->macScheme.macAlgo             = HSE_MAC_ALGO_CMAC;
                pMacGenCmd->macScheme.sch.cmac.cipherAlgo = HSE_CIPHER_ALGO_AES;
            }
            else
            {
                pMacGenCmd->macScheme.macAlgo             = HSE_MAC_ALGO_XCBC_MAC;
                /* XCBC MAC supports only AES128. */
            }
            pMacGenCmd->streamId    = u8StreamId;
            pMacGenCmd->accessMode  = Crypto_Hse_ConfigureAccessMode (pJob->jobPrimitiveInputOutput.mode);
            pMacGenCmd->authDir     = HSE_AUTH_DIR_GENERATE;
            pMacGenCmd->keyHandle   = (hseKeyHandle_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseKeyHandle;
            pMacGenCmd->inputLength = pJob->jobPrimitiveInputOutput.inputLength;
            pMacGenCmd->pInput      = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.inputPtr);
            pMacGenCmd->sgtOption   = HSE_SGT_OPTION_NONE;

            /* [SWS_Crypto_00071] Output buffer length and output buffer are available only at finish step */
            if ((uint8)CRYPTO_OPERATIONMODE_FINISH == (uint8)(((uint8)(pJob->jobPrimitiveInputOutput.mode)) & CRYPTO_OPERATIONMODE_FINISH_MASK_U8))
            {
                pMacGenCmd->pTag        = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.outputPtr);
                pMacGenCmd->pTagLength  = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.outputLengthPtr);
                /* [SWS_Crypto_00135]Fill the outputLengthPtr with the size of CMAC or XCBCMAC. If job->jobPrimitiveInfo->primitiveInfo->service is set to CRYPTO_HASH or CRYPTO_MACGENERATE, the parameter job->jobPrimitiveInfo->primitiveInfo->resultLength is required.*/
                *pJob->jobPrimitiveInputOutput.outputLengthPtr = pJob->jobPrimitiveInfo->primitiveInfo->resultLength;
            }

            RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
            /* Mark the request as sent */
            *pbReqSentOverChannel = TRUE;
        }
        else
        {
            /* [SWS_Crypto_00136] Output buffer is not big enough to store the result */
            RetVal = CRYPTO_E_SMALL_BUFFER;
        }
    }

    return RetVal;
}

/**
* @brief           Fills the CMAC or XCBCMAC verify descriptor
* @details         Configures the HSE CMAC or XCBCMAC verify descriptor
*
* @param[in]       u8MuInstance:         MU instance where the job will be issued
* @param[in]       u8MuChannel:          MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       pJob:                 Pointer to the job
* @param[in]       u8StreamId:           Identifier of the HSE stream used for the job
* @param[inout]    pbReqSentOverChannel: Pointer to a boolean storing channel usage
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: keyId not existent, IV(if needed)
*/
static inline Std_ReturnType Crypto_Hse_CmacXcbcMacVerify
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
)
{
    uint32         u32KeyMaterialKeyElemIdx;
    hseMacSrv_t*   pMacVerCmd;
    Std_ReturnType RetVal          = (Std_ReturnType)E_NOT_OK;
    uint32         u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(Crypto_Util_GetJobKey(pJob));

    u32KeyMaterialKeyElemIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KEY_MATERIAL_U32);
    pHseSrvDescriptor->srvId = HSE_SRV_ID_MAC;
    pMacVerCmd               = &(pHseSrvDescriptor->hseSrv.macReq);

    if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32KeyMaterialKeyElemIdx)
    {
        /* Choose CMAC or XCBCMAC */
        if (CRYPTO_ALGOMODE_CMAC == pJob->jobPrimitiveInfo->primitiveInfo->algorithm.mode)
        {
            pMacVerCmd->macScheme.macAlgo             = HSE_MAC_ALGO_CMAC;
            pMacVerCmd->macScheme.sch.cmac.cipherAlgo = HSE_CIPHER_ALGO_AES;
        }
        else
        {
            pMacVerCmd->macScheme.macAlgo                 = HSE_MAC_ALGO_XCBC_MAC;
        }
        pMacVerCmd->accessMode  = Crypto_Hse_ConfigureAccessMode (pJob->jobPrimitiveInputOutput.mode);
        pMacVerCmd->streamId    = u8StreamId;
        pMacVerCmd->authDir     = HSE_AUTH_DIR_VERIFY;
        pMacVerCmd->keyHandle   = (hseKeyHandle_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseKeyHandle;
        pMacVerCmd->inputLength =            pJob->jobPrimitiveInputOutput.inputLength;
        pMacVerCmd->pInput      = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.inputPtr);
        pMacVerCmd->pTagLength  = HSE_PTR_TO_HOST_ADDR(&(pJob->jobPrimitiveInputOutput.secondaryInputLength));
        pMacVerCmd->pTag        = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.secondaryInputPtr);
        pMacVerCmd->sgtOption   = HSE_SGT_OPTION_NONE;

        RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
        /* Mark the request as sent */
        *pbReqSentOverChannel = TRUE;
    }
    /* [SWS_Crypto_00071] Fill verifyPtr at FINISH */
    if ((CRYPTO_PROCESSING_SYNC == pJob->jobPrimitiveInfo->processingType) && ((uint8)CRYPTO_OPERATIONMODE_FINISH == (uint8)(((uint8)(pJob->jobPrimitiveInputOutput.mode)) & CRYPTO_OPERATIONMODE_FINISH_MASK_U8)))
    {
        /* If the job is synchronous, fill the VerifyPtr in case the primitive was MacVerify or AEAD Decrypt */
        Crypto_Hse_FillVerifyPtr(pJob, RetVal);
    }

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_SPT_CMAC) || (STD_ON == CRYPTO_SPT_XCBCMAC) */

#if (STD_ON == CRYPTO_SPT_FAST_CMAC)
/**
* @brief           Fills the FAST CMAC generate descriptor
* @details         Configures the HSE Fast CMAC descriptor for MAC generation or verification
*
* @param[in]       u8MuInstance:         MU instance where the job will be issued
* @param[in]       u8MuChannel:          MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       pJob:                 Pointer to the job
* @param[in]       HseAuthDir:           HSE_AUTH_DIR_GENERATE or HSE_AUTH_DIR_VERIFY
* @param[inout]    pbReqSentOverChannel: Pointer to a boolean storing channel usage
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: keyId not existent, IV(if needed)
*/
static inline Std_ReturnType Crypto_Hse_FastCmac
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    hseAuthDir_t          HseAuthDir,
    boolean*              pbReqSentOverChannel
)
{
    uint32            u32KeyMaterialKeyElemIdx;
    hseFastCMACSrv_t* pFastCMACSrv    = &(pHseSrvDescriptor->hseSrv.fastCmacReq);
    Std_ReturnType    RetVal          = (Std_ReturnType)E_NOT_OK;
    uint32            u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(Crypto_Util_GetJobKey(pJob));

    u32KeyMaterialKeyElemIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KEY_MATERIAL_U32);

    if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32KeyMaterialKeyElemIdx)
    {
        pHseSrvDescriptor->srvId     = HSE_SRV_ID_FAST_CMAC;

        pFastCMACSrv->authDir        = HseAuthDir;
        pFastCMACSrv->keyHandle      = (hseKeyHandle_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseKeyHandle;
        pFastCMACSrv->pInput         = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.inputPtr);
        pFastCMACSrv->inputBitLength = pJob->jobPrimitiveInputOutput.inputLength;

        if (HSE_AUTH_DIR_GENERATE == HseAuthDir)
        {
            pFastCMACSrv->pTag         = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.outputPtr);
            pFastCMACSrv->tagBitLength = (uint8)(*(pJob->jobPrimitiveInputOutput.outputLengthPtr));
        }
        else
        {
            pFastCMACSrv->pTag         = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.secondaryInputPtr);
            pFastCMACSrv->tagBitLength = (uint8)pJob->jobPrimitiveInputOutput.secondaryInputLength;
        }
        RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
        /* Mark the request as sent */
        *pbReqSentOverChannel = TRUE;
    }

    if (HSE_AUTH_DIR_VERIFY == HseAuthDir)
    {
        /* If the job is synchronous, fill the VerifyPtr */
        Crypto_Hse_FillVerifyPtr(pJob, RetVal);
    }

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_SPT_FAST_CMAC) */

#if (STD_ON == CRYPTO_SPT_GMAC)
/**
* @brief           Fills the GMAC generate descriptor
* @details         Configures the HSE GMAC generate descriptor
*
* @param[in]       u8MuInstance:         MU instance where the job will be issued
* @param[in]       u8MuChannel:          MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       pJob:                 Pointer to the job
* @param[in]       u8StreamId:           Identifier of the HSE stream used for the job
* @param[inout]    pbReqSentOverChannel: Pointer to a boolean storing channel usage
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: keyId not existent, IV(if needed)
*                   CRYPTO_E_SMALL_BUFFER: provided buffer is to small
*/
static inline Std_ReturnType Crypto_Hse_GmacGenerate
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
)
{
    uint32         u32CipherIvKeyElemIdx;
    uint32         u32CryptoKeyMaterialIdx;
    hseMacSrv_t*   pMacGenCmd;
    Std_ReturnType RetVal          = (Std_ReturnType)E_OK;
    uint32         u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(Crypto_Util_GetJobKey(pJob));

    u32CryptoKeyMaterialIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KEY_MATERIAL_U32);

    /* Search for IV, if not found return an error */
    if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoKeyMaterialIdx)
    {
        u32CipherIvKeyElemIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KE_CIPHER_IV);

        if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CipherIvKeyElemIdx)
        {
            pMacGenCmd = &(pHseSrvDescriptor->hseSrv.macReq);

            pMacGenCmd->macScheme.sch.gmac.pIV      = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32CipherIvKeyElemIdx].pCryptoElementArray);
            pMacGenCmd->macScheme.sch.gmac.ivLength =                     *Crypto_aKeyElementList[u32CipherIvKeyElemIdx].pu32CryptoElementActualSize;
        }
        else
        {
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
    }
    else
    {
        RetVal = (Std_ReturnType)E_NOT_OK;
    }

    if ((Std_ReturnType)E_OK == RetVal)
    {
        /* [SWS_Crypto_00071] In case of START and UPDATE the output length should not be checked as the length is required only for FINISH and SINGLE CALL */
        if (((uint8)CRYPTO_OPERATIONMODE_FINISH != (((uint8)(pJob->jobPrimitiveInputOutput.mode)) & CRYPTO_OPERATIONMODE_FINISH_MASK_U8)) || (*pJob->jobPrimitiveInputOutput.outputLengthPtr >= pJob->jobPrimitiveInfo->primitiveInfo->resultLength))
        {
            pHseSrvDescriptor->srvId      = HSE_SRV_ID_MAC;
            pMacGenCmd->accessMode        = Crypto_Hse_ConfigureAccessMode (pJob->jobPrimitiveInputOutput.mode);
            pMacGenCmd->macScheme.macAlgo = HSE_MAC_ALGO_GMAC;
            pMacGenCmd->streamId          = u8StreamId;
            pMacGenCmd->authDir           = HSE_AUTH_DIR_GENERATE;
            pMacGenCmd->keyHandle         = (hseKeyHandle_t)Crypto_aKeyElementList[u32CryptoKeyMaterialIdx].u32HseKeyHandle;
            pMacGenCmd->inputLength       =            pJob->jobPrimitiveInputOutput.inputLength;
            pMacGenCmd->pInput            = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.inputPtr);
            pMacGenCmd->sgtOption         = HSE_SGT_OPTION_NONE;

            if ((uint8)CRYPTO_OPERATIONMODE_FINISH == (uint8)(((uint8)(pJob->jobPrimitiveInputOutput.mode)) & CRYPTO_OPERATIONMODE_FINISH_MASK_U8))
            {
                pMacGenCmd->pTag         = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.outputPtr);
                /* Fill the outputLengthPtr with the size of CMAC. If job->jobPrimitiveInfo->primitiveInfo->service is set to CRYPTO_HASH or CRYPTO_MACGENERATE, the parameter job->jobPrimitiveInfo->primitiveInfo->resultLength is required.*/
                pMacGenCmd->pTagLength   = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.outputLengthPtr);

                *pJob->jobPrimitiveInputOutput.outputLengthPtr = pJob->jobPrimitiveInfo->primitiveInfo->resultLength;
            }
            RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
            /* Mark the request as sent */
            *pbReqSentOverChannel = TRUE;
        }
        else
        {
            /* [SWS_Crypto_00136] Output buffer is not big enough to store the result */
            RetVal = CRYPTO_E_SMALL_BUFFER;
        }
    }

    return RetVal;
}

/**
* @brief           Fills the GMAC verify descriptor
* @details         Configures the HSE GMAC verify descriptor
*
* @param[in]       u8MuInstance:         MU instance where the job will be issued
* @param[in]       u8MuChannel:          MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       pJob:                 Pointer to the job
* @param[in]       u8StreamId:           Identifier of the HSE stream used for the job
* @param[inout]    pbReqSentOverChannel: Pointer to a boolean storing channel usage
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: keyId not existent, IV(if needed)
*/
static inline Std_ReturnType Crypto_Hse_GmacVerify
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
)
{
    uint32         u32CipherIvKeyElemIdx;
    uint32         u32CryptoKeyMaterialIdx;
    hseMacSrv_t*   pMacVerCmd      = &(pHseSrvDescriptor->hseSrv.macReq);
    Std_ReturnType RetVal          = (Std_ReturnType)E_OK;
    uint32         u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(Crypto_Util_GetJobKey(pJob));

    u32CryptoKeyMaterialIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KEY_MATERIAL_U32);

    /* Search for IV, if not found return an error */
    if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 !=  u32CryptoKeyMaterialIdx)
    {
        u32CipherIvKeyElemIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KE_CIPHER_IV);

        if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 !=  u32CipherIvKeyElemIdx)
        {
            pMacVerCmd->macScheme.sch.gmac.pIV      = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32CipherIvKeyElemIdx].pCryptoElementArray);
            pMacVerCmd->macScheme.sch.gmac.ivLength =                     *Crypto_aKeyElementList[u32CipherIvKeyElemIdx].pu32CryptoElementActualSize;
        }
        else
        {
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
    }
    else
    {
        RetVal = (Std_ReturnType)E_NOT_OK;
    }

    if ((Std_ReturnType)E_OK == RetVal)
    {
        pHseSrvDescriptor->srvId      = HSE_SRV_ID_MAC;
        pMacVerCmd->accessMode        = Crypto_Hse_ConfigureAccessMode (pJob->jobPrimitiveInputOutput.mode);
        pMacVerCmd->streamId          = u8StreamId;
        pMacVerCmd->macScheme.macAlgo = HSE_MAC_ALGO_GMAC;
        pMacVerCmd->authDir           = HSE_AUTH_DIR_VERIFY;
        pMacVerCmd->keyHandle         = (hseKeyHandle_t)Crypto_aKeyElementList[u32CryptoKeyMaterialIdx].u32HseKeyHandle;
        pMacVerCmd->inputLength       = pJob->jobPrimitiveInputOutput.inputLength;
        pMacVerCmd->pInput            = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.inputPtr);
        pMacVerCmd->pTagLength        = HSE_PTR_TO_HOST_ADDR(&(pJob->jobPrimitiveInputOutput.secondaryInputLength));
        pMacVerCmd->pTag              = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.secondaryInputPtr);
        pMacVerCmd->sgtOption         = HSE_SGT_OPTION_NONE;

        RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
        /* Mark the request as sent */
        *pbReqSentOverChannel = TRUE;
    }
    if ((CRYPTO_PROCESSING_SYNC == pJob->jobPrimitiveInfo->processingType) && ((uint8)CRYPTO_OPERATIONMODE_FINISH == (uint8)(((uint8)(pJob->jobPrimitiveInputOutput.mode)) & CRYPTO_OPERATIONMODE_FINISH_MASK_U8)))
    {
        /* If the job is synchronous, fill the VerifyPtr in case the primitive was MacGenerate or AEAD Decrypt */
        Crypto_Hse_FillVerifyPtr(pJob, RetVal);
    }

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_SPT_GMAC) */

#if (STD_ON == CRYPTO_SPT_HMAC)
/**
* @brief           Fills the HMAC verify descriptor
* @details         Configures the HSE HMAC verify descriptor
*
* @param[in]       u8MuInstance:         MU instance where the job will be issued
* @param[in]       u8MuChannel:          MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       pJob:                 Pointer to the job
* @param[in]       HashAlgo:             Translates AUTOSAR hash Algo into available HSE hash algo
* @param[in]       u8StreamId:           Identifier of the HSE stream used for the job
* @param[inout]    pbReqSentOverChannel: Pointer to a boolean storing channel usage
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: keyId not existent, IV(if needed)
*/
static inline Std_ReturnType Crypto_Hse_HmacVerify
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    hseHashAlgo_t         HashAlgo,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
)
{
    uint32         u32CryptoKeyMaterialIdx;
    hseMacSrv_t*   pMacVerCmd         = &(pHseSrvDescriptor->hseSrv.macReq);
    Std_ReturnType RetVal             = (Std_ReturnType)E_NOT_OK;
    uint32         u32CryptoKeyIdx    = Crypto_Util_GetKeyIdx(Crypto_Util_GetJobKey(pJob));
    const uint8*   pInput             = NULL_PTR;
    uint32         u32inputLength     = 0U;
    const uint8*   pSecondInput       = NULL_PTR;
    const uint32*  pSecondInputLength = NULL_PTR;
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    uint32         u32RedirectedInputKeyIdx;
    uint32         u32RedirectedInputKeyElemIdx;
    uint32         u32RedirectedSecondInputKeyIdx;
    uint32         u32RedirectedSecondInputKeyElemIdx;
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))  */

#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    /* Check if input is redirected */
    if ((NULL_PTR != pJob->jobRedirectionInfoRef) &&
        (Crypto_Util_IsRedirectionSet(pJob->jobRedirectionInfoRef->redirectionConfig, CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT)))
    {
        /* Take the key index and key element index that was already checked in upper layer */
        u32RedirectedInputKeyIdx     = Crypto_Util_GetKeyIdx(pJob->jobRedirectionInfoRef->inputKeyId);
        u32RedirectedInputKeyElemIdx = Crypto_Util_GetKeyElementIdx (u32RedirectedInputKeyIdx, pJob->jobRedirectionInfoRef->inputKeyElementId);

        /* Check if the u32RedirectedInputKeyElemIdx is valid */
        if(CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32RedirectedInputKeyElemIdx)
        {
            pInput         = Crypto_aKeyElementList[u32RedirectedInputKeyElemIdx].pCryptoElementArray;
            u32inputLength = *Crypto_aKeyElementList[u32RedirectedInputKeyElemIdx].pu32CryptoElementActualSize;
        }
    }
    else
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))  */
    {
        pInput         = pJob->jobPrimitiveInputOutput.inputPtr;
        u32inputLength = pJob->jobPrimitiveInputOutput.inputLength;
    }

#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    /* Check if input is redirected */
    if ((NULL_PTR != pJob->jobRedirectionInfoRef) &&
        (Crypto_Util_IsRedirectionSet(pJob->jobRedirectionInfoRef->redirectionConfig, CRYPTO_REDIRECT_CONFIG_SECONDARY_INPUT)))
    {
        /* Take the key index and key element index that was already checked in upper layer */
        u32RedirectedSecondInputKeyIdx     = Crypto_Util_GetKeyIdx(pJob->jobRedirectionInfoRef->secondaryInputKeyId);
        u32RedirectedSecondInputKeyElemIdx = Crypto_Util_GetKeyElementIdx (u32RedirectedSecondInputKeyIdx, pJob->jobRedirectionInfoRef->secondaryInputKeyElementId);

        /* Check if the u32RedirectedSecondInputKeyElemIdx is valid */
        if(CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32RedirectedSecondInputKeyElemIdx)
        {
            pSecondInput       = Crypto_aKeyElementList[u32RedirectedSecondInputKeyElemIdx].pCryptoElementArray;
            pSecondInputLength = Crypto_aKeyElementList[u32RedirectedSecondInputKeyElemIdx].pu32CryptoElementActualSize;
        }
    }
    else
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))  */
    {
        pSecondInput        = pJob->jobPrimitiveInputOutput.secondaryInputPtr;
        pSecondInputLength  = &pJob->jobPrimitiveInputOutput.secondaryInputLength;
    }

    u32CryptoKeyMaterialIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KEY_MATERIAL_U32);

    if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoKeyMaterialIdx)
    {
        pHseSrvDescriptor->srvId                = HSE_SRV_ID_MAC;
        pMacVerCmd->streamId                    = u8StreamId;   /* Only 0 or 1 possible - maximum 2 jobs in paralel*/
        pMacVerCmd->macScheme.macAlgo           = HSE_MAC_ALGO_HMAC;
        pMacVerCmd->macScheme.sch.hmac.hashAlgo = HashAlgo;
        pMacVerCmd->accessMode                  = Crypto_Hse_ConfigureAccessMode (pJob->jobPrimitiveInputOutput.mode);
        pMacVerCmd->authDir                     = HSE_AUTH_DIR_VERIFY;
        pMacVerCmd->keyHandle                   = (hseKeyHandle_t)Crypto_aKeyElementList[u32CryptoKeyMaterialIdx].u32HseKeyHandle;
        pMacVerCmd->inputLength                 = u32inputLength;
        pMacVerCmd->pInput                      = HSE_PTR_TO_HOST_ADDR(pInput);
        pMacVerCmd->pTag                        = HSE_PTR_TO_HOST_ADDR(pSecondInput);
        pMacVerCmd->pTagLength                  = HSE_PTR_TO_HOST_ADDR(pSecondInputLength);
        pMacVerCmd->sgtOption                   = HSE_SGT_OPTION_NONE;

        RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
        /* Mark the request as sent */
        *pbReqSentOverChannel = TRUE;
    }
    /* [SWS_Crypto_00071] Fill verifyPtr at FINISH */
    if ((CRYPTO_PROCESSING_SYNC == pJob->jobPrimitiveInfo->processingType) && ((uint8)CRYPTO_OPERATIONMODE_FINISH == (uint8)(((uint8)(pJob->jobPrimitiveInputOutput.mode)) & CRYPTO_OPERATIONMODE_FINISH_MASK_U8)))
    {
        /* If the job is synchronous, fill the VerifyPtr in case the primitive was MacGenerate or AEAD Decrypt */
        Crypto_Hse_FillVerifyPtr(pJob, RetVal);
    }
    return RetVal;
}
#endif /* (STD_ON == CRYPTO_SPT_HMAC) */

#if (STD_ON == CRYPTO_SPT_AEAD)
/**
* @brief           Fills the AEAD encrypt descriptor
* @details         Configures the AEAD encrypt descriptor
*
* @param[in]       u8MuInstance:         MU instance where the job will be issued
* @param[in]       u8MuChannel:          MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       pJob:                 Pointer to the job
* @param[in]       u8StreamId:           Allocated stream ID
* @param[in]       CipherDir:            Choose between encrypt / decrypt
* @param[inout]    pbReqSentOverChannel: Pointer to a boolean storing channel usage
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: keyId not existent
*                   CRYPTO_E_SMALL_BUFFER: provided buffer is to small
*/
static inline Std_ReturnType Crypto_Hse_AeadEncDec
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    hseCipherDir_t        CipherDir,
    boolean*              pbReqSentOverChannel
)
{
    uint32         u32CipherIvKeyElemIdx;
    uint32         u32CryptoKeyMaterialIdx;
    hseAeadSrv_t*  pAeadCmd        = &(pHseSrvDescriptor->hseSrv.aeadReq);
    Std_ReturnType RetVal          = (Std_ReturnType)E_OK;
    uint32         u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(Crypto_Util_GetJobKey(pJob));

    u32CryptoKeyMaterialIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KEY_MATERIAL_U32);
    u32CipherIvKeyElemIdx   = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KE_CIPHER_IV);

    /* Check if a key material element and a initialization vector element was found for the current job key */
    if ((CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoKeyMaterialIdx) && (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CipherIvKeyElemIdx))
    {
        /* Check if there is input data to be processed. If there is no data to process skip checking the job output buffer length.
           For data processing the output buffer must be big enough for the result */
        if ((0U == pJob->jobPrimitiveInputOutput.inputLength) || (*pJob->jobPrimitiveInputOutput.outputLengthPtr >= pJob->jobPrimitiveInputOutput.inputLength))
        {
            pHseSrvDescriptor->srvId      = HSE_SRV_ID_AEAD;
            pAeadCmd->authCipherMode      = HSE_AUTH_CIPHER_MODE_GCM;
            pAeadCmd->cipherDir           = CipherDir;
            pAeadCmd->accessMode          = Crypto_Hse_ConfigureAccessMode (pJob->jobPrimitiveInputOutput.mode);
            /* Use allocated stream, it will be ignored by firmware for single call */
            pAeadCmd->streamId            = u8StreamId;
            pAeadCmd->keyHandle           = (hseKeyHandle_t)Crypto_aKeyElementList[u32CryptoKeyMaterialIdx].u32HseKeyHandle;
            pAeadCmd->pInput              = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.inputPtr);
            pAeadCmd->pOutput             = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.outputPtr);
             /* The length of the plaintext and ciphertext (in bytes). Zero means authentication only. GCM: It can have any number of bits between 0 & (2^39-256) (use byte equivalent) */
            pAeadCmd->inputLength         =           pJob->jobPrimitiveInputOutput.inputLength;
            pAeadCmd->pIV                 = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32CipherIvKeyElemIdx].pCryptoElementArray);
            pAeadCmd->ivLength            =                     *Crypto_aKeyElementList[u32CipherIvKeyElemIdx].pu32CryptoElementActualSize;
            pAeadCmd->pAAD                = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.secondaryInputPtr);
            pAeadCmd->aadLength           =                      pJob->jobPrimitiveInputOutput.secondaryInputLength;
            pAeadCmd->sgtOption           = HSE_SGT_OPTION_NONE;

            if (HSE_CIPHER_DIR_ENCRYPT == CipherDir)
            {
                pAeadCmd->pTag            = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.secondaryOutputPtr);
                pAeadCmd->tagLength       =                     *pJob->jobPrimitiveInputOutput.secondaryOutputLengthPtr;
            }
            else
            {
                pAeadCmd->pTag            = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.tertiaryInputPtr);
                pAeadCmd->tagLength       =                      pJob->jobPrimitiveInputOutput.tertiaryInputLength;
            }

            /* [SWS_Crypto_00071], [SWS_Crypto_00135] When the request has finished, the actual length of the returned value shall be stored.
               User can provide input data even for START step, this is an extension to the [SWS_Crypto_00071] requirement, the user can skip the input data processing at START or FINISH
               by setting the input length to 0.
               If the input length is 0 no result will be provided so output length should not be updated. */
            if (0U != pJob->jobPrimitiveInputOutput.inputLength)
            {
                *pJob->jobPrimitiveInputOutput.outputLengthPtr = pJob->jobPrimitiveInputOutput.inputLength;
            }

            RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
            /* Mark the request as sent */
            *pbReqSentOverChannel = TRUE;
        }
        else
        {
            /* [SWS_Crypto_00136] Output buffer is not big enough to store the result */
            RetVal = (Std_ReturnType)CRYPTO_E_SMALL_BUFFER;
        }
    }
    else
    {
        RetVal = (Std_ReturnType)E_NOT_OK;
    }

    if (HSE_CIPHER_DIR_DECRYPT == CipherDir)
    {
        /* [SWS_Crypto_00071] Fill verifyPtr at FINISH */
        if ((CRYPTO_PROCESSING_SYNC == pJob->jobPrimitiveInfo->processingType) && ((uint8)CRYPTO_OPERATIONMODE_FINISH == (uint8)(((uint8)(pJob->jobPrimitiveInputOutput.mode)) & CRYPTO_OPERATIONMODE_FINISH_MASK_U8)))
        {
            /* If the job is synchronous, fill the VerifyPtr in case the primitive was MacGenerate or AEAD Decrypt */
            Crypto_Hse_FillVerifyPtr(pJob, RetVal);
        }
    }

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_SPT_AEAD) */

/**
* @brief           Fills the encrypt/decript  descriptor
* @details         Configures the encrypt/decript descriptor
*
* @param[in]       u8MuInstance:         MU instance where the job will be issued
* @param[in]       u8MuChannel:          MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       pJob:                 Pointer to the job
* @param[in]       u8CipherDir:          Choose between encrypt / decrypt
* @param[in]       u8StreamId:           Allocated stream ID
* @param[inout]    pbReqSentOverChannel: Pointer to a boolean storing channel usage
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: incorrect lengths
*                   CRYPTO_E_SMALL_BUFFER: provided buffer is to small
*/
static inline Std_ReturnType Crypto_Hse_EncDec
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    hseCipherDir_t        u8CipherDir,
    boolean*              pbReqSentOverChannel
)
{
    uint32               u32CipherIvKeyElemIdx;
    uint32               u32CryptoKeyMaterialIdx;
    hseSymCipherSrv_t*   pSymEncDecCmd   = &(pHseSrvDescriptor->hseSrv.symCipherReq);
    Std_ReturnType       RetVal          = (Std_ReturnType)E_NOT_OK;
    hseCipherBlockMode_t HseAlgoMode     = Crypto_Hse_ConfigureAlgoMode((uint8)pJob->jobPrimitiveInfo->primitiveInfo->algorithm.mode);
    boolean              bRequestIsValid = TRUE;
    uint32               u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(Crypto_Util_GetJobKey(pJob));
    const uint8*         pInput          = NULL_PTR;
    uint32               u32inputLength  = 0U;
    const uint8*         pOutputPtr      = NULL_PTR;
    uint32*              pOutputLength   = NULL_PTR;
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    uint32               u32RedirectedInputKeyIdx;
    uint32               u32RedirectedInputKeyElemIdx;
    uint32               u32RedirectedOutputKeyIdx;
    uint32               u32RedirectedOutputKeyElemIdx;
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */

    u32CryptoKeyMaterialIdx   = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KEY_MATERIAL_U32);
    pHseSrvDescriptor->srvId  = HSE_SRV_ID_SYM_CIPHER;
    pSymEncDecCmd->accessMode = Crypto_Hse_ConfigureAccessMode (pJob->jobPrimitiveInputOutput.mode);
    /* Use allocated stream, it will be ignored by firmware for single call */
    pSymEncDecCmd->streamId   = u8StreamId;
    pSymEncDecCmd->sgtOption  = HSE_SGT_OPTION_NONE;

    if (CRYPTO_ALGOFAM_AES == pJob->jobPrimitiveInfo->primitiveInfo->algorithm.family)
    {
        pSymEncDecCmd->cipherAlgo = HSE_CIPHER_ALGO_AES;
    }
#if(STD_ON == CRYPTO_SPT_TDES)
    else if (CRYPTO_ALGOFAM_3DES == pJob->jobPrimitiveInfo->primitiveInfo->algorithm.family)
    {
        pSymEncDecCmd->cipherAlgo = HSE_CIPHER_ALGO_3DES;
    }
#endif /* (STD_ON == CRYPTO_SPT_TDES) */
    else
    {
        pSymEncDecCmd->cipherAlgo = HSE_CIPHER_ALGO_NULL;
    }
    /* Check if a key material element was found for the current job key */
    if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoKeyMaterialIdx)
    {
        /* Check for IV in case of block mode different than ECB */
        if (CRYPTO_ALGOMODE_ECB != pJob->jobPrimitiveInfo->primitiveInfo->algorithm.mode)
        {
            u32CipherIvKeyElemIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KE_CIPHER_IV);
            /* Check if a initialization vector element was found for the current job key */
            if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 !=  u32CipherIvKeyElemIdx)
            {
                /* IV length is not required, as it is considered to be always set to 16 bytes */
                pSymEncDecCmd->pIV = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32CipherIvKeyElemIdx].pCryptoElementArray);
            }
            else
            {
                bRequestIsValid = FALSE;
            }
        }
    }
    else
    {
        bRequestIsValid = FALSE;
    }

#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    /* Check if input is redirected */
    if ((NULL_PTR != pJob->jobRedirectionInfoRef) &&
        (Crypto_Util_IsRedirectionSet(pJob->jobRedirectionInfoRef->redirectionConfig, CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT)))
    {
        /* Take the key index and key element index that was already checked in upper layer */
        u32RedirectedInputKeyIdx     = Crypto_Util_GetKeyIdx(pJob->jobRedirectionInfoRef->inputKeyId);
        u32RedirectedInputKeyElemIdx = Crypto_Util_GetKeyElementIdx (u32RedirectedInputKeyIdx, pJob->jobRedirectionInfoRef->inputKeyElementId);

        /* Check if the u32RedirectedInputKeyElemIdx is valid */
        if(CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32RedirectedInputKeyElemIdx)
        {
            pInput         = Crypto_aKeyElementList[u32RedirectedInputKeyElemIdx].pCryptoElementArray;
            u32inputLength = *Crypto_aKeyElementList[u32RedirectedInputKeyElemIdx].pu32CryptoElementActualSize;
        }
    }
    else
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))  */
    {
        pInput         = pJob->jobPrimitiveInputOutput.inputPtr;
        u32inputLength = pJob->jobPrimitiveInputOutput.inputLength;
    }

#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    /* Check if output is redirected */
    if ((NULL_PTR != pJob->jobRedirectionInfoRef) &&
        (Crypto_Util_IsRedirectionSet(pJob->jobRedirectionInfoRef->redirectionConfig, CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT)))
    {
        /* Take the key index and key element index that was already checked in upper layer */
        u32RedirectedOutputKeyIdx     = Crypto_Util_GetKeyIdx(pJob->jobRedirectionInfoRef->outputKeyId);
        u32RedirectedOutputKeyElemIdx = Crypto_Util_GetKeyElementIdx (u32RedirectedOutputKeyIdx, pJob->jobRedirectionInfoRef->outputKeyElementId);

        /* Check if the u32RedirectedOutputKeyElemIdx is valid */
        if(CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32RedirectedOutputKeyElemIdx)
        {
            pOutputPtr    = Crypto_aKeyElementList[u32RedirectedOutputKeyElemIdx].pCryptoElementArray;
            pOutputLength = Crypto_aKeyElementList[u32RedirectedOutputKeyElemIdx].pu32CryptoElementActualSize;
        }
    }
    else
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))  */
    {
        pOutputPtr    = pJob->jobPrimitiveInputOutput.outputPtr;
        pOutputLength = pJob->jobPrimitiveInputOutput.outputLengthPtr;
    }

    /* Check if the request is valid (has all the needed information in place) */
    if (TRUE == bRequestIsValid)
    {
        pSymEncDecCmd->cipherDir       = u8CipherDir;
        pSymEncDecCmd->cipherBlockMode = HseAlgoMode;
        pSymEncDecCmd->inputLength     = u32inputLength;
        pSymEncDecCmd->pInput          = HSE_PTR_TO_HOST_ADDR(pInput);
        pSymEncDecCmd->pOutput         = HSE_PTR_TO_HOST_ADDR(pOutputPtr);
        pSymEncDecCmd->keyHandle       = (hseKeyHandle_t)Crypto_aKeyElementList[u32CryptoKeyMaterialIdx].u32HseKeyHandle;

        /* If the input data is provided the output buffer length must be greater or equal to input data length */
        if ((0U == u32inputLength) || (*pOutputLength >= u32inputLength))
        {
            /* If input data has been provided update the value of the output length */
            if (0U != u32inputLength)
            {
                /* [SWS_Crypto_00071], [SWS_Crypto_00135] When the request has finished, the actual length of the returned value shall be stored */
                *pOutputLength = u32inputLength;
            }

            RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
            /* Mark the request as sent */
            *pbReqSentOverChannel = TRUE;
        }
        else
        {
            /* [SWS_Crypto_00136] Output buffer is not big enough to store the result */
            RetVal = (Std_ReturnType)CRYPTO_E_SMALL_BUFFER;
        }
    }

    return RetVal;
}

#if (STD_ON == CRYPTO_SPT_RSA)
/**
* @brief           Fills the RSA encrypt/decrypt descriptor
* @details         Configures the RSA encrypt/decrypt descriptor
*
* @param[in]       u8MuInstance:         MU instance where the job will be issued
* @param[in]       u8MuChannel:          MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       pJob:                 Pointer to the job
* @param[in]       CipherDir:            Choose between encrypt / decrypt
* @param[inout]    pbReqSentOverChannel: Pointer to a boolean storing channel usage
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: keyId not existent
*/
static inline Std_ReturnType Crypto_Hse_RSAEncDec
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    hseCipherDir_t        CipherDir,
    boolean*              pbReqSentOverChannel
)
{
    uint32                    u32CryptoKeyMaterialIdx;
    hseRsaCipherSrv_t*        pRsaEncDecSrv   = &(pHseSrvDescriptor->hseSrv.rsaCipherReq);
#if (STD_ON == CRYPTO_SPT_HASH)
    Crypto_Hse_HashConfigType HashAlgo        = Crypto_Hse_ConfigureHashAlgo((uint8)pJob->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily);
#endif /* (STD_ON == CRYPTO_SPT_HASH) */
    Std_ReturnType            RetVal          = (Std_ReturnType)E_NOT_OK;
    boolean                   bAlgoFound      = TRUE;
    uint32                    u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(Crypto_Util_GetJobKey(pJob));

    u32CryptoKeyMaterialIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KEY_MATERIAL_U32);

    if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoKeyMaterialIdx)
    {
        pHseSrvDescriptor->srvId = HSE_SRV_ID_RSA_CIPHER;
        pRsaEncDecSrv->cipherDir = CipherDir;

        if (CRYPTO_ALGOMODE_RSAES_OAEP == pJob->jobPrimitiveInfo->primitiveInfo->algorithm.mode)
        {
            pRsaEncDecSrv->rsaScheme.rsaAlgo                 = HSE_RSA_ALGO_RSAES_OAEP;
#if (STD_ON == CRYPTO_SPT_HASH)
            pRsaEncDecSrv->rsaScheme.sch.rsaOAEP.hashAlgo    = HashAlgo.HseHashAlgo;
#endif /* (STD_ON == CRYPTO_SPT_HASH) */
            if ((0U == pJob->jobPrimitiveInputOutput.secondaryInputLength) ||
                (NULL_PTR == pJob->jobPrimitiveInputOutput.secondaryInputPtr))
            {
                pRsaEncDecSrv->rsaScheme.sch.rsaOAEP.labelLength = (uint32)0UL;
                pRsaEncDecSrv->rsaScheme.sch.rsaOAEP.pLabel      = (HOST_ADDR)0U;
            }
            else
            {
                pRsaEncDecSrv->rsaScheme.sch.rsaOAEP.labelLength = pJob->jobPrimitiveInputOutput.secondaryInputLength;
                pRsaEncDecSrv->rsaScheme.sch.rsaOAEP.pLabel      = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.secondaryInputPtr);
            }
        }
        else if (CRYPTO_ALGOMODE_RSAES_PKCS1_v1_5 == pJob->jobPrimitiveInfo->primitiveInfo->algorithm.mode)
        {
            pRsaEncDecSrv->rsaScheme.rsaAlgo = HSE_RSA_ALGO_RSAES_PKCS1_V15;
        }
        else
        {
            bAlgoFound = FALSE;
        }
        if (TRUE == bAlgoFound)
        {
            pRsaEncDecSrv->pInput        = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.inputPtr);
            pRsaEncDecSrv->inputLength   =                      pJob->jobPrimitiveInputOutput.inputLength;
            pRsaEncDecSrv->pOutput       = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.outputPtr);
            pRsaEncDecSrv->pOutputLength = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.outputLengthPtr);
            pRsaEncDecSrv->keyHandle     = (hseKeyHandle_t)Crypto_aKeyElementList[u32CryptoKeyMaterialIdx].u32HseKeyHandle;

            RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
            /* Mark the request as sent */
            *pbReqSentOverChannel = TRUE;
        }
    }

    return RetVal;
}
#endif /* STD_ON == CRYPTO_SPT_RSA) */

#if (STD_ON == CRYPTO_SPT_SYM_KEY_IMPORT)
/**
* @brief           Fills the import descriptor
* @details         Configures the import descriptor for symetric keys
*
* @param[in]       u8MuInstance:             MU instance where the job will be issued
* @param[in]       u8MuChannel:              MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:        Descriptor to be filled and send to HSE
* @param[in]       u32KeyLength:             Key length defined
* @param[in]       pKeyElement:              Pointer to Key Material
* @param[in]       u32KeyMaterialKeyElemIdx: Index of Key Material
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: incorrect lengths
*/
static inline Std_ReturnType Crypto_Hse_LoadSimKey
(
    uint8               u8MuInstance,
    uint8               u8MuChannel,
    hseSrvDescriptor_t* pHseSrvDescriptor,
    uint32              u32KeyLength,
    const uint8*        pKeyMaterial,
    uint32              u32KeyMaterialKeyElemIdx
)
{
    hseImportKeySrv_t* pImportKeyReq;
    uint32             u32KeyBitLen;
    Std_ReturnType     RetVal = (Std_ReturnType)E_NOT_OK;

    pImportKeyReq     = &(pHseSrvDescriptor->hseSrv.importKeyReq);
    u32KeyBitLen      = Crypto_GetBitLenFromByteLen(u32KeyLength);

    Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyType                   = (hseKeyType_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u8HseKeyType;
    Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyBitLen                 = (uint16)u32KeyBitLen;
    Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyFlags                  = (hseKeyFlags_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u16HseKeyFlags;
    Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyCounter                =                Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseKeyCounter;
    Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.smrFlags                  = (hseSmrFlags_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseKeySmrFlags;
#if (STD_ON == CRYPTO_AES_BLOCK_MODE_MASK)
    Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.specific.aesBlockModeMask = (hseAesBlockModeMask_t)0U; /* Any AES block mode can be used */
#endif /* (STD_ON == CRYPTO_AES_BLOCK_MODE_MASK) */

    pHseSrvDescriptor->srvId = HSE_SRV_ID_IMPORT_KEY;
    /* Check if Cipher Scheme is used */
    pImportKeyReq->cipher.cipherKeyHandle     = (hseKeyHandle_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseCipherKeyHandle;
    pImportKeyReq->keyContainer.authKeyHandle = (hseKeyHandle_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseAuthKeyHandle;

    if (CRYPTO_WA_ENCRYPTED == Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].eCryptoKeyElementWriteAccess)
    {
        Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyBitLen = (uint16)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->u32EncryptedHseKeyBitLen;

        if
        (
            (HSE_CIPHER_ALGO_AES  == (hseCipherAlgo_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->u8CryptoCipherAlgo)
#if(STD_ON == CRYPTO_SPT_TDES)
            ||
            (HSE_CIPHER_ALGO_3DES == (hseCipherAlgo_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->u8CryptoCipherAlgo)
#endif /* (STD_ON == CRYPTO_SPT_TDES) */
        )
        {
            pImportKeyReq->cipher.cipherScheme.symCipher.cipherAlgo      = (hseCipherAlgo_t)     Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->u8CryptoCipherAlgo;
            pImportKeyReq->cipher.cipherScheme.symCipher.cipherBlockMode = (hseCipherBlockMode_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->u8CryptoCipherBlockMode;
            pImportKeyReq->cipher.cipherScheme.symCipher.ivLength        =                       Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->u32CryptoCipherIVLen;
            pImportKeyReq->cipher.cipherScheme.symCipher.pIV             = HSE_PTR_TO_HOST_ADDR (Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->pCryptoCipherIV);
        }
        else if (TRUE == Crypto_Hse_AuthCipherModeIsCcmOrGcm(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->u8CryptoAuthCipherMode))
        {
            /* Aead cipher scheme. */
            /* Authenticated cipher mode. */
            pImportKeyReq->cipher.cipherScheme.aeadCipher.authCipherMode = (hseAuthCipherMode_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->u8CryptoAuthCipherMode;
            /* Tag pointer length. */
            pImportKeyReq->cipher.cipherScheme.aeadCipher.tagLength      =            (uint16_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->u32CryptoAuthCipherTagLen;
            /* Tag pointer. */
            pImportKeyReq->cipher.cipherScheme.aeadCipher.pTag           = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->pCryptoAuthCipherTag);
            /* Initialization Vector length(at least 12 bytes). */
            pImportKeyReq->cipher.cipherScheme.aeadCipher.ivLength       =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->u32CryptoAuthCipherIVLen;
            /* Initialization Vector/Nonce. */
            pImportKeyReq->cipher.cipherScheme.aeadCipher.pIV            = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->pCryptoAuthCipherIV);
            /* AAD Header data length. */
            pImportKeyReq->cipher.cipherScheme.aeadCipher.aadLength      =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->u32CryptoAuthCipherAADLen;
            /* AAD Header data. */
            pImportKeyReq->cipher.cipherScheme.aeadCipher.pAAD           = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->pCryptoAuthCipherAAD);
            /* Key info must be in AAD data */
            pImportKeyReq->pKeyInfo                                      = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->pCryptoAuthCipherAAD);
        }
        else
        {
            pImportKeyReq->cipher.cipherScheme.rsaCipher.rsaAlgo         = (hseRsaAlgo_t)        Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->u8CryptoCipherAlgo;

            if (HSE_RSA_ALGO_RSAES_OAEP == (hseRsaAlgo_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->u8CryptoCipherAlgo)
            {
                pImportKeyReq->cipher.cipherScheme.rsaCipher.sch.rsaOAEP.hashAlgo    = (hseHashAlgo_t)      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->u8CryptoCipherRsaHashAlgo;
                pImportKeyReq->cipher.cipherScheme.rsaCipher.sch.rsaOAEP.labelLength =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->u32CryptoCipherRsaLabelLen;
                pImportKeyReq->cipher.cipherScheme.rsaCipher.sch.rsaOAEP.pLabel      = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->pCryptoCipherRsaLabel);
            }
            else
            {
                /* For RSA-PKCS1V15 is no scheme */
            }
        }
    }

    if (HSE_INVALID_KEY_HANDLE != Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseAuthKeyHandle)
    {
        if (NULL_PTR != Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthMacScheme)
        {
            switch (Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthMacScheme->u8CryptoAuthMacAlgo)
            {
#if (STD_ON == CRYPTO_SPT_HMAC)
                case HSE_MAC_ALGO_HMAC:
                    pImportKeyReq->keyContainer.authScheme.macScheme.macAlgo             = (hseMacAlgo_t)       Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthMacScheme->u8CryptoAuthMacAlgo;
                    pImportKeyReq->keyContainer.authScheme.macScheme.sch.hmac.hashAlgo   = (hseHashAlgo_t)      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthMacScheme->u8CryptoAuthHashAlgo;
                    pImportKeyReq->keyContainer.authLen[0]                               =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->u16CryptoAuthTagLength0;
                    pImportKeyReq->keyContainer.pAuth[0]                                 = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->pCryptoAuthTag0);
                    break;
#endif /* (STD_ON == CRYPTO_SPT_HMAC) */

#if (STD_ON == CRYPTO_SPT_CMAC)
                case HSE_MAC_ALGO_CMAC:
                    pImportKeyReq->keyContainer.authScheme.macScheme.macAlgo             = (hseMacAlgo_t)       Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthMacScheme->u8CryptoAuthMacAlgo;
                    pImportKeyReq->keyContainer.authScheme.macScheme.sch.cmac.cipherAlgo = (hseCipherAlgo_t)    Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthMacScheme->u8CryptoAuthCipherAlgo;
                    pImportKeyReq->keyContainer.authLen[0]                               =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->u16CryptoAuthTagLength0;
                    pImportKeyReq->keyContainer.pAuth[0]                                 = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->pCryptoAuthTag0);
                    break;
#endif /* (STD_ON == CRYPTO_SPT_CMAC) */

#if (STD_ON == CRYPTO_SPT_GMAC)
                case HSE_MAC_ALGO_GMAC:
                    pImportKeyReq->keyContainer.authScheme.macScheme.macAlgo             = (hseMacAlgo_t)       Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthMacScheme->u8CryptoAuthMacAlgo;
                    pImportKeyReq->keyContainer.authScheme.macScheme.sch.gmac.ivLength   =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthMacScheme->u32CryptoAuthIvLength;
                    pImportKeyReq->keyContainer.authScheme.macScheme.sch.gmac.pIV        = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthMacScheme->pCryptoAuthIv);
                    pImportKeyReq->keyContainer.authLen[0]                               =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->u16CryptoAuthTagLength0;
                    pImportKeyReq->keyContainer.pAuth[0]                                 = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->pCryptoAuthTag0);
                    break;
#endif /* (STD_ON == CRYPTO_SPT_GMAC) */

#if (STD_ON == CRYPTO_SPT_XCBCMAC)
                case HSE_MAC_ALGO_XCBC_MAC:
                    pImportKeyReq->keyContainer.authScheme.macScheme.macAlgo             = (hseMacAlgo_t)       Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthMacScheme->u8CryptoAuthMacAlgo;
                    pImportKeyReq->keyContainer.authLen[0]                               =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->u16CryptoAuthTagLength0;
                    pImportKeyReq->keyContainer.pAuth[0]                                 = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->pCryptoAuthTag0);
                    break;
#endif /* (STD_ON == CRYPTO_SPT_XCBCMAC) */

                default:
                    /* Do nothing. */
                    break;
            }
        }
        else if (NULL_PTR != Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthSignScheme)
        {
            switch (Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthSignScheme->u8CryptoAuthSignScheme)
            {
#if (STD_ON == CRYPTO_SPT_ECDSA)
                case HSE_SIGN_ECDSA:
                    pImportKeyReq->keyContainer.authScheme.sigScheme.signSch                  = (hseSignSchemeEnum_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthSignScheme->u8CryptoAuthSignScheme;
                    pImportKeyReq->keyContainer.authScheme.sigScheme.sch.ecdsa.hashAlgo       = (hseHashAlgo_t)      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthSignScheme->u8CryptoAuthSignHashAlgo;
                    pImportKeyReq->keyContainer.authLen[0]                                    =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->u16CryptoAuthTagLength0;
                    pImportKeyReq->keyContainer.pAuth[0]                                      = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->pCryptoAuthTag0);
                    pImportKeyReq->keyContainer.authLen[1]                                    =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->u16CryptoAuthTagLength1;
                    pImportKeyReq->keyContainer.pAuth[1]                                      = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->pCryptoAuthTag1);
                    break;
#endif /* (STD_ON == CRYPTO_SPT_ECDSA) */

#if (STD_ON == CRYPTO_SPT_EDDSA)
                case HSE_SIGN_EDDSA:
                    pImportKeyReq->keyContainer.authScheme.sigScheme.signSch                  = (hseSignSchemeEnum_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthSignScheme->u8CryptoAuthSignScheme;
                    pImportKeyReq->keyContainer.authScheme.sigScheme.sch.eddsa.contextLength  =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthSignScheme->u8CryptoAuthContextLength;
                    pImportKeyReq->keyContainer.authScheme.sigScheme.sch.eddsa.pContext       = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthSignScheme->pCryptoAuthContext);
                    pImportKeyReq->keyContainer.authScheme.sigScheme.sch.eddsa.bHashEddsa     =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthSignScheme->bCryptoHashEddsa;
                    pImportKeyReq->keyContainer.authLen[0]                                    =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->u16CryptoAuthTagLength0;
                    pImportKeyReq->keyContainer.authLen[1]                                    =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->u16CryptoAuthTagLength1;
                    pImportKeyReq->keyContainer.pAuth[0]                                      = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->pCryptoAuthTag0);
                    pImportKeyReq->keyContainer.pAuth[1]                                      = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->pCryptoAuthTag1);
                    break;
#endif /* (STD_ON == CRYPTO_SPT_EDDSA) */

#if(STD_ON == CRYPTO_SPT_RSASSA_PCKS1_V15)
                case HSE_SIGN_RSASSA_PKCS1_V15:
                    pImportKeyReq->keyContainer.authScheme.sigScheme.signSch                  = (hseSignSchemeEnum_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthSignScheme->u8CryptoAuthSignScheme;
                    pImportKeyReq->keyContainer.authScheme.sigScheme.sch.rsaPkcs1v15.hashAlgo = (hseHashAlgo_t)      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthSignScheme->u8CryptoAuthSignHashAlgo;
                    pImportKeyReq->keyContainer.authLen[0]                                    =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->u16CryptoAuthTagLength0;
                    pImportKeyReq->keyContainer.pAuth[0]                                      = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->pCryptoAuthTag0);
                    break;
#endif /* (STD_ON == CRYPTO_SPT_RSASSA_PCKS1_V15) */

#if (STD_ON == CRYPTO_SPT_RSAASSA_PSS)
                case HSE_SIGN_RSASSA_PSS:
                    pImportKeyReq->keyContainer.authScheme.sigScheme.signSch                  = (hseSignSchemeEnum_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthSignScheme->u8CryptoAuthSignScheme;
                    pImportKeyReq->keyContainer.authScheme.sigScheme.sch.rsaPss.saltLength    = Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthSignScheme->u8CryptoAuthSaltLength;
                    pImportKeyReq->keyContainer.authScheme.sigScheme.sch.rsaPss.hashAlgo      = (hseHashAlgo_t)      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthSignScheme->u8CryptoAuthSignHashAlgo;
                    pImportKeyReq->keyContainer.authLen[0]                                    =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->u16CryptoAuthTagLength0;
                    pImportKeyReq->keyContainer.pAuth[0]                                      = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->pCryptoAuthTag0);
                    break;
#endif /* (STD_ON == CRYPTO_SPT_RSAASSA_PSS) */

                default:
                    /* Do nothing */
                    break;
            }
        }
        else
        {
            /* Do nothing */
        }
        /* Load the container address in the descriptor */
        pImportKeyReq->keyContainer.keyContainerLen =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->u16CryptoAuthKeyContLength;
        pImportKeyReq->keyContainer.pKeyContainer   = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->u32CryptoAuthKeyCont);
        /* Load the KeyInfo from container, the base address of the container represents the start address of the KeyInfo */
        pImportKeyReq->pKeyInfo                     = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->u32CryptoAuthKeyCont);
        /* Load from container the pointer to the key value */
        pImportKeyReq->pKey[2]                      = HSE_PTR_TO_HOST_ADDR((Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseAuthContainer->u32CryptoAuthKeyCont + ((uint32)sizeof(hseKeyInfo_t))));
        pImportKeyReq->keyLen[2]                    = (uint16) u32KeyLength;
    }
    else
    {
        if (NULL_PTR != Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme)
        {
            if ((HSE_AUTH_CIPHER_MODE_CCM != (hseAuthCipherMode_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->u8CryptoAuthCipherMode) && \
                (HSE_AUTH_CIPHER_MODE_GCM != (hseAuthCipherMode_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseCipherScheme->u8CryptoAuthCipherMode))
            {
                pImportKeyReq->pKeyInfo  = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage);
            }
        }
        else
        {
            pImportKeyReq->pKeyInfo      = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage);
        }

        pImportKeyReq->pKey[2]   = HSE_PTR_TO_HOST_ADDR(pKeyMaterial);
        pImportKeyReq->keyLen[2] = (uint16)u32KeyLength;
    }

    pImportKeyReq->targetKeyHandle = (hseKeyHandle_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseKeyHandle;

    /* Loading a key is always synchronous */
    RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR);

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_SPT_SYM_KEY_IMPORT) */

#if (STD_ON == CRYPTO_SPT_SHE_KEY_IMPORT)
/**
* @brief           Fills the import descriptor
* @details         Configures the import descriptor for SHE keys in plain format
*
* @param[in]       u8MuInstance:        MU instance where the job will be issued
* @param[in]       u8MuChannel:         MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:   Descriptor to be filled and send to HSE
* @param[in]       pKeyMaterial:        Pointer to Key Material
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: incorrect lengths
*/
static inline Std_ReturnType Crypto_Hse_SheLoadPlainKey
(
    uint8               u8MuInstance,
    uint8               u8MuChannel,
    hseSrvDescriptor_t* pHseSrvDescriptor,
    const uint8*        pKeyMaterial
)
{
    hseSheLoadPlainKeySrv_t* pSheLoadPlainKeyReq;
    Std_ReturnType           RetVal = (Std_ReturnType)E_NOT_OK;

    pHseSrvDescriptor->srvId  = HSE_SRV_ID_SHE_LOAD_PLAIN_KEY;
    pSheLoadPlainKeyReq       = &(pHseSrvDescriptor->hseSrv.sheLoadPlainKeyReq);
    pSheLoadPlainKeyReq->pKey = HSE_PTR_TO_HOST_ADDR(pKeyMaterial);

    /* Loading a key is always synchronous */
    RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR);

    return RetVal;
}

/**
* @brief           Fills the import descriptor
* @details         Configures the import descriptor for SHE keys in format encrypted
*
* @param[in]       u8MuInstance:             MU instance where the job will be issued
* @param[in]       u8MuChannel:              MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:        Descriptor to be filled and send to HSE
* @param[in]       pKeyMaterial:             Pointer to Key Material element
* @param[in]       u32MacProofKeyElemIdx:    Index of Key Mac Proof element
* @param[in]       u32CipherProofKeyElemIdx: Index of Cipher Proof element
* @param[in]       u32KeyMaterialKeyElemIdx: Index of Key Material
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: incorrect lengths
*/
static inline Std_ReturnType Crypto_Hse_SheLoadKey
(
    uint8               u8MuInstance,
    uint8               u8MuChannel,
    hseSrvDescriptor_t* pHseSrvDescriptor,
    const uint8*        pKeyMaterial,
    uint32              u32MacProofKeyElemIdx,
    uint32              u32CipherProofKeyElemIdx,
    uint32              u32KeyMaterialKeyElemIdx
)
{
    Std_ReturnType      RetVal         = (Std_ReturnType)E_NOT_OK;
    uint32              u32KeyHandle   = Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseKeyHandle;
    boolean             bM4M5Requested = FALSE;
    hseSheLoadKeySrv_t* pSheLoadKeyReq;
    /* Dummy buffers to store the SHE M4 and M5 output messages from HSE FW, when these are not requested by Crypto driver */
    uint8               au8DummyM4[CRYPTO_M4_SIZE_U32];
    uint8               au8DummyM5[CRYPTO_M5_SIZE_U32];

    pSheLoadKeyReq = &(pHseSrvDescriptor->hseSrv.sheLoadKeyReq);
    pHseSrvDescriptor->srvId = HSE_SRV_ID_SHE_LOAD_KEY;
    pSheLoadKeyReq->sheGroupIndex = (hseKeyGroupIdx_t)GET_GROUP_IDX(u32KeyHandle);

    pSheLoadKeyReq->pM1 = HSE_PTR_TO_HOST_ADDR(&pKeyMaterial[CRYPTO_INDEX_M1_U32]);
    pSheLoadKeyReq->pM2 = HSE_PTR_TO_HOST_ADDR(&pKeyMaterial[CRYPTO_INDEX_M2_U32]);
    pSheLoadKeyReq->pM3 = HSE_PTR_TO_HOST_ADDR(&pKeyMaterial[CRYPTO_INDEX_M3_U32]);

    if ((CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CipherProofKeyElemIdx) && (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32MacProofKeyElemIdx) &&
       (CRYPTO_M4_SIZE_U32 <= Crypto_aKeyElementList[u32CipherProofKeyElemIdx].u32CryptoKeyElementMaxSize)                               &&
       (CRYPTO_M5_SIZE_U32 <= Crypto_aKeyElementList[u32MacProofKeyElemIdx].u32CryptoKeyElementMaxSize))
    {
        pSheLoadKeyReq->pM4 = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32CipherProofKeyElemIdx].pCryptoElementArray);
        pSheLoadKeyReq->pM5 = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32MacProofKeyElemIdx].pCryptoElementArray);
        bM4M5Requested      = TRUE;
    }
    else
    {
        pSheLoadKeyReq->pM4 = HSE_PTR_TO_HOST_ADDR(au8DummyM4);
        pSheLoadKeyReq->pM5 = HSE_PTR_TO_HOST_ADDR(au8DummyM5);
    }

    /* Loading a key is always synchronous */
    RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR);

    /* If importing the key was successful and M4 and M5 were requested by upper layer, they were already updated by HSE in the CipherProof and MacProof key elements
       We need to update the actual length of these 2 key elements */
    if (((Std_ReturnType)E_OK == RetVal) && (TRUE == bM4M5Requested))
    {
        *Crypto_aKeyElementList[u32CipherProofKeyElemIdx].pu32CryptoElementActualSize = CRYPTO_M4_SIZE_U32;
        *Crypto_aKeyElementList[u32MacProofKeyElemIdx].pu32CryptoElementActualSize    = CRYPTO_M5_SIZE_U32;
    }

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_SPT_SHE_KEY_IMPORT) */

#if ((STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT))
/**
* @brief           Exports symmetric key or public part of a key pair.
* @details         Restrictions for export service(which apply for both SuperUser and User rights):
*                  - Key flags (of key properties) are always applied, this service can only be used if the key is exportable.
*                  - NVM/RAM symmetric keys can be exported only encrypted and authenticated.
*                  - NVM/RAM public keys (from key pair or public key slots) can be exported in plain, keys can/cannot be authenticated.
*                  - The private part of a key pair can NOT be exported (the private part is never disclosed to the host).
*                  - public keys, where the key value is stored in the application area(_PUB_EXT), can not be exported.
*                  - To export an encrypted/authenticated NVM key, the provided provision key must have the same group owner as the exported NVM key (not applicable for RAM keys).
*                  Key Information export is disabled.
*
* @param[in]       u8MuInstance:             MU instance where the job will be issued
* @param[in]       u8MuChannel:              MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:        Descriptor to be filled and send to HSE
* @param[in]       u32KeyMaterialKeyElemIdx: Index of Key Material
* @param[in]       u32KeyExportKeyElemIdx:   Index of key element where the tag or signature will be stored
* @param[in]       pResult:                  Output key element where the result will be stored
* @param[in]       pResultLength:            Pointer to the length of the output key element
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: incorrect lengths
*/
static inline Std_ReturnType Crypto_Hse_ExportSymPrivAsymPub
(
    uint8               u8MuInstance,
    uint8               u8MuChannel,
    hseSrvDescriptor_t* pHseSrvDescriptor,
    uint32              u32KeyMaterialKeyElemIdx,
    uint32              u32KeyExportKeyElemIdx,
    uint8* const        pResult,
#if (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT)
    uint32*             pResultLength
#else
    const uint32*       pResultLength
#endif /* (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) */
)
{
    hseExportKeySrv_t* pExportKeyReq;
    uint32             u32EccPubLen;
    Std_ReturnType     RetVal       = E_NOT_OK;
    const uint32       u32KeyHandle = Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseKeyHandle;
    const uint8        u8HseKeyType = Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u8HseKeyType;
#if (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT)
    boolean            bSymKey      = FALSE;
#endif /* (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) */

    pExportKeyReq = &(pHseSrvDescriptor->hseSrv.exportKeyReq);

    /* Export Service Request */
    pHseSrvDescriptor->srvId = HSE_SRV_ID_EXPORT_KEY;
    /* The key handle to be exported. */
    pExportKeyReq->targetKeyHandle = (hseKeyHandle_t)u32KeyHandle;
    /* The cipherKeyHandle can only be a provisioning key (HSE_KF_USAGE_KEY_PROVISION and HSE_KF_USAGE_ENCRYPT flags are set). */
    pExportKeyReq->cipher.cipherKeyHandle       = (hseKeyHandle_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseExportCipherKeyHandle;
    /* Authentication key handle (HSE_KF_USAGE_KEY_PROVISION and HSE_KF_USAGE_SIGN flags are set). */
    pExportKeyReq->keyContainer.authKeyHandle   = (hseKeyHandle_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseExportAuthKeyHandle;
    /* Key Information export is required only for symmetric keys. */
    pExportKeyReq->pKeyInfo = (HOST_ADDR)0U;

#if (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT)
    /* Symmetric Keys must be exported encrypted */
    if
    (
        (HSE_KEY_TYPE_AES == u8HseKeyType)
        ||
        (HSE_KEY_TYPE_HMAC == u8HseKeyType)
#if(STD_ON == CRYPTO_SPT_TDES)
        ||
        (HSE_KEY_TYPE_TDES == u8HseKeyType)
#endif /* (STD_ON == CRYPTO_SPT_TDES) */
    )
    {
        /* Remember for later the key family */
        bSymKey = TRUE;
        /* Check if the cipher key handle used for export is available.
           If the cipher key handle is not available the firmware will check the request and return an error */
        if (HSE_INVALID_KEY_HANDLE != Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseExportCipherKeyHandle)
        {
            /* Fill Cipher Scheme */
            Crypto_Hse_ExportCipherScheme(&pExportKeyReq->cipher.cipherScheme, Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportCipherScheme);

            /* AEAD does the authentication part too */
            if ((TRUE == Crypto_Hse_AuthCipherModeIsCcmOrGcm(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportCipherScheme->u8CryptoAuthCipherMode)) && \
                (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32KeyExportKeyElemIdx)
               )
            {
                /* Tag pointer point to CRYPTO_KE_EXPORT_AUTHENTICATION key element's array */
                pExportKeyReq->cipher.cipherScheme.aeadCipher.pTag        = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyExportKeyElemIdx].pCryptoElementArray);
                /* Take the tag length from CRYPTO_KE_EXPORT_AUTHENTICATION key element's size */
                pExportKeyReq->cipher.cipherScheme.aeadCipher.tagLength   = (uint16)Crypto_aKeyElementList[u32KeyExportKeyElemIdx].u32CryptoKeyElementMaxSize;
                /* Take key info form AAD data */
                pExportKeyReq->pKeyInfo                                   = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportCipherScheme->pCryptoAuthCipherAAD);
            }
        }
    }
    /* For symmetric keys authentication is requred when exported. For key pairs or public keys the authentication is optional, for this type of keys only the public key is exported */
    if ((HSE_INVALID_KEY_HANDLE != Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseExportAuthKeyHandle) && (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32KeyExportKeyElemIdx))
    {
        /* Container for authenticate scheme */
        /* The key container length is the size of the byte block to be signed (without the signature).*/
        if (bSymKey)
        {
            /* For the export of symmetric keys the key information must be placed in the container, the size of the container must be equal or greater than the sum of key information size and key(encrypted) size */
            pExportKeyReq->keyContainer.keyContainerLen = (uint16)sizeof(Crypto_xContainer);
            pExportKeyReq->keyContainer.pKeyContainer   = HSE_PTR_TO_HOST_ADDR(&Crypto_xContainer);
            /* For symmetric key information must be part of the key container */
            pExportKeyReq->pKeyInfo = HSE_PTR_TO_HOST_ADDR(&Crypto_xContainer.HseKeyInfoContainer);
        }
        else
        {
            pExportKeyReq->keyContainer.keyContainerLen = (uint16) *pResultLength;
            pExportKeyReq->keyContainer.pKeyContainer   = HSE_PTR_TO_HOST_ADDR(pResult);
        }
        /* Fill authentication scheme. */
        Crypto_Hse_ExportAuthScheme(pExportKeyReq, u32KeyMaterialKeyElemIdx, u32KeyExportKeyElemIdx, u8MuInstance);
    }
#endif /* (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) */

        /* Fill the export descriptor with required result and result length addresses */
        RetVal = Crypto_Hse_ExportResultAddr
                                           (
                                                pExportKeyReq,
                                                u8HseKeyType,
                                                pResult,
                                                pResultLength
#if (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) && (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT)
                                                , u32KeyHandle,
                                                u8MuInstance
#endif /* (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) && (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) */
                                            );

    /* Check if the size is right, only for RSA key export RetVal can be E_NOT_OK if the result size can not contain modulus n and public exponent e */
    if ((Std_ReturnType)E_OK == RetVal)
    {
        RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR);

#if ((STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) && (STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT))
        if (bSymKey && ((Std_ReturnType)E_OK == RetVal))
        {
            /* For symmetric key copy the key from container to buffer */
            Crypto_Util_MemCpy8(pResult, Crypto_xContainer.aKey, *pResultLength);
        }
#endif /* ((STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) && (STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT)) */
    }

#if (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT)
    if ((HSE_KEY_TYPE_ECC_PAIR == u8HseKeyType) || (HSE_KEY_TYPE_ECC_PUB == u8HseKeyType))
    {
        if ((Std_ReturnType)E_OK == Crypto_Hse_GetKeyInfo(u8MuInstance, u32KeyHandle))
        {
            u32EccPubLen = Crypto_GetByteLenFromBitLen((uint32)Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyBitLen);
            if ((HSE_EC_25519_ED25519 == Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.specific.eccCurveId) || (HSE_EC_25519_CURVE25519 == Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.specific.eccCurveId))
            {
                /* [SWS_Crypto_00192]The public key information for ED25519 contains a point on the curve:ED25519 Public Key = Point X. The point is stored in little endian format. */
                /* The public key information for ED448 contains a point on the curve: ED448 Public Key = Point X. The point is stored in little endian format. */
                /* Convert Point X (convert public key)*/
                Crypto_Util_SwapArrayBytes(pResult, u32EccPubLen);
            }
            else
            {
                /* [SWS_Crypto_00190]Public keys for NIST and Brainpool ECC curves are provided with their X and Y coordinates: ECC Public Key = Point X | Point Y.
                 * The points are stored in little endian format.The number of bytes for the key depends on the implementation of the curve. */
                /* Convert X (First point of Public Key)from Little Endian to Big endian */
                Crypto_Util_SwapArrayBytes(&pResult[0], u32EccPubLen);
                /* Convert Y (Second point  of Public Key)from Little Endian to Big endian */
                Crypto_Util_SwapArrayBytes(&pResult[u32EccPubLen], u32EccPubLen);
            }
        }
        else
        {
            /* If the information about the key can't be read the swap fails */
            RetVal = E_NOT_OK;
        }
    }
#endif /* (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT) */

#if (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT)
    /* Compute total length for RSA key export */
    if (((Std_ReturnType)E_OK == RetVal) && ((HSE_KEY_TYPE_RSA_PUB == u8HseKeyType) || (HSE_KEY_TYPE_RSA_PAIR == u8HseKeyType)))
    {
        *pResultLength = Crypto_Hse_apMuState[u8MuInstance]->au32SizeRsa[0] + Crypto_Hse_apMuState[u8MuInstance]->au32SizeRsa[1];
    }
#else
    (void)u32KeyExportKeyElemIdx;
#endif/* (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) */
    return RetVal;
}
#endif /* ((STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT)) */

#if ((STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) && (STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT))
/**
* @brief           Fills the export descriptor for symmetric keys
* @details         Fills the descriptor for encrypted export of a symmetric key
*
* @param[out]      pCipherScheme:        Pointer to cipher scheme contained by descriptor
* @param[in]       pExportCipherScheme:  Pointer to configured cipher scheme
*
* @returns         void
*/
static inline void Crypto_Hse_ExportCipherScheme
(
    hseCipherScheme_t*                   pCipherScheme,
    const Crypto_ExportCipherSchemeType* pExportCipherScheme
)
{
    if
    (
        (HSE_CIPHER_ALGO_AES  == (hseCipherAlgo_t)pExportCipherScheme->u8CryptoCipherAlgo)
#if(STD_ON == CRYPTO_SPT_TDES)
        ||
        (HSE_CIPHER_ALGO_3DES == (hseCipherAlgo_t)pExportCipherScheme->u8CryptoCipherAlgo)
#endif /* (STD_ON == CRYPTO_SPT_TDES) */
    )
    {
        /* Symmetric cipher scheme. */
        /* Selected symmetric cipher. */
        pCipherScheme->symCipher.cipherAlgo      = (hseCipherAlgo_t)     pExportCipherScheme->u8CryptoCipherAlgo;
        /* Selected cipher block mode. */
        pCipherScheme->symCipher.cipherBlockMode = (hseCipherBlockMode_t)pExportCipherScheme->u8CryptoCipherBlockMode;
        /* Initialization Vector length(at least 12 bytes). */
        pCipherScheme->symCipher.ivLength        =                       pExportCipherScheme->u32CryptoCipherIVLen;
        /* Initialization Vector/Nonce. */
        pCipherScheme->symCipher.pIV             = HSE_PTR_TO_HOST_ADDR (pExportCipherScheme->pCryptoCipherIV);
    }
    else if
    (
        (HSE_RSA_ALGO_NO_PADDING      == (hseRsaAlgo_t)pExportCipherScheme->u8CryptoCipherAlgo)
        ||
        (HSE_RSA_ALGO_RSAES_OAEP      == (hseRsaAlgo_t)pExportCipherScheme->u8CryptoCipherAlgo)
        ||
        (HSE_RSA_ALGO_RSAES_PKCS1_V15 == (hseRsaAlgo_t)pExportCipherScheme->u8CryptoCipherAlgo)
    )
    {
        /* RSA cipher scheme. */
        /* RSA algorithm. */
        pCipherScheme->rsaCipher.rsaAlgo                     = (hseRsaAlgo_t)pExportCipherScheme->u8CryptoCipherAlgo;
        /* Scheme for selected RSA algorithm. */
        /* RSA-OAEP scheme. */
        if (HSE_RSA_ALGO_RSAES_OAEP == (hseRsaAlgo_t)pExportCipherScheme->u8CryptoCipherAlgo)
        {
            /* The Hash algorithm for RSA OAEP padding. */
            pCipherScheme->rsaCipher.sch.rsaOAEP.hashAlgo    = (hseHashAlgo_t)      pExportCipherScheme->u8CryptoCipherRsaHashAlgo;
            /* Optional OAEP label length (it can be 0). */
            pCipherScheme->rsaCipher.sch.rsaOAEP.labelLength =                      pExportCipherScheme->u32CryptoCipherRsaLabelLen;
            /* Optional OAEP label (it can be NULL if label length is 0). */
            pCipherScheme->rsaCipher.sch.rsaOAEP.pLabel      = HSE_PTR_TO_HOST_ADDR(pExportCipherScheme->pCryptoCipherRsaLabel);
        }
    }
    else if (TRUE == Crypto_Hse_AuthCipherModeIsCcmOrGcm(pExportCipherScheme->u8CryptoAuthCipherMode))
    {
        /* Aead cipher scheme. */
        /* Authenticated cipher mode. */
        pCipherScheme->aeadCipher.authCipherMode = (hseAuthCipherMode_t)pExportCipherScheme->u8CryptoAuthCipherMode;
        /* Initialization Vector length(at least 12 bytes). */
        pCipherScheme->aeadCipher.ivLength       =                      pExportCipherScheme->u32CryptoAuthCipherIVLen;
        /* Initialization Vector/Nonce. */
        pCipherScheme->aeadCipher.pIV            = HSE_PTR_TO_HOST_ADDR(pExportCipherScheme->pCryptoAuthCipherIV);
        /* Tag pointer length. */
        pCipherScheme->aeadCipher.aadLength      =                      pExportCipherScheme->u32CryptoAuthCipherAADLen;
        /* Tag pointer . */
        pCipherScheme->aeadCipher.pAAD           = HSE_PTR_TO_HOST_ADDR(pExportCipherScheme->pCryptoAuthCipherAAD);
    }
    else
    {
        /* Should not get here */
    }
}
#endif /* ((STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) && (STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT)) */

#if ((STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) && ((STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT)))
/**
* @brief           Fills the authentication part of the descriptor for exported keys
* @details         Configures the export descriptor for authenticated export of a key
*
* @param[out]      pExportKeyReq:            Pointer to export descriptor
* @param[in]       u32KeyMaterialKeyElemIdx: Index of Key Material
* @param[in]       u32KeyExportKeyElemIdx:   Index of key element where the tag or signature will be stored
* @param[in]       u8MuInstance:             MU instance where the job will be issued
*
* @returns         void
*/
static inline void Crypto_Hse_ExportAuthScheme
(
    hseExportKeySrv_t* pExportKeyReq,
    uint32             u32KeyMaterialKeyElemIdx,
    uint32             u32KeyExportKeyElemIdx,
    uint8              u8MuInstance
)
{
    /* MAC scheme. */
    if (NULL_PTR != Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthMacScheme)
    {
        switch (Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthMacScheme->u8CryptoAuthMacAlgo)
        {
#if (STD_ON == CRYPTO_SPT_HMAC)
            case HSE_MAC_ALGO_HMAC:
                /* Select an MAC algorithm. */
                pExportKeyReq->keyContainer.authScheme.macScheme.macAlgo             = (hseMacAlgo_t)       Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthMacScheme->u8CryptoAuthMacAlgo;
                /* Specifies the hash algorithm for HMAC. SHA3 and Miyaguchi-Preneel are not supported for HMAC */
                pExportKeyReq->keyContainer.authScheme.macScheme.sch.hmac.hashAlgo   = (hseHashAlgo_t)      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthMacScheme->u8CryptoAuthHashAlgo;
                /* Get the size of the CryptoKeyElement buffer */
                Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize[0]            = (uint16)             Crypto_aKeyElementList[u32KeyExportKeyElemIdx].u32CryptoKeyElementMaxSize;
                /* Address(es) for the length(s) (uin16_t values) of the authentication tag. */
                pExportKeyReq->keyContainer.pAuthLen[0]                              = HSE_PTR_TO_HOST_ADDR(Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize);
                /* Address(es) of authentication tag(s). */
                pExportKeyReq->keyContainer.pAuth[0]                                 = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyExportKeyElemIdx].pCryptoElementArray);
                break;
#endif /* (STD_ON == CRYPTO_SPT_HMAC) */

#if (STD_ON == CRYPTO_SPT_CMAC)
            case HSE_MAC_ALGO_CMAC:
                /* Select an MAC algorithm. */
                pExportKeyReq->keyContainer.authScheme.macScheme.macAlgo             = (hseMacAlgo_t)       Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthMacScheme->u8CryptoAuthMacAlgo;
                /* Cipher algorithm for CMAC. */
                pExportKeyReq->keyContainer.authScheme.macScheme.sch.cmac.cipherAlgo = (hseCipherAlgo_t)    Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthMacScheme->u8CryptoAuthCipherAlgo;
                /* Get the size of the CryptoKeyElement buffer */
                Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize[0]            = (uint16)             Crypto_aKeyElementList[u32KeyExportKeyElemIdx].u32CryptoKeyElementMaxSize;
                /* Address(es) for the length(s) (uin16_t values) of the authentication tag. */
                pExportKeyReq->keyContainer.pAuthLen[0]                              = HSE_PTR_TO_HOST_ADDR(Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize);
                /* Address(es) of authentication tag(s). */
                pExportKeyReq->keyContainer.pAuth[0]                                 = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyExportKeyElemIdx].pCryptoElementArray);
                break;
#endif /* (STD_ON == CRYPTO_SPT_CMAC) */

#if (STD_ON == CRYPTO_SPT_GMAC)
            case HSE_MAC_ALGO_GMAC:
                /* Select an MAC algorithm. */
                pExportKeyReq->keyContainer.authScheme.macScheme.macAlgo             = (hseMacAlgo_t)       Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthMacScheme->u8CryptoAuthMacAlgo;
                /* Initialization Vector length. Zero is not allowed. */
                pExportKeyReq->keyContainer.authScheme.macScheme.sch.gmac.ivLength   =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthMacScheme->u32CryptoAuthIvLength;
                /* Initialization Vector/Nonce. */
                pExportKeyReq->keyContainer.authScheme.macScheme.sch.gmac.pIV        = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthMacScheme->pCryptoAuthIv);
                /* Get the size of the CryptoKeyElement buffer */
                Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize[0]            = (uint16)             Crypto_aKeyElementList[u32KeyExportKeyElemIdx].u32CryptoKeyElementMaxSize;
                /* Address(es) for the length(s) (uin16_t values) of the authentication tag. */
                pExportKeyReq->keyContainer.pAuthLen[0]                              = HSE_PTR_TO_HOST_ADDR(Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize);
                /* Address(es) of authentication tag(s). */
                pExportKeyReq->keyContainer.pAuth[0]                                 = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyExportKeyElemIdx].pCryptoElementArray);
                break;
#endif /* (STD_ON == CRYPTO_SPT_GMAC) */

#if (STD_ON == CRYPTO_SPT_XCBCMAC)
            case HSE_MAC_ALGO_XCBC_MAC:
                /* Select an MAC algorithm. */
                pExportKeyReq->keyContainer.authScheme.macScheme.macAlgo             = (hseMacAlgo_t)       Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthMacScheme->u8CryptoAuthMacAlgo;
                /* Get the size of the CryptoKeyElement buffer */
                Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize[0]            = (uint16)             Crypto_aKeyElementList[u32KeyExportKeyElemIdx].u32CryptoKeyElementMaxSize;
                /* Address(es) for the length(s) (uin16_t values) of the authentication tag. */
                pExportKeyReq->keyContainer.pAuthLen[0]                              = HSE_PTR_TO_HOST_ADDR(Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize);
                /* Address(es) of authentication tag(s). */
                pExportKeyReq->keyContainer.pAuth[0]                                 = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyExportKeyElemIdx].pCryptoElementArray);
                break;
#endif /* (STD_ON == CRYPTO_SPT_XCBCMAC) */

            default:
                /* Do nothing */
                break;
        }
    }
    else if (NULL_PTR != Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthSignScheme)
    {
        switch (Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthSignScheme->u8CryptoAuthSignScheme)
        {
#if (STD_ON == CRYPTO_SPT_ECDSA)
            case HSE_SIGN_ECDSA:
                /* Signature scheme.*/
                pExportKeyReq->keyContainer.authScheme.sigScheme.signSch                 = (hseSignSchemeEnum_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthSignScheme->u8CryptoAuthSignScheme;
                /* The hash algorithm. */
                pExportKeyReq->keyContainer.authScheme.sigScheme.sch.ecdsa.hashAlgo      = (hseHashAlgo_t)      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthSignScheme->u8CryptoAuthSignHashAlgo;
                /* Get the size of the CryptoKeyElement buffer allocated for each signature */
                Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize[0]                = (uint16)             Crypto_aKeyElementList[u32KeyExportKeyElemIdx].u32CryptoKeyElementMaxSize >> 1U;
                Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize[1]                = (uint16)             Crypto_aKeyElementList[u32KeyExportKeyElemIdx].u32CryptoKeyElementMaxSize >> 1U;
                /* Address(es) for the length(s) (uin16_t values) of the authentication tag. */
                pExportKeyReq->keyContainer.pAuthLen[0]                                  = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize[0]);
                pExportKeyReq->keyContainer.pAuthLen[1]                                  = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize[1]);
                /* Address(es) of authentication tag(s). */
                pExportKeyReq->keyContainer.pAuth[0]                                     = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyExportKeyElemIdx].pCryptoElementArray);
                pExportKeyReq->keyContainer.pAuth[1]                                     = HSE_PTR_TO_HOST_ADDR(&Crypto_aKeyElementList[u32KeyExportKeyElemIdx].pCryptoElementArray[Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize[0]]);
                break;
#endif /* (STD_ON == CRYPTO_SPT_ECDSA) */

#if (STD_ON == CRYPTO_SPT_EDDSA)
            case HSE_SIGN_EDDSA:
                /* Signature scheme.*/
                pExportKeyReq->keyContainer.authScheme.sigScheme.signSch                  = (hseSignSchemeEnum_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthSignScheme->u8CryptoAuthSignScheme;
                /* The length of the EDDSA context. Length of zero means no context. */
                pExportKeyReq->keyContainer.authScheme.sigScheme.sch.eddsa.contextLength  =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthSignScheme->u8CryptoAuthContextLength;
                /* The EDDSA context. Ignored if contextLength is zero. */
                pExportKeyReq->keyContainer.authScheme.sigScheme.sch.eddsa.pContext       = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthSignScheme->pCryptoAuthContext);
                /* The hash algorithm. For EDDSA this is the pre-hash algorithm.Use HSE_HASH_ALGO_NULL for pure EDDSA, or the curve's specific hash algo for standard pre-hashed EDDSA */
                pExportKeyReq->keyContainer.authScheme.sigScheme.sch.eddsa.bHashEddsa     =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthSignScheme->bCryptoHashEddsa;
                /* Get the size of the CryptoKeyElement buffer allocated for each signature */
                Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize[0]                 = (uint16)             Crypto_aKeyElementList[u32KeyExportKeyElemIdx].u32CryptoKeyElementMaxSize >> 1U;
                Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize[1]                 = (uint16)             Crypto_aKeyElementList[u32KeyExportKeyElemIdx].u32CryptoKeyElementMaxSize >> 1U;
                /* Address(es) for the length(s) (uin16_t values) of the authentication tag. */
                pExportKeyReq->keyContainer.pAuthLen[0]                                   = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize[0]);
                pExportKeyReq->keyContainer.pAuthLen[1]                                   = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize[1]);
                /* Address(es) of authentication tag(s). */
                pExportKeyReq->keyContainer.pAuth[0]                                      = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyExportKeyElemIdx].pCryptoElementArray);
                pExportKeyReq->keyContainer.pAuth[1]                                      = HSE_PTR_TO_HOST_ADDR(&Crypto_aKeyElementList[u32KeyExportKeyElemIdx].pCryptoElementArray[Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize[0]]);
                break;
#endif /* (STD_ON == CRYPTO_SPT_EDDSA) */

#if(STD_ON == CRYPTO_SPT_RSASSA_PCKS1_V15)
            case HSE_SIGN_RSASSA_PKCS1_V15:
                /* Signature scheme.*/
                pExportKeyReq->keyContainer.authScheme.sigScheme.signSch                  = (hseSignSchemeEnum_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthSignScheme->u8CryptoAuthSignScheme;
                /* The hash algorithm. For RSA schemes HSE_HASH_ALGO_NULL is treated as invalid, i.e. pre-hashed inputs are not accepted. */
                pExportKeyReq->keyContainer.authScheme.sigScheme.sch.rsaPkcs1v15.hashAlgo = (hseHashAlgo_t)      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthSignScheme->u8CryptoAuthSignHashAlgo;
                /* Get the size of the CryptoKeyElement buffer */
                Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize[0]                 = (uint16)             Crypto_aKeyElementList[u32KeyExportKeyElemIdx].u32CryptoKeyElementMaxSize;
                /* Address(es) for the length(s) (uin16_t values) of the authentication tag. */
                pExportKeyReq->keyContainer.pAuthLen[0]                                   = HSE_PTR_TO_HOST_ADDR(Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize);
                /* Address(es) of authentication tag(s). */
                pExportKeyReq->keyContainer.pAuth[0]                                      = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyExportKeyElemIdx].pCryptoElementArray);
                break;
#endif /* (STD_ON == CRYPTO_SPT_RSASSA_PCKS1_V15) */

#if(STD_ON == CRYPTO_SPT_RSAASSA_PSS)
            case HSE_SIGN_RSASSA_PSS:
                /* Signature scheme.*/
                pExportKeyReq->keyContainer.authScheme.sigScheme.signSch                  = (hseSignSchemeEnum_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthSignScheme->u8CryptoAuthSignScheme;
                pExportKeyReq->keyContainer.authScheme.sigScheme.sch.rsaPss.saltLength    =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthSignScheme->u8CryptoAuthSaltLength;
                /* The hash algorithm. For RSA schemes HSE_HASH_ALGO_NULL is treated as invalid, i.e. pre-hashed inputs are not accepted. */
                pExportKeyReq->keyContainer.authScheme.sigScheme.sch.rsaPss.hashAlgo      =                      Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].pHseExportAuthSignScheme->u8CryptoAuthSignHashAlgo;
                /* Get the size of the CryptoKeyElement buffer */
                Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize[0]                 = (uint16)             Crypto_aKeyElementList[u32KeyExportKeyElemIdx].u32CryptoKeyElementMaxSize;
                /* Address(es) for the length(s) (uin16_t values) of the authentication tag. */
                pExportKeyReq->keyContainer.pAuthLen[0]                                   = HSE_PTR_TO_HOST_ADDR(Crypto_Hse_apMuState[u8MuInstance]->au16AuthResultSize);
                /* Address(es) of authentication tag(s). */
                pExportKeyReq->keyContainer.pAuth[0]                                      = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32KeyExportKeyElemIdx].pCryptoElementArray);
                break;
#endif /* (STD_ON == CRYPTO_SPT_RSAASSA_PSS) */

            default:
                /* Do nothing */
                break;
        }
    }
    else
    {
        /* Do nothing */
    }
}
#endif /* ((STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) && ((STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT))) */

#if ((STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT))
/**
* @brief           Fills the descriptor for exported keys
* @details         Configures the export descriptor with required result and result length addresses
*
* @param[out]      pExportKeyReq: Pointer to export descriptor
* @param[in]       u8HseKeyType:  Configured key type
* @param[in]       pResult:       Output key element where the result will be stored
* @param[in]       pResultLength: Pointer to the length of the output key element
* @param[in]       u32KeyHandle:  Handle used to identify a key in the firmware
* @param[in]       u8MuInstance:  MU instance where the job will be issued
*
* @returns         Std_ReturnType
*/
static inline Std_ReturnType Crypto_Hse_ExportResultAddr
(
    hseExportKeySrv_t*    pExportKeyReq,
    uint8                 u8HseKeyType,
    const uint8* const    pResult,
    const uint32*         pResultLength
#if (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) && (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT)
    , const uint32        u32KeyHandle,
    uint8                 u8MuInstance
#endif /* (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT) && (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) */
)
{
    Std_ReturnType RetVal = E_OK;

    switch (u8HseKeyType)
    {
#if (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT)
        /* Symmetric Key Export */
#if (STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT)
#if (STD_ON == CRYPTO_SPT_AES)
        case HSE_KEY_TYPE_AES :
#endif /* (STD_ON == CRYPTO_SPT_AES) */
#if (STD_ON == CRYPTO_SPT_HMAC)
        case HSE_KEY_TYPE_HMAC:
#endif /* (STD_ON == CRYPTO_SPT_HMAC) */
#if(STD_ON == CRYPTO_SPT_TDES)
        case HSE_KEY_TYPE_TDES:
#endif /* (STD_ON == CRYPTO_SPT_TDES) */
            /* The symmetric key (e.g AES, HMAC). */
            pExportKeyReq->pKey[2]    = HSE_PTR_TO_HOST_ADDR(&Crypto_xContainer.aKey);
            /* Length of the buffer used for storing the exported key, if the buffer is too small the export will fail */
            /* Address of uint16 value of the length (in bytes) for the above buffer.
               After a succesful request it provides the length of the encrypted key.*/
            pExportKeyReq->pKeyLen[2] = HSE_PTR_TO_HOST_ADDR(pResultLength);
            break;
#endif /* (STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) */

#if (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT)
        /* Asymmetric Key Export */
        case HSE_KEY_TYPE_RSA_PUB:
        /* fall-through */
        case HSE_KEY_TYPE_RSA_PAIR:
            /* Obtain key information to know the size of modulus n and exponent e. */
            if ((Std_ReturnType)E_OK == Crypto_Hse_GetKeyInfo(u8MuInstance, u32KeyHandle))
            {
                /* Check if the length of the pResult is big enough to contain the key */
                if (*pResultLength < (((uint32)Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyBitLen >> 3) + (uint32)(Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.specific.pubExponentSize)))
                {
                    /* Buffer is not big enough */
                    RetVal = E_NOT_OK;
                }
                else
                {
                    /* Use the following variables to save the returned length */
                    /* RSA public modulus n size. */
                    Crypto_Hse_apMuState[u8MuInstance]->au32SizeRsa[0] = ((uint32)Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyBitLen >> 3);
                    /* RSA public exponent e size.*/
                    Crypto_Hse_apMuState[u8MuInstance]->au32SizeRsa[1] = (Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.specific.pubExponentSize);
                    /* RSA public exponent e. */
                    pExportKeyReq->pKey[1]    = HSE_PTR_TO_HOST_ADDR((&pResult[Crypto_Hse_apMuState[u8MuInstance]->au32SizeRsa[0]]));
                    /* Length of the buffer used for storing the exported key, if the buffer is too small the export will fail */
                    /* Address of uint16 value of the length (in bytes) for the above buffer.
                       After a succesful request it provides the length of the public exponent e.*/
                    pExportKeyReq->pKeyLen[1] = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au32SizeRsa[1]);
                    /* RSA public modulus n. */
                    pExportKeyReq->pKey[0]    = HSE_PTR_TO_HOST_ADDR(pResult);
                    /* Length of the buffer used for storing the exported key, if the buffer is too small the export will fail */
                    /* Address of uint16 value of the length (in bytes) for the above buffer.
                       After a succesful request it provides the length of the public modulus n.*/
                    pExportKeyReq->pKeyLen[0] = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au32SizeRsa[0]);
                }
            }
            break;
#endif /* (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) */
#endif/*(STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT)*/

#if (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT)
        case HSE_KEY_TYPE_ECC_PAIR:
        case HSE_KEY_TYPE_ECC_PUB :
            /* ECC the x and y coordinate of the public key must be passed one after another(the byte length of the stored value of the public key must be twice the byte length of the prime p).
               ED25519 point x. */
            pExportKeyReq->pKey[0]  = HSE_PTR_TO_HOST_ADDR(pResult);
            /* Length of the buffer used for storing the exported key, if the buffer is too small the export will fail */
            /* Address of uint16 value of the length (in bytes) for the above buffer.
               After a succesful request it provides the total length of the key.*/
            pExportKeyReq->pKeyLen[0] = HSE_PTR_TO_HOST_ADDR(pResultLength);
            break;
#endif /* (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT) */

        default:
            /* Should not get here */
            break;
    }

    return RetVal;
}
#endif /* ((STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT)) */

#if (STD_ON == CRYPTO_SPT_SHE_KEY_EXPORT)
/**
* @brief           Fills the export descriptor
* @details         Configures the export descriptor for SHE RAM keys
*
* @param[in]       u8MuInstance:        MU instance where the job will be issued
* @param[in]       u8MuChannel:         MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:   Descriptor to be filled and send to HSE
* @param[in]       pResult:             Output key element where the result will be stored
* @param[in]       pResultLength:       Pointer to the length of the output key element
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: incorrect lengths
*/
static inline Std_ReturnType Crypto_Hse_ExportRamSheKey
(
    uint8               u8MuInstance,
    uint8               u8MuChannel,
    hseSrvDescriptor_t* pHseSrvDescriptor,
    const uint8*        pResult
)
{
    hseSheExportRamKeySrv_t* pSheExportRamKeyReq;
    Std_ReturnType           RetVal = (Std_ReturnType)E_NOT_OK;

    pSheExportRamKeyReq      = &(pHseSrvDescriptor->hseSrv.sheExportRamKeyReq);
    pHseSrvDescriptor->srvId = HSE_SRV_ID_SHE_EXPORT_RAM_KEY;

    pSheExportRamKeyReq->pM1 = HSE_PTR_TO_HOST_ADDR(&pResult[CRYPTO_INDEX_M1_U32]);
    pSheExportRamKeyReq->pM2 = HSE_PTR_TO_HOST_ADDR(&pResult[CRYPTO_INDEX_M2_U32]);
    pSheExportRamKeyReq->pM3 = HSE_PTR_TO_HOST_ADDR(&pResult[CRYPTO_INDEX_M3_U32]);
    pSheExportRamKeyReq->pM4 = HSE_PTR_TO_HOST_ADDR(&pResult[CRYPTO_INDEX_M4_U32]);
    pSheExportRamKeyReq->pM5 = HSE_PTR_TO_HOST_ADDR(&pResult[CRYPTO_INDEX_M5_U32]);

    RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR);

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_SPT_SHE_KEY_EXPORT) */

#if (STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT)
/**
* @brief           Fills the import descriptor for ECC keys
* @details         Configures the improt descriptor for ECC keys in plain format
*
* @param[in]       u8MuInstance:             MU instance where the job will be issued
* @param[in]       u8MuChannel:              MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:        Descriptor to be filled and send to HSE
* @param[in]       pKey:                     Pointer to the Public key that will be set
* @param[in]       u32KeyLength:                Length of Public key
* @param[in]       u32KeyMaterialKeyElemIdx: Index of Key Material
* @param[in]       u8CurveId:                Curve id configured by user
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: incorrect lengths
*/
static inline Std_ReturnType Crypto_Hse_EccLoadPlainPubKey
(
    uint8               u8MuInstance,
    uint8               u8MuChannel,
    hseSrvDescriptor_t* pHseSrvDescriptor,
    const uint8*        pKey,
    uint32              u32KeyLength,
    uint32              u32KeyMaterialKeyElemIdx,
    uint8               u8CurveId
)
{
    hseImportKeySrv_t* pImportKeyReq;
    uint32             u32pubKeyByteLen;
    uint32             u32PrivKeyByteLen;
    Std_ReturnType     RetVal          = (Std_ReturnType)E_NOT_OK;
    uint16             u16EccKeyBitlen = Crypto_Hse_EccKeyBitLen(u8CurveId);

    pImportKeyReq     = &pHseSrvDescriptor->hseSrv.importKeyReq;
    u32pubKeyByteLen  = Crypto_GetByteLenFromBitLen((uint32)u16EccKeyBitlen);
    u32PrivKeyByteLen = Crypto_GetByteLenFromBitLen((uint32)u16EccKeyBitlen);
    if (CRYPTO_KE_INVALID_CURVE_ID_U8 != u8CurveId)
    {
        /* Set the public key length for x & y if curve not ED25519 or ED448 */
        if (
                (HSE_EC_25519_ED25519       != (hseEccCurveId_t)u8CurveId) &&
                (HSE_EC_25519_CURVE25519    != (hseEccCurveId_t)u8CurveId)
#if (STD_ON == CRYPTO_SPT_EC448_ED448)
                && (HSE_EC_448_ED448        != (hseEccCurveId_t)u8CurveId)
#endif /* (STD_ON == CRYPTO_SPT_EC448_ED448) */
#if (STD_ON == CRYPTO_SPT_EC448_CURVE448)
                && (HSE_EC_448_CURVE448     != (hseEccCurveId_t)u8CurveId)
#endif /* (STD_ON == CRYPTO_SPT_EC448_CURVE448) */
           )
        {
            u32pubKeyByteLen =  2U * u32pubKeyByteLen;
        }
        /* In case of custom ecc curve id */
        if (0U == u32pubKeyByteLen)
        {
            u32pubKeyByteLen = u32KeyLength;
        }
        if (u32pubKeyByteLen == u32KeyLength)
        {
            /* Copy the key in a buffer used for big endian conversion */
            Crypto_Util_MemCpy8(Crypto_Hse_apMuState[u8MuInstance]->au8KeyBigEndForm, pKey, u32KeyLength);

            /* Check the curve type to see if the conversion must be made for X and Y or just for X */
            if (
                (HSE_EC_25519_ED25519       != (hseEccCurveId_t)u8CurveId) &&
                (HSE_EC_25519_CURVE25519    != (hseEccCurveId_t)u8CurveId)
#if (STD_ON == CRYPTO_SPT_EC448_ED448)
                && (HSE_EC_448_ED448        != (hseEccCurveId_t)u8CurveId)
#endif /* (STD_ON == CRYPTO_SPT_EC448_ED448) */
#if (STD_ON == CRYPTO_SPT_EC448_CURVE448)
                && (HSE_EC_448_CURVE448     != (hseEccCurveId_t)u8CurveId)
#endif /* (STD_ON == CRYPTO_SPT_EC448_CURVE448) */
              )
            {
                /* [SWS_Crypto_00190]Public keys for NIST and Brainpool ECC curves are provided with their X and Y coordinates: ECC Public Key = Point X | Point Y.
                 * The points are stored in little endian format.The number of bytes for the key depends on the implementation of the curve. */
                /* Public key is composed of  X(PubKeyLen/2) | Y (PubKeyLen/2)*/
                /* Convert X (First point of Public Key)from Little Endian to Big endian */
                Crypto_Util_SwapArrayBytes(&Crypto_Hse_apMuState[u8MuInstance]->au8KeyBigEndForm[0], (u32pubKeyByteLen >> 1U));
                /* Convert Y (Second point  of Public Key)from Little Endian to Big endian */
                Crypto_Util_SwapArrayBytes(&Crypto_Hse_apMuState[u8MuInstance]->au8KeyBigEndForm[u32pubKeyByteLen >> 1U], (u32pubKeyByteLen >> 1U));
            }
            else
            {
                /* [SWS_Crypto_00192]The public key information for ED25519 contains a point on the curve:ED25519 Public Key = Point X. The point is stored in little endian format. */
                /* The public key information for ED448 contains a point on the curve: ED448 Public Key = Point X. The point is stored in little endian format. */
                /* Convert X (Public Key)from Little Endian to Big endian */
                Crypto_Util_SwapArrayBytes(&Crypto_Hse_apMuState[u8MuInstance]->au8KeyBigEndForm[0], u32pubKeyByteLen);
            }

            Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyType             = (hseKeyType_t) Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u8HseKeyType;
            Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyFlags            = (hseKeyFlags_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u16HseKeyFlags;
            Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.specific.eccCurveId = (hseEccCurveId_t)u8CurveId;
            Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyBitLen           =  u16EccKeyBitlen;
            Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyCounter          =                Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseKeyCounter;
            Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.smrFlags            = (hseSmrFlags_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseKeySmrFlags;
            /* If Crypto_Hse_EccKeyBitLen() returns 0 a custom ECC ID is used -> take the key bit len from configuration */
            if (0U == Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyBitLen)
            {
                Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyBitLen = (uint16)Crypto_GetBitLenFromByteLen(Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32CryptoKeyElementMaxSize);
            }
            pHseSrvDescriptor->srvId       = HSE_SRV_ID_IMPORT_KEY;
            pImportKeyReq->pKeyInfo        = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage);
            pImportKeyReq->pKey[0]         = HSE_PTR_TO_HOST_ADDR(Crypto_Hse_apMuState[u8MuInstance]->au8KeyBigEndForm);
            pImportKeyReq->keyLen[0]       = (uint16)u32pubKeyByteLen;
            pImportKeyReq->pKey[2]         = (HOST_ADDR)0U;
            pImportKeyReq->keyLen[2]       = (uint16)u32PrivKeyByteLen;
            pImportKeyReq->targetKeyHandle = Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseKeyHandle;

            pImportKeyReq->cipher.cipherKeyHandle     = HSE_INVALID_KEY_HANDLE;
            pImportKeyReq->keyContainer.authKeyHandle = HSE_INVALID_KEY_HANDLE;
 #if (STD_ON == CRYPTO_ECC_KEY_FORMAT)
            /* Support for HSE ECC key format */
            pImportKeyReq->keyFormat.eccKeyFormat     = HSE_KEY_FORMAT_ECC_PUB_RAW;
#endif /* (STD_ON == CRYPTO_ECC_KEY_FORMAT) */

            RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR);
        }
    }
    return RetVal;
}

/**
* @brief           Fills the import descriptor for ECC keys
* @details         Configures the improt descriptor for ECC keys in plain format
*
* @param[in]       u8MuInstance:             MU instance where the job will be issued
* @param[in]       u8MuChannel:              MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:        Descriptor to be filled and send to HSE
* @param[in]       pKey:                     Pointer to the Public key that will be set
* @param[in]       u32KeyLength:                Length of Public key
* @param[in]       u32KeyMaterialKeyElemIdx: Index of Key Material
* @param[in]       u8CurveId:                Curve id configured by user
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: incorrect lengths
*/
static inline Std_ReturnType Crypto_Hse_EccLoadPlainPairKey
(
    uint8               u8MuInstance,
    uint8               u8MuChannel,
    hseSrvDescriptor_t* pHseSrvDescriptor,
    const uint8*        pKey,
    uint32              u32KeyLength,
    uint32              u32KeyMaterialKeyElemIdx,
    uint8               u8CurveId
)
{
    hseImportKeySrv_t* pImportKeyReq;
    Std_ReturnType     RetVal            = (Std_ReturnType)E_NOT_OK;
    uint32             u32pubKeyByteLen  = Crypto_GetByteLenFromBitLen((uint32)Crypto_Hse_EccKeyBitLen(u8CurveId));
    uint32             u32PrivKeyByteLen = Crypto_GetByteLenFromBitLen((uint32)Crypto_Hse_EccKeyBitLen(u8CurveId));

    pImportKeyReq  = &pHseSrvDescriptor->hseSrv.importKeyReq;

    if (CRYPTO_KE_INVALID_CURVE_ID_U8 != u8CurveId)
    {
        /* Copy the key in a buffer used for big endian conversion */
        Crypto_Util_MemCpy8(Crypto_Hse_apMuState[u8MuInstance]->au8KeyBigEndForm, pKey, u32KeyLength);

        /* Set the public key length for x & y if curve not ED25519 */
        if (
                (HSE_EC_25519_ED25519       != (hseEccCurveId_t)u8CurveId) &&
                (HSE_EC_25519_CURVE25519    != (hseEccCurveId_t)u8CurveId)
#if (STD_ON == CRYPTO_SPT_EC448_ED448)
                && (HSE_EC_448_ED448        != (hseEccCurveId_t)u8CurveId)
#endif /* (STD_ON == CRYPTO_SPT_EC448_ED448) */
#if (STD_ON == CRYPTO_SPT_EC448_CURVE448)
                && (HSE_EC_448_CURVE448     != (hseEccCurveId_t)u8CurveId)
#endif /* (STD_ON == CRYPTO_SPT_EC448_CURVE448) */
           )
        {
            /* Public key is composed of  X | Y */
            u32pubKeyByteLen =  2U * u32pubKeyByteLen;

            /* [SWS_Crypto_00190]Public keys for NIST and Brainpool ECC curves are provided with their X and Y coordinates: ECC Public Key = Point X | Point Y.
             * The points are stored in little endian format.The number of bytes for the key depends on the implementation of the curve. */
            /* Convert X (First point of Public Key)from Little Endian to Big endian */
            Crypto_Util_SwapArrayBytes(&Crypto_Hse_apMuState[u8MuInstance]->au8KeyBigEndForm[0], u32pubKeyByteLen >> 1U);
            /* Convert Y (Second point  of Public Key)from Little Endian to Big endian */
            Crypto_Util_SwapArrayBytes(&Crypto_Hse_apMuState[u8MuInstance]->au8KeyBigEndForm[u32pubKeyByteLen >> 1U], u32pubKeyByteLen >> 1U);

            /* [SWS_Crypto_00191]Private keys for NIST and Brainpool ECC curves are provided with their X and Y coordinates and an additional scalar:ECC Private Key = Point X | Point Y | Scalar.
             * The points and the scalar are stored in little endian. */
            /* Convert Private Key from Little Endian to Big endian */
            Crypto_Util_SwapArrayBytes(&Crypto_Hse_apMuState[u8MuInstance]->au8KeyBigEndForm[u32pubKeyByteLen],u32PrivKeyByteLen);

            pImportKeyReq->pKey[0]   = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au8KeyBigEndForm[0]);
            pImportKeyReq->keyLen[0] = (uint16)u32pubKeyByteLen;

            pImportKeyReq->pKey[2]   = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au8KeyBigEndForm[u32pubKeyByteLen]);
            pImportKeyReq->keyLen[2] = (uint16)u32PrivKeyByteLen;
        }
        else
        {
            /* The private key information for ED448 contains a random constant and the point X on the curve: ED448 Private Key = Seed K | Point X. */
            /* [SWS_Crypto_00193]The private key information for ED25519 contains a random constant and the point X on the curve:ED25519 Private Key = Seed K | Point X.
             * The point and the seed are stored in little endian format. */
            /* Convert SeedK (Convert Private key) */
            Crypto_Util_SwapArrayBytes(&Crypto_Hse_apMuState[u8MuInstance]->au8KeyBigEndForm[0], u32PrivKeyByteLen);

            /* [SWS_Crypto_00192]The public key information for ED25519 contains a point on the curve:ED25519 Public Key = Point X. The point is stored in little endian format. */
            /* The public key information for ED448 contains a point on the curve: ED448 Public Key = Point X. The point is stored in little endian format. */
            /* Convert Point X (convert public key)*/
            Crypto_Util_SwapArrayBytes(&Crypto_Hse_apMuState[u8MuInstance]->au8KeyBigEndForm[u32PrivKeyByteLen], u32pubKeyByteLen);

            pImportKeyReq->pKey[0]   = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au8KeyBigEndForm[u32PrivKeyByteLen]);
            pImportKeyReq->keyLen[0] = (uint16)u32pubKeyByteLen;

            pImportKeyReq->pKey[2]   = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au8KeyBigEndForm[0]);
            pImportKeyReq->keyLen[2] = (uint16)u32PrivKeyByteLen;
        }

        if ((u32pubKeyByteLen + u32PrivKeyByteLen) == u32KeyLength)
        {
            Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyType             = (hseKeyType_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u8HseKeyType;
            Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyFlags            = (hseKeyFlags_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u16HseKeyFlags;
            Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.specific.eccCurveId = (hseEccCurveId_t)u8CurveId;
            Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyBitLen           = Crypto_Hse_EccKeyBitLen(u8CurveId);
            Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyCounter          =                Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseKeyCounter;
            Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.smrFlags            = (hseSmrFlags_t)Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseKeySmrFlags;

            pHseSrvDescriptor->srvId       = HSE_SRV_ID_IMPORT_KEY;
            pImportKeyReq->pKeyInfo        = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage);
            pImportKeyReq->targetKeyHandle = Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseKeyHandle;

            /* Only for encrypted ECC is needed*/
            pImportKeyReq->cipher.cipherKeyHandle     = HSE_INVALID_KEY_HANDLE;
            pImportKeyReq->keyContainer.authKeyHandle = HSE_INVALID_KEY_HANDLE;
#if (STD_ON == CRYPTO_ECC_KEY_FORMAT)
            /* Support for HSE ECC key format */
            pImportKeyReq->keyFormat.eccKeyFormat     = HSE_KEY_FORMAT_ECC_PUB_RAW;
#endif /* (STD_ON == CRYPTO_ECC_KEY_FORMAT) */

            RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR);
        }
    }
    return RetVal;
}
#endif /* (STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT) */

#if ((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC))
/**
* @brief           Signature generation or verification.
* @details         A signature is generated for the message provided or a verification of the signature is done for RSA PSS, RSA PKCS1 V1.5, ECDSA, EDDSA.
*
* @param[in]       u8MuInstance:         MU instance where the job will be issued
* @param[in]       u8MuChannel:          MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       pJob:                 Pointer to the job
* @param[in]       u8StreamId:           Identifier of the HSE stream used for the job
* @param[inout]    pbReqSentOverChannel: Pointer to a boolean storing channel usage
*
* @returns         Std_ReturnType
*                   E_OK:     Request successful.
*                   E_NOT_OK: Request failed.
*/
static inline Std_ReturnType Crypto_Hse_SignRequest
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
)
{
    hseSignSrv_t*                             pSignSrv;
    Std_ReturnType                            RetVal                = (Std_ReturnType)E_NOT_OK;
    uint32                                    u32CryptoKeyIdx       = Crypto_Util_GetKeyIdx(Crypto_Util_GetJobKey(pJob));
    uint32                                    u32SignKeyElemIdx     = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KE_SIGNATURE_KEY);
    uint8                                     u8AlgoFamily          = (uint8)pJob->jobPrimitiveInfo->primitiveInfo->algorithm.family;
    boolean                                   bError                = FALSE;
    const Crypto_JobPrimitiveInputOutputType* pPrimitiveInputOutput = &pJob->jobPrimitiveInputOutput;
    const Crypto_PrimitiveInfoType*           pPrimitiveInfo        = pJob->jobPrimitiveInfo->primitiveInfo;

    if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32SignKeyElemIdx)
    {
        pSignSrv = &(pHseSrvDescriptor->hseSrv.signReq);

        pHseSrvDescriptor->srvId      = HSE_SRV_ID_SIGN;
        pSignSrv->accessMode          = Crypto_Hse_ConfigureAccessMode(pPrimitiveInputOutput->mode);
        pSignSrv->sgtOption           = HSE_SGT_OPTION_NONE;
        pSignSrv->keyHandle           = (hseKeyHandle_t)Crypto_aKeyElementList[u32SignKeyElemIdx].u32HseKeyHandle;
        pSignSrv->streamId            = u8StreamId;
        /* Load the input message */
        pSignSrv->inputLength         = pPrimitiveInputOutput->inputLength;
        pSignSrv->pInput              = HSE_PTR_TO_HOST_ADDR(pPrimitiveInputOutput->inputPtr);
        pSignSrv->bInputIsHashed      = FALSE;

        /* Choose the direction of the signature */
        if (CRYPTO_SIGNATUREGENERATE == pPrimitiveInfo->service)
        {
            pSignSrv->authDir = HSE_AUTH_DIR_GENERATE;
            Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel] = *(pPrimitiveInputOutput->outputLengthPtr);
        }
        else if (CRYPTO_SIGNATUREVERIFY == pPrimitiveInfo->service)
        {
            pSignSrv->authDir = HSE_AUTH_DIR_VERIFY;
            Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel] = pPrimitiveInputOutput->secondaryInputLength;
        }
        else
        {
            /* Should not get here */
            /* If this point is reached, do not send message to HSE */
            bError = TRUE;
        }

        if (FALSE == bError)
        {
            bError = Crypto_Hse_FillSignRequestDescriptor(u8MuInstance, u8MuChannel, pSignSrv, pJob, u32SignKeyElemIdx);

            /* Check if the descriptor is valid */
            if (FALSE == bError)
            {
                RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
                /* Mark the request as sent */
                *pbReqSentOverChannel = TRUE;
            }
        }
    }
    if ((CRYPTO_PROCESSING_SYNC == pJob->jobPrimitiveInfo->processingType) && ((uint8)CRYPTO_OPERATIONMODE_FINISH == (uint8)(((uint8)(pJob->jobPrimitiveInputOutput.mode)) & CRYPTO_OPERATIONMODE_FINISH_MASK_U8)))
    {
        if (CRYPTO_SIGNATUREVERIFY == pPrimitiveInfo->service)
        {
            /*[SWS_Crypto_00071]If the job is synchronous, fill the VerifyPtr */
            Crypto_Hse_FillVerifyPtr(pJob, RetVal);
        }
        /* [SWS_Crypto_00135], [SWS_Crypto_00071] Fill the output length pointer with the result length when FINISH is set for signature generation */
        else if ((Std_ReturnType)E_OK == RetVal)
        {
            if ((uint8)CRYPTO_ALGOFAM_RSA == u8AlgoFamily)
            {
                *pPrimitiveInputOutput->outputLengthPtr = Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel];
            }
            else
            {
                *pPrimitiveInputOutput->outputLengthPtr = Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel] << 1;
            }
        }
        else
        {
            /* Should not get here */
        }
    }
    return RetVal;
}

/**
* @brief           Fills signature descriptor.
* @details         Fills signature descriptor based on the configured job.
*
* @param[in]       u8MuInstance:         MU instance where the job will be issued
* @param[in]       u8MuChannel:          MU channel where the job will be issued
* @param[out]      pSignSrv:             Signature descriptor to be filled and send to HSE
* @param[in]       pJob:                 Pointer to the job
* @param[in]       u32SignKeyElemIdx:    Holds the index of the Crypto Key Element
*
* @returns         boolean
*                   False:     Descriptor filled successful.
*                   True:      Parameter invalid.
*/
static inline boolean Crypto_Hse_FillSignRequestDescriptor
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSignSrv_t*         pSignSrv,
    const Crypto_JobType* pJob,
    const uint32          u32SignKeyElemIdx
)
{
    uint8                                     u8AlgoFamily          = (uint8)pJob->jobPrimitiveInfo->primitiveInfo->algorithm.family;
    const Crypto_PrimitiveInfoType*           pPrimitiveInfo        = pJob->jobPrimitiveInfo->primitiveInfo;
    const Crypto_JobPrimitiveInputOutputType* pPrimitiveInputOutput = &pJob->jobPrimitiveInputOutput;
    uint32                                    u32JobKeyLength       = Crypto_GetByteLenFromBitLen(pJob->jobPrimitiveInfo->primitiveInfo->algorithm.keyLength);
    boolean                                   bError                = FALSE;
    uint8                                     u8AlgoSecFamily       = (uint8)pJob->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily;
#if (STD_ON == CRYPTO_SPT_HASH)
    Crypto_Hse_HashConfigType                 HashAlgo              = Crypto_Hse_ConfigureHashAlgo(u8AlgoSecFamily);
#endif /* (STD_ON == CRYPTO_SPT_HASH) */

    /* Fill the signature descriptor based on algorithm family used */
    switch (u8AlgoFamily)
    {
#if (STD_ON == CRYPTO_SPT_RSA)
        case (uint8)CRYPTO_ALGOFAM_RSA:
            /* Setup the Salt Length */
            switch (pPrimitiveInfo->algorithm.mode)
            {
                case CRYPTO_ALGOMODE_RSASSA_PSS:
                    pSignSrv->signScheme.signSch             = HSE_SIGN_RSASSA_PSS;
#if (STD_ON == CRYPTO_SPT_HASH)
                    pSignSrv->signScheme.sch.rsaPss.hashAlgo = HashAlgo.HseHashAlgo;
                    if (((HSE_HASH_ALGO_SHA2_512 == HashAlgo.HseHashAlgo) || (HSE_HASH_ALGO_SHA3_512 == HashAlgo.HseHashAlgo)) &&
                        (1024U == Crypto_GetBitLenFromByteLen(Crypto_aKeyElementList[u32SignKeyElemIdx].u32CryptoKeyElementMaxSize)))
                    {
                        pSignSrv->signScheme.sch.rsaPss.saltLength = 62U;
                    }
                    else
                    {
                        pSignSrv->signScheme.sch.rsaPss.saltLength = HashAlgo.u32HashLength;
                    }
#endif /* (STD_ON == CRYPTO_SPT_HASH) */
                    break;

                case CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5:
                       pSignSrv->signScheme.signSch                  = HSE_SIGN_RSASSA_PKCS1_V15;
#if (STD_ON == CRYPTO_SPT_HASH)
                    pSignSrv->signScheme.sch.rsaPkcs1v15.hashAlgo = HashAlgo.HseHashAlgo;
#endif /* (STD_ON == CRYPTO_SPT_HASH) */
                    break;

                default:
                    /* Should not get here */
                    /* If this point is reached, do not send message to HSE */
                    bError = TRUE;
                    break;
            }

            if (CRYPTO_SIGNATUREGENERATE == pPrimitiveInfo->service)
            {
                pSignSrv->pSignature[0]       = HSE_PTR_TO_HOST_ADDR(pPrimitiveInputOutput->outputPtr);
                pSignSrv->pSignature[1]       = (HOST_ADDR)0U; /* To be removed after fw stops additional checking */
                pSignSrv->pSignatureLength[0] = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel]);
                pSignSrv->pSignatureLength[1] = (HOST_ADDR)0U; /* To be removed after fw stops additional checking */
            }
            else
            {
                pSignSrv->pSignature[0]       = HSE_PTR_TO_HOST_ADDR(pPrimitiveInputOutput->secondaryInputPtr);
                pSignSrv->pSignatureLength[0] = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel]);
            }
            break;
#endif /* (STD_ON == CRYPTO_SPT_RSA) */

#if (STD_ON == CRYPTO_SPT_ECC)
        case (uint8)CRYPTO_ALGOFAM_ECCNIST:
            if (u32JobKeyLength <=  Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel])
            {
                pSignSrv->signScheme.signSch            = HSE_SIGN_ECDSA;
#if (STD_ON == CRYPTO_SPT_HASH)
                pSignSrv->signScheme.sch.ecdsa.hashAlgo = HashAlgo.HseHashAlgo;
#endif /* (STD_ON == CRYPTO_SPT_HASH) */
                /* Compute signature length */
                Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel] =(uint32)(Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel] >> 1U);

                if (CRYPTO_SIGNATUREGENERATE == pPrimitiveInfo->service)
                {
                    pSignSrv->pSignature[0]       = HSE_PTR_TO_HOST_ADDR(pPrimitiveInputOutput->outputPtr);
                    pSignSrv->pSignatureLength[0] = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel]);
                    pSignSrv->pSignature[1]       = HSE_PTR_TO_HOST_ADDR((&pPrimitiveInputOutput->outputPtr[Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel]]));
                    pSignSrv->pSignatureLength[1] = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel]);
                }
                else
                {
                    pSignSrv->pSignature[0]       = HSE_PTR_TO_HOST_ADDR(pPrimitiveInputOutput->secondaryInputPtr);
                    pSignSrv->pSignature[1]       = HSE_PTR_TO_HOST_ADDR((&pPrimitiveInputOutput->secondaryInputPtr[Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel]]));
                    pSignSrv->pSignatureLength[0] = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel]);
                    pSignSrv->pSignatureLength[1] = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel]);
                }
            }
            else
            {
                /* If the length is not the expected one */
                bError = TRUE;
            }
            break;

        case (uint8)CRYPTO_ALGOFAM_ED25519:
        /* Walk through */
        case (uint8)CRYPTO_ALGOFAM_ED448:
            if (u32JobKeyLength <= Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel])
            {
                pSignSrv->signScheme.signSch = HSE_SIGN_EDDSA;
                /* Input is not hashed */
                pSignSrv->signScheme.sch.eddsa.bHashEddsa = FALSE;
                /* Compute the length of the signature */
                Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel] =(uint32)(Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel] >> 1U);

                if (CRYPTO_SIGNATUREGENERATE == pPrimitiveInfo->service)
                {
                    pSignSrv->pSignature[0]       = HSE_PTR_TO_HOST_ADDR(pPrimitiveInputOutput->outputPtr);
                    pSignSrv->pSignatureLength[0] = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel]);
                    pSignSrv->pSignature[1]       = HSE_PTR_TO_HOST_ADDR((&pPrimitiveInputOutput->outputPtr[Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel]]));
                    pSignSrv->pSignatureLength[1] = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel]);
                    /* Fill the context */
                    pSignSrv->signScheme.sch.eddsa.contextLength = (uint8)pPrimitiveInputOutput->secondaryInputLength;
                    pSignSrv->signScheme.sch.eddsa.pContext      = HSE_PTR_TO_HOST_ADDR(pPrimitiveInputOutput->secondaryInputPtr);
                }
                else
                {
                    pSignSrv->pSignature[0]       = HSE_PTR_TO_HOST_ADDR(pPrimitiveInputOutput->secondaryInputPtr);
                    pSignSrv->pSignature[1]       = HSE_PTR_TO_HOST_ADDR((&pPrimitiveInputOutput->secondaryInputPtr[Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel]]));
                    pSignSrv->pSignatureLength[0] = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel]);
                    pSignSrv->pSignatureLength[1] = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->au32OutLength[u8MuChannel]);
                    /* Fill the context */
                    pSignSrv->signScheme.sch.eddsa.contextLength = (uint8)pPrimitiveInputOutput->tertiaryInputLength;
                    pSignSrv->signScheme.sch.eddsa.pContext      = HSE_PTR_TO_HOST_ADDR(pPrimitiveInputOutput->tertiaryInputPtr);
                }
            }
            else
            {
                /* If the length is not the expected one */
                bError = TRUE;
            }
            break;
#endif /* (STD_ON == CRYPTO_SPT_ECC) */

        default:
            /* If this point is reached, do not send message to HSE */
            bError = TRUE;
            break;
    }

#if (STD_OFF == CRYPTO_SPT_RSA)
    /* Avoid compiler warning */
    (void)u32SignKeyElemIdx;
#endif /* (STD_ON == CRYPTO_SPT_RSA) */

    return bError;
}
#endif /* ((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC)) */
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

/**
* @brief           Function that checks the Job Operation Mode status
* @details         Check the operation mode state machine
*
* @param[in]       pJob: Pointer to job
*
* @returns         Std_ReturnType
*                   E_OK:     Correct configuration of parameters
*                   E_NOT_OK: Incorrect configuration
*/
static inline Std_ReturnType Crypto_Hse_OperationModeControl
(
    Crypto_JobType* pJob
)
{
    Std_ReturnType                 RetVal     = (Std_ReturnType)E_NOT_OK;
    const Crypto_OperationModeType eJobMode   = pJob->jobPrimitiveInputOutput.mode;
    const Crypto_JobStateType      eJobState  = pJob->jobState;

    switch (eJobMode)
    {
        case CRYPTO_OPERATIONMODE_STREAMSTART:
        /* fall-through */
        case CRYPTO_OPERATIONMODE_START:
        /* fall-through */
        case CRYPTO_OPERATIONMODE_SINGLECALL:
            /** [SWS_Crypto_00020] If Crypto_ProcessJob() is called while in Idle or Active state and
            with the operation mode START, the previous request shall be cancelled.
            That means, that all previously buffered data for this job shall be reset,
            and the job shall switch to Active state and process the new one.*/
            pJob->jobState = CRYPTO_JOBSTATE_ACTIVE;
            RetVal = E_OK;
            break;

        case CRYPTO_OPERATIONMODE_UPDATE:
            /**[SWS_Crypto_00118]  If Crypto_ProcessJob() is called while the job is in state Idle and
            the START flag in the operation mode is not set, the function shall return with E_NOT_OK.*/
            if (CRYPTO_JOBSTATE_ACTIVE == eJobState)
            {
                RetVal = E_OK;
            }
            break;
        case CRYPTO_OPERATIONMODE_FINISH:
            /**[SWS_Crypto_00023] If Crypto_ProcessJob() is called while in Active state and
            with the operation mode FINISH, the cryptographic calculations shall be finalized.*/
            if (CRYPTO_JOBSTATE_ACTIVE == eJobState)
            {
                RetVal = E_OK;
            }
            break;
        default:
            /*Do Nothing*/
            break;
    }
    return RetVal;
}

#if (STD_ON == CRYPTO_SPT_HASH)
/**
* @brief           Configure HSE algo
* @details         Set algo according to Autosar job
*
* @param[in]       u8AlgoFamily:    Crypto job algorithm
*
* @returns         Crypto_Hse_HashConfigType: Corresponding HSE algorithm and Hash Length
*/
static inline Crypto_Hse_HashConfigType Crypto_Hse_ConfigureHashAlgo
(
    uint8 u8AlgoFamily
)
{
    Crypto_Hse_HashConfigType RetVal;

    switch (u8AlgoFamily)
    {
#if (STD_ON == CRYPTO_SPT_SHA1)
        case (uint8)CRYPTO_ALGOFAM_SHA1:
            RetVal.HseHashAlgo   = HSE_HASH_ALGO_SHA_1;
            RetVal.u32HashLength = 20U;
            break;
#endif

#if (STD_ON == CRYPTO_SPT_SHA2_224)
        case (uint8)CRYPTO_ALGOFAM_SHA2_224:
            RetVal.HseHashAlgo   = HSE_HASH_ALGO_SHA2_224;
            RetVal.u32HashLength = 28U;
            break;
#endif

#if (STD_ON == CRYPTO_SPT_SHA2_256)
        case (uint8)CRYPTO_ALGOFAM_SHA2_256:
            RetVal.HseHashAlgo   = HSE_HASH_ALGO_SHA2_256;
            RetVal.u32HashLength = 32U;
            break;
#endif

#if (STD_ON == CRYPTO_SPT_SHA2_384)
        case (uint8)CRYPTO_ALGOFAM_SHA2_384:
            RetVal.HseHashAlgo   = HSE_HASH_ALGO_SHA2_384;
            RetVal.u32HashLength = 48U;
            break;
#endif

#if (STD_ON == CRYPTO_SPT_SHA2_512)
        case (uint8)CRYPTO_ALGOFAM_SHA2_512:
            RetVal.HseHashAlgo   = HSE_HASH_ALGO_SHA2_512;
            RetVal.u32HashLength = 64U;
            break;
#endif

#if (STD_ON == CRYPTO_SPT_SHA2_512_224)
        case (uint8)CRYPTO_ALGOFAM_SHA2_512_224:
            RetVal.HseHashAlgo   = HSE_HASH_ALGO_SHA2_512_224;
            RetVal.u32HashLength = 28U;
            break;
#endif

#if (STD_ON == CRYPTO_SPT_SHA2_512_256)
        case (uint8)CRYPTO_ALGOFAM_SHA2_512_256:
            RetVal.HseHashAlgo   = HSE_HASH_ALGO_SHA2_512_256;
            RetVal.u32HashLength = 32U;
            break;
#endif

#if (STD_ON == CRYPTO_SPT_SHA3)
        case (uint8)CRYPTO_ALGOFAM_SHA3_224:
            RetVal.HseHashAlgo   = HSE_HASH_ALGO_SHA3_224;
            RetVal.u32HashLength = 28U;
            break;

        case (uint8)CRYPTO_ALGOFAM_SHA3_256:
            RetVal.HseHashAlgo   = HSE_HASH_ALGO_SHA3_256;
            RetVal.u32HashLength = 32U;
            break;

        case (uint8)CRYPTO_ALGOFAM_SHA3_384:
            RetVal.HseHashAlgo   = HSE_HASH_ALGO_SHA3_384;
            RetVal.u32HashLength = 48U;
            break;

        case (uint8)CRYPTO_ALGOFAM_SHA3_512:
            RetVal.HseHashAlgo   = HSE_HASH_ALGO_SHA3_512;
            RetVal.u32HashLength = 64U;
            break;
#endif

        default:
            RetVal.HseHashAlgo   = HSE_HASH_ALGO_NULL;
            RetVal.u32HashLength = 0U;
            break;
    }
    return RetVal;
}

/**
* @brief           Fills the hash descriptor
* @details         Configures the hash descriptor
*
* @param[in]       u8MuInstance:         MU instance where the job will be issued
* @param[in]       u8MuChannel:          MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       pJob:                 Pointer to the job
* @param[in]       u8StreamId:           Identifier of the HSE stream used for the job
* @param[inout]    pbReqSentOverChannel: Pointer to a boolean storing channel usage
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: incorrect lengths
*                   CRYPTO_E_SMALL_BUFFER: provided buffer is to small
*/
static inline Std_ReturnType Crypto_Hse_Hash
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
)
{
    hseHashSrv_t*             pHashSrv      = &(pHseSrvDescriptor->hseSrv.hashReq);
    Std_ReturnType            RetVal        = (Std_ReturnType)E_NOT_OK;
#if (STD_ON == CRYPTO_SPT_HASH)
    Crypto_Hse_HashConfigType HashAlgo      = Crypto_Hse_ConfigureHashAlgo((uint8)pJob->jobPrimitiveInfo->primitiveInfo->algorithm.family);
#endif /* (STD_ON == CRYPTO_SPT_HASH) */
    uint32*                   pOutputLength = NULL_PTR;
    uint32                    u32ResultLen  = pJob->jobPrimitiveInfo->primitiveInfo->resultLength;
    const uint8*              pInput        = NULL_PTR;
    uint32                    inputLength   = 0U;
    const uint8*              pOutputPtr    = NULL_PTR;
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    uint32                    u32RedirectedInputKeyIdx;
    uint32                    u32RedirectedInputKeyElemIdx;
    uint32                    u32RedirectedOutputKeyIdx;
    uint32                    u32RedirectedOutputKeyElemIdx;
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */

#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    /* Check if input is redirected */
    if ((NULL_PTR != pJob->jobRedirectionInfoRef) &&
        (Crypto_Util_IsRedirectionSet(pJob->jobRedirectionInfoRef->redirectionConfig, CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT)))
    {
        /* Take the key index and key element index that was already checked in upper layer */
        u32RedirectedInputKeyIdx     = Crypto_Util_GetKeyIdx(pJob->jobRedirectionInfoRef->inputKeyId);
        u32RedirectedInputKeyElemIdx = Crypto_Util_GetKeyElementIdx (u32RedirectedInputKeyIdx, pJob->jobRedirectionInfoRef->inputKeyElementId);

        /* Check if the u32RedirectedInputKeyElemIdx is valid */
        if(CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32RedirectedInputKeyElemIdx)
        {
            pInput      = Crypto_aKeyElementList[u32RedirectedInputKeyElemIdx].pCryptoElementArray;
            inputLength = *Crypto_aKeyElementList[u32RedirectedInputKeyElemIdx].pu32CryptoElementActualSize;
        }
    }
    else
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))  */
    {
        pInput      = pJob->jobPrimitiveInputOutput.inputPtr;
        inputLength = pJob->jobPrimitiveInputOutput.inputLength;
    }
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    /* Check if output is redirected */
    if ((NULL_PTR != pJob->jobRedirectionInfoRef) &&
        (Crypto_Util_IsRedirectionSet(pJob->jobRedirectionInfoRef->redirectionConfig, CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT)))
    {
        /* Take the key index and key element index that was already checked in upper layer */
        u32RedirectedOutputKeyIdx     = Crypto_Util_GetKeyIdx(pJob->jobRedirectionInfoRef->outputKeyId);
        u32RedirectedOutputKeyElemIdx = Crypto_Util_GetKeyElementIdx (u32RedirectedOutputKeyIdx, pJob->jobRedirectionInfoRef->outputKeyElementId);

        /* Check if the u32RedirectedOutputKeyElemIdx is valid */
        if(CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32RedirectedOutputKeyElemIdx)
        {
            pOutputPtr    = Crypto_aKeyElementList[u32RedirectedOutputKeyElemIdx].pCryptoElementArray;
            pOutputLength = Crypto_aKeyElementList[u32RedirectedOutputKeyElemIdx].pu32CryptoElementActualSize;
        }
    }
    else
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))  */
    {
        pOutputPtr    = pJob->jobPrimitiveInputOutput.outputPtr;
        pOutputLength = pJob->jobPrimitiveInputOutput.outputLengthPtr;
    }

    /* [SWS_Crypto_00071] In case of START and UPDATE the output length should not be checked as the length is required only for FINISH and SINGLE CALL */
    if (((uint8)CRYPTO_OPERATIONMODE_FINISH != (uint8)(((uint8)(pJob->jobPrimitiveInputOutput.mode)) & CRYPTO_OPERATIONMODE_FINISH_MASK_U8)) || (*pOutputLength >= u32ResultLen))
    {
        pHseSrvDescriptor->srvId = HSE_SRV_ID_HASH;
        pHashSrv->accessMode     = Crypto_Hse_ConfigureAccessMode (pJob->jobPrimitiveInputOutput.mode);
#if (STD_ON == CRYPTO_SPT_HASH)
        pHashSrv->hashAlgo       = HashAlgo.HseHashAlgo;
#endif /* (STD_ON == CRYPTO_SPT_HASH) */
        pHashSrv->streamId       = u8StreamId;
        pHashSrv->sgtOption      = HSE_SGT_OPTION_NONE;
        /* START: inputPtr, inputLength, outputPtr and outputLengthPtr are not required, if inputLength is 0 the firmware will not process the intput.
                  If inputLength is not 0 the input will be processed.
           UPDATE: inputPtr, inputLength are required
           FINISH: inputPtr, inputLength are optional, if inputLength is not 0 the input will be processed */
        pHashSrv->pInput      = HSE_PTR_TO_HOST_ADDR(pInput);
        pHashSrv->inputLength = inputLength; /* Must be a multiple of block length. */
        if ((uint8)CRYPTO_OPERATIONMODE_FINISH == (((uint8)(pJob->jobPrimitiveInputOutput.mode)) & CRYPTO_OPERATIONMODE_FINISH_MASK_U8))
        {
                pHashSrv->pHash       = HSE_PTR_TO_HOST_ADDR(pOutputPtr);
                /* [SWS_Crypto_00135] Store in outputLengthPtr or redirected output length the actual length of the returned value.
                   If the buffer is smaller than the size of the hash, the hash will be truncated (not applicable for Miyaguchi Preneel).*/
                pHashSrv->pHashLength = HSE_PTR_TO_HOST_ADDR(pOutputLength);
                /* [SWS_Crypto_00065] If job->jobPrimitiveInfo->primitiveInfo->service is set to CRYPTO_HASH or CRYPTO_MACGENERATE, the parameter job->jobPrimitiveInfo->primitiveInfo->resultLength is required.*/
                /* Load the value of resultLength in pOutputLength, which is send to the firmware, because the user may want the result truncated to a smaller length */
                *pOutputLength = u32ResultLen;
        }

        RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
        /* Mark the request as sent */
        *pbReqSentOverChannel = TRUE;
    }
    else
    {
        /* [SWS_Crypto_00136] Output buffer is not big enough to store the result */
        RetVal = (Std_ReturnType)CRYPTO_E_SMALL_BUFFER;
    }

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_SPT_HASH) */


#if ((STD_ON == CRYPTO_SPT_SIPHASH) && (STD_ON == CRYPTO_KEYS_EXIST))
/**
* @brief           Fills the SipHash descriptor
* @details         Configures the hash descriptor
*
* @param[in]       u8MuInstance:         MU instance where the job will be issued
* @param[in]       u8MuChannel:          MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       pJob:                 Pointer to the job
* @param[in]       HseAuthDir:           HSE_AUTH_DIR_GENERATE or HSE_AUTH_DIR_VERIFY
* @param[inout]    pbReqSentOverChannel: Pointer to a boolean storing channel usage
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: incorrect lengths
*                   CRYPTO_E_SMALL_BUFFER: provided buffer is to small
*/
static inline Std_ReturnType Crypto_Hse_SipHash
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    hseAuthDir_t          HseAuthDir,
    boolean*              pbReqSentOverChannel
)
{
    hseSipHashSrv_t*                          pSipHashSrv;
    Std_ReturnType                            RetVal                = (Std_ReturnType)E_NOT_OK;
    uint32                                    u32CryptoKeyIdx       = Crypto_Util_GetKeyIdx(Crypto_Util_GetJobKey(pJob));
    uint32                                    u32SipHashKeyElemIdx  = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KEY_MATERIAL_U32);
    const Crypto_JobPrimitiveInputOutputType* pPrimitiveInputOutput = &pJob->jobPrimitiveInputOutput;
    const Crypto_PrimitiveInfoType*           pPrimitiveInfo        = pJob->jobPrimitiveInfo->primitiveInfo;

    if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32SipHashKeyElemIdx)
    {
        pSipHashSrv = &(pHseSrvDescriptor->hseSrv.sipHashReq);

        pHseSrvDescriptor->srvId    = HSE_SRV_ID_SIPHASH;
        pSipHashSrv->authDir        = HseAuthDir;
        pSipHashSrv->keyHandle      = (hseKeyHandle_t)Crypto_aKeyElementList[u32SipHashKeyElemIdx].u32HseKeyHandle;
        /* Load the input message */
        pSipHashSrv->inputLength    = (uint16)pPrimitiveInputOutput->inputLength;
        pSipHashSrv->pInput         = HSE_PTR_TO_HOST_ADDR(pPrimitiveInputOutput->inputPtr);

        if (CRYPTO_ALGOMODE_SIPHASH_2_4 == pPrimitiveInfo->algorithm.mode)
        {
            /* Please use a 128-bit key for CRYPTO_ALGOMODE_SIPHASH_2_4 variant */
            pSipHashSrv->sipHashVariant = HSE_SIPHASH_VARIANT_64;
        }
        else
        {
            /* Please use a 128-bit key for CRYPTO_ALGOMODE_SIPHASH_2_4_128 variant */
            pSipHashSrv->sipHashVariant = HSE_SIPHASH_VARIANT_128;
        }

        /* Choose the direction of the SipHash */
        if (HSE_AUTH_DIR_GENERATE == HseAuthDir)
        {
            pSipHashSrv->pTag      = HSE_PTR_TO_HOST_ADDR(pPrimitiveInputOutput->outputPtr);

            /* The output buffer length should be equal or lager than result length */
            if (*pJob->jobPrimitiveInputOutput.outputLengthPtr >= pJob->jobPrimitiveInfo->primitiveInfo->resultLength)
            {
                /* [SWS_Crypto_00065] The output length requested for the mac is stored in resultLength */
                pSipHashSrv->tagLength = (uint8)pJob->jobPrimitiveInfo->primitiveInfo->resultLength;

                RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
                /* Mark the request as sent */
                *pbReqSentOverChannel = TRUE;
            }
            else
            {
                /* [SWS_Crypto_00136] Output buffer is not big enough to store the result */
                RetVal = CRYPTO_E_SMALL_BUFFER;
            }
        }
        else
        {
            pSipHashSrv->pTag      = HSE_PTR_TO_HOST_ADDR(pPrimitiveInputOutput->secondaryInputPtr);
            pSipHashSrv->tagLength = (uint8)pPrimitiveInputOutput->secondaryInputLength;
            RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
            /* Mark the request as sent */
           *pbReqSentOverChannel = TRUE;
        }
    }
    /* Fill verifyPtr only for synchronous processing type. For asynchronous processing type the verifyPtr will be filled by Crypto_Hse_ProcessMuChannelResponse() function.  The service is only accessible in one-pass. */
    if (
          (HSE_AUTH_DIR_VERIFY == HseAuthDir) && \
          (CRYPTO_PROCESSING_SYNC == pJob->jobPrimitiveInfo->processingType)
        )
    {
        /* Fill the VerifyPtr */
        Crypto_Hse_FillVerifyPtr(pJob, RetVal);
    }
    /* Fill the output length pointer with the result length when SipHash generation is finished successfully */
    else if (
                (HSE_AUTH_DIR_GENERATE == HseAuthDir) && \
                ((Std_ReturnType)E_OK == RetVal)
              )
    {
        /* For 64 bits SipHash, if value of outputLengthPtr is larger than 8 (bytes), store in outputLengthPtr the standard size */
        if (CRYPTO_ALGOMODE_SIPHASH_2_4 == pPrimitiveInfo->algorithm.mode)
        {
            if (pJob->jobPrimitiveInfo->primitiveInfo->resultLength > 8U)
            {
                *pPrimitiveInputOutput->outputLengthPtr = 8U;
            }
            else
            {
                /* [SWS_Crypto_00065] If the value of resultLength is lower or equal to 8 (bytes), store in outputLengthPtr the resultLength size */
                *pPrimitiveInputOutput->outputLengthPtr = pJob->jobPrimitiveInfo->primitiveInfo->resultLength;
            }
        }

        /* For 128 bits SipHash, if value of outputLengthPtr is larger than 16 (bytes), store in outputLengthPtr the standard size */
        if (CRYPTO_ALGOMODE_SIPHASH_2_4_128 == (uint8)pPrimitiveInfo->algorithm.mode)
        {
            if (pJob->jobPrimitiveInfo->primitiveInfo->resultLength > 16U)
            {
                *pPrimitiveInputOutput->outputLengthPtr = 16U;
            }
            else
            {
                /* [SWS_Crypto_00065] If the value of resultLength is lower or equal to 16 (bytes), store in outputLengthPtr the resultLength size */
                *pPrimitiveInputOutput->outputLengthPtr = pJob->jobPrimitiveInfo->primitiveInfo->resultLength;
            }
        }
    }
    else
    {
        /* Do nothing. Added else branch to avoid MISRA violation. */
    }
    return RetVal;
}
#endif /* ((STD_ON == CRYPTO_SPT_SIPHASH) && (STD_ON == CRYPTO_KEYS_EXIST)) */

/**
* @brief           Fills the random descriptor
* @details         Configures the random descriptor
*
* @param[in]       u8MuInstance:        MU instance where the job will be issued
* @param[in]       u8MuChannel:         MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:   Descriptor to be filled and send to HSE
* @param[in]       pJob:                Pointer to the job
*
* @returns         Std_ReturnType
*                   E_OK: correct configuration of parameters
*                   E_NOT_OK: incorrect configuration of parameters: incorrect lengths
*/
static inline Std_ReturnType Crypto_Hse_Random
(
    uint8                 u8MuInstance,
    uint8                 u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob
)
{
    Std_ReturnType        RetVal;
    hseGetRandomNumSrv_t* pRndCmd    = &(pHseSrvDescriptor->hseSrv.getRandomNumReq);
    hseRngClass_t         u8RngClass = 0xFFU;
    const uint8           u8AlgoMode = (uint8)pJob->jobPrimitiveInfo->primitiveInfo->algorithm.mode;

    pHseSrvDescriptor->srvId                = HSE_SRV_ID_GET_RANDOM_NUM;
    pRndCmd->pRandomNum      = HSE_PTR_TO_HOST_ADDR(pJob->jobPrimitiveInputOutput.outputPtr);
    /* Length of the random number should not be more than 2048 bytes */
    pRndCmd->randomNumLength =           *(pJob->jobPrimitiveInputOutput.outputLengthPtr);
    /* Deterministic Random Generator class selection */
    switch (u8AlgoMode)
    {
        case (uint8)CRYPTO_ALGOMODE_CTRDRBG:
            u8RngClass = HSE_RNG_CLASS_DRG3;
            break;

        case CRYPTO_ALGOMODE_CUSTOM_DRBG_PR:
            u8RngClass = HSE_RNG_CLASS_DRG4;
            break;

        case CRYPTO_ALGOMODE_CUSTOM_DRBG_RS:
            u8RngClass = HSE_RNG_CLASS_PTG3;
            break;

        default:
            /* Do nothing, this point must not be reached */
            break;
    }
    pRndCmd->rngClass = u8RngClass;

    RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);

    return RetVal;
}

/**
* @brief           Verifies the response received from HSE
* @details         Verifies the response received from HSE and sets the corresponding response for CRYPTO driver
*
* @param[in]       u32Message: Response received from HSE
*
* @returns         Std_ReturnType
*                   E_OK:                       Request successful
*                   E_NOT_OK:                   Request unsuccessful
*                   CRYPTO_E_SMALL_BUFFER:      Provided buffer is to small
*                   CRYPTO_E_KEY_READ_FAIL:     Failure in reading the key
*                   CRYPTO_E_KEY_WRITE_FAIL:    Failure in writing the key
*                   CRYPTO_E_KEY_NOT_AVAILABLE: Key not available
*                   CRYPTO_E_KEY_NOT_VALID:     Invalid key
*                   CRYPTO_E_BUSY:              Busy
*/
static inline Std_ReturnType Crypto_Hse_TranslateHseResponse
(
    hseSrvResponse_t HseResponse
)
{
    Std_ReturnType RetVal;

    switch (HseResponse)
    {
        /* HSE command successfully executed with no error. */
        case HSE_SRV_RSP_OK:
            RetVal = (Std_ReturnType)E_OK;
            break;

        case HSE_SRV_RSP_NOT_ENOUGH_SPACE:          /* There is no enough space to perform operation (e.g. load a key) */
        /* fall-through */
        case HSE_SRV_RSP_STREAMING_MODE_FAILURE:    /* The service request that uses streaming mode failed (e.g. UPDATES and FINISH steps do not use the same HSE interface ID and channel ID as START step) */
        /* fall-through */
        case HSE_SRV_RSP_VERIFY_FAILED:             /* HSE signals that a verification request fails (e.g. MAC and Signature verification)  */
        /* fall-through */
        case HSE_SRV_RSP_MEMORY_FAILURE:            /* Detect physical errors, flipped bits etc., during memory read or write operations. */
        /* fall-through */
        case HSE_SRV_RSP_GENERAL_ERROR:             /* This error code is returned if an error not covered by the error codes above is detected inside HSE. */
            RetVal = (Std_ReturnType)E_NOT_OK;
            break;

        /* The service request failed because the specified key slot is empty. */
        case HSE_SRV_RSP_KEY_EMPTY:
            RetVal = (Std_ReturnType)CRYPTO_E_KEY_EMPTY;
            break;

        /* The service request failed because read access was denied */
        case HSE_SRV_RSP_READ_FAILURE:
#if (STD_ON == CRYPTO_KEYS_EXIST)
            RetVal = (Std_ReturnType)CRYPTO_E_KEY_READ_FAIL;
#else
            RetVal = (Std_ReturnType)E_NOT_OK;
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */
            break;

        /* The service request failed because read access was denied */
        case HSE_SRV_RSP_WRITE_FAILURE:
            RetVal = (Std_ReturnType)CRYPTO_E_KEY_WRITE_FAIL;
            break;

        /* This error code is returned if a key is locked due to failed boot measurement or an active debugger. */
        case HSE_SRV_RSP_KEY_NOT_AVAILABLE:
            RetVal = (Std_ReturnType)CRYPTO_E_KEY_NOT_AVAILABLE;
            break;

        /* Specified key slot is either not valid or not available due to a key usage flags restrictions. */
        case HSE_SRV_RSP_KEY_INVALID:
            RetVal = (Std_ReturnType)CRYPTO_E_KEY_NOT_VALID;
            break;

        /* Specified size of tag is not valid or not supported by HSE */
        case HSE_SRV_RSP_INVALID_PARAM: /* The HSE request parameters are invalid (e.g misaligned, invalid range) */
            RetVal = (Std_ReturnType)CRYPTO_RET_INVALID_PARAM;
            break;

        /* Extension to HSE returned values for signaling that no response was received from HSE in the timeout window */
        case HSE_IP_SRV_RSP_NO_RESPONSE:
            RetVal = (Std_ReturnType)CRYPTO_RET_OPERATION_TIMEOUT;
            break;

        default:
            /* Should not get here */
            RetVal = (Std_ReturnType)E_NOT_OK;
            break;
    }

    return RetVal;
}

/**
* @brief           Function to process the Hse response received on a channel
* @details         Function to process the Hse response received on a channel
*
* @param[in]       u8MuInstance:   MU instance where the Hse response was received
* @param[in]       u8MuChannel:    MU channel where the Hse response was received
* @param[in]       HseResponse:    HSE service response
* @param[in]       pCallbackParam: Parameter used to call the asynchronous callback (can be NULL)
*
* @returns         void
*/
static void Crypto_Hse_ProcessMuChannelResponse
(
    uint8            u8MuInstance,
    uint8            u8MuChannel,
    hseSrvResponse_t HseResponse,
    void*            pCallbackParam
)
{
    /* Crypto_Hse layer always adds the pointer to the Crypto job as callback parameter to Hse_Ip_ServiceRequest() */
    Crypto_JobType* const pCryptoJob = (Crypto_JobType*)pCallbackParam;
    Std_ReturnType        CryptoErrorCode;

    /* This Hse request is complete. If there are jobs in the queue of the CDO running on the current partition, send them to HSE for processing */
    Crypto_Hse_PushJobsFromQueuesToHse(u8MuInstance);
    /* As the asynchronous job is complete, mark the channel as having an invalid JobId value */
    Crypto_Hse_apMuState[u8MuInstance]->au32MuChannelJobId[u8MuChannel] = CRYPTO_INVALID_VAL_FOR_JOBID_U32;
    /* Translate the message received from HSE to Crypto error code */
    CryptoErrorCode = Crypto_Hse_TranslateHseResponse(HseResponse);
    if (CRYPTO_RET_INVALID_PARAM == CryptoErrorCode)
    {
        CryptoErrorCode = (Std_ReturnType)E_NOT_OK;
    }
    /* Fill the verifyPtr in case the primitive was MacGenerate or AEAD Decrypt */
    Crypto_Hse_FillVerifyPtr(pCryptoJob, CryptoErrorCode);

    /* Check if the job is in FINISH operation mode, single call operation mode or if an error occured */
    if (((uint8)CRYPTO_OPERATIONMODE_FINISH == (uint8)(((uint8)(pCryptoJob->jobPrimitiveInputOutput.mode)) & CRYPTO_OPERATIONMODE_FINISH_MASK_U8))||
        ((Std_ReturnType)E_OK != CryptoErrorCode)
       )
    {
        /* [SWS_Crypto_00018][SWS_Crypto_00119] Clear the job state for FINISH operation mode or if an error occured in START,UPDATE or FINISH operation mode.
           The state will be cleared for single call too as it contains the FINISH step. */
        pCryptoJob->jobState = CRYPTO_JOBSTATE_IDLE;
        /* Free stream for FINISH operation mode or if an error occured during streaming */
        if (CRYPTO_OPERATIONMODE_SINGLECALL!= pCryptoJob->jobPrimitiveInputOutput.mode)
        {
            Crypto_Hse_FreeStream(u8MuInstance, pCryptoJob->jobId);
        }
    }

    /* Call the callback */
    CryIf_CallbackNotification(pCryptoJob, CryptoErrorCode);
}

/**
* @brief           Processes a job
* @details         Tries to initiate a request to HSE to process a job. If no MU channels are available returns CRYPTO_E_BUSY
*
* @param[in]       u8MuInstance: MU instance used to send the request to HSE
* @param[in]       u32ObjectIdx: Index of the Crypto Driver Object where the job will be processed
* @param[in]       pJob:         Pointer to the job to be processed
*
* @returns         Std_ReturnType
*                   E_OK:          Request successful
*                   E_NOT_OK:      Request unsuccessful
*                   CRYPTO_E_BUSY: No MU channels available to process the requested job operation
*/
static Std_ReturnType Crypto_Hse_ProcessOperation
(
    const uint8     u8MuInstance,
    uint32          u32ObjectIdx,
    Crypto_JobType* pJob
)
{
    uint8               u8MuChannel;
    Std_ReturnType      RetVal     = (Std_ReturnType)E_NOT_OK;
    uint8               u8StreamId = CRYPTO_INVALID_STREAM_ID_U8;
    hseSrvDescriptor_t* pHseSrvDescriptor;
    boolean             bReqSentOverChannel;

    /* Check if a free MU channel can be found */
    u8MuChannel = Hse_Ip_GetFreeChannel(u8MuInstance);
    /* If a free channel is found */
    if (HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        /* Channel successfully obtain, keep the status of the request and mark it as waiting to be send */
        bReqSentOverChannel = FALSE;
        pHseSrvDescriptor = &(Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor[u8MuChannel]);
        /* Check operation mode status  */
        RetVal = Crypto_Hse_OperationModeControl(pJob);

        /* If the job has stream mode operation check if any stream slots is available */
        if ((CRYPTO_OPERATIONMODE_SINGLECALL != pJob->jobPrimitiveInputOutput.mode))
        {
            /* Grab an available stream */
            u8StreamId = Crypto_Hse_AllocStream(u8MuInstance, pJob);
            if (CRYPTO_INVALID_STREAM_ID_U8 == u8StreamId)
            {
                RetVal = (Std_ReturnType)CRYPTO_RET_STREAM_BUSY;
            }
        }

        if ((Std_ReturnType)E_OK == RetVal)
        {
            /* Store the index of the CDO where the job came from */
            Crypto_Hse_apMuState[u8MuInstance]->au32MuChannelCDOIdx[u8MuChannel] = u32ObjectIdx;

            /* Service the request */
            RetVal = Crypto_Hse_ServiceRequest(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, u8StreamId, &bReqSentOverChannel);

            if ((CRYPTO_PROCESSING_SYNC      == pJob->jobPrimitiveInfo->processingType) &&
                ((CRYPTO_OPERATIONMODE_FINISH == pJob->jobPrimitiveInputOutput.mode)     ||
                ((Std_ReturnType)E_OK != RetVal))
                )
            {
                /* Clear stream status for SYNC requests */
                Crypto_Hse_FreeStream(u8MuInstance, pJob->jobId);
            }
        }
        /* Free the allocated channel in case the request was not sent to firmware */
        if (FALSE == bReqSentOverChannel)
        {
            Hse_Ip_ReleaseChannel(u8MuInstance, u8MuChannel);
        }
    }
    else
    {
        /* No free MU channel was found */
        RetVal = (Std_ReturnType)CRYPTO_E_BUSY;
    }
    return RetVal;
}


/**
* @brief           Sends to HSE the request to cancel a service request in progress
* @details         Sends to HSE the request to cancel a service request in progress
*
* @param[in]       u8MuInstance:         MU instance used to send the request to HSE
* @param[in]       u8MuChannel:          MU channel used to send the request to HSE
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       u8MuChannelToCancel:  MU channel for which the request should be cancelled
*
* @returns         Std_ReturnType
*/
static inline Std_ReturnType Crypto_Hse_SendCancelJobReq
(
    uint8               u8MuInstance,
    uint8               u8MuChannel,
    hseSrvDescriptor_t* pHseSrvDescriptor,
    uint8               u8MuChannelToCancel
)
{
    /* Fill the request message */
    pHseSrvDescriptor->srvId = HSE_SRV_ID_CANCEL;
    pHseSrvDescriptor->hseSrv.cancelSrvReq.muChannelIdx = u8MuChannelToCancel;

    /* Send the cancel request to HSE */
    return Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR);
}

#if (STD_ON == CRYPTO_KEYS_EXIST)
#if ((STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT))
/**
* @brief           Function to obtain key information.
* @details         Requests HSE to return hseKeyInfo_t of the key.
*
* @param[in]       u8MuInstance:    MU instance where the key info will be requested
* @param[in]       u32HseKeyHandle: Key handle of the key.
*
* @returns         Std_ReturnType
*/
static inline Std_ReturnType Crypto_Hse_GetKeyInfo
(
    const uint8 u8MuInstance,
    uint32      u32HseKeyHandle
)
{
#if(STD_ON == CRYPTO_SPT_GETKEYINFO)
    hseSrvDescriptor_t* pHseSrvDescriptor;
    uint8               u8MuChannel;
    Std_ReturnType      RetVal = (Std_ReturnType)E_NOT_OK;

    u8MuChannel = Hse_Ip_GetFreeChannel(u8MuInstance);
    if (HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        pHseSrvDescriptor = &(Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor[u8MuChannel]);
        /* Fill the HSE descriptor with a request to get key info */
        pHseSrvDescriptor->srvId                          = HSE_SRV_ID_GET_KEY_INFO;
        pHseSrvDescriptor->hseSrv.getKeyInfoReq.keyHandle = u32HseKeyHandle;
        pHseSrvDescriptor->hseSrv.getKeyInfoReq.pKeyInfo  = HSE_PTR_TO_HOST_ADDR(&Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage);
        /* Send the request to HSE, synchronously */
        RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR);
    }
#else
    Std_ReturnType RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;
    /* Ignore compiler warnings */
    (void)u8MuInstance;
    (void)u32HseKeyHandle;
#endif /* (STD_ON == CRYPTO_SPT_GETKEYINFO) */
    return RetVal;
}
#endif /* ((STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT)) */


#if (STD_ON == CRYPTO_SPT_KEY_GENERATE)
/**
* @brief           Function to fill and send to HSE request to generate a key
* @details         Function to fill and send to HSE request to generate a key
*
* @param[in]       u8MuInstance:         MU instance where the Hse request will be sent
* @param[in]       u8MuChannel:          MU channel where the Hse request will be sent
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       u32CryptoKeyIdx:      Index of the Crypto key
* @param[in]       pJob:                 Pointer to the Crypto job
* @param[inout]    pbReqSentOverChannel: Pointer to a boolean storing channel usage
*
* @returns         Std_ReturnType
*/
static Std_ReturnType Crypto_Hse_KeyGenerateRequest
(
    const uint8           u8MuInstance,
    const uint8           u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    const uint32          u32CryptoKeyIdx,
    boolean*              pbReqSentOverChannel
)
{
    Std_ReturnType       RetVal                    = (Std_ReturnType)E_NOT_OK;
    hseKeyGenerateSrv_t* pKeyGenSrv;
          uint32         u32Intermediate;
    const uint32         u32KeyMaterialKeyElemIdx  = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KEY_MATERIAL_U32);
          uint8          u8AlgoFam                 = (uint8)CRYPTO_ALGOFAM_NOT_SET;
          uint32         u32KeyLength;
          hseKeyType_t   KeyType;
          uint32         u32KeyHandle;
          uint16         u16KeyFlags;
    const uint32         u32AlgoFamKeyElemIdx      = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYGENERATE_ALGORITHM);

#if (STD_ON == CRYPTO_SPT_RSA_KEY_PAIR_GEN)
    const uint8*         pBaseExchange;
          uint32         u32BaseExchangeLength;
    const uint32         u32BaseExchangeKeyElemIdx = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYEXCHANGE_BASE);
#endif /* (STD_ON == CRYPTO_SPT_RSA_KEY_PAIR_GEN) */
#if (STD_ON == CRYPTO_SPT_ECC_KEY_PAIR_GEN)
    const uint32         u32CurveIdKeyElemIdx      = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYGENERATE_CURVETYPE);
    uint8                u8CurveId                 = HSE_EC_CURVE_NONE;
#endif /* (STD_ON == CRYPTO_SPT_ECC_KEY_PAIR_GEN) */

    /* Check if the u32KeyMaterialKeyElemIdx is valid */
    if(CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32KeyMaterialKeyElemIdx)
    {
        u32KeyLength                  = Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32CryptoKeyElementMaxSize;
        KeyType                       = Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u8HseKeyType;
        u32KeyHandle                  = Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u32HseKeyHandle;
        u16KeyFlags                   = Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u16HseKeyFlags;

        pHseSrvDescriptor->srvId      = HSE_SRV_ID_KEY_GENERATE;
        u32Intermediate               = Crypto_GetBitLenFromByteLen(u32KeyLength);
        pKeyGenSrv                    = &(pHseSrvDescriptor->hseSrv.keyGenReq);

        pKeyGenSrv->targetKeyHandle   = (hseKeyHandle_t)u32KeyHandle;
        pKeyGenSrv->keyInfo.keyType   = KeyType;
        pKeyGenSrv->keyInfo.keyBitLen = (uint16)u32Intermediate;
        pKeyGenSrv->keyInfo.keyFlags  = (hseKeyFlags_t)u16KeyFlags;
        /* SMR not used thus the flags are set to zero */
        pKeyGenSrv->keyInfo.smrFlags    = (hseSmrFlags_t)0x00;
        /* For RAM keys, the key counter is forced to zero (not used) and for NVM it will be set to zero */
        pKeyGenSrv->keyInfo.keyCounter  = (uint32)0x00;

        if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32AlgoFamKeyElemIdx)
        {
            u8AlgoFam = *Crypto_aKeyElementList[u32AlgoFamKeyElemIdx].pCryptoElementArray;
        }

        switch (u8AlgoFam)
        {
#if (STD_ON == CRYPTO_SPT_SYM_RND_KEY_GEN)
            case (uint8)CRYPTO_ALGOFAM_AES:
                pKeyGenSrv->keyGenScheme = (hseKeyGenScheme_t)HSE_KEY_GEN_SYM_RANDOM_KEY;
                /* Mark the request as being valid. Use temporary the RetVal variable */
                RetVal                   = (Std_ReturnType)E_OK;
                break;
#endif /* (STD_ON == CRYPTO_SPT_SYM_RND_KEY_GEN) */

#if (STD_ON == CRYPTO_SPT_RSA_KEY_PAIR_GEN)
            case (uint8)CRYPTO_ALGOFAM_RSA:
                /* Check if Base Exchange key element exists in target key */
                if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32BaseExchangeKeyElemIdx)
                {
                    pBaseExchange         = Crypto_aKeyElementList[u32BaseExchangeKeyElemIdx].pCryptoElementArray;
                    u32BaseExchangeLength = *Crypto_aKeyElementList[u32BaseExchangeKeyElemIdx].pu32CryptoElementActualSize;
                    /* - For RSA, keyBitLen specifies the bit length of the public modulus which shall be generated. */
                    pKeyGenSrv->keyInfo.keyBitLen                = (uint16) Crypto_GetBitLenFromByteLen(RSA_MOD_LEN);
                    pKeyGenSrv->keyInfo.specific.pubExponentSize = (uint8)u32BaseExchangeLength;
                    pKeyGenSrv->keyGenScheme                     = (hseKeyGenScheme_t) HSE_KEY_GEN_RSA_KEY_PAIR;
                    pKeyGenSrv->sch.rsaKey.pubExpLength          = u32BaseExchangeLength;
                    pKeyGenSrv->sch.rsaKey.pPubExp               = HSE_PTR_TO_HOST_ADDR(pBaseExchange);
                    /*The  public modulus n. It can be NULL (the modulus is not provided using this service).*/
                    pKeyGenSrv->sch.rsaKey.pModulus              = (HOST_ADDR)0U;
                    /* Mark the request as being valid. Use temporary the RetVal variable */
                    RetVal                                       = (Std_ReturnType)E_OK;
                }
                break;
#endif /* (STD_ON == CRYPTO_SPT_RSA_KEY_PAIR_GEN) */

#if (STD_ON == CRYPTO_SPT_ECC_KEY_PAIR_GEN)
            case (uint8)CRYPTO_ALGOFAM_ECCNIST:
            /* fall-through */
            case (uint8)CRYPTO_ALGOFAM_BRAINPOOL:
            /* fall-through */
            case (uint8)CRYPTO_ALGOFAM_ED25519:
            /* fall-through */
            case (uint8)CRYPTO_ALGOFAM_ED448:
                /* Check if curve id data have been imported previously in the target key */
                if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CurveIdKeyElemIdx)
                {
                    /* Load the coresponding curve id based on user key element data */
                    u8CurveId = Crypto_Hse_GetEccCurveId(u32CurveIdKeyElemIdx, u8AlgoFam);

                    if (HSE_EC_CURVE_NONE != u8CurveId)
                    {
                        pKeyGenSrv->keyInfo.keyBitLen           = Crypto_Hse_EccKeyBitLen(u8CurveId); /* For ECC, the bit length of the base point order. */
                        pKeyGenSrv->keyInfo.specific.eccCurveId = (hseEccCurveId_t)u8CurveId;
                        pKeyGenSrv->keyGenScheme                = (hseKeyGenScheme_t)HSE_KEY_GEN_ECC_KEY_PAIR;
                        pKeyGenSrv->sch.eccKey.pPubKey          = (HOST_ADDR)0U;
                        /* Mark the request as being valid. Use temporary the RetVal variable */
                        RetVal                                  = (Std_ReturnType)E_OK;
                    }
                }
                break;
#endif /* (STD_ON == CRYPTO_SPT_ECC_KEY_PAIR_GEN) */

            default:
                 /* Do nothing. RetVal is already set to E_NOT_OK which is the value that will be returned by the function */
                 break;
        }

        if ((Std_ReturnType)E_OK == RetVal)
        {
            RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
            /* Mark the request as sent */
            *pbReqSentOverChannel = TRUE;
        }
    }

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_SPT_KEY_GENERATE) */

#if (STD_ON == CRYPTO_SPT_KEY_DERIVE)
/**
* @brief         Fill and send service data for key derivation
* @details       Based on user configured algorithm this function will send key derivation command to HSE
*
* @param[in]       u8MuInstance:         MU instance where the Hse request will be sent
* @param[in]       u8MuChannel:          MU channel where the Hse request will be sent
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       u32CryptoKeyIdx:      Index of the Crypto key
* @param[in]       u32TargetKeyIdx:      Index of the Target key
* @param[in]       pJob:                 Pointer to the Crypto job
* @param[inout]    pbReqSentOverChannel: Pointer to a boolean storing channel usage
*
* @returns         Std_ReturnType
*                   E_OK:     Correct configuration of parameters
*                   E_NOT_OK: Incorrect configuration of parameters: incorrect lengths
*/
static Std_ReturnType Crypto_Hse_KeyDeriveRequest
(
    const uint8           u8MuInstance,
    const uint8           u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    const uint32          u32CryptoKeyIdx,
    const uint32          u32TargetKeyIdx,
    boolean*              pbReqSentOverChannel
)
{
    Std_ReturnType                  RetVal                        = (Std_ReturnType)E_NOT_OK;
    hseKeyDeriveSrv_t*              pDeriveKeySrv;
    const uint32                    u32PasswordKeyElemIdx         = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYDERIVATION_PASSWORD);
    const uint32                    u32DerivAlgoKeyElemIdx        = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYDERIVATION_ALGORITHM);
    const uint32                    u32DerivSaltKeyElemIdx        = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYDERIVATION_SALT);
    const uint32                    u32TargetKeyMaterialElemIdx   = Crypto_Util_GetKeyElementIdx(u32TargetKeyIdx, CRYPTO_KEY_MATERIAL_U32);
    const uint32                    u32DerivSecondAlgoKeyElemIdx  = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYDERIVATION_ALGORITHM_SECONDARY);
#if (STD_ON == CRYPTO_SPT_PBKDF2)
    const uint32                    u32DerivIterationKeyElemIdx   = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYDERIVATION_ITERATIONS);
#endif /* (STD_ON == CRYPTO_SPT_PBKDF2) */
#if (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT)
    const uint32                    u32TargetMasterKeyElemIdx     = Crypto_Util_GetKeyElementIdx(u32TargetKeyIdx, CRYPTO_KE_KEYDERIVATION_KEY_MASTER);
    const uint32                    u32TargetOutputSizeKeyElemIdx = Crypto_Util_GetKeyElementIdx(u32TargetKeyIdx, CRYPTO_KE_KEYDERIVATION_OUTPUT_SIZE);
    const uint32                    u32TargetOutputDataKeyElemIdx = Crypto_Util_GetKeyElementIdx(u32TargetKeyIdx, CRYPTO_KE_KEYDERIVATION_OUTPUT_DATA);
    const uint32                    u32TargetMasterKeyHandle      = Crypto_aKeyElementList[u32TargetMasterKeyElemIdx].u32HseKeyHandle;
#endif /* (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT) */
          uint32                    u32KeyHandle;
          uint32                    u32TargetKeyHandle;
          uint8                     u8KeyDeriveAlgo;
#if (STD_ON == CRYPTO_SPT_HASH)
          Crypto_Hse_HashConfigType HashAlgo;
#endif /* (STD_ON == CRYPTO_SPT_HASH) */

    /* Check that all indexes returned by Crypto_Util_GetKeyElementIdx are valid. A logic OR between all indexes will be equal to CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 when at least 
       one of the indexes is invalid */
#if (STD_ON == CRYPTO_SPT_PBKDF2)
    if(CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != (u32PasswordKeyElemIdx | u32DerivAlgoKeyElemIdx | u32TargetKeyMaterialElemIdx | u32DerivSecondAlgoKeyElemIdx | u32DerivIterationKeyElemIdx))
#else
    if(CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != (u32PasswordKeyElemIdx | u32DerivAlgoKeyElemIdx | u32TargetKeyMaterialElemIdx | u32DerivSecondAlgoKeyElemIdx))
#endif
    {
        u32KeyHandle       = Crypto_aKeyElementList[u32PasswordKeyElemIdx].u32HseKeyHandle;
        u32TargetKeyHandle = Crypto_aKeyElementList[u32TargetKeyMaterialElemIdx].u32HseKeyHandle;
        u8KeyDeriveAlgo    = Crypto_aKeyElementList[u32DerivAlgoKeyElemIdx].pCryptoElementArray[CRYPTO_KEY_DERIVE_ALGORITHM_IDX];
#if (STD_ON == CRYPTO_SPT_HASH)
        HashAlgo           = Crypto_Hse_ConfigureHashAlgo(Crypto_aKeyElementList[u32DerivSecondAlgoKeyElemIdx].pCryptoElementArray[CRYPTO_ALGORITHM_SECONDARY_IDX]);
#endif /* (STD_ON == CRYPTO_SPT_HASH) */

        /* Set Hse service type to Derive */
        pHseSrvDescriptor->srvId = HSE_SRV_ID_KEY_DERIVE;
        /* Keep a pointer to the derive structure, to optimize code */
        pDeriveKeySrv = &(pHseSrvDescriptor->hseSrv.keyDeriveReq);

        switch (u8KeyDeriveAlgo)
        {
#if (STD_ON == CRYPTO_SPT_KDF_ANS_X963)
            case (uint8)CRYPTO_ALGOFAM_KDFX963:
                pDeriveKeySrv->kdfAlgo                      = HSE_KDF_ALGO_ANS_X963;
                pDeriveKeySrv->sch.ANS_X963.srcKeyHandle    = (hseKeyHandle_t)u32KeyHandle;
                pDeriveKeySrv->sch.ANS_X963.targetKeyHandle = (hseKeyHandle_t)u32TargetKeyHandle;
                pDeriveKeySrv->sch.ANS_X963.keyMatLen       = (uint16)Crypto_aKeyElementList[u32TargetKeyMaterialElemIdx].u32CryptoKeyElementMaxSize;
                pDeriveKeySrv->sch.ANS_X963.kdfPrf          = HSE_KDF_PRF_HASH;
                /* KDF hash can only take the values defined for hseHmacPrfAlgo_t */
                pDeriveKeySrv->sch.ANS_X963.prfAlgo.hash    = (hseHmacPrfAlgo_t)HashAlgo.HseHashAlgo;

                if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32DerivSaltKeyElemIdx)
                {
                    pDeriveKeySrv->sch.ANS_X963.pInfo      = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32DerivSaltKeyElemIdx].pCryptoElementArray);
                    pDeriveKeySrv->sch.ANS_X963.infoLength = *Crypto_aKeyElementList[u32DerivSaltKeyElemIdx].pu32CryptoElementActualSize;
                }

                RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
                /* Mark the request as sent */
                *pbReqSentOverChannel = TRUE;
                break;
#endif /* (STD_ON == CRYPTO_SPT_KDF_ANS_X963) */

#if (STD_ON == CRYPTO_SPT_PBKDF2)
            case (uint8)CRYPTO_ALGOFAM_PBKDF2:
                pDeriveKeySrv->kdfAlgo                    = HSE_KDF_ALGO_PBKDF2HMAC;
                pDeriveKeySrv->sch.PBKDF2.srcKeyHandle    = (hseKeyHandle_t)u32KeyHandle;
                pDeriveKeySrv->sch.PBKDF2.targetKeyHandle = (hseKeyHandle_t)u32TargetKeyHandle;
                pDeriveKeySrv->sch.PBKDF2.keyMatLen       = (uint16)Crypto_aKeyElementList[u32TargetKeyMaterialElemIdx].u32CryptoKeyElementMaxSize;
                /* KDF hash can only take the values defined for hseHmacPrfAlgo_t */
                pDeriveKeySrv->sch.PBKDF2.hmacHash        = (hseHmacPrfAlgo_t)HashAlgo.HseHashAlgo;
                pDeriveKeySrv->sch.PBKDF2.iterations      = *Crypto_aKeyElementList[u32DerivIterationKeyElemIdx].pCryptoElementArray;

                if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32DerivSaltKeyElemIdx)
                {
                    pDeriveKeySrv->sch.PBKDF2.saltLength = *Crypto_aKeyElementList[u32DerivSaltKeyElemIdx].pu32CryptoElementActualSize;
                    pDeriveKeySrv->sch.PBKDF2.pSalt      = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32DerivSaltKeyElemIdx].pCryptoElementArray);
                }

                RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
                /* Mark the request as sent */
                *pbReqSentOverChannel = TRUE;
                break;
#endif /* (STD_ON == CRYPTO_SPT_PBKDF2) */

#if (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT)
            case CRYPTO_ALGOFAM_TLS12_GENERATE_MASTER:
            /* fall-through */
            case CRYPTO_ALGOFAM_TLS12_KEY_EXPANSION:
            /* fall-through */
            case CRYPTO_ALGOFAM_TLS12_GENERATE_VERIFY_CLIENT:
            /* fall-through */
            case CRYPTO_ALGOFAM_TLS12_GENERATE_VERIFY_SERVER:
                pDeriveKeySrv->kdfAlgo                 = HSE_KDF_ALGO_TLS12PRF;
#if (STD_ON == CRYPTO_SPT_HASH)
                pDeriveKeySrv->sch.TLS12Prf.hmacHash   = HashAlgo.HseHashAlgo;
#endif /* (STD_ON == CRYPTO_SPT_HASH) */
                pDeriveKeySrv->sch.TLS12Prf.seedLength = *Crypto_aKeyElementList[u32DerivSaltKeyElemIdx].pu32CryptoElementActualSize;
                pDeriveKeySrv->sch.TLS12Prf.pSeed      = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32DerivSaltKeyElemIdx].pCryptoElementArray);

                switch (u8KeyDeriveAlgo)
                {
                    case CRYPTO_ALGOFAM_TLS12_GENERATE_MASTER:
                        pDeriveKeySrv->sch.TLS12Prf.tlsPskUsage     = HSE_TLS_PSK_NOT_USED;
                        pDeriveKeySrv->sch.TLS12Prf.srcKeyHandle    = (hseKeyHandle_t)u32KeyHandle;
                        pDeriveKeySrv->sch.TLS12Prf.labelLength     = CRYPTO_TLS12_LABEL_MASTER_SECRET_SIZE;
                        pDeriveKeySrv->sch.TLS12Prf.pLabel          = HSE_PTR_TO_HOST_ADDR(Crypto_au8Tls12LabelMasterSecret);
                        pDeriveKeySrv->sch.TLS12Prf.targetKeyHandle = (hseKeyHandle_t)u32TargetMasterKeyHandle;
                        pDeriveKeySrv->sch.TLS12Prf.keyMatLength    = CRYPTO_TLS12_MASTER_KEY_SIZE;
                        pDeriveKeySrv->sch.TLS12Prf.outputLength    = 0U;
                        pDeriveKeySrv->sch.TLS12Prf.pOutput         = (HOST_ADDR)0U;
                        break;

                    case CRYPTO_ALGOFAM_TLS12_KEY_EXPANSION:
                        pDeriveKeySrv->sch.TLS12Prf.srcKeyHandle    = (hseKeyHandle_t)u32TargetMasterKeyHandle;
                        pDeriveKeySrv->sch.TLS12Prf.labelLength     = CRYPTO_TLS12_LABEL_KEY_EXPANSION_SIZE;
                        pDeriveKeySrv->sch.TLS12Prf.pLabel          = HSE_PTR_TO_HOST_ADDR(Crypto_au8Tls12LabelKeyExpansion);
                        pDeriveKeySrv->sch.TLS12Prf.targetKeyHandle = (hseKeyHandle_t)u32TargetKeyHandle;
                        pDeriveKeySrv->sch.TLS12Prf.keyMatLength    = Crypto_aKeyElementList[u32TargetOutputSizeKeyElemIdx].pCryptoElementArray[CRYPTO_TLS12_KEY_MATERIAL_LENGTH_IDX];
                        pDeriveKeySrv->sch.TLS12Prf.outputLength    = Crypto_aKeyElementList[u32TargetOutputSizeKeyElemIdx].pCryptoElementArray[CRYPTO_TLS12_IV_LENGTH_IDX];
                        pDeriveKeySrv->sch.TLS12Prf.pOutput         = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32TargetOutputDataKeyElemIdx].pCryptoElementArray);
                        break;

                    case CRYPTO_ALGOFAM_TLS12_GENERATE_VERIFY_CLIENT:
                        pDeriveKeySrv->sch.TLS12Prf.srcKeyHandle    = (hseKeyHandle_t)u32TargetMasterKeyHandle;
                        pDeriveKeySrv->sch.TLS12Prf.labelLength     = CRYPTO_TLS12_LABEL_CLIENT_FINISHED_SIZE;
                        pDeriveKeySrv->sch.TLS12Prf.pLabel          = HSE_PTR_TO_HOST_ADDR(Crypto_au8Tls12LabelClientFinished);
                        pDeriveKeySrv->sch.TLS12Prf.targetKeyHandle = (hseKeyHandle_t)0U;
                        pDeriveKeySrv->sch.TLS12Prf.keyMatLength    = 0U;
                        pDeriveKeySrv->sch.TLS12Prf.outputLength    = Crypto_aKeyElementList[u32TargetOutputSizeKeyElemIdx].pCryptoElementArray[CRYPTO_TLS12_VERIFY_DATA_LENGTH_IDX];
                        pDeriveKeySrv->sch.TLS12Prf.pOutput         = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32TargetOutputDataKeyElemIdx].pCryptoElementArray);
                        break;

                    case CRYPTO_ALGOFAM_TLS12_GENERATE_VERIFY_SERVER:
                        pDeriveKeySrv->sch.TLS12Prf.srcKeyHandle    = (hseKeyHandle_t)u32TargetMasterKeyHandle;
                        pDeriveKeySrv->sch.TLS12Prf.labelLength     = CRYPTO_TLS12_LABEL_SERVER_FINISHED_SIZE;
                        pDeriveKeySrv->sch.TLS12Prf.pLabel          = HSE_PTR_TO_HOST_ADDR(Crypto_au8Tls12LabelServerFinished);
                        pDeriveKeySrv->sch.TLS12Prf.targetKeyHandle = (hseKeyHandle_t)0U;
                        pDeriveKeySrv->sch.TLS12Prf.keyMatLength    = 0U;
                        pDeriveKeySrv->sch.TLS12Prf.outputLength    = Crypto_aKeyElementList[u32TargetOutputSizeKeyElemIdx].pCryptoElementArray[CRYPTO_TLS12_VERIFY_DATA_LENGTH_IDX];
                        pDeriveKeySrv->sch.TLS12Prf.pOutput         = HSE_PTR_TO_HOST_ADDR(Crypto_aKeyElementList[u32TargetOutputDataKeyElemIdx].pCryptoElementArray);
                        break;

                    default:
                        /* Code cannot actually get here */
                        break;
                }

                RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
                /* Mark the request as sent */
                *pbReqSentOverChannel = TRUE;

                /* If everything went fine, store the actual size of the CRYPTO_KE_KEYDERIVATION_OUTPUT_DATA key element */
                if ((Std_ReturnType)E_OK == RetVal)
                {
                    switch (u8KeyDeriveAlgo)
                    {
                        case CRYPTO_ALGOFAM_TLS12_KEY_EXPANSION:
                            *Crypto_aKeyElementList[u32TargetOutputDataKeyElemIdx].pu32CryptoElementActualSize = Crypto_aKeyElementList[u32TargetOutputSizeKeyElemIdx].pCryptoElementArray[CRYPTO_TLS12_IV_LENGTH_IDX];
                            break;

                        case CRYPTO_ALGOFAM_TLS12_GENERATE_VERIFY_CLIENT:
                        /* fall-through */
                        case CRYPTO_ALGOFAM_TLS12_GENERATE_VERIFY_SERVER:
                            *Crypto_aKeyElementList[u32TargetOutputDataKeyElemIdx].pu32CryptoElementActualSize = Crypto_aKeyElementList[u32TargetOutputSizeKeyElemIdx].pCryptoElementArray[CRYPTO_TLS12_VERIFY_DATA_LENGTH_IDX];
                            break;

                        default:
                            /* Code cannot actually get here */
                            break;
                    }
                }
                break;
#endif /* (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT) */

            default:
                /* Do nothing. Function will return E_NOT_OK for an unsupported derive algo */
                break;
        }
    }

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_SPT_KEY_DERIVE) */

#if (((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC)) && (STD_ON == CRYPTO_SPT_COMPUTE_DH))
/**
* @brief         Fill and send service data for computing secret
* @details       Fill and send service data for computing secret
*
* @param[in]       u8MuInstance:                MU instance where the Hse request will be sent
* @param[in]       u8MuChannel:                 MU channel where the Hse request will be sent
* @param[in]       pHseSrvDescriptor:           Descriptor to be filled and send to HSE
* @param[in]       u32CryptoKeyIdx:             Index of the Crypto key
* @param[in]       pPartnerPublicValue:         Value of the partner's public key
* @param[in]       u32PartnerPublicValueLength: Length of the partner's public key
* @param[in]       pJob:                        Pointer to the Crypto job
*
* @returns         Std_ReturnType
*                   E_OK:     Correct configuration of parameters
*                   E_NOT_OK: Incorrect configuration of parameters: incorrect lengths
*/
static Std_ReturnType Crypto_Hse_CalcSecretRequest
(
    const uint8           u8MuInstance,
    const uint8           u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    const uint32          u32CryptoKeyIdx,
    const uint8* const    pPartnerPublicValue,
    const uint32          u32PartnerPublicValueLength
)
{
    hseDHComputeSharedSecretSrv_t* pKeyGenSrv;
    Std_ReturnType                 RetVal                     = (Std_ReturnType)E_NOT_OK;
    const uint32                   u32PrivKeyElementIdx       = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYEXCHANGE_PRIVKEY);
    const uint32                   u32PartnerPubKeyElementIdx = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYEXCHANGE_PARTNERPUBKEY);
    const uint32                   u32SharedKeyElementIdx     = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CYRPTO_KE_KEYEXCHANGE_SHAREDVALUE);

    /* Check that all indexes returned by Crypto_Util_GetKeyElementIdx are valid. A logic OR between all indexes will be equal to CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 when at least 
       one of the indexes is invalid */
    if(CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != (u32PrivKeyElementIdx | u32PartnerPubKeyElementIdx | u32SharedKeyElementIdx))
    {
        pHseSrvDescriptor->srvId = HSE_SRV_ID_DH_COMPUTE_SHARED_SECRET;

        pKeyGenSrv                   = &(pHseSrvDescriptor->hseSrv.dhComputeSecretReq);
        pKeyGenSrv->privKeyHandle    = (hseKeyHandle_t)Crypto_aKeyElementList[u32PrivKeyElementIdx].u32HseKeyHandle;
        pKeyGenSrv->peerPubKeyHandle = (hseKeyHandle_t)Crypto_aKeyElementList[u32PartnerPubKeyElementIdx].u32HseKeyHandle;
        pKeyGenSrv->targetKeyHandle  = (hseKeyHandle_t)Crypto_aKeyElementList[u32SharedKeyElementIdx].u32HseKeyHandle;

        RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);

        /* Avoid compiler warnings */
        (void)pPartnerPublicValue;
        (void)u32PartnerPublicValueLength;
    }

    return RetVal;
}
#endif /* (((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC)) && (STD_ON == CRYPTO_SPT_COMPUTE_DH)) */

#if ((STD_ON == CRYPTO_SPT_SYM_KEY_IMPORT) || (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT))
/**
* @brief            Checks if the u8AuthCipherMode parameter is CCM or GCM.
* @details          Checks if the u8AuthCipherMode parameter is CCM or GCM.
*
* @param[in]        u8AuthCipherMode  Holds the auth cipher mode
*
* @return           Status of the check
* @retval           TRUE           u8AuthCipherMode param is set to either CCM or GCM
* @retval           FALSE          u8AuthCipherMode param is not set to neither CCM nor GCM
*
*
* @pre
*
*/
static inline boolean Crypto_Hse_AuthCipherModeIsCcmOrGcm
(
    const uint8 u8AuthCipherMode
)
{
    boolean RetVal = FALSE;

    if ((HSE_AUTH_CIPHER_MODE_CCM == (hseAuthCipherMode_t)u8AuthCipherMode) ||
        (HSE_AUTH_CIPHER_MODE_GCM == (hseAuthCipherMode_t)u8AuthCipherMode))
    {
        RetVal = TRUE;
    }
    return RetVal;
}
#endif /* ((STD_ON == CRYPTO_SPT_SYM_KEY_IMPORT) || (STD_ON == CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT)) */
#endif /*  (STD_ON == CRYPTO_KEYS_EXIST) */

/**
* @brief           Sends a mesage to HSE via MU
* @details         Sends a mesage to HSE via MU. The message contains the address of a descriptor
*
* @param[in]       u8MuInstance: MU instance used to send the request to HSE
* @param[in]       u8MuChannel:  MU channel used to send the request to HSE
* @param[in]       pHseSrvDescriptor:      Message to be sent to HSE
* @param[in]       eProcessType: SYNC/ASYNC
*
* @returns         Std_ReturnType
*                   E_OK:     Request successful
*                   E_NOT_OK: Request unsuccessful
*/
static Std_ReturnType Crypto_Hse_SendMsg
(
    const uint8                 u8MuInstance,
    const uint8                 u8MuChannel,
    hseSrvDescriptor_t* const   pHseSrvDescriptor,
    Crypto_JobType* const       pJob
)
{
    hseSrvResponse_t      HseResponse;
    Std_ReturnType        RetVal;
    Hse_Ip_ReqType*       pHseIpReq       = &Crypto_Hse_apMuState[u8MuInstance]->Hse_Ip_aRequest[u8MuChannel];
    /* If pJob parameter (pointer to the Crypto job requesting this service) is NULL, the request is for sure synchronous */
    Crypto_ProcessingType eProcessingType = CRYPTO_PROCESSING_SYNC;

    if (NULL_PTR != pJob)
    {
        eProcessingType = pJob->jobPrimitiveInfo->processingType;
        /* Set the value of the stored jobId */
        Crypto_Hse_apMuState[u8MuInstance]->au32MuChannelJobId[u8MuChannel] = pJob->jobInfo->jobId;
    }

    if (CRYPTO_PROCESSING_SYNC == eProcessingType)
    {
        pHseIpReq->eReqType   = HSE_IP_REQTYPE_SYNC;
        pHseIpReq->u32Timeout = Crypto_Hse_apMuState[u8MuInstance]->u32HseSyncRequestsTimeout;
    }
    else
    {
#if   (STD_ON == CRYPTO_USE_INTERRUPTS_FOR_ASYNC_JOBS)
        pHseIpReq->eReqType = HSE_IP_REQTYPE_ASYNC_IRQ;
#else
        pHseIpReq->eReqType = HSE_IP_REQTYPE_ASYNC_POLL;
#endif /*(STD_ON == CRYPTO_USE_INTERRUPTS_FOR_ASYNC_JOBS)*/

        pHseIpReq->pfCallback     = Crypto_Hse_ProcessMuChannelResponse;
        /* Set the pointer to the job as parameter to the callback */
        pHseIpReq->pCallbackParam = (void*)pJob;
    }

    /* Send the request to HSE driver */
    HseResponse = Hse_Ip_ServiceRequest(u8MuInstance, u8MuChannel, pHseIpReq, pHseSrvDescriptor);
    /* Translate the message received from HSE to a Std_ReturnType value */
    RetVal      = Crypto_Hse_TranslateHseResponse(HseResponse);

    if (CRYPTO_PROCESSING_SYNC == eProcessingType)
    {
        /* As the synchronous job is done, mark the channel as having an invalid JobId value */
        Crypto_Hse_apMuState[u8MuInstance]->au32MuChannelJobId[u8MuChannel] = CRYPTO_INVALID_VAL_FOR_JOBID_U32;
    }

    return RetVal;
}

/**
* @brief           Checks the CDO queues for pending messages and sends them to HSE for processing
* @details         Checks the CDO queues for pending messages and sends them to HSE for processing
*
* @param[in]       u8MuInstance: MU instance used to send the request to HSE
*
* @returns         void
*/
static void Crypto_Hse_PushJobsFromQueuesToHse
(
    const uint8 u8MuInstance
)
{
    const uint32    u32PartitionId           = Crypto_Util_GetPartitionId();
    boolean         bJobSentToHseAndDequeued = TRUE;
    boolean         bBreakWhile;
    Std_ReturnType  StdRetVal;
    uint32          u32HeadOfQueuedJobs;
    uint32          u32NumCDOsNotAllowedToProcessQueuedJobs;
    Crypto_JobType* pJob;
    uint8           u8ObjectIdx;
    static uint32   u32ObjectCounter[CRYPTO_MAX_NUMBER_PARTITIONS_U8] = {0U};

    /* Get the index of the Crypto Driver Object processed on the current partition */
    u8ObjectIdx = Crypto_aPartitionToCdoMapping[u32PartitionId].au8CDOsList[u32ObjectCounter[u32PartitionId]];

    /* Try to get jobs from the CDO's queues only if the Crypto Driver has been initialized */
    if (CRYPTO_DRIVER_INITIALIZED == Crypto_aeDriverState[u32PartitionId])
    {
        /* Check if the current CDO does not have jobs in queue */
        if (CRYPTO_JOB_QUEUE_NONE_U32 == Crypto_aObjectQueueList[u8ObjectIdx].u32HeadOfQueuedJobs)
        {
            /* Pass to the queue of the next CDO in the current partition */
            u32ObjectCounter[u32PartitionId] = (u32ObjectCounter[u32PartitionId] + 1UL)%Crypto_aPartitionToCdoMapping[u32PartitionId].u8NumCDOs;
            u8ObjectIdx                      = Crypto_aPartitionToCdoMapping[u32PartitionId].au8CDOsList[u32ObjectCounter[u32PartitionId]];
        }
        u32NumCDOsNotAllowedToProcessQueuedJobs = 0U;
        /* Keep looping while we manage to dequeue async jobs from any of the CDOs queues and send them to HSE */
        while (TRUE == bJobSentToHseAndDequeued)
        {
            bJobSentToHseAndDequeued = FALSE;
            /* Clear the break so that the queue of the next CDO in the current partition will be processed if the process operation function is not busy */
            bBreakWhile = FALSE;
            /* Try to take the first job from each CDO's queue of jobs and send it to HSE, in order to assure that jobs requests from all CDOs are sent being processed with the same priority */
            while ((CRYPTO_JOB_QUEUE_NONE_U32 != Crypto_aObjectQueueList[u8ObjectIdx].u32HeadOfQueuedJobs) && (FALSE == bBreakWhile))
            {
                u32HeadOfQueuedJobs = Crypto_aObjectQueueList[u8ObjectIdx].u32HeadOfQueuedJobs;
                /* Take the first job in the queue. The queue is already ordered by priority so the first job in queue has the highest priority */
                pJob = Crypto_aDriverObjectList[u8ObjectIdx].pQueuedJobs[u32HeadOfQueuedJobs].pJob;

                /* SWS_Crypto_00199 - If the Crypto Driver has a queue and if a synchronous job is issued and the priority is greater than the highest priority available in the queue, the Crypto Driver shall disable
                processing new jobs from the queue until the next call of the main function has finished that follows after completion of the currently processed job.*/
                if ((NULL_PTR != Crypto_apSyncJobInProgress[u8ObjectIdx]) &&
                   (Crypto_apSyncJobInProgress[u8ObjectIdx]->jobInfo->jobPriority > pJob->jobInfo->jobPriority))
                {
                    /* Count the number of CDO's for which processing was not allowed because of synchronous request ongoing */
                    u32NumCDOsNotAllowedToProcessQueuedJobs++;
                    /* If all the CDOs were not allowed to process queued jobs, exit the inner loop*/
                    if (Crypto_aPartitionToCdoMapping[u32PartitionId].u8NumCDOs == u32NumCDOsNotAllowedToProcessQueuedJobs)
                    {
                        break;
                    }
                    /* Pass to the queue of the next CDO in the current partition */
                    u32ObjectCounter[u32PartitionId] = (u32ObjectCounter[u32PartitionId] + 1UL)%Crypto_aPartitionToCdoMapping[u32PartitionId].u8NumCDOs;
                    u8ObjectIdx                      = Crypto_aPartitionToCdoMapping[u32PartitionId].au8CDOsList[u32ObjectCounter[u32PartitionId]];
                    continue;
                }

                /* Try to send the job to HSE for processing */
                StdRetVal = Crypto_Hse_ProcessOperation(u8MuInstance, u8ObjectIdx, pJob);

                if ((Std_ReturnType)CRYPTO_E_BUSY != StdRetVal)
                {
                    /* If return value is not CRYPTO_E_BUSY, it means that a free MU channel was found and the request was send to HSE
                       Take the first queued job and put it back in the free jobs queue */
                    Crypto_Util_DequeueHeadJob(u8ObjectIdx);
                    /* Mark the job as being dequeued and sent to HSE for processing */
                    bJobSentToHseAndDequeued = TRUE;
                    /* [SWS_Crypto_00119]If return value is not E_OK, call the callback to inform the upper layers an error happened with the job */
                    if ((Std_ReturnType)E_OK != StdRetVal)
                    {
                        /* [SWS_Crypto_00119]Clear the job state in case of error. If no error occurs the state will be cleared in Crypto_Hse_ProcessMuChannelResponse */
                        pJob->jobState = CRYPTO_JOBSTATE_IDLE;
                        CryIf_CallbackNotification (pJob, StdRetVal);
                    }
                    /* Pass to the queue of the next CDO in the current partition */
                    u32ObjectCounter[u32PartitionId] = (u32ObjectCounter[u32PartitionId] + 1UL)%Crypto_aPartitionToCdoMapping[u32PartitionId].u8NumCDOs;
                    u8ObjectIdx                      = Crypto_aPartitionToCdoMapping[u32PartitionId].au8CDOsList[u32ObjectCounter[u32PartitionId]];
                }
                else
                {
                    /* Exit the inner while without updating the CDO, so that at next MainFunction call the current CDO has the chance to send again the job to HSE */
                    bBreakWhile = TRUE;
                }
            }
        }
    }
}


/**
* @brief           Calls the function which will service the request
* @details         Based on the configured service in job a request will be send to the firmware
*
* @param[in]       u8MuInstance:         MU instance where the job will be issued
* @param[in]       u8MuChannel:          MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       pJob:                 Pointer to the job
* @param[in]       u8StreamId:           Allocated stream ID
* @param[out]      pbReqSentOverChannel: Pointer to a boolean storing channel usage
*
* @returns         Std_ReturnType
*                   E_OK:                         Request successful
*                   E_NOT_OK:                     Request unsuccessful
*                   CRYPTO_E_KEY_EMPTY:           The service request failed because the specified key slot is empty
*                   CRYPTO_E_KEY_NOT_AVAILABLE:   This error code is returned if a key is locked due to failed boot measurement or an active debugger
*                   CRYPTO_E_KEY_NOT_VALID:       Specified key slot is either not valid or not available due to a key usage flags restrictions
*                   CRYPTO_RET_INVALID_PARAM:     The request parameters are invalid
*                   CRYPTO_RET_OPERATION_TIMEOUT: No response was received from firmware in the timeout window
*                   CRYPTO_RET_NOT_SUPPORTED:     Service is not available
*/
static inline Std_ReturnType Crypto_Hse_ServiceRequest
(
    const uint8           u8MuInstance,
    const uint8           u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
)
{
    Std_ReturnType  RetVal    = (Std_ReturnType)E_NOT_OK;

    switch (pJob->jobPrimitiveInfo->primitiveInfo->service)
    {
#if (STD_ON == CRYPTO_KEYS_EXIST)
#if (STD_ON == CRYPTO_SPT_MAC)
        case CRYPTO_MACGENERATE:
        /* fall-through */
        case CRYPTO_MACVERIFY:
            RetVal = Crypto_Hse_MacServiceRequest(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, u8StreamId, pbReqSentOverChannel);
            break;
#endif /* (STD_ON == CRYPTO_SPT_MAC) */

        case CRYPTO_ENCRYPT:
#if (STD_ON == CRYPTO_SPT_RSA)
            if (CRYPTO_ALGOFAM_RSA == pJob->jobPrimitiveInfo->primitiveInfo->algorithm.family)
            {
                RetVal = Crypto_Hse_RSAEncDec(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, HSE_CIPHER_DIR_ENCRYPT, pbReqSentOverChannel);
            }
            else
#endif /* (STD_ON == CRYPTO_SPT_RSA) */
            {
                RetVal = Crypto_Hse_EncDec(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, u8StreamId, HSE_CIPHER_DIR_ENCRYPT, pbReqSentOverChannel);
            }
            break;

        case CRYPTO_DECRYPT:
#if (STD_ON == CRYPTO_SPT_RSA)
            if (CRYPTO_ALGOFAM_RSA == pJob->jobPrimitiveInfo->primitiveInfo->algorithm.family)
            {
                RetVal = Crypto_Hse_RSAEncDec(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, HSE_CIPHER_DIR_DECRYPT, pbReqSentOverChannel);
            }
            else
#endif /* (STD_ON == CRYPTO_SPT_RSA) */
            {
                RetVal = Crypto_Hse_EncDec(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, u8StreamId, HSE_CIPHER_DIR_DECRYPT, pbReqSentOverChannel);
            }
            break;

#if (STD_ON == CRYPTO_SPT_AEAD)
        case CRYPTO_AEADENCRYPT:
            RetVal = Crypto_Hse_AeadEncDec(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, u8StreamId, HSE_CIPHER_DIR_ENCRYPT, pbReqSentOverChannel);
            break;

        case CRYPTO_AEADDECRYPT:
            RetVal = Crypto_Hse_AeadEncDec (u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, u8StreamId, HSE_CIPHER_DIR_DECRYPT, pbReqSentOverChannel);
            break;
#endif /* (STD_ON == CRYPTO_SPT_AEAD) */

#if ((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC))
        case CRYPTO_SIGNATUREGENERATE:
        /* fall-through */
        case CRYPTO_SIGNATUREVERIFY:
            RetVal = Crypto_Hse_SignRequest(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, u8StreamId, pbReqSentOverChannel);
            break;
#endif /* ((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC)) */
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

#if (STD_ON == CRYPTO_SPT_HASH)
        case CRYPTO_HASH:
            RetVal = Crypto_Hse_Hash(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, u8StreamId, pbReqSentOverChannel);
            break;
#endif /* (STD_ON == CRYPTO_SPT_HASH) */

        case CRYPTO_RANDOMGENERATE:
            RetVal = Crypto_Hse_Random(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob);
            /* Crypto_Hse_Random() will always send the request to firmware */
            *pbReqSentOverChannel = TRUE;
            break;

#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT))
        /* Functionality of synchronous key management functions accessed through Crypto_ProcessJob interface */
        case CRYPTO_KEYGENERATE:
#if (STD_ON == CRYPTO_SPT_KEY_GENERATE)
            RetVal = Crypto_Hse_KeyGenerateRequest(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, pJob->jobPrimitiveInputOutput.cryIfKeyId, pbReqSentOverChannel);
#else
            /* HSE does not support this service */
            RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;
#endif
            break;

        case CRYPTO_KEYDERIVE:
#if (STD_ON == CRYPTO_SPT_KEY_DERIVE)
            RetVal = Crypto_Hse_KeyDeriveRequest(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, pJob->jobPrimitiveInputOutput.cryIfKeyId, pJob->jobPrimitiveInputOutput.targetCryIfKeyId, pbReqSentOverChannel);
#else
            /* HSE does not support this service */
            RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;
#endif /* (STD_ON == CRYPTO_SPT_KEY_DERIVE) */
            break;

        case CRYPTO_KEYEXCHANGECALCSECRET:
#if (((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC)) && (STD_ON == CRYPTO_SPT_COMPUTE_DH))
            RetVal = Crypto_Hse_CalcSecretRequest(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, pJob->jobPrimitiveInputOutput.cryIfKeyId,  pJob->jobPrimitiveInputOutput.inputPtr, pJob->jobPrimitiveInputOutput.inputLength);
            /* Crypto_Hse_CalcSecretRequest() will always send the request to firmware */
            *pbReqSentOverChannel = TRUE;
#else
            /* HSE does not support this service */
            RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;
#endif /* (((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC)) && (STD_ON == CRYPTO_SPT_COMPUTE_DH)) */
            break;

        case CRYPTO_RANDOMSEED:
            /* HSE does not support this service */
            RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;
            break;

#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT)) */
        default:
            /* Should not get here. If yes, report an error */
            RetVal = (Std_ReturnType)E_NOT_OK;
            break;
    }
#if ((STD_OFF == CRYPTO_KEYS_EXIST) && (STD_OFF == CRYPTO_SPT_HASH))

    /* Ignore compiler warning */
    (void)u8StreamId;
#endif

    return RetVal;
}

#if ((STD_ON == CRYPTO_SPT_MAC) && (STD_ON == CRYPTO_KEYS_EXIST))
/**
* @brief           Calls the MAC function which will service the request
* @details         Based on the configured service in job a MAC request will be send to the firmware
*
* @param[in]       u8MuInstance:         MU instance where the job will be issued
* @param[in]       u8MuChannel:          MU channel where the job will be issued
* @param[in]       pHseSrvDescriptor:    Descriptor to be filled and send to HSE
* @param[in]       pJob:                 Pointer to the job
* @param[in]       u8StreamId:           Allocated stream ID
* @param[out]      pbReqSentOverChannel: Pointer to a boolean storing channel usage
*
* @returns         Std_ReturnType
*                   E_OK:                         Request successful
*                   E_NOT_OK:                     Request unsuccessful
*                   CRYPTO_E_KEY_EMPTY:           The service request failed because the specified key slot is empty
*                   CRYPTO_E_KEY_NOT_AVAILABLE:   This error code is returned if a key is locked due to failed boot measurement or an active debugger
*                   CRYPTO_E_KEY_NOT_VALID:       Specified key slot is either not valid or not available due to a key usage flags restrictions
*                   CRYPTO_RET_INVALID_PARAM:     The request parameters are invalid
*                   CRYPTO_RET_OPERATION_TIMEOUT: No response was received from firmware in the timeout window
*                   CRYPTO_RET_NOT_SUPPORTED:     Service is not available
*/
static inline Std_ReturnType Crypto_Hse_MacServiceRequest
(
    const uint8           u8MuInstance,
    const uint8           u8MuChannel,
    hseSrvDescriptor_t*   pHseSrvDescriptor,
    Crypto_JobType*       pJob,
    uint8                 u8StreamId,
    boolean*              pbReqSentOverChannel
)
{
    Std_ReturnType            RetVal     = (Std_ReturnType)E_NOT_OK;
#if ((STD_ON == CRYPTO_SPT_HMAC) && (STD_ON == CRYPTO_SPT_HASH))
    Crypto_Hse_HashConfigType HashAlgo   = Crypto_Hse_ConfigureHashAlgo((uint8)pJob->jobPrimitiveInfo->primitiveInfo->algorithm.family);
#endif /* ((STD_ON == CRYPTO_SPT_HMAC) && (STD_ON == CRYPTO_SPT_HASH)) */
    /* Put algorithm.mode in a local variable to optimize code */
    uint8                     u8AlgoMode = (uint8)pJob->jobPrimitiveInfo->primitiveInfo->algorithm.mode;

    if (CRYPTO_MACGENERATE == pJob->jobPrimitiveInfo->primitiveInfo->service)
    {
        switch (u8AlgoMode)
        {
#if (STD_ON == CRYPTO_SPT_FAST_CMAC)
            case (uint8)CRYPTO_ALGOMODE_FAST_CMAC:
                RetVal = Crypto_Hse_FastCmac(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, HSE_AUTH_DIR_GENERATE, pbReqSentOverChannel);
                break;
#endif /* (STD_ON == CRYPTO_SPT_FAST_CMAC) */

#if (STD_ON == CRYPTO_SPT_CMAC) || (STD_ON == CRYPTO_SPT_XCBCMAC)
            case (uint8)CRYPTO_ALGOMODE_CMAC:
            /* fall-through */
            case (uint8)CRYPTO_ALGOMODE_CBC:
                RetVal = Crypto_Hse_CmacXcbcMacGenerate(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, u8StreamId, pbReqSentOverChannel);
                break;
#endif /* (STD_ON == CRYPTO_SPT_CMAC) || (STD_ON == CRYPTO_SPT_XCBCMAC) */

#if (STD_ON == CRYPTO_SPT_GMAC)
            case (uint8)CRYPTO_ALGOMODE_GMAC:
                RetVal = Crypto_Hse_GmacGenerate (u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, u8StreamId, pbReqSentOverChannel);
                break;
#endif /* (STD_ON == CRYPTO_SPT_GMAC) */

#if (STD_ON == CRYPTO_SPT_HMAC) && (STD_ON == CRYPTO_SPT_HASH)
            case (uint8)CRYPTO_ALGOMODE_HMAC:
                RetVal = Crypto_Hse_HmacGen(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, HashAlgo.HseHashAlgo, u8StreamId, pbReqSentOverChannel);
                break;
#endif /* (STD_ON == CRYPTO_SPT_HMAC) && (STD_ON == CRYPTO_SPT_HASH) */

#if (STD_ON == CRYPTO_SPT_SIPHASH)
            case (uint8)CRYPTO_ALGOMODE_SIPHASH_2_4:
            /* fall-through */
            case (uint8)CRYPTO_ALGOMODE_SIPHASH_2_4_128:
                RetVal = Crypto_Hse_SipHash(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, HSE_AUTH_DIR_GENERATE, pbReqSentOverChannel);
                break;
#endif /* (STD_ON == CRYPTO_SPT_SIPHASH) */

            default:
                RetVal = (Std_ReturnType)E_NOT_OK;
                break;
        }
    }
    else
    {
        switch (u8AlgoMode)
        {
#if (STD_ON == CRYPTO_SPT_FAST_CMAC)
            case (uint8)CRYPTO_ALGOMODE_FAST_CMAC:
                RetVal = Crypto_Hse_FastCmac(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, HSE_AUTH_DIR_VERIFY, pbReqSentOverChannel);
                break;
#endif /* (STD_ON == CRYPTO_SPT_FAST_CMAC) */

#if (STD_ON == CRYPTO_SPT_CMAC) || (STD_ON == CRYPTO_SPT_XCBCMAC)
            case (uint8)CRYPTO_ALGOMODE_CMAC:
            /* fall-through */
            case (uint8)CRYPTO_ALGOMODE_CBC:
                RetVal = Crypto_Hse_CmacXcbcMacVerify(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, u8StreamId, pbReqSentOverChannel);
                break;
#endif /* (STD_ON == CRYPTO_SPT_CMAC) || (STD_ON == CRYPTO_SPT_XCBCMAC) */

#if (STD_ON == CRYPTO_SPT_GMAC)
            case (uint8)CRYPTO_ALGOMODE_GMAC:
                RetVal = Crypto_Hse_GmacVerify (u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, u8StreamId, pbReqSentOverChannel);
                break;
#endif /* (STD_ON == CRYPTO_SPT_GMAC) */

#if (STD_ON == CRYPTO_SPT_HMAC) && (STD_ON == CRYPTO_SPT_HASH)
            case (uint8)CRYPTO_ALGOMODE_HMAC:
                RetVal = Crypto_Hse_HmacVerify (u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, HashAlgo.HseHashAlgo, u8StreamId, pbReqSentOverChannel);
                break;
#endif /* (STD_ON == CRYPTO_SPT_HMAC) && (STD_ON == CRYPTO_SPT_HASH)*/

#if (STD_ON == CRYPTO_SPT_SIPHASH)
            case (uint8)CRYPTO_ALGOMODE_SIPHASH_2_4:
            /* fall-through */
            case (uint8)CRYPTO_ALGOMODE_SIPHASH_2_4_128:
                RetVal = Crypto_Hse_SipHash(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pJob, HSE_AUTH_DIR_VERIFY, pbReqSentOverChannel);
                break;
#endif /* (STD_ON == CRYPTO_SPT_SIPHASH) */

            default:
                RetVal = (Std_ReturnType)E_NOT_OK;
                break;
        }
    }
    return RetVal;
}
#endif/* ((STD_ON == CRYPTO_SPT_MAC) && (STD_ON == CRYPTO_KEYS_EXIST)) */

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
/**
* @brief           Initializes the driver
* @details         Initializes the driver
*
* @param[in]       u32PartitionId:   Id of the partition from where the call to the function was made
*
* @returns         Std_ReturnType: Status of initializing the MU
*/
Std_ReturnType Crypto_Hse_Init
(
    const uint32 u32PartitionId
)
{
    const uint8    u8MuInstance    = Crypto_aPartitionToMuMapping[u32PartitionId].u8MuInstance;
    const uint8    u8IdxMuInstance = Crypto_aPartitionToMuMapping[u32PartitionId].u8IdxMuInstance;
    uint8          u8CDOCounter;
    uint8          u8CDOIdx;
    uint8          u8MuChannel;

    /* Initialize the pointer to the Crypto_Hse layer state machine */
    Crypto_Hse_apMuState[u8MuInstance] = &Crypto_aCryptoHseMuState[u8IdxMuInstance];

    /* Initialize the Hse Ip layer */
    (void)Hse_Ip_Init(u8MuInstance, &(Crypto_Hse_apMuState[u8MuInstance]->Hse_Ip_MuState));

    /* Initialize the CDO pointers that store the synchronous job currently processing */
    for (u8CDOCounter = 0U; u8CDOCounter < Crypto_aPartitionToCdoMapping[u32PartitionId].u8NumCDOs; u8CDOCounter++)
    {
        u8CDOIdx = Crypto_aPartitionToCdoMapping[u32PartitionId].au8CDOsList[u8CDOCounter];
        Crypto_apSyncJobInProgress[u8CDOIdx] = NULL_PTR;
    }

    for (u8MuChannel = 0U; u8MuChannel < HSE_NUM_OF_CHANNELS_PER_MU; u8MuChannel++)
    {
        /* Initialize the jobId's of the references to Hse requests kept per MU instance to some magic number.
           This is needed in order to have the CancelJob work correctly even when trying to launch a Crypto_CancelJob() for a never launched job having jobId = 0 */
        Crypto_Hse_apMuState[u8MuInstance]->au32MuChannelJobId[u8MuChannel] = CRYPTO_INVALID_VAL_FOR_JOBID_U32;
    }
    /* Set the pointer to the start of the 16 global service descriptors allocated for the current used MU instance */
    Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor = &(Crypto_aHseSrvDescriptor[u8IdxMuInstance][0U]);
    /* Set default timeout for synchronous operations */
    Crypto_Hse_apMuState[u8MuInstance]->u32HseSyncRequestsTimeout = CRYPTO_TIMEOUT_DURATION_U32;

    /* Initialize stream status */
    Crypto_Hse_apMuState[u8MuInstance]->u8StreamBusyBitMap = (uint8)0U;

    return (Std_ReturnType)E_OK;
}

/**
* @brief           Processes a job
* @details         Tries to initiate a request to HSE to process a job. If all MU channels are busy and the job is async, it queues it
*
* @param[in]       u8MuInstance: MU instance used to send the request to HSE
* @param[in]       u32ObjectIdx: Identifier of the Crypto Driver Object to be used for processing the job
* @param[in]       pJob:         Pointer to the job to be processed
*
* @returns         Std_ReturnType
*                   E_OK:                Request successful
*                   CRYPTO_E_BUSY:       Sync request received when Crypto is busy or async request received when Crypto is busy and queue size is 0
*                   CRYPTO_E_QUEUE_FULL: Ssync request received when Crypto is busy and there is no more room to store the job in the queue
*/
Std_ReturnType Crypto_Hse_ProcessJob
(
    const uint8           u8MuInstance,
    const uint32          u32ObjectIdx,
    Crypto_JobType* const pJob
)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;

    if (CRYPTO_PROCESSING_SYNC == pJob->jobPrimitiveInfo->processingType)
    {
        Crypto_apSyncJobInProgress[u32ObjectIdx] = pJob;
    }

    {
        /* Try to send the job for processing to HSE over MU */
        RetVal = Crypto_Hse_ProcessOperation(u8MuInstance, u32ObjectIdx, pJob);
    }
    /* Processing of an eventual synchronous request has finished. Set pointer back to NULL */
    Crypto_apSyncJobInProgress[u32ObjectIdx] = NULL_PTR;

    /* If there was no MU channel available to process the request, check the type of the request (sync or async) */
    if ((Std_ReturnType)CRYPTO_E_BUSY == RetVal)
    {
        if  (
               (CRYPTO_PROCESSING_ASYNC == pJob->jobPrimitiveInfo->processingType) && \
               (0U != Crypto_aDriverObjectList[u32ObjectIdx].u32CryptoQueueSize)
            )
        {
            /* Try to queue the job */
            if (TRUE == Crypto_Util_QueueJob(u32ObjectIdx, pJob))
            {
                /* Set return value to E_OK, as the async job has been queued successfully for later processing */
                RetVal = (Std_ReturnType)E_OK;
            }
            else
            {
                /* Job could not be queued as the CDO queue is full */
                RetVal = (Std_ReturnType)CRYPTO_E_QUEUE_FULL;
            }
        }
    }
    else if ((Std_ReturnType)E_OK != RetVal)
    {
        /* [SWS_Crypto_00025]Clear the job state in case of error. If no error occurs the state will be cleared in Crypto_Hse_ProcessMuChannelResponse */
        pJob->jobState = CRYPTO_JOBSTATE_IDLE;
    }
    /* [SWS_Crypto_00018][SWS_Crypto_00023] If the job is processed synchronously and operation mode is set to single call or finish change the state of the job to idle */
    else if ((CRYPTO_PROCESSING_SYNC == pJob->jobPrimitiveInfo->processingType) && ((uint8)CRYPTO_OPERATIONMODE_FINISH == (uint8)(((uint8)(pJob->jobPrimitiveInputOutput.mode)) & CRYPTO_OPERATIONMODE_FINISH_MASK_U8)))
    {
        pJob->jobState = CRYPTO_JOBSTATE_IDLE;
    }
    else
    {
        /*Do nothing */
    }
    return RetVal;
}

/**
* @brief           Cancels a job
* @details         Tries to cancel the currently processing job if the id is the same. Otherwise searches the queue of jobs in order to dequeue it
*
* @param[in]       u8MuInstance: MU instance used to send the request to HSE
* @param[in]       u32ObjectIdx: Identifier of the Crypto Driver Object to be used for canceling the job
* @param[in]       pJobInfo:     Pointer to the jobInfo of the job to be canceled
*
* @returns         Std_ReturnType
*/
Std_ReturnType Crypto_Hse_CancelJob
(
    const uint8                     u8MuInstance,
    const uint32                    u32ObjectIdx,
    const Crypto_JobInfoType* const pJobInfo
)
{
    uint8               u8Index;
    hseSrvDescriptor_t* pHseSrvDescriptor;
    Std_ReturnType      RetVal    = (Std_ReturnType)E_OK;
    boolean             bJobFound = FALSE;
    uint8               u8MuChannel;

    /* Loop through all MU channels where a request have been sent but a response was not yet received */
    for (u8Index = 0U; u8Index < HSE_NUM_OF_CHANNELS_PER_MU; u8Index++)
    {
        /* Search for a job:
            1. coming from the Crypto Driver Object received as parameter
            2. matching the same jobId like the one received as parameter */
        if ((Crypto_Hse_apMuState[u8MuInstance]->au32MuChannelCDOIdx[u8Index] == u32ObjectIdx) &&
           (Crypto_Hse_apMuState[u8MuInstance]->au32MuChannelJobId[u8Index]  == pJobInfo->jobId))
        {
            /* Administrative requests must only be sent on channel 0 */
            u8MuChannel = CRYPTO_CHANNEL_0_U8;
            /* Obtain a descriptor for the request */
            pHseSrvDescriptor = &(Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor[u8MuChannel]);
            /* Try to cancel the job */
            RetVal = Crypto_Hse_SendCancelJobReq(u8MuInstance, u8MuChannel, pHseSrvDescriptor, u8Index);
            bJobFound = TRUE;
            break;
        }
    }

    /* If job was not found in the list of jobs sent to HSE for processing, check if it's not waiting in the queue */
    if (FALSE == bJobFound)
    {
        /* No matter if the job is found in the CDO's queue or not, always return E_OK because of the following 2 requirements:
           - SWS_Crypto_00143: If no errors are detected by Crypto Driver and the driver is able to cancel the job immediately, the service Crypto_CancelJob() shall remove the job from the queue
        and cancel the job in the hardware. If the cancellation is successful E_OK shall be returned, otherwise it shall return E_NOT_OK.
           - SWS_Crypto_00214: If no errors are detected by Crypto Driver and the driver does currently not process this job, the service Crypto_CancelJob() shall return E_OK without any processing. */
        (void)Crypto_Util_DequeueJob(u32ObjectIdx, pJobInfo);
    }

    return RetVal;
}

#if (STD_ON == CRYPTO_KEYS_EXIST)
/**
* @brief           Imports a key
* @details         Fills the descriptor for importing a key
*
* @param[in]       u8MuInstance:           MU instance used to send the request to HSE
* @param[in]       pKey:                   Address of the key to be loaded
* @param[in]       u32KeyLength:           Key length
* @param[in]       u32CryptoKeyIdx:        Id of the Crypto Key for which the key material is imported
* @param[in]       u32CryptoKeyElementIdx: The index of the Crypto Key Element
*
* @returns         Std_ReturnType
*                       E_OK: request successful
*                       E_NOT_OK: request unsuccessful
*                       CRYPTO_E_SMALL_BUFFER - provided buffer is to small
*                       CRYPTO_E_KEY_READ_FAIL - failure in reading the key
*                       CRYPTO_E_KEY_WRITE_FAIL - failure in writing the key
*                       CRYPTO_E_KEY_NOT_AVAILABLE - key not available
*                       CRYPTO_E_KEY_NOT_VALID - invalid key
*                       CRYPTO_E_BUSY - busy
*/
Std_ReturnType Crypto_Hse_ImportKey
(
    const uint8        u8MuInstance,
    const uint8* const pKey,
    const uint32       u32KeyLength,
    const uint32       u32CryptoKeyIdx,
    const uint32       u32CryptoKeyElementIdx
)
{
    Std_ReturnType                   RetVal                 = (Std_ReturnType)E_NOT_OK;
    uint8                            u8KeyType              = Crypto_aKeyElementList[u32CryptoKeyElementIdx].u8HseKeyType;
    Crypto_KeyElementWriteAccessType eKeyElementWriteAccess = Crypto_aKeyElementList[u32CryptoKeyElementIdx].eCryptoKeyElementWriteAccess;
    Crypto_KeyFormatType             eKeyElementFormat      = Crypto_aKeyElementList[u32CryptoKeyElementIdx].eCryptoKeyFormat;
    uint8                            u8MuChannel;
    hseSrvDescriptor_t*              pHseSrvDescriptor;
#if (STD_ON == CRYPTO_SPT_SHE_KEY_IMPORT)
    /* In case of SHE keys,  MAC Proof key element and Cipher Proof key element ar needed for M4 and M5 output , Autosar marks them as not mandatory */
    uint32 u32MacProofKeyElemIdx    = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_MAC_PROOF);
    uint32 u32CipherProofKeyElemIdx = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_CIPHER_PROOF);
#endif /* (STD_ON == CRYPTO_SPT_SHE_KEY_IMPORT) */
#if (STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT)
     uint8 u8CurveId = Crypto_aKeyElementList[u32CryptoKeyElementIdx].u8HseEccCurveId;
#endif /* (STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT) */

    /* Get a free channel, if possible */
    u8MuChannel = Hse_Ip_GetFreeChannel(u8MuInstance);
    /* If a free channel is found */
    if (HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        pHseSrvDescriptor = &(Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor[u8MuChannel]);
        switch (u8KeyType)
        {
#if (STD_ON == CRYPTO_SPT_SHE_KEY_IMPORT)
            case HSE_KEY_TYPE_SHE:
                /* Ignore compiler warning */
                (void)u32KeyLength;

                if ((Crypto_KeyFormatType)CRYPTO_KE_FORMAT_BIN_SHEKEYS == eKeyElementFormat)
                {
                    if (CRYPTO_WA_ENCRYPTED == eKeyElementWriteAccess)
                    {
                        RetVal = Crypto_Hse_SheLoadKey(u8MuInstance,
                                                       u8MuChannel,
                                                       pHseSrvDescriptor,
                                                       pKey,
                                                       u32MacProofKeyElemIdx,
                                                       u32CipherProofKeyElemIdx,
                                                       u32CryptoKeyElementIdx);
                    }
                    else
                    {
                        RetVal = Crypto_Hse_SheLoadPlainKey(u8MuInstance,
                                                            u8MuChannel,
                                                            pHseSrvDescriptor,
                                                            pKey);
                    }
                }
                break;
#endif /* (STD_ON == CRYPTO_SPT_SHE_KEY_IMPORT) */

#if (STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT)
            case HSE_KEY_TYPE_ECC_PAIR:
                 if ((Crypto_KeyFormatType)CRYPTO_KE_FORMAT_BIN_OCTET == eKeyElementFormat)
                 {
                     if (CRYPTO_WA_ENCRYPTED == eKeyElementWriteAccess)
                     {
                         /* Not supported */
                     }
                     else
                     {
                         RetVal = Crypto_Hse_EccLoadPlainPairKey (u8MuInstance,
                                                                  u8MuChannel,
                                                                  pHseSrvDescriptor,
                                                                  pKey,
                                                                  u32KeyLength,
                                                                  u32CryptoKeyElementIdx,
                                                                  u8CurveId
                                                                 );
                     }
                 }
                break;

            case HSE_KEY_TYPE_ECC_PUB:
                if ((Crypto_KeyFormatType)CRYPTO_KE_FORMAT_BIN_OCTET == eKeyElementFormat)
                {
                    if (CRYPTO_WA_ENCRYPTED == eKeyElementWriteAccess)
                    {
                     /* Not supported */
                    }
                    else
                    {

                     RetVal = Crypto_Hse_EccLoadPlainPubKey(u8MuInstance,
                                                            u8MuChannel,
                                                            pHseSrvDescriptor,
                                                            pKey,
                                                            u32KeyLength,
                                                            u32CryptoKeyElementIdx,
                                                            u8CurveId
                                                            );
                    }
                }
                break;
#endif /* (STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT) */

#if (STD_ON == CRYPTO_SPT_RSA_KEY_IMPORT)
            case  HSE_KEY_TYPE_RSA_PAIR:
                if (((Crypto_KeyFormatType)CRYPTO_KE_FORMAT_BIN_RSA_PRIVATEKEY == eKeyElementFormat) || ((Crypto_KeyFormatType)CRYPTO_KE_FORMAT_BIN_RSA_PUBLICKEY == eKeyElementFormat))
                {
                    if (CRYPTO_WA_ENCRYPTED == eKeyElementWriteAccess)
                    {
                        /* Not supported */
                    }
                    else
                    {
                        /* Not supported */
                    }
                }
                break;
#endif /* (STD_ON == CRYPTO_SPT_RSA_KEY_IMPORT) */

            default:
                /* Import symmetric keys, if support for this is enabled */
#if (STD_ON == CRYPTO_SPT_SYM_KEY_IMPORT)
                if ((Crypto_KeyFormatType)CRYPTO_KE_FORMAT_BIN_OCTET == eKeyElementFormat)
                {
                    RetVal = Crypto_Hse_LoadSimKey(u8MuInstance,
                                                   u8MuChannel,
                                                   pHseSrvDescriptor,
                                                   u32KeyLength,
                                                   pKey,
                                                   u32CryptoKeyElementIdx);
                }
#endif /* (STD_ON == CRYPTO_SPT_SYM_KEY_IMPORT) */
                break;
        }
    }
    /* If no channel found - return CRYPTO_E_BUSY */
    else
    {
        RetVal = (Std_ReturnType)CRYPTO_E_BUSY;
    }

    return RetVal;
}

/**
* @brief           Exports a key
* @details         Fills the descriptor for exporting a key
*
* @param[in]       u8MuInstance:             MU instance used to send the request to HSE
* @param[in]       u32KeyMaterialKeyElemIdx: Id of the Key Material Crypto Key Element
* @param[in]       u32KeyExportKeyElemIdx:   Id of the key element where the tag or signature will be stored
* @param[in]       pResult:                  Location where the key will be exported
* @param[inout]    pResultLength:            The length of the exported key
*
* @returns         Std_ReturnType
*                   E_OK:                       Request successful
*                   E_NOT_OK:                   Request unsuccessful
*                   CRYPTO_E_SMALL_BUFFER:      Provided buffer is to small
*                   CRYPTO_E_KEY_READ_FAIL:     Failure in reading the key
*                   CRYPTO_E_KEY_WRITE_FAIL:    Failure in writing the key
*                   CRYPTO_E_KEY_NOT_AVAILABLE: Key not available
*                   CRYPTO_E_KEY_NOT_VALID:     Invalid key
*                   CRYPTO_E_BUSY:              Busy
*/
Std_ReturnType Crypto_Hse_ExportKey
(
    const uint8   u8MuInstance,
    const uint32  u32KeyMaterialKeyElemIdx,
    const uint32  u32KeyExportKeyElemIdx,
#if ((STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT))
    uint8* const  pResult,
#else
    const uint8*  pResult,
#endif
    uint32* const pResultLength
)
{
#if (STD_ON == CRYPTO_SPT_KEY_EXPORT)
    Std_ReturnType       RetVal     = (Std_ReturnType)E_NOT_OK;
    uint8                u8KeyType  = Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].u8HseKeyType;
    Crypto_KeyFormatType eKeyFormat = Crypto_aKeyElementList[u32KeyMaterialKeyElemIdx].eCryptoKeyFormat;
    uint8                u8MuChannel;
    hseSrvDescriptor_t*  pHseSrvDescriptor;

    /* Get a free channel, if possible */
    u8MuChannel = Hse_Ip_GetFreeChannel(u8MuInstance);

    /* If a free channel is found */
    if (HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        pHseSrvDescriptor = &(Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor[u8MuChannel]);

#if (STD_ON == CRYPTO_SPT_SHE_KEY_EXPORT)
        if ((uint8)HSE_KEY_TYPE_SHE == u8KeyType)
        {
            if ((Crypto_KeyFormatType)CRYPTO_KE_FORMAT_BIN_SHEKEYS == eKeyFormat)
            {
                RetVal         = Crypto_Hse_ExportRamSheKey(u8MuInstance, u8MuChannel, pHseSrvDescriptor, pResult);
                *pResultLength = CRYPTO_SHE_SIZE_OUT_U32;
            }
        }
        else
#endif /* (STD_ON == CRYPTO_SPT_SHE_KEY_EXPORT) */
        {
#if ((STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT))
            if ((Crypto_KeyFormatType)CRYPTO_KE_FORMAT_BIN_OCTET == eKeyFormat)
            {
                RetVal = Crypto_Hse_ExportSymPrivAsymPub(u8MuInstance, u8MuChannel, pHseSrvDescriptor, u32KeyMaterialKeyElemIdx, u32KeyExportKeyElemIdx, pResult, pResultLength);
            }
#endif /* ((STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT)) */
        }
    }
    /* If no channel found - return CRYPTO_E_BUSY */
    else
    {
        RetVal = (Std_ReturnType)CRYPTO_E_BUSY;
    }

#if ((STD_OFF == CRYPTO_SPT_SYM_KEY_EXPORT) && (STD_OFF == CRYPTO_SPT_RSA_KEY_EXPORT) && (STD_OFF == CRYPTO_SPT_ECC_KEY_EXPORT))
    /* Ignore compiler warning */
    (void)u32KeyExportKeyElemIdx;
#endif /* ((STD_OFF == CRYPTO_SPT_SYM_KEY_EXPORT) && (STD_OFF == CRYPTO_SPT_RSA_KEY_EXPORT) && (STD_OFF == CRYPTO_SPT_ECC_KEY_EXPORT)) */
#else
    Std_ReturnType RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;
    /* Ignore compiler warnings */
    (void)u8MuInstance;
    (void)u32KeyMaterialKeyElemIdx;
    (void)u32KeyExportKeyElemIdx;
    (void)pResult;
    (void)pResultLength;
#endif /* (STD_ON == CRYPTO_SPT_KEY_EXPORT) */
    return RetVal;
}

/**
* @brief           Copy Key service
* @details         This service can be used to extract keys (or a key) from the derived key material placed
*                  in a temporary shared secret slot (HSE_KEY_TYPE_SHARED_SECRET)
*
* @param[in]       u8MuInstance:              MU instance used to send the request to HSE
* @param[in]       u32CryptoSrcKeyIdx:        Key element index of source key to be copied
* @param[in]       u32TargetCryptoKeyIdx:     Key element index of target key
* @param[in]       u32KeyElementSourceOffset: Key element source offset
* @param[in]       u32KeyElementCopyLength:   Specifies the number of bytes that shall be copied
*
* @returns         Std_ReturnType
*                       E_OK         : Request successful.
*                       E_NOT_OK     : Request unsuccessful.
*                       CRYPTO_E_BUSY: No MU channels available to process the requested job operation.
*/
Std_ReturnType Crypto_Hse_KeyDeriveExtract
(
    const uint8  u8MuInstance,
    const uint32 u32CryptoSrcKeyIdx,
    const uint32 u32TargetCryptoKeyIdx,
    const uint32 u32KeyElementSourceOffset,
    const uint32 u32KeyElementCopyLength
)
{
#if(STD_ON == CRYPTO_SPT_KEY_DERIVE)
    hseKeyDeriveCopyKeySrv_t* pExtractKeySrv;
    hseSrvDescriptor_t*       pHseSrvDescriptor;
    uint8                     u8MuChannel;
    Std_ReturnType            RetVal = (Std_ReturnType)E_NOT_OK;

    /* Get a free channel, if possible */
    u8MuChannel = Hse_Ip_GetFreeChannel(u8MuInstance);

    /* If a free channel is found */
    if (HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyType                   = (hseKeyType_t) Crypto_aKeyElementList[u32TargetCryptoKeyIdx].u8HseKeyType;
        Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyBitLen                 = (uint16)Crypto_GetBitLenFromByteLen(u32KeyElementCopyLength);
        Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.keyFlags                  = (hseKeyFlags_t)Crypto_aKeyElementList[u32TargetCryptoKeyIdx].u16HseKeyFlags;
        Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.smrFlags                  = (hseSmrFlags_t)Crypto_aKeyElementList[u32TargetCryptoKeyIdx].u32HseKeySmrFlags;
#if (STD_ON == CRYPTO_AES_BLOCK_MODE_MASK)
        Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage.specific.aesBlockModeMask = (hseAesBlockModeMask_t)0U; /* Any AES block mode can be used */
#endif /* (STD_ON == CRYPTO_AES_BLOCK_MODE_MASK) */

        pHseSrvDescriptor = &(Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor[u8MuChannel]);
        pExtractKeySrv    = &pHseSrvDescriptor->hseSrv.keyDeriveCopyKeyReq;

        pHseSrvDescriptor->srvId        = HSE_SRV_ID_KEY_DERIVE_COPY;
        pExtractKeySrv->keyHandle       = Crypto_aKeyElementList[u32CryptoSrcKeyIdx].u32HseKeyHandle;
        pExtractKeySrv->startOffset     = (uint16)u32KeyElementSourceOffset;
        pExtractKeySrv->targetKeyHandle = Crypto_aKeyElementList[u32TargetCryptoKeyIdx].u32HseKeyHandle;
        pExtractKeySrv->keyInfo         = Crypto_Hse_apMuState[u8MuInstance]->HseKeyInfoStorage;

        RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR);
    }
    else
    {
        /* No free MU channel was found */
        RetVal = (Std_ReturnType)CRYPTO_E_BUSY;
    }
    /* Ignore compiler warnings */
    (void)u32KeyElementCopyLength;
    (void)u32KeyElementSourceOffset;
#else
    Std_ReturnType RetVal = CRYPTO_RET_NOT_SUPPORTED;
    /* Ignore compiler warnings */
    (void)u8MuInstance;
    (void)u32CryptoSrcKeyIdx;
    (void)u32TargetCryptoKeyIdx;
    (void)u32KeyElementSourceOffset;
    (void)u32KeyElementCopyLength;
#endif /* (STD_ON == CRYPTO_SPT_KEY_DERIVE) */

    return RetVal;
}

/**
* @brief           Random seed
* @details         Random seed
*
* @param[in]       u8MuInstance:    MU instance used to send the request to HSE
* @param[in]       u32CryptoKeyIdx: If of the Crypto Key to be used for Random Seed
* @param[in]       pu8SeedPtr:      Pointer to the seed
* @param[in]       u32SeedLength:   Length of the seed
*
* @returns         Std_ReturnType
*                       E_OK:     Correct configuration of parameters
*                       E_NOT_OK: Incorrect configuration of parameters: keyId not existent, wrong length
*/
Std_ReturnType Crypto_Hse_RandomSeed
(
    const uint8        u8MuInstance,
    const uint32       u32CryptoKeyIdx,
    const uint8* const pu8SeedPtr,
    const uint32       u32SeedLength
)
{
    /* Ignore compiler warnings */
    (void)u8MuInstance;
    (void)u32CryptoKeyIdx;
    (void)pu8SeedPtr;
    (void)u32SeedLength;

    return (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;
}

/**
* @brief           Generate key
* @details         Generate random key, RSA pair, ECC pair
*
* @param[in]       u8MuInstance:   MU instance used to send the request to HSE
* @param[in]       u32CryptoKeyId: If of the Crypto Key to be used for Key Generation
*
* @returns         Std_ReturnType
*                       E_OK         : Correct configuration of parameters.
*                       E_NOT_OK     : Incorrect configuration of parameters: keyId not existent, wrong length.
*                       CRYPTO_E_BUSY: No MU channels available to process the requested job operation.
*/
Std_ReturnType Crypto_Hse_KeyGenerate
(
    const uint8  u8MuInstance,
    const uint32 u32CryptoKeyIdx
)
{
#if (STD_ON == CRYPTO_SPT_KEY_GENERATE)
    uint8               u8MuChannel;
    hseSrvDescriptor_t* pHseSrvDescriptor;
    Std_ReturnType      RetVal = (Std_ReturnType)E_NOT_OK;
    boolean             bReqSentOverChannel = FALSE;

    /* Check if a free MU channel can be found */
    u8MuChannel = Hse_Ip_GetFreeChannel(u8MuInstance);
    /* If a free channel is found */
    if (HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        pHseSrvDescriptor = &(Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor[u8MuChannel]);

        RetVal = Crypto_Hse_KeyGenerateRequest(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR, u32CryptoKeyIdx, &bReqSentOverChannel);
        /* Free the allocated channel in case the request was not sent to firmware */
        if (FALSE == bReqSentOverChannel)
        {
            Hse_Ip_ReleaseChannel(u8MuInstance, u8MuChannel);
        }
    }
    else
    {
        /* No free MU channel was found */
        RetVal = (Std_ReturnType)CRYPTO_E_BUSY;
    }
#else
    Std_ReturnType RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;
    /* Ignore compiler warnings */
    (void)u8MuInstance;
    (void)u32CryptoKeyIdx;
#endif /* (STD_ON == CRYPTO_SPT_KEY_GENERATE) */
    return RetVal;
}

/**
* @brief         Tries to allocate a free MU channel for initiating the KeyDerive operation in HSE
* @details       Tries to allocate a free MU channel for initiating the KeyDerive operation in HSE
*
* @param[in]     u8MuInstance:    MU instance used to send the request to HSE
* @param[in]     u32CryptoKeyIdx: Index of the Crypto Key
* @param[in]     u32TargetKeyIdx: Index of the Target Crypto Key
*
* @returns         Std_ReturnType
*                   E_OK         : Correct configuration of parameters.
*                   E_NOT_OK     : Incorrect configuration of parameters: incorrect lengths.
*                   CRYPTO_E_BUSY: No MU channels available to process the requested job operation.
*/
Std_ReturnType Crypto_Hse_KeyDerive
(
    const uint8  u8MuInstance,
    const uint32 u32CryptoKeyIdx,
    const uint32 u32TargetKeyIdx
)
{
#if (STD_ON == CRYPTO_SPT_KEY_DERIVE)
    uint8               u8MuChannel;
    hseSrvDescriptor_t* pHseSrvDescriptor;
    Std_ReturnType      RetVal = (Std_ReturnType)E_NOT_OK;
    boolean             bReqSentOverChannel = FALSE;

    /* Check if a free MU channel can be found */
    u8MuChannel = Hse_Ip_GetFreeChannel(u8MuInstance);
    /* If a free channel is found */
    if (HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        pHseSrvDescriptor = &(Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor[u8MuChannel]);

        RetVal = Crypto_Hse_KeyDeriveRequest(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR, u32CryptoKeyIdx, u32TargetKeyIdx, &bReqSentOverChannel);
        /* Free the allocated channel in case the request was not sent to firmware */
        if (FALSE == bReqSentOverChannel)
        {
            Hse_Ip_ReleaseChannel(u8MuInstance, u8MuChannel);
        }
    }
    else
    {
        /* No free MU channel was found */
        RetVal = (Std_ReturnType)CRYPTO_E_BUSY;
    }
#else
    Std_ReturnType RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;
    /* Ignore compiler warnings */
    (void)u8MuInstance;
    (void)u32CryptoKeyIdx;
    (void)u32TargetKeyIdx;
#endif /* (STD_ON == CRYPTO_SPT_KEY_DERIVE) */

    return RetVal;
}

/**
* @brief           Computes Shared Secret
* @details         Calculates the shared secret key for the key exchange with the key material of the key
*                  identified by the cryptoKeyId and the partner public key. The shared secret key is
*                  stored as a key element in the same key.
*
* @param[in]       u8MuInstance:                MU instance used to send the request to HSE
* @param[in]       u32CryptoKeyIdx:             Index of the key
* @param[in]       pPartnerPublicValue:         Pointer to partner public key data
* @param[in]       u32PartnerPublicValueLength: Length of partner public key
*
* @returns      Std_ReturnType -
*                  E_OK         : Correct configuration of parameters.
*                  E_NOT_OK     : Incorrect configuration of parameters: keyId not existent, wrong length.
*                  CRYPTO_E_BUSY: No MU channels available to process the requested job operation.
*/
Std_ReturnType Crypto_Hse_CalcSecret
(
    const uint8        u8MuInstance,
    const uint32       u32CryptoKeyIdx,
    const uint8* const pPartnerPublicValue,
    const uint32       u32PartnerPublicValueLength
)
{
#if (((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC)) && (STD_ON == CRYPTO_SPT_COMPUTE_DH))
    uint8               u8MuChannel;
    hseSrvDescriptor_t* pHseSrvDescriptor;
    Std_ReturnType      RetVal = (Std_ReturnType)E_NOT_OK;

    /* Check if a free MU channel can be found */
    u8MuChannel = Hse_Ip_GetFreeChannel(u8MuInstance);
    /* If a free channel is found */
    if (HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        pHseSrvDescriptor = &(Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor[u8MuChannel]);

        RetVal = Crypto_Hse_CalcSecretRequest(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR, u32CryptoKeyIdx, pPartnerPublicValue, u32PartnerPublicValueLength);
    }
    else
    {
        /* No free MU channel was found */
        RetVal = (Std_ReturnType)CRYPTO_E_BUSY;
    }
#else
    Std_ReturnType RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;
    /* Ignore compiler warnings */
    (void)u8MuInstance;
    (void)u32CryptoKeyIdx;
    (void)pPartnerPublicValue;
    (void)u32PartnerPublicValueLength;
#endif /* (((STD_ON == CRYPTO_SPT_RSA) || (STD_ON == CRYPTO_SPT_ECC)) && (STD_ON == CRYPTO_SPT_COMPUTE_DH)) */

    return RetVal;
}

/**
* @brief           Parses a certificate
* @details         In the current implementation, this function is not supported
*
* @param[in]       u8MuInstance:                MU instance used to send the request to HSE
* @param[in]       u32CryptoKeyIdx:             Index of the key
*
* @returns      Std_ReturnType -
*                  CRYPTO_E_NOT_SUPPORTED:      Functionality not yet supported
*/
Std_ReturnType Crypto_Hse_CertificateParse
(
    const uint8  u8MuInstance,
    const uint32 u32CryptoKeyIdx
)
{
    /* Avoid compiler warnings */
    (void)u8MuInstance;
    (void)u32CryptoKeyIdx;

    return (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;
}

/**
* @brief           Verifies a certificate
* @details         In the current implementation, this function is not supported
*
* @param[in]       u8MuInstance:                MU instance used to send the request to HSE
* @param[in]       u32CryptoKeyIdx:             Index of the key
* @param[in]       u32VerifyCryptoKeyIdx:       Index of the key to verify
* @param[in]       verifyPtr:                   Pointer to the memory location which will contain the result of the certificate verification
*
* @returns      Std_ReturnType -
*                  CRYPTO_E_NOT_SUPPORTED:      Functionality not yet supported
*/
Std_ReturnType Crypto_Hse_CertificateVerify
(
    const uint8                          u8MuInstance,
    const uint32                         u32CryptoKeyIdx,
    const uint32                         u32VerifyCryptoKeyIdx,
    const Crypto_VerifyResultType* const verifyPtr
)
{
    /* Avoid compiler warnings */
    (void)u8MuInstance;
    (void)u32CryptoKeyIdx;
    (void)u32VerifyCryptoKeyIdx;
    (void)verifyPtr;

    return (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;
}

#if (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)
/**
* @brief           Function that tries to restore information about the valid flag of the Crypto keys from Nvram
* @details         Function that tries to restore information about the valid flag of the Crypto keys from Nvram
*
* @param[in]       u8MuInstance:  MU instance used to send the request to HSE
*
* @returns         Std_ReturnType
*                  E_OK         : The operation was executed successfully.
*                  E_NOT_OK     : The operation failed.
*                  CRYPTO_E_BUSY: No MU channels available to process the requested job operation.
*/
Std_ReturnType Crypto_Hse_RestoreKeyValidInfoFromNvram
(
    const uint8 u8MuInstance
)
{
    Std_ReturnType      RetVal = (Std_ReturnType)E_OK;
#if(STD_ON == CRYPTO_SPT_GETKEYINFO)
    uint32              u32KeyCounter;
    uint32              u32KeyElemCounter;
    uint32              u32HseKeyHandle;
    uint8               u8MuChannel;
    hseKeyInfo_t        HseKeyInfoFromNvram;
    hseSrvDescriptor_t* pHseSrvDescriptor;
    boolean             bValidKeyBlobNeedsUpdate = FALSE;

    /* Loop through all the keys that are marked as valid from the just restored blob */
    for (u32KeyCounter = 0; u32KeyCounter < CRYPTO_NUMBER_OF_KEYS_U32; u32KeyCounter++)
    {
        if (Crypto_IsKeyValidFlagSet(u32KeyCounter))
        {
            /* We assume here that a key has been marked valid by the upper layer only after all its key elements that use a HSE key have been imported in HSE
               Therefore, we will check here that all key elements of the currently looped key that use a HSE key are residing in non-empty HSE key slots */
            for (u32KeyElemCounter = 0; u32KeyElemCounter < Crypto_aKeyList[u32KeyCounter].u32NumCryptoKeyElements; u32KeyElemCounter++)
            {
                u32HseKeyHandle = Crypto_aKeyElementList[Crypto_aKeyList[u32KeyCounter].pCryptoKeyElementList[u32KeyElemCounter]].u32HseKeyHandle;
                if ((uint32)HSE_INVALID_KEY_HANDLE != u32HseKeyHandle)
                {
                    /* Get a free channel, if possible */
                    u8MuChannel = Hse_Ip_GetFreeChannel(u8MuInstance);
                    if (HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
                    {
                        pHseSrvDescriptor = &(Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor[u8MuChannel]);
                        /* Fill the HSE descriptor with a request to get key info */
                        pHseSrvDescriptor->srvId                          = HSE_SRV_ID_GET_KEY_INFO;
                        pHseSrvDescriptor->hseSrv.getKeyInfoReq.keyHandle = u32HseKeyHandle;
                        pHseSrvDescriptor->hseSrv.getKeyInfoReq.pKeyInfo  = HSE_PTR_TO_HOST_ADDR(&HseKeyInfoFromNvram);
                        /* Send the request to HSE, synchronously */
                        if ((Std_ReturnType)E_OK != Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR))
                        {
                            /* We have found at least one key element in the current looped valid key which is using HSE, but the HSE slot information is not ok. Mark the key as invalid. */
                            Crypto_ClearKeyValidFlag(u32KeyCounter);
                            /* We also need to update the key valid blob into Nvram */
                            bValidKeyBlobNeedsUpdate = TRUE;
                            /* No need to loop over the other key elements of the current key, pass to next key */
                            break;
                        }
                    }
                    else
                    {
                        /* No free MU channel was found */
                        RetVal = (Std_ReturnType)CRYPTO_E_BUSY;
                    }
                }
            }
        }
        /* If the blob containing valid key info has been updated (some keys has been marked as invalid), request upper layer to store it in Nvram */
        if (TRUE == bValidKeyBlobNeedsUpdate)
        {
            /* Notify the upper layer that the information in the KeyValid blob has been updated */
            RetVal = pfCryptoUpdateNvramBlobHandler(CRYPTO_NVRAM_BLOB_0_ID, CRYPTO_SIZEOF_NVRAM_BLOB_0);
        }
    }
#else
    /* Ignore compiler warning */
    (void)u8MuInstance;
#endif /* (STD_ON == CRYPTO_SPT_GETKEYINFO) */

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT) */

#if (STD_ON == CRYPTO_ENABLE_FEED_HSE_DESC_SUPPORT)
/**
* @brief           Function that performs a request to HSE based on the received descriptor parameter
* @details         Function that always performs a synchronous request to HSE based on the received descriptor parameter
*
* @param[in]       u8MuInstance:    MU instance used to send the request to HSE
* @param[in]       pu8ResultPtr:       Pointer to the descriptor to be sent to HSE
* @param[in]       pu32ResultLengthPtr: Pointer to location where the value of the HSE response will be stored
*
* @returns         Std_ReturnType
*/
Std_ReturnType Crypto_Hse_FeedHseDescriptor
(
    const uint8 u8MuInstance,
    uint8*      pu8ResultPtr,
    uint32*     pu32ResultLengthPtr
)
{
    uint8            u8MuChannel;
    hseSrvResponse_t HseResponse;
    Hse_Ip_ReqType*  pHseIpReq;
    Std_ReturnType   RetVal = (Std_ReturnType)E_NOT_OK;;

    /* Get a free channel, if possible */
    u8MuChannel = Hse_Ip_GetFreeChannel(u8MuInstance);

    /* If a free channel is found */
    if (HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        pHseIpReq = &Crypto_Hse_apMuState[u8MuInstance]->Hse_Ip_aRequest[u8MuChannel];

        pHseIpReq->eReqType   = HSE_IP_REQTYPE_SYNC;
        pHseIpReq->u32Timeout = Crypto_Hse_apMuState[u8MuInstance]->u32HseSyncRequestsTimeout;

        /* Send the request to Hse Ip layer */
        HseResponse = Hse_Ip_ServiceRequest(u8MuInstance, u8MuChannel, pHseIpReq, (hseSrvDescriptor_t*)pu8ResultPtr);
        /* Translate the message received from HSE to a Std_ReturnType value */
        RetVal = Crypto_Hse_TranslateHseResponse(HseResponse);
        /* Update the content of the pu32ResultLengthPtr with the response received from HSE */
        *pu32ResultLengthPtr = (uint32)HseResponse;
    }
    else
    {
        /* No free MU channel was found */
        RetVal = (Std_ReturnType)CRYPTO_E_BUSY;
    }

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_ENABLE_FEED_HSE_DESC_SUPPORT) */
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

/**
* @brief           Main function
* @details         Main function - check for async jobs, then send jobs from the queue
*
* @param[in]       u8MuInstance:  MU instance used to send the request to HSE
*
* @returns         void
*/
void Crypto_Hse_MainFunction
(
    const uint8 u8MuInstance
)
{
    /* Call the main function of the HseIp layer. This is responsible for calling back the async jobs callbacks */
    Hse_Ip_MainFunction(u8MuInstance);
}

/**
* @brief           Format the Key Catalogs
* @details         HSE firmware formatting key catalogs
*
* @param[in]       u8MuInstance:  MU instance used to send the request to HSE
*
* @returns         Std_ReturnType
*/
Std_ReturnType Crypto_Hse_FormatKeyCatalogs
(
    const uint8 u8MuInstance
)
{
#if(STD_ON == CRYPTO_SPT_FORMATKEYCATALOGS)
    hseFormatKeyCatalogsSrv_t* pFormatKeyCatalogsReq;
    hseSrvDescriptor_t*        pHseSrvDescriptor;
    uint8                      u8MuChannel;
    Std_ReturnType             RetVal = (Std_ReturnType)E_NOT_OK;

    /* Get a free channel, if possible */
    u8MuChannel = Hse_Ip_GetFreeChannel(u8MuInstance);

    /* If a free channel is found */
    if (HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        pHseSrvDescriptor = &(Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor[u8MuChannel]);
        pFormatKeyCatalogsReq = &pHseSrvDescriptor->hseSrv.formatKeyCatalogsReq;

        pHseSrvDescriptor->srvId = HSE_SRV_ID_FORMAT_KEY_CATALOGS;
        pFormatKeyCatalogsReq->pNvmKeyCatalogCfg = HSE_PTR_TO_HOST_ADDR(aHseNvmKeyCatalog);
        pFormatKeyCatalogsReq->pRamKeyCatalogCfg = HSE_PTR_TO_HOST_ADDR(aHseRamKeyCatalog);

        RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR);
    }
#else
    Std_ReturnType RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;

    /* Ignore compiler warning */
    (void)u8MuInstance;
#endif /* (STD_ON == CRYPTO_SPT_FORMATKEYCATALOGS) */

    return RetVal;
}

/**
* @brief           Sets the timeout for synhronous requests send to HSE
* @details         Sets the timeout for synhronous requests send to HSE
*
* @param[in]       u8MuInstance:  MU instance used to send the request to HSE
* @param[in]       u32Timeout:    Value of the timeout
*
* @returns         Std_ReturnType
*/
void Crypto_Hse_SetSynchronousRequestsTimeout
(
    const uint8  u8MuInstance,
    const uint32 u32Timeout
)
{
    Crypto_Hse_apMuState[u8MuInstance]->u32HseSyncRequestsTimeout = u32Timeout;
}

/**
* @brief           Sets SHE boot status to ok or failed
* @details         Sets SHE boot status to ok or failed
*
* @param[in]       u8MuInstance: MU instance used to send the request to HSE
* @param[in]       bStatus:      Boolean flag defining if status should be ok (when TRUE) or failed (when FALSE)
*
* @returns         Std_ReturnType
*/
Std_ReturnType Crypto_Hse_SheSetBootStatus
(
    const uint8   u8MuInstance,
    const boolean bStatus
)
{
#if (STD_ON == CRYPTO_SPT_SHE)
    hseSrvDescriptor_t*        pHseSrvDescriptor;
    uint8                      u8MuChannel;
    Std_ReturnType             RetVal = (Std_ReturnType)E_NOT_OK;

    /* Get a free channel, if possible */
    u8MuChannel = Hse_Ip_GetFreeChannel(u8MuInstance);

    /* If a free channel is found */
    if (HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        pHseSrvDescriptor = &(Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor[u8MuChannel]);

        if (TRUE == bStatus)
        {
            pHseSrvDescriptor->srvId = HSE_SRV_ID_SHE_BOOT_OK;
        }
        else
        {
            pHseSrvDescriptor->srvId = HSE_SRV_ID_SHE_BOOT_FAILURE;
        }

        RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR);
    }
#else
    Std_ReturnType RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;

    /* Ignore compiler warning */
    (void)u8MuInstance;
    (void)bStatus;
#endif /* (STD_ON == CRYPTO_SPT_SHE) */

    return RetVal;
}


/**
* @brief           Gets SHE status
* @details         Gets SHE status
*
* @param[in]       u8MuInstance: MU instance used to send the request to HSE
* @param[out]      pStatus:      Pointer to one byte location provide by app where Crypto will store the status of HSE
*
* @returns         Std_ReturnType
*/
Std_ReturnType Crypto_Hse_SheGetStatus
(
    const uint8  u8MuInstance,
    uint8*       pStatus
)
{
#if (STD_ON == CRYPTO_SPT_SHE)
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;

    *pStatus = (uint8)Hse_Ip_GetHseStatus(u8MuInstance);
#else
    Std_ReturnType RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;

    /* Ignore compiler warning */
    (void)u8MuInstance;
    (void)pStatus;
#endif /* (STD_ON == CRYPTO_SPT_SHE) */

    return RetVal;
}


/**
* @brief           Gets SHE ID
* @details         The function returns the identity (UID) and the value of the status register protected by a MAC over a challenge and the data
*
* @param[in]       u8MuInstance: MU instance used to send the request to HSE
* @param[in]       pChallenge:   Pointer to a 128-bit buffer where from the challenge will be taken
* @param[out]      pId:          Pointer to a 128-bit buffer where UID will be stored
* @param[out]      pSreg:        Pointer to a 8-bit buffer where status register will be stored
* @param[out]      pMac:         Pointer to a 128-bit buffer where MAC key will be stored
*
* @returns         Std_ReturnType
*/
Std_ReturnType Crypto_Hse_SheGetId
(
    const uint8  u8MuInstance,
    const uint8* pChallenge,
    const uint8* pId,
    const uint8* pSreg,
    const uint8* pMac
)
{
#if (STD_ON == CRYPTO_SPT_SHE)
    hseSheGetIdSrv_t*   pSheGetIdReq;
    hseSrvDescriptor_t* pHseSrvDescriptor;
    uint8               u8MuChannel;
    Std_ReturnType      RetVal = (Std_ReturnType)E_NOT_OK;

    /* Get a free channel, if possible */
    u8MuChannel = Hse_Ip_GetFreeChannel(u8MuInstance);

    /* If a free channel is found */
    if (HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        pHseSrvDescriptor = &(Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor[u8MuChannel]);
        pSheGetIdReq      = &(pHseSrvDescriptor->hseSrv.sheGetIdReq);

        pHseSrvDescriptor->srvId = HSE_SRV_ID_SHE_GET_ID;

        pSheGetIdReq->pChallenge = HSE_PTR_TO_HOST_ADDR(pChallenge);
        pSheGetIdReq->pId        = HSE_PTR_TO_HOST_ADDR(pId);
        pSheGetIdReq->pSreg      = HSE_PTR_TO_HOST_ADDR(pSreg);
        pSheGetIdReq->pMac       = HSE_PTR_TO_HOST_ADDR(pMac);

        RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR);
    }
#else
    Std_ReturnType RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;

    /* Ignore compiler warning */
    (void)u8MuInstance;
    (void)pChallenge;
    (void)pId;
    (void)pSreg;
    (void)pMac;
#endif /* (STD_ON == CRYPTO_SPT_SHE) */

    return RetVal;
}

/**
* @brief           Initiates SHE debug challenge
* @details         Initiates SHE debug challenge
*
* @param[in]       u8MuInstance: MU instance used to send the request to HSE
* @param[out]      pChallenge:   Pointer to a 128-bit buffer where the challenge will be stored
*
* @returns         Std_ReturnType
*/
Std_ReturnType Crypto_Hse_SheDebugChal
(
    const uint8  u8MuInstance,
    const uint8* pChallenge
)
{
#if (STD_ON == CRYPTO_SPT_SHE)
    hseSysAuthorizationReqSrv_t* pSysAuthReq;
    hseSrvDescriptor_t*          pHseSrvDescriptor;
    uint8                        u8MuChannel;
    Std_ReturnType               RetVal = (Std_ReturnType)E_NOT_OK;

    /* Get a free channel, if possible */
    u8MuChannel = Hse_Ip_GetFreeChannel(u8MuInstance);

    /* If a free channel is found */
    if (HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        pHseSrvDescriptor = &(Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor[u8MuChannel]);
        pSysAuthReq       = &(pHseSrvDescriptor->hseSrv.sysAuthorizationReq);

        pHseSrvDescriptor->srvId = HSE_SRV_ID_SYS_AUTH_REQ;

        pSysAuthReq->sysAuthOption  = HSE_SYS_AUTH_KEY_MGMT;
        pSysAuthReq->sysRights      = HSE_RIGHTS_SUPER_USER;
        pSysAuthReq->ownerKeyHandle = CRYPTO_SHE_MASTER_ECU_KEY_HANDLE_U32;
        pSysAuthReq->pChallenge     = HSE_PTR_TO_HOST_ADDR(pChallenge);
        pSysAuthReq->authScheme.macScheme.macAlgo             = HSE_MAC_ALGO_CMAC;
        pSysAuthReq->authScheme.macScheme.sch.cmac.cipherAlgo = HSE_CIPHER_ALGO_AES;

        RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR);
    }
#else
    Std_ReturnType RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;

    /* Ignore compiler warning */
    (void)u8MuInstance;
    (void)pChallenge;
#endif /* (STD_ON == CRYPTO_SPT_SHE) */

    return RetVal;
}


/**
* @brief           Initiates SHE debug authorization
* @details         Initiates SHE debug authorization
*
* @param[in]       u8MuInstance:   MU instance used to send the request to HSE
* @param[in]       pAuthorization: Pointer to a 128-bit buffer where from the authorization value will be taken
*
* @returns         Std_ReturnType
*/
Std_ReturnType Crypto_Hse_SheDebugAuth
(
    const uint8  u8MuInstance,
    const uint8* pAuthorization
)
{
#if (STD_ON == CRYPTO_SPT_SHE)
    hseSysAuthorizationRespSrv_t* pSysAuthResp;
    hseEraseKeySrv_t*             pEraseKeyReq;
    hseSrvDescriptor_t*           pHseSrvDescriptor;
    uint8                         u8MuChannel;
    Std_ReturnType                RetVal = (Std_ReturnType)E_NOT_OK;

    /* Get a free channel, if possible */
    u8MuChannel = Hse_Ip_GetFreeChannel(u8MuInstance);

    /* If a free channel is found */
    if (HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        pHseSrvDescriptor = &(Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor[u8MuChannel]);
        pSysAuthResp      = &(pHseSrvDescriptor->hseSrv.sysAuthorizationResp);

        pHseSrvDescriptor->srvId = HSE_SRV_ID_SYS_AUTH_RESP;

        pSysAuthResp->authLen[0] = 16U;
        pSysAuthResp->authLen[1] = 0U;
        pSysAuthResp->pAuth[0]   = HSE_PTR_TO_HOST_ADDR(pAuthorization);
        pSysAuthResp->pAuth[1]   = (HOST_ADDR)0U;

        RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR);

        if ((Std_ReturnType)E_OK == RetVal)
        {
            /* Get a free channel, if possible */
            u8MuChannel = Hse_Ip_GetFreeChannel(u8MuInstance);

            /* If a free channel is found */
            if (HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
            {
                pHseSrvDescriptor = &(Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor[u8MuChannel]);
                pEraseKeyReq      = &(pHseSrvDescriptor->hseSrv.eraseKeyReq);

                pHseSrvDescriptor->srvId = HSE_SRV_ID_ERASE_KEY;

                pEraseKeyReq->keyHandle       = HSE_INVALID_KEY_HANDLE;
                pEraseKeyReq->eraseKeyOptions = HSE_ERASE_ALL_NVM_SYM_KEYS_ON_MU_IF;

                RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR);
            }
        }
    }
#else
    Std_ReturnType RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;

    /* Ignore compiler warning */
    (void)u8MuInstance;
    (void)pAuthorization;
#endif /* (STD_ON == CRYPTO_SPT_SHE) */

    return RetVal;
}


/**
* @brief           Miyaguchi-Preneel compression function
* @details         Gets SHE status
*
* @param[in]       u8MuInstance: MU instance used to send the request to HSE
* @param[in]       pInput:       Pointer to input data
* @param[in]       u32InputLen:  Input data length
* @param[out]      pResult:      Pointer to result data
* @param[inout]    pResultLen:   Pointer to result data length
*
* @returns         Std_ReturnType
*/
Std_ReturnType Crypto_Hse_SheMPCompression
(
    const uint8   u8MuInstance,
    const uint8*  pInput,
    const uint32  u32InputLen,
    const uint8*  pResult,
    const uint32* pResultLen
)
{
#if (STD_ON == CRYPTO_SPT_MP)
    hseHashSrv_t*        pHashSrv;
    hseSrvDescriptor_t*  pHseSrvDescriptor;
    uint8                u8MuChannel;
    Std_ReturnType       RetVal = (Std_ReturnType)E_NOT_OK;

    /* Get a free channel, if possible */
    u8MuChannel = Hse_Ip_GetFreeChannel(u8MuInstance);

    /* If a free channel is found */
    if (HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        pHseSrvDescriptor = &(Crypto_Hse_apMuState[u8MuInstance]->pHseSrvDescriptor[u8MuChannel]);
        pHashSrv          = &(pHseSrvDescriptor->hseSrv.hashReq);

        pHseSrvDescriptor->srvId = HSE_SRV_ID_HASH;

        pHashSrv->accessMode  = HSE_ACCESS_MODE_ONE_PASS;
        pHashSrv->hashAlgo    = HSE_HASH_ALGO_MP;
        pHashSrv->inputLength = u32InputLen;
        pHashSrv->pInput      = HSE_PTR_TO_HOST_ADDR(pInput);
        pHashSrv->pHash       = HSE_PTR_TO_HOST_ADDR(pResult);
        pHashSrv->pHashLength = HSE_PTR_TO_HOST_ADDR(pResultLen);

        RetVal = Crypto_Hse_SendMsg(u8MuInstance, u8MuChannel, pHseSrvDescriptor, NULL_PTR);
    }
#else
    Std_ReturnType RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;

    /* Ignore compiler warning */
    (void)u8MuInstance;
    (void)pInput;
    (void)u32InputLen;
    (void)pResult;
    (void)pResultLen;
#endif /* (STD_ON == CRYPTO_SPT_MP) */

    return RetVal;
}


#define CRYPTO_STOP_SEC_CODE
#include "Crypto_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */


