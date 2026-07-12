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
/*!        \file  ESLib_SipHash.c
 *        \brief  Sip Hash implementation.
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

#define ESLIB_SIPHASH_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "ESLib.h"
#include "ESLib_Helper.h"

#include "actISipHash.h"

#if (VSECPRIM_SIP_HASH_ENABLED == STD_ON)

/* Check that 64 bit type is supported by platform. */
# ifndef ACT_PLATFORM_UINT64_AVAILABLE
#  error ACT_PLATFORM_UINT64_AVAILABLE must be defined
# endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  esl_initSipHash
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initSipHash(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSipHash) workSpace, const eslt_Length keyLength, VSECPRIM_P2CONST_PARA(eslt_Byte) key)
{
  eslt_ErrorCode retVal;

  if (workSpace == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_SIPHASH)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal already set */
  }
  else if (key == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if (keyLength != ESL_SIZEOF_SIPHASH_KEY)
  {
    retVal = ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE;
  }
  else
  {
    /* Init actCLib SipHash */
    (void)actSipHashInit(&workSpace->wsSipHash, key, ESL_SIZEOF_SIPHASH_KEY, workSpace->header.watchdog);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* Set workSpace state */
    esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_SIPHASH); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
}

/***********************************************************************************************************************
 *  esl_updateSipHash
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_updateSipHash(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSipHash) workSpace, const eslt_Length inputLength, VSECPRIM_P2CONST_PARA(eslt_Byte) input)
{
  eslt_ErrorCode retVal;
  if ((workSpace == NULL_PTR) || (input == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_SIPHASH, ESL_WST_ALGO_SIPHASH)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    /* Update actCLib SipHash */
    if (actSipHashUpdate(&workSpace->wsSipHash, input, (actLengthType)inputLength, workSpace->header.watchdog) == actEXCEPTION_LENGTH)  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    {
      retVal = ESL_ERC_INPUT_INVALID;
    }
  }
  return retVal;
}

/***********************************************************************************************************************
 *  esl_finalizeSipHash
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_finalizeSipHash(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSipHash) workSpace, VSECPRIM_P2VAR_PARA(eslt_Byte) messageMAC)
{
  eslt_ErrorCode retVal;
  if ((workSpace == NULL_PTR) || (messageMAC == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_SIPHASH, ESL_WST_ALGO_SIPHASH)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    actU64 mac64;
    /* Finalize actCLib SipHash */
    (void)actSipHashFinalize(&workSpace->wsSipHash, &mac64, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    messageMAC[0] = (eslt_Byte) (mac64 & 0xFFuLL);          /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX_SIPHASH_FINALIZE */
    messageMAC[1] = (eslt_Byte) ((mac64 >> 8) & 0xFFuLL);   /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX_SIPHASH_FINALIZE */
    messageMAC[2] = (eslt_Byte) ((mac64 >> 16) & 0xFFuLL);  /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX_SIPHASH_FINALIZE */
    messageMAC[3] = (eslt_Byte) ((mac64 >> 24) & 0xFFuLL);  /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX_SIPHASH_FINALIZE */
    messageMAC[4] = (eslt_Byte) ((mac64 >> 32) & 0xFFuLL);  /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX_SIPHASH_FINALIZE */
    messageMAC[5] = (eslt_Byte) ((mac64 >> 40) & 0xFFuLL);  /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX_SIPHASH_FINALIZE */
    messageMAC[6] = (eslt_Byte) ((mac64 >> 48) & 0xFFuLL);  /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX_SIPHASH_FINALIZE */
    messageMAC[7] = (eslt_Byte) (mac64 >> 56);              /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX_SIPHASH_FINALIZE */

    /* Reset and clear workspace */
    esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->wsSipHash); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }
  return retVal;
}

/***********************************************************************************************************************
 *  esl_verifySipHash
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifySipHash(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSipHash) workSpace, VSECPRIM_P2CONST_PARA(eslt_Byte) messageMAC)
{
  eslt_ErrorCode retVal;
  if ((workSpace == NULL_PTR) || (messageMAC == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_SIPHASH, ESL_WST_ALGO_SIPHASH)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    eslt_Byte tmpMAC[sizeof(actU64)];

    /* Calculate MAC */
    (void)esl_finalizeSipHash(workSpace, tmpMAC); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */

    if (actMemcmp(tmpMAC, messageMAC, sizeof(actU64)) != TRUE) /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
    {
      retVal = ESL_ERC_HMAC_INCORRECT_MAC;
    }
  }
  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_SIP_HASH_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_SipHash.c
 *********************************************************************************************************************/
