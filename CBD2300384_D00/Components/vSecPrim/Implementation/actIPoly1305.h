/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2017 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  actIPoly1305.h
 *        \brief  Poly1305 implementation.
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
 ** Poly1305 programming interface
 **
 ** constants:
 **
 ** types:
 **   actPoly1305Context
 **
 ** macros:
 **
 ** functions:
 **   actPoly1305Init
 **   actPoly1305Update
 **   actPoly1305Finalize
 **
 ***************************************************************************/

#ifndef ACTIPoly1305_H
# define ACTIPoly1305_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"
# include "actBigNum.h"

 /* PRQA S 5209 EOF */ /* MD_VSECPRIM_USE_OF_BASIC_TYPES */

/****************************************************************************
 ** Types and constants
 ***************************************************************************/

typedef struct
{
  actU8 remains[16]; /* remaining data from last call */
  actU32 rLen; /* length of remains */

  actU8 opaque[140]; /* depends on core used: */
                     /* * cv needs 132 bytes, */
                     /* * AM needs (far) less (50...70) */
} actPoly1305Context;

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
 *  actPoly1305Init()
 *********************************************************************************************************************/
/*! \brief          This function initializes the Poly1305 authenticator.
 *  \details        -
 *  \param[in,out]  ctx          (in) pointer to Poly1305 context
 *                               (out) initialized context
 *  \param[in]      key          pointer to the key (256 bit / 32 byte)
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actPoly1305Init(VSECPRIM_P2VAR_PARA(actPoly1305Context) ctx,
  VSECPRIM_P2CONST_PARA(actU8) key);

/**********************************************************************************************************************
 *  actUpdatePoly1305()
 *********************************************************************************************************************/
/*! \brief          This function updates the Poly1305 authenticator.
 *  \details        -
 *  \param[in,out]  ctx          (in) pointer to ChaCha context
 *                               (out) updated context
 *  \param[in]      data         pointer to input data
 *  \param[in]      dLen         length of input data (bytes)
 *  \param[in]      watchdog     pointer to the watchdog function
 *  \pre            actPoly1305Init () has been called before calling this function
 *                  in != NULL is a valid pointer
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actPoly1305Update(VSECPRIM_P2VAR_PARA(actPoly1305Context) ctx,
  VSECPRIM_P2CONST_PARA(actU8) data,
  const actLengthType dLen,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actPoly1305HandleRemains()
 *********************************************************************************************************************/
/*! \brief          This function handles any remains stored in the Poly1305 authenticator.
 *  \details        -
 *  \param[in,out]  ctx          (in) pointer to ChaCha context
 *                               (out) updated context
 *  \param[in]      fillUp       != 0  ->  fill up block with zeroes
 *                               == 0  ->  use exact (shorter length)
 *  \param[in]      watchdog     pointer to the watchdog function
 *  \pre            actPoly1305Init () has been called before calling this function
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actPoly1305HandleRemains(VSECPRIM_P2VAR_PARA(actPoly1305Context) ctx,
  const actU8 fillUp,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actPoly1305Finalize()
 *********************************************************************************************************************/
/*! \brief          This function updates the Poly1305 authenticator.
 *  \details        -
 *  \param[in,out]  ctx          (in) pointer to ChaCha context
 *                               (out) updated context
 *  \param[in,out]  tag          (in) pointer to TAG buffer
 *                               sufficient size (16 bytes) is assumed!
 *                               (out) final tag
 *  \param[in]      watchdog     pointer to the watchdog function
 *  \pre            actPoly1305Init () has been called before calling this function
 *                  tag != NULL is a valid pointer
 *                  tag buffer is large enough (16 bytes)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actPoly1305Finalize(VSECPRIM_P2VAR_PARA(actPoly1305Context) ctx,
  VSECPRIM_P2VAR_PARA(actU8) tag,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif /* ACTIPoly1305_H */

/**********************************************************************************************************************
 *  END OF FILE: actIPoly1305.h
 *********************************************************************************************************************/
