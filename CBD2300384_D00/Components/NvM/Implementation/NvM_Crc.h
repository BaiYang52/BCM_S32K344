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
/*!        \file  NvM_Crc.h
 *        \brief  NvM CRC submodule header file
 *      \details  This submodule implements the CRC calculation.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if (!defined NVM_CRC_H_)
# define NVM_CRC_H_

#include "NvM_DataIntegrity.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Define optimal pointer class for crc buffer, depending on NVM_DATA_INTEGRITY_INT_BUFFER setting.                            *
 * CRC buffer will either point to user RAM Block (NVM_APPL_DATA) or to an internal buffer (NVM_PRIVATE_DATE)       *
 * Since it NVM_DATA_INTEGRITY_INT_BUFFER is is pre-compile, so we can do it...                                                */
# if(NVM_DATA_INTEGRITY_INT_BUFFER == STD_ON)
#  define NVM_CRC_BUFFER_PTR_CLASS NVM_PRIVATE_DATA
# else
#  define NVM_CRC_BUFFER_PTR_CLASS NVM_APPL_DATA
# endif

# define NvM_CrcJob_GetCrcLength(selfPtr)           ((selfPtr)->HandlerInstance_pt->crcLength)                          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define NvM_CrcJob_isBusy(selfPtr)                 ((selfPtr)->RemainingLength_u16 != 0u)                              /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define NvM_CrcJob_ReassignBuffer(selfPtr, crcPtr) ((selfPtr)->CrcBuffer = (crcPtr))                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* CRC queue will only be available if the SetRamBlockStatus API and NvMCalcRamCrc are enabled. */
# if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_CALC_RAM_CRC_USED == STD_ON))

/* Define the base type for the CRC queue bitstring. */
#  define NVM_CRC_QUEUE_ENTRY_BITS     32u
#  define NVM_CRC_QUEUE_BITINDEX_MASK  0x1Fu
#  define NVM_CRC_QUEUE_ENTRY_SHIFT    5
#  define NVM_CRC_QUEUE_ENTRY_MASK     0xFFFFFFFFu

# endif /* ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_CALC_RAM_CRC_USED == STD_ON)) */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef P2VAR(uint8, AUTOMATIC, NVM_CRC_BUFFER_PTR_CLASS) NvM_CrcBufferPtrType;
typedef P2CONST(uint8, AUTOMATIC, NVM_CRC_BUFFER_PTR_CLASS) NvM_CrcBufferConstPtrType;

typedef P2VAR(uint32, AUTOMATIC, NVM_PRIVATE_DATA) NvM_CrcValuePtrType;
typedef P2CONST(uint8, AUTOMATIC, NVM_PRIVATE_DATA) NvM_CrcValueRefType;

typedef P2FUNC(void, NVM_PRIVATE_CODE, NvM_CrcCalculateFPtr) (NvM_ConstRamAddressType DataPtr, uint16 Length, NvM_CrcValuePtrType CurrentValue, boolean IsFirstCall);
typedef P2FUNC(boolean, NVM_PRIVATE_CODE, NvM_CrcCompareFPtr) (NvM_CrcBufferConstPtrType CrcBuff, NvM_CrcValueRefType CurrentValue);
typedef P2FUNC(void, NVM_PRIVATE_CODE, NvM_CrcCopyToBufferFPtr) (NvM_CrcBufferPtrType Dest, NvM_CrcValueRefType Src);


struct NvM_CrcHandlerClass
{
  NvM_CrcCalculateFPtr    calc;                         /* stores the function pointer to CRC calculation function */
  NvM_CrcCompareFPtr      compare;                      /* stores the function pointer to CRC comparison function */
  NvM_CrcCopyToBufferFPtr copyToBuffer;                 /* stores the function pointer to CRC copy function */
  uint32                  initialCrcValue;              /* stores the initial value for CRC calculation */
  uint8                   crcLength;                    /* stores the CRC length in bytes */
}; /*!< Structure storing all required functions and values to calculate a CRC. */

typedef P2CONST(struct NvM_CrcHandlerClass, AUTOMATIC, NVM_PRIVATE_CONST) NvM_CrcHandlerClassConstPtr;


/*!
 * Enumeration describing internal CRC module processing status
 */
