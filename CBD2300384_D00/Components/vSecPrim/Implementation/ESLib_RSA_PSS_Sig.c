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
/*!        \file  ESLib_RSA_PSS_Sig.c
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
#define ESLIB_RSA_PSS_SIG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ESLib.h"
#include "ESLib_Helper.h"
#include "ESLib_RSA_Common.h"

#include "actIRSA.h"
#include "actBigNum.h"

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
 *  GLOBAL DATA
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM(VSECPRIM_NONE, eslt_Byte) esl_RSAPSSEncodingMask[8] =
{
  0xFFu, 0x01u, 0x03u, 0x07u, 0x0Fu, 0x1Fu, 0x3Fu, 0x7Fu
};

# define VSECPRIM_STOP_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
  esl_checkSaltSizeSig()
 *********************************************************************************************************************/
/*! \brief         checks if the provided memory size is sufficient to store the salt
 *  \details       Checks if exceeds remaining memory space (dbLength - 1) in subarray db of 
 *                 workSpace->encodedMessage, where the salt will be stored.
 *  \param[in]     saltSize        size of salt
 *  \param[in]     dbLength        Size of key pair module
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Value refered to by saltSize is too big
 *  \context       TASK
 *  \reentrant     TRUE
 *  \pre           saltSize must be a valid pointer
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_checkSaltSizeSig(
  eslt_Length saltSize,
  eslt_Length dbLength);

/**********************************************************************************************************************
  esl_checkSignatureSize()
 *********************************************************************************************************************/
/*! \brief        Preliminary signature size check.
 *  \details       Checks if signatureSize is too short and adapts signatureSize, if it is too long.
 *  \param[in,out] signatureSize  (in) (pointer to) original signatureSize
 *                                (out) (pointer to) modified signatureSize
 *  \param[in]     emLength       length of encoded message in byte
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_OUTPUT_SIZE_TOO_SHORT value refered to by signatureSize is too small
 *  \context       TASK
 *  \reentrant     TRUE
 *  \pre           signatureSize needs to be a valid pointer.
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_checkSignatureSize(
  VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
  eslt_Length emLength);

/**********************************************************************************************************************
  esl_FillDataBlock()
 *********************************************************************************************************************/
/*! \brief        Fills data block db with a padding string and a salt array.
 *  \details       Required during EMSA-PSS encoding.
 *                 If no salt is provided by the user, i.e., if salt is a null pointer, random salt values are
 *                 generated instead,  using the RNG which is implemented by the call back function
 *                 esl_getBytesRNG(..).
 *  \param[in,out] db        (in) (pointer to) data block db in workSpace->encodedMessage
 *                           (out) (pointer to) data block db, filled with a zero-padding string and the salt values
 *  \param[in,out] dbSalt    (in) (pointer to) part of db in which the salt array is stored
 *                           (out) (pointer to) part of db in which the salt array is stored, filled with salt values
 *  \param[in]     saltSize  length of the salt array
 *  \param[in]     salt      pointer to salt defined by user or nullpointer
 *  \param[in]     psLength  length of the padding string
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 esl_getBytesRNG(..) return values are passed through
 *  \context       TASK
 *  \reentrant     TRUE
 *  \pre           db, dbSalt need to be a valid pointers.
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_FillDataBlock(
  VSECPRIM_P2VAR_PARA(eslt_Byte) db,
  VSECPRIM_P2VAR_PARA(eslt_Byte) dbSalt,
  eslt_Length saltSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
  eslt_Length psLength);

/**********************************************************************************************************************
 *  esl_initSignRSA_PSS()
 *********************************************************************************************************************/
/*! \brief         Initialize signature generation
 *  \details       This function initializes the signature generation based on RSA PSS, independent from the
 *                 underlying hash function: the hashing procedure is initialized and the workspace for the RSA
 *                 signature primitive is set up.
 *  \param[in,out] workSpace               (in)  Signature generation work space
 *                                         (out) Signature generation work space, contained workSpaces initialized
 *  \param[in]     keyPairModuleSize       Size of RSA key pair module
 *  \param[in]     keyPairModule           RSA key pair module
 *  \param[in]     privateKeyExponentSize  Size of public RSA key pair exponent
 *  \param[in]     privateKeyExponent      Public RSA key pair exponent
 *  \param[in]     hashID                  parameter to identify underlying hash function
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter NULL
 *                 ESL_ERC_WS_TOO_SMALL  Work space too small
 *                 ESL_ERC_RSA_PRIVKEY_INVALID  The private exponent is invalid
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           workSpace must be a valid pointer
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initSignRSA_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   eslt_Length keyPairModuleSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
   eslt_Length privateKeyExponentSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent,
   eslt_HashAlgorithm hashID);

/**********************************************************************************************************************
  esl_updateSignRSA_PSS()
 *********************************************************************************************************************/
