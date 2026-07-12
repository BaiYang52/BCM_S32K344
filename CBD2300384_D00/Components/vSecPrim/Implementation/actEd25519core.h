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
/*!        \file  actEd25519core.h
 *        \brief  Core Ed25519 implementation.
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

#ifndef ACTED25519CORE_H
# define ACTED25519CORE_H

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
 *  actEd25519_initCore()
 *********************************************************************************************************************/
/*! \brief       Init core.
 *  \details     This function initializes the internal data structures.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \pre         ws must be a valid workspace pointer
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actEd25519_initCore(VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws);

/**********************************************************************************************************************
 *  actEd25519_sign()
 *********************************************************************************************************************/
/*! \brief       Creates a signature over the data.
 *  \details     This function creates a signature over the given data.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[out]  signature  Pointer to the signature buffer (64 bytes).
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \pre         ws must be a valid workspace pointer
 *               signature must be a valid pointer
 *               ws must be initialized
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actEd25519_sign(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2VAR_PARA(actU8) signature,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actEd25519_verify()
 *********************************************************************************************************************/
/*! \brief       Verifies signature over data.
 *  \details     This function verifies a signature over the data.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[out]  signature  Pointer to the signature.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \return      actOK - Signatures verification successful.
 *  \return      actVERIFICATION_FAILED - otherwise.
 *  \pre         ws must be a valid workspace pointer
 *               signature must be a valid pointer
 *               ws must be initialized
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actEd25519_verify(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) signature,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actEd25519_importKeyPair()
 *********************************************************************************************************************/
/*! \brief       Import key pair.
 *  \details     This function imports the secret and / or public key for signature / verification. The private scalar
 *               is derived from the secret key, and when no public key is passed, that is calculated from the private
 *               scalar.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[in]   secretKey  Pointer to the secret key (32 bytes).
 *  \param[in]   publicKey  Pointer to the public key (32 bytes).
 *  \param[in]   watchdog  Pointer to watchdog trigger function.
 *  \pre         ws must be a valid workspace pointer
 *               secretKey, publicKey must be valid pointers
 *               ws must be initialized
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actEd25519_importKeyPair(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) secretKey,
  VSECPRIM_P2CONST_PARA(actU8) publicKey,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
}
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTED25519CORE_H */

/**********************************************************************************************************************
 *  END OF FILE: actEd25519core.h
 *********************************************************************************************************************/
