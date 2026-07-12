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
/*!        \file  ESLib_RSA_Common.c
 *        \brief  Crypto library - PKCS #1 RSA PSS signature verification / generation (SHA-256)
 *
 *      \details Helper functions for RSA signature generation / generation using RSA PSS encoding scheme
 *               according to PKCS #1 v2.2 (see also RFC8017 and the numeration therein)
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

#define ESLIB_RSA_PSS_COMMON_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ESLib.h"
#include "ESLib_RSA_Common.h"

#if ((VSECPRIM_RSA_OAEP_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_ENABLED == STD_ON)) /* COV_VSECPRIM_NO_SAFE_CONFIG TF xf tf */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

# if ((VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) || (VSECPRIM_RSA_OAEP_SHA1_ENABLED == STD_ON)) /* COV_VSECPRIM_NO_SAFE_CONFIG XF xf xf */
#  define VSECPRIM_WRAPPER_SHA1
# endif
# if ((VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON) || (VSECPRIM_RSA_OAEP_SHA2_256_ENABLED == STD_ON)) /* COV_VSECPRIM_NO_SAFE_CONFIG TX tx xf */
#  define VSECPRIM_WRAPPER_SHA256
# endif
# if (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  define VSECPRIM_WRAPPER_RIPEMD160
# endif

typedef eslt_WorkSpaceUnionHashFunctions_RSA eslt_WorkSpaceUnionHashFunctions;
# include "actHashFctWrappers_Implementation.h" /* PRQA S 5087 */ /* MD_VSECPRIM_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  esl_calcSaltedHashRSA_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_calcSaltedHashRSA_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceUnionHashFunctions_RSA) wsuHash,
  eslt_Length saltSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
  VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
  eslt_Length hashDigestSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte) saltedHash,
  eslt_HashAlgorithm hashID)
{
  eslt_ErrorCode returnValue = ESL_ERC_NO_ERROR;
  actRETURNCODE actReturnValue;
  eslt_Byte padding[ESL_SIZEOF_RSA_PSS_PADDING1];

  /* Initialize hash calculation */
  actReturnValue = actHashInit(wsuHash, hashID); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  if (actReturnValue == actOK)
  {
    /* 9.1.2, Steps 5-6: Hash leftmost zero octets */
    actMemClear(padding, sizeof(padding)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */
    actReturnValue = actHashUpdate(wsuHash, padding, sizeof(padding), hashID); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */

    /* 9.1.1, Steps 5-6: Hash given message digest */
    if (actReturnValue == actOK)
    {
      actReturnValue = actHashUpdate(wsuHash, messageDigest, hashDigestSize, hashID); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }

    if (actReturnValue == actOK)
    {
      /* 9.1.1, Steps 5-6: Hash salt when given */
      if (saltSize > 0u)
      {
        actReturnValue = actHashUpdate(wsuHash, salt, saltSize, hashID); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }
    }

    if (actReturnValue == actOK)
    {
      /* 9.1.2, Steps 6: Finalize hash calculation and return in provided buffer */
      (void)actHashFinalize(wsuHash, saltedHash, hashID); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
    else /* actReturnValue == actEXCPETION_LENGTH */
    {
      returnValue = ESL_ERC_TOTAL_LENGTH_OVERFLOW;
    }
  }
  else /* actReturnValue = actEXCEPTION_ALGOID */
  {
    returnValue = ESL_ERC_INPUT_INVALID;
  }

  return returnValue;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  esl_generateMaskMGF1()
 *********************************************************************************************************************/
/*!
 *Internal comment removed.
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_generateMaskMGF1(
  eslt_HashAlgorithm hashAlgorithm,
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceUnionHashFunctions_RSA) wsHash,
  VSECPRIM_P2VAR_PARA(eslt_Byte) tempHash,
  eslt_Length hashDigestSize,
  eslt_Length seedLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) seed,
  eslt_Length maskLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) mask)
{
  eslt_ErrorCode returnValue;
  actRETURNCODE actReturnValue = actEXCEPTION_UNKNOWN; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  eslt_Size32 counter;
  eslt_Byte octetCounter[4u];
  eslt_Length digestNumber;
  eslt_Length remainingLength;
  eslt_Length offset;
  eslt_Length passLength;
  eslt_Length i;

  if (hashDigestSize == 0u)
  {
    returnValue = ESL_ERC_ERROR;
  }
  else
  {
    /* B.2.1, Step 3: Initialize counter value */
    counter = 0u;
    offset = 0u;
    digestNumber = (maskLength + hashDigestSize - 1u) / hashDigestSize;
    remainingLength = maskLength;

    returnValue = ESL_ERC_NO_ERROR;

    /* B.2.1, Step 3: Generate requested length */
    for (i = 0; i < digestNumber; i++)
    {
      /* Initialize hash calculation */
      actReturnValue = actHashInit(wsHash, hashAlgorithm); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      if (actReturnValue == actOK)
      {
        /* B.2.1, Step 3b: Hash given seed value */
        actReturnValue = actHashUpdate(wsHash, seed, seedLength, hashAlgorithm); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }

      if (actReturnValue == actOK)
      {
        /* B.2.1, Step 3a: Convert counter value to big-endian octet string */
        actU32toBE(octetCounter, counter); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */

        /* B.2.1, Step 3b: Hash octet string of counter */
        actReturnValue = actHashUpdate(wsHash, octetCounter, sizeof(octetCounter), hashAlgorithm); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }

      if (actReturnValue == actOK)
      {
        /* Finalize hash calculation */
        actReturnValue = actHashFinalize(wsHash, tempHash, hashAlgorithm); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }

      if (actReturnValue == actOK)
      {
        /* B.2.1, Step 4: Limit length in this pass, when end of buffer is reached */
        passLength = hashDigestSize;
        if (passLength > remainingLength)
        {
          passLength = remainingLength;
        }

        /* B.2.1, Steps 3-4: Concatenate (truncated) hash to output */
        actMemCpyByteArray(&mask[offset], tempHash, passLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS */

        /* B.2.1, Step 3: Update counter */
        counter++;
        offset += passLength;
        remainingLength -= passLength;
      }

      /* Abort loop in case of error */
      if (actReturnValue != actOK)
      {
        returnValue = ESL_ERC_ERROR;
        break;
      }
    }
  }

  return returnValue;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ((VSECPRIM_RSA_OAEP_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_ENABLED == STD_ON)) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSA_PSS.c
 *********************************************************************************************************************/
