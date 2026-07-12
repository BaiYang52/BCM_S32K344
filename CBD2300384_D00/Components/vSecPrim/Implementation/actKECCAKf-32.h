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
/*!        \file  actKECCAKf-32.h
 *        \brief  Implementation for KECCAK-f permutation, based on 32-bit representation of words.
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

#ifndef ACTKECCAKF32_H
# define ACTKECCAKF32_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actITypes.h"
# include "actUtilities.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define KECCAK_ROUNDS                                                (24u)
# define SLICE_LENGTH_UINT32                                          (10u)
# define WORD_NUMBER_64BITWORD_UINT32                                 (2u)

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
/*  straightforward KECCAKf implementation

 *  state is store lane by lane and plane by plane
 *  each LANE consists of two 32bit words, LSW stored first
 *  X coordinate (addressing sheets)
 *  Y coordinate (addressing planes)
 *  Z coordinate (addressing slices) - bits within a lane
 *  linear addressing is used:
 *  - increment y by 10,
 *  - increment x by  2                                     */

# define VSECPRIM_START_SEC_CONST_32BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) RC[] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */
{
  /*  round constants
   *  table on page 8 of KECCAK implementation overview, split to 32-bit words and stored LSW first  */
  0x00000001uL, 0x00000000uL, 0x00008082uL, 0x00000000uL, 0x0000808AuL, 0x80000000uL, 0x80008000uL, 0x80000000uL,
  0x0000808BuL, 0x00000000uL, 0x80000001uL, 0x00000000uL, 0x80008081uL, 0x80000000uL, 0x00008009uL, 0x80000000uL,
  0x0000008AuL, 0x00000000uL, 0x00000088uL, 0x00000000uL, 0x80008009uL, 0x00000000uL, 0x8000000AuL, 0x00000000uL,
  0x8000808BuL, 0x00000000uL, 0x0000008BuL, 0x80000000uL, 0x00008089uL, 0x80000000uL, 0x00008003uL, 0x80000000uL,
  0x00008002uL, 0x80000000uL, 0x00000080uL, 0x80000000uL, 0x0000800AuL, 0x00000000uL, 0x8000000AuL, 0x80000000uL,
  0x80008081uL, 0x80000000uL, 0x00008080uL, 0x80000000uL, 0x80000001uL, 0x00000000uL, 0x80008008uL, 0x80000000uL
};

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
 *  actKECCAKRotL64()
 *********************************************************************************************************************/
/*! \brief          Rotates a 64 bit word by n bit to the left.
 *  \details        The 64 bit word x is represented by two 32 bit words. The function assumes little endian word
 *                  order.
 *  \param[in,out]  tgt  (in)  pointer to buffer to store the rotated word x
 *                       (out) pointer to buffer, storing the rotated word x
 *  \param[in]      x    word to be rotated
 *  \param[in]      n    number of bits by which x shall be rotated
 *  \pre            all pointers must be valid
 *                  The buffer referenced by tgt must provide at least 2 bytes.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actKECCAKRotL64(
  VSECPRIM_P2VAR_PARA(actU32) tgt,
  VSECPRIM_P2CONST_PARA(actU32) x,
  const actU32 n);

/**********************************************************************************************************************
 *  actKECCAKCpy64()
 *********************************************************************************************************************/
/*! \brief          Copies the 64 bit word src to the 64 bit word tgt.
 *  \details        The 64 bit word src is represented by two 32 bit words
 *  \param[in,out]  tgt  (in)  pointer to buffer to store the copied values
 *                       (out) pointer to buffer storing the copied values
 *  \param[in]      src  pointer to buffer storing the word to be copied
 *  \pre            All pointers must be valid.
 *                  The buffer referenced by tgt must provide at least 2 elements of size sizeof(uint32).
 *                  The buffer referenced by src must provide at least 2 elements of size sizeof(uint32).
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actKECCAKCpy64(
  VSECPRIM_P2VAR_PARA(actU32) tgt,
  VSECPRIM_P2CONST_PARA(actU32) src);

/**********************************************************************************************************************
 *  actKECCAKNot64()
 *********************************************************************************************************************/
