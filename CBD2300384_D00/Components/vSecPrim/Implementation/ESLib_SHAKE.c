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
/*!        \file  ESLib_SHAKE.c
 *        \brief  SHA-3 XOFs implementation.
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

#define ESLIB_SHAKE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ESLib.h"
#include "ESLib_Helper.h"

#include "actISHAKE.h"

#if (VSECPRIM_SHA3_ENABLED == STD_ON)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

 /**********************************************************************************************************************
 *  esl_initSHAKE()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initSHAKE(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHAKE) workSpace,
  const eslt_Length strength)
{
  eslt_ErrorCode  retVal;

  /* validate parameters */
  if (workSpace == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_SHAKE)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else if ((strength != 128u) && (strength != 256u))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    /* initialize actCLib SHAKE
     * ALL possible error conditions have been checked before - it's safe to ignore the return value */
    (void)actSHAKEInit(&workSpace->wsSHAKE, strength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* set workSpace state; retVal already set */
    esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_SHAKE); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_updateSHAKE()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSHAKE(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHAKE) workSpace,
const  eslt_Length inputSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) input)
{
  eslt_ErrorCode  retVal;
  actRETURNCODE actRetVal;

  /* validate parameters */
  if ((workSpace == NULL_PTR) || (input == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_SHAKE, ESL_WST_ALGO_SHAKE)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    /* update actCLib SHAKE */
    actRetVal = actSHAKEUpdate(&workSpace->wsSHAKE, input, (actLengthType)inputSize, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */

    if (actRetVal == actEXCEPTION_MODE)
    {
      retVal = ESL_ERC_STATE_INVALID;
    }
    /* If actSHAKEUpdate does not return actEXCEPTION_MODE, it returns actOK.
     * In this case, retVal = ESL_ERC_NO_ERROR is already set. */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_getXOFDigestSHAKE()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_getXOFDigestSHAKE(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHAKE) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest,
const eslt_Length digestLength)
{
  eslt_ErrorCode  retVal;
  actRETURNCODE actRetVal;

  /* validate parameters */
  if ((workSpace == NULL_PTR) || (messageDigest == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_SHA3, ESL_WST_ALGO_SHA3)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    /* finalize actCLib ParallelHash */
    actRetVal = actSHAKEGetXOFDigest (&workSpace->wsSHAKE, messageDigest, (actLengthType)digestLength, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actRetVal == actEXCEPTION_LENGTH)
    {
      retVal = ESL_ERC_INVALID_LENGTH;
    }
    /* If actSHAKEGetXOFDigest does not return actEXCEPTION_LENGTH, it returns actOK.
     * In this case, retVal = ESL_ERC_NO_ERROR is already set. */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_finalizeSHAKE()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSHAKE(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHAKE) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest,
VSECPRIM_P2VAR_PARA(eslt_Length) messageDigestLength)
{
  eslt_ErrorCode  retVal;

  /* validate parameters */
  if ((workSpace == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_SHA3, ESL_WST_ALGO_SHA3)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    if (messageDigest != NULL_PTR)
    {
      if (messageDigestLength == NULL_PTR)
      {
        retVal = ESL_ERC_PARAMETER_INVALID;
      }
      else
      {
        if (*messageDigestLength >= (eslt_Length)(Esl_BitsToBytes(2u * workSpace->wsSHAKE.strength)))
        {
          /* always returns actOK - it's safe to ignore the return value */
          (void)actSHAKEFinalize(&workSpace->wsSHAKE, messageDigest, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */
          *messageDigestLength = (eslt_Length)(Esl_BitsToBytes(2u * workSpace->wsSHAKE.strength)); /* SBSW_VSECPRIM_WRITE_POINTER */
        }
        else
        {
          /* always returns actOK - it's safe to ignore the return value */
          (void)actSHAKEFinalize(&workSpace->wsSHAKE, workSpace->wsSHAKE.digest, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */
          actMemCpyByteArray(messageDigest, workSpace->wsSHAKE.digest, *messageDigestLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        }
      }
    }

    if (retVal == ESL_ERC_NO_ERROR)
    {
      /* reset and clear workspace; retVal already set */
      esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsSHAKE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    }
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_SHA3_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_SHAKE.c
 *********************************************************************************************************************/
