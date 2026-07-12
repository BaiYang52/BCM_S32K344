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
/*!        \file  actBNModExp.c
 *        \brief  A basic (unsigned) integer and module arithmetic used for elliptic curve point arithmetic.
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

#define ACTBNMODEXP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "actBigNum.h"
#include "actUtilities.h"
#include "actWatchdog.h"

#if (VSECPRIM_ACTBNMODEXP_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actBNModExp()
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
VSECPRIM_FUNC(void) actBNModExp(
   VSECPRIM_P2CONST_PARA(actBNDIGIT) gR, VSECPRIM_P2CONST_PARA(actBNDIGIT) e,
   VSECPRIM_P2VAR_PARA(actBNDIGIT) AR, VSECPRIM_P2CONST_PARA(actBNRING) Ring,
   VSECPRIM_P2VAR_PARA(actBNDIGIT) t[], VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void)) /* PRQA S 3677 */ /* MD_VSECPRIM_CONST_POINTER */
{
# if (actBN_MOD_EXP_WINDOW_SIZE==1) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
  /* left-to-right square-and-multiply algorithm, bitwise operation */

  actLengthType m_length = Ring->m_length;
  actLengthType i = actBNGetBitLength(Ring->m, Ring->m_length) - 1u; /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  VSECPRIM_P2VAR_PARA(actBNDIGIT) tmp = t[0];

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* set i to the most significant bit of the exponent 'e' */
  while(actBNGetBit(e, i) == FALSE)
  {
    if ((i & 0x07u) == 0x00u)
    {
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
    }
    --i;
  }

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* AR = gR */
  actBNCopy(AR, gR, m_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* square-and-multiply loop, starting with the most significant bit */
  for (; i > 0u; i--)
  {
    actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

    /* tmp = AR^2 * R^(-1) */
    actBNMontMul(AR, AR, tmp, Ring, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */

    actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

    /* test bit i of e */
    if (actBNGetBit(e, (i - 1u)) == TRUE)
    {
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

      /* AR = tmp * gR * R^(-1) */
      actBNMontMul(tmp, gR, AR, Ring, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
    }
    else
    {
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

      /* AR = tmp */
      actBNCopy(AR, tmp, m_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    }
  }

# elif (actBN_MOD_EXP_WINDOW_SIZE == 2)

  /*
     left-to-right sliding window algorithm (fixed 2-bit window)
   */

  actLengthType i = actBNGetBitLength(Ring->m, Ring->m_length) - 1;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) tmp = t[0];
  actU8 e1_bit, e2_bit;

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* go to msb of 'e' */
  while(actBNGetBit(e, i) == 0)
  {
    if ((i & 0x07) == 0x00)
    {
      actL3trigger(watchdog);
    }
    --i;
  }

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  if (i & 1u)
  {
    /* AR = R (1-element in Montgomery representation) */
    actBNSetZero(AR, Ring->m_length);
    AR[0] = (actBNDIGIT) 1;

    actL3trigger(watchdog);

    actBNMontMulCopy(AR, Ring->RR, tmp, Ring, watchdog);
  }
  else
  {
    /* AR = gR */
    actBNCopy(AR, gR, Ring->m_length);
    --i;
  }

  /* tmp = g^2 * R */
  actBNMontMul(gR, gR, tmp, Ring, watchdog);

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* t[1] = g^3 * R */
  actBNMontMul(gR, tmp, t[1], Ring, watchdog);

  /* sliding window loop */
  while(i >= 0)
  {
    /* test bit i and (i-1) of e */
    e1_bit = (actU8) actBNGetBit(e, i);
    --i;
    e2_bit = (actU8) actBNGetBit(e, i);
    --i;

    actL3trigger(watchdog);

    /* tmp = AR^2 */
    actBNMontMul(AR, AR, tmp, Ring, watchdog);

    actL3trigger(watchdog);

    if (e1_bit)
    {
      if (e2_bit)
      {
        /* AR = AR^2 * gR^3 */
        actBNMontMul(tmp, tmp, AR, Ring, watchdog);

        actL3trigger(watchdog);

        actBNMontMulCopy(AR, t[1], tmp, Ring, watchdog);
      }
      else
      {
        /* AR = (AR * gR)^2 */
        actBNMontMul(tmp, gR, AR, Ring, watchdog);

        actL3trigger(watchdog);

        actBNMontMulCopy(AR, AR, tmp, Ring, watchdog);
      }
    }
    else
    {
      actBNMontMul(tmp, tmp, AR, Ring, watchdog);

      actL3trigger(watchdog);

      if (e2_bit)
      {
        /* AR = AR^2 * gR */
        actBNMontMulCopy(AR, gR, tmp, Ring, watchdog);
      }
    }
  }

# elif (actBN_MOD_EXP_WINDOW_SIZE == 3)

  /*
     left-to-right sliding window algorithm (fixed 3-bit window)
   */

  actLengthType i = actBNGetBitLength(Ring->m, Ring->m_length) - 1;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) tmp = t[0];
  actU8 e1_bit, e2_bit, e3_bit;

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* go to msb of 'e' */
  while(actBNGetBit(e, i) == 0)
  {
    if ((i & 0x07) == 0x00)
    {
      actL3trigger(watchdog);
    }
    --i;
  }

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* AR = R (1-element in Montgomery representation) */
  actBNSetZero(AR, Ring->m_length);
  AR[0] = (actBNDIGIT) 1;

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  actBNMontMulCopy(AR, Ring->RR, tmp, Ring, watchdog);

  /* tmp = g^2 * R */
  actBNMontMul(gR, gR, tmp, Ring, watchdog);

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* t[1] = g^3 * R */
  actBNMontMul(gR, tmp, t[1], Ring, watchdog);

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* t[2] = g^5 * R */
  actBNMontMul(t[1], tmp, t[2], Ring, watchdog);

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* t[3] = g^7 * R */
  actBNMontMul(t[2], tmp, t[3], Ring, watchdog);

  /* sliding window loop */
  while(i >= 2)
  {
    actL3trigger(watchdog);

    /* test bit i, (i-1) and (i-2) of e */
    e1_bit = (actU8) actBNGetBit(e, i);
    --i;
    e2_bit = (actU8) actBNGetBit(e, i);
    --i;
    e3_bit = (actU8) actBNGetBit(e, i);
    --i;

    actL3trigger(watchdog);

    /* tmp = AR^2 */
    actBNMontMul(AR, AR, tmp, Ring, watchdog);

    actL3trigger(watchdog);

    if (e1_bit)
    {
      if (e2_bit)
      {
        /* AR = tmp^2 */
        actBNMontMul(tmp, tmp, AR, Ring, watchdog);

        actL3trigger(watchdog);

        if (e3_bit)
        {
          /* AR = AR^2 * gR^7 */
          actBNMontMul(AR, AR, tmp, Ring, watchdog);

          actL3trigger(watchdog);

          actBNMontMul(tmp, t[3], AR, Ring, watchdog);
        }
        else
        {
          /* AR = (AR * gR^3)^2 */
          actBNMontMul(AR, t[1], tmp, Ring, watchdog);

          actL3trigger(watchdog);

          actBNMontMul(tmp, tmp, AR, Ring, watchdog);
        }
      }
      else
      {
        if (e3_bit)
        {
          /* AR = AR^4 * gR^5 */
          actBNMontMul(tmp, tmp, AR, Ring, watchdog);

          actL3trigger(watchdog);

          actBNMontMul(AR, AR, tmp, Ring, watchdog);

          actL3trigger(watchdog);

          actBNMontMul(tmp, t[2], AR, Ring, watchdog);
        }
        else
        {
          /* AR = (AR * gR)^4 */
          actBNMontMul(tmp, gR, AR, Ring, watchdog);

          actL3trigger(watchdog);

          actBNMontMul(AR, AR, tmp, Ring, watchdog);

          actL3trigger(watchdog);

          actBNMontMul(tmp, tmp, AR, Ring, watchdog);
        }
      }
    }
    else
    {
      actL3trigger(watchdog);

      /* AR = tmp^2 */
      actBNMontMul(tmp, tmp, AR, Ring, watchdog);

      actL3trigger(watchdog);

      if (e3_bit)
      {
        /* tmp = AR^2 */
        actBNMontMul(AR, AR, tmp, Ring, watchdog);

        actL3trigger(watchdog);

        if (e2_bit)
        {
          /* AR = tmp * gR^3 */
          actBNMontMul(tmp, t[1], AR, Ring, watchdog);
        }
        else
        {
          /* AR = tmp * gR */
          actBNMontMul(tmp, gR, AR, Ring, watchdog);
        }
      }
      else
      {
        if (e2_bit)
        {
          /* AR = (AR * gR)^2 */
          actBNMontMul(AR, gR, tmp, Ring, watchdog);

          actL3trigger(watchdog);

          actBNMontMul(tmp, tmp, AR, Ring, watchdog);
        }
        else
        {
          /* AR = AR^2 */
          actBNMontMulCopy(AR, AR, tmp, Ring, watchdog);
        }
      }
    }
  }

  for (; i > 0; --i)
  {
    actL3trigger(watchdog);

    /* AR = AR^2 */
    actBNMontMulCopy(AR, AR, tmp, Ring, watchdog);

    actL3trigger(watchdog);

    /* test bit i of e */
    if (actBNGetBit(e, i-1) == 1)
    {
      /* AR = AR * gR */
      actBNMontMulCopy(AR, gR, tmp, Ring, watchdog);
    }
  }

# else

  /*
     left-to-right sliding window algorithm ((Ring->window_size)-bit window)
   */

  actLengthType i = actBNGetBitLength(Ring->m, Ring->m_length) - 1;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) tmp = t[0];
  actLengthType j, l, index, len;
  actLengthType k = Ring->window_size - 1;
  actLengthType num_of_tmps = 1 << k; /* 2^k powers to precompute */

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* go to msb of 'e' */
  while(actBNGetBit(e, i) == 0)
  {
    --i;
  }

  actL3trigger(watchdog);

  /* AR = R (1-element in Montgomery representation) */
  actBNSetZero(AR, Ring->m_length);
  AR[0] = (actBNDIGIT) 1;
  actBNMontMulCopy(AR, Ring->RR, tmp, Ring, watchdog);

  actL3trigger(watchdog);

  /* tmp = g^2 * R */
  actBNMontMul(gR, gR, tmp, Ring, watchdog);
  /* t[1] = g^3 * R */
  actBNMontMul(gR, tmp, t[1], Ring, watchdog);

  actL3trigger(watchdog);

  for (j = 2; j < num_of_tmps; ++j)
  {
    /* 'odd powers' from (g^5 * R) to (g^(2^k-1) * R) */
    actBNMontMul(t[j - 1], tmp, t[j], Ring, watchdog);
  }
  /* set t[0] pointer to gR */
  t[0] = (VSECPRIM_P2VAR_PARA(actBNDIGIT)) gR;

  while(i >= 0)
  {
    actL3trigger(watchdog);

    if (actBNGetBit(e, i) == 0)
    {
      /* AR = AR^2 */
      actBNMontMulCopy(AR, AR, tmp, Ring, watchdog);
      --i;
    }
    else
    {
      /* calculate actuall window_length */
      l = i - k;
      l = actMax(l, 0);
      while(actBNGetBit(e, l) == 0)
      {
        ++l;
      }
      len = i - l;

      actL3trigger(watchdog);

      /* AR = AR^(2*window_length) */
      for (j = 0; j <= len; j++)
      {
        actBNMontMulCopy(AR, AR, tmp, Ring, watchdog);
      }

      actL3trigger(watchdog);

      /* calculate index for t[] */
      index = 1u << len;
      for (j = l + 1; j < i; j++)
      {
        if (actBNGetBit(e, j) == 1)
        {
          index += (1u << (j - l));
        }
      }

      actL3trigger(watchdog);

      /* AR *= t[index>>1] */
      actBNMontMulCopy(AR, t[index >> 1], tmp, Ring, watchdog);

      i = l - 1;
    }
  }

  /* reset t[0] pointer */
  t[0] = tmp;

# endif /* (actBN_MOD_EXP_WINDOW_SIZE==1) */

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTBNMODEXP_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actBNModExp.c
 *********************************************************************************************************************/
