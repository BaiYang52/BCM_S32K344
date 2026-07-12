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
/*!        \file  vSecPrim_Hash.c
 *        \brief  Wrapper for several hash functions.
 *
 *      \details Currently the ESLib version is used.
 *               This file is part of the embedded systems library cvActLib/ES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to module's header file.
 *********************************************************************************************************************/

#define VSECPRIM_HASH_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vSecPrim.h"

#if (VSECPRIM_HASH_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* table of digest sizes in byte of all provided ahsh algorithms */
VSECPRIM_ROM(VSECPRIM_LOCAL, eslt_Byte) vSecPrim_digestSizes[VSECPRIM_NUMBER_OF_HASH_FUNCTIONS] = { /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */
  /* digest size of SHA1:         20 bytes */  ESL_SIZEOF_SHA1_DIGEST,
  /* digest size of SHA224:       28 bytes */  ESL_SIZEOF_SHA224_DIGEST,
  /* digest size of SHA256:       32 bytes */  ESL_SIZEOF_SHA256_DIGEST,
  /* digest size of SHA384:       48 bytes */  ESL_SIZEOF_SHA384_DIGEST,
  /* digest size of SHA2-512:     64 bytes */  ESL_SIZEOF_SHA512_DIGEST,
  /* digest size of SHA2-512-224: 28 bytes */  ESL_SIZEOF_SHA512_224_DIGEST,
  /* digest size of SHA2-512-256: 32 bytes */  ESL_SIZEOF_SHA512_256_DIGEST,
  /* digest size of SHA3-224:     28 bytes */  ESL_SIZEOF_SHA3_224_DIGEST,
  /* digest size of SHA3-256:     32 bytes */  ESL_SIZEOF_SHA3_256_DIGEST,
  /* digest size of SHA3-384:     48 bytes */  ESL_SIZEOF_SHA3_384_DIGEST,
  /* digest size of SHA3-512:     64 bytes */  ESL_SIZEOF_SHA3_512_DIGEST,
  /* digest size of SHAKE128:     32 bytes */  ESL_SIZEOF_SHAKE128_DIGEST,
  /* digest size of SHAKE256:     64 bytes */  ESL_SIZEOF_SHAKE256_DIGEST,
  /* digest size of RIPEMD160:    20 bytes */  ESL_SIZEOF_RIPEMD160_DIGEST,
  /* digest size of MD5:          16 bytes */  ESL_SIZEOF_MD5_DIGEST
};

# define VSECPRIM_STOP_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  vSecPrim_switchHashIDInit()
 *********************************************************************************************************************/
/*! \brief          Depending on the value of 'hashID', this function calls the respective hash init function.
 *  \details        -
 *  \param[in,out]  workSpace  (in)  algorithm context buffer
 *                             (out) initialized algorithm context structure
 *  \param[in]      hashID     identifier of the hash function to be used
 *                  ESL_ERC_INPUT_INVALID        value of hashID invalid
 *                  ESL_ERC_WS_TOO_SMALL         workspace too small
 *                  ESL_ERC_NO_ERROR             otherwise
 *  \pre            workSpace has to be a valid pointer
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) vSecPrim_switchHashIDInit(
  VSECPRIM_P2VAR_PARA(vSecPrimType_WorkSpaceHash) workSpace,
  vSecPrimType_HashAlgorithm hashID);

/**********************************************************************************************************************
 *  vSecPrim_switchHashIDUpdate()
 *********************************************************************************************************************/
/*! \brief          Depending on the value of 'workSpace->hashID', this function calls the respective hash update function.
 *  \details        -
 *  \param[in,out]  workSpace    (in)  algorithm context buffer
 *                               (out) updated algorithm context structure
 *  \param[in]      inputLength  length of input data in bytes
 *  \param[in]      input        pointer to data to be hashed
 *                  ESL_ERC_INPUT_INVALID          value of hashID invalid
 *                  ESL_ERC_WS_TOO_SMALL           workspace too small
 *                  ESL_ERC_WS_STATE_INVALID       workspace state invalid
 *                  ESL_ERC_TOTAL_LENGTH_OVERFLOW  inputLength too high
 *                  ESL_ERC_STATE_INVALID          SHA3 KECCAK state invalid
 *                  ESL_ERC_ERROR                  dispatching failed
 *                  ESL_ERC_NO_ERROR               otherwise
 *  \pre            workSpace has to be a valid pointer
 *                  workSpace has been initialized by vSecPrim_initHash(...)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) vSecPrim_switchHashIDUpdate(
  VSECPRIM_P2VAR_PARA(vSecPrimType_WorkSpaceHash) workSpace,
  eslt_Length inputLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  vSecPrim_switchHashIDFinalize()
 *********************************************************************************************************************/
