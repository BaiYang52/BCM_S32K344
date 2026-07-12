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
/*!        \file  ESLib_DHEcP.c
 *        \brief  DHEcP implementation.
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

#define ESLIB_DHECP_SOURCE

#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIECDH.h"

#if (VSECPRIM_ECDH_GENERIC_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/* PRQA S 6060, 6080 EOF */ /* MD_VSECPRIM_QM_ONLY_METRIC */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/****************************************************************************
 * esl_initGenerateSharedSecretDHEcP_prim
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initGenerateSharedSecretDHEcP_prim(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace,
                                                                     VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain, VSECPRIM_P2ROMCONST_PARA(eslt_EccDomainExt) domainExt)
{
  eslt_ErrorCode retVal;

  if ((workSpace == NULL_PTR) || (domain == NULL_PTR) || (domainExt == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_ECP)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  else
  {
    /* init actCLib EC-DH primitive */
    actRETURNCODE result = actECDHInitGetSecret(domain, domainExt, &workSpace->wsEcP, (workSpace->header.size));
    if (result != actOK)
    {
      if (result == actEXCEPTION_NULL)
      {
        retVal = ESL_ERC_PARAMETER_INVALID;
      }
      else if (result == actEXCEPTION_MEMORY)
      {
        retVal = ESL_ERC_WS_TOO_SMALL;
      }
      else if (result == actEXCEPTION_DOMAIN)
      {
        retVal = ESL_ERC_ECC_DOMAIN_INVALID;
      }
      else /* (result == actEXCEPTION_DOMAIN_EXT) */
      {
        retVal = ESL_ERC_ECC_DOMAINEXT_INVALID;
      }
    }
    else
    {
      /* Set workSpace state */
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_ECP);
    }
  }

  return retVal;
}

/****************************************************************************
 * esl_generateSharedSecretDHEcP_prim
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_generateSharedSecretDHEcP_prim(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKey,
                                                                 VSECPRIM_P2CONST_PARA(eslt_Byte) publicKey_x, VSECPRIM_P2CONST_PARA(eslt_Byte) publicKey_y,
                                                                 VSECPRIM_P2VAR_PARA(eslt_Byte) secret_x, VSECPRIM_P2VAR_PARA(eslt_Byte) secret_y)
{
  eslt_ErrorCode retVal;
  actRETURNCODE result;

  /* Note: secret_y is allowed to be a null pointer and this is checked in actECDHGetSecret */
  if ((workSpace == NULL_PTR) || (privateKey == NULL_PTR) || (publicKey_x == NULL_PTR) || (publicKey_y == NULL_PTR) || (secret_x == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* Check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_ECP, ESL_WST_ALGO_ECP)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  else
  {
    /* call actCLib EC-DH secret generation */
    result = actECDHGetSecret(privateKey, publicKey_x, publicKey_y, secret_x, secret_y, &workSpace->wsEcP, workSpace->header.watchdog);
    if (result != actOK)
    {
      if (result == actEXCEPTION_NULL)
      {
        retVal = ESL_ERC_PARAMETER_INVALID;
      }
      else if (result == actEXCEPTION_PUBKEY)
      {
        retVal = ESL_ERC_ECC_PUBKEY_INVALID;
      }
      else if (result == actEXCEPTION_PRIVKEY)
      {
        retVal = ESL_ERC_ECC_PRIVKEY_INVALID;
      }
      else /* (result == actEXCEPTION_UNKNOWN) */
      {
        retVal = ESL_ERC_ECC_INTERNAL_ERROR;
      }
    }
  }

  return retVal;
}

/****************************************************************************
 * esl_initExchangeKeyDHEcP_key
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initExchangeKeyDHEcP_key(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace,
                                                           VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain, VSECPRIM_P2ROMCONST_PARA(eslt_EccDomainExt) domainExt)
{
  eslt_ErrorCode retVal;

  if ((workSpace == NULL_PTR) || (domain == NULL_PTR) || (domainExt == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_ECP)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  else
  {
    /* init actCLib EC-DH */
    actRETURNCODE result = actECDHInitKeyDerive(domain, domainExt, &workSpace->wsEcP, (workSpace->header.size));
    if (result != actOK)
    {
      if (result == actEXCEPTION_NULL)
      {
        retVal = ESL_ERC_PARAMETER_INVALID;
      }
      else if (result == actEXCEPTION_MEMORY)
      {
        retVal = ESL_ERC_WS_TOO_SMALL;
      }
      else if (result == actEXCEPTION_DOMAIN)
      {
        retVal = ESL_ERC_ECC_DOMAIN_INVALID;
      }
      else /* (result == actEXCEPTION_DOMAIN_EXT) */
      {
        retVal = ESL_ERC_ECC_DOMAINEXT_INVALID;
      }
    }
    else
    {
      /* Set workSpace state */
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_ECP);
    }
  }

  return retVal;
}

/****************************************************************************
 * esl_exchangeKeyDHEcP_key
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_exchangeKeyDHEcP_key(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace,
                                                       VSECPRIM_P2CONST_PARA(eslt_Byte) privateKey,
                                                       VSECPRIM_P2CONST_PARA(eslt_Byte) publicKey_x, VSECPRIM_P2CONST_PARA(eslt_Byte) publicKey_y,
                                                       eslt_Length infoLength, VSECPRIM_P2CONST_PARA(eslt_Byte) info,
                                                       eslt_Length iterationCount, eslt_Length keyLength, VSECPRIM_P2VAR_PARA(eslt_Byte) key)
{
  eslt_ErrorCode retVal;
  actRETURNCODE result;

  if ((workSpace == NULL_PTR) || (privateKey == NULL_PTR) || (publicKey_x == NULL_PTR) || (publicKey_y == NULL_PTR) || (key == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* Check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_ECP, ESL_WST_ALGO_ECP)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  else
  {
    /* call actCLib EC-DH secret generation */
    result = actECDHKeyDerive(privateKey, publicKey_x, publicKey_y, iterationCount, info, infoLength, key, keyLength, &workSpace->wsEcP, workSpace->header.watchdog);
    if (result != actOK)
    {
      if (result == actEXCEPTION_NULL)
      {
        retVal = ESL_ERC_PARAMETER_INVALID;
      }
      else if (result == actEXCEPTION_PUBKEY)
      {
        retVal = ESL_ERC_ECC_PUBKEY_INVALID;
      }
      else if (result == actEXCEPTION_PRIVKEY)
      {
        retVal = ESL_ERC_ECC_PRIVKEY_INVALID;
      }
      else /* (result == actEXCEPTION_UNKNOWN) */
      {
        retVal = ESL_ERC_ECC_INTERNAL_ERROR;
      }
    }
  }

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ECDH_GENERIC_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_DHEcP.c
 *********************************************************************************************************************/
