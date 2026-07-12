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
/*      \file  Crypto_30_LibCv.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Implementation of the MICROSAR Crypto Driver (Crypto)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2016-10-31  vismss                Initial creation for ASR 4.3
 *  01.00.01  2016-12-06  vismss                Minor adaptions to the specification
 *  01.00.02  2016-12-07  vismss                Fixed rng and key storage init
 *  01.00.03  2016-12-14  vismss  ESCAN00093302 Fixed AES enc/dec and EdDSA
 *  01.00.04  2017-01-24  vismss  ESCAN00093690 Support of simplified SHE Key Update Protocol
 *                                ESCAN00093350 Fixed workspace casts
 *                                ESCAN00093365 DET encapsulation
 *                                              Adaptions to ASR spec and coding style patterns
 *  01.01.00              visros                Fixed DET ServiceIds
 *                                              Added DET for process job
 *                        visgut                Fixed processing of asynchronous jobs in mainfunction.
 *                                              Include SchM_Crypto_30_LibCv.h
 *                                              Support multiple driver objects and locking of workspaces.
 *                                ESCAN00094360 Compiler warning: GetVersionInfo API missing return statement at end of non-void function
 *                        vistof                Moved Crypto_30_LibCv_QueueStateType from Csm_types.h to Crypto
 *                                              Added Cancel API
 *                        vismss                Added SipHash
 *                                              Adapted EDDSA
 *  01.01.01              vismaw  ESCAN00094769 Calling of services always returns false
 *  01.02.00  2017-05-05  vismaw  FEATC-1229    Release of component
 *                                ESCAN00093105 Release of component
 *                                ESCAN00095032 AEAD Encrypt does not check if output buffer for authentication tag has a wrong size
 *                                ESCAN00095030 Crypto_30_LibCv_KeyElementIdsGet always returns E_NOT_OK
 *                                ESCAN00095028 Wrong output length returned for AEAD Encrypt
 *                                ESCAN00095033 KeyElementSet with SHE Key update mechanism does not set the new key
 *  02.00.00  2017-08-06  vismwe  ESCAN00095629 Crypto_30_LibCv_KeyElementIdsGet returns E_NOT_OK instead of CRYPTO_E_SMALL_BUFFER
 *                                ESCAN00096063 Crypto_30_LibCv_KeyElementSet calculates wrong values for M4 proof if a SHE update key flags is set.
 *                                ESCAN00096032 Crypto_30_LibCv_KeyDerive returns suspect values, if the calculation failed.
 *                                ESCAN00095987 Crypto_30_LibCv_KeyElementSet returns always E_NOT_OK for SHE key update without the key element "proof"
 *                                ESCAN00095985 Compiler error: undeclared identifier for virtual key usage
 *                                ESCAN00095928 Crypto_30_LibCv_KeyExchangeCalcSecret does not reject calls with invalid length of public key
 *                                ESCAN00096031 Crypto_30_LibCv_KeyCopy copies key elements with different lengths without enabled partial access.
 *                                ESCAN00096176 Crypto_30_LibCv_KeyElementGet returns CRYPTO_E_KEY_NOT_VALID instead of CRYPTO_E_KEY_NOT_AVAILABLE
 *  02.01.00  2017-08-15  vistof  ESCAN00096308 MAC verification only done for complete bytes.
 *                        vismaw  STORYC-934    Optimize AES runtime through keeping the SubKey and Roundkeys of former used Keys
 *                        vismwe  ESCAN00096452 Error is returned when IV key element is missing during AES-ECB.
 *                                STORYC-281    Safe BSW Release
 *  03.00.00  2017-09-11  vismwe  STORYC-1851   Support of RSA
 *                                STORYC-2438   SHE Key Update Protocol - Use and check key slot mentioned in M1
 *                                STORYC-2485   Support Access Rights of Key Elements
 *  03.01.00  2017-09-11  vismwe  STORYC-2700   Support Key Locking
 *                                ESCAN00096711 The key element is set valid if Crypto_30_LibCv_KeyElementSet failed
 *                                ESCAN00096872 Crypto_30_LibCv_ProcessJob: Missing Det report if the Crypro <algorithm> is not supported on the driver object
 *                                STORYC-2159   Support ECC with NIST P-256 (ECDSA)
 *                                STORYC-2600   Support ECDSA for SECGp256r1
 *                                ESCAN00097178 ProcessJob returned with E_NOT_OK if the RSA signature verification failed
 *  03.02.00  2017-11-06  vismwe  STORYC-2606   Support ECDHE for ANSIp256r1
 *                                STORYC-2599   Support RSA Encrypt/Decrypt
 *                                STORYC-2604   Support HMAC with SHA1 and SHA256
 *                                ESCAN00097356 Compiler error: C2065: 'actX25519Struct' : undeclared identifier
 *                                ESCAN00097524 Crypto_30_LibCv_ProcessJob returns CRYPTO_E_KEY_NOT_VALID, if RSA signature is generated.
 *  03.03.00  2017-11-29  vismwe  STORYC-2601   Support concatenation KDF
 *                                STORYC-2603   Support certificate installation and update according to ISO15118
 *                                ESCAN00097542 Compiler error: unresolved external Crypto_30_LibCv_Local_ReadTwoKeyElements
 *  03.04.00  2018-01-08  vismwe  STORYC-2605   Support ECDSA for ANSIp256r1 with SHA-1
 *                                ESCAN00097921 Compiler error: 'CRYPTO_RANDOM' : undeclared identifier
 *  03.05.00  2018-01-22  vismwe  STORYC-3392   Support CTR-DRBG using AES-128 according to NIST SP 800-90A
 *                                STORYC-2607   Support RSA CRT DSA SHA-1 and SHA-256 and PKCS1_V15
 *                                ESCAN00098171 Compiler warning: uninitialized variable retVal.
 *                                ESCAN00098173 Compiler warning: incompatible types uint8* and uint8_least*
 *                                STORYC-2602   Support ECDHE for SECp256r1
 *                                STORYC-4075   Support delete key element
 *                                ESCAN00098280 Compiler error: Crypto_30_LibCv.c undefined identifier "Crypto_30_LibCv_QueueElementType"
 *                                ESCAN00098328 AES encryption/decryption does not fail if the key is too short.
 *                                ESCAN00098330 EDDSA 25519 generate/verify does not fail if the key is too short.
 *                                ESCAN00098322 AES CMAC Round-key Reuse does not reduce the runtime.
 *                                ESCAN00098354 Crypto_ProcessJob returned CRYPTO_E_BUSY for async job update in streaming mode.
 *  03.06.00  2018-02-19  vismwe  STORYC-4373   Support AES 128 encryption and decryption without padding
 *                                ESCAN00098488 Crypto_30_LibCv_ProcessJob return CRYPTO_E_KEY_NOT_VALID instead of E_NOT_OK if AES key length is not correct.
 *                                ESCAN00098501 Compiler error: undeclared identifiers for Ecc Curve Parameters.
 *                                ESCAN00098502 Crypto_30_LibCv_RandomSeed returns always E_NOT_OK if DRBG is used.
 *                                ESCAN00098593 Crypto_30_LibCv_ProcessJob returns E_OK instead of E_NOT_OK if key element seed state is missing for FIPS186.
 *  03.06.01  2018-04-09  vismwe  ESCAN00098720 Compiler warning: incompatible types - from 'eslt_WorkSpace *' to 'eslt_WorkSpaceEd25519 *' relevant if SysService_CryptoCv is greater 3.02.00
 *  04.00.00  2018-05-23  vismwe  STORYC-5043   Support for ECDSA 384
 *                                STORYC-5044   Support for AES 256
 *  04.01.00  2018-05-23  vismwe  STORYC-5529   SHE Key Update Protocol - Support UID, Fid and counter
 *                                ESCAN00098947 No error generated when length greater than 65535 (0xFFFF) bytes is passed to an algorithm
 *  04.02.00  2018-08-20  vismwe  ESCAN00100198 Compiler error: function Crypto_30_LibCv_CompareMac undefined
 *                                ESCAN00099294 Keys will not be persist in NVM
 *                                ESCAN00100597 AES-GCM output size check is not sufficient in streaming mode.
 *  05.00.00  2018-09-20  vismwe  STORYC-6301   Support RSAES-OAEP with SHA-1 and SHA-256
 *                                ESCAN00100617 A SHE key with debugger/boot-protection will not be provided
 *  05.01.00  2018-11-05  vismwe  STORYC-6543   Support redirection of input/output buffers from/to key elements
 *                                STORYC-6544   Support partial key element copy
 *                                STORYC-7031   Support vSecPrim watchdog trigger function
 *                                ESCAN00101150 AES 256 CBC: Crypto_ProcessJob returns CRYPTO_E_KEY_SIZE_MISMATCH in case of IV != 32Bytes
 *                                ESCAN00101246 Memory Corruption if ProcessJob is called with an invalid cryptoKeyId
 *                                ESCAN00101340 HMAC returns CRYPTO_E_KEY_NOT_VALID if key length is longer than Hash length
 *  05.02.00  2018-11-21  visivg  STORYC-7065   Support of SHA384 and SHA512 pre-hashing for ECDSA
 *                                STORYC-5518   Support of SHA384 hashing
 *                                STORYC-5520   Support of ECDHE for SECp384r1
 *                        vismwe  ESCAN00101561 Compiler warning: Crypto_30_LibCv_Local_KeyWriteLockGetNotProtected is undefined
 *                                ESCAN00101562 Compiler warning: Incompatible types - of "Crypto_30_LibCv_SizeOfKeyStorageType *" and "Crypto_30_LibCv_SizeOfKeyElementsType *"
 *  05.03.00  2019-01-02  visivg  STORYC-5807   Support GMAC AES-256, AES-192 and AES-128
 *  05.04.00  2019-01-25  visivg  STORYC-7447   Support HMAC SHA-384
 *  05.05.00  2019-02-26  visivg  STORYC-7655   Prepare Release of Crypto 5.x
 *                                ESCAN00102255 Compiler warning: 'Crypto_30_LibCv_Dispatch_RsaOaepSha1Decrypt' and 'Crypto_30_LibCv_Dispatch_RsaOaepSha256Decrypt' : undeclared identifier in Crypto_30_LibCv_cfg.c
 *                                ESCAN00099936 ProcessJob returns different values for different primitives if the key size is not matching.
 *                        vismwe  ESCAN00101248 CMAC wrong calculation for updated CMAC key, if CMAC key reuse is enabled.
 *                                ESCAN00100548 Random Number Generator FIPS186 will provide the same random numbers after restart
 *                                ESCAN00102474 Key Derivation according to ISO15118 returns always E_NOT_OK
 *                                ESCAN00102491 Unexpected behavior if a job is executed which configuration is not supported in the crypto configuration.
 *  05.06.00  2019-05-11  vismwe  STORYC-7979   Support RSAES PKCS#1 v1.5 public key decryption
 *                                STORYC-5721   MISRA-C:2012 Compliance
 *                                STORYC-7161   Support  KDF X9.63 with Sha256 in Crypto Driver
 *                                ESCAN00102689 Primitive execution always fails with an error for a specific primitive.
 *  06.00.00  2019-07-08  vismwe  SEC-62        Support Export of She RAM Keys
 *                                SEC-27        Support of She DEBUG_CMD
 *                                SEC-61        Introduce multiple NvM Blocks
 *  06.01.00  2019-08-05  vismwe  SEC-667       Introduce Immediate write of NvM Blocks
 *  06.01.01  2019-08-20  vismaw  ESCAN00104069 LibCv causes endless loop in NvM_Readall
 *  07.00.00  2019-12-03  vismwe  SEC-934       Trigger Persist only if effected persist keys are changed
 *                                ESCAN00102474 Key Derivation according to ISO15118 returns always E_NOT_OK
 *                                ESCAN00105277 Crypto_30_LibCv_ProcessJob provides wrong Det errorId if KeyId is wrong
 *                        visenc  SEC-54        Support Asynchronous Key Handling
 *  07.00.01  2020-01-20  vismwe  ESCAN00105402 Compiler error: Crypto_30_LibCv_KeyManagement.c(8760): 'writeBlock' : undeclared identifier
 *  07.01.00  2020-01-20  vismwe  SEC-10        Add callout implementation for esl_getBytesRNG
 *                                ESCAN00105358 Memory Corruption if a DRBG Random Generate job is called with less than 16 Bytes of output.
 *                                ESCAN00093129 esl_getBytesRNG is missing
 *  07.02.00  2020-03-09  vismwe  SEC-881       Remove 64k single call limitation for all security algorithms
 *                                SEC-1005      Remove limitation for signature input data pointers
 *                                ESCAN00105884 Exception caused by access to uninitialized memory.
 *                                ESCAN00105886 She Debug Command corrupts key storage data.
 *                        visenc  SEC-885       Support AES Padding One with Zeros
 *  07.03.00  2020-03-23  visenc  SEC-902       Support CryptoStack KeyGenerate Interfaces
 *  08.00.00  2020-04-17  visenc  SEC-1280      Support HMAC RIPEMD-160
 *                                ESCAN00106153 Crypto_30_LibCv_ProcessJob for RSA PKCS1 CRT returns always E_NOT_OK.
 *  08.00.01  2020-04-20  vismwe  ESCAN00106180 Primitive execution fails or provides wrong calculation for asynchronous jobs
 *  08.01.00  2020-04-29  visenc  SEC-1276      Support of ECDSA with SECP160R1
 *                                ESCAN00106259 Compiler error: Missing functions for key generation
 *  08.02.00  2020-05-25  visenc  SEC-1278      Support of HASH with RIPEMD-160
 *                                SEC-796       Support of AES CTR-Mode
 *                                SEC-954       Support of PBKDF2 HMAC SHA-1 and SHA-256
 *  08.03.00  2020-06-12  visenc  SEC-1534      Support KDF963 in combination with SHA512
 *                                ESCAN00106613 Useless access of uninitialized stack variable reseedCnt.
 *  08.04.00  2020-06-29  visenc  SEC-1250      Support ChaCha20 with Poly1305 as authenticator for AEAD
 *                                ESCAN00106735 Access of uninitialized stack variable while ProcessJob redirection and negative error code
 *  08.05.00  2020-07-20  visenc  SEC-1164      Support CTR_DRBG based on AES-128 from vSecPrim
 *                                SEC-1375      Harmonize Crypto_JobType according to different ASR version
 *                                SEC-1215      Support PRNG based on SHA512 according to SP800-90A
 *                                SEC-1472      Support SHA-3-256
 *                                ESCAN00106930 Key Init Value is not Correct or leads to compile Error
 *                                ESCAN00106936 She key update fails or compile Error
 *                                ESCAN00106783 Memory Corruption: Crypto_KeyElementCopyPartial with invalid values.
 *                                ESCAN00106807 Changed keys are not persisted with KeySetValid after WriteAll
 *                        vismwe  SEC-1687      Support Write Once Keys
 *  09.00.00  2020-08-31  visenc  SEC-1691      Support CTR-DRBG using AES-256 according to NIST SP 800-90A
 *                                SEC-1665      Support SHE command CMD_GET_ID
 *                                ESCAN00107335 MacVerify fails on Big-Endian Platforms for truncated MACs when length in bits is not divisible by 8
 *                                ESCAN00107666 Primitive execution fails or provides wrong calculation
 *  09.01.00  2020-10-27  visenc  SEC-1801      Support CMAC AES-256
 *                                SEC-1805      Support HKDF HMAC SHA256
 *                                SEC-1809      Support AES-CCM
 *                                ESCAN00107841 Access of uninitialized memory, RandomSeedBuffer in Crypto_30_LibCv_Init
 *                        vismwe  SEC-1812      Support SPAKE2+
 *  09.02.00  2020-11-20  visenc  SEC-2080      Support Hash MD5
 *                                SEC-1677      Support Key Generation for Ed25519 Keys
 *  09.03.00  2021-01-12  vismxe  SEC-1887      Support Custom Mode Synchronization according to AUTOSAR R20-11
 *                        vismwe  SEC-2263      Support Runtime DET for CRYPTO_E_ENTROPY_EXHAUSTION
 *  10.00.00  2021-05-11  visenc  SEC-2714      Support save and restore context for running crypto operations
 *                                ESCAN00109421 Crypto_30_LibCv_Local_KeyElementGet_Standard access to uninitialized memory
 *                                ESCAN00109433 Crypto_30_LibCv_Local_KeyGenerate_Ed25519_With_Ws forwards uninitialized key length variable
 *                                ESCAN00108731 SHE DEBUG Command Due to Write Lock bit Enabled Crypto Driver won't provide data to NVM during Zero Value write in to D flash
 *  10.01.00  2021-08-31  vismwe  SEC-3114      Support Elliptic Curve Burmester Desmedt (ECBD)
 *                                ESCAN00109930 SHE DEBUG Command: Key access issues after DEBUG_CMD execution.
 *  11.00.00  2021-11-29  visenc  SEC-3388      Support Key Derivation NIST SP 800-56C Option 1
 *  11.01.00  2021-12-20  visenc  SEC-3570      Support key value changed callouts
 *                        visenc  ESCAN00111088 Persistency fails or unexpected behaviour when calling KeyDerive API
 *                        viseag  SEC-3640      Usage of <Mip>_MemMap.h
 *  12.00.00  2022-05-16  visenc  SEC-4482      Support of ECDSA secp521r1 with SHA2-512
 *            2022-09-01  viseag  SEC-4863      Support of AES-192
 *            2022-10-24  viskju  ESCAN00111536 Missing validation can lead to undefined behavior
 *                                ESCAN00111597 Missing Partial Access Limitation
 *                                ESCAN00111639 Algorithm message length limitation too strict
 *  12.01.00  2022-11-11  viskju  HSM-3249      Support of ECDHE secp521r1
 *                                HSM-3317      Provide APIs for redirection for Hwa Use-Case
 *                                HSM-3096      Support SHA-3 and SHAKE according to FIPS 202
 *                                ESCAN00113238 Redirection will provide wrong data in output key element for Encrypt, Decrypt, Aead Encrypt and Aead Decrypt
 *                                ESCAN00113241 Undefined behavior after restoring context of job with configured redirection
 *                                ESCAN00113214 RSA PKCS1 decryption fails after restore
 *                                ESCAN00113213 RSA PSS signature verification fails after restore
 *  12.02.00  2023-01-24  viskju  HSM-3086      Support of HKDF HMAC SHA384
 *********************************************************************************************************************/

