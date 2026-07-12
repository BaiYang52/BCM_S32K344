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
/*!        \file  actIEd25519.h
 *        \brief  Ed25519 internal programming interface
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

#ifndef ACTIED25519_H
# define ACTIED25519_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"
# include "actISHA2_64.h"

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
 *  actEd25519Init()
 *********************************************************************************************************************/
/*! \brief       Init Ed25519.
 *  \details     This function initializes the Ed25519 algorithm.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[in]   instance  Ed25519 instance to use; one of:
 *                         actEd25519pure
 *                         actEd25519ctx
 *                         actEd25519ph
 *  \param[in]   context   Pointer to the context data
 *  \param[in]   ctxLen    Length of the context data
 *  \return      actOK  -  actOK is always returned.
 *  \pre         ws must be a valid pointer.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actEd25519Init(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  const actU8 instance,
  VSECPRIM_P2CONST_PARA(actU8) context,
  const actLengthType ctxLen);

/**********************************************************************************************************************
 *  actEd25519Update()
 *********************************************************************************************************************/
/*! \brief       Update Ed25519.
 *  \details     This function updates the internal hash, if pre-hash is used.
 *  \param[in,out]  ws     Pointer to the work-space structure.
 *  \param[in]   message   Pointer to the input data.
 *  \param[in]   msgLen    Length of the input data.
 *  \param[in]   watchdog  Pointer to watchdog trigger function.
 *  \return      actOK  -  Update was successful.
 *               actEXCEPTION_ALGOID  -  Invalid instance.
 *               actEXCEPTION_LENGTH  -  Length exception.
 *  \pre         pointers must be valid
 *               ws must be initialized
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actEd25519Update(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) message,
  const actLengthType msgLen,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actEd25519Sign()
 *********************************************************************************************************************/
/*! \brief       Sign provided message.
 *  \details     This function creates a signature over the data. When pre-hash is used, the data are hashed first.
 *               The final hash value is then signed.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[in]   message    Pointer to the input data.
 *  \param[in]   msgLen     Length of the input data
 *  \param[in]   secretKey  Pointer to the secret key used to sign data (32 bytes)
 *  \param[in]   publicKey  Pointer to the public key used to sign data (32 bytes - optional!)
 *  \param[out]  signature  Pointer to the signature buffer (64 bytes)
 *  \param[in]   watchdog   Pointer to watchdog trigger function.
 *  \return      actOK  -  success
 *               actEXCEPTION_UNKNOWN  -  Hashing failure
 *  \pre         pointers must be valid
 *               ws must be initialized
 *               signature must be at least 512 bytes
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actEd25519Sign(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) message,
  const actLengthType msgLen,
  VSECPRIM_P2CONST_PARA(actU8) secretKey,
  VSECPRIM_P2CONST_PARA(actU8) publicKey,
  VSECPRIM_P2VAR_PARA(actU8) signature,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actEd25519Verify()
 *********************************************************************************************************************/
/*! \brief       Verifies signature over given data.
 *  \details     This function verifies the signature over the data. When pre-hash is used, the data are hashed first.
 *               The signature is then verified against the final hash value.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[in]   message    Pointer to the input data.
 *  \param[in]   msgLen     Length of the input data
 *  \param[in]   publicKey  Pointer to the key used to verify signature (32 bytes)
 *  \param[in]   signature  Pointer to the signature buffer (64 bytes)
 *  \param[in]   watchdog   Pointer to watchdog trigger function.
 *  \return      actOK  -  Signature is valid
 *               actVERIFICATION_FAILED -  Signature is invalid
 *               actEXCEPTION_UNKNOWN   -  Hashing failure
 *  \pre         pointers must be valid
 *               ws must be initialized
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actEd25519Verify(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) message,
  const actLengthType msgLen,
  VSECPRIM_P2CONST_PARA(actU8) publicKey,
  VSECPRIM_P2CONST_PARA(actU8) signature,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actEd25519GenKeyPair()
 *********************************************************************************************************************/
/*! \brief       Generate Key Pair.
 *  \details     This function generates a key pair.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[out]  secretKey  Pointer to the secret key buffer (32 bytes).
 *  \param[out]  publicKey  Pointer to the public key buffer (32 bytes).
 *  \param[in]   watchdog   Pointer to watchdog trigger function.
 *  \return      actOK  -  actOK is always returned.
 *               actEXCEPTION_PRIVKEY  -  Failed to generate private key
 *  \pre         pointers must be valid
 *               ws must be initialized
 *               secretKey, publicKey must be at least 32 bytes
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actEd25519GenKeyPair(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2VAR_PARA(actU8) secretKey,
  VSECPRIM_P2VAR_PARA(actU8) publicKey,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTIED25519_H */

/**********************************************************************************************************************
 *  END OF FILE: actIEd25519.h
 *********************************************************************************************************************/
