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
/*!        \file  actX25519core.h
 *        \brief  Interface to Core Curve25519 implementation..
 *
 *      \details This file is part of the embedded systems library cvActLib
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

#ifndef ACTX25519CORE_H
# define ACTX25519CORE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "act25519util.h"
# include "actIX25519.h"

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/**********************************************************************************************************************
 *  actX25519_initCore()
 *********************************************************************************************************************/
/*! \brief       Initialize the core.
 *  \details     This function initializes the internal data structures.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \pre         ws must be a valid pointer
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actX25519_initCore(VSECPRIM_P2VAR_PARA(actX25519STRUCT) ws);

/**********************************************************************************************************************
 *  actX25519_eval()
 *********************************************************************************************************************/
/*! \brief       Evaluate Curve25519
 *  \details     This function evaluates the raw Curve25519 function. The private key and u coordinate(s) are passed
 *               in the work-space.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[in]      watchdog     pointer to the watchdog function
 *  \return      actOK  -  actOK is always returned.
 *  \pre         ws must be a valid pointer
 *               ws must be initialized
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actX25519_eval(
  VSECPRIM_P2VAR_PARA(actX25519STRUCT) ws,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
}
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTX25519CORE_H */

/**********************************************************************************************************************
 *  END OF FILE: actX25519core.h
 *********************************************************************************************************************/
