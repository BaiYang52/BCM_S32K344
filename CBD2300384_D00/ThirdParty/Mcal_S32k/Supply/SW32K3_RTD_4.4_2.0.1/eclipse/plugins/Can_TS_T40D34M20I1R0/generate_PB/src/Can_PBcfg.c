/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : FLEXCAN
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
/*==================================================================================================
==================================================================================================*/

/**
*   @file    Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!].c
*   @version 2.0.1
*
*   @brief   AUTOSAR Can - module interface
*   @details Configuration Structures for PostBuild
*
*   @addtogroup CAN_DRIVER
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/**
* @page misra_violations MISRA-C:2012 violations
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.1, External identifiers shall be distinct.
* The used compilers use more than 31 chars for identifiers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.2, Identifiers declared in the same scope and name space shall be distinct.
* The used compilers use more than 31 chars for identifiers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.4, Macro identifiers shall be distinct.
* The used compilers use more than 31 chars for identifiers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.5, Identifiers shall be distinct from macro names.
* The used compilers use more than 31 chars for identifiers.
*
* @section Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_1
* Violates MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents
* of a header file being included more than once
* This violation is not fixed since the inclusion of <MA>_MemMap.h is as per AUTOSAR requirement [SWS_MemMap_00003].
*
* @section Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_2
* Violates MISRA 2012 Advisory Rule 20.1, #include directives should only be preceded by preprocessor directives or comments.
* AUTOSAR imposes the specification of the sections in which certain parts of the driver must be placed.
*
* @section Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_3
* Violates MISRA 2012 Advisory Rule 8.7, Function and objects should not be defined with external linkage if they
* are referenced in only one translation unit.
* This constant variable will be called by higher layer.
*
* @section Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_4
* Violates MISRA 2012 Advisory Rule 8.9, An object should be defined at block scope if identifier only appears in a single function.
* These objects are used when Precompile is supported.
*
* @section Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_5
* Violates MISRA 2012 Required Rule 8.4, A compatible declaration shall be visible when an object or function with external linkage is defined.
*
*/

/*
    (CAN078) The code file structure shall not be defined within this specification completely.
            At this point it shall be pointed out that the code-file structure shall include the following file named: Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!].c.
            This file shall contain all post-build time configurable parameters.
    (CAN221) VariantPB: (Mix of precompile and Post Build multiple selectable configurable configurations.
*/
/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Can.h"
#include "Can_Ipw_Types.h"
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "postBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!]
[!ELSE!]
    [!VAR "postBuildVariantNameUnderscore" = "string(null)"!]
[!ENDIF!]
[!ENDNOCODE!]
[!AUTOSPACING!]
[!INCLUDE "Can_VersionCheck_Src_PB.m"!]
[!INCLUDE "Can_PluginMacro.m"!]
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/*
* @file           Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!].c
*/
#define CAN_VENDOR_ID_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C                      43
#define CAN_AR_RELEASE_MAJOR_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C       4
#define CAN_AR_RELEASE_MINOR_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C       4
#define CAN_AR_RELEASE_REVISION_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C    0
#define CAN_SW_MAJOR_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C               2
#define CAN_SW_MINOR_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C               0
#define CAN_SW_PATCH_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C               1
/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and Can.h are of the same vendor */
#if (CAN_VENDOR_ID_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C != CAN_VENDOR_ID)
    #error "Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!].c and Can.h have different vendor ids"
#endif
/* Check if current file and Can.h are of the same Autosar version */
#if ((CAN_AR_RELEASE_MAJOR_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C != CAN_AR_RELEASE_MAJOR_VERSION) || \
    (CAN_AR_RELEASE_MINOR_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C != CAN_AR_RELEASE_MINOR_VERSION) || \
    (CAN_AR_RELEASE_REVISION_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C != CAN_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!].c and Can.h are different"
#endif
/* Check if current file and Can.h are of the same Software version */
#if ((CAN_SW_MAJOR_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C != CAN_SW_MAJOR_VERSION) || \
    (CAN_SW_MINOR_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C != CAN_SW_MINOR_VERSION) || \
    (CAN_SW_PATCH_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C != CAN_SW_PATCH_VERSION))
    #error "Software Version Numbers of Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!].c and Can.h are different"
#endif
/* Check if current file and Can_Ipw_Types.h are of the same vendor */
#if (CAN_VENDOR_ID_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C != CAN_IPW_TYPES_VENDOR_ID_H)
    #error "Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!].c and Can_Ipw_Types.h have different vendor ids"
#endif
/* Check if current file and Can_Ipw_Types.h are of the same Autosar version */
#if ((CAN_AR_RELEASE_MAJOR_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C != CAN_IPW_TYPES_AR_RELEASE_MAJOR_VERSION_H) || \
    (CAN_AR_RELEASE_MINOR_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C != CAN_IPW_TYPES_AR_RELEASE_MINOR_VERSION_H) || \
    (CAN_AR_RELEASE_REVISION_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C != CAN_IPW_TYPES_AR_RELEASE_REVISION_VERSION_H))
    #error "AutoSar Version Numbers of Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!].c and Can_Ipw_Types.h are different"
#endif
/* Check if current file and Can_Ipw_Types.h are of the same Software version */
#if ((CAN_SW_MAJOR_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C != CAN_IPW_TYPES_SW_MAJOR_VERSION_H) || \
    (CAN_SW_MINOR_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C != CAN_IPW_TYPES_SW_MINOR_VERSION_H) || \
    (CAN_SW_PATCH_VERSION_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_C != CAN_IPW_TYPES_SW_PATCH_VERSION_H))
    #error "Software Version Numbers of Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!].c and Can_Ipw_Types.h are different"
#endif
/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
#if (CAN_FEATURE_HAS_DMA_ENABLE == STD_ON)
    /* CAN_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE */
    #define CAN_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
    #include "Can_MemMap.h"

    [!SELECT "CanConfigSet"!]
        [!LOOP "node:order(CanController/*,'node:value(CanControllerId)')"!]
            [!IF "node:exists(CanRxFiFo) and CanRxFiFo = 'CanEnhanceFiFo' and CanRxFiFo/CanEnhanceFiFoDmaEnable = 'true' and CanControllerActivation = 'true'"!]
                    /* DMA destination address */
                    VAR_ALIGN(static Flexcan_Ip_MsgBuffType g_DmaDestinationAddr[!"CanControllerId"!][[!"CanRxFiFo/NumberMBTransferDMA"!]], 32)
            [!ENDIF!]
            [!IF "node:exists(CanRxFiFo) and CanRxFiFo = 'CanLegacyFiFo' and CanRxFiFo/CanLegacyFiFoDmaEnable = 'true' and CanControllerActivation = 'true'"!]
                    /* DMA destination address */
                    VAR_ALIGN(static Flexcan_Ip_MsgBuffType g_DmaDestinationAddr[!"CanControllerId"!][1], 32)
            [!ENDIF!]
        [!ENDLOOP!]
    [!ENDSELECT!]

    #define CAN_STOP_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
    #include "Can_MemMap.h"
    /* CAN_STOP_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE */
#endif /* (CAN_FEATURE_HAS_DMA_ENABLE == STD_ON) */
[!NOCODE!]
[!VAR "CanSyncSeg" = "1"!]
/* Nominal Bit Time */
[!VAR "CanPropSegN" = "1"!]
[!VAR "CanPhaseSeg1N" = "1"!]
[!VAR "CanPhaseSeg2N" = "1"!]
[!VAR "CanSJWN" = "1"!]
/* Data Bit Time */
[!VAR "CanPropSegD" = "1"!]
[!VAR "CanPhaseSeg1D" = "1"!]
[!VAR "CanPhaseSeg2D" = "1"!]
[!VAR "CanSJWD" = "1"!]

