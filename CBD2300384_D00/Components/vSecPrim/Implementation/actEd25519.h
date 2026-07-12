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
/*!        \file  actEd25519.h
 *        \brief  Ed25519 helper functions
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

#ifndef ACTED25519_H
# define ACTED25519_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"
# include "actIEd25519.h"
# include "actISHA2_64.h"

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
 *  act25519_hashPM()
 *********************************************************************************************************************/
/*! \brief       Hash the private key and message
 *  \details     This function performs step 2 of the Ed25519 signature algorithm.
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.6
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[out]  hash  Pointer to the output buffer for the hash value
 *  \param[in]   watchdog  Pointer to watchdog trigger function.
 *  \return      actOK  -  actOK is always returned.
 *  \pre         Hash must be at least size of info->result_length. Full hash is 64 byte.
 *               ws, hash must be valid pointers
 *               ws must be initialized
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) act25519_hashPM(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act25519_hashRAM()
 *********************************************************************************************************************/
/*! \brief       Hash R, A and the Message
 *  \details     This function performs step 4 of the Ed25519 signature algorithm
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.6
 *               or step 2 of the Ed25519 verification algorithm
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.7
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[in]   R  Pointer to the R value.
 *  \param[out]  hash  Pointer to the output buffer for the hash value.
 *  \param[in]   watchdog  Pointer to watchdog trigger function.
 *  \return      actOK  -  actOK is always returned.
 *  \pre         Hash must be at least size of info->result_length. Full hash is 64 byte.
 *               ws, hash must be valid pointers
 *               ws must be initialized
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) act25519_hashRAM(
  VSECPRIM_P2VAR_PARA(actEd25519STRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) R,
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
}                               /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /*  ACTED25519_H */

/**********************************************************************************************************************
 *  END OF FILE: actEd25519.h
 *********************************************************************************************************************/
