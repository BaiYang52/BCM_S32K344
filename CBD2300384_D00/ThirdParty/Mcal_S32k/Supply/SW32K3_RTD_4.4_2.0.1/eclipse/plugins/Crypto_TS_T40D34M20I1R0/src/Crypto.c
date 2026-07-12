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
#include "Crypto_Types.h"
#include "Det.h"
#include "Crypto_Ipw.h"
#include "Crypto_Private.h"
#include "Crypto_Util.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CRYPTO_VENDOR_ID_C                     43
#define CRYPTO_AR_RELEASE_MAJOR_VERSION_C      4
#define CRYPTO_AR_RELEASE_MINOR_VERSION_C      4
#define CRYPTO_AR_RELEASE_REVISION_VERSION_C   0
#define CRYPTO_SW_MAJOR_VERSION_C              2
#define CRYPTO_SW_MINOR_VERSION_C              0
#define CRYPTO_SW_PATCH_VERSION_C              1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if Crypto source file and Crypto header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_C != CRYPTO_VENDOR_ID)
    #error "Crypto.c and Crypto.h have different vendor ids"
#endif

/* Check if Crypto source file and Crypto header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_C    != CRYPTO_AR_RELEASE_MAJOR_VERSION) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_C    != CRYPTO_AR_RELEASE_MINOR_VERSION) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_C != CRYPTO_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Crypto.c and Crypto.h are different"
#endif

/* Check if Crypto source file and Crypto header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_C != CRYPTO_SW_MAJOR_VERSION) || \
     (CRYPTO_SW_MINOR_VERSION_C != CRYPTO_SW_MINOR_VERSION) || \
     (CRYPTO_SW_PATCH_VERSION_C != CRYPTO_SW_PATCH_VERSION)    \
    )
    #error "Software Version Numbers of Crypto.c and Crypto.h are different"
#endif

/* Check if Crypto source file and Crypto types header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_C != CRYPTO_VENDOR_ID_TYPES)
    #error "Crypto.c and Crypto_Types.h have different vendor ids"
#endif

/* Check if Crypto source file and Crypto types header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_C    != CRYPTO_AR_RELEASE_MAJOR_VERSION_TYPES) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_C    != CRYPTO_AR_RELEASE_MINOR_VERSION_TYPES) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_C != CRYPTO_AR_RELEASE_REVISION_VERSION_TYPES) \
    )
    #error "AutoSar Version Numbers of Crypto.c and Crypto_Types.h are different"
#endif

/* Check if Crypto source file and Crypto types header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_C != CRYPTO_SW_MAJOR_VERSION_TYPES) || \
     (CRYPTO_SW_MINOR_VERSION_C != CRYPTO_SW_MINOR_VERSION_TYPES) || \
     (CRYPTO_SW_PATCH_VERSION_C != CRYPTO_SW_PATCH_VERSION_TYPES)    \
    )
    #error "Software Version Numbers of Crypto.c and Crypto_Types.h are different"
#endif

/* Check if Crypto source file and Crypto Ipw header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_C != CRYPTO_VENDOR_ID_IPW_H)
    #error "Crypto.c and Crypto_Ipw.h have different vendor ids"
#endif

/* Check if Crypto source file and Crypto Ipw header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_C    != CRYPTO_AR_RELEASE_MAJOR_VERSION_IPW_H) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_C    != CRYPTO_AR_RELEASE_MINOR_VERSION_IPW_H) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_C != CRYPTO_AR_RELEASE_REVISION_VERSION_IPW_H) \
    )
    #error "AutoSar Version Numbers of Crypto.c and Crypto_Ipw.h are different"
#endif

/* Check if Crypto source file and Crypto Ipw header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_C != CRYPTO_SW_MAJOR_VERSION_IPW_H) || \
     (CRYPTO_SW_MINOR_VERSION_C != CRYPTO_SW_MINOR_VERSION_IPW_H) || \
     (CRYPTO_SW_PATCH_VERSION_C != CRYPTO_SW_PATCH_VERSION_IPW_H)    \
    )
    #error "Software Version Numbers of Crypto.c and Crypto_Ipw.h are different"
#endif

/* Check if Crypto source file and Crypto private header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_C != CRYPTO_VENDOR_ID_PRIVATE)
    #error "Crypto.c and Crypto_Private.h have different vendor ids"
#endif

/* Check if Crypto source file and Crypto private header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_C    != CRYPTO_AR_RELEASE_MAJOR_VERSION_PRIVATE) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_C    != CRYPTO_AR_RELEASE_MINOR_VERSION_PRIVATE) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_C != CRYPTO_AR_RELEASE_REVISION_VERSION_PRIVATE) \
    )
    #error "AutoSar Version Numbers of Crypto.c and Crypto_Private.h are different"
#endif

/* Check if Crypto source file and Crypto private header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_C != CRYPTO_SW_MAJOR_VERSION_PRIVATE) || \
     (CRYPTO_SW_MINOR_VERSION_C != CRYPTO_SW_MINOR_VERSION_PRIVATE) || \
     (CRYPTO_SW_PATCH_VERSION_C != CRYPTO_SW_PATCH_VERSION_PRIVATE)    \
    )
    #error "Software Version Numbers of Crypto.c and Crypto_Private.h are different"
#endif

/* Check if Crypto source file and Crypto_Util header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_C != CRYPTO_UTIL_VENDOR_ID)
    #error "Crypto.c and Crypto_Util.h have different vendor ids"
#endif

/* Check if Crypto source file and Crypto_Util header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_C    != CRYPTO_UTIL_AR_RELEASE_MAJOR_VERSION) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_C    != CRYPTO_UTIL_AR_RELEASE_MINOR_VERSION) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_C != CRYPTO_UTIL_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Crypto.c and Crypto_Util.h are different"
#endif

/* Check if Crypto source file and Crypto_Util header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_C != CRYPTO_UTIL_SW_MAJOR_VERSION) || \
     (CRYPTO_SW_MINOR_VERSION_C != CRYPTO_UTIL_SW_MINOR_VERSION) || \
     (CRYPTO_SW_PATCH_VERSION_C != CRYPTO_UTIL_SW_PATCH_VERSION)    \
    )
    #error "Software Version Numbers of Crypto.c and Crypto_Util.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if Crypto.c file and Det header file are of the same AutoSar version */
    #if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_C != DET_AR_RELEASE_MAJOR_VERSION) || \
         (CRYPTO_AR_RELEASE_MINOR_VERSION_C != DET_AR_RELEASE_MINOR_VERSION)    \
        )
        #error "AutoSar Version Numbers of Crypto.c and Det.h are different"
    #endif
#endif

/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
/**
* @brief Enum containing the possible return values for function Crypto_GetProcessJobError()
*/
typedef enum
{
    CRYPTO_NO_DET_RET_OK  = 0x00U,
    CRYPTO_NO_DET_RET_NOK,
    CRYPTO_NO_DET_RET_KEY_NOT_VALID,
    CRYPTO_NO_DET_RET_NOT_SUPPORTED,
    CRYPTO_DET_NULL_PTR,
    CRYPTO_DET_INVALID_LEN,
    CRYPTO_DET_INVALID_PARAM
} Crypto_ProcessJobErrorType;

/**
* @brief Enum containing the possible operations that can be requested to the Crypto_HandleNvramInfo function
*/
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT))
typedef enum
{
    CRYPTO_NVRAM_OP_READ  = 0x00U,
    CRYPTO_NVRAM_OP_WRITE = 0x01U
} Crypto_NvramOperationType;
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)) */

/* Structure defining the way the parameters of Crypto_ProcessJob() API should be handled */
typedef struct
{
    boolean bSingleCallOnly;           /* Boolean flag stating if the service is available only in SingleCall mode */
    boolean bCheckTargetKeyId;         /* Boolean flag stating if a check should be performed for the validity of TargetKeyId. Applicable for KeyManagement services accessible through Crypto_ProcessJob() API */
    uint8   u8InputModeMask;           /* 8 bit mask defining in which streaming modes the input parameter of the job should be checked for availability and corectness */
    uint8   u8SecondaryInputModeMask;  /* 8 bit mask defining in which streaming modes the secondary input parameter of the job should be checked for availability and corectness */
    uint8   u8TertiaryInputModeMask;   /* 8 bit mask defining in which streaming modes the tertiary input parameter of the job should be checked for availability and corectness */
    uint8   u8OutputModeMask;          /* 8 bit mask defining in which streaming modes the output parameter of the job should be checked for availability and corectness */
    uint8   u8SecondaryOutputModeMask; /* 8 bit mask defining in which streaming modes the secondary output parameter of the job should be checked for availability and corectness */
    uint8   u8VerifyPtrModeMask;       /* 8 bit mask defining in which streaming modes the verifyPtr parameter of the job should be checked for availability and corectness */
}Crypto_ProcessJobServiceParametersType;

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/
/* Source offset for key element copy */
#define CRYPTO_KEYCOPY_SRC_OFFSET_U32           ((uint32)0U)
/* Define the number of services supported by Crypto driver */
#if (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT)
    #define CRYPTO_NUM_SUPPORTED_SERVICES_U8    ((uint8)((uint8)CRYPTO_KEYSETVALID + 1U))
#else
    #define CRYPTO_NUM_SUPPORTED_SERVICES_U8    ((uint8)((uint8)CRYPTO_RANDOMGENERATE + 1U))
#endif
/* Defines for the UPDATE (U), FINISH (F) or UPDATE & FINISH (UF) Crypto operation modes */
#define CRYPTO_U_U8                             ((uint8)(CRYPTO_OPERATIONMODE_UPDATE))
#define CRYPTO_F_U8                             ((uint8)(CRYPTO_OPERATIONMODE_FINISH))
#define CRYPTO_UF_U8                            ((uint8)(CRYPTO_U_U8 | CRYPTO_F_U8))

#if (STD_ON == CRYPTO_ENABLE_FEED_HSE_DESC_SUPPORT)
    /* Define for the id of the keyElement requesting Crypto to perform a direct request to HSE FW */
    #define CRYPTO_KE_FEED_HSE_DESCRIPTOR_U32   ((uint32)0xFEEDDE5CU)
#endif /* (STD_ON == CRYPTO_ENABLE_FEED_HSE_DESC_SUPPORT) */

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/
#define CRYPTO_START_SEC_CONST_UNSPECIFIED
#include "Crypto_MemMap.h"

/* Array containing information about job parameters for Crypto_ProcessJob() API */
static const Crypto_ProcessJobServiceParametersType Crypto_aProcessJobServiceParams[CRYPTO_NUM_SUPPORTED_SERVICES_U8] =
{
    /* bSingleCallOnly, bCheckTargetKeyId, u8InputModeMask, u8SecondaryInputModeMask, u8TertiaryInputModeMask, u8OutputModeMask, u8SecondaryOutputModeMask, u8VerifyPtrModeMask */
    {  (boolean)FALSE,   (boolean)FALSE,     CRYPTO_U_U8,            0x0U,                    0x0U,              CRYPTO_F_U8,              0x0U,                   0x0U         },  /* CRYPTO_HASH                  Idx = 0x00 */
    {  (boolean)FALSE,   (boolean)FALSE,     CRYPTO_U_U8,            0x0U,                    0x0U,              CRYPTO_F_U8,              0x0U,                   0x0U         },  /* CRYPTO_MACGENERATE           Idx = 0x01 */
    {  (boolean)FALSE,   (boolean)FALSE,     CRYPTO_U_U8,         CRYPTO_F_U8,                0x0U,                 0x0U,                  0x0U,                CRYPTO_F_U8     },  /* CRYPTO_MACVERIFY             Idx = 0x02 */
    {  (boolean)FALSE,   (boolean)FALSE,     CRYPTO_U_U8,            0x0U,                    0x0U,              CRYPTO_UF_U8,             0x0U,                   0x0U         },  /* CRYPTO_ENCRYPT               Idx = 0x03 */
    {  (boolean)FALSE,   (boolean)FALSE,     CRYPTO_U_U8,            0x0U,                    0x0U,              CRYPTO_UF_U8,             0x0U,                   0x0U         },  /* CRYPTO_DECRYPT               Idx = 0x04 */
    {  (boolean)FALSE,   (boolean)FALSE,     CRYPTO_U_U8,         CRYPTO_F_U8,                0x0U,              CRYPTO_UF_U8,          CRYPTO_F_U8,               0x0U         },  /* CRYPTO_AEADENCRYPT           Idx = 0x05 */
    {  (boolean)FALSE,   (boolean)FALSE,     CRYPTO_U_U8,         CRYPTO_F_U8,             CRYPTO_F_U8,          CRYPTO_UF_U8,             0x0U,                CRYPTO_F_U8     },  /* CRYPTO_AEADDECRYPT           Idx = 0x06 */
    {  (boolean)FALSE,   (boolean)FALSE,     CRYPTO_U_U8,            0x0U,                    0x0U,              CRYPTO_F_U8,              0x0U,                   0x0U         },  /* CRYPTO_SIGNATUREGENERATE     Idx = 0x07 */
    {  (boolean)FALSE,   (boolean)FALSE,     CRYPTO_U_U8,         CRYPTO_F_U8,                0x0U,                 0x0U,                  0x0U,                CRYPTO_F_U8     },  /* CRYPTO_SIGNATUREVERIFY       Idx = 0x08 */
    {  (boolean)FALSE,   (boolean)FALSE,        0x0U,                0x0U,                    0x0U,                 0x0U,                  0x0U,                   0x0U         },  /* EMPTY SLOT                   Idx = 0x09 */
    {  (boolean)FALSE,   (boolean)FALSE,        0x0U,                0x0U,                    0x0U,                 0x0U,                  0x0U,                   0x0U         },  /* EMPTY SLOT                   Idx = 0x0A */
    {  (boolean)TRUE,    (boolean)FALSE,        0x0U,                0x0U,                    0x0U,              CRYPTO_F_U8,              0x0U,                   0x0U         },  /* CRYPTO_RANDOMGENERATE        Idx = 0x0B */
#if (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT)
    {  (boolean)TRUE,    (boolean)TRUE,      CRYPTO_F_U8,            0x0U,                    0x0U,                 0x0U,                  0x0U,                   0x0U         },  /* CRYPTO_RANDOMSEED            Idx = 0x0C */
    {  (boolean)TRUE,    (boolean)FALSE,        0x0U,                0x0U,                    0x0U,                 0x0U,                  0x0U,                   0x0U         },  /* CRYPTO_KEYGENERATE           Idx = 0x0D */
    {  (boolean)TRUE,    (boolean)TRUE,         0x0U,                0x0U,                    0x0U,                 0x0U,                  0x0U,                   0x0U         },  /* CRYPTO_KEYDERIVE             Idx = 0x0E */
    {  (boolean)TRUE,    (boolean)FALSE,        0x0U,             CRYPTO_F_U8,                0x0U,                 0x0U,                  0x0U,                   0x0U         },  /* CRYPTO_KEYEXCHANGECALCPUBVAL Idx = 0x0F */
    {  (boolean)TRUE,    (boolean)FALSE,     CRYPTO_F_U8,            0x0U,                    0x0U,                 0x0U,                  0x0U,                   0x0U         },  /* CRYPTO_KEYEXCHANGECALCSECRET Idx = 0x10 */
    {  (boolean)TRUE,    (boolean)FALSE,        0x0U,                0x0U,                    0x0U,                 0x0U,                  0x0U,                   0x0U         },  /* CRYPTO_CERTIFICATEPARSE      Idx = 0x11 */
    {  (boolean)TRUE,    (boolean)TRUE,         0x0U,                0x0U,                 CRYPTO_F_U8,             0x0U,               CRYPTO_F_U8,               0x0U         },  /* CRYPTO_CERTIFICATEVERIFY     Idx = 0x12 */
    {  (boolean)TRUE,    (boolean)FALSE,        0x0U,                0x0U,                    0x0U,                 0x0U,                  0x0U,                   0x0U         }   /* CRYPTO_KEYSETVALID           Idx = 0x13 */
#endif /* (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT) */
};

#define CRYPTO_STOP_SEC_CONST_UNSPECIFIED
#include "Crypto_MemMap.h"

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/
#define CRYPTO_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_MemMap.h"


/**
* @brief          This variable holds the state of the driver.
* @details        This variable holds the state of the driver. After reset is UNINIT. The output of Crypto_Init() function
*                 should set this variable into IDLE state.
*                  CRYPTO_DRIVER_UNINIT = The CRYPTO controller is not initialized. All registers belonging to the CRYPTO module are in reset state.
*/
Crypto_DriverStateType Crypto_aeDriverState[CRYPTO_MAX_NUMBER_PARTITIONS_U8];

#define CRYPTO_STOP_SEC_VAR_CLEARED_UNSPECIFIED
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

#if (STD_ON == CRYPTO_KEYS_EXIST)
static inline Std_ReturnType Crypto_VerifyKeyValidity
(
    const uint32                 u32CryptoKeyIdx,
    const Crypto_ServiceInfoType eJobService
);

static inline Std_ReturnType Crypto_CopyKeyElements
(
    const uint32 u32PartitionId,
    const uint32 u32KeyMaterialElementIndex,
    const uint32 u32CryptoKeyIdx,
    const uint32 u32TargetKeyIdx,
    const uint32 u32TargetKeyElementIndex
);

#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

static inline Crypto_ProcessJobErrorType Crypto_GetJobErrorForInputPtr
(
    const Crypto_ServiceInfoType                    eJobService,
    const Crypto_JobPrimitiveInputOutputType* const pJobPrimitiveInputOutput,
    const Crypto_OperationModeType                  eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    ,const Crypto_JobRedirectionInfoType*           pJobRedir
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
);

static inline Crypto_ProcessJobErrorType Crypto_GetJobErrorForSecondaryInputPtr
(
    const Crypto_ServiceInfoType                    eJobService,
    const Crypto_JobPrimitiveInputOutputType* const pJobPrimitiveInputOutput,
    const Crypto_OperationModeType                  eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    ,const Crypto_JobRedirectionInfoType*           pJobRedir
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
);

static inline Crypto_ProcessJobErrorType Crypto_GetJobErrorForTertiaryInputPtr
(
    const Crypto_ServiceInfoType                    eJobService,
    const Crypto_JobPrimitiveInputOutputType* const pJobPrimitiveInputOutput,
    const Crypto_OperationModeType                  eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    ,const Crypto_JobRedirectionInfoType*           pJobRedir
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
);

static inline Crypto_ProcessJobErrorType Crypto_GetJobErrorForOutputPtr
(
    const Crypto_ServiceInfoType                    eJobService,
    const Crypto_JobPrimitiveInputOutputType* const pJobPrimitiveInputOutput,
    const Crypto_OperationModeType                  eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    ,const Crypto_JobRedirectionInfoType*           pJobRedir
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
);

static inline Crypto_ProcessJobErrorType Crypto_GetJobErrorForSecondaryOutputPtr
(
    const Crypto_ServiceInfoType                    eJobService,
    const Crypto_JobPrimitiveInputOutputType* const pJobPrimitiveInputOutput,
    const Crypto_OperationModeType                  eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    ,const Crypto_JobRedirectionInfoType*           pJobRedir
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
);

static inline Crypto_ProcessJobErrorType Crypto_GetJobErrorForVerifyPtr
(
    const Crypto_ServiceInfoType                    eJobService,
    const Crypto_JobPrimitiveInputOutputType* const pJobPrimitiveInputOutput,
    const Crypto_OperationModeType                  eJobMode
);

static inline Crypto_ProcessJobErrorType Crypto_GetJobErrorForService
(
    const Crypto_ServiceInfoType                    eJobService,
    const Crypto_JobPrimitiveInputOutputType* const pJobPrimitiveInputOutput,
    const Crypto_OperationModeType                  eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    ,const Crypto_JobRedirectionInfoType*           pJobRedir
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
);

static inline Crypto_ProcessJobErrorType Crypto_GetJobErrorForOutAndVerifyPtrs
(
    const Crypto_ServiceInfoType                    eJobService,
    const Crypto_JobPrimitiveInputOutputType* const pJobPrimitiveInputOutput,
    const Crypto_OperationModeType                  eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    ,const Crypto_JobRedirectionInfoType*           pJobRedir
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
);

static inline Crypto_ProcessJobErrorType Crypto_GetProcessJobError
(
    const Crypto_JobType* const pJob
);

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)

static inline Std_ReturnType Crypto_VerifyPrimitive
(
    uint32                u32ObjectId,
    const Crypto_JobType* pJob
);

#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */

#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT))
static Std_ReturnType Crypto_HandleNvramInfo
(
    const uint32              u32PartitionId,
    Crypto_NvramOperationType eNvramOperation,
    uint32                    u32BlobId,
    uint8                     u8ServiceId
);
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)) */

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
static Std_ReturnType Crypto_CheckValidCoreAndDrvInitForCdoApis
(
    const uint32 u32PartitionId,
    const uint32 u32ObjectIdx,
    const uint8  u8ApiId
);
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT)) */

#if (STD_ON == CRYPTO_KEYS_EXIST)
static inline Std_ReturnType Crypto_GetNonDetErrorsForRandomSeed
(
    const uint32 u32CryptoKeyIdx
);

#if (STD_ON == CRYPTO_SPT_KEY_GENERATE)
static Std_ReturnType Crypto_GetNonDetErrorsForKeyGenerate
(
    const uint32 u32CryptoKeyIdx
);
#endif /* (STD_ON == CRYPTO_SPT_KEY_GENERATE) */

#if (STD_ON == CRYPTO_SPT_KEY_DERIVE)
static Std_ReturnType Crypto_GetNonDetErrorsForKeyDerive
(
    const uint32 u32CryptoKeyIdx,
    const uint32 u32CryptoTargetKeyIdx
);
#endif /* (STD_ON == CRYPTO_SPT_KEY_DERIVE) */

