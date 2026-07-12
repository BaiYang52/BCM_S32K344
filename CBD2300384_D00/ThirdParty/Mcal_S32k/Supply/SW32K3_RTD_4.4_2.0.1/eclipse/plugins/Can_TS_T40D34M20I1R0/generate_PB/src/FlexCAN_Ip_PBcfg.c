
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

/**
*   @file FlexCAN_IP_PBcfg.c
*
*   @addtogroup FlexCAN
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
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "postBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!]
[!ELSE!]
    [!VAR "postBuildVariantNameUnderscore" = "string(null)"!]
[!ENDIF!]
#include "FlexCAN_Ip_Types.h"
#if (FLEXCAN_IP_FEATURE_HAS_DMA_ENABLE == STD_ON)
    #include "Dma_Ip.h"
#endif
#include "Can_Irq.h"
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLEXCAN_IP_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C 43
#define FLEXCAN_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C 4
#define FLEXCAN_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C 4
#define FLEXCAN_IP_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C 0
#define FLEXCAN_IP_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C 2
#define FLEXCAN_IP_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C 0
#define FLEXCAN_IP_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C 1
/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and FlexCAN_Ip_Types header file are of the same vendor */
#if (FLEXCAN_IP_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != FLEXCAN_IP_TYPES_VENDOR_ID_H)
    #error "FlexCAN_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and FlexCAN_Ip_Types.h have different vendor ids"
#endif
/* Check if current file and FlexCAN_Ip_Types header file are of the same Autosar version */
#if ((FLEXCAN_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C    != FLEXCAN_IP_TYPES_AR_RELEASE_MAJOR_VERSION_H) || \
     (FLEXCAN_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C    != FLEXCAN_IP_TYPES_AR_RELEASE_MINOR_VERSION_H) || \
     (FLEXCAN_IP_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != FLEXCAN_IP_TYPES_AR_RELEASE_REVISION_VERSION_H) \
    )
    #error "AutoSar Version Numbers of FlexCAN_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and FlexCAN_Ip_Types.h are different"
#endif
/* Check if current file and FlexCAN_Ip_Types header file are of the same Software version */
#if ((FLEXCAN_IP_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != FLEXCAN_IP_TYPES_SW_MAJOR_VERSION_H) || \
     (FLEXCAN_IP_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != FLEXCAN_IP_TYPES_SW_MINOR_VERSION_H) || \
     (FLEXCAN_IP_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != FLEXCAN_IP_TYPES_SW_PATCH_VERSION_H) \
    )
    #error "Software Version Numbers of FlexCAN_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and FlexCAN_Ip_Types.h are different"
#endif

/* Check if current file and Can_Irq header file are of the same vendor */
#if (FLEXCAN_IP_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != CAN_IRQ_VENDOR_ID)
    #error "FlexCAN_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Can_Irq.h have different vendor ids"
