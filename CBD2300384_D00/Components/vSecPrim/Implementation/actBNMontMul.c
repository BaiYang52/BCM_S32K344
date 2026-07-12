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
/*!        \file  actBNMontMul.c
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

#define ACTBNMONTMUL_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actBigNum.h"
#include "actUtilities.h"
#include "actWatchdog.h"

#if (VSECPRIM_ACTBNMONTMUL_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actBNMontMul()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actBNMontMul(
   VSECPRIM_P2CONST_PARA(actBNDIGIT) x, VSECPRIM_P2CONST_PARA(actBNDIGIT) y,
   VSECPRIM_P2VAR_PARA(actBNDIGIT) A, VSECPRIM_P2CONST_PARA(actBNRING) Ring,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2CONST_PARA(actBNDIGIT) m = Ring->m;
  actLengthType n = Ring->m_length;
  actBNDIGIT m_bar = Ring->m_bar;

  actBNDIGIT u_i, x_i;
  actBNDDIGIT tmp1, tmp2;
  actLengthType i, j;

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* Step 1: A = 0 */
  actBNSetZero(A, n + 1u); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

  /* Step 2: iterate over all digits in x, starting with the least significant digit */
  for (i = 0u; i < n; i++)
  {
    x_i = x[i];

    /* We need to compute two values: A_0 + x_i*y_0 (part of Step 2.1) and A_0 + x_i*y (part of Step 2.2).
    Therefore, tmp1 = A_0 + x_i*y_0 is computed once for Step 2.1 (here, stored in u_i) and reused
    in Step 2.2 (later). */
    tmp1 = ( ((actBNDDIGIT)y[0]) * x_i ) + A[0];
    /* store result in A[0], discard a possible carry */
    A[0] = actBNLoWord(tmp1); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */

    u_i = A[0] * m_bar;

    /* Similar to tmp1: One digit of the value A + x_i*y + u_i*m is already computed here and added as carry later */
    /* tmp2 = m_0 * u_i + x_i * y_0 + A_1 */
    tmp2 = ( ((actBNDDIGIT)m[0]) * u_i ) + A[0];

    /* Step 2.2: multiplication loop to compute x_i*y, i.e., y (array of digits) is multiplied by x_i (single digit) */
    for (j = 1u; j < n; j++)
    {
      /* Step 2.2.1: A += x_i*y (scalar multiplication) */
      /* tmp = y[j] * x_i + A[j] + carry */
      tmp1 = ( ((actBNDDIGIT)(y[j])) * x_i ) + A[j] + actBNHiWord(tmp1);

      /* Step 2.2.2: A = (A + u_i*m) / b   (scalar multiplication with 1-digit-right-shift) */
      /* tmp = m[j] * u_i + A[j] + carry; */
      tmp2 = ( ((actBNDDIGIT)(m[j])) * u_i ) + actBNLoWord(tmp1) + actBNHiWord(tmp2);
      /* equivalent to shifting tmp by one digit to the right, i.e., to dividing by 2^(bits per digit) */
      A[j-1u] = actBNLoWord(tmp2); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_VARIABLE_INDEX_CHECK */

      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
    }

    /* collect the carries and store them in the two most significant digits of A */
    tmp1 = ((actBNDDIGIT)A[n]) + actBNHiWord(tmp1) + actBNHiWord(tmp2);
    A[n - 1u] = actBNLoWord(tmp1); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
    A[n] = actBNHiWord(tmp1); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */

    actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }

  /* Step 3: if A >= m then A -= m */
  /* The comparison needs to be done "twice", because actBNCompare only compares numbers that have the same amount of digits,
     but A can have one more digit than m (in this case, the most significant digit of A is 1 and A > m) */
  if (A[n] == 1u)
  {
    /* A has one digit more than m, so A > m, do A -= m */
    actBNSub(A, m, A, n);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    A[n] = 0; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  }
  else
  {
    if (actBNCompare(A, m, n) >= actCOMPARE_EQUAL) /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    {
      actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
      actBNSub(A, m, A, n); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    }
  }

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
}

/**********************************************************************************************************************
 *  actBNMontMulCopy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(void) actBNMontMulCopy(
   VSECPRIM_P2VAR_PARA(actBNDIGIT) x, VSECPRIM_P2CONST_PARA(actBNDIGIT) y,
   VSECPRIM_P2VAR_PARA(actBNDIGIT) A, VSECPRIM_P2CONST_PARA(actBNRING) Ring,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actLengthType i;

  actBNMontMul(x, y, A, Ring, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */ /* Silent Justification: the sizes of x, y and A are passed as members of Ring */

  for (i = 0u; i < Ring->m_length; i++)
  {
    x[i] = A[i]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_VARIABLE_INDEX_CHECK */
  }
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTBNMONTMUL_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actBNMontMul.c
 *********************************************************************************************************************/