static Std_ReturnType Crypto_GetNonDetErrorsForKeyExchCalcPubVal
(
    const uint8         u8ServiceId,
    const uint32        u32CryptoKeyIdx,
    const uint8*  const pu8PublicValuePtr,
    const uint32* const pu32PublicValueLengthPtr
);

static Std_ReturnType Crypto_CopyDataFromPubValKeyElementToUser
(
    const uint32  u32PartitionId,
    const uint32  u32CryptoKeyIdx,
#if ((STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT))
    uint8* const  pu8PublicValuePtr,
#else
    const uint8*  pu8PublicValuePtr,
#endif
    uint32* const pu32PublicValueLengthPtr
);

static Std_ReturnType Crypto_GetNonDetErrorsForKeyExchCalcSecret
(
    const uint32 u32CryptoKeyIdx
);

static Std_ReturnType Crypto_MarkKeyAsValid
(
    const uint32 u32PartitionId,
    const uint32 u32CryptoKeyIdx,
    const uint8  u8ServiceId
);

static Std_ReturnType Crypto_CheckKeyCopyValid
(
    uint32 u32CryptoKeyIdx,
    uint32 u32TargetKeyIdx
);

static Std_ReturnType Crypto_CheckKeyCopyValidKeyElements
(
    uint32   u32CryptoKeyIdx,
    uint32   u32CryptoKeyElemIdx,
    uint32   u32TargetKeyIdx,
    boolean* pbFound
);

static inline Std_ReturnType Crypto_CheckParamAndConfForKeyElementCopy
(
    const uint32 u32CryptoKeyElementIdx,
    const uint32 u32CryptoTargetKeyElementIdx
);

static inline Std_ReturnType Crypto_CheckParamAndConfForKeyElementCopyPartial
(
    const uint32 u32CryptoKeyElementIdx,
    const uint32 u32CryptoTargetKeyElementIdx,
    const uint32 u32KeyElementSourceOffset,
    const uint32 u32KeyElementTargetOffset,
    const uint32 u32KeyElementCopyLength
);

static inline Std_ReturnType Crypto_CheckForErrorsInKeyElementSet
(
    const uint32 u32PartitionId,
    uint32       u32CryptoKeyIdx,
    const uint8* pu8Key,
    uint32       u32KeyLength,
    uint32       u32KeyElementId,
    uint32*      pu32CryptoKeyElementIdx
);

static inline Std_ReturnType Crypto_ProcessJobAndCheckNonDetErrors
(
    Crypto_JobType* pJob,
    const uint32    u32PartitionId
#if (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT)
    , boolean*      pbJobProcessingComplete
#endif /* (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT) */
);
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

static inline Std_ReturnType Crypto_CheckForErrorsInProcessJob
(
    const uint32          u32PartitionId,
    const uint32          u32ObjectIdx,
    const Crypto_JobType* pJob
);

#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
static Crypto_ProcessJobErrorType Crypto_CheckRedirectionConfig
(
    uint32 u32RedirectedKeyId,
    uint32 u32RedirectedKeyElementId
);
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */

#if (STD_ON == CRYPTO_KEYS_EXIST)
#if (STD_ON == CRYPTO_SPT_ENC_AUTH_KEY_EXPORT)
static inline boolean Crypto_AuthCipherModeIsCcmOrGcm
(
    const uint8 u8AuthCipherMode
);
#endif /* (STD_ON == CRYPTO_SPT_ENC_AUTH_KEY_EXPORT) */

static inline Std_ReturnType Crypto_GetNonKeyMaterialElement
(
    const uint32 u32PartitionId,
    const uint32 u32CryptoKeyIdx,
    const uint32 u32CryptoKeyElementId,
    uint8*       pResult,
    uint32*      pResultLength
);
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/
#if (STD_ON == CRYPTO_KEYS_EXIST)
/**
* @brief           Verify if a key ID is valid
* @details         Verify if a key ID is valid  (found inside the array of all key IDs supported by the Crypto driver)
*
* @param[in]       u32CryptoKeyIdx: Index of the Cypto key
* @param[in]       eJobService:     Job's service type
*
* @return          E_OK: Request successful, key ID found
*                  E_NOT_OK: Request failed, key ID not found
*
* @pre
*
*/
static inline Std_ReturnType Crypto_VerifyKeyValidity
(
    const uint32                 u32CryptoKeyIdx,
    const Crypto_ServiceInfoType eJobService
)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;

    /* All crypto services listed in Crypto_ServiceInfoType except for:
        - CRYPTO_RANDOMGENERATE
        - CRYPTO_HASH
    require a key represented as a key identifier. */
    if ((CRYPTO_RANDOMGENERATE != eJobService) &&
        (CRYPTO_HASH           != eJobService)
       )
    {
        if (CRYPTO_INVALID_KEY_IDX_U32 != u32CryptoKeyIdx)
        {
            if (!Crypto_IsKeyValidFlagSet(u32CryptoKeyIdx))
            {
                RetVal = (Std_ReturnType)CRYPTO_E_KEY_NOT_VALID;
            }
        }
        else
        {
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
    }
    return RetVal;
}

/**
* @brief           Copy all keyMaterial and non material key elements from one key to another
* @details         Copy all keyMaterial and non material key elements from one key to another
*
* @param[in]       u32KeyMaterialElementIndex - index of the keyMaterial element in the cryptoKey
* @param[in]       u32CryptoKeyIdx            - index of the key from where to copy the non material key elements
* @param[in]       u32TargetKeyIdx            - index of the key where to copy the non material key elements
* @param[in]       u32TargetKeyElementIndex   - index of the TargetkeyMaterial element in the cryptoKey
*
* @return          Std_ReturnType
*/
static inline Std_ReturnType Crypto_CopyKeyElements
(
    const uint32 u32PartitionId,
    const uint32 u32KeyMaterialElementIndex,
    const uint32 u32CryptoKeyIdx,
    const uint32 u32TargetKeyIdx,
    const uint32 u32TargetKeyElementIndex
)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;
    boolean        bFound;
    uint32         u32Counter1;
    uint32         u32Counter2;
#if (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)
    boolean        bKeyElemValBlobNeedsUpdate = FALSE;
#endif /* (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT) */

    if ((CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32KeyMaterialElementIndex) && (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32TargetKeyElementIndex))
    {
        if ((hseKeyType_t)HSE_KEY_TYPE_SHARED_SECRET == Crypto_aKeyElementList[u32KeyMaterialElementIndex].u8HseKeyType)
        {
            /* Extract/Move the specified length starting from specified offset from source key element(key material) to target source element(key material) */
            RetVal = Crypto_Ipw_KeyDeriveExtract(u32PartitionId, u32KeyMaterialElementIndex, u32TargetKeyElementIndex,CRYPTO_KEYCOPY_SRC_OFFSET_U32,*Crypto_aKeyElementList[u32TargetKeyElementIndex].pu32CryptoElementActualSize);
        }
    }

    if ((Std_ReturnType)E_OK == RetVal)
    /* Copy the rest of the elements (different from key material) */
    {
        for (u32Counter1 = 0U; u32Counter1 < Crypto_aKeyList[u32CryptoKeyIdx].u32NumCryptoKeyElements; u32Counter1++)
        {
            bFound = FALSE;
            for (u32Counter2 = 0U; ((u32Counter2 < Crypto_aKeyList[u32TargetKeyIdx].u32NumCryptoKeyElements) && (FALSE == bFound)); u32Counter2++)
            {
                /* Need to check if:
                   1. Key elements have same KeyElementId
                   2. The KeyElementId is not KeyMaterial
                   3. The buffer of the target u32KeyElementId (where data will be copied to) is not a NULL pointer */
                if ((Crypto_aKeyElementList[Crypto_aKeyList[u32CryptoKeyIdx].pCryptoKeyElementList[u32Counter1]].u32CryptoKeyElementId ==  \
                     Crypto_aKeyElementList[Crypto_aKeyList[u32TargetKeyIdx].pCryptoKeyElementList[u32Counter2]].u32CryptoKeyElementId) &&
                     (CRYPTO_KEY_MATERIAL_U32 != Crypto_aKeyElementList[Crypto_aKeyList[u32CryptoKeyIdx].pCryptoKeyElementList[u32Counter1]].u32CryptoKeyElementId) &&
                     (NULL_PTR != Crypto_aKeyElementList[Crypto_aKeyList[u32TargetKeyIdx].pCryptoKeyElementList[u32Counter2]].pCryptoElementArray)
                    )
                {
                    bFound = TRUE;
                    Crypto_Util_MemCpy8(Crypto_aKeyElementList[Crypto_aKeyList[u32TargetKeyIdx].pCryptoKeyElementList[u32Counter2]].pCryptoElementArray,
                                        Crypto_aKeyElementList[Crypto_aKeyList[u32CryptoKeyIdx].pCryptoKeyElementList[u32Counter1]].pCryptoElementArray,
                                        *Crypto_aKeyElementList[Crypto_aKeyList[u32CryptoKeyIdx].pCryptoKeyElementList[u32Counter1]].pu32CryptoElementActualSize
                                      );
                    *Crypto_aKeyElementList[Crypto_aKeyList[u32TargetKeyIdx].pCryptoKeyElementList[u32Counter2]].pu32CryptoElementActualSize = \
                    *Crypto_aKeyElementList[Crypto_aKeyList[u32CryptoKeyIdx].pCryptoKeyElementList[u32Counter1]].pu32CryptoElementActualSize;
#if (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)
                    /* If the destination key element is marked as persistent, we need to request the upper layer to store in Nvram the key elements values blob */
                    if (TRUE == Crypto_aKeyElementList[Crypto_aKeyList[u32TargetKeyIdx].pCryptoKeyElementList[u32Counter2]].bCryptoKeyElementPersist)
                    {
                        bKeyElemValBlobNeedsUpdate = TRUE;
                    }
#endif /* (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT) */
                }
            }
        }
#if (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)
        if (TRUE == bKeyElemValBlobNeedsUpdate)
        {
            RetVal = Crypto_HandleNvramInfo(u32PartitionId, CRYPTO_NVRAM_OP_WRITE, CRYPTO_NVRAM_BLOB_1_ID, CRYPTO_KEYCOPY_ID);
        }
#endif /* (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT) */
    }

    return RetVal;
}

#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

/**
* @brief           Verifies primary input pointer of one job for corectness (null ptr, invalid length)
* @details         Verifies primary input  pointer of one job for corectness (null ptr, invalid length)
*
* @param[in]       eJobService              - CSM service as present in the job
* @param[in]       pJobPrimitiveInputOutput - Pointer to the JobPrimitiveInputOutput structrue inside the job
* @param[in]       eJobMode                 - Streaming or singlecall
* @param[in]       pJobRedir                - Pointer to the redirection structure in the job
*
* @return          Result of the operation
* @retval          CRYPTO_NO_DET_RET_OK:            No error to report to DET, the function should return E_OK
* @retval          CRYPTO_NO_DET_RET_NOK:           No error to report to DET, the function should return E_NOT_OK
* @retval          CRYPTO_NO_DET_RET_KEY_NOT_VALID: No error to report to DET, the function should return CRYPTO_E_KEY_NOT_VALID
* @retval          CRYPTO_NO_DET_RET_NOT_SUPPORTED: No error to report to DET, the function should return CRYPTO_E_NOT_SUPPORTED
* @retval          CRYPTO_DET_NULL_PTR:             CRYPTO_E_PARAM_POINTER should be reported to DET
* @retval          CRYPTO_DET_INVALID_PARAM:        CRYPTO_E_PARAM_HANDLE  should be reported to DET
* @retval          CRYPTO_DET_INVALID_LEN:          CRYPTO_E_PARAM_VALUE   should be reported to DET
*/
static inline Crypto_ProcessJobErrorType Crypto_GetJobErrorForInputPtr
(
    const Crypto_ServiceInfoType                    eJobService,
    const Crypto_JobPrimitiveInputOutputType* const pJobPrimitiveInputOutput,
    const Crypto_OperationModeType                  eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    ,const Crypto_JobRedirectionInfoType*           pJobRedir
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
)
{
    Crypto_ProcessJobErrorType eRetVal = CRYPTO_NO_DET_RET_OK;

    /* Check the inputBuffer and inputLength, if a match exists between what spec requires for this service and what is the value provided by the app in eJobMode parameter */
    if (0x0U != ((uint8)eJobMode & (Crypto_aProcessJobServiceParams[eJobService].u8InputModeMask)))
    {
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
        /* If redirection is not configured, or redirection is configured and primary input is not set check inputPtr */
        if (
            (NULL_PTR == pJobRedir) ||
            !(Crypto_Util_IsRedirectionSet(pJobRedir->redirectionConfig, CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT))
           )
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
        {
            if (NULL_PTR == pJobPrimitiveInputOutput->inputPtr)
            {
                eRetVal = CRYPTO_DET_NULL_PTR;
            }
        }
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
        else
        {
            /* If redirection is configured to redirect primary input, check the inputKey */
            eRetVal = Crypto_CheckRedirectionConfig(
                                                    pJobRedir->inputKeyId,
                                                    pJobRedir->inputKeyElementId
                                                   );
        }
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
    }
    return eRetVal;
}

/**
* @brief           Verifies secondary input pointer of one job for corectness (null ptr, invalid length)
* @details         Verifies secondary input  pointer of one job for corectness (null ptr, invalid length)
*
* @param[in]       eJobService              - CSM service as present in the job
* @param[in]       pJobPrimitiveInputOutput - Pointer to the JobPrimitiveInputOutput structrue inside the job
* @param[in]       eJobMode                 - Streaming or singlecall
* @param[in]       pJobRedir                - Pointer to the redirection structure in the job
*
* @return          Result of the operation
* @retval          CRYPTO_NO_DET_RET_OK:            No error to report to DET, the function should return E_OK
* @retval          CRYPTO_NO_DET_RET_NOK:           No error to report to DET, the function should return E_NOT_OK
* @retval          CRYPTO_NO_DET_RET_KEY_NOT_VALID: No error to report to DET, the function should return CRYPTO_E_KEY_NOT_VALID
* @retval          CRYPTO_NO_DET_RET_NOT_SUPPORTED: No error to report to DET, the function should return CRYPTO_E_NOT_SUPPORTED
* @retval          CRYPTO_DET_NULL_PTR:             CRYPTO_E_PARAM_POINTER should be reported to DET
* @retval          CRYPTO_DET_INVALID_PARAM:        CRYPTO_E_PARAM_HANDLE  should be reported to DET
* @retval          CRYPTO_DET_INVALID_LEN:          CRYPTO_E_PARAM_VALUE   should be reported to DET
*/
static inline Crypto_ProcessJobErrorType Crypto_GetJobErrorForSecondaryInputPtr
(
    const Crypto_ServiceInfoType                    eJobService,
    const Crypto_JobPrimitiveInputOutputType* const pJobPrimitiveInputOutput,
    const Crypto_OperationModeType                  eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    ,const Crypto_JobRedirectionInfoType*           pJobRedir
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
)
{
    Crypto_ProcessJobErrorType eRetVal = CRYPTO_NO_DET_RET_OK;

    /* Check the secondaryInputBuffer and secondaryInputLength, if a match exists between what spec requires for this service and what is the value provided by the app in eJobMode parameter */
    if (0x0U != ((uint8)eJobMode & (Crypto_aProcessJobServiceParams[eJobService].u8SecondaryInputModeMask)))
    {
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
        /* If redirection is not configured, or redirection is configured and secondary input redirection is not set check secondaryInputPtr */
        if (
            (NULL_PTR == pJobRedir) ||
            !(Crypto_Util_IsRedirectionSet(pJobRedir->redirectionConfig, CRYPTO_REDIRECT_CONFIG_SECONDARY_INPUT))
           )
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
        {
            if (NULL_PTR == pJobPrimitiveInputOutput->secondaryInputPtr)
            {
                eRetVal = CRYPTO_DET_NULL_PTR;
            }
        }
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
        else
        {
            /* If redirection is configured to redirect secondary input, check the secondaryInputKey */
            eRetVal = Crypto_CheckRedirectionConfig(
                                                    pJobRedir->secondaryInputKeyId,
                                                    pJobRedir->secondaryInputKeyElementId
                                                   );
        }
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
    }

    return eRetVal;
}

/**
* @brief           Verifies tertiary input pointer of one job for corectness (null ptr, invalid length)
* @details         Verifies tertiary input  pointer of one job for corectness (null ptr, invalid length)
*
* @param[in]       eJobService              - CSM service as present in the job
* @param[in]       pJobPrimitiveInputOutput - Pointer to the JobPrimitiveInputOutput structrue inside the job
* @param[in]       eJobMode                 - Streaming or singlecall
* @param[in]       pJobRedir                - Pointer to the redirection structure in the job
*
* @return          Result of the operation
* @retval          CRYPTO_NO_DET_RET_OK:            No error to report to DET, the function should return E_OK
* @retval          CRYPTO_NO_DET_RET_NOK:           No error to report to DET, the function should return E_NOT_OK
* @retval          CRYPTO_NO_DET_RET_KEY_NOT_VALID: No error to report to DET, the function should return CRYPTO_E_KEY_NOT_VALID
* @retval          CRYPTO_NO_DET_RET_NOT_SUPPORTED: No error to report to DET, the function should return CRYPTO_E_NOT_SUPPORTED
* @retval          CRYPTO_DET_NULL_PTR:             CRYPTO_E_PARAM_POINTER should be reported to DET
* @retval          CRYPTO_DET_INVALID_PARAM:        CRYPTO_E_PARAM_HANDLE  should be reported to DET
* @retval          CRYPTO_DET_INVALID_LEN:          CRYPTO_E_PARAM_VALUE   should be reported to DET
*/
static inline Crypto_ProcessJobErrorType Crypto_GetJobErrorForTertiaryInputPtr
(
    const Crypto_ServiceInfoType                    eJobService,
    const Crypto_JobPrimitiveInputOutputType* const pJobPrimitiveInputOutput,
    const Crypto_OperationModeType                  eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    ,const Crypto_JobRedirectionInfoType*           pJobRedir
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
)
{
    Crypto_ProcessJobErrorType eRetVal = CRYPTO_NO_DET_RET_OK;

    /* Check the tertiaryInputBuffer and tertiaryInputLength, if a match exists between what spec requires for this service and what is the value provided by the app in eJobMode parameter */
    if (0x0U != ((uint8)eJobMode & (Crypto_aProcessJobServiceParams[eJobService].u8TertiaryInputModeMask)))
    {
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
        /* If redirection is not configured, or redirection is configured and tertiary input redirection is not set check tertiaryInputPtr */
        if (
            (NULL_PTR == pJobRedir) ||
            !(Crypto_Util_IsRedirectionSet(pJobRedir->redirectionConfig, CRYPTO_REDIRECT_CONFIG_TERTIARY_INPUT))
           )
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
        {
            if (NULL_PTR == pJobPrimitiveInputOutput->tertiaryInputPtr)
            {
                eRetVal = CRYPTO_DET_NULL_PTR;
            }
        }
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
        else
        {
            /* If redirection is configured to redirect tertiary input, check the tertiaryInputKey */
            eRetVal = Crypto_CheckRedirectionConfig(
                                                    pJobRedir->tertiaryInputKeyId,
                                                    pJobRedir->tertiaryInputKeyElementId
                                                   );
        }
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
    }
    return eRetVal;
}

/**
* @brief           Verifies primary output pointer of one job for corectness (null ptr, invalid length)
* @details         Verifies primary output pointer of one job for corectness (null ptr, invalid length)
*
* @param[in]       eJobService              - CSM service as present in the job
* @param[in]       pJobPrimitiveInputOutput - Pointer to the JobPrimitiveInputOutput structrue inside the job
* @param[in]       eJobMode                 - Streaming or singlecall
* @param[in]       pJobRedir                - Pointer to the redirection structure in the job
*
* @return          Result of the operation
* @retval          CRYPTO_NO_DET_RET_OK:            No error to report to DET, the function should return E_OK
* @retval          CRYPTO_NO_DET_RET_NOK:           No error to report to DET, the function should return E_NOT_OK
* @retval          CRYPTO_NO_DET_RET_KEY_NOT_VALID: No error to report to DET, the function should return CRYPTO_E_KEY_NOT_VALID
* @retval          CRYPTO_NO_DET_RET_NOT_SUPPORTED: No error to report to DET, the function should return CRYPTO_E_NOT_SUPPORTED
* @retval          CRYPTO_DET_NULL_PTR:             CRYPTO_E_PARAM_POINTER should be reported to DET
* @retval          CRYPTO_DET_INVALID_PARAM:        CRYPTO_E_PARAM_HANDLE  should be reported to DET
* @retval          CRYPTO_DET_INVALID_LEN:          CRYPTO_E_PARAM_VALUE   should be reported to DET
*/
static inline Crypto_ProcessJobErrorType Crypto_GetJobErrorForOutputPtr
(
    const Crypto_ServiceInfoType                    eJobService,
    const Crypto_JobPrimitiveInputOutputType* const pJobPrimitiveInputOutput,
    const Crypto_OperationModeType                  eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    ,const Crypto_JobRedirectionInfoType*           pJobRedir
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
)
{
    Crypto_ProcessJobErrorType eRetVal = CRYPTO_NO_DET_RET_OK;

    /* Check the outputBuffer and outputLength, if a match exists between what spec requires for this service and what is the value provided by the app in eJobMode parameter */
    if (0x0U != ((uint8)eJobMode & (Crypto_aProcessJobServiceParams[eJobService].u8OutputModeMask)))
    {
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
        /* If redirection is not configured, or redirection is configured and primary output redirection is not set check outputPtr */
        if (
            (NULL_PTR == pJobRedir) ||
            !(Crypto_Util_IsRedirectionSet(pJobRedir->redirectionConfig, CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT))
           )
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
        {
            if ((NULL_PTR == pJobPrimitiveInputOutput->outputPtr) || (NULL_PTR == pJobPrimitiveInputOutput->outputLengthPtr))
            {
                eRetVal = CRYPTO_DET_NULL_PTR;
            }
            else
            {
                if (0U == *(pJobPrimitiveInputOutput->outputLengthPtr))
                {
                    eRetVal = CRYPTO_DET_INVALID_LEN;
                }
            }
        }
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
        else
        {
            /* If redirection is configured to redirect the primary output, check the primaryOutputKey */
            eRetVal = Crypto_CheckRedirectionConfig(
                                                    pJobRedir->outputKeyId,
                                                    pJobRedir->outputKeyElementId
                                                   );
        }
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
    }
    return eRetVal;
}