#if !defined (CRYPTO_30_LIBCV_H)
# define CRYPTO_30_LIBCV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Csm_Types.h"
# include "Crypto_30_LibCv_Cfg.h"
# include "Crypto_30_LibCv_KeyManagement.h"
# include "SchM_Crypto_30_LibCv.h"

# include "Crypto_30_LibCv_Custom.h"

# if (CRYPTO_30_LIBCV_USE_VSTD_LIB == STD_ON)
#  include "vstdlib.h"
# endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define CRYPTO_30_LIBCV_VENDOR_ID                                    (30u)
# define CRYPTO_30_LIBCV_MODULE_ID                                    (114u)
# define CRYPTO_30_LIBCV_INSTANCE_ID                                  (0u)

/* AUTOSAR Software specification version information */
# define CRYPTO_30_LIBCV_AR_RELEASE_MAJOR_VERSION                     (0x04u)
# define CRYPTO_30_LIBCV_AR_RELEASE_MINOR_VERSION                     (0x03u)
# define CRYPTO_30_LIBCV_AR_RELEASE_REVISION_VERSION                  (0x00u)

/* Component version information (decimal version of ALM implementation package) */
# define CRYPTO_30_LIBCV_MAJOR_VERSION                                (12u)
# define CRYPTO_30_LIBCV_MINOR_VERSION                                (2u)
# define CRYPTO_30_LIBCV_PATCH_VERSION                                (0u)

