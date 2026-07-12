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
*   @file Fls.c
*
*   @addtogroup FLS FLS Driver
*   @{
*/

/* implements Fls.c_Artifact */

#ifdef __cplusplus
extern "C"{
#endif

 /* Compiler warning fls_c_REF_CW_01: explicit cast discards volatile qualifier.
   The cast is explicit, intended and the casted value is treated properly.
   Sizes of the pointers and integral types for all the supported platforms/compilers are well known
   and the volatile keyword is not needed when using the pointer value as a flash address counter,
   as the pointer value is not updated in the DSI interrupt context or by other hardware means.
 */


/*==================================================================================================
 *                                        INCLUDE FILES
 * 1) system and project includes
 * 2) needed interfaces from external units
 * 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Fls.h"
#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
#include "C40_Ip.h"
#endif
#if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
#include "Qspi_Ip.h"
#endif
#if (STD_ON == FLS_AC_LOAD_ON_JOB_START)
#if (STD_ON == FLS_CLEAN_CACHE_AFTER_LOAD_AC)
#include "Cache_Ip.h"
#endif /* FLS_CLEAN_CACHE_AFTER_LOAD_AC */
#endif /* FLS_AC_LOAD_ON_JOB_START */
#include "Det.h"
#include "SchM_Fls.h"
#include "StandardTypes.h"
#include "Fls_IPW.h"


/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLS_VENDOR_ID_C                      43
#define FLS_AR_RELEASE_MAJOR_VERSION_C       4
#define FLS_AR_RELEASE_MINOR_VERSION_C       4
#define FLS_AR_RELEASE_REVISION_VERSION_C    0
#define FLS_SW_MAJOR_VERSION_C               2
#define FLS_SW_MINOR_VERSION_C               0
#define FLS_SW_PATCH_VERSION_C               1


/*==================================================================================================
 *                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and Fls header file are of the same vendor */
#if (FLS_VENDOR_ID_C != FLS_VENDOR_ID)
    #error "Fls.c and Fls.h have different vendor ids"
#endif
/* Check if current file and Fls header file are of the same Autosar version */
#if ((FLS_AR_RELEASE_MAJOR_VERSION_C    != FLS_AR_RELEASE_MAJOR_VERSION) || \
     (FLS_AR_RELEASE_MINOR_VERSION_C    != FLS_AR_RELEASE_MINOR_VERSION) || \
     (FLS_AR_RELEASE_REVISION_VERSION_C != FLS_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Fls.c and Fls.h are different"
#endif
/* Check if current file and Fls header file are of the same Software version */
#if ((FLS_SW_MAJOR_VERSION_C != FLS_SW_MAJOR_VERSION) || \
     (FLS_SW_MINOR_VERSION_C != FLS_SW_MINOR_VERSION) || \
     (FLS_SW_PATCH_VERSION_C != FLS_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Fls.c and Fls.h are different"
#endif

#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
    /* Check if current file and C40_IP.h header file are of the same vendor */
    #if (FLS_VENDOR_ID_C != C40_IP_VENDOR_ID_H)
        #error "Fls.c and C40_IP.h have different vendor ids"
    #endif
    /* Check if current file and C40_IP.h header file are of the same Autosar version */
    #if ((FLS_AR_RELEASE_MAJOR_VERSION_C    != C40_IP_AR_RELEASE_MAJOR_VERSION_H) || \
         (FLS_AR_RELEASE_MINOR_VERSION_C    != C40_IP_AR_RELEASE_MINOR_VERSION_H) || \
         (FLS_AR_RELEASE_REVISION_VERSION_C != C40_IP_AR_RELEASE_REVISION_VERSION_H) \
        )
        #error "AutoSar Version Numbers of Fls.c and C40_IP.h are different"
    #endif
    /* Check if current file and C40_IP.h header file are of the same Software version */
    #if ((FLS_SW_MAJOR_VERSION_C != C40_IP_SW_MAJOR_VERSION_H) || \
         (FLS_SW_MINOR_VERSION_C != C40_IP_SW_MINOR_VERSION_H) || \
         (FLS_SW_PATCH_VERSION_C != C40_IP_SW_PATCH_VERSION_H) \
        )
        #error "Software Version Numbers of Fls.c and C40_IP.h are different"
    #endif
#endif /* STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED */

#if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
    /* Check if current file and Qspi_Ip.h header file are of the same vendor */
    #if (FLS_VENDOR_ID_C != QSPI_IP_VENDOR_ID_H)
        #error "Fls.c and Qspi_Ip.h have different vendor ids"
    #endif
    /* Check if current file and Qspi_Ip.h header file are of the same Autosar version */
    #if ((FLS_AR_RELEASE_MAJOR_VERSION_C    != QSPI_IP_AR_RELEASE_MAJOR_VERSION_H) || \
         (FLS_AR_RELEASE_MINOR_VERSION_C    != QSPI_IP_AR_RELEASE_MINOR_VERSION_H) || \
         (FLS_AR_RELEASE_REVISION_VERSION_C != QSPI_IP_AR_RELEASE_REVISION_VERSION_H) \
        )
        #error "AutoSar Version Numbers of Fls.c and Qspi_Ip.h are different"
    #endif
    /* Check if current file and Qspi_Ip.h header file are of the same Software version */
    #if ((FLS_SW_MAJOR_VERSION_C != QSPI_IP_SW_MAJOR_VERSION_H) || \
         (FLS_SW_MINOR_VERSION_C != QSPI_IP_SW_MINOR_VERSION_H) || \
         (FLS_SW_PATCH_VERSION_C != QSPI_IP_SW_PATCH_VERSION_H) \
        )
        #error "Software Version Numbers of Fls.c and Qspi_Ip.h are different"
    #endif
#endif /* STD_ON == FLS_QSPI_SECTORS_CONFIGURED */

#if (STD_ON == FLS_AC_LOAD_ON_JOB_START)
    #if (STD_ON == FLS_CLEAN_CACHE_AFTER_LOAD_AC )
        #ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
            /* Check if current file and Cache_Ip.h header file are of the same Autosar version */
            #if ((FLS_AR_RELEASE_MAJOR_VERSION_C != CACHE_IP_AR_RELEASE_MAJOR_VERSION) || \
                 (FLS_AR_RELEASE_MINOR_VERSION_C != CACHE_IP_AR_RELEASE_MINOR_VERSION) \
                )
                #error "AutoSar Version Numbers of Fls.c and Cache_Ip.h are different"
            #endif
        #endif
    #endif /* STD_ON == FLS_CLEAN_CACHE_AFTER_LOAD_AC  */
