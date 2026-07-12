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
/*!        \file  actIECDSA.h
 *        \brief  Interface for ECDSA verification.
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

#ifndef ACTIECDSA_H
# define ACTIECDSA_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actITypes.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* signature not validated */
# define actSIGNATURE_INVALID                                         (0x80u)
/* signature (r, s) does not satisfiy 0 < r, s < n  (n = basepoint order) */
# define actSIGNATURE_OUT_OF_RANGE                                    (0x81u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/**********************************************************************************************************************
 *  actECDSAGetSignatureCompLength()
 *********************************************************************************************************************/
/*! \brief       This function returns the signature component length in bytes for the desired curve.
 *  \details     -
 *  \param[in]   domain domain parameter
 *  \return      length of each signature component (r, s) in bytes (0, if domain not valid)
 *  \pre         domain must be a valid pointer
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actLengthType) actECDSAGetSignatureCompLength(VSECPRIM_P2ROMCONST_PARA(actU8) domain);

/**********************************************************************************************************************
 *  actECDSAInitVerify()
 *********************************************************************************************************************/
/*! \brief       This function initializes the ECC workspace.
 *  \details     -
 *  \param[in]       domain      domain parameter
 *  \param[in]       domain_ext  domain parameter extensions (Montgomery constants etc.)
 *  \param[in,out]   wksp        (in) workspace
 *                                (out) initialized workspace
 *  \param[in]       wksp_len    length of workspace in bytes
 *  \return      actEXCEPTION_DOMAIN       domain decoding error
 *               actEXCEPTION_DOMAIN_EXT   domain_ext decoding error
 *               actEXCEPTION_MEMORY       wksp_len to small
 *               actOK                     else
 *  \pre         all provided pointers must be valid
 *               domain and domain_ext have to be valid ASN1 structures for EC domain parameters.
 *               The addresses of domain and domain_ext have to be valid until returning from
 *               actECDSAVerify(), because the pointers to the data stored in domain and domain_ext
 *               are stored in the workspace. These pointers are used as long as the workspace is used.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECDSAInitVerify(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
  VSECPRIM_P2ROMCONST_PARA(actU8) domain_ext,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp,
  actLengthType wksp_len);

/**********************************************************************************************************************
 *  actECDSAVerify()
 *********************************************************************************************************************/
/*! \brief       This function verifies a signature of a message value.
 *  \details     -
 *  \param[in]     message      pointer to message that has been signed
 *  \param[in]     message_len  length of message in bytes
 *  \param[in]     signature_r  pointer to the signature component r of (r, s)
 *  \param[in]     signature_r_ len length of signature_r
 *  \param[in]     signature_s  pointer to the signature component s of (r, s)
 *  \param[in]     signature_s_ len length of signature_s
 *  \param[in]     publickey_x  x-coordinate of the public key Q of the key pair (d, Q)
 *  \param[in]     publickey_y  y-coordinate of the public key Q of the key pair (d, Q)
 *  \param[in,out] wksp         (in) workspace
 *                              (out) workspace
 *  \param[in]     watchdog     Watchdog trigger function pointer
 *  \return      actEXCEPTION_NULL          if an input parameter is NULL
 *               actEXCEPTION_LENGTH        wrong message_len (in bits)
 *               actEXCEPTION_PUBKEY        if Q is invalid
 *               actSIGNATURE_OUT_OF_RANGE  if (r==0||s==0||r>=n||s>=n)
 *               actSIGNATURE_INVALID       if the signature is invalid
 *               actEXCEPTION_UNKNOWN       internal error (result point
 *                                          not on curve)
 *               actOK                      else
 *  \pre         all provided pointers must be valid
 *               wksp is initialized with actECDSAInitVerify, i.e., actECDSAInitVerify is called before
 *               The buffers provided by publickey_x and publickey_y must provide p_bytes bytes each.
 *               The buffers referenced by wksp->hashed_message, wksp->signature_r and wksp->signature_s must provide
 *               at least n_length actBNDIGITs.
 *               The buffers referenced by wksp->public_key_x and wksp->public_key_y must provide at least p_length
 *               actBNDIGITs.
 *               Note: p_bytes, p_length and n_length are stored in wksp->Curve.p_field.m_byte_length,
 *               wksp->Curve.p_field.m_length and wksp->Curve.n_field.m_length, respectively.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECDSAVerify(
  VSECPRIM_P2CONST_PARA(actU8) message,
  actLengthType message_len,
  VSECPRIM_P2CONST_PARA(actU8) signature_r,
  actLengthType signature_r_len,
  VSECPRIM_P2CONST_PARA(actU8) signature_s,
  actLengthType signature_s_len,
  VSECPRIM_P2CONST_PARA(actU8) publickey_x,
  VSECPRIM_P2CONST_PARA(actU8) publickey_y,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actECDSAInitSign()
 *********************************************************************************************************************/
