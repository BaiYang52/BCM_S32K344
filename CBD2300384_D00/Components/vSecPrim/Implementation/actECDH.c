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
/*!        \file  actECDH.c
 *        \brief  Implementation file for actECDH.h
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

#define ACTECDH_SOURCE

/****************************************************************************
 **
 ** This file contains: Implementation file for actECDH.h
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   actECDHp
 **
 ***************************************************************************/

#include "actECDH.h"
#include "actECLengthInfo.h"

#if (VSECPRIM_ACTECDH_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/* PRQA S 2889 EOF */ /* MD_VSECPRIM_QM_ONLY_RETURN_PATHS */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/**********************************************************************************************************************
 *  actECDHp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actECDHp(
   VSECPRIM_P2CONST_PARA(actBNDIGIT) d, VSECPRIM_P2CONST_PARA(actECPOINT) Q,
   VSECPRIM_P2VAR_PARA(actECPOINT) point, VSECPRIM_P2VAR_PARA(actECCURVE) Curve,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   /* Convert Q to Montgomery Representation */
   actECPToMont(Q, Curve, watchdog);

   /* check if Q is on curve Q before continue */
   if (actECPointIsOnCurve(Q, Curve, watchdog) != actOK)
   {
      return actEXCEPTION_PUBKEY;
   }

   /* Compute actPoint = d*Q */
   actECPMult(Q, d, point, Curve, watchdog);
   /* Transform actPoint to affine */
   if (actECPToAffineFromMont(point, Curve, 0, watchdog) != actOK) { return actEXCEPTION_UNKNOWN; }

   return actOK;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTECDH_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actECDH.c
 *********************************************************************************************************************/
