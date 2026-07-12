/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : C40_IP IPV_QSPI
*   Dependencies         : none
*
*   Autosar Version      : 4.4.0
*   Autosar Revision     : ASR_REL_4_4_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 2.0.1
*   Build Version        : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
*   (c) Copyright 2020 - 2022 NXP Semiconductors
*   All Rights Reserved.
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/
/*==================================================================================================
==================================================================================================*/

/**
*   @file Fls_IPW.c
*
*   @addtogroup FLS
*   @{
*/

/* implements Fls_IPW.c_Artifact */

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Fls.h"
#include "Fls_IPW.h"
#include "Det.h"
#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
#include "C40_Ip.h"
#include "C40_Ip_Ac.h"
#endif /* (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED) */
#if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
#include "Qspi_Ip.h"
#endif /* (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */


/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLS_IPW_VENDOR_ID_C                           43
#define FLS_IPW_AR_RELEASE_MAJOR_VERSION_C            4
#define FLS_IPW_AR_RELEASE_MINOR_VERSION_C            4
#define FLS_IPW_AR_RELEASE_REVISION_VERSION_C         0
#define FLS_IPW_SW_MAJOR_VERSION_C                    2
#define FLS_IPW_SW_MINOR_VERSION_C                    0
#define FLS_IPW_SW_PATCH_VERSION_C                    1


/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and Fls header file are of the same vendor */
#if (FLS_IPW_VENDOR_ID_C != FLS_VENDOR_ID)
    #error "Fls_IPW.c and Fls.h have different vendor ids"
#endif
/* Check if current file and Fls header file are of the same Autosar version */
#if ((FLS_IPW_AR_RELEASE_MAJOR_VERSION_C    != FLS_AR_RELEASE_MAJOR_VERSION) || \
     (FLS_IPW_AR_RELEASE_MINOR_VERSION_C    != FLS_AR_RELEASE_MINOR_VERSION) || \
     (FLS_IPW_AR_RELEASE_REVISION_VERSION_C != FLS_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Fls_IPW.c and Fls.h are different"
#endif
/* Check if current file and Fls header file are of the same Software version */
#if ((FLS_IPW_SW_MAJOR_VERSION_C != FLS_SW_MAJOR_VERSION) || \
     (FLS_IPW_SW_MINOR_VERSION_C != FLS_SW_MINOR_VERSION) || \
     (FLS_IPW_SW_PATCH_VERSION_C != FLS_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Fls_IPW.c and Fls.h are different"
#endif

/* Check if current file and Fls_IPW.h header file have the same Vendor ID */
#if (FLS_IPW_VENDOR_ID_C != FLS_IPW_VENDOR_ID_H)
    #error "Fls_IPW.c and Fls_IPW.h have different vendor ids"
#endif
/* Check if current file and Fls_IPW.h header file are of the same Autosar version */
#if ((FLS_IPW_AR_RELEASE_MAJOR_VERSION_C    != FLS_IPW_AR_RELEASE_MAJOR_VERSION_H) || \
     (FLS_IPW_AR_RELEASE_MINOR_VERSION_C    != FLS_IPW_AR_RELEASE_MINOR_VERSION_H) || \
     (FLS_IPW_AR_RELEASE_REVISION_VERSION_C != FLS_IPW_AR_RELEASE_REVISION_VERSION_H) \
    )
    #error "AutoSar Version Numbers of Fls_IPW.c and Fls_IPW.h are different"
#endif
/* Check if current file and Fls_IPW.h header file are of the same Software version */
#if ((FLS_IPW_SW_MAJOR_VERSION_C != FLS_IPW_SW_MAJOR_VERSION_H) || \
     (FLS_IPW_SW_MINOR_VERSION_C != FLS_IPW_SW_MINOR_VERSION_H) || \
     (FLS_IPW_SW_PATCH_VERSION_C != FLS_IPW_SW_PATCH_VERSION_H) \
    )
    #error "Software Version Numbers of Fls_IPW.c and Fls_IPW.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and Det header file are of the same Autosar version */
    #if ((FLS_IPW_AR_RELEASE_MAJOR_VERSION_C != DET_AR_RELEASE_MAJOR_VERSION) || \
         (FLS_IPW_AR_RELEASE_MINOR_VERSION_C != DET_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AutoSar Version Numbers of Fls_IPW.c and Det.h are different"
    #endif
#endif

#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
    /* Check if current file and C40_IP.h header file have the same Vendor ID */
    #if (FLS_IPW_VENDOR_ID_C != C40_IP_VENDOR_ID_H)
        #error "Fls_IPW.c and C40_IP.h have different vendor ids"
    #endif
    /* Check if current file and C40_IP.h header file are of the same Autosar version */
    #if ((FLS_IPW_AR_RELEASE_MAJOR_VERSION_C    != C40_IP_AR_RELEASE_MAJOR_VERSION_H) || \
         (FLS_IPW_AR_RELEASE_MINOR_VERSION_C    != C40_IP_AR_RELEASE_MINOR_VERSION_H) || \
         (FLS_IPW_AR_RELEASE_REVISION_VERSION_C != C40_IP_AR_RELEASE_REVISION_VERSION_H) \
        )
        #error "AutoSar Version Numbers of Fls_IPW.c and C40_IP.h are different"
    #endif
    /* Check if current file and C40_IP.h header file are of the same Software version */
    #if ((FLS_IPW_SW_MAJOR_VERSION_C != C40_IP_SW_MAJOR_VERSION_H) || \
         (FLS_IPW_SW_MINOR_VERSION_C != C40_IP_SW_MINOR_VERSION_H) || \
         (FLS_IPW_SW_PATCH_VERSION_C != C40_IP_SW_PATCH_VERSION_H) \
        )
        #error "Software Version Numbers of Fls_IPW.c and C40_IP.h are different"
    #endif

    /* Check if current file and C40_Ip_Ac.h header file have the same Vendor ID */
    #if (FLS_IPW_VENDOR_ID_C != FLS_AC_VENDOR_ID_H)
        #error "Fls_IPW.c and C40_Ip_Ac.h have different vendor ids"
    #endif
    /* Check if current file and C40_Ip_Ac.h  header file are of the same Autosar version */
    #if ((FLS_IPW_AR_RELEASE_MAJOR_VERSION_C    != FLS_AC_AR_RELEASE_MAJOR_VERSION_H) || \
         (FLS_IPW_AR_RELEASE_MINOR_VERSION_C    != FLS_AC_AR_RELEASE_MINOR_VERSION_H) || \
         (FLS_IPW_AR_RELEASE_REVISION_VERSION_C != FLS_AC_AR_RELEASE_REVISION_VERSION_H) \
        )
        #error "AutoSar Version Numbers of Fls_IPW.c and C40_Ip_Ac.h are different"
    #endif
    /* Check if current file and C40_Ip_Ac.h  header file are of the same Software version */
    #if ((FLS_IPW_SW_MAJOR_VERSION_C != FLS_AC_SW_MAJOR_VERSION_H) || \
         (FLS_IPW_SW_MINOR_VERSION_C != FLS_AC_SW_MINOR_VERSION_H) || \
         (FLS_IPW_SW_PATCH_VERSION_C != FLS_AC_SW_PATCH_VERSION_H) \
        )
        #error "Software Version Numbers of Fls_IPW.c and C40_Ip_Ac.h are different"
    #endif
#endif /* (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED) */

#if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
    /* Check if current file and Fls_Qspi header file are of the same vendor */
    #if (FLS_IPW_VENDOR_ID_C != QSPI_IP_VENDOR_ID_H)
        #error "Fls_IPW.c and Fls_Qspi.h have different vendor ids"
    #endif
    #ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
        /* Check if current file and Fls_Qspi header file are of the same Autosar version */
        #if ((FLS_IPW_AR_RELEASE_MAJOR_VERSION_C    != QSPI_IP_AR_RELEASE_MAJOR_VERSION_H) || \
             (FLS_IPW_AR_RELEASE_MINOR_VERSION_C    != QSPI_IP_AR_RELEASE_MINOR_VERSION_H) || \
             (FLS_IPW_AR_RELEASE_REVISION_VERSION_C != QSPI_IP_AR_RELEASE_REVISION_VERSION_H) \
            )
            #error "AutoSar Version Numbers of Fls_IPW.c and Fls_Qspi.h are different"
        #endif
    #endif
    /* Check if current file and Fls_Qspi header file are of the same Software version */
    #if ((FLS_IPW_SW_MAJOR_VERSION_C != QSPI_IP_SW_MAJOR_VERSION_H) || \
         (FLS_IPW_SW_MINOR_VERSION_C != QSPI_IP_SW_MINOR_VERSION_H) || \
         (FLS_IPW_SW_PATCH_VERSION_C != QSPI_IP_SW_PATCH_VERSION_H) \
        )
        #error "Software Version Numbers of Fls_IPW.c and Fls_Qspi.h are different"
    #endif
#endif /* (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */


/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
static Fls_LLDReturnType Fls_IPW_TranslateReturnCode(C40_Ip_StatusType ReturnCode);
static void Fls_IPW_LLDMainFunctionC40Jobs(void);
static inline void Fls_IPW_LLDMainFunctionC40Jobs_CheckEraseStatus(C40_Ip_StatusType Status);
static inline void Fls_IPW_LLDMainFunctionC40Jobs_CheckWriteStatus(C40_Ip_StatusType Status);
static inline Fls_LLDReturnType Fls_IPW_SectorEraseC40Jobs(boolean Asynch,
                                                           Fls_AddressType SectorOffset
                                                          );
static inline Fls_LLDReturnType Fls_IPW_SectorWriteC40Jobs(const Fls_AddressType SectorOffset,
                                                           const Fls_AddressType Length,
                                                           const uint8 *JobDataSrcPtr,
                                                           const boolean Asynch
                                                          );
