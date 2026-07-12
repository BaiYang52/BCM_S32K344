/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  ESLib_RSA_PSS_Ver.c
 *        \brief  Crypto library - PKCS #1 RSA PSS signature generation (SHA1, SHA-256 or RIPEMD160)
 *
 *      \details RSA signature generation using RSA PSS encoding scheme according to PKCS #1 v2.2
 *               A signature scheme consists of (at least) an encoding method and a signature primitive. In RSA, the
 *               signature primitive is a power function, whereas the encoding method may consist of more steps.
 *               The functions in this file prepare and execute the encoding method. As a last point, the signature
 *               primitive is called.
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
#define ESLIB_RSA_PSS_VER_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ESLib.h"
#include "ESLib_Helper.h"
#include "ESLib_RSA_Common.h"
#include "actIRSA.h"

#if (VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
# define VSECPRIM_WRAPPER_SHA1
#endif
#if (VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON)
# define VSECPRIM_WRAPPER_SHA256
#endif
#if (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
# define VSECPRIM_WRAPPER_RIPEMD160
#endif

#if ((VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON) ) /* COV_VSECPRIM_NO_SAFE_CONFIG TF xf tf xf */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
 /* Trailer field of encoded message */
# define ESL_RSA_PSS_TRAILER                                          (0xBCu)

 /**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef eslt_WorkSpaceUnionHashFunctions_RSA eslt_WorkSpaceUnionHashFunctions;
# include "actHashFctWrappers_Implementation.h" /* PRQA S 5087 */ /* MD_VSECPRIM_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
  esl_checkSaltSizeVer()
 **********************************************************************************************************************/
/*! \brief        checks and adjusts saltSize for further computations
 *  \details       Checks if input argument for saltSize was -1 and
 *                 if so, sets saltSize to the value induced by available memory. Also checks if saltSize exceeds
 *                 remaining memory space (dbLength - 1) in subarray db of workSpace->encodedMessage, where the salt
 *                 will be stored.
 *  \param[in,out] saltSize           (in) (pointer to) original size of salt
 *                                    (out) original or modified size of salt
 *  \param[in]     dbLength           Size of key pair module
 *  \param[in]     estimatedPSLength  estimated length of the padding string, used to determine the available
 *                                    memory space
 *  \return        ESL_ERC_NO_ERROR   Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Value refered to by saltSize is too big
 *  \context       TASK
 *  \reentrant     TRUE
 *  \pre           saltSize must be a valid pointer
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_checkSaltSizeVer(
  VSECPRIM_P2VAR_PARA(eslt_Length) saltSize,
  eslt_Length dbLength,
  eslt_Length estimatedPSLength);

/**********************************************************************************************************************
  esl_checkStandardConditionsEncodedMessage()
 **********************************************************************************************************************/
/*! \brief    Checks whether the encoded message fulfills standard conditions on the first and the last eight bits.
 *  \details   The first bit must be zero, the last byte must be the value 0xBC.
 *  \param[in]     workSpace         Pointer to RSA verification workspace
 *  \param[in]     signatureSize     size of the signature to be verified
 *  \param[in]     keyPairModuleSize size of the kayPairModule
 *  \param[in]     encodedMessage    encoded message which is checked for the fulfillment of the standard conditions
 *  \param[in,out] emBits            (in) pointer to buffer to store the length of the encoded message in bits
 *                                   (out) length of the encoded message in bits
 *  \param[in,out] emLength          (in) pointer to buffer to store the length of the encoded message in bytes
 *                                   (out) length of the encoded message in bytes
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Value refered to by saltSize is too big
 *  \context       TASK
 *  \reentrant     TRUE
 *  \pre           wsRSAVer, encodedMessage, emBits and emLength must be  valid pointers
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_checkStandardConditionsEncodedMessage(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceRSAver_prim) workSpace,
  eslt_Length signatureSize,
  eslt_Length keyPairModuleSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) encodedMessage,
  VSECPRIM_P2VAR_PARA(eslt_Size32) emBits,
  VSECPRIM_P2VAR_PARA(eslt_Length) emLength);

/**********************************************************************************************************************
  esl_initVerifyRSA_PSS()
 *********************************************************************************************************************/
