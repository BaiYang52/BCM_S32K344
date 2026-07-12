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
/*!        \file  ESLib_getLengthEcP.c
 *        \brief  ECC length info implementation.
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

#define ESLIB_GETLENGTHECP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ESLib.h"

/* actCLib includes */
#include "actIECKey.h"

#if (VSECPRIM_ESLGETLENGTHECP_ENABLED == STD_ON)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * esl_getLengthOfEcPprivateKey()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_Length) esl_getLengthOfEcPprivateKey(VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain)
{
  eslt_Length result;
  if (domain == NULL_PTR)
  {
    result = 0u;
  }
  else
  {
    result = (eslt_Length)(actECGetPrivateKeyLength(domain)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }
  return result;
}

/**********************************************************************************************************************
 * esl_getLengthOfEcPpublicKey_comp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_Length) esl_getLengthOfEcPpublicKey_comp(VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain)
{
  eslt_Length result;
  if (domain == NULL_PTR)
  {
    result = 0u;
  }
  else
  {
    result = (eslt_Length)(actECGetPublicKeyCompLength(domain)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }
  return result;
}

/**********************************************************************************************************************
 * esl_getMaxLengthOfEcPmessage()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_Length) esl_getMaxLengthOfEcPmessage(VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain)
{
  eslt_Length result;
  if (domain == NULL_PTR)
  {
    result = 0u;
  }
  else
  {
    result = esl_getLengthOfEcPprivateKey(domain); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }
  return result;
}

/**********************************************************************************************************************
 * esl_getLengthOfEcPsignature_comp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_Length) esl_getLengthOfEcPsignature_comp(VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain)
{
  eslt_Length result;
  if (domain == NULL_PTR)
  {
    result = 0u;
  }
  else
  {
    result = esl_getLengthOfEcPprivateKey(domain); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }
  return result;
}

/**********************************************************************************************************************
 * esl_getLengthOfEcPsecret_comp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_Length) esl_getLengthOfEcPsecret_comp(VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain)
{
  eslt_Length result;
  if (domain == NULL_PTR)
  {
    result = 0u;
  }
  else
  {
    result = esl_getLengthOfEcPpublicKey_comp(domain); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }
  return result;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ESLGETLENGTHECP_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_getLengthEcP.c
 *********************************************************************************************************************/
