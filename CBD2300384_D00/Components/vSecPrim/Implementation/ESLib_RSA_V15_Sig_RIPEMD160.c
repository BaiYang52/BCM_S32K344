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
/*!        \file  ESLib_RSA_V15_Sig_RIPEMD160.c
 *        \brief  RSA V1.5 (Sign_RIPEMD160/Verify_RIPEMD160) implementation.
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

#define ESLIB_RSA_V15_SIG_RIPEMD160_SOURCE

#include "ESLib.h"
#include "ESLib_Helper.h"
#include "ESLib_ASN_1.h"

#include "actIRSA.h"
#include "actIRMD160.h"

#if (VSECPRIM_RSA_V15_RIPEMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Hash initialization function */
# define ESL_RSA_V15_INIT_HASH_FCT(workspace)                         (void)actRMD160Init(&((workspace)->wsRIPEMD160))
/* Hash update function */
# define ESL_RSA_V15_UPDATE_HASH_FCT(workspace, inputSize, input)     actRMD160Update(&((workspace)->wsRIPEMD160), (VSECPRIM_P2CONST_PARA(actU8))(input), (inputSize),  (workspace)->header.watchdog)
/* Hash finalization function */
# define ESL_RSA_V15_FINALIZE_HASH_FCT(workspace, messageDigest)      (void)actRMD160Finalize(&((workspace)->wsRIPEMD160), (VSECPRIM_P2VAR_PARA(actU8))(messageDigest), (workspace)->header.watchdog)

/****************************************************************************
 ** Types and constants
 ***************************************************************************/
# define VSECPRIM_START_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM(VSECPRIM_LOCAL, eslt_Byte) eslt_SIGN_RSA_RIPEMD160_V15_ASN1_DIGESTINFO[] = ASN1_DIGESTINFO_RIPEMD160; /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */

# define VSECPRIM_STOP_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** Global Functions
 ***************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

 /**********************************************************************************************************************
  *  esl_initSignRSARIPEMD160_V15()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignRSARIPEMD160_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig) workSpace,
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
        workSpace->wsHashUnion.RIPEMD160ws.header.size = ESL_MAXSIZEOF_WS_RIPEMD160;
        workSpace->wsHashUnion.RIPEMD160ws.header.watchdog = workSpace->header.watchdog;
        ESL_RSA_V15_INIT_HASH_FCT(&(workSpace->wsHashUnion.RIPEMD160ws));
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
 *  esl_updateSignRSARIPEMD160_V15()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSignRSARIPEMD160_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig) workSpace, eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input)
{

  eslt_ErrorCode returnValue = ESL_ERC_NO_ERROR;

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
      if (actOK != ESL_RSA_V15_UPDATE_HASH_FCT(&(workSpace->wsHashUnion.RIPEMD160ws), inputSize, input))
      {
        returnValue = ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW;
      }
    }
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  esl_finalizeSignRSARIPEMD160_V15()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSignRSARIPEMD160_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSAsig) workSpace,
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
      messageBuf[i + 1u] = 0x01u;
      i += 2u;

      /* During esl_initSignRSARIPEMD160_V15 it is checked that
       * keyPairModuleSize >= ASN1_MINIMAL_SIGVER_KEY_LENGTH = 46,
       * so in keyPairModuleSize - ASN1_SIZEOF_PARAMETERS_WITH_FIXED_LENGTH = keyPairModuleSize - 38
       * no overflow occurs. 
       * Also, padBytesNeeded >= 46 - 38 = 8 = ASN1_MINIMAL_PADDING_LENGTH, so no additional check is required. */
      padBytesNeeded = keyPairModuleSize - ASN1_SIZEOF_PARAMETERS_WITH_FIXED_LENGTH;

      actMemset((VSECPRIM_P2VAR_PARA(actU8)) (&messageBuf[i]), 0xFFu, padBytesNeeded); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
      i = padBytesNeeded + i;
      messageBuf[i] = 0x00u;
      i += 1u;
      actMemCpyByteArray((VSECPRIM_P2VAR_PARA(actU8)) (&messageBuf[i]), eslt_SIGN_RSA_RIPEMD160_V15_ASN1_DIGESTINFO, sizeof(eslt_SIGN_RSA_RIPEMD160_V15_ASN1_DIGESTINFO)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
      i += ((eslt_Length) sizeof(eslt_SIGN_RSA_RIPEMD160_V15_ASN1_DIGESTINFO));
      ESL_RSA_V15_FINALIZE_HASH_FCT(&(workSpace->wsHashUnion.RIPEMD160ws), &messageBuf[i]);

      returnValue = esl_signRSA_prim(&(workSpace->wsRSASigPrim), keyPairModuleSize, messageBuf, signatureSize, signature);
    }
  }
  return returnValue;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_RSA_V15_RIPEMD160_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSA_V15_Sig_RIPEMD160.c
 *********************************************************************************************************************/
