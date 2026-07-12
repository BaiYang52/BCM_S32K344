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
/*!        \file  ESLib_RSA_OAEP_Dec_SHA256.c
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
#define ESLIB_RSA_OAEP_DEC_SHA256_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "ESLib.h"
#include "ESLib_Helper.h"

#include "actIRSA.h"
#include "actISHA2_32.h"

#if (VSECPRIM_RSA_OAEP_SHA2_256_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Hash initialization function */
# define ESL_RSA_OAEP_INIT_HASH_FCT(workspace)                        (void)actSHA256Init(&((workspace)->wsSHA256))
/* Hash update function */
# define ESL_RSA_OAEP_UPDATE_HASH_FCT(workspace, inputSize, input)    actSHA256Update(&((workspace)->wsSHA256), (VSECPRIM_P2CONST_PARA(actU8))(input), (actLengthType)(inputSize), (workspace)->header.watchdog)
/* Hash finalization function */
# define ESL_RSA_OAEP_FINALIZE_HASH_FCT(workspace, messageDigest)     (void)actSHA256Finalize(&((workspace)->wsSHA256), (VSECPRIM_P2VAR_PARA(actU8))(messageDigest),  (workspace)->header.watchdog)

/* Size of message digest (hash) */
# define ESL_RSA_OAEP_SIZEOF_HASH                                     ESL_SIZEOF_SHA256_DIGEST

