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
/*!        \file  ESLib_RSA_V15_Enc.c
 *        \brief  RSA V1.5 (Encryption) implementation.
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
#define ESLIB_RSA_V15_ENC_SOURCE

#include "ESLib.h"
#include "ESLib_Helper.h"
#include "ESLib_ASN_1.h"

#include "actIRSA.h"

#if (VSECPRIM_RSA_V15_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/****************************************************************************
 ** Global Functions
 ***************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  esl_initEncryptRSA_V15()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptRSA_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAenc) workSpace,
                                                     eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule, eslt_Length publicKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent)
{
  eslt_ErrorCode returnValue;

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    /* get underlying workspace pointer */
    if (workSpace->header.size < ESL_MAXSIZEOF_WS_RSA_ENC)
    {
      returnValue = ESL_ERC_WS_TOO_SMALL;
    }
    else
    {
      workSpace->wsRSAEncPrim.header.size = ESL_MAXSIZEOF_WS_RSA_ENC_PRIM;
      workSpace->wsRSAEncPrim.header.watchdog = workSpace->header.watchdog;
      returnValue = esl_initEncryptRSA_prim(&(workSpace->wsRSAEncPrim), keyPairModuleSize, keyPairModule, publicKeyExponentSize, publicKeyExponent);
    }
    if (ESL_ERC_NO_ERROR == returnValue)
    {
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_CALL_FUNCTION */
      /* store message buffer length in workspace */
      workSpace->modulusSizeInformation = keyPairModuleSize;
    }
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_encryptRSA_V15()
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
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRSA_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAenc) workSpace,
                                                 eslt_Length messageSize, VSECPRIM_P2CONST_PARA(eslt_Byte) message, VSECPRIM_P2VAR_PARA(eslt_Length) cipherSize, VSECPRIM_P2VAR_PARA(eslt_Byte) cipher)
{
  eslt_ErrorCode returnValue;
  eslt_Length keyPairModuleSize;
  eslt_Length rnd_BytesNeeded;
  eslt_Length i;
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageBuf;

  rnd_BytesNeeded = 0u; /* PRQA S 2982 */ /* MD_VSECPRIM_2.2 */

  /* any null pointers ? */
  if ((workSpace == NULL_PTR) || (message == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    /* get underlying workspace pointer */
    keyPairModuleSize = workSpace->modulusSizeInformation;
    messageBuf = workSpace->messageBuffer;

    if (ESL_WST_ALGO_RSA != (workSpace->header.status & ESL_WST_M_ALGO))
    {
      returnValue = ESL_ERC_WS_STATE_INVALID;
    }
    else if (0u == (workSpace->header.status & ESL_WST_M_RUNNING))
    {
      returnValue = ESL_ERC_WS_STATE_INVALID;
    }
    else if (keyPairModuleSize < ASN1_SIZEOF_MINIMAL_PADDING)
    {
      returnValue = ESL_ERC_RSA_MODULE_OUT_OF_RANGE;
    }
    else if (messageSize > (keyPairModuleSize - ASN1_SIZEOF_MINIMAL_PADDING))
    {
      returnValue = ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE;
    }
    else
    {
      /* The previous check ensures that messageSize <= keyPairModuleSize - ASN1_SIZEOF_MINIMAL_PADDING. */
      /* Thus, rnd_BytesNeeded = keyPairModuleSize - messageSize - 3 >= 0 because
       * keyPairModuleSize - messageSize - 3 >= keyPairModuleSize - keyPairModuleSize + ASN1_SIZEOF_MINIMAL_PADDING - 3
       *                                      = 11 - 3 = 8 >= 0,
       * i.e., no overflow occurrs in this subtraction. */
      rnd_BytesNeeded = keyPairModuleSize - messageSize - 3u;

      messageBuf[0] = 0x00;
      messageBuf[1] = 0x02;

      returnValue = esl_getBytesRNG(rnd_BytesNeeded, &messageBuf[2]);
      if (returnValue == ESL_ERC_NO_ERROR)
      {
        for (i = 0u; i < rnd_BytesNeeded; i++)
        {
          while ((returnValue == ESL_ERC_NO_ERROR) && (messageBuf[i+2u] == 0x00u))
          {
            returnValue = esl_getBytesRNG(1u, &messageBuf[i+2u]);
          }
        }
      }

    }

    if (ESL_ERC_NO_ERROR == returnValue)
    {
      messageBuf[rnd_BytesNeeded + 2u] = 0x00u;
      actMemCpyByteArray(&messageBuf[rnd_BytesNeeded + 3u], message, messageSize); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

      returnValue = esl_encryptRSA_prim(&(workSpace->wsRSAEncPrim), keyPairModuleSize, messageBuf, cipherSize, cipher);
    }
  }
  return returnValue;
} /* PRQA S 6080 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_RSA_V15_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSA_V15_Enc.c
 *********************************************************************************************************************/
