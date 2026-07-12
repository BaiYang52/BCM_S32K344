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

#ifndef CRYPTO_H
#define CRYPTO_H

/**
*   @file
*   @implements Crypto.h_Artifact
*
*   @addtogroup CRYPTO_ASR
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
#include "CryIf_Cbk.h"
#include "Csm_Types.h"
#include "Crypto_ASRExtension.h"
#include "SchM_Crypto.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CRYPTO_VENDOR_ID                       43
#define CRYPTO_MODULE_ID                       114
#define CRYPTO_AR_RELEASE_MAJOR_VERSION        4
#define CRYPTO_AR_RELEASE_MINOR_VERSION        4
#define CRYPTO_AR_RELEASE_REVISION_VERSION     0
#define CRYPTO_SW_MAJOR_VERSION                2
#define CRYPTO_SW_MINOR_VERSION                0
#define CRYPTO_SW_PATCH_VERSION                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if Crypto header file and Crypto configuration header file are of the same vendor */
#if (CRYPTO_VENDOR_ID != CRYPTO_VENDOR_ID_CFG)
    #error "Crypto.h and Crypto_Cfg.h have different vendor ids"
#endif

/* Check if Crypto header file and Crypto configuration header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION    != CRYPTO_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION    != CRYPTO_AR_RELEASE_MINOR_VERSION_CFG) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION != CRYPTO_AR_RELEASE_REVISION_VERSION_CFG) \
    )
    #error "AutoSar Version Numbers of Crypto.h and Crypto_Cfg.h are different"
#endif

/* Check if Crypto header file and Crypto configuration header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION != CRYPTO_SW_MAJOR_VERSION_CFG) || \
     (CRYPTO_SW_MINOR_VERSION != CRYPTO_SW_MINOR_VERSION_CFG) || \
     (CRYPTO_SW_PATCH_VERSION != CRYPTO_SW_PATCH_VERSION_CFG)    \
    )
    #error "Software Version Numbers of Crypto.h and Crypto_Cfg.h are different"
#endif

/* Check if Crypto header file and Crypto Autosar extension header file are of the same vendor */
#if (CRYPTO_VENDOR_ID != CRYPTO_VENDOR_ID_ASREXTENSION_H)
    #error "Crypto.h and Crypto_ASRExtension.h have different vendor ids"
#endif

