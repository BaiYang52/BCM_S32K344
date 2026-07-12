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
/*!        \file  actIAEAD7539.c
 *        \brief  AEAD7539 implementation.
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

#define ACTIAEAD7539_SOURCE

/****************************************************************************
 **
 ** AEAD_ChaCha20_Poly1305 (RFC 7539) core and interface implementation.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   actAEAD7539Init
 **   actAEAD7539Update
 **   actAEAD7539SwitchToData
 **   actAEAD7539Finalze
 **
 ***************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actIAEAD7539.h"
#include "actUtilities.h"
#include "actWatchdog.h"

#if (VSECPRIM_ACTAEAD7539_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/* PRQA S 2889 EOF */ /* MD_VSECPRIM_QM_ONLY_RETURN_PATHS */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define SIZEOF_POLY1305_KEY                                          (32)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actAEAD7539Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actAEAD7539Init(VSECPRIM_P2VAR_PARA(actAEAD7539Context) ctx, VSECPRIM_P2CONST_PARA(actU8) key, VSECPRIM_P2CONST_PARA(actU8) nonce, const actU32 mode, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  ctx->mode = ((DIRECTION_ENCRYPT == mode) ? DIRECTION_ENCRYPT : DIRECTION_DECRYPT);
  /* Updating ctx->mode |= PHASE_AAD not necessary, since PHASE_AAD = 0u. */

  actChaCha20Init(&(ctx->cc20), key, nonce, 0u, watchdog);
  actChaCha20NextBlock(&(ctx->cc20), watchdog);
  actPoly1305Init(&(ctx->p130), (VSECPRIM_P2CONST_PARA(actU8))(ctx->cc20.cipher));
  actChaCha20EatBlock(&(ctx->cc20));

  ctx->aadLenHI = 0u;  /* PRQA S 2983 */ /* MD_VSECPRIM_REDUNDANT_ASSIGNMENT */
  ctx->aadLenLO = 0u;  /* PRQA S 2983 */ /* MD_VSECPRIM_REDUNDANT_ASSIGNMENT */
  ctx->dataLenHI = 0u;  /* PRQA S 2983 */ /* MD_VSECPRIM_REDUNDANT_ASSIGNMENT */
  ctx->dataLenLO = 0u;   /* PRQA S 2983 */ /* MD_VSECPRIM_REDUNDANT_ASSIGNMENT */

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
}

/**********************************************************************************************************************
 *  actAEAD7539Update()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actAEAD7539Update(VSECPRIM_P2VAR_PARA(actAEAD7539Context) ctx, VSECPRIM_P2CONST_PARA(actU8) in, const actLengthType iLen, VSECPRIM_P2VAR_PARA(actU8) out, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal = actOK;
  actU32 lo, hi;

  switch (ctx->mode)
  {
  case PHASE_AAD | DIRECTION_ENCRYPT:
  case PHASE_AAD | DIRECTION_DECRYPT:

    /* update & validate AAD counter: */
    /* max: 2^64 -1 bytes (RFC 7539) */
    /* -> FFFFFFFFFFFFFFFF */
    hi = ctx->aadLenHI;
    lo = ctx->aadLenLO + iLen;    /* increment AAD counter */
    if (lo < ctx->aadLenLO)      /* LSN overflow? */
    {
      hi += 1u;
      if (hi == 0u) /* MSN overflow (2^64 -1 bytes)? */
      {
        retVal = actEXCEPTION_LENGTH; /* overflow! */
      }
    }

    if (retVal == actOK)
    {
      ctx->aadLenHI = hi;
      ctx->aadLenLO = lo;

      /* update authenticator */
      actPoly1305Update(&(ctx->p130), in, iLen, watchdog);
    }

    break;


  case PHASE_DATA | DIRECTION_ENCRYPT:
  case PHASE_DATA | DIRECTION_DECRYPT:

    /* update & validate data counter: */
    /* max: 2^32 -1  blocks (RFC 7539) */
    /* -> (2^32 -1) *64 bytes */
    /* -> 3F.FFFF.FFC0 */
    hi = ctx->dataLenHI;
    lo = ctx->dataLenLO + iLen;  /* increment data counter */
    if (lo < ctx->dataLenLO)     /* overflow? */
    {
      hi++;
    }

    if ((hi > 0x0000003FUL)
      || ((hi == 0x0000003FUL) && (lo > 0xFFFFFFC0UL)))
    {
      retVal = actEXCEPTION_LENGTH; /* overflow! */
    }

    if (retVal == actOK)
    {
      ctx->dataLenHI = hi;
      ctx->dataLenLO = lo;

      /* update authenticator */
      if (DIRECTION_ENCRYPT == (ctx->mode & DIRECTION))
      {
        /* encrypt / decrypt "in" -> "out" */
        actChaCha20Cipher(&(ctx->cc20), in, iLen, out, watchdog);

        actPoly1305Update(&(ctx->p130), out, iLen, watchdog); /* ENcrypt:  "out" is cipher */
      }
      else
      {
        actPoly1305Update(&(ctx->p130), in, iLen, watchdog); /* DEcrypt:  "in"  is cipher */

        /* encrypt / decrypt "in" -> "out" */
        actChaCha20Cipher(&(ctx->cc20), in, iLen, out, watchdog);
      }
    }

    break;


  default:
    retVal = actEXCEPTION_MODE;
    break;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  actAEAD7539SwitchToData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actAEAD7539SwitchToData(VSECPRIM_P2VAR_PARA(actAEAD7539Context) ctx)
{
  actPoly1305HandleRemains(&(ctx->p130), 1u, NULL_PTR);

  ctx->mode &= DIRECTION;
  ctx->mode |= PHASE_DATA;
}

/**********************************************************************************************************************
 *  actAEAD7539Finalize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actAEAD7539Finalize(VSECPRIM_P2VAR_PARA(actAEAD7539Context) ctx, VSECPRIM_P2VAR_PARA(actU8) tag, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actPoly1305HandleRemains(&(ctx->p130), 1, NULL_PTR);

  actU32toLE(&(ctx->data[0]), ctx->aadLenLO);
  actU32toLE(&(ctx->data[4]), ctx->aadLenHI);
  actU32toLE(&(ctx->data[8]), ctx->dataLenLO);
  actU32toLE(&(ctx->data[12]), ctx->dataLenHI);
  actPoly1305Update(&(ctx->p130), ctx->data, sizeof(ctx->data), watchdog);
  actPoly1305Finalize(&(ctx->p130), tag, watchdog);
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTAEAD7539_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actIAES.c
 *********************************************************************************************************************/