#endif /* STD_ON == FLS_AC_LOAD_ON_JOB_START */

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and Det header file are of the same Autosar version */
    #if ((FLS_AR_RELEASE_MAJOR_VERSION_C != DET_AR_RELEASE_MAJOR_VERSION) || \
         (FLS_AR_RELEASE_MINOR_VERSION_C != DET_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AutoSar Version Numbers of Fls.c and Det.h are different"
    #endif
    /* Check if current file and SchM_Fls header file are of the same Autosar version */
    #if ((FLS_AR_RELEASE_MAJOR_VERSION_C != SCHM_FLS_AR_RELEASE_MAJOR_VERSION) || \
         (FLS_AR_RELEASE_MINOR_VERSION_C != SCHM_FLS_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AutoSar Version Numbers of Fls.c and SchM_Fls.h are different"
    #endif
    /* Check if current file and StandardTypes header file are of the same Autosar version */
    #if ((FLS_AR_RELEASE_MAJOR_VERSION_C != STD_AR_RELEASE_MAJOR_VERSION) || \
         (FLS_AR_RELEASE_MINOR_VERSION_C != STD_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AutoSar Version Numbers of Fls.c and StandardTypes.h are different"
    #endif
#endif

/* Check if current file and Fls_IPW.h header file are of the same vendor */
#if (FLS_VENDOR_ID_C != FLS_IPW_VENDOR_ID_H)
    #error "Fls.c and Fls_IPW.h have different vendor ids"
#endif
/* Check if current file and Dem header file are of the same Autosar version */
#if ((FLS_AR_RELEASE_MAJOR_VERSION_C    != FLS_IPW_AR_RELEASE_MAJOR_VERSION_H) || \
     (FLS_AR_RELEASE_MINOR_VERSION_C    != FLS_IPW_AR_RELEASE_MINOR_VERSION_H) || \
     (FLS_AR_RELEASE_REVISION_VERSION_C != FLS_IPW_AR_RELEASE_REVISION_VERSION_H) \
    )
    #error "AutoSar Version Numbers of Fls.c and Fls_IPW.h are different"
#endif
/* Check if current file and Fls_IPW.h header file are of the same Software version */
#if ((FLS_SW_MAJOR_VERSION_C != FLS_IPW_SW_MAJOR_VERSION_H) || \
     (FLS_SW_MINOR_VERSION_C != FLS_IPW_SW_MINOR_VERSION_H) || \
     (FLS_SW_PATCH_VERSION_C != FLS_IPW_SW_PATCH_VERSION_H) \
    )
    #error "Software Version Numbers of Fls.c and Fls_IPW.h are different"
#endif


/*==================================================================================================
 *                                      GLOBAL VARIABLES
==================================================================================================*/

#define FLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fls_MemMap.h"
/**
 * @brief Logical address of data block currently processed by Fls_MainFunction
 */
Fls_AddressType Fls_u32JobAddrIt;
/**
 * @brief Last logical address to be processed by a job
 */
Fls_AddressType Fls_u32JobAddrEnd;
/**
 * @brief Index of flash sector currently processed by a job
 * @details Used by all types of job
 */
volatile Fls_SectorIndexType Fls_u32JobSectorIt;

/**
 * @brief Type of currently executed job (erase, write, read, or compare)
 */
static Fls_JobType Fls_eJob;                             /* implicit zero initialization: FLS_JOB_ERASE */
/**
 * @brief Index of last flash sector by current job
 * @details Used to check status of all external flash chips before start jobs
 * or is the last sector in Erease job
 */
Fls_SectorIndexType Fls_u32JobSectorEnd;
/**
    @brief Result of last flash hardware job
 */
volatile MemIf_JobResultType Fls_eLLDJobResult;          /* implicit zero initialization: MEMIF_JOB_OK */
/**
    @brief Type of current flash hardware job - used for asynchronous operating mode.
 */
Fls_LLDJobType Fls_eLLDJob;    /* implicit zero initialization: FLASH_JOB_NONE */
/**
 * @brief Pointer to current flash module configuration set
 */
const Fls_ConfigType * Fls_pConfigPtr;                   /* implicit zero initialization: NULL_PTR */

#if (FLS_MCORE_ENABLED == STD_ON)
 /**
 * @brief XRDC assigned DomainID
 */
static uint8 Fls_u8DomainID;

 /**
 * @brief job status in multicore feature
 */
static Fls_MCoreHwJobStatusType Fls_MCoreHwJobStatus;    /* implicit zero initialization: FLS_MCORE_HW_JOB_IDLE */
#endif /* #if (FLS_MCORE_ENABLED == STD_ON)  */

#define FLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fls_MemMap.h"


/*==================================================================================================
 *                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/
/**
    @brief fill pattern used to clear write and erase access code in RAM
 */
#if ( STD_ON == FLS_AC_LOAD_ON_JOB_START )
#define FLS_AC_UNLOAD_PATTERN 0xffffffffU


#define FLS_START_SEC_VAR_CLEARED_BOOLEAN
#include "Fls_MemMap.h"
/* @brief verify that AC loaded or not */
boolean Fls_bACloaded;                                   /* implicit zero initialization: FALSE */

#define FLS_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Fls_MemMap.h"
#endif

/**
    @brief Calculate total flash size in bytes
 */
#if ( FLS_DEV_ERROR_DETECT == STD_ON )
#define FLS_TOTAL_SIZE    ((*(Fls_pConfigPtr->paSectorEndAddr))[Fls_pConfigPtr->u32SectorCount - 1U] + 1U)
#endif


/*==================================================================================================
                                       LOCAL VARIABLES
==================================================================================================*/
#define FLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fls_MemMap.h"

/**
 * @brief Maximum number of bytes to read or compare in one cycle of Fls_MainFunction
 */
static Fls_LengthType Fls_u32MaxRead;
/**
 * @brief Maximum number of bytes to write in one cycle of Fls_MainFunction
 */
static Fls_LengthType Fls_u32MaxWrite;
/**
 * @brief Result of last flash module job
 * @implements Fls_eJobResult_Object
 */
static MemIf_JobResultType Fls_eJobResult;

#define FLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fls_MemMap.h"

#define FLS_START_SEC_VAR_CLEARED_8
#include "Fls_MemMap.h"
/**
 * @brief Pointer to current position in source data buffer
 * @details Used by both write and compare jobs
 */
static const uint8 * Fls_pJobDataSrcPtr;
/**
 * @brief Pointer to current position in target data buffer
 * @details Used only by read job
 */
static uint8 * Fls_pJobDataDestPtr;
/**
 * @brief Indicates that new job has been accepted
 * @details Used by all types of job
 */
static uint8 Fls_u8JobStart;


#define FLS_STOP_SEC_VAR_CLEARED_8
#include "Fls_MemMap.h"

#define FLS_START_SEC_VAR_CLEARED_32
#include "Fls_MemMap.h"

#if (FLS_CHECK_CFG_CRC == STD_ON)
/**
 * @brief Pointer to current flash module configuration set
 * @implements Fls_u32AccCRCremainder_Object
 */
static uint32 Fls_u32AccCRCremainder;
#endif

#if (FLS_MCORE_ENABLED == STD_ON)
#if (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON)
/**
 * @brief Timeout counter used in multicore access requests.
 */
static uint32 Fls_u32MCoreTimeout_ElapsedTicks;
static uint32 Fls_u32MCoreTimeout_TimeoutTicks;
static uint32 Fls_u32MCoreTimeout_CurrentTicks;
#endif /* #if (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON) */
#endif /* #if (FLS_MCORE_ENABLED == STD_ON)*/
#define FLS_STOP_SEC_VAR_CLEARED_32
#include "Fls_MemMap.h"

/*==================================================================================================
                                       LOCAL CONSTANTS
==================================================================================================*/

#if ( STD_ON == FLS_AC_LOAD_ON_JOB_START )
#define FLS_START_SEC_CONST_32
#include "Fls_MemMap.h"

/* Note: Linker scripts symbol declarations must be accessed by the use of the '&' operators */
/* Start Position and Size of erase access code in flash */
extern const uint32 Fls_ACEraseRomStart;
extern const uint32 Fls_ACEraseSize;

/* Start Position and Size of write access code in flash */
extern const uint32 Fls_ACWriteRomStart;
extern const uint32 Fls_ACWriteSize;

#define FLS_STOP_SEC_CONST_32
#include "Fls_MemMap.h"

#define FLS_START_SEC_VAR_CLEARED_8
#include "Fls_MemMap.h"

/* Block of ACErase function address */
static Fls_BlockNumberOfSectorType  FLs_u8blockAcErase;
/* Block of ACWrite function address */
static Fls_BlockNumberOfSectorType  FLs_u8blockAcWrite;

#define FLS_STOP_SEC_VAR_CLEARED_8
#include "Fls_MemMap.h"

#endif /* STD_ON == FLS_AC_LOAD_ON_JOB_START */


/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#define FLS_START_SEC_CODE
#include "Fls_MemMap.h"
static Fls_SectorIndexType Fls_GetSectorIndexByAddr(const Fls_AddressType TargetAddress);
static Fls_AddressType Fls_GetSectorStartAddr(const Fls_SectorIndexType SectorIndex);
#if ( FLS_DEV_ERROR_DETECT == STD_ON )
static boolean Fls_IsAddrPageAligned(const Fls_AddressType TargetAddress);
static boolean Fls_IsAddrPageStartAligned(const Fls_AddressType TargetAddress);
static boolean Fls_IsAddrPageEndAligned(const Fls_AddressType TargetAddress);
static boolean Fls_IsAddrSectorStartAligned(const Fls_AddressType TargetAddress);
static boolean Fls_IsAddrSectorEndAligned(const Fls_AddressType TargetAddress);
static Std_ReturnType Fls_CheckInputParamReadCompareJob(Fls_AddressType SourceAddress,
                                                        const uint8 * TargetAddressPtr,
                                                        Fls_LengthType Length,
                                                        uint8 ApiId
                                                       );
#if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
static boolean Fls_IsAddrWordAligned(const Fls_AddressType TargetAddress);
#endif /* (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */
#endif /* #if ( FLS_DEV_ERROR_DETECT == STD_ON ) */
#if ( STD_ON == FLS_AC_LOAD_ON_JOB_START )
static void Fls_LoadAc(const Fls_JobType Job);
static void Fls_UnloadAc(const Fls_JobType Job);
static inline void Fls_CheckLoadAc(Fls_BlockNumberOfSectorType BlockAc,
                                   Fls_AddressType SectorOffset
                                  );
static inline void Fls_CheckUnLoadAc(void);
#endif

#if (FLS_CHECK_CFG_CRC == STD_ON)
static void Fls_AccumulateCRC(uint32 InputData);
static void Fls_ResetCRC(void);
static uint32 Fls_FinalizeCRC(void);
static void Fls_UpdateCRCreminder(Fls_CrcDataSizeType DataSize);

#if (FLS_QSPI_SECTORS_CONFIGURED == STD_ON)
static void Fls_AccumulateCRC_Qspi_dllSettings(const Qspi_Ip_ControllerConfigType *QspiUnitCfg);
static void Fls_AccumulateCRC_Qspi_paFlashCfg(void);
static void Fls_AccumulateCRC_Qspi_paQspiUnitCfg(void);
static void Fls_AccumulateCRC_Qspi_eraseSettings(const Qspi_Ip_EraseConfigType *EraseSettings);
static void Fls_AccumulateCRC_Qspi_initConfiguration(const Qspi_Ip_InitConfigType *InitConfiguration);
#endif /*#if (FLS_QSPI_SECTORS_CONFIGURED == STD_ON)*/

static Fls_CrcType Fls_CalcCfgCRC(void);
#endif /* #if (FLS_CHECK_CFG_CRC == STD_ON) */

static boolean Fls_CheckCfgCRC(void);

static MemIf_JobResultType Fls_DoJobErase(void);
static Fls_AddressType Fls_CalcMaxTransferAddrEnd(const Fls_LengthType MaxTransfer);
static Fls_AddressType Fls_CalcSectorTransferOffset(void);
static Fls_AddressType Fls_CalcSectorTransferLength(const Fls_AddressType MaxTransferEndAddr);
static Fls_LLDReturnType Fls_DoJobWrite(const Fls_AddressType SectorOffset,
                                        const Fls_AddressType Length
                                       );

static Fls_LLDReturnType Fls_ProcessJobDataTransfer(const Fls_AddressType SectorOffset,
                                                    const Fls_AddressType Length
                                                   );
static MemIf_JobResultType Fls_DoJobDataTransfer(const Fls_LengthType MaxTransfer);
static MemIf_JobResultType Fls_ProcessRequestedJobs(void);

#if (FLS_MCORE_ENABLED == STD_ON)
static Std_ReturnType Fls_MCoreInitSema4sLock(void);
static Std_ReturnType Fls_MCoreJobSema4Release(void);
static Fls_MCoreReqReturnType Fls_ReqMCore(void);
static inline Fls_MCoreReqReturnType Fls_MainFunction_ReqMCore(MemIf_JobResultType *WorkResult);
static void Fls_MainFunction_ReleaseMCore(MemIf_JobResultType *WorkResult);
#if (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON)
static Std_ReturnType Fls_MCoreCheckTimeout(void);
static void Fls_MCoreNotifyTimeout(void);
#endif /* (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON) */
#endif /* (FLS_MCORE_ENABLED == STD_ON) */

static MemIf_JobResultType Fls_LLDGetJobResult(void);
static void Fls_LLDClrJobResult(void);

static inline void Fls_MainFunction_CheckJobResult(MemIf_JobResultType WorkResult);
#define FLS_STOP_SEC_CODE
#include "Fls_MemMap.h"


/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/
#define FLS_START_SEC_CODE
#include "Fls_MemMap.h"

/**
 * @brief        Maps TargetAddress to sector index
 *
 * @details      Calculate index of logical sector TargetAddress belongs to
 *
 * @param[in]    TargetAddress Target address in flash memory
 *
 * @return       Zero based index of logical sector
 * @retval       0 .. (Fls_pConfigPtr->u32SectorCount - 1)
 *
 * @pre          The module must be initialized
 * @pre          TargetAddress must be in configured flash boundary
 *
 */
static Fls_SectorIndexType Fls_GetSectorIndexByAddr(const Fls_AddressType TargetAddress)
{
    Fls_SectorIndexType SectorIndex;

    for (SectorIndex = 0U; SectorIndex < Fls_pConfigPtr->u32SectorCount; SectorIndex++)
    {
        if ( TargetAddress <= (*(Fls_pConfigPtr->paSectorEndAddr))[SectorIndex] )
        {
            /* TargetAddress belongs to sector with index SectorIndex */
            break;
        }
        else
        {
            /* Check next sector */
        }
    }

    return SectorIndex;
}

/**
 * @brief        Return start address of given sector
 *
 * @details      Calculate start address (in linear space) of logical sector
 *               specified by the SectorIndex parameter
 *
 * @param[in]    SectorIndex Index of logical sector
 *
 * @return       Start address of SectorIndex sector
 * @retval       0 .. (FLS_TOTAL_SIZE - 1)
 *
 * @pre          The module must be initialized
 * @pre          SectorIndex must be in range 0 .. (Fls_pConfigPtr->u32SectorCount - 1)
 *
 */
static Fls_AddressType Fls_GetSectorStartAddr(const Fls_SectorIndexType SectorIndex)
{
    Fls_AddressType TargetAddress;

    if (0U != SectorIndex)
    {
        TargetAddress = (*(Fls_pConfigPtr->paSectorEndAddr))[SectorIndex - 1U] + 1U;
    }
    else
    {
        /* First sector starts with address 0 */
        TargetAddress = 0U;
    }

    return TargetAddress;
}

/**
 * @brief        Check whether TargetAddress is page aligned
 *
 * @details      Check whether TargetAddress is integer multiple of Flash Page Size
 *
 * @param[in]    TargetAddress Target address in flash memory
 *
 * @return       boolean
 * @retval       TRUE TargetAddress is page aligned
 * @retval       FALSE TargetAddress is not page aligned
 *
 */
#if ( FLS_DEV_ERROR_DETECT == STD_ON )
static boolean Fls_IsAddrPageAligned(const Fls_AddressType TargetAddress)
{
    boolean RetVal;

    /* Aligned to Double Word or Word depending if CODE/DATA Flash or DFO */
    if (0U == (TargetAddress % (*(Fls_pConfigPtr->paSectorPageSize))[Fls_GetSectorIndexByAddr(TargetAddress)]))
    {
        RetVal = (boolean)TRUE;
    }
    else
    {
        /* TargetAddress is not page aligned */
        RetVal = (boolean)FALSE;
    }

    return RetVal;
}

/**
 * @brief        Check whether TargetAddress is page start aligned
 *
 * @details      Check whether TargetAddress is in range and aligned to first
 *               byte of flash page
 *
 * @param[in]    TargetAddress Target address in flash memory
 *
 * @return       boolean
 * @retval       TRUE TargetAddress is in range and aligned to start of the page
 * @retval       FALSE TargetAddress is not in range or aligned to start of the page
 *
 * @pre          The module must be initialized
 *
 */
static boolean Fls_IsAddrPageStartAligned(const Fls_AddressType TargetAddress)
{
    boolean RetVal;

    if (( TargetAddress < (Fls_AddressType)FLS_TOTAL_SIZE ) &&
        ( (boolean)TRUE == Fls_IsAddrPageAligned( TargetAddress ) ) \
       )
    {
        RetVal = (boolean)TRUE;
    }
    else
    {
        /* TargetAddress is not in range or aligned to start of the page */
        RetVal = (boolean)FALSE;
    }

    return RetVal;
}

/**
 * @brief        Check whether TargetAddress is page start aligned
 *
 * @details      Check whether Length is in range and aligned to
 *               flash page
 *
 * @param[in]    TargetAddress Target address in flash memory
 * @param[in]    Length size of current write
 *
 * @return       boolean
 * @retval       TRUE TargetAddress is in range and aligned to end of the page
 * @retval       FALSE TargetAddress is not in range or aligned to end of the page
 *
 * @pre          The module must be initialized
 *
 */
static boolean Fls_IsAddrPageEndAligned(const Fls_AddressType TargetAddress)
{
    boolean RetVal = (boolean)FALSE;
    Fls_LengthType SectorOffset;

    /* Calculate offset in the last sector of current operation */
    SectorOffset = TargetAddress - Fls_GetSectorStartAddr(Fls_GetSectorIndexByAddr(TargetAddress));

    if (((TargetAddress - 1U) < (Fls_AddressType)FLS_TOTAL_SIZE ) &&
       (0U == (SectorOffset % (*(Fls_pConfigPtr->paSectorPageSize))[Fls_GetSectorIndexByAddr(TargetAddress)]))
       )
    {
        RetVal = (boolean)TRUE;
    }
    else
    {
        /* TargetAddress is not in range or aligned to end of the page */
    }

    return RetVal;
}


/**
 * @brief        Check whether TargetAddress is sector start aligned
 *
 * @details      Check whether TargetAddress is in range and aligned to first
 *               byte of flash sector
 *
 * @param[in]    TargetAddress Target address in flash memory
 *
 * @return       boolean
 * @retval       TRUE TargetAddress is in range and aligned to start of the sector
 * @retval       FALSE TargetAddress is not in range or not aligned to
 *               start of the sector
 *
 * @pre          The module must be initialized
 *
 */
static boolean Fls_IsAddrSectorStartAligned(const Fls_AddressType TargetAddress)
{
    boolean RetVal = (boolean)FALSE;
    Fls_SectorIndexType SectorIndex;
    Fls_LengthType SectorSize;

    if (TargetAddress < FLS_TOTAL_SIZE)
    {
        SectorIndex = Fls_GetSectorIndexByAddr( TargetAddress );
        SectorSize = (*(Fls_pConfigPtr->paSectorSize))[SectorIndex];
        if ( ( 0U == ((TargetAddress - Fls_GetSectorStartAddr( SectorIndex)) % SectorSize)))
        {
            RetVal = (boolean)TRUE;
        }
        else
        {
            /* TargetAddress is not in range or
                not aligned to start of the sector */
        }
    }

    return RetVal;
}


/**
 * @brief        Check whether TargetAddress is sector end aligned
 *
 * @details      Check whether TargetAddress is in range and aligned to last
 *               byte of flash sector
 *
 * @param[in]    TargetAddress Target address in flash memory
 *
 * @return       boolean
 * @retval       TRUE TargetAddress is in range and aligned to end of the sector
 * @retval       FALSE TargetAddress is not in range or not aligned to
 *               end of the sector
 *
 * @pre          The module must be initialized
 *
 */
static boolean Fls_IsAddrSectorEndAligned( const Fls_AddressType TargetAddress )
{
    boolean RetVal = (boolean)FALSE;
    Fls_SectorIndexType SectorIndex;
    Fls_LengthType SectorSize;
    Fls_AddressType EndAddress;

    if (TargetAddress < FLS_TOTAL_SIZE)
    {
        SectorIndex = Fls_GetSectorIndexByAddr( TargetAddress );
        SectorSize = (*(Fls_pConfigPtr->paSectorSize))[SectorIndex];
        EndAddress = (*(Fls_pConfigPtr->paSectorEndAddr))[SectorIndex];

        if ( 0U == (( EndAddress - TargetAddress ) %  SectorSize ) )
        {
            RetVal =  (boolean)TRUE;
        }
        else
        {
            /* TargetAddress is not aligned to end of the sector */
        }
    }

    return RetVal;
}

static Std_ReturnType Fls_CheckInputParamReadCompareJob(Fls_AddressType SourceAddress,
                                                        const uint8 * TargetAddressPtr,
                                                        Fls_LengthType Length,
                                                        uint8 ApiId
                                                       )
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;
    boolean CheckInputTemp = FALSE;

    if (NULL_PTR == Fls_pConfigPtr)
    {
        /* Report Error if Fls_pConfigPtr = NULL_PTR  */
        (void) Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, ApiId, FLS_E_UNINIT );
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    else if (SourceAddress >= FLS_TOTAL_SIZE)
    {
        /* Report Error if SourceAddress is invalid */
        (void) Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, ApiId, FLS_E_PARAM_ADDRESS );
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    else if (( 0U == Length ) || ((SourceAddress + Length) > FLS_TOTAL_SIZE ))
    {
        /* Report Error if Length is invalid */
        (void) Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, ApiId, FLS_E_PARAM_LENGTH );
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    else
    {
        CheckInputTemp = TRUE;
    }

    if (TRUE == CheckInputTemp)
    {
        if (NULL_PTR == TargetAddressPtr)
        {
            /* Report Error if TargetAddressPtr = NULL_PTR */
            (void) Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, ApiId, FLS_E_PARAM_DATA );
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
    #if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
        else if ( (boolean)FALSE == Fls_IsAddrWordAligned(SourceAddress) )
        {
            /* Report Error if SourceAddress is not aligned */
            (void) Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, ApiId, FLS_E_PARAM_ADDRESS );
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
        else if ( (boolean)FALSE == Fls_IsAddrWordAligned(SourceAddress + Length) )
        {
            /* Report Error if SourceAddress + Length is not aligned */
            (void) Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, ApiId, FLS_E_PARAM_LENGTH );
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
        else
        {
            /*fix misra*/
        }
    #endif /* #if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */
    }

    return RetVal;
}

#if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
/**
 * @brief        Check whether TargetAddress is word aligned
 *
 * @details      If QSPI is available and QSPI sectors are configured and
 *               if the WordAddresable option is set, check if the address
 *               is word aligned. For hyperflash memories only even addresses
 *               are allowed.
 *
 * @param[in]    TargetAddress Target address in flash memory
 *
 * @return       boolean
 * @retval       TRUE TargetAddress is word aligned
 * @retval       FALSE TargetAddress is not word aligned
 *
 * @pre          The module must be initialized
 *
 */
static boolean Fls_IsAddrWordAligned( const Fls_AddressType TargetAddress )
{
    boolean RetVal = (boolean)TRUE;

#if (FEATURE_QSPI_ADDR_CFG == 1)
    Fls_SectorIndexType TmpJobSectorIt = 0UL;
    const Qspi_Ip_ControllerConfigType *CtrlCfg;
    uint32 FlashInstance;
    uint32 ControllerNo;
    uint32 CtrlCfgNo;

    /* Check in case when a bad address is passed */
    if (FLS_TOTAL_SIZE == TargetAddress)
    {
        TmpJobSectorIt = Fls_pConfigPtr->u32SectorCount - 1U;
    }
    else
    {
        TmpJobSectorIt = Fls_GetSectorIndexByAddr( TargetAddress );
    }

    /* Get external flash instance */
    FlashInstance = (*(Fls_pConfigPtr->pFlsQspiCfgConfig->u8SectFlashUnit))[TmpJobSectorIt];
    /* Only apply for external sectors */
    if (FLS_DEVICE_INSTANCE_INVALID != FlashInstance)
    {
        /* Get controller configuration */
        ControllerNo = ((*(Fls_pConfigPtr->pFlsQspiCfgConfig->paFlashConnectionCfg))[FlashInstance]).qspiInstance;
        CtrlCfgNo = (*(Fls_pConfigPtr->pFlsQspiCfgConfig->u8QspiConfig))[ControllerNo];
        CtrlCfg = &((*(Fls_pConfigPtr->pFlsQspiCfgConfig->paQspiUnitCfg))[CtrlCfgNo]);

        if ( ((boolean)TRUE == CtrlCfg->wordAddresable) && (0U != (TargetAddress % 2U)) )
        {
           /* The WordAddresable option is set but the address is not word aligned. */
           RetVal = (boolean)FALSE;
        }
    }
#else
    /*Fix warning: unused variable*/
    (void)TargetAddress;
#endif

    return RetVal;
}
#endif /* #if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */
#endif /* FLS_DEV_ERROR_DETECT == STD_ON */

#if ( STD_ON == FLS_AC_LOAD_ON_JOB_START )
/**
 * @brief        Load access code to RAM
 *
 * @details      Load erase or write access code to statically
 *               pre-configured location in RAM
 *
 * @param[in]    Job Type of access code to load. Can be either
 *               FLS_JOB_ERASE or FLS_JOB_WRITE
 *
 * @pre          The module must be initialized
 *
 */
