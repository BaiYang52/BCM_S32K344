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
/*!        \file  actIRSA.h
 *        \brief  This file contains the interface for RSA primitives, divided into operations with respect to the
 *                public and the private key.
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

#ifndef ACTIRSA_H
# define ACTIRSA_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actITypes.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* an input parameter x does not satisfy 0 <= x < n */
# define actRSA_PARAM_OUT_OF_RANGE                                    (0x90u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C" {
# endif

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actRSAInitPublicKeyOperation()
 *********************************************************************************************************************/
/*! \brief       Initializes the RSA workspace.
 *  \details     This function initializes the RSA workspace wksp by updating the members of wksp with modulus and
 *               public exponent information.
 *  \param[in]     modulus             pointer to the public modulus
 *  \param[in]     modulus_len         length of modulus
 *  \param[in]     public_exponent     pointer to public exponent e
 *  \param[in]     public_exponent_len length of public exponent e
 *  \param[in,out] wkspBuf             (in)  pointer to provided workspace
 *                                     (out) pointer to provided workspace, updated entries
 *  \param[in]     watchdog            watchdog function pointer
 *  \return        actEXCEPTION_MEMORY         wksp_len to small
 *  \return        actRSA_PARAM_OUT_OF_RANGE   n has leading zero digits
 *  \return        actEXCEPTION_PUBKEY         e==0 || e>=modulus
 *  \return        actOK                       else
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         modulus, public_exponent and wkspBuf must be valid pointers
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actRSAInitPublicKeyOperation(
  VSECPRIM_P2CONST_PARA(actU8) modulus,
  actLengthType modulus_len,
  VSECPRIM_P2CONST_PARA(actU8) public_exponent,
  actLengthType public_exponent_len,
  VSECPRIM_P2VAR_PARA(actRSAPRIMSTRUCT) wkspBuf,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actRSAPublicKeyOperation()
 *********************************************************************************************************************/
/*! \brief       Computes cipher = message^(public exponent) mod modulus
 *  \details     This function computes the power message^(public exponent) mod modulus, where public exponent is the
 *               public exponent of the RSA scheme and stores the result in the buffer provided by cipher.
 *  \param[in]     message      pointer to the message m to be encrypted
 *  \param[in]     message_len  length of message
 *  \param[in,out] cipher       (in)  buffer to store the cipher text cipher c
 *                              (out) the encrypted message
 *  \param[in,out] cipher_len   (in)  length of cipher buffer
 *                              (out) length of cipher buffer used
 *  \param[in,out] wkspBuf      (in)  pointer to provided workspace
 *                              (out) pointer to provided workspace
 *  \param[in]     watchdog     watchdog function pointer
 *  \return      actEXCEPTION_MEMORY        *cipher_len < n_bytes
 *  \return      actRSA_PARAM_OUT_OF_RANGE  message >= modulus
 *  \return      actOK                      else
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         message and wkspBuf must be valid pointers
 *               If cipher is not a null pointer, the buffer provided by cipher must be long enough to store
 *               n_bytes elements, where n_bytes is the length of the modulus in bytes
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actRSAPublicKeyOperation(
  VSECPRIM_P2CONST_PARA(actU8) message,
  actLengthType message_len,
  VSECPRIM_P2VAR_PARA(actU8) cipher,
  VSECPRIM_P2VAR_PARA(actLengthType) cipher_len,
  VSECPRIM_P2VAR_PARA(actRSAPRIMSTRUCT) wkspBuf,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actRSAPublicKeyGetBitLength()
 *********************************************************************************************************************/
/*! \brief       This function returns the exact length in bits of the modulus.
 *  \details     -
 *  \param[in]   n_ring Pointer to the ring structure containing the modulus.
 *  \return      Exact length in bits of the modulus
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         n_ring must be a valid pointer
 *********************************************************************************************************************/
