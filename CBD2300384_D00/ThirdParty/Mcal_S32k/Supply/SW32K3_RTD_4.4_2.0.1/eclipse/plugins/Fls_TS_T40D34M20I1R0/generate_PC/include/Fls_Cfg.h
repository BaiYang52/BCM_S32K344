[!AUTOSPACING!]
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

#ifndef FLS_CFG_H
#define FLS_CFG_H

/**
*   @file Fls_Cfg.h
*
*   @addtogroup FLS
*   @{
*/

/* implements Fls_Cfg.h_Artifact */

#ifdef __cplusplus
extern "C"{
#endif


[!INDENT "0"!][!//
/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Fls_Types.h"
#include "Mcal.h"
[!ENDINDENT!][!//
#include "MemIf_Types.h"
#include "C40_Ip_Cfg.h"
[!IF "FlsGeneral/FlsAcLoadOnJobStart != 'true'"!]#include "C40_Ip_Ac.h"[!CR!][!ENDIF!][!//

#include "Qspi_Ip_Cfg.h"

[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
#include "Fls_[!"."!]_PBcfg.h"
[!ENDLOOP!]
[!ELSE!]
#include "Fls_PBcfg.h"
[!ENDIF!]

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLS_VENDOR_ID_CFG                    43
#define FLS_AR_RELEASE_MAJOR_VERSION_CFG     4
#define FLS_AR_RELEASE_MINOR_VERSION_CFG     4
#define FLS_AR_RELEASE_REVISION_VERSION_CFG  0
#define FLS_SW_MAJOR_VERSION_CFG             2
#define FLS_SW_MINOR_VERSION_CFG             0
#define FLS_SW_PATCH_VERSION_CFG             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and Fls configuration header file are of the same vendor */
#if (FLS_TYPES_VENDOR_ID != FLS_VENDOR_ID_CFG)
    #error "Fls_Types.h and Fls_Cfg.h have different vendor ids"
#endif
/* Check if current file and Fls configuration header file are of the same Autosar version */
#if ((FLS_TYPES_AR_RELEASE_MAJOR_VERSION    != FLS_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (FLS_TYPES_AR_RELEASE_MINOR_VERSION    != FLS_AR_RELEASE_MINOR_VERSION_CFG) || \
     (FLS_TYPES_AR_RELEASE_REVISION_VERSION != FLS_AR_RELEASE_REVISION_VERSION_CFG) \
    )
    #error "AutoSar Version Numbers of Fls_Types.h and Fls_Cfg.h are different"
#endif
/* Check if current file and Fls configuration header file are of the same software version */
#if ((FLS_TYPES_SW_MAJOR_VERSION != FLS_SW_MAJOR_VERSION_CFG) || \
     (FLS_TYPES_SW_MINOR_VERSION != FLS_SW_MINOR_VERSION_CFG) || \
     (FLS_TYPES_SW_PATCH_VERSION != FLS_SW_PATCH_VERSION_CFG) \
    )
    #error "Software Version Numbers of Fls_Types.h and Fls_Cfg.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and Std_Types header file are of the same version */
    #if ((FLS_AR_RELEASE_MAJOR_VERSION_CFG != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (FLS_AR_RELEASE_MINOR_VERSION_CFG != MCAL_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AutoSar Version Numbers of Fls_Types.h and Mcal.h are different"
    #endif
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and MemIf_Types.h file are of the same Autosar version */
    #if ((FLS_AR_RELEASE_MAJOR_VERSION_CFG != MEMIF_AR_RELEASE_MAJOR_VERSION) || \
         (FLS_AR_RELEASE_MINOR_VERSION_CFG != MEMIF_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AutoSar Version Numbers of Fls_Cfg.h and MemIf_Types.h are different"
    #endif
#endif

/* Check if current file and C40_Ip_Cfg header file are of the same vendor */
#if (FLS_VENDOR_ID_CFG != FLS_C40_IP_VENDOR_ID_CFG)
    #error "Fls_Cfg.h and C40_Ip_Cfg.h have different vendor ids"
#endif
/* Check if current file and C40_Ip_Cfg header file are of the same Autosar version */
#if ((FLS_AR_RELEASE_MAJOR_VERSION_CFG    != FLS_C40_IP_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (FLS_AR_RELEASE_MINOR_VERSION_CFG    != FLS_C40_IP_AR_RELEASE_MINOR_VERSION_CFG) || \
     (FLS_AR_RELEASE_REVISION_VERSION_CFG != FLS_C40_IP_AR_RELEASE_REVISION_VERSION_CFG) \
    )
    #error "AutoSar Version Numbers of Fls_Cfg.h and C40_Ip_Cfg.h are different"
#endif
/* Check if current file and C40_Ip_Cfg header file are of the same Software version */
#if ((FLS_SW_MAJOR_VERSION_CFG != FLS_C40_IP_SW_MAJOR_VERSION_CFG) || \
     (FLS_SW_MINOR_VERSION_CFG != FLS_C40_IP_SW_MINOR_VERSION_CFG) || \
     (FLS_SW_PATCH_VERSION_CFG != FLS_C40_IP_SW_PATCH_VERSION_CFG) \
    )
    #error "Software Version Numbers of Fls_Cfg.h and C40_Ip_Cfg.h are different"
#endif


/* Check if current file and Qspi_Ip_Cfg header file are of the same vendor */
#if (FLS_VENDOR_ID_CFG != QSPI_IP_VENDOR_ID_CFG)
    #error "Fls_Cfg.h and Qspi_Ip_Cfg.h have different vendor ids"
#endif
/* Check if current file and Qspi_Ip_Cfg header file are of the same Autosar version */
#if ((FLS_AR_RELEASE_MAJOR_VERSION_CFG    != QSPI_IP_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (FLS_AR_RELEASE_MINOR_VERSION_CFG    != QSPI_IP_AR_RELEASE_MINOR_VERSION_CFG) || \
     (FLS_AR_RELEASE_REVISION_VERSION_CFG != QSPI_IP_AR_RELEASE_REVISION_VERSION_CFG) \
    )
