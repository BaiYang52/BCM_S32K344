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
/*!        \file  actRMD160.c
 *        \brief  This file contains an implementation of the RMD160 hash function.
 *
 *      \details  Currently the actClib version is used.
 *                This file is part of the embedded systems library cvActLib/ES
 *********************************************************************************************************************/
#define ACTRMD160_SOURCE

#include "actIRMD160.h"
#include "actUtilities.h"
#include "actWatchdog.h"
#include "actConfig.h"

#if (VSECPRIM_ACTRMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/* PRQA S 2982, 2985 EOF */ /* MD_VSECPRIM_QM_ONLY_REDUNDANT_ASSIGNMENT_OR_OPERATION */
/* PRQA S 3226 EOF */ /* MD_VSECPRIM_ASSIGNMENT_RESULT_IN_OP */ /* Note: QM_ONLY justification (MD_VSECPRIM_QM_ONLY_...) */
/* PRQA S 2889 EOF */ /* MD_VSECPRIM_QM_ONLY_RETURN_PATHS */
/* PRQA S 6010, 6030 EOF */ /* MD_VSECPRIM_QM_ONLY_METRIC */
/* PRQA S 3205 EOF */ /* MD_VSECPRIM_QM_ONLY_UNUSED_FUNCTION */

/****************************************************************************
 ** Types and constants
 ***************************************************************************/

# define actRMD160_WORD_SIZE                                          (4u)

typedef actU32 (* RMD160fct) (actU32 x, actU32 y, actU32 z);

/****************************************************************************
 * word access order and rotate counts
 ***************************************************************************/

# define VSECPRIM_START_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if (actRMD160_SPEED_UP == 0)
VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) z[160] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */
{
   0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
   7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8,
   3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12,
   1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2,
   4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13,
   5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12,
   6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2,
   15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13,
   8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14,
   12, 15, 10, 4, 1, 5, 8, 7, 6, 2, 13, 14, 0, 3, 9, 11
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) s[160] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */
{
   11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8,
   7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12,
   11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5,
   11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12,
   9, 15, 5, 11, 6, 8, 13, 12, 5, 12, 13, 14, 11, 8, 5, 6,
   8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6,
   9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11,
   9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5,
   15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8,
   8, 5, 12, 9, 12, 5, 14, 6, 8, 13, 6, 5, 15, 13, 11, 11
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) y[10] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */
{
   0x0L, 0x5A827999L, 0x6ED9EBA1L, 0x8F1BBCDCL, 0xA953FD4EL,
   0x50A28BE6L, 0x5C4DD124L, 0x6D703EF3L, 0x7A6D76E9L, 0x0L
};

# else
VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) z_L[80] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */
{
   0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
   7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8,
   3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12,
   1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2,
   4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) z_R[80] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */
{
   5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12,
   6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2,
   15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13,
   8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14,
   12, 15, 10, 4, 1, 5, 8, 7, 6, 2, 13, 14, 0, 3, 9, 11
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) s_L[80] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */
{
   11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8,
   7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12,
   11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5,
   11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12,
   9, 15, 5, 11, 6, 8, 13, 12, 5, 12, 13, 14, 11, 8, 5, 6
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) s_R[80] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */
{
   8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6,
   9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11,
   9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5,
   15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8,
   8, 5, 12, 9, 12, 5, 14, 6, 8, 13, 6, 5, 15, 13, 11, 11
};

# endif

