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
/*!        \file  actBNReduce.c
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

#define ACTBNREDUCE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actBigNum.h"
#include "actWatchdog.h"

#if (VSECPRIM_ACTBNREDUCE_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actBNMulDigit()
 *********************************************************************************************************************/
/*! \brief       BigNum multiply digit (c = a*b).
 *  \details     This function multiplies the BigNum a by b, where b is a single-digit BigNum.
 *  \param[in]     a  First number
 *  \param[in]     a_length  Length of a in digits
 *  \param[in]     b  Second number
 *  \param[in,out] c  (in) Buffer to store the result
 *                    (out) result
 *  \return      the last carry digit
 *  \pre         provided pointers must be valid
 *               buffer c must provide a_length digits
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        a, b and r cannot overlap
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actBNDIGIT) actBNMulDigit(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  const actLengthType a_length,
  const actBNDIGIT b,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) c);

/**********************************************************************************************************************
 *  actBNNormalize()
 *********************************************************************************************************************/
/*! \brief       Normalization for actBNReduce function.
 *  \details     This function ensures that the most significant bit of the most significant digit of b
 *               is 1 by multiplying a and b with a suitable power of 2, which makes a future reduction
 *               a mod b more efficient. (This is because the reduction algorithm mostly uses the most
 *               significant digit of b, which is >= 2^(bits per digit - 1) after normalization, and any
 *               digit of a is <= 2^(bits per digit) - 1 (as usual).)
 *  \param[in,out]  a  (in) number to be shifted
 *                     (out) shifted number
 *  \param[in,out]  a_length  (in) Length of a in digits
 *                            (out) Length of shifted number
 *  \param[in,out]  b  (in) number to be shifted
 *                     (out) shifted number
 *  \param[in]      b_length  Length of b in digits
 *  \param[in]      shift number of bits to be shifted
 *  \pre         provided pointers must be valid
 *               a must provide a buffer of a_length+1 digits
 *               b must provide a buffer of b_length digits
 *               0 < shift < actBN_BITS_PER_DIGIT
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actBNNormalize(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  VSECPRIM_P2VAR_PARA(actLengthType) a_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) b,
  actLengthType b_length,
  actLengthType shift);

/**********************************************************************************************************************
 *  actBNReNormalize()
 *********************************************************************************************************************/
/*! \brief       Undo normalization for actBNReduce function.
 *  \details     This function undoes the normalization, introduced by actBNNormalize, in order for actBNReduce to
 *               return the correct remainder r = a mod b.
 *  \param[in,out]  r  (in) number to be shifted
 *                     (out) shifted number
 *  \param[in,out]  b  (in) number to be shifted
 *                     (out) shifted number
 *  \param[in]      b_length  Length of r and b in digits
 *  \param[in]      shift number of bits to be shifted
 *  \pre         provided pointers must be valid
 *               r must provide a buffer of b_length+1 digits
 *               b must provide a buffer of b_length digits
 *               0 < shift < actBN_BITS_PER_DIGIT
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actBNReNormalize(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) b,
  actLengthType b_length,
  actLengthType shift);

/**********************************************************************************************************************
 *  actBNReduceLengthCheck()
 *********************************************************************************************************************/
/*! \brief       Preparation for actBNReduce, which computes the remainder a mod b.
 *  \details     Preliminary length checks for the modulo reduction, such as checking for leading zeros, checking if
 *               b is too short and checking if a <= b holds already
 *  \param[in]      a  first operand in a mod b
 *  \param[in,out]  a_length  (in) original length of the buffer referenced by a
 *                            (out) length of bignum a without leading zeros
 *  \param[in]      b  second operand in a mod b
 *  \param[in,out]  b_length  (in) opriginal length of the buffer referenced by b
 *                            (out) length of bignum b without leading zeros
 *  \param[in,out]  r (in) pointer to buffer to store the result r = a mod b
 *                    (out) a, if a < b and if b_length > 1
 *  \return      actEXCEPTION_LENGTH if b_length <= 1
 *               actDONE - success; additionally a < b, so r = a and no further computations in actBNReduce are needed
 *               actOK - success; further computations in actBNReduce are needed
 *  \pre         provided pointers must be valid
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actBNReduceLengthCheck(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2VAR_PARA(actLengthType) a_length,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  VSECPRIM_P2VAR_PARA(actLengthType) b_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r);

/**********************************************************************************************************************
 *  actBNReduceInitialize()
 *********************************************************************************************************************/
