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
/*!         \file  ESLib_ERC.h
 *         \brief  ERC header
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

#ifndef ESLIB_ERC_H
# define ESLIB_ERC_H

# include "ESLib_t.h"

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/*****************************************************************************
 *  Return code constants
 *****************************************************************************/

/* general error or return types: the highest nibble classifies the error types */
# define ESL_ERT_RETURNCODE                                           0x0000u/* no error */
# define ESL_ERT_WARNING                                              0x1000u/* a warning */
# define ESL_ERT_ERROR                                                0x2000u/* a fatal error */

/* workspace and init-errorcodes; second nibble == 1 */
# define ESL_ERT_WS_ERROR                                             ESL_ERT_ERROR+0x0100u

/*****************************************************************************
 * General Error or Return Codes
 *****************************************************************************/

  /* OK, NO ERROR */
# define ESL_ERC_NO_ERROR                                             (ESL_ERT_RETURNCODE)

  /* WARNINGS */
# define ESL_ERC_WARNING                                              (ESL_ERT_WARNING)

  /* GENERAL ERRORS */
# define ESL_ERC_ERROR                                                (ESL_ERT_ERROR)

  /* invalid workspace  */
# define ESL_ERC_WS_STATE_INVALID                                     (ESL_ERT_ERROR + 0x0001u)

  /* the given workSpace is too small */
# define ESL_ERC_WS_TOO_SMALL                                         (ESL_ERT_ERROR + 0x0002u)

  /* some parameters contain invalid values */
# define ESL_ERC_PARAMETER_INVALID                                    (ESL_ERT_ERROR + 0x0003u)

  /* unsupported mode (eslt_Mode) */
# define ESL_ERC_MODE_INVALID                                         (ESL_ERT_ERROR + 0x0004u)

  /* if two valid modes interfere */
# define ESL_ERC_MODE_INCOMPATIBLE                                    (ESL_ERT_ERROR + 0x0005u)

  /* memory conflict  */
# define ESL_ERC_MEMORY_CONFLICT                                      (ESL_ERT_ERROR + 0x0006u)

  /* the given output length is not sufficient */
# define ESL_ERC_OUTPUT_SIZE_TOO_SHORT                                (ESL_ERT_ERROR + 0x0007u)
# define ESL_ERC_BUFFER_TOO_SMALL                                     (ESL_ERT_ERROR + 0x0007u) /* alias! */

  /* invalid input */
# define ESL_ERC_INPUT_INVALID                                        (ESL_ERT_ERROR + 0x0008u)

  /*  FIPS186 ERRORS  */
  /* init (HW not available, no response) */
# define ESL_ERC_RNG_INIT_FAILED                                      (ESL_ERT_ERROR + 0x0009u)

  /* not enough entropy */
# define ESL_ERC_RNG_ENTROPY_TOO_SMALL                                (ESL_ERT_ERROR + 0x000Au)

  /* bad internal state of RNG */
# define ESL_ERC_RNG_BAD_INTERNAL_STATE                               (ESL_ERT_ERROR + 0x000Bu)

  /*  AES128 ERRORS */
  /* the given padding is invalid */
# define ESL_ERC_AES_PADDING_INVALID                                  (ESL_ERT_ERROR + 0x000Cu)

  /* Hash and HashMAC/ ERRORS */
# define ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW                           (ESL_ERT_ERROR + 0x000Du)

  /*  key length is zero */
# define ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE                         (ESL_ERT_ERROR + 0x000Eu)

  /* the given HashMAC is invalid  */
# define ESL_ERC_HMAC_INCORRECT_MAC                                   (ESL_ERT_ERROR + 0x000Fu)
# define ESL_ERC_INCORRECT_MAC                                        ESL_ERC_HMAC_INCORRECT_MAC /*  NOT specific for HMAC!! */

  /* KDF2 ERRORS */
  /* iteration count is zero */
# define ESL_ERC_KDF_ITERATION_COUNT_OUT_OF_RANGE                     (ESL_ERT_ERROR + 0x0010u)

  /* ECC ERRORS */
  /* the given domain parameter are invalid */
# define ESL_ERC_ECC_DOMAIN_INVALID                                   (ESL_ERT_ERROR + 0x0011u)

  /* the public key is invalid */
# define ESL_ERC_ECC_PUBKEY_INVALID                                   (ESL_ERT_ERROR + 0x0012u)

  /* the input message is too long */
# define ESL_ERC_ECC_MESSAGE_TOO_LONG                                 (ESL_ERT_ERROR + 0x0013u)

  /* the private key is invalid */
# define ESL_ERC_ECC_PRIVKEY_INVALID                                  (ESL_ERT_ERROR + 0x0014u)

  /* values do not match p and n */
# define ESL_ERC_ECC_DOMAINEXT_INVALID                                (ESL_ERT_ERROR + 0x0015u)

  /* value groups is not supported */
# define ESL_ERC_ECC_SPEEDUPEXT_INVALID                               (ESL_ERT_ERROR + 0x0016u)

  /* internal error of the  */
# define ESL_ERC_ECC_INTERNAL_ERROR                                   (ESL_ERT_ERROR + 0x0017u)

  /* signature invalid */
# define ESL_ERC_ECC_SIGNATURE_INVALID                                (ESL_ERT_ERROR + 0x0018u)

  /* RSA ERRORS */
  /* the given module is not supported */
# define ESL_ERC_RSA_MODULE_OUT_OF_RANGE                              (ESL_ERT_ERROR + 0x0019u)

  /* input value (code) out of range */
