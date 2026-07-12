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
/*!        \file  NvM_JobProc.h
 *        \brief  NvM job processing submodule header file
 *      \details  This submodule implements the FSM for the NVM, including the state description table.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if (!defined NVM_JOBPROC_H_)
#define NVM_JOBPROC_H_

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "NvM_Cfg.h"
#include "NvM_PrivateCfg.h"
#include "NvM_IntTypes.h"

#include "NvM_Act.h"
#include "NvM_Crc.h"
#include "NvM_DataIntegrity.h"
#include "NvM_Qry.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* The maximum number of conditional exits a stat can have, the final ("else") exit is implicit. */
#define NVM_MAX_NUM_OF_STATE_CONDITIONS (3u)

/* The maximum number of Query functions that can be ANDed together to form a state exit condition. */
#define NVM_MAX_NUM_OF_QRY_AND (2u)

/* Maximum number of bytes that is required to hold all CRC data.
 * As NvM only supports up to CRC32.
 *
 * Note: This is only a constant to avoid magic numbers. Changing this constant will not lead to any changes regarding
 *       CRC handling but increases buffer reservation for some(!) cases.
 */
#define NVM_MAX_NUM_BYTES_CRC (4u)

/* Determine internal buffer size for data integrity record handling */
#if (defined(NVM_MAC_BUFFER_LENGTH) && (NVM_MAC_BUFFER_LENGTH > NVM_MAX_NUM_BYTES_CRC))
#define NVM_MAX_NUM_BYTES_DATAINTEGRITYRECORD   (NVM_MAC_BUFFER_LENGTH)
#else
#define NVM_MAX_NUM_BYTES_DATAINTEGRITYRECORD   (NVM_MAX_NUM_BYTES_CRC)
#endif

/* Fixed number of retry attempts for state NVM_STATE_READALL_PREPARE_DATAINTEGRITY_VERIFICATION.
 * Currently no retry handling is implemented, but due to code changes done here this keeps the door open for easy
 * and quick extension of NvM to potentially allow retry handling in the future.
 */
#define NVM_READALL_MAX_NUM_EXPLICITSYNC_RETRY_ATTEMPTS (1u)

/*
 * Constants for NV block state handling
 *
 * Note: The state information contains primary, secondary and current block's state.
 *       The current state information is placed in the least significant position (i.e. shift=0).
 */

/* Bit mask representing NV block state for redundant blocks */
#define NVM_NVBLOCK_STATE_BIT_MASK (0x3u)

/* Shift factor to address primary redundant block NV state */
#define NVM_PRI_NVBLOCK_STATE_SHIFT (0x5u)

/* Shift factor to address secondary redundant block NV state */
#define NVM_SEC_NVBLOCK_STATE_SHIFT (0x3u)

/* Flag to set/check the primary redundant block active flag */
#define NVM_NVBLOCK_STATE_PRI_ACTIVE   (0x7Fu)

/* Flag to set/check the secondary redundant block active flag */
#define NVM_NVBLOCK_STATE_SEC_ACTIVE   (0x80u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/* Extracts primary NvBlock state from NvState. */
/* PRQA S 3453 1 */ /* MD_MSR_FctLikeMacro */
#define NVM_EXTRACT_NVSTATE_PRIMARY(NvState) (((NvState) >> NVM_PRI_NVBLOCK_STATE_SHIFT) & NVM_NVBLOCK_STATE_BIT_MASK)

/* Extracts secondary NvBlock state from NvState. */
/* PRQA S 3453 1 */ /* MD_MSR_FctLikeMacro */
#define NVM_EXTRACT_NVSTATE_SECONDARY(NvState) (((NvState) >> NVM_SEC_NVBLOCK_STATE_SHIFT) & NVM_NVBLOCK_STATE_BIT_MASK)


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef struct
{
    NvM_BlockIdType             JobBlockId_t; /*!< NvM block ID. */
    NvM_InternalServiceIdType   JobServiceId_t; /*!< Requested job. */
    NvM_RamAddressType          RamAddr_t; /*!< Data pointer. */
} NvM_JobType; /*!< Type of a job that is to be queued. */

typedef P2VAR(NvM_JobType, AUTOMATIC, NVM_PRIVATE_DATA) NvM_JobPtrType;

