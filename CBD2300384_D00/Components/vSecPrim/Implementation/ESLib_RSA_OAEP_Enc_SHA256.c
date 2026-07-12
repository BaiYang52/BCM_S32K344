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
/*!        \file  ESLib_RSA_OAEP_Enc_SHA256.c
 *        \brief  Crypto library - PKCS #1 RSA OAEP (Encryption)
 *
 *      \details RSA encryption using RSA OAEP encoding scheme according to PKCS #1 v2.2
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
#define ESLIB_RSA_OAEP_ENC_SHA256_SOURCE

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
 *  esl_initEncryptRSASHA256_OAEP()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_initEncryptRSASHA256_OAEP(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPenc) workSpace,
                                                            eslt_Length keyPairModuleSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule, eslt_Length publicKeyExponentSize, VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent)
{
  eslt_ErrorCode returnValue;

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {

    if (workSpace->header.size < ESL_MAXSIZEOF_WS_RSA_OAEP_ENC)
    {
      returnValue = ESL_ERC_WS_TOO_SMALL;
    }
    else
    {
      workSpace->wsHashUnion.SHA256ws.header.size = ESL_SIZEOF_WS_HASHUNION_RSA;
      workSpace->wsHashUnion.SHA256ws.header.watchdog = workSpace->header.watchdog;

      workSpace->wsRSAEncPrim.header.size = (eslt_Length)(sizeof(eslt_WorkSpaceRSAenc_prim) - sizeof(eslt_WorkSpaceHeader));
      workSpace->wsRSAEncPrim.header.watchdog = workSpace->header.watchdog;
      returnValue = esl_initEncryptRSA_prim(&(workSpace->wsRSAEncPrim), keyPairModuleSize, keyPairModule, publicKeyExponentSize, publicKeyExponent);
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
 *  esl_encryptRSASHA256_OAEP_Label_Seed()
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
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRSASHA256_OAEP_Label_Seed(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPenc) workSpace,
                                                                   eslt_Length messageSize, VSECPRIM_P2CONST_PARA(eslt_Byte) message,
                                                                   eslt_Length labelSize, VSECPRIM_P2CONST_PARA(eslt_Byte) label,
                                                                   eslt_Length inSeedSize, VSECPRIM_P2CONST_PARA(eslt_Byte) inSeed,
                                                                   VSECPRIM_P2VAR_PARA(eslt_Length) cipherSize, VSECPRIM_P2VAR_PARA(eslt_Byte) cipher)
{
  eslt_ErrorCode returnValue;
  eslt_Length keyPairModuleSize;
  eslt_Length psLength;
  eslt_Length dbLength;
  eslt_Byte randomSeed[ESL_RSA_OAEP_SIZEOF_HASH];
  VSECPRIM_P2VAR_PARA(eslt_Byte) encodedMessage;
  VSECPRIM_P2VAR_PARA(eslt_Byte) db;
  VSECPRIM_P2VAR_PARA(eslt_Byte) maskedDb;
  VSECPRIM_P2VAR_PARA(eslt_Byte) maskedSeed;
  VSECPRIM_P2CONST_PARA(eslt_Byte) seed;
  VSECPRIM_P2VAR_PARA(eslt_Byte) ps;

  returnValue = ESL_ERC_NO_ERROR;

  if ((workSpace == NULL_PTR) || (message == NULL_PTR) || ((label == NULL_PTR) && (labelSize > 0u)))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    keyPairModuleSize = workSpace->modulusSizeInformation;
    /* General memory layout in workspace:
       | EM                                           | DB                            | Temp  | -    |
       | 0x00 | mSeed | maskedDB                      | lHash | PS   | 0x01 | M       |       | -    | */
    encodedMessage = workSpace->encodedMessageBuffer.encodedMessage;
    db = workSpace->encodedMessageBuffer.dataBlock;
    /* Points to maskedSeed in EM */
    maskedSeed = &encodedMessage[1u];

    /* 7.1.1, Step 1.a: Message length check */
    if (keyPairModuleSize < (2u * (ESL_RSA_OAEP_SIZEOF_HASH + 1u)))
    {
      returnValue = ESL_ERC_RSA_MODULE_OUT_OF_RANGE;
    }
    else if (messageSize > (keyPairModuleSize - (2u * (ESL_RSA_OAEP_SIZEOF_HASH + 1u))))
    {
      returnValue = ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE;
    }
    else
    {
      /* Seed size must match size if hash output */
      if (ESL_RSA_OAEP_SIZEOF_HASH != inSeedSize)
      {
        returnValue = ESL_ERC_PARAMETER_INVALID;
      }
    }

    /* 7.1.1, Step 2: EME-OAEP encoding encoding */
    if (ESL_ERC_NO_ERROR == returnValue)
    {
      /* 7.1.1, Step 2.a: Calculate hash lHash from label L
         Step 2.c: Place lHash at beginning of DB
         Memory contents:
         | EM                                           | DB                            | Temp  | -    |
         | -    | -     | -                             | lHash | -    | -    | -       | -     | -    | */

      /* Initialize hash calculation */
      ESL_RSA_OAEP_INIT_HASH_FCT(&(workSpace->wsHashUnion.SHA256ws));

      if (labelSize > 0u)
      {
        if (actOK != ESL_RSA_OAEP_UPDATE_HASH_FCT(&(workSpace->wsHashUnion.SHA256ws), labelSize, label))
        {
          returnValue = ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW;
        }
      }

      if (ESL_ERC_NO_ERROR == returnValue)
      {
        ESL_RSA_OAEP_FINALIZE_HASH_FCT(&(workSpace->wsHashUnion.SHA256ws), db);
      }
    }

    if (ESL_ERC_NO_ERROR == returnValue)
    {
      /* Length of maskedDB / DB */
      dbLength = keyPairModuleSize - (ESL_RSA_OAEP_SIZEOF_HASH + 1u);
      /* Points to maskedDb in EM */
      maskedDb = &encodedMessage[ESL_RSA_OAEP_SIZEOF_HASH + 1u];

      /* Calculate length of PS in DB */
      psLength = dbLength - (messageSize + ESL_RSA_OAEP_SIZEOF_HASH + 1u);
      /* Points to PS in DB */
      ps = &db[ESL_RSA_OAEP_SIZEOF_HASH];

      /* 7.1.1, Step 2.a: Generate octet string PS consisting of psLength zero octets
         Step 2.c: Prepare constant parts of DB
         Memory contents:
         | EM                                           | DB                            | Temp  | -    |
         | -    | -     | -                             | lHash | PS   | 0x01 | -       | -     | -    | */
      actMemClear(ps, psLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
      ps[psLength] = 0x01u;

      /* 7.1.1, Step 2.c: Concatenate message M with DB
         Memory contents:
         | EM                                           | DB                            | Temp  | -    |
         | -    | -     | -                             | lHash | PS   | 0x01 | M       | -     | -    | */
      actMemCpyByteArray(&ps[psLength + 1u], message, messageSize); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

      /* 7.1.1, Step 2.d: Generate random seed (or use provided value) */
      /* Any seed provided by caller? */
      if (inSeed == NULL_PTR)
      {
        /* No salt provided, generate random value */
        returnValue = esl_getBytesRNG(ESL_RSA_OAEP_SIZEOF_HASH, randomSeed);
        seed = randomSeed;
      }
      else
      {
        /* Use provided seed instead */
        seed = inSeed;
      }

      if (ESL_ERC_NO_ERROR == returnValue)
      {
        /* 7.1.1, Step 2.e: Generate dbMask using mask generation function (MGF) with seed as input
           Memory contents:
           | EM                                           | DB                            | Temp  | -    |
           | -    | -     | dbMask                        | lHash | PS   | 0x01 | M       | hash  | -    | */
        returnValue = esl_generateMaskMGF1RSASHA256_PSS(&(workSpace->wsHashUnion.SHA256ws), workSpace->encodedMessageBuffer.tempVariables,
                                                        ESL_RSA_OAEP_SIZEOF_HASH, seed, dbLength, maskedDb);
      }

      if (ESL_ERC_NO_ERROR == returnValue)
      {
        /* 7.1.1, Step 2.f: Apply mask to DB using XOR to create maskedDB
           Memory contents:
           | EM                                           | DB                            | Temp  | -    |
           | -    | -     | maskedDB                      | lHash | PS   | 0x01 | M       | hash  | -    | */
        actXOR(maskedDb, db, dbLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

        /* 7.1.1, Step 2.g: Generate seedMask using mask generation function (MGF) with maskedDB as input
           Memory contents:
           | EM                                           | DB                            | Temp  | -    |
           | -    | seedM | maskedDB                      | lHash | PS   | 0x01 | M       | hash  | -    | */
        returnValue = esl_generateMaskMGF1RSASHA256_PSS(&(workSpace->wsHashUnion.SHA256ws), workSpace->encodedMessageBuffer.tempVariables,
                                                       dbLength, maskedDb, ESL_RSA_OAEP_SIZEOF_HASH, maskedSeed);
      }

      if (ESL_ERC_NO_ERROR == returnValue)
      {
        /* 7.1.1, Step 2.h: Apply mask to seed using XOR to create maskedSeed
           Memory contents:
           | EM                                           | DB                            | Temp  | -    |
           | -    | mSeed | maskedDB                      | lHash | PS   | 0x01 | M       | hash  | -    | */
        actXOR(maskedSeed, seed, ESL_RSA_OAEP_SIZEOF_HASH); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

        /* 7.1.1, Step 2.i: Set constant zero octet at beginning of EM
           Memory contents:
           | EM                                           | DB                            | Temp  | -    |
           | 0x00 | mSeed | maskedDB                      | lHash | PS   | 0x01 | M       | hash  | -    | */
        encodedMessage[0u] = 0x00u;

        /* 7.1.1, Step 3: RSA encryption */
        returnValue = esl_encryptRSA_prim(&(workSpace->wsRSAEncPrim), keyPairModuleSize, encodedMessage, cipherSize, cipher);
      }
    }
  }

  return returnValue;
} /* PRQA S 6010, 6030, 6050, 6060 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

/**********************************************************************************************************************
 *  esl_encryptRSASHA256_OAEP()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRSASHA256_OAEP(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPenc) workSpace,
                                                        eslt_Length messageSize, VSECPRIM_P2CONST_PARA(eslt_Byte) message,
                                                        VSECPRIM_P2VAR_PARA(eslt_Length) cipherSize, VSECPRIM_P2VAR_PARA(eslt_Byte) cipher)
{
  return esl_encryptRSASHA256_OAEP_Label_Seed(workSpace, messageSize, message, 0u, NULL_PTR, ESL_RSA_OAEP_SIZEOF_HASH, NULL_PTR, cipherSize, cipher);
} /* PRQA S 6060 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

/**********************************************************************************************************************
 *  esl_encryptRSASHA256_OAEP_Seed()
 *********************************************************************************************************************/
 /*!
  *
  * Internal comment removed.
 *
 *
 *
  */
VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRSASHA256_OAEP_Seed(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPenc) workSpace,
                                                             eslt_Length messageSize, VSECPRIM_P2CONST_PARA(eslt_Byte) message,
                                                             eslt_Length inSeedSize, VSECPRIM_P2CONST_PARA(eslt_Byte) inSeed,
                                                             VSECPRIM_P2VAR_PARA(eslt_Length) cipherSize, VSECPRIM_P2VAR_PARA(eslt_Byte) cipher)
{
  return esl_encryptRSASHA256_OAEP_Label_Seed(workSpace, messageSize, message, 0u, NULL_PTR, inSeedSize, inSeed, cipherSize, cipher);
} /* PRQA S 6060 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

/**********************************************************************************************************************
 *  esl_encryptRSASHA256_OAEP_Label()
 *********************************************************************************************************************/
 /*!
  *
  * Internal comment removed.
 *
 *
 *
  */
VSECPRIM_FUNC(eslt_ErrorCode) esl_encryptRSASHA256_OAEP_Label(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAOAEPenc) workSpace,
                                                              eslt_Length messageSize, VSECPRIM_P2CONST_PARA(eslt_Byte) message,
                                                              eslt_Length labelSize, VSECPRIM_P2CONST_PARA(eslt_Byte) label,
                                                              VSECPRIM_P2VAR_PARA(eslt_Length) cipherSize, VSECPRIM_P2VAR_PARA(eslt_Byte) cipher)
{
  return esl_encryptRSASHA256_OAEP_Label_Seed(workSpace, messageSize, message, labelSize, label, ESL_RSA_OAEP_SIZEOF_HASH, NULL_PTR, cipherSize, cipher);
} /* PRQA S 6060 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_RSA_OAEP_SHA2_256_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSA_OAEP_Enc_SHA256.c
 *********************************************************************************************************************/