/* API service IDs */
# define CRYPTO_30_LIBCV_SID_INIT                                     (0x00u) /*!< Service ID: Crypto_30_LibCv_Init() */
# define CRYPTO_30_LIBCV_SID_GET_VERSION_INFO                         (0x01u) /*!< Service ID: Crypto_30_LibCv_GetVersionInfo() */
# define CRYPTO_30_LIBCV_SID_PROCESS_JOB                              (0x03u) /*!< Service ID: Crypto_30_LibCv_ProcessJob() */
# define CRYPTO_30_LIBCV_SID_CANCEL_JOB                               (0x0Eu) /*!< Service ID: Crypto_30_LibCv_CancelJob() */
# define CRYPTO_30_LIBCV_SID_KEY_ELEMENT_SET                          (0x04u) /*!< Service ID: Crypto_30_LibCv_KeyElementSet() */
# define CRYPTO_30_LIBCV_SID_KEY_VALID_SET                            (0x05u) /*!< Service ID: Crypto_30_LibCv_KeyValidSet() */
# define CRYPTO_30_LIBCV_SID_KEY_ELEMENT_GET                          (0x06u) /*!< Service ID: Crypto_30_LibCv_KeyElementGet() */
# define CRYPTO_30_LIBCV_SID_KEY_ELEMENT_COPY                         (0x0Fu) /*!< Service ID: Crypto_30_LibCv_KeyElementCopy() */
# define CRYPTO_30_LIBCV_SID_KEY_ELEMENT_COPY_PARTIAL                 (0x13u) /*!< Service ID: Crypto_30_LibCv_KeyElementCopyPartial() */
# define CRYPTO_30_LIBCV_SID_KEY_COPY                                 (0x10u) /*!< Service ID: Crypto_30_LibCv_KeyCopy() */
# define CRYPTO_30_LIBCV_SID_KEY_ELEMENT_IDS_GET                      (0x11u) /*!< Service ID: Crypto_30_LibCv_KeyElementIdsGet() */
# define CRYPTO_30_LIBCV_SID_RANDOM_SEED                              (0x0Du) /*!< Service ID: Crypto_30_LibCv_RandomSeed() */
# define CRYPTO_30_LIBCV_SID_KEY_GENERATE                             (0x07u) /*!< Service ID: Crypto_30_LibCv_KeyGenerate() */
# define CRYPTO_30_LIBCV_SID_KEY_DERIVE                               (0x08u) /*!< Service ID: Crypto_30_LibCv_KeyDerive() */
# define CRYPTO_30_LIBCV_SID_KEY_EXCHANGE_CALC_PUB_VAL                (0x09u) /*!< Service ID: Crypto_30_LibCv_KeyExchangeCalcPubVal() */
# define CRYPTO_30_LIBCV_SID_KEY_EXCHANGE_CALC_SECRET                 (0x0Au) /*!< Service ID: Crypto_30_LibCv_KeyExchangeCalcSecret() */
# define CRYPTO_30_LIBCV_SID_CERTIFICATE_PARSE                        (0x0Bu) /*!< Service ID: Crypto_30_LibCv_CertificateParse() */
# define CRYPTO_30_LIBCV_SID_CERTIFICATE_VERIFY                       (0x12u) /*!< Service ID: Crypto_30_LibCv_CertificateVerify() */
# define CRYPTO_30_LIBCV_SID_MAIN_FUNCTION                            (0x0Cu) /*!< Service ID: Crypto_30_LibCv_MainFunction() */
# define CRYPTO_30_LIBCV_SID_NVBLOCK_READFROMBLOCK                    (0x80u) /*!< Service ID: Crypto_30_LibCv_NvBlock_ReadFromBlock() */
# define CRYPTO_30_LIBCV_SID_NVBLOCK_WRITETOBLOCK                     (0x81u) /*!< Service ID: Crypto_30_LibCv_NvBlock_WriteToBlock() */
# define CRYPTO_30_LIBCV_SID_NVBLOCK_INIT                             (0x82u) /*!< Service ID: Crypto_30_LibCv_NvBlock_Init() */
# define CRYPTO_30_LIBCV_SID_NVBLOCK_CALLBACK                         (0x83u) /*!< Service ID: Crypto_30_LibCv_NvBlock_Callback() */
# define CRYPTO_30_LIBCV_SID_ESL_GETBYTESRNG                          (0x84u) /*!< Service ID: esl_getBytesRNG() */

