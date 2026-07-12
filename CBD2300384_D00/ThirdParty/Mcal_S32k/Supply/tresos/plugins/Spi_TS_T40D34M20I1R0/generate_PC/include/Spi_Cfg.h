/**
*   @file    Spi_Cfg.h
*   @version 2.0.1
*
*   @brief   AUTOSAR Spi - Spi configuration header file.
*   @details This file is the header containing all the necessary information for SPI
*            module configuration(s).
*   @addtogroup SPI_DRIVER_CONFIGURATION Spi Driver Configuration
*   @{
*/
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : LPSPI
*   Dependencies         : 
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
[!CODE!][!//

#ifndef SPI_CFG_H
#define SPI_CFG_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
* 4) user callback header files
==================================================================================================*/
#include "Mcal.h"
#include "OsIf.h"
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!][!//
#include "Spi_[!"."!]_PBcfg.h"
[!ENDLOOP!][!//
[!ELSE!]
#include "Spi_PBcfg.h"
[!ENDIF!]
#include "Spi_Ipw_Cfg.h"

[!NOCODE!][!//
[!INCLUDE "Spi_VersionCheck_Inc.m"!][!//
[!INCLUDE "Spi_RegOperations.m"!][!//
[!ENDNOCODE!][!//

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define SPI_VENDOR_ID_CFG                       43
#define SPI_AR_RELEASE_MAJOR_VERSION_CFG        4
#define SPI_AR_RELEASE_MINOR_VERSION_CFG        4
#define SPI_AR_RELEASE_REVISION_VERSION_CFG     0
#define SPI_SW_MAJOR_VERSION_CFG                2
#define SPI_SW_MINOR_VERSION_CFG                0
#define SPI_SW_PATCH_VERSION_CFG                1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and Mcal header file are of the same Autosar version */
    #if ((SPI_AR_RELEASE_MAJOR_VERSION_CFG != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (SPI_AR_RELEASE_MINOR_VERSION_CFG != MCAL_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Spi_Cfg.h and Mcal.h are different"
    #endif
     /* Check if the current file and OsIf.h header file are of the same version */
    #if ((SPI_AR_RELEASE_MAJOR_VERSION_CFG != OSIF_AR_RELEASE_MAJOR_VERSION) || \
         (SPI_AR_RELEASE_MINOR_VERSION_CFG != OSIF_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Spi_Cfg.h and OsIf.h are different"
    #endif   
#endif

[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
/* Check if Spi_[!"."!]_PBcfg header file and Spi configuration header file are of the same vendor */
#if (SPI_VENDOR_ID_[!"."!]_PBCFG_H != SPI_VENDOR_ID_CFG)
    #error "Spi_[!"."!]_PBcfg.h and Spi_Cfg.h have different vendor IDs"
#endif
    /* Check if Spi_[!"."!]_PBcfg header file and Spi  configuration header file are of the same Autosar version */
#if ((SPI_AR_RELEASE_MAJOR_VERSION_[!"."!]_PBCFG_H != SPI_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (SPI_AR_RELEASE_MINOR_VERSION_[!"."!]_PBCFG_H != SPI_AR_RELEASE_MINOR_VERSION_CFG) || \
     (SPI_AR_RELEASE_REVISION_VERSION_[!"."!]_PBCFG_H != SPI_AR_RELEASE_REVISION_VERSION_CFG))
#error "AutoSar Version Numbers of Spi_[!"."!]_PBcfg.h and Spi_Cfg.h are different"
#endif
/* Check if Spi_[!"."!]_PBcfg header file and Spi configuration header file are of the same software version */
#if ((SPI_SW_MAJOR_VERSION_[!"."!]_PBCFG_H != SPI_SW_MAJOR_VERSION_CFG) || \
     (SPI_SW_MINOR_VERSION_[!"."!]_PBCFG_H != SPI_SW_MINOR_VERSION_CFG) || \
     (SPI_SW_PATCH_VERSION_[!"."!]_PBCFG_H != SPI_SW_PATCH_VERSION_CFG))
#error "Software Version Numbers of Spi_[!"."!]_PBcfg.h and Spi_Cfg.h are different"
#endif
[!ENDLOOP!]

[!ELSE!]
/* Check if Spi_PBcfg header file and Spi configuration header file are of the same vendor */
#if (SPI_VENDOR_ID_PBCFG_H != SPI_VENDOR_ID_CFG)
    #error "Spi_PBcfg.h and Spi_Cfg.h have different vendor IDs"
#endif
    /* Check if Spi_PBcfg header file and Spi configuration header file are of the same Autosar version */
#if ((SPI_AR_RELEASE_MAJOR_VERSION_PBCFG_H != SPI_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (SPI_AR_RELEASE_MINOR_VERSION_PBCFG_H != SPI_AR_RELEASE_MINOR_VERSION_CFG) || \
     (SPI_AR_RELEASE_REVISION_VERSION_PBCFG_H != SPI_AR_RELEASE_REVISION_VERSION_CFG))
#error "AutoSar Version Numbers of Spi_PBcfg.h and Spi_Cfg.h are different"
#endif
/* Check if Spi_PBcfg header file and Spi configuration header file are of the same software version */
#if ((SPI_SW_MAJOR_VERSION_PBCFG_H != SPI_SW_MAJOR_VERSION_CFG) || \
     (SPI_SW_MINOR_VERSION_PBCFG_H != SPI_SW_MINOR_VERSION_CFG) || \
     (SPI_SW_PATCH_VERSION_PBCFG_H != SPI_SW_PATCH_VERSION_CFG))
#error "Software Version Numbers of Spi_PBcfg.h and Spi_Cfg.h are different"
#endif
[!ENDIF!]

/* Check if Spi_Ipw_Cfg header file and Spi configuration header file are of the same vendor */
#if (SPI_IPW_VENDOR_ID_CFG_H != SPI_VENDOR_ID_CFG)
    #error "Spi_Ipw_Cfg.h and Spi_Cfg.h have different vendor IDs"
#endif
    /* Check if Spi_Ipw_Cfg header file and Spi configuration header file are of the same Autosar version */
#if ((SPI_IPW_AR_RELEASE_MAJOR_VERSION_CFG_H != SPI_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (SPI_IPW_AR_RELEASE_MINOR_VERSION_CFG_H != SPI_AR_RELEASE_MINOR_VERSION_CFG) || \
     (SPI_IPW_AR_RELEASE_REVISION_VERSION_CFG_H != SPI_AR_RELEASE_REVISION_VERSION_CFG))
#error "AutoSar Version Numbers of Spi_Ipw_Cfg.h and Spi_Cfg.h are different"
#endif
/* Check if Spi_Ipw_Cfg header file and Spi configuration header file are of the same software version */
#if ((SPI_IPW_SW_MAJOR_VERSION_CFG_H != SPI_SW_MAJOR_VERSION_CFG) || \
     (SPI_IPW_SW_MINOR_VERSION_CFG_H != SPI_SW_MINOR_VERSION_CFG) || \
     (SPI_IPW_SW_PATCH_VERSION_CFG_H != SPI_SW_PATCH_VERSION_CFG))
#error "Software Version Numbers of Spi_Ipw_Cfg.h and Spi_Cfg.h are different"
#endif
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/
/**
* @brief Switches the Development Error functionality ON or OFF.
* @details Switches the Development Error Detection and Notification ON or OFF.
* @implements SPI_DEV_ERROR_DETECT_define
*/

#define SPI_DEV_ERROR_DETECT           ([!IF "SpiGeneral/SpiDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
* @brief Switches the Version Information API functionality ON or OFF.
* @details Switches the Spi_GetVersionInfo function ON or OFF.
*/

#define SPI_VERSION_INFO_API           ([!IF "SpiGeneral/SpiVersionInfoApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
* @brief Switches the Interruptible Sequences handling functionality ON or OFF. 
* @details This parameter depends on SPI_LEVEL_DELIVERED value. 
*        It is only used for SPI_LEVEL_DELIVERED configured to 1 or 2.
* @implements SPI_INTERRUPTIBLE_SEQ_ALLOWED_define
*/

#define SPI_INTERRUPTIBLE_SEQ_ALLOWED  ([!IF "SpiGeneral/SpiInterruptibleSeqAllowed"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
* @brief Switches the Spi_GetHWUnitStatus function ON or OFF.
* @details Switches the Spi_GetHWUnitStatus function ON or OFF.
* @implements SPI_HW_STATUS_API_define
*/

#define SPI_HW_STATUS_API          ([!IF "SpiGeneral/SpiHwStatusApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
* @brief Switches the Spi_Cancel function ON or OFF.
* @details Switches the Spi_Cancel function ON or OFF. (see chapter 8.3.13)
* @implements SPI_CANCEL_API_define
*/
#define SPI_CANCEL_API             ([!IF "SpiGeneral/SpiCancelApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Only Internal Buffers are allowed in Handler Driver.*/
#define SPI_USAGE0                          0x00u

/* Only External Buffers are allowed in Handler Driver.*/
#define SPI_USAGE1                          0x01u

/* Both Buffer types are allowd in Handler Driver. */
#define SPI_USAGE2                          0x02u

/**
* @brief Selects the SPI Handler Driver Channel Buffers usage allowed and delivered.
* @details Selects the SPI Handler Driver Channel Buffers usage allowed and delivered.
*        (see chapter 7.2.1)
* @implements SPI_CHANNEL_BUFFERS_ALLOWED_define
*/
#define SPI_CHANNEL_BUFFERS_ALLOWED    (SPI_USAGE[!"SpiGeneral/SpiChannelBuffersAllowed"!])

/* The LEVEL 0 Simple Synchronous SPI Handler Driver functionalities are selected.*/
#define SPI_LEVEL0                          0x00u

/* The LEVEL 1 Basic Asynchronous SPI Handler Driver functionalities are selected.*/
#define SPI_LEVEL1                          0x01u

/* The LEVEL 2 Enhanced SPI Handler Driver functionalities are selected. */
#define SPI_LEVEL2                          0x02u

/**
* @brief Selects the SPI Handler Driver level of scalable functionality.
* @details Selects the SPI Handler Driver level of scalable functionality that 
* is available and delivered. (see chapter 7.1)
* @implements SPI_LEVEL_DELIVERED_define
*/
#define SPI_LEVEL_DELIVERED            (SPI_LEVEL[!"SpiGeneral/SpiLevelDelivered"!])

/**
* @brief Defines the maximum number of supported channels.
* @details Defines the maximum number of supported channels
*     for all the driver configurations.
*/
[!LOOP "SpiDriver/SpiChannel/*"!][!//
#define SpiConf_SpiChannel_[!"name(.)"!]    ((Spi_ChannelType)[!"SpiChannelId"!])
[!ENDLOOP!][!//
#define SPI_MAX_CHANNEL  ([!"num:i(count(SpiDriver/SpiChannel/*))"!]u)

/**
* @brief Total number of Jobs configured.
* @details Defines the maximum number of supported jobs
*     for all the driver configurations.
*/
[!LOOP "SpiDriver/SpiJob/*"!][!//
#define SpiConf_SpiJob_[!"name(.)"!]   ((Spi_JobType)[!"SpiJobId"!])
[!ENDLOOP!][!//
#define SPI_MAX_JOB     ([!"num:i(count(SpiDriver/SpiJob/*))"!]u)

/**
* @brief Total number of Sequences configured.
* @details Defines the maximum number of supported sequences
*     for all the driver configurations.
*/
[!LOOP "SpiDriver/SpiSequence/*"!][!//
#define SpiConf_SpiSequence_[!"name(.)"!]   ((Spi_SequenceType)[!"SpiSequenceId"!])
[!ENDLOOP!][!//
#define SPI_MAX_SEQUENCE  ([!"num:i(count(SpiDriver/SpiSequence/*))"!]u)

/**
* @brief Defines the peripherals used throughout the configuration(s).
* @details Defines the peripherals used throughout the configuration(s).
*/
[!LOOP "SpiGeneral/SpiPhyUnit/*"!][!//
#define CSIB[!"@index"!]  ((uint8)[!"@index"!]u)
[!ENDLOOP!][!//

/**
* @brief Total number of SpiPhyUnit configured.
*/
#define SPI_MAX_HWUNIT   ([!"num:i(count(SpiGeneral/SpiPhyUnit/*))"!]u)

/**
* @brief Defines the external devices the driver will use.
* @details Reference to the external device used by this job.
*/
[!LOOP "SpiDriver/SpiExternalDevice/*"!][!//
#define SPI_[!"name(.)"!]  ((Spi_ExternalDeviceType)[!"@index"!]u)
[!ENDLOOP!][!//

[!NOCODE!]
[!VAR "SpiDemErrorEnable" = "'false'"!]
[!IF "SpiAutosarExt/SpiDisableDemReportErrorStatus ='false'"!][!//
    [!IF "node:exists(SpiDemEventParameterRefs)"!]
        [!VAR "SpiDemErrorEnable" = "'true'"!]
    [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!]
/**
* @brief   Switches the Production Error Detection and Notification OFF
*
* @implements SPI_DISABLE_DEM_REPORT_ERROR_STATUS_define
* 
*/
#define SPI_DISABLE_DEM_REPORT_ERROR_STATUS   [!IF "$SpiDemErrorEnable"!](STD_OFF)  /* Enable Production Error Detection */[!ELSE!](STD_ON) /* Disable Production Error Detection */[!ENDIF!]


/*==================================================================================================
 *                                     DEFINES AND MACROS
==================================================================================================*/
[!IF "(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPostBuild' or (variant:size()>1))"!] [!//
#define SPI_CONFIG_EXT \
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
[!WS8!]SPI_CONFIG_[!"."!]_PB \
[!ENDLOOP!][!//
[!ELSE!][!//
[!WS8!]SPI_CONFIG_PB
[!ENDIF!]
[!ENDIF!]

/**
* @brief Define values for Autosar configuration variants.
* @details Define values for Autosar configuration variants.
*/
#define SPI_VARIANT_PRECOMPILE  (0)
#define SPI_VARIANT_POSTBUILD   (1)
#define SPI_VARIANT_LINKTIME    (2)

[!IF "IMPLEMENTATION_CONFIG_VARIANT='VariantPreCompile'"!][!//
/**
* @brief Defines the use of Pre-Compile(PC) support
* @details VARIANT-PRE-COMPILE: Only parameters with "Pre-compile time" configu-ration are allowed 
*        in this variant.
*/
/* Pre-Compile(PC) Support. */
[!ELSEIF "IMPLEMENTATION_CONFIG_VARIANT='VariantPostBuild'"!][!//
/**
* @brief Defines the use of Post-Build(PB) support.
* @details VARIANT-POST-BUILD: Parameters with "Pre-compile time", 
*        "Link time" and "Post-build time" are allowed in this variant.
*/
/* Post-Build(PB) Support. */
[!ELSEIF "IMPLEMENTATION_CONFIG_VARIANT='VariantLinkTime'"!][!//
/**
* @brief Defines the use of Link-Time(L) support.
* @details VARIANT-LINK-TIME: Only parameters with "Pre-compile time" 
*        and "Link time" are allowed in this variant.
*/
[!ENDIF!][!//
/* Link Time Support. */
#define SPI_CONFIG_VARIANT      [!//
[!IF "IMPLEMENTATION_CONFIG_VARIANT='VariantPreCompile'"!][!//
(SPI_VARIANT_PRECOMPILE)
[!ELSEIF "IMPLEMENTATION_CONFIG_VARIANT='VariantPostBuild'"!][!//
(SPI_VARIANT_POSTBUILD)
[!ELSEIF "IMPLEMENTATION_CONFIG_VARIANT='VariantLinkTime'"!][!//
(SPI_VARIANT_LINKTIME)
[!ENDIF!][!//

/**
* @brief Define precompile support.
* @details Define precompile support if VariantPreCompile or VariantLinkTime is selected and number of variant <=1.
*/
#define SPI_PRECOMPILE_SUPPORT  [!//
[!IF "(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile' or IMPLEMENTATION_CONFIG_VARIANT='VariantLinkTime') and (variant:size() <= 1)"!][!//
    (STD_ON)
[!ELSE!][!//
    (STD_OFF)
[!ENDIF!][!//

/**
* @brief Defines the "Number of Loops" timeout.
* @details Defines the "Number of Loops" timeout used by Spi_SyncTransmit and Spi_AsyncTransmit
*        function during the wait on TX/RX transmission to complete one frame.
*        One timeout unit means that no TX or RX was executed(the IF statements are returning FALSE).
*/
#define SPI_TIMEOUT_COUNTER_U32       ((uint32)([!"num:i(SpiGeneral/SpiTransmitTimeout)"!]))

/**
* @brief Allow simultaneous calls to Spi_SyncTransmit() for different threads.
* @details Two concurrent calls to Spi_SyncTransmit() will be allowed only if the related sequences
*       do not share HW units.
*/
#define SPI_SUPPORT_CONCURRENT_SYNC_TRANSMIT    ([!IF "SpiGeneral/SpiSupportConcurrentSyncTransmit"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*==================================================================================================
 *                                           SpiAutosarExt DEFINES
==================================================================================================*/
/**
* @brief If enabled, the asyncronous operation mode (POLLING or INTERRUPT)
* @details If enabled, the asyncronous operation mode (POLLING or INTERRUPT) can
*       be defined independently for each HWUnit using Spi_SetHWUnitAsyncMode().
* @implements SPI_HWUNIT_ASYNC_MODE_define
*/
#define SPI_HWUNIT_ASYNC_MODE [!//
[!IF "(SpiGeneral/SpiLevelDelivered = 2)"!][!//
 ([!IF "SpiAutosarExt/SpiEnableHWUnitAsyncMode"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!ELSE!][!//
 (STD_OFF)
[!ENDIF!][!//

/**
* @brief If enabled, allows to configure more than 256 sequences, jobs and channels.
*/
#define SPI_ALLOW_BIGSIZE_COLLECTIONS  [!//
[!IF "(node:value(SpiAutosarExt/SpiAllowBigSizeCollections) = 'true')"!][!//
(STD_ON)
[!ELSE!][!//
(STD_OFF)
[!ENDIF!][!//

/**
* @brief If enabled, SPI_MAIN_FUNCTION_PERIOD defines the cycle time of the function Spi_MainFunction_Handling in seconds
*/
[!IF " node:exists(SpiGeneral/SpiMainFunctionPeriod)"!][!//
#define SPI_MAIN_FUNCTION_PERIOD            ([!"num:f(SpiGeneral/SpiMainFunctionPeriod)"!])
[!ENDIF!][!//

[!NOCODE!][!//
[!VAR "maxPartition" = "num:i(0)"!]
[!IF "SpiGeneral/SpiMulticoreSupport = 'true'"!]
    [!IF "node:exists(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition)"!]
        [!VAR "maxPartition" = "num:i(count(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition/*))"!] 
    [!ENDIF!]
    [!IF "$maxPartition = 0"!][!VAR "maxPartition" = "num:i(1)"!][!ENDIF!]
[!ELSE!]
    [!VAR "maxPartition"="num:i(1)"!]
[!ENDIF!]
[!ENDNOCODE!][!//
/**
* @brief Total number of partitions configured in Ecu.
*/
#define SPI_MAX_PARTITIONS             ([!"num:i($maxPartition)"!]U)

/**
* @brief          Enable Multicore Support.
* @details        When SpiGeneral/SpiMulticoreSupport = TRUE,
*                 the SPI driver can be configured to support multicore.
*/
#define SPI_MULTICORE_ENABLED          [!IF "SpiGeneral/SpiMulticoreSupport = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

#if (STD_OFF == SPI_MULTICORE_ENABLED)
/**
* @brief Defines default CodeId value which is assigned to HWUnits, Sequences, Jobs and Channels in the case multicore is not enabled.
*/
    #define SPI_SPURIOUS_CORE_ID_U32        ((uint32)0UL)
#endif

#if (STD_ON == SPI_MULTICORE_ENABLED)
    #define Spi_GetCoreID     ((uint32)OsIf_GetCoreID())
#else
    #define Spi_GetCoreID     SPI_SPURIOUS_CORE_ID_U32
#endif

/**
* @brief  Slave support
*/
#define SPI_SLAVE_SUPPORT SPI_IPW_SLAVE_SUPPORT

/**
* @brief Defines if transfers are made using DMA or FIFO.
* @details Defines if transfers are made using DMA or FIFO.
* @implements SPI_DMA_USED_define
*/
#define SPI_DMA_USED SPI_IPW_DMA_USED

/**
* @brief If enabled, allows dual MCU clock configuration settings.
* @details If enabled, allows dual MCU clock configuration settings.
* @implements SPI_DUAL_CLOCK_MODE_define
*/
#define SPI_DUAL_CLOCK_MODE SPI_IPW_DUAL_CLOCK_MODE

/**
* @brief If enabled, allows to Sequence transfer in Dma Fast mode.
*/
#define SPI_ENABLE_DMAFASTTRANSFER_SUPPORT  SPI_IPW_ENABLE_DMAFASTTRANSFER_SUPPORT

/**
* @brief Half duplex mdoe enable .
*/
#ifdef SPI_IPW_SPI_HALF_DUPLEX_MODE_SUPPORT
#define SPI_HALF_DUPLEX_MODE_SUPPORT  SPI_IPW_SPI_HALF_DUPLEX_MODE_SUPPORT
#endif
/*==================================================================================================
*                                            ENUMS
==================================================================================================*/


/*==================================================================================================
*                               STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/


/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/


/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/


#ifdef __cplusplus
}
#endif

#endif /*SPI_CFG_H*/

/** @} */
[!ENDCODE!][!//