/**
* @brief           Verifies secondary output pointer of one job for corectness (null ptr, invalid length)
* @details         Verifies secondary output pointer of one job for corectness (null ptr, invalid length)
*
* @param[in]       eJobService              - CSM service as present in the job
* @param[in]       pJobPrimitiveInputOutput - Pointer to the JobPrimitiveInputOutput structrue inside the job
* @param[in]       eJobMode                 - Streaming or singlecall
* @param[in]       pJobRedir                - Pointer to the redirection structure in the job
*
* @return          Result of the operation
* @retval          CRYPTO_NO_DET_RET_OK:            No error to report to DET, the function should return E_OK
* @retval          CRYPTO_NO_DET_RET_NOK:           No error to report to DET, the function should return E_NOT_OK
* @retval          CRYPTO_NO_DET_RET_KEY_NOT_VALID: No error to report to DET, the function should return CRYPTO_E_KEY_NOT_VALID
* @retval          CRYPTO_NO_DET_RET_NOT_SUPPORTED: No error to report to DET, the function should return CRYPTO_E_NOT_SUPPORTED
* @retval          CRYPTO_DET_NULL_PTR:             CRYPTO_E_PARAM_POINTER should be reported to DET
* @retval          CRYPTO_DET_INVALID_PARAM:        CRYPTO_E_PARAM_HANDLE  should be reported to DET
* @retval          CRYPTO_DET_INVALID_LEN:          CRYPTO_E_PARAM_VALUE   should be reported to DET
*/
static inline Crypto_ProcessJobErrorType Crypto_GetJobErrorForSecondaryOutputPtr
(
    const Crypto_ServiceInfoType                    eJobService,
    const Crypto_JobPrimitiveInputOutputType* const pJobPrimitiveInputOutput,
    const Crypto_OperationModeType                  eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    ,const Crypto_JobRedirectionInfoType*           pJobRedir
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
)
{
    Crypto_ProcessJobErrorType eRetVal = CRYPTO_NO_DET_RET_OK;

    /* Check the secondaryOutputBuffer and secondaryOutputLength, if a match exists between what spec requires for this service and what is the value provided by the app in eJobMode parameter */
    if (0x0U != ((uint8)eJobMode & (Crypto_aProcessJobServiceParams[eJobService].u8SecondaryOutputModeMask)))
    {
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
        /* If redirection is not configured, or redirection is configured and secondary output redirection is not set check secondaryOutputPtr */
        if (
            (NULL_PTR == pJobRedir) ||
            !(Crypto_Util_IsRedirectionSet(pJobRedir->redirectionConfig, CRYPTO_REDIRECT_CONFIG_SECONDARY_OUTPUT))
           )
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
        {
            if ((NULL_PTR == pJobPrimitiveInputOutput->secondaryOutputPtr) || (NULL_PTR == pJobPrimitiveInputOutput->secondaryOutputLengthPtr))
            {
                eRetVal = CRYPTO_DET_NULL_PTR;
            }
            else
            {
                if (0U == *(pJobPrimitiveInputOutput->secondaryOutputLengthPtr))
                {
                    eRetVal = CRYPTO_DET_INVALID_LEN;
                }
            }
        }
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
        else
        {
            /* If redirection is configured to redirect the secondary output, check the secondaryOutputKey */
            eRetVal = Crypto_CheckRedirectionConfig(
                                                    pJobRedir->secondaryOutputKeyId,
                                                    pJobRedir->secondaryOutputKeyElementId
                                                   );
        }
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
    }
    return eRetVal;
}

/**
* @brief           Checks the verifyPtr job structure member for corectness
* @details         Checks the verifyPtr job structure member for corectness
*
* @param[in]       eJobService              - CSM service as present in the job
* @param[in]       pJobPrimitiveInputOutput - Pointer to the JobPrimitiveInputOutput structrue inside the job
* @param[in]       eJobMode                 - Streaming or singlecall
*
* @return          Result of the operation
* @retval          CRYPTO_NO_DET_RET_OK:            No error to report to DET, the function should return E_OK
* @retval          CRYPTO_NO_DET_RET_NOK:           No error to report to DET, the function should return E_NOT_OK
* @retval          CRYPTO_NO_DET_RET_KEY_NOT_VALID: No error to report to DET, the function should return CRYPTO_E_KEY_NOT_VALID
* @retval          CRYPTO_NO_DET_RET_NOT_SUPPORTED: No error to report to DET, the function should return CRYPTO_E_NOT_SUPPORTED
* @retval          CRYPTO_DET_NULL_PTR:             CRYPTO_E_PARAM_POINTER should be reported to DET
* @retval          CRYPTO_DET_INVALID_PARAM:        CRYPTO_E_PARAM_HANDLE  should be reported to DET
* @retval          CRYPTO_DET_INVALID_LEN:          CRYPTO_E_PARAM_VALUE   should be reported to DET
*/
static inline Crypto_ProcessJobErrorType Crypto_GetJobErrorForVerifyPtr
(
    const Crypto_ServiceInfoType                    eJobService,
    const Crypto_JobPrimitiveInputOutputType* const pJobPrimitiveInputOutput,
    const Crypto_OperationModeType                  eJobMode
)
{
    Crypto_ProcessJobErrorType eRetVal = CRYPTO_NO_DET_RET_OK;

    /* Check the verifyPtr, if a match exists between what spec requires for this service and what is the value provided by the app in eJobMode parameter */
    if (0x0U != ((uint8)eJobMode & (Crypto_aProcessJobServiceParams[eJobService].u8VerifyPtrModeMask)))
    {
        if (NULL_PTR == pJobPrimitiveInputOutput->verifyPtr)
        {
            eRetVal = CRYPTO_DET_NULL_PTR;
        }
    }
    return eRetVal;
}

/**
* @brief           Verifies all pointer and lengths parameters of one job for corectness (null ptr, invalid length)
* @details         Verifies all pointer and lengths parameters of one job for corectness (null ptr, invalid length)
*
* @param[in]       eJobService              - CSM service as present in the job
* @param[in]       pJobPrimitiveInputOutput - Pointer to the JobPrimitiveInputOutput structrue inside the job
* @param[in]       eJobMode                 - Streaming or singlecall
* @param[in]       pJobRedir                - Pointer to the redirection structure in the job
*
* @return          Result of the operation
* @retval          CRYPTO_NO_DET_RET_OK:            No error to report to DET, the function should return E_OK
* @retval          CRYPTO_NO_DET_RET_NOK:           No error to report to DET, the function should return E_NOT_OK
* @retval          CRYPTO_NO_DET_RET_KEY_NOT_VALID: No error to report to DET, the function should return CRYPTO_E_KEY_NOT_VALID
* @retval          CRYPTO_NO_DET_RET_NOT_SUPPORTED: No error to report to DET, the function should return CRYPTO_E_NOT_SUPPORTED
* @retval          CRYPTO_DET_NULL_PTR:             CRYPTO_E_PARAM_POINTER should be reported to DET
* @retval          CRYPTO_DET_INVALID_PARAM:        CRYPTO_E_PARAM_HANDLE  should be reported to DET
* @retval          CRYPTO_DET_INVALID_LEN:          CRYPTO_E_PARAM_VALUE   should be reported to DET
*/
static inline Crypto_ProcessJobErrorType Crypto_GetJobErrorForService
(
    const Crypto_ServiceInfoType                    eJobService,
    const Crypto_JobPrimitiveInputOutputType* const pJobPrimitiveInputOutput,
    const Crypto_OperationModeType                  eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    ,const Crypto_JobRedirectionInfoType*           pJobRedir
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
)
{
    Crypto_ProcessJobErrorType eRetVal = CRYPTO_NO_DET_RET_OK;

    /* Check first if the service supports only SingleCall operation mode. In case yes and the mode in the job is not SingleCall, return E_NOK error */
    if ((TRUE == Crypto_aProcessJobServiceParams[(uint8)eJobService].bSingleCallOnly) && (CRYPTO_OPERATIONMODE_SINGLECALL != eJobMode))
    {
        eRetVal = CRYPTO_NO_DET_RET_NOK;
    }
    else
    {
        eRetVal = Crypto_GetJobErrorForInputPtr(
                                                 eJobService,
                                                 pJobPrimitiveInputOutput,
                                                 eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
                                                 ,pJobRedir
#endif /* #if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
                                               );

        if (CRYPTO_NO_DET_RET_OK == eRetVal)
        {
            eRetVal = Crypto_GetJobErrorForSecondaryInputPtr(
                                                              eJobService,
                                                              pJobPrimitiveInputOutput,
                                                              eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
                                                              ,pJobRedir
#endif /* #if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
                                                            );

            if (CRYPTO_NO_DET_RET_OK == eRetVal)
            {
                eRetVal = Crypto_GetJobErrorForTertiaryInputPtr(
                                                                 eJobService,
                                                                 pJobPrimitiveInputOutput,
                                                                 eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
                                                                 ,pJobRedir
#endif /* #if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
                                                                );

                if (CRYPTO_NO_DET_RET_OK == eRetVal)
                {
                    eRetVal = Crypto_GetJobErrorForOutAndVerifyPtrs(
                                                                     eJobService,
                                                                     pJobPrimitiveInputOutput,
                                                                     eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
                                                                     ,pJobRedir
#endif /* #if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
                                                                   );

                }
            }
        }
    }

    return eRetVal;
}

/**
* @brief           Checks all out and verify parameters of one job for corectness (null ptr, invalid length)
* @details         Checks all out and verify parameters of one job for corectness (null ptr, invalid length)
*
* @param[in]       eJobService              - CSM service as present in the job
* @param[in]       pJobPrimitiveInputOutput - Pointer to the JobPrimitiveInputOutput structrue inside the job
* @param[in]       eJobMode                 - Streaming or singlecall
* @param[in]       pJobRedir                - Pointer to the redirection structure in the job
*
* @return          Result of the operation
* @retval          CRYPTO_NO_DET_RET_OK:            No error to report to DET, the function should return E_OK
* @retval          CRYPTO_NO_DET_RET_NOK:           No error to report to DET, the function should return E_NOT_OK
* @retval          CRYPTO_NO_DET_RET_KEY_NOT_VALID: No error to report to DET, the function should return CRYPTO_E_KEY_NOT_VALID
* @retval          CRYPTO_NO_DET_RET_NOT_SUPPORTED: No error to report to DET, the function should return CRYPTO_E_NOT_SUPPORTED
* @retval          CRYPTO_DET_NULL_PTR:             CRYPTO_E_PARAM_POINTER should be reported to DET
* @retval          CRYPTO_DET_INVALID_PARAM:        CRYPTO_E_PARAM_HANDLE  should be reported to DET
* @retval          CRYPTO_DET_INVALID_LEN:          CRYPTO_E_PARAM_VALUE   should be reported to DET
*/
static inline Crypto_ProcessJobErrorType Crypto_GetJobErrorForOutAndVerifyPtrs
(
    const Crypto_ServiceInfoType                    eJobService,
    const Crypto_JobPrimitiveInputOutputType* const pJobPrimitiveInputOutput,
    const Crypto_OperationModeType                  eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    ,const Crypto_JobRedirectionInfoType*           pJobRedir
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
)
{
    Crypto_ProcessJobErrorType eRetVal;
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT))
    uint32                     u32TargetKeyIdx;
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT)) */

    eRetVal = Crypto_GetJobErrorForOutputPtr(
                                              eJobService,
                                              pJobPrimitiveInputOutput,
                                              eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
                                              ,pJobRedir
#endif /* #if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
                                             );

    if (CRYPTO_NO_DET_RET_OK == eRetVal)
    {
        eRetVal = Crypto_GetJobErrorForSecondaryOutputPtr(
                                                           eJobService,
                                                           pJobPrimitiveInputOutput,
                                                           eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
                                                          ,pJobRedir
#endif /* #if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
                                                         );

        if (CRYPTO_NO_DET_RET_OK == eRetVal)
        {
            eRetVal = Crypto_GetJobErrorForVerifyPtr(
                                                      eJobService,
                                                      pJobPrimitiveInputOutput,
                                                      eJobMode
                                                     );

#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT))
            /* For some KeyManagement services realized through Crypto_ProcessJob(), the TargetKeyId should be checked for validity */
            if ((CRYPTO_NO_DET_RET_OK == eRetVal) && (TRUE == Crypto_aProcessJobServiceParams[(uint8)eJobService].bCheckTargetKeyId))
            {
                u32TargetKeyIdx = Crypto_Util_GetKeyIdx(pJobPrimitiveInputOutput->targetCryIfKeyId);
                if (CRYPTO_INVALID_KEY_IDX_U32 == u32TargetKeyIdx)
                {
                    eRetVal = CRYPTO_DET_INVALID_PARAM;
                }
            }
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT)) */
        }
    }

    return eRetVal;
}

/**
* @brief           Verify if certain parameters of one job are incorrect (null ptr, invalid length)
* @details         Verify if certain parameters of one job are incorrect (null ptr, invalid length)
*
* @param[in]       pJob - Pointer to the configuration of the job. Contains structures with job and primitive relevant information but also pointer to result buffers.
*
* @return          Result of the operation
* @retval          CRYPTO_NO_DET_RET_OK:            No error to report to DET, the function should return E_OK
* @retval          CRYPTO_NO_DET_RET_NOK:           No error to report to DET, the function should return E_NOT_OK
* @retval          CRYPTO_NO_DET_RET_KEY_NOT_VALID: No error to report to DET, the function should return CRYPTO_E_KEY_NOT_VALID
* @retval          CRYPTO_NO_DET_RET_NOT_SUPPORTED: No error to report to DET, the function should return CRYPTO_E_NOT_SUPPORTED
* @retval          CRYPTO_DET_NULL_PTR:             CRYPTO_E_PARAM_POINTER should be reported to DET
* @retval          CRYPTO_DET_INVALID_PARAM:        CRYPTO_E_PARAM_HANDLE  should be reported to DET
* @retval          CRYPTO_DET_INVALID_LEN:          CRYPTO_E_PARAM_VALUE   should be reported to DET
*/
static inline Crypto_ProcessJobErrorType Crypto_GetProcessJobError
(
    const Crypto_JobType* const pJob
)
{
    const Crypto_ServiceInfoType                    eJobService              = pJob->jobPrimitiveInfo->primitiveInfo->service;
    const Crypto_JobPrimitiveInputOutputType* const pJobPrimitiveInputOutput = &pJob->jobPrimitiveInputOutput;
    const Crypto_OperationModeType                  eJobMode                 = pJobPrimitiveInputOutput->mode;
    Crypto_ProcessJobErrorType                      eRetVal;
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    const Crypto_JobRedirectionInfoType*            pJobRedir                = pJob->jobRedirectionInfoRef;
#endif /* #if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */

    eRetVal = Crypto_GetJobErrorForService(
                                           eJobService,
                                           pJobPrimitiveInputOutput,
                                           eJobMode
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
                                           ,pJobRedir
#endif /* #if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */
                                           );

    return eRetVal;
}

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
/**
* @brief           Verify if a certain primitive is supported by a crypto driver object
* @details         Verify if a certain primitive is supported by a crypto driver object specified by u32objectIndex
*
* @param[in]       u32ObjectId - Holds the identifier of the Crypto Driver Object.
* @param[in]       pJob - Pointer to the configuration of the job. Contains structures with job and primitive relevant information but also pointer to result buffers.
*
* @return          E_OK:     Request successful, primitive found
*                  E_NOT_OK: Request failed, primitive not found
*
* @pre
*
*/
static inline Std_ReturnType Crypto_VerifyPrimitive
(
    uint32                u32ObjectId,
    const Crypto_JobType* pJob
)
{
    Std_ReturnType                      RetVal            = (Std_ReturnType)E_NOT_OK;
    const Crypto_PrimitiveInfoType*     pJobPrimitiveInfo = pJob->jobPrimitiveInfo->primitiveInfo;
    uint32                              u32Counter;
    const Crypto_PrimitiveType*         pCryptoKeyPrimitive;
    /* Array containing mapping between Crypto driver primitives and CSM job services */
    static const Crypto_ServiceInfoType aCryptoPrimitiveToServiceMapping[] =
    {
        CRYPTO_MACVERIFY,                /* This value is never used, there is no Crypto primitive with value 0    */
        CRYPTO_MACVERIFY,                /* Position corresponding to MAC_VERIFY         primitive with ID = 0x01U */
        CRYPTO_MACGENERATE,              /* Position corresponding to MAC_GENERATE       primitive with ID = 0x02U */
        CRYPTO_SIGNATUREVERIFY,          /* Position corresponding to SIGNATURE_VERIFY   primitive with ID = 0x03U */
        CRYPTO_SIGNATUREGENERATE,        /* Position corresponding to SIGNATURE_GENERATE primitive with ID = 0x04U */
        CRYPTO_ENCRYPT,                  /* Position corresponding to ENCRYPT            primitive with ID = 0x05U */
        CRYPTO_DECRYPT,                  /* Position corresponding to DECRYPT            primitive with ID = 0x06U */
        CRYPTO_AEADENCRYPT,              /* Position corresponding to AEAD_ENCRYPT       primitive with ID = 0x07U */
        CRYPTO_AEADDECRYPT,              /* Position corresponding to AEAD_DECRYPT       primitive with ID = 0x08U */
        CRYPTO_HASH,                     /* Position corresponding to HASH               primitive with ID = 0x09U */
        CRYPTO_RANDOMGENERATE            /* Position corresponding to RANDOM             primitive with ID = 0x0AU */
    };

    if (pJobPrimitiveInfo->service < CRYPTO_RANDOMSEED)
    {
        for (u32Counter = 0U; u32Counter < Crypto_aDriverObjectList[u32ObjectId].u32NoCryptoPrimitives; u32Counter++)
        {
            pCryptoKeyPrimitive = &Crypto_aDriverObjectList[u32ObjectId].pCryptoKeyPrimitives[u32Counter];

            if (
                (pJobPrimitiveInfo->service                 == aCryptoPrimitiveToServiceMapping[(uint8)(pCryptoKeyPrimitive->eService)]) && \
                ((uint8)pJobPrimitiveInfo->algorithm.family == pCryptoKeyPrimitive->u8AlgoFamily)                                  && \
                ((uint8)pJobPrimitiveInfo->algorithm.mode   == pCryptoKeyPrimitive->u8AlgoMode)
               )
            {
                RetVal = (Std_ReturnType)E_OK;
                break;
            }
        }
    }
#if (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT)
    else if ((uint8)(pJobPrimitiveInfo->service) < CRYPTO_NUM_SUPPORTED_SERVICES_U8)
    {
        /* This is the case of the Key Management services accessible through Crypto_ProcessJob() API */
        RetVal = (Std_ReturnType)E_OK;
    }
    else
    {
        /* Do nothing. Added to avoid MISRA violation */
    }
#endif /* (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT) */

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */

#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT))
/**
* @brief           Handles the callbacks to the application requesting read/write information to/from Nvram
* @details         Handles the callbacks to the application requesting read/write information to/from Nvram
*
* @param[in]       u32PartitionId  - Id of the partition the call is run on
* @param[in]       eNvramOperation - Type of the Nvram operation to be performed: read or write
* @param[in]       u32BlobId       - Identifier of the blob to be written/read from/to Nvmram
* @param[in]       u8ServiceId     - Indetifier of the Crypto API requesting the Nvram operation. Used to report DET error in case something goes wrong
*
* @return          E_OK         : Request successful.
*                  E_NOT_OK     : Request failed.
*                  CRYPTO_E_BUSY: Request failed, Crypro Driver Object is busy.
*
*
* @pre
*
*/
static Std_ReturnType Crypto_HandleNvramInfo
(
    const uint32              u32PartitionId,
    Crypto_NvramOperationType eNvramOperation,
    uint32                    u32BlobId,
    uint8                     u8ServiceId
)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;

    if (CRYPTO_NVRAM_OP_READ == eNvramOperation)
    {
        if (CRYPTO_NVRAM_BLOB_0_ID == u32BlobId)
        {
            RetVal = Crypto_Ipw_RestoreKeyValidInfoFromNvram(u32PartitionId);
        }
#if (0U != CRYPTO_SIZEOF_NVRAM_BLOB_1)
        else if (CRYPTO_NVRAM_BLOB_1_ID == u32BlobId)
        {
            RetVal = (Std_ReturnType)E_OK;
        }
        else
        {
            /* Should never get here */
        }
#endif /* (0U != CRYPTO_SIZEOF_NVRAM_BLOB_1) */
    }
    else if (CRYPTO_NVRAM_OP_WRITE == eNvramOperation)
    {
        if (CRYPTO_NVRAM_BLOB_0_ID == u32BlobId)
        {
            /* Notify the upper layer that the information in the NVRAM blob 0 has been updated */
            RetVal = pfCryptoUpdateNvramBlobHandler(CRYPTO_NVRAM_BLOB_0_ID, CRYPTO_SIZEOF_NVRAM_BLOB_0);
        }
#if (0U != CRYPTO_SIZEOF_NVRAM_BLOB_1)
        else if (CRYPTO_NVRAM_BLOB_1_ID == u32BlobId)
        {
            /* Notify the upper layer that the information in the NVRAM blob 1 has been updated */
            RetVal = pfCryptoUpdateNvramBlobHandler(CRYPTO_NVRAM_BLOB_1_ID, CRYPTO_SIZEOF_NVRAM_BLOB_1);
        }
        else
        {
            /* Should never get here */
        }
#endif /* (0U != CRYPTO_SIZEOF_NVRAM_BLOB_1) */
    }
    else
    {
        /* Should never get here */
    }

    if ((Std_ReturnType)E_OK != RetVal)
    {
        (void) Det_ReportRuntimeError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, u8ServiceId, (uint8)CRYPTO_E_RE_NVRAM_OPERATION_FAIL);
    }

    return RetVal;
}
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)) */