/* Check if Crypto header file and Crypto Autosar extension header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION    != CRYPTO_AR_RELEASE_MAJOR_VERSION_ASREXTENSION_H) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION    != CRYPTO_AR_RELEASE_MINOR_VERSION_ASREXTENSION_H) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION != CRYPTO_AR_RELEASE_REVISION_VERSION_ASREXTENSION_H) \
    )
    #error "AutoSar Version Numbers of Crypto.h and Crypto_ASRExtension.h are different"
#endif

/* Check if Crypto header file and Crypto Autosar extension header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION != CRYPTO_SW_MAJOR_VERSION_ASREXTENSION_H) || \
     (CRYPTO_SW_MINOR_VERSION != CRYPTO_SW_MINOR_VERSION_ASREXTENSION_H) || \
     (CRYPTO_SW_PATCH_VERSION != CRYPTO_SW_PATCH_VERSION_ASREXTENSION_H)    \
    )
    #error "Software Version Numbers of Crypto.h and Crypto_ASRExtension.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if header file and CryIf_Cbk header file are of the same AutoSar version */
    #if ((CRYPTO_AR_RELEASE_MAJOR_VERSION != CRYIF_CBK_AR_RELEASE_MAJOR_VERSION) || \
         (CRYPTO_AR_RELEASE_MINOR_VERSION != CRYIF_CBK_AR_RELEASE_MINOR_VERSION)    \
        )
        #error "AutoSar Version Numbers of Crypto.h and CryIf_Cbk.h are different"
    #endif

    /* Check if header file and Csm types header file are of the same AutoSar version */
    #if ((CRYPTO_AR_RELEASE_MAJOR_VERSION != CSM_AR_RELEASE_MAJOR_VERSION_TYPES) || \
         (CRYPTO_AR_RELEASE_MINOR_VERSION != CSM_AR_RELEASE_MINOR_VERSION_TYPES)    \
        )
        #error "AutoSar Version Numbers of Crypto.h and Csm_Types.h are different"
    #endif

    /* Check if Crypto header file and SchM_Crypto header file are of the same Autosar version */
    #if ((CRYPTO_AR_RELEASE_MAJOR_VERSION != SCHM_CRYPTO_AR_RELEASE_MAJOR_VERSION) || \
         (CRYPTO_AR_RELEASE_MINOR_VERSION != SCHM_CRYPTO_AR_RELEASE_MINOR_VERSION)    \
        )
        #error "AutoSar Version Numbers of Crypto.h and SchM_Crypto.h are different"
    #endif
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/*
* @brief Development error codes (passed to DET).
*/
#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
    /**
    * @brief API request called before initialization of Crypto Driver.
    * @implements    CRYPTO_DET_ERRORS_define
    * */
    #define CRYPTO_E_UNINIT                     ((uint8)0x00U)

    /**
    * @brief Initiation of Crypto Driver failed.
    * */
    #define CRYPTO_E_INIT_FAILED                ((uint8)0x01U)

    /**
    * @brief API request called with invalid parameter (Nullpointer).
    * */
    #define CRYPTO_E_PARAM_POINTER              ((uint8)0x02U)

    /**
    * @brief API request called with invalid parameter (out of range).
    * */
    #define CRYPTO_E_PARAM_HANDLE               ((uint8)0x04U)

    /**
    * @brief API request called with invalid parameter (invalid value).
    * */
    #define CRYPTO_E_PARAM_VALUE                ((uint8)0x05U)

#if (STD_ON == CRYPTO_ENABLE_MULTICORE_SUPPORT)
    /**
    * @brief The Crypto module is not properly configured (Extension of Development Errors).
    * */
    #define CRYPTO_E_PARAM_CONFIG               ((uint8)0x80U)
#endif /* (STD_ON == CRYPTO_ENABLE_MULTICORE_SUPPORT) */

    /**
    * @brief The service request failed because it is not supported by the driver (Extension of Development Errors).
    * */
    #define CRYPTO_E_NOT_SUPPORTED              ((uint8)0x81)

    /**
    * @brief The service request failed because at least one parameter is invalid (Extension of Development Errors).
    * */
    #define CRYPTO_E_INVALID_PARAM              ((uint8)0x82)

#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */


/**
* @brief Runtime error codes (passed to DET).
*/
/**
* @brief Buffer is too small for operation.
* */
#define CRYPTO_E_RE_SMALL_BUFFER                ((uint8)0x00U)

#if (STD_ON == CRYPTO_KEYS_EXIST)
    /**
    * @brief Requested key is not available.
    * */
    #define CRYPTO_E_RE_KEY_NOT_AVAILABLE       ((uint8)0x01U)

    /**
    * @brief Key cannot be read.
    * */
    #define CRYPTO_E_RE_KEY_READ_FAIL           ((uint8)0x02U)
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

/**
* @brief Entropy is too low.
* */
#define CRYPTO_E_RE_ENTROPY_EXHAUSTED           ((uint8)0x03U)

/**
* @brief The service request failed because timeout occurred (Extension of Runtime Errors).
* */
#define CRYPTO_E_RE_OPERATION_TIMEOUT           ((uint8)0x80)

/**
* @brief The service request failed because there was no stream available for the job (Extension of Runtime Errors).
* */
#define CRYPTO_E_RE_STREAM_BUSY                 ((uint8)0x81)

#if ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT))
    /**
    * @brief The service request failed because the application defined function repoted an error (Extension of Runtime Errors).
    * */
    #define CRYPTO_E_RE_NVRAM_OPERATION_FAIL    ((uint8)0x82)
#endif /* ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)) */

