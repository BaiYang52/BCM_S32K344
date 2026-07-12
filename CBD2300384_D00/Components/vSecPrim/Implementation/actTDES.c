/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2006 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  actTDES.c
 *        \brief  This file contains the implementation of the TDES algorithm.
 *
 *      \details  Currently the actClib version is used.
 *                This file is part of the embedded systems library cvActLib/ES
 *********************************************************************************************************************/
#define ACTTDES_SOURCE

#include "actTDES.h"
#include "actUtilities.h"
#include "actWatchdog.h"
#include "actConfig.h"

#if (VSECPRIM_ACTTDES_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/* PRQA S 6010, 6030, 6060 EOF */ /* MD_VSECPRIM_QM_ONLY_METRIC */
/* PRQA S 2889, 2024 EOF */ /* MD_VSECPRIM_QM_ONLY_RETURN_PATHS */
/* PRQA S 3200 EOF */ /* MD_VSECPRIM_QM_ONLY_UNUSED_FUNCTION */
/* PRQA S 0488 EOF */ /* MD_VSECPRIM_QM_ONLY_POINTER_ARITHMETIC */
/* PRQA S 2463, 3418 EOF */ /* MD_VSECPRIM_QM_ONLY_VARIABLE_IN_LOOP */
/* PRQA S 3387, 3440, 1338 EOF */ /* MD_VSECPRIM_QM_ONLY_SHORT_DECREMENT_INCREMENT_OPERATION */
/* PRQA S 0310, 3305 EOF */ /* MD_VSECPRIM_QM_ONLY_POINTER_CAST_TO_LARGER_TYPE */

/****************************************************************************
 ** Macros for rotate, get block ... etc
 ***************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** GLOBAL FUNCTIONS
 ***************************************************************************/

/**********************************************************************************************************************
 *  actTDESInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(actRETURNCODE) actTDESInit(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actU8 key_len,
   actU8 crypt_mode, VSECPRIM_P2CONST_PARA(actU8) iv,
   actU8 block_mode, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   /* initialization mode, buffers, etc. */
   info->mode = block_mode;
   info->buffer_used = 0u;
   switch (block_mode)
   {
      case actDES_ECB:
      {
         break;
      }
      case actDES_CBC:
      {
         if (iv == NULL_PTR)
         {
            actMemset(info->prev_block, 0u, actDES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
         }
         else
         {
            actMemCpyByteArray(info->prev_block, iv, actDES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
         }
         break;
      }
      default:
      {
         return actEXCEPTION_ALGOID;
      }
   }

   /* key setup */
   return actTDESKeySetup(info, key, key_len, crypt_mode, watchdog);
}

/**********************************************************************************************************************
 *  actTDESKeySetup()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(actRETURNCODE) actTDESKeySetup(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actU8 key_len,
   actU8 crypt_mode, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
      /* init key pointers */
      info->k1_ptr = key;
      info->k2_ptr = key + 8u;
      if (key_len == 24u)
      {
         info->k3_ptr = key + 16u;
      }
      else
      {
         info->k3_ptr = key;
      }

      if (crypt_mode == EN0)
      {
         actDESKeySetup(&info->k1, info->k1_ptr, EN0, watchdog);
         actDESKeySetup(&info->k2, info->k2_ptr, DE1, watchdog);
         actDESKeySetup(&info->k3, info->k3_ptr, EN0, watchdog);
      }
      else
      {
         actDESKeySetup(&info->k1, info->k3_ptr, DE1, watchdog);
         actDESKeySetup(&info->k2, info->k2_ptr, EN0, watchdog);
         actDESKeySetup(&info->k3, info->k1_ptr, DE1, watchdog);
      }

      return actOK;
}

/**********************************************************************************************************************
 *  actTDESInitEncrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(actRETURNCODE) actTDESInitEncrypt(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actU8  key_len,
   VSECPRIM_P2CONST_PARA(actU8) iv, actU8 mode,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   return actTDESInit(info, key, key_len, EN0, iv, mode, watchdog);
}

/**********************************************************************************************************************
 *  actTDESEncrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(actRETURNCODE) actTDESEncrypt(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
   VSECPRIM_P2VAR_PARA(actU8) out_buffer, VSECPRIM_P2VAR_PARA(actLengthType) out_length,
   boolean final, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   /* out_size = blocks*8 */
   actLengthType out_size = (actLengthType)((in_length+info->buffer_used)/actDES_BLOCK_SIZE) * actDES_BLOCK_SIZE;

   /* check out_buffer size */
   if (in_length > (0xFFFFFFFFuL - actDES_BLOCK_SIZE))
   {
     return actEXCEPTION_LENGTH;
   }

   if (final == FALSE)
   {
      if (*out_length < out_size)
      {
         return actEXCEPTION_LENGTH;
      }
      *out_length = out_size;
   }
   else
   {
      if (*out_length < (actLengthType)(out_size+actDES_BLOCK_SIZE))
      {
         return actEXCEPTION_LENGTH;
      }
      *out_length = out_size + actDES_BLOCK_SIZE;
   }

   /* transform input */
   if (in_length > 0u)
   {
      actTDESTransform(info, in, in_length, out_buffer, actTDESEncryptMultipleBlocks, watchdog);
   }

   /* finalize ? */
   if (final == TRUE)
   {
      /* PKCS 5 padding of last buffered block */
      actU8 pad = (actU8)(actDES_BLOCK_SIZE - info->buffer_used);
      actMemset(info->buffer + info->buffer_used, pad, (actLengthType)pad); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
      /* encryption of last block */
      actTDESEncryptMultipleBlocks(info, info->buffer, out_buffer + out_size, 1u, watchdog);
   }

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
   return actOK;
}

