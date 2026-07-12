/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  NvM MAC handling for data integrity
 *      \details  -
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "NvM_Mac.h"

#if (NVM_CSM_MAC_DATAINTEGRITY_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#ifndef NVM_LOCAL                                                                                                       /* COV_NVM_COMPATIBILITY */
# define NVM_LOCAL static
#endif

#ifndef NVM_LOCAL_INLINE                                                                                                /* COV_NVM_COMPATIBILITY */
# define NVM_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  NvM_Mac_SetupCommonJob
 *********************************************************************************************************************/
/*!
 * \brief           Setup of job commons.
 * \details         Wiring Self pointer
 * \param[in,out]   Self          Pointer to the MAC job structure that holds the necessary function pointer and data
 * \param[in]       BlockId       BlockId in range [1, (number of blocks - 1)]
 * \param[in]       RamDataPtr    Pointer holding the data to calculate the integrity code over
 * \param[in]       MacBuffer     Pointer where the integrity code will be written at the end of the calculation
 * \param[in]       DataLength    Length of the data to calculate the integrity code over
 * \pre             -
 * \config          NVM_CSM_MAC_DATAINTEGRITY_ENABLED
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 */
NVM_LOCAL_INLINE FUNC(void, NVM_PRIVATE_CODE) NvM_Mac_SetupCommonJob(
    NvM_MacJobPtrType Self,
    NvM_BlockIdType BlockId,
    NvM_RamAddressType RamDataPtr,
    NvM_RamAddressType MacBuffer,
    uint16 DataLength);

/**********************************************************************************************************************
 *  NvM_Mac_PerformCsmGenerateJobAttempt
 *********************************************************************************************************************/
/*!
 * \brief           Execute a Csm generate job attempt and check for success
 * \details         Exports API result via CsmResultPtr
 * \param[in]       Self          Pointer to MAC job structure
 * \param[in,out]   CsmResultPtr  Pointer holding Csm API call result
 * \return          TRUE if job was accepted and resulting MAC length is as expected; FALSE otherwise
 * \pre             -
 * \config          NVM_CSM_MAC_DATAINTEGRITY_ENABLED
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 */
NVM_LOCAL_INLINE FUNC(boolean, NVM_PRIVATE_CODE) NvM_Mac_PerformCsmGenerateJobAttempt(
    NvM_MacJobConstPtrType Self,
    Std_ReturnType* CsmResultPtr);

/**********************************************************************************************************************
 *  NvM_Mac_PerformCsmVerifyJobAttempt
 *********************************************************************************************************************/
/*!
 * \brief           Execute a Csm verify job attempt and check for success
 * \details         Exports API result via CsmResultPtr
 * \param[in]       Self          Pointer to MAC job structure
 * \param[in,out]   CsmResultPtr  Pointer holding Csm API call result
 * \return          TRUE if job was accepted and resulting verification result is OK; FALSE otherwise
 * \pre             -
 * \config          NVM_CSM_MAC_DATAINTEGRITY_ENABLED
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 */
NVM_LOCAL_INLINE FUNC(boolean, NVM_PRIVATE_CODE) NvM_Mac_PerformCsmVerifyJobAttempt(
    NvM_MacJobConstPtrType Self,
    Std_ReturnType* CsmResultPtr);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  NvM_Mac_SetupCommonJob
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
NVM_LOCAL_INLINE FUNC(void, NVM_PRIVATE_CODE) NvM_Mac_SetupCommonJob(
    NvM_MacJobPtrType Self,
    NvM_BlockIdType BlockId,
    NvM_RamAddressType RamDataPtr,
    NvM_RamAddressType MacBuffer,
    uint16 DataLength)
{
  const NvM_BlockDescrPtrType blockDescriptor = &(NvM_BlockDescriptorTable_at[NVM_BLOCK_FROM_DCM_ID(BlockId)]);

  Self->State = NVM_MACJOBSTATE_PENDING;                                                                                /* SBSW_NvM_AccessMacJobPtr */
  Self->BlockId = BlockId;                                                                                              /* SBSW_NvM_AccessMacJobPtr */
  Self->BlockDataLength = DataLength;                                                                                   /* SBSW_NvM_AccessMacJobPtr */
  Self->BlockDataPtr = RamDataPtr;                                                                                      /* SBSW_NvM_AccessMacJobPtr */
  Self->CsmAttemptCounter = 0u;                                                                                         /* SBSW_NvM_AccessMacJobPtr */
  Self->MacBuffer = MacBuffer;                                                                                          /* SBSW_NvM_AccessMacJobPtr */

  Self->CsmMacJobReference = blockDescriptor->MacReference;                                                             /* SBSW_NvM_AccessMacJobPtr */
  Self->BlockMacLength = blockDescriptor->MacSize;                                                                      /* SBSW_NvM_AccessMacJobPtr */
}

