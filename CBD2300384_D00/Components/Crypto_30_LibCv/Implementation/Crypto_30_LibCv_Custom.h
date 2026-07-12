/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*      \file  Crypto_30_LibCv_Custom.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Internal header file for custom algorithms
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#ifndef CRYPTO_30_LIBCV_CUSTOM_H
# define CRYPTO_30_LIBCV_CUSTOM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Crypto_30_LibCv_Custom_Generated.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- Additional Key Element defines ---------------------------------------------- */

/* AES MAC */
# ifndef CRYPTO_KE_CUSTOM_MAC_AES_ROUNDKEY /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_MAC_AES_ROUNDKEY                           (129u)
# endif

/* Key Derive */
# ifndef CRYPTO_KE_CUSTOM_KEYDERIVATION_LABEL /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_KEYDERIVATION_LABEL                        (130u)
# endif

# ifndef CRYPTO_KE_CUSTOM_KEYDERIVATION_ADDITIONAL_INFO /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_KEYDERIVATION_ADDITIONAL_INFO              (131u)
# endif

# ifndef CRYPTO_KE_CUSTOM_ADDITIONAL_INFO /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_ADDITIONAL_INFO                            (131u)
# endif

/* RSA */
# ifndef CRYPTO_KE_CUSTOM_RSA_MODULUS /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RSA_MODULUS                                (160u)
# endif

# ifndef CRYPTO_KE_CUSTOM_RSA_PUBLIC_EXPONENT /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RSA_PUBLIC_EXPONENT                        (161u)
# endif

# ifndef CRYPTO_KE_CUSTOM_RSA_PRIVATE_EXPONENT /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RSA_PRIVATE_EXPONENT                       (162u)
# endif

# ifndef CRYPTO_KE_CUSTOM_RSA_SALT /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RSA_SALT                                   (163u)
# endif

# ifndef CRYPTO_KE_CUSTOM_RSA_SALT_LENGTH /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RSA_SALT_LENGTH                            (164u)
# endif

/* TLS */
# ifndef CRYPTO_KE_CUSTOM_TLS_CLIENT_HELLO_RANDOM /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_TLS_CLIENT_HELLO_RANDOM                    (3000u)
# endif

# ifndef CRYPTO_KE_CUSTOM_TLS_SERVER_HELLO_RANDOM /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_TLS_SERVER_HELLO_RANDOM                    (3001u)
# endif

# ifndef CRYPTO_KE_CUSTOM_TLS_HMAC_KEY_SIZE /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_TLS_HMAC_KEY_SIZE                          (3002u)
# endif

/* Key Exchange */
# ifndef CRYPTO_KE_CUSTOM_KEYEXCHANGE_PARTNER_PUB_KEY /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_KEYEXCHANGE_PARTNER_PUB_KEY                (3003u)
# endif

# ifndef CRYPTO_KE_CUSTOM_KEYEXCHANGE_PARTNER_PUB_KEY_2 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_KEYEXCHANGE_PARTNER_PUB_KEY_2              (3004u)
# endif

# ifndef CRYPTO_KE_CUSTOM_KEYEXCHANGE_INTERMEDIATE /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_KEYEXCHANGE_INTERMEDIATE                   (3005u)
# endif

# ifndef CRYPTO_KE_CUSTOM_KEYEXCHANGE_NUM_ECU /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_KEYEXCHANGE_NUM_ECU                        (3006u)
# endif

# ifndef CRYPTO_KE_CUSTOM_KEYEXCHANGE_ECU_ID /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_KEYEXCHANGE_ECU_ID                         (3007u)
# endif

/* Smart Charge Communication */
# ifndef CRYPTO_KE_CUSTOM_SCC_CONTRACT_PUBLIC_KEY /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_SCC_CONTRACT_PUBLIC_KEY                    (3013u)
# endif

# ifndef CRYPTO_KE_CUSTOM_SCC_IV_AND_ENCRYPTED_PRIVATE_KEY /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_SCC_IV_AND_ENCRYPTED_PRIVATE_KEY           (3014u)
# endif

/* Random Number Generation */
# ifndef CRYPTO_KE_CUSTOM_RANDOM_PERSONALIZATION_STRING /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RANDOM_PERSONALIZATION_STRING              (3015u)
# endif

# ifndef CRYPTO_KE_CUSTOM_RANDOM_ADDITIONAL_INPUT /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RANDOM_ADDITIONAL_INPUT                    (3016u)
# endif

