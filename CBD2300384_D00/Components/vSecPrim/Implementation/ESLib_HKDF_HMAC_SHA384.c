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
/*!        \file  ESLib_HKDF_HMAC_SHA384.c
 *        \brief  HKDF HMAC SHA384 two step key derivation function according to NIST SP800-56C using HMAC SHA-384.
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

#define ESLIB_HKDFHMACSHA384_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ESLib.h"
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIHKDF_HMAC.h"

#if (VSECPRIM_HKDF_HMAC_SHA384_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  esl_initHKDFHMACSHA384()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initHKDF_HMAC_SHA384(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHKDFHMAC) workSpace,
  eslt_HKDFMode mode)
{
  eslt_ErrorCode retVal;
  /* validate parameters */
  if (workSpace == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_HKDF_HMAC)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal was already set */
  }
  else if ((mode != ESL_HKDF_ONE_STEP_VARIANT) && (mode != ESL_HKDF_TWO_STEP_VARIANT))
  {
    retVal = ESL_ERC_MODE_INVALID;
  }
  else
  {
    /* set workSpace mode and state */
    workSpace->wsHKDF.methodID = mode; /* SBSW_VSECPRIM_WRITE_POINTER */
    esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_HKDF_HMAC_SHA384); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }
  return retVal;
}

/***********************************************************************************************************************
 *  esl_deriveKeyHKDFHMACSHA384
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_deriveKeyHKDF_HMAC_SHA384(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHKDFHMAC) workSpace,
  const eslt_Length secretLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) secret,
  const eslt_Length saltLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
  const eslt_Length infoLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) info,
  const eslt_Length keyLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) key)
{
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;

  /* validate parameters */
  if ((workSpace == NULL_PTR) || (secret == NULL_PTR) || ((salt == NULL_PTR) && (saltLength > 0u)) || ((info == NULL_PTR) && (infoLength > 0u)) || (key == NULL_PTR) )
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* check length */
  else if ((secretLength == 0u) || (keyLength == 0u))
  {
    retVal = ESL_ERC_INPUT_INVALID;
  }
  /* check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_HKDF_HMAC, ESL_WST_ALGO_HKDF_HMAC_SHA384)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    if (workSpace->wsHKDF.methodID == ESL_HKDF_ONE_STEP_VARIANT)
    {
      /* derive key with actCLib HKDF HMAC SHA-384 one-step method */
      actRetVal = actHKDF_HMAC_OneStepDerive(&workSpace->wsHKDF, secret, secretLength, salt, saltLength, info, infoLength, key, keyLength, actHASHMACSHA384_ALGO_ID, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
    else
    {
      /* derive key with actCLib HKDF HMAC SHA-384 two-step method */
      actHKDF_HMAC_Extract(&workSpace->wsHKDF, secret, (actLengthType)secretLength, salt, (actLengthType)saltLength, actHASHMACSHA384_ALGO_ID, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      actRetVal = actHKDF_HMAC_Expand(&workSpace->wsHKDF, info, (actLengthType)infoLength, key, (actLengthType)keyLength, actHASHMACSHA384_ALGO_ID, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }

    if (actRetVal == actEXCEPTION_LENGTH)
    {
      retVal = ESL_ERC_INVALID_LENGTH;
    }
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* VSECPRIM_HKDF_HMAC_SHA384_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_HKDF_HMAC_SHA384.c
 *********************************************************************************************************************/

