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
/*!        \file  ESLib_RSA_PSS.c
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
#define ESLIB_RSA_PSS_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ESLib.h"
#include "ESLib_Helper.h"
#include "ESLib_RSA_Common.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define VSECPRIM_START_SEC_CODE
#include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/**********************************************************************************************************************
 *  esl_calcSaltedHashRSASHA1_PSS()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_calcSaltedHashRSASHA1_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA1) wsHash,  /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
eslt_Length saltSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
VSECPRIM_P2VAR_PARA(eslt_Byte) saltedHash)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  if ((wsHash == NULL_PTR) || (saltedHash == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((returnValue = esl_CheckWorkSpaceHeader(&wsHash->header, ESL_MINSIZEOF_WS_SHA1)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
   /* returnValue already set */
  }
  else
  {
    eslt_WorkSpaceUnionHashFunctions_RSA wsHashUnion; /* PRQA S 0759 */ /* MD_MSR_Union */
    wsHashUnion.SHA1ws = *wsHash;

    returnValue = esl_calcSaltedHashRSA_PSS(&wsHashUnion, saltSize,  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      salt, messageDigest, ESL_SIZEOF_SHA1_DIGEST, saltedHash, ESL_HA_SHA1);

    if (returnValue == ESL_ERC_TOTAL_LENGTH_OVERFLOW)
    {
      returnValue = ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW;
    }
  }

  return returnValue;
}
#endif

#if (VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  esl_calcSaltedHashRSASHA256_PSS()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_calcSaltedHashRSASHA256_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA256) wsHash,  /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  eslt_Length saltSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
  VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
  VSECPRIM_P2VAR_PARA(eslt_Byte) saltedHash)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if ((wsHash == NULL_PTR) || (salt == NULL_PTR) || (messageDigest == NULL_PTR) || (saltedHash == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((returnValue = esl_CheckWorkSpaceHeader(&wsHash->header, ESL_MINSIZEOF_WS_SHA256)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* returnValue already set */
  }
  else
  {
    eslt_WorkSpaceUnionHashFunctions_RSA wsHashUnion; /* PRQA S 0759 */ /* MD_MSR_Union */
    wsHashUnion.SHA256ws = *wsHash;

    returnValue = esl_calcSaltedHashRSA_PSS(&wsHashUnion, saltSize, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_RSAPSSSHA256_SALTEDHASH */
      salt, messageDigest, ESL_SIZEOF_SHA256_DIGEST, saltedHash, ESL_HA_SHA2_256);

    if (returnValue == ESL_ERC_TOTAL_LENGTH_OVERFLOW)
    {
      returnValue = ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW;
    }
  }

  return returnValue;
}
#endif

#if (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/**********************************************************************************************************************
 *  esl_calcSaltedHashRSARIPEMD160_PSS()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_calcSaltedHashRSARIPEMD160_PSS(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRIPEMD160) wsHash,  /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
eslt_Length saltSize,
VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
VSECPRIM_P2VAR_PARA(eslt_Byte) saltedHash)
{
  eslt_ErrorCode returnValue = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if ((wsHash == NULL_PTR) || (saltedHash == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((returnValue = esl_CheckWorkSpaceHeader(&wsHash->header, ESL_MINSIZEOF_WS_RIPEMD160)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* returnValue already set */
  }
  else
  {
    eslt_WorkSpaceUnionHashFunctions_RSA wsHashUnion; /* PRQA S 0759 */ /* MD_MSR_Union */
    wsHashUnion.RIPEMD160ws = *wsHash;

    returnValue = esl_calcSaltedHashRSA_PSS(&wsHashUnion, saltSize, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      salt, messageDigest, ESL_SIZEOF_RIPEMD160_DIGEST, saltedHash, ESL_HA_RMD160);

    if (returnValue == ESL_ERC_TOTAL_LENGTH_OVERFLOW)
    {
      returnValue = ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW;
    }
  }

  return returnValue;
}

#endif

#define VSECPRIM_STOP_SEC_CODE
#include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSA_PSS.c
 *********************************************************************************************************************/