/*! \brief         Updates underlying hash function of the RSA PSS signature scheme with original message
 *  \details       This function updates the signature generation based on RSA PSS, independent from the underlying
 *                 hash function. The message to be hashed is passed is passed to the hashing update routine.
 *  \param[in,out] workSpace       (in)  Signature generation work space
 *                                 (out) Hash workspace contained in workSpace updated with input to be hashed
 *  \param[in]     inputSize       Size of data to be hashed
 *  \param[in]     input           Data to be hashed
 *  \param[in]     hashID          parameter to identify underlying hash function
 *  \return        ESL_ERC_NO_ERROR               Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter NULL
 *                 ESL_ERC_TOTAL_LENGTH_OVERFLOW  Maximum total input length for hashing exceeded
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           workSpace needs to be a valid pointer, workSpace initialized
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_updateSignRSA_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   eslt_Length inputSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input,
   eslt_HashAlgorithm hashID);

/**********************************************************************************************************************
  esl_finalizeHashSignRSA_PSS()
 *********************************************************************************************************************/
/*! \brief         Computes of the hash value of the original message
 *  \details       This function finalizes the computation of the hash value of the original message as a part of the
 *                 RSA PSS signature generation procedure, independent from the underlying hash function.
 *  \param[in,out] workSpace      (in)  Signature generation work space
 *                                (out) Signature generation work space
 *  \param[in,out] messageDigest  (in)  buffer to store the hash value
 *                                (out) hash value of the original message
 *  \param[in]     hashID               parameter to identify underlying hash function
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           workSpace is initialized and a valid pointer, messageDigest is a valid pointer
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) esl_finalizeHashSignRSA_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest,
   eslt_HashAlgorithm hashID);

/**********************************************************************************************************************
  esl_finalizeSigSignRSA_PSS()
 *********************************************************************************************************************/