/**
* @brief AUTOSAR API's service IDs.
*/
#if ((STD_ON == CRYPTO_DEV_ERROR_DETECT) || ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)))
    /**
    * @brief API service ID for Crypto_Init function.
    * */
    #define CRYPTO_INIT_ID                      ((uint8)0x00U)
#endif /* ((STD_ON == CRYPTO_DEV_ERROR_DETECT) || ((STD_ON == CRYPTO_KEYS_EXIST) && (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT))) */

#if ((STD_ON == CRYPTO_DEV_ERROR_DETECT) && (STD_ON == CRYPTO_VERSION_INFO_API))
    /**
    * @brief API service ID for Crypto_GetVersionInfo function.
    * */
    #define CRYPTO_GETVERSIONINFO_ID            ((uint8)0x01U)
#endif /* ((STD_ON == CRYPTO_DEV_ERROR_DETECT) && (STD_ON == CRYPTO_VERSION_INFO_API)) */

/**
* @brief API service ID for Crypto_ProcessJob function.
* */
#define CRYPTO_PROCESSJOB_ID                    ((uint8)0x03U)

/**
* @brief API service ID for Crypto_CancelJob function.
* */
#define CRYPTO_CANCELJOB_ID                     ((uint8)0x0EU)

#if (STD_ON == CRYPTO_KEYS_EXIST)
    /**
    * @brief API service ID for Crypto_KeySetValid function.
    * */
    #define CRYPTO_KEYSETVALID_ID               ((uint8)0x05U)

    /**
    * @brief API service ID for Crypto_KeyElementSet function.
    * */
    #define CRYPTO_KEYELEMENTSET_ID             ((uint8)0x04U)

    /**
    * @brief API service ID for Crypto_KeyElementCopy function.
    * */
    #define CRYPTO_KEYELEMENTCOPY_ID            ((uint8)0x0FU)

    /**
    * @brief API service ID for Crypto_KeyCopy function.
    * */
    #define CRYPTO_KEYCOPY_ID                   ((uint8)0x10U)

    /**
    * @brief API service ID for Crypto_KeyElementCopyPartial function.
    * */
    #define CRYPTO_KEYELEMENTCOPYPARTIAL_ID     ((uint8)0x13U)

    #if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
        /**
        * @brief API service ID for Crypto_KeyElementIdsGet function.
        * */
        #define CRYPTO_KEYELEMENTIDSGET_ID      ((uint8)0x11U)

        /**
        * @brief API service ID for Crypto_CertificateParse function.
        * */
        #define CRYPTO_CERTIFICATEPARSE_ID      ((uint8)0x0BU)

        /**
        * @brief API service ID for Crypto_CertificateVerify function.
        * */
        #define CRYPTO_CERTIFICATEVERIFY_ID     ((uint8)0x12U)
    #endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */

    /**
    * @brief API service ID for Crypto_KeyDerive function.
    * */
    #define CRYPTO_KEYDERIVE_ID                 ((uint8)0x08U)

    /**
    * @brief API service ID for Crypto_KeyExchangeCalcSecret function.
    * */
    #define CRYPTO_KEYEXCHANGECALCSECRET_ID     ((uint8)0x0AU)

    /**
    * @brief API service ID for Crypto_KeyGenerate function.
    * */
    #define CRYPTO_KEYGENERATE_ID               ((uint8)0x07U)

    /**
    * @brief API service ID for Crypto_RandomSeed function.
    * */
    #define CRYPTO_RANDOMSEED_ID                ((uint8)0x0DU)

    /**
    * @brief API service ID for Crypto_KeyElementGet function.
    * */
    #define CRYPTO_KEYELEMENTGET_ID             ((uint8)0x06U)
    /**
    * @brief API service ID for Crypto_KeyExchangeCalcPubVal function.
    * */
    #define CRYPTO_KEYEXCHANGECALCPUBVAL_ID     ((uint8)0x09U)
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

