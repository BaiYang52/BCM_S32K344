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
/*!        \file  actIRSAPublic.c
 *        \brief  This file provides functions to initialize and perform an exponentiation with respect to an RSA
 *                public key.
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
#define ACTIRSAPUBLIC_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actIRSA.h"
#include "actIRSAExp.h"
#include "actBigNum.h"
#include "actUtilities.h"

#if (VSECPRIM_ACTIRSAPUBLIC_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * actRSAInitPublicKeyOperation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actRSAInitPublicKeyOperation(
  VSECPRIM_P2CONST_PARA(actU8) modulus,
  actLengthType modulus_len,
  VSECPRIM_P2CONST_PARA(actU8) public_exponent,
  actLengthType public_exponent_len,
  VSECPRIM_P2VAR_PARA(actRSAPRIMSTRUCT) wkspBuf,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  return actRSAInitExponentiation(modulus, modulus_len, public_exponent, public_exponent_len, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
                                   wkspBuf, actRSA_PUBLIC_KEY_OPERATION, watchdog);
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 * actRSAPublicKeyOperation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actRSAPublicKeyOperation(
  VSECPRIM_P2CONST_PARA(actU8) message,
  actLengthType message_len,
  VSECPRIM_P2VAR_PARA(actU8) cipher,
  VSECPRIM_P2VAR_PARA(actLengthType) cipher_len,
  VSECPRIM_P2VAR_PARA(actRSAPRIMSTRUCT) wkspBuf,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  return actRSAExponentiation(message, message_len, cipher, cipher_len, wkspBuf, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 * actRSAPublicKeyGetBitLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actLengthType) actRSAPublicKeyGetBitLength(VSECPRIM_P2CONST_PARA(actBNRING) n_ring)
{
   return actBNGetBitLength(n_ring->m, n_ring->m_length); /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTIRSAPUBLIC_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: actIRSAPublic.c
 *********************************************************************************************************************/
