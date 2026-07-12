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
/*!        \file  act25519util.h
 *        \brief  Curve25519 utilities
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

#ifndef ACT25519UTIL_H
# define ACT25519UTIL_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actBigNum.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  act25519_initCore()
 *********************************************************************************************************************/
/*! \brief       Init core x25519.
 *  \details     This function initializes the core x22519 data structure.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \pre         ws must be a valid pointer.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act25519_initCore(VSECPRIM_P2VAR_PARA(actCurve25519ws) ws);

/**********************************************************************************************************************
 *  act25519_reduceP()
 *********************************************************************************************************************/
/*! \brief       Reduce P
 *  \details     This function reduces P.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r  Pointer to r.
 *  \param[in]  a  Pointer to a.
 *  \pre         Pointers must be valid.
 *               r must have at least size of BNDIGITS_256.
 *               a must have at least size of BNDIGITS_256.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act25519_reduceP(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a);

/**********************************************************************************************************************
 *  act25519_reduceQ()
 *********************************************************************************************************************/
/*! \brief       Reduce Q
 *  \details     This function reduces Q.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r  Pointer to r.
 *  \param[in]  a  Pointer to a.
 *  \pre         Pointers must be valid.
 *               r must have at least size of 2*BNDIGITS_256.
 *               a must have at least size of BNDIGITS_256 + 1
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act25519_reduceQ(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a);

/**********************************************************************************************************************
 *  act25519_addModP()
 *********************************************************************************************************************/
/*! \brief       r = (a + b) mod p
 *  \details     This function performs addition in a finite field.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r  Pointer to result.
 *  \param[in]  a  Pointer to a.
 *  \param[in]  b  Pointer to b.
 *  \pre         Pointers must be valid.
 *               r must have at least size BNDIGITS_256.
 *               a must have at least size of BNDIGITS_256.
 *               b must have at least size of BNDIGITS_256.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act25519_addModP(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b);

/**********************************************************************************************************************
 *  act25519_subModP()
 *********************************************************************************************************************/
/*! \brief       r = (a - b) mod p
 *  \details     This function perform subtraction in a finite field.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r  Pointer to result.
 *  \param[in]  a  Pointer to a.
 *  \param[in]  b  Pointer to b.
 *  \pre         Pointers must be valid.
 *               r must have at least size BNDIGITS_256.
 *               a must have at least size of BNDIGITS_256.
 *               b must have at least size of BNDIGITS_256.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act25519_subModP(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b);

/**********************************************************************************************************************
 *  act25519_mulModP()
 *********************************************************************************************************************/
/*! \brief       r = (a * b) mod p
 *  \details     This function perform multiplication in a finite field.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r  Pointer to result.
 *  \param[in]  a  Pointer to a.
 *  \param[in]  b  Pointer to b.
 *  \pre         Pointers must be valid.
 *               r must have at least size BNDIGITS_256.
 *               a must have at least size of BNDIGITS_256.
 *               b must have at least size of BNDIGITS_256.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act25519_mulModP(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b);

/**********************************************************************************************************************
 *  act25519_powModP()
 *********************************************************************************************************************/
/*! \brief       r = (a ^ b) mod p
 *  \details     This function exponentiate base a with exponent b in a finite field.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r  Pointer to result.
 *  \param[in]  a  Pointer to a.
 *  \param[in]  b  Pointer to b.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \pre         Pointers must be valid.
 *               r must have at least size BNDIGITS_256.
 *               a must have at least size of BNDIGITS_256.
 *               b must have at least size of BNDIGITS_256.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act25519_powModP(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act25519_invModP()
 *********************************************************************************************************************/
/*! \brief       r = (1 / a) mod p
 *  \details     This function performs inversion in a finite field. According to
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.1
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r  Pointer to result.
 *  \param[in]  a  Pointer to a.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \pre         Pointers must be valid.
 *               r must have at least size BNDIGITS_256.
 *               a must have at least size of BNDIGITS_256.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act25519_invModP(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act25519_sqrtModP()
 *********************************************************************************************************************/
/*! \brief       r = a ^ ((p + 3) / 8) mod p
 *  \details     This function performs squaring in a finite field. According to
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.3
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r  Pointer to result.
 *  \param[in]  a  Pointer to a.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \pre         Pointers must be valid.
 *               r must have at least size BNDIGITS_256.
 *               a must have at least size of BNDIGITS_256.
 *  \return      TRUE - Found
 *               FALSE - Not Found
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actU8) act25519_sqrtModP(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act25519_addModQ()
 *********************************************************************************************************************/
