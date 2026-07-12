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
/*!        \file  actX25519.c
 *        \brief  Core X25519 implementation.
 *
 *      \details This file is part of the embedded systems library cvActLib
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

#define ACTX25519_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actUtilities.h"
#include "actExternRandom.h"
#include "actConfig.h"
#include "actWatchdog.h"
#include "actX25519core.h"

#if (VSECPRIM_ACTX25519_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

# define x25519_initCore(ws)                                          actX25519_initCore (ws)
# define x25519_exchangeKeys(ws, wd)                                  actX25519_eval ((ws), (wd))

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actX25519Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actX25519Init(VSECPRIM_P2VAR_PARA(actX25519STRUCT) ws)
{
  /* Call init function */
  x25519_initCore(ws); /* SBSW_VSECPRIM_CALL_FUNCTION */

  return actOK;
}

/**********************************************************************************************************************
 *  actX25519GenKeyPair()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actX25519GenKeyPair(
  VSECPRIM_P2VAR_PARA(actX25519STRUCT) ws,
  VSECPRIM_P2VAR_PARA(actU8) publicKey,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal = actGetExternRandom(ws->privateKey, actX25519_KEY_SIZE); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* Generate random ephemeral private key */
  if (retVal != actOK)
  {
    /* Clear memory */
    actMemset(ws->privateKey, 0u, actX25519_KEY_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    retVal = actEXCEPTION_PRIVKEY;
  }
  else
  {
    ws->privateKey[0] &= 0xF8u; /* SBSW_VSECPRIM_WRITE_ARRAY */
    ws->privateKey[31] &= 0x7Fu; /* SBSW_VSECPRIM_WRITE_ARRAY */
    ws->privateKey[31] |= 0x40u; /* SBSW_VSECPRIM_WRITE_ARRAY */

    /* Evaluate curve equation using base point and ephemeral private key */
    actMemset(ws->basePointBuf.uCoord, 0, actX25519_KEY_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    ws->basePointBuf.uCoord[0] = 9u; /* base point = 9 */ /* SBSW_VSECPRIM_WRITE_ARRAY */
    (void)x25519_exchangeKeys(ws, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

    actMemCpyByteArray(publicKey, ws->basePointBuf.uCoord, actX25519_KEY_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  actX25519ImportPrivateKey()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actX25519ImportPrivateKey(
  VSECPRIM_P2VAR_PARA(actX25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) privateKey,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void)) /* PRQA S 3206 */ /* MD_VSECPRIM_WATCHDOG */
{
  /* Import privateKey */
  actMemCpyByteArray(ws->privateKey, privateKey, actX25519_KEY_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  ws->privateKey[0] &= 0xF8u; /* SBSW_VSECPRIM_WRITE_ARRAY */
  ws->privateKey[31] &= 0x7Fu; /* SBSW_VSECPRIM_WRITE_ARRAY */
  ws->privateKey[31] |= 0x40u; /* SBSW_VSECPRIM_WRITE_ARRAY */
  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  return actOK;
}

/**********************************************************************************************************************
 *  actX25519GenSecret()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actX25519GenSecret(
  VSECPRIM_P2VAR_PARA(actX25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) publicKey,
  VSECPRIM_P2VAR_PARA(actU8) sharedSecret,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* Generate shared secret */
  actMemCpyByteArray(ws->basePointBuf.uCoord, publicKey, actX25519_KEY_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  (void)x25519_exchangeKeys(ws, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

  actMemCpyByteArray(sharedSecret, ws->basePointBuf.uCoord, actX25519_KEY_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  return actOK;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTX25519_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actX25519.c
 *********************************************************************************************************************/
