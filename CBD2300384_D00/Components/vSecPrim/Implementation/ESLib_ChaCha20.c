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
/*!        \file  ESLib_ChaCha20.c
 *        \brief  ChaCha20 implementation.
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
 ** ChaCha20 interface implementation.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   esl_initEncryptChaCha20
 **   esl_initDecryptChaCha20
 **   esl_encryptChaCha20
 **   esl_decryptChaCha20
 **
 ***************************************************************************/
#define ESLIB_CHACHA20_SOURCE
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIChaCha20.h"

#if (VSECPRIM_CHA_CHA20_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/* PRQA S 2889 EOF */ /* MD_VSECPRIM_QM_ONLY_RETURN_PATHS */
/* PRQA S 0310, 3305 EOF */ /* MD_VSECPRIM_QM_ONLY_CASTING_POINTER_TYPE */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** Local Functions
 ***************************************************************************/
VSECPRIM_LOCAL VSECPRIM_FUNC( eslt_ErrorCode ) initChaCha20   (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceChaCha20 ) workSpace, VSECPRIM_P2CONST_PARA( eslt_Byte ) key, VSECPRIM_P2CONST_PARA( eslt_Byte ) nonce, const eslt_Size32 blockCount);
VSECPRIM_LOCAL VSECPRIM_FUNC( eslt_ErrorCode ) cipherChaCha20 (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceChaCha20 ) workSpace, VSECPRIM_P2CONST_PARA( eslt_Byte ) in, const eslt_Length inSize, VSECPRIM_P2VAR_PARA( eslt_Byte ) out, VSECPRIM_P2VAR_PARA( eslt_Length ) outSize);

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/****************************************************************************
 * esl_initEncryptChaCha20
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC( eslt_ErrorCode ) esl_initEncryptChaCha20 (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceChaCha20 ) workSpace,
                                                         VSECPRIM_P2CONST_PARA( eslt_Byte ) key,
                                                         VSECPRIM_P2CONST_PARA( eslt_Byte ) nonce,
                                                         const eslt_Size32 blockCount)
{
  return initChaCha20 (workSpace, key, nonce, blockCount);
}

/****************************************************************************
 * esl_initDecryptChaCha20
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC( eslt_ErrorCode ) esl_initDecryptChaCha20 (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceChaCha20 ) workSpace,
                                                         VSECPRIM_P2CONST_PARA( eslt_Byte ) key,
                                                         VSECPRIM_P2CONST_PARA( eslt_Byte ) nonce,
                                                         const eslt_Size32 blockCount)
{
  return initChaCha20 (workSpace, key, nonce, blockCount);
}

/****************************************************************************
 * esl_encryptChaCha20
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC( eslt_ErrorCode ) esl_encryptChaCha20 (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceChaCha20 ) workSpace,
                                                     VSECPRIM_P2CONST_PARA( eslt_Byte )       in,
                                                     const eslt_Length      inSize,
                                                     VSECPRIM_P2VAR_PARA( eslt_Byte )       out,
                                                     VSECPRIM_P2VAR_PARA( eslt_Length )     outSize)
{
  return cipherChaCha20 (workSpace, in, inSize, out, outSize);
}

/****************************************************************************
 * esl_finalizeEncryptChaCha20
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeEncryptChaCha20(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaCha20) workSpace,
                                                          VSECPRIM_P2CONST_PARA(eslt_Byte)       in,
                                                          const eslt_Length      inSize,
                                                          VSECPRIM_P2VAR_PARA(eslt_Byte)       out,
                                                          VSECPRIM_P2VAR_PARA(eslt_Length)     outSize)
{
  eslt_ErrorCode  result;

  result = esl_encryptChaCha20(workSpace, in, inSize, out, outSize);

  /* Reset and clear workspace */
  esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsChaCha20); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

  return result;
}

/****************************************************************************
 * esl_decryptChaCha20
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC( eslt_ErrorCode ) esl_decryptChaCha20 (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceChaCha20 ) workSpace,
                                                     VSECPRIM_P2CONST_PARA( eslt_Byte )       in,
                                                     const eslt_Length      inSize,
                                                     VSECPRIM_P2VAR_PARA( eslt_Byte )       out,
                                                     VSECPRIM_P2VAR_PARA( eslt_Length )     outSize)
{
  return cipherChaCha20 (workSpace, in, inSize, out, outSize);
}

/****************************************************************************
 * esl_finalizeDecryptChaCha20
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeDecryptChaCha20(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaCha20) workSpace,
                                                          VSECPRIM_P2CONST_PARA(eslt_Byte)       in,
                                                          const eslt_Length      inSize,
                                                          VSECPRIM_P2VAR_PARA(eslt_Byte)       out,
                                                          VSECPRIM_P2VAR_PARA(eslt_Length)     outSize)
{
  eslt_ErrorCode  result;

  result = esl_decryptChaCha20(workSpace, in, inSize, out, outSize);

  /* Reset and clear workspace */
  esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsChaCha20); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

  return result;
}

/****************************************************************************
 ** Local Functions
 ***************************************************************************/
VSECPRIM_LOCAL VSECPRIM_FUNC( eslt_ErrorCode ) initChaCha20 (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceChaCha20 ) workSpace,
                                                     VSECPRIM_P2CONST_PARA( eslt_Byte ) key,
                                                     VSECPRIM_P2CONST_PARA( eslt_Byte ) nonce,
                                                     const eslt_Size32 blockCount)
{
  /* validate parameters */
  if ((NULL == workSpace)
    || (NULL == key)
    || (NULL == nonce))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* check workSpace */
  if (ESL_SIZEOF_WS_ChaCha20 > workSpace->header.size)
  {
    return ESL_ERC_WS_TOO_SMALL;
  }

  /* initialize actCLib ChaCha20 */
  actChaCha20Init ((VSECPRIM_P2VAR_PARA( actChaCha20Context )) workSpace->wsChaCha20, key, nonce, blockCount, workSpace->header.watchdog);

  /* set workSpace state */
  workSpace->header.status = (ESL_WST_ALGO_ChaCha20 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

  return ESL_ERC_NO_ERROR;
}

VSECPRIM_LOCAL VSECPRIM_FUNC(eslt_ErrorCode) cipherChaCha20(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceChaCha20) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte)       in,
  const eslt_Length      inSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte)       out,
  VSECPRIM_P2VAR_PARA(eslt_Length)     outSize)
{
  /* validate parameters */
  if ((NULL == workSpace)
    || (NULL == in)
    || (NULL == out) || (NULL == outSize))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  if (*outSize < inSize)
  {
    return ESL_ERC_BUFFER_TOO_SMALL;
  }

  /* check workSpace state */
  if ((ESL_WST_ALGO_ChaCha20 != (workSpace->header.status & ESL_WST_M_ALGO))
    || (ESL_WST_M_RUNNING != (workSpace->header.status & ESL_WST_M_RUNNING)))
  {
    return ESL_ERC_WS_STATE_INVALID;
  }

  actChaCha20Cipher ((VSECPRIM_P2VAR_PARA( actChaCha20Context )) workSpace->wsChaCha20, in, inSize, out, workSpace->header.watchdog);

  *outSize = inSize;

  return ESL_ERC_NO_ERROR;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_CHA_CHA20_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_ChaCha20.c
 *********************************************************************************************************************/

