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
/*!        \file  actAESCCM.c
 *        \brief  AES128CCM implementation.
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library actCLib
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

#define ACTIAEADAES128CCM_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actIAESCCM.h"
#include "actUtilities.h"
#include "actWatchdog.h"

#if (VSECPRIM_ACTCCM_ENABLED == STD_ON)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actAESCCMEncodeAadLen()
 *********************************************************************************************************************/
/*! \brief       Encode length of AAD.
 *  \details     If aad len < 2^16 - 2^8, the encoding is stored in the first 2 bytes of outBlock.
                 If 2^16 - 2^8 < aad len < 2^32, the encoding is stored in the first 4 bytes of outBlock.
 *  \param[in]     info             Pointer to AES CCM context structure
 *  \param[in,out] outBlock         (in)  Pointer to block in which the encoded aad length shall be stored
 *                                  (out) block containing the encoded aad length in the first bytes
 *  \param[in,out] encodingOffset   (in)  Pointer to buffer to store the number of bytes the encoding takes
 *                                  (out) Pointer to buffer storing the number of bytes the encoding takes (2 or 6)
 *  \pre         outBuffer must point to a buffer with at least 6 bytes.
 *               info must be a valid workspace pointer
 *               encodingOffset must be a valid pointer
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actAESCCMEncodeAadLen(
  VSECPRIM_P2CONST_PARA(actAESCCMSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) outBlock,
  VSECPRIM_P2VAR_PARA(actU8) encodingOffset);

/**********************************************************************************************************************
 *  actAESCCMConstructBlockB0()
 *********************************************************************************************************************/
/*! \brief       Constructs block B0, i.e., the first block that is used during the computation of the authentication
 *               tag.
 *  \details     Note: The construction of this block is independent from the aad or the message.
 *  \param[in,out] info             (in)  pointer to AES CCM context structure
 *                                  (out) pointer to AES CCM context structure, member dataBlockOut modified
 *  \param[in,out] blockB0          (in)  Pointer to buffer to store the unencrypted block B0
 *                                  (out) Pointer to buffer storing the unencrypted block B0
 *  \param[in,out] encBlockB0       (in)  Pointer to buffer to store the AES128 - encrypted block B0, i.e, block X1
 *                                  (out) Pointer to buffer storing the AES128 - encrypted block B0, i.e, block X1
 *  \param[in]     watchdog          Pointer to watchdog trigger function
 *  \pre         info must be a valid workspace pointer
 *               The buffer referenced by blockB0 must provide at least 16 (= actAES_BLOCK_SIZE) many bytes.
 *               The buffer referenced by encBlockB0 must provide at least 16 (= actAES_BLOCK_SIZE) many bytes.
 *  \note        encBlockB0 is required as a separate parameter because currently in-place encryption with AES is
 *               not supported.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actAESCCMConstructBlockB0(
  VSECPRIM_P2CONST_PARA(actAESCCMSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) blockB0,
  VSECPRIM_P2VAR_PARA(actU8) encBlockB0,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actAES128CCMConstructBlock()
 *********************************************************************************************************************/
/*! \brief       Constructs a block Bi, i.e., a block that is used during the computation of the authentication tag.
 *  \details     This function takes at most actAES_BLOCK_SIZE many bytes from data and copies them to blockOut.
 *               If necessary, remaining bytes in blockOut are filled with zero.
 *  \param[in,out] info             (in)  Pointer to AES CCM context structure
 *                                  (out) Pointer to AES CCM context structure, member dataBlockB modified
 *  \param[in]     data             pointer to data (aad or msg) to be processed block-wisely
 *  \param[in]     dataLen          total length of data (aadLen or msgLen)
 *  \param[in,out] blockOut         (in)  Pointer to buffer to store the (unencrypted) block Bi
 *                                  (out) Pointer to buffer storing the (unencrypted) block Bi
 *  \param[in,out] indexInData      (in)  index at which the copy operation from data in this function call shall start
 *                                  (out) index at which the copy operation from data in the next function call shall
 *                                        start
 *  \param[in]     isFirstBlock     flag indicating whether the block to be constructed shall be block B1 or a later
 *                                  block; B1 needs to be treated differently because it contains the aad length
 *                                  encoding
 *  \param[in]     encodedLengthDataLen  number of bytes the aad length encoding takes (2 or 6)
 *  \pre         All pointers must be valid.
 *               The buffer referenced by blockOut must provide at least 16 (= actAES_BLOCK_SIZE) many bytes.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actAES128CCMConstructBlock(
  VSECPRIM_P2CONST_PARA(actAESCCMSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) data,
  const actLengthType dataLen,
  VSECPRIM_P2VAR_PARA(actU8) blockOut,
  VSECPRIM_P2VAR_PARA(actLengthType) indexInData,
  boolean isFirstBlock,
  const actLengthType encodedLengthDataLen);

/**********************************************************************************************************************
 *  actAESCCMIncrementCounter()
 *********************************************************************************************************************/
