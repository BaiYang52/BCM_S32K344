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
/*!        \file  actBNFieldInversion.c
 *        \brief  A basic (unsigned) integer and module arithmetic used for elliptic curve point arithmetic.
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

#define ACTBNFIELDINVERSION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actBigNum.h"
#if (VSECPRIM_ACTBNEUCLID_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
# include "actBigNumGCD.h"
#endif
#include "actUtilities.h"

#if (VSECPRIM_ACTBNFIELDINVERSION_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  actBNFieldInversion()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actBNFieldInversion(
   VSECPRIM_P2CONST_PARA(actBNDIGIT) gR, VSECPRIM_P2VAR_PARA(actBNDIGIT) g_invR,
   VSECPRIM_P2CONST_PARA(actBNRING) Field, VSECPRIM_P2VAR_PARA(actBNDIGIT) t[],
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
# if (VSECPRIM_ACTBNEUCLID_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

  VSECPRIM_P2VAR_PARA(actBNDIGIT) tmp = t[4];

  /* tmp = g */
  actBNSetZero(t[3], Field->m_length);
  (t[3])[0] = (actBNDIGIT) 1;   /* t[3] = 1 */
  actBNMontMul(gR, t[3], tmp, Field, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
  /* g_invR = g^-1 */
  actBNGCDix(tmp, g_invR, Field, t);
  /* g_invR = g^-1 * R */
  actBNMontMulCopy(g_invR, Field->RR, tmp, Field, watchdog);

# else

  actLengthType p_length = Field->m_length;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) e;
#  if (actBN_MOD_EXP_WINDOW_SIZE == 0) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  e = t[1 << (Field->window_size - 1)];
#  else
  e = t[1u << (actBN_MOD_EXP_WINDOW_SIZE - 1u)];
#  endif

  /* Compute Field->m - 2, store the result in e */
  actBNSetZero(e, p_length); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */
  e[0] = 2; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
  actBNSub(Field->m, e, e, p_length); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */

  /* g_invR = gR^e = gR^-1 */
  actBNModExp(gR, e, g_invR, Field, t, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
# endif /* (VSECPRIM_ACTBNEUCLID_ENABLED == STD_ON) */
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTBNFIELDINVERSION_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actBNFieldInversion.c
 *********************************************************************************************************************/