typedef enum
{
  /*!< Compatibility mode, old API usage requested */
  NVM_CRCPROCESSINGSTATUS_COMPATIBILITY,

  /* Sub state paths to GENERATE */
  NVM_CRCPROCESSINGSTATUS_GENERATE_PENDING,                 /* Automatic generation job pending */
  NVM_CRCPROCESSINGSTATUS_GENERATE_FINISHED,                /* Automatic generation job finished */

  /* Sub state paths to VALIDATE */
  NVM_CRCPROCESSINGSTATUS_VALIDATE_GENERATION_PENDING,      /* Automatic validation job pending */
  NVM_CRCPROCESSINGSTATUS_VALIDATE_FINISHED_SUCCESSFUL,     /* Automatic validation job succeeded */
  NVM_CRCPROCESSINGSTATUS_VALIDATE_FINISHED_UNSUCCESSFUL    /* Automatic validation job failed */
} NvM_CrcProcessingStatus; /*!< Enumeration holding internal CRC module status */

typedef struct NvM_CrcJobStruct
{
  uint32                      CurrentCrcValue;          /* current CRC value */
  NvM_ConstRamAddressType     RamData_pt;               /* data to calculate the CRC for */
  NvM_CrcBufferPtrType        CrcBuffer;                /* CRC buffer */
  NvM_CrcHandlerClassConstPtr HandlerInstance_pt;       /* CRC handler pointer */
  NvM_CrcProcessingStatus     ProcessingStatus;         /* Enumeration representing internal status */
  uint16                      RemainingLength_u16;      /* remaining number of bytes to calculate the CRC for */
  uint16                      BytesPerProcessCycle;     /* Number of bytes to be processed for each Process() call */
  boolean                     IsFirstCall;              /* CRC API related information if first call to API */
} NvM_CrcJobType; /*!< Structure storing all information required for a CRC calculation job. */

typedef P2VAR(NvM_CrcJobType, AUTOMATIC, NVM_PRIVATE_DATA) NvM_CrcJobPtrType;
typedef P2CONST(NvM_CrcJobType, AUTOMATIC, NVM_PRIVATE_DATA) NvM_CrcJobConstPtrType;

# define NVM_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* CRC queue will only be available if the SetRamBlockStatus API and NvMCalcRamCrc are enabled. */
# if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_CALC_RAM_CRC_USED == STD_ON))

typedef uint32 NvM_CrcQueueEntryType;
typedef P2VAR(NvM_CrcQueueEntryType, AUTOMATIC, NVM_CONFIG_DATA) NvM_CrcQueueEntryPtr;

# endif /* ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_CALC_RAM_CRC_USED == STD_ON)) */

# define NVM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define NVM_START_SEC_CODE
# include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* CRC queue will only be available if the SetRamBlockStatus API and NvMCalcRamCrc are enabled. */
# if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_CALC_RAM_CRC_USED == STD_ON))
/**********************************************************************************************************************
 * NvM_CrcQueueInit
 *********************************************************************************************************************/
/*! \brief Initialization of NvM's Crc_Queue
 *  \details Initialization of NvM's Crc_Queue
 *  \context TASK
 *  \reentrant FALSE
 *  \synchronous TRUE
 *  \config SetRamBlockStatus Api is enabled and at least one block is configured with CalcRamCrc true
 *  \pre -
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_CrcQueueInit(void);

/**********************************************************************************************************************
 * NvM_CrcQueueJob
 *********************************************************************************************************************/
/*! \brief Function queues a CRC recalculation request for the given Block Id.
 *  \details Function queues a CRC recalculation request for the given Block Id.
 *  \param[in] BlockId
 *  \context TASK
 *  \reentrant FALSE
 *  \synchronous TRUE
 *  \config SetRamBlockStatus Api is enabled and at least one block is configured with CalcRamCrc true
 *  \pre -
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_CrcQueueJob(NvM_BlockIdType BlockId);

/**********************************************************************************************************************
 * NvM_CrcGetQueuedBlockId
 *********************************************************************************************************************/