typedef struct
{
    NvM_BlockDescrPtrType       Descriptor_pt;              /*!< Pointer to the block descriptor. */
    NvM_RamMngmtPtrType         Mngmt_pt;                   /*!< Pointer to the RAM management. */
    NvM_RamAddressType          RamAddr_t;                  /*!< Pointer to data. */
    NvM_RamAddressType          NvRamAddr_t;                /*!< Pointer to be forwarded to the NV RAM. */
    NvM_DataIntegrityJob        DataIntegrityJob;           /*!< Pointer to DataIntegrity job */
    NvM_CrcJobType              BlockCrcJob_t;              /*!< Pointer to CRC job for RepairRedundantBlocks handling. */
    uint16                      NvIdentifier_u16;           /*!< Nv ID, which will be passed to underlying modules. */
    uint16                      ByteCount_u16;              /*!< Used for block-wise copy - also if internal buffer is OFF! */
    NvM_RequestResultType       LastResult_t;               /*!< Job result. */
    uint8                       WriteRetryCounter_u8;       /*!< Write retry counter. */
    uint8                       InternalFlags_u8;           /*!< Job flags. */
    uint8                       NvState_u8;                 /*!< Current state of the block. */
    boolean                     ExplSyncAttemptSuccessful;  /*!< Flag if recent explicit synchronization attempt was successful */
    uint8                       ExplSyncAttemptCounter;     /*!< Attempt counter of explicit synchronization */
#if (NVM_CSM_CIPHERING_ENABLED == STD_ON)
    uint8                       CsmJobReturnValue_u8;       /*!< Stores return value of a CSM job */
    boolean                     CsmJobAttemptSuccessful;    /*!< Flag if last CSM job attempt was successful */
    uint8                       CsmJobRetryCounter_u8;      /*!< CSM retry counter */
#endif /* (NVM_CSM_CIPHERING_ENABLED == STD_ON) */
} NvM_BlockInfoType; /*!< Job processing structure: contains all information related to the processing of one certain job. */

typedef P2VAR(NvM_BlockInfoType, AUTOMATIC, NVM_FAST_DATA) NvM_BlockInfoPtrType;

#if (NVM_REPAIR_REDUNDANT_BLOCKS_API == STD_ON)
typedef struct
{
    NvM_BlockIdType CurrentBlockId; /*!< Current block ID. */
    uint8 NvBlockState; /*!< Current job state. */
    uint32 CrcBuffer; /*!< CRC buffer to be able to compare two CRCs. */
} NvM_RepairBlockStateType; /*!< Structure used to store a RepairRedundantBlocks job. */
#endif

typedef enum
{
    /* Possible states of the task state machine */

    NVM_STATE_UNINIT = 0u,
    NVM_STATE_IDLE,
#if ((NVM_JOB_PRIORISATION == STD_ON) && (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1))
    NVM_STATE_HIGH_PRIO_JOB,
#endif
    NVM_STATE_NORMAL_PRIO_JOB,
#if (NVM_KILL_WRITEALL_API == STD_ON)
    NVM_STATE_MULTI_BLOCK_JOB,
#endif
    /* Possible states of the jobmain- and jobsub state machine */

    /* Read Block */
    NVM_STATE_READ_READ_DATA,
    NVM_STATE_READ_DATA_VALIDATION,
    NVM_STATE_READ_VERIFY_DATA_INTEGRITY,
    NVM_STATE_READ_PRE_CHECK_LOSS_OF_REDUNDANCY,
    NVM_STATE_READ_CHECK_LOSS_OF_REDUNDANCY,
    NVM_STATE_READ_IMPL_RECOV,
    NVM_STATE_READ_LOAD_ROM,
    NVM_STATE_READ_FINALIZE,

    /* Write Block */
    NVM_STATE_WRITE_COPY_BUFFER_DATA,
#if (NVM_CSM_CIPHERING_ENABLED == STD_ON)
    NVM_STATE_WRITE_ENCRYPT_DATA,
#endif
    NVM_STATE_WRITE_GENERATE_DATA_INTEGRITY_INFORMATION,
    NVM_STATE_WRITE_CHECK_CRCCOMPARE_SKIP,
    NVM_STATE_WRITE_READABILITY_CHECK_PRIMARY_BLOCK,
    NVM_STATE_WRITE_READABILITY_CHECK_SECONDARY_BLOCK,
    NVM_STATE_WRITE_NV_WRITE_REDUNDANT_DATA,
    NVM_STATE_WRITE_NV_WRITE_DATA,

    /* RestoreBlockDefaults */
    NVM_STATE_RESTORE_LOAD_ROM,

#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)
    /* Invalidate Nv Block */
    NVM_STATE_INVALIDATING_BLOCK,

    /* Erase Nv Block */
    NVM_STATE_ERASE_ERASE_BLOCK,
#endif

    /* Read All */
#if (NVM_DYNAMIC_CONFIGURATION == STD_ON)
    NVM_STATE_READALL_PROC_CONFIG_ID,
#endif /* (NVM_DYNAMIC_CONFIGURATION == STD_ON) */
    NVM_STATE_READALL_PROC_RAM_BLOCK,
#if(NVM_SET_RAM_BLOCK_STATUS_API == STD_ON)
    NVM_STATE_READALL_PREPARE_DATAINTEGRITY_VERIFICATION,
#endif
    NVM_STATE_READALL_CHK_SKIP,
    NVM_STATE_READALL_KILLED,
    NVM_STATE_READALL_WR_ONCE_PROT,
#if (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON)
    NVM_STATE_READALL_CHK_RAM_VALIDITY,
#endif
    NVM_STATE_READALL_READ_NV,
    NVM_STATE_READALL_LOAD_DEFAULTS,
    NVM_STATE_READALL_READABILITY_CHECK,

#if(NVM_PRE_READALL_API == STD_ON)
    /* Pre Read All */
    NVM_STATE_PREREADALL_PREP,
    NVM_STATE_PREREADALL_EXEC,
#endif

    /* Write All */
    NVM_STATE_WRITEALL_PROC_BLOCK,
    NVM_STATE_WRITEALL_WRITE_FSM,
    NVM_STATE_WRITEALL_WAIT_MEMHWA,

#if (NVM_REPAIR_REDUNDANT_BLOCKS_API == STD_ON)
    /* RepairRedundantBlocks */
    NVM_STATE_REPAIRREDUNDANT_MAIN,
    NVM_STATE_REPAIRREDUNDANT_CHECK_FIRST,
    NVM_STATE_REPAIRREDUNDANT_CHECK_SECOND,
    NVM_STATE_REPAIRREDUNDANT_VALIDATE_BLOCK,
    NVM_STATE_REPAIRREDUNDANT_READ_VALID_BLOCK,
    NVM_STATE_REPAIRREDUNDANT_WRITE_INVALID_BLOCK,
#endif
    NVM_STATE_FSM_FINISHED
} NvM_StateIdType; /*!< All available/ possible states of all NvM state machines (task, job main, and the job sub state machine). */

