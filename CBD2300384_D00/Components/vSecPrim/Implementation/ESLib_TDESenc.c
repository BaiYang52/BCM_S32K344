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
/*!        \file  ESLib_TDESenc.c
 *        \brief  TDES (encryption) implementation.
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

#define ESLIB_TDESENC_SOURCE

#include "ESLib.h"
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actTDES.h"

#if (VSECPRIM_TDES_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/* PRQA S 6060 EOF */ /* MD_VSECPRIM_QM_ONLY_METRIC */
/* PRQA S 2889 EOF */ /* MD_VSECPRIM_QM_ONLY_RETURN_PATHS */
/* PRQA S 3200 EOF */ /* MD_VSECPRIM_QM_ONLY_UNUSED_FUNCTION */
/* PRQA S 0310, 3305 EOF */ /* MD_VSECPRIM_QM_ONLY_POINTER_CAST_TO_LARGER_TYPE */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/****************************************************************************
 * esl_initEncryptTDES
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptTDES(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceTDES) workSpace,
                                                  VSECPRIM_P2CONST_PARA(eslt_Byte) key, const eslt_Byte key_len,
                                                  const eslt_BlockMode blockMode, const eslt_PaddingMode paddingMode, VSECPRIM_P2CONST_PARA(eslt_Byte) initializationVector)
{
  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (key == NULL_PTR))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* Check key_len */
  if ((key_len != ESL_SIZEOF_2TDES_KEY) && (key_len != ESL_SIZEOF_3TDES_KEY))
  {
    return ESL_ERC_TDES_KEY_LENGTH_INVALID;
  }

  /* Check workSpace */
  if (workSpace->header.size < ESL_MINSIZEOF_WS_TDES)
  {
    return ESL_ERC_WS_TOO_SMALL;
  }

  /* Check paddingMode */
  if (paddingMode != ESL_PM_PKCS5)
  {
    return ESL_ERC_MODE_INVALID;
  }

  /* Init actCLib TDES */
  if (blockMode == ESL_BM_ECB)
  {
    actTDESInitEncrypt((VSECPRIM_P2VAR_PARA(actTDESSTRUCT)) workSpace->wsTDES, key, (actU8)key_len, initializationVector, actDES_ECB, workSpace->header.watchdog);
  }
  else if (blockMode == ESL_BM_CBC)
  {
    actTDESInitEncrypt((VSECPRIM_P2VAR_PARA(actTDESSTRUCT)) workSpace->wsTDES, key, (actU8)key_len, initializationVector, actDES_CBC, workSpace->header.watchdog);
  }
  else
  {
    return ESL_ERC_MODE_INVALID;
  }

  /* Set workSpace state */
  workSpace->header.status = (ESL_WST_ALGO_TDES | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

  return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 * esl_encryptTDES
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptTDES(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceTDES) workSpace,
                                              const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input, VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output)
{
  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (input == NULL_PTR) || (output == NULL_PTR))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* Check workSpace */
  if ((workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_TDES)
  {
    return ESL_ERC_WS_STATE_INVALID;
  }
  if ((workSpace->header.status & ESL_WST_M_RUNNING) == 0u)
  {
    return ESL_ERC_WS_STATE_INVALID;
  }
  if ((inputSize % ESL_SIZEOF_DES_BLOCK) == 0u)
  {
    if (workSpace->header.size < ESL_MINSIZEOF_WS_TDES)
    {
      return ESL_ERC_WS_TOO_SMALL;
    }
  }
  else
  {
    if (workSpace->header.size < ESL_MAXSIZEOF_WS_TDES)
    {
      return ESL_ERC_WS_TOO_SMALL;
    }
  }

  /* Update actCLib TDES */
  {
    actRETURNCODE result;
    actLengthType enc_len = *outputSize;

    result = actTDESEncrypt((VSECPRIM_P2VAR_PARA(actTDESSTRUCT)) workSpace->wsTDES, input, (actLengthType)inputSize, output, &enc_len, FALSE, workSpace->header.watchdog);
    if (result == actEXCEPTION_LENGTH)
    {
      return ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
    }
    else
    {
      *outputSize = (eslt_Length)enc_len;
    }
  }

  return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 * esl_finalizeEncryptTDES
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeEncryptTDES(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceTDES) workSpace, VSECPRIM_P2VAR_PARA(eslt_Length) outputSize, VSECPRIM_P2VAR_PARA(eslt_Byte) output)
{
  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (output == NULL_PTR))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* Check workSpace */
  if ((workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_TDES)
  {
    return ESL_ERC_WS_STATE_INVALID;
  }
  if ((workSpace->header.status & ESL_WST_M_RUNNING) == 0u)
  {
    return ESL_ERC_WS_STATE_INVALID;
  }
  if (workSpace->header.size < ESL_MAXSIZEOF_WS_TDES)
  {
    return ESL_ERC_WS_TOO_SMALL;
  }

  /* Update actCLib TDES */
  {
    actRETURNCODE result;
    actLengthType enc_len = *outputSize;

    result = actTDESEncrypt((VSECPRIM_P2VAR_PARA(actTDESSTRUCT)) workSpace->wsTDES, (VSECPRIM_P2CONST_PARA(actU8)) NULL_PTR, 0u, output, &enc_len, TRUE, workSpace->header.watchdog);
    if (result == actEXCEPTION_LENGTH)
    {
      return ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
    }
    else
    {
      *outputSize = (eslt_Length)enc_len;
    }
  }

  /* Reset and clear workspace */
  esl_ResetAndClearWorkspace(&workSpace->header, workSpace->wsTDES); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

  return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 * esl_initEncryptTDESBlock
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptTDESBlock(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceTDESBlock) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) key, const eslt_Byte key_len)
{
  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (key == NULL_PTR))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* Check key_len */
  if ((key_len != ESL_SIZEOF_2TDES_KEY) && (key_len != ESL_SIZEOF_3TDES_KEY))
  {
    return ESL_ERC_TDES_KEY_LENGTH_INVALID;
  }

  /* Check workSpace */
  if (workSpace->header.size < ESL_MAXSIZEOF_WS_TDESBLOCK)
  {
    return ESL_ERC_WS_TOO_SMALL;
  }

  /* Init actCLib TDES */
  actTDESInitEncryptBlock((VSECPRIM_P2VAR_PARA(actTDESSTRUCT)) workSpace->wsTDESBlock, key, (actU8)key_len, workSpace->header.watchdog);

  /* Set workSpace state */
  workSpace->header.status = (ESL_WST_ALGO_TDES | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

  return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 * esl_encryptTDESBlock
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptTDESBlock(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceTDESBlock) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) inputBlock, VSECPRIM_P2VAR_PARA(eslt_Byte) outputBlock)
{
  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (inputBlock == NULL_PTR) || (outputBlock == NULL_PTR))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* Check workSpace */
  if ((workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_TDES)
  {
    return ESL_ERC_WS_STATE_INVALID;
  }
  if ((workSpace->header.status & ESL_WST_M_RUNNING) == 0u)
  {
    return ESL_ERC_WS_STATE_INVALID;
  }
  if (workSpace->header.size < ESL_MAXSIZEOF_WS_TDESBLOCK)
  {
    return ESL_ERC_WS_TOO_SMALL;
  }

  /* Update actCLib TDES */
  actTDESCryptBlock((VSECPRIM_P2VAR_PARA(actTDESSTRUCT)) workSpace->wsTDESBlock, inputBlock, outputBlock, workSpace->header.watchdog);

  return ESL_ERC_NO_ERROR;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_TDES_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_TDESenc.c
 *********************************************************************************************************************/
