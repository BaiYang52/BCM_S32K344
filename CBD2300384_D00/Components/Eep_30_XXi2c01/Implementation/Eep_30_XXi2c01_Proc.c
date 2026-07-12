/*********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       \file     Eep_30_XXi2c01_Proc.c
 *       \brief    Eep I2c processing source file
 *
 *       \details  Contains state processing services and access to the underlying bus driver.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Eep_30_XXi2c01.h"
#include "Eep_30_XXi2c01_Proc.h"
#include "SchM_Eep_30_XXi2c01.h"

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
 *
 *
 *
 *
 */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#define EEP_30_XXI2C01_NUMBER_OF_INSTANCES       (0x02u)

/*! Addresses and Lengths */
#define EEP_30_XXI2C01_ADDRESS_DUMMY             (0u)
#define EEP_30_XXI2C01_LENGTH_DUMMY              (0u)
#define EEP_30_XXI2C01_LENGTH_COMMAND            (1u)

/*! Extended Addressing Defines */
#define EEP_30_XXI2C01_CONTROL_CODE              (0xAu)
#define EEP_30_XXI2C01_ADDRESS_MASK              (0x7u)
#define EEP_30_XXI2C01_SHIFT_ADDRESS             (0x3u)


/* critical section macros */
#if (EEP_30_XXI2C01_ASR3X_COMPATIBILITY == STD_OFF) /* COV_EEP_COMPATIBILITY */
/* ASR4 project/ environment */
# define Eep_30_XXi2c01_EnterCritical(Section)   SchM_Enter_Eep_30_XXi2c01_##Section() /* PRQA S 342 */ /* MD_Eep_30_XXI2C01_19.13 */
# define Eep_30_XXi2c01_ExitCritical(Section)    SchM_Exit_Eep_30_XXi2c01_##Section()  /* PRQA S 342 */ /* MD_Eep_30_XXI2C01_19.13 */
#else
/* ASR3 project/ environment */
# define Eep_30_XXi2c01_EnterCritical(Section)   SchM_Enter_Eep_30_XXi2c01(Section) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Eep_30_XXi2c01_ExitCritical(Section)    SchM_Exit_Eep_30_XXi2c01(Section)  /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#ifndef EEP_LOCAL /* COV_EEP_COMPATIBILITY */
# define EEP_LOCAL static
#endif

#if !defined (EEP_LOCAL_INLINE) /* COV_EEP_COMPATIBILITY */
# define EEP_LOCAL_INLINE LOCAL_INLINE
#endif


typedef P2VAR(I2c_DataType, AUTOMATIC, EEP_30_XXI2C_01_APPL_DATA) Eep_30_XXi2c01_I2cDataPtrType;
typedef P2CONST(I2c_DataType, AUTOMATIC, EEP_30_XXI2C_01_APPL_DATA) Eep_30_XXi2c01_I2cConstDataPtrType;

typedef P2VAR(Eep_30_XXi2c01_I2cDataPtrType, AUTOMATIC, EEP_30_XXI2C_01_APPL_DATA) Eep_30_XXi2c01_I2cDataPtrPtrType;
typedef P2VAR(Eep_30_XXi2c01_I2cConstDataPtrType, AUTOMATIC, EEP_30_XXI2C_01_APPL_DATA) Eep_30_XXi2c01_I2cConstDataPtrPtrType;

typedef enum
{
  EEP_30_XXI2C01_STATE_IDLE = 0,
  EEP_30_XXI2C01_STATE_EXECUTE,
  EEP_30_XXI2C01_STATE_VERIFY
} Eep_30_XXi2c01_StateType; /*!< Possible states of the state machine */

typedef enum
{
  EEP_30_XXI2C01_CMD_READ = 0,
  EEP_30_XXI2C01_CMD_WRITE,
  EEP_30_XXI2C01_CMD_ACK
} Eep_30_XXi2c01_CommandType; /*!< EEPROM instruction set */


typedef struct{
    MemIf_ModeType CurrentMode; /*!< Current mode of component. Either SLOW or FAST. Affects the number of bytes per I2C communication */
    Eep_30_XXi2c01_HandlingType CurrentHandling; /*!< Current handling of component. Either BURST or RECURRENT. Affects the number of main function calls before timeout */
    MemIf_StatusType Status;    /*!< Current global component status which is used by Eep_GetStatusStd service */
    MemIf_JobResultType JobResult; /*!< Current global component job result which is used by Eep_GetJobResultStd service */
    Eep_30_XXi2c01_StateType FsmState;    /*!< Current state machine state */
} Eep_30_XXi2c01_ComponentParamType;

typedef struct
{
    boolean ProcessMainFunction;    /*!< Indicates if MainFunction needs to be processed */
    boolean CancelRequested;    /*!< Indicates if Cancel was requested. Processed in MainFunction */
    boolean JobRequested;   /*!< Indicates if new job was requested. Processed in MainFunction */
} Eep_30_XXi2c01_ComponentFlagType;

/*! Stores context information of currently processed job */
typedef struct
{
   uint8 Index; /*!< Instance index of currently used EEPROM device */
   uint16 NrOfQueries; /*!< Global variable for remaining number of queries */
   Eep_30_XXi2c01_AddressType EepromAddress; /*!< Current address the EEPROM driver is working with */
   Eep_30_XXi2c01_LengthType RemainingBytes; /*!< Number of Bytes the EEPROM driver still needs to process */
   Eep_30_XXi2c01_LengthType ChunkSize; /*!< Size of data package the EEPROM driver can process at a time */
   Eep_30_XXi2c01_BufferPtrType DesDataBufferPtr; /*!< Reference to user buffer. Used for Read jobs only */
   Eep_30_XXi2c01_ConstBufferPtrType SrcDataBufferPtr; /*!< Reference to user buffer. Used for Write and Compare jobs only */
   Eep_30_XXi2c01_AddressType DataBufferIndex; /*!< Current index in user buffer */
   Eep_30_XXi2c01_JobType CurrentJob; /*!< Globally stored job being processed by the EEPROM driver */
   I2c_SeqResultType CurrentSequenceResult; /*!< Most recent sequence result */
   I2c_SequenceType CurrentSequence; /*!< Latest sequence transmitted to the underlying I2C driver */
} Eep_30_XXi2c01_JobContextType;

typedef enum
{
    EEP_NOT_OK = 0,
    EEP_NOT_FINISHED,
    EEP_INCONSISTENT,
    EEP_OK,
    EEP_BUSY
} Eep_30_XXi2c01_ProcessResultType; /*!< Possible return values of state processing functions */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define EEP_30_XXI2C01_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

EEP_LOCAL VAR(Eep_30_XXi2c01_JobContextType, EEP_30_XXI2C01_VAR_NO_INIT) Eep_30_XXi2c01_JobContext;

