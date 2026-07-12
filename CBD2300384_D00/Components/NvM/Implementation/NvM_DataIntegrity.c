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
/*!        \file  NvM_DataIntegrity.c
 *        \brief  NvM DataIntegrity submodule source file
 *      \details  This submodule implements the integrity data calculation and abstracts from the concrete
 *                mechanism (CRC, MAC...).
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define NVM_DATAINTEGRITY_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* NvM_JobProc.h includes all needed headers, e.g. configuration, action, query... */
#include "NvM_JobProc.h"
#include "NvM_Mac.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#ifndef NVM_LOCAL                                                                                                       /* COV_NVM_COMPATIBILITY */
# define NVM_LOCAL static
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define NVM_START_SEC_CODE
# include "NvM_MemMap.h"                                                                                                    /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  NvM_DataIntegrity_WireCrcModule
 *********************************************************************************************************************/
/*!
 * \brief           Wires the instance NvM_DataIntegrity_CrcSubModule to the given data integrity job
 * \details         Assigns all APIs and SubJobPtr correctly
 * \param[in,out]   Self   Pointer to DataIntegrity job
 * \pre             -
 * \config          -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 */
NVM_LOCAL FUNC(void, NVM_PRIVATE_CODE) NvM_DataIntegrity_WireCrcModule(NvM_DataIntegrityJobPtrType Self);

#if (NVM_CSM_MAC_DATAINTEGRITY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  NvM_DataIntegrity_WireMacModule
 *********************************************************************************************************************/
/*!
 * \brief           Wire the instance Nvm_DataIntegrity_MacSubModule to the given data integrity job
 * \details         Assigns all APIs and SubJobPtr correctly
 * \param[in,out]   Self   Pointer to DataIntegrity job
 * \pre             -
 * \config          NVM_CSM_MAC_DATAINTEGRITY_ENABLED
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 */
NVM_LOCAL FUNC(void, NVM_PRIVATE_CODE) NvM_DataIntegrity_WireMacModule(NvM_DataIntegrityJobPtrType Self);
#endif

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"                                                                                                     /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
# define NVM_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "NvM_MemMap.h"                                                                                                    /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*
 * CRC module instance for job forwarding
 */
NVM_LOCAL VAR(NvM_CrcJobType, NVM_PRIVATE_DATA) NvM_DataIntegrity_CrcSubModule;                                         /* PRQA S 3218 */ /* MD_NvM_8.9_DataIntegrity_SubmoduleUsedOnlyInOneFunction */

#if (NVM_CSM_MAC_DATAINTEGRITY_ENABLED == STD_ON)
/*
 * MAC module instance for job forwarding
 */
NVM_LOCAL VAR(NvM_MacJobStructType, NVM_PRIVATE_DATA) NvM_DataIntegrity_MacSubModule;                                   /* PRQA S 3218 */ /* MD_NvM_8.9_DataIntegrity_SubmoduleUsedOnlyInOneFunction */
#endif

# define NVM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "NvM_MemMap.h"                                                                                                    /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define NVM_START_SEC_CODE
# include "NvM_MemMap.h"                                                                                                    /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * NvM_DataIntegrity_CreateGenerateJob
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, NVM_PRIVATE_CODE) NvM_DataIntegrity_CreateGenerateJob(
    NvM_DataIntegrityJobPtrType Self,
    NvM_BlockIdType BlockId,
    NvM_RamAddressType RamDataPtr,
    NvM_RamAddressType TargetBuffer,
    uint16 DataLength)
{
  /* Bind currently running sub job depending on block settings */
#if (NVM_CSM_MAC_DATAINTEGRITY_ENABLED == STD_ON)
  const NvM_BlockDescrPtrType blockDesc = &(NvM_BlockDescriptorTable_at[NVM_BLOCK_FROM_DCM_ID(BlockId)]);

  if (blockDesc->DataIntegritySettings == NVM_BLOCK_MAC_ON)
  {
    NvM_DataIntegrity_WireMacModule(Self);                                                                              /* SBSW_NvM_FuncCall_DataIntegrityForwarding */

    NvM_Mac_CreateGenerateJob(                                                                                          /* SBSW_NvM_FuncCall_DataIntegrityForwarding */
        (NvM_MacJobPtrType)Self->SubJobPtr,                                                                             /* PRQA S 0316 */ /* MD_NvM_11.5_DataIntegrityJob_CastVoidPtrToObjPtr */
        BlockId,
        RamDataPtr,
        TargetBuffer,
        DataLength);
  }
  else
#endif
  {
    NvM_DataIntegrity_WireCrcModule(Self);                                                                              /* SBSW_NvM_FuncCall_DataIntegrityForwarding */

    NvM_CrcJob_CreateGenerateJob(                                                                                       /* SBSW_NvM_FuncCall_DataIntegrityForwarding */
        (NvM_CrcJobPtrType)Self->SubJobPtr,                                                                             /* PRQA S 0316 */ /* MD_NvM_11.5_DataIntegrityJob_CastVoidPtrToObjPtr */
        BlockId,
        RamDataPtr,
        DataLength,
        TargetBuffer,
        NvM_NoOfCrcBytes_u16);
  }
}


