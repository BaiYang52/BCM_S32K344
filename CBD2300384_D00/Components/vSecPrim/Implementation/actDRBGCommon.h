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
/*!      \file  actDRBGCommon.h
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

#ifndef ACTDRBGCOMMON_H
# define ACTDRBGCOMMON_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actITypes.h"

# if ((VSECPRIM_ACTCTRDRBG_ENABLED == STD_ON) || (VSECPRIM_ACTHASHDRBG_ENABLED == STD_ON))

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#  define VSECPRIM_START_SEC_CODE
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actDrbgIncrementBlock()
 **********************************************************************************************************************/
/*! \brief          Increments the value of block.
 *  \details        Helper function to increment the value of block V: V = (V + 1) mod 2^block_len using
 *                  big endian byte order.
 *  \param[in,out]  block        (in)  Holds the variable to be increased
 *                               (out) increased variable
 *  \param[in]      blockLength  length of the variable to be increased in bytes
 *  \pre            block has to be a valid pointer.
 *                  The buffer referenced by block has to provide at least blockLength bytes.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actDrbgIncrementBlock(
  VSECPRIM_P2VAR_PARA(actU8) block,
  actLengthType blockLength);

#  define VSECPRIM_STOP_SEC_CODE
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* ((VSECPRIM_ACTCTRDRBG_ENABLED == STD_ON) || (VSECPRIM_ACTHASHDRBG_ENABLED == STD_ON)) */

#endif /* ACTDRBGCOMMON_H */
/**********************************************************************************************************************
 *  END OF FILE: actDRBGCommon.h
 *********************************************************************************************************************/
