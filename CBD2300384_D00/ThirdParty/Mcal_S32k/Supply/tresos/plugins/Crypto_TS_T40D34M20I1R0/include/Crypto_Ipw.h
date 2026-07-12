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

#ifndef CRYPTO_IPW_H
#define CRYPTO_IPW_H

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
#include "Crypto_Hse.h"
#include "Crypto_Cfg.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CRYPTO_VENDOR_ID_IPW_H                       43
#define CRYPTO_AR_RELEASE_MAJOR_VERSION_IPW_H        4
#define CRYPTO_AR_RELEASE_MINOR_VERSION_IPW_H        4
#define CRYPTO_AR_RELEASE_REVISION_VERSION_IPW_H     0
#define CRYPTO_SW_MAJOR_VERSION_IPW_H                2
#define CRYPTO_SW_MINOR_VERSION_IPW_H                0
#define CRYPTO_SW_PATCH_VERSION_IPW_H                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

/* Check if Crypto Ipw header file and Crypto Hse header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_IPW_H != CRYPTO_VENDOR_ID_HSE_H)
    #error "Crypto_Ipw.h and Crypto_Hse.h have different vendor ids"
#endif

/* Check if Crypto Ipw header file and Crypto Hse header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_IPW_H    != CRYPTO_AR_RELEASE_MAJOR_VERSION_HSE_H) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_IPW_H    != CRYPTO_AR_RELEASE_MINOR_VERSION_HSE_H) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_IPW_H != CRYPTO_AR_RELEASE_REVISION_VERSION_HSE_H) \
    )
    #error "AutoSar Version Numbers of Crypto_Ipw.h and Crypto_Hse.h are different"
#endif

/* Check if Crypto Ipw header file and Crypto Hse header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_IPW_H != CRYPTO_SW_MAJOR_VERSION_HSE_H) || \
     (CRYPTO_SW_MINOR_VERSION_IPW_H != CRYPTO_SW_MINOR_VERSION_HSE_H) || \
     (CRYPTO_SW_PATCH_VERSION_IPW_H != CRYPTO_SW_PATCH_VERSION_HSE_H)    \
    )
    #error "Software Version Numbers of Crypto_Ipw.h and Crypto_Hse.h are different"
#endif


/* Check if Crypto Ipw header file and Crypto Cfg header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_IPW_H != CRYPTO_VENDOR_ID_CFG)
    #error "Crypto_Ipw.h and Crypto_Cfg.h have different vendor ids"
#endif

/* Check if Crypto Ipw header file and Crypto Cfg header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_IPW_H    != CRYPTO_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_IPW_H    != CRYPTO_AR_RELEASE_MINOR_VERSION_CFG) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_IPW_H != CRYPTO_AR_RELEASE_REVISION_VERSION_CFG) \
    )
    #error "AutoSar Version Numbers of Crypto_Ipw.h and Crypto_Cfg.h are different"
#endif

/* Check if Crypto Ipw header file and Crypto Cfg header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_IPW_H != CRYPTO_SW_MAJOR_VERSION_CFG) || \
     (CRYPTO_SW_MINOR_VERSION_IPW_H != CRYPTO_SW_MINOR_VERSION_CFG) || \
     (CRYPTO_SW_PATCH_VERSION_IPW_H != CRYPTO_SW_PATCH_VERSION_CFG)    \
    )
    #error "Software Version Numbers of Crypto_Ipw.h and Crypto_Cfg.h are different"
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
#define    Crypto_Ipw_Init(partitionId) \
          (Crypto_Hse_Init(partitionId))

#define     Crypto_Ipw_ProcessJob(partitionId,                                            objectIdx, job) \
           (Crypto_Hse_ProcessJob(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, objectIdx, job))

#define     Crypto_Ipw_CancelJob(partitionId,                                            objectIdx, job) \
           (Crypto_Hse_CancelJob(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, objectIdx, job))


#if (STD_ON == CRYPTO_KEYS_EXIST)
#define    Crypto_Ipw_ImportKey(partitionId,                                            pKeyPtr, keyLength, cryptoKeyIdx, cryptoKeyElementIdx) \
          (Crypto_Hse_ImportKey(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, pKeyPtr, keyLength, cryptoKeyIdx, cryptoKeyElementIdx))

#define     Crypto_Ipw_ExportKey(partitionId,                                            keyMaterialIdx, keyExportIdx, pKey, pResultLength) \
           (Crypto_Hse_ExportKey(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, keyMaterialIdx, keyExportIdx, pKey, pResultLength))
#define     Crypto_Ipw_KeyDeriveExtract(partitionId,                                            cryptoSrcKeyIdx, targetCryptoKeyIdx,keyElementSourceOffset,keyElementCopyLength) \
           (Crypto_Hse_KeyDeriveExtract(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, cryptoSrcKeyIdx, targetCryptoKeyIdx,keyElementSourceOffset,keyElementCopyLength))

#define     Crypto_Ipw_RandomSeed(partitionId,                                            cryptoKeyIdx, seedPtr, seedLength) \
            Crypto_Hse_RandomSeed(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, cryptoKeyIdx, seedPtr, seedLength)

#define     Crypto_Ipw_KeyGenerate(partitionId,                                            cryptoKeyIdx) \
           (Crypto_Hse_KeyGenerate(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, cryptoKeyIdx))

#define     Crypto_Ipw_KeyDerive(partitionId,                                            cryptoKeyIdx, targetKeyIdx) \
           (Crypto_Hse_KeyDerive(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, cryptoKeyIdx, targetKeyIdx))

#define     Crypto_Ipw_CalcSecret(partitionId,                                            cryptoKeyIdx, partnerPublicValuePtr, partnerPublicValueLength) \
           (Crypto_Hse_CalcSecret(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, cryptoKeyIdx, partnerPublicValuePtr, partnerPublicValueLength))

#define     Crypto_Ipw_CertificateParse(partitionId,                                            keyIdx) \
           (Crypto_Hse_CertificateParse(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, keyIdx))

#define     Crypto_Ipw_CertificateVerify(partitionId,                                            keyIdx, verifyKeyIdx, verifyPtr) \
           (Crypto_Hse_CertificateVerify(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, keyIdx, verifyKeyIdx, verifyPtr))


#if (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT)
    #define     Crypto_Ipw_RestoreKeyValidInfoFromNvram(partitionId) \
               (Crypto_Hse_RestoreKeyValidInfoFromNvram(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance))
#endif /* (STD_ON == CRYPTO_ENABLE_NVRAM_READ_WRITE_SUPPORT) */