static void Fls_LoadAc(const Fls_JobType Job)
{
    const uint32 *RomPtr;
    uint32 *RamPtr;
    uint32 ACSize;  /* Word size */
    uint32 Cnt;

    if (FLS_JOB_ERASE == Job)
    {
        RomPtr = (const uint32 *)( (uint32)(&Fls_ACEraseRomStart) );
        RamPtr = (uint32*)((uint32)(Fls_pConfigPtr->acErasePtr));
        ACSize = (uint32)(&Fls_ACEraseSize);
    }
    else
    {
        /* FLS_JOB_WRITE */
        RomPtr = (const uint32 *)( (uint32)(&Fls_ACWriteRomStart) );
        RamPtr = (uint32*)((uint32)(Fls_pConfigPtr->acWritePtr));
        ACSize = (uint32)(&Fls_ACWriteSize);
    }

    /* Copy erase or write access code to RAM */
    /* ACSize is dynamically calculated and might not be multiple of 4U */
    for (Cnt = 0U; Cnt < ACSize; Cnt++)
    {
        /* Copy 4 bytes at a time*/
        RamPtr[Cnt] = RomPtr[Cnt];
    }
}


/**
 * @brief        Unload access code to RAM
 *
 * @details      Erase write or erase access code from statically
 *               pre-configured location in RAM. Fills the RAM location
 *               with FLS_AC_UNLOAD_PATTERN
 *
 * @param[in]    Job Type of access code to unload. Can be either
 *               FLS_JOB_ERASE or FLS_JOB_WRITE
 *
 * @pre          The module must be initialized
 *
 */
static void Fls_UnloadAc(const Fls_JobType Job)
{
    uint32 *RamPtr;
    uint32 ACSize;  /* Word size */
    uint32 Cnt;

    if (FLS_JOB_ERASE == Job)
    {
        RamPtr = (uint32*)((uint32)(Fls_pConfigPtr->acErasePtr));
        ACSize = (uint32)(&Fls_ACEraseSize);
    }
    else
    {
        /* FLS_JOB_WRITE */
        RamPtr = (uint32*)((uint32)(Fls_pConfigPtr->acWritePtr));
        ACSize = (uint32)(&Fls_ACWriteSize);
    }

    /* Unload (erase) write or erase access code from RAM */
    for (Cnt = 0U; Cnt < ACSize; Cnt++)
    {
        /* Clear 4 bytes at a time*/
        RamPtr[Cnt] = FLS_AC_UNLOAD_PATTERN;
    }
}


static inline void Fls_CheckLoadAc(Fls_BlockNumberOfSectorType BlockAc,
                                   Fls_AddressType SectorOffset
                                  )
{
    Fls_HwChType HwCh;
    Fls_AddressType JobAddress;
    Fls_BlockNumberOfSectorType BlockSector;

    /* Get channel type(INTERNAL, QSPI,...) to determine the HW IP used(internal or external flash). */
    HwCh = (*(Fls_pConfigPtr->paHwCh))[Fls_u32JobSectorIt];
    if (FLS_CH_INTERN == HwCh)
    {
        /* Get the base address of the sector. */
        JobAddress = (Fls_AddressType)(*((*(Fls_pConfigPtr->pSectorList))[Fls_u32JobSectorIt])).pSectorStartAddressPtr;
        /* Add the offset of the sector inside sector group */
        JobAddress += SectorOffset;
        /* Get block number of the sector to be erased or written */
        BlockSector = Fls_IPW_GetBlockNumberFromAddress(JobAddress);

        /* Only load if not in Asynch mode and erare/write to the same block */
        if (BlockSector == BlockAc)
        {
            /* Load position independent access code to RAM */
            Fls_LoadAc( Fls_eJob );
            Fls_bACloaded = (boolean)TRUE;

#if (STD_ON == FLS_CLEAN_CACHE_AFTER_LOAD_AC)
            if (FLS_JOB_ERASE == Fls_eJob)
            {
                /* Clean cache after loading to sync with RAM for Erase job */
                (void)Cache_Ip_CleanByAddr(CACHE_IP_ALL, TRUE, (uint32)Fls_pConfigPtr->acErasePtr, ((uint32)(&Fls_ACEraseSize) * 4U));
            }
            else
            {
                /* Clean cache after loading to sync with RAM for Write job */
                (void)Cache_Ip_CleanByAddr(CACHE_IP_ALL, TRUE, (uint32)Fls_pConfigPtr->acWritePtr, ((uint32)(&Fls_ACWriteSize) * 4U));
            }
#endif /* FLS_SUPPORT_CACHE_FEATURE */
        }
    }
}


static inline void Fls_CheckUnLoadAc(void)
{
    /* If the access code was loaded */
    if (TRUE == Fls_bACloaded)
    {
        /* Unload from RAM */
        Fls_UnloadAc( Fls_eJob );
        /* Mask the status as unloaded */
        Fls_bACloaded = (boolean)FALSE;
    }
}
#endif /* FLS_AC_LOAD_ON_JOB_START */

/**
 * @brief        Erase one complete flash sector
 *
 * @details      Call low level flash driver service
 *               to erase one complete flash sector specified by the Fls_u32JobSectorIt
 *               internal job variable
 *
 * @return       MemIf_JobResultType
 * @retval       MEMIF_JOB_OK erase operation succeeded and there
 *               is no more sectors to erase
 * @retval       MEMIF_JOB_PENDING erase operation succeeded and there
 *               is still one or more sectors to erase
 * @retval       MEMIF_JOB_FAILED operation failed due to hardware error
 *
 * @param[in]    Job Type of access code to unload. Can be either
 *               FLS_JOB_ERASE or FLS_JOB_WRITE
 *
 * @pre          The module must be initialized, the Fls_u32JobSectorIt internal
 *               job variable must contain index of logical sector to be erased, and
 *               Fls_u32JobSectorEnd must contain index of last logical sector to be erased
 *
 * @post         increments the Fls_u32JobSectorIt internal job variable
 *
 *
 */