static inline void Fls_IPW_CallAccessCodeWrite(void);
static inline void Fls_IPW_CallAccessCodeErase(void);

#if (STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS)
static C40_Ip_StatusType Fls_IPW_CopyBlock(uint32 LogicalAddress,
                                           uint32 Length,
                                           uint8 *DestAddressPtr,
                                           const uint8 *SourceAddressPtr
                                          );
#endif /* STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS */
#endif /* (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED) */


#if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
static Fls_AddressType Fls_IPW_ComputeWriteChunkSize(Fls_AddressType JobSize,
                                                     Fls_AddressType BaseAddr,
                                                     Fls_AddressType PageSize,
                                                     uint32 MemAlignment
                                                    );
static Fls_LLDReturnType Fls_IPW_SectorWriteChunk(const boolean Asynch);
static void Fls_IPW_LLDMainFunctionQspiErase(uint32 FlashInstance);
static void Fls_IPW_LLDMainFunctionQspiWrite(uint32 FlashInstance);
static void Fls_IPW_LLDMainFunctionQspiJobs(void);
static inline Fls_LLDReturnType Fls_IPW_SectorEraseQspiJobs(boolean Asynch,
                                                            Fls_AddressType SectorOffset,
                                                            Fls_LengthType PhysicalSectorSize
                                                           );
static inline Fls_LLDReturnType Fls_IPW_SectorWriteQspiJobs(const Fls_AddressType SectorOffset,
                                                            const Fls_AddressType Length,
                                                            const uint8 *JobDataSrcPtr,
                                                            const boolean Asynch
                                                           );
static Qspi_Ip_StatusType Fls_IPW_InitControllers(void);
static Qspi_Ip_StatusType Fls_IPW_InitMemories(void);
static boolean Fls_IPW_CheckDevicesId(void);
#endif /* (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */


/*==================================================================================================
                                 ENUM TYPEDEFS
==================================================================================================*/
#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
#if (STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS)
/**
* @brief   Elements of read callout function
*/
typedef struct
{
    uint32             ReadStartAddress;    /**< @brief The start address of the memory area to be read/compare  */
    uint32             Length;              /**< @brief Number of bytes to read/compare                          */
    uint8              *DataDest;           /**< @brief Pointer to target data buffer used to read to            */
    const uint8        *DataSource;         /**< @brief Pointer to source data buffer used to compare            */
    C40_Ip_StatusType  Status;              /**< @brief Status of the copy operation                             */
} Fls_CopyDescrType;
#endif /* STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS */
#endif /* (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED) */


/*==================================================================================================
                                       GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
 *                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

#if (defined(FLS_DEVICES_USING_SFDP))
#if (FLS_DEVICES_USING_SFDP > 0)

#define FLS_QSPI_SFDP_LUT_SIZE               130U
#define FLS_QSPI_SFDP_INIT_OP_SIZE           (4U * 2U)    /* Double size for insert wait operations */

typedef struct
{
    Qspi_Ip_InstrOpType lutOps[FLS_QSPI_SFDP_LUT_SIZE];               /*!< Array for virtual LUT instructions       */
    Qspi_Ip_InitOperationType initOps[FLS_QSPI_SFDP_INIT_OP_SIZE];    /*!< Array for initial operations             */
    Qspi_Ip_MemoryConfigType sfdpCfg;                                 /*!< Flash device configuration structure     */
} Fls_Qspi_SfdpConfigType;
#endif
#endif

/*==================================================================================================
                                       LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
                                       LOCAL VARIABLES
==================================================================================================*/
#if (FLS_QSPI_SECTORS_CONFIGURED == STD_ON)

#define FLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fls_MemMap.h"

/* The external job start address */
static Fls_AddressType Fls_Ipw_xExtJobStartAddr;
/* The external job size */
static Fls_AddressType Fls_Ipw_xExtJobSize;
/* The external job current chunk size, for jobs which need to be split in chunks */
static Fls_AddressType Fls_Ipw_xExtJobChunkSize;
/* Source data buffer for the current write job. */
static const uint8 * Fls_Ipw_pExtJobBuffAddr;

#if (defined(FLS_DEVICES_USING_SFDP))
#if (FLS_DEVICES_USING_SFDP > 0)
/* Array of configurations for flash devices configured to use SFDP  */
static Fls_Qspi_SfdpConfigType Fls_Ipw_axSfdpConfigs[FLS_DEVICES_USING_SFDP];
/* Configurations indexes for flash devices configured to use SFDP  */
static uint8 Fls_Ipw_au8SfdpConfigsIndex[QSPI_IP_MEM_INSTANCE_COUNT];
#endif
#endif

#define FLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fls_MemMap.h"

#define FLS_START_SEC_VAR_CLEARED_32
#include "Fls_MemMap.h"

/* Counters for timeout detection */
static uint32 Fls_Ipw_u32QspiElapsedTicks;
static uint32 Fls_Ipw_u32QspiTimeoutTicks;
static uint32 Fls_Ipw_u32QspiCurrentTicks;

#define FLS_STOP_SEC_VAR_CLEARED_32
#include "Fls_MemMap.h"

#endif /* (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */


#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)

#define FLS_START_SEC_VAR_INIT_UNSPECIFIED
#include "Fls_MemMap.h"
static C40_Ip_VirtualSectorsType Fls_Ipw_xVirtualSectorInUse = C40_SECTOR_ERROR;
#define FLS_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Fls_MemMap.h"

#if (STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS)
#define FLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fls_MemMap.h"
/* The copy operation when read */
static Fls_CopyDescrType Fls_Ipw_xCopyDescr;
#define FLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fls_MemMap.h"
#endif /* STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS */

#endif /* (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED) */


/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/
#define FLS_START_SEC_CODE
#include "Fls_MemMap.h"
#if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
/*
    Computes the size of the next chunk of a write job.
*/
static Fls_AddressType Fls_IPW_ComputeWriteChunkSize(Fls_AddressType JobSize,
                                                     Fls_AddressType BaseAddr,
                                                     Fls_AddressType PageSize,
                                                     uint32 MemAlignment
                                                    )
{
    Fls_AddressType ChunkSize;

    /* Reserve size in case writing from odd starting address */
    uint32 StartAddrOffset  = BaseAddr & (MemAlignment - 1U);
    uint32 QspiMaxWriteSize = QSPI_IP_MAX_WRITE_SIZE - StartAddrOffset;

    /* Check external device restrictions: ChunkSize can not exceed the device page size */
    /* If address is not aligned, max. ChunkSize is the amount left in the current page */
    ChunkSize = PageSize - (BaseAddr % PageSize);
    /* Check if chunk does not exceed IP driver capabilities */
    if (ChunkSize > QspiMaxWriteSize)
    {
        ChunkSize = QspiMaxWriteSize;
    }
    /* Check if entire job fits in current page */
    if (ChunkSize > JobSize)
    {
        ChunkSize = JobSize;
    }
    return ChunkSize;
}

/**
* @brief          Writes a single data chunk.
* @details        This function initiates a write for a data chunk - amount of data than can be written in a single operation.
*/
static Fls_LLDReturnType Fls_IPW_SectorWriteChunk(const boolean Asynch)
{
    Fls_LLDReturnType LldRetVal = FLASH_E_FAILED;
    uint32 FlashInstance;
    Qspi_Ip_StatusType Status = STATUS_QSPI_IP_SUCCESS;

    /* Get external flash instance */
    FlashInstance = (*(Fls_pConfigPtr->pFlsQspiCfgConfig->u8SectFlashUnit))[Fls_u32JobSectorIt];

#if (FLS_ERASE_VERIFICATION_ENABLED == STD_ON)
    if (STATUS_QSPI_IP_SUCCESS != Qspi_Ip_EraseVerify(FlashInstance, Fls_Ipw_xExtJobStartAddr, Fls_Ipw_xExtJobChunkSize))
    {
        (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_VERIFY_ERASE_FAILED);
        LldRetVal = FLASH_E_FAILED;
    }
    else
    {
#endif /* (FLS_ERASE_VERIFICATION_ENABLED == STD_ON) */
        /* Call IP write routine. */
        if (STATUS_QSPI_IP_SUCCESS != Qspi_Ip_Program(FlashInstance, Fls_Ipw_xExtJobStartAddr, Fls_Ipw_pExtJobBuffAddr, Fls_Ipw_xExtJobChunkSize))
        {
            LldRetVal = FLASH_E_FAILED;
        }
        else
        {
            if ((boolean)FALSE == Asynch) /*SYNC Mode*/
            {
                LldRetVal = FLASH_E_OK;
                /* Wait for the write to finish. */
                do
                {
#if ( (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON))
                    Fls_Ipw_u32QspiElapsedTicks += OsIf_GetElapsed(&Fls_Ipw_u32QspiCurrentTicks, (OsIf_CounterType)QSPI_IP_TIMEOUT_TYPE);
                    if (Fls_Ipw_u32QspiElapsedTicks >= Fls_Ipw_u32QspiTimeoutTicks)
                    {
                        Status = STATUS_QSPI_IP_TIMEOUT;
                        (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_TIMEOUT);
                        break;
                    }
                    else
                    {
                        ; /* Empty clause added to fulfill MISRA. */
                    }
#endif /* (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON) */
                    Status = Qspi_Ip_GetMemoryStatus(FlashInstance);
                }
                while (STATUS_QSPI_IP_BUSY == Status);

                if (STATUS_QSPI_IP_SUCCESS != Status)
                {
                    LldRetVal = FLASH_E_FAILED;
                }
#if (FLS_WRITE_VERIFICATION_ENABLED == STD_ON)
                else if (STATUS_QSPI_IP_SUCCESS != Qspi_Ip_ProgramVerify(FlashInstance, Fls_Ipw_xExtJobStartAddr, Fls_Ipw_pExtJobBuffAddr, Fls_Ipw_xExtJobChunkSize))
                {
                    (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_VERIFY_WRITE_FAILED);
                    LldRetVal = FLASH_E_FAILED;
                }
#endif /* (FLS_WRITE_VERIFICATION_ENABLED == STD_ON) */
                else
                {
                    ; /* Empty clause added to fulfill MISRA. */
                }
            }
            else
            {
                /* Schedule async write job. */
                Fls_eLLDJobResult = MEMIF_JOB_PENDING;
                Fls_eLLDJob = FLASH_JOB_WRITE;
                LldRetVal = FLASH_E_PENDING;
            }
        }
#if (FLS_ERASE_VERIFICATION_ENABLED == STD_ON)
    }
#endif /* (FLS_ERASE_VERIFICATION_ENABLED == STD_ON) */

    return LldRetVal;
}

