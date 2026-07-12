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
/*!        \file  ESLib_DRBG.c
 *        \brief  DRBG implementation, currently based on AES128
 *
 *      \details  Implementation for RANDOM Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define ESLIB_DRBG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ESLib.h"
#include "ESLib_Helper.h"

#include "actIDRBG.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define VSECPRIM_START_SEC_CODE
#include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (VSECPRIM_CTR_DRBG_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  esl_CTRDRBG_checkMode()
 **********************************************************************************************************************/
/*! \brief         Checks whether workspace is in a valid mode for CTR DRBG.
 *  \details       This function checks whether the algorithm size of modeDRBG is AES128 or AES256 and whether the
 *                 DF mode part of modeDRBG is valid.
 *  \param[in]     modeDRBG                DRBG mode to be checked
 *  \return        ESL_ERC_NO_ERROR        DRBG mode valid
 *                 ESL_ERC_MODE_INVALID    DRBG mode invalid
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_CTRDRBG_checkMode(
  eslt_DRBGMode modeDRBG);

/**********************************************************************************************************************
 *  esl_seedCTRDRBG_checkEntropySize()
 **********************************************************************************************************************/
/*! \brief         Checks whether entropyLength is a valid value.
 *  \details       This function is called during the (re)seeding process.
 *  \param[in]     workSpace                     pointer to the DRBG workspace
 *  \param[in]     entropyLength                 length of the entropy in bytes
 *  \return        ESL_ERC_NO_ERROR              entropyLength valid
 *                 ESL_ERC_RNG_ENTROPY_TOO_SMALL entropy length too short
 *  \pre           workSpace is a valid pointer and initialized by esl_initWorkSpaceHeader(..). esl_initCTRDRBG() was
 *                 called before.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_seedCTRDRBG_checkEntropySize(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  eslt_Length entropyLength);

/**********************************************************************************************************************
 *  esl_CTRDRBG_checkInputLengthNoDF()
 **********************************************************************************************************************/
/*! \brief         Checks whether inputLength == seed length, if no derivation function shall be used.
 *  \details       This function is called during initial seeding, reseeding and random number generation to make sure
 *                 that if no derivation function shall be used, no input parameter is longer than seed len.
 *                 The seed length depends on the length of the underlying AES key, so it is different for AES128 and
 *                 AES256.
 *  \param[in]     modeDRBG       DRBG mode (algSize, DF mode)
 *  \param[in]     inputLength    length of the input parameter to be checked
 *  \return        ESL_ERC_NO_ERROR              inputLength valid
 *                 ESL_ERC_INVALID_LENGTH        inputLength invalid, i.e., != seed length
 *  \pre           workSpace is a valid pointer and initialized by esl_initWorkSpaceHeader(..). esl_initCTRDRBG() was
 *                 called before.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_CTRDRBG_checkInputLengthNoDF(
  eslt_DRBGMode modeDRBG,
  eslt_Length inputLength);

/**********************************************************************************************************************
 *  esl_seedCTRDRBG_checkInput()
 **********************************************************************************************************************/
/*! \brief         Input check for CTR DRBG seed function (based on AES) for initial seeding or reseeding.
 *  \details       This function checks whether the mode and the entropy length input parameter for the CTR DRBG seed
 *                 function are valid.
 *  \param[in]     workSpace                     pointer to the DRBG workspace
 *  \param[in]     entropyLength                 length of the entropy in bytes
 *  \return        ESL_ERC_NO_ERROR              all length parameters valid
 *                 ESL_ERC_MODE_INVALID          incorrect mode for DRBG (algorithm size or derivation function)
 *                 ESL_ERC_RNG_ENTROPY_TOO_SMALL entropy length too short
 *  \pre           workSpace is a valid pointer and initialized by esl_initWorkSpaceHeader(..).
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_seedCTRDRBG_checkInput(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  eslt_Length entropyLength);

/**********************************************************************************************************************
 *  esl_getBytesCTRDRBG_checkInput()
 **********************************************************************************************************************/
/*! \brief         Input check for CTR DRBG generation function.
 *  \details       This function checks whether the mode and the length input parameters for the DRBG
 *                 generation function are valid.
 *  \param[in]     modeDRBG                      CTR DRBG mode (algorithm size, DF mode)
 *  \param[in]     requestedLength               number of requested random bytes
 *  \param[in]     maxRequestLength              maximal number of bytes per request
 *  \param[in]     additionalInputLength         length of the additional input in bytes, possibly 0
 *  \return        ESL_ERC_NO_ERROR                 all length parameters valid
 *                 ESL_ERC_MODE_INVALID             incorrect mode for DRBG (algorithm size or derivation function)
 *                 ESL_ERC_INVALID_LENGTH           additionalInputLength too high
 *                 ESL_ERC_REQUESTED_LENGTH_INVALID requestedLength too high
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_getBytesCTRDRBG_checkInput(
  eslt_DRBGMode modeDRBG,
  eslt_Length requestedLength,
  eslt_Length maxRequestLength,
  eslt_Length additionalInputLength);

/**********************************************************************************************************************
 *  esl_initialSeed_CTRDRBG()
 **********************************************************************************************************************/
