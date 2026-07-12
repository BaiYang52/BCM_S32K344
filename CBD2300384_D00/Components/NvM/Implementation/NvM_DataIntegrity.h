/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  NvM_DataIntegrity.h
 *        \brief  NvM DataInetgrity submodule header file
 *      \details  This submodule implements the integrity data calculation and abstracts from the concrete mechanism (CRC, MAC...).
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if (!defined NVM_DATAINTEGRITY_H_)
# define NVM_DATAINTEGRITY_H_

# include "NvM_Cfg.h"
# include "NvM_PrivateCfg.h"
# include "NvM_IntTypes.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef enum 
{
    NVM_DATAINTEGRITYSTATUS_PENDING,                           /*!< Computation of the integrity code is still running */
    NVM_DATAINTEGRITYSTATUS_FINISHED_SUCCESSFUL,               /*!< Computation of the integrity code succeeded */
    NVM_DATAINTEGRITYSTATUS_FINISHED_UNSUCCESSFUL              /*!< Computation of the integrity code failed (e.g. to many csm retries).
                                                            Negative result might not occur for every concrete data integrity mechanism
                                                            e.g. Crc will never report such a result, it always succeeds) */
} NvM_DataIntegrityStatus;

typedef P2VAR(struct NvM_DataIntegrityJobStruct, AUTOMATIC, NVM_PRIVATE_DATA ) NvM_DataIntegrityJobPtrType;
typedef P2CONST(struct NvM_DataIntegrityJobStruct, AUTOMATIC, NVM_PRIVATE_DATA) NvM_DataIntegrityJobConstPtrType;

typedef P2FUNC(void, NVM_PRIVATE_CODE, NvM_DataIntegrityProcessFPtr) (void* Self);
typedef P2FUNC(NvM_DataIntegrityStatus, NVM_PRIVATE_CODE, NvM_DataIntegrityGetStatusFPtr) (const void* Self);
typedef P2FUNC(boolean, NVM_PRIVATE_CODE, NvM_DataIntegrityCompareFPtr) (const void* Self, NvM_ConstRamAddressType ToCompare);
typedef P2FUNC(void, NVM_PRIVATE_CODE, NvM_DataIntegrityExportFPtr) (const void* Self, NvM_RamAddressType ToCompare);

typedef struct NvM_DataIntegrityJobStruct
{
    NvM_DataIntegrityProcessFPtr Process;                 /*!< Function pointer pointing to the Process function of the concrete data integrity mechanism */
    NvM_DataIntegrityGetStatusFPtr GetStatus;             /*!< Function pointer pointing to the GetStatus function of the concrete data integrity mechanism */
    NvM_DataIntegrityCompareFPtr Compare;                 /*!< Function pointer pointing to the Compare function of the concrete data integrity mechanism */
    NvM_DataIntegrityExportFPtr Export;                   /*!< Function pointer pointing to the Export function of the concrete data integrity mechanism */

    void* SubJobPtr;                                      /*!< Pointer to the data of the sub job */
} NvM_DataIntegrityJob;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define NVM_START_SEC_CODE
# include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 * NvM_DataIntegrity_CreateGenerateJob
 *********************************************************************************************************************/
/*! \brief   Creates a Generate job.
 *  \details Sets up the function pointers and the data pointer according to the used algorithm and forwards the call 
 *           to the corresponding submodule.
 *  \param[in,out] Self Pointer to the data integrity job structure that holds the necessary function pointer and data. Pointer must be valid.
 *  \param[in] BlockId BlockId in range [1, (number of blocks - 1)].
 *  \param[in] RamDataPtr Pointer holding the data to calculate the integrity code over. Pointer must be valid.
 *  \param[in] TargetBuffer Pointer where the integrity code will be written at the end of the calculation. Pointer must be valid.
 *  \param[in] DataLength of the data to calculate the integrity code over. Must not exceed the buffer length.
 *  \context TASK
 *  \reentrant FALSE
 *  \pre -
 *  \synchronous TRUE
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_DataIntegrity_CreateGenerateJob(
    NvM_DataIntegrityJobPtrType Self,
    NvM_BlockIdType BlockId,
    NvM_RamAddressType RamDataPtr,
    NvM_RamAddressType TargetBuffer,
    uint16 DataLength);


/**********************************************************************************************************************
 * NvM_DataIntegrity_CreateValidateJob
 *********************************************************************************************************************/
