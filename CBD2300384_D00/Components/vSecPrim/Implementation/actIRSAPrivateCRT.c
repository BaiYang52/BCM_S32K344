/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2006 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  actIRSAPrivateCRT.c
 *        \brief  Implementation file for actIRSA.h
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
#define ACTIRSAPRIVATECRT_SOURCE

#include "actIRSA.h"
#include "actIRSAExp.h"
#include "actBigNum.h"
#include "actUtilities.h"
#include "actWatchdog.h"

#if (VSECPRIM_ACTIRSAPRIVATECRT_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/* PRQA S 2889 EOF */ /* MD_VSECPRIM_QM_ONLY_RETURN_PATHS */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * actRSAInitPrivateKeyOperationCRT()
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
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actRSAInitPrivateKeyOperationCRT(
  VSECPRIM_P2CONST_PARA(actU8) prime_p,
  actLengthType prime_p_len,
  VSECPRIM_P2CONST_PARA(actU8) prime_q,
  actLengthType prime_q_len,
  VSECPRIM_P2CONST_PARA(actU8) private_exponent_dp,
  actLengthType private_exponent_dp_len,
  VSECPRIM_P2CONST_PARA(actU8) private_exponent_dq,
  actLengthType private_exponent_dq_len,
  VSECPRIM_P2CONST_PARA(actU8) q_inverse_mod_p,
  actLengthType q_inverse_mod_p_len,
  VSECPRIM_P2VAR_PARA(actRSACRTSTRUCT) wsCRT,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal;
  actLengthType p_length = actBNGetDigitLength(prime_p_len);
  actLengthType q_length = actBNGetDigitLength(prime_q_len);

  VSECPRIM_P2VAR_PARA(actBNDIGIT) p = wsCRT->wsUnion1.wsCRTInit.primeP;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) q = wsCRT->wsUnion1.wsCRTInit.primeQ;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) dp = wsCRT->wsUnion1.wsCRTInit.primeDP;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) dq = wsCRT->wsUnion1.wsCRTInit.primeDQ;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) qInv = wsCRT->wsUnion1.wsCRTInit.qInv;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) one = wsCRT->wsUnion1.wsCRTInit.one;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) t6 = wsCRT->keySizeBuffer.fullModulus;

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* length checks  */
  if ((private_exponent_dp_len > prime_p_len) || (private_exponent_dq_len > prime_q_len) || (q_inverse_mod_p_len > prime_p_len))
  {
    retVal = actEXCEPTION_PRIVKEY;
  }
  /* check workspace length for both exponentiations (refer to actRSAInitExponentiation)
   * Check prime sizes individually: One of the two primes can be 1 (2/4) digits longer than
   * modulus_length_in_digits/2, and we do not know whether p or q is longer
   * Check prime sizes combined: In sum, p and q measured in digits are only allowed to
   * be one digit longer than the modulus */
  else if ((p_length > actRSA_CRT_PRIME_SIZE) || (q_length > actRSA_CRT_PRIME_SIZE) || ((p_length + q_length) > actRSA_CRT_PRIMEPAIR_SIZE))
  {
    retVal = actEXCEPTION_MEMORY;
  }
  else
  {
    /* BigNum initialization of p, q, dp, dq, qInv */
    actBNSetOctetStringROM(p, p_length, prime_p, prime_p_len);
    actBNSetOctetStringROM(q, q_length, prime_q, prime_q_len);
    actBNSetOctetStringROM(dp, p_length, private_exponent_dp, private_exponent_dp_len);
    actBNSetOctetStringROM(dq, q_length, private_exponent_dq, private_exponent_dq_len);
    actBNSetOctetStringROM(qInv, p_length, q_inverse_mod_p, q_inverse_mod_p_len);

    actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

    /* calculate n = p*q */
    /* n is saved in t6 for size checks in actRSAPrivateKeyOperationCRT() function */
    actBNMult(p, p_length, q, q_length, t6, VSECPRIM_FUNC_NULL_PTR);
    /* calculate exact byte length of n */
    wsCRT->n_bytes = (actBNGetBitLength(t6, p_length + q_length) + 7u) / 8u;

    /* check 0 < qInv < p  before continue */
    if (actBNIsZero(qInv, p_length) == TRUE)
    {
      retVal = actEXCEPTION_PRIVKEY;
    }
    else if (actBNCompare(qInv, p, p_length) >= actCOMPARE_EQUAL)
    {
      retVal = actEXCEPTION_PRIVKEY;
    }
    else
    {
      actBNSetZero(one, actMax(p_length, q_length));
      one[0] = (actBNDIGIT)1;

      /* check 0 < dp < (p-1) and check 0 < dq < (q-1) before continue */
      actBNSub(p, one, p, p_length); /* p = p-1 */
      actBNSub(q, one, q, q_length); /* q = q-1 */

      if (actBNIsZero(dp, p_length) == TRUE)
      {
        retVal = actEXCEPTION_PRIVKEY;
      }
      else if (actBNCompare(dp, p, p_length) >= actCOMPARE_EQUAL)
      {
        retVal = actEXCEPTION_PRIVKEY;
      }
      else if (actBNIsZero(dq, q_length) == TRUE)
      {
        retVal = actEXCEPTION_PRIVKEY;
      }
      else if (actBNCompare(dq, q, q_length) >= actCOMPARE_EQUAL)
      {
        retVal = actEXCEPTION_PRIVKEY;
      }
      else
      {
        wsCRT->p = prime_p;
        wsCRT->p_bytes = prime_p_len;
        wsCRT->q = prime_q;
        wsCRT->q_bytes = prime_q_len;
        wsCRT->dp = private_exponent_dp;
        wsCRT->dp_bytes = private_exponent_dp_len;
        wsCRT->dq = private_exponent_dq;
        wsCRT->dq_bytes = private_exponent_dq_len;
        wsCRT->q_inv = q_inverse_mod_p;
        wsCRT->q_inv_bytes = q_inverse_mod_p_len;

        retVal = actOK;
      }
    }
  }

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  return retVal;
} /* PRQA S 6010, 6030, 6050, 6060, 6080 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

/**********************************************************************************************************************
 * actRSAPrivateKeyOperationCRT()
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
VSECPRIM_FUNC(actRETURNCODE) actRSAPrivateKeyOperationCRT(
  VSECPRIM_P2CONST_PARA(actU8) cipher,
  actLengthType cipher_len,
  VSECPRIM_P2VAR_PARA(actU8) message,
  VSECPRIM_P2VAR_PARA(actLengthType) message_len,
  VSECPRIM_P2VAR_PARA(actRSACRTSTRUCT) wsCRT,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE rc;
  VSECPRIM_P2VAR_PARA(actBNRING) ring; /* PRQA S 3678 */ /* MD_VSECPRIM_16.7 */

  /* Pointers for CRT STEP 1 and 2 */
  VSECPRIM_P2VAR_PARA(actBNDIGIT) m1;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) m2;

  VSECPRIM_P2CONST_PARA(actBNDIGIT) fullModulus;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) origCipherBeforeReduction;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) reducedCipherInDigits;
  VSECPRIM_P2VAR_PARA(actU8) reducedCipherInBytes;

  /* Pointers for CRT STEP 3 */
  VSECPRIM_P2VAR_PARA(actBNDIGIT) m2Copy;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) m2ModP;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) m1m2;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) qInv;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) qInvM1M2R;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) qInvM1M2;

  /* Pointers for CRT STEP 4 */
  VSECPRIM_P2VAR_PARA(actBNDIGIT) primeQ;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) qInvM1M2q;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) m2Padded;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) msg;

  /* Length values in bytes */
  actLengthType p_bytes = wsCRT->p_bytes;
  actLengthType q_bytes = wsCRT->q_bytes;
  actLengthType n_bytes = wsCRT->n_bytes;

  /* Length values in digits */
  actLengthType p_length = actBNGetDigitLength(p_bytes);
  actLengthType q_length = actBNGetDigitLength(q_bytes);
  actLengthType n_length = actBNGetDigitLength(n_bytes);

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* check cipher length */
  if (cipher_len > n_bytes)
  {
    return actRSA_PARAM_OUT_OF_RANGE;
  }

  /* check output buffer length */
  if (*message_len < n_bytes)
  {
    return actEXCEPTION_MEMORY;
  }

  /* Initialize pointers for CRT STEP 1 and 2 */
  fullModulus = wsCRT->keySizeBuffer.fullModulus;
  origCipherBeforeReduction = wsCRT->wsUnion2.wsCRTCipherOperations.origCipherBeforeReduction;
  reducedCipherInBytes = wsCRT->wsUnion2.wsCRTCipherOperations.reducedCipherInBytes;
  reducedCipherInDigits = wsCRT->wsUnion2.wsCRTCipherOperations.reducedCipherInDigits;

  /* Preliminary cipher checks:
   * bring original cipher to actBIGNUM format and check 0 < cipher c < n */
  actBNSetOctetString(origCipherBeforeReduction, n_length, cipher, cipher_len);

  if (actBNIsZero(origCipherBeforeReduction, n_length) == TRUE)
  {
    return actRSA_PARAM_OUT_OF_RANGE;
  }
  /* the original modulus n is written to wsCRT->keySizeBuffer.fullModulus by actRSAInitPrivateKeyOperationCRT() function */
  if (actBNCompare(origCipherBeforeReduction, fullModulus, n_length) >= actCOMPARE_EQUAL)
  {
    return actRSA_PARAM_OUT_OF_RANGE;
  }

  /*
   * CRT STEP 1: calculate m2 = cq^dq mod q
   */

  m1 = wsCRT->keySizeBuffer.messages.m1;
  m2 = wsCRT->keySizeBuffer.messages.m2;

  /* The ring members will be initialized during actRSAInitExponentiation().
   * The pointers in ring will point to wsCRT->wsUnion1.wsRSAPrim.modulus (modulus p or q) and
   * wsCRT->wsUnion1.wsRSAPrim.RR (RR). */
  ring = &(wsCRT->wsUnion1.wsRSAPrim.wsRSARing);

  /* init q_ring exponentiation
   * modulus, buffer and exponent will be initialized here (q, R^2, dq: each of q_length)
   * ring->m, ring->RR will be set here */
  rc = actRSAInitExponentiation(wsCRT->q, q_bytes, wsCRT->dq, wsCRT->dq_bytes, &(wsCRT->wsUnion1.wsRSAPrim),
    actRSA_PRIVATE_KEY_OPERATION_CRT, watchdog);
  if (rc != actOK)
  {
    return rc;
  }

  /* calculate the reduced cipher cq = c mod q and store the result in reducedCipherInDigits;
   * actBNReduce returns ok, because q_length>=2 is checked in actRSAInitExponentiation */
  (void)actBNReduce(origCipherBeforeReduction, n_length, ring->m, q_length, reducedCipherInDigits, watchdog);

  /* transform reduced cipher from actBIGNUM format to actU8 format */
  actBNOctetString(reducedCipherInBytes, q_bytes, reducedCipherInDigits, q_length);

  /* no output buffer is passed, the result m2 will be stored in the workspace */
  (void)actRSAExponentiation(reducedCipherInBytes, q_bytes, (VSECPRIM_P2VAR_PARA(actU8)) 0,
    (VSECPRIM_P2VAR_PARA(actLengthType)) 0, &(wsCRT->wsUnion1.wsRSAPrim), watchdog);

  /* copy result from RSA exponentiation to m2 */
  actBNCopy(m2, wsCRT->wsUnion1.wsRSAPrim.tmp.cipherAndMsg.message, q_length);

  /*
   * CRT STEP 2: calculate m1 = cp^dp mod p
   */

  /* init p_ring exponentiation
   * modulus, buffer and exponent will be initialized here (p, R^2, dp: each of p_length)
   * ring->m, ring->RR will be re-set here */
  rc = actRSAInitExponentiation(wsCRT->p, p_bytes, wsCRT->dp, wsCRT->dp_bytes, &(wsCRT->wsUnion1.wsRSAPrim),
    actRSA_PRIVATE_KEY_OPERATION_CRT, watchdog);
  if (rc != actOK)
  {
    return rc;
  }

  /* since actBNReduce may change the first input parameter, it is necessary to refresh the
     original cipher value in origCipherBeforeReduction */
  actBNSetOctetString(origCipherBeforeReduction, n_length, cipher, cipher_len);

  /* calculate the reduced cipher cp = c mod p,
   * actBNReduce returns ok, because q_length>=2 is checked in actRSAInitExponentiation */
  (void)actBNReduce(origCipherBeforeReduction, n_length, ring->m, p_length, reducedCipherInDigits, watchdog);

  /* transform reduced cipher from actBIGNUM format to actU8 format */
  actBNOctetString(reducedCipherInBytes, p_bytes, reducedCipherInDigits, p_length);

  /* no output buffer is passed, the result m1 will be stored in the workspace */
  (void)actRSAExponentiation(reducedCipherInBytes, p_bytes, (VSECPRIM_P2VAR_PARA(actU8)) 0,
    (VSECPRIM_P2VAR_PARA(actLengthType)) 0, &(wsCRT->wsUnion1.wsRSAPrim), watchdog);

  /* copy result from RSA exponentiation to m1 */
  actBNCopy(m1, wsCRT->wsUnion1.wsRSAPrim.tmp.cipherAndMsg.message, p_length);

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /*
   * CRT STEP 3: calculate h = (m1-m2)*qInv mod p
   */

  /* initialize pointers for CRT STEP 3 */
  m2Copy = wsCRT->wsUnion2.wsCRTStep3.m2Copy;
  m2ModP = wsCRT->wsUnion2.wsCRTStep3.m2ModP;
  m1m2 = wsCRT->wsUnion2.wsCRTStep3.m1m2;
  qInv = wsCRT->wsUnion2.wsCRTStep3.qInv;
  qInvM1M2R = wsCRT->wsUnion2.wsCRTStep3.qInvM1M2R;
  qInvM1M2 = wsCRT->wsUnion2.wsCRTStep3.qInvM1M2;

  /* save unreduced m2 to m2Copy since the value in m2 could be modified by actBNReduce;
   * then reduce m2 mod p, store result in m2ModP */
  actBNCopy(m2Copy, m2, q_length);
  /* actBNReduce returns ok, because p_length>=2 is checked in actRSAInitExponentiation */
  (void)actBNReduce(m2, q_length, ring->m, p_length, m2ModP, watchdog);

  /* calculate  m1 = m1-m2 mod p
   * Prerequisites for actBNModSub: m1 < ring->m (= p) by definition of m1, m2modP < ring->m (= p) due to actBNReduce */
  actBNModSub(m1, m2ModP, m1m2, ring, watchdog);

  /* calculate h = (m1-m2)*qInv mod p */

  actBNSetOctetStringROM(qInv, p_length, wsCRT->q_inv, wsCRT->q_inv_bytes);

  /* qInvM1M2R = (m1-m2 mod p)*qInv*R^(-1) mod p */
  actBNMontMul(m1m2, qInv, qInvM1M2R, ring, watchdog);
  /* qInvM1M2 = (m1-m2)*qInv mod p = h */
  actBNMontMul(qInvM1M2R, ring->RR, qInvM1M2, ring, watchdog);

  /*
   * CRT STEP 4: calculate m = m2 + q*h
   */

  /* initialize pointers for CRT STEP 4 */
  m2Padded = wsCRT->wsUnion1.wsCRTSTEP4.m2Padded;
  primeQ = wsCRT->wsUnion1.wsCRTSTEP4.primeQ;
  qInvM1M2q = wsCRT->wsUnion1.wsCRTSTEP4.qInvM1M2q;
  msg = wsCRT->wsUnion1.wsCRTSTEP4.msg;

  /* store q in primeQ in actBIGNUM format */
  actBNSetOctetStringROM(primeQ, q_length, wsCRT->q, wsCRT->q_bytes);
  /* qInvM1M2q = q * h */
  actBNMult(primeQ, q_length, qInvM1M2, p_length, qInvM1M2q, watchdog);

  /* pad the original message m2 with leading zeros to n_length (LE digit order) */
  actBNCopy(m2Padded, m2Copy, q_length);
  actBNSetZero(&m2Padded[q_length], n_length - q_length);
  /* msg = q * h + m2 */
  (void)actBNAdd(qInvM1M2q, m2Padded, msg, n_length);

  /* write msg to message buffer */
  actBNOctetString(message, n_bytes, msg, n_length);
  *message_len = n_bytes;

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  return actOK;
} /* PRQA S 6050, 6060 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTIRSAPRIVATECRT_ENABLED == STD_ON) */
