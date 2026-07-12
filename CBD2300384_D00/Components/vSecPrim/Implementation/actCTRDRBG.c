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
/*!      \file  actCTRDRBG.c
 *      \brief  CTR DRBG implementation, based on AES128
 *
 *      \details  Implementation for RANDOM Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define ACT_CTRDRBG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actIDRBG.h"
#include "actDRBGCommon.h"
#include "actIAES.h"
#include "actUtilities.h"

#if (VSECPRIM_ACTCTRDRBG_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) tempKey[actCTRDRBG_AES256_KEY_LEN] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */
{
  0x00u, 0x01u, 0x02u, 0x03u, 0x04u, 0x05u, 0x06u, 0x07u, 0x08u, 0x09u, 0x0Au, 0x0Bu, 0x0Cu, 0x0Du, 0x0Eu, 0x0Fu,
  0x10u, 0x11u, 0x12u, 0x13u, 0x14u, 0x15u, 0x16u, 0x17u, 0x18u, 0x19u, 0x1Au, 0x1Bu, 0x1Cu, 0x1Du, 0x1Eu, 0x1Fu
};

# define VSECPRIM_STOP_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actAesCtrDrbgConstructS()
 **********************************************************************************************************************/
/*! \brief          Constructs a block of the string S used in the block cipher derivation function as specified in
 *                  10.3.2
 *  \details        This function concatenates S = L | N | inputString | 0x80, and constructs a block thereof of length
 *                  actAESCTRDRBG_BLOCK_LEN bytes.
 *  \param[in]      inputString        Holds the pointer to the input string
 *  \param[in,out]  outputString       (in)  Pointer to the provided output buffer
 *                                     (out) output block
 *  \param[in]      inputStringLength  sum of length of all elements in inputString
 *  \param[in,out]  nextElement        (in)  Buffer to store the an element index in inputString
 *                                     (out) the element in inputString from which the function shall
 *                                           continue to copy values to outputString when the function is called the
 *                                           next time
 *  \param[in,out]  nextIndex          (in)  Buffer to store the index of a dataPtr in inputString
 *                                     (out) the dataPtr index inputString[nextElement] from which the function shall
 *                                           continue to copy values to outputString when the function is called the
 *                                           next time
 *  \param[in]     numberOfBytesToReturn Number of bytes that shall be returned by the block cihper DF; this value
 *                                       will be stored in the string S; usually equals seed length
 *  \param[in]     isFirstBlock        flag to identify whether the block of S that shall be constructed is the first
 *                                     block, containing L and N.
 *  \pre            All pointers have to be valid.
 *                  The buffer referenced by outputString has to provide at least actAESCTRDRBG_BLOCK_LEN bytes.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_INLINE_FUNC(void) actAesCtrDrbgConstructS(
  VSECPRIM_P2CONST_PARA(actDrbgSeedInputType) inputString,
  VSECPRIM_P2VAR_PARA(actU8) outputString,
  actLengthType inputStringLength,
  VSECPRIM_P2VAR_PARA(actU8) nextElement,
  VSECPRIM_P2VAR_PARA(actLengthType) nextIndex,
  actLengthType numberOfBytesToReturn,
  boolean isFirstBlock);

/**********************************************************************************************************************
 *  actAesCtrDrbgBcc()
 **********************************************************************************************************************/
/*! \brief          This function implements the BCC function as specified in 10.3.3
 *  \details        -
 *  \param[in,out]  workSpace         (in)  Pointer to the workSpace
 *                                    (out) Pointer to the workSpace, AES workSpace (modified)
 *  \param[in]      IV                Pointer to the initialization vector
 *  \param[in]      SLength           Total length of the string S = L || N || input_string || 0x80,
 *                                    a multiple of the block length
 *  \param[in]      totalInputLength  Sum of the length of all elements in inputString
 *  \param[in]      inputString       Holds the pointer to the input string
 *  \param[in,out]  outputString      (in)  Pointer to the provided output buffer
 *                                    (out) output buffer
 *  \param[in]      watchdog          Watchdog function pointer
 *  \pre            workSpace has to be a valid pointer to a variable of type actCTRDRBGSTRUCT.
 *                  inputString has to be a valid pointer.
 *                  IV has to be a valid pointer. The buffer referenced by IV has to provide at least
 *                  actAESCTRDRBG_BLOCK_LEN bytes.
 *                  outputString has to be a valid pointer. The buffer referenced by outputString has to provide at
 *                  least actAESCTRDRBG_BLOCK_LEN bytes.
 *  \note           The parameters inputString and IV are named after the parameters with which the BCC function is
 *                  called during the block cipher derivation function, see 10.3.2.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_INLINE_FUNC(void) actAesCtrDrbgBcc(
  VSECPRIM_P2VAR_PARA(actCTRDRBGSTRUCT) workSpace,
  VSECPRIM_P2CONST_PARA(actU8) IV,
  actLengthType Slength,
  actLengthType totalInputLength,
  VSECPRIM_P2CONST_PARA(actDrbgSeedInputType) inputString,
  VSECPRIM_P2VAR_PARA(actU8) outputString,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actAesCtrDrbgEncryptBlock()
 **********************************************************************************************************************/