/*
 * Function Name : Fls_IPW_LLDMainFunctionQspiErase
 * Description   : Handles ASYNC QSPI erase
*/
static void Fls_IPW_LLDMainFunctionQspiErase(uint32 FlashInstance)
{
#if (FLS_ERASE_VERIFICATION_ENABLED == STD_OFF)
    (void)FlashInstance;
#else
    Fls_AddressType ChunkSize = FLS_MAX_ERASE_BLANK_CHECK;

    if (Fls_Ipw_xExtJobSize < FLS_MAX_ERASE_BLANK_CHECK)
    {
        ChunkSize = Fls_Ipw_xExtJobSize;
    }
    Fls_Ipw_xExtJobSize -= ChunkSize;
    /* Verify that the sector was successfully erased. */
    if (STATUS_QSPI_IP_SUCCESS != Qspi_Ip_EraseVerify(FlashInstance, Fls_Ipw_xExtJobStartAddr, ChunkSize))
    {
        /* Error, the memory locations are not erased or there was an error when attempting to read it. */
        (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_VERIFY_ERASE_FAILED);
        Fls_eLLDJob = FLASH_JOB_NONE;
        Fls_eLLDJobResult = MEMIF_JOB_FAILED;
    }
    if ((Fls_Ipw_xExtJobSize == 0U) && (Fls_eLLDJob != FLASH_JOB_NONE))
#endif /* (FLS_ERASE_VERIFICATION_ENABLED == STD_ON) */
    {
        /* Erase operation succeeded */
        Fls_eLLDJob = FLASH_JOB_NONE;
        Fls_eLLDJobResult = MEMIF_JOB_OK;
        if ((Fls_u32JobAddrIt > ((*(Fls_pConfigPtr->paSectorEndAddr))[Fls_u32JobSectorIt])))
        {
            /* Move on to the next sector */
            Fls_u32JobSectorIt++;
        }
    }
}


/*
 * Function Name : Fls_IPW_GetExtFlashConfig
 * Description   : Handles ASYNC QSPI write
*/
static const Qspi_Ip_MemoryConfigType * Fls_IPW_GetExtFlashConfig(uint32 FlashInstance)
{
    const Qspi_Ip_MemoryConfigType *FlashConfig = NULL_PTR;
    uint32 FlashConfigNo;

    /* Get external flash configuration */
    FlashConfigNo = (*(Fls_pConfigPtr->pFlsQspiCfgConfig->u8FlashConfig))[FlashInstance];
    if (FlashConfigNo != FLS_IPW_CFG_INVALID)
    {
        FlashConfig = &((*(Fls_pConfigPtr->pFlsQspiCfgConfig->paFlashCfg))[FlashConfigNo]);
    }
#if (defined(FLS_DEVICES_USING_SFDP))
#if (FLS_DEVICES_USING_SFDP > 0)
    else
    {
        /* SFDP auto-filled configuration */
        FlashConfigNo = Fls_Ipw_au8SfdpConfigsIndex[FlashInstance];
        FlashConfig = &(Fls_Ipw_axSfdpConfigs[FlashConfigNo].sfdpCfg);
    }
#endif
#endif
    return FlashConfig;
}


/*
 * Function Name : Fls_IPW_LLDMainFunctionQspiWrite
 * Description   : Handles ASYNC QSPI write
*/
static void Fls_IPW_LLDMainFunctionQspiWrite(uint32 FlashInstance)
{
    const Qspi_Ip_MemoryConfigType *FlashConfig;
    Fls_LLDReturnType LldRetVal = FLASH_E_FAILED;
    uint32 MemAlignment;

#if (FLS_WRITE_VERIFICATION_ENABLED == STD_ON)
    if (STATUS_QSPI_IP_SUCCESS != Qspi_Ip_ProgramVerify(FlashInstance, Fls_Ipw_xExtJobStartAddr, Fls_Ipw_pExtJobBuffAddr, Fls_Ipw_xExtJobChunkSize))
    {
        /* Error, the memory location was not programmed with the desired data. */
        (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_VERIFY_WRITE_FAILED);
        Fls_eLLDJob = FLASH_JOB_NONE;
        Fls_eLLDJobResult = MEMIF_JOB_FAILED;
    }
    else
#endif /* (FLS_WRITE_VERIFICATION_ENABLED == STD_ON) */
    {
        /* Chunk write succeeded, check if there are more chunks */
        if (Fls_Ipw_xExtJobSize == Fls_Ipw_xExtJobChunkSize)
        {
            /* This was the last chunk, operation is completed */
            Fls_eLLDJob = FLASH_JOB_NONE;
            Fls_eLLDJobResult = MEMIF_JOB_OK;
            /* For a Write Job in ASYNC mode check if Fls_u32JobSectorIt should be increased */
            if ((Fls_u32JobAddrIt > ((*(Fls_pConfigPtr->paSectorEndAddr))[Fls_u32JobSectorIt])))
            {
                /* Move on to the next sector */
                Fls_u32JobSectorIt++;
            }
        }
        else
        {
            /* Get external flash configuration */
            FlashConfig = Fls_IPW_GetExtFlashConfig(FlashInstance);
            /* Get memory alignement */
            MemAlignment = (uint32)(((*(Fls_pConfigPtr->pFlsQspiCfgConfig->paFlashConnectionCfg))[FlashInstance]).memAlignment);

            /* Update job parameters */
            Fls_Ipw_xExtJobStartAddr += Fls_Ipw_xExtJobChunkSize;
            Fls_Ipw_pExtJobBuffAddr = &(Fls_Ipw_pExtJobBuffAddr[Fls_Ipw_xExtJobChunkSize]);
            Fls_Ipw_xExtJobSize -= Fls_Ipw_xExtJobChunkSize;
            Fls_Ipw_xExtJobChunkSize = Fls_IPW_ComputeWriteChunkSize(Fls_Ipw_xExtJobSize, Fls_Ipw_xExtJobStartAddr, FlashConfig->pageSize, MemAlignment);
            /* Launch async write of next chunk */
            LldRetVal = Fls_IPW_SectorWriteChunk((boolean)TRUE);
            if (LldRetVal == FLASH_E_FAILED)
            {
                /* Error, cannot launch write for next chunk */
                Fls_eLLDJob = FLASH_JOB_NONE;
                Fls_eLLDJobResult = MEMIF_JOB_FAILED;
            }
        }
    }
}

