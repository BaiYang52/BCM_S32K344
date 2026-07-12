/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*      \file  Crypto_30_LibCv_Hash.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Implementation for HASH Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_HASH_SOURCE
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Crypto_30_LibCv.h"
#include "Crypto_30_LibCv_Services.h"
#include "Crypto_30_LibCv_Hash.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define CRYPTO_30_LIBCV_VSECPRIM_HASH_ALGO_INVALID                    (0xFFu)

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if ((CRYPTO_30_LIBCV_HASH == STD_ON) || (((CRYPTO_30_LIBCV_ECPGENERATE == STD_ON) || (CRYPTO_30_LIBCV_ECPVERIFY == STD_ON)) && (CRYPTO_30_LIBCV_SIGNATURE_PRE_HASH == STD_ON)))
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_Hash_ModeStart()
 **********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
 *
  */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_Hash_ModeStart(
  P2VAR(vSecPrimType_WorkSpaceHash, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  Crypto_AlgorithmFamilyType hashAlgoFam)
{
  eslt_ErrorCode retValCv;
  Std_ReturnType retVal = E_NOT_OK;
  vSecPrimType_HashAlgorithm esLibHashAlgoIdentifier = CRYPTO_30_LIBCV_VSECPRIM_HASH_ALGO_INVALID;

  /* Get hash algorithm identifier for vSecPrim */
# if (CRYPTO_30_LIBCV_HASH_CUSTOM_MD5 == STD_ON)
  if (hashAlgoFam == CRYPTO_ALGOFAM_CUSTOM_MD5)
  {
    esLibHashAlgoIdentifier = VSECPRIM_DIGEST_MD5;
  }
  else
# endif /* (CRYPTO_30_LIBCV_HASH_CUSTOM_MD5 == STD_ON) */
  if (hashAlgoFam < Crypto_30_LibCv_GetSizeOfFamilyToHashMapping())
  {
    esLibHashAlgoIdentifier = Crypto_30_LibCv_GetFamilyToHashMapping(hashAlgoFam);
  }
  else
  {
    /* esLibHashAlgoIdentifier already set */
  }

  /* Initialize algorithm. */
  retValCv = esl_initWorkSpaceHeader(&(workSpace->header), VSECPRIM_SIZEOF_WS_HASH, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retValCv = vSecPrim_initHash(workSpace, esLibHashAlgoIdentifier, VSECPRIM_OUTPUT_TRUNCATION_ALLOWED); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_Hash_ModeUpdate()
 **********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_Hash_ModeUpdate(
  P2VAR(vSecPrimType_WorkSpaceHash, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  uint32 inputLength,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) inputPtr)
{
  eslt_ErrorCode retValCv;
  Std_ReturnType retVal = E_NOT_OK;

  /* # Process input data. */
  retValCv = vSecPrim_updateHash(workSpace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    (eslt_Length)inputLength,
    (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))inputPtr);

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_Hash_ModeFinish()
 **********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_Hash_ModeFinish(
  P2VAR(vSecPrimType_WorkSpaceHash, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) outputLengthPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) outputPtr)
{
  eslt_ErrorCode retValCv;
  Std_ReturnType retVal = E_NOT_OK;
  eslt_Length outputLength = (eslt_Length)(*outputLengthPtr);

  /* # Write results in output buffers. Output size adaption is done in vSecPrim. */
  retValCv = vSecPrim_finalizeHash(workSpace, &outputLength, outputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    *outputLengthPtr = outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    retVal = E_OK;
  }

  return retVal;
}
#endif /* ((CRYPTO_30_LIBCV_HASH == STD_ON) || (((CRYPTO_30_LIBCV_ECPGENERATE == STD_ON) || (CRYPTO_30_LIBCV_ECPVERIFY == STD_ON)) && (CRYPTO_30_LIBCV_SIGNATURE_PRE_HASH == STD_ON))) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#if (CRYPTO_30_LIBCV_HASH == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_Hash()
 **********************************************************************************************************************/
 /*!
  *
  * Internal comment removed.
 *
 *
 *
 *
 *
 *
  */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_Hash(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_3 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(vSecPrimType_WorkSpaceHash, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpaceHash = Crypto_30_LibCv_GetWorkspaceOfHash(Crypto_30_LibCv_GetHashIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(vSecPrimType_WorkSpaceHash)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_Dispatch_Hash_ModeStart(workSpaceHash, job->jobPrimitiveInfo->primitiveInfo->algorithm.family); /* SBSW_CRYPTO_30_LIBCV_CALL_WITH_WORKSPACE */
      break;
    }
    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_Dispatch_Hash_ModeUpdate(workSpaceHash, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
        job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr);
      break;
    }
    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retVal = Crypto_30_LibCv_Dispatch_Hash_ModeFinish(workSpaceHash, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
        job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr);
      break;
    }
    default: /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      break;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_HASH == STD_ON) */

#define CRYPTO_30_LIBCV_STOP_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_Hash.c
 *********************************************************************************************************************/