typedef struct
{
    NvM_StateActionIdType    InitialActionId;
    NvM_StateIdType        InitialState_t;
    NvM_ServiceIdType      PublicFid_t;
} NvM_IntServiceDescrType; /*!< Descriptor, describing an internal service (asynchronous  NVM job). */

typedef struct
{
    NvM_StateActionIdType ExitHandler_t; /*!< Action ID. */
    NvM_StateActionIdType EntryHandler_t; /*!< Action ID. */
} NvM_StateChangeActionsType; /*!< Holds two action Ids (part of a state). */

typedef P2CONST(NvM_StateChangeActionsType, AUTOMATIC, NVM_PRIVATE_CONST) NvM_StateChangeActionsPtrType;

typedef struct
{
    NvM_StateQueryIdType         Queries_at[NVM_MAX_NUM_OF_QRY_AND]; /*!< Queries, which decide whether the actions shall be executed. */
    NvM_StateChangeActionsType Actions_t; /*!< Action IDs to execute. */
    NvM_StateIdType              NextState_t; /*!< Next state to execute. */
} NvM_StateChangeIfDescrType; /*!< Describe one State exit with its condition, its Actions and its following state. */

typedef struct
{
    NvM_StateChangeActionsType Actions_t; /*!< Action IDs to execute. */
    NvM_StateIdType              NextState_t; /*!< Next state to execute. */
} NvM_StateChangeElseDescrType; /*!< Describe the final state exit ("else") to be taken if no condition matched. */

typedef struct
{
    NvM_StateChangeIfDescrType   ChangeConditions_at[NVM_MAX_NUM_OF_STATE_CONDITIONS]; /*!<All conditions with queries. */
    NvM_StateChangeElseDescrType FinalCondition_t; /*!< Fallback condition without queries. */
} NvM_StateDescrType; /*!< Describe a state with it's possible change conditions. */

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define NVM_START_SEC_VAR_FAST_NO_INIT_8
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Note: At least for development mode (where the following two state types are enums)
 * the compiler is expected to ensure correct alignment, regardless of any given section alignment.
 * The state variables are used quite frequently (but never via pointers)
 */

/*! Holds current main state machine status */
extern VAR(NvM_StateIdType, NVM_FAST_DATA) NvM_JobMainState_t;

/*! Holds current sub state machine status */
extern VAR(NvM_StateIdType, NVM_FAST_DATA) NvM_JobSubState_t;

/*! External definition of variables that area used in the NvM submodules - they are defined in NvM.c. */
extern VAR(uint8, NVM_FAST_DATA) NvM_ApiFlags_u8;