static MemIf_JobResultType Fls_DoJobErase(void)
{
    MemIf_JobResultType RetVal = MEMIF_JOB_PENDING;
    uint32 DataStore;
    Fls_LengthType PhysicalSectorSize;
    Fls_AddressType SectorTransferOffset;

    if ( Fls_u32JobAddrIt > Fls_u32JobAddrEnd )
    {
        /* No more sectors to erase */
        RetVal = MEMIF_JOB_OK;
    }
    else
    {
        boolean Asynch;
        Fls_LLDReturnType LldRetVal;

        /* Calculate offset of the sector inside sector group */
        SectorTransferOffset = Fls_CalcSectorTransferOffset();

        /* Get the Fls_IPW_SectorErase function operation mode */
        if ( 0U != ((*(Fls_pConfigPtr->paSectorFlags))[Fls_u32JobSectorIt] &
                   (FLS_SECTOR_ERASE_ASYNCH)
                  )
          )
        {
            /* The sector write is performed in an asynchronous manner, it is ASYNC mode. */
            Asynch = (boolean)TRUE;
        }
        else
        {
            /* The sector write is SYNC */
            Asynch = (boolean)FALSE;
#if (STD_ON == FLS_AC_LOAD_ON_JOB_START)
            Fls_CheckLoadAc(FLs_u8blockAcErase, SectorTransferOffset);
#endif /* #if (STD_ON == FLS_AC_LOAD_ON_JOB_START) */
        }

        /* Get size of the physical sector */
        PhysicalSectorSize = (*(Fls_pConfigPtr->paSectorSize))[Fls_u32JobSectorIt];
        /* Update the Fls_JobAddressIt iterator */
        Fls_u32JobAddrIt += PhysicalSectorSize;

        LldRetVal = Fls_IPW_SectorErase( SectorTransferOffset , PhysicalSectorSize, Asynch );

        if ( (Fls_u32JobAddrIt > Fls_u32JobAddrEnd) && (FLASH_E_OK == LldRetVal) )
        {
            RetVal = MEMIF_JOB_OK;
        }
        else if (FLASH_E_FAILED == LldRetVal)
        {
            (void)Det_ReportTransientFault((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_ERASE_FAILED);
            RetVal = MEMIF_JOB_FAILED;
        }
        else
        {
            /* The erase operation is either:
                1. For ASYNCHRONOUS operation: pending (FLASH_E_PENDING) in hardware, or
                2. For SYNCHRONOUS operation: finished (FLASH_E_OK) and there are more sectors to erase.

                1. If the operation is Async, than the sector is increased in
                   Fls_IPW_LLDMainFunction()(for Async erase).
                   when completing the job.
                2. If the operation is Sync erase, than the sector is increased below because the job is complete.
            */
            DataStore = (*(Fls_pConfigPtr->paSectorFlags))[Fls_u32JobSectorIt];
            if (0U == ( DataStore & FLS_SECTOR_ERASE_ASYNCH))
            {
                if ((Fls_u32JobAddrIt > ((*(Fls_pConfigPtr->paSectorEndAddr))[Fls_u32JobSectorIt])))
                {
                    Fls_u32JobSectorIt++;
                }
            }
        }
#if ( STD_ON == FLS_AC_LOAD_ON_JOB_START )
        Fls_CheckUnLoadAc();
#endif /* #if (STD_ON == FLS_AC_LOAD_ON_JOB_START) */
    }

    return RetVal;
}

/**
 * @brief       Calculate last logical address to read, write, or compare
 *              from in current cycle of Fls_MainFunction
 *
 * @details      Calculate last logical address of Fls_DoJobDataTransfer
 *               loop to limit number of bytes transferred to/from flash memory
 *               in one cycle of Fls_MainFunction
 *
 * @return       Fls_AddressType
 * @retval       0 .. (FLS_TOTAL_SIZE - 1)
 *
 * @param[in]    MaxTransfer Maximum number of bytes to read, write, or compare
 *               in current cycle of Fls_MainFunction
 *
 * @pre          Fls_u32JobAddrIt must contain current, and Fls_u32JobAddrEnd last
 *               logical address of current job
 *
 */
static Fls_AddressType Fls_CalcMaxTransferAddrEnd(const Fls_LengthType MaxTransfer)
{
    Fls_AddressType MaxTransferEndAddr = Fls_u32JobAddrEnd;

    /* Adjust the MaxTransferEndAddr address to transfer only
        the MaxTransfer bytes in one Fls_MainFunction() call */
    if ( MaxTransferEndAddr >= (Fls_u32JobAddrIt + MaxTransfer) )
    {
        MaxTransferEndAddr = (Fls_u32JobAddrIt + MaxTransfer) - 1U;
    }
    else
    {
        /* No adjustment is necessary. Job will finish in this cycle
            of Fls_MainFunction */
    }

    return MaxTransferEndAddr;
}

/**
 * @brief       Calculate sector offset to read, write, or compare
 *              from current sector
 *
 * @details      Calculate sector offset in bytes for low-level driver
 *               services
 *
 * @return       Fls_AddressType
 * @retval      0 .. (FLS_SECTOR_SIZE - 1)
 *
 * @pre         The module must be initialized, Fls_u32JobSectorIt internal job
 *              variable must contain index of current logical sector,
 *              Fls_u32JobAddrIt must contain current logical address
 *
 */
static Fls_AddressType Fls_CalcSectorTransferOffset(void)
{
    Fls_AddressType SectorAddrStart;

    /* Get first address of Fls_u32JobSectorIt sector */
    SectorAddrStart = Fls_GetSectorStartAddr( Fls_u32JobSectorIt );

    /* Calculate the sector offset */
    return (Fls_u32JobAddrIt - SectorAddrStart);
}


/**
 * @brief       Calculate number of bytes to read, write, or compare
 *              from current sector
 *
 * @details      Calculate sector transfer length for low-level driver
 *               services
 *
 * @param[in]    MaxTransferEndAddr Last address to read, write,
 *               or compare data from in this cycle of Fls_MainFunction
 *
 * @return      Fls_AddressType
 * @retval      0 .. (FLS_SECTOR_SIZE - 1)
 *
 * @pre        The module must be initialized, Fls_u32JobSectorIt internal job
 *             variable must contain index of current logical sector,
 *             Fls_u32JobAddrIt must contain current logical address, and
 *             MaxTransferEndAddr must be >= Fls_u32JobAddrIt and lie within
 *             the specified lower and upper flash address boundaries
 *
 */
static Fls_AddressType Fls_CalcSectorTransferLength(const Fls_AddressType MaxTransferEndAddr)
{
    Fls_AddressType SectorEndAddr;
    Fls_AddressType MaxTransferAddr = MaxTransferEndAddr;
#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
    uint32 TransferSize = 0U;
    uint32 InterfaceSize = 0U;
#endif

    /* Get last address of Fls_u32JobSectorIt sector */
    SectorEndAddr = (*(Fls_pConfigPtr->paSectorEndAddr))[Fls_u32JobSectorIt];

    /* Adjust SectorEndAddr based on hardware implementation */
#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
    if ((FLS_CH_INTERN == (*(Fls_pConfigPtr->paHwCh))[Fls_u32JobSectorIt]) && (FLS_JOB_WRITE == Fls_eJob))
    {
        InterfaceSize = FLS_INTERNAL_WRITE_SIZE;

        TransferSize = MaxTransferAddr - Fls_u32JobAddrIt + 1U;
        if (TransferSize > (InterfaceSize - (Fls_u32JobAddrIt % InterfaceSize)))
        {
            MaxTransferAddr = (Fls_u32JobAddrIt + (InterfaceSize - (Fls_u32JobAddrIt % InterfaceSize))) - 1U;
        }
    }
#endif
    /* Adjust the SectorEndAddr according to max number of bytes to transfer
        during one cycle of Fls_MainFunction */
    if (SectorEndAddr > MaxTransferAddr)
    {
        SectorEndAddr = MaxTransferAddr;
    }
    else
    {
        /* No adjustment is necessary. There is more sectors to transfer */
    }

    /* Calculate number of bytes to read from the sector */
    return ((SectorEndAddr - Fls_u32JobAddrIt) + 1U);
}

/**
 * @brief        Write up to one physical flash sector
 *
 * @details      Call low level flash driver service to write given number of bytes
 *               at given sector offset
 *
 * @param[in]    SectorOffset Flash sector offset to write data from
 * @param[in]    Length Number of bytes to read
 *
 * @return       Fls_LLDReturnType
 * @retval       FLASH_E_OK write operation succeeded
 * @retval       FLASH_E_FAILED write operation failed due to a hardware error
 *
 * @pre          The module must be initialized, the Fls_u32JobSectorIt internal job
 *               variable must contain valid index of logical sector to write,
 *               SectorOffset and Length must be in physical sector boundary and page
 *               aligned, Fls_pJobDataSrcPtr internal job variable must point to data
 *               write buffer
 *
 */
static Fls_LLDReturnType Fls_DoJobWrite(const Fls_AddressType SectorOffset,
                                        const Fls_AddressType Length
                                       )
{
    Fls_LLDReturnType RetVal;
    boolean Asynch;

    /* Get the Fls_IPW_SectorWrite function operation mode */
    if ( 0U != ((*(Fls_pConfigPtr->paSectorFlags))[Fls_u32JobSectorIt] & \
                (FLS_PAGE_WRITE_ASYNCH) \
               )
       )
    {
        /* The sector write is performed in an asynchronous manner, it is ASYNC */
        Asynch = (boolean)TRUE;
    }
    else
    {
        /* The sector write is SYNC*/
        Asynch = (boolean)FALSE;
#if (STD_ON == FLS_AC_LOAD_ON_JOB_START)
        Fls_CheckLoadAc(FLs_u8blockAcWrite, SectorOffset);
#endif /* #if (STD_ON == FLS_AC_LOAD_ON_JOB_START) */
    }

    RetVal = Fls_IPW_SectorWrite(SectorOffset, Length, Fls_pJobDataSrcPtr, Asynch);

    if (FLASH_E_FAILED == RetVal)
    {
        /* An error will be reported */
        (void)Det_ReportTransientFault((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_WRITE_FAILED);
    }
#if ( (FLS_ERASE_VERIFICATION_ENABLED == STD_ON) || (FLS_WRITE_VERIFICATION_ENABLED == STD_ON) )
    /* A verification has been failed, convert to FLASH_E_FAILED */
    else if (FLASH_E_BLOCK_INCONSISTENT == RetVal)
    {
        RetVal = FLASH_E_FAILED;
    }
    else
    {
        ; /* empty else at the end of an else-if structure */
    }
#endif /* (FLS_ERASE_VERIFICATION_ENABLED == STD_ON) || (FLS_WRITE_VERIFICATION_ENABLED == STD_ON) */

#if ( STD_ON == FLS_AC_LOAD_ON_JOB_START )
    Fls_CheckUnLoadAc();
#endif /* #if (STD_ON == FLS_AC_LOAD_ON_JOB_START) */
    return RetVal;
}

static Fls_LLDReturnType Fls_ProcessJobDataTransfer(const Fls_AddressType SectorOffset,
                                                    const Fls_AddressType Length
                                                   )
{
    Fls_LLDReturnType TransferRetVal = FLASH_E_FAILED;

    /* Call Low-level driver to transfer data to/from physical flash device */
    switch (Fls_eJob)
    {
        case FLS_JOB_WRITE:
            TransferRetVal =  Fls_DoJobWrite( SectorOffset, Length );
            /* Update the source data pointer for next write */
            Fls_pJobDataSrcPtr = &(Fls_pJobDataSrcPtr[Length]);
            break;

        case FLS_JOB_READ:
            TransferRetVal =  Fls_IPW_SectorRead(SectorOffset, Length, Fls_pJobDataDestPtr, NULL_PTR);
            if (FLASH_E_FAILED == TransferRetVal)
            {
                (void)Det_ReportTransientFault((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_READ_FAILED);
            }
            /* Update the destination data pointer for next read */
            Fls_pJobDataDestPtr = &(Fls_pJobDataDestPtr[Length]);
            break;

#if ( FLS_COMPARE_API == STD_ON )
        case FLS_JOB_COMPARE:
            TransferRetVal =  Fls_IPW_SectorRead(SectorOffset, Length, NULL_PTR, Fls_pJobDataSrcPtr);
            if ((FLASH_E_BLOCK_INCONSISTENT == TransferRetVal) || (FLASH_E_FAILED == TransferRetVal))
            {
                (void)Det_ReportTransientFault((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_COMPARE_FAILED);
            }
            /* Update the source data pointer for next compare */
            Fls_pJobDataSrcPtr = &(Fls_pJobDataSrcPtr[Length]);
            break;
#endif /* FLS_COMPARE_API == STD_ON */

#if ( FLS_BLANK_CHECK_API == STD_ON )
        case FLS_JOB_BLANK_CHECK:
            TransferRetVal =  Fls_IPW_SectorRead(SectorOffset, Length, NULL_PTR, NULL_PTR);
            if (FLASH_E_FAILED == TransferRetVal)
            {
                (void)Det_ReportTransientFault((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_COMPARE_FAILED);
            }
            break;
#endif /* FLS_BLANK_CHECK_API == STD_ON */

        default:
            ; /* Do nothing - should not happen in Fully Trusted Environment;
            "default" clause added to fulfil MISRA Rule 15.3 */
            break;
    }

    return TransferRetVal;
}

/**
    @brief Read, write, or compare flash data
    @details Call Fls_ProcessJobDataTransfer function to read, write,
        or compare flash data. The functions ensures that only pre-configured
        maximum number of bytes is read, written, or compared during
        one cycle of the Fls_MainFunction call

    @return MemIf_JobResultType
    @retval MEMIF_JOB_OK read, write, or compare operation succeeded and there
        is no more data to read, write, or compare
    @retval MEMIF_JOB_PENDING read, write, or compare operation succeeded
        and there is still some data to read, write, or compare
    @retval MEMIF_JOB_FAILED read, write, or compare operation failed due
        to a hardware error
    @retval MEMIF_BLOCK_INCONSISTENT Compared data from a flash compare
        Job are not equal

    @pre The module must be initialized, the Fls_u32JobSectorIt internal
        Job variable must contain index of logical sector to read, write,
        or compare, the Fls_u32JobAddrIt must contain logical address to read,
        write or compare, and Fls_u32JobAddrEnd must contain last address to read,
        write or compare.

    @post Updates the Fls_u32JobSectorIt, Fls_pJobDataSrcPtr, Fls_pJobDataDestPtr,
        and Fls_u32JobAddrIt internal Job variables
 */
static MemIf_JobResultType Fls_DoJobDataTransfer(const Fls_LengthType MaxTransfer)
{
    MemIf_JobResultType RetVal = MEMIF_JOB_PENDING;

    /* No more data to write */
    if (Fls_u32JobAddrIt > Fls_u32JobAddrEnd)
    {
        RetVal = MEMIF_JOB_OK;
    }
    else
    {
        Fls_AddressType u32MaxTransferAddrEnd;
        Fls_LLDReturnType TransferRetVal;

        /* Last address to transfer data to/from in this call of the Fls_MainFunction() functions */
        u32MaxTransferAddrEnd = Fls_CalcMaxTransferAddrEnd( MaxTransfer );

        /* Transfer sector by sector until the u32MaxTransferAddrEnd address is reached or error is detected */
        do
        {
            Fls_AddressType SectorTransferOffset;
            Fls_AddressType u32SectorTransferLength;

            /* Calculate offset and length of the sector data transfer */
            SectorTransferOffset = Fls_CalcSectorTransferOffset();
            u32SectorTransferLength = Fls_CalcSectorTransferLength( u32MaxTransferAddrEnd );

            /* Update the Fls_JobAddressIt iterator.*/
            Fls_u32JobAddrIt += u32SectorTransferLength;

            /* Call Low-level driver to transfer data to/from physical flash device */
            TransferRetVal = Fls_ProcessJobDataTransfer(SectorTransferOffset, u32SectorTransferLength);

            if ( FLASH_E_OK == TransferRetVal ) /* If the previous(SYNC job) has finished successfully */
            {
                if ( (Fls_u32JobAddrIt > (*(Fls_pConfigPtr->paSectorEndAddr))[Fls_u32JobSectorIt]))
                {
                    /* Move on to the next sector */
                    Fls_u32JobSectorIt++;
                }
            }
        }
        while ( (Fls_u32JobAddrIt <= u32MaxTransferAddrEnd) &&
                (FLASH_E_OK == TransferRetVal)
              );


        if (FLASH_E_FAILED == TransferRetVal)
        {
            RetVal = MEMIF_JOB_FAILED;
        }
        else if (FLASH_E_BLOCK_INCONSISTENT == TransferRetVal)
        {
            /* compare job only */
            RetVal = MEMIF_BLOCK_INCONSISTENT;
        }
        else if ( (Fls_u32JobAddrIt > Fls_u32JobAddrEnd) &&
                  (FLASH_E_OK == TransferRetVal)
                )
        {
            /* All desired job data has been successfully transferred */
            RetVal = MEMIF_JOB_OK;
        }
        else
        {
            ; /* The write operation is either pending (FLASH_E_PENDING)
            in hardware in case of asynchronous operation or
            finished (FLASH_E_OK) in case of synchronous operation
            and there is more pages to write */
        }
    }

    return RetVal;
}

#if (FLS_CHECK_CFG_CRC == STD_ON)
/**
 * @brief        Function to handle cumulative CRC calculation over input data.
 *
 * @details      Handles cumulative CRC calculation over input 32-bit data, .
 *
 * @param[in]    inputData ... data to be CRC-ed
 *
 * @return       void
 * @retval       None.
 *
 * @pre          Fls_ResetCRC() was executed before the first call of
 *               Fls_AccumulateCRC().
 *
 *
 */
static void Fls_AccumulateCRC(uint32 InputData)
{
    if (0xFFFFU < InputData)
    {
        /* preparation for accumulation of higher 16 bits of the InputData */
        Fls_u32AccCRCremainder = (Fls_u32AccCRCremainder << 16U) | (InputData >> 16U);
        /* make 16-bit accumulated result (in lower 16-bits of Fls_u32AccCRCremainder) */
        Fls_UpdateCRCreminder(FLS_CRC_16_BITS);
    }

    if (0xFFU < InputData)
    {
        /* preparation for accumulation of lower 16 bits of the InputData */
        Fls_u32AccCRCremainder = (Fls_u32AccCRCremainder << 16U) | (InputData & 0x0000FFFFU);
        /* make 16-bit accumulated result (in lower 16-bits of Fls_u32AccCRCremainder) */
        Fls_UpdateCRCreminder(FLS_CRC_16_BITS);
    }
    else
    {
        /* optimization: only 8 LSB bits are processed */
        /* preparation for accumulation of lower 8 bits of the InputData */
        Fls_u32AccCRCremainder = (Fls_u32AccCRCremainder << 8U) | InputData;
        /* make 16-bit accumulated result (in lower 16-bits of Fls_u32AccCRCremainder) */
        Fls_UpdateCRCreminder(FLS_CRC_8_BITS);
    }
}


/**
 * @brief        Function to reset CRC calculation.
 *
 * @details      Resets accumulated Fls_u32AccCRCremainder.
 *
 * @param[in]    void
 *
 * @return       void
 * @retval       None.
 *
 * @pre          None.
 *
 *
 */
static void Fls_ResetCRC(void)
{
    Fls_u32AccCRCremainder = 0U;
}

/**
 * @brief        Function to finalize CRC calculation.
 *
 * @details      Finalizes accumulated CRC computation and resturns the final
 *               CRC checksum.
 *
 * @param[in]    void
 *
 * @return       uint32
 * @retval       The final CRC checksum in the lower 16 bits.
 *
 * @pre          Fls_AccumulateCRC() was executed at least once before
 *               calling Fls_FinalizeCRC().
 *
 */
static uint32 Fls_FinalizeCRC(void)
{
    /* add the final 0x0000 to the remainder */
    Fls_u32AccCRCremainder = (Fls_u32AccCRCremainder << 16U);
    /* make the final 16-bit CRC */
    Fls_UpdateCRCreminder(FLS_CRC_16_BITS);

    return Fls_u32AccCRCremainder;
}

/**
 * @brief        Function to perform CRC calculation over input 32-bit data.
 *
 * @details      Process 32-bit data to 16-bit reminder.
 *
 * @param[in]    accDataPtr ... ptr to data to be processed
 *
 * @return       void
 * @retval       None.
 *
 * @pre          Can be called only from Fls_AccumulateCRC().
 *
 */

static void Fls_UpdateCRCreminder(Fls_CrcDataSizeType DataSize)
{
    uint32 CrcPolynomSft;
    uint32 LeadingOne;
    uint32 AccDataLoc;
    uint32 LeadingOneInitial;

    switch (DataSize)
    {
        case FLS_CRC_8_BITS:
            CrcPolynomSft = 0x11021U << 7U; /* shifted CRC-16-CCITT (x.25 protocol)*/
            LeadingOneInitial = 0x10000U << 7U;
            break;

        case FLS_CRC_16_BITS:
        default:
            CrcPolynomSft = 0x11021U << 15U; /* shifted CRC-16-CCITT (x.25 protocol)*/
            LeadingOneInitial = 0x10000U << 15U;
            break;
    }

    /* copy static variable to auto (computation over static may be slow) */
    AccDataLoc = Fls_u32AccCRCremainder;

    /* CRC computation */
    for (LeadingOne = LeadingOneInitial; LeadingOne >= 0x00010000U; LeadingOne >>= 1U)
    {
        if (0U != (AccDataLoc & LeadingOne))
        {
            AccDataLoc ^= CrcPolynomSft;
        }
        CrcPolynomSft >>= 1U;
    }

    /* copy back to static variable */
    Fls_u32AccCRCremainder = AccDataLoc;
}

/**
 * @brief        Calculates CRC over Fls configuration.
 *
 * @details      Calculates CRC over selected items of Fls configuration set
 *               pointed to by ConfigPtr.
 *
 * @param[in]    ConfigPtr        Pointer to flash driver configuration set.
 *
 * @return       uint32
 * @retval       0 .. 0xFFFF (16-bit CRC using CRC-16-CCITT polynomial)
 *
 * @pre          Fls_pConfigPtr must not be properly initialized.
 *
 */
static Fls_CrcType  Fls_CalcCfgCRC(void)
{
    Fls_SectorIndexType SectorIndex;
#if (FLS_QSPI_SECTORS_CONFIGURED == STD_ON)
    uint8 IterUnits;
#endif /*#ifdef (FLS_QSPI_SECTORS_CONFIGURED == STD_ON)*/

    /* Reset the accumulated CRC value */
    Fls_ResetCRC();

    /* Accumulate the rest of the params (common for all sectors within a config set) */
    /* CRC - Accumulate eDefaultMode */
    Fls_AccumulateCRC((uint32)Fls_pConfigPtr->eDefaultMode);
    /* CRC - Accumulate u32MaxReadFastMode */
    Fls_AccumulateCRC((uint32)Fls_pConfigPtr->u32MaxReadFastMode );
    /* CRC - Accumulate u32MaxReadNormalMode */
    Fls_AccumulateCRC((uint32)Fls_pConfigPtr->u32MaxReadNormalMode);
    /* CRC - Accumulate u32MaxWriteFastMode */
    Fls_AccumulateCRC((uint32)Fls_pConfigPtr->u32MaxWriteFastMode);
    /* CRC - Accumulate u32MaxWriteNormalMode */
    Fls_AccumulateCRC((uint32)Fls_pConfigPtr->u32MaxWriteNormalMode);
    /* CRC - Accumulate u32SectorCount */
    Fls_AccumulateCRC((uint32)Fls_pConfigPtr->u32SectorCount);

    /* Iterate through the sector-dependent params */
    for (SectorIndex = 0U; SectorIndex < Fls_pConfigPtr->u32SectorCount; SectorIndex++)
    {
        /* CRC - Accumulate paSectorEndAddr */
        Fls_AccumulateCRC((uint32)(*(Fls_pConfigPtr->paSectorEndAddr))[SectorIndex]);
        /* CRC - Accumulate paSectorSize */
        Fls_AccumulateCRC((uint32)(*(Fls_pConfigPtr->paSectorSize))[SectorIndex]);
        /* CRC - Accumulate pSectorList */
        if (NULL_PTR != (*(Fls_pConfigPtr->pSectorList))[SectorIndex])
        {   /* Accumulate Sector index for internal flash sectors */
            Fls_AccumulateCRC((uint32)((*((*(Fls_pConfigPtr->pSectorList))[SectorIndex])).u32SectorId));
            /* Accumulate Sector Start Address for internal flash sectors */
            Fls_AccumulateCRC((uint32)(*((*(Fls_pConfigPtr->pSectorList))[SectorIndex])).pSectorStartAddressPtr);
        }
        /* CRC - Accumulate paSectorFlags */
        Fls_AccumulateCRC((uint32)(*(Fls_pConfigPtr->paSectorFlags))[SectorIndex]);
#if (FLS_QSPI_SECTORS_CONFIGURED == STD_ON)
        /* CRC - Accumulate paHwCh */
        Fls_AccumulateCRC((uint32)(*(Fls_pConfigPtr->paHwCh))[SectorIndex]);
        /* CRC - Accumulate paSectorHwAddress */
        Fls_AccumulateCRC((uint32)(*(Fls_pConfigPtr->paSectorHwAddress))[SectorIndex]);
#endif /*#if (FLS_QSPI_SECTORS_CONFIGURED == STD_ON)*/
    }
#if (FLS_QSPI_SECTORS_CONFIGURED == STD_ON)
    /* CRC - Accumulate u8FlashUnitsCount*/
    Fls_AccumulateCRC((uint32)(Fls_pConfigPtr->pFlsQspiCfgConfig->u8FlashUnitsCount));
    /* CRC - Accumulate (*paFlashConnectionCfg)[]  */
    for (IterUnits = 0U; IterUnits < Fls_pConfigPtr->pFlsQspiCfgConfig->u8FlashUnitsCount; IterUnits++)
    {
        Fls_AccumulateCRC((uint32)((*(Fls_pConfigPtr->pFlsQspiCfgConfig->paFlashConnectionCfg))[IterUnits]).qspiInstance);
        Fls_AccumulateCRC((uint32)((*(Fls_pConfigPtr->pFlsQspiCfgConfig->paFlashConnectionCfg))[IterUnits]).connectionType);
    }
    /* CRC - Accumulate (*u8FlashConfig)[] */
    for (IterUnits = 0U; IterUnits < Fls_pConfigPtr->pFlsQspiCfgConfig->u8FlashUnitsCount; IterUnits++)
    {
        Fls_AccumulateCRC((uint32)(*(Fls_pConfigPtr->pFlsQspiCfgConfig->u8FlashConfig))[IterUnits]);
    }
    /* CRC - Accumulate u8FlashConfigCount*/
    Fls_AccumulateCRC((uint32)(Fls_pConfigPtr->pFlsQspiCfgConfig->u8FlashConfigCount));

    /* CRC - Accumulate (*paFlashCfg)[] */
    Fls_AccumulateCRC_Qspi_paFlashCfg();
    /* CRC - Accumulate u8QspiUnitsCount*/
    Fls_AccumulateCRC((uint32)(Fls_pConfigPtr->pFlsQspiCfgConfig->u8QspiUnitsCount));

                    /* CRC - Accumulate (*u8QspiConfig)[] */
    for (IterUnits = 0U; IterUnits < Fls_pConfigPtr->pFlsQspiCfgConfig->u8QspiUnitsCount; IterUnits++)
    {
        /* CRC - Accumulate memSize  */
        Fls_AccumulateCRC((uint32)(*(Fls_pConfigPtr->pFlsQspiCfgConfig->u8QspiConfig))[IterUnits]);
    }

    /* CRC - Accumulate u8QspiConfigCount */
    Fls_AccumulateCRC((uint32)(Fls_pConfigPtr->pFlsQspiCfgConfig->u8QspiConfigCount));
    /* CRC - Accumulate (*QspiUnitCfg)[] */
    Fls_AccumulateCRC_Qspi_paQspiUnitCfg();
#endif /*#if (FLS_QSPI_SECTORS_CONFIGURED == STD_ON)*/
    return ((Fls_CrcType)Fls_FinalizeCRC());
}


#if (FLS_QSPI_SECTORS_CONFIGURED == STD_ON)
/**
 * @brief        Calculates CRC over QSPI FlashCfg configuration.
 *
 * @details      Calculates CRC over FlashCfg items of Fls QSPI configuration set
 *               pointed to by ConfigPtr.
 *
 * @return       void
 * @retval       None.
 *
 * @pre          Fls_pConfigPtr must be properly initialized.
 *
 */
static void Fls_AccumulateCRC_Qspi_paFlashCfg(void)
{
    uint8 IterUnits;
    const Qspi_Ip_MemoryConfigType * FlashCfg;

    /* CRC - Accumulate (*FlashCfg)[] */
    for (IterUnits = 0U; IterUnits < Fls_pConfigPtr->pFlsQspiCfgConfig->u8FlashConfigCount; IterUnits++)
    {
        FlashCfg = &((*(Fls_pConfigPtr->pFlsQspiCfgConfig->paFlashCfg))[IterUnits]);

        /* CRC - Accumulate memSize  */
        Fls_AccumulateCRC( (uint32)FlashCfg->memSize );
        /* CRC - Accumulate pageSize  */
        Fls_AccumulateCRC( (uint32)FlashCfg->pageSize );
        /* CRC - Accumulate readLut  */
        Fls_AccumulateCRC( (uint32)FlashCfg->readLut );
        /* CRC - Accumulate writeLut  */
        Fls_AccumulateCRC( (uint32)FlashCfg->writeLut );
        /* CRC - Accumulate read0xxLut  */
        Fls_AccumulateCRC( (uint32)FlashCfg->read0xxLut );
        /* CRC - Accumulate read0xxLutAHB  */
        Fls_AccumulateCRC( (uint32)FlashCfg->read0xxLutAHB );

        /* CRC - Accumulate readIdSettings.readIdLut  */
        Fls_AccumulateCRC( (uint32)FlashCfg->readIdSettings.readIdLut );
        /* CRC - Accumulate readIdSettings.readIdSize  */
        Fls_AccumulateCRC( (uint32)FlashCfg->readIdSettings.readIdSize );
        /* CRC - Accumulate readIdSettings.readIdExpected  */
        Fls_AccumulateCRC( (uint32)FlashCfg->readIdSettings.readIdExpected );

        /* CRC - Accumulate eraseSettings  */
        Fls_AccumulateCRC_Qspi_eraseSettings( &(FlashCfg->eraseSettings) );

                                            /* CRC - Accumulate statusConfig */
        /* CRC - Accumulate statusRegReadLut */
        Fls_AccumulateCRC( (uint32)FlashCfg->statusConfig.statusRegReadLut );
        /* CRC - Accumulate statusRegWriteLut */
        Fls_AccumulateCRC( (uint32)FlashCfg->statusConfig.statusRegWriteLut );
        /* CRC - Accumulate writeEnableSRLut */
        Fls_AccumulateCRC( (uint32)FlashCfg->statusConfig.writeEnableSRLut );
        /* CRC - Accumulate writeEnableLut */
        Fls_AccumulateCRC( (uint32)FlashCfg->statusConfig.writeEnableLut );
        /* CRC - Accumulate regSize */
        Fls_AccumulateCRC( (uint32)FlashCfg->statusConfig.regSize );
        /* CRC - Accumulate busyOffset */
        Fls_AccumulateCRC( (uint32)FlashCfg->statusConfig.busyOffset );
        /* CRC - Accumulate busyValue */
        Fls_AccumulateCRC( (uint32)FlashCfg->statusConfig.busyValue );
        /* CRC - Accumulate writeEnableOffset */
        Fls_AccumulateCRC( (uint32)FlashCfg->statusConfig.writeEnableOffset );
        /* CRC - Accumulate blockProtectionOffset */
        Fls_AccumulateCRC( (uint32)FlashCfg->statusConfig.blockProtectionOffset );
        /* CRC - Accumulate blockProtectionWidth */
        Fls_AccumulateCRC( (uint32)FlashCfg->statusConfig.blockProtectionWidth );

                                        /* CRC - Accumulate suspendSettings */
        /* CRC - Accumulate eraseSuspendLut */
        Fls_AccumulateCRC( (uint32)FlashCfg->suspendSettings.eraseSuspendLut );
        /* CRC - Accumulate eraseResumeLut */
        Fls_AccumulateCRC( (uint32)FlashCfg->suspendSettings.eraseResumeLut );
        /* CRC - Accumulate programSuspendLut */
        Fls_AccumulateCRC( (uint32)FlashCfg->suspendSettings.programSuspendLut );
        /* CRC - Accumulate programResumeLut */
        Fls_AccumulateCRC( (uint32)FlashCfg->suspendSettings.programResumeLut );

                                        /* CRC - Accumulate resetSettings */
        /* CRC - Accumulate resetCmdLut */
        Fls_AccumulateCRC( (uint32)FlashCfg->resetSettings.resetCmdLut );
        /* CRC - Accumulate resetCmdCount */
        Fls_AccumulateCRC( (uint32)FlashCfg->resetSettings.resetCmdCount );

                                        /* CRC - Accumulate initResetSettings */
        /* CRC - Accumulate resetCmdLut */
        Fls_AccumulateCRC( (uint32)FlashCfg->initResetSettings.resetCmdLut );
        /* CRC - Accumulate resetCmdCount */
        Fls_AccumulateCRC( (uint32)FlashCfg->initResetSettings.resetCmdCount );

        /* CRC - Accumulate initConfiguration */
        Fls_AccumulateCRC_Qspi_initConfiguration( &(FlashCfg->initConfiguration) );

                                    /* CRC - Accumulate lutSequences */
        /* CRC - Accumulate opCount */
        Fls_AccumulateCRC( (uint32)FlashCfg->lutSequences.opCount );
    }
}

/**
 * @brief        Calculates CRC over QSPI eraseSettings configuration.
 *
 * @details      Calculates CRC over eraseSettings items of Fls QSPI configuration set
 *               pointed to by ConfigPtr.
 *
 * @param[in]    IterUnits The index of QSPI configuration
 *
 * @return       void
 * @retval       None.
 *
 * @pre          Fls_pConfigPtr must be properly initialized.
 *
 */
static void Fls_AccumulateCRC_Qspi_eraseSettings(const Qspi_Ip_EraseConfigType *EraseSettings)
{
    uint8 EraseTypes;

    for (EraseTypes = 0U; EraseTypes < QSPI_IP_ERASE_TYPES; EraseTypes++)
    {
        /* CRC - Accumulate eraseLut  */
        Fls_AccumulateCRC( (uint32)EraseSettings->eraseTypes[EraseTypes].eraseLut );
        /* CRC - Accumulate Size of the erased area  */
        Fls_AccumulateCRC( (uint32)EraseSettings->eraseTypes[EraseTypes].size );
    }
    /* CRC - Accumulate chipEraseLut  */
    Fls_AccumulateCRC( (uint32)EraseSettings->chipEraseLut );
}

/**
 * @brief        Calculates CRC over QSPI InitConfiguration configuration.
 *
 * @details      Calculates CRC over EraseSettings items of Fls QSPI configuration set
 *               pointed to by ConfigPtr.
 *
 * @param[in]    IterUnits The index of QSPI configuration
 *
 * @return       void
 * @retval       None.
 *
 * @pre          Fls_pConfigPtr must be properly initialized.
 *
 */
static void Fls_AccumulateCRC_Qspi_initConfiguration(const Qspi_Ip_InitConfigType *InitConfiguration)
{
    uint8 IterInstrOpers;
    const Qspi_Ip_InitOperationType * Operation;

    /* CRC - Accumulate opCount */
    Fls_AccumulateCRC( (uint32)InitConfiguration->opCount);

    for (IterInstrOpers = 0U; IterInstrOpers < InitConfiguration->opCount; IterInstrOpers++)
    {
        Operation = &(InitConfiguration->operations[IterInstrOpers]);

        /* CRC - Accumulate opType */
        Fls_AccumulateCRC( (uint32)Operation->opType );
        /* CRC - Accumulate command1Lut */
        Fls_AccumulateCRC( (uint32)Operation->command1Lut );
        /* CRC - Accumulate command2Lut */
        Fls_AccumulateCRC( (uint32)Operation->command2Lut );
        /* CRC - Accumulate weLut */
        Fls_AccumulateCRC( (uint32)Operation->weLut );
        /* CRC - Accumulate addr */
        Fls_AccumulateCRC( (uint32)Operation->addr );
        /* CRC - Accumulate size */
        Fls_AccumulateCRC( (uint32)Operation->size );
        /* CRC - Accumulate shift */
        Fls_AccumulateCRC( (uint32)Operation->shift );
        /* CRC - Accumulate width */
        Fls_AccumulateCRC( (uint32)Operation->width );
        /* CRC - Accumulate value */
        Fls_AccumulateCRC( (uint32)Operation->value );
    }
}

/**
 * @brief        Calculates CRC over QSPI dllSettings configuration.
 *
 * @details      Calculates CRC over dllSettings items of Fls QSPI configuration set
 *               pointed to by ConfigPtr.
 *
 * @param[in]    IterUnits The index of QSPI configuration
 *
 * @return       void
 * @retval       None.
 *
 * @pre          Fls_pConfigPtr must be properly initialized.
 *
 */
static void Fls_AccumulateCRC_Qspi_dllSettings(const Qspi_Ip_ControllerConfigType *QspiUnitCfg)
{
    /* CRC - Accumulate dllMode */
    Fls_AccumulateCRC( (uint32)(QspiUnitCfg->dllSettingsA.dllMode) );
    /* CRC - Accumulate freqEnable */
    Fls_AccumulateCRC( (uint32)(QspiUnitCfg->dllSettingsA.freqEnable? 1U : 0U) );
    /* CRC - Accumulate coarseDelay */
    Fls_AccumulateCRC( (uint32)(QspiUnitCfg->dllSettingsA.coarseDelay) );
    /* CRC - Accumulate fineDelay */
    Fls_AccumulateCRC( (uint32)(QspiUnitCfg->dllSettingsA.fineDelay) );
    /* CRC - Accumulate tapSelect */
    Fls_AccumulateCRC( (uint32)(QspiUnitCfg->dllSettingsA.tapSelect) );
}




/**
 * @brief        Calculates CRC over QSPI QspiUnitCfg configuration.
 *
 * @details      Calculates CRC over QspiUnitCfg items of Fls QSPI configuration set
 *               pointed to by ConfigPtr.
 *
 * @param[in]    IterUnits The index of QSPI configuration
 *
 * @return       void
 * @retval       None.
 *
 * @pre          Fls_pConfigPtr must be properly initialized.
 *
 */
static void Fls_AccumulateCRC_Qspi_paQspiUnitCfg(void)
{
    uint8 IterUnits;
    uint8 IterAhb;
    const Qspi_Ip_ControllerConfigType *QspiUnitCfg;

    /* CRC - Accumulate (*QspiUnitCfg)[] */
    for (IterUnits = 0U; IterUnits < Fls_pConfigPtr->pFlsQspiCfgConfig->u8QspiConfigCount; IterUnits++)
    {
        QspiUnitCfg = &((*(Fls_pConfigPtr->pFlsQspiCfgConfig->paQspiUnitCfg))[IterUnits]);

        /* CRC - Accumulate memSizeA1 */
        Fls_AccumulateCRC( (uint32)(QspiUnitCfg->memSizeA1) );
        /* CRC - Accumulate memSizeA2 */
        Fls_AccumulateCRC( (uint32)(QspiUnitCfg->memSizeA2) );
        /* CRC - Accumulate csHoldTime */
        Fls_AccumulateCRC( (uint32)(QspiUnitCfg->csHoldTime) );
        /* CRC - Accumulate csSetupTime */
        Fls_AccumulateCRC( (uint32)(QspiUnitCfg->csSetupTime) );
        /* CRC - Accumulate readModeA */
        Fls_AccumulateCRC( (uint32)(QspiUnitCfg->readModeA) );
        /* CRC - Accumulate sampleDelay */
        Fls_AccumulateCRC( (uint32)(QspiUnitCfg->sampleDelay) );
        /* CRC - Accumulate samplePhase */
        Fls_AccumulateCRC( (uint32)(QspiUnitCfg->samplePhase) );
        /* CRC - Accumulate dllSettings */
        Fls_AccumulateCRC_Qspi_dllSettings(QspiUnitCfg);
        for (IterAhb = 0U; IterAhb < QSPI_IP_AHB_BUFFERS; IterAhb++)
        {
            /* CRC - Accumulate masters */
            Fls_AccumulateCRC( (uint32)(QspiUnitCfg->ahbConfig.masters[IterAhb]) );
        }
        for (IterAhb = 0U; IterAhb < QSPI_IP_AHB_BUFFERS; IterAhb++)
        {
            /* CRC - Accumulate sizes */
            Fls_AccumulateCRC( (uint32)(QspiUnitCfg->ahbConfig.sizes[IterAhb]) );
        }
        /* CRC - Accumulate allMasters */
        Fls_AccumulateCRC( (uint32)(QspiUnitCfg->ahbConfig.allMasters? 1U : 0U) );


    }
}

#endif /*#if (FLS_QSPI_SECTORS_CONFIGURED == STD_ON)*/

#endif /* #if (FLS_CHECK_CFG_CRC == STD_ON) */


static boolean Fls_CheckCfgCRC(void)
{
    boolean RetVal = TRUE;

    #if (FLS_CHECK_CFG_CRC == STD_ON)
    if (Fls_pConfigPtr->u16ConfigCrc != Fls_CalcCfgCRC())
    {
        #if (FLS_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_INIT_ID, FLS_E_PARAM_CONFIG );
        #endif

        Fls_pConfigPtr = NULL_PTR;
        Fls_eJobResult = MEMIF_JOB_FAILED;
        RetVal         = FALSE;
    }
    #endif

    return RetVal;
}


#if (FLS_MCORE_ENABLED == STD_ON)
/**
 * @brief
 * @details
 *
 * @param[in]        void
 *
 * @return           Fls_MCoreReqReturnType
 * @retval           FLS_MCORE_ERROR         Error during attempt of getting exclusive access(abnormal sema4 behaviour, incorrect exclusive area implementation, etc)
 * @retval           FLS_MCORE_TIMEOUT       Timeout during attempt of getting the sema4(sema4 was not free, timeout value too small, other higher prio operation ongoing, etc). This return value is available only when timeout supervision is enabled
 * @retval           FLS_MCORE_PENDING       Sema4 not taken, exclusive access not granted yet(sema4 was not free or other higher prio operation ongoing, but timeout did not expired yet)
 * @retval           FLS_MCORE_GRANTED       Sema4 taken, exclusive access granted
 *
 *
 * @pre              The module has to be initialized and not busy.
 * @pre              The requested job has to be set.
 * @pre              The requested priority for this job has to be set(Normal, High prio).
 * @post             Status of current job is known: error,timeout -> abort job, pending -> delay job, granted -> start/continue job in hardware.
 *
 *
 *
 */
static Fls_MCoreReqReturnType Fls_ReqMCore(void)
{
    uint8 Gate = 0U;
    uint8 GetGateStatus;
    Fls_MCoreReqReturnType RetVal = FLS_MCORE_ERROR;
    Fls_HwChType eHwCh;

    /* Get channel type(INTERNAL, EXTERNAL_A1, A2,...) to determine the HW IP used(internal or external flash). */
    eHwCh = (*(Fls_pConfigPtr->paHwCh))[Fls_u32JobSectorIt];

#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
    if (FLS_CH_INTERN == eHwCh)
    {
        Gate = (uint8)FLS_MCORE_SEMA4_JOB;
    }
#endif /* (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED) */
#if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
    if (FLS_CH_QSPI == eHwCh)
    {
        Gate = (uint8)FLS_MCORE_SEMA4_QJOB;
    }
#endif /* (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */

    GetGateStatus = Rm_SemaphoreGetStatus(Gate);

    if (Fls_u8DomainID == GetGateStatus)
    {
        RetVal = FLS_MCORE_GRANTED;
    }
    else if (RM_SEMAPHORE_FREE == GetGateStatus)
    {
        /* SEMA4 is free */
        if (FLS_MCORE_HW_JOB_STARTED == Fls_MCoreHwJobStatus)
        {
            /* The job started in hardware, but the SEMA4 was cleared. This behaviour
                occurs when a cancel request was made on the other core. */
            Fls_MCoreHwJobStatus = FLS_MCORE_HW_JOB_CANCELLED;

            RetVal = FLS_MCORE_CANCELLED;
        }
        else
        {
            /* Attempt to take the SEMA4. */
            if ( (Std_ReturnType)E_OK == (Std_ReturnType)(Rm_SemaphoreLockGate(Gate)) )
            {
                /* FLS_MCORE_SEMA4_JOB taken. */
                RetVal = FLS_MCORE_GRANTED;
            }
            else
            {
                /* Due to race condition this core couldn't take the SEMA4. */
                RetVal = FLS_MCORE_PENDING;
            }
        }
    }
    else
    {
        /* The other core has the job SEMA4. */
        RetVal = FLS_MCORE_PENDING;
    }

    return RetVal;
}


static inline Fls_MCoreReqReturnType Fls_MainFunction_ReqMCore(MemIf_JobResultType *WorkResult)
{
    Fls_MCoreReqReturnType CtrlMCoreStatus = FLS_MCORE_ERROR;

#if (STD_ON == FLS_TIMEOUT_SUPERVISION_ENABLED)
    if (FLS_MCORE_HW_JOB_IDLE == Fls_MCoreHwJobStatus)
    {
        /* Load the configuration value into the multicore timeout counter. */
        Fls_u32MCoreTimeout_TimeoutTicks = OsIf_MicrosToTicks(FLS_MCORE_ARBITRATION_TIMEOUT, (OsIf_CounterType)FLS_TIMEOUT_TYPE);
        Fls_u32MCoreTimeout_CurrentTicks = OsIf_GetCounter((OsIf_CounterType)FLS_TIMEOUT_TYPE);
        Fls_u32MCoreTimeout_ElapsedTicks = 0U;
        Fls_MCoreHwJobStatus = FLS_MCORE_HW_JOB_MAINF_STARTED;
    }
#endif /* (STD_ON == FLS_TIMEOUT_SUPERVISION_ENABLED) */

    /* Fls_u8DomainID requests access to the flash controller for a Fls_eJob.*/
    CtrlMCoreStatus = Fls_ReqMCore();

    if (FLS_MCORE_ERROR == CtrlMCoreStatus)
    {
        *WorkResult = MEMIF_JOB_FAILED;
    }
    else if (FLS_MCORE_CANCELLED == CtrlMCoreStatus)
    {
        *WorkResult = MEMIF_JOB_CANCELED;
    }
    else if (FLS_MCORE_PENDING == CtrlMCoreStatus)
    {
        *WorkResult = MEMIF_JOB_PENDING;

#if (STD_ON == FLS_TIMEOUT_SUPERVISION_ENABLED)
        /* Update timer value. */
        Fls_u32MCoreTimeout_ElapsedTicks += OsIf_GetElapsed(&Fls_u32MCoreTimeout_CurrentTicks, (OsIf_CounterType)FLS_TIMEOUT_TYPE);
        /* Check if timeout value was exceeded. */
        if ((Std_ReturnType)E_NOT_OK == (Std_ReturnType)(Fls_MCoreCheckTimeout()))
        {
            /* Mark job as failed. */
            *WorkResult = MEMIF_JOB_FAILED;
            Fls_eLLDJob = FLASH_JOB_NONE;
            Fls_eLLDJobResult = MEMIF_JOB_FAILED;
            /* Report timeout runtime error. */
            (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_ID, FLS_E_TIMEOUT);
            /* Call FlsMCoreTimeoutNotification function if configured */
            Fls_MCoreNotifyTimeout();
        }
#endif /* (STD_ON == FLS_TIMEOUT_SUPERVISION_ENABLED) */

    }
    else if (FLS_MCORE_GRANTED == CtrlMCoreStatus)
    {
        /* Requesting was success */
    }

    return CtrlMCoreStatus;
}


static void Fls_MainFunction_ReleaseMCore(MemIf_JobResultType *WorkResult)
{
    if (MEMIF_JOB_PENDING != *WorkResult)
    {
        /* Clear multicore hardware status. */
        Fls_MCoreHwJobStatus = FLS_MCORE_HW_JOB_IDLE;
        /* Release any sema4 used for processing this job in multicore context, if any was used. */
        if ( (Std_ReturnType)E_OK != (Std_ReturnType)(Fls_MCoreJobSema4Release()))
        {
            /* Release is not successful */
            *WorkResult = MEMIF_JOB_FAILED;
        }
    }
}


static Std_ReturnType Fls_MCoreInitSema4sLock(void)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;

#if (STD_ON == FLS_TIMEOUT_SUPERVISION_ENABLED)
    /* Load the initialization timeout value. */
    Fls_u32MCoreTimeout_TimeoutTicks = OsIf_MicrosToTicks(FLS_MCORE_INIT_TIMEOUT, (OsIf_CounterType)FLS_TIMEOUT_TYPE);
    Fls_u32MCoreTimeout_CurrentTicks = OsIf_GetCounter((OsIf_CounterType)FLS_TIMEOUT_TYPE);
    Fls_u32MCoreTimeout_ElapsedTicks = 0U;
#endif /* (STD_ON == FLS_TIMEOUT_SUPERVISION_ENABLED) */

#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
#if (STD_ON == FLS_TIMEOUT_SUPERVISION_ENABLED)
    /* Wait until SEMA4 is taken or timeout is reached. */
    while ((Std_ReturnType)E_OK != (Std_ReturnType)(Rm_SemaphoreLockGate((uint8)FLS_MCORE_SEMA4_JOB)) && (E_OK == Fls_MCoreCheckTimeout()))
    {
        Fls_u32MCoreTimeout_ElapsedTicks += OsIf_GetElapsed(&Fls_u32MCoreTimeout_CurrentTicks, (OsIf_CounterType)FLS_TIMEOUT_TYPE);
    }
#else /* (STD_ON == FLS_TIMEOUT_SUPERVISION_ENABLED) */
    while ((Std_ReturnType)E_OK != (Std_ReturnType)(Rm_SemaphoreLockGate((uint8)FLS_MCORE_SEMA4_JOB)))
    {
    }
#endif /* (STD_ON == FLS_TIMEOUT_SUPERVISION_ENABLED) */
#endif /* (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED) */

#if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
#if (STD_ON == FLS_TIMEOUT_SUPERVISION_ENABLED)
    /* Wait until SEMA4 is taken or timeout is reached. */
    while (((Std_ReturnType)E_OK != (Std_ReturnType)(Rm_SemaphoreLockGate((uint8)FLS_MCORE_SEMA4_QJOB))) && ((Std_ReturnType)E_OK == (Std_ReturnType)(Fls_MCoreCheckTimeout())))
    {
        Fls_u32MCoreTimeout_ElapsedTicks += OsIf_GetElapsed(&Fls_u32MCoreTimeout_CurrentTicks, (OsIf_CounterType)FLS_TIMEOUT_TYPE);
    }
#else /* (STD_ON == FLS_TIMEOUT_SUPERVISION_ENABLED) */
    while ((Std_ReturnType)E_OK != (Std_ReturnType)(Rm_SemaphoreLockGate((uint8)FLS_MCORE_SEMA4_QJOB)))
    {
    }
#endif /* (STD_ON == FLS_TIMEOUT_SUPERVISION_ENABLED) */
#endif /* (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */
#if (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON)
    /* Check if the timeout value was exceeded. */
    RetVal = Fls_MCoreCheckTimeout();
#endif /* FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON */

    return RetVal;
}

/* Cleanup function, called: at the end of the job(successful or erroneous) and in canceled jobs, to clean all sema4s acquired by this core.*/
static Std_ReturnType Fls_MCoreJobSema4Release(void)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;

#if (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED)
    /* If the internal job SEMA4 is held by this core, clear it. */
    if (Fls_u8DomainID == Rm_SemaphoreGetStatus((uint8)FLS_MCORE_SEMA4_JOB))
    {
        if ( (Std_ReturnType)E_OK != (Std_ReturnType)(Rm_SemaphoreUnlockGate((uint8)FLS_MCORE_SEMA4_JOB)) )
        {
            RetVal = E_NOT_OK;
        }
    }
    else
    {
        /* if other cores are still ongoing */
        if (FLS_MCORE_HW_JOB_STARTED == Fls_MCoreHwJobStatus)
        {
            /* Release is not successful because other cores are still taking sema4 */
            RetVal = E_NOT_OK;
        }
    }
#endif /* (STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED) */
#if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
    /* If the external job SEMA4 is held by this core, clear it. */
    if (Fls_u8DomainID == Rm_SemaphoreGetStatus((uint8)FLS_MCORE_SEMA4_QJOB))
    {
        if ( (Std_ReturnType)E_OK != (Std_ReturnType)(Rm_SemaphoreUnlockGate((uint8)FLS_MCORE_SEMA4_QJOB)))
        {
            RetVal = E_NOT_OK;
        }
    }
    else
    {
        /* if other cores are still ongoing */
        if (FLS_MCORE_HW_JOB_STARTED == Fls_MCoreHwJobStatus)
        {
            /* Release is not successful because other cores are still taking sema4 */
            RetVal = E_NOT_OK;
        }
    }
#endif /* (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */

    return RetVal;
}

#if (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON)
static Std_ReturnType Fls_MCoreCheckTimeout(void)
{
    /* save the return status */
    Std_ReturnType RetVal = (Std_ReturnType)E_NOT_OK;
    /* if ElapsedTicks < Timeout ticks */
    if (Fls_u32MCoreTimeout_ElapsedTicks < Fls_u32MCoreTimeout_TimeoutTicks)
    {
        RetVal = E_OK;
    }
    else
    {
        RetVal = E_NOT_OK;
    }

    return RetVal;
}

static void Fls_MCoreNotifyTimeout(void)
{
    Fls_MCoreTimeoutJobType TimeoutJob = FLS_MCORE_TIMEOUT_ERASE;

    /* Callback notification was configured */
    if (NULL_PTR != Fls_pConfigPtr->MCoreTimeoutNotifPtr)
    {
        /* Check the job which timeout occurred */
        switch (Fls_eJob)
        {
            case FLS_JOB_ERASE:
                TimeoutJob = FLS_MCORE_TIMEOUT_ERASE;
                break;

            case FLS_JOB_WRITE:
                TimeoutJob = FLS_MCORE_TIMEOUT_WRITE;
                break;

            case FLS_JOB_READ:
                TimeoutJob = FLS_MCORE_TIMEOUT_READ;
                break;

            case FLS_JOB_COMPARE:
                TimeoutJob = FLS_MCORE_TIMEOUT_COMPARE;
                break;

            case FLS_JOB_BLANK_CHECK:
                TimeoutJob = FLS_MCORE_TIMEOUT_BLANK_CHECK;
                break;

            default:
                /* Do nothing - 'default' clause added to fulfil MISRA Rule 15.3 */
                break;
        }

        /* Call FlsMCoreTimeoutNotification function */
        Fls_pConfigPtr->MCoreTimeoutNotifPtr(TimeoutJob);
    }
}
#endif /* #if (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON) */
#endif /* #if (FLS_MCORE_ENABLED == STD_ON)*/

/**
 * @brief   Decides which sub-function to call based on the job type: Modify (Write, Erase) or Read (Read, Compare).
 **/
static MemIf_JobResultType Fls_ProcessRequestedJobs( void )
{
    MemIf_JobResultType WorkResult;

    if (FLS_JOB_ERASE == Fls_eJob)
    {
        /**** MODIFY jobs ****/
        /* Start current job or start a new batch of current job or verify status of current job. */
        WorkResult = Fls_DoJobErase();
    }
    else if (FLS_JOB_WRITE == Fls_eJob)
    {
        WorkResult = Fls_DoJobDataTransfer(Fls_u32MaxWrite);
    }
    else
    {
        /**** READ jobs ****/
        /* FLS_JOB_READ, FLS_JOB_BLANK_CHECK and FLS_JOB_COMPARE jobs are always synchronous */
        WorkResult = Fls_DoJobDataTransfer(Fls_u32MaxRead);
    }

    return WorkResult;
}

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/

/**
 * @brief        The function initializes Fls module.
 * @details      The function sets the internal module variables according to given
 *               configuration set.
 *
 * @param[in]    ConfigPtr        Pointer to flash driver configuration set.
 *
 * @api
 *
 * @pre          @p ConfigPtr must not be @p NULL_PTR and the module status must not
 *               be @p MEMIF_BUSY.
 *
 * @implements     Fls_Init_Activity
 *
 */
void Fls_Init(const Fls_ConfigType * ConfigPtr)
{
#if ( FLS_DEV_ERROR_DETECT == STD_ON )
#ifdef FLS_PRECOMPILE_SUPPORT
    if (NULL_PTR != ConfigPtr)
#else
    if (NULL_PTR == ConfigPtr)
#endif /* FLS_PRECOMPILE_SUPPORT */
    {
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_INIT_ID, FLS_E_PARAM_CONFIG );
    }
    else if (MEMIF_JOB_PENDING == Fls_eJobResult)
    {
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_INIT_ID, FLS_E_BUSY );
    }
    else
    {
#endif /* FLS_DEV_ERROR_DETECT == STD_ON */

    #ifdef FLS_PRECOMPILE_SUPPORT
        Fls_pConfigPtr = &Fls_Config;
        (void)ConfigPtr;
    #else
        Fls_pConfigPtr = ConfigPtr;
    #endif /* <DRIVER>_PRECOMPILE_SUPPORT */

        if (TRUE == Fls_CheckCfgCRC())
        {
            /* Set the max number of bytes to read/write
                during Fls_MainFunction call */
            if (MEMIF_MODE_SLOW == Fls_pConfigPtr->eDefaultMode)
            {
                Fls_u32MaxRead = Fls_pConfigPtr->u32MaxReadNormalMode;
                Fls_u32MaxWrite = Fls_pConfigPtr->u32MaxWriteNormalMode;
            }
            else
            {
                Fls_u32MaxRead = Fls_pConfigPtr->u32MaxReadFastMode;
                Fls_u32MaxWrite = Fls_pConfigPtr->u32MaxWriteFastMode;
            }
#if (FLS_MCORE_ENABLED == STD_ON)
            /* Set up the DomainID for further operations. */
            Fls_u8DomainID = (uint8)Rm_XrdcGetDomainID(0U);
            /* Take the job SEMA4s to ensure that no operation is in progress during the hardware initialization. */
            if ((Std_ReturnType)E_OK == Fls_MCoreInitSema4sLock())
            {
#endif /* #if (FLS_MCORE_ENABLED == STD_ON)*/
                /* Initialize flash hardware
                   NOTE: The variable 'Fls_eLLDJobResult' will be updated in the below function*/
                Fls_IPW_Init();
#if (FLS_MCORE_ENABLED == STD_ON)
            }
            else
            {
#if (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON)
                (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_INIT_ID, FLS_E_TIMEOUT);
#endif  /* #if (FLS_TIMEOUT_SUPERVISION_ENABLED == STD_ON) */
                /* Unlock is not successful */
                Fls_pConfigPtr = NULL_PTR;
                Fls_eJobResult = MEMIF_JOB_FAILED;
            }

            /* Release the taken SEMA4s. */
            if ((Std_ReturnType)E_OK != (Std_ReturnType)(Fls_MCoreJobSema4Release()))
            {
                /* Release is not successful */
                Fls_pConfigPtr = NULL_PTR;
                Fls_eJobResult = MEMIF_JOB_FAILED;
            }
#endif /* #if (FLS_MCORE_ENABLED == STD_ON)*/

            if (MEMIF_JOB_FAILED == Fls_LLDGetJobResult())
            {
                Fls_eJobResult = MEMIF_JOB_FAILED;
            #if ( FLS_DEV_ERROR_DETECT == STD_ON )
                Fls_pConfigPtr = NULL_PTR;
            #endif
            }
            else
            {
                Fls_eJobResult = MEMIF_JOB_OK;
            #if (STD_ON == FLS_AC_LOAD_ON_JOB_START)
                /* Get block number of ACWrite/Erase function address */
                FLs_u8blockAcWrite = Fls_IPW_GetBlockNumberFromAddress((Fls_AddressType)(&Fls_ACWriteRomStart));
                FLs_u8blockAcErase = Fls_IPW_GetBlockNumberFromAddress((Fls_AddressType)(&Fls_ACEraseRomStart));
            #endif /* #if (STD_ON == FLS_AC_LOAD_ON_JOB_START) */
            }
        }
    #if (FLS_DEV_ERROR_DETECT == STD_ON)
    }
    #endif
}

/**
 * @brief            Erase one or more complete flash sectors.
 * @details          Starts an erase job asynchronously. The actual job is performed
 *                   by the @p Fls_MainFunction.
 *
 * @param[in]        TargetAddress        Target address in flash memory.
 * @param[in]        Length               Number of bytes to erase.
 *
 * @return           Std_ReturnType
 * @retval           E_OK                    Erase command has been accepted.
 * @retval           E_NOT_OK                Erase command has not been accepted.
 *
 * @api
 *
 * @pre              The module has to be initialized and not busy.
 * @post             @p Fls_Erase changes module status and some internal variables
 *                   (@p Fls_u32JobSectorIt, @p Fls_u32JobSectorEnd, @p Fls_Job,
 *                   @p Fls_eJobResult).
 *
 * @implements       Fls_Erase_Activity
 *
 */
Std_ReturnType Fls_Erase(Fls_AddressType TargetAddress,
                         Fls_LengthType Length
                        )
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;
    Fls_SectorIndexType TmpJobSectorIt;
    Fls_SectorIndexType TmpJobSectorEnd;

#if ( FLS_DEV_ERROR_DETECT == STD_ON )
    if ( NULL_PTR == Fls_pConfigPtr )
    {
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_ERASE_ID, FLS_E_UNINIT );
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    else if ( (boolean)FALSE == (Fls_IsAddrSectorStartAligned( (Fls_AddressType)TargetAddress )) )
    {
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_ERASE_ID, FLS_E_PARAM_ADDRESS );
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    else if (( 0U == Length ) || ( (boolean)FALSE == (Fls_IsAddrSectorEndAligned(( (Fls_AddressType)TargetAddress + Length ) - 1U ))) )
    {
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_ERASE_ID, FLS_E_PARAM_LENGTH );
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    else
#endif    /* FLS_DEV_ERROR_DETECT == STD_ON */
    {

        /* Perform calculations outside the critical section in order
            to limit time spend in the CS */
        TmpJobSectorIt = Fls_GetSectorIndexByAddr( TargetAddress );
        TmpJobSectorEnd = Fls_GetSectorIndexByAddr(( TargetAddress + Length ) - 1U );

        SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_10();

        if (MEMIF_JOB_PENDING == Fls_eJobResult)
        {
#if ( FLS_DEV_ERROR_DETECT == STD_ON )
            (void) Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_ERASE_ID, FLS_E_BUSY );
#endif    /* FLS_DEV_ERROR_DETECT == STD_ON */
            RetVal = (Std_ReturnType)E_NOT_OK;

            SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_10();
        }
        else
        {
            /* Configure the erase job */
            Fls_u32JobSectorIt  = TmpJobSectorIt;
            Fls_u32JobSectorEnd = TmpJobSectorEnd;
            Fls_u32JobAddrIt    = TargetAddress;
            Fls_u32JobAddrEnd   = (TargetAddress + Length) - 1U;
            Fls_eJob            = FLS_JOB_ERASE;
            Fls_u8JobStart      = 1U;
            /* Execute the erase job */
            Fls_eJobResult      = MEMIF_JOB_PENDING;

            SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_10();
        }
    }

    return RetVal;
}