/*! \brief          Depending on the value of 'workSpace->hashID', this function calls the respective
 *                  hash finalize function.
 *  \details        This function calls vSecPrim_finalizeHashWithLengthHandling() and/or vSecPrim_finalizeHashLengthCheck(),
 *                  see corresponding documentation.
 *                  If messageDigest is a null pointer, this function writes the required number of bytes to store
 *                  the full digest to *messageDigestLength and returns.
 *                  If the values of workSpace->outputLengthFlag and of *messageDigestLength are inconsistent,
 *                  the function returns an error code, see vSecPrim_finalizeHashLengthCheck().
 *  \param[in,out]  workSpace             (in)  algorithm context buffer
 *                                        (out) updated algorithm context structure
 *  \param[in,out]  messageDigestLength   (in)  pointer to buffer, storing the length of messageDigest
 *                                        (out) pointer to buffer, storing the length of the digest written to
 *                                              the buffer referenced by 'messageDigest'
 *  \param[in,out]  messageDigest         (in)  pointer to buffer to store the digest, possibly a null pointer
 *                                        (out) pointer to buffer storing the computed digest
 *  \return         ESL_ERC_INPUT_INVALID          value of hashID invalid
 *                  ESL_ERC_WS_TOO_SMALL           workspace too small
 *                  ESL_ERC_WS_STATE_INVALID       workspace state invalid
 *                  ESL_ERC_INPUT_INVALID          *messageDigestLength invalid with respect to
 *                                                 workSpace->outputLengthFlag
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT  *messageDigestLength too small with respect to
 *                                                 workSpace->outputLengthFlag
 *                  ESL_ERC_NO_ERROR               otherwise
 *  \pre            workSpace and messageDigestLength have to be valid pointers.
 *                  workSpace has been initialized by vSecPrim_initHash(...).
 *                  If 'messageDigest' is not a null pointer, the buffer referenced by messageDigest must provide
 *                  at least as many bytes as referenced by messageDigestLength.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) vSecPrim_switchHashIDFinalize(
  VSECPRIM_P2VAR_PARA(vSecPrimType_WorkSpaceHash) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Length) messageDigestLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

/**********************************************************************************************************************
 *  vSecPrim_finalizeHashWithLengthHandling()
 *********************************************************************************************************************/
/*! \brief          Finalizes the distinguished hash computation based on workSpace->hashID.
 *  \details        Depending on the value of 'workSpace->hashID', this function calls
                    vSecPrim_finalizeHashLengthCheck () and the respective  hash finalize function.
 *  \param[in,out]  workSpace             (in)  algorithm context buffer
 *                                        (out) updated algorithm context structure
 *  \param[in,out]  messageDigestLength   (in)  pointer to buffer, storing the length of messageDigest
 *                                        (out) pointer to buffer, storing the length of the digest written to
 *                                              the buffer referenced by 'messageDigest'
 *  \param[in,out]  messageDigest         (in)  pointer to buffer to store the digest, possibly a null pointer
 *                                        (out) pointer to buffer storing the computed digest
 *  \param[in]      maxDigestLength       hash function specific maximal digest length
 *  \return         ESL_ERC_INPUT_INVALID          value of hashID invalid
 *                  ESL_ERC_WS_TOO_SMALL           workspace too small
 *                  ESL_ERC_WS_STATE_INVALID       workspace state invalid
 *                  ESL_ERC_INPUT_INVALID          *messageDigestLength invalid with respect to
 *                                                 workSpace->outputLengthFlag
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT  *messageDigestLength too small with respect to
 *                                                 workSpace->outputLengthFlag
 *                  ESL_ERC_NO_ERROR               otherwise
 *  \pre            workSpace and messageDigestLength have to be valid pointers.
 *                  workSpace has been initialized by vSecPrim_initHash(...).
 *                  If 'messageDigest' is not a null pointer, the buffer referenced by messageDigest must provide
 *                  at least as many bytes as referenced by messageDigestLength.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) vSecPrim_finalizeHashWithLengthHandling(
  VSECPRIM_P2VAR_PARA(vSecPrimType_WorkSpaceHash) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Length) messageDigestLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest,
  const eslt_Length maxDigestLength);

/**********************************************************************************************************************
 *  vSecPrim_updateHashMapReturnValue()
 *********************************************************************************************************************/
/*! \brief          Maps different hash specific length overflow error codes to the general error code
 *                  ESL_ERC_TOTAL_LENGTH_OVERFLOW.
 *  \details        -
 *  \param[in,out]  retVal           (in)  pointer to current return value of hash update function
 *                                   (out) pointer to (possibly updated) return value
 *  \param[in]      referenceRetVal  hash specific length overflow error code
 *  \pre            retVal has to be a valid pointer
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) vSecPrim_updateHashMapReturnValue(
  VSECPRIM_P2VAR_PARA(eslt_ErrorCode) retVal,
  eslt_ErrorCode referenceRetVal);

/**********************************************************************************************************************
 *  vSecPrim_finalizeHashLengthCheck()
 *********************************************************************************************************************/
