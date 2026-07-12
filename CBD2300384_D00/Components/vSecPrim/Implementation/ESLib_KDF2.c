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
/*!        \file  ESLib_KDF2.c
 *        \brief  KDF2 key derivation function implementation.
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

#define ESLIB_KDF2_SOURCE

#include "ESLib.h"
#include "ESLib_Helper.h"
#include "actITypes.h"

/* actCLib includes */
#include "actIKDF2.h"

#if (VSECPRIM_PKCS5_ENABLED == STD_ON || VSECPRIM_PKCS5_HMAC_SHA256_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF xf xf */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** Local Function Prototypes
 ***************************************************************************/

/**********************************************************************************************************************
 *  esl_initKDF2
 *********************************************************************************************************************/
/*! \brief         Initialize the key derivation for KDF2, independent of underlying HMAC primitive
 *  \details       -
 *  \param[in,out]  workSpace    (in) algorithm context buffer, initialized by
 *                               esl_initWorkSpaceHeader(..) function
 *                               (out) initialized algorithm context structure     -
 *  \param[in]      iterationCount  number of SHA iterations
 *  \param[in]      workSpaceSize   size of workspace corresponding to desired HMAC primitive
 *  \param[in]      algo            workspace identifier for KDF2 HMAC SHA1/SHA256
 *  \return         ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL      work space too small
 *                  ESL_ERC_KDF_ITERATION_COUNT_OUT_OF_RANGE
 *                  ESL_ERC_NO_ERROR          else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initKDF2(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDF2) workSpace,
  const eslt_Length iterationCount,
  eslt_Length workSpaceSize,
  eslt_WSStatus algo);

/***********************************************************************************************************************
 *  esl_deriveKeyKDF2
 **********************************************************************************************************************/
/*! \brief        Derive Key according to KDF2, independent of underlying HMAC primitive
 *  \details       -
 *  \param[in,out] workSpace algorithm context buffer, initialized by
 *                           esl_initWorkSpaceHeader(..) function
 *  \param[in]     secretLength   length of secret in bytes
 *  \param[in]     secret         Pointer to the secret
 *  \param[in]     infoLength     length of the shared info (bytes)
 *  \param[in]     info           pointer to the shared info
 *  \param[in]     keyLength      length of output key buffer in bytes
 *  \param[out]    key            pointer to the output key buffer
 *  \param[in]     SHAid          indicates underlying HMAC SHA primitive
 *  \param[in]     SHAsize        length of SHA digest buffer
 *  \param[in]     algo           workspace identifier for KDF2 HMAC SHA1/SHA256
 *  \return        ESL_ERC_PARAMETER_INVALID input parameter is NULL
 *                 ESL_ERC_WS_TOO_SMALL      work space too small
 *                 ESL_ERC_WS_STATE_INVALID Invalid state
 *                 ESL_ERC_NO_ERROR          else
 *  \pre           workSpace is initialized by esl_initKDF2(..)
 *                 key need to have at least the length of keyLength.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_deriveKeyKDF2(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDF2) workSpace,
  const eslt_Length secretLength, VSECPRIM_P2CONST_PARA(eslt_Byte) secret,
  const eslt_Length infoLength, VSECPRIM_P2CONST_PARA(eslt_Byte) info,
  const eslt_Length keyLength, VSECPRIM_P2VAR_PARA(eslt_Byte) key,
  eslt_Byte SHAid, const eslt_Length SHAsize, eslt_WSStatus algo);

/****************************************************************************
 ** Local Functions
 ***************************************************************************/

/***********************************************************************************************************************
 *  esl_initKDF2
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initKDF2(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDF2) workSpace,
  const eslt_Length iterationCount,
  eslt_Length workSpaceSize,
  eslt_WSStatus algo)
{
  eslt_ErrorCode retVal;

  /* Check iteration count */
  if (iterationCount == 0u)
  {
    retVal = ESL_ERC_KDF_ITERATION_COUNT_OUT_OF_RANGE;
  }
  /* Check workSpace */
  else if (workSpace == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, workSpaceSize)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal already set */
  }
  /* Init actCLib KDF2, set workSpace state */
  else
  {
    (void)actKDF2Init((VSECPRIM_P2VAR_PARA(actKDF2STRUCT)) &(workSpace->wsKDF2HMACSHA), iterationCount); /* SBSW_VSECPRIM_CALL_FUNCTION */
    esl_SetWorkspaceStatus(&workSpace->header, algo); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* retVal already set */
  }

  return retVal;
}