/**
 * @brief            Write one or more complete flash pages to the flash device.
 * @details          Starts a write job asynchronously. The actual job is performed by
 *                   @p Fls_MainFunction.
 *
 * @param[in]        TargetAddress        Target address in flash memory. This address offset will be added to the flash memory base address.
 * @param[in]        SourceAddressPtr    Pointer to source data buffer.
 * @param[in]        Length               Number of bytes to write.
 *
 * @return           Std_ReturnType
 * @retval           E_OK                 Write command has been accepted.
 * @retval           E_NOT_OK             Write command has not been accepted.
 *
 * @api
 *
 * @pre              The module has to be initialized and not busy.
 * @post             @p Fls_Write changes module status and some internal variables
 *                  (@p Fls_u32JobSectorIt, @p Fls_u32JobAddrIt, @p Fls_u32JobAddrEnd,
 *                  @p Fls_pJobDataSrcPtr, @p Fls_eJob, @p Fls_eJobResult).
 *
 * @implements       Fls_Write_Activity
 *
 */
Std_ReturnType Fls_Write(Fls_AddressType TargetAddress,
                         const uint8 * SourceAddressPtr,
                         Fls_LengthType Length
                        )
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;
    Fls_SectorIndexType TmpJobSectorIt;
    Fls_AddressType TmpJobAddrEnd;

