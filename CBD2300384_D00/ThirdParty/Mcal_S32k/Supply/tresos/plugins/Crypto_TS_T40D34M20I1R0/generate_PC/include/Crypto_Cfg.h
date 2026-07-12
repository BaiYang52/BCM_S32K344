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

#ifndef CRYPTO_CFG_H
#define CRYPTO_CFG_H

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
#include "Crypto_Types.h"
#include "hse_interface.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CRYPTO_VENDOR_ID_CFG                       43
#define CRYPTO_MODULE_ID_CFG                       114
#define CRYPTO_AR_RELEASE_MAJOR_VERSION_CFG        4
#define CRYPTO_AR_RELEASE_MINOR_VERSION_CFG        4
#define CRYPTO_AR_RELEASE_REVISION_VERSION_CFG     0
#define CRYPTO_SW_MAJOR_VERSION_CFG                2
#define CRYPTO_SW_MINOR_VERSION_CFG                0
#define CRYPTO_SW_PATCH_VERSION_CFG                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
[!NOCODE!][!//
[!INCLUDE "Crypto_VersionCheck_Inc.m"!][!//
[!ENDNOCODE!][!//
[!AUTOSPACING!][!//
/* Check if Crypto Cfg header file and Crypto Types header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_CFG != CRYPTO_VENDOR_ID_TYPES)
    #error "Crypto_Cfg.h and Crypto_Types.h have different vendor ids"
#endif

/* Check if Crypto Cfg header file and Crypto Types header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_CFG    != CRYPTO_AR_RELEASE_MAJOR_VERSION_TYPES) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_CFG    != CRYPTO_AR_RELEASE_MINOR_VERSION_TYPES) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_CFG != CRYPTO_AR_RELEASE_REVISION_VERSION_TYPES) \
    )
    #error "AutoSar Version Numbers of Crypto_Cfg.h and Crypto_Types.h are different"
#endif

/* Check if Crypto Cfg header file and Crypto Types header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_CFG != CRYPTO_SW_MAJOR_VERSION_TYPES) || \
     (CRYPTO_SW_MINOR_VERSION_CFG != CRYPTO_SW_MINOR_VERSION_TYPES) || \
     (CRYPTO_SW_PATCH_VERSION_CFG != CRYPTO_SW_PATCH_VERSION_TYPES)    \
    )
    #error "Software Version Numbers of Crypto_Cfg.h and Crypto_Types.h are different"
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
[!INDENT "0"!][!//
/* Pre-processor switch to enable/disable development error detection for Crypto API */
#define CRYPTO_DEV_ERROR_DETECT                         ([!IF "CryptoGeneral/CryptoDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Pre-processor switch to enable/disable the API to read out the modules version information */
#define CRYPTO_VERSION_INFO_API                         ([!IF "CryptoGeneral/CryptoVersionInfoApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Crypto instance ID value */
#define CRYPTO_INSTANCE_ID                              ((uint8)[!"node:value(CryptoGeneral/CryptoInstanceId)"!])

[!IF "node:exists(CryptoGeneral/CryptoMainFunctionPeriod)"!][!//
    /* Crypto main function period */
    #define CRYPTO_MAIN_FUNCTION_PERIOD                     ([!"node:value(CryptoGeneral/CryptoMainFunctionPeriod)"!])

[!ENDIF!][!//
/* Set asynchronous job process method */
#define CRYPTO_USE_INTERRUPTS_FOR_ASYNC_JOBS            ([!IF "node:value(CryptoGeneral/CryptoAsyncJobProcessMethod) = 'INTERRUPT'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Crypto Timeout value */
#define CRYPTO_TIMEOUT_DURATION_U32                     ((uint32)[!"node:value("CryptoGeneral/CryptoTimeoutDuration")"!]U)

/* Pre-processor switch to enable/disable the support in Crypto_ProcessJob() for handling key management services */
[!IF "node:exists(CryptoKeys)"!][!//
    #define CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT               ([!IF "CryptoGeneral/CryptoJobKeyManagement"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!ELSE!][!//
    #define CRYPTO_JOB_KEY_MANAGEMENT_SUPPORT               (STD_OFF)
[!ENDIF!][!//

/* Defines for the available MU instances */
[!NOCODE!][!//
    [!FOR "LoopCounter" = "1" TO "num:i(count(ecu:list('Crypto.MuInstances.List')))"!][!//
        [!CODE!]#define CRYPTO_[!"ecu:list('Crypto.MuInstances.List')[num:i($LoopCounter)]"!]_U8                                  ((uint8)[!"num:i($LoopCounter - 1)"!]U)[!ENDCODE!][!CR!][!//
    [!ENDFOR!][!//
[!ENDNOCODE!][!//

[!IF "num:i(count(CryptoGeneral/CryptoEcucPartitionRef/*)) > 0"!][!//
/* Pre-processor switch to enable multicore support in Crypto driver */
#define CRYPTO_ENABLE_MULTICORE_SUPPORT                 (STD_ON)

/* Max number of partitions configured from the application */
#define CRYPTO_MAX_NUMBER_PARTITIONS_U8                 ((uint8)[!"num:i(count(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition/*))"!]U)

[!NOCODE!][!//
[!VAR "MaxNumPartitions" = "num:i(count(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition/*))"!][!//
[!VAR "NumPartitionsMatched" = "num:i(0)"!][!//
[!FOR "x" = "0" TO "num:i($MaxNumPartitions - 1)"!][!//
    [!VAR "PartitionMatched" = "'false'"!][!//
    [!LOOP "CryptoDriverObjects/CryptoDriverObject/*"!][!//
        [!LOOP "CryptoDriverObjectEcucPartitionRef/*"!][!//
            [!VAR "CurCdoPartition" = "node:value(.)"!][!//
            [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
                [!IF "node:refvalid(./OsAppEcucPartitionRef)"!][!//
                    [!IF "$CurCdoPartition = node:value(./OsAppEcucPartitionRef)"!][!//
                        [!IF "node:refvalid(./OsApplicationCoreRef)"!][!//
                            [!IF "$x = node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!][!//
                                [!VAR "PartitionMatched" = "'true'"!][!//
                                [!VAR "NumPartitionsMatched" = "$NumPartitionsMatched + 1"!][!//
                            [!ENDIF!][!//
                        [!ENDIF!][!//
                    [!ENDIF!][!//
                [!ENDIF!][!//
            [!ENDLOOP!][!//
        [!ENDLOOP!][!//
        [!IF "$PartitionMatched = 'true'"!][!//
            [!BREAK!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDFOR!][!//
[!IF "$NumPartitionsMatched = num:i(0)"!][!//
    [!ERROR!][!//
        None of the referenced ECUC partitions in the CryptoDriverObjectEcucPartitionRef list of the CryptoDriverObject list is associated with an OsApplication or the OsApplication does not have a valid OsApplicationCoreRef!
    [!ENDERROR!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
/* Number of configured partitions the driver can run in */
#define CRYPTO_NUMBER_PARTITIONS_ALLOWED_TO_RUN_IN_U8   ((uint8)[!"num:i($NumPartitionsMatched)"!]U)
[!ELSE!][!//
/* Pre-processor switch to disable multicore support in Crypto driver */
#define CRYPTO_ENABLE_MULTICORE_SUPPORT                 (STD_OFF)

/* Max number of partitions configured from the application */
#define CRYPTO_MAX_NUMBER_PARTITIONS_U8                 ((uint8)1U)

/* Number of configured partitions the driver can run in */
#define CRYPTO_NUMBER_PARTITIONS_ALLOWED_TO_RUN_IN_U8   ((uint8)1U)
[!ENDIF!][!//

/* Number of configured Crypto driver objects */
#define CRYPTO_NUMBER_OF_DRIVER_OBJECTS_U32             ((uint32)[!"num:i(count(CryptoDriverObjects/CryptoDriverObject/*))"!]U)

[!IF "node:exists(CryptoKeys)"!][!//
    /* Compile time switch stating that there is at least one configured key */
    #define CRYPTO_KEYS_EXIST                               (STD_ON)

    /* Number of Crypto keys */
    #define CRYPTO_NUMBER_OF_KEYS_U32                       ((uint32)[!"num:i(count(CryptoKeys/CryptoKey/*))"!]U)

    /* Number of Crypto key elements */
    #define CRYPTO_NUMBER_OF_KEY_ELEMENTS_U32               ((uint32)[!"num:i(count(CryptoKeyElements/CryptoKeyElement/*))"!]U)

    /* Support for enabling alternate mapping of crypto job keys.
    *       STD_ON:  the Crypto driver will read the key related information of Csm jobs from an alternate location which is the cryptoKeyId member of the Crypto_JobType structure.
    *       STD_OFF: the Crypto driver will read the key related information of Csm jobs from the cryIfKeyId member of Crypto_JobPrimitiveInfoType substructure of the Crypto_JobType structure. */
    #define CRYPTO_ENABLE_ALTERNATE_MAPPING_FOR_JOB_KEY     ([!IF "CryptoGeneral/CryptoAlternateJobKeyMapping"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

    /* Identifiers for the blobs of information that need to be stored in NVRAM used by the Crypto driver */
    #define CRYPTO_NVRAM_BLOB_0_ID                          (0U)
    #define CRYPTO_NVRAM_BLOB_1_ID                          (1U)

    /* Sizes for the blobs of information used by the Crypto driver */
    #define CRYPTO_SIZEOF_NVRAM_BLOB_0                      ([!IF "num:i(count(CryptoKeys/CryptoKey/*) mod 8) = 0"!][!"num:i(count(CryptoKeys/CryptoKey/*) div 8)"!][!ELSE!][!"num:i((count(CryptoKeys/CryptoKey/*) div 8) + 1)"!][!ENDIF!]U)
[!NOCODE!][!//
    [!VAR "SizeOfKeyElemPersistValuesBlob"="0"!][!//
    [!VAR "KeyElemPersistBlobHasAllBytesZero"="'true'"!][!//
    [!LOOP "CryptoKeyElements/CryptoKeyElement/*"!][!//
        [!IF "node:value(./UseHseKey)='false' and node:value(./CryptoKeyElementPersist) = 'true' "!][!//
            /* If the key element does not use a HSE key, add the max size of the key element buffer + 4 bytes storing the 32bit value of the actual size */
            [!VAR "SizeOfKeyElemPersistValuesBlob" = "$SizeOfKeyElemPersistValuesBlob + node:value(./CryptoKeyElementSize) + num:i(4)"!][!//
            [!IF "num:i(string-length(node:value(./CryptoKeyElementInitValue)))!=0"!][!//
                [!VAR "KeyElemPersistBlobHasAllBytesZero"="'false'"!][!//
            [!ENDIF!][!//
        [!ELSEIF "node:value(./UseHseKey)='true' and node:name(concat(noderef:ref(HseKeyCatalogGroupRef), '/../.')) = 'NvmKeyCatalog' "!][!//
            /* If the key element uses a HSE key, add the 4 bytes storing the 32bit value of the actual size */
            [!VAR "SizeOfKeyElemPersistValuesBlob" = "$SizeOfKeyElemPersistValuesBlob + num:i(4)"!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDNOCODE!][!//
    #define CRYPTO_SIZEOF_NVRAM_BLOB_1                      ([!"num:i($SizeOfKeyElemPersistValuesBlob)"!]U)

[!NOCODE!][!//
    /* Create the initializer for the array with the information that needs to be stored in Nvram for all persistent Key Elements (buffers + actual sizes) */
    [!IF "$SizeOfKeyElemPersistValuesBlob > 0"!][!//
        [!IF "$KeyElemPersistBlobHasAllBytesZero = 'false'"!][!//
            [!CODE!][!//
            [!INDENT "0"!]
                /* Initializer of NVRAM Blob1 */
                #define CRYPTO_NVRAM_BLOB_1_INITIALIZER                 {        \
            [!ENDINDENT!][!//
            [!ENDCODE!][!//
            [!VAR "CurIdx"="0"!][!//
            [!LOOP "CryptoKeyElements/CryptoKeyElement/*"!][!//
                [!IF "node:value(./UseHseKey)='false' and node:value(./CryptoKeyElementPersist) = 'true' "!][!//
                    /* If the key element does not use a HSE key, add the 4 bytes storing the 32bit value of the actual size + the max size buffer of the key element */
                    [!INDENT "58"!]
                    [!CODE!][!//
                        /* Offset [!"num:i($CurIdx)"!] - Length of [!"node:name(.)"!] */ \
                        [!"num:inttohex(bit:and((bit:shr(num:i(string-length(node:value(./CryptoKeyElementInitValue)) div 2),0)),255),2)"!]U, \
                        [!"num:inttohex(bit:and((bit:shr(num:i(string-length(node:value(./CryptoKeyElementInitValue)) div 2),8)),255),2)"!]U, \
                        [!"num:inttohex(bit:and((bit:shr(num:i(string-length(node:value(./CryptoKeyElementInitValue)) div 2),16)),255),2)"!]U, \
                        [!"num:inttohex(bit:and((bit:shr(num:i(string-length(node:value(./CryptoKeyElementInitValue)) div 2),24)),255),2)"!]U, \
                    [!ENDCODE!][!//
                    [!VAR "CurIdx" = "$CurIdx + 4"!][!//
                    [!CODE!][!//
                     /* Offset [!"num:i($CurIdx)"!] - Buffer of [!"node:name(.)"!] */ \
                    [!ENDCODE!][!//
                    [!VAR "Index" = "num:i(-1)"!][!//
                    [!FOR "Index" = "num:i(1)" TO "num:i(string-length(node:value(./CryptoKeyElementInitValue)))" STEP "num:i(2)"!][!//
                        [!VAR "CurIdx" = "$CurIdx + 1"!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./CryptoKeyElementInitValue), $Index, 2)"!]U[!IF "num:i($CurIdx) < num:i($SizeOfKeyElemPersistValuesBlob)"!], \[!ELSE!][!WS!] \[!ENDIF!][!CR!]
                        [!ENDCODE!][!//
                    [!ENDFOR!][!//
                    [!VAR "CountEl" = "$Index"!][!//
                    [!FOR "Index" = "num:i($CountEl+2)" TO "num:i(2 * node:value(./CryptoKeyElementSize))" STEP "num:i(2)"!][!//
                        [!VAR "CurIdx" = "$CurIdx + 1"!][!//
                        [!CODE!][!//
                            0x00U[!IF "num:i($CurIdx) < num:i($SizeOfKeyElemPersistValuesBlob)"!], \[!ELSE!][!WS!] \[!ENDIF!][!CR!][!//
                        [!ENDCODE!][!//
                    [!ENDFOR!][!//
                    [!ENDINDENT!][!//
                [!ELSEIF "node:value(./UseHseKey)='true' and node:name(concat(noderef:ref(HseKeyCatalogGroupRef), '/../.')) = 'NvmKeyCatalog' "!][!//
                    [!INDENT "58"!]
                    /* If the key element uses a HSE key and it is a Nvm key, add the 4 bytes storing the 32bit value of the actual size */
                    [!"keyElemActualSize = num:i(string-length(node:value(./CryptoKeyElementInitValue)))"!][!//
                    [!CODE!][!//
                        /* Offset [!"num:i($CurIdx)"!] - Length of [!"node:name(.)"!] */ \
                        [!VAR "CurIdx" = "$CurIdx + 1"!]0x00U, \
                        [!VAR "CurIdx" = "$CurIdx + 1"!]0x00U, \
                        [!VAR "CurIdx" = "$CurIdx + 1"!]0x00U, \
                        [!VAR "CurIdx" = "$CurIdx + 1"!]0x00U[!IF "num:i($CurIdx) < num:i($SizeOfKeyElemPersistValuesBlob)"!], \[!ELSE!] \[!ENDIF!][!CR!]
                    [!ENDCODE!][!//
                    [!ENDINDENT!][!//
                [!ENDIF!][!//
            [!ENDLOOP!][!//
            [!INDENT "56"!]
            [!CODE!][!//
                }
            [!ENDCODE!][!//
            [!ENDINDENT!][!//
        [!ENDIF!][!//
    [!ENDIF!][!//
[!ENDNOCODE!][!//

    /* Compile time switch stating that support for Nvram read/write operations is enabled or disabled */
    #define CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT          ([!IF "node:exists(CryptoGeneral/CryptoUpdateNvramBlobHandler)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!ELSE!][!//
    /* Compile time switch stating that there is no configured key */
    #define CRYPTO_KEYS_EXIST                               (STD_OFF)
[!ENDIF!][!//

/* Pre-processor switch to enable/disable support in Crypto driver for job redirection feature */
[!IF "node:exists(CryptoKeys)"!][!//
    #define CRYPTO_ENABLE_REDIRECTION_SUPPORT               ([!IF "CryptoGeneral/CryptoEnableRedirection"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!ELSE!][!//
    #define CRYPTO_ENABLE_REDIRECTION_SUPPORT               (STD_OFF)
[!ENDIF!][!//

/* Pre-processor switch to enable/disable support in Crypto driver for feeding Hse Firmware with descriptors using Crypto_KeyElementGet() API */
[!IF "node:exists(CryptoKeys)"!][!//
    #define CRYPTO_ENABLE_FEED_HSE_DESC_SUPPORT             ([!IF "CryptoGeneral/CryptoEnableFeedHseDesc"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!ELSE!][!//
    #define CRYPTO_ENABLE_FEED_HSE_DESC_SUPPORT             (STD_OFF)
[!ENDIF!][!//

/* Support for User mode.
*       STD_ON:  the Crypto driver can be executed from both supervisor and user mode
*       STD_OFF: the Crypto driver can be executed only from supervisor mode */
#define CRYPTO_ENABLE_USER_MODE_SUPPORT                 ([!IF "CryptoGeneral/CryptoEnableUserModeSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Support for TLS1.2 key derivation in Crypto_KeyDerive() API */
#define CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT              ([!IF "CryptoGeneral/CryptoEnableTLS12KeyDeriveSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

[!INDENT "0"!]#ifndef MCAL_ENABLE_USER_MODE_SUPPORT[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#ifdef CRYPTO_ENABLE_USER_MODE_SUPPORT[!CR!][!ENDINDENT!][!//
[!INDENT "8"!]#if (STD_ON == CRYPTO_ENABLE_USER_MODE_SUPPORT)[!CR!][!ENDINDENT!][!//
[!INDENT "12"!]#error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running Crypto driver in user mode the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined.[!CR!][!ENDINDENT!][!//
[!INDENT "8"!]#endif /* (STD_ON == CRYPTO_ENABLE_USER_MODE_SUPPORT) */[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#endif /* ifndef CRYPTO_ENABLE_USER_MODE_SUPPORT */[!CR!][!ENDINDENT!][!//
[!INDENT "0"!]#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT*/[!CR!][!ENDINDENT!][!//

/* Support for SHE */
#define CRYPTO_SPT_SHE                                  ([!"ecu:get(concat('Crypto.HseSptShe.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])

/* Support for Miyaguchi-Preneel compression function (SHE spec support) */
#define CRYPTO_SPT_MP                                   ([!"ecu:get(concat('Crypto.HseSptMP.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])

/* Support for AES */
#define CRYPTO_SPT_AES                                  ([!"ecu:get(concat('Crypto.HseSptAes.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])
[!INDENT "4"!]#define CRYPTO_SPT_AES_CBC                          ([!"ecu:get(concat('Crypto.HseSptAesCbc.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_AES_CFB                          ([!"ecu:get(concat('Crypto.HseSptAesCfb.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_AES_CTR                          ([!"ecu:get(concat('Crypto.HseSptAesCtr.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_AES_ECB                          ([!"ecu:get(concat('Crypto.HseSptAesEcb.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_AES_OFB                          ([!"ecu:get(concat('Crypto.HseSptAesOfb.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//

/* Support for AEAD */
#define CRYPTO_SPT_AEAD                                 ([!"ecu:get(concat('Crypto.HseSptAead.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])

/* Support for MAC */
#define CRYPTO_SPT_MAC                                  ([!"ecu:get(concat('Crypto.HseSptMac.',      string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])
[!INDENT "4"!]#define CRYPTO_SPT_CMAC                             ([!"ecu:get(concat('Crypto.HseSptCmac.',     string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_FAST_CMAC                        ([!"ecu:get(concat('Crypto.HseSptFastCmac.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_GMAC                             ([!"ecu:get(concat('Crypto.HseSptGmac.',     string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_HMAC                             ([!"ecu:get(concat('Crypto.HseSptHmac.',     string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_XCBCMAC                          ([!"ecu:get(concat('Crypto.HseSptXCbcmac.',  string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//

/* Support for TDES_(128, 192)_(ECB, CBC, CFB, OFB) as defined  in NIST SP 800-67 rev1. */
#define CRYPTO_SPT_TDES                                 ([!"ecu:get(concat('Crypto.HseSptTdes.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])

/* Support for HASH primitives */
#define CRYPTO_SPT_HASH                                 ([!"ecu:get(concat('Crypto.HseSptHash.',         string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])
[!INDENT "4"!]#define CRYPTO_SPT_MD5                              ([!"ecu:get(concat('Crypto.HseSptMd5.',          string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_SHA1                             ([!"ecu:get(concat('Crypto.HseSptSha1.',         string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_SHA2_224                         ([!"ecu:get(concat('Crypto.HseSptSha2_224.',     string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_SHA2_256                         ([!"ecu:get(concat('Crypto.HseSptSha2_256.',     string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_SHA2_384                         ([!"ecu:get(concat('Crypto.HseSptSha2_384.',     string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_SHA2_512                         ([!"ecu:get(concat('Crypto.HseSptSha2_512.',     string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_SHA2_512_224                     ([!"ecu:get(concat('Crypto.HseSptSha2_512_224.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_SHA2_512_256                     ([!"ecu:get(concat('Crypto.HseSptSha2_512_256.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_SHA3                             ([!"ecu:get(concat('Crypto.HseSptSha3.',         string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//

/* Support for SIPHASH primitive */
#define CRYPTO_SPT_SIPHASH                              ([!"ecu:get(concat('Crypto.HseSptSiphash.',         string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])

/* Support for RSA primitives */
#define CRYPTO_SPT_RSA                                  ([!"ecu:get(concat('Crypto.HseSptRsa.',                    string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])
[!INDENT "4"!]#define CRYPTO_SPT_RSAES_NO_PADDING                 ([!"ecu:get(concat('Crypto.HseSptRsaesNoPadding.',         string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_RSAES_OAEP                       ([!"ecu:get(concat('Crypto.HseSptRsaesOaep.',              string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_RSAES_PCKS1_V15                  ([!"ecu:get(concat('Crypto.HseSptRsaesPkcs1V15.',          string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_RSAASSA_PSS                      ([!"ecu:get(concat('Crypto.HseSptRsassaPss.',              string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_RSASSA_PCKS1_V15                 ([!"ecu:get(concat('Crypto.HseSptRsassaPkcs1V15.',         string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//

/* Support for ECC primitives */
#define CRYPTO_SPT_ECC                                  ([!"ecu:get(concat('Crypto.HseSptEcc.',                    string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])
[!INDENT "4"!]#define CRYPTO_SPT_EDDSA                            ([!"ecu:get(concat('Crypto.HseSptEddsa.',                  string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_ECDSA                            ([!"ecu:get(concat('Crypto.HseSptEcdsa.',                  string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_SECP256R1                        ([!"ecu:get(concat('Crypto.HseSptEcSecSecp256R1.',         string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_SECP384R1                        ([!"ecu:get(concat('Crypto.HseSptEcSecSecp384R1.',         string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_SECP521R1                        ([!"ecu:get(concat('Crypto.HseSptEcSecSecp521R1.',         string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_BRAINPOOLP256R1                  ([!"ecu:get(concat('Crypto.HseSptEcBrainpoolP256R1.',      string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_BRAINPOOLP320R1                  ([!"ecu:get(concat('Crypto.HseSptEcBrainpoolP320R1.',      string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_BRAINPOOLP384R1                  ([!"ecu:get(concat('Crypto.HseSptEcBrainpoolP384R1.',      string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_BRAINPOOLP512R1                  ([!"ecu:get(concat('Crypto.HseSptEcBrainpoolP512R1.',      string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_EC25519_ED25519                  ([!"ecu:get(concat('Crypto.HseSptEc25519Ed25519.',         string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_EC25519_CURVE25519               ([!"ecu:get(concat('Crypto.HseSptEc25519Curve25519.',      string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_EC448_ED448                      ([!"ecu:get(concat('Crypto.HseSptEc448Ed448.',             string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_EC448_CURVE448                   ([!"ecu:get(concat('Crypto.HseSptEc448Curve448.',          string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//

/* Support for various key operations */
#define CRYPTO_SPT_KEY_IMPORT                           ([!"ecu:get(concat('Crypto.HseSptKeyImport.',             string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])
[!INDENT "4"!]#define CRYPTO_SPT_SHE_KEY_IMPORT                   ([!"ecu:get(concat('Crypto.HseSptSheKeyImport.',           string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_SYM_KEY_IMPORT                   ([!"ecu:get(concat('Crypto.HseSptSymKeyImport.',           string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_RSA_KEY_IMPORT                   ([!"ecu:get(concat('Crypto.HseSptRsaKeyImport.',           string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_ECC_KEY_IMPORT                   ([!"ecu:get(concat('Crypto.HseSptEccKeyImport.',           string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_ENC_AUTH_KEY_IMPORT              ([!"ecu:get(concat('Crypto.HseSptEncAuthKeyImport.',       string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
#define CRYPTO_SPT_KEY_EXPORT                           ([!"ecu:get(concat('Crypto.HseSptKeyExport.',              string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])
[!INDENT "4"!]#define CRYPTO_SPT_SHE_KEY_EXPORT                   ([!"ecu:get(concat('Crypto.HseSptSheKeyExport.',           string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_SYM_KEY_EXPORT                   ([!"ecu:get(concat('Crypto.HseSptSymKeyExport.',           string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_RSA_KEY_EXPORT                   ([!"ecu:get(concat('Crypto.HseSptRsaKeyExport.',           string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_ECC_KEY_EXPORT                   ([!"ecu:get(concat('Crypto.HseSptEccKeyExport.',           string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_ENC_AUTH_KEY_EXPORT              ([!"ecu:get(concat('Crypto.HseSptEncAuthKeyExport.',       string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
#define CRYPTO_SPT_FORMATKEYCATALOGS                    ([!"ecu:get(concat('Crypto.HseSptFormatKeyCatalogs.',      string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])
#define CRYPTO_SPT_GETKEYINFO                           ([!"ecu:get(concat('Crypto.HseSptGetKeyInfo.',             string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])

/* Support for key derivation */
#define CRYPTO_SPT_KEY_DERIVE                           ([!"ecu:get(concat('Crypto.HseSptKeyDerive.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])
[!INDENT "4"!]#define CRYPTO_SPT_KDF_ANS_X963                     ([!"ecu:get(concat('Crypto.HseSptKDFAnsX963.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_PBKDF2                           ([!"ecu:get(concat('Crypto.HseSptPBKDF2.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_KDFTLS_12PRF                     ([!"ecu:get(concat('Crypto.HseSptKdfTls12Prf.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//

/* Support for Diffie-Hellman Compute Key */
#define CRYPTO_SPT_COMPUTE_DH                           ([!"ecu:get(concat('Crypto.HseSptComputeDH.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])

/* Support for key generation */
#define CRYPTO_SPT_KEY_GENERATE                         ([!"ecu:get(concat('Crypto.HseSptKeyGenerate.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])
[!INDENT "4"!]#define CRYPTO_SPT_SYM_RND_KEY_GEN                  ([!"ecu:get(concat('Crypto.HseSptSymRndKeyGen.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_RSA_KEY_PAIR_GEN                 ([!"ecu:get(concat('Crypto.HseSptRsaKeyPairGen.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_ECC_KEY_PAIR_GEN                 ([!"ecu:get(concat('Crypto.HseSptEccKeyPairGen.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//
[!INDENT "4"!]#define CRYPTO_SPT_CLASSIC_DH_KEY_PAIR_GEN          ([!"ecu:get(concat('Crypto.HseSptClassicDHKeyPairGen.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])[!CR!][!ENDINDENT!][!//

/* Support for Cipher modes flags for AES keys */
#define CRYPTO_AES_BLOCK_MODE_MASK                      ([!"ecu:get(concat('Crypto.HseAesBlockModeMask.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])
/* Support for HSE ECC key format */
#define CRYPTO_ECC_KEY_FORMAT                           ([!"ecu:get(concat('Crypto.HseEccKeyFormat.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD'))))"!])

/* Define stating if at least one key in the configuration has support for encrypted and/or authenticated import */
[!IF "ecu:get(concat('Crypto.HseSptEncAuthKeyImport.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON'"!][!//
    [!VAR "Import" = "'STD_OFF'"!][!//
    [!LOOP "CryptoKeyElements/CryptoKeyElement/*"!][!//
        [!IF "node:value(./EncryptedKeyImport)='true'"!][!//
            [!VAR "Import" = "'STD_ON'"!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
    [!INDENT "0"!][!//
        [!CODE!]#define CRYPTO_ENABLE_ENC_AUTH_KEY_IMPORT               ([!ENDCODE!][!"$Import"!][!CODE!])[!CR!][!CR!][!ENDCODE!][!//
    [!ENDINDENT!][!//
[!ELSE!][!//
    [!INDENT "0"!][!//
        [!CODE!]#define CRYPTO_ENABLE_ENC_AUTH_KEY_IMPORT               (STD_OFF)[!CR!][!CR!][!ENDCODE!][!//
    [!ENDINDENT!][!//
[!ENDIF!][!//
/* Define stating if at least one key in the configuration has support for encrypted and/or authenticated export */
[!IF "ecu:get(concat('Crypto.HseSptEncAuthKeyExport.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON'"!][!//
    [!VAR "Export" = "'STD_OFF'"!][!//
    [!LOOP "CryptoKeyElements/CryptoKeyElement/*"!][!//
        [!IF "node:value(./KeyExport)='true'"!][!//
            [!VAR "Export" = "'STD_ON'"!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
    [!INDENT "0"!][!//
        [!CODE!]#define CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT               ([!ENDCODE!][!"$Export"!][!CODE!])[!CR!][!CR!][!ENDCODE!][!//
    [!ENDINDENT!][!//
[!ELSE!][!//
    [!INDENT "0"!][!//
        [!CODE!]#define CRYPTO_ENABLE_ENC_AUTH_KEY_EXPORT               (STD_OFF)[!CR!][!CR!][!ENDCODE!][!//
    [!ENDINDENT!][!//
[!ENDIF!][!//
[!NOCODE!][!//
    /* The Tresos generator code below creates the symbolic name defines for CryptoDriverObjectId, CryptoKeyId and CryptoKeyElementId attributes from their
    corresponding lists, making sure that all the values are written on the same column of the generated Crypto_Cfg.h file */
    [!VAR "MaxLength" = "0"!][!//
    [!LOOP "CryptoDriverObjects/CryptoDriverObject/*"!][!//
        [!IF "string-length(concat('CryptoConf_CryptoDriverObject_', node:name(.))) > $MaxLength"!][!//
            [!VAR "MaxLength" = "string-length(concat('CryptoConf_CryptoDriverObject_', node:name(.)))"!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
    [!IF "node:exists(CryptoKeys)"!][!//
        [!LOOP "CryptoKeys/CryptoKey/*"!][!//
            [!IF "string-length(concat('CryptoConf_CryptoKey_', node:name(.))) > $MaxLength"!][!//
                [!VAR "MaxLength" = "string-length(concat('CryptoConf_CryptoKey_', node:name(.)))"!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
    [!ENDIF!][!//
    [!IF "node:exists(CryptoKeyElements)"!][!//
        [!LOOP "CryptoKeyElements/CryptoKeyElement/*"!][!//
            [!IF "string-length(concat('CryptoConf_CryptoKeyElement_', node:name(.))) > $MaxLength"!][!//
                [!VAR "MaxLength" = "string-length(concat('CryptoConf_CryptoKeyElement_', node:name(.)))"!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
    [!ENDIF!][!//

    [!CODE!]/* Symbolic names for the CryptoDriverObjectId attribute of all the Crypto Driver Objects */[!ENDCODE!][!CR!][!//
    [!LOOP "CryptoDriverObjects/CryptoDriverObject/*"!][!//
        [!CODE!]#define CryptoConf_CryptoDriverObject_[!"node:name(.)"!][!ENDCODE!][!//
        [!FOR "LoopCounter" = "1" TO "$MaxLength + 2 - string-length(concat('CryptoConf_CryptoDriverObject_', node:name(.)))"!][!//
            [!CODE!] [!ENDCODE!][!//
        [!ENDFOR!][!//
        [!CODE!]((uint32)[!"CryptoDriverObjectId"!]U)[!ENDCODE!][!CR!][!//
    [!ENDLOOP!][!//

    [!IF "node:exists(CryptoKeys)"!][!//
        [!CR!][!CODE!]/* Symbolic names for the CryptoKeyId attribute of all the Crypto keys */[!ENDCODE!][!CR!][!//
        [!LOOP "CryptoKeys/CryptoKey/*"!][!//
            [!CODE!]#define CryptoConf_CryptoKey_[!"node:name(.)"!][!ENDCODE!][!//
            [!FOR "LoopCounter" = "1" TO "$MaxLength + 2 - string-length(concat('CryptoConf_CryptoKey_', node:name(.)))"!][!//
                [!CODE!] [!ENDCODE!][!//
            [!ENDFOR!][!//
            [!CODE!]((uint32)[!"CryptoKeyId"!]U)[!ENDCODE!][!CR!][!//
        [!ENDLOOP!][!//
    [!ENDIF!][!//

    [!IF "node:exists(CryptoKeyElements)"!][!//
        [!CR!][!CODE!]/* Symbolic names for the CryptoKeyElementId attribute of all the Crypto key elements */[!ENDCODE!][!CR!][!//
        [!LOOP "CryptoKeyElements/CryptoKeyElement/*"!][!//
            [!CODE!]#define CryptoConf_CryptoKeyElement_[!"node:name(.)"!][!ENDCODE!][!//
            [!FOR "LoopCounter" = "1" TO "$MaxLength + 2 - string-length(concat('CryptoConf_CryptoKeyElement_', node:name(.)))"!][!//
                [!CODE!] [!ENDCODE!][!//
            [!ENDFOR!][!//
            [!CODE!]((uint32)[!"CryptoKeyElementId"!]U)[!ENDCODE!][!CR!][!//
        [!ENDLOOP!][!//
    [!ENDIF!][!//
[!ENDNOCODE!][!//
[!ENDINDENT!][!//

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
[!IF "node:exists(CryptoKeys)"!][!//
[!IF "ecu:get(concat('Crypto.HseSptEncAuthKeyImport.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON' or
      ecu:get(concat('Crypto.HseSptEncAuthKeyExport.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON'"!][!//
typedef struct
{
    const uint8  u8CryptoAuthMacAlgo;          /* Defines the MAC scheme: CMAC, HMAC, GMAC or XCBC_MAC */
    const uint8  u8CryptoAuthCipherAlgo;       /* Used only when MAC scheme is CMAC */
    const uint8  u8CryptoAuthHashAlgo;         /* Used only when MAC scheme is HMAC */
    const uint32 u32CryptoAuthIvLength;        /* Used only when MAC scheme is GMAC */
    const uint8* pCryptoAuthIv;                /* Used only when MAC scheme is GMAC */
}Crypto_AuthMacSchemeType;

typedef struct
{
    const uint8   u8CryptoAuthSignScheme;       /* Defines the signScheme:  ECDSA, EDDSA, RSASSA_PSS or RSASSA_PKCS1_V15 */
    const uint8   u8CryptoAuthContextLength;    /* Used only when signScheme is EDDSA */
    const uint8*  pCryptoAuthContext;           /* Used only when signScheme is EDDSA */
    const boolean bCryptoHashEddsa;             /* Used only when signScheme is EDDSA */
    const uint8   u8CryptoAuthSaltLength;       /* Used only when signScheme is RSASSA_PSS */
    const uint8   u8CryptoAuthSignHashAlgo;     /* Always used when signScheme is any of ECDSA, RSASSA_PSS, RSASSA_PKCS1_V15 */
}Crypto_AuthSignSchemeType;

[!ENDIF!][!//
[!IF "ecu:get(concat('Crypto.HseSptEncAuthKeyImport.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON'"!][!//
typedef struct
{
    const uint16    u16CryptoAuthKeyContLength;
    const HOST_ADDR u32CryptoAuthKeyCont;
    const uint16    u16CryptoAuthTagLength0;
    const uint8*    pCryptoAuthTag0;
    const uint16    u16CryptoAuthTagLength1;
    const uint8*    pCryptoAuthTag1;
}Crypto_AuthContainerType;

[!ENDIF!][!//
[!IF "ecu:get(concat('Crypto.HseSptEncAuthKeyImport.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON'"!][!//
typedef struct
{
    const uint8  u8CryptoCipherAlgo;
    const uint8  u8CryptoCipherBlockMode;
    const uint32 u32CryptoCipherIVLen;
    const uint8* pCryptoCipherIV;
    const uint8  u8CryptoAuthCipherMode;
    const uint32 u32CryptoAuthCipherTagLen;
    const uint8* pCryptoAuthCipherTag;
    const uint32 u32CryptoAuthCipherIVLen;
    const uint8* pCryptoAuthCipherIV;
    const uint32 u32CryptoAuthCipherAADLen;
    const uint8* pCryptoAuthCipherAAD;
    const uint8  u8CryptoCipherRsaHashAlgo;
    const uint32 u32CryptoCipherRsaLabelLen;
    const uint8* pCryptoCipherRsaLabel;
    const uint32 u32EncryptedHseKeyBitLen;
}Crypto_CipherSchemeType;

[!ENDIF!][!//
[!IF "ecu:get(concat('Crypto.HseSptEncAuthKeyExport.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON'"!][!//
typedef struct
{
    const uint8  u8CryptoCipherAlgo;
    const uint8  u8CryptoCipherBlockMode;
    const uint32 u32CryptoCipherIVLen;
    const uint8* pCryptoCipherIV;
    const uint8  u8CryptoAuthCipherMode;
    const uint32 u32CryptoAuthCipherIVLen;
    const uint8* pCryptoAuthCipherIV;
    const uint32 u32CryptoAuthCipherAADLen;
    const uint8* pCryptoAuthCipherAAD;
    const uint8  u8CryptoCipherRsaHashAlgo;
    const uint32 u32CryptoCipherRsaLabelLen;
    const uint8* pCryptoCipherRsaLabel;
}Crypto_ExportCipherSchemeType;

[!ENDIF!][!//
/* Structure storing information about a Crypto Key Element */
typedef struct
{
    /* Members storing Autosar required information */
    const   uint32                           u32CryptoKeyElementId;
    const   boolean                          bCryptoKeyElementAllowPartialAccess;
    const   Crypto_KeyFormatType             eCryptoKeyFormat;
    const   boolean                          bCryptoKeyElementPersist;
    const   Crypto_KeyElementReadAccessType  eCryptoKeyElementReadAccess;
    const   uint32                           u32CryptoKeyElementMaxSize;
    const   Crypto_KeyElementWriteAccessType eCryptoKeyElementWriteAccess;
    uint32* const                            pu32CryptoElementActualSize;
    uint8*  const                            pCryptoElementArray;
    /* Members storing HW specific required information (HSE related) */
[!IF "ecu:get(concat('Crypto.HseSptEncAuthKeyImport.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON'"!][!//
    const   uint32                           u32HseCipherKeyHandle;
    const   Crypto_CipherSchemeType*         pHseCipherScheme;
    const   uint32                           u32HseAuthKeyHandle;
    const   Crypto_AuthMacSchemeType*        pHseAuthMacScheme;
    const   Crypto_AuthSignSchemeType*       pHseAuthSignScheme;
    const   Crypto_AuthContainerType*        pHseAuthContainer;
    const   uint32                           u32HseKeyCounter;
[!ENDIF!][!//
[!IF "ecu:get(concat('Crypto.HseSptEncAuthKeyExport.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON'"!][!//
    const   uint32                           u32HseExportCipherKeyHandle;
    const   Crypto_ExportCipherSchemeType*   pHseExportCipherScheme;
    const   uint32                           u32HseExportAuthKeyHandle;
    const   Crypto_AuthMacSchemeType*        pHseExportAuthMacScheme;
    const   Crypto_AuthSignSchemeType*       pHseExportAuthSignScheme;
[!ENDIF!][!//
[!IF "ecu:get(concat('Crypto.HseSptEccKeyImport.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON'"!][!//
    const   uint8                            u8HseEccCurveId;
[!ENDIF!][!//
[!IF "ecu:get(concat('Crypto.HseSptSymKeyImport.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON' or
      ecu:get(concat('Crypto.HseSptRsaKeyImport.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON' or
      ecu:get(concat('Crypto.HseSptEccKeyImport.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON' or
      ecu:get(concat('Crypto.HseSptKeyDerive.',    string(node:fallback(CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON'"!][!//
    const   uint16                           u16HseKeyFlags;
    const   uint32                           u32HseKeySmrFlags;
[!ENDIF!][!//
    const   uint8                            u8HseKeyType;
    const   uint32                           u32HseKeyHandle;
} Crypto_KeyElementType;

[!IF "node:exists(CryptoGeneral/CryptoUpdateNvramBlobHandler)"!][!//
/* Pointer type to the upper layer handler that will update in Nvram blobs of persistent information */
typedef Std_ReturnType (*Crypto_UpdateNvramBlobHandlerType)(uint32 u32BlobId, uint32 u32BlobLength);

[!ENDIF!][!//
[!ENDIF!][!//


/* Structure containing the number and list of Crypto Driver Objects allocated to a partition */
typedef struct
{
    const uint8  u8NumCDOs;
    const uint8* au8CDOsList;
}Crypto_PartitionToCdoMappingType;

/* Structure storing information about a Crypto Driver Object */
typedef struct
{
[!IF "num:i(count(CryptoGeneral/CryptoEcucPartitionRef/*)) > 0"!][!//
    const uint32                      u32ValidPartitionId;
[!ENDIF!][!//
    Crypto_QueueElementType* const    pQueuedJobs;
    const uint32                      u32CryptoQueueSize;
    const Crypto_PrimitiveType* const pCryptoKeyPrimitives;
    const uint32                      u32NoCryptoPrimitives;
} Crypto_ObjectType;

/* Structure storing information about which Mu instance is allocated to a partition */
typedef struct
{
    const uint8 u8MuInstance;
    const uint8 u8IdxMuInstance;
} Crypto_PartitionToMuMappingType;

/*==================================================================================================
*                                  GLOBAL CONSTANTS DECLARATIONS
==================================================================================================*/
[!IF "ecu:get(concat('Crypto.HseSptFormatKeyCatalogs.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON'"!][!//
#define CRYPTO_START_SEC_CONST_UNSPECIFIED
#include "Crypto_MemMap.h"

/* Variables related to Hse KeyCatalog feature */
extern const hseKeyGroupCfgEntry_t aHseNvmKeyCatalog[[!"num:i(count(NvmKeyCatalog/*)+1)"!]U];
extern const hseKeyGroupCfgEntry_t aHseRamKeyCatalog[[!"num:i(count(RamKeyCatalog/*)+1)"!]U];

#define CRYPTO_STOP_SEC_CONST_UNSPECIFIED
#include "Crypto_MemMap.h"
[!ENDIF!][!//

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
[!IF "node:exists(CryptoKeyElements)"!][!//
#define CRYPTO_START_SEC_VAR_CLEARED_8
#include "Crypto_MemMap.h"

/* Array storing blob 0 containing information that should be persistent between resets */
extern uint8 Crypto_au8NvramBlob0[CRYPTO_SIZEOF_NVRAM_BLOB_0];

#define CRYPTO_STOP_SEC_VAR_CLEARED_8
#include "Crypto_MemMap.h"


    [!NOCODE!][!//
    /* Create the uint8 array with the information that needs to be stored in Nvram for all persistent Key Elements (buffers + actual sizes) */
    /* Count first the max size of the blob storing information about the persistent key elements. Need to be known when to append commas after each value */
    [!VAR "SizeOfKeyElemPersistValuesBlob"="0"!][!//
    [!VAR "KeyElemPersistBlobHasAllBytesZero"="'true'"!][!//
    [!LOOP "CryptoKeyElements/CryptoKeyElement/*"!][!//
        [!IF "node:value(./UseHseKey)='false' and node:value(./CryptoKeyElementPersist) = 'true' "!][!//
            /* If the key element does not use a HSE key, add the max size of the key element buffer + 4 bytes storing the 32bit value of the actual size */
            [!VAR "SizeOfKeyElemPersistValuesBlob" = "$SizeOfKeyElemPersistValuesBlob + node:value(./CryptoKeyElementSize) + num:i(4)"!][!//
            [!IF "num:i(string-length(node:value(./CryptoKeyElementInitValue)))!=0"!][!//
                [!VAR "KeyElemPersistBlobHasAllBytesZero"="'false'"!][!//
            [!ENDIF!][!//
        [!ELSEIF "node:value(./UseHseKey)='true' and node:name(concat(noderef:ref(HseKeyCatalogGroupRef), '/../.')) = 'NvmKeyCatalog' "!][!//
            /* If the key element uses a HSE key and it is a Nvm key, add the 4 bytes storing the 32bit value of the actual size */
            [!VAR "SizeOfKeyElemPersistValuesBlob" = "$SizeOfKeyElemPersistValuesBlob + num:i(4)"!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
    [!IF "$SizeOfKeyElemPersistValuesBlob > 0"!][!//
        [!IF "$KeyElemPersistBlobHasAllBytesZero = 'true'"!][!//
            [!INDENT "0"!]
            [!CODE!][!//
                #define CRYPTO_START_SEC_VAR_CLEARED_8_NO_CACHEABLE
                #include "Crypto_MemMap.h"

                /* Array storing blob 1 containing information that should be persistent between resets */
                extern uint8 Crypto_au8NvramBlob1[CRYPTO_SIZEOF_NVRAM_BLOB_1];

                #define CRYPTO_STOP_SEC_VAR_CLEARED_8_NO_CACHEABLE
                #include "Crypto_MemMap.h"

            [!ENDCODE!][!//
            [!ENDINDENT!][!//
        [!ELSE!][!//
            [!INDENT "0"!]
            [!CODE!][!//
                #define CRYPTO_START_SEC_VAR_INIT_8_NO_CACHEABLE
                #include "Crypto_MemMap.h"

                /* Array storing blob 1 containing information that should be persistent between resets */
                extern uint8 Crypto_au8NvramBlob1[CRYPTO_SIZEOF_NVRAM_BLOB_1];

                #define CRYPTO_STOP_SEC_VAR_INIT_8_NO_CACHEABLE
                #include "Crypto_MemMap.h"

            [!ENDCODE!][!//
            [!ENDINDENT!][!//
        [!ENDIF!][!//
    [!ENDIF!][!//
    [!ENDNOCODE!][!//
[!ENDIF!][!//


/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/
[!IF "node:exists(CryptoGeneral/CryptoUpdateNvramBlobHandler)"!][!//
/**
* @brief            Handler to be implemented in the upper layer, for updating the information of the blob pointed by Crypto into Nvram.
*
* @param[in]        u32BlobId        Holds the identifier of the blob the Crypto driver is requesting the upper layer to save to NVRAM
*                                    Can have one of the 2 possible following values:
*                                       - CRYPTO_NVRAM_BLOB_0_ID,
*                                         When the u32BlobId handler parameter has the value above, the length of the blob will always be equal to the value of the define CRYPTO_SIZEOF_NVRAM_BLOB_0
*                                       - CRYPTO_NVRAM_BLOB_1_ID
*                                         When the u32BlobId handler parameter has the value above, the length of the blob will always be equal to the value of the define CRYPTO_SIZEOF_NVRAM_BLOB_1
* @param[in]        u32BlobLength    Holds the length of the blob the Crypto driver is requesting the upper layer to update to NVRAM
*
* @return           Result of the operation. Upper layer should respond with one of the following return values:
* @retval           E_OK                        Request was successful, upper layer updated successfully the blob into NVRAM
* @retval           E_NOT_OK                    Request failed, blob data was not successfully updated into NVRAM
*
*/
extern Std_ReturnType [!"CryptoGeneral/CryptoUpdateNvramBlobHandler"!]
(
    uint32 u32BlobId,
    uint32 u32BlobLength
);
[!ENDIF!][!//


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CRYPTO_CFG_H */

