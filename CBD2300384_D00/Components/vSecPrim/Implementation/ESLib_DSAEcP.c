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
/*!        \file  ESLib_DSAEcP.c
 *        \brief  DSAEcP implementation.
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

#define ESLIB_DSAECP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ESLib.h"
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIECDSA.h"

#if (VSECPRIM_ECDSA_GENERIC_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * esl_initSignDSAEcP_prim()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initSignDSAEcP_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomainExt) domainExt,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccSpeedUpExt) speedUpExt)
{
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;
  /* Need speed up extensions */
  if ((workSpace == NULL_PTR) || (domain == NULL_PTR) || (domainExt == NULL_PTR) || (speedUpExt == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_ECP)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    /* Init actCLib EC-DSA sign */
    actRetVal = actECDSAInitSign(domain, domainExt, speedUpExt, &(workSpace->wsEcP), workSpace->header.size); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actRetVal != actOK)
    {
      if (actRetVal == actEXCEPTION_MEMORY)
      {
        retVal = ESL_ERC_WS_TOO_SMALL;
      }
      else if (actRetVal == actEXCEPTION_DOMAIN)
      {
        retVal = ESL_ERC_ECC_DOMAIN_INVALID;
      }
      else if (actRetVal == actEXCEPTION_DOMAIN_EXT)
      {
        retVal = ESL_ERC_ECC_DOMAINEXT_INVALID;
      }
      else /* actRetVal == actEXCEPTION_SPEEDUP_EXT */
      {
        retVal = ESL_ERC_ECC_SPEEDUPEXT_INVALID;
      }
    }
    else
    {
      retVal = ESL_ERC_NO_ERROR;
      /* set workSpace state */
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_ECP); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * esl_signDSAEcP_prim()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_signDSAEcP_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace,
  eslt_Length messageLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) message,
  VSECPRIM_P2CONST_PARA(eslt_Byte) privateKey,
  VSECPRIM_P2VAR_PARA(eslt_Length) signature_rLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) signature_r,
  VSECPRIM_P2VAR_PARA(eslt_Length) signature_sLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) signature_s)
{
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;

  actLengthType r_len;
  actLengthType s_len;

  if ((workSpace == NULL_PTR) || (message == NULL_PTR) || (privateKey == NULL_PTR) ||
    (signature_rLength == NULL_PTR) || (signature_r == NULL_PTR) || (signature_sLength == NULL_PTR) || (signature_s == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_ECP, ESL_WST_ALGO_ECP)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    r_len = (actLengthType)(*signature_rLength);
    s_len = (actLengthType)(*signature_sLength);

    /* Call actCLib EC-DSA signature generation */
    actRetVal = actECDSASign(message, messageLength, privateKey, signature_r, &r_len, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      signature_s, &s_len, &(workSpace->wsEcP), workSpace->header.watchdog);

    if (actRetVal != actOK)
    {
      if (actRetVal == actEXCEPTION_MEMORY)
      {
        retVal = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
      }
      else if (actRetVal == actEXCEPTION_LENGTH)
      {
        retVal = ESL_ERC_ECC_MESSAGE_TOO_LONG;
      }
      else if (actRetVal == actEXCEPTION_PRIVKEY)
      {
        retVal = ESL_ERC_ECC_PRIVKEY_INVALID;
      }
      else if (actRetVal == actEXCEPTION_UNKNOWN)
      {
        retVal = ESL_ERC_ECC_INTERNAL_ERROR;
      }
      else /* actRetVal >= ESL_ERT_ERROR, workaround for random callback function esl_getBytesRNG */
      {
        retVal = (eslt_ErrorCode)actRetVal;
      }
    }
    else
    {
      retVal = ESL_ERC_NO_ERROR;
      *signature_rLength = (eslt_Length)r_len; /* SBSW_VSECPRIM_WRITE_POINTER */
      *signature_sLength = (eslt_Length)s_len; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  return retVal;
} /* PRQA S 6060, 6080 */ /* MD_VSECPRIM_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 * esl_initVerifyDSAEcP_prim()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initVerifyDSAEcP_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomainExt) domainExt)
{
  eslt_ErrorCode retVal;

  if ((workSpace == NULL_PTR) || (domain == NULL_PTR) || (domainExt == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_ECP)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal = already set */
  }
  else
  {
    /* Init actCLib EC-DSA verify */
    actRETURNCODE actRetVal = actECDSAInitVerify(domain, domainExt, &(workSpace->wsEcP), workSpace->header.size); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actRetVal != actOK)
    {
      /* Note: actEXCEPTION_MEMORY will not be returned during signature verification,
       * since the workspace size was already checked before. */
      if (actRetVal == actEXCEPTION_DOMAIN)
      {
        retVal = ESL_ERC_ECC_DOMAIN_INVALID;
      }
      else /* actRetVal == actEXCEPTION_DOMAIN_EXT */
      {
        retVal = ESL_ERC_ECC_DOMAINEXT_INVALID;
      }
    }
    else
    {
      retVal = ESL_ERC_NO_ERROR;
      /* set workSpace state */
      workSpace->header.status = (ESL_WST_ALGO_ECP | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL); /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * esl_verifyDSAEcP_prim()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyDSAEcP_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace,
  eslt_Length messageLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) message,
  VSECPRIM_P2CONST_PARA(eslt_Byte) publicKey_x,
  VSECPRIM_P2CONST_PARA(eslt_Byte) publicKey_y,
  eslt_Length signature_rLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) signature_r,
  eslt_Length signature_sLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) signature_s)
{
  actRETURNCODE actRetVal;
  eslt_ErrorCode retVal = ESL_ERC_ECC_SIGNATURE_INVALID; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if ((workSpace == NULL_PTR) || (message == NULL_PTR) || (publicKey_x == NULL_PTR) ||
      (publicKey_y == NULL_PTR) || (signature_r == NULL_PTR) || (signature_s == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_ECP, ESL_WST_ALGO_ECP)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
  /* Call actCLib EC-DSA signature verification */
    actRetVal = actECDSAVerify(message, messageLength, signature_r, signature_rLength, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                               signature_s, signature_sLength, publicKey_x, publicKey_y, &(workSpace->wsEcP), workSpace->header.watchdog);
    if (actRetVal != actOK)
    {
      if (actRetVal == actEXCEPTION_LENGTH)
      {
        retVal = ESL_ERC_ECC_MESSAGE_TOO_LONG;
      }
      else if (actRetVal == actEXCEPTION_PUBKEY)
      {
        retVal = ESL_ERC_ECC_PUBKEY_INVALID;
      }
      else if ((actRetVal == actSIGNATURE_OUT_OF_RANGE) || (actRetVal == actSIGNATURE_INVALID))
      {
        retVal = ESL_ERC_ECC_SIGNATURE_INVALID;
      }
      else  /* actRetVal == actEXCEPTION_UNKNOWN */
      {
        retVal = ESL_ERC_ECC_INTERNAL_ERROR;
      }
    }
    else
    {
      retVal = ESL_ERC_NO_ERROR;
    }
  }

  return retVal;
} /* PRQA S 6060, 6080 */ /* MD_VSECPRIM_STPAR, MD_MSR_STMIF */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ECDSA_GENERIC_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_DSAEcP.c
 *********************************************************************************************************************/