#endif
/* Check if current file and Can_Irq header file are of the same Autosar version */
#if ((FLEXCAN_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C    != CAN_IRQ_AR_RELEASE_MAJOR_VERSION) || \
     (FLEXCAN_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C    != CAN_IRQ_AR_RELEASE_MINOR_VERSION) || \
     (FLEXCAN_IP_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != CAN_IRQ_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of FlexCAN_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Can_Irq.h are different"
#endif
/* Check if current file and Can_Irq header file are of the same Software version */
#if ((FLEXCAN_IP_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != CAN_IRQ_SW_MAJOR_VERSION) || \
     (FLEXCAN_IP_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != CAN_IRQ_SW_MINOR_VERSION) || \
     (FLEXCAN_IP_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != CAN_IRQ_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of FlexCAN_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Can_Irq.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against current file and Dma_Ip.h */
    #if (FLEXCAN_IP_FEATURE_HAS_DMA_ENABLE == STD_ON)
        #if ((FLEXCAN_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C    != DMA_IP_AR_RELEASE_MAJOR_VERSION) || \
            (FLEXCAN_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C     != DMA_IP_AR_RELEASE_MINOR_VERSION) \
            )
            #error "AUTOSAR Version Numbers of FlexCAN_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Dma_Ip.h are different"
        #endif
    #endif
#endif

[!AUTOSPACING!]
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
[!SELECT "CanConfigSet"!]
[!VAR "CtrlConfigCount" = "num:i(count(./CanController/*))"!]
[!VAR "interruptEnable" = "0"!]
[!VAR "needInterruptExtern" = "0"!]
[!LOOP "node:order(./CanController/*, './CanControllerId')"!]
    [!IF "(CanRxProcessing != 'POLLING') or (CanTxProcessing != 'POLLING')"!]
        [!VAR "needInterruptExtern" = "1"!]
    [!ENDIF!]
[!ENDLOOP!]

[!IF "ecu:get('Can.CanConfigSet.CanPretendedNetworking') = 'STD_ON' and ../CanGeneral/CanPublicIcomSupport = 'true'"!]
    [!VAR "needInterruptExtern" = "1"!]
[!ENDIF!]
/*==================================================================================================
*                                   GLOBAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/
#define CAN_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Can_MemMap.h"

const Flexcan_Ip_ConfigType Flexcan_aCtrlConfigPB[!"$postBuildVariantNameUnderscore"!][[!"$CtrlConfigCount"!]U]=
{
    [!INDENT "4"!]
    [!VAR "CtrlIdx" = "num:i(0)"!]
    [!VAR "CanGeneralPart" = "as:path(../CanGeneral, true())"!]
    [!LOOP "node:order(./CanController/*, './CanControllerId')"!]
    /* Can Hardware Channel [!"./CanHwChannel"!] */
    {
        [!NOCODE!]
        [!VAR "interruptEnable" = "0"!]
        [!VAR "enNumIdFilter" = "0"!]
        [!VAR "CtrlIdx" = "num:i(./CanControllerId)"!]
        [!IF "CanRxProcessing != 'POLLING'"!]
            [!VAR "interruptEnable" = "1"!]
        [!ENDIF!]
        [!IF "CanTxProcessing != 'POLLING'"!]
            [!VAR "interruptEnable" = "1"!]
        [!ENDIF!]
        [!IF "ecu:get('Can.CanConfigSet.CanPretendedNetworking') = 'STD_ON' and ../../../CanGeneral/CanPublicIcomSupport = 'true' and text:contains(ecu:get('Can.CanConfigSet.CanPretendedNetworkingList'), ./CanHwChannel)"!]
            [!VAR "interruptEnable" = "1"!]
        [!ENDIF!]
        [!ENDNOCODE!]
        [!INDENT "8"!]
        /* Number Of Message Buffer used .max_num_mb */
        [!VAR "CtrlNumberOfMbUsed" = "text:split($Can_aNumberOfMbUsed, '#')[position() - 1 = number($CtrlIdx)]"!]
        (uint8)[!"num:i($CtrlNumberOfMbUsed)"!]U,
        /*Can Hw filter count* .num_id_filters */
        [!IF "node:exists(CanRxFiFo) and node:name(CanRxFiFo) = 'CanLegacyFiFo'"!]
            [!SELECT "../../CanHardwareObject"!]
                [!LOOP "node:order(./*, './CanObjectId')"!]
                    [!IF "node:ref(./CanControllerRef)/CanControllerId = $CtrlIdx and ./CanObjectType = 'RECEIVE' and $enNumIdFilter=0"!]
                        [!VAR "ObjectFilterCount" = "num:i(count(./CanHwFilter/*))"!]
                        [!IF "node:ref(./CanControllerRef)/CanRxFiFo/CanIdAcceptanceMode = 'FORMAT_A'"!]
                            [!VAR "ObjectFilterCount" = "$ObjectFilterCount"!]
                        [!ELSEIF "node:ref(./CanControllerRef)/CanRxFiFo/CanIdAcceptanceMode = 'FORMAT_B'"!]
                            [!VAR "ObjectFilterCount" = "num:i($ObjectFilterCount div 2)"!]
                        [!ELSEIF "node:ref(./CanControllerRef)/CanRxFiFo/CanIdAcceptanceMode = 'FORMAT_C'"!]
                            [!VAR "ObjectFilterCount" = "num:i($ObjectFilterCount div 4)"!]
                        [!ENDIF!]
                        (Flexcan_Ip_RxFifoIdFilterNumType)(([!"$ObjectFilterCount"!]U >> 3U) - 1U),
                        [!VAR "enNumIdFilter" = "1"!]
                    [!ENDIF!]
                [!ENDLOOP!]
            [!ENDSELECT!]
        [!ELSE!]
            (Flexcan_Ip_RxFifoIdFilterNumType)0U,
        [!ENDIF!]
        /* Legacy FIFO ENABLED .is_rx_fifo_needed */
        [!IF "node:exists(CanRxFiFo) and node:name(CanRxFiFo) = 'CanLegacyFiFo'"!]
        (boolean)TRUE,
        [!ELSE!]
        (boolean)FALSE,
        [!ENDIF!]
#if (FLEXCAN_IP_FEATURE_HAS_ENHANCED_RX_FIFO == STD_ON)
        [!VAR "dmaEnable" = "0"!]
        [!VAR "stdIdEnhencedFilters" = "num:i(0)"!]
        [!VAR "extIdEnhencedFilters" = "num:i(0)"!]
        [!IF "node:exists(CanRxFiFo) and node:name(CanRxFiFo) = 'CanLegacyFiFo'"!]
            [!IF "CanRxFiFo/CanLegacyFiFoDmaEnable = 'true'"!]
                [!VAR "dmaEnable" = "1"!]
            [!ENDIF!]
        [!ENDIF!]
        [!IF "node:exists(CanRxFiFo) and node:name(CanRxFiFo) = 'CanEnhanceFiFo'"!]
            [!VAR "schemeType" = "CanRxFiFo/CanEnhancedSchemeType"!]
            [!SELECT "../../CanHardwareObject"!]
                [!LOOP "node:order(./*, './CanObjectId')"!]
                    [!IF "node:ref(./CanControllerRef)/CanControllerId = $CtrlIdx and ./CanObjectType = 'RECEIVE'"!]
                        [!IF "CanIdType = 'STANDARD'"!]
                            [!VAR "stdIdEnhencedFilters" = "(num:i(count(./CanHwFilter/*)))"!]
                        [!ELSE!]
                            [!VAR "extIdEnhencedFilters" = "(num:i(count(./CanHwFilter/*)))"!]
                        [!ENDIF!]
                        [!BREAK!]
                    [!ENDIF!]
                [!ENDLOOP!]
            [!ENDSELECT!]
            [!IF "$schemeType != 'MASK_FILTER_SCHEME'"!]
                [!VAR "stdIdEnhencedFilters" = "num:i($stdIdEnhencedFilters div 2)"!]
                [!VAR "extIdEnhencedFilters" = "num:i($extIdEnhencedFilters div 2)"!]
            [!ENDIF!]
            [!IF "CanRxFiFo/CanEnhanceFiFoDmaEnable = 'true'"!]
                [!VAR "dmaEnable" = "1"!]
            [!ENDIF!]
        [!ENDIF!]
        /* The number of standard ID filter elements */
        [!"$stdIdEnhencedFilters"!]U,
        /* The number of extended ID filter elements */
        [!"$extIdEnhencedFilters"!]U,
        [!IF "$dmaEnable = 1 and node:name(CanRxFiFo) = 'CanEnhanceFiFo'"!]
            /* The number of enhanced Rx FIFO watermark */
            [!"num:i(CanRxFiFo/NumberMBTransferDMA)"!]U,
        [!ELSE!]
            /* The number of enhanced Rx FIFO watermark */
            [!"num:i(ecu:get('Can.CanConfigSet.CanRxFiFoEnhancedDeep') - 1)"!]U,
        [!ENDIF!]
        /* The Enhanced Rx FIFO feature is enabled or not. */
        [!IF "node:exists(CanRxFiFo) and node:name(CanRxFiFo) = 'CanEnhanceFiFo'"!]
          (boolean)TRUE,
        [!ELSE!]
          (boolean)FALSE,
        [!ENDIF!]
#endif
        [!IF "(ecu:get('Can.CanConfigSet.TimeStampSupport')='STD_ON') and (node:exists(./../../../CanGeneral/CanTimeStamp))"!]
#if (FLEXCAN_IP_FEATURE_HAS_TS_ENABLE == STD_ON)
            [!IF "node:exists(CanControllerTimeStamp)"!]
                {
                /* timeStampSurce Timer Source */
                [!IF "text:contains(ecu:get('Can.CanConfigSet.Ctrl2TimerSrcAvailableList'), ./CanHwChannel)"!]
                    [!"node:value(CanControllerTimeStamp/TimestampTimeSource)"!]
                [!ELSE!]
                    FLEXCAN_CAN_CLK_TIMESTAMP_SRC
                [!ENDIF!]
                [!IF "ecu:get('Can.CanConfigSet.TimeStampSrcList') != ''"!]
            #if (FLEXCAN_IP_FEATURE_HAS_HR_TIMER == STD_ON)
                /* msgBuffTimeStampType Timestamp MB Type  */
                [!IF "node:exists(CanRxFiFo) and node:name(CanRxFiFo) = 'CanLegacyFiFo' and node:value(CanControllerTimeStamp/HRTimeStampCapturePoint) != 'FLEXCAN_TIMESTAMPCAPTURE_DISABLE'"!][!// Hardcode: when legacy fifo and hr timestamp used together:the timestamp value of legacy fifo always captures from TIMER register.=> to sync up all timestamp values => TIMER is clocked from 16 bits upper of HR time source !]
                    ,FLEXCAN_MSGBUFFTIMESTAMP_UPPER
                [!ELSE!]
                    ,[!"node:value(CanControllerTimeStamp/MBTSBASE)"!]
                [!ENDIF!]
                    /* hrConfigType Timestamp HR capture configuration */
                    ,[!"node:value(CanControllerTimeStamp/HRTimeStampCapturePoint)"!]
                    /* hrSrc HT Timer Source */
                    ,[!"node:value(../../../CanGeneral/CanTimeStamp/TimestampHRTimeSource)"!]
            #endif /* (FLEXCAN_IP_FEATURE_HAS_HR_TIMER == STD_ON) */
                [!ENDIF!]
                },
            [!ELSE!]
                {
                    /* timeStampSurce Timer Source */
                    FLEXCAN_CAN_CLK_TIMESTAMP_SRC
                [!IF "ecu:get('Can.CanConfigSet.TimeStampSrcList') != ''"!]
            #if (FLEXCAN_IP_FEATURE_HAS_HR_TIMER == STD_ON)
                    /* msgBuffTimeStampType Timestamp MB Type  */
                    ,FLEXCAN_MSGBUFFTIMESTAMP_TIMER
                    /* hrConfigType Timestamp HR capture configuration */
                    ,FLEXCAN_TIMESTAMPCAPTURE_DISABLE
                    /* hrSrc HT Timer Source */
                    ,[!"ecu:list('Can.CanConfigSet.TimeStampSrcList')[1]"!]
            #endif /* (FLEXCAN_IP_FEATURE_HAS_HR_TIMER == STD_ON) */
                [!ENDIF!]
                },
            [!ENDIF!]
#endif /* (FLEXCAN_IP_FEATURE_HAS_TS_ENABLE == STD_ON) */
        [!ENDIF!]
        /* Loopback enabled .flexcanMode */
        [!IF "CanLoopBackMode = 'true'"!]
            FLEXCAN_LOOPBACK_MODE,
        [!ELSE!]
            FLEXCAN_NORMAL_MODE,
        [!ENDIF!]
        /* Controller Options .ctrlOptions */
        (uint32)([!//
        [!INDENT "16"!]
            [!IF "./CanTrippleSamplingEnable = 'true'"!] FLEXCAN_IP_THREE_SAMPLES_U32 |[!ENDIF!]
            [!IF "./CanAutoBusOffRecovery = 'true'"!] FLEXCAN_IP_BUSOFF_RECOVERY_U32 |[!ENDIF!]
            [!IF "./CanControllerPrExcEn = 'true'"!] FLEXCAN_IP_PROTOCOL_EXCEPTION_U32 |[!ENDIF!]
            [!IF "./CanControllerEdgeFilter = 'true'"!] FLEXCAN_IP_EDGE_FILTER_U32 |[!ENDIF!]
            [!IF "./CanControllerFdISO = 'true'"!] FLEXCAN_IP_ISO_U32 |[!ENDIF!]
            FLEXCAN_IP_EACEN_U32),
        [!ENDINDENT!]
        /* Can FD RamBlock specified .payload */
        [!IF "node:exists(./CanRamBlock)"!]
        {
            [!INDENT "12"!]
            [!FOR "i" = "1" TO "num:i(ecu:get('Can.CanConfigSet.NumOfRegion'))"!]
                [!IF "node:exists(./CanRamBlock/*[num:i($i)])"!]
                    [!VAR "PayloadSize" = "num:i(text:split(./CanRamBlock/*[num:i($i)], '_')[2])"!][!// RamBlock specific
                [!ELSE!]
                    [!VAR "PayloadSize" = "num:i(text:split(./CanRamBlock/*[1], '_')[2])"!][!// RamBlock unified
                [!ENDIF!]
                FLEXCAN_PAYLOAD_SIZE_[!"$PayloadSize"!][!IF "$i < ecu:get('Can.CanConfigSet.NumOfRegion')"!],[!ENDIF!][!CR!]
            [!ENDFOR!]
            [!ENDINDENT!]
        },
        [!ELSE!]
        {
            [!INDENT "12"!]
            [!FOR "i" = "1" TO "ecu:get('Can.CanConfigSet.NumOfRegion')"!]
                FLEXCAN_PAYLOAD_SIZE_8[!IF "$i < ecu:get('Can.CanConfigSet.NumOfRegion')"!],[!ENDIF!][!CR!]
            [!ENDFOR!]
            [!ENDINDENT!]
        },
        [!ENDIF!]
        /* Can FD enabled .fd_enable */
        [!IF "node:exists(node:ref(./CanControllerDefaultBaudrate)/CanControllerFdBaudrateConfig)"!]
            [!VAR "CanFDEnable" = "'true'"!]
            (boolean)TRUE,
        [!ELSE!]
            (boolean)FALSE,
        [!ENDIF!]
        [!IF "ecu:get('Can.CanConfig.CtrlClksrcAvailable') = 'STD_ON'"!]
#if (FLEXCAN_IP_FEATURE_HAS_PE_CLKSRC_SELECT == STD_ON)
        /* Can PE clock selected .is_pe_clock */
        (boolean)[!"text:toupper(node:value(./CanClockFromBus))"!],
#endif
        [!ENDIF!]
        /* Cbt support .extCbtEnable */
        (boolean)[!IF "node:ref(./CanControllerDefaultBaudrate)/CanBaudrateTypeSuport = 'ENHANCE_CBT'"!]TRUE[!ELSE!]FALSE[!ENDIF!],[!CR!]
        /*  BRS for FD .bitRateSwitch */
        [!IF "node:exists(node:ref(./CanControllerDefaultBaudrate)/CanControllerFdBaudrateConfig)"!]
            (boolean)[!"text:toupper(node:ref(./CanControllerDefaultBaudrate)/CanControllerFdBaudrateConfig/CanControllerTxBitRateSwitch)"!],
        [!ELSE!]
            (boolean)FALSE,
        [!ENDIF!]
        /* Values for normal baudrate .bitrate */
        {
            [!INDENT "12"!]
            [!IF "(node:ref(./CanControllerDefaultBaudrate)/CanAdvancedSetting) = 'false'"!]
                (uint8)[!"num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerPropSeg - 1)"!]U,
                (uint8)[!"num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerSeg1 - 1)"!]U,
                (uint8)[!"num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerSeg2 - 1)"!]U,
                (uint16)[!"num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerPrescaller - 1)"!],
                (uint8)[!"num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerSyncJumpWidth - 1)"!]U
            [!ELSE!]
                [!VAR "CanClockSrc" = "node:ref(./CanCpuClockRef)/McuClockReferencePointFrequency"!]
                [!VAR "TimeQuantum" = "num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerPrescaller) div $CanClockSrc"!]
                [!VAR "NumOfTQN" = "($CanClockSrc div ((node:ref(./CanControllerDefaultBaudrate)/CanControllerBaudRate) * 1000 * (node:ref(./CanControllerDefaultBaudrate)/CanControllerPrescaller)))"!][!// Number of Tq in normal bit time.
                [!VAR "BusPropogationDelay" = "5 div 1000000000"!][!// Hard coded to 5ns/m.
                [!VAR "PhysicalDelayOfBus" = "(node:ref(./CanControllerDefaultBaudrate)/CanBusLength) * $BusPropogationDelay"!]
                [!VAR "tPropseg" = "2 * ($PhysicalDelayOfBus + (node:ref(./CanControllerDefaultBaudrate)/CanPropDelayTranceiver) div 1000000000)"!]
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
                    [!ELSE!]
                        [!ERROR!]
                            For [!"node:ref(./CanControllerDefaultBaudrate)/@name"!] of [!"./@name"!]
                            PHASE_SEG1 + PHASE_SEG2 is ([!"$CanPhaseSeg"!]) and it's out of range [3:16]
                        [!ENDERROR!]
                    [!ENDIF!]
                [!ELSE!]
                    [!ERROR!]
                        For [!"node:ref(./CanControllerDefaultBaudrate)/@name"!] of [!"./@name"!]
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
                (uint16)[!"num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerPrescaller - 1)"!],
                (uint8)[!"num:i($CanSJWN - 1)"!]U
            [!ENDIF!]
            [!ENDINDENT!]
        },
        /* Values for CBT baudrate .bitrate_cbt */
        [!IF "node:exists(node:ref(./CanControllerDefaultBaudrate)/CanControllerFdBaudrateConfig)"!]
        {
            [!INDENT "12"!]
            (uint8)[!"num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerFdBaudrateConfig/CanControllerPropSeg)"!]U,
            (uint8)[!"num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerFdBaudrateConfig/CanControllerSeg1 - 1)"!]U,
            (uint8)[!"num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerFdBaudrateConfig/CanControllerSeg2 - 1)"!]U,
        [!IF "node:exists(node:ref(./CanControllerDefaultBaudrate)/CanControllerFdBaudrateConfig/CanControllerFdPrescaller)"!]
            (uint16)[!"num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerFdBaudrateConfig/CanControllerFdPrescaller - 1)"!]U,
        [!ELSE!]
            (uint16)[!"num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerPrescaller - 1)"!]U,
        [!ENDIF!]
            (uint8)[!"num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerFdBaudrateConfig/CanControllerSyncJumpWidth - 1)"!]U
            [!ENDINDENT!]
        },
        [!ELSE!]
        {
            [!INDENT "12"!]
            (uint8)[!"num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerPropSeg)"!]U,
            (uint8)[!"num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerSeg1 - 1)"!]U,
            (uint8)[!"num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerSeg2 - 1)"!]U,
            (uint16)[!"num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerPrescaller - 1)"!]U,
            (uint8)[!"num:i(node:ref(./CanControllerDefaultBaudrate)/CanControllerSyncJumpWidth - 1)"!]U
            [!ENDINDENT!]
        },
        [!ENDIF!]
        /* Fifo Transfer Type .transfer_type */
        [!NOCODE!]
        [!VAR "usePooling" = "0"!]
        [!IF "node:exists(CanRxFiFo)"!]
            [!IF "CanRxProcessing = 'POLLING'"!]
                [!VAR "usePooling" = "1"!]
            [!ELSE!]
                [!SELECT "../../CanHardwareObject"!]
                    [!LOOP "node:order(./*, './CanObjectId')"!]
                        [!IF "node:ref(./CanControllerRef)/CanControllerId = $CtrlIdx and ./CanObjectType = 'RECEIVE'"!]
                            [!IF "node:exists(./CanHardwareObjectUsesPolling)"!]
                                [!IF "node:value(./CanHardwareObjectUsesPolling) = 'true'"!]
                                    [!VAR "usePooling" = "1"!]
                                [!ELSE!]
                                    [!VAR "usePooling" = "0"!]
                                [!ENDIF!]
                            [!ELSE!]
                                [!VAR "usePooling" = "0"!]
                            [!ENDIF!]
                            [!BREAK!]
                        [!ENDIF!]
                    [!ENDLOOP!]
                [!ENDSELECT!]
                [!IF "CanRxFiFo = 'CanLegacyFiFo'"!]
                    [!IF "CanRxFiFo/CanLegacyFiFoDmaEnable = 'true'"!]
                        [!VAR "usePooling" = "2"!][!// dma
                    [!ENDIF!]
                [!ENDIF!]
                [!IF "CanRxFiFo = 'CanEnhanceFiFo'"!]
                    [!IF "CanRxFiFo/CanEnhanceFiFoDmaEnable = 'true'"!]
                        [!VAR "usePooling" = "2"!][!// dma
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDIF!]
        [!ELSE!]
            [!VAR "usePooling" = "0"!]
        [!ENDIF!]
        [!ENDNOCODE!]
        [!IF "num:i($usePooling) = '2'"!]
            FLEXCAN_RXFIFO_USING_DMA,
        [!ELSE!]
            [!IF "num:i($usePooling) = '1'"!]
                FLEXCAN_RXFIFO_USING_POLLING,
            [!ELSE!]
                FLEXCAN_RXFIFO_USING_INTERRUPTS,
            [!ENDIF!]
        [!ENDIF!]
#if (FLEXCAN_IP_FEATURE_HAS_DMA_ENABLE == STD_ON)
        [!IF "$dmaEnable = 1 and node:name(CanRxFiFo) = 'CanEnhanceFiFo'"!]
            /* DMA channel number used for transfers. */
            (uint8)[!"node:value(node:ref(CanRxFiFo/CanEnhanceFiFoDmaRef)/dmaLogicChannel_LogicName)"!],
        [!ELSEIF "$dmaEnable = 1 and node:name(CanRxFiFo) = 'CanLegacyFiFo'"!]
            /* DMA channel number used for transfers. */
            (uint8)[!"node:value(node:ref(CanRxFiFo/CanLegacyFiFoDmaRef)/dmaLogicChannel_LogicName)"!],
        [!ELSE!]
            /* DMA channel number used for transfers. */
            (uint8)0U,
        [!ENDIF!]
#endif
        /* Controller Callback .Callback */
        [!IF "num:i($interruptEnable) = '1'"!]
            Can_CommonIrqCallback,
        [!ELSE!]
            NULL_PTR,
        [!ENDIF!]
        /* Error Callback .ErrorCallback */
        Can_ErrorIrqCallback
        [!ENDINDENT!]
        }[!IF "num:i($CtrlIdx + 1) < $CtrlConfigCount"!],[!ENDIF!][!CR!]
    [!ENDLOOP!]
    [!ENDINDENT!]
};


[!IF "ecu:get('Can.CanConfigSet.CanPretendedNetworking') = 'STD_ON' and ../CanGeneral/CanPublicIcomSupport = 'true'"!]
    [!VAR "numOfPnConfig" = "num:i(count(CanIcom/CanIcomConfig/*/CanIcomWakeupCauses/CanIcomRxMessage/*))"!]
    [!VAR "Idx" = "0"!]
/* PN array configuration */
const Flexcan_Ip_PnConfigType Flexcan_aPnConfigPB[!"$postBuildVariantNameUnderscore"!][[!"$numOfPnConfig"!]U]=
{
    [!INDENT "4"!]
    [!LOOP "node:order(CanIcom/CanIcomConfig/*, './CanIcomConfigId')"!][!LOOP "CanIcomWakeupCauses/CanIcomRxMessage/*"!]
    {
        [!INDENT "8"!]
        /* Wakeup timeout enable .bWakeUpTimeout */
        (boolean)[!IF "node:exists(./CanIcomMissingMessageTimerValue) and ./CanIcomMissingMessageTimerValue != 0"!]TRUE[!ELSE!]FALSE[!ENDIF!],[!CR!]
        /* Wakeup on mactch enable .bWakeUpMatch */
        (boolean)TRUE,
        /* The number of matches needed before generating an wake up event .u16numMatches */
        (uint16)[!IF "node:exists(./CanIcomCounterValue)"!][!"./CanIcomCounterValue"!]U[!ELSE!]1U[!ENDIF!],[!CR!]
        [!IF "node:exists(./CanIcomMissingMessageTimerValue)"!]
            [!VAR "NumOfTicks" = "num:i((./CanIcomMissingMessageTimerValue * node:ref(./CanIcomDefaultBaudrate)/CanControllerBaudRate * 1000) div 64)"!]
            [!IF "$NumOfTicks > 65535 or $NumOfTicks < 1"!]
                [!ERROR!]
                    For [!"./@name"!], enabled CanIcomMissingMessageTimerValue referred to [!"node:ref(./CanIcomDefaultBaudrate)/@name"!]
                    the number of ticks converted is (CanIcomMissingMessageTimerValue * baudrate) / 64 is out of range [1:65535]
                [!ENDERROR!]
            [!ENDIF!]
        [!ELSE!]
            [!VAR "NumOfTicks" = "num:i(0)"!]
        [!ENDIF!]
        /*  Defines a timeout value that generates an wake up event if wakeUpTimeout is true .u16matchTimeout */
        (uint16)[!"$NumOfTicks"!],[!CR!]
        /* Defines the filtering scheme used .eFilterComb */
        [!IF "./CanPayloadFilter = 'true'"!][!// if payload filter is enabled.
            [!IF "node:exists(./CanIcomCounterValue) and ./CanIcomCounterValue > 0"!]
                FLEXCAN_FILTER_ID_PAYLOAD_NTIMES,
            [!ELSE!]
                FLEXCAN_FILTER_ID_PAYLOAD,
            [!ENDIF!]
        [!ELSE!][!// always filter ID
            [!IF "node:exists(./CanIcomCounterValue) and ./CanIcomCounterValue > 0"!]
                FLEXCAN_FILTER_ID_NTIMES,
            [!ELSE!]
                FLEXCAN_FILTER_ID,
            [!ENDIF!]
        [!ENDIF!]
        /* The configuration of the first ID filter .idFilter1 */
        {
            [!INDENT "12"!]
            /* Specifies if the ID is standard or extended .bExtendedId */
            (boolean)[!IF "./CanIcomMessageIdType = 'EXTENDED'"!]TRUE[!ELSE!]FALSE[!ENDIF!],[!CR!]
            /* Specifies if the frame is standard or remote .bRemoteFrame */
            (boolean)FALSE,
            /* Specifies the ID value .u32Id */
            (uint32)[!"./CanIcomMessageId"!]U
            [!ENDINDENT!]
        },
        /* The configuration of the first ID filter .idFilter2 */
        {
            [!INDENT "12"!]
            /* Specifies if the ID is standard or extended .bExtendedId */
            (boolean)TRUE,
            /* Specifies if the frame is standard or remote .bRemoteFrame */
            (boolean)TRUE,
            /* Specifies the ID value .u32Id */
            [!IF "./CanIcomIdOperation = 'INSIDE_RANGE' or ./CanIcomIdOperation = 'EXACTLY'"!]
                (uint32)[!IF "node:exists(./CanIcomMessageIdMask)"!][!"./CanIcomMessageIdMask"!]U[!ELSE!]0x1FFFFFFFU[!ENDIF!][!CR!]
            [!ELSE!]
                (boolean)0U,
            [!ENDIF!]
            [!ENDINDENT!]
        },
        /* Defines the ID filtering scheme .eIdFilterType */
        [!IF "./CanIcomIdOperation = 'EXACTLY'"!]
            FLEXCAN_FILTER_MATCH_EXACT,
        [!ELSEIF "./CanIcomIdOperation = 'GREATER_MINNUM'"!]
            FLEXCAN_FILTER_MATCH_GEQ,
        [!ELSEIF "./CanIcomIdOperation = 'SMALLER_MAXNUM'"!]
            FLEXCAN_FILTER_MATCH_LEQ,
        [!ELSEIF "./CanIcomIdOperation = 'INSIDE_RANGE'"!]
            FLEXCAN_FILTER_MATCH_RANGE,
        [!ENDIF!]
        /* Defines the payload filtering scheme. .ePayloadFilterType */
        [!IF "./CanPayloadFilter = 'true'"!]
            [!IF "./CanIcomRxMessageSignalConfig/*[1]/CanIcomSignalOperation = 'EQUAL'"!]
                FLEXCAN_FILTER_MATCH_EXACT,
            [!ELSEIF "./CanIcomRxMessageSignalConfig/*[1]/CanIcomSignalOperation = 'GREATER'"!]
                FLEXCAN_FILTER_MATCH_GEQ,
            [!ELSEIF "./CanIcomRxMessageSignalConfig/*[1]/CanIcomSignalOperation = 'SMALLER'"!]
                FLEXCAN_FILTER_MATCH_LEQ,
            [!ELSEIF "./CanIcomRxMessageSignalConfig/*[1]/CanIcomSignalOperation = 'XOR'"!]
                FLEXCAN_FILTER_MATCH_RANGE,
            [!ENDIF!]
        [!ELSE!]
            FLEXCAN_FILTER_MATCH_EXACT,
        [!ENDIF!]
        /* The configuration of the payload filter .payloadFilter */
        [!IF "./CanPayloadFilter = 'true'"!]
        {
            [!INDENT "12"!]
            /* Specifies the lower limit of the payload size .u8DlcLow */
            (uint8)[!"./CanIcomRxMessageSignalConfig/*[1]/DLCLowValue"!]U,
            /* Specifies the upper limit of the payload size .u8DlcHigh */
            (uint8)[!"./CanIcomRxMessageSignalConfig/*[1]/DLCHighValue"!]U,
            [!VAR "IcomSignal_Val" = "./CanIcomRxMessageSignalConfig/*[1]/CanIcomSignalValue"!]
            {[!FOR "i" = "1" TO "8"!][!"num:inttohex(bit:and(bit:shr($IcomSignal_Val, (8 - $i) * 8), 255), 2)"!]U[!IF "$i < 8"!], [!ENDIF!][!ENDFOR!] },
            [!IF "./CanIcomRxMessageSignalConfig/*[1]/CanIcomSignalOperation = 'EQUAL' or ./CanIcomRxMessageSignalConfig/*[1]/CanIcomSignalOperation = 'XOR'"!]
                [!VAR "IcomSignal_Val" = "./CanIcomRxMessageSignalConfig/*[1]/CanIcomSignalMask"!]
            [!ELSE!]
                [!VAR "IcomSignal_Val" = "9223372036854775807"!]
            [!ENDIF!]
            {[!FOR "i" = "1" TO "8"!][!"num:inttohex(bit:and(bit:shr($IcomSignal_Val, (8 - $i) * 8), 255), 2)"!]U[!IF "$i < 8"!], [!ENDIF!][!ENDFOR!] }
            [!ENDINDENT!]
        }
        [!ELSE!]
        {
            [!INDENT "12"!]
            (uint8)0U,
            (uint8)0U,
            {0U},
            {0U}
            [!ENDINDENT!]
        }
        [!ENDIF!]
        [!ENDINDENT!]
        [!VAR "Idx" = "$Idx + 1"!]
    }[!IF "$Idx < $numOfPnConfig"!],[!ENDIF!][!CR!]
    [!ENDLOOP!][!ENDLOOP!]
    [!ENDINDENT!]
};
[!ENDIF!]

#define CAN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Can_MemMap.h"

[!ENDSELECT!]

#ifdef __cplusplus
}
#endif

/** @} */
