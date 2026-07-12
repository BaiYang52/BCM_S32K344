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
/*!        \file  ESLib_HashMACSHA1.c
 *        \brief  Hash MAC SHA-1 implementation.
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

#define ESLIB_HASHMACSHA1_SOURCE

#include "ESLib.h"
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIHashMAC.h"

#if (VSECPRIM_HMAC_SHA1_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  esl_initHashMACSHA1()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_initHashMACSHA1(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACSHA1) workSpace, const eslt_Length keyLength, VSECPRIM_P2CONST_PARA(eslt_Byte) key)
{
  eslt_ErrorCode retVal;

  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (key == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* Check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_HMACSHA1)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  /* check keyLength */
  else if (keyLength == 0u)
  {
    retVal = ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE;
  }
  else
  {
    /* init actCLib HMAC-SHA-1 */
    if (actOK == actHashMACInit(&workSpace->wsHMACSHA1, key, (actLengthType)keyLength, workSpace->header.watchdog))
    {
      /* Set workSpace state */
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_HMACSHA1);
    }
    else
    {
      retVal = ESL_ERC_ERROR;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_updateHashMACSHA1()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateHashMACSHA1(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACSHA1) workSpace, const eslt_Length inputLength, VSECPRIM_P2CONST_PARA(eslt_Byte) input) /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
{
  eslt_ErrorCode retVal;

  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (input == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* Check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_HMACSHA1, ESL_WST_ALGO_HMACSHA1)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  else
  {
    /* Update HMAC calculation */
    if (actEXCEPTION_LENGTH == actHashMACUpdate(&workSpace->wsHMACSHA1, input, (actLengthType)inputLength, workSpace->header.watchdog))
    {
      retVal = ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW;
    }
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  esl_finalizeHashMACSHA1()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashMACSHA1(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACSHA1) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageHashMAC)
{
  eslt_ErrorCode retVal;

  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (messageHashMAC == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* Check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_HMACSHA1, ESL_WST_ALGO_HMACSHA1)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  else
  {
    /* finalize actCLib HMAC-SHA-1 */
    if (actOK != actHashMACFinalize(&workSpace->wsHMACSHA1, messageHashMAC, workSpace->header.watchdog))
    {
      retVal = ESL_ERC_ERROR;
    }

    /* Reset and clear workspace */
    esl_ResetAndClearWorkspace(&workSpace->header, &(workSpace->wsHMACSHA1)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  esl_verifyHashMACSHA1()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyHashMACSHA1(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACSHA1) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) messageHashMAC)
{
  eslt_Byte tmpMAC[ESL_SIZEOF_SHA1_DIGEST];
  eslt_ErrorCode retVal;

  actMemClear(tmpMAC, ESL_SIZEOF_SHA1_DIGEST); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (messageHashMAC == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* Check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_HMACSHA1, ESL_WST_ALGO_HMACSHA1)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  else
  {
    /* Verify MAC */
    if (ESL_ERC_NO_ERROR == esl_finalizeHashMACSHA1(workSpace, tmpMAC))
    {
      if (actMemcmp(tmpMAC, messageHashMAC, ESL_SIZEOF_SHA1_DIGEST) != TRUE) /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
      {
        retVal = ESL_ERC_HMAC_INCORRECT_MAC;
      }
    }
    else
    {
      retVal = ESL_ERC_ERROR;
    }
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_HMAC_SHA1_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_HashMACSHA1.c
 *********************************************************************************************************************/
