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
/*!        \file  actIRSAExp.c
 *        \brief  Implementation for actIRSAExp.h
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
#define ACTIRSAEXP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actIRSA.h"
#include "actIRSAExp.h"
#include "actBigNum.h"
#include "actUtilities.h"
#include "actWatchdog.h"

#if (VSECPRIM_ACTIRSAEXP_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
  actRSAInitExpCheckInputParameters()
 *********************************************************************************************************************/
/*! \brief       Preliminary checks on the input parameters during actRSAInitExponentiation().
 *  \details     -
 *  \param[in]     modulus_len    length of modulus
 *  \param[in]     exponent_len   length of exponent
 *  \param[in]     n_length       length of the modulus in actBNDIGITs
 *  \param[in]     key_flag       parameter to determine whether the exponent is a public or private exponent
 *  \return    actEXCEPTION_PRIVKEY    exponent_len > modulus_len and key_flag = actRSA_PRIVATE_KEY_OPERATION
 *  \return    actEXCEPTION_PUBKEY     exponent_len > modulus_len and key_flag = actRSA_PUBLIC_KEY_OPERATION
 *  \return    actEXCEPTION_MEMORY     the provided workspace is too small
 *  \return    actOK                   else
 *  \pre         wksp must be a valid pointer
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actRSAInitExpCheckInputParameters(
  actLengthType modulus_len,
  actLengthType exponent_len,
  actLengthType n_length,
  actU16 key_flag);

/**********************************************************************************************************************
  actRSAInitExpCheckExponent()
 *********************************************************************************************************************/
/*! \brief     Preliminary checks on the exponent during actRSAInitExponentiation().
 *  \details    This function checks if the exponent takes on values which are too small or too large.
 *  \param[in]  key_flag  Parameter to determine whether e is a public o a private exponent
 *  \param[in]  e         exponent of the RSA scheme
 *  \param[in]  modulus   modulus of the RSA scheme
 *  \param[in]  n_length  length of the modulus in bytes
 *  \param[in]  watchdog  watchdog function pointer
 *  \return     actEXCEPTION_PUBKEY   if e is out of range and a public key
 *  \return     actEXCEPTION_PRIVKEY  if e is out of range and a private key
 *  \return     actOK else
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        e and modulus must be valid pointers
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actRSAInitExpCheckExponent(
  actU16 key_flag,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) e,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) modulus,
  actLengthType n_length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 actRSAExpInputCheck()
 *********************************************************************************************************************/
