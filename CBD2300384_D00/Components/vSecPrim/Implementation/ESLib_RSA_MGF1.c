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
/*!        \file  ESLib_RSA_MGF1.c
 *        \brief  Crypto library - RSA Mask Generation Function
 *
 *      \details Helper functions for RSA Mask generation according to PKCS #1 v2.2
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
#define ESLIB_RSA_MGF1_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "ESLib_RSA_Common.h"
#include "ESLib_Helper.h"

#if ((VSECPRIM_RSA_OAEP_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_ENABLED == STD_ON)) /* COV_VSECPRIM_NO_SAFE_CONFIG TF xf tf */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if (VSECPRIM_RSA_HASHALGORITHM_SHA1_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/**********************************************************************************************************************
 *  esl_generateMaskMGF1RSASHA1_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_generateMaskMGF1RSASHA1_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA1) wsHash,  /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2VAR_PARA(eslt_Byte) tempHash,
  eslt_Length seedLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) seed,
  eslt_Length maskLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) mask)
{
  eslt_ErrorCode returnValue;

  if ((wsHash == NULL_PTR) || (tempHash == NULL_PTR) || (seed == NULL_PTR) || (mask == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((returnValue = esl_CheckWorkSpaceHeader(&wsHash->header, ESL_MINSIZEOF_WS_SHA1)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* returnValue already set */
  }
  else
  {
    eslt_WorkSpaceUnionHashFunctions_RSA wsHashUnion; /* PRQA S 0759 */ /* MD_MSR_Union */
    wsHashUnion.SHA1ws = *wsHash;

    returnValue = esl_generateMaskMGF1(ESL_HA_SHA1, &wsHashUnion, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      tempHash, ESL_SIZEOF_SHA1_DIGEST, seedLength, seed, maskLength, mask);
  }

  return returnValue;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */
# endif /* (VSECPRIM_RSA_HASHALGORITHM_SHA1_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_HASHALGORITHM_SHA2_256_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
/**********************************************************************************************************************
 *  esl_generateMaskMGF1RSASHA256_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_generateMaskMGF1RSASHA256_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSHA256) wsHash,  /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2VAR_PARA(eslt_Byte) tempHash,
  eslt_Length seedLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) seed,
  eslt_Length maskLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) mask)
{
  eslt_ErrorCode returnValue;

  if ((wsHash == NULL_PTR) || (tempHash == NULL_PTR) || (seed == NULL_PTR) || (mask == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((returnValue = esl_CheckWorkSpaceHeader(&wsHash->header, ESL_MINSIZEOF_WS_SHA256)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* returnValue already set */
  }
  else
  {
    eslt_WorkSpaceUnionHashFunctions_RSA wsHashUnion; /* PRQA S 0759 */ /* MD_MSR_Union */
    wsHashUnion.SHA256ws = *wsHash;

    returnValue = esl_generateMaskMGF1(ESL_HA_SHA2_256, &wsHashUnion, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_RSAPSSSHA256_MASK */
      tempHash, ESL_SIZEOF_SHA256_DIGEST, seedLength, seed, maskLength, mask);
  }

  return returnValue;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */
# endif /* (VSECPRIM_RSA_HASHALGORITHM_SHA2_256_ENABLED == STD_ON) */

# if (VSECPRIM_RSA_HASHALGORITHM_RIPEMD160_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/**********************************************************************************************************************
 *  esl_generateMaskMGF1RSARIPEMD160_PSS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_generateMaskMGF1RSARIPEMD160_PSS(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRIPEMD160) wsHash,  /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2VAR_PARA(eslt_Byte) tempHash,
  eslt_Length seedLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) seed,
  eslt_Length maskLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) mask)
{
  eslt_ErrorCode returnValue;

  if ((wsHash == NULL_PTR) || (tempHash == NULL_PTR) || (seed == NULL_PTR) || (mask == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((returnValue = esl_CheckWorkSpaceHeader(&wsHash->header, ESL_MINSIZEOF_WS_RIPEMD160)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
   /* returnValue already set */
  }
  else
  {
    eslt_WorkSpaceUnionHashFunctions_RSA wsHashUnion; /* PRQA S 0759 */ /* MD_MSR_Union */
    wsHashUnion.RIPEMD160ws = *wsHash;

    returnValue = esl_generateMaskMGF1(ESL_HA_RMD160, &wsHashUnion,
      tempHash, ESL_SIZEOF_RIPEMD160_DIGEST, seedLength, seed, maskLength, mask);
  }

  return returnValue;
}/* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */
# endif /* (VSECPRIM_RSA_HASHALGORITHM_RIPEMD160_ENABLED == STD_ON) */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ((VSECPRIM_RSA_OAEP_ENABLED == STD_ON) || (VSECPRIM_RSA_PSS_ENABLED == STD_ON)) */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSA_MGF1.c
 *********************************************************************************************************************/
