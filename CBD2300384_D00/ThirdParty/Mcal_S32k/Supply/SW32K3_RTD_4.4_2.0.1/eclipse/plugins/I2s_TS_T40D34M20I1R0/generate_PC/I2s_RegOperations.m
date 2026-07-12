[!/* *** multiple inclusion protection *** */!]
[!IF "not(var:defined('I2S_REGOPERATIONS_M'))"!]
[!VAR "I2S_REGOPERATIONS_M"="'true'"!]
[!NOCODE!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : I2S
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
*   @file
*
*   @addtogroup i2s_driver_config I2s Driver Configuration
*   @{
*/

[!ENDNOCODE!][!//
[!AUTOSPACING!]
// ----------------------------------------- SAI IPL Configuration -----------------------------------------
// IP Callback declaration
[!MACRO "Sai_Ip_ConfigCallbackDeclaration"!][!//
[!NOCODE!][!//
    [!LOOP "I2sConfigSet/I2sConfiguration/*[(I2sIpType='SAI')]"!][!//
[!CODE!]
[!IF "I2sCallback != 'NULL_PTR'"!][!//
/**
* @brief          SAI User Callback Function [!"@index"!].
*/
extern void [!"I2sCallback"!](Sai_Ip_ReportType event);
[!ENDIF!]
[!ENDCODE!][!//
    [!ENDLOOP!]
[!ENDNOCODE!]
[!ENDMACRO!]

// IP Config
[!MACRO "Sai_Ip_Config" , "MacroGenerationType"!][!// GenerationType='definition' or 'declaration'
[!NOCODE!][!//
[!VAR "VariantsNo" = "variant:size()"!]
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "postBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!]
[!ELSE!]
    [!VAR "postBuildVariantNameUnderscore" = "string(null)"!]
[!ENDIF!]
[!LOOP "I2sConfigSet/I2sConfiguration/*[(I2sIpType='SAI')]"!][!//
    [!IF "$MacroGenerationType = 'declaration'"!]
        [!IF "../../../GeneralConfiguration/I2sMulticoreSupport"!][!//
            [!VAR "I2sConfiguredPartitions" = "num:i((count(./I2sChannelEcucPartitionRef/*)))"!][!//
            [!IF "$I2sConfiguredPartitions > 0"!][!//
            [!VAR "currentPartitionNumber" = "substring-after(substring-after(substring-after(substring-after(./I2sChannelEcucPartitionRef/*[1],'/'),'/'),'/'),'/')"!][!//
    [!CODE!][!//
/**
* @brief          SAI Ip Config [!"@index"!] for configuration variant [!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!]_[!"$currentPartitionNumber"!][!ENDIF!].
*/
extern const Sai_Ip_ConfigType Sai_Ip_xInitConfig_[!"@index"!][!"$postBuildVariantNameUnderscore"!]_[!"$currentPartitionNumber"!];
[!ENDCODE!][!//
            [!ENDIF!]
        [!ELSE!]
[!CODE!][!//
/**
* @brief          SAI Ip Config [!"@index"!] for configuration variant [!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!].
*/
extern const Sai_Ip_ConfigType Sai_Ip_xInitConfig_[!"@index"!][!"$postBuildVariantNameUnderscore"!];
[!ENDCODE!][!//
        [!ENDIF!]
    [!ELSEIF "$MacroGenerationType = 'definition_state'"!]
        [!IF "../../../GeneralConfiguration/I2sMulticoreSupport"!][!//
            [!VAR "I2sConfiguredPartitions" = "num:i((count(./I2sChannelEcucPartitionRef/*)))"!][!//
            [!IF "$I2sConfiguredPartitions > 0"!][!//
            [!VAR "currentPartitionNumber" = "substring-after(substring-after(substring-after(substring-after(./I2sChannelEcucPartitionRef/*[1],'/'),'/'),'/'),'/')"!][!//
[!CODE!][!//
/**
* @brief          SAI Ip State [!"@index"!] for configuration variant [!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!]_[!"$currentPartitionNumber"!][!ENDIF!].
*/
static Sai_Ip_StateStructType Sai_Ip_xState_[!"@index"!][!"$postBuildVariantNameUnderscore"!]_[!"$currentPartitionNumber"!] = {0};
[!ENDCODE!][!//
            [!ENDIF!]
        [!ELSE!]
[!CODE!][!//
/**
* @brief          SAI Ip State [!"@index"!] for configuration variant [!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!].
*/
static Sai_Ip_StateStructType Sai_Ip_xState_[!"@index"!][!"$postBuildVariantNameUnderscore"!] = {0};
[!ENDCODE!][!//
        [!ENDIF!]
    [!ELSE!]
        [!IF "../../../GeneralConfiguration/I2sMulticoreSupport"!][!//
            [!VAR "I2sConfiguredPartitions" = "num:i((count(./I2sChannelEcucPartitionRef/*)))"!][!//
            [!IF "$I2sConfiguredPartitions > 0"!][!//
            [!VAR "currentPartitionNumber" = "substring-after(substring-after(substring-after(substring-after(./I2sChannelEcucPartitionRef/*[1],'/'),'/'),'/'),'/')"!][!//

[!CODE!][!//
/**
* @brief          SAI Ip Config [!"@index"!] for configuration variant [!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!]_[!"$currentPartitionNumber"!][!ENDIF!].
*/
const Sai_Ip_ConfigType Sai_Ip_xInitConfig_[!"@index"!][!"$postBuildVariantNameUnderscore"!]_[!"$currentPartitionNumber"!] =[!//
[!ENDCODE!]
            [!ENDIF!]
        [!ELSE!]
[!CODE!][!//
/**
* @brief          SAI Ip Config [!"@index"!] for configuration variant [!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!].
*/
const Sai_Ip_ConfigType Sai_Ip_xInitConfig_[!"@index"!][!"$postBuildVariantNameUnderscore"!] =[!//
[!ENDCODE!]
        [!ENDIF!]

[!CODE!]
{
[!ENDCODE!][!//
[!VAR "SaiChannelMask" = "0"!][!//
[!IF "node:ref(I2sModuleRef)/SaiDataLine0"!]
[!VAR "SaiChannelMask" = "$SaiChannelMask + 1"!][!//
[!ENDIF!]
[!IF "node:ref(I2sModuleRef)/SaiDataLine1"!]
[!VAR "SaiChannelMask" = "$SaiChannelMask + 2"!][!//
[!ENDIF!]
[!IF "node:ref(I2sModuleRef)/SaiDataLine2"!]
[!VAR "SaiChannelMask" = "$SaiChannelMask + 4"!][!//
[!ENDIF!]
[!IF "node:ref(I2sModuleRef)/SaiDataLine3"!]
[!VAR "SaiChannelMask" = "$SaiChannelMask + 8"!][!//
[!ENDIF!]
[!VAR "varSaiDmaChannel0" = "'0U'"!][!//
[!VAR "varSaiDmaChannel1" = "'0U'"!][!//
[!VAR "varSaiDmaChannel2" = "'0U'"!][!//
[!VAR "varSaiDmaChannel3" = "'0U'"!][!//
[!IF "node:value(node:ref(I2sModuleRef)/SaiTransferType) = 'DMA'"!]
    [!IF "node:exists(node:ref(I2sModuleRef)/SaiDmaChannel0) = 'true'"!]
        [!VAR "varSaiDmaChannel0" = "node:value(node:ref(node:ref(I2sModuleRef)/SaiDmaChannel0)/dmaLogicChannel_LogicName)"!][!//
    [!ENDIF!]
    [!IF "node:exists(node:ref(I2sModuleRef)/SaiDmaChannel1) = 'true'"!]
        [!VAR "varSaiDmaChannel1" = "node:value(node:ref(node:ref(I2sModuleRef)/SaiDmaChannel1)/dmaLogicChannel_LogicName)"!][!//
    [!ENDIF!]
    [!IF "node:exists(node:ref(I2sModuleRef)/SaiDmaChannel2) = 'true'"!]
        [!VAR "varSaiDmaChannel2" = "node:value(node:ref(node:ref(I2sModuleRef)/SaiDmaChannel2)/dmaLogicChannel_LogicName)"!][!//
    [!ENDIF!]
    [!IF "node:exists(node:ref(I2sModuleRef)/SaiDmaChannel3) = 'true'"!]
        [!VAR "varSaiDmaChannel3" = "node:value(node:ref(node:ref(I2sModuleRef)/SaiDmaChannel3)/dmaLogicChannel_LogicName)"!][!//
    [!ENDIF!]
[!ENDIF!]
[!CODE!][!//
    (Sai_Ip_SyncType) [!"concat('SAI_IP_', node:ref(I2sModuleRef)/SaiSyncType)"!], /* .eSyncMode */
    (Sai_Ip_MasterClkSelType) [!"concat('SAI_IP_', node:ref(I2sModuleRef)/SaiMasterClkSrc)"!], /* .eMasterClkSrc */
    (boolean)[!IF "node:ref(I2sModuleRef)/SaiBitClkNegPolar = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* .bBitClkNegPolar */
    (boolean)[!IF "node:ref(I2sModuleRef)/SaiMode = 'Master'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* .bBitClkInternal */
    (uint32)[!"num:i(num:i(noderef:value(concat(noderef:ref(noderef:value(concat(noderef:ref(node:fallback(node:ref(I2sModuleRef)/SaiClkSrcRef, .)), '/I2sClockReference'))),'/McuClockReferencePointFrequency'))))"!]U, /* .u32MasterClkFreq */
    (uint32)[!"num:i((((node:value(node:ref(I2sModuleRef)/SaiFrameSize) - 1) * node:value(node:ref(I2sModuleRef)/SaiWordNWidth)) + node:value(node:ref(I2sModuleRef)/SaiWord0Width)) * node:value(node:ref(I2sModuleRef)/SaiSampleRate))"!]U, /* .u32BitClkFreq */
    (uint8)[!"num:i($SaiChannelMask)"!]U, /* .u8ChannelEnable */
    (uint8)[!"node:ref(I2sModuleRef)/SaiFrameSize"!]U, /* .u8FrameSize */
    (uint8)[!"node:ref(I2sModuleRef)/SaiSyncWidth"!]U, /* .u8SyncWidth */
#ifdef FEATURE_SAI_IP_HAS_CHMOD
    (Sai_Ip_MaskType) [!"concat('SAI_IP_', node:ref(I2sModuleRef)/SaiChannelMode)"!], /* .eMaskMode */
#endif
#if (STD_ON == SAI_IP_BCS_FEATURE_AVAILABLE)
    (boolean)[!IF "node:ref(I2sModuleRef)/SaiBitClkSwap = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* .bBitClkSwap */
#endif
    (boolean)[!IF "node:ref(I2sModuleRef)/SaiEnableDebug = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* .bDebugEnable */
    (boolean)[!IF "node:ref(I2sModuleRef)/SaiMsbFirst = 'MSB'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* .bMsbFirst */
    (boolean)[!IF "node:ref(I2sModuleRef)/SaiSyncEarly = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* .bSyncEarly */
    (boolean)[!IF "node:ref(I2sModuleRef)/SaiSyncNegPolar = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* .bSyncNegPolar */
    (boolean)[!IF "node:ref(I2sModuleRef)/SaiMode = 'Master'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* .bSyncInternal */
    (uint8)[!"node:ref(I2sModuleRef)/SaiWord0Width"!]U, /* .u8Word0Width */
    (uint8)[!"node:ref(I2sModuleRef)/SaiWordNWidth"!]U, /* .u8WordNWidth */
    (uint8)[!"node:ref(I2sModuleRef)/SaiFirstBitIndex"!]U, /* .u8FirstBitIndex */
    (boolean)[!IF "node:ref(I2sModuleRef)/SaiRunErrorReport = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* .bRunErrorReport */
    (boolean)[!IF "node:ref(I2sModuleRef)/SaiSyncErrorReport = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* .bSyncErrorReport */
    (boolean)[!IF "node:ref(I2sModuleRef)/SaiFrameStartReport = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* .bFrameStartReport */
#if (STD_ON == SAI_IP_FPACK_FEATURE_AVAILABLE)
    (boolean)[!IF "node:ref(I2sModuleRef)/SaiFifoPackEnable = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* .bFPack */
#endif
    (Sai_Ip_MuxType) [!"concat('SAI_IP_', node:ref(I2sModuleRef)/SaiMuxMode)"!], /* .eMuxMode */
    (Sai_Ip_TransferType) [!"concat('SAI_IP_', node:ref(I2sModuleRef)/SaiTransferType)"!], /* .eTransferType */
#if (STD_ON == SAI_IP_DMA_FEATURE_AVAILABLE)
    { [!"concat($varSaiDmaChannel0, ', ', $varSaiDmaChannel1, ', ', $varSaiDmaChannel2, ', ', $varSaiDmaChannel3)"!] }, /* .aDmaChannel */
#endif  /* STD_ON == SAI_IP_DMA_FEATURE_AVAILABLE */
    (uint8)[!"node:ref(I2sModuleRef)/SaiElementSize"!]U, /* .u8ElementSize */
    (uint8)[!"node:ref(I2sModuleRef)/SaiDataLineCount"!]U, /* .u8ChannelCount */
    (uint8)[!"node:ref(I2sModuleRef)/SaiBufferAccessSize"!]U, /* .u8BufferAccessSize */
    (uint8)[!"node:ref(I2sModuleRef)/SaiWordFlagIndex"!]U, /* .u8WordFlagIndex */
    (boolean)[!IF "node:ref(I2sModuleRef)/SaiBitClkInput = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* .bBitClkInput */
    (boolean)[!IF "node:ref(I2sModuleRef)/SaiContOnErr = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* .bContOnErr */
    (boolean)[!IF "node:ref(I2sModuleRef)/SaiSyncOnDemand = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* .bSyncOnDemand */
    (Sai_Ip_TransferCallbackType) [!"I2sCallback"!], /* .pCallback */
[!ENDCODE!][!//
        [!IF "../../../GeneralConfiguration/I2sMulticoreSupport"!][!//
            [!VAR "I2sConfiguredPartitions" = "num:i((count(./I2sChannelEcucPartitionRef/*)))"!][!//
            [!IF "$I2sConfiguredPartitions > 0"!][!//
            [!VAR "currentPartitionNumber" = "substring-after(substring-after(substring-after(substring-after(./I2sChannelEcucPartitionRef/*[1],'/'),'/'),'/'),'/')"!][!//
[!CODE!][!//
    (Sai_Ip_StateStructType *) &Sai_Ip_xState_[!"@index"!][!"$postBuildVariantNameUnderscore"!]_[!"$currentPartitionNumber"!] /* .pState */
[!ENDCODE!][!//
            [!ENDIF!]
        [!ELSE!]
[!CODE!][!//
    (Sai_Ip_StateStructType *) &Sai_Ip_xState_[!"@index"!][!"$postBuildVariantNameUnderscore"!] /* .pState */
[!ENDCODE!][!//
        [!ENDIF!]
[!CODE!][!//
};

[!ENDCODE!][!//
    [!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!]
[!ENDMACRO!]

// IP Config Def
[!MACRO "Sai_Ip_ConfigDef"!][!//
#define SAI_IP_ENABLE                     [!IF "GeneralConfiguration/I2sSaiSupport/I2sSaiEnable"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
[!IF "GeneralConfiguration/I2sSaiSupport/I2sSaiEnable"!][!//
#define SAI_IP_DEV_ERROR_DETECT           [!IF "GeneralConfiguration/I2sSaiSupport/SaiIpDevErrorDetect"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define SAI_IP_TIMEOUT_TYPE               ([!"GeneralConfiguration/I2sTimeoutMethod"!])
#define SAI_IP_INTERNAL_TIMEOUT_VALUE     ([!"GeneralConfiguration/I2sTimeoutVal"!]U)
#define SAI_IP_DMA_FEATURE_AVAILABLE      [!IF "GeneralConfiguration/I2sSaiSupport/SaiDmaFeature"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define SAI_IP_FPACK_FEATURE_AVAILABLE    [!IF "GeneralConfiguration/I2sSaiSupport/SaiFifoPackFeature"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define SAI_IP_BCS_FEATURE_AVAILABLE      [!IF "GeneralConfiguration/I2sSaiSupport/SaiBitClkSwapSupport"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define SAI_IP_ENABLE_DATA_MASKING        [!IF "GeneralConfiguration/I2sSaiSupport/SaiEnableDataMasking"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define SAI_IP_AUTO_DISABLE_CLOCK         [!IF "GeneralConfiguration/I2sSaiSupport/SaiAutoDisableClockFeature"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
[!NOCODE!][!//
[!LOOP "I2sConfigSet/SaiHwConfiguration/*"!][!//
[!CODE!][!//
#define SAI_IP_[!IF "SaiTxRxConfiguration = 'Transmitter'"!]TX[!ELSE!]RX[!ENDIF!]_[!IF "SaiTransferType = 'INTERRUPT'"!]ISR[!"SaiInstanceId"!][!ELSE!]DMA[!"SaiInstanceId"!][!ENDIF!]_USED
[!ENDCODE!][!//
[!ENDLOOP!]
[!ENDNOCODE!]
[!ENDIF!]
[!ENDMACRO!]


// -----------------------------------------Flexio I2s IPL Configuration -----------------------------------------
// IP Callback declaration
[!MACRO "Flexio_I2s_Ip_ConfigCallbackDeclaration"!][!//
[!NOCODE!][!//
    [!LOOP "I2sConfigSet/I2sConfiguration/*[(I2sIpType='FLEXIO_I2S')]"!][!//
[!CODE!]
[!IF "I2sCallback != 'NULL_PTR'"!][!//
/**
* @brief          FLEXIO I2S User Callback Function [!"@index"!].
*/
extern void [!"I2sCallback"!](Flexio_I2s_Ip_EventType Event, void *UserData);
[!ENDIF!]
[!ENDCODE!][!//
    [!ENDLOOP!]
[!ENDNOCODE!]
[!ENDMACRO!]

// IP Config
[!MACRO "Flexio_I2s_Ip_Config" , "MacroGenerationType"!][!// GenerationType='definition' or 'declaration'
[!NOCODE!][!//
[!VAR "VariantsNo" = "variant:size()"!]
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "postBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!]
[!ELSE!]
    [!VAR "postBuildVariantNameUnderscore" = "string(null)"!]
[!ENDIF!]
[!LOOP "I2sConfigSet/I2sConfiguration/*[(I2sIpType='FLEXIO_I2S')]"!][!//
    [!IF "$MacroGenerationType = 'declaration'"!]
        [!IF "../../../GeneralConfiguration/I2sMulticoreSupport"!][!//
            [!VAR "I2sConfiguredPartitions" = "num:i((count(./I2sChannelEcucPartitionRef/*)))"!][!//
            [!IF "$I2sConfiguredPartitions > 0"!][!//
            [!VAR "currentPartitionNumber" = "substring-after(substring-after(substring-after(substring-after(./I2sChannelEcucPartitionRef/*[1],'/'),'/'),'/'),'/')"!][!//
    [!CODE!][!//
/**
* @brief          FLEXIO I2S Ip [!"node:ref(I2sModuleRef)/FlexioI2sMode"!] Config [!"@index"!] for configuration variant [!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!]_[!"$currentPartitionNumber"!][!ENDIF!].
*/
extern const Flexio_I2s_Ip_[!"node:ref(I2sModuleRef)/FlexioI2sMode"!]ConfigType Flexio_I2s_Ip_xInitConfig_[!"@index"!][!"$postBuildVariantNameUnderscore"!]_[!"$currentPartitionNumber"!];
[!ENDCODE!][!//
            [!ENDIF!]
        [!ELSE!]
[!CODE!][!//
/**
* @brief          FLEXIO I2S Ip [!"node:ref(I2sModuleRef)/FlexioI2sMode"!] Config [!"@index"!] for configuration variant [!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!].
*/
extern const Flexio_I2s_Ip_[!"node:ref(I2sModuleRef)/FlexioI2sMode"!]ConfigType Flexio_I2s_Ip_xInitConfig_[!"@index"!][!"$postBuildVariantNameUnderscore"!];
[!ENDCODE!][!//
        [!ENDIF!]
    [!ELSE!][!// definition
        [!IF "../../../GeneralConfiguration/I2sMulticoreSupport"!][!//
            [!VAR "I2sConfiguredPartitions" = "num:i((count(./I2sChannelEcucPartitionRef/*)))"!][!//
            [!IF "$I2sConfiguredPartitions > 0"!][!//
            [!VAR "currentPartitionNumber" = "substring-after(substring-after(substring-after(substring-after(./I2sChannelEcucPartitionRef/*[1],'/'),'/'),'/'),'/')"!][!//
[!CODE!][!//
/**
* @brief          FLEXIO I2S Ip [!"node:ref(I2sModuleRef)/FlexioI2sMode"!] Config [!"@index"!] for configuration variant [!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!]_[!"$currentPartitionNumber"!][!ENDIF!].
*/
const Flexio_I2s_Ip_[!"node:ref(I2sModuleRef)/FlexioI2sMode"!]ConfigType Flexio_I2s_Ip_xInitConfig_[!"@index"!][!"$postBuildVariantNameUnderscore"!]_[!"$currentPartitionNumber"!] =[!//
[!ENDCODE!]
            [!ENDIF!]
        [!ELSE!]
[!CODE!][!//
/**
* @brief          FLEXIO I2S Ip [!"node:ref(I2sModuleRef)/FlexioI2sMode"!] Config [!"@index"!] for configuration variant [!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!].
*/
const Flexio_I2s_Ip_[!"node:ref(I2sModuleRef)/FlexioI2sMode"!]ConfigType Flexio_I2s_Ip_xInitConfig_[!"@index"!][!"$postBuildVariantNameUnderscore"!] =[!//
[!ENDCODE!]
        [!IF "node:ref(I2sModuleRef)/FlexioI2sMode = 'Master'"!][!//
            [!VAR "varFlexioClock" = "noderef:value(concat(noderef:ref(noderef:value(concat(noderef:ref(node:fallback(node:ref(I2sModuleRef)/FlexioI2sClkSrcRef, .)), '/I2sClockReference'))),'/McuClockReferencePointFrequency'))"!][!//
            [!VAR "varDivider" = "((($varFlexioClock + node:ref(I2sModuleRef)/FlexioI2sBaudRate) div (node:ref(I2sModuleRef)/FlexioI2sBaudRate * 2)) - 1)"!][!//
            [!IF "$varDivider > 255"!][!// TODO: there are only 8 bit for divider in flexio, so it cannt be greater than 0xFF, consider to add constraints to configurator
                [!VAR "varDivider" = "255"!][!//
            [!ELSEIF "$varDivider < 1"!]
                [!VAR "varDivider" = "1"!][!//
            [!ENDIF!]
        [!ENDIF!][!//
        [!VAR "varFlexioChannel" = "node:value(node:ref(I2sModuleRef)/FlexioI2sInstanceId)"!][!//
        [!VAR "varResourceIndex" = "substring-after(node:ref(node:ref(I2sModuleRef)/FlexioI2sTxLine)/FlexioMclChannelId, 'CHANNEL_')"!][!//
        [!VAR "varTxPin" = "substring-after(node:ref(node:ref(I2sModuleRef)/FlexioI2sTxLine)/FlexioMclPinId, 'PIN_')"!][!//
        [!VAR "varRxPin" = "0"!][!//
        [!IF "node:ref(I2sModuleRef)/FlexioI2sRxLine"!][!//
            [!VAR "varRxPin" = "substring-after(node:ref(node:ref(I2sModuleRef)/FlexioI2sTxLine)/FlexioMclAddPinId, 'PIN_')"!][!//
        [!ENDIF!]
        [!VAR "varSckPin" = "substring-after(node:ref(node:ref(I2sModuleRef)/FlexioI2sSckLine)/FlexioMclPinId, 'PIN_')"!][!//
        [!VAR "varWsPin" = "0"!][!//
        [!IF "node:ref(I2sModuleRef)/FlexioI2sWsLine"!][!//
            [!VAR "varWsPin" = "substring-after(node:ref(node:ref(I2sModuleRef)/FlexioI2sSckLine)/FlexioMclAddPinId, 'PIN_')"!][!//
        [!ENDIF!]
        [!VAR "varRxDMAChannel" = "'0U'"!][!//
        [!VAR "varTxDMAChannel" = "'0U'"!][!//
        [!IF "node:ref(I2sModuleRef)/FlexioI2sTransferType = 'DMA'"!][!//
            [!VAR "varRxDMAChannel" = "node:value(node:ref(node:ref(I2sModuleRef)/FlexioI2sDmaRxChannel)/dmaLogicChannel_LogicName)"!][!//
            [!VAR "varTxDMAChannel" = "node:value(node:ref(node:ref(I2sModuleRef)/FlexioI2sDmaTxChannel)/dmaLogicChannel_LogicName)"!][!//
        [!ENDIF!]
[!CODE!]
{
    (Flexio_I2s_Ip_DriverType)[!"concat('FLEXIO_I2S_IP_DRIVER_TYPE_', node:ref(I2sModuleRef)/FlexioI2sTransferType)"!], /* .driverType */
    (uint8)[!"num:i($varResourceIndex)"!]U, /* .ResourceIndex */
[!IF "node:ref(I2sModuleRef)/FlexioI2sMode = 'Master'"!][!//
    (uint16)[!"num:i($varDivider)"!]U, /* .DividerValue */
[!ENDIF!][!//
    (uint8)[!"node:ref(I2sModuleRef)/FlexioI2sBitsWidth"!]U, /* .bitsWidth */
    (uint8)[!"num:i($varTxPin)"!]U, /* .txPin */
    (uint8)[!"num:i($varRxPin)"!]U, /* .rxPin */
    (uint8)[!"num:i($varSckPin)"!]U, /* .sckPin */
    (uint8)[!"num:i($varWsPin)"!]U, /* .wsPin */
    [!"I2sCallback"!], /* .callback */
    [!"node:ref(I2sModuleRef)/FlexioI2sCallbackParam"!], /* .callbackParam */
#if (STD_ON == FLEXIO_I2S_IP_DMA_FEATURE_AVAILABLE)
    (uint8)[!"($varRxDMAChannel)"!], /* .rxDMAChannel */
    (uint8)[!"($varTxDMAChannel)"!], /* .txDMAChannel */
#endif /* (STD_ON == FLEXIO_I2S_IP_DMA_FEATURE_AVAILABLE) */
    &Flexio_I2s_Ip_MasterSlaveState[[!"num:i($varFlexioChannel)"!]]
};

[!ENDCODE!]
        [!ENDIF!]
    [!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!]
[!ENDMACRO!]


// IP Config Def
[!MACRO "Flexio_I2s_Ip_FlexioChannelRefDef"!][!//
[!NOCODE!][!//
[!LOOP "I2sConfigSet/I2sConfiguration/*[(I2sIpType='FLEXIO_I2S')]"!][!//
    [!VAR "varTxChannel" = "substring-after(node:ref(node:ref(I2sModuleRef)/FlexioI2sTxLine)/FlexioMclChannelId, 'CHANNEL_')"!][!//
    [!VAR "varRxChannel" = "substring-after(node:ref(node:ref(I2sModuleRef)/FlexioI2sSckLine)/FlexioMclChannelId, 'CHANNEL_')"!][!//
    [!IF "node:ref(I2sModuleRef)/FlexioI2sWsLine"!][!//
        [!VAR "varWsChannel" = "num:i($varRxChannel + 1)"!][!//
    [!ENDIF!][!//
[!CODE!][!//
/* Macros that indicate FLEXIO channels used by I2S */
#ifndef FLEXIO_0_CH_[!"$varTxChannel"!]_USED
    #define FLEXIO_0_CH_[!"$varTxChannel"!]_USED
#else
    #error "CHANNEL_[!"$varTxChannel"!] cannot be used by I2S driver. Channel locked by another driver"
#endif

#ifdef FLEXIO_0_CH_[!"$varTxChannel"!]_USED
    #define I2S_CHANNEL_[!"$varTxChannel"!]_USED
#else
    #error "CHANNEL_[!"$varTxChannel"!] cannot be used by I2S driver. Channel locked by another driver"
#endif

#ifndef FLEXIO_0_CH_[!"$varRxChannel"!]_USED
    #define FLEXIO_0_CH_[!"$varRxChannel"!]_USED
#else
    #error "CHANNEL_[!"$varRxChannel"!] cannot be used by I2S driver. Channel locked by another driver"
#endif

#ifdef FLEXIO_0_CH_[!"$varRxChannel"!]_USED
    #define I2S_CHANNEL_[!"$varRxChannel"!]_USED
#else
    #error "CHANNEL_[!"$varRxChannel"!] cannot be used by I2S driver. Channel locked by another driver"
#endif
[!ENDCODE!][!//
[!IF "node:ref(I2sModuleRef)/FlexioI2sMode = 'Slave'"!][!//
[!CODE!]
#ifndef FLEXIO_0_CH_[!"$varWsChannel"!]_USED
    #define FLEXIO_0_CH_[!"$varWsChannel"!]_USED
#else
    #error "CHANNEL_[!"$varWsChannel"!] cannot be used by I2S driver. Channel locked by another driver"
#endif

#ifdef FLEXIO_0_CH_[!"$varWsChannel"!]_USED
    #define I2S_CHANNEL_[!"$varWsChannel"!]_USED
#else
    #error "CHANNEL_[!"$varWsChannel"!] cannot be used by I2S driver. Channel locked by another driver"
#endif
[!ENDCODE!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDNOCODE!]
[!ENDMACRO!][!//


[!MACRO "Flexio_I2s_Ip_FlexioPinRefDef"!][!//
[!NOCODE!][!//
[!LOOP "I2sConfigSet/I2sConfiguration/*[(I2sIpType='FLEXIO_I2S')]"!][!//
    [!VAR "varTxPin" = "substring-after(node:ref(node:ref(I2sModuleRef)/FlexioI2sTxLine)/FlexioMclPinId, 'PIN_')"!][!//
    [!VAR "varRxPin" = "0"!][!//
    [!IF "node:ref(I2sModuleRef)/FlexioI2sRxLine"!][!//
        [!VAR "varRxPin" = "substring-after(node:ref(node:ref(I2sModuleRef)/FlexioI2sTxLine)/FlexioMclAddPinId, 'PIN_')"!][!//
    [!ENDIF!][!//
    [!VAR "varSckPin" = "substring-after(node:ref(node:ref(I2sModuleRef)/FlexioI2sSckLine)/FlexioMclPinId, 'PIN_')"!][!//
    [!VAR "varWsPin" = "0"!][!//
    [!IF "node:ref(I2sModuleRef)/FlexioI2sWsLine"!][!//
        [!VAR "varWsPin" = "substring-after(node:ref(node:ref(I2sModuleRef)/FlexioI2sSckLine)/FlexioMclAddPinId, 'PIN_')"!][!//
    [!ENDIF!]
[!CODE!][!//
/* Macros that indicate FLEXIO pins used by I2S */
#ifndef FLEXIO_0_PIN_[!"$varTxPin"!]_USED
    #define FLEXIO_0_PIN_[!"$varTxPin"!]_USED
#else
    #error "PIN_[!"$varTxPin"!] cannot be used by I2S driver. Pin used by another driver!"
#endif

#ifndef FLEXIO_0_PIN_[!"$varRxPin"!]_USED
    #define FLEXIO_0_PIN_[!"$varRxPin"!]_USED
#else
    #error "PIN_[!"$varRxPin"!] cannot be used by I2S driver. Pin used by another driver!"
#endif

#ifndef FLEXIO_0_PIN_[!"$varSckPin"!]_USED
    #define FLEXIO_0_PIN_[!"$varSckPin"!]_USED
#else
    #error "PIN_[!"$varSckPin"!] cannot be used by I2S driver. Pin used by another driver!"
#endif

#ifndef FLEXIO_0_PIN_[!"$varWsPin"!]_USED
    #define FLEXIO_0_PIN_[!"$varWsPin"!]_USED
#else
    #error "PIN_[!"$varWsPin"!] cannot be used by I2S driver. Pin used by another driver!"
#endif
[!ENDCODE!][!//
[!ENDLOOP!][!//
[!ENDNOCODE!]
[!ENDMACRO!][!//

// IP Config Def
[!MACRO "Flexio_I2s_Ip_ConfigDef"!][!//
#define FLEXIO_I2S_IP_ENABLE                     [!IF "GeneralConfiguration/I2sFlexioSupport/I2sFlexioI2sEnable"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
[!IF "GeneralConfiguration/I2sFlexioSupport/I2sFlexioI2sEnable"!][!//
#define FLEXIO_I2S_IP_DEV_ERROR_DETECT           [!IF "GeneralConfiguration/I2sFlexioSupport/FlexioI2sIpDevErrorDetect"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define FLEXIO_I2S_IP_TIMEOUT_TYPE               ([!"GeneralConfiguration/I2sTimeoutMethod"!])
#define FLEXIO_I2S_IP_INTERNAL_TIMEOUT_VALUE     ([!"GeneralConfiguration/I2sTimeoutVal"!]U)
#define FLEXIO_I2S_IP_INSTANCE_COUNT             (1U)
#define FLEXIO_I2S_IP_DMA_FEATURE_AVAILABLE      [!IF "GeneralConfiguration/I2sFlexioSupport/FlexioI2sDmaFeature"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define FLEXIO_I2S_IP_MAX_CHANNEL_COUNT          ([!"num:i(ecu:get('I2s.Flexio.Resource.Max'))"!]U)
#define FLEXIO_I2S_IP_NUMBER_OF_CHANNEL          ([!"num:i(count(I2sConfigSet/I2sConfiguration/*[(I2sIpType='FLEXIO_I2S')]))"!]U)
[!ENDIF!]
[!ENDMACRO!]


// ----------------------------------------- IPW Configuration -----------------------------------------
// IPW Config Def
[!MACRO "I2s_Ipw_ConfigDef"!][!//
[!NOCODE!][!//
[!IF "./GeneralConfiguration/I2sMulticoreSupport"!][!//
    [!LOOP "as:modconf('I2s')[1]/I2sEcucPartitionRef/*"!][!//
        [!VAR "currentPartitionNumber" = "substring-after(substring-after(substring-after(substring-after(node:value(.),'/'),'/'),'/'),'/')"!][!//
        [!VAR "ConfigCount" = "0"!][!//
        [!VAR "I2sEcucPartitionRefName" = "node:value(.)"!][!//
        [!LOOP "as:modconf('I2s')[1]/I2sConfigSet/I2sConfiguration/*"!][!//
            [!LOOP "./I2sChannelEcucPartitionRef/*"!][!//
                [!VAR "I2sChannelEcucPartitionRefName" = "node:value(.)"!] [!//
                [!IF "$I2sChannelEcucPartitionRefName = $I2sEcucPartitionRefName"!][!//
                    [!VAR "ConfigCount" = "$ConfigCount + 1"!][!//
                [!ENDIF!][!//
            [!ENDLOOP!][!//
        [!ENDLOOP!][!//
    [!CODE!]#define I2S_IPW_CONFIG_COUNT_[!"$currentPartitionNumber"!]              ([!"num:i($ConfigCount)"!]U)[!CR!][!ENDCODE!]
    [!ENDLOOP!][!//
[!ELSE!]
[!CODE!]#define I2S_IPW_CONFIG_COUNT              ([!"num:i(count(I2sConfigSet/I2sConfiguration/*))"!]U)[!CR!][!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!][!//
#define I2S_IPW_USING_SAI                [!IF "GeneralConfiguration/I2sSaiSupport/I2sSaiEnable"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define I2S_IPW_USING_FLEXIO_I2S         [!IF "GeneralConfiguration/I2sFlexioSupport/I2sFlexioI2sEnable"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define I2S_IPW_ENABLE_DATA_MASKING      (SAI_IP_ENABLE_DATA_MASKING)
#define I2S_IPW_AUTO_DISABLE_CLOCK       (SAI_IP_AUTO_DISABLE_CLOCK)
#define I2S_IPW_TIMEOUT                  ([!"GeneralConfiguration/I2sTimeoutVal"!]U)
#define I2S_LOGICAL_CHANNEL_COUNT        ([!"num:i(count(I2sConfigSet/I2sConfiguration/*))"!]U)
#define I2S_IPW_MULTICORE_ENABLED        [!IF "GeneralConfiguration/I2sMulticoreSupport"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
[!ENDMACRO!]


// IPW Config
[!MACRO "I2s_Ipw_Config" , "MacroGenerationType"!][!// GenerationType='definition' or 'declaration'
[!NOCODE!][!//
[!VAR "VariantsNo" = "variant:size()"!]
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "postBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!]
[!ELSE!]
    [!VAR "postBuildVariantNameUnderscore" = "string(null)"!]
[!ENDIF!]
    [!LOOP "I2sConfigSet/I2sConfiguration/*"!][!//
    [!VAR "varI2sLogicalChannelId" = "node:value(I2sLogicalChannelId)"!][!//
    [!VAR "varI2sIpType" = "node:value(I2sIpType)"!][!//
    [!VAR "varI2sTxRxConfiguration" = "'Transmitter'"!][!//
    [!IF "$varI2sIpType = 'SAI'"!]
        [!VAR "varI2sInstanceId" = "node:value(node:ref(I2sModuleRef)/SaiInstanceId)"!][!//
        [!VAR "varI2sTxRxConfiguration" = "node:value(node:ref(I2sModuleRef)/SaiTxRxConfiguration)"!][!//
        [!VAR "varI2sMode" = "node:value(node:ref(I2sModuleRef)/SaiMode)"!][!//
    [!ELSE!][!// FLEXIO
        [!VAR "varI2sInstanceId" = "node:value(node:ref(I2sModuleRef)/FlexioI2sInstanceId)"!][!//
        [!VAR "varI2sMode" = "node:value(node:ref(I2sModuleRef)/FlexioI2sMode)"!][!//
    [!ENDIF!]
    [!IF "$MacroGenerationType = 'declaration'"!]
        [!IF "../../../GeneralConfiguration/I2sMulticoreSupport"!][!//
            [!VAR "I2sConfiguredPartitions" = "num:i((count(./I2sChannelEcucPartitionRef/*)))"!][!//
            [!IF "$I2sConfiguredPartitions > 0"!][!//
                [!VAR "currentPartitionName"="node:value(./I2sChannelEcucPartitionRef/*[1])"!]
                [!VAR "currentPartitionNumber" = "substring-after(substring-after(substring-after(substring-after(./I2sChannelEcucPartitionRef/*[1],'/'),'/'),'/'),'/')"!][!//
[!CODE!][!//
/**
* @brief          I2s Ipw Config [!"@index"!] for configuration variant [!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!].
*/
extern const I2s_HwConfigType I2s_Ipw_xHwConfig_[!"@index"!][!"$postBuildVariantNameUnderscore"!]_[!"$currentPartitionNumber"!];

[!ENDCODE!][!//
            [!ENDIF!]
        [!ELSE!]
[!CODE!][!//
/**
* @brief          I2s Ipw Config [!"@index"!] for configuration variant [!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!].
*/
extern const I2s_HwConfigType I2s_Ipw_xHwConfig_[!"@index"!][!"$postBuildVariantNameUnderscore"!];

[!ENDCODE!][!//
        [!ENDIF!]
    [!ELSE!]
        [!IF "../../../GeneralConfiguration/I2sMulticoreSupport"!][!//
            [!VAR "I2sConfiguredPartitions" = "num:i((count(./I2sChannelEcucPartitionRef/*)))"!][!//
            [!IF "$I2sConfiguredPartitions > 0"!][!//
            [!VAR "currentPartitionName"="node:value(./I2sChannelEcucPartitionRef/*[1])"!]
            [!VAR "currentPartitionNumber" = "substring-after(substring-after(substring-after(substring-after(./I2sChannelEcucPartitionRef/*[1],'/'),'/'),'/'),'/')"!][!//
            [!VAR "varSaiConfig" = "'NULL_PTR'"!][!//
            [!VAR "varFlexioMasterConfig" = "'NULL_PTR'"!][!//
            [!VAR "varFlexioSlaveConfig" = "'NULL_PTR'"!][!//
            [!IF "$varI2sIpType = 'SAI'"!]
                [!VAR "varSaiConfig" = "concat('&Sai_Ip_xInitConfig_', @index, $postBuildVariantNameUnderscore, '_', $currentPartitionNumber)"!][!//
            [!ELSE!][!// FLEXIO
                [!IF "$varI2sMode = 'Master'"!][!//
                    [!VAR "varFlexioMasterConfig" = "concat('&Flexio_I2s_Ip_xInitConfig_', @index, $postBuildVariantNameUnderscore, '_', $currentPartitionNumber)"!][!//
                [!ELSE!][!//
                    [!VAR "varFlexioSlaveConfig" = "concat('&Flexio_I2s_Ip_xInitConfig_', @index, $postBuildVariantNameUnderscore, '_', $currentPartitionNumber)"!][!//
                [!ENDIF!][!//
            [!ENDIF!]
[!CODE!][!//
/**
* @brief          I2s Ipw Config [!"@index"!] for configuration variant [!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!].
*/
const I2s_HwConfigType I2s_Ipw_xHwConfig_[!"@index"!][!"$postBuildVariantNameUnderscore"!]_[!"$currentPartitionNumber"!] =
{
[!ENDCODE!][!//
    [!VAR "coreused" = "65535"!][!//
    [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
        [!IF "$currentPartitionName = node:value(./OsAppEcucPartitionRef)"!][!//​
            [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//​
                [!VAR "coreused" ="node:value(./EcucCoreId)"!][!//​
            [!ENDSELECT!][!//
        [!ENDIF!][!//​
    [!ENDLOOP!][!//
    [!IF "$coreused = 65535"!][!//
        [!ERROR!][!//
            "Os appication is not configured for [!"text:split($currentPartitionName,'/')[5]"!]"[!//
        [!ENDERROR!][!//
    [!ELSE!][!CODE!][!//
    (uint32)[!"$coreused"!]U, /*.CoreId*/[!ENDCODE!][!ENDIF!][!CR!][!//
    [!CODE!][!//
    (uint8)[!"$varI2sLogicalChannelId"!]U, /*.LogicChn*/
    (uint8)[!"$varI2sInstanceId"!]U, /*.HwInstance*/
    (I2s_SupportedIPsType)[!"concat('I2S_OVER_', $varI2sIpType)"!], /*.IpType*/
    (I2s_ModeType)[!"concat('I2S_', text:toupper($varI2sMode))"!], /*.IpMode*/
    (I2s_TransmitReceiveType)[!"concat('I2S_', text:toupper($varI2sTxRxConfiguration))"!], /*.IpTransmitReceiveMode*/
    { /*.IpConfig*/
[!IF "../../../GeneralConfiguration/I2sSaiSupport/I2sSaiEnable"!][!//
        [!"$varSaiConfig"!], /*.SaiIpConfig*/
[!ENDIF!][!//
[!IF "../../../GeneralConfiguration/I2sFlexioSupport/I2sFlexioI2sEnable"!][!//
        [!"$varFlexioMasterConfig"!], /*.FlexioMasterIpConfig*/
        [!"$varFlexioSlaveConfig"!] /*.FlexioSlaveIpConfig*/
[!ENDIF!][!//
    }

};
[!ENDCODE!][!//
            [!ENDIF!]
        [!ELSE!][!// multicore disabled

[!CODE!][!//
/**
* @brief          I2s Ipw Config [!"@index"!] for configuration variant [!IF "var:defined('postBuildVariant')"!] [!"$postBuildVariant"!][!ENDIF!].
*/
const I2s_HwConfigType I2s_Ipw_xHwConfig_[!"@index"!][!"$postBuildVariantNameUnderscore"!] =
{
[!ENDCODE!][!//
    [!VAR "varI2sTxRxConfiguration" = "'Transmitter'"!][!//
    [!VAR "varSaiConfig" = "'NULL_PTR'"!][!//
    [!VAR "varFlexioMasterConfig" = "'NULL_PTR'"!][!//
    [!VAR "varFlexioSlaveConfig" = "'NULL_PTR'"!][!//
    [!IF "I2sIpType = 'SAI'"!][!//
        [!VAR "varI2sInstanceId" = "node:value(node:ref(I2sModuleRef)/SaiInstanceId)"!][!//
        [!VAR "varI2sTxRxConfiguration" = "node:value(node:ref(I2sModuleRef)/SaiTxRxConfiguration)"!][!//
        [!VAR "varI2sMode" = "node:value(node:ref(I2sModuleRef)/SaiMode)"!][!//
        [!VAR "varSaiConfig" = "concat('&Sai_Ip_xInitConfig_', @index, $postBuildVariantNameUnderscore)"!][!//
    [!ELSE!][!// FLEXIO
        [!VAR "varI2sInstanceId" = "node:value(node:ref(I2sModuleRef)/FlexioI2sInstanceId)"!][!//
        [!VAR "varI2sMode" = "node:value(node:ref(I2sModuleRef)/FlexioI2sMode)"!][!//
        [!IF "$varI2sMode = 'Master'"!][!//
            [!VAR "varFlexioMasterConfig" = "concat('&Flexio_I2s_Ip_xInitConfig_', @index, $postBuildVariantNameUnderscore)"!][!//
        [!ELSE!][!//
            [!VAR "varFlexioSlaveConfig" = "concat('&Flexio_I2s_Ip_xInitConfig_', @index, $postBuildVariantNameUnderscore)"!][!//
        [!ENDIF!][!//
    [!ENDIF!][!//
[!CODE!][!//
    (uint32)0U, /*.CoreId*/
    (uint8) [!"I2sLogicalChannelId"!]U, /*.LogicChn */
    (uint8) [!"$varI2sInstanceId"!]U, /*.HwInstance */
    (I2s_SupportedIPsType) [!"concat('I2S_OVER_', I2sIpType)"!], /*.IpType */
    (I2s_ModeType)[!"concat('I2S_', text:toupper($varI2sMode))"!], /*.IpMode*/
    (I2s_TransmitReceiveType)[!"concat('I2S_', text:toupper($varI2sTxRxConfiguration))"!], /*.IpTransmitReceiveMode*/
    { /*.IpConfig*/
[!IF "../../../GeneralConfiguration/I2sSaiSupport/I2sSaiEnable"!][!//
        [!"$varSaiConfig"!], /*.SaiIpConfig*/
[!ENDIF!][!//
[!IF "../../../GeneralConfiguration/I2sFlexioSupport/I2sFlexioI2sEnable"!][!//
        [!"$varFlexioMasterConfig"!], /*.FlexioMasterIpConfig*/
        [!"$varFlexioSlaveConfig"!] /*.FlexioSlaveIpConfig*/
[!ENDIF!][!//
    }
};
[!ENDCODE!][!//

        [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
[!ENDNOCODE!]
[!ENDMACRO!]


// IPW Cfg
[!MACRO "I2s_Ipw_Cfg" , "MacroGenerationType"!][!// GenerationType='definition' or 'declaration'
/**
* @brief          I2s Hw configuration array.
*/
[!NOCODE!][!//
    [!IF "$MacroGenerationType = 'declaration'"!]
        [!IF "./GeneralConfiguration/I2sMulticoreSupport"!][!//
            [!VAR "I2sConfiguredPartitions" = "num:i((count(./I2sEcucPartitionRef/*)))"!][!//
            [!IF "$I2sConfiguredPartitions > 0"!][!//
            [!LOOP "./I2sEcucPartitionRef/*"!][!//
            [!VAR "currentPartitionName"="node:value(.)"!]
            [!VAR "currentPartitionNumber" = "substring-after(substring-after(substring-after(substring-after(.,'/'),'/'),'/'),'/')"!][!//
[!CODE!][!//
extern const I2s_HwConfigType * const I2s_Ipw_apxHwConfigArr[!"$postBuildVariantNameUnderscore"!]_[!"$currentPartitionNumber"!][I2S_IPW_CONFIG_COUNT_[!"$currentPartitionNumber"!]];

[!ENDCODE!][!//

            [!ENDLOOP!]
            [!ENDIF!]
        [!ELSE!]
[!CODE!][!//
extern const I2s_HwConfigType * const I2s_Ipw_apxHwConfigArr[!"$postBuildVariantNameUnderscore"!][I2S_IPW_CONFIG_COUNT];

[!ENDCODE!][!//
        [!ENDIF!]
    [!ELSE!]

    [!IF "./GeneralConfiguration/I2sMulticoreSupport"!][!//
        [!VAR "I2sConfiguredPartitions" = "num:i((count(./I2sEcucPartitionRef/*)))"!][!//
        [!IF "$I2sConfiguredPartitions > 0"!][!//
        [!LOOP "./I2sEcucPartitionRef/*"!][!//
        [!VAR "currentPartitionName"="node:value(.)"!]
        [!VAR "currentPartitionNumber" = "substring-after(substring-after(substring-after(substring-after(.,'/'),'/'),'/'),'/')"!][!//

    [!CODE!][!//
const I2s_HwConfigType * const I2s_Ipw_apxHwConfigArr[!"$postBuildVariantNameUnderscore"!]_[!"$currentPartitionNumber"!][I2S_IPW_CONFIG_COUNT_[!"$currentPartitionNumber"!]] =
{
[!ENDCODE!][!//
            [!LOOP "./../../I2sConfigSet/I2sConfiguration/*"!][!//
                [!IF "$currentPartitionName = node:value(./I2sChannelEcucPartitionRef/*[1])"!][!//
[!CODE!][!//
    &I2s_Ipw_xHwConfig_[!"@index"!][!"$postBuildVariantNameUnderscore"!]_[!"$currentPartitionNumber"!],[!CR!][!ENDCODE!][!//
[!ENDIF!]
            [!ENDLOOP!]
[!CODE!][!//
};
[!ENDCODE!][!//
        [!ENDLOOP!]
        [!ENDIF!]
    [!ELSE!]
[!CODE!][!//
const I2s_HwConfigType * const I2s_Ipw_apxHwConfigArr[!"$postBuildVariantNameUnderscore"!][I2S_IPW_CONFIG_COUNT] =
{
[!ENDCODE!][!//
[!LOOP "I2sConfigSet/I2sConfiguration/*"!][!//
[!CODE!][!//
    &I2s_Ipw_xHwConfig_[!"@index"!][!"$postBuildVariantNameUnderscore"!],
[!ENDCODE!][!//
[!ENDLOOP!]
[!CODE!][!//
};
[!ENDCODE!][!//
    [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!]
[!ENDMACRO!]

[!MACRO "I2s_Structure_Config"!][!//
[!NOCODE!][!//
[!VAR "VariantsNo" = "variant:size()"!]
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "postBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!]
[!ELSE!]
    [!VAR "postBuildVariantNameUnderscore" = "string(null)"!]
[!ENDIF!]


/* Variant not aware (no support for variants or Precompile structures <=1 variant)​  - (“multicore & no variant”) */
[!IF "(IMPLEMENTATION_CONFIG_VARIANT !='VariantPostBuild' and ($VariantsNo <= 1))"!][!//
[!IF "./GeneralConfiguration/I2sMulticoreSupport"!][!//
    [!VAR "I2sConfiguredPartitions" = "num:i((count(./I2sEcucPartitionRef/*)))"!][!//
    [!IF "$I2sConfiguredPartitions > 0"!][!//
    [!LOOP "./I2sEcucPartitionRef/*"!][!//
    [!VAR "currentPartitionName"="node:value(.)"!]
    [!VAR "currentPartitionNumber" = "substring-after(substring-after(substring-after(substring-after(.,'/'),'/'),'/'),'/')"!][!//
[!CODE!][!//
/**
* @brief          I2s driver Configuration structure [!"$currentPartitionNumber"!].
*/
const I2s_ConfigType I2s_xConfig_[!"$currentPartitionNumber"!] =
{
    (I2s_HwUnitCount)I2S_IPW_CONFIG_COUNT_[!"$currentPartitionNumber"!], /*.cfgCount*/
    I2s_Ipw_apxHwConfigArr[!"$postBuildVariantNameUnderscore"!]_[!"$currentPartitionNumber"!],    /*.pHwConfig*/
};
[!ENDCODE!]
[!ENDLOOP!]
[!ENDIF!]
[!ELSE!]
/* Variant not aware and Ecuc not referenced/used - (“no multicore & no variant”) */
[!CODE!][!//
/**
* @brief          I2s driver Configuration structure [!"$postBuildVariantNameUnderscore"!].
*/
const I2s_ConfigType I2s_xConfig =
{
    (I2s_HwUnitCount)I2S_IPW_CONFIG_COUNT,  /*.cfgCount*/
    I2s_Ipw_apxHwConfigArr[!"$postBuildVariantNameUnderscore"!],    /*.pHwConfig*/
};
[!ENDCODE!]
[!ENDIF!]
[!ENDIF!]

[!IF "($VariantsNo > 1) or (IMPLEMENTATION_CONFIG_VARIANT = 'VariantPostBuild')"!][!//
[!IF "./GeneralConfiguration/I2sMulticoreSupport"!][!//
    [!VAR "I2sConfiguredPartitions" = "num:i((count(./I2sEcucPartitionRef/*)))"!][!//
    [!IF "$I2sConfiguredPartitions > 0"!][!//
    [!LOOP "./I2sEcucPartitionRef/*"!][!//
    [!VAR "currentPartitionName"="node:value(.)"!]
    [!VAR "currentPartitionNumber" = "substring-after(substring-after(substring-after(substring-after(.,'/'),'/'),'/'),'/')"!][!//
/* Variant-aware structures (PostBuild or PreCompile structures with >1 variant) - (“multicore & variants”) */
[!CODE!][!//
/**
* @brief          I2s driver Configuration structure [!"$postBuildVariantNameUnderscore"!]_[!"$currentPartitionNumber"!].
*/
const I2s_ConfigType I2s_xConfig[!"$postBuildVariantNameUnderscore"!]_[!"$currentPartitionNumber"!] =
{
    (I2s_HwUnitCount)I2S_IPW_CONFIG_COUNT_[!"$currentPartitionNumber"!],    /*.cfgCount*/
    I2s_Ipw_apxHwConfigArr[!"$postBuildVariantNameUnderscore"!]_[!"$currentPartitionNumber"!], /*.pHwConfig*/
};
[!ENDCODE!]
[!ENDLOOP!]
[!ENDIF!]
[!ELSE!]
/* Variant-aware and Ecuc not referenced/not used - (“variant & no multicore”)*/
[!CODE!][!//
/**
* @brief          I2s driver Configuration structure [!"$postBuildVariantNameUnderscore"!].
*/
const I2s_ConfigType I2s_xConfig[!"$postBuildVariantNameUnderscore"!] =
{
    (I2s_HwUnitCount)I2S_IPW_CONFIG_COUNT,   /*.cfgCount*/
    I2s_Ipw_apxHwConfigArr[!"$postBuildVariantNameUnderscore"!],   /*.pHwConfig*/
};
[!ENDCODE!]
[!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//



[!MACRO "I2s_Global_Pointer_Config"!][!//
[!NOCODE!][!//
[!VAR "I2sConfiguredPartitions" = "num:i((count(./I2sEcucPartitionRef/*)))"!][!//
[!IF "$I2sConfiguredPartitions!=0"!][!//
[!VAR "maxPartition" = "num:i(0)"!][!//
[!IF "node:value(GeneralConfiguration/I2sMulticoreSupport) = 'true'"!][!//
[!IF "node:exists(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition)"!][!//
    [!VAR "maxPartition" = "num:i(count(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition/*))"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!NOCODE!][!//
[!IF "(IMPLEMENTATION_CONFIG_VARIANT ='VariantPreCompile' and (variant:size()<=1))"!][!//
[!IF "($I2sConfiguredPartitions!=0) and (../GeneralConfiguration/I2sMulticoreSupport = 'true')"!][!//
    [!CODE!][!//
const I2s_ConfigType * const I2s_apxConfigVariantPredefined[I2S_MAX_PARTITIONS] =
{
    [!ENDCODE!][!//
    [!FOR "CurrentCoreId" = "0" TO "num:i($maxPartition) - 1"!][!//
        [!VAR "HasCore" = "0"!][!//
        [!VAR "HasPartition" = "0"!][!//
        [!VAR "foundPartition" = "0"!]  [!//
        [!LOOP "as:modconf('I2s')[1]/I2sEcucPartitionRef/*"!][!//
            [!IF "num:i($foundPartition) = 0"!][!//
                [!VAR "I2sEcucPartitionRefName" = "node:value(.)"!][!//
                [!VAR "PartitionIndex" = "node:name(node:value(.))"!][!//
                [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
                [!IF "$I2sEcucPartitionRefName = node:value(./OsAppEcucPartitionRef)"!][!//
                    [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
                    [!VAR "OsCoreId" = "node:value(./EcucCoreId)"!][!//
                        [!IF "num:i($OsCoreId) = num:i($CurrentCoreId)"!][!//
                            [!VAR "HasCore" = "1"!][!//
                            [!LOOP "//I2sConfiguration/*[1]"!][!//
                            [!LOOP "//I2sChannelEcucPartitionRef/*"!][!//
                            [!VAR "I2sChannelEcucPartitionRefName" = "node:value(.)"!] [!//
                            [!IF "$I2sChannelEcucPartitionRefName = $I2sEcucPartitionRefName"!][!//
                                [!VAR "HasPartition" = "1"!][!//
                                [!VAR "foundPartition" = "1"!][!//
                            [!ENDIF!][!//
                            [!ENDLOOP!][!//
                            [!ENDLOOP!][!//
                        [!ENDIF!][!//
                    [!ENDSELECT!][!//
                [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
        [!IF "$HasCore = 1"!][!//
            [!IF "$HasPartition = 1"!][!//
[!CODE!]
    /** @brief  Pointer to configuration structure of I2s for partition [!"$PartitionIndex"!] referred by core [!"num:i($CurrentCoreId)"!] */
    &I2s_xConfig_[!"$PartitionIndex"!][!//
[!ENDCODE!]
            [!ELSE!][!//
[!CODE!]
    /** @brief  No configuration structure of I2s referred by core [!"num:i($CurrentCoreId)"!] */
    NULL_PTR[!//
[!ENDCODE!]
            [!ENDIF!]
        [!ELSE!][!//
[!CODE!]
    /** @brief  No configuration structure of I2s referred by core [!"num:i($CurrentCoreId)"!] */
    NULL_PTR[!//
[!ENDCODE!]
        [!ENDIF!][!//
[!IF "$CurrentCoreId < num:i($maxPartition) - 1"!][!CODE!],[!ENDCODE!][!ENDIF!][!CR!]
    [!ENDFOR!]
[!CODE!]
};
[!ENDCODE!]
[!ELSE!]
[!CODE!][!//
const I2s_ConfigType * const I2s_pxConfigVariantPredefined =
{
[!ENDCODE!]
[!CODE!]    &I2s_xConfig
[!ENDCODE!]
[!CODE!]};[!ENDCODE!]
[!ENDIF!]
[!ENDIF!] /* precompile */
[!ENDNOCODE!]
[!ENDMACRO!]

// ----------------------------------------- HLD Configuration -----------------------------------------
[!MACRO "CDD_I2s_ConfigDef"!][!//
/**
* @brief  This define indicate that the I2s Dev Error Detection is enabled or disabled
*/
#define I2S_DEV_ERROR_DETECT           [!IF "GeneralConfiguration/I2sDevErrorDetect"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
/**
* @brief  This define indicate that the I2s Data Masking for transfer is enabled or disabled
*/
#define I2S_SAI_ENABLE_DATA_MASKING    (I2S_IPW_ENABLE_DATA_MASKING)
/**
* @brief  This define indicate that the I2s Clock will be disabled or enabled after transmission completed
*/
#define I2S_SAI_AUTO_DISABLE_CLOCK     (I2S_IPW_AUTO_DISABLE_CLOCK)
/**
* @brief  This define indicate that the I2s Version Information is enabled or disabled
*/
#define I2S_VERSION_INFO_API           [!IF "GeneralConfiguration/I2sVersionInfoApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
/**
* @brief  This define indicate that the I2s User Mode Support is enabled or disabled
*/
#define I2S_ENABLE_USER_MODE_SUPPORT   [!IF "GeneralConfiguration/I2sEnableUserModeSupport"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
/**
* @brief  This define indicate that the I2s Multicore Support is enabled or disabled
*/
#define I2S_MULTICORE_ENABLED          [!IF "GeneralConfiguration/I2sMulticoreSupport"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

[!ENDMACRO!]

[!MACRO "CDD_I2s_ChannelIdToIndex"!][!//
[!NOCODE!][!//
[!IF "./GeneralConfiguration/I2sMulticoreSupport"!][!//
[!CODE!]
#if (I2S_MULTICORE_ENABLED == STD_ON)
[!ENDCODE!]
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "VS_number" = "concat('_',$postBuildVariant)"!]
[!ELSE!]
    [!VAR "VS_number" = "''"!]
[!ENDIF!]
[!VAR "LogicalChannelNum" = "num:i(count(I2sConfigSet/I2sConfiguration/*))"!]
[!CODE!]const uint8 u8I2sChannelIdToCoreMap[I2S_NUM_CONFIG] =[!CR!]{[!CR!][!ENDCODE!][!//
[!FOR "I2sLogicalChannel" = "0" TO "num:i($LogicalChannelNum) - 1"!][!//
    [!VAR "CoreFound" = "0"!][!//
    [!LOOP "./I2sConfiguration/*"!][!//
        [!IF "num:i(node:value(./I2sLogicalChannelId)) = num:i($I2sLogicalChannel)"!][!//
            [!VAR "I2sChannelEcucPartitionRefName" = "node:value(./I2sChannelEcucPartitionRef/*[1])"!][!//
            [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
            [!IF "$I2sChannelEcucPartitionRefName = node:value(./OsAppEcucPartitionRef)"!][!//
                [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
                    [!VAR "OsCoreId" = "node:value(./EcucCoreId)"!][!//
                    [!CODE!]    [!"$OsCoreId"!]U,[!CR!][!ENDCODE!][!//
                    [!VAR "CoreFound" = "1"!][!//
                [!ENDSELECT!][!//
            [!ENDIF!][!//
            [!ENDLOOP!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
    [!IF "$CoreFound = 0"!]
        [!CODE!]    255U,[!CR!][!ENDCODE!][!//
    [!ENDIF!]
[!ENDFOR!][!//
[!CODE!]};[!CR!]#endif[!CR!][!ENDCODE!][!//
[!ENDIF!][!//
[!ENDNOCODE!]
[!ENDMACRO!]

/** @} */
[!ENDIF!]
