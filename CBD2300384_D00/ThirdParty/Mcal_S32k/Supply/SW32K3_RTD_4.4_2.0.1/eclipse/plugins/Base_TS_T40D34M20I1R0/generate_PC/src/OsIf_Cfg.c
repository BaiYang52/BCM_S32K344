/*==================================================================================================
* Project : RTD AUTOSAR 4.4
* Platform : CORTEXM
* Peripheral : S32K3XX
* Dependencies : none
*
* Autosar Version : 4.4.0
* Autosar Revision : ASR_REL_4_4_REV_0000
* Autosar Conf.Variant :
* SW Version : 2.0.1
* Build Version : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
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
*   @file       OsIf_Cfg.c
*   @version 2.0.1
*
*
*   @addtogroup OSIF_DRIVER
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "OsIf_Cfg.h"
#include "OsIf_Cfg_TypesDef.h"

#if defined(USING_OS_AUTOSAROS)
#include "Os.h"
#endif /* defined(USING_OS_AUTOSAROS) */

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define OSIF_CFG_VENDOR_ID_C                    43
#define OSIF_CFG_AR_RELEASE_MAJOR_VERSION_C     4
#define OSIF_CFG_AR_RELEASE_MINOR_VERSION_C     4
#define OSIF_CFG_AR_RELEASE_REVISION_VERSION_C  0
#define OSIF_CFG_SW_MAJOR_VERSION_C             2
#define OSIF_CFG_SW_MINOR_VERSION_C             0
#define OSIF_CFG_SW_PATCH_VERSION_C             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against OsIf_Cfg.h */
#if (OSIF_CFG_VENDOR_ID_C != OSIF_CFG_VENDOR_ID)
    #error "OsIf_Cfg.c and OsIf_Cfg.h have different vendor ids"