#define EEP_30_XXI2C01_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define EEP_30_XXI2C01_START_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

EEP_LOCAL VAR(Eep_30_XXi2c01_ComponentParamType, EEP_APPL_DATA) Eep_30_XXi2c01_ComponentParameter =
{
    MEMIF_MODE_SLOW,                /*!< CurrentMode */
    EEP_30_XXI2C01_HANDLING_BURST,  /*!< CurrentHandling */
    MEMIF_UNINIT,                   /*!< Status */
    MEMIF_JOB_FAILED,               /*!< JobResult */
    EEP_30_XXI2C01_STATE_IDLE,      /*!< FsmState */
};

EEP_LOCAL VAR(Eep_30_XXi2c01_ComponentFlagType, EEP_APPL_DATA) Eep_30_XXi2c01_ComponentFlags = /*!< Important flags to process asynchronous jobs correctly */
{
    FALSE, /*!< ProcessMainFunction */
    FALSE, /*!< CancelRequested */
    FALSE  /*!< JobRequested */
};

#define EEP_30_XXI2C01_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define EEP_30_XXI2C01_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
  Eep_30_XXi2c01_ReportDemError()
**********************************************************************************************************************/
/*!
 * \brief      Reports error to DEM according to current job
 * \details    Reports error to DEM according to current job. Service is called upon failed job end
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_ReportDemError(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_ResetFsm()
**********************************************************************************************************************/
/*!
 * \brief      Resets state machine to IDLE state.
 * \details    -
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC (void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_ResetFsm(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_ResetMainFunctionFlag()
**********************************************************************************************************************/
/*!
 * \brief      Resets ProcessMainFunction flag to false.
 * \details    -
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC (void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_ResetMainFunctionFlag(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_ResetCancelFlag()
**********************************************************************************************************************/
/*!
 * \brief      Resets CancelRequested flag to false.
 * \details    -
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC (void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_ResetCancelFlag(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_StartJob()
**********************************************************************************************************************/
/*!
 * \brief      Initializes component parameters for current job.
 * \details    Initializes current mode and current handling according to default values in configuration.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_StartJob(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_InitJobContext()
**********************************************************************************************************************/
/*!
 * \brief      Initializes low level implementation's job context.
 * \details    Initializes low level's job context which is used to perform current job. Service is called upon each job request.
 * \param[in]  Index
 * \param[out] DesDataBufferPtr - Pointer to user buffer where data will be stored
 * \param[in]  SrcDataBufferPtr - Pointer to user buffer which contains data to be written
 * \param[in]  Length
 * \param[in]  EepromAddress
 * \param[in]  JobType
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_InitJobContext(uint8 Index,
                                                              Eep_30_XXi2c01_BufferPtrType DesDataBufferPtr,
                                                              Eep_30_XXi2c01_ConstBufferPtrType SrcDataBufferPtr,
                                                              Eep_30_XXi2c01_LengthType Length,
                                                              Eep_30_XXi2c01_AddressType EepromAddress,
                                                              Eep_30_XXi2c01_JobType JobType);


/**********************************************************************************************************************
  Eep_30_XXi2c01_PerformExecuteTask()
**********************************************************************************************************************/
/*!
 * \brief      Performs execute task with regard to the requested job.
 * \details    -
 * \return     EEP_OK: State was performed successfully
 *             EEP_NOT_OK: State failed
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL FUNC(Eep_30_XXi2c01_ProcessResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_PerformExecuteTask(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_PerformVerifyTask()
**********************************************************************************************************************/
/*!
 * \brief      Performs tasks according to verification state.
 * \details    Performs tasks according to verification state. Read status register command is sent to Eeprom.
 * \return     EEP_OK: State was performed successfully
 *             EEP_NOT_OK: State failed
 * \pre        Verification depends on current job
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL FUNC(Eep_30_XXi2c01_ProcessResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_PerformVerifyTask(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_PerformFinalizeTask()
**********************************************************************************************************************/
/*!
 * \brief      Performs tasks according to finalize state.
 * \details    Performs tasks according to finalize state. Current job is finalized.
 * \return     EEP_OK: Entire job was successfully
 *             EEP_NOT_OK: Entire job failed
 *             EEP_NOT_FINISHED: Part of job was successful. Start over with next part.
 *             EEP_BUSY: EEPROM is still busy. Start over with verify state
 *             EEP_INCONSISTENT: Entire job failed due to inconsistent data within a compare job
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL FUNC(Eep_30_XXi2c01_ProcessResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_PerformFinalizeTask(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_FinalizeReadClassJob()
**********************************************************************************************************************/
/*!
 * \brief      Finalizes recent I2c communication for read-class jobs.
 * \details    Finalizes recent I2c communication for read-class jobs and initiates next part if job is not yet finished.
 * \return     EEP_OK: Entire job is finished successfully
 *             EEP_NOT_OK: Entire job failed
 *             EEP_INCONSISTENT: Entire job failed due to inconsistent data within a compare job
 *             EEP_BUSY: EEPROM is still busy, status has to be checked again
 *             EEP_NOT_FINISHED: Part of job is successful, start over with next part
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC(Eep_30_XXi2c01_ProcessResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_FinalizeReadClassJob(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_FinalizeWriteClassJob()
**********************************************************************************************************************/
/*!
 * \brief      Finalizes recent I2c communication for write-class jobs.
 * \details    Finalizes recent I2c communication for write-class jobs and initiates next part if job is not yet finished.
 * \return     EEP_OK: Entire job is finished successfully
 *             EEP_NOT_OK: Entire job failed
 *             EEP_BUSY: EEPROM is still busy, status has to be checked again
 *             EEP_NOT_FINISHED: Part of job is successful, start over with next part
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC(Eep_30_XXi2c01_ProcessResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_FinalizeWriteClassJob(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_Min()
**********************************************************************************************************************/
/*!
 * \brief      Compares and returns smaller value of both input parameters.
 * \details    -
 * \param[in]  Param1 - parameter which is compared to Param2
 * \param[in]  Param2 - parameter which is compared to Param1
 * \return     Smaller value of both input parameters
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC(Eep_30_XXi2c01_LengthType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Min(Eep_30_XXi2c01_LengthType Param1, Eep_30_XXi2c01_LengthType Param2);


/**********************************************************************************************************************
  Eep_30_XXi2c01_CheckEepromBusy()
**********************************************************************************************************************/
/*!
 * \brief      Evaluates recent sequence result and checks if EEPROM is still busy.
 * \details    -
 * \return     EEP_OK: Entire job is finished successfully
 *             EEP_NOT_OK: EEPROM timed out
 *             EEP_BUSY: EEPROM is still busy, status has to be checked again
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC(Eep_30_XXi2c01_ProcessResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_CheckEepromBusy(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_CompareBuffers()
**********************************************************************************************************************/
/*!
 * \brief      Compares recently read data with source buffer.
 * \details    -
 * \return     EEP_OK: Recently read data matches the source data
 *             EEP_INCONSISTENT: Recently read data is different to source data
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC(Eep_30_XXi2c01_ProcessResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_CompareBuffers(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_SetDataAndStartTransfer()
**********************************************************************************************************************/
/*!
 * \brief      Invokes I2c's SetupEB and AsyncTransmit services with parameters according to current job.
 * \details    -
 * \param[in]  Command
 * \param[in]  Address
 * \param[in]  SrcDataBufferPtr
 * \param[in]  DesDataBufferPtr
 * \param[in]  Length
 * \return     E_OK: I2c job was requested successfully
 *             E_NOT_OK: Setting up I2c job failed
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetDataAndStartTransfer(Eep_30_XXi2c01_CommandType Command,
                                                            Eep_30_XXi2c01_AddressType Address,
                                                            Eep_30_XXi2c01_I2cConstDataPtrType SrcDataBufferPtr,
                                                            Eep_30_XXi2c01_I2cDataPtrType DesDataBufferPtr,
                                                            I2c_DataLengthType Length);


/**********************************************************************************************************************
  Eep_30_XXi2c01_SwitchState()
**********************************************************************************************************************/
/*!
 * \brief      Handles state machine transitions based on result of last state.
 * \details    -
 * \param[in]  Result
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC (void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SwitchState(Eep_30_XXi2c01_ProcessResultType Result);


/**********************************************************************************************************************
  Eep_30_XXi2c01_IsWriteClassJob()
**********************************************************************************************************************/
/*!
 * \brief      Checks if current job is a write class job.
 * \details    -
 * \return     TRUE: Write Class job
 * \return     FALSE: Read Class job
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC(boolean, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_IsWriteClassJob(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_SetNrOfQueries()
**********************************************************************************************************************/
/*!
 * \brief      Sets number of queries for current job according to job type and current handling.
 * \details    -
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetNrOfQueries(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_SetChunkSizeAndCommand()
**********************************************************************************************************************/
/*!
 * \brief      Sets size of chunk for next I2C transmission. Returns EEPROM command for next I2C transmission.
 * \details    -
 * \return     Command
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC(Eep_30_XXi2c01_CommandType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetChunkSizeAndCommand(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_PrepareBuffer()
**********************************************************************************************************************/
/*!
 * \brief      Sets buffer according to current job.
 * \details    Sets buffer according to current job for upcoming I2c's SetupEB invocation.
 * \param[out]  SrcBufferPtrPtr
 * \param[out]  DesBufferPtrPtr
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_PrepareBuffer(Eep_30_XXi2c01_I2cConstDataPtrPtrType SrcBufferPtrPtr,
                                                                              Eep_30_XXi2c01_I2cDataPtrPtrType DesBufferPtrPtr);


/**********************************************************************************************************************
  Eep_30_XXi2c01_SetupChannel()
**********************************************************************************************************************/
/*!
 * \brief      Prepares I2C channel and external buffer and calls I2C's SetupEB function.
 * \details    -
 * \param[in]  Channel
 * \param[in]  Address - Only used for Extended Addressing option
 * \param[in]  TxDataBufferPtr - TransmitBuffer
 * \param[in]  RxDataBufferPtr - ReceiveBuffer
 * \param[in]  Length
 * \return     E_OK: I2c job was requested successfully
 *             E_NOT_OK: Setting up I2c job failed
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetupChannel(I2c_ChannelType Channel,
                                                                                       Eep_30_XXi2c01_AddressType Address,
                                                                                       I2c_DataConstPtrType TxDataBufferPtr, 
                                                                                       I2c_DataPtrType RxDataBufferPtr,
                                                                                       I2c_DataLengthType Length);


/**********************************************************************************************************************
  Eep_30_XXi2c01_FinishJob()
**********************************************************************************************************************/
/*!
 * \brief      Finishes current job with passed JobResult.
 * \details    Resets internal parameters and state machine. Invokes upper layer's callback notifications if configured.
 * \param[in]  JobResult
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace DSGN-Eep22874
 */