/**********************************************************************************************************************
 *  actTDESInitDecrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(actRETURNCODE) actTDESInitDecrypt(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actU8 key_len,
   VSECPRIM_P2CONST_PARA(actU8) iv, actU8 mode,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   return actTDESInit(info, key, key_len, DE1, iv, mode, watchdog);
}

/**********************************************************************************************************************
 *  actTDESDecrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(actRETURNCODE) actTDESDecrypt(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
   VSECPRIM_P2VAR_PARA(actU8) out_buffer, VSECPRIM_P2VAR_PARA(actLengthType) out_length,
   boolean final, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actLengthType total_bytes = (actLengthType)(in_length + info->buffer_used);
  actLengthType out_size;

   /* if finalize: check (in_length+buffer_length)%8==0 */
   if (final == TRUE)
   {
      if ((total_bytes == 0u) || ((total_bytes % actDES_BLOCK_SIZE) != 0u))
      {
         return actEXCEPTION_INPUT_LENGTH;
      }
   }

   if (total_bytes > 0u)
   {
     /* out_size = blocks*8 */
     out_size = ((total_bytes-1u) / actDES_BLOCK_SIZE) * actDES_BLOCK_SIZE;
   }
   else
   {
     /* No ouput data will be produced as no input data present */
     out_size = 0u;
   }

   /* check out_buffer size */
   if (*out_length < out_size)
   {
      return actEXCEPTION_LENGTH;
   }

   /* transform input */
   if (in_length > 0u)
   {
      if (total_bytes <= actDES_BLOCK_SIZE)
      {
         actMemCpyByteArray(info->buffer + info->buffer_used, in, (actLengthType)in_length); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
         info->buffer_used += in_length;
      }
      else
      {
         if ((total_bytes % actDES_BLOCK_SIZE) == 0u)
         {
            actTDESTransform(info, in, (actLengthType) (in_length-actDES_BLOCK_SIZE), out_buffer, actTDESDecryptMultipleBlocks, watchdog);
            actMemCpyByteArray(info->buffer, in + in_length - actDES_BLOCK_SIZE, actDES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
            info->buffer_used = actDES_BLOCK_SIZE;
         }
         else
         {
            actTDESTransform(info, in, in_length, out_buffer, actTDESDecryptMultipleBlocks, watchdog);
         }
      }
   }

   if (final == TRUE)
   {
      /* decryption of the last buffered block */
      actTDESDecryptMultipleBlocks(info, info->buffer, info->buffer, 1u, watchdog);
      {
         /* PKCS 5 unpad of the last block */
         actU8 unpad = (actU8)(actDES_BLOCK_SIZE - info->buffer[actDES_BLOCK_SIZE-1u]);
         if (unpad >= actDES_BLOCK_SIZE)
         {
            return actEXCEPTION_PADDING;
         }
         else
         {
            if ((*out_length) < (out_size+unpad))
            {
               return actEXCEPTION_LENGTH;
            }
            actMemCpyByteArray(out_buffer + out_size, info->buffer, (actLengthType)unpad); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
            *out_length = out_size + unpad;
         }
      }
   }
   else
   {
      *out_length = out_size;
   }

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
   return actOK;
}

