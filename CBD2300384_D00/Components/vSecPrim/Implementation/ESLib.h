/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2018 cv cryptovision GmbH.                                                All rights reserved.
 *
 *  For modifications by Vector Informatik GmbH:
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is protected under intellectual property laws and proprietary to cv cryptovision GmbH
 *                and/or Vector Informatik GmbH.
 *                No right or license is granted save as expressly set out in the applicable license conditions.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  ESLib.h
 *        \brief  Provide service API's for the Module
 *
 *      \details The interface for the standard set of functions.
 *               Currently the actClib version is used.
 *               This file is part of the embedded systems library cvActLib/ES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the ESLib_version.h.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to module's header file.
 *********************************************************************************************************************/

#ifndef ESLIB_H
# define ESLIB_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "ESLib_t.h"
# include "ESLib_ERC.h"
# include "ESLib_RNG.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* PRQA S 0777 EOF */ /* MD_MSR_Rule5.1 */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/*****************************************************************************
 * Library version
 *****************************************************************************/

# ifndef NULL /* COV_VSECPRIM_STD_VALUES */
#  define NULL                                                        ((void*)0)
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

/*****************************************************************************
 * Library initialisation function
 * (must be called before any use of the library)
 *****************************************************************************/
/**********************************************************************************************************************
 *  esl_initESCryptoLib()
 *********************************************************************************************************************/
/*! \brief          This function sets the library to a default value.
 *                  sets LibStatus to some default value
 *                  also initializes Lib internal vars if needed
 *                  on some controlers this function initializes
 *                  the library for multithreading features
 *  \details        -
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initESCryptoLib(void);

/*****************************************************************************
 * Workspace initialisation function
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_initWorkSpaceHeader()
 *********************************************************************************************************************/
/*! \brief       Init workspace header
 *  \details     This function initializes a workSpace header. This function must be called before calling (nearly)
 *               every function from the cv act library/es to initialize the workSpace header correctly.
 *  \param[in,out]  workSpaceHeader  Pointer to workspace header
 *  \param[in]   workSpaceSize  Size of the provided workspace
 *  \param[in]   watchdog  Pointer to watchdog trigger function
 *  \return      ESL_ERC_NO_ERROR - OK
 *               ESL_ERC_PARAMETER_INVALID - Nullpointer check failed
 *  \pre         workspaceHeader must be a valid pointer.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initWorkSpaceHeader(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHeader) workSpaceHeader,
  const eslt_Length workSpaceSize,
  esl_WatchdogFuncPtr watchdog);

# if (VSECPRIM_FIPS186_ENABLED == STD_ON)
/*****************************************************************************
 * FIPS186 functions for a random number generator
 *****************************************************************************/

/**********************************************************************************************************************
 * esl_initFIPS186
 *********************************************************************************************************************/
/*!
 * \brief          Initialize FIPS186-2
 * \details        This function initializes the random number generator according to FIPS-186-2.
 * \param[in,out]  workSpace     (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 * \param[in]      entropyLength The length of the given entropy used as seed for the RNG.
 * \param[in]      entropy       A pointer to the value that has to be used as seed.
 * \param[in,out]  savedState    A pointer to a state that has been saved after the previous
 *                 initialisation. Length is given by ESL_SIZEOF_RNGFIPS186_STATE.
 *                 If this pointer is set to NULL, no savedState is used.
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_RNG_ENTROPY_TOO_SMALL  provided entropy length is to small
 *                 ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW   total input length overflow
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initFIPS186(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceFIPS186) workSpace,
const eslt_Length entropyLength, VSECPRIM_P2CONST_PARA(eslt_Byte) entropy,
VSECPRIM_P2VAR_PARA(eslt_Byte) savedState);

/**********************************************************************************************************************
 * esl_getBytesFIPS186
 *********************************************************************************************************************/
/*!
 * \brief          Get random byte
 * \details        This function generates random numbers according to FIPS-186-2.
 * \param[in,out]  workSpace       (in) algorithm context buffer, initialized by esl_initFIPS186(..) function
 *                                 (out) updated algorithm context structure
 * \param[in]      targetLength    the number of bytes that shall be generated
 * \param[in,out]  target          (in) pointer to output buffer
 *                                 (out) pointer to generated random bytes
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initFIPS186(..)
 * \context        TASK
 * \reentrant      TRUE
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_getBytesFIPS186(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceFIPS186) workSpace,
const eslt_Length targetLength, VSECPRIM_P2VAR_PARA(eslt_Byte) target);

/**********************************************************************************************************************
 * esl_stirFIPS186
 *********************************************************************************************************************/
/*!
 * \brief          Stir up seed state.
 * \details        This function stirs up the internal state of the random number generator.
 * \param[in,out]  workSpace     (in) algorithm context buffer, initialized by esl_initFIPS186(..) function
 *                               (out) updated algorithm context structure
 * \param[in]      inputLength   the number of input bytes used to stir up therandom number generator
 * \param[in,out]  input         pointer to input bytes used to stir up the random number generator
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initFIPS186(..)
 * \context        TASK
 * \reentrant      TRUE
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_stirFIPS186(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceFIPS186) workSpace,
const eslt_Length inputLength, VSECPRIM_P2VAR_PARA(eslt_Byte) input);

# endif /* (VSECPRIM_FIPS186_ENABLED == STD_ON) */

/*****************************************************************************
 * CTR_CRBG functions for a random number generator
 *****************************************************************************/

# if (VSECPRIM_CTR_DRBG_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  esl_initCTRDRBG()
 *********************************************************************************************************************/
/*! \brief         Initializes the DRBG and sets seed status in workspace to ESL_DRBG_SEED_STATE_UNSEEDED.
 *  \details       This function initializes the CTR DRBG AES128 or CTR DRBG AES256, depending on the value of
 *                 'modeDRBG'.
 *  \param[in,out] workSpace   (in)  pointer to the DRBG workSpace
 *                             (out) DRBG workspace, mode member modified
 *  \param[in]     modeDRBG    indication which AES size (128 or 256) the DRBG shall be based on and whether the
 *                             seeding and generating functions shall use a derivation function or not
 *  \return        ESL_ERC_NO_ERROR          initializing successful
 *                 ESL_ERC_PARAMETER_INVALID input parameter workSpace is a null pointer
 *                 ESL_ERC_WS_TOO_SMALL      workSpace too small
 *                 ESL_ERC_MODE_INVALID      DRBG mode invalid
 *  \pre           workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_initCTRDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  eslt_DRBGMode modeDRBG);

/**********************************************************************************************************************
 *  esl_seedCTRDRBG()
 **********************************************************************************************************************/
/*! \brief         Seed function for NIST800-90A CTR DRBG (based on AES) for initial seeding or reseeding.
 *  \details       This function calls esl_initialSeed_CTRDRBG() or esl_reeed_CTRDRBG, depending on the seed status.
 *  \param[in,out] workSpace                     (in)  pointer to the DRBG workspace
 *                                               (out) DRBG workspace, wsAES, key, variable and reseedCounter modified
 *  \param[in]     entropyPtr                    pointer to entropy buffer
 *  \param[in]     entropyLength                 length of the entropy in bytes
 *  \param[in]     noncePtr                      pointer to nonce buffer, possibly a null pointer
 *  \param[in]     nonceLength                   length of the nonce in bytes, possibly 0
 *  \param[in]     personalizationStringPtr      pointer to personalization string buffer, possibly a null pointer
 *  \param[in]     personalizationStringLength   length of the personalization string in bytes, possibly 0
 *  \param[in]     additionalInputPtr            pointer to additional input buffer, possibly a null pointer
 *  \param[in]     additionalInputLength         length of the additional input in bytes, possibly 0
 *  \return        ESL_ERC_NO_ERROR              (re)seeding successful.
 *                 ESL_ERC_PARAMETER_INVALID     an input parameter is a null pointer
 *                 ESL_ERC_WS_TOO_SMALL          workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID      workSpace state invalid
 *                 ESL_ERC_MODE_INVALID          incorrect mode for derivation function
 *                 ESL_ERC_RNG_ENTROPY_TOO_SMALL entropy length too short
 *                 ESL_ERC_INVALID_LENGTH        length of one of the input parameters invalid
 *  \pre           workSpace is a valid pointer and initialized by esl_initWorkSpaceHeader(..).
 *                 esl_initCTRDRBG() was called before.
 *  \note          If no derivation function is used, entropyLength must be exactly ESL_CTRDRBG_AES128_SEEDLEN
 *                 (or ESL_CTRDRBG_AES256_SEEDLEN) bytes. If a derivation function is used, the minimal value
 *                 of entropyLength is vSecPrim_GetCTRDRBGAES128SecurityStrengthOfGeneral() (or 256, for CTR DRBG
 *                 AES256) bytes and the maximal value of entropyLength is (2^32 - 10) bytes.
 *                 noncePtr is a required parameter for initial seeding with derivation function. The maximal length of
 *                 noncePtr is (2^32 - 10) bytes.
 *                 noncePtr is not required if no derivation function is used while seeding initially or if
 *                 esl_seedCTRDRBG is called to reseed the DRBG.
 *                 personalizationStringPtr is an optional parameter for initial seeding, with or without derivation
 *                 function.
 *                 If no derivation function is used, the maximal length of personalizationStringPtr is
 *                 ESL_CTRDRBG_AES128_SEEDLEN (or ESL_CTRDRBG_AES256_SEEDLEN) bytes. If a derivation function is used,
 *                 the maximal length of personalizationStringPtr is (2^32 - 10) bytes.
 *                 additionalInputPtr is an optional parameter for reseeding, with or without derivation function.
 *                 If no derivation function is used, the maximal length of additionalInputPtr is
 *                 ESL_CTRDRBG_AES128_SEEDLEN (or ESL_CTRDRBG_AES256_SEEDLEN) bytes. If no derivation function is
 *                 used, the maximal length of additionalInputPtr is (2^32 - 10) bytes.
 *  \context       TASK
 *  \reentrant     TRUE, for different workSpaces
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_seedCTRDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) entropyPtr,
  eslt_Length entropyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) noncePtr,
  eslt_Length nonceLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) personalizationStringPtr,
  eslt_Length personalizationStringLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) additionalInputPtr,
  eslt_Length additionalInputLength);

/**********************************************************************************************************************
 *  esl_getBytesCTRDRBG()
 **********************************************************************************************************************/
/*! \brief         Generates requestedLength many random bytes using the CTR DRBG based on AES.
 *  \details        -
 *  \param[in,out] workSpace              (in)  pointer to the DRBG workspace
 *                                        (out) DRBG workspace, wsAES, key, variable and reseedCounter modified
 *  \param[in]     requestedLength        number of random bytes to be generated
 *  \param[in,out] outputBuffer           (in)  Pointer to the output buffer
 *                                        (out) output buffer, containing requestedLength random bytes
 *  \param[in]     additionalInputPtr     pointer to additional input buffer, possibly a null pointer
 *  \param[in]     additionalInputLength  length of the additional input in bytes, possibly 0
 *  \return         ESL_ERC_NO_ERROR                  Request was successful.
 *                  ESL_ERC_PARAMETER_INVALID         workSpace or outputBuffer is a null pointer
 *                  ESL_ERC_WS_TOO_SMALL              workSpace too small
 *                  ESL_ERC_WS_STATE_INVALID          invalid workspace state
 *                  ESL_ERC_MODE_INVALID              incorrect mode for derivation function
 *                  ESL_ERC_REQUESTED_LENGTH_INVALID  requested length greater than maximal DRBG output size
 *                  ESL_ERC_INVALID_LENGTH            length of additional input invalid
 *                  ESL_ERC_ENTROPY_EXHAUSTION        Entropy is exhausted or DRBG not seeded. Reseed is necessary.
 *  \pre            workSpace must be initialized by calling esl_initCTRDRBG(). The DRBG has to be seeded by calling
 *                  esl_seedCTRDRBG() or esl_restoreStateCTRDRBG().
 *                  The buffer referenced by outputBuffer must provide at least requestedLength bytes.
 *  \note           additionalInputPtr is an optional parameter with and without derivation function. If no derivation
 *                  function is used, the maximal length of additionalInputPtr is ESL_CTRDRBG_AES128_SEEDLEN
 *                  (or ESL_CTRDRBG_AES256_SEEDLEN) bytes. If a derivation function is used, the maximal length
 *                  of additionalInputPtr is (2^32 - 10) bytes.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_getBytesCTRDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  eslt_Length requestedLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) outputBuffer,
  VSECPRIM_P2CONST_PARA(eslt_Byte) additionalInputPtr,
  eslt_Length additionalInputLength);

/**********************************************************************************************************************
 *  esl_getStateCTRDRBG()
 **********************************************************************************************************************/
/*! \brief         Stores key, variable, reseedCount, seed status and DRBG mode in provided output buffers.
 *  \details       If the seed status is UNSEEDED, the values in the output buffers are set to 0.
 *                 key and variable are both stored in internalStateBuffer.
 *  \param[in]     workSpace                 pointer to the DRBG workspace
 *  \param[in,out] internalStateBuffer       (in)  Pointer to the output buffer, where [key, V] will be stored
 *                                           (out) output buffer, containing [key, V]
 *  \param[in,out] internalStateBufferLength (in)  length of internalStateBuffer in bytes
 *                                           (out) number of bytes written to internalStateBuffer
 *  \param[in,out] reseedCounter             (in)  pointer to output buffer for the reseed counter
 *                                           (out) output buffer for the reseed counter, containing the value of
 *                                                 the reseed counter
 *  \param[in,out] seedStatus                (in)  pointer to output buffer for the seed status, saved in the workspace
 *                                                 header
 *                                           (out) output buffer for the seed status, containing the current seed status
 *  \param[in,out] modeDRBG                  (in)  pointer to output buffer for the DRBG mode
 *                                           (out) output buffer containing DRBG mode
 *  \return         ESL_ERC_NO_ERROR            Request was successful.
 *                  ESL_ERC_PARAMETER_INVALID   input parameter is a null pointer
 *                  ESL_ERC_BUFFER_TOO_SMALL    internalStateBufferLength too short
 *  \pre            workSpace must be initialized by calling esl_initCTRDRBG().
 *                  The buffer referenced by internalStateBuffer must provide at least internalStateBufferLength bytes.
 *  \note           For CTR DRBG AES128, internalStateBuffer must provide at least ESL_CTRDRBG_AES128_SEEDLEN bytes.
 *                  For CTR DRBG AES256, internalStateBuffer must provide at least ESL_CTRDRBG_AES256_SEEDLEN bytes.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_getStateCTRDRBG(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) internalStateBuffer,
  VSECPRIM_P2VAR_PARA(eslt_Length) internalStateBufferLength,
  VSECPRIM_P2VAR_PARA(eslt_DRBGReseedCounterType) reseedCounter,
  VSECPRIM_P2VAR_PARA(eslt_DRBGSeedStatusType) seedStatus,
  VSECPRIM_P2VAR_PARA(eslt_DRBGMode) modeDRBG);

/**********************************************************************************************************************
 *  esl_restoreStateCTRDRBG()
 **********************************************************************************************************************/
/*! \brief         Restores internal state of DRBG from provided input values.
 *  \details       Stores the values from internalStateBuffer, reseedCounter, seedStatus and DRBGmode in
 *                 workSpace->wsDRBG.key, workSpace->wsDRBG.variable, workSpace->wsDRBG.reseedCount,
 *                 workSpace->wsDRBG.mode and workSpace->wsDRBG.seedState
 *  \param[in,out] workSpace                 (in)  pointer to the DRBG workspace
 *                                           (out) DRBG workspace, key, variable, reseedCount, seed status
 *                                                 and mode modified
 *  \param[in]     internalStateBuffer       Pointer to the buffer containing [key, V]
 *  \param[in]     internalStateBufferLength length of internalStateBuffer in bytes
 *  \param[in]     reseedCounter             value of reseed counter to be restored
 *  \param[in]     seedStatus                current seed status to be restored
 *  \param[in]     modeDRBG                  DRBG mode data (algorithm size, DF mode)
 *  \return        ESL_ERC_NO_ERROR            Request was successful.
 *                 ESL_ERC_PARAMETER_INVALID   input parameter is a null pointer
 *                 ESL_ERC_INVALID_LENGTH      internalStateBufferLength too high
 *  \pre           workSpace must be initialized by calling esl_initCTRDRBG().
 *  \note          For CTR DRBG AES128, internalStateBuffer must provide exactly ESL_CTRDRBG_AES128_SEEDLEN bytes.
 *                 For CTR DRBG AES256, internalStateBuffer must provide exactly ESL_CTRDRBG_AES256_SEEDLEN bytes.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_restoreStateCTRDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCTRDRBG) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) internalStateBuffer,
  eslt_Length internalStateBufferLength,
  eslt_DRBGReseedCounterType reseedCounter,
  eslt_DRBGSeedStatusType seedStatus,
  eslt_DRBGMode modeDRBG);
# endif /* VSECPRIM_CTR_DRBG_ENABLED == STD_ON */

# if (VSECPRIM_HASH_DRBG_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  esl_initHASHDRBG()
 *********************************************************************************************************************/
/*! \brief         Initializes the DRBG and sets seed status in workspace to ESL_DRBG_SEED_STATE_UNSEEDED.
 *  \details       This function initializes the DRBG by setting the DRBG mode.
 *  \param[in,out] workSpace   (in)  pointer to the DRBG workSpace
 *                             (out) DRBG workspace, mode member modified
 *  \param[in]     modeDRBG    indication which SHA size the DRBG shall be based on
 *  \return        ESL_ERC_NO_ERROR          initializing successful
 *                 ESL_ERC_PARAMETER_INVALID input parameter workSpace is a null pointer
 *                 ESL_ERC_WS_TOO_SMALL      workSpace too small
 *                 ESL_ERC_MODE_INVALID      DRBG mode invalid
 *  \pre           workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_initHASHDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHASHDRBG) workSpace,
  eslt_DRBGMode modeDRBG);

/**********************************************************************************************************************
 *  esl_seedHASHDRBG()
 **********************************************************************************************************************/
/*! \brief         Seed function for NIST800-90A Hash DRBG (based on SHA2-512) for initial seeding or reseeding.
 *  \details       This function calls esl_initialSeed_HASHDRBG() or esl_reeed_HASHDRBG, depending on the seed status.
 *  \param[in,out] workSpace                     (in)  pointer to the DRBG workspace
 *                                               (out) DRBG workspace, wsSHA512, constant, variable and reseedCounter
 *                                                     modified
 *  \param[in]     entropyPtr                    pointer to entropy buffer
 *  \param[in]     entropyLength                 length of the entropy in bytes
 *  \param[in]     noncePtr                      pointer to nonce buffer, possibly a null pointer
 *  \param[in]     nonceLength                   length of the nonce in bytes, possibly 0
 *  \param[in]     personalizationStringPtr      pointer to personalization string buffer, possibly a null pointer
 *  \param[in]     personalizationStringLength   length of the personalization string in bytes, possibly 0
 *  \param[in]     additionalInputPtr            pointer to additional input buffer, possibly a null pointer
 *  \param[in]     additionalInputLength         length of the additional input in bytes, possibly 0
 *  \return        ESL_ERC_NO_ERROR              (re)seeding successful.
 *                 ESL_ERC_PARAMETER_INVALID     an input parameter is a null pointer
 *                 ESL_ERC_WS_TOO_SMALL          workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID      workSpace state invalid
 *                 ES_ERC_MODE_INVALID           incorrect DRBG mode
 *                 ESL_ERC_RNG_ENTROPY_TOO_SMALL entropy length too short
 *  \pre           workSpace is a valid pointer and initialized by esl_initWorkSpaceHeader(..).
 *                 esl_initHASHDRBG() was called before.
 *  \note          The minimal value of entropyLength is 256 bits and the maximal value of entropyLength
 *                 is (2^32 - 1) bytes.
 *                 noncePtr is a required parameter for initial seeding. The maximal length of
 *                 noncePtr is (2^32 - 1) bytes.
 *                 personalizationStringPtr is an optional parameter for initial seeding. The maximal length of
 *                 personalizationStringPtr is (2^32 - 1) bytes.
 *                 additionalInputPtr is an optional parameter for reseeding. The maximal length of
 *                 additionalInputPtr is (2^32 - 1) bytes.
 *  \context       TASK
 *  \reentrant     TRUE, for different workSpaces
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_seedHASHDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHASHDRBG) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) entropyPtr,
  eslt_Length entropyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) noncePtr,
  eslt_Length nonceLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) personalizationStringPtr,
  eslt_Length personalizationStringLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) additionalInputPtr,
  eslt_Length additionalInputLength);

/**********************************************************************************************************************
 *  esl_getBytesHASHDRBG()
 **********************************************************************************************************************/
/*! \brief         Generates requestedLength many random bytes using the Hash DRBG based on SHA2-512.
 *  \details        -
 *  \param[in,out] workSpace              (in)  pointer to the DRBG workspace
 *                                        (out) DRBG workspace, wsSHA512, constant, variable and reseedCounter modified
 *  \param[in]     requestedLength        number of random bytes to be generated
 *  \param[in,out] outputBuffer           (in)  Pointer to the output buffer
 *                                        (out) output buffer, containing requestedLength random bytes
 *  \param[in]     additionalInputPtr     pointer to additional input buffer, possibly a null pointer
 *  \param[in]     additionalInputLength  length of the additional input in bytes, possibly 0
 *  \return         ESL_ERC_NO_ERROR                     Request was successful.
 *                  ESL_ERC_PARAMETER_INVALID            workSpace or outputBuffer is a null pointer
 *                  ESL_ERC_WS_TOO_SMALL                 workSpace too small
 *                  ESL_ERC_WS_STATE_INVALID             invalid workspace state
 *                  ESL_ERC_RNG_REQUESTED_LENGTH_INVALID requested length greater than maximal DRBG output size
 *                  ESL_ERC_ENTROPY_EXHAUSTION           Entropy is exhausted or DRBG not seeded. Reseed is necessary.
 *  \pre            workSpace must be initialized by calling esl_initHASHDRBG(). The DRBG has to be seeded by calling
 *                  esl_seedHASHDRBG() or esl_restoreStateHASHDRBG().
 *                  The buffer referenced by outputBuffer must provide at least requestedLength bytes.
 *  \note           additionalInputPtr is an optional parameter. The maximal length of additionalInputPtr
 *                  is (2^32 - 1) bytes.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_getBytesHASHDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHASHDRBG) workSpace,
  eslt_Length requestedLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) outputBuffer,
  VSECPRIM_P2CONST_PARA(eslt_Byte) additionalInputPtr,
  eslt_Length additionalInputLength);

/**********************************************************************************************************************
 *  esl_getStateHASHDRBG()
 **********************************************************************************************************************/
/*! \brief         Stores constant, variable, reseedCount, seed status and DRBG mode in provided output buffers.
 *  \details       If the seed status is UNSEEDED, the values in the output buffers are set to 0.
 *                 constant and variable are both stored in internalStateBuffer.
 *  \param[in]     workSpace                 pointer to the DRBG workspace
 *  \param[in,out] internalStateBuffer       (in)  Pointer to the output buffer, where [constant, V] will be stored
 *                                           (out) output buffer, containing [constant, V]
 *  \param[in,out] internalStateBufferLength (in)  length of internalStateBuffer in bytes
 *                                           (out) number of bytes written to internalStateBuffer
 *  \param[in,out] reseedCounter             (in)  pointer to output buffer for the reseed counter
 *                                           (out) output buffer for the reseed counter, containing the value of
 *                                                 the reseed counter
 *  \param[in,out] seedStatus                (in)  pointer to output buffer for the seed status, saved in the workspace
 *                                                 header
 *                                           (out) output buffer for the seed status, containing the current seed status
 *  \param[in,out] modeDRBG                  (in)  pointer to output buffer for the DRBG mode
 *                                           (out) output buffer containing DRBG mode
 *  \return         ESL_ERC_NO_ERROR            Request was successful.
 *                  ESL_ERC_PARAMETER_INVALID   input parameter is a null pointer
 *                  ESL_ERC_BUFFER_TOO_SMALL    internalStateBufferLength too short
 *  \pre            workSpace must be initialized by calling esl_initCTRDRBG().
 *                  The buffer referenced by internalStateBuffer must provide at least internalStateBufferLength bytes.
 *  \note           For Hash DRBG SHA2-512, internalStateBuffer must provide at least
 *                  2*ESL_HASHDRBG_SHA512_SEED_LEN bytes.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_getStateHASHDRBG(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceHASHDRBG) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) internalStateBuffer,
  VSECPRIM_P2VAR_PARA(eslt_Length) internalStateBufferLength,
  VSECPRIM_P2VAR_PARA(eslt_DRBGReseedCounterType) reseedCounter,
  VSECPRIM_P2VAR_PARA(eslt_DRBGSeedStatusType) seedStatus,
  VSECPRIM_P2VAR_PARA(eslt_DRBGMode) modeDRBG);

/**********************************************************************************************************************
 *  esl_restoreStateHASHDRBG()
 **********************************************************************************************************************/
/*! \brief         Restores internal state of DRBG from provided input values.
 *  \details       Stores the values from internalStateBuffer, reseedCounter, seedStatus and DRBGmode in
 *                 workSpace->wsDRBG.constant, workSpace->wsDRBG.variable, workSpace->wsDRBG.reseedCount,
 *                 workSpace->wsDRBG.mode and workSpace->wsDRBG.seedState
 *  \param[in,out] workSpace                 (in)  pointer to the DRBG workspace
 *                                           (out) DRBG workspace, constant, variable, reseedCount, seed status
 *                                                 and mode modified
 *  \param[in]     internalStateBuffer       Pointer to the buffer containing [constant, V]
 *  \param[in]     internalStateBufferLength length of internalStateBuffer in bytes
 *  \param[in]     reseedCounter             value of reseed counter to be restored
 *  \param[in]     seedStatus                current seed status to be restored
 *  \param[in]     modeDRBG                  DRBG mode data (algorithm size, DF mode)
 *  \return        ESL_ERC_NO_ERROR            Request was successful.
 *                 ESL_ERC_PARAMETER_INVALID   input parameter is a null pointer
 *                 ESL_ERC_INVALID_LENGTH      internalStateBufferLength too little
 *  \pre           workSpace must be initialized by calling esl_initHASHDRBG().
 *  \note          For Hash DRBG SHA2-512, internalStateBuffer must provide at least
 *                 2*ESL_HASHDRBG_SHA512_SEED_LEN bytes.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_restoreStateHASHDRBG(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHASHDRBG) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) internalStateBuffer,
  eslt_Length internalStateBufferLength,
  eslt_DRBGReseedCounterType reseedCounter,
  eslt_DRBGSeedStatusType seedStatus,
  eslt_DRBGMode modeDRBG);

# endif /* VSECPRIM_HASH_DRBG_ENABLED == STD_ON */

/*****************************************************************************
 * AES functions supporting different modes
 *****************************************************************************/

# if (VSECPRIM_AES128_ENABLED == STD_ON)

/**********************************************************************************************************************
 * esl_initEncryptAES128
 *********************************************************************************************************************/
/*!
 * \brief          Init AES128 encryption.
 * \details        This function initializes the AES128 stream encryption algorithm.
 * \param[in,out]  workSpace    (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                              (out) initialized algorithm context structure
 * \param[in]      key          pointer to user key of length ESL_SIZEOF_AES128_KEY
 * \param[in]      blockMode    ECB, CBC, CTR, etc. block mode switch
 * \param[in]      paddingMode  padding mode switch
 * \param[in]      initializationVector in CBC mode: pointer to initialization vector of length ESL_SIZEOF_AES128_BLOCK
 *                                      in CTR mode: pointer to counter of length ESL_SIZEOF_AES128_BLOCK
 *                                      if initializationVector==NULL, a trivial zero block is used as iv or counter
 *                                      redundant in ECB mode
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptAES128(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES128) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key,
  const eslt_BlockMode blockMode, const eslt_PaddingMode paddingMode,
  VSECPRIM_P2CONST_PARA(eslt_Byte) initializationVector);

/**********************************************************************************************************************
 * esl_encryptAES128
 *********************************************************************************************************************/
/*!
 * \brief          This function encrypts input data of any length and can be called
 *                 arbitrarily often after the algorithm initialization.
 * \details        Exact (inputSize+buffered_bytes)/16 blocks will be encrypted and written to the output buffer,
 *                 (inputSize+buffered_bytes)%16 remaining bytes will be buffered, where 0 <= buffered_bytes < 16.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initEncryptAES128(..) function
 *                             (out) updated algorithm context structure
 * \param[in]      inputSize   length of input data in bytes
 * \param[in]      input       pointer to data to be encrypted
 * \param[in,out]  outputSize  (in) output buffer size in bytes, a safe length is  ((inputSize+15)/16)*16  (see output)
 *                             (out) total size of encrypted blocks  ((inputSize+buffered_bytes)/16)*16
 * \param[in,out]   output     (in) pointer to encrypted data buffer
 *                             (out) pointer to encrypted data blocks
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initEncryptAES128(..)
 * \note           input and output can point to the same address, i.e., inplace encryption is possible for ECB, CBC
 *                 and CTR mode, independent from the chosen padding method
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptAES128(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES128) workSpace,
  const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input,
  VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 * esl_finalizeEncryptAES128
 *********************************************************************************************************************/
/*!
 * \brief          Finalize AES 128 encrypt
 * \details        This function finalizes the AES encryption by proccessing the remaining input bytes in the
 *                 internal buffer and padding the input according to the chosen padding in the corresponding
 *                 init function.
 * \param[in,out]  workSpace    (in) algorithm context structure
 *                              (out) updated algorithm context structure
 * \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is  2*AES_BLOCK_LENGTH
 *                              (out) total size of encrypted input  16 or 32 bytes
 * \param[in,out]  output       (in) pointer to encrypted data buffer
 *                              (out) pointer to encrypted data blocks
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initEncryptAES128(..)
 * \pre            esl_encryptAES128 called before
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeEncryptAES128(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES128) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 * esl_initDecryptAES128
 *********************************************************************************************************************/
/*!
 * \brief          Init AES128 decryption
 * \details        This function initializes the AES stream decryption algorithm.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                             (out) initialized algorithm context structure
 * \param[in]      key         pointer to user key of length ESL_SIZEOF_AES128_KEY
 * \param[in]      blockMode   ECB, CBC, CTR, etc. block mode switch
 * \param[in]      paddingMode padding mode switch
 * \param[in]      initializationVector in CBC mode: pointer to initialization vector of length ESL_SIZEOF_AES128_BLOCK
 *                                      in CTR mode: pointer to counter of length ESL_SIZEOF_AES128_BLOCK
 *                                      if initializationVector==NULL, a trivial zero block is used as iv or counter
 *                                      redundant in ECB mode
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptAES128(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES128) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key,
  const eslt_BlockMode blockMode, const eslt_PaddingMode paddingMode,
  VSECPRIM_P2CONST_PARA(eslt_Byte) initializationVector);

/**********************************************************************************************************************
 * esl_decryptAES128
 *********************************************************************************************************************/
/*!
 * \brief          This function decrypts input data of any length and can be called arbitrarily often
 *                 after the algorithm initialization.
 * \details        Exact (inputSize+buffered_bytes)/16 blocks will be decrypted and written to the output buffer,
 *                 (inputSize+buffered_bytes)%16 remaining bytes will be buffered, where 0 <= buffered_bytes < 16.
 * \param[in,out]  workSpace    (in) algorithm context buffer, initialized by esl_initDecryptAES128(..) function
 *                              (out) updated algorithm context structure
 * \param[in]      inputSize    length of input data in bytes
 * \param[in]      input        pointer to data to be encrypted
 * \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is  ((inputSize+15)/16)*16  (see output)
 *                              (out) total size of encrypted blocks  ((inputSize+buffered_bytes)/16)*16
 * \param[in,out]  output       (in) pointer to encrypted data buffer
 *                              (out) pointer to encrypted data blocks
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initDecryptAES128(..)
 * \note           input and output can point to the same address, i.e., inplace decryption is possible for ECB, CBC
 *                 and CTR mode, independent from the chosen padding method
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptAES128(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES128) workSpace,
  const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input,
  VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 * esl_finalizeDecryptAES128
 *********************************************************************************************************************/
/*!
 * \brief          Finalize AES 128 decrypt
 * \details        This function finalizes the AES decryption by proccessing the remaining input bytes in the
 *                 internal buffer.
 * \param[in,out]  workSpace   (in) algorithm context structure
 *                             (out) updated algorithm context structure
 * \param[in,out]  outputSize  (in) output buffer size in bytes, a safe length is  AES_BLOCK_LENGTH
 *                             (out) total size of decryted input  16 or 32 bytes
 * \param[in,out]  output      (in) pointer to decrypted data buffer
 *                             (out) pointer to encrypted data blocks
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                 ESL_ERC_AES_PADDING_INVALID  the padding is invalid
 *                 ESL_ERC_INPUT_INVALID        the total input length is not zero mod blocksize
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initDecryptAES128(..)
 * \pre            esl_decryptAES128 called before
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeDecryptAES128(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES128) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 * esl_initEncryptAES128Block
 *********************************************************************************************************************/
/*!
 * \brief          Init AES128 block decryption
 * \details        This function initializes the AES block encryption algorithm.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                             (out) initialized algorithm context structure
 * \param[in]      key         pointer to user key of length ESL_SIZEOF_AES128_KEY
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptAES128Block(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES128Block) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 * esl_encryptAES128Block
 *********************************************************************************************************************/
/*!
 * \brief          Encrypt AES Block
 * \details        This function encrypts an AES Block (= 16 bytes)
 * \param[in,out]  workSpace     (in) algorithm context buffer, initialized by esl_initEncryptAES128Block(..) function
 *                               (out) updated algorithm context structure
 * \param[in]      inputBlock    pointer to the given input block (16 bytes)
 * \param[in,out]  outputBlock   (in) pointer to output buffer (at least 16 bytes)
 *                               (out) pointer to encrypted data block
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initEncryptAES128Block(..)
 * \note           inputBlock and outputBlock can point to the same address, i.e., inplace encryption is possible
 * \context        TASK
 * \reentrant      TRUE
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptAES128Block(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES128Block) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) inputBlock, VSECPRIM_P2VAR_PARA(eslt_Byte) outputBlock);

/**********************************************************************************************************************
 * esl_initDecryptAES128Block
 *********************************************************************************************************************/
/*!
 * \brief          Decrypt AES 128 block.
 * \details        This function initializes the AES block decryption algorithm.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                             (out) initialized algorithm context structure
 * \param[in]      key         pointer to user key of length ESL_SIZEOF_AES128_KEY
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptAES128Block(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES128Block) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 * esl_decryptAES128Block
 *********************************************************************************************************************/
/*!
 * \brief          Decrypt AES block
 * \details        This function decrypts an AES Block (= 16 bytes)
 * \param[in]      workSpace     (in) algorithm context buffer, initialized by esl_initDecryptAES128Block(..) function
 *                               (out) updated algorithm context structure
 * \param[in]      inputBlock    pointer to the given input block (16 bytes)
 * \param[in,out]  outputBlock   (in) pointer to output buffer (at least 16 bytes)
 *                               (out) pointer to encrypted data block
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initDecryptAES128Block(..)
 * \note           inputBlock and outputBlock can point to the same address, i.e., inplace decryption is possible
 * \context        TASK
 * \reentrant      TRUE
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptAES128Block(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceAES128Block) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) inputBlock, VSECPRIM_P2VAR_PARA(eslt_Byte) outputBlock);
# endif /* VSECPRIM_AES128_ENABLED */

# if (VSECPRIM_AES192_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/**********************************************************************************************************************
 * esl_initEncryptAES192
 *********************************************************************************************************************/
/*!
 * \brief          Init AES192 encryption.
 * \details        This function initializes the AES192 stream encryption algorithm.
 * \param[in,out]  workSpace    (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                              (out) initialized algorithm context structure
 * \param[in]      key          pointer to user key of length ESL_SIZEOF_AES192_KEY
 * \param[in]      blockMode    ECB, CBC, CTR, etc. block mode switch
 * \param[in]      paddingMode  padding mode switch
 * \param[in]      initializationVector in CBC mode: pointer to initialization vector of length ESL_SIZEOF_AES192_BLOCK
 *                                      in CTR mode: pointer to counter of length ESL_SIZEOF_AES192_BLOCK
 *                                      if initializationVector==NULL, a trivial zero block is used as iv or counter
 *                                      redundant in ECB mode
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptAES192(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES192) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key,
  const eslt_BlockMode blockMode, const eslt_PaddingMode paddingMode,
  VSECPRIM_P2CONST_PARA(eslt_Byte) initializationVector);

/**********************************************************************************************************************
 *  esl_encryptAES192()
 *********************************************************************************************************************/
/*! \brief          This function encrypts input data of any length and can be called
 *                  arbitrarily often after the algorithm initialization.
 *                  Exact (inputSize+buffered_bytes)/16 blocks will be encrypted
 *                  and written to the output buffer, (inputSize+buffered_bytes)%16
 *                  remaining bytes will be buffered, where 0 <= buffered_bytes < 16.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initEncryptAES192(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputSize    length of input data in bytes
 *  \param[in]      input        pointer to data to be encrypted
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               ((inputSize+15)/16)*16       (see output)
 *                               (out) total size of encrypted blocks
 *                               ((inputSize+buffered_bytes)/16)*16
 *  \param[in,out]  output       (in) pointer to encrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initEncryptAES192(..)
 *  \note           input and output can point to the same address, i.e., inplace encryption is possible for ECB, CBC
 *                  and CTR mode, independent from the chosen padding method
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptAES192(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES192) workSpace,
  const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input,
  VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 *  esl_finalizeEncryptAES192()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the AES encryption by proccessing the
 *                  remaining input bytes in the internal buffer and padding the
 *                  input according to the chosen padding in the corresponding
 *                  init function.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initEncryptAES192(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               2*AES_BLOCK_LENGTH
 *                               (out) total size of encrypted input
 *                               16 or 32 bytes
 *  \param[in,out]  output       (in) pointer to encrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initEncryptAES192(..)
 *                  esl_encryptAES192 called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeEncryptAES192(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES192) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 *  esl_initDecryptAES192()
 *********************************************************************************************************************/
/*! \brief          This function initializes the AES stream decryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key of length ESL_SIZEOF_AES192_KEY
 *  \param[in]      blockMode    ECB, CBC, CTR, etc. block mode switch
 *  \param[in]      paddingMode  padding mode switch
 * \param[in]      initializationVector in CBC mode: pointer to initialization vector of length ESL_SIZEOF_AES192_BLOCK
 *                                      in CTR mode: pointer to counter of length ESL_SIZEOF_AES192_BLOCK
 *                                      if initializationVector==NULL, a trivial zero block is used as iv or counter
 *                                      redundant in ECB mode
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptAES192(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES192) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key,
  const eslt_BlockMode blockMode, const eslt_PaddingMode paddingMode,
  VSECPRIM_P2CONST_PARA(eslt_Byte) initializationVector);

/**********************************************************************************************************************
 *  esl_decryptAES192()
 *********************************************************************************************************************/
/*! \brief          This function decrypts input data of any length and can be called
 *                  arbitrarily often after the algorithm initialization.
 *                  Exact (inputSize+buffered_bytes)/16 blocks will be decrypted
 *                  and written to the output buffer, (inputSize+buffered_bytes)%16
 *                  remaining bytes will be buffered, where 0 <= buffered_bytes < 16.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initDecryptAES192(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputSize    length of input data in bytes
 *  \param[in]      input        pointer to data to be encrypted
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               ((inputSize+15)/16)*16       (see output)
 *                               (out) total size of encrypted blocks
 *                               ((inputSize+buffered_bytes)/16)*16
 *  \param[in,out]  output       (in) pointer to encrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initDecryptAES192(..)
 *  \note           input and output can point to the same address, i.e., inplace decryption is possible for ECB, CBC
 *                  and CTR mode, independent from the chosen padding method
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptAES192(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES192) workSpace,
  const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input,
  VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 *  esl_finalizeDecryptAES192()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the AES decryption by proccessing the
 *                  remaining input bytes in the internal buffer.
 *  \details        -
 *  \param[out]     workSpace    actualized algorithm context structure
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               AES_BLOCK_LENGTH
 *                               (out) total size of decryted input
 *                               16 or 32 bytes
 *  \param[in,out]  output       (in) pointer to decrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_AES_PADDING_INVALID  the padding is invalid
 *                  ESL_ERC_INPUT_INVALID        the total input length is not zero mod blocksize
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initDecryptAES192(..)
 *                  esl_decryptAES192 called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeDecryptAES192(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES192) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 *  esl_initEncryptAES192Block()
 *********************************************************************************************************************/
/*! \brief          This function initializes the AES block encryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key of length ESL_SIZEOF_AES192_KEY
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptAES192Block(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES192Block) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 *  esl_encryptAES192Block()
 *********************************************************************************************************************/
/*! \brief          This function encrypts an AES Block (= 16 bytes)
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initEncryptAES192Block(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputBlock   pointer to the given input block (16 bytes)
 *  \param[in,out]  outputBlock  (in) pointer to output buffer (at least 16 bytes)
 *                               (out) pointer to encrypted data block
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initEncryptAES192Block(..)
 *  \note           inputBlock and outputBlock can point to the same address, i.e., inplace encryption is possible
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptAES192Block(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES192Block) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) inputBlock, VSECPRIM_P2VAR_PARA(eslt_Byte) outputBlock);

/**********************************************************************************************************************
 *  esl_initDecryptAES192Block()
 *********************************************************************************************************************/
/*! \brief          This function initializes the AES block decryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key of length ESL_SIZEOF_AES192_KEY
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptAES192Block(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES192Block) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 *  esl_decryptAES192Block()
 *********************************************************************************************************************/
/*! \brief          This function decrypts an AES Block (= 16 bytes)
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initDecryptAES192Block(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputBlock   pointer to the given input block (16 bytes)
 *  \param[in,out]  outputBlock  (in) pointer to output buffer (at least 16 bytes)
 *                               (out) pointer to encrypted data block
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initDecryptAES192Block(..)
 *  \note           inputBlock and outputBlock can point to the same address, i.e., inplace decryption is possible
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptAES192Block(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES192Block) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) inputBlock, VSECPRIM_P2VAR_PARA(eslt_Byte) outputBlock);

# endif /* VSECPRIM_AES192_ENABLED */

# if (VSECPRIM_AES256_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/**********************************************************************************************************************
 * esl_initEncryptAES256
 *********************************************************************************************************************/
/*!
 * \brief          Init AES256 encryption.
 * \details        This function initializes the AES256 stream encryption algorithm.
 * \param[in,out]  workSpace    (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                              (out) initialized algorithm context structure
 * \param[in]      key          pointer to user key of length ESL_SIZEOF_AES256_KEY
 * \param[in]      blockMode    ECB, CBC, CTR, etc. block mode switch
 * \param[in]      paddingMode  padding mode switch
 * \param[in]      initializationVector in CBC mode: pointer to initialization vector of length ESL_SIZEOF_AES256_BLOCK
 *                                      in CTR mode: pointer to counter of length ESL_SIZEOF_AES256_BLOCK
 *                                      if initializationVector==NULL, a trivial zero block is used as iv or counter
 *                                      redundant in ECB mode
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptAES256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES256) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key,
  const eslt_BlockMode blockMode, const eslt_PaddingMode paddingMode,
  VSECPRIM_P2CONST_PARA(eslt_Byte) initializationVector);

/**********************************************************************************************************************
 *  esl_encryptAES256()
 *********************************************************************************************************************/
/*! \brief          This function encrypts input data of any length and can be called
 *                  arbitrarily often after the algorithm initialization.
 *                  Exact (inputSize+buffered_bytes)/16 blocks will be encrypted
 *                  and written to the output buffer, (inputSize+buffered_bytes)%16
 *                  remaining bytes will be buffered, where 0 <= buffered_bytes < 16.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initEncryptAES256(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputSize    length of input data in bytes
 *  \param[in]      input        pointer to data to be encrypted
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               ((inputSize+15)/16)*16       (see output)
 *                               (out) total size of encrypted blocks
 *                               ((inputSize+buffered_bytes)/16)*16
 *  \param[in,out]  output       (in) pointer to encrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initEncryptAES256(..)
 *  \note           input and output can point to the same address, i.e., inplace encryption is possible for ECB, CBC
 *                  and CTR mode, independent from the chosen padding method
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptAES256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES256) workSpace,
  const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input,
  VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 *  esl_finalizeEncryptAES256()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the AES encryption by proccessing the
 *                  remaining input bytes in the internal buffer and padding the
 *                  input according to the chosen padding in the corresponding
 *                  init function.
 *  \details        -
 *  \param[out]     workSpace    actualized algorithm context structure
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               2*AES_BLOCK_LENGTH
 *                               (out) total size of encrypted input
 *                               16 or 32 bytes
 *  \param[in,out]  output       (in) pointer to encrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initEncryptAES256(..)
 *                  esl_encryptAES256 called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeEncryptAES256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES256) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 *  esl_initDecryptAES256()
 *********************************************************************************************************************/
/*! \brief          This function initializes the AES stream decryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key of length ESL_SIZEOF_AES256_KEY
 *  \param[in]      blockMode    ECB, CBC, CTR, etc. block mode switch
 *  \param[in]      paddingMode  padding mode switch
 * \param[in]      initializationVector in CBC mode: pointer to initialization vector of length ESL_SIZEOF_AES256_BLOCK
 *                                      in CTR mode: pointer to counter of length ESL_SIZEOF_AES256_BLOCK
 *                                      if initializationVector==NULL, a trivial zero block is used as iv or counter
 *                                      redundant in ECB mode
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptAES256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES256) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key,
  const eslt_BlockMode blockMode, const eslt_PaddingMode paddingMode,
  VSECPRIM_P2CONST_PARA(eslt_Byte) initializationVector);

/**********************************************************************************************************************
 *  esl_decryptAES256()
 *********************************************************************************************************************/
/*! \brief          This function decrypts input data of any length and can be called
 *                  arbitrarily often after the algorithm initialization.
 *                  Exact (inputSize+buffered_bytes)/16 blocks will be decrypted
 *                  and written to the output buffer, (inputSize+buffered_bytes)%16
 *                  remaining bytes will be buffered, where 0 <= buffered_bytes < 16.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initDecryptAES256(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputSize    length of input data in bytes
 *  \param[in]      input        pointer to data to be encrypted
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               ((inputSize+15)/16)*16       (see output)
 *                               (out) total size of encrypted blocks
 *                               ((inputSize+buffered_bytes)/16)*16
 *  \param[in,out]  output       (in) pointer to encrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initDecryptAES256(..)
 *  \note           input and output can point to the same address, i.e., inplace decryption is possible for ECB, CBC
 *                  and CTR mode, independent from the chosen padding method
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptAES256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES256) workSpace,
  const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input,
  VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 *  esl_finalizeDecryptAES256()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the AES decryption by proccessing the
 *                  remaining input bytes in the internal buffer.
 *  \details        -
 *  \param[out]     workSpace    actualized algorithm context structure
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               AES_BLOCK_LENGTH
 *                               (out) total size of decryted input
 *                               16 or 32 bytes
 *  \param[in,out]  output       (in) pointer to decrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_AES_PADDING_INVALID  the padding is invalid
 *                  ESL_ERC_INPUT_INVALID        the total input length is not zero mod blocksize
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initDecryptAES256(..)
 *                  esl_decryptAES256 called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeDecryptAES256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES256) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 *  esl_initEncryptAES256Block()
 *********************************************************************************************************************/
/*! \brief          This function initializes the AES block encryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key of length ESL_SIZEOF_AES256_KEY
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptAES256Block(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES256Block) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 *  esl_encryptAES256Block()
 *********************************************************************************************************************/
/*! \brief          This function encrypts an AES Block (= 16 bytes)
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initEncryptAES256Block(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputBlock   pointer to the given input block (16 bytes)
 *  \param[in,out]  outputBlock  (in) pointer to output buffer (at least 16 bytes)
 *                               (out) pointer to encrypted data block
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initEncryptAES256Block(..)
 *  \note           inputBlock and outputBlock can point to the same address, i.e., inplace encryption
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptAES256Block(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES256Block) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) inputBlock, VSECPRIM_P2VAR_PARA(eslt_Byte) outputBlock);

/**********************************************************************************************************************
 *  esl_initDecryptAES256Block()
 *********************************************************************************************************************/
/*! \brief          This function initializes the AES block decryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key of length ESL_SIZEOF_AES256_KEY
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptAES256Block(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES256Block) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 *  esl_decryptAES256Block()
 *********************************************************************************************************************/
/*! \brief          This function decrypts an AES Block (= 16 bytes)
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initDecryptAES256Block(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputBlock   pointer to the given input block (16 bytes)
 *  \param[in,out]  outputBlock  (in) pointer to output buffer (at least 16 bytes)
 *                               (out) pointer to encrypted data block
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initDecryptAES256Block(..)
 *  \note           inputBlock and outputBlock can point to the same address, i.e., inplace decryption is possible
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptAES256Block(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES256Block) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) inputBlock, VSECPRIM_P2VAR_PARA(eslt_Byte) outputBlock);

# endif /* VSECPRIM_AES256_ENABLED */

# if (VSECPRIM_DES_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * DES functions supporting different modes
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_generateKeyDES()
 *********************************************************************************************************************/
/*! \brief           This function generates a 64 bit DES key for usage in block or
 *                   stream DES functions. It uses the RNG which is implemented
 *                   by the call back function esl_getBytesRNG(..).
 *  \details        -
 *  \param[in,out]  key          (in) pointer to user key buffer of size ESL_SIZEOF_DES_KEY
 *                               (out) pointer to the generated key
 *  \return         esl_getBytesRNG(..) return values are passed through
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_generateKeyDES(VSECPRIM_P2VAR_PARA(eslt_Byte) key);

/****************************************************************************
 * DES multi block crypto functions
 ****************************************************************************/

/**********************************************************************************************************************
 *  esl_initEncryptDES()
 *********************************************************************************************************************/
/*! \brief          This function initializes the DES stream encryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key of length ESL_SIZEOF_DES_KEY
 *  \param[in]      blockMode    ECB, CBC, etc. block mode switch
 *  \param[in]      paddingMode  padding mode switch
 *  \param[in]      initializationVector  pointer to initialization vector of length
 *                               ESL_SIZEOF_DES_BLOCK (redundant in ECB mode,
 *                               if iV==NULL, a trivial zero block is used as iV)
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptDES(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceDES) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key,
const eslt_BlockMode blockMode, const eslt_PaddingMode paddingMode,
VSECPRIM_P2CONST_PARA(eslt_Byte) initializationVector);

/**********************************************************************************************************************
 *  esl_encryptDES()
 *********************************************************************************************************************/
/*! \brief          This function encrypts input data of any length and can be called
 *                  arbitrarily often after the algorithm initialization.
 *                  Exact (inputSize+buffered_bytes)/8 blocks will be encrypted
 *                  and written to the output buffer, (inputSize+buffered_bytes)%8
 *                  remaining bytes will be buffered, where 0 <= buffered_bytes < 8.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initEncryptDES(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputSize    length of input data in bytes
 *  \param[in]      input        pointer to data to be encrypted
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               ((inputSize+7)/8)*8       (see output)
 *                               (out) total size of encrypted blocks
 *                               ((inputSize+buffered_bytes)/8)*8
 *  \param[in,out]  output       (in) pointer to encrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initEncryptDES(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptDES(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceDES) workSpace,
const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input,
VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 *  esl_finalizeEncryptDES()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the DES encryption by proccessing the
 *                  remaining input bytes in the internal buffer and padding the
 *                  input according to the chosen padding in the corresponding
 *                  init function.
 *  \details        -
 *  \param[out]     workSpace    actualized algorithm context structure
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               2*DES_BLOCK_LENGTH
 *                               (out) total size of encrypted input
 *                               8 or 16 bytes
 *  \param[in,out]  output       (in) pointer to encrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initEncryptDES(..)
 *                  esl_encryptDES called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeEncryptDES(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceDES) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 *  esl_initDecryptDES()
 *********************************************************************************************************************/
/*! \brief          This function initializes the DES stream decryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key of length ESL_SIZEOF_DES_KEY
 *  \param[in]      blockMode    ECB, CBC, etc. block mode switch
 *  \param[in]      paddingMode  padding mode switch
 *  \param[in]      initializationVector  pointer to initialization vector of length
 *                               ESL_SIZEOF_DES_BLOCK (redundant in ECB mode,
 *                               if iV==NULL, a trivial zero block is used as iV)
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptDES(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceDES) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key,
const eslt_BlockMode blockMode, const eslt_PaddingMode paddingMode,
VSECPRIM_P2CONST_PARA(eslt_Byte) initializationVector);

/**********************************************************************************************************************
 *  esl_decryptDES()
 *********************************************************************************************************************/
/*! \brief          This function decrypts input data of any length and can be called
 *                  arbitrarily often after the algorithm initialization.
 *                  Exact (inputSize+buffered_bytes)/8 blocks will be decrypted
 *                  and written to the output buffer, (inputSize+buffered_bytes)%8
 *                  remaining bytes will be buffered, where 0 <= buffered_bytes < 8.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initDecryptDES(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputSize    length of input data in bytes
 *  \param[in]      input        pointer to data to be encrypted
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               ((inputSize+7)/8)*8       (see output)
 *                               (out) total size of encrypted blocks
 *                               ((inputSize+buffered_bytes)/8)*8
 *  \param[in,out]  output       (in) pointer to encrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initDecryptDES(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptDES(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceDES) workSpace,
const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input,
VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 *  esl_finalizeDecryptDES()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the DES decryption by proccessing the
 *                  remaining input bytes in the internal buffer.
 *  \details        -
 *  \param[out]     workSpace    actualized algorithm context structure
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               DES_BLOCK_LENGTH
 *                               (out) total size of decryted input
 *                               8 or 16 bytes
 *  \param[in,out]  output       (in) pointer to decrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_DES_PADDING_INVALID  the padding is invalid
 *                  ESL_ERC_INPUT_INVALID        the total input length is not zero mod blocksize
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initDecryptDES(..)
 *                  esl_decryptDES called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeDecryptDES(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceDES) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/****************************************************************************
 * DES single block crypto functions (other key length on request)
 ****************************************************************************/

/**********************************************************************************************************************
 *  esl_initEncryptDESBlock()
 *********************************************************************************************************************/
/*! \brief          This function initializes the DES block encryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key of length ESL_SIZEOF_DES_KEY
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptDESBlock(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceDESBlock) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 *  esl_encryptDESBlock()
 *********************************************************************************************************************/
/*! \brief          This function encrypts an DES Block (= 8 bytes)
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initEncryptDESBlock(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputBlock   pointer to the given input block (8 bytes)
 *  \param[in,out]  outputBlock  (in) pointer to output buffer (at least 8 bytes)
 *                               (out) pointer to encrypted data block
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initEncryptDESBlock(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptDESBlock(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceDESBlock) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) inputBlock, VSECPRIM_P2VAR_PARA(eslt_Byte) outputBlock);

/**********************************************************************************************************************
 *  esl_initDecryptDESBlock()
 *********************************************************************************************************************/
/*! \brief          This function initializes the DES block decryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key of length ESL_SIZEOF_DES_KEY
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptDESBlock(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceDESBlock) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 *  esl_decryptDESBlock()
 *********************************************************************************************************************/
/*! \brief          This function decrypts an DES Block (= 8 bytes)
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initDecryptDESBlock(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputBlock   pointer to the given input block (8 bytes)
 *  \param[in,out]  outputBlock  (in) pointer to output buffer (at least 8 bytes)
 *                               (out) pointer to encrypted data block
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initDecryptDESBlock(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptDESBlock(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceDESBlock) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) inputBlock, VSECPRIM_P2VAR_PARA(eslt_Byte) outputBlock);

# endif /* (VSECPRIM_DES_ENABLED == STD_ON) */

# if (VSECPRIM_TDES_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * TDES functions supporting different modes
 *****************************************************************************/

/****************************************************************************
 * TDES functions for multi and single mode (key generation)
 ****************************************************************************/

/**********************************************************************************************************************
 *  esl_generateKey3TDES()
 *********************************************************************************************************************/
/*! \brief           This function generates a 192 bit TDES key for usage in block or
 *                   stream TDES functions. It uses the RNG which is implemented
 *                   by the call back function esl_getBytesRNG(..).
 *  \details        -
 *  \param[in,out]  key          (in) pointer to user key buffer of size ESL_SIZEOF_3TDES_KEY
 *                               (out) pointer to the generated key
 *  \return         esl_getBytesRNG(..) return values are passed through
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_generateKey3TDES(VSECPRIM_P2VAR_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 *  esl_generateKey2TDES()
 *********************************************************************************************************************/
/*! \brief           This function generates a 128 bit TDES key for usage in block or
 *                   stream TDES functions. It uses the RNG which is implemented
 *                   by the call back function esl_getBytesRNG(..).
 *  \details        -
 *  \param[in,out]  key          (in) pointer to user key buffer of size ESL_SIZEOF_2TDES_KEY
 *                               (out) pointer to the generated key
 *  \return         esl_getBytesRNG(..) return values are passed through
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_generateKey2TDES(VSECPRIM_P2VAR_PARA(eslt_Byte) key);

/****************************************************************************
 * TDES multi block crypto functions
 ****************************************************************************/

/**********************************************************************************************************************
 *  esl_initEncryptTDES()
 *********************************************************************************************************************/
/*! \brief          This function initializes the TDES stream encryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key
 *  \param[in]      key_len      length of user key
 *  \param[in]      blockMode    ECB, CBC, etc. block mode switch
 *  \param[in]      paddingMode  padding mode switch
 *  \param[in]      initializationVector  pointer to initialization vector of length
 *                               ESL_SIZEOF_DES_BLOCK (redundant in ECB mode,
 *                               if iV==NULL, a trivial zero block is used as iV)
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                  ESL_ERC_TDES_KEY_LENGTH_INVALID key length != 16 or 24
 *                  ESL_ERC_WS_TOO_SMALL            work space too small
 *                  ESL_ERC_MODE_INVALID            block or padding mode is invalid
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptTDES(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceTDES) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) key, const eslt_Byte key_len,
const eslt_BlockMode blockMode, const eslt_PaddingMode paddingMode,
VSECPRIM_P2CONST_PARA(eslt_Byte) initializationVector);

/**********************************************************************************************************************
 *  esl_encryptTDES()
 *********************************************************************************************************************/
/*! \brief          This function encrypts input data of any length and can be called
 *                  arbitrarily often after the algorithm initialization.
 *                  Exact (inputSize+buffered_bytes)/8 blocks will be encrypted
 *                  and written to the output buffer, (inputSize+buffered_bytes)%8
 *                  remaining bytes will be buffered, where 0 <= buffered_bytes < 8.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initEncryptTDES(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputSize    length of input data in bytes
 *  \param[in]      input        pointer to data to be encrypted
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               ((inputSize+7)/8)*8       (see output)
 *                               (out) total size of encrypted blocks
 *                               ((inputSize+buffered_bytes)/8)*8
 *  \param[in,out]  output       (in) pointer to encrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initEncryptTDES(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptTDES(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceTDES) workSpace,
const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input,
VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 *  esl_finalizeEncryptTDES()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the TDES encryption by proccessing the
 *                  remaining input bytes in the internal buffer and padding the
 *                  input according to the chosen padding in the corresponding
 *                  init function.
 *  \details        -
 *  \param[out]     workSpace    actualized algorithm context structure
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               2*DES_BLOCK_LENGTH
 *                               (out) total size of encrypted input
 *                               8 or 16 bytes
 *  \param[in,out]  output       (in) pointer to encrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initEncryptTDES(..)
 *                  esl_encryptTDES called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeEncryptTDES(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceTDES) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 *  esl_initDecryptTDES()
 *********************************************************************************************************************/
/*! \brief          This function initializes the TDES stream decryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key of length ESL_SIZEOF_3TDES_KEY or ESL_SIZEOF_2TDES_KEY
 *  \param[in]      key_len      length of the key
 *  \param[in]      blockMode    ECB, CBC, etc. block mode switch
 *  \param[in]      paddingMode  padding mode switch
 *  \param[in]      initializationVector  pointer to initialization vector of length
 *                               ESL_SIZEOF_DES_BLOCK (redundant in ECB mode,
 *                               if iV==NULL, a trivial zero block is used as iV)
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL            work space too small
 *                  ESL_ERC_MODE_INVALID            block or padding mode is invalid
 *                  ESL_ERC_TDES_KEY_LENGTH_INVALID key length != 16 or 24
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptTDES(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceTDES) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) key, const eslt_Byte key_len,
const eslt_BlockMode blockMode, const eslt_PaddingMode paddingMode,
VSECPRIM_P2CONST_PARA(eslt_Byte) initializationVector);

/**********************************************************************************************************************
 *  esl_decryptTDES()
 *********************************************************************************************************************/
/*! \brief          This function decrypts input data of any length and can be called
 *                  arbitrarily often after the algorithm initialization.
 *                  Exact (inputSize+buffered_bytes)/8 blocks will be decrypted
 *                  and written to the output buffer, (inputSize+buffered_bytes)%8
 *                  remaining bytes will be buffered, where 0 <= buffered_bytes < 8.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initDecryptTDES(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputSize    length of input data in bytes
 *  \param[in]      input        pointer to data to be encrypted
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               ((inputSize+7)/8)*8       (see output)
 *                               (out) total size of encrypted blocks
 *                               ((inputSize+buffered_bytes)/8)*8
 *  \param[in,out]  output       (in) pointer to encrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initDecryptTDES(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptTDES(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceTDES) workSpace,
const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input,
VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 *  esl_finalizeDecryptTDES()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the TDES decryption by proccessing the
 *                  remaining input bytes in the internal buffer.
 *  \details        -
 *  \param[out]     workSpace    actualized algorithm context structure
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               DES_BLOCK_LENGTH
 *                               (out) total size of decryted input
 *                               8 or 16 bytes
 *  \param[in,out]  output       (in) pointer to decrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_DES_PADDING_INVALID  the padding is invalid
 *                  ESL_ERC_INPUT_INVALID        the total input length is not zero mod blocksize
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initDecryptTDES(..)
 *                  esl_decryptTDES called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeDecryptTDES(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceTDES) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/****************************************************************************
 * TDES single block crypto functions (other key length on request)
 ****************************************************************************/

/**********************************************************************************************************************
 *  esl_initEncryptTDESBlock()
 *********************************************************************************************************************/
/*! \brief          This function initializes the TDES block encryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key
 *  \param[in]      key_len      length of user key
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                  ESL_ERC_TDES_KEY_LENGTH_INVALID key length != 16 or 24
 *                  ESL_ERC_WS_TOO_SMALL            work space too small
 *                  ESL_ERC_MODE_INVALID            block or padding mode is invalid
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptTDESBlock(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceTDESBlock) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) key, const eslt_Byte key_len);

/**********************************************************************************************************************
 *  esl_encryptTDESBlock()
 *********************************************************************************************************************/
/*! \brief          This function encrypts an TDES Block (= 8 bytes)
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initEncryptTDESBlock(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputBlock   pointer to the given input block (8 bytes)
 *  \param[in,out]  outputBlock  (in) pointer to output buffer (at least 8 bytes)
 *                               (out) pointer to encrypted data block
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initEncryptTDESBlock(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptTDESBlock(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceTDESBlock) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) inputBlock, VSECPRIM_P2VAR_PARA(eslt_Byte) outputBlock);

/**********************************************************************************************************************
 *  esl_initDecryptTDESBlock()
 *********************************************************************************************************************/
/*! \brief          This function initializes the TDES block decryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key of length ESL_SIZEOF_3TDES_KEY or ESL_SIZEOF_2TDES_KEY
 *  \param[in]      key_len      key length
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL            work space too small
 *                  ESL_ERC_TDES_KEY_LENGTH_INVALID key length != 16 or 24
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptTDESBlock(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceTDESBlock) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) key, const eslt_Byte key_len);

/**********************************************************************************************************************
 *  esl_decryptTDESBlock()
 *********************************************************************************************************************/
/*! \brief          This function decrypts an TDES Block (= 8 bytes)
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initDecryptTDESBlock(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputBlock   pointer to the given input block (8 bytes)
 *  \param[in,out]  outputBlock  (in) pointer to output buffer (at least 8 bytes)
 *                               (out) pointer to encrypted data block
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initDecryptTDESBlock(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptTDESBlock(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceTDESBlock) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) inputBlock, VSECPRIM_P2VAR_PARA(eslt_Byte) outputBlock);

# endif /* (VSECPRIM_TDES_ENABLED == STD_ON) */

# if (VSECPRIM_RC2_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * RC2 functions supporting different modes
 *****************************************************************************/

/****************************************************************************
 * RC2 functions for multi and single mode (key generation)
 ****************************************************************************/

/**********************************************************************************************************************
 *  esl_generateKeyRC2()
 *********************************************************************************************************************/
/*! \brief           This function generates a ESL_SIZEOF_RC2_KEY RC2 key for usage in block or stream RC2 functions.
 *                    It uses the RNG which is implemented by the call back function esl_getBytesRNG(..).
 *  \details        -
 *  \param[in,out]  key          (in) pointer to user key buffer of size ESL_SIZEOF_RC2_KEY
 *                               (out) pointer to the generated key
 *  \return         esl_getBytesRNG(..) return values are passed through
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_generateKeyRC2(VSECPRIM_P2VAR_PARA(eslt_Byte) key);

/****************************************************************************
 * RC2 multi block crypto functions
 ****************************************************************************/

/**********************************************************************************************************************
 *  esl_initEncryptRC2()
 *********************************************************************************************************************/
/*! \brief          This function initializes the RC2 stream encryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key of length ESL_SIZEOF_RC2_KEY
 *  \param[in]      key_len      length of key in byte
 *  \param[in]      eff_key_len  eff_key_len length of effective key in Byte
 *  \param[in]      blockMode    ECB, CBC, etc. block mode switch
 *  \param[in]      paddingMode  padding mode switch
 *  \param[in]      initializationVector  pointer to initialization vector of length
 *                               ESL_SIZEOF_RC2_BLOCK (redundant in ECB mode,
 *                               if iV==NULL, a trivial zero block is used as iV)
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptRC2(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRC2) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) key, const eslt_Length key_len, const eslt_Length eff_key_len,
const eslt_BlockMode blockMode, const eslt_PaddingMode paddingMode,
VSECPRIM_P2CONST_PARA(eslt_Byte) initializationVector);

/**********************************************************************************************************************
 *  esl_encryptRC2()
 *********************************************************************************************************************/
/*! \brief          This function encrypts input data of any length and can be called
 *                  arbitrarily often after the algorithm initialization.
 *                  Exact (inputSize+buffered_bytes)/8 blocks will be encrypted
 *                  and written to the output buffer, (inputSize+buffered_bytes)%8
 *                  remaining bytes will be buffered, where 0 <= buffered_bytes < 8.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initRC2(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputSize    length of input data in bytes
 *  \param[in]      input        pointer to data to be encrypted
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               ((inputSize+7)/8)*8       (see output)
 *                               (out) total size of encrypted blocks
 *                               ((inputSize+buffered_bytes)/8)*8
 *  \param[in,out]  output       (in) pointer to encrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initRC2(..)
 *  \note           input and output can point to the same address, i.e., in-place encryption is possible
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRC2(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRC2) workSpace,
const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input,
VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 *  esl_finalizeEncryptRC2()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the RC2 encryption by proccessing the
 *                  remaining input bytes in the internal buffer and padding the
 *                  input according to the chosen padding in the corresponding
 *                  init function.
 *  \details        -
 *  \param[out]     workSpace    actualized algorithm context structure
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               2*RC2_BLOCK_LENGTH
 *                               (out) total size of encrypted input
 *  \param[in,out]  output       (in) pointer to encrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initRC2(..)
 *                  esl_encryptRC2 called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeEncryptRC2(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRC2) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 *  esl_initDecryptRC2()
 *********************************************************************************************************************/
/*! \brief          This function initializes the RC2 stream decryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key of length ESL_SIZEOF_RC2_KEY
 *  \param[in]      key_len      length of key in byte
 *  \param[in]      eff_key_len  eff_key_len length of effective key in Byte
 *  \param[in]      blockMode    ECB, CBC, etc. block mode switch
 *  \param[in]      paddingMode  padding mode switch
 *  \param[in]      initializationVector  pointer to initialization vector of length
 *                               ESL_SIZEOF_RC2_BLOCK (redundant in ECB mode,
 *                               if iV==NULL, a trivial zero block is used as iV)
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptRC2(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRC2) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) key, const eslt_Length key_len, const eslt_Length eff_key_len,
const eslt_BlockMode blockMode, const eslt_PaddingMode paddingMode,
VSECPRIM_P2CONST_PARA(eslt_Byte) initializationVector);

/**********************************************************************************************************************
 *  esl_decryptRC2()
 *********************************************************************************************************************/
/*! \brief          This function decrypts input data of any length and can be called
 *                  arbitrarily often after the algorithm initialization.
 *                  Exact (inputSize+buffered_bytes)/8 blocks will be decrypted
 *                  and written to the output buffer, (inputSize+buffered_bytes)%8
 *                  remaining bytes will be buffered, where 0 <= buffered_bytes < 8.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initDecryptRC2(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputSize    length of input data in bytes
 *  \param[in]      input        pointer to data to be encrypted
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               ((inputSize+7)/8)*8       (see output)
 *                               (out) total size of encrypted blocks
 *                               ((inputSize+buffered_bytes)/8)*8
 *  \param[in,out]  output       (in) pointer to encrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initDecryptRC2(..)
 *  \note           input and output can point to the same address, i.e., in-place decryption is possible
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRC2(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRC2) workSpace,
const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input,
VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/**********************************************************************************************************************
 *  esl_finalizeDecryptRC2()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the RC2 decryption by proccessing the
 *                  remaining input bytes in the internal buffer.
 *  \details        -
 *  \param[out]     workSpace    actualized algorithm context structure
 *  \param[in,out]  outputSize   (in) output buffer size in bytes, a safe length is
 *                               RC2_BLOCK_LENGTH
 *                               (out) total size of decryted input
 *  \param[in,out]  output       (in) pointer to decrypted data buffer
 *                               (out) pointer to encrypted data blocks
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 *                  ESL_ERC_RC2_PADDING_INVALID  the padding is invalid
 *                  ESL_ERC_INPUT_INVALID        the total input length is not zero mod blocksize
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initDecryptRC2(..)
 *                  esl_decryptRC2 called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeDecryptRC2(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRC2) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output);

/****************************************************************************
 * RC2 single block crypto functions (other key length on request)
 ****************************************************************************/

/**********************************************************************************************************************
 *  esl_initEncryptRC2Block()
 *********************************************************************************************************************/
/*! \brief          This function initializes the RC2 block encryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key of length ESL_SIZEOF_RC2_KEY
 *  \param[in]      key_len      length of key in byte
 *  \param[in]      eff_key_len  eff_key_len length of effective key in Byte
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptRC2Block(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRC2Block) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) key, const eslt_Length key_len, const eslt_Length eff_key_len);

/**********************************************************************************************************************
 *  esl_encryptRC2Block()
 *********************************************************************************************************************/
/*! \brief          This function encrypts an RC2 Block (= 8 bytes)
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initEncryptRC2Block(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputBlock   pointer to the given input block (8 bytes)
 *  \param[in,out]  outputBlock  (in) pointer to output buffer (at least 8 bytes)
 *                               (out) pointer to encrypted data block
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initEncryptRC2Block(..)
 *  \note           inputBlock and outputBlock can point to the same address, i.e., in-place encryption is possible
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRC2Block(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRC2Block) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) inputBlock, VSECPRIM_P2VAR_PARA(eslt_Byte) outputBlock);

/**********************************************************************************************************************
 *  esl_initDecryptRC2Block()
 *********************************************************************************************************************/
/*! \brief          This function initializes the RC2 block decryption algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to user key of length ESL_SIZEOF_RC2_KEY
 *  \param[in]      key_len      length of key in byte
 *  \param[in]      eff_key_len  eff_key_len length of effective key in Byte
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_MODE_INVALID      block or padding mode is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptRC2Block(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRC2Block) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) key, const eslt_Length key_len, const eslt_Length eff_key_len);

/**********************************************************************************************************************
 *  esl_decryptRC2Block()
 *********************************************************************************************************************/
/*! \brief          This function decrypts an RC2 Block (= 8 bytes)
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initDecryptRC2Block(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputBlock   pointer to the given input block (8 bytes)
 *  \param[in,out]  outputBlock  (in) pointer to output buffer (at least 8 bytes)
 *                               (out) pointer to encrypted data block
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initDecryptRC2Block(..)
 *  \note           inputBlock and outputBlock can point to the same address, i.e., in-place decryption is possible
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRC2Block(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRC2Block) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) inputBlock, VSECPRIM_P2VAR_PARA(eslt_Byte) outputBlock);

# endif /* (VSECPRIM_RC2_ENABLED == STD_ON) */

# if (VSECPRIM_GMAC_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * GMAC functions
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_initGMAC()
 *********************************************************************************************************************/
/*! \brief          This function initializes the GMAC calculation.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      keyLength    the length of the AES key in bytes, must be 16, 24 or 32
 *  \param[in]      key          the symmetric AES master key
 *  \param[in]      ivLength     the length of the initialization vector in bytes
 *  \param[in]      iv           the initialization vector
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL            work space too small
 *                  ESL_ERC_GCM_INVALID_KEY_LENGTH  no valid AES keyLength
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initGMAC(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGMAC) workSpace,
const eslt_Length keyLength,
VSECPRIM_P2CONST_PARA(eslt_Byte) key,
const eslt_Length ivLength,
VSECPRIM_P2CONST_PARA(eslt_Byte) iv);

/**********************************************************************************************************************
 *  esl_updateGMAC()
 *********************************************************************************************************************/
/*! \brief          This function updates the GMAC operation.
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      inputLength  the length of the input
 *  \param[in]      input        the input data
 *  \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID           work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL               work space too small
 *                  ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW  the input exceeded the length limit
 *                  ESL_ERC_NO_ERROR                   else
 *  \pre            workSpace is initialized by esl_initGMAC(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateGMAC(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGMAC) workSpace,
const eslt_Length inputLength,
VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeGMAC()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the GMAC operation.
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *  \param[in,out]  tag          (in) a pointer to a buffer for the authentication tag
 *                               (out) this variable is incremented by the number of bytes that
 *                               were handled in this iteration of updateEncryptGCM
 *  \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID           work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL               work space too small
 *                  ESL_ERC_NO_ERROR                   else
 *  \pre            workSpace is initialized by esl_initGMAC(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeGMAC(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGMAC) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Byte) tag);

/**********************************************************************************************************************
 *  esl_verifyGMAC()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the GMAC operation.
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *  \param[in]      tag          a pointer to a buffer containing the authentication tag
 *  \return         ESL_ERC_PARAMETER_INVALID            input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID             work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL                 work space too small
 *                  ESL_ERC_GCM_TAG_VERIFICATION_FAILED  the provided tag does not
 *                  match the computed one
 *                  ESL_ERC_NO_ERROR                     else
 *  \pre            workSpace is initialized by esl_initGMAC(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyGMAC(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGMAC) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) tag);

# endif /* (VSECPRIM_GMAC_ENABLED == STD_ON) */

# if (VSECPRIM_GCM_ENABLED == STD_ON)
/*****************************************************************************
 * GCM functions
 *****************************************************************************/

/**********************************************************************************************************************
 * esl_initEncryptGCM
 *********************************************************************************************************************/
/*!
 * \brief          Initializes the GCM encryption.
 * \details        This function initializes the GCM encryption.
 * \param[in,out]  workSpace     (in)  algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 * \param[in]      key           the symmetric AES master key
 * \param[in]      keyLength     the length of the AES key in bytes, must be 16, 24 or 32
 * \param[in]      iv            the initialization vector
 * \param[in]      ivLength      the length of the initialization vector in bytes
 * \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL            work space too small
 *                 ESL_ERC_GCM_INVALID_KEY_LENGTH  no valid AES keyLength
 *                 ESL_ERC_NO_ERROR                else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptGCM(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGCM) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) key,
  const eslt_Length keyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) iv,
  const eslt_Length ivLength);

/**********************************************************************************************************************
 * esl_initDecryptGCM
 *********************************************************************************************************************/
/*!
 * \brief          Initializes the GCM decryption.
 * \details        This function initializes the GCM decryption.
 * \param[in,out]  workSpace     (in)  algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 * \param[in]      key           the symmetric AES master key
 * \param[in]      keyLength     the length of the AES key in bytes, must be 16, 24 or 32
 * \param[in]      iv            the initialization vector
 * \param[in]      ivLength      the length of the initialization vector in bytes
 * \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL            work space too small
 *                 ESL_ERC_GCM_INVALID_KEY_LENGTH  no valid AES keyLength
 *                 ESL_ERC_NO_ERROR                else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptGCM(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGCM) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) key,
  const eslt_Length keyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) iv,
  const eslt_Length ivLength);

/**********************************************************************************************************************
 * esl_updateAuthDataEncryptGCM
 *********************************************************************************************************************/
/*!
 * \brief          Hashes additional authenticated data using AES-GCM.
 * \details        This function hashes additional authenticated data using AES-GCM.
 * \param[in,out]  workSpace     (in)  algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) updated algorithm context structure
 * \param[in]      input         additional authenticated data
 * \param[in]      inputLength   the length of the input
 * \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID           work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL               work space too small
 *                 ESL_ERC_GCM_STATE_INVALID          GCM state invalid
 *                 ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW  the input exceeded the length limit
 *                 ESL_ERC_NO_ERROR                   else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateAuthDataEncryptGCM(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGCM) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input,
  const eslt_Length inputLength);

/**********************************************************************************************************************
 * esl_updateAuthDataDecryptGCM
 *********************************************************************************************************************/
/*!
 * \brief          Decrypts the ciphertext using AES-GCM.
 * \details        This function decrypts the ciphertext using AES-GCM.
 * \param[in,out]  workSpace     (in)  algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) updated algorithm context structure
 * \param[in]      input         additional authenticated data
 * \param[in]      inputLength   the length of the input
 * \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID           work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL               work space too small
 *                 ESL_ERC_GCM_STATE_INVALID          GCM state invalid
 *                 ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW  the input exceeded the length limit
 *                 ESL_ERC_NO_ERROR                   else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateAuthDataDecryptGCM(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGCM) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input,
  const eslt_Length inputLength);

/**********************************************************************************************************************
 * esl_updatePlaintextEncryptGCM
 *********************************************************************************************************************/
/*!
 * \brief          Encrypts the plaintext message using AES-GCM.
 * \details        This function encrypts the plaintext message using AES-GCM.
 * \param[in,out]  workSpace     (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) updated algorithm context structure
 * \param[in]      input         plaintext message
 * \param[in]      inputLength   the length of the input
 * \param[out]     output        a pointer to the output buffer for the encrypted message
 * \param[in,out]  bytes_out     a pointer to an output counter variable, this variable is incremented by the number
 *                               of bytes that were handled in this iteration of esl_updatePlaintextEncryptGCM
 * \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID           work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL               work space too small
 *                 ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW  the input exceeded the length limit
 *                 ESL_ERC_NO_ERROR                   else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..),
 *                 output references a valid memory location whose size is a multiple of the GCM block size (16 bytes)
 *                 and greater or equal than inputLength
 *                 bytes_out is a valid pointer
 * \note           input and output can point to the same address, i.e., inplace encryption is possible
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updatePlaintextEncryptGCM(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGCM) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input,
  const eslt_Length inputLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) output,
  VSECPRIM_P2VAR_PARA(eslt_Size32) bytes_out);

/**********************************************************************************************************************
 * esl_updateCiphertextDecryptGCM
 *********************************************************************************************************************/
/*!
 * \brief          Decrypts the ciphertext using AES-GCM.
 * \details        This function decrypts the ciphertext using AES-GCM.
 * \param[in,out]  workSpace     (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) updated algorithm context structure
 * \param[in]      input         ciphertext
 * \param[in]      inputLength   the length of the input
 * \param[out]     output        a pointer to the output buffer for the decrypted ciphertext
 * \param[in,out]  bytes_out     a pointer to an output counter variable, this variable is incremented by the number
 *                               of bytes that were handled in this iteration of esl_updateCiphertextDecryptGCM
 * \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID           work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL               work space too small
 *                 ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW  the input exceeded the length limit
 *                 ESL_ERC_NO_ERROR                   else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..),
 *                 output references a valid memory location whose size is a multiple of the GCM block size (16 bytes)
 *                 and greater or equal than inputLength
 *                 bytes_out is a valid pointer
 * \note           input and output can point to the same address, i.e., inplace decryption is possible
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateCiphertextDecryptGCM(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGCM) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input,
  const eslt_Length inputLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) output,
  VSECPRIM_P2VAR_PARA(eslt_Size32) bytes_out);

/**********************************************************************************************************************
 * esl_finalizeEncryptGCM
 *********************************************************************************************************************/
/*!
 * \brief          Finalizes the GCM encryption.
 * \details        This function finalizes the GCM encryption.
 * \param[in,out]  workSpace     (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) updated algorithm context structure
 * \param[out]     output         a pointer to the output buffer for a possible last block, if there is still data in
 *                                the internal buffer, the remaining bytes are written here
 * \param[out]     bytes_out      incremented by number of remaining bytes
 * \param[out]     tag            a pointer to a buffer for the authentication tag
 * \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID           work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL               work space too small
 *                 ESL_ERC_NO_ERROR                   else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..),
 *                 output references a valid memory location whose size is greater or equal than the GCM block size
 *                 (16 bytes),
 *                 bytes_out is a valid pointer
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeEncryptGCM(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGCM) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) output,
  VSECPRIM_P2VAR_PARA(eslt_Size32) bytes_out,
  VSECPRIM_P2VAR_PARA(eslt_Byte) tag);

/**********************************************************************************************************************
 * esl_finalizeDecryptGCM
 *********************************************************************************************************************/
/*!
 * \brief          Finalizes the GCM operation.
 * \details        This function finalizes the GCM operation.
 * \param[in,out]  workSpace     (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) updated algorithm context structure
 * \param[out]     output         a pointer to the output buffer for a possible last block, if there is still data in
 *                                the internal buffer, the remaining bytes are written here
 * \param[out]     bytes_out      incremented by number of remaining bytes
 * \param[in]      tag            a pointer to a buffer for the authentication tag
 * \return         ESL_ERC_PARAMETER_INVALID            input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID             work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL                 work space too small
 *                 ESL_ERC_GCM_TAG_VERIFICATION_FAILED  the provided tag does not
 *                                                      match the computed one
 *                 ESL_ERC_NO_ERROR                     else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..),
 *                 output references a valid memory location whose size is greater or equal than the GCM block size
 *                 (16 bytes),
 *                 bytes_out is a valid pointer
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeDecryptGCM(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGCM) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) output,
  VSECPRIM_P2VAR_PARA(eslt_Size32) bytes_out,
  VSECPRIM_P2CONST_PARA(eslt_Byte) tag);
# endif /* (VSECPRIM_GCM_ENABLED == STD_ON) */

/*****************************************************************************
 * AES CCM functions
 *****************************************************************************/
# if (VSECPRIM_AES_CCM_ENABLED == STD_ON)
/**********************************************************************************************************************
 * esl_initAESCCM
 *********************************************************************************************************************/
/*!
 * \brief          Initializes the CCM encryption.
 * \details        This function initializes the CCM encryption or decryption.
 * \param[in,out]  workSpace                 (in)  algorithm context buffer, initialized by esl_initWorkSpaceHeader(..)
 *                                                 function
 *                                           (out) initialized algorithm context structure
 * \param[in]      key                       the symmetric AES master key
 * \param[in]      keyLength                 the length of the AES key in bytes, must be 16
 * \param[in]      nonce                     the nonce
 * \param[in]      nonceLength               the length of the nonce in bytes
 * \param[in]      aadLength                 total length of the additional authentication data in bytes
 * \param[in]      msgLength                 total length of the message to be encrypted in bytes
 * \param[in]      authenticationFieldSize   size of the desired MAC in bytes, can be 0
 * \param[in]      lengthFieldSize           value L satisfying msgLength < 2^(8*L), see note
 * \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL            work space too small
 *                 ESL_ERC_INVALID_LENGTH          length of key or iv invalid or value of authenticationFieldSize,
 *                                                 lengthFieldSize or nonce invalid
 *                 ESL_ERC_NO_ERROR                else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \note           The value L of lengthFieldSize indicates that the message to be encrypted is < 2^(8*L) bytes long.
 *                 Valid values for L are 2, 3, 4, 5, 6, 7 or 8
 *                 Valid values for authenticationFieldSize are 4, 6, 8, 10, 12, 14, 16.
 *                 The length of the message is limited to uint32max bytes.
 *                 The nonce must be 15 - lengthFieldSize bytes long. nonceLength is expected to be 7, 8, 9, 10, 11
 *                 12 or 13.
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initAESCCM(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAESCCM) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) key,
  const eslt_Length keyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte)nonce,
  const eslt_Length nonceLength,
  const eslt_Length aadLength,
  const eslt_Length msgLength,
  const eslt_Byte authenticationFieldSize,
  const eslt_Byte lengthFieldSize);

/**********************************************************************************************************************
 * esl_updateAESCCMEncrypt
 *********************************************************************************************************************/
/*!
 * \brief          Starts computing the CBC-MAC and encrypting the plaintext message.
 * \details        This function uses the additional data aad and the message msg to compute the CBC-MAC and the
 *                 encrypted message. All but the last block of msg are processed.
 *                 This function does not allow for a multi-update. The parameters aadSize and msgSize have to match
 *                 the parameters aadLength and msgLength in the call to esl_initAESCCM().
 * \param[in,out]  workSpace     (in)  algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) updated algorithm context structure
 * \param[in]      aad           additional authentication data
 * \param[in]      aadSize       length of aad buffer in bytes
 * \param[in]      msg           plaintext message to be encrypted
 * \param[in]      msgSize       length of msg buffer in bytes
 * \param[in,out]  out           (in)  pointer to buffer to store the encrypted message
 *                               (out) pointer to buffer storing the encrypted message
 * \param[in,out]  outSize       (in)  pointer to buffer storing the number of bytes provided by out
 *                               (out) pointer to buffer storing the number of bytes written to out
 * \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL or length value is invalid
 *                 ESL_ERC_WS_STATE_INVALID           work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL               work space too small
 *                 ESL_ERC_BUFFER_TOO_SMALL           memory provided by out not sufficient to store encrypted message
 *                 ESL_ERC_CCM_TOTAL_LENGTH_OVERFLOW  input length values too high
 *                 ESL_ERC_STATE_INVALID              esl_updateAESCCMEncrypt called more than once
 *                 ESL_ERC_NO_ERROR                   else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                 The buffer provided by out has to provide at least as many bytes as given in outSize.
 * \note           aad can have any length between 0 and (2^32 - 16) bytes, including these values.
 *                 msg can have any length between 0 and (2^32 - 1) bytes, including these values, as long as it is
 *                 consistent with workSpace->wsAESCCM.lengthFieldSize, i.e., the number of bytes in msg has to be
 *                 less than (2^8*(workSpace->wsAESCCM.lengthFieldSize)).
 *                 The parameters aadSize and msgSize have to match the parameters aadLength and msgLength
 *                 in the call to esl_initAESCCM().
 *                 msg and out can point to the same address, i.e., in-place encryption is possible
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateAESCCMEncrypt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAESCCM) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) aad,
  const eslt_Length aadSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) msg,
  const eslt_Length msgSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte)out,
  VSECPRIM_P2VAR_PARA(eslt_Length) outSize);

/**********************************************************************************************************************
 * esl_updateAESCCMDecrypt
 *********************************************************************************************************************/
/*!
 * \brief          Starts re-computing the CBC-MAC and decrypting the encrypted message.
 * \details        This function uses the additional data aad and the encrypted message encMsg to compute the
 *                 CBC-MAC and the decrypted message. All but the last block of msg are processed.
 *                 This function does not allow for a multi-update! The parameters aadSize and encMsgSize have to match
 *                 the parameters aadLength and msgLength in the call to esl_initAESCCM()!
 * \param[in,out]  workSpace    (in)  algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                              (out) updated algorithm context structure
 * \param[in]      aad          additional authentication data
 * \param[in]      aadSize      length of aad buffer in bytes
 * \param[in]      encMsg       plaintext message to be decrypted
 * \param[in]      encMsgSize   length of encMsg buffer in bytes
 * \param[in,out]  out          (in)  pointer to buffer to store the decrypted message
 *                              (out) pointer to buffer storing the decrypted message
 * \param[in,out]  outSize      (in)  pointer to buffer storing the number of bytes provided by out
 *                              (out) pointer to buffer storing the number of bytes written to out
 * \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL or length value is invalid
 *                 ESL_ERC_WS_STATE_INVALID           work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL               work space too small
 *                 ESL_ERC_BUFFER_TOO_SMALL           memory provided by out not sufficient to store encrypted message
 *                 ESL_ERC_CCM_TOTAL_LENGTH_OVERFLOW  input length values too high
 *                 ESL_ERC_STATE_INVALID              esl_updateAESCCMDecrypt called more than once
 *                 ESL_ERC_NO_ERROR                   else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                 The buffer provided by out has to provide at least as many bytes as given in outSize.
 * \note           aad can have any length between 0 and (2^32 - 16) bytes, including these values.
 *                 encMsg can have any length between 0 and (2^32 - 1) bytes, including these values, as long as it is
 *                 consistent with workSpace->wsAESCCM.lengthFieldSize, i.e., the number of bytes in encMsg has to be
 *                 less than (2^8*(workSpace->wsAESCCM.lengthFieldSize)).
 *                 The parameters aadSize and encMsgSize have to match the parameters aadLength and msgLength
 *                 in the call to esl_initAESCCM().
 *                 encMsg and out can point to the same address, i.e., in-place decryption is possible
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateAESCCMDecrypt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAESCCM) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) aad,
  const eslt_Length aadSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) encMsg,
  const eslt_Length encMsgSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte)out,
  VSECPRIM_P2VAR_PARA(eslt_Length) outSize);

/**********************************************************************************************************************
 * esl_finalizeAESCCMEncrypt
 *********************************************************************************************************************/
/*!
 * \brief          Finalizes the CCM authentication and encryption.
 * \details        This function finalizes the AES CCM authentication and encryption by encrypting the last block
 *                 of the message and by computing and encrypting the CBC-MAC.
 * \param[in,out]  workSpace     (in)  algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) updated algorithm context structure
 * \param[in,out]  out           (in)  pointer to a buffer for the remaining bytes of the message
 *                               (out) pointer to buffer storing the remaining bytes of the encrypted message
 * \param[in,out]  outLength     (in)  pointer to buffer storing the number of bytes provided by out
 *                               (out) pointer to buffer storing the number of bytes written to out
 * \param[in,out]  tag           (in)  pointer to a buffer for the authentication tag
 *                               (out) pointer to buffer storing the authentication tag
 * \param[in,out]  tagLength     (in)  pointer to buffer storing the number of bytes provided by tag
 *                               (out) pointer to buffer storing the number of bytes written to tag
 * \return         ESL_ERC_PARAMETER_INVALID     input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID      work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL          work space too small
 *                 ESL_ERC_BUFFER_TOO_SMALL      memory provided by out or tag not sufficient to store the remaining
 *                                               bytes of the encrypted message or the authentication tag
 *                 ESL_ERC_INVALID_LENGTH        value in workSpace->remainingLen invalid, e.g.
 *                                               esl_updateAESCCMEncrypt not called before
 *                 ESL_ERC_NO_ERROR              else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..).
 *                 The buffer referenced by out has to provide at least outLength many bytes.
 *                 The buffer referenced by tag has to provide at least tagLength many bytes.
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeAESCCMEncrypt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAESCCM) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) out,
  VSECPRIM_P2VAR_PARA(eslt_Length) outLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) tag,
  VSECPRIM_P2VAR_PARA(eslt_Length) tagLength);

/**********************************************************************************************************************
 * esl_finalizeAESCCMDecrypt
 *********************************************************************************************************************/
/*!
 * \brief          Finalizes the CCM authentication and decryption.
 * \details        This function finalizes the AES CCM authentication and decryption by decrypting the last block
 *                 of the message and by re-computing and encrypting the CBC-MAC. The recomputed authentication tag
 *                 is compared to the tag given in the parameter 'tag'.
 * \param[in,out]  workSpace     (in)  algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) updated algorithm context structure
 * \param[in,out]  out           (in)  pointer to a buffer for the remaining bytes of the message
 *                               (out) pointer to buffer storing the remaining bytes of the decrypted message
 * \param[in,out]  outLength     (in)  pointer to buffer storing the number of bytes provided by out
 *                               (out) pointer to buffer storing the number of bytes written to out
 * \param[in]      tag           tag to be verified
 * \return         ESL_ERC_PARAMETER_INVALID     input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID      work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL          work space too small
 *                 ESL_ERC_BUFFER_TOO_SMALL      memory provided by out not sufficient to store the remaining
 *                                               bytes of the decrypted message
 *                 ESL_ERC_INVALID_LENGTH        value in workSpace->remainingLen invalid, e.g.
 *                                               esl_updateAESCCMDecrypt not called before
 *                 ESL_ERC_INCORRECT_TAG         recomputed tag does not match the given tag
 *                 ESL_ERC_NO_ERROR              else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..),
 *                 The buffer referenced by out has to provide at least outLength many bytes.
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeAESCCMDecrypt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAESCCM) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) out,
  VSECPRIM_P2VAR_PARA(eslt_Length) outLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) tag);

# endif /* VSECPRIM_AES_CCM_ENABLED == STD_ON */

# if (VSECPRIM_SHA1_ENABLED == STD_ON)
/**********************************************************************************************************************
 * esl_initSHA1
 *********************************************************************************************************************/
/*!
 * \brief          Initializes the SHA-1 calculation.
 * \details        This function initializes the SHA-1 hash calculation.
 * \param[in,out]  workSpace     (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 * \return         ESL_ERC_PARAMETER_INVALID  input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL       work space too small
 *                 ESL_ERC_NO_ERROR           else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSHA1(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA1) workSpace);

/**********************************************************************************************************************
 * esl_updateSHA1
 *********************************************************************************************************************/
/*!
 * \brief          This function updates the SHA-1 calculation for the given input message (or parts of it).
 * \details        This function can be called several times until all input to be hashed is handled.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initSHA1(..) function
 *                             (out) updated algorithm context structure
 * \param[in]      inputSize   length of input data in bytes
 * \param[in]      input       pointer to data to be handled
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW   total input length overflow
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initSHA1(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSHA1(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA1) workSpace,
const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 * esl_finalizeSHA1
 *********************************************************************************************************************/
/*!
 * \brief          Finalizes the SHA-1 calculation.
 * \details        This function finalizes the SHA-1 calculation by doing the appropriate
 *                 padding of the input value and returning the hash value.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initSHA1(..) function
 *                             (out) updated algorithm context structure
 * \param[in,out]  messageDigest   (in) pointer to buffer for the hash value (20 bytes)
 *                                 (out) pointer to computed hash value
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initSHA1(..), esl_updateSHA1 called before
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSHA1(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA1) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

# endif /* (VSECPRIM_SHA1_ENABLED == STD_ON) */

# if (VSECPRIM_SHA2_256_ENABLED == STD_ON)
/******************************************************************************
 * SHA-224
 ******************************************************************************/

/**********************************************************************************************************************
 * esl_initSHA224
 *********************************************************************************************************************/
/*!
 * \brief          Initializes the SHA-224 calculation.
 * \details        This function initializes the SHA-224 hash calculation.
 * \param[in,out]  workSpace     (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 * \return         ESL_ERC_PARAMETER_INVALID  input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL       work space too small
 *                 ESL_ERC_NO_ERROR           else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSHA224(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA224) workSpace);

/**********************************************************************************************************************
 * esl_updateSHA224
 *********************************************************************************************************************/
/*!
 * \brief          This function updates the SHA-224 calculation for the given input message (or parts of it).
 * \details        This function can be called several times until all input to be hashed is handled.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initSHA224(..) function
 *                             (out) updated algorithm context structure
 * \param[in]      inputSize   length of input data in bytes
 * \param[in]      input       pointer to data to be handled
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW   total input length overflow
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initSHA224(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSHA224(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA224) workSpace,
  const eslt_Length inputSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 * esl_finalizeSHA224
 *********************************************************************************************************************/
/*!
 * \brief          Finalizes the SHA-224 calculation.
 * \details        This function finalizes the SHA-224 calculation by doing the appropriate
 *                 padding of the input value and returning the hash value.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initSHA224(..) function
 *                             (out) updated algorithm context structure
 * \param[in,out]  messageDigest   (in) pointer to buffer for the hash value (28 bytes)
 *                                 (out) pointer to computed hash value
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initSHA224(..), esl_updateSHA224 called before
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSHA224(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA224) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

/******************************************************************************
 * SHA-256 hash functions                                                     *
 ******************************************************************************/

/**********************************************************************************************************************
 * esl_initSHA256
 *********************************************************************************************************************/
/*!
 * \brief          Initializes the SHA-256 calculation.
 * \details        This function initializes the SHA-256 hash calculation.
 * \param[in,out]  workSpace     (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 * \return         ESL_ERC_PARAMETER_INVALID  input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL       work space too small
 *                 ESL_ERC_NO_ERROR           else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSHA256(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA256) workSpace);

/**********************************************************************************************************************
 * esl_updateSHA256
 *********************************************************************************************************************/
/*!
 * \brief          This function updates the SHA-256 calculation for the given input message (or parts of it).
 * \details        This function can be called several times until all input to be hashed is handled.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initSHA256(..) function
 *                             (out) updated algorithm context structure
 * \param[in]      inputSize   length of input data in bytes
 * \param[in]      input       pointer to data to be handled
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW   total input length overflow
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initSHA256(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSHA256(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA256) workSpace,
const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 * esl_finalizeSHA256
 *********************************************************************************************************************/
/*!
 * \brief          Finalizes the SHA-256 calculation.
 * \details        This function finalizes the SHA-256 calculation by doing the appropriate
 *                 padding of the input value and returning the hash value.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initSHA256(..) function
 *                             (out) updated algorithm context structure
 * \param[in,out]  messageDigest   (in) pointer to buffer for the hash value (32 bytes)
 *                                 (out) pointer to computed hash value
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initSHA256(..), esl_updateSHA256 called before
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSHA256(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA256) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);
# endif /* (VSECPRIM_SHA2_256_ENABLED == STD_ON) */

# if (VSECPRIM_SHA2_512_ENABLED == STD_ON)
/******************************************************************************
 * SHA-384
 ******************************************************************************/

/**********************************************************************************************************************
 * esl_initSHA384
 *********************************************************************************************************************/
/*!
 * \brief          Initializes the SHA-384 calculation.
 * \details        This function initializes the SHA-384 hash calculation.
 * \param[in,out]  workSpace     (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 * \return         ESL_ERC_PARAMETER_INVALID  input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL       work space too small
 *                 ESL_ERC_NO_ERROR           else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSHA384(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA384) workSpace);

/**********************************************************************************************************************
 * esl_updateSHA384
 *********************************************************************************************************************/
/*!
 * \brief          This function updates the SHA-384 calculation for the given input message (or parts of it).
 * \details        This function can be called several times until all input to be hashed is handled.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initSHA384(..) function
 *                             (out) updated algorithm context structure
 * \param[in]      inputSize   length of input data in bytes
 * \param[in]      input       pointer to data to be handled
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW   total input length overflow
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initSHA384(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSHA384(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA384) workSpace,
  const eslt_Length inputSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 * esl_finalizeSHA384
 *********************************************************************************************************************/
/*!
 * \brief          Finalizes the SHA-384 calculation.
 * \details        This function finalizes the SHA-384 calculation by doing the appropriate
 *                 padding of the input value and returning the hash value.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initSHA384(..) function
 *                             (out) updated algorithm context structure
 * \param[in,out]  messageDigest   (in) pointer to buffer for the hash value (48 bytes)
 *                                 (out) pointer to computed hash value
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initSHA384(..), esl_updateSHA384 called before
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSHA384(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA384) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

/******************************************************************************
 * SHA-512 hash functions
 ******************************************************************************/

/**********************************************************************************************************************
 * esl_initSHA512
 *********************************************************************************************************************/
/*!
 * \brief          Initializes the SHA-512 calculation.
 * \details        This function initializes the SHA-512 hash calculation.
 * \param[in,out]  workSpace     (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 * \return         ESL_ERC_PARAMETER_INVALID  input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL       work space too small
 *                 ESL_ERC_NO_ERROR           else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSHA512(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA512) workSpace);

/**********************************************************************************************************************
 * esl_updateSHA512
 *********************************************************************************************************************/
/*!
 * \brief          This function updates the SHA-512 calculation for the given input message (or parts of it).
 * \details        This function can be called several times until all input to be hashed is handled.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initSHA512(..) function
 *                             (out) updated algorithm context structure
 * \param[in]      inputSize   length of input data in bytes
 * \param[in]      input       pointer to data to be handled
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW   total input length overflow
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initSHA512(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSHA512(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA512) workSpace,
const eslt_Length inputSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 * esl_finalizeSHA512
 *********************************************************************************************************************/
/*!
 * \brief          Finalizes the SHA-512 calculation.
 * \details        This function finalizes the SHA-512 calculation by doing the appropriate
 *                 padding of the input value and returning the hash value.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initSHA512(..) function
 *                             (out) updated algorithm context structure
 * \param[in,out]  messageDigest   (in) pointer to buffer for the hash value (64 bytes)
 *                                 (out) pointer to computed hash value
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initSHA512(..), esl_updateSHA512 called before
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSHA512(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA512) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

/**********************************************************************************************************************
 * esl_initSHA512_224
 *********************************************************************************************************************/
/*!
 * \brief          Initializes the SHA-512-224 calculation.
 * \details        This function initializes the SHA-512-224 hash calculation.
 * \param[in,out]  workSpace     (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 * \return         ESL_ERC_PARAMETER_INVALID  input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL       work space too small
 *                 ESL_ERC_NO_ERROR           else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSHA512_224(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA512_224) workSpace);

/**********************************************************************************************************************
 * esl_updateSHA512_224
 *********************************************************************************************************************/
/*!
 * \brief          This function updates the SHA-512-224 calculation for the given input message (or parts of it).
 * \details        This function can be called several times until all input to be hashed is handled.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initSHA512_224(..) function
 *                             (out) updated algorithm context structure
 * \param[in]      inputSize   length of input data in bytes
 * \param[in]      input       pointer to data to be handled
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW   total input length overflow
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initSHA512_224(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSHA512_224(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA512_224) workSpace,
const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 * esl_finalizeSHA512_224
 *********************************************************************************************************************/
/*!
 * \brief          Finalizes the SHA-512-224 calculation.
 * \details        This function finalizes the SHA-512-224 calculation by doing the appropriate
 *                 padding of the input value and returning the hash value.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initSHA512_224(..) function
 *                             (out) updated algorithm context structure
 * \param[in,out]  messageDigest   (in) pointer to buffer for the hash value (28 bytes)
 *                                 (out) pointer to computed hash value
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initSHA512_224(..), esl_updateSHA512_224 called before
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSHA512_224(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA512_224) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

/**********************************************************************************************************************
 * esl_initSHA512_256
 *********************************************************************************************************************/
/*!
 * \brief          Initializes the SHA-512-256 calculation.
 * \details        This function initializes the SHA-512-256 hash calculation.
 * \param[in,out]  workSpace     (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 * \return         ESL_ERC_PARAMETER_INVALID  input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL       work space too small
 *                 ESL_ERC_NO_ERROR           else
 * \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSHA512_256(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA512_256) workSpace);

/**********************************************************************************************************************
 * esl_updateSHA512_256
 *********************************************************************************************************************/
/*!
 * \brief          This function updates the SHA-512-256 calculation for the given input message (or parts of it).
 * \details        This function can be called several times until all input to be hashed is handled.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initSHA512_256(..) function
 *                             (out) updated algorithm context structure
 * \param[in]      inputSize   length of input data in bytes
 * \param[in]      input       pointer to data to be handled
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW   total input length overflow
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initSHA512_256(..)
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSHA512_256(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA512_256) workSpace,
const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 * esl_finalizeSHA512_256
 *********************************************************************************************************************/
/*!
 * \brief          Finalizes the SHA-512-256 calculation.
 * \details        This function finalizes the SHA-512-256 calculation by doing the appropriate
 *                 padding of the input value and returning the hash value.
 * \param[in,out]  workSpace   (in) algorithm context buffer, initialized by esl_initSHA512_256(..) function
 *                             (out) updated algorithm context structure
 * \param[in,out]  messageDigest   (in) pointer to buffer for the hash value (32 bytes)
 *                                 (out) pointer to computed hash value
 * \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_NO_ERROR          else
 * \pre            workSpace is initialized by esl_initSHA512_256(..), esl_updateSHA512_256 called before
 * \context        TASK
 * \reentrant      TRUE, for different workspaces
 * \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSHA512_256(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA512_256) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

# endif /* (VSECPRIM_SHA2_512_ENABLED == STD_ON) */

# if (VSECPRIM_SHA3_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  esl_initSHA3()
 *********************************************************************************************************************/
/*! \brief          Initialize the SHA3 hash calculation.
 *  \details        -
 *  \param[in,out]  workSpace  (in)  algorithm context buffer, initialized by esl_initWorkSpaceHeader (...) function
 *                             (out) initialized algorithm context structure
 *  \param[in]      bitLength  desired length of the digest, determines whether SHA3-224, SHA3-256, SHA3-384 or
 *                             SHA3-512 is used
 *  \return         ESL_ERC_PARAMETER_INVALID    input parameter is NULL, or bit length parameter is invalid,
 *                                               i.e., not 224, nor 256, nor 384 nor 512
 *                  ESL_ERC_WS_TOO_SMALL         work space too small
 *                  ESL_ERC_NO_ERROR             otherwise
 *  \pre            workSpace has been initialized by esl_initWorkSpaceHeader (...)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSHA3(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA3) workSpace,
  const eslt_Length bitLength);

/**********************************************************************************************************************
 *  esl_updateSHA3()
 *********************************************************************************************************************/
/*! \brief          Updates the SHA3 hash calculation for the given input data.
 *  \details        This function can be called several times until all data to be hashed has been passed.
 *  \param[in,out]  workSpace    (in) SHA3 context buffer
 *                               (out) updated SHA3 context structure
 *  \param[in]      inputSize    length of input data in bytes
 *  \param[in]      input        pointer to data to be hashed
 *  \return         ESL_ERC_PARAMETER_INVALID   input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL        work space too small
 *                  ESL_ERC_WS_STATE_INVALID    work space state invalid
 *                  ESL_ERC_STATE_INVALID       workSpace in invalid KECCAKc state, e.g. when called
 *                                              after esl_finalizeSHA3()
 *                  ESL_ERC_NO_ERROR            otherwise
 *  \pre            workSpace has been initialized by esl_initSHA3 (...)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSHA3(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA3) workSpace,
  const eslt_Length inputSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeSHA3()
 *********************************************************************************************************************/
/*! \brief          Finalizes the SHA3 hash calculation and returns the hash value.
 *  \details        The input data is padded and the bit counter is added to the last hash input block.
 *  \param[in,out]  workSpace              (in)  SHA3 context buffer
 *                                         (out) updated algorithm context structure
 *  \param[in,out]  messageDigest          (in)  pointer to buffer for the hash value
 *                                         (out) pointer to computed hash value
 *  \param[in,out]  messageDigestLength    (in)  pointer to buffer, storing the length of messageDigest
 *                                         (out) pointer to buffer, storing the length of the digest written to
 *                                               the buffer referenced by 'messageDigest'
 *  \return         ESL_ERC_PARAMETER_INVALID   input parameter is NULL
 *                  ESLL_ERC_WS_TOO_SMALL       work space too small
 *                  ESL_ERC_WS_STATE_INVALID    work space state invalid
 *                  ESL_ERC_NO_ERROR            otherwise
 *  \pre            workSpace has been initialized by esl_initSHA3 (...)
 *                  The buffer referenced by messageDigest must provide at least as many bytes as referenced by
 *                  messageDigestLength.
 *  \note           If SHA3 was initialized with bitLength 224 (256/384/512), the digest length will by default be
 *                  28 (32/48/64) bytes. However, the value referenced by messageDigestLength can be less than 28
 *                  (32/48/64), if only a truncated digest is required.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSHA3(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA3) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest,
  VSECPRIM_P2VAR_PARA(eslt_Length) messageDigestLength);

/******************************************************************************
 * SHAKExxx digest functions
 ******************************************************************************/
/**********************************************************************************************************************
 *  esl_initSHAKE()
 *********************************************************************************************************************/
/*! \brief          Initialize the SHAKE hash calculation.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader (...) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      strength     algorithm strength - 128 or 256 BITs
 *  \return         ESL_ERC_PARAMETER_INVALID    input parameter is NULL, or
 *                  strength parameter is neither 128 nor 256 bits
 *                  ESL_ERC_WS_TOO_SMALL         work space too small
 *                  ESL_ERC_NO_ERROR             otherwise
 *  \pre            workSpace has been initialized by esl_initWorkSpaceHeader (...)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSHAKE(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHAKE) workSpace,
  const eslt_Length strength);

/**********************************************************************************************************************
 *  esl_updateSHAKE()
 *********************************************************************************************************************/
/*! \brief          Updates the SHAKE hash calculation for the given input data.
 *                  This function can be called several times until all data to be hashed has been passed.
 *  \details        -
 *  \param[in,out]  workSpace    (in) SHAKE context buffer
 *                               (out) updated SHAKE context structure
 *  \param[in]      inputSize    length of input data in bytes
 *  \param[in]      input        pointer to data to be hashed
 *  \return         ESL_ERC_PARAMETER_INVALID   input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL         work space too small
 *                  ESL_ERC_WS_STATE_INVALID    work space state invalid
 *                  ESL_ERC_STATE_INVALID workSpace in invalid KECCAKc state, e.g. when called after
 *                                        esl_getXOFDigestSHAKE() or after esl_finalizeSHAKE()
 *                  ESL_ERC_NO_ERROR            otherwise
 *  \pre            workSpace has been initialized by esl_initSHAKE (...)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSHAKE(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHAKE) workSpace,
  const eslt_Length inputSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_getXOFDigestSHAKE()
 *********************************************************************************************************************/
/*! \brief          Get the digest from the SHAKE hash calculation.
 *                  This function can be used repeatedly to return more digest bits (XOF).
 *  \details        The workspace is not cleared after calculating the hash value.
 *  \param[in,out]  workSpace    (in) SHAKE context buffer
 *                               (out) updated algorithm context structure
 *  \param[in,out]  messageDigest  (in) pointer to buffer for the hash value
 *                               (out) pointer to computed hash value
 *  \param[in]      digestLength  length of requested digest in bits
 *  \return         ESL_ERC_PARAMETER_INVALID   input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL        work space too small
 *                  ESL_ERC_WS_STATE_INVALID    work space state invalid
 *                  ESL_ERC_INVALID_LENGTH      digest length invalid (not multiple of 8)
 *                  ESL_ERC_NO_ERROR            otherwise
 *  \pre            workSpace has been initialized by esl_initSHAKE (...)
 *                  The buffer provided by messageDigest must provide at least digestLength many bytes.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_getXOFDigestSHAKE(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHAKE) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest,
  const eslt_Length digestLength);

/**********************************************************************************************************************
 *  esl_finalizeSHAKE()
 *********************************************************************************************************************/
/*! \brief          Finalizes the SHAKE hash calculation and returns (additional) digest bits (XOF).
 *                  The input data is padded and the bit counter added to the last hash input block.
 *  \details        This function cannot be used repeatedly to return more digest bits.
 *                  If messageDigest is a null pointer, the function only resets and clears the workSpace.
 *  \param[in,out]  workSpace              (in)  SHAKE context buffer
 *                                         (out) updated algorithm context structure
 *  \param[in,out]  messageDigest          (in)  pointer to buffer for the hash value, possibly a null pointer
 *                                         (out) pointer to computed hash value, if messageDigest is not a null pointer
 *                                               when NULL, NO digest is returned, but just the context RESET
 *  \param[in,out]  messageDigestLength    (in)  pointer to buffer, storing the length of messageDigest, possibly
 *                                               a null pointer
 *                                         (out) pointer to buffer, storing the length of the digest written to
 *                                               the buffer referenced by 'messageDigest', if messageDigestLength is
 *                                               not a null pointer
 *  \return         ESL_ERC_PARAMETER_INVALID   work space is NULL or messageDigestLength is NULL, if messageDigest is
 *                                              not a null pointer
 *                  ESL_ERC_WS_TOO_SMALL        work space too small
 *                  ESL_ERC_WS_STATE_INVALID    work space state invalid
 *                  ESL_ERC_NO_ERROR            otherwise
 *  \pre            workSpace has been initialized by esl_initSHAKE (...)
 *                  If messageDigest is not a null pointer, the buffer provided by messageDigest must provide
 *                  at least as many bytes as referenced by messageDigestLength.
 *  \note           If SHAKE was initialized with strength 128 (256), the digest length will by default be
 *                  32 (64) bytes. However, the value referenced by messageDigestLength can be less than 32 (64), if
 *                  only a truncated digest is required.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSHAKE(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHAKE) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest,
  VSECPRIM_P2VAR_PARA(eslt_Length) messageDigestLength);

# endif /* (VSECPRIM_SHA3_ENABLED == STD_ON) */

# if (VSECPRIM_RIPEMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * RIPEMD-160 hash functions
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_initRIPEMD160()
 *********************************************************************************************************************/
/*! \brief          This function initializes the RIPEMD-160 hash calculation.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initRIPEMD160(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRIPEMD160) workSpace);

/**********************************************************************************************************************
 *  esl_updateRIPEMD160()
 *********************************************************************************************************************/
/*! \brief          This function updates the RIPEMD-160 calculation for the given
 *                  input message (or parts of it). This function can be called
 *                  several times until all input to be hashed is handled.
 *  \details        -
 *  \param[in,out]  workSpace    (in) length of input data in bytes
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputSize    length of the input in bytes
 *  \param[in]      input        pointer to data to be handled
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW   total input length overflow
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initRIPEMD160(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateRIPEMD160(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRIPEMD160) workSpace,
const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeRIPEMD160()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the RIPEMD-160 calculation by doing the appropriate
 *                  padding of the input value and returning the hash value.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initRIPEMD160(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in,out]  messageDigest  (in) pointer to buffer for the hash value (20 bytes)
 *                               (out) pointer to computed hash value
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initRIPEMD160(..)
 *                  esl_updateRIPEMD160 called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeRIPEMD160(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRIPEMD160) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

# endif /* (VSECPRIM_RIPEMD160_ENABLED == STD_ON) */

# if (VSECPRIM_MD5_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * MD5 hash functions
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_initMD5()
 *********************************************************************************************************************/
/*! \brief          This function initializes the MD5 hash calculation.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initMD5(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceMD5) workSpace);

/**********************************************************************************************************************
 *  esl_updateMD5()
 *********************************************************************************************************************/
/*! \brief          This function updates the MD5 calculation for the given
 *                  input message (or parts of it). This function can be called
 *                  several times until all input to be hashed is handled.
 *  \details        -
 *  \param[in,out]  workSpace    (in) length of input data in bytes
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputSize    length of the input in bytes
 *  \param[in]      input        pointer to data to be handled
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initMD5(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateMD5(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceMD5) workSpace,
const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeMD5()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the MD5 calculation by doing the appropriate
 *                  padding of the input value and returning the hash value.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initMD5(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in,out]  messageDigest  (in) pointer to buffer for the hash value (20 bytes)
 *                               (out) pointer to computed hash value
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initMD5(..)
 *                  esl_updateMD5 called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeMD5(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceMD5) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);
# endif /* (VSECPRIM_MD5_ENABLED == STD_ON) */

# if (VSECPRIM_HMAC_SHA1_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * HashMACSHA-1 crypto functions (according to RFC 2104 with SHA-1)
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_generateKeyHashMACSHA1()
 *********************************************************************************************************************/
/*! \brief           This function generates a HMAC key for an HashMac calculation. It uses the RNG which is
 *                   implemented by the call back function esl_getBytesRNG(..).
 *  \details        -
 *  \param[in]      keyLength    the byte length of the key to be generated
 *  \param[in,out]  key          (in) pointer to user key buffer of size keyLength
 *                               (out) pointer to the generated key
 *  \return         esl_getBytesRNG(..) return values are passed through
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_generateKeyHashMACSHA1(
const eslt_Length keyLength, VSECPRIM_P2VAR_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 *  esl_initHashMACSHA1()
 *********************************************************************************************************************/
/*! \brief          This function initializes the HashMAC calculation according
 *                  to RFC2104 based on the hash function SHA-1.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      keyLength    the byte length of the key to be used
 *  \param[in]      key          pointer to user key
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE      the given key length is zero
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initHashMACSHA1(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACSHA1) workSpace,
const eslt_Length keyLength, VSECPRIM_P2CONST_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 *  esl_updateHashMACSHA1()
 *********************************************************************************************************************/
/*! \brief          This function updates the HashMAC calculation according
 *                  to RFC2104 based on the hash function SHA-1.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initHashMACSHA1(..) function
 *                               (out) updated algorithm context structure
 *  \param[in]      inputLength  length of input data in bytes
 *  \param[in]      input        pointer to data to be handled
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW   total input length overflow
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initHashMACSHA1(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateHashMACSHA1(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACSHA1) workSpace,
const eslt_Length inputLength, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeHashMACSHA1()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the HashMAC calculation by returning
 *                  the calculated HashMAC according to RFC2104 based on SHA-1
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initHashMACSHA1(..) function
 *                               (out) updated algorithm context structure
 *  \param[in,out]  messageHashMAC  (in) pointer to buffer for HashMAC (at least 20 bytes)
 *                               (out) pointer to computed HashMAC
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initHashMACSHA1(..)
 *                  esl_updateHashMACSHA1 called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashMACSHA1(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACSHA1) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageHashMAC);

/**********************************************************************************************************************
 *  esl_verifyHashMACSHA1()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the verification of a given HashMAC
 *                  according to RFC2104 based on SHA-1.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initHashMACSHA1(..) function
 *                               (out) updated algorithm context structure
 *  \param[in]      messageHashMAC  pointer to the given HashMAC (20 bytes)
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_HMAC_INCORRECT_MAC   the given HashMAC is incorrect
 *                  ESL_ERC_NO_ERROR          HashMAC is correct
 *  \pre            workSpace is initialized by esl_initHashMACSHA1(..)
 *                  esl_updateHashMACSHA1 called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyHashMACSHA1(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACSHA1) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) messageHashMAC);
# endif /* (VSECPRIM_HMAC_SHA1_ENABLED == STD_ON) */

# if (VSECPRIM_HMAC_RMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * HashMACRIPEMD160 crypto functions (according to RFC 2104 with RIPEMD-160)
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_generateKeyHashMACRIPEMD160()
 *********************************************************************************************************************/
/*! \brief           This function generates a HMAC key for an HashMac calculation. It uses the RNG which is
 *                   implemented by the call back function esl_getBytesRNG(..).
 *  \details        -
 *  \param[in]      keyLength    the byte length of the key to be generated
 *  \param[in,out]  key          (in) pointer to user key buffer of size keyLength
 *                               (out) pointer to the generated key
 *  \return         esl_getBytesRNG(..) return values are passed through
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_generateKeyHashMACRIPEMD160(
const eslt_Length keyLength, VSECPRIM_P2VAR_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 *  esl_initHashMACRIPEMD160()
 *********************************************************************************************************************/
/*! \brief          This function initializes the HashMAC calculation according
 *                  to RFC2104 based on the hash function RIPEMD-160.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      keyLength    the byte length of the key to be used
 *  \param[in]      key          pointer to user key
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE      the given key length is zero
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initHashMACRIPEMD160(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACRIPEMD160) workSpace,
const eslt_Length keyLength, VSECPRIM_P2CONST_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 *  esl_updateHashMACRIPEMD160()
 *********************************************************************************************************************/
/*! \brief          This function updates the HashMAC calculation according
 *                  to RFC2104 based on the hash function RIPEMD-160.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initHashMACRIPEMD160(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputLength  length of input data in bytes
 *  \param[in]      input        pointer to data to be handled
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW   total input length overflow
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initHashMACRIPEMD160(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateHashMACRIPEMD160(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACRIPEMD160) workSpace,
const eslt_Length inputLength, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeHashMACRIPEMD160()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the HashMAC calculation by returning
 *                  the calculated HashMAC according to RFC2104 based on RIPEMD-160
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initHashMACRIPEMD160(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in,out]  messageHashMAC  (in) pointer to buffer for HashMAC (at least 20 bytes)
 *                               (out) pointer to computed HashMAC
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initHashMACRIPEMD160(..)
 *                  esl_updateHashMACRIPEMD160 called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashMACRIPEMD160(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACRIPEMD160) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageHashMAC);

/**********************************************************************************************************************
 *  esl_verifyHashMACRIPEMD160()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the verification of a given HashMAC
 *                  according to RFC2104 based on RIPEMD-160.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initHashMACRIPEMD160(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      messageHashMAC  pointer to the given HashMAC (20 bytes)
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_HMAC_INCORRECT_MAC   the given HashMAC is incorrect
 *                  ESL_ERC_NO_ERROR          HashMAC is correct
 *  \pre            workSpace is initialized by esl_initHashMACRIPEMD160(..)
 *                  esl_updateHashMACRIPEMD160 called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyHashMACRIPEMD160(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACRIPEMD160) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) messageHashMAC);
# endif /* (VSECPRIM_HMAC_RMD160_ENABLED == STD_ON) */

# if (VSECPRIM_HMAC_SHA2_256_ENABLED == STD_ON)
/*****************************************************************************
 * HashMACSHA256 crypto functions (according to RFC 2104 with SHA-256)
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_generateKeyHashMACSHA256()
 *********************************************************************************************************************/
/*! \brief           This function generates a HMAC key for an HashMac calculation. It uses the RNG which is
 *                   implemented by the call back function esl_getBytesRNG(..).
 *  \details        -
 *  \param[in]      keyLength    the byte length of the key to be generated
 *  \param[in,out]  key          (in) pointer to user key buffer of size keyLength
 *                               (out) pointer to the generated key
 *  \return         esl_getBytesRNG(..) return values are passed through
 *  \pre            The buffer referenced by key must provide at least 'keyLength' many bytes.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_generateKeyHashMACSHA256(
const eslt_Length keyLength,
VSECPRIM_P2VAR_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 *  esl_initHashMACSHA256()
 *********************************************************************************************************************/
/*! \brief          This function initializes the HashMAC calculation according
 *                  to RFC2104 based on the hash function SHA-256.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      keyLength    the byte length of the key to be used
 *  \param[in]      key          pointer to user key
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE      the given key length is zero
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initHashMACSHA256(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACSHA256) workSpace,
const eslt_Length keyLength,
VSECPRIM_P2CONST_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 *  esl_updateHashMACSHA256()
 *********************************************************************************************************************/
/*! \brief          This function updates the HashMAC calculation according
 *                  to RFC2104 based on the hash function SHA-256.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by esl_initHashMACSHA256(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputLength  length of input data in bytes
 *  \param[in]      input        pointer to data to be handled
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW   total input length overflow
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initHashMACSHA256(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateHashMACSHA256(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACSHA256) workSpace,
const eslt_Length inputLength,
VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeHashMACSHA256()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the HashMAC calculation by returning
 *                  the calculated HashMAC according to RFC2104 based on SHA-256
 *  \details        -
 *  \param[in,out]  workSpace       (in)  algorithm context buffer, initialized by esl_initHashMACSHA256(..) function
 *                                  (out) actualized algorithm context structure
 *  \param[in,out]  messageHashMAC  (in)  pointer to buffer for HashMAC (at least 32 bytes)
 *                                  (out) pointer to computed HashMAC
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initHashMACSHA256(..)
 *                  esl_updateHashMACSHA256 called before
 *                  The buffer referenced by messageHashMAC must provide at least ESL_SIZEOF_SHA256_DIGEST (= 32)
 *                  bytes.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashMACSHA256(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACSHA256) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Byte) messageHashMAC);

/**********************************************************************************************************************
 *  esl_verifyHashMACSHA256()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the verification of a given HashMAC
 *                  according to RFC2104 based on SHA-256.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initHashMACSHA256(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      messageHashMAC  pointer to the given HashMAC (32 bytes)
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_HMAC_INCORRECT_MAC   the given HashMAC is incorrect
 *                  ESL_ERC_NO_ERROR          HashMAC is correct
 *  \pre            workSpace is initialized by esl_initHashMACSHA256(..)
 *                  esl_updateHashMACSHA256 called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyHashMACSHA256(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACSHA256) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) messageHashMAC);
# endif /* (VSECPRIM_HMAC_SHA2_256_ENABLED == STD_ON) */

# if (VSECPRIM_HMAC_SHA2_384_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * HashMACSHA384 crypto functions (according to RFC 2104 with SHA-384)
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_generateKeyHashMACSHA384()
 *********************************************************************************************************************/
/*! \brief           This function generates a HMAC key for an HashMac calculation. It uses the RNG which is
 *                   implemented by the call back function esl_getBytesRNG(..).
 *  \details        -
 *  \param[in]      keyLength    the byte length of the key to be generated
 *  \param[in,out]  key          (in) pointer to user key buffer of size keyLength
 *                               (out) pointer to the generated key
 *  \return         esl_getBytesRNG(..) return values are passed through
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_generateKeyHashMACSHA384(
  const eslt_Length keyLength, VSECPRIM_P2VAR_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 *  esl_initHashMACSHA384()
 *********************************************************************************************************************/
/*! \brief          This function initializes the HashMAC calculation according
 *                  to RFC2104 based on the hash function SHA-384.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      keyLength    the byte length of the key to be used
 *  \param[in]      key          pointer to user key
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE      the given key length is zero
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initHashMACSHA384(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACSHA384) workSpace,
  const eslt_Length keyLength, VSECPRIM_P2CONST_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 *  esl_updateHashMACSHA384()
 *********************************************************************************************************************/
/*! \brief          This function updates the HashMAC calculation according
 *                  to RFC2104 based on the hash function SHA-384.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initHashMACSHA384(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      inputLength  length of input data in bytes
 *  \param[in]      input        pointer to data to be handled
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_SHA384_TOTAL_LENGTH_OVERFLOW   total input length overflow
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initHashMACSHA384(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateHashMACSHA384(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACSHA384) workSpace,
  const eslt_Length inputLength, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeHashMACSHA384()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the HashMAC calculation by returning
 *                  the calculated HashMAC according to RFC2104 based on SHA-384
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initHashMACSHA384(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in,out]  messageHashMAC  (in) pointer to buffer for HashMAC (at least 32 bytes)
 *                               (out) pointer to computed HashMAC
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initHashMACSHA384(..)
 *                  esl_updateHashMACSHA384 called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashMACSHA384(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACSHA384) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageHashMAC);

/**********************************************************************************************************************
 *  esl_verifyHashMACSHA384()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the verification of a given HashMAC
 *                  according to RFC2104 based on SHA-384.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initHashMACSHA384(..) function
 *                               (out) actualized algorithm context structure
 *  \param[in]      messageHashMAC  pointer to the given HashMAC (32 bytes)
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_HMAC_INCORRECT_MAC   the given HashMAC is incorrect
 *                  ESL_ERC_NO_ERROR          HashMAC is correct
 *  \pre            workSpace is initialized by esl_initHashMACSHA384(..)
 *                  esl_updateHashMACSHA384 called before
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyHashMACSHA384(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACSHA384) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) messageHashMAC);
# endif /* (VSECPRIM_HMAC_SHA2_384_ENABLED == STD_ON) */

# if (VSECPRIM_CMAC_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  esl_initCMACAES128
 **********************************************************************************************************************/
/*! \brief         Initialize CMAC calculation
 *  \details       This function initializes the CMAC calculation workspace.
 *  \param[in,out] workSpace  CMAC work space
 *  \param[in]     keyLength  Size of AES key to be used
 *  \param[in]     key  AES key to be used
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE  AES key length is invalid
 *  \pre           Pointers must be valid
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initCMACAES128(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCMACAES) workSpace,
  const eslt_Length keyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) key);

/***********************************************************************************************************************
 *  esl_initExpandedCMACAES128
 **********************************************************************************************************************/
/*! \brief         Initialize CMAC calculation with a pre-calculated CMAC expanded key.
 *  \details       This function initializes the CMAC calculation workspace using a pre-calculated CMAC expanded key.
 *                 This will speed up the CMAC verification process.
 *  \param[in,out] workSpace  CMAC work space
 *  \param[in]     expandedKey  CMAC expanded key which shall be used
 *  \param[in]     expandedKeyLength  Size of provided key
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE  AES key length is invalid
 *  \pre           Pointers must be valid
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initExpandedCMACAES128(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCMACAES) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) expandedKey,
  const eslt_Length expandedKeyLength);

/***********************************************************************************************************************
 *  esl_getExpandedKeyCMACAES128
 **********************************************************************************************************************/
/*! \brief         Extracts the expanded CMAC key.
 *  \details       This function extracts the expanded CMAC key from the given workspace. Re-using the expanded key
 *                 on a system with constant key usage will speed up the CMAC calculation.
 *  \param[in]     workSpace  CMAC work space
 *  \param[out]    keyPtr  Pointer to buffer where the expanded key shall be stored.
 *  \param[in,out] keyLengthPtr  In  Size of provided buffer, Out  Size of written data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *                 ESL_ERC_INPUT_INVALID  Invalid input length
 *  \pre           Workspace needs to be initialized.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_getExpandedKeyCMACAES128(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceCMACAES) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) keyPtr,
  VSECPRIM_P2VAR_PARA(eslt_Length) keyLengthPtr);

/***********************************************************************************************************************
 *  esl_updateCMACAES128
 **********************************************************************************************************************/
/*! \brief         Update CMAC calculation.
 *  \details       This function is used to feed the CMAC calculation with input data.
 *  \param[in,out] workSpace  CMAC work space
 *  \param[in]     inputLength  Length of input data
 *  \param[in]     input  Pointer to input data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *  \pre           Workspace needs to be initialized.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateCMACAES128(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCMACAES) workSpace,
  const eslt_Length inputLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/***********************************************************************************************************************
 *  esl_finalizeCMACAES128
 **********************************************************************************************************************/
/*! \brief         Finalize CMAC calculation.
 *  \details       This function finalizes the MAC calculation.
 *  \param[in,out] workSpace  CMAC work space
 *  \param[out]    messageCMAC  Pointer to computed CMAC. The size must be 16 bytes.
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *                 ESL_ERC_INPUT_INVALID  Invalid input length
 *  \pre           Workspace needs to be initialized.
 *                 messageCMAC must point to valid memory of at least 16 bytes
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeCMACAES128(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCMACAES) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Byte) messageCMAC);

/***********************************************************************************************************************
 *  esl_verifyCMACAES128
 **********************************************************************************************************************/
/*! \brief         Verify given CMAC.
 *  \details       This function verifies the provided CMAC against the calculated one.
 *  \param[in,out] workSpace  CMAC work space
 *  \param[in]     messageCMAC  Pointer to CMAC to be compared. The size must be 16 bytes.
 *  \return        ESL_ERC_NO_ERROR  CMAC is correct
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *  \pre           Workspace needs to be initialized.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyCMACAES128(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCMACAES) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) messageCMAC);

/***********************************************************************************************************************
 *  esl_initCMACAES256
 **********************************************************************************************************************/
/*! \brief         Initialize CMAC calculation
 *  \details       This function initializes the CMAC calculation workspace.
 *  \param[in,out] workSpace   CMAC work space
 *  \param[in]     keyLength   Size of AES key to be used
 *  \param[in]     key         AES key to be used
 *  \return        ESL_ERC_NO_ERROR                      Operation successful
 *                 ESL_ERC_PARAMETER_INVALID             Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL                  Work space too small
 *                 ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE  AES key length is invalid
 *  \pre           Pointers must be valid
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initCMACAES256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCMACAES) workSpace,
  const eslt_Length keyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) key);

/***********************************************************************************************************************
 *  esl_initExpandedCMACAES256
 **********************************************************************************************************************/
/*! \brief         Initialize CMAC AES256 calculation with a pre-calculated CMAC expanded key.
 *  \details       This function initializes the CMAC AES256 calculation workspace using a pre-calculated CMAC
 *                 expanded key.
 *                 This will speed up the CMAC verification process.
 *  \param[in,out] workSpace          CMAC work space
 *  \param[in]     expandedKey        CMAC expanded key which shall be used
 *  \param[in]     expandedKeyLength  Size of provided key
 *  \return        ESL_ERC_NO_ERROR                      Operation successful
 *                 ESL_ERC_PARAMETER_INVALID             Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL                  Work space too small
 *                 ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE  AES key length is invalid
 *  \pre           Pointers must be valid
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initExpandedCMACAES256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCMACAES) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) expandedKey,
  const eslt_Length expandedKeyLength);

/***********************************************************************************************************************
 *  esl_getExpandedKeyCMACAES256
 **********************************************************************************************************************/
/*! \brief         Extracts the expanded CMAC key.
 *  \details       This function extracts the expanded CMAC key from the given workspace. Re-using the expanded key
 *                 on a system with constant key usage will speed up the CMAC calculation.
 *  \param[in]     workSpace     CMAC work space
 *  \param[out]    keyPtr        Pointer to buffer where the expanded key shall be stored.
 *  \param[in,out] keyLengthPtr  In  Size of provided buffer, Out  Size of written data
 *  \return        ESL_ERC_NO_ERROR           Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL       Work space too small
 *                 ESL_ERC_WS_STATE_INVALID   Work space state invalid
 *                 ESL_ERC_INPUT_INVALID      Invalid input length
 *  \pre           Workspace needs to be initialized.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_getExpandedKeyCMACAES256(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceCMACAES) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) keyPtr,
  VSECPRIM_P2VAR_PARA(eslt_Length) keyLengthPtr);

/***********************************************************************************************************************
 *  esl_updateCMACAES256
 **********************************************************************************************************************/
/*! \brief         Update CMAC AES256 calculation.
 *  \details       This function is used to feed the CMAC AES256 calculation with input data.
 *  \param[in,out] workSpace    CMAC work space
 *  \param[in]     inputLength  Length of input data
 *  \param[in]     input        Pointer to input data
 *  \return        ESL_ERC_NO_ERROR           Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID   Work space state invalid
 *  \pre           Workspace needs to be initialized.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateCMACAES256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCMACAES) workSpace,
  const eslt_Length inputLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/***********************************************************************************************************************
 *  esl_finalizeCMACAES256
 **********************************************************************************************************************/
/*! \brief         Finalize CMAC AES256 calculation.
 *  \details       This function finalizes the CMAC AES256 calculation.
 *  \param[in,out] workSpace         (in)  pointer to CMAC work space
 *                                   (out) pointer to CMAC work space
 *  \param[in,out] messageCMAC       (in)  pointer to buffer for CMAC
 *                                   (out) Pointer to computed CMAC.
 *  \param[in,out] messageCMACLength (in)  pointer to buffer storing the length of the buffer referenced by messageCMAC
 *                                   (out) pointer to buffer storing the length of the data written to messageCMAC
 *  \return        ESL_ERC_NO_ERROR           Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL       Work space too small
 *                 ESL_ERC_WS_STATE_INVALID   Work space state invalid
 *                 ESL_ERC_INPUT_INVALID      Invalid input length
 *                 ESL_ERC_BUFFER_TOO_SMALL   buffer referenced by messageCMAC not sufficient to store CMAC
 *  \pre           Workspace needs to be initialized.
 *                 The buffer referenced by messageCMAC must provide at least *messageCMACLength bytes.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeCMACAES256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCMACAES) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageCMAC,
  VSECPRIM_P2VAR_PARA(eslt_Length) messageCMACLength);

/***********************************************************************************************************************
 *  esl_verifyCMACAES256
 **********************************************************************************************************************/
/*! \brief         Verify given CMAC based on AES256.
 *  \details       This function verifies the provided CMAC based on AES256 against the calculated one.
 *  \param[in,out] workSpace      CMAC work space
 *  \param[in]     messageCMAC    Pointer to CMAC to be compared. The size must be 16 bytes.
 *  \return        ESL_ERC_NO_ERROR           CMAC is correct
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL       Work space too small
 *                 ESL_ERC_WS_STATE_INVALID   Work space state invalid
 *  \pre           Workspace needs to be initialized.
 *                 The buffer referenced by messageCMAC must provide at least ESL_SIZOF_AES256BLOCK (= 16) bytes.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyCMACAES256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCMACAES) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) messageCMAC);

# endif /* (VSECPRIM_CMAC_ENABLED == STD_ON) */

# if (VSECPRIM_SIP_HASH_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  esl_initSipHash
 **********************************************************************************************************************/
/*! \brief         Initializes SipHash calculation.
 *  \details       This function initializes SipHash calculation.
 *  \param[in,out] workSpace  SipHash work space
 *  \param[in]     keyLength  Size of SipHash key to be used
 *  \param[in]     key  SipHash key to be used
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE  SipHash key length is invalid
 *  \pre           Pointers must be valid
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSipHash(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSipHash) workSpace,
  const eslt_Length keyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) key);

/***********************************************************************************************************************
 *  esl_updateSipHash
 **********************************************************************************************************************/
/*! \brief         Updates SipHash calculation.
 *  \details       This function updates SipHash calculation.
 *  \param[in,out] workSpace  SipHash work space
 *  \param[in]     inputLength  Length of input data
 *  \param[in]     input  Pointer to input data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *                 ESL_ERC_INPUT_INVALID  Invalid input length
 *  \pre           Pointers must be valid
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSipHash(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSipHash) workSpace,
  const eslt_Length inputLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/***********************************************************************************************************************
 *  esl_finalizeSipHash
 **********************************************************************************************************************/
/*! \brief         Finalizes SipHash calculation.
 *  \details       This function finalizes SipHash calculation.
 *  \param[in,out] workSpace  SipHash work space
 *  \param[out]    messageMAC  Pointer to computed MAC. Size must be 8 bytes.
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *  \pre           Pointers must be valid
 *                 messageMAC must point to valid memory of at least 8 bytes
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSipHash(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSipHash) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageMAC);

/***********************************************************************************************************************
 *  esl_verifySipHash
 **********************************************************************************************************************/
/*! \brief         Verifies given SipHash against calculated one.
 *  \details       This function verifies given SipHash against calculated one.
 *  \param[in,out] workSpace  SipHash work space
 *  \param[in]     messageMAC  Pointer to SipHash to be compared. Size must be 8 bytes.
 *  \return        ESL_ERC_NO_ERROR  SipHash is correct
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *                 ESL_ERC_HMAC_INCORRECT_MAC   the given HashMAC is incorrect
 *  \pre           Pointers must be valid
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySipHash(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSipHash) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) messageMAC);
# endif /* (VSECPRIM_SIP_HASH_ENABLED == STD_ON) */

/*****************************************************************************
 * Key derivation function (KDF) according to ANSI X9.63
 *****************************************************************************/
# if (VSECPRIM_ANSI_X963_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  esl_initKDFX963SHA1
 **********************************************************************************************************************/
/*! \brief         Initializes KDF X9.63 with SHA1 calculation.
 *  \details       This function initializes KDF X9.63 with SHA1 calculation.
 *  \param[in,out] workSpace  KDF X9.63 with SHA1 work space
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *  \pre           workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initKDFX963SHA1(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDFX963SHA1) workSpace);

/***********************************************************************************************************************
 *  esl_deriveKeyKDFX963SHA1
 **********************************************************************************************************************/
/*! \brief         Derive Key according KDF X9.63 with SHA1
 *  \details       This function derives a key according to KDF X9.63 with SHA1.
 *  \param[in,out] workSpace  KDF X9.63 with SHA1 work space
 *  \param[in]     secretLength   length of secret in bytes
 *  \param[in]     secret         Pointer to the secret
 *  \param[in]     infoLength     length of salt in bytes (if zero, no salt is used)
 *  \param[in]     info           pointer to the optional salt (if NULL, no salt is used)
 *  \param[in]     keyLength      length of output key buffer in bytes
 *  \param[out]    key            pointer to the output key buffer
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *                 ESL_ERC_INPUT_INVALID  Invalid input length
 *  \pre           workSpace is initialized by esl_initKDFX963SHA1(..)
 *                 key need to have at least the length of keyLength.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_deriveKeyKDFX963SHA1(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDFX963SHA1) workSpace,
  const eslt_Length secretLength, VSECPRIM_P2CONST_PARA(eslt_Byte) secret,
  const eslt_Length infoLength, VSECPRIM_P2CONST_PARA(eslt_Byte) info,
  const eslt_Length keyLength, VSECPRIM_P2VAR_PARA(eslt_Byte) key);
# endif /* (VSECPRIM_ANSI_X963_ENABLED == STD_ON) */

# if (VSECPRIM_ANSI_X963_SHA256_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  esl_initKDFX963SHA256
 **********************************************************************************************************************/
/*! \brief         Initializes KDF X9.63 with SHA256 calculation.
 *  \details       This function initializes KDF X9.63 with SHA256 calculation.
 *  \param[in,out] workSpace  KDF X9.63 with SHA256 work space
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *  \pre           workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initKDFX963SHA256(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDFX963SHA256) workSpace);

/***********************************************************************************************************************
 *  esl_deriveKeyKDFX963SHA256
 **********************************************************************************************************************/
/*! \brief         Derive Key according KDF X9.63 with SHA256
 *  \details       This function derives a key according to KDF X9.63 with SHA256.
 *  \param[in,out] workSpace  KDF X9.63 with SHA256 work space
 *  \param[in]     secretLength   length of secret in bytes
 *  \param[in]     secret         Pointer to the secret
 *  \param[in]     infoLength     length of salt in bytes (if zero, no salt is used)
 *  \param[in]     info           pointer to the optional salt (if NULL, no salt is used)
 *  \param[in]     keyLength      length of output key buffer in bytes
 *  \param[out]    key            pointer to the output key buffer
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *                 ESL_ERC_INPUT_INVALID  Invalid input length
 *  \pre           workSpace is initialized by esl_initKDFX963SHA256(..)
 *                 Length of the buffer provided by key need to have at least keyLength.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_deriveKeyKDFX963SHA256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDFX963SHA256) workSpace,
  const eslt_Length secretLength, VSECPRIM_P2CONST_PARA(eslt_Byte) secret,
  const eslt_Length infoLength, VSECPRIM_P2CONST_PARA(eslt_Byte) info,
  const eslt_Length keyLength, VSECPRIM_P2VAR_PARA(eslt_Byte) key);
# endif /* (VSECPRIM_ANSI_X963_SHA256_ENABLED == STD_ON) */

# if (VSECPRIM_ANSI_X963_SHA512_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  esl_initKDFX963SHA512
 **********************************************************************************************************************/
/*! \brief         Initializes KDF X9.63 with SHA512 calculation.
 *  \details       This function initializes KDF X9.63 with SHA512 calculation.
 *  \param[in,out] workSpace  KDF X9.63 with SHA512 work space
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *  \pre           workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initKDFX963SHA512(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDFX963SHA512) workSpace);

/***********************************************************************************************************************
 *  esl_deriveKeyKDFX963SHA512
 **********************************************************************************************************************/
/*! \brief         Derive Key according KDF X9.63 with SHA512
 *  \details       This function derives a key according to KDF X9.63 with SHA512.
 *  \param[in,out] workSpace  KDF X9.63 with SHA512 work space
 *  \param[in]     secretLength   length of secret in bytes
 *  \param[in]     secret         Pointer to the secret
 *  \param[in]     infoLength     length of salt in bytes (if zero, no salt is used)
 *  \param[in]     info           pointer to the optional salt (if NULL, no salt is used)
 *  \param[in]     keyLength      length of output key buffer in bytes
 *  \param[out]    key            pointer to the output key buffer
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *                 ESL_ERC_INPUT_INVALID  Invalid input length
 *  \pre           workSpace is initialized by esl_initKDFX963SHA512(..)
 *                 Length of the buffer provided by key need to have at least keyLength.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_deriveKeyKDFX963SHA512(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDFX963SHA512) workSpace,
  const eslt_Length secretLength, VSECPRIM_P2CONST_PARA(eslt_Byte) secret,
  const eslt_Length infoLength, VSECPRIM_P2CONST_PARA(eslt_Byte) info,
  const eslt_Length keyLength, VSECPRIM_P2VAR_PARA(eslt_Byte) key);
# endif /* (VSECPRIM_ANSI_X963_SHA512_ENABLED == STD_ON) */

# if (VSECPRIM_PKCS5_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * Crypto functions KEY-derive according to PKCS#5 v2.0
 *****************************************************************************/

 /**********************************************************************************************************************
 *  esl_initKDF2HMACSHA1()
 *********************************************************************************************************************/
/*! \brief          Initialize the key derivation for KDF with underlying HMAC SHA1 primitive.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      iterationCount  number of (SHA-1) iterations
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_KDF_ITERATION_COUNT_OUT_OF_RANGE
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initKDF2HMACSHA1(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDF2HMACSHA1) workSpace, const eslt_Length iterationCount);

/***********************************************************************************************************************
 *  esl_deriveKeyKDF2HMACSHA1
 **********************************************************************************************************************/
/*! \brief         Derive Key according KDF2 with SHA1
 *  \details       -
 *  \param[in,out] workSpace algorithm context buffer, initialized by
 *                           esl_initWorkSpaceHeader(..) function
 *  \param[in]     secretLength   length of secret in bytes
 *  \param[in]     secret         Pointer to the secret
 *  \param[in]     infoLength     length of the shared info (bytes)
 *  \param[in]     info           pointer to the shared info
 *  \param[in]     keyLength      length of output key buffer in bytes
 *  \param[out]    key            pointer to the output key buffer
 *  \return        ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_WS_STATE_INVALID Invalid state
 *                 ESL_ERC_NO_ERROR          else
 *  \pre           workSpace is initialized by esl_initKDF2HMACSHA1(..)
 *                 key need to have at least the length of keyLength.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_deriveKeyKDF2HMACSHA1(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDF2HMACSHA1) workSpace,
  const eslt_Length secretLength, VSECPRIM_P2CONST_PARA(eslt_Byte) secret,
  const eslt_Length infoLength, VSECPRIM_P2CONST_PARA(eslt_Byte) info,
  const eslt_Length keyLength, VSECPRIM_P2VAR_PARA(eslt_Byte) key);
# endif /* (VSECPRIM_PKCS5_ENABLED == STD_ON) */

# if (VSECPRIM_HKDF_SHA2_256_ONESTEP_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  esl_initHKDF_Hash_OneStep
 **********************************************************************************************************************/
 /*! \brief         Initializes hash based HKDF one step method.
  *  \details       This function initializes the one step HKDF, based on any hash function (Option 1).
  *                 Currently, only SHA2-256 is supported.
  *  \param[in,out] workSpace  (in)  HKDF hash work space
  *                            (out) HKDF hash work space
  *  \param[in]     hashID     flag which indicates which hash function shall be used; currently only ESL_HA_SHA2_256
  *                            is accepted.
  *  \return        ESL_ERC_NO_ERROR           Operation successful
  *                 ESL_ERC_WS_TOO_SMALL       Work space too small
  *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
  *                 ESL_ERC_INPUT_INVALID      hashID != ESL_HA_SHA2_256
  *  \pre           workSpace is initialized by esl_initWorkSpaceHeader(..)
  *  \context       TASK
  *  \reentrant     TRUE, for different workspaces.
  *  \synchronous   TRUE
  **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initHKDF_Hash_OneStep(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHKDFHASH) workSpace,
  eslt_HashAlgorithm hashID);

/***********************************************************************************************************************
 *  esl_deriveKeyHKDF_Hash_OneStep
 **********************************************************************************************************************/
 /*! \brief         Derive Key according to hash based HKDF one step method.
  *  \details       This function derives a key according to HKDF one step method, based on any hash function (Option 1).
  *                 Currently, only SHA2-256 is supported.
  *                 The hash is determined by the hashID flag, a parameter for esl_initHKDF_Hash_OneStep, which is
  *                 stored in the workspace.
  *  \param[in,out] workSpace      (in)  HKDF hash work space
  *                                (out) HKDF hash work space
  *  \param[in]     secretLength   length of secret in bytes
  *  \param[in]     secret         Pointer to the secret
  *  \param[in]     infoLength     length of additional input in bytes, can be zero
  *  \param[in]     info           pointer to the optional additional input, can be NULL
  *  \param[in]     keyLength      desired length of the key to be derived
  *  \param[out]    key            pointer to the output key buffer
  *  \return        ESL_ERC_NO_ERROR           Operation successful
  *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
  *                 ESL_ERC_INPUT_INVALID      secretLength = 0 or keyLength = 0
  *                 ESL_ERC_WS_TOO_SMALL       Work space too small
  *                 ESL_ERC_WS_STATE_INVALID   Work space state invalid
  *                 ESL_ERC_INVALID_LENGTH     Invalid input length, i.e., keyLength > 4 294 967 264 bytes
  *  \pre           workSpace is initialized by esl_initHKDF_Hash_OneStep(..)
  *                 The buffer referenced by key needs to provide at least keyLength many bytes.
  *  \context       TASK
  *  \reentrant     TRUE, for different workspaces.
  *  \synchronous   TRUE
  **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_deriveKeyHKDF_Hash_OneStep(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHKDFHASH) workSpace,
  const eslt_Length secretLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) secret,
  const eslt_Length infoLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) info,
  const eslt_Length keyLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) key);
# endif /* (VSECPRIM_HKDF_SHA2_256_ONESTEP_ENABLED == STD_ON) */

# if (VSECPRIM_HKDF_HMAC_SHA256_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  esl_initHKDF_HMAC_SHA256
 **********************************************************************************************************************/
/*! \brief         Initializes HKDF HMAC with SHA256 calculation.
 *  \details       This function initializes one step (Option 2) or two step HKDF HMAC with SHA256 calculation.
 *  \param[in,out] workSpace  (in)  HKDF HMAC SHA256 work space
 *                            (out) HKDF HMAC SHA256 work space
 *  \param[in]     mode       HKDF mode, indicating whether the one-step or the two-step variant shall be used
 *  \return        ESL_ERC_NO_ERROR           Operation successful
 *                 ESL_ERC_WS_TOO_SMALL       Work space too small
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_MODE_INVALID       mode neither ESL_HKDF_ONE_STEP_VARIANT nor ESL_HKDF_TWO_STEP_VARIANT
 *  \pre           workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initHKDF_HMAC_SHA256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHKDFHMAC) workSpace,
  eslt_HKDFMode mode);

/***********************************************************************************************************************
 *  esl_deriveKeyHKDF_HMAC_SHA256
 **********************************************************************************************************************/
/*! \brief         Derive Key according to HKDF HMAC with SHA256
 *  \details       This function derives a key according to HKDF HMAC with SHA256.
 *                 Either the one-step method (Option 2) or the two-step method (extraction and expansion step)
 *                 is performed. The method is determined by the mode flag, a parameter for esl_initHKDF_HMAC_SHA256,
 *                 which is stored in the workspace.
 *  \param[in,out] workSpace      (in)  HKDF HMAC with SHA256 work space
 *                                (out) HKDF HMAC with SHA256 work space
 *  \param[in]     secretLength   length of secret in bytes
 *  \param[in]     secret         Pointer to the secret
 *  \param[in]     saltLength     length of salt in bytes (if zero, an all zero vector is used instead)
 *  \param[in]     salt           pointer to the optional salt (if NULL, an all zero vector is used instead)
 *  \param[in]     infoLength     length of additional input in bytes, can be zero
 *  \param[in]     info           pointer to the optional additional input, can be NULL
 *  \param[in]     keyLength      desired length of the key to be derived in bytes
 *  \param[out]    key            pointer to the output key buffer
 *  \return        ESL_ERC_NO_ERROR           Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_INPUT_INVALID      secretLength = 0 or keyLength = 0
 *                 ESL_ERC_WS_TOO_SMALL       Work space too small
 *                 ESL_ERC_WS_STATE_INVALID   Work space state invalid
 *                 ESL_ERC_INVALID_LENGTH     Invalid input length, i.e., 
 *                                            keyLength > 4 294 967 264 bytes for the one step method, or
 *                                            keyLength > 255*SHA256 digest length (in bytes) for the two step method
 *  \pre           workSpace is initialized by esl_initHKDF_HMAC_SHA256(..)
 *                 The buffer referenced by key needs to provide at least keyLength many bytes.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_deriveKeyHKDF_HMAC_SHA256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHKDFHMAC) workSpace,
  const eslt_Length secretLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) secret,
  const eslt_Length saltLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
  const eslt_Length infoLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) info,
  const eslt_Length keyLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) key);
# endif /* VSECPRIM_HKDF_HMAC_SHA256_ENABLED == STD_ON */

# if (VSECPRIM_HKDF_HMAC_SHA384_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/***********************************************************************************************************************
 *  esl_initHKDF_HMAC_SHA384
 **********************************************************************************************************************/
/*! \brief         Initializes HKDF HMAC with SHA384 calculation.
 *  \details       This function initializes one step (Option 2) or two step HKDF HMAC with SHA384 calculation.
 *  \param[in,out] workSpace  (in)  HKDF HMAC SHA384 work space
 *                            (out) HKDF HMAC SHA384 work space
 *  \param[in]     mode       HKDF mode, indicating whether the one-step or the two-step variant shall be used
 *  \return        ESL_ERC_NO_ERROR           Operation successful
 *                 ESL_ERC_WS_TOO_SMALL       Work space too small
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_MODE_INVALID       mode neither ESL_HKDF_ONE_STEP_VARIANT nor ESL_HKDF_TWO_STEP_VARIANT
 *  \pre           workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initHKDF_HMAC_SHA384(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHKDFHMAC) workSpace,
  eslt_HKDFMode mode);

/***********************************************************************************************************************
 *  esl_deriveKeyHKDF_HMAC_SHA384
 **********************************************************************************************************************/
/*! \brief         Derive Key according to HKDF HMAC with SHA384
 *  \details       This function derives a key according to HKDF HMAC with SHA384.
 *                 Either the one-step method (Option 2) or the two-step method (extraction and expansion step)
 *                 is performed. The method is determined by the mode flag, a parameter for esl_initHKDF_HMAC_SHA384,
 *                 which is stored in the workspace.
 *  \param[in,out] workSpace      (in)  HKDF HMAC with SHA384 work space
 *                                (out) HKDF HMAC with SHA384 work space
 *  \param[in]     secretLength   length of secret in bytes
 *  \param[in]     secret         Pointer to the secret
 *  \param[in]     saltLength     length of salt in bytes (if zero, an all zero vector is used instead)
 *  \param[in]     salt           pointer to the optional salt (if NULL, an all zero vector is used instead)
 *  \param[in]     infoLength     length of additional input in bytes, can be zero
 *  \param[in]     info           pointer to the optional additional input, can be NULL
 *  \param[in]     keyLength      desired length of the key to be derived in bytes
 *  \param[out]    key            pointer to the output key buffer
 *  \return        ESL_ERC_NO_ERROR           Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_INPUT_INVALID      secretLength = 0 or keyLength = 0
 *                 ESL_ERC_WS_TOO_SMALL       Work space too small
 *                 ESL_ERC_WS_STATE_INVALID   Work space state invalid
 *                 ESL_ERC_INVALID_LENGTH     Invalid input length, i.e., 
 *                                            keyLength > 4 294 967 248 bytes for the one step method, or
 *                                            keyLength > 255*SHA384 digest length (in bytes) for the two step method
 *  \pre           workSpace is initialized by esl_initHKDF_HMAC_SHA384(..)
 *                 The buffer referenced by key needs to provide at least keyLength many bytes.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_deriveKeyHKDF_HMAC_SHA384(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHKDFHMAC) workSpace,
  const eslt_Length secretLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) secret,
  const eslt_Length saltLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
  const eslt_Length infoLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) info,
  const eslt_Length keyLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) key);
# endif /* VSECPRIM_HKDF_HMAC_SHA384_ENABLED == STD_ON */

# if (VSECPRIM_PKCS5_HMAC_SHA256_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/**********************************************************************************************************************
 *  esl_initKDF2HMACSHA256()
 *********************************************************************************************************************/
/*! \brief         Initialize the key derivation for KDF with underlying HMAC SHA256 primitive.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      iterationCount  number of (SHA-256) iterations
 *  \return         ESL_ERC_PARAMETER_INVALID                input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL                     work space too small
 *                  ESL_ERC_KDF_ITERATION_COUNT_OUT_OF_RANGE iterationCount is 0
 *                  ESL_ERC_NO_ERROR                         else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initKDF2HMACSHA256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDF2HMACSHA256) workSpace, const eslt_Length iterationCount);

/***********************************************************************************************************************
 *  esl_deriveKeyKDF2HMACSHA256
 **********************************************************************************************************************/
/*! \brief         Derive Key according KDF2 with SHA256
 *  \details       -
 *  \param[in,out] workSpace algorithm context buffer, initialized by
 *                           esl_initWorkSpaceHeader(..) function
 *  \param[in]     secretLength   length of secret in bytes
 *  \param[in]     secret         Pointer to the secret
 *  \param[in]     infoLength     length of the shared info (bytes)
 *  \param[in]     info           pointer to the shared info
 *  \param[in]     keyLength      length of output key buffer in bytes
 *  \param[out]    key            pointer to the output key buffer
 *  \return        ESL_ERC_PARAMETER_INVALID  input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL       work space too small
 *                 ESL_ERC_WS_STATE_INVALID   invalid state
 *                 ESL_ERC_NO_ERROR           else
 *  \pre           workSpace is initialized by esl_initKDF2HMACSHA256(..)
 *                 key needs to have at least the length of keyLength
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
 extern VSECPRIM_FUNC(eslt_ErrorCode) esl_deriveKeyKDF2HMACSHA256(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDF2HMACSHA256) workSpace,
   const eslt_Length secretLength, VSECPRIM_P2CONST_PARA(eslt_Byte) secret,
   const eslt_Length infoLength, VSECPRIM_P2CONST_PARA(eslt_Byte) info,
   const eslt_Length keyLength, VSECPRIM_P2VAR_PARA(eslt_Byte) key);

# endif /* (VSECPRIM_PKCS5_HMAC_SHA256_ENABLED == STD_ON) */
# if (VSECPRIM_ESLGETLENGTHECP_ENABLED == STD_ON)
/*****************************************************************************
 * Crypto functions ECP (Ecc over Gf(p))
 *****************************************************************************/

/****************************************************************************
 * LengthOf-Functions
 ****************************************************************************
 * They can compute the bytelength of the i-n and outputparameters of EcP
 * functions */

/**********************************************************************************************************************
 *  esl_getMaxLengthOfEcPmessage()
 *********************************************************************************************************************/
/*! \brief          Returns the maximally possible byte length of a message to be signed or verified by the ECDSA
 *                   protocol.
 *  \details        -
 *  \param[in]      domain       pointer to domain parameter structure
 *  \return         0 if domain==NULL or domain decoding error,
 *                  maximally possible message length in bytes, else
 *  \pre            domain has to be a valid ASN1 structure for EC domain parameters
 *  \note           For information about the expected data format in domain see comment in file ESLib_t.h,
 *                  Section 'Algorithm specific types and constants'
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_Length) esl_getMaxLengthOfEcPmessage(VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain);

/**********************************************************************************************************************
 *  esl_getLengthOfEcPpublicKey_comp()
 *********************************************************************************************************************/
/*! \brief          Returns the byte length of a ECC public key
 *  \details        An ECC public key is a point on a curve. The returned length is the length of one coordinate
 *                  of the point (and not the joint length of all coordinates).
 *  \param[in]      domain       pointer to domain parameter structure
 *  \return         0 if domain==NULL or domain decoding error,
 *                  length of public key in bytes, else
 *  \pre            domain has to be a valid ASN1 structure for EC domain parameters
 *  \note           For information about the expected data format in domain see comment in file ESLib_t.h,
 *                  Section 'Algorithm specific types and constants'
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_Length) esl_getLengthOfEcPpublicKey_comp(VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain);

/**********************************************************************************************************************
 *  esl_getLengthOfEcPprivateKey()
 *********************************************************************************************************************/
/*! \brief          Returns the maximally possible byte length of the private key in an ECC protocol.
 *  \details        -
 *  \param[in]      domain       pointer to domain parameter structure
 *  \return         0 if domain==NULL or domain decoding error,
 *                  maximally possible byte length of the ECC private key, else
 *  \pre            domain has to be a valid ASN1 structure for EC domain parameters
 *  \note           For information about the expected data format in domain see comment in file ESLib_t.h,
 *                  Section 'Algorithm specific types and constants'
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_Length) esl_getLengthOfEcPprivateKey(VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain);

/**********************************************************************************************************************
 *  esl_getLengthOfEcPsignature_comp()
 *********************************************************************************************************************/
/*! \brief          Returns the maximally possible byte length of an ECDSA signature (for signature component r or s)
 *  \details        This function only returns the length of one component, r or s, and not the joint length of both
 *                  components together.
 *  \param[in]      domain       pointer to domain parameter structure
 *  \return         0 if domain==NULL or domain decoding error,
 *                  maximally possible byte length of ECDSA signature component
 *  \pre            domain has to be a valid ASN1 structure for EC domain parameters
 *  \note           For information about the expected data format in domain see comment in file ESLib_t.h,
 *                  Section 'Algorithm specific types and constants'
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_Length) esl_getLengthOfEcPsignature_comp(VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain);

/**********************************************************************************************************************
 *  esl_getLengthOfEcPsecret_comp()
 *********************************************************************************************************************/
/*! \brief          Returns the maximally possible length of a shared secret in an ECC protocol in bytes.
 *  \details        A shared secret in an ECC protocol like Diffie Hellman is a point on a curve. This function only
 *                  returns the length of one coordinate of this point and not the joint length of all coordinates
 *                  together.
 *  \param[in]      domain       pointer to domain parameter structure
 *  \return         0 if domain==NULL or domain decoding error,
 *                  maximally possible byte length of the shared secret, else
 *  \pre            domain has to be a valid ASN1 structure for EC domain parameters
 *  \note           For information about the expected data format in domain see comment in file ESLib_t.h,
 *                  Section 'Algorithm specific types and constants'
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_Length) esl_getLengthOfEcPsecret_comp(VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain);
# endif /* (VSECPRIM_ESLGETLENGTHECP_ENABLED == STD_ON) */

# if (VSECPRIM_ECCENABLED == STD_ON)
/*****************************************************************************
 * Crypto functions Key Generation based on EC-DH Generic
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_initGenerateDSAEcP_prim()
 *********************************************************************************************************************/
/*! \brief          This function initializes the EcP key generation.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      domain       pointer to domain parameter structure
 *  \param[in]      domainExt    pointer to domain parameter extension structure
 *  \param[in]      speedUpExt   pointer to precomputation structure
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_ECC_DOMAIN_INVALID      domain structure is invalid
 *                  ESL_ERC_ECC_DOMAINEXT_INVALID   domainExt structure is invalid
 *                  ESL_ERC_ECC_SPEEDUPEXT_INVALID  speedUpExt structure is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  domain and domainExt have to be valid ASN1 structures for EC domain parameters
 *                  speedUpExt has to be a valid ASN1 struct for precomputed EC points
 *                  The addresses of domain, domainExt and speedUpExt have to be valid until returning from
 *                  esl_generateKeyEcP_prim(), because the pointers to the data stored in domain, domainExt and
 *                  speedUpExt are stored in the workspace. These pointers are used as long as the workspace is used.
 *  \note           For information about the expected data format in domain, domainExt and speedUpExt see comment
 *                  in file ESLib_t.h, Section 'Algorithm specific types and constants'
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initGenerateKeyEcP_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace, VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomainExt) domainExt, VSECPRIM_P2ROMCONST_PARA(eslt_EccSpeedUpExt) speedUpExt);

/**********************************************************************************************************************
 *  esl_generateKeyEcP_prim()
 *********************************************************************************************************************/
/*! \brief          This function generates an ECC key pair
 *                      (d=privateKey, Q=(publicKey_x, publicKey_y)).
 *  \details        -
 *  \param[in,out] workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in,out]  privateKey   (in) buffer to store the private key d
 *                               (of length getLengthOfEcPprivateKey(domain))
 *                               (out) the private key d
 *  \param[in,out]  publicKey_x  (in) buffer to store x-coordinate of the public key
 *                               (of length getLengthOfEcPpublicKey_comp(domain))
 *                               (out) x-coordinate of the public key Q
 *  \param[in,out]  publicKey_y  (in) buffer to store y-coordinate of the public key
 *                               (of length getLengthOfEcPpublicKey_comp(domain))
 *                               (out) y-coordinate of the public key Q
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL            work space too small
 *                  ESL_ERC_WS_STATE_INVALID        wrong work space state
 *                  ESL_ERC_ECC_PRIVKEY_INVALID     privateKey is invalid
 *                  ESL_ERC_INTERNAL_ERROR          calculation of affine point representation from
 *                                                  projective Montgomery representation failed or
 *                                                  generation of random number by call-out failed
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initGenerateKeyEcP_prim(..)
 *                  The pointers privateKey, publicKey_x and publicKey_y passed to the function
 *                  esl_generateKeyEcP_prim must reference valid memory locations and the sizes of the arrays
 *                  referenced by the parameters privateKey, publicKey_x and publicKey_y must be greater than
 *                  or equal to the return values of
 *                  esl_getLengthOfEcPprivateKey(domain) for privateKey and
 *                  esl_getLengthOfEcPpublicKey_comp(domain) for publicKey_x and publicKey_y
 *                  in bytes, where domain is the ASN1 structure describing the domain parameters
 *                  of the underlying curve.
 *                  The maximally required buffer size is 32 byte, if ECP_MAX_KEY_SIZE = ECP_MAX_KEY_SIZE_256,
 *                  and 48 byte, if ECP_MAX_KEY_SIZE = ECP_MAX_KEY_SIZE_384. The interface of this function does not
 *                  pass the lengths of the arrays referenced by the pointers privateKey, publicKey_x and publicKey_y.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_generateKeyEcP_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) privateKey,
  VSECPRIM_P2VAR_PARA(eslt_Byte) publicKey_x, VSECPRIM_P2VAR_PARA(eslt_Byte) publicKey_y);
# endif /* (VSECPRIM_ECCENABLED == STD_ON) */

# if (VSECPRIM_ECDSA_GENERIC_ENABLED == STD_ON)
/*****************************************************************************
 * Crypto functions Digital Signature Generation / Verification based on EC-DH Generic
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_initSignDSAEcP_prim()
 *********************************************************************************************************************/
/*! \brief          This function initializes the DSAEcP sign algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      domain       pointer to domain parameter structure
 *  \param[in]      domainExt    pointer to domain parameter extension structure
 *  \param[in]      speedUpExt   pointer to precomputation structure
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_ECC_DOMAIN_INVALID      domain structure is invalid
 *                  ESL_ERC_ECC_DOMAINEXT_INVALID   domainExt structure is invalid
 *                  ESL_ERC_ECC_SPEEDUPEXT_INVALID  speedUpExt structure is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  domain and domainExt have to be valid ASN1 structures for EC domain parameters
 *                  speedUpExt has to be a valid ASN1 struct for precomputed EC points
 *                  The addresses of domain, domainExt and speedUpExt have to be valid until returning from
 *                  esl_initSignDSAEcP_prim(), because the pointers to the data stored in domain, domainExt and
 *                  speedUpExt are stored in the workspace. These pointers are used as long as the workspace is used.
 *  \note           For information about the expected data format in domain, domainExt and speedUpExt see comment
 *                  in file ESLib_t.h, Section 'Algorithm specific types and constants'
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignDSAEcP_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace, VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomainExt) domainExt, VSECPRIM_P2ROMCONST_PARA(eslt_EccSpeedUpExt) speedUpExt);

/**********************************************************************************************************************
 *  esl_signDSAEcP_prim()
 *********************************************************************************************************************/
/*! \brief          This function signs the given message with the passed private key.
 *  \details        -
 *  \param[in,out] workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      messageLength  message length in bytes
 *                               (<= esl_getMaxLengthOfEcPmessage(domain))
 *  \param[in]      message      pointer to message
 *  \param[in]      privateKey   pointer to private key
 *                               (of length getLengthOfEcPprivateKey(domain))
 *  \param[in,out]  signature_rLength  (in) signature_r buffer length in bytes
 *                               (>= esl_getLengthOfEcPsignature_comp(domain))
 *                               (out) signature_r length in bytes
 *  \param[in,out]  signature_r  (in) pointer to signature_r buffer
 *                               (out) signature_r
 *  \param[in,out]  signature_sLength  (in) signature_s buffer length in bytes
 *                               (>= esl_getLengthOfEcPsignature_comp(domain))
 *                               (out) signature_s length in bytes
 *  \param[in,out]  signature_s  (in) pointer to signature_s buffer
 *                               (out) signature_s
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL            workspace too small
 *                  ESL_ERC_WS_STATE_INVALID        wrong workspace state
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT   signature buffer too small
 *                  ESL_ERC_ECC_MESSAGE_TOO_LONG    wrong messageLength
 *                  ESL_ERC_ECC_PRIVKEY_INVALID     privateKey is invalid
 *                  ESL_ERC_INTERNAL_ERROR          calculation of affine point representation from
 *                                                  projective Montgomery representation failed
 *                  all possible error codes which can occur in esl_getBytesRNG
 *                  ESL_ERC_NO_ERROR                operation successful
 *  \pre            workSpace is initialized by esl_initSignDSAEcP_prim(..)
 *                  The buffer referenced by signature_r must provide at least signature_rLength bytes.
 *                  The buffer referenced by signature_s must provide at least signature_sLength bytes.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_signDSAEcP_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace,
  eslt_Length messageLength, VSECPRIM_P2CONST_PARA(eslt_Byte) message, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKey,
  VSECPRIM_P2VAR_PARA(eslt_Length) signature_rLength, VSECPRIM_P2VAR_PARA(eslt_Byte) signature_r,
  VSECPRIM_P2VAR_PARA(eslt_Length) signature_sLength, VSECPRIM_P2VAR_PARA(eslt_Byte) signature_s);

/**********************************************************************************************************************
 *  esl_initVerifyDSAEcP_prim()
 *********************************************************************************************************************/
/*! \brief          This function initializes the DSAEcP verification algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      domain       pointer to domain parameter structure
 *  \param[in]      domainExt    pointer to domain parameter extension structure
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_ECC_DOMAIN_INVALID      domain structure is invalid
 *                  ESL_ERC_ECC_DOMAINEXT_INVALID   domainExt structure is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  domain and domainExt have to be valid ASN1 structures for EC domain parameters
 *                  The addresses of domain and domainExt have to be valid until returning from
 *                  esl_verifyDSAEcP_prim(), because the pointers to the data stored in domain and domainExt
 *                  are stored in the workspace. These pointers are used as long as the workspace is used.
 *  \note           For information about the expected data format in domain and domainExt see comment
 *                  in file ESLib_t.h, Section 'Algorithm specific types and constants'
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initVerifyDSAEcP_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain, VSECPRIM_P2ROMCONST_PARA(eslt_EccDomainExt) domainExt);

/**********************************************************************************************************************
 *  esl_verifyDSAEcP_prim()
 *********************************************************************************************************************/
/*! \brief          This function verifies the given signature of the input message
 *                  with the passed public key.
 *  \details        -
 *  \param[in,out] workSpace           algorithm context buffer, initialized by
 *                                     esl_initWorkSpaceHeader(..) function
 *  \param[in]      messageLength      message length in bytes
 *                                     (<= esl_getMaxLengthOfEcPmessage(domain))
 *  \param[in]      message            pointer to message
 *  \param[in]      publicKey_x        pointer to x-coordinate of public key
 *                                     (of length getLengthOfEcPpublicKey_comp(domain))
 *  \param[in]      publicKey_y        pointer to y-coordinate of public key
 *                                     (of length getLengthOfEcPpublicKey_comp(domain))
 *  \param[in]      signature_rLength  signature_r length in bytes
 *  \param[in]      signature_r        pointer to signature_r
 *  \param[in]      signature_sLength  signature_slength in bytes
 *  \param[in]      signature_s        pointer to signature_s
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL            workspace too small
 *                  ESL_ERC_WS_STATE_INVALID        wrong work space state
 *                  ESL_ERC_ECC_MESSAGE_TOO_LONG    wrong messageLength
 *                  ESL_ERC_ECC_PUBKEY_INVALID      publicKey is invalid
 *                  ESL_ERC_ECC_SIGNATURE_INVALID   invalid signature
 *                  ESL_ERC_INTERNAL_ERROR          calculation of affine point representation from
 *                                                  Montgomery representation failed
 *                  ESL_ERC_NO_ERROR                else; signature valid
 *  \pre            workSpace is initialized by esl_initVerifyDSAEcP_prim(..)
 *                  The buffer referenced by signature_r must provide at least signature_rLength bytes.
 *                  The buffer referenced by signature_s must provide at least signature_sLength bytes.
 *  \note           If one of the coordinates publicKey_x or publicKey_y is shorter than
 *                  getLengthOfEcPpublicKey_comp(domain) bytes, the coordinate must be padded with zeros to reach
 *                  a total length of getLengthOfEcPpublicKey_comp(domain) bytes. 'domain' corresponds to the
 *                  EC domain parameter used in esl_initVerifyDSAEcP_prim(). 
 *                  The only return code that indicates that the signature is valid is ESL_ERC_NO_ERROR. Any other
 *                  return code implies that the signature shall not be accepted as valid.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyDSAEcP_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace,
  eslt_Length messageLength, VSECPRIM_P2CONST_PARA(eslt_Byte) message,
  VSECPRIM_P2CONST_PARA(eslt_Byte) publicKey_x, VSECPRIM_P2CONST_PARA(eslt_Byte) publicKey_y,
  eslt_Length signature_rLength, VSECPRIM_P2CONST_PARA(eslt_Byte) signature_r,
  eslt_Length signature_sLength, VSECPRIM_P2CONST_PARA(eslt_Byte) signature_s);
# endif /* (VSECPRIM_ECDSA_GENERIC_ENABLED == STD_ON) */

# if (VSECPRIM_ECDH_GENERIC_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * Crypto functions Key Exchange EC-DH Generic
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_initExchangeKeyDHEcP_key()
 *********************************************************************************************************************/
/*! \brief          This function initializes the DHEcP key exchange algorithm. For key
 *                  derivation KDF2HMACSHA1 of the library is used.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      domain       pointer to domain parameter structure
 *  \param[in]      domainExt    pointer to domain parameter extension structure
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_ECC_DOMAIN_INVALID      domain structure is invalid
 *                  ESL_ERC_ECC_DOMAINEXT_INVALID   domainExt structure is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  domain and domainExt have to be valid ASN1 structures for EC domain parameters
 *                  The addresses of domain and domainExt have to be valid until returning from
 *                  esl_exchangeKeyDHecP_key(), because the pointers to the data stored in domain and domainExt
 *                  are stored in the workspace. These pointers are used as long as the workspace is used.
 *  \note           For information about the expected data format in domain and domainExt see comment
 *                  in file ESLib_t.h, Section 'Algorithm specific types and constants'
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initExchangeKeyDHEcP_key(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain, VSECPRIM_P2ROMCONST_PARA(eslt_EccDomainExt) domainExt);

/**********************************************************************************************************************
 *  esl_exchangeKeyDHEcP_key()
 *********************************************************************************************************************/
/*! \brief          This function calculates the common secret d1 * Q2 (= d1 * d2 * G)
 *                  and derives the output key from the x-coordinate of the secret with
 *                  the KDF2HMACSHA1 derivation function of the library.
 *                     if (secret_x == publicKey_x) secret_x will replace publicKey_x
 *                     if (secret_y == publicKey_y) secret_y will replace publicKey_y
 *  \details        -
 *  \param[in,out] workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      privateKey   the private key d1 of the key pair (d1, Q1)
 *                               (of length getLengthOfEcPprivateKey(domain))
 *  \param[in]      publicKey_x  x-coordinate of the public key Q2 of the key pair (d2, Q2)
 *                               (of length getLengthOfEcPpublicKey_comp(domain))
 *  \param[in]      publicKey_y  y-coordinate of the public key Q2 of the key pair (d2, Q2)
 *                               (of length getLengthOfEcPpublicKey_comp(domain))
 *  \param[in]      infoLength   KDF2 salt length in bytes
 *  \param[in]      info         pointer to KDF2 salt (optional, maybe NULL)
 *  \param[in]      iterationCount  KDF2 key derivation iteration count
 *  \param[in]      keyLength    output key buffer length in bytes
 *  \param[in,out] key          output key buffer
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID        wrong work space state
 *                  ESL_ERC_ECC_PRIVKEY_INVALID     privateKey is invalid
 *                  ESL_ERC_ECC_PUBKEY_INVALID      publicKey is invalid
 *                  ESL_ERC_KDF_ITERATION_COUNT_OUT_OF_RANGE   iterationCount==0
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initExchangeKeyDHEcP_key(..)
 *                  The buffer referenced by key has to provide at least keyLength bytes.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_exchangeKeyDHEcP_key(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKey,
  VSECPRIM_P2CONST_PARA(eslt_Byte) publicKey_x, VSECPRIM_P2CONST_PARA(eslt_Byte) publicKey_y,
  eslt_Length infoLength, VSECPRIM_P2CONST_PARA(eslt_Byte) info, eslt_Length iterationCount,
  eslt_Length keyLength, VSECPRIM_P2VAR_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 *  esl_initGenerateSharedSecretDHEcP_prim()
 *********************************************************************************************************************/
/*! \brief          This function initializes the DHEcP generate secret algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      domain       pointer to domain parameter structure
 *  \param[in]      domainExt    pointer to domain parameter extension structure
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_ECC_DOMAIN_INVALID      domain structure is invalid
 *                  ESL_ERC_ECC_DOMAINEXT_INVALID   domainExt structure is invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  domain and domainExt have to be valid ASN1 structures for EC domain parameters
 *                  The addresses of domain and domainExt have to be valid until returning from
 *                  esl_generateSharedSecretDHEcP_prim(), because the pointers to the data stored in domain and
 *                  domainExt are stored in the workspace. These pointers are used as long as the workspace is used.
 *  \note           For information about the expected data format in domain and domainExt see comment
 *                  in file ESLib_t.h, Section 'Algorithm specific types and constants'
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initGenerateSharedSecretDHEcP_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain, VSECPRIM_P2ROMCONST_PARA(eslt_EccDomainExt) domainExt);

/**********************************************************************************************************************
 *  esl_generateSharedSecretDHEcP_prim()
 *********************************************************************************************************************/
/*! \brief          This function calculates the common secret d1 * Q2 (= d1 * d2 * G).
 *  \details        -
 *  \param[in,out] workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      privateKey   the private key d1 of the key pair (d1, Q1)
 *                               (of length getLengthOfEcPprivateKey(domain))
 *  \param[in]      publicKey_x  x-coordinate of the public key Q2 of the key pair (d2, Q2)
 *                               (of length getLengthOfEcPpublicKey_comp(domain))
 *  \param[in]      publicKey_y  y-coordinate of the public key Q2 of the key pair (d2, Q2)
 *                               (of length getLengthOfEcPpublicKey_comp(domain))
 *  \param[in,out]  secret_x     (in) buffer to store x-coordinate of the secret d1 * Q2
 *                               (of length getLengthOfEcPsecret_comp(domain))
 *                               (out) x-coordinate of the secret d1 * Q2
 *  \param[in,out]  secret_y     (in) buffer to store y-coordinate of the secret d1 * Q2
 *                               (of length getLengthOfEcPsecret_comp(domain))
 *                               (out) y-coordinate of the secret d1 * Q2
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID        wrong work space state
 *                  ESL_ERC_ECC_PRIVKEY_INVALID     privateKey is invalid
 *                  ESL_ERC_ECC_PUBKEY_INVALID      publicKey is invalid
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initGenerateSharedSecretDHEcP_prim(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_generateSharedSecretDHEcP_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKey,
  VSECPRIM_P2CONST_PARA(eslt_Byte) publicKey_x, VSECPRIM_P2CONST_PARA(eslt_Byte) publicKey_y,
  VSECPRIM_P2VAR_PARA(eslt_Byte) secret_x, VSECPRIM_P2VAR_PARA(eslt_Byte) secret_y);
# endif /* (VSECPRIM_ECDH_GENERIC_ENABLED == STD_ON) */

# if (VSECPRIM_SPAKE2_P_ENABLED == STD_ON)
/*****************************************************************************
 * Crypto functions SPAKE2+ protocol: preamble phase
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_initSPAKE2PPreamble()
 *********************************************************************************************************************/
/*! \brief          This function initializes the SPAKE2+ preamble phase.
 *  \details        In this function, the curve domain is parsed in order to find the field prime p, which is used
 *                  in a later step during the preamble phase.
 *  \param[in,out]  wsSpakePre   (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..)
 *                               (out) initialized algorithm context structure
 *  \param[in]      domain       pointer to domain parameter structure
 *  \param[in]      domainExt    pointer to domain parameter extension structure
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL or length values invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_ECC_DOMAIN_INVALID      domain structure is invalid
 *                  ESL_ERC_ECC_DOMAINEXT_INVALID   domainExt structure is invalid
 *                  ESL_ERC_CURVE_NOT_SUPPORTED     a different curve than P256 is used
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  domain and domainExt have to be valid ASN1 structures for EC domain parameters
 *                  The addresses of domain and domainExt have to be valid until returning from
 *                  esl_calcSPAKE2PPreamble(), because the pointers to the data stored in domain and domainExt
 *                  are stored in the workspace. These pointers are used as long as the workspace is used.
 *  \note           For information about the expected data format in domain and domainExt see comment
 *                  in file ESLib_t.h, Section 'Algorithm specific types and constants'
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSPAKE2PPreamble(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2PPreamble) wsSpakePre,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomainExt) domainExt);

/**********************************************************************************************************************
 *  esl_calcSPAKE2PPreamble()
 *********************************************************************************************************************/
/*! \brief          This function calculates the values w0 and w1 as w0 = w0s mod n, w1 = w1s mod n  and
 *                   L = w1*P according to the SPAKE2+ preamble phase.
 *  \details        w0s and w1s are assumed to be concatenated in the parameter 'pbkdfOutput'. n is the group order
 *                  and P is the curve base point.
 *  \param[in,out]  wsSpakePre         (in)  algorithm context buffer, initialized by esl_initSPAKE2PPreamble(..)
 *                                     (out) algorithm context buffer,
 *  \param[in]      pbkdfOutput        PBKDF output, [w0s || w1s] = PBKDF(...)
 *  \param[in]      pbkdfOutputLength  length of the buffer referenced by pbkdfOutput
 *  \param[in,out]  w0                 (in)  buffer to store the result w0 = w0s mod n
 *                                     (out) the result w0
 *  \param[in,out]  w0Length           (in)  length of buffer referenced by w0
 *                                     (out) data length (in bytes) written to w0
 *  \param[in,out]  w1                 (in)  buffer to store the result w1 = w1s mod n
 *                                     (out) the result w1
 *  \param[in,out]  w1Length           (in)  length of buffer referenced by w1
 *                                     (out) data length (in bytes) written to w1
 *  \param[in,out]  pointL             (in)  buffer to store the point L = w1*P in uncompressed representation,
 *                                           including the encoding byte 0x04
 *                                     (out) the point L = w1*P in uncompressed representation,
 *                                           including the encoding byte 0x04
 *  \param[in,out]  pointLLength       (in)  length of the buffer referenced by pointL
 *                                     (out) data length (in bytes) written to pointL
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL            work space too small
 *                  ESL_ERC_WS_STATE_INVALID        wrong work space state
 *                  ESL_ERC_BUFFER_TOO_SMALL        memory provided by output buffers not sufficient
 *                  ESL_ERC_INVALID_LENGTH          value of pbkdfOutputLength invalid (i.e., odd or too high)
 *                  ESL_ERC_ERROR                   an error occurred during the reduction or ECC operation
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initSPAKE2PPreamble(..)
 *  \note           The buffers referenced by w0 and w1 each must provide esl_getLengthOfEcPprivateKey()
 *                  many (i.e., group order n many) bytes.
 *                  The buffer referenced by pointL must provide at least esl_getLengthOfEcPpublicKey_comp()
 *                  many bytes (i.e., 2*p + 1 many bytes, where p is the field prime).
 *                  The buffer referenced by pbkdfOutput must provide at least pbkdfOutputLength many bytes.
 *                  The value of pbkdfOutputLength can be any even value between 0 and
 *                  VSECPRIM_SPAKE2_P_PREAMBLE_OKMLENGTH. In most cases
 *                  pbkdfOutputLength >= 2*esl_getLengthOfEcPprivateKey().
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_calcSPAKE2PPreamble(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2PPreamble) wsSpakePre,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pbkdfOutput,
  const eslt_Length pbkdfOutputLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) w0,
  VSECPRIM_P2VAR_PARA(eslt_Length) w0Length,
  VSECPRIM_P2VAR_PARA(eslt_Byte) w1,
  VSECPRIM_P2VAR_PARA(eslt_Length) w1Length,
  VSECPRIM_P2VAR_PARA(eslt_Byte) pointL,
  VSECPRIM_P2VAR_PARA(eslt_Length) pointLLength);

/*****************************************************************************
 * Crypto functions SPAKE2+ protocol: key exchange
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_initSPAKE2P()
 *********************************************************************************************************************/
/*! \brief          This function initializes the SPAKE2+ key exchange algorithm.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by esl_initWorkSpaceHeader(..)
 *                               (out) initialized algorithm context structure
 *  \param[in]      domain       pointer to domain parameter structure
 *  \param[in]      domainExt    pointer to domain parameter extension structure
 *  \param[in]      pointM       Point M on the curve in uncompressed representation, including the encoding byte 0x04
 *  \param[in]      mlength      length of buffer referenced by pointM
 *  \param[in]      pointN       Point N on the curve in uncompressed representation, including the encoding byte 0x04
 *  \param[in]      nlength      length of buffer referenced by pointN
 *  \param[in]      ciphersuite  parameter specifying the ciphersuite (hash function, key derivation function,
 *                               mac algorithm) and the version used by SPAKE (order of keys). Currently, only the
 *                               combination [SHA256 | HKDF | CMAC-AES-128] is supported.
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL or length values invalid or point encoding
 *                                                  is incorrect
 *                  ESL_ERC_WS_TOO_SMALL            work space too small
 *                  ESL_ERC_ECC_DOMAIN_INVALID      domain structure is invalid
 *                  ESL_ERC_ECC_DOMAINEXT_INVALID   domainExt structure is invalid
 *                  ESL_ERC_CURVE_NOT_SUPPORTED     a different curve than P256 is used
 *                  ESL_ERC_MODE_INVALID            value of ciphersuite is not consistent with supported ciphersuite
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  domain and domainExt have to be valid ASN1 structures for EC domain parameters
 *                  The addresses of domain and domainExt have to be valid until returning from
 *                  esl_calcSharedSecretSPAKE2P(), because the pointers to the data stored in domain and domainExt
 *                  are stored in the workspace. These pointers are used as long as the workspace is used.
 *  \note           For information about the expected data format in domain and domainExt see comment
 *                  in file ESLib_t.h, Section 'Algorithm specific types and constants'
 *                  The value of mlength and nlength must be (2*esl_getLengthOfEcPpublicKey_comp() + 1),
 *                  (i.e., 2*p + 1, where p is the field prime).
 *                  The points M and N can be found in file ESLib_SPAKE2PConstants.h.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSPAKE2P(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomainExt) domainExt,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointM,
  const eslt_Length mlength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointN,
  const eslt_Length nlength,
  eslt_SPAKE2PMode ciphersuite);

/**********************************************************************************************************************
 *  esl_setPreambleDataPartyASPAKE2P()
 *********************************************************************************************************************/
/*! \brief           This function provides data from the preamble phase to the SPAKE2+ algorithm for party A.
 *  \details         The provided data will be stored in the workspace. In this function the workspace also sets a
 *                   flag that the operations for party A shall be executed.
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by esl_initSPAKE2P(..)
 *                               (out) algorithm context structure, now storing the preamble data
 *  \param[in]      w0           value w0 (w0 is the first half of the OKM of a PBKDF mod n, computed during the
 *                               preamble phase)
 *  \param[in]      w0length     length of buffer referenced by w0
 *  \param[in]      w1           value w1 (w1 is the second half of the OKM of a PBKDF mod n, computed during the
 *                               preamble phase)
 *  \param[in]      w1length     length of buffer referenced by w1
 *  \return         ESL_ERC_PARAMETER_INVALID    input parameter is NULL or length values invalid
 *                  ESL_ERC_WS_TOO_SMALL         work space too small
 *                  ESL_ERC_WS_STATE_INVALID     wrong work space state
 *                  ESL_ERC_STATE_INVALID        esl_setPreambleDataPartyASPAKE2P() or
 *                                               esl_setPreambleDataPartyBSPAKE2P() was called before with
 *                                               the same workSpace
 *                  ESL_ERC_NO_ERROR             else
 *  \pre            workSpace is initialized by esl_initSPAKE2P(..)
 *  \note           The value of w0length and w1length must be esl_getLengthOfEcPprivateKey(), (i.e., n, where
 *                  n is the elliptic curve group order).
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_setPreambleDataPartyASPAKE2P(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) w0,
  const eslt_Length w0length,
  VSECPRIM_P2CONST_PARA(eslt_Byte) w1,
  const eslt_Length w1length);

/**********************************************************************************************************************
 *  esl_setPreambleDataPartyBSPAKE2P()
 *********************************************************************************************************************/
/*! \brief           This function provides data from the preamble phase to the SPAKE2+ algorithm for party B.
 *  \details         The provided data will be stored in the workspace. In this function the workspace also sets a
 *                   flag that the operations for party B shall be executed.
 *  \param[in,out]  workSpace     (in) algorithm context buffer, initialized by esl_initSPAKE2P(..)
 *                                (out) algorithm context structure, now storing the preamble data
 *  \param[in]      w0            value w0 (w0 is the first half of the OKM of a PBKDF mod n, computed during the
 *                                preamble phase)
 *  \param[in]      w0length      length of buffer referenced by w0
 *  \param[in]      pointL        point L in uncompressed representation, including the encoding byte 0x04
 *                                (L = w1*P, where w1 is the second half of the OKM of a PBKDF mod p,
 *                                computed during the preamble phase, and P is the base point of the curve)
 *  \param[in]      pointLLength  length of buffer referenced by pointL
 *  \return         ESL_ERC_PARAMETER_INVALID    input parameter is NULL or length values invalid or point encoding
 *                                               invalid
 *                  ESL_ERC_WS_TOO_SMALL         work space too small
 *                  ESL_ERC_WS_STATE_INVALID     wrong work space state
 *                  ESL_ERC_STATE_INVALID        esl_setPreambleDataPartyASPAKE2P() or
 *                                               esl_setPreambleDataPartyBSPAKE2P() was called before with
 *                                               the same workSpace
 *                  ESL_ERC_NO_ERROR             else
 *  \pre            workSpace is initialized by esl_initSPAKE2P(..)
 *  \note           The value of w0length must be esl_getLengthOfEcPprivateKey(), (i.e., n, where n is the
 *                  elliptic curve group order).
 *                  The value of pointLLength must be (2*esl_getLengthOfEcPpublicKey_comp() + 1), (i.e., 2*p + 1,
 *                  where p is the field prime).
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_setPreambleDataPartyBSPAKE2P(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) w0,
  const eslt_Length w0length,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointL,
  const eslt_Length pointLLength);

/**********************************************************************************************************************
 *  esl_calcPubValSPAKE2P()
 *********************************************************************************************************************/
/*! \brief          This function calculates the public value for party A or party B according to the SPAKE2+ protocol.
 *  \details        The workspace stores a flag indicating whether the operations for party A or for party B shall be
 *                  executed, i.e., this function computes
 *                  - X = pA = x*P + w0*M or
 *                  - Y = pB = y*P + w0*N
 *                  The random number x (or y) is generated using the RNG which is implemented by the call back
 *                  function esl_getBytesRNG(..).
 *  \param[in,out] workSpace        (in)  algorithm context buffer, initialized by esl_initSPAKE2P
 *                                  (out) algorithm context buffer
 *  \param[in,out]  pubVal_x        (in)  buffer to store the x-coordinate of the public value X or Y
 *                                        (of length getLengthOfEcPpublicKey_comp(domain))
 *                                  (out) the x-coordinate of the public value X or Y
 *  \param[in,out]  pubVal_y        (in)  buffer to store the y-coordinate of the public value X or Y
 *                                        (of length getLengthOfEcPpublicKey_comp(domain))
 *                                  (out) the y-coordinate of the public value X or Y
 *  \param[in,out]  pubValLength    (in)  length of the buffers referenced by pubVal_x and pubVal_y
 *                                  (out) length of the data in byte written to pubVal_x and pubVal_y
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL            work space too small
 *                  ESL_ERC_WS_STATE_INVALID        wrong work space state
 *                  ESL_ERC_STATE_INVALID           no preamble data was set before calling esl_calcPubValSPAKE2P()
 *                  ESL_ERC_BUFFER_TOO_SMALL        memory provided by output buffers not sufficient
 *                  ESL_ERC_ERROR                   calculation of public value failed
 *                  all possible error codes of esl_getBytesRNG()
 *                  ESL_ERC_ECC_INTERNAL_ERROR      curve point operation failed
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initSPAKE2P(..), esl_setPreambleDataPartyASPAKE2P() or
 *                  esl_setPreambleDataPartyBSPAKE2P() called before
 *                  The two buffers referenced by pubVal_x and pubVal_y each have to provide at least *pubValLength
 *                  bytes.
 *  \note           The value of pubValLength is expected to be esl_getLengthOfEcPpublicKey_comp(), (i.e., p, where
 *                  p is the field prime)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_calcPubValSPAKE2P(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) pubVal_x,
  VSECPRIM_P2VAR_PARA(eslt_Byte) pubVal_y,
  VSECPRIM_P2VAR_PARA(eslt_Length) pubValLength);

/**********************************************************************************************************************
 *  esl_setAdditionalInformationSPAKE2P()
 *********************************************************************************************************************/
/*! \brief           This optional function provides additional input data for the derivation of the shared secret
 *                   according to the SPAKE2+ protocol.
 *  \details         The provided data will be stored in the workspace. This function does not distinguish between
 *                   the two parties A and B. Calling this function is optional.
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by esl_initSPAKE2P(..)
 *                               (out) algorithm context structure
 *  \param[in]      infoA        additional information concering party A (e.g. A's name,...) or NULL
 *  \param[in]      infoALength  length of buffer referenced by infoA, possibly 0
 *  \param[in]      infoB        additional information concering party B (e.g. B's name,...) or NULL
 *  \param[in]      infoBLength  length of buffer referenced by infoB, possibly 0
 *  \param[in]      aad          additional authentication data, used as input for the KDF, or NULL
 *  \param[in]      aadLength    length of buffer referenced by aad, possibily 0
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL or length values invalid
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_WS_STATE_INVALID  wrong work space state
 *                  ESL_ERC_STATE_INVALID     esl_calcSharedSecretSPAKE2P() already called before
 *                  ESL_ERC_INVALID_LENGTH    aadLength greater than configured maximal aad length
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initSPAKE2P(..)
 *  \note           The call to this function is optional. This function can be called at any time after the call to
 *                  esl_initSPAKE2P() and before the call to esl_calcSharedSecretSPAKE2P().
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_setAdditionalInformationSPAKE2P(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) infoA,
  const eslt_Length infoALength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) infoB,
  const eslt_Length infoBLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) aad,
  const eslt_Length aadLength);

/**********************************************************************************************************************
 *  esl_calcSharedSecretSPAKE2P()
 *********************************************************************************************************************/
/*! \brief         This function calculates the common secret Ke, where Ka||Ke = Hash(TT), the MAC keys and the
 *                 confirmation MAC according to the SPAKE2+ protocol.
 *  \details       The MAC keys are not written to output buffers, but only stored in the workspace.
 *  \param[in,out] workSpace    (in)  algorithm context buffer, initialized by esl_initSPAKE2P(..)
 *                              (out) algorithm context buffer, now storing the common values Z and V, the shared secret
 *                                    and the MAC keys
 *  \param[in]      pubVal_x     x-coordinate of the other party's public value
 *  \param[in]      pubVal_y     y-coordinate of the other party's public value
 *  \param[in]      pubValLength length of the buffers referenced by pubVal_x and pubVal_y
 *  \param[in,out]  sharedSecret (in)  buffer to store the secret Ke
 *                               (out) secret Ke
 *  \param[in,out]  sharedSecretLength (in)  length of buffer referenced by sharedSecret
 *                                     (out) data length (in bytes) written to sharedSecret
 *  \param[in,out]  confirmationMAC    (in)  buffer to store confirmation MAC
 *                                     (out) confirmation MAC
 *  \param[in,out]  confirmationMACLength (in)  length of buffer referenced by confirmationMAC
 *                                        (out) data length (in bytes) written to confirmationMAC
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL or pubValLength is invalid
 *                  ESL_ERC_WS_TOO_SMALL            work space too small
 *                  ESL_ERC_WS_STATE_INVALID        wrong work space state
 *                  ESL_ERC_STATE_INVALID           esl_calcPubValSPAKE2P() was not called before
 *                  ESL_ERC_BUFFER_TOO_SMALL        memory provided by output buffers not sufficient
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initSPAKE2P(..), esl_calcPubValSPAKE2P() called before.
 *                  The buffer referenced by sharedSecret must provide at least *sharedSecretLength many bytes.
 *                  The buffer referenced by confirmationMAC must provide at least *confirmationMACLength many bytes.
 *  \note           The value of pubValLength must be esl_getLengthOfEcPpublicKey_comp(), (i.e., p, where
 *                  p is the field prime).
 *                  The buffers referenced by pubVal_x and pubVal_y both must provide at least pubValLength many bytes.
 *                  The buffer referenced by sharedSecret must provide at least (digest length/2) bytes, where
 *                  digest length is the digest length of the hash function specified by the cipher suite.
 *                  The buffer referenced by confirmationMAC must provide at least MAC length bytes, where MAC length
 *                  is the length of the MAC produced by the MAC algorithm specified by the cipher suite.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_calcSharedSecretSPAKE2P(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pubVal_x,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pubVal_y,
  const eslt_Length pubValLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) sharedSecret,
  VSECPRIM_P2VAR_PARA(eslt_Length) sharedSecretLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) confirmationMAC,
  VSECPRIM_P2VAR_PARA(eslt_Length) confirmationMACLength);

/**********************************************************************************************************************
 *  esl_confirmKeySPAKE2P()
 *********************************************************************************************************************/
/*! \brief         This function re-calculates the other party's confirmation MAC according to the SPAKE2+ protocol.
 *  \details       This function performs the required key confirmation by comparing the given MAC to the recomputed
 *                 one based on data stored in the workspace.
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by esl_initSPAKE2P(..)
 *  \param[in]      mac          other party's MAC to be verified
 *  \param[in]      macLength    length of mac in bytes
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL            work space too small
 *                  ESL_ERC_WS_STATE_INVALID        wrong work space state
 *                  ESL_ERC_STATE_INVALID           esl_calcSharedSecretSPAKE2P() was not called before
 *                  ESL_ERC_INVALID_LENGTH          macLength does not match the expected MAC length according to the
 *                                                  MAC algorithm specified in the cipher suite
 *                  ESL_ERC_INCORRECT_MAC           key confirmation failed, given MAC incorrect
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initSPAKE2P(..), esl_calcSharedSecretSPAKE2P() called before.
 *  \note           The value of macLength is expected to exactly match the MAC length of the MAC algorithm, specified
 *                  by the cipher suite.
 *                  The buffer referenced by mac must provide at least macLength many bytes.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_confirmKeySPAKE2P(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) mac,
  const eslt_Length macLength);
# endif

# if (VSECPRIM_RSA_CIPHER_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */ /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * Crypto functions RSA
 * according to PKCS#1 2.1
 * according to PKCS#1 primitives RSAEP, RSADP, RSASP1, RSAVP1
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_initDecryptRSA_prim()
 *********************************************************************************************************************/
/*! \brief          This function initializes the RSA decryption primitive (plain version).
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      keyPairModuleSize  the byte length of the modulus n
 *  \param[in]      keyPairModule  pointer to the modulus n
 *  \param[in]      privateKeyExponentSize  the byte length of the private exponent d
 *  \param[in]      privateKeyExponent  pointer to the private exponent d
 *  \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL               work space too small
 *                  ESL_ERC_RSA_PRIVKEY_INVALID        the private exponent d
 *                  is invalid
 *                  ESL_ERC_NO_ERROR                   else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptRSA_prim(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAdec_prim) workSpace,
    eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
    eslt_Length privateKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent);

/**********************************************************************************************************************
 *  esl_decryptRSA_prim()
 *********************************************************************************************************************/
/*! \brief          This function performs the RSA decryption operation (RSA_DP1).
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      cipherSize   the byte length of the input
 *  \param[in]      cipher       pointer to the input
 *  \param[in,out]  messageSize  pointer to byte length of the output buffer
 *  \param[in,out]  message      pointer to the output buffer
 *  \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 *                  ESL_ERC_RSA_CODE_OUT_OF_RANGE   (int) cipher > n-1
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT      *messageSize is too small
 *                  ESL_ERC_WS_STATE_INVALID           invalid Algo
 *                  ESL_ERC_NO_ERROR                   else
 *  \pre            workSpace is initialized by esl_initDecryptRSA_prim(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSA_prim(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAdec_prim) workSpace,
   eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
   VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message);

/**********************************************************************************************************************
 *  esl_initDecryptRSACRT_prim()
 *********************************************************************************************************************/
/*! \brief          This function initializes the RSA decryption primitive (CRT version).
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      keyPairPrimePSize  the byte length of the prime p
 *  \param[in]      keyPairPrimeP  pointer to prime p
 *  \param[in]      keyPairPrimeQSize  the byte length of the prime q
 *  \param[in]      keyPairPrimeQ  pointer to prime q
 *  \param[in]      privateKeyExponentDPSize  the byte length of dp
 *  \param[in]      privateKeyExponentDP  pointer to CRT key component dp
 *  \param[in]      privateKeyExponentDQSize  the byte length of dq
 *  \param[in]      privateKeyExponentDQ  pointer to CRT key component dq
 *  \param[in]      privateKeyInverseQISize  the byte length of CRT key component qInv
 *  \param[in]      privateKeyInverseQI  pointer to CRT key component qInv
 *  \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL               work space too small
 *                  ESL_ERC_RSA_PRIVKEY_INVALID        the private key
 *                  is invalid
 *                  ESL_ERC_NO_ERROR                   else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptRSACRT_prim(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTdec_prim) workSpace,
   eslt_Length   keyPairPrimePSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeP,
   eslt_Length   keyPairPrimeQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeQ,
   eslt_Length   privateKeyExponentDPSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDP,
   eslt_Length   privateKeyExponentDQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDQ,
   eslt_Length   privateKeyInverseQISize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyInverseQI);

/**********************************************************************************************************************
 *  esl_decryptRSACRT_prim()
 *********************************************************************************************************************/
/*! \brief          This function performs the RSA decryption operation (RSA_DP1).
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      cipherSize   the byte length of the input
 *  \param[in]      cipher       pointer to the input
 *  \param[in,out]  messageSize  pointer to byte length of the output buffer
 *  \param[in,out]  message      pointer to the output buffer
 *  \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 *                  ESL_ERC_RSA_CODE_OUT_OF_RANGE   (int) cipher > n-1
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT      *messageSize is too small
 *                  ESL_ERC_WS_STATE_INVALID           invalid Algo
 *                  ESL_ERC_NO_ERROR                   else
 *  \pre            workSpace is initialized by esl_initDecryptRSACRT_prim(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSACRT_prim(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTdec_prim) workSpace,
   eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
   VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message);
# endif /* (VSECPRIM_RSA_CIPHER_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_SIGNATURE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  esl_initSignRSA_prim()
 *********************************************************************************************************************/
/*! \brief          This function initializes the RSA signature primitive (plain version).
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      keyPairModuleSize  the byte length of the modulus n
 *  \param[in]      keyPairModule  pointer to the modulus n
 *  \param[in]      privateKeyExponentSize  the byte length of the private exponent d
 *  \param[in]      privateKeyExponent  pointer to the private exponent d
 *  \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL or exponent is larger than module
 *                  ESL_ERC_WS_TOO_SMALL               work space too small
 *                  ESL_ERC_RSA_PRIVKEY_INVALID        the private exponent d is invalid
 *                  ESL_ERC_NO_ERROR                   else
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSA_prim(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig_prim) workSpace,
     eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
     eslt_Length privateKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent);

/**********************************************************************************************************************
 *  esl_signRSA_prim()
 *********************************************************************************************************************/
/*! \brief          This function performs the RSA signature generation (RSA_SP1).
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      messageSize  the byte length of the input
 *  \param[in]      message      pointer to the input
 *  \param[in,out]  signatureSize  pointer to byte length of the output buffer
 *  \param[in,out]  signature    pointer to the output buffer
 *  \return         ESL_ERC_PARAMETER_INVALID          input parameter message is NULL
 *                  ESL_ERC_WS_TOO_SMALL               workSpace too small
 *                  ESL_ERC_WS_STATE_INVALID           workSpace in invalid state
 *                  ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE   message > module-1
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT      *signatureSize is too small
 *                  ESL_ERC_NO_ERROR                   else
 *  \pre            workSpace state set by esl_initSignRSA_prim(..)
 *                  the buffer referenced by signature must be at least of length signatureSize
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_signRSA_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig_prim) workSpace,
  eslt_Length messageSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) message,
  VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

#  if (VSECPRIM_RSACRT == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/**********************************************************************************************************************
 *  esl_initSignRSACRT_prim()
 *********************************************************************************************************************/
/*! \brief          This function initializes the RSA signature primitive (CRT version).
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      keyPairPrimePSize  the byte length of the prime p
 *  \param[in]      keyPairPrimeP  pointer to prime p
 *  \param[in]      keyPairPrimeQSize  the byte length of the prime q
 *  \param[in]      keyPairPrimeQ  pointer to prime q
 *  \param[in]      privateKeyExponentDPSize  the byte length of dp
 *  \param[in]      privateKeyExponentDP  pointer to CRT key component dp
 *  \param[in]      privateKeyExponentDQSize  the byte length of dq
 *  \param[in]      privateKeyExponentDQ  pointer to CRT key component dq
 *  \param[in]      privateKeyInverseQISize  the byte length of CRT key component qInv
 *  \param[in]      privateKeyInverseQI  pointer to CRT key component qInv
 *  \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL               work space too small
 *                  ESL_ERC_RSA_PRIVKEY_INVALID        the private key
 *                  is invalid
 *                  ESL_ERC_NO_ERROR                   else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSACRT_prim(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTsig_prim) workSpace,
   eslt_Length keyPairPrimePSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeP,
   eslt_Length keyPairPrimeQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeQ,
   eslt_Length privateKeyExponentDPSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDP,
   eslt_Length privateKeyExponentDQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDQ,
   eslt_Length privateKeyInverseQISize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyInverseQI);

/**********************************************************************************************************************
 *  esl_signRSACRT_prim()
 *********************************************************************************************************************/
/*! \brief          This function performs the RSA signature operation (RSA_SP1).
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      messageSize  the byte length of the input
 *  \param[in]      message      pointer to the input
 *  \param[in,out]  signatureSize  pointer to byte length of the output buffer
 *  \param[in,out]  signature    pointer to the output buffer
 *  \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 *                  ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE   (int) code > n-1
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT      *signatureSize is too small
 *                  ESL_ERC_WS_STATE_INVALID           invalid Algo
 *                  ESL_ERC_NO_ERROR                   else
 *  \pre            workSpace is initialized by esl_initSignRSACRT_prim(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_signRSACRT_prim(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTsig_prim) workSpace,
   eslt_Length messageSize, VSECPRIM_P2CONST_PARA(eslt_Byte) message,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize, VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

#  endif /* (VSECPRIM_RSACRT == STD_ON) */
# endif /* (VSECPRIM_RSA_SIGNATURE_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_CIPHER_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/**********************************************************************************************************************
 *  esl_initEncryptRSA_prim()
 *********************************************************************************************************************/
/*! \brief          This function initializes the RSA encryption primitive.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      keyPairModuleSize  the byte length of the modulus n
 *  \param[in]      keyPairModule  pointer to the modulus n
 *  \param[in]      publicKeyExponentSize  the byte length of the public exponent e
 *  \param[in]      publicKeyExponent  pointer to the public exponent e
 *  \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL               work space too small
 *                  ESL_ERC_RSA_PUBKEY_INVALID         the public exponent is
 *                  invalid
 *                  ESL_ERC_NO_ERROR                   else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptRSA_prim(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAenc_prim) workSpace,
   eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length publicKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent);

/**********************************************************************************************************************
 *  esl_encryptRSA_prim()
 *********************************************************************************************************************/
/*! \brief          This function performs the RSA encryption operation (RSA_EP1).
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      messageSize  the byte length of the input message
 *  \param[in]      message      pointer to the input message
 *  \param[in,out]  cipherSize   pointer to byte length of the output buffer
 *  \param[in,out]  cipher       pointer to the output buffer
 *  \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 *                  ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE   (int) message > n-1
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT      *cipherSize is too small
 *                  ESL_ERC_WS_STATE_INVALID           invalid Algo
 *                  ESL_ERC_NO_ERROR                   else
 *  \pre            workSpace is initialized by esl_initEncryptRSA_prim(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRSA_prim(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAenc_prim) workSpace,
   eslt_Length messageSize, VSECPRIM_P2CONST_PARA(eslt_Byte) message,
   VSECPRIM_P2VAR_PARA(eslt_Length) cipherSize, VSECPRIM_P2VAR_PARA(eslt_Byte) cipher);
# endif /* (VSECPRIM_RSA_CIPHER_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_SIGNATURE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  esl_initVerifyRSA_prim()
 *********************************************************************************************************************/
/*! \brief          This function initializes the RSA verification primitive.
 *  \details        -
 *  \param[in,out]  workSpace              (in)  algorithm context buffer, initialized by
 *                                               esl_initWorkSpaceHeader(..) function
 *                                         (out) initialized algorithm context structure
 *  \param[in]      keyPairModuleSize      the byte length of the modulus n
 *  \param[in]      keyPairModule          pointer to the modulus n
 *  \param[in]      publicKeyExponentSize  the byte length of the public exponent e
 *  \param[in]      publicKeyExponent      pointer to the public exponent e
 *  \return         ESL_ERC_PARAMETER_INVALID     input parameter is NULL or exponent is larger than module
 *                  ESL_ERC_WS_TOO_SMALL          work space too small
 *                  ESL_ERC_RSA_PUBKEY_INVALID    the public exponent is invalid
 *                  ESL_ERC_NO_ERROR              else
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initVerifyRSA_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver_prim) workSpace,
  eslt_Length keyPairModuleSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
  eslt_Length publicKeyExponentSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent);

/**********************************************************************************************************************
 *  esl_verifyRSA_prim()
 *********************************************************************************************************************/
/*! \brief          This function performs the RSA verification primitive (RSA_VP1).
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure
 *  \param[in]      signatureSize  the byte length of the given signature
 *  \param[in]      signature    pointer to the given signature to be verified
 *  \param[in,out]  messageSize  (in)  pointer to byte length of the output buffer
 *                               (out)
 *  \param[in,out]  message      (in)  pointer to the output buffer
 *                               (out) message, i.e. power of the signature wrt. the public exponent
 *  \return         ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL workSpace too small
 *                  ESL_ERC_WS_STATE_INVALID           invalid Algo
 *                  ESL_ERC_RSA_SIGNATURE_OUT_OF_RANGE (int) signature > n-1
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT      *messageSize is too small
 *                  ESL_ERC_NO_ERROR                   else
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *  \pre            workSpace state set by esl_initVerifyRSA_prim(..)
 *                  the buffer referenced by message must be at least of length messageSize
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyRSA_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver_prim) workSpace,
  eslt_Length signatureSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) signature,
  VSECPRIM_P2VAR_PARA(eslt_Length) messageSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte) message);
# endif /* (VSECPRIM_RSA_SIGNATURE_ENABLED == STD_ON) */

# if ((VSECPRIM_RSA_V15_SHA1_ENABLED == STD_ON) || (VSECPRIM_RSA_V15_ENABLED == STD_ON))  /* COV_VSECPRIM_NO_SAFE_CONFIG XF xf xf */
/*****************************************************************************
 * Crypto functions RSA
 * according to PKCS#1 2.1
 * according to PKCS#1 V1.5
 * these versions include simple padding, sign & verify use SHA-1
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_initDecryptRSA_V15()
 *********************************************************************************************************************/
/*! \brief          Initialize decryption for RSA PKCS#1 V1.5
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      keyPairModuleSize  length of the keyPairModulus (bytes)
 *  \param[in]      keyPairModule  pointer to the key modulus
 *  \param[in]      privateKeyExponentSize  length of the public keyExponent (bytes)
 *  \param[in]      privateKeyExponent  pointer to the public exponent
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  keyExponent  < keyPairModule
 *                  Module is a prime
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptRSA_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAdec) workSpace,
   eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length privateKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent);

/**********************************************************************************************************************
 *  esl_decryptPubRSA_V15()
 *********************************************************************************************************************/
/*! \brief          Perform decryption for RSA PKCS#1 V1.5 with public key
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      cipherSize   length of the cipher (bytes)
 *  \param[in]      cipher       pointer to the cipher
 *  \param[out]     messageSize  pointer to length of the message buffer (bytes)
 *  \param[out]     message      pointer to the message buffer
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_CODE_OUT_OF_RANGE (int) cipher > n-1
 *                  ESL_ERC_RSA_ENCODING_INVALID  Invalid padding
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT outputSize too small
 *  \pre            workSpace is initialized by esl_initDecryptRSA_V15(..)
 *                  public keyExponent  < keyPairModule
 *                  Module is a prime
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptPubRSA_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAdec) workSpace,
   eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
   VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message);

/**********************************************************************************************************************
 *  esl_decryptPrivRSA_V15()
 *********************************************************************************************************************/
/*! \brief          Perform decryption for RSA PKCS#1 V1.5 with private key
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      cipherSize   length of the cipher (bytes)
 *  \param[in]      cipher       pointer to the cipher
 *  \param[out]     messageSize  pointer to length of the message buffer (bytes)
 *  \param[out]     message      pointer to the message buffer
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_CODE_OUT_OF_RANGE (int) cipher > n-1
 *                  ESL_ERC_RSA_ENCODING_INVALID  Invalid padding
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT outputSize too small
 *  \pre            workSpace is initialized by esl_initDecryptRSA_V15(..)
 *                  private keyExponent  < keyPairModule
 *                  Module is a prime
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptPrivRSA_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAdec) workSpace,
   eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
   VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message);

/**********************************************************************************************************************
 *  esl_decryptRSA_V15()
 *********************************************************************************************************************/
/*! \brief          Perform decryption for RSA PKCS#1 V1.5 (default with private key)
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      cipherSize   length of the cipher (bytes)
 *  \param[in]      cipher       pointer to the cipher
 *  \param[out]     messageSize  pointer to length of the message buffer (bytes)
 *  \param[out]     message      pointer to the message buffer
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_CODE_OUT_OF_RANGE (int) cipher > n-1
 *                  ESL_ERC_RSA_ENCODING_INVALID  Invalid padding
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT outputSize too small
 *  \pre            workSpace is initialized by esl_initDecryptRSA_V15(..)
 *                  private keyExponent  < keyPairModule
 *                  Module is a prime
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSA_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAdec) workSpace,
   eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
   VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message);

/**********************************************************************************************************************
 *  esl_initDecryptRSACRT_V15()
 *********************************************************************************************************************/
/*! \brief          Initialize decryption for RSA PKCS#1 V1.5 CRT
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      keyPairPrimePSize  length of the keyPairPrimeP (bytes)
 *  \param[in]      keyPairPrimeP  pointer to the 1st key prime
 *  \param[in]      keyPairPrimeQSize  length of the keyPairPrimeQ (bytes)
 *  \param[in]      keyPairPrimeQ  pointer to the 2nd key prime
 *  \param[in]      privateKeyExponentDPSize  length of the privKeyExponenentDP (bytes)
 *  \param[in]      privateKeyExponentDP  pointer to the partial privKey exponent
 *  \param[in]      privateKeyExponentDQSize  length of the privKeyExponenentDQ (bytes)
 *  \param[in]      privateKeyExponentDQ  pointer to the partial privKey exponent
 *  \param[in]      privateKeyInverseQISize  length of the privateKeyInverseQI (bytes)
 *  \param[in]      privateKeyInverseQI  pointer to the inverse
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_PARAMETER_INVALID   format problems, nullpointer, ..
 *                  ESL_ERC_RSA_PRIVKEY_INVALID  The private exponent is invalid
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  PrimeP and Q are a prime
 *                  privateKeyExpDP<PrimeP
 *                  privateKeyExpDQ<PrimeQ
 *                  QI is inverse to Q, could be checked, then move this line to errorcodes
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptRSACRT_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTdec) workSpace,
   eslt_Length keyPairPrimePSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeP,
   eslt_Length keyPairPrimeQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeQ,
   eslt_Length privateKeyExponentDPSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDP,
   eslt_Length privateKeyExponentDQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDQ,
   eslt_Length privateKeyInverseQISize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyInverseQI);

/**********************************************************************************************************************
 *  esl_decryptRSACRT_V15()
 *********************************************************************************************************************/
/*! \brief          Perform decryption for RSA PKCS#1 V1.5 CRT
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      cipherSize   length of the cipher (bytes)
 *  \param[in]      cipher       pointer to the cipher
 *  \param[out]     messageSize  pointer to length of the message buffer (bytes)
 *  \param[out]     message      pointer to the message buffer
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_MODULE_OUT_OF_RANGE - modulus too short
 *                  ESL_ERC_RSA_CODE_OUT_OF_RANGE (int) cipher > n-1
 *                  - 0 <= (int) cipher <= module-1, where module = PrimeP*PrimeQ
 *                  ESL_ERC_RSA_ENCODING_INVALID  Invalid padding
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT outputSize too small
 *  \pre            workSpace is initialized by esl_initDecryptRSACRT_V15(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSACRT_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTdec) workSpace,
   eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
   VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message);

/**********************************************************************************************************************
 *  esl_initSignRSASHA1_V15()
 *********************************************************************************************************************/
/*! \brief          Initialize Signature for RSA PKCS#1 V1.5 with SHA-1
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      keyPairModuleSize  length of the keyPairModulus (bytes)
 *  \param[in]      keyPairModule  pointer to the key modulus
 *  \param[in]      privateKeyExponentSize  length of the privateKeyExponent (bytes)
 *  \param[in]      privateKeyExponent  pointer to the private key exponent
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_PRIVKEY_INVALID  The private exponent is invalid
 *                  ESL_ERC_RSA_MODULE_OUT_OF_RANGE the given module is not supported the given module is not supported
 *                  - if keyPairModuleSize < 46d = headLen+hLen+11, where
 *                  + hLen is the length of the Hashvalue (SHA-1  20d= 0x14)
 *                  + headLen is the AlgoIDentifier (RSA+SHA1  15d=0x0f)
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  privateKeyExponent  < keyPairModule
 *                  Module is a prime
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSASHA1_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig) workSpace,
   eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length privateKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent);

/**********************************************************************************************************************
 *  esl_updateSignRSASHA1_V15()
 *********************************************************************************************************************/
/*! \brief          Update Signature for RSA PKCS#1 V1.5 with SHA-1
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      inputSize    size of the input data (bytes)
 *  \param[in]      input        pointer to the input data
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID - format problems, nullpointer, ...
 *                  ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW   total input length overflow
 *  \pre            workSpace is initialized by esl_initSignRSASHA1_V15(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSignRSASHA1_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig) workSpace,
   eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeSignRSASHA1_V15()
 *********************************************************************************************************************/
/*! \brief          Finalize Signature for RSA PKCS#1 V1.5 with SHA-1
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[out]     signatureSize  length of the signature (bytes)
 *  \param[out]     signature    pointer to the signature
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT outputSize too small
 *  \pre            workSpace is initialized by esl_initSignRSASHA1_V15(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSASHA1_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize, VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/**********************************************************************************************************************
 *  esl_initSignRSACRTSHA1_V15()
 *********************************************************************************************************************/
/*! \brief          Initialize Signature for RSA PKCS#1 V1.5 CRT with SHA-1
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      keyPairPrimePSize  length of the keyPairPrimeP (bytes)
 *  \param[in]      keyPairPrimeP  pointer to the 1st key prime
 *  \param[in]      keyPairPrimeQSize  length of the keyPairPrimeQ (bytes)
 *  \param[in]      keyPairPrimeQ  pointer to the 2nd key prime
 *  \param[in]      privateKeyExponentDPSize  length of the privKeyExponenentDP (bytes)
 *  \param[in]      privateKeyExponentDP  pointer to the partial privKey exponent
 *  \param[in]      privateKeyExponentDQSize  length of the privKeyExponenentDQ (bytes)
 *  \param[in]      privateKeyExponentDQ  pointer to the partial privKey exponent
 *  \param[in]      privateKeyInverseQISize  length of the privateKeyInverseQI (bytes)
 *  \param[in]      privateKeyInverseQI  pointer to the inverse
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_PRIVKEY_INVALID  The private exponent is invalid
 *                  ESL_ERC_RSA_MODULE_OUT_OF_RANGE the given module is not supported
 *                  - keyPairModuleSize < 46d = headLen+hLen+11, where
 *                  + hLen is the length of the Hashvalue (SHA-1  20d= 0x14)
 *                  + headLen is the AlgoIDentifier (RSA+SHA1  15d=0x0f)
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  PrimeP and PrimeQ are a prime
 *                  privateKeyExpDP<PrimeP
 *                  privateKeyExpDQ<PrimeQ
 *                  QI is inverse to Q, could be checked, then move this line to errorcodes
 *                  the Hash-Function is SHA-1
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSACRTSHA1_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTsig) workSpace,
   eslt_Length keyPairPrimePSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeP,
   eslt_Length keyPairPrimeQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeQ,
   eslt_Length privateKeyExponentDPSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDP,
   eslt_Length privateKeyExponentDQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDQ,
   eslt_Length privateKeyInverseQISize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyInverseQI);

/**********************************************************************************************************************
 *  esl_updateSignRSACRTSHA1_V15()
 *********************************************************************************************************************/
/*! \brief          Update Signature for RSA PKCS#1 V1.5 CRT with SHA-1
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      inputSize    size of the input data (bytes)
 *  \param[in]      input        pointer to the input data
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID - format problems, nullpointer, ...
 *                  ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW   total input length overflow
 *  \pre            workSpace is initialized by esl_initSignRSACRTSHA1_V15(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSignRSACRTSHA1_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTsig) workSpace,
   const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeSignRSACRTSHA1_V15()
 *********************************************************************************************************************/
/*! \brief          Finalize Signature for RSA PKCS#1 V1.5 CRT with SHA-1
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[out]     signatureSize  length of the signature (bytes)
 *  \param[out]     signature    pointer to the signature
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT outputSize too small
 *  \pre            workSpace is initialized by esl_initSignRSACRTSHA1_V15(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSACRTSHA1_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTsig) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize, VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/**********************************************************************************************************************
 *  esl_initEncryptRSA_V15()
 *********************************************************************************************************************/
/*! \brief          Initialize encryption for RSA PKCS#1 V1.5
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      keyPairModuleSize  length of the keyPairModulus (bytes)
 *  \param[in]      keyPairModule  pointer to the key modulus
 *  \param[in]      publicKeyExponentSize  length of the public publicKeyExponent (bytes)
 *  \param[in]      publicKeyExponent  pointer to the public exponent
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_PUBKEY_INVALID  The public exponent is invalid
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  privateKeyExponent  < keyPairModule
 *                  Module is a prime
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptRSA_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAenc) workSpace,
   eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length publicKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent);

/**********************************************************************************************************************
 *  esl_encryptRSA_V15()
 *********************************************************************************************************************/
/*! \brief          Perform encryption for RSA PKCS#1 V1.5
 *  \details        Perform encryption for RSA PKCS#1 V1.5, thereby using the RNG which is
 *                  implemented by the call back function esl_getBytesRNG(..).
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      messageSize  pointer to length of the message buffer (bytes)
 *  \param[in]      message      pointer to the message buffer
 *  \param[out]     cipherSize   length of the cipher (bytes)
 *  \param[out]     cipher       pointer to the cipher
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE
 *                  - if messageSize > keyPairModuleSize - ASN1_SIZEOF_MINIMAL_PADDING
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT outputSize too small
 *                  ESL_ERC_RSA_MODULE_OUT_OF_RANGE keyPairModuleSize < ASN1_SIZEOF_MINIMAL_PADDING
 *                  and all possible error codes which can occur in esl_getBytesRNG
 *  \pre            workSpace is initialized by esl_initEncryptRSA_V15(..)
 *                  *cipherSize >= keyPairModuleSize
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRSA_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAenc) workSpace,
   eslt_Length messageSize, VSECPRIM_P2CONST_PARA(eslt_Byte) message,
   VSECPRIM_P2VAR_PARA(eslt_Length) cipherSize, VSECPRIM_P2VAR_PARA(eslt_Byte) cipher);

/**********************************************************************************************************************
 *  esl_initVerifyRSASHA1_V15()
 *********************************************************************************************************************/
/*! \brief          Initialize signature verification for RSA PKCS#1 V1.5 with SHA-1
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      keyPairModuleSize  length of the keyPairModulus (bytes)
 *  \param[in]      keyPairModule  pointer to the key modulus
 *  \param[in]      publicKeyExponentSize  length of the public publicKeyExponent (bytes)
 *  \param[in]      publicKeyExponent  pointer to the public exponent
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  publicKeyExponent  < keyPairModule
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initVerifyRSASHA1_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace,
   eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length publicKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent);

/**********************************************************************************************************************
 *  esl_updateVerifyRSASHA1_V15()
 *********************************************************************************************************************/
/*! \brief          Update signature verification for RSA PKCS#1 V1.5 with SHA-1
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      inputSize    size of the input data (bytes)
 *  \param[in]      input        pointer to the input data
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID - format problems, nullpointer, ...
 *                  ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW   total input length overflow
 *  \pre            workSpace is initialized by esl_initVerifyRSASHA1_V15(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateVerifyRSASHA1_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace,
   eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeHashVerifyRSASHA1_V15
 *********************************************************************************************************************/
/*! \brief          Finalize hash for signature verification for RSA PKCS#1 V1.5 with SHA-1
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in,out]  messageDigest  pointer to computed hash value
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *  \pre            workSpace is initialized by esl_initVerifyRSASHA1_V15(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashVerifyRSASHA1_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

/**********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA1_V15()
 *********************************************************************************************************************/
/*! \brief          Finalize signature for signature verification for RSA PKCS#1 V1.5 with SHA-1
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      messageDigest  pointer to computed hash value
 *  \param[in]      signatureSize  length of the signature (bytes)
 *  \param[in]      signature    pointer to the signature
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_MODULE_OUT_OF_RANGE - modulus too short
 *                  ESL_ERC_RSA_SIGNATURE_OUT_OF_RANGE
 *                  - 0 <= (int) signature <= module-1, where module = PrimeP*PrimeQ
 *                  ESL_ERC_RSA_ENCODING_INVALID  Invalid padding
 *                  ESL_ERC_RSA_PUBKEY_INVALID  The public exponent is invalid
 *                  ESL_ERC_RSA_SIGNATURE_INVALID  Signature verification failed
 *  \pre            workSpace is initialized by esl_initVerifyRSASHA1_V15(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA1_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/**********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA1_V15()
 *********************************************************************************************************************/
/*! \brief          Finalize hash and signature for signature verification for RSA PKCS#1 V1.5 with SHA-1
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      signatureSize  length of the signature (bytes)
 *  \param[in]      signature    pointer to the signature
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_MODULE_OUT_OF_RANGE - modulus too short
 *                  ESL_ERC_RSA_SIGNATURE_OUT_OF_RANGE
 *                  - 0 <= (int) signature <= module-1, where module = PrimeP*PrimeQ
 *                  ESL_ERC_RSA_ENCODING_INVALID  Invalid padding
 *                  ESL_ERC_RSA_PUBKEY_INVALID  The public exponent is invalid
 *                  ESL_ERC_RSA_SIGNATURE_INVALID  Signature verification failed
 *  \pre            workSpace is initialized by esl_initVerifyRSASHA1_V15(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA1_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);
# endif /* ((VSECPRIM_RSA_V15_SHA1_ENABLED == STD_ON) || (VSECPRIM_RSA_V15_ENABLED == STD_ON)) */

# if (VSECPRIM_RSA_V15_RIPEMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * Crypto functions RSA (sign/verify) with RIPEMD-160
 * according to PKCS#1 1.5
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_initSignRSARIPEMD160_V15()
 *********************************************************************************************************************/
/*! \brief          Initialize Signature for RSA PKCS#1 V1.5 with RIPEMD160
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      keyPairModuleSize  length of the keyPairModulus (bytes)
 *  \param[in]      keyPairModule  pointer to the key modulus
 *  \param[in]      privateKeyExponentSize  length of the privateKeyExponent (bytes)
 *  \param[in]      privateKeyExponent  pointer to the private key exponent
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_PRIVKEY_INVALID  The private exponent is invalid
 *                  ESL_ERC_RSA_MODULE_OUT_OF_RANGE the given module is not supported
 *                  - if keyPairModuleSize < 46d = headLen+hLen+11, where
 *                  + hLen is the length of the Hashvalue (RIPEMD160  20d= 0x14)
 *                  + headLen is the AlgoIDentifier (RSA+RIPEMD160  15d=0x0f)
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  privateKeyExponent  < keyPairModule
 *                  Module is a prime

 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSARIPEMD160_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig) workSpace,
   eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length privateKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent);

/**********************************************************************************************************************
 *  esl_updateSignRSARIPEMD160_V15()
 *********************************************************************************************************************/
/*! \brief          Update Signature for RSA PKCS#1 V1.5 with RIPEMD160
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      inputSize    size of the input data (bytes)
 *  \param[in]      input        pointer to the input data
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID - format problems, nullpointer, ...
 *                  ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW  Maximum total input length exceeded
 *  \pre            workSpace is initialized by esl_initSignRSARIPEMD160_V15(..)

 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSignRSARIPEMD160_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig) workSpace,
   eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeSignRSARIPEMD160_V15()
 *********************************************************************************************************************/
/*! \brief          Finalize Signature for RSA PKCS#1 V1.5 with RIPEMD160
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[out]     signatureSize  length of the signature (bytes)
 *  \param[out]     signature    pointer to the signature
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT outputSize too small
 *  \pre            workSpace is initialized by esl_initSignRSARIPEMD160_V15(..)

 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSARIPEMD160_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize, VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/**********************************************************************************************************************
 *  esl_initSignRSACRTRIPEMD160_V15()
 *********************************************************************************************************************/
/*! \brief          Initialize Signature for RSA PKCS#1 V1.5 CRT with RIPEMD160
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      keyPairPrimePSize  length of the keyPairPrimeP (bytes)
 *  \param[in]      keyPairPrimeP  pointer to the 1st key prime
 *  \param[in]      keyPairPrimeQSize  length of the keyPairPrimeQ (bytes)
 *  \param[in]      keyPairPrimeQ  pointer to the 2nd key prime
 *  \param[in]      privateKeyExponentDPSize  length of the privKeyExponenentDP (bytes)
 *  \param[in]      privateKeyExponentDP  pointer to the partial privKey exponent
 *  \param[in]      privateKeyExponentDQSize  length of the privKeyExponenentDQ (bytes)
 *  \param[in]      privateKeyExponentDQ  pointer to the partial privKey exponent
 *  \param[in]      privateKeyInverseQISize  length of the privateKeyInverseQI (bytes)
 *  \param[in]      privateKeyInverseQI  pointer to the inverse
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_PRIVKEY_INVALID  The private exponent is invalid
 *                  ESL_ERC_RSA_MODULE_OUT_OF_RANGE the given module is not supported
 *                  - keyPairModuleSize < 46d = headLen+hLen+11, where
 *                  + hLen is the length of the Hashvalue (RIPEMD160  20d= 0x14)
 *                  + headLen is the AlgoIDentifier (RSA+RIPEMD160  15d=0x0f)
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  PrimeP and PrimeQ are a prime
 *                  privateKeyExpDP<PrimeP
 *                  privateKeyExpDQ<PrimeQ
 *                  QI is inverse to Q, could be checked, then move this line to errorcodes
 *                  the Hash-Function is SHA-1

 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSACRTRIPEMD160_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTsig) workSpace,
   eslt_Length keyPairPrimePSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeP,
   eslt_Length keyPairPrimeQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeQ,
   eslt_Length privateKeyExponentDPSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDP,
   eslt_Length privateKeyExponentDQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDQ,
   eslt_Length privateKeyInverseQISize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyInverseQI);

/**********************************************************************************************************************
 *  esl_updateSignRSACRTRIPEMD160_V15()
 *********************************************************************************************************************/
/*! \brief          Update Signature for RSA PKCS#1 V1.5 CRT with RIPEMD160
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      inputSize    size of the input data (bytes)
 *  \param[in]      input        pointer to the input data
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID - format problems, nullpointer, ...
 *                  ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW  Maximum total input length exceeded
 *  \pre            workSpace is initialized by esl_initSignRSACRTRIPEMD160_V15(..)

 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSignRSACRTRIPEMD160_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTsig) workSpace,
   const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeSignRSACRTRIPEMD160_V15()
 *********************************************************************************************************************/
/*! \brief          Finalize Signature for RSA PKCS#1 V1.5 CRT with RIPEMD160
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[out]     signatureSize  length of the signature (bytes)
 *  \param[out]     signature    pointer to the signature
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT outputSize too small
 *  \pre            workSpace is initialized by esl_initSignRSACRTRIPEMD160_V15(..)

 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSACRTRIPEMD160_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTsig) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize, VSECPRIM_P2VAR_PARA(eslt_Byte  ) signature);

/**********************************************************************************************************************
 *  esl_initVerifyRSARIPEMD160_V15()
 *********************************************************************************************************************/
/*! \brief          Initialize signature verification for RSA PKCS#1 V1.5 with RIPEMD160
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      keyPairModuleSize  length of the keyPairModulus (bytes)
 *  \param[in]      keyPairModule  pointer to the key modulus
 *  \param[in]      publicKeyExponentSize  length of the public publicKeyExponent (bytes)
 *  \param[in]      publicKeyExponent  pointer to the public exponent
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_PUBKEY_INVALID  The public exponent is invalid
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  publicKeyExponent  < keyPairModule

 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initVerifyRSARIPEMD160_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace,
   eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length publicKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent);

/**********************************************************************************************************************
 *  esl_updateVerifyRSARIPEMD160_V15()
 *********************************************************************************************************************/
/*! \brief          Update signature verification for RSA PKCS#1 V1.5 with RIPEMD160
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      inputSize    size of the input data (bytes)
 *  \param[in]      input        pointer to the input data
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID - format problems, nullpointer, ...
 *                  ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW  Maximum total input length exceeded
 *  \pre            workSpace is initialized by esl_initVerifyRSARIPEMD160_V15(..)

 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateVerifyRSARIPEMD160_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace,
   eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeHashVerifyRSARIPEMD160_V15
 *********************************************************************************************************************/
/*! \brief          Finalize hash for signature verification for RSA PKCS#1 V1.5 with RIPEMD160
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in,out]  messageDigest  pointer to computed hash value
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *  \pre            workSpace is initialized by esl_initVerifyRSARIPEMD160_V15(..)

 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashVerifyRSARIPEMD160_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

/**********************************************************************************************************************
 *  esl_verifySigVerifyRSARIPEMD160_V15()
 *********************************************************************************************************************/
/*! \brief          Finalize signature for signature verification for RSA PKCS#1 V1.5 with RIPEMD160
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      messageDigest  pointer to computed hash value
 *  \param[in]      signatureSize  length of the signature (bytes)
 *  \param[in]      signature    pointer to the signature
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_MODULE_OUT_OF_RANGE - modulus too short
 *                  ESL_ERC_RSA_SIGNATURE_OUT_OF_RANGE
 *                  - 0 <= (int) signature <= module-1, where module = PrimeP*PrimeQ
 *                  ESL_ERC_RSA_ENCODING_INVALID  Invalid padding
 *                  ESL_ERC_RSA_PUBKEY_INVALID  The public exponent is invalid
 *                  ESL_ERC_RSA_SIGNATURE_INVALID  Signature verification failed
 *  \pre            workSpace is initialized by esl_initVerifyRSARIPEMD160_V15(..)

 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSARIPEMD160_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/**********************************************************************************************************************
 *  esl_finalizeVerifyRSARIPEMD160_V15()
 *********************************************************************************************************************/
/*! \brief          Finalize hash and signature for signature verification for RSA PKCS#1 V1.5 with RIPEMD160
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      signatureSize  length of the signature (bytes)
 *  \param[in]      signature    pointer to the signature
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_MODULE_OUT_OF_RANGE - modulus too short
 *                  ESL_ERC_RSA_SIGNATURE_OUT_OF_RANGE
 *                  - 0 <= (int) signature <= module-1, where module = PrimeP*PrimeQ
 *                  ESL_ERC_RSA_ENCODING_INVALID  Invalid padding
 *                  ESL_ERC_RSA_PUBKEY_INVALID  The public exponent is invalid
 *                  ESL_ERC_RSA_SIGNATURE_INVALID  Signature verification failed
 *  \pre            workSpace is initialized by esl_initVerifyRSARIPEMD160_V15(..)

 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSARIPEMD160_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);
# endif /* (VSECPRIM_RSA_V15_RIPEMD160_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_V15_SHA2_256_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * Crypto functions RSA (sign/verify) with SHA-256
 * according to PKCS#1 1.5
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_initSignRSASHA256_V15()
 *********************************************************************************************************************/
/*! \brief          Initialize Signature for RSA PKCS#1 V1.5 with SHA-256
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      keyPairModuleSize  length of the keyPairModulus (bytes)
 *  \param[in]      keyPairModule  pointer to the key modulus
 *  \param[in]      privateKeyExponentSize  length of the privateKeyExponent (bytes)
 *  \param[in]      privateKeyExponent  pointer to the private key exponent
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_PRIVKEY_INVALID  The private exponent is invalid
 *                  ESL_ERC_RSA_MODULE_OUT_OF_RANGE the given module is not supported
 *                  - if keyPairModuleSize < 46d = headLen+hLen+11, where
 *                  + hLen is the length of the Hashvalue (SHA-256  32d= 0x20)
 *                  + headLen is the AlgoIDentifier (RSA+SHA256  19d=0x13)
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  privateKeyExponent  < keyPairModule
 *                  Module is a prime
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSASHA256_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig) workSpace,
   eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length privateKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent   );

/**********************************************************************************************************************
 *  esl_updateSignRSASHA256_V15()
 *********************************************************************************************************************/
/*! \brief          Update Signature for RSA PKCS#1 V1.5 with SHA-256
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      inputSize    size of the input data (bytes)
 *  \param[in]      input        pointer to the input data
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID - format problems, nullpointer, ...
 *                  ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW  Maximum total input length exceeded
 *  \pre            workSpace is initialized by esl_initSignRSASHA256_V15(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSignRSASHA256_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig) workSpace,
   eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeSignRSASHA256_V15()
 *********************************************************************************************************************/
/*! \brief          Finalize Signature for RSA PKCS#1 V1.5 with SHA-256
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[out]     signatureSize  length of the signature (bytes)
 *  \param[out]     signature    pointer to the signature
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT outputSize too small
 *  \pre            workSpace is initialized by esl_initSignRSASHA256_V15(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSASHA256_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize, VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/**********************************************************************************************************************
 *  esl_initSignRSACRTSHA256_V15()
 *********************************************************************************************************************/
/*! \brief          Initialize Signature for RSA PKCS#1 V1.5 CRT with SHA-256
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      keyPairPrimePSize  length of the keyPairPrimeP (bytes)
 *  \param[in]      keyPairPrimeP  pointer to the 1st key prime
 *  \param[in]      keyPairPrimeQSize  length of the keyPairPrimeQ (bytes)
 *  \param[in]      keyPairPrimeQ  pointer to the 2nd key prime
 *  \param[in]      privateKeyExponentDPSize  length of the privKeyExponenentDP (bytes)
 *  \param[in]      privateKeyExponentDP  pointer to the partial privKey exponent
 *  \param[in]      privateKeyExponentDQSize  length of the privKeyExponenentDQ (bytes)
 *  \param[in]      privateKeyExponentDQ  pointer to the partial privKey exponent
 *  \param[in]      privateKeyInverseQISize  length of the privateKeyInverseQI (bytes)
 *  \param[in]      privateKeyInverseQI  pointer to the inverse
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_PRIVKEY_INVALID  The private exponent is invalid
 *                  ESL_ERC_RSA_MODULE_OUT_OF_RANGE the given module is not supported
 *                  - keyPairModuleSize < 46d = headLen+hLen+11, where
 *                  + hLen is the length of the Hashvalue (SHA-256  32d= 0x20)
 *                  + headLen is the AlgoIDentifier (RSA+SHA256  19d=0x13)
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  PrimeP and PrimeQ are a prime
 *                  privateKeyExpDP<PrimeP
 *                  privateKeyExpDQ<PrimeQ
 *                  QI is inverse to Q, could be checked, then move this line to errorcodes
 *                  the Hash-Function is SHA-1
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSACRTSHA256_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTsig) workSpace,
   eslt_Length keyPairPrimePSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeP,
   eslt_Length keyPairPrimeQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeQ,
   eslt_Length privateKeyExponentDPSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDP,
   eslt_Length privateKeyExponentDQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDQ,
   eslt_Length privateKeyInverseQISize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyInverseQI);

/**********************************************************************************************************************
 *  esl_updateSignRSACRTSHA256_V15()
 *********************************************************************************************************************/
/*! \brief          Update Signature for RSA PKCS#1 V1.5 CRT with SHA-256
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      inputSize    size of the input data (bytes)
 *  \param[in]      input        pointer to the input data
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID - format problems, nullpointer, ...
 *                  ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW  Maximum total input length exceeded
 *  \pre            workSpace is initialized by esl_initSignRSACRTSHA256_V15(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSignRSACRTSHA256_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTsig) workSpace,
   const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeSignRSACRTSHA256_V15()
 *********************************************************************************************************************/
/*! \brief          Finalize Signature for RSA PKCS#1 V1.5 CRT with SHA-256
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[out]     signatureSize  length of the signature (bytes)
 *  \param[out]     signature    pointer to the signature
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_OUTPUT_SIZE_TOO_SHORT outputSize too small
 *  \pre            workSpace is initialized by esl_initSignRSACRTSHA256_V15(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSACRTSHA256_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTsig) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize, VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/**********************************************************************************************************************
 *  esl_initVerifyRSASHA256_V15()
 *********************************************************************************************************************/
/*! \brief          Initialize signature verification for RSA PKCS#1 V1.5 with SHA-256
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      keyPairModuleSize  length of the keyPairModulus (bytes)
 *  \param[in]      keyPairModule  pointer to the key modulus
 *  \param[in]      publicKeyExponentSize  length of the public publicKeyExponent (bytes)
 *  \param[in]      publicKeyExponent  pointer to the public exponent
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_PUBKEY_INVALID  The public exponent is invalid
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  publicKeyExponent  < keyPairModule
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initVerifyRSASHA256_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace,
   eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length publicKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent);

/**********************************************************************************************************************
 *  esl_updateVerifyRSASHA256_V15()
 *********************************************************************************************************************/
/*! \brief          Update signature verification for RSA PKCS#1 V1.5 with SHA-256
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      inputSize    size of the input data (bytes)
 *  \param[in]      input        pointer to the input data
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID - format problems, nullpointer, ...
 *                  ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW  Maximum total input length exceeded
 *  \pre            workSpace is initialized by esl_initVerifyRSASHA256_V15(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateVerifyRSASHA256_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace,
   eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeHashVerifyRSASHA256_V15
 *********************************************************************************************************************/
/*! \brief          Finalize hash for signature verification for RSA PKCS#1 V1.5 with SHA-256
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in,out]  messageDigest  pointer to computed hash value
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *  \pre            workSpace is initialized by esl_initVerifyRSASHA256_V15(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashVerifyRSASHA256_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

/**********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA256_V15()
 *********************************************************************************************************************/
/*! \brief          Finalize signature for signature verification for RSA PKCS#1 V1.5 with SHA-256
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      messageDigest  pointer to computed hash value
 *  \param[in]      signatureSize  length of the signature (bytes)
 *  \param[in]      signature    pointer to the signature
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_MODULE_OUT_OF_RANGE - modulus too short
 *                  ESL_ERC_RSA_SIGNATURE_OUT_OF_RANGE
 *                  - 0 <= (int) signature <= module-1, where module = PrimeP*PrimeQ
 *                  ESL_ERC_RSA_ENCODING_INVALID  Invalid padding
 *                  ESL_ERC_RSA_PUBKEY_INVALID  The public exponent is invalid
 *                  ESL_ERC_RSA_SIGNATURE_INVALID  Signature verification failed
 *  \pre            workSpace is initialized by esl_initVerifyRSASHA256_V15(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA256_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/**********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA256_V15()
 *********************************************************************************************************************/
/*! \brief          Finalize hash and signature for signature verification for RSA PKCS#1 V1.5 with SHA-256
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      signatureSize  length of the signature (bytes)
 *  \param[in]      signature    pointer to the signature
 *  \return         ESL_ERC_NO_ERROR Operation successful
 *                  ESL_ERC_WS_TOO_SMALL Work space too small
 *                  ESL_ERC_WS_STATE_INVALID Invalid state
 *                  ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 *                  ESL_ERC_RSA_MODULE_OUT_OF_RANGE - modulus too short
 *                  ESL_ERC_RSA_SIGNATURE_OUT_OF_RANGE
 *                  - 0 <= (int) signature <= module-1, where module = PrimeP*PrimeQ
 *                  ESL_ERC_RSA_ENCODING_INVALID  Invalid padding
 *                  ESL_ERC_RSA_PUBKEY_INVALID  The public exponent is invalid
 *                  ESL_ERC_RSA_SIGNATURE_INVALID  Signature verification failed
 *  \pre            workSpace is initialized by esl_initVerifyRSASHA256_V15(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA256_V15(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);
# endif /* (VSECPRIM_RSA_V15_SHA2_256_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_OAEP_SHA1_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * Crypto functions RSA (encrypt/decrypt) using SHA-1
 * according to PKCS#1 OAEP
 *****************************************************************************/
/***********************************************************************************************************************
 *  esl_initEncryptRSASHA1_OAEP
 **********************************************************************************************************************/
/*! \brief         Initialize encryption
 *  \details       -
 *  \param[in,out] workSpace  Encryption work space
 *  \param[in]     keyPairModuleSize  Size of RSA key pair module
 *  \param[in]     keyPairModule  RSA key pair module
 *  \param[in]     publicKeyExponentSize  Size of public RSA key pair exponent
 *  \param[in]     publicKeyExponent  Public RSA key pair exponent
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_PUBKEY_INVALID  The public exponent is invalid
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptRSASHA1_OAEP(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPenc) workSpace,
   eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length publicKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent);

/***********************************************************************************************************************
 *  esl_encryptRSASHA1_OAEP_Label_Seed
 **********************************************************************************************************************/
/*! \brief         Encrypt message
 *  \details       If no inSeed is provided, this function uses the RNG which is implemented by the call back function
 *                 esl_getBytesRNG(..) to generate random seed values.
 *  \param[in,out] workSpace  Encryption work space
 *  \param[in]     messageSize  Size of data to be encrypted
 *  \param[in]     message  Data to be encrypted
 *  \param[in]     labelSize  Size of optional label
 *  \param[in]     label  Optional label to be associated with the message
 *  \param[in]     inSeedSize  Size of seed (must match size if hash digest)
 *  \param[in]     inSeed  Seed value (pass null pointer to trigger internal generation of random seed)
 *  \param[in,out] cipherSize  Input  Maximum size of encrypted data
 *                             Output  Actual size of encrypted data
 *  \param[out]    cipher  Encrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *                 ESL_ERC_RSA_MODULE_OUT_OF_RANGE modulus too short
 *                 and all possible error codes which can occur in esl_getBytesRNG
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRSASHA1_OAEP_Label_Seed(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPenc) workSpace,
   eslt_Length messageSize, VSECPRIM_P2CONST_PARA(eslt_Byte) message,
   eslt_Length labelSize, VSECPRIM_P2CONST_PARA(eslt_Byte) label,
   eslt_Length inSeedSize, VSECPRIM_P2CONST_PARA(eslt_Byte) inSeed,
   VSECPRIM_P2VAR_PARA(eslt_Length) cipherSize, VSECPRIM_P2VAR_PARA(eslt_Byte) cipher);

/***********************************************************************************************************************
 *  esl_encryptRSASHA1_OAEP
 **********************************************************************************************************************/
/*! \brief         Encrypt message
 *  \details       Random seed generated internally, empty string used as default label
 *  \param[in,out] workSpace  Encryption work space
 *  \param[in]     messageSize  Size of data to be encrypted
 *  \param[in]     message  Data to be encrypted
 *  \param[in,out] cipherSize  Input  Maximum size of encrypted data
 *                             Output  Actual size of encrypted data
 *  \param[out]    cipher  Encrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *                 ESL_ERC_RSA_MODULE_OUT_OF_RANGE modulus too short
 *                 and all possible error codes which can occur in esl_getBytesRNG
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRSASHA1_OAEP(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPenc) workSpace,
   eslt_Length messageSize, VSECPRIM_P2CONST_PARA(eslt_Byte) message,
   VSECPRIM_P2VAR_PARA(eslt_Length) cipherSize, VSECPRIM_P2VAR_PARA(eslt_Byte) cipher);

/***********************************************************************************************************************
 *  esl_encryptRSASHA1_OAEP_Seed
 **********************************************************************************************************************/
/*! \brief         Encrypt message
 *  \details       Empty string used as default label
 *  \param[in,out] workSpace  Encryption work space
 *  \param[in]     messageSize  Size of data to be encrypted
 *  \param[in]     message  Data to be encrypted
 *  \param[in]     inSeedSize  Size of seed (must match size of hash digest)
 *  \param[in]     inSeed  Seed value (pass null pointer to trigger internal generation of random seed)
 *  \param[in,out] cipherSize  Input  Maximum size of encrypted data
 *                             Output  Actual size of encrypted data
 *  \param[out]    cipher  Encrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *                 ESL_ERC_RSA_MODULE_OUT_OF_RANGE modulus too short
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRSASHA1_OAEP_Seed(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPenc) workSpace,
   eslt_Length messageSize, VSECPRIM_P2CONST_PARA(eslt_Byte) message,
   eslt_Length inSeedSize, VSECPRIM_P2CONST_PARA(eslt_Byte) inSeed,
   VSECPRIM_P2VAR_PARA(eslt_Length) cipherSize, VSECPRIM_P2VAR_PARA(eslt_Byte) cipher);

/***********************************************************************************************************************
 *  esl_encryptRSASHA1_OAEP_Label
 **********************************************************************************************************************/
/*! \brief         Encrypt message
 *  \details       Random seed generated internally
 *  \param[in,out] workSpace  Encryption work space
 *  \param[in]     messageSize  Size of data to be encrypted
 *  \param[in]     message  Data to be encrypted
 *  \param[in]     labelSize  Size of optional label
 *  \param[in]     label  Optional label to be associated with the message
 *  \param[in,out] cipherSize  Input  Maximum size of encrypted data
 *                             Output  Actual size of encrypted data
 *  \param[out]    cipher  Encrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *                 ESL_ERC_RSA_MODULE_OUT_OF_RANGE modulus too short
 *                 and all possible error codes which can occur in esl_getBytesRNG
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRSASHA1_OAEP_Label(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPenc) workSpace,
   eslt_Length messageSize, VSECPRIM_P2CONST_PARA(eslt_Byte) message,
   eslt_Length labelSize, VSECPRIM_P2CONST_PARA(eslt_Byte) label,
   VSECPRIM_P2VAR_PARA(eslt_Length) cipherSize, VSECPRIM_P2VAR_PARA(eslt_Byte) cipher);

/***********************************************************************************************************************
 *  esl_initDecryptRSASHA1_OAEP
 **********************************************************************************************************************/
/*! \brief         Initialize decryption
 *  \details       -
 *  \param[in,out] workSpace  Decryption work space
 *  \param[in]     keyPairModuleSize  Size of RSA key pair module
 *  \param[in]     keyPairModule  RSA key pair module
 *  \param[in]     privateKeyExponentSize  Size of private RSA key pair exponent
 *  \param[in]     privateKeyExponent  Private RSA key pair exponent
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_PRIVKEY_INVALID  The private exponent is invalid
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptRSASHA1_OAEP(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPdec) workSpace,
   eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length privateKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent);

/***********************************************************************************************************************
 *  esl_decodeRSASHA1_OAEP
 **********************************************************************************************************************/
/*! \brief         Decode an OAEP encoded message
 *  \details       -
 *  \param[in,out] workSpace  Decryption work space
 *  \param[in]     encodedMessageSize  Size of encoded message located in work space
 *  \param[in]     labelSize  Size of optional label
 *  \param[in]     label  Optional label associated with the message
 *  \param[in,out] messageSize  Input  Maximum size of decrypted data
 *                              Output  Actual size of decrypted data
 *  \param[out]    message  Decrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decodeRSASHA1_OAEP(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpace) workSpace, eslt_Length encodedMessageSize,
   eslt_Length labelSize, VSECPRIM_P2CONST_PARA(eslt_Byte) label,
   VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message);

/***********************************************************************************************************************
 *  esl_decryptRSASHA1_OAEP_Label
 **********************************************************************************************************************/
/*! \brief         Decrypt message
 *  \details       -
 *  \param[in,out] workSpace  Decryption work space
 *  \param[in]     cipherSize  Size of encrypted data
 *  \param[in]     cipher  Encrypted data
 *  \param[in]     labelSize  Size of optional label
 *  \param[in]     label  Optional label associated with the message
 *  \param[in,out] messageSize  Input  Maximum size of decrypted data
 *                              Output  Actual size of decrypted data
 *  \param[out]    message  Decrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSASHA1_OAEP_Label(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPdec) workSpace,
   eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
   eslt_Length labelSize, VSECPRIM_P2CONST_PARA(eslt_Byte) label,
   VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message);

/***********************************************************************************************************************
 *  esl_decryptRSASHA1_OAEP
 **********************************************************************************************************************/
/*! \brief         Decrypt message
 *  \details       Empty string used as default label
 *  \param[in,out] workSpace  Decryption work space
 *  \param[in]     cipherSize  Size of encrypted data
 *  \param[in]     cipher  Encrypted data
 *  \param[in,out] messageSize  Input  Maximum size of decrypted data
 *                              Output  Actual size of decrypted data
 *  \param[out]    message  Decrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSASHA1_OAEP(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPdec) workSpace,
   eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
   VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message);

/***********************************************************************************************************************
 *  esl_initDecryptRSACRTSHA1_OAEP
 **********************************************************************************************************************/
/*! \brief         Initialize decryption (CRT variant)
 *  \details       -
 *  \param[in,out] workSpace  Decryption work space
 *  \param[in]     keyPairPrimePSize  Size of secret prime p
 *  \param[in]     keyPairPrimeP  Secret prime p
 *  \param[in]     keyPairPrimeQSize  Size of secret prime q
 *  \param[in]     keyPairPrimeQ  Secret prime q
 *  \param[in]     privateKeyExponentDPSize  Size of private key component dp
 *  \param[in]     privateKeyExponentDP  Private key component dp
 *  \param[in]     privateKeyExponentDQSize  Size of private key component dq
 *  \param[in]     privateKeyExponentDQ  Private key component dq
 *  \param[in]     privateKeyInverseQISize  Size of private key component q^-1 mod p
 *  \param[in]     privateKeyInverseQI  Private key component q^-1 mod p
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_PRIVKEY_INVALID  The private exponent is invalid
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptRSACRTSHA1_OAEP(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTOAEPdec) workSpace,
   eslt_Length keyPairPrimePSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeP,
   eslt_Length keyPairPrimeQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeQ,
   eslt_Length privateKeyExponentDPSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDP,
   eslt_Length privateKeyExponentDQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDQ,
   eslt_Length privateKeyInverseQISize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyInverseQI);

/***********************************************************************************************************************
 *  esl_decryptRSACRTSHA1_OAEP_Label
 **********************************************************************************************************************/
/*! \brief         Decrypt message (CRT variant)
 *  \details       -
 *  \param[in,out] workSpace  Decryption work space
 *  \param[in]     cipherSize  Size of encrypted data
 *  \param[in]     cipher  Encrypted data
 *  \param[in]     labelSize  Size of optional label
 *  \param[in]     label  Optional label associated with the message
 *  \param[in,out] messageSize  Input  Maximum size of decrypted data
 *                              Output  Actual size of decrypted data
 *  \param[out]    message  Decrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSACRTSHA1_OAEP_Label(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTOAEPdec) workSpace,
   eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
   eslt_Length labelSize, VSECPRIM_P2CONST_PARA(eslt_Byte) label,
   VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message);

/***********************************************************************************************************************
 *  esl_decryptRSACRTSHA1_OAEP
 **********************************************************************************************************************/
/*! \brief         Decrypt message (CRT variant)
 *  \details       Empty string used as default label
 *  \param[in,out] workSpace  Decryption work space
 *  \param[in]     cipherSize  Size of encrypted data
 *  \param[in]     cipher  Encrypted data
 *  \param[in,out] messageSize  Input  Maximum size of decrypted data
 *                              Output  Actual size of decrypted data
 *  \param[out]    message  Decrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSACRTSHA1_OAEP(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTOAEPdec) workSpace,
   eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
   VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message);
# endif /* (VSECPRIM_RSA_OAEP_SHA1_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_OAEP_SHA2_256_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * Crypto functions RSA (encrypt/decrypt) using SHA-256
 * according to PKCS#1 OAEP
 *****************************************************************************/

/***********************************************************************************************************************
 *  esl_initEncryptRSASHA256_OAEP
 **********************************************************************************************************************/
/*! \brief         Initialize encryption
 *  \details       -
 *  \param[in,out] workSpace  Encryption work space
 *  \param[in]     keyPairModuleSize  Size of RSA key pair module
 *  \param[in]     keyPairModule  RSA key pair module
 *  \param[in]     publicKeyExponentSize  Size of public RSA key pair exponent
 *  \param[in]     publicKeyExponent  Public RSA key pair exponent
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_PUBKEY_INVALID  The public exponent is invalid
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptRSASHA256_OAEP(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPenc) workSpace,
   eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length publicKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent);

/***********************************************************************************************************************
 *  esl_encryptRSASHA256_OAEP_Label_Seed
 **********************************************************************************************************************/
/*! \brief         Encrypt message
 *  \details       If no inSeed is provided, this function uses the RNG which is implemented by the call back function
 *                 esl_getBytesRNG(..) to generate random seed values.
 *  \param[in,out] workSpace  Encryption work space
 *  \param[in]     messageSize  Size of data to be encrypted
 *  \param[in]     message  Data to be encrypted
 *  \param[in]     labelSize  Size of optional label
 *  \param[in]     label  Optional label to be associated with the message
 *  \param[in]     inSeedSize  Size of seed (must match size if hash digest)
 *  \param[in]     inSeed  Seed value (pass null pointer to trigger internal generation of random seed)
 *  \param[in,out] cipherSize  Input  Maximum size of encrypted data
 *                             Output  Actual size of encrypted data
 *  \param[out]    cipher  Encrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *                 ESL_ERC_RSA_MODULE_OUT_OF_RANGE modulus too short
 *                 and all possible error codes which can occur in esl_getBytesRNG
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRSASHA256_OAEP_Label_Seed(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPenc) workSpace,
   eslt_Length messageSize, VSECPRIM_P2CONST_PARA(eslt_Byte) message,
   eslt_Length labelSize, VSECPRIM_P2CONST_PARA(eslt_Byte) label,
   eslt_Length inSeedSize, VSECPRIM_P2CONST_PARA(eslt_Byte) inSeed,
   VSECPRIM_P2VAR_PARA(eslt_Length) cipherSize, VSECPRIM_P2VAR_PARA(eslt_Byte) cipher);

/***********************************************************************************************************************
 *  esl_encryptRSASHA256_OAEP
 **********************************************************************************************************************/
/*! \brief         Encrypt message
 *  \details       Random seed generated internally, empty string used as default label
 *  \param[in,out] workSpace  Encryption work space
 *  \param[in]     messageSize  Size of data to be encrypted
 *  \param[in]     message  Data to be encrypted
 *  \param[in,out] cipherSize  Input  Maximum size of encrypted data
 *                             Output  Actual size of encrypted data
 *  \param[out]    cipher  Encrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *                 ESL_ERC_RSA_MODULE_OUT_OF_RANGE modulus too short
 *                 and all possible error codes which can occur in esl_getBytesRNG
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRSASHA256_OAEP(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPenc) workSpace,
   eslt_Length messageSize, VSECPRIM_P2CONST_PARA(eslt_Byte) message,
   VSECPRIM_P2VAR_PARA(eslt_Length) cipherSize, VSECPRIM_P2VAR_PARA(eslt_Byte) cipher);

/***********************************************************************************************************************
 *  esl_encryptRSASHA256_OAEP_Seed
 **********************************************************************************************************************/
/*! \brief         Encrypt message
 *  \details       Empty string used as default label
 *  \param[in,out] workSpace  Encryption work space
 *  \param[in]     messageSize  Size of data to be encrypted
 *  \param[in]     message  Data to be encrypted
 *  \param[in]     inSeedSize  Size of seed (must match size if hash digest)
 *  \param[in]     inSeed  Seed value (pass null pointer to trigger internal generation of random seed)
 *  \param[in,out] cipherSize  Input  Maximum size of encrypted data
 *                             Output  Actual size of encrypted data
 *  \param[out]    cipher  Encrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *                 ESL_ERC_RSA_MODULE_OUT_OF_RANGE modulus too short
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRSASHA256_OAEP_Seed(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPenc) workSpace,
   eslt_Length messageSize, VSECPRIM_P2CONST_PARA(eslt_Byte) message,
   eslt_Length inSeedSize, VSECPRIM_P2CONST_PARA(eslt_Byte) inSeed,
   VSECPRIM_P2VAR_PARA(eslt_Length) cipherSize, VSECPRIM_P2VAR_PARA(eslt_Byte) cipher);

/***********************************************************************************************************************
 *  esl_encryptRSASHA256_OAEP_Label
 **********************************************************************************************************************/
/*! \brief         Encrypt message
 *  \details       Random seed generated internally
 *  \param[in,out] workSpace  Encryption work space
 *  \param[in]     messageSize  Size of data to be encrypted
 *  \param[in]     message  Data to be encrypted
 *  \param[in]     labelSize  Size of optional label
 *  \param[in]     label  Optional label to be associated with the message
 *  \param[in,out] cipherSize  Input  Maximum size of encrypted data
 *                             Output  Actual size of encrypted data
 *  \param[out]    cipher  Encrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *                 ESL_ERC_RSA_MODULE_OUT_OF_RANGE modulus too short
 *                 and all possible error codes which can occur in esl_getBytesRNG
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRSASHA256_OAEP_Label(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPenc) workSpace,
   eslt_Length messageSize, VSECPRIM_P2CONST_PARA(eslt_Byte) message,
   eslt_Length labelSize, VSECPRIM_P2CONST_PARA(eslt_Byte) label,
   VSECPRIM_P2VAR_PARA(eslt_Length) cipherSize, VSECPRIM_P2VAR_PARA(eslt_Byte) cipher);

/***********************************************************************************************************************
 *  esl_initDecryptRSASHA256_OAEP
 **********************************************************************************************************************/
/*! \brief         Initialize decryption
 *  \details       -
 *  \param[in,out] workSpace  Decryption work space
 *  \param[in]     keyPairModuleSize  Size of RSA key pair module
 *  \param[in]     keyPairModule  RSA key pair module
 *  \param[in]     privateKeyExponentSize  Size of private RSA key pair exponent
 *  \param[in]     privateKeyExponent  Private RSA key pair exponent
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_PRIVKEY_INVALID  The private exponent is invalid
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptRSASHA256_OAEP(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPdec) workSpace,
   eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length privateKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent);

/***********************************************************************************************************************
 *  esl_decodeRSASHA256_OAEP
 **********************************************************************************************************************/
/*! \brief         Decode an OAEP encoded message
 *  \details       -
 *  \param[in,out] workSpace  Decryption work space
 *  \param[in]     encodedMessageSize  Size of encoded message located in work space
 *  \param[in]     labelSize  Size of optional label
 *  \param[in]     label  Optional label associated with the message
 *  \param[in,out] messageSize  Input  Maximum size of decrypted data
 *                              Output  Actual size of decrypted data
 *  \param[out]    message  Decrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decodeRSASHA256_OAEP(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpace) workSpace, eslt_Length encodedMessageSize,
   eslt_Length labelSize, VSECPRIM_P2CONST_PARA(eslt_Byte) label,
   VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message);

/***********************************************************************************************************************
 *  esl_decryptRSASHA256_OAEP_Label
 **********************************************************************************************************************/
/*! \brief         Decrypt message
 *  \details       -
 *  \param[in,out] workSpace  Decryption work space
 *  \param[in]     cipherSize  Size of encrypted data
 *  \param[in]     cipher  Encrypted data
 *  \param[in]     labelSize  Size of optional label
 *  \param[in]     label  Optional label associated with the message
 *  \param[in,out] messageSize  Input  Maximum size of decrypted data
 *                              Output  Actual size of decrypted data
 *  \param[out]    message  Decrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSASHA256_OAEP_Label(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPdec) workSpace,
   eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
   eslt_Length labelSize, VSECPRIM_P2CONST_PARA(eslt_Byte) label,
   VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message);

/***********************************************************************************************************************
 *  esl_decryptRSASHA256_OAEP
 **********************************************************************************************************************/
/*! \brief         Decrypt message
 *  \details       Empty string used as default label
 *  \param[in,out] workSpace  Decryption work space
 *  \param[in]     cipherSize  Size of encrypted data
 *  \param[in]     cipher  Encrypted data
 *  \param[in,out] messageSize  Input  Maximum size of decrypted data
 *                              Output  Actual size of decrypted data
 *  \param[out]    message  Decrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSASHA256_OAEP(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPdec) workSpace,
   eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
   VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message);

/***********************************************************************************************************************
 *  esl_initDecryptRSACRTSHA256_OAEP
 **********************************************************************************************************************/
/*! \brief         Initialize decryption (CRT variant)
 *  \details       -
 *  \param[in,out] workSpace  Decryption work space
 *  \param[in]     keyPairPrimePSize  Size of secret prime p
 *  \param[in]     keyPairPrimeP  Secret prime p
 *  \param[in]     keyPairPrimeQSize  Size of secret prime q
 *  \param[in]     keyPairPrimeQ  Secret prime q
 *  \param[in]     privateKeyExponentDPSize  Size of private key component dp
 *  \param[in]     privateKeyExponentDP  Private key component dp
 *  \param[in]     privateKeyExponentDQSize  Size of private key component dq
 *  \param[in]     privateKeyExponentDQ  Private key component dq
 *  \param[in]     privateKeyInverseQISize  Size of private key component q^-1 mod p
 *  \param[in]     privateKeyInverseQI  Private key component q^-1 mod p
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_PRIVKEY_INVALID  The private exponent is invalid
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptRSACRTSHA256_OAEP(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTOAEPdec) workSpace,
   eslt_Length keyPairPrimePSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeP,
   eslt_Length keyPairPrimeQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeQ,
   eslt_Length privateKeyExponentDPSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDP,
   eslt_Length privateKeyExponentDQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDQ,
   eslt_Length privateKeyInverseQISize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyInverseQI);

/***********************************************************************************************************************
 *  esl_decryptRSACRTSHA256_OAEP_Label
 **********************************************************************************************************************/
/*! \brief         Decrypt message (CRT variant)
 *  \details       -
 *  \param[in,out] workSpace  Decryption work space
 *  \param[in]     cipherSize  Size of encrypted data
 *  \param[in]     cipher  Encrypted data
 *  \param[in]     labelSize  Size of optional label
 *  \param[in]     label  Optional label associated with the message
 *  \param[in,out] messageSize  Input  Maximum size of decrypted data
 *                              Output  Actual size of decrypted data
 *  \param[out]    message  Decrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSACRTSHA256_OAEP_Label(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTOAEPdec) workSpace,
   eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
   eslt_Length labelSize, VSECPRIM_P2CONST_PARA(eslt_Byte) label,
   VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message);

/***********************************************************************************************************************
 *  esl_decryptRSACRTSHA256_OAEP
 **********************************************************************************************************************/
/*! \brief         Decrypt message (CRT variant)
 *  \details       Empty string used as default label
 *  \param[in,out] workSpace  Decryption work space
 *  \param[in]     cipherSize  Size of encrypted data
 *  \param[in]     cipher  Encrypted data
 *  \param[in,out] messageSize  Input  Maximum size of decrypted data
 *                              Output  Actual size of decrypted data
 *  \param[out]    message  Decrypted data
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL or out of range
 *                 ESL_ERC_WS_STATE_INVALID  Invalid state
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  Input message out of range
 *  \pre           Work space initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSACRTSHA256_OAEP(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTOAEPdec) workSpace,
   eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
   VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message);
# endif /* (VSECPRIM_RSA_OAEP_SHA2_256_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * Crypto functions RSA (sign/verify) with SHA-1
 * according to PKCS#1 PSS
 *****************************************************************************/

/***********************************************************************************************************************
 *  esl_initSignRSASHA1_PSS
 **********************************************************************************************************************/
/*! \brief         Initialize signature generation with EMSA PSS encoding, based on SHA1
 *  \details       -
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) initialized work space
 *  \param[in]     keyPairModuleSize  Size of RSA key pair module
 *  \param[in]     keyPairModule  RSA key pair module
 *  \param[in]     privateKeyExponentSize  Size of private RSA key pair exponent
 *  \param[in]     privateKeyExponent  Private RSA key pair exponent
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter workSpace, keyPairModule or privateKeyExponent is NULL
 *                 ESL_ERC_WS_TOO_SMALL  workSpace or RSAPrimWorkSpace contained in workSpace too small
 *                 ESL_ERC_RSA_PRIVKEY_INVALID privateKeyExponent is invalid
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSASHA1_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length privateKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent);

/***********************************************************************************************************************
 *  esl_updateSignRSASHA1_PSS
 **********************************************************************************************************************/
/*! \brief         Update hash workspace with message to be signed
 *  \details       During the RSA PSS signature generation scheme, the original message needs to be hashed. For RSA
 *                 PSS SHA1, the hash function is SHA1. This function stores the message into the hash workspace,
 *                 contained in the parameter workSpace.
 *  \param[in,out] workSpace (in) Signature generation work space
 *                           (out) hash workspace contained in signature generation workspace updated with data
 *                                  to be hashed
 *  \param[in]     inputSize  Size of data to be hashed
 *  \param[in]     input  Data to be hashed, usually the original message
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID input parameter workSpace is null
 *                 ESL_ERC_WS_TOO_SMALL workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID workSpace in invalid state
 *                 ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW  Maximum total input length exceeded
 *  \pre           workSpace is initialized by esl_initSignRSASHA1_PSS(..)
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/

extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSignRSASHA1_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/***********************************************************************************************************************
 *  esl_finalizeHashSignRSASHA1_PSS
 **********************************************************************************************************************/
/*! \brief         Calculate the hash value of the data in hash workspace
 *  \details       During the RSA PSS signature generation scheme, the original message needs to be hashed. For RSA
 *                 PSS SHA1, the hash function is SHA1. The message was passed to the hash workSpace before,
 *                 which is contained in the parameter 'workSpace'. This function computes its hash value.
 *  \param[in,out] workSpace (in) Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in,out] messageDigest (in) buffer to store the message digest (hash)
 *                               (out) message digest
 *  \return        ESL_ERC_PARAMETER_INVALID input parameter workSpace or messageDigest is null
 *                 ESL_ERC_WS_TOO_SMALL workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID workSpace in invalid state
 *                 ESL_ERC_NO_ERROR  Operation successful
 *  \pre           workSpace is initialized by esl_initSignRSASHA1_PSS(..), esl_updateSignRSASHA1(..) called before
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA1_DIGEST (= 20) byte
 *                 long, since this buffer is passed to actSHAFinalize.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashSignRSASHA1_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

/***********************************************************************************************************************
 *  esl_finalizeSigSignRSASHA1_PSS
 **********************************************************************************************************************/
/*! \brief         Generates the RSA PSS SHA1 signature by encoding and signing the message.
 *  \details       -
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in]     saltSize  Size of salt to be used
 *  \param[in]     salt  Salt to be used
 *                       0 (NULL) if salt should be generated randomly
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  input parameter null or
 *                                            saltSize too high for salt to be stored in data block
 *                 ESL_ERC_WS_TOO_SMALL               workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID           workSpace state invalid
 *                 all possible error codes which can occur in esl_getBytesRNG
 *                 ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW  total length of data to be hashed (while calculating the
 *                                                       saltedHashValue) is too long
 *                 ESL_ERC_INPUT_INVALID hashID that is passed to esl_calcSaltedHashRSA_PSS does not correspond
 *                                       to a supported hash function
 *                 ESL_ERC_ERROR an error occurred during the call to esl_generateMaskMGF1
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE   message to be signed is greater than the module
 *                 ESL_ERC_OUTPUT_SIZE_TOO_SHORT      *signatureSize is too small
 *                 ESL_ERC_RSA_ENCODING_INVALID       RSA encoded message length too short
 *  \pre           workSpace is initialized by esl_initSignRSASHA1_PSS(..), esl_updateSignRSASHA1(..) and
 *                 esl_finalizeHashSignRSASHA1_PSS(..) called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA1_DIGEST (= 20) byte long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSASHA1_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length saltSize, VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeSigSignRSASHA1_PSS_NoSalt
 **********************************************************************************************************************/
/*! \brief         Generates the RSA PSS SHA1 signature by encoding and signing the message without using salt.
 *  \details       This function calls esl_finalizeSigSignRSASHA1_PSS() with the given input parameters, saltSize = 0
 *                 and a null pointer for salt.
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 all possible error codes which can occur in esl_finalizeSigSignRSASHA1_PSS
 *  \pre           workSpace is initialized by esl_initSignRSASHA1_PSS(..), esl_updateSignRSASHA1(..) and
 *                 esl_finalizeHashSignRSASHA1_PSS(..) called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA1_DIGEST (= 20) byte long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSASHA1_PSS_NoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeSigSignRSASHA1_PSS_DigestLengthSalt
 **********************************************************************************************************************/
/*! \brief         Generates the RSA PSS SHA1 signature by encoding and signing the message with random salt.
 *  \details       This function calls esl_finalizeSigSignRSASHA1_PSS() with the given input parameters,
 *                 saltSize equal to message digest length and a null pointer for salt, which means that during
 *                 encoding the message, a salt buffer of length ESL_SIZEOF_SHA1_DIGEST with random data will be
 *                 generated.
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 all possible error codes which can occur in esl_finalizeSigSignRSASHA1_PSS
 *  \pre           workSpace is initialized by esl_initSignRSASHA1_PSS(..), esl_updateSignRSASHA1(..) and
 *                 esl_finalizeHashSignRSASHA1_PSS(..) called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA1_DIGEST (= 20) byte long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSASHA1_PSS_DigestLengthSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeSignRSASHA1_PSS
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and generate signature for RSA PSS SHA1
 *  \details       This function calls esl_finalizeHashSignRSASHA1_PSS() and esl_finalizeSigSignRSASHA1_PSS()
 *                 with the given input parameters.
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in]     saltSize  Size of salt to be used
 *  \param[in]     salt  Salt to be used
 *                       0 (NULL) if salt should be generated randomly
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 all possible error codes which can occur in esl_finalizeSigSignRSASHA1_PSS
 *  \pre           workSpace is initialized by esl_initSignRSASHA1_PSS(..), esl_updateSignRSASHA1(..) called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSASHA1_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   eslt_Length saltSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeSignRSASHA1_PSS_NoSalt
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and generate signature for RSA PSS SHA1
 *  \details       This function calls esl_finalizeSignRSASHA1_PSS() with the given input parameters, saltSize = 0
 *                 and a null pointer for salt.
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 all possible error codes which can occur in esl_finalizeSignRSASHA1_PSS
 *  \pre           workSpace is initialized by esl_initSignRSASHA1_PSS(..), esl_updateSignRSASHA1(..) called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSASHA1_PSS_NoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeSignRSASHA1_PSS_DigestLengthSalt
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and generate signature for RSA PSS SHA1
 *  \details       This function calls esl_finalizeSignRSASHA1_PSS()with a null pointer for salt and saltSize
 *                 equal to message digest length for signature generation
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 all possible error codes which can occur in esl_finalizeSignRSASHA1_PSS
 *  \pre           workSpace is initialized by esl_initSignRSASHA1_PSS(..), esl_updateSignRSASHA1(..) called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSASHA1_PSS_DigestLengthSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_initVerifyRSASHA1_PSS
 **********************************************************************************************************************/
/*! \brief         Initialize signature verification with EMSA PSS encoding, based on SHA1
 *  \details       -
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) initialized work space
 *  \param[in]     keyPairModuleSize  Size of RSA key pair module
 *  \param[in]     keyPairModule  RSA key pair module
 *  \param[in]     publicKeyExponentSize  Size of public RSA key pair exponent
 *  \param[in]     publicKeyExponent  Public RSA key pair exponent
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter workSpace/keyPairModule/publicKeyExponent is NULL or
 *                 exponent is larger than module
 *                 ESL_ERC_WS_TOO_SMALL  workSpace or RSAPrimWorkSpace, contained in workSpace, too small
 *                 ESL_ERC_RSA_PUBKEY_INVALID   publicExponentKeySize is invalid
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initVerifyRSASHA1_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length publicKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent);

/***********************************************************************************************************************
 *  esl_updateVerifyRSASHA1_PSS
 **********************************************************************************************************************/
/*! \brief         Update hash workspace with message to be signed
 *  \details       During the RSA PSS signature verification scheme, the original message needs to be hashed. For RSA
 *                 PSS SHA1, the hash function is SHA1. This function stores the message into the hash workspace,
 *                 contained in the parameter workSpace.
 *  \param[in,out] workSpace (in) Signature generation work space
 *                           (out) hash workspace contained in signature generation workspace updated with data
 *                                 to be hashed
 *  \param[in]     inputSize  Size of data to be hashed
 *  \param[in]     input  Data to be hashed, usually the original message
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID input parameter workSpace is null
 *                 ESL_ERC_WS_TOO_SMALL workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID workSpace in invalid state
 *                 ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW  Maximum total input length exceeded
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA1_PSS(..)
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateVerifyRSASHA1_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/***********************************************************************************************************************
 *  esl_finalizeHashVerifyRSASHA1_PSS
 **********************************************************************************************************************/
/*! \brief         Calculate the hash value of the data in the hash workspace
 *  \details       During the RSA PSS signature generation scheme, the original message needs to be hashed. For RSA
 *                 PSS SHA1, the hash function is SHA1. The message was passed to the hash workSpace before,
 *                 which is contained in the parameter 'workSpace'. This function computes its hash value.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in,out] messageDigest (in)  buffer to store the message digest (hash)
 *                               (out) message digest
 *  \return        ESL_ERC_PARAMETER_INVALID input parameter workSpace or messageDigest is null
 *                 ESL_ERC_WS_TOO_SMALL workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID workSpace in invalid state
 *                 ESL_ERC_NO_ERROR  Operation successful
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA1_PSS(..), esl_updateVerifyRSASHA1(..) called
 *                 before. The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA1_DIGEST
 *                 (= 20) byte long, since this buffer is passed to actSHAFinalize.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashVerifyRSASHA1_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

/***********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA1_PSS
 **********************************************************************************************************************/
/*! \brief         Verifies the RSA PSS SHA1 signature by applying the RSA verification primitive to the signature
 *                 and decoding the message.
 *  \details       -
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in]     saltSize  Expected size of salt,
 *                           -1 if size of salt used during signature generation should be evaluated automatically
 *  \param[in]     signatureSize  size of signature
 *  \param[in]     signature signature value to be verified
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  input parameter null or
 *                                            saltSize too high for salt to be stored in data block
 *                 ESL_ERC_WS_TOO_SMALL       workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID   workSpace state invalid
 *                 ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW  total length of data to be hashed (while calculation the
 *                                                saltedHashValue) is too long
 *                 ESL_ERC_INPUT_INVALID hashID that is passed to esl_calcSaltedHashRSA_PSS does not correspond
 *                                       to a supported hash function
 *                 ESL_ERC_ERROR an error occurred during the call to esl_generateMaskMGF1
 *                 ESL_ERC_RSA_SIGNATURE_OUT_OF_RANGE   signature to be verified is greater than the module
 *                 ESL_ERC_RSA_SIGNATURE_INVALID signature verification failed
 *                 ESL_ERC_RSA_ENCODING_INVALID       RSA encoded message length too short
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA1_PSS(..), esl_updateVerifyRSASHA1(..) and
 *                 esl_finalizeHashVerifyRSASHA1_PSS(..) called before
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA1_DIGEST (= 20) byte long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA1_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length saltSize,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA1_PSS_AutoSalt
 **********************************************************************************************************************/
/*! \brief         Verifies the RSA PSS SHA1 signature by encoding and signing the message.
 *  \details       This function calls esl_verifySigVerifyRSASHA1_PSS() with the given input parameters and
 *                 saltSize = -1. This requires that the size of the salt used during signature generation was
 *                 evaluated automatically.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in]     signatureSize size of signature
 *  \param[in]     signature signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSASHA1_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA1_PSS(..), esl_updateVerifyRSASHA1(..) and
 *                 esl_finalizeHashVerifyRSASHA1_PSS(..) called before
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA1_DIGEST (= 20) byte long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA1_PSS_AutoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA1_PSS_NoSalt
 **********************************************************************************************************************/
/*! \brief         Verifies the RSA PSS SHA1 signature by encoding and signing the message.
 *  \details       This function calls esl_verifySigVerifyRSASHA1_PSS() with the given input parameters and
 *                 saltSize = 0. This requires that no salt was used during signature generation.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in]     signatureSize size of signature
 *  \param[in]     signature signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSASHA1_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA1_PSS(..), esl_updateVerifyRSASHA1(..) and
 *                 esl_finalizeHashVerifyRSASHA1_PSS(..) called before
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA1_DIGEST (= 20) byte long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA1_PSS_NoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA1_PSS_DigestLengthSalt
 **********************************************************************************************************************/
/*! \brief         Verifies the RSA PSS SHA1 signature by encoding and signing the message.
 *  \details       This function calls esl_verifySigVerifyRSASHA1_PSS() with the given input parameters and
 *                 saltSize = ESL_SIZEOF_SHA1_DIGEST. This requires that salt with size equal to message digest
 *                 length was used during signature generation.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in]     signatureSize size of signature
 *  \param[in]     signature signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSASHA1_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA1_PSS(..), esl_updateVerifyRSASHA1(..) and
 *                 esl_finalizeHashVerifyRSASHA1_PSS(..) called before
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA1_DIGEST (= 20) byte long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA1_PSS_DigestLengthSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA1_PSS
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and verify signature for RSA PSS SHA1
 *  \details       This function calls esl_finalizeHashVerifyRSASHA1_PSS() and esl_verifySigVerifyRSASHA1_PSS()
 *                 with the given input parameters.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     saltSize  Expected size of salt,
 *                           -1 if size of salt used during signature generation should be evaluated automatically
 *  \param[in]     signatureSize  Size of signature
 *  \param[in]     signature  Signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSASHA1_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA1_PSS(..), esl_updateVerifyRSASHA1(..)
 *                 called before
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA1_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length saltSize, eslt_Length signatureSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA1_PSS_AutoSalt
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and verify signature for RSA PSS SHA1
 *  \details       This function calls esl_finalizeHashVerifyRSASHA1_PSS() and esl_verifySigVerifyRSASHA1_PSS()
 *                 with the given input parameters and saltSize = -1, thus it assumes that the size of the salt used
 *                 during signature generation is evaluated automatically.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     signatureSize  Size of signature
 *  \param[in]     signature  Signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSASHA1_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA1_PSS(..), esl_updateVerifyRSASHA1(..)
 *                 called before
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA1_PSS_AutoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA1_PSS_NoSalt
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and verify signature for RSA PSS SHA1
 *  \details       This function calls esl_finalizeHashVerifyRSASHA1_PSS() and esl_verifySigVerifyRSASHA1_PSS()
 *                 with the given input parameters and saltSize = 0, thus it requires that no salt was used during
 *                 signature generation.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     signatureSize  Size of signature
 *  \param[in]     signature  Signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSASHA1_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA1_PSS(..), esl_updateVerifyRSASHA1(..)
 *                 called before
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA1_PSS_NoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA1_PSS_DigestLengthSalt
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and verify signature for RSA PSS SHA1
 *  \details       This function calls esl_finalizeHashVerifyRSASHA1_PSS() and esl_verifySigVerifyRSASHA1_PSS()
 *                 with the given input parameters and saltSize = ESL_SIZEOF_SHA1_DIGEST, thus it  requires that a
 *                 salt buffer of size equal to message digest was used during signature generation.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     signatureSize  Size of signature
 *  \param[in]     signature  Signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSASHA1_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA1_PSS(..), esl_updateVerifyRSASHA1(..)
 *                 called before
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA1_PSS_DigestLengthSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/**********************************************************************************************************************
 *  esl_calcSaltedHashRSASHA1_PSS
 *********************************************************************************************************************/
/*! \brief      Calculate salted hash value (SHA1)
 *  \details    Based on a given hash and a random salt the hash of
 *                M' = (0x) 00 00 00 00 00 00 00 00 || hash(orig. msg) || salt
 *              is calculated, as described in chapter 9.1.1, steps 5-6 and 9.1.2, steps 12-13 of PKCS#1 2.2
 *  \param[in,out] wsHash      (in)  Pointer to hash workspace
 *                             (out) Pointer to hash workspace
 *  \param[in]  saltSize       Length of salt
 *  \param[in]  salt           Input salt
 *  \param[in]  messageDigest  Input hash value of the original message, will be hashed again here
 *  \param[in,out] saltedHash  (in)  Output buffer for calculated hash (SHA1)
 *                             (out) calculated salted hash (SHA1)
 *  \return     ESL_ERC_NO_ERROR  Operation successful
 *              ESL_ERC_PARAMETER_INVALID wsHash or saltedHash null
 *              ESL_ERC_WS_TOO_SMALL wsHash too small
 *              ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW total length of data to be hashed too long
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           buffer referenced by saltedHash has to be at least of length ESL_SIZEOF_SHA1_DIGEST
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA1_DIGEST (= 20) byte long.
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_calcSaltedHashRSASHA1_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA1) wsHash,
   eslt_Length saltSize, VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   VSECPRIM_P2VAR_PARA(eslt_Byte) saltedHash);
# endif /* (VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * Crypto functions RSA (sign/verify) with RIPEMD-160
 * according to PKCS#1 PSS
 *****************************************************************************/

/***********************************************************************************************************************
 *  esl_initSignRSARIPEMD160_PSS
 **********************************************************************************************************************/
/*! \brief         Initialize signature generation with EMSA PSS encoding, based on RIPEMD160
 *  \details       -
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) initialized work space
 *  \param[in]     keyPairModuleSize  Size of RSA key pair module
 *  \param[in]     keyPairModule  RSA key pair module
 *  \param[in]     privateKeyExponentSize  Size of private RSA key pair exponent
 *  \param[in]     privateKeyExponent  Private RSA key pair exponent
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter workSpace, keyPairModule or privateKeyExponent is NULL
 *                 ESL_ERC_WS_TOO_SMALL  workSpace or RSAPrimWorkSpace contained in workSpace too small
 *                 ESL_ERC_RSA_PRIVKEY_INVALID privateKeyExponent is invalid
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSARIPEMD160_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length privateKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent);

/***********************************************************************************************************************
 *  esl_updateSignRSARIPEMD160_PSS
 **********************************************************************************************************************/
/*! \brief         Update hash workspace with message to be signed
 *  \details       During the RSA PSS signature generation scheme, the original message needs to be hashed. For RSA
 *                 PSS RIPEMD160, the hash function is RMD160. This function stores the message into the hash
 *                 workspace, contained in the parameter workSpace.
 *  \param[in,out] workSpace (in) Signature generation work space
 *                           (out) hash workspace contained in signature generation workspace updated with data
 *                                  to be hashed
 *  \param[in]     inputSize  Size of data to be hashed
 *  \param[in]     input  Data to be hashed, usually the original message
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID input parameter workSpace is null
 *                 ESL_ERC_WS_TOO_SMALL workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID workSpace in invalid state
 *                 ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW  Maximum total input length exceeded
 *  \pre           workSpace is initialized by esl_initSignRSASHA256_PSS(..)
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSignRSARIPEMD160_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/***********************************************************************************************************************
 *  esl_finalizeHashSignRSARIPEMD160_PSS
 **********************************************************************************************************************/
/*! \brief         Calculate the hash value of the data in hash workspace
 *  \details       During the RSA PSS signature generation scheme, the original message needs to be hashed. For RSA
 *                 PSS RIPME160, the hash function is RMD160. The message was passed to the hash workSpace before,
 *                 which is contained in the parameter 'workSpace'. This function computes its hash value.
 *  \param[in,out] workSpace (in) Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in,out] messageDigest (in) buffer to store the message digest (hash)
 *                               (out) message digest
 *  \return        ESL_ERC_PARAMETER_INVALID input parameter workSpace or messageDigest is null
 *                 ESL_ERC_WS_TOO_SMALL workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID workSpace in invalid state
 *                 ESL_ERC_NO_ERROR  Operation successful
 *  \pre           workSpace is initialized by esl_initSignRSARIPEMD160_PSS(..), esl_updateSignRSARIPEM160(..) called before
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_RIPEMD160_DIGEST (= 20) byte
 *                 long, since this buffer is passed to actRMD160Finalize.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashSignRSARIPEMD160_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

/***********************************************************************************************************************
 *  esl_finalizeSigSignRSARIPEMD160_PSS
 **********************************************************************************************************************/
/*! \brief         Generates the RSA PSS RIPEMD160 signature by encoding and signing the message.
 *  \details       -
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in]     saltSize  Size of salt to be used
 *  \param[in]     salt  Salt to be used
 *                       0 (NULL) if salt should be generated randomly
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  input parameter null or
 *                                            saltSize too high for salt to be stored in data block
 *                 ESL_ERC_WS_TOO_SMALL               workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID           workSpace state invalid
 *                 all possible error codes which can occur in esl_getBytesRNG
 *                 ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW  total length of data to be hashed (while calculating the
 *                                                       saltedHashValue) is too long
 *                 ESL_ERC_INPUT_INVALID hashID that is passed to esl_calcSaltedHashRSA_PSS does not correspond
 *                                       to a supported hash function
 *                 ESL_ERC_ERROR an error occurred during the call to esl_generateMaskMGF1
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE   message to be signed is greater than the module
 *                 ESL_ERC_OUTPUT_SIZE_TOO_SHORT      *signatureSize is too small
 *                 ESL_ERC_RSA_ENCODING_INVALID       RSA encoded message length too short
 *  \pre           workSpace is initialized by esl_initSignRSARIPEMD160_PSS(..), esl_updateSignRSARIPEMD160(..) and
 *                 esl_finalizeHashSignRSARIPEMD160_PSS(..) called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_RIPEMD160_DIGEST (= 20) byte long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSARIPEMD160_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length saltSize, VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeSigSignRSARIPEMD160_PSS_NoSalt
 **********************************************************************************************************************/
/*! \brief         Generates the RSA PSS RIPEMD160 signature by encoding and signing the message without using salt.
 *  \details       This function calls esl_finalizeSigSignRSARIPEMD160_PSS() with the given input parameters,
 *                 saltSize = 0 and a null pointer for salt.
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 all possible error codes which can occur in esl_finalizeSigSignRSARIPEMD160_PSS
 *  \pre           workSpace is initialized by esl_initSignRSARIPEMD160_PSS(..), esl_updateSignRSARIPEMD160(..) and
 *                 esl_finalizeHashSignRSARIPEMD160_PSS(..) called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_RIPEMD160_DIGEST (= 20) byte long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSARIPEMD160_PSS_NoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeSigSignRSARIPEMD160_PSS_DigestLengthSalt
 **********************************************************************************************************************/
/*! \brief         Generates the RSA PSS RIPEMD160 signature by encoding and signing the message with random salt.
 *  \details       This function calls esl_finalizeSigSignRSARIPEMD160_PSS() with the given input parameters,
 *                 saltSize equal to message digest length and a null pointer for salt, which means that during
 *                 encoding the message, a salt buffer of length ESL_SIZEOF_RIPEMD160_DIGEST with random data will be
 *                 generated.
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 all possible error codes which can occur in esl_finalizeSigSignRSARIPEMD160_PSS
 *  \pre           workSpace is initialized by esl_initSignRSARIPEMD160_PSS(..), esl_updateSignRSARIPEMD160(..) and
 *                 esl_finalizeHashSignRSARIPEMD160_PSS(..) called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_RIPEMD160_DIGEST (= 20) byte long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSARIPEMD160_PSS_DigestLengthSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeSignRSARIPEMD160_PSS
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and generate signature for RSA PSS RIPEMD160
 *  \details       This function calls esl_finalizeHashSignRSARIPEMD160_PSS() and esl_finalizeSigSignRSARIPEMD160_PSS()
 *                 with the given input parameters.
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in]     saltSize  Size of salt to be used
 *  \param[in]     salt  Salt to be used
 *                       0 (NULL) if salt should be generated randomly
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 all possible error codes which can occur in esl_finalizeSigSignRSARIPEMD160_PSS
 *  \pre           workSpace is initialized by esl_initSignRSARIPEMD160_PSS(..), esl_updateSignRSARIPEMD160(..) called
 *                 before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSARIPEMD160_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   eslt_Length saltSize, VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeSignRSARIPEMD160_PSS_NoSalt
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and generate signature for RSA PSS RIPEMD160
 *  \details       This function calls esl_finalizeSignRSARIPEMD160_PSS() with the given input parameters,
 *                 saltSize = 0 and a null pointer for salt.
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 all possible error codes which can occur in esl_finalizeSignRSARIPEMD160_PSS
 *  \pre           workSpace is initialized by esl_initSignRSARIPEMD160_PSS(..), esl_updateSignRSARIPEMD160(..) called
 *                 before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSARIPEMD160_PSS_NoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeSignRSARIPEMD160_PSS_DigestLengthSalt
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and generate signature for RSA PSS RIPEMD160
 *  \details       This function calls esl_finalizeSignRSARIPEMD160_PSS()with a null pointer for salt and saltSize
 *                 equal to message digest length for signature generation
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 all possible error codes which can occur in esl_finalizeSignRSARIPEMD160_PSS
 *  \pre           workSpace is initialized by esl_initSignRSARIPEMD160_PSS(..), esl_updateSignRSARIPEMD160(..) called
 *                 before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE

 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSARIPEMD160_PSS_DigestLengthSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_initVerifyRSARIPEMD160_PSS
 **********************************************************************************************************************/
/*! \brief         Initialize signature verification with EMSA PSS encoding, based on RIPEMD160
 *  \details       -
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) initialized work space
 *  \param[in]     keyPairModuleSize  Size of RSA key pair module
 *  \param[in]     keyPairModule  RSA key pair module
 *  \param[in]     publicKeyExponentSize  Size of public RSA key pair exponent
 *  \param[in]     publicKeyExponent  Public RSA key pair exponent
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter workSpace/keyPairModule/publicKeyExponent is NULL or
 *                 exponent is larger than module
 *                 ESL_ERC_WS_TOO_SMALL  workSpace or RSAPrimWorkSpace, contained in workSpace, too small
 *                 ESL_ERC_RSA_PUBKEY_INVALID   publicExponentKeySize is invalid
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initVerifyRSARIPEMD160_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length publicKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent);

/***********************************************************************************************************************
 *  esl_updateVerifyRSARIPEMD160_PSS
 **********************************************************************************************************************/
/*! \brief         Update hash workspace with message to be signed
 *  \details       During the RSA PSS signature verification scheme, the original message needs to be hashed. For RSA
 *                 PSS RIPEMD160, the hash function is RMD160. This function stores the message into the hash
 *                 workspace, contained in the parameter workSpace.
 *  \param[in,out] workSpace (in) Signature generation work space
 *                           (out) hash workspace contained in signature generation workspace updated with data
 *                                 to be hashed
 *  \param[in]     inputSize  Size of data to be hashed
 *  \param[in]     input  Data to be hashed, usually the original message
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID input parameter workSpace is null
 *                 ESL_ERC_WS_TOO_SMALL workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID workSpace in invalid state
 *                 ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW  Maximum total input length exceeded
 *  \pre           workSpace is initialized by esl_initVerifyRSARIPEMD160_PSS(..)
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateVerifyRSARIPEMD160_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/***********************************************************************************************************************
 *  esl_finalizeHashVerifyRSARIPEMD160_PSS
 **********************************************************************************************************************/
/*! \brief         Calculate the hash value of the data in the hash workspace
 *  \details       During the RSA PSS signature generation scheme, the original message needs to be hashed. For RSA
 *                 PSS RIPEMD160, the hash function is RMD160. The message was passed to the hash workSpace before,
 *                 which is contained in the parameter 'workSpace'. This function computes its hash value.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in,out] messageDigest (in)  buffer to store the message digest (hash)
 *                               (out) message digest
 *  \return        ESL_ERC_PARAMETER_INVALID input parameter workSpace or messageDigest is null
 *                 ESL_ERC_WS_TOO_SMALL workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID workSpace in invalid state
 *                 ESL_ERC_NO_ERROR  Operation successful
 *  \pre           workSpace is initialized by esl_initVerifyRSARIPEMD160_PSS(..), esl_updateVerifyRSARIPEMD160(..) called
 *                 before. The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_RIPEMD160_DIGEST
 *                 (= 20) byte long, since this buffer is passed to actRMD160Finalize.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashVerifyRSARIPEMD160_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

/***********************************************************************************************************************
 *  esl_verifySigVerifyRSARIPEMD160_PSS
 **********************************************************************************************************************/
/*! \brief         Verifies the RSA PSS RIPEMD160 signature by applying the RSA verification primitive to the signature
 *                 and decoding the message.
 *  \details       -
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in]     saltSize  Expected size of salt,
 *                           -1 if size of salt used during signature generation should be evaluated automatically
 *  \param[in]     signatureSize  size of signature
 *  \param[in]     signature signature value to be verified
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  input parameter null or
 *                                            saltSize too high for salt to be stored in data block
 *                 ESL_ERC_WS_TOO_SMALL       workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID   workSpace state invalid
 *                 ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW  total length of data to be hashed (while calculation the
 *                                                saltedHashValue) is too long
 *                 ESL_ERC_INPUT_INVALID hashID that is passed to esl_calcSaltedHashRSA_PSS does not correspond
 *                                       to a supported hash function
 *                 ESL_ERC_ERROR an error occurred during the call to esl_generateMaskMGF1
 *                 ESL_ERC_RSA_SIGNATURE_OUT_OF_RANGE   signature to be verified is greater than the module
 *                 ESL_ERC_RSA_SIGNATURE_INVALID signature verification
 *                 ESL_ERC_RSA_ENCODING_INVALID       RSA encoded message length too short
 *  \pre           workSpace is initialized by esl_initVerifyRSARIPEMD160_PSS(..), esl_updateVerifyRSARIPEMD160(..) and
 *                 esl_finalizeHashVerifyRSARIPEMD160_PSS(..) called before
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_RIPEMD160_DIGEST (= 20) byte long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSARIPEMD160_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length saltSize,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_verifySigVerifyRSARIPEMD160_PSS_AutoSalt
 **********************************************************************************************************************/
/*! \brief         Verifies the RSA PSS RIPEMD160 signature by encoding and signing the message.
 *  \details       This function calls esl_verifySigVerifyRSARIPEMD160_PSS() with the given input parameters and
 *                 saltSize = -1. This requires that the size of the salt used during signature generation was
 *                 evaluated automatically.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in]     signatureSize size of signature
 *  \param[in]     signature signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSARIPEMD160_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSARIPEMD160_PSS(..), esl_updateVerifyRSARIPEMD160(..) and
 *                 esl_finalizeHashVerifyRSARIPEMD160_PSS(..) called before
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_RIPEMD160_DIGEST (= 20) byte long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSARIPEMD160_PSS_AutoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_verifySigVerifyRSARIPEMD160_PSS_NoSalt
 **********************************************************************************************************************/
/*! \brief         Verifies the RSA PSS RIPEMD160 signature by encoding and signing the message.
 *  \details       This function calls esl_verifySigVerifyRSARIPEMD160_PSS() with the given input parameters and
 *                 saltSize = 0. This requires that no salt was used during signature generation.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in]     signatureSize size of signature
 *  \param[in]     signature signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSARIPEMD160_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSARIPEMD160_PSS(..), esl_updateVerifyRSARIPEMD160(..) and
 *                 esl_finalizeHashVerifyRSARIPEMD160_PSS(..) called before
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_RIPEMD160_DIGEST (= 20) byte long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSARIPEMD160_PSS_NoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_verifySigVerifyRSARIPEMD160_PSS_DigestLengthSalt
 **********************************************************************************************************************/
/*! \brief         Verifies the RSA PSS RIPEMD160 signature by encoding and signing the message.
 *  \details       This function calls esl_verifySigVerifyRSARIPEMD160_PSS() with the given input parameters and
 *                 saltSize = ESL_SIZEOF_RIPEMD160_DIGEST. This requires that salt with size equal to message digest
 *                 length was used during signature generation.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in]     signatureSize size of signature
 *  \param[in]     signature signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSARIPEMD160_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSARIPEMD160_PSS(..), esl_updateVerifyRSARIPEMD160(..) and
 *                 esl_finalizeHashVerifyRSARIPEMD160_PSS(..) called before
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_RIPEMD160_DIGEST (= 20) byte long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSARIPEMD160_PSS_DigestLengthSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeVerifyRSARIPEMD160_PSS
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and verify signature for RSA PSS RIPEMD160
 *  \details       This function calls esl_finalizeHashVerifyRSARIPEMD160_PSS() and esl_verifySigVerifyRSARIPEMD160_PSS()
 *                 with the given input parameters.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     saltSize  Expected size of salt,
 *                           -1 if size of salt used during signature generation should be evaluated automatically
 *  \param[in]     signatureSize  Size of signature
 *  \param[in]     signature  Signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSARIPEMD160_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSARIPEMD160_PSS(..), esl_updateVerifyRSARIPEMD160(..)
 *                 called before
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSARIPEMD160_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length saltSize, eslt_Length signatureSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeVerifyRSARIPEMD160_PSS_AutoSalt
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and verify signature for RSA PSS RIPEMD160
 *  \details       This function calls esl_finalizeVerifyRSARIPEMD160_PSS() with the given input parameters
 *                 and saltSize = -1, thus it assumes that the size of the salt used during signature generation
 *                 is evaluated automatically.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     signatureSize  Size of signature
 *  \param[in]     signature  Signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSARIPEMD160_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSARIPEMD160_PSS(..), esl_updateVerifyRSARIPEMD160(..)
 *                 called before
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSARIPEMD160_PSS_AutoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeVerifyRSARIPEMD160_PSS_NoSalt
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and verify signature for RSA PSS RIPEMD160
 *  \details       This function calls esl_finalizeVerifyRSARIPEMD160_PSS()
 *                 with the given input parameters and saltSize = 0, thus it requires that no salt was used during
 *                 signature generation.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     signatureSize  Size of signature
 *  \param[in]     signature  Signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSARIPEMD160_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSARIPEMD160_PSS(..), esl_updateVerifyRSARIPEMD160(..)
 *                 called before
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSARIPEMD160_PSS_NoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeVerifyRSARIPEMD160_PSS_DigestLengthSalt
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and verify signature for RSA PSS RIPEMD160
 *  \details       This function calls esl_finalizeVerifyRSARIPEMD160_PSS() with the given input parameters and
 *                 saltSize = ESL_SIZEOF_RIPEMD160_DIGEST, thus it  requires that a
 *                 salt buffer of size equal to message digest was used during signature generation.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     signatureSize  Size of signature
 *  \param[in]     signature  Signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSARIPEMD160_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSARIPEMD160_PSS(..), esl_updateVerifyRSARIPEMD160(..)
 *                 called before
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSARIPEMD160_PSS_DigestLengthSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_calcSaltedHashRSARIPEMD160_PSS
 **********************************************************************************************************************/
/*! \brief      Calculate salted hash value (RIPEMD160)
 *  \details    Based on a given hash and a random salt, the hash of
 *                M' = (0x) 00 00 00 00 00 00 00 00 || hash(orig. msg) || salt
 *              is calculated, as described in chapter 9.1.1, steps 5-6 and 9.1.2, steps 12-13 of PKCS#1 2.2
 *  \param[in,out] wsHash      (in)  Pointer to hash workspace
 *                             (out) Pointer to hash workspace
 *  \param[in]  saltSize       Length of salt
 *  \param[in]  salt           Input salt
 *  \param[in]  messageDigest  Input hash value of the original message, will be hashed again here
 *  \param[in,out] saltedHash  (in)  Output buffer for calculated hash (RIPEMD160)
 *                             (out) calculated salted hash (RIPEM160)
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID wsHash or saltedHash null
 *                 ESL_ERC_WS_TOO_SMALL wsHash too small
 *                 ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW total length of data to be hashed too long
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           buffer referenced by saltedHash has to be at least of length ESL_SIZEOF_RIPEMD160_DIGEST
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_RIPEMD160_DIGEST (= 20) byte long.
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_calcSaltedHashRSARIPEMD160_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRIPEMD160) wsHash,
   eslt_Length saltSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   VSECPRIM_P2VAR_PARA(eslt_Byte) saltedHash);
# endif /* (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  esl_initSignRSASHA256_PSS
 **********************************************************************************************************************/
/*! \brief         Initialize signature generation with EMSA PSS encoding, based on SHA2-256
 *  \details       -
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) initialized work space
 *  \param[in]     keyPairModuleSize  Size of RSA key pair module
 *  \param[in]     keyPairModule  RSA key pair module
 *  \param[in]     privateKeyExponentSize  Size of private RSA key pair exponent
 *  \param[in]     privateKeyExponent  Private RSA key pair exponent
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter workSpace, keyPairModule or privateKeyExponent is NULL
 *                 ESL_ERC_WS_TOO_SMALL  workSpace or RSAPrimWorkSpace contained in workSpace too small
 *                 ESL_ERC_RSA_PRIVKEY_INVALID privateKeyExponent is invalid
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSASHA256_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
  eslt_Length keyPairModuleSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
  eslt_Length privateKeyExponentSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent);

/***********************************************************************************************************************
 *  esl_updateSignRSASHA256_PSS
 **********************************************************************************************************************/
/*! \brief         Update hash workspace with message to be signed
 *  \details       During the RSA PSS signature generation scheme, the original message needs to be hashed. For RSA
 *                 PSS SHA256, the hash function is SHA2-256. This function stores the message into the hash workspace,
 *                  contained in the parameter workSpace.
 *  \param[in,out] workSpace (in) Signature generation work space
 *                           (out) hash workspace contained in signature generation workspace updated with data
 *                                  to be hashed
 *  \param[in]     inputSize  Size of data to be hashed
 *  \param[in]     input  Data to be hashed, usually the original message
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID input parameter workSpace is null
 *                 ESL_ERC_WS_TOO_SMALL workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID workSpace in invalid state
 *                 ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW  Maximum total input length exceeded
 *  \pre           workSpace is initialized by esl_initSignRSASHA256_PSS(..)
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSignRSASHA256_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   eslt_Length inputSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/***********************************************************************************************************************
 *  esl_finalizeHashSignRSASHA256_PSS
 **********************************************************************************************************************/
/*! \brief         Calculate the hash value of the data in hash workspace
 *  \details       During the RSA PSS signature generation scheme, the original message needs to be hashed. For RSA
 *                 PSS SHA256, the hash function is SHA2-256. The message was passed to the hash workSpace before,
                   which is contained in the parameter 'workSpace'. This function computes its hash value.
 *  \param[in,out] workSpace (in) Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in,out] messageDigest (in) buffer to store the message digest (hash)
 *                               (out) message digest
 *  \return        ESL_ERC_PARAMETER_INVALID input parameter workSpace or messageDigest is null
 *                 ESL_ERC_WS_TOO_SMALL workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID workSpace in invalid state
 *                 ESL_ERC_NO_ERROR  Operation successful
 *  \pre           workSpace is initialized by esl_initSignRSASHA256_PSS(..), esl_updateSignRSASHA256(..) called before
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA256_DIGEST (= 32) byte
 *                 long, since this buffer is passed to actSHA256Finalize.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashSignRSASHA256_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

/***********************************************************************************************************************
 *  esl_finalizeSigSignRSASHA256_PSS
 **********************************************************************************************************************/
/*! \brief         Generates the RSA PSS SHA256 signature by encoding and signing the message.
 *  \details       -
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in]     saltSize  Size of salt to be used
 *  \param[in]     salt  Salt to be used
 *                       0 (NULL) if salt should be generated randomly
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  input parameter null or
 *                                            saltSize too high for salt to be stored in data block
 *                 ESL_ERC_WS_TOO_SMALL               workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID           workSpace state invalid
 *                 all possible error codes which can occur in esl_getBytesRNG
 *                 ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW  total length of data to be hashed (while calculating the
 *                                                       saltedHashValue) is too long
 *                 ESL_ERC_INPUT_INVALID hashID that is passed to esl_calcSaltedHashRSA_PSS does not correspond
 *                                       to a supported hash function
 *                 ESL_ERC_ERROR an error occurred during the call to esl_generateMaskMGF1
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE   message to be signed is greater than the module
 *                 ESL_ERC_OUTPUT_SIZE_TOO_SHORT      *signatureSize is too small
 *                 ESL_ERC_RSA_ENCODING_INVALID       RSA encoded message length too short
 *  \pre           workSpace is initialized by esl_initSignRSASHA256_PSS(..), esl_updateSignRSASHA256(..) and
 *                 esl_finalizeHashSignRSASHA256_PSS(..) called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA256_DIGEST (= 32) byte long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSASHA256_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length saltSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeSigSignRSASHA256_PSS_NoSalt
 **********************************************************************************************************************/
/*! \brief         Generates the RSA PSS SHA256 signature by encoding and signing the message without using salt.
 *  \details       This function calls esl_finalizeSigSignRSASHA256_PSS() with the given input parameters, saltSize = 0
 *                 and a null pointer for salt.
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 all possible error codes which can occur in esl_finalizeSigSignRSASHA256_PSS
 *  \pre           workSpace is initialized by esl_initSignRSASHA256_PSS(..), esl_updateSignRSASHA256(..) and
 *                 esl_finalizeHashSignRSASHA256_PSS(..) called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA256_DIGEST (= 32) byte long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSASHA256_PSS_NoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/**********************************************************************************************************************
 *  esl_finalizeSigSignRSASHA256_PSS_DigestLengthSalt
 *********************************************************************************************************************/
/*! \brief         Generates the RSA PSS SHA256 signature by encoding and signing the message with random salt.
 *  \details       This function calls esl_finalizeSigSignRSASHA256_PSS() with the given input parameters,
 *                 saltSize equal to message digest length and a null pointer for salt, which means that during
 *                 encoding the message, a salt buffer of length ESL_SIZEOF_SHA256_DIGEST with random data will be
 *                 generated.
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 all possible error codes which can occur in esl_finalizeSigSignRSASHA256_PSS
 *  \pre           workSpace is initialized by esl_initSignRSASHA256_PSS(..), esl_updateSignRSASHA256(..) and
 *                 esl_finalizeHashSignRSASHA256_PSS(..) called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA256_DIGEST (= 32) byte long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSASHA256_PSS_DigestLengthSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeSignRSASHA256_PSS
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and generate signature for RSA PSS SHA256
 *  \details       This function calls esl_finalizeHashSignRSASHA256_PSS() and esl_finalizeSigSignRSASHA256_PSS()
 *                 with the given input parameters.
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in]     saltSize  Size of salt to be used
 *  \param[in]     salt  Salt to be used
 *                       0 (NULL) if salt should be generated randomly
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 all possible error codes which can occur in esl_finalizeSigSignRSASHA256_PSS
 *  \pre           workSpace is initialized by esl_initSignRSASHA256_PSS(..), esl_updateSignRSASHA256(..) called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *                 The buffer referenced by salt needs to be at least saltSize bytes long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSASHA256_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   eslt_Length saltSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeSignRSASHA256_PSS_NoSalt
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and generate signature for RSA PSS SHA256
 *  \details       This function calls esl_finalizeSignRSASHA256_PSS() with the given input parameters, saltSize = 0
 *                 and a null pointer for salt.
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 all possible error codes which can occur in esl_finalizeSignRSASHA256_PSS
 *  \pre           workSpace is initialized by esl_initSignRSASHA256_PSS(..), esl_updateSignRSASHA256(..) called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSASHA256_PSS_NoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeSignRSASHA256_PSS_DigestLengthSalt
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and generate signature for RSA PSS SHA256
 *  \details       This function calls esl_finalizeSignRSASHA256_PSS()with a null pointer for salt and saltSize
 *                 equal to message digest length for signature generation
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) Signature generation work space
 *  \param[in,out] signatureSize  (in)  Maximum size of signature
 *                                (out) Actual size of signature
 *  \param[in,out] signature (in)  buffer to store the signature value
 *                           (out) Generated signature value
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 all possible error codes which can occur in esl_finalizeSignRSASHA256_PSS
 *  \pre           workSpace is initialized by esl_initSignRSASHA256_PSS(..), esl_updateSignRSASHA256(..) called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSASHA256_PSS_DigestLengthSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_initVerifyRSASHA256_PSS
 **********************************************************************************************************************/
/*! \brief         Initialize signature verification with EMSA PSS encoding, based on SHA2-256
 *  \details       -
 *  \param[in,out] workSpace (in)  Signature generation work space
 *                           (out) initialized work space
 *  \param[in]     keyPairModuleSize  Size of RSA key pair module
 *  \param[in]     keyPairModule  RSA key pair module
 *  \param[in]     publicKeyExponentSize  Size of public RSA key pair exponent
 *  \param[in]     publicKeyExponent  Public RSA key pair exponent
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter workSpace/keyPairModule/publicKeyExponent is NULL or
 *                 exponent is larger than module
 *                 ESL_ERC_WS_TOO_SMALL  workSpace or RSAPrimWorkSpace, contained in workSpace, too small
 *                 ESL_ERC_RSA_PUBKEY_INVALID   publicExponentKeySize is invalid
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initVerifyRSASHA256_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
  eslt_Length keyPairModuleSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
  eslt_Length publicKeyExponentSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent);

/***********************************************************************************************************************
 *  esl_updateVerifyRSASHA256_PSS
 **********************************************************************************************************************/
/*! \brief         Update hash workspace with message to be signed
 *  \details       During the RSA PSS signature verification scheme, the original message needs to be hashed. For RSA
 *                 PSS SHA256, the hash function is SHA2-256. This function stores the message into the hash workspace,
 *                 contained in the parameter workSpace.
 *  \param[in,out] workSpace (in) Signature generation work space
 *                           (out) hash workspace contained in signature generation workspace updated with data
 *                                 to be hashed
 *  \param[in]     inputSize  Size of data to be hashed
 *  \param[in]     input  Data to be hashed, usually the original message
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID input parameter workSpace is null
 *                 ESL_ERC_WS_TOO_SMALL workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID workSpace in invalid state
 *                 ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW  Maximum total input length exceeded
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA256_PSS(..)
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateVerifyRSASHA256_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length inputSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) input);

/**********************************************************************************************************************
 *  esl_finalizeHashVerifyRSASHA256_PSS
 *********************************************************************************************************************/
/*! \brief         Calculate the hash value of the data in the hash workspace
 *  \details       During the RSA PSS signature generation scheme, the original message needs to be hashed. For RSA
 *                 PSS SHA256, the hash function is SHA2-256. The message was passed to the hash workSpace before,
 *                 which is contained in the parameter 'workSpace'. This function computes its hash value.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in,out] messageDigest (in)  buffer to store the message digest (hash)
 *                               (out) message digest
 *  \return        ESL_ERC_PARAMETER_INVALID input parameter workSpace or messageDigest is null
 *                 ESL_ERC_WS_TOO_SMALL workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID workSpace in invalid state
 *                 ESL_ERC_NO_ERROR  Operation successful
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA256_PSS(..), esl_updateVerifyRSASHA256(..) called
 *                 before. The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA256_DIGEST
 *                 (= 32) byte long, since this buffer is passed to actSHA256Finalize.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashVerifyRSASHA256_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest);

/***********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA256_PSS
 **********************************************************************************************************************/
/*! \brief         Verifies the RSA PSS SHA256 signature by applying the RSA verification primitive to the signature
 *                 and decoding the message.
 *  \details       -
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in]     saltSize  Expected size of salt,
 *                           -1 if size of salt used during signature generation should be evaluated automatically
 *  \param[in]     signatureSize  size of signature
 *  \param[in]     signature signature value to be verified
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  input parameter null or
 *                                            saltSize too high for salt to be stored in data block
 *                 ESL_ERC_WS_TOO_SMALL       workSpace too small
 *                 ESL_ERC_WS_STATE_INVALID   workSpace state invalid
 *                 ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW  total length of data to be hashed (while calculation the
 *                                                saltedHashValue) is too long
 *                 ESL_ERC_INPUT_INVALID hashID that is passed to esl_calcSaltedHashRSA_PSS does not correspond
 *                                       to a supported hash function
 *                 ESL_ERC_ERROR an error occurred during the call to esl_generateMaskMGF1
 *                 ESL_ERC_RSA_SIGNATURE_OUT_OF_RANGE   signature to be verified is greater than the module
 *                 ESL_ERC_RSA_SIGNATURE_INVALID signature verification failed
 *                 ESL_ERC_RSA_ENCODING_INVALID       RSA encoded message length too short
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA256_PSS(..), esl_updateVerifyRSASHA256(..) and
 *                 esl_finalizeHashVerifyRSASHA256_PSS(..) called before
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA256_DIGEST (= 32) byte long.
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA256_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length saltSize,
   eslt_Length signatureSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA256_PSS_AutoSalt
 **********************************************************************************************************************/
/*! \brief         Verifies the RSA PSS SHA256 signature by encoding and signing the message.
 *  \details       This function calls esl_verifySigVerifyRSASHA256_PSS() with the given input parameters and
 *                 saltSize = -1. This requires that the size of the salt used during signature generation was
 *                 evaluated automatically.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in]     signatureSize size of signature
 *  \param[in]     signature signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSASHA256_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA256_PSS(..), esl_updateVerifyRSASHA256(..) and
 *                 esl_finalizeHashVerifyRSASHA256_PSS(..) called before
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA256_DIGEST (= 32) byte long.
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA256_PSS_AutoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length signatureSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA256_PSS_NoSalt
 **********************************************************************************************************************/
/*! \brief         Verifies the RSA PSS SHA256 signature by encoding and signing the message.
 *  \details       This function calls esl_verifySigVerifyRSASHA256_PSS() with the given input parameters and
 *                 saltSize = 0. This requires that no salt was used during signature generation.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in]     signatureSize size of signature
 *  \param[in]     signature signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSASHA256_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA256_PSS(..), esl_updateVerifyRSASHA256(..) and
 *                 esl_finalizeHashVerifyRSASHA256_PSS(..) called before
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA256_DIGEST (= 32) byte long.
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA256_PSS_NoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length signatureSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA256_PSS_DigestLengthSalt
 **********************************************************************************************************************/
/*! \brief         Verifies the RSA PSS SHA256 signature by encoding and signing the message.
 *  \details       This function calls esl_verifySigVerifyRSASHA256_PSS() with the given input parameters and
 *                 saltSize = ESL_SIZEOF_SHA256_DIGEST. This requires that salt with size equal to message digest
 *                 length was used during signature generation.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     messageDigest  Message digest (hash) of previously hashed original message
 *  \param[in]     signatureSize size of signature
 *  \param[in]     signature signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSASHA256_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA256_PSS(..), esl_updateVerifyRSASHA256(..) and
 *                 esl_finalizeHashVerifyRSASHA256_PSS(..) called before
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA256_DIGEST (= 32) byte long.
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA256_PSS_DigestLengthSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length signatureSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA256_PSS
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and verify signature for RSA PSS SHA256
 *  \details       This function calls esl_finalizeHashVerifyRSASHA256_PSS() and esl_verifySigVerifyRSASHA256_PSS()
 *                 with the given input parameters.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     saltSize  Expected size of salt,
 *                           -1 if size of salt used during signature generation should be evaluated automatically
 *  \param[in]     signatureSize  Size of signature
 *  \param[in]     signature  Signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSASHA256_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA256_PSS(..), esl_updateVerifyRSASHA256(..)
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *                 called before
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA256_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length saltSize,
   eslt_Length signatureSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA256_PSS_AutoSalt
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and verify signature for RSA PSS SHA256
 *  \details       This function calls esl_finalizeVerifyRSASHA256_PSS() with the given input parameters and
 *                 saltSize = -1, thus it assumes that the size of the salt used during signature generation is
 *                 evaluated automatically.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     signatureSize  Size of signature
 *  \param[in]     signature  Signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSASHA256_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA256_PSS(..), esl_updateVerifyRSASHA256(..)
 *                 called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA256_PSS_AutoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length signatureSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA256_PSS_NoSalt
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and verify signature for RSA PSS SHA256
 *  \details       This function calls esl_finalizeVerifyRSASHA256_PSS() with the given input parameters and
 *                 saltSize = 0, thus it requires that no salt was used during signature generation.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     signatureSize  Size of signature
 *  \param[in]     signature  Signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSASHA256_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA256_PSS(..), esl_updateVerifyRSASHA256(..)
 *                 called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA256_PSS_NoSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length signatureSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/***********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA256_PSS_DigestLengthSalt
 **********************************************************************************************************************/
/*! \brief         Finalize message digest (hash) calculation and verify signature for RSA PSS SHA256
 *  \details       This function calls esl_finalizeVerifyRSASHA256_PSS() with the given input parameters and
 *                 saltSize = ESL_SIZEOF_SHA256_DIGEST, thus it  requires that a salt buffer of size equal to message
 *                 digest was used during signature generation.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out) Signature verification work space
 *  \param[in]     signatureSize  Size of signature
 *  \param[in]     signature  Signature value to be verified
 *  \return        all possible error codes which can occur in esl_verifySigVerifyRSASHA256_PSS
 *  \pre           workSpace is initialized by esl_initVerifyRSASHA256_PSS(..), esl_updateVerifyRSASHA256(..)
 *                 called before
 *                 The buffer referenced by signature needs to be at least signatureSize bytes long.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA256_PSS_DigestLengthSalt(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length signatureSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) signature);

/**********************************************************************************************************************
 *  esl_calcSaltedHashRSASHA256_PSS
 *********************************************************************************************************************/
/*! \brief      Calculate salted hash value (SHA256)
 *  \details    Based on a given hash and a random salt, the hash of
 *                M' = (0x) 00 00 00 00 00 00 00 00 || hash(orig. msg) || salt
 *              is calculated, as described in chapter 9.1.1, steps 5-6 and 9.1.2, steps 12-13 of PKCS#1 2.2
 *  \param[in,out] wsHash      (in)  Pointer to hash workspace
 *                             (out) Pointer to hash workspace
 *  \param[in]  saltSize       Length of salt
 *  \param[in]  salt           Input salt
 *  \param[in]  messageDigest  Input hash value of the original message, will be hashed again here
 *  \param[in,out] saltedHash  (in)  Output buffer for calculated hash (SHA256)
 *                             (out) calculated salted hash (SHA256)
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID wsHash or saltedHash null
 *                 ESL_ERC_WS_TOO_SMALL wsHash too small
 *                 ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW total length of data to be hashed too long
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           buffer referenced by saltedHash has to be at least of length ESL_SIZEOF_SHA256_DIGEST
 *                 The buffer referenced by messageDigest needs to be at least ESL_SIZEOF_SHA256_DIGEST (= 32) byte long.
 *                 The buffer referenced by salt needs to be at least saltSize bytes long.
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_calcSaltedHashRSASHA256_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA256) wsHash,
   eslt_Length saltSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   VSECPRIM_P2VAR_PARA(eslt_Byte) saltedHash);
# endif /* (VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON) */

/*****************************************************************************
 * Crypto function to generate MGF1 masks with different hash algorithms
 * according to PKCS#1 PSS
 *****************************************************************************/

# if (VSECPRIM_RSA_HASHALGORITHM_RIPEMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/**********************************************************************************************************************
 *  esl_generateMaskMGF1RSARIPEMD160_PSS()
 *********************************************************************************************************************/
/*! \brief      Mask generation function (MGF1)
 *  \details    MGF1 implementation as described in appendix B.2.1 of PKCS#1 2.2
 *              Based on hash function (RIPEMD-160) this function generates a pseudorandom output string of arbitrary length
 *              depending on an input string (seed) of variable length
 *  \param[in,out] wsHash (in)  Pointer to hash workspace
 *                        (out) Pointer to hash workspace
 *  \param[in,out] tempHash (in)  Temporary buffer for intermediate hash
 *                          (out) intermediate hash value
 *  \param[in]  seedLength  Length input seed
 *  \param[in]  seed        Input seed from which mask is generated
 *  \param[in]  maskLength  Length of mask to be generated
 *  \param[in,out] mask  (in)  Output buffer for generated mask
 *                       (out) generated  mask
 *  \return     ESL_ERC_NO_ERROR  Operation successful
 *              ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *              ESL_ERC_WS_TOO_SMALL  Work space too small
 *              ESL_ERC_ERROR an error occurred
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           buffer referenced by tempHash has to be at least of length ESL_SIZEOF_RIPEMD160_DIGEST
 *                 buffer referenced by mask has to be at least of length maskLength
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_generateMaskMGF1RSARIPEMD160_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRIPEMD160) wsHash,
  VSECPRIM_P2VAR_PARA(eslt_Byte) tempHash,
  eslt_Length seedLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) seed,
  eslt_Length maskLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) mask);
# endif /* (VSECPRIM_RSA_HASHALGORITHM_RIPEMD160_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_HASHALGORITHM_SHA1_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/**********************************************************************************************************************
 *  esl_generateMaskMGF1RSASHA1_PSS()
 *********************************************************************************************************************/
/*! \brief      Mask generation function (MGF1)
 *  \details    MGF1 implementation as described in appendix B.2.1 of PKCS#1 2.2
 *              Based on hash function (SHA1) this function generates a pseudorandom output string of arbitrary length
 *              depending on an input string (seed) of variable length
 *  \param[in,out] wsHash (in)  Pointer to hash workspace
 *                        (out) Pointer to hash workspace
 *  \param[in,out] tempHash (in)  Temporary buffer for intermediate hash
 *                          (out) intermediate hash value
 *  \param[in]  seedLength  Length input seed
 *  \param[in]  seed        Input seed from which mask is generated
 *  \param[in]  maskLength  Length of mask to be generated
 *  \param[in,out] mask  (in)  Output buffer for generated mask
 *                       (out) generated  mask
 *  \return     ESL_ERC_NO_ERROR  Operation successful
 *              ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *              ESL_ERC_WS_TOO_SMALL  Work space too small
 *              ESL_ERC_ERROR an error occurred
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           buffer referenced by tempHash has to be at least of length ESL_SIZEOF_SHA1_DIGEST
 *                 buffer referenced by mask has to be at least of length maskLength
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_generateMaskMGF1RSASHA1_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA1) wsHash,
  VSECPRIM_P2VAR_PARA(eslt_Byte) tempHash,
  eslt_Length seedLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) seed,
  eslt_Length maskLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) mask);
# endif /* (VSECPRIM_RSA_HASHALGORITHM_SHA1_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_HASHALGORITHM_SHA2_256_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  esl_generateMaskMGF1RSASHA256_PSS()
 *********************************************************************************************************************/
/*! \brief      Mask generation function (MGF1)
 *  \details    MGF1 implementation as described in appendix B.2.1 of PKCS#1 2.2
 *              Based on hash function (SHA256) this function generates a pseudorandom output string of arbitrary length
 *              depending on an input string (seed) of variable length
 *  \param[in,out] wsHash (in)  Pointer to hash workspace
 *                        (out) Pointer to hash workspace
 *  \param[in,out] tempHash (in)  Temporary buffer for intermediate hash
 *                          (out) intermediate hash value
 *  \param[in]  seedLength  Length input seed
 *  \param[in]  seed        Input seed from which mask is generated
 *  \param[in]  maskLength  Length of mask to be generated
 *  \param[in,out] mask  (in)  Output buffer for generated mask
 *                       (out) generated  mask
 *  \return     ESL_ERC_NO_ERROR  Operation successful
 *              ESL_ERC_PARAMETER_INVALID input parameter null
 *              ESL_ERC_WS_TOO_SMALL wsHash too small
 *              ESL_ERC_ERROR an error occurred
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           buffer referenced by tempHash has to be at least of length ESL_SIZEOF_SHA256_DIGEST
 *                 buffer referenced by mask has to be at least of length maskLength
 *                 The buffer referenced by seed needs to be at least seedLength bytes long.
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_generateMaskMGF1RSASHA256_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA256) wsHash,
  VSECPRIM_P2VAR_PARA(eslt_Byte) tempHash,
  eslt_Length seedLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) seed,
  eslt_Length maskLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) mask);
# endif /* (VSECPRIM_RSA_HASHALGORITHM_SHA2_256_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * Crypto function to verify Padding RSA EM
 * according to PKCS#1 1.5
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_verifyPaddingRSAEM_V15()
 *********************************************************************************************************************/
/*! \brief          Verify padding for RSA PKCS#1 V1.5
 *  \details        -
 *  \param[in]      message      Decrypted message
 *  \param[in,out]  paddingLength  (in) Length of message
 *                               (out) Length of padding
 *  \param[in]      blockType    Block type according to PKCS #1 1.5
 *                               ASN1_PADDING_BLOCK_TYPE_PRIVATE_ZERO
 *                               ASN1_PADDING_BLOCK_TYPE_PRIVATE
 *                               ASN1_PADDING_BLOCK_TYPE_PUBLIC
 *  \return         ESL_ERC_PARAMETER_INVALID     Message length smaller than minimum padding
 *                  ESL_ERC_RSA_ENCODING_INVALID  Invalid padding
 *                  ESL_ERC_NO_ERROR              else
 *  \pre            Work space initialized
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyPaddingRSAEM_V15(
  VSECPRIM_P2CONST_PARA(eslt_Byte) message, VSECPRIM_P2VAR_PARA(eslt_Length) paddingLength, eslt_Byte blockType);
# endif /* (VSECPRIM_RSA_ENABLED == STD_ON) */
/*****************************************************************************
 * Crypto functions EdDSA & ECDH
 *****************************************************************************/

# if (VSECPRIM_ECDSA_25519_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  esl_initSignEdDSA()
 *********************************************************************************************************************/
/*! \brief       Init EdDSA.
 *  \details     This function initializes the EdDSA signature.
 *  \param[in,out]  workSpace  algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *  \param[in]   curve  underlying curve; supports
 *                      - ESL_Curve25519
 *  \param[in]   instance  EdDSA instance to use; one of
 *                         - ESL_INSTANCE_Ed25519
 *                         - ESL_INSTANCE_Ed25519ctx
 *                         - ESL_INSTANCE_Ed25519ph
 *  \param[in]   context  Context data pointer
 *  \param[in]   contextLength The length of the context data
 *  \return      ESL_ERC_NO_ERROR - success
 *               ESL_ERC_PARAMETER_INVALID      - some parameter is NULL
 *               ESL_ERC_CURVE_NOT_SUPPORTED    - unknown curve
 *               ESL_ERC_INSTANCE_NOT_SUPPORTED - unknown EdDSA instance
 *               ESL_ERC_WS_TOO_SMALL           - work space too small
 *  \pre         workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignEdDSA(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEd25519) workSpace,
  eslt_Size16 curve,
  const eslt_Byte instance,
  VSECPRIM_P2CONST_PARA(eslt_Byte) context,
  const eslt_Length contextLength);

/**********************************************************************************************************************
 *  esl_signEdDSA()
 *********************************************************************************************************************/
/*! \brief       Sign data EdDSA.
 *  \details     This function signs the given input data.
 *  \param[in,out]  workSpace  algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *  \param[in]   input  Pointer to the input data
 *  \param[in]   inputLength  The length of the provided input data
 *  \param[in]   secretKey  Pointer to the secret key of length 32 byte to use for signature
 *  \param[in]   publicKey  Pointer to the public key of length 32 byte. Pass null pointer to calculate from private key.
 *  \param[out]   signature  Pointer to where the signature shall be stored.
 *  \param[in,out]   signatureLength  Pointer to where the size of the buffer is provided
 *  \return      ESL_ERC_NO_ERROR - success
 *               ESL_ERC_PARAMETER_INVALID     - some parameter is NULL
 *               ESL_ERC_WS_STATE_INVALID      - work space state invalid
 *               ESL_ERC_WS_TOO_SMALL          - work space too small
 *               ESL_ERC_BUFFER_TOO_SMALL      - buffer for signature too small
 *               ESL_ERC_TOTAL_LENGTH_OVERFLOW - internal overflow (== message too long - SHA-512)
 *  \pre         workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_signEdDSA(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEd25519) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input,
  eslt_Length inputLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) secretKey,
  VSECPRIM_P2CONST_PARA(eslt_Byte) publicKey,
  VSECPRIM_P2VAR_PARA(eslt_Byte) signature,
  VSECPRIM_P2VAR_PARA(eslt_Length) signatureLength);

/**********************************************************************************************************************
 *  esl_initVerifyEdDSA()
 *********************************************************************************************************************/
/*! \brief       Init verification for EdDSA.
 *  \details     This function initializes the EdDSA verification.
 *  \param[in,out]  workSpace  algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *  \param[in]   curve  Underlying curve; supports
 *                      - ESL_Curve25519
 *  \param[in]   instance  EdDSA instance to use; one of
 *                         - ESL_INSTANCE_Ed25519
 *                         - ESL_INSTANCE_Ed25519ctx
 *                         - ESL_INSTANCE_Ed25519ph
 *  \param[in]   context  Pointer to the context data
 *  \param[in]   contextLength  Size of provided context data
 *  \return      ESL_ERC_NO_ERROR - success
 *               ESL_ERC_PARAMETER_INVALID      - some parameter is NULL
 *               ESL_ERC_CURVE_NOT_SUPPORTED    - unknown curve
 *               ESL_ERC_INSTANCE_NOT_SUPPORTED - unknown EdDSA instance
 *               ESL_ERC_WS_TOO_SMALL           - work space too small
 *  \pre         workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initVerifyEdDSA(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEd25519) workSpace,
  eslt_Size16 curve,
  const eslt_Byte instance,
  VSECPRIM_P2CONST_PARA(eslt_Byte) context,
  const eslt_Length contextLength);

/**********************************************************************************************************************
 *  esl_verifyEdDSA()
 *********************************************************************************************************************/
/*! \brief       EdDSA verification.
 *  \details     This function verifies given signature against the given data.
 *  \param[in,out]  workSpace  algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *  \param[in]   input  Pointer to the input data.
 *  \param[in]   inputLength  The length of the input data
 *  \param[in]   publicKey  Pointer to the public key
 *  \param[in]   signature  Pointer to the signature to be validated against input data
 *  \param[in]   signatureLength  Length of the signature
 *  \return      ESL_ERC_NO_ERROR - success
 *               ESL_ERC_PARAMETER_INVALID     - some parameter is NULL
 *               ESL_ERC_WS_STATE_INVALID      - work space state invalid
 *               ESL_ERC_WS_TOO_SMALL          - work space too small
 *               ESL_ERC_TOTAL_LENGTH_OVERFLOW - internal overflow (== message too long - SHA-512)
 *               ESL_ERC_SIGNATURE_INVALID     - the signature does NOT verify
 *  \pre         workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyEdDSA(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEd25519) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input,
  eslt_Length inputLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) publicKey,
  VSECPRIM_P2CONST_PARA(eslt_Byte) signature,
  eslt_Length signatureLength);

/**********************************************************************************************************************
 *  esl_updateEdDSA()
 *********************************************************************************************************************/
/*! \brief       Perform hashing
 *  \details     This function updates the internal hash value in a pre-hash EdDSA signature OR verification.
 *  \param[in,out]  workSpace  algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *  \param[in]   input  Pointer to the input data.
 *  \param[in]   inputLength  The length of the input data
 *  \return      ESL_ERC_NO_ERROR - success
 *               ESL_ERC_PARAMETER_INVALID     - some parameter is NULL
 *               ESL_ERC_WS_STATE_INVALID      - work space state invalid
 *               ESL_ERC_WS_TOO_SMALL          - work space too small
 *               ESL_ERC_TOTAL_LENGTH_OVERFLOW - internal overflow (== message too long - SHA-512)
 *  \pre         workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateEdDSA(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEd25519) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input,
  eslt_Length inputLength);

/**********************************************************************************************************************
 *  esl_initGenerateKeyPairEdDSA()
 *********************************************************************************************************************/
/*! \brief       Initialize generated key pair
 *  \details     This function initializes the EdDSA key-pair generation.
 *  \param[in,out]  workSpace  algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *  \param[in]   curve  Underlying curve; supports
 *                      - ESL_Curve25519
 *  \return      ESL_ERC_NO_ERROR - success
 *               ESL_ERC_PARAMETER_INVALID   - some parameter is NULL
 *               ESL_ERC_CURVE_NOT_SUPPORTED - unknown curve
 *               ESL_ERC_WS_TOO_SMALL        - work space too small
 *  \pre         workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initGenerateKeyPairEdDSA(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEd25519) workSpace,
  eslt_Size16 curve);

/**********************************************************************************************************************
 *  esl_generateKeyPairEdDSA()
 *********************************************************************************************************************/
/*! \brief       Generate Key Pair
 *  \details     This function generates an EdDSA key-pair.
 *  \param[in,out]  workSpace  algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *  \param[out]  secretKey  Pointer to where the secret key shall be stored
 *  \param[in,out]  secretKeyLength  Length of the provided buffer/Length of the written data
 *  \param[out]  publicKey  Pointer to where the public key shall be stored
 *  \param[in,out]  publicKeyLength  Length of the provided buffer/Length of the written data
 *  \return      ESL_ERC_NO_ERROR - success
 *               ESL_ERC_PARAMETER_INVALID - some parameter is NULL
 *               ESL_ERC_WS_STATE_INVALID  - work space state invalid
 *               ESL_ERC_WS_TOO_SMALL      - work space too small
 *               ESL_ERC_BUFFER_TOO_SMALL  - buffer for private or public key too small
 *               ESL_ERC_RNG_FAILED        - the random number generator failed
 *  \pre         workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_generateKeyPairEdDSA(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEd25519) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) secretKey,
  VSECPRIM_P2VAR_PARA(eslt_Length) secretKeyLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) publicKey,
  VSECPRIM_P2VAR_PARA(eslt_Length) publicKeyLength);

# endif /* (VSECPRIM_ECDSA_25519_ENABLED == STD_ON) */

# if (VSECPRIM_ECDH_25519_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  esl_initECDH()
 *********************************************************************************************************************/
/*! \brief       Init ECDH
 *  \details     This function initializes the EC-D/H key-exchange.
 *  \param[in,out]  workSpace  algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *  \param[in]     curve  underlying curve; supports
 *                      - ESL_Curve25519
 *  \return      ESL_ERC_NO_ERROR - success
 *               ESL_ERC_PARAMETER_INVALID - some parameter is NULL
 *               ESL_ERC_CURVE_NOT_SUPPORTED - unknown curve
 *               ESL_ERC_WS_TOO_SMALL - work space too small
 *  \pre         workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initECDH(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceX25519) workSpace,
  eslt_Size16 curve);

/**********************************************************************************************************************
 *  esl_generateEphemeralKeyPairECDH()
 *********************************************************************************************************************/
/*! \brief       Generate Ephemeral Key Pair.
 *  \details     This function generates an ephemeral key-pair for EC-D/H.
 *  \param[in,out]  workSpace  Algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *  \param[in,out]  publicKey  Pointer to public key memory
 *  \param[in,out]  publicKeyLength  Pointer to public key memory size
 *  \return      ESL_ERC_NO_ERROR - success
 *               ESL_ERC_PARAMETER_INVALID - some parameter is NULL
 *               ESL_ERC_WS_STATE_INVALID  - work space state invalid
 *               ESL_ERC_WS_TOO_SMALL      - work space too small
 *               ESL_ERC_BUFFER_TOO_SMALL  - buffer for private or public key too small
 *               ESL_ERC_RNG_FAILED        - the random number generator failed
 *  \pre         workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_generateEphemeralKeyPairECDH(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceX25519) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) publicKey,
  VSECPRIM_P2VAR_PARA(eslt_Length) publicKeyLength);

/**********************************************************************************************************************
 *  esl_importStaticPrivateKeyECDH()
 *********************************************************************************************************************/
/*! \brief       Import private key
 *  \details     This function imports a static private key for EC-D/H.
 *  \param[in,out]  workSpace  Algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *  \param[in]   privateKey  Pointer to private key
 *  \return      ESL_ERC_NO_ERROR - success
 *               ESL_ERC_PARAMETER_INVALID - some parameter is NULL
 *               ESL_ERC_WS_STATE_INVALID  - work space state invalid
 *               ESL_ERC_WS_TOO_SMALL      - work space too small
 *               ESL_ERC_BUFFER_TOO_SMALL  - buffer for private or public key too small
 *               ESL_ERC_RNG_FAILED        - the random number generator failed
 *  \pre         workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_importStaticPrivateKeyECDH(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceX25519) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) privateKey);

/**********************************************************************************************************************
 *  esl_generateSharedSecretECDH()
 *********************************************************************************************************************/
/*! \brief       Generate shared secret
 *  \details     This function generates the shared secret EC-D/H.
 *  \param[in,out]  workSpace  Algorithm context buffer, initialized by esl_initWorkSpaceHeader(..) function
 *  \param[in,out]  publicKey  Pointer to public key
 *  \param[out]     sharedSecret  Pointer to shared secret memory
 *  \param[in,out]  sharedSecretLength  Pointer to shared secret memory size
 *  \return      ESL_ERC_NO_ERROR - success
 *               ESL_ERC_PARAMETER_INVALID - some parameter is NULL
 *               ESL_ERC_WS_STATE_INVALID  - work space state invalid
 *               ESL_ERC_WS_TOO_SMALL      - work space too small
 *               ESL_ERC_BUFFER_TOO_SMALL  - buffer for shared secret too small
 *  \pre         workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_generateSharedSecretECDH(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceX25519) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) publicKey,
  VSECPRIM_P2VAR_PARA(eslt_Byte) sharedSecret,
  VSECPRIM_P2VAR_PARA(eslt_Length) sharedSecretLength);
# endif /* (VSECPRIM_ECDH_25519_ENABLED == STD_ON) */

# if (VSECPRIM_CHA_CHA20_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * Crypto functions ChaCha20
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_initEncryptChaCha20()
 *********************************************************************************************************************/
/*! \brief          This function initializes the ChaCha20 stream encryption.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader () function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to key of length ESL_SIZEOF_ChaCha20_KEY
 *  \param[in]      nonce        pointer to nonce of length ESL_SIZEOF_ChaCha20_NONCE
 *  \param[in]      blockCount   initial block count
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader ()
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptChaCha20(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaCha20) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) key,
  VSECPRIM_P2CONST_PARA(eslt_Byte) nonce,
  const eslt_Size32 blockCount);

/**********************************************************************************************************************
 *  esl_initDecryptChaCha20()
 *********************************************************************************************************************/
/*! \brief          This function initializes the ChaCha20 stream DEcryption.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader () function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to key of length ESL_SIZEOF_ChaCha20_KEY
 *  \param[in]      nonce        pointer to nonce of length ESL_SIZEOF_ChaCha20_NONCE
 *  \param[in]      blockCount   initial block count
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader ()
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptChaCha20(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaCha20) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) key,
  VSECPRIM_P2CONST_PARA(eslt_Byte) nonce,
  const eslt_Size32 blockCount);

/**********************************************************************************************************************
 *  esl_encryptChaCha20()
 *********************************************************************************************************************/
/*! \brief          This function encrypts input data of any length and can be called
 *                  arbitrarily often after the algorithm initialization.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initChaCha20 () function
 *                               (out) updated algorithm context buffer
 *  \param[in]      in           pointer to input data
 *  \param[in]      inSize       length of input data (bytes)
 *  \param[in,out]  out          pointer to output buffer
 *  \param[in,out]  outSize      pointer to size of output buffer (bytes)
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter null
 *                  ESL_ERC_BUFFER_TOO_SMALL outSize too small
 *                  ESL_ERC_WS_STATE_INVALID workspace state invalid
 *                  ESL_ERC_NO_ERROR operation successful
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader ()
 *  \note           in and out can point to the same address, i.e., inplace encryption is possible
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptChaCha20(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaCha20) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) in,
  const eslt_Length inSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte) out,
  VSECPRIM_P2VAR_PARA(eslt_Length) outSize);

/**********************************************************************************************************************
 *  esl_finalizeEncryptChaCha20()
 *********************************************************************************************************************/
/*! \brief          This function encrypts the input data of any length and resets and clears the workspace
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initChaCha20 () function
 *                               (out) updated algorithm context buffer
 *  \param[in]      in           pointer to input data
 *  \param[in]      inSize       length of input data (bytes)
 *  \param[in,out]  out          pointer to output buffer
 *  \param[in,out]  outSize      pointer to size of output buffer (bytes)
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter null
 *                  ESL_ERC_BUFFER_TOO_SMALL outSize too small
 *                  ESL_ERC_WS_STATE_INVALID workspace state invalid
 *                  ESL_ERC_NO_ERROR operation successful
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader ()
 *  \note           in and out can point to the same address, i.e., inplace encryption is possible
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeEncryptChaCha20(
    VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaCha20) workSpace,
    VSECPRIM_P2CONST_PARA(eslt_Byte) in,
    const eslt_Length inSize,
    VSECPRIM_P2VAR_PARA(eslt_Byte) out,
    VSECPRIM_P2VAR_PARA(eslt_Length) outSize);

/**********************************************************************************************************************
 *  esl_decryptChaCha20()
 *********************************************************************************************************************/
/*! \brief          This function DEcrypts input data of any length and can be called
 *                  arbitrarily often after the algorithm initialization.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initChaCha20 () function
 *  \param[in]      in           pointer to input data
 *  \param[in]      inSize       length of input data (bytes)
 *  \param[in,out]  out          pointer to output buffer
 *                               (out) updated algorithm context buffer
 *  \param[in,out]  outSize      size of output buffer (bytes)
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter null
 *                  ESL_ERC_BUFFER_TOO_SMALL outSize too small
 *                  ESL_ERC_WS_STATE_INVALID workspace state invalid
 *                  ESL_ERC_NO_ERROR operation successful
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader ()
 *  \note           in and out can point to the same address, i.e., inplace decryption is possible
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptChaCha20(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaCha20) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) in,
  const eslt_Length inSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte) out,
  VSECPRIM_P2VAR_PARA(eslt_Length) outSize);

/**********************************************************************************************************************
 *  esl_finalizeDecryptChaCha20()
 *********************************************************************************************************************/
/*! \brief          This function decrypts the input data of any length and resets and clears the workspace
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initChaCha20 () function
 *                               (out) updated algorithm context buffer
 *  \param[in]      in           pointer to input data
 *  \param[in]      inSize       length of input data (bytes)
 *  \param[in,out]  out          pointer to output buffer
 *  \param[in,out]  outSize      size of output buffer (bytes)
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter null
 *                  ESL_ERC_BUFFER_TOO_SMALL outSize too small
 *                  ESL_ERC_WS_STATE_INVALID workspace state invalid
 *                  ESL_ERC_NO_ERROR operation successful
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader ()
 *  \note           in and out can point to the same address, i.e., inplace decryption is possible
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeDecryptChaCha20(
    VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaCha20) workSpace,
    VSECPRIM_P2CONST_PARA(eslt_Byte) in,
    const eslt_Length inSize,
    VSECPRIM_P2VAR_PARA(eslt_Byte) out,
    VSECPRIM_P2VAR_PARA(eslt_Length) outSize);
# endif /* (VSECPRIM_CHA_CHA20_ENABLED == STD_ON) */

# if (VSECPRIM_POLY1305_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
      /*****************************************************************************
 * Crypto functions Poly1305
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_initPoly1305()
 *********************************************************************************************************************/
/*! \brief          This function initializes the Poly1305 autenticator.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader () function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to key of length ESL_SIZEOF_Poly1305_KEY
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader ()
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initPoly1305(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpacePoly1305) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 *  esl_updatePoly1305()
 *********************************************************************************************************************/
/*! \brief          This function updates the Poly1305 authenticator.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initPoly1305 () function
 *                               (out) updated algorithm context structure
 *  \param[in]      message      pointer to message data
 *  \param[in]      msgSize      length of message data (bytes)
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by initPoly1305 ()
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updatePoly1305(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpacePoly1305) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) message,
  const eslt_Length msgSize);

/**********************************************************************************************************************
 *  esl_finalizePoly1305()
 *********************************************************************************************************************/
/*! \brief          This function calculates and returns the Poly1305 TAG.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initPoly1305 () function
 *                               (out) updated algorithm context buffer
 *  \param[in,out]  tag          (in) pointer to TAG (output) buffer;
 *                               size must be at least ESL_SIZEOF_Poly1305_TAG
 *                               (out) authentication TAG calculated
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID  work space state invalid
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by initPoly1305 ()
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizePoly1305(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpacePoly1305) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) tag);

/**********************************************************************************************************************
 *  esl_verifyPoly1305()
 *********************************************************************************************************************/
/*! \brief          This function verifies a Poly1305 TAG.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initPoly1305 () function
 *                               (out) updated algorithm context buffer
 *  \param[in]      tag          (in) pointer to TAG (input)
 *                               (out) authentication TAG calculated
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID work space state invalid
 *                  ESL_ERC_INCORRECT_TAG the given TAG is incorrect
 *                  ESL_ERC_NO_ERROR Poly1305 TAG is correct
 *  \pre            workSpace is initialized by initPoly1305 ()
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyPoly1305(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpacePoly1305) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) tag);
# endif /* (VSECPRIM_POLY1305_ENABLED == STD_ON) */

# if (VSECPRIM_AEAD_CHA_CHA20_POLY1305_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * Crypto functions AEAD_ChaCha20_Poly1305
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_initEncryptChaChaPoly()
 *********************************************************************************************************************/
/*! \brief          This function initializes the AEAD_ChaCha20_Poly1305 (RFC 7539)
 *                  stream encryption and authentication TAG creation.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader () function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to key of length ESL_SIZEOF_ChaCha20_KEY
 *  \param[in]      nonce        pointer to nonce of length ESL_SIZEOF_ChaCha20_NONCE
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL work space too small
 *                  ESL_ERC_NO_ERROR else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader ()
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptChaChaPoly(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaChaPoly) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) key,
  VSECPRIM_P2CONST_PARA(eslt_Byte) nonce);

/**********************************************************************************************************************
 *  esl_initDecryptChaChaPoly()
 *********************************************************************************************************************/
/*! \brief          This function initializes the AEAD_ChaCha20_Poly1305
 *                  stream decryption and authentication TAG verification.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader () function
 *                               (out) initialized algorithm context structure
 *  \param[in]      key          pointer to key of length ESL_SIZEOF_ChaCha20_KEY
 *  \param[in]      nonce        pointer to nonce of length ESL_SIZEOF_ChaCha20_NONCE
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL work space too small
 *                  ESL_ERC_NO_ERROR else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader ()
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptChaChaPoly(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaChaPoly) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) key,
  VSECPRIM_P2CONST_PARA(eslt_Byte) nonce);

/**********************************************************************************************************************
 *  esl_updateAADChaChaPoly()
 *********************************************************************************************************************/
/*! \brief          This function passes additional authenticated data (AAD)
 *                  to the AEAD_ChaCha20_Poly1305.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initXXcryptChaChaPoly () function
 *                               (out) internal state updated
 *  \param[in]      aad          pointer to AAD
 *  \param[in]      aadSize      length of AAD (bytes)
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID work space state invalid
 *                  ESL_ERC_TOTAL_LENGTH_OVERFLOW length of AAD > 2^64 -1
 *                  ESL_ERC_NO_ERROR else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader ()
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateAADChaChaPoly(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaChaPoly) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) aad,
  const eslt_Length aadSize);

/**********************************************************************************************************************
 *  esl_updateDataChaChaPoly()
 *********************************************************************************************************************/
/*! \brief          This function passes data to be encrypted / decrypted
 *                  to the AEAD_ChaCha20_Poly1305.
 *                  When the workSpace has been initialized for encryption,
 *                  "in" is the message and "out" is the cipher; in case of
 *                  decryption, it's the other way 'round.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initXXcryptChaChaPoly () function
 *                               (out) internal state updated
 *  \param[in]      in           pointer to input data
 *  \param[in]      inSize       length of input data (bytes)
 *  \param[in,out]  out          pointer to output buffer
 *  \param[out]     outSize      length of encrypted data
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader ()
 *  \notes          in and out can reference the same address, i.e., in-place encryption and decryption is possible
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateDataChaChaPoly(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaChaPoly) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) in,
  const eslt_Length inSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte) out,
  VSECPRIM_P2VAR_PARA(eslt_Length) outSize);

/**********************************************************************************************************************
 *  esl_finalizeChaChaPoly()
 *********************************************************************************************************************/
/*! \brief          This function calculates the authentication TAG of the
 *                  AEAD_ChaCha20_Poly1305.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initEncryptChaChaPoly () function
 *                               (out) internal state updated
 *  \param[in,out]  tag          pointer to TAG buffer of size ESL_SIZEOF_Poly1305_TAG
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID work space state invalid
 *                  ESL_ERC_NO_ERROR else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader ()
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeChaChaPoly(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaChaPoly) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) tag);

/**********************************************************************************************************************
 *  esl_verifyChaChaPoly()
 *********************************************************************************************************************/
/*! \brief          This function verifies the authentication TAG of the
 *                  AEAD_ChaCha20_Poly1305.
 *  \details        -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initDecryptChaChaPoly () function
 *                               (out) internal state updated
 *  \param[in]      tag          pointer to TAG of length ESL_SIZEOF_Poly1305_TAG
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID work space state invalid
 *                  ESL_ERC_INCORRECT_TAG the given TAG is incorrect
 *                  ESL_ERC_NO_ERROR else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader ()
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyChaChaPoly(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaChaPoly) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) tag);
# endif /* (VSECPRIM_AEAD_CHA_CHA20_POLY1305_ENABLED == STD_ON) */

# if (VSECPRIM_ECBD_GENERIC_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
 * Burmester-Desmedt key agreement protocol
 *****************************************************************************/

/**********************************************************************************************************************
 *  esl_initECBD()
 *********************************************************************************************************************/
/*! \brief           This function initializes the EC-B/D key agreement.
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *  \param[in]      nDevices     number of devices partaking in the key agreement
 *  \param[in]      deviceID     ID of this device (1..nDevices)
 *  \param[in]      domain       domain parameter structure
 *  \param[in]      domainExt    domain parameter extension structure
 *  \param[in]      speedUpExt   pre-computation structure
 *  \return         ESL_ERC_PARAMETER_INVALID     some parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL          work space too small
 *                  ESL_ERC_INPUT_INVALID         deviceID is not matching nDevices
 *                  ESL_ERC_NO_ERROR              otherwise
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *                  domain and domainExt have to be valid ASN1 structures for EC domain parameters
 *                  speedUpExt has to be a valid ASN1 struct for precomputed EC points
 *                  The addresses of domain, domainExt and speedUpExt have to be valid until returning from
 *                  esl_generateEphemeralKeyPairECBD(), esl_calculateIntermediateECBD(), esl_initSharedSecretECBD,
 *                  esl_updateSharedSecretECBD and esl_retrieveSharedSecretECBD, because the pointers to the data
 *                  stored in domain, domainExt and speedUpExt are stored in the workspace. These pointers are used
 *                  as long as the workspace is used.
 *  \note           For information about the expected data format in domain, domainExt and speedUpExt see comment
 *                  in file ESLib_t.h, Section 'Algorithm specific types and constants'
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_initECBD(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceECBD) workSpace,
  const eslt_Size32 nDevices,
  const eslt_Size32 deviceID,
  VSECPRIM_P2CONST_PARA(eslt_EccDomain) domain,
  VSECPRIM_P2CONST_PARA(eslt_EccDomainExt) domainExt,
  VSECPRIM_P2CONST_PARA(eslt_EccSpeedUpExt) speedUpExt);

/**********************************************************************************************************************
 *  esl_generateEphemeralKeyPairECBD()
 *********************************************************************************************************************/
/*! \brief          This function generates an ephemeral key-pair for EC-B/D.
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initECBD(..) function
 *  \param[in,out]  privateKey   (in) pointer to memory for private key (if NULL, key is not returned)
 *                               (out) private key (if pointer not NULL)
 *  \param[in,out]  publicKey_x  (in) pointer to memory for public key (X coordinate)
 *                               (out) public key (X coordinate)
 *  \param[in,out]  publicKey_y  (in) pointer to memory for public key (Y coordinate)
 *                               (out) public key (Y coordinate)
 *  \return         ESL_ERC_PARAMETER_INVALID   some parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID    work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL        work space too small
 *                  ESL_ERC_RNG_FAILED          the random number generator failed
 *                  ESL_ERC_ECC_INTERNAL_ERROR  calculation failed
 *                  ESL_ERC_NO_ERROR            otherwise
 *  \pre            workSpace is initialized by esl_initECBD(..)
 *                  both publicKey_x and .._y are sufficiently large memory areas
(>= prime length of the underlying curve)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_generateEphemeralKeyPairECBD(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceECBD) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) privateKey,
  VSECPRIM_P2VAR_PARA(eslt_Byte) publicKey_x,
  VSECPRIM_P2VAR_PARA(eslt_Byte) publicKey_y);

/**********************************************************************************************************************
 *  esl_calculateIntermediateECBD()
 *********************************************************************************************************************/
/*! \brief          This function calculates the intermediate value (Xi)
 *                  from the left and right neighbors' public keys.
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initECBD(..) function
 *  \param[in]      pubKeyLeft_x  pointer to left  neighbor's public key (Zi-1 - X coordinate)
 *  \param[in]      pubKeyLeft_y  pointer to left  neighbor's public key (Zi-1 - Y coordinate)
 *  \param[in]      pubKeyRight_x  pointer to right neighbor's public key (Zi+1 - X coordinate)
 *  \param[in]      pubKeyRight_y  pointer to right neighbor's public key (Zi+1 - Y coordinate)
 *  \param[in,out]  intermediate_x  (in) pointer to memory for intermediate value (Xi - X coordinate)
 *                               (out) intermediate value (Xi - X coordinate)
 *  \param[in,out]  intermediate_y  (in) pointer to memory for intermediate value (Xi - Y coordinate)
 *                               (out) intermediate value (Xi - Y coordinate)
 *  \return         ESL_ERC_PARAMETER_INVALID   some parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID    work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL        work space too small
 *                  ESL_ERC_ECC_INTERNAL_ERROR  calculation failed
 *                  ESL_ERC_NO_ERROR            otherwise
 *  \pre            workSpace is initialized by esl_initECBD(..)
 *                  both intermediate_x and .._y are sufficiently large memory areas
(>= prime length of the underlying curve)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_calculateIntermediateECBD(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceECBD) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pubKeyLeft_x,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pubKeyLeft_y,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pubKeyRight_x,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pubKeyRight_y,
  VSECPRIM_P2VAR_PARA(eslt_Byte) intermediate_x,
  VSECPRIM_P2VAR_PARA(eslt_Byte) intermediate_y);

/**********************************************************************************************************************
 *  esl_initSharedSecretECBD()
 *********************************************************************************************************************/
/*! \brief           This function initializes the shared secret generation for EC-B/D.
 *                   It calculates the starting value from already known values (stored in the work space)
 *                   Zi, ai, Xi-1 and nNodes
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initECBD(..) function
 *  \return         ESL_ERC_PARAMETER_INVALID   some parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID    work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL        work space too small
 *                  ESL_ERC_NO_ERROR            otherwise
 *  \pre            workSpace is initialized by esl_initECBD(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC( eslt_ErrorCode ) esl_initSharedSecretECBD(
  VSECPRIM_P2VAR_PARA( eslt_WorkSpaceECBD ) workSpace);

/**********************************************************************************************************************
 *  esl_updateSharedSecretECBD()
 *********************************************************************************************************************/
/*! \brief           This function updates the shared secret with an intermediate from another ECU.
 *                   It is to be called repeatedly for all intermediate values received.
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initECBD(..) function
 *  \param[in]      deviceID     ID of the device the intermediate was received from
 *  \param[in]      intermediate_x  pointer to intermediate value (Xi - X coordinate)
 *  \param[in]      intermediate_y  pointer to intermediate value (Xi - Y coordinate)
 *  \return         ESL_ERC_PARAMETER_INVALID   some parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID    work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL        work space too small
 *                  ESL_ERC_NO_ERROR            otherwise
 *  \pre            workSpace is initialized by esl_initECBD(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSharedSecretECBD(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceECBD) workSpace,
  const eslt_Size32          deviceID,
  VSECPRIM_P2CONST_PARA(eslt_Byte) intermediate_x,
  VSECPRIM_P2CONST_PARA(eslt_Byte) intermediate_y);

/**********************************************************************************************************************
 *  esl_retrieveSharedSecretECBD()
 *********************************************************************************************************************/
/*! \brief           This function retrieves the shared secret (K).
 *                   When either sharedSecret_x or .._y are NULL, the respective value is DISCARDED.
 *  \details        -
 *  \param[in,out]  workSpace    algorithm context buffer, initialized by
 *                               esl_initECBD(..) function
 *  \param[in,out]  sharedSecret_x  (in) pointer to memory for shared secret (K - X coordinate)
 *                               (out) shared secret (K - X coordinate)
 *  \param[in,out]  sharedSecret_y  (in) pointer to memory for shared secret (K - Y coordinate)
 *                               (if NULL, coordinate is not returned)
 *                               (out) shared secret (K - Y coordinate)
 *  \return         ESL_ERC_PARAMETER_INVALID   some parameter is NULL
 *                  ESL_ERC_WS_STATE_INVALID    work space state invalid
 *                  ESL_ERC_WS_TOO_SMALL        work space too small
 *                  ESL_ERC_ECC_INTERNAL_ERROR  calculation failed
 *                  ESL_ERC_NO_ERROR            otherwise
 *  \pre            workSpace is initialized by esl_initECBD(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(eslt_ErrorCode) esl_retrieveSharedSecretECBD(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceECBD) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) sharedSecret_x,
  VSECPRIM_P2VAR_PARA(eslt_Byte) sharedSecret_y);
# endif /* (VSECPRIM_ECBD_GENERIC_ENABLED == STD_ON) */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

#endif /* ESLIB_H */

/**********************************************************************************************************************
 *  END OF FILE  ESLib_types.h
 *********************************************************************************************************************/
