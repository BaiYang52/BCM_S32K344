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
/*!        \file  ESLib_KDFX963_SHA256.c
 *        \brief  KDFX963 key derivation function according to ANSI X9.63 using SHA-256 as underlying hash function.
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

#define ESLIB_KDFX963SHA256_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ESLib.h"
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIKDFX963_SHA256.h"

#if (VSECPRIM_ANSI_X963_SHA256_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  esl_initKDFX963SHA256
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initKDFX963SHA256(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDFX963SHA256) workSpace)
{
  eslt_ErrorCode retVal;
  /* validate parameters */
  if (workSpace == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_KDFX963SHA256)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal was already set */
  }
  else
  {
    /* set workSpace state */
    esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_KDFX963SHA256); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }
  return retVal;
}

/***********************************************************************************************************************
 *  esl_deriveKeyKDFX963SHA256
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_deriveKeyKDFX963SHA256(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDFX963SHA256) workSpace,
  const eslt_Length secretLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) secret,
  const eslt_Length infoLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) info,
  const eslt_Length keyLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) key)
{
  eslt_ErrorCode retVal;

  /* validate parameters */
  if ((workSpace == NULL_PTR) || (secret == NULL_PTR) || (key == NULL_PTR) || ((infoLength != 0u) && (info == NULL_PTR)))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* check length */
  else if ((secretLength == 0u) || (keyLength == 0u))
  {
    retVal = ESL_ERC_INPUT_INVALID;
  }
  /* check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_KDFX963SHA256, ESL_WST_ALGO_KDFX963SHA256)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    /* derive key with actCLib KDF X9.63 and SHA-256 */
    actKDFX963_SHA256_Derive(&workSpace->wsKDFX963SHA256, secret, secretLength, info, infoLength, key, keyLength, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ANSI_X963_SHA256_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_KDFX963SHA256.c
 *********************************************************************************************************************/