/*! \brief         Initialize signature verification
 *  \details       This function initializes the signature verification based on RSA PSS, independent from the
 *                 underlying hash function: the hashing procedure is initialized and the workspace for the RSA
 *                 signature primitive is set up.
 *  \param[in,out] workSpace              (in)  Signature verification work space
 *                                        (out) Signature verification work space, contained workSpaces initialized
 *  \param[in]     keyPairModuleSize      Size of RSA key pair module
 *  \param[in]     keyPairModule          RSA key pair module
 *  \param[in]     publicKeyExponentSize  Size of public RSA key pair exponent
 *  \param[in]     publicKeyExponent      Public RSA key pair exponent
 *  \param[in]     hashID                 parameter to identify underlying hash function
 *  \return        ESL_ERC_NO_ERROR            Operation successful
 *                 ESL_ERC_PARAMETER_INVALID   Input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL        Work space too small
 *                 ESL_ERC_RSA_PUBKEY_INVALID  The public exponent is invalid
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           -
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initVerifyRSA_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length keyPairModuleSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length publicKeyExponentSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent,
  eslt_HashAlgorithm hashID);

/**********************************************************************************************************************
  esl_updateVerifyRSA_PSS()
 *********************************************************************************************************************/
/*! \brief         Updates underlying hash function of the RSA PSS verification scheme with original message
 *  \details       This function updates the signature verification based on RSA PSS, independent from the underlying
 *                 hash function. The message to be hashed is passed is passed to the hashing update routine.
 *  \param[in,out] workSpace       (in)  Signature verification work space
 *                                 (out) Hash workspace contained in workSpace updated with input to be hashed
 *  \param[in]     inputSize       Size of data to be hashed
 *  \param[in]     input           Data to be hashed
 *  \param[in]     hashID          parameter to identify underlying hash function
 *  \return        ESL_ERC_NO_ERROR               Operation successful
 *                 ESL_ERC_PARAMETER_INVALID   Input parameter is NULL
 *                 ESL_ERC_TOTAL_LENGTH_OVERFLOW  Maximum total input length for hashing exceeded
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           Work space initialized
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_updateVerifyRSA_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   eslt_Length inputSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) input,
  eslt_HashAlgorithm hashID);

/**********************************************************************************************************************
  esl_finalizeHashVerifyRSA_PSS()
 *********************************************************************************************************************/
/*! \brief         Computes of the hash value of the original message
 *  \details       This function finalizes the computation of the hash value of the original message as a part of the
 *                 RSA PSS signature verification procedure, independent from the underlying hash function.
 *  \param[in,out] workSpace      (in)  Signature verification work space
 *                                (out) Signature verification work space
 *  \param[in,out] messageDigest  (in)  buffer to store the hash value
 *                                (out) hash value of the original message
 *  \param[in]     hashID               parameter to identify underlying hash function
 *  \return        ESL_ERC_NO_ERROR  always
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           workSpace is initialized and a valid pointer, messageDigest is a valid pointer
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) esl_finalizeHashVerifyRSA_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest,
  eslt_HashAlgorithm hashID);

/**********************************************************************************************************************
  esl_verifySigVerifyRSA_PSS()
 *********************************************************************************************************************/
