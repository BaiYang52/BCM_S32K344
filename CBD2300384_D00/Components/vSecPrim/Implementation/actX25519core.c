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
/*!        \file  actX25519core.c
 *        \brief  Core X25519 implementation.
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

#define ACTX25519CORE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actX25519core.h"
#include "actWatchdog.h"
#include "actUtilities.h"
#include "actBigNum.h"

#if (VSECPRIM_ACTX25519CORE_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* (A - 2) / 4     (used in Curve25519 evaluation)
 * A (= 486662) is the Montgomery curve constant
 */
VSECPRIM_ROM(VSECPRIM_LOCAL, actBNDIGIT) a24[BNDIGITS_256] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.7 */
{
  DIGITs(0x0001DB41uL), DIGITs(0x00000000uL), DIGITs(0x00000000uL), DIGITs(0x00000000uL),     /*lint !e572 */
  DIGITs(0x00000000uL), DIGITs(0x00000000uL), DIGITs(0x00000000uL), DIGITs(0x00000000uL)      /*lint !e572 */
};

# define VSECPRIM_STOP_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actX25519_cswap()
 **********************************************************************************************************************/
/*! \brief      Conditional swap function
 *  \details    This function implements the conditional swap function defined in RFC 7748 Chapter 5.
 *  \param[in]  swap  input for mask
 *  \param[in,out]  a  pointer to a
 *  \param[in,out]  b  pointer to b
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        a, b must be valid pointers
 *              a, b must be of size BNBYTES_256
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actX25519_cswap(actU32 swap, VSECPRIM_P2VAR_PARA(actBNDIGIT) a, VSECPRIM_P2VAR_PARA(actBNDIGIT) b);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actX25519_cswap()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actX25519_cswap(actU32 swap, VSECPRIM_P2VAR_PARA(actBNDIGIT) a, VSECPRIM_P2VAR_PARA(actBNDIGIT) b)
{
  actBNDIGIT mask = (actBNDIGIT)(0u - swap); /* PRQA S 2986 */ /* MD_VSECPRIM_REDUNDANT_OP */
  actBNDIGIT digit;
  actU8 d;

  for (d = 0; d < BNDIGITS_256; d++)
  {
    digit = mask & (a[d] ^ b[d]);
    a[d] ^= digit; /* SBSW_VSECPRIM_WRITE_ARRAY */
    b[d] ^= digit; /* SBSW_VSECPRIM_WRITE_ARRAY */
  }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actX25519_initCore()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actX25519_initCore(VSECPRIM_P2VAR_PARA(actX25519STRUCT) ws) /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
{
  /* Initialize core */
  act25519_initCore((VSECPRIM_P2VAR_PARA(actCurve25519ws)) &ws->coreWS); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 *  actX25519_eval()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actX25519_eval(
  VSECPRIM_P2VAR_PARA(actX25519STRUCT) ws, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2VAR_PARA(actCurve25519ws) coreWSptr = (VSECPRIM_P2VAR_PARA(actCurve25519ws)) &ws->coreWS;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) x_1 = ((VSECPRIM_P2VAR_PARA(actBNDIGIT)) ws->basePointBuf.bnDigit_x_1);
  VSECPRIM_P2VAR_PARA(actBNDIGIT) x_2 = ((VSECPRIM_P2VAR_PARA(actBNDIGIT)) coreWSptr->auxVar.tempA);
  VSECPRIM_P2VAR_PARA(actBNDIGIT) z_2 = ((VSECPRIM_P2VAR_PARA(actBNDIGIT)) coreWSptr->auxVar.tempB);
  VSECPRIM_P2VAR_PARA(actBNDIGIT) x_3 = ((VSECPRIM_P2VAR_PARA(actBNDIGIT)) coreWSptr->auxVar.tempC);
  VSECPRIM_P2VAR_PARA(actBNDIGIT) z_3 = ((VSECPRIM_P2VAR_PARA(actBNDIGIT)) coreWSptr->auxVar.tempD);
  VSECPRIM_P2VAR_PARA(actBNDIGIT) A   = ((VSECPRIM_P2VAR_PARA(actBNDIGIT)) coreWSptr->auxVar.tempE);
  VSECPRIM_P2VAR_PARA(actBNDIGIT) B   = ((VSECPRIM_P2VAR_PARA(actBNDIGIT)) coreWSptr->auxVar.tempF);
  VSECPRIM_P2VAR_PARA(actBNDIGIT) C   = ((VSECPRIM_P2VAR_PARA(actBNDIGIT)) coreWSptr->auxVar.tempG);
  VSECPRIM_P2VAR_PARA(actBNDIGIT) D   = ((VSECPRIM_P2VAR_PARA(actBNDIGIT)) coreWSptr->auxVar.tempH);
  VSECPRIM_P2VAR_PARA(actBNDIGIT) AA = A;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) BB = B;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) CB = C;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) DA = D;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) E = x_3;

  actU32 k_t;
  actU32 swap;
  actU8 i;

  /* Initialize */
  actBNLoadLE(x_1, ws->basePointBuf.uCoord, BNBYTES_256);  /*  x_1 = u */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  x_1[BNDIGITS_256 - 1u] &= (actBNDIGIT)MASK_MSBit;  /*  mask MSbit of input u coordinate */ /* SBSW_VSECPRIM_WRITE_ARRAY */
  actMemset(x_2, 0u, BNBYTES_256); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  *x_2 = 1;                     /*  x_2 = 1 */ /* SBSW_VSECPRIM_WRITE_POINTER */
  actMemset(z_2, 0u, BNBYTES_256); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /*  x_2 = 0 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemCpyWordArray(x_3, x_1, BNDIGITS_256); /*  x_3 = u */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemCpyWordArray(z_3, x_2, BNDIGITS_256); /*  x_3 = 1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  swap = 0u;
  for (i = 255u; i > 0u; i--)
  {
    k_t = ((actU32)ws->privateKey[(i - 1u) / 8u] >> ((i - 1u) & 0x07u)) & 0x01u;

    /* conditional swap; see below. */
    swap ^= k_t;
    actX25519_cswap(swap, x_2, x_3); /* SBSW_VSECPRIM_CALL_FUNCTION */
    actX25519_cswap(swap, z_2, z_3); /* SBSW_VSECPRIM_CALL_FUNCTION */
    swap = k_t;

    /* algorithm from https:/ /tools.ietf.org/html/rfc7748
     * has been re-ordered to save memory!
     *
     *  1. calculate A,  B,  C  and D
     *  2. calculate DA, CB, BB and AA
     *  3. calculate E
     *  4. calculate x_2, z_2, x_3, z_3
     */

    act25519_addModP(coreWSptr, A, x_2, z_2);  /*  A   = x2 + z_2 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_subModP(coreWSptr, B, x_2, z_2);  /*  B   = x2 - z_2 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_addModP(coreWSptr, C, x_3, z_3);  /*  C   = x3 + z_3 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_subModP(coreWSptr, D, x_3, z_3);  /*  D   = x3 - z_3 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_mulModP(coreWSptr, DA, D, A); /*  DA  =  D * A */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_mulModP(coreWSptr, CB, C, B); /*  CB  =  C * B */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_mulModP(coreWSptr, BB, B, B); /*  BB  =  B * B */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_mulModP(coreWSptr, AA, A, A); /*  AA  =  A * A */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_subModP(coreWSptr, E, AA, BB);  /*  E   = AA - BB */ /* SBSW_VSECPRIM_CALL_FUNCTION */

    act25519_mulModP(coreWSptr, x_2, AA, BB);  /*  x_2 = AA * BB */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* 5 multiplications + 5 additions */
    actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

    actMemCpyWordArray(BB, a24, BNDIGITS_256); /* BB  =           a24 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_mulModP(coreWSptr, z_2, BB, E); /* z_2 =           a24 * E */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_addModP(coreWSptr, z_2, AA, z_2); /* z_2 =      AA + a24 * E */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_mulModP(coreWSptr, z_2, E, z_2);  /* z_2 = E * (AA + a24 * E) */ /* SBSW_VSECPRIM_CALL_FUNCTION */

    act25519_addModP(coreWSptr, x_3, DA, CB);  /* x_3 =  DA + CB */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_mulModP(coreWSptr, x_3, x_3, x_3);  /* x_3 = (DA + CB) ^2 */ /* SBSW_VSECPRIM_CALL_FUNCTION */

    act25519_subModP(coreWSptr, z_3, DA, CB);  /* z_3 =        DA - CB */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_mulModP(coreWSptr, z_3, z_3, z_3);  /* z_3 =       (DA - CB) ^2 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_mulModP(coreWSptr, z_3, z_3, x_1);  /* z_3 = x_1 * (DA - CB) ^2 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* 5 multiplications + 3 additions */
    actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }

  /* conditional swap; see below. */
  actX25519_cswap(swap, x_2, x_3); /* SBSW_VSECPRIM_CALL_FUNCTION */
  actX25519_cswap(swap, z_2, z_3); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* return x_2 * z_2^-1 */
  act25519_invModP(coreWSptr, z_3, z_2, watchdog); /* z_3 =      z_2^-1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_mulModP(coreWSptr, x_2, x_2, z_3);  /* x_2 = x2 * z_2^-1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actBNStoreLE(ws->basePointBuf.uCoord, x_2, BNDIGITS_256); /* SBSW_VSECPRIM_CALL_FUNCTION */

  actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempA, 0u, sizeof(coreWSptr->auxVar.tempA)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempB, 0u, sizeof(coreWSptr->auxVar.tempB)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempC, 0u, sizeof(coreWSptr->auxVar.tempC)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempD, 0u, sizeof(coreWSptr->auxVar.tempD)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempE, 0u, sizeof(coreWSptr->auxVar.tempE)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempF, 0u, sizeof(coreWSptr->auxVar.tempF)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempG, 0u, sizeof(coreWSptr->auxVar.tempG)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempH, 0u, sizeof(coreWSptr->auxVar.tempH)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
  return actOK;
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTX25519CORE_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actX25519core.c
 *********************************************************************************************************************/