#if ( FLS_DEV_ERROR_DETECT == STD_ON )
    if ( NULL_PTR == Fls_pConfigPtr )
    {
        /* Report Error if Fls_pConfigPtr = NULL_PTR  */
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_WRITE_ID, FLS_E_UNINIT );
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    else if ( (boolean)FALSE == (Fls_IsAddrPageStartAligned( (Fls_AddressType)TargetAddress )))
    {
        /* Report Error if TargetAddress is not aligned */
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_WRITE_ID, FLS_E_PARAM_ADDRESS );
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    else if (( 0U == Length ) || ( (boolean)FALSE == (Fls_IsAddrPageEndAligned( TargetAddress +  Length ) )))
    {
        /* Report Error if TargetAddress +  Length is not aligned */
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_WRITE_ID, FLS_E_PARAM_LENGTH );
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    else if ( NULL_PTR == SourceAddressPtr )
    {
        /* Report Error if SourceAddressPtr = NULL_PTR */
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_WRITE_ID, FLS_E_PARAM_DATA );
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    else
#endif    /* FLS_DEV_ERROR_DETECT == STD_ON */
    {

        /* Perform calculations outside the critical section in order
            to limit time spend in the CS */
        TmpJobSectorIt = Fls_GetSectorIndexByAddr(TargetAddress);
        TmpJobAddrEnd = ( TargetAddress + Length ) - 1U;
        /* Start of exclusive area. Implementation depends on integrator. */
        SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_11();

        if (MEMIF_JOB_PENDING == Fls_eJobResult)
        {
#if ( FLS_DEV_ERROR_DETECT == STD_ON )
            (void) Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_WRITE_ID, FLS_E_BUSY );
#endif    /* FLS_DEV_ERROR_DETECT == STD_ON */
            RetVal = (Std_ReturnType)E_NOT_OK;
            /* End of exclusive area. Implementation depends on integrator. */
            SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_11();
        }
        else
        {
            /* Configure the write job */
            Fls_u32JobSectorIt  = TmpJobSectorIt;
            Fls_u32JobSectorEnd = Fls_GetSectorIndexByAddr(TmpJobAddrEnd);
            Fls_u32JobAddrEnd   = TmpJobAddrEnd;
            Fls_u32JobAddrIt    = TargetAddress;
            Fls_pJobDataSrcPtr  = SourceAddressPtr;
            Fls_eJob            = FLS_JOB_WRITE;
            Fls_u8JobStart      = 1U;
            /* Execute the write job */
            Fls_eJobResult      = MEMIF_JOB_PENDING;
            /* End of exclusive area. Implementation depends on integrator. */
            SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_11();
        }
    }

    return RetVal;
}


