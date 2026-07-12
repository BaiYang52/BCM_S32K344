/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*      \file  Crypto_30_Vtt_Custom.h
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

/**********************************************************************************************************************
*  GLOBAL CONSTANT MACROS
*********************************************************************************************************************/

#ifndef _CRYPTO_30_VTT_CUSTOM_H_
# define _CRYPTO_30_VTT_CUSTOM_H_

/* Additional Key Element defines */
# ifndef CRYPTO_KE_CUSTOM_MAC_AES_ROUNDKEY /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_MAC_AES_ROUNDKEY                               (0x81u)
# endif

# ifndef CRYPTO_KE_CUSTOM_KEYDERIVATION_LABEL /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_KEYDERIVATION_LABEL                            (0x82u)
# endif

# ifndef CRYPTO_KE_CUSTOM_KEYDERIVATION_ADDITIONAL_INFO /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_KEYDERIVATION_ADDITIONAL_INFO                  (0x83u)
# endif

/* RSA */
# ifndef CRYPTO_KE_CUSTOM_RSA_MODULUS /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RSA_MODULUS                                    (0xA0u)
# endif

# ifndef CRYPTO_KE_CUSTOM_RSA_PUBLIC_EXPONENT /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RSA_PUBLIC_EXPONENT                            (0xA1u)
# endif

# ifndef CRYPTO_KE_CUSTOM_RSA_PRIVATE_EXPONENT /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RSA_PRIVATE_EXPONENT                           (0xA2u)
# endif

# ifndef CRYPTO_KE_CUSTOM_RSA_SALT /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RSA_SALT                                       (0xA3u)
# endif

# ifndef CRYPTO_KE_CUSTOM_RSA_SALT_LENGTH /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_RSA_SALT_LENGTH                                (0xA4u)
# endif

# ifndef CRYPTO_KE_CUSTOM_TLS_CLIENT_HELLO_RANDOM /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_TLS_CLIENT_HELLO_RANDOM                        (3000u)
# endif

# ifndef CRYPTO_KE_CUSTOM_TLS_SERVER_HELLO_RANDOM /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_TLS_SERVER_HELLO_RANDOM                        (3001u)
# endif

# ifndef CRYPTO_KE_CUSTOM_TLS_HMAC_KEY_SIZE /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_TLS_HMAC_KEY_SIZE                              (3002u)
# endif

# ifndef CRYPTO_KE_CUSTOM_KEYEXCHANGE_PARTNER_PUB_KEY /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_KEYEXCHANGE_PARTNER_PUB_KEY                    (3003u)
# endif

# ifndef CRYPTO_KE_CUSTOM_SCC_CONTRACT_PUBLIC_KEY /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_SCC_CONTRACT_PUBLIC_KEY                        (3013u)
# endif

# ifndef CRYPTO_KE_CUSTOM_SCC_IV_AND_ENCRYPTED_PRIVATE_KEY /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ELEMENTS */
#  define CRYPTO_KE_CUSTOM_SCC_IV_AND_ENCRYPTED_PRIVATE_KEY               (3014u)
# endif

/* Custom algorithm defines */

/* Custom algorithm mode defines */
# ifndef CRYPTO_ALGOMODE_CUSTOM_P256R1  /* COV_CRYPTO_30_VTT_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOMODE_CUSTOM_P256R1                                   (0x80u)
# endif

/* Custom algorithm family defines */
# ifndef CRYPTO_ALGOFAM_CUSTOM_ECCANSI /* COV_CRYPTO_30_VTT_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOFAM_CUSTOM_ECCANSI                                   (0x81u)
# endif

# ifndef CRYPTO_ALGOFAM_CUSTOM_ECCSEC  /* COV_CRYPTO_30_VTT_CUSTOM_ALGORITHM */
#  define CRYPTO_ALGOFAM_CUSTOM_ECCSEC                                    (0x82u)
# endif

/* Custom Key Exchange defines */
# ifndef CRYPTO_30_VTT_KEY_EXCHANGE_X25519 /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_VTT_KEY_EXCHANGE_X25519                             (0x00u)
# endif

# ifndef CRYPTO_30_VTT_KEY_EXCHANGE_ANSIP256R1 /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_VTT_KEY_EXCHANGE_ANSIP256R1                         (0x01u)
# endif

/* Supported Key Derivation Algorithms */

/* KDF_NIST_800-108 'KDF in Counter Mode' with SHA-256 as PRF */
# ifndef CRYPTO_30_VTT_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256 /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_VTT_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256   (0x01u)
# endif

/* NIST.FIPS.186-4 'Key Pair Generation Using Extra Random Bits' with 'KDF in Counter Mode' as RBG */
# ifndef CRYPTO_30_VTT_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_VTT_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB           (0x02u)
# endif

/* NIST.SP.800-56A Single-Step KDF with Option 1: H(x) = hash(x) */
# ifndef CRYPTO_30_VTT_KDF_ALGO_KDF_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256 /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_VTT_KDF_ALGO_KDF_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256  (0x03u)
# endif

/* ISO 15118 install and update certificate.*/
# ifndef CRYPTO_30_VTT_KDF_ALGO_KDF_ISO_15118_CERTIFICATE_HANDLING /* COV_CRYPTO_30_VTT_CUSTOM_KEY_ALGORITHM */
#  define CRYPTO_30_VTT_KDF_ALGO_KDF_ISO_15118_CERTIFICATE_HANDLING     (0x04u)
# endif

#endif /* _CRYPTO_30_VTT_CUSTOM_H_ */
/**********************************************************************************************************************
*  END OF FILE: Crypto_30_Vtt_Custom.h
*********************************************************************************************************************/
