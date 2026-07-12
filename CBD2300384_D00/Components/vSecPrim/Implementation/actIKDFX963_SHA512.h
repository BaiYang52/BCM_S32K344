/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  actIKDFX963_SHA512.h
 *        \brief  KDFX963 key derivation function according to ANSI X9.63 using SHA-512 as underlying hash function.
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library cvActLib/ES
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

#ifndef ACTIKDFX963SHA512_H
# define ACTIKDFX963SHA512_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"
# include "actISHA2_64.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C" {
# endif

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  actKDFX963_SHA512_Derive
 **********************************************************************************************************************/
/*! \brief         Derive Key according KDF X9.63 with SHA512
 *  \details       This function derives a key according to KDF X9.63 with SHA512.
 *                 This function derives a key of the desired length from the input password and the (optional) salt.
 *  \param[in,out] info           KDF X9.63 with SHA512 work space
 *  \param[in]     passwd         Pointer to the secret
 *  \param[in]     passwd_length  length of secret in bytes
 *  \param[in]     salt           pointer to the optional salt (if NULL, no salt is used)
 *  \param[in]     salt_length    length of salt in bytes (if zero, no salt is used)
 *  \param[out]    key            pointer to the output key buffer
 *  \param[in]     key_length     length of output key buffer in bytes
 *  \param[in]     watchdog       pointer to watchdog reset function
 *  \pre           Pointers must be valid
 *                 workSpace is initialized by esl_initKDFX963SHA512(..)
 *                 key need to have at least the length of keyLength.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_FUNC(void) actKDFX963_SHA512_Derive(
   VSECPRIM_P2VAR_PARA(actKDFX963SHA512) info,
   VSECPRIM_P2CONST_PARA(actU8) passwd, actLengthType passwd_length,
   VSECPRIM_P2CONST_PARA(actU8) salt, actLengthType salt_length,
   VSECPRIM_P2VAR_PARA(actU8) key, actLengthType key_length,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

#endif /* ACTIKDFX963SHA512_H */