# ifndef CRYPTO_KE_CUSTOM_RANDOM_NONCE /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RANDOM_NONCE                               (3017u)
# endif

# ifndef CRYPTO_KE_CUSTOM_RANDOM_RESEED_COUNTER /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RANDOM_RESEED_COUNTER                      (3018u)
# endif

/* SHE Key Update */
# ifndef CRYPTO_KE_CUSTOM_SHE_COUNTER /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_SHE_COUNTER                                (3019u)
# endif

# ifndef CRYPTO_KE_CUSTOM_SHE_UID /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_SHE_UID                                    (3021u)
# endif

# ifndef CRYPTO_KE_CUSTOM_SHE_BOOT_PROTECTION /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_SHE_BOOT_PROTECTION                        (3056u)
# endif

# ifndef CRYPTO_KE_CUSTOM_SHE_DEBUGGER_PROTECTION /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_SHE_DEBUGGER_PROTECTION                    (3057u)
# endif

# ifndef CRYPTO_KE_CUSTOM_SHE_DEBUG_CMD /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_SHE_DEBUG_CMD                              (3059u)
# endif

/* RSA CRT */
# ifndef CRYPTO_KE_CUSTOM_RSA_PRIME_P /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RSA_PRIME_P                                (3051u)
# endif

# ifndef CRYPTO_KE_CUSTOM_RSA_PRIME_Q /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RSA_PRIME_Q                                (3052u)
# endif

# ifndef CRYPTO_KE_CUSTOM_RSA_EXPONENT_DP /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RSA_EXPONENT_DP                            (3053u)
# endif

# ifndef CRYPTO_KE_CUSTOM_RSA_EXPONENT_DQ /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RSA_EXPONENT_DQ                            (3054u)
# endif

# ifndef CRYPTO_KE_CUSTOM_RSA_INVERSE_QI /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RSA_INVERSE_QI                             (3055u)
# endif

# ifndef CRYPTO_KE_CUSTOM_LABEL /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_LABEL                                      (3058u)
# endif

/* Spake2+ */
# ifndef CRYPTO_KE_CUSTOM_W0 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_W0                                         (3084u)
# endif

# ifndef CRYPTO_KE_CUSTOM_W1 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_W1                                         (3085u)
# endif

# ifndef CRYPTO_KE_CUSTOM_L /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_L                                          (3086u)
# endif

# ifndef CRYPTO_KE_CUSTOM_VERIFICATION /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_VERIFICATION                               (3087u)
# endif

# ifndef CRYPTO_KE_CUSTOM_VERIFICATION_RESULT /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_VERIFICATION_RESULT                        (3088u)
# endif

/* ----- Custom Algorithm Mode Defines ---------------------------------------------- */
/* ECC curve p224r1 */
# ifndef CRYPTO_ALGOMODE_CUSTOM_P224R1  /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOMODE_CUSTOM_P224R1                               (CryptoConf_CryptoPrimitiveAlgorithmModeCustom_Crypto_30_LibCv_P224r1)
# endif

/* ECC curve p256r1 */
# ifndef CRYPTO_ALGOMODE_CUSTOM_P256R1  /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOMODE_CUSTOM_P256R1                               (CryptoConf_CryptoPrimitiveAlgorithmModeCustom_Crypto_30_LibCv_P256r1)
# endif

/* RSA CRT */
# ifndef CRYPTO_ALGOMODE_CUSTOM_RSASSA_PKCS1_v1_5_CRT /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOMODE_CUSTOM_RSASSA_PKCS1_v1_5_CRT                (CryptoConf_CryptoPrimitiveAlgorithmModeCustom_Crypto_30_LibCv_RSASSA_PKCS1_v1_5_CRT)
# endif

# ifndef CRYPTO_ALGOMODE_CUSTOM_RSAES_OAEP_CRT /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOMODE_CUSTOM_RSAES_OAEP_CRT                       (CryptoConf_CryptoPrimitiveAlgorithmModeCustom_Crypto_30_LibCv_RSAES_OAEP_CRT)
# endif

/* Use Derivation function e.g. for NIST DRBG AES128 */
# ifndef CRYPTO_ALGOMODE_CUSTOM_USE_DF /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOMODE_CUSTOM_USE_DF                               (CryptoConf_CryptoPrimitiveAlgorithmModeCustom_Crypto_30_LibCv_UseDF)
# endif

