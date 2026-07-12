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
/*!        \file  actBigNumGCD.c
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

#define ACTBIGNUMGCD_SOURCE

#include "actBigNum.h"
#include "actBigNumGCD.h"

#if (VSECPRIM_ACTBNEUCLID_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/* PRQA S 5209 EOF */ /* MD_VSECPRIM_USE_OF_BASIC_TYPES */

/****************************************************************************
 ** Types
 ***************************************************************************/

/* BigInt type */
typedef struct
{
   int sign;   /* 1 for pos, -1 for neg, 0 for zero */
   VSECPRIM_P2VAR_PARA(actBNDIGIT) value;
} actBIGINT;

/****************************************************************************
 ** Makros
 ***************************************************************************/

# define actBIIsEven(x)                                               (actBNIsEven((x).value))

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actBIDiv2()
 *********************************************************************************************************************/
/*! \brief          Division by 2 (a /= 2 ).
 *  \details        -
 *  \param[in,out]  a            (in) number to be divided                   length
 *                               (out) the result a / 2                      m_length
 *  \param[in]      length       length of a in digits
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actBIDiv2(VSECPRIM_P2VAR_PARA(actBIGINT) a, actLengthType length);

/**********************************************************************************************************************
 *  actBIAdd()
 *********************************************************************************************************************/
/*! \brief          Integer Addition (c = a+b).
 *  \details        -
 *  \param[in,out]  a            first number                                  length
 *  \param[in,out]  b            second number                                 length
 *  \param[in,out]  c            (in) number to store the result               length
 *                               (out) the result a + b                        length
 *  \param[in]      length       length of a and b in digits
 *  \pre            length of a+b is equal to the length of a and b (a+b does not
 *                  produce a carry)
 *  \note           c may overlap with a or b (a+=b or b+=a is possible)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actBIAdd(
   VSECPRIM_P2VAR_PARA(actBIGINT) a, VSECPRIM_P2VAR_PARA(actBIGINT) b,
   VSECPRIM_P2VAR_PARA(actBIGINT) c, actLengthType length);

/**********************************************************************************************************************
 *  actBISub()
 *********************************************************************************************************************/
/*! \brief          Integer Subtraction (c = a-b).
 *  \details        -
 *  \param[in,out]  a            first number                                  length
 *  \param[in,out]  b            second number                                 length
 *  \param[in,out]  c            (in) number to store the result               length
 *                               (out) the result a - b                        length
 *  \param[in]      length       length of a and b in digits
 *  \pre            length of a-b is equal to the length of a and b (a-b does not
 *                  produce a carry)
 *  \note           c may overlap with a or b (a-=b or b-=a is possible)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actBISub(
   VSECPRIM_P2VAR_PARA(actBIGINT) a, VSECPRIM_P2VAR_PARA(actBIGINT) b,
   VSECPRIM_P2VAR_PARA(actBIGINT) c, actLengthType length);




/****************************************************************************
 ** Local Functions
 ***************************************************************************/

/**********************************************************************************************************************
 *  actBIDiv2()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actBIDiv2(VSECPRIM_P2VAR_PARA(actBIGINT) a, actLengthType length)
{
  /* shift a to the right by 1 bit and check sign */
  actBNDiv2(a->value, 0, length);
  if (actBNIsZero(a->value, length) == TRUE)
  {
    a->sign = 0;
  }
}

/**********************************************************************************************************************
 *  actBIAdd()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actBIAdd(
   VSECPRIM_P2VAR_PARA(actBIGINT) a, VSECPRIM_P2VAR_PARA(actBIGINT) b,
   VSECPRIM_P2VAR_PARA(actBIGINT) c, actLengthType length)
{
  if (a->sign == b->sign)
  {
    /* equal signs of a and b */
    (void)actBNAdd(a->value, b->value, c->value, length); /* |c| = |a|+|b| */
    c->sign = a->sign;
  }
  else
  {
    /* unequal signs of a and b */
    if (actBNCompare(a->value, b->value, length) >= actCOMPARE_EQUAL)
    {
      /* case |a|>=|b| */
      actBNSub(a->value, b->value, c->value, length); /* |c| = |a|-|b| */
      if (actBNIsZero(c->value, length) == TRUE)
      {
        c->sign = 0;            /* |c| = 0 */
      }
      else
      {
        c->sign = a->sign;
      }
    }
    else
    {
      /* case |a|<|b| */
      actBNSub(b->value, a->value, c->value, length); /* |c| = |b|-|a| */
      c->sign = b->sign;
    }
  }
}