/*! \brief         Initial Seed for NIST800-90A DRBG based on AES128
 *  \details       This function generates the initial internal seed state using the provided entropy source.
 *  \param[in,out] workSpace                    (in)  pointer to DRBG workSpace
 *                                              (out) DRBG workSpace, wsAES, key and variable modified
 *  \param[in]     entropyPtr                   pointer to entropy buffer
 *  \param[in]     entropyLength                length of the entropy in bytes
 *  \param[in]     noncePtr                     pointer to nonce buffer, possibly a null pointer
 *  \param[in]     nonceLength                  length of the nonce in bytes, possibly 0
 *  \param[in]     personalizationStringPtr     pointer to personalization string buffer, possibly a null pointer
 *  \param[in]     personalizationStringLength  length of the personalization string in bytes, possibly 0
 *  \return        ESL_ERC_NO_ERROR               seeding successful
 *                 ESL_ERC_PARAMETER_INVALID      a derivation function shall be used, but noncePtr is a null pointer
 *                 ESL_ERC_INVALID_LENGTH         nonceLength or personalizationStringLength too high
 *  \pre           workSpace has to be a valid pointer. workSpace has to be initialized by calling esl_initCTRDRBG().
 *                 entropyPtr needs to be a valid pointer. The buffer referenced by entropyPtr needs to provide
 *                 at least entropyLength bytes.
 *                 If noncePtr is not a null pointer, the buffer referenced by noncePtr has to provide at least
 *                 nonceLength bytes.
 *                 If personalizationStringPtr is not a null pointer, the buffer referenced by personalizationStringPtr
 *                 has to provide at least personalizationStringLength bytes.
 *  \context       TASK
 *  \reentrant     TRUE, for different workSpaces
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initialSeed_CTRDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) entropyPtr,
  eslt_Length entropyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) noncePtr,
  eslt_Length nonceLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) personalizationStringPtr,
  eslt_Length personalizationStringLength);

/**********************************************************************************************************************
 *  esl_reseed_CTRDRBG()
 **********************************************************************************************************************/
/*! \brief         Reeed for NIST800-90A DRBG based on AES128
 *  \details       This function generates the new internal seed state using the provided entropy source.
 *  \param[in,out] workSpace               (in)  pointer to DRBG workSpace
 *                                         (out) DRBG workSpace, wsAES, key and variable modified
 *  \param[in]     entropyPtr              pointer to entropy buffer
 *  \param[in]     entropyLength           length of the entropy in bytes
 *  \param[in]     additionalInputPtr      pointer to additional input buffer, possibly a null pointer
 *  \param[in]     additionalInputLength   length of the additional input in bytes, possibly 0
 *  \return        ESL_ERC_NO_ERROR               reseeding successful
 *                 ESL_ERC_INVALID_LENGTH         additionalInputLength too high
 *  \pre           workSpace has to be a valid pointer. workSpace has to be initialized by calling esl_initCTRDRBG().
 *                 entropyPtr needs to be a valid pointer. The buffer referenced by entropyPtr needs to provide
 *                 at least entropyLength bytes.
 *                 If additionalInputPtr is not a null pointer, the buffer referenced by additionalInputPtr has to
 *                 provide at least additionalInputLength bytes.
 *  \context       TASK
 *  \reentrant     TRUE, for different workSpaces
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_reseed_CTRDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) entropyPtr,
  eslt_Length entropyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) additionalInputPtr,
  eslt_Length additionalInputLength);

#endif /* VSECPRIM_CTR_DRBG_ENABLED == STD_ON */

#if (VSECPRIM_HASH_DRBG_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  esl_seedHASHDRBG_checkInput()
 **********************************************************************************************************************/
/*! \brief         Input check for Hash DRBG seed function (based on SHA) for initial seeding or reseeding.
 *  \details       This function checks whether the mode and the entropy length input parameter for the Hash DRBG seed
 *                 function are valid.
 *  \param[in]     workSpace                     pointer to the DRBG workspace
 *  \param[in]     entropyLength                 length of the entropy in bytes
 *  \return        ESL_ERC_NO_ERROR              all length parameters valid
 *                 ESL_ERC_MODE_INVALID          incorrect mode for HASH DRBG (algorithm size)
 *                 ESL_ERC_RNG_ENTROPY_TOO_SMALL entropy length too short
 *  \pre           workSpace is a valid pointer and initialized by esl_initWorkSpaceHeader(..).
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_seedHASHDRBG_checkInput(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceHASHDRBG) workSpace,
  eslt_Length entropyLength);

/**********************************************************************************************************************
 *  esl_getBytesHASHDRBG_checkInput()
 **********************************************************************************************************************/
/*! \brief         Input check for HASH DRBG generation function.
 *  \details       This function checks whether the mode and the length input parameters for the HASH DRBG
 *                 generation function are valid.
 *  \param[in]     modeDRBG                      Hash DRBG mode (algorithm size)
 *  \param[in]     requestedLength               number of requested random bytes
 *  \param[in]     maxRequestLength              maximal number of bytes per request
 *  \return        ESL_ERC_NO_ERROR                 all length parameters valid
 *                 ESL_ERC_MODE_INVALID             incorrect mode for DRBG (algorithm size or derivation function)
 *                 ESL_ERC_REQUESTED_LENGTH_INVALID requestedLength too high
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_getBytesHASHDRBG_checkInput(
  eslt_DRBGMode modeDRBG,
  eslt_Length requestedLength,
  eslt_Length maxRequestLength);

/**********************************************************************************************************************
 *  esl_initialSeed_HASHDRBG()
 **********************************************************************************************************************/
