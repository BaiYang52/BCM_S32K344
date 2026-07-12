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

#ifndef FLS_API_H
#define FLS_API_H

/**
*   @file Fls_Api.h
*
*   @addtogroup FLS FLS Driver
*   @{
*/

/* implements Fls_Api.h_Artifact */

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Fls_Cfg.h"
#include "MemIf_Types.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/**
* @brief          Version Check parameters.
*/
#define FLS_API_VENDOR_ID                    43
#define FLS_API_AR_RELEASE_MAJOR_VERSION     4
#define FLS_API_AR_RELEASE_MINOR_VERSION     4
#define FLS_API_AR_RELEASE_REVISION_VERSION  0
#define FLS_API_SW_MAJOR_VERSION             2
#define FLS_API_SW_MINOR_VERSION             0
#define FLS_API_SW_PATCH_VERSION             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and Fls_Cfg.h file are of the same vendor */
#if (FLS_API_VENDOR_ID != FLS_VENDOR_ID_CFG)
    #error "Fls_Api.h and Fls_Cfg.h have different vendor ids"
#endif
/* Check if current file and Fls_Cfg.h file are of the same Autosar version */
#if ((FLS_API_AR_RELEASE_MAJOR_VERSION    != FLS_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (FLS_API_AR_RELEASE_MINOR_VERSION    != FLS_AR_RELEASE_MINOR_VERSION_CFG) || \
     (FLS_API_AR_RELEASE_REVISION_VERSION != FLS_AR_RELEASE_REVISION_VERSION_CFG) \
    )
    #error "AutoSar Version Numbers of Fls_Api.h and Fls_Cfg.h are different"