VSECPRIM_FUNC(actLengthType) actRSAPublicKeyGetBitLength(
  VSECPRIM_P2CONST_PARA(actBNRING) n_ring);

/**********************************************************************************************************************
 *  actRSAInitPrivateKeyOperation()
 *********************************************************************************************************************/
/*! \brief       Initializes the RSA workspace.
 *  \details     This function initializes the RSA workspace wksp by updating the members of wksp with modulus and
 *               privateexponent information.
 *  \param[in]     modulus              pointer to the public modulus
 *  \param[in]     modulus_len          length of modulus
 *  \param[in]     private_exponent     pointer to private exponent d
 *  \param[in]     private_exponent_len length of private exponent d
 *  \param[in,out] wksp                 (in)  pointer to provided workspace
 *                                      (out) pointer to provided workspace, updated entries
 *  \param[in]     watchdog             watchdog function pointer
 *  \return        actEXCEPTION_MEMORY         wksp_len to small
 *  \return        actRSA_PARAM_OUT_OF_RANGE   n has leading zero digits
 *  \return        actEXCEPTION_PUBKEY         d==0 || d>=modulus
 *  \return        actOK                       else
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         modulus, private_exponent and wkspBuf must be valid pointers
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actRSAInitPrivateKeyOperation(
  VSECPRIM_P2CONST_PARA(actU8) modulus,
  actLengthType modulus_len,
  VSECPRIM_P2CONST_PARA(actU8) private_exponent,
  actLengthType private_exponent_len,
  VSECPRIM_P2VAR_PARA(actRSAPRIMSTRUCT) wkspBuf,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actRSAPrivateKeyOperation()
 *********************************************************************************************************************/
/*! \brief       Computes message = cipher^(private exponent) mod modulus
 *  \details     This function computes the power cipher^(private exponent) mod modulus, where private exponent is the
 *               private exponent of the RSA scheme and stores the result in the buffer provided by message.
 *  \param[in]     cipher       pointer to the cipher c to be decrypted
 *  \param[in]     cipher_len   length of cipher
 *  \param[in,out] message      (in)  buffer to store the decrypted cipher text message m
 *                              (out) the decrypted cipher text message
 *  \param[in,out] message_len  (in)  length of message buffer
 *                              (out) length of message buffer used
 *  \param[in,out] wkspBuf      (in)  pointer to provided workspace
 *                              (out) pointer to provided workspace
 *  \param[in]     watchdog     watchdog function pointer
 *  \return      actEXCEPTION_MEMORY        *message_len < n_bytes
 *  \return      actRSA_PARAM_OUT_OF_RANGE  cipher >= modulus
 *  \return      actOK                      else
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         cipher and wkspBuf must be valid pointers
 *               If message is not a null pointer, the buffer provided by message must be long enough to store
 *               n_bytes elements, where n_bytes is the length of the modulus in bytes
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actRSAPrivateKeyOperation(
  VSECPRIM_P2CONST_PARA(actU8) cipher,
  actLengthType cipher_len,
  VSECPRIM_P2VAR_PARA(actU8) message,
  VSECPRIM_P2VAR_PARA(actLengthType) message_len,
  VSECPRIM_P2VAR_PARA(actRSAPRIMSTRUCT) wkspBuf,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actRSAPrivateKeyGetBitLength()
 *********************************************************************************************************************/
/*! \brief       This function returns the exact length in bits of the modulus.
 *  \details     -
 *  \param[in]   n_ring Pointer to the ring structure containing the modulus.
 *  \return      Exact length in bits of the modulus
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         n_ring must be a valid pointer
 *********************************************************************************************************************/
VSECPRIM_FUNC(actLengthType) actRSAPrivateKeyGetBitLength(
  VSECPRIM_P2CONST_PARA(actBNRING) n_ring);

# if (VSECPRIM_ACTIRSAPRIVATECRT_ENABLED == STD_ON)  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/**********************************************************************************************************************
 *  actRSAInitPrivateKeyOperationCRT()
 *********************************************************************************************************************/
