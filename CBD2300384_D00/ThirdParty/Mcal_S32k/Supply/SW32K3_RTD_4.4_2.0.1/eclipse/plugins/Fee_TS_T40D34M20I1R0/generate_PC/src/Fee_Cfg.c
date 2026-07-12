[!AUTOSPACING!]
[!CODE!]
/*==================================================================================================
* Project              : RTD AUTOSAR 4.4
* Platform             : CORTEXM
* Peripheral           : none
* Dependencies         : Fls
*
* Autosar Version      : 4.4.0
* Autosar Revision     : ASR_REL_4_4_REV_0000
* Autosar Conf.Variant :
* SW Version           : 2.0.1
* Build Version        : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
* (c) Copyright 2020 - 2022 NXP Semiconductors
* All Rights Reserved.
*
* NXP Confidential. This software is owned or controlled by NXP and may only be
* used strictly in accordance with the applicable license terms. By expressly
* accepting such terms or by downloading, installing, activating and/or otherwise
* using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms. If you do not agree to be
* bound by the applicable license terms, then you may not retain, install,
* activate or otherwise use the software.
==================================================================================================*/

/**
*   @file
*
*   @addtogroup FEE
*   @{
*/
/*   @implements Fee_Cfg.c_Artifact*/
#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Fee.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FEE_VENDOR_ID_CFG_C                      43
#define FEE_AR_RELEASE_MAJOR_VERSION_CFG_C       4
#define FEE_AR_RELEASE_MINOR_VERSION_CFG_C       4
#define FEE_AR_RELEASE_REVISION_VERSION_CFG_C    0
#define FEE_SW_MAJOR_VERSION_CFG_C               2
#define FEE_SW_MINOR_VERSION_CFG_C               0
#define FEE_SW_PATCH_VERSION_CFG_C               1

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
/* Check if source file and Fee header file are of the same vendor */
#if (FEE_VENDOR_ID_CFG_C != FEE_VENDOR_ID)
    #error "Fee_Cfg.c and Fee.h have different vendor ids"
