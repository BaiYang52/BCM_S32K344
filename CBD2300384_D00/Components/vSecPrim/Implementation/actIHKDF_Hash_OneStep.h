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
/*!        \file  actIHKDF_Hash_OneStep.h
 *        \brief  HKDF_key derivation function according to ANSI X9.63 using SHA-256 as underlying hash function.
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

#ifndef ACTIHKDFHASH_ONESTEP_H
# define ACTIHKDFHASH_ONESTEP_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"
# include "actISHA2_32.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C" {
# endif

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  actHKDF_Hash_OneStepDerive
 **********************************************************************************************************************/
/*! \brief         Derive Key according one-step HKDF based on SHA.
 *  \details       This function derives a key of length derivedKeyLength according to the hash based one step HKDF
 *                 method. Currently, only SHA2-256 is supported.
 *  \param[in,out] info                  HKDF work space
 *  \param[in]     secret                pointer to the secret
 *  \param[in]     secretLength          length of secret in bytes
 *  \param[in]     addInput              Pointer to the additional input, possibly Null
 *  \param[in]     addInputLength        length of additional input in bytes, possibly 0
 *  \param[in,out] derivedKey            (in)  pointer to the output key buffer
 *                                       (out) pointer to buffer, containing the output key
 *  \param[in]     derivedKeyLength      desired length of the key to be derived in bytes
 *  \param[in]     watchdog              pointer to watchdog reset function
 *  \return        actEXCEPTION_LENGTH   derivedKeyLength too high, i.e., greater than 4 294 967 264
 *                 actOK                 operation successful
 *  \pre           Pointers must be valid, except addInput, which can be NULL, if addInputLength is 0
 *                 workSpace is initialized by esl_initHKDF_Hash_OneStep(..).
 *                 The buffer referenced by derivedKey must provide at least derivedKeyLength many bytes.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actHKDF_Hash_OneStepDerive(
  VSECPRIM_P2VAR_PARA(actHKDFHASHSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) secret,
  actLengthType secretLength,
  VSECPRIM_P2CONST_PARA(actU8) addInput,
  actLengthType addInputLength,
  VSECPRIM_P2VAR_PARA(actU8) derivedKey,
  const actLengthType derivedKeyLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

#endif /* ACTIHKDF_HMACSHA256_H */