/*! \brief          Preliminary length check before finalizing the hash calculation.
 *  \details        If 'messageDigest' is a null pointer, this function writes the number of required bytes to store
 *                  the (untruncated) digest to *messageDigestLength and returns.
 *                  If 'messageDigest' is not a null pointer, this function checks the plausibility of
 *                  *messageDigestLength:
 *                  - If workSpace->outputLengthFlag == VSECPRIM_PROVIDED_BUFFER_LENGTH_MATCHES_DESIRED_OUTPUT_LENGTH
 *                    and *messageDigestLength is incorrect, the function returns an error.
 *                  - If workSpace->outputLengthFlag == VSECPRIM_PROVIDED_BUFFER_LENGTH_MATCHES_OR_EXCEEDS_DESIRED_OUTPUT_LENGTH
 *                    and *messageDigestLength is too little, the function returns an error.
 *  \param[in]      workSpace              hash context buffer
 *  \param[in,out]  messageDigestLength    (in)  pointer to buffer, storing the length of messageDigest
 *                                         (out) pointer to buffer, storing the maximal digest length,
 *                                               if messageDigest is a null pointer
 *  \param[in]      maxDigestLength        hash function specific maximal digest length
 *  \param[in,out]  truncateDigest         (in)  pointer to boolean value to store information whether the digest
 *                                               shall be truncated or not
 *                                         (out) pointer to boolean value storing information whether the digest
                                                 shall be truncated or not
 *  \return         ESL_ERC_PARAMETER_INVALID      outputLength is a null pointer
 *                  ESL_ERC_INPUT_INVALID          *messageDigestLength invalid with respect to
 *                                                 workSpace->outputLengthFlag
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT  *messageDigestLength too small with respect to
 *                                                 workSpace->outputLengthFlag
 *                  ESL_ERC_NO_ERROR               otherwise
 *  \pre            workSpace, messageDigestLength, outputLength and truncateDigest need to be valid pointers.
 *                  workSpace has been initialized by vSecPrim_initHash(...).
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) vSecPrim_finalizeHashLengthCheck(
  VSECPRIM_P2CONST_PARA(vSecPrimType_WorkSpaceHash) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Length) messageDigestLength,
  const eslt_Length maxDigestLength,
  VSECPRIM_P2VAR_PARA(boolean) truncateDigest);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  vSecPrim_switchHashIDInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) vSecPrim_switchHashIDInit(
  VSECPRIM_P2VAR_PARA(vSecPrimType_WorkSpaceHash) workSpace,
  vSecPrimType_HashAlgorithm hashID)
{
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;

  /* Switch hashID: initialize workspace for specific hash function and call specific init routine. */
  switch (hashID)
  {
# if (VSECPRIM_SHA1_ENABLED == STD_ON)
  case VSECPRIM_DIGEST_SHA1:
    /* esl_initWorkSpaceHeader only returns an error code, if the workspace header is a null pointer. This is not
     * the case, so it is safe to ignore the return value. */
    (void)esl_initWorkSpaceHeader(&(workSpace->wsHash.wsSHA1.header), ESL_MAXSIZEOF_WS_SHA1, (workSpace->header.watchdog)); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* esl_initSHA1 only returns an error code, if the workspace is a null pointer of if the workspace size is not
     * sufficient. This is not the case, so it is safe to ignore the return value. */
    (void)esl_initSHA1(&(workSpace->wsHash.wsSHA1)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;
# endif

# if (VSECPRIM_SHA2_256_ENABLED == STD_ON)
  case VSECPRIM_DIGEST_SHA2_224:
    /* esl_initWorkSpaceHeader only returns an error code, if the workspace header is a null pointer. This is not
     * the case, so it is safe to ignore the return value. */
    (void)esl_initWorkSpaceHeader(&(workSpace->wsHash.wsSHA2_224.header), ESL_MAXSIZEOF_WS_SHA224, (workSpace->header.watchdog)); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* esl_initSHA224 only returns an error code, if the workspace is a null pointer of if the workspace size is not
     * sufficient. This is not the case, so it is safe to ignore the return value. */
    (void)esl_initSHA224(&(workSpace->wsHash.wsSHA2_224)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;

  case VSECPRIM_DIGEST_SHA2_256:
    /* esl_initWorkSpaceHeader only returns an error code, if the workspace header is a null pointer. This is not
     * the case, so it is safe to ignore the return value. */
    (void)esl_initWorkSpaceHeader(&(workSpace->wsHash.wsSHA2_256.header), ESL_MAXSIZEOF_WS_SHA256, (workSpace->header.watchdog)); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* esl_initSHA256 only returns an error code, if the workspace is a null pointer of if the workspace size is not
     * sufficient. This is not the case, so it is safe to ignore the return value. */
    (void)esl_initSHA256(&(workSpace->wsHash.wsSHA2_256)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;
# endif

# if (VSECPRIM_SHA2_512_ENABLED == STD_ON)
  case VSECPRIM_DIGEST_SHA2_384:
    /* esl_initWorkSpaceHeader only returns an error code, if the workspace header is a null pointer. This is not
     * the case, so it is safe to ignore the return value. */
    (void)esl_initWorkSpaceHeader(&(workSpace->wsHash.wsSHA2_384.header), ESL_MAXSIZEOF_WS_SHA384, (workSpace->header.watchdog)); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* esl_initSHA384 only returns an error code, if the workspace is a null pointer of if the workspace size is not
     * sufficient. This is not the case, so it is safe to ignore the return value. */
    (void)esl_initSHA384(&(workSpace->wsHash.wsSHA2_384)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;

  case VSECPRIM_DIGEST_SHA2_512:
    /* esl_initWorkSpaceHeader only returns an error code, if the workspace header is a null pointer. This is not
     * the case, so it is safe to ignore the return value. */
    (void)esl_initWorkSpaceHeader(&(workSpace->wsHash.wsSHA2_512.header), ESL_MAXSIZEOF_WS_SHA512, (workSpace->header.watchdog)); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* esl_initSHA512 only returns an error code, if the workspace is a null pointer of if the workspace size is not
     * sufficient. This is not the case, so it is safe to ignore the return value. */
    (void)esl_initSHA512(&(workSpace->wsHash.wsSHA2_512)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;

  case VSECPRIM_DIGEST_SHA2_512_224:
    /* esl_initWorkSpaceHeader only returns an error code, if the workspace header is a null pointer. This is not
       * the case, so it is safe to ignore the return value. */
    (void)esl_initWorkSpaceHeader(&(workSpace->wsHash.wsSHA2_512_224.header), ESL_MAXSIZEOF_WS_SHA512_224, (workSpace->header.watchdog)); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* esl_initSHA512_224 only returns an error code, if the workspace is a null pointer of if the workspace size is
     * not sufficient. This is not the case, so it is safe to ignore the return value. */
    (void)esl_initSHA512_224(&(workSpace->wsHash.wsSHA2_512_224)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;

  case VSECPRIM_DIGEST_SHA2_512_256:
    /* esl_initWorkSpaceHeader only returns an error code, if the workspace header is a null pointer. This is not
       * the case, so it is safe to ignore the return value. */
    (void)esl_initWorkSpaceHeader(&(workSpace->wsHash.wsSHA2_512_256.header), ESL_MAXSIZEOF_WS_SHA512_256, (workSpace->header.watchdog)); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* esl_initSHA512_256 only returns an error code, if the workspace is a null pointer of if the workspace size is
     * not sufficient. This is not the case, so it is safe to ignore the return value. */
    (void)esl_initSHA512_256(&(workSpace->wsHash.wsSHA2_512_256)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;
# endif

# if (VSECPRIM_SHA3_ENABLED == STD_ON)
  case VSECPRIM_DIGEST_SHA3_224:
  case VSECPRIM_DIGEST_SHA3_256:
  case VSECPRIM_DIGEST_SHA3_384:
  case VSECPRIM_DIGEST_SHA3_512:
    /* esl_initWorkSpaceHeader only returns an error code, if the workspace header is a null pointer. This is not
     * the case, so it is safe to ignore the return value. */
    (void)esl_initWorkSpaceHeader(&(workSpace->wsHash.wsSHA3.header), ESL_MAXSIZEOF_WS_SHA3, (workSpace->header.watchdog)); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* esl_initSHA3 only returns an error code, if the workspace is a null pointer of if the workspace size is
     * not sufficient. This is not the case, so it is safe to ignore the return value. */
    (void)esl_initSHA3(&(workSpace->wsHash.wsSHA3), (Esl_BytesToBits((eslt_Length)vSecPrim_digestSizes[hashID]))); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;

  case VSECPRIM_DIGEST_SHAKE_128:
  case VSECPRIM_DIGEST_SHAKE_256:
    /* esl_initWorkSpaceHeader only returns an error code, if the workspace header is a null pointer. This is not
     * the case, so it is safe to ignore the return value.
     * Note: The SHAKE algorithm has to be initiated with a strength value,
     * where strength = (digest length in bit) / 2. */
    (void)esl_initWorkSpaceHeader(&(workSpace->wsHash.wsSHAKE.header), ESL_MAXSIZEOF_WS_SHAKE, (workSpace->header.watchdog)); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* esl_initSHAKE only returns an error code, if the workspace is a null pointer of if the workspace size is
     * not sufficient. This is not the case, so it is safe to ignore the return value. */
    (void)esl_initSHAKE(&(workSpace->wsHash.wsSHAKE), (Esl_BytesToBits(Esl_DivideByTwo((eslt_Length)(vSecPrim_digestSizes[hashID]))))); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;
# endif

# if (VSECPRIM_RIPEMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  case VSECPRIM_DIGEST_RIPEMD160:
    /* esl_initWorkSpaceHeader only returns an error code, if the workspace header is a null pointer. This is not
     * the case, so it is safe to ignore the return value. */
    (void)esl_initWorkSpaceHeader(&(workSpace->wsHash.wsRIPEMD160.header), ESL_MAXSIZEOF_WS_RIPEMD160, (workSpace->header.watchdog)); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* esl_initRIPEMD160 only returns an error code, if the workspace is a null pointer of if the workspace size is
     * not sufficient. This is not the case, so it is safe to ignore the return value. */
    (void)esl_initRIPEMD160(&(workSpace->wsHash.wsRIPEMD160)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;
# endif

# if (VSECPRIM_MD5_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  case VSECPRIM_DIGEST_MD5:
    /* esl_initWorkSpaceHeader only returns an error code, if the workspace header is a null pointer. This is not
     * the case, so it is safe to ignore the return value. */
    (void)esl_initWorkSpaceHeader(&(workSpace->wsHash.wsMD5.header), ESL_MAXSIZEOF_WS_MD5, (workSpace->header.watchdog)); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* esl_initMD5 only returns an error code, if the workspace is a null pointer of if the workspace size is
     * not sufficient. This is not the case, so it is safe to ignore the return value. */
    (void)esl_initMD5(&(workSpace->wsHash.wsMD5)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;
# endif

  default:
    retVal = ESL_ERC_INPUT_INVALID;
    break;
  }

  return retVal;
} /* PRQA S 6030, 6060, 6050 */ /* MD_VSECPRIM_QM_ONLY_METRIC, MD_VSECPRIM_STPAR, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  vSecPrim_switchHashIDUpdate()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) vSecPrim_switchHashIDUpdate(
  VSECPRIM_P2VAR_PARA(vSecPrimType_WorkSpaceHash) workSpace,
  eslt_Length inputLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input)
{
  eslt_ErrorCode retVal;

  /* Switch hashID: Call specific hash update routine, possibly remap error value in case of length overflow */
  switch (workSpace->hashID)
  {
# if (VSECPRIM_SHA1_ENABLED == STD_ON)
  case VSECPRIM_DIGEST_SHA1:
    retVal = esl_updateSHA1(&(workSpace->wsHash.wsSHA1), inputLength, input); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */
    vSecPrim_updateHashMapReturnValue(&retVal, ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */
    break;
# endif

# if (VSECPRIM_SHA2_256_ENABLED == STD_ON)
  case VSECPRIM_DIGEST_SHA2_224:
    retVal = esl_updateSHA224(&(workSpace->wsHash.wsSHA2_224), inputLength, input); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */
    vSecPrim_updateHashMapReturnValue(&retVal, ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */
    break;

  case VSECPRIM_DIGEST_SHA2_256:
    retVal = esl_updateSHA256(&(workSpace->wsHash.wsSHA2_256), inputLength, input); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */
    vSecPrim_updateHashMapReturnValue(&retVal, ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */
    break;
# endif

# if (VSECPRIM_SHA2_512_ENABLED == STD_ON)
  case VSECPRIM_DIGEST_SHA2_384:
    retVal = esl_updateSHA384(&(workSpace->wsHash.wsSHA2_384), inputLength, input); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */
    vSecPrim_updateHashMapReturnValue(&retVal, ESL_ERC_SHA384_TOTAL_LENGTH_OVERFLOW); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */
    break;

  case VSECPRIM_DIGEST_SHA2_512:
    retVal = esl_updateSHA512(&(workSpace->wsHash.wsSHA2_512), inputLength, input); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */
    break;

  case VSECPRIM_DIGEST_SHA2_512_224:
    retVal = esl_updateSHA512_224(&(workSpace->wsHash.wsSHA2_512_224), inputLength, input); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */
    break;

  case VSECPRIM_DIGEST_SHA2_512_256:
    retVal = esl_updateSHA512_256(&(workSpace->wsHash.wsSHA2_512_256), inputLength, input); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */
    break;
# endif

# if (VSECPRIM_SHA3_ENABLED == STD_ON)
  case VSECPRIM_DIGEST_SHA3_224:
  case VSECPRIM_DIGEST_SHA3_256:
  case VSECPRIM_DIGEST_SHA3_384:
  case VSECPRIM_DIGEST_SHA3_512:
    retVal = esl_updateSHA3(&(workSpace->wsHash.wsSHA3), inputLength, input); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */
    break;

  case VSECPRIM_DIGEST_SHAKE_128:
  case VSECPRIM_DIGEST_SHAKE_256:

    retVal = esl_updateSHAKE(&(workSpace->wsHash.wsSHAKE), inputLength, input); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */
    break;
# endif

# if (VSECPRIM_RIPEMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  case VSECPRIM_DIGEST_RIPEMD160:
    retVal = esl_updateRIPEMD160(&(workSpace->wsHash.wsRIPEMD160), inputLength, input); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */
    vSecPrim_updateHashMapReturnValue(&retVal, ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */
    break;
# endif

# if (VSECPRIM_MD5_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  case VSECPRIM_DIGEST_MD5:
    retVal = esl_updateMD5(&(workSpace->wsHash.wsMD5), inputLength, input); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */
    break;
# endif

  default:
    retVal = ESL_ERC_INPUT_INVALID;
    break;

  }

  return retVal;
} /* PRQA S 6030, 6060, 6050 */ /* MD_VSECPRIM_QM_ONLY_METRIC, MD_VSECPRIM_STPAR, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  vSecPrim_switchHashIDFinalize()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) vSecPrim_switchHashIDFinalize(
  VSECPRIM_P2VAR_PARA(vSecPrimType_WorkSpaceHash) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Length) messageDigestLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest)
{
  eslt_ErrorCode retVal;
  boolean truncateDigest = FALSE;

  /* If messageDigest is a null pointer, write the full digest length to *messageDigestLength and return.
   * In this case, no hash value is computed. */
  if (messageDigest == NULL_PTR)
  {
    *messageDigestLength = vSecPrim_digestSizes[workSpace->hashID]; /* SBSW_VSECPRIM_WRITE_POINTER */
    retVal = ESL_ERC_NO_ERROR;
  }
  else
  {
    /* If workspace->hashID belongs to SHA3/SHAKE algorithm family:
     * - Call vSecPrim_finalizeHashLengthCheck()
     * - Call finalize hash function in case no error was returned before
     * Note: The finalize routines of SHA3 and SHAKE truncate the digest, if desired. */

    switch (workSpace->hashID) /* PRQA S 3315 */ /* MD_VSECPRIM_CONFIGURATION_DEPENDENT_REDUNDANT_OP */
    {
# if (VSECPRIM_SHA3_ENABLED == STD_ON)
    case VSECPRIM_DIGEST_SHA3_224:
    case VSECPRIM_DIGEST_SHA3_256:
    case VSECPRIM_DIGEST_SHA3_384:
    case VSECPRIM_DIGEST_SHA3_512:

      retVal = vSecPrim_finalizeHashLengthCheck(workSpace, messageDigestLength, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        vSecPrim_digestSizes[workSpace->hashID], &truncateDigest);

      if (retVal == ESL_ERC_NO_ERROR)
      {
        /* Finding the minimum of *messageDigestLength and the maximal digest length is done
         * internally in esl_finalizeSHA3.
         * Also, the *messageDigestLength is updated there. */
        retVal = esl_finalizeSHA3(&(workSpace->wsHash.wsSHA3), messageDigest, messageDigestLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }

      break;

    case VSECPRIM_DIGEST_SHAKE_128:
    case VSECPRIM_DIGEST_SHAKE_256:

      retVal = vSecPrim_finalizeHashLengthCheck(workSpace, messageDigestLength, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        vSecPrim_digestSizes[workSpace->hashID], &truncateDigest);

      if (retVal == ESL_ERC_NO_ERROR)
      {
        /* Finding the minimum of *messageDigestLength and the maximal digest length is done
         * internally in esl_finalizeSHAKE.
         * Also, the *messageDigestLength is updated there. */
        retVal = esl_finalizeSHAKE(&(workSpace->wsHash.wsSHAKE), messageDigest, messageDigestLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }

      break;
# endif
    default:
      /* If workspace->hashID does not belong to SHA3 / SHAKE algorithm family:
       * call vSecPrim_finalizeHashWithLengthHandling() */

      retVal = vSecPrim_finalizeHashWithLengthHandling(workSpace, messageDigestLength, messageDigest, vSecPrim_digestSizes[vSecPrimMin(workSpace->hashID, (VSECPRIM_NUMBER_OF_HASH_FUNCTIONS - 1u))]); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      VSECPRIM_DUMMY_STATEMENT(truncateDigest); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e438 */
      break;
    }
  }

  return retVal;

} /* PRQA S 6030 */ /* MD_MSR_STCYC */

/**********************************************************************************************************************
 *  vSecPrim_finalizeHashWithLengthHandling()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) vSecPrim_finalizeHashWithLengthHandling(
  VSECPRIM_P2VAR_PARA(vSecPrimType_WorkSpaceHash) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Length) messageDigestLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest,
  const eslt_Length maxDigestLength)
{
  eslt_ErrorCode retVal;
  boolean truncateDigest;
  VSECPRIM_P2VAR_PARA(eslt_Byte) outputBuffer;

  /* Length check and possible update of *messageDigestLength */
  retVal = vSecPrim_finalizeHashLengthCheck(workSpace, messageDigestLength, maxDigestLength, &truncateDigest); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */

  /* If the digest shall be truncated, it needs to be written to a temporary buffer first instead of directly
   * being written of messageDigest. In this case, the truncated digest will be copied to messageDigest later. */
  if (truncateDigest == TRUE)
  {
    outputBuffer = workSpace->tempHash;
  }
  else
  {
    outputBuffer = messageDigest;
  }

  if (retVal == ESL_ERC_NO_ERROR)
  {
    /* Switch hashID: Call specific hash finalize routine. */
    switch (workSpace->hashID)
    {

# if (VSECPRIM_SHA1_ENABLED == STD_ON)
    case VSECPRIM_DIGEST_SHA1:

      retVal = esl_finalizeSHA1(&(workSpace->wsHash.wsSHA1), outputBuffer); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif

# if (VSECPRIM_SHA2_256_ENABLED == STD_ON)
    case VSECPRIM_DIGEST_SHA2_224:
      retVal = esl_finalizeSHA224(&(workSpace->wsHash.wsSHA2_224), outputBuffer); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;

    case VSECPRIM_DIGEST_SHA2_256:
      retVal = esl_finalizeSHA256(&(workSpace->wsHash.wsSHA2_256), outputBuffer); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif

# if (VSECPRIM_SHA2_512_ENABLED == STD_ON)
    case VSECPRIM_DIGEST_SHA2_384:
      retVal = esl_finalizeSHA384(&(workSpace->wsHash.wsSHA2_384), outputBuffer); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;

    case VSECPRIM_DIGEST_SHA2_512:
      retVal = esl_finalizeSHA512(&(workSpace->wsHash.wsSHA2_512), outputBuffer); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;

    case VSECPRIM_DIGEST_SHA2_512_224:
      retVal = esl_finalizeSHA512_224(&(workSpace->wsHash.wsSHA2_512_224), outputBuffer); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;

    case VSECPRIM_DIGEST_SHA2_512_256:
      retVal = esl_finalizeSHA512_256(&(workSpace->wsHash.wsSHA2_512_256), outputBuffer); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif

# if (VSECPRIM_RIPEMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case VSECPRIM_DIGEST_RIPEMD160:
      retVal = esl_finalizeRIPEMD160(&(workSpace->wsHash.wsRIPEMD160), outputBuffer); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif

# if (VSECPRIM_MD5_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case VSECPRIM_DIGEST_MD5:
      retVal = esl_finalizeMD5(&(workSpace->wsHash.wsMD5), outputBuffer); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif

    default:
      retVal = ESL_ERC_INPUT_INVALID;
      break;
    }

    /* Copy truncated digest, if requried. */
    if ((retVal == ESL_ERC_NO_ERROR) && (truncateDigest == TRUE))
    {
      Esl_MemCpyByteArray(messageDigest, outputBuffer, *messageDigestLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */
    }
  }

  return retVal;
} /* PRQA S 6050, 6030 */ /* MD_MSR_STCAL, MD_MSR_STCYC */

/**********************************************************************************************************************
 *  vSecPrim_updateHashMapReturnValue()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) vSecPrim_updateHashMapReturnValue(
  VSECPRIM_P2VAR_PARA(eslt_ErrorCode) retVal,
  eslt_ErrorCode referenceRetVal)
{
  if (*retVal == referenceRetVal)
  {
    *retVal = ESL_ERC_TOTAL_LENGTH_OVERFLOW; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
  }
}

/**********************************************************************************************************************
 *  vSecPrim_finalizeHashLengthCheck()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) vSecPrim_finalizeHashLengthCheck(
  VSECPRIM_P2CONST_PARA(vSecPrimType_WorkSpaceHash) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Length) messageDigestLength,
  const eslt_Length maxDigestLength,
  VSECPRIM_P2VAR_PARA(boolean) truncateDigest)
{
  eslt_ErrorCode retVal;
  *truncateDigest = FALSE; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */

  /* Check if *messageDigestLength matches maxDigestLength, if the outputLengthFlag indicates,
   * that the provided output buffer length shall match the maximal digest length exactly. */
  if ((workSpace->outputLengthFlag == VSECPRIM_PROVIDED_BUFFER_LENGTH_MATCHES_DESIRED_OUTPUT_LENGTH) &&
    (*messageDigestLength != maxDigestLength))
  {
    retVal = ESL_ERC_INPUT_INVALID;
  }
  /* Check if *messageDigestLength matches or exceeds maxDigestLength, if the outputLengthFlag indicates,
   * that the provided output buffer length may exceed the maximal digest length exactly. */
  else if ((workSpace->outputLengthFlag == VSECPRIM_PROVIDED_BUFFER_LENGTH_MATCHES_OR_EXCEEDS_DESIRED_OUTPUT_LENGTH) &&
    (*messageDigestLength < maxDigestLength))
  {
    retVal = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
  }
  else
  {
    /* If *messageDigestLength is valid, update the value if necessary:
     * - If workSpace->outputLengthFlag == VSECPRIM_PROVIDED_BUFFER_LENGTH_MATCHES_DESIRED_OUTPUT_LENGTH,
     *   *messageDigestLength stays unchanged, because its value was checked before already.
     *   *truncateDigest remains FALSE.
     * - If workSpace->outputLengthFlag == VSECPRIM_PROVIDED_BUFFER_LENGTH_MATCHES_OR_EXCEEDS_DESIRED_OUTPUT_LENGTH,
     *   *messageDigestLength might be greater than maxDigestLength and thus might be updated to match maxDigestLength.
     *   *truncateDigest remains FALSE.
     * - If *messageDigestLength < maxDigestLength, its value remains unchanged, and *truncateDigest is set to TRUE.
     *   In this case, it automatically holds that workSpace->outputLengthFlag == VSECPRIM_OUTPUT_TRUNCATION_ALLOWED. */

    *messageDigestLength = vSecPrimMin(*messageDigestLength, maxDigestLength); /* SBSW_VSECPRIM_WRITE_POINTER */

    if (*messageDigestLength < maxDigestLength)
    {
      *truncateDigest = TRUE; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    }

    retVal = ESL_ERC_NO_ERROR;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  vSecPrim_initHash()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) vSecPrim_initHash(
  VSECPRIM_P2VAR_PARA(vSecPrimType_WorkSpaceHash) workSpace,
  vSecPrimType_HashAlgorithm hashID,
  vSecPrimType_OutputBufferFlag outputLengthFlag)
{
  eslt_ErrorCode retVal;

  /* input parameter plausibility check */
  if (workSpace == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, VSECPRIM_SIZEOF_WS_HASH)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal already set */
  }
  else if ((outputLengthFlag != VSECPRIM_PROVIDED_BUFFER_LENGTH_MATCHES_DESIRED_OUTPUT_LENGTH) &&
    (outputLengthFlag != VSECPRIM_PROVIDED_BUFFER_LENGTH_MATCHES_OR_EXCEEDS_DESIRED_OUTPUT_LENGTH) &&
    (outputLengthFlag != VSECPRIM_OUTPUT_TRUNCATION_ALLOWED))
  {
    retVal = ESL_ERC_INPUT_INVALID;
  }
  else
  {
    /* Call vSecPrim_switchHashIDInit() to initialize the workspace for
     * a specific hash function and to call the specific hash init routine. */
    retVal = vSecPrim_switchHashIDInit(workSpace, hashID); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (retVal == ESL_ERC_NO_ERROR)
    {
      /* Save information in workspace. */
      workSpace->hashID = hashID; /* SBSW_VSECPRIM_WRITE_POINTER */
      workSpace->outputLengthFlag = outputLengthFlag; /* SBSW_VSECPRIM_WRITE_POINTER */

      /* Store workspace state and size; storing the size is required when clearing the workspace
       * during the hash finalization. */
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_HASH); /* SBSW_VSECPRIM_CALL_FUNCTION */
      workSpace->updateCalled = VSECPRIM_UPDATE_NOT_YET_CALLED; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  vSecPrim_updateHash()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) vSecPrim_updateHash(
  VSECPRIM_P2VAR_PARA(vSecPrimType_WorkSpaceHash) workSpace,
  const eslt_Length inputLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input)
{
  eslt_ErrorCode retVal;

  /* null pointer check */
  if ((workSpace == NULL_PTR) || (input == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceState(&workSpace->header, ESL_WST_ALGO_HASH)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    retVal = vSecPrim_switchHashIDUpdate(workSpace, inputLength, input); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */
    /* Update the workspace state.
     * This is used to ensure, that vSecPrim_digestHash() is not called after vSecPrim_updateHash() was called. */
    workSpace->updateCalled = VSECPRIM_UPDATE_CALLED; /* SBSW_VSECPRIM_WRITE_POINTER */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  vSecPrim_finalizeHash()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) vSecPrim_finalizeHash(
  VSECPRIM_P2VAR_PARA(vSecPrimType_WorkSpaceHash) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Length) messageDigestLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest)
{
  eslt_ErrorCode retVal;

  /* null pointer check */
  if ((workSpace == NULL_PTR) || (messageDigestLength == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = (esl_CheckWorkSpaceState(&workSpace->header, ESL_WST_ALGO_HASH))) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set
     * Note: vSecPrim_updateHash() can either be called before or not, both cases are allowed. */
  }
  else
  {
    retVal = vSecPrim_switchHashIDFinalize(workSpace, messageDigestLength, messageDigest); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* If messageDigest is a null pointer, the previous function only wrote the full digest length to
     * *messageDigestLength, but no hash value was computed. In this case, it shall be possible to
     * call the function again. Thus, the workspace must not be reset. */
    if (messageDigest != NULL_PTR)
    {
      esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsHash); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  vSecPrim_digestHash()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) vSecPrim_digestHash(
  VSECPRIM_P2VAR_PARA(vSecPrimType_WorkSpaceHash) workSpace,
  const eslt_Length inputLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input,
  VSECPRIM_P2VAR_PARA(eslt_Length) messageDigestLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest)
{
  eslt_ErrorCode retVal;

  /* null pointer check */
  if ((workSpace == NULL_PTR) || (input == NULL_PTR) || (messageDigestLength == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* Check whether vSecPrim_initHash() was called - it must be called before. */
  else if ((retVal = esl_CheckWorkSpaceState(&workSpace->header, ESL_WST_ALGO_HASH)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  /* Check whether vSecPrim_updateHash() was called - it must not be called before (see PKCS 11) */
  else if (workSpace->updateCalled != VSECPRIM_UPDATE_NOT_YET_CALLED)
  {
    retVal = ESL_ERC_WS_STATE_INVALID;
  }
  else if (messageDigest == NULL_PTR)
  {
    /* If messageDigest is a null pointer, write the full digest length to *messageDigestLength and return */
    retVal = vSecPrim_switchHashIDFinalize(workSpace, messageDigestLength, messageDigest); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }
  else
  {
    /* If messageDigest is not a null pointer, hash the input data */
    retVal = vSecPrim_switchHashIDUpdate(workSpace, inputLength, input); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */

    if (retVal == ESL_ERC_NO_ERROR)
    {
      retVal = vSecPrim_switchHashIDFinalize(workSpace, messageDigestLength, messageDigest); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsHash); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    }
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_HASH_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: vSecPrim_Hash.c
 *********************************************************************************************************************/
