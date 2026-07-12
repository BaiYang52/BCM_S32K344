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
/*!        \file  ESLib_HashMACRMD160.c
 *        \brief  Hash MAC RIPEMD-160 implementation.
 *
 *      \details  Currently the actClib version is used.
 *                This file is part of the embedded systems library cvActLib/ES
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

#define ESLIB_HASHMACRMD160_SOURCE

#include "ESLib.h"
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIHashMACRMD160.h"

#if (VSECPRIM_HMAC_RMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 * esl_initHashMACRIPEMD160
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initHashMACRIPEMD160(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACRIPEMD160) workSpace, const eslt_Length keyLength, VSECPRIM_P2CONST_PARA(eslt_Byte) key)
{
  eslt_ErrorCode retVal;
  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (key == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* Check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_HMACRIPEMD160)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  /* Check keyLength */
  else if (keyLength == 0u)
  {
    retVal = ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE;
  }
  else
  {
    /* Init actCLib HMAC-RIPEMD-160; it is safe to void actHashMACRMD160Init, since actHashMACRMD160Init only returns
     * actEXCEPTION_LENGTH, if keyLength < 1, and this was checked before */
    (void)actHashMACRMD160Init(&workSpace->wsHMACRIPEMD160, key, keyLength, workSpace->header.watchdog);

    /* Set workSpace state */
    esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_HMACRIPEMD160);
  }

  return retVal;
}

/****************************************************************************
 * esl_updateHashMACRIPEMD160
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateHashMACRIPEMD160(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACRIPEMD160) workSpace, const eslt_Length inputLength, VSECPRIM_P2CONST_PARA(eslt_Byte) input)
{
  eslt_ErrorCode retVal;
  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (input == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* Check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_HMACRIPEMD160, ESL_WST_ALGO_HMACRIPEMD160)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  /* Update actCLib HMAC-RIPEMD-160 */
  else
  {
    actRETURNCODE result = actHashMACRMD160Update(&workSpace->wsHMACRIPEMD160,
                                                  input, inputLength, workSpace->header.watchdog);
    if (result == actEXCEPTION_LENGTH)
    {
      retVal = ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW;
    }
  }

  return retVal;
}

/****************************************************************************
 * esl_finalizeHashMACRIPEMD160
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeHashMACRIPEMD160(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACRIPEMD160) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageHashMAC)
{
  eslt_ErrorCode retVal;

  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (messageHashMAC == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* Check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_HMACRIPEMD160, ESL_WST_ALGO_HMACRIPEMD160)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  else
  {
    /* Finalize actCLib HMAC-RIPEMD-160; actHashMACRMD160Finalize always returns actOK */
    (void)actHashMACRMD160Finalize(&workSpace->wsHMACRIPEMD160, messageHashMAC, workSpace->header.watchdog);

    /* Reset and clear workspace */
    esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsHMACRIPEMD160); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
  }

  return retVal;
}

/****************************************************************************
 * esl_verifyHashMACRIPEMD160
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyHashMACRIPEMD160(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHMACRIPEMD160) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) messageHashMAC)
{
  eslt_ErrorCode retVal;
  eslt_Byte tmpMAC[ESL_SIZEOF_RIPEMD160_DIGEST];

  actMemClear(tmpMAC, ESL_SIZEOF_RIPEMD160_DIGEST); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (messageHashMAC == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* Check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_HMACRIPEMD160, ESL_WST_ALGO_HMACRIPEMD160)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */
  {
    /* retVal already set */
  }
  else
  {
    /* Verify MAC; it is safe to void esl_finalizeHashMACRIPEMD160, since all relevant error conditions for
     * esl_finalizeHashMACRIPEMD160 were checked before */
    (void)esl_finalizeHashMACRIPEMD160(workSpace, tmpMAC);
    if (actMemcmp(tmpMAC, messageHashMAC, ESL_SIZEOF_RIPEMD160_DIGEST) != TRUE) /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
    {
      retVal = ESL_ERC_HMAC_INCORRECT_MAC;
    }
  }

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_HMAC_RMD160_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_HashMACRMD160.c
 *********************************************************************************************************************/
