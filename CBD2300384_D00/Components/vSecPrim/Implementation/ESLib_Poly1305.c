 /**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2017 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  ESLib_Poly1305.c
 *        \brief  Poly1305 implementation.
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

/****************************************************************************
 **
 ** Poly1305 interface implementation.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   esl_initPoly1305
 **   esl_updatePoly1305
 **   esl_finalizePoly1305
 **   esl_verifyPoly1305
 **
 ***************************************************************************/
#define ESLIB_POLY1305_SOURCE

#include "ESLib.h"
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIPoly1305.h"

#if (VSECPRIM_POLY1305_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/* PRQA S 2889 EOF */ /* MD_VSECPRIM_QM_ONLY_RETURN_PATHS */
/* PRQA S 0310, 3305 EOF */ /* MD_VSECPRIM_QM_ONLY_CASTING_POINTER_TYPE */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/****************************************************************************
 * esl_initPoly1305
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initPoly1305(VSECPRIM_P2VAR_PARA( eslt_WorkSpacePoly1305 ) workSpace,
                                               VSECPRIM_P2CONST_PARA( eslt_Byte ) key)
{
  /* validate parameters */
  if ((NULL == workSpace)
    || (NULL == key))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* check workSpace */
  if (ESL_SIZEOF_WS_Poly1305 > workSpace->header.size)
  {
    return ESL_ERC_WS_TOO_SMALL;
  }

  /* initialize actCLib Poly1305 */
  actPoly1305Init((VSECPRIM_P2VAR_PARA(actPoly1305Context)) workSpace->wsPoly1305, key);

  /* set workSpace state */
  workSpace->header.status = (ESL_WST_ALGO_Poly1305 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

  return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 * esl_updatePoly1305
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updatePoly1305(VSECPRIM_P2VAR_PARA( eslt_WorkSpacePoly1305 ) workSpace,
                                                 VSECPRIM_P2CONST_PARA( eslt_Byte ) message,
                                                 const eslt_Length msgSize)
{
  /* validate parameters */
  if ((NULL == workSpace)
    || (NULL == message))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* check workSpace state */
  if ((ESL_WST_ALGO_Poly1305 != (workSpace->header.status & ESL_WST_M_ALGO))
    || (ESL_WST_M_RUNNING != (workSpace->header.status & ESL_WST_M_RUNNING)))
  {
    return ESL_ERC_WS_STATE_INVALID;
  }

  /* update actCLib Poly1305 */
  actPoly1305Update((VSECPRIM_P2VAR_PARA( actPoly1305Context ))workSpace->wsPoly1305, message, msgSize, workSpace->header.watchdog);

  return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 * esl_finalizePoly1305
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizePoly1305 (VSECPRIM_P2VAR_PARA( eslt_WorkSpacePoly1305 ) workSpace,
                                                    VSECPRIM_P2VAR_PARA( eslt_Byte ) tag)
{
  /* validate parameters */
  if ((NULL == workSpace)
    || (NULL == tag))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* check workSpace state */
  if ((ESL_WST_ALGO_Poly1305 != (workSpace->header.status & ESL_WST_M_ALGO))
    || (ESL_WST_M_RUNNING != (workSpace->header.status & ESL_WST_M_RUNNING)))
  {
    return ESL_ERC_WS_STATE_INVALID;
  }

  /* calculate Poly1305 TAG */
  actPoly1305Finalize ((VSECPRIM_P2VAR_PARA( actPoly1305Context )) workSpace->wsPoly1305, tag, workSpace->header.watchdog);

  /* Reset and clear workspace */
  esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsPoly1305); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 * esl_verifyPoly1305
 ***************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyPoly1305 (VSECPRIM_P2VAR_PARA( eslt_WorkSpacePoly1305 ) workSpace,
                                                  VSECPRIM_P2CONST_PARA( eslt_Byte ) tag)
{
  eslt_Byte tmpTag [ESL_SIZEOF_Poly1305_TAG];

  /* validate parameters */
  if ((NULL == workSpace)
    || (NULL == tag))
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

  /* check workSpace state */
  if ((ESL_WST_ALGO_Poly1305 != (workSpace->header.status & ESL_WST_M_ALGO))
    || (ESL_WST_M_RUNNING != (workSpace->header.status & ESL_WST_M_RUNNING)))
  {
    return ESL_ERC_WS_STATE_INVALID;
  }

  /* calculate Poly1305 TAG */
  actPoly1305Finalize ((VSECPRIM_P2VAR_PARA( actPoly1305Context )) workSpace->wsPoly1305, tmpTag, workSpace->header.watchdog);

  /* compare TAG */
  if (TRUE != actMemcmp(tmpTag, tag, ESL_SIZEOF_Poly1305_TAG))  /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
  {
    return ESL_ERC_INCORRECT_TAG;
  }

  /* Reset and clear workspace */
  esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsPoly1305); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  return ESL_ERC_NO_ERROR;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_POLY1305_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_Poly1305.c
 *********************************************************************************************************************/
