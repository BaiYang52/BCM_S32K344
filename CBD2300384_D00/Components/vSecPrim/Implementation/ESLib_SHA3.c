/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2020 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  ESLib_SHA3.c
 *        \brief  SHA-3 implementation.
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

#define ESLIB_SHA1_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ESLib.h"
#include "ESLib_Helper.h"

#include "actISHA3.h"

#if (VSECPRIM_SHA3_ENABLED == STD_ON)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  esl_initSHA3()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initSHA3(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA3) workSpace,
  const eslt_Length          bitLength)
{
  eslt_ErrorCode  retVal;

  /* validate parameters */
  if (workSpace == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* validate work space */
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_SHA3)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  /* validate bit length */
  else if ((bitLength != 224u) && (bitLength != 256u) && (bitLength != 384u) && (bitLength != 512u))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    /* initialize actCLib SHA3 */
    /* ALL possible error conditions have been checked before - it's safe to ignore the return value */
    (void)actSHA3Init (&workSpace->wsSHA3, bitLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* set workSpace state; retVal already set */
    esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_SHA3); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_updateSHA3()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSHA3(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA3) workSpace,
  const  eslt_Length          inputSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input)
{
  eslt_ErrorCode  retVal;
  actRETURNCODE actRetVal;

    /* validate parameters */
  if ((workSpace == NULL_PTR) || (input == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_SHA3, ESL_WST_ALGO_SHA3)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    /* update actCLib SHA3 */
    actRetVal = actSHA3Update (&workSpace->wsSHA3, input, (actLengthType)inputSize, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */

    if (actRetVal == actEXCEPTION_MODE)
    {
      retVal = ESL_ERC_STATE_INVALID;
    }
    /* If actSHA3Update does not return actEXCEPTION_MODE, it returns actOK.
     * In this case, retVal = ESL_ERC_NO_ERROR is already set. */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_finalizeSHA3()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSHA3(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA3) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest,
  VSECPRIM_P2VAR_PARA(eslt_Length) messageDigestLength)
{
  eslt_ErrorCode  retVal;

  /* validate parameters */
  if ((workSpace == NULL_PTR) || (messageDigest == NULL_PTR) || (messageDigestLength == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_SHA3, ESL_WST_ALGO_SHA3)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    if (*messageDigestLength >= (eslt_Length)(Esl_BitsToBytes(workSpace->wsSHA3.strength)))
    {
      /* finalize actCLib SHA3 */
      /* always returns actOK - it's safe to ignore the return value */
      (void)actSHA3Finalize(&workSpace->wsSHA3, messageDigest, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      *messageDigestLength = (eslt_Length)(Esl_BitsToBytes(workSpace->wsSHA3.strength)); /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    else
    {
      /* finalize actCLib SHA3 */
      /* always returns actOK - it's safe to ignore the return value */
      (void)actSHA3Finalize(&workSpace->wsSHA3, workSpace->wsSHA3.digest, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      actMemCpyByteArray(messageDigest, workSpace->wsSHA3.digest, *messageDigestLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }

    /* reset and clear workspace; retVal already set */
    esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsSHA3); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_SHA3_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_SHA3.c
 *********************************************************************************************************************/
