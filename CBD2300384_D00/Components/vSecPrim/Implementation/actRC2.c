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
/*!        \file  actRC2.c
 *        \brief  This file contains an RC2 implementation.
 *
 *      \details  Currently the actClib version is used.
 *                This file is part of the embedded systems library cvActLib/ES
 *********************************************************************************************************************/

/****************************************************************************
 **
 ** This file contains: A RC2 implementation.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **   byte rotation and basic RC2 transformation macros (see bellow)
 **
 ** local functions:
 **   basic RC2 transformation functions (see bellow)
 **   actRC2KeySetup
 **   actRC2Init
 **   actRC2Encrypt
 **   actRC2Decrypt
 **   actRC2InitCryptBlock
 **   actRC2EncryptBlock
 **   actRC2DecryptBlock
 **   actRC2EncryptMultipleBlocks
 **   actRC2DecryptMultipleBlocks
 **   actRC2Transform
 **
 ** global functions:
 **
 **
 ***************************************************************************/
#define ACTRC2_SOURCE

#include "actRC2.h"
#include "actUtilities.h"
#include "actConfig.h"
#include "actWatchdog.h"

#if (VSECPRIM_ACTRC2_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/* PRQA S 6010, 6030, 6060 EOF */ /* MD_VSECPRIM_QM_ONLY_METRIC */
/* PRQA S 2889, 2024 EOF */ /* MD_VSECPRIM_QM_ONLY_RETURN_PATHS */
/* PRQA S 3200 EOF */ /* MD_VSECPRIM_QM_ONLY_UNUSED_FUNCTION */
/* PRQA S 2981, 2983, 2985 EOF */ /* MD_VSECPRIM_REDUNDANT_ASSIGNMENT */
/* PRQA S 0488 EOF */ /* MD_VSECPRIM_QM_ONLY_POINTER_ARITHMETIC */
/* PRQA S 3387, 3440, 1338 EOF */ /* MD_VSECPRIM_QM_ONLY_SHORT_DECREMENT_INCREMENT_OPERATION */
/* PRQA S 2463, 3418 EOF */ /* MD_VSECPRIM_QM_ONLY_VARIABLE_IN_LOOP */
/* PRQA S 0310, 3305 EOF */ /* MD_VSECPRIM_QM_ONLY_POINTER_CAST_TO_LARGER_TYPE */

/****************************************************************************
 ** Types and constants
 ***************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** Look up table made of "random" bytes based on the digits of PI
 ***************************************************************************/
# define VSECPRIM_START_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) pitable[256] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.7 */
{
   0xd9, 0x78, 0xf9, 0xc4, 0x19, 0xdd, 0xb5, 0xed, 0x28, 0xe9, 0xfd, 0x79, 0x4a, 0xa0, 0xd8, 0x9d,
   0xc6, 0x7e, 0x37, 0x83, 0x2b, 0x76, 0x53, 0x8e, 0x62, 0x4c, 0x64, 0x88, 0x44, 0x8b, 0xfb, 0xa2,
   0x17, 0x9a, 0x59, 0xf5, 0x87, 0xb3, 0x4f, 0x13, 0x61, 0x45, 0x6d, 0x8d, 0x09, 0x81, 0x7d, 0x32,
   0xbd, 0x8f, 0x40, 0xeb, 0x86, 0xb7, 0x7b, 0x0b, 0xf0, 0x95, 0x21, 0x22, 0x5c, 0x6b, 0x4e, 0x82,
   0x54, 0xd6, 0x65, 0x93, 0xce, 0x60, 0xb2, 0x1c, 0x73, 0x56, 0xc0, 0x14, 0xa7, 0x8c, 0xf1, 0xdc,
   0x12, 0x75, 0xca, 0x1f, 0x3b, 0xbe, 0xe4, 0xd1, 0x42, 0x3d, 0xd4, 0x30, 0xa3, 0x3c, 0xb6, 0x26,
   0x6f, 0xbf, 0x0e, 0xda, 0x46, 0x69, 0x07, 0x57, 0x27, 0xf2, 0x1d, 0x9b, 0xbc, 0x94, 0x43, 0x03,
   0xf8, 0x11, 0xc7, 0xf6, 0x90, 0xef, 0x3e, 0xe7, 0x06, 0xc3, 0xd5, 0x2f, 0xc8, 0x66, 0x1e, 0xd7,
   0x08, 0xe8, 0xea, 0xde, 0x80, 0x52, 0xee, 0xf7, 0x84, 0xaa, 0x72, 0xac, 0x35, 0x4d, 0x6a, 0x2a,
   0x96, 0x1a, 0xd2, 0x71, 0x5a, 0x15, 0x49, 0x74, 0x4b, 0x9f, 0xd0, 0x5e, 0x04, 0x18, 0xa4, 0xec,
   0xc2, 0xe0, 0x41, 0x6e, 0x0f, 0x51, 0xcb, 0xcc, 0x24, 0x91, 0xaf, 0x50, 0xa1, 0xf4, 0x70, 0x39,
   0x99, 0x7c, 0x3a, 0x85, 0x23, 0xb8, 0xb4, 0x7a, 0xfc, 0x02, 0x36, 0x5b, 0x25, 0x55, 0x97, 0x31,
   0x2d, 0x5d, 0xfa, 0x98, 0xe3, 0x8a, 0x92, 0xae, 0x05, 0xdf, 0x29, 0x10, 0x67, 0x6c, 0xba, 0xc9,
   0xd3, 0x00, 0xe6, 0xcf, 0xe1, 0x9e, 0xa8, 0x2c, 0x63, 0x16, 0x01, 0x3f, 0x58, 0xe2, 0x89, 0xa9,
   0x0d, 0x38, 0x34, 0x1b, 0xab, 0x33, 0xff, 0xb0, 0xbb, 0x48, 0x0c, 0x5f, 0xb9, 0xb1, 0xcd, 0x2e,
   0xc5, 0xf3, 0xdb, 0x47, 0xe5, 0xa5, 0x9c, 0x77, 0x0a, 0xa6, 0x20, 0x68, 0xfe, 0x7f, 0xc1, 0xad
};

