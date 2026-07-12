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

/**   
*   @file    Spi_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c
*   @version 2.0.1
*
*   @brief   AUTOSAR Spi - Post-Build(PB) configuration file code template.
*   @details Code template for Post-Build(PB) configuration file generation.
*
*   @addtogroup SPI_DRIVER_CONFIGURATION Spi Driver Configuration
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Spi_Ipw_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.h"
#include "Lpspi_Ip_Cfg.h"
#include "Flexio_Spi_Ip_Cfg.h"
#include "Spi.h"


/*==================================================================================================
*                                    SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_PBCFG_C                        43
#define SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_PBCFG_C         4
#define SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_PBCFG_C         4
#define SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_PBCFG_C      0
#define SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_PBCFG_C                 2
#define SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_PBCFG_C                 0
#define SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_PBCFG_C                 1
/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if Spi_Ipw_PBcfg.h and Spi_Ipw_PBcfg.c are of the same vendor */
#if (SPI_IPW_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_PBCFG_C)
    #error "Spi_Ipw_PBcfg.h and Spi_Ipw_PBcfg.c have different vendor ids"
#endif
/* Check if Spi_Ipw_PBcfg.h file and Spi_Ipw_PBcfg.c file are of the same Autosar version */
#if ((SPI_IPW_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_PBCFG_C) || \
     (SPI_IPW_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_PBCFG_C) || \
     (SPI_IPW_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_PBCFG_C))
#error "AutoSar Version Numbers of Spi_Ipw_PBcfg.h and Spi_Ipw_PBcfg.c are different"
#endif
#if ((SPI_IPW_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_PBCFG_C) || \
     (SPI_IPW_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_PBCFG_C) || \
     (SPI_IPW_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_H != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_PBCFG_C))
#error "Software Version Numbers of Spi_Ipw_PBcfg.h and Spi_Ipw_PBcfg.c are different"
#endif

/* Check if Lpspi_Ip_Cfg.h and Spi_Ipw_PBcfg.c are of the same vendor */
#if (LPSPI_IP_VENDOR_ID_CFG != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_PBCFG_C)
    #error "Lpspi_Ip_Cfg.h and Spi_Ipw_PBcfg.c have different vendor ids"
#endif
/* Check if Lpspi_Ip_Cfg.h file and Spi_Ipw_PBcfg.c file are of the same Autosar version */
#if ((LPSPI_IP_AR_RELEASE_MAJOR_VERSION_CFG != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_PBCFG_C) || \
     (LPSPI_IP_AR_RELEASE_MINOR_VERSION_CFG != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_PBCFG_C) || \
     (LPSPI_IP_AR_RELEASE_REVISION_VERSION_CFG != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_PBCFG_C))
#error "AutoSar Version Numbers of Lpspi_Ip_Cfg.h and Spi_Ipw_PBcfg.c are different"
#endif
#if ((LPSPI_IP_SW_MAJOR_VERSION_CFG != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_PBCFG_C) || \
     (LPSPI_IP_SW_MINOR_VERSION_CFG != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_PBCFG_C) || \
     (LPSPI_IP_SW_PATCH_VERSION_CFG != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_PBCFG_C))
#error "Software Version Numbers of Lpspi_Ip_Cfg.h and Spi_Ipw_PBcfg.c are different"
#endif

/* Check if Spi.h and Spi_Ipw_PBcfg.c are of the same vendor */
#if (SPI_VENDOR_ID != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_PBCFG_C)
    #error "Spi.h and Spi_Ipw_PBcfg.c have different vendor ids"
#endif
/* Check if Spi.h file and Spi_Ipw_PBcfg.c file are of the same Autosar version */
#if ((SPI_AR_RELEASE_MAJOR_VERSION != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_PBCFG_C) || \
     (SPI_AR_RELEASE_MINOR_VERSION != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_PBCFG_C) || \
     (SPI_AR_RELEASE_REVISION_VERSION != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_PBCFG_C))