#endif
/* Check if current file and Fee header file are of the same Autosar version */
#if ((FEE_AR_RELEASE_MAJOR_VERSION_CFG_C    != FEE_AR_RELEASE_MAJOR_VERSION) || \
     (FEE_AR_RELEASE_MINOR_VERSION_CFG_C    != FEE_AR_RELEASE_MINOR_VERSION) || \
     (FEE_AR_RELEASE_REVISION_VERSION_CFG_C != FEE_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Fee_Cfg.c and Fee.h are different"
#endif
/* Check if current file and Fee header file are of the same Software version */
#if ((FEE_SW_MAJOR_VERSION_CFG_C != FEE_SW_MAJOR_VERSION) || \
     (FEE_SW_MINOR_VERSION_CFG_C != FEE_SW_MINOR_VERSION) || \
     (FEE_SW_PATCH_VERSION_CFG_C != FEE_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Fee_Cfg.c and Fee.h are different"
#endif

[!INCLUDE "Fee_VersionCheck.m"!][!//


/*==================================================================================================
                                       GLOBAL VARIABLES
==================================================================================================*/
[!VAR "FeeVPS" = "num:i(FeeGeneral/FeeVirtualPageSize)"!]
[!VAR "FeeBlkOverhead" = "num:i(ceiling(12 div $FeeVPS + 2) * $FeeVPS)"!]
[!VAR "FeeClsOverhead" = "num:i(ceiling(16 div $FeeVPS + 2) * $FeeVPS)"!]
[!//
[!// Reserve 1-byte for BlockAssignment
[!VAR "FeeBlkFirstPartSize" = "$FeeBlkOverhead - ($FeeVPS * 2)"!][!//
[!IF "$FeeBlkFirstPartSize < 13"!][!// 2-byte block number + 2-byte block size + 4-byte block address + 4-byte checksum + 1-byte block assignment
    [!VAR "FeeBlkOverhead" = "num:i($FeeBlkOverhead + 13 - $FeeBlkFirstPartSize)"!]
[!ENDIF!][!//
[!//
[!// Generate sectors information data structure for Sector Retirement mode
[!VAR "FeeSectorRetirement" = "FeeGeneral/FeeSectorRetirement"!]
[!IF "$FeeSectorRetirement = 'true'"!]
[!//
[!VAR "FeeMaxSectorIndex"  = "num:max(FeeClusterGroup/*/FeeCluster/*/FeeSector/*/FeeSectorIndex)"!][!//
[!VAR "FeeSectorsInfoSize" = "num:i( ceiling(($FeeMaxSectorIndex + 1) div 8) )"!][!// Use 1 bit per sector
[!IF "bit:and($FeeSectorsInfoSize, 1) != 0"!][!//
    [!VAR "FeeSectorsInfoSize" = "$FeeSectorsInfoSize + 1"!][!// Round up to even number
[!ENDIF!][!//
[!VAR "FeeSectorsInfoSize" = "$FeeSectorsInfoSize + 2"!][!// Use 2 bytes for the number of sectors
[!// Round to the biggest page size
[!VAR "FlsBiggestPageSize" = "num:max( node:refs(FeeClusterGroup/*/FeeCluster/*/FeeSector/*/FeeSectorRef)/FlsPageSize )"!][!//
[!VAR "FeeSectorsInfoSize" = "num:i(ceiling($FeeSectorsInfoSize div $FlsBiggestPageSize) * $FlsBiggestPageSize)"!][!//
[!VAR "FeeClsFirstPartSize" = "$FeeClsOverhead - ($FeeVPS * 2)"!][!//
[!IF "$FeeClsFirstPartSize < (16 + FeeSectorsInfoSize)"!][!// expand the cluster header to have enough size for storing the sectors information
    [!VAR "FeeClsOverhead" = "num:i($FeeClsOverhead + (16 + FeeSectorsInfoSize) - $FeeClsFirstPartSize)"!][!//
[!ENDIF!][!//

#define FEE_START_SEC_VAR_INIT_UNSPECIFIED
#include "Fee_MemMap.h"

[!LOOP "FeeClusterGroup/*"!]
[!VAR "FeeCluterGrName" = "node:name(.)"!]
/* Sectors information of group [!"$FeeCluterGrName"!] */
[!LOOP "FeeCluster/*"!]
[!VAR "FeeCluterName" = "node:name(.)"!]
static Fee_SectorRuntimeInfoType Fee_[!"$FeeCluterGrName"!]_[!"$FeeCluterName"!][[!"num:i(count(FeeSector/*))"!]U] =
{
    [!VAR "FeeLoopIt" = "count(FeeSector/*)"!][!//
    [!LOOP "node:order(FeeSector/*, 'node:value(FeeSectorIndex)')"!][!// Iterate through the Fee Sector Index
    {
        [!VAR "FeeLoopIt" = "$FeeLoopIt - 1"!]
        [!"num:i(node:ref(FeeSectorRef)/FlsSectorStartaddress)"!]U,  /* Fls start address */
        [!"num:i((node:ref(./FeeSectorRef)/FlsSectorSize) * num:i(node:ref(./FeeSectorRef)/FlsNumberOfSectors))"!]U  /* Size */
    }[!IF "$FeeLoopIt != 0"!],[!ENDIF!][!CR!][!//
    [!ENDLOOP!]
};

[!ENDLOOP!][!//FeeCluster/*
[!ENDLOOP!][!//FeeClusterGroup/*

[!LOOP "FeeClusterGroup/*"!]
[!VAR "FeeCluterGrName" = "node:name(.)"!]

/* Clusters information of group [!"$FeeCluterGrName"!] */
static Fee_ClusterRuntimeInfoType Fee_[!"node:name(.)"!]_ClusterInfo[[!"num:i(count(FeeCluster/*))"!]U] =
{
    [!VAR "FeeLoopIt" = "count(FeeCluster/*)"!]
    [!LOOP "FeeCluster/*"!]
        [!VAR "FeeCluterName"  = "node:name(.)"!]
        [!VAR "FeeLoopIt"      = "$FeeLoopIt - 1"!]
        [!VAR "FeeClusterSize" = "0"!]
        [!LOOP "FeeSector/*"!]
            [!VAR "FeeClusterSize" = "$FeeClusterSize + num:i(node:ref(./FeeSectorRef)/FlsSectorSize) * num:i(node:ref(./FeeSectorRef)/FlsNumberOfSectors)"!]
        [!ENDLOOP!]
    /* [!"node:name(.)"!] */
    {
        [!"num:i($FeeClusterSize)"!]U, /* Cluster length */
        [!"num:i(count(FeeSector/*))"!]U,  /* Number of Sectors*/
        Fee_[!"$FeeCluterGrName"!]_[!"$FeeCluterName"!]  /* Sector set */
    }[!IF "$FeeLoopIt != 0"!],[!ENDIF!][!CR!][!//
    [!ENDLOOP!]
};

[!ENDLOOP!][!//FeeClusterGroup/*

/* List of pointer for information accross all cluster groups */
Fee_ClusterRuntimeInfoType *Fee_ClusterRuntimeInfo[FEE_NUMBER_OF_CLUSTER_GROUPS] =
{
[!VAR "FeeLoopIt" = "count(FeeClusterGroup/*)"!]
[!LOOP "FeeClusterGroup/*"!]
    [!VAR "FeeLoopIt" = "$FeeLoopIt - 1"!]
    Fee_[!"node:name(.)"!]_ClusterInfo[!IF "$FeeLoopIt != 0"!],[!ELSE!][!WS!][!ENDIF!]  /* [!"node:name(.)"!] */
[!ENDLOOP!]
};

#define FEE_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Fee_MemMap.h"

[!IF "num:i(count(FeeSectorToRecover/*)) > 0"!]
#define FEE_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fee_MemMap.h"

const Fee_SectorToRecoverType Fee_SectorToRecover[FEE_NUMBER_OF_SECTORS_TO_RECOVER] =
{
[!VAR "FeeLoopIt" = "count(FeeSectorToRecover/*)"!]
[!LOOP "FeeSectorToRecover/*"!]
    [!VAR "FeeLoopIt"       = "$FeeLoopIt - 1"!]
    [!VAR "FeeCluterGrName" = "text:split(./FeeSectorToRecoverRef, '/')[4]"!]
    [!VAR "FeeCluterName"   = "text:split(./FeeSectorToRecoverRef, '/')[5]"!]
    [!VAR "FeeSectorIndex"  = "node:ref(./FeeSectorToRecoverRef)/FeeSectorIndex"!]
    [!VAR "FeeSectorSize"   = "num:i(node:ref(node:ref(./FeeSectorToRecoverRef)/FeeSectorRef)/FlsSectorSize * node:ref(node:ref(./FeeSectorToRecoverRef)/FeeSectorRef)/FlsNumberOfSectors)"!]
    /* [!"node:name(.)"!] */
    {
        &(Fee_[!"$FeeCluterGrName"!]_[!"$FeeCluterName"!][[!"$FeeSectorIndex"!]U]),  /* Sector to recover */
        [!"$FeeSectorSize"!]U  /* Sector size */
    }[!IF "$FeeLoopIt != 0"!],[!ENDIF!][!CR!][!//
[!ENDLOOP!]
};

#define FEE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fee_MemMap.h"
[!ENDIF!][!//FeeSectorToRecover

[!ENDIF!][!//FeeSectorRetirement


#define FEE_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fee_MemMap.h"

[!VAR "FeeClusterStartAddress" = "0"!]
[!VAR "FeeEmulationAddress" = "0"!]
[!LOOP "FeeClusterGroup/*"!]
[!VAR "FeeCluterGrName" = "node:name(.)"!]

/* Configuration of cluster group [!"$FeeCluterGrName"!] */
static const Fee_ClusterType Fee_[!"node:name(.)"!][[!"num:i(count(FeeCluster/*))"!]] =
{
    [!VAR "FeeLoopIt" = "count(FeeCluster/*)"!]
    [!VAR "comma" = "' '"!]
    [!LOOP "FeeCluster/*"!]
        [!VAR "FeeLoopIt"      = "$FeeLoopIt - 1"!]
        [!VAR "FeeClusterSize" = "0"!]
        [!LOOP "FeeSector/*"!]
            [!VAR "FeeClusterSize" = "$FeeClusterSize + num:i(node:ref(./FeeSectorRef)/FlsSectorSize) * num:i(node:ref(./FeeSectorRef)/FlsNumberOfSectors)"!]
        [!ENDLOOP!]
        [!IF "$FeeSectorRetirement = 'true'"!][!// Use the emulations address
            [!VAR "FeeClusterStartAddress" = "$FeeEmulationAddress"!][!//
            [!VAR "FeeEmulationAddress"    = "$FeeEmulationAddress + $FeeClusterSize"!][!// Jump to the next cluster
        [!ELSE!][!// Use the Fls address from the first FeeSectorRef
            [!VAR "FeeClusterStartAddress" = "num:i(node:ref(FeeSector/*[FeeSectorIndex = 0]/FeeSectorRef)/FlsSectorStartaddress)"!]
        [!ENDIF!]
    /* [!"node:name(.)"!] */
    {
[!IF "$FeeSectorRetirement = 'false'"!][!//
        [!VAR "comma" = "','"!]
[!ENDIF!][!//
        [!"num:i($FeeClusterStartAddress)"!]U[!"$comma"!] /* Start address */
[!IF "$FeeSectorRetirement = 'false'"!][!//
        [!"num:i($FeeClusterSize)"!]U /* Size */
[!ENDIF!][!//
    }[!IF "$FeeLoopIt != 0"!],[!ENDIF!][!CR!][!//
    [!ENDLOOP!]
};
[!ENDLOOP!]

/* Configuration of cluster group set */
const Fee_ClusterGroupType Fee_ClrGrps[FEE_NUMBER_OF_CLUSTER_GROUPS] =
{
[!VAR "FeeLoopIt" = "count(FeeClusterGroup/*)"!]
[!LOOP "FeeClusterGroup/*"!]
    [!VAR "FeeLoopIt" = "$FeeLoopIt - 1"!]
    [!// Compute the size of the reserved area (immediate block headers and aligned data)
    [!VAR "FeeReservedSize" = "0"!][!//
    [!LOOP "../../FeeBlockConfiguration/*[node:name(node:ref(FeeClusterGroupRef)) = node:current()/@name]"!]
        [!IF "./FeeImmediateData"!]
            [!VAR "FeeReservedSize" = "$FeeReservedSize + $FeeBlkOverhead + [!//
                             num:i( ceiling( ./FeeBlockSize div $FeeVPS ) * $FeeVPS )"!]
        [!ENDIF!]
    [!ENDLOOP!]     
    /* [!"node:name(.)"!] */
    {
        Fee_[!"node:name(.)"!], /* Cluster set */
        [!"num:i(count(./FeeCluster/*))"!]U, /* Number of clusters */
        [!"num:i( $FeeReservedSize )"!]U /* Size of the reserved area */
    }[!IF "$FeeLoopIt != 0"!],[!ENDIF!][!//

[!ENDLOOP!]
};

/* Configuration of Fee blocks */
const Fee_BlockConfigType Fee_BlockConfig[FEE_CRT_CFG_NR_OF_BLOCKS] =
{
[!VAR "FeeLoopIt" = "count(FeeBlockConfiguration/*)"!]
[!LOOP "node:order(FeeBlockConfiguration/*,'node:value(./FeeBlockNumber)')"!]
    [!VAR "FeeLoopIt" = "$FeeLoopIt - 1"!]
    /* [!"node:name(.)"!] */
    {
        FeeConf_FeeBlockConfiguration_[!"node:name(.)"!], /* FeeBlockNumber symbol */
        [!"./FeeBlockSize"!]U, /* FeeBlockSize */
        [!"node:ref(./FeeClusterGroupRef)/@index"!]U, /* FeeClusterGroup */
        (boolean) [!IF "./FeeImmediateData"!]TRUE [!ELSE!]FALSE[!ENDIF!],[!//
   /* FeeImmediateData */
#if (FEE_SWAP_FOREIGN_BLOCKS_ENABLED == STD_ON) 
        FEE_PROJECT_[!"node:value(./FeeBlockAssignment)"!] /* Fee Block Assignment to a project */
#else
        FEE_PROJECT_RESERVED
#endif
    }[!IF "$FeeLoopIt != 0"!],[!ENDIF!][!//

[!ENDLOOP!]
};
[!ENDCODE!]

#define FEE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fee_MemMap.h"

#ifdef __cplusplus
}
#endif
/** @}*/
