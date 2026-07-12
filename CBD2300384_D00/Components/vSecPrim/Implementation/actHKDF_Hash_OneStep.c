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
/*!        \file  actHKDF_Hash_OneStep.c
 *        \brief  One step HKDF key derivation function according to NIST SP800-56C1 using SHA256.
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

#define ACTHKDF_HASH_ONESTEP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actIHKDF_Hash_OneStep.h"
#include "actUtilities.h"

#if (VSECPRIM_ACTHKDFHASHONESTEP_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  actHKDF_Hash_OneStepDerive()
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
VSECPRIM_FUNC(actRETURNCODE) actHKDF_Hash_OneStepDerive(
  VSECPRIM_P2VAR_PARA(actHKDFHASHSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) secret,
  actLengthType secretLength,
  VSECPRIM_P2CONST_PARA(actU8) addInput,
  actLengthType addInputLength,
  VSECPRIM_P2VAR_PARA(actU8) derivedKey,
  const actLengthType derivedKeyLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* ----- Local Variables ------------------------------------------------ */
  actRETURNCODE retVal;
  actLengthType numberOfBlocks, remainingBytes, currentBlock;
  actU8 counterBuffer[4];
  actU32 counter = 1u;

  /* ----- Implementation ------------------------------------------------- */
 /* Initialize counter as a 4-byte array, starting from 1 */
  actU32toBE(counterBuffer, counter); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */

  /* Check whether derivedKeyLength is too high to be processed. */
  if ((derivedKeyLength + actHASH_SIZE_SHA256 - 1u) < derivedKeyLength)
  {
    retVal = actEXCEPTION_LENGTH;
  }
  else
  {
    /* Determine the necessary number of blocks */
    numberOfBlocks = (derivedKeyLength + actHASH_SIZE_SHA256 - 1u) / actHASH_SIZE_SHA256;
    remainingBytes = derivedKeyLength % actHASH_SIZE_SHA256;

    for (currentBlock = 1u; currentBlock <= numberOfBlocks; currentBlock++) /*lint !e644 */
    {
      /* Calculate block K(i) as K(i) = HASH([counter || secret || addInput]) */
      (void)actSHA256Init(&info->actwsSha); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* actSHA256Update returns actEXCEPTION_LENGTH, if its parameter 'length' > 2^64 bit;
       * counter length = 32 bit, secretLength < 2^35 bit, addInputLength < 2^35 bit -> sum < 2^36 + 32 bit < 2^64 bit */
      (void)actSHA256Update(&info->actwsSha, counterBuffer, 4u, watchdog);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      (void)actSHA256Update(&info->actwsSha, secret, secretLength, watchdog);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      if (addInputLength > 0u)
      {
         /* If addInputLength > 0u, then addInput != NULL_PTR, as this was checked in esl_deriveKeyHKDF_Hash_OneStep */
        (void)actSHA256Update(&info->actwsSha, addInput, addInputLength, watchdog);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }

      /* not last block or complete last block required? */
      if ((currentBlock != numberOfBlocks) || (0u == remainingBytes))
      {
        /* complete (last) block; actSHA256Finalize always returns actOK */
        (void)actSHA256Finalize(&info->actwsSha, &derivedKey[actHASH_SIZE_SHA256 * (currentBlock - 1u)], watchdog);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }
      else
      {
        /* partial last block; actSHA256Finalize always returns actOK */
        (void)actSHA256Finalize(&info->actwsSha, info->blockT, watchdog);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        actMemCpyByteArray(&derivedKey[actHASH_SIZE_SHA256 * (currentBlock - 1u)], info->blockT, remainingBytes);  /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS */
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

#endif /* (VSECPRIM_ACTHKDFHASHONESTEP_ENABLED == STD_ON) */