/**********************************************************************************************************************
 *  NvM_Mac_PerformCsmGenerateJobAttempt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
NVM_LOCAL_INLINE FUNC(boolean, NVM_PRIVATE_CODE) NvM_Mac_PerformCsmGenerateJobAttempt(
    NvM_MacJobConstPtrType Self,
    Std_ReturnType* CsmResultPtr)
{
  /* API precondition of MacGenerate(): expected MAC block length must be injected and will be checked by CSM */
  uint32 macResultLength = Self->BlockMacLength;
  boolean attemptSuccess = FALSE;

  *CsmResultPtr = Csm_MacGenerate(                                                                                      /* SBSW_NvM_MacInternalFunctionsWithOutParameters */ /* SBSW_NvM_MacExternalFunctionCallsToCsm */
      NvM_CsmMacJobs[Self->CsmMacJobReference].GenerateJobId,
      CRYPTO_OPERATIONMODE_SINGLECALL,
      Self->BlockDataPtr,
      Self->BlockDataLength,
      Self->MacBuffer,
      &macResultLength);

  if ((*CsmResultPtr == E_OK) && (macResultLength == Self->BlockMacLength))
  {
    attemptSuccess = TRUE;
  }

  return attemptSuccess;
}

/**********************************************************************************************************************
 *  NvM_Mac_PerformCsmVerifyJobAttempt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
NVM_LOCAL_INLINE FUNC(boolean, NVM_PRIVATE_CODE) NvM_Mac_PerformCsmVerifyJobAttempt(
    NvM_MacJobConstPtrType Self,
    Std_ReturnType* CsmResultPtr)
{
  Crypto_VerifyResultType verifyResult = CRYPTO_E_VER_NOT_OK;
  boolean attemptSuccess = FALSE;

  /* API precondition: length must be presented as BIT-length */
  uint32 macLengthInBits = Self->BlockMacLength * 8u;

  *CsmResultPtr = Csm_MacVerify(                                                                                        /* SBSW_NvM_MacInternalFunctionsWithOutParameters */ /* SBSW_NvM_MacExternalFunctionCallsToCsm */
      NvM_CsmMacJobs[Self->CsmMacJobReference].VerifyJobId,
      CRYPTO_OPERATIONMODE_SINGLECALL,
      Self->BlockDataPtr,
      Self->BlockDataLength,
      Self->MacBuffer,
      macLengthInBits,
      &verifyResult);

  if ((*CsmResultPtr == E_OK) && (verifyResult == CRYPTO_E_VER_OK))
  {
    attemptSuccess = TRUE;
  }

  return attemptSuccess;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  NvM_Mac_CreateGenerateJob
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, NVM_PRIVATE_CODE) NvM_Mac_CreateGenerateJob(
    NvM_MacJobPtrType Self,
    NvM_BlockIdType BlockId,
    NvM_RamAddressType RamDataPtr,
    NvM_RamAddressType MacTargetBuffer,
    uint16 DataLength)
{
  NvM_Mac_SetupCommonJob(Self, BlockId, RamDataPtr, MacTargetBuffer, DataLength);                                       /* SBSW_NvM_MacInternalFunctions */

  Self->JobType = NVM_MACJOBTYPE_GENERATE;                                                                              /* SBSW_NvM_AccessMacJobPtr */
}

/**********************************************************************************************************************
 *  NvM_Mac_CreateValidateJob
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, NVM_PRIVATE_CODE) NvM_Mac_CreateValidateJob(
    NvM_MacJobPtrType Self,
    NvM_BlockIdType BlockId,
    NvM_RamAddressType RamDataPtr,
    NvM_RamAddressType MacVerifyBuffer,
    uint16 DataLength)
{
  NvM_Mac_SetupCommonJob(Self, BlockId, RamDataPtr, MacVerifyBuffer, DataLength);                                       /* SBSW_NvM_MacInternalFunctions */

  Self->JobType = NVM_MACJOBTYPE_VERIFY;                                                                                /* SBSW_NvM_AccessMacJobPtr */
}

