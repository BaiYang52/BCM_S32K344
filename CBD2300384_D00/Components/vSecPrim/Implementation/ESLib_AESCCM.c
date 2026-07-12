/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is protected under intellectual property laws and proprietary to cv cryptovision GmbH
 *                and/or Vector Informatik GmbH.
 *                No right or license is granted save as expressly set out in the applicable license conditions.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  ESLib_AESCCM.c
 *        \brief  AES 128 CCM implementation.
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

#define ESLIB_AESCCM_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "ESLib.h"
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIAESCCM.h"

#if (VSECPRIM_AES_CCM_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 esl_initAESCCMLengthCheck()
 *********************************************************************************************************************/
/*! \brief         Length checks for AES CCM Init function.
 *  \details       -
 *  \param[in]     keyLength                AES key length
 *  \param[in]     nonceLength              length of the nonce
 *  \param[in]     msgLength                length of the message
 *  \param[in]     authenticationFieldSize  desired length of the authentication tag
 *  \param[in]     lengthFieldSize          value L fulfilling msgLength < 2^(8*L) bytes
 *  \return        ESL_ERC_NO_ERROR         length check successful
 *                 ESL_ERC_INVALID_LENGTH   length check failed
 *  \pre           -
 * \context        TASK
 * \reentrant      TRUE
 * \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initAESCCMLengthCheck(
  const eslt_Length keyLength,
  const eslt_Length nonceLength,
  const eslt_Length msgLength,
  const eslt_Byte authenticationFieldSize,
  const eslt_Byte lengthFieldSize);

/**********************************************************************************************************************
 esl_initAESCCMLengthFieldCheck()
 *********************************************************************************************************************/
