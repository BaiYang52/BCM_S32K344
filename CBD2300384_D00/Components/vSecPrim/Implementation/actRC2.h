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
/*!        \file  actRC2.h
 *        \brief  This file contains the interface for the RC2 implementation.
 *
 *      \details  Currently the actClib version is used.
 *                This file is part of the embedded systems library cvActLib/ES
 *********************************************************************************************************************/
#ifndef ACTRC2_H
# define ACTRC2_H

# include "actITypes.h"

/****************************************************************************
 ** Types and constants
 ***************************************************************************/

# define RC2_MAX_KEY_LENGTH                                           128u   /* max user key length set to 128 byte  */
# define actRC2_MIN_KEY_SIZE                                          5u     /* min key size in bytes */
# define RC2_MASK                                                     0xffu  /* TM - mask based on eff. key length in bits - implementation requires effective_key modulo 8 !=0 */
# define MAX_ROUNDS                                                   16u    /* constant: 16 rounds */
# define actRC2_BLOCK_SIZE                                            8u     /* block size in bytes */


/* Context structure of the RC2 algorithm */
typedef struct
{
   actU16 e_key[RC2_MAX_KEY_LENGTH/2u];
   actLengthType key_word_len;
   actLengthType eff_key_word_len;
   actLengthType max_key_len;
   actU8 mode;
   actU8 prev_block[actRC2_BLOCK_SIZE];
   actLengthType buffer_used;
   actU8 buffer[actRC2_BLOCK_SIZE];
   actU8 copyInputData[actRC2_BLOCK_SIZE];
} actRC2STRUCT;

/* PRQA S 0850 3 */ /* MD_MSR_MacroArgumentEmpty */
typedef VSECPRIM_P2FUNC(VSECPRIM_NONE, void, actRC2TransformFctType)(
   VSECPRIM_P2VAR_PARA(actRC2STRUCT) info, VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU8) out_buffer,
   actLengthType blocks, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/****************************************************************************
 ** RC2 mode identifiers
 **
 **    actRC2_[mode_id]
 ***************************************************************************/
/* ECB */
# define actRC2_ECB                                                   (0x20u)
/* CBC */
# define actRC2_CBC                                                   (0x21u)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actRC2KeySetup()
 *********************************************************************************************************************/
/*! \brief          This function intializes info->e_key
 *  \details        -
 *  \param[in,out]  info         (in) initialized RC2 context structure
 *                               (out) modified RC2 context structure
 *  \param[in]      key          pointer to user key
 *  \param[in]      key_len      length of key in byte
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \return         actEXCEPTION_NULL      if key parameter is NULL
 *                  actEXCEPTION_LENGTH    if key_len is invalid
 *                  actOK                  else
 *  \pre            actRC2Init(info) is called once before calling this function
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actRC2KeySetup(
  VSECPRIM_P2VAR_PARA(actRC2STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));


/**********************************************************************************************************************
 *  actRC2Init()
 *********************************************************************************************************************/
/*! \brief          This function initializes the RC2 algorithm (key setup etc.).
 *  \details        -
 *  \param[in,out]  info         (in) pointer to RC2 context structure
 *                               (out) initialized RC2 context structure (initialized mode switch)
 *  \param[in]      key          pointer to user key
 *  \param[in]      key_len      length of key in byte
 *  \param[in]      eff_key_len  length of effective key in Byte
 *  \param[in]      iv           pointer to initialization vector (only in CBC mode;
 *                               of length actRC2_BLOCK_SIZE; if iv==NULL, a zeroized
 *                               block is used as iv)
 *  \param[in]      mode         ECB, CBC mode switch
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \return         actEXCEPTION_NULL      if key parameter is NULL
 *                  actEXCEPTION_LENGTH    if key_len is invalid
 *                  actEXCEPTION_ALGOID    mode is invalid
 *                  actOK                  else
 *  \pre            -
 *  \note           -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actRC2Init(
  VSECPRIM_P2VAR_PARA(actRC2STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len, actLengthType eff_key_len,
  VSECPRIM_P2CONST_PARA(actU8) iv, actU8 mode,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));


/**********************************************************************************************************************
 *  actRC2Encrypt()
 *********************************************************************************************************************/
/*! \brief          This function encrypts input data of any length.
 *  \details        If final==0, ((in_length+buffered_bytes)/8) blocks will be encrypted
 *                  and written to out_buffer, ((in_length+buffered_bytes)%8) bytes will
 *                  be buffered, where 0 <= buffered_bytes < 8.
 *                  If final!=0, (((in_length+buffered_bytes)/8)+1) blocks will be
 *                  encrypted and written to out_buffer. PKCS 5 padding is used for the
 *                  last block padding.
 *  \param[in,out]  info         (in) pointer to initialized RC2 context structure
 *                               (out) info->prev_block updated previous block
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
 *  \pre            actRC2Init() is called once before calling this function
 *                  in!=NULL, out_buffer!=NULL are valid pointers
 *                  in_length>=0, *out_length>=0 (at input)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actRC2Encrypt(
  VSECPRIM_P2VAR_PARA(actRC2STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
  VSECPRIM_P2VAR_PARA(actU8) out_buffer, VSECPRIM_P2VAR_PARA(actLengthType) out_length,
  boolean final, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));


/**********************************************************************************************************************
 *  actRC2Decrypt()
 *********************************************************************************************************************/