/* Check which define is used by CSM */
#ifndef CYRPTO_KE_KEYEXCHANGE_SHAREDVALUE
    /**
    * @brief Redefine the fixed key element name to the one used by the driver.
    * */
    #define CYRPTO_KE_KEYEXCHANGE_SHAREDVALUE     CRYPTO_KE_KEYEXCHANGE_SHAREDVALUE
#endif

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
* @brief Configuration data structure of Crypto module.
*
* @implements       Crypto_ConfigType_typedef
*/
typedef void Crypto_ConfigType;

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/
#define CRYPTO_START_SEC_CODE
#include "Crypto_MemMap.h"

/**
* @brief            Initializes the Crypto Driver.
* @details          Initializes the internal variables of the driver, initializes the MU communication layer.
*
* @param[in]        configPtr    Holds the pointer to the configuration data structure of CryIf module
*
* @return           void
*
*
* @pre
*
*/
void Crypto_Init
(
    const Crypto_ConfigType* configPtr
);

#if (CRYPTO_VERSION_INFO_API == STD_ON)
/**
* @brief            Returns the version information of this module.
* @details          Writes the version information attributes of this module in the location pointed by versioninfo parameter.
*
* @param[inout]     versioninfo     Pointer where to store the version information of this module
*
* @return           void
*
*
* @pre
*
*/
void Crypto_GetVersionInfo
(
    Std_VersionInfoType* versioninfo
);
#endif  /* CRYPTO_VERSION_INFO_API == STD_ON */

/**
* @brief            Performs the crypto primitive that is configured in the job parameter.
* @details          Performs the crypto primitive, that is configured in the job parameter.
*
* @param[in]        objectId    Holds the identifier of the Crypto Driver Object
* @param[inout]     job         Pointer to the configuration of the job. Contains structures with job and primitive relevant information but also pointer to result buffers.
*
* @return           Result of the operation
* @retval           E_OK                        Request successful
* @retval           E_NOT_OK                    Request failed
* @retval           CRYPTO_E_BUSY               Request failed, Crypro Driver Object is Busy
* @retval           CRYPTO_E_KEY_NOT_VALID      Request failed, the key is not valid
* @retval           CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, a key element has the wrong size
* @retval           CRYPTO_E_QUEUE_FULL         Request failed, the queue is full
* @retval           CRYPTO_E_ENTROPY_EXHAUSTION Request failed, the entropy is exhausted
* @retval           CRYPTO_E_SMALL_BUFFER       The provided buffer is too small to store the result
* @retval           CRYPTO_E_JOB_CANCELED       The service request failed because the synchronous Job has been canceled
*
*
* @pre
*
*/
Std_ReturnType Crypto_ProcessJob
(
    uint32          objectId,
    Crypto_JobType* job
);

/**
* @brief           This interface removes the provided job from the queue and cancels the processing of the job if possible.
* @details         This interface removes the provided job from the queue and cancels the processing of the job if possible.
*
* @param[in]       objectId     Holds the identifier of the Crypto Driver Object.
* @param[inout]    job          Pointer to the configuration of the job. Contains structures with job and primitive relevant information.

* @return           Result of the operation
* @retval           E_OK        Request successful, job has been removed
* @retval           E_NOT_OK    Request failed, job couldn't be removed
*
*
* @pre
*
*/
Std_ReturnType Crypto_CancelJob
(
    uint32              objectId,
    Crypto_JobInfoType* job
);

/**
* @brief            Sets the given key element bytes to the key identified by cryptoKeyId.
* @details          Sets the given key element bytes to the key identified by cryptoKeyId.
*
* @param[in]        cryptoKeyId      Holds the identifier of the key whose key element shall be set
* @param[in]        keyElementId     Holds the identifier of the key element which shall be set
* @param[in]        keyPtr           Holds the pointer to the key data which shall be set as key element
* @param[in]        keyLength        Contains the length of the key element in bytes
*
* @return           Result of the operation
* @retval           E_OK                        Request successful
* @retval           E_NOT_OK                    Request failed
* @retval           CRYPTO_E_BUSY               Request failed, Crypto Driver Object is busy
* @retval           CRYPTO_E_KEY_WRITE_FAIL     Request failed because write access was denied
* @retval           CRYPTO_E_KEY_NOT_AVAILABLE  Request failed because the key is not available
* @retval           CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, key element size does not match size of provided data
*
*
* @pre
*
*/
Std_ReturnType Crypto_KeyElementSet
(
    uint32       cryptoKeyId,
    uint32       keyElementId,
    const uint8* keyPtr,
    uint32       keyLength
);