#endif
/* Check if current file and Fls_Cfg.h file are of the same software version */
#if ((FLS_API_SW_MAJOR_VERSION != FLS_SW_MAJOR_VERSION_CFG) || \
     (FLS_API_SW_MINOR_VERSION != FLS_SW_MINOR_VERSION_CFG) || \
     (FLS_API_SW_PATCH_VERSION != FLS_SW_PATCH_VERSION_CFG) \
    )
    #error "Software Version Numbers of Fls_Api.h and Fls_Cfg.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and MemIf_Types.h file are of the same Autosar version */
    #if ((FLS_API_AR_RELEASE_MAJOR_VERSION    != MEMIF_AR_RELEASE_MAJOR_VERSION) || \
        (FLS_API_AR_RELEASE_MINOR_VERSION    != MEMIF_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AutoSar Version Numbers of Fls_Api.h and MemIf_Types.h are different"
    #endif
#endif

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/**
*  @brief AUTOSAR module identification
*/
#define FLS_MODULE_ID            92
/**
*  @brief AUTOSAR module instance identification
*/
#define FLS_INSTANCE_ID          0U
/**
* @brief          Development error codes (passed to DET).
*/

/**
*  @brief API service called with wrong config parameter
*/
#define FLS_E_PARAM_CONFIG           0x01U
/**
*  @brief API service called with wrong address parameter
*/
#define FLS_E_PARAM_ADDRESS          0x02U
/**
*  @brief API service called with wrong length parameter
*/
#define FLS_E_PARAM_LENGTH           0x03U
/**
*  @brief API service called with wrong data parameter
*/
#define FLS_E_PARAM_DATA             0x04U
/**
*  @brief API service called without module initialization
*/
#define FLS_E_UNINIT                 0x05U
/**
*  @brief API service called while driver still busy
*/
#define FLS_E_BUSY                   0x06U
/**
*  @brief API service called with NULL pointer
*/
#define FLS_E_PARAM_POINTER          0x0AU
/**
* @brief          Runtime error codes (passed to DET).
*/

/**
* @brief          Erase verification (blank check) failed
*/
#define FLS_E_VERIFY_ERASE_FAILED    0x07U
/**
* @brief          Write verification (compare) failed
*/
#define FLS_E_VERIFY_WRITE_FAILED    0x08U
/**
* @brief           Timeout exceeded
*/
#define FLS_E_TIMEOUT                0x09U

/**
* @brief          Transient Faults codes (passed to DET).
*/
/**
* @brief           Flash erase failed (HW)
*/
#define FLS_E_ERASE_FAILED           0x01U
/**
* @brief           Flash write failed (HW)
*/
#define FLS_E_WRITE_FAILED           0x02U
/**
* @brief           Flash read failed (HW)
*/
#define FLS_E_READ_FAILED            0x03U
/**
* @brief           Flash compare failed (HW)
*/
#define FLS_E_COMPARE_FAILED         0x04U
#if (FLS_QSPI_SECTORS_CONFIGURED == STD_ON)
    #define FLS_E_UNEXPECTED_FLASH_ID    0x05U /* Expected hardware ID not matched */
#endif
/**
* @brief          All service IDs (passed to DET).
* @implements     SERVICEIDs_enumeration
*/

/**
*   @brief service ID of function: Fls_Init. (passed to DET)
*/
#define FLS_INIT_ID                  0x00U
/**
*   @brief service ID of function: Fls_Erase. (passed to DET)
*/
#define FLS_ERASE_ID                 0x01U
/**
*   @brief service ID of function: Fls_Write. (passed to DET)
*/
#define FLS_WRITE_ID                 0x02U
/**
*   @brief service ID of function: Fls_Cancel. (passed to DET)
*/
#define FLS_CANCEL_ID                0x03U
/**
*   @brief service ID of function: Fls_GetJobResult. (passed to DET)
*/
#define FLS_GETJOBRESULT_ID          0x05U
/**
*   @brief service ID of function: Fls_MainFunction. (passed to DET)
*/
#define FLS_MAINFUNCTION_ID          0x06U
/**
*   @brief service ID of function: Fls_Read. (passed to DET)
*/
#define FLS_READ_ID                  0x07U
/**
*   @brief service ID of function: Fls_Compare. (passed to DET)
*/
#define FLS_COMPARE_ID               0x08U
/**
*   @brief service ID of function: Fls_SetMode. (passed to DET)
*/
#define FLS_SETMODE_ID               0x09U
/**
*   @brief service ID of function: Fls_GetVersionInfo. (passed to DET)
*/
#define FLS_GETVERSIONINFO_ID        0x10U
/**
*   @brief service ID of function: Fls_BlankCheck. (passed to DET)
*/
#define FLS_BLANK_CHECK_ID           0x11U

/**
* @brief          All sector flags.
*/

/**
*   @brief fls sector erase asynch
*/
#define FLS_SECTOR_ERASE_ASYNCH      0x01U
/**
*   @brief fls page write asynch
*/
#define FLS_PAGE_WRITE_ASYNCH        0x02U
/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

/**
* @brief    Start of Fls section CODE
*/
#define FLS_START_SEC_CODE
#include "Fls_MemMap.h"


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
 *
 */
extern void Fls_Init(const Fls_ConfigType * ConfigPtr);

/**
 * @brief            Write one or more complete flash pages to the flash device.
 * @details          Starts a write job asynchronously. The actual job is performed by
 *                   @p Fls_MainFunction.
 *
 * @param[in]        TargetAddress        Target address in flash memory. This address offset will be added to the flash memory base address.
 * @param[in]        SourceAddressPtr     Pointer to source data buffer.
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
 */
extern Std_ReturnType Fls_Write(Fls_AddressType TargetAddress,
                                const uint8 * SourceAddressPtr,
                                Fls_LengthType Length
                               );

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
 *
 */
extern Std_ReturnType Fls_Erase(Fls_AddressType TargetAddress,
                                Fls_LengthType Length
                               );

#if ( FLS_CANCEL_API == STD_ON )
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
 *
 */
extern void Fls_Cancel( void );
#endif    /* FLS_CANCEL_API == STD_ON */

#if ( FLS_GET_STATUS_API == STD_ON )
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
 */
extern MemIf_StatusType Fls_GetStatus( void );
#endif    /* FLS_GET_STATUS_API == STD_ON */

#if ( FLS_GET_JOB_RESULT_API == STD_ON )
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
extern MemIf_JobResultType Fls_GetJobResult( void );
#endif    /* FLS_GET_JOB_RESULT_API == STD_ON */


/**
 * @brief            Reads from flash memory.
 * @details          Starts a read job asynchronously. The actual job is performed by
 *                   @p Fls_MainFunction.
 *
 * @param[in]        SourceAddress        Source address in flash memory. This address offset will be added to the flash memory base address.
 * @param[in]        Length               Number of bytes to read.
 * @param[out]       TargetAddressPtr       Pointer to target data buffer.
 *
 * @return           Std_ReturnType
 * @retval           E_OK                 Read command has been accepted
 * @retval           E_NOT_OK             Read command has not been accepted
 *
 * @api
 *
 * @pre            The module has to be initialized and not busy.
 * @post           @p Fls_Read changes module status and some internal variables
 *                 (@p Fls_u32JobSectorIt, @p Fls_u32JobAddrIt, @p Fls_u32JobAddrEnd,
 *                 @p Fls_pJobDataDestPtr, @p Fls_eJob, @p Fls_eJobResult).
 *
 *
 */
extern Std_ReturnType Fls_Read(Fls_AddressType SourceAddress,
                               uint8 * TargetAddressPtr,
                               Fls_LengthType Length
                              );

#if ( FLS_COMPARE_API == STD_ON )
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
 */
extern Std_ReturnType Fls_Compare(Fls_AddressType SourceAddress,
                                  const uint8 * TargetAddressPtr,
                                  Fls_LengthType Length
                                 );

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
                             );
