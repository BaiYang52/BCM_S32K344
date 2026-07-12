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
/*!        \file  actEd25519core.c
 *        \brief  Core Ed25519 implementation.
 *
 *      \details This file is part of the embedded systems library cvActLib
 *               Reference: https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08
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

#define ACTED25519CORE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actEd25519core.h"
#include "actWatchdog.h"
#include "actUtilities.h"
#include "actISHA2_64.h"
#include "actEd25519.h"
#include "act25519util.h"

#if (VSECPRIM_ACTED25519CORE_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actEd25519_initCore()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actEd25519_initCore(VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws) /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
{
  /* Init core */
  act25519_initCore((VSECPRIM_P2VAR_PARA(actCurve25519ws))&ws->coreWS); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 *  actEd25519_sign()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actEd25519_sign(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2VAR_PARA(actU8) signature,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2VAR_PARA(actCurve25519ws) coreWSptr = (VSECPRIM_P2VAR_PARA(actCurve25519ws)) &ws->coreWS;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a = ((VSECPRIM_P2VAR_PARA(actBNDIGIT)) (coreWSptr->auxVar.tempA));
  VSECPRIM_P2VAR_PARA(actBNDIGIT) k = ((VSECPRIM_P2VAR_PARA(actBNDIGIT)) (coreWSptr->auxVar.tempB));
  VSECPRIM_P2VAR_PARA(actBNDIGIT) s = ((VSECPRIM_P2VAR_PARA(actBNDIGIT)) (coreWSptr->auxVar.tempC));
  VSECPRIM_P2VAR_PARA(actPoint) rB = ((VSECPRIM_P2VAR_PARA(actPoint))&(ws->shaWs.point_rB));
  VSECPRIM_P2VAR_PARA(actU8) hash = ((VSECPRIM_P2VAR_PARA(actU8))(coreWSptr->auxVar.tempA)); /* need  64 bytes - temA || tempB   is  72 bytes */

  actBNDIGIT r[BNDIGITS_256];

  /* Hash the prefix and the message to derive r */
  (void)act25519_hashPM(ws, hash, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
  actBNLoadLE(coreWSptr->auxVar.temp1, hash, actHASH_SIZE_SHA512); /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_reduceQ(coreWSptr, r, coreWSptr->auxVar.temp1); /* r = hash mod Q */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* Encode rB into the first half of the signature buffer as R */
  act25519_mulBasePoint(coreWSptr, rB, r, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_encodePoint(coreWSptr, signature, rB, watchdog);  /* R */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* Hash R, A, and the message to get k */
  (void)act25519_hashRAM(ws, signature, ws->hashBuf.hashValue, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

  actBNLoadLE(coreWSptr->auxVar.temp1, ws->hashBuf.hashValue, actHASH_SIZE_SHA512); /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_reduceQ(coreWSptr, k, coreWSptr->auxVar.temp1); /* k = hash mod Q */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* Compute s = (r + k * a) mod q */
  actBNLoadLE(a, ws->privateKey, 32u); /* private scalar -> a */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_mulModQ(coreWSptr, s, k, a); /* s =     (k * a) mod q */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  act25519_addModQ(coreWSptr, s, r, s); /* s = (r + k * a) mod q */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  actBNStoreLE(&signature[32], s, BNDIGITS_256); /* SBSW_VSECPRIM_CALL_FUNCTION */

  actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* Clean up */
  actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempA, 0u, sizeof(coreWSptr->auxVar.tempA)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempB, 0u, sizeof(coreWSptr->auxVar.tempB)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempC, 0u, sizeof(coreWSptr->auxVar.tempC)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.temp1, 0u, sizeof(coreWSptr->auxVar.temp1)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  actEd25519_verify()
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
VSECPRIM_FUNC(actRETURNCODE) actEd25519_verify(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) signature,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2VAR_PARA(actCurve25519ws) coreWSptr = (VSECPRIM_P2VAR_PARA(actCurve25519ws)) &ws->coreWS;
  /* Even if sB/A abnd s/k are taken from union, they are not used concurrently!
   * For better readability and comparability to algorithm specification, addtional variables pointing to same address are used. */
  VSECPRIM_P2VAR_PARA(actPoint) A = ((VSECPRIM_P2VAR_PARA(actPoint))&(ws->shaWs.point_A));
  VSECPRIM_P2VAR_PARA(actPoint) sB = ((VSECPRIM_P2VAR_PARA(actPoint))&(ws->shaWs.point_sB));
  VSECPRIM_P2VAR_PARA(actBNDIGIT) s = ((VSECPRIM_P2VAR_PARA(actBNDIGIT))&(ws->hashBuf.bnDigit_s));
  VSECPRIM_P2VAR_PARA(actBNDIGIT) k = ((VSECPRIM_P2VAR_PARA(actBNDIGIT))&(ws->hashBuf.bnDigit_k));
  actPoint R;
  actRETURNCODE result = actVERIFICATION_FAILED;

  /* Reconstruct the k value from the signing step */
  (void)act25519_hashRAM(ws, signature, ws->hashBuf.hashValue, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* Decode the public key and the R component of the signature */
  if (actOK == act25519_decodePoint(coreWSptr, A, ws->pubKeyBuf.publicKey, watchdog)) /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    if (actOK == act25519_decodePoint(coreWSptr, &R, signature, watchdog)) /* SBSW_VSECPRIM_CALL_FUNCTION */
    {
      /* Calculate R + k * A */
      actBNLoadLE(coreWSptr->auxVar.temp1, ws->hashBuf.hashValue, actHASH_SIZE_SHA512); /* SBSW_VSECPRIM_CALL_FUNCTION */
      act25519_reduceQ(coreWSptr, k, coreWSptr->auxVar.temp1); /* k = hash mod Q */ /* SBSW_VSECPRIM_CALL_FUNCTION */
      act25519_mulAddPoint(coreWSptr, &R, A, k, watchdog); /* R = R + k * A */ /* SBSW_VSECPRIM_CALL_FUNCTION */

      /* Calculate s * B */
      actBNLoadLE(s, &signature[32], 32); /* SBSW_VSECPRIM_CALL_FUNCTION */
      act25519_mulBasePoint(coreWSptr, sB, s, watchdog); /* sB = s * B */ /* SBSW_VSECPRIM_CALL_FUNCTION */

      /* Compare s * B and R + k * A for equality */
      if (act25519_comparePoints(coreWSptr, sB, &R, watchdog) == actOK) /* SBSW_VSECPRIM_CALL_FUNCTION */
      {
        result = actOK;
      }
      else
      {
        result = actVERIFICATION_FAILED;
      }
    }
  }

  /* clean up and exit */
  actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempA, 0u, sizeof(coreWSptr->auxVar.tempA)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempB, 0u, sizeof(coreWSptr->auxVar.tempB)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempC, 0u, sizeof(coreWSptr->auxVar.tempC)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempD, 0u, sizeof(coreWSptr->auxVar.tempD)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.temp1, 0u, sizeof(coreWSptr->auxVar.temp1)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  return result;
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  actEd25519_importKeyPair()
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
VSECPRIM_FUNC(void)actEd25519_importKeyPair(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) secretKey,
  VSECPRIM_P2CONST_PARA(actU8) publicKey,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  if (NULL_PTR != secretKey)
  {
    /* Hash the secret key to get the private scalar 'a' and the message prefix. */
    (void)actSHA512Init(&(ws->shaWs.sha512)); /* SBSW_VSECPRIM_CALL_FUNCTION */
    (void)actSHA512Update(&(ws->shaWs.sha512), secretKey, actEd25519_KEY_SIZE, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    (void)actSHA512Finalize(&(ws->shaWs.sha512), ws->privateKey, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

    ws->privateKey[0] &= 0xF8u; /* SBSW_VSECPRIM_WRITE_ARRAY */
    ws->privateKey[31] &= 0x7Fu; /* SBSW_VSECPRIM_WRITE_ARRAY */
    ws->privateKey[31] |= 0x40u; /* SBSW_VSECPRIM_WRITE_ARRAY */
  }

  if (NULL_PTR != publicKey)
  {
    /* Copy public key */
    actMemCpyByteArray(ws->pubKeyBuf.publicKey, publicKey, actEd25519_KEY_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  }
  else
  {
    /*  Calculate public key from private key
     *  to conserve (re-use) memory:
     *  - A is overlayed onto the SHA-512 work-space
     *  - a is overlayed onto the public key
     */
    VSECPRIM_P2VAR_PARA(actCurve25519ws) coreWSptr = (VSECPRIM_P2VAR_PARA(actCurve25519ws)) &ws->coreWS;
    VSECPRIM_P2VAR_PARA(actPoint) A = ((VSECPRIM_P2VAR_PARA(actPoint))&(ws->shaWs.point_A));
    VSECPRIM_P2VAR_PARA(actBNDIGIT) a = ((VSECPRIM_P2VAR_PARA(actBNDIGIT))&(ws->pubKeyBuf.bnDigit_a));

    /* Compute  (a * BP)  and encode the point (-> public key) */
    actBNLoadLE(a, ws->privateKey, actEd25519_KEY_SIZE); /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_mulBasePoint(coreWSptr, A, a, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    act25519_encodePoint(coreWSptr, ws->pubKeyBuf.publicKey, A, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

    /* Clean up workspace */
    actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempA, 0u, sizeof(coreWSptr->auxVar.tempA)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempB, 0u, sizeof(coreWSptr->auxVar.tempB)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempC, 0u, sizeof(coreWSptr->auxVar.tempC)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    actMemset((VSECPRIM_P2VAR_PARA(actU8))coreWSptr->auxVar.tempD, 0u, sizeof(coreWSptr->auxVar.tempD)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  }
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTED25519CORE_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actEd25519core.c
 *********************************************************************************************************************/
