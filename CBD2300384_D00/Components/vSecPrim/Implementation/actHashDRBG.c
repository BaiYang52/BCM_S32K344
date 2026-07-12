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
/*!      \file  actHashDRBG.c
 *      \brief  Hash DRBG implementation, based on SHA512
 *
 *      \details  Implementation for RANDOM Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define ACT_HASHDRBG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actIDRBG.h"
#include "actDRBGCommon.h"
#include "actISHA2_64.h"
#include "actUtilities.h"

#if (VSECPRIM_ACTHASHDRBG_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actHashDrbgAdd()
 **********************************************************************************************************************/
/*! \brief          Computes (bufferA + bufferB) mod 2^seedlen
 *  \details        Helper function to compute (V + H + C + reseedCount) mod 2^seedlen using big endian byte order.
 *                  BufferB can be shorter than bufferA and is expanded to bufferA's length before the addition.
 *  \param[in]      bufferA      pointer to first buffer
 *  \param[in]      bufferB      pointer to second buffer
 *  \param[in]      bufferBLength  length of bufferB in bytes
 *  \param[in,out]  result         pointer to buffer to store the result
 *                                 result: (bufferA + bufferB) mod 2^seedlen
 *  \pre            All pointers have to be valid.
 *                  The buffer referenced by extendedBufferB has to provide at least actHASHDRBG_SHA512_SEED_LEN bytes.
 *                  The buffer referenced by result has to provide at least actHASHDRBG_SHA512_SEED_LEN bytes.
 *  \note           bufferA and result can overlap.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actHashDrbgAdd(
  VSECPRIM_P2CONST_PARA(actU8) bufferA,
  VSECPRIM_P2CONST_PARA(actU8) bufferB,
  actLengthType bufferBLength,
  VSECPRIM_P2VAR_PARA(actU8) result);

/**********************************************************************************************************************
 *  actHashDrbgAddWorkSpaceValues()
 **********************************************************************************************************************/
/*! \brief          Computes (V + H + C + reseedCounter) mod 2^seedlen
 *  \details        Helper function to compute (V + H + C + reseedCount) mod 2^seedlen using big endian byte order.
 *  \param[in,out]  workSpace (in)  pointer to HASHDRBG workspace, storing variable C, hash digest H, constant C and
 *                                  reseedCounter
 *                            (out) HASHDRBG workspace, V modified
 *  \pre            workSpace has to be a valid pointer.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actHashDrbgAddWorkSpaceValues(
  VSECPRIM_P2VAR_PARA(actHASHDRBGSTRUCT) workSpace);

/**********************************************************************************************************************
 *  actHashDrbgDF()
 **********************************************************************************************************************/
/*! \brief          This function implements the derivation function as specified in 10.3.2
 *  \details        -
 *  \param[in,out]  workSpace              (in)  Pointer to the DRBG workspace
 *                                         (out) DRBG workspace, SHA512 workspace member is modified
 *  \param[in]      inputString            Holds the pointer to the input string
 *  \param[in]      numberOfBitsToReturn   number of bits the derivation function shall return, usually 888
 *  \param[in,out]  outputString           (in)  Pointer to the output buffer
 *                                         (out) output buffer
 *  \param[in]      watchdog               Watchdog function pointer
 *  \pre            All pointers have to be valid.
 *                  The buffer referenced by outputBuffer has to provide at least numberOfBitsToReturn
 *                  (= actHASHDRBG_SHA512_SEED_LEN) many bytes.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actHashDrbgDF(
  VSECPRIM_P2VAR_PARA(actHASHDRBGSTRUCT) workSpace,
  VSECPRIM_P2CONST_PARA(actDrbgSeedInputType) inputString,
  actLengthType numberOfBitsToReturn,
  VSECPRIM_P2VAR_PARA(actU8) outputString,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actHashDrbgGeneratePrepareV()
 **********************************************************************************************************************/