/*! \brief          Computes ~val where val is a 64 bit word and stores the result in tgt.
 *  \details        The 64 bit word val is represented by two 32 bit words.
 *  \param[in,out]  tgt  (in)  pointer to buffer to store the result ~val
 *                       (out) pointer to buffer storing the result ~val
 *  \param[in]      val  pointer to buffer storing the word to be negated
 *  \pre            All pointers must be valid.
 *                  The buffer referenced by tgt must provide at least 2 elements of size sizeof(uint32).
 *                  The buffer referenced by val must provide at least 2 elements of size sizeof(uint32).
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actKECCAKNot64(
  VSECPRIM_P2VAR_PARA(actU32) tgt,
  VSECPRIM_P2CONST_PARA(actU32) val);

/**********************************************************************************************************************
 *  actKECCAKAnd64()
 *********************************************************************************************************************/
/*! \brief          Links two 64 bit words by AND.
 *  \details        The 64 bit words tgt and val are represented by two 32 bit words, each. The result is
 *                  stored in tgt.
 *  \param[in,out]  tgt  (in)  pointer to buffer storing the first word
 *                       (out) pointer to buffer storing tgt AND val
 *  \param[in]      val  pointer to buffer storing the second word
 *  \pre            All pointers must be valid.
 *                  The buffer referenced by tgt must provide at least 2 elements of size sizeof(uint32).
 *                  The buffer referenced by val must provide at least 2 elements of size sizeof(uint32).
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actKECCAKAnd64(
  VSECPRIM_P2VAR_PARA(actU32) tgt,
  VSECPRIM_P2CONST_PARA(actU32) val);

/**********************************************************************************************************************
 *  actKECCAKXor64()
 *********************************************************************************************************************/
/*! \brief          Links two 64 bit words by XOR.
 *  \details        The 64 bit words tgt and val are represented by two 32 bit words, each. The result is
 *                  stored in tgt.
 *  \param[in,out]  tgt  (in)  pointer to buffer storing the first word
 *                       (out) pointer to buffer storing tgt XOR val
 *  \param[in]      val  pointer to buffer storing the second word
 *  \pre            All pointers must be valid.
 *                  The buffer referenced by tgt must provide at least 2 elements of size sizeof(uint32).
 *                  The buffer referenced by val must provide at least 2 elements of size sizeof(uint32).
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actKECCAKXor64(
  VSECPRIM_P2VAR_PARA(actU32) tgt,
  VSECPRIM_P2CONST_PARA(actU32) val);

/**********************************************************************************************************************
 *  actKECCAKfPermutationRhoPi()
 *********************************************************************************************************************/
/*! \brief          Squeeze data from info->state into data.
 *  \details        -
 *  \param[in,out]  info         (in)  pointer to the KECCAK[c] context
 *                               (out) pointer to the KECCAK[c] context, state modified
 *  \param[in,out]  lane10       (in)  pointer to buffer to store intermediate data
 *                               (out) pointer to buffer that stores the intermediate data
 *  \pre            all pointers must be valid
 *                  The buffer referenced by lane10 must provide at least 2 elements of size sizeof(uint32).
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actKECCAKfPermutationRhoPi(
  VSECPRIM_P2VAR_PARA(actKECCAKcSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU32) lane10);

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
  info->state[(info->rUsed * 2u)] ^= actLEtoU32(&((data)[0])); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  info->state[(info->rUsed * 2u) + 1u] ^= actLEtoU32(&((data)[4])); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
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
  actU32toLE(&((data)[0]), info->state[info->rUsed * 2u]); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */
  actU32toLE(&((data)[4]), info->state[(info->rUsed * 2u) + 1u]); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */
}

