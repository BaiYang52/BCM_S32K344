/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2006 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  actTDES.h
 *        \brief  This file contains the interface for TDES algorithm.
 *
 *      \details  Currently the actClib version is used.
 *                This file is part of the embedded systems library cvActLib/ES
 *********************************************************************************************************************/

#ifndef ACTTDES_H
# define ACTTDES_H

# include "actDES.h"
# include "actITypes.h"
# include "actConfig.h"

/****************************************************************************
 ** Types and constants
 ***************************************************************************/

/* Context structure of the TDES algorithm */
typedef struct
{
   actDESSTRUCT k1;
   actDESSTRUCT k2;
   actDESSTRUCT k3;
   VSECPRIM_P2CONST_PARA(actU8) k1_ptr;
   VSECPRIM_P2CONST_PARA(actU8) k2_ptr;
   VSECPRIM_P2CONST_PARA(actU8) k3_ptr;
   actU8 mode;
   actU8 prev_block[actDES_BLOCK_SIZE];
   actLengthType buffer_used;
   actU8 buffer[actDES_BLOCK_SIZE];
} actTDESSTRUCT;

/* PRQA S 0850 4 */ /* MD_MSR_MacroArgumentEmpty */
typedef VSECPRIM_P2FUNC(VSECPRIM_NONE, void, actTDESTransformFctType)(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU8) out_buffer,
   actLengthType blocks, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actTDESInit()
 *********************************************************************************************************************/
/*! \brief          This function initializes the TDES algorithm (key setup etc.).
 *  \details        -
 *  \param[in,out]  info         (in) pointer to TDES context structure
 *                               (out) initialized TDES context structure
 *  \param[in]      key          pointer to user key
 *  \param[in]      key_len      length of user key
 *  \param[in]      crypt_mode   encryption, decryption mode switch
 *  \param[in]      iv           pointer to initialization vector (only in CBC mode;
 *                               of length actDES_BLOCK_SIZE; if iv==NULL, a zeroized
 *                               block is used as iv)
 *  \param[in]      block_mode   ECB, CBC mode switch
 *  \param[in]      watchdog     Pointer to watchdog trigger function.
 *  \return         actEXCEPTION_NULL      if key parameter is NULL
 *                  actEXCEPTION_ALGOID    mode is invalid
 *                  actOK                  else
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actTDESInit(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actU8 key_len,
   actU8 crypt_mode, VSECPRIM_P2CONST_PARA(actU8) iv,
   actU8 block_mode, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actTDESKeySetup()
 *********************************************************************************************************************/
/*! \brief          This function initializes round keys
 *  \details        -
 *  \param[in,out]  info         (in) pointer to TDES context structure
 *                               (out) modified TDES context structure
 *  \param[in]      key          pointer to user key
 *  \param[in]      key_len      length of user key
 *  \param[in]      crypt_mode   encryption, decryption mode switch
 *  \param[in]      watchdog     Pointer to watchdog trigger function.
 *  \return         actEXCEPTION_NULL      if key parameter is NULL
 *                  actOK                  else
 *  \pre            actTDESInit(info) is called once before calling this function
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actTDESKeySetup(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actU8 key_len,
   actU8 crypt_mode, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actTDESInitEncrypt()
 *********************************************************************************************************************/
/*! \brief          This function initializes the TDES algorithm (key setup etc.).
 *  \details        -
 *  \param[in,out]  info         (in) pointer to TDES context structure
 *                               (out) initialized TDES context structure
 *  \param[in]      key          pointer to user key
 *  \param[in]      key_len      length of user key
 *  \param[in]      iv           pointer to initialization vector (only in CBC mode;
 *                               of length actDES_BLOCK_SIZE; if iv==NULL, a zeroized
 *                               block is used as iv)
 *  \param[in]      mode         ECB, CBC mode switch
 *  \param[in]      watchdog     Pointer to watchdog trigger function.
 *  \return         actEXCEPTION_NULL      if key parameter is NULL
 *                  actEXCEPTION_ALGOID    mode is invalid
 *                  actOK                  else
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actTDESInitEncrypt(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actU8  key_len,
   VSECPRIM_P2CONST_PARA(actU8) iv, actU8 mode,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actTDESEncrypt()
 *********************************************************************************************************************/
