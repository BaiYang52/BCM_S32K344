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
/*!        \file  ESLib_RSAprim_signCRT.c
 *        \brief  RSA sign primitives (CRT version)
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

#define ESLIB_RSA_PRIM_SIGN_CRT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ESLib.h"
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIRSA.h"
#if ( (VSECPRIM_RSA_SIGNATURE_ENABLED == STD_ON) && (VSECPRIM_RSAPSSONLY_SIGNATURE_ENABLED == STD_OFF) ) /* COV_VSECPRIM_NO_SAFE_CONFIG XF tf tf */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actRSAInitPrivateKeyOperationCRT()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSACRT_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTsig_prim) workSpace,
  eslt_Length keyPairPrimePSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeP,
  eslt_Length keyPairPrimeQSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeQ,
  eslt_Length privateKeyExponentDPSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDP,
  eslt_Length privateKeyExponentDQSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDQ,
  eslt_Length privateKeyInverseQISize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyInverseQI)
{
  eslt_ErrorCode returnValue;

  /* any null pointers ? */
  if ((workSpace == NULL_PTR) || (keyPairPrimeP == NULL_PTR) || (keyPairPrimeQ == NULL_PTR) ||
      (privateKeyExponentDP == NULL_PTR) || (privateKeyExponentDQ == NULL_PTR) || (privateKeyInverseQI == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    /* Init actCLib RSA private key initialization */
    actRETURNCODE result = actRSAInitPrivateKeyOperationCRT(keyPairPrimeP, keyPairPrimePSize, keyPairPrimeQ,
                                                            keyPairPrimeQSize, privateKeyExponentDP,
                                                            privateKeyExponentDPSize, privateKeyExponentDQ,
                                                            privateKeyExponentDQSize, privateKeyInverseQI,
                                                            privateKeyInverseQISize, &(workSpace->CRTStruct),
                                                            workSpace->header.watchdog);

    if (result != actOK)
    {
      if (result == actEXCEPTION_MEMORY)
      {
        returnValue = ESL_ERC_WS_TOO_SMALL;
      }
      else if (result == actRSA_PARAM_OUT_OF_RANGE)
      {
        returnValue = ESL_ERC_PARAMETER_INVALID;
      }
      else /* result == actEXCEPTION_PRIVKEY */
      {
        returnValue = ESL_ERC_RSA_PRIVKEY_INVALID;
      }
    }
    else
    {
      /* Set workSpace state */
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_CALL_FUNCTION */
      returnValue = ESL_ERC_NO_ERROR;
    }
  }

  return returnValue;
} /* PRQA S 6060 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

/**********************************************************************************************************************
 *  esl_signRSACRT_prim()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_signRSACRT_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTsig_prim) workSpace,
  eslt_Length messageSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) message,
  VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue;
  actRETURNCODE actReturnValue;
  actLengthType signatureLen;

  /* any null pointers ? */
  if ((workSpace == NULL_PTR) || (message == NULL_PTR) || (signature == NULL_PTR) || (signatureSize == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_CRT_SIG_PRIM, ESL_WST_ALGO_RSA)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  else
  {
    /* Call actCLib RSA private key operation (CRT) */
    signatureLen = *signatureSize;
    actReturnValue = actRSAPrivateKeyOperationCRT(message, messageSize, signature, &signatureLen, &(workSpace->CRTStruct),
                                                  workSpace->header.watchdog);

    if (actReturnValue != actOK)
    {
      if (actReturnValue == actEXCEPTION_MEMORY)
      {
        returnValue = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
      }
      else  /* actReturnValue == actRSA_PARAM_OUT_OF_RANGE */
      {
        returnValue = ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE;
      }
    }
    else
    {
      /* returnValue already set in this case */
      *signatureSize = (eslt_Length)(signatureLen & 0xFFFFu);
    }
  }

  return returnValue;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_RSA_SIGNATURE_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSAprim_signCRT.c
 *********************************************************************************************************************/