/*! \brief       r = (a + b) mod q
 *  \details     This function performs addition in a finite field.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r  Pointer to result.
 *  \param[in]  a  Pointer to a.
 *  \param[in]  b  Pointer to b.
 *  \pre         Pointers must be valid.
 *               r must have at least size BNDIGITS_256.
 *               a must have at least size of BNDIGITS_256.
 *               b must have at least size of BNDIGITS_256.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act25519_addModQ(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b);

/**********************************************************************************************************************
 *  act25519_mulModQ()
 *********************************************************************************************************************/
/*! \brief       r = (a * b) mod q
 *  \details     This function perform multiplication in a finite field.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r  Pointer to result.
 *  \param[in]  a  Pointer to a.
 *  \param[in]  b  Pointer to b.
 *  \pre         Pointers must be valid.
 *               r must have at least size BNDIGITS_256.
 *               a must have at least size of BNDIGITS_256.
 *               b must have at least size of BNDIGITS_256.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act25519_mulModQ(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b);

/**********************************************************************************************************************
 *  act25519_encodePoint()
 *********************************************************************************************************************/
/*! \brief       Encode point.
 *  \details     This function encodes a given point according to
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.2
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  buffer  Pointer to buffer where the encoded point shall be stored.
 *  \param[in]  point  Pointer to the point on the curve.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \pre         Pointers must be valid.
 *               buffer must have at least size BNDIGITS_256
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act25519_encodePoint(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actU8) buffer,
  VSECPRIM_P2CONST_PARA(actPoint) point,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act25519_decodePoint()
 *********************************************************************************************************************/
/*! \brief       Decode point.
 *  \details     This function decodes a given point according to
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.3
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  point  Pointer where the curve point shall be stored.
 *  \param[in]  buffer  Pointer to the encrypted point.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \pre         Pointers must be valid.
 *               buffer must have at least size BNDIGITS_256
 *  \return      actOK - success
 *               actEXCEPTION_UNKNOWN - No square root found or point not valid
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) act25519_decodePoint(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actPoint) point,
  VSECPRIM_P2CONST_PARA(actU8) buffer,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act25519_addPoint()
 *********************************************************************************************************************/
/*! \brief       actPoint addition.
 *  \details     This function performs point addition. According to:
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.4
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r  Pointer to the first point.
 *  \param[in]  p  Pointer to point which shall be added.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \pre         Pointers must be valid.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act25519_addPoint(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actPoint) r,
  VSECPRIM_P2CONST_PARA(actPoint) p,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act25519_mulPoint()
 *********************************************************************************************************************/
/*! \brief       actPoint multiplication.
 *  \details     This function performs point multiplication. According to:
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.4
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r  Pointer to the result.
 *  \param[in]  p  Pointer to point which shall be multiplied.
 *  \param[in]  s  Pointer to scalar which shall be used for multiplication.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \pre         Pointers must be valid.
 *               s must have the size BNDIGITS_256
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act25519_mulPoint(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actPoint) r,
  VSECPRIM_P2VAR_PARA(actPoint) p,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) s,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act25519_mulAddPoint()
 *********************************************************************************************************************/
/*! \brief       actPoint multiplication by point addition.
 *  \details     This function performs point multiplication by point addition. According to:
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.4
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r  Pointer to the result.
 *  \param[in]  p  Pointer to point which shall be multiplied.
 *  \param[in]  s  Pointer to scalar which shall be used for multiplication.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \pre         Pointers must be valid.
 *               s must have the size BNDIGITS_256
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act25519_mulAddPoint(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actPoint) r,
  VSECPRIM_P2VAR_PARA(actPoint) p,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) s,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act25519_mulBasePoint()
 *********************************************************************************************************************/
/*! \brief       Multiply base point.
 *  \details     This function performs multiplication of base point with given scalar.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r  Pointer to the result.
 *  \param[in]  s  Pointer to scalar which shall be used for multiplication.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \pre         Pointers must be valid.
 *               s must have the size BNDIGITS_256
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act25519_mulBasePoint(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actPoint) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) s,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act25519_comparePoints()
 *********************************************************************************************************************/
/*! \brief       Compare give points.
 *  \details     This function compares the given points.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r  Pointer to the result.
 *  \param[in]  s  Pointer to scalar which shall be used for multiplication.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \return      actOK  Points are identical.
 *               actEXCEPTION_UNKNOWN  Points are not identical.
 *  \pre         Pointers must be valid.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) act25519_comparePoints(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2CONST_PARA(actPoint) p,
  VSECPRIM_P2CONST_PARA(actPoint) q,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
}
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACT25519UTIL_H */

/**********************************************************************************************************************
 *  END OF FILE: act25519util.h
 *********************************************************************************************************************/
