/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!      \file  actDRBGCommon.c
 *      \brief  Functions required by CTR DRBG and Hash DRBG
 *
 *      \details  Implementation for RANDOM Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define ACT_DRBG_COMMON_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actDRBGCommon.h"

#if ((VSECPRIM_ACTCTRDRBG_ENABLED == STD_ON) || (VSECPRIM_ACTHASHDRBG_ENABLED == STD_ON))

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actDrbgIncrementBlock()
 **********************************************************************************************************************/
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
VSECPRIM_FUNC(void) actDrbgIncrementBlock(
  VSECPRIM_P2VAR_PARA(actU8) block,
  actLengthType blockLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 i = blockLength;
  actU8 result;

  /* ----- Implementation ------------------------------------------------- */
  while (i != 0u)
  {
    i--;
    /* Increment block element and store result */
    ++block[i];  /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
    result = block[i];

    /* Check for overflow */
    if (result != 0u)
    {
      /* No overflow happens - stop incrementing */
      break;
    }
  }
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ((VSECPRIM_ACTCTRDRBG_ENABLED == STD_ON) || (VSECPRIM_ACTHASHDRBG_ENABLED == STD_ON)) */

/**********************************************************************************************************************
 *  END OF FILE: actDRBGCommon.c
 *********************************************************************************************************************/
