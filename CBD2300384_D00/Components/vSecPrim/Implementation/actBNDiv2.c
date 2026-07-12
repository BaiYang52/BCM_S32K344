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
/*!        \file  actBNDiv2.c
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
#define ACTBNDIV2_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actBigNum.h"
#include "actUtilities.h"

#if (VSECPRIM_ACTBNDIV2_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actBNDiv2()
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
 */
VSECPRIM_FUNC(void) actBNDiv2(VSECPRIM_P2VAR_PARA(actBNDIGIT) a, actU8 first_carry, actLengthType length)
{
  actLengthType tmpLength;
  actU8 carry1 = first_carry, carry2;
  actBNDIGIT tmp;
  uint32 i;

  /* shift a to the right by 1 bit */
  /* start with the last big num block, mind the little endian order of the array entries */
  for (i = 0u; i < length; i++)
  {
    tmpLength = (length - i) - 1u;
    tmp = a[tmpLength];
    carry2 = (actU8) (tmp & 1u);
    tmp >>= 1;
    tmp |= (((actBNDIGIT) carry1) << (actBN_BITS_PER_DIGIT - 1u));
    carry1 = carry2;
    a[tmpLength] = tmp; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
  }
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTBNDIV2_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actBNDiv2.c
 *********************************************************************************************************************/