/**********************************************************************************************************************
 * NvM_DataIntegrity_CreateValidateJob
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, NVM_PRIVATE_CODE) NvM_DataIntegrity_CreateValidateJob(
    NvM_DataIntegrityJobPtrType Self,
    NvM_BlockIdType BlockId,
    NvM_RamAddressType RamDataPtr,
    NvM_RamAddressType VerifyBuffer,
    uint16 DataLength)
{
  /* Bind currently running sub job depending on block settings */
#if (NVM_CSM_MAC_DATAINTEGRITY_ENABLED == STD_ON)
  const NvM_BlockDescrPtrType blockDesc = &(NvM_BlockDescriptorTable_at[NVM_BLOCK_FROM_DCM_ID(BlockId)]);

  if (blockDesc->DataIntegritySettings == NVM_BLOCK_MAC_ON)
  {
    NvM_DataIntegrity_WireMacModule(Self);                                                                              /* SBSW_NvM_FuncCall_DataIntegrityForwarding */

    NvM_Mac_CreateValidateJob(                                                                                          /* SBSW_NvM_FuncCall_DataIntegrityForwarding */
        (NvM_MacJobPtrType)Self->SubJobPtr,                                                                             /* PRQA S 0316 */ /* MD_NvM_11.5_DataIntegrityJob_CastVoidPtrToObjPtr */
        BlockId,
        RamDataPtr,
        VerifyBuffer,
        DataLength);
  }
  else
#endif
  {
    NvM_DataIntegrity_WireCrcModule(Self);                                                                              /* SBSW_NvM_FuncCall_DataIntegrityForwarding */

    NvM_CrcJob_CreateValidateJob(
        (NvM_CrcJobPtrType)Self->SubJobPtr,                                                                             /* PRQA S 0316 */ /* MD_NvM_11.5_DataIntegrityJob_CastVoidPtrToObjPtr */
        BlockId,
        RamDataPtr,
        DataLength,
        VerifyBuffer,
        NvM_NoOfCrcBytes_u16);                                                                                          /* SBSW_NvM_FuncCall_DataIntegrityForwarding */
  }
}

/**********************************************************************************************************************
 * NvM_DataIntegrity_Process
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, NVM_PRIVATE_CODE) NvM_DataIntegrity_Process(
    NvM_DataIntegrityJobConstPtrType Self)
{

    Self->Process(Self->SubJobPtr);                                                                                     /* SBSW_NvM_FuncCall_DataIntegrityForwarding */
}

/**********************************************************************************************************************
 *  NvM_DataIntegrity_GetStatus
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(NvM_DataIntegrityStatus, NVM_PRIVATE_CODE) NvM_DataIntegrity_GetStatus(
    NvM_DataIntegrityJobConstPtrType Self)
{
  return Self->GetStatus(Self->SubJobPtr);                                                                              /* SBSW_NvM_FuncCall_DataIntegrityForwarding */
}

/**********************************************************************************************************************
 *  NvM_DataIntegrity_ExportDataIntegrityRecord
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, NVM_PRIVATE_CODE) NvM_DataIntegrity_ExportDataIntegrityRecord(
    NvM_DataIntegrityJobConstPtrType Self,
    NvM_RamAddressType DestPtr)
{
  Self->Export(Self->SubJobPtr, DestPtr);                                                                               /* SBSW_NvM_FuncCall_DataIntegrityForwarding */
}

/**********************************************************************************************************************
 * NvM_DataIntegrity_Compare
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, NVM_PRIVATE_CODE) NvM_DataIntegrity_Compare(
    NvM_DataIntegrityJobConstPtrType Self,
    NvM_ConstRamAddressType ToCompare)
{
    return Self->Compare(Self->SubJobPtr, ToCompare);    /* SBSW_NvM_FuncPtrCall_DataIntegrityModule */
}

