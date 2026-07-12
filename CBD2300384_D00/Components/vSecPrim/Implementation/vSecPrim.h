 /**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is protected under intellectual property laws and proprietary to cv cryptovision GmbH
 *                and/or Vector Informatik GmbH.
 *                No right or license is granted save as expressly set out in the applicable license conditions.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  vSecPrim.h
 *        \brief  Header file containing current version, revision history and global function prototypes
 *
 *      \details  Contains the version, history and static code checker justifications
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2018-07-04   vismaw    STORYC-5521      Initial creation base on former modules SedMod/CryptoCv
 *  01.01.00  2018-08-16   visewl    ESCAN00100414    actMemcmp now performs comparisons in constant time
 *                                   ESCAN00100671    Set memory map sections in ESLib_AES_common.h
 *                                   STORYC-1927      SafeBSW Release of AES-GCM and SipHash
 *                                   STORYC-5720      MISRA-C:2012 Compliance
 *  02.00.00  2018-12-03   vismwe    STORYC-7254      Avoid usage of limit.h
 *                                   STORYC-7277      Support ChaCha20, Poly1305 and EC-Burmester-Desmedt
 *                                   STORYC-7162      Support  KDF X9.63 with SHA-2 256
 *                                   ESCAN00101592    Hash calculation is wrong for data >= 512MB for SHA2-64 (SHA384 and SHA512 variants)
 *                                   ESCAN00101643    x64 only: esl_initVerifyDSAEcP_prim returns always ESL_ERC_WS_TOO_SMALL (160bit)
 *  02.01.00  2019-01-21   vismwe    ESCAN00101811    Compiler error: actBNLENGTH is undefined
 *                         visivg    STORYC-7448      Supprt of HMAC-SHA-384
 *  02.02.00  2019-02-20   vismwe    STORYC-7656      Release of vSecPrim 2.x
 *                                   ESCAN00102231    RC2 encryption and decryption output is to short (4Byte instead of 8Byte).
 *  03.00.00  2019-11-11   viseag    SEC-831          Support PKCS5 / KDF2 with Sha-2 256
 *                                   ESCAN00104179    Check if input==NULL_PTR && inputLen != 0, avoid calling actSHAUpdate with msgLength = 0
 *  03.01.00  2019-12-16   viseag    SEC-789          Safe BSW for Big Number Calculation
 *                                   SEC-1063         Safe BSW for RSA PSS SHA256
 *                                   ESCAN00105755    Compiler warning: Several compiler warnings with gcc
 *                                   ESCAN00105844    V15 Decryption: message length limitation too strict
 *                                   SEC-879          Remove 64k single call limitation
 *  03.01.01  2020-03-25   vismwe    ESCAN00105935    Compiler error: actIRSAPrivateCRT.c identifier not declared
 *  04.00.00  2020-04-14   viseag    SEC-1159         SafeBSW for ECDSA and ECC Key Generation functions
 *                                   SEC-1230         Support CMAC calculation for input length zero
 *                                   SEC-794          Support AES in CTR mode
 *  04.00.01  2020-05-14   viseag    ESCAN00106368    Silent violation warning within workspace in RSA operation
 *  04.01.00  2020-05-19   viseag    SEC-1162         Support CTR_DRBG based on AES128
 *                         vismwe    SEC-1532         Support  KDF X9.63 with SHA-2 512
 *  04.02.00  2020-07-06   viseag    SEC-1221         Support Hash DRBG based on SHA2-512
 *                                   ESCAN00106850    Potential use of uninitialized stack variables during EC operations
 *                                   SEC-1474         Support SHA3 according to FIPS PUB 202
 *                                   ESCAN00106942    Possible access of uninitialized memory during RSA public
 *                                                    key operation if input data is incorrect
 *                                   SEC-1494         Support CTR DRBG based on AES256
 *                                   ESCAN00107053    Freedom from interference violation during RSA operation with
 *                                                    non-standard key sizes
 *  05.00.00  2020-08-19   viseag    SEC-1748         Support AES-CMAC 256 Beta
 *                                   SEC-1750         Support AES-CCM 128 Beta
 *                                   SEC-1752         Support HKDF HMAC SHA256 Two Step Beta
 *                                   SEC-1754         Support SPAKE2+ Beta
 *  05.01.00 2020-09-04    viseag    SEC-1803         Support AES-256-CMAC - Production
 *                                   SEC-1807         Support HKDF HMAC SHA256 - Production
 *                                   SEC-1810         Support AES 128 CCM - Production
 *                                   ESCAN00107426    Potential blockage of ECU during EC operation, leading to watchdog resets
 *  05.02.00 2020-10-21    viseag    SEC-1814         Support SPAKE2+ - Production
 *                                   ESCAN00107723    BETA version - the BSW module has a feature with BETA state (SPAKE2+)
 *                                   SEC-1816         Support in-place encryption/decryption for AES GCM
 *  05.03.00 2020-12-02    viseag    SEC-301          Use VStdLib for MemCpy, MemSet and similar services
 *                                   SEC-2075         Generic Hashing
 *                                   SEC-1137         Rework RSA workspaces to reduce memory usage
 *                                   ESCAN00108499    Watchdog is not triggered in ECC operations
 *                                   ESCAN00108507    Watchdog not triggered often enough in RSA PSS or RSA OAEP finalize step
 *  05.04.00 2021-02-19    viseag    SEC-1693         QAC for QM part of component
 *  05.05.00 2021-06-15    olapointe SEC-2489         QAC for QM part of component (continued)
 *                         viseag    ESCAN00109541    Possible memory corruption in ECU due to RSA V15 Decrypt operation with CRT
 *                                   ESCAN00109530    Memory corruption in ECU due to RSA PSS or RSA OAEP operation with small modulus sizes
 *                                   ESCAN00109492    Possible memory corruption in ECU due to RSA V15 Decrypt operation, no CRT
 *                                   ESCAN00109490    Missing length check in RSA V15 Decrypt operation
 *                                   ESCAN00109721    Possible memory corruption in ECU due to RSA V15 Encrypt operation with short module sizes
 *                                   ESCAN00109724    Possible access of uninitialized memory during RSA V15 signature verification operation with short modulus sizes
 *                                   ESCAN00109742    Possible memory corruption in ECU due to RSA V15 SHA256 operation with small modulus sizes
 * 06.00.00 2021-09-27     olapointe ESCAN00109923    Potential use of uninitialized stack variables during EC operations if EC domain encoding is incorrect
 *                         viseag    SEC-3458         Support HKDF SHA2-256 (One Step)
 * 06.01.00 2022-01-13     viseag    SEC-3148         Analysis of safety requirements for ECDSA (signature verification)
 *                                   SEC-3669         Usage of <Mip>_MemMap
 * 07.00.00 2022-12-15     viseag    SEC-5046         Support of HKDF HMAC SHA384
 *                                   ESCAN00113427    Missing information in API description of vSecPrim_initHash().
 **********************************************************************************************************************/

