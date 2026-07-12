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
/*!        \file  actIAES.h
 *        \brief  AES implementation.
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

#ifndef ACTIAES_H
# define ACTIAES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"
# include "actPadding.h"

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
 *  actAESInitEncrypt()
 *********************************************************************************************************************/
/*! \brief       AES Encrypt Init.
 *  \details     This function initializes the AES algorithm (key setup etc.).
 *  \param[in,out]  info   Pointer to AES context structure.
 *  \param[in]   key       Pointer to the user key.
 *  \param[in]   key_len   Length of user key.
 *  \param[in]   iv        Pointer to initialization vector (16 byte).
 *  \param[in]   mode      Block mode (ECB/CBC/CTR).
 *  \param[in]   watchdog  Function pointer to watchdog trigger function.
 *  \return      actOK - function has been called with invalid parameters
 *               actEXCEPTION_NULL - if key parameter is NULL
 *               actEXCEPTION_LENGTH - if key_len is not 16, 24 or 32
 *               actEXCEPTION_ALGOID - mode is invalid
 *  \pre         -
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actAESInitEncrypt(
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len, VSECPRIM_P2CONST_PARA(actU8) iv,
  actU8 mode, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actAESEncrypt()
 *********************************************************************************************************************/
/*! \brief       Perform AES Encryption.
 *  \details     This function encrypts input data of any length. If final==0, ((in_length+buffered_bytes)/16) blocks
 *               will be encrypted and written to out_buffer, ((in_length+buffered_bytes)%16) bytes will be buffered,
 *               where 0 <= buffered_bytes < 16. If final!=0, (((in_length+buffered_bytes)/16)+1) blocks will be
 *               encrypted and written to out_buffer. PKCS 5 padding is used for the last block padding.
 *  \param[in,out]  info   pointer to initialized AES context structure.
 *  \param[in]   in        Pointer to data to be encrypted.
 *  \param[in]   in_length   Length of data in bytes.
 *  \param[out]  out_buffer  Pointer to encrypted data buffer.
 *  \param[in,out]  out_length  In: Pointer to buffer size; Out: Actually written length.
 *  \param[in]   final     Flag for padding.
 *  \param[in]   watchdog  Function pointer to watchdog trigger function.
 *  \return      actOK - function has been called with invalid parameters
 *               actEXCEPTION_NULL - pointer is NULL pointer
 *               actEXCEPTION_LENGTH - *out_length is too small
 *               actEXCEPTION_PADDING - Padding scheme not available
 *  \pre         Provided workspace is initialized.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actAESEncrypt(
   VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
   VSECPRIM_P2VAR_PARA(actU8) out_buffer, VSECPRIM_P2VAR_PARA(actLengthType) out_length,
   actBoolean final, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actAESInitDecrypt()
 *********************************************************************************************************************/
/*! \brief       AES Decrypt Init.
 *  \details     This function initializes the AES algorithm (key setup etc.).
 *  \param[in,out]  info   Pointer to AES context structure.
 *  \param[in]   key       Pointer to the user key.
 *  \param[in]   key_len   Length of user key.
 *  \param[in]   iv        Pointer to initialization vector (16 byte).
 *  \param[in]   mode      Block mode (ECB/CBC/CTR).
 *  \param[in]   watchdog  Function pointer to watchdog trigger function.
 *  \return      actOK - function has been called with invalid parameters
 *               actEXCEPTION_NULL - if key parameter is NULL
 *               actEXCEPTION_LENGTH - if key_len is not 16, 24 or 32
 *               actEXCEPTION_ALGOID - mode is invalid
 *  \pre         Pointers must be valid.
 *               iv can be null pointer for ECB mode.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actAESInitDecrypt(
   VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len, VSECPRIM_P2CONST_PARA(actU8) iv,
   actU8 mode, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actAESDecrypt()
 *********************************************************************************************************************/
