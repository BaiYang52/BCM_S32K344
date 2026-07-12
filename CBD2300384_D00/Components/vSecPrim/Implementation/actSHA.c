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
/*!        \file  actSHA.c
 *        \brief  SHA-1 implementation.
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

#define ACTSHA_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actISHA.h"
#include "actUtilities.h"
#include "actWatchdog.h"
#include "actConfig.h"

#if (VSECPRIM_ACTSHA_ENABLED == STD_ON)

/* PRQA S 0850 EOF */ /* MD_MSR_MacroArgumentEmpty */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define actSHA_WORD_SIZE                                             (4u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

 /* The SHA f()-functions */

# define f1(x, y, z)                                                  ((z)^((x)&((y)^(z))))
# define f2(x, y, z)                                                  ((x)^(y)^(z))
# define f3(x, y, z)                                                  (((x)&(y))|((z)&((x)|(y))))
# define f4(x, y, z)                                                  ((x)^(y)^(z))

/* blk0(), blk1() functions working on actSHATransform() local workspace 'W' */
# define blk0(W, i)                                                   ((W)[(i)])
# if (actSHA_SPEED_UP > 2) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
   /* blk1() as macro */
#  define blk1(W, i)                                                  actRotl((W)[((i)+13u)&15u]^(W)[((i)+8u)&15u]^(W)[((i)+2u)&15u]^(W)[(i)&15u], 1u)
# endif

/* SHA-1 bit rotate functions */
# if (actSHA_SPEED_UP > 0) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */

#  define actRotl5(x)                                                 (actRotl((x), 5))
#  define actRotl30(x)                                                (actRotl((x), 30))
# endif

/* R1, R2, R3, R4 are the different operations used in SHA1 */

# define R0(v, w, x, y, z, i)                                         ((z) += f1((w), (x), (y)) + blk0(W, (i)) + 0x5A827999uL + actRotl5((v)))
# define R1(v, w, x, y, z, i)                                         ((W)[(i)&15u]) = blk1(W, (i)); ((z) += f1((w), (x), (y)) + ((W)[(i)&15u]) + 0x5A827999uL + actRotl5((v)))
# define R2(v, w, x, y, z, i)                                         ((W)[(i)&15u]) = blk1(W, (i)); ((z) += f2((w), (x), (y)) + ((W)[(i)&15u]) + 0x6ED9EBA1uL + actRotl5((v)))
# define R3(v, w, x, y, z, i)                                         ((W)[(i)&15u]) = blk1(W, (i)); ((z) += f3((w), (x), (y)) + ((W)[(i)&15u]) + 0x8F1BBCDCuL + actRotl5((v)))
# define R4(v, w, x, y, z, i)                                         ((W)[(i)&15u]) = blk1(W, (i)); ((z) += f4((w), (x), (y)) + ((W)[(i)&15u]) + 0xCA62C1D6uL + actRotl5((v)))

# define RL(w)                                                        ((w)=actRotl30((w)))

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
  actSHATransform()
 **********************************************************************************************************************/
/*! \brief          Perform SHA transformation.
 *  \details        This function performs the SHA transformation.
 *  \param[in]      in        Input data pointer
 *  \param[in,out]  H         Message digest
 *  \param[in]      watchdog  Pointer to watchdog function
 *  \pre            H has to point to an array of at least 20 Bytes of size.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actSHATransform(
  VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU32) H,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# if (!(actSHA_SPEED_UP > 2)) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
/**********************************************************************************************************************
  blk1()
 **********************************************************************************************************************/
/*! \brief          Perform blk1 operation.
 *  \details        This function performs the blk1 operation.
 *  \param[in,out]  W     Lookup table pointer.
 *  \param[in]      i     Index in lookup table.
 *  \return         Value from lookup table entry
 *  \pre            W must be of size 512 byte
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actU32) blk1(VSECPRIM_P2CONST_PARA(actU32) W, actU32 i);
# endif

# if (actSHA_SPEED_UP == 0u) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/**********************************************************************************************************************
  actRotl5()
 **********************************************************************************************************************/
/*! \brief          Rotate word by 5 bits left
 *  \details        This function rotate the provided word by 5 bits left
 *  \param[in]      x     Word to be shifted
 *  \return         Shifted value
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actU32) actRotl5(actU32 x);

/**********************************************************************************************************************
  actRotl30()
 **********************************************************************************************************************/
/*! \brief          Rotate word by 5 bits left
 *  \details        This function rotate the provided word by 5 bits left
 *  \param[in]      x     Word to be shifted
 *  \return         Shifted value
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actU32) actRotl30(actU32 x);
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  actSHATransform
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actSHATransform(
  VSECPRIM_P2CONST_PARA(actU8) in,
  VSECPRIM_P2VAR_PARA(actU32) H,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void)) /* PRQA S 3206 */ /* MD_VSECPRIM_WATCHDOG */
{
  actU32 W[16] = { 0u };
# if (actSHA_SPEED_UP < 2u) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
  actU32 t;
  actLengthType i;
# endif

  /* Copy H[] to working vars */
  actU32 Aval = H[0];
  actU32 Bval = H[1];
  actU32 Cval = H[2];
  actU32 Dval = H[3];
  actU32 Eval = H[4];

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* interpret block 'in' as BIG endian 32 bit values */
   actCpyBEtoU32 (W, in, actHASH_BLOCK_SIZE_SHA); /* SBSW_VSECPRIM_CALL_FUNCTION */

# if (actSHA_SPEED_UP > 1) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
   /* 4 rounds of 20 operations each. Loops unrolled. */
   /* round 1 */
   /* PRQA S 1338, 2983, 3112 5 */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R0(Aval, Bval, Cval, Dval, Eval, 0u); RL(Bval); actL3trigger(watchdog);   R0(Eval, Aval, Bval, Cval, Dval, 1u); RL(Aval); actL3trigger(watchdog);   R0(Dval, Eval, Aval, Bval, Cval, 2u); RL(Eval); actL3trigger(watchdog);   R0(Cval, Dval, Eval, Aval, Bval, 3u); RL(Dval); actL3trigger(watchdog);
   R0(Bval, Cval, Dval, Eval, Aval, 4u); RL(Cval); actL3trigger(watchdog);   R0(Aval, Bval, Cval, Dval, Eval, 5u); RL(Bval); actL3trigger(watchdog);   R0(Eval, Aval, Bval, Cval, Dval, 6u); RL(Aval); actL3trigger(watchdog);   R0(Dval, Eval, Aval, Bval, Cval, 7u); RL(Eval); actL3trigger(watchdog);
   R0(Cval, Dval, Eval, Aval, Bval, 8u); RL(Dval); actL3trigger(watchdog);   R0(Bval, Cval, Dval, Eval, Aval, 9u); RL(Cval); actL3trigger(watchdog);   R0(Aval, Bval, Cval, Dval, Eval, 10u); RL(Bval); actL3trigger(watchdog);   R0(Eval, Aval, Bval, Cval, Dval, 11u); RL(Aval); actL3trigger(watchdog);
   R0(Dval, Eval, Aval, Bval, Cval, 12u); RL(Eval); actL3trigger(watchdog);   R0(Cval, Dval, Eval, Aval, Bval, 13u); RL(Dval); actL3trigger(watchdog);   R0(Bval, Cval, Dval, Eval, Aval, 14u); RL(Cval); actL3trigger(watchdog);   R0(Aval, Bval, Cval, Dval, Eval, 15u); RL(Bval); actL3trigger(watchdog);
   R1(Eval, Aval, Bval, Cval, Dval, 16u); RL(Aval); actL3trigger(watchdog);   R1(Dval, Eval, Aval, Bval, Cval, 17u); RL(Eval); actL3trigger(watchdog);   R1(Cval, Dval, Eval, Aval, Bval, 18u); RL(Dval); actL3trigger(watchdog);   R1(Bval, Cval, Dval, Eval, Aval, 19u); RL(Cval);
   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* round 2 */
   /* PRQA S 1338, 2983, 3112 5 */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R2(Aval, Bval, Cval, Dval, Eval, 20u); RL(Bval); actL3trigger(watchdog);   R2(Eval, Aval, Bval, Cval, Dval, 21u); RL(Aval); actL3trigger(watchdog);   R2(Dval, Eval, Aval, Bval, Cval, 22u); RL(Eval); actL3trigger(watchdog);   R2(Cval, Dval, Eval, Aval, Bval, 23u); RL(Dval); actL3trigger(watchdog);
   R2(Bval, Cval, Dval, Eval, Aval, 24u); RL(Cval); actL3trigger(watchdog);   R2(Aval, Bval, Cval, Dval, Eval, 25u); RL(Bval); actL3trigger(watchdog);   R2(Eval, Aval, Bval, Cval, Dval, 26u); RL(Aval); actL3trigger(watchdog);   R2(Dval, Eval, Aval, Bval, Cval, 27u); RL(Eval); actL3trigger(watchdog);
   R2(Cval, Dval, Eval, Aval, Bval, 28u); RL(Dval); actL3trigger(watchdog);   R2(Bval, Cval, Dval, Eval, Aval, 29u); RL(Cval); actL3trigger(watchdog);   R2(Aval, Bval, Cval, Dval, Eval, 30u); RL(Bval); actL3trigger(watchdog);   R2(Eval, Aval, Bval, Cval, Dval, 31u); RL(Aval); actL3trigger(watchdog);
   R2(Dval, Eval, Aval, Bval, Cval, 32u); RL(Eval); actL3trigger(watchdog);   R2(Cval, Dval, Eval, Aval, Bval, 33u); RL(Dval); actL3trigger(watchdog);   R2(Bval, Cval, Dval, Eval, Aval, 34u); RL(Cval); actL3trigger(watchdog);   R2(Aval, Bval, Cval, Dval, Eval, 35u); RL(Bval); actL3trigger(watchdog);
   R2(Eval, Aval, Bval, Cval, Dval, 36u); RL(Aval); actL3trigger(watchdog);   R2(Dval, Eval, Aval, Bval, Cval, 37u); RL(Eval); actL3trigger(watchdog);   R2(Cval, Dval, Eval, Aval, Bval, 38u); RL(Dval); actL3trigger(watchdog);   R2(Bval, Cval, Dval, Eval, Aval, 39u); RL(Cval);
   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* round 3 */
   /* PRQA S 1338, 2983, 3112 5 */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R3(Aval, Bval, Cval, Dval, Eval, 40u); RL(Bval); actL3trigger(watchdog);   R3(Eval, Aval, Bval, Cval, Dval, 41u); RL(Aval); actL3trigger(watchdog);   R3(Dval, Eval, Aval, Bval, Cval, 42u); RL(Eval); actL3trigger(watchdog);   R3(Cval, Dval, Eval, Aval, Bval, 43u); RL(Dval); actL3trigger(watchdog);
   R3(Bval, Cval, Dval, Eval, Aval, 44u); RL(Cval); actL3trigger(watchdog);   R3(Aval, Bval, Cval, Dval, Eval, 45u); RL(Bval); actL3trigger(watchdog);   R3(Eval, Aval, Bval, Cval, Dval, 46u); RL(Aval); actL3trigger(watchdog);   R3(Dval, Eval, Aval, Bval, Cval, 47u); RL(Eval); actL3trigger(watchdog);
   R3(Cval, Dval, Eval, Aval, Bval, 48u); RL(Dval); actL3trigger(watchdog);   R3(Bval, Cval, Dval, Eval, Aval, 49u); RL(Cval); actL3trigger(watchdog);   R3(Aval, Bval, Cval, Dval, Eval, 50u); RL(Bval); actL3trigger(watchdog);   R3(Eval, Aval, Bval, Cval, Dval, 51u); RL(Aval); actL3trigger(watchdog);
   R3(Dval, Eval, Aval, Bval, Cval, 52u); RL(Eval); actL3trigger(watchdog);   R3(Cval, Dval, Eval, Aval, Bval, 53u); RL(Dval); actL3trigger(watchdog);   R3(Bval, Cval, Dval, Eval, Aval, 54u); RL(Cval); actL3trigger(watchdog);   R3(Aval, Bval, Cval, Dval, Eval, 55u); RL(Bval); actL3trigger(watchdog);
   R3(Eval, Aval, Bval, Cval, Dval, 56u); RL(Aval); actL3trigger(watchdog);   R3(Dval, Eval, Aval, Bval, Cval, 57u); RL(Eval); actL3trigger(watchdog);   R3(Cval, Dval, Eval, Aval, Bval, 58u); RL(Dval); actL3trigger(watchdog);   R3(Bval, Cval, Dval, Eval, Aval, 59u); RL(Cval);
   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* round 4 */
   /* PRQA S 1338, 2983, 3112 5 */ /* MD_MSR_DummyStmt */ /*lint -e438 */
   R4(Aval, Bval, Cval, Dval, Eval, 60u); RL(Bval); actL3trigger(watchdog);   R4(Eval, Aval, Bval, Cval, Dval, 61u); RL(Aval); actL3trigger(watchdog);   R4(Dval, Eval, Aval, Bval, Cval, 62u); RL(Eval); actL3trigger(watchdog);   R4(Cval, Dval, Eval, Aval, Bval, 63u); RL(Dval); actL3trigger(watchdog);
   R4(Bval, Cval, Dval, Eval, Aval, 64u); RL(Cval); actL3trigger(watchdog);   R4(Aval, Bval, Cval, Dval, Eval, 65u); RL(Bval); actL3trigger(watchdog);   R4(Eval, Aval, Bval, Cval, Dval, 66u); RL(Aval); actL3trigger(watchdog);   R4(Dval, Eval, Aval, Bval, Cval, 67u); RL(Eval); actL3trigger(watchdog);
   R4(Cval, Dval, Eval, Aval, Bval, 68u); RL(Dval); actL3trigger(watchdog);   R4(Bval, Cval, Dval, Eval, Aval, 69u); RL(Cval); actL3trigger(watchdog);   R4(Aval, Bval, Cval, Dval, Eval, 70u); RL(Bval); actL3trigger(watchdog);   R4(Eval, Aval, Bval, Cval, Dval, 71u); RL(Aval); actL3trigger(watchdog);
   R4(Dval, Eval, Aval, Bval, Cval, 72u); RL(Eval); actL3trigger(watchdog);   R4(Cval, Dval, Eval, Aval, Bval, 73u); RL(Dval); actL3trigger(watchdog);   R4(Bval, Cval, Dval, Eval, Aval, 74u); RL(Cval); actL3trigger(watchdog);   R4(Aval, Bval, Cval, Dval, Eval, 75u); RL(Bval); actL3trigger(watchdog);
   R4(Eval, Aval, Bval, Cval, Dval, 76u); RL(Aval); actL3trigger(watchdog);   R4(Dval, Eval, Aval, Bval, Cval, 77u); RL(Eval); actL3trigger(watchdog);   R4(Cval, Dval, Eval, Aval, Bval, 78u); RL(Dval); actL3trigger(watchdog);   R4(Bval, Cval, Dval, Eval, Aval, 79u); RL(Cval);
# else
  /* 4 rounds of 20 operations each. */
  /* round 1 */
  for (i = 0u; i < 16u; ++i)
  {
    R0(Aval, Bval, Cval, Dval, Eval, i);
    t = Eval;
    Eval = Dval;
    Dval = Cval;
    Cval = actRotl30(Bval);
    Bval = Aval;
    Aval = t;
    actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }
  for (; i < 20u; ++i)
  {
    R1(Aval, Bval, Cval, Dval, Eval, i); /* SBSW_VSECPRIM_CALL_FUNCTION */ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
    t = Eval;
    Eval = Dval;
    Dval = Cval;
    Cval = actRotl30(Bval);
    Bval = Aval;
    Aval = t;
    actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }
  actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* round 2 */
  for (; i < 40u; ++i)
  {
    R2(Aval, Bval, Cval, Dval, Eval, i); /* SBSW_VSECPRIM_CALL_FUNCTION */ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
    t = Eval;
    Eval = Dval;
    Dval = Cval;
    Cval = actRotl30(Bval);
    Bval = Aval;
    Aval = t;
    actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }
  actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* round 3 */
  for (; i < 60u; ++i)
  {
    R3(Aval, Bval, Cval, Dval, Eval, i); /* SBSW_VSECPRIM_CALL_FUNCTION */ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
    t = Eval;
    Eval = Dval;
    Dval = Cval;
    Cval = actRotl30(Bval);
    Bval = Aval;
    Aval = t;
    actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }
  actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* round 4 */
  for (; i < 80u; ++i)
  {
    R4(Aval, Bval, Cval, Dval, Eval, i); /* SBSW_VSECPRIM_CALL_FUNCTION */ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
    t = Eval;
    Eval = Dval;
    Dval = Cval;
    Cval = actRotl30(Bval);
    Bval = Aval;
    Aval = t;
    actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }
# endif

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* Add the working vars back into state H[] */
  H[0] += Aval; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
  H[1] += Bval; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
  H[2] += Cval; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
  H[3] += Dval; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
  H[4] += Eval; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
}

# if (!(actSHA_SPEED_UP > 2)) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
/***********************************************************************************************************************
 *  blk1
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actU32) blk1(VSECPRIM_P2CONST_PARA(actU32) W, actU32 i)
{
  actU32 t = W[(i + 13u) & 15u] ^ W[(i + 8u) & 15u] ^ W[(i + 2u) & 15u] ^ W[i & 15u];
  t = (t << 1) | (((t & 0x80000000uL) == 0x80000000uL) ? 1u : 0u);
  return t;
}
# endif

# if (actSHA_SPEED_UP == 0) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/***********************************************************************************************************************
 *  actRotl5
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actU32) actRotl5(actU32 x)
{
  actU8 t_hi5 = (actU8) actGetByte(x, 3u); /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
  t_hi5 >>= 3;
  return (x << 5) | (actU32) t_hi5;
}

/***********************************************************************************************************************
 *  actRotl30
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actU32) actRotl30(actU32 x)
{
  actU8 t_lo2 = (actU8) actGetByte(x, 0u); /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
  t_lo2 <<= 6u;
  return ((actU32) t_lo2 << 24u) | (x >> 2u);
}
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  actSHAInit
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHAInit(VSECPRIM_P2VAR_PARA(actSHASTRUCT) info)
{
  /* init message digest */
  info->H[0] = 0x67452301UL; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
  info->H[1] = 0xEFCDAB89UL; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
  info->H[2] = 0x98BADCFEUL; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
  info->H[3] = 0x10325476UL; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
  info->H[4] = 0xC3D2E1F0UL; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
  /* init count variables */
  info->low_count = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->hi_count = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->buffer_used = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */

  return actOK;
}

