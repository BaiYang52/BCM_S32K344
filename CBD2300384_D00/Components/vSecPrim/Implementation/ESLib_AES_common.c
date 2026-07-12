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
/*!        \file  ESLib_AES_common.c
 *        \brief  AES common includes, types and functions.
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

#define ESLIB_AES_COMMON_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ESLib_AES_common.h"

#if ((VSECPRIM_AES128_ENABLED == STD_ON) || (VSECPRIM_AES192_ENABLED == STD_ON) || (VSECPRIM_AES256_ENABLED == STD_ON)) /* COV_VSECPRIM_NO_SAFE_CONFIG TF tf xf xf */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  esl_AES_setBlockAndPaddingModes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_AES_setBlockAndPaddingModes(VSECPRIM_P2VAR_PARA(eslt_Byte) target, eslt_BlockMode extBlockMode, eslt_PaddingMode extPaddingMode)
{
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;
  *target = actPADDING_PM_OFF; /* SBSW_VSECPRIM_WRITE_POINTER */

  /* #-- Check paddingMode */
  switch (extPaddingMode)
  {
    case ESL_PM_PKCS5:
    {
      *target = actPADDING_PM_PKCS7; /* SBSW_VSECPRIM_WRITE_POINTER */
      break;
    }
    case ESL_PM_OFF:
    {
      /* target was already set to off */
      break;
    }
    case ESL_PM_ONEWITHZEROES:
    {
      *target = actPADDING_PM_ONEWITHZEROES; /* SBSW_VSECPRIM_WRITE_POINTER */
      break;
    }
    default:
    {
      retVal = ESL_ERC_MODE_INVALID;
      break;
    }
  }

  /* #-- Check block mode */
  switch (extBlockMode)
  {
    case ESL_BM_ECB:
    {
      *target |= actAES_BM_ECB; /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */ /* SBSW_VSECPRIM_WRITE_POINTER */
      break;
    }
    case ESL_BM_CBC:
    {
      *target |= actAES_BM_CBC; /* SBSW_VSECPRIM_WRITE_POINTER */
      break;
    }
    case ESL_BM_CTR:
    {
      *target |= actAES_BM_CTR; /* SBSW_VSECPRIM_WRITE_POINTER */
      break;
    }
    default:
    {
      retVal = ESL_ERC_MODE_INVALID;
      break;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_AES_CommonInitChecks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_AES_CommonInitChecks(VSECPRIM_P2CONST_PARA(eslt_WorkSpaceAES) workSpace, eslt_Length wsSize, VSECPRIM_P2CONST_PARA(eslt_Byte) key)
{
  eslt_ErrorCode retVal;

  if (workSpace == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    retVal = esl_CheckWorkSpaceHeader(&workSpace->header, wsSize); /* SBSW_VSECPRIM_CALL_FUNCTION */
    if ((retVal == ESL_ERC_NO_ERROR) && (key == NULL_PTR))
    {
      retVal = ESL_ERC_PARAMETER_INVALID;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_AES_CommonInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_AES_CommonInit(VSECPRIM_P2VAR_PARA(eslt_Byte) targetMode, VSECPRIM_P2CONST_PARA(eslt_WorkSpaceAES) workSpace, eslt_Length wsSize, VSECPRIM_P2CONST_PARA(eslt_Byte) key,
                                                        eslt_BlockMode blockMode, eslt_PaddingMode paddingMode)
{
  eslt_ErrorCode retVal = esl_AES_CommonInitChecks(workSpace, wsSize, key); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FUNCTION_CALL_POINTER_CHECKED_WITHIN */

  if (retVal == ESL_ERC_NO_ERROR)
  {
    retVal = esl_AES_setBlockAndPaddingModes(targetMode, blockMode, paddingMode); /* SBSW_VSECPRIM_FUNCTION_CALL_POINTER_CHECKED_WITHIN */
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  esl_AES_CommonProcChecks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_AES_CommonProcChecks(VSECPRIM_P2CONST_PARA(eslt_WorkSpaceAES) workSpace, eslt_WSStatus algo, eslt_Length wsSize, VSECPRIM_P2CONST_PARA(eslt_Byte) inBuffer, VSECPRIM_P2CONST_PARA(eslt_Byte) outBuffer)
{
  eslt_ErrorCode retVal;

  if ((workSpace == NULL_PTR) || (inBuffer == NULL_PTR) || (outBuffer == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, wsSize, algo); /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  }

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_AES128_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_AES_common.h
 *********************************************************************************************************************/
