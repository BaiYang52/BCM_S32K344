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
/*!        \file  ESLib_types.h
 *        \brief  Definition of internal types and constants.
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
#ifndef ESLIB_TYPES_H
# define ESLIB_TYPES_H

# include "ESLib_t.h"

/*****************************************************************************
 Types and constants
 *****************************************************************************/
/* important WorkSpace-STate-Masks */
# define ESL_WST_M_LEN                                                (sizeof(eslt_WSStatus) * 8u)     /* L = status-bitLength */
# define ESL_WST_M_STATE                                              (0xC000u) /* bits 14, 15 */
# define ESL_WST_M_ALGO                                               (0x1F80u) /* bits 7, 8, 9, 10, 11, 12 */
# define ESL_WST_M_PRIM                                               (0x0070u) /* bits 4, 5, 6 */
                                                                                /* not used: bits 0, 1, 2, 3 */

/* the workspace contains critical data
 maybe during consecutive enc/dec-steps */
# define ESL_WST_M_CRITICAL                                           (0x8000u)

/* during usage of the workspace, this state-bit
 is active, this allows to recognize if some
 routine was aborted at some unwanted place */
# define ESL_WST_M_RUNNING                                            (0x4000u)

# define ESL_WST_ALLOCATED                                            (0x007Fu) /* the STATUS has been set only by
                                                                                   esl_initWorkSpaceHeader,
                                                                                   no information about the content is
                                                                                   known to the lib */

# define ESL_WST_ALGO_NOALGO                                          (0x0u)
# define ESL_WST_ALGO_AES128                                          (0x0100u)
# define ESL_WST_ALGO_SHA1                                            (0x0200u)
# define ESL_WST_ALGO_HMACSHA1                                        (0x0300u)
# define ESL_WST_ALGO_FIPS186                                         (0x0400u)
# define ESL_WST_ALGO_KDF2HMACSHA1                                    (0x0500u)
# define ESL_WST_ALGO_ECP                                             (0x0600u)
# define ESL_WST_ALGO_RSA                                             (0x0700u)
# define ESL_WST_ALGO_SHA256                                          (0x0800u)
# define ESL_WST_ALGO_SHA224                                          (ESL_WST_ALGO_SHA256)
# define ESL_WST_ALGO_RIPEMD160                                       (0x0900u)
# define ESL_WST_ALGO_AES192                                          (0x0a00u)
# define ESL_WST_ALGO_AES256                                          (0x0b00u)
# define ESL_WST_ALGO_HMACRIPEMD160                                   (0x0c00u)
# define ESL_WST_ALGO_KDFX963SHA1                                     (0x0d00u)
# define ESL_WST_ALGO_MD5                                             (0x0e00u)
# define ESL_WST_ALGO_HMACSHA256                                      (0x0f00u)
# define ESL_WST_ALGO_RC2                                             (0x1000u)
# define ESL_WST_ALGO_DES                                             (0x1100u)
# define ESL_WST_ALGO_TDES                                            (0x1200u)
# define ESL_WST_ALGO_CMACAES128                                      (0x1300u)
# define ESL_WST_ALGO_GCM                                             (0x1400u)
# define ESL_WST_ALGO_GMAC                                            (0x1500u)
# define ESL_WST_ALGO_SHA512                                          (0x1600u)
# define ESL_WST_ALGO_SHA512_224                                      (ESL_WST_ALGO_SHA512)
# define ESL_WST_ALGO_SHA512_256                                      (ESL_WST_ALGO_SHA512)
# define ESL_WST_ALGO_SHA384                                          (ESL_WST_ALGO_SHA512)
# define ESL_WST_ALGO_Ed25519                                         (0x1700u)
# define ESL_WST_ALGO_X25519                                          (0x1800u)
# define ESL_WST_ALGO_ChaCha20                                        (0x1D00u)
# define ESL_WST_ALGO_Poly1305                                        (0x1E00u)
# define ESL_WST_ALGO_SIPHASH                                         (0x1900u)
# define ESL_WST_ALGO_AEAD_7539                                       (0x1A00u)
# define ESL_WST_ALGO_KDFX963SHA256                                   (0x1B00u)
# define ESL_WST_ALGO_ECBD_KA                                         (0x1C00u)
# define ESL_WST_ALGO_HMACSHA384                                      (0x1F00u)
# define ESL_WST_ALGO_KDF2HMACSHA256                                  (0x0080u)
# define ESL_WST_ALGO_CTRDRBG                                         (0x0180u)
# define ESL_WST_ALGO_HASHDRBG                                        (0x0280u)
# define ESL_WST_ALGO_SHA3                                            (0x0380u)
# define ESL_WST_ALGO_SHAKE                                           (ESL_WST_ALGO_SHA3)
# define ESL_WST_ALGO_KDFX963SHA512                                   (0x0480u)
# define ESL_WST_ALGO_CMACAES256                                      (0x0580u)
# define ESL_WST_ALGO_AES128CCM                                       (0x0680u)
# define ESL_WST_ALGO_HKDF_HMAC_SHA256                                (0x0780u)
# define ESL_WST_ALGO_SPAKE2P                                         (0x0880u)
# define ESL_WST_ALGO_SPAKE2P_PREAMBLE                                (0x0980u)
# define ESL_WST_ALGO_HASH                                            (0x0A80u)
# define ESL_WST_ALGO_HKDF_HASH_SHA256                                (0x0B80u)
# define ESL_WST_ALGO_HKDF_HMAC_SHA384                                (0x0C80u)
/* the state is specified more exactly by XOR with one of the following constants */
# define ESL_WST_PRIM_ECPKG                                           (0x0010u)
# define ESL_WST_PRIM_ECPSP                                           (0x0020u)
# define ESL_WST_PRIM_ECPVP                                           (0x0030u)
# define ESL_WST_PRIM_ECPDH                                           (0x0040u)

# define ESL_WST_PRIM_RSA_CRT                                         (0x0010u)
# define ESL_WST_PRIM_RSA_NOCRT                                       (0x0020u)
# define ESL_WST_PRIM_RSA_KG                                          (0x0030u)
# define ESL_WST_PRIM_RSA_KGCRT                                       (0x0040u)

#endif /* ESLIB_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_types.h
 *********************************************************************************************************************/

