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
/*!       \file  act25519util.c
 *       \brief  Curve25519 utilities
 *
 *      \details This file is part of the embedded systems library cvActLib.
 *               Implements the following RFCs:
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08
 *               https:/ /tools.ietf.org/html/rfc7748
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

#define ACT25519UTIL_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "act25519util.h"
#include "actWatchdog.h"
#include "actUtilities.h"

#if (VSECPRIM_ACT25519UTIL_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* prime:
 * p = 2^255 - 19
 */
VSECPRIM_ROM(VSECPRIM_LOCAL, actBNDIGIT) curvePrime[BNDIGITS_256] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.7 */
{
  DIGITs(0xFFFFFFEDuL), DIGITs(0xFFFFFFFFuL), DIGITs(0xFFFFFFFFuL), DIGITs(0xFFFFFFFFuL),
  DIGITs(0xFFFFFFFFuL), DIGITs(0xFFFFFFFFuL), DIGITs(0xFFFFFFFFuL), DIGITs(0x7FFFFFFFuL)
};

/* curve constant (Edwards):
 * d = -121665 / 121666 % p
 *   = 37095705934669439343138083508754565189542113879843219016388785533085940283555
 */
VSECPRIM_ROM(VSECPRIM_LOCAL, actBNDIGIT) curveConst[BNDIGITS_256] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.7 */
{
  DIGITs(0x135978A3uL), DIGITs(0x75EB4DCAuL), DIGITs(0x4141D8ABuL), DIGITs(0x00700A4DuL),
  DIGITs(0x7779E898uL), DIGITs(0x8CC74079uL), DIGITs(0x2B6FFE73uL), DIGITs(0x52036CEEuL)
};

/* group order:
 * q = 2^252 + 27742317777372353535851937790883648493
 */
VSECPRIM_ROM(VSECPRIM_LOCAL, actBNDIGIT) groupOrder[BNDIGITS_256] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.7 */
{
  DIGITs(0x5CF5D3EDuL), DIGITs(0x5812631AuL), DIGITs(0xA2F79CD6uL), DIGITs(0x14DEF9DEuL),
  DIGITs(0x00000000uL), DIGITs(0x00000000uL), DIGITs(0x00000000uL), DIGITs(0x10000000uL)                    /*lint !e572 */
};

/* base point in extended twisted Edwards coordinates
 * bX = 15112221349535400772501151409588531511454012693041857206046113283949847762202
 * bY = 46316835694926478169428394003475163141307993866256225615783033603165251855960
 */
VSECPRIM_ROM(VSECPRIM_LOCAL, actPoint) basePoint = /* PRQA S 3218 */ /* MD_VSECPRIM_8.7 */
{
  {DIGITs(0x8F25D51AuL), DIGITs(0xC9562D60uL), DIGITs(0x9525A7B2uL), DIGITs(0x692CC760uL),
    DIGITs(0xFDD6DC5CuL), DIGITs(0xC0A4E231uL), DIGITs(0xCD6E53FEuL), DIGITs(0x216936D3uL)}, /*  X */
  {DIGITs(0x66666658uL), DIGITs(0x66666666uL), DIGITs(0x66666666uL), DIGITs(0x66666666uL),
    DIGITs(0x66666666uL), DIGITs(0x66666666uL), DIGITs(0x66666666uL), DIGITs(0x66666666uL)}, /*  Y */
  {DIGITs(0x00000001uL), DIGITs(0x00000000uL), DIGITs(0x00000000uL), DIGITs(0x00000000uL),                   /*lint !e572 */
    DIGITs(0x00000000uL), DIGITs(0x00000000uL), DIGITs(0x00000000uL), DIGITs(0x00000000uL)}, /*  Z (== 1) */ /*lint !e572 */
  {DIGITs(0xA5B7DDA3uL), DIGITs(0x6DDE8AB3uL), DIGITs(0x775152F5uL), DIGITs(0x20F09F80uL),
    DIGITs(0x64ABE37DuL), DIGITs(0x66EA4E8EuL), DIGITs(0xD78B7665uL), DIGITs(0x67875F0FuL)} /*  T (== X*Y) */
};