/*! \brief       Preparation for the division process in actBNReduce, which computes r = a mod b
 *  \details     Preliminary calculations such as normalizing a and b if necessary, adapting the length of a and
 *               subtracting b from the b_length most significant digits of a if possible
 *  \param[in,out]  a (in) first operand in a mod b
 *                    (out) possible normalized operand a
 *  \param[in,out]  a_length  (in) length of the buffer referenced by a
 *                            (out) length of the buffer referenced by a after normalization
 *  \param[in,out]  b (in) second operand in a mod b
 *                    (out) possibly normalized operand b
 *  \param[in]      b_length  length of the buffer referenced by b
 *  \param[in,out]  shift (in) parameter to count the number of shifting steps required for normalization, initially 0
 *                        (out) number of shifting steps required for normalization
 *  \pre         provided pointers must be valid
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNReduceInitialize(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  VSECPRIM_P2VAR_PARA(actLengthType) a_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) b,
  VSECPRIM_P2CONST_PARA(actLengthType) b_length,
  VSECPRIM_P2VAR_PARA(actLengthType) shift);

/**********************************************************************************************************************
 *  actBNReduceComputeQuotientEstimate()
 *********************************************************************************************************************/
/*! \brief       Estimates the current digit qDig of the quotient q in a = q*b + remainder
 *  \details     Divides a by the current digit under consideration of b; the estimate is refined in a later step.
 *  \param[in]      a first operand in a mod b
 *  \param[in]      b second operand in a mod b
 *  \param[in]      idx  index of the current digit under consideration of a
 *  \param[in]      b_length  length of the buffer referenced by b
 *  \param[in,out]  qDig (in)  actBNDIGIT to store the estimate of the quotient: a = qDig*b + remainder
 *                       (out) estimated quotient
 *  \pre         provided pointers must be valid
 *               the buffer referenced by a must have b_length+1 elements
 *               the buffer referenced by b must have b_length elements
 *               the buffer referenced by qDig must have 1 element
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNReduceComputeQuotientEstimate(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  actLengthType idx,
  actLengthType b_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) qDig);

/**********************************************************************************************************************
 *  actBNReduceRefineQuotientEstimate()
 *********************************************************************************************************************/
/*! \brief       Refines the first, rough estimate of the current digit qDig of the quotient q in a = q*b + remainder
 *  \details     The first estimate only took a single digit of b into account and may thus be too large.
 *               This function checks whether the product of the estimated quotient times the two consecutive digits
 *               of b are larger than three digits of a, starting from the position referenced by the index idx.
 *               If this is the case, the estimated quotient is reduced.
 *  \param[in]      a first operand in a mod b
 *  \param[in]      idx  index of the current digit under consideration of a
 *  \param[in]      b second operand in a mod b
 *  \param[in]      b_length  length of the buffer referenced by b
 *  \param[in,out]  qDig (in)  estimated quotient: a = qDig*b + remainder, 1 actBNDIGIT
 *                       (out) refined estimated quotient
 *  \param[in]      watchdog       watchdog function pointer
 *  \pre         provided pointers must be valid
 *               the buffer referenced by a must have b_length+1 elements
 *               the buffer referenced by b must have b_length elements
 *               the buffer referenced by qDig must have 1 element
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNReduceRefineQuotientEstimate(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  actLengthType idx,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  actLengthType b_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) qDig,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actBNReduceUpdateCurrentProduct()
 *********************************************************************************************************************/