/**********************************************************************************************************************
 *  actTDESInitEncryptBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(actRETURNCODE) actTDESInitEncryptBlock(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actU8 key_len,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   return actTDESKeySetup(info, key, key_len, EN0, watchdog);
}

/**********************************************************************************************************************
 *  actTDESInitDecryptBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(actRETURNCODE) actTDESInitDecryptBlock(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actU8  key_len,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   return actTDESKeySetup(info, key, key_len, DE1, watchdog);
}

/**********************************************************************************************************************
 *  actTDESCryptBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(void) actTDESCryptBlock(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in_block, VSECPRIM_P2VAR_PARA(actU8) out_block,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   actU8 tmp_block[actDES_BLOCK_SIZE];

   actDESCryptBlock(&info->k1, in_block, out_block, watchdog);
   actDESCryptBlock(&info->k2, out_block, tmp_block, watchdog);
   actDESCryptBlock(&info->k3, tmp_block, out_block, watchdog);

}

/**********************************************************************************************************************
 *  actTDESEncryptMultipleBlocks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(void) actTDESEncryptMultipleBlocks(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU8) out_buffer,
   actLengthType blocks, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   VSECPRIM_P2VAR_PARA(actU8) prev_block = info->prev_block;
   actLengthType i, bcount;

   switch (info->mode)
   {
      case actDES_CBC:
      {
         for (bcount = 0u; bcount < blocks; ++bcount)
         {
            /* out_block = prev_block ^ in_block */
            for (i = 0u; i < actDES_BLOCK_SIZE; ++i)
            {
               out_buffer[i] = (actU8)(prev_block[i] ^ *in++);
            }
            /* out_block = Encrypt(out_block) */
            actTDESCryptBlock(info, out_buffer, out_buffer, watchdog);
            /* prev_block = out_block */
            for (i = 0u; i < actDES_BLOCK_SIZE; ++i)
            {
               prev_block[i] = *out_buffer++;
            }
         }
         break;
      }
      default:   /* ECB */
      {
         for (bcount = 0u; bcount < blocks; ++bcount, in += actDES_BLOCK_SIZE, out_buffer += actDES_BLOCK_SIZE)
         {
            /* out_block = Encrypt(in_block) */
            actTDESCryptBlock(info, in, out_buffer, watchdog);
         }
         break;
      }
   }
}

/**********************************************************************************************************************
 *  actTDESDecryptMultipleBlocks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(void) actTDESDecryptMultipleBlocks(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU8) out_buffer,
  actLengthType blocks, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   VSECPRIM_P2VAR_PARA(actU8) prev_block = info->prev_block;
   actLengthType i, bcount;

   switch (info->mode)
   {
      case actDES_CBC:
      {
         for (bcount = 0u; bcount<blocks; ++bcount)
         {
             /* out_block = Decrypt(in_block) */
             actTDESCryptBlock(info, in, out_buffer, watchdog);
             /* out_block = prev_block ^ out_block */
             /* prev_block = in_block */
             for (i = 0u; i < actDES_BLOCK_SIZE; ++i)
             {
                *out_buffer++ ^= prev_block[i];
                 prev_block[i] = *in++;
             }
         }
         break;
      }
      default:   /* ECB */
      {
         for (bcount = 0u; bcount < blocks; ++bcount, in += actDES_BLOCK_SIZE, out_buffer += actDES_BLOCK_SIZE)
         {
            /* out_block = Decrypt(in_block) */
            actTDESCryptBlock(info, in, out_buffer, watchdog);
         }
         break;
      }
   }
}


/**********************************************************************************************************************
 *  actTDESTransform()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(void) actTDESTransform(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
   VSECPRIM_P2VAR_PARA(actU8) out_buffer, actTDESTransformFctType transform,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actLengthType blocks, bytes, diff = 0u;

   /* left overs of the last transform */
   if ((info->buffer_used > 0u) && (info->buffer_used <= actDES_BLOCK_SIZE))
   {
      diff = actMin(in_length, (actLengthType)(actDES_BLOCK_SIZE - info->buffer_used));
      actMemCpyByteArray(&info->buffer[info->buffer_used], in, diff); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
      info->buffer_used += diff;
      if (info->buffer_used < actDES_BLOCK_SIZE)
      {
         /* no complete block */
         return;
      }
      else
      {
         /* transform buffered block */
# ifdef actNO_INDIRECT_CALLS
         if (transform == actTDESEncryptMultipleBlocks)
         {
           actTDESEncryptMultipleBlocks(info, info->buffer, out_buffer, 1u, watchdog);
         }
         else
         {
           actTDESDecryptMultipleBlocks(info, info->buffer, out_buffer, 1u, watchdog);
         }
# else
         transform(info, info->buffer, out_buffer, 1u, watchdog);
# endif
         in += diff; out_buffer += actDES_BLOCK_SIZE;
      }
   }

   /* transform blocks */
   blocks = (in_length-diff)/actDES_BLOCK_SIZE;
   bytes = actDES_BLOCK_SIZE*blocks;
# ifdef actNO_INDIRECT_CALLS
   if (transform == actTDESEncryptMultipleBlocks)
   {
     actTDESEncryptMultipleBlocks(info, in, out_buffer, blocks, watchdog);
   }
   else
   {
     actTDESDecryptMultipleBlocks(info, in, out_buffer, blocks, watchdog);
   }
# else
   transform(info, in, out_buffer, blocks, watchdog);
# endif

   /* copy left overs into info->buffer */
   info->buffer_used = in_length - (diff + bytes);
   actMemCpyByteArray(info->buffer, in + bytes, (actLengthType)(info->buffer_used)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTTDES_ENABLED == STD_ON) */