/* 2^512 / q       (used in reduction) */
VSECPRIM_ROM(VSECPRIM_LOCAL, actBNDIGIT) gOBarrett[BNDIGITS_256 + 1u] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.7 */
{
  DIGITs(0x0A2C131BuL), DIGITs(0xED9CE5A3uL), DIGITs(0x086329A7uL), DIGITs(0x2106215DuL),
  DIGITs(0xFFFFFFEBuL), DIGITs(0xFFFFFFFFuL), DIGITs(0xFFFFFFFFuL), DIGITs(0xFFFFFFFFuL),
  0x0Fu
};

/* 2* d mod p      (used in point addition) */
VSECPRIM_ROM(VSECPRIM_LOCAL, actBNDIGIT) cCtimes2[BNDIGITS_256] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.7 */
{
  DIGITs(0x26B2F159uL), DIGITs(0xEBD69B94uL), DIGITs(0x8283B156uL), DIGITs(0x00E0149AuL),
  DIGITs(0xEEF3D130uL), DIGITs(0x198E80F2uL), DIGITs(0x56DFFCE7uL), DIGITs(0x2406D9DCuL)
};

/* (p + 3) / 8     (used in sqrt (and inversion)) */
VSECPRIM_ROM(VSECPRIM_LOCAL, actBNDIGIT) prime38[BNDIGITS_256] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.7 */
{
  DIGITs(0xFFFFFFFEuL), DIGITs(0xFFFFFFFFuL), DIGITs(0xFFFFFFFFuL), DIGITs(0xFFFFFFFFuL),
  DIGITs(0xFFFFFFFFuL), DIGITs(0xFFFFFFFFuL), DIGITs(0xFFFFFFFFuL), DIGITs(0x0FFFFFFFuL)
};

/* sqrt (-1) mod p (used in sqrt (and inversion)) */
VSECPRIM_ROM(VSECPRIM_LOCAL, actBNDIGIT) sqrtM1[BNDIGITS_256] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.7 */
{
  DIGITs(0x4A0EA0B0uL), DIGITs(0xC4EE1B27uL), DIGITs(0xAD2FE478uL), DIGITs(0x2F431806uL),
  DIGITs(0x3DFBD7A7uL), DIGITs(0x2B4D0099uL), DIGITs(0x4FC1DF0BuL), DIGITs(0x2B832480uL)
};

# define VSECPRIM_STOP_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
  act25519_reduce()
 **********************************************************************************************************************/
/*! \brief      Barrett reduction
 *  \details    This function performs the operation r = a mod n using Barrett's reduction methode.
 *  \param[in,out]  r  In: Pointer to buffer; Out: Result of operation.
 *  \param[in]  a  Pointer to dividend.
 *  \param[in]  q  Pointer to constant expression q = ((a * m) / 2^512)
 *  \param[in]  n  Pointer to divisor.
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre r has to be at least 2*BNDIGITS_256
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) act25519_reduce(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) q,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) n);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  act25519_reduce()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) act25519_reduce(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) q,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) n)
{
  actU32 d;
  actBNDDIGIT tempBuffer;

  /* (2)   r = q * n = ((a * m) / 2^512) * n
   *
   *    we just calculate the BNDIGITS_256 LSdigits:
   *    * the MSdigits will NOT influence the final result
   *      (which is just BNDIGITS_256 "long")
   *
   */
  actMemset(r, 0u, BNBYTES_256); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  for (d = 0u; d < BNDIGITS_256; d++)
  {
    actU32 j;
    actBNDDIGIT digit;

    digit = n[d];
    tempBuffer = 0u;
    for (j = 0u; j < ((actU32)BNDIGITS_256 - d); j++)
    {
      tempBuffer += ((actBNDDIGIT) q[j]) * digit;
      tempBuffer += r[d + j];
      r[d + j] = actBNLoWord(tempBuffer); /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
      tempBuffer = actBNHiWord(tempBuffer);
    }
  }

  /* (3)   r = a - r = a - (((a * m) / 2^512) * n)
   *
   *    we just calculate the BNDIGITS_256 LSdigits:
   *    * the MSdigits will turn to ZERO anyway!
   */
  actBNSub(a, r, r, BNDIGITS_256); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* (4)   r -= n    (while still too large)
   *
   *    ONE subtraction should be enough:
   *    * just in case we'll repeat this...
   */
  while (actCOMPARE_LARGER == actBNCompare(r, n, BNDIGITS_256)) /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    actBNSub(r, n, r, BNDIGITS_256); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  act25519_initCore()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) act25519_initCore(VSECPRIM_P2VAR_PARA(actCurve25519ws) ws)
{
  VSECPRIM_P2VAR_PARA(actBNRING) ring;

  /* Initialize core variables */
  actMemCpyWordArray(ws->prime, curvePrime, BNDIGITS_256); /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemCpyWordArray(ws->order, groupOrder, BNDIGITS_256); /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemCpyWordArray(ws->orderB, gOBarrett, BNDIGITS_256 + 1u);  /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* Set p-ring */
  ring = &(ws->pRing);
  ring->m = ws->prime; /* SBSW_VSECPRIM_WRITE_POINTER */
  ring->m_length = BNDIGITS_256; /* SBSW_VSECPRIM_WRITE_POINTER */

  /* Set q-ring */
  ring = &(ws->qRing);
  ring->m = ws->order; /* SBSW_VSECPRIM_WRITE_POINTER */
  ring->m_length = BNDIGITS_256; /* SBSW_VSECPRIM_WRITE_POINTER */
}

