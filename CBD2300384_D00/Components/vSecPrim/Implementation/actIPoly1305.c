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
/*!        \file  actIPoly1305.c
 *        \brief  Poly1305 implementation.
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

/****************************************************************************
 **
 ** Poly1305 programming interface
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   actPoly1305Init
 **   actPoly1305Update
 **   actPoly1305Finalize
 **
 ***************************************************************************/

#define ACTIPOLY1305_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actIPoly1305.h"
#include "actBigNum.h"
#include "actUtilities.h"
#include "actWatchdog.h"

#if (VSECPRIM_ACTPOLY1305_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/* PRQA S 2889 EOF */ /* MD_VSECPRIM_QM_ONLY_RETURN_PATHS */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define BLOCK                                                        16u

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/* use cv core */
# include "actPoly1305core.h"

# define poly_initCore(ws,  key)                                      actPoly1305_init   ((ws), (key))
# define poly_calcBlock(ws, data, pad, wd)                            actPoly1305_block  ((ws), (data), (pad), (wd))
# define poly_finalize(ws,  tag)                                      actPoly1305_finish ((ws), (tag))

# define VSECPRIM_START_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) padding[BLOCK] = { /* PRQA S 3218 */ /* MD_VSECPRIM_8.9 */
  1u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u
};

# define VSECPRIM_STOP_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actPoly1305Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actPoly1305Init(VSECPRIM_P2VAR_PARA(actPoly1305Context) ctx, VSECPRIM_P2CONST_PARA(actU8) key)
{
  poly_initCore(ctx, key);

  ctx->rLen = 0; /* NO remains */
}

/**********************************************************************************************************************
 *  actPoly1305Update()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actPoly1305Update(VSECPRIM_P2VAR_PARA(actPoly1305Context) ctx, VSECPRIM_P2CONST_PARA(actU8) data, const actLengthType dLen, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actLengthType len = dLen;
  actLengthType pos = 0u;

  if ((BLOCK - ctx->rLen) > len) /* block incomplete? */
  {
    actMemCpyByteArray(&(ctx->remains[ctx->rLen]), data, len); /* just store the data */  /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
    ctx->rLen += len;

    return; /* can't do anything else */
  }

  if (0u != ctx->rLen) /* any remains from last call? */
  {
    actLengthType rLen = (BLOCK - ctx->rLen);

    actMemCpyByteArray(&(ctx->remains[ctx->rLen]), data, rLen); /* fill up block */  /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
    poly_calcBlock(ctx, ctx->remains, 1, watchdog); /* include block in MAC */
    actL2trigger(watchdog); /* AM's Poly1305 doesn't handle watchdog! */  /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

    len -= rLen;
    pos = rLen;
  }

  while (BLOCK <= len) /* any complete blocks? */
  {
    poly_calcBlock(ctx, &(data[pos]), 1, watchdog); /* include block in MAC */
    actL2trigger(watchdog); /* AM's Poly1305 doesn't handle watchdog! */  /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

    len -= BLOCK;
    pos += BLOCK;
  }

  if (0u != len)
  {
    actMemCpyByteArray(ctx->remains, &(data[pos]), len); /* store the remains */  /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
  }
  ctx->rLen = len; /* note the remains */   /* PRQA S 2983 */ /* MD_VSECPRIM_REDUNDANT_ASSIGNMENT */

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
}

/**********************************************************************************************************************
 *  actPoly1305HandleRemains()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actPoly1305HandleRemains(VSECPRIM_P2VAR_PARA(actPoly1305Context) ctx, const actU8 fillUp, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actU8 pad = ((fillUp != 0u) ? 1u : 0u); /* normalize to 1|0 */

  if (0u != ctx->rLen) /* any remains from last call? */
  {
    VSECPRIM_P2VAR_PARA(actU8) t = &(ctx->remains[ctx->rLen]); /* pad remains to full block */
    VSECPRIM_P2ROMCONST_PARA(actU8) p = &(padding[pad]); /* pad with 1, 0, ... or 0, 0, ... */

    actMemCpyByteArray(t, p, (BLOCK - ctx->rLen)); /* pad remains to full block */  /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
    poly_calcBlock(ctx, ctx->remains, pad, watchdog); /* include block in MAC */
    ctx->rLen = 0; /* no remains! */   /* PRQA S 2983 */ /* MD_VSECPRIM_REDUNDANT_ASSIGNMENT */

    actL2trigger(watchdog); /* AM's Poly1305 doesn't handle watchdog! */  /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }
}

/**********************************************************************************************************************
 *  actPoly1305Finalize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actPoly1305Finalize(VSECPRIM_P2VAR_PARA(actPoly1305Context) ctx, VSECPRIM_P2VAR_PARA(actU8) tag, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actPoly1305HandleRemains(ctx, 0, watchdog); /* include remains in MAC */
  poly_finalize(ctx, tag);

  actMemset(ctx->opaque, 0, sizeof(ctx->opaque)); /* clear memory */  /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTPOLY1305_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actIPoly1305.c
 *********************************************************************************************************************/
