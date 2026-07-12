 /**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2017 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  ESLib_AEAD_ChaCha20_Poly1305.c
 *        \brief  AEAD_ChaCha20_Poly1305 (decryption) implementation.
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

/****************************************************************************
 **
 ** AEAD_ChaCha20_Poly1305 (RFC 7539) interface implementation.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   esl_initEncryptChaChaPoly
 **   esl_initDecryptChaChaPoly
 **   esl_updateAADChaChaPoly
 **   esl_updateDataChaChaPoly
 **   esl_finalizeChaChaPoly
 **   esl_verifyChaChaPoly
 **
 ***************************************************************************/

#define ESLIB_AEAD_CHACHA20_POLY1305_SOURCE
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIAEAD7539.h"

#if (VSECPRIM_AEAD_CHA_CHA20_POLY1305_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/* PRQA S 2889 EOF */ /* MD_VSECPRIM_QM_ONLY_RETURN_PATHS */
/* PRQA S 0310, 3305 EOF */ /* MD_VSECPRIM_QM_ONLY_CASTING_POINTER_TYPE */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** Local Functions
 ***************************************************************************/
VSECPRIM_LOCAL VSECPRIM_FUNC( eslt_ErrorCode ) initChaChaPoly (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceChaChaPoly ) workSpace,
                                                       VSECPRIM_P2CONST_PARA( eslt_Byte ) key,
                                                       VSECPRIM_P2CONST_PARA( eslt_Byte ) nonce,
                                                       const eslt_Byte mode);

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/****************************************************************************
 * esl_initEncryptChaChaPoly
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC( eslt_ErrorCode ) esl_initEncryptChaChaPoly (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceChaChaPoly ) workSpace,
                                                           VSECPRIM_P2CONST_PARA( eslt_Byte ) key,
                                                           VSECPRIM_P2CONST_PARA( eslt_Byte )nonce)
{

  return initChaChaPoly (workSpace, key, nonce, DIRECTION_ENCRYPT);
}

/****************************************************************************
 * esl_initDecryptChaChaPoly
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC( eslt_ErrorCode ) esl_initDecryptChaChaPoly (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceChaChaPoly ) workSpace,
                                                           VSECPRIM_P2CONST_PARA( eslt_Byte ) key,
                                                           VSECPRIM_P2CONST_PARA( eslt_Byte )nonce)
{

  return initChaChaPoly (workSpace, key, nonce, DIRECTION_DECRYPT);
}

/****************************************************************************
 * esl_updateAADChaChaPoly
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateAADChaChaPoly(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaChaPoly) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) aad,
  const eslt_Length aadSize)
{
  VSECPRIM_P2VAR_PARA(actAEAD7539Context) ctx;
  actRETURNCODE rc;

  /* validate parameters */
  if ((NULL == workSpace)
    || (NULL == aad))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* check workSpace state */
  if ((ESL_WST_ALGO_AEAD_7539 != (workSpace->header.status & ESL_WST_M_ALGO))
    || (ESL_WST_M_RUNNING != (workSpace->header.status & ESL_WST_M_RUNNING)))
  {
    return ESL_ERC_WS_STATE_INVALID;
  }

  /* verify that current phase IS "AAD" */
  ctx = (VSECPRIM_P2VAR_PARA(actAEAD7539Context)) workSpace->wsAEADChaChaPoly;
  if (PHASE_AAD != (ctx->mode & PHASE))
  {
    return ESL_ERC_WS_STATE_INVALID;
  }

  /* update actCLib AEAD7539 */
  rc = actAEAD7539Update(ctx, aad, aadSize, NULL, workSpace->header.watchdog);
  if (actOK != rc)
  {
    if (actEXCEPTION_LENGTH == rc)
    {
      return ESL_ERC_TOTAL_LENGTH_OVERFLOW;
    }
    if (actEXCEPTION_MODE == rc)
    {
      return  ESL_ERC_WS_STATE_INVALID;
    }
    return ESL_ERC_ERROR;
  }

  return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 * esl_updateDataChaChaPoly
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateDataChaChaPoly(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaChaPoly) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) in,
  const eslt_Length inSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte)out,
  VSECPRIM_P2VAR_PARA(eslt_Length) outSize)
{
  VSECPRIM_P2VAR_PARA(actAEAD7539Context) ctx;
  actRETURNCODE rc;

  /* validate parameters */
  if ((NULL == workSpace)
    || (NULL == in)
    || (NULL == out)
    || (NULL == outSize))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  if (inSize > *outSize)
  {
    return ESL_ERC_BUFFER_TOO_SMALL;
  }

  /* check workSpace state */
  if ((ESL_WST_ALGO_AEAD_7539 != (workSpace->header.status & ESL_WST_M_ALGO))
    || (ESL_WST_M_RUNNING != (workSpace->header.status & ESL_WST_M_RUNNING)))
  {
    return ESL_ERC_WS_STATE_INVALID;
  }

  /* switch to "data" phase */
  ctx = (VSECPRIM_P2VAR_PARA(actAEAD7539Context)) workSpace->wsAEADChaChaPoly;
  if (PHASE_DATA != (ctx->mode & PHASE))
  {
    actAEAD7539SwitchToData(ctx);
  }

  /* update actCLib AEAD7539 */
  rc = actAEAD7539Update(ctx, in, inSize, out, workSpace->header.watchdog);
  if (actOK != rc)
  {
    if (actEXCEPTION_LENGTH == rc)
    {
      return ESL_ERC_TOTAL_LENGTH_OVERFLOW;
    }
    if (actEXCEPTION_MODE == rc)
    {
      return ESL_ERC_WS_STATE_INVALID;
    }
    return ESL_ERC_ERROR;
  }

  *outSize = inSize;

  return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 * esl_finalizeChaChaPoly
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeChaChaPoly(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaChaPoly) workSpace,
                                                     VSECPRIM_P2VAR_PARA(eslt_Byte) tag)
{
  VSECPRIM_P2VAR_PARA(actAEAD7539Context) ctx;

  /* validate parameters */
  if ((NULL == workSpace)
    || (NULL == tag))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* check workSpace state */
  if ((ESL_WST_ALGO_AEAD_7539 != (workSpace->header.status & ESL_WST_M_ALGO))
    || (ESL_WST_M_RUNNING != (workSpace->header.status & ESL_WST_M_RUNNING)))
  {
    return ESL_ERC_WS_STATE_INVALID;
  }

  /* finalize actCLib AEAD7539 */
  ctx = (VSECPRIM_P2VAR_PARA(actAEAD7539Context)) workSpace->wsAEADChaChaPoly;
  actAEAD7539Finalize(ctx, tag, workSpace->header.watchdog);

  /* Reset and clear workspace */
  esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsAEADChaChaPoly); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 * esl_verifyChaChaPoly
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC( eslt_ErrorCode ) esl_verifyChaChaPoly (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceChaChaPoly ) workSpace,
                      VSECPRIM_P2CONST_PARA( eslt_Byte ) tag)
{

  actU8 tmpTag [ESL_SIZEOF_Poly1305_TAG];
  eslt_ErrorCode erc;

  erc = esl_finalizeChaChaPoly (workSpace, tmpTag);
  if (ESL_ERC_NO_ERROR != erc)
  {
    return erc;
  }

  if (TRUE != actMemcmp(tmpTag, tag, ESL_SIZEOF_Poly1305_TAG))  /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
  {
    return ESL_ERC_INCORRECT_TAG;
  }

  /* Reset and clear workspace */
  esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsAEADChaChaPoly); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 ** Local Functions
 ***************************************************************************/
