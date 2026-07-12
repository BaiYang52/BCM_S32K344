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
/*!        \file  ESLib_RSA_OAEP_DecCRT_SHA1.c
 *        \brief  Crypto library - PKCS #1 RSA OAEP (Decryption)
 *
 *      \details RSA decryption using RSA OAEP encoding scheme according to PKCS #1 v2.2
 *               Currently the actClib version is used.
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
#define ESLIB_RSA_OAEP_DEC_CRT_SHA1_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "ESLib.h"
#include "ESLib_Helper.h"

#include "actIRSA.h"
#include "actISHA.h"

#if (VSECPRIM_RSA_OAEP_SHA1_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Hash initialization function */
# define ESL_RSA_OAEP_INIT_HASH_FCT(workspace)                        actSHAInit(&((workspace)->wsSHA1))
/* Hash update function */
# define ESL_RSA_OAEP_UPDATE_HASH_FCT(workspace, inputSize, input)    actSHAUpdate(&((workspace)->wsSHA1), (VSECPRIM_P2CONST_PARA(actU8))(input), (actLengthType)(inputSize), (workspace)->header.watchdog)
/* Hash finalization function */
# define ESL_RSA_OAEP_FINALIZE_HASH_FCT(workspace, messageDigest)     actSHAFinalize(&((workspace)->wsSHA1), (VSECPRIM_P2VAR_PARA(actU8))(messageDigest),  (workspace)->header.watchdog)

/* Size of message digest (hash) */
# define ESL_RSA_OAEP_SIZEOF_HASH                                     ESL_SIZEOF_SHA1_DIGEST

/* Offset of buffer length in work space */
# define ESL_WS_RSA_OAEP_ED_BUFFERLENGTH                              ESL_WS_RSA_OAEP_ED_SHA1_BUFFERLENGTH
/* Offset of message buffer in work space */
# define ESL_WS_RSA_OAEP_ED_BUFFER                                    ESL_WS_RSA_OAEP_ED_SHA1_BUFFER

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  esl_initDecryptRSACRTSHA1_OAEP()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptRSACRTSHA1_OAEP(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTOAEPdec) workSpace,
                                                             eslt_Length keyPairPrimePSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeP,
                                                             eslt_Length keyPairPrimeQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeQ,
                                                             eslt_Length privateKeyExponentDPSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDP,
                                                             eslt_Length privateKeyExponentDQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDQ,
                                                             eslt_Length privateKeyInverseQISize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyInverseQI)
{
  eslt_ErrorCode returnValue;
  eslt_Length keyPairModuleSize;
  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    keyPairModuleSize = keyPairPrimePSize + keyPairPrimeQSize;

    if (workSpace->header.size < ESL_MAXSIZEOF_WS_RSA_CRT_OAEP_DEC)
    {
      returnValue = ESL_ERC_WS_TOO_SMALL;
    }
    else
    {
      workSpace->wsHashUnion.SHA1ws.header.size = ESL_SIZEOF_WS_HASHUNION_RSA;
      workSpace->wsHashUnion.SHA1ws.header.watchdog = workSpace->header.watchdog;

      workSpace->wsRSACRTDecPrim.header.size = (eslt_Length)(ESL_MAXSIZEOF_WS_RSA_CRT_DEC_PRIM);
      workSpace->wsRSACRTDecPrim.header.watchdog = workSpace->header.watchdog;
      returnValue = esl_initDecryptRSACRT_prim(&(workSpace->wsRSACRTDecPrim),
                                               keyPairPrimePSize, keyPairPrimeP,
                                               keyPairPrimeQSize, keyPairPrimeQ,
                                               privateKeyExponentDPSize, privateKeyExponentDP, privateKeyExponentDQSize, privateKeyExponentDQ, privateKeyInverseQISize, privateKeyInverseQI);
    }

    if (ESL_ERC_NO_ERROR == returnValue)
    {
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_CALL_FUNCTION */
      /* Store key pair module size / message buffer length in work space */
      workSpace->modulusSizeInformation = keyPairModuleSize;
    }
  }

  return returnValue;
} /* PRQA S 6060 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

/**********************************************************************************************************************
 *  esl_decryptRSACRTSHA1_OAEP_Label()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSACRTSHA1_OAEP_Label(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTOAEPdec) workSpace,
                                                               eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
                                                               eslt_Length labelSize, VSECPRIM_P2CONST_PARA(eslt_Byte) label,
                                                               VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message)
{
  eslt_ErrorCode returnValue;
  eslt_Length keyPairModuleSize;
  eslt_Length encodedMessageSize;
  VSECPRIM_P2VAR_PARA(eslt_Byte) encodedMessage;
  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    keyPairModuleSize = (eslt_Length)(workSpace->modulusSizeInformation);
    encodedMessage = workSpace->encodedMessageBuffer.encodedMessage;
    /* General memory layout in workspace:
       | EM                                           | DB                            | Temp  | -    |
       | 0x00 | mSeed | maskedDB                      | lHash | PS   | 0x01 | M       |       | -    | */
    encodedMessageSize = keyPairModuleSize;

    /* 7.1.2, Step 2: RSA decryption */
    returnValue = esl_decryptRSACRT_prim(&(workSpace->wsRSACRTDecPrim), cipherSize, cipher, &encodedMessageSize, encodedMessage);

    if (ESL_ERC_NO_ERROR == returnValue)
    {
      eslt_WorkSpace wsDecoding;
      wsDecoding.encodedMessageBufferPtr = &(workSpace->encodedMessageBuffer);
      wsDecoding.wsHashUnionPtr = &(workSpace->wsHashUnion);
      wsDecoding.modulusInformationPtr = &(workSpace->modulusSizeInformation);

      /* 7.1.2, Step 3: EME-OAEP encoding decoding */
      returnValue = esl_decodeRSASHA1_OAEP(&wsDecoding, encodedMessageSize, labelSize, label, messageSize, message);
    }
  }

  return returnValue;
} /* PRQA S 6060 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

/**********************************************************************************************************************
 *  esl_decryptRSACRTSHA1_OAEP()
 *********************************************************************************************************************/
 /*!
  *
  * Internal comment removed.
 *
 *
 *
  */
VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSACRTSHA1_OAEP(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTOAEPdec) workSpace,
                                                         eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
                                                         VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message)
{
  return esl_decryptRSACRTSHA1_OAEP_Label(workSpace, cipherSize, cipher, 0u, NULL_PTR, messageSize, message);
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_RSA_OAEP_SHA1_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSA_OAEP_DecCRT_SHA1.c
 *********************************************************************************************************************/
