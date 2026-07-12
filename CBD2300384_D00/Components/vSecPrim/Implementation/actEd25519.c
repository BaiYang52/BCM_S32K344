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
/*!        \file  actEd25519.c
 *        \brief  Core Ed25519 implementation.
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

#define ACTED25519_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actUtilities.h"
#include "actExternRandom.h"
#include "actConfig.h"
#include "actWatchdog.h"
#include "actEd25519core.h"
#include "actEd25519.h"

#if (VSECPRIM_ACTED25519_ENABLED == STD_ON)

 /**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) dom2[] = { "SigEd25519 no Ed25519 collisions" }; /* PRQA S 3218 */ /* MD_VSECPRIM_8.7 */
VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) dom2len = 32u; /* PRQA S 3218 */ /* MD_VSECPRIM_8.7 */

# define VSECPRIM_STOP_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  act25519_dom2()
 **********************************************************************************************************************/
/*! \brief      DOM2 Parser
 *  \details    This function handles and processes the DOM2 tag.
 *  \param[in,out]  ws  In: Pointer to the workspace. Out: Workspace with processed message information.
 *  \param[in]  watchdog  Pointer to watchdog function.
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        -
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) act25519_dom2(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actEd25519ImportMessage()
 **********************************************************************************************************************/
/*! \brief      Handles the provided message.
 *  \details    This function is used to cover different instance variants of the EdDSA. Actually only supports the
 *              pre-hash variant.
 *  \param[in,out]  ws  In: Pointer to the workspace. Out: Workspace with processed message information.
 *  \param[in]  message  Pointer to the actual message.
 *  \param[in]  msgLen  Length of the provided message.
 *  \param[in]  watchdog  Pointer to watchdog function.
 *  \return     actOK - Operation successful.
 *              actEXCEPTION_UNKNOWN - Pre-hashing failure.
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        ws must be a valid pointer.
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actEd25519ImportMessage(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) message,
  const actLengthType msgLen,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  act25519_dom2()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) act25519_dom2(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  if (ws->instance != actEd25519pure)
  {
    /* Process DOM2 tag */
    VSECPRIM_P2VAR_PARA(actSHA512STRUCT) sha512WsPtr = &(ws->shaWs.sha512);
    actU8 ph_len[2] = {0u, 0u};

    if (actEd25519ph == ws->instance)
    {
      ph_len[0] = 1u; /* SBSW_VSECPRIM_WRITE_ARRAY */
    }

    if (ws->context != NULL_PTR)
    {
      ph_len[1] = (actU8)(ws->ctxLen & 0xFFu); /* SBSW_VSECPRIM_WRITE_ARRAY */
    }

    (void)actSHA512Update(sha512WsPtr, dom2, dom2len, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    (void)actSHA512Update(sha512WsPtr, ph_len, 2, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

    if ((ws->context != NULL_PTR) && (ws->ctxLen != 0u))
    {
      (void)actSHA512Update(sha512WsPtr, ws->context, ws->ctxLen, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    }
  }
}

/**********************************************************************************************************************
 *  actEd25519ImportMessage()
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
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actEd25519ImportMessage(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) message,
  const actLengthType msgLen,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal = actOK;

  /* Check if context is ph instance */
  if (actEd25519ph == ws->instance)
  {
    /* Prehash provided message */
    if (actOK != (actSHA512Update(&(ws->shaWs.sha512), message, msgLen, watchdog))) /* SBSW_VSECPRIM_CALL_FUNCTION */
    {
      retVal = actEXCEPTION_UNKNOWN;
    }
    else
    {
      if (actOK != (actSHA512Finalize(&(ws->shaWs.sha512), ws->hashBuf.hashValue, watchdog))) /* SBSW_VSECPRIM_CALL_FUNCTION */
      {
        retVal = actEXCEPTION_UNKNOWN;
      }
      else
      {
        /* Store message informations */
        ws->message = ws->hashBuf.hashValue; /* SBSW_VSECPRIM_WRITE_POINTER */
        ws->msgLen = actHASH_SIZE_SHA512; /* SBSW_VSECPRIM_WRITE_POINTER */
      }
    }
  }
  else
  {
    /* Store plain message informations */
    ws->message = message; /* SBSW_VSECPRIM_WRITE_POINTER */
    ws->msgLen = msgLen; /* SBSW_VSECPRIM_WRITE_POINTER */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  act25519_hashPM()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) act25519_hashPM(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2VAR_PARA(actSHA512STRUCT) sha512WsPtr = &(ws->shaWs.sha512);

  /* Init SHA-512 */
  (void)actSHA512Init(sha512WsPtr); /* SBSW_VSECPRIM_CALL_FUNCTION */
  /* Process DOM2 tag */
  act25519_dom2(ws, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* Hash the private key and message */
  (void)actSHA512Update(sha512WsPtr, &ws->privateKey[32], 32, watchdog); /* prefix */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  (void)actSHA512Update(sha512WsPtr, ws->message, ws->msgLen, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
  (void)actSHA512Finalize(sha512WsPtr, hash, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

  return actOK;
}

/**********************************************************************************************************************
 *  act25519_hashRAM()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) act25519_hashRAM(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) R,
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2VAR_PARA(actSHA512STRUCT) sha512WsPtr = &(ws->shaWs.sha512);
  /* Hash R, A and the Message */
  (void)actSHA512Init(sha512WsPtr); /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_dom2(ws, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

  (void)actSHA512Update(sha512WsPtr, R, 32, watchdog); /* R */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  (void)actSHA512Update(sha512WsPtr, ws->pubKeyBuf.publicKey, 32, watchdog); /* A */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  (void)actSHA512Update(sha512WsPtr, ws->message, ws->msgLen, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
  (void)actSHA512Finalize(sha512WsPtr, hash, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

  return actOK;
}

/**********************************************************************************************************************
 *  actEd25519Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actEd25519Init(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  const actU8 instance,
  VSECPRIM_P2CONST_PARA(actU8) context,
  const actLengthType ctxLen)
{
  /* Initialize SHA-512, when instance is pre-hash */
  ws->instance = instance; /* SBSW_VSECPRIM_WRITE_POINTER */

  if (actEd25519ph == instance)
  {
    (void)actSHA512Init(&(ws->shaWs.sha512)); /* no need to check for error - returns actOK anyway */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  /* Initialize EdDSA core */
  actEd25519_initCore(ws); /* SBSW_VSECPRIM_CALL_FUNCTION */
  ws->context = context; /* SBSW_VSECPRIM_WRITE_POINTER */
  ws->ctxLen = ctxLen; /* SBSW_VSECPRIM_WRITE_POINTER */

  return actOK;
}

/**********************************************************************************************************************
 *  actEd25519Update()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actEd25519Update(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) message,
  const actLengthType msgLen,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal = actEXCEPTION_ALGOID;

  /* Check instance */
  if (actEd25519ph != ws->instance)
  {
    /* Return value already set */
  }
  else
  {
    /* Update SHA-512 */
    retVal = actSHA512Update(&(ws->shaWs.sha512), message, msgLen, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  actEd25519Sign()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actEd25519Sign(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) message,
  const actLengthType msgLen,
  VSECPRIM_P2CONST_PARA(actU8) secretKey,
  VSECPRIM_P2CONST_PARA(actU8) publicKey,
  VSECPRIM_P2VAR_PARA(actU8) signature,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* Handle instance variants */
  actRETURNCODE retVal = actEd25519ImportMessage(ws, message, msgLen, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

  if (retVal == actOK)
  {
    /* Import key pairs */
    actEd25519_importKeyPair(ws, secretKey, publicKey, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* Sign provided message */
    actEd25519_sign(ws, signature, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

    /* Clear local memory */
    actMemset(ws->privateKey, 0u, sizeof(ws->privateKey)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    actMemset(ws->pubKeyBuf.publicKey, 0u, sizeof(ws->pubKeyBuf.publicKey)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

    actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actEd25519Verify()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actEd25519Verify(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) message,
  const actLengthType msgLen,
  VSECPRIM_P2CONST_PARA(actU8) publicKey,
  VSECPRIM_P2CONST_PARA(actU8) signature,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* Handle instance variants */
  actRETURNCODE retVal = actEd25519ImportMessage(ws, message, msgLen, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

  if (retVal == actOK)
  {
    /* Import the key pair */
    actEd25519_importKeyPair(ws, NULL_PTR, publicKey, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* Verify provided signature over given message */
    retVal = actEd25519_verify(ws, signature, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

    /* Clear local memory */
    actMemset(ws->pubKeyBuf.publicKey, 0u, sizeof(ws->pubKeyBuf.publicKey)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    /*  ws->privateKey: no need to clear - holds the signature (re-) calculated */
    actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actEd25519GenKeyPair()
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
VSECPRIM_FUNC(actRETURNCODE) actEd25519GenKeyPair(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2VAR_PARA(actU8) secretKey,
  VSECPRIM_P2VAR_PARA(actU8) publicKey,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* Generate secretKey randomly */
  actRETURNCODE retVal = actGetExternRandom(secretKey, actEd25519_KEY_SIZE); /* SBSW_VSECPRIM_CALL_FUNCTION */

  if (retVal != actOK)
  {
    /* Failed to get random bytes */
    actMemset(secretKey, 0u, actEd25519_KEY_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    retVal = actEXCEPTION_PRIVKEY;
  }
  else
  {
    /* Generate key pair from random private key */
    actEd25519_importKeyPair(ws, secretKey, NULL_PTR, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* Copy public key from workspace to user buffer */
    actMemCpyByteArray(publicKey, ws->pubKeyBuf.publicKey, actEd25519_KEY_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

    /* Clear workspace */
    actMemset(ws->privateKey, 0u, sizeof(ws->privateKey)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    actMemset(ws->pubKeyBuf.publicKey, 0u, sizeof(ws->pubKeyBuf.publicKey)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

    actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTED25519_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actEd25519.c
 *********************************************************************************************************************/
