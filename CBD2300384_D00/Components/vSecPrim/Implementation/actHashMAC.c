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
/*!        \file  actHashMAC.c
 *        \brief  Interface for SHA-1 based Hash MAC algorithm.
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
#define ACTHASHMAC_SOURCE

#include "actIHashMAC.h"
#include "actUtilities.h"

#if (VSECPRIM_ACTHASHMAC_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actHashMACInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actHashMACInit(
   VSECPRIM_P2VAR_PARA(actHASHMACSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_length,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Local Variables ------------------------------------------------ */
  VSECPRIM_P2VAR_PARA(actSHASTRUCT) sha = &info->sha; /* PRQA S 0781 */ /* MD_VSECPRIM_5.6 */
  VSECPRIM_P2VAR_PARA(actU8) tmp_block = info->sha.buffer;
  actRETURNCODE retVal = actOK;
  actLengthType i;

  /* ----- Implementation ------------------------------------------------- */
  /* store key */
  if (key_length < 1u)
  {
    retVal = actEXCEPTION_LENGTH;
  }
  else if (key_length <= actHASH_BLOCK_SIZE_SHA)
  {
    /* copy key to key_buf */
    actMemCpyByteArray(info->key_buf, key, key_length); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
    info->key_length = key_length;
  }
  else
  {
    /* use SHA-1(key) */
    (void)actSHAInit(sha);
    (void)actSHAUpdate(sha, key, key_length, watchdog);
    (void)actSHAFinalize(sha, info->key_buf, watchdog);
    info->key_length = actHASH_SIZE_SHA;
  }

  if (retVal == actOK)
  {
    /* ipad */
    actMemCpyByteArray(tmp_block, info->key_buf, (info->key_length)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
    actMemClear(&tmp_block[info->key_length], (actLengthType)(actHASH_BLOCK_SIZE_SHA - info->key_length)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
    for (i = 0u; i < actHASH_BLOCK_SIZE_SHA; ++i)
    {
      tmp_block[i] ^= (actU8)(0x36u);
    }

    (void)actSHAInit(sha);
    (void)actSHAUpdate(sha, tmp_block, actHASH_BLOCK_SIZE_SHA, watchdog);
  }

  return retVal;
}


/**********************************************************************************************************************
 *  actHashMACUpdate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actHashMACUpdate(
   VSECPRIM_P2VAR_PARA(actHASHMACSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) dataIN, actLengthType length,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  return actSHAUpdate(&info->sha, dataIN, length, watchdog);
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
VSECPRIM_FUNC(actRETURNCODE) actHashMACFinalize(
   VSECPRIM_P2VAR_PARA(actHASHMACSTRUCT) info, VSECPRIM_P2VAR_PARA(actU8) hash,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2VAR_PARA(actSHASTRUCT) sha = &info->sha; /* PRQA S 0781 */ /* MD_VSECPRIM_5.6 */
  VSECPRIM_P2VAR_PARA(actU8) tmp_block = info->sha.buffer;
   actLengthType i;

  (void)actSHAFinalize(sha, hash, watchdog);

  /* opad */
  actMemCpyByteArray(tmp_block, info->key_buf, (info->key_length)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
  actMemClear(&tmp_block[info->key_length], (actLengthType)(actHASH_BLOCK_SIZE_SHA - info->key_length)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
  for (i = 0u; i < actHASH_BLOCK_SIZE_SHA; ++i)
  {
    tmp_block[i] ^= (actU8)(0x5cu);
  }

  (void)actSHAInit(sha);
  (void)actSHAUpdate(sha, tmp_block, actHASH_BLOCK_SIZE_SHA, watchdog);
  (void)actSHAUpdate(sha, hash, actHASH_SIZE_SHA, watchdog);
  (void)actSHAFinalize(sha, hash, watchdog);

  return actOK;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTHASHMAC_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actHashMAC.c
 *********************************************************************************************************************/