/**********************************************************************************************************************
 *  actBISub()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actBISub(
   VSECPRIM_P2VAR_PARA(actBIGINT) a, VSECPRIM_P2VAR_PARA(actBIGINT) b,
   VSECPRIM_P2VAR_PARA(actBIGINT) c, actLengthType length)
{
  /* c = a + (-b) */
  b->sign = -(b->sign);
  actBIAdd(a, b, c, length);
  b->sign = -(b->sign);
}


/****************************************************************************
 ** Global Function
 ***************************************************************************/


/**********************************************************************************************************************
 *  actBNGCDix()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actBNGCDix(
   VSECPRIM_P2CONST_PARA(actBNDIGIT) a, VSECPRIM_P2VAR_PARA(actBNDIGIT) a_inv,
   VSECPRIM_P2CONST_PARA(actBNRING) Field, VSECPRIM_P2VAR_PARA(actBNDIGIT) t[])
{
  /*
  To invert g, we use the extended Euclidian algorithm.
  'actBNGCDix' calculates x, such that
  gcd(g, p) = 1 = x*g + y*p   (p prime)
  <=>      1 = x*g mod p
  <=>   g^-1 = x
  */

  actBIGINT x, B, D;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) u;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) v;
  actLengthType length = Field->m_length;
  actLengthType save_length = length + 1;
  int i;

  /*
     initialization
   */

  /* zeroize workspace variables */
  actBNSetZero(a_inv, save_length);
  for (i = 0; i < 4; ++i)
  {
    actBNSetZero(t[i], save_length);
  }
  /* assign workspace variables */
  x.value = t[0];
  B.value = t[1];
  D.value = a_inv;
  u = t[2];
  v = t[3];

  /*
     Binary extended gcd algorithm (see MOV 14.61, 14.64)
   */

  /* x = field prime */
  actBNCopy(x.value, Field->m, length);
  x.sign = 1;
  /* B = 0 */
  B.sign = 0;
  /* D = a_inv = 1 */
  D.value[0] = 1;
  D.sign = 1;
  /* u = x */
  actBNCopy(u, x.value, length);
  /* v = a, where a>0 */
  actBNCopy(v, a, length);

  while(actBNIsZero(u, length) == FALSE)  /* while u>0 */
  {
    /* u even ? */
    while(actBNIsEven(u))
    {
      actBNDiv2(u, 0, length);  /* u /= 2 */
      /* B even ? */
      if (actBIIsEven(B))
      {
        actBIDiv2(&B, save_length); /* B /= 2 */
      }
      else
      {
        actBISub(&B, &x, &B, save_length);  /* B -= x; */
        actBIDiv2(&B, save_length); /* B /= 2 */
      }
    }

    /* v even ? */
    while(actBNIsEven(v))
    {
      actBNDiv2(v, 0, length);  /* v /= 2 */
      /* D even ? */
      if (actBIIsEven(D))
      {
        actBIDiv2(&D, save_length); /* D /= 2 */
      }
      else
      {
        actBISub(&D, &x, &D, save_length);  /* D -= x; */
        actBIDiv2(&D, save_length); /* D /= 2 */
      }
    }

    /* u >= v ? */
    if (actBNCompare(u, v, length) >= actCOMPARE_EQUAL)
    {
      actBNSub(u, v, u, length);  /* u -= v; */
      actBISub(&B, &D, &B, save_length);  /* B -= D; */
    }
    else
    {
      actBNSub(v, u, v, length);  /* v -= u; */
      actBISub(&D, &B, &D, save_length);  /* D -= B; */
    }
  }

  /* while D < 0, do D += field prime */
  while(D.sign < 0)
  {
    actBIAdd(&D, &x, &D, save_length);
  }
  /* while D >= field prime, do D -= field prime */
  while(actBNCompare(D.value, x.value, save_length) >= actCOMPARE_EQUAL)
  {
    actBNSub(D.value, x.value, D.value, save_length);
  }
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (ACTBIGNUMGCD == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actBigNumGCD.c
 *********************************************************************************************************************/