#if ( FLS_CANCEL_API == STD_ON ) || defined(__DOXYGEN__)
/**
 * @brief            Cancel an ongoing flash read, write, erase or compare job.
 * @details          Abort a running job synchronously so that directly after returning
 *                   from this function a new job can be started.
 *
 * @api
 *
 * @pre              The module must be initialized.
 * @post             @p Fls_Cancel changes module status and @p Fls_eJobResult
 *                   internal variable.
 *
 * @implements       Fls_Cancel_Activity
 *
 */
void Fls_Cancel(void)
{
#if (FLS_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == Fls_pConfigPtr)
    {
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_CANCEL_ID, FLS_E_UNINIT );
    }
    else
    {
#endif    /* FLS_DEV_ERROR_DETECT == STD_ON */
        if (MEMIF_JOB_PENDING == Fls_eJobResult)
        {
            /* Cancel ongoing hardware job */
            Fls_IPW_Cancel();
        #if ( STD_ON == FLS_AC_LOAD_ON_JOB_START )
            if (TRUE == Fls_bACloaded)
            {
                Fls_UnloadAc(Fls_eJob);
                Fls_bACloaded = FALSE;
            }
            else
            {
                /* The FLS_JOB_READ and FLS_JOB_COMPARE jobs don't use the access code */
            }
        #endif /* #if STD_ON == FLS_AC_LOAD_ON_JOB_START */
            /* If underlying hardware job was successfully cancelled */
            if (MEMIF_JOB_CANCELED == Fls_LLDGetJobResult())
            {
                /* Update global job result status */
                Fls_eJobResult = MEMIF_JOB_CANCELED;
            }

            if (NULL_PTR != Fls_pConfigPtr->jobErrorNotificationPtr)
            {
                /* Call FlsJobErrorNotification function if configured */
                Fls_pConfigPtr->jobErrorNotificationPtr();
            }
            else
            {
                /* Callback notification configured as null pointer */
            }
#if (FLS_MCORE_ENABLED == STD_ON)
            /* Release any sema4 used for processing this job in multicore context, if any was used. */
            if ((Std_ReturnType)E_OK != (Std_ReturnType)(Fls_MCoreJobSema4Release()))
            {
                /* Release is not successful */
                Fls_eJobResult = MEMIF_JOB_FAILED;
            }
            Fls_MCoreHwJobStatus = FLS_MCORE_HW_JOB_IDLE;
#endif /* #if (FLS_MCORE_ENABLED == STD_ON)*/
        }
        else
        {
            /* Leave the job result unchanged */
        }
#if ( FLS_DEV_ERROR_DETECT == STD_ON )
    }
#endif    /* FLS_DEV_ERROR_DETECT == STD_ON */
}
#endif    /* FLS_CANCEL_API == STD_ON */


#if (( FLS_GET_STATUS_API == STD_ON ) || defined (__DOXYGEN__))
/**
 * @brief            Returns the FLS module status.
 * @details          Returns the FLS module status synchronously.
 *
 * @return           MemIf_StatusType
 * @retval           MEMIF_UNINIT        Module has not been initialized (yet).
 * @retval           MEMIF_IDLE          Module is currently idle.
 * @retval           MEMIF_BUSY          Module is currently busy.
 *
 * @api
 *
 * @implements       Fls_GetStatus_Activity
 *
 */
MemIf_StatusType Fls_GetStatus(void)
{
    MemIf_StatusType RetVal;

    /* if the Fls_pConfigPtr = NULL_PTR */
    if (NULL_PTR == Fls_pConfigPtr)
    {
        RetVal = MEMIF_UNINIT;
    }
    else if (MEMIF_JOB_PENDING == Fls_eJobResult)
    {
        /* return MEMIF_BUSY if the other job is in progress */
        RetVal = MEMIF_BUSY;
    }
    else
    {
        /* return MEMIF_IDLE if no job  */
        RetVal = MEMIF_IDLE;
    }

    return RetVal;
}
#endif    /* FLS_GET_STATUS_API == STD_ON */


#if ( FLS_GET_JOB_RESULT_API == STD_ON )||  defined (__DOXYGEN__)
/**
 * @brief            Returns the result of the last job.
 * @details          Returns synchronously the result of the last job.
 *
 * @return           MemIf_JobResultType
 * @retval           MEMIF_JOB_OK              Successfully completed job.
 * @retval           MEMIF_JOB_FAILED          Not successfully completed job.
 * @retval           MEMIF_JOB_PENDING         Still pending job (not yet completed).
 * @retval           MEMIF_JOB_CANCELED        Job has been cancelled.
 * @retval           MEMIF_BLOCK_INCONSISTENT  Inconsistent block requested, it may
 *                                             contains corrupted data.
 * @retval           MEMIF_BLOCK_INVALID       Invalid block requested.
 *
 * @api
 *
 * @implements       Fls_GetJobResult_Activity
 *
 */
MemIf_JobResultType Fls_GetJobResult( void )
{
    MemIf_JobResultType RetVal;

#if ( FLS_DEV_ERROR_DETECT == STD_ON )
    if ( NULL_PTR == Fls_pConfigPtr )
    {
        (void) Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_GETJOBRESULT_ID, FLS_E_UNINIT );
        RetVal = MEMIF_JOB_FAILED;
    }
    else
#endif    /* FLS_DEV_ERROR_DETECT == STD_ON */
    {
        RetVal = Fls_eJobResult;
    }
    return RetVal;
}
#endif    /* FLS_GET_JOB_RESULT_API == STD_ON */


/**
 * @brief            Reads from flash memory.
 * @details          Starts a read job asynchronously. The actual job is performed by
 *                   @p Fls_MainFunction.
 *
 * @param[in]        SourceAddress        Source address in flash memory. This address offset will be added to the flash memory base address.
 * @param[in]        Length               Number of bytes to read.
 * @param[out]       TargetAddressPtr     Pointer to target data buffer.
 *
 * @return           Std_ReturnType
 * @retval           E_OK                 Read command has been accepted.
 * @retval           E_NOT_OK             Read command has not been accepted.
 *
 * @api
 *
 * @pre            The module has to be initialized and not busy.
 * @post           @p Fls_Read changes module status and some internal variables
 *                 (@p Fls_u32JobSectorIt, @p Fls_u32JobAddrIt, @p Fls_u32JobAddrEnd,
 *                 @p Fls_pJobDataDestPtr, @p Fls_eJob, @p Fls_eJobResult).
 *
 * @implements       Fls_Read_Activity
 *
 */
Std_ReturnType Fls_Read(Fls_AddressType SourceAddress,
                        uint8 * TargetAddressPtr,
                        Fls_LengthType Length
                       )
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;
    Fls_SectorIndexType TmpJobSectorIt = 0UL;
    Fls_AddressType TmpJobAddrEnd = 0UL;

#if ( FLS_DEV_ERROR_DETECT == STD_ON )
    RetVal = Fls_CheckInputParamReadCompareJob(SourceAddress, TargetAddressPtr, Length, FLS_READ_ID);
    if ((Std_ReturnType)E_OK == RetVal)
#endif    /* FLS_DEV_ERROR_DETECT == STD_ON */
    {

        /* Perform calculations outside the critical section in order
            to limit time spend in the CS */
        TmpJobSectorIt = Fls_GetSectorIndexByAddr( SourceAddress );
        TmpJobAddrEnd = ( SourceAddress + Length ) - 1U;
        /* Start of exclusive area. Implementation depends on integrator. */
        SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_12();

        if (MEMIF_JOB_PENDING == Fls_eJobResult)
        {
#if ( FLS_DEV_ERROR_DETECT == STD_ON )
            (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_READ_ID, FLS_E_BUSY );
#endif    /* FLS_DEV_ERROR_DETECT == STD_ON */
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
        else
        {
            /* Configure the read job */
            Fls_u32JobSectorIt  = TmpJobSectorIt;
            Fls_u32JobSectorEnd = Fls_GetSectorIndexByAddr(TmpJobAddrEnd);
            Fls_u32JobAddrIt    = SourceAddress;
            Fls_u32JobAddrEnd   = TmpJobAddrEnd;
            Fls_pJobDataDestPtr = TargetAddressPtr;
            Fls_eJob            = FLS_JOB_READ;

            Fls_u8JobStart      = 1U;

            /* Execute the read job */
            Fls_eJobResult      = MEMIF_JOB_PENDING;

        }
        /* End of exclusive area. Implementation depends on integrator. */
        SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_12();
    }

    return RetVal;
}

