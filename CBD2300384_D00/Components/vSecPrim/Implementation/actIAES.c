/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2016 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  actIAES.c
 *        \brief  AES implementation.
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

#define ACTIAES_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actIAES.h"
#include "actAES.h"
#include "actUtilities.h"
#include "actWatchdog.h"
#include "actConfig.h"

#if (VSECPRIM_ACTIAES_ENABLED == STD_ON)

/* PRQA S 0850 EOF */ /* MD_MSR_MacroArgumentEmpty */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define actAES_WORD_SIZE                                             (4u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actAESInitEncryptBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actAESInitEncryptBlock(
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* Call AES Key Setup */
  return actAESKeySetup(info, key, key_len, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 *  actAESEncryptBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actAESEncryptBlock(
   VSECPRIM_P2CONST_PARA(actAESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in_block, VSECPRIM_P2VAR_PARA(actU8) out_block,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2CONST_PARA(actU32) k_ptr = info->e_key;
  actU32 b0[4], b1[4]; /* PRQA S 0781 */ /* MD_VSECPRIM_5.6 */
  actLengthType i, iterations = 4u + ((info->key_dword_len - 4u) / 2u);

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* b0 = in_block */
  actCpyLEtoU32(b0, in_block, actAES_BLOCK_SIZE); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* encryption */
  for (i = 0u; i < 4u; ++i)
  {
    b0[i] ^= k_ptr[i]; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */

  }
  k_ptr = &k_ptr[4];
  for (i = 0u; i < iterations; ++i)
  {
    actAESEncryptionRound(b1, b0, k_ptr); /* SBSW_VSECPRIM_CALL_FUNCTION */
    k_ptr = &k_ptr[4];
    actAESEncryptionRound(b0, b1, k_ptr); /* SBSW_VSECPRIM_CALL_FUNCTION */
    k_ptr = &k_ptr[4];
    actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }
  actAESEncryptionRound(b1, b0, k_ptr); /* SBSW_VSECPRIM_CALL_FUNCTION */
  k_ptr = &k_ptr[4];
  actAESEncryptionFinalRound(b0, b1, k_ptr); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* out_block = b0 */
  actCpyU32toLE (out_block, b0, actAES_BLOCK_SIZE /actAES_WORD_SIZE); /* SBSW_VSECPRIM_CALL_FUNCTION */

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
}

/**********************************************************************************************************************
 *  actAESInitDecryptBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actAESInitDecryptBlock(
   VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* Call AES Key Setup */
  actRETURNCODE result = actAESKeySetup(info, key, key_len, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

  if (result == actOK)
  {
    actAESMakeDecryptionKey(info); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return result;
}

/**********************************************************************************************************************
 *  actAESDecryptBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actAESDecryptBlock(
   VSECPRIM_P2CONST_PARA(actAESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in_block, VSECPRIM_P2VAR_PARA(actU8) out_block,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2CONST_PARA(actU32) k_ptr = &info->e_key[((4u * info->key_dword_len) + 20u)];
  actU32 b0[4], b1[4]; /* PRQA S 0781 */ /* MD_VSECPRIM_5.6 */
  actLengthType i, iterations = 4u + ((info->key_dword_len - 4u) / 2u);
  actLengthType arrayIndex = ((4u * info->key_dword_len) + 20u);

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* b0 = in_block */
  actCpyLEtoU32 (b0, in_block, actAES_BLOCK_SIZE); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* decryption */
  for (i = 0u; i < 4u; ++i)
  {
    b0[i] ^= k_ptr[i + 4u]; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
  }
  for (i = 0u; i < iterations; ++i)
  {
    actAESDecryptionRound(b1, b0, k_ptr); /* SBSW_VSECPRIM_CALL_FUNCTION */
    arrayIndex -= 4u;
    k_ptr = &info->e_key[arrayIndex];
    actAESDecryptionRound(b0, b1, k_ptr); /* SBSW_VSECPRIM_CALL_FUNCTION */
    arrayIndex -= 4u;
    k_ptr = &info->e_key[arrayIndex];
    actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }
  actAESDecryptionRound(b1, b0, k_ptr); /* SBSW_VSECPRIM_CALL_FUNCTION */
  arrayIndex -= 4u;
  k_ptr = &info->e_key[arrayIndex];
  actAESDecryptionFinalRound(b0, b1, k_ptr); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* out_block = b0 */
  actCpyU32toLE(out_block, b0, actAES_BLOCK_SIZE /actAES_WORD_SIZE); /* SBSW_VSECPRIM_CALL_FUNCTION */

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
}

/**********************************************************************************************************************
 *  actAESInitEncrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actAESInitEncrypt(
   VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len, VSECPRIM_P2CONST_PARA(actU8) iv,
   actU8 mode, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE result;

  /* Check padding mode */
  result = actPaddingInit(mode & actPADDING_PM_MASK);

  if (result == actOK)
  {
    /* Init AES */
    result = actAESInit(info, key, key_len, iv, mode, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return result;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actAESEncrypt()
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
VSECPRIM_FUNC(actRETURNCODE) actAESEncrypt(
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
  VSECPRIM_P2VAR_PARA(actU8) out_buffer, VSECPRIM_P2VAR_PARA(actLengthType) out_length,
  actBoolean final, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE result = actOK;
  /* out_size = blocks*16 */
  actLengthType out_size = ((in_length + info->buffer_used) / actAES_BLOCK_SIZE) * actAES_BLOCK_SIZE;

  /* Check out_buffer size */
  if (in_length > (0xFFFFFFFFuL - actAES_BLOCK_SIZE))
  {
    result = actEXCEPTION_LENGTH;
  }
  else if ((final == TRUE) && ((info->mode & actAES_BM_MASK) == actAES_BM_CTR) && ((info->mode & actPADDING_PM_MASK) == actPADDING_PM_OFF))
  {
    /* In CTR mode, padding the final block is not required. If no padding is chosen, the last incomplete block
    is encrypted anyway. In this case, *out_length needs to be set to info->buffer_used, the number of bytes
    in the last block. */
    if (*out_length < info->buffer_used)
    {
      result = actEXCEPTION_LENGTH;
    }
    else
    {
      *out_length = info->buffer_used; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }
  else if ((final == FALSE) || ((info->mode & actPADDING_PM_MASK) == actPADDING_PM_OFF))
  {
    if (*out_length < out_size)
    {
      result = actEXCEPTION_LENGTH;
    }
    else
    {
      *out_length = out_size; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }
  else
  {
    if (*out_length < (actLengthType) (out_size + actAES_BLOCK_SIZE))
    {
      result = actEXCEPTION_LENGTH;
    }
    else
    {
      *out_length = out_size + actAES_BLOCK_SIZE; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  if (result == actOK)
  {
    /* transform input */
    if (in_length > 0u)
    {
      actAESTransform(info, in, in_length, out_buffer, actAESEncryptMultipleBlocks, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    }

    /* Finalize? */
    if (final == TRUE)
    {
      /* For any other mode than CTR mode without padding, call actPaddingPad. Without padding, actPaddingPad returns
         an error if info->buffer_used != 0. IN CTR mode without padding, an incomplete block shall not be padded and
         thus, the returned error is not the desired behaviour. */
      if (((info->mode & actAES_BM_MASK) != actAES_BM_CTR) || ((info->mode & actPADDING_PM_MASK) != actPADDING_PM_OFF))
      {
        result = actPaddingPad(info->mode & actPADDING_PM_MASK, actAES_BLOCK_SIZE, &(info->buffer_used), info->buffer); /* SBSW_VSECPRIM_CALL_FUNCTION */
      }

      if ((result == actOK) && (info->buffer_used > 0u))
      {
        /* Encryption of last block */
        actAESEncryptMultipleBlocks(info, info->buffer, &out_buffer[out_size], 1, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
      }
    }

    actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }

  return result;
} /* PRQA S 6030, 6060 */ /* MD_MSR_STCYC, MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actAESInitDecrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actAESInitDecrypt(
   VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len, VSECPRIM_P2CONST_PARA(actU8) iv,
   actU8 mode, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE result;

  /* Check padding mode */
  result = actPaddingInit(mode & actPADDING_PM_MASK);

  if (result == actOK)
  {
    /* Init AES */
    result = actAESInit(info, key, key_len, iv, mode, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

    if (result == actOK)
    {
      if ((info->mode & actAES_BM_MASK) != actAES_BM_CTR)
      {
        /* Make Decryption Key */
        actAESMakeDecryptionKey(info); /* SBSW_VSECPRIM_CALL_FUNCTION */
      }
    }
  }

  return result;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actAESDecrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actAESDecrypt(
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
  VSECPRIM_P2VAR_PARA(actU8) out_buffer, VSECPRIM_P2VAR_PARA(actLengthType) out_length,
  actBoolean final, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE result = actOK;
  actLengthType total_bytes = in_length + info->buffer_used;
  actLengthType out_size;

  /* if finalize: check (in_length+buffer_length)%16==0 */
  if (final == TRUE)
  {
     if (((info->mode & actAES_BM_MASK) != actAES_BM_CTR) || ((info->mode & actPADDING_PM_MASK) != actPADDING_PM_OFF))
     {
       /* return actEXCEPTION_INPUT_LENGTH for any mode other than CTR without padding */
       if ((total_bytes == 0u) || ((total_bytes % actAES_BLOCK_SIZE) != 0u))
       {
         result = actEXCEPTION_INPUT_LENGTH;
       }
     }
  }

  if (result == actOK)
  {
    if (total_bytes > 0u)
    {
      /* out_size = blocks*16 */
      out_size = ((total_bytes - 1u) / actAES_BLOCK_SIZE) * actAES_BLOCK_SIZE;
    }
    else
    {
      /* No ouput data will be produced as no input data present */
      out_size = 0u;
    }

    /* check out_buffer size */
    if (*out_length < out_size)
    {
      result = actEXCEPTION_LENGTH;
    }

    if (result == actOK)
    {
      /* transform input */
      if (in_length > 0u)
      {
        if (total_bytes <= actAES_BLOCK_SIZE)
        {
          actMemCpyByteArray(&info->buffer[info->buffer_used], in, (actU32)in_length); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
          info->buffer_used += in_length; /* SBSW_VSECPRIM_WRITE_POINTER */
        }
        else
        {
          if ((total_bytes % actAES_BLOCK_SIZE) == 0u)
          {
            actAESTransform(info, in, (actLengthType)(in_length - actAES_BLOCK_SIZE), out_buffer, actAESDecryptMultipleBlocks, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
            actMemCpyByteArray(info->buffer, &in[(in_length - actAES_BLOCK_SIZE)], actAES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
            info->buffer_used = actAES_BLOCK_SIZE; /* SBSW_VSECPRIM_WRITE_POINTER */
          }
          else
          {
            actAESTransform(info, in, in_length, out_buffer, actAESDecryptMultipleBlocks, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
          }
        }
      }

      if (final == TRUE)
      {
        /* decryption of the last buffered block */
        actAESDecryptMultipleBlocks(info, info->buffer, info->buffer, 1u, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

        /* The function actPaddingUnpad changes the value of info->buffer_used. In CTR mode without padding,
           this value shall not  changed. */
        if (((info->mode & actAES_BM_MASK) != actAES_BM_CTR) || ((info->mode & actPADDING_PM_MASK) != actPADDING_PM_OFF))
        {
          result = actPaddingUnpad(info->mode & actPADDING_PM_MASK, actAES_BLOCK_SIZE, &(info->buffer_used), info->buffer); /* SBSW_VSECPRIM_CALL_FUNCTION */
        }

        if (result == actOK)
        {
          if (*out_length < (out_size + info->buffer_used))
          {
            result = actEXCEPTION_LENGTH;
          }
          else
          {
            actMemCpyByteArray(&out_buffer[out_size], info->buffer, info->buffer_used); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
            *out_length = out_size + info->buffer_used; /* SBSW_VSECPRIM_WRITE_POINTER */
          }
        }
      }
      else
      {
        *out_length = out_size; /* SBSW_VSECPRIM_WRITE_POINTER */
      }

      actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
    }
  }

  return result;
} /* PRQA S 6060, 6080, 6010, 6030 */ /* MD_VSECPRIM_STPAR, MD_MSR_STMIF, MD_MSR_STPTH, MD_MSR_STCYC */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTIAES_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actIAES.c
 *********************************************************************************************************************/
