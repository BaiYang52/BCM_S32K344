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

#ifndef CRYPTO_UTIL_H
#define CRYPTO_UTIL_H

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
#include "Crypto_Cfg.h"
#include "Crypto_Private.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CRYPTO_UTIL_VENDOR_ID                       43
#define CRYPTO_UTIL_AR_RELEASE_MAJOR_VERSION        4
#define CRYPTO_UTIL_AR_RELEASE_MINOR_VERSION        4
#define CRYPTO_UTIL_AR_RELEASE_REVISION_VERSION     0
#define CRYPTO_UTIL_SW_MAJOR_VERSION                2
#define CRYPTO_UTIL_SW_MINOR_VERSION                0
#define CRYPTO_UTIL_SW_PATCH_VERSION                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if Crypto_Util header file and Crypto configuration header file are of the same vendor */
#if (CRYPTO_UTIL_VENDOR_ID != CRYPTO_VENDOR_ID_CFG)
    #error "Crypto_Util.h and Crypto_Cfg.h have different vendor ids"
#endif

/* Check if Crypto_Util header file and Crypto configuration header file are of the same Autosar version */
#if ((CRYPTO_UTIL_AR_RELEASE_MAJOR_VERSION    != CRYPTO_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (CRYPTO_UTIL_AR_RELEASE_MINOR_VERSION    != CRYPTO_AR_RELEASE_MINOR_VERSION_CFG) || \
     (CRYPTO_UTIL_AR_RELEASE_REVISION_VERSION != CRYPTO_AR_RELEASE_REVISION_VERSION_CFG) \
    )
    #error "AutoSar Version Numbers of Crypto_Util.h and Crypto_Cfg.h are different"
#endif

/* Check if Crypto_Util header file and Crypto configuration header file are of the same Software version */
#if ((CRYPTO_UTIL_SW_MAJOR_VERSION != CRYPTO_SW_MAJOR_VERSION_CFG) || \
     (CRYPTO_UTIL_SW_MINOR_VERSION != CRYPTO_SW_MINOR_VERSION_CFG) || \
     (CRYPTO_UTIL_SW_PATCH_VERSION != CRYPTO_SW_PATCH_VERSION_CFG) \
    )
    #error "Software Version Numbers of Crypto_Util.h and Crypto_Cfg.h are different"
#endif

/* Check if Crypto_Util header file and Crypto private header file are of the same vendor */
#if (CRYPTO_UTIL_VENDOR_ID != CRYPTO_VENDOR_ID_PRIVATE)
    #error "Crypto_Util.h and Crypto_Private.h have different vendor ids"
#endif

/* Check if Crypto_Util header file and Crypto private header file are of the same Autosar version */
#if ((CRYPTO_UTIL_AR_RELEASE_MAJOR_VERSION    != CRYPTO_AR_RELEASE_MAJOR_VERSION_PRIVATE) || \
     (CRYPTO_UTIL_AR_RELEASE_MINOR_VERSION    != CRYPTO_AR_RELEASE_MINOR_VERSION_PRIVATE) || \
     (CRYPTO_UTIL_AR_RELEASE_REVISION_VERSION != CRYPTO_AR_RELEASE_REVISION_VERSION_PRIVATE) \
    )
    #error "AutoSar Version Numbers of Crypto_Util.h and Crypto_Private.h are different"
#endif

/* Check if Crypto_Util header file and Crypto private header file are of the same Software version */
#if ((CRYPTO_UTIL_SW_MAJOR_VERSION != CRYPTO_SW_MAJOR_VERSION_PRIVATE) || \
     (CRYPTO_UTIL_SW_MINOR_VERSION != CRYPTO_SW_MINOR_VERSION_PRIVATE) || \
     (CRYPTO_UTIL_SW_PATCH_VERSION != CRYPTO_SW_PATCH_VERSION_PRIVATE)    \
    )
    #error "Software Version Numbers of Crypto_Util.h and Crypto_Private.h are different"
#endif
/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/* Value used to mark the end of a job queue */
#define CRYPTO_JOB_QUEUE_NONE_U32               ((uint32)0xFFFFFFFFU)