# define VSECPRIM_STOP_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** GLOBAL FUNCTIONS
 ***************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actRC2KeySetup()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(actRETURNCODE) actRC2KeySetup(
   VSECPRIM_P2VAR_PARA(actRC2STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   if (key == NULL_PTR)
   {
      return actEXCEPTION_NULL;
   }
   else if ((key_len > (info->max_key_len)) || (key_len == 0u))
   {
      return actEXCEPTION_LENGTH;
   }
   else
   {

      actU8 L[RC2_MAX_KEY_LENGTH];
      VSECPRIM_P2VAR_PARA(actU16) e_key = info->e_key;
      actLengthType i;

      /* read user key into L[] */
      info->key_word_len = key_len;
      for (i = 0u; i < info->key_word_len; i++)
      {
         L[i] = key[i];
      }

      /* key expansion */
      for (i = info->key_word_len; i < info->max_key_len; i++)
      {
         L[i] = pitable[(L[i-1u] + L[i-info->key_word_len]) & 0xFFu];
         actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
      }

      L[info->max_key_len - info->eff_key_word_len] = pitable[ L[info->max_key_len - info->eff_key_word_len] & RC2_MASK];

      for (i = 0u; i <= ((info->max_key_len - 1u) - info->eff_key_word_len); i++)
      {
         L[(info->max_key_len - 1u - info->eff_key_word_len) - i] = pitable[L[(info->max_key_len  - info->eff_key_word_len) - i] ^ L[info->max_key_len - 1u - i ]];
      }
      /* copy key to RC2 key structure */
      for (i = 0u; i < ((info->max_key_len)/2u); i++)
      {
         e_key[i] =  L[2u*i] +  (256u * (L[(2u*i)+1u]));
      }

      actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
      return actOK;
   }
}


/**********************************************************************************************************************
 *  actRC2Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(actRETURNCODE) actRC2Init(
   VSECPRIM_P2VAR_PARA(actRC2STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len, actLengthType eff_key_len,
   VSECPRIM_P2CONST_PARA(actU8) iv, actU8 mode,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   /* initialization mode, buffers, effective key lenght, etc. */

   info->mode = mode;
   info->buffer_used = 0u;
   info->eff_key_word_len = actMin(eff_key_len, RC2_MAX_KEY_LENGTH);
   info->max_key_len = RC2_MAX_KEY_LENGTH;
   switch (mode)
   {
      case actRC2_ECB:
      {
         break;
      }
      case actRC2_CBC:
      {
         if (iv == NULL_PTR)
         {
            actMemset(info->prev_block, 0u, actRC2_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
         } else {
            actMemCpyByteArray(info->prev_block, iv, actRC2_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
         }
         break;
      }
      default:
      {
         return actEXCEPTION_ALGOID; /* PRQA S 2982 */ /* MD_VSECPRIM_2.2 */
      }
   }

   /* key setup */
   return actRC2KeySetup(info, key, key_len, watchdog);
}


/**********************************************************************************************************************
 *  actRC2Encrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(actRETURNCODE) actRC2Encrypt(
   VSECPRIM_P2VAR_PARA(actRC2STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
   VSECPRIM_P2VAR_PARA(actU8) out_buffer, VSECPRIM_P2VAR_PARA(actLengthType) out_length,
   boolean final, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   /* out_size = blocks*8 */
   actLengthType out_size = ((in_length + info->buffer_used) / actRC2_BLOCK_SIZE) * actRC2_BLOCK_SIZE;

   /* check out_buffer size */
   if (in_length > (0xFFFFFFFFuL - actRC2_BLOCK_SIZE))
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
      if (*out_length < (out_size + actRC2_BLOCK_SIZE))
      {
         return actEXCEPTION_LENGTH;
      }
      *out_length = out_size + actRC2_BLOCK_SIZE;
   }

   /* transform input */
   if (in_length > 0u)
   {
      actRC2Transform(info, in, in_length, out_buffer, actRC2EncryptMultipleBlocks, watchdog);
   }

   /* finalize ? */
   if (final == TRUE)
   {
      /* PKCS 5 padding of last buffered block */
      actU8 pad = (actU8)(actRC2_BLOCK_SIZE - info->buffer_used);
      actMemset(info->buffer + info->buffer_used, pad, pad); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
      /* encryption of last block */
      actRC2EncryptMultipleBlocks(info, info->buffer, out_buffer+out_size, 1u, watchdog);
   }

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
   return actOK;
}


