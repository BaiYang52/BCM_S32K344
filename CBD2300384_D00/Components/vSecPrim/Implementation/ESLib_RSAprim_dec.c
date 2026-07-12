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
/*!        \file  ESLib_RSAprim_dec.c
 *        \brief  RSA sign decrypt primitive
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

#define ESLIB_RSA_PRIM_DEC_SOURCE

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
  *  esl_initDecryptRSA_prim()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptRSA_prim(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAdec_prim) workSpace,
                                                      eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule, eslt_Length privateKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent)
{
  eslt_ErrorCode retVal;
  actRETURNCODE result;

  if ((workSpace == NULL_PTR) || (keyPairModule == NULL_PTR) || (privateKeyExponent == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    /* Init actCLib RSA private key initialization */
    result = actRSAInitPrivateKeyOperation(keyPairModule, keyPairModuleSize, privateKeyExponent,
                                           privateKeyExponentSize, &(workSpace->wsRSAPrimBuf),
                                           workSpace->header.watchdog);
    if (result != actOK)
    {
      if (result == actEXCEPTION_MEMORY)
      {
        retVal = ESL_ERC_WS_TOO_SMALL;
      }
      else if (result == actRSA_PARAM_OUT_OF_RANGE)
      {
        retVal = ESL_ERC_PARAMETER_INVALID;
      }
      else /* (result == actEXCEPTION_PRIVKEY) */
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
}

/**********************************************************************************************************************
 *  esl_decryptRSA_prim()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSA_prim(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAdec_prim) workSpace,
                                                  eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
                                                  VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message)
{
  eslt_ErrorCode retVal;
  actRETURNCODE result;
  actLengthType messageLen;

  if ((workSpace == NULL_PTR) || (cipher == NULL_PTR) || (messageSize == NULL_PTR) || (message == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_RSA_DEC_PRIM, ESL_WST_ALGO_RSA)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  else
  {
  /* Call actCLib RSA private key operation */

    messageLen = *messageSize;
    result = actRSAPrivateKeyOperation(cipher, cipherSize, message, &messageLen,
                                       &(workSpace->wsRSAPrimBuf), workSpace->header.watchdog);
    if (result != actOK)
    {
      if (result == actEXCEPTION_MEMORY)
      {
        retVal = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
      }
      else /* result == actRSA_PARAM_OUT_OF_RANGE */
      {
        retVal = ESL_ERC_RSA_CODE_OUT_OF_RANGE;
      }
    }
    else
    {
      /* retVal = ESL_ERC_NO_ERROR already set in this case */
      *messageSize = (eslt_Length)(messageLen & 0xFFFFu);
    }
  }

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_RSA_CIPHER_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSAprim_dec.c
 *********************************************************************************************************************/
