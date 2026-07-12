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
/*!        \file  ESLib_KGEcP.c
 *        \brief  EC-DH generate secret / key exchange implementation.
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

#define ESLIB_KGECP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ESLib.h"
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIECKey.h"

#if (VSECPRIM_ECCENABLED == STD_ON)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * esl_initGenerateDSAEcP_prim()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initGenerateKeyEcP_prim(
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
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_ECP)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  {
    /* retVal already set */
  }
  else
  {
    /* Init actCLib EC key generation */
    actRetVal = actECInitGenerateKeyPair(domain, domainExt, speedUpExt, &workSpace->wsEcP, workSpace->header.size); /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

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
      /* Set workSpace state */
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_ECP); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
  }

  return retVal;
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * esl_generateKeyEcP_prim()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_generateKeyEcP_prim(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceEcP) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) privateKey,
  VSECPRIM_P2VAR_PARA(eslt_Byte) publicKey_x,
  VSECPRIM_P2VAR_PARA(eslt_Byte) publicKey_y)
{
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;

  if ((workSpace == NULL_PTR) || (privateKey == NULL_PTR) || (publicKey_x == NULL_PTR) || (publicKey_y == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_ECP, ESL_WST_ALGO_ECP)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  {
    /* retVal already set */
  }
  else
  {
    /* Call actCLib EC key pair generation */
    actRetVal = actECGenerateKeyPair(TRUE, privateKey, publicKey_x, publicKey_y, &workSpace->wsEcP, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_GENERATEKEY_ECP */

    if (actRetVal == actEXCEPTION_UNKNOWN)
    {
      retVal = ESL_ERC_ECC_INTERNAL_ERROR;
    }
    else if (actRetVal == actEXCEPTION_PRIVKEY)
    {
      retVal = ESL_ERC_ECC_PRIVKEY_INVALID;
    }
    else /* actRetVal == actOK */
    {
      retVal = ESL_ERC_NO_ERROR;
    }
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ECCENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_KGEcP.c
 *********************************************************************************************************************/