/* ECC curve p384r1 */
# ifndef CRYPTO_ALGOMODE_CUSTOM_P384R1  /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOMODE_CUSTOM_P384R1                               (CryptoConf_CryptoPrimitiveAlgorithmModeCustom_Crypto_30_LibCv_P384r1)
# endif

/* ECC curve p160r1 */
# ifndef CRYPTO_ALGOMODE_CUSTOM_P160R1  /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOMODE_CUSTOM_P160R1                               (CryptoConf_CryptoPrimitiveAlgorithmModeCustom_Crypto_30_LibCv_P160r1)
# endif

/* ECC curve p521r1 */
# ifndef CRYPTO_ALGOMODE_CUSTOM_P521R1  /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOMODE_CUSTOM_P521R1                               (CryptoConf_CryptoPrimitiveAlgorithmModeCustom_Crypto_30_LibCv_P521r1)
# endif

/* Aead Aes Ccm */
# ifndef CRYPTO_ALGOMODE_CUSTOM_CCM /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOMODE_CUSTOM_CCM                                  (CryptoConf_CryptoPrimitiveAlgorithmModeCustom_Crypto_30_LibCv_CCM)
# endif

/* Modes */
# ifndef CRYPTO_ALGOMODE_CUSTOM_MODE_1  /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOMODE_CUSTOM_MODE_1                               (CryptoConf_CryptoPrimitiveAlgorithmModeCustom_Crypto_30_LibCv_Mode1)
# endif

/* ----- Custom Algorithm Family Defines ---------------------------------------------- */
/* Ecc-ANSI conform */
# ifndef CRYPTO_ALGOFAM_CUSTOM_ECCANSI /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOFAM_CUSTOM_ECCANSI                               (CryptoConf_CryptoPrimitiveAlgorithmFamilyCustom_Crypto_30_LibCv_ECCANSI)
# endif
/* Ecc-SEC conform */
# ifndef CRYPTO_ALGOFAM_CUSTOM_ECCSEC  /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOFAM_CUSTOM_ECCSEC                                (CryptoConf_CryptoPrimitiveAlgorithmFamilyCustom_Crypto_30_LibCv_ECCSEC)
# endif
/* NIST 800-90A DRBG conform */
# ifndef CRYPTO_ALGOFAM_CUSTOM_DRBG /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOFAM_CUSTOM_DRBG                                  (CryptoConf_CryptoPrimitiveAlgorithmFamilyCustom_Crypto_30_LibCv_DRBG)
# endif
/* FIPS 186.2 conform */
# ifndef CRYPTO_ALGOFAM_CUSTOM_FIPS186  /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOFAM_CUSTOM_FIPS186                               (CryptoConf_CryptoPrimitiveAlgorithmFamilyCustom_Crypto_30_LibCv_FIPS186)
# endif
/* Padding PKCS7 */
# ifndef CRYPTO_ALGOFAM_CUSTOM_PADDING_PKCS7 /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOFAM_CUSTOM_PADDING_PKCS7                         (CryptoConf_CryptoPrimitiveAlgorithmFamilyCustom_Crypto_30_LibCv_PaddingPKCS7)
# endif
/* Key exchange X25519 */
# ifndef CRYPTO_ALGOFAM_CUSTOM_X25519 /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOFAM_CUSTOM_X25519                                (CryptoConf_CryptoPrimitiveAlgorithmFamilyCustom_Crypto_30_LibCv_X25519)
# endif
/* Key Derive ISO15118 */
# ifndef CRYPTO_ALGOFAM_CUSTOM_ISO15118 /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOFAM_CUSTOM_ISO15118                              (CryptoConf_CryptoPrimitiveAlgorithmFamilyCustom_Crypto_30_LibCv_ISO15118)
# endif
/* Poly 1305 */
# ifndef CRYPTO_ALGOFAM_CUSTOM_POLY_1305 /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOFAM_CUSTOM_POLY_1305                             (CryptoConf_CryptoPrimitiveAlgorithmFamilyCustom_Crypto_30_LibCv_POLY1305)
# endif
/* Spake2+ */
# ifndef CRYPTO_ALGOFAM_CUSTOM_SPAKE2_PLUS /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOFAM_CUSTOM_SPAKE2_PLUS                           (CryptoConf_CryptoPrimitiveAlgorithmFamilyCustom_Crypto_30_LibCv_SPAKE2PLUS)
# endif
/* HKDF */
# ifndef CRYPTO_ALGOFAM_CUSTOM_HKDF /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOFAM_CUSTOM_HKDF                                  (CryptoConf_CryptoPrimitiveAlgorithmFamilyCustom_Crypto_30_LibCv_HKDF)
# endif
/* Cmd get ID */
# ifndef CRYPTO_ALGOFAM_CUSTOM_CMD_GET_ID /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOFAM_CUSTOM_CMD_GET_ID                            (CryptoConf_CryptoPrimitiveAlgorithmFamilyCustom_Crypto_30_LibCv_CmdGetId)
# endif
/* Cipher  Suites */
# ifndef CRYPTO_ALGOFAM_CUSTOM_CIPHER_SUITE_8  /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOFAM_CUSTOM_CIPHER_SUITE_8                        (CryptoConf_CryptoPrimitiveAlgorithmFamilyCustom_Crypto_30_LibCv_CipherSuite8)
# endif
/* Hash MD5 */
# ifndef CRYPTO_ALGOFAM_CUSTOM_MD5 /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOFAM_CUSTOM_MD5                                   (CryptoConf_CryptoPrimitiveAlgorithmFamilyCustom_Crypto_30_LibCv_MD5)
# endif
/* Burmester Desmedt */
# ifndef CRYPTO_ALGOFAM_CUSTOM_BD /* COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOFAM_CUSTOM_BD                                    (CryptoConf_CryptoPrimitiveAlgorithmFamilyCustom_Crypto_30_LibCv_BD)
# endif