/*! \brief         Initial Seed for NIST800-90A HASH DRBG based on SHA512
 *  \details       This function generates the initial internal seed state using the provided entropy source.
 *  \param[in,out] workSpace                    (in)  pointer to DRBG workSpace
 *                                              (out) DRBG workSpace, wsSHA, constant and variable modified
 *  \param[in]     entropyPtr                   pointer to entropy buffer
 *  \param[in]     entropyLength                length of the entropy in bytes
 *  \param[in]     noncePtr                     pointer to nonce buffer, possibly a null pointer
 *  \param[in]     nonceLength                  length of the nonce in bytes, possibly 0
 *  \param[in]     personalizationStringPtr     pointer to personalization string buffer, possibly a null pointer
 *  \param[in]     personalizationStringLength  length of the personalization string in bytes, possibly 0
 *  \return        ESL_ERC_NO_ERROR             seeding successful
 *                 ESL_ERC_PARAMETER_INVALID    noncePtr is a null pointer
 *  \pre           workSpace has to be a valid pointer. workSpace has to be initialized by calling esl_initHASHDRBG().
 *                 entropyPtr needs to be a valid pointer. The buffer referenced by entropyPtr needs to provide
 *                 at least entropyLength bytes.
 *                 If noncePtr is not a null pointer, the buffer referenced by noncePtr has to provide at least
 *                 nonceLength bytes.
 *                 If personalizationStringPtr is not a null pointer, the buffer referenced by personalizationStringPtr
 *                 has to provide at least personalizationStringLength bytes.
 *  \context       TASK
 *  \reentrant     TRUE, for different workSpaces
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initialSeed_HASHDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHASHDRBG) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) entropyPtr,
  eslt_Length entropyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) noncePtr,
  eslt_Length nonceLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) personalizationStringPtr,
  eslt_Length personalizationStringLength);

/**********************************************************************************************************************
 *  esl_reseed_HASHDRBG()
 **********************************************************************************************************************/