/**
* @brief            Sets the key state of the key identified by cryptoKeyId to valid.
* @details          Sets the key state of the key identified by cryptoKeyId to valid.
*
* @param[in]        cryptoKeyId  Holds the identifier of the key which shall be set to valid
*
* @return           Result of the operation
* @retval           E_OK            Request successful
* @retval           E_NOT_OK        Request failed
* @retval           CRYPTO_E_BUSY   Request failed, Crypto Driver Object is busy
*
*
* @pre
*
*/
Std_ReturnType Crypto_KeySetValid
(
    uint32 cryptoKeyId
);

/**
* @brief            This interface shall be used to get a key element of the key identified by the cryptoKeyId and store the key element in the memory location pointed by the result pointer.
* @details          This interface shall be used to get a key element of the key identified by the cryptoKeyId and store the key element in the memory location pointed by the result pointer.
*                   Note: If the actual key element is directly mapped to flash memory, there could be a bigger delay when calling this function (synchronous operation).
*
* @param[in]        cryptoKeyId      Holds the identifier of the key whose key element shall be returned
* @param[in]        keyElementId     Holds the identifier of the key element which shall be returned
* @param[out]       resultPtr        Holds the pointer of the buffer for the returned key element
* @param[inout]     resultLengthPtr  Holds a pointer to a memory location in which the length information is stored. On calling this function this parameter shall contain the size of the buffer provided by resultPtr.
*                                    If the key element is configured to allow partial access, this parameter contains the amount of data which should be read from the key element.
*                                    The size may not be equal to the size of the provided buffer anymore. When the request has finished, the amount of data that has been stored shall be stored.
*                                    If the key identified by the cryptoKeyId is exported authenticated this parameter shall have the size of the exported key because the tag or signature will be generated over this length.
*
* @return           Result of the operation
* @retval           E_OK                        Request successful
* @retval           E_NOT_OK                    Request failed
* @retval           CRYPTO_E_BUSY               Request failed, Crypto Driver Object is busy
* @retval           CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the requested key element is not available
* @retval           CRYPTO_E_KEY_READ_FAIL      Request failed because read access was denied
* @retval           CRYPTO_E_SMALL_BUFFER       The provided buffer is too small to store the result
* @retval           CRYPTO_E_KEY_EMPTY          Request failed, source key element is uninitialized
*
*
* @pre
*/
Std_ReturnType Crypto_KeyElementGet
(
    uint32  cryptoKeyId,
    uint32  keyElementId,
    uint8*  resultPtr,
    uint32* resultLengthPtr
);

/**
* @brief            Copies a key element to another key element in the same Crypto driver.
* @details          Copies a key element to another key element in the same Crypto driver.
*                   Note: If the actual key element is directly mapped to flash memory, there could be a bigger delay when calling this function (synchronous operation)
*
* @param[in]        cryptoKeyId          Holds the identifier of the key whose key element shall be the source element
* @param[in]        keyElementId         Holds the identifier of the key element which shall be the source for the copy operation
* @param[in]        targetCryptoKeyId    Holds the identifier of the key whose key element shall be the destination element
* @param[in]        targetKeyElementId   Holds the identifier of the key element which shall be the destination for the copy operation
*
* @return           Result of the operation
* @retval           E_OK                        Request successful
* @retval           E_NOT_OK                    Request failed
* @retval           CRYPTO_E_BUSY               Request failed, Crypto Driver Object is busy
* @retval           CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the requested key element is not available
* @retval           CRYPTO_E_KEY_READ_FAIL      Request failed, not allowed to extract key element
* @retval           CRYPTO_E_KEY_WRITE_FAIL     Request failed, not allowed to write key element
* @retval           CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, key element sizes are not compatible
* @retval           CRYPTO_E_KEY_EMPTY          Request failed, source key element is uninitialized
*
*
* @pre
*/
Std_ReturnType Crypto_KeyElementCopy
(
    uint32 cryptoKeyId,
    uint32 keyElementId,
    uint32 targetCryptoKeyId,
    uint32 targetKeyElementId
);