/*! \brief         Check value of lengthFieldSize for AES CCM Init function.
 *  \details       -
 *  \param[in]     msgLength                length of the message
 *  \param[in]     lengthFieldSize          value L fulfilling msgLength < 2^(8*L) bytes
 *  \return        ESL_ERC_NO_ERROR         length check successful
 *                 ESL_ERC_INVALID_LENGTH   length check failed
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initAESCCMLengthFieldCheck(
  const eslt_Length msgLength,
  const eslt_Byte lengthFieldSize);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  esl_initAESCCMLengthCheck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initAESCCMLengthCheck(
  const eslt_Length keyLength,
  const eslt_Length nonceLength,
  const eslt_Length msgLength,
  const eslt_Byte authenticationFieldSize,
  const eslt_Byte lengthFieldSize)
{
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;
  eslt_Byte expectedNonceLength = (eslt_Byte)15u - lengthFieldSize;

  if (keyLength != ESL_SIZEOF_AES128_KEY)
  {
    retVal = ESL_ERC_INVALID_LENGTH;
  }

  if (retVal == ESL_ERC_NO_ERROR)
  {
    if (authenticationFieldSize < 4u)
    {
      retVal = ESL_ERC_INVALID_LENGTH;
    }
    else
    {
      if ((authenticationFieldSize > 16u) || ((authenticationFieldSize % 2u) != 0u))
      {
        retVal = ESL_ERC_INVALID_LENGTH;
      }
    }
  }

  if (retVal == ESL_ERC_NO_ERROR)
  {
    retVal = esl_initAESCCMLengthFieldCheck(msgLength, lengthFieldSize);
  }

  if (retVal == ESL_ERC_NO_ERROR)
  {
    if (nonceLength != (eslt_Length)(expectedNonceLength))
    {
      retVal = ESL_ERC_INVALID_LENGTH;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_initAESCCMLengthFieldCheck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initAESCCMLengthFieldCheck(
  const eslt_Length msgLength,
  const eslt_Byte lengthFieldSize)
{
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;

  if ((lengthFieldSize < 2u) || (lengthFieldSize > 8u))
  {
    retVal = ESL_ERC_INVALID_LENGTH;
  }

# if (vSecPrim_GetSizeOfEsltLengthOfGeneral() == VSECPRIM_SIZE_UINT32) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
  if (retVal == ESL_ERC_NO_ERROR)
  {
    if ((lengthFieldSize == 2u) && (msgLength > 0xFFFFu))
    {
      retVal = ESL_ERC_INVALID_LENGTH;
    }
    else
    {
      if ((lengthFieldSize == 3u) && (msgLength > 0x00FFFFFFu))
      {
        retVal = ESL_ERC_INVALID_LENGTH;
      }
    }
  }
# else
  VSECPRIM_DUMMY_STATEMENT_CONST(msgLength); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
# endif

  return retVal;
}

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * esl_initAESCCM
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initAESCCM (
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAESCCM) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) key,
  const eslt_Length keyLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte)nonce,
  const eslt_Length nonceLength,
  const eslt_Length aadLength,
  const eslt_Length msgLength,
  const eslt_Byte authenticationFieldSize,
  const eslt_Byte lengthFieldSize)
{
  eslt_ErrorCode retVal;

  /* validate parameters */
  if ((workSpace == NULL_PTR) || (key == NULL_PTR) || (nonce == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_initAESCCMLengthCheck(keyLength, nonceLength, msgLength, authenticationFieldSize, lengthFieldSize)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_SIZEOF_WS_AESCCM)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal already set */
  }
  else
  {
    /* call act init function and set workSpace state */
    actAESCCMInit(&workSpace->wsAESCCM, key, (actLengthType)keyLength, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                  nonce, (actLengthType)nonceLength, (actLengthType)aadLength, (actLengthType)msgLength,
                  authenticationFieldSize, lengthFieldSize, workSpace->header.watchdog);

    workSpace->header.status = (ESL_WST_ALGO_AES128CCM | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL); /* SBSW_VSECPRIM_WRITE_POINTER */
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/***********************************************************************************************************************
 * esl_updateAESCCMEncrypt
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateAESCCMEncrypt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAESCCM) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) aad,
  const eslt_Length aadSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) msg,
  const eslt_Length msgSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte)out,
  VSECPRIM_P2VAR_PARA(eslt_Length) outSize)
{
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;

  /* validate parameters */
  if ((workSpace == NULL_PTR) ||
      ((aad == NULL_PTR) && (aadSize > 0u)) ||
      ((msg == NULL_PTR) && (msgSize > 0u)) ||
      (out == NULL_PTR) || (outSize == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_SIZEOF_WS_AESCCM, ESL_WST_ALGO_AES128CCM)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal already set */
  }
  else if (*outSize < (msgSize - (msgSize % ESL_SIZEOF_AES_BLOCK)))
  {
    retVal = ESL_ERC_BUFFER_TOO_SMALL;
  }
  else
  {
    /* update actCLib */
    actRetVal = actAESCCMUpdateEncrypt(&workSpace->wsAESCCM, aad, (actLengthType)aadSize, msg, (actLengthType)msgSize, out, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actRetVal == actEXCEPTION_LENGTH)
    {
      retVal = ESL_ERC_CCM_TOTAL_LENGTH_OVERFLOW;
      *outSize = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    else if (actRetVal == actEXCEPTION_INPUT_LENGTH)
    {
      retVal = ESL_ERC_PARAMETER_INVALID;
      *outSize = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    else if (actRetVal == actEXCEPTION_MODE)
    {
      retVal = ESL_ERC_STATE_INVALID;
      *outSize = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    else
    {
      *outSize = (msgSize - (msgSize % ESL_SIZEOF_AES_BLOCK)); /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  return retVal;
} /* PRQA S 6060, 6080 */ /* MD_VSECPRIM_STPAR, MD_MSR_STMIF */

/***********************************************************************************************************************
 * esl_updateAESCCMDecrypt
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateAESCCMDecrypt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAESCCM) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) aad,
  const eslt_Length aadSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) encMsg,
  const eslt_Length encMsgSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte)out,
  VSECPRIM_P2VAR_PARA(eslt_Length) outSize)
{
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;

  /* validate parameters */
  if ((workSpace == NULL_PTR) ||
    ((encMsg == NULL_PTR) && (encMsgSize > 0u)) ||
    ((aad == NULL_PTR) && (aadSize > 0u)) ||
    (out == NULL_PTR) ||  (outSize == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_SIZEOF_WS_AESCCM, ESL_WST_ALGO_AES128CCM)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal already set */
  }
  else if (*outSize < (encMsgSize - (encMsgSize % ESL_SIZEOF_AES_BLOCK)))
  {
    retVal = ESL_ERC_BUFFER_TOO_SMALL;
  }
  else
  {
    /* update actCLib */
    actRetVal = actAESCCMUpdateDecrypt(&workSpace->wsAESCCM, aad, (actLengthType)aadSize, encMsg, (actLengthType)encMsgSize, out, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actRetVal == actEXCEPTION_LENGTH)
    {
      retVal = ESL_ERC_CCM_TOTAL_LENGTH_OVERFLOW;
      *outSize = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    else if (actRetVal == actEXCEPTION_INPUT_LENGTH)
    {
      retVal = ESL_ERC_PARAMETER_INVALID;
      *outSize = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    else if (actRetVal == actEXCEPTION_MODE)
    {
      retVal = ESL_ERC_STATE_INVALID;
      *outSize = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    else
    {
      *outSize = (encMsgSize - (encMsgSize % ESL_SIZEOF_AES_BLOCK)); /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  return retVal;
} /* PRQA S 6060, 6080 */ /* MD_VSECPRIM_STPAR, MD_MSR_STMIF */

/***********************************************************************************************************************
 * esl_finalizeAESCCMEncrypt
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeAESCCMEncrypt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAESCCM) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) out,
  VSECPRIM_P2VAR_PARA(eslt_Length) outLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) tag,
  VSECPRIM_P2VAR_PARA(eslt_Length) tagLength)
{
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;

  /* validate parameters */
  if ((workSpace == NULL_PTR) || (outLength == NULL_PTR) || (tag == NULL_PTR) || (tagLength == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((out == NULL_PTR) && (*outLength > 0u))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if (*outLength < workSpace->wsAESCCM.remainingLen)
  {
    retVal = ESL_ERC_BUFFER_TOO_SMALL;
  }
  else if (*tagLength < workSpace->wsAESCCM.authFieldSize)
  {
    retVal = ESL_ERC_BUFFER_TOO_SMALL;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_SIZEOF_WS_AESCCM, ESL_WST_ALGO_AES128CCM)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal already set */
  }
  else
  {
    actRetVal = actAESCCMFinalizeEncrypt(&workSpace->wsAESCCM, out, tag, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actRetVal == actOK)
    {
      *outLength = (eslt_Length)workSpace->wsAESCCM.remainingLen; /* SBSW_VSECPRIM_WRITE_POINTER */
      *tagLength = (eslt_Length)workSpace->wsAESCCM.authFieldSize; /* SBSW_VSECPRIM_WRITE_POINTER */
      /* retVal already set */
    }
    else /* actRetVal = actEXCEPTION_LENGTH */
    {
      *outLength = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
      *tagLength = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
      retVal = ESL_ERC_INVALID_LENGTH;
    }

    /* Reset and clear workspace */
    esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsAESCCM); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 * esl_finalizeAESCCMDecrypt
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeAESCCMDecrypt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceAESCCM) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) out,
  VSECPRIM_P2VAR_PARA(eslt_Length) outLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) tag)
{
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;

  actU8 tmpTag[ESL_SIZEOF_CCM_TAG];

  /* validate parameters */
  if ((workSpace == NULL_PTR) || (outLength == NULL_PTR) || (tag == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((out == NULL_PTR) && (*outLength > 0u))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if (*outLength < workSpace->wsAESCCM.remainingLen)
  {
    retVal = ESL_ERC_BUFFER_TOO_SMALL;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_SIZEOF_WS_AESCCM, ESL_WST_ALGO_AES128CCM)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal already set */
  }
  else
  {
    actRetVal = actAESCCMFinalizeDecrypt(&workSpace->wsAESCCM, out, tmpTag, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actRetVal == actOK)
    {
      if (actMemcmp(tmpTag, tag, workSpace->wsAESCCM.authFieldSize) != TRUE) /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
      {
        retVal = ESL_ERC_INCORRECT_TAG;
      }

      *outLength = (eslt_Length)workSpace->wsAESCCM.remainingLen; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    else
    {
      *outLength = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
      retVal = ESL_ERC_INVALID_LENGTH;
    }

    /* Reset and clear workspace */
    esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsAESCCM); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_AES_CCM_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_AES128CCM.c
 *********************************************************************************************************************/

