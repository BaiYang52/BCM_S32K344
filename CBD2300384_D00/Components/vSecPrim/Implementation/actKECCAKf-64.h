/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2020 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  actKECCAKf-64.h
 *        \brief  Implementation for KECCAK-f permutation, based on 64 bit representation of words.
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library actCLib
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

#ifndef ACTKECCAKF64_H
# define ACTKECCAKF64_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actITypes.h"
# include "actUtilities.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define KECCAK_ROUNDS                                                (24u)
# define SLICE_LENGTH                                                 (5u)

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/*  straightforward KECCAKf implementation

 *  state is store lane by lane and plane by plane
 *  X coordinate (addressing sheets)
 *  Y coordinate (addressing planes)
 *  Z coordinate (addressing slices) - bits within a lane
 *  linear addressing is used:
 *  * increment y by 5,
 *  * increment x by 1                                      */

# define VSECPRIM_START_SEC_CONST_64BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM(VSECPRIM_LOCAL, actU64) RC[] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */
{
  /*  round constants  */
  /*  (table on page 8 of KECCAK implementation overview)  */
  0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808AULL, 0x8000000080008000ULL,
  0x000000000000808BULL, 0x0000000080000001ULL, 0x8000000080008081ULL, 0x8000000000008009ULL,
  0x000000000000008AULL, 0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000AULL,
  0x000000008000808BULL, 0x800000000000008BULL, 0x8000000000008089ULL, 0x8000000000008003ULL,
  0x8000000000008002ULL, 0x8000000000000080ULL, 0x000000000000800AULL, 0x800000008000000AULL,
  0x8000000080008081ULL, 0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
};

# define VSECPRIM_STOP_SEC_CONST_64BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define VSECPRIM_START_SEC_CONST_32BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) pi_lane[] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */
{
  /*  provides the (pi) reorder source per lane    */
  /*  (algorithm 3, step 1 of FIPS 202)            */
  /*                                               */
  /*  x,y  <-  (x + 3y) %5,x  --  linear (x + 5y)  */
  /*  -------------------------------------------  */
  /*  0,0  <-            0,0  */    0u, /*  UNused  */
  /*  1,0  <-            1,1  */    6u,
  /*  2,0  <-            2,2  */   12u,
  /*  3,0  <-            3,3  */   18u,
  /*  4,0  <-            4,4  */   24u,
  /*  0,1  <-            3,0  */    3u,
  /*  1,1  <-            4,1  */    9u,
  /*  2,1  <-            0,2  */   10u,
  /*  3,1  <-            1,3  */   16u,
  /*  4,1  <-            2,4  */   22u,
  /*  0,2  <-            1,0  */    1u,
  /*  1,2  <-            2,1  */    7u,
  /*  2,2  <-            3,2  */   13u,
  /*  3,2  <-            4,3  */   19u,
  /*  4,2  <-            0,4  */   20u,
  /*  0,3  <-            4,0  */    4u,
  /*  1,3  <-            0,1  */    5u,
  /*  2,3  <-            1,2  */   11u,
  /*  3,3  <-            2,3  */   17u,
  /*  4,3  <-            3,4  */   23u,
  /*  0,4  <-            2,0  */    2u,
  /*  1,4  <-            3,1  */    8u,
  /*  2,4  <-            4,2  */   14u,
  /*  3,4  <-            0,3  */   15u,
  /*  4,4  <-            1,4  */   21u
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) rho_offset[] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */
{
  /*  gives the rotate offset per lane  */
  /*  (table 2 of FIPS 202)             */
  /*                                    */
  /*  x,y  --  offset                   */
  /*  --------------------------------  */
  /*  0,0  */     0u,      /*  NOT used  */
  /*  1,0  */     1u,
  /*  2,0  */    62u,
  /*  3,0  */    28u,
  /*  4,0  */    27u,
  /*  0,1  */    36u,
  /*  1,1  */    44u,
  /*  2,1  */     6u,
  /*  3,1  */    55u,
  /*  4,1  */    20u,
  /*  0,2  */     3u,
  /*  1,2  */    10u,
  /*  2,2  */    43u,
  /*  3,2  */    25u,
  /*  4,2  */    39u,
  /*  0,3  */    41u,
  /*  1,3  */    45u,
  /*  2,3  */    15u,
  /*  3,3  */    21u,
  /*  4,3  */     8u,
  /*  0,4  */    18u,
  /*  1,4  */     2u,
  /*  2,4  */    61u,
  /*  3,4  */    56u,
  /*  4,4  */    14u
};

# define VSECPRIM_STOP_SEC_CONST_32BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actKECCAKcAbsorbData()
 *********************************************************************************************************************/
/*! \brief          Absorb data into info->state
 *  \details        One data element is a 64 bit word, represented by two 32 bit words.
 *  \param[in,out]  info         (in)  pointer to the KECCAK[c] context
 *                               (out) updated KECCAK[c] context structure
 *  \param[in]      data         pointer to data to be absorbed
 *  \pre            all pointers must be valid
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actKECCAKcAbsorbData(
  VSECPRIM_P2VAR_PARA(actKECCAKcSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) data);

/**********************************************************************************************************************
 *  actKECCAKcSqueezeData()
 *********************************************************************************************************************/