/**********************************************************************************************************************
 *  actRC2Decrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(actRETURNCODE) actRC2Decrypt(
   VSECPRIM_P2VAR_PARA(actRC2STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
   VSECPRIM_P2VAR_PARA(actU8) out_buffer, VSECPRIM_P2VAR_PARA(actLengthType) out_length,
   boolean final, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   actLengthType total_bytes = in_length+info->buffer_used;
   actLengthType out_size;

   /* if finalize: check (in_length+buffer_length)%8==0 */
   if (final == TRUE)
   {
      if ((total_bytes == 0u) || ((total_bytes%actRC2_BLOCK_SIZE)!=0u))
      {
         return actEXCEPTION_INPUT_LENGTH;
      }
   }

   if (total_bytes > 0u)
   {
     /* out_size = blocks*8 */
     out_size = ((total_bytes-1u)/actRC2_BLOCK_SIZE)*actRC2_BLOCK_SIZE;
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
      if (total_bytes <= actRC2_BLOCK_SIZE)
      {
         actMemCpyByteArray(info->buffer + info->buffer_used, in, in_length); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
         info->buffer_used += in_length;
      }
      else
      {
         if ((total_bytes % actRC2_BLOCK_SIZE) == 0u)
         {
            actRC2Transform(info, in, in_length - actRC2_BLOCK_SIZE, out_buffer, actRC2DecryptMultipleBlocks, watchdog);
            actMemCpyByteArray(info->buffer, in + in_length - actRC2_BLOCK_SIZE, actRC2_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
            info->buffer_used = actRC2_BLOCK_SIZE;
         }
         else
         {
            actRC2Transform(info, in, in_length, out_buffer, actRC2DecryptMultipleBlocks, watchdog);
         }
      }
   }

   if (final == TRUE)
   {
      /* decryption of the last buffered block */
      actRC2DecryptMultipleBlocks(info, info->buffer, info->buffer, 1u, watchdog);
      {
         /* PKCS 5 unpad of the last block */
         actU8 unpad = (actU8)(actRC2_BLOCK_SIZE - info->buffer[actRC2_BLOCK_SIZE-1u]);
         if (unpad >= actRC2_BLOCK_SIZE)
         {
            return actEXCEPTION_PADDING;
         }
         else
         {
            if (*out_length < (out_size+unpad))
            {
               return actEXCEPTION_LENGTH;
            }
            actMemCpyByteArray(out_buffer + out_size, info->buffer, unpad); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
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
 *  actRC2InitCryptBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(actRETURNCODE) actRC2InitCryptBlock(
   VSECPRIM_P2VAR_PARA(actRC2STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len, actLengthType eff_key_len,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   info->eff_key_word_len = actMin(eff_key_len, RC2_MAX_KEY_LENGTH);
   info->max_key_len = RC2_MAX_KEY_LENGTH;

   return actRC2KeySetup(info, key, key_len, watchdog);
}

/**********************************************************************************************************************
 *  actRC2EncryptBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(void)  actRC2EncryptBlock(
   VSECPRIM_P2CONST_PARA(actRC2STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in_block, VSECPRIM_P2VAR_PARA(actU8) out_block,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   VSECPRIM_P2CONST_PARA(actU16) k_ptr = info->e_key;
   actU16 R[4];
   actLengthType i;

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* interpret 'in_block' as little endian 16 bit values */
   actCpyLEtoU16 (R, in_block, actRC2_BLOCK_SIZE);

   /* 16 rounds of encryption */
   for (i = 0u; i < MAX_ROUNDS; i++)
   {
         R[0] += (k_ptr[(4u * i) + 0u] + ((R[3] & R[2]) + (~R[3] & R[1])));
         R[0] = actRotl16(R[0], 1);
         R[1] += (k_ptr[(4u * i) + 1u] + ((R[0] & R[3]) + (~R[0] & R[2])));
         R[1] = actRotl16(R[1], 2);
         R[2] += (k_ptr[(4u * i) + 2u] + ((R[1] & R[0]) + (~R[1] & R[3])));
         R[2] = actRotl16(R[2], 3);
         R[3] += (k_ptr[(4u * i) + 3u] + ((R[2] & R[1]) + (~R[2] & R[0])));
         R[3] = actRotl16(R[3], 5);

         /* adding one mashing round after 5 and 11 mixing rounds */
         if ((i == 4u) || (i == 10u))
         {
            R[0] += k_ptr[R[3] & 0x3fu];
            R[1] += k_ptr[R[0] & 0x3fu];
            R[2] += k_ptr[R[1] & 0x3fu];
            R[3] += k_ptr[R[2] & 0x3fu];
         }
         actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
   }

   /* store block in little endian byte order */
   actCpyU16toLE (out_block, R, actRC2_BLOCK_SIZE);

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
}

/**********************************************************************************************************************
 *  actRC2DecryptBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(void) actRC2DecryptBlock(
   VSECPRIM_P2CONST_PARA(actRC2STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in_block, VSECPRIM_P2VAR_PARA(actU8) out_block,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   VSECPRIM_P2CONST_PARA(actU16) k_ptr = info->e_key;
   actU16 R[4];
   actLengthType i;

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* interpret 'in_block' as little endian 16 bit values */
   actCpyLEtoU16 (R, in_block, actRC2_BLOCK_SIZE);
   /* 16 rounds of decryption */
   for (i = 0u; i <= (MAX_ROUNDS - 1u); i++)
   {
         if ((((MAX_ROUNDS - 1u) - i) == 4u) || (((MAX_ROUNDS - 1u) - i) == 10u))
         {
            R[3] -= k_ptr[R[2] & 0x3fu];
            R[2] -= k_ptr[R[1] & 0x3fu];
            R[1] -= k_ptr[R[0] & 0x3fu];
            R[0] -= k_ptr[R[3] & 0x3fu];
         }
         R[3] = actRotr16(R[3], 5);
         R[3] -= ((R[0] & ~R[2]) + (R[1] & R[2]) + k_ptr[(4u * ((MAX_ROUNDS - 1u) - i)) +3u]);
         R[2] = actRotr16(R[2], 3);
         R[2] -= ((R[3] & ~R[1]) + (R[0] & R[1]) + k_ptr[(4u * ((MAX_ROUNDS - 1u) - i)) +2u]);
         R[1] = actRotr16(R[1], 2);
         R[1] -= ((R[2] & ~R[0]) + (R[3] & R[0]) + k_ptr[(4u * ((MAX_ROUNDS - 1u) - i)) +1u]);
         R[0] = actRotr16(R[0], 1);
         R[0] -= ((R[1] & ~R[3]) + (R[2] & R[3]) + k_ptr[(4u * ((MAX_ROUNDS - 1u) - i)) +0u]);

         actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
   }

   /* store block in little endian byte order */
   actCpyU16toLE (out_block, R, actRC2_BLOCK_SIZE);

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
}

/**********************************************************************************************************************
 *  actRC2EncryptMultipleBlocks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(void) actRC2EncryptMultipleBlocks(
   VSECPRIM_P2VAR_PARA(actRC2STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU8) out_buffer,
   actLengthType blocks, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   VSECPRIM_P2VAR_PARA(actU8) prev_block = info->prev_block;
   actLengthType i, bcount;

   switch (info->mode)
   {
      case actRC2_CBC:
      {
         for (bcount = 0u; bcount < blocks; ++bcount)
         {
            /* out_block = prev_block ^ in_block */
            for (i = 0u; i < actRC2_BLOCK_SIZE; ++i)
            {
               out_buffer[i] = (actU8)(prev_block[i] ^ *in++);
            }
            /* out_block = Encrypt(out_block) */
            actRC2EncryptBlock(info, out_buffer, out_buffer, watchdog);
            /* prev_block = out_block */
            for (i = 0u; i < actRC2_BLOCK_SIZE; ++i)
            {
               prev_block[i] = *out_buffer++;
            }
         }
         break;
      }
      default:   /* ECB */
      {
         for (bcount = 0u; bcount < blocks; ++bcount, in += actRC2_BLOCK_SIZE, out_buffer += actRC2_BLOCK_SIZE)
         {
            /* out_block = Encrypt(in_block) */
            actRC2EncryptBlock(info, in, out_buffer, watchdog);
         }
         break;
      }
   }
}


