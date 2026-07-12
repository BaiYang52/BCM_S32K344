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

#ifndef FEE_CFG_H
#define FEE_CFG_H

/**
*   @file
*
*   @addtogroup FEE
*   @{
*/
/*   @implements Fee_Cfg.h_Artifact*/
#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Fee_Types.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define FEE_VENDOR_ID_CFG                    43
#define FEE_AR_RELEASE_MAJOR_VERSION_CFG     4
#define FEE_AR_RELEASE_MINOR_VERSION_CFG     4
#define FEE_AR_RELEASE_REVISION_VERSION_CFG  0
#define FEE_SW_MAJOR_VERSION_CFG             2
#define FEE_SW_MINOR_VERSION_CFG             0
#define FEE_SW_PATCH_VERSION_CFG             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and MODULENAME configuration header file are of the same vendor */
#if (FEE_VENDOR_ID_CFG != FEE_TYPES_VENDOR_ID)
    #error "Fee_Types.h and Fee_Cfg.h have different vendor ids"
#endif
/* Check if current file and MODULENAME configuration header file are
   of the same Autosar version
*/
#if ((FEE_AR_RELEASE_MAJOR_VERSION_CFG    != FEE_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (FEE_AR_RELEASE_MINOR_VERSION_CFG    != FEE_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (FEE_AR_RELEASE_REVISION_VERSION_CFG != FEE_TYPES_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Fee_Types.h and Fee_Cfg.h are different"
#endif
/* Check if current file and MODULENAME configuration header file are
   of the same software version
*/
#if ((FEE_SW_MAJOR_VERSION_CFG != FEE_TYPES_SW_MAJOR_VERSION) || \
     (FEE_SW_MINOR_VERSION_CFG != FEE_TYPES_SW_MINOR_VERSION) || \
     (FEE_SW_PATCH_VERSION_CFG != FEE_TYPES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Fee_Types.h and Fee_Cfg.h are different"
#endif
[!VAR "NvmEndFlg" = "0"!]
[!IF "node:exists(FeeGeneral/FeeNvmJobEndNotification)"!]
    [!IF "not(text:match(normalize-space(FeeGeneral/FeeNvmJobEndNotification),'^[_a-zA-Z][_0-9a-zA-Z]*$'))"!]
        [!ERROR!]Invalid name of the Fee Nvm Job End Notification routine. Must be a valid C function name, NULL_PTR or NULL.[!ENDERROR!]
    [!ELSE!]
        [!VAR "NvmEndFlg" = "node:when((normalize-space(FeeGeneral/FeeNvmJobEndNotification) = 'NULL_PTR') or (normalize-space(FeeGeneral/FeeNvmJobEndNotification) = 'NULL'),0,1)"!]
    [!ENDIF!]
[!ENDIF!]
[!VAR "NvmErrFlg" = "0"!]
[!IF "node:exists(FeeGeneral/FeeNvmJobErrorNotification)"!]
    [!IF "not(text:match(normalize-space(FeeGeneral/FeeNvmJobErrorNotification),'^[_a-zA-Z][_0-9a-zA-Z]*$'))"!]
        [!ERROR!]Invalid name of the Fee Nvm Job Error Notification routine. Must be a valid C function name, NULL_PTR or NULL.[!ENDERROR!]    
    [!ELSE!]
        [!VAR "NvmErrFlg" = "node:when((normalize-space(FeeGeneral/FeeNvmJobErrorNotification) = 'NULL_PTR') or (normalize-space(FeeGeneral/FeeNvmJobErrorNotification) = 'NULL'),0,1)"!]
    [!ENDIF!]
[!ENDIF!]
[!VAR "NvmClusterFormatFlg" = "0"!]
[!IF "node:exists(FeeGeneral/FeeClusterFormatNotification)"!]
    [!IF "not(text:match(normalize-space(FeeGeneral/FeeClusterFormatNotification),'^[_a-zA-Z][_0-9a-zA-Z]*$'))"!]
        [!ERROR!]Invalid name of the Fee Cluster Format During Init Notification routine. Must be a valid C function name, NULL_PTR or NULL.[!ENDERROR!]    
    [!ELSE!]
        [!VAR "NvmClusterFormatFlg" = "node:when((normalize-space(FeeGeneral/FeeClusterFormatNotification) = 'NULL_PTR') or (normalize-space(FeeGeneral/FeeClusterFormatNotification) = 'NULL'),0,1)"!]
    [!ENDIF!]
[!ENDIF!]

[!INCLUDE "Fee_VersionCheck.m"!][!//

/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/
[!VAR "FeeVPS" = "num:i(FeeGeneral/FeeVirtualPageSize)"!]
[!VAR "FeeBlkOverhead" = "num:i(ceiling(12 div $FeeVPS + 2) * $FeeVPS)"!]
[!VAR "FeeClsOverhead" = "num:i(ceiling(16 div $FeeVPS + 2) * $FeeVPS)"!]
[!VAR "FeeDataBufferSize" = "num:i(FeeGeneral/FeeDataBufferSize)"!]
[!//
[!// Reserve 1-byte for BlockAssignment
[!VAR "FeeBlkFirstPartSize" = "$FeeBlkOverhead - ($FeeVPS * 2)"!][!//
[!IF "$FeeBlkFirstPartSize < 13"!][!// 2-byte block number + 2-byte block size + 4-byte block address + 4-byte checksum + 1-byte block assignment
    [!VAR "FeeBlkOverhead" = "num:i($FeeBlkOverhead + 13 - $FeeBlkFirstPartSize)"!]
[!ENDIF!][!//
[!//
[!IF "FeeGeneral/FeeSectorRetirement = 'true'"!]
    [!VAR "FeeMaxSectorIndex"  = "num:max(FeeClusterGroup/*/FeeCluster/*/FeeSector/*/FeeSectorIndex)"!][!//
    [!VAR "FeeSectorsInfoSize" = "num:i( ceiling(($FeeMaxSectorIndex + 1) div 8) )"!][!// Use 1 bit per sector
    [!IF "bit:and($FeeSectorsInfoSize, 1) != 0"!]
        [!VAR "FeeSectorsInfoSize" = "$FeeSectorsInfoSize + 1"!][!// Round up to even number
    [!ENDIF!]
    [!VAR "FeeSectorsInfoSize" = "$FeeSectorsInfoSize + 2"!][!// Use 2 bytes for the number of sectors
    [!// Round to the biggest page size
    [!VAR "FlsBiggestPageSize" = "num:max( node:refs(FeeClusterGroup/*/FeeCluster/*/FeeSector/*/FeeSectorRef)/FlsPageSize )"!]
    [!VAR "FeeSectorsInfoSize" = "num:i(ceiling($FeeSectorsInfoSize div $FlsBiggestPageSize) * $FlsBiggestPageSize)"!]

    [!VAR "FeeClsFirstPartSize" = "$FeeClsOverhead - ($FeeVPS * 2)"!]

    [!IF "$FeeClsFirstPartSize < (16 + FeeSectorsInfoSize)"!][!// expand the cluster header to have enough size for storing the sectors information
        [!VAR "FeeClsOverhead" = "num:i($FeeClsOverhead + (16 + FeeSectorsInfoSize) - $FeeClsFirstPartSize)"!]
    [!ENDIF!]
    [!IF "$FeeDataBufferSize < $FeeClsOverhead"!][!// expand the internal buffer too, it must be greater than or equal to the cluster header overhead
        [!VAR "FeeDataBufferSize" = "$FeeClsOverhead"!]
    [!ENDIF!]
[!ENDIF!]

/* Pre-processor switch to enable and disable development error detection */
/** @implements   FeeDevErrorDetect_Object */
#define FEE_DEV_ERROR_DETECT                      ([!IF "FeeGeneral/FeeDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Pre-processor switch to enable / disable the API to read out the modules
    version information */
/** @implements   FeeVersionInfoApi_Object */
#define FEE_VERSION_INFO_API                      ([!IF "FeeGeneral/FeeVersionInfoApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Compiler switch to enable/disable the SetMode functionality of the module */
/** @implements   FeeSetModeSupported_Object */
#define FEE_SETMODE_API_SUPPORTED                 ([!IF "FeeGeneral/FeeSetModeSupported"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

[!SELECT "as:modconf('Fls')"!]
/* Pre-processor switch to enable /disable the Fls_Cancel function of the module */
#define FEE_CANCEL_API                            ([!IF "FlsGeneral/FlsCancelApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!ENDSELECT!]

/* The size in bytes to which logical blocks shall be aligned */
/** @implements   FeeVirtualPageSize_Object */
#define FEE_VIRTUAL_PAGE_SIZE                     ([!"$FeeVPS"!]U)

/* Job end notification routine provided by the upper layer module (declaration) */
/** @implements   FeeNvmJobEndNotification_Object */

#define FEE_NVM_JOB_END_NOTIFICATION_DECL         [!//
[!IF "$NvmEndFlg = '1.0'"!]extern void [!"normalize-space(FeeGeneral/FeeNvmJobEndNotification)"!](void);[!ENDIF!]

/* Job end notification routine provided by the upper layer module */
/** @implements   FeeNvmJobEndNotification_Object */

#define FEE_NVM_JOB_END_NOTIFICATION              [!//
[!IF "$NvmEndFlg = '1.0'"!][!"normalize-space(FeeGeneral/FeeNvmJobEndNotification)"!]();[!ENDIF!]

/* Job error notification routine provided by the upper layer module (declaration) */
/** @implements   FeeNvmJobErrorNotification_Object */

#define FEE_NVM_JOB_ERROR_NOTIFICATION_DECL       [!//
[!IF "$NvmErrFlg = '1.0'"!]extern void [!"normalize-space(FeeGeneral/FeeNvmJobErrorNotification)"!](void);[!ENDIF!]

/* Job error notification routine provided by the upper layer module */
/** @implements   FeeNvmJobErrorNotification_Object */

#define FEE_NVM_JOB_ERROR_NOTIFICATION            [!//
[!IF "$NvmErrFlg = '1.0'"!][!"normalize-space(FeeGeneral/FeeNvmJobErrorNotification)"!]();[!ENDIF!]

/* Job cluster format notification routine provided by the upper layer module (declaration) */
#define FEE_NVM_CLUSTER_FORMAT_NOTIFICATION_DECL  [!//
[!IF "$NvmClusterFormatFlg = '1.0'"!]extern void [!"normalize-space(FeeGeneral/FeeClusterFormatNotification)"!](void);[!ENDIF!]

/* Job cluster format notification routine provided by the upper layer module */
#define FEE_NVM_CLUSTER_FORMAT_NOTIFICATION       [!//
[!IF "$NvmClusterFormatFlg = '1.0'"!][!"normalize-space(FeeGeneral/FeeClusterFormatNotification)"!]();[!ENDIF!]

/* Number of configured Fee cluster groups */
#define FEE_NUMBER_OF_CLUSTER_GROUPS              ([!"num:i(count(FeeClusterGroup/*))"!]U)

/* Number of configured Fee blocks */
#define FEE_CRT_CFG_NR_OF_BLOCKS                  ([!"num:i(count(FeeBlockConfiguration/*))"!]U)

/* Maximum number of Fee blocks in all project versions and configurations */
#define FEE_MAX_NR_OF_BLOCKS                      ([!//
[!IF "FeeGeneral/FeeSwapForeignBlocksEnabled"!][!"node:value(FeeGeneral/FeeMaximumNumberBlocks)"!]U[!ELSE!](FEE_CRT_CFG_NR_OF_BLOCKS)[!ENDIF!])

/* Management overhead per logical block in bytes */
/** @implements   FeeBlockOverhead_Object */
#define FEE_BLOCK_OVERHEAD                        ([!"$FeeBlkOverhead"!]U)

/* Management overhead per logical cluster in bytes */
#define FEE_CLUSTER_OVERHEAD                      ([!"$FeeClsOverhead"!]U)

/* Size of the data buffer in bytes */
#define FEE_DATA_BUFFER_SIZE                      ([!"$FeeDataBufferSize"!]U)

/* The contents of an erased flash memory cell */
#define FEE_ERASED_VALUE                          ([!"num:inttohex(bit:and(as:modconf('Fls')[1]/FlsPublishedInformation/FlsErasedValue,255),2)"!]U)

/* Value of the block and cluster validation flag */
#define FEE_VALIDATED_VALUE                       ([!"num:inttohex(bit:and(bit:xor(as:modconf('Fls')[1]/FlsPublishedInformation/FlsErasedValue,126),255),2)"!]U)

/* Value of the block and cluster invalidation flag */
#define FEE_INVALIDATED_VALUE                     ([!"num:inttohex(bit:and(bit:xor(as:modconf('Fls')[1]/FlsPublishedInformation/FlsErasedValue,231),255),2)"!]U)

/* If reset, power loss etc. occurs here, neither newly nor previously written data is available */
#define FEE_BLOCK_ALWAYS_AVAILABLE                ([!IF "FeeGeneral/FeeBlockAlwaysAvailable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Behavior of Fee_ImmediateBlockErase */
#define FEE_LEGACY_IMM_ERASE_MODE                 ([!IF "FeeGeneral/FeeLegacyEraseMode"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* This configuration defines wheather Fee should swap foreign blocks found in flash at swap or not */
#define FEE_SWAP_FOREIGN_BLOCKS_ENABLED           ([!IF "FeeGeneral/FeeSwapForeignBlocksEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#if (FEE_SWAP_FOREIGN_BLOCKS_ENABLED == STD_ON)
/* This configuration defines for which project the Fee configuration is used  */
#define FEE_BOOTLOADER_CONFIG                     ([!IF "FeeGeneral/FeeConfigAssignment='BOOTLOADER'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
#endif
/* This configuration defines if the status for the never written blocks must be INVALID or INCONSISTENT */
#define FEE_MARK_EMPTY_BLOCKS_INVALID             ([!IF "FeeGeneral/FeeMarkEmptyBlocksInvalid"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Symbolic names of configured Fee blocks */
[!LOOP "node:order(FeeBlockConfiguration/*,'node:value(./FeeBlockNumber)')"!]
#define FeeConf_FeeBlockConfiguration_[!"node:name(.)"!]     ([!"./FeeBlockNumber"!]U)
[!ENDLOOP!]
[!NOCODE!]
[!VAR "FeeVirtualPageSize" = "FeeGeneral/FeeVirtualPageSize"!]
[!IF "$FeeVirtualPageSize < 16"!]
    [!VAR "FeeVirtualPageSize" = "16"!]
[!ENDIF!]
[!IF "(node:value(as:modconf('Fls')[1]/FlsGeneral/FlsCancelApi))"!]
    [!IF "(node:value(as:modconf('Fls')[1]/FlsConfigSet/FlsMaxWriteFastMode )< $FeeVirtualPageSize)"!]
        [!ERROR!]
            Incompatible FLS configuration. If the Fls_Cancel API is enabled in the FLS driver used by the FEE, 
            the FlsMaxWriteFastMode parameter value cannot be smaller than the size of the FEE Block/Cluster Header or the size of the FEE virtual page, whichever is larger.
            Disable Fls_Cancel API, increase the FlsMaxWriteFastMode parameter value in the given FlsConfigSet
        [!ENDERROR!]
    [!ENDIF!]
    [!IF "(node:value(as:modconf('Fls')[1]/FlsConfigSet/FlsMaxWriteNormalMode )< $FeeVirtualPageSize)"!]
        [!ERROR!]
            Incompatible FLS configuration. If the Fls_Cancel API is enabled in the FLS driver used by the FEE,
            the FlsMaxWriteNormalMode parameter value cannot be smaller than the size of the FEE Block/Cluster Header or the size of the FEE virtual page, whichever is larger.
            Disable Fls_Cancel API, increase the FlsMaxWriteNormalMode parameter value in the given FlsConfigSet
        [!ENDERROR!]
    [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!]
#define FEE_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fee_MemMap.h"
[!NOCODE!]
[!VAR "FlsCancelApi" = "node:value(as:modconf('Fls')[1]/FlsGeneral/FlsCancelApi)"!]
[!VAR "FlsBiggestPageSize" = "0"!]
[!LOOP "FeeClusterGroup/*"!]
    [!VAR "FeeSmallestClusterSize" = "2147483647"!]
    [!VAR "FeeLoopIt" = "count(FeeCluster/*)"!]
    [!VAR "FeeReservedSize" = "0"!]
    [!LOOP "FeeCluster/*"!]
        [!VAR "FeeClusterSize" = "0"!]
        [!LOOP "FeeSector/*"!]
            [!IF "$FlsCancelApi"!]
                [!IF "(node:ref(./FeeSectorRef)/FlsPageWriteAsynch)"!]
                    [!ERROR!]
                        Incompatible FLS configuration. If the Fls_Cancel API is enabled in the FLS driver used by the FEE, the Fls Page Write Asynch
                        mode cannot be used for flash sectors. Disable Fls_Cancel API, select different sector, or use synchronous write (Sector: [!"node:ref(./FeeSectorRef)/@name"!]).
                    [!ENDERROR!]
                [!ENDIF!]
            [!ENDIF!]
            [!// Calculate total cluster size
            [!VAR "FeeClusterSize" = "$FeeClusterSize + (num:i(node:ref(./FeeSectorRef)/FlsSectorSize) * num:i(node:ref(./FeeSectorRef)/FlsNumberOfSectors))"!]

            [!// Get the FlsPageSize value of current sector fls.
            [!VAR "FlsCurrentPageSize" = "(num:i(node:ref(./FeeSectorRef)/FlsPageSize))"!]
            [!// Get the biggest FlsPageSize value.
            [!IF "$FlsBiggestPageSize < $FlsCurrentPageSize"!]
                [!VAR "FlsBiggestPageSize" = "$FlsCurrentPageSize"!]
                [!// check if FeeVirtualPageSize is not aligned with FlsPageSize.
                [!IF "num:i(num:mod($FeeVPS,num:i($FlsBiggestPageSize))) != '0'"!]
                    [!ERROR!]
                        The FeeVirtualPageSize must be configured to an integer multiple of FlsPageSize (Sector: [!"node:ref(./FeeSectorRef)/@name"!]).
                    [!ENDERROR!]
                [!ENDIF!]
            [!ENDIF!]
        [!ENDLOOP!]
        [!IF "$FeeClusterSize < $FeeSmallestClusterSize"!]
            [!VAR "FeeSmallestClusterSize" = "$FeeClusterSize"!]
        [!ENDIF!]
    [!ENDLOOP!]
    [!// Calculate space remaining of the cluster after writing overhead and adding padding.
    [!VAR "FeeRemainingClusterSpace" = "$FeeSmallestClusterSize - ($FeeClsOverhead + $FeeBlkOverhead)"!]

    [!VAR "FeeBiggestBlockSize" = "0"!]
    [!VAR "FeeBiggestBlockImmediateData" = "'false'"!]

    [!LOOP "../../FeeBlockConfiguration/*[node:name(node:ref(FeeClusterGroupRef)) = node:current()/@name]"!]
        [!// Calculate space remaining of the cluster after writing all blocks at once.
        [!VAR "FeeRemainingClusterSpace" = "$FeeRemainingClusterSpace - num:i(ceiling(./FeeBlockSize div $FeeVPS) * $FeeVPS) - $FeeBlkOverhead"!]
        [!// Find the biggest block.
        [!IF "$FeeBiggestBlockSize < ./FeeBlockSize"!]
            [!VAR "FeeBiggestBlockSize" = "./FeeBlockSize"!]
            [!VAR "FeeBiggestBlockImmediateData" = "./FeeImmediateData"!]
        [!ENDIF!]
        [!// Calculate reserved space when FeeImmediateData is enable.
        [!IF "./FeeImmediateData"!]
            [!VAR "FeeReservedSize" = "$FeeReservedSize + $FeeBlkOverhead + num:i( ceiling( ./FeeBlockSize div $FeeVPS ) * $FeeVPS )"!]
        [!ENDIF!]
    [!ENDLOOP!]

    [!// Calculate space remaining of the cluster after writing the biggest block one more time
    [!IF "FeeBiggestBlockImmediateData = 'false'"!][!// Except for immediate blocks because they have been calculated in the reserved space
        [!VAR "FeeRemainingClusterSpace" = "$FeeRemainingClusterSpace - num:i(ceiling($FeeBiggestBlockSize div $FeeVPS) * $FeeVPS) - $FeeBlkOverhead"!]
    [!ENDIF!]

    [!// Space remaining of the cluster minus reserved space.
    [!VAR "FeeRemainingClusterSpace" = "$FeeRemainingClusterSpace - $FeeReservedSize"!]
    [!// Raise the error when space remaining of the cluster are less than 0.
    [!IF "($FeeRemainingClusterSpace < 0)"!]
        [!ERROR!]
The total size of blocks is too big. They do not fit into the cluster group [!"node:name(.)"!]. Increase the size of the cluster or reduce the size of blocks.
        [!ENDERROR!]
    [!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!]
/* Configuration of cluster group set */
extern const Fee_ClusterGroupType Fee_ClrGrps[ FEE_NUMBER_OF_CLUSTER_GROUPS ];

/* Configuration of Fee blocks */
extern const Fee_BlockConfigType Fee_BlockConfig[ FEE_CRT_CFG_NR_OF_BLOCKS ];

#define FEE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fee_MemMap.h"

[!IF "FeeGeneral/FeeSectorRetirement"!]
#define FEE_START_SEC_VAR_INIT_UNSPECIFIED
#include "Fee_MemMap.h"

/* List of pointer for information accross all cluster groups */
extern Fee_ClusterRuntimeInfoType *Fee_ClusterRuntimeInfo[ FEE_NUMBER_OF_CLUSTER_GROUPS ];

#define FEE_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Fee_MemMap.h"

[!IF "num:i(count(FeeSectorToRecover/*)) > 0"!]
#define FEE_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fee_MemMap.h"

/* List of sectors to be recovered */
extern const Fee_SectorToRecoverType Fee_SectorToRecover[FEE_NUMBER_OF_SECTORS_TO_RECOVER];

#define FEE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fee_MemMap.h"
[!ENDIF!][!//FeeSectorToRecover
[!ENDIF!][!//FeeSectorRetirement

[!ENDCODE!]
#ifdef __cplusplus
}
#endif

/** @} */

#endif /* FEE_CFG_H */
