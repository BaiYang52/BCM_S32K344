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
/*!         \file  ESLib_CMACAES256.c
 *         \brief  Crypto library -  RFC 4493 AES-CMAC (Generation/Verification)
 *
 *  \description  This file is part of the actCLibrary.
 *                Implements AES-CMAC generation and verification according to RFC 4493
 *
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

#define ESLIB_CMACAES256_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "ESLib.h"
#include "ESLib_Helper.h"

#include "actICMACAES.h"

#if (VSECPRIM_CMAC_ENABLED == STD_ON)

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  esl_initCMACAES256()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initCMACAES256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCMACAES) workSpace,
  const eslt_Length keyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) key)
{
  eslt_ErrorCode retVal;

  if ((workSpace == NULL_PTR) || (key == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_CMACAES)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal already set */
  }
  /* Check keyLength */
  else if (keyLength != ESL_SIZEOF_AES256_KEY)
  {
    retVal = ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE;
  }
  else
  {
    /* Init actCLib CMAC-AES */
    (void)actCMACAESInit(&workSpace->wsCMACAESbuffer, key, ESL_SIZEOF_AES256_KEY, workSpace->header.watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

    /* Provide backward compatible pointer */
    workSpace->wsCMACAES = (VSECPRIM_P2VAR_PARA(eslt_Byte))&(workSpace->wsCMACAESbuffer); /* PRQA S 0310 */ /* MD_VSECPRIM_11.4 */ /* SBSW_VSECPRIM_WRITE_POINTER */

    /* Set workSpace state */
    esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_CMACAES256); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_getExpandedKeyCMACAES256()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_getExpandedKeyCMACAES256(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceCMACAES) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) keyPtr,
  VSECPRIM_P2VAR_PARA(eslt_Length) keyLengthPtr) /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
{
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;

  if ((workSpace == NULL_PTR) || (keyLengthPtr == NULL_PTR) || (keyPtr == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((workSpace->header.status & ESL_WST_M_RUNNING) != ESL_WST_M_RUNNING)
  {
    retVal = ESL_ERC_WS_STATE_INVALID;
  }
  else if (*keyLengthPtr < 256u)
  {
    retVal = ESL_ERC_INPUT_INVALID;
  }
  else
  {
    /* Copy expanded key to provided buffer */
    *keyLengthPtr = 256u; /* SBSW_VSECPRIM_WRITE_POINTER */
    actMemCpyByteArray(keyPtr, workSpace->wsCMACAESbuffer.e_key, *keyLengthPtr); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_initExpandedCMACAES256()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initExpandedCMACAES256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCMACAES) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) expandedKey,
  const eslt_Length expandedKeyLength)
{
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;
  VSECPRIM_P2VAR_PARA(actCMACAESSTRUCT) internalWorkspace;

  if ((workSpace == NULL_PTR) || (expandedKey == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if (workSpace->header.size < ESL_MAXSIZEOF_WS_CMACAES)
  {
    retVal = ESL_ERC_WS_TOO_SMALL;
  }
  /* Check keyLength */
  else if (expandedKeyLength != 256u)
  {
    retVal = ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE;
  }
  else
  {
    /* Init actCLib CMAC-AES */
    internalWorkspace = &workSpace->wsCMACAESbuffer;
    actMemset(internalWorkspace->buffer, 0x00u, actAES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    internalWorkspace->buffer_used = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
    internalWorkspace->key_dword_len = 4u; /* SBSW_VSECPRIM_WRITE_POINTER */
    actMemCpyByteArray(internalWorkspace->e_key, expandedKey, expandedKeyLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

    /* Set workSpace state */
    esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_CMACAES256); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_updateCMACAES256()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateCMACAES256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCMACAES) workSpace,
  const eslt_Length inputLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input)
{
  eslt_ErrorCode retVal;

  if ((workSpace == NULL_PTR) || (input == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_CMACAES, ESL_WST_ALGO_CMACAES256)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    /* Update actCLib CMAC-AES */
    (void)actCMACAESUpdate(&workSpace->wsCMACAESbuffer, input, (actLengthType)inputLength, workSpace->header.watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_finalizeCMACAES256()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeCMACAES256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCMACAES) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageCMAC,
  VSECPRIM_P2VAR_PARA(eslt_Length) messageCMACLength)
{
  eslt_ErrorCode retVal;
  if ((workSpace == NULL_PTR) || (messageCMAC == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_CMACAES, ESL_WST_ALGO_CMACAES256)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else if (*messageCMACLength < ESL_SIZEOF_AES256_BLOCK)
  {
    retVal = ESL_ERC_BUFFER_TOO_SMALL;
  }
  else
  {
    /* Finalize actCLib CMAC-AES */
    (void)actCMACAESFinalize(&workSpace->wsCMACAESbuffer, messageCMAC, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    *messageCMACLength = ESL_SIZEOF_AES256_BLOCK; /* SBSW_VSECPRIM_WRITE_POINTER */

    esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsCMACAESbuffer); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  }
  return retVal;
}

/**********************************************************************************************************************
 *  esl_verifyCMACAES256()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyCMACAES256(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceCMACAES) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) messageCMAC)
{
  eslt_ErrorCode retVal;

  if ((workSpace == NULL_PTR) || (messageCMAC == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_CMACAES, ESL_WST_ALGO_CMACAES256)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    eslt_Byte tmpMAC[ESL_SIZEOF_AES256_BLOCK];
    eslt_Length tmpMACLength = ESL_SIZEOF_AES256_BLOCK;

    actMemClear(tmpMAC, ESL_SIZEOF_AES256_BLOCK); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */

    /* Calculate MAC */
    (void)esl_finalizeCMACAES256(workSpace, tmpMAC, &tmpMACLength); /* SBSW_VSECPRIM_CALL_FUNCTION */

    if (actMemcmp(tmpMAC, messageCMAC, ESL_SIZEOF_AES256_BLOCK) == FALSE) /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    {
      retVal = ESL_ERC_HMAC_INCORRECT_MAC;
    }
  }

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_CMAC_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_CMACAES256.c
 *********************************************************************************************************************/