/*! \brief          This function decrypts input data of any length.
 *  \details        If final==0, ((in_length+buffered_bytes)/8) blocks will be decrypted
 *                  and written to out_buffer, ((in_length+buffered_bytes)%8) bytes will
 *                  be buffered, where 0 <= buffered_bytes < 8.
 *                  If final!=0, ((in_length+buffered_bytes)/8) blocks will be decrypted
 *                  and written to out_buffer, where (in_length+buffered_bytes) is a
 *                  multiple of 8. PKCS 5 padding is used to unpad the last block.
 *  \param[in,out]  info         (in) pointer to initialized RC2 context structure
 *                               (out) info->prev_block updated previous block
 *  \param[in]      in           pointer to data to be decrypted
 *  \param[in]      in_length    length of data in bytes (multiple of 8)
 *  \param[in,out]  out_buffer   (in) pointer to decrypted data buffer of length in_length
 *                               (out) decrypted data blocks
 *  \param[in,out]  out_length   (in) out_buffer size in bytes, a safe length is:
 *                               ((in_length+7)/8)*8       (see output)
 *                               (out) number of decrypted bytes:
 *                               ((in_length+buffered_bytes)/8)*8          if final==0
 *                               ((in_length+buffered_bytes)/8)*8 - pad    else, where 1<=pad<=8
 *  \param[in]      final        flag for unpadding
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \return         actEXCEPTION_LENGTH   *out_length is too small
 *                  f final!=0) actEXCEPTION_INPUT_LENGTH (in_length+buffered_bytes)%8!=0
 *                  actEXCEPTION_PADDING  PKCS 5 unpad error
 *                  actOK                 else
 *  \pre            actRC2Init() is called once before calling this function
 *                  in!=NULL, out_buffer!=NULL are a valid pointers
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actRC2Decrypt(
  VSECPRIM_P2VAR_PARA(actRC2STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
  VSECPRIM_P2VAR_PARA(actU8) out_buffer, VSECPRIM_P2VAR_PARA(actLengthType) out_length,
  boolean final, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actRC2InitCryptBlock()
 *********************************************************************************************************************/
/*! \brief          This function initializes the RC2 block encryption.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to RC2 context structure
 *                               (out) initialized RC2 context structure
 *  \param[in]      key          pointer to user key
 *  \param[in]      key_len      length of key in byte
 *  \param[in]      eff_key_len  length of effective key in byte
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \return         actEXCEPTION_NULL      if key parameter is NULL
 *                  actEXCEPTION_LENGTH    if key_len is invalid
 *                  actOK                  else
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actRC2InitCryptBlock(
  VSECPRIM_P2VAR_PARA(actRC2STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len, actLengthType eff_key_len,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));


/**********************************************************************************************************************
 *  actRC2EncryptBlock()
 *********************************************************************************************************************/
/*! \brief          This function encrypts the given block.
 *  \details        -
 *  \param[in]      info         info->e.key: pointer to expanded encryption key
 *  \param[in]      in_block     pointer to block to be encrypted
 *  \param[in,out]  out_block    (in) pointer to allocated output block
 *                               (out) encrypted output block
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \pre            actRC2InitCryptBlock(info) is called once before calling this function
 *                  'in_block' and 'out_block' are of size actRC2_BLOCK_SIZE
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void)  actRC2EncryptBlock(
   VSECPRIM_P2CONST_PARA(actRC2STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in_block, VSECPRIM_P2VAR_PARA(actU8) out_block,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));


/**********************************************************************************************************************
 *  actRC2DecryptBlock()
 *********************************************************************************************************************/
/*! \brief          This function decrypts the given block.
 *  \details        -
 *  \param[in]      info         info->e_key: pointer to expanded decryption key
 *  \param[in]      in_block     pointer to block to be decrypted
 *  \param[in,out]  out_block    (in) pointer to allocated output block
 *                               (out) decrypted output block
 *  \param[in]      watchdog     Watchdog trigger function pointer
 *  \pre            actRC2InitDecryptBlock(info) is called once before calling this function
 *                  'in_block' and 'out_block' are of size actRC2_BLOCK_SIZE
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actRC2DecryptBlock(
   VSECPRIM_P2CONST_PARA(actRC2STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in_block, VSECPRIM_P2VAR_PARA(actU8) out_block,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));


/**********************************************************************************************************************
 *  actRC2EncryptMultipleBlocks()
 *********************************************************************************************************************/
/*! \brief          This function encrypts the given blocks in different modes.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to RC2 context structure
 *                               (out) modified RC2 context structure
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
VSECPRIM_FUNC(void) actRC2EncryptMultipleBlocks(
  VSECPRIM_P2VAR_PARA(actRC2STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU8) out_buffer,
  actLengthType blocks, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));


/**********************************************************************************************************************
 *  actRC2DecryptMultipleBlocks()
 *********************************************************************************************************************/
/*! \brief          This function decrypts the given blocks in different modes.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to RC2 context structure
 *                               (out) modified RC2 context structure
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
VSECPRIM_FUNC(void) actRC2DecryptMultipleBlocks(
  VSECPRIM_P2VAR_PARA(actRC2STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU8) out_buffer,
  actLengthType blocks, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));


/**********************************************************************************************************************
 *  actRC2Transform()
 *********************************************************************************************************************/
/*! \brief          This function transforms input data of any length.
 *  \details        This function transforms input data of any length.
 *                  ((in_length+buffered_bytes)/8) blocks will be transformed and written
 *                  to out_buffer, ((in_length+buffered_bytes)%8) bytes will be buffered,
 *                  where 0 <= buffered_bytes < 8.
 *  \param[in,out]  info         (in) pointer to initialized RC2 context structure
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
VSECPRIM_FUNC(void) actRC2Transform(
   VSECPRIM_P2VAR_PARA(actRC2STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
   VSECPRIM_P2VAR_PARA(actU8) out_buffer, actRC2TransformFctType transform,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif
