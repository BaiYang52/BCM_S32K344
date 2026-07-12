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
/*!        \file  ESLib_AES128dec.c
 *        \brief  AES-128 (decryption) implementation.
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

#define ESLIB_AES128DEC_SOURCE

#include "ESLib_AES_common.h"

#if (VSECPRIM_AES128_ENABLED == STD_ON)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/**********************************************************************************************************************
 *  esl_initDecryptAES128()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptAES128(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES128) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key,
                                                    const eslt_BlockMode blockMode, const eslt_PaddingMode paddingMode, VSECPRIM_P2CONST_PARA(eslt_Byte) initializationVector)
{
  actU8 targetMode = actPADDING_PM_OFF;
  eslt_ErrorCode retVal = esl_AES_CommonInit(&targetMode, workSpace, ESL_MAXSIZEOF_WS_AES128, key, blockMode, paddingMode); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FUNCTION_CALL_POINTER_CHECKED_WITHIN */

  if (retVal == ESL_ERC_NO_ERROR)
  {
    /* Init actCLib AES128 */
    if (actOK == actAESInitDecrypt(&workSpace->wsAES, key, ESL_SIZEOF_AES128_KEY, initializationVector, targetMode, workSpace->header.watchdog)) /* SBSW_VSECPRIM_CALL_FUNCTION */
    {
      /* Set workSpace state */
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_AES128); /* SBSW_VSECPRIM_CALL_FUNCTION */
    }
    else
    {
      retVal = ESL_ERC_INPUT_INVALID;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_decryptAES128()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptAES128(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES128) workSpace,
                                                const eslt_Length inputSize,
                                                VSECPRIM_P2CONST_PARA(eslt_Byte) input,
                                                VSECPRIM_P2VAR_PARA(eslt_Length) outputSize,
                                                VSECPRIM_P2VAR_PARA(eslt_Byte) output)
{
  eslt_ErrorCode retVal = esl_AES_CommonProcChecks(workSpace, ESL_WST_ALGO_AES128, ESL_MAXSIZEOF_WS_AES128, input, output); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FUNCTION_CALL_POINTER_CHECKED_WITHIN */
  /* Perform parameter checks */
  if (retVal != ESL_ERC_NO_ERROR)
  {
    /* retVal was already set */
  }
  /* explicit check for ESL_MAXSIZEOF_WS_AES128, if inputSize is not aligned to ESL_SIZEOF_AES128_BLOCK */
  else if (outputSize == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    /* Update actCLib AES128 */
    actRETURNCODE result;
    actLengthType dec_len = *outputSize;

    result = actAESDecrypt(&workSpace->wsAES, input, (actLengthType)inputSize, output, &dec_len, FALSE, workSpace->header.watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

    if (result == actEXCEPTION_LENGTH)
    {
      retVal = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
    }
    else
    {
      *outputSize = (eslt_Length)dec_len; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  esl_finalizeDecryptAES128()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeDecryptAES128(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES128) workSpace, VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output)
{
  eslt_ErrorCode retVal;
  if ((workSpace == NULL_PTR) || (outputSize == NULL_PTR) || (output == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_AES128, ESL_WST_ALGO_AES128)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    actRETURNCODE result;
    actLengthType dec_len = *outputSize;

    result = actAESDecrypt(&workSpace->wsAES, NULL_PTR, 0, output, &dec_len, TRUE, workSpace->header.watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

    if (result == actEXCEPTION_LENGTH)
    {
      retVal = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
    }
    else if (result == actEXCEPTION_INPUT_LENGTH)
    {
      retVal = ESL_ERC_INPUT_INVALID; /* The total input length is not zero mod blocksize */
    }
    else if (result == actEXCEPTION_PADDING)
    {
      retVal = ESL_ERC_AES_PADDING_INVALID;
    }
    else
    {
      *outputSize = (eslt_Length)dec_len; /* SBSW_VSECPRIM_WRITE_POINTER */

      esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsAES); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    }
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  esl_initDecryptAES128Block()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptAES128Block(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAES128Block) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key)
{
  eslt_ErrorCode retVal = esl_AES_CommonInitChecks(workSpace, sizeof(workSpace->wsAES.e_key), key); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FUNCTION_CALL_POINTER_CHECKED_WITHIN */

  if (retVal == ESL_ERC_NO_ERROR)
  {
    /* Init actCLib AES128 */
    if (actOK == actAESInitDecryptBlock(&workSpace->wsAES, key, ESL_SIZEOF_AES128_KEY, workSpace->header.watchdog)) /* SBSW_VSECPRIM_CALL_FUNCTION */
    {
      /* Set workSpace state */
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_AES128); /* SBSW_VSECPRIM_CALL_FUNCTION */
    }
    else
    {
      retVal = ESL_ERC_INPUT_INVALID;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_decryptAES128Block()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptAES128Block(VSECPRIM_P2CONST_PARA(eslt_WorkSpaceAES128Block) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) inputBlock, VSECPRIM_P2VAR_PARA(eslt_Byte) outputBlock)
{
  eslt_ErrorCode retVal = esl_AES_CommonProcChecks(workSpace, ESL_WST_ALGO_AES128, sizeof(workSpace->wsAES.e_key), inputBlock, outputBlock); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FUNCTION_CALL_POINTER_CHECKED_WITHIN */

  if (retVal == ESL_ERC_NO_ERROR)
  {
    /* Update actCLib AES128 */
    actAESDecryptBlock(&workSpace->wsAES, inputBlock, outputBlock, workSpace->header.watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }
  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_AES128_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_AES128dec.c
 *********************************************************************************************************************/