/* Offset of buffer length in work space */
# define ESL_WS_RSA_OAEP_ED_BUFFERLENGTH                              ESL_WS_RSA_OAEP_ED_SHA256_BUFFERLENGTH
/* Offset of message buffer in work space */
# define ESL_WS_RSA_OAEP_ED_BUFFER                                    ESL_WS_RSA_OAEP_ED_SHA256_BUFFER

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  esl_initDecryptRSASHA256_OAEP()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptRSASHA256_OAEP(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPdec) workSpace,
                                                            eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule, eslt_Length privateKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent)
{
  eslt_ErrorCode returnValue;

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    if (workSpace->header.size < ESL_MAXSIZEOF_WS_RSA_OAEP_DEC)
    {
      returnValue = ESL_ERC_WS_TOO_SMALL;
    }
    else
    {
      workSpace->wsHashUnion.SHA256ws.header.size = ESL_SIZEOF_WS_HASHUNION_RSA;
      workSpace->wsHashUnion.SHA256ws.header.watchdog = workSpace->header.watchdog;

      workSpace->wsRSADecPrim.header.size = (eslt_Length)(sizeof(eslt_WorkSpaceRSAdec_prim) - sizeof(eslt_WorkSpaceHeader));
      workSpace->wsRSADecPrim.header.watchdog = workSpace->header.watchdog;
      returnValue = esl_initDecryptRSA_prim(&(workSpace->wsRSADecPrim), keyPairModuleSize, keyPairModule, privateKeyExponentSize, privateKeyExponent);
    }

    if (ESL_ERC_NO_ERROR == returnValue)
    {
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_CALL_FUNCTION */
      /* Store key pair module size / message buffer length in work space */
      workSpace->modulusSizeInformation = keyPairModuleSize;
    }
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  esl_decodeRSASHA256_OAEP()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_decodeRSASHA256_OAEP(VSECPRIM_P2VAR_PARA(eslt_WorkSpace) workSpace, eslt_Length encodedMessageSize, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
                                                       eslt_Length labelSize, VSECPRIM_P2CONST_PARA(eslt_Byte) label,
                                                       VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message)
{
  eslt_ErrorCode returnValue;
  eslt_Length index;
  eslt_Byte combinedValue;
  eslt_Length dummy;
  eslt_Length psLength;
  eslt_Length dbLength;
  eslt_Length messageLength;
  VSECPRIM_P2VAR_PARA(eslt_Byte) encodedMessage;
  VSECPRIM_P2VAR_PARA(eslt_Byte) db;
  VSECPRIM_P2CONST_PARA(eslt_Byte) maskedDb;
  VSECPRIM_P2VAR_PARA(eslt_Byte) maskedSeed;
  VSECPRIM_P2CONST_PARA(eslt_Byte) ps;

  returnValue = ESL_ERC_NO_ERROR;

  if ((workSpace == NULL_PTR) || ((label == NULL_PTR) && (labelSize > 0u)) || (messageSize == NULL_PTR) || (message == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    /* General memory layout in workspace:
       | EM                                           | DB                            | Temp  | -    |
       | 0x00 | mSeed | maskedDB                      | lHash | PS   | 0x01 | M       |       | -    | */
    encodedMessage = workSpace->encodedMessageBufferPtr->encodedMessage;
    db = workSpace->encodedMessageBufferPtr->dataBlock;

    /* 7.1.2, Step 1.c: Length check */
    if (encodedMessageSize < (2u * (ESL_RSA_OAEP_SIZEOF_HASH + 1u)))
    {
      returnValue = ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE;
    }

    /* 7.1.2, Step 3: EME-OAEP encoding decoding */
    if (ESL_ERC_NO_ERROR == returnValue)
    {
      /* Length of maskedDB / DB */
      dbLength = encodedMessageSize - (ESL_RSA_OAEP_SIZEOF_HASH + 1u);
      /* Points to maskedDb in EM */
      maskedDb = &encodedMessage[ESL_RSA_OAEP_SIZEOF_HASH + 1u];
      /* Points to maskedSeed in EM */
      maskedSeed = &encodedMessage[1u];

      /* 7.1.2, Step 3.c: Generate seedMask using mask generation function (MGF) with maskedDB as input
         Memory contents:
         | EM                                           | DB                            | Temp  | -    |
         | Y    | mSeed | maskedDB                      | seedM | -    | -    | -       | hash  | -    | */
      returnValue = esl_generateMaskMGF1RSASHA256_PSS(&(workSpace->wsHashUnionPtr->SHA256ws), workSpace->encodedMessageBufferPtr->tempVariables,
                                                      dbLength, maskedDb, ESL_RSA_OAEP_SIZEOF_HASH, db);

      if (ESL_ERC_NO_ERROR == returnValue)
      {
        /* 7.1.2, Step 3.d: Apply mask to maskedSeed using XOR to get original seed
           Memory contents:
           | EM                                           | DB                            | Temp  | -    |
           | Y    | seed  | maskedDB                      | seedM | -    | -    | -       | -     | -    | */
        actXOR(maskedSeed, db, ESL_RSA_OAEP_SIZEOF_HASH); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

        /* 7.1.2, Step 3.e: Generate dbMask using mask generation function (MGF) with seed as input
           Memory contents:
           | EM                                           | DB                            | Temp  | -    |
           | Y    | seed  | maskedDB                      | dbMask                        | hash  | -    | */
        returnValue = esl_generateMaskMGF1RSASHA256_PSS(&(workSpace->wsHashUnionPtr->SHA256ws), workSpace->encodedMessageBufferPtr->tempVariables,
                                                        ESL_RSA_OAEP_SIZEOF_HASH, maskedSeed, dbLength, db);
      }

      if (ESL_ERC_NO_ERROR == returnValue)
      {
        /* 7.1.2, Step 3.f: Apply mask to maskedDB using XOR to get original DB
           Memory contents:
           | EM                                           | DB                            | Temp  | -    |
           | Y    | seed  | maskedDB                      | lHash'| PS   | ?    | M       | -     | -    | */
        actXOR(db, maskedDb, dbLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

        /* 7.1.2, Step 3.a: Calculate hash lHash from label L
           Step 3.c: Place lHash at beginning of DB
           Memory contents:
           | EM                                           | DB                            | Temp  | -    |
           | Y    | lHash | maskedDB                      | lHash'| PS   | ?    | M       | -     | -    | */

        /* Initialize hash calculation */
        ESL_RSA_OAEP_INIT_HASH_FCT(&(workSpace->wsHashUnionPtr->SHA256ws));

        if (labelSize > 0u)
        {
          if (actOK != ESL_RSA_OAEP_UPDATE_HASH_FCT(&(workSpace->wsHashUnionPtr->SHA256ws), labelSize, label))
          {
            returnValue = ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW;
          }
        }

        if (ESL_ERC_NO_ERROR == returnValue)
        {
          ESL_RSA_OAEP_FINALIZE_HASH_FCT(&(workSpace->wsHashUnionPtr->SHA256ws), maskedSeed);
        }
      }

      if (ESL_ERC_NO_ERROR == returnValue)
      {
        /* Points to PS in DB */
        ps = &db[ESL_RSA_OAEP_SIZEOF_HASH];

        /* 7.1.2, Step 3.g: Evaluate length of PS, by checking octets of DB after lHash' which are zero
           Iterate over complete potential PS to prevent timing analysis */
        psLength = 0u;
        dummy = 0u;
        combinedValue = 0x00u;

        /* Iterate over complete potential PS to prevent timing analysis */
        for (index = 0u; index < (dbLength - ESL_RSA_OAEP_SIZEOF_HASH); index++)
        {
          combinedValue |= ps[index];

          /* All previous values were zero? */
          if (0x00u == combinedValue)
          {
            /* Increment actual length of PS */
            psLength++;
          }
          else
          {
            /* Increment dummy value instead */
            dummy++;
          }
        }

        /* 7.1.2, Step 3.g: Verify that an octet value 0x01 separates PS and M */
        if (0x01u != ps[psLength])
        {
          returnValue = ESL_ERC_RSA_ENCODING_INVALID;
        }
        /* 7.1.2, Step 3.g: Verify that Y has a value of zero */
        else if (0x00u != encodedMessage[0u])
        {
          returnValue = ESL_ERC_RSA_ENCODING_INVALID;
        }
        /* 7.1.2, Step 3.g: Verify lHash and lHash' are equal */
        else if (TRUE != actMemcmp(db, maskedSeed, ESL_RSA_OAEP_SIZEOF_HASH)) /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
        {
          returnValue = ESL_ERC_RSA_ENCODING_INVALID;
        }
        else
        {
          /* 7.1.2, Step 4: Output message M
             Memory contents:
             | EM                                           | DB                            | Temp  | -    |
             | 0x00 | lHash | maskedDB                      | lHash'| PS   | 0x01 | M       | -     | -    | */
          messageLength = dbLength - (psLength + ESL_RSA_OAEP_SIZEOF_HASH + 1u);

          if (*messageSize < messageLength)
          {
            returnValue = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
          }
          else
          {
            *messageSize = messageLength;
            actMemCpyByteArray(message, &ps[psLength + 1u], messageLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
          }
        }
      }
    }
  }

  return returnValue;
} /* PRQA S 6010, 6030, 6060, 6080 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

/**********************************************************************************************************************
 *  esl_decryptRSASHA256_OAEP_Label()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSASHA256_OAEP_Label(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPdec) workSpace,
                                                              eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
                                                              eslt_Length labelSize, VSECPRIM_P2CONST_PARA(eslt_Byte) label,
                                                              VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message)
{
  eslt_ErrorCode returnValue;

  VSECPRIM_P2VAR_PARA(eslt_Byte) encodedMessage;
  eslt_Length keyPairModuleSize;
  eslt_Length encodedMessageSize;

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    keyPairModuleSize = workSpace->modulusSizeInformation;
    encodedMessage = workSpace->encodedMessageBuffer.encodedMessage;
    /* General memory layout in workspace:
       | EM                                           | DB                            | Temp  | -    |
       | 0x00 | mSeed | maskedDB                      | lHash | PS   | 0x01 | M       |       | -    | */
    encodedMessageSize = keyPairModuleSize;

    /* 7.1.2, Step 2: RSA decryption */
    returnValue = esl_decryptRSA_prim(&(workSpace->wsRSADecPrim), cipherSize, cipher, &encodedMessageSize, encodedMessage);

    if (ESL_ERC_NO_ERROR == returnValue)
    {
      /* 7.1.2, Step 3: EME-OAEP encoding decoding */
      eslt_WorkSpace wsDecoding;
      wsDecoding.encodedMessageBufferPtr = &(workSpace->encodedMessageBuffer);
      wsDecoding.wsHashUnionPtr = &(workSpace->wsHashUnion);
      wsDecoding.modulusInformationPtr = &(workSpace->modulusSizeInformation);

      returnValue = esl_decodeRSASHA256_OAEP(&wsDecoding, encodedMessageSize, labelSize, label, messageSize, message);
    }
  }

  return returnValue;
} /* PRQA S 6060 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

/**********************************************************************************************************************
 *  esl_decryptRSASHA256_OAEP()
 *********************************************************************************************************************/
 /*!
  *
  * Internal comment removed.
 *
 *
 *
  */
VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSASHA256_OAEP(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPdec) workSpace,
                                                        eslt_Length cipherSize, VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
                                                        VSECPRIM_P2VAR_PARA(eslt_Length) messageSize, VSECPRIM_P2VAR_PARA(eslt_Byte) message)
{
  return esl_decryptRSASHA256_OAEP_Label(workSpace, cipherSize, cipher, 0u, NULL_PTR, messageSize, message);
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_RSA_OAEP_SHA2_256_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSA_OAEP_Dec_SHA256.c
 *********************************************************************************************************************/
