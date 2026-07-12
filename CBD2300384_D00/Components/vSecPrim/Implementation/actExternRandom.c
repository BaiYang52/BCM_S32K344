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
/*!        \file  actExternRandom.c
 *        \brief  Callout to random generate function implementation.
 *
 *      \details This file is part of the embedded systems library cvActLib/ES
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

#define ACTEXTERNRANDOM_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actConfig.h"
#include "actExternRandom.h"
#include "ESLib_ERC.h"
#include "ESLib_RNG.h"

#if (VSECPRIM_ACTEXTERNRANDOM_ENABLED == STD_ON)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actGetExternRandom()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actGetExternRandom(VSECPRIM_P2VAR_PARA(actU8) output, actLengthType length)
{
  /* Call dedicated random function */
  actU8 retVal = actEXCEPTION_UNKNOWN;

  if (esl_getBytesRNG(((actU16)length), output) == ESL_ERC_NO_ERROR) /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    retVal = actOK;
  }

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTEXTERNRANDOM_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: ACTEXTERNRANDOM.c
 *********************************************************************************************************************/