/*! \brief      Preliminary checks on the input parameters during actRSAExponentiation().
 *  \details    -
 *  \param[in]  message             buffer to store the decrypted cipher text message m
 *  \param[in]  message_len         length of message buffer
 *  \param[in]  cipher_len          length of cipher
 *  \param[in]  modulusLengthBytes  length of the modulus in Bytes
 *  \return     actEXCEPTION_MEMORY        *message_len < modulusLengthBytes
 *  \return     actRSA_PARAM_OUT_OF_RANGE  cipher >= modulus
 *  \return     actOK                      else
 *  \return     actOK else
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        message_len must be a valid pointer
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actRSAExpInputCheck(
  VSECPRIM_P2CONST_PARA(actU8) message,
  VSECPRIM_P2CONST_PARA(actLengthType) message_len,
  actLengthType cipher_len,
  actLengthType modulusLengthBytes);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  actRSAInitExpCheckInputParameters()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actRSAInitExpCheckInputParameters(
  actLengthType modulus_len,
  actLengthType exponent_len,
  actLengthType n_length,
  actU16 key_flag)
{
  actRETURNCODE returnValue = actOK;
  actU32 key_out_of_range = ((key_flag == actRSA_PUBLIC_KEY_OPERATION) ? actEXCEPTION_PUBKEY : actEXCEPTION_PRIVKEY);

  /* check exponent length */
  if (exponent_len > modulus_len)
  {
    returnValue = key_out_of_range;
  }

  if (returnValue == actOK)
  {
    /* Note: If key_flag == actRSA_PRIVATE_KEY_OPERATION_CRT, it was already checked before that
       n_length < actRSA_CRT_PRIME_SIZE, so in this case, the following check is redundant.
       However, the check is required, if this is not a CRT operation. */

    if (n_length > actRSA_MAX_KEY_SIZE_IN_DIGIT)
    {
      returnValue = actEXCEPTION_MEMORY;
    }
  }

  return returnValue;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actRSAInitExpCheckExponent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actRSAInitExpCheckExponent(
  actU16 key_flag,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) e,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) modulus,
  actLengthType n_length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE returnValue = actOK;
  actU32 key_out_of_range = ((key_flag == actRSA_PUBLIC_KEY_OPERATION) ? actEXCEPTION_PUBKEY : actEXCEPTION_PRIVKEY);

  if (key_flag != actRSA_PRIVATE_KEY_OPERATION_CRT) /* checks are done in actRSAInitPrivateKeyOperationCRT */
  {
    if (key_flag == actRSA_PRIVATE_KEY_OPERATION)
    {
      /* e is the private exponent: check e > 0 before continue */
      if (actBNIsZero(e, n_length) == TRUE) /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      {
        returnValue = key_out_of_range;
      }
    }
    else
    {
      /* e is the public exponent: check e odd, e >= 3 before continue */
      if (e[0] <= 1u)
      {
        if (actBNIsZero(&e[1], n_length - 1u) == TRUE) /* e<=1 */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        {
          returnValue = key_out_of_range;
        }
      }
      else if ((actBNIsEven(e))) /* e is even */
      {
        returnValue = key_out_of_range;
      }
      else
      {
        /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
      }
    }

    actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

    /* check e < n before continue */
    if (returnValue == actOK)
    {
      if (actBNCompare(e, modulus, n_length) >= actCOMPARE_EQUAL) /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      {
        returnValue = key_out_of_range;
      }
    }
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  actRSAExpInputCheck()
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
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actRSAExpInputCheck(
  VSECPRIM_P2CONST_PARA(actU8) message,
  VSECPRIM_P2CONST_PARA(actLengthType) message_len,
  actLengthType cipher_len,
  actLengthType modulusLengthBytes)
{
  actRETURNCODE returnValue = actOK;

  if ((message != NULL_PTR) && (*message_len < modulusLengthBytes))
  {
    returnValue = actEXCEPTION_MEMORY;
  }

  if (cipher_len > modulusLengthBytes)
  {
    returnValue = actRSA_PARAM_OUT_OF_RANGE;
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actRSAInitExponentiation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actRSAInitExponentiation(
  VSECPRIM_P2CONST_PARA(actU8) modulus,
  actLengthType modulus_len,
  VSECPRIM_P2CONST_PARA(actU8) exponent,
  actLengthType exponent_len,
  VSECPRIM_P2VAR_PARA(actRSAPRIMSTRUCT) wksp,
  actU16 key_flag,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE returnValue = actOK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  VSECPRIM_P2VAR_PARA(actBNRING) n_ring = &(wksp->wsRSARing);
  actLengthType n_bytes = modulus_len;
  actLengthType n_length = actBNGetDigitLength(n_bytes);
  actLengthType tmp_len;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) tmp;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) e;

  returnValue = actRSAInitExpCheckInputParameters(modulus_len, exponent_len, n_length, key_flag);

  if (returnValue == actOK)
  {
    actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

    /* initialize n_ring structure */
    n_ring->m = wksp->modulusBuffer; /* SBSW_VSECPRIM_WRITE_POINTER */
    n_ring->RR = wksp->RRBuffer; /* SBSW_VSECPRIM_WRITE_POINTER */
    n_ring->m_length = n_length; /* SBSW_VSECPRIM_WRITE_POINTER */
    n_ring->m_byte_length = n_bytes; /* SBSW_VSECPRIM_WRITE_POINTER */
    actBNSetOctetStringROM(n_ring->m, n_length, modulus, n_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

# if (actBN_MONT_MUL_VERSION == 1) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    n_ring->prime_structure = 0u;
# endif
# if (actBN_MOD_EXP_WINDOW_SIZE == 0)   /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    /* variable k-bit window algorithm */
    n_ring->window_size = actRSA_MOD_EXP_WINDOW_SIZE;
# endif

    actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

    /* calculate Montgomery constants R^2 mod m and m_bar and store them in n_ring structure */
    tmp = wksp->tmp.tmpInit;
    /* n_length is multiplied by 2, since we want to compute R^2 (and not only R) */
    tmp_len = (2u * n_length) + 1u;
    actBNSetZero(tmp, tmp_len); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    tmp[tmp_len - 1u] = 1u;  /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
    /* tmp has value 2^(32 * 2 * length of modulus in actBNDIGITs) */

    actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

    /* calculate R^2 mod m = tmp mod m */
    returnValue = actBNReduce(tmp, tmp_len, n_ring->m, n_length, wksp->RRBuffer, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

    if ( returnValue != actOK)
    {
      /* n_ring->m = 0, i.e., modulus too small */
      returnValue = actRSA_PARAM_OUT_OF_RANGE;
    }
    else /* returnValue == actOK */
    {
      actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

      /* calculate n_bar = -(m^-1) mod 2^bits_per_digit */
      actBNOddInvModBase(&(n_ring->m_bar), &((n_ring->m)[0])); /* SBSW_VSECPRIM_CALL_FUNCTION */

      /* assign workspace for exponent e */
      e = wksp->exponent;
      actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

      /* the parameter exponent is an actU8-buffer; transform this into an actBNDIGIT representation and store it in e */
      actBNSetOctetStringROM(e, n_length, exponent, exponent_len); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

      returnValue = actRSAInitExpCheckExponent(key_flag, e, wksp->modulusBuffer, n_length, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
    }
  }
  return returnValue;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actRSAExponentiation()
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
VSECPRIM_FUNC(actRETURNCODE) actRSAExponentiation(
  VSECPRIM_P2CONST_PARA(actU8) cipher,
  actLengthType cipher_len,
  VSECPRIM_P2VAR_PARA(actU8) message,
  VSECPRIM_P2VAR_PARA(actLengthType) message_len,
  VSECPRIM_P2VAR_PARA(actRSAPRIMSTRUCT) wksp,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE returnValue = actOK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  VSECPRIM_P2CONST_PARA(actBNRING) n_ring = &(wksp->wsRSARing);
  actLengthType n_length = n_ring->m_length;
  actLengthType n_bytes = n_ring->m_byte_length;

  VSECPRIM_P2VAR_PARA(actBNDIGIT) e;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) c;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) m;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) t[actRSA_MOD_EXP_TMP];
  actU8 i;

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* Basic input parameter checks */
  returnValue = actRSAExpInputCheck(message, message_len, cipher_len, n_bytes); /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */

  if (returnValue == actOK)
  {
    /* assign temporary bignum workspace, size has already been checked by init function,
       space for m, R^2 and e has been initialized */
    e = wksp->exponent;
    c = wksp->tmp.cipherAndMsg.cipher;
    m = wksp->tmp.cipherAndMsg.message;

    for (i = 0; i < actRSA_MOD_EXP_TMP; ++i) /* actRSA_MOD_EXP_TMP = 1 - for greater values the code does not work! */
    {
      t[i] = wksp->tmpExp; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
    }

    actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

    /* store cipher in c
       the parameter cipher is an actU8-buffer; transform this into an actBNDIGIT representation and store it in c */
    actBNSetOctetString(c, n_length, cipher, cipher_len); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

    /* check 0 <= c < modulus */
    if (actBNCompare(c, n_ring->m, n_length) >= actCOMPARE_EQUAL) /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    {
      returnValue = actRSA_PARAM_OUT_OF_RANGE;
    }

    /* Main part starts here. */
    if (returnValue == actOK)
    {
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

      /* convert c to Montgomery representation cR, store result in c
         actBNMontMulCopy computes c = c*RR*R^(-1) mod modulus = c*R^2*R^(-1) mod modulus = c*R mod modulus */
      actBNMontMulCopy(c, n_ring->RR, t[0], n_ring, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

      /* calculate cR^e mod n_ring->m, store the result in m (message buffer to store the outcome)
         Note: The result m is the Montgomery representation mR of the actual power c^e and needs to be transformed back! */
      actBNModExp(c, e, m, n_ring, t, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

      /* reconvert m from Montgomery representation mR by calling actBNMontMulCopy(m,one,...),
         which computes mR*1*R^(-1) mod modulus and stores the result in m */
      {
        /* store 1-Element of the field in e's memory space, this also wipes the exponent */
        VSECPRIM_P2VAR_PARA(actBNDIGIT) one = e;
        actBNSetZero(one, n_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        one[0] = (actBNDIGIT)1; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
        actBNMontMulCopy(m, one, t[0], n_ring, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

      /* write m to message buffer */
      if (message != NULL_PTR)
      {
        /* the parameter message is an actU8 - buffer; transform m from its actBNDIGIT representation
           to actU8 representation and store it in message */
        actBNOctetString(message, n_bytes, m, n_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        *message_len = n_bytes; /* SBSW_VSECPRIM_WRITE_POINTER */
      }
      actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
    }

  }
  return returnValue;
} /* PRQA S 6060, 6050 */ /* MD_VSECPRIM_STPAR, MD_MSR_STCAL */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTIRSAEXP_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: actIRSAExp.c
 *********************************************************************************************************************/
