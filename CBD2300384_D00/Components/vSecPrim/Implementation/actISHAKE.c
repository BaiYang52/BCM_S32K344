/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2020 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  actISHAKE.c
 *        \brief  SHAKE implementation.
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library cvActLib/ES
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

#define ACTISHAKE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actISHAKE.h"
#include "actITypes.h"
#include "actKECCAKc.h"

#if (VSECPRIM_ACTSHA3_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actSHAKEInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHAKEInit(
  VSECPRIM_P2VAR_PARA(actKECCAKcSTRUCT) info,
  const actU32 strength)
{
    return actKECCAKcInit (info, strength, actHASH_SHA3_PAD_1_SHAKE); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  actSHAKEUpdate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHAKEUpdate(
  VSECPRIM_P2VAR_PARA(actKECCAKcSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) data,
  const actLengthType dataLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
    return actKECCAKcAbsorb (info, data, dataLength, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  actSHAKEGetXOFDigest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHAKEGetXOFDigest(
  VSECPRIM_P2VAR_PARA(actKECCAKcSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) digest,
  const actLengthType digestSize,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
    return actKECCAKcSqueeze (info, digest, digestSize, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  actSHAKEFinalize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHAKEFinalize(
  VSECPRIM_P2VAR_PARA(actKECCAKcSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) digest,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
    return actKECCAKcSqueeze (info, digest, (2u * info->strength), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* VSECPRIM_ACTSHA3_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  END OF FILE: actISHA3.c
 *********************************************************************************************************************/