/*! \brief      Initializes the RSA CRT workspace.
 *  \details    This function initializes the RSA CRT workspace wksp by updating the members of wksp with modulus
 *              information and information about the primes p and q.
 *  \param[in]     prime_p                  pointer to first prime of primes p,q with p*q = n (n is the ring module)
 *  \param[in]     prime_p_len              length of prime p
 *  \param[in]     prime_q                  pointer to second prime of primes p,q with p*q = n (n is the ring module)
 *  \param[in]     prime_q_len              length of prime q
 *  \param[in]     private_exponent_dp      pointer to dp = d mod (p-1), where d is the private exponent
 *  \param[in]     private_exponent_dp_len  length of dp
 *  \param[in]     private_exponent_dq      pointer to dq = d mod (q-1), where d is the private exponent
 *  \param[in]     private_exponent_dq_len  length of dq
 *  \param[in]     q_inverse_mod_p          pointer to inverse of q mod p
 *  \param[in]     q_inverse_mod_p_len      length of q_inverse_mod_p
 *  \param[in,out] wsCRT        (in)  pointer to provided workspace, providing memory for auxiliary calculations and
                                      for storing the modulus
 *                              (out) pointer to provided workspace
 *  \param[in]     watchdog     watchdog function pointer
 *  \return        actEXCEPTION_MEMORY   wksp_len to small
 *  \return        actEXCEPTION_PRIVKEY  0 < qInv < p is violated or 0 < dp < (p-1)  is violated or 0 < dq < (q-1)  is violated
 *  \return        actOK                 else
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         wksp must be a valid pointer
 *               the CRTKey (p, q, dp, dq, qInv) must stay in memory after the call of this function
 *********************************************************************************************************************/
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
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actRSAPrivateKeyOperationCRT()
 *********************************************************************************************************************/
/*! \brief       Computes message = cipher^(private exponent) mod modulus using the chinese remainder theorem
 *  \details     This function computes the power cipher^(private exponent) mod modulus, where private exponent is the
 *               private exponent of the RSA scheme and stores the result in the buffer provided by message.
 *  \param[in]     cipher       pointer to the cipher c to be decrypted
 *  \param[in]     cipher_len   length of cipher
 *  \param[in,out] message      (in)  buffer to store the decrypted cipher text message m
 *                              (out) the decrypted cipher text message
 *  \param[in,out] message_len  (in)  length of message buffer
 *                              (out) length of message buffer used
 *  \param[in,out] wsCRT        (in)  pointer to workspace used for CRT computations
 *                              (out) pointer to workspace used for CRT computations
 *  \param[in]     watchdog     watchdog function pointer
 *  \return      actEXCEPTION_MEMORY        *message_len < n_bytes
 *  \return      actRSA_PARAM_OUT_OF_RANGE  cipher >= modulus
 *  \return      actOK                      else
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         Provided pointers must be valid
 *               Buffer provided by message must be long enough to store n_bytes elements, where n_bytes is the
 *               length of the modulus in bytes
 *               wsCRT and wsCRTBuf are initialized by actRSAInitPrivateKeyOperationCRT()
 *               the CRTKey (p, q, dp, dq, qInv) must stay in memory after the call of the init function
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actRSAPrivateKeyOperationCRT(
  VSECPRIM_P2CONST_PARA(actU8) cipher,
  actLengthType cipher_len,
  VSECPRIM_P2VAR_PARA(actU8) message,
  VSECPRIM_P2VAR_PARA(actLengthType) message_len,
  VSECPRIM_P2VAR_PARA(actRSACRTSTRUCT) wsCRT,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# endif /* (VSECPRIM_ACTIRSAPRIVATECRT_ENABLED == STD_ON) */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

#endif /* ACTIRSA_H */
/**********************************************************************************************************************
 *  END OF FILE: actIRSA.h
 *********************************************************************************************************************/
