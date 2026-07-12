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
/*!        \file  actECDH.h
 *        \brief  An ECDH arithmetic interface.
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

/****************************************************************************
 **
 ** This file contains: An ECDH arithmetic interface.
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

#ifndef ACTECDH_H
# define ACTECDH_H

# include "actIECDH.h"
# include "actECPoint.h"

/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C" {
# endif

/**********************************************************************************************************************
 *  actECDHp()
 *********************************************************************************************************************/
/*! \brief          This function implements the ECDH protocol arithmetic.
 *  \details        -
 *  \param[in]      d            my private key                                 n_length
 *  \param[in]      Q            your (affine) public key                       2*p_length
 *  \param[in,out]  point        (in) buffer to store d*Q                       3*p_length
 *                               (out) d*Q                                      3*p_length
 *  \param[in]      Curve        the underlying curve (see assumptions)
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \pre            -
 *  \return         actEXCEPTION_PUBKEY     if Q2 is not on curve
 *                  actEXCEPTION_UNKNOWN    internal error (result point
 *                  not on curve)
 *                  actOK                   else
 *  \note           The actECCURVE structure parameter 'Curve' holds all necessary
 *                  information and the workspace. It has to be initialized as far as
 *                  listed below. Please have a look at the actECCURVE definition in
 *                  actECPoint.h; an example for the usage of this function is the
 *                  actECDSAVerify(.) implementation in actIECDSA.c, which is basically
 *                  a call of actECDSAvp(.) with all previous initializations.
 *                  Parameters of Curve:
 *                  Curve->p_field: the underlying field GF(p):
 *                         p_field->m:        the modulus p               p_length
 *                         p_field->m_length: equals to p_length
 *                         p_field->RR:       constant for Montgomery     p_length
 *                         p_field->m_bar:    constant for Montgomery     1
 *                         p_field->prime_structure==curveid
 *                         p_field->window_size for modular exponentiation
 *                  Curve->n_field: the field GF(n):
 *                         n_field->m_length: equals to n_length
 *                  Curve->b_R:    coefficient b of curve equation        p_bytes
 *                  Curve->t[0..(i-1)]:  i temporary BigNum variables,   i*(max_length+1)
 *                                        where i = actEC_BIGNUM_TMP
 *                        [ Notation: max_length = max(p_length, n_length) ]
 *                  Q is in affine representation (X, Y, 1)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECDHp(VSECPRIM_P2CONST_PARA(actBNDIGIT) d, VSECPRIM_P2CONST_PARA(actECPOINT) Q,
   VSECPRIM_P2VAR_PARA(actECPOINT) point, VSECPRIM_P2VAR_PARA(actECCURVE) Curve,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTECDH_H */

/**********************************************************************************************************************
 *  END OF FILE: actECDH.h
 *********************************************************************************************************************/