/*! \brief         Decoding step of the signature verification scheme based on RSA PSS
 *  \details       This function applies the RSA verification primitive to the given signature and performs the
 *                 PSS decoding step, independent from the underlying hash function.
 *  \param[in,out] workSpace (in)  Signature verification work space
 *                           (out)  Signature verification work space
 *  \param[in]     messageDigest   hash value of the original message
 *  \param[in]     hashDigestSize  length of the hash value
 *  \param[in]     saltSize  Expected size of salt,
 *                           -1 if salt used during signature generation should be evaluated automatically
 *  \param[in]     signatureSize  Size of signature
 *  \param[in]     signature  Signature to be verified
 *  \param[in]     hashID               parameter to identify underlying hash function
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_RSA_SIGNATURE_INVALID  Signature verification failed
 *                 ESL_ERC_ERROR error during mask generation step
 *                 ESL_ERC_TOTAL_LENGTH_OVERFLOW total length of data to be hashed (during calculation of salted
 *                 hash value) too long
 *                 ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 *                 ESL_ERC_RSA_SIGNATURE_OUT_OF_RANGE signature > modulus
 *                 ESL_ERC_OUTPUT_SIZE_TOO_SHORT      buffer to store decoded signature is too short
 *                 ESL_ERC_WS_STATE_INVALID           RSA verification primitive workspace in invalid state
 *                 ESL_ERC_WS_TOO_SMALL               RSA verification primitive workspace too small
 *                 ESL_ERC_RSA_ENCODING_INVALID       RSA encoded message length too short
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           workSpace is initialized and a valid pointer, messageDigest and signature are valid pointers
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSA_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length hashDigestSize,
   eslt_Length saltSize,
   eslt_Length signatureSize,
   VSECPRIM_P2CONST_PARA(eslt_Byte) signature,
  eslt_HashAlgorithm hashID);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  esl_checkSaltSizeVer()
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
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_checkSaltSizeVer(
  VSECPRIM_P2VAR_PARA(eslt_Length) saltSize,
  eslt_Length dbLength,
  eslt_Length estimatedPSLength)
{
  eslt_ErrorCode returnValue = ESL_ERC_NO_ERROR;

  /* Auto-evaluation of salt requested? */
  if (*saltSize == (eslt_Length)-1) /* PRQA S 2895 */ /* MD_MSR_Dir1.1 */
  {
    /* Set salt size depending on evaluated length of PS */
    *saltSize = (dbLength - 1u) - estimatedPSLength; /* SBSW_VSECPRIM_WRITE_POINTER */
  }

  /* 9.1.2, Step 3: Salt and 0x01 have to fit into DB */
  if ((*saltSize + 1u) > dbLength)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  esl_checkStandardConditionsEncodedMessage()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_checkStandardConditionsEncodedMessage(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceRSAver_prim) workSpace,
  eslt_Length signatureSize,
  eslt_Length keyPairModuleSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) encodedMessage,
  VSECPRIM_P2VAR_PARA(eslt_Size32) emBits,
  VSECPRIM_P2VAR_PARA(eslt_Length) emLength)
{
  eslt_ErrorCode returnValue = ESL_ERC_NO_ERROR;

  /* 9.1.2, Step 4: Rightmost octet of EM must contain "trailer field" (also see 9.1, Note 1) */
  if ((signatureSize != keyPairModuleSize) || (ESL_RSA_PSS_TRAILER != encodedMessage[keyPairModuleSize - 1u]))
  {
    returnValue = ESL_ERC_RSA_SIGNATURE_INVALID;
  }
  else
  {
    *emBits = actRSAPublicKeyGetBitLength(&(workSpace->wsRSAPrimBuf.wsRSARing)) - 1u; /* SBSW_VSECPRIM_WRITE_POINTER */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    *emLength = (eslt_Length)((*emBits + 7u) / 8u); /* SBSW_VSECPRIM_WRITE_POINTER */

    /* 9.1.2, Step 8: Leftmost 8emLen - emBits bits of the leftmost octet in maskedDB are all zero? */
    if ((encodedMessage[0u] & (esl_RSAPSSEncodingMask[*emBits & 0x07u] ^ 0xFFu)) != 0x00u)
    {
      returnValue = ESL_ERC_RSA_SIGNATURE_INVALID;
    }
  }

  return returnValue;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  esl_initVerifyRSA_PSS()
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
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initVerifyRSA_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
  eslt_Length keyPairModuleSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
  eslt_Length publicKeyExponentSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent,
  eslt_HashAlgorithm hashID)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  actRETURNCODE actReturnValue;

  if ((keyPairModule == NULL_PTR) || (publicKeyExponent == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    actReturnValue = actHashInit(&(workSpace->wsHashUnion), hashID); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actReturnValue == actOK)
    {
      workSpace->wsRSAVerPrim.header.size = (eslt_Length)(ESL_MAXSIZEOF_WS_RSA_PRIM); /* SBSW_VSECPRIM_WRITE_POINTER */
      workSpace->wsRSAVerPrim.header.watchdog = workSpace->header.watchdog; /* SBSW_VSECPRIM_WRITE_POINTER */
      returnValue = esl_initVerifyRSA_prim(&(workSpace->wsRSAVerPrim), keyPairModuleSize, keyPairModule,
                                           publicKeyExponentSize, publicKeyExponent); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }

    if (returnValue == ESL_ERC_NO_ERROR)
    {
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_CALL_FUNCTION */

      /* Store key pair module size / message buffer length in work space */
      workSpace->modulusSizeInformation = keyPairModuleSize; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
    }
  }

  return returnValue;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  esl_updateVerifyRSA_PSS()
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
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_updateVerifyRSA_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
  eslt_Length inputSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input,
  eslt_HashAlgorithm hashID)
{
  eslt_ErrorCode returnValue = ESL_ERC_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  actRETURNCODE actReturnValue;

  if (input == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    /* 9.1.2, Step 2 */
    actReturnValue = actHashUpdate(&(workSpace->wsHashUnion), input, inputSize, hashID); /* SBSW_VSECPRIM_CALL_FUNCTION */

    if (actReturnValue != actOK)
    {
      returnValue = ESL_ERC_TOTAL_LENGTH_OVERFLOW;
    }
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeHashVerifyRSA_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) esl_finalizeHashVerifyRSA_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest,
  eslt_HashAlgorithm hashID)
{
  /* 9.1.2, Step 2 */
  (void)actHashFinalize(&(workSpace->wsHashUnion), messageDigest, hashID); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  esl_verifySigVerifyRSA_PSS()
 *********************************************************************************************************************/
/*!
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
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSA_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
  eslt_Length hashDigestSize,
  eslt_Length saltSize,
  eslt_Length signatureSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) signature,
  eslt_HashAlgorithm hashID)
{
  eslt_ErrorCode returnValue;
  eslt_Length keyPairModuleSize, saltLength;
  eslt_Length i;
  eslt_Size32 emBits = 0u;
  eslt_Length emLength = 0u;
  eslt_Length psLength, dbLength;

  /* Define arrays to provide easier access to different subarrays in the encodedMessage and dataBlock:
  saltedHash: beginning of the subarray "saltedHash" in encodedMessage, will be compared to the hash
              value that is computed in this function to determine whether the signature is valid or not
  db: points to dataBlock
  dbSalt: beginning of the subarray "salt" in dataBlock
  tempHash: points to tempVariables, in the end the referenced buffer stores hash( [padding | hash(M) | salt] ) */

  VSECPRIM_P2CONST_PARA(eslt_Byte) saltedHash;
  VSECPRIM_P2VAR_PARA(eslt_Byte) db;
  VSECPRIM_P2CONST_PARA(eslt_Byte) dbSalt;
  VSECPRIM_P2VAR_PARA(eslt_Byte) tempHash;

  db = &(workSpace->encodedMessageBuffer.dataBlock[0]);
  tempHash = &(workSpace->encodedMessageBuffer.tempVariables[0]);

  saltLength = saltSize;
  keyPairModuleSize = workSpace->modulusSizeInformation;

  /* 8.1.2, Step 2: apply RSA verification primitive to given signature; this yields the encoded message */
  returnValue = esl_verifyRSA_prim(&(workSpace->wsRSAVerPrim), signatureSize, signature,
                                    &keyPairModuleSize, workSpace->encodedMessageBuffer.encodedMessage); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  /* 8.1.2, Step 3: EMSA-PSS verification - decode the encoded message
     9.1.2, Steps 4 and 6: trailer check and check for zeros in leftmost octet */
  if (returnValue == ESL_ERC_NO_ERROR)
  {
    returnValue = esl_checkStandardConditionsEncodedMessage(&(workSpace->wsRSAVerPrim), /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */
                                                            signatureSize, keyPairModuleSize,
                                                            workSpace->encodedMessageBuffer.encodedMessage,
                                                            &emBits, &emLength);
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    /* Note: RFC8017 specifies to ensure emLength >= hashDigestSize + saltLength + 2. 
     * However, this is not possible here, since this would eliminate the AutoSalt option (where saltSize = -1).
     * We ensure the following: (1) emLength >= hashDigestSize + 2u 
     *                          (2) dbLength >= saltLength + 1
     * The function computes                        dbLength = emLength - hashDigestSize - 1, which is a well defined subtraction due to (1).
     * This leads to                                           emLength - hashDigestSize - 1 >= saltLength + 1, using (2),
     * and thus the checks here are equivalent to ensuring     emLength >= hashDigestSize + saltLength + 2.     
     * Note: We ensure (1) to make sure that dbLength >= 1, so that dbLength - 1 >= 0.
     */
    if (emLength < (hashDigestSize + 2u))
    {
      returnValue = ESL_ERC_RSA_ENCODING_INVALID;
    }
    else
    {
      /* Length of maskedDB and DB (both buffers have the same length) */
      dbLength = emLength - hashDigestSize - 1u;
      saltedHash = &(workSpace->encodedMessageBuffer.encodedMessage[dbLength]);

      /* 9.1.2, Step 7: Generate dbMask using mask generation function (MGF) */
      returnValue = esl_generateMaskMGF1(hashID, &(workSpace->wsHashUnion), tempHash, hashDigestSize, hashDigestSize,
        saltedHash, dbLength, db); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      if (returnValue == ESL_ERC_NO_ERROR)
      {
        /* 9.1.2, Step 8: Apply mask to maskedDB using XOR to create DB */
        actXOR(db, workSpace->encodedMessageBuffer.encodedMessage, dbLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

        /* 9.1.2, Step 9: Set leftmost 8emLen - emBits bits of the leftmost octet in DB to zero */
        db[0u] &= esl_RSAPSSEncodingMask[emBits & 0x07u]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */

        /* 9.1.2, Step 10: Evaluate length of PS, by checking how many of the leftmost octets in DB are zero  */
        for (i = 0u; i < (dbLength - 1u); i++)
        {
          if (0x00u != db[i])
          {
            break;
          }
        }

        returnValue = esl_checkSaltSizeVer(&saltLength, dbLength, i); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */

        if (returnValue == ESL_ERC_NO_ERROR)
        {
          /* Calculate length of the zero padding string in DB
             Officially, the last entry of the padding string is 0x01. This entry is ignored with respect to
             the length of the padding string, that is why -1u is subtracted. */
          psLength = dbLength - saltLength - 1u;

          /* 9.1.2, Step 11: Salt is the last octets in DB
             May point behind actual DB, when saltSize == 0. In this case, nothing is written in dbSalt. */
          dbSalt = &db[psLength + 1u];

          /* 9.1.2, Step 10: Evaluated length of PS has to match calculated one, which is dependent on salt length
          First octet after PS must be 0x01 */
          if ((i != psLength) || (db[psLength] != 0x01u))
          {
            returnValue = ESL_ERC_RSA_SIGNATURE_INVALID;
          }
          else
          {
            /* 9.1.2, Steps 12-13: Calculate salted hash H' from given message digest (= hash of original message)
                and the salt */
            returnValue = esl_calcSaltedHashRSA_PSS(&(workSpace->wsHashUnion), saltLength, dbSalt,
              messageDigest, hashDigestSize, tempHash, hashID); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
          }
        }
      }

      if (returnValue == ESL_ERC_NO_ERROR)
      {
        /* 9.1.2, Steps 14: Compare H and H' */
        if (actMemcmp(saltedHash, tempHash, hashDigestSize) != TRUE) /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
        {
          returnValue = ESL_ERC_RSA_SIGNATURE_INVALID;
        }
      }
    }
  }

  return returnValue;
} /* PRQA S 6030, 6060, 6080 */ /* MD_MSR_STCYC, MD_VSECPRIM_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# if (VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/**********************************************************************************************************************
 *  esl_initVerifyRSASHA1_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initVerifyRSASHA1_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
eslt_Length keyPairModuleSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
eslt_Length publicKeyExponentSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((returnValue = esl_CheckWorkSpaceHeader(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_VER)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* returnValue already set */
  }
  else
  {
    workSpace->wsHashUnion.SHA1ws.header.size = ESL_MAXSIZEOF_WS_SHA1;
    workSpace->wsHashUnion.SHA1ws.header.watchdog = workSpace->header.watchdog;
    returnValue = esl_initVerifyRSA_PSS(workSpace, keyPairModuleSize, keyPairModule, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                        publicKeyExponentSize, publicKeyExponent, ESL_HA_SHA1);
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_updateVerifyRSASHA1_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateVerifyRSASHA1_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
eslt_Length inputSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) input)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_VER, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    returnValue = esl_updateVerifyRSA_PSS(workSpace, inputSize, input, ESL_HA_SHA1); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    if (returnValue == ESL_ERC_TOTAL_LENGTH_OVERFLOW)
    {
      returnValue = ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW;
    }
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeHashVerifyRSASHA1_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashVerifyRSASHA1_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest)
{
  eslt_ErrorCode returnValue = ESL_ERC_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if ((workSpace == NULL_PTR) || (messageDigest == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_VER, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    esl_finalizeHashVerifyRSA_PSS(workSpace, messageDigest, ESL_HA_SHA1); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA1_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA1_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
eslt_Length saltSize,
eslt_Length signatureSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  /* null pointer checks; the validity of pointer 'signature' will be checked later */
  if ((workSpace == NULL_PTR) || (messageDigest == NULL_PTR) || (signature == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_VER, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    returnValue = esl_verifySigVerifyRSA_PSS(workSpace, messageDigest, ESL_SIZEOF_SHA1_DIGEST, saltSize, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                             signatureSize, signature, ESL_HA_SHA1);
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA1_PSS_AutoSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA1_PSS_AutoSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
eslt_Length signatureSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_verifySigVerifyRSASHA1_PSS(workSpace, messageDigest, (eslt_Length)-1, signatureSize, signature); /* PRQA S 2895 */ /* MD_MSR_Dir1.1 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA1_PSS_NoSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA1_PSS_NoSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
eslt_Length signatureSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_verifySigVerifyRSASHA1_PSS(workSpace, messageDigest, 0u, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA1_PSS_DigestLengthSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA1_PSS_DigestLengthSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
eslt_Length signatureSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_verifySigVerifyRSASHA1_PSS(workSpace, messageDigest, ESL_SIZEOF_SHA1_DIGEST, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                               signatureSize, signature);
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA1_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA1_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
eslt_Length saltSize,
eslt_Length signatureSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  eslt_Byte hash[ESL_SIZEOF_SHA1_DIGEST];

  actMemClear(hash, ESL_SIZEOF_SHA1_DIGEST); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_VER, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    (void)esl_finalizeHashVerifyRSASHA1_PSS(workSpace, hash);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
    returnValue = esl_verifySigVerifyRSASHA1_PSS(workSpace, hash, saltSize, signatureSize, signature);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA1_PSS_AutoSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA1_PSS_AutoSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
eslt_Length signatureSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeVerifyRSASHA1_PSS(workSpace, (eslt_Length)-1, signatureSize, signature); /* PRQA S 2895 */ /* MD_MSR_Dir1.1 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA1_PSS_NoSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA1_PSS_NoSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
eslt_Length signatureSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeVerifyRSASHA1_PSS(workSpace, 0u, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA1_PSS_DigestLengthSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA1_PSS_DigestLengthSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
eslt_Length signatureSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeVerifyRSASHA1_PSS(workSpace, ESL_SIZEOF_SHA1_DIGEST, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}
# endif /* (VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
/**********************************************************************************************************************
 *  esl_initVerifyRSASHA256_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initVerifyRSASHA256_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
  eslt_Length keyPairModuleSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
  eslt_Length publicKeyExponentSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((returnValue = esl_CheckWorkSpaceHeader(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_VER)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* returnValue already set */
  }
  else
  {
    workSpace->wsHashUnion.SHA256ws.header.size = ESL_MAXSIZEOF_WS_SHA256; /* SBSW_VSECPRIM_WRITE_POINTER */
    workSpace->wsHashUnion.SHA256ws.header.watchdog = workSpace->header.watchdog; /* SBSW_VSECPRIM_WRITE_POINTER */
    returnValue = esl_initVerifyRSA_PSS(workSpace, keyPairModuleSize, keyPairModule, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                        publicKeyExponentSize, publicKeyExponent, ESL_HA_SHA2_256);
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_updateVerifyRSASHA256_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateVerifyRSASHA256_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
  eslt_Length inputSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_VER, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    returnValue = esl_updateVerifyRSA_PSS(workSpace, inputSize, input, ESL_HA_SHA2_256); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    if (returnValue == ESL_ERC_TOTAL_LENGTH_OVERFLOW)
    {
      returnValue = ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW;
    }
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeHashVerifyRSASHA256_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashVerifyRSASHA256_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest)
{
  eslt_ErrorCode returnValue = ESL_ERC_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  if ((workSpace == NULL_PTR) || (messageDigest == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_VER, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    esl_finalizeHashVerifyRSA_PSS(workSpace, messageDigest, ESL_HA_SHA2_256); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_RSAPSSSHA256_FINALIZE_HASH */
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA256_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA256_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
  eslt_Length saltSize,
  eslt_Length signatureSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  if ((workSpace == NULL_PTR) || (messageDigest == NULL_PTR) || (signature == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_VER, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    returnValue = esl_verifySigVerifyRSA_PSS(workSpace, messageDigest, ESL_SIZEOF_SHA256_DIGEST, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                             saltSize, signatureSize, signature, ESL_HA_SHA2_256);
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA256_PSS_AutoSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA256_PSS_AutoSalt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
  eslt_Length signatureSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue =esl_verifySigVerifyRSASHA256_PSS(workSpace, messageDigest, (eslt_Length)-1, signatureSize, signature);  /* PRQA S 2895 */ /* MD_MSR_Dir1.1 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA256_PSS_NoSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA256_PSS_NoSalt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
  eslt_Length signatureSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_verifySigVerifyRSASHA256_PSS(workSpace, messageDigest, 0u, signatureSize, signature);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA256_PSS_DigestLengthSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA256_PSS_DigestLengthSalt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
  eslt_Length signatureSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_verifySigVerifyRSASHA256_PSS(workSpace, messageDigest, ESL_SIZEOF_SHA256_DIGEST,
                                                 signatureSize, signature);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA256_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA256_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
  eslt_Length saltSize,
  eslt_Length signatureSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  eslt_Byte hash[ESL_SIZEOF_SHA256_DIGEST];

  actMemClear(hash, ESL_SIZEOF_SHA256_DIGEST); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_VER, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    (void)esl_finalizeHashVerifyRSASHA256_PSS(workSpace, hash);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
    returnValue = esl_verifySigVerifyRSASHA256_PSS(workSpace, hash, saltSize, signatureSize, signature);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA256_PSS_AutoSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA256_PSS_AutoSalt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
  eslt_Length signatureSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeVerifyRSASHA256_PSS(workSpace, (eslt_Length)-1, signatureSize, signature); /* PRQA S 2895 */ /* MD_MSR_Dir1.1 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA256_PSS_NoSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA256_PSS_NoSalt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
  eslt_Length signatureSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeVerifyRSASHA256_PSS(workSpace, 0u, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA256_PSS_DigestLengthSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA256_PSS_DigestLengthSalt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
  eslt_Length signatureSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeVerifyRSASHA256_PSS(workSpace, ESL_SIZEOF_SHA256_DIGEST, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

# endif /* (VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/**********************************************************************************************************************
 *  esl_initVerifyRSARIPEMD160_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initVerifyRSARIPEMD160_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
eslt_Length keyPairModuleSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
eslt_Length publicKeyExponentSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) publicKeyExponent)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((returnValue = esl_CheckWorkSpaceHeader(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_VER)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* returnValue already set */
  }
  else
  {
    workSpace->wsHashUnion.RIPEMD160ws.header.size = ESL_MAXSIZEOF_WS_RIPEMD160;
    workSpace->wsHashUnion.RIPEMD160ws.header.watchdog = workSpace->header.watchdog;
    returnValue = esl_initVerifyRSA_PSS(workSpace, keyPairModuleSize, keyPairModule, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                        publicKeyExponentSize, publicKeyExponent, ESL_HA_RMD160);
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  esl_updateVerifyRSARIPEMD160_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateVerifyRSARIPEMD160_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
eslt_Length inputSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) input)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_VER, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    returnValue = esl_updateVerifyRSA_PSS(workSpace, inputSize, input, ESL_HA_RMD160); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    if (returnValue == ESL_ERC_TOTAL_LENGTH_OVERFLOW)
    {
      returnValue = ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW;
    }
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeHashVerifyRSARIPEMD160_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashVerifyRSARIPEMD160_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest)
{
  eslt_ErrorCode returnValue = ESL_ERC_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  if ((workSpace == NULL_PTR) || (messageDigest == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_VER, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    esl_finalizeHashVerifyRSA_PSS(workSpace, messageDigest, ESL_HA_RMD160); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_verifySigVerifyRSARIPEMD160_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSARIPEMD160_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
eslt_Length saltSize,
eslt_Length signatureSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  if ((workSpace == NULL_PTR) || (messageDigest == NULL_PTR) || (signature == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_VER, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    returnValue = esl_verifySigVerifyRSA_PSS(workSpace, messageDigest, ESL_SIZEOF_RIPEMD160_DIGEST, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                             saltSize, signatureSize, signature, ESL_HA_RMD160);
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_verifySigVerifyRSARIPEMD160_PSS_AutoSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSARIPEMD160_PSS_AutoSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
eslt_Length signatureSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_verifySigVerifyRSARIPEMD160_PSS(workSpace, messageDigest, (eslt_Length)-1, signatureSize, signature); /* PRQA S 2895 */ /* MD_MSR_Dir1.1 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_verifySigVerifyRSARIPEMD160_PSS_NoSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSARIPEMD160_PSS_NoSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
eslt_Length signatureSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_verifySigVerifyRSARIPEMD160_PSS(workSpace, messageDigest, 0u, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_verifySigVerifyRSARIPEMD160_PSS_DigestLengthSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSARIPEMD160_PSS_DigestLengthSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
eslt_Length signatureSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_verifySigVerifyRSARIPEMD160_PSS(workSpace, messageDigest, ESL_SIZEOF_RIPEMD160_DIGEST, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                                    signatureSize, signature);
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeVerifyRSARIPEMD160_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSARIPEMD160_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
eslt_Length saltSize,
eslt_Length signatureSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  eslt_Byte hash[ESL_SIZEOF_RIPEMD160_DIGEST];

  actMemClear(hash, ESL_SIZEOF_RIPEMD160_DIGEST); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_VER, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    (void)esl_finalizeHashVerifyRSARIPEMD160_PSS(workSpace, hash);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
    returnValue = esl_verifySigVerifyRSARIPEMD160_PSS(workSpace, hash, saltSize, signatureSize, signature);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeVerifyRSARIPEMD160_PSS_AutoSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSARIPEMD160_PSS_AutoSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
eslt_Length signatureSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeVerifyRSARIPEMD160_PSS(workSpace, (eslt_Length)-1, signatureSize, signature); /* PRQA S 2895 */ /* MD_MSR_Dir1.1 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeVerifyRSARIPEMD160_PSS_NoSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSARIPEMD160_PSS_NoSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
eslt_Length signatureSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeVerifyRSARIPEMD160_PSS(workSpace, 0u, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeVerifyRSARIPEMD160_PSS_DigestLengthSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSARIPEMD160_PSS_DigestLengthSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSver) workSpace,
eslt_Length signatureSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeVerifyRSARIPEMD160_PSS(workSpace, ESL_SIZEOF_RIPEMD160_DIGEST, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}
# endif /* (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON) */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ((VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON) ) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSA_PSS_Ver.c
 *********************************************************************************************************************/