[!VAR "Can_aHwBufferConfig" = "''"!]
[!VAR "Can_aNumberOfMbUsed" = "''"!]
[!VAR "Can_aCtrlBlockArray" = "''"!][!// Create an array of Controller Configured that contain array of ram block.
[!VAR "Can_aCtrlMaxMbPerBlockArray" = "''"!][!// Create an array of Controller Configured that contain array Max Mbs of ram block.
[!/* Error Log */!]
[!SELECT "CanConfigSet"!]
    [!LOOP "node:order(./CanController/*, './CanControllerId')"!]
        [!VAR "CtrlID" = "num:i(./CanControllerId)"!]
        [!VAR "CtrlhwOffsetIdx" = "num:i(substring-after(./CanHwChannel, '_'))"!]
        [!VAR "MaxLegacyFifoFilterAvailable" = "num:i((ecu:list('Can.CanConfigSet.CanController.NoMB')[position() - 1 = $CtrlhwOffsetIdx] - 6) * 4)"!]
        [!IF "$MaxLegacyFifoFilterAvailable > 128"!]
            [!VAR "MaxLegacyFifoFilterAvailable" = "num:i(128)"!][!// max filters supported by hw is 128
        [!ENDIF!]
        [!VAR "CtrlFiFoFlag" = "0"!]
        [!VAR "HwBufferConfig" = "0"!]
        [!VAR "CtrlNumberOfMbUsed" = "0"!]
        [!VAR "CtrlHighestBlockConfig" = "num:i(0)"!]
        [!VAR "Can_aMaxMbPerBlock" = "'0x00 0x00 0x00 0x00 0x07 0x07 0x07 0x07'"!]
        [!VAR "Can_aMbConfigPerBlock" = "'0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00'"!]
        [!VAR "Can_aCtrlBlockArray" = "concat($Can_aCtrlBlockArray, $Can_aMbConfigPerBlock, '#')"!]
        [!VAR "CtrlFDUsed" = "node:exists(node:ref(./CanControllerDefaultBaudrate)/CanControllerFdBaudrateConfig)"!]
        [!IF "$CtrlFDUsed = 'true' and node:name(./CanRamBlock) = 'CanRamBlockSpecified'"!]
            [!VAR "PayloadLength" = "0"!]
            [!VAR "NumOfMbBlock" = "0"!][!// total number of MBs in blocks 0, 1, 2, 3
            [!FOR "i" = "1" TO "num:i(count(./CanRamBlock/*))"!]
                [!VAR "SubCan_aMaxMbPerBlockBefore" = "substring($Can_aMaxMbPerBlock, 1, num:i(($i - 1) * 5))"!]
                [!VAR "SubCan_aMaxMbPerBlockAfter" = "substring($Can_aMaxMbPerBlock, num:i($i * 5))"!]
                [!VAR "PayloadLength" = "substring-before(substring-after(./CanRamBlock/*[number($i)], '_'), '_')"!]
                [!IF "$PayloadLength = 8"!]
                    [!VAR "MaxMbForBlock" = "32"!]
                [!ELSEIF "$PayloadLength = 16"!]
                    [!VAR "MaxMbForBlock" = "21"!]
                [!ELSEIF "$PayloadLength = 32"!]
                    [!VAR "MaxMbForBlock" = "12"!]
                [!ELSEIF "$PayloadLength = 64"!]
                    [!VAR "MaxMbForBlock" = "7"!]
                [!ENDIF!]
                [!VAR "NumOfMbBlock" = "num:i($NumOfMbBlock + $MaxMbForBlock)"!]
                [!VAR "Can_aMaxMbPerBlock" = "text:join(concat($SubCan_aMaxMbPerBlockBefore, num:inttohex($MaxMbForBlock, 2), $SubCan_aMaxMbPerBlockAfter))"!]
            [!ENDFOR!]
        [!ENDIF!]
        [!LOOP "node:order(../../CanHardwareObject/*[node:ref(./CanControllerRef)/CanControllerId = $CtrlID], './CanObjectId')"!]
            [!// Log Error FIFO
            [!IF "node:exists(node:ref(./CanControllerRef)/CanRxFiFo) and (./CanObjectType = 'RECEIVE') and $CtrlFiFoFlag = 0"!]
                [!VAR "CtrlFiFoFlag" = "1"!]
                [!VAR "HwFilterCount" = "num:i(count(./CanHwFilter/*))"!][!// Count number of Hw filter configured for HwObject
                [!IF "node:name(node:ref(./CanControllerRef)/CanRxFiFo) = 'CanLegacyFiFo'"!][!// Legacy FIFO
                    [!VAR "HwBufferConfig" = "num:i($HwBufferConfig + 1)"!]
                    [!VAR "FIFOAcptMode" = "node:ref(./CanControllerRef)/CanRxFiFo/CanIdAcceptanceMode"!]
                    [!IF "node:value(./CanHwObjectCount) != 6"!]
                        [!ERROR!]
                            Since [!"@name"!] is used for Legacy FIFO, the CanHwObjectCount configured must be 6.
                        [!ENDERROR!]
                    [!ELSEIF "($FIFOAcptMode = 'FORMAT_A')"!]
                        [!VAR "max_HwFilter_Element" = "num:i($MaxLegacyFifoFilterAvailable)"!]
                        [!IF "($HwFilterCount mod 8) != 0 or $HwFilterCount > $max_HwFilter_Element"!]
                            [!ERROR!]
                                Since [!"@name"!] is used [!"$FIFOAcptMode"!] of Legacy FIFO, the CanHwFilter configured must be multiple of 8 and in range [8:[!"$max_HwFilter_Element"!]].
                            [!ENDERROR!]
                        [!ELSE!]
                            [!VAR "CtrlNumberOfMbUsed" = "num:i($CtrlNumberOfMbUsed + 6 + ($HwFilterCount div 4))"!]
                        [!ENDIF!]
                    [!ELSEIF "($FIFOAcptMode = 'FORMAT_B')"!]
                        [!VAR "max_HwFilter_Element" = "num:i($MaxLegacyFifoFilterAvailable * 2)"!]
                        [!IF "($HwFilterCount mod 16) != 0 or $HwFilterCount > $max_HwFilter_Element"!]
                            [!ERROR!]
                                Since [!"@name"!] is used [!"$FIFOAcptMode"!] of Legacy FIFO, the CanHwFilter configured must be multiple of 16 and in range [16:[!"$max_HwFilter_Element"!]]
                            [!ENDERROR!]
                        [!ELSE!]
                            [!VAR "CtrlNumberOfMbUsed" = "num:i($CtrlNumberOfMbUsed + 6 + ($HwFilterCount div 8))"!]
                        [!ENDIF!]
                    [!ELSEIF "($FIFOAcptMode = 'FORMAT_C')"!]
                        [!VAR "max_HwFilter_Element" = "num:i(num:i($MaxLegacyFifoFilterAvailable * 4))"!]
                        [!IF "($HwFilterCount mod 32) != 0 or $HwFilterCount > $max_HwFilter_Element"!]
                            [!ERROR!]
                                Since [!"@name"!] is used [!"$FIFOAcptMode"!] of Legacy FIFO, the CanHwFilter configured must be multiple of 32 and in range [32:[!"$max_HwFilter_Element"!]]
                            [!ENDERROR!]
                        [!ELSE!]
                            [!VAR "CtrlNumberOfMbUsed" = "num:i($CtrlNumberOfMbUsed + 6 + ($HwFilterCount div 16))"!]
                        [!ENDIF!]
                    [!ENDIF!]
                [!ELSEIF "node:name(node:ref(./CanControllerRef)/CanRxFiFo) = 'CanEnhanceFiFo'"!][!// Enhanced FIFO
                    [!IF "node:ref(./CanControllerRef)/CanRxFiFo/CanEnhancedSchemeType != 'MASK_FILTER_SCHEME'"!]
                        [!IF "(($HwFilterCount mod 2) != 0)"!]
                            [!ERROR!]
                                Since [!"@name"!] used as Enhanced FIFO,
                                for Enhance RxFifo Scheme Type is RANGE_FILTER_SCHEME or TWO_FILTER_SCHEME,
                                2 consecutive elements in CanHwFilter are used to configure for 1 enhance RxFiFo filter element
                                need to add/remove one element in CanHwFilter!
                            [!ENDERROR!]
                        [!ENDIF!]
                        [!VAR "HwFilterCount" = "num:i($HwFilterCount div 2)"!]
                    [!ENDIF!]
                    [!IF "(./CanIdType = 'STANDARD') and ((($HwFilterCount mod 2) != 0) or ($HwFilterCount < 2) or ($HwFilterCount > 128))"!]
                        [!ERROR!]
                            Since [!"@name"!] used Enhanced FIFO with STANDARD Message ID, the Enhance FIFO Filter Elements configured must be multiple of 2 and in range [2:128].
                            (for Enhance RxFifo Scheme Type is RANGE_FILTER_SCHEME or TWO_FILTER_SCHEME, 2 consecutive elements in CanHwFilter are used to configure for 1 Enhance FIFO Filter Element,
                            ID filter1 is CanHwFilterCode in the first element and  ID filter2 is CanHwFilterCode in second element).
                        [!ENDERROR!]
                    [!ELSEIF "(./CanIdType != 'STANDARD') and (($HwFilterCount < 1) or ($HwFilterCount > 64))"!]
                        [!ERROR!]
                            Since [!"@name"!] used Enhanced FIFO with EXTENDED/MIXED Message ID hence the Enhance FIFO Filter Elements configured must be in range [1:64]
                            (for Enhance RxFifo Scheme Type is RANGE_FILTER_SCHEME or TWO_FILTER_SCHEME, 2 consecutive elements in CanHwFilter are used to configure for 1 Enhance FIFO Filter Element,
                            ID filter1 is CanHwFilterCode in the first element and  ID filter2 is CanHwFilterCode in second element).
                        [!ENDERROR!]
                    [!ELSEIF "node:value(./CanHwObjectCount) > num:i(ecu:get('Can.CanConfigSet.CanRxFiFoEnhancedDeep'))"!]
                        [!ERROR!]
                            Since [!"@name"!] is used for Enhanced FIFO, the Depth of Enhance FIFO is [!"ecu:get('Can.CanConfigSet.CanRxFiFoEnhancedDeep')"!].
                        [!ENDERROR!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ELSE!]
                [!// Log Error CAN FD
                [!IF "(./CanObjectType = 'RECEIVE') and (node:value(./CanHwObjectCount) > 1)"!]
                    [!ERROR!]
                        Since [!"@name"!] is used for Rx normal buffer, the CanHwObjectCount configured must be 1.
                    [!ENDERROR!]
                [!ELSEIF "$CtrlFDUsed = 'true'"!]
                    [!IF "node:name(node:ref(./CanControllerRef)/CanRamBlock) = 'CanRamBlockUnified'"!][!// Can FD Ram Block Unified
                        [!VAR "HwBufferConfig" = "num:i($HwBufferConfig + ./CanHwObjectCount)"!]
                        [!VAR "CtrlNumberOfMbUsed" = "num:i($CtrlNumberOfMbUsed + ./CanHwObjectCount)"!]
                    [!ELSE!][!// Can FD Ram Block specified
                        [!VAR "CtrlRamBlockIdx" = "substring-after(./CanHwObjectUsesBlock, 'CAN_RAM_BLOCK_')"!]
                        [!VAR "SubCan_aMbConfigPerBlockBefore" = "substring($Can_aMbConfigPerBlock, 1, num:i($CtrlRamBlockIdx * 5))"!]
                        [!VAR "SubCan_aMbConfigPerBlockAfter" = "substring($Can_aMbConfigPerBlock, num:i(($CtrlRamBlockIdx + 1) * 5))"!]
                        [!VAR "MbConfig" = "text:split($Can_aMbConfigPerBlock)[position() - 1 = number($CtrlRamBlockIdx)]"!]
                        [!VAR "MaxMb" = "text:split($Can_aMaxMbPerBlock)[position() - 1 = number($CtrlRamBlockIdx)]"!]
                        [!VAR "MbConfig" = "num:i(num:hextoint($MbConfig) + node:value(./CanHwObjectCount))"!]
                        [!IF "$CtrlRamBlockIdx = 4"!]
                            [!IF "$NumOfMbBlock = 128"!]
                                [!ERROR!]
                                    The Message Buffers reserved for four first block (0, 1, 2, 3) is [!"num:i($NumOfMbBlock)"!], is maximum MBs supported by
                                    [!"node:name(node:ref(CanControllerRef))"!]. To select lower block or remove [!"node:name(.)"!] will be resolved error.
                                [!ENDERROR!]
                            [!ELSEIF "num:i($NumOfMbBlock + $MbConfig) > 128"!]
                                [!ERROR!]
                                    [!"node:name(.)"!] assigned to [!"./CanHwObjectUsesBlock"!] is taken over than the maximum MBs supported by
                                    [!"node:name(node:ref(CanControllerRef))"!]. To reduce CanHwObjectCount value or remove [!"node:name(.)"!] or refer to other Controller will be resolved error.
                                [!ENDERROR!]
                            [!ENDIF!]
                        [!ELSEIF "$CtrlRamBlockIdx = 5"!]
                            [!IF "num:i($NumOfMbBlock  + 7)= 128"!]
                                [!ERROR!]
                                    The Message Buffers reserved for five first block (0, 1, 2, 3, 4) is [!"num:i($NumOfMbBlock  + 7)"!], is maximum MBs supported by
                                    [!"node:name(node:ref(CanControllerRef))"!]. To select lower block or remove [!"node:name(.)"!] will be resolved error.
                                [!ENDERROR!]
                            [!ELSEIF "num:i($NumOfMbBlock  + 7 + $MbConfig) > 128"!]
                                [!ERROR!]
                                    [!"node:name(.)"!] assigned to [!"./CanHwObjectUsesBlock"!] is taken over than the maximum MBs supported by
                                    [!"node:name(node:ref(CanControllerRef))"!]. To reduce CanHwObjectCount value or remove [!"node:name(.)"!] or refer to other Controller will be resolved error.
                                [!ENDERROR!]
                            [!ENDIF!]
                        [!ELSEIF "$CtrlRamBlockIdx = 6"!]
                            [!IF "num:i($NumOfMbBlock  + 2 * 7)= 128"!]
                                [!ERROR!]
                                    The Message Buffers reserved for six first block (0, 1, 2, 3, 4, 5) is [!"num:i($NumOfMbBlock  + 2 * 7)"!], is maximum MBs supported by
                                    [!"node:name(node:ref(CanControllerRef))"!]. To select lower block or remove [!"node:name(.)"!] will be resolved error.
                                [!ENDERROR!]
                            [!ELSEIF "num:i($NumOfMbBlock  + 2 * 7 + $MbConfig) > 128"!]
                                [!ERROR!]
                                    [!"node:name(.)"!] assigned to [!"./CanHwObjectUsesBlock"!] is taken over than the maximum MBs supported by
                                    [!"node:name(node:ref(CanControllerRef))"!]. To reduce CanHwObjectCount value or remove [!"node:name(.)"!] or refer to other Controller will be resolved error.
                                [!ENDERROR!]
                            [!ENDIF!]
                        [!ELSEIF "$CtrlRamBlockIdx = 7"!]
                            [!IF "num:i($NumOfMbBlock  + 3 * 7)= 128"!]
                                [!ERROR!]
                                    The Message Buffers reserved for seven first block (0, 1, 2, 3, 4, 5, 6) is [!"num:i($NumOfMbBlock  + 3 * 7)"!], is maximum MBs supported by
                                    [!"node:name(node:ref(CanControllerRef))"!]. To select lower block or remove [!"node:name(.)"!] will be resolved error.
                                [!ENDERROR!]
                            [!ELSEIF "num:i($NumOfMbBlock  + 3 * 7 + $MbConfig) > 128"!]
                                [!ERROR!]
                                    [!"node:name(.)"!] assigned to [!"./CanHwObjectUsesBlock"!] is taken over than the maximum MBs supported by
                                    [!"node:name(node:ref(CanControllerRef))"!]. To reduce CanHwObjectCount value or remove [!"node:name(.)"!] or refer to other Controller will be resolved error.
                                [!ENDERROR!]
                            [!ENDIF!]
                        [!ELSEIF "$MbConfig > num:hextoint($MaxMb)"!]
                            [!ERROR!]
                                The total of Message Buffer configured for is [!"$MbConfig"!], that is over than the max Message Buffer
                                required [!"num:hextoint($MaxMb)"!] in [!"./CanHwObjectUsesBlock"!]. [!"node:name(.)"!] should be reduce CanHwObjectCount value or
                                assign to other block.
                            [!ENDERROR!]
                        [!ENDIF!]
                        [!IF "$CtrlRamBlockIdx > $CtrlHighestBlockConfig"!][!VAR "CtrlHighestBlockConfig" = "$CtrlRamBlockIdx"!][!ENDIF!]
                        [!VAR "Can_aMbConfigPerBlock" = "text:join(concat($SubCan_aMbConfigPerBlockBefore, num:inttohex($MbConfig, 2), $SubCan_aMbConfigPerBlockAfter))"!]
                        [!VAR "HwBufferConfig" = "num:i($HwBufferConfig + ./CanHwObjectCount)"!]
                    [!ENDIF!]
                [!ELSE!]
                    [!VAR "HwBufferConfig" = "num:i($HwBufferConfig + ./CanHwObjectCount)"!]
                    [!VAR "CtrlNumberOfMbUsed" = "num:i($CtrlNumberOfMbUsed + ./CanHwObjectCount)"!]
                [!ENDIF!]
            [!ENDIF!]
        [!ENDLOOP!]
        [!IF "node:exists(./CanRxFiFo) and $CtrlFiFoFlag = 0"!]
            [!ERROR!]
                Since CanRxFiFo is enabled, there must be at least 1 CanHwObject with CanObjectType like RECEIVE configured for [!"node:name(.)"!].
            [!ENDERROR!]
        [!ENDIF!]
        [!IF "$CtrlFDUsed = 'true' and node:name(./CanRamBlock) = 'CanRamBlockSpecified'"!]
            [!IF "$CtrlHighestBlockConfig = 7"!]
                [!VAR "MbUsed" = "text:split($Can_aMbConfigPerBlock)[position() - 1 = number($CtrlHighestBlockConfig)]"!]
                [!VAR "CtrlNumberOfMbUsed" = "num:i($NumOfMbBlock + 3 * 7 + num:hextoint($MbUsed))"!]
            [!ELSEIF "$CtrlHighestBlockConfig = 6"!]
                [!VAR "MbUsed" = "text:split($Can_aMbConfigPerBlock)[position() - 1 = number($CtrlHighestBlockConfig)]"!]
                [!VAR "CtrlNumberOfMbUsed" = "num:i($NumOfMbBlock + 2 * 7 + num:hextoint($MbUsed))"!]
            [!ELSEIF "$CtrlHighestBlockConfig = 5"!]
                [!VAR "MbUsed" = "text:split($Can_aMbConfigPerBlock)[position() - 1 = number($CtrlHighestBlockConfig)]"!]
                [!VAR "CtrlNumberOfMbUsed" = "num:i($NumOfMbBlock + 7 + num:hextoint($MbUsed))"!]
            [!ELSEIF "$CtrlHighestBlockConfig = 4"!]
                [!VAR "MbUsed" = "text:split($Can_aMbConfigPerBlock)[position() - 1 = number($CtrlHighestBlockConfig)]"!]
                [!VAR "CtrlNumberOfMbUsed" = "num:i($NumOfMbBlock + num:hextoint($MbUsed))"!]
            [!ELSEIF "$CtrlHighestBlockConfig = 3"!]
                [!VAR "MbUsed" = "text:split($Can_aMbConfigPerBlock)[position() - 1 = number($CtrlHighestBlockConfig)]"!]
                [!FOR "i" = "1" TO "$CtrlHighestBlockConfig"!]
                    [!VAR "Temp" = "text:split($Can_aMaxMbPerBlock)[number($i)]"!]
                    [!VAR "CtrlNumberOfMbUsed" = "$CtrlNumberOfMbUsed + num:hextoint($Temp)"!]
                [!ENDFOR!]
                [!VAR "CtrlNumberOfMbUsed" = "num:i($CtrlNumberOfMbUsed + num:hextoint($MbUsed))"!]
            [!ELSEIF "$CtrlHighestBlockConfig = 2"!]
                [!VAR "MbUsed" = "text:split($Can_aMbConfigPerBlock)[position() - 1 = number($CtrlHighestBlockConfig)]"!]
                [!FOR "i" = "1" TO "$CtrlHighestBlockConfig"!]
                    [!VAR "Temp" = "text:split($Can_aMaxMbPerBlock)[number($i)]"!]
                    [!VAR "CtrlNumberOfMbUsed" = "$CtrlNumberOfMbUsed + num:hextoint($Temp)"!]
                [!ENDFOR!]
                [!VAR "CtrlNumberOfMbUsed" = "num:i($CtrlNumberOfMbUsed + num:hextoint($MbUsed))"!]
            [!ELSEIF "$CtrlHighestBlockConfig = 1"!]
                [!VAR "MbUsed" = "text:split($Can_aMbConfigPerBlock)[position() - 1 = number($CtrlHighestBlockConfig)]"!]
                [!FOR "i" = "1" TO "$CtrlHighestBlockConfig"!]
                    [!VAR "Temp" = "text:split($Can_aMaxMbPerBlock)[number($i)]"!]
                    [!VAR "CtrlNumberOfMbUsed" = "$CtrlNumberOfMbUsed + num:hextoint($Temp)"!]
                [!ENDFOR!]
                [!VAR "CtrlNumberOfMbUsed" = "num:i($CtrlNumberOfMbUsed + num:hextoint($MbUsed))"!]
            [!ELSEIF "$CtrlHighestBlockConfig = 0"!]
                [!VAR "MbUsed" = "text:split($Can_aMbConfigPerBlock)[position() - 1 = number($CtrlHighestBlockConfig)]"!]
                [!VAR "CtrlNumberOfMbUsed" = "num:i($CtrlNumberOfMbUsed + num:hextoint($MbUsed))"!]
            [!ENDIF!]
        [!ENDIF!]
        [!/*
            In case, only one HwObject configured for Enhance FIFO and this is not counted as a MB taken control by MCR_MAXMB
            CtrlNumberOfMbUsed is set to be 1 to avoid overwritten MCR register during initialization.
        */!]
        [!IF "$CtrlNumberOfMbUsed = 0"!][!VAR "CtrlNumberOfMbUsed" = "num:i(1)"!][!ENDIF!]
        [!VAR "Can_aHwBufferConfig" = "concat($Can_aHwBufferConfig, $HwBufferConfig, '#')"!]
        [!VAR "Can_aNumberOfMbUsed" = "concat($Can_aNumberOfMbUsed, $CtrlNumberOfMbUsed, '#')"!]
        [!VAR "Can_aCtrlMaxMbPerBlockArray" = "concat($Can_aCtrlMaxMbPerBlockArray, $Can_aMaxMbPerBlock, '#')"!]
    [!ENDLOOP!]
[!ENDSELECT!]
[!ENDNOCODE!]
/*==================================================================================================
*                                   GLOBAL FUNCTION PROTOTYPES
==================================================================================================*/
[!INDENT "0"!]
#define CAN_START_SEC_CODE
/* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_1 MISRA 2012 Required Directive 4.10 */
#include "Can_MemMap.h"
    [!CALL "Can_NotificationHandler"!]
#define CAN_STOP_SEC_CODE
/* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_1 MISRA 2012 Required Directive 4.10 */
/* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_2 MISRA 2012 Advisory Rule 20.1 */
#include "Can_MemMap.h"
/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/
#define CAN_START_SEC_CONFIG_DATA_32
#include "Can_MemMap.h"
[!SELECT "CanConfigSet"!]
    [!LOOP "node:order(./CanController/*, './CanControllerId')"!]
        [!VAR "CtrlID" = "num:i(./CanControllerId)"!]
        [!VAR "CtrHwOjb" = "num:i(substring-after(CanHwChannel, 'FLEXCAN_'))"!]
        [!VAR "CanFDEnable" = "'false'"!]
        [!VAR "CanBlockSpecified" = "'false'"!]
        [!IF "node:exists(node:ref(./CanControllerDefaultBaudrate)/CanControllerFdBaudrateConfig)"!]
            [!VAR "CanFDEnable" = "'true'"!]
        [!ENDIF!]
        [!VAR "PayloadLength" = "8"!]
        [!VAR "HwBufferCount" = "text:split($Can_aHwBufferConfig, '#')[position() - 1 = $CtrlID]"!]
        [!VAR "Can_aControllerHwBufferAddr" = "'0x0080 0x0080 0x0080 0x0080 0x0080 0x0080 0x0080 0x0080'"!]
        [!VAR "SubCan_aControllerHwBufferAddrBefore" = "substring($Can_aControllerHwBufferAddr, 1, num:i($CtrlID * 7))"!]
        [!VAR "SubCan_aControllerHwBufferAddrAfter" = "substring($Can_aControllerHwBufferAddr, num:i(($CtrlID + 1) * 7))"!]
        [!VAR "Can_aRamBlockAddr" = "'0x0080 0x0280 0x0480 0x0680 0x1000 0x1200 0x1400 0x1600'"!][!// Specify the Address start of each block
        [!VAR "Can_aRamBlockAddrUpdate" = "'0x0080 0x0280 0x0480 0x0680 0x1000 0x1200 0x1400 0x1600'"!][!// Get Address of buffer in each block and update the new one then.
        [!IF "$HwBufferCount > 0"!]
            /**
            *   @brief  Hardware Buffer Address of [!"@name"!]
            */
            static const uint32 Can_au32HwBufferAddr_Ctrl[!"./CanControllerId"!][[!"num:i($HwBufferCount)"!]U]=
            {
                [!INDENT "4"!]
                [!VAR "HwBufferIdx" = "0"!]
                [!VAR "CtrlFiFoFlag" = "0"!]
                [!VAR "CtrlRamBlockIdx" = "0"!]
                [!VAR "CtrlhwOffsetIdx" = "num:i(substring-after(./CanHwChannel, '_'))"!]
                [!VAR "CtrlRamSizeMax" = "num:i(text:split(ecu:get('Can.CanConfigSet.CanController.NoMB'), ',')[position() - 1 = $CtrlhwOffsetIdx] * 16 + num:hextoint('0x80'))"!]
                [!VAR "CtrlExpandedRamSizeMax" = "num:i(num:hextoint('0x1000') + num:hextoint('0x200') * 4)"!][!// Expended Message Buffer size
                [!LOOP "node:order(../../CanHardwareObject/*[node:ref(./CanControllerRef)/CanControllerId = $CtrlID], './CanObjectId')"!]
                    [!VAR "ObjectFilterCount" = "num:i(count(./CanHwFilter/*))"!]
                    [!IF "$CanFDEnable = 'false'"!][!// CAN Classic
                        [!VAR "CtrlMbAddr" = "text:split($Can_aControllerHwBufferAddr)[position() - 1 = $CtrlID]"!]
                        [!IF "node:exists(node:ref(./CanControllerRef)/CanRxFiFo) and ($CtrlFiFoFlag = 0)"!]
                            [!VAR "CtrlFiFoFlag" = "1"!]
                            [!IF "node:name(node:ref(./CanControllerRef)/CanRxFiFo) = 'CanLegacyFiFo'"!]
                                [!VAR "HwBufferIdx" = "num:i($HwBufferIdx + 1)"!]
                                ([!"node:ref(./CanControllerRef)/CanHwChannel"!]_BASE + (uint32)[!"$CtrlMbAddr"!]U)[!IF "$HwBufferIdx < $HwBufferCount"!],[!ENDIF!][!CR!]
                                [!IF "node:ref(./CanControllerRef)/CanRxFiFo/CanIdAcceptanceMode = 'FORMAT_A'"!]
                                    [!VAR "ObjectFilterCount" = "$ObjectFilterCount"!]
                                [!ELSEIF "node:ref(./CanControllerRef)/CanRxFiFo/CanIdAcceptanceMode = 'FORMAT_B'"!]
                                    [!VAR "ObjectFilterCount" = "num:i($ObjectFilterCount div 2)"!]
                                [!ELSEIF "node:ref(./CanControllerRef)/CanRxFiFo/CanIdAcceptanceMode = 'FORMAT_C'"!]
                                    [!VAR "ObjectFilterCount" = "num:i($ObjectFilterCount div 4)"!]
                                [!ENDIF!]
                                [!VAR "Can_aControllerHwBufferAddr" = "text:join(concat($SubCan_aControllerHwBufferAddrBefore, num:inttohex(num:hextoint($CtrlMbAddr) + 6 * (8 + $PayloadLength) + 4 * $ObjectFilterCount, 4), $SubCan_aControllerHwBufferAddrAfter))"!]
                            [!ENDIF!]
                        [!ELSE!]
                            [!FOR "i" = "1" TO "node:value(./CanHwObjectCount)"!]
                                [!VAR "HwBufferIdx" = "num:i($HwBufferIdx + 1)"!]
                                ([!"node:ref(./CanControllerRef)/CanHwChannel"!]_BASE + (uint32)[!"$CtrlMbAddr"!]U)[!IF "$HwBufferIdx < $HwBufferCount"!],[!ENDIF!][!CR!]
                                [!IF "$CtrlRamSizeMax < num:i(num:hextoint($CtrlMbAddr) + 8 + $PayloadLength)"!]
                                    [!ERROR!]
                                        [!"node:name(.)"!] with payload length ([!"num:i($PayloadLength)"!]) refered to [!"node:name(node:ref(./CanControllerRef))"!] is out of
                                        the Max Message Buffer required. To reduce the value of CanHwObjectCount or assign Object to another Controller will be resolved error.
                                    [!ENDERROR!]
                                [!ENDIF!]
                                [!VAR "CtrlMbAddr" = "num:inttohex(num:hextoint($CtrlMbAddr) + 8 + $PayloadLength, 4)"!]
                            [!ENDFOR!]
                            [!VAR "Can_aControllerHwBufferAddr" = "text:join(concat($SubCan_aControllerHwBufferAddrBefore, $CtrlMbAddr, $SubCan_aControllerHwBufferAddrAfter))"!]
                        [!ENDIF!]
                    [!ELSE!][!// CAN FD
                        [!IF "node:exists(node:ref(./CanControllerRef)/CanRxFiFo) and ($CtrlFiFoFlag = 0)"!][!// Object used for Enhance FIFO, that isn't palce in Mb ram
                            [!VAR "CtrlFiFoFlag" = "1"!]
                        [!ELSE!]
                            [!IF "node:name(node:ref(./CanControllerRef)/CanRamBlock) = 'CanRamBlockUnified'"!]
                                [!VAR "PayloadLength" = "substring-before(substring-after(node:ref(./CanControllerRef)/CanRamBlock/CanBlockUnifiedPayloadLength,'_'),'_')"!]
                                [!FOR "i" = "1" TO "node:value(./CanHwObjectCount)"!]
                                    [!VAR "CtrlMbAddr" = "text:split($Can_aControllerHwBufferAddr)[position() - 1 = $CtrlID]"!]
                                    [!VAR "CtrlBlockAddrCurr" = "text:split($Can_aRamBlockAddr)[position() - 1 = number($CtrlRamBlockIdx)]"!][!// Address Start of current block
                                    [!VAR "CanBlockSizeMax" = "num:hextoint('0x200') + num:hextoint($CtrlBlockAddrCurr)"!][!// size of each block
                                    [!VAR "HwBufferIdx" = "num:i($HwBufferIdx + 1)"!]
                                    ([!"node:ref(./CanControllerRef)/CanHwChannel"!]_BASE + (uint32)[!"$CtrlMbAddr"!]U)[!IF "$HwBufferIdx < $HwBufferCount"!],[!ENDIF!][!CR!]
                                    [!IF "$PayloadLength = 8"!][!// Payload length is 8
                                        [!IF "$CtrlRamSizeMax < num:i(num:hextoint($CtrlMbAddr) + 8 + $PayloadLength)"!]
                                            [!ERROR!]
                                                [!"node:name(.)"!] with payload length ([!"num:i($PayloadLength)"!]) refered to [!"node:name(node:ref(./CanControllerRef))"!] is out of
                                                the Max Message Buffer required. To reduce the value of CanHwObjectCount or assign [!"node:name(.)"!] to another Controller will be resolved error.
                                            [!ENDERROR!]
                                        [!ENDIF!]
                                        [!VAR "CtrlMbAddr" = "num:inttohex(num:hextoint($CtrlMbAddr) + 8 + $PayloadLength, 4)"!]
                                    [!ELSE!][!// Payload length over than 8, there remaining spaces that does not fit for a Mb. So, the spaces will be ignored and move Mb to next block.
                                        [!IF "(node:ref(./CanControllerRef)/CanHwChannel = text:split(ecu:get('Can.CanConfigSet.CanExpandedMBMemoryList'), ',')[position() - 1 = $CtrHwOjb]) and $PayloadLength = 64"!]
                                            [!IF "$CtrlExpandedRamSizeMax < num:i(num:hextoint($CtrlMbAddr) + 8 + $PayloadLength)"!]
                                                [!ERROR!]
                                                    [!"node:name(.)"!] with payload length ([!"num:i($PayloadLength)"!]) refered to [!"node:name(node:ref(./CanControllerRef))"!] is out of
                                                    the Max Message Buffer required. To reduce the value of CanHwObjectCount or assign [!"node:name(.)"!] to another Controller will be resolved error.
                                                [!ENDERROR!]
                                            [!ENDIF!]
                                        [!ELSE!][!// Expanded Mb Memory unsupported
                                            [!IF "$CtrlRamSizeMax < num:i(num:hextoint($CtrlMbAddr) + 8 + $PayloadLength)"!]
                                                [!ERROR!]
                                                    [!"node:name(.)"!] with payload length ([!"num:i($PayloadLength)"!]) refered to [!"node:name(node:ref(./CanControllerRef))"!] is out of
                                                    the Max Message Buffer required. To reduce the value of CanHwObjectCount or assign [!"node:name(.)"!] to another Controller will be resolved error.
                                                [!ENDERROR!]
                                            [!ENDIF!]
                                        [!ENDIF!]
                                        [!IF "$CanBlockSizeMax < num:i(num:hextoint($CtrlMbAddr) + 2 * (8 + $PayloadLength))"!]
                                            [!VAR "CtrlRamBlockIdx" = "num:i($CtrlRamBlockIdx + 1)"!]
                                            [!VAR "CtrlMbAddr" = "text:split($Can_aRamBlockAddr)[position() - 1 = number($CtrlRamBlockIdx)]"!]
                                        [!ELSE!]
                                            [!VAR "CtrlMbAddr" = "num:inttohex(num:hextoint($CtrlMbAddr) + 8 + $PayloadLength, 4)"!]
                                        [!ENDIF!]
                                    [!ENDIF!]
                                    [!VAR "Can_aControllerHwBufferAddr" = "text:join(concat($SubCan_aControllerHwBufferAddrBefore, $CtrlMbAddr, $SubCan_aControllerHwBufferAddrAfter))"!]
                                [!ENDFOR!]
                            [!ELSE!][!// Ram block specified
                                [!IF "./CanHwObjectUsesBlock = 'CAN_RAM_BLOCK_0'"!]
                                    [!VAR "CtrlRamBlockIdx" = "0"!]
                                    [!VAR "PayloadLength" = "substring-before(substring-after(node:ref(./CanControllerRef)/CanRamBlock/CanBlock0PayloadLength,'_'),'_')"!]
                                [!ELSEIF "./CanHwObjectUsesBlock = 'CAN_RAM_BLOCK_1'"!]
                                    [!VAR "CtrlRamBlockIdx" = "1"!]
                                    [!VAR "PayloadLength" = "substring-before(substring-after(node:ref(./CanControllerRef)/CanRamBlock/CanBlock1PayloadLength,'_'),'_')"!]
                                [!ELSEIF "./CanHwObjectUsesBlock = 'CAN_RAM_BLOCK_2'"!]
                                    [!VAR "CtrlRamBlockIdx" = "2"!]
                                    [!VAR "PayloadLength" = "substring-before(substring-after(node:ref(./CanControllerRef)/CanRamBlock/CanBlock2PayloadLength,'_'),'_')"!]
                                [!ELSEIF "./CanHwObjectUsesBlock = 'CAN_RAM_BLOCK_3'"!]
                                    [!VAR "CtrlRamBlockIdx" = "3"!]
                                    [!VAR "PayloadLength" = "substring-before(substring-after(node:ref(./CanControllerRef)/CanRamBlock/CanBlock3PayloadLength,'_'),'_')"!]
                                [!ELSE!]
                                    [!VAR "CtrlRamBlockIdx" = "substring-after(./CanHwObjectUsesBlock, 'CAN_RAM_BLOCK_')"!]
                                    [!VAR "PayloadLength" = "64"!]
                                [!ENDIF!]
                                [!VAR "CtrlBlockAddrCurr" = "text:split($Can_aRamBlockAddr)[position() - 1 = number($CtrlRamBlockIdx)]"!][!// Address Start of current block
                                [!VAR "CanBlockSizeMax" = "num:hextoint('0x200') + num:hextoint($CtrlBlockAddrCurr)"!][!// size of each block
                                [!VAR "SubCan_aRamBlockAddrUpdateBefore" = "substring($Can_aRamBlockAddrUpdate, 1, num:i($CtrlRamBlockIdx * 7))"!]
                                [!VAR "SubCan_aRamBlockAddrUpdateAfter" = "substring($Can_aRamBlockAddrUpdate, num:i(($CtrlRamBlockIdx + 1) * 7))"!]
                                [!FOR "i" = "1" TO "node:value(./CanHwObjectCount)"!]
                                    [!VAR "CtrlMbAddr" = "text:split($Can_aRamBlockAddrUpdate)[position() - 1 = number($CtrlRamBlockIdx)]"!]
                                    [!IF "$CanBlockSizeMax < num:i(num:hextoint($CtrlMbAddr) + 8 + $PayloadLength)"!]
                                        [!ERROR!]
                                        The total of Message Buffer configured for [!"node:name(.)"!] is [!"node:value(./CanHwObjectCount)"!], that is over than the max Message Buffer
                                        required [!"num:i(512 div ($PayloadLength + 8))"!] in [!"./CanHwObjectUsesBlock"!]. [!"node:name(.)"!] should be reduce CanHwObjectCount value or
                                        assign to other block.
                                        [!ENDERROR!]
                                    [!ENDIF!]
                                    [!VAR "HwBufferIdx" = "num:i($HwBufferIdx + 1)"!]
                                    ([!"node:ref(./CanControllerRef)/CanHwChannel"!]_BASE + (uint32)[!"$CtrlMbAddr"!]U)[!IF "$HwBufferIdx < $HwBufferCount"!],[!ENDIF!][!CR!]
                                    [!VAR "CtrlMbAddr" = "num:inttohex(num:hextoint($CtrlMbAddr) + 8 + $PayloadLength, 4)"!]
                                    [!VAR "Can_aRamBlockAddrUpdate" = "text:join(concat($SubCan_aRamBlockAddrUpdateBefore, $CtrlMbAddr, $SubCan_aRamBlockAddrUpdateAfter))"!]
                                [!ENDFOR!]
                            [!ENDIF!]
                        [!ENDIF!]
                    [!ENDIF!]
                [!ENDLOOP!]
                [!ENDINDENT!]
            };
            [!CR!]
        [!ENDIF!]
    [!ENDLOOP!]

    [!VAR "Can_aFlagFIFO" = "'0 0 0 0 0 0 0 0'"!]
    [!LOOP "node:order(./CanHardwareObject/*[./CanObjectType = 'RECEIVE'], './CanObjectId')"!]
        /**
        *   @brief  Hardware Filter of [!"@name"!]
        */
        [!VAR "CtrlID" = "node:ref(CanControllerRef)/CanControllerId"!]
        [!VAR "CtrlFiFoFlag" = "text:split($Can_aFlagFIFO)[position() - 1 = $CtrlID]"!]
        [!IF "node:exists(node:ref(./CanControllerRef)/CanRxFiFo) and ($CtrlFiFoFlag = 0)"!]
            [!VAR "CtrlFiFoFlag" = "1"!]
            [!IF "node:name(node:ref(./CanControllerRef)/CanRxFiFo) = 'CanLegacyFiFo'"!]
                [!VAR "ObjectFilterCount" = "num:i(count(./CanHwFilter/*))"!]
                static const Can_HwFilterType Can_aHwFilter_Object[!"./CanObjectId"!][[!"$ObjectFilterCount"!]U]=
                {
                    [!INDENT "4"!]
                    [!FOR "i" = "1" TO "$ObjectFilterCount"!]
                        [!VAR "CanHwFilterCode" = "node:value(./CanHwFilter/*[number($i)]/CanHwFilterCode)"!]
                        [!VAR "CanHwFilterMask" = "node:value(./CanHwFilter/*[number($i)]/CanHwFilterMask)"!]
                        {
                            [!INDENT "8"!]
                            (uint32)[!"num:inttohex($CanHwFilterCode, 8)"!]U,
                            (uint32)[!"num:inttohex($CanHwFilterMask, 8)"!]U
                            [!ENDINDENT!]
                        }[!IF "num:i($i) < $ObjectFilterCount"!],[!ENDIF!][!CR!]
                    [!ENDFOR!]
                    [!ENDINDENT!]
                };
            [!ELSE!]
                /* Enhance FIFO */
                [!VAR "ObjectFilterCount" = "num:i(count(./CanHwFilter/*))"!]
                [!IF "node:ref(./CanControllerRef)/CanRxFiFo/CanEnhancedSchemeType != 'MASK_FILTER_SCHEME'"!]
                    [!VAR "ObjectFilterCount" = "num:i($ObjectFilterCount div 2)"!]
                [!ENDIF!]
                [!VAR "sChemeType" = "num:hextoint('0x00000000')"!]
                [!IF "node:ref(./CanControllerRef)/CanRxFiFo/CanEnhancedSchemeType = 'RANGE_FILTER_SCHEME'"!]
                    [!VAR "sChemeType" = "num:hextoint('0x40000000')"!][!/* FSCH = b01*/!]
                [!ELSEIF "node:ref(./CanControllerRef)/CanRxFiFo/CanEnhancedSchemeType = 'TWO_FILTER_SCHEME'"!]
                    [!VAR "sChemeType" = "num:hextoint('0x80000000')"!][!/* FSCH = b10*/!]
                [!ENDIF!]
                static const Can_HwFilterType Can_aHwFilter_Object[!"./CanObjectId"!][[!"$ObjectFilterCount"!]U]=
                {
                    [!VAR "TableIdCountLocal" = "0"!]
                    [!VAR "IdType" = "CanIdType"!]
                    [!VAR "hwojb" = "node:ref(./CanControllerRef)/CanHwChannel"!]
                    [!FOR "i" = "0" TO "$ObjectFilterCount - 1"!]
                        [!VAR "TableIdCountLocal" = "$TableIdCountLocal+1"!][!//
                        [!IF "node:ref(./CanControllerRef)/CanRxFiFo/CanEnhancedSchemeType != 'MASK_FILTER_SCHEME'"!]
                            [!VAR "EnhancedFilterID1" = "./CanHwFilter/*[(2 * $i) + 2]/CanHwFilterCode"!]
                            [!VAR "EnhancedFilterID2" = "./CanHwFilter/*[2 * $i + 1]/CanHwFilterCode"!]
                            [!IF "(node:ref(./CanControllerRef)/CanRxFiFo/CanEnhancedSchemeType = 'RANGE_FILTER_SCHEME') and ($EnhancedFilterID2 > $EnhancedFilterID1)"!]
                                [!VAR "EnhancedFilterID1" = "./CanHwFilter/*[2 * $i + 1]/CanHwFilterCode"!]
                                [!VAR "EnhancedFilterID2" = "./CanHwFilter/*[(2 * $i) + 2]/CanHwFilterCode"!]
                            [!ENDIF!]
                        [!ELSE!]
                            [!VAR "EnhancedFilterID1" = "./CanHwFilter/*[$i + 1]/CanHwFilterCode"!]
                            [!VAR "EnhancedFilterID2" = "./CanHwFilter/*[$i + 1]/CanHwFilterMask"!]
                        [!ENDIF!]
                        [!VAR "EnhancedFilterID1" = "bit:or($EnhancedFilterID1, $sChemeType)"!]
                        [!INDENT "4"!]
                        /* [!"../@name"!]: [!"$IdType"!] filter for [!"$hwojb"!] with scheme type [!"node:ref(./CanControllerRef)/CanRxFiFo/CanEnhancedSchemeType"!]*/
                        {
                            [!INDENT "8"!]
                            (uint32)[!"num:inttohex($EnhancedFilterID1, 8)"!]U,
                            (uint32)[!"num:inttohex($EnhancedFilterID2, 8)"!]U
                            [!ENDINDENT!]
                        }[!IF "$ObjectFilterCount != $TableIdCountLocal"!],[!ENDIF!][!CR!]
                        [!ENDINDENT!]
                    [!ENDFOR!][!//
                };
            [!ENDIF!]
            [!VAR "SubCan_aFlagFIFOBefore" = "substring($Can_aFlagFIFO, 1, num:i($CtrlID * 2))"!]
            [!VAR "SubCan_aFlagFIFOAfter" = "substring($Can_aFlagFIFO, num:i(($CtrlID + 1) * 2))"!]
            [!VAR "Can_aFlagFIFO" = "text:join(concat($SubCan_aFlagFIFOBefore, num:i($CtrlFiFoFlag) , $SubCan_aFlagFIFOAfter))"!][!// Set FIFO flag to avoid generate twice
        [!ELSE!]
            static const Can_HwFilterType Can_aHwFilter_Object[!"./CanObjectId"!]=
            {
                [!VAR "CanHwFilterCode" = "node:value(./CanHwFilter/*[number(1)]/CanHwFilterCode)"!]
                [!VAR "CanHwFilterMask" = "num:hextoint('0xC0000000')"!][!// Comparation of both Rx mailbox filter's IDE and RTR bit with their corresponding bits
                [!IF "./CanIdType != 'STANDARD'"!]
                    [!VAR "CanHwFilterMask" = "$CanHwFilterMask + bit:shl(node:value(./CanHwFilter/*[number(1)]/CanHwFilterMask), 0)"!]
                [!ELSE!]
                    [!VAR "CanHwFilterMask" = "$CanHwFilterMask + bit:shl(node:value(./CanHwFilter/*[number(1)]/CanHwFilterMask), 18)"!]
                [!ENDIF!]
                [!INDENT "4"!]
                (uint32)[!"num:inttohex($CanHwFilterCode, 8)"!]U,
                (uint32)[!"num:inttohex($CanHwFilterMask, 8)"!]U
                [!ENDINDENT!]
            };
        [!ENDIF!]
    [!ENDLOOP!]

[!ENDSELECT!]
#define CAN_STOP_SEC_CONFIG_DATA_32
#include "Can_MemMap.h"
#define CAN_START_SEC_CONFIG_DATA_8
#include "Can_MemMap.h"
[!SELECT "CanConfigSet"!]
    /**
    *   @brief  Mapping Controller ID to Controller hardware offset
    */
    static const uint8 Can_aCtrlOffsetToCtrlIDMap[CAN_HWCONTROLLER_SUPPORT]=
    {
        [!INDENT "4"!]
        [!VAR "CtrlHwSupport" = "num:i(ecu:get('Can.CanConfigSet.CanController'))"!]
        [!FOR "CtrlOffset" = "0" TO "$CtrlHwSupport - 1"!]
            [!VAR "CanIDVal" = "'CAN_CONTROLLER_UNUSED'"!]
            [!IF "node:exists(./CanController/*[substring-after(./CanHwChannel, '_') = $CtrlOffset]/CanControllerId)"!]
                [!VAR "CanIDVal" = "./CanController/*[substring-after(./CanHwChannel, '_') = $CtrlOffset]/CanControllerId"!]
            [!ENDIF!]
            (uint8)[!"$CanIDVal"!][!IF "$CtrlOffset < ($CtrlHwSupport - 1)"!],[!ENDIF!][!CR!]
        [!ENDFOR!]
        [!ENDINDENT!]
    };

    /**
    *   @brief  Mapping Controller ID to Hardware Object ID
    */
    static const uint8 Can_aHwObjIDToCtrlIDMap[CAN_HWOBJECT_CONFIG_COUNT]=
    {
        [!INDENT "4"!]
        [!VAR "ObjConfigCount" = "num:i(count(./CanHardwareObject/*))"!]
        [!VAR "ObjIdx" = "0"!]
        [!LOOP "node:order(./CanHardwareObject/*, './CanObjectId')"!]
            [!VAR "ObjIdx" = "num:i($ObjIdx + 1)"!]
            (uint8)[!"num:i(node:ref(CanControllerRef)/CanControllerId)"!]U[!IF "$ObjIdx < $ObjConfigCount"!],[!ENDIF!][!CR!]
        [!ENDLOOP!]
        [!ENDINDENT!]
    };

[!ENDSELECT!]
#define CAN_STOP_SEC_CONFIG_DATA_8
#include "Can_MemMap.h"

#define CAN_START_SEC_CONFIG_DATA_UNSPECIFIED
/* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_1 MISRA 2012 Required Directive 4.10 */
/* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_2 MISRA 2012 Advisory Rule 20.1 */
#include "Can_MemMap.h"

[!SELECT "CanConfigSet"!]
    /**
    *   @brief  Can Hardware Object Config
    */
    [!VAR "Can_aFlagFIFO" = "'0 0 0 0 0 0 0 0'"!]
    [!VAR "Can_aCtrlMbIdx" = "'0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00'"!][!// Get and update Message buffer index in Message Ram
    [!VAR "Can_aCtrlMbAddrIdx" = "'0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00'"!][!// Get and update Address index of Mb in Can_au32HwBufferAddr array
    /* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_5 MISRA 2012 Required Rule 8.4*/
    static const Can_HwObjectConfigType Can_aHwObjectConfig[CAN_HWOBJECT_CONFIG_COUNT]=
    {
        [!INDENT "4"!]
        [!VAR "ObjConfigCount" = "num:i(count(./CanHardwareObject/*))"!]
        [!VAR "ObjIdx" = "0"!]
        [!LOOP "node:order(./CanHardwareObject/*, './CanObjectId')"!]
        /* [!"@name"!] of [!"node:ref(CanControllerRef)/@name"!]*/
        {
            [!INDENT "8"!]
            [!VAR "ObjIdx" = "num:i($ObjIdx + 1)"!]
            [!VAR "CtrlID" = "node:ref(CanControllerRef)/CanControllerId"!]
            [!VAR "CtrlFiFoFlag" = "0"!]
            [!VAR "PayloadLength" = "0"!]
            [!VAR "SubCan_aCtrlMbIdxBefore" = "substring($Can_aCtrlMbIdx, 1, num:i($CtrlID * 5))"!]
            [!VAR "SubCan_aCtrlMbIdxAfter" = "substring($Can_aCtrlMbIdx, num:i(($CtrlID + 1) * 5))"!]
            [!VAR "CtrlMbIdx" = "0"!]
            [!VAR "CtrlMbAddrIdx" = "text:split($Can_aCtrlMbAddrIdx)[position() - 1 = $CtrlID]"!]
            [!VAR "SubCan_aCtrlMbAddrIdxBefore" = "substring($Can_aCtrlMbAddrIdx, 1, num:i($CtrlID * 5))"!]
            [!VAR "SubCan_aCtrlMbAddrIdxAfter" = "substring($Can_aCtrlMbAddrIdx, num:i(($CtrlID + 1) * 5))"!]
            [!IF "node:exists(node:ref(node:ref(./CanControllerRef)/CanControllerDefaultBaudrate)/CanControllerFdBaudrateConfig)"!]
                [!IF "node:name(node:ref(./CanControllerRef)/CanRamBlock) = 'CanRamBlockUnified'"!]
                    [!VAR "PayloadLength" = "substring-before(substring-after(node:ref(./CanControllerRef)/CanRamBlock/CanBlockUnifiedPayloadLength,'_'),'_')"!]
                [!ELSE!]
                    [!IF "./CanHwObjectUsesBlock = 'CAN_RAM_BLOCK_0'"!]
                        [!VAR "PayloadLength" = "substring-before(substring-after(node:ref(./CanControllerRef)/CanRamBlock/CanBlock0PayloadLength,'_'),'_')"!]
                    [!ELSEIF "./CanHwObjectUsesBlock = 'CAN_RAM_BLOCK_1'"!]
                        [!VAR "PayloadLength" = "substring-before(substring-after(node:ref(./CanControllerRef)/CanRamBlock/CanBlock1PayloadLength,'_'),'_')"!]
                    [!ELSEIF "./CanHwObjectUsesBlock = 'CAN_RAM_BLOCK_2'"!]
                        [!VAR "PayloadLength" = "substring-before(substring-after(node:ref(./CanControllerRef)/CanRamBlock/CanBlock2PayloadLength,'_'),'_')"!]
                    [!ELSEIF "./CanHwObjectUsesBlock = 'CAN_RAM_BLOCK_3'"!]
                        [!VAR "PayloadLength" = "substring-before(substring-after(node:ref(./CanControllerRef)/CanRamBlock/CanBlock3PayloadLength,'_'),'_')"!]
                    [!ELSE!]
                        [!VAR "PayloadLength" = "64"!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ELSE!]
                [!VAR "PayloadLength" = "8"!]
            [!ENDIF!]
            /* Hardware Object ID */
            (Can_HwHandleType)[!"./CanObjectId"!]U,
            /* Hardware Object handle type */
            CAN_[!"./CanObjectType"!],
            /* ID Message type */
            CAN_[!"./CanIdType"!],
            /* Object uses polling */
            [!IF "node:exists(./CanHardwareObjectUsesPolling)"!]
                (boolean)[!"text:toupper(./CanHardwareObjectUsesPolling)"!],
            [!ELSE!]
                [!IF "(./CanObjectType = 'RECEIVE' and node:ref(./CanControllerRef)/CanRxProcessing = 'POLLING') or (./CanObjectType = 'TRANSMIT' and node:ref(./CanControllerRef)/CanTxProcessing = 'POLLING')"!]
                    (boolean)TRUE,
                [!ELSE!]
                    (boolean)FALSE,
                [!ENDIF!]
            [!ENDIF!]
            /* Object enable trigger transmit */
            (boolean)[!IF "./CanObjectType = 'TRANSMIT' and node:exists(./CanTriggerTransmitEnable)"!][!"text:toupper(./CanTriggerTransmitEnable)"!][!ELSE!]FALSE[!ENDIF!],[!CR!]
            /* Number of Hw Object used in one Hoh */
            (uint8)[!"./CanHwObjectCount"!]U,
            /* MainFunction RW period reference */
            (uint8)[!IF "node:exists(./CanMainFunctionRWPeriodRef)"!][!"node:name(node:ref(./CanMainFunctionRWPeriodRef))"!][!ELSE!]0xFFU[!ENDIF!],
            /* Data Payload length */
            (uint8)[!"num:i($PayloadLength)"!]U,
            /* Padding value */
            (uint8)[!IF "node:exists(./CanFdPaddingValue)"!][!"num:i(./CanFdPaddingValue)"!]U[!ELSE!]0U[!ENDIF!],
            [!IF "./CanObjectType = 'RECEIVE'"!]
                [!VAR "ObjectFilterCount" = "num:i(count(./CanHwFilter/*))"!]
                [!VAR "CtrlFiFoFlag" = "text:split($Can_aFlagFIFO)[position() - 1 = $CtrlID]"!]
                [!IF "node:exists(node:ref(./CanControllerRef)/CanRxFiFo) and $CtrlFiFoFlag = 0"!]
                    [!IF "node:name(node:ref(./CanControllerRef)/CanRxFiFo) = 'CanLegacyFiFo'"!][!// Legacy FIFO
                        [!IF "node:exists(./CanHardwareObjectUsesPolling)"!]
                            [!IF "./CanHardwareObjectUsesPolling = 'true' and node:ref(./CanControllerRef)/CanRxFiFo/CanLegacyFiFoDmaEnable = 'true'"!]
                                [!ERROR!]
                                    [!"@name"!] referred to [!"node:ref(./CanControllerRef)/@name"!] enabling DMA implied to INTERRUPT,
                                    change this HRH to INTERRUPT or disable DMA to resolve this error.
                                [!ENDERROR!]
                            [!ENDIF!]
                        [!ENDIF!]
                        [!IF "node:ref(./CanControllerRef)/CanRxFiFo/CanIdAcceptanceMode = 'FORMAT_A'"!]
                            [!VAR "ObjectFilterCount" = "$ObjectFilterCount"!]
                        [!ELSEIF "node:ref(./CanControllerRef)/CanRxFiFo/CanIdAcceptanceMode = 'FORMAT_B'"!]
                            [!VAR "ObjectFilterCount" = "num:i($ObjectFilterCount div 2)"!]
                        [!ELSEIF "node:ref(./CanControllerRef)/CanRxFiFo/CanIdAcceptanceMode = 'FORMAT_C'"!]
                            [!VAR "ObjectFilterCount" = "num:i($ObjectFilterCount div 4)"!]
                        [!ENDIF!]
                        [!VAR "Can_aCtrlMbIdx" = "text:join(concat($SubCan_aCtrlMbIdxBefore, num:inttohex(($ObjectFilterCount div 4)+ 6, 2) , $SubCan_aCtrlMbIdxAfter))"!][!// Update the next index of hw buffer
                        [!VAR "Can_aCtrlMbAddrIdx" = "text:join(concat($SubCan_aCtrlMbAddrIdxBefore, num:inttohex(num:hextoint($CtrlMbAddrIdx) + 1, 2) , $SubCan_aCtrlMbAddrIdxAfter))"!][!// Update the next Address index of Mb
                        /* Hardware Filter Count */
                        (uint8)[!"num:i($ObjectFilterCount)"!]U,
                        /* Hw Filter Config */
                        Can_aHwFilter_Object[!"./CanObjectId"!],
                        /* Message Buffer is Legacy FIFO */
                        CAN_RX_LEGACY_FIFO,
                        /* Buffer Index in Message buffer ram */
                        (uint8)0U,
                        /* Message buffer address */
                        &Can_au32HwBufferAddr_Ctrl[!"node:ref(./CanControllerRef)/CanControllerId"!][0U]
                    [!ELSE!][!// Enhance FIFO
                        [!IF "node:exists(./CanHardwareObjectUsesPolling)"!]
                            [!IF "./CanHardwareObjectUsesPolling = 'true' and node:ref(./CanControllerRef)/CanRxFiFo/CanEnhanceFiFoDmaEnable = 'true'"!]
                                [!ERROR!]
                                    [!"@name"!] referred to [!"node:ref(./CanControllerRef)/@name"!] enabling DMA implied to INTERRUPT,
                                    change this HRH to INTERRUPT or disable DMA to resolve this error.
                                [!ENDERROR!]
                            [!ENDIF!]
                        [!ENDIF!]
                        [!IF "node:ref(./CanControllerRef)/CanRxFiFo/CanEnhancedSchemeType != 'MASK_FILTER_SCHEME'"!]
                            [!VAR "ObjectFilterCount" = "num:i($ObjectFilterCount div 2)"!]
                        [!ENDIF!]
                        /* Hardware Filter Count */
                        (uint8)[!"num:i($ObjectFilterCount)"!]U,
                        /* Hw Filter Config */
                        Can_aHwFilter_Object[!"./CanObjectId"!],
                        /* Message Buffer is Enhance FIFO */
                        CAN_RX_ENHANCED_FIFO,
                        /* Buffer Index in Message buffer ram */
                        (uint8)0xFFU,
                        /* Message buffer address */
                        NULL_PTR
                    [!ENDIF!]
                    [!VAR "SubCan_aFlagFIFOBefore" = "substring($Can_aFlagFIFO, 1, num:i($CtrlID * 2))"!]
                    [!VAR "SubCan_aFlagFIFOAfter" = "substring($Can_aFlagFIFO, num:i(($CtrlID + 1) * 2))"!]
                    [!VAR "Can_aFlagFIFO" = "text:join(concat($SubCan_aFlagFIFOBefore, 1 , $SubCan_aFlagFIFOAfter))"!][!// Set FIFO flag to avoid generate twice
                [!ELSE!][!// Receive Normal
                    [!IF "node:exists(./CanHwObjectUsesBlock)"!][!// Using Can FD with Block specified
                        [!VAR "SubCan_aCtrlBlockArrayBefore" = "substring($Can_aCtrlBlockArray, 1, num:i($CtrlID * 8 * 5))"!]
                        [!VAR "SubCan_aCtrlBlockArrayAfter" = "substring($Can_aCtrlBlockArray, num:i(($CtrlID + 1) * 8 * 5))"!]
                        [!VAR "CtrlBlockIdx" = "substring-after(./CanHwObjectUsesBlock, 'CAN_RAM_BLOCK_')"!][!// Get the Block Index that HwObject selected
                        [!VAR "CtrlBlockArr" = "text:split($Can_aCtrlBlockArray, '#')[position() - 1 = $CtrlID]"!][!// Get the array block which belonged to current Controller
                        [!VAR "SubCtrlBlockArrBefore" = "substring($CtrlBlockArr, 1, num:i($CtrlBlockIdx * 5))"!][!// Slice the part value before the Current Block
                        [!VAR "SubCtrlBlockArrAfter" = "substring($CtrlBlockArr, num:i(($CtrlBlockIdx + 1) * 5))"!][!// Slice the part value After the Current Block
                        [!VAR "CtrlMaxMbPerBlockArr" = "text:split($Can_aCtrlMaxMbPerBlockArray, '#')[position() - 1 = $CtrlID]"!][!// Get the array Max Mbs per block which belonged to current Controller
                        [!VAR "MbUsed" = "text:split($CtrlBlockArr)[position() - 1 = $CtrlBlockIdx]"!][!// Get the number of Mbs configured in Block
                        [!IF "$CtrlBlockIdx != 0"!]
                            [!FOR "i" = "1" TO "$CtrlBlockIdx"!]
                                [!VAR "Temp" = "text:split($CtrlMaxMbPerBlockArr)[number($i)]"!]
                                [!VAR "CtrlMbIdx" = "$CtrlMbIdx + num:hextoint($Temp)"!]
                            [!ENDFOR!]
                            [!VAR "CtrlMbIdx" = "$CtrlMbIdx + num:hextoint($MbUsed)"!]
                        [!ELSE!]
                            [!VAR "CtrlMbIdx" = "num:hextoint($MbUsed)"!]
                        [!ENDIF!]
                        [!VAR "CtrlBlockArr" = "text:join(concat($SubCtrlBlockArrBefore, num:inttohex(num:hextoint($MbUsed) + node:value(./CanHwObjectCount), 2) , $SubCtrlBlockArrAfter))"!]
                        [!VAR "Can_aCtrlBlockArray" = "text:join(concat($SubCan_aCtrlBlockArrayBefore, $CtrlBlockArr , $SubCan_aCtrlBlockArrayAfter))"!]
                        [!VAR "CtrlMbIdx" = "num:inttohex($CtrlMbIdx, 2)"!]
                    [!ELSE!][!// Other
                        [!VAR "CtrlMbIdx" = "text:split($Can_aCtrlMbIdx)[position() - 1 = $CtrlID]"!]
                        [!VAR "Can_aCtrlMbIdx" = "text:join(concat($SubCan_aCtrlMbIdxBefore, num:inttohex(num:hextoint($CtrlMbIdx) + node:value(./CanHwObjectCount), 2) , $SubCan_aCtrlMbIdxAfter))"!][!// Update the next index of hw buffer
                    [!ENDIF!]
                    [!VAR "Can_aCtrlMbAddrIdx" = "text:join(concat($SubCan_aCtrlMbAddrIdxBefore, num:inttohex(num:hextoint($CtrlMbAddrIdx) + 1, 2) , $SubCan_aCtrlMbAddrIdxAfter))"!][!// Update the next Address index of Mb
                    /* Hardware Filter Count */
                    (uint8)[!"num:i($ObjectFilterCount)"!]U,
                    /* Hw Filter Config */
                    &Can_aHwFilter_Object[!"./CanObjectId"!],
                    /* Message Buffer is Rx normal */
                    CAN_RX_NORMAL,
                    /* Buffer Index in Message buffer ram */
                    (uint8)[!"num:hextoint($CtrlMbIdx)"!]U,
                    /* Message buffer address */
                    &Can_au32HwBufferAddr_Ctrl[!"node:ref(./CanControllerRef)/CanControllerId"!][[!"num:hextoint($CtrlMbAddrIdx)"!]U]
                [!ENDIF!]
            [!ELSE!][!// Transmit Object
                [!IF "node:exists(./CanHwObjectUsesBlock)"!][!// Using Can FD with Block specified
                    [!VAR "SubCan_aCtrlBlockArrayBefore" = "substring($Can_aCtrlBlockArray, 1, num:i($CtrlID * 8 * 5))"!]
                    [!VAR "SubCan_aCtrlBlockArrayAfter" = "substring($Can_aCtrlBlockArray, num:i(($CtrlID + 1) * 8 * 5))"!]
                    [!VAR "CtrlBlockIdx" = "substring-after(./CanHwObjectUsesBlock, 'CAN_RAM_BLOCK_')"!][!// Get the Block Index that HwObject selected
                    [!VAR "CtrlBlockArr" = "text:split($Can_aCtrlBlockArray, '#')[position() - 1 = $CtrlID]"!][!// Get the array block which belonged to current Controller
                    [!VAR "SubCtrlBlockArrBefore" = "substring($CtrlBlockArr, 1, num:i($CtrlBlockIdx * 5))"!][!// Slice the part value before the Current Block
                    [!VAR "SubCtrlBlockArrAfter" = "substring($CtrlBlockArr, num:i(($CtrlBlockIdx + 1) * 5))"!][!// Slice the part value After the Current Block
                    [!VAR "CtrlMaxMbPerBlockArr" = "text:split($Can_aCtrlMaxMbPerBlockArray, '#')[position() - 1 = $CtrlID]"!][!// Get the array Max Mbs per block which belonged to current Controller
                    [!VAR "MbUsed" = "text:split($CtrlBlockArr)[position() - 1 = $CtrlBlockIdx]"!][!// Get the number of Mbs configured in Block
                    [!IF "$CtrlBlockIdx != 0"!]
                        [!FOR "i" = "1" TO "$CtrlBlockIdx"!]
                            [!VAR "Temp" = "text:split($CtrlMaxMbPerBlockArr)[number($i)]"!]
                            [!VAR "CtrlMbIdx" = "$CtrlMbIdx + num:hextoint($Temp)"!]
                        [!ENDFOR!]
                        [!VAR "CtrlMbIdx" = "$CtrlMbIdx + num:hextoint($MbUsed)"!]
                    [!ELSE!]
                        [!VAR "CtrlMbIdx" = "num:hextoint($MbUsed)"!]
                    [!ENDIF!]
                    [!VAR "CtrlBlockArr" = "text:join(concat($SubCtrlBlockArrBefore, num:inttohex(num:hextoint($MbUsed) + node:value(./CanHwObjectCount), 2) , $SubCtrlBlockArrAfter))"!]
                    [!VAR "Can_aCtrlBlockArray" = "text:join(concat($SubCan_aCtrlBlockArrayBefore, $CtrlBlockArr , $SubCan_aCtrlBlockArrayAfter))"!]
                    [!VAR "CtrlMbIdx" = "num:inttohex($CtrlMbIdx, 2)"!]
                [!ELSE!][!// Other
                    [!VAR "CtrlMbIdx" = "text:split($Can_aCtrlMbIdx)[position() - 1 = $CtrlID]"!]
                    [!VAR "Can_aCtrlMbIdx" = "text:join(concat($SubCan_aCtrlMbIdxBefore, num:inttohex(num:hextoint($CtrlMbIdx) + node:value(./CanHwObjectCount), 2) , $SubCan_aCtrlMbIdxAfter))"!][!// Update the next index of hw buffer
                [!ENDIF!]
                [!VAR "Can_aCtrlMbAddrIdx" = "text:join(concat($SubCan_aCtrlMbAddrIdxBefore, num:inttohex(num:hextoint($CtrlMbAddrIdx) + node:value(./CanHwObjectCount), 2) , $SubCan_aCtrlMbAddrIdxAfter))"!][!// Update the next Address index of Mb
                /* Hardware Filter Count */
                (uint8)0U,
                /* Hw Filter Config */
                NULL_PTR,
                /* Message Buffer is Tx normal */
                CAN_TX_NORMAL,
                /* Buffer Index in Message buffer ram */
                (uint8)[!"num:hextoint($CtrlMbIdx)"!]U,
                /* Message buffer address */
                &Can_au32HwBufferAddr_Ctrl[!"node:ref(./CanControllerRef)/CanControllerId"!][[!"num:hextoint($CtrlMbAddrIdx)"!]U]
            [!ENDIF!]
            [!ENDINDENT!]
        }[!IF "$ObjIdx < $ObjConfigCount"!],[!ENDIF!][!CR!]
        [!ENDLOOP!]
        [!ENDINDENT!]
    };

    [!LOOP "node:order(./CanController/*, './CanControllerId')"!]
        /**
        *   @brief  Can Baudrate Config of [!"@name"!]
        */
        [!VAR "CanClockSrc" = "node:ref(./CanCpuClockRef)/McuClockReferencePointFrequency"!]
        [!IF "node:value(../../../CanGeneral/CanEnableDualClockMode) = 'true'"!]
            [!VAR "CanClockSrcAlternate" = "node:ref(./CanCpuClockRefAlternate)/McuClockReferencePointFrequency"!]
        [!ENDIF!]
        [!VAR "BaudrateConfigCount" = "num:i(count(./CanControllerBaudrateConfig/*))"!]
        static const Can_BaudrateConfigType Can_aBaudrateConfig_Ctrl[!"./CanControllerId"!][[!"$BaudrateConfigCount"!]U]=
        {
            [!INDENT "4"!]
            [!VAR "BaudrateIdx" = "0"!]
            [!LOOP "node:order(./CanControllerBaudrateConfig/*, './CanControllerBaudRateConfigID')"!]
            {
                [!INDENT "8"!]
                [!VAR "TimeQuantum" = "num:i(./CanControllerPrescaller) div $CanClockSrc"!]
                [!VAR "BaudrateIdx" = "num:i($BaudrateIdx + 1)"!]
                /*Can_bEnhanceCBTEnable*/
                (boolean)[!IF "CanBaudrateTypeSuport = 'ENHANCE_CBT'"!]TRUE[!ELSE!]FALSE[!ENDIF!],[!CR!]
                /* Tx Bit Rate Switch */
                (boolean)[!IF "node:exists(./CanControllerFdBaudrateConfig)"!][!"text:toupper(./CanControllerFdBaudrateConfig/CanControllerTxBitRateSwitch)"!][!ELSE!]FALSE[!ENDIF!],[!CR!]
                /* FD enable */
                (boolean)[!"text:toupper(node:exists(./CanControllerFdBaudrateConfig))"!],
                /* Nominal bit rate */
                {
                    [!INDENT "12"!]
                    [!VAR "NumOfTQN" = "($CanClockSrc div (./CanControllerBaudRate * 1000 * ./CanControllerPrescaller))"!][!// Number of Tq in normal bit time.
                    [!IF "node:value(../../../../../CanGeneral/CanEnableDualClockMode) = 'true'"!]
                        [!VAR "NumOfTQNAlternate" = "($CanClockSrcAlternate div (./CanControllerBaudRate * 1000 * ./CanControllerPrescallerAlternate))"!]
                        [!IF "$NumOfTQNAlternate != $NumOfTQN"!]
                            [!WARNING!]
                                The total time quanta configured ([!"$NumOfTQN"!]) with Normal Prescaler is not equal to
                                the Alternate one computed ([!"$NumOfTQNAlternate"!]) with Alternate Prescaler.
                            [!ENDWARNING!]
                        [!ENDIF!]
                    [!ENDIF!]
                    [!IF "./CanAdvancedSetting = 'false'"!]
                        [!IF "$NumOfTQN != num:i(./CanControllerPropSeg + ./CanControllerSeg1 + ./CanControllerSeg2 + ./CanControllerSyncSeg)"!]
                            [!WARNING!]
                                The total time quanta configured ([!"(num:i(./CanControllerPropSeg + ./CanControllerSeg1 + ./CanControllerSeg2 + ./CanControllerSyncSeg))"!]) is not equal to
                                the one computed ([!"$NumOfTQN"!]) from Can Clock, Prescaler and Nominal Baudrate configured
                            [!ENDWARNING!]
                        [!ENDIF!]
                        (uint8)[!"num:i(./CanControllerPropSeg - 1)"!]U,
                        (uint8)[!"num:i(./CanControllerSeg1 - 1)"!]U,
                        (uint8)[!"num:i(./CanControllerSeg2 - 1)"!]U,
                        (uint16)[!"num:i(./CanControllerPrescaller - 1)"!]U,
                    #if (CAN_DUAL_CLOCK_MODE == STD_ON)
                        /* Alternative Baudrate Nominal Prescaler */
                        (uint16)[!"num:i(./CanControllerPrescallerAlternate - 1)"!]U,
                    #endif
                        (uint8)[!"num:i(./CanControllerSyncJumpWidth - 1)"!]U
                    [!ELSE!]
                        [!VAR "BusPropogationDelay" = "5 div 1000000000"!][!// Hard coded to 5ns/m.
                        [!VAR "PhysicalDelayOfBus" = "(./CanBusLength) * $BusPropogationDelay"!]
                        [!VAR "tPropseg" = "2 * ($PhysicalDelayOfBus + (./CanPropDelayTranceiver) div 1000000000)"!]
                        [!VAR "CanPropSegN" = "num:i(ceiling($tPropseg div $TimeQuantum))"!]
                        [!IF "$CanPropSegN >= 1 and $CanPropSegN <= 8"!]
                            [!VAR "CanPhaseSeg" = "num:i($NumOfTQN - $CanPropSegN - $CanSyncSeg)"!]
                            [!IF "$CanPhaseSeg >= 3 and $CanPhaseSeg <= 16"!]
                                [!IF "($CanPhaseSeg > 3) and (($CanPhaseSeg mod 2) = 1) and $CanPropSegN + 1 <= 8"!]
                                    [!VAR "CanPropSegN" = "$CanPropSegN + 1"!]
                                    [!VAR "CanPhaseSeg1N" = "num:i(($CanPhaseSeg -1) div 2)"!]
                                    [!VAR "CanPhaseSeg2N" = "num:i($CanPhaseSeg1N)"!]
                                [!ELSE!]
                                    [!IF "num:i($CanPhaseSeg mod 2) = 0"!]
                                        [!VAR "CanPhaseSeg1N" = "num:i($CanPhaseSeg div 2)"!]
                                        [!VAR "CanPhaseSeg2N" = "num:i($CanPhaseSeg1N)"!]
                                    [!ELSE!]
                                        [!VAR "CanPhaseSeg1N" = "num:i(($CanPhaseSeg - 1) div 2)"!]
                                        [!VAR "CanPhaseSeg2N" = "num:i($CanPhaseSeg1N + 1)"!]
                                    [!ENDIF!]
                                [!ENDIF!]
                                [!IF "$CanPhaseSeg1N < 4"!][!// RJW is the smaller of 4 and PHASE_SEG1.
                                    [!VAR "CanSJWN" = "$CanPhaseSeg1N"!]
                                [!ELSE!]
                                    [!VAR "CanSJWN" = "4"!]
                                [!ENDIF!]
                                [!VAR "baudrate_recalculated" = "$CanClockSrc div ((($CanPropSegN + $CanPhaseSeg1N + $CanPhaseSeg2N + $CanSyncSeg) * num:i(./CanControllerPrescaller)) * 1000)"!][!// /* re-calculate bitrate
                                [!IF "($baudrate_recalculated != ./CanControllerBaudRate)"!]
                                    [!WARNING!]
                                        For [!"./@name"!] of [!"../../@name"!]
                                        Baurate after automatic calculation ([!"$baudrate_recalculated"!]Kbps) is not equal to expected value ([!"./CanControllerBaudRate"!])Kbps,
                                        changing prescler to have an even value of timequanta may resolve it!
                                    [!ENDWARNING!]
                                [!ENDIF!]
                            [!ELSE!]
                                [!ERROR!]
                                    For [!"./@name"!] of [!"../../@name"!]
                                    PHASE_SEG1 + PHASE_SEG2 is ([!"$CanPhaseSeg"!]) and it's out of range [3:16]
                                [!ENDERROR!]
                            [!ENDIF!]
                        [!ELSE!]
                            [!ERROR!]
                                For [!"./@name"!] of [!"../../@name"!]
                                Propagation Segment is ([!"$CanPropSegN"!]) and it's out of range [1:8]

                                The formula used:
                                TimeQuantum = Presscaler div CanClockSrc
                                BusPropagationDelay = 5e-9 s/m
                                PhysicalDelayOfBus = CanBusLength * BusPropagationDelay
                                tPropSeg = 2 * (PhysicalDelayOfBus + CanPropDelayTranceiver div 1000000000)
                                PropSeg = ROUND_UP(tPropSeg div TimeQuantum)
                            [!ENDERROR!]
                        [!ENDIF!]
                        (uint8)[!"num:i($CanPropSegN - 1)"!]U,
                        (uint8)[!"num:i($CanPhaseSeg1N - 1)"!]U,
                        (uint8)[!"num:i($CanPhaseSeg2N - 1)"!]U,
                        (uint16)[!"num:i(./CanControllerPrescaller - 1)"!]U,
                    #if (CAN_DUAL_CLOCK_MODE == STD_ON)
                        /* Alternative Baudrate Nominal Prescaler */
                        (uint16)[!"num:i(./CanControllerPrescallerAlternate - 1)"!]U,
                    #endif
                        (uint8)[!"num:i($CanSJWN - 1)"!]U
                    [!ENDIF!]
                    [!ENDINDENT!]
                },

                /* Data bit rate */
                [!IF "node:exists(./CanControllerFdBaudrateConfig) and (./CanControllerFdBaudrateConfig/CanControllerTxBitRateSwitch = 'true')"!]
                    [!VAR "NumOfTQD" = "($CanClockSrc div (./CanControllerFdBaudrateConfig/CanControllerFdBaudRate * 1000 * ./CanControllerPrescaller))"!]
                    [!SELECT "./CanControllerFdBaudrateConfig"!]
                    {
                        [!IF "node:exists(./CanControllerFdPrescaller)"!]
                            [!VAR "NumOfTQD" = "($CanClockSrc div (./CanControllerFdBaudRate * 1000 * ./CanControllerFdPrescaller))"!]
                        [!ENDIF!]
                        [!IF "$NumOfTQD != num:i(./CanControllerPropSeg + ./CanControllerSeg1 + ./CanControllerSeg2 + ./CanControllerFdSyncSeg)"!]
                            [!WARNING!]
                                The total time quanta configured ([!"(num:i(./CanControllerPropSeg + ./CanControllerSeg1 + ./CanControllerSeg2 + ./CanControllerFdSyncSeg))"!]) is over than
                                the one computed ([!"$NumOfTQD"!]) from Can Clock, Prescaler and Data Baudrate configured
                            [!ENDWARNING!]
                        [!ENDIF!]
                        [!INDENT "12"!]
                        (uint8)[!"num:i(./CanControllerPropSeg)"!]U,
                        (uint8)[!"num:i(./CanControllerSeg1 - 1)"!]U,
                        (uint8)[!"num:i(./CanControllerSeg2 - 1)"!]U,
                        [!IF "node:exists(CanControllerFdPrescaller)"!]
                        (uint16)[!"num:i(node:value(CanControllerFdPrescaller) - 1)"!]U,
                        [!ELSE!]
                        (uint16)[!"num:i(node:value(../CanControllerPrescaller) - 1)"!]U,
                        [!ENDIF!]
                    #if (CAN_DUAL_CLOCK_MODE == STD_ON)
                        /* Alternative Baudrate Data Prescaler */
                        (uint16)[!"num:i(./CanControllerPrescallerAlternateFd - 1)"!]U,
                    #endif
                        (uint8)[!"num:i(./CanControllerSyncJumpWidth - 1)"!]U
                        [!ENDINDENT!]
                    },
                    [!ENDSELECT!]
                [!ELSE!]
                {
                    (uint8)0U,
                    (uint8)0U,
                    (uint8)0U,
                    (uint16)0U,
                #if (CAN_DUAL_CLOCK_MODE == STD_ON)
                    (uint16)0U,
                #endif
                    (uint8)0U
                },
                [!ENDIF!]

                /* Tx Arbitration Start delay */
                (uint8)[!"num:i(./CanTxArbitrationStartDelay)"!]U,
                [!IF "node:exists(./CanControllerFdBaudrateConfig/CanControllerSspOffset)"!]
                    /* Tranceiver Delay Enable */
                    (boolean)TRUE,
                    /* Tranceiver Delay Compensation Offset */
                    (uint8)[!"num:i(./CanControllerFdBaudrateConfig/CanControllerSspOffset)"!]U
                [!ELSE!][!// Disable Tranceiver Delay.
                    (boolean)FALSE,
                    (uint8)0U
                [!ENDIF!]
                [!ENDINDENT!]
            }[!IF "$BaudrateIdx < $BaudrateConfigCount"!],[!ENDIF!][!CR!]
            [!ENDLOOP!]
            [!ENDINDENT!]
        };

        [!VAR "CtrlIdx" = "num:i(./CanControllerId)"!]
        [!VAR "ObjectCount" = "num:i(count(../../CanHardwareObject/*[node:ref(./CanControllerRef)/CanControllerId = $CtrlIdx]))"!]
        [!IF "$ObjectCount > 0"!]
            /**
            *   @brief  HwObject's pointer structure of [!"@name"!]
            */
            static const Can_HwObjectConfigType * const Can_apHwObject_Ctrl[!"./CanControllerId"!][[!"$ObjectCount"!]U]=
            {
                [!INDENT "4"!]
                [!VAR "pIdx" = "0"!]
                [!LOOP "node:order(../../CanHardwareObject/*[node:ref(./CanControllerRef)/CanControllerId = $CtrlIdx], './CanObjectId')"!]
                    [!VAR "pIdx" = "num:i($pIdx + 1)"!]
                    &Can_aHwObjectConfig[[!"num:i(./CanObjectId)"!]U][!IF "$pIdx < $ObjectCount"!],[!ENDIF!][!CR!]
                [!ENDLOOP!]
                [!ENDINDENT!]
            };
        [!ENDIF!]
    [!ENDLOOP!]
    /* Inport external structure defined by IPW */
    CAN_IPW_EXT
    /**
    *   @brief  Can Controller Config
    */
    [!VAR "CtrlConfigCount" = "num:i(count(./CanController/*))"!]
    static const Can_ControllerConfigType Can_aControllerConfig[CAN_CONTROLLER_CONFIG_COUNT]=
    {
        [!INDENT "4"!]
        [!VAR "CtrlIdx" = "num:i(0)"!]
        [!VAR "CanGeneralPart" = "as:path(../CanGeneral, true())"!]
        [!LOOP "node:order(./CanController/*, './CanControllerId')"!]
        {
            [!INDENT "8"!]
            [!VAR "CtrlName" = "./@name"!]
            [!VAR "CanControllerPath" = "as:path(., true())"!]
            [!VAR "AbstCanIfControllerID" = "./CanControllerId"!]
            /* Abstracted CanIf Controller ID */
            [!IF "node:exists(as:modconf('CanIf')[1])"!]
                [!SELECT "as:modconf('CanIf')[1]"!]
                    [!VAR "CanCanIfMatchCounter" = "0"!]
                    [!LOOP "./CanIfCtrlDrvCfg/*"!]
                        [!IF "node:value(./CanIfCtrlDrvNameRef) = $CanGeneralPart"!]
                            [!VAR "CanCanIfMatchCounter" = "$CanCanIfMatchCounter + 1"!]
                            [!VAR "CanIfCtrlDrvCfgRef" = "node:name(.)"!]
                        [!ENDIF!]
                    [!ENDLOOP!]
                    [!IF "$CanCanIfMatchCounter = 1"!]
                        [!VAR "CanIfCtrlCanCtrlMatchCounter" = "0"!]
                        [!LOOP "./CanIfCtrlDrvCfg/*[node:name(.) = $CanIfCtrlDrvCfgRef]/CanIfCtrlCfg/*"!]
                            [!IF "node:value(./CanIfCtrlCanCtrlRef) = $CanControllerPath"!]
                                [!VAR "AbstCanIfControllerID" = "node:value(./CanIfCtrlId)"!]
                                [!VAR "CanIfCtrlCanCtrlMatchCounter" = "$CanIfCtrlCanCtrlMatchCounter + 1"!]
                            [!ENDIF!]
                        [!ENDLOOP!]
                        [!IF "$CanIfCtrlCanCtrlMatchCounter = 0"!]
                            [!WARNING!][!"substring-after($CanControllerPath, 'CanConfigSet/')"!] should be referred by CanIfCtrlCanCtrlRef[!ENDWARNING!]
                        [!ENDIF!]
                    [!ELSE!]
                        [!WARNING!]Can driver was not refered by CanIf [CanIfCtrlDrvNameRef] or it has been refered more than one time.[!ENDWARNING!]
                    [!ENDIF!]
                [!ENDSELECT!]
            [!ELSE!]
                [!WARNING!]User should add CanIf into user's configuration[!ENDWARNING!]
            [!ENDIF!]
            (uint8)[!"num:i($AbstCanIfControllerID)"!]U,
            /* Controller ID */
            (uint8)[!"./CanControllerId"!]U,
            /* Controller Offset */
            (uint8)[!"num:i(text:split(CanHwChannel,'_')[2])"!]U,
            /* Controller Base Address */
            [!"./CanHwChannel"!]_BASE,
            /* Controller Activation */
            (boolean)[!"text:toupper(./CanControllerActivation)"!],
            /* Bus Off uses polling */
            (boolean)[!IF "./CanBusoffProcessing = 'POLLING'"!]TRUE[!ELSE!]FALSE[!ENDIF!],[!CR!]
            [!IF "node:exists(./CanRxFiFo) and node:name(./CanRxFiFo) = 'CanLegacyFiFo'"!]
                [!SELECT "./CanRxFiFo"!]
                    [!VAR "LegacyIDType" = "../../../CanHardwareObject/*[CanHwObjectCount = 6 and text:contains(string(CanControllerRef), $CtrlName) and CanObjectType = 'RECEIVE']/CanIdType"!]
                    [!IF "./CanIdAcceptanceMode = 'FORMAT_A'"!][!// format A
                        [!VAR "RXFGMASK" = "num:hextoint('0xC0000000')"!][!// mask for IDE, RTR
                        [!IF "$LegacyIDType = 'STANDARD'"!][!//
                            [!VAR "temp" = "bit:shl(CanLegacyFIFOGlobalMask0, 19)"!][!//
                            [!VAR "RXFGMASK" = "bit:or($RXFGMASK, $temp)"!][!//
                        [!ELSE!][!//
                            [!VAR "temp" = "bit:shl(CanLegacyFIFOGlobalMask0, 1)"!][!//
                            [!VAR "RXFGMASK" = "bit:or($RXFGMASK, $temp)"!][!//
                        [!ENDIF!][!//
                    [!ELSEIF "./CanIdAcceptanceMode = 'FORMAT_B'"!][!// format B
                        [!VAR "RXFGMASK" = "num:hextoint('0xC000C000')"!][!// mask for IDE, RTR
                        [!IF "$LegacyIDType = 'STANDARD'"!][!//
                            [!VAR "temp" = "bit:shl(CanLegacyFIFOGlobalMask0, 19)"!][!//
                            [!VAR "RXFGMASK" = "bit:or($RXFGMASK, $temp)"!][!//
                            [!VAR "temp" = "bit:shl(CanLegacyFIFOGlobalMask1, 3)"!][!//
                            [!VAR "RXFGMASK" = "bit:or($RXFGMASK, $temp)"!][!//
                        [!ELSE!][!//
                            [!VAR "temp" = "bit:shr(CanLegacyFIFOGlobalMask0, 15)"!][!//
                            [!VAR "temp" = "bit:shl($temp, 16)"!][!//
                            [!VAR "RXFGMASK" = "bit:or($RXFGMASK, $temp)"!][!//
                            [!VAR "temp" = "bit:shr(CanLegacyFIFOGlobalMask1, 15)"!][!//
                            [!VAR "temp" = "bit:shl($temp, 0)"!][!//
                            [!VAR "RXFGMASK" = "bit:or($RXFGMASK, $temp)"!][!//
                        [!ENDIF!][!//
                    [!ELSE!][!// format C
                        [!VAR "RXFGMASK" = "0"!][!// no mask for IDE, RTR
                        [!IF "$LegacyIDType = 'STANDARD'"!][!//
                            [!VAR "temp" = "bit:shr(CanLegacyFIFOGlobalMask0, 3)"!][!//
                            [!VAR "temp" = "bit:shl($temp, 24)"!][!//
                            [!VAR "RXFGMASK" = "bit:or($RXFGMASK, $temp)"!][!//
                            [!VAR "temp" = "bit:shr(CanLegacyFIFOGlobalMask1, 3)"!][!//
                            [!VAR "temp" = "bit:shl($temp, 16)"!][!//
                            [!VAR "RXFGMASK" = "bit:or($RXFGMASK, $temp)"!][!//
                            [!VAR "temp" = "bit:shr(CanLegacyFIFOGlobalMask2, 3)"!][!//
                            [!VAR "temp" = "bit:shl($temp, 8)"!][!//
                            [!VAR "RXFGMASK" = "bit:or($RXFGMASK, $temp)"!][!//
                            [!VAR "temp" = "bit:shr(CanLegacyFIFOGlobalMask3, 3)"!][!//
                            [!VAR "temp" = "bit:shl($temp, 0)"!][!//
                            [!VAR "RXFGMASK" = "bit:or($RXFGMASK, $temp)"!][!//
                        [!ELSE!][!//
                            [!VAR "temp" = "bit:shr(CanLegacyFIFOGlobalMask0, 21)"!][!//
                            [!VAR "temp" = "bit:shl($temp, 24)"!][!//
                            [!VAR "RXFGMASK" = "bit:or($RXFGMASK, $temp)"!][!//
                            [!VAR "temp" = "bit:shr(CanLegacyFIFOGlobalMask1, 21)"!][!//
                            [!VAR "temp" = "bit:shl($temp, 16)"!][!//
                            [!VAR "RXFGMASK" = "bit:or($RXFGMASK, $temp)"!][!//
                            [!VAR "temp" = "bit:shr(CanLegacyFIFOGlobalMask2, 21)"!][!//
                            [!VAR "temp" = "bit:shl($temp, 8)"!][!//
                            [!VAR "RXFGMASK" = "bit:or($RXFGMASK, $temp)"!][!//
                            [!VAR "temp" = "bit:shr(CanLegacyFIFOGlobalMask3, 21)"!][!//
                            [!VAR "temp" = "bit:shl($temp, 0)"!][!//
                            [!VAR "RXFGMASK" = "bit:or($RXFGMASK, $temp)"!][!//
                        [!ENDIF!][!//
                    [!ENDIF!][!//
                    /* Global mask of Legacy FIFO */
                    (uint32)[!"num:inttohex($RXFGMASK, 8)"!]U,
                    /* ID Acceptance Mode */
                    [!"concat('CAN_LEGACY_FIFO_', node:value(./CanIdAcceptanceMode))"!],
                    /* Legacy FIFO Warning Notification */
                    [!IF "./CanFiFoWarnNotif != 'NULL_PTR'"!][!"normalize-space(./CanFiFoWarnNotif)"!][!ELSE!]NULL_PTR[!ENDIF!],
                    /* Legacy FIFO Overflow Notification */
                    [!IF "./CanFiFoOverflowNotif != 'NULL_PTR'"!][!"normalize-space(./CanFiFoOverflowNotif)"!][!ELSE!]NULL_PTR[!ENDIF!],
                [!ENDSELECT!]
            [!ELSE!]
                /* --- Default values when Legacy FIFO is not available --- */
                /* Global mask of Legacy FIFO */
                (uint32)0xFFFFFFFFU,
                /* ID Acceptance Mode */
                CAN_LEGACY_FIFO_FORMAT_A,
                /* Legacy FIFO Warning Notification */
                NULL_PTR,
                /* Legacy FIFO Overflow Notification */
                NULL_PTR,
            [!ENDIF!]
            /* Enhanced FIFO Overflow Notification */
            [!IF "node:exists(./CanRxFiFo) and node:name(./CanRxFiFo) = 'CanEnhanceFiFo' and CanRxFiFo/CanEnhanceFiFoDmaEnable = 'false'"!]
                [!IF "./CanRxFiFo/CanFiFoOverflowNotif != 'NULL_PTR'"!][!"normalize-space(./CanRxFiFo/CanFiFoOverflowNotif)"!][!ELSE!]NULL_PTR[!ENDIF!],
            [!ELSE!]
                NULL_PTR,
            [!ENDIF!]
        #if (CAN_FEATURE_HAS_DMA_ENABLE == STD_ON)
            [!IF "node:exists(CanRxFiFo) and CanRxFiFo = 'CanEnhanceFiFo' and CanRxFiFo/CanEnhanceFiFoDmaEnable = 'true' and CanControllerActivation = 'true'"!]
                /* FIFO Dma Error Notification */
                [!IF "./CanRxFiFo/CanFiFoDmaErrorNotif != 'NULL_PTR'"!][!"normalize-space(./CanRxFiFo/CanFiFoDmaErrorNotif)"!][!ELSE!]NULL_PTR[!ENDIF!],
                /* DMA destination address */
                g_DmaDestinationAddr[!"CanControllerId"!],
            [!ELSEIF "node:exists(CanRxFiFo) and CanRxFiFo = 'CanLegacyFiFo' and CanRxFiFo/CanLegacyFiFoDmaEnable = 'true' and CanControllerActivation = 'true'"!]
                 /* FIFO Dma Error Notification */
                [!IF "./CanRxFiFo/CanFiFoDmaErrorNotif != 'NULL_PTR'"!][!"normalize-space(./CanRxFiFo/CanFiFoDmaErrorNotif)"!][!ELSE!]NULL_PTR[!ENDIF!],
                /* DMA destination address */
                g_DmaDestinationAddr[!"CanControllerId"!],
            [!ELSE!]
                /* Enhance FIFO Dma Error Notification */
                NULL_PTR,
                /* DMA is not enabled */
                NULL_PTR,
            [!ENDIF!]
        #endif /* (CAN_FEATURE_HAS_DMA_ENABLE == STD_ON) */
            /* Error interrupt enable */
            [!IF "node:exists(./CanErrorNotification)"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!],[!CR!]
            /* Error Notification */
            [!IF "node:exists(./CanErrorNotification)"!][!"normalize-space(./CanErrorNotification)"!][!ELSE!]NULL_PTR[!ENDIF!],[!CR!]
            /* Error FD Notification */
            [!IF "text:contains(ecu:get('Can.CanConfigSet.CanFdChannelList'), (./CanHwChannel)) and node:exists(./CanErrorNotification)"!][!"normalize-space(./CanFDErrorNotification)"!][!ELSE!]NULL_PTR[!ENDIF!],[!CR!]
            /* Default Baudrate ID */
            (uint16)[!"node:ref(CanControllerDefaultBaudrate)/CanControllerBaudRateConfigID"!]U,
            /* Baudrate config Count*/
            (uint16)[!"num:i(count(./CanControllerBaudrateConfig/*))"!]U,
            /* Baudrate config Pointer Structure */
            Can_aBaudrateConfig_Ctrl[!"./CanControllerId"!],
            /* Pointer to IPW structure to IP config */
            &CanIpwHwChannelConfig[!"$postBuildVariantNameUnderscore"!][!"$CtrlIdx"!],
            /* Hw Object reference count */
            [!VAR "ObjectCount" = "num:i(count(../../CanHardwareObject/*[node:ref(./CanControllerRef)/CanControllerId = $CtrlIdx]))"!]
            (uint8)[!"$ObjectCount"!]U,
            /* Pointer point to Group of Hw Object that refer to Controller */
            [!IF "$ObjectCount > 0"!]Can_apHwObject_Ctrl[!"./CanControllerId"!][!ELSE!]NULL_PTR[!ENDIF!][!CR!]
        #if (CAN_TIMESTAMP_ENABLE == STD_ON)
            ,(boolean)[!IF "node:exists(./CanControllerTimeStamp)"!]TRUE[!ELSE!]FALSE[!ENDIF!][!CR!]
        #endif /* (CAN_TIMESTAMP_ENABLE == STD_ON) */
            [!ENDINDENT!]
            [!VAR "CtrlIdx" = "num:i($CtrlIdx + 1)"!]
        }[!IF "$CtrlIdx < $CtrlConfigCount"!],[!ENDIF!][!CR!]
        [!ENDLOOP!]
        [!ENDINDENT!]
    };

[!VAR "Icom_En" = "'false'"!]
[!IF "(ecu:get('Can.CanConfigSet.CanPretendedNetworking') = 'STD_ON') and (../CanGeneral/CanPublicIcomSupport = 'true')"!]
[!VAR "Icom_En" = "'true'"!]
/* Inport external structure defined by IPW */
CAN_IPW_ICOM_RXMESSAGE_EXT

[!VAR "numOfPnConfig" = "num:i(count(CanIcom/CanIcomConfig/*))"!]
static const Can_IcomConfigsType Can_aIcomConfig[[!"$numOfPnConfig"!]U] =
{
    [!INDENT "4"!]
    [!VAR "Idx" = "0"!]
    [!LOOP "node:order(CanIcom/CanIcomConfig/*, './CanIcomConfigId')"!]
    {
         [!INDENT "8"!]
        /* .u8CanIcomConfigId */
        (uint8)[!"./CanIcomConfigId"!],
        /* .bCanIcomWakeOnBusOff */
        (boolean)[!"text:toupper(./CanIcomWakeOnBusOff)"!],
        /* .u8NumberCanIcomRxMessage */
        (uint8)[!"num:i(count(./CanIcomWakeupCauses/CanIcomRxMessage/*))"!]U,
        /* .pCanIcomRxMessageConfigs */
        &CanIpwIcomRxMessageConfig[!"$postBuildVariantNameUnderscore"!]_[!"num:i($Idx)"!]
        [!ENDINDENT!]
        [!VAR "Idx" = "num:i($Idx + 1)"!]
    }[!IF "$Idx < $numOfPnConfig"!],[!ENDIF!][!CR!]
    [!ENDLOOP!]
    [!ENDINDENT!]
};
[!ENDIF!]

[!ENDSELECT!]

[!VAR "CanEcucPartitionRefCount" = "num:i(count(./CanGeneral/CanEcucPartitionRef/*))"!]
[!IF "$CanEcucPartitionRefCount != 0"!]
    [!LOOP "./CanGeneral/CanEcucPartitionRef/*"!]
        [!VAR "CanEcuCPartitionPath" = "node:value(.)"!]
        [!VAR "CanPartitionMatchCounter" = "0"!]
        [!LOOP "../../../CanConfigSet/CanController/*"!]
            [!IF "node:exists(./CanControllerEcucPartitionRef) and (node:value(CanControllerEcucPartitionRef) = $CanEcuCPartitionPath)"!]
                [!VAR "CanPartitionMatchCounter" = "1"!]
            [!ENDIF!]
        [!ENDLOOP!]
        [!IF "$CanPartitionMatchCounter = 1"!]
            /**
            *   @brief  Can Controller Object's pointer structure of [!"node:name(node:ref(.))"!]
            */
            static const Can_ControllerConfigType * const Can_apController_[!"node:name(node:ref(.))"!][CAN_CONTROLLER_CONFIG_COUNT]=
            {
                [!INDENT "4"!]
                [!VAR "CtrIdx" = "0"!]
                [!LOOP "node:order(../../../CanConfigSet/CanController/*, './CanControllerId')"!]
                    [!VAR "CtrIdx" = "num:i($CtrIdx + 1)"!]
                    [!IF "node:exists(./CanControllerEcucPartitionRef)"!]
                        [!IF "(node:value(./CanControllerEcucPartitionRef) = $CanEcuCPartitionPath)"!]
                            &Can_aControllerConfig[[!"num:i(./CanControllerId)"!]U][!IF "$CtrIdx < $CtrlConfigCount"!],[!ENDIF!][!CR!]
                        [!ELSE!]
                            NULL_PTR[!IF "$CtrIdx < $CtrlConfigCount"!],[!ENDIF!][!CR!]
                        [!ENDIF!]
                    [!ELSE!]
                        NULL_PTR[!IF "$CtrIdx < $CtrlConfigCount"!],[!ENDIF!][!CR!]
                    [!ENDIF!]
                [!ENDLOOP!]
                [!ENDINDENT!]
            };
        [!ENDIF!]
    [!ENDLOOP!]
[!ELSE!]
    static const Can_ControllerConfigType * const Can_apController[CAN_CONTROLLER_CONFIG_COUNT]=
    {
        [!INDENT "4"!]
            [!VAR "CtrIdx" = "0"!]
            [!LOOP "node:order(./CanConfigSet/CanController/*, './CanControllerId')"!]
                [!VAR "CtrIdx" = "num:i($CtrIdx + 1)"!]
                &Can_aControllerConfig[[!"num:i(./CanControllerId)"!]U][!IF "$CtrIdx < $CtrlConfigCount"!],[!ENDIF!][!CR!]
            [!ENDLOOP!]
        [!ENDINDENT!]
    };
[!ENDIF!]
/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/
/**
*   @brief  Top level structure contain all Can driver Config
*/
[!IF "(./IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1)"!][!// VariantPreCompile/LinkTime and at most 1 configured variant
    [!IF "$CanEcucPartitionRefCount != 0"!][!// Existing CAN EcucPartition References
        [!LOOP "./CanGeneral/CanEcucPartitionRef/*"!]
            [!VAR "CanEcuCPartitionPath" = "node:value(.)"!]
            [!VAR "CanEcucPartitionName" = "substring-after(substring-after($CanEcuCPartitionPath, 'EcuC/EcuC/'), '/')"!]
            [!VAR "CanPartitionMatchCounter" = "0"!]
            [!LOOP "../../../CanConfigSet/CanController/*"!]
                [!IF "node:exists(./CanControllerEcucPartitionRef) and (node:value(CanControllerEcucPartitionRef) = $CanEcuCPartitionPath)"!]
                    [!VAR "CanPartitionMatchCounter" = "1"!]
                [!ENDIF!]
            [!ENDLOOP!]
            [!IF "$CanPartitionMatchCounter = 1"!]
                static const Can_ConfigType[!WS!]Can_Config_[!"$CanEcucPartitionName"!] =
                {
                    [!INDENT "4"!]
                        /* Configuration Core ID */
                        [!VAR "OsAppMatchCounter" = "0"!]
                        [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
                            [!IF "contains(node:value(./OsAppEcucPartitionRef), $CanEcucPartitionName)"!]
                                [!VAR "OsAppMatchCounter" = "1"!]
                                [!VAR "CoreID" = "node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!]
                            [!ENDIF!]
                        [!ENDLOOP!]
                        [!IF "$OsAppMatchCounter = 1"!](uint32)[!"$CoreID"!]U[!ELSE!]0U[!ENDIF!],[!CR!]
                        /* The HTH first Index after HRH consecutive */
                        (Can_HwHandleType)[!"num:i(count(../../../CanConfigSet/CanHardwareObject/*[./CanObjectType = 'RECEIVE']))"!]U,
                        /* Mapping Controller ID to Controller Hw Offset */
                        Can_aCtrlOffsetToCtrlIDMap,
                        /* Mapping Controller ID to Hardware Object ID */
                        Can_aHwObjIDToCtrlIDMap,
                        /* Pointer point to Can Hw Object Config */
                        Can_aHwObjectConfig,
                        [!IF "$Icom_En = 'true'"!]
                        /* The size of Can Icom Configs */
                        (uint8)[!"$numOfPnConfig"!]U,
                        /* Pointer point to Can Icom Configs */
                        Can_aIcomConfig,
                        [!ENDIF!]
                        /* Array of Pointer to Controller Type which are assigned to partition */
                        Can_apController_[!"$CanEcucPartitionName"!]
                    [!ENDINDENT!]
                };[!CR!]
            [!ENDIF!]
        [!ENDLOOP!]
    [!ELSE!][!// No ECUC partition referenced in CAN
        /* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_3 MISRA 2012 Advisory Rule 8.7 */
        /* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_4 MISRA 2012 Advisory Rule 8.9 */
        /* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_5 MISRA 2012 Required Rule 8.4 */
        const Can_ConfigType[!WS!]Can_Config =
        {
            [!INDENT "4"!]
                /* Configuration Core ID */
                (uint32)0U,
                /* The HTH first Index after HRH consecutive */
                (Can_HwHandleType)[!"num:i(count(./CanConfigSet/CanHardwareObject/*[./CanObjectType = 'RECEIVE']))"!]U,
                /* Mapping Controller ID to Controller Hw Offset */
                Can_aCtrlOffsetToCtrlIDMap,
                /* Mapping Controller ID to Hardware Object ID */
                Can_aHwObjIDToCtrlIDMap,
                /* Pointer point to Can Hw Object Config */
                Can_aHwObjectConfig,
                [!IF "$Icom_En = 'true'"!]
                /* The size of Can Icom Configs */
                (uint8)[!"$numOfPnConfig"!]U,
                /* Pointer point to Can Icom Configs */
                Can_aIcomConfig,
                [!ENDIF!]
                /* Array of Pointer to Controller infomation have aligned by Controller ID */
                Can_apController
            [!ENDINDENT!]
        };[!CR!]

    [!ENDIF!]
[!ELSE!][!// VariantPostBuild or more than 1 configured variant
    [!IF "variant:size() > 0"!][!// If ECUC exists or there is at least one variant
        [!IF "$CanEcucPartitionRefCount != 0"!][!// Existing CAN EcucPartition References
            [!LOOP "./CanGeneral/CanEcucPartitionRef/*"!]
                [!VAR "CanEcuCPartitionPath" = "node:value(.)"!]
                [!VAR "CanEcucPartitionName" = "substring-after(substring-after($CanEcuCPartitionPath, 'EcuC/EcuC/'), '/')"!]
                [!VAR "CanPartitionMatchCounter" = "0"!]
                [!LOOP "../../../CanConfigSet/CanController/*"!]
                    [!IF "node:exists(./CanControllerEcucPartitionRef) and (node:value(CanControllerEcucPartitionRef) = $CanEcuCPartitionPath)"!]
                        [!VAR "CanPartitionMatchCounter" = "1"!]
                    [!ENDIF!]
                [!ENDLOOP!]
                [!IF "$CanPartitionMatchCounter = 1"!]
                    /* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_5 MISRA 2012 Required Rule 8.4 */
                    /* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_3 MISRA 2012 Advisory Rule 8.7 */
                    const Can_ConfigType[!WS!]Can_Config_[!"variant:name()"!]_[!"$CanEcucPartitionName"!] =
                    {
                        [!INDENT "4"!]
                            /* Configuration Core ID */
                            [!VAR "OsAppMatchCounter" = "0"!]
                            [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
                                [!IF "contains(node:value(./OsAppEcucPartitionRef), $CanEcucPartitionName)"!]
                                    [!VAR "OsAppMatchCounter" = "1"!]
                                    [!VAR "CoreID" = "node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!]
                                [!ENDIF!]
                            [!ENDLOOP!]
                            [!IF "$OsAppMatchCounter = 1"!](uint32)[!"$CoreID"!]U[!ELSE!](uint32)0U[!ENDIF!],[!CR!]
                            /* The HTH first Index after HRH consecutive */
                            (Can_HwHandleType)[!"num:i(count(../../../CanConfigSet/CanHardwareObject/*[./CanObjectType = 'RECEIVE']))"!]U,
                            /* Mapping Controller ID to Controller Hw Offset */
                            Can_aCtrlOffsetToCtrlIDMap,
                            /* Mapping Controller ID to Hardware Object ID */
                            Can_aHwObjIDToCtrlIDMap,
                            /* Pointer point to Can Hw Object Config */
                            Can_aHwObjectConfig,
                            [!IF "$Icom_En = 'true'"!]
                            /* The size of Can Icom Configs */
                            (uint8)[!"$numOfPnConfig"!]U,
                            /* Pointer point to Can Icom Configs */
                            Can_aIcomConfig,
                            [!ENDIF!]
                            /* Array of Pointer to Controller Type which are assigned to partition */
                            Can_apController_[!"$CanEcucPartitionName"!]
                        [!ENDINDENT!]
                    };[!CR!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ELSE!][!// No ECUC partition referenced in CAN
            /* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_5 MISRA 2012 Required Rule 8.4 */
            /* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_3 MISRA 2012 Advisory Rule 8.7 */
            const Can_ConfigType[!WS!]Can_Config_[!"variant:name()"!] =
            {
                [!INDENT "4"!]
                    /* Configuration Core ID */
                    (uint32)0U,
                    /* The HTH first Index after HRH consecutive */
                    (Can_HwHandleType)[!"num:i(count(./CanConfigSet/CanHardwareObject/*[./CanObjectType = 'RECEIVE']))"!]U,
                    /* Mapping Controller ID to Controller Hw Offset */
                    Can_aCtrlOffsetToCtrlIDMap,
                    /* Mapping Controller ID to Hardware Object ID */
                    Can_aHwObjIDToCtrlIDMap,
                    /* Pointer point to Can Hw Object Config */
                    Can_aHwObjectConfig,
                    [!IF "$Icom_En = 'true'"!]
                    /* The size of Can Icom Configs */
                    (uint8)[!"$numOfPnConfig"!]U,
                    /* Pointer point to Can Icom Configs */
                    Can_aIcomConfig,
                    [!ENDIF!]
                    /* Array of Pointer to Controller infomation have aligned by Controller ID */
                    Can_apController
                [!ENDINDENT!]
            };[!CR!]

        [!ENDIF!]
    [!ELSE!][!// PB with no variant or no ECUC
        [!IF "$CanEcucPartitionRefCount != 0"!][!// Existing CAN EcucPartition References
            [!LOOP "./CanGeneral/CanEcucPartitionRef/*"!]
                [!VAR "CanEcuCPartitionPath" = "node:value(.)"!]
                [!VAR "CanEcucPartitionName" = "substring-after(substring-after($CanEcuCPartitionPath, 'EcuC/EcuC/'), '/')"!]
                [!VAR "CanPartitionMatchCounter" = "0"!]
                [!LOOP "../../../CanConfigSet/CanController/*"!]
                    [!IF "node:exists(./CanControllerEcucPartitionRef) and (node:value(CanControllerEcucPartitionRef) = $CanEcuCPartitionPath)"!]
                        [!VAR "CanPartitionMatchCounter" = "1"!]
                    [!ENDIF!]
                [!ENDLOOP!]
                [!IF "$CanPartitionMatchCounter = 1"!]
                    /* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_5 MISRA 2012 Required Rule 8.4 */
                    /* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_3 MISRA 2012 Advisory Rule 8.7 */
                    const Can_ConfigType[!WS!]Can_Config_[!"$CanEcucPartitionName"!] =
                    {
                        [!INDENT "4"!]
                            /* Configuration Core ID */
                            [!VAR "OsAppMatchCounter" = "0"!]
                            [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
                                [!IF "contains(node:value(./OsAppEcucPartitionRef), $CanEcucPartitionName)"!]
                                    [!VAR "OsAppMatchCounter" = "1"!]
                                    [!VAR "CoreID" = "node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!]
                                [!ENDIF!]
                            [!ENDLOOP!]
                            [!IF "$OsAppMatchCounter = 1"!](uint32)[!"$CoreID"!]U[!ELSE!](uint32)0U[!ENDIF!],[!CR!]
                            /* The HTH first Index after HRH consecutive */
                            (Can_HwHandleType)[!"num:i(count(../../../CanConfigSet/CanHardwareObject/*[./CanObjectType = 'RECEIVE']))"!]U,
                            /* Mapping Controller ID to Controller Hw Offset */
                            Can_aCtrlOffsetToCtrlIDMap,
                            /* Mapping Controller ID to Hardware Object ID */
                            Can_aHwObjIDToCtrlIDMap,
                            /* Pointer point to Can Hw Object Config */
                            Can_aHwObjectConfig,
                            [!IF "$Icom_En = 'true'"!]
                            /* The size of Can Icom Configs */
                            (uint8)[!"$numOfPnConfig"!]U,
                            /* Pointer point to Can Icom Configs */
                            Can_aIcomConfig,
                            [!ENDIF!]
                            /* Array of Pointer to Controller Type which are assigned to partition */
                            Can_apController_[!"$CanEcucPartitionName"!]
                        [!ENDINDENT!]
                    };[!CR!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ELSE!][!// No ECUC partition referenced in CAN
            /* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_3 MISRA 2012 Advisory Rule 8.7 */
            /* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_4 MISRA 2012 Advisory Rule 8.9 */
            /* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_5 MISRA 2012 Required Rule 8.4 */
            const Can_ConfigType[!WS!]Can_Config =
            {
                [!INDENT "4"!]
                    /* Configuration Core ID */
                    (uint32)0U,
                    /* The HTH first Index after HRH consecutive */
                    (Can_HwHandleType)[!"num:i(count(./CanConfigSet/CanHardwareObject/*[./CanObjectType = 'RECEIVE']))"!]U,
                    /* Mapping Controller ID to Controller Hw Offset */
                    Can_aCtrlOffsetToCtrlIDMap,
                    /* Mapping Controller ID to Hardware Object ID */
                    Can_aHwObjIDToCtrlIDMap,
                    /* Pointer point to Can Hw Object Config */
                    Can_aHwObjectConfig,
                    [!IF "$Icom_En = 'true'"!]
                    /* The size of Can Icom Configs */
                    (uint8)[!"$numOfPnConfig"!]U,
                    /* Pointer point to Can Icom Configs */
                    Can_aIcomConfig,
                    [!ENDIF!]
                    /* Array of Pointer to Controller infomation have aligned by Controller ID */
                    Can_apController
                [!ENDINDENT!]
            };[!CR!]

        [!ENDIF!]
    [!ENDIF!]
[!ENDIF!]

[!IF "(./IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1)"!]
    [!IF "$CanEcucPartitionRefCount != 0"!][!// Existing CAN EcucPartition References
        [!SELECT "as:modconf('EcuC')[1]"!]
            [!VAR "MaxCoreIdConfig" = "num:i(count(./EcucHardware/*[1]/EcucCoreDefinition/*))"!]
        [!ENDSELECT!]
        /* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_4 MISRA 2012 Advisory Rule 8.9 */
        /* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_5 MISRA 2012 Required Rule 8.4 */
        const Can_ConfigType * const [!WS!]Can_Config[[!"$MaxCoreIdConfig"!]U] =
        {
            [!INDENT "4"!]
            [!VAR "coreIdx" = "0"!]
            [!LOOP "node:order(as:modconf('EcuC')[1]/EcucHardware/*[1]/EcucCoreDefinition/*, './EcucCoreId')"!]
                [!VAR "OsAppPart" = "''"!]
                [!VAR "coreIdx" = "$coreIdx + 1"!]
                [!VAR "CoreId" = "node:value(./EcucCoreId)"!]
                [!VAR "CanPartShortName" = "''"!]
                [!VAR "CanPartMatchCounter" = "0"!]
                [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
                    [!IF "$CoreId = node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!]
                        [!VAR "OsAppPart" = "node:value(./OsAppEcucPartitionRef)"!]
                    [!ENDIF!]
                [!ENDLOOP!]
                [!LOOP "as:modconf('Can')[1]/CanGeneral/CanEcucPartitionRef/*"!]
                    [!IF "$OsAppPart = node:value(.)"!]
                        [!VAR "CanPartMatchCounter" = "$CanPartMatchCounter + 1"!]
                        [!VAR "CanPartShortName" = "substring-after(substring-after(., 'EcuC/EcuC/'), '/')"!]
                    [!ENDIF!]
                [!ENDLOOP!]
                [!IF "$CanPartMatchCounter = 1"!]&Can_Config_[!"$CanPartShortName"!][!ELSE!]NULL_PTR[!ENDIF!][!IF "$coreIdx < $MaxCoreIdConfig"!],[!ENDIF!][!CR!]
            [!ENDLOOP!]
            [!ENDINDENT!]
        };
    [!ENDIF!]
[!ENDIF!]
[!ENDINDENT!]
#define CAN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
/* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_1 MISRA 2012 Required Directive 4.10 */
/* @violates @ref Can_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_2 MISRA 2012 Advisory Rule 20.1 */
#include "Can_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