/*! \brief          This function will encrypt the actual seed and concatenate the results.
 *  \details        -
 *  \param[in,out]  workSpace     (in)  Pointer to the DRBG workspace
 *                                (out) DRBG workspace, AES workspace member, internal state and variable are modified
 *  \param[in,out]  outputData    (in)  Pointer to the output buffer
 *                                (out) output buffer
 *  \param[in]      dataLength    Size of the output buffer
 *  \param[in]      keyLength     Size of the underlying AES key (128 or 256 bit, depending on workspace mode)
 *  \param[in]      watchdog      watchdog function pointer
 *  \pre            workSpace has to be a valid pointer to a variable of type actCTRDRBGSTRUCT.
 *                  outputData has to be a valid pointer. The buffer referenced by outputData has to provide at least
 *                  dataLength bytes.
 *                  outputData buffer length % ESL_AESCTRDRBG_BLOCK_LEN has to be 0.
 *                  keyLength must be either 16 (CTR DRBG AES128) or 32 (CTR DRBG AES256) bytes.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actAesCtrDrbgEncryptBlock(
  VSECPRIM_P2VAR_PARA(actCTRDRBGSTRUCT) workSpace,
  VSECPRIM_P2VAR_PARA(actU8) outputData,
  actLengthType dataLength,
  actLengthType keyLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actAesCtrDrbgBlockUpdate()
 **********************************************************************************************************************/
/*! \brief          This function implements the update function specified in NIST SP800-90A 10.2.1.2.
 *  \details        -
 *  \param[in,out]  workSpace     (in)  Pointer to the DRBG workspace
 *                                (out) DRBG workspace, internal state and variable are modified
 *  \param[in]      inputData     Pointer to the input data
 *  \param[in]      keyLength     length of underlying AES key (128 oder 256 bit, depending on workspace mode)
 *  \param[in]      seedLength    length of DRBG seed (256 or 384 bit, depending on workspace mode)
 *  \param[in]      watchdog      watchdog function pointer
 *  \pre            workSpace has to be a valid pointer to a variable of type actCTRDRBGSTRUCT.
 *                  inputData has to be a valid pointer. The buffer refereneced by inputData has to provide at least
 *                  actAESCTRDRBG_SEED_LEN bytes.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actAesCtrDrbgBlockUpdate(
  VSECPRIM_P2VAR_PARA(actCTRDRBGSTRUCT) workSpace,
  VSECPRIM_P2CONST_PARA(actU8) inputData,
  actLengthType keyLength,
  actLengthType seedLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actAesCtrDrbgDF()
 **********************************************************************************************************************/
/*! \brief          This function implements the derivation function as specified in 10.3.2
 *  \details        -
 *  \param[in,out]  workSpace           (in)  Pointer to the DRBG workspace
 *                                      (out) DRBG workspace, AES workspace member is modified
 *  \param[in]      inputString         Holds the pointer to the input string
 *  \param[in,out]  outputString        (in)  Pointer to the output buffer
 *                                      (out) output buffer
 *  \param[in]      keyLength           length of underlying AES key (16 or 32 bytes, depending on workspace mode)
 *  \param[in]      seedLength          length of DRBG seed (32 or 48 bytes, depending on workspace mode)
 *  \param[in]      watchdog            Watchdog function pointer
 *  \return         actOK                      successful termination
 *                  actEXCEPTION_INPUT_LENGTH  the sum of the length of the elements in inputString is greater than
 *                                             uint32max
 *  \pre            workSpace has to be a valid pointer to a variable of type actCTRDRBGSTRUCT.
 *                  inputString has to be a valid pointer to a variable of type actDrbgSeedInputType.
 *                  outputString has to be a valid pointer. The buffer referenced by outputBuffer has to provide
 *                  at least actAESCTRDRBG_SEED_LEN bytes.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actAesCtrDrbgDF(
  VSECPRIM_P2VAR_PARA(actCTRDRBGSTRUCT) workSpace,
  VSECPRIM_P2CONST_PARA(actDrbgSeedInputType) inputString,
  VSECPRIM_P2VAR_PARA(actU8) outputString,
  actLengthType keyLength,
  actLengthType seedLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actAesCtrDrbgNoDF()
 **********************************************************************************************************************/
/*! \brief          This function implements the alternative to the derivation function.
 *  \details        -
 *  \param[in]      inputString        Holds the pointer to the input string
 *  \param[in,out]  outputString       (in)  Pointer to the output buffer
 *                                     (out) output buffer
 *  \pre            inputString has to be a valid pointer to a variable of type actDrbgSeedInputType.
 *                  outputString has to be a valid pointer. The buffer referenced by outputString has to provide at
 *                  least actAESCTRDRBG_SEED_LEN bytes.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_INLINE_FUNC(void) actAesCtrDrbgNoDF(
  VSECPRIM_P2CONST_PARA(actDrbgSeedInputType) inputString,
  VSECPRIM_P2VAR_PARA(actU8) outputString);

/**********************************************************************************************************************
 *  actAesCtrDrbgGenerateSetInput()
 *********************************************************************************************************************/
/*! \brief         Prepare Input data for esl_AesCtrDrbgGenerateRn
 *  \details       This function prepares the additional input by either applying the derivation function or
 *                 filling the remaining bytes with zeros and calls actAesCtrDrbgBlockUpdate. If additionalInputPtr
 *                 is a null pointer, additionalInputPrepared will be an all-zero buffer.
 *  \param[in,out] workSpace                 (in)  pointer to DRBG workSpace
 *                                           (out) DRBG workSpace, wsAES, key and variable modified
 *  \param[in]     additionalInputPtr        pointer to additional input buffer, possibly a null pointer
 *  \param[in]     additionalInputLength     length of the additional input in bytes, possibly 0
 *  \param[in]     keyLength                 length of underlying AES key (128 oder 256 bit, depending on workspace
 *                                           mode)
 *  \param[in]     seedLength                length of DRBG seed (256 or 384 bit, depending on workspace mode)
 *  \param[in]     watchdog                  Watchdog function pointer
 *  \return        actOK                      input preparation successful
 *                 actEXCEPTION_INPUT_LENGTH  the sum of the length of the elements in inputString is greater than
 *                                            uint32max
 *  \pre           workSpace has to be a valid pointer.
 *                 If additionalInputPtr is not a null pointer, the buffer referenced by additionalInputPtr has to
 *                 provide at least additionalInputLength bytes.
 *  \context       TASK
 *  \reentrant     TRUE, for different workSpaces
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_INLINE_FUNC(actRETURNCODE) actAesCtrDrbgGenerateSetInput(
  VSECPRIM_P2VAR_PARA(actCTRDRBGSTRUCT) workSpace,
  VSECPRIM_P2CONST_PARA(actU8) additionalInputPtr,
  actLengthType additionalInputLength,
  actLengthType keyLength,
  actLengthType seedLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actAesCtrDrbgConstructS()
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
VSECPRIM_LOCAL_INLINE_FUNC(void) actAesCtrDrbgConstructS(
  VSECPRIM_P2CONST_PARA(actDrbgSeedInputType) inputString,
  VSECPRIM_P2VAR_PARA(actU8) outputString,
  actLengthType inputStringLength,
  VSECPRIM_P2VAR_PARA(actU8) nextElement,
  VSECPRIM_P2VAR_PARA(actLengthType) nextIndex,
  actLengthType numberOfBytesToReturn,
  boolean isFirstBlock)
{
  /* ----- Local Variables ------------------------------------------------ */
  actLengthType j, k, nextIndexLocal;
  actU8 i, nextElementLocal;
  actLengthType offset = 0u;
  actLengthType remainingBytesInBuffer = actCTRDRBG_BLOCK_LEN;

  j = 0u;
  k = *nextIndex;
  nextElementLocal = *nextElement;
  nextIndexLocal = *nextIndex;

  /* ----- Implementation ------------------------------------------------- */
  if (isFirstBlock == TRUE)
  {
    /* 2. L = len (input_string)/8. */
    actU32toBE(&outputString[0u], inputStringLength); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
    /* 3. N = number_of_bits_to_return/8, always set to seedlen */
    actU32toBE(&outputString[actDRBG_SIZEOF_INPUTLENGTH], numberOfBytesToReturn); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */

    offset = actAESCTRDRBG_DF_BUFFER_OFFSET;
    remainingBytesInBuffer = actCTRDRBG_BLOCK_LEN - actAESCTRDRBG_DF_BUFFER_OFFSET;
  }

  for (i = *nextElement; ((i < inputString->elements) && (remainingBytesInBuffer > 0u)); i++)
  {
    while ((k < inputString->input[i].length) && (remainingBytesInBuffer > 0u))
    {
      /* 4. S = L || N || input_string || 0x80. */
      outputString[offset + j] = inputString->input[i].dataPtr[k]; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
      j++;
      k++;
      nextElementLocal = i;
      nextIndexLocal = k;
      remainingBytesInBuffer--;
    }
    k = 0u;
  }

  *nextElement = nextElementLocal; /* SBSW_VSECPRIM_WRITE_POINTER */
  *nextIndex = nextIndexLocal; /* SBSW_VSECPRIM_WRITE_POINTER */

  /* When all value of inputString are processed, check whether 0x80u can be appended to outputString. */
  if (((nextElementLocal == (inputString->elements - 1u)) &&
    (nextIndexLocal == inputString->input[nextElementLocal].length) &&
    (remainingBytesInBuffer > 0u)))
  {
    /* 4. S = L || N || input_string || 0x80. */
    /* -> S = - || - || - || 0x80 */
    outputString[offset + j] = 0x80u; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */

    if ((remainingBytesInBuffer - 1u) > 0u)
    {
      /* 5. While len(S) mod outlen != 0, do S = S || 0x00  */
      actMemClear(&outputString[offset + j + 1u], remainingBytesInBuffer - 1u); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
  }

} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actAesCtrDrbgBcc()
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
VSECPRIM_LOCAL_INLINE_FUNC(void) actAesCtrDrbgBcc(
  VSECPRIM_P2VAR_PARA(actCTRDRBGSTRUCT) workSpace,
  VSECPRIM_P2CONST_PARA(actU8) IV,
  actLengthType Slength,
  actLengthType totalInputLength,
  VSECPRIM_P2CONST_PARA(actDrbgSeedInputType) inputString,
  VSECPRIM_P2VAR_PARA(actU8) outputString,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Local Variables ------------------------------------------------ */
  actLengthType i, j;
  actLengthType numberOfBytesToReturn, localKeyLength;
  VSECPRIM_P2VAR_PARA(actU8) blockOfS = workSpace->tempBlocks.blockOfS;    /* < Buffer for the S construct */

  VSECPRIM_P2VAR_PARA(actU8) tempResult = workSpace->BCCUpdateTmpBuffer.BCCTmpBuffer.result;
  VSECPRIM_P2VAR_PARA(actU8) tempInput = workSpace->BCCUpdateTmpBuffer.BCCTmpBuffer.input;

  /* indices used while constructing S block-wisely */
  actU8 nextInputElement = 0u;
  actLengthType nextIndexInInputElement = 0u;

  if ((workSpace->mode & actDRBG_ALGSIZE_MASK) == actCTRDRBG_ALGSIZE_AES128)
  {
    numberOfBytesToReturn = actAES128CTRDRBG_SEED_LEN;
    localKeyLength = actAES128_KEY_SIZE;
  }
  else
  {
    /* In this case, (workSpace->mode & actDRBG_ALGSIZE_MASK) = actCTRDRBG_ALGSIZE_AES256, since the
     * validity of the size was checked before */
    numberOfBytesToReturn = actAES256CTRDRBG_SEED_LEN;
    localKeyLength = actAES256_KEY_SIZE;
  }

  /* ----- Implementation ------------------------------------------------- */
  /* Load key; Since tempKey is a local buffer of length 32 and localKeyLength is either 16 or 32,
   * actAESInitEncryptBlock returns actOK. */
  (void)actAESInitEncryptBlock(&workSpace->wsAES, tempKey, localKeyLength, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  /* 1. chaining_value = 0^outlen */
  /* Encrypt the IV */
  actAESEncryptBlock(&workSpace->wsAES, IV, tempResult, watchdog);  /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */

  actMemClear(blockOfS, actCTRDRBG_BLOCK_LEN); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

  /* 2. n = len (data)/outlen. */
  /* 4. For i = 1 to n do  */
  for (i = 0u; i < Slength; i += actCTRDRBG_BLOCK_LEN)
  {
    if (i == 0u)
    {
      actAesCtrDrbgConstructS(inputString, blockOfS, totalInputLength, &nextInputElement, &nextIndexInInputElement, numberOfBytesToReturn, TRUE); /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */
    }
    else
    {
      actAesCtrDrbgConstructS(inputString, blockOfS, totalInputLength, &nextInputElement, &nextIndexInInputElement, numberOfBytesToReturn, FALSE); /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */
    }

    /* 4.1  input_block = chaining_value ^ block_i */
    for (j = 0u; j < actCTRDRBG_BLOCK_LEN; j++)
    {
      tempInput[j] = tempResult[j] ^ blockOfS[j]; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    }

    /* 4.2 chaining_value = Block_Encrypt (Key, input_block) */
    actAESEncryptBlock(&workSpace->wsAES, tempInput, tempResult, watchdog); /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */
  }

  /* 5. output_block = chaining_value. */
  /* 6. Return output_block */
  actMemCpyByteArray(outputString, tempResult, actCTRDRBG_BLOCK_LEN); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actAesCtrDrbgEncryptBlock()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actAesCtrDrbgEncryptBlock(
  VSECPRIM_P2VAR_PARA(actCTRDRBGSTRUCT) workSpace,
  VSECPRIM_P2VAR_PARA(actU8) outputData,
  actLengthType dataLength,
  actLengthType keyLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 i = 0u;

  /* ----- Implementation ------------------------------------------------- */
  /* Since workSpace->key is not a null pointer and localKeyLength is a valid key size value,
   * actAESInitEncryptBlock returns actOK. */
  (void)actAESInitEncryptBlock(&workSpace->wsAES, workSpace->key, keyLength, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  /* 2. while (len(tempBuffer) < 256) do */
  if (dataLength >= actCTRDRBG_BLOCK_LEN)
  {
    for (i = 0u; i <= (dataLength - actCTRDRBG_BLOCK_LEN); i += actCTRDRBG_BLOCK_LEN)
    {
      /* 2.1 V = (V + 1) mod 2^128 */
      actDrbgIncrementBlock(workSpace->variable, actCTRDRBG_BLOCK_LEN); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

      /* 2.2 output_block = AES_ECB_Encrypt(Key, V) */
      /* 2.3 tempBuffer = tempBuffer || output_block */
      actAESEncryptBlock(&workSpace->wsAES, workSpace->variable, &outputData[i], watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
  }

  /* if last block is not full AES 128 block */
  if (i < dataLength)
  {
    /* 2.1 V = (V + 1) mod 2^128 */
    actDrbgIncrementBlock(workSpace->variable, actCTRDRBG_BLOCK_LEN); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

    /* 2.2 output_block = AES_ECB_Encrypt(Key, V) */
    /* 2.3 tempBuffer = tempBuffer || output_block */
    actAESEncryptBlock(&workSpace->wsAES, workSpace->variable, workSpace->tempBlocks.tempAesBlock, watchdog); /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */
    actMemCpyByteArray(&outputData[i], workSpace->tempBlocks.tempAesBlock, (actLengthType)(dataLength - i)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }
}

/**********************************************************************************************************************
 *  actAesCtrDrbgBlockUpdate()
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
VSECPRIM_LOCAL_FUNC(void) actAesCtrDrbgBlockUpdate(
  VSECPRIM_P2VAR_PARA(actCTRDRBGSTRUCT) workSpace,
  VSECPRIM_P2CONST_PARA(actU8) inputData,
  actLengthType keyLength,
  actLengthType seedLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Local Variables ------------------------------------------------ */
  actLengthType i;
  VSECPRIM_P2VAR_PARA(actU8) tempBuffer = workSpace->BCCUpdateTmpBuffer.blockUpdateTmpBuffer;

  /* ----- Implementation ------------------------------------------------- */
  /* 1. tempBuffer = Null */
  /* -> Initialization is not necessary hence buffer will be written in 2.3 */

  /* 2. while (len(tempBuffer) < 256) do */
  /* 2.1 V = (V + 1) mod 2^128 */
  /* 2.2 output_block = AES_ECB_Encrypt(Key, V) */
  /* 2.3 tempBuffer = tempBuffer || output_block */
  actAesCtrDrbgEncryptBlock(workSpace, tempBuffer, seedLength, keyLength, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  /* 3. tempBuffer = Leftmost 256 bits of tempBuffer */
  for (i = 0u; i < seedLength; i++)
  {
    /* 4. tempBuffer = tempBuffer xor provided_data */
    tempBuffer[i] ^= inputData[i]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  }

  /* 5. key = Leftmost 128 bits of tempBuffer */
  actMemCpyByteArray(workSpace->key, &tempBuffer[0], keyLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  /* 6. V = Rightmost 128 bits of tempBuffer */
  /* 7. Return (key, v) -> key and seedState are provided in the workSpace buffer; */
  actMemCpyByteArray(workSpace->variable, &tempBuffer[keyLength], actCTRDRBG_BLOCK_LEN); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

}

/**********************************************************************************************************************
 *  actAesCtrDrbgDF()
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
 *
 */
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actAesCtrDrbgDF(
  VSECPRIM_P2VAR_PARA(actCTRDRBGSTRUCT) workSpace,
  VSECPRIM_P2CONST_PARA(actDrbgSeedInputType) inputString,
  VSECPRIM_P2VAR_PARA(actU8) outputString,
  actLengthType keyLength,
  actLengthType seedLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Local Variables ------------------------------------------------ */
  actRETURNCODE retVal = actOK;

  actLengthType j, totalInputLength, totalInputLengthControlValue;
  actLengthType bccInputLength;                                               /* < Actual buffer size used */

  VSECPRIM_P2VAR_PARA(actU8) IV = workSpace->DFIV;                            /* < Initialization vector */
  VSECPRIM_P2VAR_PARA(actU8) encryptBuffer = workSpace->DFencryptBuffer;      /* < Buffer for the encryption */

  /* 7. i = 0 */
  actU32 i = 0u;

  /* ----- Implementation ------------------------------------------------- */
  /* 9.1 IV = i || 0^outlen - len (i) */
  actMemClear(IV, actCTRDRBG_BLOCK_LEN); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

  /* Nullify encryptBuffer */
  actMemClear(encryptBuffer, actAESCTRDRBG_SEED_LEN); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

  /* 4. S = L || N || input_string || 0x80.
   * Loop over the input elements to find total length L of inputString, must be a multiple of block length */
  totalInputLength = 0u;
  for (j = 0u; j < inputString->elements; j++)
  {
    totalInputLengthControlValue = totalInputLength;
    totalInputLength += inputString->input[j].length;

    /* Overflow check: The sum of the length values of the elements in inputString must not be greater
     * then uint32max. */
    if (totalInputLength < totalInputLengthControlValue)
    {
      retVal = actEXCEPTION_INPUT_LENGTH;
    }
  }

  /* Overflow check: S = L || N || inputString || 0x80, len(S) = 8 + totalInputLength + 1 */
  if ((totalInputLength + (actLengthType)(actAESCTRDRBG_DF_BUFFER_OFFSET) + 1u) < totalInputLength)
  {
    retVal = actEXCEPTION_INPUT_LENGTH;
  }

  if (retVal == actOK)
  {
    bccInputLength = ((((actAESCTRDRBG_DF_BUFFER_OFFSET + totalInputLength) / actCTRDRBG_BLOCK_LEN) + 1u) * actCTRDRBG_BLOCK_LEN);

    /* 9. While len (tempBuffer) < keylen + outlen, do */
    for (j = 0u; j < seedLength; j += actCTRDRBG_BLOCK_LEN)
    {
      /* 9.2 tempBuffer = tempBuffer || BCC(K, (IV||S)) */
      actAesCtrDrbgBcc(workSpace, IV, bccInputLength, totalInputLength, inputString, &encryptBuffer[j], watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* 9.3 i = i + 1 */
      i++;
      actU32toBE(IV, i); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }

    /* 10. K = Leftmost keylen bits of tempBuffer; Since encryptBuffer is a local buffer and actAESCTRDRBG_MAX_KEY_LEN
    * is a valid key size value, actAESInitEncryptBlock returns actOK. */
    (void)actAESInitEncryptBlock(&workSpace->wsAES, encryptBuffer, keyLength, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* 13. while len(tempBuffer) < number_of_bits_to_return, do */
    for (j = 0u; j <= (seedLength - actCTRDRBG_BLOCK_LEN); j += actCTRDRBG_BLOCK_LEN)
    {
      /* 13.1 X = Block_Encrypt(K, X) */
      /* 13.2 tempBuffer = tempBuffer||X */
      /* 11. X = Next outlen bits of tempBuffer */
      actAESEncryptBlock(&workSpace->wsAES, &encryptBuffer[keyLength], &encryptBuffer[keyLength], watchdog); /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */

      /* 14. requested_bits = Leftmost number_of_bits_to_return of tempBuffer */
      /* 15. return Success and requested_bits */
      actMemCpyByteArray(&outputString[j], &encryptBuffer[keyLength], actCTRDRBG_BLOCK_LEN); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    }
  }

  return retVal;
} /* PRQA S 6060, 6080 */ /* MD_VSECPRIM_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  actAesCtrDrbgNoDF()
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
VSECPRIM_LOCAL_INLINE_FUNC(void) actAesCtrDrbgNoDF(
  VSECPRIM_P2CONST_PARA(actDrbgSeedInputType) inputString,
  VSECPRIM_P2VAR_PARA(actU8) outputString)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 i;
  VSECPRIM_P2CONST_PARA(actU8) entropyPtr;
  VSECPRIM_P2CONST_PARA(actU8) personalStrPtr;
  actLengthType entropyLength, personalStrLength;

  entropyPtr = inputString->input[0u].dataPtr;
  entropyLength = inputString->input[0u].length;

  /* ----- Implementation ------------------------------------------------- */
  /* seed_material = entropy_input xor personalization_string */

    /* Copy seed, the entropy Length is fix actAESCTRDRBG_SEED_LEN 10.2 table 3: entropyLen = seedLen
     * The seedLen depends on the underlying AES key size. */
  actMemCpyByteArray(outputString, entropyPtr, entropyLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

  /* Personal string handling */
  if (1u < inputString->elements)
  {
    personalStrPtr = inputString->input[1u].dataPtr;
    personalStrLength = inputString->input[1u].length;

    for (i = 0u; i < personalStrLength; i++)
    {
      outputString[i] ^= personalStrPtr[i]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
    }
  }
}

/**********************************************************************************************************************
 *  actAesCtrDrbgGenerateSetInput()
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
VSECPRIM_LOCAL_INLINE_FUNC(actRETURNCODE) actAesCtrDrbgGenerateSetInput(
  VSECPRIM_P2VAR_PARA(actCTRDRBGSTRUCT) workSpace,
  VSECPRIM_P2CONST_PARA(actU8) additionalInputPtr,
  actLengthType additionalInputLength,
  actLengthType keyLength,
  actLengthType seedLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Local Variables ------------------------------------------------ */
  actRETURNCODE retVal = actOK;
  actDrbgSeedInputType seedInput;
  VSECPRIM_P2VAR_PARA(actU8) additionalInputPrepared = workSpace->tmpBuffer.additionalInputPrepared;

  /* ----- Implementation ------------------------------------------------- */
  /* 2. If (additional_input != Null), then. */
  if ((additionalInputPtr != NULL_PTR) && (additionalInputLength > 0u))
  {
    if ((actDRBG_DFMODE_MASK & workSpace->mode) == actCTRDRBG_DFMODE_USE_BLOCKCIPHER_DF)
    {
      seedInput.input[actDRBGSEED_INPUT_PARAM1].dataPtr = additionalInputPtr; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
      seedInput.input[actDRBGSEED_INPUT_PARAM1].length = additionalInputLength; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
      seedInput.elements = 1u;

      /* 2.1 additional_input = Block_Cipher_df (additional_input, seedlen). */
      retVal = actAesCtrDrbgDF(workSpace, &seedInput, additionalInputPrepared, keyLength, seedLength, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
    else  /* workSpace->DFmode == actCTRDRBG_USE_NO_DF */
    {
      /* 2.1 additional_input = additional_input || 0^(seedlen-len(additional_input)) */
      actMemCpyByteArray(additionalInputPrepared, additionalInputPtr, additionalInputLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      actMemClear(&additionalInputPrepared[additionalInputLength], ((actLengthType)(actAESCTRDRBG_SEED_LEN)-additionalInputLength));  /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }

    if (retVal == actOK)
    {
      /* 2.2 (Key, V) = CTR_DRBG_Update (additional_input, Key, V). */
      actAesCtrDrbgBlockUpdate(workSpace, additionalInputPrepared, keyLength, seedLength, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
  }
  else
  {
    /* No additionalInput -> fill the buffer with zeros for step 6. */
    actMemClear(additionalInputPrepared, actAESCTRDRBG_SEED_LEN); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actAesCtrDrbgInstantiation()
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
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actAesCtrDrbgInstantiation(
  VSECPRIM_P2VAR_PARA(actCTRDRBGSTRUCT) workSpace,
  VSECPRIM_P2CONST_PARA(actDrbgSeedInputType) dataInput,
  boolean reseedFlag,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Local Variables ------------------------------------------------ */
  actRETURNCODE retVal = actOK;
  /* Seedlen and KeyLen, derived from mode, which is stored in workSpace */
  actLengthType localSeedLength, localKeyLength;
  VSECPRIM_P2VAR_PARA(actU8) seedData = workSpace->tmpBuffer.seedData;

  if ((workSpace->mode & actDRBG_ALGSIZE_MASK) == actCTRDRBG_ALGSIZE_AES128)
  {
    localSeedLength = actAES128CTRDRBG_SEED_LEN;
    localKeyLength = actAES128_KEY_SIZE;
  }
  /* in this case ((workSpace->mode & actDRBG_ALGSIZE_MASK) = actCTRDRBG_ALGSIZE_AES256),
   * since the validity of the size was checked before */
  else
  {
    localSeedLength = actAES256CTRDRBG_SEED_LEN;
    localKeyLength = actAES256_KEY_SIZE;
  }

  /* ----- Implementation ------------------------------------------------- */
  if ((actDRBG_DFMODE_MASK & workSpace->mode) == actCTRDRBG_DFMODE_USE_BLOCKCIPHER_DF)
  {
    /* 1. seed_material = entropy_input || nonce || personalization_string. */
    /* 2. seed_material = Block_Cipher_df (seed_material, seedlen). */
    retVal = actAesCtrDrbgDF(workSpace, dataInput, seedData, localKeyLength, localSeedLength, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }
  else /* (workSpace->DFmode == actCTRDRBG_USE_NO_DF) */
  {
    /* seed_material = entropy_input || personalization_string. */
    actAesCtrDrbgNoDF(dataInput, seedData); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (retVal == actOK)
  {
    if (reseedFlag != TRUE)
    {
      /* 3. Key = 0^keylen */
      actMemClear(workSpace->key, actAESCTRDRBG_MAX_KEY_LEN); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

      /* 4. V= = 0^outlen */
      actMemClear(workSpace->variable, actCTRDRBG_BLOCK_LEN); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    }

    /* 5. (Key, V) = CTR_DRBG_Update (seed_material, Key, V). */
    actAesCtrDrbgBlockUpdate(workSpace, seedData, localKeyLength, localSeedLength, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* 6. reseed_counter = 1. */
    workSpace->reseedCount = 1u; /* SBSW_VSECPRIM_WRITE_POINTER */
  }

  /* 7. Return V, Key, and reseed_counter as the initial_working_state. */
  return retVal;
}

/**********************************************************************************************************************
 *  actAesCtrDrbgGenerate()
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
VSECPRIM_FUNC(actRETURNCODE) actAesCtrDrbgGenerate(
  VSECPRIM_P2VAR_PARA(actCTRDRBGSTRUCT) workSpace,
  actLengthType requestedLength,
  VSECPRIM_P2VAR_PARA(actU8) outputBuffer,
  VSECPRIM_P2CONST_PARA(actU8) additionalInputPtr,
  actLengthType additionalInputLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Local Variables ------------------------------------------------ */
  actRETURNCODE retVal;
  /* Seedlen and KeyLen, derived from mode, which is stored in workSpace */
  actLengthType localSeedLength, localKeyLength;

  if ((workSpace->mode & actDRBG_ALGSIZE_MASK) == actCTRDRBG_ALGSIZE_AES128)
  {
    localSeedLength = actAES128CTRDRBG_SEED_LEN;
    localKeyLength = actAES128_KEY_SIZE;
  }
  /* in this case ((workSpace->mode & actDRBG_ALGSIZE_MASK) = actCTRDRBG_ALGSIZE_AES256),
   * since the validity of the size was checked before */
  else
  {
    localSeedLength = actAES256CTRDRBG_SEED_LEN;
    localKeyLength = actAES256_KEY_SIZE;
  }

  /* ----- Implementation ------------------------------------------------- */
  /* 1. If reseed_counter  > reseed_interval, then return an indication that a reseed is required. */
  if (actCTRDRBG_AES128_RESEED_INTERVAL > workSpace->reseedCount)
  {
    retVal = actAesCtrDrbgGenerateSetInput(workSpace, additionalInputPtr, additionalInputLength, localKeyLength, localSeedLength, watchdog); /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (retVal == actOK)
    {
      /* 3. tempBuffer = Null. */
      /* no tempBuffer buffer used to reduce ram usage / to avoid max buffer calculation */
      actMemClear(outputBuffer, requestedLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

      /* 4.  While (len (tempBuffer) < requested_number_of_bits) do: */
      /* 4.1 V = (V + 1) mod 2^outlen */
      /* 4.2 output_block = Block_Encrypt (Key, V). */
      /* 4.3 tempBuffer = tempBuffer || output_block. */
      actAesCtrDrbgEncryptBlock(workSpace, outputBuffer, requestedLength, localKeyLength, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* 5. returned_bits = Leftmost requested_number_of_bits of tempBuffer. */
      /* not required since outputdata is written directly to the outputBuffer */

      /* 6. (Key, V) = CTR_DRBG_Update (additional_input, Key, V). */
      actAesCtrDrbgBlockUpdate(workSpace, workSpace->tmpBuffer.additionalInputPrepared, localKeyLength, localSeedLength, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* 7.  reseed_counter = reseed_counter + 1. */
      workSpace->reseedCount++; /* SBSW_VSECPRIM_WRITE_POINTER */
      /* 8.  Return(SUCCESS, returned_bits, Key, V, reseed_counter).
      * key, V and reseed counter are returned via the workspace */
    }
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

#endif /* (VSECPRIM_ACTCTRDRBG_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actDRBG.c
 *********************************************************************************************************************/