#ifndef VSECPRIM_H
# define VSECPRIM_H

# include "vSecPrim_types.h"
# include "ESLib.h"

/**********************************************************************************************************************
 *  VERSION
 *********************************************************************************************************************/
/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define VSECPRIM_SW_MAJOR_VERSION                                    (7u)
# define VSECPRIM_SW_MINOR_VERSION                                    (0u)
# define VSECPRIM_SW_PATCH_VERSION                                    (0u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# if (VSECPRIM_HASH_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vSecPrim_initHash()
 *********************************************************************************************************************/
/*! \brief          Initializes the hash calculation.
 *  \details        This function initializes the workspace header and the workspace itself by calling the hash
 *                  function init routine corresponding to the value of hashID.
 *  \param[in,out]  workSpace         (in)  algorithm context buffer
 *                                    (out) initialized algorithm context structure
 *  \param[in]      hashID            identifier of the hash function to be used
 *  \param[in]      outputLengthFlag  flag indicating whether
 *                                    - the provided output buffer in finalize will be exactly as long as the desired
 *                                      digest length and truncation of the digest is not allowed
 *                                    - the provided output buffer in finalize will possibly be longer than the desired
 *                                      digest length and truncation of the digest is not allowed
 *                                    - the provided output buffer in finalize will either be exactly as long as or
 *                                      longer than the desired digest length and truncation of the digest is allowed
 *  \return         ESL_ERC_PARAMETER_INVALID    input parameter is NULL
 *                  ESL_ERC_INPUT_INVALID        value of hashID or of outputLengthFlag invalid
 *                  ESL_ERC_WS_TOO_SMALL         workspace too small
 *                  ESL_ERC_NO_ERROR             otherwise
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..), in particular the watchdog function
 *                  pointer in the workspace header must be initialized.
 *  \note           Please refer to the API descriptions in ESLib.h for further information about the specific
 *                  hash initialization APIs. They can be found under esl_init[hash function name](..).
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) vSecPrim_initHash(
VSECPRIM_P2VAR_PARA(vSecPrimType_WorkSpaceHash) workSpace,
vSecPrimType_HashAlgorithm hashID,
vSecPrimType_OutputBufferFlag outputLengthFlag);

/**********************************************************************************************************************
 *  vSecPrim_updateHash()
 *********************************************************************************************************************/
