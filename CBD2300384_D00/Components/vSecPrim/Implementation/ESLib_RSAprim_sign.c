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
/*!        \file  ESLib_RSAprim_sign.c
 *        \brief  RSA sign primitive
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

#define ESLIB_RSA_PRIM_SIGN_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ESLib.h"
#include "ESLib_Helper.h"
#include "actIRSA.h"

#if (VSECPRIM_RSA_SIGNATURE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  esl_initSignRSA_prim()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSA_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig_prim) workSpace,
  eslt_Length keyPairModuleSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
  eslt_Length privateKeyExponentSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent)
{
  eslt_ErrorCode returnValue = ESL_ERC_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if ((workSpace == NULL_PTR) || (keyPairModule == NULL_PTR) || (privateKeyExponent == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((returnValue = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_RSA_PRIM)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* returnValue already set */
  }
  else
  {
    /* Init actCLib RSA private key initialization */
    actRETURNCODE actReturnValue = actRSAInitPrivateKeyOperation(keyPairModule, keyPairModuleSize,
      privateKeyExponent, privateKeyExponentSize,
      &(workSpace->wsRSAPrimBuf), /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      workSpace->header.watchdog);

    if (actReturnValue != actOK)
    {
      if (actReturnValue == actEXCEPTION_MEMORY)
      {
        returnValue = ESL_ERC_WS_TOO_SMALL;
      }
      else if (actReturnValue == actRSA_PARAM_OUT_OF_RANGE)
      {
        returnValue = ESL_ERC_PARAMETER_INVALID;
      }
      else
      {
        /* actReturnValue == actEXCEPTION_PRIVKEY
           actRSAInitPrivateKeyOperation does not return actEXCEPTION_PUBKEY in a private key operation */
        returnValue = ESL_ERC_RSA_PRIVKEY_INVALID;
      }
    }
    else
    {
      /* returnValue already set */
      /* Set workSpace state */
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_CALL_FUNCTION */
    }
  }

  return returnValue;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  esl_signRSA_prim()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_signRSA_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig_prim) workSpace,
  eslt_Length messageSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) message,
  VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if ((workSpace == NULL_PTR) || (signatureSize == NULL_PTR) || (signature == NULL_PTR) || (message == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PRIM, ESL_WST_ALGO_RSA)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  {
    /* returnValue already set */
  }
  else
  {
    /* Call actCLib RSA private key operation */
      actLengthType signatureLen = *signatureSize;
      actRETURNCODE actReturnValue = actRSAPrivateKeyOperation(message, messageSize, signature, &signatureLen, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        &(workSpace->wsRSAPrimBuf), workSpace->header.watchdog);

    if (actReturnValue != actOK)
    {
      if (actReturnValue == actEXCEPTION_MEMORY)
      {
        returnValue = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
      }
      else /* actReturnValue == actRSA_PARAM_OUT_OF_RANGE */
      {
        returnValue = ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE;
      }
    }
    else
    {
      /* returnValue was already set */
      *signatureSize = (eslt_Length)(signatureLen & 0xFFFFu); /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  return returnValue;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_RSA_SIGNATURE_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSAprim_sign.c
 *********************************************************************************************************************/