#if (STD_ON == CRYPTO_ENABLE_FEED_HSE_DESC_SUPPORT)
    #define     Crypto_Ipw_FeedHseDescriptor(partitionId,                                            resultPtr, resultLengthPtr) \
               (Crypto_Hse_FeedHseDescriptor(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, resultPtr, resultLengthPtr))
#endif /* (STD_ON == CRYPTO_ENABLE_FEED_HSE_DESC_SUPPORT) */
#endif /* (STD_ON == CRYPTO_KEYS_EXIST) */

#define    Crypto_Ipw_MainFunction(partitionId) \
          (Crypto_Hse_MainFunction(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance))

#define     Crypto_Ipw_FormatKeyCatalogs(partitionId) \
           (Crypto_Hse_FormatKeyCatalogs(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance))

#define     Crypto_Ipw_SetSynchronousRequestsTimeout(partitionId,                                            timeout) \
           (Crypto_Hse_SetSynchronousRequestsTimeout(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, timeout))

#define     Crypto_Ipw_SheSetBootStatus(partitionId,                                            bStatus) \
           (Crypto_Hse_SheSetBootStatus(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, bStatus))

#define     Crypto_Ipw_SheGetStatus(partitionId,                                            pStatus) \
           (Crypto_Hse_SheGetStatus(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, pStatus))

#define     Crypto_Ipw_SheGetId(partitionId,                                            pChallenge, pId, pSreg, pMac) \
           (Crypto_Hse_SheGetId(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, pChallenge, pId, pSreg, pMac))

#define     Crypto_Ipw_SheDebugChal(partitionId,                                            pChallenge) \
           (Crypto_Hse_SheDebugChal(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, pChallenge))

#define     Crypto_Ipw_SheDebugAuth(partitionId,                                            pAuthorization) \
           (Crypto_Hse_SheDebugAuth(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, pAuthorization))

#define     Crypto_Ipw_SheMPCompression(partitionId,                                            pInput, u32InputLen, pResult, pResultLen) \
           (Crypto_Hse_SheMPCompression(Crypto_aPartitionToMuMapping[partitionId].u8MuInstance, pInput, u32InputLen, pResult, pResultLen))

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


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CRYPTO_IPW_H */


