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
/*!        \file  actECKey.c
 *        \brief  Implementation file for actECKey.h.
 *
 *      \details This file is part of the embedded systems library cvActLib
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

#define ACTECKEY_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actECKey.h"
#include "actECLengthInfo.h"

#if (VSECPRIM_ACTECKEY_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actECkgp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actECkgp(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) d,
  VSECPRIM_P2VAR_PARA(actECPOINT) Q,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp_ptr,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actECPOINT Prec;
  actRETURNCODE retVal = actOK;

  /* assign workspace for Prec (precomputed point, p_length in each coordinate) */
  Prec.x = wksp_ptr->curvePoint.precomputedPoint.xcoord;
  Prec.y = wksp_ptr->curvePoint.precomputedPoint.ycoord;

  /* Compute Q = d*G */
  actECPMultG(d, &Prec, Q, &wksp_ptr->baseData.Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  /* Transform to affine */
  if (actECPToAffineFromMont(Q, &wksp_ptr->baseData.Curve, FALSE, watchdog) != actOK) /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  {
    retVal = actEXCEPTION_UNKNOWN;
  }

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTECKEY_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actECKey.c
 *********************************************************************************************************************/
