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

#ifndef FLS_TYPES_H
#define FLS_TYPES_H

/**
*   @file Fls_Types.h
*
*   @addtogroup FLS FLS Driver
*   @{
*/

/* implements Fls_Types.h_Artifact */

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "StandardTypes.h"
#include "MemIf_Types.h"



#include "C40_Ip_Types.h"


#include "Qspi_Ip_Types.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLS_TYPES_VENDOR_ID                    43
#define FLS_TYPES_AR_RELEASE_MAJOR_VERSION     4
#define FLS_TYPES_AR_RELEASE_MINOR_VERSION     4
#define FLS_TYPES_AR_RELEASE_REVISION_VERSION  0
#define FLS_TYPES_SW_MAJOR_VERSION             2
#define FLS_TYPES_SW_MINOR_VERSION             0
#define FLS_TYPES_SW_PATCH_VERSION             1
/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and StandardTypes.h file are of the same version */
    #if ((FLS_TYPES_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
         (FLS_TYPES_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION)\
        )
        #error "AutoSar Version Numbers of Fls_Types.h and StandardTypes.h are different"
    #endif
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and MemIf_Types.h file are of the same version */
    #if ((FLS_TYPES_AR_RELEASE_MAJOR_VERSION != MEMIF_AR_RELEASE_MAJOR_VERSION) || \
         (FLS_TYPES_AR_RELEASE_MINOR_VERSION != MEMIF_AR_RELEASE_MINOR_VERSION)\
        )
        #error "AutoSar Version Numbers of Fls_Types.h and MemIf_Types.h are different"
    #endif
#endif


/* Check if current file and C40_Ip_Types header file are of the same vendor */
#if (FLS_TYPES_VENDOR_ID != C40_IP_TYPES_VENDOR_ID_CFG)
    #error "Fls_Types.h and C40_Ip_Types.h have different vendor ids"
#endif
/* Check if current file and C40_Ip_Types header file are of the same Autosar version */
#if ((FLS_TYPES_AR_RELEASE_MAJOR_VERSION    != C40_IP_TYPES_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (FLS_TYPES_AR_RELEASE_MINOR_VERSION    != C40_IP_TYPES_AR_RELEASE_MINOR_VERSION_CFG) || \
     (FLS_TYPES_AR_RELEASE_REVISION_VERSION != C40_IP_TYPES_AR_RELEASE_REVISION_VERSION_CFG) \
    )
#error "AutoSar Version Numbers of Fls_Types.h and C40_Ip_Types.h are different"
#endif
/* Check if current file and C40_Ip_Types header file are of the same software version */
#if ((FLS_TYPES_SW_MAJOR_VERSION != C40_IP_TYPES_SW_MAJOR_VERSION_CFG) || \
     (FLS_TYPES_SW_MINOR_VERSION != C40_IP_TYPES_SW_MINOR_VERSION_CFG) || \
     (FLS_TYPES_SW_PATCH_VERSION != C40_IP_TYPES_SW_PATCH_VERSION_CFG) \
    )
    #error "Software Version Numbers of Fls_Types.h and C40_Ip_Types.h are different"
#endif

/* Check if current file and Qspi_Ip_Types.h header file are of the same vendor */
#if (FLS_TYPES_VENDOR_ID != FLS_QSPI_TYPES_VENDOR_ID)
    #error "Fls_Types.h and Qspi_Ip_Types.h have different vendor ids"
#endif
/* Check if current file and Qspi_Ip_Types.h header file are of the same Autosar version */
#if ((FLS_TYPES_AR_RELEASE_MAJOR_VERSION    != FLS_QSPI_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (FLS_TYPES_AR_RELEASE_MINOR_VERSION    != FLS_QSPI_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (FLS_TYPES_AR_RELEASE_REVISION_VERSION != FLS_QSPI_TYPES_AR_RELEASE_REVISION_VERSION) \
    )