/* ----- Custom Key Exchange Algorithm Defines ---------------------------------------------- */
/* Exchange algorithm x25519 */
# ifndef CRYPTO_30_LIBCV_KEY_EXCHANGE_X25519 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KEY_EXCHANGE_X25519                         (0u)
# endif
/* Exchange algorithm ANSI p256r1 */
# ifndef CRYPTO_30_LIBCV_KEY_EXCHANGE_ANSIP256R1 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KEY_EXCHANGE_ANSIP256R1                     (1u)
# endif
/* Exchange algorithm SEC p256r1 */
# ifndef CRYPTO_30_LIBCV_KEY_EXCHANGE_SECP256R1 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KEY_EXCHANGE_SECP256R1                      (2u)
# endif

/* Exchange algorithm Nist p224r1 for burmester desmedt */
# ifndef CRYPTO_30_LIBCV_KEY_EXCHANGE_NISTP224R1_BD /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KEY_EXCHANGE_NISTP224R1_BD                  (3u)
# endif

/* Exchange algorithm SEC p384r1 */
# ifndef CRYPTO_30_LIBCV_KEY_EXCHANGE_SECP384R1 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KEY_EXCHANGE_SECP384R1                      (4u)
# endif

/* Exchange algorithm Spake2+ Cipher Suite 8 */
# ifndef CRYPTO_30_LIBCV_KEY_EXCHANGE_SPAKE2_PLUS_CIPHERSUITE_8 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KEY_EXCHANGE_SPAKE2_PLUS_CIPHERSUITE_8      (5u)
# endif
# ifndef CRYPTO_30_LIBCV_KEY_EXCHANGE_SPAKE2_PLUS_CIPHERSUITE_8_1 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KEY_EXCHANGE_SPAKE2_PLUS_CIPHERSUITE_8_1    (6u)
# endif

/* Exchange algorithm SEC p521r1 */
# ifndef CRYPTO_30_LIBCV_KEY_EXCHANGE_SECP521R1 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KEY_EXCHANGE_SECP521R1                      (7u)
# endif

/* ----- Custom Key Generate Algorithm Defines ---------------------------------------------- */
/* Key Generate Symmetric Generic */
# ifndef CRYPTO_30_LIBCV_KEY_GENERATE_SYMMETRIC /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KEY_GENERATE_SYMMETRIC                      (0u)
# endif
/* Key Generate algorithm P256R1 */
# ifndef CRYPTO_30_LIBCV_KEY_GENERATE_P256R1 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KEY_GENERATE_P256R1                         (1u)
# endif
/* Key Generate algorithm P384R1 */
# ifndef CRYPTO_30_LIBCV_KEY_GENERATE_P384R1 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KEY_GENERATE_P384R1                         (2u)
# endif
# ifndef CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_ED25519 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_ED25519              (3u)
# endif

/* ----- Custom Key Derivation Algorithm Defines ---------------------------------------------- */
/* KDF_NIST_800-108 'KDF in Counter Mode' with SHA-256 as PRF */
# ifndef CRYPTO_30_LIBCV_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256 (1u)
# endif