/* Modes */
/* State: Uninitialized */
# define CRYPTO_30_LIBCV_UNINIT                                       (0x00u)
/* State: Initialized */
# define CRYPTO_30_LIBCV_INITIALIZED                                  (0x01u)

/* Development Error Types [SWS_Crypto_00040] */
# ifndef CRYPTO_E_NO_ERROR /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
#  define CRYPTO_E_NO_ERROR                                           (255u)
# endif
# ifndef CRYPTO_E_UNINIT /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
#  define CRYPTO_E_UNINIT                                             (0u)
# endif
# ifndef CRYPTO_E_INIT_FAILED /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
#  define CRYPTO_E_INIT_FAILED                                        (1u)
# endif
# ifndef CRYPTO_E_PARAM_POINTER /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
#  define CRYPTO_E_PARAM_POINTER                                      (2u)
# endif
# ifndef CRYPTO_E_PARAM_HANDLE /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
#  define CRYPTO_E_PARAM_HANDLE                                       (4u)
# endif
# ifndef CRYPTO_E_PARAM_VALUE /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
#  define CRYPTO_E_PARAM_VALUE                                        (5u)
# endif

/* Crypto Stack Error Codes used from Autosar 4.4 */
# ifndef CRYPTO_E_KEY_EMPTY /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
#  define CRYPTO_E_KEY_EMPTY                                          (13u)
# endif