/* Reserved value associated with invalid Crypto Driver Object id */
#define CRYPTO_INVALID_CDO_IDX_U32              ((uint32)0xFFFFFFFFU)

#if (STD_ON == CRYPTO_KEYS_EXIST)
    /* Reserved value associated with invalid Crypto Key id */
    #define CRYPTO_INVALID_KEY_IDX_U32          ((uint32)0xFFFFFFFFU)

    /* Reserved value associated with invalid Crypto Key Element id */
    #define CRYPTO_INVALID_KEY_ELEMENT_IDX_U32  ((uint32)0xFFFFFFFFU)
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */


/* Function-like macro for getting the index of a Crypto Driver Object from the list of Crypto Driver Objects, based on the id */
#define Crypto_Util_GetObjectIdx(driverObjectId)      (((driverObjectId) < CRYPTO_NUMBER_OF_DRIVER_OBJECTS_U32) ? (driverObjectId) : CRYPTO_INVALID_CDO_IDX_U32)

#if (STD_ON == CRYPTO_KEYS_EXIST)
    /* Function-like macro for getting the id of the key inside the Crypto job */
    #if (STD_ON == CRYPTO_ENABLE_ALTERNATE_MAPPING_FOR_JOB_KEY)
        #define Crypto_Util_GetJobKey(pJob)           (pJob->cryptoKeyId)
    #else
        #define Crypto_Util_GetJobKey(pJob)           (pJob->jobPrimitiveInfo->cryIfKeyId)
    #endif
    
    /* Function-like macro for getting the index of a Crypto Key from the list of Crypto Keys based on the id */
    #define Crypto_Util_GetKeyIdx(keyId)              (((keyId) < CRYPTO_NUMBER_OF_KEYS_U32) ? (keyId) : CRYPTO_INVALID_KEY_IDX_U32)
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

/* Function-like macro for getting the id of the partition the code is running on */
#if (STD_ON == CRYPTO_ENABLE_MULTICORE_SUPPORT)
    #define Crypto_Util_GetPartitionId()        ((uint32)OsIf_GetCoreID())
#else
    #define Crypto_Util_GetPartitionId()        ((uint32)0U)
#endif /* (STD_ON == CRYPTO_ENABLE_MULTICORE_SUPPORT) */

#if ((STD_ON == CRYPTO_SPT_KEY_DERIVE) || (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT))
    /* Index of the byte storing the Key Derive Algorithm value in the buffer of the CRYPTO_KE_KEYDERIVATION_ALGORITHM key element */
    #define CRYPTO_KEY_DERIVE_ALGORITHM_IDX             (0U)
#endif /* ((STD_ON == CRYPTO_SPT_KEY_DERIVE) || (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT)) */

#if (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT)
    /* Index of the Key Material Length (used in CRYPTO_ALGOFAM_TLS12_KEY_EXPANSION) value in the buffer of the CRYPTO_KE_KEYDERIVATION_OUTPUT_SIZE key element */
    #define CRYPTO_TLS12_KEY_MATERIAL_LENGTH_IDX    (0U)
    /* Index of the IV Length (used in CRYPTO_ALGOFAM_TLS12_KEY_EXPANSION) value in the buffer of the CRYPTO_KE_KEYDERIVATION_OUTPUT_SIZE key element */
    #define CRYPTO_TLS12_IV_LENGTH_IDX              (1U)
    /* Index of the Verify Data Length (used in CRYPTO_ALGOFAM_TLS12_GENERATE_VERIFY_CLIENT/CRYPTO_ALGOFAM_TLS12_GENERATE_VERIFY_SERVER) value in the buffer of the CRYPTO_KE_KEYDERIVATION_OUTPUT_SIZE key element */
    #define CRYPTO_TLS12_VERIFY_DATA_LENGTH_IDX     (0U)
#endif /* (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT) */

