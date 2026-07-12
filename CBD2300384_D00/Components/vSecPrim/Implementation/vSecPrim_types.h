/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is protected under intellectual property laws and proprietary to cv cryptovision GmbH
 *                and/or Vector Informatik GmbH.
 *                No right or license is granted save as expressly set out in the applicable license conditions.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  vSecPrim_types.h
 *        \brief  Basic type definitions.
 *
 *      \details Currently the ESLib version is used.
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

#ifndef VSECPRIM_TYPES_H
# define VSECPRIM_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ESLib_Helper.h"
# include "ESLib_t.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Values for variables of type vSecPrimType_HashAlgorithm */
# define VSECPRIM_DIGEST_SHA1                                         (ESL_HA_SHA1)
# define VSECPRIM_DIGEST_SHA2_224                                     (ESL_HA_SHA2_224)
# define VSECPRIM_DIGEST_SHA2_256                                     (ESL_HA_SHA2_256)
# define VSECPRIM_DIGEST_SHA2_384                                     (ESL_HA_SHA2_384)
# define VSECPRIM_DIGEST_SHA2_512                                     (ESL_HA_SHA2_512)
# define VSECPRIM_DIGEST_SHA2_512_224                                 (ESL_HA_SHA2_512_224)
# define VSECPRIM_DIGEST_SHA2_512_256                                 (ESL_HA_SHA2_512_256)
# define VSECPRIM_DIGEST_SHA3_224                                     (ESL_HA_SHA3_224)
# define VSECPRIM_DIGEST_SHA3_256                                     (ESL_HA_SHA3_256)
# define VSECPRIM_DIGEST_SHA3_384                                     (ESL_HA_SHA3_384)
# define VSECPRIM_DIGEST_SHA3_512                                     (ESL_HA_SHA3_512)
# define VSECPRIM_DIGEST_SHAKE_128                                    (ESL_HA_SHAKE_128)
# define VSECPRIM_DIGEST_SHAKE_256                                    (ESL_HA_SHAKE_256)
# define VSECPRIM_DIGEST_RIPEMD160                                    (ESL_HA_RMD160)
# define VSECPRIM_DIGEST_MD5                                          (ESL_HA_MD5)

# define VSECPRIM_NUMBER_OF_HASH_FUNCTIONS                            (15u)
# define VSECPRIM_MAX_DIGEST_LENGTH                                   (ESL_SIZEOF_SHA512_DIGEST)

/* Values for variables of type vSecPrimType_outputBufferFlag */
/* Version 05.03.00 (viseag): These values are meant to be individually set bits in a 16-bit value.
 * As of now, they are only used for the generic hash functions and can thus also be interpreted as
 * absolute values. In later use cases, they can also be xor-ed and evaluated with masks. */
# define VSECPRIM_PROVIDED_BUFFER_LENGTH_MATCHES_DESIRED_OUTPUT_LENGTH (0x01u)
# define VSECPRIM_PROVIDED_BUFFER_LENGTH_MATCHES_OR_EXCEEDS_DESIRED_OUTPUT_LENGTH (0x02u)
# define VSECPRIM_OUTPUT_TRUNCATION_ALLOWED                           (0x04u)

/* Values for variables of type vSecPrimType_CallUpdateFlag */
# define VSECPRIM_UPDATE_NOT_YET_CALLED                               (0x00u)
# define VSECPRIM_UPDATE_CALLED                                       (0x01u)

 /* generic hashing workspace size */
# define VSECPRIM_SIZEOF_WS_HASH                                      (sizeof(vSecPrimType_WorkSpaceHash) - sizeof(eslt_WorkSpaceHeader))

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

# define vSecPrimMin(x, y)                                            (((x)<(y))? (x):(y))

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef eslt_Size16                                                   vSecPrimType_OutputBufferFlag;
typedef eslt_HashAlgorithm                                            vSecPrimType_HashAlgorithm;
typedef eslt_Byte                                                     vSecPrimType_CallUpdateFlag;

/* union containing all hash workspaces, used in vSecPrimType_WorkSpaceHash.
 * wsSHA1 is always contained (independent from the configuration), since
 * empty unions cause compile errors. */
typedef union
{ /* PRQA S 0750 */ /* MD_MSR_Union */
  eslt_WorkSpaceSHA1 wsSHA1;

# if (VSECPRIM_SHA2_256_ENABLED == STD_ON)
  eslt_WorkSpaceSHA224 wsSHA2_224;
  eslt_WorkSpaceSHA256 wsSHA2_256;
# endif

# if (VSECPRIM_SHA2_512_ENABLED == STD_ON)
  eslt_WorkSpaceSHA384 wsSHA2_384;
  eslt_WorkSpaceSHA512 wsSHA2_512;
  eslt_WorkSpaceSHA512_224 wsSHA2_512_224;
  eslt_WorkSpaceSHA512_256 wsSHA2_512_256;
# endif

# if (VSECPRIM_SHA3_ENABLED == STD_ON)
  eslt_WorkSpaceSHA3 wsSHA3;
  eslt_WorkSpaceSHAKE wsSHAKE;
# endif

# if (VSECPRIM_RIPEMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  eslt_WorkSpaceRIPEMD160 wsRIPEMD160;
# endif

# if (VSECPRIM_MD5_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  eslt_WorkSpaceMD5 wsMD5;
# endif

} vSecPrimType_WorkSpaceUnionHash;

/* workspace structure for hashing */
typedef struct
{
  eslt_WorkSpaceHeader header;
  vSecPrimType_WorkSpaceUnionHash wsHash;

  vSecPrimType_OutputBufferFlag outputLengthFlag;
  eslt_Byte tempHash[VSECPRIM_MAX_DIGEST_LENGTH];
  vSecPrimType_HashAlgorithm hashID;
  vSecPrimType_CallUpdateFlag updateCalled;

} vSecPrimType_WorkSpaceHash;

#endif /* __VSECPRIM_TYPES_H__ */

/**********************************************************************************************************************
 *  END OF FILE: vSecPrim_types.h
 *********************************************************************************************************************/