#define NVM_STOP_SEC_VAR_FAST_NO_INIT_8
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define NVM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! State of the task state machine. */
extern VAR(NvM_StateIdType, NVM_PRIVATE_DATA) NvM_TaskState_t;

#define NVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define NVM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Currently processed NvM job type */
extern VAR(NvM_JobType, NVM_PRIVATE_DATA) NvM_CurrentJob_t;

/*! Information to currently processed block */
extern VAR(NvM_BlockInfoType, NVM_PRIVATE_DATA) NvM_CurrentBlockInfo_t;

#if (NVM_REPAIR_REDUNDANT_BLOCKS_API == STD_ON)
/*! Information required for NvM_RepairRedundant block handling */
extern VAR(NvM_RepairBlockStateType, NVM_PRIVATE_DATA) NvM_RepairRedBlockState;
#endif

/*! Array of RAM Block Management areas, defined to be usable in block descriptor table */
extern VAR(NvM_RamMngmtAreaType, NVM_CONFIG_DATA) NvM_BlockMngmtArea_at[NVM_TOTAL_NUM_OF_NVRAM_BLOCKS];

/*! Management area for DCM blocks */
extern VAR(NvM_RamMngmtAreaType, NVM_CONFIG_DATA) NvM_DcmBlockMngmt_t;

#define NVM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Data section containing a NvM internal buffer. If job forwarding from application to memory core is enabled the buffer must be placed in the shared memory. */
#define NVM_START_SEC_VAR_INTERNAL_BUFFER
#include "NvM_MemMap.h"

/*! Holds data integrity record of a redundant block only. */
extern VAR(uint8, NVM_PRIVATE_DATA) NvM_SecondaryRedundantBlockDataIntegrityRecord[NVM_MAX_NUM_BYTES_DATAINTEGRITYRECORD];

#define NVM_STOP_SEC_VAR_INTERNAL_BUFFER
#include "NvM_MemMap.h"

#define NVM_START_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Internal job descriptor table. */
extern CONST(NvM_IntServiceDescrType, NVM_PRIVATE_CONST) NvM_IntServiceDescrTable_at[(uint32)NVM_INT_FID_NO_JOB_PENDING + 1u];

#define NVM_STOP_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * NvM_JobProcInit
 *********************************************************************************************************************/
/*! \brief Initialize the module variables
 *  \details Initialize the module variables
 *  \context TASK
 *  \reentrant FALSE
 *  \synchronous TRUE
 *  \pre -
 */
extern FUNC(void, NVM_PRIVATE_CODE) NvM_JobProcInit(void);

/**********************************************************************************************************************
 * NvM_Fsm
 *********************************************************************************************************************/
/*! \brief Sets next state machine state to process depending on query results.
 *  \details This function executes exactly one State of a state machine: e.g. the initial state of the State Machine
 *           for ReadBlock. This initial state contains four exits. Which exit is taken depends on the queries
 *           which in turn lead to the actions. At last the next state is stored. The next time the function is called,
 *           it executes this next state. This function is called from NvM_Mainfunction() which is called cyclically
 *           by an Application (or event-driven when NVM is an ECC Task).
 *  \param[in] NvM_CurrentState_t as an index to the state table array. Must be in range
 *             [NVM_STATE_UNINIT, NVM_STATE_FSM_FINISHED]. Caller has to ensure validity.
 *  \return next state
 *  \context TASK
 *  \reentrant FALSE
 *  \synchronous TRUE
 *  \pre -
 */
extern FUNC(NvM_StateIdType, NVM_PRIVATE_CODE) NvM_Fsm(NvM_StateIdType NvM_CurrentState_t);

/**********************************************************************************************************************
 *  NvM_IsBooleanBitfieldTrue
 *********************************************************************************************************************/
/*!
 * \brief           Translates a given "boolean bitfield" into a native boolean type.
 * \details         Bit fields with length 1 are intrinsic booleans.
 *                  The MISRA QAC tool does not accept this and shows warnings. To solve the problem, this inline
 *                  function is used to check boolean expressions for such types.
 *
 *                  Note: This function is only allowed to be used for NvM_BitfieldType with length 1!
 * \param[in]       BooleanBitfield A bitfield with length "1" that is used for boolean expression.
 * \return          TRUE if bitfield is not equal to zero, FALSE otherwise.
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 */
extern FUNC(boolean, NVM_PRIVATE_CODE) NvM_IsBooleanBitfieldTrue(const NvM_BitFieldType BooleanBitfield);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* NVM_JOBPROC_H_ */

/**********************************************************************************************************************
 *  END OF FILE: NvM_JobProc.h
 *********************************************************************************************************************/
