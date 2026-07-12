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
/*!        \file  ESLib_GCM.c
 *        \brief  AES-GCM implementation.
 *
 *      \details  Currently the actClib version is used.
 *                This file is part of the embedded systems library cvActLib/ES
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

#define ESLIB_GCM_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "ESLib.h"
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIGCM.h"

#if (VSECPRIM_GCM_ENABLED == STD_ON)

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  esl_initEncryptGCM
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptGCM(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGCM) workSpace,
                                                 VSECPRIM_P2CONST_PARA(eslt_Byte) key, const eslt_Length keyLength, VSECPRIM_P2CONST_PARA(eslt_Byte) iv, const eslt_Length ivLength)
{
  eslt_ErrorCode retVal;

  if ((workSpace == NULL_PTR) || (key == NULL_PTR) || (iv == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_GCM)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal already set */
  }
  /* check keyLength */
  else if ((keyLength != ESL_SIZEOF_AES128_KEY) && (keyLength != ESL_SIZEOF_AES192_KEY) && (keyLength != ESL_SIZEOF_AES256_KEY))
  {
    retVal = ESL_ERC_GCM_INVALID_KEY_LENGTH;
  }
  /* check ivLength */
  else if (ivLength < 1u)
  {
    retVal = ESL_ERC_INVALID_IV_LENGTH;
  }
  else
  {
    /* Return value of actGCMInit can be ignored because it's always OK */
    (void)actGCMInit(&workSpace->wsGCM, key, (actLengthType) keyLength, iv, (actLengthType) ivLength, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* set workSpace state */
    esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_GCM); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  return retVal;
}

/***********************************************************************************************************************
 *  esl_updateAuthDataEncryptGCM
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateAuthDataEncryptGCM(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGCM) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) input, const eslt_Length inputLength)
{
  eslt_ErrorCode retVal;
  VSECPRIM_P2VAR_PARA(actU8) output = NULL_PTR;
  actLengthType bytes_out;

  if ((workSpace == NULL_PTR) || (input == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_GCM, ESL_WST_ALGO_GCM)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  /* check workspace state */
  else if (STATE_AAD != workSpace->wsGCM.state)
  {
    retVal = ESL_ERC_GCM_STATE_INVALID;
  }
  /* update actCLib GCM */
  else if (actEXCEPTION_LENGTH == actGCMEncryptUpdate(&workSpace->wsGCM, input, (actLengthType)inputLength, output, &bytes_out, workSpace->header.watchdog)) /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  {
    retVal = ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW;
  }
  else
  {
    retVal = ESL_ERC_NO_ERROR;
  }

  return retVal;
}

/***********************************************************************************************************************
 *  esl_updatePlaintextEncryptGCM
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updatePlaintextEncryptGCM(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGCM) workSpace,
                                                            VSECPRIM_P2CONST_PARA(eslt_Byte) input,
                                                            const eslt_Length inputLength, VSECPRIM_P2VAR_PARA(eslt_Byte) output, VSECPRIM_P2VAR_PARA(eslt_Size32) bytes_out)
{
  eslt_ErrorCode retVal;
  if ((workSpace == NULL_PTR) || (input == NULL_PTR) || (output == NULL_PTR) || (bytes_out == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_GCM, ESL_WST_ALGO_GCM)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  /* check workspace state */
  else
  {
    if (STATE_CIPHERTEXT != workSpace->wsGCM.state)
    {
      /* switch: AAD -> plaintext message */
      retVal = (actGCMTriggerState(&workSpace->wsGCM, workSpace->header.watchdog) == actOK) ? ESL_ERC_NO_ERROR : ESL_ERC_WS_STATE_INVALID; /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_GCM_ENCRYPT_DECRYPT_UPDATE */
    }

    if (retVal == ESL_ERC_NO_ERROR)
    {
      /* update actCLib GCM */
      if (actEXCEPTION_LENGTH ==
        actGCMEncryptUpdate(&workSpace->wsGCM, input, (actLengthType)inputLength, output, (VSECPRIM_P2VAR_PARA(actLengthType)) bytes_out, workSpace->header.watchdog)) /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      {
        retVal = ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW;
      }
    }
  }

  return retVal;
}

/***********************************************************************************************************************
 *  esl_finalizeEncryptGCM
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeEncryptGCM(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGCM) workSpace,
                                                     VSECPRIM_P2VAR_PARA(eslt_Byte) output, VSECPRIM_P2VAR_PARA(eslt_Size32) bytes_out, VSECPRIM_P2VAR_PARA(eslt_Byte) tag)
{
  eslt_ErrorCode retVal;
  if ((workSpace == NULL_PTR) || (output == NULL_PTR) || (bytes_out == NULL_PTR) || (tag == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_GCM, ESL_WST_ALGO_GCM)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    /* finalize actCLib GCM */
    /* Return value of actGCMEncryptFinalize can be ignored because it's always OK */
    (void)actGCMEncryptFinalize(&workSpace->wsGCM, output, (VSECPRIM_P2VAR_PARA(actLengthType)) bytes_out, tag, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_GCM_ENCRYPT_DECRYPT_FINALIZE */
    esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsGCM); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  return retVal;
}