/*! \brief          This function encrypts input data of any length.
 *  \details        If final==0, ((in_length+buffered_bytes)/8) blocks will be encrypted
 *                  and written to out_buffer, ((in_length+buffered_bytes)%8) bytes will
 *                  be buffered, where 0 <= buffered_bytes < 8.
 *                  If final!=0, (((in_length+buffered_bytes)/8)+1) blocks will be
 *                  encrypted and written to out_buffer.
 *                  PKCS 5 padding is used for the last block padding.
 *  \param[in,out]  info         (in) pointer to initialized TDES context structure
 *                               (out) initialized TDES context structure. 
 *                               In info->prev_block at the end: updated previous block.
 *  \param[in]      in           pointer to data to be encrypted
 *  \param[in]      in_length    length of data in bytes
 *  \param[in,out]  out_buffer   (in) pointer to encrypted data buffer
 *                               (out) encrypted data blocks
 *  \param[in,out]  *out_length  (in) out_buffer size in bytes, a safe length is:
 *                               (((in_length+7)/8)+1)*8       (see output)
 *                               (out) number of encrypted bytes:
 *                               ((in_length+buffered_bytes)/8)*8       if final==0
 *                               (((in_length+buffered_bytes)/8)+1)*8   else
 *  \param[in]      final        flag for padding
 *  \param[in]      watchdog     Pointer to watchdog trigger function.
 *  \return         actEXCEPTION_LENGTH   *out_length is too small
 *                  actOK                 else
 *  \pre            actTDESInitEncrypt() is called once before calling this function
 *                  in!=NULL, out_buffer!=NULL are a valid pointers
 *                  in_length>=0, *out_length>=0 (at input)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actTDESEncrypt(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
   VSECPRIM_P2VAR_PARA(actU8) out_buffer, VSECPRIM_P2VAR_PARA(actLengthType) out_length,
   boolean final, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actTDESInitDecrypt()
 *********************************************************************************************************************/
/*! \brief          This function initializes the TDES decryption.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to TDES context structure
 *                               (out) initialized TDES context structure
 *  \param[in]      key          pointer to user key
 *  \param[in]      key_len      length of user key
 *  \param[in]      iv           pointer to initialization vector (only in CBC mode;
 *                               of length actDES_BLOCK_SIZE; if iv==NULL, a zeroized
 *                               block is used as iv)
 *  \param[in]      mode         ECB, CBC mode switch
 *  \param[in]      watchdog     Pointer to watchdog trigger function.
 *  \return         actEXCEPTION_NULL      if key parameter is NULL
 *                  actEXCEPTION_ALGOID    mode is invalid
 *                  actOK                  else
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actTDESInitDecrypt(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actU8 key_len,
   VSECPRIM_P2CONST_PARA(actU8) iv, actU8 mode,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actTDESDecrypt()
 *********************************************************************************************************************/
/*! \brief          This function decrypts input data of any length.
 *  \details        If final==0, ((in_length+buffered_bytes)/8) blocks will be decrypted
 *                  and written to out_buffer, ((in_length+buffered_bytes)%8) bytes will
 *                  be buffered, where 0 <= buffered_bytes < 8.
 *                  If final!=0, ((in_length+buffered_bytes)/8) blocks will be decrypted
 *                  and written to out_buffer, where (in_length+buffered_bytes) is a
 *                  multiple of 8. PKCS 5 padding is used to unpad the last block.
 *  \param[in,out]  info         (in) pointer to initialized TDES context structure
 *                               (out) initialized TDES context structure
 *                               In info->prev_block at the end: updated previous block
 *  \param[in]      in           pointer to data to be decrypted
 *  \param[in]      in_length    length of data in bytes (multiple of 8)
 *  \param[in,out]  out_buffer   (in) pointer to decrypted data buffer of length in_length
 *                               (out) decrypted data blocks
 *  \param[in,out]  *out_length  (in) out_buffer size in bytes, a safe length is:
 *                               ((in_length+7)/8)*8       (see output)
 *                               (out) number of decrypted bytes:
 *                               ((in_length+buffered_bytes)/8)*8          if final==0
 *                               (in_length+buffered_bytes)/8)*8 - pad     else, where 1<=pad<=8
 *  \param[in]      final        flag for unpadding
 *  \param[in]      watchdog     Pointer to watchdog trigger function.
 *  \return         actEXCEPTION_LENGTH   *out_length is too small
 *                  actEXCEPTION_INPUT_LENGTH (in_length+buffered_bytes)%8!=0 (if final!=0) 
 *                  actEXCEPTION_PADDING  PKCS 5 unpad error
 *                  actOK                 else
 *  \pre            actTDESInitDecrypt() is called once before calling this function
 *                  in!=NULL, out_buffer!=NULL are a valid pointers
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actTDESDecrypt(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
   VSECPRIM_P2VAR_PARA(actU8) out_buffer, VSECPRIM_P2VAR_PARA(actLengthType) out_length,
   boolean final, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actTDESInitEncryptBlock()
 *********************************************************************************************************************/