/*! \brief          Squeeze data from info->state into data.
 *  \details        One data element is a 64 bit word, represented by two 32 bit words.
 *  \param[in]      info         pointer to the KECCAK[c] context
 *  \param[in,out]  data         (in)  pointer to buffer to store the squeezed data
 *                               (out) pointer to buffer that stores the squeezed data
 *  \pre            All pointers must be valid
 *                  The buffer referenced by data must provide at least 8 bytes.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actKECCAKcSqueezeData(
  VSECPRIM_P2CONST_PARA(actKECCAKcSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) data);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  actKECCAKcAbsorbData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actKECCAKcAbsorbData(
  VSECPRIM_P2VAR_PARA(actKECCAKcSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) data)
{
  info->state[info->rUsed] ^= actLEtoU64(data); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
}

/**********************************************************************************************************************
 *  actKECCAKcSqueezeData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actKECCAKcSqueezeData(
  VSECPRIM_P2CONST_PARA(actKECCAKcSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) data)
{
  actU64toLE((data), info->state[info->rUsed]); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  KECCAKf()
 *********************************************************************************************************************/
/*! \brief          Initialize the KECCAK[c] algorithm.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to the KECCAK[c] context
 *                               (out) updated KECCAK[c] context structure
 *  \param[in]      watchdog     pointer to watchdog reset function
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) KECCAKf(
  VSECPRIM_P2VAR_PARA(actKECCAKcSTRUCT) info,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  KECCAKf()
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
 */
VSECPRIM_LOCAL_FUNC(void) KECCAKf(
  VSECPRIM_P2VAR_PARA(actKECCAKcSTRUCT) info,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actU32 dst, src, x, y, r;
  actU64 Dx, C[SLICE_LENGTH];

  actL1trigger (watchdog);  /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  for (r = 0u; r < KECCAK_ROUNDS; r++)
  {
    /* theta - XOR bit with column parity; done lane- / slice-wise
     * C[x] = A[x,0] XOR A[x,1] XOR A[x,2] XOR A[x,3] XOR A[x,4]
     * (1) - calculate slice parity      */
    for (x = 0u; x < SLICE_LENGTH; x++)
    {
      C[x] = info->state[x] ^ info->state[x + SLICE_LENGTH] ^ info->state[x + (2u * SLICE_LENGTH)] ^
             info->state[x + (3u * SLICE_LENGTH)] ^ info->state[x + (4u * SLICE_LENGTH)]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
    }

      /* (2) - add slice parity to lanes
       * D[x] = C[x-1] XOR ROT(C[x-1], 1)
       * A[x,y] = A[x,y] XOR D[x] */
    for (x = 0u; x < SLICE_LENGTH; x++)
    {
      Dx = C [(x + 4u) % SLICE_LENGTH] ^ actRotl64 (C [(x + 1u) % SLICE_LENGTH], 1u);

      for (y = 0u; y <= (SLICE_LENGTH * (SLICE_LENGTH - 1u)); y += SLICE_LENGTH)
      {
        info->state[y + x] ^= Dx; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
      }
    }

    /*  rho | pi  - rotate and reorder lanes: B[y, 2x + 3y] = ROT(A[x,y], r[x,y])
    * rotation taken from rho_offset, reordering taken from pi_lane
    * info->state [0] = actRotl64 (info->state [0], 0); --  lane [0,0] NOT touched by rho / pi  */

    /*  rotate and save lane [1,0]; lane[1,0] will temporarily be stored in Dx */
    Dx = actRotl64(info->state[1], rho_offset[1]);

    dst = 1u;
    src = pi_lane[1];

    while(src != 1u)
    {
      info->state[dst] = actRotl64(info->state[src], rho_offset[src]); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
      dst = src;
      src = pi_lane[dst];
    }

    /* write final lane [0,2] from lane [1,0]; lane[1,0] is currently stored in Dx */
    info->state[dst] = Dx; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */

    /* chi  -  XOR bits within rows
     * A[x,y] = B[x,y] XOR (NOT(B[x+1,y]) AND B[x+2,y]) */
    for (y = 0u; y <= (SLICE_LENGTH * (SLICE_LENGTH - 1u)); y += SLICE_LENGTH)
    {
      for (x = 0u; x < SLICE_LENGTH; x++)
      {
        C[x] = info->state[y + x]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
      }

      for (x = 0u; x < SLICE_LENGTH; x++)
      {
        info->state[y + x] ^= (~C[(x + 1u) % SLICE_LENGTH]) & C[(x + 2u) % SLICE_LENGTH]; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
      }
    }

    /* iota
     * A[0,0] = A[0,0] XOR RC, RC = round constant
     * info->state [0] ^= RC [r]; */
    info->state[0] ^= RC[r]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */

    actL2trigger (watchdog);  /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }

  actL1trigger (watchdog);  /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /*  ACTKECCAKF64_H  */

/**********************************************************************************************************************
 *  END OF FILE: actKECCAKf-64.h
 *********************************************************************************************************************/