#endif
#if ((OSIF_CFG_AR_RELEASE_MAJOR_VERSION_C    != OSIF_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (OSIF_CFG_AR_RELEASE_MINOR_VERSION_C    != OSIF_CFG_AR_RELEASE_MINOR_VERSION) || \
     (OSIF_CFG_AR_RELEASE_REVISION_VERSION_C != OSIF_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of OsIf_Cfg.c and OsIf_Cfg.h are different"
#endif
#if ((OSIF_CFG_SW_MAJOR_VERSION_C != OSIF_CFG_SW_MAJOR_VERSION) || \
     (OSIF_CFG_SW_MINOR_VERSION_C != OSIF_CFG_SW_MINOR_VERSION) || \
     (OSIF_CFG_SW_PATCH_VERSION_C != OSIF_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of OsIf_Cfg.c and OsIf_Cfg.h are different"
#endif

/* Checks against OsIf_Cfg_TypesDef.h */
#if (OSIF_CFG_VENDOR_ID_C != OSIF_CFG_TYPESDEF_VENDOR_ID)
    #error "OsIf_Cfg.c and OsIf_Cfg_TypesDef.h have different vendor ids"
#endif
#if ((OSIF_CFG_AR_RELEASE_MAJOR_VERSION_C    != OSIF_CFG_TYPESDEF_AR_RELEASE_MAJOR_VERSION) || \
     (OSIF_CFG_AR_RELEASE_MINOR_VERSION_C    != OSIF_CFG_TYPESDEF_AR_RELEASE_MINOR_VERSION) || \
     (OSIF_CFG_AR_RELEASE_REVISION_VERSION_C != OSIF_CFG_TYPESDEF_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of OsIf_Cfg.c and OsIf_Cfg_TypesDef.h are different"
#endif
#if ((OSIF_CFG_SW_MAJOR_VERSION_C != OSIF_CFG_TYPESDEF_SW_MAJOR_VERSION) || \
     (OSIF_CFG_SW_MINOR_VERSION_C != OSIF_CFG_TYPESDEF_SW_MINOR_VERSION) || \
     (OSIF_CFG_SW_PATCH_VERSION_C != OSIF_CFG_TYPESDEF_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of OsIf_Cfg.c and OsIf_Cfg_TypesDef.h are different"
#endif

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
[!NOCODE!]
[!MACRO "GetOsCounterFrequency", "EcucPartitionRef"!][!//
[!NOCODE!]
    [!VAR "CounterFrequency" = "string(null)"!]
    [!LOOP "as:modconf('Base')[1]/OsIfGeneral/OsIfCounterConfig/*"!]
        [!IF "$EcucPartitionRef != string(null)"!]
            [!IF "node:exists(OsIfCounterEcucPartitionRef) and node:refvalid(OsIfCounterEcucPartitionRef)"!]
                [!IF "node:value(OsIfCounterEcucPartitionRef) = $EcucPartitionRef"!]
                    [!IF "(node:name(../../OsIfOperatingSystemType) = 'OsIfAutosarOsType')"!]
                        [!IF "node:refvalid(OsIfOsCounterRef)"!]
                            [!IF "not(node:exists(node:ref(OsIfOsCounterRef)/OsSecondsPerTick))"!]
                                [!ERROR!]Parameter [!"OsIfOsCounterRef"!]/OsSecondsPerTick is required for counter objects that count timing related information.[!ENDERROR!]
                            [!ENDIF!]
                            [!VAR "CounterFrequency" = "num:i(round(1.0 div node:ref(OsIfOsCounterRef)/OsSecondsPerTick))"!]
                        [!ENDIF!]
                    [!ELSEIF "(node:name(../../OsIfOperatingSystemType) = 'OsIfFreeRtosType')"!]
                        [!VAR "CounterFrequency" = "num:i(0)"!]
                    [!ELSEIF "(node:name(../../OsIfOperatingSystemType) = 'OsIfZephyrOsType')"!]
                        [!VAR "CounterFrequency" = "num:i(0)"!]
                    [!ELSEIF "(node:name(../../OsIfOperatingSystemType) = 'OsIfBaremetalType')"!]
                        [!IF "node:refvalid(OsIfSystemTimerClockRef)"!]
                            [!VAR "CounterFrequency" = "num:i(node:ref(OsIfSystemTimerClockRef)/McuClockReferencePointFrequency)"!]
                        [!ENDIF!]
                    [!ENDIF!]
                    [!BREAK!]
                [!ENDIF!]
            [!ENDIF!]
        [!ELSE!]
            [!BREAK!]
        [!ENDIF!]
    [!ENDLOOP!]
    [!IF "$CounterFrequency = string(null)"!][!ERROR!]Unknown or unsupported OS Counter Frequency! ([!"node:name(../../OsIfOperatingSystemType)"!])[!ENDERROR!][!ENDIF!]
    [!CODE!][!"$CounterFrequency"!]U[!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetOsCounterFrequencySingleCore"!][!//
[!NOCODE!]
    [!VAR "CounterFrequency" = "string(null)"!]

    [!IF "(node:name(../../OsIfOperatingSystemType) = 'OsIfAutosarOsType')"!]
        [!IF "node:refvalid(OsIfOsCounterRef)"!]
            [!IF "not(node:exists(node:ref(OsIfOsCounterRef)/OsSecondsPerTick))"!]
                [!ERROR!]Parameter [!"OsIfOsCounterRef"!]/OsSecondsPerTick is required for counter objects that count timing related information.[!ENDERROR!]
            [!ENDIF!]
            [!VAR "CounterFrequency" = "num:i(round(1.0 div node:ref(OsIfOsCounterRef)/OsSecondsPerTick))"!]
        [!ENDIF!]
    [!ELSEIF "(node:name(../../OsIfOperatingSystemType) = 'OsIfFreeRtosType')"!]
        [!VAR "CounterFrequency" = "num:i(0)"!]
    [!ELSEIF "(node:name(../../OsIfOperatingSystemType) = 'OsIfZephyrOsType')"!]
        [!VAR "CounterFrequency" = "num:i(0)"!]
    [!ELSEIF "(node:name(../../OsIfOperatingSystemType) = 'OsIfBaremetalType')"!]
        [!IF "node:refvalid(OsIfSystemTimerClockRef)"!]
            [!VAR "CounterFrequency" = "num:i(node:ref(OsIfSystemTimerClockRef)/McuClockReferencePointFrequency)"!]
        [!ENDIF!]
    [!ENDIF!]

    [!IF "$CounterFrequency = string(null)"!][!ERROR!]Unknown or unsupported OS Counter Frequency! ([!"node:name(../../OsIfOperatingSystemType)"!])[!ENDERROR!][!ENDIF!]
    [!CODE!][!"$CounterFrequency"!]U[!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetOsCounterId", "EcucPartitionRef"!][!//
[!NOCODE!]
    [!VAR "CounterId" = "string(null)"!]

    [!LOOP "as:modconf('Base')[1]/OsIfGeneral/OsIfCounterConfig/*"!]
        [!IF "$EcucPartitionRef != string(null)"!]
            [!IF "node:exists(OsIfCounterEcucPartitionRef) and node:refvalid(OsIfCounterEcucPartitionRef)"!]
                [!IF "node:value(OsIfCounterEcucPartitionRef) = $EcucPartitionRef"!]
                    [!IF "(node:name(../../OsIfOperatingSystemType) = 'OsIfAutosarOsType')"!]
                        [!IF "node:refvalid(OsIfOsCounterRef)"!]
                            [!VAR "CounterId" = "node:ref(OsIfOsCounterRef)/@name"!]
                        [!ENDIF!]
                    [!ELSEIF "(node:name(../../OsIfOperatingSystemType) = 'OsIfFreeRtosType')"!]
                        [!VAR "CounterId" = "num:i(0)"!]
                    [!ELSEIF "(node:name(../../OsIfOperatingSystemType) = 'OsIfZephyrOsType')"!]
                        [!VAR "CounterId" = "num:i(0)"!]
                    [!ELSEIF "(node:name(../../OsIfOperatingSystemType) = 'OsIfBaremetalType')"!]
                        [!VAR "CounterId" = "num:i(0)"!]
                    [!ENDIF!]
                    [!BREAK!]
                [!ENDIF!]
            [!ENDIF!]
        [!ELSE!]
            [!BREAK!]
        [!ENDIF!]
    [!ENDLOOP!]
    [!IF "$CounterId = string(null)"!][!ERROR!]Unknown or unsupported OS Counter ID![!ENDERROR!][!ENDIF!]
    [!CODE!][!"$CounterId"!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetOsCounterIdSingleCore"!][!//
[!NOCODE!]
    [!VAR "CounterId" = "string(null)"!]

    [!IF "(node:name(../../OsIfOperatingSystemType) = 'OsIfAutosarOsType')"!]
        [!IF "node:refvalid(OsIfOsCounterRef)"!]
            [!VAR "CounterId" = "node:ref(OsIfOsCounterRef)/@name"!]
        [!ENDIF!]
    [!ELSEIF "(node:name(../../OsIfOperatingSystemType) = 'OsIfFreeRtosType')"!]
        [!VAR "CounterId" = "num:i(@index)"!]
    [!ELSEIF "(node:name(../../OsIfOperatingSystemType) = 'OsIfZephyrOsType')"!]
        [!VAR "CounterId" = "num:i(@index)"!]
    [!ELSEIF "(node:name(../../OsIfOperatingSystemType) = 'OsIfBaremetalType')"!]
        [!VAR "CounterId" = "num:i(@index)"!]
    [!ENDIF!]

    [!IF "$CounterId = string(null)"!][!ERROR!]Unknown or unsupported OS Counter ID! ([!"node:name(../../OsIfOperatingSystemType)"!])[!ENDERROR!][!ENDIF!]
    [!CODE!][!"$CounterId"!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetEcucPartitionRef", "CoreId"!][!//
[!NOCODE!]
    [!VAR "EcucPartitionRef" = "string(null)"!]
    [!SELECT "as:modconf('Os')[1]/OsApplication/*/OsApplicationCoreRef"!]
        [!IF "node:refvalid(.) "!]
            [!IF "num:i($CoreId) = num:i(node:value(node:ref(.)/EcucCoreId))"!]
                [!VAR "EcucPartitionRef" = "node:value(../OsAppEcucPartitionRef)"!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDSELECT!]

    [!CODE!][!"$EcucPartitionRef"!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetConfigStructName", "EcucPartitionRef"!][!//
[!NOCODE!]
    [!VAR "ConfigStructureName" = "'NULL_PTR'"!]  
    [!LOOP "OsIfGeneral/OsIfCounterConfig/*"!]
        [!IF "node:exists(OsIfCounterEcucPartitionRef) and node:refvalid(OsIfCounterEcucPartitionRef)"!]
            [!IF "node:value(OsIfCounterEcucPartitionRef) = $EcucPartitionRef"!]
                [!VAR "CntrPartitionShortName" = "node:name(node:ref(OsIfCounterEcucPartitionRef))"!]
                [!VAR "ConfigStructureName"    = "concat('&OsIf_xPredefinedConfig_', $CntrPartitionShortName)"!]
                [!BREAK!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]

    [!CODE!][!"$ConfigStructureName"!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//
[!ENDNOCODE!][!//


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
[!NOCODE!]
[!MACRO "GenerateSingleCoreConfig"!][!//
[!NOCODE!]
    [!VAR "NumOfOsCntrs" = "count(OsIfGeneral/OsIfCounterConfig/*)"!]
    [!IF "$NumOfOsCntrs > 0"!]
        [!SELECT "OsIfGeneral/OsIfCounterConfig/*[1]"!]
[!CODE!]
#define BASE_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Base_MemMap.h"

static const OsIf_ConfigType OsIf_xPredefinedConfig =
{
    /*.counterId */         [!CALL "GetOsCounterIdSingleCore"!],
    /*.counterFrequency */  [!CALL "GetOsCounterFrequencySingleCore"!]
};

#define BASE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Base_MemMap.h"

#define BASE_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Base_MemMap.h"

const OsIf_ConfigType *const OsIf_apxPredefinedConfig[OSIF_MAX_COREIDX_SUPPORTED] =
{
    &OsIf_xPredefinedConfig
};

#define BASE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Base_MemMap.h"
[!ENDCODE!]
        [!ENDSELECT!]
    [!ENDIF!][!// $NumOfOsCntrs > 0
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GenerateMultiCorePartitionConfig"!][!//
[!NOCODE!]
    [!LOOP "OsIfGeneral/OsIfEcucPartitionRef/*"!]
        [!VAR "partitionShortName" = "node:name(node:ref(.))"!]
[!CODE!]
#define BASE_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Base_MemMap.h"

static const OsIf_ConfigType OsIf_xPredefinedConfig_[!"$partitionShortName"!] =
{
    /*.counterId */         [!CALL "GetOsCounterId", "EcucPartitionRef" = "node:value(.)"!],
    /*.counterFrequency */  [!CALL "GetOsCounterFrequency", "EcucPartitionRef" = "node:value(.)"!]
};

#define BASE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Base_MemMap.h"
[!ENDCODE!]
    [!ENDLOOP!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GenerateMultiCorePredefinedConfig"!][!//
[!NOCODE!]
[!CODE!]
#define BASE_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Base_MemMap.h"

const OsIf_ConfigType *const OsIf_apxPredefinedConfig[OSIF_MAX_COREIDX_SUPPORTED] =
{
[!ENDCODE!]
    [!VAR "MaxCoreIdx"!][!"num:i(node:fallback("->num:max(as:modconf('EcuC')/EcucHardware/*/EcucCoreDefinition/*/EcucCoreId)", -1))"!][!ENDVAR!]
    [!FOR "CoreIdx" = "0" TO "$MaxCoreIdx"!]
        [!VAR "OsEcucPartitionRef"!][!CALL "GetEcucPartitionRef", "CoreId" = "$CoreIdx"!][!ENDVAR!]
[!CODE!]
    [!CALL "GetConfigStructName", "EcucPartitionRef" = "$OsEcucPartitionRef"!][!IF "$CoreIdx!=$MaxCoreIdx"!],[!ELSE!] [!ENDIF!] /*EcucCoreId = [!"$CoreIdx"!] */[!//
[!ENDCODE!]
    [!ENDFOR!][!// CoreIdx
[!CODE!]
};

#define BASE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Base_MemMap.h"
[!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!IF "OsIfGeneral/OsIfMulticoreSupport = 'true'"!]
    [!CALL "GenerateMultiCorePartitionConfig"!]
    [!CALL "GenerateMultiCorePredefinedConfig"!]
[!ELSE!]
    [!CALL "GenerateSingleCoreConfig"!]
[!ENDIF!]
[!ENDNOCODE!][!//


/*==================================================================================================
                                       GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/


#ifdef __cplusplus
}
#endif /* __cplusplus */   
/** @} */
