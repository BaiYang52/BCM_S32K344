[!AUTOSPACING!]
[!//
[!MACRO "FlsGetBlock4PipeSelect", "FlsEnumPipe"!][!//
    [!IF "$FlsEnumPipe='FLS_COMMAND_PIPE_0'"!][!//
        [!VAR "OutPipeSelect" = "'0x00U'"!][!//
    [!ELSEIF "$FlsEnumPipe='FLS_COMMAND_PIPE_1'"!][!//
        [!VAR "OutPipeSelect" = "'0x01U'"!][!//
    [!ELSEIF "$FlsEnumPipe='FLS_COMMAND_PIPE_2'"!][!//
        [!VAR "OutPipeSelect" = "'0x02U'"!][!//
    [!ELSEIF "$FlsEnumPipe='FLS_COMMAND_PIPE_3'"!][!//
        [!VAR "OutPipeSelect" = "'0x03U'"!][!//
    [!ELSE!][!//
        [!VAR "OutPipeSelect" = "'0x04U'"!][!//
    [!ENDIF!][!//
    [!CODE!][!"$OutPipeSelect"!][!ENDCODE!][!//
[!ENDMACRO!][!//
[!//
[!CODE!][!//
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

#ifndef C40_IP_CFG_H
#define C40_IP_CFG_H

/**
*   @file C40_Ip_Cfg.h
*
*   @addtogroup FLS
*   @{
*/

/* implements C40_Ip_Cfg.h_Artifact */

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "OsIf.h"
#include "C40_Ip_Types.h"
#include "[!"ecu:get('Fls.Internal.C40.Header')"!]_PFLASH.h"
#include "[!"ecu:get('Fls.Internal.C40.Header')"!]_FLASH.h"


[!IF "FlsGeneral/FlsEnableDevAssert"!]
#include "Devassert.h"
[!ENDIF!][!//

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLS_C40_IP_VENDOR_ID_CFG                          43
#define FLS_C40_IP_AR_RELEASE_MAJOR_VERSION_CFG           4
#define FLS_C40_IP_AR_RELEASE_MINOR_VERSION_CFG           4
#define FLS_C40_IP_AR_RELEASE_REVISION_VERSION_CFG        0
#define FLS_C40_IP_SW_MAJOR_VERSION_CFG                   2
#define FLS_C40_IP_SW_MINOR_VERSION_CFG                   0
#define FLS_C40_IP_SW_PATCH_VERSION_CFG                   1


/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and OsIf.h header file are of the same Autosar version */
    #if ((FLS_C40_IP_AR_RELEASE_MAJOR_VERSION_CFG != OSIF_AR_RELEASE_MAJOR_VERSION) || \
         (FLS_C40_IP_AR_RELEASE_MINOR_VERSION_CFG != OSIF_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AutoSar Version Numbers of C40_Ip_Cfg.h and OsIf.h are different"
    #endif
[!IF "FlsGeneral/FlsEnableDevAssert"!][!//
    /* Check if current file and Devassert.h header file are of the same Autosar version */
    #if ((FLS_C40_IP_AR_RELEASE_MAJOR_VERSION_CFG != DEVASSERT_AR_RELEASE_MAJOR_VERSION) || \
         (FLS_C40_IP_AR_RELEASE_MINOR_VERSION_CFG != DEVASSERT_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AutoSar Version Numbers of C40_Ip_Cfg.h and Devassert.h are different"
    #endif
[!ENDIF!][!//
#endif

/* Check if current file and C40_Ip_Types header file are of the same vendor */
#if (FLS_C40_IP_VENDOR_ID_CFG != C40_IP_TYPES_VENDOR_ID_CFG)
    #error "C40_Ip_Cfg.h and C40_Ip_Types.h have different vendor ids"
#endif
/* Check if current file and C40_Ip_Types header file are of the same Autosar version */
#if ((FLS_C40_IP_AR_RELEASE_MAJOR_VERSION_CFG    != C40_IP_TYPES_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (FLS_C40_IP_AR_RELEASE_MINOR_VERSION_CFG    != C40_IP_TYPES_AR_RELEASE_MINOR_VERSION_CFG) || \
     (FLS_C40_IP_AR_RELEASE_REVISION_VERSION_CFG != C40_IP_TYPES_AR_RELEASE_REVISION_VERSION_CFG) \
    )
    #error "AutoSar Version Numbers of C40_Ip_Cfg.h and C40_Ip_Types.h are different"
#endif
/* Check if current file and C40_Ip_Types header file are of the same Software version */
#if ((FLS_C40_IP_SW_MAJOR_VERSION_CFG != C40_IP_TYPES_SW_MAJOR_VERSION_CFG) || \
     (FLS_C40_IP_SW_MINOR_VERSION_CFG != C40_IP_TYPES_SW_MINOR_VERSION_CFG) || \
     (FLS_C40_IP_SW_PATCH_VERSION_CFG != C40_IP_TYPES_SW_PATCH_VERSION_CFG) \
    )
    #error "Software Version Numbers of C40_Ip_Cfg.h and C40_Ip_Types.h are different"
#endif


/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/
[!NOCODE!][!//
[!VAR "c40_ip_syncronize_cache" = "'STD_OFF'"!][!//
[!IF "ecu:has('Fls.SupportSynchronizeCacheFeature')"!][!//
    [!IF "ecu:get('Fls.SupportSynchronizeCacheFeature') = 'true'"!][!//
        [!IF "AutosarExt/FlsSynchronizeCache"!][!//
            [!VAR "c40_ip_syncronize_cache" = "'STD_ON'"!][!//
        [!ENDIF!] [!//
    [!ENDIF!] [!//
[!ENDIF!] [!//
[!ENDNOCODE!][!//

/**
 * @brief  PFlash register base address of Pflash
 */
#define PFLASH_BASEADDR                             IP_PFLASH_BASE

#define FLS_MAX_DATA_SECTOR                         ([!"(ecu:get('Fls.NumberOfSectorData'))"!]U)

#define FLS_NO_OF_SECTOR_PER_BLOCK                  ([!"(ecu:get('Fls.NoSectorPerBlock'))"!]U)

#define FLS_NO_OF_SUPPER_SECTOR_PER_BLOCK           ([!"(ecu:get('Fls.NoSuperSectorPerBlock'))"!]U)

#define FLS_DATA_PFCBLK_ORDER                       (PFLASH_PFCBLKI_SPELOCK_COUNT - 1U)


/* Order of data flash PFCBLK Registers Arrays */
#define PFLASH_PFCBLK_DATA_LOCKMASTER_S0_ADDR32     ((uint32)&(IP_PFLASH->PFCBLK_LOCKMASTER_S[PFLASH_PFCBLKI_LOCKMASTER_S_COUNT - 1U][0U]))
/**
 * @brief  Block a Lock Master Sectorb
 */
#define PFLASH_PFCBLK0_LOCKMASTER_S0_ADDR32         ((uint32)&(IP_PFLASH->PFCBLK_LOCKMASTER_S[0U][0U]))
/**
 * @brief   Block UTEST Lock Master Sector
 */
#define PFLASH_PFCBLKU_LOCKMASTER_S_ADDR32          ((uint32)&(IP_PFLASH->PFCBLKU_LOCKMASTER_S[0U]))
/**
 * @brief   Block a Lock Master Super Sectorb
 */
#define PFLASH_PFCBLK0_LOCKMASTER_SS0_ADDR32        ((uint32)(PFLASH_BASEADDR + (uint32)0x484UL))

/* Mask of failed  address regions: A0-A5 */
#define FLASH_ADDRESS_REGION_0              (FLASH_ADR_A0_MASK)
#define FLASH_ADDRESS_REGION_1              (FLASH_ADR_A1_MASK)
#define FLASH_ADDRESS_REGION_2              (FLASH_ADR_A2_MASK)
#define FLASH_ADDRESS_REGION_3              ([!IF "contains(ecu:list('Fls.Sector.Physical.List'), 'FLS_CODE_ARRAY_0_BLOCK_2')"!]FLASH_ADR_A3_MASK[!ELSE!]0U[!ENDIF!])
#define FLASH_ADDRESS_REGION_4              ([!IF "contains(ecu:list('Fls.Sector.Physical.List'), 'FLS_CODE_ARRAY_0_BLOCK_3')"!]FLASH_ADR_A4_MASK[!ELSE!]0U[!ENDIF!])
#define FLASH_ADDRESS_REGION_5              (FLASH_ADR_A5_MASK)

#define FLASH_FAILED_ADDRESS_REGION_MASK    (FLASH_ADDRESS_REGION_0  | \
                                             FLASH_ADDRESS_REGION_1  | \
                                             FLASH_ADDRESS_REGION_2  | \
                                             FLASH_ADDRESS_REGION_3  | \
                                             FLASH_ADDRESS_REGION_4  | \
                                             FLASH_ADDRESS_REGION_5)


/* Enable the use of cache invalidate */
#define C40_IP_SYNCRONIZE_CACHE             ([!"$c40_ip_syncronize_cache"!])

#define C40_MAIN_INTERFACE_ENABLED          ([!IF "AutosarExt/FlsUsesAlterInterface"!]STD_OFF[!ELSE!]STD_ON[!ENDIF!])


#define C40_IP_ENABLE_USER_MODE_SUPPORT     ([!IF "AutosarExt/FlsEnableUserModeSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    #if (STD_ON == C40_IP_ENABLE_USER_MODE_SUPPORT)
        #error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running Fls in user mode the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined
    #endif
#endif

#define C40_TIMEOUT_SUPERVISION_ENABLED     ([!IF "FlsGeneral/FlsTimeoutSupervisionEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#define C40_ERASE_VERIFICATION_ENABLED      ([!IF "FlsGeneral/FlsEraseVerificationEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#define C40_PROGRAM_VERIFICATION_ENABLED    ([!IF "FlsGeneral/FlsWriteVerificationEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#define C40_ERASED_VALUE_32                 ([!"num:inttohex(FlsPublishedInformation/FlsErasedValue)"!]U)

#define C40_ERASED_VALUE_16                 (C40_ERASED_VALUE_32 & 0xFFFFU)

#define C40_ERASED_VALUE_8                  (C40_ERASED_VALUE_32 & 0xFFU)

#define C40_DATA_FLASH_ERROR_SUPPRESSION    ([!IF "AutosarExt/FlsDataErrorSuppression"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#define FLS_BLOCK_4_PIPE_SELECTABLE         ([!"ecu:get('Fls.SupportBlock4PipeSelect')"!])

#if (STD_ON == FLS_BLOCK_4_PIPE_SELECTABLE)
#define C40_BLOCK4_PIPE_SELECT              ([!CALL "FlsGetBlock4PipeSelect", "FlsEnumPipe" = "AutosarExt/FlsBlock4PipeSelect"!])
#endif

#define C40_ECC_CHECK                       ([!IF "FlsGeneral/FlsECCCheck"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#define C40_ECC_CHECK_BY_AUTOSAR_OS         ([!IF "FlsGeneral/FlsECCHandlingProtectionHook"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#if ( (C40_ECC_CHECK == STD_ON) || (C40_ECC_CHECK_BY_AUTOSAR_OS == STD_ON) )
    #define C40_ECC_VALUE                   ([!"num:inttohex(FlsPublishedInformation/FlsECCValue)"!]U)
#endif

/*! Enable development error check */
#define C40_IP_DEV_ASSERT(x)                [!WS "16"!][!IF "FlsGeneral/FlsEnableDevAssert"!]DevAssert((x))[!ELSE!](void)(x)[!ENDIF!][!CR!]

#define C40_TIMEOUT_TYPE                    ([!"FlsGeneral/FlsTimeoutMethod"!])

#if (STD_ON == C40_TIMEOUT_SUPERVISION_ENABLED)

#define C40_ASYNC_WRITE_TIMEOUT             ([!"FlsGeneral/FlsAsyncWriteTimeout"!]U)

#define C40_ASYNC_ERASE_TIMEOUT             ([!"FlsGeneral/FlsAsyncEraseTimeout"!]U)

#define C40_SYNC_WRITE_TIMEOUT              ([!"FlsGeneral/FlsSyncWriteTimeout"!]U)

#define C40_SYNC_ERASE_TIMEOUT              ([!"FlsGeneral/FlsSyncEraseTimeout"!]U)

#define C40_ABORT_TIMEOUT                   ([!"FlsGeneral/FlsAbortTimeout"!]U)

#endif  /*(STD_ON == C40_TIMEOUT_SUPERVISION_ENABLED)*/

#define FLS_HAS_CODE_ARRAY_0_BLOCK_2        ([!IF "contains(ecu:list('Fls.Sector.Physical.List'), 'FLS_CODE_ARRAY_0_BLOCK_2')"!]1[!ELSE!]0[!ENDIF!]U)
#define FLS_HAS_CODE_ARRAY_0_BLOCK_3        ([!IF "contains(ecu:list('Fls.Sector.Physical.List'), 'FLS_CODE_ARRAY_0_BLOCK_3')"!]1[!ELSE!]0[!ENDIF!]U)

#define FLS_HAS_CODE_ARRAY_0_BLOCK_2_AND_3  (FLS_HAS_CODE_ARRAY_0_BLOCK_2 & FLS_HAS_CODE_ARRAY_0_BLOCK_3)

/* Code block 0 addresses */
#define FLS_CODE_BLOCK_0_BASE_ADDR          (0x00400000U)  /*!< the base address of code block 0 */
#define FLS_CODE_BLOCK_0_END_ADDR           (0x004FFFFFU)  /*!< the end address of code block 0 */

/* Code block 1 addresses */
#define FLS_CODE_BLOCK_1_BASE_ADDR          (0x00500000U)  /*!< the base address of code block 1 */
#define FLS_CODE_BLOCK_1_END_ADDR           (0x005FFFFFU)  /*!< the end address of code block 1 */

#if (FLS_HAS_CODE_ARRAY_0_BLOCK_2 == 1U)
/* Code block 2 addresses */
#define FLS_CODE_BLOCK_2_BASE_ADDR          (0x00600000U)  /*!< the base address of code block 2 */
#define FLS_CODE_BLOCK_2_END_ADDR           (0x006FFFFFU)  /*!< the end address of code block 2 */
#endif

#if (FLS_HAS_CODE_ARRAY_0_BLOCK_3 == 1U)
/* Code block 3 addresses */
#define FLS_CODE_BLOCK_3_BASE_ADDR          (0x00700000U)  /*!< the base address of code block 3 */
#define FLS_CODE_BLOCK_3_END_ADDR           (0x007FFFFFU)  /*!< the end address of code block 3 */
#endif

/* Data block addresses */
#define FLS_DATA_BLOCK_BASE_ADDR            (0x10000000U)  /*!< the base address of data block */
[!VAR "fls_data_block_end_addr" = "num:hextoint(ecu:get('Fls.DFlashBaseAddr')) + num:hextoint(ecu:get('Fls.DFlashsize')) - 1"!][!//
[!VAR "fls_data_block_end_addr" = "text:toupper(text:replace(num:inttohex($fls_data_block_end_addr, 8), '0x', ''))"!][!//
#define FLS_DATA_BLOCK_END_ADDR             (0x[!"$fls_data_block_end_addr"!]U)  /*!< the end address of data block   */

/* Utest block addresses */
#define FLS_UTEST_BLOCK_BASE_ADDR           (0x1B000000U)  /*!< the base address of Utest block */
#define FLS_UTEST_BLOCK_END_ADDR            (0x1B001FFFU)  /*!< the end address of Utest block */

/* Memory flash sector characteristics */
#define FLS_MAX_VIRTUAL_SECTOR              ([!"num:i(count(ecu:list('Fls.Sector.Physical.List')) - 2)"!]U)

[!VAR "fls_code_block_end_addr" = "'FLS_CODE_BLOCK_1_END_ADDR'"!][!//
[!IF "contains(ecu:list('Fls.Sector.Physical.List'), 'FLS_CODE_ARRAY_0_BLOCK_3')"!][!//
    [!VAR "fls_code_block_end_addr" = "'FLS_CODE_BLOCK_3_END_ADDR'"!][!//
[!ELSEIF "contains(ecu:list('Fls.Sector.Physical.List'), 'FLS_CODE_ARRAY_0_BLOCK_2')"!][!//
    [!VAR "fls_code_block_end_addr" = "'FLS_CODE_BLOCK_2_END_ADDR'"!][!//
[!ENDIF!][!//
#define FLS_CODE_BLOCK_END_ADDR              [!"$fls_code_block_end_addr"!]

/*==================================================================================================
                                 ENUM TYPEDEFS
==================================================================================================*/

typedef uint32 C40_Ip_VirtualSectorsType;

/**
    For flash area : Code flash : Start 0040_0000h -> 0x007F_E000h and Data Flash: 1000_0000h -> 1003_FFFFh and Utest Flash :  1B00_0000h -> 1B00_1FFFh
*/
[!FOR "SectorIt" = "1" TO "num:i(count(ecu:list('Fls.Sector.Physical.List'))) - 1"!][!//
[!VAR "PhysicalSector" = "ecu:list('Fls.Sector.Physical.List')[num:i($SectorIt)]"!][!//
[!VAR "PhysicalSector" = "substring-after($PhysicalSector, '_')"!][!//
[!VAR "PhysicalSector" = "text:replace($PhysicalSector, 'FLS', 'C40')"!][!//
[!IF "contains($PhysicalSector, 'DATA') = 'true'"!][!//
    [!VAR "PhysicalAddr" = "num:hextoint(ecu:get('Fls.DFlashBaseAddr')) + (num:hextoint(ecu:get('Fls.DFlashSectorsize')) * ($SectorIt - 1))"!][!//
[!ELSEIF "contains($PhysicalSector, 'CODE') = 'true'"!][!//
    [!VAR "PhysicalAddr" = "num:hextoint(ecu:get('Fls.PFlashBaseAddr')) + (num:hextoint(ecu:get('Fls.PFlashSectorsize')) * ($SectorIt - 1 - num:i(ecu:get('Fls.NumberOfSectorData'))))"!][!//
[!ELSE!][!//
    [!VAR "PhysicalAddr" = "num:hextoint('0x1B000000')"!][!//
    [!VAR "PhysicalSector" = "concat($PhysicalSector, '       ')"!][!//
[!ENDIF!][!//
#define [!"$PhysicalSector"!]       ([!"num:i($SectorIt - 1)"!]U)    /* 0x[!"text:toupper(text:replace(num:inttohex($PhysicalAddr, 8), '0x', ''))"!] */
[!ENDFOR!]
#define C40_SECTOR_ERROR                    ([!"num:i($SectorIt)"!]U)    /* default and error return value */


/*==================================================================================================
                                 GLOBAL CONSTANT DECLARATIONS
==================================================================================================*/
#define FLS_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fls_MemMap.h"

[!INDENT "0"!][!//
[!NOCODE!][!//
[!VAR "configName" = "as:name(FlsConfigSet)"!][!//
[!IF "var:defined('postBuildVariant')"!][!//
    [!LOOP "variant:all()"!][!//
        [!CODE!]extern const C40_ConfigType [!"$configName"!]_[!"."!]_InitCfg;[!CR!][!ENDCODE!]
    [!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!]extern const C40_ConfigType [!"$configName"!]_InitCfg;[!CR!][!ENDCODE!]
[!ENDIF!][!//
[!ENDNOCODE!][!//

#define FLS_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fls_MemMap.h"


[!ENDINDENT!][!//
[!ENDCODE!][!//
#ifdef __cplusplus
}
#endif

/** @} */

#endif /* C40_IP_CFG_H */