#if ( FLS_COMPARE_API == STD_ON ) || defined(__DOXYGEN__)
/**
 * @brief           Compares a flash memory area with an application data buffer.
 * @details         Starts a compare job asynchronously. The actual job is performed by
 *                  @p Fls_MainFunction.
 *
 * @param[in]        SourceAddress          Source address in flash memory.
 * @param[in]        TargetAddressPtr        Pointer to source data buffer.
 * @param[in]        Length                 Number of bytes to compare.
 *
 * @return           Std_ReturnType
 * @retval           E_OK                      Compare command has been accepted.
 * @retval           E_NOT_OK                   Compare command has not been accepted.
 *
 * @api
 *
 * @pre            The module has to be initialized and not busy.
 * @post           @p Fls_Read changes module status and some internal variables
 *                (@p Fls_u32JobSectorIt, @p Fls_u32JobAddrIt, @p Fls_u32JobAddrEnd,
 *                @p Fls_pJobDataSrcPtr, @p Fls_eJob, @p Fls_eJobResult).
 *
 * @implements       Fls_Compare_Activity
 *
 */
Std_ReturnType Fls_Compare(Fls_AddressType SourceAddress,
                           const uint8 * TargetAddressPtr,
                           Fls_LengthType Length
                          )
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;
    Fls_SectorIndexType TmpJobSectorIt;
    Fls_AddressType TmpJobAddrEnd;

#if ( FLS_DEV_ERROR_DETECT == STD_ON )
    RetVal = Fls_CheckInputParamReadCompareJob(SourceAddress, TargetAddressPtr, Length, FLS_COMPARE_ID);
    if ((Std_ReturnType)E_OK == RetVal)
#endif    /* FLS_DEV_ERROR_DETECT == STD_ON */
    {

        /* Perform calculations outside the critical section in order to limit time spend in the CS */
        TmpJobSectorIt = Fls_GetSectorIndexByAddr( SourceAddress );
        TmpJobAddrEnd = ( SourceAddress + Length ) - 1U;
        /* Start of exclusive area. Implementation depends on integrator. */
        SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_13();

        if (MEMIF_JOB_PENDING == Fls_eJobResult)
        {
#if ( FLS_DEV_ERROR_DETECT == STD_ON )
            (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_COMPARE_ID, FLS_E_BUSY );
#endif    /* FLS_DEV_ERROR_DETECT == STD_ON */
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
        else
        {
            Fls_u32JobSectorIt  = TmpJobSectorIt;
            Fls_u32JobSectorEnd = Fls_GetSectorIndexByAddr(TmpJobAddrEnd);
            Fls_u32JobAddrIt    = SourceAddress;
            Fls_u32JobAddrEnd   = TmpJobAddrEnd;
            Fls_pJobDataSrcPtr  = TargetAddressPtr;
            Fls_eJob            = FLS_JOB_COMPARE;
            Fls_u8JobStart      = 1U;

            /* Execute the read job */
            Fls_eJobResult      = MEMIF_JOB_PENDING;

        }
        /* End of exclusive area. Implementation depends on integrator. */
        SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_13();
    }

    return RetVal;
}
#endif /* FLS_COMPARE_API == STD_ON */

#if (FLS_BLANK_CHECK_API == STD_ON)
/**
 * @brief           Verify whether a given memory area has been erased but not (yet) programmed.
 * @details         Starts a compare job asynchronously. The actual job is performed by
 *                  @p Fls_MainFunction.
 *
 * @param[in]        TargetAddress          Address in flash memory from which the blank check should be started.
 * @param[in]        Length                 Number of bytes to be checked for erase pattern.
 *
 * @return           Std_ReturnType
 * @retval           E_OK                   Blank checking command has been accepted.
 * @retval           E_NOT_OK               Blank checking command has not been accepted.
 *
 * @api
 *
 * @pre              The module has to be initialized and not busy.
 * @post             @p Fls_Read changes module status and some internal variables
 *                   (@p Fls_u32JobSectorIt, @p Fls_u32JobAddrIt, @p Fls_u32JobAddrEnd,
 *                   @p Fls_pJobDataSrcPtr, @p Fls_eJob, @p Fls_eJobResult).
 *
 * @implements       Fls_BlankCheck_Activity
 *
 */
Std_ReturnType Fls_BlankCheck(Fls_AddressType TargetAddress,
                              Fls_LengthType Length
                             )
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;
    Fls_SectorIndexType TmpJobSectorIt = 0UL;
    Fls_AddressType TmpJobAddrEnd = 0UL;

#if ( FLS_DEV_ERROR_DETECT == STD_ON )
    if (NULL_PTR == Fls_pConfigPtr)
    {
        /* Report Error if Fls_pConfigPtr = NULL_PTR  */
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_BLANK_CHECK_ID, FLS_E_UNINIT );
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    else if (TargetAddress >= FLS_TOTAL_SIZE)
    {
        /* Report Error if TargetAddress is not aligned */
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_BLANK_CHECK_ID, FLS_E_PARAM_ADDRESS );
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    else if ( (0U == Length) || ((TargetAddress + Length) > FLS_TOTAL_SIZE) )
    {
        /* Report Error if Length is invalid */
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_BLANK_CHECK_ID, FLS_E_PARAM_LENGTH );
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    #if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED)
    else if ( (boolean)FALSE == Fls_IsAddrWordAligned(TargetAddress) )
    {
        /* Report Error if TargetAddress is not aligned */
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_BLANK_CHECK_ID, FLS_E_PARAM_ADDRESS );
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    else if ( (boolean)FALSE == Fls_IsAddrWordAligned(TargetAddress + Length) )
    {
        /* Report Error if TargetAddress + Length is not aligned */
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_BLANK_CHECK_ID, FLS_E_PARAM_LENGTH );
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    #endif /* #if (STD_ON == FLS_QSPI_SECTORS_CONFIGURED) */
    else
#endif    /* FLS_DEV_ERROR_DETECT == STD_ON */
    {
        /* Perform calculations outside the critical section in order
            to limit time spend in the CS */
        TmpJobSectorIt = Fls_GetSectorIndexByAddr(TargetAddress);
        TmpJobAddrEnd  = (TargetAddress + Length) - 1U;
        /* Start of exclusive area. Implementation depends on integrator. */
        SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_14();

        if (MEMIF_JOB_PENDING == Fls_eJobResult)
        {
#if ( FLS_DEV_ERROR_DETECT == STD_ON )
            (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_BLANK_CHECK_ID, FLS_E_BUSY );
#endif    /* FLS_DEV_ERROR_DETECT == STD_ON */
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
        else
        {
            Fls_u32JobSectorIt = TmpJobSectorIt;
            Fls_u32JobAddrIt   = TargetAddress;
            Fls_u32JobAddrEnd  = TmpJobAddrEnd;
            Fls_eJob           = FLS_JOB_BLANK_CHECK;
            Fls_u8JobStart     = 1U;

            /* Execute the read job */
            Fls_eJobResult     = MEMIF_JOB_PENDING;
        }
        /* End of exclusive area. Implementation depends on integrator. */
        SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_14();
    }

    return RetVal;
}
#endif /* FLS_BLANK_CHECK_API == STD_ON */

#if ( FLS_SET_MODE_API == STD_ON )|| defined (__DOXYGEN__)
/**
 * @brief           Sets the FLS module's operation mode to the given Mode.
 * @details         Every given mode determines maximum bytes for read-write
 *                  operations. Every mode has a set of pre-configured values.
 *
 * @param[in]        Mode        MEMIF_MODE_FAST or MEMIF_MODE_SLOW.
 *
 * @api
 *
 * @pre            The module has to be initialized and not busy.
 * @post           @p Fls_SetMode changes internal variables @p Fls_u32MaxRead and
 *                 @p Fls_u32MaxWrite.
 *
 * @implements       Fls_SetMode_Activity
 *
 */
void Fls_SetMode(MemIf_ModeType Mode)
{
#if ( FLS_DEV_ERROR_DETECT == STD_ON )
    if (NULL_PTR == Fls_pConfigPtr)
    {
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_SETMODE_ID, FLS_E_UNINIT );
    }
    else if (MEMIF_JOB_PENDING == Fls_eJobResult)
    {
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_SETMODE_ID, FLS_E_BUSY );
    }
    else
    {
#endif  /* FLS_DEV_ERROR_DETECT == STD_ON */
        /* Set the max number of bytes to read/write
            during Fls_MainFunction call */
        switch (Mode)
        {
            case MEMIF_MODE_FAST:
                Fls_u32MaxRead  = Fls_pConfigPtr->u32MaxReadFastMode;
                Fls_u32MaxWrite = Fls_pConfigPtr->u32MaxWriteFastMode;
                break;

            case MEMIF_MODE_SLOW:
                Fls_u32MaxRead  = Fls_pConfigPtr->u32MaxReadNormalMode;
                Fls_u32MaxWrite = Fls_pConfigPtr->u32MaxWriteNormalMode;
                break;

            default:
                ; /* Do nothing - should not happen in Fully Trusted Environment;
                   'default' clause added to fulfil MISRA Rule 15.3 */
                break;
        }
#if ( FLS_DEV_ERROR_DETECT == STD_ON )
    }
#endif    /* FLS_DEV_ERROR_DETECT == STD_ON */
}
#endif /* FLS_SET_MODE_API == STD_ON */

#if ( FLS_VERSION_INFO_API == STD_ON )|| defined(__DOXYGEN__)
/**
 * @brief        Returns version information about FLS module.
 * @details      Version information includes:
 *               - Module Id
 *               - Vendor Id
 *               - Vendor specific version numbers (BSW00407).
 *
 * @param[in,out] VersionInfoPtr  Pointer to where to store the version information of this module.
 *
 * @api
 *
 * @implements       Fls_GetVersionInfo_Activity
 *
 */
void Fls_GetVersionInfo(Std_VersionInfoType * VersionInfoPtr)
{
#if ( FLS_DEV_ERROR_DETECT == STD_ON )
    if (NULL_PTR == VersionInfoPtr)
    {
        (void)Det_ReportError( (uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_GETVERSIONINFO_ID, FLS_E_PARAM_POINTER );
    }
    else
    {
#endif /* FLS_DEV_ERROR_DETECT == STD_ON */
        VersionInfoPtr->moduleID = (uint16)FLS_MODULE_ID;
        VersionInfoPtr->vendorID = (uint16)FLS_VENDOR_ID;
        VersionInfoPtr->sw_major_version = (uint8)FLS_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = (uint8)FLS_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = (uint8)FLS_SW_PATCH_VERSION;
#if ( FLS_DEV_ERROR_DETECT == STD_ON )
    }
#endif /* FLS_DEV_ERROR_DETECT == STD_ON */
}
#endif /* FLS_VERSION_INFO_API == STD_ON */


static inline void Fls_MainFunction_CheckJobResult(MemIf_JobResultType WorkResult)
{
    Fls_eJobResult = WorkResult;

    if (MEMIF_JOB_OK == WorkResult)
    {
        if (NULL_PTR != Fls_pConfigPtr->jobEndNotificationPtr)
        {
            /* Call FlsJobEndNotification function if configured */
            Fls_pConfigPtr->jobEndNotificationPtr();
        }
    }
    else if ( (MEMIF_JOB_FAILED == WorkResult) ||
              (MEMIF_JOB_CANCELED == WorkResult) ||
              (MEMIF_BLOCK_INCONSISTENT == WorkResult)
            )
    {
        if (NULL_PTR != Fls_pConfigPtr->jobErrorNotificationPtr)
        {
            /* Call FlsJobErrorNotification function if configured */
            Fls_pConfigPtr->jobErrorNotificationPtr();
        }
    }
    else
    {
        ; /* Empty clause added to fulfill MISRA. */
    }
}


/**
 * @brief            Performs actual flash read, write, erase and compare jobs.
 * @details          Bytes number processed per cycle depends by job type (erase, write, read, compare)
 *                   current FLS module's operating mode (normal, fast)
 *                   and write, erase Mode of Execution (sync, async).
 *
 * @api
 *
 * @pre              The module has to be initialized.
 *
 *
 * @note             This function have to be called cyclically by the Basic Software Module;
 *                   it will do nothing if there aren't pending job.
 *
 * @implements       Fls_MainFunction_Activity
 */
void Fls_MainFunction(void)
{
    /* Check if Fls is initialized*/
    /* SWS_BSW_00037 - If the module is not initialized and the main function is executed then no error shall be reported and the main function shall return immediately. */
    if (NULL_PTR != Fls_pConfigPtr)
    {
        if (MEMIF_JOB_PENDING == Fls_eJobResult)
        {
            MemIf_JobResultType WorkResult = MEMIF_JOB_OK;

#if (STD_ON == FLS_MCORE_ENABLED)
            if (FLS_MCORE_GRANTED == Fls_MainFunction_ReqMCore(&WorkResult))
            {
#endif /* #if (FLS_MCORE_ENABLED == STD_ON) */
                if (1U == Fls_u8JobStart)
                {
#if (FLS_MCORE_ENABLED == STD_ON)
                    /* Update hardware job status. */
                    Fls_MCoreHwJobStatus = FLS_MCORE_HW_JOB_STARTED;
#endif /* #if (FLS_MCORE_ENABLED == STD_ON) */
                    Fls_u8JobStart = 0U;

                    /* Clear result of hardware job */
                    Fls_LLDClrJobResult();

                    Fls_IPW_AbortSuspended();
                }
                /* Check the status of pending jobs. */
                if (FLASH_JOB_NONE != Fls_eLLDJob) /* If there is any Async job scheduled. */
                {
                    /* Process ongoing erase or write asynchronous hardware job */
                    Fls_IPW_LLDMainFunction();  /* Process the maximum defined length(configuration parameter), or until a sector boundary. */
                }
                WorkResult = Fls_LLDGetJobResult();
                if (MEMIF_JOB_OK == WorkResult)
                {
                    /* Process the requested jobs : write, compare, erase, read */
                    WorkResult = Fls_ProcessRequestedJobs();
                }
#if (FLS_MCORE_ENABLED == STD_ON)
            }
            else
            {
                ; /* Empty clause added to fulfill MISRA. */
            }

            Fls_MainFunction_ReleaseMCore(&WorkResult);
#endif /*(FLS_MCORE_ENABLED == STD_ON)*/

            Fls_MainFunction_CheckJobResult(WorkResult);
        }
        else
        {
            ; /* Nothing to do since no job is pending */
        }
    }
}

/**
 * @brief          Returns the LLD JobResult
 */
static MemIf_JobResultType Fls_LLDGetJobResult(void)
{
    return Fls_eLLDJobResult;
}

/**
 * @brief          Clear status of erase or write hardware job result.
 * @details        Set the internal status to  MEMIF_JOB_OK
 */
static void Fls_LLDClrJobResult(void)
{
    Fls_eLLDJobResult = MEMIF_JOB_OK;
    Fls_eLLDJob = FLASH_JOB_NONE;
}

#if ( STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED )
#if ( (STD_ON == FLS_ECC_CHECK) || (STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS) )
/**
* @brief          Low level data storage exception handler.
*
* @return         Exc_CompHandlerReturnType
* @retval         EXC_HANDLED_SKIP The data storage exception was
*                 caused by currently pending flash read or compare job
* @retval         EXC_UNHANDLED The data storage exception was
*                 NOT caused by currently pending flash read or compare job
*
* @implements    Fls_DsiHandler_Activity
*/
Fls_CompHandlerReturnType Fls_DsiHandler(const Fls_ExceptionDetailsType * pExceptionDetailsPtr)
{
    return Fls_IPW_DsiHandler(pExceptionDetailsPtr);
}
#endif /* (STD_ON == FLS_ECC_CHECK) || (STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS) */

#if (STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS)
/**
 * @brief         In the FlsReadFunctionCallout, user can start a task and call this function to performs the actual copy operation.
 *
 * @implements    Fls_ReadEachBlock_Activity
 */
void Fls_ReadEachBlock(void)
{
    /* Perform the copy */
    Fls_IPW_ReadEachBlock();
}
#endif /* STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS */

#endif /* STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED */

#define FLS_STOP_SEC_CODE
#include "Fls_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