/*! \brief       Computes the product qDig * b.
 *  \details     After the estimate of qDig has been refined, this function computes r = qDig*b and checks if a > r.
 *               In this case, r is updated to r = r - b (which corresponds to reducing qDig by 1 and then
 *               computing qDig * b.
 *  \param[in]      a first operand in a mod b
 *  \param[in]      b second operand in a mod b
 *  \param[in]      idx  index of the current digit under consideration of a
 *  \param[in]      b_length  length of the buffer referenced by b
 *  \param[in]      qDig (in)  digit of quotient q (a = q*b + remainder) that is currently under consideration
 *                       (out) digit of estimated quotient, possibly reduced by 1
 *  \param[in,out]  r  (in)  buffer to store the product qDig*b, where qDig has reached its final value
 *                     (out) qDig*b
 *  \param[in]      watchdog       watchdog function pointer
 *  \pre         provided pointers must be valid
 *               the buffer referenced by a must have b_length+1 elements
 *               the buffer referenced by b must have b_length elements
 *               the buffer referenced by qDig must have 1 element
 *               the buffer referenced by r must have b_length+1 elements
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNReduceUpdateCurrentProduct(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  actLengthType idx,
  actLengthType b_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) qDig,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  actBNMultDigit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
  VSECPRIM_LOCAL_FUNC(actBNDIGIT) actBNMulDigit(
    VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
    const actLengthType a_length,
    const actBNDIGIT b,
    VSECPRIM_P2VAR_PARA(actBNDIGIT) c)
{
  actLengthType i;
  actBNDDIGIT tmp = 0;

  for (i = 0u; i < a_length; ++i)
  {
    /* actBNHiWord(tmp) is the carry of the last iteration */
    tmp = ((actBNDDIGIT) a[i] * b) + actBNHiWord(tmp);
    c[i] = actBNLoWord(tmp); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  }
  return actBNHiWord(tmp);
}

/**********************************************************************************************************************
 *  actBNNormalize()
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
 */
VSECPRIM_LOCAL_FUNC(void) actBNNormalize(
   VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  VSECPRIM_P2VAR_PARA(actLengthType) a_length,
   VSECPRIM_P2VAR_PARA(actBNDIGIT) b,
  actLengthType b_length,
  actLengthType shift)
{
  /* tmp = 2^shift */
  actBNDIGIT tmp = (actBNDIGIT) (((actBNDIGIT) 1u) << shift);

  /* a *= tmp */
  actBNDIGIT carry = actBNMulDigit(a, *a_length, tmp, a); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  if (carry != 0u)
  {
    a[*a_length] = carry; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
    ++(*a_length); /* SBSW_VSECPRIM_WRITE_POINTER */
  }

  /* b *= tmp */
  (void)actBNMulDigit(b, b_length, tmp, b); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
}

/**********************************************************************************************************************
 *  actBNReNormalize()
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
 */
