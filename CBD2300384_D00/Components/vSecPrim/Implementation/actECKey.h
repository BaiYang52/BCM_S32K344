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
/*!        \file  actECKey.h
 *        \brief  Interface for ECC key generation arithmetic.
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

#ifndef ACTECKEY_H
# define ACTECKEY_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actIECKey.h"
# include "actECPoint.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/**********************************************************************************************************************
 *  actECkgp()
 *********************************************************************************************************************/
/*! \brief          This function calculates Q = d * G.
 *  \details        -
 *  \param[in]      d            the private key
 *  \param[in,out]  Q            (in) buffer to store the public key
 *                               (out) the public key (d*G)
 *  \param[in,out]  wksp_ptr     (in)  workspace for G, containing the underlying curve
 *                               (out) workspace
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \return         actEXCEPTION_UNKNOWN    internal error (result point not on curve)
 *                  actOK                   else
 *  \pre            Each of the three buffers referenced by Q must provide at least p_length actBNDIGITs, where
 *                  p_length is the length of the prime p of the prime field p over which the Curve is defined
 *                  The buffers referenced by Curve->t[0..(i-1)], where i = actEC_BIGNUM_TMP, each must provide
 *                  at least (max_length+1) actBNDIGITs, max_length = max(p_length, n_length), n_length is the length
 *                  of the order of the base point G
 *                  The buffers referenced by wksp_ptr->point0_y and wksp_ptr->point0_z must provide at least p_length
 *                  actBNDIGITs each.
 *                  Note: p_length and n_length are stored in wksp_ptr->Curve.p_field.m_length and
 *                  wksp_ptr->Curve.n_field.m_length, respectively.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECkgp(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) d,
  VSECPRIM_P2VAR_PARA(actECPOINT) Q,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp_ptr,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
    } /* extern "C" */
# endif
# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTECKEY_H */

/**********************************************************************************************************************
 *  END OF FILE: actECKey.h
 *********************************************************************************************************************/
