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
/*!        \file  actBNOddInvModBase.c
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

#define ACTBNODDINVMODBASE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actBigNum.h"

#if (VSECPRIM_ACTBNODDINVMODBASE_ENABLED == STD_ON)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

 /**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
 /**********************************************************************************************************************
 *  actBNOddInvModBase()
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
 */
VSECPRIM_FUNC(void) actBNOddInvModBase(VSECPRIM_P2VAR_PARA(actBNDIGIT) inverse, VSECPRIM_P2CONST_PARA(actBNDIGIT) value)
{
  actBNDDIGIT d = 1;
  actBNDDIGIT power = 1;
  actBNDIGIT tempVal = *value;
  actU8 i;

  *inverse = 0u; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
  for (i = 0u; i < actBN_BITS_PER_DIGIT; ++i)
  {
    /* power = 2^(i+1) */
    power <<= 1;
    if ((d % power) != 0u)
    {
      /* d += 2^i * tempVal */
      d = (actBNDDIGIT) (d + tempVal);
      /* inverse += 2^i */
      (*inverse) = (actBNDIGIT) ((*inverse) + (actBNDIGIT) (power >> 1)); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    }
    tempVal <<= 1;
  }
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTBNODDINVMODBASE_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actBNOddInvModBase.c
 *********************************************************************************************************************/
