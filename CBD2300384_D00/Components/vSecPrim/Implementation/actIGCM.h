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
/*!        \file  actIGCM.h
 *        \brief  Interface of GCM implementation.
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

#ifndef ACTIGCM_H
# define ACTIGCM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"
# include "actIAES.h"
# include "actIGHash.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define STATE_AAD                                                    0u
# define STATE_CIPHERTEXT                                             1u

# define MODE_ENC                                                     0u
# define MODE_DEC                                                     1u

# define AAD_HI                                                       0
# define AAD_LO                                                       1
# define Data_HI                                                      2
# define Data_LO                                                      3

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

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
 *  actGCMInit()
 *********************************************************************************************************************/
/*! \brief          Initializes the Galois/Counter-Mode.
 *  \details        This function initializes the Galois/Counter-Mode.
 *  \param[in,out]  info  (in)  pointer to context structure
 *                        (out) initialized context structure
 *  \param[in]      key  master key
 *  \param[in]      key_length  length of key in bytes
 *  \param[in]      iv  initialization vector
 *  \param[in]      iv_length  length of the initialization vector (max 2^32 bit)
 *  \param[in]      watchdog  pointer to watchdog reset function
 *  \return         actEXCEPTION_LENGTH    if key_len < 1
 *                  actOK                  else
 *  \pre            info is a valid workspace pointer
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actGCMInit (
   VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key,
   actLengthType key_len,
   VSECPRIM_P2CONST_PARA(actU8) IV,
   actLengthType IV_len,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actGCMTransform()
 *********************************************************************************************************************/
/*! \brief          Encrypts/decrypts any amount of data.
 *  \details        This function encrypts/decrypts any amount of data
 *                  and can be called arbitrarily often. It returns, how many
 *                  bytes have actually been handled so far.
 *  \param[in,out]  info  pointer to context structure
 *  \param[in]      input  input data
 *  \param[in]      in_len  length of input in bytes
 *  \param[out]     output  pointer to output buffer for the encrypted/decrypted data
 *  \param[in,out]  bytes_out  pointer to output counter, amount of bytes already handled
 *  \param[in]      encORdec  switch to distinguish between encryption and decryption
 *  \param[in]      watchdog  pointer to watchdog reset function
 *  \return         actEXCEPTION_LENGTH   aad length exceeds 2^64 - 1 bit or
 *                                        plaintext/ciphertext  2^39 - 256 bit
 *                  actOK                 else
 *  \pre            info is a valid workspace pointer,
 *                  workspace is initialized,
 *                  in state P / C output references a valid memory location whose size is a multiple of the block size and greater or equal than inputLength,
 *                  bytes_out is a valid pointer
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actGCMTransform (
   VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) input,
   const actLengthType in_len,
   VSECPRIM_P2VAR_PARA(actU8) output,
   VSECPRIM_P2VAR_PARA(actLengthType) bytes_out,
   const uint8 encORdec,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actGCMEncryptUpdate()
 *********************************************************************************************************************/
/*! \brief          Encrypts any amount of data.
 *  \details        This function encrypts any amount of data
 *                  and can be called arbitrarily often. It returns, how many
 *                  bytes have actually been handled so far.
 *  \param[in,out]  info  pointer to context structure
 *  \param[in]      X  input
 *  \param[in]      X_len  length of input in bytes
 *  \param[out]     output  pointer to output buffer for the encrypted data
 *  \param[in,out]  bytes_out  pointer to output counter, amount of bytes already handled
 *  \param[in]      watchdog  pointer to watchdog reset function
 *  \return         return value of actGCMTransform
 *  \pre            info is a valid workspace pointer,
 *                  workspace is initialized,
 *                  in state P / C output references a valid memory location whose size is a multiple of the block size and greater or equal than inputLength,
 *                  bytes_out is a valid pointer
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actGCMEncryptUpdate (
   VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) X,
   const actLengthType X_len,
   VSECPRIM_P2VAR_PARA(actU8) output,
   VSECPRIM_P2VAR_PARA(actLengthType) bytes_out,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actGCMDecryptUpdate()
 *********************************************************************************************************************/
