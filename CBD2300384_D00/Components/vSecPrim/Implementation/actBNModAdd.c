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
/*!        \file  actBNModAdd.c
 *        \brief  A basic (unsigned) integer and module arithmetic used for elliptic curve point arithmetic.
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library actCLib
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

#define ACTBNMODADD_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actBigNum.h"
#include "actUtilities.h"
#include "actWatchdog.h"

#if (VSECPRIM_ACTBNMODADD_ENABLED == STD_ON)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actBNModAdd()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actBNModAdd(
   VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
   VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
   VSECPRIM_P2VAR_PARA(actBNDIGIT) c,
   VSECPRIM_P2CONST_PARA(actBNRING) Ring,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actLengthType length = Ring->m_length;  /* modulus length */

  actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* Calculate c = a + b and subtract m, if c > m */
  if (actBNAdd(a, b, c, length) == 1u) /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* We have an add_carry, so c > m, do c -= m */
    actBNSub(c, Ring->m, c, length); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }
  else
  {
    /* there is no carry from adding a and b, still check if a+b > m */
    if (actBNCompare(c, Ring->m, length) == actCOMPARE_LARGER) /* SBSW_VSECPRIM_CALL_FUNCTION */
    {
      /* If c > m, compute c -= m */
      actBNSub(c, Ring->m, c, length); /* SBSW_VSECPRIM_CALL_FUNCTION */
    }
  }
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTBNMODADD_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actBNModAdd.c
 *********************************************************************************************************************/
