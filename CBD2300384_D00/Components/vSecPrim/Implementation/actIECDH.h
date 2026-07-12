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
/*!        \file  actIECDH.h
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

#ifndef ACTIECDH_H
# define ACTIECDH_H

# include "actITypes.h"

/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/**********************************************************************************************************************
 *  actECDSAGetSecretCompLength()
 *********************************************************************************************************************/
/*! \brief          This function returns the secret component length in bytes
 *                  for the desired curve.
 *  \details        -
 *  \param[in]      domain       domain parameter
 *  \return         length of each secret component (x, y) in bytes
 *                  (0, if domain not valid)
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/

  VSECPRIM_FUNC(actLengthType) actECDSAGetSecretCompLength(VSECPRIM_P2ROMCONST_PARA(actU8) domain);

/**********************************************************************************************************************
 *  actECDHInitGetSecret()
 *********************************************************************************************************************/
/*! \brief          This function initializes the ECC workspace.
 *  \details        -
 *  \param[in]      domain       domain parameter
 *  \param[in]      domain_ext   domain parameter extensions (Montgomery constants etc.)
 *  \param[in,out]  wksp         (in) workspace
 *                               (out) initialized workspace
 *  \param[in]      wksp_len     length of workspace in bytes
 *  \return         actEXCEPTION_NULL         if an input parameter is NULL
 *                  actEXCEPTION_DOMAIN       domain decoding error
 *                  actEXCEPTION_DOMAIN_EXT   domain_ext decoding error
 *                  actEXCEPTION_MEMORY       wksp_len to small
 *                  actOK                     else
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECDHInitGetSecret(VSECPRIM_P2ROMCONST_PARA(actU8) domain, VSECPRIM_P2ROMCONST_PARA(actU8) domain_ext, VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp, actLengthType wksp_len);

/**********************************************************************************************************************
 *  actECDHGetSecret()
 *********************************************************************************************************************/
/*! \brief          This function calculates the common secret d1 * Q2 (= d1 * d2 * G)
 *  \details        -
 *  \param[in]      privatekey   the private key d1 of the key pair (d1, Q1)
 *  \param[in]      publickey_x  x-coordinate of the public key Q2 of the key pair (d2, Q2)
 *  \param[in]      publickey_y  y-coordinate of the public key Q2 of the key pair (d2, Q2)
 *  \param[in,out]  secret_x     (in) buffer to store x-coordinate of the secret d1 * Q2
 *                               (out) x-coordinate of the secret d1 * Q2
 *  \param[in,out]  secret_y     (in) buffer to store y-coordinate of the secret d1 * Q2
 *                               (optional secret_y maybe NULL)
 *                               (out) y-coordinate of the secret d1 * Q2
 *  \param[in,out]  wksp         (in) workspace
 *                               (out) initialized workspace
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \return         actEXCEPTION_NULL       if an input parameter is NULL
 *                  actEXCEPTION_PRIVKEY    if (d==0||d>=n)
 *                  actEXCEPTION_PUBKEY     if Q2 is invalid
 *                  actEXCEPTION_UNKNOWN    internal error (result point
 *                  not on curve)
 *                  actOK                   else
 *  \pre            wksp has been initialized with actECDHInitGetSecret
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECDHGetSecret(VSECPRIM_P2CONST_PARA(actU8) privatekey,
                                              VSECPRIM_P2CONST_PARA(actU8) publickey_x, VSECPRIM_P2CONST_PARA(actU8) publickey_y,
                                              VSECPRIM_P2VAR_PARA(actU8) secret_x, VSECPRIM_P2VAR_PARA(actU8) secret_y, VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp,
                                              VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actECDHInitKeyDerive()
 *********************************************************************************************************************/
/*! \brief          This function initializes the ECC workspace.
 *  \details        -
 *  \param[in]      domain       domain parameter
 *  \param[in]      domain_ext   domain parameter extensions (Montgomery constants etc.)
 *  \param[in,out]  wksp         (in) workspace
 *                               (out) initialized workspace
 *  \param[in]      wksp_len     length of workspace in bytes
 *  \return         actEXCEPTION_NULL         if an input parameter is NULL
 *                  actEXCEPTION_DOMAIN       domain decoding error
 *                  actEXCEPTION_DOMAIN_EXT   domain_ext decoding error
 *                  actEXCEPTION_MEMORY       wksp_len to small
 *                  actOK                     else
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/

    VSECPRIM_FUNC(actRETURNCODE) actECDHInitKeyDerive(VSECPRIM_P2ROMCONST_PARA(actU8) domain, VSECPRIM_P2ROMCONST_PARA(actU8) domain_ext, VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp, actLengthType wksp_len);

/**********************************************************************************************************************
 *  actECDHKeyDerive()
 *********************************************************************************************************************/
/*! \brief          This function calculates the common secret.
 *  \details        This function calculates the common secret d1 * Q2 (= d1 * d2 * G)
 *                  and derives the output key from the x-coordinate of the secret with
 *                  the KDF2HMACSHA1 derivation function of the library.
 *  \param[in]      privatekey   the private key d1 of the key pair (d1, Q1)
 *  \param[in]      publickey_x  x-coordinate of the public key Q2 of the key pair (d2, Q2)
 *  \param[in]      publickey_y  y-coordinate of the public key Q2 of the key pair (d2, Q2)
 *  \param[in]      iteration_count  number of iterations used for the key derivation,
 *                               if zero, actKDF2_DEFAULT_ITERATION_COUNT is used.
 *  \param[in]      salt         pointer to the optional salt (if NULL, no salt is used)
 *  \param[in]      salt_length  length of salt in bytes
 *  \param[in,out]  key          (in) pointer to the output key buffer
 *                               (out) the derived key of length key_length
 *  \param[in]      key_length   length of output key buffer in bytes
 *  \param[in,out]  wksp         (in) workspace
 *                               (out) initialized workspace
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \return         actEXCEPTION_NULL       if an input parameter is NULL
 *                  actEXCEPTION_PRIVKEY    if (d==0||d>=n)
 *                  actEXCEPTION_PUBKEY     if Q2 is invalid
 *                  actEXCEPTION_UNKNOWN    internal error (result point not on curve)
 *                  actOK                   else
 *  \pre            wksp has been initialized with actECDHInitGetSecret
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECDHKeyDerive(VSECPRIM_P2CONST_PARA(actU8) privatekey, VSECPRIM_P2CONST_PARA(actU8) publickey_x,
                                              VSECPRIM_P2CONST_PARA(actU8) publickey_y, actLengthType iteration_count,
                                              VSECPRIM_P2CONST_PARA(actU8) salt, actLengthType salt_length, VSECPRIM_P2VAR_PARA(actU8) key, actLengthType key_length, VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp,
                                              VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
}                               /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTIECDH_H */

/**********************************************************************************************************************
 *  END OF FILE: actIECDH.h
 *********************************************************************************************************************/