/*! \brief          Decrypts any amount of data.
 *  \details        This function decrypts any amount of data
 *                  and can be called arbitrarily often. It returns, how many
 *                  bytes have actually been handled so far.
 *  \param[in,out]  info  pointer to context structure
 *  \param[in]      X  input
 *  \param[in]      X_len  length of input in bytes
 *  \param[out]     output  pointer to output buffer for the decrypted data
 *  \param[in,out]  bytes_out  pointer to output counter, amount of bytes already handled
 *  \param[in]      watchdog  pointer to watchdog reset function
 *  \return         return value of actGCMTransform
 *  \pre            info is a valid workspace pointer,
 *                  workspace is initialized,
 *                  in state P / C output references a valid memory location whose size is a multiple of the block size and greater or equal than inputLength,
 *                  bytes_out is a valid pointer
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actGCMDecryptUpdate (
   VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) X,
   const actLengthType X_len,
   VSECPRIM_P2VAR_PARA(actU8) output,
   VSECPRIM_P2VAR_PARA(actLengthType) bytes_out,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actGCMTriggerState()
 *********************************************************************************************************************/
/*! \brief          Switches the internal state from AAD to P / C.
 *  \details        This function switches the internal state from AAD to P / C,
 *                  so actGCM*Update now expects plaintext/ciphertext.
 *  \param[in,out]  info  (in)  pointer to context structure
 *                        (out) updated GHash and internal buffers
 *  \param[in]      watchdog  pointer to watchdog reset function
 *  \return         actEXCEPTION_UNKNOWN  if called from an infalid state
 *                                        (everything other than STATE_AAD)
 *                  actOK                 else
 *  \pre            info must be a valid workspace pointer,
 *                  workspace must be initialized
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actGCMTriggerState (
   VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actGMACFinalize()
 *********************************************************************************************************************/
/*! \brief          Finishes the plaintext / ciphertext section and computes the tag.
 *  \details        This function finishes the plaintext / ciphertext section and computes the tag.
 *                  If there is still data in the buffer, it is written to output.
 *  \param[in,out]  info  pointer to context structure
 *  \param[out]     tag  the authentication tag
 *  \param[in]      watchdog  pointer to watchdog reset function
 *  \return         actOK always
 *  \pre            info is a valid workspace pointer,
 *                  workspace is initialized,
 *                  tag references a valid memory location whose size is greater or equal than 16 bytes
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actGMACFinalize (
   VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
   VSECPRIM_P2VAR_PARA(actU8) tag,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actGCMEncryptFinalize()
 *********************************************************************************************************************/
/*! \brief          Finalizes encryption and returns the tag.
 *  \details        This function finalizes encryption and returns the tag.
 *                  If there is still data in the buffer, it is written to output.
 *  \param[in,out]  info  pointer to context structure
 *  \param[out]     output  pointer to output buffer for remaining data bytes
 *  \param[in,out]  bytes_out  pointer to output counter, incremented by no. of remaining bytes
 *  \param[out]     tag  the authentication tag
 *  \param[in]      watchdog  pointer to watchdog reset function
 *  \return         actOK always
 *  \pre            info is a valid workspace pointer,
 *                  workspace is initialized,
 *                  output references a valid memory location whose size is greater or equal than the block size (16 bytes),
 *                  bytes_out is a valid pointer
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actGCMEncryptFinalize (
   VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
   VSECPRIM_P2VAR_PARA(actU8) output,
   VSECPRIM_P2VAR_PARA(actLengthType) bytes_out,
   VSECPRIM_P2VAR_PARA(actU8) tag,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actGCMDecryptFinalize()
 *********************************************************************************************************************/
/*! \brief          Finalizes decryption and returns the tag.
 *  \details        This function finalizes decryption and returns the tag.
 *                  If there is still data in the buffer, it is written to output.
 *  \param[in,out]  info  pointer to context structure
 *  \param[out]     output  pointer to output buffer for remaining data bytes
 *  \param[in,out]  bytes_out  pointer to output counter, incremented by no. of remaining bytes
 *  \param[out]     tag  the authentication tag
 *  \param[in]      watchdog  pointer to watchdog reset function
 *  \return         actOK always
 *  \pre            info is a valid workspace pointer,
 *                  workspace is initialized,
 *                  output references a valid memory location whose size is greater or equal than the GCM block size (16 bytes),
 *                  bytes_out is a valid pointer
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actGCMDecryptFinalize (
   VSECPRIM_P2VAR_PARA(actGCMSTRUCT) info,
   VSECPRIM_P2VAR_PARA(actU8) output,
   VSECPRIM_P2VAR_PARA(actLengthType) bytes_out,
   VSECPRIM_P2VAR_PARA(actU8) tag,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
}                               /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTIGCM_H */