VSECPRIM_LOCAL_FUNC(void) actBNReNormalize(
   VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
   VSECPRIM_P2VAR_PARA(actBNDIGIT) b,
  actLengthType b_length,
  actLengthType shift)
{
  /* tmp = 2^(bits_per_digit-shift) */
  actBNDIGIT tmp = (actBNDIGIT)(((actBNDIGIT)1u) << (actBN_BITS_PER_DIGIT - shift));

  /* r *= tmp, the multiplication carry is stored in r[b_length] */
  r[b_length] = actBNMulDigit(r, b_length, tmp, r); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  /* shift r one digit to the left */
  actBNCopy(r, &r[1u], b_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

  /* b *= tmp, write last carry to tmp */
  tmp = actBNMulDigit(b, b_length, tmp, b); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
 /* shift b one digit to the left */
  actBNCopy(b, &b[1], b_length - 1u);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  /* store carry in most significant digit of b */
  b[b_length - 1u] = tmp; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
}

/**********************************************************************************************************************
 *  actBNReduceLengthCheck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actBNReduceLengthCheck(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2VAR_PARA(actLengthType) a_length,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  VSECPRIM_P2VAR_PARA(actLengthType) b_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r)
{
  actRETURNCODE retVal = actOK;

  /* adjusting of a_length and b_length in case a or b have leading zeros */
  while ((*a_length > 1u) && (a[*a_length - 1u] == 0u))
  {
    --(*a_length); /* SBSW_VSECPRIM_WRITE_POINTER */
  }

  while ((*b_length > 1u) && (b[*b_length - 1u] == 0u))
  {
    --(*b_length); /* SBSW_VSECPRIM_WRITE_POINTER */
  }

  if (*b_length <= 1u)
  {
    /* error: b too short */
    retVal = actEXCEPTION_LENGTH;
  }
  /* If b is not too short, check if a is already less than b and if yes, copy a to r */
  else
  {
    if (*a_length == *b_length)
    {
      if (actBNCompare(a, b, *a_length) == actCOMPARE_SMALLER) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
      {
        actBNCopy(r, a, *a_length);  /* r = a */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        retVal = actDONE;             /* done  */
      }
    }
    if (*a_length < *b_length)
    {
      actBNCopy(r, a, *a_length);  /* r = a */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      retVal = actDONE;             /* done  */
    }
  }

  return retVal;

}

/**********************************************************************************************************************
 *  actBNReduceInitialize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actBNReduceInitialize(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  VSECPRIM_P2VAR_PARA(actLengthType) a_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) b,
  VSECPRIM_P2CONST_PARA(actLengthType) b_length,
  VSECPRIM_P2VAR_PARA(actLengthType) shift)
{
  /* Normalization; guarantees that the most significant bit in b[b_len-1] is 1
     efficiency reasons: this limits the amount of loops executed later */

  actBNDIGIT tmp = b[*b_length - 1u];
  while (tmp < actBN_MAX_BIT) /* actBN_MAX_BIT = 2^31 */
  {
    tmp <<= 1;
    *shift = *shift + 1u; /* SBSW_VSECPRIM_WRITE_POINTER */
  }

  if (*shift > 0u)
  {
    /* *(a *= 2^shift), *(b *= 2^shift) */
    actBNNormalize(a, a_length, b, *b_length, *shift); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  }

  /* Prepare division process by subtracting b from the b_length most significant digits of a
     start from subaddress, as the array referenced by a is longer than the one referenced by b
     Note: In MOV'97, this step consists of a while-loop with possibly several iterations. However,
     due to the normalization procedure, at most one iteration is performed. */
  if (actBNCompare(&a[*a_length - *b_length], b, *b_length) >= actCOMPARE_EQUAL) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    actBNSub(&a[*a_length - *b_length], b, &a[*a_length - *b_length], *b_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS_FIXED_SIZE */
  }
}

/**********************************************************************************************************************
 *  actBNReduceComputeQuotientEstimate()
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
 */
VSECPRIM_FUNC(void) actBNReduceComputeQuotientEstimate(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  actLengthType idx,
  actLengthType b_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) qDig)
{
  actBNDDIGIT tmp;

  if (a[idx] == b[b_length - 1u])
  {
    qDig[0] = 0; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  }
  else
  {
    tmp = ((((actBNDDIGIT)a[idx]) << actBN_BITS_PER_DIGIT) + a[idx - 1u]) / b[b_length - 1u];
    /* the high word of tmp is zero because b[b_length - 1] >= 2^31 due to the normalization process */
    qDig[0] = (actBNDIGIT)(((actBNDIGIT)tmp) + 1u); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  }
}

/**********************************************************************************************************************
 *  actBNReduceRefineQuotientEstimate()
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
 */
