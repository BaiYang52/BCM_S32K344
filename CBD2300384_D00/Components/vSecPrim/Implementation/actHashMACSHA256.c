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
/*!        \file  actHashMACSHA256.c
 *        \brief  This file contains the interface for SHA-256 based Hash MAC algorithm.
 *
 *      \details  Currently the actClib version is used.
 *                This file is part of the embedded systems library cvActLib/ES
 *********************************************************************************************************************/
#define ACTHASHMACSHA256_SOURCE

#include "actIHashMACSHA256.h"
#include "actUtilities.h"

#if (VSECPRIM_ACTHASHMACSHA256_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actHashMACSHA256Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actHashMACSHA256Init(
  VSECPRIM_P2VAR_PARA(actHASHMACSHA256STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) key,
  actLengthType key_length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Local Variables ------------------------------------------------ */
  VSECPRIM_P2VAR_PARA(actSHA256STRUCT) sha256 = &info->sha256;
  VSECPRIM_P2VAR_PARA(actU8) tmp_block = info->sha256.buffer;
  actRETURNCODE retVal = actOK;
  actLengthType i;

  /* ----- Implementation ------------------------------------------------- */
  /* store key */
  if (key_length < 1u)
  {
    retVal = actEXCEPTION_LENGTH;
  }
  else if (key_length <= actHASH_BLOCK_SIZE_SHA256)
  {
    /* copy key to key_buf... */
    actMemCpyByteArray(info->key_buf, key, key_length); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    info->key_length = key_length; /* SBSW_VSECPRIM_WRITE_POINTER */
  }
  else
  {
    /* ...or use SHA-256(key) */
    (void)actSHA256Init(sha256); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    (void)actSHA256Update(sha256, key, key_length, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    (void)actSHA256Finalize(sha256, info->key_buf, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    info->key_length = actHASH_SIZE_SHA256; /* SBSW_VSECPRIM_WRITE_POINTER */
  }

  if (retVal == actOK)
  {
    /* ipad phase: Compute key XOR ipad */
    actMemCpyByteArray(tmp_block, info->key_buf, (info->key_length)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    actMemClear(&tmp_block[info->key_length], (actLengthType)(actHASH_BLOCK_SIZE_SHA256 - info->key_length)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    for (i = 0u; i < actHASH_BLOCK_SIZE_SHA256; ++i)
    {
      tmp_block[i] ^= (actU8)(0x36u); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    }

    /* hash the "ipad-ded" key */
    (void)actSHA256Init(sha256); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    (void)actSHA256Update(sha256, tmp_block, actHASH_BLOCK_SIZE_SHA256, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  actHashMACSHA256Update()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actHashMACSHA256Update(
   VSECPRIM_P2VAR_PARA(actHASHMACSHA256STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) dataIN,
   actLengthType length,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   return actSHA256Update(&info->sha256, dataIN, length, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  actHashMACSHA256Finalize()
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
VSECPRIM_FUNC(actRETURNCODE) actHashMACSHA256Finalize(
   VSECPRIM_P2VAR_PARA(actHASHMACSHA256STRUCT) info,
   VSECPRIM_P2VAR_PARA(actU8) hash,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   /* ----- Local Variables ------------------------------------------------ */
   VSECPRIM_P2VAR_PARA(actSHA256STRUCT) sha256 = &info->sha256;
   VSECPRIM_P2VAR_PARA(actU8) tmp_block = info->sha256.buffer;
   actLengthType i;

   /* ----- Implementation ------------------------------------------------- */
   /* finalize the hash computation of the message */
   (void)actSHA256Finalize(sha256, hash, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

   /* compute key XOR opad */
   actMemCpyByteArray(tmp_block, info->key_buf, (info->key_length)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
   actMemClear(&tmp_block[info->key_length], (actLengthType)(actHASH_BLOCK_SIZE_SHA256 - info->key_length)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
   for (i=0u; i < actHASH_BLOCK_SIZE_SHA256; ++i)
   {
      tmp_block[i] ^= (actU8)(0x5c); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
   }

   /* hash the "opad-ded" key and the previously computed hash value */
   (void)actSHA256Init(sha256); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
   (void)actSHA256Update(sha256, tmp_block, actHASH_BLOCK_SIZE_SHA256, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
   (void)actSHA256Update(sha256, hash, actHASH_SIZE_SHA256, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
   (void)actSHA256Finalize(sha256, hash, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

   return actOK;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTHASHMACSHA256_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actHashMACSHA256.c
 *********************************************************************************************************************/