/*! \brief       This function initializes the ECC workspace.
 *  \details     -
 *  \param[in]     domain       domain parameter
 *  \param[in]     domain_ext   domain parameter extensions (Montgomery constants etc.)
 *  \param[in]     speedup_ext  (optional) precomputations (for ECDSA-Sign, -GenKey)
 *  \param[in,out] wksp         (in) workspace
 *                              (out) initialized workspace
 *  \param[in]     wksp_len     length of workspace in bytes
 *  \return      actEXCEPTION_DOMAIN       domain decoding error
 *               actEXCEPTION_DOMAIN_EXT   domain_ext decoding error
 *               actEXCEPTION_SPEEDUP_EXT  speedup_ext decoding error
 *               actEXCEPTION_MEMORY       wksp_len to small
 *               actOK                     else
 *  \pre         all provided pointers must be valid
 *               domain, domain_ext and speedup_ext have to be valid ASN1 structures for EC domain parameters.
 *               The addresses of domain, domain_ext and speedup_ext have to be valid until returning from
 *               actECDSASign(), because the pointers to the data stored in domain, domain_ext and speedup_ext
 *               are stored in the workspace. These pointers are used as long as the workspace is used.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECDSAInitSign(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
  VSECPRIM_P2ROMCONST_PARA(actU8) domain_ext,
  VSECPRIM_P2ROMCONST_PARA(actU8) speedup_ext,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp,
  actLengthType wksp_len);

/**********************************************************************************************************************
 *  actECDSASign()
 *********************************************************************************************************************/
/*! \brief       This function signs the message value under usage of the private key.
 *  \details     -
 *  \param[in]     message           pointer to message to be signed
 *  \param[in]     message_len       length of message in bytes
 *  \param[in]     privatekey        the private key d of the key pair (d, Q)
 *  \param[in,out] signature_r       (in) buffer to store the signature component r of (r, s)
 *                                   (out) signature component r of (r, s)
 *  \param[in,out] signature_r_len   (in) length of signature_r buffer
 *                                   (out) length of signature_r
 *  \param[in,out] signature_s       (in) buffer to store the signature component s of (r, s)
 *                                   (out) signature component s of (r, s)
 *  \param[in,out] signature_s_len   (in) length of signature_s buffer
 *                                   (out) length of signature_s
 *  \param[in,out] wksp              (in) workspace
 *                                   (out) workspace
 *  \param[in]    watchdog           Watchdog trigger function pointer
 *  \return       actEXCEPTION_NULL       if an input parameter is NULL
 *                actEXCEPTION_MEMORY     signature buffer to small
 *                actEXCEPTION_LENGTH     wrong message_len (in bits)
 *                actEXCEPTION_PRIVKEY    if (d==0||d>=n)
 *                actEXCEPTION_UNKNOWN    internal error (result point not on curve)
 *                actOK                   else
 *  \pre         all provided pointers must be valid
 *               wksp is initialized with actECDSAInitSign, i.e., actECDSAInitSign is called before
 *               The buffer referenced by privatekey must be at least n_bytes bytes long.
 *               The buffer referenced by signature_r must be at least signature_r_len bytes long
 *               The buffer referenced by signature_s must be at least signature_s_len bytes long
 *               The buffers referenced by wksp->hashed_message, wksp->var1, wksp->signature_r and
 *               wksp->signature_s must provide at least n_length actBNDIGITs.
 *               Note: n_bytes and n_length are stored in wksp->Curve.n_field.m_byte_length and
 *               wksp->Curve.n_field.m_length, respectively.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECDSASign(
  VSECPRIM_P2CONST_PARA(actU8) message,
  actLengthType message_len,
  VSECPRIM_P2CONST_PARA(actU8) privatekey,
  VSECPRIM_P2VAR_PARA(actU8) signature_r,
  VSECPRIM_P2VAR_PARA(actLengthType) signature_r_len,
  VSECPRIM_P2VAR_PARA(actU8) signature_s,
  VSECPRIM_P2VAR_PARA(actLengthType) signature_s_len,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
}                               /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTIECDSA_H */

/**********************************************************************************************************************
 *  END OF FILE: actIECDSA.h
 *********************************************************************************************************************/