/*! \brief       Perform AES Decryption.
 *  \details     This function decrypts input data of any length. If final==0, ((in_length+buffered_bytes)/16) blocks
 *               will be decrypted and written to out_buffer, ((in_length+buffered_bytes)%16) bytes will be buffered,
 *               where 0 <= buffered_bytes < 16. If final!=0, ((in_length+buffered_bytes)/16) blocks will be decrypted
 *               and written to out_buffer, where (in_length+buffered_bytes) is a multiple of 16. PKCS 5 padding is
 *               used to unpad the last block.
 *  \param[in,out]  info   pointer to initialized AES context structure.
 *  \param[in]   in        Pointer to data to be decrypted.
 *  \param[in]   in_length   Length of data in bytes.
 *  \param[out]  out_buffer  Pointer to decrypted data buffer.
 *  \param[in,out]  out_length  In: Pointer to buffer size; Out: Actually written length.
 *  \param[in]   final     Flag for unpadding.
 *  \param[in]   watchdog  Function pointer to watchdog trigger function.
 *  \return      actOK - function has been called with invalid parameters
 *               actEXCEPTION_NULL - pointer is NULL pointer
 *               actEXCEPTION_LENGTH - *out_length is too small
 *               actEXCEPTION_INPUT_LENGTH  - input length incorrect
 *               actEXCEPTION_PADDING - Padding scheme not available
 *  \pre         Provided workspace is initialized.
 *               Pointers must be valid.
 *               out_buffer must be multiple of block size.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actAESDecrypt(
   VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
   VSECPRIM_P2VAR_PARA(actU8) out_buffer, VSECPRIM_P2VAR_PARA(actLengthType) out_length,
   actBoolean final, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actAESInitEncryptBlock()
 *********************************************************************************************************************/
/*! \brief       AES Block Encrypt Init.
 *  \details     This function initializes the AES block encryption.
 *  \param[in,out]  info   Pointer to AES context structure.
 *  \param[in]   key       Pointer to the user key.
 *  \param[in]   key_len   Length of user key.
 *  \param[in]   watchdog  Function pointer to watchdog trigger function.
 *  \return      actOK - function has been called with invalid parameters
 *               actEXCEPTION_NULL - if key parameter is NULL
 *               actEXCEPTION_LENGTH - if key_len is not 16, 24 or 32
 *  \pre         Pointers must be valid
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces.
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actAESInitEncryptBlock(
   VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actAESEncryptBlock()
 *********************************************************************************************************************/
/*! \brief       Perform AES block encryption.
 *  \details     This function encrypts the given block.
 *  \param[in,out]  info   Pointer to AES context structure.
 *  \param[in]   in_block  Pointer to block to be encrypted (16 bytes).
 *  \param[out]  out_block   Pointer to allocated output block (16 bytes).
 *  \param[in]   watchdog  Function pointer to watchdog trigger function.
 *  \pre         Provided workspace is initialized.
 *               in_block, out_block must be of size 16 bytes
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actAESEncryptBlock(
   VSECPRIM_P2CONST_PARA(actAESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in_block, VSECPRIM_P2VAR_PARA(actU8) out_block,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actAESInitDecryptBlock()
 *********************************************************************************************************************/
/*! \brief       AES Block Decrypt Init.
 *  \details     This function initializes the AES block decryption.
 *  \param[in,out]  info   Pointer to AES context structure.
 *  \param[in]   key       Pointer to the user key.
 *  \param[in]   key_len   Length of user key.
 *  \param[in]   watchdog  Function pointer to watchdog trigger function.
 *  \return      actOK - function has been called with invalid parameters
 *               actEXCEPTION_NULL - if key parameter is NULL
 *               actEXCEPTION_LENGTH - if key_len is not 16, 24 or 32
 *  \pre         Pointers must be valid
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actAESInitDecryptBlock(
   VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actAESDecryptBlock()
 *********************************************************************************************************************/
/*! \brief       Perform AES block decryption.
 *  \details     This function decrypts the given block.
 *  \param[in,out]  info   Pointer to AES context structure.
 *  \param[in]   in_block  Pointer to block to be decrypted (16 bytes).
 *  \param[out]  out_block  Pointer to allocated output block (16 bytes).
 *  \param[in]   watchdog  Function pointer to watchdog trigger function.
 *  \pre         Provided workspace is initialized.
 *               Pointers must be valid.
 *               in_block, out_block must be of size 16 bytes
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actAESDecryptBlock(
   VSECPRIM_P2CONST_PARA(actAESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in_block, VSECPRIM_P2VAR_PARA(actU8) out_block,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTIAES_H */

/**********************************************************************************************************************
 *  END OF FILE: actIAES.h
 *********************************************************************************************************************/