/*! \brief         Encoding step of the signature generation scheme based on RSA PSS
 *  \details       This function performs the PSS encoding step and applies the RSA signing primitive to the
 *                 encoded message, independent from the underlying hash function.
 *  \param[in,out] workSpace       (in)  Signature generation work space
 *                                 (out) Signature generation work space
 *  \param[in]     messageDigest   hash value of the original message
 *  \param[in]     hashDigestSize  length of the hash value
 *  \param[in]     saltSize        intended size of salt,
 *                                 -1 if random salt values shall be generated automatically
 *  \param[in]     salt            buffer storing the salt, possibly a null pointer
 *  \param[in,out] signatureSize   (in)  pointer to buffer, storing the size of the signature
 *                                 (out) size of the signature, equals the modulus size
 *  \param[in,out] signature       (in)  buffer to store the signature to be generated
 *                                 (out) generated signature
 *  \param[in]     hashID           parameter to identify underlying hash function
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID input parameter null
 *                 ESL_ERC_ERROR error during mask generation step
 *                 all possible error codes which can occur in esl_getBytesRNG
 *                 ESL_ERC_TOTAL_LENGTH_OVERFLOW total length of data to be hashed (while calculating the salted hash
 *                                               value) too long
 *                 ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE  message > modulus
 *                 ESL_ERC_OUTPUT_SIZE_TOO_SHORT  *signatureSize too small
 *                 ESL_ERC_RSA_ENCODING_INVALID       RSA encoded message length too short
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           workSpace is initialized and a valid pointer, messageDigest and signature are valid pointers
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSA_PSS(
   VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
   VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
   eslt_Length hashDigestSize,
   eslt_Length saltSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
   VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
   VSECPRIM_P2VAR_PARA(eslt_Byte) signature,
   eslt_HashAlgorithm hashID);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  esl_checkSaltSizeSig()
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
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_checkSaltSizeSig(
  eslt_Length saltSize,
  eslt_Length dbLength)
{
  eslt_ErrorCode returnValue = ESL_ERC_NO_ERROR;

  /* 9.1.1, Step 3: Salt and 0x01 have to fit into DB */
  if ((saltSize + 1u) > dbLength)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  esl_checkSignatureSize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_checkSignatureSize(
  VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
  eslt_Length emLength)
{
  eslt_ErrorCode returnValue = ESL_ERC_NO_ERROR;

  if (*signatureSize < emLength)
  {
    returnValue = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
  }
  else
  {
    *signatureSize = emLength; /* SBSW_VSECPRIM_WRITE_POINTER */
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  esl_FillDataBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_FillDataBlock(
  VSECPRIM_P2VAR_PARA(eslt_Byte) db,
  VSECPRIM_P2VAR_PARA(eslt_Byte) dbSalt,
  eslt_Length saltSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
  eslt_Length psLength)
{
  eslt_ErrorCode returnValue = ESL_ERC_NO_ERROR;

  /* 9.1.1, Step 8: Prepare constant parts of DB */
  actMemClear(db, psLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  db[psLength] = 0x01u; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */

  /* 9.1.1, Steps 4, 8: Add salt to DB */
  if (saltSize > 0u)
  {
    /* Any salt provided by caller? */
    if (salt == NULL_PTR)
    {
      /* No salt provided, generate random value */
      returnValue = esl_getBytesRNG(saltSize, dbSalt); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    }
    else
    {
      /* Use provided salt instead */
      actMemCpyByteArray(dbSalt, salt, saltSize); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    }
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  esl_initSignRSA_PSS()
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
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initSignRSA_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
  eslt_Length keyPairModuleSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
  eslt_Length privateKeyExponentSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent,
  eslt_HashAlgorithm hashID)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if ((keyPairModule == NULL_PTR) || (privateKeyExponent == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {

    /* initialize hash workspace and function; actHashInit always returns actOK */
    (void)actHashInit(&(workSpace->wsHashUnion), hashID); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* initialize RSA signature primitive workspace and function */
    workSpace->wsRSASigPrim.header.size = (eslt_Length)(ESL_MAXSIZEOF_WS_RSA_PRIM); /* SBSW_VSECPRIM_WRITE_POINTER */
    workSpace->wsRSASigPrim.header.watchdog = workSpace->header.watchdog; /* SBSW_VSECPRIM_WRITE_POINTER */
    returnValue = esl_initSignRSA_prim(&(workSpace->wsRSASigPrim),
                                       keyPairModuleSize, keyPairModule,
                                       privateKeyExponentSize, privateKeyExponent); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

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
 *  esl_updateSignRSA_PSS()
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
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_updateSignRSA_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
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
    actReturnValue = actHashUpdate(&(workSpace->wsHashUnion), input, inputSize, hashID); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actReturnValue != actOK)
    {
      returnValue = ESL_ERC_TOTAL_LENGTH_OVERFLOW;
    }
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeHashSignRSA_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) esl_finalizeHashSignRSA_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest,
  eslt_HashAlgorithm hashID)
{
  /* 9.1.1, Step 2 */
  (void)actHashFinalize(&(workSpace->wsHashUnion), messageDigest, hashID); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  esl_finalizeSigSignRSA_PSS()
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
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSA_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
  eslt_Length hashDigestSize,
  eslt_Length saltSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
  VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte) signature,
  eslt_HashAlgorithm hashID)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  eslt_Length saltLength;
  eslt_Size32 emBits;
  eslt_Length emLength, psLength, dbLength = 0u; /* PRQA S 2981 */ /* MD_VSECPRIM_REDUNDANT_OP */

  /* Define arrays to provide easier access to different subarrays in the encodedMessage and dataBlock:
  saltedHash: beginning of the subarray "saltedHash" in encodedMessage, will be compared to the hash
  value that is computed in this function to determine whether the signature is valid or not
  db: points to dataBlock
  dbSalt: beginning of the subarray "salt" in dataBlock */

  VSECPRIM_P2VAR_PARA(eslt_Byte) saltedHash;
  VSECPRIM_P2VAR_PARA(eslt_Byte) db;
  VSECPRIM_P2VAR_PARA(eslt_Byte) dbSalt;

  saltLength = ((saltSize == (eslt_Length)-1) ? hashDigestSize : saltSize); /* PRQA S 2895 */ /* MD_MSR_Dir1.1 */
  db = &(workSpace->encodedMessageBuffer.dataBlock[0]);

  /* see RFC, emBits = maximal bit length of the integer OS2IP(EM)
     (actRSAPrivateKeyGetBitLength returns length of module in bit),
     emLength = length of the octet string representation of the encoded message */
  emBits = actRSAPrivateKeyGetBitLength(&(workSpace->wsRSASigPrim.wsRSAPrimBuf.wsRSARing)) - 1u;  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  emLength = actMin((eslt_Length)((emBits + 7u) / 8u), Esl_BitsToBytes(ESL_RSA_KEYSIZE));
  returnValue = esl_checkSignatureSize(signatureSize, emLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  /* 8.1.1, Step 1: EMSA-PSS encoding */
  if (returnValue == ESL_ERC_NO_ERROR)
  {
    /* Note: RFC8017 specifies to ensure emLength >= hashDigestSize + saltLength + 2.
     * However, this is not possible here, since this would eliminate the AutoSalt option (where saltSize = -1).
     * We ensure the following: (1) emLength >= hashDigestSize + 1u
     *                          (2) dbLength >= saltLength + 1
     * The function computes                        dbLength = emLength - hashDigestSize - 1, which is a well defined subtraction due to (1).
     * This leads to                                           emLength - hashDigestSize - 1 >= saltLength + 1, using (2),
     * and thus the checks here are equivalent to ensuring     emLength >= hashDigestSize + saltLength + 2.
     */
    if (emLength < (hashDigestSize + 1u))
    {
      returnValue = ESL_ERC_RSA_ENCODING_INVALID;
    }
    else
    {
      /* Length of maskedDB and DB (both buffers have the same length) */
      dbLength = emLength - hashDigestSize - 1u;
      saltedHash = &(workSpace->encodedMessageBuffer.encodedMessage[dbLength]);

      /* 9.1, Note 4: Typical salt length is length of the output of the hash function */
      returnValue = esl_checkSaltSizeSig(saltLength, dbLength);

      if (returnValue == ESL_ERC_NO_ERROR)
      {
        /* Calculate length of the zero padding string in DB
           Officially, the last entry of the padding string is 0x01. This entry is ignored with respect to
           the length of the padding string, that is why -1u is subtracted. */
        psLength = dbLength - saltLength - 1u;

        /* Points to salt in DB;
           May be behind actual DB, when saltSize == 0. In this case, nothing is written in dbSalt.  */
        dbSalt = &db[psLength + 1u];
        returnValue = esl_FillDataBlock(db, dbSalt, saltLength, salt, psLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

        if (returnValue == ESL_ERC_NO_ERROR)
        {
          /* 9.1.1, Steps 5-6: Calculate salted hash H from given message digest */
          returnValue = esl_calcSaltedHashRSA_PSS(&(workSpace->wsHashUnion), saltLength, dbSalt,
            messageDigest, hashDigestSize, saltedHash, hashID); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        }
      }

      if (returnValue == ESL_ERC_NO_ERROR)
      {
        /* 9.1.1, Step 9: Generate dbMask using mask generation function (MGF) */
        returnValue = esl_generateMaskMGF1(hashID, &(workSpace->wsHashUnion),
          workSpace->encodedMessageBuffer.tempVariables, hashDigestSize,
          hashDigestSize, saltedHash, dbLength,
          workSpace->encodedMessageBuffer.encodedMessage); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }
    }
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    /* 9.1.1, Steps 10, 12: Apply mask to DB using XOR to create maskedDB */
    actXOR(workSpace->encodedMessageBuffer.encodedMessage, db, dbLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

    /* 9.1.1, Step 11: Set leftmost 8emLen - emBits bits of the leftmost octet in maskedDB to zero */
    (workSpace->encodedMessageBuffer.encodedMessage)[0u] &= esl_RSAPSSEncodingMask[emBits & 0x07u]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
    /* 9.1.1, Step 12: Set constant "trailer field" in EM (also see 9.1, Note 1)
        This is the last step of the encoding procedure. */
    (workSpace->encodedMessageBuffer.encodedMessage)[emLength - 1u] = ESL_RSA_PSS_TRAILER; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */

    /* apply RSA signature primitive to the encoded message */
    returnValue = esl_signRSA_prim(&(workSpace->wsRSASigPrim), workSpace->modulusSizeInformation,
      workSpace->encodedMessageBuffer.encodedMessage, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  return returnValue;
} /* PRQA S 6060, 6050 */ /* MD_VSECPRIM_STPAR, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# if (VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/**********************************************************************************************************************
 *  esl_initSignRSASHA1_PSS
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSASHA1_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
  eslt_Length keyPairModuleSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
  eslt_Length privateKeyExponentSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((returnValue = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_RSA_PSS_SIG)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* returnValue already set */
  }
  else
  {
    workSpace->wsHashUnion.SHA1ws.header.size = ESL_MAXSIZEOF_WS_SHA1;
    workSpace->wsHashUnion.SHA1ws.header.watchdog = workSpace->header.watchdog;
    returnValue = esl_initSignRSA_PSS(workSpace, keyPairModuleSize, keyPairModule, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                      privateKeyExponentSize, privateKeyExponent, ESL_HA_SHA1);
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_updateSignRSASHA1_PSS
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSignRSASHA1_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
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
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_SIG, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    returnValue = esl_updateSignRSA_PSS(workSpace, inputSize, input, ESL_HA_SHA1); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    if (returnValue == ESL_ERC_TOTAL_LENGTH_OVERFLOW)
    {
      returnValue = ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW;
    }
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeHashSignRSASHA1_PSS
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashSignRSASHA1_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest)
{
  eslt_ErrorCode returnValue = ESL_ERC_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if ((workSpace == NULL_PTR) || (messageDigest == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_SIG, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    esl_finalizeHashSignRSA_PSS(workSpace, messageDigest, ESL_HA_SHA1); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  return returnValue;
}

/***********************************************************************************************************************
 *  esl_finalizeSigSignRSASHA1_PSS
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSASHA1_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
eslt_Length saltSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  if ((workSpace == NULL_PTR) || (signatureSize == NULL_PTR) || (signature == NULL_PTR) || (messageDigest == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_SIG, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    returnValue = esl_finalizeSigSignRSA_PSS(workSpace, messageDigest, ESL_SIZEOF_SHA1_DIGEST, saltSize, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                             salt, signatureSize, signature, ESL_HA_SHA1);
  }
  return returnValue;
} /* PRQA S 6060 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

/**********************************************************************************************************************
 *  esl_finalizeSigSignRSASHA1_PSS_NoSalt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSASHA1_PSS_NoSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeSigSignRSASHA1_PSS(workSpace, messageDigest, 0u, NULL_PTR, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeSigSignRSASHA1_PSS_DigestLengthSalt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSASHA1_PSS_DigestLengthSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeSigSignRSASHA1_PSS(workSpace, messageDigest, ESL_SIZEOF_SHA1_DIGEST, NULL_PTR, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                               signatureSize, signature);
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeSignRSASHA1_PSS
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSASHA1_PSS(VSECPRIM_P2VAR_PARA(
  eslt_WorkSpaceRSAPSSsig) workSpace,
  eslt_Length saltSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
  VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  eslt_Byte hash[ESL_SIZEOF_SHA1_DIGEST];

  actMemClear(hash, ESL_SIZEOF_SHA1_DIGEST); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */

  if ((workSpace == NULL_PTR) || (signatureSize == NULL_PTR) || (signature == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_SIG, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    (void)esl_finalizeHashSignRSASHA1_PSS(workSpace, hash); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
    returnValue = esl_finalizeSigSignRSASHA1_PSS(workSpace, hash, saltSize, salt, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeSignRSASHA1_PSS_NoSalt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSASHA1_PSS_NoSalt(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
                                                                 VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
                                                                 VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeSignRSASHA1_PSS(workSpace, 0u, NULL_PTR, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeSignRSASHA1_PSS_DigestLengthSalt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSASHA1_PSS_DigestLengthSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeSignRSASHA1_PSS(workSpace, ESL_SIZEOF_SHA1_DIGEST, NULL_PTR, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}
# endif /* (VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
/**********************************************************************************************************************
 *  esl_initSignRSASHA256_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSASHA256_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
  eslt_Length keyPairModuleSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
  eslt_Length privateKeyExponentSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((returnValue = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_RSA_PSS_SIG)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* returnValue already set */
  }
  else
  {
    workSpace->wsHashUnion.SHA256ws.header.size = ESL_MAXSIZEOF_WS_SHA256; /* SBSW_VSECPRIM_WRITE_POINTER */
    workSpace->wsHashUnion.SHA256ws.header.watchdog = workSpace->header.watchdog; /* SBSW_VSECPRIM_WRITE_POINTER */
    returnValue = esl_initSignRSA_PSS(workSpace, keyPairModuleSize, keyPairModule, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                      privateKeyExponentSize, privateKeyExponent, ESL_HA_SHA2_256);
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_updateSignRSASHA256_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSignRSASHA256_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
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
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_SIG, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    returnValue = esl_updateSignRSA_PSS(workSpace, inputSize, input, ESL_HA_SHA2_256); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    if (returnValue == ESL_ERC_TOTAL_LENGTH_OVERFLOW)
    {
      returnValue = ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW;
    }
  }
  return returnValue;
}
/**********************************************************************************************************************
 *  esl_finalizeHashSignRSASHA256_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashSignRSASHA256_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest)
{
  eslt_ErrorCode returnValue = ESL_ERC_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  if ((workSpace == NULL_PTR) || (messageDigest == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_SIG, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    esl_finalizeHashSignRSA_PSS(workSpace, messageDigest, ESL_HA_SHA2_256); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_RSAPSSSHA256_FINALIZE_HASH */
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeSigSignRSASHA256_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSASHA256_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
  eslt_Length saltSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
  VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  if ((workSpace == NULL_PTR) || (signatureSize == NULL_PTR) || (signature == NULL_PTR) || (messageDigest == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_SIG, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    returnValue = esl_finalizeSigSignRSA_PSS(workSpace, messageDigest, ESL_SIZEOF_SHA256_DIGEST, saltSize, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                             salt, signatureSize, signature, ESL_HA_SHA2_256);
  }
  return returnValue;
} /* PRQA S 6060, 6050 */ /* MD_VSECPRIM_STPAR, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  esl_finalizeSigSignRSASHA256_PSS_NoSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSASHA256_PSS_NoSalt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
  VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeSigSignRSASHA256_PSS(workSpace, messageDigest, 0u, NULL_PTR, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeSigSignRSASHA256_PSS_DigestLengthSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSASHA256_PSS_DigestLengthSalt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
  VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeSigSignRSASHA256_PSS(workSpace, messageDigest, ESL_SIZEOF_SHA256_DIGEST, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                                 NULL_PTR, signatureSize, signature);
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeSignRSASHA256_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSASHA256_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
  eslt_Length saltSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
  VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  eslt_Byte hash[ESL_SIZEOF_SHA256_DIGEST];

  actMemClear(hash, ESL_SIZEOF_SHA256_DIGEST); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */

  if ((workSpace == NULL_PTR) || (signatureSize == NULL_PTR) || (signature == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_SIG, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    (void)esl_finalizeHashSignRSASHA256_PSS(workSpace, hash); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
     returnValue = esl_finalizeSigSignRSASHA256_PSS(workSpace, hash, saltSize, salt, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeSignRSASHA256_PSS_NoSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSASHA256_PSS_NoSalt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeSignRSASHA256_PSS(workSpace, 0u, NULL_PTR, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeSignRSASHA256_PSS_DigestLengthSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSASHA256_PSS_DigestLengthSalt(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeSignRSASHA256_PSS(workSpace, ESL_SIZEOF_SHA256_DIGEST, NULL_PTR, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}
# endif /* (VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/**********************************************************************************************************************
 *  esl_initSignRSARIPEMD160_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSARIPEMD160_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
eslt_Length keyPairModuleSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairModule,
eslt_Length privateKeyExponentSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponent)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((returnValue = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_RSA_PSS_SIG)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* returnValue already set */
  }
  else
  {
    workSpace->wsHashUnion.RIPEMD160ws.header.size = ESL_MAXSIZEOF_WS_RIPEMD160;
    workSpace->wsHashUnion.RIPEMD160ws.header.watchdog = workSpace->header.watchdog;
    returnValue = esl_initSignRSA_PSS(workSpace, keyPairModuleSize, keyPairModule, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                      privateKeyExponentSize, privateKeyExponent, ESL_HA_RMD160);
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  esl_updateSignRSARIPEMD160_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSignRSARIPEMD160_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
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
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_SIG, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    returnValue = esl_updateSignRSA_PSS(workSpace, inputSize, input, ESL_HA_RMD160); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    if (returnValue == ESL_ERC_TOTAL_LENGTH_OVERFLOW)
    {
      returnValue = ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW;
    }
  }
  return returnValue;
}

/***********************************************************************************************************************
 *  esl_finalizeHashSignRSARIPEMD160_PSS()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashSignRSARIPEMD160_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest)
{
  eslt_ErrorCode returnValue = ESL_ERC_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  if ((workSpace == NULL_PTR) || (messageDigest == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_SIG, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    esl_finalizeHashSignRSA_PSS(workSpace, messageDigest, ESL_HA_RMD160); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeSigSignRSARIPEMD160_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSARIPEMD160_PSS(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
                                                                  VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
                                                                  eslt_Length saltSize,
                                                                  VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
                                                                  VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
                                                                  VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  if ((workSpace == NULL_PTR) || (signatureSize == NULL_PTR) || (signature == NULL_PTR) || (messageDigest == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_SIG, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    returnValue = esl_finalizeSigSignRSA_PSS(workSpace, messageDigest, ESL_SIZEOF_RIPEMD160_DIGEST, saltSize, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      salt, signatureSize, signature, ESL_HA_RMD160);
  }
  return returnValue;
} /* PRQA S 6060 */ /* MD_VSECPRIM_QM_ONLY_METRIC */


/**********************************************************************************************************************
 *  esl_finalizeSigSignRSARIPEMD160_PSS_NoSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSARIPEMD160_PSS_NoSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeSigSignRSARIPEMD160_PSS(workSpace, messageDigest, 0u, NULL_PTR, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeSigSignRSARIPEMD160_PSS_DigestLengthSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSigSignRSARIPEMD160_PSS_DigestLengthSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeSigSignRSARIPEMD160_PSS(workSpace, messageDigest, ESL_SIZEOF_RIPEMD160_DIGEST, NULL_PTR, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                              signatureSize, signature);
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeSignRSARIPEMD160_PSS()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSARIPEMD160_PSS(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
                                                               eslt_Length saltSize,
                                                               VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
                                                               VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
                                                               VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  eslt_Byte hash[ESL_SIZEOF_RIPEMD160_DIGEST];

  actMemClear(hash, ESL_SIZEOF_RIPEMD160_DIGEST); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */

  if ((workSpace == NULL_PTR) || (signatureSize == NULL_PTR) || (signature == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&(workSpace->header), ESL_MAXSIZEOF_WS_RSA_PSS_SIG, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    (void)esl_finalizeHashSignRSARIPEMD160_PSS(workSpace, hash); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
    returnValue = esl_finalizeSigSignRSARIPEMD160_PSS(workSpace, hash, saltSize, salt, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
  }

  return returnValue;
}


/**********************************************************************************************************************
 *  esl_finalizeSignRSARIPEMD160_PSS_NoSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSARIPEMD160_PSS_NoSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeSignRSARIPEMD160_PSS(workSpace, 0u, NULL_PTR, signatureSize, signature); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeSignRSARIPEMD160_PSS_DigestLengthSalt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSARIPEMD160_PSS_DigestLengthSalt(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAPSSsig) workSpace,
VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize,
VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  returnValue = esl_finalizeSignRSARIPEMD160_PSS(workSpace, ESL_SIZEOF_RIPEMD160_DIGEST, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                                 NULL_PTR, signatureSize, signature);
  return returnValue;
}
# endif /* (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON)*/

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ((VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON) ) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSA_PSS_Sig.c
 *********************************************************************************************************************/