/**
* @brief            Copies a key element to another key element in the same Crypto driver.
* @details          Copies a key element to another key element in the same crypto driver.
*                   The keyElementSourceOffset and keyElementCopyLength allows to copy just a part of
*                   the source key element into the destination. The offset of the target key is also specified with this function.
*
* @param[in]        cryptoKeyId              Holds the identifier of the key whose key element shall be the source element
* @param[in]        keyElementId             Holds the identifier of the key element which shall be the source for the copy operation
* @param[in]        keyElementSourceOffset   This is the offset of the of the source key element indicating the start index of the copy operation.
* @param[in]        keyElementTargetOffset   This is the offset of the of the target key element indicating the start index of the copy operation.
* @param[in]        keyElementCopyLength     Specifies the number of bytes that shall be copied
* @param[in]        targetCryptoKeyId        Holds the identifier of the key whose key element shall be the destination element.
* @param[in]        targetKeyElementId       Holds the identifier of the key element which shall be the destination for the copy operation.

*
* @return           Result of the operation
* @retval           E_OK                        Request successful
* @retval           E_NOT_OK                    Request failed
* @retval           CRYPTO_E_BUSY               Request failed, Crypto Driver Object is busy
* @retval           CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the requested key element is not available
* @retval           CRYPTO_E_KEY_READ_FAIL      Request failed, not allowed to extract key element
* @retval           CRYPTO_E_KEY_WRITE_FAIL     Request failed, not allowed to write key element
* @retval           CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, key element sizes are not compatible
* @retval           CRYPTO_E_KEY_EMPTY          Request failed, source key element is uninitialized
*
*
* @pre
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
);

/**
* @brief            Copies a key with all its elements to another key in the same crypto driver.
* @details          Copies a key with all its elements to another key in the same crypto driver.
*                   Note: If the actual key element is directly mapped to flash memory, there could be a bigger delay when calling this function (synchronous operation)
*
* @param[in]        cryptoKeyId          Holds the identifier of the key whose key element shall be the source element
* @param[in]        targetCryptoKeyId    Holds the identifier of the key whose key element shall be the destination element
*
* @return           Result of the operation
* @retval           E_OK                        Request successful
* @retval           E_NOT_OK                    Request failed
* @retval           CRYPTO_E_BUSY               Request failed, Crypto Driver Object is busy
* @retval           CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the requested key element is not available
* @retval           CRYPTO_E_KEY_READ_FAIL      Request failed, not allowed to extract key element
* @retval           CRYPTO_E_KEY_WRITE_FAIL     Request failed, not allowed to write key element
* @retval           CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, key element sizes are not compatible
* @retval           CRYPTO_E_KEY_EMPTY          Request failed, source key element is uninitialized
*
*
* @pre
*
*/
Std_ReturnType Crypto_KeyCopy
(
    uint32 cryptoKeyId,
    uint32 targetCryptoKeyId
);