#error "AutoSar Version Numbers of Spi.h and Spi_Ipw_PBcfg.c are different"
#endif
#if ((SPI_SW_MAJOR_VERSION != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_PBCFG_C) || \
     (SPI_SW_MINOR_VERSION != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_PBCFG_C) || \
     (SPI_SW_PATCH_VERSION != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_PBCFG_C))
#error "Software Version Numbers of Spi.h and Spi_Ipw_PBcfg.c are different"
#endif

/* Check if Flexio_Spi_Ip_Cfg.h and Spi_Ipw_PBcfg.c are of the same vendor */
#if (FLEXIO_SPI_IP_VENDOR_ID_CFG != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_PBCFG_C)
    #error "Flexio_Spi_Ip_Cfg.h and Spi_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c have different vendor ids"
#endif
/* Check if Flexio_Spi_Ip_Cfg.h file and Spi_Ipw_PBcfg.c file are of the same Autosar version */
#if ((FLEXIO_SPI_IP_AR_RELEASE_MAJOR_VERSION_CFG != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_PBCFG_C) || \
     (FLEXIO_SPI_IP_AR_RELEASE_MINOR_VERSION_CFG != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_PBCFG_C) || \
     (FLEXIO_SPI_IP_AR_RELEASE_REVISION_VERSION_CFG != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_PBCFG_C))
    #error "AutoSar Version Numbers of Flexio_Spi_Ip_Cfg.h and Spi_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c are different"
#endif

#if ((FLEXIO_SPI_IP_SW_MAJOR_VERSION_CFG != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_PBCFG_C) || \
     (FLEXIO_SPI_IP_SW_MINOR_VERSION_CFG != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_PBCFG_C) || \
     (FLEXIO_SPI_IP_SW_PATCH_VERSION_CFG != SPI_IPW[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_PBCFG_C))
    #error "Software Version Numbers of Flexio_Spi_Ip_Cfg.h and Spi_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c are different"