#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
/**
* @brief           Checks if call is done from a valid core and driver is initialized
* @details         Checks if call is done from a valid core and driver is initialized
*
* @param[in]       u32ObjectIdx - Idx of the Crypto Driver Object
* @param[in]       u8ApiId      - Id of the caller API
*
* @return          E_OK:     Request successful
*                  E_NOT_OK: Request failed
*/
static Std_ReturnType Crypto_CheckValidCoreAndDrvInitForCdoApis
(
    const uint32 u32PartitionId,
    const uint32 u32ObjectIdx,
    const uint8  u8ApiId
)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == CRYPTO_ENABLE_MULTICORE_SUPPORT)
    /* Check if the partition this call is coming from is valid */
    if (Crypto_aDriverObjectList[u32ObjectIdx].u32ValidPartitionId != u32PartitionId)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, u8ApiId, (uint8)CRYPTO_E_PARAM_CONFIG);
    }
    /* Check whether the Crypto driver is in CRYPTO_DRIVER_UNINIT state */
    else
#else
    /*Avoid compiler warnings*/
    (void)u32ObjectIdx;
#endif /* (STD_ON == CRYPTO_ENABLE_MULTICORE_SUPPORT) */
    {
        if (CRYPTO_DRIVER_UNINIT == Crypto_aeDriverState[u32PartitionId])
        {
            (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, u8ApiId, (uint8)CRYPTO_E_UNINIT);
        }
        else
        {
            RetVal = (Std_ReturnType)E_OK;
        }
    }

    return RetVal;
}

#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT)) */

#if (STD_ON == CRYPTO_KEYS_EXIST)

/**
* @brief           Validates the parameters used by the Crypto_RandomSeed() API
* @details         Validates the parameters used by the Crypto_RandomSeed() API
*
* @param[in]       u32CryptoKeyIdx - Index of the Crypto key
*
* @return          E_OK:               Request successful
*                  E_NOT_OK:           Request failed
*                  CRYPTO_E_KEY_EMPTY: Key is empty
*/
static inline Std_ReturnType Crypto_GetNonDetErrorsForRandomSeed
(
    const uint32 u32CryptoKeyIdx
)
{
    Std_ReturnType RetVal                = (Std_ReturnType)E_NOT_OK;
    const uint32   u32CryptoSeedStateIdx = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_RANDOM_SEED_STATE);

    if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoSeedStateIdx)
    {
        RetVal = (Std_ReturnType)E_OK;
    }

    return RetVal;
}

#if (STD_ON == CRYPTO_SPT_KEY_GENERATE)
/**
* @brief           Validates the parameters used by the Crypto_KeyGenerate() API
* @details         Validates the parameters used by the Crypto_KeyGenerate() API
*
* @param[in]       u32CryptoKeyIdx - Index of the Crypto key
*
* @return          E_OK:               Request successful
*                  E_NOT_OK:           Request failed
*                  CRYPTO_E_KEY_EMPTY: Key is empty
*/
static Std_ReturnType Crypto_GetNonDetErrorsForKeyGenerate
(
    const uint32 u32CryptoKeyIdx
)
{
    Std_ReturnType RetVal                      = (Std_ReturnType)E_NOT_OK;
    const uint32   u32CryptoKeyElementIdx      = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KEY_MATERIAL_U32);
    const uint32   u32AlgoFamKeyElemIdx        = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYGENERATE_ALGORITHM);
    const uint32   u32CryptoKeyBaseExchangeIdx = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYEXCHANGE_BASE);
    const uint32   u32CurveIdKeyElemIdx        = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYGENERATE_CURVETYPE);

    /* Request failed, source key element is uninitialized */
    if (
        ((CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoKeyBaseExchangeIdx) && (0U == *Crypto_aKeyElementList[u32CryptoKeyBaseExchangeIdx].pu32CryptoElementActualSize)) || \
        ((CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CurveIdKeyElemIdx) && (0U == *Crypto_aKeyElementList[u32CurveIdKeyElemIdx].pu32CryptoElementActualSize)) || \
        ((CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32AlgoFamKeyElemIdx) && (0U == *Crypto_aKeyElementList[u32AlgoFamKeyElemIdx].pu32CryptoElementActualSize))
       )
    {
        RetVal = (Std_ReturnType)CRYPTO_E_KEY_EMPTY;
    }
    else if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoKeyElementIdx)
    {
        RetVal = (Std_ReturnType)E_OK;
    }
    else
    {
        /* Do nothing*/
    }

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_SPT_KEY_GENERATE) */

#if (STD_ON == CRYPTO_SPT_KEY_DERIVE)
/**
* @brief           Validates the parameters used by the Crypto_KeyDerive() API
* @details         Validates the parameters used by the Crypto_KeyDerive() API
*
* @param[in]       u32CryptoKeyIdx       - Index of the Crypto key
* @param[in]       u32CryptoTargetKeyIdx - Index of the target Crypto key
*
* @return          E_OK:               Request successful
*                  E_NOT_OK:           Request failed
*                  CRYPTO_E_KEY_EMPTY: Key is empty
*/
static Std_ReturnType Crypto_GetNonDetErrorsForKeyDerive
(
    const uint32 u32CryptoKeyIdx,
    const uint32 u32CryptoTargetKeyIdx
)
{
#if (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT)
    uint32         u32TargetMasterKeyElemIdx;
    uint32         u32TargetOutputSizeKeyElemIdx;
    uint32         u32TargetOutputDataKeyElemIdx;
#endif /* (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT) */
#if ((STD_ON == CRYPTO_SPT_PBKDF2) || (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT))
    uint8          u8KeyDeriveAlgo;
#endif /* ((STD_ON == CRYPTO_SPT_PBKDF2) || (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT)) */
    Std_ReturnType RetVal                      = (Std_ReturnType)E_OK;
    const uint32   u32PasswordKeyElemIdx       = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYDERIVATION_PASSWORD);
    const uint32   u32DerivAlgoKeyElemIdx      = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYDERIVATION_ALGORITHM);
    const uint32   u32DerivSaltKeyElemIdx      = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYDERIVATION_SALT);
#if (STD_ON == CRYPTO_SPT_PBKDF2)
    const uint32   u32DerivIterationKeyElemIdx = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYDERIVATION_ITERATIONS);
#endif /* (STD_ON == CRYPTO_SPT_PBKDF2) */
    const uint32   u32TargetPasswordKeyElemIdx = Crypto_Util_GetKeyElementIdx(u32CryptoTargetKeyIdx, CRYPTO_KE_KEYDERIVATION_PASSWORD);
    const uint32   u32DerivSecondAlgoKeyElemIdx  = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYDERIVATION_ALGORITHM_SECONDARY);

    if ((CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32DerivSecondAlgoKeyElemIdx)  &&
        (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32PasswordKeyElemIdx)         &&
        (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32DerivAlgoKeyElemIdx)        &&
        (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32DerivSaltKeyElemIdx)        &&
        (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32TargetPasswordKeyElemIdx)
       )
    {
#if ((STD_ON == CRYPTO_SPT_PBKDF2) || (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT))
        u8KeyDeriveAlgo = Crypto_aKeyElementList[u32DerivAlgoKeyElemIdx].pCryptoElementArray[CRYPTO_KEY_DERIVE_ALGORITHM_IDX];
#endif /* ((STD_ON == CRYPTO_SPT_PBKDF2) || (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT)) */

        if ((0U == *Crypto_aKeyElementList[u32PasswordKeyElemIdx].pu32CryptoElementActualSize))
        {
            /* Request failed, source key element is uninitialized */
            RetVal = (Std_ReturnType)CRYPTO_E_KEY_EMPTY;
            if ((hseKeyType_t)HSE_KEY_TYPE_SHARED_SECRET == Crypto_aKeyElementList[u32PasswordKeyElemIdx].u8HseKeyType)
            {
                RetVal = (Std_ReturnType)E_OK;
            }
        }
#if (STD_ON == CRYPTO_SPT_PBKDF2)
        if (((Std_ReturnType)E_OK == RetVal) && ((uint8)CRYPTO_ALGOFAM_PBKDF2 == u8KeyDeriveAlgo))
        {
            if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 == u32DerivIterationKeyElemIdx)
            {
                RetVal = (Std_ReturnType)E_NOT_OK;
            }
        }
#endif /* (STD_ON == CRYPTO_SPT_PBKDF2) */
#if (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT)
        if (((Std_ReturnType)E_OK == RetVal) && ((CRYPTO_ALGOFAM_TLS12_GENERATE_MASTER <= u8KeyDeriveAlgo) && (u8KeyDeriveAlgo <= CRYPTO_ALGOFAM_TLS12_GENERATE_VERIFY_SERVER)))
        {
            /* In the TLS 1.2 derivation case, we should check the presence of other key elements too */
            u32TargetMasterKeyElemIdx     = Crypto_Util_GetKeyElementIdx(u32CryptoTargetKeyIdx, CRYPTO_KE_KEYDERIVATION_KEY_MASTER);
            u32TargetOutputSizeKeyElemIdx = Crypto_Util_GetKeyElementIdx(u32CryptoTargetKeyIdx, CRYPTO_KE_KEYDERIVATION_OUTPUT_SIZE);
            u32TargetOutputDataKeyElemIdx = Crypto_Util_GetKeyElementIdx(u32CryptoTargetKeyIdx, CRYPTO_KE_KEYDERIVATION_OUTPUT_DATA);

             if ((CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32TargetMasterKeyElemIdx)     &&
                 (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32TargetOutputSizeKeyElemIdx) &&
                 (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32TargetOutputDataKeyElemIdx)
                )
            {
                switch (u8KeyDeriveAlgo)
                {
                    case CRYPTO_ALGOFAM_TLS12_KEY_EXPANSION:
                        /* Must assure that the OUTPUT_DATA key element is large enough to store the value requested by the application through the values in the CRYPTO_KE_KEYDERIVATION_OUTPUT_SIZE key element */
                        if (Crypto_aKeyElementList[u32TargetOutputDataKeyElemIdx].u32CryptoKeyElementMaxSize < Crypto_aKeyElementList[u32TargetOutputSizeKeyElemIdx].pCryptoElementArray[CRYPTO_TLS12_IV_LENGTH_IDX])
                        {
                            RetVal = (Std_ReturnType)CRYPTO_E_SMALL_BUFFER;
                        }
                        break;

                    case CRYPTO_ALGOFAM_TLS12_GENERATE_VERIFY_CLIENT:
                    /* fall-through */
                    case CRYPTO_ALGOFAM_TLS12_GENERATE_VERIFY_SERVER:
                        /* Must assure that the OUTPUT_DATA key element is large enough to store the value requested by the application through the values in the CRYPTO_KE_KEYDERIVATION_OUTPUT_SIZE key element */
                        if (Crypto_aKeyElementList[u32TargetOutputDataKeyElemIdx].u32CryptoKeyElementMaxSize < Crypto_aKeyElementList[u32TargetOutputSizeKeyElemIdx].pCryptoElementArray[CRYPTO_TLS12_VERIFY_DATA_LENGTH_IDX])
                        {
                            RetVal = (Std_ReturnType)CRYPTO_E_SMALL_BUFFER;
                        }
                        break;

                    default:
                        /* Code cannot actually get here */
                        break;
                }
            }
            else
            {
                RetVal = (Std_ReturnType)E_NOT_OK;
            }
        }
#endif /* (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT) */
    }
    else
    {
        RetVal = (Std_ReturnType)E_NOT_OK;
    }

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_SPT_KEY_DERIVE) */

/**
* @brief           Validates the parameters used by the Crypto_KeyExchangeCalcPubVal() API
* @details         Validates the parameters used by the Crypto_KeyExchangeCalcPubVal() API
*
* @param[in]       u8ServiceId              - Service id of the caller function
* @param[in]       u32CryptoKeyIdx          - Index of the Crypto key
* @param[in]       pu8PublicValuePtr        - Pointer to location where public key will be stored
* @param[in]       pu32PublicValueLengthPtr - Pointer to the length of the public key
*
* @return          E_OK:               Request successful
*                  E_NOT_OK:           Request failed
*                  CRYPTO_E_KEY_EMPTY: Key is empty
*/
static Std_ReturnType Crypto_GetNonDetErrorsForKeyExchCalcPubVal
(
    const uint8         u8ServiceId,
    const uint32        u32CryptoKeyIdx,
    const uint8*  const pu8PublicValuePtr,
    const uint32* const pu32PublicValueLengthPtr
)
{
    Std_ReturnType RetVal                 = (Std_ReturnType)E_NOT_OK;
    const uint32   u32CryptoKeyElementIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY);

    if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoKeyElementIdx)
    {
        /* If the buffer pu8PublicValuePtr is too small to store the result of the request, CRYPTO_E_SMALL_BUFFER shall be returned and the function shall additionally report the runtime error CRYPTO_E_RE_SMALL_BUFFER. */
        if (*pu32PublicValueLengthPtr < Crypto_aKeyElementList[u32CryptoKeyElementIdx].u32CryptoKeyElementMaxSize)
        {
            if ((uint8)CRYPTO_KEYEXCHANGECALCPUBVAL_ID == u8ServiceId)
            {
                (void)Det_ReportRuntimeError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, CRYPTO_KEYEXCHANGECALCPUBVAL_ID, (uint8)CRYPTO_E_RE_SMALL_BUFFER);
            }
            RetVal = (Std_ReturnType)CRYPTO_E_SMALL_BUFFER;
        }
        else
        {
            RetVal = (Std_ReturnType)E_OK;
        }
    }
    /*Avoid compiler warnings*/
    (void)pu8PublicValuePtr;

    return RetVal;
}

/**
* @brief           Export public key.
* @details         Exports the public key from CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY and place it in pu8PublicValuePtr.
*
* @param[in]       u32CryptoKeyIdx           - Index of the Crypto key
* @param[in]       pu8PublicValuePtr         - Pointer to location where public key will be stored
* @param[in]       pu32PublicValueLengthPtr  - Pointer to the length of the public key
*
* @return          E_OK         : Request successful.
*                  E_NOT_OK     : Request failed.
*                  CRYPTO_E_BUSY: Request failed, Crypro Driver Object is busy.
*/
static Std_ReturnType Crypto_CopyDataFromPubValKeyElementToUser
(
    const uint32  u32PartitionId,
    const uint32  u32CryptoKeyIdx,
#if ((STD_ON == CRYPTO_SPT_SYM_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_RSA_KEY_EXPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT))
    uint8* const  pu8PublicValuePtr,
#else
    const uint8*  pu8PublicValuePtr,
#endif
    uint32* const pu32PublicValueLengthPtr
)
{
    Std_ReturnType RetVal;
    const uint32   u32CryptoKeyElementIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY);

    /* Export the ECC public key that later will be used to generate shared secret */
    RetVal = Crypto_Ipw_ExportKey(u32PartitionId, u32CryptoKeyElementIdx, CRYPTO_INVALID_KEY_ELEMENT_IDX_U32, pu8PublicValuePtr, pu32PublicValueLengthPtr);

    return RetVal;
}

/**
* @brief           Checks if there are errors that do not need to be reported to DET in the validation of parameters for Crypto_KeyExchangeCalcSecret() API
* @details         Checks if there are errors that do not need to be reported to DET in the validation of parameters for Crypto_KeyExchangeCalcSecret() API
*
* @param[in]       u32CryptoKeyIdx          - Index of the Crypto key
*
* @return          E_OK:               Request successful
*                  E_NOT_OK:           Request failed
*/
static Std_ReturnType Crypto_GetNonDetErrorsForKeyExchCalcSecret
(
    const uint32 u32CryptoKeyIdx
)
{
    Std_ReturnType RetVal                       = (Std_ReturnType)E_NOT_OK;
    const uint32   u32CryptoKeyElementPrivIdx   = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYEXCHANGE_PRIVKEY);
    const uint32   u32CryptoKeyElementPubIdx    = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY);
    const uint32   u32CryptoKeyElementSharedIdx = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx, CYRPTO_KE_KEYEXCHANGE_SHAREDVALUE);

    if ((CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoKeyElementPrivIdx) &&
       (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoKeyElementPubIdx)  &&
       (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoKeyElementSharedIdx)
       )
    {
        RetVal = (Std_ReturnType)E_OK;
    }

    return RetVal;
}

#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    /**
* @brief           Checks if redirection is configured
* @details         Checks redirection configuration based on key id and key element id
*
* @param[in]       u32RedirectedKeyId        - Input key id
* @param[in]       u32RedirectedKeyElementId - Input key element id
*
* @return          E_OK:                Request successful
*                  E_NOT_OK:            Request failed
*/
static Crypto_ProcessJobErrorType Crypto_CheckRedirectionConfig
(
    uint32 u32RedirectedKeyId,
    uint32 u32RedirectedKeyElementId
)
{
    uint32                     u32CryptoKeyIdx;
    uint32                     u32CryptoKeyElementIdx;
    Crypto_ProcessJobErrorType eRetVal = CRYPTO_NO_DET_RET_NOK;

    if (CRYPTO_KEY_MATERIAL_U32 != u32RedirectedKeyElementId)
    {
        u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(u32RedirectedKeyId);

        if (CRYPTO_INVALID_KEY_IDX_U32 != u32CryptoKeyIdx)
        {
            if (!Crypto_IsKeyValidFlagSet(u32CryptoKeyIdx))
            {
                eRetVal = CRYPTO_NO_DET_RET_KEY_NOT_VALID;
            }
            else
            {
                u32CryptoKeyElementIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, u32RedirectedKeyElementId);

                if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoKeyElementIdx)
                {
                    eRetVal = CRYPTO_NO_DET_RET_OK;
                }
                else
                {
                     eRetVal = CRYPTO_DET_INVALID_PARAM;
                }
            }
        }
        else
        {
             eRetVal = CRYPTO_DET_INVALID_PARAM;
        }
    }
    else
    {
        eRetVal = CRYPTO_NO_DET_RET_NOT_SUPPORTED;
    }

    return eRetVal;
}
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT)) */

/**
* @brief           Marks a key as valid
* @details         Marks a key as valid
*
* @param[in]       u32CryptoKeyIdx - Index of the Crypto key
* @param[in]       u8ServiceId     - Service Id of the caller
*
* @return          E_OK         : Request successful
*                  E_NOT_OK     : Request failed
*                  CRYPTO_E_BUSY: Request failed, Crypro Driver Object is busy.
*/
static Std_ReturnType Crypto_MarkKeyAsValid
(
    const uint32 u32PartitionId,
    const uint32 u32CryptoKeyIdx,
    const uint8  u8ServiceId
)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;

    /* Set the key as being valid */
    Crypto_SetKeyValidFlag(u32CryptoKeyIdx);
#if (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)
    RetVal = Crypto_HandleNvramInfo(u32PartitionId, CRYPTO_NVRAM_OP_WRITE, CRYPTO_NVRAM_BLOB_0_ID, u8ServiceId);
#else
    /*Avoid compiler warnings*/
    (void)u32PartitionId;
    (void)u8ServiceId;
#endif /* (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT) */

    return RetVal;
}

#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

#if (STD_ON == CRYPTO_KEYS_EXIST)
/**
* @brief            Check that the keys are valid for copying.
* @details          Check that the source key and target key are valid for copying.
*
* @param[in]        u32CryptoKeyIdx    Holds the identifier of the key whose key element shall be the source element
* @param[in]        u32TargetKeyIdx    Holds the identifier of the key whose key element shall be the destination element
*
* @return           Result of the operation
* @retval           E_OK                        Keys are valid for copying
* @retval           Other values of E_OK        Keys are not valid for copying
*
*
* @pre
*
*/
static Std_ReturnType Crypto_CheckKeyCopyValid
(
    uint32 u32CryptoKeyIdx,
    uint32 u32TargetKeyIdx
)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;
    uint32         u32Counter;
    boolean        bFound;

    for (u32Counter = 0U; u32Counter < Crypto_aKeyList[u32CryptoKeyIdx].u32NumCryptoKeyElements; u32Counter++)
    {
        if (CRYPTO_RA_DENIED == Crypto_aKeyElementList[Crypto_aKeyList[u32CryptoKeyIdx].pCryptoKeyElementList[u32Counter]].eCryptoKeyElementReadAccess)
        {
            RetVal = (Std_ReturnType)CRYPTO_E_KEY_READ_FAIL;
        }
        else if (0U == *Crypto_aKeyElementList[Crypto_aKeyList[u32CryptoKeyIdx].pCryptoKeyElementList[u32Counter]].pu32CryptoElementActualSize)
        {
            if ((hseKeyType_t)HSE_KEY_TYPE_SHARED_SECRET != Crypto_aKeyElementList[Crypto_aKeyList[u32CryptoKeyIdx].pCryptoKeyElementList[u32Counter]].u8HseKeyType)
            {
                RetVal = (Std_ReturnType)CRYPTO_E_KEY_EMPTY;
            }
        }
        else
        {
            /* Do nothing*/
        }
        bFound = FALSE;
        if ((Std_ReturnType)E_OK == RetVal)
        {
            RetVal = Crypto_CheckKeyCopyValidKeyElements(u32CryptoKeyIdx, u32Counter, u32TargetKeyIdx, &bFound);
        }
        /* If no compatible key element was found, return */
        if ((FALSE == bFound) && ((Std_ReturnType)E_OK == RetVal))
        {
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
        /* If any errors occured, get out */
        if ((Std_ReturnType)E_OK != RetVal)
        {
            break;
        }
    }
    return RetVal;
}

