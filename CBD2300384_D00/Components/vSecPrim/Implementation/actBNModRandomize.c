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
/*!        \file  actBNModRandomize.c
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

#define ACTBNMODRANDOMIZE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actBigNum.h"
#include "actUtilities.h"
#include "actExternRandom.h"

#if (VSECPRIM_ACTBNMODRANDOMIZE_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actBNModRandomize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actBNModRandomize(VSECPRIM_P2VAR_PARA(actBNDIGIT) x,
  VSECPRIM_P2CONST_PARA(actBNRING) Ring,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) t[], /* PRQA S 3677 */ /* MD_VSECPRIM_CONST_POINTER */
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void)) /* PRQA S 3677 */ /* MD_VSECPRIM_CONST_POINTER */
{
  actLengthType m_length = Ring->m_length;
  actRETURNCODE retVal = actOK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  do
  {
    retVal = actGetExternRandom(((VSECPRIM_P2VAR_PARA(actU8)) x), m_length * actBN_BYTES_PER_DIGIT); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

    if (retVal == actOK)
    {
      retVal = actGetExternRandom(((VSECPRIM_P2VAR_PARA(actU8)) t[0]), m_length * actBN_BYTES_PER_DIGIT); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

      if (retVal == actOK)
      {
        actBNMontMulCopy(x, t[0], t[1], Ring, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        actBNMontMulCopy(x, Ring->RR, t[0], Ring, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }
    }
  }
  while((actBNIsZero(x, m_length) == TRUE) && (retVal == actOK));  /* while x==0 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTBNMODRANDOMIZE_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actBNModRandomize.c
 *********************************************************************************************************************/
