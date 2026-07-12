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

#ifndef CRYPTO_PRIVATE_H
#define CRYPTO_PRIVATE_H

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

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CRYPTO_VENDOR_ID_PRIVATE                       43
#define CRYPTO_AR_RELEASE_MAJOR_VERSION_PRIVATE        4
#define CRYPTO_AR_RELEASE_MINOR_VERSION_PRIVATE        4
#define CRYPTO_AR_RELEASE_REVISION_VERSION_PRIVATE     0
#define CRYPTO_SW_MAJOR_VERSION_PRIVATE                2
#define CRYPTO_SW_MINOR_VERSION_PRIVATE                0
#define CRYPTO_SW_PATCH_VERSION_PRIVATE                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if Crypto private header file and Crypto Cfg header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_PRIVATE != CRYPTO_VENDOR_ID_CFG)
    #error "Crypto_Private.h and Crypto_Cfg.h have different vendor ids"
#endif

/* Check if Crypto private header file and Crypto Cfg header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_PRIVATE    != CRYPTO_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_PRIVATE    != CRYPTO_AR_RELEASE_MINOR_VERSION_CFG) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_PRIVATE != CRYPTO_AR_RELEASE_REVISION_VERSION_CFG) \
    )
    #error "AutoSar Version Numbers of Crypto_Private.h and Crypto_Cfg.h are different"
#endif

/* Check if Crypto private header file and Crypto Cfg header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_PRIVATE != CRYPTO_SW_MAJOR_VERSION_CFG) || \
     (CRYPTO_SW_MINOR_VERSION_PRIVATE != CRYPTO_SW_MINOR_VERSION_CFG) || \
     (CRYPTO_SW_PATCH_VERSION_PRIVATE != CRYPTO_SW_PATCH_VERSION_CFG)    \
    )
    #error "Software Version Numbers of Crypto_Private.h and Crypto_Cfg.h are different"
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/
#define CRYPTO_START_SEC_CONST_8
#include "Crypto_MemMap.h"


/* Array of structures storing the information of the Crypto Driver Objects mapped on partitions */
extern const Crypto_PartitionToCdoMappingType Crypto_aPartitionToCdoMapping[CRYPTO_MAX_NUMBER_PARTITIONS_U8];

/* Array storing the mapping of the MU instance per partition */
extern const Crypto_PartitionToMuMappingType Crypto_aPartitionToMuMapping[CRYPTO_MAX_NUMBER_PARTITIONS_U8];

#define CRYPTO_STOP_SEC_CONST_8
#include "Crypto_MemMap.h"


#define CRYPTO_START_SEC_CONST_UNSPECIFIED
#include "Crypto_MemMap.h"

/* Array of structures storing the information about the Crypto Driver Objects */
extern const Crypto_ObjectType Crypto_aDriverObjectList[CRYPTO_NUMBER_OF_DRIVER_OBJECTS_U32];

#if (STD_ON == CRYPTO_KEYS_EXIST)
    /* Array of structures storing the information for the Crypto Keys */
    extern const Crypto_KeyType Crypto_aKeyList[CRYPTO_NUMBER_OF_KEYS_U32];
    /* Array of structures storing the information for the Crypto Key Elements */
    extern const Crypto_KeyElementType Crypto_aKeyElementList[CRYPTO_NUMBER_OF_KEY_ELEMENTS_U32];
    #if (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)
        /* Pointer to the upper layer handler that will update in Nvram blobs of persistent information */
        extern const Crypto_UpdateNvramBlobHandlerType pfCryptoUpdateNvramBlobHandler;
    #endif /* (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT) */
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

