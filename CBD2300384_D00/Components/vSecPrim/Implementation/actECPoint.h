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
/*!        \file  actECPoint.h
 *        \brief  Interface for Elliptic curve point arithmetic functions.
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

#ifndef ACTECPOINT_H
# define ACTECPOINT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actUtilities.h"
# include "actBigNum.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* actPoint type
 The internal point representation is projective.
 affine -> proj:  (X, Y) -> (X, Y, 1)
 proj -> affine:  (X, Y, Z) -> (X/(Z^2), Y/(Z^3)) */
typedef struct
{
  VSECPRIM_P2VAR_PARA(actBNDIGIT) x; /* X-coordinate */
  VSECPRIM_P2VAR_PARA(actBNDIGIT) y; /* Y-coordinate */
  VSECPRIM_P2VAR_PARA(actBNDIGIT) z; /* Z-coordinate */
  boolean is_affine; /* internal flag for affine points (Z==1) */
  boolean is_infinity; /* internal flag for the infinity point */
} actECPOINT;

/* Curve type

 Contains pointers to:
 - the underlying field GF(p) and the field generated
 by the basepoint order GF(n)
 - the coefficient b and the basepoint G = (0x04, Gx, Gy
 - the precomputation information for scalar multiplications of the basepoint
 - temporary BigNum variables (see above) */
typedef actBNDIGIT actEcTempType[10];

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* Q == 0 */
# define actECPIsInfinity(Q, Curve)                                   (actBNIsZero((Q)->z, (Curve)->p_field->m_length) == TRUE)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C" {
# endif

/**********************************************************************************************************************
 *  actECPAssign()
 *********************************************************************************************************************/
/*! \brief           Assign point Q2 to point Q1 (Q1 = Q2).
 *  \details         The resulting point (Q1) will be in projective Montgomery representation.
 *  \param[in,out]  Q1           (in) destination point
 *                               (out) Q1 = Q2
 *  \param[in]      Q2           source point
 *  \param[in,out]  Curve        (in)  the underlying curve
 *                               (out) the underlying curve; the temporary variables referenced by the member t
 *                                     may have changed
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \pre            Q1 and Q2 must be valid pointers.
 *                  Q2 is in Montgomery representation (affine OR projective).
 *                  Q1 is in projective Montgomery representation.
 *                  All three buffers referenced by Q1 must provide at least p_length actBNDIGITs.
 *                  The buffer referenced by Curve->t[0] must provide at least p_length+1 actBNDIGITs, where p_length
 *                  is the length of the modulus p of the underlying field on which the curve is defined.
 *                  Note: p_length is stored in Curve.p_field.m_length.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC( void ) actECPAssign (
  VSECPRIM_P2VAR_PARA( actECPOINT ) Q1,
  VSECPRIM_P2CONST_PARA( actECPOINT ) Q2,
  VSECPRIM_P2CONST_PARA( actECCURVE ) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actECPToMont()
 *********************************************************************************************************************/
/*! \brief          This function converts a point from normal to Montgomery Representation: (X, Y, Z) -> (XR, YR, ZR)
 *  \details        -
 *  \param[in,out]  Q            (in)  the point (X, Y, Z) to be converted
 *                               (out) the converted point (XR, YR, ZR)
 *  \param[in,out]  Curve        (in)  the underlying curve
 *                               (out) the underlying curve; the temporary variables referenced by the member t
 *                                     may have changed
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \pre            Q and Curve must be valid pointers.
 *                  Each of the three buffers referenced by Q must provide at least p_length actBNDIGITs.
 *                  The buffer referenced by Curve->t[0] must provide at least p_length+1 actBNDIGITs, where p_length
 *                  is the length of the modulus p of the underlying field on which the curve is defined
 *                  Note: p_length is stored in Curve.p_field.m_length
 * \note            Q->is_affine is not touched by this routine
 *                  Q->is_infinity is not touched by this routine
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actECPToMont(
  VSECPRIM_P2CONST_PARA(actECPOINT) Q,
  VSECPRIM_P2CONST_PARA(actECCURVE) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actECPToAffineFromMont()
 *********************************************************************************************************************/
