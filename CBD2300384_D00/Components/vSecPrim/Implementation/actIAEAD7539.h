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
/*!        \file  actIAEAD7539.h
 *        \brief  AEAD7539 implementation.
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
 ** AEAD_ChaCha20_Poly1305 (RFC 7539) programming interface
 **
 ** constants:
 **
 ** types:
 **   actAEAD7539Context
 **
 ** macros:
 **
 ** functions:
 **   actAEAD7539Init
 **   actAEAD7539Update
 **   actAEAD7539SwitchToData
 **   actAEAD7539Finalze
 **
 ***************************************************************************/


#ifndef ACTIAEAD7539_H
# define ACTIAEAD7539_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"
# include "actIChaCha20.h"
# include "actIPoly1305.h"

 /* PRQA S 5209 EOF */ /* MD_VSECPRIM_USE_OF_BASIC_TYPES */

/****************************************************************************
 ** Types and constants
 ***************************************************************************/

# define PHASE                                                        1u
# define PHASE_AAD                                                    0u
# define PHASE_DATA                                                   1u
# define DIRECTION                                                    2u
# define DIRECTION_ENCRYPT                                            0u
# define DIRECTION_DECRYPT                                            2u


typedef struct
{
  actU32 mode;
  actU8 data[16];
  actU32 aadLenLO, aadLenHI;
  actU32 dataLenLO, dataLenHI;
  actChaCha20Context cc20;
  actPoly1305Context p130;
} actAEAD7539Context;



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
 *  actAEAD7539Init()
 *********************************************************************************************************************/
/*! \brief          This function initializes the AEAD_ChaCha20_Poly1305 (RFC 7539)
 *                  authenticated cipher (encryption as well as decryption).
 *  \details        -
 *  \param[in,out]  ctx          (in) pointer to AEAD context
 *                               (out) initialized context, the operating mode is switched to "AAD"
 *  \param[in]      key          pointer to the key   (256 bit / 32 byte)
 *  \param[in]      nonce        pointer to the nonce  (96 bit / 12 byte)
 *  \param[in]      mode         encryption / decryption mode
 *                               any value != MODE_ENCRYPT will be interpreted as MODE_DECRYPT!
 *  \param[in]      watchdog     pointer to the watchdog function
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actAEAD7539Init(VSECPRIM_P2VAR_PARA(actAEAD7539Context) ctx,
  VSECPRIM_P2CONST_PARA(actU8) key,
  VSECPRIM_P2CONST_PARA(actU8) nonce,
  const actU32 mode,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));


/**********************************************************************************************************************
 *  actAEAD7539Update()
 *********************************************************************************************************************/
/*! \brief          This function updates data in the AEAD_ChaCha20_Poly1305 (RFC 7539)
 *                  authenticated cipher (encryption as well as decryption).
 *  \details        Depending on the operating mode, different data are passed / returned:
 *                  * AAD: AAD are passed in "in" (and "iLen");
 *                        "out" is not used (and may be NULL).
 *                  * ENcryption: the plaintext is passed in "in" (and "iLen");
 *                                the ciphertext is returned in "out";
 *                                "out" is assumed to be large enough!
 *                  * DEcryption: the ciphertext is passed in "in" (and "iLen");
 *                                the plaintext is returned in "out";
 *                               "out" is assumed to be large enough!
 *  \param[in,out]  ctx          (in) pointer to AEAD 7539 context
 *                               (out) updated context
 *  \param[in]      in           pointer to input data
 *  \param[in]      iLen         length of input data (bytes)
 *  \param[in,out]  out          (in) pointer to output buffer
 *                               (out) encrypted / decrypted data
 *  \param[in]      watchdog     pointer to the watchdog function
 *  \return         actEXCEPTION_LENGTH when AAD or Data overflows (> 2^64 -1)
 *                  actEXCEPTION_MODE when the operating mode is invalid
 *                  actOK else
 *  \pre            actAEAD7539Init () has been called before calling this function
 *                  in != NULL, out != NULL are a valid pointers (out not used in case of AAD)
 *                  NULL pointers or sizes will NOT be checked!
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actAEAD7539Update(VSECPRIM_P2VAR_PARA(actAEAD7539Context) ctx,
  VSECPRIM_P2CONST_PARA(actU8) in,
  const actLengthType iLen,
  VSECPRIM_P2VAR_PARA(actU8) out,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));


/**********************************************************************************************************************
 *  actAEAD7539SwitchToData()
 *********************************************************************************************************************/
/*! \brief          This function switches operating mode of the AEAD_ChaCha20_Poly1305
 *                  (RFC 7539) cipher from "AAD" to "ENcryption" / "DEcryption".
 *  \details        -> remaining AAD data is padded and authenticated
 *                  -> actAEAD7539Update now expects the plaintext / ciphertext.
 *                  The switch is UNCONDITIONAL, the phase is NOT verified to be "AAD"!
 *  \param[in,out]  ctx          (in) pointer to AEAD 7539 context
 *                               (out) updated context
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actAEAD7539SwitchToData(VSECPRIM_P2VAR_PARA(actAEAD7539Context) ctx);


/**********************************************************************************************************************
 *  actAEAD7539Finalize()
 *********************************************************************************************************************/
/*! \brief          This function calculates and retruns the authentication TAG
 *                  of the AEAD_ChaCha20_Poly1305 (RFC 7539) cipher.
 *  \details        -
 *  \param[in,out]  ctx          (in) pointer to AEAD 7539 context
 *                               (out) updated context
 *  \param[in,out]  tag          (in)  pointer referencing a buffer to store the output tag
 *                               (out) pointer referencing a buffer storing the output tag
 *  \param[in]      watchdog     pointer to the watchdog function
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actAEAD7539Finalize(VSECPRIM_P2VAR_PARA(actAEAD7539Context) ctx,
  VSECPRIM_P2VAR_PARA(actU8) tag,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));


# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTIAEAD7539_H */

/**********************************************************************************************************************
 *  END OF FILE: actIAEAD7539.h
 *********************************************************************************************************************/