/**
* @brief            Check that the keys elements are valid for copying.
* @details          Check that the source key elements and target key elements are valid for copying.
*
* @param[in]        u32CryptoKeyIdx         Holds the identifier of the key whose key element shall be the source element
* @param[in]        u32CryptoKeyElemIdx     Holds the identifier of the key elements in the source element that must be checked
* @param[in]        u32TargetKeyIdx         Holds the identifier of the key whose key element shall be the destination element
* @param[out]       pbFound                 Out variable used by the function to state that a match between key elements in source and destination keys has been found
*
* @return           Result of the operation
* @retval           E_OK                        Key elements are valid for copying
* @retval           Other values of E_OK        Key elements are not valid for copying
*
*
* @pre
*
*/
static Std_ReturnType Crypto_CheckKeyCopyValidKeyElements
(
    uint32   u32CryptoKeyIdx,
    uint32   u32CryptoKeyElemIdx,
    uint32   u32TargetKeyIdx,
    boolean* pbFound
)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;
    uint32         u32Counter;

    for (u32Counter = 0U; u32Counter < Crypto_aKeyList[u32TargetKeyIdx].u32NumCryptoKeyElements; u32Counter++)
    {
        if (Crypto_aKeyElementList[Crypto_aKeyList[u32CryptoKeyIdx].pCryptoKeyElementList[u32CryptoKeyElemIdx]].u32CryptoKeyElementId ==  \
            Crypto_aKeyElementList[Crypto_aKeyList[u32TargetKeyIdx].pCryptoKeyElementList[u32Counter]].u32CryptoKeyElementId
            )
        {
            *pbFound = TRUE;
            if (CRYPTO_WA_DENIED == Crypto_aKeyElementList[Crypto_aKeyList[u32TargetKeyIdx].pCryptoKeyElementList[u32Counter]].eCryptoKeyElementWriteAccess)
            {
                RetVal = (Std_ReturnType)CRYPTO_E_KEY_WRITE_FAIL;
            }
            else if (
                      (*Crypto_aKeyElementList[Crypto_aKeyList[u32CryptoKeyIdx].pCryptoKeyElementList[u32CryptoKeyElemIdx]].pu32CryptoElementActualSize >  \
                        Crypto_aKeyElementList[Crypto_aKeyList[u32TargetKeyIdx].pCryptoKeyElementList[u32Counter]].u32CryptoKeyElementMaxSize     \
                      )    \
                        || \
                     (
                      (*Crypto_aKeyElementList[Crypto_aKeyList[u32CryptoKeyIdx].pCryptoKeyElementList[u32CryptoKeyElemIdx]].pu32CryptoElementActualSize != \
                        Crypto_aKeyElementList[Crypto_aKeyList[u32TargetKeyIdx].pCryptoKeyElementList[u32Counter]].u32CryptoKeyElementMaxSize) && \
                       ((boolean)FALSE == Crypto_aKeyElementList[Crypto_aKeyList[u32TargetKeyIdx].pCryptoKeyElementList[u32Counter]].bCryptoKeyElementAllowPartialAccess)
                     )
                   )
            {
                RetVal = (Std_ReturnType)CRYPTO_E_KEY_SIZE_MISMATCH;
            }
            else if (CRYPTO_KEY_MATERIAL_U32 == Crypto_aKeyElementList[Crypto_aKeyList[u32CryptoKeyIdx].pCryptoKeyElementList[u32CryptoKeyElemIdx]].u32CryptoKeyElementId)
            {
                /* If the key element is key material feature is not supported because intermediary buffer is needed*/
                if ((hseKeyType_t)HSE_KEY_TYPE_SHARED_SECRET != Crypto_aKeyElementList[Crypto_aKeyList[u32CryptoKeyIdx].pCryptoKeyElementList[u32CryptoKeyElemIdx]].u8HseKeyType)
                {
                    RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;
                }
            }
            else
            {
                /* Do nothing */
            }
        }
        /* Checking if an error was reported, if so exit the loop */
        if ((Std_ReturnType)E_OK != RetVal)
        {
            break;
        }
    }

    return RetVal;
}

/**
* @brief            Check that the parameters and configuration are valid for copying.
* @details          Check that the Key Element Id, Target Key Element Id and Crypto Key Element configuration are valid for copying.
*
* @param[in]        u32CryptoKeyElementIdx          Holds the index of the Crypto Key Element who shall be the source element
* @param[in]        u32CryptoTargetKeyElementIdx    Holds the index of the Crypto Key Element who shall be the destination element
*
* @return           Result of the operation
* @retval           E_OK                        Crypto Key Elements are valid for copying
* @retval           Other values of E_OK        Crypto Key Elements are not valid for copying
*
*
* @pre
*
*/
static inline Std_ReturnType Crypto_CheckParamAndConfForKeyElementCopy
(
    const uint32 u32CryptoKeyElementIdx,
    const uint32 u32CryptoTargetKeyElementIdx
)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;

    if ((CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoKeyElementIdx) && (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoTargetKeyElementIdx))
    {
        /* Request failed, not allowed to extract key element */
        if (CRYPTO_RA_DENIED == Crypto_aKeyElementList[u32CryptoKeyElementIdx].eCryptoKeyElementReadAccess)
        {
            RetVal = (Std_ReturnType)CRYPTO_E_KEY_READ_FAIL;
        }
        /* Request failed, not allowed to write key element */
        else if (CRYPTO_WA_DENIED == Crypto_aKeyElementList[u32CryptoTargetKeyElementIdx].eCryptoKeyElementWriteAccess)
        {
            RetVal = (Std_ReturnType)CRYPTO_E_KEY_WRITE_FAIL;
        }
        /* Incompatible sizes between key elements */
        else if (
                  (*Crypto_aKeyElementList[u32CryptoKeyElementIdx].pu32CryptoElementActualSize > Crypto_aKeyElementList[u32CryptoTargetKeyElementIdx].u32CryptoKeyElementMaxSize) || \
                  (
                    (*Crypto_aKeyElementList[u32CryptoKeyElementIdx].pu32CryptoElementActualSize != Crypto_aKeyElementList[u32CryptoTargetKeyElementIdx].u32CryptoKeyElementMaxSize) && \
                    ((boolean)FALSE == Crypto_aKeyElementList[u32CryptoTargetKeyElementIdx].bCryptoKeyElementAllowPartialAccess)
                  )
                 )
        {
            RetVal = (Std_ReturnType)CRYPTO_E_KEY_SIZE_MISMATCH;
        }
        /* Request failed, source key element is uninitialized */
        else if (0U == *Crypto_aKeyElementList[u32CryptoKeyElementIdx].pu32CryptoElementActualSize)
        {
            RetVal = (Std_ReturnType)CRYPTO_E_KEY_EMPTY;
        }
        else
        {
            /* Do nothing. RetVal is set to E_OK */
        }
    }
    else
    {
#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTCOPY_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    return RetVal;
}

/**
* @brief            Check that the parameters and configuration are valid for copying.
* @details          Check that the Key Element Id, Target KeyElement Id and Crypto Key Element configuration are valid for copying.
*
* @param[in]        u32CryptoKeyElementIdx          Holds the index of the Crypto Key Element who shall be the source element
* @param[in]        u32CryptoTargetKeyElementIdx    Holds the index of the Crypto Key Element who shall be the destination element
* @param[in]        u32KeyElementSourceOffset       This is the offset of the of the source key element indicating the start index of the copy operation.
* @param[in]        u32KeyElementTargetOffset       This is the offset of the of the target key element indicating the start index of the copy operation.
* @param[in]        u32KeyElementCopyLength         Specifies the number of bytes that shall be copied
*
* @return           Result of the operation
* @retval           E_OK                        Crypto Key Elements are valid for copying
* @retval           Other values of E_OK        Crypto Key Elements are not valid for copying
*
*
* @pre
*
*/
static inline Std_ReturnType Crypto_CheckParamAndConfForKeyElementCopyPartial
(
    const uint32 u32CryptoKeyElementIdx,
    const uint32 u32CryptoTargetKeyElementIdx,
    const uint32 u32KeyElementSourceOffset,
    const uint32 u32KeyElementTargetOffset,
    const uint32 u32KeyElementCopyLength
)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_NOT_OK;
    uint32         u32CryptoKeyElementMaxSize;
    uint32         u32TargetCryptoKeyElementMaxSize;
    uint32         u32CryptoKeyElementActualSize;

    if ((CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoKeyElementIdx) && (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoTargetKeyElementIdx))
    {
        u32CryptoKeyElementMaxSize          = Crypto_aKeyElementList[u32CryptoKeyElementIdx].u32CryptoKeyElementMaxSize;
        u32TargetCryptoKeyElementMaxSize    = Crypto_aKeyElementList[u32CryptoTargetKeyElementIdx].u32CryptoKeyElementMaxSize;
        u32CryptoKeyElementActualSize       = *Crypto_aKeyElementList[u32CryptoKeyElementIdx].pu32CryptoElementActualSize;

        /* Request failed, not allowed to extract key element */
        if (CRYPTO_RA_DENIED == Crypto_aKeyElementList[u32CryptoKeyElementIdx].eCryptoKeyElementReadAccess)
        {
            RetVal = (Std_ReturnType)CRYPTO_E_KEY_READ_FAIL;
        }
        /* Request failed, not allowed to write key element */
        else if (CRYPTO_WA_DENIED == Crypto_aKeyElementList[u32CryptoTargetKeyElementIdx].eCryptoKeyElementWriteAccess)
        {
            RetVal = (Std_ReturnType)CRYPTO_E_KEY_WRITE_FAIL;
        }
        /* Incompatible sizes between key elements */
        else if ((u32CryptoKeyElementMaxSize < (uint32)(u32KeyElementSourceOffset + u32KeyElementCopyLength)) ||  \
                  (u32TargetCryptoKeyElementMaxSize < (uint32)(u32KeyElementTargetOffset + u32KeyElementCopyLength))
                )
        {
            RetVal = (Std_ReturnType)CRYPTO_E_KEY_SIZE_MISMATCH;
        }
        /* Request failed, source key element is uninitialized */
        else if (0U == u32CryptoKeyElementActualSize)
        {
            RetVal = (Std_ReturnType)CRYPTO_E_KEY_EMPTY;
            if ((hseKeyType_t)HSE_KEY_TYPE_SHARED_SECRET == Crypto_aKeyElementList[u32CryptoKeyElementIdx].u8HseKeyType)
            {
               RetVal = (Std_ReturnType)E_OK;
            }
        }
        else
        {
            RetVal = (Std_ReturnType)E_OK;
        }
    }
#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
    else
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTCOPYPARTIAL_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    return RetVal;
}

/**
* @brief            Check that the parameters and configuration are valid.
* @details          Check that the parameters and configuration are valid for Crypto_KeyElementSet().
*
* @param[in]        u32PartitionId            Holds the Id of the current partition
* @param[in]        u32CryptoKeyIdx           Holds the index of the Crypto Key
* @param[in]        pu8Key                    Holds the pointer to the key data which shall be set as key element
* @param[in]        u32KeyLength              Holds the length of the key element in bytes
* @param[in]        u32KeyElementId           Holds the identifier of the key element which shall be set
* @param[out]       pu32CryptoKeyElementIdx   Holds the pointer to the index of the key element which shall be set
*
* @return           Result of the operation
* @retval           E_OK                        Service request is valid
* @retval           Other values of E_OK        Service request is not valid
*
*
* @pre
*
*/
static inline Std_ReturnType Crypto_CheckForErrorsInKeyElementSet
(
    const uint32 u32PartitionId,
    uint32       u32CryptoKeyIdx,
    const uint8* pu8Key,
    uint32       u32KeyLength,
    uint32       u32KeyElementId,
    uint32*      pu32CryptoKeyElementIdx
)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
    /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_KEYELEMENTSET_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    /* If u32CryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyElementSet shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    else if (CRYPTO_INVALID_KEY_IDX_U32 == u32CryptoKeyIdx)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTSET_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
    /* If the parameter pu8Key is a null pointer and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyElementSet shall report CRYPTO_E_PARAM_POINTER to the DET and return E_NOT_OK. */
    else if (NULL_PTR == pu8Key)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTSET_ID, (uint8)CRYPTO_E_PARAM_POINTER);
    }
    /* If u32KeyLength is zero and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyElementSet shall report CRYPTO_E_PARAM_VALUE to the DET and return E_NOT_OK. */
    else if (0U == u32KeyLength)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTSET_ID, (uint8)CRYPTO_E_PARAM_VALUE);
    }
    else
#else
    /* Avoid compiler warnings */
    (void)u32PartitionId;
    (void)pu8Key;
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        /* If parameter u32KeyElementId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyElementSet shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
        *pu32CryptoKeyElementIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, u32KeyElementId);

        if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != *pu32CryptoKeyElementIdx)
        {
            /* All verifications done until now have passed. Set RetVal to E_OK */
            RetVal = (Std_ReturnType)E_OK;

            /* If the key does not allow write access - request failed because write access was denied */
            if (CRYPTO_WA_DENIED == Crypto_aKeyElementList[*pu32CryptoKeyElementIdx].eCryptoKeyElementWriteAccess)
            {
                RetVal = (Std_ReturnType)CRYPTO_E_KEY_WRITE_FAIL;
            }
#if (STD_ON == CRYPTO_SPT_SHE_KEY_IMPORT)
            else if ((Crypto_KeyFormatType)CRYPTO_KE_FORMAT_BIN_SHEKEYS == Crypto_aKeyElementList[*pu32CryptoKeyElementIdx].eCryptoKeyFormat)
            {
                /* For SHE keys we have some special cases:
                    1. When Write Access is Encrypted, the key is written providing M1+M2+M3
                       Driver will check that the input is exactly 64 bytes length and the key element max size is larger or equal with 16 bytes
                    2. When Write Access is ALLOWED or INTERNAL_COPY the key is written in plain (this is the case of RAM key)
                       Driver will check that the input is exactly 16 bytes length and the key element max size is larger or equal with 16 bytes */
                if (CRYPTO_WA_ENCRYPTED == Crypto_aKeyElementList[*pu32CryptoKeyElementIdx].eCryptoKeyElementWriteAccess)
                {
                    if ((CRYPTO_SHE_SIZE_IN_U32 != u32KeyLength) || (Crypto_aKeyElementList[*pu32CryptoKeyElementIdx].u32CryptoKeyElementMaxSize < CRYPTO_SHE_KEY_SIZE_U32))
                    {
                        RetVal = (Std_ReturnType)CRYPTO_E_KEY_SIZE_MISMATCH;
                    }
                }
                else
                {
                    if ((CRYPTO_SHE_KEY_SIZE_U32 != u32KeyLength) || (Crypto_aKeyElementList[*pu32CryptoKeyElementIdx].u32CryptoKeyElementMaxSize < CRYPTO_SHE_KEY_SIZE_U32))
                    {
                        RetVal = (Std_ReturnType)CRYPTO_E_KEY_SIZE_MISMATCH;
                    }
                }
            }
#endif /* (STD_ON == CRYPTO_SPT_SHE_KEY_IMPORT) */
            else if (((FALSE == Crypto_aKeyElementList[*pu32CryptoKeyElementIdx].bCryptoKeyElementAllowPartialAccess) && (u32KeyLength < Crypto_aKeyElementList[*pu32CryptoKeyElementIdx].u32CryptoKeyElementMaxSize)) ||
                     (u32KeyLength > Crypto_aKeyElementList[*pu32CryptoKeyElementIdx].u32CryptoKeyElementMaxSize))
            {
                /* If u32KeyLength parameter of the function is smaller than the size of the key element and the key element is not configured to allow partial access
                   or if is greater than the key length return CRYPTO_E_KEY_SIZE_MISMATCH  */
                RetVal = (Std_ReturnType)CRYPTO_E_KEY_SIZE_MISMATCH;
            }
            else
            {
                /* Do nothing */
            }
        }
#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
        else
        {
            (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTSET_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
        }
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    }
    return RetVal;
}
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

/**
* @brief            Check that the parameters and configuration are valid.
* @details          Check that the parameters and configuration are valid for Crypto_ProcessJob().
*
* @param[in]        u32PartitionId  Holds the Id of the current partition
* @param[in]        u32ObjectIdx    Holds the index of the Crypto Driver Object
* @param[in]        pJob            Pointer to the configuration of the job. Contains structures with job and primitive relevant information but also pointer to result buffers.
*
* @return           Result of the operation
* @retval           E_OK                        Service request is valid
* @retval           Other values of E_OK        Service request is not valid
*
*
* @pre
*
*/
static inline Std_ReturnType Crypto_CheckForErrorsInProcessJob
(
    const uint32          u32PartitionId,
    const uint32          u32ObjectIdx,
    const Crypto_JobType* pJob
)
{
    Std_ReturnType             RetVal         = (Std_ReturnType)E_NOT_OK;
    Crypto_ProcessJobErrorType eProcessJobError;

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
    if (CRYPTO_INVALID_CDO_IDX_U32 == u32ObjectIdx)
    {
        /* If the parameter u32ObjectId is out of range and if development error detection for the Crypto Driver is enabled,
           the function Crypto_ProcessJob shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_PROCESSJOB_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
    /* Check whether the call is coming from an authorized core and the driver is initialized */
    else if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForCdoApis(u32PartitionId, u32ObjectIdx, CRYPTO_PROCESSJOB_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForCdoApis() function */
    }
    else if (NULL_PTR == pJob)
    {
        /* If the parameter job is a null pointer and if development error detection for the Crypto Driver is enabled,
           the function Crypto_ProcessJob shall report CRYPTO_E_PARAM_POINTER to the DET and return E_NOT_OK. */
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_PROCESSJOB_ID, (uint8)CRYPTO_E_PARAM_POINTER);
    }
#if (STD_ON == CRYPTO_KEYS_EXIST)
    /* Check whether the u32CryptoKeyId is valid */
    else if ((
             (CRYPTO_INVALID_KEY_IDX_U32 == Crypto_Util_GetKeyIdx(Crypto_Util_GetJobKey(pJob))) &&
             (CRYPTO_RANDOMGENERATE      != pJob->jobPrimitiveInfo->primitiveInfo->service)            &&
             (CRYPTO_HASH                != pJob->jobPrimitiveInfo->primitiveInfo->service)
#if (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT)
              && (pJob->jobPrimitiveInfo->primitiveInfo->service < CRYPTO_RANDOMSEED)
              )
              ||
              ((CRYPTO_INVALID_KEY_IDX_U32 == Crypto_Util_GetKeyIdx(pJob->jobPrimitiveInputOutput.cryIfKeyId)) && (pJob->jobPrimitiveInfo->primitiveInfo->service >= CRYPTO_RANDOMSEED))
#else
            )
#endif /* (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT) */
           )
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_PROCESSJOB_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */
    /* If the parameter job->jobPrimitiveInfo->primitiveInfo->service is not supported by the Crypto Driver Object and
      if development error detection for the Crypto Driver is enabled, the function Crypto_ProcessJob shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK */
    else if ((Std_ReturnType)E_NOT_OK == Crypto_VerifyPrimitive(u32ObjectIdx, pJob))
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_PROCESSJOB_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
    /* If a pointer is required as an argument, but it is a null pointer, the Crypto_ProcessJob() function shall report CRYPTO_E_PARAM_POINTER. */
    else
#else
    /* Avoid compiler warnings */
    (void)u32PartitionId;
    (void)u32ObjectIdx;
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        /* Based on the information in the job, check if there are other errors the function shall report */
        eProcessJobError = Crypto_GetProcessJobError(pJob);

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
        if (CRYPTO_DET_NULL_PTR == eProcessJobError)
        {
            (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_PROCESSJOB_ID, (uint8)CRYPTO_E_PARAM_POINTER);
        }
        else if (CRYPTO_DET_INVALID_LEN == eProcessJobError)
        {
            /* If the value, which is pointed by a length pointer, is zero, and if development error detection for the Crypto Driver is enabled, the Crypto_ProcessJob() function report CRYPTO_E_PARAM_VALUE to the DET and return E_NOT_OK.
            If a length pointer is required as an argument, but the value, which is pointed by the length pointer is zero, and if development error detection for the Crypto Driver is enabled, the Crypto_ProcessJob() function     report CRYPTO_E_PARAM_VALUE to the DET and return E_NOT_OK.    */
            (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_PROCESSJOB_ID, (uint8)CRYPTO_E_PARAM_VALUE);
        }
#if (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT)
        else if (CRYPTO_DET_INVALID_PARAM == eProcessJobError)
        {
            /* Invalid DET param can only be reported by synchronous key management services using 2 parameters (cryIfKeyId and targetCryIfKeyId): Crypto_KeyDerive() and Crypto_CertificateVerify() */
            (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_PROCESSJOB_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
        }
#endif /* (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT) */
        else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
        {
            if (CRYPTO_NO_DET_RET_KEY_NOT_VALID == eProcessJobError)
            {
                RetVal = (Std_ReturnType)CRYPTO_E_KEY_NOT_VALID;
            }
            else if (CRYPTO_NO_DET_RET_NOT_SUPPORTED == eProcessJobError)
            {
                RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;
            }
            else if (CRYPTO_NO_DET_RET_OK == eProcessJobError)
            {
                RetVal = (Std_ReturnType)E_OK;
            }
            else
            {
                /* We get here when the function Crypto_GetProcessJobError() returns either:
                    1. CRYPTO_NO_DET_RET_NOK
                    2. CRYPTO_DET_NULL_PTR or CRYPTO_DET_INVALID_LEN or CRYPTO_DET_INVALID_PARAM when DET is disabled
                   In both case we should return E_NOT_OK. RetVal is already set to E_NOT_OK from the moment it was declared, so nothing else to do */
            }
        }
    }
    return RetVal;
}

