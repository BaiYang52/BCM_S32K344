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
/*!        \file  ESLib_RC2enc.c
 *        \brief  RC2 (encyrption) implementation.
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

#define ESLIB_RC2ENC_SOURCE

#include "ESLib.h"
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actRC2.h"

#if (VSECPRIM_RC2_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/* PRQA S 6010, 6030, 6060 EOF */ /* MD_VSECPRIM_QM_ONLY_METRIC */
/* PRQA S 2889 EOF */ /* MD_VSECPRIM_QM_ONLY_RETURN_PATHS */
/* PRQA S 3200 EOF */ /* MD_VSECPRIM_QM_ONLY_UNUSED_FUNCTION */
/* PRQA S 0310, 3305 EOF */ /* MD_VSECPRIM_QM_ONLY_POINTER_CAST_TO_LARGER_TYPE */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/****************************************************************************
 * esl_initEncryptRC2
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptRC2(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRC2) workSpace,
                                                 VSECPRIM_P2CONST_PARA(eslt_Byte) key, const eslt_Length key_len, const eslt_Length eff_key_len,
                                                 const eslt_BlockMode blockMode, const eslt_PaddingMode paddingMode, VSECPRIM_P2CONST_PARA(eslt_Byte) initializationVector)
{
  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (key == NULL_PTR))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* Check workSpace */
  if (workSpace->header.size < ESL_MINSIZEOF_WS_RC2)
  {
    return ESL_ERC_WS_TOO_SMALL;
  }

  /* Check paddingMode */
  if (paddingMode != ESL_PM_PKCS5)
  {
    return ESL_ERC_MODE_INVALID;
  }

  /* Check key_len */
  if (key_len < actRC2_MIN_KEY_SIZE)
  {
    return ESL_ERC_INPUT_INVALID;
  }

  /* Init actCLib RC2 */
  if (blockMode == ESL_BM_ECB)
  {
    actRC2Init((VSECPRIM_P2VAR_PARA(actRC2STRUCT)) workSpace->wsRC2, key, (actLengthType)key_len, (actLengthType)eff_key_len, initializationVector, actRC2_ECB, workSpace->header.watchdog);
  }
  else if (blockMode == ESL_BM_CBC)
  {
    actRC2Init((VSECPRIM_P2VAR_PARA(actRC2STRUCT)) workSpace->wsRC2, key, (actLengthType)key_len, (actLengthType)eff_key_len, initializationVector, actRC2_CBC, workSpace->header.watchdog);
  }
  else
  {
    return ESL_ERC_MODE_INVALID;
  }

  /* Set workSpace state */
  workSpace->header.status = (ESL_WST_ALGO_RC2 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

  return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 * esl_encryptRC2
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRC2(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRC2) workSpace,
                                             const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input, VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output)
{
  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (input == NULL_PTR) || (output == NULL_PTR))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* Check workSpace */
  if ((workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_RC2)
  {
    return ESL_ERC_WS_STATE_INVALID;
  }
  if ((workSpace->header.status & ESL_WST_M_RUNNING) == 0u)
  {
    return ESL_ERC_WS_STATE_INVALID;
  }
  if ((inputSize % ESL_SIZEOF_RC2_BLOCK) == 0u)
  {
    if (workSpace->header.size < ESL_MINSIZEOF_WS_RC2)
    {
      return ESL_ERC_WS_TOO_SMALL;
    }
  }
  else
  {
    if (workSpace->header.size < ESL_MAXSIZEOF_WS_RC2)
    {
      return ESL_ERC_WS_TOO_SMALL;
    }
  }

  /* Update actCLib RC2 */
  {
    actRETURNCODE result;
    actLengthType enc_len = *outputSize;

    result = actRC2Encrypt((VSECPRIM_P2VAR_PARA(actRC2STRUCT)) workSpace->wsRC2, input, (actLengthType)inputSize, output, &enc_len, FALSE, workSpace->header.watchdog);
    if (result == actEXCEPTION_LENGTH)
    {
      return ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
    }
    else
    {
      *outputSize = (eslt_Length)(enc_len);
    }
  }

  return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 * esl_finalizeEncryptRC2
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeEncryptRC2(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRC2) workSpace, VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output)
{
  actRETURNCODE result;
  actLengthType enc_len;

  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (output == NULL_PTR))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* Check workSpace */
  if ((workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_RC2)
  {
    return ESL_ERC_WS_STATE_INVALID;
  }
  if ((workSpace->header.status & ESL_WST_M_RUNNING) == 0u)
  {
    return ESL_ERC_WS_STATE_INVALID;
  }
  if (workSpace->header.size < ESL_MAXSIZEOF_WS_RC2)
  {
    return ESL_ERC_WS_TOO_SMALL;
  }

  enc_len = *outputSize;

  /* Update actCLib RC2 */
  result = actRC2Encrypt((VSECPRIM_P2VAR_PARA(actRC2STRUCT)) workSpace->wsRC2, (VSECPRIM_P2CONST_PARA(eslt_Byte)) NULL_PTR, 0u, output, &enc_len, TRUE, workSpace->header.watchdog);
  if (result == actEXCEPTION_LENGTH)
  {
    return ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
  }
  else
  {
    *outputSize = (eslt_Length)enc_len;
  }

  /* Reset and clear workspace */
  esl_ResetAndClearWorkspace(&workSpace->header, workSpace->wsRC2); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

  return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 * esl_initEncryptRC2Block
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptRC2Block(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRC2Block) workSpace,
                                                      VSECPRIM_P2CONST_PARA(eslt_Byte) key, const eslt_Length key_len, const eslt_Length eff_key_len)
{
  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (key == NULL_PTR))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* Check workSpace */
  if (workSpace->header.size < ESL_MAXSIZEOF_WS_RC2BLOCK)
  {
    return ESL_ERC_WS_TOO_SMALL;
  }

  /* Check key_len */
  if (key_len < actRC2_MIN_KEY_SIZE)
  {
    return ESL_ERC_INPUT_INVALID;
  }

  /* Init actCLib RC2 */
  actRC2InitCryptBlock((VSECPRIM_P2VAR_PARA(actRC2STRUCT)) workSpace->wsRC2Block, key, (actLengthType)key_len, (actLengthType)eff_key_len, workSpace->header.watchdog);

  /* Set workSpace state */
  workSpace->header.status = (ESL_WST_ALGO_RC2 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

  return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 * esl_encryptRC2Block
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRC2Block(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRC2Block) workSpace, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2CONST_PARA(eslt_Byte) inputBlock, VSECPRIM_P2VAR_PARA(eslt_Byte) outputBlock)
{
  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (inputBlock == NULL_PTR) || (outputBlock == NULL_PTR))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* Check workSpace */
  if ((workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_RC2)
  {
    return ESL_ERC_WS_STATE_INVALID;
  }
  if ((workSpace->header.status & ESL_WST_M_RUNNING) == 0u)
  {
    return ESL_ERC_WS_STATE_INVALID;
  }
  if (workSpace->header.size < ESL_MAXSIZEOF_WS_RC2BLOCK)
  {
    return ESL_ERC_WS_TOO_SMALL;
  }

  /* Update actCLib RC2 */
  actRC2EncryptBlock((VSECPRIM_P2VAR_PARA(actRC2STRUCT)) workSpace->wsRC2Block, inputBlock, outputBlock, workSpace->header.watchdog);

  return ESL_ERC_NO_ERROR;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_RC2_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RC2enc.c
 *********************************************************************************************************************/