/*
 * Function Name : Fls_IPW_LLDMainFunctionQspiJobs
 * Description   : Handles ASYNC QSPI jobs
*/
static void Fls_IPW_LLDMainFunctionQspiJobs(void)
{
    Qspi_Ip_StatusType Status;
    uint32 FlashInstance;

    /* Get external flash instance */
    FlashInstance = (*(Fls_pConfigPtr->pFlsQspiCfgConfig->u8SectFlashUnit))[Fls_u32JobSectorIt];

    Status = Qspi_Ip_GetMemoryStatus(FlashInstance);
    if (STATUS_QSPI_IP_BUSY != Status)
    {
        if (STATUS_QSPI_IP_SUCCESS != Status)
        {
            /* Error, unable to retrieve flash device Status */
            if (FLASH_JOB_ERASE == Fls_eLLDJob)
            {
                (void)Det_ReportTransientFault((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_ERASE_FAILED);
            }
            else if (FLASH_JOB_WRITE == Fls_eLLDJob )
            {

                (void)Det_ReportTransientFault((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_WRITE_FAILED);
            }
            else
            {
                ; /* Do nothing - should not happen in Fully Trusted Environment;
                'else' clause added to fulfil MISRA Rule 14.10 */
            }

            Fls_eLLDJob = FLASH_JOB_NONE;
            Fls_eLLDJobResult = MEMIF_JOB_FAILED;
        }
        else
        {
            if (FLASH_JOB_ERASE == Fls_eLLDJob)
            {
                Fls_IPW_LLDMainFunctionQspiErase(FlashInstance);
            }
            else if (FLASH_JOB_WRITE == Fls_eLLDJob )
            {
                Fls_IPW_LLDMainFunctionQspiWrite(FlashInstance);
            }
            else
            {
                ; /* Do nothing - should not happen in Fully Trusted Environment;
                'else' clause added to fulfil MISRA Rule 14.10 */
            }
        }
    }   /* if (STATUS_QSPI_IP_BUSY != Status ) */
    else
    {
        /* device busy, check timeout */
#if ( (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON))
        Fls_Ipw_u32QspiElapsedTicks += OsIf_GetElapsed(&Fls_Ipw_u32QspiCurrentTicks, (OsIf_CounterType)QSPI_IP_TIMEOUT_TYPE);
        if (Fls_Ipw_u32QspiElapsedTicks >= Fls_Ipw_u32QspiTimeoutTicks)
        {
            /* operation timed out */
            (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_TIMEOUT);

            if (FLASH_JOB_ERASE == Fls_eLLDJob)
            {
                (void)Det_ReportTransientFault((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_ERASE_FAILED);
            }
            else if (FLASH_JOB_WRITE == Fls_eLLDJob )
            {
                (void)Det_ReportTransientFault((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_WRITE_FAILED);
            }
            else
            {
                ; /* Do nothing - should not happen in Fully Trusted Environment;
                'else' clause added to fulfil MISRA Rule 14.10 */
            }

            Fls_eLLDJob = FLASH_JOB_NONE;
            Fls_eLLDJobResult = MEMIF_JOB_FAILED;
        }
#endif /* (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON) */
    }   /* if (STATUS_QSPI_IP_BUSY != Status ) */
}


static inline Fls_LLDReturnType Fls_IPW_SectorEraseQspiJobs(boolean Asynch,
                                                            Fls_AddressType SectorOffset,
                                                            Fls_LengthType PhysicalSectorSize
                                                           )
{
    Fls_LLDReturnType LldRetVal = FLASH_E_FAILED;
    Qspi_Ip_StatusType Status = STATUS_QSPI_IP_SUCCESS;
    uint32 FlashInstance;

    /* Get external flash instance */
    FlashInstance = (*(Fls_pConfigPtr->pFlsQspiCfgConfig->u8SectFlashUnit))[Fls_u32JobSectorIt];
    /* Compute target address : sector start address */
    Fls_Ipw_xExtJobStartAddr = (*(Fls_pConfigPtr->paSectorHwAddress))[Fls_u32JobSectorIt] + SectorOffset;
    /* Record job size */
    Fls_Ipw_xExtJobSize = PhysicalSectorSize;

    /* Call IP routine to erase external sector. */
    if (STATUS_QSPI_IP_SUCCESS != Qspi_Ip_EraseBlock(FlashInstance, Fls_Ipw_xExtJobStartAddr, Fls_Ipw_xExtJobSize))
    {
        LldRetVal = FLASH_E_FAILED;
    }
    else
    {
#if ( (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON))
        /* Prepare timeout counter */
        Fls_Ipw_u32QspiElapsedTicks = 0U;
        Fls_Ipw_u32QspiTimeoutTicks = OsIf_MicrosToTicks(((boolean)FALSE == Asynch)?FLS_QSPI_SYNC_ERASE_TIMEOUT:FLS_QSPI_ASYNC_ERASE_TIMEOUT, (OsIf_CounterType)QSPI_IP_TIMEOUT_TYPE);
        Fls_Ipw_u32QspiCurrentTicks = OsIf_GetCounter((OsIf_CounterType)QSPI_IP_TIMEOUT_TYPE);
#endif /* (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON) */
        if ((boolean)FALSE == Asynch) /*SYNC Mode*/
        {
            LldRetVal = FLASH_E_OK;
            /* Wait for the erase to finish. */
            do
            {
#if ( (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON))
                Fls_Ipw_u32QspiElapsedTicks += OsIf_GetElapsed(&Fls_Ipw_u32QspiCurrentTicks, (OsIf_CounterType)QSPI_IP_TIMEOUT_TYPE);
                if (Fls_Ipw_u32QspiElapsedTicks >= Fls_Ipw_u32QspiTimeoutTicks)
                {
                    Status = STATUS_QSPI_IP_TIMEOUT;
                    (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_TIMEOUT);
                    break;
                }
#endif /* (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON) */
                Status = Qspi_Ip_GetMemoryStatus(FlashInstance);
            }
            while (STATUS_QSPI_IP_BUSY == Status);

            if (STATUS_QSPI_IP_SUCCESS != Status)
            {
                LldRetVal = FLASH_E_FAILED;
            }
#if (FLS_ERASE_VERIFICATION_ENABLED == STD_ON)
            /* Verify that the sector was succesfully erased. */
            else if (STATUS_QSPI_IP_SUCCESS != Qspi_Ip_EraseVerify(FlashInstance, Fls_Ipw_xExtJobStartAddr, Fls_Ipw_xExtJobSize))
            {
                (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_VERIFY_ERASE_FAILED);
                LldRetVal = FLASH_E_FAILED;
            }
#endif
            else
            {
                ; /* Empty clause added to fulfill MISRA. */
            }
        }
        else
        {
            /* Schedule async erase job. */
            Fls_eLLDJobResult = MEMIF_JOB_PENDING;
            Fls_eLLDJob = FLASH_JOB_ERASE;
            LldRetVal = FLASH_E_PENDING;
        }
    }

    return LldRetVal;
}


static inline Fls_LLDReturnType Fls_IPW_SectorWriteQspiJobs(const Fls_AddressType SectorOffset,
                                                            const Fls_AddressType Length,
                                                            const uint8 *JobDataSrcPtr,
                                                            const boolean Asynch
                                                           )
{
    Fls_LLDReturnType LldRetVal;
    const Qspi_Ip_MemoryConfigType *FlashConfig;
    uint32 FlashInstance;
    uint32 MemAlignment;

    /* Get external flash instance */
    FlashInstance = (*(Fls_pConfigPtr->pFlsQspiCfgConfig->u8SectFlashUnit))[Fls_u32JobSectorIt];
    /* Get external flash configuration */
    FlashConfig = Fls_IPW_GetExtFlashConfig(FlashInstance);
    /* Get memory alignement */
    MemAlignment = (uint32)(((*(Fls_pConfigPtr->pFlsQspiCfgConfig->paFlashConnectionCfg))[FlashInstance]).memAlignment);

    /* Compute target address : sector address + offset */
    Fls_Ipw_xExtJobStartAddr = (*(Fls_pConfigPtr->paSectorHwAddress))[Fls_u32JobSectorIt] + SectorOffset;
    /* Record source pointer */
    Fls_Ipw_pExtJobBuffAddr = JobDataSrcPtr;
    /* Record job size */
    Fls_Ipw_xExtJobSize = Length;
    /* Compute next chunk size - consider QSPI and external device restrictions */
    Fls_Ipw_xExtJobChunkSize = Fls_IPW_ComputeWriteChunkSize(Fls_Ipw_xExtJobSize, Fls_Ipw_xExtJobStartAddr, FlashConfig->pageSize, MemAlignment);

#if ( (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON))
    /* Prepare timeout counter */
    Fls_Ipw_u32QspiElapsedTicks = 0U;
    Fls_Ipw_u32QspiTimeoutTicks = OsIf_MicrosToTicks(((boolean)FALSE == Asynch)?FLS_QSPI_SYNC_WRITE_TIMEOUT:FLS_QSPI_ASYNC_WRITE_TIMEOUT, (OsIf_CounterType)QSPI_IP_TIMEOUT_TYPE);
    Fls_Ipw_u32QspiCurrentTicks = OsIf_GetCounter((OsIf_CounterType)QSPI_IP_TIMEOUT_TYPE);
#endif /* (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON) */

    /* Write the first chunk */
    LldRetVal = Fls_IPW_SectorWriteChunk(Asynch);
    if ((boolean)FALSE == Asynch)
    {    /* SYNC Mode - complete operation */
        while ((LldRetVal == FLASH_E_OK) && (Fls_Ipw_xExtJobSize > Fls_Ipw_xExtJobChunkSize))
        {
            /* Update job parameters */
            Fls_Ipw_xExtJobStartAddr += Fls_Ipw_xExtJobChunkSize;
            Fls_Ipw_pExtJobBuffAddr = &(Fls_Ipw_pExtJobBuffAddr[Fls_Ipw_xExtJobChunkSize]);
            Fls_Ipw_xExtJobSize -= Fls_Ipw_xExtJobChunkSize;
            Fls_Ipw_xExtJobChunkSize = Fls_IPW_ComputeWriteChunkSize(Fls_Ipw_xExtJobSize, Fls_Ipw_xExtJobStartAddr, FlashConfig->pageSize, MemAlignment);
            /* Write current chunk */
            LldRetVal = Fls_IPW_SectorWriteChunk(Asynch);
        }
    }

    return LldRetVal;
}


/*
 * Function Name : Fls_IPW_InitControllers
 * Description   : Initialize QSPI controllers
*/
static Qspi_Ip_StatusType Fls_IPW_InitControllers(void)
{
    Qspi_Ip_StatusType Status = STATUS_QSPI_IP_SUCCESS;
    uint8 Config = 0U;
    uint8 Cnt = 0U;

    for (Cnt = 0U; Cnt < Fls_pConfigPtr->pFlsQspiCfgConfig->u8QspiUnitsCount; Cnt++)
    {
        /* Get configuration for current QSPI device */
        Config = (*(Fls_pConfigPtr->pFlsQspiCfgConfig->u8QspiConfig))[Cnt];
        if (Config != FLS_IPW_CFG_INVALID)
        {
            /* Use configuration "Config" to initialize QSPI controller "Cnt" */
            Status = Qspi_Ip_ControllerInit(Cnt, &((*(Fls_pConfigPtr->pFlsQspiCfgConfig->paQspiUnitCfg))[Config]));
            if (STATUS_QSPI_IP_SUCCESS != Status)
            {
                break;
            }
        }
    }

    return Status;
}

/*
 * Function Name : Fls_IPW_InitMemories
 * Description   : Initialize Qspi external flash devices
*/
static Qspi_Ip_StatusType Fls_IPW_InitMemories(void)
{
    Qspi_Ip_StatusType Status = STATUS_QSPI_IP_SUCCESS;
    uint8 Config = 0U;
    uint8 Cnt = 0U;
#if (defined(FLS_DEVICES_USING_SFDP))
#if (FLS_DEVICES_USING_SFDP > 0)
    Qspi_Ip_MemoryConfigType *flashCfg;
    /* Current SFDP configuration  */
    uint8 Fls_Qspi_SfdpConfigsCount = 0U;
#endif
#endif

    for (Cnt = 0U; Cnt < Fls_pConfigPtr->pFlsQspiCfgConfig->u8FlashUnitsCount; Cnt++)
    {
        /* Get configuration for current QSPI device */
        Config = (*(Fls_pConfigPtr->pFlsQspiCfgConfig->u8FlashConfig))[Cnt];
        if (Config != FLS_IPW_CFG_INVALID)
        {
            /* Use configuration "Config" to initialize flash device "Cnt" */
            Status = Qspi_Ip_Init(Cnt, &((*(Fls_pConfigPtr->pFlsQspiCfgConfig->paFlashCfg))[Config]), &((*(Fls_pConfigPtr->pFlsQspiCfgConfig->paFlashConnectionCfg))[Cnt]));
        }
#if (defined(FLS_DEVICES_USING_SFDP))
#if (FLS_DEVICES_USING_SFDP > 0)
        else
        {
            /* This device must auto-configure using SFDP */
            /* Prepare configuration structure, link lut and init operations arrays */
            flashCfg = &(Fls_Ipw_axSfdpConfigs[Fls_Qspi_SfdpConfigsCount].sfdpCfg);
            flashCfg->lutSequences.opCount = FLS_QSPI_SFDP_LUT_SIZE;
            flashCfg->lutSequences.lutOps = Fls_Ipw_axSfdpConfigs[Fls_Qspi_SfdpConfigsCount].lutOps;
            flashCfg->initConfiguration.opCount = FLS_QSPI_SFDP_INIT_OP_SIZE;
            flashCfg->initConfiguration.operations = Fls_Ipw_axSfdpConfigs[Fls_Qspi_SfdpConfigsCount].initOps;
            Status = Qspi_Ip_ReadSfdp(flashCfg, &((*(Fls_pConfigPtr->pFlsQspiCfgConfig->paFlashConnectionCfg))[Cnt]));
            if (STATUS_QSPI_IP_SUCCESS == Status)
            {
                /* Qspi_Ip_ReadSfdp auto-filled configuration, use it to initialize flash device */
                Status = Qspi_Ip_Init(Cnt, flashCfg, &((*(Fls_pConfigPtr->pFlsQspiCfgConfig->paFlashConnectionCfg))[Cnt]));
            }
            /* Store index to allow configuration to be retrieved later */
            Fls_Ipw_au8SfdpConfigsIndex[Cnt] = Fls_Qspi_SfdpConfigsCount;
            Fls_Qspi_SfdpConfigsCount++;
        }
#endif
#endif

        /* Configure the AHB reads for flash unit "Cnt" */
        if ( (STATUS_QSPI_IP_SUCCESS == Status) && ((boolean)TRUE == (*(Fls_pConfigPtr->pFlsQspiCfgConfig->paAHBReadCfg))[Cnt]) )
        {
            Status = Qspi_Ip_AhbReadEnable( ((*(Fls_pConfigPtr->pFlsQspiCfgConfig->paFlashConnectionCfg))[Cnt]).qspiInstance );
        }

        if (STATUS_QSPI_IP_SUCCESS != Status)
        {
            break;
        }
    }

    return Status;
}

/*
 * Function Name : Fls_IPW_CheckDevicesId
 * Description   : Check the identification of the external flash device against the configured one
*/
static boolean Fls_IPW_CheckDevicesId(void)
{
    Qspi_Ip_StatusType Status = STATUS_QSPI_IP_SUCCESS;
    boolean RetVal = (boolean)TRUE;
    const Qspi_Ip_MemoryConfigType * Config;
    uint32 ReadId = 0U;
    uint8 Cnt;

    for (Cnt = 0U; Cnt < Fls_pConfigPtr->pFlsQspiCfgConfig->u8FlashUnitsCount; Cnt++)
    {
        /* Get configuration for current QSPI device */
        Config = Fls_IPW_GetExtFlashConfig(Cnt);

        /* If enabled, check identification of the external flash device */
        if (QSPI_IP_LUT_INVALID != Config->readIdSettings.readIdLut)
        {
            Status = Qspi_Ip_ReadId(Cnt, (uint8 *)&ReadId);
            /* Compare current device's ID with the corresponding configuration */
            if ( (STATUS_QSPI_IP_SUCCESS != Status) || (ReadId != Config->readIdSettings.readIdExpected) )
            {
                (void)Det_ReportTransientFault((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_INIT_ID, FLS_E_UNEXPECTED_FLASH_ID);
                RetVal = (boolean)FALSE;
                break;
            }
        }
    }
    return RetVal;
}

#endif /* (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */

/*
 * Function Name : Fls_IPW_Init
 * Description   : Initialize Fls module
 * @implements     Fls_IPW_Init_Activity
*/
void Fls_IPW_Init(void)
{
    Fls_eLLDJobResult = MEMIF_JOB_OK;
#if (FLS_QSPI_SECTORS_CONFIGURED == STD_ON)
    Qspi_Ip_StatusType Status;
    boolean CheckId = (boolean)FALSE;

    /* Initialize QSPI controllers */
    Status = Fls_IPW_InitControllers();

    /* Initialize external flash devices */
    if (STATUS_QSPI_IP_SUCCESS == Status)
    {
        Status = Fls_IPW_InitMemories();
    }

    /* Check the hardware ID of the external flash devices */
    if (STATUS_QSPI_IP_SUCCESS == Status)
    {
        CheckId = Fls_IPW_CheckDevicesId();
    }

    if ((boolean)TRUE == CheckId)
    {
        Fls_eLLDJobResult = MEMIF_JOB_OK;
    }
    else
    {
        Fls_eLLDJobResult = MEMIF_JOB_FAILED;
    }
#endif /* (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */

#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
    if (MEMIF_JOB_OK == Fls_eLLDJobResult)
    {
        if (STATUS_C40_IP_SUCCESS == C40_Ip_Init(Fls_pConfigPtr->pFlsInternalCfgConfig))
        {
            Fls_eLLDJobResult = MEMIF_JOB_OK;
        }
        else
        {
            Fls_eLLDJobResult = MEMIF_JOB_FAILED;
        }
    }
#endif /* (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED) */
    Fls_eLLDJob = FLASH_JOB_NONE;
}

/*
 * Function Name : Fls_IPW_AbortSuspended
 * Description   : Abort suspended Fls module
 * @implements     Fls_IPW_AbortSuspended_Activity
*/
void Fls_IPW_AbortSuspended(void)
{
#if (FLS_QSPI_SECTORS_CONFIGURED == STD_ON)
    Fls_HwChType HwCh;
    Fls_SectorIndexType SectorIndexIter;
    Qspi_Ip_StatusType Status = STATUS_QSPI_IP_SUCCESS;
    uint32 FlashInstance;
    uint32 ControllerInstance;
    uint32 LastFlashInstance = FLS_DEVICE_INSTANCE_INVALID;

    for (SectorIndexIter = Fls_u32JobSectorIt; SectorIndexIter <= Fls_u32JobSectorEnd; SectorIndexIter++ )
    {
        /* Get channel type(INTERNAL, EXTERNAL_A1, A2,...) to determine the HW IP used(internal or external flash). */
        HwCh = (*(Fls_pConfigPtr->paHwCh))[SectorIndexIter];
         /* Get external flash instance */
        FlashInstance = (*(Fls_pConfigPtr->pFlsQspiCfgConfig->u8SectFlashUnit))[SectorIndexIter];

        /* Check if this channel already was checked before */
        if ( (FLS_CH_QSPI == HwCh) && (FlashInstance != LastFlashInstance) )
        {
            LastFlashInstance = FlashInstance;
            /* Get controller instance */
            ControllerInstance = ((*(Fls_pConfigPtr->pFlsQspiCfgConfig->paFlashConnectionCfg))[FlashInstance]).qspiInstance;

            /* Prepare timeout counter */
            Fls_Ipw_u32QspiElapsedTicks = 0U;
            Fls_Ipw_u32QspiTimeoutTicks = OsIf_MicrosToTicks(QSPI_IP_CMD_COMPLETE_TIMEOUT, (OsIf_CounterType)QSPI_IP_TIMEOUT_TYPE);
            Fls_Ipw_u32QspiCurrentTicks = OsIf_GetCounter((OsIf_CounterType)QSPI_IP_TIMEOUT_TYPE);
            /* Wait for the controller to become idle */
            do
            {
                /* Add Fault Injection point for FR_ILLINE flag */
                MCAL_FAULT_INJECTION_POINT(FLS_FIP_FR_ERROR_ABORTSUSPEND);

                Status = Qspi_Ip_ControllerGetStatus(ControllerInstance);
                Fls_Ipw_u32QspiElapsedTicks += OsIf_GetElapsed(&Fls_Ipw_u32QspiCurrentTicks, (OsIf_CounterType)QSPI_IP_TIMEOUT_TYPE);
                if ((STATUS_QSPI_IP_BUSY == Status) && (Fls_Ipw_u32QspiElapsedTicks >= Fls_Ipw_u32QspiTimeoutTicks))
                {
                    Status = STATUS_QSPI_IP_TIMEOUT;
                }
            }
            while (STATUS_QSPI_IP_BUSY == Status);

            if (STATUS_QSPI_IP_SUCCESS == Status)
            {
                /* Check that external memory is idle */
                if (STATUS_QSPI_IP_SUCCESS != Qspi_Ip_GetMemoryStatus(FlashInstance))
                {
                    /* Reset external memory */
                    Status = Qspi_Ip_Reset(FlashInstance);
                }
            }

            if (STATUS_QSPI_IP_SUCCESS != Status)
            {
                /*Exit for-loop*/
                Fls_eLLDJob = FLASH_JOB_NONE;
                Fls_eLLDJobResult = MEMIF_JOB_FAILED;
                break;
            }
        }
    }

#endif /* (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */
}

void Fls_IPW_ClearHVJob(void)
{
    /* Hardware did not support this feature */
}

#if (FLS_CANCEL_API == STD_ON )

/*
 * Function Name : Fls_IPW_Cancel
 * Description   : Cancel Fls module
 * @implements     Fls_IPW_Cancel_Activity
*/
void Fls_IPW_Cancel(void)
{
    Fls_HwChType HwCh;

    /* Get channel type(INTERNAL, QSPI,...) to determine the HW IP used(internal or external flash). */
    HwCh = (*(Fls_pConfigPtr->paHwCh))[Fls_u32JobSectorIt];

    if (MEMIF_JOB_PENDING == Fls_eLLDJobResult )
    {
        #if (FLS_INTERNAL_SECTORS_CONFIGURED == STD_ON)
        if (FLS_CH_INTERN == HwCh)
        {
            if (STATUS_C40_IP_SUCCESS == C40_Ip_Abort())
            {
                /* Mark the job as cancelled.*/
                Fls_eLLDJob = FLASH_JOB_NONE;
                Fls_eLLDJobResult = MEMIF_JOB_CANCELED;
            }
        }
        #endif /* (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED) */

        #if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
        if (FLS_CH_QSPI == HwCh)
        {
            /* Mark the job as cancelled.*/
            Fls_eLLDJob = FLASH_JOB_NONE;
            Fls_eLLDJobResult = MEMIF_JOB_CANCELED;
        }
        #endif /* (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */
    }
    else
    {
        /* no hardware job (asynchronous) is pending, do nothing */

        /* Mark the internal job as cancelled.*/
        Fls_eLLDJob = FLASH_JOB_NONE;
        Fls_eLLDJobResult = MEMIF_JOB_CANCELED;
    }
}
#endif


/**
* @brief          Process ongoing erase or write hardware job.
* @details        In case Async Operation is ongoing this function will complete the following job:
*                 - Erase
*                 - Erase on Interleaved sectors
*                 - Write
*                 - Erase blank Check
*
* @note           Dem_ReportErrorStatus(Fls_pConfigPtr->Fls_E_EraseFailedCfg.id, DEM_EVENT_STATUS_FAILED)
*                 when erase operation failed due to hardware error.
*                 Dem_ReportErrorStatus(Fls_pConfigPtr->Fls_E_WriteFailedCfg.id, DEM_EVENT_STATUS_FAILED)
*                 when write operation failed due to hardware error
* @implements     Fls_IPW_LLDMainFunction_Activity
*/
void Fls_IPW_LLDMainFunction( void )
{
    Fls_HwChType HwCh;

    /* Get channel type(INTERNAL, EXTERNAL_A1, A2,...) to determine the HW IP used(internal or external flash). */
    HwCh = (*(Fls_pConfigPtr->paHwCh))[Fls_u32JobSectorIt];

    if (MEMIF_JOB_PENDING == Fls_eLLDJobResult)
    {
#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
        if (FLS_CH_INTERN == HwCh)
        {
            Fls_IPW_LLDMainFunctionC40Jobs();
        }
#endif /* (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED) */

#if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
        if (FLS_CH_QSPI == HwCh)
        {
            Fls_IPW_LLDMainFunctionQspiJobs();
        }
#endif /* (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */

    }   /* if (MEMIF_JOB_PENDING == Fls_eLLDJobResult) */
    else
    {
        /* no hardware job (asynchronous) is pending, do nothing */
    }
}


/**
* @brief          IP wrapper sector read function.
* @details        Route the read job to appropriate low level IP function.
* @implements     Fls_IPW_SectorRead_Activity
*/
Fls_LLDReturnType Fls_IPW_SectorRead(const Fls_AddressType SectorOffset,
                                     const Fls_AddressType Length,
                                     uint8 *JobDataDestPtr,
                                     const uint8 *JobDataSrcPtr
                                    )
{
    Fls_LLDReturnType LldRetVal = FLASH_E_FAILED;
    Fls_HwChType HwCh;
#if ( (FLS_COMPARE_API == STD_OFF) && (FLS_BLANK_CHECK_API == STD_OFF) )
    (void)JobDataSrcPtr;
#endif  /* (( FLS_COMPARE_API == STD_ON ) || ( FLS_BLANK_CHECK_API == STD_ON )) */
#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
    C40_Ip_StatusType ReturnCode = STATUS_C40_IP_ERROR;
    uint32 ReadStartAddress = 0U;
#endif /* #if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED) */
#if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
    Qspi_Ip_StatusType Status = STATUS_QSPI_IP_SUCCESS;
    uint32 FlashInstance;
    Fls_AddressType ReadAddr;
#endif /* #if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */

    /* Get channel type(INTERNAL, QSPI,...) to determine the HW IP used(internal or external flash). */
    HwCh = (*(Fls_pConfigPtr->paHwCh))[Fls_u32JobSectorIt];

    /* Decide the IP used: internal flash or external QSPI */
#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
    if (FLS_CH_INTERN == HwCh)
    {
        /* Get the base address of the sector. */
        ReadStartAddress = (uint32)(*((*(Fls_pConfigPtr->pSectorList))[Fls_u32JobSectorIt])).pSectorStartAddressPtr;
        /* Add the offset */
        ReadStartAddress += SectorOffset;

        /* Decide whether the job is a Read, a compare, or a blank check. */
        if (NULL_PTR != JobDataDestPtr)
        {
#if (STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS)
            if ( NULL_PTR != Fls_pConfigPtr->FlsReadFunctionCallout )
            {
                /* Users will have to define Callout function to call Fls_ReadEachBlock to read data from memory */
                ReturnCode = Fls_IPW_CopyBlock(ReadStartAddress, Length, JobDataDestPtr, NULL_PTR);
            }
            else
#endif /* STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS */
            {
                ReturnCode = C40_Ip_Read(ReadStartAddress, Length, JobDataDestPtr);
            }
        }
#if (( FLS_COMPARE_API == STD_ON ) || ( FLS_BLANK_CHECK_API == STD_ON ))
        else
        {
#if (STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS)
            if ( NULL_PTR != Fls_pConfigPtr->FlsReadFunctionCallout )
            {
                /* Users will have to define Callout function to call Fls_ReadEachBlock to read data from memory */
                ReturnCode = Fls_IPW_CopyBlock(ReadStartAddress, Length, NULL_PTR, JobDataSrcPtr);
            }
            else
#endif /* STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS */
            {
                ReturnCode = C40_Ip_Compare(ReadStartAddress, Length, JobDataSrcPtr);
            }
        }
#endif  /* (( FLS_COMPARE_API == STD_ON ) || ( FLS_BLANK_CHECK_API == STD_ON )) */

        LldRetVal = Fls_IPW_TranslateReturnCode(ReturnCode);
    }
#endif /* (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED) */

#if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
    if (FLS_CH_QSPI == HwCh)
    {
        /* Get external flash instance */
        FlashInstance = (*(Fls_pConfigPtr->pFlsQspiCfgConfig->u8SectFlashUnit))[Fls_u32JobSectorIt];
        /* Initialize job parameters */
        /* Compute target address : sector address + offset */
        ReadAddr = (*(Fls_pConfigPtr->paSectorHwAddress))[Fls_u32JobSectorIt] + SectorOffset;

        /* Fault Injection point for testing when read data is executing */
        MCAL_FAULT_INJECTION_POINT(FLS_FIP_FR_ERROR_DATA_IPREAD);

        /* Decide whether the job is a Read, a compare, or a blank check. */
        if (NULL_PTR != JobDataDestPtr)
        {
            Status = Qspi_Ip_Read(FlashInstance, ReadAddr, JobDataDestPtr, Length);
        }
#if ( FLS_COMPARE_API == STD_ON )
        else if (NULL_PTR != JobDataSrcPtr)
        {
            Status = Qspi_Ip_ProgramVerify(FlashInstance, ReadAddr, JobDataSrcPtr, Length);
        }
#endif /*#if ( FLS_COMPARE_API == STD_ON ) */
#if ( FLS_BLANK_CHECK_API == STD_ON )
        else
        {
            Status = Qspi_Ip_EraseVerify(FlashInstance, ReadAddr, Length);
        }
#endif /*#if ( FLS_BLANK_CHECK_API == STD_ON ) */
        (void)JobDataSrcPtr;

        if (STATUS_QSPI_IP_SUCCESS != Status)
        {
#if (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON)
            /*check timeout after checking Status fail*/
            if (STATUS_QSPI_IP_TIMEOUT == Status)
            {
                (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_TIMEOUT);
            }
#endif /* (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON) */
#if ( FLS_COMPARE_API == STD_ON )
            if (STATUS_QSPI_IP_ERROR_PROGRAM_VERIFY == Status)
            {
                LldRetVal = FLASH_E_BLOCK_INCONSISTENT;
            }
            else
            {
                LldRetVal = FLASH_E_FAILED;
            }
#else
            LldRetVal = FLASH_E_FAILED;
#endif /*#if ( FLS_COMPARE_API == STD_ON ) */
        }
        else
        {
            LldRetVal = FLASH_E_OK;
        }
    } /* FLS_CH_QSPI == HwCh */
#endif /* (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */

    return LldRetVal;
}

/**
* @brief          IP wrapper sector erase function.
* @details        Route the erase job to appropriate low level IP function.
* @implements     Fls_IPW_SectorErase_Activity
*/
Fls_LLDReturnType Fls_IPW_SectorErase(const Fls_AddressType SectorOffset,
                                      const Fls_LengthType PhysicalSectorSize,
                                      const boolean Asynch
                                     )
{
    Fls_LLDReturnType LldRetVal = FLASH_E_FAILED;
    Fls_HwChType HwCh;

    /* Get channel type(INTERNAL, QSPI,...) to determine the HW IP used(internal or external flash). */
    HwCh = (*(Fls_pConfigPtr->paHwCh))[Fls_u32JobSectorIt];

    /* Decide the IP used: internal flash or external QSPI */
#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
    if (FLS_CH_INTERN == HwCh)
    {
        LldRetVal = Fls_IPW_SectorEraseC40Jobs(Asynch, SectorOffset);
    }
#endif /* (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED) */


#if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
    if (FLS_CH_QSPI == HwCh)
    {
        LldRetVal = Fls_IPW_SectorEraseQspiJobs(Asynch, SectorOffset, PhysicalSectorSize);
    }
#else
    /*Fix warning: unused variable*/
    (void)PhysicalSectorSize;
#endif /* (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */

    return LldRetVal;
}

/**
* @brief          IP wrapper sector write function.
* @details        Route the write job to appropriate low level IP function.
* @implements     Fls_IPW_SectorWrite_Activity
*/
Fls_LLDReturnType Fls_IPW_SectorWrite(const Fls_AddressType SectorOffset,
                                      const Fls_AddressType Length,
                                      const uint8 *JobDataSrcPtr,
                                      const boolean Asynch
                                     )
{
    Fls_LLDReturnType LldRetVal = FLASH_E_FAILED;
    Fls_HwChType HwCh;

    /* Get channel type(INTERNAL, QSPI,...) to determine the HW IP used(internal or external flash). */
    HwCh = (*(Fls_pConfigPtr->paHwCh))[Fls_u32JobSectorIt];

    /* Decide the IP used: internal flash or external QSPI */
#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
    if (FLS_CH_INTERN == HwCh)
    {
        LldRetVal = Fls_IPW_SectorWriteC40Jobs(SectorOffset, Length, JobDataSrcPtr, Asynch);
    }
#endif /* (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED) */


#if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
    if (FLS_CH_QSPI == HwCh)
    {
        LldRetVal = Fls_IPW_SectorWriteQspiJobs(SectorOffset, Length, JobDataSrcPtr, Asynch);
    }
#endif /* (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */

    return LldRetVal;
}

#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
static inline void Fls_IPW_LLDMainFunctionC40Jobs_CheckEraseStatus(C40_Ip_StatusType Status)
{
    if (STATUS_C40_IP_BUSY != Status)
    {
        /* Erase operation finished at IP level - successfully or with errors or timed out */
        /* Lock the sector */
        if (STATUS_C40_IP_SUCCESS != C40_Ip_SetLock(Fls_Ipw_xVirtualSectorInUse, (uint8)FLS_DOMAIN_ID))
        {
            /* Sector lock failed */
            Fls_eLLDJobResult = MEMIF_JOB_FAILED;
            (void)Det_ReportTransientFault((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_ERASE_FAILED);
        }
        else if (STATUS_C40_IP_SUCCESS != Status)
        {
            /* Sector lock was ok, but IP operation failed */
            Fls_eLLDJobResult = MEMIF_JOB_FAILED;
            (void)Det_ReportTransientFault((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_ERASE_FAILED);
            /* Report Runtime error when comparing is incorrect */
            if (STATUS_C40_IP_ERROR_BLANK_CHECK == Status)
            {
                (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_VERIFY_ERASE_FAILED);
            }

        #if (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON)
            /*check timeout after checking status fail*/
            if (STATUS_C40_IP_ERROR_TIMEOUT == Status)
            {
                (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_TIMEOUT);
            }
        #endif /* (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON) */
        }
        else
        {
            /* Assume that everything was ok */
            Fls_eLLDJob = FLASH_JOB_NONE;
            Fls_eLLDJobResult = MEMIF_JOB_OK;
            if ((Fls_u32JobAddrIt > ((*(Fls_pConfigPtr->paSectorEndAddr))[Fls_u32JobSectorIt])))
            {
                /* Move on to the next sector */
                Fls_u32JobSectorIt++;
            }
        }
    }
}


static inline void Fls_IPW_LLDMainFunctionC40Jobs_CheckWriteStatus(C40_Ip_StatusType Status)
{
    if (STATUS_C40_IP_BUSY != Status)
    {
        /* Write operation finished at IP level - successfully or with errors or timed out */
        if ((Fls_u32JobAddrIt > (*(Fls_pConfigPtr->paSectorEndAddr))[Fls_u32JobSectorIt]) || \
            (Fls_u32JobAddrIt > Fls_u32JobAddrEnd))
        {
            /* The write job has finished or the end of the sector was reached. Time to lock the sector */
            if (STATUS_C40_IP_SUCCESS != C40_Ip_SetLock(Fls_Ipw_xVirtualSectorInUse, (uint8)FLS_DOMAIN_ID))
            {
                /* Sector locking failed */
                Fls_eLLDJobResult = MEMIF_JOB_FAILED;
                /* An error will be reported */
                (void)Det_ReportTransientFault((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_WRITE_FAILED);
            }
            else if (STATUS_C40_IP_SUCCESS != Status)
            {
                /* Sector lock was ok, but IP operation failed */
                Fls_eLLDJobResult = MEMIF_JOB_FAILED;
                /* An error will be reported */
                (void)Det_ReportTransientFault((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_WRITE_FAILED);

            #if (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON)
                /*check timeout after checking status fail*/
                if (STATUS_C40_IP_ERROR_TIMEOUT == Status)
                {
                    (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_TIMEOUT);
                }
            #endif /* (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON) */
            }
            else
            {
                /* Everything was ok */
                Fls_eLLDJobResult = MEMIF_JOB_OK;
            }
        }
        else
        {
            if (STATUS_C40_IP_SUCCESS != Status)
            {
                /* Sector lock was ok, but IP operation failed */
                Fls_eLLDJobResult = MEMIF_JOB_FAILED;
                /* An error will be reported */
                (void)Det_ReportTransientFault((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_WRITE_FAILED);
                /* Report errors due to incorrect verifying writing */
                if (STATUS_C40_IP_ERROR_PROGRAM_VERIFY == Status)
                {
                    (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_VERIFY_WRITE_FAILED);
                }
            }
            else
            {
                /* Everything was ok */
                Fls_eLLDJobResult = MEMIF_JOB_OK;
            }
        }
    }
}

static void Fls_IPW_LLDMainFunctionC40Jobs(void)
{
    C40_Ip_StatusType ReturnCode = STATUS_C40_IP_ERROR;

    /* some hardware job (asynchronous) is pending */
    if (FLASH_JOB_ERASE == Fls_eLLDJob )
    {
    /* Decide the IP used: internal flash or external QSPI */
    #if (STD_ON == C40_MAIN_INTERFACE_ENABLED)
        ReturnCode = C40_Ip_MainInterfaceSectorEraseStatus();
    #endif

        Fls_IPW_LLDMainFunctionC40Jobs_CheckEraseStatus(ReturnCode);
    }
    /* Write hardware job (asynchronous) is pending */
    else if (FLASH_JOB_WRITE == Fls_eLLDJob )
    {
        #if (STD_ON == C40_MAIN_INTERFACE_ENABLED)
            ReturnCode = C40_Ip_MainInterfaceWriteStatus();
        #endif

        Fls_IPW_LLDMainFunctionC40Jobs_CheckWriteStatus(ReturnCode);
    }
    /* Write interleaved hardware job (asynchronous) is pending */
    else if (FLASH_JOB_NONE == Fls_eLLDJob)
    {
        /* Do nothing */
    }
    else
    {
        /* Do nothing - should not happen in Fully Trusted Environment;
           'else' clause added to fulfil MISRA Rule 14.10 */
    }
}


static inline Fls_LLDReturnType Fls_IPW_SectorEraseC40Jobs(boolean Asynch,
                                                           Fls_AddressType SectorOffset
                                                          )
{
    Fls_LLDReturnType LldRetVal = FLASH_E_FAILED;
    uint32 SectorStartAddress;
    C40_Ip_StatusType ReturnCode = STATUS_C40_IP_ERROR;

    /* Get the base address of the sector. */
    SectorStartAddress = (uint32)(*((*(Fls_pConfigPtr->pSectorList))[Fls_u32JobSectorIt])).pSectorStartAddressPtr;
    /* Get the physical sector number */
    Fls_Ipw_xVirtualSectorInUse = C40_Ip_GetSectorNumberFromAddress(SectorStartAddress + SectorOffset);
    /* Check sector lock */
    ReturnCode = C40_Ip_ClearLock(Fls_Ipw_xVirtualSectorInUse, (uint8)FLS_DOMAIN_ID);

    if (ReturnCode == STATUS_C40_IP_SUCCESS)
    {
        /* Set synch/Asynch at IP layer base on Asynch */
        C40_Ip_SetAsyncMode(Asynch);

#if (STD_ON == C40_MAIN_INTERFACE_ENABLED)
        ReturnCode = C40_Ip_MainInterfaceSectorErase(Fls_Ipw_xVirtualSectorInUse, (uint8)FLS_DOMAIN_ID);
#endif
    }

    if ((STATUS_C40_IP_SUCCESS == ReturnCode) && (FALSE == Asynch))
    {
        Fls_IPW_CallAccessCodeErase();

        /* check status of erase hardware */
    #if (STD_ON == C40_MAIN_INTERFACE_ENABLED)
        ReturnCode = C40_Ip_MainInterfaceSectorEraseStatus();
    #endif

        /* Assume that everything was ok */
        Fls_eLLDJobResult = MEMIF_JOB_OK;
        LldRetVal = FLASH_E_OK;

        /* The job has finished. Time to lock the sector */
        if (STATUS_C40_IP_SUCCESS != C40_Ip_SetLock(Fls_Ipw_xVirtualSectorInUse, (uint8)FLS_DOMAIN_ID))
        {
            /* Sector locking failed */
            Fls_eLLDJobResult = MEMIF_JOB_FAILED;
            LldRetVal = FLASH_E_FAILED;
        }

        if (STATUS_C40_IP_SUCCESS != ReturnCode)
        {
            /* IP operation failed */
            Fls_eLLDJobResult = MEMIF_JOB_FAILED;
            LldRetVal = Fls_IPW_TranslateReturnCode(ReturnCode);
        }
    }
    else
    {
        if (STATUS_C40_IP_SUCCESS != ReturnCode)
        {
            /* IP operation failed */
            Fls_eLLDJobResult = MEMIF_JOB_FAILED;
            LldRetVal = Fls_IPW_TranslateReturnCode(ReturnCode);
        }
        else
        {
            LldRetVal = FLASH_E_PENDING;
            Fls_eLLDJob = FLASH_JOB_ERASE;
            Fls_eLLDJobResult = MEMIF_JOB_PENDING;
        }
    }

#if ( (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON))
    /* Check timeout after perform erase */
    if (STATUS_C40_IP_ERROR_TIMEOUT == ReturnCode)
    {
        (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_TIMEOUT);
    }
#endif

    return LldRetVal;
}


static inline Fls_LLDReturnType Fls_IPW_SectorWriteC40Jobs(const Fls_AddressType SectorOffset,
                                                           const Fls_AddressType Length,
                                                           const uint8 *JobDataSrcPtr,
                                                           const boolean Asynch
                                                          )
{
    Fls_LLDReturnType LldRetVal = FLASH_E_FAILED;
    uint32 WriteStartAddress;
    C40_Ip_StatusType ReturnCode;

    /* Get the base address of the sector. */
    WriteStartAddress = (uint32)(*((*(Fls_pConfigPtr->pSectorList))[Fls_u32JobSectorIt])).pSectorStartAddressPtr;
    /* Add the offset */
    WriteStartAddress += SectorOffset;

    /* Get the physical sector number */
    Fls_Ipw_xVirtualSectorInUse = C40_Ip_GetSectorNumberFromAddress(WriteStartAddress);
    /* Check sector lock */
    ReturnCode = C40_Ip_ClearLock(Fls_Ipw_xVirtualSectorInUse, (uint8)FLS_DOMAIN_ID);

    if (STATUS_C40_IP_SUCCESS == ReturnCode)
    {
        /* Set synch/Asynch at IP layer base on Asynch */
        C40_Ip_SetAsyncMode(Asynch);

#if (STD_ON == C40_MAIN_INTERFACE_ENABLED)
        ReturnCode = C40_Ip_MainInterfaceWrite(WriteStartAddress, Length, JobDataSrcPtr, (uint8)FLS_DOMAIN_ID);
#endif
    }

    /* On sync */
    if ((STATUS_C40_IP_SUCCESS == ReturnCode) && (FALSE == Asynch))
    {
        Fls_IPW_CallAccessCodeWrite();

        /* Check status of write hardware */
    #if (STD_ON == C40_MAIN_INTERFACE_ENABLED)
        ReturnCode = C40_Ip_MainInterfaceWriteStatus();
    #endif

        /* Assume that everything was ok */
        LldRetVal = FLASH_E_OK;

        /* The job has finished. Time to lock the sector */
        if ( (Fls_u32JobAddrIt > (*(Fls_pConfigPtr->paSectorEndAddr))[Fls_u32JobSectorIt]) ||
             (Fls_u32JobAddrIt > Fls_u32JobAddrEnd) )
        {
            if (STATUS_C40_IP_SUCCESS != C40_Ip_SetLock(Fls_Ipw_xVirtualSectorInUse, (uint8)FLS_DOMAIN_ID))
            {
                /* Sector locking failed */
                Fls_eLLDJobResult = MEMIF_JOB_FAILED;
                LldRetVal = FLASH_E_FAILED;
            }
        }

        if (STATUS_C40_IP_SUCCESS != ReturnCode)
        {
            /* IP operation failed */
            Fls_eLLDJobResult = MEMIF_JOB_FAILED;
            LldRetVal = Fls_IPW_TranslateReturnCode(ReturnCode);
        }
    }
    /* If failed or Async */
    else
    {
        if (STATUS_C40_IP_SUCCESS != ReturnCode)
        {
            /* IP operation failed */
            Fls_eLLDJobResult = MEMIF_JOB_FAILED;
            LldRetVal = Fls_IPW_TranslateReturnCode(ReturnCode);
        }
        else
        {
            LldRetVal = FLASH_E_PENDING;
            Fls_eLLDJob = FLASH_JOB_WRITE;
            Fls_eLLDJobResult = MEMIF_JOB_PENDING;
        }
    }

#if ( (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON))
    /* Check timeout after performing write */
    if (STATUS_C40_IP_ERROR_TIMEOUT == ReturnCode)
    {
        (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_TIMEOUT);
    }
#endif

    return LldRetVal;
}

static inline void Fls_IPW_CallAccessCodeWrite(void)
{
#if ( STD_ON == FLS_AC_LOAD_ON_JOB_START )
    if (TRUE == Fls_bACloaded)
    {
        /* Sync mode - wait for IP to finish */
        FLASH_AC_CALL(Fls_pConfigPtr->acWritePtr, Fls_AcWritePtrType)(Fls_pConfigPtr->acCallBackPtr);
    }
    else
#endif /* STD_ON == FLS_AC_LOAD_ON_JOB_START */
    {
        C40_Ip_AccessCode(Fls_pConfigPtr->acCallBackPtr);
    }
}


static inline void Fls_IPW_CallAccessCodeErase(void)
{
#if ( STD_ON == FLS_AC_LOAD_ON_JOB_START )
    if (TRUE == Fls_bACloaded)
    {
        /* Sync mode - wait for IP to finish */
        FLASH_AC_CALL(Fls_pConfigPtr->acErasePtr, Fls_AcErasePtrType)(Fls_pConfigPtr->acCallBackPtr);
    }
    else
#endif /* STD_ON == FLS_AC_LOAD_ON_JOB_START */
    {
        C40_Ip_AccessCode(Fls_pConfigPtr->acCallBackPtr);
    }
}


static Fls_LLDReturnType Fls_IPW_TranslateReturnCode(C40_Ip_StatusType ReturnCode)
{
    Fls_LLDReturnType LldRetVal;

    /* Translate the return code from IPV to HLD */
    switch (ReturnCode)
    {
        case STATUS_C40_IP_SUCCESS:
            /* Operation succeeded */
            LldRetVal = FLASH_E_OK;
            break;

        case STATUS_C40_IP_BUSY:
            /* Operation is pending */
            LldRetVal = FLASH_E_PENDING;
            break;

        case STATUS_C40_IP_ERROR_BLANK_CHECK:
            /* Content of flash memory doesn't match with erased value */
            LldRetVal = FLASH_E_BLOCK_INCONSISTENT;
            (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_VERIFY_ERASE_FAILED);
            break;

        case STATUS_C40_IP_ERROR_PROGRAM_VERIFY:
            /* Content of flash memory doesn't match with data buffer */
            LldRetVal = FLASH_E_BLOCK_INCONSISTENT;
            (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_VERIFY_WRITE_FAILED);
            break;

        default:
            /* Operation failed due to hardware error */
            LldRetVal = FLASH_E_FAILED;
            break;
    }

    return LldRetVal;
}

#if ( (STD_ON == FLS_ECC_CHECK) || (STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS) )
Fls_CompHandlerReturnType Fls_IPW_DsiHandler(const Fls_ExceptionDetailsType *ExceptionDetailsPtr)
{
    /* Call the handler to confirm ECC exception */
    return C40_Ip_DsiHandler(ExceptionDetailsPtr);
}
#endif /* (STD_ON == FLS_ECC_CHECK) || (STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS) */


#if (STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS)
/**
 * Function Name : Fls_IPW_CopyBlock
 * Description   : Call the FlsReadFunctionCallout where user can schedule a task which performs the copy operation (Fls_ReadEachBlock)
 *
 */
static C40_Ip_StatusType Fls_IPW_CopyBlock(uint32 LogicalAddress,
                                           uint32 Length,
                                           uint8 *DestAddressPtr,
                                           const uint8 *SourceAddressPtr
                                          )
{
    /* Prepare for the copy operation */
    Fls_Ipw_xCopyDescr.ReadStartAddress = LogicalAddress;
    Fls_Ipw_xCopyDescr.Length           = Length;
    Fls_Ipw_xCopyDescr.DataDest         = DestAddressPtr;
    Fls_Ipw_xCopyDescr.DataSource       = SourceAddressPtr;
    Fls_Ipw_xCopyDescr.Status           = STATUS_C40_IP_ERROR;

    /* This callout can schedule a task which performs the copy operation. The amount of data to copy (Length) can be arbitrarily long.
     * If the copy fails because of the expected exception, the task is forcibly terminated from the ProtectionHook().
     */
    Fls_pConfigPtr->FlsReadFunctionCallout();

    return Fls_Ipw_xCopyDescr.Status;
}

/**
 * Function Name : Fls_IPW_ReadEachBlock
 * Description   : Performs the actual copy operation.
 */
void Fls_IPW_ReadEachBlock(void)
{
    if (NULL_PTR != Fls_Ipw_xCopyDescr.DataDest)
    {
        /* Read operation */
        Fls_Ipw_xCopyDescr.Status = C40_Ip_Read(Fls_Ipw_xCopyDescr.ReadStartAddress, Fls_Ipw_xCopyDescr.Length, Fls_Ipw_xCopyDescr.DataDest);
    }
    else
    {
        /* Compare operation */
        Fls_Ipw_xCopyDescr.Status = C40_Ip_Compare(Fls_Ipw_xCopyDescr.ReadStartAddress, Fls_Ipw_xCopyDescr.Length, Fls_Ipw_xCopyDescr.DataSource);
    }
}
#endif /* STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS */

#endif /* (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED) */

#if ( STD_ON == FLS_AC_LOAD_ON_JOB_START )
Fls_BlockNumberOfSectorType Fls_IPW_GetBlockNumberFromAddress(Fls_AddressType TargetAddress)
{
    /* Get block number from address */
    return (Fls_BlockNumberOfSectorType)C40_Ip_GetBlockNumberFromAddress((uint32)TargetAddress);
}
#endif /* STD_ON == FLS_AC_LOAD_ON_JOB_START */

#define FLS_STOP_SEC_CODE
#include "Fls_MemMap.h"


#ifdef __cplusplus
}
#endif

/** @} */