/*! \brief          Updates the hash calculation with the given input data.
 *  \details        This function can be called several times until all data to be hashed has been passed. It calls
 *                  the hash function update routine specified by workSpace->hashID.
 *  \param[in,out]  workSpace    (in) hash context buffer
 *                               (out) updated hash context structure
 *  \param[in]      inputLength  length of input data in bytes
 *  \param[in]      input        pointer to data to be hashed
 *  \return         ESL_ERC_PARAMETER_INVALID           input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL                work space too small
 *                  ESL_ERC_WS_STATE_INVALID            work space state invalid
 *                  ESL_ERC_TOTAL_LENGTH_OVERFLOW       total input data too long
 *                  ESL_ERC_STATE_INVALID               update failed (e.g. SHA3 specific error occurred)
 *                  ESL_ERC_NO_ERROR                    otherwise
 *  \pre            workSpace has been initialized by vSecPrim_initHash (...)
 *  \note           Please refer to the API descriptions in ESLib.h for further information about the specific
 *                  hash update APIs. They can be found under esl_update[hash function name](..).
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) vSecPrim_updateHash(
  VSECPRIM_P2VAR_PARA(vSecPrimType_WorkSpaceHash) workSpace,
  const eslt_Length inputLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  vSecPrim_finalizeHash()
 *********************************************************************************************************************/
/*! \brief          Finalizes the hash calculation and returns the (possibly truncated) hash value.
 *  \details        The hash function is specified by the parameter workSpace->hashID.
 *                  If 'messageDigest' is not a null pointer, this function calls the hash function
 *                  finalize routine specified by workSpace->hashID and returns the hash value.
 *                  If 'messageDigest' is not a null pointer and if *messageDigestLength is shorter than the
 *                  algorithm specific digest size and if truncation is allowed, the truncated digest
 *                  is written to 'messageDigest'.
 *                  If 'messageDigest' is a null pointer, this function writes the number of required bytes to store
 *                  the (untruncated) digest to *messageDigestLength and does not compute the hash value. In this
 *                  case, the function can be called again.
 *  \param[in,out]  workSpace              (in)  hash context buffer
 *                                         (out) updated hash context structure
 *  \param[in,out]  messageDigestLength    (in)  pointer to buffer, storing the length of messageDigest
 *                                         (out) pointer to buffer, storing the length of the digest written to
 *                                               the buffer referenced by 'messageDigest'
 *  \param[in,out]  messageDigest          (in)  pointer to buffer to store the digest, possibly a null pointer
 *                                         (out) pointer to buffer storing the computed digest
 *  \return         ESL_ERC_PARAMETER_INVALID      input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL           work space too small
 *                  ESL_ERC_WS_STATE_INVALID       work space state invalid
 *                  ESL_ERC_INPUT_INVALID          *messageDigestLength invalid with respect to
 *                                                 workSpace->outputLengthFlag
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT  *messageDigestLength too small
 *                  ESL_ERC_NO_ERROR               otherwise
 *  \pre            workSpace has been initialized by vSecPrim_initHash (...)
 *                  If 'messageDigest' is not a null pointer, the buffer referenced by messageDigest must provide
 *                  at least as many bytes as referenced by messageDigestLength.
 *  \note           Please refer to the API descriptions in ESLib.h for further information about the specific
 *                  hash finalization APIs. They can be found under esl_finalize[hash function name](..).
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) vSecPrim_finalizeHash(
  VSECPRIM_P2VAR_PARA(vSecPrimType_WorkSpaceHash) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Length) messageDigestLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

/**********************************************************************************************************************
 *  vSecPrim_digestHash()
 *********************************************************************************************************************/