#if (STD_ON == CRYPTO_KEYS_EXIST)
/**
* @brief            Process jobs at HLD level and check that the parameters are valid.
* @details          Some of the jobs will be proceesed only at HLD level. Checks the parameters validity of the configured job for Crypto_ProcessJob().
*
* @param[in]        pJob                     Pointer to the configuration of the job. Contains structures with job and primitive relevant information but also pointer to result buffers.
* @param[in]        u32PartitionId           Holds the Id of the current partition
* @param[inout]     pbJobProcessingComplete  Pointer to boolean variable storing the processing status.
*
* @return           Result of the operation
* @retval           E_OK                        Service request is valid
* @retval           Other values of E_OK        Service request is not valid
*
*
* @pre
*
*/
static inline Std_ReturnType Crypto_ProcessJobAndCheckNonDetErrors
(
    Crypto_JobType* pJob,
    const uint32    u32PartitionId
#if (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT)
    , boolean*      pbJobProcessingComplete
#endif /* (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT) */
)
{
    Std_ReturnType         RetVal      = (Std_ReturnType)E_OK;
    Crypto_ServiceInfoType eJobService = pJob->jobPrimitiveInfo->primitiveInfo->service;
    uint32                 u32CryptoKeyIdx;
#if (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT)
    Crypto_ProcessingType  eProcessingType;
    uint32                 u32CryptoTargetKeyIdx;
#endif /* (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT) */

#if (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT)
    u32CryptoTargetKeyIdx = Crypto_Util_GetKeyIdx(pJob->jobPrimitiveInputOutput.targetCryIfKeyId);

    /* Get the crypto key from the proper place inside the job structure, depending on the service */
    if (eJobService < CRYPTO_RANDOMSEED)
#endif /* (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT) */
    {
        u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(Crypto_Util_GetJobKey(pJob));
    }
#if (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT)
    else
    {
        u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(pJob->jobPrimitiveInputOutput.cryIfKeyId);
    }

    if ((uint8)eJobService < (uint8)CRYPTO_RANDOMSEED)
#endif /* (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT) */
    {
        /* This is the case of Crypto services that are natively handled by Crypto_ProcessJob interface */
        RetVal = Crypto_VerifyKeyValidity(u32CryptoKeyIdx, eJobService);
    }
#if (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT)
    else
    {
        eProcessingType = pJob->jobPrimitiveInfo->processingType;
        /* These are the cases of synchronous key management API services that are required to work on Crypto_ProcessJob interface */
        switch (eJobService)
        {
            /* Need to check the validity of parameters that are not checked by DET */
            case CRYPTO_RANDOMSEED:
                RetVal = Crypto_GetNonDetErrorsForRandomSeed(u32CryptoKeyIdx);
                break;

#if (STD_ON == CRYPTO_SPT_KEY_GENERATE)
            case CRYPTO_KEYGENERATE:
                RetVal = Crypto_GetNonDetErrorsForKeyGenerate(u32CryptoKeyIdx);
                break;
#endif /* (STD_ON == CRYPTO_SPT_KEY_GENERATE) */

#if (STD_ON == CRYPTO_SPT_KEY_DERIVE)
            case CRYPTO_KEYDERIVE:
                RetVal = Crypto_GetNonDetErrorsForKeyDerive(u32CryptoKeyIdx, u32CryptoTargetKeyIdx);
                break;
#endif /* (STD_ON == CRYPTO_SPT_KEY_DERIVE) */

            case CRYPTO_KEYEXCHANGECALCPUBVAL:
                RetVal = Crypto_GetNonDetErrorsForKeyExchCalcPubVal(CRYPTO_PROCESSJOB_ID, u32CryptoKeyIdx, pJob->jobPrimitiveInputOutput.outputPtr, pJob->jobPrimitiveInputOutput.outputLengthPtr);
                if ((Std_ReturnType)E_OK == RetVal)
                {
                    RetVal = Crypto_CopyDataFromPubValKeyElementToUser(u32PartitionId, u32CryptoKeyIdx, pJob->jobPrimitiveInputOutput.outputPtr, pJob->jobPrimitiveInputOutput.outputLengthPtr);
                    /* Check if the job is async; in this case we need to call the CryIf callback */
                    if (CRYPTO_PROCESSING_ASYNC == eProcessingType)
                    {
                        CryIf_CallbackNotification(pJob, RetVal);
                    }
                }
                /* This job has been completely processed at HLD level. No need to call any IPW functionality */
                *pbJobProcessingComplete = TRUE;
                break;

            case CRYPTO_KEYEXCHANGECALCSECRET:
                RetVal = Crypto_GetNonDetErrorsForKeyExchCalcSecret(u32CryptoKeyIdx);
                break;

            case CRYPTO_KEYSETVALID:
                RetVal = Crypto_MarkKeyAsValid(u32PartitionId, u32CryptoKeyIdx, CRYPTO_PROCESSJOB_ID);
                /* Check if the job is async; in this case we need to call the CryIf callback */
                if (CRYPTO_PROCESSING_ASYNC == eProcessingType)
                {
                    CryIf_CallbackNotification(pJob, RetVal);
                }
                /* This job has been completely processed at HLD level. No need to call any IPW functionality */
                *pbJobProcessingComplete = TRUE;
                break;

            case CRYPTO_CERTIFICATEPARSE:
            /* fall-through */
            case CRYPTO_CERTIFICATEVERIFY:
                RetVal = (Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED;
                /* This job has been completely processed at HLD level. No need to call any IPW functionality */
                *pbJobProcessingComplete = TRUE;
                break;

            default:
                /* Invalid service id. Return E_NOT_OK */
                RetVal = (Std_ReturnType)E_NOT_OK;
                break;
        }
    }
#else
    /* Avoid compiler warnings */
    (void)u32PartitionId;
#endif /* (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT) */

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

#if (STD_ON == CRYPTO_KEYS_EXIST)
#if (STD_ON == CRYPTO_SPT_ENC_AUTH_KEY_EXPORT)
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
static inline boolean Crypto_AuthCipherModeIsCcmOrGcm
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
#endif /* (STD_ON == CRYPTO_SPT_ENC_AUTH_KEY_EXPORT) */

/**
* @brief           Reads the content of a non key material keyElement
* @details         Reads the content of a non key material keyElement
*
* @param[in]       u32PartitionId:           Id of the partition where this function is run
* @param[in]       u32CryptoKeyIdx:          Id of the key
* @param[in]       u32CryptoKeyElementId:    Id of the key element
* @param[in]       pResult:                  Location where the key element will be exported
* @param[inout]    pResultLength:            The length of the exported key element
*
* @returns         Std_ReturnType
*                   E_OK:                       Request successful
*                   E_NOT_OK:                   Request unsuccessful
*                   CRYPTO_E_KEY_EMPTY:         Key is empty
*/
static inline Std_ReturnType Crypto_GetNonKeyMaterialElement
(
    const uint32 u32PartitionId,
    const uint32 u32CryptoKeyIdx,
    const uint32 u32CryptoKeyElementId,
    uint8*       pResult,
    uint32*      pResultLength
)
{
    Std_ReturnType RetVal                 = (Std_ReturnType)E_NOT_OK;
    uint32         u32CryptoKeyElementIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, u32CryptoKeyElementId);

    /* Check if the u32CryptoKeyElementIdx is valid */
    if(CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoKeyElementIdx)
    {
        {
#if (STD_ON == CRYPTO_ENABLE_FEED_HSE_DESC_SUPPORT)
            /* Check if we are in the special case of FEEDDE5C keyElementId */
            if (CRYPTO_KE_FEED_HSE_DESCRIPTOR_U32 == u32CryptoKeyElementId)
            {
                RetVal = Crypto_Ipw_FeedHseDescriptor(u32PartitionId,
                                                      pResult,
                                                      pResultLength);
            }
            else
#endif /* (STD_ON == CRYPTO_ENABLE_FEED_HSE_DESC_SUPPORT) */
            {
                /* Request failed, source key element is uninitialized */
                if (0U < *Crypto_aKeyElementList[u32CryptoKeyElementIdx].pu32CryptoElementActualSize)
                {
                    /* If the KeyElement we are trying to read information from uses an IP key, it will not have a buffer reserved in the Crypto driver, so we would get into the situation where
                    we read information from address 0. Protect against this case */
                    if (NULL_PTR != Crypto_aKeyElementList[u32CryptoKeyElementIdx].pCryptoElementArray)
                    {
                        Crypto_Util_MemCpy8(pResult, Crypto_aKeyElementList[u32CryptoKeyElementIdx].pCryptoElementArray, *Crypto_aKeyElementList[u32CryptoKeyElementIdx].pu32CryptoElementActualSize);
                        /* When the request has finished, the amount of data that has been stored shall be stored. */
                        *pResultLength = *Crypto_aKeyElementList[u32CryptoKeyElementIdx].pu32CryptoElementActualSize;
                        RetVal = (Std_ReturnType)E_OK;
                    }
                    else
                    {
                        RetVal = (Std_ReturnType)E_NOT_OK;
                    }
                }
                else
                {
                    RetVal = (Std_ReturnType)CRYPTO_E_KEY_EMPTY;
                }

                /* Avoid compiler warnings */
                (void)u32PartitionId;
            }
        }
    }

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/
/**
* @implements       Crypto_Init_Activity
*/
void Crypto_Init
(
    const Crypto_ConfigType* configPtr
)
{
    Std_ReturnType RetVal;
    const uint32   u32PartitionId = Crypto_Util_GetPartitionId();

#if ((STD_ON == CRYPTO_ENABLE_MULTICORE_SUPPORT) && (STD_ON == CRYPTO_DEV_ERROR_DETECT))
    /* Check if the partition this call is coming from is authorized. There should be at least one Crypto Driver Object mapped on the partition, in order for the call to be valid */
    if (0U != Crypto_aPartitionToCdoMapping[u32PartitionId].u8NumCDOs)
    {
#endif /* (STD_ON == CRYPTO_ENABLE_MULTICORE_SUPPORT && STD_ON == CRYPTO_DEV_ERROR_DETECT) */
        /* Initialize the driver only if the driver was not already initialized */
        if (CRYPTO_DRIVER_UNINIT == Crypto_aeDriverState[u32PartitionId])
        {
            RetVal = Crypto_Ipw_Init(u32PartitionId);
            if ((Std_ReturnType)E_OK == RetVal)
            {
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT))
                /* Try to restore from Nvram the information in the blob containing the Crypto Key valid flags */
                RetVal = Crypto_HandleNvramInfo(u32PartitionId, CRYPTO_NVRAM_OP_READ, CRYPTO_NVRAM_BLOB_0_ID, CRYPTO_INIT_ID);
                if ((Std_ReturnType)E_OK == RetVal)
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)) */
                {
#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT))
                    /* Try to restore from Nvram the information in the blob containing the values of the persistent Crypto Key Elements */
                    RetVal = Crypto_HandleNvramInfo(u32PartitionId, CRYPTO_NVRAM_OP_READ, CRYPTO_NVRAM_BLOB_1_ID, CRYPTO_INIT_ID);
                    if ((Std_ReturnType)E_OK == RetVal)
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)) */
                    {
                        /* Initialize queues used by the driver's CDOs */
                        Crypto_Util_InitJobQueues();
                        /* After initialization, the Crypto driver is in initialized state */
                        Crypto_aeDriverState[u32PartitionId] = CRYPTO_DRIVER_INITIALIZED;
                    }
                }
            }
#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
            else
            {
                (void) Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_INIT_ID, (uint8)CRYPTO_E_INIT_FAILED);
            }
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
        }
#if ((STD_ON == CRYPTO_ENABLE_MULTICORE_SUPPORT) && (STD_ON == CRYPTO_DEV_ERROR_DETECT))
    }
    else
    {
        (void) Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_INIT_ID, (uint8)CRYPTO_E_PARAM_CONFIG);
    }
#endif /* (STD_ON == CRYPTO_ENABLE_MULTICORE_SUPPORT && STD_ON == CRYPTO_DEV_ERROR_DETECT) */

    /* Avoid compiler warnings */
    (void) configPtr;
}

#if (STD_ON == CRYPTO_VERSION_INFO_API)
/**
* @implements       Crypto_GetVersionInfo_Activity
*/
void Crypto_GetVersionInfo
(
    Std_VersionInfoType* versioninfo
)
{
#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
    if (NULL_PTR == versioninfo)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_GETVERSIONINFO_ID, (uint8)CRYPTO_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == CRYPTO_DEV_ERROR_DETECT */
    {
        versioninfo->moduleID           = (uint16)CRYPTO_MODULE_ID;
        versioninfo->vendorID           = (uint16)CRYPTO_VENDOR_ID;
        versioninfo->sw_major_version   = (uint8)CRYPTO_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version   = (uint8)CRYPTO_SW_MINOR_VERSION;
        versioninfo->sw_patch_version   = (uint8)CRYPTO_SW_PATCH_VERSION;
    }
}
#endif /* (STD_ON == CRYPTO_VERSION_INFO_API) */

/**
* @implements     Crypto_ProcessJob_Activity
*/
Std_ReturnType Crypto_ProcessJob
(
    uint32          objectId,
    Crypto_JobType* job
)
{
    const uint32               u32PartitionId = Crypto_Util_GetPartitionId();
    const uint32               u32ObjectIdx   = Crypto_Util_GetObjectIdx(objectId);
    Std_ReturnType             RetVal         = (Std_ReturnType)E_NOT_OK;
#if (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT)
    boolean                    bJobProcessingComplete = FALSE;
#endif /* (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT) */

    RetVal = Crypto_CheckForErrorsInProcessJob(u32PartitionId, u32ObjectIdx, job);

    if ((Std_ReturnType)E_OK == RetVal)
    {
#if (STD_ON == CRYPTO_KEYS_EXIST)
    RetVal = Crypto_ProcessJobAndCheckNonDetErrors
             (
                job,
                u32PartitionId
    #if (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT)
                , &bJobProcessingComplete
    #endif /* (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT) */
             );
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */
    }

    if (((Std_ReturnType)E_OK == RetVal)
#if (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT)
        && (FALSE == bJobProcessingComplete)
#endif /* (STD_ON == CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT) */
        )
    {
        RetVal = Crypto_Ipw_ProcessJob(u32PartitionId, u32ObjectIdx, job);
    }

    /* If the buffer pJob->jobPrimitiveInput.outputPtr or pJob->jobPrimitiveInput.secondaryOutputPtr is too small to store the result of the request,
    CRYPTO_E_SMALL_BUFFER shall be returned and the function shall additionally report the runtime error CRYPTO_E_RE_SMALL_BUFFER */
    if ((Std_ReturnType)CRYPTO_E_SMALL_BUFFER == RetVal)
    {
        (void)Det_ReportRuntimeError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_PROCESSJOB_ID, (uint8)CRYPTO_E_RE_SMALL_BUFFER);
    }

    if (CRYPTO_RET_OPERATION_TIMEOUT <= RetVal)
    {
        Crypto_ReportErrorToDet((uint8)CRYPTO_PROCESSJOB_ID, (uint8)RetVal);
        RetVal = (Std_ReturnType)E_NOT_OK;
    }

    return RetVal;
}

/**
* @implements     Crypto_CancelJob_Activity
*/
Std_ReturnType Crypto_CancelJob
(
    uint32              objectId,
    Crypto_JobInfoType* job
)
{
    const uint32   u32PartitionId = Crypto_Util_GetPartitionId();
    Std_ReturnType RetVal         = (Std_ReturnType)E_NOT_OK;
    uint32         u32ObjectIdx   = Crypto_Util_GetObjectIdx(objectId);
#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)

    /* If development error detection for the Crypto Driver is enabled: The function Crypto_CancelJob shall raise the error CRYPTO_E_PARAM_HANDLE and return E_NOT_OK if the parameter u32ObjectId is out or range. */
    if (CRYPTO_INVALID_CDO_IDX_U32 == u32ObjectIdx)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_CANCELJOB_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
    /* Check whether the call is coming from an authorized core and the driver is initialized */
    else if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForCdoApis(u32PartitionId, u32ObjectIdx, CRYPTO_CANCELJOB_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForCdoApis() function */
    }
    /* If development error detection for the Crypto Driver is enabled: The function Crypto_CancelJob shall raise the error CRYPTO_E_PARAM_POINTER and return E_NOT_OK if the parameter job is a null pointer. */
    else if (NULL_PTR == job)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_CANCELJOB_ID, (uint8)CRYPTO_E_PARAM_POINTER);
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        RetVal = Crypto_Ipw_CancelJob(u32PartitionId, u32ObjectIdx, job);
        if (CRYPTO_RET_OPERATION_TIMEOUT <= RetVal)
        {
            Crypto_ReportErrorToDet((uint8)CRYPTO_CANCELJOB_ID, (uint8)RetVal);
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
    }

    return RetVal;
}

/**
* @implements     Crypto_KeyElementSet_Activity
*/
Std_ReturnType Crypto_KeyElementSet
(
    uint32       cryptoKeyId,
    uint32       keyElementId,
    const uint8* keyPtr,
    uint32       keyLength
)
{
    Std_ReturnType RetVal         = (Std_ReturnType)E_NOT_OK;
#if (STD_ON == CRYPTO_KEYS_EXIST)
    const uint32   u32PartitionId = Crypto_Util_GetPartitionId();
#if (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)
    Std_ReturnType RetValNvramOp;
#endif /* (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT) */
    uint32         u32CryptoKeyElementIdx;
    uint32         u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(cryptoKeyId);

    RetVal = Crypto_CheckForErrorsInKeyElementSet(u32PartitionId, u32CryptoKeyIdx, keyPtr, keyLength, keyElementId, &u32CryptoKeyElementIdx);

    if ((Std_ReturnType)E_OK == RetVal)
    {
        /* If the key element is key material or partner's public key, send it to be stored */
        if ((CRYPTO_KEY_MATERIAL_U32 == keyElementId) || (CRYPTO_KE_KEYEXCHANGE_PARTNERPUBKEY == keyElementId))
        {
            RetVal = Crypto_Ipw_ImportKey(u32PartitionId,
                                          keyPtr,
                                          keyLength,
                                          u32CryptoKeyIdx,
                                          u32CryptoKeyElementIdx
                                         );
            if (CRYPTO_RET_OPERATION_TIMEOUT <= RetVal)
            {
                Crypto_ReportErrorToDet((uint8)CRYPTO_KEYELEMENTSET_ID, (uint8)RetVal);
                RetVal = (Std_ReturnType)E_NOT_OK;
            }
        }
        /* If the key element is different from key material (IV, seed) - store it */
        else
        {
            /* If the key element pointed by u32CryptoKeyElementIdx is not key material but uses a IP key, it will not have memory allocated inside Crypto.
               We need to check this condition. */
            if (NULL_PTR != Crypto_aKeyElementList[u32CryptoKeyElementIdx].pCryptoElementArray)
            {
                Crypto_Util_MemCpy8(Crypto_aKeyElementList[u32CryptoKeyElementIdx].pCryptoElementArray, keyPtr, keyLength);
                RetVal = (Std_ReturnType)E_OK;
            }
            else
            {
                RetVal = (Std_ReturnType)E_NOT_OK;
            }
        }
        /* Store the actual size of the key element */
        if ((Std_ReturnType)E_OK == RetVal)
        {
#if (STD_ON == CRYPTO_SPT_SHE)
            if ((Crypto_KeyFormatType)CRYPTO_KE_FORMAT_BIN_SHEKEYS == Crypto_aKeyElementList[u32CryptoKeyElementIdx].eCryptoKeyFormat)
            {
                if (CRYPTO_WA_ENCRYPTED == Crypto_aKeyElementList[u32CryptoKeyElementIdx].eCryptoKeyElementWriteAccess)
                {
                    *Crypto_aKeyElementList[u32CryptoKeyElementIdx].pu32CryptoElementActualSize = CRYPTO_SHE_KEY_SIZE_U32;
                }
                else
                {
                    /* This is the case of imported RAM key. Need to set it's actual length to sum of M1 to M5 because this the length that the upper layer
                    will get when exporting the RAM key */
                    *Crypto_aKeyElementList[u32CryptoKeyElementIdx].pu32CryptoElementActualSize = CRYPTO_SHE_SIZE_OUT_U32;
                }
            }
            else
#endif /* (STD_ON == CRYPTO_SPT_SHE) */
            {
                *Crypto_aKeyElementList[u32CryptoKeyElementIdx].pu32CryptoElementActualSize = keyLength;
            }

            /* If the keyElement is marked as persistent, we need to request upper layer to update in Nvram the content of the blob storing key elements values and actual lengths */
#if (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)
            if (TRUE == Crypto_aKeyElementList[u32CryptoKeyElementIdx].bCryptoKeyElementPersist)
            {
                RetVal = Crypto_HandleNvramInfo(u32PartitionId, CRYPTO_NVRAM_OP_WRITE, CRYPTO_NVRAM_BLOB_1_ID, CRYPTO_KEYELEMENTSET_ID);
            }
#endif /* (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT) */
        }

        /* When the KeyElementSet is called, the key state is set to invalid */
        Crypto_ClearKeyValidFlag(u32CryptoKeyIdx);
        /* Need to save in Nvram the keyValid blob which has just been updated */
#if (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)
        RetValNvramOp = Crypto_HandleNvramInfo(u32PartitionId, CRYPTO_NVRAM_OP_WRITE, CRYPTO_NVRAM_BLOB_0_ID, CRYPTO_KEYELEMENTSET_ID);
        /* Change the value of the function return value to the one from Nvram operation, only if something has not failed before,
        case when we will want to report that previous value */
        if ((Std_ReturnType)E_OK == RetVal)
        {
            RetVal = RetValNvramOp;
        }
#endif /* (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT) */
    }
#else
    (void) cryptoKeyId;
    (void) keyElementId;
    (void) keyPtr;
    (void) keyLength;
#endif /* (STD_ON == CRYPTO_KEYS_EXIST)*/

    return RetVal;
}