/**
* @brief            Used to retrieve information which key elements are available in a given key.
* @details          Used to retrieve information which key elements are available in a given key.
*
* @param[in]        cryptoKeyId               Holds the identifier of the key whose available element ids shall be exported
* @param[out]       keyElementIdsPtr          Contains the pointer to the array where the ids of the key elements shall be stored
* @param[inout]     keyElementIdsLengthPtr    Holds a pointer to the memory location in which the number of key elements in the given key is stored.
*                                             On calling this function, this parameter shall contain the size of the buffer provided by keyElementIdsPtr.
*                                             When the request has finished, the actual number of key elements shall be stored.
*
* @return           Result of the operation
* @retval           E_OK                    Request successful
* @retval           E_NOT_OK                Request failed
* @retval           CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy
* @retval           CRYPTO_E_SMALL_BUFFER   The provided buffer is too small to store the result
*
*
* @pre
*
*/
Std_ReturnType Crypto_KeyElementIdsGet
(
    uint32  cryptoKeyId,
    uint32* keyElementIdsPtr,
    uint32* keyElementIdsLengthPtr
);

/**
* @brief            This function generates the internal seed state using the provided entropy source. Furthermore, this function can be used to update the seed state with new entropy.
* @details          This function generates the internal seed state using the provided entropy source. Furthermore, this function can be used to update the seed state with new entropy.
*
* @param[in]        cryptoKeyId               Holds the identifier of the key for which a new seed shall be generated
* @param[in]        seedPtr                   Holds a pointer to the memory location which contains the data to feed the entropy
* @param[in]        seedLength                Contains the length of the entropy in bytes
*
* @return           Result of the operation
* @retval           E_OK        Request successful
* @retval           E_NOT_OK    Request failed
*
*
* @pre
*
*/
Std_ReturnType Crypto_RandomSeed
(
    uint32       cryptoKeyId,
    const uint8* seedPtr,
    uint32       seedLength
);

/**
* @brief            Generates new key material and stores it in the key identified by cryptoKeyId.
* @details          Generates new key material and stores it in the key identified by cryptoKeyId.
*
* @param[in]        cryptoKeyId     Holds the identifier of the key which is to be updated with the generated value
*
* @return           Result of the operation
* @retval           E_OK                  Request successful
* @retval           E_NOT_OK              Request failed
* @retval           CRYPTO_E_BUSY         Request failed, Crypto Driver Object is busy
* @retval           CRYPTO_E_KEY_EMPTY    Request failed, source key element is uninitialized
*
*
* @pre
*
*/
Std_ReturnType Crypto_KeyGenerate
(
    uint32 cryptoKeyId
);

/**
* @brief            Derives a new key by using the key elements in the given key identified by the cryptoKeyId.
* @details          Derives a new key by using the key elements in the given key identified by the cryptoKeyId. The given key contains the key elements for the password, salt.
*                   The derived key is stored in the key element with the id 1 of the key identified by targetCryptoKeyId. The number of iterations is given in the key element CRYPTO_KE_KEYDERIVATION_ITERATIONS.
*
* @param[in]        cryptoKeyId          Holds the identifier of the key which is used for key derivation
* @param[in]        targetCryptoKeyId    Holds the identifier of the key which is used to store the derived key
*
* @return           Result of the operation
* @retval           E_OK                  Request successful
* @retval           E_NOT_OK              Request failed
* @retval           CRYPTO_E_BUSY         Request failed, Crypto Driver Object is busy
* @retval           CRYPTO_E_KEY_EMPTY    Request failed, source key element is uninitialized
*
*
* @pre
*
*/
Std_ReturnType Crypto_KeyDerive
(
    uint32 cryptoKeyId,
    uint32 targetCryptoKeyId
);

/**
* @brief            Calculates the public value for the key exchange and stores the public key in the memory location pointed by the public value pointer.
* @details          Calculates the public value for the key exchange and stores the public key in the memory location pointed by the public value pointer.
*
* @param[in]        cryptoKeyId               Holds the identifier of the key which shall be used for the key exchange protocol
* @param[out]       publicValuePtr            Contains the pointer to the data where the public value shall be stored
* @param[inout]     publicValueLengthPtr      Holds a pointer to the memory location in which the public value length information is stored.
*                                             On calling this function, this parameter shall contain the size of the buffer provided by publicValuePtr.
*                                             When the request has finished, the actual length of the returned value shall be stored.
*
* @return           Result of the operation
* @retval           E_OK                    Request successful
* @retval           E_NOT_OK                Request failed
* @retval           CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy
* @retval           CRYPTO_E_SMALL_BUFFER   The provided buffer is too small to store the result
* @retval           CRYPTO_E_KEY_EMPTY      Request failed, source key element is uninitialized
*
*
* @pre
*
*/
Std_ReturnType Crypto_KeyExchangeCalcPubVal
(
    uint32  cryptoKeyId,
    uint8*  publicValuePtr,
    uint32* publicValueLengthPtr
);

