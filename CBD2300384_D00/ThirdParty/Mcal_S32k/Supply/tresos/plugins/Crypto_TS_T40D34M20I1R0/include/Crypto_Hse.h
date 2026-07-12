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

#ifndef CRYPTO_HSE_H
#define CRYPTO_HSE_H

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
#include "Crypto_Types.h"
#include "Crypto_Cfg.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CRYPTO_VENDOR_ID_HSE_H                       43
#define CRYPTO_AR_RELEASE_MAJOR_VERSION_HSE_H        4
#define CRYPTO_AR_RELEASE_MINOR_VERSION_HSE_H        4
#define CRYPTO_AR_RELEASE_REVISION_VERSION_HSE_H     0
#define CRYPTO_SW_MAJOR_VERSION_HSE_H                2
#define CRYPTO_SW_MINOR_VERSION_HSE_H                0
#define CRYPTO_SW_PATCH_VERSION_HSE_H                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if Crypto Hse header file and Crypto Types header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_HSE_H != CRYPTO_VENDOR_ID_TYPES)
    #error "Crypto_Hse.h and Crypto_Types.h have different vendor ids"
#endif

/* Check if Crypto Hse header file and Crypto Types header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_HSE_H    != CRYPTO_AR_RELEASE_MAJOR_VERSION_TYPES) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_HSE_H    != CRYPTO_AR_RELEASE_MINOR_VERSION_TYPES) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_HSE_H != CRYPTO_AR_RELEASE_REVISION_VERSION_TYPES) \
    )
    #error "AutoSar Version Numbers of Crypto_Hse.h and Crypto_Types.h are different"
#endif

/* Check if Crypto Hse header file and Crypto Types header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_HSE_H != CRYPTO_SW_MAJOR_VERSION_TYPES) || \
     (CRYPTO_SW_MINOR_VERSION_HSE_H != CRYPTO_SW_MINOR_VERSION_TYPES) || \
     (CRYPTO_SW_PATCH_VERSION_HSE_H != CRYPTO_SW_PATCH_VERSION_TYPES)    \
    )
    #error "Software Version Numbers of Crypto_Hse.h and Crypto_Types.h are different"
#endif

/* Check if Crypto Hse header file and Crypto Cfg header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_HSE_H != CRYPTO_VENDOR_ID_CFG)
    #error "Crypto_Hse.h and Crypto_Cfg.h have different vendor ids"
#endif

/* Check if Crypto Hse header file and Crypto Cfg header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_HSE_H    != CRYPTO_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_HSE_H    != CRYPTO_AR_RELEASE_MINOR_VERSION_CFG) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_HSE_H != CRYPTO_AR_RELEASE_REVISION_VERSION_CFG) \
    )
    #error "AutoSar Version Numbers of Crypto_Hse.h and Crypto_Cfg.h are different"
#endif

/* Check if Crypto Hse header file and Crypto Cfg header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_HSE_H != CRYPTO_SW_MAJOR_VERSION_CFG) || \
     (CRYPTO_SW_MINOR_VERSION_HSE_H != CRYPTO_SW_MINOR_VERSION_CFG) || \
     (CRYPTO_SW_PATCH_VERSION_HSE_H != CRYPTO_SW_PATCH_VERSION_CFG)    \
    )
    #error "Software Version Numbers of Crypto_Hse.h and Crypto_Cfg.h are different"
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

Std_ReturnType Crypto_Hse_Init
(
    const uint32 u32PartitionId
);

Std_ReturnType Crypto_Hse_ProcessJob
(
    const uint8           u8MuInstance,
    const uint32          u32ObjectIdx,
    Crypto_JobType* const pJob
);

Std_ReturnType Crypto_Hse_CancelJob
(
    const uint8                     u8MuInstance,
    const uint32                    u32ObjectIdx,
    const Crypto_JobInfoType* const pJobInfo
);

#if (STD_ON == CRYPTO_KEYS_EXIST)
Std_ReturnType Crypto_Hse_ImportKey
(
    const uint8        u8MuInstance,
    const uint8* const pKey,
    const uint32       u32KeyLength,
    const uint32       u32CryptoKeyIdx,
    const uint32       u32CryptoKeyElementIdx
);

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
);

Std_ReturnType Crypto_Hse_KeyDeriveExtract
(
    const uint8  u8MuInstance,
    const uint32 u32CryptoSrcKeyIdx,
    const uint32 u32TargetCryptoKeyIdx,
    const uint32 u32KeyElementSourceOffset,
    const uint32 u32KeyElementCopyLength
);

Std_ReturnType Crypto_Hse_RandomSeed
(
    const uint8        u8MuInstance,
    const uint32       u32CryptoKeyIdx,
    const uint8* const pu8SeedPtr,
    const uint32       u32SeedLength
);

Std_ReturnType Crypto_Hse_KeyGenerate
(
    const uint8  u8MuInstance,
    const uint32 u32CryptoKeyIdx
);

Std_ReturnType Crypto_Hse_KeyDerive
(
    const uint8  u8MuInstance,
    const uint32 u32CryptoKeyIdx,
    const uint32 u32TargetKeyIdx
);

Std_ReturnType Crypto_Hse_CalcSecret
(
    const uint8        u8MuInstance,
    const uint32       u32CryptoKeyIdx,
    const uint8* const pPartnerPublicValue,
    const uint32       u32PartnerPublicValueLength
);

Std_ReturnType Crypto_Hse_CertificateParse
(
    const uint8  u8MuInstance,
    const uint32 u32CryptoKeyIdx
);

Std_ReturnType Crypto_Hse_CertificateVerify
(
    const uint8                          u8MuInstance,
    const uint32                         u32CryptoKeyIdx,
    const uint32                         u32VerifyCryptoKeyIdx,
    const Crypto_VerifyResultType* const verifyPtr
);

#if (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)
Std_ReturnType Crypto_Hse_RestoreKeyValidInfoFromNvram
(
    const uint8 u8MuInstance
);
#endif /* (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT) */

