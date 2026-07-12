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
/*!        \file  actBNSub.c
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

#define ACTBNSUB_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actBigNum.h"
#include "actUtilities.h"

#if (VSECPRIM_ACTBNSUB_ENABLED == STD_ON)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actBNSub()
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
 */
VSECPRIM_FUNC(void) actBNSub(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) c,
  const actLengthType length)
{
  actLengthType i;
  actBNDDIGIT tmp = actBN_BASE;

  for (i = 0; i < length; ++i)
  {
    /* tmp = a[i] - b[i] - carry */
     tmp = (actBNDDIGIT)(actBN_BASE | a[i]) - b[i] - (1u - (actBNDDIGIT)actBNHiWord(tmp));
    c[i] = actBNLoWord(tmp); /* SBSW_VSECPRIM_WRITE_ARRAY */
  }
}

/**********************************************************************************************************************
 *  actBNSubDigit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actBNSubDigit(
  VSECPRIM_P2CONST_PARA(actBNDIGIT)  a,
  const actBNDIGIT b,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) c,
  const actBNLENGTH length)
{
  actBNLENGTH  i;
  actBNDDIGIT  tmp;
  actBNDDIGIT  carry = b;

  for (i = 0u; ((carry != 0u) && (i < length)); ++i)
  {
    tmp = (actBNDDIGIT)a[i] - carry;
    c[i] = actBNLoWord(tmp); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */

    if (actBNHiWord(tmp) != 0u)
    {
      carry = 1u;
    }
    else
    {
      carry = 0u;
    }
  }
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTBNSUB_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actBNSub.c
 *********************************************************************************************************************/