/*! \brief Function checks if there are still Crc jobs queued and returns the next block for Crc calculation.
 *  \details Function checks if there are still Crc jobs queued and returns the next block for Crc calculation.
 *  \return 0 CRC queue was empty
 *  \return next block Id for Crc calculation
 *  \context TASK
 *  \reentrant FALSE
 *  \synchronous TRUE
 *  \config SetRamBlockStatus Api is enabled and at least one block is configured with CalcRamCrc true
 *  \pre -
 */
extern FUNC(NvM_BlockIdType, NVM_PRIVATE_CODE) NvM_CrcGetQueuedBlockId(void);
# endif /* ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_CALC_RAM_CRC_USED == STD_ON)) */

/**********************************************************************************************************************
 * NvM_CrcJob_Create
 *********************************************************************************************************************/
/*! \brief Creates a Crc job
 *  \details This API is available for software parts that are relying on the "old" version of CRC module usage and is
 *           for now kept as still needed in code parts of NvM. APIs to be used are
 *            - NvM_CrcJob_CreateGenerateJob: generation of CRC and automatic assignment when finished
 *            - NvM_CrcJob_CreateValidateJob: generation of CRC and automatic comparison with validation data
 *           MWDG-4972 is planned to completely remove this API.
 *  \param[in,out] Self                 Job pointer to be created.
 *  \param[in]     BlockId              BlockId in range [1, (number of blocks - 1)].
 *  \param[in]     RamDataPtr           Pointer holding the data to calculate the CRC over. NULL_PTR leads to no job creation.
 *  \param[in]     DataLength           Length of data to be processed.
 *  \param[in]     BytesPerProcessCycle Number of bytes to be processed for each API call to NvM_CrcJob_Process()
 *  \context TASK
 *  \reentrant FALSE
 *  \synchronous TRUE
 *  \pre -
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_CrcJob_Create(
    NvM_CrcJobPtrType Self,
    NvM_BlockIdType BlockId,
    NvM_RamAddressType RamDataPtr,
    uint16 DataLength,
    uint16 BytesPerProcessCycle);

/**********************************************************************************************************************
 *  NvM_CrcJob_CreateGenerateJob
 *********************************************************************************************************************/
/*!
 * \brief           Create a CRC generation job.
 * \details         Automatically stores calculated CRC to TargetBuffer after job has finished.
 * \param[in]       Self                  Job pointer to be created
 * \param[in]       BlockId               Corresponding block ID
 * \param[in]       RamDataPtr            Pointer holding the data used for CRC calculation. Must be valid.
 * \param[in]       DataLength            Length of data to be processed.
 * \param[in]       TargetBuffer          Pointer to target buffer to export generated data to.
 * \param[in]       BytesPerProcessCycle  Number of bytes to be processed for each API call to NvM_CrcJob_Process()
 * \pre             -
 * \config          -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_CrcJob_CreateGenerateJob(
    NvM_CrcJobPtrType Self,
    NvM_BlockIdType BlockId,
    NvM_RamAddressType RamDataPtr,
    uint16 DataLength,
    NvM_RamAddressType TargetBuffer,
    uint16 BytesPerProcessCycle);

/**********************************************************************************************************************
 *  NvM_CrcJob_CreateValidateJob
 *********************************************************************************************************************/
/*!
 * \brief           Create a CRC validation job.
 * \details         Generates a CRC for RamDataPtr and compares it to the data placed in VerifyBuffer.
 * \param[in]       Self                  Job pointer to be created
 * \param[in]       BlockId               Corresponding block ID
 * \param[in]       RamDataPtr            Pointer holding the data used for CRC calculation. Must be valid.
 * \param[in]       DataLength            Length of data to be processed.
 * \param[in]       VerifyBuffer          Pointer to verification buffer used for comparison
 * \param[in]       BytesPerProcessCycle  Number of bytes to be processed for each API call to NvM_CrcJob_Process()
 * \pre             -
 * \config          -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_CrcJob_CreateValidateJob(
    NvM_CrcJobPtrType Self,
    NvM_BlockIdType BlockId,
    NvM_RamAddressType RamDataPtr,
    uint16 DataLength,
    NvM_RamAddressType VerifyBuffer,
    uint16 BytesPerProcessCycle);

/**********************************************************************************************************************
 *  NvM_CrcJob_GetStatus
 *********************************************************************************************************************/