/*! \brief   Creates a Validate job.
 *  \details Sets up the function pointers and the data pointer according to the used algorithm and forwards the call
 *           to the corresponding submodule.
 *  \param[in,out] Self Pointer to the data integrity job structure that holds the necessary function pointer and data. Pointer must be valid.
 *  \param[in] BlockId BlockId in range [1, (number of blocks - 1)].
 *  \param[in] RamDataPtr Pointer holding the data to calculate the integrity code over. Pointer must be valid.
 *  \param[in] VerifyBuffer Pointer that points to the expected integrity code that shall be checked. Pointer must be valid.
 *  \param[in] DataLength of the data to calculate the integrity code over. Must not exceed the buffer length.
 *  \context TASK
 *  \reentrant FALSE
 *  \pre -
 *  \synchronous TRUE
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_DataIntegrity_CreateValidateJob(
    NvM_DataIntegrityJobPtrType Self,
    NvM_BlockIdType BlockId,
    NvM_RamAddressType RamDataPtr,
    NvM_RamAddressType VerifyBuffer,
    uint16 DataLength);

/**********************************************************************************************************************
 * NvM_DataIntegrity_Process
 *********************************************************************************************************************/
/*! \brief Cyclically called function for processing the data integrity calculation.
 *  \details Forwards the call to the corresponding submodule (via function pointer).
 *  \param[in,out] Self Pointer to the data integrity job structure that holds the necessary function pointer and data. Pointer must be valid.
 *  \context TASK
 *  \reentrant FALSE
 *  \pre -
 *  \synchronous TRUE
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_DataIntegrity_Process(
    NvM_DataIntegrityJobConstPtrType Self);


/**********************************************************************************************************************
 *  NvM_DataIntegrity_GetStatus
 *********************************************************************************************************************/
/*!
 * \brief           Return status of DataIntegrity module.
 * \details         Forwards the get status request to the corresponding submodule.
 * \param[in]       Self   Pointer to data integrity job. Pointer must be valid.
 * \return          return DataIntegrity job status of corresponding submodule.
 * \pre             -
 * \config          -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 */
extern FUNC(NvM_DataIntegrityStatus, NVM_PRIVATE_CODE) NvM_DataIntegrity_GetStatus(
    NvM_DataIntegrityJobConstPtrType Self);

/**********************************************************************************************************************
 *  NvM_DataIntegrity_ExportDataIntegrityRecord
 *********************************************************************************************************************/
/*!
 * \brief           Copy data integrity record of given job into the DestPtr.
 * \details         -
 * \param[in,out]   Self      Pointer to DataIntegrity job. Pointer must be valid.
 * \param[in,out]   DestPtr   Target pointer to copy DataIntegrity record to. Pointer must be valid.
 * \pre             -
 * \config          -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_DataIntegrity_ExportDataIntegrityRecord(
    NvM_DataIntegrityJobConstPtrType Self,
    NvM_RamAddressType DestPtr);

/**********************************************************************************************************************
 * NvM_DataIntegrity_Compare
 *********************************************************************************************************************/
/*! \brief Compares the calculated integrity code with given data from outside.
 *  \details Forwards the call to the corresponding submodule (via function pointer).
 *  \param[in] Self Pointer to the data integrity job structure that holds the necessary function pointer and data. Pointer must be valid.
 *  \param[in] ToCompare Value that shall be compared with the calculated integrity code. Pointer must be valid.
 *  \return TRUE Integrity code match
 *  \return FALSE Integrity code mismatch
 *  \context TASK
 *  \reentrant FALSE
 *  \pre Generate job must have been finished before
 *  \synchronous TRUE
 */
extern FUNC(boolean, NVM_PRIVATE_CODE) NvM_DataIntegrity_Compare(
    NvM_DataIntegrityJobConstPtrType Self,
    NvM_ConstRamAddressType ToCompare);

/**********************************************************************************************************************
 * NvM_DataIntegrity_GetIntegrityCodeLength
 *********************************************************************************************************************/
/*! \brief Returns the length that is needed for the integrity code.
 *  \details -
 *  \param[in] Self Pointer to the data integrity job structure that holds the necessary function pointer and data. Pointer must be valid.
 *  \param[in] BlockId BlockId in range [1, (number of blocks - 1)].
 *  \return length of the integrity code in bytes.
 *  \context TASK
 *  \reentrant FALSE
 *  \pre -
 *  \synchronous TRUE
 */
extern FUNC(uint16, NVM_PRIVATE_CODE) NvM_DataIntegrity_GetIntegrityCodeLength(
    NvM_BlockIdType BlockId);


# define NVM_STOP_SEC_CODE
# include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* NVM_DATAINTEGRITY_H_ */

/**********************************************************************************************************************
 *  END OF FILE: NvM_DataIntegrity.h
 *********************************************************************************************************************/