/*! \brief          This function initializes the TDES block encryption.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to TDES context structure
 *                               (out) initialized TDES context structure
 *  \param[in]      key          pointer to user key
 *  \param[in]      key_len      length of user key
 *  \param[in]      watchdog     Pointer to watchdog trigger function.
 *  \return         actEXCEPTION_NULL      if key parameter is NULL
 *                  actOK                  else
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actTDESInitEncryptBlock(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actU8 key_len,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actTDESInitDecryptBlock()
 *********************************************************************************************************************/
/*! \brief          This function initializes the TDES block decryption.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to TDES context structure
 *                               (out) initialized TDES context structure
 *  \param[in]      key          pointer to user key
 *  \param[in]      key_len      length of user key
 *  \param[in]      watchdog     Pointer to watchdog trigger function.
 *  \return         actEXCEPTION_NULL      if key parameter is NULL
 *                  actOK                  else
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actTDESInitDecryptBlock(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actU8  key_len,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actTDESCryptBlock()
 *********************************************************************************************************************/
/*! \brief          This function encrypts the given blocks in different modes.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to TDES context structure
 *                               (out) initialized TDES context structure
 *  \param[in]      in_block     pointer to block to be encrypted
 *  \param[in,out]  out_block    (in) pointer to allocated output block
 *                               (out) encrypted output block
 *  \param[in]      watchdog     Pointer to watchdog trigger function.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actTDESCryptBlock (
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in_block, VSECPRIM_P2VAR_PARA(actU8) out_block,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actTDESEncryptMultipleBlocks()
 *********************************************************************************************************************/
/*! \brief          This function encrypts the given blocks in different modes.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to TDES context structure
 *                               (out) initialized TDES context structure
 *  \param[in]      in           pointer to blocks to be encrypted
 *  \param[in,out]  out_buffer   (in) pointer to allocated output blocks
 *                               (out) encrypted output blocks
 *  \param[in]      blocks       number of blocks
 *  \param[in]      watchdog     Pointer to watchdog trigger function.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actTDESEncryptMultipleBlocks(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU8) out_buffer,
   actLengthType blocks, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actTDESDecryptMultipleBlocks()
 *********************************************************************************************************************/
/*! \brief          This function decrypts the given blocks in different modes.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to TDES context structure
 *                               (out) initialized TDES context structure
 *  \param[in]      in           pointer to blocks to be decrypted
 *  \param[in,out]  out_buffer   (in) pointer to allocated output blocks
 *                               (out) decrypted output blocks
 *  \param[in]      blocks       number of blocks
 *  \param[in]      watchdog     Pointer to watchdog trigger function.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actTDESDecryptMultipleBlocks(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU8) out_buffer,
   actLengthType blocks, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actTDESTransform()
 *********************************************************************************************************************/
/*! \brief          This function transforms input data of any length.
 *  \details        ((in_length+buffered_bytes)/8) blocks will be transformed and written
 *                  to out_buffer, ((in_length+buffered_bytes)%8) bytes will be buffered,
 *                  where 0 <= buffered_bytes < 8.
 *  \param[in,out]  info         (in) pointer to TDES context structure
 *                               (out) initialized TDES context structure
 *                               In info->prev_block at the end: updated previous block
 *  \param[in]      in           pointer to data to be transformed
 *  \param[in]      in_length    length of data in bytes
 *  \param[in,out]  out_buffer   (in) pointer to encrypted data buffer
 *                               (out) encrypted data blocks
 *  \param[in]      transform    pointer to transformation function (Encrypt/Decrypt)
 *  \param[in]      watchdog     Pointer to watchdog trigger function.
 *  \pre            in!=NULL, out_buffer!=NULL are a valid pointers
 *                  in_length>=0
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actTDESTransform(
   VSECPRIM_P2VAR_PARA(actTDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
   VSECPRIM_P2VAR_PARA(actU8) out_buffer, actTDESTransformFctType transform,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTTDES_H */