/* Runtime Error Types [SWS_Crypto_00194] */
# ifndef CRYPTO_E_RE_ENTROPY_EXHAUSTED /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
#  define CRYPTO_E_RE_ENTROPY_EXHAUSTED                               (3u)
# endif
# ifndef CRYPTO_E_RE_GET_BYTES_RNG_ERROR /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
#  define CRYPTO_E_RE_GET_BYTES_RNG_ERROR                             (30u)
# endif

/* Data Services */
# if (CRYPTO_30_LIBCV_USE_VSTD_LIB == STD_ON)
/* Use VStdLib */
#  define Crypto_30_LibCv_CopyData(destinationPtr, sourcePtr, length) (VStdLib_MemCpy((P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))(destinationPtr), (P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))(sourcePtr), (VStdLib_CntType)(length)))
#  define Crypto_30_LibCv_ClearData(dataPtr, length)                  (VStdLib_MemClr((P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))(dataPtr), (VStdLib_CntType)(length)))
#  define Crypto_30_LibCv_SetData(dataPtr, pattern, length)           (VStdLib_MemSet((P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))(dataPtr), (pattern), (VStdLib_CntType)(length)))
# else
/* Use internal implementation */
#  define Crypto_30_LibCv_CopyData(destinationPtr, sourcePtr, length) (Crypto_30_LibCv_Local_CopyData_Implementation((P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))(destinationPtr), (P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))(sourcePtr), (length)))
#  define Crypto_30_LibCv_ClearData(dataPtr, length)                  (Crypto_30_LibCv_Local_ClearData_Implementation((P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))(dataPtr), (length)))
#  define Crypto_30_LibCv_SetData(dataPtr, pattern, length)           (Crypto_30_LibCv_Local_SetData_Implementation((P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))(dataPtr), (pattern), (length)))
# endif

