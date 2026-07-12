[!AUTOSPACING!][!//
[!CODE!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Emios Siul2 Wkpu LpCmp
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
 *   @file    Emios_Icu_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c
 *   @version 2.0.1
 *
 *   @brief   AUTOSAR Icu - contains the data exported by the ICU module.
 *   @details Contains the information that will be exported by the module, as requested by AUTOSAR.
 *
 *   @addtogroup emios_icu_ip EMIOS IPL
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif
[!NOCODE!]

/*==================================================================================================
 *                              GENERATION MACRO DEFINTION 
 *================================================================================================*/
[!//MACRO for generating the notification function prototype
[!MACRO "ICU_GENERATE_NOTIFICATION_TIMESTAMP"!][!//
[!NOCODE!]
    [!VAR "MacOuterLoopCounter" = "0"!]
    [!VAR "MacInnerLoopCounter" = "0"!]
    [!VAR "Found" = "'false'"!]
    [!SELECT "IcuConfigSet"!]
     [!LOOP "./IcuChannel/*"!]
        [!VAR "a" = "IcuMeasurementMode"!]
        [!IF "(contains($a,'ICU_MODE_TIMESTAMP') and (IcuTimestampMeasurement/IcuTimestampNotification != 'NULL_PTR') and (IcuTimestampMeasurement/IcuTimestampNotification != 'NULL'))"!]
            [!VAR "MacOuterLoopCounter" = "$MacOuterLoopCounter + 1"!]
            [!VAR "MacInnerLoopCounter" = "0"!]
            [!VAR "MacMatchcounter" = "0"!]
            [!VAR "MacNotification" = "IcuTimestampMeasurement/IcuTimestampNotification"!]
            [!LOOP "../../../*/IcuChannel/*"!] [!//All notifications from all configurations
                [!VAR "a" = "IcuMeasurementMode"!]
                [!IF "(contains($a,'ICU_MODE_TIMESTAMP') and (IcuTimestampMeasurement/IcuTimestampNotification != 'NULL_PTR')and (IcuTimestampMeasurement/IcuTimestampNotification != 'NULL'))"!]
                    [!VAR "MacInnerLoopCounter" = "$MacInnerLoopCounter + 1"!]
                    [!IF "$MacOuterLoopCounter >= $MacInnerLoopCounter"!]
                        [!IF "($MacNotification = IcuTimestampMeasurement/IcuTimestampNotification)"!]
                             [!VAR "MacMatchcounter" = "$MacMatchcounter + 1"!]
                        [!ENDIF!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDLOOP!]
            [!IF "$MacMatchcounter = 1"!]
                  [!IF "$Found = 'false'"!]
                      [!VAR "Found" = "'true'"!]
                      [!CODE!]/** @brief External Notifications for Timestamp */ [!CR!][!ENDCODE!]
                  [!ENDIF!]
                  [!CODE!]extern void [!"IcuTimestampMeasurement/IcuTimestampNotification"!](void);[!CR!][!ENDCODE!]
            [!ENDIF!]
        [!ENDIF!]
     [!ENDLOOP!]
    [!ENDSELECT!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//    
 
[!MACRO "ICU_CHECK_EMIOS_CHANNEL_VS_MASTERBUS_CH"!]
[!NOCODE!]
[!IF "node:exists(as:modconf('Mcl')[1]/MclGeneral/MclEmiosCommon/MclEnableEmiosCommon)"!]
    [!IF "node:value(as:modconf('Mcl')[1]/MclGeneral/MclEmiosCommon/MclEnableEmiosCommon) = 'true'"!]
        [!SELECT "IcuConfigSet"!]
            [!LOOP "./IcueMios/*"!]
                [!VAR "icuEmiosModule" = "concat('EMIOS_', node:value(./IcueMiosModule))"!]
                [!LOOP "./IcueMiosChannels/*"!]
                    [!VAR "icuEmiosChannel" = "concat('CH_',node:value(./IcueMiosChannel))"!]
                    [!VAR "eMiosChannelName" = "concat($icuEmiosModule, '_', $icuEmiosChannel)"!]
                    [!LOOP "as:modconf('Mcl')[1]/MclConfig/EmiosCommon/*"!]
                        [!VAR "mclEmiosModule" = "node:value(./EmiosMclInstances)"!]
                        [!LOOP "./EmiosMclMasterBus/*"!]
                            [!VAR "mclEmiosChannel" = "concat('CH_', node:value(./EmiosMclMasterBusNumber))"!]
                            [!VAR "masterbusChannelName" = "concat($mclEmiosModule, '_', $mclEmiosChannel)"!]
                            [!IF "($eMiosChannelName = $masterbusChannelName) "!]
                                [!ERROR!] ICU cannot used [!"$eMiosChannelName"!] when MCL use this channel as masterbus in variant "[!"variant:name( )"!]"" [!ENDERROR!]
                            [!ENDIF!]
                        [!ENDLOOP!]
                    [!ENDLOOP!]
                [!ENDLOOP!]
            [!ENDLOOP!]
        [!ENDSELECT!]
    [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!]
[!ENDMACRO!]

[!// MACRO for get prescaler for Master Bus
[!MACRO "GetMasterPrescaler", "parameMiosBus", "parameMiosChannel"!]
[!NOCODE!]
    [!IF "($parameMiosBus = 'EMIOS_ICU_BUS_A')"!]
        [!VAR "eMiosMaster" = "23"!]
    [!ELSEIF "($parameMiosBus = 'EMIOS_ICU_BUS_F')"!]
        [!VAR "eMiosMaster" = "22"!]
    [!ELSEIF "($parameMiosBus = 'EMIOS_ICU_BUS_DIVERSE')"!]
        [!IF "($parameMiosChannel < 8)"!][!//
            [!VAR "eMiosMaster" = "0"!]
        [!ELSEIF "($parameMiosChannel < 16)"!]
            [!VAR "eMiosMaster" = "8"!]
        [!ELSEIF "($parameMiosChannel < 24)"!]
            [!VAR "eMiosMaster" = "16"!]
        [!ELSE!]
            [!VAR "eMiosMaster" = "24"!]
        [!ENDIF!]
    [!ENDIF!]
    
    [!IF "num:i(count(as:modconf('Mcl')[1]/MclConfig/EmiosCommon/*[node:value(EmiosMclInstances) = text:concat('EMIOS_', node:current()/../../IcueMiosModule)]/EmiosMclMasterBus/*[EmiosMclMasterBusNumber = num:i($eMiosMaster)])) = 0"!]
        [!ERROR!] From variant "[!"variant:name( )"!]", EMIOS_[!"node:value(node:current()/../../IcueMiosModule)"!]_CH_[!"node:value(node:current()/IcueMiosChannel)"!] using master bus, so masterbus must be configured in the MCL module.[!ENDERROR!]
    [!ELSE!]
        [!IF "node:value(as:modconf('Mcl')[1]/MclConfig/EmiosCommon/*[node:value(EmiosMclInstances) = text:concat('EMIOS_', node:current()/../../IcueMiosModule)]/EmiosMclMasterBus/*[EmiosMclMasterBusNumber = num:i($eMiosMaster)]/EmiosMclMasterBusModeType) != 'MCB_UP_COUNTER'"!]
            [!ERROR!] From variant "[!"variant:name( )"!]", node "[!"node:name(as:modconf('Mcl')[1]/MclConfig/EmiosCommon/*[node:value(EmiosMclInstances) = text:concat('EMIOS_', node:current()/../../IcueMiosModule)]/EmiosMclMasterBus/*[EmiosMclMasterBusNumber = num:i($eMiosMaster)]/EmiosMclMasterBusModeType)"!]",  ICU only using counter mode 'MCB_UP_COUNTER' [!ENDERROR!]
        [!ENDIF!]
        
        [!IF "num:i(node:value(as:modconf('Mcl')[1]/MclConfig/EmiosCommon/*[node:value(EmiosMclInstances) = text:concat('EMIOS_', node:current()/../../IcueMiosModule)]/EmiosMclMasterBus/*[EmiosMclMasterBusNumber = num:i($eMiosMaster)]/EmiosMclDefaultPeriod)) != num:i(65535)"!]
            [!ERROR!] From variant "[!"variant:name( )"!]", node "[!"node:name(as:modconf('Mcl')[1]/MclConfig/EmiosCommon/*[node:value(EmiosMclInstances) = text:concat('EMIOS_', node:current()/../../IcueMiosModule)]/EmiosMclMasterBus/*[EmiosMclMasterBusNumber = num:i($eMiosMaster)]/EmiosMclDefaultPeriod)"!]",  ICU only using MasterBusCounterMaxDefault = 65535 [!ENDERROR!]
        [!ENDIF!]
        
        [!VAR "PrescalerVal" = "concat( 'EMIOS_PRESCALER_DIVIDE_', text:split(node:value(as:modconf('Mcl')[1]/MclConfig/EmiosCommon/*[node:value(EmiosMclInstances) = text:concat('EMIOS_', node:current()/../../IcueMiosModule)]/EmiosMclMasterBus/*[EmiosMclMasterBusNumber = num:i($eMiosMaster)]/EmiosMclMasterBusPrescaler), '_')[2])"!]
        
        [!VAR "PrescalerVal_Alternate" = "concat( 'EMIOS_PRESCALER_DIVIDE_', text:split(node:value(as:modconf('Mcl')[1]/MclConfig/EmiosCommon/*[node:value(EmiosMclInstances) = text:concat('EMIOS_', node:current()/../../IcueMiosModule)]/EmiosMclMasterBus/*[EmiosMclMasterBusNumber = num:i($eMiosMaster)]/EmiosMclMasterBusAltPrescaler), '_')[2])"!]
        
        
    [!ENDIF!]
[!ENDNOCODE!]
[!ENDMACRO!]

[!// MACRO for creating the IP configuration structure(s)
[!MACRO "ICU_GENERATE_IP_CONFIGURATION","VARIANT"="'PB'"!][!//
[!NOCODE!]
    
[!SELECT "IcuConfigSet"!]
[!IF "count(IcueMios/*) > '0'"!]
    [!LOOP "IcueMios/*"!]
        [!CODE!]/*[!CR!][!ENDCODE!]
        [!CODE!] *  @brief    [!"$VARIANT"!] [!"node:value(./IcueMiosModule)"!] Channels Configuration [!CR!][!ENDCODE!]
        [!CODE!] */[!CR!][!ENDCODE!]
        [!CODE!]const eMios_Icu_Ip_ChannelConfigType eMios_Icu_Ip_[!"node:value(./IcueMiosModule)"!]_ChannelConfig_[!"$VARIANT"!][[!"num:i(count(IcueMiosChannels/*))"!]U] =[!ENDCODE!]
        [!CODE!][!CR!]{[!CR!][!ENDCODE!]
        [!VAR "comma" = "num:i(count(IcueMiosChannels/*))"!]
            [!LOOP "IcueMiosChannels/*"!]
                [!CODE!][!WS "4"!]/** @brief [!"node:name(.)"!] */[!CR!][!ENDCODE!]
                [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]

                [!CODE!][!WS "8"!]/** @brief EMIOS hardware channel used by the ICU driver. */[!CR!][!ENDCODE!][!//
                [!CODE!][!WS "8"!][!"node:value(./IcueMiosChannel)"!]U,[!CR!][!ENDCODE!]

                [!CODE!][!WS "8"!]/** @brief Channel DutyCycle measurement mode. */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]EMIOS_ICU_[!"node:value(./IcuSubModeforMeasurement)"!],[!CR!][!ENDCODE!]

                [!CODE!][!WS "8"!]/** @brief Emios Channel Freeze bit. */[!CR!][!ENDCODE!][!//
                [!CODE!][!WS "8"!](boolean)[!"text:toupper(node:value(./IcuEmiosFreeze))"!],[!CR!][!ENDCODE!]

                [!IF "(contains(IcuEmiosBusSelect,'EMIOS_ICU_BUS_INTERNAL_COUNTER'))"!]
                    [!CODE!][!WS "8"!]/** @brief Value of EMIOS channel prescaler. */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!][!"node:value(./IcuEmiosPrescaler)"!],[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief Value of EMIOS channel alternate prescaler. */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!][!"node:value(./IcuEmiosPrescaler_Alternate)"!],[!CR!][!ENDCODE!] 
                [!ELSE!]
                    [!CALL "GetMasterPrescaler", "parameMiosBus" = "IcuEmiosBusSelect", "parameMiosChannel" = "IcueMiosChannel"!]
                    [!CODE!][!WS "8"!]/** @brief Channel Prescaler */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!][!"$PrescalerVal"!],[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief Channel Alternate Prescaler */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!][!"$PrescalerVal_Alternate"!],[!CR!][!ENDCODE!] 
                [!ENDIF!]
                
                [!CODE!][!WS "8"!]/** @brief Channel Counter bus selection. */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!][!"node:value(./IcuEmiosBusSelect)"!],[!CR!][!ENDCODE!]
                
                [!CODE!][!WS "8"!]/** @brief eMios IPL mode of operation */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]EMIOS_[!"node:value(../../../../IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuMeasurementMode)"!],[!CR!][!ENDCODE!]

                [!CODE!][!WS "8"!]/** @brief DMA support for timestamp measurement. */[!CR!][!ENDCODE!]
                [!IF "node:value(../../../../IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuDMAChannelEnable)"!]
                    [!CODE!][!WS "8"!]EMIOS_ICU_MODE_WITH_DMA,[!CR!][!ENDCODE!]
                [!ELSE!]
                    [!CODE!][!WS "8"!]EMIOS_ICU_MODE_WITHOUT_DMA,[!CR!][!ENDCODE!]
                [!ENDIF!]

                [!CODE!][!WS "8"!]/** @brief Measurement mode for signal measurement. */[!CR!][!ENDCODE!]
                [!IF "node:exists(../../../../IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuSignalMeasurement) and
                      (text:split(node:value(../../../../IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuMeasurementMode), 'MODE_')[2] = 'SIGNAL_MEASUREMENT')"!]
                    [!CODE!][!WS "8"!]EMIOS_[!"node:value(../../../../IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuSignalMeasurement/IcuSignalMeasurementProperty)"!],[!CR!][!ENDCODE!]
                [!ELSE!]
                    [!CODE!][!WS "8"!]EMIOS_ICU_NO_MEASUREMENT,[!CR!][!ENDCODE!]
                [!ENDIF!]

                [!CODE!][!WS "8"!]/** @brief Edge alignment for signal measurement. */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]EMIOS_[!"node:value(../../../../IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuDefaultStartEdge)"!],[!CR!][!ENDCODE!]

                [!CODE!][!WS "8"!]/** @brief Value of EMIOS channel digital filter. */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!][!"node:value(./IcuEmiosDigitalFilter)"!],[!CR!][!ENDCODE!]                
                
                [!CODE!][!WS "8"!]/** @brief Callback function for channels. */[!CR!][!ENDCODE!]
                [!IF "(node:value(../../../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuMeasurementMode) = 'ICU_MODE_SIGNAL_EDGE_DETECT') or
                        (((node:value(../../../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuMeasurementMode) = 'ICU_MODE_SIGNAL_MEASUREMENT') or
                         (node:value(../../../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuMeasurementMode) = 'ICU_MODE_TIMESTAMP')) and
                         (node:value(../../../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuOverflowNotification) != 'NULL_PTR'))"!]
                    [!CODE!][!WS "8"!]&Icu_ReportEvents,[!CR!][!ENDCODE!]
                [!ELSEIF "node:value(../../../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuMeasurementMode) = 'ICU_MODE_EDGE_COUNTER'"!]
                    [!CODE!][!WS "8"!]&Icu_ReportWakeupAndOverflow,[!CR!][!ENDCODE!][!//
                [!ELSE!][!//
                    [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!][!//
                [!ENDIF!][!//
                
                [!CODE!][!WS "8"!]/** @brief Callback function for changing channels status */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]&Icu_LogicChStateCallback,[!CR!][!ENDCODE!]
                
                [!CODE!][!WS "8"!]/** @brief Parameters used by callback function. */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!](uint8)[!"node:value(../../../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(IcuChannelRef)) = as:path(node:current())]/IcuChannelId)"!]U,[!CR!][!ENDCODE!]
                
                [!CODE!][!WS "8"!]/** @brief Channel perform measurements without interrupt. */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!](boolean)[!"text:toupper(node:value(./IcuSignalMeasureWithoutInterrupt))"!],[!CR!][!ENDCODE!]

                [!CODE!][!WS "0"!]#if (STD_ON == EMIOS_ICU_IP_TIMESTAMP_API)[!CR!][!ENDCODE!][!//
                [!CODE!][!WS "8"!]/** @brief Timestamp buffer used. */[!CR!][!ENDCODE!][!//
                [!IF "(node:value(../../../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuMeasurementMode) = 'ICU_MODE_TIMESTAMP')"!]
                    [!CODE!][!WS "8"!]EMIOS_[!"node:value(../../../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(IcuChannelRef)) = as:path(node:current())]/IcuTimestampMeasurement/IcuTimestampMeasurementProperty)"!],[!CR!][!ENDCODE!]
                [!ELSE!]
                    [!CODE!][!WS "8"!]EMIOS_ICU_NO_TIMESTAMP,[!CR!][!ENDCODE!]
                [!ENDIF!]
                [!CODE!][!WS "0"!]#endif[!CR!][!ENDCODE!][!//                

                [!IF "((node:value(../../../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuMeasurementMode) = 'ICU_MODE_TIMESTAMP') and 
                      (node:exists(../../../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuTimestampMeasurement/IcuTimestampNotification)))"!]
                    [!IF "(node:value(../../../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuTimestampMeasurement/IcuTimestampNotification) = 'NULL_PTR')"!]
                        [!CODE!][!WS "8"!]/** @brief Channel specific notification */[!CR!][!ENDCODE!][!//
                        [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!][!//
                    [!ELSE!]
                        [!CODE!][!WS "8"!]/** @brief Channel specific notification */[!CR!][!ENDCODE!][!//
                        [!CODE!][!WS "8"!]&[!"node:value(../../../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuTimestampMeasurement/IcuTimestampNotification)"!],[!CR!][!ENDCODE!][!//
                    [!ENDIF!]
                [!ELSE!]
                        [!CODE!][!WS "8"!]/** @brief Channel specific notification */[!CR!][!ENDCODE!][!//
                        [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!][!//
                [!ENDIF!]
                
                [!CODE!][!WS "8"!]/** @brief Channel overflow notification. */[!CR!][!ENDCODE!][!//
                [!CODE!][!WS "8"!]NULL_PTR[!CR!][!ENDCODE!][!//
                
                [!CODE!][!WS "4"!]}[!ENDCODE!]
                [!IF "(num:i($comma))>1"!][!CODE!],[!CR!][!ENDCODE!][!VAR "comma"="$comma - 1"!][!ELSE!][!CODE!][!CR!][!ENDCODE!][!ENDIF!]
            [!ENDLOOP!]
        [!CODE!]};[!CR!][!CR!][!ENDCODE!]
    [!ENDLOOP!]
[!ENDIF!]
[!ENDSELECT!]

[!SELECT "IcuConfigSet"!]
[!IF "count(IcueMios/*) > '0'"!]
    [!LOOP "IcueMios/*"!][!//
        [!CODE!]/** @brief Emios [!"$VARIANT"!] instance [!"node:value(./IcueMiosModule)"!] IP configuration. */[!CR!][!ENDCODE!][!//
        [!CODE!]const eMios_Icu_Ip_ConfigType eMios_Icu_Ip_[!"node:value(./IcueMiosModule)"!]_Config_[!"$VARIANT"!] = [!ENDCODE!][!//
        [!CODE!][!CR!]{[!CR!][!ENDCODE!]
            [!CODE!][!WS "4"!]/** @brief Number of channels in the configuration */[!CR!][!ENDCODE!]
            [!CODE!][!WS "4"!](uint8) [!"num:i(count(IcueMiosChannels/*))"!]U,[!CR!][!ENDCODE!]
            [!CODE!][!WS "4"!]/** @brief Instance Channels Config */[!CR!][!ENDCODE!]
            [!CODE!][!WS "4"!]&eMios_Icu_Ip_[!"node:value(./IcueMiosModule)"!]_ChannelConfig_[!"$VARIANT"!][!ENDCODE!]
        [!CODE!][!CR!]};[!CR!][!CR!][!ENDCODE!]
    [!ENDLOOP!]
[!ENDIF!]
[!ENDSELECT!]

[!ENDNOCODE!]
[!ENDMACRO!][!// End of Macro ICU_GENERATE_IP_CONFIGURATION

[!ENDNOCODE!][!//

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
*================================================================================================*/
#include "Emios_Icu_Ip_Types.h"

/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 *================================================================================================*/
#define EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C                    43
#define EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C     4
#define EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C     4
#define EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C  0
#define EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C             2
#define EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C             0
#define EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C             1

/*==================================================================================================
 *                                      FILE VERSION CHECKS
 *================================================================================================*/
#if (EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C != EMIOS_ICU_IP_TYPES_VENDOR_ID)
    #error "Emios_Icu_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Emios_Icu_Ip_Types.h have different vendor ids"
#endif

/* Check if  source file and Emios_Icu_Ip_Types.h file are of the same Autosar version */
#if ((EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C != EMIOS_ICU_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C != EMIOS_ICU_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C != EMIOS_ICU_IP_TYPES_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Emios_Icu_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Emios_Icu_Ip_Types.h are different"
#endif

/* Check if source file and Emios_Icu_Ip_Types.h file are of the same Software version */
#if ((EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C != EMIOS_ICU_IP_TYPES_SW_MAJOR_VERSION) || \
     (EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C != EMIOS_ICU_IP_TYPES_SW_MINOR_VERSION) || \
     (EMIOS_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C != EMIOS_ICU_IP_TYPES_SW_PATCH_VERSION))
    #error "Software Version Numbers of Emios_Icu_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Emios_Icu_Ip_Types.h are different"
#endif

/*==================================================================================================
 *                                       GLOBAL FUNCTIONS
 *================================================================================================*/
#define ICU_START_SEC_CODE
#include "Icu_MemMap.h"

/** @brief Signature of report events interrupt function. */
extern void Icu_ReportEvents(uint16 Channel, boolean bOverflow);

/** @brief Signature of wakeup and overflow interrupt function. */
extern void Icu_ReportWakeupAndOverflow(uint16 Channel, boolean bOverflow);

/** @brief Signature of wakeup and overflow interrupt function. */
extern void Icu_LogicChStateCallback(uint16 logicChannel, uint8 mask, boolean set);

[!CALL "ICU_GENERATE_NOTIFICATION_TIMESTAMP"!]

#define ICU_STOP_SEC_CODE
#include "Icu_MemMap.h"

/*==================================================================================================
 *                                       GLOBAL CONSTANTS
 *================================================================================================*/
#define ICU_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Icu_MemMap.h"
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "PBCfg_Variant" = "concat('PB_', $postBuildVariant)"!]
[!ELSE!]
    [!VAR "PBCfg_Variant" = "'PB'"!]
[!ENDIF!]

[!IF "node:exists(as:modconf('Mcl')[1])"!]
    [!CALL "ICU_CHECK_EMIOS_CHANNEL_VS_MASTERBUS_CH"!]
[!ENDIF!]
[!CALL "ICU_GENERATE_IP_CONFIGURATION","VARIANT"="$PBCfg_Variant"!]

[!ENDNOCODE!]
#define ICU_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Icu_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
[!ENDCODE!][!//
