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
/*!        \file  actIRSAExp.h
 *        \brief  This file contains an RSA exponentiation interface where the exponent can be the public or as well
 *                the private exponent of the RSA scheme.
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

#ifndef ACTIRSAEXP_H
# define ACTIRSAEXP_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actITypes.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define actRSA_PRIVATE_KEY_OPERATION                                 (0u)
# define actRSA_PRIVATE_KEY_OPERATION_CRT                             (1u)
# define actRSA_PUBLIC_KEY_OPERATION                                  (2u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C" {
# endif

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actRSAInitExponentiation()
 *********************************************************************************************************************/
/*! \brief       Initializes the RSA workspace.
 *  \details     This function initializes the RSA workspace wksp by updating the members of wksp with modulus and
 *               exponent information.
 *  \param[in]     modulus        pointer to the public modulus
 *  \param[in]     modulus_len    length of modulus
 *  \param[in]     exponent       pointer to the exponent (private or public)
 *  \param[in]     exponent_len   length of exponent
 *  \param[in,out] wksp           (in)  pointer to provided workspace
 *                                (out) pointer to provided workspace, updated entries
 *  \param[in]     key_flag       actRSA_PRIVATE(PUBLIC)_KEY_OPERATION(_CRT)
 *  \param[in]     watchdog       watchdog function pointer
 *  \return    actEXCEPTION_MEMORY         wksp_len to small
 *  \return    actRSA_PARAM_OUT_OF_RANGE   n has leading zero digits
 *  \return    actEXCEPTION_PRIVKEY        exponent==0 || exponent>=n and key_flag = actRSA_PRIVATE_KEY_OPERATION
 *  \return    actEXCEPTION_PUBKEY         exponent==0 || exponent>=n and key_flag = actRSA_PUBLIC_KEY_OPERATION
 *  \return    actOK                   else
 *  \pre         modulus, exponent and wksp must be valid pointers
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actRSAInitExponentiation(
  VSECPRIM_P2CONST_PARA(actU8) modulus,
  actLengthType modulus_len,
  VSECPRIM_P2CONST_PARA(actU8) exponent,
  actLengthType exponent_len,
  VSECPRIM_P2VAR_PARA(actRSAPRIMSTRUCT) wksp,
  actU16 key_flag,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actRSAExponentiation()
 *********************************************************************************************************************/
/*! \brief       Computes message = cipher^(private exponent) mod modulus
 *  \details     This function computes the power cipher^(exponent) mod modulus, where exponent is either the
 *               public or the private exponent of the RSA scheme and stores the result in the buffer provided
 *               by message.
 *  \param[in]     cipher       pointer to the cipher c to be decrypted
 *  \param[in]     cipher_len   length of cipher
 *  \param[in,out] message      (in)  buffer to store the decrypted cipher text message m
 *                              (out) the decrypted cipher text message
 *  \param[in,out] message_len  (in)  length of message buffer
 *                              (out) length of message buffer used
 *  \param[in,out] wksp         (in)  pointer to provided workspace
 *                              (out) pointer to provided workspace
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
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actRSAExponentiation(
  VSECPRIM_P2CONST_PARA(actU8) cipher,
  actLengthType cipher_len,
  VSECPRIM_P2VAR_PARA(actU8) message,
  VSECPRIM_P2VAR_PARA(actLengthType) message_len,
  VSECPRIM_P2VAR_PARA(actRSAPRIMSTRUCT) wksp,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

#endif /* ACTIRSAEXP_H */
/**********************************************************************************************************************
 *  END OF FILE: actIRSAExp.h
 *********************************************************************************************************************/
