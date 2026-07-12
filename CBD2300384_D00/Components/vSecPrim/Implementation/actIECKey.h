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
/*!        \file  actIECKey.h
 *        \brief  An ECC key generation interface.
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

#ifndef ACTIECKEY_H
# define ACTIECKEY_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actITypes.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C" {
# endif

/**********************************************************************************************************************
 *  actECGetPrivateKeyLength()
 *********************************************************************************************************************/
/*! \brief          This function returns the key length in bytes for the desired curve.
 *  \details        -
 *  \param[in]      domain       domain parameter
 *  \return         length of key in bytes
 *                  (0, if domain not valid)
 *  \pre            domain must be a valid pointer
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actLengthType) actECGetPrivateKeyLength(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain);

/**********************************************************************************************************************
 *  actECGetPublicKeyCompLength()
 *********************************************************************************************************************/
/*! \brief          This function returns the key length in bytes for the desired curve.
 *  \details        -
 *  \param[in]      domain       domain parameter
 *  \return         length of key in bytes
 *                  (0, if domain not valid)
 *  \pre            domain must be a valid pointer
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actLengthType) actECGetPublicKeyCompLength(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain);

/**********************************************************************************************************************
 *  actECInitGenerateKeyPair()
 *********************************************************************************************************************/
/*! \brief          This function initializes the ECC workspace.
 *  \details        -
 *  \param[in]      domain       domain parameter
 *  \param[in]      domain_ext   domain parameter extensions (Montgomery constants etc.)
 *  \param[in]      speedup_ext  (optional) precomputations (for ECDSA-Sign, -GenKey)
 *  \param[in,out]  wksp         (in) workspace
 *                               (out) initialized workspace
 *  \param[in]      wksp_len     length of workspace in bytes
 *  \return         actEXCEPTION_DOMAIN       domain decoding error
 *                  actEXCEPTION_DOMAIN_EXT   domain_ext decoding error
 *                  actEXCEPTION_SPEEDUP_EXT  speedup_ext decoding error
 *                  actEXCEPTION_MEMORY       wksp_len to small
 *                  actOK                     else
 *  \pre            All provided pointers must be valid.
 *                  domain, domain_ext and speedup_ext have to be valid ASN1 structures for EC domain parameters.
 *                  The addresses of domain, domain_ext and speedup_ext have to be valid until returning from
 *                  actECGenerateKeyPair(), because the pointers to the data stored in domain, domain_ext and speedup_ext
 *                  are stored in the workspace. These pointers are used as long as the workspace is used.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECInitGenerateKeyPair(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
  VSECPRIM_P2ROMCONST_PARA(actU8) domain_ext,
  VSECPRIM_P2ROMCONST_PARA(actU8) speedup_ext,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp,
  actLengthType wksp_len);

/**********************************************************************************************************************
 *  actECGenerateKeyPair()
 *********************************************************************************************************************/
/*! \brief          This function generate an ECC key pair (d, Q), where Q = d * G.
 *  \details        -
 *  \param[in]      generate_privatekey flag which indicates if the private key is passed by the user (false) or
 *                                      shall be generated at random (true)
 *  \param[in,out]  privatekey   (in)  if generate_privatekey == false, privatekey is passed by the user
 *                                     if generate_privatekey == true, privatekey is a buffer to store the
                                       private key d which will be generated at random
                                 (out) private key d
 *  \param[in,out]  publickey_x  (in) buffer to store x-coordinate of the public key Q
 *                               (out) x-coordinate of the public key Q
 *  \param[in,out]  publickey_y  (in) buffer to store y-coordinate of the public key Q
 *                               (out) y-coordinate of the public key Q
 *  \param[in,out]  wksp         (in)  workspace
 *                               (out) workspace
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \return         actEXCEPTION_PRIVKEY    if d is passed and (d==0||d>=n)
 *                  actEXCEPTION_UNKNOWN    internal error (result point not on curve)
 *                  actOK                   else
 *  \pre            All provided pointers must be valid.
 *                  wksp is initialized with actECInitGenerateKeyPair, i.e., actECIniGenerateKeyPair is called before
 *                  The buffer referenced by privatekey must provide at least n_bytes bytes.
 *                  The buffers referenced by publickey_x and publickey_y must provide at least p_bytes bytes each.
 *                  The buffer referenced by wksp->var1 must provide at least n_length actBNDIGITs.
 *                  The buffers referenced by wksp->point0_x, wksp->point0_y and wksp->point0_z must
 *                  provide at least p_length actBNDIGITs each.
 *                  Note: p_length, p_bytes, n_length and n_length are stored in wksp->Curve.p_field.m_length,
 *                  wksp->Curve.p_field.m_byte_length, wksp->Curve.n_field.m_length and
 *                  wksp->Curve.n_field.m_byte_length, respectively.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECGenerateKeyPair(
  boolean generate_privatekey,
  VSECPRIM_P2VAR_PARA(actU8) privatekey,
  VSECPRIM_P2VAR_PARA(actU8) publickey_x,
  VSECPRIM_P2VAR_PARA(actU8) publickey_y,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif
# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTIECKEY_H */

/**********************************************************************************************************************
 *  END OF FILE: actIECKey.h
 *********************************************************************************************************************/
