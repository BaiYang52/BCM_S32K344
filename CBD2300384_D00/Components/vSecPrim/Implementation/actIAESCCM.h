/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is protected under intellectual property laws and proprietary to cv cryptovision GmbH
 *                and/or Vector Informatik GmbH.
 *                No right or license is granted save as expressly set out in the applicable license conditions.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  actIAESCCM.h
 *        \brief  AES128CCM implementation.
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library actCLib
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

#ifndef ACTIAEADAES128CCM_H
# define ACTIAEADAES128CCM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"
# include "actIAES.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define actAESCCM_UPDATE_NOT_CALLED                                  (0x00u)
# define actAESCCM_UPDATE_CALLED                                      (0x01u)

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
 *  actAESCCMInit()
 *********************************************************************************************************************/
/*! \brief          Initializes the Counter-with-CBC-MAC-Mode for en- or decryption.
 *  \details        This function initializes the Counter-with-CBC-MAC-Mode.
 *  \param[in,out]  info  (in)  pointer to context structure
 *                        (out) initialized context structure
 *  \param[in]      key  AES key to be used
 *  \param[in]      key_length  length of key in bytes
 *  \param[in]      nonce  nonce vector
 *  \param[in]      nonce_length  length of the nonce (max 2^32 - 1 byte)
 *  \param[in]      aadLength total length of additional authentication data that will be used to compute the CBC MAC/
 *                            the authentication tag
 *  \param[in]      msgLength total length of the message to be encrypted or decrypted
 *  \param[in]      authenticationFieldSize desired length of the authentication tag in bytes, must be either
 *                                          4, 6, 8, 10, 12, 14 or 16
 *  \param[in]      lengthFieldSize value L indicating that msgLength < 2^(8*L) bytes, i.e., L is the dual logarithm of
 *                                  msgLength divided by 8
 *  \param[in]      watchdog  pointer to watchdog reset function
 *  \pre            pointers must be valid
 *  \note           Although the aad and the msg are arguments of the update functions, their total length needs to be
 *                  known in advance. CCM operates block-wisely and these length values are needed for the very first
 *                  two blocks.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actAESCCMInit(
 VSECPRIM_P2VAR_PARA(actAESCCMSTRUCT) info,
 VSECPRIM_P2CONST_PARA(actU8) key,
 const actLengthType keyLength,
 VSECPRIM_P2CONST_PARA(actU8) nonce,
 const actLengthType nonceLength,
 const actLengthType aadLength,
 const actLengthType msgLength,
 const actU8 authenticationFieldSize,
 const actU8 lengthFieldSize,
 VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actAESCCMUpdateEncrypt()
 *********************************************************************************************************************/
/*! \brief          Encrypts the messsage and starts computing the authentication tag.
 *  \details        This function encrypts as many blocks of size actAES_BLOCK_SIZE of the message as are fully
 *                  contained in the message. If msgLength is a multiple of actAES_BLOCK_SIZE, the complete message
 *                  is encrypted. However, the authentication tag is not computed yet.
 *                  Note: This function does not allow for a multi update! The aad and the msg have to be passed
 *                  to this function completely in one call. The aadLength and the msgLength have to match the
 *                  length values passed to the init function.
 *  \param[in,out]  info        (in)  pointer to context structure
 *                              (out) pointer to context structure
 *  \param[in]      aad         pointer to additional authentication data
 *  \param[in]      aadLength   length of additional authentication data
 *  \param[in]      msg         pointer to the message to be encrypted
 *  \param[in]      msgLength   length of the message to be encrypted
 *  \param[in,out]  out         (in)  pointer to output buffer for the encrypted data
                                (out) pointer to output buffer, containing the encrypted data
 *  \param[in]      watchdog  pointer to watchdog reset function
 *  \return         actEXCEPTION_MODE         actAESCCMUpdateEncrypt called more than once
 *                  actEXCPETION_INPUT_LENGTH aadLength or msgLength does not match the length value stored in info
 *                                            (this value was passed in actAESCCMInit)
 *                  actEXCEPTION_LENGTH aadLength > (2^32 - actAES_BLOCK_SIZE - length of aad length encoding) byte
 *  \pre            pointers must be valid
 *                  out references a valid memory location of size msgLength - (msgLength mod actAES_BLOCK_SIZE)
 *  \note           msgLength is limited to 2^32 - 1 byte
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actAESCCMUpdateEncrypt(
 VSECPRIM_P2VAR_PARA(actAESCCMSTRUCT) info,
 VSECPRIM_P2CONST_PARA(actU8) aad,
 const actLengthType aadLength,
 VSECPRIM_P2CONST_PARA(actU8) msg,
 const actLengthType msgLength,
 VSECPRIM_P2VAR_PARA(actU8) out,
 VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actAESCCMUpdateDecrypt()
 *********************************************************************************************************************/