#error "AutoSar Version Numbers of Fls_Cfg.h and Qspi_Ip_Cfg.h are different"
#endif
/* Check if current file and Qspi_Ip_Cfg header file are of the same software version */
#if ((FLS_SW_MAJOR_VERSION_CFG != QSPI_IP_SW_MAJOR_VERSION_CFG) || \
     (FLS_SW_MINOR_VERSION_CFG != QSPI_IP_SW_MINOR_VERSION_CFG) || \
     (FLS_SW_PATCH_VERSION_CFG != QSPI_IP_SW_PATCH_VERSION_CFG) \
    )
    #error "Software Version Numbers of Fls_Cfg.h and Qspi_Ip_Cfg.h are different"
#endif

[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
/* Check if current file and Fls_[!"."!]_PBcfg header file are of the same vendor */
#if (FLS_VENDOR_ID_CFG != FLS_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID)
    #error "Fls_Cfg.h and Fls_[!"."!]_PBcfg.h have different vendor ids"
#endif
/* Check if current file and Fls_[!"."!]_PBcfg header file are of the same Autosar version */
#if ((FLS_AR_RELEASE_MAJOR_VERSION_CFG    != FLS_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (FLS_AR_RELEASE_MINOR_VERSION_CFG    != FLS_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (FLS_AR_RELEASE_REVISION_VERSION_CFG != FLS_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
#error "AutoSar Version Numbers of Fls_Cfg.h and Fls_[!"."!]_PBcfg.h are different"
#endif
/* Check if current file and Fls_[!"."!]_PBcfg header file are of the same software version */
#if ((FLS_SW_MAJOR_VERSION_CFG != FLS_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION) || \
     (FLS_SW_MINOR_VERSION_CFG != FLS_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION) || \
     (FLS_SW_PATCH_VERSION_CFG != FLS_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Fls_Cfg.h and Fls_[!"."!]_PBcfg.h are different"
#endif
[!ENDLOOP!]
[!ELSE!]
/* Check if current file and Fls_PBcfg header file are of the same vendor */
#if (FLS_VENDOR_ID_CFG != FLS_PBCFG_VENDOR_ID)
    #error "Fls_Cfg.h and Fls_PBcfg.h have different vendor ids"
#endif
/* Check if current file and Fls_PBcfg header file are of the same Autosar version */
#if ((FLS_AR_RELEASE_MAJOR_VERSION_CFG    != FLS_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (FLS_AR_RELEASE_MINOR_VERSION_CFG    != FLS_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (FLS_AR_RELEASE_REVISION_VERSION_CFG != FLS_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
#error "AutoSar Version Numbers of Fls_Cfg.h and Fls_PBcfg.h are different"
#endif
/* Check if current file and Fls_PBcfg header file are of the same software version */
#if ((FLS_SW_MAJOR_VERSION_CFG != FLS_PBCFG_SW_MAJOR_VERSION) || \
     (FLS_SW_MINOR_VERSION_CFG != FLS_PBCFG_SW_MINOR_VERSION) || \
     (FLS_SW_PATCH_VERSION_CFG != FLS_PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Fls_Cfg.h and Fls_PBcfg.h are different"
#endif
[!ENDIF!]

[!INCLUDE "Fls_VersionCheck.m"!][!//

[!MACRO "FlsExportDriverConfiguration"!][!//
[!NOCODE!][!//
[!VAR "configName" = "as:name(FlsConfigSet)"!][!//
[!IF "var:defined('postBuildVariant')"!][!//
    [!VAR "variantIndex"="0"!][!//
    [!LOOP "variant:all()"!][!VAR "variantIndex"="$variantIndex + 1"!][!//
    [!//
        [!CODE!][!WS4!][!CR!][!ENDCODE!]
        [!CODE!][!WS4!]extern const Fls_ConfigType [!"$configName"!]_[!"."!];[!CR!][!ENDCODE!]
    [!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS4!][!CR!][!ENDCODE!]
    [!CODE!][!WS4!]extern const Fls_ConfigType [!"$configName"!];[!CR!][!ENDCODE!]
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!]
/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/

#define FLS_CONFIG_EXT \
[!IF "var:defined('postBuildVariant')"!][!//
[!VAR "variantNumber"="variant:size()"!][!//
[!FOR "variantIndex" = "1" TO "$variantNumber - 1"!]
    FLS_CONFIG_[!"variant:all()[number($variantIndex)]"!]_PB \
[!ENDFOR!]
    FLS_CONFIG_[!"variant:all()[number($variantNumber)]"!]_PB
[!ELSE!][!//
    FLS_CONFIG_PB
[!ENDIF!]

/* STD_ON: Flash access code loaded on job start / unloaded on job end or error */
#define FLS_AC_LOAD_ON_JOB_START             ([!IF "FlsGeneral/FlsAcLoadOnJobStart"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#if (FLS_AC_LOAD_ON_JOB_START == STD_ON)
    /* Cleaning cache after loading access code to RAM */
    #define FLS_CLEAN_CACHE_AFTER_LOAD_AC    ([!IF "FlsGeneral/FlsCleanCacheAfterLoadAc"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
#endif

/* Enable calculates CRC for items of Fls Configuration */
#define FLS_CHECK_CFG_CRC                    ([!IF "FlsGeneral/FlsEnableCheckCfgCrc"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Compile switch to enable and disable the Fls_Cancel function */
#define FLS_CANCEL_API                       ([!IF "FlsGeneral/FlsCancelApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Compile switch to enable and disable the Fls_Compare function */
#define FLS_COMPARE_API                      ([!IF "FlsGeneral/FlsCompareApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Compile switch to enable and disable the Fls_BlankCheck function */
#define FLS_BLANK_CHECK_API                  ([!IF "FlsGeneral/FlsBlankCheckApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Pre-processor switch to enable and disable development error detection */
#define FLS_DEV_ERROR_DETECT                 ([!IF "FlsGeneral/FlsDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Compile switch to enable and disable the Fls_GetJobResult function */
#define FLS_GET_JOB_RESULT_API               ([!IF "FlsGeneral/FlsGetJobResultApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Compile switch to enable and disable the Fls_GetStatus function */
#define FLS_GET_STATUS_API                   ([!IF "FlsGeneral/FlsGetStatusApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Compile switch to enable and disable the Fls_SetMode function */
#define FLS_SET_MODE_API                     ([!IF "FlsGeneral/FlsSetModeApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Pre-processor switch to enable / disable the API to read out the modules version information */
#define FLS_VERSION_INFO_API                 ([!IF "FlsGeneral/FlsVersionInfoApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Pre-processor switch to enable / disable the API to report data storage (ECC) errors to the flash driver */
#define FLS_ECC_CHECK                        ([!IF "FlsGeneral/FlsECCCheck"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
#define FLS_ECC_CHECK_BY_AUTOSAR_OS          ([!IF "FlsGeneral/FlsECCHandlingProtectionHook"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Pre-processor switch to enable / disable the erase blank check */
#define FLS_ERASE_VERIFICATION_ENABLED       ([!IF "FlsGeneral/FlsEraseVerificationEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Pre-processor switch to enable / disable the write verify check */
#define FLS_WRITE_VERIFICATION_ENABLED       ([!IF "FlsGeneral/FlsWriteVerificationEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#if (FLS_ERASE_VERIFICATION_ENABLED == STD_ON)
    /* The maximum number of bytes to blank check in one cycle of the flash driver job processing function */
    #define FLS_MAX_ERASE_BLANK_CHECK        ([!"FlsGeneral/FlsMaxEraseBlankCheck"!]U)
#endif

/* Timeout handling enabled */
#define FLS_TIMEOUT_SUPERVISION_ENABLED      ([!IF "FlsGeneral/FlsTimeoutSupervisionEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!INDENT "0"!][!//
[!IF "FlsGeneral/FlsTimeoutSupervisionEnabled"!]

/* Timeout value for Erase and Write operation - QSPI flash operations. */
#define FLS_QSPI_SYNC_WRITE_TIMEOUT          ([!"FlsGeneral/FlsQspiSyncWriteTimeout"!]U)
#define FLS_QSPI_SYNC_ERASE_TIMEOUT          ([!"FlsGeneral/FlsQspiSyncEraseTimeout"!]U)
#define FLS_QSPI_ASYNC_WRITE_TIMEOUT         ([!"FlsGeneral/FlsQspiAsyncWriteTimeout"!]U)
#define FLS_QSPI_ASYNC_ERASE_TIMEOUT         ([!"FlsGeneral/FlsQspiAsyncEraseTimeout"!]U)

[!IF "AutosarExt/FlsMCoreEnable = 'true'"!]
/* OsIf counter type used in timeout detection for FLS operations */
#define FLS_TIMEOUT_TYPE                     ([!"FlsGeneral/FlsTimeoutMethod"!])

/* Multicore operations timeout */
#define FLS_MCORE_ARBITRATION_TIMEOUT        ([!"FlsGeneral/FlsMCoreArbitrationTimeout"!]U)
#define FLS_MCORE_INIT_TIMEOUT               ([!"FlsGeneral/FlsMCoreInitTimeout"!]U)
[!ENDIF!]
[!ENDIF!]
[!ENDINDENT!][!//

/* Enable multicore core synchronization feature. */
#define FLS_MCORE_ENABLED                    ([!IF "AutosarExt/FlsMCoreEnable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

[!IF "AutosarExt/FlsMCoreEnable = 'true'"!]    
/* SEMA4 used for internal memory operations. */
#define FLS_MCORE_SEMA4_JOB                  ([!"num:inttohex(AutosarExt/FlsMCoreJobSemaphoreChannelNo)"!]UL)
/* SEMA4 used for external memory operations. */
#define FLS_MCORE_SEMA4_QJOB                 ([!"num:inttohex(AutosarExt/FlsMCoreQJobSemaphoreChannelNo)"!]UL)
[!ENDIF!]

/* Internal sectors are present or not in the current configuration. */
#define FLS_INTERNAL_SECTORS_CONFIGURED      ([!IF "AutosarExt/FlsInternalSectorsConfigured"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Base address of code flash */
#define FLS_PROGRAM_FLASH_BASE_ADDR          ([!"(ecu:get('Fls.PFlashBaseAddr'))"!]UL)
/* Base address of data flash */
#define FLS_DATA_FLASH_BASE_ADDR             ([!"(ecu:get('Fls.DFlashBaseAddr'))"!]UL)

/* Internal flash write register size */
#define FLS_INTERNAL_WRITE_SIZE              ([!IF "AutosarExt/FlsUsesAlterInterface"!]32[!ELSE!]128[!ENDIF!]UL)


/* External QSPI sectors are present or not in the current configuration. */
#define FLS_QSPI_SECTORS_CONFIGURED          ([!IF "AutosarExt/FlsExternalSectorsConfigured"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Domain ID value */
#define FLS_DOMAIN_ID                        ([!"AutosarExt/FlsDomainID"!]U)

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
                                 GLOBAL CONSTANT DECLARATIONS
==================================================================================================*/
[!IF "IMPLEMENTATION_CONFIG_VARIANT='VariantPreCompile'"!][!//
  [!IF "variant:size()<=1"!][!//
#define FLS_PRECOMPILE_SUPPORT

[!INDENT "0"!][!//
#define FLS_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fls_MemMap.h"

/* Declaration of pre-compile configuration set structure */
#define Fls_Config Fls_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!CR!]

extern const Fls_ConfigType Fls_Config;

#define FLS_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fls_MemMap.h"

[!ENDINDENT!][!//
  [!ENDIF!][!//
[!ENDIF!][!//
[!ENDCODE!][!//

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* FLS_CFG_H */
