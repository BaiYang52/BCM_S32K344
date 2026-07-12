[!AUTOSPACING!]
[!NOCODE!]
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : DMA,CACHE,TRGMUX,LCU,EMIOS,FLEXIO
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

[!VAR "NumberEmiosInstance" = "num:i(count(MclConfig/EmiosCommon/*))"!]
[!VAR "NumberEmiosChannels" = "num:i(count(MclConfig/EmiosCommon/*/EmiosMclMasterBus/*))"!]


[!MACRO "Mcl_EmiosInitConfigTypeGen"!]
[!NOCODE!]

[!CODE!]
[!WS "0"!]/** 
[!WS "0"!]* @brief   Configuration for EMIOS initialization
[!WS "0"!]* @details This structure should contains data about the EMIOS common
[!WS "0"!]*          part configurated using Mcl driver
[!WS "0"!]* @implements Mcl_EmiosInitConfigType
[!WS "0"!]*/
[!WS "0"!]const Mcl_EmiosInitConfigType Mcl_EmiosConfig[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] = 
[!WS "0"!]{
[!WS "4"!]/** @brief  Number of EMIOS instaces to configure. */
[!WS "4"!](uint8)[!"num:i($NumberEmiosInstance)"!],
[!WS "4"!]/** @brief  Address of the EMIOS instances configuration. */
[!WS "4"!]&Mcl_EmiosInstanceConfig[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
};
[!ENDCODE!]

[!ENDNOCODE!]
[!ENDMACRO!]
[!NOCODE!]


[!ENDNOCODE!]
[!MACRO "Mcl_EmiosInstanceConfigTypeGen"!]
[!NOCODE!]

[!VAR "Mac_Comma" = "1"!]
[!CODE!]
[!WS "0"!]/** 
[!WS "0"!]* @brief   Configuration for EMIOS instance
[!WS "0"!]* @details This structure should contains data about the EMIOS common
[!WS "0"!]*          part configurated using Mcl driver
[!WS "0"!]* @implements Mcl_EmiosInstanceConfigType
[!WS "0"!]*/
[!WS "0"!]const Mcl_EmiosInstanceConfigType Mcl_EmiosInstanceConfig[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($NumberEmiosInstance)"!]U] = 
[!WS "0"!]{
[!ENDCODE!]
[!LOOP "MclConfig/EmiosCommon/*"!]
    [!CODE!]
[!WS "4"!]{
[!WS "8"!]/** @brief  Hardware EMIOS instace number */
[!WS "8"!](uint8) [!"text:split(EmiosMclInstances, '_')[2]"!]U,
[!WS "8"!]/** @brief  Address of configuration for EMIOS hardware instance */
[!WS "8"!]&Emios_Mcl_Ip_[!"text:split(node:value(./EmiosMclInstances), '_')[2]"!]_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
[!WS "4"!]}[!IF "(num:i($Mac_Comma)) < (num:i($NumberEmiosInstance))"!],[!ENDIF!][!VAR "Mac_Comma"="$Mac_Comma + 1"!]
    [!ENDCODE!]
[!ENDLOOP!]
[!CODE!]
[!WS "0"!]};
[!ENDCODE!]

[!ENDNOCODE!]    
[!ENDMACRO!]
[!NOCODE!]


[!ENDNOCODE!]

    

[!MACRO "Emios_Ip_GlobalConfigTypeGen"!]
[!NOCODE!]

[!LOOP "MclConfig/EmiosCommon/*"!]
    [!CODE!]
[!WS "0"!]/** 
[!WS "0"!]* @brief   Configuration for EMIOS IP instance
[!WS "0"!]* @details This structure should contains data about the EMIOS common
[!WS "0"!]*          part configurated using Mcl driver
[!WS "0"!]* @implements Emios_Ip_GlobalConfigType
[!WS "0"!]*/
[!WS "0"!]/* Emios instance configuration of global parameters */
[!WS "0"!]const Emios_Ip_GlobalConfigType Emios_Ip_[!"text:split(EmiosMclInstances, '_')[2]"!]_GlobalConfig[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] = 
[!WS "0"!]{
[!WS "4"!]/** @brief  Allow Debug Mode */
[!WS "4"!](boolean)[!"text:toupper(node:value(./EmiosMclEnableFreezState))"!],
[!WS "4"!]/** @brief  Clock Divider Value */
[!WS "4"!](uint8) [!"num:i(num:i(node:value(./EmiosMclClkDivVal)) - 1)"!]U,
[!WS "4"!]/** @brief  Enable Global Time Base */
[!WS "4"!](boolean)[!"text:toupper(node:value(./EmiosMclEnableGlobalTimeBase))"!]
[!WS "0"!]};
    [!ENDCODE!]
[!ENDLOOP!]

[!ENDNOCODE!]  
[!ENDMACRO!]


[!MACRO "Emios_Ip_MasterBusConfigTypeGen"!]
[!NOCODE!]

[!CODE!]
[!WS "0"!]/** 
[!WS "1"!]* @brief   Configuration for EMIOS master bus.
[!WS "1"!]* @details This structure contains data about the EMIOS master bus.
[!WS "1"!]*/
[!ENDCODE!]
[!LOOP "MclConfig/EmiosCommon/*"!]
    [!VAR "Mac_NumberOfMasterBuses" = "num:i(count(EmiosMclMasterBus/*))"!]
    [!VAR "Mac_NumberOfCommas" = "num:i(count(EmiosMclMasterBus/*) - 1)"!]

    [!CODE!][!WS "0"!]const Emios_Ip_MasterBusConfigType Emios_Mcl_Ip_[!"text:split(node:value(./EmiosMclInstances), '_')[2]"!]_MasterBusConfig[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"$Mac_NumberOfMasterBuses"!]U] = [!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]{[!CR!][!ENDCODE!][!//

    [!LOOP "EmiosMclMasterBus/*"!][!//
        [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!][!//
        [!CODE!][!WS "8"!]/** @brief EMIOS hardware channel used. */[!CR!][!ENDCODE!][!//
        [!CODE!][!WS "8"!](uint8)[!"node:value(./EmiosMclMasterBusNumber)"!],[!CR!][!ENDCODE!][!//
[!CODE!][!WS "8"!]/** @brief Master bus mode for use. */
[!WS "8"!]EMIOS_IP_[!"node:value(./EmiosMclMasterBusModeType)"!],
[!WS "8"!]/** @brief  Default period */
[!WS "8"!](uint32) [!"node:value(./EmiosMclDefaultPeriod)"!]U,
[!WS "8"!]/** @brief  Offset at start. */
[!WS "8"!](uint32) [!"node:value(./EmiosMclFirstOffsetValue)"!]U,
[!WS "8"!]/** @brief  Prescaler value */
[!WS "8"!](uint8) [!"num:i(num:i(text:split(./EmiosMclMasterBusPrescaler, '_')[2]) - 1)"!]U,
[!WS "8"!]/** @brief  Alternate prescaler value */
[!WS "8"!](uint8) [!"num:i(num:i(text:split(./EmiosMclMasterBusAltPrescaler, '_')[2]) - 1)"!]U,
[!WS "8"!]/** @brief  Channel Allow Debug Mode */
[!WS "8"!](boolean)[!"text:toupper(node:value(./EmiosMclChannelAllowDebugMode))"!]
[!ENDCODE!]
        [!CODE!][!WS "4"!]}[!ENDCODE!][!//
        [!IF "$Mac_NumberOfCommas > 0"!][!//
            [!VAR "Mac_NumberOfCommas"="$Mac_NumberOfCommas - 1"!][!//
            [!CODE!][!WS "0"!],[!CR!][!ENDCODE!][!//
        [!ELSE!][!//
            [!CODE!][!WS "0"!][!CR!][!ENDCODE!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
    
    [!CODE!][!WS "0"!]};[!CR!][!ENDCODE!][!//

[!ENDLOOP!]
    
[!ENDNOCODE!]
[!ENDMACRO!]


[!MACRO "Emios_Mcl_Ip_ConfigTypeGen"!]
[!NOCODE!]

[!CODE!]
[!WS "0"!]/** 
[!WS "1"!]* @brief   Configuration for EMIOS initialization.
[!WS "1"!]* @details This structure contains data about one instance of EMIOS.
[!WS "1"!]*/
[!ENDCODE!]

[!VAR "Mac_EmiosMulticoreSupport" = "node:value(./MclGeneral/MclEnableMulticoreSupport)"!]
[!VAR "Mac_OutRespectiveCoreId" = "0"!]
[!LOOP "MclConfig/EmiosCommon/*"!]
    [!VAR "Mac_NumberOfMasterBuses" = "num:i(count(EmiosMclMasterBus/*))"!]

    [!CODE!][!WS "0"!]const Emios_Mcl_Ip_ConfigType Emios_Mcl_Ip_[!"text:split(node:value(./EmiosMclInstances), '_')[2]"!]_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] = [!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]{[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]/** @brief Number of master buses used by this EMIOS IP. */[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!](uint8)[!"$Mac_NumberOfMasterBuses"!],[!CR!][!ENDCODE!][!//

    [!IF "$Mac_EmiosMulticoreSupport"!][!//
        [!VAR "Mac_EmiosPartRefName" = "node:value(./EmiosCommonEcucPartitionRef)"!][!//
        [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
            [!IF "$Mac_EmiosPartRefName = node:value(./OsAppEcucPartitionRef)"!][!//
                [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
                    [!VAR "Mac_OutRespectiveCoreId" = "node:value(./EcucCoreId)"!][!//
                [!ENDSELECT!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
        [!CODE!][!WS "4"!]/** @brief Core where EMIOS instance will be used. */[!CR!][!ENDCODE!][!//
        [!CODE!][!WS "4"!](uint8)[!"$Mac_OutRespectiveCoreId"!],[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//

    [!CODE!][!WS "4"!]/** @brief EMIOS instance configuration. */[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]&Emios_Ip_[!"text:split(EmiosMclInstances, '_')[2]"!]_GlobalConfig[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],[!CR!][!ENDCODE!][!//

    [!CODE!][!WS "4"!]/** @brief EMIOS master buses configuration. */[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]&Emios_Mcl_Ip_[!"text:split(node:value(./EmiosMclInstances), '_')[2]"!]_MasterBusConfig[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!CR!][!ENDCODE!][!//
    
    [!CODE!][!WS "0"!]};[!CR!][!ENDCODE!][!//
[!ENDLOOP!][!//
    
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!ENDNOCODE!][!//
