/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2018 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  actIChaCha20.h
 *        \brief  ChaCha20 implementation.
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

/****************************************************************************
 **
 ** ChaCha20 programming interface
 **
 ** constants:
 **
 ** types:
 **   actChaCha20Context
 **
 ** macros:
 **
 ** functions:
 **   actChaCha20Init
 **   actChaCha20Cipher
 **
 ***************************************************************************/

#ifndef ACTIChaCha20_H
# define ACTIChaCha20_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"

 /* PRQA S 5209 EOF */ /* MD_VSECPRIM_USE_OF_BASIC_TYPES */

/****************************************************************************
 ** Types and constants
 ***************************************************************************/

typedef struct
{
  actU32 iState[16]; /* initial state */
  actU8 cipher[64];  /* current state (aka XOR cipher block) */
  actLengthType c;             /* index into cipher block */
} actChaCha20Context;

/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/**********************************************************************************************************************
 *  actChaCha20Init()
 *********************************************************************************************************************/
/*! \brief           This function initializes the ChaCha cipher (encryption as well as decryption).
 *  \details        -
 *  \param[in,out]  ctx          (in) pointer to ChaCha context
 *                               (out) initialized context
 *  \param[in]      key          pointer to the key   (256 bit / 32 byte)
 *  \param[in]      nonce        pointer to the nonce  (96 bit / 12 byte)
 *  \param[in]      bCount       initial block counter (32 bit)
 *  \param[in]      watchdog     pointer to the watchdog function
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC( void ) actChaCha20Init(VSECPRIM_P2VAR_PARA( actChaCha20Context ) ctx,
  VSECPRIM_P2CONST_PARA( actU8 ) key,
  VSECPRIM_P2CONST_PARA( actU8 ) nonce,
  const actU32 bCount,
  VSECPRIM_P2FUNC( VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actChaCha20Cipher()
 *********************************************************************************************************************/
/*! \brief          This function encrypts or decrypts input data of any length.
 *  \details        -
 *  \param[in,out]  ctx          (in) pointer to ChaCha context
 *                               (out) updated context
 *  \param[in]      in           pointer to input data
 *  \param[in]      iLen         length of input data (bytes)
 *  \param[in,out]  out          (in) pointer to output buffer
 *                               (out) encrypted data
 *  \param[in]      watchdog     pointer to the watchdog function
 *  \pre            actChaChaInit () has been called before calling this function
 *                  in != NULL, out != NULL are a valid pointers
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC( void ) actChaCha20Cipher(VSECPRIM_P2VAR_PARA( actChaCha20Context ) ctx,
  VSECPRIM_P2CONST_PARA( actU8 ) in,
  const actLengthType iLen,
  VSECPRIM_P2VAR_PARA( actU8 ) out,
  VSECPRIM_P2FUNC( VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actChaCha20NextBlock()
 *********************************************************************************************************************/
/*! \brief          This function calculates the next cipher block.
 *                  Used during AEAD_ChaCha20_Poly1305 initialization.
 *  \details        -
 *  \param[in,out]  ctx          (in) pointer to ChaCha context
 *                               (out) updated context
 *  \param[in]      watchdog     pointer to the watchdog function
 *  \pre            actChaChaInit () has been called before calling this function
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC( void ) actChaCha20NextBlock(VSECPRIM_P2VAR_PARA( actChaCha20Context ) ctx,
  VSECPRIM_P2FUNC( VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actChaCha20EatBlock()
 *********************************************************************************************************************/
/*! \brief          This function eats up the cipher block.
 *                  Used during AEAD_ChaCha20_Poly1305 initialization.
 *  \details        -
 *  \param[in,out]  ctx          (in) pointer to ChaCha context
 *                               (out) updated context
 *                               cipher block depleted
 *  \pre            actChaChaInit () has been called before calling this function
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC( void ) actChaCha20EatBlock(VSECPRIM_P2VAR_PARA( actChaCha20Context ) ctx);

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTIChaCha20_H */

/**********************************************************************************************************************
 *  END OF FILE: actIChaCha20.h
 *********************************************************************************************************************/