/*! \brief          Increments the value of block.
 *  \details        Helper function to increment the value of block B: B = (B + 1) mod 2^block_len using
 *                  big endian byte order. The last bytes of block store a counter required for encrypting the message.
 *  \param[in,out]  block        (in)  Holds the block whose counter is to be increased
 *                               (out) increased block
 *  \param[in]      blockLength  length of the block to be increased in bytes
 *  \pre            block has to be a valid pointer.
 *                  The buffer referenced by block has to provide at least blockLength bytes.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actAESCCMIncrementCounter(
  VSECPRIM_P2VAR_PARA(actU8) block,
  const actLengthType blockLength);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actAESCCMEncodeAadLen()
 *********************************************************************************************************************/
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
VSECPRIM_LOCAL_FUNC(void) actAESCCMEncodeAadLen(
  VSECPRIM_P2CONST_PARA(actAESCCMSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) outBlock,
  VSECPRIM_P2VAR_PARA(actU8) encodingOffset)
{
  if (info->aadLen > 0u)
  {
    if (info->aadLen < 0xFF00u)
    {
      outBlock[0] = actGetU8fromU32(info->aadLen, 1u); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
      outBlock[1] = (actU8)(info->aadLen); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
      *encodingOffset = 2; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    else
    {
      outBlock[0] = 0xFFu; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
      outBlock[1] = 0xFEu; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
      actU32toBE(&outBlock[2], info->aadLen); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS */
      *encodingOffset = 6; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }
  else
  {
    *encodingOffset = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
  }
}

/**********************************************************************************************************************
 *  actAESCCMConstructBlockB0()
 *********************************************************************************************************************/
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
VSECPRIM_LOCAL_FUNC(void) actAESCCMConstructBlockB0(
  VSECPRIM_P2CONST_PARA(actAESCCMSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) blockB0,
  VSECPRIM_P2VAR_PARA(actU8) encBlockB0,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actU8 lengthBuffer[4];
  actLengthType nonceLength = 15u - (actLengthType)info->lengthFieldSize;
  actLengthType maxIndex = actMax((actAES_BLOCK_SIZE - (actLengthType)info->lengthFieldSize), (actAES_BLOCK_SIZE - 4u));

  actMemClear(blockB0, actAES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

  /* 1: Construct block B_0 = [flag | nonce | msgLen] */
  blockB0[0] = ((actU8)8u * ((info->authFieldSize - (actU8)2u) / (actU8)2u)) + info->lengthFieldSize - (actU8)1u; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */

  if (info->aadLen > 0u)
  {
    blockB0[0] += 64u; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  }

  /* 2: Copy nonce to block B_0, starting from second byte */
  actMemCpyByteArray(&blockB0[1], info->nonce, nonceLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS */

  /* 3: Fill block B_0 with message length, using info->lengthFieldSize many bytes */
  actU32toBE(lengthBuffer, info->msgLen); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */
  actMemCpyByteArray(&blockB0[maxIndex], &lengthBuffer[4u - (actAES_BLOCK_SIZE - maxIndex)], actAES_BLOCK_SIZE - maxIndex); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SUBADDRESS_VARIABLE_SIZE */

  /* 4: encrypt block B_0 */
  actAESEncryptBlock(&info->wsAES, blockB0, encBlockB0, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  actAES128CCMConstructBlock()
 *********************************************************************************************************************/
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
VSECPRIM_LOCAL_FUNC(void) actAES128CCMConstructBlock(
  VSECPRIM_P2CONST_PARA(actAESCCMSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) data,
  const actLengthType dataLen,
  VSECPRIM_P2VAR_PARA(actU8) blockOut,
  VSECPRIM_P2VAR_PARA(actLengthType) indexInData,
  boolean isFirstBlock,
  const actLengthType encodedLengthDataLen)
{
  const actU8 * dataWord = data;

  if (isFirstBlock == TRUE)
  {
    /* dataBlockIn contains the encoded length of data */
    actMemCpyByteArray(blockOut, info->dataBlockIn, encodedLengthDataLen); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* start copying data */
    if (dataLen >= (actAES_BLOCK_SIZE - encodedLengthDataLen))
    {
      actMemCpyByteArray(&blockOut[encodedLengthDataLen], dataWord, actAES_BLOCK_SIZE - encodedLengthDataLen); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SUBADDRESS */
      *indexInData = actAES_BLOCK_SIZE - encodedLengthDataLen; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    else
    {
      actMemCpyByteArray(&blockOut[encodedLengthDataLen], dataWord, dataLen); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SUBADDRESS */
      actMemClear(&blockOut[encodedLengthDataLen + dataLen], actAES_BLOCK_SIZE - encodedLengthDataLen - dataLen); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SUBADDRESS */
    }
  }
  else
  {
    if ((*indexInData + actAES_BLOCK_SIZE) < dataLen)
    {
      /* can fill remaining bytes in block with bytes from data */
      actMemCpyByteArray(blockOut, &dataWord[*indexInData], actAES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      *indexInData += actAES_BLOCK_SIZE; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    else
    {
      /* remaining bytes in data do not form a complete block - fill up with padding bytes */
      actMemCpyByteArray(blockOut, &dataWord[*indexInData], dataLen - *indexInData); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */
      actMemClear(&blockOut[dataLen - *indexInData], actAES_BLOCK_SIZE - dataLen + *indexInData); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SUBADDRESS_VARIABLE_SIZE */
      *indexInData = dataLen - 1u; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actAESCCMIncrementCounter()
 *********************************************************************************************************************/
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
VSECPRIM_LOCAL_FUNC(void) actAESCCMIncrementCounter(
  VSECPRIM_P2VAR_PARA(actU8) block,
  const actLengthType blockLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 i = blockLength;
  actU8 result;

  /* ----- Implementation ------------------------------------------------- */
  while (i != 0u)
  {
    i--;
    /* Increment block element and store result */
    ++block[i];  /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    result = block[i];

    /* Check for overflow */
    if (result != 0u)
    {
      /* No overflow happens - stop incrementing */
      break;
    }
  }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actAESCCMInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actAESCCMInit(
  VSECPRIM_P2VAR_PARA(actAESCCMSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) key,
  const actLengthType keyLength,
  VSECPRIM_P2CONST_PARA(actU8) nonce,
  const actLengthType nonceLength,
  const actLengthType aadLength,
  const actLengthType msgLength,
  const actU8 authenticationFieldSize,
  const actU8 lengthFieldSize,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* actAESInitEncryptBlock does not return actOK, if key is a null pointer or if keyLength is invalid.
   * This was checked before. */
  (void)actAESInitEncryptBlock(&info->wsAES, key, keyLength, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  actMemClear(info->AESCounter, actAES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

  actMemCpyByteArray(info->nonce, nonce, nonceLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */
  actMemClear(&info->nonce[nonceLength], actAES_BLOCK_SIZE - 1u - nonceLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SUBADDRESS_VARIABLE_SIZE */

  info->aadLen = aadLength; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->msgLen = msgLength; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->authFieldSize = authenticationFieldSize; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->lengthFieldSize = lengthFieldSize; /* SBSW_VSECPRIM_WRITE_POINTER */

  info->updateState = actAESCCM_UPDATE_NOT_CALLED; /* SBSW_VSECPRIM_WRITE_POINTER */

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actAESCCMUpdateEncrypt()
 *********************************************************************************************************************/
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
VSECPRIM_FUNC(actRETURNCODE) actAESCCMUpdateEncrypt(
  VSECPRIM_P2VAR_PARA(actAESCCMSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) aad,
  const actLengthType aadLength,
  VSECPRIM_P2CONST_PARA(actU8) msg,
  const actLengthType msgLength,
  VSECPRIM_P2VAR_PARA(actU8) out,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal = actOK;

  actU8 j, encodingOffset;
  actLengthType i, numberAadBlocks, numberMsgBlocks;

  actU8 * block = info->dataBlockIn;

  actLengthType indexInData = 0u;
  actLengthType remainingLength = info->msgLen;
  actLengthType nonceLength = 15u - (actLengthType)info->lengthFieldSize;

  if (info->updateState == actAESCCM_UPDATE_CALLED)
  {
    retVal = actEXCEPTION_MODE;
  }
  else if ((aadLength != info->aadLen) || (msgLength != info->msgLen))
  {
    retVal = actEXCEPTION_INPUT_LENGTH;
  }
  else
  {
    /* Construct block B0 */
    actAESCCMConstructBlockB0(info, block, info->dataBlockOut, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* encode aad length */
    actAESCCMEncodeAadLen(info, block, &encodingOffset); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* compute number of blocks to process */
    if ((info->aadLen + encodingOffset + actAES_BLOCK_SIZE - 1u) < info->aadLen)
    {
      retVal = actEXCEPTION_LENGTH;
    }
    else
    {
      numberAadBlocks = (info->aadLen + encodingOffset + actAES_BLOCK_SIZE - 1u) / actAES_BLOCK_SIZE;
      numberMsgBlocks = (info->msgLen) / actAES_BLOCK_SIZE;

      /* process aad */
      for (i = 0u; i < numberAadBlocks; i++)
      {
        actAES128CCMConstructBlock(info, aad, info->aadLen, info->dataBlockB, &indexInData, (i == 0u) ? TRUE : FALSE, encodingOffset); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

        /* compute block = X_i XOR B_i */
        for (j = 0u; j < actAES_BLOCK_SIZE; j++)
        {
          block[j] = info->dataBlockOut[j] ^ info->dataBlockB[j]; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
        }

        /* dataBlockOut = X_{i+1} = AES(block) */
        actAESEncryptBlock(&info->wsAES, block, info->dataBlockOut, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }

      /* process message and start encryption process */
      /* set up the first bytes of key stream block A_i, they are the same for every block A_i */
      info->AESCounter[0] = info->lengthFieldSize - (actU8)1u; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
      actMemCpyByteArray(&info->AESCounter[1], info->nonce, nonceLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS */

      for (i = 0u; i < numberMsgBlocks; i++)
      {

        /* increment and encrypt counter */
        actAESCCMIncrementCounter(&info->AESCounter[nonceLength + 1u], actAES_BLOCK_SIZE - nonceLength - 1u); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        actAESEncryptBlock(&(info->wsAES), info->AESCounter, info->dataBlockB, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

        /* compute block = X_i XOR B_i */
        for (j = 0u; j < actAES_BLOCK_SIZE; j++)
        {
          /* authentication */
          block[j] = info->dataBlockOut[j] ^ msg[(actAES_BLOCK_SIZE * i) + j]; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
          /* encryption */
          out[(actAES_BLOCK_SIZE*i) + j] = msg[(actAES_BLOCK_SIZE * i) + j] ^ info->dataBlockB[j]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_VARIABLE_INDEX_CHECK */
        }

        /* authentication: dataBlockOut = X_{i+1} = AES(block) */
        actAESEncryptBlock(&info->wsAES, block, info->dataBlockOut, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        remainingLength -= actAES_BLOCK_SIZE;
      }

      /* copy leftovers from msg to workSpace; this will be processed in finalize */
      actMemCpyByteArray(info->dataBlockIn, &msg[info->msgLen - remainingLength], remainingLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */
      info->remainingLen = remainingLength; /* SBSW_VSECPRIM_WRITE_POINTER */
      info->updateState = actAESCCM_UPDATE_CALLED; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  return retVal;
} /* PRQA S 6060, 6080 */ /* MD_VSECPRIM_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  actAESCCMUpdateDecrypt()
 *********************************************************************************************************************/
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
 */
VSECPRIM_FUNC(actRETURNCODE) actAESCCMUpdateDecrypt(
  VSECPRIM_P2VAR_PARA(actAESCCMSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) aad,
  const actLengthType aadLength,
  VSECPRIM_P2CONST_PARA(actU8) msg,
  const actLengthType msgLength,
  VSECPRIM_P2VAR_PARA(actU8) out,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal = actOK;

  actU8 j, encodingOffset;
  actLengthType i, numberAadBlocks, numberMsgBlocks;
  actLengthType indexInData = 0u;

  actU8 * block = info->dataBlockIn;
  actLengthType remainingLength = info->msgLen;
  actLengthType nonceLength = 15u - (actLengthType)info->lengthFieldSize;

  if (info->updateState == actAESCCM_UPDATE_CALLED)
  {
    retVal = actEXCEPTION_MODE;
  }
  else if ((aadLength != info->aadLen) || (msgLength != info->msgLen))
  {
    retVal = actEXCEPTION_INPUT_LENGTH;
  }
  else
  {
    /* Start: Reconstruct the CBC-MAC T;
     * Step 1: Process aad */

     /* Construct block B0 */
    actAESCCMConstructBlockB0(info, block, info->dataBlockOut, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* encode aad length */
    actAESCCMEncodeAadLen(info, block, &encodingOffset); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* compute number of blocks to process */  /* compute number of blocks to process */
    if ((info->aadLen + encodingOffset + actAES_BLOCK_SIZE - 1u) < info->aadLen)
    {
      retVal = actEXCEPTION_LENGTH;
    }
    else
    {
      numberAadBlocks = (info->aadLen + encodingOffset + actAES_BLOCK_SIZE - 1u) / actAES_BLOCK_SIZE;
      numberMsgBlocks = (info->msgLen) / actAES_BLOCK_SIZE;

      /* process aad */
      for (i = 0u; i < numberAadBlocks; i++)
      {
        actAES128CCMConstructBlock(info, aad, info->aadLen, info->dataBlockB, &indexInData, (i == 0u) ? TRUE : FALSE, encodingOffset); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

        /* compute block = X_i XOR B_i */
        for (j = 0u; j < actAES_BLOCK_SIZE; j++)
        {
          block[j] = info->dataBlockOut[j] ^ info->dataBlockB[j]; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
        }

        /* dataBlockOut = X_{i+1} = AES(block) */
        actAESEncryptBlock(&info->wsAES, block, info->dataBlockOut, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }

      /* Step 2: Process the message;
       * The message has to be decrypted first
       * set up the first bytes of block A_i in info->AESCounter, they are the same for every block A_i */
      info->AESCounter[0] = info->lengthFieldSize - (actU8)1u; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
      actMemCpyByteArray(&(info->AESCounter[1]), info->nonce, nonceLength);  /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS */

      for (i = 0u; i < numberMsgBlocks; i++)
      {

        /* increment and encrypt counter; the encrypted counter will be contained in block */
        actAESCCMIncrementCounter(&info->AESCounter[nonceLength + 1u], actAES_BLOCK_SIZE - nonceLength - 1u); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        actAESEncryptBlock(&(info->wsAES), info->AESCounter, block, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

        /* decrypt a block of the message */
        for (j = 0; j < actMin(actAES_BLOCK_SIZE, remainingLength); j++)
        {
          /* write a block of the decrypted message to out */
          out[(actAES_BLOCK_SIZE*i) + j] = msg[(actAES_BLOCK_SIZE * i) + j] ^ block[j]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_VARIABLE_INDEX_CHECK */
        }

        /* compute block = X_i XOR B_i */
        for (j = 0u; j < actAES_BLOCK_SIZE; j++)
        {
          block[j] = out[(actAES_BLOCK_SIZE*i) + j] ^ info->dataBlockOut[j]; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
        }

        /* dataBlockOut = X_{i+1} = AES(block) */
        actAESEncryptBlock(&info->wsAES, block, info->dataBlockOut, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        remainingLength -= actAES_BLOCK_SIZE;
      }

      /* copy leftovers from msg to workSpace; this will be processed in finalize */
      actMemCpyByteArray(info->dataBlockIn, &msg[info->msgLen - remainingLength], remainingLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */
      info->remainingLen = remainingLength; /* SBSW_VSECPRIM_WRITE_POINTER */
      info->updateState = actAESCCM_UPDATE_CALLED; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  return retVal;
} /* PRQA S 6060, 6080 */ /* MD_VSECPRIM_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  actAESCCMFinalizeEncrypt()
 *********************************************************************************************************************/
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
 */
VSECPRIM_FUNC(actRETURNCODE) actAESCCMFinalizeEncrypt(
  VSECPRIM_P2VAR_PARA(actAESCCMSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) out,
  VSECPRIM_P2VAR_PARA(actU8) tag,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal;
  actU8 j;
  actU8 * block = info->dataBlockB;
  actLengthType remainingLength = info->remainingLen;
  actLengthType nonceLength = 15u - (actLengthType)info->lengthFieldSize;

  if (remainingLength > actAES_BLOCK_SIZE)
  {
    retVal = actEXCEPTION_LENGTH;
  }
  else
  {
    if (remainingLength > 0u)
    {
      /* pad last block of msg with zeros */
      actMemClear(&info->dataBlockIn[remainingLength], actAES_BLOCK_SIZE - remainingLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */

      for (j = 0; j < actAES_BLOCK_SIZE; j++)
      {
        block[j] = info->dataBlockOut[j] ^ info->dataBlockIn[j]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_VARIABLE_INDEX_CHECK */
      }

      actAESEncryptBlock(&(info->wsAES), block, info->dataBlockOut, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }

    /* compute CBC MAC T = trunc_M(X_{n+1}) */
    actMemCpyByteArray(info->MAC, info->dataBlockOut, info->authFieldSize); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */

    /* encrypt remaining bytes of msg */
    actAESCCMIncrementCounter(&info->AESCounter[nonceLength + 1u], actAES_BLOCK_SIZE - nonceLength - 1u); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    actAESEncryptBlock(&(info->wsAES), info->AESCounter, info->dataBlockOut, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    for (j = 0; j < remainingLength; j++)
    {
      out[j] = info->dataBlockIn[j] ^ info->dataBlockOut[j]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_VARIABLE_INDEX_CHECK */
    }

    /* compute authentication tag by encrypting the MAC
     * therefore reset counter */
    actMemClear(info->AESCounter, actAES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    info->AESCounter[0] = info->lengthFieldSize - (actU8)1u; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
    actMemCpyByteArray(&info->AESCounter[1], info->nonce, nonceLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */

    actAESEncryptBlock(&(info->wsAES), info->AESCounter, info->dataBlockOut, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* store encrypted authentication value in encAuthValue */
    for (j = 0; j < info->authFieldSize; j++)
    {
      tag[j] = info->MAC[j] ^ info->dataBlockOut[j]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_VARIABLE_INDEX_CHECK */
    }

    retVal = actOK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  actAESCCMFinalizeDecrypt()
 *********************************************************************************************************************/
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
 */
VSECPRIM_FUNC(actRETURNCODE) actAESCCMFinalizeDecrypt(
  VSECPRIM_P2VAR_PARA(actAESCCMSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) out,
  VSECPRIM_P2VAR_PARA(actU8) tag,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal;
  actU8 j;
  actU8 * block = info->dataBlockB;
  actLengthType remainingLength = info->remainingLen;
  actLengthType nonceLength = 15u - (actLengthType)info->lengthFieldSize;

  if (remainingLength > actAES_BLOCK_SIZE)
  {
    retVal = actEXCEPTION_LENGTH;
  }
  else
  {
    if (remainingLength > 0u)
    {
      /* decrypt last block of msg */
      actAESCCMIncrementCounter(&info->AESCounter[nonceLength + 1u], actAES_BLOCK_SIZE - nonceLength - 1u); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      actAESEncryptBlock(&(info->wsAES), info->AESCounter, info->dataBlockB, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* decrypt remaining bytes of msg */
      for (j = 0; j < remainingLength; j++)
      {
        info->dataBlockIn[j] ^= info->dataBlockB[j]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_VARIABLE_INDEX_CHECK */
      }

      /* copy bytes of decrypted msg to out */
      actMemCpyByteArray(out, info->dataBlockIn, remainingLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */

      /* start authentication phase */
      /* pad decrypted msg with zeros */
      actMemClear(&info->dataBlockIn[remainingLength], actAES_BLOCK_SIZE - remainingLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */

      /* decryptedMsgBlock XOR dataBlockOut */
      for (j = 0; j < actAES_BLOCK_SIZE; j++)
      {
        block[j] = info->dataBlockIn[j] ^ info->dataBlockOut[j]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_VARIABLE_INDEX_CHECK */
      }

      actAESEncryptBlock(&(info->wsAES), block, info->dataBlockOut, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }

    /* compute CBC MAC T = trunc_M(X_{n+1}) */
    actMemCpyByteArray(info->MAC, info->dataBlockOut, info->authFieldSize); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */

    /* compute authentication tag by encrypting the MAC
    * therefore reset counter */
    actMemClear(info->AESCounter, actAES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    info->AESCounter[0] = info->lengthFieldSize - (actU8)1u; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
    actMemCpyByteArray(&info->AESCounter[1], info->nonce, nonceLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */

    actAESEncryptBlock(&(info->wsAES), info->AESCounter, info->dataBlockOut, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* store encrypted authentication value in encAuthValue */
    for (j = 0; j < info->authFieldSize; j++)
    {
      tag[j] = info->MAC[j] ^ info->dataBlockOut[j]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_VARIABLE_INDEX_CHECK */
    }

    retVal = actOK;
  }

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* VSECPRIM_ACTCCM_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  END OF FILE: actIAES.c
 *********************************************************************************************************************/
