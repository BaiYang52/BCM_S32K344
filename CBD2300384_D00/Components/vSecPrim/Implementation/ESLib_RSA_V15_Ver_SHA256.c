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
/*!        \file  ESLib_RSA_V15_Ver_SHA256.c
 *        \brief  RSA V1.5 (Sign_SHA256/Verify_SHA256) implementation.
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
#define ESLIB_RSA_V15_VER_SHA256_SOURCE

#include "ESLib.h"
#include "ESLib_Helper.h"
#include "ESLib_ASN_1.h"

#include "actIRSA.h"
#include "actISHA2_32.h"

#if (VSECPRIM_RSA_V15_SHA2_256_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/
/* Hash initialization function */
# define ESL_RSA_V15_INIT_HASH_FCT(workspace)                         (void)actSHA256Init(&((workspace)->wsSHA256))
/* Hash update function */
# define ESL_RSA_V15_UPDATE_HASH_FCT(workspace, inputSize, input)     actSHA256Update(&((workspace)->wsSHA256), (VSECPRIM_P2CONST_PARA(actU8))(input), (actLengthType)(inputSize), (workspace)->header.watchdog)
/* Hash finalization function */
# define ESL_RSA_V15_FINALIZE_HASH_FCT(workspace, messageDigest)      (void)actSHA256Finalize(&((workspace)->wsSHA256), (VSECPRIM_P2VAR_PARA(actU8))(messageDigest),  (workspace)->header.watchdog)

/****************************************************************************
 ** Types and constants
 ***************************************************************************/
# define VSECPRIM_START_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM(VSECPRIM_LOCAL, eslt_Byte) eslt_VERIFY_RSA_SHA256_V15_ASN1_DIGESTINFO[] = ASN1_DIGESTINFO_SHA256; /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */

# define VSECPRIM_STOP_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** Local Functions
 ***************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/**********************************************************************************************************************
 *  esl_initVerifyRSASHA256_V15()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_initVerifyRSASHA256_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace,
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
    if (workSpace->header.size < ESL_MAXSIZEOF_WS_RSA_VER)
    {
      returnValue = ESL_ERC_WS_TOO_SMALL;
    }
    else
    {
      workSpace->wsHashUnion.SHA256ws.header.size = (eslt_Length) (sizeof(eslt_WorkSpaceSHA256) - sizeof(eslt_WorkSpaceHeader));
      workSpace->wsHashUnion.SHA256ws.header.watchdog = workSpace->header.watchdog;
      ESL_RSA_V15_INIT_HASH_FCT(&(workSpace->wsHashUnion.SHA256ws));

      workSpace->wsRSAVerPrim.header.size = ESL_MAXSIZEOF_WS_RSA_PRIM;
      workSpace->wsRSAVerPrim.header.watchdog = workSpace->header.watchdog;
      returnValue = esl_initVerifyRSA_prim(&(workSpace->wsRSAVerPrim), keyPairModuleSize, keyPairModule, publicKeyExponentSize, publicKeyExponent);
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
 *  esl_updateVerifyRSASHA256_V15()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateVerifyRSASHA256_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace, eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input)
{
  eslt_ErrorCode returnValue;

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_RSA_VER, ESL_WST_ALGO_RSA);
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    if (actOK != ESL_RSA_V15_UPDATE_HASH_FCT(&(workSpace->wsHashUnion.SHA256ws), inputSize, input))
    {
      returnValue = ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW;
    }
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeHashVerifyRSASHA1_V15()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashVerifyRSASHA256_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest)
{
  eslt_ErrorCode returnValue;

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_RSA_VER, ESL_WST_ALGO_RSA);
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    ESL_RSA_V15_FINALIZE_HASH_FCT(&(workSpace->wsHashUnion.SHA256ws), messageDigest);
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  esl_verifySigVerifyRSASHA256_V15()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySigVerifyRSASHA256_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace,
                                                               VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest, eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{

  eslt_ErrorCode returnValue;
  eslt_Length keyPairModuleSize, i;
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageBuf;

  if ((workSpace == NULL_PTR) || (messageDigest == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    /* get underlying workspace pointer */
    messageBuf = workSpace->messageBuffer;
    keyPairModuleSize = workSpace->modulusSizeInformation;

    returnValue = esl_verifyRSA_prim(&(workSpace->wsRSAVerPrim), signatureSize, signature, &keyPairModuleSize, messageBuf);

    if (ESL_ERC_NO_ERROR == returnValue)
    {
      if (keyPairModuleSize < (ESL_SIZEOF_SHA256_DIGEST + ((eslt_Length) sizeof(eslt_VERIFY_RSA_SHA256_V15_ASN1_DIGESTINFO))))
      {
        returnValue = ESL_ERC_RSA_MODULE_OUT_OF_RANGE;
      }
      else
      {
        i = keyPairModuleSize - ESL_SIZEOF_SHA256_DIGEST - ((eslt_Length) sizeof(eslt_VERIFY_RSA_SHA256_V15_ASN1_DIGESTINFO));
        returnValue = esl_verifyPaddingRSAEM_V15(messageBuf, &i, ASN1_PADDING_BLOCK_TYPE_PRIVATE);

        if ((signatureSize != keyPairModuleSize) || (returnValue != ESL_ERC_NO_ERROR))
        {
          returnValue = ESL_ERC_RSA_SIGNATURE_INVALID;
        }
        else
        {
          if (TRUE != actMemcmp_ROMRAM((VSECPRIM_P2CONST_PARA(actU8))&messageBuf[i], eslt_VERIFY_RSA_SHA256_V15_ASN1_DIGESTINFO, sizeof(eslt_VERIFY_RSA_SHA256_V15_ASN1_DIGESTINFO))) /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
          {
            returnValue = ESL_ERC_RSA_SIGNATURE_INVALID;
          }
          else
          {
            i += ((eslt_Length) sizeof(eslt_VERIFY_RSA_SHA256_V15_ASN1_DIGESTINFO));

            if (TRUE != actMemcmp((VSECPRIM_P2CONST_PARA(actU8)) messageDigest, (VSECPRIM_P2CONST_PARA(actU8))&messageBuf[i], ESL_SIZEOF_SHA256_DIGEST)) /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
            {
              returnValue = ESL_ERC_RSA_SIGNATURE_INVALID;
            }
          }
        }
      }
    }
  }

  return returnValue;
} /* PRQA S 6080 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

/**********************************************************************************************************************
 *  esl_finalizeVerifyRSASHA256_V15()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeVerifyRSASHA256_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAver) workSpace, eslt_Length signatureSize, VSECPRIM_P2CONST_PARA(eslt_Byte) signature)
{
  eslt_ErrorCode returnValue;
  eslt_Byte hash[ESL_SIZEOF_SHA256_DIGEST];

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    returnValue = esl_finalizeHashVerifyRSASHA256_V15(workSpace, hash);
    if (returnValue == ESL_ERC_NO_ERROR)
    {
      returnValue = esl_verifySigVerifyRSASHA256_V15(workSpace, hash, signatureSize, signature);
    }
  }

  return returnValue;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_RSA_V15_SHA2_256_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSA_V15_Ver_SHA256.c
 *********************************************************************************************************************/
