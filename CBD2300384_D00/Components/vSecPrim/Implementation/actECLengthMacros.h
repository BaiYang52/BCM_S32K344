/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2018 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  actECLengthMacros.h
 *        \brief  Macros for Length information for internal use.
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

#ifndef ACTECLENGTHMACROS_H
# define ACTECLENGTHMACROS_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actITypes.h"
# include "actECPoint.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

    /* ----------------------------------------------------------------------
     |  the long number scratch required (in actBNDIGIT units)
     |  is calculated from the context input vars:
     |    p_length, n_length
     --------------------------------------------------------------------- */

    /*  ECC domain parameters       */
    /*    p, R^2 in GF(p)           */
    /*    n, R^2 in GF(n)           */
# define actEC_DOMAIN_WKSP(p_length, n_length)                        ((2u* (p_length)) + (2u* (n_length)))

    /* temporary long numbers of length (max (p_length, n_length) +1) */
# define actEC_TMP_WKSP(p_length, n_length)                           (actEC_BIGNUM_TMP * (actMax ((n_length), (p_length)) +1u))

    /*  Point (projective)          */
    /*   coordinates: X, Y, Z       */
# define actEC_POINT_WKSP(p_length)                                   (3u* (p_length))

    /*  Point (affine)              */
    /*   coordinates: X, Y          */
# define actEC_POINT_AFFINE_WKSP(p_length)                            (2u* (p_length))

    /*  basic ECC work space =                                        ECC domain parameters + temporary long numbers */
# define actEC_BASIC_WKSP(p_length, n_length)                         (actEC_DOMAIN_WKSP ((p_length), (n_length)) + actEC_TMP_WKSP ((p_length), (n_length)))

    /* key-generation work space =                                    basic ECC work space + d + Q + d*G + G (pre-computed X & Y) */
# define actEC_KG_WKSP(p_length, n_length)                            (actEC_BASIC_WKSP ((p_length), (n_length)) + (n_length) + actEC_POINT_WKSP (p_length) + actEC_POINT_AFFINE_WKSP (p_length))

    /*  EC-D/H (prim.) work space =                                   basic ECC work space + d (own private key) + Q (others public key) + d*Q */
# define actEC_DHp_WKSP(p_length, n_length)                           (actEC_BASIC_WKSP ((p_length), (n_length)) + (n_length) + actEC_POINT_AFFINE_WKSP (p_length) + actEC_POINT_WKSP (p_length))

    /*  EC-D/H work space =                                           D/H (prim) work space + common secret */
# define actEC_DH_WKSP(p_length, n_length)                            (actEC_DHp_WKSP((p_length), (n_length)) + (p_length))

    /*  signature work space =                                        basic ECC work space + [e, d, r, s] + k + Q = (k * G) + G (pre-computed X & Y) */
# define actEC_DSASp_WKSP(p_length, n_length)                         (actEC_BASIC_WKSP ((p_length), (n_length)) + (4u*(n_length)) + (n_length) + actEC_POINT_WKSP (p_length) + actEC_POINT_AFFINE_WKSP (p_length))

    /*  verification work space =                                     basic ECC work space + [e, r, s] + Q + [u1, u2] + G + (G+Q), (u1*G + u2*Q) */
# define actEC_DSAVp_WKSP(p_length, n_length)                         (actEC_BASIC_WKSP ((p_length), (n_length)) + (3u*(n_length)) + actEC_POINT_AFFINE_WKSP (p_length) + (2u*((n_length) +1u)) + actEC_POINT_AFFINE_WKSP (p_length) +  (2u*actEC_POINT_WKSP (p_length)))

    /*  EC-B/D work space =                                           basic ECC work space + ai + (Zi = ai * G / also used for K) + (Xi = (Zi(r) - Zi (l))) + TP (temp. point) */
# define actEC_BDKA_WKSP(p_length, n_length)                          (actEC_BASIC_WKSP ((p_length), (n_length)) + (n_length) + actEC_POINT_WKSP (p_length) +  actEC_POINT_WKSP (p_length) +  actEC_POINT_WKSP (p_length))

# ifdef __cplusplus /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

#endif /*  ACTECLENGTHMACROS_H  */
/**********************************************************************************************************************
 *  END OF FILE: actECLengthMacros.h
 *********************************************************************************************************************/

