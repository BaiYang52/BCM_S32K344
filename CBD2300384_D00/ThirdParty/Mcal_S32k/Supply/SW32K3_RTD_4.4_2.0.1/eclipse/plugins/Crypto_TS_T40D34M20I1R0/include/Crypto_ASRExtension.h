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

#ifndef CRYPTO_ASREXTENSION_H
#define CRYPTO_ASREXTENSION_H

/**
*   @file
*
*   @addtogroup CRYPTO_ASR_EXTENSIONS
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
#define CRYPTO_VENDOR_ID_ASREXTENSION_H                       43
#define CRYPTO_AR_RELEASE_MAJOR_VERSION_ASREXTENSION_H        4
#define CRYPTO_AR_RELEASE_MINOR_VERSION_ASREXTENSION_H        4
#define CRYPTO_AR_RELEASE_REVISION_VERSION_ASREXTENSION_H     0
#define CRYPTO_SW_MAJOR_VERSION_ASREXTENSION_H                2
#define CRYPTO_SW_MINOR_VERSION_ASREXTENSION_H                0
#define CRYPTO_SW_PATCH_VERSION_ASREXTENSION_H                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if Crypto ASRExtensions header file and Crypto configuration header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_ASREXTENSION_H != CRYPTO_VENDOR_ID_CFG)
    #error "Crypto_ASRExtension.h and Crypto_Cfg.h have different vendor ids"
#endif

/* Check if Crypto ASRExtensions header file and Crypto configuration header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_ASREXTENSION_H    != CRYPTO_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_ASREXTENSION_H    != CRYPTO_AR_RELEASE_MINOR_VERSION_CFG) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_ASREXTENSION_H != CRYPTO_AR_RELEASE_REVISION_VERSION_CFG) \
    )
    #error "AutoSar Version Numbers of Crypto_ASRExtension.h and Crypto_Cfg.h are different"
#endif

/* Check if Crypto ASRExtensions header file and Crypto configuration header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_ASREXTENSION_H != CRYPTO_SW_MAJOR_VERSION_CFG) || \
     (CRYPTO_SW_MINOR_VERSION_ASREXTENSION_H != CRYPTO_SW_MINOR_VERSION_CFG) || \
     (CRYPTO_SW_PATCH_VERSION_ASREXTENSION_H != CRYPTO_SW_PATCH_VERSION_CFG)    \
    )
    #error "Software Version Numbers of Crypto_ASRExtension.h and Crypto_Cfg.h are different"
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/* AUTOSAR Extension API's service IDs */
/**
* @brief API service ID for Crypto_Exts_FormatKeyCatalogs function
* */
#define CRYPTO_EXTS_FORMATKEYCATALOGS_ID                    ((uint8)0x80U)

/**
* @brief API service ID for Crypto_Exts_SetSynchronousRequestsTimeout function
* */
#define CRYPTO_EXTS_SETSYNCREQUESTSTIMEOUT_ID               ((uint8)0x81U)

/**
* @brief API service ID for Crypto_Exts_She_BootFailure function
* */
#define CRYPTO_EXTS_SHE_BOOTFAILURE_ID                      ((uint8)0x82U)

/**
* @brief API service ID for Crypto_Exts_She_BootOk function
* */
#define CRYPTO_EXTS_SHE_BOOTOK_ID                           ((uint8)0x83U)

/**
* @brief API service ID for Crypto_Exts_She_GetStatus function
* */
#define CRYPTO_EXTS_SHE_GETSTATUS_ID                        ((uint8)0x84U)

/**
* @brief API service ID for Crypto_Exts_She_GetId function
* */
#define CRYPTO_EXTS_SHE_GETID_ID                            ((uint8)0x85U)

/**
* @brief API service ID for Crypto_Exts_She_DebugChal function
* */
#define CRYPTO_EXTS_SHE_DEBUGCHAL_ID                        ((uint8)0x86U)

/**
* @brief API service ID for Crypto_Exts_She_DebugAuth function
* */
#define CRYPTO_EXTS_SHE_DEBUGAUTH_ID                        ((uint8)0x87U)

/**
* @brief API service ID for Crypto_Exts_She_MPCompression function
* */
#define CRYPTO_EXTS_SHE_MPCOMPRESSION_ID                    ((uint8)0x88U)


/* Key Element Index Definition for Crypto ASR extension functions */
/* Key Exchange */
#ifndef CRYPTO_KE_KEYEXCHANGE_PARTNERPUBKEY
    #define CRYPTO_KE_KEYEXCHANGE_PARTNERPUBKEY             ((uint32)98U)   /* Partner Public Key */
