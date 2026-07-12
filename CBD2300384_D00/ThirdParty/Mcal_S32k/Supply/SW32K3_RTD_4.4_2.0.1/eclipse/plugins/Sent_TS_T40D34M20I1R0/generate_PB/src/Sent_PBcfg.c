
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : FLEXIO_SENT_IP
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

/**
*   @file    Sent[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c
*   @version 2.0.1
*
*   @brief   AUTOSAR Sent - module interface
*   @details Configuration Structures for PostBuild.
*
*   @addtogroup SENT_DRIVER
*   @{
*/

#ifdef __cplusplus
extern "C"
{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "CDD_Sent.h"
#include "Sent_Ipw_Cfg.h"

[!AUTOSPACING!][!//
[!NOCODE!][!//
[!INCLUDE "Sent_VersionCheck_Src_PB.m"!][!//
[!ENDNOCODE!][!//
[!//
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/*
* @file           Sent[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c
*/
#define SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C                       43
#define SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C        4
#define SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C        4
#define SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C     0
#define SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C                2
#define SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C                0
#define SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C                1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/* Check if current file and SENT header file are of the same vendor */
#if (SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != SENT_VENDOR_ID)
#error "Sent_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!].c and CDD_Sent.h have different vendor ids"
#endif
/* Check if current file and SENT header file are of the same Autosar version */
#if ((SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C     != SENT_AR_RELEASE_MAJOR_VERSION) || \
     (SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C     != SENT_AR_RELEASE_MINOR_VERSION) || \
     (SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C  != SENT_AR_RELEASE_REVISION_VERSION) \
    )
  #error "AutoSar Version Numbers of Sent_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!].c and CDD_Sent.h are different"
#endif
/* Check if current file and SENT header file are of the same Software version */
#if ((SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != SENT_SW_MAJOR_VERSION) || \
     (SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != SENT_SW_MINOR_VERSION) || \
     (SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != SENT_SW_PATCH_VERSION) \
    )
       #error "Software Version Numbers of Sent_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!].c and CDD_Sent.h are different"
#endif

/* Check if current file and SENT configuration header file are of the same vendor */
#if (SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != SENT_IPW_CFG_VENDOR_ID)
    #error "Sent[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Sent_Ipw_Cfg.h have different vendor ids"
#endif
/* Check if current file and SENT header file are of the same Autosar version */
#if ((SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C     != SENT_IPW_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C     != SENT_IPW_CFG_AR_RELEASE_MINOR_VERSION) || \
     (SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C  != SENT_IPW_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Sent[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Sent_Ipw_Cfg.h are different"
#endif
/* Check if current file and SENT header file are of the same Software version */
#if ((SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != SENT_IPW_CFG_SW_MAJOR_VERSION) || \
     (SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != SENT_IPW_CFG_SW_MINOR_VERSION) || \
     (SENT_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != SENT_IPW_CFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Sent[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Sent_Ipw_Cfg.h are different"
#endif

/*==================================================================================================
*                                   GLOBAL FUNCTION PROTOTYPES
==================================================================================================*/
[!INDENT "0"!]
/**
* @brief   Export SENT_IPW configurations.
*/
SENT_IPW_CONFIG_EXT
/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

#define SENT_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Sent_MemMap.h"

[!SELECT "SentConfigSet"!]
    [!NOCODE!]
        [!VAR "SENT_CONTROLLER_CONFIG_CNT" = "num:i(0)"!]
        [!VAR "SENT_CHANNEL_CONFIG_CNT" = "num:i(0)"!]
        [!VAR "SENT_CONTROLLER_CONFIGURED" = "num:i(count(SentControllerConfig/*))"!]
        [!VAR "SENT_CHANNEL_CONFIG_CONFIGURED" = "num:i(count(SentControllerConfig/*/SentChannelConfig/*))"!]
    [!ENDNOCODE!]
    [!CR!]
    static const Sent_ControllerConfigType Sent_aHwConfig[[!"$SENT_CONTROLLER_CONFIGURED"!]] =
    {
        [!INDENT "4"!]
            [!LOOP "node:order(SentControllerConfig/*, 'node:value(SentControllerId)')"!]
                [!VAR "ChannelCount" = "num:i(count(SentChannelConfig/*))"!]
                [!VAR "SENT_CONTROLLER_CONFIG_CNT" = "$SENT_CONTROLLER_CONFIG_CNT + 1"!]
                {
                    [!INDENT "8"!]
                        /* Controller HW offset [!"node:ref(SentHwControllerRef)/FlexioMclInstances"!]*/
                        [!IF "SentControllerActivation = 'true'"!]
                            (uint8)[!"num:i(text:split((node:ref(SentHwControllerRef)/FlexioMclInstances),'_')[2])"!]U,
                        [!ELSE!]
                            (uint8)SENT_FLEXIO_NULL_OFFSET_U8,
                        [!ENDIF!]
                        /* ID of controller [!"node:ref(SentHwControllerRef)/FlexioMclInstances"!] */
                        (uint8)[!"num:i(SentControllerId)"!]U,
                        /* Number of configured channels of Sent Controller Unit [!"node:ref(SentHwControllerRef)/FlexioMclInstances"!]*/
                        (uint8)[!"num:i($ChannelCount)"!]U,
                        /* Array of pointer point to Channel config of this Controller*/
                        &Sent_Ipw_aCtrlConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"SentControllerId"!]]
                    [!ENDINDENT!]
                }[!IF "$SENT_CONTROLLER_CONFIG_CNT < $SENT_CONTROLLER_CONFIGURED"!],[!ENDIF!][!CR!]
            [!ENDLOOP!]
        [!ENDINDENT!]
    };
    [!CR!]
[!ENDSELECT!]

[!VAR "SentEcucPartitionRefCount" = "num:i(count(./SentGeneral/SentEcucPartitionRef/*))"!]
[!IF "$SentEcucPartitionRefCount != 0"!]
    [!LOOP "./SentGeneral/SentEcucPartitionRef/*"!]
        [!VAR "SentEcuCPartitionPath" = "node:value(.)"!]
        [!VAR "SentPartitionMatchCounter" = "0"!]
        [!LOOP "../../../SentConfigSet/SentControllerConfig/*"!]
            [!IF "node:exists(./SentControllerEcucPartitionRef) and (node:value(SentControllerEcucPartitionRef) = $SentEcuCPartitionPath)"!]
                [!VAR "SentPartitionMatchCounter" = "1"!]
            [!ENDIF!]
        [!ENDLOOP!]
        [!IF "$SentPartitionMatchCounter = 1"!]
            /**
            *   @brief  Sent Controller Object's pointer structure of [!"node:name(node:ref(.))"!]
            */
            static Sent_ControllerConfigType const * const Sent_apController_[!"node:name(node:ref(.))"!][[!"$SENT_CONTROLLER_CONFIGURED"!]]=
            {
                [!INDENT "4"!]
                [!VAR "CtrIdx" = "0"!]
                [!LOOP "node:order(../../../SentConfigSet/SentControllerConfig/*, './SentControllerId')"!]
                    [!VAR "CtrIdx" = "num:i($CtrIdx + 1)"!]
                    [!IF "node:exists(./SentControllerEcucPartitionRef)"!]
                        [!IF "(node:value(./SentControllerEcucPartitionRef) = $SentEcuCPartitionPath)"!]
                            &Sent_aHwConfig[[!"num:i(./SentControllerId)"!]U][!IF "$CtrIdx < $SENT_CONTROLLER_CONFIGURED"!],[!ENDIF!][!CR!]
                        [!ELSE!]
                            NULL_PTR[!IF "$CtrIdx < $SENT_CONTROLLER_CONFIGURED"!],[!ENDIF!][!CR!]
                        [!ENDIF!]
                    [!ELSE!]
                        NULL_PTR[!IF "$CtrIdx < $SENT_CONTROLLER_CONFIGURED"!],[!ENDIF!][!CR!]
                    [!ENDIF!]
                [!ENDLOOP!]
                [!ENDINDENT!]
            };
        [!ENDIF!]
    [!ENDLOOP!]
[!ELSE!]
    /**
    *   @brief  Sent Controller Object's pointer structure
    */
    static Sent_ControllerConfigType const * const Sent_apController[[!"$SENT_CONTROLLER_CONFIGURED"!]]=
    {
        [!INDENT "4"!]
            [!VAR "CtrIdx" = "0"!]
            [!LOOP "node:order(./SentConfigSet/SentControllerConfig/*, './SentControllerId')"!]
                [!VAR "CtrIdx" = "num:i($CtrIdx + 1)"!]
                &Sent_aHwConfig[[!"num:i(./SentControllerId)"!]U][!IF "$CtrIdx < $SENT_CONTROLLER_CONFIGURED"!],[!ENDIF!][!CR!]
            [!ENDLOOP!]
        [!ENDINDENT!]
    };
[!ENDIF!]

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/
/**
*   @brief  Top level structure contain all Sent driver Config
*/

[!IF "(./IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1)"!][!// VariantPreCompile/LinkTime and at most 1 configured variant
    [!IF "$SentEcucPartitionRefCount != 0"!][!// Existing SENT EcucPartition References
        [!LOOP "./SentGeneral/SentEcucPartitionRef/*"!]
            [!VAR "SentEcuCPartitionPath" = "node:value(.)"!]
            [!VAR "SentEcucPartitionName" = "substring-after(substring-after($SentEcuCPartitionPath, 'EcuC/EcuC/'), '/')"!]
            [!VAR "SentPartitionMatchCounter" = "0"!]
            [!LOOP "../../../SentConfigSet/SentControllerConfig/*"!]
                [!IF "node:exists(./SentControllerEcucPartitionRef) and (node:value(SentControllerEcucPartitionRef) = $SentEcuCPartitionPath)"!]
                    [!VAR "SentPartitionMatchCounter" = "1"!]
                [!ENDIF!]
            [!ENDLOOP!]
            [!IF "$SentPartitionMatchCounter = 1"!]
                static const Sent_ConfigType Sent_Config_[!"$SentEcucPartitionName"!] =
                {
                    [!INDENT "4"!]
                        /* Configuration Core ID */
                        [!VAR "OsAppMatchCounter" = "0"!]
                        [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
                            [!IF "contains(node:value(./OsAppEcucPartitionRef), $SentEcucPartitionName)"!]
                                [!VAR "OsAppMatchCounter" = "1"!]
                                [!VAR "CoreID" = "node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!]
                            [!ENDIF!]
                        [!ENDLOOP!]
                        [!IF "$OsAppMatchCounter = 1"!](uint32)[!"$CoreID"!]U[!ELSE!]0U[!ENDIF!],[!CR!]
                        /* The maximum number of configured Sent Hw unit in the current configuration structure. */
                        [!VAR "CtrIdx" = "0"!]
                        [!LOOP "node:order(../../../SentConfigSet/SentControllerConfig/*, './SentControllerId')"!]
                            [!VAR "CtrIdx" = "num:i($CtrIdx + 1)"!]
                        [!ENDLOOP!]
                        [!"$CtrIdx"!]U,
                        /* Pointer to the Sent Controller Configured. */
                        Sent_apController_[!"node:name(node:ref(.))"!]
                    [!ENDINDENT!]
                };[!CR!]
            [!ENDIF!]
        [!ENDLOOP!]
    [!ELSE!][!// No ECUC partition referenced in SENT
        const Sent_ConfigType Sent_Config=
        {
        [!INDENT "4"!]
            /* Configuration Core ID */
            0U,
            /* The maximum number of configured Sent Hw unit in the current configuration structure. */
            [!VAR "CtrIdx" = "0"!]
            [!LOOP "node:order(./SentConfigSet/SentControllerConfig/*, './SentControllerId')"!]
                [!VAR "CtrIdx" = "num:i($CtrIdx + 1)"!]
            [!ENDLOOP!]
            [!"$CtrIdx"!]U,
            /* Pointer to the Sent Controller Configured. */
            Sent_apController
        [!ENDINDENT!]
        };[!CR!]
    [!ENDIF!]
[!ELSE!][!// VariantPostBuild or more than 1 configured variant
    [!IF "variant:size() > 0"!][!// If ECUC exists or there is at least one variant
        [!IF "$SentEcucPartitionRefCount != 0"!][!// Existing SENT EcucPartition References
            [!LOOP "./SentGeneral/SentEcucPartitionRef/*"!]
                [!VAR "SentEcuCPartitionPath" = "node:value(.)"!]
                [!VAR "SentEcucPartitionName" = "substring-after(substring-after($SentEcuCPartitionPath, 'EcuC/EcuC/'), '/')"!]
                [!VAR "SentPartitionMatchCounter" = "0"!]
                [!LOOP "../../../SentConfigSet/SentControllerConfig/*"!]
                    [!IF "node:exists(./SentControllerEcucPartitionRef) and (node:value(SentControllerEcucPartitionRef) = $SentEcuCPartitionPath)"!]
                        [!VAR "SentPartitionMatchCounter" = "1"!]
                    [!ENDIF!]
                [!ENDLOOP!]
                [!IF "$SentPartitionMatchCounter = 1"!]
                    const Sent_ConfigType Sent_Config_[!"variant:name()"!]_[!"$SentEcucPartitionName"!]=
                    {
                        [!INDENT "4"!]
                            /* Configuration Core ID */
                            [!VAR "OsAppMatchCounter" = "0"!]
                            [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
                                [!IF "contains(node:value(./OsAppEcucPartitionRef), $SentEcucPartitionName)"!]
                                    [!VAR "OsAppMatchCounter" = "1"!]
                                    [!VAR "CoreID" = "node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!]
                                [!ENDIF!]
                            [!ENDLOOP!]
                            [!IF "$OsAppMatchCounter = 1"!](uint32)[!"$CoreID"!]U[!ELSE!](uint32)0U[!ENDIF!],[!CR!]
                            /* The maximum number of configured Sent Hw unit in the current configuration structure. */
                            [!VAR "CtrIdx" = "0"!]
                            [!LOOP "node:order(../../../SentConfigSet/SentControllerConfig/*, './SentControllerId')"!]
                                [!VAR "CtrIdx" = "num:i($CtrIdx + 1)"!]
                            [!ENDLOOP!]
                            [!"$CtrIdx"!]U,
                            /* Pointer to the Sent Controller Configured. */
                            Sent_apController_[!"$SentEcucPartitionName"!]
                        [!ENDINDENT!]
                    };[!CR!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ELSE!][!// No ECUC partition referenced in SENT
            const Sent_ConfigType Sent_Config_[!"variant:name()"!]=
            {
                [!INDENT "4"!]
                    /* Configuration Core ID */
                    (uint32)0U,
                     /* The maximum number of configured Sent Hw unit in the current configuration structure. */
                    [!VAR "CtrIdx" = "0"!]
                    [!LOOP "node:order(./SentConfigSet/SentControllerConfig/*, './SentControllerId')"!]
                        [!VAR "CtrIdx" = "num:i($CtrIdx + 1)"!]
                    [!ENDLOOP!]
                    [!"$CtrIdx"!]U,
                    /* Pointer to the Sent Controller Configured. */
                    Sent_apController
                [!ENDINDENT!]
            };[!CR!]

        [!ENDIF!]
    [!ELSE!][!// PB with no variant or no ECUC
        [!IF "$SentEcucPartitionRefCount != 0"!][!// Existing SENT EcucPartition References
            [!LOOP "./SentGeneral/SentEcucPartitionRef/*"!]
                [!VAR "SentEcuCPartitionPath" = "node:value(.)"!]
                [!VAR "SentEcucPartitionName" = "substring-after(substring-after($SentEcuCPartitionPath, 'EcuC/EcuC/'), '/')"!]
                [!VAR "SentPartitionMatchCounter" = "0"!]
                [!LOOP "../../../SentConfigSet/SentControllerConfig/*"!]
                    [!IF "node:exists(./SentControllerEcucPartitionRef) and (node:value(SentControllerEcucPartitionRef) = $SentEcuCPartitionPath)"!]
                        [!VAR "SentPartitionMatchCounter" = "1"!]
                    [!ENDIF!]
                [!ENDLOOP!]
                [!IF "$SentPartitionMatchCounter = 1"!]
                    const Sent_ConfigType Sent_Config_[!"$SentEcucPartitionName"!]=
                    {
                        [!INDENT "4"!]
                            /* Configuration Core ID */
                            [!VAR "OsAppMatchCounter" = "0"!]
                            [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
                                [!IF "contains(node:value(./OsAppEcucPartitionRef), $SentEcucPartitionName)"!]
                                    [!VAR "OsAppMatchCounter" = "1"!]
                                    [!VAR "CoreID" = "node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!]
                                [!ENDIF!]
                            [!ENDLOOP!]
                            [!IF "$OsAppMatchCounter = 1"!](uint32)[!"$CoreID"!]U[!ELSE!](uint32)0U[!ENDIF!],[!CR!]
                            /* The maximum number of configured Sent Hw unit in the current configuration structure. */
                            [!VAR "CtrIdx" = "0"!]
                            [!LOOP "node:order(../../../SentConfigSet/SentControllerConfig/*, './SentControllerId')"!]
                                [!VAR "CtrIdx" = "num:i($CtrIdx + 1)"!]
                            [!ENDLOOP!]
                            [!"$CtrIdx"!]U,
                            /* Pointer to the Sent Controller Configured. */
                            Sent_apController_[!"$SentEcucPartitionName"!]
                        [!ENDINDENT!]
                    };[!CR!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ELSE!][!// No ECUC partition referenced in SENT
            const Sent_ConfigType Sent_Config=
            {
                [!INDENT "4"!]
                    /* Configuration Core ID */
                    (uint32)0U,
                    /* The maximum number of configured Sent Hw unit in the current configuration structure. */
                    [!VAR "CtrIdx" = "0"!]
                    [!LOOP "node:order(./SentConfigSet/SentControllerConfig/*, './SentControllerId')"!]
                        [!VAR "CtrIdx" = "num:i($CtrIdx + 1)"!]
                    [!ENDLOOP!]
                    [!"$CtrIdx"!]U,
                    /* Pointer to the Sent Controller Configured. */
                    Sent_apController
                [!ENDINDENT!]
            };[!CR!]

        [!ENDIF!]
    [!ENDIF!]
[!ENDIF!]

[!IF "(./IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1)"!]
    [!IF "$SentEcucPartitionRefCount != 0"!][!// Existing SENT EcucPartition References
        [!SELECT "as:modconf('EcuC')[1]"!]
            [!VAR "MaxCoreIdConfig" = "num:i(count(./EcucHardware/*[1]/EcucCoreDefinition/*))"!]
        [!ENDSELECT!]
        const Sent_ConfigType * const [!WS!]Sent_Config[[!"$MaxCoreIdConfig"!]U] =
        {
            [!INDENT "4"!]
            [!IF "num:i(count(SentGeneral/SentEcucPartitionRef/*)) = '0'"!][!//
                &Sent_Config
            [!ELSE!]
                [!VAR "CoreIdx" = "0"!]
                [!LOOP "node:order(as:modconf('EcuC')[1]/EcucHardware/*[1]/EcucCoreDefinition/*, './EcucCoreId')"!]
                    [!VAR "OsAppPart" = "''"!]
                    [!VAR "CoreIdx" = "$CoreIdx + 1"!]
                    [!VAR "CoreId" = "node:value(./EcucCoreId)"!]
                    [!VAR "SentPartShortName" = "''"!]
                    [!VAR "SentPartitionMatchCounter" = "0"!]
                    [!VAR "SentPartMatchCounter" = "0"!]
                    [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
                        [!IF "$CoreId = node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!]
                            [!VAR "OsAppPart" = "node:value(./OsAppEcucPartitionRef)"!]
                        [!ENDIF!]
                    [!ENDLOOP!]
                    [!LOOP "as:modconf('Sent')[1]/SentGeneral/SentEcucPartitionRef/*"!]
                        [!LOOP "../../../SentConfigSet/SentControllerConfig/*"!]
                            [!IF "node:exists(./SentControllerEcucPartitionRef) and (node:value(SentControllerEcucPartitionRef) = $OsAppPart)"!]
                                [!VAR "SentPartitionMatchCounter" = "1"!]
                            [!ENDIF!]
                        [!ENDLOOP!]
                        [!IF "$OsAppPart = node:value(.)"!]
                            [!VAR "SentPartMatchCounter" = "$SentPartMatchCounter + 1"!]
                            [!VAR "SentPartShortName" = "substring-after(substring-after(., 'EcuC/EcuC/'), '/')"!]
                        [!ENDIF!]
                    [!ENDLOOP!]
                    [!IF "$SentPartMatchCounter = 1"!]
                        [!IF "$SentPartitionMatchCounter = 1"!]&Sent_Config_[!"$SentPartShortName"!][!ELSE!]NULL_PTR[!ENDIF!]
                    [!ELSE!]NULL_PTR[!ENDIF!][!IF "$CoreIdx < $MaxCoreIdConfig"!],[!ENDIF!][!CR!]
                [!ENDLOOP!]
            [!ENDIF!]
            [!ENDINDENT!]
        };
    [!ENDIF!]
[!ENDIF!]

[!ENDINDENT!]

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

#define SENT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Sent_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