/**********************************************************************************************************************
 *  NvM_Mac_Process
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, NVM_PRIVATE_CODE) NvM_Mac_Process(NvM_MacJobPtrType Self)
{
  if (Self->State == NVM_MACJOBSTATE_PENDING)
  {
    boolean csmJobSucceeded;
    Std_ReturnType csmResult = E_NOT_OK;

    Self->CsmAttemptCounter++;                                                                                          /* SBSW_NvM_AccessMacJobPtr */

    /* Process configured Csm job */

    if (Self->JobType == NVM_MACJOBTYPE_GENERATE)
    {
      csmJobSucceeded = NvM_Mac_PerformCsmGenerateJobAttempt(Self, &csmResult);                                         /* SBSW_NvM_MacInternalFunctions */
    }
    else /* VERIFY */
    {
      csmJobSucceeded = NvM_Mac_PerformCsmVerifyJobAttempt(Self, &csmResult);                                           /* SBSW_NvM_MacInternalFunctions */
    }

    /* Validate job attempt result */

    if (csmJobSucceeded)
    {
      Self->State = NVM_MACJOBSTATE_FINISHED_SUCCESSFUL;                                                                /* SBSW_NvM_AccessMacJobPtr */
    }
    else if ((csmResult == CRYPTO_E_BUSY) || (csmResult == CRYPTO_E_QUEUE_FULL))
    {
      /* CSM module busy. Evaluate retry */

      if (Self->CsmAttemptCounter >= (NVM_CSM_RETRY_COUNT + 1u))
      {
        /* Retry attempts exceeded */
        Self->State = NVM_MACJOBSTATE_FINISHED_UNSUCCESSFUL;                                                            /* SBSW_NvM_AccessMacJobPtr */
      }
    }
    else
    {
      /* Error => abort */
      Self->State = NVM_MACJOBSTATE_FINISHED_UNSUCCESSFUL;                                                              /* SBSW_NvM_AccessMacJobPtr */
    }
  }
}

/**********************************************************************************************************************
 *  NvM_Mac_GetStatus
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(NvM_DataIntegrityStatus, NVM_PRIVATE_CODE) NvM_Mac_GetStatus(NvM_MacJobConstPtrType Self)
{
  NvM_DataIntegrityStatus result;

  switch (Self->State)
  {
    case NVM_MACJOBSTATE_PENDING:
      result = NVM_DATAINTEGRITYSTATUS_PENDING;
      break;

    case NVM_MACJOBSTATE_FINISHED_SUCCESSFUL:
      result = NVM_DATAINTEGRITYSTATUS_FINISHED_SUCCESSFUL;
      break;

    case NVM_MACJOBSTATE_FINISHED_UNSUCCESSFUL:
    default:                                                                                                            /* COV_NVM_MISRA */
      result = NVM_DATAINTEGRITYSTATUS_FINISHED_UNSUCCESSFUL;
      break;
  }

  return result;
}

/**********************************************************************************************************************
 *  NvM_Mac_Export
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, NVM_PRIVATE_CODE) NvM_Mac_Export(
    NvM_MacJobConstPtrType Self,
    NvM_RamAddressType DestPtr)
{
  uint32 i;
  for (i = 0u; i < Self->BlockMacLength; i++)                                                                           /* FETA_NVM_Mac_ExportAndCompare */
  {
    DestPtr[i] = Self->MacBuffer[i];                                                                                    /* SBSW_NvM_MacExportFunctionByteCopy */
  }
}

/**********************************************************************************************************************
 *  NvM_Mac_Compare
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(boolean, NVM_PRIVATE_CODE) NvM_Mac_Compare(
    NvM_MacJobConstPtrType Self,
    NvM_ConstRamAddressType ToCompare)
{
  boolean isEqual = TRUE;

  uint32 i;
  for (i = 0u; i < Self->BlockMacLength; i++)                                                                           /* FETA_NVM_Mac_ExportAndCompare */
  {
    if (ToCompare[i] != Self->MacBuffer[i])
    {
      isEqual = FALSE;
      break;
    }
  }

  return isEqual;
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (NVM_CSM_MAC_DATAINTEGRITY_ENABLED == STD_ON) */


/**********************************************************************************************************************
 *  END OF FILE: NvM_Mac.c
 *********************************************************************************************************************/
