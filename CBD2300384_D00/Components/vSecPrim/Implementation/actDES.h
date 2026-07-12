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
/*!        \file  actDES.h
 *        \brief  This file contains an interface for the DES implementation.
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library cvActLib/ES
 *********************************************************************************************************************/

#ifndef ACTDES_H
# define ACTDES_H

# include "actITypes.h"

/****************************************************************************
 ** Types and constants
 ***************************************************************************/
# define actDES_BLOCK_SIZE                                            (8u)
# define EN0                                                          (0u)
# define DE1                                                          (1u)

/* Context structure of the DES algorithm */
typedef struct
{
   actU32 n_keys[32];
   actU8 mode;
   actU8 prev_block[actDES_BLOCK_SIZE];
   actLengthType buffer_used;
   actU8 buffer[actDES_BLOCK_SIZE];
} actDESSTRUCT;

/* PRQA S 0850 4 */ /* MD_MSR_MacroArgumentEmpty */
typedef VSECPRIM_P2FUNC(VSECPRIM_NONE, void, actDESTransformFctType)(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info, VSECPRIM_P2CONST_PARA(actU8) in,
   VSECPRIM_P2VAR_PARA(actU8) out_buffer, actLengthType blocks,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/****************************************************************************
 ** DES mode identifiers
 **
 **    actDES_[mode_id]
 ***************************************************************************/
/* ECB */
# define actDES_ECB                                                   (0x20u)
/* CBC */
# define actDES_CBC                                                   (0x21u)


/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actDESInit()
 *********************************************************************************************************************/
/*! \brief          This function initializes the DES algorithm (key setup etc.).
 *  \details        -
 *  \param[in,out]  info         (in) pointer to DES context structure
 *                               (out) initialized DES context structure
 *  \param[in]      key          pointer to user key
 *  \param[in]      crypt_mode   encryption, decryption mode switch
 *  \param[in]      iv           pointer to initialization vector (only in CBC mode;
 *                               of length actDES_BLOCK_SIZE; if iv==NULL, a zeroized
 *                               block is used as iv)
 *  \param[in]      block_mode   ECB, CBC mode switch
 *  \param[in]      watchdog     Watchdog trigger function pointer

 *  \return         actEXCEPTION_NULL      if key parameter is NULL
 *                  actEXCEPTION_ALGOID    mode is invalid
 *                  actOK                  else
 *  \pre            -
 *  \note           This function initializes the mode switch "mode" of the context structure
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actDESInit(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info, VSECPRIM_P2CONST_PARA(actU8) key,
   actU8 crypt_mode, VSECPRIM_P2CONST_PARA(actU8) iv, actU8 block_mode,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actDESKeySetup()
 *********************************************************************************************************************/
/*! \brief          This function initializes round keys
 *  \details        -
 *  \param[in,out]  info         (out) modified DES context structure
 *                               (in) pointer to DES context structure
 *  \param[in]      key          pointer to user key
 *  \param[in]      mode         switch for decryption keys
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \return         actEXCEPTION_NULL      if key parameter is NULL
 *                  actOK                  else
 *  \pre            actDESInit(info) is called once before calling this function
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actDESKeySetup(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info, VSECPRIM_P2CONST_PARA(actU8) key,
   actU8 crypt_mode, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actDESInitEncrypt()
 *********************************************************************************************************************/
/*! \brief          This function initializes the DES algorithm (key setup etc.).
 *  \details        -
 *  \param[in,out]  info         (in) pointer to DES context structure
 *                               (out) initialized DES context structure
 *  \param[in]      key          pointer to user key
 *  \param[in]      iv           pointer to initialization vector (only in CBC mode)
 *                               of length actDES_BLOCK_SIZE; if iv==NULL, a zeroized
 *                               block is used as iv)
 *  \param[in]      mode         ECB, CBC mode switch
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \return         actEXCEPTION_NULL      if key parameter is NULL
 *                  actEXCEPTION_ALGOID    mode is invalid
 *                  actOK                  else
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actDESInitEncrypt(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info, VSECPRIM_P2CONST_PARA(actU8) key,
   VSECPRIM_P2CONST_PARA(actU8) iv, actU8 mode,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actDESEncrypt()
 *********************************************************************************************************************/
/*! \brief          This function encrypts input data of any length.
 *  \details        If final==0, ((in_length+buffered_bytes)/8) blocks will be encrypted
 *                  and written to out_buffer, ((in_length+buffered_bytes)%8) bytes will
 *                  be buffered, where 0 <= buffered_bytes < 8.
 *                  If final!=0, (((in_length+buffered_bytes)/8)+1) blocks will be
 *                  encrypted and written to out_buffer.
 *                  PKCS 5 padding is used for the last block padding
 *  \param[in,out]  info         (in) pointer to initialized DES context structure
 *                               (out) info->prev_block:   updated previous block
 *  \param[in]      in           pointer to data to be encrypted
 *  \param[in]      in_length    length of data in bytes
 *  \param[in,out]  out_buffer   (in) pointer to encrypted data buffer
 *                               (out) encrypted data blocks
 *  \param[in,out]  out_length   (in) out_buffer size in bytes, a safe length is:
 *                               (((in_length+7)/8)+1)*8       (see output)
 *                               (out) number of encrypted bytes:
 *                               ((in_length+buffered_bytes)/8)*8       if final==0
 *                               (((in_length+buffered_bytes)/8)+1)*8   else
 *  \param[in]      final        flag for padding
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \return         actEXCEPTION_LENGTH   *out_length is too small
 *                  actOK                 else
 *  \pre            actDESInitEncrypt() is called once before calling this function
 *                  in!=NULL, out_buffer!=NULL are a valid pointers
 *                  in_length>=0, *out_length>=0 (at input)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actDESEncrypt(
  VSECPRIM_P2VAR_PARA(actDESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
  VSECPRIM_P2VAR_PARA(actU8) out_buffer, VSECPRIM_P2VAR_PARA(actLengthType) out_length,
  boolean final, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actDESInitDecrypt()
 *********************************************************************************************************************/
/*! \brief          This function initializes the DES decryption.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to DES context structure
 *                               (out) initialized DES context structure
 *  \param[in]      key          pointer to user key
 *  \param[in]      iv           pointer to initialization vector (only in CBC mode)
 *                               of length actDES_BLOCK_SIZE; if iv==NULL, a zeroized
 *                               block is used as iv)
 *  \param[in]      mode         ECB, CBC mode switch
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \return         actEXCEPTION_NULL      if key parameter is NULL
 *                  actEXCEPTION_ALGOID    mode is invalid
 *                  actOK                  else
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actDESInitDecrypt(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info, VSECPRIM_P2CONST_PARA(actU8) key,
   VSECPRIM_P2CONST_PARA(actU8) iv, actU8 mode,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actDESDecrypt()
 *********************************************************************************************************************/
/*! \brief          This function decrypts input data of any length.
 *  \details        If final==0, ((in_length+buffered_bytes)/8) blocks will be decrypted
 *                  and written to out_buffer, ((in_length+buffered_bytes)%8) bytes will
 *                  be buffered, where 0 <= buffered_bytes < 8.
 *                  If final!=0, ((in_length+buffered_bytes)/8) blocks will be decrypted
 *                  and written to out_buffer, where (in_length+buffered_bytes) is a
 *                  multiple of 8. PKCS 5 padding is used to unpad the last block.
 *  \param[in,out]  info         (in) pointer to initialized DES context structure
 *                               (out) info->prev_block  updated previous block
 *  \param[in]      in           pointer to data to be decrypted
 *  \param[in]      in_length    length of data in bytes (multiple of 8)
 *  \param[in,out]  out_buffer   (in) pointer to decrypted data buffer of length in_length
 *                               (out) decrypted data blocks
 *  \param[in,out]  *out_length  (in) out_buffer size in bytes, a safe length is:
 *                               ((in_length+7)/8)*8       (see output)
 *                               (out) number of decrypted bytes:
 *                               ((in_length+buffered_bytes)/8)*8          if final==0
 *                               (in_length+buffered_bytes)/8)*8 - pad     else, where 1<=pad<=8
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \param[in]      final        flag for unpadding
 *  \return         actEXCEPTION_LENGTH   *out_length is too small
 *                  actEXCEPTION_INPUT_LENGTH (in_length+buffered_bytes)%8!=0
 *                  actEXCEPTION_PADDING  PKCS 5 unpad error
 *                  actOK                 else
 *  \pre            actDESInitDecrypt() is called once before calling this function
 *                  in!=NULL, out_buffer!=NULL are a valid pointers
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actDESDecrypt(
  VSECPRIM_P2VAR_PARA(actDESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
  VSECPRIM_P2VAR_PARA(actU8) out_buffer, VSECPRIM_P2VAR_PARA(actLengthType) out_length,
  boolean final, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actDESInitEncryptBlock()
 *********************************************************************************************************************/
/*! \brief          This function initializes the DES block encryption.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to DES context structure
 *                               (out) initialized DES context structure
 *  \param[in]      key          pointer to user key
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \return         actEXCEPTION_NULL      if key parameter is NULL
 *                  actOK                  else
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actDESInitEncryptBlock(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info, VSECPRIM_P2CONST_PARA(actU8) key,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actDESInitDecryptBlock()
 *********************************************************************************************************************/
/*! \brief          This function initializes the DES block decryption.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to DES context structure
 *                               (out) initialized DES context structure
 *  \param[in]      key          pointer to user key
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \return         actEXCEPTION_NULL      if key parameter is NULL
 *                  actOK                  else
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actDESInitDecryptBlock(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info, VSECPRIM_P2CONST_PARA(actU8) key,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actDESCryptBlock()
 *********************************************************************************************************************/
/*! \brief          This function encrypts the given blocks in different modes.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to DES context structure
 *                               (out) modified DES context structure
 *  \param[in]      in_block     pointer to blocks to be encrypted
 *  \param[in,out]  out_block    (in) pointer to allocated output blocks
 *                               (out) encrypted output blocks
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actDESCryptBlock(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in_block, VSECPRIM_P2VAR_PARA(actU8) out_block,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actDESEncryptMultipleBlocks()
 *********************************************************************************************************************/
/*! \brief          This function encrypts the given blocks in different modes.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to DES context structure
 *                               (out) modified DES context structure
 *  \param[in]      in           pointer to blocks to be encrypted
 *  \param[in,out]  out_buffer   (in) pointer to allocated output blocks
 *                               (out) encrypted output blocks
 *  \param[in]      blocks       number of blocks
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actDESEncryptMultipleBlocks(
  VSECPRIM_P2VAR_PARA(actDESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU8) out_buffer,
  actLengthType blocks, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actDESDecryptMultipleBlocks()
 *********************************************************************************************************************/
/*! \brief          This function decrypts the given blocks in different modes.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to DES context structure
 *                               (out) modified DES context structure
 *  \param[in]      in           pointer to blocks to be decrypted
 *  \param[in,out]  out_buffer   (in) pointer to allocated output blocks
 *                               (out) decrypted output blocks
 *  \param[in]      blocks       number of blocks
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actDESDecryptMultipleBlocks(
  VSECPRIM_P2VAR_PARA(actDESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU8) out_buffer,
  actLengthType blocks, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actDESTransform()
 *********************************************************************************************************************/
/*! \brief          This function transforms input data of any length.
 *  \details        ((in_length+buffered_bytes)/8) blocks will be transformed and written
 *                  to out_buffer, ((in_length+buffered_bytes)%8) bytes will be buffered,
 *                  where 0 <= buffered_bytes < 8.
 *  \param[in,out]  info         (in) pointer to initialized DES context structure
 *                               (out) info->prev_block  updated previous block
 *  \param[in]      in           pointer to data to be transformed
 *  \param[in]      in_length    length of data in bytes
 *  \param[in,out]  out_buffer   (in) pointer to encrypted data buffer
 *                               (out) encrypted data blocks
 *  \param[in]      transform    pointer to transformation function (Encrypt/Decrypt)
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \pre            in!=NULL, out_buffer!=NULL are a valid pointers
 *                  in_length>=0
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actDESTransform(
  VSECPRIM_P2VAR_PARA(actDESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
  VSECPRIM_P2VAR_PARA(actU8) out_buffer, actDESTransformFctType transform,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTDES_H */

