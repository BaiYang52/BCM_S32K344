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
/*!        \file  actBNMult.c
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

#define ACTBNMULT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actBigNum.h"
#include "actUtilities.h"
#include "actWatchdog.h"

#if (VSECPRIM_ACTBNMULT_ENABLED == STD_ON)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actBNMult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actBNMult(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  const actLengthType a_length,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  const actLengthType b_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) c,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actLengthType i, j, c_length = a_length + b_length;
  actBNDDIGIT tmp;
  actBNDIGIT carry;

  actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  actBNSetZero(c, c_length); /* c = 0 */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* School method, start with the least significant digit (little endian array order) */
  for (i = 0u; i < b_length; ++i)
  {
    carry = 0u;
    for (j = 0u; j < a_length; ++j)
    {
      tmp = (((actBNDDIGIT) a[j]) * b[i]) + c[(i + j)] + carry;
      c[(i + j)] = actBNLoWord(tmp); /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
      carry = actBNHiWord(tmp);
    }
    c[(i + a_length)] = carry; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
  }
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTBNMULT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actBNMult.c
 *********************************************************************************************************************/