/**********************************************************************************************************************
 *  actKECCAKRotL64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actKECCAKRotL64(
  VSECPRIM_P2VAR_PARA(actU32) tgt,
  VSECPRIM_P2CONST_PARA(actU32) x,
  const actU32 n)
{
  if (n <= 32u)
  {
    (tgt)[0] = (((x)[0] << (n)) | ((x)[1] >> (32u - (n)))); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
    (tgt)[1] = (((x)[1] << (n)) | ((x)[0] >> (32u - (n)))); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  }
  else
  {
    (tgt)[1] = (((x)[0] << (n - 32u)) | ((x)[1] >> (32u - (n - 32u)))); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
    (tgt)[0] = (((x)[1] << (n - 32u)) | ((x)[0] >> (32u - (n - 32u)))); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  }
}

/**********************************************************************************************************************
 *  actKECCAKCpy64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actKECCAKCpy64(
  VSECPRIM_P2VAR_PARA(actU32) tgt,
  VSECPRIM_P2CONST_PARA(actU32) src)
{
  (tgt)[0] = (src)[0]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  (tgt)[1] = (src)[1]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
}

/**********************************************************************************************************************
 *  actKECCAKNot64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actKECCAKNot64(
  VSECPRIM_P2VAR_PARA(actU32) tgt,
  VSECPRIM_P2CONST_PARA(actU32) val)
{
  (tgt)[0] = ~(val)[0]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  (tgt)[1] = ~(val)[1]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
}

/**********************************************************************************************************************
 *  actKECCAKAnd64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actKECCAKAnd64(
  VSECPRIM_P2VAR_PARA(actU32) tgt,
  VSECPRIM_P2CONST_PARA(actU32) val)
{
  (tgt)[0] &= (val)[0]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  (tgt)[1] &= (val)[1]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
}

/**********************************************************************************************************************
 *  actKECCAKXor64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actKECCAKXor64(
  VSECPRIM_P2VAR_PARA(actU32) tgt,
  VSECPRIM_P2CONST_PARA(actU32) val)
{
  (tgt)[0] ^= (val)[0]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  (tgt)[1] ^= (val)[1]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
}

/**********************************************************************************************************************
 *  actKECCAKfPermutationRhoPi()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actKECCAKfPermutationRhoPi(
  VSECPRIM_P2VAR_PARA(actKECCAKcSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU32) lane10)
{
  actU32 dst, src;

  /*  rho | pi  - rotate and reorder lanes
   *              rotation taken from rho_offset
   *              reordering taken from pi_lane
   * info->state [0] = actRotl64 (info->state [0], 0); --  lane [0,0] NOT touched by rho / pi
   * B[y, 2x + 3y] = ROT(A[x,y], r[x,y]) */

  /* rotate and save lane [1,0] */
  actKECCAKRotL64(lane10, &(info->state[2]), rho_offset[1]); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */

  dst = 1u;
  src = pi_lane[1];
  while (src != 1u)
  {
    /* Rotate info->state[src] by rho_offset[src], save it in info->state[dst] */
    if (rho_offset[src] <= 32u)
    {
      actKECCAKRotL64(&(info->state[dst * 2u]), &(info->state[src * 2u]), rho_offset[src]); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
    else
    {
      actKECCAKRotL64(&(info->state[dst * 2u]), &(info->state[src * 2u]), rho_offset[src]); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }

    dst = src;
    src = pi_lane[dst];
  }

  /* write final lane [0,2] from lane [1,0] */
  actKECCAKCpy64(&(info->state[dst * 2u]), lane10); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
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
  actU32 x, y, r;
  actU32 Dx[WORD_NUMBER_64BITWORD_UINT32], C[SLICE_LENGTH_UINT32];

  actL1trigger (watchdog);  /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  for (r = 0u; r < KECCAK_ROUNDS; r++)
  {
    /* theta - XOR bit with column parity; done lane- / slice-wise
     * C[x] = A[x,0] XOR A[x,1] XOR A[x,2] XOR A[x,3] XOR A[x,4]
     * (1) - calculate slice parity      */

      for (x = 0u; x <= 8u; x += WORD_NUMBER_64BITWORD_UINT32)
      {
        /* C [x]  = info->state [x + (0 * 10)]; */
        actKECCAKCpy64 (&(C [x]), &(info->state [x]));             /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */
        /* C [x] ^= info->state [x + (1 * 10)]; */
        actKECCAKXor64(&(C [x]), &(info->state [x + (1u *SLICE_LENGTH_UINT32)]));  /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */
        /* C [x] ^= info->state [x + (2 * 10)]; */
        actKECCAKXor64(&(C [x]), &(info->state [x + (2u *SLICE_LENGTH_UINT32)])); /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */
        /* C [x] ^= info->state [x + (3 * 10)]; */
        actKECCAKXor64(&(C [x]), &(info->state [x + (3u *SLICE_LENGTH_UINT32)]));  /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */
        /* C [x] ^= info->state [x + (4 * 10)]; */
        actKECCAKXor64(&(C [x]), &(info->state [x + (4u *SLICE_LENGTH_UINT32)]));  /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */
      }

      /* (2) - add slice parity to lanes
       * D[x] = C[x-1] XOR ROT(C[x-1], 1)
       * A[x,y] = A[x,y] XOR D[x] */
      for (x = 0u; x <= 8u; x += WORD_NUMBER_64BITWORD_UINT32)
      {
        actKECCAKRotL64 (Dx, &(C [(x + 2u) % SLICE_LENGTH_UINT32]), 1u); /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */
        actKECCAKXor64(Dx, &(C [(x + 8u) % SLICE_LENGTH_UINT32])); /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */

        for (y = 0u; y <= (4u * SLICE_LENGTH_UINT32); y += SLICE_LENGTH_UINT32)
        {
          /* info->state [y + x] ^= Dx; */
          actKECCAKXor64(&(info->state[y + x]), Dx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */
        }
      }

      /*  rho | pi  - rotate and reorder lanes
       * B[y, 2x + 3y] = ROT(A[x,y], r[x,y]) */
      actKECCAKfPermutationRhoPi(info, Dx);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */

      /* chi  -  XOR bits within rows
       * A[x,y] = B[x,y] XOR (NOT(B[x+1,y]) AND B[x+2,y]) */
      for (y = 0u; y <= (4u *SLICE_LENGTH_UINT32); y += SLICE_LENGTH_UINT32)
      {
        for (x = 0u; x <= 8u; x += WORD_NUMBER_64BITWORD_UINT32)/* for (x = 0; x < 5; x++) */
        {
          /* C [x] = info->state [y + x]; */
          actKECCAKCpy64(&(C[x]), &(info->state[y + x])); /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */
        }

        for (x = 0u; x <= 8u; x += WORD_NUMBER_64BITWORD_UINT32)
        {
          actKECCAKNot64 (Dx, &(C [(x + 2u) % SLICE_LENGTH_UINT32])); /* ~C [(x +1) %5] */ /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */
          actKECCAKAnd64 (Dx, &(C [(x + 4u) % SLICE_LENGTH_UINT32])); /* &C [(x +2) %5] */ /* SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER */
          actKECCAKXor64(&(info->state [y + x]), Dx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */
        }

      }

      /* iota
       * A[0,0] = A[0,0] XOR RC, RC = round constant
       * info->state [0] ^= RC [r]; */
      actKECCAKXor64(&(info->state [0]), &(RC [r * 2u])); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */

      actL2trigger (watchdog);  /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }

  actL1trigger (watchdog);  /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /*  ACTKECCAKF32_H  */

/**********************************************************************************************************************
 *  END OF FILE: actKECCAKf-32.h
 *********************************************************************************************************************/