/*! \brief         This function updates V for the DRBG generation function.
 *  \details       This function computes V = (V+w) mod 2^seedlen, where w = Hash(0x02 || V || additional_input)
 *  \param[in,out] workSpace                  (in) pointer to the DRBG workspace
 *                                            (out) DRBG workspace, wsSHA512, internalState variable modified
 *  \param[in]     additionalInputPtr         pointer to additional input buffer
 *  \param[in]     additionalInputLength      length of the additional input in bytes
 *  \param[in]     watchdog                   watchdog function pointer
 *  \pre           All pointers have to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actHashDrbgGeneratePrepareV(
  VSECPRIM_P2VAR_PARA(actHASHDRBGSTRUCT) workSpace,
  VSECPRIM_P2CONST_PARA(actU8) additionalInputPtr,
  actLengthType additionalInputLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actHashDrbgHashGen()
 **********************************************************************************************************************/
/*! \brief         This function implements the Hashgen function as specified in 10.1.1.4
 *  \details       -
 *  \param[in,out] workSpace                (in) pointer to the DRBG workspace
 *                                          (out) DRBG workspace, wsSHA512, internalState variable modified
 *  \param[in]     requestedLength          number of random bytes to be returned to the generate function
 *  \param[in,out] outputBuffer             (in)  Pointer to the output buffer
 *                                          (out) output buffer, containing requestedLength random bytes
 *  \param[in]     watchdog                 watchdog function pointer
 *  \pre           All pointers have to be valid.
 *                 The buffer referenced by outputBuffer must provide at least requestedLength bytes.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actHashDrbgHashGen(
  VSECPRIM_P2VAR_PARA(actHASHDRBGSTRUCT) workSpace,
  actLengthType requestedLength,
  VSECPRIM_P2VAR_PARA(actU8) outputBuffer,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actHashDrbgAdd()
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
VSECPRIM_LOCAL_FUNC(void) actHashDrbgAdd(
  VSECPRIM_P2CONST_PARA(actU8) bufferA,
  VSECPRIM_P2CONST_PARA(actU8) bufferB,
  actLengthType bufferBLength,
  VSECPRIM_P2VAR_PARA(actU8) result)
{
  /* ----- Local Variables ------------------------------------------------ */
  actLengthType i;
  actU16 tmp = 0u;

  /* ----- Implementation ------------------------------------------------- */
  for (i = 1; i <= bufferBLength; i++)
  {
    tmp = ((actU16)bufferA[actHASHDRBG_SHA512_SEED_LEN - i]) + bufferB[bufferBLength - i] + actGetU8fromU16(tmp, 2u);
    result[actHASHDRBG_SHA512_SEED_LEN - i] = (actU8)tmp; /* SBSW_VSECPRIM_WRITE_ARRAY */
  }

  for (i = bufferBLength + 1u; i <= actHASHDRBG_SHA512_SEED_LEN; i++)
  {
    tmp = ((actU16)bufferA[actHASHDRBG_SHA512_SEED_LEN - i]) + actGetU8fromU16(tmp, 2u);
    result[actHASHDRBG_SHA512_SEED_LEN - i] = (actU8)tmp; /* SBSW_VSECPRIM_WRITE_ARRAY */
  }
}