/*! \brief          Calculates the affine representation of the actPoint Q, where Q is in
 *                  Montgomery Representation (XR, YR, ZR)
 *  \details        -
 *  \param[in,out]  Q                      (in)  the projective point (XR, YR, ZR) in Montgomery representation
 *                                         (out) transformToAffineOnly==false: the validated affine point (X, Y)
 *                                               [validation means that checking whether the point is on the curve
 *                                               was positive]
 *                                               transformToAffineOnly==true: the affine point (XR, YR) in Montgomery
 *                                               Representation
 *  \param[in,out]  Curve                  (in)  the underlying curve
 *                                         (out) the underlying curve; the temporary variables referenced by the
 *                                               member t may have changed
 *  \param[in]      transformToAffineOnly  Output control flag; if zero the point will be checked
 *                                         to be on curve and transformed from Montgomery to normal
 *                                         representation (used in protocol functions, e.g. ECDSASign).
 *  \param[in]      watchdog                Watchdog trigger function pointer
 *  \return         actEXCEPTION_POINT  if !flag and Q is not on curve (internal error/attack)
 *                  actOK  else
 *  \pre            Q and Curve must be valid pointers.
 *                  The function assumes that Q != Infinity is in Montgomery Representation (XR, YR, ZR).
 *                  Two of the buffers referenced by Q must provide at least p_length actBNDIGITs.
 *                  The buffers referenced by Curve->t[0..(i-1)], where i = actEC_BIGNUM_TMP, must each provide at
 *                  least (p_length+1) actBNDIGITs, where p_length is the length of the modulus p of the underlying
 *                  field on which the curve is defined
 *                  Note: p_length is stored in Curve.p_field.m_length
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECPToAffineFromMont(
  VSECPRIM_P2VAR_PARA(actECPOINT) Q,
  VSECPRIM_P2VAR_PARA(actECCURVE) Curve,
  boolean transformToAffineOnly,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actECPointIsOnCurve()
 *********************************************************************************************************************/
/*! \brief          This function checks if a point Q is on curve, where Q is in 
 *                  Montgomery Representation (XR, YR, R)
 *  \details        -
 *  \param[in]      Q         the (affine) point to be checked, each coordinate is represented by a buffer
 *                            of length p_length
 *  \param[in,out]  Curve     (in)  the underlying curve
 *                            (out) the underlying curve; the temporary variables referenced by the member t
 *                                  may have changed
 *  \param[in]      watchdog  Watchdog trigger function pointer
 *  \return         actEXCEPTION_POINT     if the point is not on curve
 *                  actOK     else
 *  \pre            Q and Curve must be valid pointers.
 *                  Q is not the point at infinity and must be in Montgomery Representation (XR, YR, R)
 *                  The two buffers referenced by Q->x and Q->y each must provide at least p_length actBNDIGITs.
 *                  curve equation coefficient a is equal to p-3 (is the case for supported curves of the ECC module)
 *                  The buffers referenced by Curve->[0],..., Curve->t[3] must provide at least p_length+1
 *                  actBNDIGITs, where p_length is the length of the modulus p of the underlying field on which the
 *                  curve is defined
 *                  Note: p_length is stored in Curve.p_field.m_length
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECPointIsOnCurve(
  VSECPRIM_P2CONST_PARA(actECPOINT) Q,
  VSECPRIM_P2CONST_PARA(actECCURVE) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actECPDouble()
 *********************************************************************************************************************/
/*! \brief          This routine doubles the projective point Q (Q *= 2).
 *  \details        The in- and output point(s) is(are) projective and in Montgomery Representation.
 *                  Implementation Reference: IEEE 1363 / D 13, Algorithm A.10.4
 *  \param[in,out]  Q         (in)  the point (X_1*R, Y_1*R, Z_1*R)
 *                            (out) the point 2Q = (X_2*R, Y_2*R, Z_2*R)
 *  \param[in,out]  Curve     (in)  the underlying curve
 *                            (out) the underlying curve; the temporary variables referenced by the member t
 *                                  may have changed
 *  \param[in]      watchdog  pointer to watchdog trigger function
 *  \pre            Q and Curve must be valid pointers.
 *                  Q is in projective Montgomery Representation (XR, YR, ZR)
 *                  curve equation coefficient a is equal to p-3 (is the case for supported curves of the ECC module)
 *                  Each of the three buffers referenced by Q must provide at least p_length actBNDIGITs.
 *                  The buffers referenced by Curve->t[0], Curve->t[1] and Curve->t[2] each must provide at least
 *                  (p_length + 1) actBNDIGITs.
 *                  Note: p_length is stored in Curve.p_field.m_length
 *  \note           Q->is_affine is not touched by this routine
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actECPDouble(
  VSECPRIM_P2VAR_PARA(actECPOINT) Q,
  VSECPRIM_P2CONST_PARA(actECCURVE) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actECPAdd()
 *********************************************************************************************************************/