/**
* @implements     Crypto_KeySetValid_Activity
*/
Std_ReturnType Crypto_KeySetValid
(
    uint32 cryptoKeyId
)
{
    Std_ReturnType RetVal          = (Std_ReturnType)E_NOT_OK;
#if (STD_ON == CRYPTO_KEYS_EXIST)
    const uint32   u32PartitionId  = Crypto_Util_GetPartitionId();
    uint32         u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(cryptoKeyId);

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_KEYSETVALID_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    /* If parameter cryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeySetValid shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    else if (CRYPTO_INVALID_KEY_IDX_U32 == u32CryptoKeyIdx)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYSETVALID_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        RetVal = Crypto_MarkKeyAsValid(u32PartitionId, u32CryptoKeyIdx, CRYPTO_KEYSETVALID_ID);
    }
#else
    /* Avoid compiler warnings */
    (void) cryptoKeyId;
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

    return RetVal;
}

/**
* @implements     Crypto_KeyElementGet_Activity
*/
Std_ReturnType Crypto_KeyElementGet
(
    uint32  cryptoKeyId,
    uint32  keyElementId,
    uint8*  resultPtr,
    uint32* resultLengthPtr
)
{
    Std_ReturnType RetVal                    = (Std_ReturnType)E_NOT_OK;
#if (STD_ON == CRYPTO_KEYS_EXIST)
    const uint32   u32PartitionId            = Crypto_Util_GetPartitionId();
    uint32         u32CryptoKeyIdx           = Crypto_Util_GetKeyIdx(cryptoKeyId);
    uint32         u32CryptoKeyElementExpIdx = CRYPTO_INVALID_KEY_ELEMENT_IDX_U32;
    uint32         u32CryptoKeyElementIdx;

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_KEYELEMENTGET_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    /* If parameter cryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeySetValid shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    else if (CRYPTO_INVALID_KEY_IDX_U32 == u32CryptoKeyIdx)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTGET_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
    /* If the parameter resultPtr is a null pointer and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyElementGet shall report CRYPTO_E_PARAM_POINTER the DET and return E_NOT_OK. */
    /* If the parameter resultLengthPtr is a null pointer and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyElementGet shall report CRYPTO_E_PARAM_POINTER to the DET and return E_NOT_OK. */
    else if ((NULL_PTR == resultPtr) || (NULL_PTR == resultLengthPtr))
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTGET_ID, (uint8)CRYPTO_E_PARAM_POINTER);
    }
    /* If the value, which is pointed by resultLengthPtr is zero and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyElementGet shall report CRYPTO_E_PARAM_VALUE to the DET and return E_NOT_OK. */
    else if (0U == *resultLengthPtr)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTGET_ID, (uint8)CRYPTO_E_PARAM_VALUE);
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        /* If the parameter keyElementId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyElementGet shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
        u32CryptoKeyElementIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, keyElementId);

        if (CRYPTO_INVALID_KEY_ELEMENT_IDX_U32 != u32CryptoKeyElementIdx)
        {
            /* Request failed because read acces was denied */
            if (CRYPTO_RA_DENIED == Crypto_aKeyElementList[u32CryptoKeyElementIdx].eCryptoKeyElementReadAccess)
            {
                RetVal = (Std_ReturnType)CRYPTO_E_KEY_READ_FAIL;
            }
            /* If the buffer resultPtr is too small to store the result of the request, CRYPTO_E_SMALL_BUFFER shall be returned and if development error detection is enabled, CRYPTO_E_SMALL_BUFFER shall be reported to the DET. */
            else if (*resultLengthPtr < *Crypto_aKeyElementList[u32CryptoKeyElementIdx].pu32CryptoElementActualSize)
            {
                RetVal = (Std_ReturnType)CRYPTO_E_SMALL_BUFFER;
#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
                (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTGET_ID, (uint8)CRYPTO_E_SMALL_BUFFER);
#endif
            }
            else
            {
                /* If the key element is key material, retrieve the key */
                if (CRYPTO_KEY_MATERIAL_U32 == keyElementId)
                {
#if (STD_ON == CRYPTO_SPT_ENC_AUTH_KEY_EXPORT)
                    /* Check if the authenticated export option is enabled by verifing the value of the authentication key handle or
                       in case of AEAD, which offers encryption and authentication with a single key, by verifing the authentication cipher mode stored in cipher scheme.
                       For symmetric keys is mandatory and for public keys is optional */
                    if (
                        (HSE_INVALID_KEY_HANDLE != Crypto_aKeyElementList[u32CryptoKeyElementIdx].u32HseExportAuthKeyHandle) ||
                        (
                            (NULL_PTR != Crypto_aKeyElementList[u32CryptoKeyElementIdx].pHseExportCipherScheme) &&
                            (TRUE     == Crypto_AuthCipherModeIsCcmOrGcm(Crypto_aKeyElementList[u32CryptoKeyElementIdx].pHseExportCipherScheme->u8CryptoAuthCipherMode))
                        )
                       )
                    {
                        /* Find the key element with keyElementId = 99 */
                        u32CryptoKeyElementExpIdx = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KE_EXPORT_AUTHENTICATION);
                    }
#endif /* (STD_ON == CRYPTO_SPT_ENC_AUTH_KEY_EXPORT) */
                    /* Export the key */
                    RetVal = Crypto_Ipw_ExportKey(u32PartitionId,
                                                  u32CryptoKeyElementIdx,
                                                  u32CryptoKeyElementExpIdx,
                                                  resultPtr,
                                                  resultLengthPtr
                                                 );
                    if (CRYPTO_RET_OPERATION_TIMEOUT <= RetVal)
                    {
                        Crypto_ReportErrorToDet((uint8)CRYPTO_KEYELEMENTGET_ID, (uint8)RetVal);
                        RetVal = (Std_ReturnType)E_NOT_OK;
                    }
                }
                /* If the key element is different from key material (IV, seed) */
                else
                {
                    RetVal = Crypto_GetNonKeyMaterialElement(u32PartitionId,
                                                             u32CryptoKeyIdx,
                                                             keyElementId,
                                                             resultPtr,
                                                             resultLengthPtr
                                                            );
                }
            }
        }
#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
        else
        {
            (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTGET_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
        }
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    }

    /* If the function Crypto_KeyElementGet returns CRYPTO_E_KEY_NOT_AVAILABLE, the function shall additionally report the runtime error CRYPTO_E_RE_KEY_NOT_AVAILABLE */
    if ((Std_ReturnType)CRYPTO_E_KEY_NOT_AVAILABLE == RetVal)
    {
        (void)Det_ReportRuntimeError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTGET_ID, (uint8)CRYPTO_E_RE_KEY_NOT_AVAILABLE);
    }
    else if ((Std_ReturnType)CRYPTO_E_KEY_READ_FAIL == RetVal)
    {
        /* If the function Crypto_KeyElementGet returns CRYPTO_E_KEY_READ_FAIL, the function shall additionally report the runtime error CRYPTO_E_RE_KEY_READ_FAIL */
        (void)Det_ReportRuntimeError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTGET_ID, (uint8)CRYPTO_E_RE_KEY_READ_FAIL);
    }
    else
    {
        /* do nothing */
    }

#else
    /* Avoid compiler warnings */
    (void) cryptoKeyId;
    (void) keyElementId;
    (void) resultPtr;
    (void) resultLengthPtr;
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

    return RetVal;
}

/**
* @implements     Crypto_KeyElementCopy_Activity
*/
Std_ReturnType Crypto_KeyElementCopy
(
    uint32 cryptoKeyId,
    uint32 keyElementId,
    uint32 targetCryptoKeyId,
    uint32 targetKeyElementId
)
{
    Std_ReturnType RetVal          = (Std_ReturnType)E_NOT_OK;
#if (STD_ON == CRYPTO_KEYS_EXIST)
    const uint32   u32PartitionId  = Crypto_Util_GetPartitionId();
    uint32         u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(cryptoKeyId);
    uint32         u32TargetKeyIdx = Crypto_Util_GetKeyIdx(targetCryptoKeyId);
    uint32         u32CryptoKeyElementIdx;
    uint32         u32CryptoTargetKeyElementIdx;
#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)

     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_KEYELEMENTCOPY_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    /* If parameter cryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyElementCopy shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    /* If parameter targetCryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyElementCopy shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    else if ((CRYPTO_INVALID_KEY_IDX_U32 == u32CryptoKeyIdx) || (CRYPTO_INVALID_KEY_IDX_U32 == u32TargetKeyIdx))
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTCOPY_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        /* If parameter keyElementId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyElementCopy shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
        /* If parameter targetKeyElementId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyElementCopy shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
        u32CryptoKeyElementIdx       = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx,       keyElementId);
        u32CryptoTargetKeyElementIdx = Crypto_Util_GetKeyElementIdx(u32TargetKeyIdx, targetKeyElementId);

        RetVal = Crypto_CheckParamAndConfForKeyElementCopy(u32CryptoKeyElementIdx, u32CryptoTargetKeyElementIdx);
        if ((Std_ReturnType)E_OK == RetVal)
        {
            if (CRYPTO_KEY_MATERIAL_U32 != keyElementId)
            {
                /* If the key element is key material, send it to be stored */
                if (CRYPTO_KEY_MATERIAL_U32 == targetKeyElementId)
                {
                    RetVal = Crypto_Ipw_ImportKey(u32PartitionId,
                                                  Crypto_aKeyElementList[u32CryptoKeyElementIdx].pCryptoElementArray,
                                                  *Crypto_aKeyElementList[u32CryptoKeyElementIdx].pu32CryptoElementActualSize,
                                                  u32TargetKeyIdx,
                                                  u32CryptoTargetKeyElementIdx
                                                 );
                    /* If the key import was successful update the Crypto element actual size */
                    if ((Std_ReturnType)E_OK == RetVal)
                    {
                        *Crypto_aKeyElementList[u32CryptoTargetKeyElementIdx].pu32CryptoElementActualSize = *Crypto_aKeyElementList[u32CryptoKeyElementIdx].pu32CryptoElementActualSize;
                    }
                    /* Check if the error should be reported to Det */
                    if (CRYPTO_RET_OPERATION_TIMEOUT <= RetVal)
                    {
                        Crypto_ReportErrorToDet((uint8)CRYPTO_KEYELEMENTCOPY_ID, (uint8)RetVal);
                        RetVal = (Std_ReturnType)E_NOT_OK;
                    }
                }
                /* If the key element is different from key material (IV, seed) - store it */
                else
                {
                    /* Check if the buffer of the destination keyElement is allocated inside Crypto driver or is allocated inside IP, case when it is a NULL pointer */
                    if (NULL_PTR != Crypto_aKeyElementList[u32CryptoTargetKeyElementIdx].pCryptoElementArray)
                    {
                        Crypto_Util_MemCpy8(Crypto_aKeyElementList[u32CryptoTargetKeyElementIdx].pCryptoElementArray,
                                            Crypto_aKeyElementList[u32CryptoKeyElementIdx].pCryptoElementArray,
                                           *Crypto_aKeyElementList[u32CryptoKeyElementIdx].pu32CryptoElementActualSize
                                           );
                        /* Update the actual size of the target element to source element actual size  */
                        *Crypto_aKeyElementList[u32CryptoTargetKeyElementIdx].pu32CryptoElementActualSize = *Crypto_aKeyElementList[u32CryptoKeyElementIdx].pu32CryptoElementActualSize;
                        RetVal = (Std_ReturnType)E_OK;
                    }
                    else
                    {
                        RetVal = (Std_ReturnType)E_NOT_OK;
                    }
                }
            }
            else
            {
                if (CRYPTO_KEY_MATERIAL_U32 != targetKeyElementId)
                {
                    RetVal = Crypto_Ipw_ExportKey(u32PartitionId,
                                                  u32CryptoKeyElementIdx,
                                                  CRYPTO_INVALID_KEY_ELEMENT_IDX_U32,
                                                  Crypto_aKeyElementList[u32CryptoTargetKeyElementIdx].pCryptoElementArray,
                                                  Crypto_aKeyElementList[u32CryptoTargetKeyElementIdx].pu32CryptoElementActualSize);
                    if (CRYPTO_RET_OPERATION_TIMEOUT <= RetVal)
                    {
                        Crypto_ReportErrorToDet((uint8)CRYPTO_KEYELEMENTCOPY_ID, (uint8)RetVal);
                        RetVal = (Std_ReturnType)E_NOT_OK;
                    }
                }
                else
                {
                    /* Copying a key material in another key material is not allowed. */
                    RetVal = (Std_ReturnType)E_NOT_OK;
                }
            }

            /* If copying the key element was successful and the target key element is marked as persistent, we need to update in Nvram the blob containing the key elements values and actual lengths */
#if (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)
            if (((Std_ReturnType)E_OK == RetVal) && (TRUE == Crypto_aKeyElementList[u32CryptoTargetKeyElementIdx].bCryptoKeyElementPersist))
            {
                RetVal = Crypto_HandleNvramInfo(u32PartitionId, CRYPTO_NVRAM_OP_WRITE, CRYPTO_NVRAM_BLOB_1_ID, CRYPTO_KEYELEMENTCOPY_ID);
            }
#endif /* (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT) */
        }
    }
#else
    /* Avoid compiler warnings */
    (void) cryptoKeyId;
    (void) keyElementId;
    (void) targetCryptoKeyId;
    (void) targetKeyElementId;
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

    return RetVal;
}

/**
* @implements     Crypto_KeyElementCopyPartial_Activity
*/
Std_ReturnType Crypto_KeyElementCopyPartial
(
    uint32 cryptoKeyId,
    uint32 keyElementId,
    uint32 keyElementSourceOffset,
    uint32 keyElementTargetOffset,
    uint32 keyElementCopyLength,
    uint32 targetCryptoKeyId,
    uint32 targetKeyElementId
)
{
    Std_ReturnType RetVal          = (Std_ReturnType)E_NOT_OK;
#if (STD_ON == CRYPTO_KEYS_EXIST)
    const uint32   u32PartitionId  = Crypto_Util_GetPartitionId();
    uint32         u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(cryptoKeyId);
    uint32         u32TargetKeyIdx = Crypto_Util_GetKeyIdx(targetCryptoKeyId);
    uint32         u32CryptoKeyElementIdx;
    uint32         u32CryptoTargetKeyElementIdx;
    uint32         u32TargetCryptoKeyElementActualSize;
    const uint8*   pCryptoSourceElementArray;
    uint8*         pCryptoTargetElementArray;
#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)

     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_KEYELEMENTCOPYPARTIAL_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    /* If parameter cryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyElementCopyPartial shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    /* If parameter targetCryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyElementCopyPartial shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    else if ((CRYPTO_INVALID_KEY_IDX_U32 == u32CryptoKeyIdx) || (CRYPTO_INVALID_KEY_IDX_U32 == u32TargetKeyIdx))
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTCOPYPARTIAL_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        /* If parameter keyElementId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyElementCopyPartial shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
        /* If parameter targetKeyElementId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyElementCopyPartial shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
        u32CryptoKeyElementIdx       = Crypto_Util_GetKeyElementIdx(u32CryptoKeyIdx,       keyElementId);
        u32CryptoTargetKeyElementIdx = Crypto_Util_GetKeyElementIdx(u32TargetKeyIdx, targetKeyElementId);

        RetVal = Crypto_CheckParamAndConfForKeyElementCopyPartial(u32CryptoKeyElementIdx, u32CryptoTargetKeyElementIdx, keyElementSourceOffset, keyElementTargetOffset, keyElementCopyLength);

        if ((Std_ReturnType)E_OK == RetVal)
        {
            u32TargetCryptoKeyElementActualSize = *Crypto_aKeyElementList[u32CryptoTargetKeyElementIdx].pu32CryptoElementActualSize;

            pCryptoSourceElementArray = Crypto_aKeyElementList[u32CryptoKeyElementIdx].pCryptoElementArray;
            pCryptoTargetElementArray = Crypto_aKeyElementList[u32CryptoTargetKeyElementIdx].pCryptoElementArray;

            /* This case is used for non Key material to non key material keys */
            if ((CRYPTO_KEY_MATERIAL_U32 != keyElementId) && (CRYPTO_KEY_MATERIAL_U32 != targetKeyElementId))
            {
                Crypto_Util_MemCpy8(&pCryptoTargetElementArray[keyElementTargetOffset], &pCryptoSourceElementArray[keyElementSourceOffset], keyElementCopyLength);

                if (u32TargetCryptoKeyElementActualSize < (uint32)(keyElementTargetOffset + keyElementCopyLength))
                {
                    *Crypto_aKeyElementList[u32CryptoTargetKeyElementIdx].pu32CryptoElementActualSize = (uint32)(keyElementTargetOffset + keyElementCopyLength);
                }

                RetVal = (Std_ReturnType)E_OK;

            /* If the target keyElement is marked as persistent, we need to request upper layer to update in Nvram the content of the blob storing key elements values and actual lengths */
#if (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)
                if (TRUE == Crypto_aKeyElementList[u32CryptoTargetKeyElementIdx].bCryptoKeyElementPersist)
                {
                    RetVal = Crypto_HandleNvramInfo(u32PartitionId, CRYPTO_NVRAM_OP_WRITE, CRYPTO_NVRAM_BLOB_1_ID, CRYPTO_KEYELEMENTCOPYPARTIAL_ID);
                }
#endif /* (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT) */

            }
            /* This case is used for Shared Secret source key type */
            else if ((CRYPTO_KEY_MATERIAL_U32 == keyElementId) && (CRYPTO_KEY_MATERIAL_U32 == targetKeyElementId))
            {
                if ((hseKeyType_t)HSE_KEY_TYPE_SHARED_SECRET == Crypto_aKeyElementList[u32CryptoKeyElementIdx].u8HseKeyType)
                {
                    /*  Extract/Move the specified length starting from specified offset from source key element(key material) to target source element(key material) */
                    RetVal = Crypto_Ipw_KeyDeriveExtract(u32PartitionId, u32CryptoKeyElementIdx, u32CryptoTargetKeyElementIdx,keyElementSourceOffset, keyElementCopyLength);
                    if (CRYPTO_RET_OPERATION_TIMEOUT <= RetVal)
                    {
                        Crypto_ReportErrorToDet((uint8)CRYPTO_KEYELEMENTCOPYPARTIAL_ID, (uint8)RetVal);
                        RetVal = (Std_ReturnType)E_NOT_OK;
                    }
                }
                else
                {
                    RetVal = (Std_ReturnType)E_NOT_OK;
                }
            }
            else
            {
                Crypto_ReportErrorToDet((uint8)CRYPTO_KEYELEMENTCOPYPARTIAL_ID, CRYPTO_RET_NOT_SUPPORTED);
                RetVal = (Std_ReturnType)E_NOT_OK;
            }
        }
    }
#else
    /* Avoid compiler warnings */
    (void) cryptoKeyId;
    (void) keyElementId;
    (void) keyElementSourceOffset;
    (void) keyElementTargetOffset;
    (void) keyElementCopyLength;
    (void) targetCryptoKeyId;
    (void) targetKeyElementId;
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */
    return RetVal;
}

/**
* @implements     Crypto_KeyCopy_Activity
*/
Std_ReturnType Crypto_KeyCopy
(
    uint32 cryptoKeyId,
    uint32 targetCryptoKeyId
)
{
    Std_ReturnType RetVal          = (Std_ReturnType)E_NOT_OK;
#if (STD_ON == CRYPTO_KEYS_EXIST)
    const uint32   u32PartitionId  = Crypto_Util_GetPartitionId();
    uint32         u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(cryptoKeyId);
    uint32         u32TargetKeyIdx = Crypto_Util_GetKeyIdx(targetCryptoKeyId);
    uint32         u32KeyMaterialElementIndex;
    uint32         u32TargetKeyMaterialElementIndex;

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_KEYCOPY_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    /* If cryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyCopy shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    /* If targetCryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyCopy shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    else if ((CRYPTO_INVALID_KEY_IDX_U32 == u32CryptoKeyIdx) || (CRYPTO_INVALID_KEY_IDX_U32 == u32TargetKeyIdx))
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYCOPY_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif
    {
        /* Verify if number of Crypto Key Elements is larger on the source than on target. Report E_NOT_OK if this happens */
        if (Crypto_aKeyList[u32CryptoKeyIdx].u32NumCryptoKeyElements > Crypto_aKeyList[u32TargetKeyIdx].u32NumCryptoKeyElements)
        {
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
        else
        {
            RetVal = (Std_ReturnType)Crypto_CheckKeyCopyValid(u32CryptoKeyIdx, u32TargetKeyIdx);
            if ((Std_ReturnType)CRYPTO_RET_NOT_SUPPORTED == RetVal)
            {
                Crypto_ReportErrorToDet((uint8)CRYPTO_KEYCOPY_ID, CRYPTO_RET_NOT_SUPPORTED);
                RetVal = (Std_ReturnType)E_NOT_OK;
            }
        }
        if ((Std_ReturnType)E_OK == RetVal)
        {
            /* Get the indexes for source and target element */
            u32KeyMaterialElementIndex        = Crypto_Util_GetKeyElementIdx (u32CryptoKeyIdx, CRYPTO_KEY_MATERIAL_U32);
            u32TargetKeyMaterialElementIndex  = Crypto_Util_GetKeyElementIdx (u32TargetKeyIdx, CRYPTO_KEY_MATERIAL_U32);

            RetVal = Crypto_CopyKeyElements(u32PartitionId, u32KeyMaterialElementIndex, u32CryptoKeyIdx, u32TargetKeyIdx,u32TargetKeyMaterialElementIndex);
            if (CRYPTO_RET_OPERATION_TIMEOUT <= RetVal)
            {
                Crypto_ReportErrorToDet((uint8)CRYPTO_KEYCOPY_ID, (uint8)RetVal);
                RetVal = (Std_ReturnType)E_NOT_OK;
            }
        }
    }
#else
    /* Avoid compiler warnings */
    (void) cryptoKeyId;
    (void) targetCryptoKeyId;
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

    return RetVal;
}

