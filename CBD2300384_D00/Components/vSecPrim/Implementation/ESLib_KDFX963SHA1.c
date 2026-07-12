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
/*!        \file  ESLib_KDFX963SHA1.c
 *        \brief  KDFX963 key derivation function according to ANSI X9.63 using SHA-1 as underlying hash function.
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

#define ESLIB_KDFX963SHA1_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ESLib.h"
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIKDFX963.h"

#if (VSECPRIM_ANSI_X963_ENABLED == STD_ON)

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
 *  esl_initKDFX963SHA1
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initKDFX963SHA1(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDFX963SHA1) workSpace)
{
  eslt_ErrorCode retVal;
  /* any NULLs */
  if (workSpace == NULL_PTR)
  {
    retVal =  ESL_ERC_PARAMETER_INVALID;
  }
  /* check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_KDFX963SHA1)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal was already set */
  }
  else
  {
  /* set workSpace state */
    esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_KDFX963SHA1); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
}

/***********************************************************************************************************************
 *  esl_deriveKeyKDFX963SHA1
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_deriveKeyKDFX963SHA1(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDFX963SHA1) workSpace,
                                                       const eslt_Length secretLength, VSECPRIM_P2CONST_PARA(eslt_Byte) secret,
                                                       const eslt_Length infoLength, VSECPRIM_P2CONST_PARA(eslt_Byte) info, const eslt_Length keyLength, VSECPRIM_P2VAR_PARA(eslt_Byte) key)
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
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_KDFX963SHA1, ESL_WST_ALGO_KDFX963SHA1)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    /* derive key with actCLib KDFX963 */
    actKDFX963Derive(&workSpace->wsKDFX963SHA1, secret, secretLength, info, infoLength, key, keyLength, workSpace->header.watchdog);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ANSI_X963_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_KDFX963SHA1.c
 *********************************************************************************************************************/