#if (STD_ON == CRYPTO_ENABLE_FEED_HSE_DESC_SUPPORT)
Std_ReturnType Crypto_Hse_FeedHseDescriptor
(
    const uint8 u8MuInstance,
    uint8*      pu8ResultPtr,
    uint32*     pu32ResultLengthPtr
);
#endif /* (STD_ON == CRYPTO_ENABLE_FEED_HSE_DESC_SUPPORT) */
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

void Crypto_Hse_MainFunction
(
    const uint8 u8MuInstance
);

Std_ReturnType Crypto_Hse_FormatKeyCatalogs
(
    const uint8 u8MuInstance
);

void Crypto_Hse_SetSynchronousRequestsTimeout
(
    const uint8  u8MuInstance,
    const uint32 u32Timeout
);

Std_ReturnType Crypto_Hse_SheSetBootStatus
(
    const uint8   u8MuInstance,
    const boolean bStatus
);

Std_ReturnType Crypto_Hse_SheGetStatus
(
    const uint8  u8MuInstance,
    uint8*       pStatus
);

Std_ReturnType Crypto_Hse_SheGetId
(
    const uint8  u8MuInstance,
    const uint8* pChallenge,
    const uint8* pId,
    const uint8* pSreg,
    const uint8* pMac
);

Std_ReturnType Crypto_Hse_SheDebugChal
(
    const uint8  u8MuInstance,
    const uint8* pChallenge
);

Std_ReturnType Crypto_Hse_SheDebugAuth
(
    const uint8  u8MuInstance,
    const uint8* pAuthorization
);

Std_ReturnType Crypto_Hse_SheMPCompression
(
    const uint8   u8MuInstance,
    const uint8*  pInput,
    const uint32  u32InputLen,
    const uint8*  pResult,
    const uint32* pResultLen
);


#define CRYPTO_STOP_SEC_CODE
#include "Crypto_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CRYPTO_HSE_H */