#endif

/* Key Generate */
#ifndef CRYPTO_KE_KEYGENERATE_CURVETYPE
    #define CRYPTO_KE_KEYGENERATE_CURVETYPE                 ((uint32)29U)   /* Curve Id OID */
#endif

/* Key Derivation */
#ifndef CRYPTO_KE_KEYDERIVATION_ALGORITHM_SECONDARY
    #define CRYPTO_KE_KEYDERIVATION_ALGORITHM_SECONDARY     ((uint32)100U)  /* Secondary algorithm */
#endif
#if (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT)
    #ifndef CRYPTO_KE_KEYDERIVATION_KEY_MASTER
        #define CRYPTO_KE_KEYDERIVATION_KEY_MASTER          ((uint32)101U)  /* Master Key */
    #endif
    #ifndef CRYPTO_KE_KEYDERIVATION_OUTPUT_SIZE
        #define CRYPTO_KE_KEYDERIVATION_OUTPUT_SIZE         ((uint32)102U)  /* Output size */
    #endif
    #ifndef CRYPTO_KE_KEYDERIVATION_OUTPUT_DATA
        #define CRYPTO_KE_KEYDERIVATION_OUTPUT_DATA         ((uint32)103U)  /* Output data */
    #endif
#endif /* (STD_ON == CRYPTO_ENABLE_TLS12_DERIVE_SUPPORT) */

/* Symmetric Key Export */
#ifndef CRYPTO_KE_EXPORT_AUTHENTICATION
    #define CRYPTO_KE_EXPORT_AUTHENTICATION                 ((uint32)99U)   /* Tag/Signature */
#endif

/**
* @brief Defines for Crypto ASR extension functionality
* */
/* Extension to Crypto_AlgorithmModeType */
#ifndef CRYPTO_ALGOMODE_SIPHASH_2_4_128
    #define CRYPTO_ALGOMODE_SIPHASH_2_4_128                 ((uint8)0xfbU) /* SipHash 2-4 variant with tag length 128 bits */
#endif
#ifndef CRYPTO_ALGOMODE_FAST_CMAC
    #define CRYPTO_ALGOMODE_FAST_CMAC                       ((uint8)0xfcU)  /* Hse optimized AES CMAC */
#endif
#ifndef CRYPTO_ALGOMODE_CUSTOM_DRBG_PR
    #define CRYPTO_ALGOMODE_CUSTOM_DRBG_PR                  ((uint8)0xfdU)  /* Deterministic Random Bit Generator with prediction resistance */
#endif
#ifndef CRYPTO_ALGOMODE_CUSTOM_DRBG_RS
    #define CRYPTO_ALGOMODE_CUSTOM_DRBG_RS                  ((uint8)0xfeU)  /* Deterministic Random Bit Generator with prediction resistance and reseed for each 16 bytes of data */
#endif

    /* Extension to Crypto_AlgorithmFamilyType */
#ifndef CRYPTO_ALGOFAM_TLS12_GENERATE_MASTER
    #define CRYPTO_ALGOFAM_TLS12_GENERATE_MASTER            ((uint8)0x80U)
#endif
#ifndef CRYPTO_ALGOFAM_TLS12_KEY_EXPANSION
    #define CRYPTO_ALGOFAM_TLS12_KEY_EXPANSION              ((uint8)0x81U)
#endif
#ifndef CRYPTO_ALGOFAM_TLS12_GENERATE_VERIFY_CLIENT
    #define CRYPTO_ALGOFAM_TLS12_GENERATE_VERIFY_CLIENT     ((uint8)0x82U)
#endif
#ifndef CRYPTO_ALGOFAM_TLS12_GENERATE_VERIFY_SERVER
    #define CRYPTO_ALGOFAM_TLS12_GENERATE_VERIFY_SERVER     ((uint8)0x83U)
#endif
#ifndef CRYPTO_ALGOFAM_ED448
    #define CRYPTO_ALGOFAM_ED448                            ((uint8)0x84U)
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
 * @brief           Format Key Catalogs service.
 * @details         Used to configure the NVM and RAM key catalogs.
 *
 * @param[in]       none
 * @param[inout]    none
 * @param[out]      none
 * @returns         Std_ReturnType
 *                  E_OK: The operation was executed successfully.
 *                  E_NOT_OK: The operation could not be executed successfully.
 *
 * @pre
 *
 */
Std_ReturnType Crypto_Exts_FormatKeyCatalogs(void);

