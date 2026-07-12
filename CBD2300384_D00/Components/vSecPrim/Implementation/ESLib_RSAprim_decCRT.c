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
/*!        \file  ESLib_RSAprim_decCRT.c
 *        \brief  RSA sign decrypt primitive (CRT version)
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

#define ESLIB_RSA_PRIM_DEC_CRT_SOURCE

#include "ESLib.h"
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIRSA.h"

#if (VSECPRIM_RSA_CIPHER_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/****************************************************************************
 ** Global Functions
 ***************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

 /**********************************************************************************************************************
  *  esl_initDecryptRSACRT_prim()
  *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptRSACRT_prim(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTdec_prim) workSpace,
                                                         eslt_Length keyPairPrimePSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeP,
                                                         eslt_Length keyPairPrimeQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeQ,
                                                         eslt_Length privateKeyExponentDPSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDP,
                                                         eslt_Length privateKeyExponentDQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDQ,
                                                         eslt_Length privateKeyInverseQISize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyInverseQI)
{
  eslt_ErrorCode retVal;
  actRETURNCODE result;

  /* any null pointers ? */
  if ((workSpace == NULL_PTR) || (keyPairPrimeP == NULL_PTR) || (keyPairPrimeQ == NULL_PTR) ||
      (privateKeyExponentDP == NULL_PTR) || (privateKeyExponentDQ == NULL_PTR) || (privateKeyInverseQI == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    /* Init actCLib RSA private key initialization */
    result = actRSAInitPrivateKeyOperationCRT(keyPairPrimeP, keyPairPrimePSize, keyPairPrimeQ,
                                              keyPairPrimeQSize, privateKeyExponentDP,
                                              privateKeyExponentDPSize, privateKeyExponentDQ,
                                              privateKeyExponentDQSize, privateKeyInverseQI,
                                              privateKeyInverseQISize, &(workSpace->CRTStruct),
                                              workSpace->header.watchdog);

    if (result != actOK)
    {
      if (result == actEXCEPTION_MEMORY)
      {
        retVal = ESL_ERC_WS_TOO_SMALL;
      }
      else /* result == actEXCEPTION_PRIVKEY */
      {
        retVal = ESL_ERC_RSA_PRIVKEY_INVALID;
      }
    }
    else
    {
      /* Set workSpace state */
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_CALL_FUNCTION */
      retVal = ESL_ERC_NO_ERROR;
    }
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

/**********************************************************************************************************************
 *  esl_decryptRSACRT_prim()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSACRT_prim(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTdec_prim) workSpace,
                                                     eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
                                                     VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message)
{
  eslt_ErrorCode returnValue;
  actRETURNCODE result;
  actLengthType messageLen;

  /* any null pointers ? */
  if ((workSpace == NULL_PTR) || (messageSize == NULL_PTR) || (message == NULL_PTR) || (cipher == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  /* Check workSpace */
  else if ((returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_CRT_DEC_PRIM, ESL_WST_ALGO_RSA)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* returnValue already set */
  }
  else
  {
    /* Call actCLib RSA private key operation (CRT) */
    messageLen = *messageSize;
    result = actRSAPrivateKeyOperationCRT(cipher, cipherSize, message, &messageLen, &(workSpace->CRTStruct), workSpace->header.watchdog);

    if (result != actOK)
    {
      if (result == actEXCEPTION_MEMORY)
      {
        returnValue = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
      }
      else /* result == actRSA_PARAM_OUT_OF_RANGE */
      {
        returnValue = ESL_ERC_RSA_CODE_OUT_OF_RANGE;
      }
    }
    else
    {
      *messageSize = (eslt_Length)(messageLen & 0xFFFFu);
      /* retVal = ESL_ERC_NO_ERROR already set in this case */
    }
  }

  return returnValue;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_RSA_CIPHER_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSAprim_decCRT.c
 *********************************************************************************************************************/
