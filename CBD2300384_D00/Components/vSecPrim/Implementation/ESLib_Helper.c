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
/*!        \file  ESLib_Helper.c
 *        \brief  ES Library internal helper functions
 *
 *      \details  Contains definitions of function declarations, which are internally used
 *                Common checks and activities (e.g. clean up work space data)
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
#define ESLIB_HELPER_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ESLib_Helper.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define VSECPRIM_START_SEC_CODE
#include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  esl_ResetAndClearWorkspace()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) esl_ResetAndClearWorkspace(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHeader) workSpaceHeader, VSECPRIM_P2VAR_PARA(void) workSpaceBuffer) /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
{
  /* Reset workspace status */
  workSpaceHeader->status = ESL_WST_ALLOCATED; /* SBSW_VSECPRIM_WRITE_POINTER */
#if (VSECPRIM_CLEAR_WORKSPACE_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  /* Clear workspace */
  actMemset(workSpaceBuffer, 0, workSpaceHeader->size); /* SBSW_VSECPRIM_CALL_FUNCTION */
#else
  VSECPRIM_DUMMY_STATEMENT(workSpaceBuffer); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
#endif
}

/**********************************************************************************************************************
 *  esl_SetWorkspaceStatus()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) esl_SetWorkspaceStatus(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHeader) workSpaceHeader, eslt_WSStatus algo)
{
    /* Set Workspace Status */
    workSpaceHeader->status = (algo | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL); /* SBSW_VSECPRIM_WRITE_POINTER */
}

/**********************************************************************************************************************
 *  esl_CheckWorkSpaceHeader()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_CheckWorkSpaceHeader(VSECPRIM_P2CONST_PARA(eslt_WorkSpaceHeader) workSpaceHeader, eslt_Length minSize)
{
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;

  /* Check workSpace size */
  if (workSpaceHeader->size < minSize)
  {
    retVal = ESL_ERC_WS_TOO_SMALL;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_CheckWorkSpaceState()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_CheckWorkSpaceState(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceHeader) workSpaceHeader,
  eslt_WSStatus algo)
{
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;

  if (((workSpaceHeader->status & ESL_WST_M_ALGO) != algo) || ((workSpaceHeader->status & ESL_WST_M_RUNNING) == 0u))
  {
    retVal = ESL_ERC_WS_STATE_INVALID;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_CheckWorkSpaceHeaderAndState()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_CheckWorkSpaceHeaderAndState(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceHeader) workSpaceHeader,
  eslt_Length minSize, eslt_WSStatus algo)
{
  eslt_ErrorCode retVal = esl_CheckWorkSpaceHeader(workSpaceHeader, minSize); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FUNCTION_CALL_POINTER_CHECKED_WITHIN */

  /* Check return value */
  if (retVal != ESL_ERC_NO_ERROR)
  {
    /* retVal was already set */
  }
  else
  {
    retVal = esl_CheckWorkSpaceState(workSpaceHeader, algo); /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_checkLengthRef()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_checkLengthRef(eslt_LengthConstRef lenRef, eslt_Length lengthValue)
{
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;

  if (lenRef == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if (*lenRef < lengthValue)
  {
    retVal = ESL_ERC_BUFFER_TOO_SMALL;
  }
  else
  {
    /* Nothing to do */
  }

  return retVal;
}

#define VSECPRIM_STOP_SEC_CODE
#include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_Helper.c
 *********************************************************************************************************************/