/*! \brief          Computes the hash value of input and writes it (possibly truncated) to messageDigest.
 *  \details        This API simplifies the single call hashing use case, so vSecPrim_updateHash() must not be
 *                  called before.
 *                  The hash function is specified by the parameter workSpace->hashID.
 *                  If 'messageDigest' is not a null pointer, this function calls the hash function update and
 *                  finalize routines specified by workSpace->hashID and returns the hash value.
 *                  If 'messageDigest' is not a null pointer and if *messageDigestLength is shorter than the
 *                  algorithm specific digest size and if truncation is allowed, the truncated digest
 *                  is written to 'messageDigest'.
 *                  If 'messageDigest' is a null pointer, this function writes the number of required bytes to store
 *                  the (untruncated) digest to *messageDigestLength and does not compute the hash value. In this
 *                  case, the function can be called again.
 *  \param[in,out]  workSpace              (in)  hash context buffer
 *                                         (out) updated hash context structure
 *  \param[in]      inputLength            length of input data in bytes
 *  \param[in]      input                  pointer to data to be hashed
 *  \param[in,out]  messageDigestLength    (in)  pointer to buffer, storing the length of messageDigest
 *                                         (out) pointer to buffer, storing the length of the digest written to
 *                                               the buffer referenced by 'messageDigest'
 *  \param[in,out]  messageDigest          (in)  pointer to buffer to store the digest, possibly a null pointer
 *                                         (out) pointer to buffer storing the computed digest
 *  \return         ESL_ERC_PARAMETER_INVALID      input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL           work space too small
 *                  ESL_ERC_WS_STATE_INVALID       work space state invalid
 *                  ESL_ERC_TOTAL_LENGTH_OVERFLOW  total input data too long
 *                  ESL_ERC_INPUT_INVALID          *messageDigestLength invalid with respect to
 *                                                 workSpace->outputLengthFlag
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT  *messageDigestLength too small with respect to
 *                                                 workSpace->outputLengthFlag
 *                  ESL_ERC_NO_ERROR               otherwise
 *  \pre            workSpace has been initialized by vSecPrim_initHash (...)
 *                  If 'messageDigest' is not a null pointer, the buffer referenced by messageDigest must provide
 *                  at least as many bytes as referenced by messageDigestLength.
 *  \note           Please refer to the API descriptions in ESLib.h for further information about the specific
 *                  hash update and finalization APIs. They can be found under esl_update[hash function name](..)
 *                  and esl_finalize[hash function name](..).
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) vSecPrim_digestHash(
  VSECPRIM_P2VAR_PARA(vSecPrimType_WorkSpaceHash) workSpace,
  const eslt_Length inputLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input,
  VSECPRIM_P2VAR_PARA(eslt_Length) messageDigestLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

# endif /* # (VSECPRIM_HASH_ENABLED == STD_ON) */

#endif /* VSECPRIM_H */

/**********************************************************************************************************************
 *  END OF FILE: vSecPrim.h
 *********************************************************************************************************************/