/*! \brief         Reeed for NIST800-90A HASH DRBG based on SHA512
 *  \details       This function generates the new internal seed state using the provided entropy source.
 *  \param[in,out] workSpace               (in)  pointer to DRBG workSpace
 *                                         (out) DRBG workSpace, wsSHA, constant and variable modified
 *  \param[in]     entropyPtr              pointer to entropy buffer
 *  \param[in]     entropyLength           length of the entropy in bytes
 *  \param[in]     additionalInputPtr      pointer to additional input buffer, possibly a null pointer
 *  \param[in]     additionalInputLength   length of the additional input in bytes, possibly 0
 *  \pre           workSpace has to be a valid pointer. workSpace has to be initialized by calling esl_initHASHDRBG().
 *                 entropyPtr needs to be a valid pointer. The buffer referenced by entropyPtr needs to provide
 *                 at least entropyLength bytes.
 *                 If additionalInputPtr is not a null pointer, the buffer referenced by additionalInputPtr has to
 *                 provide at least additionalInputLength bytes.
 *  \context       TASK
 *  \reentrant     TRUE, for different workSpaces
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) esl_reseed_HASHDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHASHDRBG) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) entropyPtr,
  eslt_Length entropyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) additionalInputPtr,
  eslt_Length additionalInputLength);

#endif /* VSECPRIM_HASH_DRBG_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (VSECPRIM_CTR_DRBG_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  esl_CTRDRBG_checkMode()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_CTRDRBG_checkMode(
  eslt_DRBGMode modeDRBG)
{
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;

  /* check algorithm size */
  if ( ((actDRBG_ALGSIZE_MASK & modeDRBG) != actCTRDRBG_ALGSIZE_AES128) &&
    ((actDRBG_ALGSIZE_MASK & modeDRBG) != actCTRDRBG_ALGSIZE_AES256))
  {
    retVal = ESL_ERC_MODE_INVALID;
  }
  else
  {
    /* check mode of derivation function */
    if (((actDRBG_DFMODE_MASK & modeDRBG) != actCTRDRBG_DFMODE_USE_BLOCKCIPHER_DF) &&
      ((actDRBG_DFMODE_MASK & modeDRBG) != actCTRDRBG_DFMODE_USE_NO_DF))
    {
      retVal = ESL_ERC_MODE_INVALID;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_CTRDRBG_checkInputLengthNoDF()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_CTRDRBG_checkInputLengthNoDF(
  eslt_DRBGMode modeDRBG,
  eslt_Length inputLength)
{
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;

  if ((actDRBG_DFMODE_MASK & modeDRBG) == actCTRDRBG_DFMODE_USE_NO_DF)
  {
    /* Since actCTRDRBG_DFMODE_USE_NO_DF = 0x00u, in this case it is not necessary to use actDRBG_ALGSIZE_MASK
     * to check the AES variant. */
    if ((modeDRBG == actCTRDRBG_ALGSIZE_AES128) && (inputLength > ESL_CTRDRBG_AES128_SEEDLEN))
    {
      retVal = ESL_ERC_INVALID_LENGTH;
    }
    else if ((modeDRBG == actCTRDRBG_ALGSIZE_AES256) && (inputLength > ESL_CTRDRBG_AES256_SEEDLEN))
    {
      retVal = ESL_ERC_INVALID_LENGTH;
    }
    else
    {
      /* retVal already set */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_seedCTRDRBG_checkEntropySize()
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
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_seedCTRDRBG_checkEntropySize(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  eslt_Length entropyLength)
{
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;
  eslt_Length requiredEntropyLengthNoDF, requiredEntropyLengthDF;

  if ((actDRBG_ALGSIZE_MASK & workSpace->wsDRBG.mode) == actCTRDRBG_ALGSIZE_AES128)
  {
    requiredEntropyLengthDF = actBitsToBytes(actAES128CTRDRBG_SECURITY_STRENGTH);
    requiredEntropyLengthNoDF = ESL_CTRDRBG_AES128_SEEDLEN;
  }
  else /* size = actCTRDRBG_ALGSIZE_AES256, validity of size was checked before */
  {
    requiredEntropyLengthDF = actBitsToBytes(actAES256CTRDRBG_SECURITY_STRENGTH);
    requiredEntropyLengthNoDF = ESL_CTRDRBG_AES256_SEEDLEN;
  }

  /* if a derivation function is used, entropy must at least have security strength length
   * if no derivation function is used, entropy must have seed length */
  if ((actDRBG_DFMODE_MASK & workSpace->wsDRBG.mode) == actCTRDRBG_DFMODE_USE_BLOCKCIPHER_DF)
  {
    if (entropyLength < requiredEntropyLengthDF)
    {
      retVal = ESL_ERC_RNG_ENTROPY_TOO_SMALL;
    }
  }
  else /* mode = actCTRDRBG_DFMODE_USE_NO_DF, validity of mode was checked before */
  {
    if (entropyLength != requiredEntropyLengthNoDF)
    {
      retVal = ESL_ERC_RNG_ENTROPY_TOO_SMALL;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_seedCTRDRBG_checkInput()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_seedCTRDRBG_checkInput(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  eslt_Length entropyLength)
{
  eslt_ErrorCode retVal;

  /* check DRBG mode */
  retVal = esl_CTRDRBG_checkMode(workSpace->wsDRBG.mode);

  if (retVal == ESL_ERC_NO_ERROR)
  {
    /* check entropy size */
    retVal = esl_seedCTRDRBG_checkEntropySize(workSpace, entropyLength); /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_getBytesCTRDRBG_checkInput()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_getBytesCTRDRBG_checkInput(
  eslt_DRBGMode modeDRBG,
  eslt_Length requestedLength,
  eslt_Length maxRequestLength,
  eslt_Length additionalInputLength)
{
  eslt_ErrorCode retVal;

  retVal = esl_CTRDRBG_checkMode(modeDRBG);

  if (retVal == ESL_ERC_NO_ERROR)
  {
    if (requestedLength > maxRequestLength)
    {
      retVal = ESL_ERC_REQUESTED_LENGTH_INVALID;
    }
  }

  if (retVal == ESL_ERC_NO_ERROR)
  {
    retVal = esl_CTRDRBG_checkInputLengthNoDF(modeDRBG, additionalInputLength);
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_reseed_CTRDRBG()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_reseed_CTRDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) entropyPtr,
  eslt_Length entropyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) additionalInputPtr,
  eslt_Length additionalInputLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;
  actRETURNCODE actRetVal;
  actDrbgSeedInputType dataInput;

  /* ----- Implementation ------------------------------------------------- */

  /* #2 set input parameter */
  dataInput.input[actDRBGSEED_INPUT_PARAM1].dataPtr = entropyPtr; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
  dataInput.input[actDRBGSEED_INPUT_PARAM1].length = (actLengthType)entropyLength; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
  dataInput.elements = 1u;

  /* #5 check if additional input is available. (optional feature) */
  if (additionalInputPtr != NULL_PTR)
  {
    /* if no derivation function is used, additional input must not be longer than seed length (null pointer allowed) */
    retVal = esl_CTRDRBG_checkInputLengthNoDF(workSpace->wsDRBG.mode, additionalInputLength);
    if (retVal == ESL_ERC_NO_ERROR)
    {
      /* use additional input for reseeding. */
      dataInput.input[actDRBGSEED_INPUT_PARAM2].dataPtr = additionalInputPtr; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
      dataInput.input[actDRBGSEED_INPUT_PARAM2].length = (actLengthType)additionalInputLength; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
      dataInput.elements++;
    }
  }

  if (retVal == ESL_ERC_NO_ERROR)
  {
    /* #10 reseed via actAesCtrDrbgInstantiation */
    actRetVal = actAesCtrDrbgInstantiation(&(workSpace->wsDRBG), &dataInput, TRUE, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actRetVal == actEXCEPTION_INPUT_LENGTH)
    {
      retVal = ESL_ERC_INVALID_LENGTH;
    }
    else /* actRetVal == actOK */
    {
      workSpace->wsDRBG.seedState = ESL_DRBG_SEED_STATUS_SEEDED; /* SBSW_VSECPRIM_WRITE_POINTER */
      /* retVal already set */
    }
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  esl_initialSeed_CTRDRBG()
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
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initialSeed_CTRDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) entropyPtr,
  eslt_Length entropyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) noncePtr,
  eslt_Length nonceLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) personalizationStringPtr,
  eslt_Length personalizationStringLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;
  actRETURNCODE actRetVal;
  actDrbgSeedInputType dataInput;

  /* ----- Implementation ------------------------------------------------- */
  /* #1 clear seed state: (will be done in actAesCtrDrbgInstantiation) */
  /* #5 set input parameter also recommended and optional inputs */
  dataInput.input[actDRBGSEED_INPUT_PARAM1].dataPtr = entropyPtr; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
  dataInput.input[actDRBGSEED_INPUT_PARAM1].length = (actLengthType)entropyLength; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
  dataInput.elements = 1u;

  if ((actDRBG_DFMODE_MASK & workSpace->wsDRBG.mode) == actCTRDRBG_DFMODE_USE_BLOCKCIPHER_DF)
  {
    /* If a derivation function shall be sed and nonce is a null pointer, do not call actAesCtrDrbgInstantiation */
    if (noncePtr != NULL_PTR)
    {
      dataInput.input[actDRBGSEED_INPUT_PARAM2].dataPtr = noncePtr; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
      dataInput.input[actDRBGSEED_INPUT_PARAM2].length = (actLengthType)nonceLength; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
      dataInput.elements++;
    }
    else
    {
      retVal = ESL_ERC_PARAMETER_INVALID;
    }
  }

  if (retVal == ESL_ERC_NO_ERROR)
  {
    if (personalizationStringPtr != NULL_PTR)
    {
      /* if no derivation function is used, personalization string must not be longer than seed length (null pointer allowed) */
      retVal = esl_CTRDRBG_checkInputLengthNoDF(workSpace->wsDRBG.mode, personalizationStringLength);

      if (retVal == ESL_ERC_NO_ERROR)
      {
        /* set personal string and seed */
        dataInput.input[dataInput.elements].dataPtr = personalizationStringPtr; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
        dataInput.input[dataInput.elements].length = (actLengthType)personalizationStringLength; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
        dataInput.elements++;
      }
    }
  }

  if (retVal == ESL_ERC_NO_ERROR)
  {
    /* #20 seed with personal string via actAesCtrDrbgInstantiation */
    actRetVal = actAesCtrDrbgInstantiation(&(workSpace->wsDRBG), &dataInput, FALSE, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actRetVal == actEXCEPTION_INPUT_LENGTH)
    {
      retVal = ESL_ERC_INVALID_LENGTH;
    }
    else /* actRetVal == actOK */
    {
      /* #30 set algorithm initiated */
      workSpace->wsDRBG.seedState = ESL_DRBG_SEED_STATUS_SEEDED; /* SBSW_VSECPRIM_WRITE_POINTER */
      /* retVal already set to ESL_ERC_NO_ERROR */
    }
  }

  return retVal;
}  /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

#endif /* VSECPRIM_CTR_DRBG_ENABLED == STD_ON */

#if (VSECPRIM_HASH_DRBG_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  esl_seedHASHDRBG_checkInput()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_seedHASHDRBG_checkInput(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceHASHDRBG) workSpace,
  eslt_Length entropyLength)
{
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;

  /* check DRBG mode */
  if (workSpace->wsDRBG.mode != actDRBGMODE_HASHDRBG_SHA512)
  {
    retVal = ESL_ERC_MODE_INVALID;
  }
  else
  {
    /* check entropy size */
    if (entropyLength < actBitsToBytes(actHASHDRBG_SECURITY_STRENGTH))
    {
      retVal = ESL_ERC_RNG_ENTROPY_TOO_SMALL;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_getBytesHASHDRBG_checkInput()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_getBytesHASHDRBG_checkInput(
  eslt_DRBGMode modeDRBG,
  eslt_Length requestedLength,
  eslt_Length maxRequestLength)
{
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;

  /* check DRBG mode */
  if (modeDRBG != actDRBGMODE_HASHDRBG_SHA512)
  {
    retVal = ESL_ERC_MODE_INVALID;
  }
  else
  {
    /* check requested length */
    if (requestedLength > maxRequestLength)
    {
      retVal = ESL_ERC_REQUESTED_LENGTH_INVALID;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_initialSeed_HASHDRBG()
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
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initialSeed_HASHDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHASHDRBG) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) entropyPtr,
  eslt_Length entropyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) noncePtr,
  eslt_Length nonceLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) personalizationStringPtr,
  eslt_Length personalizationStringLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;
  actDrbgSeedInputType dataInput;

  /* A nonce value is required for */
  if (noncePtr == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------- */
    /* #1 clear seed state: (will be done in actHashDrbgInstantiation) */
    /* #5 set input parameters, also recommended and optional inputs */
    dataInput.input[actDRBGSEED_INPUT_PARAM1].dataPtr = entropyPtr; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
    dataInput.input[actDRBGSEED_INPUT_PARAM1].length = (actLengthType)entropyLength; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */

    dataInput.input[actDRBGSEED_INPUT_PARAM2].dataPtr = noncePtr; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
    dataInput.input[actDRBGSEED_INPUT_PARAM2].length = (actLengthType)nonceLength; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
    dataInput.elements = 2u;

    if (personalizationStringPtr != NULL_PTR)
    {
      /* set personal string and seed */
      dataInput.input[actDRBGSEED_INPUT_PARAM3].dataPtr = personalizationStringPtr; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
      dataInput.input[actDRBGSEED_INPUT_PARAM3].length = (actLengthType)personalizationStringLength; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
      dataInput.elements++;
    }

    /* #20 seed with personal string via actHashDrbgInstantiation */
    actHashDrbgInstantiation(&(workSpace->wsDRBG), &dataInput, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* #30 set algorithm initiated */
    workSpace->wsDRBG.seedState = ESL_DRBG_SEED_STATUS_SEEDED; /* SBSW_VSECPRIM_WRITE_POINTER */
    /* retVal already set to ESL_ERC_NO_ERROR */
  }

  return retVal;
}  /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  esl_reseed_HASHDRBG()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) esl_reseed_HASHDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHASHDRBG) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) entropyPtr,
  eslt_Length entropyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) additionalInputPtr,
  eslt_Length additionalInputLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  actDrbgSeedInputType dataInput;
  actU8 firstElementToHash = actHASHDRBG_SEEDING_PREFIX2;

  /* ----- Implementation ------------------------------------------------- */
  /* #2 set input parameter: [0x01 || V || entropy || additionalInput] */
  dataInput.input[actDRBGSEED_INPUT_PARAM1].dataPtr = &firstElementToHash; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
  dataInput.input[actDRBGSEED_INPUT_PARAM1].length = 1u; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */

  dataInput.input[actDRBGSEED_INPUT_PARAM2].dataPtr = workSpace->wsDRBG.variable; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
  dataInput.input[actDRBGSEED_INPUT_PARAM2].length = ESL_HASHDRBG_SHA512_SEED_LEN; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */

  dataInput.input[actDRBGSEED_INPUT_PARAM3].dataPtr = entropyPtr; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
  dataInput.input[actDRBGSEED_INPUT_PARAM3].length = (actLengthType)entropyLength; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
  dataInput.elements = 3u;

  /* #5 check if additional input is available. (optional feature) */
  if (additionalInputPtr != NULL_PTR)
  {
    /* use additional input for reseeding. */
    dataInput.input[actDRBGSEED_INPUT_PARAM4].dataPtr = additionalInputPtr; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
    dataInput.input[actDRBGSEED_INPUT_PARAM4].length = (actLengthType)additionalInputLength; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
    dataInput.elements++;
  }

  /* #10 reseed via actHashDrbgInstantiation */
  actHashDrbgInstantiation(&(workSpace->wsDRBG), &dataInput, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  /* #30 set algorithm (re)seeded */
  workSpace->wsDRBG.seedState = ESL_DRBG_SEED_STATUS_SEEDED; /* SBSW_VSECPRIM_WRITE_POINTER */

} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

#endif /* VSECPRIM_HASH_DRBG_ENABLED == STD_ON */

/**********************************************************************************************************************
 * GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if VSECPRIM_CTR_DRBG_ENABLED == STD_ON

/**********************************************************************************************************************
 *  esl_initCTRDRBG()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_initCTRDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  eslt_DRBGMode modeDRBG)
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_ErrorCode retVal;

  /* ----- Implementation ------------------------------------------------- */

  if (workSpace == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_CTRDRBG)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    workSpace->wsDRBG.seedState = ESL_DRBG_SEED_STATUS_UNSEEDED; /* SBSW_VSECPRIM_WRITE_POINTER */

    switch (modeDRBG)
    {
    case ESL_DRBGMODE_CTRDRBG_AES128_NODF:
    case ESL_DRBGMODE_CTRDRBG_AES128_DF:
    case ESL_DRBGMODE_CTRDRBG_AES256_NODF:
    case ESL_DRBGMODE_CTRDRBG_AES256_DF:
      workSpace->wsDRBG.mode = modeDRBG; /* SBSW_VSECPRIM_WRITE_POINTER */
      break;
    default:
      retVal = ESL_ERC_MODE_INVALID;
      break;
    }

    esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_CTRDRBG); /* SBSW_VSECPRIM_CALL_FUNCTION */

  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_seedCTRDRBG()
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
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_seedCTRDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) entropyPtr,
  eslt_Length entropyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) noncePtr,
  eslt_Length nonceLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) personalizationStringPtr,
  eslt_Length personalizationStringLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) additionalInputPtr,
  eslt_Length additionalInputLength)
{
  eslt_ErrorCode retVal;

  if ((workSpace == NULL_PTR) || (entropyPtr == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_CTRDRBG, ESL_WST_ALGO_CTRDRBG)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  /* check mode and entropy length */
  else if ((retVal = esl_seedCTRDRBG_checkInput(workSpace, entropyLength)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    /* initial seeding */
    if (workSpace->wsDRBG.seedState == ESL_DRBG_SEED_STATUS_UNSEEDED)
    {
      retVal = esl_initialSeed_CTRDRBG(workSpace, entropyPtr, entropyLength,
                                       noncePtr, nonceLength,
                                       personalizationStringPtr, personalizationStringLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
    /* reseeding */
    else
    {
      retVal = esl_reseed_CTRDRBG(workSpace, entropyPtr, entropyLength,
                                  additionalInputPtr, additionalInputLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
  }

  return retVal;
} /* PRQA S 6060, 6080 */ /* MD_VSECPRIM_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  esl_getBytesCTRDRBG()
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
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_getBytesCTRDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  eslt_Length requestedLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) outputBuffer,
  VSECPRIM_P2CONST_PARA(eslt_Byte) additionalInputPtr,
  eslt_Length additionalInputLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;

  /* ----- Implementation ------------------------------------------------- */
  if ((workSpace == NULL_PTR) || (outputBuffer == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_CTRDRBG, ESL_WST_ALGO_CTRDRBG)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else if ((retVal = esl_getBytesCTRDRBG_checkInput(workSpace->wsDRBG.mode, requestedLength, actAESCTRDRBG_MAX_OUTPUT_SIZE, additionalInputLength)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  else if (workSpace->wsDRBG.seedState != ESL_DRBG_SEED_STATUS_SEEDED)
  {
    retVal = ESL_ERC_ENTROPY_EXHAUSTION;
  }
  else
  {
    actRetVal = actAesCtrDrbgGenerate(&workSpace->wsDRBG, (actLengthType)requestedLength, outputBuffer,
                                      additionalInputPtr, (actLengthType)additionalInputLength,
                                      workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actRetVal == actEXCEPTION_ENTROPY_EXHAUSTION)
    {
      retVal = ESL_ERC_ENTROPY_EXHAUSTION;
    }
    else if (actRetVal == actEXCEPTION_INPUT_LENGTH)
    {
      retVal = ESL_ERC_INVALID_LENGTH;
    }
    else /*(actRetVal == actOK) */
    {
      retVal = ESL_ERC_NO_ERROR;
    }
  }

  /* 8. Return SUCCESS and returned_bits; also return Key, V, and reseed_counter as the new_working_state. */
  return retVal;
} /* PRQA S 6060, 6080 */ /* MD_VSECPRIM_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  esl_getStateCTRDRBG()
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
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_getStateCTRDRBG(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) internalStateBuffer,
  VSECPRIM_P2VAR_PARA(eslt_Length) internalStateBufferLength,
  VSECPRIM_P2VAR_PARA(eslt_DRBGReseedCounterType) reseedCounter,
  VSECPRIM_P2VAR_PARA(eslt_DRBGSeedStatusType) seedStatus,
  VSECPRIM_P2VAR_PARA(eslt_DRBGMode) modeDRBG)
{
  eslt_ErrorCode retVal;
  eslt_Length localSeedLength, localKeyLength;

  if ((workSpace == NULL_PTR) || (internalStateBuffer == NULL_PTR) || (internalStateBufferLength == NULL_PTR) ||
      (reseedCounter == NULL_PTR) || (seedStatus == NULL_PTR) || (modeDRBG == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    if ((workSpace->wsDRBG.mode & actDRBG_ALGSIZE_MASK) == actCTRDRBG_ALGSIZE_AES128)
    {
      localSeedLength = ESL_CTRDRBG_AES128_SEEDLEN;
      localKeyLength = ESL_SIZEOF_CTRDRBG_AES128_KEY;
    }
    else /* pre: esl_initCTRDRBG was called before. Sufficient? */
    {
      localSeedLength = ESL_CTRDRBG_AES256_SEEDLEN;
      localKeyLength = ESL_SIZEOF_CTRDRBG_AES256_KEY;
    }

    if (*internalStateBufferLength < localSeedLength)
    {
      retVal = ESL_ERC_BUFFER_TOO_SMALL;
    }
    else
    {
      /* If workspace state is unseeded, set output buffers to zero. */
      if (workSpace->wsDRBG.seedState == ESL_DRBG_SEED_STATUS_UNSEEDED)
      {
        actMemClear(internalStateBuffer, localSeedLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        *reseedCounter = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
      }
      else /* If state is seeded (or needs reseed), copy internal state of DRBG to output buffer. */
      {
        actMemCpyByteArray(internalStateBuffer, workSpace->wsDRBG.key, localKeyLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        actMemCpyByteArray(&internalStateBuffer[localKeyLength], workSpace->wsDRBG.variable, ESL_SIZEOF_CTRDRBG_BLOCK); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS_FIXED_SIZE */
        *reseedCounter = workSpace->wsDRBG.reseedCount; /* SBSW_VSECPRIM_WRITE_POINTER */
      }

      *seedStatus = workSpace->wsDRBG.seedState; /* SBSW_VSECPRIM_WRITE_POINTER */
      *modeDRBG = workSpace->wsDRBG.mode; /* SBSW_VSECPRIM_WRITE_POINTER */
      *internalStateBufferLength = localSeedLength; /* SBSW_VSECPRIM_WRITE_POINTER */
      retVal = ESL_ERC_NO_ERROR;
    }
  }

  return retVal;
}  /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  esl_restoreStateCTRDRBG()
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
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_restoreStateCTRDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) internalStateBuffer,
  eslt_Length internalStateBufferLength,
  eslt_DRBGReseedCounterType reseedCounter,
  eslt_DRBGSeedStatusType seedStatus,
  eslt_DRBGMode modeDRBG)
{
  eslt_ErrorCode retVal;
  eslt_Length localSeedLength, localKeyLength;

  if ((workSpace == NULL_PTR) || (internalStateBuffer == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    if ((workSpace->wsDRBG.mode & actDRBG_ALGSIZE_MASK) == actCTRDRBG_ALGSIZE_AES128)
    {
      localSeedLength = ESL_CTRDRBG_AES128_SEEDLEN;
      localKeyLength = ESL_SIZEOF_CTRDRBG_AES128_KEY;
    }
    else /* in this case ((workSpace->mode & actDRBG_ALGSIZE_MASK) = actCTRDRBG_ALGSIZE_AES256),
         * since the validity of the size was checked before */
    {
      localSeedLength = ESL_CTRDRBG_AES256_SEEDLEN;
      localKeyLength = ESL_SIZEOF_CTRDRBG_AES256_KEY;
    }

    if (internalStateBufferLength != localSeedLength)
    {
      retVal = ESL_ERC_INVALID_LENGTH;
    }
    else
    {
      /* restore internal state from inout arguments */
      actMemCpyByteArray(workSpace->wsDRBG.key, internalStateBuffer, localKeyLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      actMemCpyByteArray(workSpace->wsDRBG.variable, &internalStateBuffer[localKeyLength], ESL_SIZEOF_CTRDRBG_BLOCK); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      workSpace->wsDRBG.reseedCount = reseedCounter; /* SBSW_VSECPRIM_WRITE_POINTER */
      workSpace->wsDRBG.seedState = seedStatus; /* SBSW_VSECPRIM_WRITE_POINTER */
      workSpace->wsDRBG.mode = modeDRBG; /* SBSW_VSECPRIM_WRITE_POINTER */

      retVal = ESL_ERC_NO_ERROR;
    }
  }

  return retVal;
}  /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

#endif /* VSECPRIM_CTR_DRBG_ENABLED == STD_ON */

#if VSECPRIM_HASH_DRBG_ENABLED == STD_ON

/**********************************************************************************************************************
 *  esl_initHASHDRBG()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_initHASHDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHASHDRBG) workSpace,
  eslt_DRBGMode modeDRBG)
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_ErrorCode retVal;

  /* ----- Implementation ------------------------------------------------- */

  if (workSpace == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_HASHDRBG)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    workSpace->wsDRBG.seedState = ESL_DRBG_SEED_STATUS_UNSEEDED; /* SBSW_VSECPRIM_WRITE_POINTER */

    switch (modeDRBG)
    {
    case ESL_DRBGMODE_HASHDRBG_SHA512:
      workSpace->wsDRBG.mode = modeDRBG; /* SBSW_VSECPRIM_WRITE_POINTER */
      break;
    default:
      retVal = ESL_ERC_MODE_INVALID;
      break;
    }

    esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_HASHDRBG); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_seedHASHDRBG()
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
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_seedHASHDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHASHDRBG) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) entropyPtr,
  eslt_Length entropyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) noncePtr,
  eslt_Length nonceLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) personalizationStringPtr,
  eslt_Length personalizationStringLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) additionalInputPtr,
  eslt_Length additionalInputLength)
{
  eslt_ErrorCode retVal;

  if ((workSpace == NULL_PTR) || (entropyPtr == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_HASHDRBG, ESL_WST_ALGO_HASHDRBG)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else if ((retVal = esl_seedHASHDRBG_checkInput(workSpace, entropyLength)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    /* initial seeding */
    if (workSpace->wsDRBG.seedState == ESL_DRBG_SEED_STATUS_UNSEEDED)
    {
      retVal = esl_initialSeed_HASHDRBG(workSpace, entropyPtr, entropyLength, noncePtr, nonceLength,
        personalizationStringPtr, personalizationStringLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
    /* reseeding */
    else
    {
      esl_reseed_HASHDRBG(workSpace, entropyPtr, entropyLength, additionalInputPtr, additionalInputLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* retVal = ESL_ERC_NO_ERROR already set */
    }
  }

  return retVal;
} /* PRQA S 6060, 6080 */ /* MD_VSECPRIM_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  esl_getBytesHASHDRBG()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_getBytesHASHDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHASHDRBG) workSpace,
  eslt_Length requestedLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) outputBuffer,
  VSECPRIM_P2CONST_PARA(eslt_Byte) additionalInputPtr,
  eslt_Length additionalInputLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;

  /* ----- Implementation ------------------------------------------------- */
  if ((workSpace == NULL_PTR) || (outputBuffer == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_HASHDRBG, ESL_WST_ALGO_HASHDRBG)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else if ((retVal = esl_getBytesHASHDRBG_checkInput(workSpace->wsDRBG.mode, requestedLength, actHASHDRBG_MAX_OUTPUT_SIZE)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  else if (workSpace->wsDRBG.seedState != ESL_DRBG_SEED_STATUS_SEEDED)
  {
    retVal = ESL_ERC_ENTROPY_EXHAUSTION;
  }
  else
  {
    actRetVal = actHashDrbgGenerate(&workSpace->wsDRBG, requestedLength, outputBuffer,
      additionalInputPtr, additionalInputLength, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actRetVal == actEXCEPTION_ENTROPY_EXHAUSTION)
    {
      retVal = ESL_ERC_ENTROPY_EXHAUSTION;
    }
    else /*(actRetVal == actOK) */
    {
      retVal = ESL_ERC_NO_ERROR;
    }
  }

  /* 8. Return SUCCESS and returned_bits; also return Key, V, and reseed_counter as the new_working_state. */
  return retVal;
} /* PRQA S 6060, 6080 */ /* MD_VSECPRIM_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  esl_getStateHASHDRBG()
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
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_getStateHASHDRBG(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceHASHDRBG) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) internalStateBuffer,
  VSECPRIM_P2VAR_PARA(eslt_Length) internalStateBufferLength,
  VSECPRIM_P2VAR_PARA(eslt_DRBGReseedCounterType) reseedCounter,
  VSECPRIM_P2VAR_PARA(eslt_DRBGSeedStatusType) seedStatus,
  VSECPRIM_P2VAR_PARA(eslt_DRBGMode) modeDRBG)
{
  eslt_ErrorCode retVal;

  if ((workSpace == NULL_PTR) || (internalStateBuffer == NULL_PTR) || (internalStateBufferLength == NULL_PTR) ||
    (reseedCounter == NULL_PTR) || (seedStatus == NULL_PTR) || (modeDRBG == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    if (*internalStateBufferLength < (2u*ESL_HASHDRBG_SHA512_SEED_LEN))
    {
      retVal = ESL_ERC_BUFFER_TOO_SMALL;
    }
    else
    {
      /* If workspace state is unseeded, set output buffers to zero. */
      if (workSpace->wsDRBG.seedState == ESL_DRBG_SEED_STATUS_UNSEEDED)
      {
        actMemClear(internalStateBuffer, (2u* ESL_HASHDRBG_SHA512_SEED_LEN)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        *reseedCounter = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
      }
      else /* If state is seeded (or needs reseed), copy internal state of DRBG to output buffer. */
      {
        actMemCpyByteArray(internalStateBuffer, workSpace->wsDRBG.constant, ESL_HASHDRBG_SHA512_SEED_LEN); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        actMemCpyByteArray(&internalStateBuffer[ESL_HASHDRBG_SHA512_SEED_LEN], workSpace->wsDRBG.variable, ESL_HASHDRBG_SHA512_SEED_LEN); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS_FIXED_SIZE */
        *reseedCounter = workSpace->wsDRBG.reseedCount; /* SBSW_VSECPRIM_WRITE_POINTER */
      }

      *seedStatus = workSpace->wsDRBG.seedState; /* SBSW_VSECPRIM_WRITE_POINTER */
      *modeDRBG = workSpace->wsDRBG.mode; /* SBSW_VSECPRIM_WRITE_POINTER */
      *internalStateBufferLength = 2u*ESL_HASHDRBG_SHA512_SEED_LEN; /* SBSW_VSECPRIM_WRITE_POINTER */
      retVal = ESL_ERC_NO_ERROR;
    }
  }

  return retVal;
}  /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  esl_restoreStateHASHDRBG()
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
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_restoreStateHASHDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHASHDRBG) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) internalStateBuffer,
  eslt_Length internalStateBufferLength,
  eslt_DRBGReseedCounterType reseedCounter,
  eslt_DRBGSeedStatusType seedStatus,
  eslt_DRBGMode modeDRBG)
{
  eslt_ErrorCode retVal;

  if ((workSpace == NULL_PTR) || (internalStateBuffer == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    if (internalStateBufferLength < (2u * ESL_HASHDRBG_SHA512_SEED_LEN))
    {
      retVal = ESL_ERC_INVALID_LENGTH;
    }
    else
    {
      /* restore internal state from inout arguments */
      actMemCpyByteArray(workSpace->wsDRBG.constant, internalStateBuffer, ESL_HASHDRBG_SHA512_SEED_LEN); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      actMemCpyByteArray(workSpace->wsDRBG.variable, &internalStateBuffer[ESL_HASHDRBG_SHA512_SEED_LEN], ESL_HASHDRBG_SHA512_SEED_LEN); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      workSpace->wsDRBG.reseedCount = reseedCounter; /* SBSW_VSECPRIM_WRITE_POINTER */
      workSpace->wsDRBG.seedState = seedStatus; /* SBSW_VSECPRIM_WRITE_POINTER */
      workSpace->wsDRBG.mode = modeDRBG; /* SBSW_VSECPRIM_WRITE_POINTER */

      retVal = ESL_ERC_NO_ERROR;
    }
  }

  return retVal;
}  /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

#endif /* VSECPRIM_HASH_DRBG_ENABLED == STD_ON */

#define VSECPRIM_STOP_SEC_CODE
#include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_DRBG.c
 *********************************************************************************************************************/
