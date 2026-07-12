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
/*!        \file  actIX25519.h
 *        \brief  X25519 internal programming interface
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

#ifndef ACTIX25519_H
# define ACTIX25519_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/**********************************************************************************************************************
 *  actX25519Init()
 *********************************************************************************************************************/
/*! \brief       Init X25519.
 *  \details     This function initializes the X25519 algorithm.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \return      actOK  -  actOK is always returned.
 *  \pre         ws must be a valid pointer
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actX25519Init(VSECPRIM_P2VAR_PARA(actX25519STRUCT) ws);

/**********************************************************************************************************************
 *  actX25519GenKeyPair()
 *********************************************************************************************************************/
/*! \brief       Generate key pair.
 *  \details     This function generates an ephemeral key-pair for EC-D/H.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[out]  publicKey  Pointer to memory where the public key shall be stored.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \return      actOK  -  actOK is always returned.
 *  \pre         ws, publicKey must be valid pointers
 *               ws must be initialized
 *               publicKey must be at least 32 bytes
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actX25519GenKeyPair(
  VSECPRIM_P2VAR_PARA(actX25519STRUCT) ws,
  VSECPRIM_P2VAR_PARA(actU8) publicKey,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actX25519ImportPrivateKey()
 *********************************************************************************************************************/
/*! \brief       Import private key.
 *  \details     This function imports the private key for EC-D/H.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[in]  privateKey  Pointer to the private key buffer.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \return      actOK  -  actOK is always returned.
 *  \pre         ws, privateKey must be valid pointers
 *               ws must be initialized
 *               privateKey must be at least 32 bytes
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actX25519ImportPrivateKey(
  VSECPRIM_P2VAR_PARA(actX25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) privateKey,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actX25519GenSecret()
 *********************************************************************************************************************/
/*! \brief       Generate shared secret.
 *  \details     This function generates the shared secret for EC-D/H.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[in]  publicKey  Pointer to the public key buffer.
 *  \param[out]  sharedSecret  Pointer to the buffer where the shared secret shall be stored.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \return      actOK  -  actOK is always returned.
 *  \pre         ws, publicKey, sharedSecret must be valid pointers
 *               ws must be initialized
 *               publicKey, sharedSecret must be at least 32 bytes
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actX25519GenSecret(
  VSECPRIM_P2VAR_PARA(actX25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) publicKey,
  VSECPRIM_P2VAR_PARA(actU8) sharedSecret,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTIX25519_H */

/**********************************************************************************************************************
 *  END OF FILE: actIX25519.h
 *********************************************************************************************************************/