#endif /* FLS_BLANK_CHECK_API == STD_ON */

#if (FLS_SET_MODE_API == STD_ON) || defined(__DOXYGEN__)
/**
 * @brief           Sets the FLS module's operation mode to the given Mode.
 * @details         Every given mode determinates maximum bytes for read-write
 *                  operations. Every mode has a set of pre-configured values.
 *
 * @param[in]       Mode        MEMIF_MODE_FAST or MEMIF_MODE_SLOW.
 *
 * @api
 *
 * @pre            The module has to be initialized and not busy.
 * @post           @p Fls_SetMode changes internal variables @p Fls_u32MaxRead and
 *                 @p Fls_u32MaxWrite.
 *
 */
extern void Fls_SetMode(MemIf_ModeType Mode);
#endif /* FLS_SET_MODE_API == STD_ON */

#if ( FLS_VERSION_INFO_API == STD_ON )
/**
 * @brief        Returns version information about FLS module.
 * @details      Version information includes:
 *               - Module Id
 *               - Vendor Id
 *               - Vendor specific version numbers (BSW00407).
 *
 * @param[inout] VersionInfoPtr  Pointer to where to store the version information of this module.
 *
 * @api
 *
 */
extern void Fls_GetVersionInfo(Std_VersionInfoType * VersionInfoPtr);
#endif /* FLS_VERSION_INFO_API == STD_ON */

#if ( STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED )
#if ( (STD_ON == FLS_ECC_CHECK) || (STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS) )
/**
* @brief          Low level data storage exception handler.
* @details        Low level data storage exception handler.
*
* @param[in] pExceptionDetailsPtr  Exception pointer
*
* @return         Exc_CompHandlerReturnType
* @retval         EXC_HANDLED_SKIP The data storage exception was
*                 caused by currently pending flash read or compare job
* @retval         EXC_UNHANDLED The data storage exception was
*                 NOT caused by currently pending flash read or compare job
*
* @implements    Fls_DsiHandler_Activity
*/
extern Fls_CompHandlerReturnType Fls_DsiHandler(const Fls_ExceptionDetailsType * pExceptionDetailsPtr);
#endif /* (STD_ON == FLS_ECC_CHECK) || (STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS) */

#if (STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS)
/**
 * @brief         In the FlsReadFunctionCallout, user can start a task and call this function to performs the actual copy operation.
 *
 * @implements    Fls_ReadEachBlock_Activity
 */
extern void Fls_ReadEachBlock(void);
#endif /* STD_ON == FLS_ECC_CHECK_BY_AUTOSAR_OS */

#endif /* STD_ON == FLS_INTERNAL_SECTORS_CONFIGURED */

/**
* @brief    Stop of Fls section CODE
*/
#define FLS_STOP_SEC_CODE
#include "Fls_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @}*/

#endif /* FLS_API_H */
