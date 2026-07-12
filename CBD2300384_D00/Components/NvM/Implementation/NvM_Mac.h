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
/*!        \file
 *        \brief  NvM MAC handling for data integrity
 *      \details  -
 *********************************************************************************************************************/

#if !defined (NVM_MAC_H_)
# define NVM_MAC_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "NvM_JobProc.h"
# include "NvM_DataIntegrity.h"

#if (NVM_CSM_MAC_DATAINTEGRITY_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*
 * Enumeration holding job status of MAC processing
 */
typedef enum
{
  NVM_MACJOBSTATE_PENDING,                             /*!< Job is currently running */
  NVM_MACJOBSTATE_FINISHED_SUCCESSFUL,                 /*!< Job has finished with success */
  NVM_MACJOBSTATE_FINISHED_UNSUCCESSFUL                /*!< Job has finished erroneous */
} NvM_MacJobState;

/*
 * Enumeration for supported job types
 */
typedef enum
{
  NVM_MACJOBTYPE_GENERATE,                            /*!< Generation job requested */
  NVM_MACJOBTYPE_VERIFY                               /*!< Verification job requested */
} NvM_MacJobType;

/*
 * Structure storing all information, required for MAC job handling
 */
typedef struct
{
  NvM_MacJobType JobType;                           /*!< Job type that is requested */
  NvM_MacJobState State;                            /*!< Current state of job */
  uint32 CsmMacJobReference;                        /*!< Job ID for CSM API */
  uint32 BlockDataLength;                           /*!< Block length */
  NvM_RamAddressType BlockDataPtr;                  /*!< Block data pointer */
  NvM_RamAddressType MacBuffer;                     /*!< Target buffer where MAC is placed */
  uint32 BlockMacLength;                            /*!< Configured MAC length of block */
  NvM_BlockIdType BlockId;                          /*!< Block identifier */
  uint8 CsmAttemptCounter;                          /*!< Attempt counter */
} NvM_MacJobStructType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
typedef P2VAR(NvM_MacJobStructType, AUTOMATIC, NVM_PRIVATE_DATA) NvM_MacJobPtrType;
typedef P2CONST(NvM_MacJobStructType, AUTOMATIC, NVM_PRIVATE_DATA) NvM_MacJobConstPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define NVM_START_SEC_CODE
# include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  NvM_Mac_CreateGenerateJob
 *********************************************************************************************************************/
/*!
 * \brief             Create a MAC generate job
 * \details           -
 * \param[in,out]     Self              Pointer to the MAC job structure that holds the necessary function pointer and data. Pointer must be valid.
 * \param[in]         BlockId           BlockId in range [1, (number of blocks - 1)]
 * \param[in]         RamDataPtr        Pointer holding the data to calculate the integrity code over. Pointer must be valid.
 * \param[in]         MacTargetBuffer   Pointer where the integrity code will be written at the end of the calculation. Pointer must be valid.
 * \param[in]         DataLength        Length of the data to calculate the integrity code over. Must not exceed the buffer length.
 * \pre               -
 * \config            NVM_CSM_MAC_DATAINTEGRITY_ENABLED
 * \context           TASK
 * \reentrant         TRUE
 * \synchronous       TRUE
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_Mac_CreateGenerateJob(
    NvM_MacJobPtrType Self,
    NvM_BlockIdType BlockId,
    NvM_RamAddressType RamDataPtr,
    NvM_RamAddressType MacTargetBuffer,
    uint16 DataLength);

/**********************************************************************************************************************
 *  NvM_Mac_CreateValidateJob
 *********************************************************************************************************************/
/*!
 * \brief           Create a MAC validation job
 * \details         -
 * \param[in,out]   Self                Pointer to the MAC job structure that holds the necessary function pointer and data. Pointer must be valid.
 * \param[in]       BlockId             BlockId in range [1, (number of blocks - 1)]
 * \param[in]       RamDataPtr          Pointer holding the data to calculate the integrity code over. Pointer must be valid.
 * \param[in]       MacVerifyBuffer     Pointer to MAC that is used for verification. Pointer must be valid.
 * \param[in]       DataLength          Length of the data to calculate the integrity code over. Must not exceed the buffer length.
 * \pre             -
 * \config          NVM_CSM_MAC_DATAINTEGRITY_ENABLED
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_Mac_CreateValidateJob(
    NvM_MacJobPtrType Self,
    NvM_BlockIdType BlockId,
    NvM_RamAddressType RamDataPtr,
    NvM_RamAddressType MacVerifyBuffer,
    uint16 DataLength);

/**********************************************************************************************************************
 *  NvM_Mac_Process
 *********************************************************************************************************************/
/*!
 * \brief           Process the given MAC job
 * \details         -
 * \param[in,out]   Self Pointer to the MAC job structure that holds the necessary function pointer and data. Pointer must be valid.
 * \pre             -
 * \config          NVM_CSM_MAC_DATAINTEGRITY_ENABLED
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_Mac_Process(NvM_MacJobPtrType Self);

/**********************************************************************************************************************
 *  NvM_Mac_GetStatus
 *********************************************************************************************************************/
/*!
 * \brief           Returns the status of currently processed validation or generation job
 * \details         -
 * \param[in,out]   Self Pointer to MAC job structure. Pointer must be valid.
 * \return          Current status of processing
 * \pre             -
 * \config          NVM_CSM_MAC_DATAINTEGRITY_ENABLED
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 */
extern FUNC(NvM_DataIntegrityStatus, NVM_PRIVATE_CODE) NvM_Mac_GetStatus(NvM_MacJobConstPtrType Self);

/**********************************************************************************************************************
 *  NvM_Mac_Export
 *********************************************************************************************************************/
/*!
 * \brief           Export MAC to a given buffer
* \details          MAC target or verification buffer is used, dependent on preceding job
 * \param[in,out]   Self      Pointer to MAC job structure. Pointer must be valid.
 * \param[in,out]   DestPtr   Target buffer for MAC. Pointer must be valid.
 * \pre             Generate/Validate job must be generated and processed successfully before calling this function
 * \config          NVM_CSM_MAC_DATAINTEGRITY_ENABLED
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_Mac_Export(
    NvM_MacJobConstPtrType Self,
    NvM_RamAddressType DestPtr);

/**********************************************************************************************************************
 *  NvM_Mac_Compare
 *********************************************************************************************************************/
/*!
 * \brief           Compare MAC to a given buffer.
 * \details         MAC target or verification buffer is used, dependent on preceding job
 * \param[in,out]   Self        Pointer to MAC job structure. Pointer must be valid.
 * \param[in]       ToCompare   Buffer used for comparison (shall hold the MAC to compare to). Pointer must be valid.
 * \return          TRUE if MAC bytes are equal; FALSE otherwise
 * \pre             Generate/Validate job must be generated and processed successfully before calling this function
 * \config          NVM_CSM_MAC_DATAINTEGRITY_ENABLED
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 */
extern FUNC(boolean, NVM_PRIVATE_CODE) NvM_Mac_Compare(
    NvM_MacJobConstPtrType Self,
    NvM_ConstRamAddressType ToCompare);

# define NVM_STOP_SEC_CODE
# include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (NVM_CSM_MAC_DATAINTEGRITY_ENABLED == STD_ON) */

#endif /* NVM_MAC_H_ */

/**********************************************************************************************************************
 *  END OF FILE: NvM_Mac.h
 *********************************************************************************************************************/