/*!
 * \brief           Returns the status of currently processed Validation or Generation job.
 * \details         -
 * \param[in,out]   Self Job pointer to be created
 * \return          Current status of processing.
 * \pre             NvM_CrcJob_CreateGenerateJob or NvM_CrcJob_CreateValidateJob must be used. NvM_CrcJob_Create API is
 *                  not taken into account as it is temporarily only available for compatibility reasons.
 * \config          -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 */
extern FUNC(NvM_DataIntegrityStatus, NVM_PRIVATE_CODE) NvM_CrcJob_GetStatus(
    NvM_CrcJobConstPtrType Self);

/**********************************************************************************************************************
 * NvM_CrcJob_Process
 *********************************************************************************************************************/
/*! \brief Cyclically called function for processing the CRC calculation (one step)
 *  \details -
 *  \param[in,out] Self
 *  \context TASK
 *  \reentrant FALSE
 *  \synchronous TRUE
 *  \pre -
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_CrcJob_Process(NvM_CrcJobPtrType Self);

/**********************************************************************************************************************
 * NvM_CrcJob_CopyToBuffer
 *********************************************************************************************************************/
/*! \brief Copy Crc into data buffer.
 *  \details Copy Crc into data buffer.
 *  \param[in,out] Self
 *  \context TASK
 *  \reentrant FALSE
 *  \synchronous TRUE
 *  \pre -
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_CrcJob_CopyToBuffer(NvM_CrcJobConstPtrType Self);

/**********************************************************************************************************************
 * NvM_CrcJob_Compare
 *********************************************************************************************************************/
/*! \brief Compare recalculated Crc against the Crc from NV RAM.
 *  \details Compare recalculated Crc against the Crc from NV RAM.
 *  \param[in] Self
 *  \return TRUE Crc match
 *  \return FALSE Crc mismatch
 *  \context TASK
 *  \reentrant FALSE
 *  \synchronous TRUE
 *  \pre -
 */
extern FUNC(boolean, NVM_PRIVATE_CODE) NvM_CrcJob_Compare(NvM_CrcJobConstPtrType Self);

/**********************************************************************************************************************
 *  NvM_CrcJob_CompareToExternal
 *********************************************************************************************************************/
/*!
 * \brief           Compare an external buffer with currently calculated CRC value
 * \details         -
 * \param[in]       Self      Pointer to CRC job that shall be used
 * \param[in]       ExtBuffer Pointer to buffer that shall be compared
 * \return          TRUE if buffer matches CRC value, FALSE otherwise
 * \pre             -
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 */
extern FUNC(boolean, NVM_PRIVATE_CODE) NvM_CrcJob_CompareToExternal(
    NvM_CrcJobConstPtrType Self,
    NvM_CrcBufferConstPtrType ExtBuffer);

/**********************************************************************************************************************
 * NvM_CrcJob_ExportBufferedValue
 *********************************************************************************************************************/
/*! \brief Copy Crc into the given buffer.
 *  \details Copy Crc into the given buffer.
 *  \param[in,out] Self
 *  \param[in,out] DestPtr
 *  \context TASK
 *  \reentrant FALSE
 *  \synchronous TRUE
 *  \config Crc internal buffer is enabled
 *  \pre -
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_CrcJob_ExportBufferedValue(
    NvM_CrcJobConstPtrType Self,
    NvM_CrcBufferPtrType DestPtr);

/**********************************************************************************************************************
 * NvM_CrcJob_ImportBufferedValue
 *********************************************************************************************************************/
/*! \brief Copy buffer to CRC buffer.
 *  \details Copy buffer to CRC buffer.
 *  \param[in,out] Self
 *  \param[in,out] SrcPtr
 *  \context TASK
 *  \reentrant FALSE
 *  \synchronous TRUE
 *  \config Crc internal buffer is enabled
 *  \pre -
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_CrcJob_ImportBufferedValue(
    NvM_CrcJobConstPtrType Self,
    NvM_CrcBufferConstPtrType SrcPtr);

# define NVM_STOP_SEC_CODE
# include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* NVM_CRC_H_ */

/**********************************************************************************************************************
 *  END OF FILE: NvM_Crc.h
 *********************************************************************************************************************/