#if ((STD_ON == CRYPTO_KEYS_EXIST)&&(STD_ON == CRYPTO_ENABLE_REDIRECTION_SUPPORT))
    /* Function-like macro for getting the redirection config type */
    #define Crypto_Util_IsRedirectionSet(redirectionConfig, redirectionType)   (0U != ((uint8)(redirectionConfig) & (uint8)(redirectionType)))
#endif


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
*                                       FUNCTION PROTOTYPES
==================================================================================================*/
#define CRYPTO_START_SEC_CODE
#include "Crypto_MemMap.h"

/**
* @brief           Memcopy function
* @details         Memcopy function
*
* @param[in]       pu8Dest   - address of the destination
* @param[in]       pu8Source - address of the source
* @param[in]       u32Size   - size to be copied
*
* @returns         void
*
*
* @pre
*
*/
void Crypto_Util_MemCpy8
(
    uint8*       pu8Dest,
    const uint8* pu8Source,
    uint32       u32Size
);

/**
* @brief           Initialize the CDO job queues
* @details         Each CDO has 2 queues, one with free elements, the second containing jobs sorted by priority.
*                  For each of the CDO queues, only the first element (head) is stored, as each queue element has a reference to the next one.
*                  This function initializes the queues heads and chains the elements in the free jobs queues
*
* @param[in]       void
* @returns         void
*
*
* @pre
*
*/
void Crypto_Util_InitJobQueues(void);

/**
* @brief           Adds a job in the jobs queue
* @details         Adds a job in the jobs queue, if at least one element is found in the queue of free jobs. The job is inserted in the queue
*                  based on its priority
*
* @param[in]       u32ObjectIdx: identifier of the Crypto Driver Object to be used for queuing the job
* @param[in]       pJob:        pointer to the job to be queued
* @returns         boolean
*                   TRUE  - job has been queued
*                   FALSE - job could not be queued
*
*
* @pre
*
*/
boolean Crypto_Util_QueueJob
(
    uint32          u32ObjectIdx,
    Crypto_JobType* pJob
);

/**
* @brief           Dequeues the first job in the queue
* @details         Removes the first queued job (head) and adds it in the queue of free jobs
*
* @param[in]       u32ObjectIdx: identifier of the Crypto Driver Object to be used for dequeuing the job
* @returns         void
*
*
* @pre            Assumes that at least one element exists in the queued jobs
*
*/
void Crypto_Util_DequeueHeadJob
(
    uint32 u32ObjectIdx
);

/**
* @brief           Dequeues from the list of queued jobs the first found job with the given jobId
* @details         Searches the queued jobs for a job with a given jobId and if found, removes it
*
* @param[in]       u32ObjectIdx: identifier of the Crypto Driver Object to be used for dequeuing the job
* @param[in]       pJobInfo:     pointer to jobInfo structure containing the jobId to search for
* @returns         Std_ReturnType
*
*
* @pre
*
*/
Std_ReturnType Crypto_Util_DequeueJob
(
    uint32                    u32ObjectIdx,
    const Crypto_JobInfoType* pJobInfo
);

#if (STD_ON == CRYPTO_KEYS_EXIST)
#if (STD_ON == CRYPTO_SPT_ECC_KEY_PAIR_GEN)
boolean Crypto_Util_MemCmp
(
    const uint8* pu8Source,
    const uint8* pu8Dest,
    const uint32 u32Size
);
#endif /* (STD_ON == CRYPTO_SPT_ECC_KEY_PAIR_GEN) */

#if ((STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT))
void Crypto_Util_SwapArrayBytes
(
    uint8* pu8Source,
    uint32 u32Len
);
#endif /* ((STD_ON == CRYPTO_SPT_ECC_KEY_IMPORT) || (STD_ON == CRYPTO_SPT_ECC_KEY_EXPORT)) */
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

#if (STD_ON == CRYPTO_KEYS_EXIST)
uint32 Crypto_Util_GetKeyElementIdx
(
    uint32 u32KeyIndex,
    uint32 u32keyElementId
);
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */



#define CRYPTO_STOP_SEC_CODE
#include "Crypto_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CRYPTO_UTIL_H */