/**********************************************************************************************************************
 * NvM_DataIntegrity_GetIntegrityCodeLength
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(uint16, NVM_PRIVATE_CODE) NvM_DataIntegrity_GetIntegrityCodeLength(
    NvM_BlockIdType BlockId)
{    
  uint16 integrityCodeLength;
  const NvM_BlockDescrPtrType blockDesc = &(NvM_BlockDescriptorTable_at[NVM_BLOCK_FROM_DCM_ID(BlockId)]);

  switch (blockDesc->DataIntegritySettings)
  {
  case NVM_BLOCK_CRC_16_ON:                                                                                             /* COV_NVM_COVEREDINOTHERCFG */
    integrityCodeLength = 2u;
    break;
  case NVM_BLOCK_CRC_32_ON:                                                                                             /* COV_NVM_COVEREDINOTHERCFG */
    integrityCodeLength = 4u;
    break;

#if (NVM_CSM_MAC_DATAINTEGRITY_ENABLED == STD_ON)
  case NVM_BLOCK_MAC_ON:
    integrityCodeLength = blockDesc->MacSize;
    break;
#endif

    /* Note: CRC8 is not supported by NvM. All non supported cases shall return 0 length */
  default:
    integrityCodeLength = 0u;
    break;
  }

  return integrityCodeLength;
}

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  NvM_DataIntegrity_WireCrcModule
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
NVM_LOCAL FUNC(void, NVM_PRIVATE_CODE) NvM_DataIntegrity_WireCrcModule(NvM_DataIntegrityJobPtrType Self)
{
  Self->SubJobPtr = &NvM_DataIntegrity_CrcSubModule;                                                                    /* PRQA S 0315 */ /* MD_NvM_11.5_DataIntegrityJob_CastVoidPtrToObjPtr */ /* SBSW_NvM_AccessDataIntegrityJobPtr */

  Self->Process = (NvM_DataIntegrityProcessFPtr)&NvM_CrcJob_Process;                                                    /* PRQA S 0313 */ /* MD_NvM_1.1_DataIntegrity_IncompatibleFPtrTypes */ /* SBSW_NvM_AccessDataIntegrityJobPtr */
  Self->GetStatus = (NvM_DataIntegrityGetStatusFPtr)&NvM_CrcJob_GetStatus;                                              /* PRQA S 0313 */ /* MD_NvM_1.1_DataIntegrity_IncompatibleFPtrTypes */ /* SBSW_NvM_AccessDataIntegrityJobPtr */
  Self->Compare = (NvM_DataIntegrityCompareFPtr)&NvM_CrcJob_CompareToExternal;                                          /* PRQA S 0313 */ /* MD_NvM_1.1_DataIntegrity_IncompatibleFPtrTypes */ /* SBSW_NvM_AccessDataIntegrityJobPtr */
  Self->Export = (NvM_DataIntegrityExportFPtr)&NvM_CrcJob_ExportBufferedValue;                                          /* PRQA S 0313 */ /* MD_NvM_1.1_DataIntegrity_IncompatibleFPtrTypes */ /* SBSW_NvM_AccessDataIntegrityJobPtr */
}

#if (NVM_CSM_MAC_DATAINTEGRITY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  NvM_DataIntegrity_WireMacModule
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
NVM_LOCAL FUNC(void, NVM_PRIVATE_CODE) NvM_DataIntegrity_WireMacModule(NvM_DataIntegrityJobPtrType Self)
{
  Self->SubJobPtr = &NvM_DataIntegrity_MacSubModule;                                                                    /* PRQA S 0315 */ /* MD_NvM_11.5_DataIntegrityJob_CastVoidPtrToObjPtr */ /* SBSW_NvM_AccessDataIntegrityJobPtr */

  Self->Process = (NvM_DataIntegrityProcessFPtr)&NvM_Mac_Process;                                                       /* PRQA S 0313 */ /* MD_NvM_1.1_DataIntegrity_IncompatibleFPtrTypes */ /* SBSW_NvM_AccessDataIntegrityJobPtr */
  Self->GetStatus = (NvM_DataIntegrityGetStatusFPtr)&NvM_Mac_GetStatus;                                                 /* PRQA S 0313 */ /* MD_NvM_1.1_DataIntegrity_IncompatibleFPtrTypes */ /* SBSW_NvM_AccessDataIntegrityJobPtr */
  Self->Compare = (NvM_DataIntegrityCompareFPtr)&NvM_Mac_Compare;                                                       /* PRQA S 0313 */ /* MD_NvM_1.1_DataIntegrity_IncompatibleFPtrTypes */ /* SBSW_NvM_AccessDataIntegrityJobPtr */
  Self->Export = (NvM_DataIntegrityExportFPtr)&NvM_Mac_Export;                                                          /* PRQA S 0313 */ /* MD_NvM_1.1_DataIntegrity_IncompatibleFPtrTypes */ /* SBSW_NvM_AccessDataIntegrityJobPtr */
}
#endif

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"                                                                                                     /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: NvM_DataIntegrity.c
 *********************************************************************************************************************/
