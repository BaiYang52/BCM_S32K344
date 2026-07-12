[!/* *** multiple inclusion protection *** */!]
[!IF "not(var:defined('I3C_REGOPERATIONS_M'))"!]
[!VAR "I3C_REGOPERATIONS_M"="'true'"!]
[!NOCODE!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : I3C
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
*   @addtogroup i3c_driver_config I3c Driver Configuration
*   @{
*/

[!ENDNOCODE!][!//
[!AUTOSPACING!]
// ----------------------------------------- IPL Configuration -----------------------------------------
// IP Master Config
// GenerationType='declaration_config' or 'declaration_callback' or 'definition_state' or 'definition_IbiAddressRegistry' or 'definition_DynamicAddressList' or 'definition_config'
[!MACRO "I3c_Ip_MasterConfig" , "MacroGenerationType"!][!//
[!NOCODE!][!//
[!VAR "VariantsNo" = "variant:size()"!]
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "postBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!]
[!ELSE!]
    [!VAR "postBuildVariantNameUnderscore" = "string(null)"!]
[!ENDIF!]
[!LOOP "I3cChannel/*"!][!//
[!IF "I3cMasterSlaveMode != 'SLAVE'"!]
    [!IF "$MacroGenerationType = 'declaration_config'"!]
[!CODE!]
/**
* @brief I3C Ip Master Config for configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!].
*/
extern const I3c_Ip_MasterConfigType I3c_Ip_MasterConfig[!"$postBuildVariantNameUnderscore"!];
[!ENDCODE!][!//
    [!ELSEIF "$MacroGenerationType = 'declaration_callback'"!]
[!CODE!]
[!IF "I3cMasterConfiguration/I3cMasterCallback != 'NULL_PTR'"!][!//
/**
* @brief I3C Master Callback Function.
*/
extern void [!"I3cMasterConfiguration/I3cMasterCallback"!](I3c_Ip_MasterEventType Event);[!//
[!ENDIF!]
[!IF "I3cMasterConfiguration/I3cMasterGetIbiAddressCallback != 'NULL_PTR'"!]
/**
* @brief I3C Master Callback Function to get the In-Band interrupt address.
*/
extern void [!"I3cMasterConfiguration/I3cMasterGetIbiAddressCallback"!](I3c_Ip_MasterEventType Event, uint8 IbiAddress);[!//
[!ENDIF!]
[!ENDCODE!][!//
    [!ELSEIF "$MacroGenerationType = 'definition_state'"!]
[!CODE!]
/**
* @brief I3C Ip Master State for configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!].
*/
static I3c_Ip_MasterStateType I3c_Ip_MasterState[!"$postBuildVariantNameUnderscore"!] =
{
    0U,                     /* .BufferSize default value. This will be overwritten at runtime. */
    NULL_PTR,               /* .TxDataBuffer default value. This will be overwritten at runtime. */
    NULL_PTR,               /* .RxDataBuffer default value. This will be overwritten at runtime. */
    I3C_IP_STATUS_SUCCESS,  /* .Status default value. This will be overwritten at runtime. */
    /* .TransferOption default values. These will be overwritten at runtime. */
    {
        0x00,                   /* .SlaveAddress */
        (boolean)TRUE,          /* .SendStop */
        I3C_IP_WRITE,           /* .Direction */
        I3C_IP_TRANSFER_BYTES,  /* .TransferSize */
        I3C_IP_BUS_TYPE_I2C     /* .BusType */
    },
[!IF "../../GeneralConfiguration/I3cEnableI3cfeatures = 'true'"!][!//
#if (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)
    [!IF "num:i(count(I3cMasterConfiguration/I3cDynamicAddressList/*)) > 0"!]I3c_Ip_DynamicAddressList[!"$postBuildVariantNameUnderscore"!][!ELSE!]NULL_PTR[!ENDIF!],    /* .DynamicAddressList default value. This can be overwritten at runtime. */
    [!"num:i(count(I3cMasterConfiguration/I3cDynamicAddressList/*))"!]U,    /* .DynamicAddressCount default value. This can be overwritten at runtime. */
#endif /* (STD_ON == I3C_IP_ENABLE_I3C_FEATURES )*/
[!ENDIF!][!//
    I3C_IP_USING_[!"I3cMasterConfiguration/I3cMasterTransferType"!],    /* .TransferType */
#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
    [!IF "(I3cMasterConfiguration/I3cMasterTransferType = 'DMA') and (node:exists(I3cMasterConfiguration/I3cMasterDmaTxChannel) = 'true')"!][!"node:value(node:ref(I3cMasterConfiguration/I3cMasterDmaTxChannel)/dmaLogicChannel_LogicName)"!][!ELSE!]0U[!ENDIF!],    /* .DmaTxChannel */
    [!IF "(I3cMasterConfiguration/I3cMasterTransferType = 'DMA') and (node:exists(I3cMasterConfiguration/I3cMasterDmaRxChannel) = 'true')"!][!"node:value(node:ref(I3cMasterConfiguration/I3cMasterDmaRxChannel)/dmaLogicChannel_LogicName)"!][!ELSE!]0U[!ENDIF!],    /* .DmaRxChannel */
#endif /* I3C_IP_DMA_FEATURE_AVAILABLE */
[!IF "../../GeneralConfiguration/I3cEnableI3cfeatures = 'true'"!][!//
[!IF "../../GeneralConfiguration/I3cEnableSlaveRequests = 'true'"!][!//
#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
    /* .IbiOptions */
    {
        (boolean)[!IF "I3cMasterConfiguration/I3cSendStop = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!],        /* .IbiSendStop default value. This can be overwritten at runtime. */
        (boolean)[!IF "I3cMasterConfiguration/I3cEmitAutoIbi = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!],        /* .EmitAutoIbi default value. This can be overwritten at runtime. */
        NULL_PTR        /* .IbiDataBuffer default value. This can be overwritten at runtime. */
    },
    I3C_IP_IBI_RESP_[!"I3cMasterConfiguration/I3cIbiResp"!],    /* .IbiResp */
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS )*/
[!ENDIF!][!//
[!ENDIF!][!//
    (I3c_Ip_MasterCallbackType)[!"I3cMasterConfiguration/I3cMasterCallback"!],    /* .MasterCallback */
[!IF "../../GeneralConfiguration/I3cEnableI3cfeatures = 'true'"!][!//
[!IF "../../GeneralConfiguration/I3cEnableSlaveRequests = 'true'"!][!//
#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
    (I3c_Ip_MasterGetIbiAddressCallback)[!"I3cMasterConfiguration/I3cMasterGetIbiAddressCallback"!]    /* .MasterGetIbiAddressCallback */
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS )*/
[!ENDIF!][!//
[!ENDIF!][!//
};[!//
[!ENDCODE!][!//
    [!ELSEIF "$MacroGenerationType = 'definition_IbiAddressRegistry'"!]
        [!IF "../../GeneralConfiguration/I3cEnableI3cfeatures = 'true'"!]
        [!IF "../../GeneralConfiguration/I3cEnableSlaveRequests = 'true'"!]
        [!VAR "maxI3cIbiAddressRegistry"="num:i(count(I3cMasterConfiguration/I3cIbiAddressRegistry/*))"!]
        [!IF "num:i($maxI3cIbiAddressRegistry) > 0"!]
[!CODE!]
#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
/**
* @brief I3C Ip IBI Address Registry for configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!].
*/
static uint8 I3c_Ip_IbiAddressRegistry[!"$postBuildVariantNameUnderscore"!][[!"num:i($maxI3cIbiAddressRegistry)"!]U] =
{
[!ENDCODE!][!//
            [!VAR "loopIdx"="0"!][!//
            [!LOOP "I3cMasterConfiguration/I3cIbiAddressRegistry/*"!][!//
                [!VAR "loopIdx" = "$loopIdx + 1"!][!//
[!CODE!][!//
    0x[!"text:toupper(text:replace(num:inttohex(I3cSlaveAddress), '0x', ''))"!]U[!IF "$loopIdx < num:i($maxI3cIbiAddressRegistry)"!],[!ENDIF!]
[!ENDCODE!][!//
            [!ENDLOOP!]
[!CODE!][!//
};
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS )*/
[!ENDCODE!][!//
        [!ENDIF!]
        [!ENDIF!]
        [!ENDIF!]
    [!ELSEIF "$MacroGenerationType = 'definition_DynamicAddressList'"!]
        [!IF "../../GeneralConfiguration/I3cEnableI3cfeatures = 'true'"!]
        [!VAR "maxI3cDynamicAddressList"="num:i(count(I3cMasterConfiguration/I3cDynamicAddressList/*))"!][!//
        [!IF "num:i($maxI3cDynamicAddressList) > 0"!]
[!CODE!]
#if (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)
/**
* @brief I3C Ip Dynamic Adress List for configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!].
*/
static uint8 I3c_Ip_DynamicAddressList[!"$postBuildVariantNameUnderscore"!][[!"num:i($maxI3cDynamicAddressList)"!]U] =
{
[!ENDCODE!][!//
            [!VAR "loopIdx"="0"!][!//
            [!LOOP "I3cMasterConfiguration/I3cDynamicAddressList/*"!][!//
                [!VAR "loopIdx" = "$loopIdx + 1"!][!//
[!CODE!][!//
    0x[!"text:toupper(text:replace(num:inttohex(I3cSlaveDynamicAddress), '0x', ''))"!]U[!IF "$loopIdx < num:i($maxI3cDynamicAddressList)"!],[!ENDIF!]
[!ENDCODE!][!//
            [!ENDLOOP!]
[!CODE!][!//
};
#endif /* (STD_ON == I3C_IP_ENABLE_I3C_FEATURES )*/
[!ENDCODE!][!//
        [!ENDIF!]
        [!ENDIF!]
    [!ELSE!]
[!CODE!]
/**
* @brief I3C Ip Master Config for configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!].
*/
const I3c_Ip_MasterConfigType I3c_Ip_MasterConfig[!"$postBuildVariantNameUnderscore"!] =
{

    I3C_IP_[!"I3cMasterConfiguration/I3cMasterEnable"!],    /* .MasterEnable */
    (boolean)[!IF "I3cMasterConfiguration/I3cDisableTimeout = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!],    /* .DisableTimeout */
    I3C_IP_MASTER_[!"I3cMasterConfiguration/I3cMasterHighKeeper"!],    /* .MasterHighKeeper */
    [!"I3cMasterConfiguration/I3cI2cBaud"!]U,    /* .I2cBaud */
    [!"I3cMasterConfiguration/I3cOpenDrainBaud"!]U,    /* .OpenDrainBaud */
    [!"I3cMasterConfiguration/I3cPushPullBaud"!]U,    /* .PushPullBaud */
    [!"I3cMasterConfiguration/I3cPushPullLow"!]U,    /* .PushPullLow */
    (boolean)[!IF "I3cMasterConfiguration/I3cOpenDrainHighPP = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!],    /* .OpenDrainHighPP */
    (boolean)[!IF "I3cMasterConfiguration/I3cOpenDrainStop = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!],    /* .OpenDrainStop */
    [!"I3cMasterConfiguration/I3cSkew"!]U,    /* .Skew */
[!IF "../../GeneralConfiguration/I3cEnableI3cfeatures = 'true'"!][!//
    (boolean)[!IF "I3cMasterConfiguration/I3cEnableStartOptimization = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!],    /* .EnableStartOptimization */
[!ELSE!][!//
    (boolean)FALSE,    /* .EnableStartOptimization */
[!ENDIF!][!//
[!IF "../../GeneralConfiguration/I3cEnableI3cfeatures = 'true'"!][!//
[!IF "../../GeneralConfiguration/I3cEnableSlaveRequests = 'true'"!][!//
    (boolean)[!IF "I3cMasterConfiguration/I3cNoMandatoryByte = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!],    /* .NoMandatoryByte */
#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
    [!IF "num:i(count(I3cMasterConfiguration/I3cIbiAddressRegistry/*)) > 0"!]I3c_Ip_IbiAddressRegistry[!"$postBuildVariantNameUnderscore"!][!ELSE!]NULL_PTR[!ENDIF!],    /* .IbiAddressRegistry */
    [!"num:i(count(I3cMasterConfiguration/I3cIbiAddressRegistry/*))"!]U,    /*.SlavesIbiByteCount */
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS )*/
[!ELSE!][!//
    (boolean)FALSE,    /* .NoMandatoryByte */
[!ENDIF!][!//
[!ELSE!][!//
    (boolean)FALSE,    /* .NoMandatoryByte */
[!ENDIF!][!//
    /* .MasterState */
    &I3c_Ip_MasterState[!"$postBuildVariantNameUnderscore"!]
};
[!ENDCODE!][!//
    [!ENDIF!]
[!ENDIF!]
[!ENDLOOP!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

// IP Slave Config
// GenerationType='declaration_config' or 'declaration_callback' or 'definition_state' or 'definition_IbiAddressRegistry' or 'definition_DynamicAddressList' or 'definition_config'
[!MACRO "I3c_Ip_SlaveConfig" , "MacroGenerationType"!][!//
[!NOCODE!][!//
[!VAR "VariantsNo" = "variant:size()"!]
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "postBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!]
[!ELSE!]
    [!VAR "postBuildVariantNameUnderscore" = "string(null)"!]
[!ENDIF!]
[!LOOP "I3cChannel/*"!][!//
[!IF "I3cMasterSlaveMode != 'MASTER'"!]
    [!IF "$MacroGenerationType = 'declaration_config'"!]
[!CODE!]
/**
* @brief I3C Ip Slave Config for configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!].
*/
extern const I3c_Ip_SlaveConfigType I3c_Ip_SlaveConfig[!"$postBuildVariantNameUnderscore"!];
[!ENDCODE!][!//
    [!ELSEIF "$MacroGenerationType = 'declaration_callback'"!]
[!CODE!]
[!IF "I3cSlaveConfiguration/I3cSlaveCallback != 'NULL_PTR'"!][!//
/**
* @brief I3C Slave Callback Function.
*/
extern void [!"I3cSlaveConfiguration/I3cSlaveCallback"!](I3c_Ip_SlaveEventType Event);
[!ENDIF!]
[!ENDCODE!][!//
    [!ELSEIF "$MacroGenerationType = 'definition_state'"!]
[!CODE!]
/**
* @brief I3C Ip Slave State for configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!].
*/
static I3c_Ip_SlaveStateType I3c_Ip_SlaveState[!"$postBuildVariantNameUnderscore"!] =
{
    0U,                     /* .BufferSize default value. This will be overwritten at runtime. */
    NULL_PTR,               /* .DataBuffer default value. This will be overwritten at runtime. */
    I3C_IP_STATUS_SUCCESS,  /* .Status default value. This will be overwritten at runtime. */
    0U,                     /* .RepeatedStarts default value. This will be overwritten at runtime. */
    I3C_IP_READ,            /* .Direction default value. This will be overwritten at runtime. */
    I3C_IP_TRANSFER_[!"I3cSlaveConfiguration/I3cTransferSize"!],    /* .TransferSize */
    I3C_IP_USING_[!"I3cSlaveConfiguration/I3cSlaveTransferType"!],    /* .TransferType */
[!IF "../../GeneralConfiguration/I3cEnableI3cfeatures = 'true'"!][!//
[!IF "../../GeneralConfiguration/I3cEnableSlaveRequests = 'true'"!][!//
#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
    (boolean)[!IF "I3cSlaveConfiguration/I3cExtendedIbiData = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!],    /* .ExtendedIbiData */
    [!"I3cSlaveConfiguration/I3cCountExtraBytes"!]U,    /* .CountExtraBytes */
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS )*/
[!ENDIF!][!//
[!ENDIF!][!//
#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
    [!IF "(I3cSlaveConfiguration/I3cSlaveTransferType = 'DMA') and (node:exists(I3cSlaveConfiguration/I3cSlaveDmaTxChannel) = 'true')"!][!"node:value(node:ref(I3cSlaveConfiguration/I3cSlaveDmaTxChannel)/dmaLogicChannel_LogicName)"!][!ELSE!]0U[!ENDIF!],    /* .DmaTxChannel */
    [!IF "(I3cSlaveConfiguration/I3cSlaveTransferType = 'DMA') and (node:exists(I3cSlaveConfiguration/I3cSlaveDmaRxChannel) = 'true')"!][!"node:value(node:ref(I3cSlaveConfiguration/I3cSlaveDmaRxChannel)/dmaLogicChannel_LogicName)"!][!ELSE!]0U[!ENDIF!],    /* .DmaRxChannel */
#endif /* I3C_IP_DMA_FEATURE_AVAILABLE */
    (I3c_Ip_SlaveCallbackType)[!"I3cSlaveConfiguration/I3cSlaveCallback"!]    /* .SlaveCallback */
};[!//
[!ENDCODE!][!//
    [!ELSE!]
[!CODE!]
/**
* @brief I3C Ip Slave Config for configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!].
*/
const I3c_Ip_SlaveConfigType I3c_Ip_SlaveConfig[!"$postBuildVariantNameUnderscore"!] =
{
    (boolean)[!IF "I3cSlaveConfiguration/I3cSlaveEnable = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!],    /* .SlaveEnable */
    (boolean)[!IF "I3cSlaveConfiguration/I3cNack = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!],    /* .Nack */
    (boolean)[!IF "I3cSlaveConfiguration/I3cMatchStartStop = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!],    /* .MatchStartStop*/
    (boolean)[!IF "I3cSlaveConfiguration/I3cIgnoreS0S1Error = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!],    /* .IgnoreS0S1Error */
[!IF "../../GeneralConfiguration/I3cEnableI3cfeatures = 'true'"!][!//
    (boolean)[!IF "I3cSlaveConfiguration/I3cHighDataRateSupport = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!],    /* .HighDataRateSupport */
[!ELSE!][!//
    (boolean)FALSE,    /* .HighDataRateSupport */
[!ENDIF!][!//
    (boolean)[!IF "I3cSlaveConfiguration/I3cOfflineSupport = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!],    /* .OfflineSupport */
    [!"I3cSlaveConfiguration/I3cBusAvailableMatch"!]U,    /* .BusAvailableMatch */
    [!"I3cSlaveConfiguration/I3cStaticAddress"!]U,    /* .StaticAddress */
    [!"I3cSlaveConfiguration/I3cBCR"!]U,    /* .BCR */
    [!"I3cSlaveConfiguration/I3cDCR"!]U,    /* .DCR */
    [!"I3cSlaveConfiguration/I3cVendorId"!]U,    /* .VendorId */
    [!"I3cSlaveConfiguration/I3cPartNumber"!]U,    /* .PartNumber */
    [!"I3cSlaveConfiguration/I3cVendorInfo"!]U,    /* .VendorInformation */
    [!"I3cSlaveConfiguration/I3cActState"!]U,    /* .ActivityState */
[!IF "../../GeneralConfiguration/I3cEnableI3cfeatures = 'true'"!][!//
[!IF "../../GeneralConfiguration/I3cEnableSlaveRequests = 'true'"!][!//
    [!"I3cSlaveConfiguration/I3cPendingInt"!]U,    /* .PendingInt */
[!ELSE!][!//
    0U,    /* .PendingInt */
[!ENDIF!][!//
[!ELSE!][!//
    0U,    /* .PendingInt */
[!ENDIF!][!//
    [!"I3cSlaveConfiguration/I3cMapIndex"!]U,    /* .MapIndex */
    [!"I3cSlaveConfiguration/I3cClockFrequency"!]U,    /* .ClockFrequency */
    [!"I3cSlaveConfiguration/I3cClockAccuracy"!]U,    /* .ClockAccuracy */
[!IF "../../GeneralConfiguration/I3cEnableI3cfeatures = 'true'"!][!//
[!IF "../../GeneralConfiguration/I3cEnableSlaveRequests = 'true'"!][!//
#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
    (boolean)[!IF "I3cSlaveConfiguration/I3cHotJoinDevice = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!],    /* .HotJoinDevice */
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS )*/
[!ENDIF!][!//
[!ENDIF!][!//

    &I3c_Ip_SlaveState[!"$postBuildVariantNameUnderscore"!]    /* .SlaveState */
};
[!ENDCODE!][!//
    [!ENDIF!]
[!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!]
[!ENDMACRO!][!//

// IP Config Def
[!MACRO "I3c_Ip_ConfigDef"!][!//
/**
* @brief  This define indicate that the I3c Dev Error Detection is enabled or disabled
*/
#define I3C_IP_DEV_ERROR_DETECT                [!IF "GeneralConfiguration/I3cDevErrorDetect"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief  This define indicate that the I3c DMA feature is enabled or disabled
*/
#define I3C_IP_DMA_FEATURE_AVAILABLE           [!IF "GeneralConfiguration/I3cDmaFeature"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief  This define indicate that the I3C Osif Timeout Method
*/
#define I3C_IP_TIMEOUT_TYPE                    ([!"GeneralConfiguration/I3cTimeoutMethod"!])

/**
* @brief  This define indicate that the I3C Timeout Value
*/
#define I3C_IP_TIMEOUT_VALUE                   ([!"GeneralConfiguration/I3cTimeoutVal"!]U)

/**
* @brief  This define indicate that the I3C Tx Fifo Size supported
*/
#define I3C_IP_TX_FIFO_SIZE                    ([!"ecu:get('I3c.TxFifoSize')"!]U)

/**
* @brief  This define indicate that the I3C Rx Fifo Size supported
*/
#define I3C_IP_RX_FIFO_SIZE                    ([!"ecu:get('I3c.RxFifoSize')"!]U)

/**
* @brief  This define indicate that the I3C maximum slave devices supported
*/
#define I3C_IP_MAX_DEVICES                     ([!"ecu:get('I3c.I3cMaxDevices')"!]U)

/**
* @brief  This define indicate that the I3C instance is used or not
*/
#define I3C_IP_0_AVAILABLE                     [!IF "num:i(ecu:get('I3c.Has.I3C0'))=1"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define I3C_IP_1_AVAILABLE                     [!IF "num:i(ecu:get('I3c.Has.I3C1'))=1"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

[!NOCODE!]
[!IF "num:i(ecu:get('I3c.Support.BitField.HDRMATCHED'))!=1"!]
[!CODE!][!//
/* Redefine these macros because they are modified naming in the header file  */
#define I3C_SSTATUS_HDRMATCHED_MASK            I3C_SSTATUS_HDRMATCH_MASK
[!ENDCODE!][!//
[!ENDIF!]

[!IF "num:i(ecu:get('I3c.Support.BitField.HDROK'))!=1"!]
[!CODE!]
/* Redefine these macros because they are modified naming in the header file  */
#define I3C_SCONFIG_HDROK_MASK                 I3C_SCONFIG_DDROK_MASK
#define I3C_SCONFIG_HDROK                      I3C_SCONFIG_DDROK
[!ENDCODE!][!//
[!ENDIF!][!//

[!IF "num:i(ecu:get('I3c.Support.BitField.SA10B'))!=1"!]
[!CODE!]
/*
* Define dummy macro to avoid compilation failed and these macros will not affect to static slave 10bits address
* Because SA10B bits always have value as zero on the device unsupportted SA10B bits
*/
#define I3C_SMAPCTRL1_SA10B_MASK               (0xE00U)
#define I3C_SMAPCTRL1_SA10B_SHIFT              (9U)
#define I3C_SMAPCTRL1_SA10B(x)                 (((uint32_t)(((uint32_t)(x)) << I3C_SMAPCTRL1_SA10B_SHIFT)) & I3C_SMAPCTRL1_SA10B_MASK)
[!ENDCODE!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//

/**
* @brief  This define indicate that Slave Requests feature is enabled or disabled.
*/
#define I3C_IP_ENABLE_SLAVE_REQUESTS           [!IF "GeneralConfiguration/I3cEnableI3cfeatures"!][!IF "GeneralConfiguration/I3cEnableSlaveRequests"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!][!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief  This define indicate that I3c features is enabled or disabled.
*/
#define I3C_IP_ENABLE_I3C_FEATURES             [!IF "GeneralConfiguration/I3cEnableI3cfeatures"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief  This define indicate that Init Dynamic Process Assignment is enabled or disabled.
*/
[!IF "GeneralConfiguration/I3cEnableI3cfeatures"!][!//
#define I3C_IP_ENABLE_DAA_INIT_TIME            [!IF "GeneralConfiguration/I3cInitDynamicProcessAssignment"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
[!ELSE!][!//
#define I3C_IP_ENABLE_DAA_INIT_TIME            (STD_OFF)
[!ENDIF!][!//
[!ENDMACRO!]


// ----------------------------------------- HLD Configuration -----------------------------------------
// HLD Config Def
[!MACRO "CDD_I3c_ConfigDef"!][!//
[!NOCODE!][!//
[!VAR "VariantsNo" = "variant:size()"!]
[!ENDNOCODE!][!//
/**
* @brief  This define indicate that the I3c Dev Error Detection is enabled or disabled
*/
#define I3C_DEV_ERROR_DETECT           [!IF "GeneralConfiguration/I3cDevErrorDetect"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief  This define indicate that the I3c Version Information is enabled or disabled
*/
#define I3C_VERSION_INFO_API           [!IF "GeneralConfiguration/I3cVersionInfoApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief  This define indicate that the I3c User Mode Support is enabled or disabled
*/
#define I3C_ENABLE_USER_MODE_SUPPORT   [!IF "GeneralConfiguration/I3cEnableUserModeSupport"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief  This define indicate that the I3c Multicore Support is enabled or disabled
*/
#define I3C_MULTICORE_SUPPORT          [!IF "GeneralConfiguration/I3cMulticoreSupport"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief   Total number of I3c channel configured.
*/
#define I3C_HW_MAX_CONFIG              ([!"num:i(count(I3cChannel/*))"!]U)

/**
* @brief   I3c hardware channel
*/
[!NOCODE!][!//
[!FOR "i" = "0" TO "num:i(ecu:get('I3c.I3cChannels'))-1"!]
[!CODE!]#define I3C_[!"$i"!]                          ([!"$i"!]U)[!ENDCODE!][!CR!]
[!ENDFOR!]
[!ENDNOCODE!][!//

/**
* @brief  This define indicate that Slave Requests feature is enabled or disabled.
*/
#define I3C_ENABLE_SLAVE_REQUESTS      (I3C_IP_ENABLE_SLAVE_REQUESTS)

/**
* @brief  This define indicate that I3c features is enabled or disabled.
*/
#define I3C_ENABLE_I3C_FEATURES        (I3C_IP_ENABLE_I3C_FEATURES)

/**
* @brief  This define indicate that Dynamic Address Assignment process is enabled/disabled at Init().
*/
#define I3C_ENABLE_DAA_INIT_TIME       (I3C_IP_ENABLE_DAA_INIT_TIME)

[!IF "($VariantsNo <= 1) and (IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild')"!][!//
/**
* @brief  This define indicate that the I3c precompile supported
*/
#define I3C_PRECOMPILE_SUPPORT
[!ENDIF!][!//
[!//
#ifdef I3C_ENABLE_USER_MODE_SUPPORT
    #if(STD_ON == I3C_ENABLE_USER_MODE_SUPPORT)
        /**
        * @brief  This define indicate that the ipv I3C is protected by register protection.
        */
        #define I3C_REG_PROT_AVAILABLE
    #endif /* (STD_ON == I3C_ENABLE_USER_MODE_SUPPORT) */
#endif /* ifdef I3C_ENABLE_USER_MODE_SUPPORT */

/**
* @brief  perform checking marco MCAL_ENABLE_USER_MODE_SUPPORT available or not.
*/
#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    #if (STD_ON == I3C_ENABLE_USER_MODE_SUPPORT)
        #error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running I3C in user mode, the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined
    #endif /* (STD_ON == I3C_ENABLE_USER_MODE_SUPPORT) */
#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT */
[!ENDMACRO!]

// HLD Config
[!MACRO "CDD_I3c_Config" , "MacroGenerationType"!][!// GenerationType='definition' or 'declaration' or 'definition_local' or 'definition_precompile'
[!NOCODE!][!//
[!VAR "VariantsNo" = "variant:size()"!]
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "postBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!]
[!ELSE!]
    [!VAR "postBuildVariantNameUnderscore" = "string(null)"!]
[!ENDIF!]
[!IF "$MacroGenerationType = 'declaration'"!]
    [!IF "not((IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1))"!][!//
        [!CODE!]#define I3C_CONFIG[!"$postBuildVariantNameUnderscore"!]_PB \[!CR!][!ENDCODE!][!///
        [!IF "$VariantsNo > 0"!][!//
            [!CODE!][!WS4!]extern const I3c_ConfigType I3c_Config[!"$postBuildVariantNameUnderscore"!];[!CR!][!ENDCODE!]
        [!ELSE!][!//
            [!CODE!][!WS4!]extern const I3c_ConfigType I3c_Config;[!CR!][!ENDCODE!]
        [!ENDIF!][!//
    [!ENDIF!]
[!ELSEIF "$MacroGenerationType = 'definition_local'"!]

[!CODE!]
/**
* @brief I3C HW Channel Config for configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!].
*/
[!ENDCODE!][!//
[!FOR "i" = "0" TO "num:i(ecu:get('I3c.I3cChannels'))-1"!]
    [!VAR "xPathModule" = "concat('I3cChannel/I3cChannel_',$i)"!]
    [!IF "node:exists(concat($xPathModule,'/I3cMasterSlaveMode'))"!]
    [!CODE!][!//
static const I3c_HwChannelConfigType I3c_[!"$i"!]_HwChannelConfig[!"$postBuildVariantNameUnderscore"!] =
{
    [!ENDCODE!][!//

    [!IF "node:value(concat($xPathModule,'/I3cMasterSlaveMode')) != 'SLAVE'"!]
        [!CODE!]&I3c_Ip_MasterConfig[!"$postBuildVariantNameUnderscore"!],    /* Pointer to the Master config structure */[!ENDCODE!][!CR!]
    [!ELSE!]
        [!CODE!]NULL_PTR,    /* Pointer to the Master config structure */[!ENDCODE!][!CR!]
    [!ENDIF!]
    [!CODE!][!//
    [!ENDCODE!][!//
    [!IF "node:value(concat($xPathModule,'/I3cMasterSlaveMode')) != 'MASTER'"!]
        [!CODE!]&I3c_Ip_SlaveConfig[!"$postBuildVariantNameUnderscore"!]/* Pointer to the Slave config structure */[!ENDCODE!][!CR!]
    [!ELSE!]
        [!CODE!]NULL_PTR    /* Pointer to the Slave config structure */[!ENDCODE!][!CR!]
    [!ENDIF!]
[!CODE!][!//
};
[!ENDCODE!][!//
    [!ENDIF!]
[!ENDFOR!]

[!CODE!]
/**
* @brief I3C HW Unit Config for configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!].
*/
[!ENDCODE!][!//
[!FOR "i" = "0" TO "num:i(ecu:get('I3c.I3cChannels'))-1"!]
    [!VAR "xPathModule" = "concat('I3cChannel/I3cChannel_',$i)"!]
    [!IF "node:exists(concat($xPathModule,'/I3cMasterSlaveMode'))"!]
    [!CODE!][!//
static const I3c_HwConfigType I3c_[!"$i"!]_HwConfig[!"$postBuildVariantNameUnderscore"!] =
{

    [!"node:value(concat($xPathModule,'/I3cHwChannel'))"!],    /* I3c hardware channel */
    I3C_[!"node:value(concat($xPathModule,'/I3cMasterSlaveMode'))"!]_MODE,    /* Module mode: master or slave */
    &I3c_[!"$i"!]_HwChannelConfig[!"$postBuildVariantNameUnderscore"!]    /* Pointer to the HWardware channel config structure */
};

[!ENDCODE!][!//
    [!ENDIF!]
[!ENDFOR!]

[!CODE!]
/**
* @brief I3C HW Unit array pointer for configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!].
*/
static const I3c_HwConfigType* const I3c_apxHwConfig[!"$postBuildVariantNameUnderscore"!][[!"num:i(count(I3cChannel/*))"!]] =
{
[!ENDCODE!][!//

[!FOR "i" = "0" TO "num:i(count(I3cChannel/*))-1"!]
    [!VAR "xPathModule" = "concat('I3cChannel/I3cChannel_',$i)"!]
    [!IF "node:exists(concat($xPathModule,'/I3cMasterSlaveMode'))"!]
        [!CODE!]    &I3c_[!"$i"!]_HwConfig[!"$postBuildVariantNameUnderscore"!],[!ENDCODE!][!CR!]
    [!ELSE!]
        [!CODE!]NULL_PTR,[!ENDCODE!][!CR!]
    [!ENDIF!]
[!ENDFOR!]

[!CODE!][!//
};
[!ENDCODE!][!//

[!ELSEIF "$MacroGenerationType = 'definition_precompile'"!]
    [!IF "($VariantsNo <= 1) and (IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild')"!][!//
[!CODE!][!// TODO: Multicore support
const I3c_ConfigType * const I3c_ConfigVariantPredefined[1U] =
{
    &I3c_Config
};
[!ENDCODE!][!//
    [!ENDIF!][!//
[!ELSE!]
[!CODE!][!//
/**
* @brief I3C Config for configuration variant [!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!].
*/
[!ENDCODE!][!//
[!IF "not((IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1))"!][!//
    [!IF "$VariantsNo > 0"!][!//
[!CODE!]const I3c_ConfigType I3c_Config[!"$postBuildVariantNameUnderscore"!] =[!ENDCODE!]
    [!ELSE!][!//
[!CODE!]const I3c_ConfigType I3c_Config =[!ENDCODE!]
    [!ENDIF!][!//
[!ELSE!][!//
[!CODE!]const I3c_ConfigType I3c_Config =[!ENDCODE!]
[!ENDIF!]
[!CODE!]
{
    0U,    /* The Partition Core Id */[!// TODO: Multicore support
    I3c_apxHwConfig[!"$postBuildVariantNameUnderscore"!]/* Pointer to the I3C Hardware Unit configuration. */
};[!//
[!ENDCODE!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//



/** @} */
[!ENDIF!]
