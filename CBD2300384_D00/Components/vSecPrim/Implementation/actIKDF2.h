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
/*!        \file  actIKDF2.h
 *        \brief  KDF2 key derivation function according to PKCS#5 using SHA-1 or SHA-256 as underlying hash function.
 *
 *      \details  Currently the actClib version is used.
 *                This file is part of the embedded systems library cvActLib/ES
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

#ifndef ACTIKDF2_H
# define ACTIKDF2_H

# include "actITypes.h"
# include "actIHashMAC.h"
# include "actIHashMACSHA256.h"

/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C" {
# endif

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actKDF2Init()
 *********************************************************************************************************************/
/*! \brief          This function initializes the KDF2 algorithm.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to context structure
 *                               (out) initialized context structure
 *  \param[in]      iteration_count  number of iterations used for the key derivation
 *  \return         actEXCEPTION_LENGTH   iteration_count is negative
 *                  actOK                 else
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actKDF2Init(VSECPRIM_P2VAR_PARA(actKDF2STRUCT) info, actLengthType iteration_count);

/**********************************************************************************************************************
 *  actKDF2Derive()
 *********************************************************************************************************************/
/*! \brief          This function derives a key of the desired length from the input
 *                  password and the (optional) salt.
 *  \details        -
 *  \param[in,out]  info           (in) pointer to context structure
 *                                 (out) actualized context structure
 *  \param[in]      passwd         pointer to the password
 *  \param[in]      passwd_length  length of passwd in bytes
 *  \param[in]      salt           pointer to the optional salt (if NULL, no salt is used)
 *  \param[in]      salt_length    length of salt in bytes
 *  \param[in,out]  key            (in) pointer to the output key buffer
 *                                 (out) the derived key
 *  \param[in]      key_length     length of output key buffer in bytes
 *  \param[in]      hashMACidx     indicates which HMAC SHA version will be the underlying primitive
 *                                 (HMAC SHA1 or HMAC SHA256)
 *  \param[in]      sizeSHA        size of the SHA digest; used to compute number of digests needed to
 *                                 derive a key of the desired key length
 *  \return         actEXCEPTION_NULL     passwd or key is NULL
 *                  actEXCEPTION_LENGTH   an input length value is negative
 *                  actOK                 else
 *  \pre            actKDF2Init() is called once before calling this function
 *                  key and passwd must be valid pointers
 *                  passwd_length, salt_length and key_length must be greater than or equal to zero
 *                  key_length must not be greater than (2^32-1) - sizeSHA + 1 to avoid overflow
 *                  U_buf must point to a buffer of actHASH_SIZE_SHA_KDF2 length
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actKDF2Derive(
  VSECPRIM_P2VAR_PARA(actKDF2STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) passwd, actLengthType passwd_length,
  VSECPRIM_P2CONST_PARA(actU8) salt, actLengthType salt_length,
  VSECPRIM_P2VAR_PARA(actU8) key, actLengthType key_length,
  actU8 hashMACidx, actLengthType sizeSHA);

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

#endif /* ACTIKDF2_H */