/**********************************************************************************************************************
 *  act25519_reduceP()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(void) act25519_reduceP(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,  /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a)
{
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a76 = a;
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a2 = a;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) q = ws->auxVar.tempQ;
  actBNDDIGIT tempBuffer = 0u;
  actU8 d;
  actU8 offsetA2 = 0u;
  actU8 offsetQ = 0u;

  /* (1)   q = (a * m) / 2^512, m = mP   (the division is implicit)
   *
   *    we're taking advantage of the structure of m:
   *    * m has just two digits which aren't ZERO
   *    * this holds true for all digit sizes (8, 16, 32)!
   *    -> we multiply just these two digits (2 and 76)
   *       with matching offset (of BNDIGITS_256)
   *
   *    in addition we only store the MSdigits:
   *    -> this implicitly divides the result into 2^512
   */
  for (d = 0u; d < BNDIGITS_256; d++)
  {
    tempBuffer += ((actBNDDIGIT)a76[d]) * 76u;
    tempBuffer = actBNHiWord(tempBuffer);
  }

  for (; d < (2u * (BNDIGITS_256)); d++)
  {
    tempBuffer += ((actBNDDIGIT)a76[d]) * 76u;
    tempBuffer += ((actBNDDIGIT)a2[offsetA2]) * 2u;
    tempBuffer = actBNHiWord(tempBuffer);
    offsetA2++;
  }

  for (; d < (3u * (BNDIGITS_256)); d++)
  {
    tempBuffer += ((actBNDDIGIT)a2[offsetA2]) * 2u;
    q[offsetQ] = actBNLoWord(tempBuffer); /* Overwrite MSdigits of a */ /* SBSW_VSECPRIM_WRITE_ARRAY */
    tempBuffer = actBNHiWord(tempBuffer);
    offsetA2++;
    offsetQ++;
  }

  /* Call act25519_reduce() to perform step 2 - 4 of Barrett reduction */
  act25519_reduce(r, a, ws->auxVar.tempQ, ws->prime); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 *  act25519_reduceQ()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(void) act25519_reduceQ(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a)
{
  actBNDIGIT q[BNDIGITS_512 * 2u];

  /* (1)   q = (a * m) / 2^512, m = mQ   (the division is implicit)
   *
   *    just a plain multiplication (not used that often!)
   *
   *    in addition we only use the MSdigits for further calculation:
   *    -> this implicitly divides the result into 2^512
   *
   *    (memory) optimization potential:
   *    * multiply in three chunks at 256 bits result each
   *      and store just the last one (like: reduceP)
   */
  actBNMult(a, BNDIGITS_512, ws->orderB, BNDIGITS_256 + 1u, q, NULL_PTR); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* Call act25519_reduce() to perform step 2 - 4 of Barrett reduction */
  act25519_reduce(r, a, &q[BNDIGITS_512], ws->order); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 *  act25519_addModP()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) act25519_addModP(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b)
{
  /* r = (a + b) mod p */
  actBNModAdd(a, b, r, &(ws->pRing), NULL_PTR); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 *  act25519_subModP()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) act25519_subModP(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b)
{
  /* r = (a - b) mod p */
  actBNModSub(a, b, r, &(ws->pRing), NULL_PTR); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 *  act25519_mulModP()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(void) act25519_mulModP(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b)
{
  /* c = (a * b) */
  actBNMult(a, BNDIGITS_256, b, BNDIGITS_256, ws->auxVar.temp1, NULL_PTR); /* SBSW_VSECPRIM_CALL_FUNCTION */
  /* r = c mod p */
  act25519_reduceP(ws, r, ws->auxVar.temp1); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 *  act25519_powModP()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) act25519_powModP(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actU8 dig;

  actMemset(r, 0u, BNBYTES_256); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  r[0] = 1u; /* r = 1 */ /* SBSW_VSECPRIM_WRITE_ARRAY */

  /* Iterate over the digits of "b" */
  for (dig = 0u; dig < BNDIGITS_256; dig++)
  {
    actBNDIGIT digit = b[dig];
    actU8 bit;

    /* Iterate over the bits of "digit" */
    for (bit = 0u; bit < actBN_BITS_PER_DIGIT; bit++)
    {
      /* Multiply */
      if ((digit & 0x1u) == TRUE)
      {
        act25519_mulModP(ws, r, a, r);  /* multiply */ /* SBSW_VSECPRIM_CALL_FUNCTION */
      }
      act25519_mulModP(ws, a, a, a);  /* square */ /* SBSW_VSECPRIM_CALL_FUNCTION */
      digit >>= 1u;

      ifWDisL2(0u == (bit % 2u), watchdog); /* every 2..4 multiplications */ /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_POINTER_CALL_WATCHDOG */
    }
  }
}

/**********************************************************************************************************************
 *  act25519_invModP()
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
VSECPRIM_FUNC(void) act25519_invModP(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* Copy data into local buffer */
  actMemCpyWordArray(ws->auxVar.tempE, a, BNDIGITS_256); /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemCpyWordArray(ws->auxVar.tempF, ws->prime, BNDIGITS_256); /* SBSW_VSECPRIM_CALL_FUNCTION */
  /* Calculate exponent b = p - 2 */
  ws->auxVar.tempF[0] -= 2u; /* SBSW_VSECPRIM_WRITE_ARRAY */

  /* r = (a ^ b) mod p */
  act25519_powModP(ws, r, ws->auxVar.tempE, ws->auxVar.tempF, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 *  act25519_sqrtModP()
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
VSECPRIM_FUNC(actU8) act25519_sqrtModP(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actU8 retVal = FALSE;
  actMemCpyWordArray(ws->auxVar.tempG, a, BNDIGITS_256); /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemCpyWordArray(ws->auxVar.tempH, prime38, BNDIGITS_256); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* #10 candidate: r = a ^ ((p + 3) / 8) mod p */
  act25519_powModP(ws, ws->auxVar.tempE, ws->auxVar.tempG, ws->auxVar.tempH, watchdog); /* tempE = candidate */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  act25519_mulModP(ws, ws->auxVar.tempF, ws->auxVar.tempE, ws->auxVar.tempE); /* square */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  if (TRUE == actMemcmp(ws->auxVar.tempF, a, BNBYTES_256)) /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  { /* found! */
    actMemCpyWordArray(r, ws->auxVar.tempE, BNDIGITS_256); /* SBSW_VSECPRIM_CALL_FUNCTION */

    actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
    retVal = TRUE;
  }

  if (retVal == FALSE)
  {
    /* #30 multiply by -1 */
    actMemCpyWordArray(ws->auxVar.tempH, sqrtM1, BNDIGITS_256); /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_mulModP(ws, ws->auxVar.tempE, ws->auxVar.tempE, ws->auxVar.tempH); /* tempE = candidate * -1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */

    /* #40 square; if identical -> sqrt found */
    act25519_mulModP(ws, ws->auxVar.tempF, ws->auxVar.tempE, ws->auxVar.tempE); /* square */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    if (TRUE == actMemcmp(ws->auxVar.tempF, a, BNBYTES_256)) /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    { /* found! */
      actMemCpyWordArray(r, ws->auxVar.tempE, BNDIGITS_256); /* SBSW_VSECPRIM_CALL_FUNCTION */

      actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
      retVal = TRUE;
    }
  }

  actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  return retVal;
}

/**********************************************************************************************************************
 * act25519_addModQ()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(void) act25519_addModQ(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b)
{
  /* Big number modular addition */
  actBNModAdd(a, b, r, &(ws->qRing), NULL_PTR); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 * act25519_mulModQ()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) act25519_mulModQ(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b)
{
  /* r = (a * b) mod q */
  /* Big number multiplication c = a * b */
  actBNMult(a, BNDIGITS_256, b, BNDIGITS_256, ws->auxVar.temp1, NULL_PTR); /* SBSW_VSECPRIM_CALL_FUNCTION */
  /* Perform Barrett reduction for r = c mod q */
  act25519_reduceQ(ws, r, ws->auxVar.temp1); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 * act25519_encodePoint()
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
VSECPRIM_FUNC(void) act25519_encodePoint(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actU8) buffer,
  VSECPRIM_P2CONST_PARA(actPoint) point,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actMemCpyWordArray(ws->auxVar.tempA, point->z, BNDIGITS_256); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* Transform point projective -> affine coordinates */
  act25519_invModP(ws, ws->auxVar.tempA, point->z, watchdog);  /* A = 1 / z */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_mulModP(ws, ws->auxVar.tempB, point->x, ws->auxVar.tempA); /* x / z = x * A */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_mulModP(ws, ws->auxVar.tempC, point->y, ws->auxVar.tempA); /* y / z = y * A */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* MSBit (y) = lsBit (x) */
  /* MSbit (y) will be ZERO, because p = 2^255 -19 */
  ws->auxVar.tempC[BNDIGITS_256 - 1u] |= ws->auxVar.tempB[0] << (actBN_BITS_PER_DIGIT - 1u); /* SBSW_VSECPRIM_WRITE_ARRAY */

  /* Store as little endian byte array */
  actBNStoreLE(buffer, ws->auxVar.tempC, BNDIGITS_256); /* SBSW_VSECPRIM_CALL_FUNCTION */

  actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
}

/**********************************************************************************************************************
 *  act25519_decodePoint()
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
 */
VSECPRIM_FUNC(actRETURNCODE) act25519_decodePoint(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actPoint) point,
  VSECPRIM_P2CONST_PARA(actU8) buffer,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actU8 x_0;
  actU8 d;
  actBNDIGIT xNOTzero = 0u;
  actRETURNCODE retVal = actOK;

  /* #10 Load from LE byte array */
  actBNLoadLE(point->y, buffer, BNBYTES_256); /* SBSW_VSECPRIM_CALL_FUNCTION */
  /* #11 x_0 = lsBit (x) = MSBit (y) */
  x_0 = (actU8)((point->y[BNDIGITS_256 - 1u] >> (actBN_BITS_PER_DIGIT - 1u)) & 0xFFu); /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
  /* #12 y = clear MSBit (y) */
  point->y[BNDIGITS_256 - 1u] &= (actBNDIGIT)MASK_MSBit; /* SBSW_VSECPRIM_WRITE_ARRAY */
  /* #13 z = 1 */
  actMemset(point->z, 0u, sizeof(point->z)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  point->z[0] = 1u; /* SBSW_VSECPRIM_WRITE_ARRAY */

  /* #20 Evaluate curve equation to get x^2 */
  /* #21 x^2 = (u/v) = (y^2 -1) / (d * y^2 +1) (mod p) */
  act25519_mulModP(ws, ws->auxVar.tempD, point->y, point->y); /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_subModP(ws, ws->auxVar.tempA, ws->auxVar.tempD, point->z); /* tempA = u = y^2 -1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemCpyWordArray(ws->auxVar.tempC, curveConst, BNDIGITS_256); /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_mulModP(ws, ws->auxVar.tempB, ws->auxVar.tempD, ws->auxVar.tempC); /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_addModP(ws, ws->auxVar.tempB, ws->auxVar.tempB, point->z); /* tempB = v = d * y^2 +1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* #22 x = sqrt (u/v) (mod p) */
  act25519_invModP(ws, ws->auxVar.tempC, ws->auxVar.tempB, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_mulModP(ws, ws->auxVar.tempD, ws->auxVar.tempA, ws->auxVar.tempC);  /* tempD = (u/v) = x^2 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  if (FALSE == act25519_sqrtModP(ws, point->x, ws->auxVar.tempD, watchdog)) /* p.x = x */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    retVal = actEXCEPTION_UNKNOWN; /* NO square-root found */
  }
  else
  {
    for (d = 0u; d < BNDIGITS_256; ++d)
    {
      xNOTzero |= point->x[d];
    }

    if ((xNOTzero == 0x0u) && (x_0 == 0x1u)) /* (x == 0 && x_0) ? */
    {
      retVal = actEXCEPTION_UNKNOWN; /* NOT a valid point */
    }
    else
    {
      /* #30 If not a valid point -> x = (0 - x) (mod p) */
      if (x_0 != (point->x[0] & 1u))
      { /* (x_0 != lsBit (x)) ? */
        actMemset(ws->auxVar.tempB, 0u, BNBYTES_256); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
        act25519_subModP(ws, point->x, ws->auxVar.tempB, point->x); /* x = (0 - x) (mod p) */ /* SBSW_VSECPRIM_CALL_FUNCTION */
      }

      /* #40 t = x * y */
      act25519_mulModP(ws, point->t, point->x, point->y); /* SBSW_VSECPRIM_CALL_FUNCTION */
    }
  }

  actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  return retVal;
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  act25519_addPoint()
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
 */
VSECPRIM_FUNC(void) act25519_addPoint(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actPoint) r, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2CONST_PARA(actPoint) p,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* #10 D = 2*d (curve constant) */
  actMemCpyWordArray(ws->auxVar.tempD, cCtimes2, BNDIGITS_256); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* #20 A = (p.y - p.x) * (q.y - q.x) */
  act25519_subModP(ws, ws->auxVar.tempA, r->y, r->x); /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_subModP(ws, ws->auxVar.tempC, p->y, p->x); /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_mulModP(ws, ws->auxVar.tempA, ws->auxVar.tempA, ws->auxVar.tempC); /* SBSW_VSECPRIM_CALL_FUNCTION */
  /* #30 B = (p.y + p.x) * (q.y + q.x) */
  act25519_addModP(ws, ws->auxVar.tempB, r->y, r->x); /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_addModP(ws, ws->auxVar.tempC, p->y, p->x); /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_mulModP(ws, ws->auxVar.tempB, ws->auxVar.tempB, ws->auxVar.tempC); /* SBSW_VSECPRIM_CALL_FUNCTION */
  /* #40 C = p.t * 2*d * q.t */
  act25519_mulModP(ws, ws->auxVar.tempC, r->t, ws->auxVar.tempD); /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_mulModP(ws, ws->auxVar.tempC, ws->auxVar.tempC, p->t); /* SBSW_VSECPRIM_CALL_FUNCTION */
  /* #50 D = p.z * 2 * q.z */
  act25519_mulModP(ws, ws->auxVar.tempD, r->z, p->z); /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_addModP(ws, ws->auxVar.tempD, ws->auxVar.tempD, ws->auxVar.tempD); /* SBSW_VSECPRIM_CALL_FUNCTION */

  actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* #60 E = B - A */
  act25519_subModP(ws, r->t, ws->auxVar.tempB, ws->auxVar.tempA); /* SBSW_VSECPRIM_CALL_FUNCTION */
  /* #70 F = D - C */
  act25519_subModP(ws, r->z, ws->auxVar.tempD, ws->auxVar.tempC); /* SBSW_VSECPRIM_CALL_FUNCTION */
  /* #80 G = D + C */
  act25519_addModP(ws, ws->auxVar.tempD, ws->auxVar.tempD, ws->auxVar.tempC); /* SBSW_VSECPRIM_CALL_FUNCTION */
  /* #90 H = B + A */
  act25519_addModP(ws, ws->auxVar.tempB, ws->auxVar.tempB, ws->auxVar.tempA); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* #100 p.x = E * F */
  act25519_mulModP(ws, r->x, r->t, r->z); /* SBSW_VSECPRIM_CALL_FUNCTION */
  /* #110 p.y = G * H */
  act25519_mulModP(ws, r->y, ws->auxVar.tempD, ws->auxVar.tempB); /* SBSW_VSECPRIM_CALL_FUNCTION */
  /* #120 p.z = F * G */
  act25519_mulModP(ws, r->z, r->z, ws->auxVar.tempD); /* SBSW_VSECPRIM_CALL_FUNCTION */
  /* #130 p.t = E * H */
  act25519_mulModP(ws, r->t, r->t, ws->auxVar.tempB); /* SBSW_VSECPRIM_CALL_FUNCTION */

  actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
}

/**********************************************************************************************************************
 *  act25519_mulPoint()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) act25519_mulPoint(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actPoint) r,
  VSECPRIM_P2VAR_PARA(actPoint) p,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) s,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* Initialize the result to (0, 1, 1, 0), the neutral element of the group. */
  actMemset(r, 0u, sizeof(actPoint)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  r->y[0] = 1u; /* SBSW_VSECPRIM_WRITE_ARRAY */
  r->z[0] = 1u; /* SBSW_VSECPRIM_WRITE_ARRAY */
  /* actPoint addition */
  act25519_mulAddPoint(ws, r, p, s, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 *  act25519_mulAddPoint()
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
VSECPRIM_FUNC(void) act25519_mulAddPoint(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actPoint) r,
  VSECPRIM_P2VAR_PARA(actPoint) p,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) s,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actU8 dig;

  /* Iterate over the digits of "s" */
  for (dig = 0u; dig < BNDIGITS_256; dig++)
  {
    actU8 bit;
    actBNDIGIT digit = s[dig];

    /* Iterate over the bits of "digit" */
    for (bit = 0; bit < actBN_BITS_PER_DIGIT; bit++)
    {
      if ((digit & 0x1u) == TRUE)
      {
        act25519_addPoint(ws, r, p, watchdog);  /* add */ /* SBSW_VSECPRIM_CALL_FUNCTION */
      }
      act25519_addPoint(ws, p, p, watchdog);  /* double */ /* SBSW_VSECPRIM_CALL_FUNCTION */
      digit >>= 1;
    }
  }
}

/**********************************************************************************************************************
 *  act25519_mulBasePoint()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) act25519_mulBasePoint(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2VAR_PARA(actPoint) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) s,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actPoint p;

  /* Multiply base point */
  actMemCpyByteArray(&p, &basePoint, sizeof(basePoint)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_mulPoint(ws, r, &p, s, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 *  act25519_comparePoints()
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
VSECPRIM_FUNC(actRETURNCODE) act25519_comparePoints(
  VSECPRIM_P2VAR_PARA(actCurve25519ws) ws,
  VSECPRIM_P2CONST_PARA(actPoint) p,
  VSECPRIM_P2CONST_PARA(actPoint) q,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE result = actEXCEPTION_UNKNOWN;

  /* Compare x and y coordinates */
  act25519_mulModP(ws, ws->auxVar.tempA, p->x, q->z); /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_mulModP(ws, ws->auxVar.tempB, q->x, p->z); /* SBSW_VSECPRIM_CALL_FUNCTION */

  if (TRUE == actMemcmp(ws->auxVar.tempA, ws->auxVar.tempB, BNBYTES_256)) /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    act25519_mulModP(ws, ws->auxVar.tempA, p->y, q->z); /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_mulModP(ws, ws->auxVar.tempB, q->y, p->z); /* SBSW_VSECPRIM_CALL_FUNCTION */

    if (TRUE == actMemcmp(ws->auxVar.tempA, ws->auxVar.tempB, BNBYTES_256)) /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    {
      result = actOK;
    }
  }

  actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  return result;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACT25519UTIL_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: act25519util.c
 *********************************************************************************************************************/
