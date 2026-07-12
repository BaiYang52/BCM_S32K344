/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  actHKDF_HMAC.c
 *        \brief  HKDF key derivation function according to NIST SP800-56C1 using HMAC SHA256 or HMAC SHA384.
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

#define ACTHKDF_HMAC_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actIHKDF_HMAC.h"
#include "actUtilities.h"

#if (VSECPRIM_ACTHKDFHMACSHA384_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
# define VSECPRIM_WRAPPER_HMAC_SHA384
#endif

#if (VSECPRIM_ACTHKDFHMACSHA256_ENABLED == STD_ON)
# define VSECPRIM_WRAPPER_HMAC_SHA256
#endif

#if (VSECPRIM_ACTHKDF_ENABLED == STD_ON)

typedef actHASHMACUNION_HKDF actHASHMACUNION;
# include "actHashMACWrappers_int.h" /* PRQA S 5087 */ /* MD_VSECPRIM_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  actHKDF_HMAC_Extract()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actHKDF_HMAC_Extract(
  VSECPRIM_P2VAR_PARA(actHKDFSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) secret,
  actLengthType secretLength,
  VSECPRIM_P2CONST_PARA(actU8) salt,
  actLengthType saltLength,
  actU8 hashMACidx,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Implementation ------------------------------------------------- */

  /* Initialize HMAC with salt as key. (If no salt is given, use an all-zero array.) */
  if ((salt == NULL_PTR) || (saltLength == 0u))
  {
    actLengthType hashSize = actHASH_SIZE_SHA256;
# if (VSECPRIM_ACTHKDFHMACSHA384_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    if (hashMACidx == actHASHMACSHA384_ALGO_ID)
    {
      hashSize = actHASH_SIZE_SHA384;
    }
# endif

    /* In case no salt is given, the specification requires an all-zero array of size actHASH_BLOCK_SIZE_SHA256
     * or actHASH_BLOCK_SIZE_SHA384, respectively, instead. However, the key is automatically padded with zeros to
     * be of the expected size in actHashMACSHA***Init(). Thus, a smaller temporary key suffices. */
    actMemClear(info->tmpBlock.tmpKey, hashSize); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

    /* actHashMACSHA***Init returns actEXCEPTION_LENGTH, if its parameter 'key_length' < 1;
     * Here, 'key_length' = actHASH_SIZE_SHA256 = 32 or 'key_length' = actHASH_SIZE_SHA384 = 48. */
    (void)actHashMACInitWrapper(&info->actwsHmac, info->tmpBlock.tmpKey, hashSize, watchdog, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }
  else
  {
    /* actHashMACSHA***Init returns actEXCEPTION_LENGTH, if its parameter 'key_length' < 1;
     * Here, 'key_length' = salt_length >= 1
     * actHashMACSHA***Init automatically replaces salt by hash(salt), if salt_length > actHASH_BLOCK_SIZE_SHA*** */
    (void)actHashMACInitWrapper(&info->actwsHmac, salt, saltLength, watchdog, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  /*  Calculate HMAC of secret */

  /* actHashMACSHA***Update returns actEXCEPTION_LENGTH, if its parameter 'length' > 2^64 bit;
   * Here, 'length' = secretLength < 2^35 bit */
  (void)actHashMACUpdateWrapper(&info->actwsHmac, secret, secretLength, watchdog, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  /* actHashMACSHA***Finalize always returns actOK */
  (void)actHashMACFinalizeWrapper(&info->actwsHmac, info->keyDerivationKey, watchdog, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/***********************************************************************************************************************
 *  actHKDF_HMAC_Expand
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
VSECPRIM_FUNC(actRETURNCODE) actHKDF_HMAC_Expand(
  VSECPRIM_P2VAR_PARA(actHKDFSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) addInput,
  actLengthType addInputLength,
  VSECPRIM_P2VAR_PARA(actU8) derivedKey,
  const actLengthType derivedKeyLength,
  actU8 hashMACidx,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Local Variables ------------------------------------------------ */
  actRETURNCODE retVal = actOK;
  actLengthType numberOfBlocks, remainingBytes, currentBlock, blockSize;
  actU8 counter = 1u;
  actLengthType hashSize = actHASH_SIZE_SHA256;

  /* ----- Implementation ------------------------------------------------- */
# if (VSECPRIM_ACTHKDFHMACSHA384_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  if (hashMACidx == actHASHMACSHA384_ALGO_ID)
  {
    hashSize = actHASH_SIZE_SHA384;
  }
# endif

  /* Check desired length of OKM (output keying material) */
  if (derivedKeyLength > (255u * hashSize))
  {
    /* desired output size too high */
    retVal = actEXCEPTION_LENGTH;
  }
  else
  {
    /* # Determine the necessary number of blocks */
    numberOfBlocks = (derivedKeyLength + hashSize - 1u) / hashSize;
    remainingBytes = derivedKeyLength % hashSize;

    actMemClear(info->tmpBlock.blockT, hashSize); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    blockSize = 0u;

    /* Calculate block T(i) as T(i) = HMAC(keyDerivationKey, [T(i-1) || addInput || counter] */

    for (currentBlock = 1u; currentBlock <= numberOfBlocks; currentBlock++)
    {
      /* actHashMACSHA***Init returns actEXCEPTION_LENGTH, if its parameter 'key_length' < 1;
       * Here, 'key_length' = hashSize = 32 or 48. */
      (void)actHashMACInitWrapper(&info->actwsHmac, info->keyDerivationKey, hashSize, watchdog, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* actHashMACSHA***Update returns actEXCEPTION_LENGTH, if its parameter 'length' > 2^64 bit;
       * blockSize <= 16 byte, aadInputLength < 2^35 bit, size of counter = 1 byte */
      (void)actHashMACUpdateWrapper(&info->actwsHmac, info->tmpBlock.blockT, blockSize, watchdog, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      if (addInputLength > 0u)
      {
        /* If addInputLength > 0u, then addInput != NULL_PTR, as this was checked in esl_deriveKeyHKDF_HMAC_SHA*** */
        (void)actHashMACUpdateWrapper(&info->actwsHmac, addInput, addInputLength, watchdog, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }

      (void)actHashMACUpdateWrapper(&info->actwsHmac, &counter, 1u, watchdog, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* not last block or complete last block required? */
      if ((currentBlock != numberOfBlocks) || (0u == remainingBytes))
      {
        /* complete (last) block; actHashMACSHA***Finalize always returns actOK */
        (void)actHashMACFinalizeWrapper(&info->actwsHmac, info->tmpBlock.blockT, watchdog, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        actMemCpyByteArray(&derivedKey[hashSize * (currentBlock - 1u)], info->tmpBlock.blockT, hashSize); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS */
      }
      else
      {
        /* partial last block; actHashMACSHA***Finalize always returns actOK */
        (void)actHashMACFinalizeWrapper(&info->actwsHmac, info->tmpBlock.blockT, watchdog, hashMACidx);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        actMemCpyByteArray(&derivedKey[hashSize * (currentBlock - 1u)], info->tmpBlock.blockT, remainingBytes);  /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS */
      }

      blockSize = hashSize;
      counter++;
    }
  }

  return retVal;

} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/***********************************************************************************************************************
 *  actHKDF_HMAC_OneStepDerive()
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
 */
VSECPRIM_FUNC(actRETURNCODE) actHKDF_HMAC_OneStepDerive(
  VSECPRIM_P2VAR_PARA(actHKDFSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) secret,
  actLengthType secretLength,
  VSECPRIM_P2CONST_PARA(actU8) salt,
  actLengthType saltLength,
  VSECPRIM_P2CONST_PARA(actU8) addInput,
  actLengthType addInputLength,
  VSECPRIM_P2VAR_PARA(actU8) derivedKey,
  const actLengthType derivedKeyLength,
  actU8 hashMACidx,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Local Variables ------------------------------------------------ */
  actRETURNCODE retVal;
  actLengthType numberOfBlocks, remainingBytes, currentBlock;
  actU8 counterBuffer[4];
  actU32 counter = 1u;
  actLengthType hashSize = actHASH_SIZE_SHA256;

  /* ----- Implementation ------------------------------------------------- */
# if (VSECPRIM_ACTHKDFHMACSHA384_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  if (hashMACidx == actHASHMACSHA384_ALGO_ID)
  {
    hashSize = actHASH_SIZE_SHA384;
  }
# endif

 /* Initialize counter as a 4-byte array, starting from 1 */
  actU32toBE(counterBuffer, counter); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */

  /* Determine the necessary number of blocks */
  if ((derivedKeyLength + hashSize - 1u) < derivedKeyLength)
  {
    retVal = actEXCEPTION_LENGTH;
  }
  else
  {
    numberOfBlocks = (derivedKeyLength + hashSize - 1u) / hashSize;
    remainingBytes = derivedKeyLength % hashSize;

    for (currentBlock = 1u; currentBlock <= numberOfBlocks; currentBlock++) /*lint !e644 */
    {
      /* Initialize HMAC algorithm using salt as key */
      if ((salt == NULL_PTR) || (saltLength == 0u))
      {
        actMemClear(info->tmpBlock.tmpKey, hashSize); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        /* actHashMACSHA***Init returns actEXCEPTION_LENGTH, if its parameter 'key_length' < 1;
         * Here, 'key_length' = hashSize = 32 or 48. */
        (void)actHashMACInitWrapper(&info->actwsHmac, info->tmpBlock.tmpKey, hashSize, watchdog, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }
      else
      {
        /* actHashMACSHA***Init returns actEXCEPTION_LENGTH, if its parameter 'key_length' < 1;
         * Here, 'key_length' = saltLength >= 1
         * actHashMACSHA***Init automatically replaces salt by hash(salt), if salt_length > actHASH_BLOCK_SIZE_SHA*** */
        (void)actHashMACInitWrapper(&info->actwsHmac, salt, saltLength, watchdog, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }

      /* Calculate block K(i) as K(i) = HashMAC([counter || secret || addInput]) */

      /* actHashMACSHA***Update returns actEXCEPTION_LENGTH, if its parameter 'length' > 2^64 bit;
       * counter length = 32 bit, secretLength < 2^35 bit, addInputLength < 2^35 bit -> sum < 2^36 + 32 bit < 2^64 bit */
      (void)actHashMACUpdateWrapper(&info->actwsHmac, counterBuffer, 4u, watchdog, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      (void)actHashMACUpdateWrapper(&info->actwsHmac, secret, secretLength, watchdog, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      (void)actHashMACUpdateWrapper(&info->actwsHmac, addInput, addInputLength, watchdog, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* not last block or complete last block required? */
      if ((currentBlock != numberOfBlocks) || (0u == remainingBytes))
      {
        /* complete (last) block; actHashMACSHA***Finalize always returns actOK */
        (void)actHashMACFinalizeWrapper(&info->actwsHmac, &derivedKey[hashSize * (currentBlock - 1u)], watchdog, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }
      else
      {
        /* partial last block; actHashMACSHA***Finalize always returns actOK */
        (void)actHashMACFinalizeWrapper(&info->actwsHmac, info->tmpBlock.blockT, watchdog, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        actMemCpyByteArray(&derivedKey[hashSize * (currentBlock - 1u)], info->tmpBlock.blockT, remainingBytes);  /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS */
      }

      /* increment counter */
      counter++;
      actU32toBE(counterBuffer, counter); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */
    }

    retVal = actOK;
  }

  return retVal;

} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTHKDF_ENABLED == STD_ON) */