/**
 * @brief           Sets the timeout for synchronous job requests
 * @details         Sets the timeout for synchronous job requests
 *
 * @param[in]       u32Timeout - Timeout value, based on the configured 'Timeout Counter Type' the value is interpreted as ticks, microseconds or user defined unit.
 *
 * @returns         void
 *
 *
 * @pre
 *
 */
Std_ReturnType Crypto_Exts_SetSynchronousRequestsTimeout
(
    uint32 u32Timeout
);

/**
 * @brief           SHE boot failure service.
 * @details         Used to impose sanctions during invalid boot.
 *
 * @param[in]       none
 * @returns         Std_ReturnType
 *                  E_OK: The operation was executed successfully.
 *                  E_NOT_OK: The operation could not be executed successfully.
 *
 * @pre
 *
 */
Std_ReturnType Crypto_Exts_SHE_BootFailure(void);

/**
 * @brief           SHE boot ok service.
 * @details         Used to mark successful boot verification.
 *
 * @param[in]       none
 * @returns         Std_ReturnType
 *                  E_OK: The operation was executed successfully.
 *                  E_NOT_OK: The operation could not be executed successfully.
 *
 * @pre
 *
 */
Std_ReturnType Crypto_Exts_SHE_BootOk(void);

/**
 * @brief           SHE get status service.
 * @details         Used to return the contents of the status register.
 *
 * @param[out]      pStatus - Pointer to uint8 location where the function will write the SHE status
 * @returns         Std_ReturnType
 *                  E_OK: The operation was executed successfully.
 *                  E_NOT_OK: The operation could not be executed successfully.
 *
 * @pre
 *
 */
Std_ReturnType Crypto_Exts_SHE_GetStatus
(
    uint8*  pStatus
);

/**
 * @brief           SHE get id service.
 * @details         Used return the identity and the value of the status register protected by a MAC over a challenge and the data.
 *
 * @param[in]       pChallenge - Pointer to a 128-bit buffer where from the challenge will be taken
 * @param[out]      pId        - Pointer to a 128-bit buffer where UID will be stored
 * @param[out]      pSreg      - Pointer to a 8-bit buffer where status register will be stored
 * @param[out]      pMac       - Pointer to a 128-bit buffer where MAC key will be stored
 * @returns         Std_ReturnType
 *                  E_OK: The operation was executed successfully.
 *                  E_NOT_OK: The operation could not be executed successfully.
 *
 * @pre
 *
 */
Std_ReturnType Crypto_Exts_SHE_GetId
(
    const uint8* pChallenge,
    const uint8* pId,
    const uint8* pSreg,
    const uint8* pMac
);

/**
 * @brief           SHE debug challenge service.
 * @details         Used to generate a 128-bit random challenge output value that is used in conjunction with the DEBUG_AUTH command.
 *
 * @param[out]      pChallenge - Pointer to uint8 location where the output challenge will be stored
 * @returns         Std_ReturnType
 *                  E_OK: The operation was executed successfully.
 *                  E_NOT_OK: The operation could not be executed successfully.
 *
 * @pre
 *
 */
Std_ReturnType Crypto_Exts_SHE_DebugChal
(
    const uint8* pChallenge
);

/**
 * @brief           SHE debug authorization service.
 * @details         Erases all user keys.
 *
 * @param[in]       pAuthorization - Pointer to uint8 location storing authorization value
 * @returns         Std_ReturnType
 *                  E_OK: The operation was executed successfully.
 *                  E_NOT_OK: The operation could not be executed successfully.
 *
 * @pre
 *
 */
Std_ReturnType Crypto_Exts_SHE_DebugAuth
(
    const uint8* pAuthorization
);

/**
 * @brief          Miyaguchi-Preneel Compression.
 * @details        One-way compression function used to derive a 128 bit output from a given message
 *
 * @param[in]      pInputKey        Message start address
 * @param[in]      u32InputKeyLen   Message length (bytes) address
 * @param[out]     pResult          Output address
 * @param[in.out]  pResultLen       Message length (bytes) for output buffer
 * @returns        Std_ReturnType
 *                 E_OK: The operation was executed successfully.
 *                 E_NOT_OK: The operation could not be executed successfully.
 *
 * @pre
 *
*/
Std_ReturnType Crypto_Exts_MPCompression
(
    const uint8*  pInput,
          uint32  u32InputLen,
    const uint8*  pResult,
    const uint32* pResultLen
);

#define CRYPTO_STOP_SEC_CODE
#include "Crypto_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CRYPTO_ASREXTENSION_H */