/**********************************************************************************************************************
 *  actRC2DecryptMultipleBlocks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(void) actRC2DecryptMultipleBlocks(
   VSECPRIM_P2VAR_PARA(actRC2STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU8) out_buffer,
   actLengthType blocks, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   VSECPRIM_P2VAR_PARA(actU8) prev_block = info->prev_block;
   VSECPRIM_P2VAR_PARA(actU8) cipher = info->copyInputData;
   actLengthType i, bcount;

   switch (info->mode)
   {
      case actRC2_CBC:
      {
         for (bcount = 0u; bcount < blocks; ++bcount)
         {
           cipher = info->copyInputData;
           actMemCpyByteArray(cipher, &in[bcount * actRC2_BLOCK_SIZE], actRC2_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

           /* out_block = Decrypt(in_block) */
            actRC2DecryptBlock(info, &in[bcount * actRC2_BLOCK_SIZE], out_buffer, watchdog);

            /* out_block = prev_block ^ out_block */
            /* prev_block = in_block */
            for (i = 0u; i < actRC2_BLOCK_SIZE; ++i)
            {
              *out_buffer++ ^= prev_block[i];
              prev_block[i] = *cipher++;
            }
         }
         break;
      }
      default:   /* ECB */
      {
        for (bcount=0u; bcount < blocks; ++bcount, in += actRC2_BLOCK_SIZE, out_buffer += actRC2_BLOCK_SIZE)
         {
            /* out_block = Decrypt(in_block) */
            actRC2DecryptBlock(info, in, out_buffer, watchdog);
         }
      break;
      }
   }
}