/*! \brief          Decrypts the messsage and starts re-computing the authentication tag.
 *  \details        This function decrypts as many blocks of size actAES_BLOCK_SIZE of the message as are fully
 *                  contained in the message. If msgLength is a multiple of actAES_BLOCK_SIZE, the complete message
 *                  is decrypted. However, the authentication tag is not re-computed yet.
 *                  Note: This function does not allow for a multi update! The aad and the msg have to be passed
 *                  to this function completely in one call. The aadLength and the msgLength have to match the
 *                  length values passed to the init function.
 *  \param[in,out]  info        (in)  pointer to context structure
 *                              (out) pointer to context structure
 *  \param[in]      aad         pointer to additional authentication data
 *  \param[in]      aadLength   length of additional authentication data
 *  \param[in]      msg         pointer to the message to be decrypted
 *  \param[in]      msgLength   length of the message to be decrypted
 *  \param[in,out]  out         (in)  pointer to output buffer for the decrypted data
                                (out) pointer to output buffer, containing the decrypted data
 *  \param[in]      watchdog  pointer to watchdog reset function
 *  \return         actEXCEPTION_MODE         actAESCCMUpdateDecrypt called more than once
 *                  actEXCPETION_INPUT_LENGTH aadLength or msgLength does not match the length value stored in info
 *                                            (this value was passed in actAESCCMInit)
 *                  actEXCEPTION_LENGTH aadLength > (2^32 - actAES_BLOCK_SIZE - length of aad length encoding) byte
 *  \pre            pointers must be valid
 *                  out references a valid memory location of size msgLength - (msgLength mod actAES_BLOCK_SIZE)
 *  \note           msgLength is limited to 2^32 - 1 byte
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
 VSECPRIM_FUNC(actRETURNCODE) actAESCCMUpdateDecrypt(
  VSECPRIM_P2VAR_PARA(actAESCCMSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) aad,
  const actLengthType aadLength,
  VSECPRIM_P2CONST_PARA(actU8) msg,
  const actLengthType msgLength,
  VSECPRIM_P2VAR_PARA(actU8) out,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actAESCCMFinalizeEncrypt()
 *********************************************************************************************************************/
/*! \brief          Finalizes encryption and returns the tag.
 *  \details        This function finalizes encryption and returns the tag.
 *                  If the message was not encrypted completely during the call to update, the remaining bytes are
 *                  encrypted and written to out.
 *  \param[in,out]  info       (in)  pointer to context structure
 *                             (out) pointer to context structure
 *  \param[in,out]  out        (in)  pointer to output buffer for remaining data bytes
                               (out) pointer to output buffer, containing the remaining data bytes
 *  \param[in,out]  tag        (in)  pointer to output buffer for the authentication tag
                               (out) pointer to output buffer, containing the authentication tag
 *  \param[in]      watchdog  pointer to watchdog reset function
 *  \return         actEXCEPTION_LENGTH  invalid value in info->remainingLen
 *                  actOK                operation successful
 *  \pre            info and tag must be valid pointers
 *                  out is a valid pointer, if info->remainingLen > 0
 *                  In this case, out references a valid memory location of size (msgLen mod actAES_BLOCK_SIZE) bytes.
 *                  The buffer referenced by tag provides at least info->authenticationFieldSize bytes.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actAESCCMFinalizeEncrypt(
  VSECPRIM_P2VAR_PARA(actAESCCMSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) out,
  VSECPRIM_P2VAR_PARA(actU8) tag,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actAESCCMFinalizeDecrypt()
 *********************************************************************************************************************/
/*! \brief          Finalizes Decryption and returns the tag.
 *  \details        This function finalizes decryption and returns the tag.
 *                  If the message was not decrypted completely during the call to update, the remaining bytes are
 *                  decrypted and written to out.
 *  \param[in,out]  info       (in)  pointer to context structure
 *                             (out) pointer to context structure
 *  \param[in,out]  out        (in)  pointer to output buffer for remaining data bytes
                               (out) pointer to output buffer, containing the remaining data bytes
 *  \param[in,out]  tag        (in)  pointer to output buffer for the authentication tag
                               (out) pointer to output buffer, containing the authentication tag
 *  \param[in]      watchdog  pointer to watchdog reset function
 *  \return         actEXCEPTION_LENGTH  invalid value in info->remainingLen
 *                  actOK                operation successful
 *  \pre            info and tag must be valid pointers
 *                  out is a valid pointer, if info->remainingLen > 0
 *                  In this case, out references a valid memory location of size (msgLen mod actAES_BLOCK_SIZE) bytes.
 *                  The buffer referenced by tag provides at least info->authenticationFieldSize bytes.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actAESCCMFinalizeDecrypt(
  VSECPRIM_P2VAR_PARA(actAESCCMSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) out,
  VSECPRIM_P2VAR_PARA(actU8) tag,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTIAEADAES128CCM_H */

/**********************************************************************************************************************
 *  END OF FILE: actIAEADAES128CCM.h
 *********************************************************************************************************************/