/***********************************************************************************************************************
 *  esl_initDecryptGCM
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
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptGCM(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGCM) workSpace,
                                                 VSECPRIM_P2CONST_PARA(eslt_Byte) key, const eslt_Length keyLength, VSECPRIM_P2CONST_PARA(eslt_Byte) iv, const eslt_Length ivLength)
{
  eslt_ErrorCode retVal;

  if ((workSpace == NULL_PTR) || (key == NULL_PTR) || (iv == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_GCM)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal already set */
  }
  /* check keyLength */
  else if ((keyLength != ESL_SIZEOF_AES128_KEY) && (keyLength != ESL_SIZEOF_AES192_KEY) && (keyLength != ESL_SIZEOF_AES256_KEY))
  {
    retVal = ESL_ERC_GCM_INVALID_KEY_LENGTH;
  }
  /* check ivLength */
  else if (ivLength < 1u)
  {
    retVal = ESL_ERC_INVALID_IV_LENGTH;
  }
  else
  {
    /* init actCLib GCM */
    /* Return value of actGCMInit can be ignored because it's always OK */
    (void)actGCMInit(&workSpace->wsGCM, key, (actLengthType)keyLength, iv, (actLengthType)ivLength, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* set workSpace status */
    esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_GCM); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  return retVal;
}

/***********************************************************************************************************************
 *  esl_updateAuthDataDecryptGCM
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateAuthDataDecryptGCM(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGCM) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) input, const eslt_Length inputLength)
{
  eslt_ErrorCode retVal;
  actU8 *output = NULL_PTR;
  actLengthType bytes_out;

  if ((workSpace == NULL_PTR) || (input == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_GCM, ESL_WST_ALGO_GCM)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  /* check workspace state */
  else if (STATE_AAD != workSpace->wsGCM.state)
  {
    retVal = ESL_ERC_GCM_STATE_INVALID;
  }
  /* update actCLib GCM */
  else if (actEXCEPTION_LENGTH == actGCMDecryptUpdate(&workSpace->wsGCM, input, (actLengthType)inputLength, output, &bytes_out, workSpace->header.watchdog)) /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  {
    retVal = ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW;
  }
  else
  {
    retVal = ESL_ERC_NO_ERROR;
  }

  return retVal;
}

/***********************************************************************************************************************
 *  esl_updateCiphertextDecryptGCM
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateCiphertextDecryptGCM(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGCM) workSpace,
                                                             VSECPRIM_P2CONST_PARA(eslt_Byte) input,
                                                             const eslt_Length inputLength, VSECPRIM_P2VAR_PARA(eslt_Byte) output, VSECPRIM_P2VAR_PARA(eslt_Size32) bytes_out)
{
  eslt_ErrorCode retVal;
  if ((workSpace == NULL_PTR) || (input == NULL_PTR) || (output == NULL_PTR) || (bytes_out == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_GCM, ESL_WST_ALGO_GCM)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    if (STATE_CIPHERTEXT != workSpace->wsGCM.state)
    {
      /* switch: AAD -> plaintext message */
      retVal = (actGCMTriggerState(&workSpace->wsGCM, workSpace->header.watchdog) == actOK) ? ESL_ERC_NO_ERROR : ESL_ERC_WS_STATE_INVALID; /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }

    if (retVal == ESL_ERC_NO_ERROR)
    {
      /* update actCLib GCM */
      if (actEXCEPTION_LENGTH ==
         actGCMDecryptUpdate(&workSpace->wsGCM, input, (actLengthType)inputLength, output, (VSECPRIM_P2VAR_PARA(actLengthType)) bytes_out, workSpace->header.watchdog)) /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_GCM_ENCRYPT_DECRYPT_UPDATE */
      {
        retVal = ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW;
      }
    }
  }

  return retVal;
}

/***********************************************************************************************************************
 *  esl_finalizeDecryptGCM
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeDecryptGCM(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceGCM) workSpace,
                                                     VSECPRIM_P2VAR_PARA(eslt_Byte) output, VSECPRIM_P2VAR_PARA(eslt_Size32) bytes_out, VSECPRIM_P2CONST_PARA(eslt_Byte) tag)
{
  eslt_ErrorCode retVal;
  eslt_Byte tmpTag[ESL_SIZEOF_GCM_TAG];

  if ((workSpace == NULL_PTR) || (output == NULL_PTR) || (bytes_out == NULL_PTR) || (tag == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_GCM, ESL_WST_ALGO_GCM)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    /* finalize actCLib GCM and compare provided tag with computed one */
    /* Return value of actGCMDecryptFinalize can be ignored because it's always OK */
    (void)actGCMDecryptFinalize(&workSpace->wsGCM, output, (VSECPRIM_P2VAR_PARA(actLengthType)) bytes_out, tmpTag, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_GCM_ENCRYPT_DECRYPT_FINALIZE */

    if (actMemcmp(tmpTag, tag, ESL_SIZEOF_GCM_TAG) != TRUE) /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
    {
      retVal = ESL_ERC_GCM_TAG_VERIFICATION_FAILED;
    }

    esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsGCM); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_GCM_ENABLED) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_GCM.c
 *********************************************************************************************************************/
