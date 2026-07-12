/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2017 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  actPoly1305core.c
 *        \brief  Poly1305core implementation.
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

#define ACTPOLY1305CORE_SOURCE

/****************************************************************************
 **
 ** Core Poly1305 implementation
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   actPoly1305_init
 **   actPoly1305_block
 **   actPoly1305_finish
 **
 ***************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actIPoly1305.h"
#include "actPoly1305core.h"
#include "actBigNum.h"
#include "actUtilities.h"
#include "actWatchdog.h"

#if (VSECPRIM_ACTPOLY1305_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/* PRQA S 0310, 3305 EOF */ /* MD_VSECPRIM_QM_ONLY_CASTING_POINTER_TYPE */

/****************************************************************************
 ** Types and constants
 ***************************************************************************/

/* expand a 32-bit value into a set of actBNDIGITs depending upon the actBNDIGIT size */
/* LITTLE endian ordering! */
# if (8 == actBN_BITS_PER_DIGIT)
#  define DIGITs(value)                                               ((actBNDIGIT) ((value) >>  0)), \
                                                                      ((actBNDIGIT) ((value) >>  8)), \
                                                                      ((actBNDIGIT) ((value) >> 16)), \
                                                                      ((actBNDIGIT) ((value) >> 24))
#  define actLEtoBigNum                                               actMemCpyByteArray
#  define actBigNumtoLE                                               actMemCpyByteArray
# elif (16 == actBN_BITS_PER_DIGIT)
#  define DIGITs(value)                                               ((actBNDIGIT) ((value) >>  0)), \
                                                                      ((actBNDIGIT) ((value) >> 16))
#  define actLEtoBigNum                                               actCpyLEtoU16
#  define actBigNumtoLE                                               actCpyU16toLE
# elif (32 == actBN_BITS_PER_DIGIT)
#  define DIGITs(value)                                               (value)
#  define actLEtoBigNum                                               actCpyLEtoU32
#  define actBigNumtoLE                                               actCpyU32toLE
# endif

/****************************************************************************
 ** Local data, functions and macros
 ***************************************************************************/
# define VSECPRIM_START_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM( VSECPRIM_LOCAL, actBNDIGIT ) clamp [BNDIGITS_128] = { /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */
  DIGITs (0x0FFFFFFFUL), DIGITs (0x0FFFFFFCUL), DIGITs (0x0FFFFFFCUL), DIGITs (0x0FFFFFFCUL)
};

VSECPRIM_ROM( VSECPRIM_LOCAL, actBNDIGIT ) p [BNDIGITS_130] = { /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */
  DIGITs (0xFFFFFFFBUL), DIGITs (0xFFFFFFFFUL), DIGITs (0xFFFFFFFFUL), DIGITs (0xFFFFFFFFUL), 0x03    /* last value is but a single DIGIT!! */
};

# define VSECPRIM_STOP_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actPoly1305_init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actPoly1305_init(VSECPRIM_P2VAR_PARA(actPoly1305Context) context, VSECPRIM_P2CONST_PARA(actU8) key)
{
  VSECPRIM_P2VAR_PARA(poly1305context) ctx = (poly1305context *)context->opaque;

  actLengthType i;

  actLEtoBigNum(ctx->r, &(key[0]), BNBYTES_128);  /* convert key (r | s) into actBNDIGIT representation */
  actLEtoBigNum(ctx->s, &(key[BNBYTES_128]), BNBYTES_128);

  for (i = 0; i < BNDIGITS_128; i++) /* clamp r */
  {
    ctx->r[i] &= clamp[i];
  }

  for (i = 0; i < BNDIGITS_130; i++) /* load p */
  {
    ctx->p[i] = p[i];
  }

  actMemset((actU8 *)ctx->a, 0, sizeof(ctx->a)); /* clear accumulator */  /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
}

VSECPRIM_FUNC(void) actPoly1305_block(VSECPRIM_P2VAR_PARA(actPoly1305Context) context, VSECPRIM_P2CONST_PARA(actU8) data, const actU8 pad, VSECPRIM_P2FUNC( VSECPRIM_NONE, void, watchdog)(void)) {
  VSECPRIM_P2VAR_PARA(poly1305context) ctx = (poly1305context *)context->opaque;

  actLEtoBigNum(ctx->d, data, BNBYTES_128);                  /* convert data into actBNDIGIT representation */
  ctx->d[BNDIGITS_128] = pad; /* pad block */

  (void)actBNAdd(ctx->a, ctx->d, ctx->a, BNDIGITS_130); /* accu += block */

  actBNMult(ctx->a, BNDIGITS_130, ctx->r, BNDIGITS_128, ctx->tmp, watchdog);

  (void)actBNReduce(ctx->tmp, BNDIGITS_130 + BNDIGITS_128, ctx->p, BNDIGITS_130, ctx->a, watchdog);
}

VSECPRIM_FUNC(void) actPoly1305_finish(VSECPRIM_P2VAR_PARA(actPoly1305Context) context, VSECPRIM_P2VAR_PARA(actU8) tag) {
  VSECPRIM_P2VAR_PARA(poly1305context) ctx = (poly1305context *)context->opaque;

  (void)actBNAdd(ctx->a, ctx->s, ctx->a, BNDIGITS_128); /* accu += s */

  actBigNumtoLE(tag, ctx->a, BNDIGITS_128);                  /* export result */
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTPOLY1305_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actPoly1305core.c
 *********************************************************************************************************************/
