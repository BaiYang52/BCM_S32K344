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
/*!        \file  ESLib_RSA_Common.h
 *        \brief  Crypto library - PKCS #1 RSA PSS signature verification / generation (SHA-256)
 *
 *      \details Helper functions for RSA signature generation / generation using RSA PSS encoding scheme
 *               according to PKCS #1 v2.2 (see also RFC8017 and the numeration therein)
 *               Currently the actClib version is used.
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

#ifndef ESLIB_RSA_PSS_COMMON_H
# define ESLIB_RSA_PSS_COMMON_H

# include "ESLib.h"
# include "actUtilities.h"

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
# if ((VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON)) /* COV_VSECPRIM_NO_SAFE_CONFIG TF xf tf xf */

#  define VSECPRIM_START_SEC_CONST_8BIT
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM_EXTERN(eslt_Byte) esl_RSAPSSEncodingMask[8];

#  define VSECPRIM_STOP_SEC_CONST_8BIT
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* # if ((VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON)) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# if ((VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON)) /* COV_VSECPRIM_NO_SAFE_CONFIG TF xf tf xf */

#  define VSECPRIM_START_SEC_CODE
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  esl_calcSaltedHashRSA_PSS()
 **********************************************************************************************************************/
/*! \brief      Calculate salted hash value for RSA PSS, independent of underlying hash function
 *  \details    Based on a given hash and a random salt the hash of
 *                M' = (0x) 00 00 00 00 00 00 00 00 || hash(orig. msg) || salt
 *              is calculated, as described in chapter 9.1.1, steps 5-6 and 9.1.2, steps 12-13 of PKCS#1 2.2
 *              The parameter hashID indicates which specific hash function is used.
 *  \param[in,out] wsuHash      (in)  Pointer to hash workspace union, containing either a SHA1, a SHA2 or a RMD160
 *                                    workspace
 *                             (out) Pointer to hash workspace union
 *  \param[in]  saltSize       Length of salt
 *  \param[in]  salt           Input salt
 *  \param[in]  messageDigest  Input hash value of the original message, will be hashed again here
 *  \param[in]  hashDigestSize Length of messageDigest
 *  \param[in,out] saltedHash  (in)  Output buffer for calculated hash
 *                             (out) calculated salted hash
 *  \param[in]  hashID parameter to determine the hash function to be used
 *  \return     ESL_ERC_NO_ERROR  Operation successful
 *              ESL_ERC_TOTAL_LENGTH_OVERFLOW  total length of data to be hashed is too long
 *              ESL_ERC_INPUT_INVALID hashID does not correspond to a supported hash function
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           wsHash, salt, messageDigest and saltedHash have to be valid pointers
 *                 buffer referenced by saltedHash has to be at least of length ESL_SIZEOF_SHA256_DIGEST
 **********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_calcSaltedHashRSA_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceUnionHashFunctions_RSA) wsuHash,
  eslt_Length saltSize,
  VSECPRIM_P2CONST_PARA(eslt_Byte) salt,
  VSECPRIM_P2CONST_PARA(eslt_Byte) messageDigest,
  eslt_Length hashDigestSize,
  VSECPRIM_P2VAR_PARA(eslt_Byte) saltedHash,
  eslt_HashAlgorithm hashID);

#  define VSECPRIM_STOP_SEC_CODE
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* ((VSECPRIM_RSA_PSS_SHA1_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_SHA2_256_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_RIPEMD160_ENABLED == STD_ON))  */

# if ((VSECPRIM_RSA_OAEP_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_ENABLED == STD_ON)) /* COV_VSECPRIM_NO_SAFE_CONFIG TF xf tf */

#  define VSECPRIM_START_SEC_CODE
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  esl_generateMaskMGF1()
 **********************************************************************************************************************/
/*! \brief     Mask generation function (MGF1)
 *  \details    MGF1 implementation as described in appendix B.2.1 of PKCS#1 2.2
 *              Based on selected hash function, this function generates a pseudorandom output string of arbitrary length
 *              depending on an input string (seed) of variable length
 *  \param[in] hashAlgorithm determines which hash function will be used
 *  \param[in,out] wsHash    (in)  Pointer to hash workspace
 *                           (out) Pointer to hash workspace
 *  \param[in,out] tempHash  (in)  Temporary buffer for intermediate hash
 *                           (out) intermediate hash value
 *  \param[in]  hashDigestSize size of the hash digest, depending on the algorithm specified by 'hashAlgorithm'
 *  \param[in]  seedLength   Length input seed
 *  \param[in]  seed         Input seed from which mask is generated
 *  \param[in]  maskLength   Length of mask to be generated
 *  \param[in,out] mask      (in)  Output buffer for generated mask
 *                           (out) generated  mask
 *  \return     ESL_ERC_NO_ERROR  Operation successful
 *              ESL_ERC_ERROR an error occured while hashing
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           wsHash, tempHash, seed and mask have to be valid pointers
 *                 buffer referenced by tempHash has to be at least of length ESL_SIZEOF_SHA256_DIGEST
 *                 buffer referenced by mask has to be at least of length maskLength
 **********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_generateMaskMGF1(
  eslt_HashAlgorithm hashAlgorithm,
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceUnionHashFunctions_RSA) wsHash,
  VSECPRIM_P2VAR_PARA(eslt_Byte) tempHash,
  eslt_Length hashDigestSize,
  eslt_Length seedLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) seed,
  eslt_Length maskLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) mask);

#  define VSECPRIM_STOP_SEC_CODE
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* ((VSECPRIM_RSA_OAEP_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_ENABLED == STD_ON)) */
#endif /* ESLIB_RSA_PSS_COMMON_H */