/***********************************************************************************************************************
 *  actSHAUpdate
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHAUpdate(
  VSECPRIM_P2VAR_PARA(actSHASTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN,
  actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2VAR_PARA(actU32) H = info->H; /* PRQA S 0781 */ /* MD_VSECPRIM_5.6 */
  VSECPRIM_P2VAR_PARA(actU8) buffer = info->buffer; /* PRQA S 0781 */ /* MD_VSECPRIM_5.6 */
  actLengthType diff = 0u;
  actRETURNCODE retVal = actOK;
  actLengthType lastBlockEnd;
  actLengthType bcount;
  actU8 finished = FALSE;

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* Update byte count */
  if ((info->low_count + (actU32) length) < info->low_count)
  {
    info->hi_count++; /* SBSW_VSECPRIM_WRITE_POINTER */
    if ((info->hi_count) == ((actU32) 1u << 29))  /* add carry */
    {
      retVal = actEXCEPTION_LENGTH; /* overflow: 2^64 bit input */
    }
  }

  if (retVal == actOK)
  {
    info->low_count += (actU32) length; /* SBSW_VSECPRIM_WRITE_POINTER */

    /* Left overs of the last update */
    if (info->buffer_used > 0u)
    {
      diff = (actLengthType)actMin(length, (actHASH_BLOCK_SIZE_SHA - info->buffer_used));
      actMemCpyByteArray(&buffer[info->buffer_used], dataIN, diff); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
      info->buffer_used += (actU16)(diff & 0xFFu); /* SBSW_VSECPRIM_WRITE_POINTER */
      if (info->buffer_used < actHASH_BLOCK_SIZE_SHA)
      {
        retVal = actOK; /* Operation is finished */
        finished = TRUE;
      }
      else
      {
        actSHATransform(buffer, H, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
      }
    }

    if (finished == FALSE)
    {
      /* Complete blocks */
      lastBlockEnd = (actLengthType)((length - diff) / actHASH_BLOCK_SIZE_SHA);
      lastBlockEnd = (lastBlockEnd*actHASH_BLOCK_SIZE_SHA) + diff;

      for (bcount = diff; bcount < lastBlockEnd; bcount += actHASH_BLOCK_SIZE_SHA)
      {
        actSHATransform(&dataIN[bcount], H, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
      }

      /* Copy left overs into buffer */
      info->buffer_used = (actLengthType)(length - lastBlockEnd); /* SBSW_VSECPRIM_WRITE_POINTER */
      actMemCpyByteArray(buffer, &dataIN[lastBlockEnd], (actLengthType)(info->buffer_used)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

      actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
    }
  }

  return retVal;
}

/***********************************************************************************************************************
 *  actSHAFinalize
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHAFinalize(
  VSECPRIM_P2VAR_PARA(actSHASTRUCT) info, VSECPRIM_P2VAR_PARA(actU8) hash,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2VAR_PARA(actU8) tempBuffer = (VSECPRIM_P2VAR_PARA(actU8))info->buffer;
  VSECPRIM_P2VAR_PARA(actU32) digest = (VSECPRIM_P2VAR_PARA(actU32))info->H;
  actU32 low_bitcount = info->low_count << 3;
  actU32 hi_bitcount = (info->hi_count << 3) | (info->low_count >> 29);
  actLengthType len = info->low_count & 0x3Fu;
  actLengthType pad_len = (actLengthType)(actHASH_BLOCK_SIZE_SHA - (2u * actSHA_WORD_SIZE));

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* pad byte '0x80' */
  tempBuffer[len] = 0x80u; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
  len = len + 1u;

  /* pad zeros */
  actMemset(&tempBuffer[len], 0u, (actLengthType)(actHASH_BLOCK_SIZE_SHA - len)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  if (len > pad_len)
  {
    actSHATransform((VSECPRIM_P2CONST_PARA(actU8))tempBuffer, digest, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    actMemset(tempBuffer, 0u, (actLengthType)pad_len); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  /* pad input bit count to last 8 bytes in BIG endian format */
  actU32toBE (&tempBuffer[pad_len],                   hi_bitcount); /* SBSW_VSECPRIM_CALL_FUNCTION */
  actU32toBE (&tempBuffer[pad_len+actSHA_WORD_SIZE], low_bitcount); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* transform last block */
  actSHATransform((VSECPRIM_P2CONST_PARA(actU8))tempBuffer, digest, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* store digest in BIG endian byte order */
  actCpyU32toBE (hash, digest, actHASH_SIZE_SHA /actSHA_WORD_SIZE); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* wipe buffer */
  actMemset(tempBuffer, 0u, actHASH_BLOCK_SIZE_SHA); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  return actOK;
}

/**********************************************************************************************************************
 *  actSHAGetDigest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actSHAGetDigest(
  VSECPRIM_P2CONST_PARA(actSHASTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* Store digest in BIG endian byte order */
  actCpyU32toBE(hash, info->H, actHASH_SIZE_SHA /actSHA_WORD_SIZE); /* SBSW_VSECPRIM_CALL_FUNCTION */

  actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
}


# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTSHA_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actSHA.c
 *********************************************************************************************************************/
