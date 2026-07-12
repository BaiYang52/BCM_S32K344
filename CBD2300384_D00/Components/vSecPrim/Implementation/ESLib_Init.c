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
/*!        \file  ESLib_Init.c
 *        \brief  Initialization functions implementation.
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

#define ESLIB_INIT_SOURCE

#include "ESLib.h"
#include "ESLib_types.h"

#define VSECPRIM_START_SEC_CODE
#include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/**********************************************************************************************************************
 *  esl_initESCryptoLib()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initESCryptoLib(void)
{
  return ESL_ERC_NO_ERROR;
}

/**********************************************************************************************************************
 *  esl_initWorkSpaceHeader()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initWorkSpaceHeader(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHeader) workSpaceHeader,
  const eslt_Length workSpaceSize,
  esl_WatchdogFuncPtr watchdog)
{
  eslt_ErrorCode retVal;

  /* Any NULLs ? */
  if (workSpaceHeader == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    /* init workSpaceHeader */
    workSpaceHeader->size = workSpaceSize; /* SBSW_VSECPRIM_WRITE_POINTER */
    workSpaceHeader->status = ESL_WST_ALLOCATED; /* SBSW_VSECPRIM_WRITE_POINTER */
    workSpaceHeader->watchdog = watchdog; /* SBSW_VSECPRIM_WRITE_POINTER */

    workSpaceHeader->checkSum = 0u;  /* not implemented at this point */ /* SBSW_VSECPRIM_WRITE_POINTER */

    retVal = ESL_ERC_NO_ERROR;
  }

  return retVal;
}

#define VSECPRIM_STOP_SEC_CODE
#include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_Init.c
 *********************************************************************************************************************/