/**
* @implements     Crypto_KeyElementIdsGet_Activity
*/
Std_ReturnType Crypto_KeyElementIdsGet
(
    uint32  cryptoKeyId,
    uint32* keyElementIdsPtr,
    uint32* keyElementIdsLengthPtr
)
{
    Std_ReturnType RetVal            = (Std_ReturnType)E_NOT_OK;
#if (STD_ON == CRYPTO_KEYS_EXIST)
    uint32*        pu32KeyElementIds = keyElementIdsPtr;
    uint32         u32CryptoKeyIdx   = Crypto_Util_GetKeyIdx(cryptoKeyId);
    uint32         u32Index;

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
    const uint32   u32PartitionId  = Crypto_Util_GetPartitionId();

     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_KEYELEMENTIDSGET_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    /* If cryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyElementIdsGet shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    else if (CRYPTO_INVALID_KEY_IDX_U32 == u32CryptoKeyIdx)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTIDSGET_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
    else if ((NULL_PTR == pu32KeyElementIds) || (NULL_PTR == keyElementIdsLengthPtr))
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTIDSGET_ID, (uint8)CRYPTO_E_PARAM_POINTER);
    }
    else if (Crypto_aKeyList[u32CryptoKeyIdx].u32NumCryptoKeyElements > *keyElementIdsLengthPtr)
    {
        /* If the buffer keyElementIdsPtr is too small to store the result of the request,
        CRYPTO_E_SMALL_BUFFER shall be returned and if development error detection is enabled, CRYPTO_E_SMALL_BUFFER shall be reported to the DET. */
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYELEMENTIDSGET_ID, (uint8)CRYPTO_E_SMALL_BUFFER);
        RetVal = (Std_ReturnType)CRYPTO_E_SMALL_BUFFER;
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        *keyElementIdsLengthPtr = Crypto_aKeyList[u32CryptoKeyIdx].u32NumCryptoKeyElements;

        for (u32Index = 0U; u32Index < Crypto_aKeyList[u32CryptoKeyIdx].u32NumCryptoKeyElements; u32Index++)
        {
            *pu32KeyElementIds = Crypto_aKeyElementList[Crypto_aKeyList[u32CryptoKeyIdx].pCryptoKeyElementList[u32Index]].u32CryptoKeyElementId;
            pu32KeyElementIds++;
        }
        RetVal = (Std_ReturnType)E_OK;
    }
#else
    /* Avoid compiler warnings */
    (void) cryptoKeyId;
    (void) keyElementIdsPtr;
    (void) keyElementIdsLengthPtr;
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

    return RetVal;
}

/**
* @implements     Crypto_RandomSeed_Activity
*/
Std_ReturnType Crypto_RandomSeed
(
    uint32       cryptoKeyId,
    const uint8* seedPtr,
    uint32       seedLength
)
{
    Std_ReturnType RetVal          = (Std_ReturnType)E_NOT_OK;
#if (STD_ON == CRYPTO_KEYS_EXIST)
    const uint32   u32PartitionId  = Crypto_Util_GetPartitionId();
    const uint32   u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(cryptoKeyId);

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_RANDOMSEED_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    /* If u32CryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_RandomSeed shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    else if (CRYPTO_INVALID_KEY_IDX_U32 == u32CryptoKeyIdx)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_RANDOMSEED_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
    /* If the parameter seedPtr is a null pointer and if development error detection for the Crypto Driver is enabled, the function Crypto_RandomSeed shall report CRYPTO_E_PARAM_POINTER to the DET and return E_NOT_OK. */
    else if (NULL_PTR == seedPtr)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_RANDOMSEED_ID, (uint8)CRYPTO_E_PARAM_POINTER);
    }
    /* If seedLength is zero and if development error detection for the Crypto Driver is enabled, the function Crypto_RandomSeed shall report CRYPTO_E_PARAM_VALUE to the DET and return E_NOT_OK. */
    else if  (0U == seedLength)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_RANDOMSEED_ID, (uint8)CRYPTO_E_PARAM_VALUE);
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        RetVal = Crypto_GetNonDetErrorsForRandomSeed(u32CryptoKeyIdx);
        if ((Std_ReturnType)E_OK == RetVal)
        {
            RetVal = Crypto_Ipw_RandomSeed(u32PartitionId, u32CryptoKeyIdx, seedPtr, seedLength);
            if (CRYPTO_RET_OPERATION_TIMEOUT <= RetVal)
            {
                Crypto_ReportErrorToDet((uint8)CRYPTO_RANDOMSEED_ID, (uint8)RetVal);
                RetVal = (Std_ReturnType)E_NOT_OK;
            }
        }
    }
#else
    /* Avoid compiler warnings */
    (void) cryptoKeyId;
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

    /* Avoid compiler warnings */
    (void) seedPtr;
    (void) seedLength;

    return RetVal;
}

/**
* @implements     Crypto_KeyGenerate_Activity
*/
Std_ReturnType Crypto_KeyGenerate
(
    uint32 cryptoKeyId
)
{
    Std_ReturnType RetVal          = (Std_ReturnType)E_NOT_OK;
#if (STD_ON == CRYPTO_KEYS_EXIST)
    const uint32   u32PartitionId  = Crypto_Util_GetPartitionId();
    uint32         u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(cryptoKeyId);

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_KEYGENERATE_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    /* If the parameter cryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyGenerate shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    else if (CRYPTO_INVALID_KEY_IDX_U32 == u32CryptoKeyIdx)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYGENERATE_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
#if (STD_ON == CRYPTO_SPT_KEY_GENERATE)
        RetVal = Crypto_GetNonDetErrorsForKeyGenerate(u32CryptoKeyIdx);
        if ((Std_ReturnType)E_OK == RetVal)
#endif /* (STD_ON == CRYPTO_SPT_KEY_GENERATE) */
        {
            RetVal = Crypto_Ipw_KeyGenerate(u32PartitionId, u32CryptoKeyIdx);
            if (CRYPTO_RET_OPERATION_TIMEOUT <= RetVal)
            {
                Crypto_ReportErrorToDet((uint8)CRYPTO_KEYGENERATE_ID, (uint8)RetVal);
                RetVal = (Std_ReturnType)E_NOT_OK;
            }
        }
    }
#else
    /* Avoid compiler warnings */
    (void)cryptoKeyId;
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

    return RetVal;
}


/**
* @implements     Crypto_KeyDerive_Activity
*/
Std_ReturnType Crypto_KeyDerive
(
    uint32 cryptoKeyId,
    uint32 targetCryptoKeyId
)
{
    Std_ReturnType RetVal          = (Std_ReturnType)E_NOT_OK;
#if (STD_ON == CRYPTO_KEYS_EXIST)
    const uint32   u32PartitionId  = Crypto_Util_GetPartitionId();
    const uint32   u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(cryptoKeyId);
    const uint32   u32TargetKeyIdx = Crypto_Util_GetKeyIdx(targetCryptoKeyId);

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_KEYDERIVE_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    /* If the parameter cryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyDerive shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    /* If the parameter targetCryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyDerive shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK.*/
    else if ((CRYPTO_INVALID_KEY_IDX_U32 == u32CryptoKeyIdx) || (CRYPTO_INVALID_KEY_IDX_U32 == u32TargetKeyIdx))
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYDERIVE_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
#if (STD_ON == CRYPTO_SPT_KEY_DERIVE)
        RetVal = Crypto_GetNonDetErrorsForKeyDerive(u32CryptoKeyIdx, u32TargetKeyIdx);
        if ((Std_ReturnType)E_OK == RetVal)
#endif /* (STD_ON == CRYPTO_SPT_KEY_DERIVE) */
        {
            RetVal = Crypto_Ipw_KeyDerive(u32PartitionId, u32CryptoKeyIdx, u32TargetKeyIdx);
            if (CRYPTO_RET_OPERATION_TIMEOUT <= RetVal)
            {
                Crypto_ReportErrorToDet((uint8)CRYPTO_KEYDERIVE_ID, (uint8)RetVal);
                RetVal = (Std_ReturnType)E_NOT_OK;
            }
        }
    }
#else
    /* Avoid compiler warnings */
    (void)cryptoKeyId;
    (void)targetCryptoKeyId;
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

    return RetVal;
}

/**
* @implements     Crypto_KeyExchangeCalcPubVal_Activity
*/
Std_ReturnType Crypto_KeyExchangeCalcPubVal
(
    uint32  cryptoKeyId,
    uint8*  publicValuePtr,
    uint32* publicValueLengthPtr
)
{
    Std_ReturnType RetVal          = (Std_ReturnType)E_NOT_OK;
#if (STD_ON == CRYPTO_KEYS_EXIST)
    const uint32   u32PartitionId  = Crypto_Util_GetPartitionId();
    uint32         u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(cryptoKeyId);

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_KEYEXCHANGECALCPUBVAL_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    /* If the parameter cryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyExchangeCalcPubVal shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    else if (CRYPTO_INVALID_KEY_IDX_U32 == u32CryptoKeyIdx)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYEXCHANGECALCPUBVAL_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
    /* If the parameter publicValuePtr is a null pointer and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyExchangeCalcPubVal shall report CRYPTO_E_PARAM_POINTER to the DET and return E_NOT_OK. */
    else if (NULL_PTR == publicValuePtr)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYEXCHANGECALCPUBVAL_ID, (uint8)CRYPTO_E_PARAM_POINTER);
    }
    /* If the parameter pubValueLengthPtr is a null pointer and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyExchangeCalcPubVal shall report CRYPTO_E_PARAM_POINTER to the DET and return E_NOT_OK. */
    else if (NULL_PTR == publicValueLengthPtr)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYEXCHANGECALCPUBVAL_ID, (uint8)CRYPTO_E_PARAM_POINTER);
    }
    /* If the value, which is pointed by pubValueLengthPtr is zero and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyExchangeCalcPubVal shall report CRYPTO_E_PARAM_VALUE to the DET and return E_NOT_OK. */
    else if (0U == *publicValueLengthPtr)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYEXCHANGECALCPUBVAL_ID, (uint8)CRYPTO_E_PARAM_VALUE);
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        RetVal = Crypto_GetNonDetErrorsForKeyExchCalcPubVal(CRYPTO_KEYEXCHANGECALCPUBVAL_ID, u32CryptoKeyIdx, publicValuePtr, publicValueLengthPtr);
        if ((Std_ReturnType)E_OK == RetVal)
        {
            RetVal = Crypto_CopyDataFromPubValKeyElementToUser(u32PartitionId, u32CryptoKeyIdx, publicValuePtr, publicValueLengthPtr);
            if (CRYPTO_RET_OPERATION_TIMEOUT <= RetVal)
            {
                Crypto_ReportErrorToDet((uint8)CRYPTO_KEYEXCHANGECALCPUBVAL_ID, (uint8)RetVal);
                RetVal = (Std_ReturnType)E_NOT_OK;
            }
        }
    }
#else
    /* Avoid compiler warnings */
    (void)cryptoKeyId;
    (void)publicValuePtr;
    (void)publicValueLengthPtr;
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

    return RetVal;
}

/**
* @implements     Crypto_KeyExchangeCalcSecret_Activity
*/
Std_ReturnType Crypto_KeyExchangeCalcSecret
(
    uint32       cryptoKeyId,
    const uint8* partnerPublicValuePtr,
    uint32       partnerPublicValueLength
)
{
    Std_ReturnType RetVal          = (Std_ReturnType)E_NOT_OK;
#if (STD_ON == CRYPTO_KEYS_EXIST)
    const uint32   u32PartitionId  = Crypto_Util_GetPartitionId();
    const uint32   u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(cryptoKeyId);

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_KEYEXCHANGECALCSECRET_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    /* If the parameter cryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyExchangeCalcSecret shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    else if (CRYPTO_INVALID_KEY_IDX_U32 == u32CryptoKeyIdx)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYEXCHANGECALCSECRET_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
    /* If the parameter partnerPublicValuePtr is a null pointer and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyExchangeCalcSecret shall report CRYPTO_E_PARAM_POINTER to the DET and return E_NOT_OK. */
    else if (NULL_PTR == partnerPublicValuePtr)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYEXCHANGECALCSECRET_ID, (uint8)CRYPTO_E_PARAM_POINTER);
    }
    /* If partnerPublicValueLength is zero and if development error detection for the Crypto Driver is enabled, the function Crypto_KeyExchangeCalcSecret shall report CRYPTO_E_PARAM_VALUE to the DET and return E_NOT_OK. */
    else if (0U == partnerPublicValueLength)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_KEYEXCHANGECALCSECRET_ID, (uint8)CRYPTO_E_PARAM_VALUE);
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        RetVal =  Crypto_GetNonDetErrorsForKeyExchCalcSecret(u32CryptoKeyIdx);
        if ((Std_ReturnType)E_OK == RetVal)
        {
            RetVal = Crypto_Ipw_CalcSecret(u32PartitionId, u32CryptoKeyIdx, partnerPublicValuePtr, partnerPublicValueLength);
            if (CRYPTO_RET_OPERATION_TIMEOUT <= RetVal)
            {
                Crypto_ReportErrorToDet((uint8)CRYPTO_KEYEXCHANGECALCSECRET_ID, (uint8)RetVal);
                RetVal = (Std_ReturnType)E_NOT_OK;
            }
        }
    }
#else
    /* Avoid compiler warnings */
    (void)cryptoKeyId;
    (void)partnerPublicValuePtr;
    (void)partnerPublicValueLength;
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

    return RetVal;
}

/**
* @implements  Crypto_CertificateParse_Activity
*/
Std_ReturnType Crypto_CertificateParse
(
    uint32 cryptoKeyId
)
{
    Std_ReturnType RetVal          = (Std_ReturnType)E_NOT_OK;
#if (STD_ON == CRYPTO_KEYS_EXIST)
    const uint32   u32PartitionId  = Crypto_Util_GetPartitionId();
    uint32         u32CryptoKeyIdx = Crypto_Util_GetKeyIdx(cryptoKeyId);

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_CERTIFICATEPARSE_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    /* If the parameter cryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_CertificateParse shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    else if (CRYPTO_INVALID_KEY_IDX_U32 == u32CryptoKeyIdx)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_CERTIFICATEPARSE_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        RetVal = Crypto_Ipw_CertificateParse(u32PartitionId, u32CryptoKeyIdx);
        if (CRYPTO_RET_OPERATION_TIMEOUT <= RetVal)
        {
#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
            Crypto_ReportErrorToDet((uint8)CRYPTO_CERTIFICATEPARSE_ID, (uint8)RetVal);
#endif /*(STD_ON == CRYPTO_DEV_ERROR_DETECT)*/
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
    }
    (void)u32CryptoKeyIdx;
#else
    /* Avoid compiler warnings */
    (void)cryptoKeyId;
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

    return RetVal;
}

/**
* @implements  Crypto_CertificateVerify_Activity
*/
Std_ReturnType Crypto_CertificateVerify
(
    uint32                   cryptoKeyId,
    uint32                   verifyCryptoKeyId,
    Crypto_VerifyResultType* verifyPtr
)
{
    Std_ReturnType RetVal                = (Std_ReturnType)E_NOT_OK;
#if (STD_ON == CRYPTO_KEYS_EXIST)
    const uint32   u32PartitionId        = Crypto_Util_GetPartitionId();
    uint32         u32CryptoKeyIdx       = Crypto_Util_GetKeyIdx(cryptoKeyId);
    uint32         u32VerifyCryptoKeyIdx = Crypto_Util_GetKeyIdx(verifyCryptoKeyId);

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_CERTIFICATEVERIFY_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    /* If cryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_CertificateVerify shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    /* If verifyCryptoKeyId is out of range and if development error detection for the Crypto Driver is enabled, the function Crypto_CertificateVerify shall report CRYPTO_E_PARAM_HANDLE to the DET and return E_NOT_OK. */
    else if ((CRYPTO_INVALID_KEY_IDX_U32 == u32CryptoKeyIdx) || (CRYPTO_INVALID_KEY_IDX_U32 == u32VerifyCryptoKeyIdx))
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_CERTIFICATEVERIFY_ID, (uint8)CRYPTO_E_PARAM_HANDLE);
    }
    /* If the parameter verifyPtr is a null pointer and if development error detection for the Crypto Driver is enabled, the function Crypto_CertificateVerify shall report CRYPTO_E_PARAM_POINTER to the DET and return E_NOT_OK. */
    else if (NULL_PTR == verifyPtr)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_CERTIFICATEVERIFY_ID, (uint8)CRYPTO_E_PARAM_POINTER);
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        RetVal = Crypto_Ipw_CertificateVerify(u32PartitionId, u32CryptoKeyIdx, u32VerifyCryptoKeyIdx, verifyPtr);
        if (CRYPTO_RET_OPERATION_TIMEOUT <= RetVal)
        {
#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
            Crypto_ReportErrorToDet((uint8)CRYPTO_CERTIFICATEVERIFY_ID, (uint8)RetVal);
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
        (void)u32CryptoKeyIdx;
        (void)u32VerifyCryptoKeyIdx;
    }
#else
    /* Avoid compiler warnings */
    (void)cryptoKeyId;
    (void)verifyCryptoKeyId;
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

    /* Avoid compiler warnings */
    (void)verifyPtr;

    return RetVal;
}

/**
* @brief            If asynchronous job processing is configured and there are job queues, the function is called cyclically to process queued jobs.
* @details          If asynchronous job processing is configured and there are job queues, the function is called cyclically to process queued jobs.
*
* @param[in]        none
*
* @return           void
*
*
* @pre
*/
/**
* @implements  Crypto_MainFunction_Activity
*/
void Crypto_MainFunction (void)
{
    const uint32 u32PartitionId = Crypto_Util_GetPartitionId();

    /* SWS_BSW_00037 - If the module is not initialized and the main function is executed then no error shall be reported and the main function shall return immediately. */
    if (CRYPTO_DRIVER_INITIALIZED == Crypto_aeDriverState[u32PartitionId])
    {
        Crypto_Ipw_MainFunction(u32PartitionId);
    }
}

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
/**
* @brief           Checks if call is done from a valid core and driver is initialized
* @details         Checks if call is done from a valid core and driver is initialized
*
* @param[in]       u32PartitionId - Id of the partition where fromt this call is made
* @param[in]       u8ApiId        - Id of the caller API
*
* @return          E_OK:     Request successful
*                  E_NOT_OK: Request failed
*/
Std_ReturnType Crypto_CheckValidCoreAndDrvInitForNonCdoApis
(
    const uint32 u32PartitionId,
    const uint8  u8ApiId
)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == CRYPTO_ENABLE_MULTICORE_SUPPORT)
    /* Check if the partition this call is coming from is valid. There should be at least one Crypto Driver Object mapped on the partition in order for the call to be valid */
    if (0U == Crypto_aPartitionToCdoMapping[u32PartitionId].u8NumCDOs)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, u8ApiId, (uint8)CRYPTO_E_PARAM_CONFIG);
    }
    /* Check whether the Crypto driver is in CRYPTO_DRIVER_UNINIT state */
    else
#endif /* (STD_ON == CRYPTO_ENABLE_MULTICORE_SUPPORT) */
    {
        if (CRYPTO_DRIVER_UNINIT == Crypto_aeDriverState[u32PartitionId])
        {
            (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, u8ApiId, (uint8)CRYPTO_E_UNINIT);
        }
        else
        {
            RetVal = (Std_ReturnType)E_OK;
        }
    }

    return RetVal;
}
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */

/**
* @brief           Reports to DET detected errros.
* @details         Reports to DET the extended development and runtime errros.
*
* @param[in]       u8ServiceId - The API service ID
* @param[in]       u8Error     - Crypto error extension to Std_ReturnType to be reported via DET
*
* @return          void
*/

void Crypto_ReportErrorToDet
(
    uint8 u8ServiceId,
    uint8 u8Error
)
{
    switch (u8Error)
    {
#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
        /* Development error report*/
        case CRYPTO_RET_NOT_SUPPORTED:
            (void) Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, u8ServiceId, CRYPTO_E_NOT_SUPPORTED);
            break;

        case CRYPTO_RET_INVALID_PARAM:
            (void) Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, u8ServiceId, CRYPTO_E_INVALID_PARAM);
            break;
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
        /* Runtime error report */
        case CRYPTO_RET_OPERATION_TIMEOUT:
            (void) Det_ReportRuntimeError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, u8ServiceId, CRYPTO_E_RE_OPERATION_TIMEOUT);
            break;

        case CRYPTO_RET_STREAM_BUSY:
            (void) Det_ReportRuntimeError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, u8ServiceId, CRYPTO_E_RE_STREAM_BUSY);
            break;

        default:
            /* Should not get here */
            break;
    }
}

#define CRYPTO_STOP_SEC_CODE
#include "Crypto_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */


