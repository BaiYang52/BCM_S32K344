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
/*!        \file  actHashMACSHA384.c
 *        \brief  Hash MAC SHA-384 implementation.
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library cvActLib/ES
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

#define ACTHASHMACSHA384_SOURCE

#include "actIHashMACSHA384.h"
#include "actUtilities.h"

#if (VSECPRIM_ACTHASHMACSHA384_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/****************************************************************************
 ** Global Functions
 ***************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actHashMACSHA384Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actHashMACSHA384Init(
  VSECPRIM_P2VAR_PARA(actHASHMACSHA384STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Local Variables ------------------------------------------------ */
  VSECPRIM_P2VAR_PARA(actSHA384STRUCT) sha384 = &info->sha384;
  VSECPRIM_P2VAR_PARA(actU8) tmp_block = info->sha384.buffer;
  actRETURNCODE retVal = actOK;
  actLengthType i;

  /* ----- Implementation ------------------------------------------------- */
  /* store key */
  if (key_length < 1u)
  {
    retVal = actEXCEPTION_LENGTH;
  }
  else if (key_length <= actHASH_BLOCK_SIZE_SHA512)
  {
    /* copy key to key_buf */
    actMemCpyByteArray(info->key_buf, key, key_length); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
    info->key_length = key_length;
  }
  else
  {
    /* use SHA-384(key) */
    (void)actSHA384Init(sha384);
    (void)actSHA384Update(sha384, key, key_length, watchdog);
    (void)actSHA384Finalize(sha384, info->key_buf, watchdog);
    info->key_length = actHASH_SIZE_SHA384;
  }

  if (retVal == actOK)
  {
    /* ipad */
    actMemCpyByteArray(tmp_block, info->key_buf, (info->key_length)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
    actMemClear(&tmp_block[info->key_length], (actLengthType)(actHASH_BLOCK_SIZE_SHA512 - info->key_length)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
    for (i = 0u; i < actHASH_BLOCK_SIZE_SHA512; ++i)
    {
      tmp_block[i] ^= (actU8)(0x36u);
    }

    (void)actSHA384Init(sha384);
    (void)actSHA384Update(sha384, tmp_block, actHASH_BLOCK_SIZE_SHA512, watchdog);
  }

  return retVal;
}

/**********************************************************************************************************************
 *  actHashMACSHA384Update()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actHashMACSHA384Update(
   VSECPRIM_P2VAR_PARA(actHASHMACSHA384STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) dataIN, actLengthType length,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   return actSHA384Update(&info->sha384, dataIN, length, watchdog);
}

/**********************************************************************************************************************
 *  actHashMACFinalize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actHashMACSHA384Finalize(
   VSECPRIM_P2VAR_PARA(actHASHMACSHA384STRUCT) info, VSECPRIM_P2VAR_PARA(actU8) hash,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   VSECPRIM_P2VAR_PARA(actSHA384STRUCT) sha384 = &info->sha384;
   VSECPRIM_P2VAR_PARA(actU8) tmp_block = info->sha384.buffer;
   actLengthType i;

   (void)actSHA384Finalize(sha384, hash, watchdog);

   /* opad */
   actMemCpyByteArray(tmp_block, info->key_buf, (info->key_length)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
   actMemClear(&tmp_block[info->key_length], (actLengthType)(actHASH_BLOCK_SIZE_SHA512 - info->key_length)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
   for (i=0u; i < actHASH_BLOCK_SIZE_SHA512; ++i)
   {
      tmp_block[i] ^= (actU8)(0x5cu);
   }

   (void)actSHA384Init(sha384);
   (void)actSHA384Update(sha384, tmp_block, actHASH_BLOCK_SIZE_SHA512, watchdog);
   (void)actSHA384Update(sha384, hash, actHASH_SIZE_SHA384, watchdog);
   (void)actSHA384Finalize(sha384, hash, watchdog);

   return actOK;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTHASHMACSHA384_ENABLED == STD_ON) */