# define ESL_ERC_RSA_CODE_OUT_OF_RANGE                                (ESL_ERT_ERROR + 0x001Au)

  /* input value (message) out of range */
# define ESL_ERC_RSA_MESSAGE_OUT_OF_RANGE                             (ESL_ERT_ERROR + 0x001Bu)

  /* input value (signature) out of range */
# define ESL_ERC_RSA_SIGNATURE_OUT_OF_RANGE                           (ESL_ERT_ERROR + 0x001Cu)

  /* the given signature is invalid */
# define ESL_ERC_RSA_SIGNATURE_INVALID                                (ESL_ERT_ERROR + 0x001Du)

  /* encoding invalid */
# define ESL_ERC_RSA_ENCODING_INVALID                                 (ESL_ERT_ERROR + 0x001Eu)

  /* RSA public key invalid */
# define ESL_ERC_RSA_PUBKEY_INVALID                                   (ESL_ERT_ERROR + 0x001Fu)

  /* RSA private key invalid */
# define ESL_ERC_RSA_PRIVKEY_INVALID                                  (ESL_ERT_ERROR + 0x0020u)

/* SHA-512 Errors */
# define ESL_ERC_TOTAL_LENGTH_OVERFLOW                                (ESL_ERT_ERROR + 0x002Cu)  /* NOT specific for SHA-512!! */

  /* SHA-256 ERRORS */
# define ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW                         (ESL_ERC_TOTAL_LENGTH_OVERFLOW)

  /* SHA-384 ERRORS */
# define ESL_ERC_SHA384_TOTAL_LENGTH_OVERFLOW                         (ESL_ERC_TOTAL_LENGTH_OVERFLOW)

  /* RIPEMD160 ERRORS */
# define ESL_ERC_RIPEMD160_TOTAL_LENGTH_OVERFLOW                      (ESL_ERC_TOTAL_LENGTH_OVERFLOW)

  /*  DES ERRORS */
  /* the given padding is invalid */
# define ESL_ERC_DES_PADDING_INVALID                                  (ESL_ERT_ERROR + 0x0025u)

  /*  TDES ERRORS */
  /* the given key length is invalid */
# define ESL_ERC_TDES_KEY_LENGTH_INVALID                              (ESL_ERT_ERROR + 0x0026u)

  /* GCM Errors */
  /* key length must be one of the AES key lengths: 16, 24 or 32 bytes */
# define ESL_ERC_GCM_INVALID_KEY_LENGTH                               (ESL_ERT_ERROR + 0x0027u)
# define ESL_ERC_CCM_INVALID_KEY_LENGTH                               (define ESL_ERC_GCM_INVALID_KEY_LENGTH)

  /* input too long */
# define ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW                            (ESL_ERT_ERROR + 0x0028u)
# define ESL_ERC_CCM_TOTAL_LENGTH_OVERFLOW                            (ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW)

  /* given tag does not match the computed tag */
# define ESL_ERC_GCM_TAG_VERIFICATION_FAILED                          (ESL_ERT_ERROR + 0x0029u)

  /* passing AAD after message data */
# define ESL_ERC_GCM_STATE_INVALID                                    (ESL_ERT_ERROR + 0x002Au)
/* SHA3 in invalid KECCAK state (squeeze instead of absorb) */
# define ESL_ERC_STATE_INVALID                                        (ESL_ERC_GCM_STATE_INVALID) /* NOT specific for GCM!! */

  /* IV length must be at least 1 (bit) */
# define ESL_ERC_INVALID_IV_LENGTH                                    (ESL_ERT_ERROR + 0x002Bu)  /* NOT specific for GCM!! */

  /* EdDSA Errors */
  /* curve not supported */
# define ESL_ERC_CURVE_NOT_SUPPORTED                                  (ESL_ERT_ERROR + 0x002Du)  /* NOT specific for EdDSA!! */

  /* signature invalid */
# define ESL_ERC_SIGNATURE_INVALID                                    (ESL_ERT_ERROR + 0x002Eu)  /* NOT specific for EdDSA!! */

  /* RNG failed - no private key generated */
# define ESL_ERC_RNG_FAILED                                           (ESL_ERT_ERROR + 0x002Fu) /* NOT specific for EdDSA!! */

  /* EdDSA instance not supported */
# define ESL_ERC_INSTANCE_NOT_SUPPORTED                               (ESL_ERT_ERROR + 0x0030u)

/* entropy exhausted, reseeding required */
# define ESL_ERC_ENTROPY_EXHAUSTION                                   (ESL_ERT_ERROR + 0x0031u)

# define ESL_ERC_INVALID_LENGTH                                       (ESL_ERT_ERROR + 0x0032u)

# define ESL_ERC_REQUESTED_LENGTH_INVALID                             (ESL_ERT_ERROR + 0x0033u)

   /* ChaCha20 & Poly1305 Errors */
   /* the given Poly1305 TAG is invalid  */
# define ESL_ERC_INCORRECT_TAG                                        ESL_ERC_INCORRECT_MAC

  /* unspecific Errors */
  /* unrecoverable error */
# define ESL_ERC_UNRECOVERABLE_ERROR                                  (ESL_ERT_ERROR + 0x0800u)

  /* 16 bit signed error codes */
# define ESL_ERC_16BITS                                               (0x7FFFu)

typedef actU16 eslt_ErrorCode;

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
}                               /* extern "C" */
# endif

#endif /* ESLIB_ERC_H */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_ERC.h
 *********************************************************************************************************************/