#error "AutoSar Version Numbers of Fls_Types.h and Qspi_Ip_Types.h are different"
#endif
/* Check if current file and Qspi_Ip_Types.h header file are of the same software version */
#if ((FLS_TYPES_SW_MAJOR_VERSION != FLS_QSPI_TYPES_SW_MAJOR_VERSION) || \
     (FLS_TYPES_SW_MINOR_VERSION != FLS_QSPI_TYPES_SW_MINOR_VERSION) || \
     (FLS_TYPES_SW_PATCH_VERSION != FLS_QSPI_TYPES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Fls_Types.h and Qspi_Ip_Types.h are different"
#endif

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/**
    @brief Flash sector channel type.
*/
typedef enum
{
    FLS_CH_INTERN = 0,
    FLS_CH_QSPI = 1
} Fls_HwChType;

/**
* @brief          Type of job currently executed by Fls_MainFunction.
*/
typedef enum
{
    /**
    * @brief erase one or more complete flash sectors
    */
    FLS_JOB_ERASE = 0,
    /**
    * @brief write one or more complete flash pages
    */
    FLS_JOB_WRITE,
    /**
    * @brief read one or more bytes from flash memory
    */
    FLS_JOB_READ,
    /**
    * @brief compare data buffer with content of flash memory
    */
    FLS_JOB_COMPARE,
    /**
    * @brief check content of erased flash memory area
    */
    FLS_JOB_BLANK_CHECK
} Fls_JobType;

/**
* @brief          Result of low-level flash operation.
*/
typedef enum
{
    FLASH_E_OK = 0,                 /**< @brief operation succeeded */
    FLASH_E_FAILED,                 /**< @brief operation failed due to hardware error */
    FLASH_E_BLOCK_INCONSISTENT,     /**< @brief data buffer doesn't match with content of flash memory */
    FLASH_E_PENDING,                /**< @brief operation is pending */
    FLASH_E_PARTITION_ERR           /**< @brief FlexNVM partition ratio error */
} Fls_LLDReturnType;

/**
* @brief          Type of job currently executed by Fls_LLDMainFunction.
*/
typedef enum
{
    /**
    * @brief no job executed by Fls_LLDMainFunction
    */
    FLASH_JOB_NONE = 0,
    /**
    * @brief erase one flash sector
    */
    FLASH_JOB_ERASE,
    /**
    * @brief complete erase and start an interleaved erase flash sector
    */
    FLASH_JOB_ERASE_TEMP,
    /**
    * @brief write one or more complete flash pages
    */
    FLASH_JOB_WRITE,
    /**
    * @brief erase blank check of flash sector
    */
    FLASH_JOB_ERASE_BLANK_CHECK

} Fls_LLDJobType;

/**
* @brief          Size of data to be processeed by CRC.
*
* @implements     Fls_CrcDataSizeType_enumeration
*/
typedef enum
{
    /**
    * @brief crc 8 bits
    */
    FLS_CRC_8_BITS = 0,
    /**
    * @brief crc 16 bits
    */
    FLS_CRC_16_BITS
} Fls_CrcDataSizeType;


/**
* @brief          Logical sector index.
*/
typedef uint32 Fls_SectorIndexType;

/**
* @brief          Fls CRC Type.
* @details        CRC computed over config set.
* @implements     Fls_CrcType_typedef
*/
typedef uint16 Fls_CrcType;

/**
* @brief          Fls Address Type.
* @details        Address offset from the configured flash base address to access a certain flash
*                 memory area.
* @implements     Fls_AddressType_typedef */
typedef uint32 Fls_AddressType;

/**
* @brief          Fls Length Type.
* @details        Number of bytes to read,write,erase,compare
* @implements     Fls_LengthType_typedef */
typedef uint32 Fls_LengthType;

/**
* @brief          Fls Sector Count Type
* @details        Number of configured sectors
*/
typedef uint32 Fls_SectorCountType;

/**
* @brief          Fls BLock Count Type
* @details        Block number of sectors type
*/
typedef uint8 Fls_BlockNumberOfSectorType;

/**
* @brief          Fls Internal Flash Type
* @details        Configuration structure of internal flash.
*/
typedef C40_ConfigType Fls_InternalConfigType;

/**
* @brief          Fls Multi Core Request Return Type.
* @details        The return value for the function requesting multi core access.
*
*/
typedef enum
{
   /**
    * @brief return error
    */
    FLS_MCORE_ERROR = 0,
    /**
    * @brief return timeout
    */
    FLS_MCORE_TIMEOUT,
    /**
    * @brief return pending
    */
    FLS_MCORE_PENDING,
    /**
    * @brief return granted
    */
    FLS_MCORE_GRANTED,
    /**
    * @brief return cancelled
    */
    FLS_MCORE_CANCELLED
} Fls_MCoreReqReturnType;

/**
* @brief          Fls Multi Core hardware job status
* @details        The status of a multicore core flash job, in hardware. Used to determine
*                 if a flash job subject to multicore arbitration was started/suspended/aborted
*                 in hardware, in the flash controller. This can be used for example, to
*                 clear a semaphore granted for erase directly, if the job was not actually started
*                 in hardware, instead of attempting to suspend it.
*
*/
typedef enum
{
    /**
    * @brief idle status
    */
    FLS_MCORE_HW_JOB_IDLE = 0,
    /**
    * @brief mainf started status
    */
    FLS_MCORE_HW_JOB_MAINF_STARTED,
    /**
    * @brief started status
    */
    FLS_MCORE_HW_JOB_STARTED,
    /**
    * @brief cancelled status
    */
    FLS_MCORE_HW_JOB_CANCELLED
} Fls_MCoreHwJobStatusType;

/**
* @brief          Fls Multi Core timeout notification jobs
* @details        The timeout notification specifies the job during which timeout occurred.
*
*/
typedef enum
{
    FLS_MCORE_TIMEOUT_ERASE = 0,       /**< @brief timeout job erase */
    FLS_MCORE_TIMEOUT_WRITE,           /**< @brief timeout job write */
    FLS_MCORE_TIMEOUT_READ,            /**< @brief timeout job read */
    FLS_MCORE_TIMEOUT_COMPARE,         /**< @brief timeout job compare */
    FLS_MCORE_TIMEOUT_BLANK_CHECK      /**< @brief timeout job blank check */
} Fls_MCoreTimeoutJobType;

/**
* @brief          Fls Job End Notification Pointer Type
* @details        Pointer type of Fls_JobEndNotification function
* @implements     Fls_JobEndNotificationPtrType_typedef */
typedef void (*Fls_JobEndNotificationPtrType)(void);

/**
* @brief          Fls Job Error Notification Pointer Type
* @details        Pointer type of Fls_JobErrorNotification function
* @implements     Fls_JobErrorNotificationPtrType_typedef */
typedef void (*Fls_JobErrorNotificationPtrType)(void);


/**
 * @brief Pointer type of Fls_AC_Callback function
*/
typedef void (*Fls_ACCallbackPtrType)  ( void );
/**
    @brief Define pointer type of erase access code function
*/
typedef void (*Fls_AcErasePtrType)  ( void (*CallBack)( void ));

/**
    @brief Define pointer type of write access code function
*/
typedef void (*Fls_AcWritePtrType)  ( void (*CallBack)( void ));

/**
 * @brief Pointer type of Fls_ReadFunctionPtrType function
 *
 * @details      The callout for the user to check for ECC errors for Internal Flash memories.
 *               In this callout, the user can schedule a task that reads from flash memory
 *               to a read source buffer and check/handle for an ECC exception.
 *
 *
*/
typedef void (*Fls_ReadFunctionPtrType) (void);

/**
    @brief FLASH physical sector description
*/
typedef struct
{
    uint32 pSectorStartAddressPtr;         /**< @brief FLASH physical sector start address */
    uint32 u32SectorId;                    /**< @brief Corresponding number in sector location to calc cfgCRC */
} Fls_Flash_InternalSectorInfoType;
/**
* @brief          Fls Multi Core Notification Pointer Type
* @details        Pointer type of Fls_MCoreTimeoutNotifPtrType function
*
*/
typedef void (*Fls_MCoreTimeoutNotifPtrType)(Fls_MCoreTimeoutJobType eMCoreTimeoutJob);



/**
* @brief          Fls Qspi CfgConfig Type
* @details        Fls Qspi CfgConfig Type
* @implements     Fls_QspiCfgConfigType_typedef */
typedef struct
{
    /**
    * @brief External flash unit assigned to each sector. Size: u32SectorCount
    */
    const uint8 (*u8SectFlashUnit)[];
    /**
    * @brief Number of serial flash instances.
    */
    const uint8 u8FlashUnitsCount;
    /**
    * @brief Connection for each external memory device to available controllers. Size: u8FlashUnitsCount
    */
    const Qspi_Ip_MemoryConnectionType (*paFlashConnectionCfg)[];
    /**
    * @brief Configuration index used for each flash unit. Size: u8FlashUnitsCount
    */
    const uint8 (*u8FlashConfig)[];
    /**
    * @brief AHB direct reads configurations. Size: u8FlashUnitsCount
    */
    const boolean (*paAHBReadCfg)[];

    /**
    * @brief Number of serial flash configurations.
    */
    const uint8 u8FlashConfigCount;
    /**
    * @brief External memory devices configurations. Size: u8FlashConfigCount
    */
    const Qspi_Ip_MemoryConfigType (*paFlashCfg)[];

    /**
    * @brief Number of QSPI hardware instances.
    */
    const uint8 u8QspiUnitsCount;
    /**
    * @brief Configuration for each QSPI unit. Size: u8QspiUnitsCount ]
    */
    const uint8 (*u8QspiConfig)[];
    /**
    * @brief Number of QSPI configurations.
    */
    const uint8 u8QspiConfigCount;
    /**
    * @brief QSPI configurations. Size: u8QspiConfigCount
    */
    const Qspi_Ip_ControllerConfigType (*paQspiUnitCfg)[];
}Fls_QspiCfgConfigType;
/**
* @brief          Fls Config Type
* @details        Fls module initialization data structure
* @implements     Fls_ConfigType_typedef */
typedef struct
{
    /**
    * @brief pointer to erase access code function in RAM or ROM
    */
    Fls_AcErasePtrType acErasePtr;
    /**
    * @brief pointer to write access code function in RAM or ROM
    */
    Fls_AcWritePtrType acWritePtr;
    /**
    * @brief pointer to ac callback function
    */
    Fls_ACCallbackPtrType acCallBackPtr;
    /**
    * @brief pointer to job end notification function
    */
    Fls_JobEndNotificationPtrType jobEndNotificationPtr;
    /**
    * @brief pointer to job error notification function
    */
    Fls_JobErrorNotificationPtrType jobErrorNotificationPtr;
    /**
    * @brief pointer to finished flash access notification
    */
    Fls_MCoreTimeoutNotifPtrType MCoreTimeoutNotifPtr;
    /**
    * @brief pointer to read to flash memory callout
    */
    Fls_ReadFunctionPtrType FlsReadFunctionCallout;
    /**
    * @brief default FLS device mode after initialization (MEMIF_MODE_FAST, MEMIF_MODE_SLOW)
    */
    MemIf_ModeType eDefaultMode;
    /**
    * @brief max number of bytes to read in one cycle of Fls_MainFunction (fast mode)
    */
    Fls_LengthType u32MaxReadFastMode;
    /**
    * @brief max number of bytes to read in one cycle of  Fls_MainFunction (normal mode)
    */
    Fls_LengthType u32MaxReadNormalMode;
    /**
    * @brief max number of bytes to write in one cycle of Fls_MainFunction (fast mode)
    */
    Fls_LengthType u32MaxWriteFastMode;
    /**
    * @brief max number of bytes to write in one cycle of  Fls_MainFunction (normal mode)
    */
    Fls_LengthType u32MaxWriteNormalMode;
    /**
    * @brief number of configured logical sectors
    */
    Fls_SectorCountType u32SectorCount;
    /**
    * @brief pointer to array containing last logical address of each configured sector
    */
    const Fls_AddressType (*paSectorEndAddr)[];
    /**
    * @brief pointer to array containing sector size of each configured sector
    */
    const Fls_LengthType (*paSectorSize)[];
    /**
    * @brief pointer to array containing physical sector ID of each configured sector
    */
    const Fls_Flash_InternalSectorInfoType * const (*pSectorList)[];
    /**
    * @brief pointer to array containing flags set of each configured sector
    */
    const uint8 (*paSectorFlags)[];
    /**
    * @brief pointer to array containing page size information of each configured sector
    */
    const Fls_LengthType (*paSectorPageSize)[];
    /**
    * @brief Pointer to array containing the hardware channel(internal, external_qspi, external_emmc) of each configured sector.
    */
    const Fls_HwChType (*paHwCh)[];
    /**
    * @brief Pointer to array containing the configured hardware start address of each external sector.
    */
    const uint32 (*paSectorHwAddress)[];

    /** @brief  Pointer to configuration structure of QSPI. */
    const Fls_QspiCfgConfigType *  pFlsQspiCfgConfig;
    /**
    * @brief Pointer to configuration structure internal flash.
    */
    const Fls_InternalConfigType * pFlsInternalCfgConfig;
    /**
     * @brief FLS Config Set CRC checksum
     */
    Fls_CrcType u16ConfigCrc;

} Fls_ConfigType;

#ifdef __cplusplus
}
#endif

/** @}*/

#endif /* FLS_TYPES_H */