/**
* @brief            Calculates the shared secret key.
* @details          Calculates the shared secret key for the key exchange with the key material of the key identified by the
*                   cryptoKeyId and the partner public key. The shared secret key is stored as a key element in the same key.
*
* @param[in]        cryptoKeyId                   Holds the identifier of the key which shall be used for the key exchange protocol
* @param[in]        partnerPublicValuePtr         Holds the pointer to the memory location which contains the partner's public value
* @param[in]        partnerPublicValueLength      Contains the length of the partner's public value in bytes.
*                                                 On calling this function, this parameter shall contain the size of the buffer provided by publicValuePtr.
*                                                 When the request has finished, the actual length of the returned value shall be stored.
*
* @return           Result of the operation
* @retval           E_OK                    Request successful
* @retval           E_NOT_OK                Request failed
* @retval           CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy
* @retval           CRYPTO_E_SMALL_BUFFER   The provided buffer is too small to store the result
* @retval           CRYPTO_E_KEY_EMPTY      Request failed, source key element is uninitialized
*
*
* @pre
*
*/
Std_ReturnType Crypto_KeyExchangeCalcSecret
(
    uint32       cryptoKeyId,
    const uint8* partnerPublicValuePtr,
    uint32       partnerPublicValueLength
);

/**
* @brief            Parses the certificate data stored in the key element CRYPTO_KE_CERT_DATA and fills the key elements CRYPTO_KE_CERT_SIGNEDDATA, CRYPTO_KE_CERT_PARSEDPUBLICKEY and CRYPTO_KE_CERT_SIGNATURE.
* @details          Parses the certificate data stored in the key element CRYPTO_KE_CERT_DATA and fills the key elements CRYPTO_KE_CERT_SIGNEDDATA, CRYPTO_KE_CERT_PARSEDPUBLICKEY and CRYPTO_KE_CERT_SIGNATURE.
*
* @param[in]        cryptoKeyId     Holds the identifier of the key which shall be parsed
*
* @return           Result of the operation
* @retval           E_OK            Request successful
* @retval           E_NOT_OK        Request failed
* @retval           CRYPTO_E_BUSY   Request failed, Crypto Driver Object is busy
*
*
* @pre
*
*/
Std_ReturnType Crypto_CertificateParse
(
    uint32 cryptoKeyId
);

/**
* @brief            Verifies the certificate stored in the key referenced by cryptoValidateKeyId with the certificate stored in the key referenced by cryptoKeyId.
* @details          Verifies the certificate stored in the key referenced by cryptoValidateKeyId with the certificate stored in the key referenced by cryptoKeyId.
*
* @param[in]        cryptoKeyId         Holds the identifier of the key which shall be used to validate the certificate
* @param[in]        verifyCryptoKeyId   Holds the identifier of the key contain
* @param[out]       verifyPtr           Holds a pointer to the memory location which will contain the result of the certificate verification
*
* @return           Result of the operation
* @retval           E_OK            Request successful
* @retval           E_NOT_OK        Request failed
* @retval           CRYPTO_E_BUSY   Request failed, Crypto Driver Object is busy
*
*
* @pre
*
*/
Std_ReturnType Crypto_CertificateVerify
(
    uint32                   cryptoKeyId,
    uint32                   verifyCryptoKeyId,
    Crypto_VerifyResultType* verifyPtr
);


#define CRYPTO_STOP_SEC_CODE
#include "Crypto_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CRYPTO_H */
