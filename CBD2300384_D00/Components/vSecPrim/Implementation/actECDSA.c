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
/*!        \file  actECDSA.c
 *        \brief  Implementation of Elliptic Curve Digital Signature Algorithm
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

#define ACTECDSA_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actECDSA.h"
#include "actECLengthInfo.h"

#if (VSECPRIM_ACTECDSA_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 actECReduceXModN()
 **********************************************************************************************************************/
/*! \brief      Reduces the x-coordinate of a point on a curve modulo n
 *  \details    x is the x-coordinate x of a point on a curve over a p-field and shall be reduced modulo n, where
 *              n is the order of the base point of the curve
 *  \param[in]      n  order of the base point of the curve
 *  \param[in]      n_length length of n
 *  \param[in,out]  x (in)  x-coordinate of the point on the curve, which shall be reduced mod n
 *                    (out) x mod n
 *  \param[in]      p_length length of the prime p of the underlying field over which the curve is defined
 *  \param[in,out]  Nval (in)  auxiliary variable to store n enlarged to p_length if necessary
 *                       (out) buffer of length p_length storing n and additional zeros at the end if
 *                             p_length > n_length
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        All provided pointers must be valid.
 *              The buffer referenced by x must provide at least p_length actBNDIGITs.
 *              The buffer referenced by Nval must provide at least p_length actBNDIGITs.
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actECReduceXModN(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) n,
  actLengthType n_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) x,
  actLengthType p_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) Nval);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  actECReduceXModN()
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
VSECPRIM_LOCAL_FUNC(void) actECReduceXModN(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) n,
  actLengthType n_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) x,
  actLengthType p_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) Nval)
{
  /* for n_length > p_length is nothing to do */
  if (p_length >= n_length)
  {
    /* copy n to a workspace variable of length >= p_length */
    actBNSetZero(Nval, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    actBNCopy(Nval, n, n_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    /* Reduce per subtraction; this is sufficient, because n is close to p in general.
    (The Hasse bound for cofactor 1 is:   |n-p| <= 2*sqroot(p) + 1 )  */
    while (actBNCompare(x, Nval, p_length) >= actCOMPARE_EQUAL) /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    {
       actBNSub(x, Nval, x, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    }
  }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  actECDSAvp()
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
VSECPRIM_FUNC(actRETURNCODE) actECDSAvp(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) e,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) s,
  VSECPRIM_P2CONST_PARA(actECPOINT) Q,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp_ptr,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal = actOK;

   actECPOINT G, QG, point;
   VSECPRIM_P2CONST_PARA(actBNRING) n_field = &wksp_ptr->baseData.Curve.n_field;
   VSECPRIM_P2VAR_PARA(actBNDIGIT) x1     = wksp_ptr->baseData.Curve.t[0];
   VSECPRIM_P2VAR_PARA(actBNDIGIT) s_inv  = wksp_ptr->baseData.Curve.t[actEC_BIGNUM_TMP-1u]; /* PRQA S 3494 */ /* MD_VSECPRIM_CONST_BOOL_OPERAND */ /*lint !e506 */
   actLengthType p_length = wksp_ptr->baseData.Curve.p_field.m_length;
   actLengthType p_bytes  = wksp_ptr->baseData.Curve.p_field.m_byte_length;
   actLengthType n_length = wksp_ptr->baseData.Curve.n_field.m_length;
   VSECPRIM_P2VAR_PARA(actBNDIGIT) u1;
   VSECPRIM_P2VAR_PARA(actBNDIGIT) u2;

   u1 = wksp_ptr->dataPair.u12.u1;
   u2 = wksp_ptr->dataPair.u12.u2;
   G.x = wksp_ptr->curvePoint.basePoint.xcoord;
   G.y = wksp_ptr->curvePoint.basePoint.ycoord;
   QG.x = wksp_ptr->multG.QG.xcoord;
   QG.y = wksp_ptr->multG.QG.ycoord;
   QG.z = wksp_ptr->multG.QG.zcoord;
   point.x = wksp_ptr->Q.xcoord;
   point.y = wksp_ptr->Q.ycoord;
   point.z = wksp_ptr->Q.zcoord;

   /* initialization of basepoint coordinates Gx, Gy, Gz */
   actBNSetOctetStringROM(G.x, p_length, wksp_ptr->baseData.Curve.G_R, p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
   actBNSetOctetStringROM(G.y, p_length, &(wksp_ptr->baseData.Curve.G_R[p_bytes]), p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
   G.is_affine   = TRUE;
   G.is_infinity = FALSE;

   /* Convert Q to Montgomery Representation */
   actECPToMont(Q, &(wksp_ptr->baseData.Curve), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

   /* check if Q is on curve Q before continue */
   if (actECPointIsOnCurve(Q, &(wksp_ptr->baseData.Curve), watchdog) != actOK) /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
   {
      retVal = actEXCEPTION_PUBKEY;
   }

   if (retVal == actOK)
   {

     /* Compute u1 and u2 */

     /* u1 = sR mod n (s in Montgomery Representation) */
     actBNMontMul(s, n_field->RR, u1, n_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
     /* s_inv = (s^-1)*R mod n */
     actBNFieldInversion(u1, s_inv, n_field, wksp_ptr->baseData.Curve.t, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
     /* u1 = (e * s^(-1)) mod n */
     actBNMontMul(e, s_inv, u1, n_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
     /* u2 = (r * s^(-1)) mod n */
     actBNMontMul(r, s_inv, u2, n_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

     /* Compute x-Coordinate of u1*Basepoint + u2*pubkey */

     /* Compute point = u1*Basepoint + u2*pubkey */
     actECPSimMult(&G, Q, u1, u2, &QG, &point, &(wksp_ptr->baseData.Curve), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
     /* Compute x-Coordinate x1 of point */
     if (actECPToAffineFromMont(&point, &(wksp_ptr->baseData.Curve), 0, watchdog) != actOK) /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
     {
       retVal = actEXCEPTION_UNKNOWN;
     }

     if (retVal == actOK)
     {
       actBNCopy(x1, point.x, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

       /* Reduce the x-Coordinate x1 mod n */
       actECReduceXModN(n_field->m, n_length, x1, p_length, wksp_ptr->baseData.Curve.t[1]); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

       /* finally compare x1 and r (should be equal) */
       if (actBNCompare(x1, r, n_length) != actCOMPARE_EQUAL) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
       {
         retVal = actSIGNATURE_INVALID;
       }
     }
   }

   return retVal;
} /* PRQA S 6050, 6060 */ /* MD_MSR_STCAL, MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actECDSAsp()
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
VSECPRIM_FUNC(actRETURNCODE) actECDSAsp(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) e,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) d,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) s,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal = actOK;
   actRETURNCODE rng_callback_error; /* rng_error is an error code of the actPRNG_GET callback function */

   actECPOINT Prec, Q;
   VSECPRIM_P2CONST_PARA(actBNRING) n_field = &(wksp->baseData.Curve.n_field);
   actLengthType n_length = n_field->m_length;
   actLengthType p_length = wksp->baseData.Curve.p_field.m_length;
   VSECPRIM_P2CONST_PARA(actBNDIGIT) RR     = n_field->RR;
   VSECPRIM_P2VAR_PARA(actBNDIGIT) x1     = wksp->baseData.Curve.t[0];
   VSECPRIM_P2VAR_PARA(actBNDIGIT) k_inv  = wksp->baseData.Curve.t[actEC_BIGNUM_TMP-1u]; /* PRQA S 3494 */ /* MD_VSECPRIM_CONST_BOOL_OPERAND */ /*lint !e506 */
   VSECPRIM_P2VAR_PARA(actBNDIGIT) t0     = wksp->baseData.Curve.t[0];
   VSECPRIM_P2VAR_PARA(actBNDIGIT) t1     = wksp->baseData.Curve.t[1];
   VSECPRIM_P2VAR_PARA(actBNDIGIT) k;

   actBoolean found = FALSE;

   k = wksp->dataPair.privKeys.ephKey;
   Q.x = wksp->multG.kG.xcoord;
   Q.y = wksp->multG.kG.ycoord;
   Q.z = wksp->multG.kG.zcoord;
   Prec.x = wksp->curvePoint.precomputedPoint.xcoord;
   Prec.y = wksp->curvePoint.precomputedPoint.ycoord;

   /* from a formal point of view, signature generation is an endless loop, but
      the probability of termination even in the first round is very high */
   while (found == FALSE)
   {
      /* 1) generate a one time key pair (k, Q) */

      /* generate k at random */
      rng_callback_error = actBNModRandomize(k, n_field, wksp->baseData.Curve.t, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      if (rng_callback_error != actOK)
      {
        found = TRUE;
        retVal = rng_callback_error;
      }

      if (retVal == actOK)
      {
        /* compute Q = k*G */
        actECPMultG(k, &Prec, &Q, &(wksp->baseData.Curve), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        /* compute x-Coordinate x1 of point */
        if (actECPToAffineFromMont(&Q, &(wksp->baseData.Curve), 0, watchdog) != actOK) /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        {
          found = TRUE;
          retVal = actEXCEPTION_UNKNOWN;
        }

        if (retVal == actOK)
        {
          actBNCopy(x1, Q.x, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

          /* 2)  r = (X-coordinate of Q) mod n; if r==0, goto step 1) */

          /* Reduce the x-Coordinate x1 mod n (assumption: n_length >= p_length,
          which is the case for all supported curves) */
          actECReduceXModN(n_field->m, n_length, x1, p_length, wksp->baseData.Curve.t[1]); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

          /* r = x1 */
          actBNSetZero(r, n_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
          actBNCopy(r, x1, n_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
          if (actBNIsZero(r, n_length) == FALSE) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
          {
            /* 3 )  s = k^(-1) * (e + d*r) mod n; if s==0 then go to step 1) */

            /* k = k*R mod n (k in Montgomery Representation) */
            actBNMontMulCopy(k, RR, t0, n_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
            /* k_inv = (k^-1)*R mod n */
            actBNFieldInversion(k, k_inv, n_field, wksp->baseData.Curve.t, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
            /* t0 = d*r*(R^-1) mod n */
            actBNMontMul(d, r, t0, n_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
            /* t1 = d*r mod n */
            actBNMontMul(t0, RR, t1, n_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
            /* t0 = e + d*r mod n */
            actBNModAdd(e, t1, t0, n_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
            /* t1 = k^(-1) * (e + d*r) mod n */
            actBNMontMul(k_inv, t0, t1, n_field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

            /* s = t1 */
            actBNSetZero(s, n_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
            actBNCopy(s, t1, n_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

            if (actBNIsZero(s, n_length) == FALSE) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
            {
              found = TRUE;
            }
            else
            {
              found = FALSE;
            }
          }
        }
      }
   }

   return retVal;
} /* PRQA S 6050, 6060, 6080 */ /* MD_MSR_STCAL, MD_VSECPRIM_STPAR, MD_MSR_STMIF */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTECDSA_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actECDSA.c
 *********************************************************************************************************************/
