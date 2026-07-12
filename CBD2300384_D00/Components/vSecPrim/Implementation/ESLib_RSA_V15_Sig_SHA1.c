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
/*!        \file  ESLib_RSA_V15_Sig_SHA1.c
 *        \brief  RSA V1.5 (Sign_SHA1/Verify_SHA1) implementation.
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
#define ESLIB_RSA_V15_SIG_SHA1_SOURCE

#include "ESLib.h"
#include "ESLib_Helper.h"
#include "ESLib_ASN_1.h"

#include "actIRSA.h"
#include "actISHA.h"

#if (VSECPRIM_RSA_V15_SHA1_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Hash initialization function */
# define ESL_RSA_V15_INIT_HASH_FCT(workspace)                         actSHAInit(&((workspace)->wsSHA1))
/* Hash update function */
# define ESL_RSA_V15_UPDATE_HASH_FCT(workspace, inputSize, input)     actSHAUpdate(&((workspace)->wsSHA1), (VSECPRIM_P2CONST_PARA(actU8))(input), (actLengthType)(inputSize), (workspace)->header.watchdog)
/* Hash finalization function */
# define ESL_RSA_V15_FINALIZE_HASH_FCT(workspace, messageDigest)      actSHAFinalize(&((workspace)->wsSHA1), (VSECPRIM_P2VAR_PARA(actU8))(messageDigest),  (workspace)->header.watchdog)

/****************************************************************************
 ** Types and constants
 ***************************************************************************/
# define VSECPRIM_START_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM(VSECPRIM_LOCAL, eslt_Byte) eslt_SIGN_RSA_SHA1_V15_ASN1_DIGESTINFO[] = ASN1_DIGESTINFO_SHA1; /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */

# define VSECPRIM_STOP_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** Global Functions
 ***************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  esl_initSignRSASHA1_V15()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSASHA1_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig) workSpace,
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
    if (workSpace->header.size < ESL_MAXSIZEOF_WS_RSA_SIG)
    {
      returnValue = ESL_ERC_WS_TOO_SMALL;
    }
    else
    {
      if (keyPairModuleSize < ASN1_MINIMAL_SIGVER_KEY_LENGTH)
      {
        returnValue = ESL_ERC_RSA_MODULE_OUT_OF_RANGE;
      }
      else
      {
        workSpace->wsHashUnion.SHA1ws.header.size = (eslt_Length) (sizeof(eslt_WorkSpaceSHA1) - sizeof(eslt_WorkSpaceHeader));
        workSpace->wsHashUnion.SHA1ws.header.watchdog = workSpace->header.watchdog;
        /* actSHAInit always returns actOK */
        (void)ESL_RSA_V15_INIT_HASH_FCT(&(workSpace->wsHashUnion.SHA1ws));
        workSpace->wsRSASigPrim.header.size = ESL_MAXSIZEOF_WS_RSA_PRIM;
        workSpace->wsRSASigPrim.header.watchdog = workSpace->header.watchdog;
        returnValue = esl_initSignRSA_prim(&(workSpace->wsRSASigPrim), keyPairModuleSize, keyPairModule, privateKeyExponentSize, privateKeyExponent);
      }
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
 *  esl_updateSignRSASHA1_V15()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSignRSASHA1_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig) workSpace, eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input)
{

  eslt_ErrorCode returnValue;
  returnValue = ESL_ERC_NO_ERROR;

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    if (ESL_WST_ALGO_RSA != (workSpace->header.status & ESL_WST_M_ALGO))
    {
      returnValue = ESL_ERC_WS_STATE_INVALID;
    }
    else if (0u == (workSpace->header.status & ESL_WST_M_RUNNING))
    {
      returnValue = ESL_ERC_WS_STATE_INVALID;
    }
    else
    {
      if (actOK != ESL_RSA_V15_UPDATE_HASH_FCT(&(workSpace->wsHashUnion.SHA1ws), inputSize, input))
      {
        returnValue = ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW;
      }
    }
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeSignRSASHA1_V15()
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
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSASHA1_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig) workSpace,
                                                          VSECPRIM_P2VAR_PARA(eslt_Length) signatureSize, VSECPRIM_P2VAR_PARA(eslt_Byte) signature)
{

  eslt_ErrorCode returnValue;
  eslt_Length keyPairModuleSize, i, padBytesNeeded;
  VSECPRIM_P2VAR_PARA(eslt_Byte) messageBuf;

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    i = 0u;
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
    else
    {
      messageBuf[i] = 0x00u;
      messageBuf[i+1u] = 0x01u;
      i = i + 2u;

      /* During esl_initSignRSASHA1_V15 it is checked that
       * keyPairModuleSize >= ASN1_MINIMAL_SIGVER_KEY_LENGTH = 46,
       * so in keyPairModuleSize - ASN1_SIZEOF_PARAMETERS_WITH_FIXED_LENGTH = keyPairModuleSize - 38
       * no overflow occurs. 
       * Also, padBytesNeeded >= 46 - 38 = 8 = ASN1_MINIMAL_PADDING_LENGTH, so no additional check is required. */
      padBytesNeeded = keyPairModuleSize - ASN1_SIZEOF_PARAMETERS_WITH_FIXED_LENGTH;

      actMemset((VSECPRIM_P2VAR_PARA(actU8)) (&messageBuf[i]), 0xFFu, padBytesNeeded); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
      i = (eslt_Length) (padBytesNeeded + i);
      messageBuf[i] = 0x00;
      i = i + 1u;
      actMemCpyByteArray((VSECPRIM_P2VAR_PARA(actU8)) (&messageBuf[i]), eslt_SIGN_RSA_SHA1_V15_ASN1_DIGESTINFO, sizeof(eslt_SIGN_RSA_SHA1_V15_ASN1_DIGESTINFO)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
      i += ((eslt_Length) sizeof(eslt_SIGN_RSA_SHA1_V15_ASN1_DIGESTINFO));
      /* actSHAFinalize always returns actOK */
      (void)ESL_RSA_V15_FINALIZE_HASH_FCT(&(workSpace->wsHashUnion.SHA1ws), &messageBuf[i]);

      returnValue = esl_signRSA_prim(&(workSpace->wsRSASigPrim), keyPairModuleSize, messageBuf, signatureSize, signature);
    }
  }
  return returnValue;
} /* PRQA S 6080 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_RSA_V15_SHA1_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSA_V15_Sig_SHA1.c
 *********************************************************************************************************************/