# define VSECPRIM_STOP_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** LOCAL FUNCTION PROTOTYPES
 ***************************************************************************/

 VSECPRIM_LOCAL_FUNC(void) RMD160Loop(
   VSECPRIM_P2CONST_PARA(actU32) W, VSECPRIM_P2VAR_PARA(actU32) x, actU8 off,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

 /**********************************************************************************************************************
 *  actRMD160Transform()
 *********************************************************************************************************************/
/*! \brief          This function hashes one 64 byte block of data.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to hash context structure
 *                               (out) actualized hash context structure
 *  \param[in]      in           pointer to a little endian byte array
 *  \param[in,out]  H            (in) pointer to a 64 byte block
 *                               (out) hash RMD160 of the 64 byte block
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/

 VSECPRIM_LOCAL_FUNC(void) actRMD160Transform(
   VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU32) H,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/****************************************************************************
 ** Local functions and macros
 ***************************************************************************/

/* RMD160 bit rotate function */
# define actRotl10(x)                                                 (actRotl((x), 10))
# define RL(w)                                                        ((w)=actRotl10((w)))

# if (actRMD160_SPEED_UP == 0)

#  define VSECPRIM_START_SEC_CODE
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_LOCAL_FUNC(actU32) f(actU32 u, actU32 v, actU32 w) { return ((u) ^ (v) ^ (w)); }
VSECPRIM_LOCAL_FUNC(actU32) g(actU32 u, actU32 v, actU32 w) { return ((w) ^ ((u) & ((v) ^ (w)))); }
VSECPRIM_LOCAL_FUNC(actU32) h(actU32 u, actU32 v, actU32 w) { return (((u) | ~(v)) ^ (w)); }
VSECPRIM_LOCAL_FUNC(actU32) k(actU32 u, actU32 v, actU32 w) { return ((v) ^ ((w) & ((u) ^ (v)))); }
VSECPRIM_LOCAL_FUNC(actU32) l(actU32 u, actU32 v, actU32 w) { return ((u) ^ ((v) | ~(w))); }

#  define VSECPRIM_STOP_SEC_CODE
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#  define VSECPRIM_START_SEC_CONST_UNSPECIFIED
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM(VSECPRIM_LOCAL, RMD160fct) roundfct[] = {f, g, h, k, l, l, k, h, g, f};

#  define VSECPRIM_STOP_SEC_CONST_UNSPECIFIED
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#  define Round(a, b, c, d, e, i)                                     ((a) += roundfct[((i)/16)]((b), (c), (d)) + W[ACT_ROM_R_BYT(z[(i)])] + (c)[((i)/16)])

#  define VSECPRIM_START_SEC_CODE
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_LOCAL_FUNC(void) RMD160Loop(
   VSECPRIM_P2CONST_PARA(actU32) W, VSECPRIM_P2VAR_PARA(actU32) x, actU8 off,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{

   actU8 m, i, j = 0;

   for (i = 0; i < 80; i += 5)
   {

      m = (actU8)(i+off);
      j = (actU8)(m/16);

/*    Round(x[0], x[1], x[2], x[3], x[4], j); */
      x[0] += roundfct[j]((x[1]), (x[2]), (x[3])) + W[ACT_ROM_R_BYT(z[m])] + ACT_ROM_R_DWD(y[j]);
      RL(x[2]);
      x[0] = x[4] + actRotl(x[0], ACT_ROM_R_BYT(s[m]));
      j = (actU8)(++m/16);
      actL3trigger(watchdog);

/*     Round(x[4], x[0], x[1], x[2], x[3], j); */
      x[4] += roundfct[j]((x[0]), (x[1]), (x[2])) + W[ACT_ROM_R_BYT(z[m])] + ACT_ROM_R_DWD(y[j]);
      RL(x[1]);
      x[4] = x[3] + actRotl(x[4], ACT_ROM_R_BYT(s[m]));
      j = (actU8)(++m/16);
      actL3trigger(watchdog);

/*    Round(x[3], x[4], x[0], x[1], x[2], j); */
      x[3] += roundfct[j]((x[4]), (x[0]), (x[1])) + W[ACT_ROM_R_BYT(z[m])] + ACT_ROM_R_DWD(y[j]);
      RL(x[0]);
      x[3] = x[2] + actRotl(x[3], ACT_ROM_R_BYT(s[m]));
      j = (actU8)(++m/16);
      actL3trigger(watchdog);

/*    Round(x[2], x[3], x[4], x[0], x[1], j); */
      x[2] += roundfct[j]((x[3]), (x[4]), (x[0])) + W[ACT_ROM_R_BYT(z[m])] + ACT_ROM_R_DWD(y[j]);
      RL(x[4]);
      x[2] = x[1] + actRotl(x[2], ACT_ROM_R_BYT(s[m]));
      j = (actU8)(++m/16);
      actL3trigger(watchdog);

/*    Round(x[1], x[2], x[3], x[4], x[0], j); */
      x[1] += roundfct[j]((x[2]), (x[3]), (x[4])) + W[ACT_ROM_R_BYT(z[m])] + ACT_ROM_R_DWD(y[j]);
      RL(x[3]);
      x[1] = x[0] + actRotl(x[1], ACT_ROM_R_BYT(s[m]));

      actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   }
}
#  define VSECPRIM_STOP_SEC_CODE
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# else

/* The RMD-160 round functions */
#  define f(x, y, z)                                                  ((x) ^ (y) ^ (z))
#  define g(x, y, z)                                                  ((z) ^ ((x) & ((y) ^ (z))))
#  define h(x, y, z)                                                  (((x) | ~(y)) ^ (z))
#  define k(x, y, z)                                                  ((y) ^ ((z) & ((x) ^ (y))))
#  define l(x, y, z)                                                  ((x) ^ ((y) | ~(z)))

/* L0, L1, L2, L3, L4 are the different left line operations used in RMD160 */
#  define L0(a, b, c, d, e, i)                                        (a += f((b), (c), (d)) + W[ACT_ROM_R_BYT(z_L[i])] + 0x00000000UL)
#  define L1(a, b, c, d, e, i)                                        (a += g((b), (c), (d)) + W[ACT_ROM_R_BYT(z_L[i])] + 0x5A827999UL)
#  define L2(a, b, c, d, e, i)                                        (a += h((b), (c), (d)) + W[ACT_ROM_R_BYT(z_L[i])] + 0x6ED9EBA1UL)
#  define L3(a, b, c, d, e, i)                                        (a += k((b), (c), (d)) + W[ACT_ROM_R_BYT(z_L[i])] + 0x8F1BBCDCUL)
#  define L4(a, b, c, d, e, i)                                        (a += l((b), (c), (d)) + W[ACT_ROM_R_BYT(z_L[i])] + 0xA953FD4EUL)

/* R0, R1, R2, R3, R4 are the different right line operations used in RMD160 */
#  define R0(a, b, c, d, e, i)                                        (a += l((b), (c), (d)) + W[ACT_ROM_R_BYT(z_R[i])] + 0x50A28BE6UL)
#  define R1(a, b, c, d, e, i)                                        (a += k((b), (c), (d)) + W[ACT_ROM_R_BYT(z_R[i])] + 0x5C4DD124UL)
#  define R2(a, b, c, d, e, i)                                        (a += h((b), (c), (d)) + W[ACT_ROM_R_BYT(z_R[i])] + 0x6D703EF3UL)
#  define R3(a, b, c, d, e, i)                                        (a += g((b), (c), (d)) + W[ACT_ROM_R_BYT(z_R[i])] + 0x7A6D76E9UL)
#  define R4(a, b, c, d, e, i)                                        (a += f((b), (c), (d)) + W[ACT_ROM_R_BYT(z_R[i])] + 0x00000000UL)

# endif

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** Main local function: hash one 64 byte block of data
 ***************************************************************************/

/**********************************************************************************************************************
 *  actRMD160Transform()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 * 
 */

VSECPRIM_LOCAL_FUNC(void) actRMD160Transform(
   VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU32) H,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{

   actU32 W[16];
   actLengthType i;
   actU32 t;

# if (actRMD160_SPEED_UP == 0)

   actU32 xl[5], xr[5];

   /* Copy H[] to working vars of left and right lines */
   xl[0] = xr[0] = H[0];
   xl[1] = xr[1] = H[1];
   xl[2] = xr[2] = H[2];
   xl[3] = xr[3] = H[3];
   xl[4] = xr[4] = H[4];

# else

   /* Copy H[] to working vars of left and right lines */
   actU32 A_L = H[0];
   actU32 B_L = H[1];
   actU32 C_L = H[2];
   actU32 D_L = H[3];
   actU32 E_L = H[4];

   actU32 A_R = H[0];
   actU32 B_R = H[1];
   actU32 C_R = H[2];
   actU32 D_R = H[3];
   actU32 E_R = H[4];

# endif

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* interpret block 'in' as little endian 32 bit values */
   actCpyLEtoU32 (W, in, actHASH_BLOCK_SIZE_RMD160);

# if (actRMD160_SPEED_UP == 0)

   RMD160Loop(W, xl, 0, watchdog);

   RMD160Loop(W, xr, 80, watchdog);

   t = H[0];
   H[0] = H[1] + xl[2] + xr[3];
   H[1] = H[2] + xl[3] + xr[4];
   H[2] = H[3] + xl[4] + xr[0];
   H[3] = H[4] + xl[0] + xr[1];
   H[4] = t    + xl[1] + xr[2];

# endif

# if (actRMD160_SPEED_UP == 1)

   /* 5 rounds of 16 operations each */
   /* round 1 */
   for (i = 0u; i < 16u; ++i)
   {
      L0(A_L, B_L, C_L, D_L, E_L, i); t=A_L; A_L=E_L; E_L=D_L; D_L=RL(C_L); C_L=B_L; B_L=A_L+actRotl(t, ACT_ROM_R_BYT(s_L[i]));
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   }
   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* round 2 */
   for (; i<32u; ++i)
   {
      L1(A_L, B_L, C_L, D_L, E_L, i); t=A_L; A_L=E_L; E_L=D_L; D_L=RL(C_L); C_L=B_L; B_L=A_L+actRotl(t, ACT_ROM_R_BYT(s_L[i]));
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   }
   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* round 3 */
   for (; i<48u; ++i)
   {
      L2(A_L, B_L, C_L, D_L, E_L, i); t=A_L; A_L=E_L; E_L=D_L; D_L=RL(C_L); C_L=B_L; B_L=A_L+actRotl(t, ACT_ROM_R_BYT(s_L[i]));
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   }
   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* round 4 */
   for (; i<64u; ++i)
   {
      L3(A_L, B_L, C_L, D_L, E_L, i); t=A_L; A_L=E_L; E_L=D_L; D_L=RL(C_L); C_L=B_L; B_L=A_L+actRotl(t, ACT_ROM_R_BYT(s_L[i]));
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   }
   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* round 5 */
   for (; i<80u; ++i)
   {
      L4(A_L, B_L, C_L, D_L, E_L, i); t=A_L; A_L=E_L; E_L=D_L; D_L=RL(C_L); C_L=B_L; B_L=A_L+actRotl(t, ACT_ROM_R_BYT(s_L[i]));
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   };
   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

# elif (actRMD160_SPEED_UP > 1)

   /* 5 rounds of 16 operations each. Loops unrolled. */
   /* round 1 */
   L0(A_L, B_L, C_L, D_L, E_L, 0); RL(C_L); A_L=E_L+actRotl(A_L, ACT_ROM_R_BYT(s_L[0])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L0(E_L, A_L, B_L, C_L, D_L, 1); RL(B_L); E_L=D_L+actRotl(E_L, ACT_ROM_R_BYT(s_L[1])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L0(D_L, E_L, A_L, B_L, C_L, 2); RL(A_L); D_L=C_L+actRotl(D_L, ACT_ROM_R_BYT(s_L[2])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L0(C_L, D_L, E_L, A_L, B_L, 3); RL(E_L); C_L=B_L+actRotl(C_L, ACT_ROM_R_BYT(s_L[3])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L0(B_L, C_L, D_L, E_L, A_L, 4); RL(D_L); B_L=A_L+actRotl(B_L, ACT_ROM_R_BYT(s_L[4])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L0(A_L, B_L, C_L, D_L, E_L, 5); RL(C_L); A_L=E_L+actRotl(A_L, ACT_ROM_R_BYT(s_L[5])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L0(E_L, A_L, B_L, C_L, D_L, 6); RL(B_L); E_L=D_L+actRotl(E_L, ACT_ROM_R_BYT(s_L[6])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L0(D_L, E_L, A_L, B_L, C_L, 7); RL(A_L); D_L=C_L+actRotl(D_L, ACT_ROM_R_BYT(s_L[7])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L0(C_L, D_L, E_L, A_L, B_L, 8); RL(E_L); C_L=B_L+actRotl(C_L, ACT_ROM_R_BYT(s_L[8])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L0(B_L, C_L, D_L, E_L, A_L, 9); RL(D_L); B_L=A_L+actRotl(B_L, ACT_ROM_R_BYT(s_L[9])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L0(A_L, B_L, C_L, D_L, E_L, 10); RL(C_L); A_L=E_L+actRotl(A_L, ACT_ROM_R_BYT(s_L[10])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L0(E_L, A_L, B_L, C_L, D_L, 11); RL(B_L); E_L=D_L+actRotl(E_L, ACT_ROM_R_BYT(s_L[11])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L0(D_L, E_L, A_L, B_L, C_L, 12); RL(A_L); D_L=C_L+actRotl(D_L, ACT_ROM_R_BYT(s_L[12])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L0(C_L, D_L, E_L, A_L, B_L, 13); RL(E_L); C_L=B_L+actRotl(C_L, ACT_ROM_R_BYT(s_L[13])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L0(B_L, C_L, D_L, E_L, A_L, 14); RL(D_L); B_L=A_L+actRotl(B_L, ACT_ROM_R_BYT(s_L[14])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L0(A_L, B_L, C_L, D_L, E_L, 15); RL(C_L); A_L=E_L+actRotl(A_L, ACT_ROM_R_BYT(s_L[15]));

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* round 2 */
   L1(E_L, A_L, B_L, C_L, D_L, 16); RL(B_L); E_L=D_L+actRotl(E_L, ACT_ROM_R_BYT(s_L[16])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L1(D_L, E_L, A_L, B_L, C_L, 17); RL(A_L); D_L=C_L+actRotl(D_L, ACT_ROM_R_BYT(s_L[17])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L1(C_L, D_L, E_L, A_L, B_L, 18); RL(E_L); C_L=B_L+actRotl(C_L, ACT_ROM_R_BYT(s_L[18])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L1(B_L, C_L, D_L, E_L, A_L, 19); RL(D_L); B_L=A_L+actRotl(B_L, ACT_ROM_R_BYT(s_L[19])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L1(A_L, B_L, C_L, D_L, E_L, 20); RL(C_L); A_L=E_L+actRotl(A_L, ACT_ROM_R_BYT(s_L[20])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L1(E_L, A_L, B_L, C_L, D_L, 21); RL(B_L); E_L=D_L+actRotl(E_L, ACT_ROM_R_BYT(s_L[21])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L1(D_L, E_L, A_L, B_L, C_L, 22); RL(A_L); D_L=C_L+actRotl(D_L, ACT_ROM_R_BYT(s_L[22])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L1(C_L, D_L, E_L, A_L, B_L, 23); RL(E_L); C_L=B_L+actRotl(C_L, ACT_ROM_R_BYT(s_L[23])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L1(B_L, C_L, D_L, E_L, A_L, 24); RL(D_L); B_L=A_L+actRotl(B_L, ACT_ROM_R_BYT(s_L[24])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L1(A_L, B_L, C_L, D_L, E_L, 25); RL(C_L); A_L=E_L+actRotl(A_L, ACT_ROM_R_BYT(s_L[25])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L1(E_L, A_L, B_L, C_L, D_L, 26); RL(B_L); E_L=D_L+actRotl(E_L, ACT_ROM_R_BYT(s_L[26])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L1(D_L, E_L, A_L, B_L, C_L, 27); RL(A_L); D_L=C_L+actRotl(D_L, ACT_ROM_R_BYT(s_L[27])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L1(C_L, D_L, E_L, A_L, B_L, 28); RL(E_L); C_L=B_L+actRotl(C_L, ACT_ROM_R_BYT(s_L[28])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L1(B_L, C_L, D_L, E_L, A_L, 29); RL(D_L); B_L=A_L+actRotl(B_L, ACT_ROM_R_BYT(s_L[29])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L1(A_L, B_L, C_L, D_L, E_L, 30); RL(C_L); A_L=E_L+actRotl(A_L, ACT_ROM_R_BYT(s_L[30])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L1(E_L, A_L, B_L, C_L, D_L, 31); RL(B_L); E_L=D_L+actRotl(E_L, ACT_ROM_R_BYT(s_L[31]));

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* round 3 */
   L2(D_L, E_L, A_L, B_L, C_L, 32); RL(A_L); D_L=C_L+actRotl(D_L, ACT_ROM_R_BYT(s_L[32])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L2(C_L, D_L, E_L, A_L, B_L, 33); RL(E_L); C_L=B_L+actRotl(C_L, ACT_ROM_R_BYT(s_L[33])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L2(B_L, C_L, D_L, E_L, A_L, 34); RL(D_L); B_L=A_L+actRotl(B_L, ACT_ROM_R_BYT(s_L[34])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L2(A_L, B_L, C_L, D_L, E_L, 35); RL(C_L); A_L=E_L+actRotl(A_L, ACT_ROM_R_BYT(s_L[35])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L2(E_L, A_L, B_L, C_L, D_L, 36); RL(B_L); E_L=D_L+actRotl(E_L, ACT_ROM_R_BYT(s_L[36])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L2(D_L, E_L, A_L, B_L, C_L, 37); RL(A_L); D_L=C_L+actRotl(D_L, ACT_ROM_R_BYT(s_L[37])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L2(C_L, D_L, E_L, A_L, B_L, 38); RL(E_L); C_L=B_L+actRotl(C_L, ACT_ROM_R_BYT(s_L[38])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L2(B_L, C_L, D_L, E_L, A_L, 39); RL(D_L); B_L=A_L+actRotl(B_L, ACT_ROM_R_BYT(s_L[39])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L2(A_L, B_L, C_L, D_L, E_L, 40); RL(C_L); A_L=E_L+actRotl(A_L, ACT_ROM_R_BYT(s_L[40])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L2(E_L, A_L, B_L, C_L, D_L, 41); RL(B_L); E_L=D_L+actRotl(E_L, ACT_ROM_R_BYT(s_L[41])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L2(D_L, E_L, A_L, B_L, C_L, 42); RL(A_L); D_L=C_L+actRotl(D_L, ACT_ROM_R_BYT(s_L[42])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L2(C_L, D_L, E_L, A_L, B_L, 43); RL(E_L); C_L=B_L+actRotl(C_L, ACT_ROM_R_BYT(s_L[43])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L2(B_L, C_L, D_L, E_L, A_L, 44); RL(D_L); B_L=A_L+actRotl(B_L, ACT_ROM_R_BYT(s_L[44])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L2(A_L, B_L, C_L, D_L, E_L, 45); RL(C_L); A_L=E_L+actRotl(A_L, ACT_ROM_R_BYT(s_L[45])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L2(E_L, A_L, B_L, C_L, D_L, 46); RL(B_L); E_L=D_L+actRotl(E_L, ACT_ROM_R_BYT(s_L[46])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L2(D_L, E_L, A_L, B_L, C_L, 47); RL(A_L); D_L=C_L+actRotl(D_L, ACT_ROM_R_BYT(s_L[47]));

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* round 4 */
   L3(C_L, D_L, E_L, A_L, B_L, 48); RL(E_L); C_L=B_L+actRotl(C_L, ACT_ROM_R_BYT(s_L[48])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L3(B_L, C_L, D_L, E_L, A_L, 49); RL(D_L); B_L=A_L+actRotl(B_L, ACT_ROM_R_BYT(s_L[49])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L3(A_L, B_L, C_L, D_L, E_L, 50); RL(C_L); A_L=E_L+actRotl(A_L, ACT_ROM_R_BYT(s_L[50])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L3(E_L, A_L, B_L, C_L, D_L, 51); RL(B_L); E_L=D_L+actRotl(E_L, ACT_ROM_R_BYT(s_L[51])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L3(D_L, E_L, A_L, B_L, C_L, 52); RL(A_L); D_L=C_L+actRotl(D_L, ACT_ROM_R_BYT(s_L[52])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L3(C_L, D_L, E_L, A_L, B_L, 53); RL(E_L); C_L=B_L+actRotl(C_L, ACT_ROM_R_BYT(s_L[53])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L3(B_L, C_L, D_L, E_L, A_L, 54); RL(D_L); B_L=A_L+actRotl(B_L, ACT_ROM_R_BYT(s_L[54])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L3(A_L, B_L, C_L, D_L, E_L, 55); RL(C_L); A_L=E_L+actRotl(A_L, ACT_ROM_R_BYT(s_L[55])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L3(E_L, A_L, B_L, C_L, D_L, 56); RL(B_L); E_L=D_L+actRotl(E_L, ACT_ROM_R_BYT(s_L[56])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L3(D_L, E_L, A_L, B_L, C_L, 57); RL(A_L); D_L=C_L+actRotl(D_L, ACT_ROM_R_BYT(s_L[57])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L3(C_L, D_L, E_L, A_L, B_L, 58); RL(E_L); C_L=B_L+actRotl(C_L, ACT_ROM_R_BYT(s_L[58])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L3(B_L, C_L, D_L, E_L, A_L, 59); RL(D_L); B_L=A_L+actRotl(B_L, ACT_ROM_R_BYT(s_L[59])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L3(A_L, B_L, C_L, D_L, E_L, 60); RL(C_L); A_L=E_L+actRotl(A_L, ACT_ROM_R_BYT(s_L[60])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L3(E_L, A_L, B_L, C_L, D_L, 61); RL(B_L); E_L=D_L+actRotl(E_L, ACT_ROM_R_BYT(s_L[61])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L3(D_L, E_L, A_L, B_L, C_L, 62); RL(A_L); D_L=C_L+actRotl(D_L, ACT_ROM_R_BYT(s_L[62])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L3(C_L, D_L, E_L, A_L, B_L, 63); RL(E_L); C_L=B_L+actRotl(C_L, ACT_ROM_R_BYT(s_L[63]));

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* round 5 */
   L4(B_L, C_L, D_L, E_L, A_L, 64); RL(D_L); B_L=A_L+actRotl(B_L, ACT_ROM_R_BYT(s_L[64])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L4(A_L, B_L, C_L, D_L, E_L, 65); RL(C_L); A_L=E_L+actRotl(A_L, ACT_ROM_R_BYT(s_L[65])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L4(E_L, A_L, B_L, C_L, D_L, 66); RL(B_L); E_L=D_L+actRotl(E_L, ACT_ROM_R_BYT(s_L[66])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L4(D_L, E_L, A_L, B_L, C_L, 67); RL(A_L); D_L=C_L+actRotl(D_L, ACT_ROM_R_BYT(s_L[67])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L4(C_L, D_L, E_L, A_L, B_L, 68); RL(E_L); C_L=B_L+actRotl(C_L, ACT_ROM_R_BYT(s_L[68])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L4(B_L, C_L, D_L, E_L, A_L, 69); RL(D_L); B_L=A_L+actRotl(B_L, ACT_ROM_R_BYT(s_L[69])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L4(A_L, B_L, C_L, D_L, E_L, 70); RL(C_L); A_L=E_L+actRotl(A_L, ACT_ROM_R_BYT(s_L[70])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L4(E_L, A_L, B_L, C_L, D_L, 71); RL(B_L); E_L=D_L+actRotl(E_L, ACT_ROM_R_BYT(s_L[71])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L4(D_L, E_L, A_L, B_L, C_L, 72); RL(A_L); D_L=C_L+actRotl(D_L, ACT_ROM_R_BYT(s_L[72])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L4(C_L, D_L, E_L, A_L, B_L, 73); RL(E_L); C_L=B_L+actRotl(C_L, ACT_ROM_R_BYT(s_L[73])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L4(B_L, C_L, D_L, E_L, A_L, 74); RL(D_L); B_L=A_L+actRotl(B_L, ACT_ROM_R_BYT(s_L[74])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L4(A_L, B_L, C_L, D_L, E_L, 75); RL(C_L); A_L=E_L+actRotl(A_L, ACT_ROM_R_BYT(s_L[75])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L4(E_L, A_L, B_L, C_L, D_L, 76); RL(B_L); E_L=D_L+actRotl(E_L, ACT_ROM_R_BYT(s_L[76])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L4(D_L, E_L, A_L, B_L, C_L, 77); RL(A_L); D_L=C_L+actRotl(D_L, ACT_ROM_R_BYT(s_L[77])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L4(C_L, D_L, E_L, A_L, B_L, 78); RL(E_L); C_L=B_L+actRotl(C_L, ACT_ROM_R_BYT(s_L[78])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   L4(B_L, C_L, D_L, E_L, A_L, 79); RL(D_L); B_L=A_L+actRotl(B_L, ACT_ROM_R_BYT(s_L[79]));

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

# endif

# if (actRMD160_SPEED_UP == 1)

   /* 5 rounds of 16 operations each. */
   /* round 1 */
   for (i = 0; i < 16u; ++i)
   {
      R0(A_R, B_R, C_R, D_R, E_R, i); t=A_R; A_R=E_R; E_R=D_R; D_R=RL(C_R); C_R=B_R; B_R=A_R+actRotl(t, ACT_ROM_R_BYT(s_R[i]));
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   }
   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* round 2 */
   for (; i<32u; ++i)
   {
      R1(A_R, B_R, C_R, D_R, E_R, i); t=A_R; A_R=E_R; E_R=D_R; D_R=RL(C_R); C_R=B_R; B_R=A_R+actRotl(t, ACT_ROM_R_BYT(s_R[i]));
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   }
   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* round 3 */
   for (; i<48u; ++i)
   {
      R2(A_R, B_R, C_R, D_R, E_R, i); t=A_R; A_R=E_R; E_R=D_R; D_R=RL(C_R); C_R=B_R; B_R=A_R+actRotl(t, ACT_ROM_R_BYT(s_R[i]));
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   }
   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* round 4 */
   for (; i<64u; ++i)
   {
      R3(A_R, B_R, C_R, D_R, E_R, i); t=A_R; A_R=E_R; E_R=D_R; D_R=RL(C_R); C_R=B_R; B_R=A_R+actRotl(t, ACT_ROM_R_BYT(s_R[i]));
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   }
   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* round 5 */
   for (; i<80u; ++i)
   {
      R4(A_R, B_R, C_R, D_R, E_R, i); t=A_R; A_R=E_R; E_R=D_R; D_R=RL(C_R); C_R=B_R; B_R=A_R+actRotl(t, ACT_ROM_R_BYT(s_R[i]));
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   };

# elif (actRMD160_SPEED_UP > 1)

   /* 5 rounds of 16 operations each. Loops unrolled. */
   /* round 1 */
   R0(A_R, B_R, C_R, D_R, E_R, 0); RL(C_R); A_R=E_R+actRotl(A_R, ACT_ROM_R_BYT(s_R[0])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R0(E_R, A_R, B_R, C_R, D_R, 1); RL(B_R); E_R=D_R+actRotl(E_R, ACT_ROM_R_BYT(s_R[1])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R0(D_R, E_R, A_R, B_R, C_R, 2); RL(A_R); D_R=C_R+actRotl(D_R, ACT_ROM_R_BYT(s_R[2])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R0(C_R, D_R, E_R, A_R, B_R, 3); RL(E_R); C_R=B_R+actRotl(C_R, ACT_ROM_R_BYT(s_R[3])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R0(B_R, C_R, D_R, E_R, A_R, 4); RL(D_R); B_R=A_R+actRotl(B_R, ACT_ROM_R_BYT(s_R[4])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R0(A_R, B_R, C_R, D_R, E_R, 5); RL(C_R); A_R=E_R+actRotl(A_R, ACT_ROM_R_BYT(s_R[5])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R0(E_R, A_R, B_R, C_R, D_R, 6); RL(B_R); E_R=D_R+actRotl(E_R, ACT_ROM_R_BYT(s_R[6])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R0(D_R, E_R, A_R, B_R, C_R, 7); RL(A_R); D_R=C_R+actRotl(D_R, ACT_ROM_R_BYT(s_R[7])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R0(C_R, D_R, E_R, A_R, B_R, 8); RL(E_R); C_R=B_R+actRotl(C_R, ACT_ROM_R_BYT(s_R[8])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R0(B_R, C_R, D_R, E_R, A_R, 9); RL(D_R); B_R=A_R+actRotl(B_R, ACT_ROM_R_BYT(s_R[9])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R0(A_R, B_R, C_R, D_R, E_R, 10); RL(C_R); A_R=E_R+actRotl(A_R, ACT_ROM_R_BYT(s_R[10])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R0(E_R, A_R, B_R, C_R, D_R, 11); RL(B_R); E_R=D_R+actRotl(E_R, ACT_ROM_R_BYT(s_R[11])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R0(D_R, E_R, A_R, B_R, C_R, 12); RL(A_R); D_R=C_R+actRotl(D_R, ACT_ROM_R_BYT(s_R[12])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R0(C_R, D_R, E_R, A_R, B_R, 13); RL(E_R); C_R=B_R+actRotl(C_R, ACT_ROM_R_BYT(s_R[13])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R0(B_R, C_R, D_R, E_R, A_R, 14); RL(D_R); B_R=A_R+actRotl(B_R, ACT_ROM_R_BYT(s_R[14])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R0(A_R, B_R, C_R, D_R, E_R, 15); RL(C_R); A_R=E_R+actRotl(A_R, ACT_ROM_R_BYT(s_R[15]));

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* round 2 */
   R1(E_R, A_R, B_R, C_R, D_R, 16); RL(B_R); E_R=D_R+actRotl(E_R, ACT_ROM_R_BYT(s_R[16])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R1(D_R, E_R, A_R, B_R, C_R, 17); RL(A_R); D_R=C_R+actRotl(D_R, ACT_ROM_R_BYT(s_R[17])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R1(C_R, D_R, E_R, A_R, B_R, 18); RL(E_R); C_R=B_R+actRotl(C_R, ACT_ROM_R_BYT(s_R[18])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R1(B_R, C_R, D_R, E_R, A_R, 19); RL(D_R); B_R=A_R+actRotl(B_R, ACT_ROM_R_BYT(s_R[19])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R1(A_R, B_R, C_R, D_R, E_R, 20); RL(C_R); A_R=E_R+actRotl(A_R, ACT_ROM_R_BYT(s_R[20])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R1(E_R, A_R, B_R, C_R, D_R, 21); RL(B_R); E_R=D_R+actRotl(E_R, ACT_ROM_R_BYT(s_R[21])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R1(D_R, E_R, A_R, B_R, C_R, 22); RL(A_R); D_R=C_R+actRotl(D_R, ACT_ROM_R_BYT(s_R[22])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R1(C_R, D_R, E_R, A_R, B_R, 23); RL(E_R); C_R=B_R+actRotl(C_R, ACT_ROM_R_BYT(s_R[23])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R1(B_R, C_R, D_R, E_R, A_R, 24); RL(D_R); B_R=A_R+actRotl(B_R, ACT_ROM_R_BYT(s_R[24])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R1(A_R, B_R, C_R, D_R, E_R, 25); RL(C_R); A_R=E_R+actRotl(A_R, ACT_ROM_R_BYT(s_R[25])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R1(E_R, A_R, B_R, C_R, D_R, 26); RL(B_R); E_R=D_R+actRotl(E_R, ACT_ROM_R_BYT(s_R[26])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R1(D_R, E_R, A_R, B_R, C_R, 27); RL(A_R); D_R=C_R+actRotl(D_R, ACT_ROM_R_BYT(s_R[27])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R1(C_R, D_R, E_R, A_R, B_R, 28); RL(E_R); C_R=B_R+actRotl(C_R, ACT_ROM_R_BYT(s_R[28])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R1(B_R, C_R, D_R, E_R, A_R, 29); RL(D_R); B_R=A_R+actRotl(B_R, ACT_ROM_R_BYT(s_R[29])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R1(A_R, B_R, C_R, D_R, E_R, 30); RL(C_R); A_R=E_R+actRotl(A_R, ACT_ROM_R_BYT(s_R[30])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R1(E_R, A_R, B_R, C_R, D_R, 31); RL(B_R); E_R=D_R+actRotl(E_R, ACT_ROM_R_BYT(s_R[31]));

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* round 3 */
   R2(D_R, E_R, A_R, B_R, C_R, 32); RL(A_R); D_R=C_R+actRotl(D_R, ACT_ROM_R_BYT(s_R[32])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R2(C_R, D_R, E_R, A_R, B_R, 33); RL(E_R); C_R=B_R+actRotl(C_R, ACT_ROM_R_BYT(s_R[33])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R2(B_R, C_R, D_R, E_R, A_R, 34); RL(D_R); B_R=A_R+actRotl(B_R, ACT_ROM_R_BYT(s_R[34])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R2(A_R, B_R, C_R, D_R, E_R, 35); RL(C_R); A_R=E_R+actRotl(A_R, ACT_ROM_R_BYT(s_R[35])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R2(E_R, A_R, B_R, C_R, D_R, 36); RL(B_R); E_R=D_R+actRotl(E_R, ACT_ROM_R_BYT(s_R[36])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R2(D_R, E_R, A_R, B_R, C_R, 37); RL(A_R); D_R=C_R+actRotl(D_R, ACT_ROM_R_BYT(s_R[37])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R2(C_R, D_R, E_R, A_R, B_R, 38); RL(E_R); C_R=B_R+actRotl(C_R, ACT_ROM_R_BYT(s_R[38])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R2(B_R, C_R, D_R, E_R, A_R, 39); RL(D_R); B_R=A_R+actRotl(B_R, ACT_ROM_R_BYT(s_R[39])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R2(A_R, B_R, C_R, D_R, E_R, 40); RL(C_R); A_R=E_R+actRotl(A_R, ACT_ROM_R_BYT(s_R[40])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R2(E_R, A_R, B_R, C_R, D_R, 41); RL(B_R); E_R=D_R+actRotl(E_R, ACT_ROM_R_BYT(s_R[41])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R2(D_R, E_R, A_R, B_R, C_R, 42); RL(A_R); D_R=C_R+actRotl(D_R, ACT_ROM_R_BYT(s_R[42])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R2(C_R, D_R, E_R, A_R, B_R, 43); RL(E_R); C_R=B_R+actRotl(C_R, ACT_ROM_R_BYT(s_R[43])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R2(B_R, C_R, D_R, E_R, A_R, 44); RL(D_R); B_R=A_R+actRotl(B_R, ACT_ROM_R_BYT(s_R[44])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R2(A_R, B_R, C_R, D_R, E_R, 45); RL(C_R); A_R=E_R+actRotl(A_R, ACT_ROM_R_BYT(s_R[45])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R2(E_R, A_R, B_R, C_R, D_R, 46); RL(B_R); E_R=D_R+actRotl(E_R, ACT_ROM_R_BYT(s_R[46])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R2(D_R, E_R, A_R, B_R, C_R, 47); RL(A_R); D_R=C_R+actRotl(D_R, ACT_ROM_R_BYT(s_R[47]));

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* round 4 */
   R3(C_R, D_R, E_R, A_R, B_R, 48); RL(E_R); C_R=B_R+actRotl(C_R, ACT_ROM_R_BYT(s_R[48])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R3(B_R, C_R, D_R, E_R, A_R, 49); RL(D_R); B_R=A_R+actRotl(B_R, ACT_ROM_R_BYT(s_R[49])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R3(A_R, B_R, C_R, D_R, E_R, 50); RL(C_R); A_R=E_R+actRotl(A_R, ACT_ROM_R_BYT(s_R[50])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R3(E_R, A_R, B_R, C_R, D_R, 51); RL(B_R); E_R=D_R+actRotl(E_R, ACT_ROM_R_BYT(s_R[51])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R3(D_R, E_R, A_R, B_R, C_R, 52); RL(A_R); D_R=C_R+actRotl(D_R, ACT_ROM_R_BYT(s_R[52])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R3(C_R, D_R, E_R, A_R, B_R, 53); RL(E_R); C_R=B_R+actRotl(C_R, ACT_ROM_R_BYT(s_R[53])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R3(B_R, C_R, D_R, E_R, A_R, 54); RL(D_R); B_R=A_R+actRotl(B_R, ACT_ROM_R_BYT(s_R[54])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R3(A_R, B_R, C_R, D_R, E_R, 55); RL(C_R); A_R=E_R+actRotl(A_R, ACT_ROM_R_BYT(s_R[55])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R3(E_R, A_R, B_R, C_R, D_R, 56); RL(B_R); E_R=D_R+actRotl(E_R, ACT_ROM_R_BYT(s_R[56])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R3(D_R, E_R, A_R, B_R, C_R, 57); RL(A_R); D_R=C_R+actRotl(D_R, ACT_ROM_R_BYT(s_R[57])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R3(C_R, D_R, E_R, A_R, B_R, 58); RL(E_R); C_R=B_R+actRotl(C_R, ACT_ROM_R_BYT(s_R[58])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R3(B_R, C_R, D_R, E_R, A_R, 59); RL(D_R); B_R=A_R+actRotl(B_R, ACT_ROM_R_BYT(s_R[59])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R3(A_R, B_R, C_R, D_R, E_R, 60); RL(C_R); A_R=E_R+actRotl(A_R, ACT_ROM_R_BYT(s_R[60])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R3(E_R, A_R, B_R, C_R, D_R, 61); RL(B_R); E_R=D_R+actRotl(E_R, ACT_ROM_R_BYT(s_R[61])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R3(D_R, E_R, A_R, B_R, C_R, 62); RL(A_R); D_R=C_R+actRotl(D_R, ACT_ROM_R_BYT(s_R[62])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R3(C_R, D_R, E_R, A_R, B_R, 63); RL(E_R); C_R=B_R+actRotl(C_R, ACT_ROM_R_BYT(s_R[63]));

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* round 5 */
   R4(B_R, C_R, D_R, E_R, A_R, 64); RL(D_R); B_R=A_R+actRotl(B_R, ACT_ROM_R_BYT(s_R[64])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R4(A_R, B_R, C_R, D_R, E_R, 65); RL(C_R); A_R=E_R+actRotl(A_R, ACT_ROM_R_BYT(s_R[65])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R4(E_R, A_R, B_R, C_R, D_R, 66); RL(B_R); E_R=D_R+actRotl(E_R, ACT_ROM_R_BYT(s_R[66])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R4(D_R, E_R, A_R, B_R, C_R, 67); RL(A_R); D_R=C_R+actRotl(D_R, ACT_ROM_R_BYT(s_R[67])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R4(C_R, D_R, E_R, A_R, B_R, 68); RL(E_R); C_R=B_R+actRotl(C_R, ACT_ROM_R_BYT(s_R[68])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R4(B_R, C_R, D_R, E_R, A_R, 69); RL(D_R); B_R=A_R+actRotl(B_R, ACT_ROM_R_BYT(s_R[69])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R4(A_R, B_R, C_R, D_R, E_R, 70); RL(C_R); A_R=E_R+actRotl(A_R, ACT_ROM_R_BYT(s_R[70])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R4(E_R, A_R, B_R, C_R, D_R, 71); RL(B_R); E_R=D_R+actRotl(E_R, ACT_ROM_R_BYT(s_R[71])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R4(D_R, E_R, A_R, B_R, C_R, 72); RL(A_R); D_R=C_R+actRotl(D_R, ACT_ROM_R_BYT(s_R[72])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R4(C_R, D_R, E_R, A_R, B_R, 73); RL(E_R); C_R=B_R+actRotl(C_R, ACT_ROM_R_BYT(s_R[73])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R4(B_R, C_R, D_R, E_R, A_R, 74); RL(D_R); B_R=A_R+actRotl(B_R, ACT_ROM_R_BYT(s_R[74])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R4(A_R, B_R, C_R, D_R, E_R, 75); RL(C_R); A_R=E_R+actRotl(A_R, ACT_ROM_R_BYT(s_R[75])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R4(E_R, A_R, B_R, C_R, D_R, 76); RL(B_R); E_R=D_R+actRotl(E_R, ACT_ROM_R_BYT(s_R[76])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R4(D_R, E_R, A_R, B_R, C_R, 77); RL(A_R); D_R=C_R+actRotl(D_R, ACT_ROM_R_BYT(s_R[77])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R4(C_R, D_R, E_R, A_R, B_R, 78); RL(E_R); C_R=B_R+actRotl(C_R, ACT_ROM_R_BYT(s_R[78])); actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R4(B_R, C_R, D_R, E_R, A_R, 79); RL(D_R); B_R=A_R+actRotl(B_R, ACT_ROM_R_BYT(s_R[79]));

# endif

# if (actRMD160_SPEED_UP > 0)
   /* Add the working vars back into state H[] */
   t = H[0];
   H[0] = H[1] + C_L + D_R;
   H[1] = H[2] + D_L + E_R;
   H[2] = H[3] + E_L + A_R;
   H[3] = H[4] + A_L + B_R;
   H[4] = t    + B_L + C_R;

# endif

}

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/**********************************************************************************************************************
 *  actRMD160Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 * 
 */
VSECPRIM_FUNC(actRETURNCODE) actRMD160Init(VSECPRIM_P2VAR_PARA(actRMD160STRUCT) info)
{
   /* init message digest */
   info->H[0] = 0x67452301UL;
   info->H[1] = 0xEFCDAB89UL;
   info->H[2] = 0x98BADCFEUL;
   info->H[3] = 0x10325476UL;
   info->H[4] = 0xC3D2E1F0UL;
   /* init count variables */
   info->low_count = 0u;
   info->hi_count = 0u;
   info->buffer_used = 0u;

   return actOK;
}

/**********************************************************************************************************************
 *  actRMD160Update()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 * 
 */
VSECPRIM_FUNC(actRETURNCODE) actRMD160Update(
   VSECPRIM_P2VAR_PARA(actRMD160STRUCT) info, VSECPRIM_P2CONST_PARA(actU8) dataIN,
   actLengthType length, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   VSECPRIM_P2VAR_PARA(actU32) H = info->H;
   VSECPRIM_P2VAR_PARA(actU8)  buffer = info->buffer;
   actLengthType diff=0u, blocks, bcount, idxOfLastBlock;

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* update byte count */
   if ((info->low_count + (actU32)length) < info->low_count)
   {
     info->hi_count++;
     if ((info->hi_count) == ((actU32)1u<<29)) /* add carry */
     {
        return actEXCEPTION_LENGTH; /* overflow: 2^64 bit input */
     }
   }
   info->low_count += (actU32)length;

   /* left overs of the last update */
   if (info->buffer_used > 0u)
   {
      diff = actMin(length, actHASH_BLOCK_SIZE_RMD160-info->buffer_used);
      actMemCpyByteArray(&buffer[info->buffer_used], dataIN, diff); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
      info->buffer_used += diff;
      if (info->buffer_used < actHASH_BLOCK_SIZE_RMD160)
      {
         return actOK;
      }
      else
      {
         actRMD160Transform(buffer, H, watchdog);
      }
   }

   /* complete blocks */
   blocks = (length - diff) / actHASH_BLOCK_SIZE_RMD160;
   idxOfLastBlock = diff + (actHASH_BLOCK_SIZE_RMD160 * blocks);

   for (bcount=diff; bcount < idxOfLastBlock; bcount += actHASH_BLOCK_SIZE_RMD160)
   {
     actRMD160Transform(&dataIN[bcount], H, watchdog);
   }

   /* copy left overs into buffer */
   info->buffer_used = length - idxOfLastBlock;
   actMemCpyByteArray(buffer, &dataIN[idxOfLastBlock], (info->buffer_used)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   return actOK;
}

/**********************************************************************************************************************
 *  actRMD160Finalize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 * 
 */
VSECPRIM_FUNC(actRETURNCODE) actRMD160Finalize(
   VSECPRIM_P2VAR_PARA(actRMD160STRUCT) info, VSECPRIM_P2VAR_PARA(actU8) hash,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   actU32 low_bitcount = info->low_count << 3;
   actU32 hi_bitcount = (info->hi_count << 3) | (info->low_count >> 29);
   VSECPRIM_P2VAR_PARA(actU8) buffer = info->buffer;
   VSECPRIM_P2VAR_PARA(actU32) H = info->H;
   actLengthType len = info->low_count & 0x3Fu;
   actLengthType pad_len = (actHASH_BLOCK_SIZE_RMD160-(2u*actRMD160_WORD_SIZE));

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   /* pad byte '0x80' */
   buffer[len] = 0x80u;
   len = len + 1u;

   /* pad zeros */
   actMemClear(&buffer[len], (actLengthType)(actHASH_BLOCK_SIZE_RMD160 - len)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
   if (len > pad_len)
   {
      actRMD160Transform(buffer, H, watchdog);
      actMemClear(buffer, pad_len); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
   }

   /* pad input bit count to last 8 bytes in little endian format */
   actU32toLE (&buffer [pad_len],                     low_bitcount);
   actU32toLE (&buffer [pad_len +actRMD160_WORD_SIZE], hi_bitcount);

   /* transform last block */
   actRMD160Transform(buffer, H, watchdog);

   /* store digest in little endian byte order */
   actCpyU32toLE (hash, H, actHASH_SIZE_RMD160 /actRMD160_WORD_SIZE);

   /* wipe buffer */
   actMemClear(info->buffer, actHASH_BLOCK_SIZE_RMD160); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */

   return actOK;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTRMD160_ENABLED == STD_ON) */