#endif
/*==================================================================================================
*                         LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/


/*==================================================================================================
*                                  LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
#define SPI_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Spi_MemMap.h"

[!NOCODE!][!//
[!LOOP "SpiGeneral/SpiPhyUnit/*"!][!//
[!IF "node:exists(SpiMaxDmaFastTransfer)"!][!//
[!CODE!][!//
/* Lpspi_Ipw_HwUnitDmaFastCfg_[!"@name"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] Dma Fast transfer Configuration */
static Lpspi_Ip_FastTransferType Lpspi_Ipw_HwUnitDmaFastCfg_[!"@name"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i(SpiMaxDmaFastTransfer)"!]U];
[!ENDCODE!][!//
[!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!][!//

#define SPI_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Spi_MemMap.h"

#define SPI_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Spi_MemMap.h"

[!NOCODE!][!//
    [!VAR "spiPath" = "node:path(.)"!][!//
    
    [!LOOP "SpiDriver/SpiExternalDevice/*"!][!//
    [!VAR "CSIB_number"="string(substring-after(node:value(SpiHwUnit),'CSIB')+1)"!][!//
[!CODE!][!//
/* External device Configuration of [!"name(.)"!]*/
static const Spi_ExternalDeviceConfigType Spi_ExternalDeviceConfig_[!"name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
        /* [!"name(.)"!]*/
[!ENDCODE!][!//
        [!IF "contains((substring-before((node:value(concat($spiPath,'/SpiGeneral/SpiPhyUnit/*[',$CSIB_number,']/SpiPhyUnitMapping'))),'_')),'LPSPI')"!][!//
[!CODE!][!//
        SPI_OVER_LPSPI, /* IpType */
[!ENDCODE!][!//
        [!ELSE!]
[!CODE!][!//
        SPI_OVER_FLEXIO, /* IpType */
[!ENDCODE!][!//
        [!ENDIF!][!//
[!CODE!][!//
        [!"num:i(substring-after((node:value(concat($spiPath,'/SpiGeneral/SpiPhyUnit/*[',$CSIB_number,']/SpiPhyUnitMapping'))),'SPI_'))"!]U,  /* Instance */
        [!IF "contains((substring-before((node:value(concat($spiPath,'/SpiGeneral/SpiPhyUnit/*[',$CSIB_number,']/SpiPhyUnitMapping'))),'_')),'LPSPI')"!]
        {   /* ExternalDeviceConfig */
            &Lpspi_Ip_DeviceAttributes_[!"@name"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] /* LPSPI ExternalDeviceConfig */
#if (SPI_IPW_SPI_FLEXIO_ENABLE == STD_ON)
            ,NULL_PTR /* Don't have FLEXIO_SPI ExternalDeviceConfig */
#endif
        },
        [!ELSE!]
        {   /* ExternalDeviceConfig */
            NULL_PTR, /* LpspiIpConfig */
#if (SPI_IPW_SPI_FLEXIO_ENABLE == STD_ON)            
            &Flexio_Spi_Ip_DeviceAttributes_[!"@name"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] /* DspiIpConfig */
#endif
        },
        [!ENDIF!]
[!ENDCODE!][!//
        [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
            [!VAR "CurrentPName"="99"!][!//
            [!IF "(count(SpiDeviceEcucPartitionRef/*))=1"!]
                [!VAR "CurrentPName" = "node:name(node:ref(SpiDeviceEcucPartitionRef/*[1]))"!]
            [!ENDIF!]
            [!IF "$CurrentPName = 99.0"!]
                [!CODE!]        (uint32)99U /* This channel don't assign to any core */[!ENDCODE!]    
            [!ELSE!]
                [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
                    [!IF "$CurrentPName = node:name(node:ref(./OsAppEcucPartitionRef))"!][!//
                        [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
                            [!VAR "CrtCore"="num:i(node:value(./EcucCoreId))"!]
                [!CODE!]        (uint32)[!"$CrtCore"!]U /* Core has assigned */[!ENDCODE!]
                        [!ENDSELECT!][!//
                    [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ENDIF!]
        [!ELSE!]
            [!CODE!]        SPI_SPURIOUS_CORE_ID_U32[!ENDCODE!]    
        [!ENDIF!]
[!CODE!][!//
};
[!ENDCODE!][!//
    [!ENDLOOP!][!//
    
    [!//----------------------------------------------------------------------
    [!VAR "count" = "count(SpiDriver/SpiExternalDevice/*)"!][!//
    
    [!VAR "SpiConfiguredPartitions" = "num:i(count(SpiGeneral/SpiEcucPartitionRef/*))"!]
    [!VAR "TotalPartition" = "num:i(count(SpiGeneral/SpiEcucPartitionRef/*))"!]
    [!IF "$SpiConfiguredPartitions = 0"!]
        [!VAR "TotalPartition" = "num:i(1)"!]
    [!ENDIF!]
    [!FOR "PartitionCount" = "1" TO "$TotalPartition"!]
        [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
            [!VAR "CurrentPartition" = "node:name(node:ref(SpiGeneral/SpiEcucPartitionRef/*[num:i($PartitionCount)]))"!]
            [!VAR "CurrentPartitionUnderScore" = "concat('_',node:name(node:ref(SpiGeneral/SpiEcucPartitionRef/*[num:i($PartitionCount)])))"!]
        [!ELSE!]
            [!VAR "CurrentPartitionUnderScore" = "string(null)"!]
        [!ENDIF!]
        [!VAR "externalDeviceIndex" = "num:i(0)"!][!//
[!CODE!][!//
/* Spi_apExternalDeviceConfigList[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] External device Configuration of [!"name(.)"!]*/
const Spi_ExDevicesConfigType Spi_aExternalDeviceConfigList[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!"$CurrentPartitionUnderScore"!][[!"num:i($count)"!]U] =
{
[!ENDCODE!][!//
    [!LOOP "SpiDriver/SpiExternalDevice/*"!][!//
[!CODE!][!//
    {
    /* [!"name(.)"!] */
[!ENDCODE!]
        [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
            [!VAR "CurrentPName"="99"!][!//
            [!IF "(count(SpiDeviceEcucPartitionRef/*))=1"!]
                [!VAR "CurrentPName" = "node:name(node:ref(SpiDeviceEcucPartitionRef/*[1]))"!]
            [!ENDIF!]
            [!IF "$CurrentPName = $CurrentPartition"!]
[!CODE!][!//
    &Spi_ExternalDeviceConfig_[!"name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
[!ENDCODE!]
            [!ELSE!]
[!CODE!][!//
    NULL_PTR  /* Not allocated to this partition */
[!ENDCODE!]
            [!ENDIF!]
        [!ELSE!]
[!CODE!][!//
    &Spi_ExternalDeviceConfig_[!"name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
[!ENDCODE!]
        [!ENDIF!]
[!CODE!][!//
    }[!IF "$externalDeviceIndex!=($count - 1)"!],[!ENDIF!]
[!ENDCODE!][!//
    [!VAR "externalDeviceIndex" = "$externalDeviceIndex + 1"!][!//
    [!ENDLOOP!][!//
[!CODE!][!//
};

[!ENDCODE!][!//
    [!ENDFOR!][!//
    
    [!//----------------------------------------------------------------------
    [!LOOP "SpiGeneral/SpiPhyUnit/*"!][!//
        [!VAR "index"="substring-after(SpiPhyUnitMapping,'SPI_')"!][!//
        
    [!VAR "CurrentPName"="99"!][!//   
    [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
        [!VAR "CoreID"="99"!][!//
        [!VAR "CSIB_PhysIdex"="concat('CSIB',@index)"!]
        [!SELECT "as:modconf('Spi')[1]/SpiDriver/SpiExternalDevice/*[SpiHwUnit=$CSIB_PhysIdex]"!][!//
            [!VAR "CurrentPName" = "node:name(node:ref(SpiDeviceEcucPartitionRef/*[1]))"!]
        [!ENDSELECT!]
        [!IF "$CurrentPName = 99.0"!]

        [!ELSE!]
            [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
                [!IF "$CurrentPName = node:name(node:ref(./OsAppEcucPartitionRef))"!][!//
                    [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
                        [!VAR "CoreID"="num:i(node:value(./EcucCoreId))"!]
                    [!ENDSELECT!][!//
                [!ENDIF!][!//​
            [!ENDLOOP!][!//
        [!ENDIF!][!//​
    [!ENDIF!]
    
[!IF "(as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true') and ($CurrentPName = 99.0)"!][!// Do not generate this structure because multicore was enable but this HW did not select any partition
[!ELSE!]
[!CODE!][!//
/* Hardware Unit configuration for [!"name(.)"!] */
static const Spi_HWUnitConfigType Spi_HwUnitConfig_[!"name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
        SPI_OVER_[!"substring-before(SpiPhyUnitMapping,'_')"!], /* IpType */
        (uint8)[!"$index"!]U, /* Instance */
[!ENDCODE!][!//
    [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
        [!IF "$CurrentPName = 99.0"!]
[!CODE!][!//
        99U, /* This hw unit is not assigned to any partition */
[!ENDCODE!][!//
        [!ELSE!]
[!CODE!][!//
        [!"num:i($CoreID)"!]U, /* SpiCoreUse */
[!ENDCODE!][!//
        [!ENDIF!][!//​
    [!ELSE!]
[!CODE!][!//
        SPI_SPURIOUS_CORE_ID_U32, /* SpiCoreUse */
[!ENDCODE!][!//
    [!ENDIF!]
[!CODE!][!//
        {
[!ENDCODE!][!//
            [!IF "contains(node:value(SpiPhyUnitMapping),'LPSPI')"!]
[!CODE!][!//
            &Lpspi_Ip_PhyUnitConfig_[!"@name"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] /* LPSPI IpConfig */
#if (SPI_IPW_SPI_FLEXIO_ENABLE == STD_ON)
            ,NULL_PTR /* Don't have Ipconfig of FLEXIO_SPI */
#endif
[!ENDCODE!][!//
            [!ELSE!]
[!CODE!][!//
            NULL_PTR, /* Don't have Ipconfig of LPSPI */
#if (SPI_IPW_SPI_FLEXIO_ENABLE == STD_ON)            
            &Flexio_Spi_Ip_PhyUnitConfig_[!"@name"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] /* FLEXIO_SPI IpConfig */
#endif
[!ENDCODE!][!//
            [!ENDIF!]
[!CODE!][!//
            #if ((LPSPI_IP_DMA_USED == STD_ON) && (LPSPI_IP_ENABLE_DMAFASTTRANSFER_SUPPORT == STD_ON))
            ,[!IF "node:exists(SpiMaxDmaFastTransfer)"!]Lpspi_Ipw_HwUnitDmaFastCfg_[!"@name"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ELSE!]NULL_PTR[!ENDIF!]
            #endif
        },
        [!IF "SpiPhyUnitSync = 'true'"!]SPI_PHYUNIT_SYNC_U32[!ELSE!]SPI_PHYUNIT_ASYNC_U32[!ENDIF!] /* IsSync */
};
[!ENDCODE!][!//
[!ENDIF!]
    [!ENDLOOP!][!//
    
[!//----------------------------------------------------------------------
[!VAR "SpiConfiguredPartitions" = "num:i(count(SpiGeneral/SpiEcucPartitionRef/*))"!]
[!VAR "TotalPartition" = "num:i(count(SpiGeneral/SpiEcucPartitionRef/*))"!]
[!IF "$SpiConfiguredPartitions = 0"!]
    [!VAR "TotalPartition" = "num:i(1)"!]
[!ENDIF!]
[!FOR "PartitionCount" = "1" TO "$TotalPartition"!]
    [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
        [!VAR "CurrentPartition" = "node:name(node:ref(SpiGeneral/SpiEcucPartitionRef/*[num:i($PartitionCount)]))"!]
        [!VAR "CurrentPartitionUnderScore" = "concat('_',node:name(node:ref(SpiGeneral/SpiEcucPartitionRef/*[num:i($PartitionCount)])))"!]
    [!ELSE!]
        [!VAR "CurrentPartitionUnderScore" = "string(null)"!]
    [!ENDIF!]
[!CODE!][!//
/* Array of Hardware Unit configurations */
const Spi_PhyUnitsConfigType Spi_aHwUnitConfigList[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!"$CurrentPartitionUnderScore"!][SPI_MAX_HWUNIT] =
{
[!ENDCODE!][!//
    [!VAR "hwUnitIndex"= "0"!]
    [!VAR "numdevices"= "num:i(count(SpiGeneral/SpiPhyUnit/*))-1"!][!//
    [!LOOP "SpiGeneral/SpiPhyUnit/*"!][!//
[!CODE!][!//
    /* [!"name(.)"!] */
    {
[!ENDCODE!]
        [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
            [!VAR "CurrentPName"="99"!][!//
            [!VAR "CSIB_PhysIdex"="concat('CSIB',@index)"!]
            [!SELECT "as:modconf('Spi')[1]/SpiDriver/SpiExternalDevice/*[SpiHwUnit=$CSIB_PhysIdex]"!][!//
                [!IF "node:exists(node:ref(SpiDeviceEcucPartitionRef/*[1]))"!]
                    [!VAR "CurrentPName" = "node:name(node:ref(SpiDeviceEcucPartitionRef/*[1]))"!]
                [!ENDIF!]
            [!ENDSELECT!]
            [!IF "$CurrentPName = $CurrentPartition"!]
[!CODE!][!//
    &Spi_HwUnitConfig_[!"name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
[!ENDCODE!]
            [!ELSE!]
[!CODE!][!//
    NULL_PTR  /* Not allocated to this partition */
[!ENDCODE!]
            [!ENDIF!]
        [!ELSE!]
[!CODE!][!//
    &Spi_HwUnitConfig_[!"name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
[!ENDCODE!]
        [!ENDIF!]
[!CODE!][!//
    }[!IF "$hwUnitIndex != $numdevices"!],[!ENDIF!][!VAR "hwUnitIndex"= "$hwUnitIndex + 1"!]
[!ENDCODE!][!//
    [!ENDLOOP!][!//
[!CODE!][!//
};
[!ENDCODE!][!//
[!ENDFOR!][!//
[!ENDNOCODE!][!//

#define SPI_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Spi_MemMap.h"

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


#ifdef __cplusplus
}
#endif

/** @} */
[!ENDCODE!]
