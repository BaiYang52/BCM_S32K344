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
/*!        \file  ESLib_RMD160.c
 *        \brief  RIPEMD-160 implementation.
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

#define ESLIB_RMD160_SOURCE

#include "ESLib.h"
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIRMD160.h"

#if (VSECPRIM_RIPEMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 * esl_initRIPEMD160
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initRIPEMD160(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRIPEMD160) workSpace)
{
  eslt_ErrorCode retVal;

  /* Any NULLs ? */
  if (workSpace == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MINSIZEOF_WS_RIPEMD160)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  else
  {
    /* Init actCLib RIPEMD-160; actRMD160Init always returns actOK */
    (void)actRMD160Init(&workSpace->wsRIPEMD160);
    /* Set workSpace state */
    workSpace->header.status = (ESL_WST_ALGO_RIPEMD160 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);
  }

  return retVal;
}

/****************************************************************************
 * esl_updateRIPEMD160
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateRIPEMD160(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRIPEMD160) workSpace, const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input)
{
  eslt_ErrorCode retVal;

  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (input == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* Check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_RIPEMD160, ESL_WST_ALGO_RIPEMD160)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  /* Update actCLib RIPEMD-160 */
  else
  {
    actRETURNCODE result = actRMD160Update(&workSpace->wsRIPEMD160,
                                           input, inputSize, workSpace->header.watchdog);
    if (result == actEXCEPTION_LENGTH)
    {
      retVal = ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW;
    }
  }

  return retVal;
}

/****************************************************************************
 * esl_finalizeRIPEMD160
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeRIPEMD160(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRIPEMD160) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest)
{
  eslt_ErrorCode retVal;

  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (messageDigest == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* Check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_RIPEMD160, ESL_WST_ALGO_RIPEMD160)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  else
  {
    /* Finalize actCLib RIPEMD-160; actRMD160Finalize always returns actOK */
    (void)actRMD160Finalize(&workSpace->wsRIPEMD160, messageDigest, workSpace->header.watchdog);
    /* Reset and clear workspace */
    esl_ResetAndClearWorkspace(&workSpace->header, &(workSpace->wsRIPEMD160)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
  }

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_RIPEMD160_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_RMD160.c
 *********************************************************************************************************************/