/***********************************************************************************************************************
 *  esl_deriveKeyKDF2
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_deriveKeyKDF2(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDF2) workSpace,
  const eslt_Length secretLength, VSECPRIM_P2CONST_PARA(eslt_Byte) secret,
  const eslt_Length infoLength, VSECPRIM_P2CONST_PARA(eslt_Byte) info,
  const eslt_Length keyLength, VSECPRIM_P2VAR_PARA(eslt_Byte) key,
  eslt_Byte SHAid, const eslt_Length SHAsize, eslt_WSStatus algo)
{
  eslt_ErrorCode retVal;

  /* validate parameters */
  if ((workSpace == NULL_PTR) || (secret == NULL_PTR) || (key == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  /* check length */
  else if ((secretLength == 0u) || (keyLength == 0u))
  {
    retVal = ESL_ERC_INPUT_INVALID;
  }
  /* check workSpace */
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_KDF2HMACSHA1, algo)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    /* Derive key with actCLib KDF2 */
    (void)actKDF2Derive((VSECPRIM_P2VAR_PARA(actKDF2STRUCT)) &(workSpace->wsKDF2HMACSHA), secret, secretLength,
      info, infoLength, key, keyLength, SHAid, SHAsize); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    /* retVal was already set */
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

# if (VSECPRIM_PKCS5_ENABLED == STD_ON)
/* Wrapper functions for KDF2 based on HMAC SHA1 */
/***********************************************************************************************************************
 *  esl_initKDF2HMACSHA1
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initKDF2HMACSHA1(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDF2HMACSHA1) workSpace,
  const eslt_Length iterationCount)
{
  return esl_initKDF2(workSpace, iterationCount, ESL_MAXSIZEOF_WS_KDF2HMACSHA1, ESL_WST_ALGO_KDF2HMACSHA1); /* SBSW_VSECPRIM_FUNCTION_CALL_POINTER_CHECKED_WITHIN */
}

/***********************************************************************************************************************
 *  esl_deriveKeyKDF2HMACSHA1
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_deriveKeyKDF2HMACSHA1(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDF2HMACSHA1) workSpace,
  const eslt_Length secretLength, VSECPRIM_P2CONST_PARA(eslt_Byte) secret,
  const eslt_Length infoLength, VSECPRIM_P2CONST_PARA(eslt_Byte) info,
  const eslt_Length keyLength, VSECPRIM_P2VAR_PARA(eslt_Byte) key)
{
  return esl_deriveKeyKDF2(workSpace, secretLength, secret, infoLength, info, keyLength, key, actHASHMACSHA1_ALGO_ID, actHASH_SIZE_SHA, ESL_WST_ALGO_KDF2HMACSHA1); /* SBSW_VSECPRIM_FUNCTION_CALL_POINTER_CHECKED_WITHIN */
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */
# endif

# if (VSECPRIM_PKCS5_HMAC_SHA256_ENABLED == STD_ON)
/* Wrapper functions for KDF2 based on HMAC SHA 256 */
/***********************************************************************************************************************
 *  esl_initKDF2HMACSHA256
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initKDF2HMACSHA256(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDF2HMACSHA256) workSpace,
  const eslt_Length iterationCount)
{
  return esl_initKDF2(workSpace, iterationCount, ESL_MAXSIZEOF_WS_KDF2HMACSHA256, ESL_WST_ALGO_KDF2HMACSHA256); /* SBSW_VSECPRIM_FUNCTION_CALL_POINTER_CHECKED_WITHIN */
}

/***********************************************************************************************************************
 *  esl_deriveKeyKDF2HMACSHA256
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_deriveKeyKDF2HMACSHA256(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceKDF2HMACSHA256) workSpace,
  const eslt_Length secretLength, VSECPRIM_P2CONST_PARA(eslt_Byte) secret,
  const eslt_Length infoLength, VSECPRIM_P2CONST_PARA(eslt_Byte) info,
  const eslt_Length keyLength, VSECPRIM_P2VAR_PARA(eslt_Byte) key)
{
  return esl_deriveKeyKDF2(workSpace, secretLength, secret,
    infoLength, info, keyLength, key, actHASHMACSHA256_ALGO_ID, actHASH_SIZE_SHA256, ESL_WST_ALGO_KDF2HMACSHA256); /* SBSW_VSECPRIM_FUNCTION_CALL_POINTER_CHECKED_WITHIN */
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_PKCS5_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_KDF2HMACSHA1.c
 *********************************************************************************************************************/