#define CRYPTO_STOP_SEC_CONST_UNSPECIFIED
#include "Crypto_MemMap.h"

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
#if (STD_ON == CRYPTO_KEYS_EXIST)
    /* Functions like macro for handling the key valid flags stored in a 8 bit blocks bit map */
    #define Crypto_IsKeyValidFlagSet(keyIdx)                ((Crypto_au8NvramBlob0[(uint32)((keyIdx) / 8U)]  &  ((uint8)((uint8)1U<<((keyIdx) % 8U)))) != 0U)
    #define Crypto_SetKeyValidFlag(keyIdx)                  (Crypto_au8NvramBlob0[(uint32)((keyIdx) / 8U)] |=  ((uint8)((uint8)1U<<((keyIdx) % 8U))))
    #define Crypto_ClearKeyValidFlag(keyIdx)                (Crypto_au8NvramBlob0[(uint32)((keyIdx) / 8U)] &= ~((uint8)((uint8)1U<<((keyIdx) % 8U))))

    /* Key Element ID of the key material */
    #define CRYPTO_KEY_MATERIAL_U32                         ((uint32)0x01U)

    #if (STD_ON == CRYPTO_SPT_SHE)
        /* Size of a SHE key */
        #define CRYPTO_SHE_KEY_SIZE_U32                     ((uint32)16U)
        /* SHE keys Input Output */
        #define CRYPTO_M1_SIZE_U32                          ((uint32)16U)
        #define CRYPTO_M2_SIZE_U32                          ((uint32)32U)
        #define CRYPTO_M3_SIZE_U32                          ((uint32)16U)
        #define CRYPTO_M4_SIZE_U32                          ((uint32)32U)
        #define CRYPTO_M5_SIZE_U32                          ((uint32)16U)

        #define CRYPTO_INDEX_M1_U32                         (0U)
        #define CRYPTO_INDEX_M2_U32                         ((uint32)(CRYPTO_M1_SIZE_U32))
        #define CRYPTO_INDEX_M3_U32                         ((uint32)(CRYPTO_M1_SIZE_U32 + CRYPTO_M2_SIZE_U32))
        #define CRYPTO_INDEX_M4_U32                         ((uint32)(CRYPTO_M1_SIZE_U32 + CRYPTO_M2_SIZE_U32 + CRYPTO_M3_SIZE_U32))
        #define CRYPTO_INDEX_M5_U32                         ((uint32)(CRYPTO_M1_SIZE_U32 + CRYPTO_M2_SIZE_U32 + CRYPTO_M3_SIZE_U32 + CRYPTO_M4_SIZE_U32))

        #define CRYPTO_SHE_SIZE_OUT_U32                     ((uint32)(CRYPTO_M1_SIZE_U32 + CRYPTO_M2_SIZE_U32 + CRYPTO_M3_SIZE_U32 + CRYPTO_M4_SIZE_U32 + CRYPTO_M5_SIZE_U32))
        #define CRYPTO_SHE_SIZE_IN_U32                      ((uint32)(CRYPTO_M1_SIZE_U32 + CRYPTO_M2_SIZE_U32 + CRYPTO_M3_SIZE_U32))
    #endif /* (STD_ON == CRYPTO_SPT_SHE) */
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

/* Crypto extension to Std_ReturnType */
/* The service request failed because timeout occurred */
#define CRYPTO_RET_OPERATION_TIMEOUT                    ((Std_ReturnType)0x80U)
/* The service request failed because it is not supported */
#define CRYPTO_RET_NOT_SUPPORTED                        ((Std_ReturnType)0x81U)
/* The service request failed because at least one parameter is invalid. */
#define CRYPTO_RET_INVALID_PARAM                        ((Std_ReturnType)0x82U)
/* The service request failed because there was no stream available for the job */
#define CRYPTO_RET_STREAM_BUSY                          ((Std_ReturnType)0x83U)

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
#define CRYPTO_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_MemMap.h"

extern Crypto_DriverStateType Crypto_aeDriverState[CRYPTO_MAX_NUMBER_PARTITIONS_U8];

#define CRYPTO_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_MemMap.h"


#define CRYPTO_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_MemMap.h"

extern Crypto_ObjectQueueType Crypto_aObjectQueueList[CRYPTO_NUMBER_OF_DRIVER_OBJECTS_U32];

#define CRYPTO_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_MemMap.h"

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/

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
);
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
);
#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CRYPTO_PRIVATE_H */


