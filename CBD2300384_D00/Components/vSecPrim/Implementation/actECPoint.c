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
/*!        \file  actECPoint.c
 *        \brief  Implementation file for actECPoint.h
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

#define ACTECPOINT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actECPoint.h"

#if (VSECPRIM_ACTECPOINT_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 actECPCopy()
 **********************************************************************************************************************/
/*! \brief      This macro copies Q2 into Q1
 *  \details    -
 *  \param[in,out]  Q1        (in)   destination point
 *                            (out)  Q1 equal to Q2
 *  \param[in]      Q2        source point
 *  \param[in,out]  Curve     (in) underlying curve
 *                            (out) underlying curve, values at Curve->t[0] modified
 *  \param[in]      watchdog  Watchdog trigger function pointer
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        Q1 and Q2 must be valid pointers.
 *              The function assumes that Q1 is projective, i.e., not affine.
 *              Each of th three buffers referenced by Q1 must provide at least p_length actBNDIGITs
 *              (p_length is the length of the prime p, the curve is defined over the prime field p).
 *              The buffer referenced by Curve->t[0] must provide at least p_length+1 actBNDIGITs.
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actECPCopy(
  VSECPRIM_P2VAR_PARA(actECPOINT) Q1,
  VSECPRIM_P2CONST_PARA(actECPOINT) Q2,
  VSECPRIM_P2CONST_PARA(actECCURVE) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  actECPCopy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actECPCopy(
  VSECPRIM_P2VAR_PARA(actECPOINT) Q1,
  VSECPRIM_P2CONST_PARA(actECPOINT) Q2,
  VSECPRIM_P2CONST_PARA(actECCURVE) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actLengthType p_length = Curve->p_field.m_length;

  actBNCopy(Q1->x, Q2->x, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  actBNCopy(Q1->y, Q2->y, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  if ((Q2->is_affine) == TRUE)
  {
    /* Q1->z = 1 */
    actBNSetOne(Q1->z, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    /* Q1->z = R */
    actBNMontMulCopy(Q1->z, Curve->p_field.RR, Curve->t[0], &Curve->p_field, watchdog);   /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }
  else
  {
    actBNCopy(Q1->z, Q2->z, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  }
  Q1->is_infinity = Q2->is_infinity; /* SBSW_VSECPRIM_WRITE_POINTER */
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  actECPAssign()
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
VSECPRIM_FUNC( void ) actECPAssign(
  VSECPRIM_P2VAR_PARA( actECPOINT ) Q1,
  VSECPRIM_P2CONST_PARA( actECPOINT ) Q2,
  VSECPRIM_P2CONST_PARA( actECCURVE ) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2CONST_PARA(actBNRING) p = &Curve->p_field;

    Q1->is_affine   = FALSE; /* SBSW_VSECPRIM_WRITE_POINTER */
    Q1->is_infinity = Q2->is_infinity; /* SBSW_VSECPRIM_WRITE_POINTER */

    actBNCopy (Q1->x, Q2->x, p->m_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    actBNCopy (Q1->y, Q2->y, p->m_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

   if ( (Q2->is_affine) == TRUE )
   {
      /* Q1->Z = 1 */
      actBNSetOne       (Q1->z, p->m_length);   /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      /* Q1->Z = R */
      actBNMontMulCopy  (Q1->z, p->RR, Curve->t[0], p, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
   }
   else
   {
     actBNCopy( Q1->z, Q2->z, p->m_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
   }
}

/**********************************************************************************************************************
 *  actECPToMont()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actECPToMont(
  VSECPRIM_P2CONST_PARA(actECPOINT) Q,
  VSECPRIM_P2CONST_PARA(actECCURVE) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   VSECPRIM_P2CONST_PARA(actBNRING)  p_field  = &Curve->p_field;
   VSECPRIM_P2CONST_PARA(actBNDIGIT) RR       = p_field->RR;
   VSECPRIM_P2VAR_PARA(actBNDIGIT) tmp      = Curve->t[0];

   /* X -> XR */
   actBNMontMulCopy(Q->x, RR, tmp, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
   /* Y -> YR */
   actBNMontMulCopy(Q->y, RR, tmp, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
   /* Z -> ZR */
   if ((Q->is_affine) == FALSE)
   {
     actBNMontMulCopy(Q->z, RR, tmp, p_field, watchdog);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
   }
}

/**********************************************************************************************************************
 *  actECPToAffineFromMont()
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
 */
VSECPRIM_FUNC(actRETURNCODE) actECPToAffineFromMont(
  VSECPRIM_P2VAR_PARA(actECPOINT) Q,
  VSECPRIM_P2VAR_PARA(actECCURVE) Curve,
  boolean transformToAffineOnly,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal = actOK;

  VSECPRIM_P2CONST_PARA(actBNRING) p_field  = &Curve->p_field;
  actLengthType p_length = p_field->m_length;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) t0    = Curve->t[0];
  VSECPRIM_P2VAR_PARA(actBNDIGIT) t1    = Curve->t[1];
  VSECPRIM_P2VAR_PARA(actBNDIGIT) Z_inv = Curve->t[actEC_BIGNUM_TMP - 1u]; /* PRQA S 3494 */ /* MD_VSECPRIM_CONST_BOOL_OPERAND */ /*lint !e506 */

  /* In order to calculate the affine from the projective representation, we need to invert the z-coordinate of Q.
   * This is not possible, if Q is zero or meaningless, if Q is not on the curve. */
  if ((actBNIsZero(Q->z, p_length) == TRUE) || ((Q->is_infinity) == TRUE)) /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  {
    retVal = actEXCEPTION_POINT;
  }

  if (retVal == actOK)
  {
    /* calculate x-coordinate */
    /* Z_inv = ZR^-1 */
    actBNFieldInversion(Q->z, Z_inv, p_field, Curve->t, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    /* t0 = ZR^-2 */
    actBNMontMul(Z_inv, Z_inv, t0, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    /* XR = XR*ZR^-2 */
    actBNMontMulCopy(Q->x, t0, t1, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* calculate y-coordinate */
    /* t1 = ZR^-3 */
    actBNMontMul(Z_inv, t0, t1, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    /* YR = YR*ZR^-3 */
    actBNMontMulCopy(Q->y, t1, t0, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (transformToAffineOnly == FALSE)
    {
      /* check point */
      if (actECPointIsOnCurve(Q, Curve, watchdog) != actOK) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
      {
        retVal = actEXCEPTION_POINT;
      }
      else
      {
        /* transform from Montgomery */
        /* t1 = 1 */
        actBNSetOne(t1, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        actBNMontMulCopy(Q->x, t1, t0, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        actBNMontMulCopy(Q->y, t1, t0, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }
    }
    else
    {
      /* set affine flag, the point may be used in further calculations */
      Q->is_affine = TRUE; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  actECPointIsOnCurve()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actECPointIsOnCurve(
   VSECPRIM_P2CONST_PARA(actECPOINT) Q,
  VSECPRIM_P2CONST_PARA(actECCURVE) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   actRETURNCODE retVal = actOK;

   if ((Q->is_infinity) == TRUE)
   {
      retVal = actEXCEPTION_POINT;
   }
   else
   {
      VSECPRIM_P2CONST_PARA(actBNRING) p_field = &Curve->p_field;
      actLengthType p_length = p_field->m_length;
      actLengthType p_bytes  = p_field->m_byte_length;
      VSECPRIM_P2VAR_PARA(actBNDIGIT) a_R    = Curve->t[0];
      VSECPRIM_P2VAR_PARA(actBNDIGIT) b_R    = Curve->t[1];
      VSECPRIM_P2VAR_PARA(actBNDIGIT) tmp1   = Curve->t[2];
      VSECPRIM_P2VAR_PARA(actBNDIGIT) tmp2   = Curve->t[3];

      /*  We have to check: y^2 = x^3 + ax + b = x(x^2 + a) + b */

      /* a_R = a * R mod p */
      actBNSetOctetStringROM(a_R, p_length, Curve->a_R, p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      /* b_R = b * R mod p */
      actBNSetOctetStringROM(b_R, p_length, Curve->b_R, p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      /* tmp1 = Q_X^2 * R mod p */
      actBNMontMul(Q->x, Q->x, tmp1, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* tmp1 = (Q_X^2 + a) * R mod p */
      /* tmp1 < p_field->m subject to Montgomery multiplication, a_R < p_field->m because ? */
      actBNModAdd(tmp1, a_R, tmp1, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* tmp2 = (Q_X^2 + a) * Q_X * R mod p */
      actBNMontMul(tmp1, Q->x, tmp2, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* tmp1 = ((Q_X^2 + a) * Q_X + b) * R mod p */
      /* tmp2 < p_field->m subject to Montgomery multiplication, b_R < p_field->m because ? */
      actBNModAdd(tmp2, b_R, tmp1, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* tmp2 = Q_Y^2 * R mod p */
      actBNMontMul(Q->y, Q->y, tmp2, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* compare tmp1 and tmp2 */
      if (actBNCompare(tmp1, tmp2, p_length) != actCOMPARE_EQUAL) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
      {
         retVal = actEXCEPTION_POINT;
      }
   }

   return retVal;
}

/**********************************************************************************************************************
 *  actECPDouble()
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
 */
VSECPRIM_FUNC(void) actECPDouble(
  VSECPRIM_P2VAR_PARA(actECPOINT) Q,
  VSECPRIM_P2CONST_PARA(actECCURVE) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   VSECPRIM_P2CONST_PARA(actBNRING)  p_field = &Curve->p_field;
   VSECPRIM_P2VAR_PARA(actBNDIGIT) result  = Curve->t[0];
   actLengthType p_length = p_field->m_length;

   /* t1 = X_1, t2 = Y_1, t3 = Z_1 */
   VSECPRIM_P2VAR_PARA(actBNDIGIT) t1 = Q->x;
   VSECPRIM_P2VAR_PARA(actBNDIGIT) t2 = Q->y;
   VSECPRIM_P2VAR_PARA(actBNDIGIT) t3 = Q->z;

   if ((Q->is_infinity) == TRUE)
   {
      /* t3==0, Q==Infinity */
   }
   else if (actBNIsZero(t2, p_length) == TRUE) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
   {
      /* t2==0, set Q=Infinity and return */
      Q->is_infinity = TRUE; /* SBSW_VSECPRIM_WRITE_POINTER */
   }
   else
   {
     /* actBNModSub and actBNModAdd subtracts (or adds, respectively) two values mod p_field->m.
     The modulo operation only works correctly, if both values are less than p_field->m.
     For every call, this is pre-checked in the comments. */

      /* need variables t4 and t5 */
      VSECPRIM_P2VAR_PARA(actBNDIGIT) t4 = Curve->t[1];
      VSECPRIM_P2VAR_PARA(actBNDIGIT) t5 = Curve->t[2];

      if ((Curve->a_equals_p_minus_3) == TRUE)
      {
         /* t4 = Z_1^2 */
         actBNMontMul(t3, t3, t4, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         /* t5 = X_1 - Z_1^2 */
         /* t1 < m, because t1 = Q->x, t4 < m subject to Montgomery multiplication  =>  t5 < m */
         actBNModSub(t1, t4, t5, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         /* t4 = X_1 + Z_1^2 */
         /* t1, t4 < m, see above */
         actBNModAdd(t1, t4, t4, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         /* result = t4 * t5 = (X_1 - Z_1^2)*(X_1 + Z_1^2) = (X_1^2 - Z_1^4) */
         actBNMontMul(t4, t5, result, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

         /* t4 = 3*result = 3*X_1^2 - 3*Z_1^4 = M */
         /* result < m subject to Montgomery multiplication  =>  t4 < m */
         actBNModAdd(result, result, t4, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         /* t4, result < m, see above  =>  resulting value from actBNModAdd t4 < m */
         actBNModAdd(t4, result, t4, p_field, watchdog); /* = M */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }
      else
      {
         /* t4 = a */
         actBNSetOctetStringROM(t4, p_length, Curve->a_R, Curve->p_field.m_byte_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
         if (actBNIsZero(t4, p_length) == FALSE) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
         {
            /* t5 = Z_1^2 */
            actBNMontMul(t3, t3, t5, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
            /* result = Z_1^4 */
            actBNMontMul(t5, t5, result, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
            /* t5 = a*Z_1^4 */
            actBNMontMul(result, t4, t5, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         }
         else
         {
            actBNSetZero(t5, p_length); /* t5 = 0 if a==0 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
         }
         /* From here on t5 = a*Z_1^4 (possibly 0, if a = 0) */
         /* result = X_1^2 */
         actBNMontMul(t1, t1, result, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

         /* t4 = 3 * X_1^2 + a*Z_1^4 = M */
         /* result < m subject to Montgomery multiplication  =>  t4 < m */
         actBNModAdd(result, result, t4, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         /* t4, result < m, see above  =>  resulting value from actBNModAdd t4 < m */
         actBNModAdd(t4, result, t4, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         /* t4 < m, see above, t5 < m, because t5 is either 0 or the result of a Montgomery multiplication
         =>  resulting value of actBNModAdd t4 < m */
         actBNModAdd(t4, t5, t4, p_field, watchdog); /* = M */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }

      /* By now, t4, t5 < m; Now compute Z_2:
       * result = 2 * t2 * t3 = 2 * Y_1 * Z_1 = Z_2 */
      actBNMontMul(t2, t3, result, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* result < m subject to Montgomery multiplication */
      actBNModAdd(result, result, t3, p_field, watchdog); /* = Z_2 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* Next, compute S and X_2:
       * t2 = result = Y_1^2 */
      actBNMontMulCopy(t2, t2, result, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* t5 = t1 * t2 = 4*X_1*Y_1^2 = S */
      actBNMontMul(t1, t2, t5, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* t5 < m subject to Montgomery multiplication  =>  resulting value from actBNModAdd t5 < m */
      actBNModAdd(t5, t5, t5, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* t5 < m   =>  resulting value from actBNModAdd t5 < m */
      actBNModAdd(t5, t5, t5, p_field, watchdog); /* = S */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* result = M^2 */
      actBNMontMul(t4, t4, result, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* t1 = M^2 - 2*S = X_2 */
      /* result < m subject to Montgomery multiplication, t5 < m  =>  resulting value from actBNModSub t1 < m */
      actBNModSub(result, t5, t1, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* t1, t5 < m  =>  resulting value from actBNModSub t1 < m */
      actBNModSub(t1, t5, t1, p_field, watchdog); /* = X_2 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* Finally, compute T and Y_2 */
      /* result = Y_1^4, t2 = 8*Y_1^4 = T */
      actBNMontMul(t2, t2, result, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* result < m subject to Montgomery multiplication =>  resulting value from actBNModAdd t2 < m */
      actBNModAdd(result, result, t2, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* t2 < m, see above  =>  resulting value from actBNModAdd t2 < m */
      actBNModAdd(t2, t2, t2, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      actBNModAdd(t2, t2, t2, p_field, watchdog); /* = T */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* t5 = S - X_2 */
      /* t5, t1 < m  =>  resulting value from actBNModSub t5 < m */
      actBNModSub(t5, t1, t5, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* result = (S - X_2) * M */
      actBNMontMul(t5, t4, result, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* t2 = M * (S - X_2) - T = Y_2 */
      /* result, t2 < m */
      actBNModSub(result, t2, t2, p_field, watchdog); /* = Y_2 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
   }
}

/**********************************************************************************************************************
 *  actECPAdd()
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
 */
VSECPRIM_FUNC(void) actECPAdd(
  VSECPRIM_P2VAR_PARA(actECPOINT) Q1,
  VSECPRIM_P2CONST_PARA(actECPOINT) Q2,
  VSECPRIM_P2CONST_PARA(actECCURVE) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   VSECPRIM_P2CONST_PARA(actBNRING)  p_field = &Curve->p_field;
   VSECPRIM_P2VAR_PARA(actBNDIGIT) result  = Curve->t[0];
   actLengthType p_length = p_field->m_length;

   /* t1 = X_0, t2 = Y_0, t3 = Z_0 */
   VSECPRIM_P2VAR_PARA(actBNDIGIT) t1 = Q1->x;
   VSECPRIM_P2VAR_PARA(actBNDIGIT) t2 = Q1->y;
   VSECPRIM_P2VAR_PARA(actBNDIGIT) t3 = Q1->z;

   if ((Q2->is_infinity) == TRUE)
   {
      /* Q1 + Infinity = Q1, return */
   }
   else if ((Q1->is_infinity) == TRUE)
   {
      /* Infinity + Q2 = Q2, set Q1=Q2 and return */
      actECPCopy(Q1, Q2, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
   }
   else
   {
     /* actBNModSub and actBNModAdd subtracts (or adds, respectively) two values mod p_field->m.
     The modulo operation only works correctly, if both values are less than p_field->m.
     For every call, this is pre-checked in the comments. */

      /* need variables t4, t5, t6 and t7 */
      VSECPRIM_P2VAR_PARA(actBNDIGIT) t4 = Curve->t[1];
      VSECPRIM_P2VAR_PARA(actBNDIGIT) t5 = Curve->t[2];
      VSECPRIM_P2VAR_PARA(actBNDIGIT) t7 = Curve->t[3];
      /* t6 is only required if Q2 is not affine; however, it needs to be
       * initialized in order to avoid a compiler warning */
      VSECPRIM_P2CONST_PARA(actBNDIGIT) t6 = Curve->t[3]; /* PRQA S 2981 */ /* MD_VSECPRIM_2.2 */

      /* t4 = X_1, t5 = Y_1 */
      actBNCopy(t4, Q2->x, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      actBNCopy(t5, Q2->y, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

      /* The computation of U_0 and S_0 requires Q2->z, and therefore Q2 needs to be projective */
      if ((Q2->is_affine) == FALSE)
      {
         /* t6 = Z_1 */
         t6 = Q2->z;
         /* t7 = Z_1^2 */
         actBNMontMul(t6, t6, t7, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         /* t1 = result = X_0 * Z_1^2 = U_0 */
         actBNMontMulCopy(t1, t7, result, p_field, watchdog); /* = U_0 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

         /* result =  Z_1^3 */
         actBNMontMul(t7, t6, result, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         /* t2 = t7 = Y_0 * Z_1^3 = S_0 */
         actBNMontMulCopy(t2, result, t7, p_field, watchdog); /* = S_0 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }

      /* t7 = Z_0^2 */
      actBNMontMul(t3, t3, t7, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* result = X_1 * Z_0^2 = U_1 */
      actBNMontMul(t4, t7, result, p_field, watchdog); /* = U_1 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* t4 =  U_0 - U_1 = W */
      /* t1, result < m, subject to Montgomery multiplication, or because t1 = Q1->x  =>  t4 < m */
      actBNModSub(t1, result, t4, p_field, watchdog);  /* = W */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* t7 = Z_0^2 * Z_0 = Z_0^3 */
      actBNMontMulCopy(t7, t3, result, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* result =  Y_1 * Z_0^3 = S_1 */
      actBNMontMul(t5, t7, result, p_field, watchdog); /* = S_1 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* t5 =  S_0 - S_1 = R */
      /* t2, result < m, subject to Montgomery multiplication, or because t2 = Q1->y  =>  t5 < m */
      actBNModSub(t2, result, t5, p_field, watchdog);  /* = R */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      if (actBNIsZero(t4, p_length) == TRUE) /* W == 0? */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      {
         if (actBNIsZero(t5, p_length) == TRUE) /* R == 0? */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
         {
            /* t5==0 means Q1==Q2, recover Q1 from Q2, double Q1 and return */
            actECPCopy(Q1, Q2, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
            actECPDouble(Q1, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         }
         else
         {
            /* set Q1=Infinity and return */
            Q1->is_infinity = TRUE; /* SBSW_VSECPRIM_WRITE_POINTER */
         }
      }
      else
      {
        /* t1 = 2*t1 - t4 = 2*U_0 - W = 2*U_0 - (U_0 - U_1) = U_0 + U_1 = T */
        /* t1 < m, either because t1=Q1->x or subject to Montogmery multiplication
        =>  resulting value vom actBNModAdd t1 < m */
         actBNModAdd(t1, t1, t1, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         /* t1, t4 < m, see above  => t1 < m */
         actBNModSub(t1, t4, t1, p_field, watchdog); /* = T */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

         /* t2 = 2*S_0 - R = 2*S_0 - (S_0 - S_1) = S_0 + S_1 = M */
         /* t2 < m, either because t2=Q1->y or subject to Montogmery multiplication
         =>  resulting value vom actBNModAdd t2 < m */
         actBNModAdd(t2, t2, t2, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         /* t2, t5 < m, see above  =>  t2 < m */
         actBNModSub(t2, t5, t2, p_field, watchdog); /* = M */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

         /* Compute Z_2 */
         if ((Q2->is_affine) == FALSE)
         {
            /* t3 = Z_0 * Z_1 */
            actBNMontMulCopy(t3, t6, result, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         }
         /* t3 = Z_0 * Z_1 * W = Z_2 */
         actBNMontMulCopy(t3, t4, result, p_field, watchdog); /* = Z_2 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

         /* Compute X_2 */
         /* t7 = W^2 */
         actBNMontMul(t4, t4, t7, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         /* t4 = W^3, will be used later for the computation of Y_2 */
         actBNMontMulCopy(t4, t7, result, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

         /* t7 = W^2 * T */
         actBNMontMulCopy(t7, t1, result, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         /* result = R^2 */
         actBNMontMul(t5, t5, result, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         /* t1 = R^2 - T*W^2 = X_2 */
         /* result, t7 < m subject to Montgomery multiplication  =>  t1 < m */
         actBNModSub(result, t7, t1, p_field, watchdog); /* = X_2 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

         /* t7 = T*W^2 - 2*X_2 = V */
         /* t7, t1 < m, see above  =>  resulting value from actBNModSub t7 < m */
         actBNModSub(t7, t1, t7, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         actBNModSub(t7, t1, t7, p_field, watchdog); /* = V */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

         /* t5 = R * V */
         actBNMontMulCopy(t5, t7, result, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         /* result = W^3 * M */
         actBNMontMul(t4, t2, result, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         /* t2 = R*V - W^3*M = 2*Y_2 */
         /* t5, result < m subject to Montgomery multiplication  =>  t2 < m */
         actBNModSub(t5, result, t2, p_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         /* t2 = 2 * Y_2 / 2 = Y_2 */
         actBNModDiv2(t2, p_field, watchdog); /* = Y_2 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }
   }
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  actECPSub()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC (void) actECPSub (
  VSECPRIM_P2VAR_PARA( actECPOINT ) Q1,
  VSECPRIM_P2CONST_PARA( actECPOINT ) Q2,
  VSECPRIM_P2CONST_PARA( actECCURVE ) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
    VSECPRIM_P2CONST_PARA (actBNRING) p = &Curve->p_field;

    /* actBNModSub subtracts two values mod p_field->m.
    The modulo operation only works correctly, if both values are less than p_field->m.
    For every call, this is pre-checked in the comments. */

    /* p-> m < p->m, Q2->y < p->m  =>  resulting value from actBNModSub Q2->y < m */
    actBNModSub (p->m, Q2->y, Q2->y, p,     watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    actECPAdd   (Q1, Q2, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    /* p-> m < p->m, Q2->y < p->m */
    actBNModSub (p->m, Q2->y, Q2->y, p, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  actECPMult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actECPMult(
  VSECPRIM_P2CONST_PARA(actECPOINT) Q,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) k,
  VSECPRIM_P2VAR_PARA(actECPOINT) R,
  VSECPRIM_P2CONST_PARA(actECCURVE) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   actLengthType i = actBNGetBitLength(Curve->n_field.m, Curve->n_field.m_length); /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */

   /* R = Infinity */
   R->is_affine   = FALSE; /* SBSW_VSECPRIM_WRITE_POINTER */
   R->is_infinity = TRUE; /* SBSW_VSECPRIM_WRITE_POINTER */

   /* Double-And-Add loop */
   for (; i > 0u; --i)
   {
      /* R *= 2 */
      actECPDouble(R, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* test bit i of k */
      if (actBNGetBit(k, i - 1u))
      {
         /* R += Q */
         actECPAdd(R, Q, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }
   }
}

/**********************************************************************************************************************
 *  actECPMultShort()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC( void ) actECPMultShort (
  VSECPRIM_P2CONST_PARA( actECPOINT ) Q,
  const actU32      k,
  VSECPRIM_P2VAR_PARA( actECPOINT ) R,
  VSECPRIM_P2CONST_PARA( actECCURVE ) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   actU32   mask = 0x80000000UL;

   /* R = Infinity */
   R->is_affine   = FALSE; /* SBSW_VSECPRIM_WRITE_POINTER */
   R->is_infinity = TRUE; /* SBSW_VSECPRIM_WRITE_POINTER */

   /* Double-And-Add loop */
   while (mask != 0u)
   {
      /* R *= 2 */
      actECPDouble (R, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* test bit i of k */
      if ((mask & k) != 0u)
      {
         /* R += Q */
         actECPAdd (R, Q, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }

      mask >>= 1u;
   }
}

/**********************************************************************************************************************
 *  actECPMultG()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actECPMultG(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) k,
  VSECPRIM_P2VAR_PARA(actECPOINT) Prec,
  VSECPRIM_P2VAR_PARA(actECPOINT) R,
  VSECPRIM_P2CONST_PARA(actECCURVE) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   VSECPRIM_P2CONST_PARA(actBNRING) p_field = &Curve->p_field;
   actLengthType p_length = p_field->m_length;
   actLengthType p_bytes = p_field->m_byte_length;
   actLengthType n_bits = actBNGetBitLength(Curve->n_field.m, Curve->n_field.m_length); /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
   actLengthType prec_offset = 3u;

   actLengthType prec_length;
   actLengthType i, j, group_bits, index, bit_pos;
   VSECPRIM_P2ROMCONST_PARA(actU8) prec_ptr;
   VSECPRIM_P2ROMCONST_PARA(actU8) correctionPoint_ptr;

# if (vSecPrim_GetECPMaxKeySizeOfGeneral() >= VSECPRIM_ECP_MAX_KEY_SIZE_512) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
   if (p_bytes >= 64u)
   {
     prec_offset = 4u;
   }
# endif
   prec_length = prec_offset + (2u * p_bytes);

   /* calculate group_bits */
   for (i = n_bits; (i % Curve->groups) != 0u; ++i) {}
   group_bits = i / Curve->groups;

   /* R = Infinity */
   R->is_affine   = FALSE; /* SBSW_VSECPRIM_WRITE_POINTER */
   R->is_infinity = TRUE; /* SBSW_VSECPRIM_WRITE_POINTER */

   /* 'Prec' will be a precomputed affine point in Montgomery representation */
   Prec->is_affine   = TRUE; /* SBSW_VSECPRIM_WRITE_POINTER */
   Prec->is_infinity = FALSE; /* SBSW_VSECPRIM_WRITE_POINTER */

   /* Double-And-Add loop, using a pre-computed point, if there is one, and G_r if there is none */
   for (i = group_bits; i > 0u; --i)
   {
      /* R *= 2 */
      actECPDouble(R, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* calculate index of precomputed point */
      index = 0u;
      for (j = Curve->groups; j > 0u; --j)
      {
         bit_pos = ((j - 1u) * group_bits) + (i - 1u); /* bit position */
         if (bit_pos < n_bits)
         {
            index += ((actLengthType)(actBNGetBit(k, bit_pos)) << (j - 1u));
         }
      }
      /* initialize precomputed point */
      if (index == 0u)
      {
         /* set pointer to x-coordinate of basepoint */
         prec_ptr = Curve->G_R;
      }
      else
      {
         /* set pointer to x-coordinate of the i-th precomputed point (i=index) */
        prec_ptr = &(Curve->prec_first[((index - 1u) * prec_length) + prec_offset]);
      }

      actBNSetOctetStringROM(Prec->x, p_length, prec_ptr, p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      actBNSetOctetStringROM(Prec->y, p_length, &prec_ptr[p_bytes], p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      /* R += Prec */
      actECPAdd(R, Prec, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
   }

   /* initialize correction point */
   correctionPoint_ptr = &(Curve->prec_first[(((((actLengthType)1u) << Curve->groups) - 1u) * prec_length) + prec_offset]);
   actBNSetOctetStringROM(Prec->x, p_length, correctionPoint_ptr, p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
   actBNSetOctetStringROM(Prec->y, p_length, &correctionPoint_ptr[p_bytes], p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
   /* R += Prec */
   actECPAdd(R, Prec, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  actECPSimMult()
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
VSECPRIM_FUNC(void) actECPSimMult(
  VSECPRIM_P2CONST_PARA(actECPOINT) Q1,
  VSECPRIM_P2CONST_PARA(actECPOINT) Q2,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) k1,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) k2,
  VSECPRIM_P2VAR_PARA(actECPOINT) Q12,
  VSECPRIM_P2VAR_PARA(actECPOINT) R,
  VSECPRIM_P2CONST_PARA(actECCURVE) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   actLengthType i = actBNGetBitLength(Curve->n_field.m, Curve->n_field.m_length); /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
   actLengthType tmpI;
   actU8 k1_bit, k2_bit;

   /* R = Infinity */
   R->is_affine   = FALSE; /* SBSW_VSECPRIM_WRITE_POINTER */
   R->is_infinity = TRUE; /* SBSW_VSECPRIM_WRITE_POINTER */

   /* Q12 = Q1 + Q2 */
   Q12->is_affine = FALSE; /* SBSW_VSECPRIM_WRITE_POINTER */
   actECPCopy(Q12, Q1, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
   actECPAdd(Q12, Q2, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

# if (VSECPRIM_ACTBNEUCLID_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
   actECPToAffineFromMont(Q12, Curve, TRUE);
# endif

   /* Double-And-Add loop */
   for (; i > 0u; --i)
   {
      tmpI = i - 1u;
      /* R *= 2 */
      actECPDouble(R, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* test bit i of k1 and k2 */
      k1_bit = (actU8)actBNGetBit(k1, tmpI);
      k2_bit = (actU8)actBNGetBit(k2, tmpI);
      if (k1_bit != 0u)
      {
         if (k2_bit != 0u)
         {
            /* R += (Q1+Q2) */
            actECPAdd(R, Q12, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         }
         else
         {
            /* R += Q1 */
            actECPAdd(R, Q1, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         }
      }
      else
      {
         if (k2_bit != 0u)
         {
            /* R += Q2 */
            actECPAdd(R, Q2, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
         }
      }
   }
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTECPOINT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actECPoint.c
 *********************************************************************************************************************/