/**********************************************************************************************************************
 *  actHashDrbgAddValues()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actHashDrbgAddWorkSpaceValues(
  VSECPRIM_P2VAR_PARA(actHASHDRBGSTRUCT) workSpace)
{
  /* ----- Local Variables ------------------------------------------------ */
  actU8 reseedCounter[actDRBG_SIZEOF_INPUTLENGTH];

  /* ----- Implementation ------------------------------------------------- */
  /* V += H */
  actHashDrbgAdd(workSpace->variable, workSpace->tmpdigest, actHASH_SIZE_SHA512, workSpace->variable); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  /* V += C */
  actHashDrbgAdd(workSpace->variable, workSpace->constant, actHASHDRBG_SHA512_SEED_LEN, workSpace->variable); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  /* V += reseedCounter */
  actU32toBE(reseedCounter, workSpace->reseedCount); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */
  actHashDrbgAdd(workSpace->variable, reseedCounter, actDRBG_SIZEOF_INPUTLENGTH, workSpace->variable); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  actHashDrbgDF()
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
VSECPRIM_LOCAL_FUNC(void) actHashDrbgDF(
  VSECPRIM_P2VAR_PARA(actHASHDRBGSTRUCT) workSpace,
  VSECPRIM_P2CONST_PARA(actDrbgSeedInputType) inputString,
  actLengthType numberOfBitsToReturn,
  VSECPRIM_P2VAR_PARA(actU8) outputString,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Local Variables ------------------------------------------------ */
  actU8 i, counter;
  actLengthType j, len;
  actU8 startHashData[actDRBG_SIZEOF_INPUTLENGTH + 1u];

  /* ----- Implementation ------------------------------------------------- */
  /* 1. temp = the Null string */
  actMemClear(workSpace->DFtmp, (2u * actHASH_SIZE_SHA512)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  /* 2. len = ceil(no_of_bits_to_return / outlen) */
  len = ((numberOfBitsToReturn + actBytesToBits(actHASHDRBG_SHA512_OUTLEN) - 1u) / actBytesToBits(actHASHDRBG_SHA512_OUTLEN));
  /* 3. counter = 0x01 */
  counter = 0x01u;

  actU32toBE(&startHashData[1], numberOfBitsToReturn); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */

  for (j = 0u; j < len; j++)
  {
    startHashData[0] = counter; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */

    /* 4.1 Hash(counter | number of bits to return | input string)  */
    (void)actSHA512Init(&workSpace->wsSHA512); /* SBSW_VSECPRIM_CALL_FUNCTION */
    (void)actSHA512Update(&workSpace->wsSHA512, startHashData, (actDRBG_SIZEOF_INPUTLENGTH + 1u), watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

    for (i = 0u; i < inputString->elements; i++)
    {
      /* actSHA512Update returns actEXCEPTION_LENGTH, if input length is greater than the maximal
        * valid input length of 2^128 bit. Here, it was checked before that the sum of all elements in input string
        * is not longer than 2^32 - 5 byte. */
      (void)actSHA512Update(&workSpace->wsSHA512, inputString->input[i].dataPtr, inputString->input[i].length, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    }

    /* 4.1 temp = temp || Hash(counter||number of bits to return||input string) */
    (void)actSHA512Finalize(&workSpace->wsSHA512, &workSpace->DFtmp[j*actHASH_SIZE_SHA512], watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_SHA_FINALIZE */
    counter += 1u;
  }

  /* 5.  requested_bits = leftmost(temp, no_of_bits_to_return) */
  actMemCpyByteArray(outputString, workSpace->DFtmp, actBitsToBytes(numberOfBitsToReturn)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

} /* PRQA S 6080 */ /* MD_MSR_STMIF */

 /**********************************************************************************************************************
 *  actHashDrbgHashGen()
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
VSECPRIM_LOCAL_FUNC(void) actHashDrbgHashGen(
  VSECPRIM_P2VAR_PARA(actHASHDRBGSTRUCT) workSpace,
  actLengthType requestedLength,
  VSECPRIM_P2VAR_PARA(actU8) outputBuffer,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Local Variables ------------------------------------------------ */
  actLengthType i, m, remainingLength;
  /* 2. data = V */
  VSECPRIM_P2VAR_PARA(actU8) data = workSpace->hashdata;
  VSECPRIM_P2VAR_PARA(actU8) w = workSpace->tmpdigest;

  actMemCpyByteArray(data, workSpace->variable, actHASHDRBG_SHA512_SEED_LEN); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

  /* ----- Implementation ------------------------------------------------- */
  /* 1. m = ceil(requested_no_of_bits/outlen) */
  m = (requestedLength + actHASHDRBG_SHA512_OUTLEN - 1u) / actHASHDRBG_SHA512_OUTLEN;

  /* only proceed, if requestedLength > 0 (in this case also m > 0) */
  if (m > 0u)
  {
    remainingLength = requestedLength;

    /* 4. For i = 1 to m
     *   4.1 w = Hash(data)
     *   4.2 W = W || w (here: W = outputBuffer)
     *   4.3 data = (data+1) mod 2^seedlen */
    for (i = 0u; i < (m - 1u); i++)
    {
      (void)actSHA512Init(&workSpace->wsSHA512); /* SBSW_VSECPRIM_CALL_FUNCTION */

      /* actSHA512Update returns actEXCEPTION_LENGTH, if input length is greater than the maximal
      * valid input length of 2^128 bit. Here, input length is only actHASHDRBG_SHA512_SEED_LEN.  */
      (void)actSHA512Update(&workSpace->wsSHA512, data, actHASHDRBG_SHA512_SEED_LEN, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
      (void)actSHA512Finalize(&workSpace->wsSHA512, &outputBuffer[i*actHASH_SIZE_SHA512], watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_SHA_FINALIZE */
      actDrbgIncrementBlock(data, actHASHDRBG_SHA512_SEED_LEN); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      remainingLength -= actHASH_SIZE_SHA512;
    }

    (void)actSHA512Init(&workSpace->wsSHA512); /* SBSW_VSECPRIM_CALL_FUNCTION */
    (void)actSHA512Update(&workSpace->wsSHA512, data, actHASHDRBG_SHA512_SEED_LEN, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    (void)actSHA512Finalize(&workSpace->wsSHA512, w, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_SHA_FINALIZE */
    actMemCpyByteArray(&outputBuffer[(m - 1u)*actHASH_SIZE_SHA512], w, remainingLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    /* 5. returned_bits = leftmost(W, requested_no_of_bits) */
  }

}

/**********************************************************************************************************************
 *  actHashDrbgGeneratePrepareV()
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
VSECPRIM_LOCAL_FUNC(void) actHashDrbgGeneratePrepareV(
  VSECPRIM_P2VAR_PARA(actHASHDRBGSTRUCT) workSpace,
  VSECPRIM_P2CONST_PARA(actU8) additionalInputPtr,
  actLengthType additionalInputLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{

  /* ----- Local Variables ------------------------------------------------ */
  actDrbgSeedInputType hashInputString;
  actLengthType j;
  actU8 firstElementToHash;

  /* ----- Implementation ------------------------------------------------- */
  /* set up [0x02 || V || additionalInput] in hashInputString */
  firstElementToHash = actHASHDRBG_GENERATE_PREFIX1;

  hashInputString.input[actDRBGSEED_INPUT_PARAM1].dataPtr = &firstElementToHash; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
  hashInputString.input[actDRBGSEED_INPUT_PARAM1].length = 1u; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */

  hashInputString.input[actDRBGSEED_INPUT_PARAM2].dataPtr = workSpace->variable; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
  hashInputString.input[actDRBGSEED_INPUT_PARAM2].length = actHASHDRBG_SHA512_SEED_LEN; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */

  hashInputString.input[actDRBGSEED_INPUT_PARAM3].dataPtr = additionalInputPtr; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
  hashInputString.input[actDRBGSEED_INPUT_PARAM3].length = additionalInputLength; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
  hashInputString.elements = 3u;

  /* 4.1 Hash(counter | number of bits to return | input string)  */
  (void)actSHA512Init(&workSpace->wsSHA512); /* SBSW_VSECPRIM_CALL_FUNCTION */

  for (j = 0u; j < hashInputString.elements; j++)
  {
    /* actSHA512Update returns actEXCEPTION_LENGTH, if input length is greater than the maximal
      * valid input length of 2^128 bit. Here, the maximal input length is 2^35 bit.  */
    (void)actSHA512Update(&workSpace->wsSHA512, hashInputString.input[j].dataPtr, hashInputString.input[j].length, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  /* 2.1 w =  Hash(0x02 || V || additional_input) */
  (void)actSHA512Finalize(&workSpace->wsSHA512, workSpace->tmpdigest, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_SHA_FINALIZE */

  /* 2.2 V = (V+w) mod 2^seedlen */
  actHashDrbgAdd(workSpace->variable, workSpace->tmpdigest, actHASH_SIZE_SHA512, workSpace->variable); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actHashDrbgInstantiation()
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
VSECPRIM_FUNC(void) actHashDrbgInstantiation(
  VSECPRIM_P2VAR_PARA(actHASHDRBGSTRUCT) workSpace,
  VSECPRIM_P2CONST_PARA(actDrbgSeedInputType) seedMaterial,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Local Variables ------------------------------------------------ */
  actDrbgSeedInputType hashDFInputConstant;
  actU8 firstElementToHash = actHASHDRBG_SEEDING_PREFIX1;
  actLengthType seedLen = (actLengthType)actHASHDRBG_SHA512_SEED_LEN;
  actLengthType numberOfBitsToReturn = actBytesToBits(seedLen);

  /* ----- Implementation ------------------------------------------------- */
  /* 2. seed = Hash_df(seed_material, seedlen)
   * 3. V = seed */
  actHashDrbgDF(workSpace, seedMaterial, numberOfBitsToReturn, workSpace->variable, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  /* set up [0x00 || V] in hashDFInputConstant */
  hashDFInputConstant.input[actDRBGSEED_INPUT_PARAM1].dataPtr = &firstElementToHash; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
  hashDFInputConstant.input[actDRBGSEED_INPUT_PARAM1].length = 1u; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */

  hashDFInputConstant.input[actDRBGSEED_INPUT_PARAM2].dataPtr = workSpace->variable; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
  hashDFInputConstant.input[actDRBGSEED_INPUT_PARAM2].length = actHASHDRBG_SHA512_SEED_LEN; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
  hashDFInputConstant.elements = 2u;

  /* 4. C = Hash_df([0x00 || V ], seedlen) */
  actHashDrbgDF(workSpace, &hashDFInputConstant, numberOfBitsToReturn, workSpace->constant, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  /* 5. reseed_counter = 1. */
  workSpace->reseedCount = 1u; /* SBSW_VSECPRIM_WRITE_POINTER */
  /* 6. Return (V,C,reseed_counter) */
}

/**********************************************************************************************************************
 *  actHashDrbgGenerate()
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
VSECPRIM_FUNC(actRETURNCODE) actHashDrbgGenerate(
  VSECPRIM_P2VAR_PARA(actHASHDRBGSTRUCT) workSpace,
  actLengthType requestedLength,
  VSECPRIM_P2VAR_PARA(actU8) outputBuffer,
  VSECPRIM_P2CONST_PARA(actU8) additionalInputPtr,
  actLengthType additionalInputLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Local Variables ------------------------------------------------ */
  actRETURNCODE retVal = actOK;
  actU8 firstValueToHash = actHASHDRBG_GENERATE_PREFIX2;

  /* ----- Implementation ------------------------------------------------- */
  /* 1. If reseed_counter  > reseed_interval, then return an indication that a reseed is required. */
  if (actHASHDRBG_RESEED_INTERVAL > workSpace->reseedCount)
  {
    /* 2. If additional_input != Null, then do */
    if ((additionalInputPtr != NULL_PTR) && (additionalInputLength > 0u))
    {
      /* 2.1 w = Hash(0x02 || V || additional_input)
       * 2.2 V = (V+w) mod 2^seedlen */
      actHashDrbgGeneratePrepareV(workSpace, additionalInputPtr, additionalInputLength, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }

    /* 3. returned_bits = Hashgen(requested_number_of_bits, V) */
    actHashDrbgHashGen(workSpace, requestedLength, outputBuffer, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* 4. H = Hash(0x03 || V) */
    (void)actSHA512Init(&workSpace->wsSHA512); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* actSHA512Update returns actEXCEPTION_LENGTH, if input length is greater than the maximal
      * valid input length of 2^128 bit. Here, input length is only actHASHDRBG_SHA512_SEED_LEN + 1u.  */
    (void)actSHA512Update(&workSpace->wsSHA512, &firstValueToHash, 1u, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    (void)actSHA512Update(&workSpace->wsSHA512, workSpace->variable, actHASHDRBG_SHA512_SEED_LEN, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    (void)actSHA512Finalize(&workSpace->wsSHA512, workSpace->tmpdigest, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_SHA_FINALIZE */

    /* 5. V = (V + H + C + reseedCounter) mod 2^(seedLen) */
    actHashDrbgAddWorkSpaceValues(workSpace); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* 6.  reseed_counter = reseed_counter + 1. */
    workSpace->reseedCount++; /* SBSW_VSECPRIM_WRITE_POINTER */

    /* 7.  Return(SUCCESS, returned_bits, V, C, reseed_counter) */
  }
  else
  {
    /* Reseed counter overflow. */
    workSpace->seedState = actDRBG_SEED_STATE_NEED_RESEED; /* SBSW_VSECPRIM_WRITE_POINTER */
    retVal = actEXCEPTION_ENTROPY_EXHAUSTION;
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTDRBG_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actDRBG.c
 *********************************************************************************************************************/
