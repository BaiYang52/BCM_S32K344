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
/*!        \file  ESLib_MD5.c
 *        \brief  MD5 implementation.
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

#define ESLIB_MD5_SOURCE

#include "ESLib.h"
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIMD5.h"

#if (VSECPRIM_MD5_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/* PRQA S 2889 EOF */ /* MD_VSECPRIM_QM_ONLY_RETURN_PATHS */
/* PRQA S 0310, 3305 EOF */ /* MD_VSECPRIM_QM_ONLY_CASTING_POINTER_TYPE */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 * esl_initMD5
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initMD5(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceMD5) workSpace)
{
  /* Any NULLs ? */
  if (workSpace == NULL_PTR)
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* Check workSpace */
  if (workSpace->header.size < ESL_MINSIZEOF_WS_MD5)
  {
    return ESL_ERC_WS_TOO_SMALL;
  }

  /* Init actCLib MD5; actMD5Init always returns actOK */
  (void)actMD5Init((VSECPRIM_P2VAR_PARA(actMD5STRUCT)) workSpace->wsMD5);

  /* Set workSpace state */
  workSpace->header.status = (ESL_WST_ALGO_MD5 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

  return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 * esl_updateMD5
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateMD5(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceMD5) workSpace, const eslt_Length inputSize, VSECPRIM_P2CONST_PARA(eslt_Byte) input)
{
  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (input == NULL_PTR))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* Check workSpace */
  if ((workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_MD5)
  {
    return ESL_ERC_WS_STATE_INVALID;
  }
  if ((workSpace->header.status & ESL_WST_M_RUNNING) == 0u)
  {
    return ESL_ERC_WS_STATE_INVALID;
  }
  if ((inputSize % ESL_SIZEOF_MD5_BLOCK) == 0u)
  {
    if (workSpace->header.size < ESL_MINSIZEOF_WS_MD5)
    {
      return ESL_ERC_WS_TOO_SMALL;
    }
  }
  else
  {
    if (workSpace->header.size < ESL_MAXSIZEOF_WS_MD5)
    {
      return ESL_ERC_WS_TOO_SMALL;
    }
  }

  /* Update actCLib MD5; actMD5Update always returns actOK */
  (void)actMD5Update((VSECPRIM_P2VAR_PARA(actMD5STRUCT)) workSpace->wsMD5, input, inputSize, workSpace->header.watchdog);

  return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 * esl_finalizeMD5
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeMD5(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceMD5) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageDigest)
{
  /* Any NULLs ? */
  if ((workSpace == NULL_PTR) || (messageDigest == NULL_PTR))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* Check workSpace */
  if ((workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_MD5)
  {
    return ESL_ERC_WS_STATE_INVALID;
  }
  if ((workSpace->header.status & ESL_WST_M_RUNNING) == 0u)
  {
    return ESL_ERC_WS_STATE_INVALID;
  }
  if (workSpace->header.size < ESL_MAXSIZEOF_WS_MD5)
  {
    return ESL_ERC_WS_TOO_SMALL;
  }

  /* Finalize actCLib MD5; actMD5Finalize always returns actOK */
  (void)actMD5Finalize((VSECPRIM_P2VAR_PARA(actMD5STRUCT)) workSpace->wsMD5, messageDigest, workSpace->header.watchdog);

  /* Reset and clear workspace */
  esl_ResetAndClearWorkspace(&workSpace->header, workSpace->wsMD5); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

  return ESL_ERC_NO_ERROR;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_MD5_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_MD5.c
 *********************************************************************************************************************/