VSECPRIM_FUNC(void) actBNReduceRefineQuotientEstimate(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  actLengthType idx,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  actLengthType b_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) qDig,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actBNDDIGIT tmp;
  actBNDIGIT digits[3];

  do
  {
    --(*qDig);  /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
    tmp = ((actBNDDIGIT)(*qDig)) * b[b_length - 2u];
    digits[0] = actBNLoWord(tmp); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
    tmp = (((actBNDDIGIT)(*qDig)) * b[b_length - 1u]) + actBNHiWord(tmp);
    digits[1] = actBNLoWord(tmp); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
    digits[2] = actBNHiWord(tmp); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */

    actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  } while (actBNCompare(&a[idx - 2u], digits, 3) == actCOMPARE_SMALLER); /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actBNReduceUpdateCurrentProduct()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actBNReduceUpdateCurrentProduct(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  actLengthType idx, actLengthType b_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) qDig,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  r[b_length] = actBNMulDigit(b, b_length, *qDig, r); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* In [MOV'97]: compute a - qDig*(base^idx)*b; the multiplication by (base^idx) is here replaced by the subadress of a,
     at which the comparison of a and r is started */
  if (actBNCompare(&a[idx - b_length], r, b_length + 1u) == actCOMPARE_SMALLER) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    --(*qDig);  /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
    r[b_length] = actBNMulDigit(b, b_length, *qDig, r); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }
  actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  actBNReduce()
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
 */
VSECPRIM_FUNC(actRETURNCODE) actBNReduce(
   VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  actLengthType a_length,
   VSECPRIM_P2VAR_PARA(actBNDIGIT) b,
  actLengthType b_length,
   VSECPRIM_P2VAR_PARA(actBNDIGIT) r, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal = actEXCEPTION_UNKNOWN; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  actLengthType a_len = a_length;
  actLengthType b_len = b_length;

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* init r, correct length of a and b */
  actBNSetZero(r, b_len); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  retVal = actBNReduceLengthCheck(a, &a_len, b, &b_len, r); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  if (retVal == actOK)
  {
    /* i: loop variable
       n: length of a - 1
       t: lenght of b - 1, t <= n
       shift: required for normalization */
    actLengthType i, n, t, shift;

    /* qDig: quotient, a = qDig*b + r, r = a mod b
       Digits: used to calculate an estimate of the product qDig*b */
    actBNDIGIT qDig;

    shift = 0;

    /* preparation of a, a_len and b: a and b are normalized if necessary; b is subtracted from a if possible */
    actBNReduceInitialize(a, &a_len, b, &b_len, &shift); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */ /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */

    /* init n, t; due to normalization process, a_len can be different from the original input parameter */
    n = a_len - 1u;
    t = b_len - 1u;

    /* [step 3 in reference] main division loop, start with most significant digits */
    for (i = n; i > t; --i)
    {
      actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

      actBNReduceComputeQuotientEstimate(a, b, i, b_len, &qDig);  /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */
      actBNReduceRefineQuotientEstimate(a, i, b, b_len, &qDig, watchdog); /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */
      actBNReduceUpdateCurrentProduct(a, b, i, b_len, &qDig, r, watchdog); /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */
      actBNSub(&a[i - t - 1u], r, &a[i - t - 1u], b_len + 1u); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS_FIXED_SIZE */
    }

    actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

    /* [step 4] copy a to r */
    actBNCopy(r, a, b_len); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

    actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

    /* renormalize r and b: divide the remainder r and b by 2^shift */
    if (shift > 0u)
    {
      actBNReNormalize(r, b, b_len, shift); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    }
  }

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  if (retVal == actDONE)
  {
    retVal = actOK;
  }

  return retVal;
} /* PRQA S 6060, 6050 */ /* MD_VSECPRIM_STPAR, MD_MSR_STCAL */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTBNREDUCE_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actBNReduce.c
 *********************************************************************************************************************/
