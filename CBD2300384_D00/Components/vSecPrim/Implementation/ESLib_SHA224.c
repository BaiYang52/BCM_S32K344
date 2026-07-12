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
/*!        \file  ESLib_SHA224.c
 *        \brief  SHA-224 implementation.
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

#define ESLIB_SHA224_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ESLib.h"
#include "ESLib_Helper.h"

#include "actISHA2_32.h"

#if (VSECPRIM_SHA2_256_ENABLED == STD_ON)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  esl_initSHA224()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initSHA224(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA224) workSpace)
{
  eslt_ErrorCode retVal;

  if (workSpace == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_SHA224)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal already set */
  }
  else
  {
    /* Init actCLib SHA-224 */
    (void)actSHA224Init(&workSpace->wsSHA224); /* SBSW_VSECPRIM_CALL_FUNCTION */
    esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_SHA224); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_updateSHA224()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSHA224(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA224) workSpace, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  const eslt_Length inputSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) input)
{
  eslt_ErrorCode retVal;
  if ((workSpace == NULL_PTR) || (input == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_SHA224, ESL_WST_ALGO_SHA224)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    /* Update actCLib SHA-224 */
    actRETURNCODE result = actSHA224Update(&workSpace->wsSHA224, /* SBSW_VSECPRIM_CALL_FUNCTION */
                                           (VSECPRIM_P2CONST_PARA(actU8))input,
                                           (actLengthType)inputSize,
                                           workSpace->header.watchdog);
    if (result == actEXCEPTION_LENGTH)
    {
      retVal = ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_finalizeSHA224()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSHA224(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA224) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest)
{
  eslt_ErrorCode retVal;
  if ((workSpace == NULL_PTR) || (messageDigest == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_SHA224, ESL_WST_ALGO_SHA224)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    /* Finalize actCLib SHA-224 */
    (void)actSHA224Finalize(&workSpace->wsSHA224,
                            (VSECPRIM_P2VAR_PARA(actU8))messageDigest,
                            workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_SHA_FINALIZE */

    /* Reset and clear workspace */
    esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsSHA224); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_SHA2_256_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_SHA224.c
 *********************************************************************************************************************/