/**********************************************************************************************************************
 *  actRC2Transform()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */

VSECPRIM_FUNC(void) actRC2Transform(
   VSECPRIM_P2VAR_PARA(actRC2STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
   VSECPRIM_P2VAR_PARA(actU8) out_buffer, actRC2TransformFctType transform,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   actLengthType blocks, bytes, diff=0u;

   /* left overs of the last transform */
   if ((info->buffer_used > 0u) && (info->buffer_used <= actRC2_BLOCK_SIZE))
   {
      diff = actMin(in_length, actRC2_BLOCK_SIZE-info->buffer_used);
      actMemCpyByteArray(&info->buffer[info->buffer_used], in, diff); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
      info->buffer_used += diff;
      if (info->buffer_used < actRC2_BLOCK_SIZE)
      {
         /* no complete block */
         return;
      }
      else
      {
        /* transform buffered block */
# ifdef actNO_INDIRECT_CALLS
        if (transform == actRC2EncryptMultipleBlocks)
        {
          actRC2EncryptMultipleBlocks(info, info->buffer, out_buffer, 1, watchdog);
        }
        else
        {
          actRC2DecryptMultipleBlocks(info, info->buffer, out_buffer, 1, watchdog);
        }
# else
         transform(info, info->buffer, out_buffer, 1u, watchdog);
# endif
         in += diff; out_buffer += actRC2_BLOCK_SIZE;
      }
   }

   /* transform blocks */
   blocks = (in_length-diff)/actRC2_BLOCK_SIZE;
   bytes = actRC2_BLOCK_SIZE*blocks;
# ifdef actNO_INDIRECT_CALLS
   if (transform == actRC2EncryptMultipleBlocks)
   {
     actRC2EncryptMultipleBlocks(info, in, out_buffer, blocks, watchdog);
   }
   else
   {
     actRC2DecryptMultipleBlocks(info, in, out_buffer, blocks, watchdog);
   }
# else
   transform(info, in, out_buffer, blocks, watchdog);
# endif

   /* copy left overs into info->buffer */
   info->buffer_used = in_length - (diff + bytes);
   actMemCpyByteArray(info->buffer, in + bytes, info->buffer_used); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTRC2_ENABLED == STD_ON) */
