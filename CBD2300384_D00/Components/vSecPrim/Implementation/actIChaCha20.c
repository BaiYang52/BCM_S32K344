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
/*!        \file  actIChaCha20.c
 *        \brief  ChaCha20 implementation.
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

#define ACTIChaCha20_SOURCE

/****************************************************************************
 **
 ** ChaCha20 core and interface implementation.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   actChaCha20Init
 **   actChaCha20Cipher
 **
 ***************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actIChaCha20.h"
#include "actUtilities.h"
#include "actWatchdog.h"

#if (VSECPRIM_ACTCHACHA20_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

 /* PRQA S 0850 EOF */ /* MD_MSR_MacroArgumentEmpty */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define CHACHA_0                                                     0x61707865
# define CHACHA_1                                                     0x3320646e
# define CHACHA_2                                                     0x79622d32
# define CHACHA_3                                                     0x6b206574

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** Local functions and macros
 ***************************************************************************/

 /**********************************************************************************************************************
 *  dotheChaCha()
 *********************************************************************************************************************/
/*! \brief          ChaCha step of ChaCha20 algorithm.
 *  \details        -
 *  \param[in,out]  ctx          (in) pointer to AEAD context
 *                               (out) initialized context
 *  \param[in]      watchdog     pointer to the watchdog function
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL VSECPRIM_FUNC(void) dotheChaCha(VSECPRIM_P2VAR_PARA( actChaCha20Context ) ctx, VSECPRIM_P2FUNC( VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actChaCha20Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actChaCha20Init(VSECPRIM_P2VAR_PARA(actChaCha20Context) ctx, VSECPRIM_P2CONST_PARA(actU8) key, VSECPRIM_P2CONST_PARA(actU8) nonce, const actU32 bCount, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* initialize iState */
  ctx->iState[0] = CHACHA_0; /* constant values */
  ctx->iState[1] = CHACHA_1;
  ctx->iState[2] = CHACHA_2;
  ctx->iState[3] = CHACHA_3;
  actCpyLEtoU32(&(ctx->iState[4]), key, 32); /* import key */
  ctx->iState[12] = bCount; /* set block counter */
  actCpyLEtoU32(&(ctx->iState[13]), nonce, 12); /* import nonce */

  ctx->c = 64; /* cipher block depleted */

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
}

/**********************************************************************************************************************
 *  actChaCha20Cipher()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actChaCha20Cipher(VSECPRIM_P2VAR_PARA(actChaCha20Context) ctx, VSECPRIM_P2CONST_PARA(actU8) in, const actLengthType iLen, VSECPRIM_P2VAR_PARA(actU8) out, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actLengthType i;

  for (i = 0u; i < iLen; i++)
  {
    if (ctx->c >= 64u)
    { /* any byte(s) left? */
      dotheChaCha(ctx, watchdog); /* no! */
      ctx->c = 0; /* cipher filled up */
    }
    out[i] = in[i] ^ ctx->cipher[ctx->c]; /* encode one byte */
    ctx->c++;
  }

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
}

/**********************************************************************************************************************
 *  actChaCha20NextBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actChaCha20NextBlock(VSECPRIM_P2VAR_PARA(actChaCha20Context) ctx, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  dotheChaCha(ctx, watchdog); /* generate cipher block */
}

/**********************************************************************************************************************
 *  actChaCha20EatBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actChaCha20EatBlock(VSECPRIM_P2VAR_PARA(actChaCha20Context) ctx)
{
  ctx->c = 64; /* cipher block depleted */
}

/****************************************************************************
 ** Local functions
 ***************************************************************************/

/* ChaCha20 quarter round */
# if actChaCha20_SPEED_UP >= 1
#  define doQRound(state, a, b, c, d)                                 \
                                                                      state [a] += state [b]; state [d] ^= state [a]; state [d] = actRotl (state [d], 16); \
                                                                      state [c] += state [d]; state [b] ^= state [c]; state [b] = actRotl (state [b], 12); \
                                                                      state [a] += state [b]; state [d] ^= state [a]; state [d] = actRotl (state [d],  8); \
                                                                      state [c] += state [d]; state [b] ^= state [c]; state [b] = actRotl (state [b],  7);
# else

/**********************************************************************************************************************
 *  doQRound()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL VSECPRIM_FUNC(void) doQRound(VSECPRIM_P2VAR_PARA(actU32) state, actLengthType a, actLengthType b, actLengthType c, actLengthType d)
{
  state[a] += state[b]; state[d] ^= state[a]; state[d] = actRotl(state[d], 16);
  state[c] += state[d]; state[b] ^= state[c]; state[b] = actRotl(state[b], 12);
  state[a] += state[b]; state[d] ^= state[a]; state[d] = actRotl(state[d], 8);
  state[c] += state[d]; state[b] ^= state[c]; state[b] = actRotl(state[b], 7);
}
# endif

/**********************************************************************************************************************
 *  dotheChaCha()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* ChaCha20 block function */
VSECPRIM_LOCAL VSECPRIM_FUNC(void) dotheChaCha(VSECPRIM_P2VAR_PARA(actChaCha20Context) ctx, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actU32  cState[16];

  actLengthType i;

  for (i = 0u; i <= 15u; i++) /* copy iState->cState */
  {
    cState[i] = ctx->iState[i];
  }

# if actChaCha20_SPEED_UP >= 2
  for (i = 0u; i < 5u; i++) { /* perform 20 (5 * 4) rounds */
    doQRound (cState,  0,  4,  8, 12); /* column round (1) */
    doQRound (cState,  1,  5,  9, 13);
    doQRound (cState,  2,  6, 10, 14);
    doQRound (cState,  3,  7, 11, 15);
    doQRound (cState,  0,  5, 10, 15); /* diagonal round (2) */
    doQRound (cState,  1,  6, 11, 12);
    doQRound (cState,  2,  7,  8, 13);
    doQRound (cState,  3,  4,  9, 14);
    doQRound (cState,  0,  4,  8, 12); /* column round (3) */
    doQRound (cState,  1,  5,  9, 13);
    doQRound (cState,  2,  6, 10, 14);
    doQRound (cState,  3,  7, 11, 15);
    doQRound (cState,  0,  5, 10, 15); /* diagonal round (4) */
    doQRound (cState,  1,  6, 11, 12);
    doQRound (cState,  2,  7,  8, 13);
    doQRound (cState,  3,  4,  9, 14);

    actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }
# else
  for (i = 0u; i < 10u; i++) /* perform 20 (10 * 2) rounds */
  {
    doQRound(cState, 0, 4, 8, 12); /* column round (1) */
    doQRound(cState, 1, 5, 9, 13);
    doQRound(cState, 2, 6, 10, 14);
    doQRound(cState, 3, 7, 11, 15);
    doQRound(cState, 0, 5, 10, 15); /* diagonal round (2) */
    doQRound(cState, 1, 6, 11, 12);
    doQRound(cState, 2, 7, 8, 13);
    doQRound(cState, 3, 4, 9, 14);

    actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }
# endif

  for (i = 0u; i <= 15u; i++) /* add iState to cState (mod 2^32) */
  {
    cState[i] += ctx->iState[i];
  }
  actCpyU32toLE(ctx->cipher, cState, 16);        /* export XOR cipher block */

  ctx->iState[12]++; /* increment block counter (for next block) */

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTCHACHA20_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actIChaCha20.c
 *********************************************************************************************************************/