EEP_LOCAL_INLINE FUNC (void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_FinishJob(MemIf_JobResultType JobResult);


/**********************************************************************************************************************
  Eep_30_XXi2c01_ProcessFsm()
**********************************************************************************************************************/
/*!
 * \brief      Processes state machine.
 * \details    -
 * \pre        Will only be called if ProcessMainFunction flag is set
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EEP_LOCAL_INLINE FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_ProcessFsm(void);


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
  Eep_30_XXi2c01_ReportDemError()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EEP_LOCAL_INLINE FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_ReportDemError(void)
{
    Dem_EventIdType CurrentId;

    /* #10 Set CurrentId depending on current job */
    switch (Eep_30_XXi2c01_JobContext.CurrentJob)
    {
        case EEP_30_XXI2C01_JOB_COMPARE:
            CurrentId = Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->ECompareFailed;
            break;
        case EEP_30_XXI2C01_JOB_READ:
            CurrentId = Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->EReadFailed;
            break;
        case EEP_30_XXI2C01_JOB_WRITE:
            CurrentId = Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->EWriteFailed;
            break;
#if (EEP_30_XXI2C01_TEST_COM_API == STD_ON)
        case EEP_30_XXI2C01_JOB_TESTCOM:
            CurrentId = Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->ETestComFailed;
            break;
#endif
        default: /* case EEP_30_XXI2C01_JOB_ERASE: */
            CurrentId = Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->EEraseFailed;
            break;
    }

    /* #20 Report error with CurrentId to DEM */
    Dem_ReportErrorStatus(CurrentId, DEM_EVENT_STATUS_FAILED);
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_ResetFsm()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EEP_LOCAL_INLINE FUNC (void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_ResetFsm(void)
{
    /* #10 Reset state machine to IDLE state */
    Eep_30_XXi2c01_ComponentParameter.FsmState = EEP_30_XXI2C01_STATE_IDLE;
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_ResetMainFunctionFlag()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EEP_LOCAL_INLINE FUNC (void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_ResetMainFunctionFlag(void)
{
    /* #0 This service is always called after any processing function of the low level part is called */
    /* #10 Reset main function flag */
    Eep_30_XXi2c01_ComponentFlags.ProcessMainFunction = FALSE;
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_ResetCancelFlag()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EEP_LOCAL_INLINE FUNC (void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_ResetCancelFlag(void)
{
    /* #10 Reset cancel flag to false */
    Eep_30_XXi2c01_ComponentFlags.CancelRequested = FALSE;
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_StartJob()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
EEP_LOCAL_INLINE FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_StartJob(void)
{
    /* #10 Set component parameter */
    Eep_30_XXi2c01_ComponentParameter.Status = MEMIF_BUSY;
    Eep_30_XXi2c01_ComponentParameter.JobResult = MEMIF_JOB_PENDING;

    /* #20 Initialize state machine */
    Eep_30_XXi2c01_ResetFsm();

    /* #30 Set component flags */
    Eep_30_XXi2c01_ComponentFlags.JobRequested = TRUE;
    Eep_30_XXi2c01_ComponentFlags.ProcessMainFunction = TRUE;
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_InitJobContext()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EEP_LOCAL_INLINE FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_InitJobContext(uint8 Index,
                                                              Eep_30_XXi2c01_BufferPtrType DesDataBufferPtr,
                                                              Eep_30_XXi2c01_ConstBufferPtrType SrcDataBufferPtr,
                                                              Eep_30_XXi2c01_LengthType Length,
                                                              Eep_30_XXi2c01_AddressType EepromAddress,
                                                              Eep_30_XXi2c01_JobType JobType)
{
    /* #10 Initialize JobContext parameters with values passed by job request */
    Eep_30_XXi2c01_JobContext.Index = Index;
    Eep_30_XXi2c01_JobContext.DesDataBufferPtr = DesDataBufferPtr;
    Eep_30_XXi2c01_JobContext.SrcDataBufferPtr = SrcDataBufferPtr;
    Eep_30_XXi2c01_JobContext.RemainingBytes = Length;
    Eep_30_XXi2c01_JobContext.EepromAddress = EepromAddress;
    Eep_30_XXi2c01_JobContext.CurrentJob = JobType;

    /* #20 Reset JobContext values which are necessary to perform current job accordingly */
    Eep_30_XXi2c01_JobContext.DataBufferIndex = 0u;

} /* PRQA S 6060 */ /* MD_MSR_STPAR */


/**********************************************************************************************************************
  Eep_30_XXi2c01_PerformExecuteTask()
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
EEP_LOCAL FUNC(Eep_30_XXi2c01_ProcessResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_PerformExecuteTask(void)
{
    Eep_30_XXi2c01_ProcessResultType retVal = EEP_NOT_OK;
    Eep_30_XXi2c01_I2cConstDataPtrType SrcI2cDataBufferPtr = NULL_PTR;
    Eep_30_XXi2c01_I2cDataPtrType DesI2cDataBufferPtr = NULL_PTR;

    /* #10 Set chunk size and command */
    Eep_30_XXi2c01_CommandType Command = Eep_30_XXi2c01_SetChunkSizeAndCommand();

    /* #20 If verify state is active (=> Write class jobs) */
    if (Eep_30_XXi2c01_IsWriteClassJob() == TRUE)
    {
        /* #30 Set number of queries */
        Eep_30_XXi2c01_SetNrOfQueries();
    }

    /* #40 Prepare buffer */
    Eep_30_XXi2c01_PrepareBuffer(&SrcI2cDataBufferPtr, &DesI2cDataBufferPtr); /* SBSW_EEP_07 */

    /* #50 Set data and start transfer */
    if (Eep_30_XXi2c01_SetDataAndStartTransfer(Command, Eep_30_XXi2c01_JobContext.EepromAddress, SrcI2cDataBufferPtr, DesI2cDataBufferPtr, (I2c_DataLengthType) Eep_30_XXi2c01_JobContext.ChunkSize) == E_OK) /* SBSW_EEP_08 */
    {
        retVal = EEP_OK;
    }

    return retVal;
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_PerformVerifyTask()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EEP_LOCAL FUNC(Eep_30_XXi2c01_ProcessResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_PerformVerifyTask(void)
{
    Eep_30_XXi2c01_ProcessResultType retVal = EEP_NOT_OK;

    /* #10 Read Status Register to DataBuffer */
    if (Eep_30_XXi2c01_SetDataAndStartTransfer(EEP_30_XXI2C01_CMD_ACK, EEP_30_XXI2C01_ADDRESS_DUMMY, NULL_PTR, NULL_PTR, EEP_30_XXI2C01_LENGTH_COMMAND) == E_OK) /* SBSW_EEP_08 */
    {
        retVal = EEP_OK;
    }

    return retVal;
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_PerformFinalizeTask()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EEP_LOCAL FUNC(Eep_30_XXi2c01_ProcessResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_PerformFinalizeTask(void)
{
    Eep_30_XXi2c01_ProcessResultType retVal;

    /* #10 If it's a write-class job finalize the Write-Class job */
    if (Eep_30_XXi2c01_IsWriteClassJob() == TRUE)
    {
        retVal = Eep_30_XXi2c01_FinalizeWriteClassJob();
    }
    /* #20 If it's a read-class job finalize the Read-Class job */
    else
    {
        retVal = Eep_30_XXi2c01_FinalizeReadClassJob();
    }

    return retVal;
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_FinalizeReadClassJob()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
EEP_LOCAL_INLINE FUNC(Eep_30_XXi2c01_ProcessResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_FinalizeReadClassJob(void)
{
    Eep_30_XXi2c01_ProcessResultType retVal = EEP_OK;

    /* #10 Check recently read data in case of a compare job */
    if (Eep_30_XXi2c01_JobContext.CurrentJob == EEP_30_XXI2C01_JOB_COMPARE)
    {
        retVal = Eep_30_XXi2c01_CompareBuffers();
    }

    /* #20 Check if no compare error occurred */
    if (retVal == EEP_OK)
    {
        /* #23 Update job context */
        Eep_30_XXi2c01_JobContext.DataBufferIndex += Eep_30_XXi2c01_JobContext.ChunkSize;
        Eep_30_XXi2c01_JobContext.EepromAddress += Eep_30_XXi2c01_JobContext.ChunkSize;
        Eep_30_XXi2c01_JobContext.RemainingBytes -= Eep_30_XXi2c01_JobContext.ChunkSize;

        /* #25 Check if current job is not yet finished */
        if (Eep_30_XXi2c01_JobContext.RemainingBytes != 0u)
        {
            retVal = EEP_NOT_FINISHED;
        }
    }

    return retVal;
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_FinalizeWriteClassJob()
**********************************************************************************************************************/
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
 */
EEP_LOCAL_INLINE FUNC(Eep_30_XXi2c01_ProcessResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_FinalizeWriteClassJob(void)
{
    Eep_30_XXi2c01_ProcessResultType retVal;

    /* #10 Check if EEPROM is still busy */
    /* #11 return EEP_OK if EEPROM is idle */
    /* #12 return EEP_BUSY if EEPROM is busy */
    /* #13 return EEP_TIMEOUT if timeout occurred */
    retVal = Eep_30_XXi2c01_CheckEepromBusy();

    /* #20 Proceed only if EEPROM is idle */
    if (retVal == EEP_OK)
    {
        /* #30 Write was successful */

        /* #31 Update job context */
        Eep_30_XXi2c01_JobContext.DataBufferIndex += Eep_30_XXi2c01_JobContext.ChunkSize;
        Eep_30_XXi2c01_JobContext.EepromAddress += (Eep_30_XXi2c01_AddressType) Eep_30_XXi2c01_JobContext.ChunkSize;
        Eep_30_XXi2c01_JobContext.RemainingBytes -= (Eep_30_XXi2c01_LengthType) Eep_30_XXi2c01_JobContext.ChunkSize;

        /* #32 Check if current job is not yet finished */
        if (Eep_30_XXi2c01_JobContext.RemainingBytes != 0u)
        {
            retVal = EEP_NOT_FINISHED;
        }
    }

    return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */


/**********************************************************************************************************************
  Eep_30_XXi2c01_Min()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EEP_LOCAL_INLINE FUNC(Eep_30_XXi2c01_LengthType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Min(Eep_30_XXi2c01_LengthType Param1, Eep_30_XXi2c01_LengthType Param2)
{
    /* #10 Returns Param1 if smaller than Param2, otherwise Param2 */
    return (Param1 < Param2) ? Param1 : Param2;
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_CheckEepromBusy()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
EEP_LOCAL_INLINE FUNC(Eep_30_XXi2c01_ProcessResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_CheckEepromBusy(void)
{
    Eep_30_XXi2c01_ProcessResultType retVal = EEP_OK;

    /* #10 Sequence result is NACK if EEPROM is still busy */
    if (Eep_30_XXi2c01_JobContext.CurrentSequenceResult == I2C_SEQ_NACK)
    {
        Eep_30_XXi2c01_JobContext.NrOfQueries--;
        retVal = EEP_BUSY;

        /* #20 Timeout error occurs if number of queries has reached the value 0 */
        if (Eep_30_XXi2c01_JobContext.NrOfQueries == 0u)
        {
            retVal = EEP_NOT_OK;
#if (EEP_30_XXI2C01_DEV_ERROR_REPORT == STD_ON)
            /* #25 Report DET for all write class jobs (write and erase) */
            (void) Det_ReportError(EEP_30_XXI2C01_MODULE_ID, EEP_30_XXI2C01_INSTANCE_ID, EEP_30_XXI2C01_SID_MAIN_FUNCTION, EEP_30_XXI2C01_E_TIMEOUT);
#endif
        }
    }

    return retVal;
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_CompareBuffers()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EEP_LOCAL_INLINE FUNC(Eep_30_XXi2c01_ProcessResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_CompareBuffers(void)
{
    Eep_30_XXi2c01_ProcessResultType retVal = EEP_OK;
    Eep_30_XXi2c01_LengthType localCounter;

    for (localCounter = 0u; localCounter < Eep_30_XXi2c01_JobContext.ChunkSize; localCounter++)
    {
        /* #10 Compare read data with compare buffer */
        if (Eep_30_XXi2c01_JobContext.SrcDataBufferPtr[Eep_30_XXi2c01_JobContext.DataBufferIndex + localCounter] != (uint8)Eep_30_XXi2c01_DataBufferPtr[Eep_30_XXi2c01_JobContext.Index][localCounter])
        {
            retVal = EEP_INCONSISTENT;
            break;
        }
    }
    return retVal;
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_SetDataAndStartTransfer()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
EEP_LOCAL FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetDataAndStartTransfer(Eep_30_XXi2c01_CommandType Command,
                                                            Eep_30_XXi2c01_AddressType Address,
                                                            Eep_30_XXi2c01_I2cConstDataPtrType SrcDataBufferPtr,
                                                            Eep_30_XXi2c01_I2cDataPtrType DesDataBufferPtr,
                                                            I2c_DataLengthType Length)
{
    Std_ReturnType retVal;
    uint16 localCounter;

    I2c_ChannelType commandChannel;
    I2c_ChannelType dataChannel;
    I2c_SequenceType sequence;

    Eep_30_XXi2c01_AddressType jobAddress = Address;

    /* #10 Store address to first, second, etc. bytes (depending on addressing width) of CommandBuffer */
    for (localCounter = Eep_30_XXi2c01_ChipPtr[Eep_30_XXi2c01_JobContext.Index]->CommandBufferSize; localCounter > 0u; localCounter--)
    {
        Eep_30_XXi2c01_CommandBufferPtr[Eep_30_XXi2c01_JobContext.Index][localCounter - 1u] = (uint8)jobAddress; /* SBSW_EEP_09 */
        jobAddress >>= 8u;
    }

    /* #15 In case option ExtendedAddressing is enabled, Address parameter might still contain address information up here */

    /* #20 Set channels and sequence according to current EEP command */
    switch (Command)
    {
        case EEP_30_XXI2C01_CMD_READ:
            commandChannel = Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->RdCommandChannel;
            dataChannel = Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->RdDataChannel;
            sequence = Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->RdSequence;
            break;
        case EEP_30_XXI2C01_CMD_WRITE:
            commandChannel = Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->WrCommandChannel;
            dataChannel = Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->WrDataChannel;
            sequence = Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->WrSequence;
            break;
        case EEP_30_XXI2C01_CMD_ACK:
        default:
            commandChannel = Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->AckChannel;
            sequence = Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->AckSequence;
            break;
    }

    /* #30 Setup command channel */
    retVal = Eep_30_XXi2c01_SetupChannel(     /* SBSW_EEP_10 */
                                          commandChannel, /* PRQA S 3347 */ /* MD_EEP_30_XXI2C01_9.1 */
                                          jobAddress,
                                          Eep_30_XXi2c01_CommandBufferPtr[Eep_30_XXi2c01_JobContext.Index],
                                          NULL_PTR,
                                          Eep_30_XXi2c01_ChipPtr[Eep_30_XXi2c01_JobContext.Index]->CommandBufferSize); 

    /* #40 Setup data channel if command channel has been setup correctly and only if necessary (CMD_ACK does not require a data channel) */
    if ((retVal == E_OK) && (Command != EEP_30_XXI2C01_CMD_ACK))
    {
        retVal = Eep_30_XXi2c01_SetupChannel(  /* SBSW_EEP_11 */
                                              dataChannel, /* PRQA S 2962 */ /* MD_EEP_30_XXI2C01_9.1 */
                                              jobAddress,
                                              SrcDataBufferPtr,
                                              DesDataBufferPtr,
                                              Length); 
    }

    /* #50 Save and transmit current sequence if setup was successful */
    if (retVal == E_OK)
    {
        Eep_30_XXi2c01_JobContext.CurrentSequence = sequence; /* PRQA S 3353 */ /* MD_EEP_30_XXI2C01_9.1 */
        retVal = Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->I2cApiPtr->AsyncTransmit(sequence); /* SBSW_EEP_13 */
    }

    return retVal;
}

/**********************************************************************************************************************
  Eep_SwitchState()
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
EEP_LOCAL_INLINE FUNC (void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SwitchState(Eep_30_XXi2c01_ProcessResultType Result)
{
    switch (Result)
    {
        case EEP_NOT_OK:
            /* #10 Finish job due to negative result */
            Eep_30_XXi2c01_FinishJob(MEMIF_JOB_FAILED);
            break;
        case EEP_OK:
            /* #20 Finish job due to successful result */
            Eep_30_XXi2c01_FinishJob(MEMIF_JOB_OK);
            break;
        case EEP_BUSY:
            /* #30 Verification needs to be performed again */
            Eep_30_XXi2c01_ComponentParameter.FsmState = EEP_30_XXI2C01_STATE_VERIFY;

            if (Eep_30_XXi2c01_PerformVerifyTask() != EEP_OK)
            {
                Eep_30_XXi2c01_FinishJob(MEMIF_JOB_FAILED);
            }
            break;
        case EEP_NOT_FINISHED:
            /* #40 Job is not yet finished, thus start from the beginning */
            Eep_30_XXi2c01_ComponentParameter.FsmState = EEP_30_XXI2C01_STATE_EXECUTE;

            if (Eep_30_XXi2c01_PerformExecuteTask() != EEP_OK)
            {
                Eep_30_XXi2c01_FinishJob(MEMIF_JOB_FAILED);
            }
            break;
        case EEP_INCONSISTENT:
            /* #50 Inconsistency was detected upon comparing data areas */
            Eep_30_XXi2c01_FinishJob(MEMIF_BLOCK_INCONSISTENT);
            break;
        default:    /* PRQA S 2018 */ /* MD_MSR_Unreachable */ /* COV_EEP_DEFAULT */
            Eep_30_XXi2c01_FinishJob(MEMIF_JOB_FAILED);
            break;
    }
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_IsWriteClassJob()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EEP_LOCAL_INLINE FUNC(boolean, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_IsWriteClassJob(void)
{
    boolean retVal;

    /* #10 Return TRUE in case of write class job: Write, Erase */
    switch (Eep_30_XXi2c01_JobContext.CurrentJob)
    {
        case EEP_30_XXI2C01_JOB_WRITE:
        case EEP_30_XXI2C01_JOB_ERASE:
            retVal = TRUE;
            break;
        default:
            retVal = FALSE;
            break;
    }

    return retVal;
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_SetNrOfQueries()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EEP_LOCAL_INLINE FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetNrOfQueries(void)
{
    /* #10 NrOfQueries is only used in verify state. Thus it is only called while performing write-class jobs */
    /* #20 Set NrOfQueries to configured value for Write and Erase jobs */
    if (Eep_30_XXi2c01_ComponentParameter.CurrentHandling == EEP_30_XXI2C01_HANDLING_RECURRENT) /* COV_EEP_POSTBUILD_CONFIG */
    {
        Eep_30_XXi2c01_JobContext.NrOfQueries = Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->NbOfRecurrentQueries;
    }
    else /* COV_EEP_POSTBUILD_CONFIG */
    {
        Eep_30_XXi2c01_JobContext.NrOfQueries = Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->NbOfBurstQueries;
    }
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_SetChunkSizeAndCommand()
**********************************************************************************************************************/
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
 */
EEP_LOCAL_INLINE FUNC(Eep_30_XXi2c01_CommandType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetChunkSizeAndCommand(void)
{
    Eep_30_XXi2c01_CommandType Command;

    /* #10 Set ChunkSize and Command according to current job */
    if (Eep_30_XXi2c01_IsWriteClassJob() == TRUE)
    {
        /* #20 Write job, Erase job */

        /* #30 ChunkSize could be at maximum the amount of bytes until the upper page boundary is reached */
        Eep_30_XXi2c01_JobContext.ChunkSize = (Eep_30_XXi2c01_LengthType) ((Eep_30_XXi2c01_LengthType)Eep_30_XXi2c01_ChipPtr[Eep_30_XXi2c01_JobContext.Index]->PageSize -
                                                                          ((Eep_30_XXi2c01_LengthType)Eep_30_XXi2c01_JobContext.EepromAddress &
                                                                          ((Eep_30_XXi2c01_LengthType)Eep_30_XXi2c01_ChipPtr[Eep_30_XXi2c01_JobContext.Index]->PageSize - 
                                                                          (Eep_30_XXi2c01_LengthType)1u)));

        /* #40 ChunkSize = min(ChunkSize, RemainingBytes, WriteSize) */
        Eep_30_XXi2c01_JobContext.ChunkSize =  Eep_30_XXi2c01_Min(Eep_30_XXi2c01_Min(Eep_30_XXi2c01_JobContext.ChunkSize,
                                                                                     Eep_30_XXi2c01_JobContext.RemainingBytes),
                                                                  Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->WriteSize[Eep_30_XXi2c01_ComponentParameter.CurrentMode]);

        Command = EEP_30_XXI2C01_CMD_WRITE;
    }
    else
    {

#if (EEP_30_XXI2C01_TEST_COM_API == STD_ON)
        if (Eep_30_XXi2c01_JobContext.CurrentJob == EEP_30_XXI2C01_JOB_TESTCOM)
        {
            /* #50 TestCom job */
            Eep_30_XXi2c01_JobContext.ChunkSize = EEP_30_XXI2C01_LENGTH_DUMMY;
            Command = EEP_30_XXI2C01_CMD_ACK;
        }
        else
#endif
        {
            /* #60 Set ChunkSize and Command in case of Read class job */
            /* #70 ChunkSize = min(ReadSize, RemainingBytes) */
            Eep_30_XXi2c01_JobContext.ChunkSize = Eep_30_XXi2c01_Min(Eep_30_XXi2c01_JobContext.RemainingBytes,
                Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->ReadSize[Eep_30_XXi2c01_ComponentParameter.CurrentMode]);

            Command = EEP_30_XXI2C01_CMD_READ;
        }
    }
    return Command;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
  Eep_30_XXi2c01_PrepareBuffer()
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
EEP_LOCAL_INLINE FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_PrepareBuffer(Eep_30_XXi2c01_I2cConstDataPtrPtrType SrcBufferPtrPtr,
                                                                              Eep_30_XXi2c01_I2cDataPtrPtrType DesBufferPtrPtr)
{

    /* #10 Buffers need only to be prepared for write or read jobs, otherwise both buffer references are NULL_PTR */

    if (Eep_30_XXi2c01_JobContext.CurrentJob == EEP_30_XXI2C01_JOB_WRITE)
    {
        /* #20 Set buffer in case of a write job. Erase jobs do not provide any buffers to I2c */
        *SrcBufferPtrPtr = &Eep_30_XXi2c01_JobContext.SrcDataBufferPtr[Eep_30_XXi2c01_JobContext.DataBufferIndex]; /* SBSW_EEP_12 */
    }
    else if (Eep_30_XXi2c01_JobContext.CurrentJob == EEP_30_XXI2C01_JOB_READ)
    {
        /* #30 Set buffer according to read job */
        *DesBufferPtrPtr = &Eep_30_XXi2c01_JobContext.DesDataBufferPtr[Eep_30_XXi2c01_JobContext.DataBufferIndex]; /* SBSW_EEP_12 */
    }
    else if (Eep_30_XXi2c01_JobContext.CurrentJob == EEP_30_XXI2C01_JOB_COMPARE)
    {
        /* #40 Set buffer according to compare job */
        *DesBufferPtrPtr = Eep_30_XXi2c01_DataBufferPtr[Eep_30_XXi2c01_JobContext.Index]; /* SBSW_EEP_12 */
    }
    else
    {
        /* #50 Set both buffers to NULL_PTR in case of Erase or TestCom job */
        *DesBufferPtrPtr = NULL_PTR;  /* SBSW_EEP_12 */
        *SrcBufferPtrPtr = NULL_PTR;  /* SBSW_EEP_12 */
    }
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_SetupChannel()
**********************************************************************************************************************/
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
 */
EEP_LOCAL_INLINE FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetupChannel(I2c_ChannelType Channel,
                                                                                       Eep_30_XXi2c01_AddressType Address,
                                                                                       I2c_DataConstPtrType TxDataBufferPtr, 
                                                                                       I2c_DataPtrType RxDataBufferPtr,
                                                                                       I2c_DataLengthType Length)
{
    /* #10 Call of SetupEB function depends on addressing scheme of the device: */
    /* #11 If Extended Addressing is enabled, the highest bits of the memory address are passed via Address parameter. Service I2c_SetupEBDynamic is used therefore */
    /* #12 If Extended Addressing is disabled, Address parameter will be ignored and I2c_SetupEB will be used */

#if (EEP_30_XXI2C01_EXTENDED_ADDRESSING == STD_ON)
    /* #20 I2c_SetupEBDynamic API uses a device address which consists of two parts - one static part and one dynamic part: */
    /* #21 deviceAddress = || control code (4 bits) | address bits (3 bits) || */
    /* #22 The static part is the control code of the device, which is usually 1010b (0xA) */
    /* #23 The dynamic part consists of up to three address bits, which are contained in input parameter Address */
    I2c_AddressType deviceAddress = (I2c_AddressType) ((EEP_30_XXI2C01_CONTROL_CODE << EEP_30_XXI2C01_SHIFT_ADDRESS) | (Address & EEP_30_XXI2C01_ADDRESS_MASK));

    /* #30 Function name I2c_SetupEBDynamic is fix */
    return I2c_SetupEBDynamic(Channel, deviceAddress, TxDataBufferPtr, RxDataBufferPtr, Length); /* SBSW_EEP_11 */
#else
    EEP_30_XXI2C01_DUMMY_STATEMENT(Address); /* PRQA S 3112, 3200 */ /* MD_MSR_DummyStmt */
    return Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->I2cApiPtr->SetupEB(Channel, TxDataBufferPtr, RxDataBufferPtr, Length); /* SBSW_EEP_11 */ /* SBSW_EEP_13 */
#endif
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_FinishJob()
**********************************************************************************************************************/
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
 */
EEP_LOCAL_INLINE FUNC (void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_FinishJob(MemIf_JobResultType JobResult)
{
    /* #10 Reset state machine */
    Eep_30_XXi2c01_ResetFsm();

    /* #20 Reset ProcessMainFunction flag. MainFunction no longer needs to be processed */
    Eep_30_XXi2c01_ResetMainFunctionFlag();

    /* #30 Set component global parameters */
    Eep_30_XXi2c01_ComponentParameter.JobResult = JobResult;
    Eep_30_XXi2c01_ComponentParameter.Status = MEMIF_IDLE;

    /* #40 Depending on configuration, job finish is reported to upper layer via callback hooks */
    if (JobResult == MEMIF_JOB_OK)
    {
        /* #41 Invoke JobEndNotification in case of successful job */
        if (Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->JobEndNotification != NULL_PTR) /* COV_EEP_POSTBUILD_CONFIG */
        {
            Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->JobEndNotification(); /* SBSW_EEP_14 */
        }
    }
    else
    {
        /* #42 Invoke JobErrorNotification in case of unsuccessful job */
        if (Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->JobErrorNotification != NULL_PTR) /* COV_EEP_POSTBUILD_CONFIG */
        {
            Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->JobErrorNotification(); /* SBSW_EEP_14 */
        }

        /* #45 Report DEM error in case of unsuccessful job finish */
        Eep_30_XXi2c01_ReportDemError();
    }

}

/**********************************************************************************************************************
  Eep_30_XXi2c01_ProcessFsm()
**********************************************************************************************************************/
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
 *
 *
 *
 */
EEP_LOCAL_INLINE FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_ProcessFsm(void)
{
    /* #10 Reset MainFunction flag, so that this function can't be processed again */
    Eep_30_XXi2c01_ResetMainFunctionFlag();

    /* #20 Perform cancel if cancel flag is set */
    if (Eep_30_XXi2c01_ComponentFlags.CancelRequested == TRUE)
    {
        Eep_30_XXi2c01_ResetFsm();
        Eep_30_XXi2c01_ResetCancelFlag();
    }

    /* #30 State machine will only be processed if main function processing flag is true */
    switch (Eep_30_XXi2c01_ComponentParameter.FsmState)
    {
        /* --------------------------------------------------------------- */
        /* --------------------------STATE IDLE--------------------------- */
        /* --------------------------------------------------------------- */
        case EEP_30_XXI2C01_STATE_IDLE:
            /* #40 Check if new job has been requested */
            if (Eep_30_XXi2c01_ComponentFlags.JobRequested == TRUE)
            {
                /* #50 First State is always EEP_30_XXI2C01_STATE_EXECUTE */
                Eep_30_XXi2c01_ComponentParameter.FsmState = EEP_30_XXI2C01_STATE_EXECUTE;

                /* #55 Perform Execution */
                if (Eep_30_XXi2c01_PerformExecuteTask() != EEP_OK)
                {
                    Eep_30_XXi2c01_FinishJob(MEMIF_JOB_FAILED);
                }
                Eep_30_XXi2c01_ComponentFlags.JobRequested = FALSE;
            }
            break;
        /* --------------------------------------------------------------- */
        /* --------------------------STATE EXECUTE------------------------ */
        /* --------------------------------------------------------------- */
        case EEP_30_XXI2C01_STATE_EXECUTE:
            /* #70 Verification is necessary if currently a write job is being processed */
            if (Eep_30_XXi2c01_IsWriteClassJob() == TRUE)
            {
                /* #80 Next state is EEP_30_XXI2C01_STATE_VERIFY */
                Eep_30_XXi2c01_ComponentParameter.FsmState = EEP_30_XXI2C01_STATE_VERIFY;

                /* #85 Perform Verification */
                if (Eep_30_XXi2c01_PerformVerifyTask() != EEP_OK)
                {
                    Eep_30_XXi2c01_FinishJob(MEMIF_JOB_FAILED);
                }
            }
            else
            {
                /* #90 Finalize current job */
                Eep_30_XXi2c01_SwitchState(Eep_30_XXi2c01_PerformFinalizeTask());
            }

            break;
        /* --------------------------------------------------------------- */
        /* --------------------------STATE VERIFY------------------------- */
        /* --------------------------------------------------------------- */
        case EEP_30_XXI2C01_STATE_VERIFY:
            /* #100 Finalize current job */
            Eep_30_XXi2c01_SwitchState(Eep_30_XXi2c01_PerformFinalizeTask());
            break;
        /* --------------------------------------------------------------- */
        /* --------------------------DEFAULT------------------------------ */
        /* --------------------------------------------------------------- */
        default: /* PRQA S 2018 */ /* MD_MSR_Unreachable */ /* COV_EEP_DEFAULT */
            Eep_30_XXi2c01_FinishJob(MEMIF_JOB_FAILED);
            break;
    }
} /* PRQA S 6030, 6050 */ /* MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
  Eep_30_XXi2c01_InitCfgArray()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_InitCfgArray(uint8 Index, P2CONST(Eep_30_XXi2c01_ConfigType, AUTOMATIC, EEP_30_XXI2C01_PBCFG) ConfigPtr)
{
    /* #10 Initialize config array at given index */
    Eep_30_XXi2c01_CfgPtr[Index] = ConfigPtr; /* SBSW_EEP_15 */
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_InitComponentParameter()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_InitComponentParameter(void)
{
    /* #10 Initialize component parameter */
    Eep_30_XXi2c01_ComponentParameter.Status = MEMIF_IDLE;
    Eep_30_XXi2c01_ComponentParameter.JobResult = MEMIF_JOB_OK;

    /* #20 Initialize state machine */
    Eep_30_XXi2c01_ResetFsm();

    /* #30 Clear internal flags */
    Eep_30_XXi2c01_ComponentFlags.JobRequested = FALSE;
    Eep_30_XXi2c01_ComponentFlags.CancelRequested = FALSE;
    Eep_30_XXi2c01_ComponentFlags.ProcessMainFunction = FALSE;
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_InitAsyncJob()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_InitAsyncJob(Eep_30_XXi2c01_AddressType EepromAddress,
                                                       Eep_30_XXi2c01_BufferPtrType DesDataBufferPtr,
                                                       Eep_30_XXi2c01_ConstBufferPtrType SrcDataBufferPtr,
                                                       Eep_30_XXi2c01_LengthType Length,
                                                       uint8 Index,
                                                       Eep_30_XXi2c01_JobType JobType)
{
    /* #10 Enter exclusive area */
    Eep_30_XXi2c01_EnterCritical(EEP_30_XXI2C01_EXCLUSIVE_AREA_0);

    /* #20 Init job and component parameters */
    Eep_30_XXi2c01_InitJobContext(Index, DesDataBufferPtr, SrcDataBufferPtr, Length, EepromAddress, JobType); /* SBSW_EEP_06 */
    Eep_30_XXi2c01_StartJob();

    /* #30 Leave exclusive area */
    Eep_30_XXi2c01_ExitCritical(EEP_30_XXI2C01_EXCLUSIVE_AREA_0);
} /* PRQA S 6060 */ /* MD_MSR_STPAR */


/**********************************************************************************************************************
  Eep_30_XXi2c01_SetCurrentMode()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetCurrentMode(MemIf_ModeType Mode)
{
    /* #10 Set current mode to passed Mode */
    Eep_30_XXi2c01_ComponentParameter.CurrentMode = Mode;
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_SetCurrentHandling()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetCurrentHandling(Eep_30_XXi2c01_HandlingType Handling)
{
    /* #10 Set current handling to passed Handling */
    Eep_30_XXi2c01_ComponentParameter.CurrentHandling = Handling;
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_GetCurrentJobResult()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (MemIf_JobResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_GetCurrentJobResult(void)
{
    /* #10 Return component job result */
    return Eep_30_XXi2c01_ComponentParameter.JobResult;
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_GetCurrentStatus()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (MemIf_StatusType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_GetCurrentStatus(void)
{
    /* #10 Return component status */
    return Eep_30_XXi2c01_ComponentParameter.Status;
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_CancelJob()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC (void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_CancelJob(void)
{
    /* #10 If any job was requested but not yet processed, it's cancelled now */
    Eep_30_XXi2c01_ComponentFlags.JobRequested = FALSE;
    /* #20 Set Cancel flag, which is processed in next call of MainFunction */
    Eep_30_XXi2c01_ComponentFlags.CancelRequested = TRUE;

    /* #30 Set component global parameters accordingly */
    Eep_30_XXi2c01_ComponentParameter.Status = MEMIF_IDLE;
    Eep_30_XXi2c01_ComponentParameter.JobResult = MEMIF_JOB_CANCELED;

    /* #40 Call JobErrorNotification if configured */
    if (Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->JobErrorNotification != NULL_PTR) /* COV_EEP_POSTBUILD_CONFIG */
    {
        Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->JobErrorNotification(); /* SBSW_EEP_14 */
    }

}

/**********************************************************************************************************************
  Eep_30_XXi2c01_MainFunctionStd()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_MainFunctionStd(void)
{
    /* #10 Check if state machine needs to be processed */
    if (Eep_30_XXi2c01_ComponentFlags.ProcessMainFunction == TRUE)
    {
        /* #20 Process state machine */
        Eep_30_XXi2c01_ProcessFsm();
    }
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_ComEndStd()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_ComEndStd(void)
{

    /* #10 Save sequence result for state machine processing */
    Eep_30_XXi2c01_JobContext.CurrentSequenceResult = Eep_30_XXi2c01_CfgPtr[Eep_30_XXi2c01_JobContext.Index]->I2cApiPtr->GetSequenceResult(Eep_30_XXi2c01_JobContext.CurrentSequence); /* SBSW_EEP_13 */

    /* #20 Only sequence result OK and NACK are allowed, all other results lead to an error */
    if ((Eep_30_XXi2c01_JobContext.CurrentSequenceResult == I2C_SEQ_NACK) ||
        (Eep_30_XXi2c01_JobContext.CurrentSequenceResult == I2C_SEQ_OK))
    {
        /* #30 Set MainFunction process flag to true, so that job processing goes on */
        Eep_30_XXi2c01_ComponentFlags.ProcessMainFunction = TRUE;
    }
    else
    {
        Eep_30_XXi2c01_FinishJob(MEMIF_JOB_FAILED);
    }
}

#define EEP_30_XXI2C01_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Eep_30_XXi2c01_Proc.c
 *********************************************************************************************************************/
