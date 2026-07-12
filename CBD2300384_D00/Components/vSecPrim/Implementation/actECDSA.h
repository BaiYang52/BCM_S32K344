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
/*!        \file  actECDSA.h
 *        \brief  An ECDSA verification arithmetic interface.
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

#ifndef ACTECDSA_H
# define ACTECDSA_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actIECDSA.h"
# include "actECPoint.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C" {
# endif

/**********************************************************************************************************************
 *  actECDSAvp()
 *********************************************************************************************************************/
/*! \brief       This function implements the ECDSA verification protocol arithmetic.
 *  \details     -
 *  \param[in]     e         the signed hash value
 *  \param[in]     r         signature r
 *  \param[in]     s         signature s
 *  \param[in]     Q         affine public key (Q = d*G, where d is the private key, Q = (Qx, Qy, 1))
 *  \param[in,out] wksp_ptr  (in)  workspace with buffers for for u1, u2, the base point G and auxiliar variables
 *                             to store G+Q, u1*G+u2*Q
 *                           (out) workspace
 *  \param[in]     watchdog  Watchdog function pointer
 *  \return      actEXCEPTION_PUBKEY     if Q is not on curve
 *               actSIGNATURE_INVALID    if signature (r, s) is invalid
 *               actEXCEPTION_UNKNOWN    internal error (result point
 *                                       not on curve)
 *               actOK                   else
 *  \pre         All provided pointers must be valid.
 *               Each of the buffers referenced by Q must provide at least p_len actBNDIGITs.
 *               The buffers referenced by wksp must provide at least the following lengths:
 *               var1, var2: n_length + 1,
 *               point0_x, point0_y, point0_z, point1_x, point1_y, point2_x, point2_y, point2_z: p_length
 *               Note: p_length and n_length are stored in wksp_ptr->Curve.p_field.m_length and
 *               wksp_ptr->Curve.n_field.m_length, respectively.
 *  \note        After the function has finished, the memory referenced by Q stores the public key in Montgomery
 *               Representation
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Curve->p_field->prime_structure==curveid, Curve->n_field->prime_structure == 0
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECDSAvp(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) e,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) s,
  VSECPRIM_P2CONST_PARA(actECPOINT) Q,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp_ptr,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actECDSAsp()
 *********************************************************************************************************************/
/*! \brief       This function implements the ECDSA signature protocol arithmetic.
 *  \details     -
 *  \param[in]     e          the hash value to be signed
 *  \param[in]     d          the private key of the key pair (d, Q)
 *  \param[in,out] r          (in)  buffer to store signature r
 *                            (out) signature r
 *  \param[in,out] s          (in)  buffer to store signature s
 *                            (out) signature s
 *  \param[in,out] wksp       (in)  workspace with buffers for for k, a precomputed point used during the computation
 *                                  of k[G] and an auxiliary variable to store the result k[G]
 *                            (out) workspace
 *  \param[in]     watchdog   Watchdog function pointer
 *  \return      actEXCEPTION_UNKNOWN internal error, result point not on curve
 *               actOK else
 *  \pre         All provided pointers must be valid.
 *               The buffers referenced by r and s muste provide at least n_length actBNDIGITs.
 *               The buffers referenced by wksp must provide at least the following lengths:
 *               public_key_x, public_key_y, point_0_z, point1_x, point1_y: p_length;  var2: n_length
 *               Note: p_length and n_length are stored in wksp_ptr->Curve.p_field.m_length and
 *               wksp_ptr->Curve.n_field.m_length, respectively.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Curve->p_field->prime_structure==curveid, Curve->n_field->prime_structure==0
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECDSAsp(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) e,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) d,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) s,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTECDSA_H */

/**********************************************************************************************************************
 *  END OF FILE: actECDSA.h
 *********************************************************************************************************************/
