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
[!VAR "variantName" = "''"!][!//
[!IF "var:defined('postBuildVariant')"!][!//
[!VAR "variantName" = "concat($postBuildVariant, '_')"!][!//
[!ENDIF!][!//

/**
*   @file Qspi_Ip_[!"$variantName"!]PBcfg.c
*
*   @addtogroup FLS
*   @{
*/

/* implements Qspi_Ip_PBcfg.c_Artifact */

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
#include "Qspi_Ip_[!"$variantName"!]PBcfg.h"
[!ENDINDENT!][!//

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define QSPI_IP_[!"$variantName"!]PBCFG_VENDOR_ID_C                      43
#define QSPI_IP_[!"$variantName"!]PBCFG_AR_RELEASE_MAJOR_VERSION_C       4
#define QSPI_IP_[!"$variantName"!]PBCFG_AR_RELEASE_MINOR_VERSION_C       4
#define QSPI_IP_[!"$variantName"!]PBCFG_AR_RELEASE_REVISION_VERSION_C    0
#define QSPI_IP_[!"$variantName"!]PBCFG_SW_MAJOR_VERSION_C               2
#define QSPI_IP_[!"$variantName"!]PBCFG_SW_MINOR_VERSION_C               0
#define QSPI_IP_[!"$variantName"!]PBCFG_SW_PATCH_VERSION_C               1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and Qspi_Ip_[!"$variantName"!]PBcfg.h header file are of the same vendor */
#if (QSPI_IP_[!"$variantName"!]PBCFG_VENDOR_ID_C != QSPI_IP_[!"$variantName"!]PBCFG_VENDOR_ID)
    #error "Qspi_Ip_[!"$variantName"!]PBcfg.c and Qspi_Ip_[!"$variantName"!]PBcfg.h have different vendor ids"
#endif
/* Check if current file and Qspi_Ip_[!"$variantName"!]PBcfg.h header file are of the same Autosar version */
#if ((QSPI_IP_[!"$variantName"!]PBCFG_AR_RELEASE_MAJOR_VERSION_C    != QSPI_IP_[!"$variantName"!]PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (QSPI_IP_[!"$variantName"!]PBCFG_AR_RELEASE_MINOR_VERSION_C    != QSPI_IP_[!"$variantName"!]PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (QSPI_IP_[!"$variantName"!]PBCFG_AR_RELEASE_REVISION_VERSION_C != QSPI_IP_[!"$variantName"!]PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
#error "AutoSar Version Numbers of Qspi_Ip_[!"$variantName"!]PBcfg.c and Qspi_Ip_[!"$variantName"!]PBcfg.h are different"
#endif
/* Check if current file and Qspi_Ip_[!"$variantName"!]PBcfg.h header file are of the same software version */
#if ((QSPI_IP_[!"$variantName"!]PBCFG_SW_MAJOR_VERSION_C != QSPI_IP_[!"$variantName"!]PBCFG_SW_MAJOR_VERSION) || \
     (QSPI_IP_[!"$variantName"!]PBCFG_SW_MINOR_VERSION_C != QSPI_IP_[!"$variantName"!]PBCFG_SW_MINOR_VERSION) || \
     (QSPI_IP_[!"$variantName"!]PBCFG_SW_PATCH_VERSION_C != QSPI_IP_[!"$variantName"!]PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Qspi_Ip_[!"$variantName"!]PBcfg.c and Qspi_Ip_[!"$variantName"!]PBcfg.h are different"
#endif

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/
[!NOCODE!][!//
[!VAR "FlsQspiHyperflashEnVar" = "'false'"!][!//
[!IF "AutosarExt/FlsQspiHyperflashEnable ='true'"!][!//
    [!VAR "FlsQspiHyperflashEnVar" = "'true'"!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!/* *** multiple inclusion protection *** */!]
[!IF "not(var:defined('FLS_CFG_SPECIFIC_MACROS_M4'))"!][!//
    [!VAR "FLS_CFG_SPECIFIC_MACROS_M4"="'true'"!][!//
    [!MACRO "FlsGetWritePageSizeValue","FlsEnumName"!][!//
    [!NOCODE!][!//
        [!IF "$FlsEnumName='FLS_WRITE_DOUBLE_WORD'"!][!//
            [!VAR "OutWritePageSize" = "8"!][!//
        [!ELSEIF "$FlsEnumName='FLS_WRITE_PAGE'"!][!//
            [!VAR "OutWritePageSize" = "32"!][!//
        [!ELSEIF "$FlsEnumName='FLS_WRITE_QUAD_PAGE'"!][!//
            [!VAR "OutWritePageSize" = "128"!][!//
        [!ELSEIF "$FlsEnumName='FLS_WRITE_128BYTES_PAGE'"!][!//
            [!VAR "OutWritePageSize" = "128"!][!//
        [!ELSEIF "$FlsEnumName='FLS_WRITE_256BYTES_PAGE'"!][!//
            [!VAR "OutWritePageSize" = "256"!][!//
        [!ELSEIF "$FlsEnumName='FLS_WRITE_512BYTES_PAGE'"!][!//
            [!VAR "OutWritePageSize" = "512"!][!//
        [!ELSE!][!//
            [!VAR "OutWritePageSize" = "0"!][!//
        [!ENDIF!][!//
    [!ENDNOCODE!][!//
    [!ENDMACRO!][!//
    [!//
    [!MACRO "FlsGetSectorId","FlsEnumName"!][!//
    [!NOCODE!][!//
        [!VAR "OutFlsSectorId" = "4294967295"!][!//
        [!VAR "LocationStartCodeSector" = "0"!][!//
        [!VAR "SectorTotal" = "num:i(count(ecu:list('Fls.Sector.Physical.List')))"!][!//
        [!FOR "LocCount" = "1" TO "num:i(count(ecu:list('Fls.Sector.Physical.List')))"!][!//
            [!IF "contains((ecu:list('Fls.Sector.Physical.List')[num:i($LocCount)]), 'DATA') = 'true'"!][!//
                 [!VAR "LocationStartCodeSector" = "$LocationStartCodeSector + 1"!][!//
            [!ENDIF!][!//
            [!IF "(substring-after((ecu:list('Fls.Sector.Physical.List')[num:i($LocCount)]), '_') = $FlsEnumName)"!][!//
                [!VAR "OutFlsSectorId" = "$LocCount - 1"!][!//
            [!ENDIF!][!//
        [!ENDFOR!][//
    [!ENDNOCODE!][!//
    [!ENDMACRO!][!//
    [!//
[!INDENT "0"!][!//
    [!MACRO "FlsPhysicalSectorlist","FlashSectorSize"!][!//
    [!IF "contains(./FlsPhysicalSector,'DATA') = 'true'"!][!//
        [!WS "4"!](FLS_DATA_FLASH_BASE_ADDR + 0x[!"text:toupper(text:replace(num:inttohex(num:i($OutFlsSectorId * $FlashSectorSize),6), '0x', ''))"!]UL),  /* pSectorStartAddressPtr */
        [!WS "4"!][!"num:i($OutFlsSectorId)"!]U  /* Sector location to calculate cfgCRC */
    [!ELSEIF "contains(./FlsPhysicalSector,'CODE') = 'true'"!][!//
        [!VAR "CurrentCodeSectorID" = " num:i($OutFlsSectorId - $LocationStartCodeSector)"!][!//
        [!WS "4"!](FLS_PROGRAM_FLASH_BASE_ADDR + 0x[!"text:toupper(text:replace(num:inttohex(num:i(($OutFlsSectorId -$LocationStartCodeSector) * $FlashSectorSize),6), '0x', ''))"!]UL),  /* pSectorStartAddressPtr */
        [!WS "4"!][!"num:i($OutFlsSectorId)"!]U  /* Sector location to calculate cfgCRC */
    [!ELSE!][!// Utest areas
        [!WS "4"!](0x1B000000UL),  /* pSectorStartAddressPtr */
        [!WS "4"!][!"num:i($OutFlsSectorId)"!]U  /* Sector location to calculate cfgCRC */
    [!ENDIF!][!//
    [!ENDMACRO!][!//
[!ENDINDENT!][!//
    [!//
    [!MACRO "FlsPhysicalInternalSectorHardwareAddress","FlashSectorSize"!][!//
    [!NOCODE!][!//
        [!VAR "InternalSectorHardwareAddress" = "4294967295"!][!//
        [!IF "contains(./FlsPhysicalSector,'DATA') = 'true'"!]
            [!VAR "InternalSectorHardwareAddress" = "num:hextoint('0x10000000') + num:i($OutFlsSectorId * $FlashSectorSize)"!][!// 
        [!ELSEIF "contains(./FlsPhysicalSector,'CODE') = 'true'"!]
            [!VAR "CurrentCodeSectorID" = " num:i($OutFlsSectorId - $LocationStartCodeSector)"!][!//
            [!VAR "InternalSectorHardwareAddress" = "num:hextoint('0x00400000') + num:i($CurrentCodeSectorID * $FlashSectorSize)"!][!//
        [!ELSE!][!// Utest areas
            [!VAR "InternalSectorHardwareAddress" = "num:hextoint('0x1B000000')"!][!//
        [!ENDIF!][!//
    [!ENDNOCODE!][!//
    [!ENDMACRO!][!//
    [!MACRO "FlsGetMemifModeValue","FlsEnumName"!][!//
    [!NOCODE!][!//
        [!IF "$FlsEnumName='MEMIF_MODE_SLOW'"!][!//
            [!VAR "OutMemifModeValue" = "0"!][!//
        [!ELSEIF "$FlsEnumName='MEMIF_MODE_FAST'"!][!//
            [!VAR "OutMemifModeValue" = "1"!][!//
        [!ELSE!][!//
            [!VAR "OutMemifModeValue" = "4294967295"!][!//
        [!ENDIF!][!//
    [!ENDNOCODE!][!//
    [!ENDMACRO!][!//
    [!//
    [!//
[!ENDIF!][!//


[!INDENT "0"!][!//
[!IF "node:exists(FlsConfigSet/FlsQspiInitCallout) or node:exists(FlsConfigSet/FlsQspiResetCallout) or node:exists(FlsConfigSet/FlsQspiErrorCheckCallout) or node:exists(FlsConfigSet/FlsQspiEccCheckCallout)"!][!//
    #define FLS_START_SEC_CODE
    #include "Fls_MemMap.h"

    /* Declaration of QSPI callout functions */
    [!LOOP "text:order(text:split(normalize-space(text:join(FlsConfigSet/FlsQspiInitCallout | FlsConfigSet/FlsQspiResetCallout | FlsConfigSet/FlsQspiErrorCheckCallout))))"!][!//
        extern Qspi_Ip_StatusType [!"."!](uint32 instance);
    [!ENDLOOP!][!//
    [!IF "node:exists(FlsConfigSet/FlsQspiEccCheckCallout)"!][!//
        extern Qspi_Ip_StatusType [!"./FlsConfigSet/FlsQspiEccCheckCallout"!](uint32 instance, uint32 startAddress, uint32 dataLength);
    [!ENDIF!][!//

    #define FLS_STOP_SEC_CODE
    #include "Fls_MemMap.h"
[!ENDIF!][!//

[!VAR "configName" = "as:name(FlsConfigSet)"!][!//
[!LOOP "FlsConfigSet"!][!//
[!AUTOSPACING!]

#define FLS_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fls_MemMap.h"

/* External QSPI flash parameters. */
[!IF "num:i(count(FlsExternalDriver/FlsMem/*)) > 0"!][!//
    /* paFlashConnectionCfg */
    const Qspi_Ip_MemoryConnectionType [!"$configName"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_paFlashConnectionCfg[[!"num:i(count(FlsExternalDriver/FlsMem/*))"!]U] =
    {
        [!VAR "FlsLoopIt" = "count(FlsExternalDriver/FlsMem/*)"!][!//
        [!LOOP "FlsExternalDriver/FlsMem/*"!][!//
            [!VAR "FlsLoopIt" = "$FlsLoopIt - 1"!][!//
            [!WS "4"!]{
            [!WS "8"!][!"substring-after(node:ref(./qspiInstance)/ControllerName, 'FLS_QSPI_')"!]U,  /* qspiInstance */
            [!WS "8"!][!"./connectionType"!],                           /* connectionType */
            [!WS "8"!][!"./MemAlignment"!]U                             /* memAlignment */
            [!WS "4"!]}[!IF "$FlsLoopIt != 0"!],[!ENDIF!][!CR!]
        [!ENDLOOP!][!//
    };
[!ENDIF!][!//

[!IF "num:i(count(FlsExternalDriver/ControllerCfg/*)) > 0"!][!//
    /* paQspiUnitCfg */
    const Qspi_Ip_ControllerConfigType [!"$configName"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_paQspiUnitCfg[[!"num:i(count(FlsExternalDriver/ControllerCfg/*))"!]U] =
    {
        [!VAR "FlsLoopIt" = "count(FlsExternalDriver/ControllerCfg/*)"!][!//
        [!LOOP "FlsExternalDriver/ControllerCfg/*"!][!//
            [!VAR "FlsLoopIt" = "$FlsLoopIt - 1"!][!//
            [!WS "4"!]/* [!"node:name(.)"!] */
            [!WS "4"!]{
                [!WS "8"!][!"./FlsSerialFlashA1Size"!]U,                                /* memSizeA1 */
                [!WS "8"!][!"./FlsSerialFlashA2Size"!]U,                                /* memSizeA2 */
                [!WS "8"!][!"./FlsHwUnitTcsh"!]U,                                /* csHoldTime */
                [!WS "8"!][!"./FlsHwUnitTcss"!]U,                                /* csSetupTime */
                [!WS "8"!][!"./FlsHwUnitSamplingModeA"!],                                /* readModeA */
                [!WS "8"!][!"./FlsHwUnitSamplingDly"!],                                /* sampleDelay */
                [!WS "8"!][!"./FlsHwUnitSamplingEdge"!],                                /* samplePhase */
                [!WS "8"!]{
                    [!WS "12"!]/* dllSettingsA */
                    [!WS "12"!][!"./DllCfgA/DllCfgADllMode"!],                                /* dllMode */
                    [!WS "12"!][!IF "./DllCfgA/DllCfgADllCraFreqEn"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!],                                /* freqEnable */
                    [!WS "12"!][!IF "./DllCfgA/DllCfgADllMode='QSPI_IP_DLL_BYPASSED'"!][!"./DllCfgA/DllCfgADllCraSlvDlyCoarse"!][!ELSE!][!"./DllCfgA/DllCfgADllCraSlvDlyOffset"!][!ENDIF!]U,                                /* coarseDelay */
                    [!WS "12"!][!"./DllCfgA/DllCfgADllCraSlvFineOffset"!]U,                         /* fineDelay */
                    [!WS "12"!][!"./DllCfgA/DllCfgADllTapSelect"!]U                                 /* tapSelect */
                [!WS "8"!]},
                [!WS "8"!]{
                    [!WS "12"!]/* AHBConfig */
                    [!WS "12"!]{
                        [!WS "16"!]/* masters */
                        [!WS "16"!][!"./FlsAhbBuffer/*[1]/FlsAhbBufferMasterId"!]U,  /* buffer 0 master ID */
                        [!WS "16"!][!"./FlsAhbBuffer/*[2]/FlsAhbBufferMasterId"!]U,  /* buffer 1 master ID */
                        [!WS "16"!][!"./FlsAhbBuffer/*[3]/FlsAhbBufferMasterId"!]U,  /* buffer 2 master ID */
                        [!WS "16"!][!"./FlsAhbBuffer/*[4]/FlsAhbBufferMasterId"!]U   /* buffer 3 master ID */
                    [!WS "12"!]},
                    [!WS "12"!]{
                        [!WS "16"!]/* sizes */
                        [!WS "16"!][!"./FlsAhbBuffer/*[1]/FlsAhbBufferSize"!]U,  /* buffer 0 size */
                        [!WS "16"!][!"./FlsAhbBuffer/*[2]/FlsAhbBufferSize"!]U,  /* buffer 1 size */
                        [!WS "16"!][!"./FlsAhbBuffer/*[3]/FlsAhbBufferSize"!]U,  /* buffer 2 size */
                        [!WS "16"!][!"./FlsAhbBuffer/*[4]/FlsAhbBufferSize"!]U   /* buffer 3 size */
                    [!WS "12"!]},
                    [!IF "./FlsAhbBuffer/*[4]/FlsAhbBufferAllMasters"!]
                    [!WS "12"!](boolean)TRUE                                /* allMasters */
                    [!ELSE!]
                    [!WS "12"!](boolean)FALSE                                /* allMasters */
                    [!ENDIF!]
                [!WS "8"!]}
            [!WS "4"!]}[!IF "$FlsLoopIt != 0"!],[!ENDIF!][!CR!]
        [!ENDLOOP!][!//
    };
[!ENDIF!][!//

[!MACRO "ComputeLutIndex", "SeqNo" = "0", "Text" = "", "Comma" = "','"!]
    [!IF "node:exists($SeqNo)"!]
        [!VAR "FlsLutIdx" = "0"!][!//
        [!VAR "FlsLutSelect" = "node:ref($SeqNo)/FlsLUTIndex"!][!//
        [!LOOP "node:order(./FlsLUT/*, 'node:value(./FlsLUTIndex)')"!][!// Iterate through the Fls LUT Index
            [!IF "$FlsLutSelect <= node:value(./FlsLUTIndex)"!][!BREAK!][!ENDIF!]
            [!VAR "FlsLutIdx" = "$FlsLutIdx + count(./FlsInstructionOperandPair/*) + 1"!][!//
        [!ENDLOOP!][!//
        [!"num:i($FlsLutIdx)"!]U[!"$Comma"!]                    /* [!"$Text"!] */
    [!ELSE!]
        QSPI_IP_LUT_INVALID[!"$Comma"!]                    /* [!"$Text"!] */
    [!ENDIF!]
[!ENDMACRO!]
[!MACRO "ComputeLutIndex_Op", "SeqNo" = "0", "Text" = ""!]
    [!IF "node:exists($SeqNo)"!]
        [!VAR "FlsLutIdx" = "0"!][!//
        [!VAR "FlsLutSelect" = "node:ref($SeqNo)/FlsLUTIndex"!][!//
        [!LOOP "node:order(../../FlsLUT/*, 'node:value(./FlsLUTIndex)')"!][!// Iterate through the Fls LUT Index
            [!IF "$FlsLutSelect <= node:value(./FlsLUTIndex)"!][!BREAK!][!ENDIF!]
            [!VAR "FlsLutIdx" = "$FlsLutIdx + count(./FlsInstructionOperandPair/*) + 1"!][!//
        [!ENDLOOP!][!//
        [!"num:i($FlsLutIdx)"!]U,                               /* [!"$Text"!] */
    [!ELSE!]
        QSPI_IP_LUT_INVALID,                               /* [!"$Text"!] */
    [!ENDIF!]
[!ENDMACRO!]

    [!LOOP "FlsExternalDriver/MemCfg/*"!][!//
        [!IF "num:i(count(./initConfiguration/*)) > 0"!]
            /* paInitOperations */
            static const Qspi_Ip_InitOperationType [!"node:name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_paInitOperations_[!"node:pos(.)"!][[!"num:i(count(./initConfiguration/*))"!]U] =
            {
                [!VAR "FlsLoopIt" = "count(./initConfiguration/*)"!][!//
                [!LOOP "./initConfiguration/*"!][!//
                    [!VAR "FlsLoopIt" = "$FlsLoopIt - 1"!][!//
                    [!WS "4"!]{
                        [!WS "8"!][!"./opType"!],                      /* opType */
                        [!WS "8"!][!CALL "ComputeLutIndex_Op", "SeqNo" = "node:when(node:exists(./command1Lut), ./command1Lut)", "Text" = "'command1Lut'"!]
                        [!WS "8"!][!CALL "ComputeLutIndex_Op", "SeqNo" = "node:when(node:exists(./command2Lut), ./command2Lut)", "Text" = "'command2Lut'"!]
                        [!WS "8"!][!CALL "ComputeLutIndex_Op", "SeqNo" = "node:when(node:exists(./weLut), ./weLut)", "Text" = "'weLut'"!]
                        [!WS "8"!][!"./addr"!]U,                      /* addr */
                        [!WS "8"!][!"./size"!]U,                      /* size */
                        [!WS "8"!][!"./shift"!]U,                      /* shift */
                        [!WS "8"!][!"./width"!]U,                      /* width */
                        [!WS "8"!][!"./value"!]U,                      /* value */
                        [!IF "node:refvalid(./ctrlCfgPtr)"!]
                            [!WS "8"!]&([!"$configName"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_paQspiUnitCfg[[!"node:pos(node:ref(./ctrlCfgPtr))"!]U])  /* ctrlCfgPtr */
                        [!ELSE!]
                            [!WS "8"!]NULL_PTR  /* ctrlCfgPtr */
                        [!ENDIF!]
                    [!WS "4"!]}[!IF "$FlsLoopIt != 0"!],[!ENDIF!][!CR!]
                [!ENDLOOP!][!//
            };
        [!ENDIF!]

        [!VAR "FlsLutSize" = "0"!][!//
        [!LOOP "./FlsLUT/*"!][!//
            [!VAR "FlsLutSize" = "$FlsLutSize + count(./FlsInstructionOperandPair/*) + 1"!][!//
        [!ENDLOOP!][!//

        /* paLutOperations */
        static const Qspi_Ip_InstrOpType [!"node:name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_paLutOperations_[!"node:pos(.)"!][[!"num:i($FlsLutSize)"!]U] =
        {
            [!VAR "FlsLutSize" = "num:i(count(./FlsLUT/*))"!][!//
            [!VAR "FlsLutIdx" = "0"!][!//
            [!LOOP "node:order(./FlsLUT/*, 'node:value(./FlsLUTIndex)')"!][!// Iterate through the Fls LUT Index
                [!VAR "FlsLutSize" = "$FlsLutSize -1"!][!//
                [!WS "4"!]/*  [!"num:i($FlsLutIdx)"!]: [!"node:name(.)"!]  */[!CR!]
                [!LOOP "node:order(./FlsInstructionOperandPair/*, 'node:value(./FlsInstrOperPairIndex)')"!][!// Iterate through the Fls Instruction Operand Pair Index
                    [!VAR "FlsLutIdx" = "$FlsLutIdx + 1"!][!//
                    [!WS "4"!](Qspi_Ip_InstrOpType)((Qspi_Ip_InstrOpType)[!"./FlsLUTInstruction"!] | (Qspi_Ip_InstrOpType)[!"./FlsLUTPad"!] | (Qspi_Ip_InstrOpType)[!"./FlsLUTOperand"!]U),
                [!ENDLOOP!][!//
                [!WS "4"!](Qspi_Ip_InstrOpType)(QSPI_IP_LUT_INSTR_STOP)[!IF "$FlsLutSize != 0"!],[!ENDIF!][!CR!]
                [!VAR "FlsLutIdx" = "$FlsLutIdx + 1"!][!//
            [!ENDLOOP!][!//
        };

    [!ENDLOOP!][!//

[!IF "num:i(count(FlsExternalDriver/MemCfg/*)) > 0"!][!//
    /* paFlashCfg */
    const Qspi_Ip_MemoryConfigType [!"$configName"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_paFlashCfg[[!"num:i(count(FlsExternalDriver/MemCfg/*))"!]U] =
    {
        [!VAR "FlsLoopIt" = "count(FlsExternalDriver/MemCfg/*)"!][!//
        [!LOOP "FlsExternalDriver/MemCfg/*"!][!//
            [!VAR "FlsLoopIt" = "$FlsLoopIt - 1"!][!//
            [!WS "4"!]{
                [!WS "8"!]/* [!"node:name(.)"!] */
                [!WS "8"!][!"./MemCfgSize"!]U,                               /* memSize */
                [!WS "8"!][!"./MemCfgPageSize"!]U,                               /* pageSize */
                [!WS "8"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./MemCfgReadLUT), ./MemCfgReadLUT)", "Text" = "'readLut'"!]
                [!WS "8"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./MemCfgWriteLUT), ./MemCfgWriteLUT)", "Text" = "'writeLut'"!]
                [!WS "8"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./MemCfgRead0xxLUT), ./MemCfgRead0xxLUT)", "Text" = "'read0xxLut'"!]
                [!WS "8"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./MemCfgRead0xxLUTAHB), ./MemCfgRead0xxLUTAHB)", "Text" = "'read0xxLutAHB'"!]
                [!WS "8"!]/* readIdSettings */
                [!WS "8"!]{
                    [!WS "12"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./MemCfgReadIdSettings/MemCfgReadIdLUT), ./MemCfgReadIdSettings/MemCfgReadIdLUT)", "Text" = "'readIdLut'"!]
                    [!WS "12"!][!"./MemCfgReadIdSettings/MemCfgReadIdSize"!]U,                               /* readIdSize */
                    [!WS "12"!][!"./MemCfgReadIdSettings/FlsQspiDeviceId"!]U  /* readIdExpected */
                [!WS "8"!]},
                [!WS "8"!]{
                    [!WS "12"!]/* eraseSettings */
                    [!WS "12"!]{
                        [!WS "16"!]{
                            [!WS "20"!]/* eraseTypes[0] */
                            [!WS "20"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./MemCfgEraseSettings/MemCfgErase1LUT), ./MemCfgEraseSettings/MemCfgErase1LUT)", "Text" = "'eraseLut'"!]
                            [!WS "20"!][!"./MemCfgEraseSettings/MemCfgErase1Size"!]U                               /* size */
                        [!WS "16"!]},
                        [!WS "16"!]{
                            [!WS "20"!]/* eraseTypes[1] */
                            [!WS "20"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./MemCfgEraseSettings/MemCfgErase2LUT), ./MemCfgEraseSettings/MemCfgErase2LUT)", "Text" = "'eraseLut'"!]
                            [!WS "20"!][!"./MemCfgEraseSettings/MemCfgErase2Size"!]U                               /* size */
                        [!WS "16"!]},
                        [!WS "16"!]{
                            [!WS "20"!]/* eraseTypes[2] */
                            [!WS "20"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./MemCfgEraseSettings/MemCfgErase3LUT), ./MemCfgEraseSettings/MemCfgErase3LUT)", "Text" = "'eraseLut'"!]
                            [!WS "20"!][!"./MemCfgEraseSettings/MemCfgErase3Size"!]U                               /* size */
                        [!WS "16"!]},
                        [!WS "16"!]{
                            [!WS "20"!]/* eraseTypes[3] */
                            [!WS "20"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./MemCfgEraseSettings/MemCfgErase4LUT), ./MemCfgEraseSettings/MemCfgErase4LUT)", "Text" = "'eraseLut'"!]
                            [!WS "20"!][!"./MemCfgEraseSettings/MemCfgErase4Size"!]U                               /* size */
                        [!WS "16"!]}
                    [!WS "12"!]},
                    [!WS "12"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./MemCfgEraseSettings/ChipEraseLUT), ./MemCfgEraseSettings/ChipEraseLUT)", "Text" = "'chipEraseLut'", "Comma" = "' '"!]
                [!WS "8"!]},
                [!WS "8"!]{
                    [!WS "12"!]/* statusConfig */
                    [!WS "12"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./statusConfig/statusRegInitReadLut), ./statusConfig/statusRegInitReadLut)", "Text" = "'statusRegInitReadLut'"!]
                    [!WS "12"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./statusConfig/statusRegReadLut), ./statusConfig/statusRegReadLut)", "Text" = "'statusRegReadLut'"!]
                    [!WS "12"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./statusConfig/statusRegWriteLut), ./statusConfig/statusRegWriteLut)", "Text" = "'statusRegWriteLut'"!]
                    [!WS "12"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./statusConfig/writeEnableSRLut), ./statusConfig/writeEnableSRLut)", "Text" = "'writeEnableSRLut'"!]
                    [!WS "12"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./statusConfig/writeEnableLut), ./statusConfig/writeEnableLut)", "Text" = "'writeEnableLut'"!]
                    [!WS "12"!][!"./statusConfig/regSize"!]U,                               /* regSize */
                    [!WS "12"!][!"./statusConfig/busyOffset"!]U,                               /* busyOffset */
                    [!WS "12"!][!"./statusConfig/busyValue"!]U,                               /* busyValue */
                    [!WS "12"!][!"./statusConfig/writeEnableOffset"!]U,                               /* writeEnableOffset */
                    [!WS "12"!][!"./statusConfig/blockProtectionOffset"!]U,                               /* blockProtectionOffset */
                    [!WS "12"!][!"./statusConfig/blockProtectionWidth"!]U,                               /* blockProtectionWidth */
                    [!WS "12"!][!"./statusConfig/blockProtectionValue"!]U                                /* blockProtectionValue */
                [!WS "8"!]},
                [!WS "8"!]{
                    [!WS "12"!]/* suspendSettings */
                    [!WS "12"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./suspendSettings/eraseSuspendLut), ./suspendSettings/eraseSuspendLut)", "Text" = "'eraseSuspendLut'"!]
                    [!WS "12"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./suspendSettings/eraseResumeLut), ./suspendSettings/eraseResumeLut)", "Text" = "'eraseResumeLut'"!]
                    [!WS "12"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./suspendSettings/programSuspendLut), ./suspendSettings/programSuspendLut)", "Text" = "'programSuspendLut'"!]
                    [!WS "12"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./suspendSettings/programResumeLut), ./suspendSettings/programResumeLut)", "Text" = "'programResumeLut'", "Comma" = "' '"!]
                [!WS "8"!]},
                [!WS "8"!]{
                    [!WS "12"!]/* resetSettings */
                    [!WS "12"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./resetSettings/resetCmdLut), ./resetSettings/resetCmdLut)", "Text" = "'resetCmdLut'"!]
                    [!WS "12"!][!"./resetSettings/resetCmdCount"!]U                               /* resetCmdCount */
                [!WS "8"!]},
                [!WS "8"!]{
                    [!WS "12"!]/* initResetSettings */
                    [!WS "12"!][!CALL "ComputeLutIndex", "SeqNo" = "node:when(node:exists(./initResetSettings/resetCmdLut), ./initResetSettings/resetCmdLut)", "Text" = "'resetCmdLut'"!]
                    [!WS "12"!][!"./initResetSettings/resetCmdCount"!]U                               /* resetCmdCount */
                [!WS "8"!]},
                [!WS "8"!]{
                    [!WS "12"!]/* initConfiguration */
                    [!WS "12"!][!"num:i(count(./initConfiguration/*))"!]U,                                  /* opCount */
                    [!IF "num:i(count(./initConfiguration/*)) > 0"!]
                        [!WS "12"!](Qspi_Ip_InitOperationType *)[!"node:name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_paInitOperations_[!"node:pos(.)"!]                    /* operations */
                    [!ELSE!]
                        [!WS "12"!]NULL_PTR,                               /* [!"$Text"!] */
                    [!ENDIF!]
                [!WS "8"!]},
                [!WS "8"!]{
                    [!WS "12"!]/* lutSequences */
                    [!VAR "FlsLutSize" = "0"!][!//
                    [!LOOP "./FlsLUT/*"!][!//
                        [!VAR "FlsLutSize" = "$FlsLutSize + count(./FlsInstructionOperandPair/*) + 1"!][!//
                    [!ENDLOOP!][!//
                    [!WS "12"!][!"num:i($FlsLutSize)"!]U,                                        /* opCount */
                    [!WS "12"!](Qspi_Ip_InstrOpType *)[!"node:name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_paLutOperations_[!"node:pos(.)"!]       /* lutOps */
                [!WS "8"!]},
                [!IF "node:exists(../../../FlsQspiInitCallout)"!]
                    [!WS "8"!]&[!"normalize-space(text:join(//FlsConfigSet/FlsQspiInitCallout))"!],                    /* initCallout */
                [!ELSE!]
                    [!WS "8"!]NULL_PTR,                               /* initCallout */
                [!ENDIF!]
                [!IF "node:exists(../../../FlsQspiResetCallout)"!]
                    [!WS "8"!]&[!"normalize-space(text:join(//FlsConfigSet/FlsQspiResetCallout))"!],                    /* resetCallout */
                [!ELSE!]
                    [!WS "8"!]NULL_PTR,                               /* resetCallout */
                [!ENDIF!]
                [!IF "node:exists(../../../FlsQspiErrorCheckCallout)"!]
                    [!WS "8"!]&[!"normalize-space(text:join(//FlsConfigSet/FlsQspiErrorCheckCallout))"!],                   /* errorCheckCallout */
                [!ELSE!]
                    [!WS "8"!]NULL_PTR,                              /* errorCheckCallout */
                [!ENDIF!]
                [!IF "node:exists(../../../FlsQspiEccCheckCallout)"!]
                    [!WS "8"!]&[!"normalize-space(text:join(//FlsConfigSet/FlsQspiEccCheckCallout))"!],                    /* eccCheckCallout */
                [!ELSE!]
                    [!WS "8"!]NULL_PTR,                               /* eccCheckCallout */
                [!ENDIF!]
                [!IF "node:refvalid(./ctrlAutoCfgPtr)"!]
                    [!WS "8"!]&([!"$configName"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_paQspiUnitCfg[[!"node:pos(node:ref(./ctrlAutoCfgPtr))"!]U])  /* ctrlAutoCfgPtr */
                [!ELSE!]
                    [!WS "8"!]NULL_PTR  /* ctrlAutoCfgPtr */
                [!ENDIF!]
            [!WS "4"!]}[!IF "$FlsLoopIt != 0"!],[!ENDIF!][!CR!]
        [!ENDLOOP!][!//
    };
[!ENDIF!][!//

#define FLS_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Fls_MemMap.h"

[!ENDLOOP!][!//
[!ENDINDENT!][!//
[!ENDCODE!][!//

#ifdef __cplusplus
}
#endif

/** @}*/