/*! \brief          This routine adds the affine point Q2 to Q1 (Q1 += Q2).
 *  \details        The in- and output point(s) is(are) projective and in Montgomery Representation.
 *                  Implementation Reference: IEEE 1363 / D 13, Algorithm A.10.5
 *  \param[in,out]  Q1        (in) the point (X_0*R, Y_0*R, Z_0*R)
 *                            (out) the point Q1+Q2 = (X_2*R, Y_2*R, Z_2*R)
 *  \param[in]      Q2        the point (X_1*R, Y_1*R, Z_1*R)
 *  \param[in,out]  Curve     (in) the underlying curve
 *                            (out) the underlying curve; the temporary variables referenced by the member t
 *                                  may have changed
 *  \param[in]      watchdog  pointer to watchdog trigger function
 *  \pre            Q1, Q2 and Curve must be valid pointers.
 *                  Q1 is in projective Montgomery Representation (XR, YR, ZR)
 *                  Q2 != 0 is in Montgomery Representation (XR, YR, ZR) (affine or pojective)
 *                  Each of the three buffers referenced by Q1 must provide at least p_length actBNDIGITs.
 *                  Each of the three buffers referenced by Q2 must provide at least p_length actBNDIGITs.
 *                  The buffers referenced by Curve->t[0], Curve->t[1], Curve->t[2] and Curve->t[3] each must provide
 *                  at least (p_length+1) actBNDIGITs.
 *                  Note: p_length is stored in Curve.p_field.m_length
 *  \note           Q1->is_affine is not touched by this routine
 *                  Q2->is_affine is not touched by this routine
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actECPAdd(
   VSECPRIM_P2VAR_PARA(actECPOINT) Q1,
   VSECPRIM_P2CONST_PARA(actECPOINT) Q2,
   VSECPRIM_P2CONST_PARA(actECCURVE) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actECPSub()
 *********************************************************************************************************************/
/*! \brief          This routine subtracts the point Q2 from Q1 (Q1 -= Q2).
 *  \details        The in- and output points are in Montgomery Representation.
 *                  Subtraction is achieved by negating the Y coordinate (Q2.y) and then adding the points.
 *                  Q2.y is restored after addition (just negated once more).
 *  \param[in,out]  Q1           (in) the point (X_0*R, Y_0*R, Z_0*R)
 *                               (out) the point Q1-Q2 = (X_2*R, Y_2*R, Z_2*R)
 *  \param[in]      Q2           the point (X_1*R, Y_1*R, Z_1*R)
 *  \param[in,out]  Curve        (in) the underlying curve
 *                               (out) the underlying curve; the temporary variables referenced by the member t
 *                                     may have changed
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \pre            see actECPAdd
 *  \note           Q1->is_affine is not touched by this routine
 *                  Q2->is_affine is not touched by this routine
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC( void ) actECPSub (
  VSECPRIM_P2VAR_PARA( actECPOINT ) Q1,
  VSECPRIM_P2CONST_PARA( actECPOINT ) Q2,
  VSECPRIM_P2CONST_PARA( actECCURVE ) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actECPMult()
 *********************************************************************************************************************/
/*! \brief          This routine calculates R = k*Q (scalar multiplication)
 *  \details        The in- and output point(s) is(are) in Montgomery Representation.
 *  \param[in]      Q            the point to be multiplied with k
 *  \param[in]      k            scalar
 *  \param[in,out]  R            (in) the point to store the result
 *                               (out) the point k*Q in Montgomery Representation (XR, YR, ZR)
 *  \param[in,out]  Curve        (in)  the underlying curve (see assumptions)
 *                               (out) the underlying curve; the temporary variables referenced by the member t
 *                                     may have changed
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \pre            Provided pointers must be valid.
 *                  Q is in affine Montgomery Representation (XR, YR, R)
 *                  Each of the three buffers referenced by R must provide at least p_length actBNDIGITs.
 *                  The buffers referenced by Curve->t[0], Curve->t[1], Curve->t[2] and Curve->t[3] each must provide
 *                  at least (p_length+1) actBNDIGITs.
 *                  Note: p_length is stored in Curve.p_field.m_length
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC( void ) actECPMult(
  VSECPRIM_P2CONST_PARA( actECPOINT ) Q,
  VSECPRIM_P2CONST_PARA( actBNDIGIT ) k,
  VSECPRIM_P2VAR_PARA( actECPOINT ) R,
  VSECPRIM_P2CONST_PARA( actECCURVE ) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actECPMultShort()
 *********************************************************************************************************************/