/* NIST.FIPS.186-4 'Key Pair Generation Using Extra Random Bits' with 'KDF in Counter Mode' as RBG */
# ifndef CRYPTO_30_LIBCV_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB       (2u)
# endif

/* NIST.SP.800-56A Single-Step KDF with Option 1: H(x) = hash(x) */
# ifndef CRYPTO_30_LIBCV_KDF_ALGO_KDF_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KDF_ALGO_KDF_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256 (3u)
# endif

/* ISO 15118 install and update certificate. */
# ifndef CRYPTO_30_LIBCV_KDF_ALGO_KDF_ISO_15118_CERTIFICATE_HANDLING /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KDF_ALGO_KDF_ISO_15118_CERTIFICATE_HANDLING (4u)
# endif

/* KDF X9.63 SHA 1 */
# ifndef CRYPTO_30_LIBCV_KDF_ALGO_KDF_X963_SHA1 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KDF_ALGO_KDF_X963_SHA1                      (5u)
# endif

/* KDF X9.63 SHA 256 */
# ifndef CRYPTO_30_LIBCV_KDF_ALGO_KDF_X963_SHA256 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KDF_ALGO_KDF_X963_SHA256                    (6u)
# endif

/* KDF X9.63 SHA 512 */
# ifndef CRYPTO_30_LIBCV_KDF_ALGO_KDF_X963_SHA512 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KDF_ALGO_KDF_X963_SHA512                    (7u)
# endif

/* PBKDF2 HMAC SHA 1 */
# ifndef CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA1 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA1                   (9u)
# endif

/* PBKDF2 HMAC SHA 256 */
# ifndef CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA256 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KDF_ALGO_PBKDF2_HMAC_SHA256                 (10u)
# endif

/* HKDF HMAC SHA 256 */
# ifndef CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA256 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA256                   (11u)
# endif

/* KDF Spake2+ P256R1 */
# ifndef CRYPTO_30_LIBCV_KDF_ALGO_SPAKE2_PLUS_P256R1 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KDF_ALGO_SPAKE2_PLUS_P256R1                 (12u)
# endif

/* HKDF HMAC Option 1 SHA2-256 */
# ifndef CRYPTO_30_LIBCV_KDF_ALGO_HKDF_OPTION1_SHA256 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KDF_ALGO_HKDF_OPTION1_SHA256                (13u)
# endif

/* HKDF HMAC SHA2-384 */
# ifndef CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA384 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_KDF_ALGO_HKDF_HMAC_SHA384                   (14u)
# endif

/* ----- Custom Random Seed Algorithm Defines ---------------------------------------------- */
/* Random Seed according to FIPS 186-2 SHA1 */
# ifndef CRYPTO_30_LIBCV_RNG_FIPS_186_2_SHA1 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_RNG_FIPS_186_2_SHA1                         (0u)
# endif

/* Random Seed according to NIST 800-90A CTR_DRBG with AES-128 */
# ifndef CRYPTO_30_LIBCV_RNG_NIST_800_90A_CTR_DRBG_AES128 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_RNG_NIST_800_90A_CTR_DRBG_AES128            (1u)
# endif

/* Random Seed according to NIST 800-90A CTR_DRBG with AES-128 and DF */
# ifndef CRYPTO_30_LIBCV_RNG_NIST_800_90A_CTR_DRBG_AES128_DF /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_RNG_NIST_800_90A_CTR_DRBG_AES128_DF         (2u)
# endif

/* Random Seed according to NIST 800-90A HASH_DRBG */
# ifndef CRYPTO_30_LIBCV_RNG_NIST_800_90A_HASH_DRBG_SHA_512 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_RNG_NIST_800_90A_HASH_DRBG_SHA_512          (3u)
# endif

/* Random Seed according to NIST 800-90A CTR_DRBG with AES-256 */
# ifndef CRYPTO_30_LIBCV_RNG_NIST_800_90A_CTR_DRBG_AES256 /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_RNG_NIST_800_90A_CTR_DRBG_AES256            (4u)
# endif

/* Random Seed according to NIST 800-90A CTR_DRBG with AES-256 and DF */
# ifndef CRYPTO_30_LIBCV_RNG_NIST_800_90A_CTR_DRBG_AES256_DF /* COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_LIBCV_RNG_NIST_800_90A_CTR_DRBG_AES256_DF         (5u)
# endif

#endif /* CRYPTO_30_LIBCV_CUSTOM_H */
/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_Custom.h
 *********************************************************************************************************************/
