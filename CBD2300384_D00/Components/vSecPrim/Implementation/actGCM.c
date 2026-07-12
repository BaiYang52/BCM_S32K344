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
/*!        \file  actGCM.c
 *        \brief  GCM transformation functions.
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
#define ACTGCM_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actConfig.h"
#include "actIGCM.h"
#include "actIGHash.h"
#include "actIAES.h"
#include "actUtilities.h"
#include "actWatchdog.h"

#if (VSECPRIM_ACTGCM_ENABLED == STD_ON)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actGCMTransformBlock()
 **********************************************************************************************************************/
/*! \brief         Transforms an input block and updates the GHash.
 *  \details       This function GCM-transforms an input block and updates the GHash.
 *  \param[in,out] info  pointer to context structure
 *  \param[in]     input  pointer to input buffer for the block that shall be encrypted/decrypted
 *  \param[out]    output  pointer to output buffer for the encrypted/decrypted block
 *  \param[in]     mode  switch to distinguish between encryption and decryption
 *  \param[in]     watchdog  pointer to watchdog reset function
 *  \pre           info must be a valid workspace pointer
 *                 input and output each must point to a valid buffer of at least the size 16 bytes
 *  \context       ANY
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actGCMTransformBlock(
  VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) input,
  VSECPRIM_P2VAR_PARA(actU8) output,
  uint8 mode,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actGCMTransformBlock()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actGCMTransformBlock (
   VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) input,
   VSECPRIM_P2VAR_PARA(actU8) output,
   uint8 mode,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actLengthType block_len;

  if (info->buffer_used == 0u)
  {
    /* buffer empty ==> complete block */
    block_len = actGCM_BLOCK_SIZE;
  }
  else
  {
    /* buffer not empty ==> partial block */
    block_len = info->buffer_used;
  }

  if (mode == MODE_DEC)
  {
    (void)actGHashUpdate(&info->ghash, input, block_len, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  /* increment counter */
  actU32toBE(&info->J[12], actBEtoU32(&info->J[12]) + 1u); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  actAESEncryptBlock(&info->aes, info->J, info->encJ, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  actXOR(info->encJ, input, block_len); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */
  actMemCpyByteArray(output, info->encJ, block_len); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */

  if (mode == MODE_ENC)
  {
    (void)actGHashUpdate(&info->ghash, output, block_len, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }
}

/**********************************************************************************************************************
 *  actGCMInit()
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
VSECPRIM_FUNC(actRETURNCODE) actGCMInit (
   VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key,
   actLengthType key_len,
   VSECPRIM_P2CONST_PARA(actU8) IV,
   actLengthType IV_len,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) KeyGen   = &info->aes;
  VSECPRIM_P2VAR_PARA(actGHASHSTRUCT) ghash  = &info->ghash;
  VSECPRIM_P2VAR_PARA(actU8) H               =  info->J0;        /*  temporarily use J0 for sub-key H */
  VSECPRIM_P2VAR_PARA(actU32) len            =  info->count;     /*  temporary BE 64bit counter */

  /*  BIG endian */
  len[0] = 0;                                 /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  /*  IV bit-length */
  len[1] = IV_len * 8u;                       /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */

  /* AES encrypt zero block for sub-key H */
  actMemset(H, 0, actGCM_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  (void)actAESInitEncryptBlock(KeyGen, key, key_len, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  /* works in-place */
  (void)actAESEncryptBlock(KeyGen, H, H, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  /* use H to initialize GHash */
  (void)actGHashInit(ghash, H, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  if (IV_len == actGCM_IV_DEFAULT_SIZE)
  {
    /* Set J0 = IV || 0^31 || 2 */
    actMemCpyByteArray(info->J, IV, IV_len); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE_CHECK */
    /*  counter = 1 */
    actU32toBE(&info->J[IV_len], 1); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS */
  }
  else
  {
    /* J = GHASH (IV || 0^s+64 || len_64(IV))*/
    (void)actGHashUpdate(ghash, IV, IV_len, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    (void)actGHashZeroPad(ghash, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* pad 64-bit input count to last block in big endian format */
    actMemset(&H[0], 0, 8); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    actCpyU32toBE(&H[8], len, 2); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS_FIXED_SIZE */

    (void)actGHashUpdate(ghash, H, actGCM_BLOCK_SIZE, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    (void)actGHashFinalize(ghash, info->J, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    /* re-initialize GHash */
    (void)actGHashReset(ghash); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  /* Encrypt the initial counter block for XOR in tag generation */
  actAESEncryptBlock(KeyGen, info->J, info->J0, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  /* initialize GCM data area */
  info->state = STATE_AAD;    /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
  info->buffer_used = 0;      /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
  info->count[AAD_HI] = 0;    /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  /* AAD  counter = 0 */
  info->count[AAD_LO] = 0;    /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  info->count[Data_HI] = 0;   /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  /* data counter = 0 */
  info->count[Data_LO] = 0;   /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */

  return actOK;
}  /* PRQA S 6050, 6060 */ /* MD_MSR_STCAL, MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actGCMTransform()
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
VSECPRIM_FUNC(actRETURNCODE) actGCMTransform (
   VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) input,
   const actLengthType in_len,
   VSECPRIM_P2VAR_PARA(actU8) output,
   VSECPRIM_P2VAR_PARA(actLengthType) bytes_out,
   const uint8 encORdec,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE result = actOK;

  switch (info->state)
  {
    case STATE_AAD:
    {
      actU32 hi = info->count [AAD_HI],
      lo = info->count [AAD_LO];

      /* update BYTE count
       * max: 2^64 -1 bit (NIST 800-38D)
       * -> (2^64 -1) /8 = 1FFF.FFFF.FFFF.FFFF byte */
      lo += (actU32) in_len;
      if (lo < info->count[AAD_LO])
      {
        hi++;
      }

      if (hi > 0x1FFFFFFFuL)
      {
        result = actEXCEPTION_LENGTH;
      }
      else
      {
        info->count[AAD_HI] = hi; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
        info->count[AAD_LO] = lo; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */

        (void)actGHashUpdate(&info->ghash, input, in_len, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }
      break;
    }

    case STATE_CIPHERTEXT:
    {
      boolean isTransformFinished = FALSE;
      actLengthType diff = 0, output_idx = 0;
      actU32 hi = info->count [Data_HI],
      lo = info->count [Data_LO];

      /* update BYTE count
       * max: 2^39 -256 bit (NIST 800-38D)
       * --> (2^39 -256) /8 = F.FFFF.FFE0 byte */
      lo += (actU32) in_len;
      if (lo < info->count [Data_LO])
      {
        hi++;
      }

      if ((hi >  0x0000000FuL)
        || ((hi == 0x0000000FuL) && (lo > 0xFFFFFFE0uL)))
      {
        isTransformFinished = TRUE;
        result = actEXCEPTION_LENGTH;
      }
      else
      {
        info->count[Data_HI] = hi; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
        info->count[Data_LO] = lo; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */

        /* left overs of the last update */
        if (info->buffer_used > 0u)
        {
          diff = actMin(in_len, (actLengthType)(actGCM_BLOCK_SIZE - info->buffer_used));
          actMemCpyByteArray(&info->buffer[info->buffer_used], input, diff); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SUBADDRESS_VARIABLE_SIZE */
          info->buffer_used += diff; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
          if (info->buffer_used < actGCM_BLOCK_SIZE)
          {
            isTransformFinished = TRUE;
          }
          else
          {
            actGCMTransformBlock(info, info->buffer, output, encORdec, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
            output_idx += actGCM_BLOCK_SIZE;
            *bytes_out += actGCM_BLOCK_SIZE; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
          }
        }
      }

      if (isTransformFinished == FALSE)
      {
        actLengthType i, blocks, input_idx;

        /* loop through (remaining) X, blockwise */
        blocks = (in_len - diff) / actGCM_BLOCK_SIZE;
        input_idx = diff;

        for (i = blocks; i > 0u; --i)
        {
          actGCMTransformBlock(info, &input[input_idx], &output[output_idx], encORdec, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SUBADDRESS */
          input_idx += actGCM_BLOCK_SIZE;
          output_idx += actGCM_BLOCK_SIZE;
          *bytes_out += actGCM_BLOCK_SIZE; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
        }

        /* copy left overs into buffer */
        info->buffer_used = in_len - (diff + (actGCM_BLOCK_SIZE * blocks)); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
        actMemCpyByteArray(info->buffer, &input[input_idx], info->buffer_used); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */
      }
      break;
    }

    default:
      result = actEXCEPTION_UNKNOWN;
      break;
  }

  return result;
} /* PRQA S 6030, 6060 */ /* MD_MSR_STCYC, MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actGCMEncryptUpdate()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actGCMEncryptUpdate (
   VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) X,
   const actLengthType X_len,
   VSECPRIM_P2VAR_PARA(actU8) output,
   VSECPRIM_P2VAR_PARA(actLengthType) bytes_out,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  return actGCMTransform(info, X, X_len, output, bytes_out, MODE_ENC, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actGCMDecryptUpdate()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actGCMDecryptUpdate (
   VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) X,
   const actLengthType X_len,
   VSECPRIM_P2VAR_PARA(actU8) output,
   VSECPRIM_P2VAR_PARA(actLengthType) bytes_out,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  return actGCMTransform(info, X, X_len, output, bytes_out, MODE_DEC, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actGCMTriggerState()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actGCMTriggerState (
   VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retCode = actEXCEPTION_UNKNOWN;

  if (info->state == STATE_AAD)
  {
    /* zero-pad and hash the authenticated data */
    (void)actGHashZeroPad(&info->ghash, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    /* switch over to ciphertext input now */
    info->state = STATE_CIPHERTEXT; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    retCode = actOK;
  }

  return retCode;
}

/**********************************************************************************************************************
 *  actGMACFinalize()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actGMACFinalize (
   VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
   VSECPRIM_P2VAR_PARA(actU8) tag,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   VSECPRIM_P2VAR_PARA(actU8) tmp = info->buffer;  /*  temporary memory */

  /* Zero-Pad the ciphertext hash */
  (void)actGHashZeroPad(&info->ghash, watchdog);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  /* pad 64-bit input BIT counts to last block in big endian format */
  info->count [AAD_HI]  = (info->count [AAD_HI]  << 3) + (info->count [AAD_LO]  >> 31); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  info->count [AAD_LO]  = (info->count [AAD_LO]  << 3);                                 /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  info->count [Data_HI] = (info->count [Data_HI] << 3) + (info->count [Data_LO] >> 31); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  info->count [Data_LO] = (info->count [Data_LO] << 3);                                 /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  actCpyU32toBE (tmp, info->count, 4); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

  (void)actGHashUpdate(&info->ghash, tmp, actGCM_BLOCK_SIZE, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  (void)actGHashFinalize(&info->ghash, tmp, watchdog);                  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  /* use tmp for the full tag now */
  actXOR(tmp, info->J0, actGCM_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  /* return the truncated tag */
  actMemCpyByteArray(tag, tmp, actGCM_TAG_LENGTH); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

  return actOK;
}

/**********************************************************************************************************************
 *  actGCMEncryptFinalize()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actGCMEncryptFinalize (
   VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
   VSECPRIM_P2VAR_PARA(actU8) output,
   VSECPRIM_P2VAR_PARA(actLengthType) bytes_out,
   VSECPRIM_P2VAR_PARA(actU8) tag,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* If there is still input in the buffer, encrypt it as the last block */
  if (info->buffer_used > 0u)
  {
    uint8 tmpBuffer[actGCM_BLOCK_SIZE];
    *bytes_out += info->buffer_used; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    actGCMTransformBlock(info, info->buffer, tmpBuffer, MODE_ENC, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
    actMemCpyByteArray(output, tmpBuffer, info->buffer_used); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */
  }

  return actGMACFinalize(info, tag, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  actGCMDecryptFinalize()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actGCMDecryptFinalize (
   VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
   VSECPRIM_P2VAR_PARA(actU8) output,
   VSECPRIM_P2VAR_PARA(actLengthType) bytes_out,
   VSECPRIM_P2VAR_PARA(actU8) tag,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* If there is still input in the buffer, decrypt it as the last block */
  if (info->buffer_used > 0u)
  {
    uint8 tmpBuffer[actGCM_BLOCK_SIZE];
    *bytes_out += info->buffer_used; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    actGCMTransformBlock(info, info->buffer, tmpBuffer, MODE_DEC, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
    actMemCpyByteArray(output, tmpBuffer, info->buffer_used); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */
  }

  return actGMACFinalize(info, tag, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTGCM_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actGCM.c
 *********************************************************************************************************************/
