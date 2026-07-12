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

#ifndef FEE_FEATURES_H
#define FEE_FEATURES_H

/**
*   @file
*
*   @addtogroup FEE
*   @{
*/
/*   @implements Fee_Features.h_Artifact*/
#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "StandardTypes.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define FEE_FEATURES_VENDOR_ID                    43
#define FEE_FEATURES_AR_RELEASE_MAJOR_VERSION     4
#define FEE_FEATURES_AR_RELEASE_MINOR_VERSION     4
#define FEE_FEATURES_AR_RELEASE_REVISION_VERSION  0
#define FEE_FEATURES_SW_MAJOR_VERSION             2
#define FEE_FEATURES_SW_MINOR_VERSION             0
#define FEE_FEATURES_SW_PATCH_VERSION             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if Fee_Feature.h header file and StandardTypes.h header file are of the same Autosar version */
    #if ((FEE_FEATURES_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
         (FEE_FEATURES_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION) \
        )
        #error "Autosar Version Numbers of Fee_Feature.h and StandardTypes.h are different"
    #endif
#endif
/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/
[!VAR "FeeMaxSectorIndex"  = "num:max(FeeClusterGroup/*/FeeCluster/*/FeeSector/*/FeeSectorIndex)"!][!//
[!VAR "FeeSectorsInfoSize" = "num:i( ceiling(($FeeMaxSectorIndex + 1) div 8) )"!][!// Use 1 bit per sector
[!IF "bit:and($FeeSectorsInfoSize, 1) != 0"!]
    [!VAR "FeeSectorsInfoSize" = "$FeeSectorsInfoSize + 1"!][!// Round up to even number
[!ENDIF!]
[!VAR "FeeSectorsInfoSize" = "$FeeSectorsInfoSize + 2"!][!// Use 2 bytes for the number of sectors
[!// Round to the biggest page size
[!VAR "FlsBiggestPageSize" = "num:max( node:refs(FeeClusterGroup/*/FeeCluster/*/FeeSector/*/FeeSectorRef)/FlsPageSize )"!]
[!VAR "FeeSectorsInfoSize" = "num:i(ceiling($FeeSectorsInfoSize div $FlsBiggestPageSize) * $FlsBiggestPageSize)"!]

/* This configuration enables the sector retirement feature */
#define FEE_SECTOR_RETIREMENT                     ([!IF "FeeGeneral/FeeSectorRetirement"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#if (FEE_SECTOR_RETIREMENT == STD_ON)
    /* The number of bytes in a cluster header, used to store the sectors information */
    #define FEE_CLUSTER_HEADER_SECTORINFO_SIZE    ([!"num:i($FeeSectorsInfoSize)"!]U)

    /* The number of attempts when erasing each sector in the cluster in swap phase */
    #define FEE_SECTOR_ERASE_RETRIES              ([!"num:i(FeeGeneral/FeeSectorEraseRetries)"!]U)

    /* The number of attempts when erasing each sector in the cluster in swap phase */
    #define FEE_NUMBER_OF_SECTORS_TO_RECOVER      ([!"num:i(count(FeeSectorToRecover/*))"!]U)
#endif

[!ENDCODE!]
#ifdef __cplusplus
}
#endif

/** @} */

#endif /* FEE_FEATURES_H */