VSECPRIM_LOCAL VSECPRIM_FUNC( eslt_ErrorCode ) initChaChaPoly (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceChaChaPoly ) workSpace, VSECPRIM_P2CONST_PARA( eslt_Byte ) key, VSECPRIM_P2CONST_PARA( eslt_Byte ) nonce, const eslt_Byte mode)
{
  VSECPRIM_P2VAR_PARA( actAEAD7539Context ) ctx;

  /* validate parameters */
  if ((NULL == workSpace)
    || (NULL == key)
    || (NULL == nonce))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* check workSpace */
  if (ESL_SIZEOF_WS_AEAD_ChaCha_Poly > workSpace->header.size)
  {
    return ESL_ERC_WS_TOO_SMALL;
  }

  /* initialize actCLib AEAD7539 */
  ctx = (VSECPRIM_P2VAR_PARA( actAEAD7539Context )) workSpace->wsAEADChaChaPoly;
  actAEAD7539Init (ctx, key, nonce, mode, workSpace->header.watchdog);

  /* set workSpace state */
  workSpace->header.status = (ESL_WST_ALGO_AEAD_7539 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

  return ESL_ERC_NO_ERROR;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_AEAD_CHA_CHA20_POLY1305_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_AEAD_ChaCha20_Poly1305.c
 *********************************************************************************************************************/

