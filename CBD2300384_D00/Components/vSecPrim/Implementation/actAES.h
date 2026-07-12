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
/*!        \file  actAES.h
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

#ifndef ACTAES_H
# define ACTAES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actIAES.h"

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* PRQA S 0850 6 */ /* MD_MSR_MacroArgumentEmpty */
typedef VSECPRIM_P2FUNC(VSECPRIM_NONE, void, actAESTransformFctType) (
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in,
  VSECPRIM_P2VAR_PARA(actU8) out_buffer,
  actLengthType blocks,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * actAESEncryptionRound()
 *********************************************************************************************************************/
/*! \brief       AES encryption round function
 *  \details     Performs an AES encryption round with the given parameters
 *  \param[out]  bo Block output pointer
 *  \param[in]   bi Block input pointer
 *  \param[in]   k  Key pointer
 *  \pre         b0, bi and k must point to valid memory of at least 16 Bytes.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actAESEncryptionRound(
  VSECPRIM_P2VAR_PARA(actU32) bo, VSECPRIM_P2CONST_PARA(actU32) bi, VSECPRIM_P2CONST_PARA(actU32) k);

/**********************************************************************************************************************
 * actAESDecryptionRound()
 *********************************************************************************************************************/
/*! \brief       AES decryption round function
 *  \details     Performs an AES decryption round with the given parameters
 *  \param[out]  bo Block output pointer
 *  \param[in]   bi Block input pointer
 *  \param[in]   k  Key pointer
 *  \pre         b0, bi and k must point to valid memory of at least 16 Bytes.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actAESDecryptionRound(
  VSECPRIM_P2VAR_PARA(actU32) bo, VSECPRIM_P2CONST_PARA(actU32) bi, VSECPRIM_P2CONST_PARA(actU32) k);

/**********************************************************************************************************************
 * actAESEncryptionFinalRound()
 *********************************************************************************************************************/
/*! \brief       AES encryption final round function
 *  \details     Performs the final AES encryption round with the given parameters
 *  \param[out]  bo Block output pointer
 *  \param[in]   bi Block input pointer
 *  \param[in]   k  Key pointer
 *  \pre         b0, bi and k must point to valid memory of at least 16 Bytes.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actAESEncryptionFinalRound(
  VSECPRIM_P2VAR_PARA(actU32) bo, VSECPRIM_P2CONST_PARA(actU32) bi, VSECPRIM_P2CONST_PARA(actU32) k);

/**********************************************************************************************************************
 * actAESDecryptionFinalRound()
 *********************************************************************************************************************/
/*! \brief       AES decryption final round function
 *  \details     Performs the final AES decryption round with the given parameters
 *  \param[out]  bo Block output pointer
 *  \param[in]   bi Block input pointer
 *  \param[in]   k  Key pointer
 *  \pre         b0, bi and k must point to valid memory of at least 16 Bytes.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actAESDecryptionFinalRound(
  VSECPRIM_P2VAR_PARA(actU32) bo, VSECPRIM_P2CONST_PARA(actU32) bi, VSECPRIM_P2CONST_PARA(actU32) k);

/**********************************************************************************************************************
 *  actAESKeySetup()
 *********************************************************************************************************************/
/*! \brief       This function initializes info->e_key.
 *  \details     AES key expansion function.
 *  \param[in,out] info    In: Initialized AES context structure Out: Modified AES context structure
 *  \param[in]     key     Pointer to user key
 *  \param[in]     key_len Length of key in byte (16, 24 or 32)
 *  \param[in]     watchdog Pointer to watchdog trigger function
 *  \return      actEXCEPTION_NULL      if key parameter is NULL
 *               actEXCEPTION_LENGTH    if key_len is not 16
 *               actOK                  else
 *  \pre         actAESInit(info) is called once before calling this function
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actAESKeySetup(
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info, VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void)); /* PRQA S 0850 */ /* MD_MSR_MacroArgumentEmpty */

/**********************************************************************************************************************
 *  actAESMakeDecryptionKey()
 *********************************************************************************************************************/
/*! \brief       Prepares the key for decryption
 *  \details     This function transforms the intialized encryption key info->e_key into a decryption key.
 *  \param[in,out] info    In: Initialized AES context structure Out: Modified AES context structure
 *  \pre         actAESKeySetup(info) is called once before calling this function
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actAESMakeDecryptionKey(VSECPRIM_P2VAR_PARA(actAESSTRUCT) info);

/**********************************************************************************************************************
 *  actAESInit()
 *********************************************************************************************************************/
/*! \brief       Initialize AES
 *  \details     This function initializes the AES algorithm (key setup etc.).
 *  \param[in,out] info    In: Pointer to AES context structure Out: Initialized AES context structure
 *  \param[in]     key     Pointer to user key
 *  \param[in]     key_len Length of key in byte (16, 24 or 32)
 *  \param[in]     iv      Pointer to initialization vector (only in CBC and CTR mode; of length actAES_BLOCK_SIZE;
 *                         if iv==NULL, a zeroized block is used as iv)
 *  \param[in]     mode    ECB, CBC, CTR mode switch
 *  \param[in]     watchdog Pointer to watchdog trigger function
 *  \return      actEXCEPTION_NULL      if key parameter is NULL
 *  \return      actEXCEPTION_LENGTH    if key_len is not 16
 *  \return      actEXCEPTION_ALGOID    mode is invalid
 *  \return      actOK                  else
 *  \pre         info must be a valid workspace pointer
 *               IV has to be blocksize, only applies for CBC block and CTR mode
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actAESInit(
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len, VSECPRIM_P2CONST_PARA(actU8) iv,
  actU8 mode, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void)); /* PRQA S 0850 */ /* MD_MSR_MacroArgumentEmpty */

/**********************************************************************************************************************
 *  actAESEncryptMultipleBlocks()
 *********************************************************************************************************************/
/*! \brief       AES encrypt multiple blocks
 *  \details     This function performs AES encryption on multiple input blocks in different modes
 *  \param[in,out] info    Pointer to AES context structure
 *  \param[in]     in      Pointer to blocks to be encrypted
 *  \param[in,out] out_buffer In: Pointer to blocks to be encrypted Out: Encrypted output blocks
 *  \param[in]     blocks  Number of blocks
 *  \param[in]     watchdog Pointer to watchdog trigger function
 *  \pre         out buffer must point to a buffer with at least 16 bytes.
 *               info must be a valid workspace pointer
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actAESEncryptMultipleBlocks(
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU8) out_buffer,
  actLengthType blocks, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void)); /* PRQA S 0850 */ /* MD_MSR_MacroArgumentEmpty */

/**********************************************************************************************************************
 *  actAESDecryptMultipleBlocks()
 *********************************************************************************************************************/
/*! \brief       AES decrypt multiple blocks
 *  \details     This function performs AES decryption on multiple input blocks in different modes
 *  \param[in,out] info    Pointer to AES context structure
 *  \param[in]     in      Pointer to blocks to be decrypted
 *  \param[in,out] out_buffer In: Pointer to blocks to be decrypted Out: Decrypted output blocks
 *  \param[in]     blocks  Number of blocks
 *  \param[in]     watchdog Pointer to watchdog trigger function
 *  \pre         info must be a valid workspace pointer
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actAESDecryptMultipleBlocks(
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU8) out_buffer,
  actLengthType blocks, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void)); /* PRQA S 0850 */ /* MD_MSR_MacroArgumentEmpty */

/**********************************************************************************************************************
 *  actAESTransform()
 *********************************************************************************************************************/
/*! \brief       AES transformation function
 *  \details     This function transforms input data of any length. ((in_length+buffered_bytes)/16) blocks will
 *               be transformed and written to out_buffer, ((in_length+buffered_bytes)%16) bytes will be buffered,
 *               where 0 <= buffered_bytes < 16.
 *  \param[in,out] info    In: Pointer to initialized AES context structure Out: Updated previous block
 *                         (info->prev_block)
 *  \param[in]     in      Pointer to blocks to be transformed
 *  \param[in]     in_length  Length of data in bytes
 *  \param[in,out] out_buffer In: Pointer to encrypted data buffer Out: Encrypted data blocks, buffer size depends on in_length
 *  \param[in]     transform  Pointer to transformation function (Encrypt/Decrypt)
 *  \param[in]     watchdog Pointer to watchdog trigger function
 *  \pre         info must be a valid workspace pointer
 *               transform must be a valid function pointer
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actAESTransform(
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
  VSECPRIM_P2VAR_PARA(actU8) out_buffer, actAESTransformFctType transform,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void)); /* PRQA S 0850 */ /* MD_MSR_MacroArgumentEmpty */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTAES_H */

/**********************************************************************************************************************
 *  END OF FILE: actAES.h
 *********************************************************************************************************************/