/* Watchdog Trigger Function */
# if (CRYPTO_30_LIBCV_WATCHDOGTRIGGERFUNCTIONOFGENERAL == STD_ON)
#  define CRYPTO_30_LIBCV_WATCHDOG_PTR                                Crypto_30_LibCv_GetWatchdogTriggerFunctionOfGeneral()
# else
#  define CRYPTO_30_LIBCV_WATCHDOG_PTR                                (NULL_PTR)
# endif

/* Macors for Job access */
# ifndef CRYPTO_30_LIBCV_JOB_STATE_MEMBER /* COV_CRYPTO_30_LIBCV_ASR_COMPATIBILITY */
#  define CRYPTO_30_LIBCV_JOB_STATE_MEMBER                            jobState
# endif

# ifndef CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER /* COV_CRYPTO_30_LIBCV_ASR_COMPATIBILITY */
#  define CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER           jobPrimitiveInputOutput
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define CRYPTO_30_LIBCV_START_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Init()
 *********************************************************************************************************************/
/*! \brief       Initializes the Crypto Driver
 *  \details     This function initializes the module Crypto_30_LibCv. It initializes all variables and sets the
 *               module state to initialized.
 *  \pre         Interrupts are disabled.
 *               Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Specification of module initialization
 *  \trace       CREQ-131331
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Init(void);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_InitMemory()
 *********************************************************************************************************************/
/*! \brief       The function initializes variables, which cannot be initialized with the startup code.
 *  \details     Initialize component variables at power up.
 *  \pre         Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_InitMemory(void);

# if (CRYPTO_30_LIBCV_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     Crypto_30_LibCv_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CRYPTO_30_LIBCV_VERSION_INFO_API == STD_ON
 *  \trace       CREQ-131329
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) versioninfo);
# endif /* (CRYPTO_30_LIBCV_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_ProcessJob()
 *********************************************************************************************************************/
/*! \brief         Processes the received job
 *  \details       Performs the crypto primitive, that is configured in the job parameter.
 *  \param[in]     objectId                Holds the identifier of the Crypto Driver Object.
 *  \param[in,out] job                     Pointer to the configuration of the job. Contains structures with job and
 *                                         primitive relevant information but also pointer to result buffers.
 *  \return        E_OK                         Request successful.
 *                 E_NOT_OK                     Request failed.
 *                 CRYPTO_E_BUSY                Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_NOT_VALID       Request failed, the key is not valid.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, a key element has the wrong size.
 *                 CRYPTO_E_QUEUE_FULL          Request failed, the queue is full.
 *                 CRYPTO_E_SMALL_BUFFER        Request failed, the provided buffer is too small to store the result.
 *                 CRYPTO_E_KEY_READ_FAIL       Request failed, read access was denied.
 *                 CRYPTO_E_KEY_WRITE_FAIL      Request failed, write access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE   Request failed, the key is not available.
 *                 CRYPTO_E_KEY_EMPTY           Request failed, uninitialized source key element.
 *                 CRYPTO_E_ENTROPY_EXHAUSTION  Request failed, the entropy is exhausted
 *                 CRYPTO_E_JOB_CANCELED        The service request failed because the synchronous Job has been canceled.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for jobs with different job ids
 *  \synchronous   TRUE|FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_ProcessJob(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_CancelJob()
 *********************************************************************************************************************/
/*! \brief         Cancels the received job
 *  \details       This interface removes the provided job from the queue and cancels the processing of the job if possible.
 *  \param[in]     objectId                Holds the identifier of the Crypto Driver Object.
 *  \param[in,out] job                     Pointer to the configuration of the job. Contains structures with user and
 *                                         primitive relevant information.
 *  \return        E_OK                    Request successful, job has been removed.
 *                 E_NOT_OK                Request failed, job could not be removed.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_CancelJob(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

# if(CRYPTO_30_LIBCV_SAVEANDRESTOREWORKSPACE == STD_ON)
/**********************************************************************************************************************
 *  Appl_Crypto_30_LibCv_SaveContextCallout()
 *********************************************************************************************************************/
/*! \brief         Saves the context to the output buffer.
 *  \details       The customer can choose whether the context data is to be written encrypted or in plain text
 *                 to the output buffer. The outputLengthPtr must be set to the written length.
 *                 The JobId and ObjectId can be used to check whether the data originates
 *                 from the correct job and object.
 *  \param[in]     objectId                Holds the identifier of the Crypto Driver Object.
 *  \param[in]     jobId                   Holds the identifier of the Job.
 *  \param[in]     context                 Holds the Address of the source context data.
 *  \param[in]     contextLength           Holds the length of the source context.
 *  \param[out]    outputPtr               Pointer to output buffer
 *  \param[in,out] outputLengthPtr         Pointer to output length buffer
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CRYPTO_30_LIBCV_SAVEANDRESTOREWORKSPACE == STD_ON
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_LIBCV_APPL_CODE) Appl_Crypto_30_LibCv_SaveContextCallout(
  uint32 objectId,
  uint32 jobId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) context,
  uint32 contextLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) outputPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) outputLengthPtr);

/**********************************************************************************************************************
 *  Appl_Crypto_30_LibCv_RestoreContextCallout()
 *********************************************************************************************************************/
/*! \brief         Restores the context from the output buffer.
 *  \details       The customer can choose to write the data encrypted or in plain text to the destination context address.
 *                 The data shall only be copied if the plain context matches the contextLength.
 *                 If the context data is encrypted, it must be decrypted first and then written to the workspace address.
 *                 The JobId and ObjectId can be used to check whether the data originates from the correct job and object.
 *  \param[in]     objectId                Holds the identifier of the Crypto Driver Object.
 *  \param[in]     jobId                   Holds the identifier of the Job.
 *  \param[out]    context                 Holds the Address of the destination context data.
 *  \param[in]     contextLength           Holds the length of the destination context.
 *  \param[in]     inputPtr                Pointer to source input buffer.
 *  \param[in]     inputLength             Holds the input buffer length.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CRYPTO_30_LIBCV_SAVEANDRESTOREWORKSPACE == STD_ON
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_LIBCV_APPL_CODE) Appl_Crypto_30_LibCv_RestoreContextCallout(
  uint32 objectId,
  uint32 jobId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) context,
  uint32 contextLength,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) inputPtr,
  uint32 inputLength);
# endif

/**********************************************************************************************************************
 *  Crypto_30_LibCv_MainFunction()
 *********************************************************************************************************************/
/*! \fn            FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_MainFunction(void)
 *  \brief         Main function of the module. Is called cyclically and handles asynchronous jobs.
 *  \details       This interface process the provided asynchronous jobs from the queue.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by SchM. Due to long runtimes, it is advisable to map function to a preemptive task.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * esl_getBytesRNG()
 *********************************************************************************************************************/
/*! \fn             VSECPRIM_FUNC(eslt_ErrorCode) esl_getBytesRNG( const eslt_Length targetLength, VSECPRIM_P2VAR_PARA(eslt_Byte) target);
 *  \brief          Get random byte
 *  \details        This function generates random numbers for primitives which requires random numbers for algorithm execution.
 *  \param[in]      targetLength    the number of bytes that shall be generated
 *  \param[in,out]  target          (in) pointer to output buffer
 *                                  (out) pointer to generated random bytes
 *  \return         ESL_ERC_ERROR input       generation failed
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         CRYPTO_30_LIBCV_DEFAULT_RANDOM_SOURCE == STD_ON
 *********************************************************************************************************************/

# define CRYPTO_30_LIBCV_STOP_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*!
 * \exclusivearea CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0
 * Ensures consistency of global RAM variables.
 * \protects DriverObjectState, NvBlock_State, Queue and Lock variables.
 * \usedin Crypto_30_LibCv_MainFunction, Crypto_30_LibCv_ProcessJob, Crypto_30_LibCv_CancelJob, Crypto_30_LibCv_KeyExchangeCalcPubVal, Crypto_30_LibCv_KeyExchangeCalcSecret, Crypto_30_LibCv_KeyCopy, Crypto_30_LibCv_KeyElementCopy, Crypto_30_LibCv_KeyElementSet, Crypto_30_LibCv_KeyValidSet, Crypto_30_LibCv_KeyElementGet, Crypto_30_LibCv_RandomSeed, Crypto_30_LibCv_KeyDerive, Crypto_30_LibCv_NvBlock_WriteToBlock, Crypto_30_LibCv_NvBlock_ReadFromBlock, Crypto_30_LibCv_NvBlock_Callback and Crypto_30_LibCv_NvBlock_Init.
 * \exclude -
 * \length SHORT in Crypto_30_LibCv_MainFunction, Crypto_30_LibCv_ProcessJob, Crypto_30_LibCv_CancelJob, Crypto_30_LibCv_KeyExchangeCalcPubVal, Crypto_30_LibCv_KeyExchangeCalcSecret, Crypto_30_LibCv_KeyCopy, Crypto_30_LibCv_KeyElementCopy, Crypto_30_LibCv_KeyElementSet, Crypto_30_LibCv_KeyValidSet, Crypto_30_LibCv_KeyElementGet, Crypto_30_LibCv_RandomSeed, Crypto_30_LibCv_KeyDerive, Crypto_30_LibCv_NvBlock_WriteToBlock, Crypto_30_LibCv_NvBlock_ReadFromBlock, Crypto_30_LibCv_NvBlock_Callback and Crypto_30_LibCv_NvBlock_Init.
 * \endexclusivearea

 * \exclusivearea CRYPTO_30_LIBCV_EXCLUSIVE_AREA_1
 * Ensures consistency of global RAM variables. Only necessary if 32-bit access of variables is non-atomic.
 * \protects Lock variables.
 * \usedin Crypto_30_LibCv_Process
 * \exclude -
 * \length SHORT as only one operation is secured.
 * \endexclusivearea
 */

#endif /* CRYPTO_30_LIBCV_H */
/**********************************************************************************************************************
 *  END OF FILE: CRYPTO_30_LIBCV.H
 *********************************************************************************************************************/
