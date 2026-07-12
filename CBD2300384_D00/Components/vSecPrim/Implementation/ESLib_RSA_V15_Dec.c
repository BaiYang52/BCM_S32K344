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
/*!        \file  ESLib_RSA_V15_Dec.c
 *        \brief  RSA V1.5 (Decryption) implementation.
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
#define ESLIB_RSA_V15_DEC_SOURCE

#include "ESLib.h"
#include "ESLib_Helper.h"
#include "ESLib_ASN_1.h"

#include "actIRSA.h"

#if (VSECPRIM_RSA_V15_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/****************************************************************************
 ** Local Functions
 ***************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  esl_decryptInternal()
 **********************************************************************************************************************/
/*! \brief   Decrypts cipher according to RSA V15 decryption scheme, stores plaintext in message.
 *  \details -
 *  \param[in,out] workSpace  (in)  RSA decrypting workspace
 *                            (out) RSA decrypting workspace
 *  \param[in]     cipherSize length of the cipher
 *  \param[in]     cipher     cipher text to decode
 *  \param[in,out] messageSize (in)  buffer to store the length of the decrypted message
 *                             (out) buffer storing the length of the decrypted message
 *  \param[in,out] message     (in)  buffer to store the decrypted message
 *                             (out) buffer storing the decrypted message
 *  \param[in]     blockType   identifier for padding block type, used during decoding
 *  \return        ESL_ERC_NO_ERROR
 *                 ESL_ERC_WS_STATE_INVALID
                   ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
                   ESL_ERC_RSA_CODE_OUT_OF_RANGE
                   ESL_ERC_RSA_ENCODING_INVALID
                   ESL_ERC_OUTPUT_SIZE_TOO_SHORT
 *  \pre           cipher, messageSize and message need to be valid pointers.
 *                 keyExponent  < keyPairModule
 *                 Module is a prime
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_decryptInternal(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAdec) workSpace,
  eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
  VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message, eslt_Byte blockType);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
 /**********************************************************************************************************************
  *  esl_decryptInternal()
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
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_decryptInternal(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAdec) workSpace,
                                                         eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
                                                         VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message, eslt_Byte blockType)
{

  eslt_ErrorCode returnValue;
  eslt_Length keyPairModuleSize;
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageBuf;
  eslt_Length i;

  if ((workSpace == NULL_PTR) || (messageSize == NULL_PTR) || (message == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  /* workSpace state is checked in esl_decryptRSA_prim() */
  else
  {
    /* get underlying workspace pointer */
    keyPairModuleSize = workSpace->modulusSizeInformation;
    messageBuf = workSpace->messageBuffer;

    i = keyPairModuleSize;
    returnValue = esl_decryptRSA_prim(&(workSpace->wsRSADecPrim), cipherSize, cipher, &i, messageBuf);

    if (ESL_ERC_NO_ERROR == returnValue)
    {
      if (i != keyPairModuleSize)
      {
        returnValue = ESL_ERC_RSA_ENCODING_INVALID;
      }
      else if ((returnValue = esl_verifyPaddingRSAEM_V15(messageBuf, &i, blockType)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
      {
        /* retVal already set */
      }
      else
      {
        /* The maximal possible value of i is keyPairModuleSize, see esl_verifyPaddingRSAEM_V15(). */
        if (i == keyPairModuleSize)
        {
          /* messageBuf only contains padding bytes, the actual message is empty
           * returnValue = ESL_ERC_NO_ERROR already set */
          *messageSize = 0u;
        }
        else if (*messageSize < (keyPairModuleSize - i))
        {
          returnValue = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
        }
        else
        {
          *messageSize = keyPairModuleSize - i;
          actMemCpyByteArray(message, &messageBuf[(keyPairModuleSize - *messageSize)], *messageSize); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
          /* returnValue = ESL_ERC_NO_ERROR already set */
        }
      }
    }
  }
  return returnValue;
} /* PRQA S 6060, 6080 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

 /**********************************************************************************************************************
  *  esl_initDecryptRSA_V15()
  *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptRSA_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAdec) workSpace,
                                                     eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule, eslt_Length privateKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent)
{

  eslt_ErrorCode returnValue;

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    /* get underlying workspace pointer */
    if (workSpace->header.size < ESL_MAXSIZEOF_WS_RSA_DEC)
    {
      returnValue = ESL_ERC_WS_TOO_SMALL;
    }
    else
    {
      workSpace->wsRSADecPrim.header.size = ESL_MAXSIZEOF_WS_RSA_DEC_PRIM;
      workSpace->wsRSADecPrim.header.watchdog = workSpace->header.watchdog;
      returnValue = esl_initDecryptRSA_prim(&(workSpace->wsRSADecPrim), keyPairModuleSize, keyPairModule, privateKeyExponentSize, privateKeyExponent);
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
 *  esl_decryptPubRSA_V15()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptPubRSA_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAdec) workSpace,
                                                    eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
                                                    VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message)
{
  /* Cipher was encrypted using private key */
  return esl_decryptInternal(workSpace, cipherSize, cipher, messageSize, message, ASN1_PADDING_BLOCK_TYPE_PRIVATE);
}

/**********************************************************************************************************************
 *  esl_decryptPrivRSA_V15()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptPrivRSA_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAdec) workSpace,
                                                     eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
                                                     VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message)
{
  /* Cipher was encrypted using public key */
  return esl_decryptInternal(workSpace, cipherSize, cipher, messageSize, message, ASN1_PADDING_BLOCK_TYPE_PUBLIC);
}

/**********************************************************************************************************************
 *  esl_decryptRSA_V15()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSA_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAdec) workSpace,
                                                 eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher, VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message)
{
  /* Default uses private key for decryption */
  return esl_decryptPrivRSA_V15(workSpace, cipherSize, cipher, messageSize, message);
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_RSA_V15_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSA_V15_Dec.c
 *********************************************************************************************************************/
