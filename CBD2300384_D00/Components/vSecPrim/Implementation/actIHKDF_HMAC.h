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
/*!        \file  actIHKDF_HMAC.h
 *        \brief  HKDF_HMAC key derivation function according to NIST SP800-56C
 *                using SHA-256 or SHA 384 as underlying hash function.
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

#ifndef ACTIHKDFHMAC_H
# define ACTIHKDFHMAC_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C" {
# endif

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  actHKDF_HMAC_Extract
 **********************************************************************************************************************/
/*! \brief         Randomness extraction step for two-step HKDF based on HMAC SHA256 or HMAC SHA384.
 *  \details       This function derives a key derivation key from secret and salt. This is the first step in the
 *                 two-step HKDF.
 *  \param[in,out] info           HKDF  work space
 *  \param[in]     secret         pointer to the secret
 *  \param[in]     secretLength   length of secret in bytes
 *  \param[in]     salt           pointer to the optional salt (if NULL, a all-zero array is used instead)
 *  \param[in]     saltLength     length of salt in bytes (possibly 0)
 *  \param[in]     hashMACidx     index of the underlying hash function to be used, must be either of
 *                                actHASHMACSHA256_ALGO_ID or actHASHMACSHA384_ALGO_ID
 *  \param[in]     watchdog       pointer to watchdog reset function
 *  \pre           Pointers must be valid
 *                 workSpace is initialized by esl_initHKDF_HMACSHA256(..)
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_FUNC(void) actHKDF_HMAC_Extract(
  VSECPRIM_P2VAR_PARA(actHKDFSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) secret,
  actLengthType secretLength,
  VSECPRIM_P2CONST_PARA(actU8) salt,
  actLengthType saltLength,
  actU8 hashMACidx,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/***********************************************************************************************************************
 *  actHKDF_HMAC_Expand
 **********************************************************************************************************************/
/*! \brief         Expand Key according two-step HKDF based on HMAC SHA256 or SHA384.
 *  \details       This function expands the input keying material computed in actHKDF_HMAC_Extract and
 *                 uses the input keying material and the optional additional Input to derive a key of length
 *                 derivedKeyLength. This is the second step of the two-step HKDF.
 *  \param[in,out] info                  HKDF work space
 *  \param[in]     addInput              pointer to the additional input, possibly Null
 *  \param[in]     addInputLength        length of additional input in bytes, possibly 0
 *  \param[in,out] derivedKey            (in)  pointer to the output key buffer
 *                                       (out) pointer to buffer, containing the output key
 *  \param[in]     derivedKeyLength      desired length of the key to be derived in bytes
 *  \param[in]     hashMACidx            index of the underlying hash function to be used, must be either of
 *                                       actHASHMACSHA256_ALGO_ID or actHASHMACSHA384_ALGO_ID
 *  \param[in]     watchdog              pointer to watchdog reset function
 *  \return        actEXCEPTION_LENGTH   desired derivedKeyLength too high
 *                 actOK                 else
 *  \pre           Pointers must be valid, except addInput, which can be NULL, if addInputLength is 0
 *                 workSpace is initialized by esl_initHKDF_HMACSHA***(..) and actHKDF_HMAC_Extract is
 *                 called before.
 *                 The buffer referenced by derivedKey must provide at least derivedKeyLength many bytes.
 *  \note          According to the specification of the HKDF (RFC5869), derivedKeyLength must be <= 255*digest length
 *                 of SHA256 or SHA384, depending on the value of hashMACidx.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actHKDF_HMAC_Expand(
  VSECPRIM_P2VAR_PARA(actHKDFSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) addInput,
  actLengthType addInputLength,
  VSECPRIM_P2VAR_PARA(actU8) derivedKey,
  const actLengthType derivedKeyLength,
  actU8 hashMACidx,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/***********************************************************************************************************************
 *  actHKDF_HMAC_OneStepDerive
 **********************************************************************************************************************/
/*! \brief         Derive Key according one-step HKDF based on HMAC SHA256 or HMAC SHA384.
 *  \details       This function derives a key of length derivedKeyLength.
 *  \param[in,out] info                  HKDF work space
 *  \param[in]     secret                pointer to the secret
 *  \param[in]     secretLength          length of secret in bytes
 *  \param[in]     salt                  pointer to the optional salt (if NULL, a all-zero array is used instead)
 *  \param[in]     saltLength            length of salt in bytes (possibly 0)
 *  \param[in]     addInput              Pointer to the additional input, possibly Null
 *  \param[in]     addInputLength        length of additional input in bytes, possibly 0
 *  \param[in,out] derivedKey            (in)  pointer to the output key buffer
 *                                       (out) pointer to buffer, containing the output key
 *  \param[in]     derivedKeyLength      desired length of the key to be derived in bytes
 *  \param[in]     hashMACidx            index of the underlying hash function to be used, must be either of
 *                                       actHASHMACSHA256_ALGO_ID or actHASHMACSHA384_ALGO_ID
 *  \param[in]     watchdog              pointer to watchdog reset function
 *  \return        actEXCEPTION_LENGTH   derivedKeyLength too high, i.e., greater than 4 294 967 264, if
 *                                       hashMACidx = actHASHMACSHA256_ALGO_ID, or greater than
 *                                       4 294 967 248, else.
 *                 actOK                 operation successful
 *  \pre           Pointers must be valid
 *                 workSpace is initialized by esl_initHKDF_HMACSHA256(..).
 *                 The buffer referenced by derivedKey must provide at least derivedKeyLength many bytes.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actHKDF_HMAC_OneStepDerive(
  VSECPRIM_P2VAR_PARA(actHKDFSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) secret,
  actLengthType secretLength,
  VSECPRIM_P2CONST_PARA(actU8) salt,
  actLengthType saltLength,
  VSECPRIM_P2CONST_PARA(actU8) addInput,
  actLengthType addInputLength,
  VSECPRIM_P2VAR_PARA(actU8) derivedKey,
  const actLengthType derivedKeyLength,
  actU8 hashMACidx,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

#endif /* ACTIHKDF_HMAC_H */