/*! \brief          This routine calculates R = k*Q (scalar multiplication).
 *  \details        The in- and output point(s) is(are) in Montgomery Representation.
 *  \param[in]      Q            the point to be multiplied with k
 *  \param[in]      k            scalar
 *  \param[in,out]  R            (in) the point to store the result
 *                               (out) the point k*Q in Montgomery Representation (XR, YR, ZR)
 *  \param[in,out]  Curve        (in)  the underlying curve (see assumptions)
 *                               (out) the underlying curve; the temporary variables referenced by the member t
 *                                     may have changed
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \pre            Provided pointers must be valid.
 *                  Q is in affine Montgomery Representation (XR, YR, R)
 *                  Each of the three buffers referenced by R must provide at least p_length actBNDIGITs.
 *                  The buffers referenced by Curve->t[0], Curve->t[1], Curve->t[2] and Curve->t[3] each must provide
 *                  at least (p_length+1) actBNDIGITs.
 *                  Note: p_length is stored in Curve.p_field.m_length
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC( void ) actECPMultShort(
  VSECPRIM_P2CONST_PARA( actECPOINT ) Q,
  const actU32 k,
  VSECPRIM_P2VAR_PARA( actECPOINT ) R,
  VSECPRIM_P2CONST_PARA( actECCURVE ) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actECPMultG()
 *********************************************************************************************************************/
/*! \brief          This routine calculates R = k*G (scalar multiplication of basepoint).
 *  \details        The in- and output point(s) is(are) in Montgomery Representation.
 *  \param[in]      k            scalar
 *  \param[in,out]  Prec         temporary point to store a precomputed point
 *  \param[in,out]  R            (in) the point to store the result
 *                               (out) the point k*G in Montgomery
 *                               Representation (XR, YR, ZR)
 *  \param[in,out]  Curve        (in)  the underlying curve
 *                               (out) the underlying curve; the temporary variables referenced by the member t
 *                                     may have changed
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \pre            Provided pointers must be valid.
 *                  Each of the three buffers referenced by Prec must provide at least p_length actBNDIGITs.
 *                  Each of the three buffers referenced by R must provide at least p_length actBNDIGITs.
 *                  The buffers referenced by Curve->t[0], Curve->t[1], Curve->t[2] and Curve->t[3] each must provide
 *                  at least (p_length+1) actBNDIGITs.
 *                  Note: p_length is stored in Curve.p_field.m_length
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actECPMultG(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) k,
  VSECPRIM_P2VAR_PARA(actECPOINT) Prec,
  VSECPRIM_P2VAR_PARA(actECPOINT) R,
  VSECPRIM_P2CONST_PARA(actECCURVE) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actECPSimMult()
 *********************************************************************************************************************/
/*! \brief          This routine calculates R = k1*Q1 + k2*Q2 (two scalar multiplications incl. addition).
 *  \details        The in- and output point(s) is(are) in Montgomery Representation.
 *                  Implementation Reference: MOV'97, algo 14.88
 *                  (MOV '97: Menezes, van Oorschot, Vanstone - Handbook of Applied Cryptography)
 *  \param[in]      Q1           the point to be multiplied with k1
 *  \param[in]      Q2           the point to be multiplied with k2
 *  \param[in]      k1           first scalar
 *  \param[in]      k2           second scalar
 *  \param[in,out]  Q12          the point to store Q1 + Q2
 *  \param[in,out]  R            (in) the point to store the result
 *                               (out) the point k1*Q1 + k2*Q2 in Montgomery Representation (XR, YR, ZR)
 *  \param[in,out]  Curve        (in) the underlying curve
 *                               (out) the underlying curve; the temporary variables referenced by the member t
 *                                     may have changed
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \pre            Provided pointers must be valid.
 *                  Q1 is in affine Montgomery Representation (XR, YR, R)
 *                  Q2 is in affine Montgomery Representation (XR, YR, R)
 *                  Each of the three buffers referenced by Q12 must provide at least p_length actBNDIGITs.
 *                  Each of the three buffers referenced by R must provide at least p_length actBNDIGITs.
 *                  The buffers referenced by Curve->t[0], Curve->t[1], Curve->t[2] and Curve->t[3] each must provide
 *                  at least (p_length+1) actBNDIGITs.
 *                  Note: p_length is stored in Curve.p_field.m_length
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actECPSimMult(
  VSECPRIM_P2CONST_PARA(actECPOINT) Q1,
  VSECPRIM_P2CONST_PARA(actECPOINT) Q2,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) k1,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) k2,
  VSECPRIM_P2VAR_PARA(actECPOINT) Q12,
  VSECPRIM_P2VAR_PARA(actECPOINT) R,
  VSECPRIM_P2CONST_PARA(actECCURVE) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif
# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTECPOINT_H */

/**********************************************************************************************************************
 *  END OF FILE: actECPoint.h
 *********************************************************************************************************************/
