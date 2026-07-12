/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : SIUL2
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
*   @file    Siul2_Port_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!][!ENDIF!]_PBcfg.c
*
*   @addtogroup Port_CFG
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Siul2_Port_Ip.h"

[!NOCODE!][!//
[!/* Include package file in order to get valid Pins */!][!//
[!IF "((ecu:get('Port.Derivative'))='s32k312')"!][!//
    [!INCLUDE "Port_S32K312_Resource.m"!][!//
[!ELSEIF "((ecu:get('Port.Derivative'))='s32k314')"!][!//
    [!INCLUDE "Port_S32K314_Resource.m"!][!//
[!ELSEIF "((ecu:get('Port.Derivative'))='s32k324')"!][!//
    [!INCLUDE "Port_S32K324_Resource.m"!][!//
[!ELSEIF "((ecu:get('Port.Derivative'))='s32k342')"!][!//
    [!INCLUDE "Port_S32K342_Resource.m"!][!//
[!ELSEIF "((ecu:get('Port.Derivative'))='s32k341')"!][!//
    [!INCLUDE "Port_S32K341_Resource.m"!][!//
[!ELSEIF "((ecu:get('Port.Derivative'))='s32k322')"!][!//
    [!INCLUDE "Port_S32K322_Resource.m"!][!//
[!ELSEIF "((ecu:get('Port.Derivative'))='s32k344')"!][!//
    [!INCLUDE "Port_S32K344_Resource.m"!][!//
 [!ENDIF!][!//
[!ENDNOCODE!][!//

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define SIUL2_PORT_IP_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C                       43
#define SIUL2_PORT_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C        4
#define SIUL2_PORT_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C        4
#define SIUL2_PORT_IP_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C     0
#define SIUL2_PORT_IP_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C                2
#define SIUL2_PORT_IP_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C                0
#define SIUL2_PORT_IP_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if Siul2_Port_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Siul2_Port_Ip.h are of the same vendor */
#if (SIUL2_PORT_IP_VENDOR_ID_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != SIUL2_PORT_IP_VENDOR_ID_H)
    #error "Siul2_Port_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Siul2_Port_Ip.h have different vendor ids"
#endif
/* Check if Siul2_Port_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Siul2_Port_Ip.h are of the same Autosar version */
#if ((SIUL2_PORT_IP_AR_RELEASE_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C    != SIUL2_PORT_IP_AR_RELEASE_MAJOR_VERSION_H) || \
     (SIUL2_PORT_IP_AR_RELEASE_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C    != SIUL2_PORT_IP_AR_RELEASE_MINOR_VERSION_H) || \
     (SIUL2_PORT_IP_AR_RELEASE_REVISION_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != SIUL2_PORT_IP_AR_RELEASE_REVISION_VERSION_H) \
    )
    #error "AutoSar Version Numbers of Siul2_Port_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Siul2_Port_Ip.h are different"
#endif
/* Check if Siul2_Port_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Siul2_Port_Ip.h are of the same software version */
#if ((SIUL2_PORT_IP_SW_MAJOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != SIUL2_PORT_IP_SW_MAJOR_VERSION_H) || \
     (SIUL2_PORT_IP_SW_MINOR_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != SIUL2_PORT_IP_SW_MINOR_VERSION_H) || \
     (SIUL2_PORT_IP_SW_PATCH_VERSION_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_C != SIUL2_PORT_IP_SW_PATCH_VERSION_H)    \
    )
    #error "Software Version Numbers of Siul2_Port_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Siul2_Port_Ip.h are different"
#endif

/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

[!VAR "base_IP" = "0"!][!//
[!VAR "mscrPdacValue_IP" = "0"!][!//
[!VAR "imcrPdacValue_IP" = "0"!][!//
[!VAR "PinPortIdx_IP" = "0"!][!//
[!VAR "Mux_value_IP" = "0"!][!//
[!VAR "Mux_IP" = "'PORT_MUX_AS_GPIO'"!][!//
[!VAR "SafeMode_IP" = "'PORT_SAFE_MODE_DISABLED'"!][!//
[!VAR "PullConfig_IP" = "'PORT_INTERNAL_PULL_NOT_ENABLED'"!][!//
[!VAR "DriveStrength_IP" = "'PORT_DRIVE_STRENTGTH_DISABLED'"!][!//
[!VAR "InputFilter_IP" = "'PORT_INPUT_FILTER_DISABLED'"!][!//
[!VAR "PullKeep_IP" = "'PORT_PULL_KEEP_DISABLED'"!][!//
[!VAR "Invert_IP" = "'PORT_INVERT_DISABLED'"!][!//
[!VAR "SlewRateCtrlSel_IP" = "'PORT_SLEW_RATE_FASTEST'"!][!//
[!VAR "ReceiverSel_IP" = "'PORT_RECEIVER_ENABLE_DIFFERENTIAL_VREF'"!][!//
[!VAR "OpenDrain_IP" = "'PORT_OPEN_DRAIN_DISABLED'"!][!//
[!VAR "InputBuffer_IP" = "'PORT_INPUT_BUFFER_DISABLED'"!][!//
[!VAR "OutputBuffer_IP" = "'PORT_OUTPUT_BUFFER_DISABLED'"!][!//
[!VAR "AdcInterleave0_IP" = "'MUX_MODE_NOT_AVAILABLE'"!][!//
[!VAR "AdcInterleave1_IP" = "'MUX_MODE_NOT_AVAILABLE'"!][!//
[!VAR "InitValue_IP" = "'0'"!][!//
[!VAR "InputMuxReg_IP" = "'0'"!][!//
[!VAR "InputMux0_IP" = "'PORT_INPUT_MUX_NO_INIT'"!][!//
[!VAR "InputMux1_IP" = "'PORT_INPUT_MUX_NO_INIT'"!][!//
[!VAR "InputMux2_IP" = "'PORT_INPUT_MUX_NO_INIT'"!][!//
[!VAR "InputMux3_IP" = "'PORT_INPUT_MUX_NO_INIT'"!][!//
[!VAR "InputMux4_IP" = "'PORT_INPUT_MUX_NO_INIT'"!][!//
[!VAR "InputMux5_IP" = "'PORT_INPUT_MUX_NO_INIT'"!][!//
[!VAR "InputMux6_IP" = "'PORT_INPUT_MUX_NO_INIT'"!][!//
[!VAR "InputMux7_IP" = "'PORT_INPUT_MUX_NO_INIT'"!][!//

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/

[!MACRO "GetAltConf_IP"!][!//
[!NOCODE!][!//
[!/* Alternative Mode 0 is the GPIO */!][!//
[!IF "text:match(./PortPinMode,'GPIO')"!][!//
    [!VAR "OutPinAltFunc_IP"="0"!]!//
[!ELSE!][!//
    [!VAR "MacPinMode_IP"="concat(./PortPinSiul2Instance, '_PORT',number(./PortPinPcr), '_', ./PortPinMode)"!][!//
    [!VAR "OutPinAltFunc_IP"="number(substring-after(substring-before((substring-after($PinMap,$MacPinMode_IP)),':'),';'))"!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//
[!MACRO "GetAltInputConf_IP"!][!//
[!NOCODE!][!//
[!/* Alternative Mode 0 is the GPIO */!][!//
[!IF "text:match(./PortPinMode,'GPIO')"!][!//
    [!VAR "InputMuxReg_IP" = "'0'"!][!//
    [!VAR "InputMux0_IP" = "'PORT_INPUT_MUX_NO_INIT'"!][!//
[!ELSE!][!//
    [!VAR "MacPinMode_IP"="concat(./PortPinSiul2Instance, '_PORT',number(./PortPinPcr), '_', ./PortPinMode)"!][!//

    [!VAR "InPinImcrSiulNumber_IP"="number(substring-after(substring-before((substring-after($PinMap,$MacPinMode_IP)),'@'),':'))"!][!//
    [!VAR "InPinImcrRegNumber_IP"="number(substring-after(substring-before((substring-after($PinMap,$MacPinMode_IP)),'^'),'@'))"!][!//
    [!VAR "OutPinAltFunc_IP"="number(substring-after(substring-before((substring-after($PinMap,$MacPinMode_IP)),':'),';'))"!][!//
    [!IF "($InPinImcrSiulNumber_IP = 65535)"!][!//
        [!VAR "InputMuxReg_IP" = "'0'"!][!//
        [!VAR "InputMux0_IP" = "'PORT_INPUT_MUX_NO_INIT'"!][!//
    [!ELSE!][!//
        [!VAR "InPinAltFunc_IP"="number(substring-after(substring-before((substring-after($PinMap,$MacPinMode_IP)),'~'),'^'))"!][!//
        [!VAR "InputMux0_IP" = "concat('PORT_INPUT_MUX_ALT',num:i($InPinAltFunc_IP))"!][!//
        [!IF "($InPinImcrSiulNumber_IP = 1)"!][!//
        [!VAR "InputMuxReg_IP" = "num:i($InPinImcrRegNumber_IP + 512)"!][!//
        [!ELSE!][!//
        [!VAR "InputMuxReg_IP" = "num:i($InPinImcrRegNumber_IP)"!][!//
        [!ENDIF!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//
[!MACRO "CalculateSSS_IO_IP"!][!//
[!NOCODE!][!//
[!CALL "GetAltConf_IP"!][!//
[!IF "($OutPinAltFunc_IP = 16)"!][!//
    [!/* Pin is in analog mode.*/!][!//
    [!VAR "Mux_value_IP" = "0"!][!//
    [!VAR "OutputBuffer_IP" = "'PORT_OUTPUT_BUFFER_DISABLED'"!][!//
    [!VAR "InputBuffer_IP" = "'PORT_INPUT_BUFFER_DISABLED'"!][!//
[!ELSE!]
    [!IF "($OutPinAltFunc_IP >= 32)"!][!//
        [!/* Pin is in input/output mode */!][!//
        [!VAR "Mux_value_IP" = "$OutPinAltFunc_IP - 31"!][!//
        [!VAR "OutputBuffer_IP" = "'PORT_OUTPUT_BUFFER_ENABLED'"!][!//
        [!VAR "InputBuffer_IP" = "'PORT_INPUT_BUFFER_ENABLED'"!][!//
    [!ELSEIF "($OutPinAltFunc_IP >= 18)"!][!//
        [!/* Pin is in 'only input' or 'input' mode. Set IBE bit */!][!//
        [!VAR "Mux_value_IP" = "0"!][!//
        [!VAR "OutputBuffer_IP" = "'PORT_OUTPUT_BUFFER_DISABLED'"!][!//
        [!VAR "InputBuffer_IP" = "'PORT_INPUT_BUFFER_ENABLED'"!][!//
    [!ELSEIF "($OutPinAltFunc_IP = 17)"!][!//
        [!/* Pin is in 'only output' mode. Set OBE and Safe Mode bits */!][!//
        [!VAR "Mux_value_IP" = "0"!][!//
        [!VAR "OutputBuffer_IP" = "'PORT_OUTPUT_BUFFER_ENABLED'"!][!//
        [!VAR "InputBuffer_IP" = "'PORT_INPUT_BUFFER_DISABLED'"!][!//
        [!VAR "SafeMode_IP" = "'PORT_SAFE_MODE_ENABLED'"!][!//
    [!ELSEIF "($OutPinAltFunc_IP > 0)"!][!//
        [!/* Pin is output. Set output params: SSS field in MSCR, Safe Mode and OBE */!][!//
        [!VAR "Mux_value_IP" = "$OutPinAltFunc_IP"!][!//
        [!VAR "OutputBuffer_IP" = "'PORT_OUTPUT_BUFFER_ENABLED'"!][!//
        [!VAR "InputBuffer_IP" = "'PORT_INPUT_BUFFER_DISABLED'"!][!//
    [!ELSE!][!//
        [!/* The pin is configured as GPIO. Look in the PortPinDirection combo for port direction */!][!//
        [!VAR "Mux_value_IP" = "0"!][!//
        [!IF "text:match(./PortPinDirection,'PORT_PIN_INOUT')"!][!//
            [!VAR "OutputBuffer_IP" = "'PORT_OUTPUT_BUFFER_ENABLED'"!][!//
            [!VAR "InputBuffer_IP" = "'PORT_INPUT_BUFFER_ENABLED'"!][!//
        [!ELSEIF "text:match(./PortPinDirection,'PORT_PIN_OUT')"!][!//
            [!VAR "OutputBuffer_IP" = "'PORT_OUTPUT_BUFFER_ENABLED'"!][!//
            [!VAR "InputBuffer_IP" = "'PORT_INPUT_BUFFER_DISABLED'"!][!//
        [!ELSEIF "text:match(./PortPinDirection,'PORT_PIN_IN')"!][!//
            [!VAR "OutputBuffer_IP" = "'PORT_OUTPUT_BUFFER_DISABLED'"!][!//
            [!VAR "InputBuffer_IP" = "'PORT_INPUT_BUFFER_ENABLED'"!][!//
        [!ELSE!][!//
            [!/* Pad is configured as high-z. Clear IBE and OBE bit */!][!//
            [!VAR "OutputBuffer_IP" = "'PORT_OUTPUT_BUFFER_DISABLED'"!][!//
            [!VAR "InputBuffer_IP" = "'PORT_INPUT_BUFFER_DISABLED'"!][!//
        [!ENDIF!][!//
    [!ENDIF!][!//
[!ENDIF!]
[!/* Check if read status is enabled */!][!//
[!IF "./PortPinWithReadBack"!][!//
    [!VAR "InputBuffer_IP" = "'PORT_INPUT_BUFFER_ENABLED'"!][!//
[!ENDIF!][!//
[!IF "($Mux_value_IP = 0)"!][!//
[!VAR "Mux_IP" = "'PORT_MUX_AS_GPIO'"!][!//
[!ELSEIF "($Mux_value_IP = 1)"!][!//
[!VAR "Mux_IP" = "'PORT_MUX_ALT1'"!][!//
[!ELSEIF "($Mux_value_IP = 2)"!][!//
[!VAR "Mux_IP" = "'PORT_MUX_ALT2'"!][!//
[!ELSEIF "($Mux_value_IP = 3)"!][!//
[!VAR "Mux_IP" = "'PORT_MUX_ALT3'"!][!//
[!ELSEIF "($Mux_value_IP = 4)"!][!//
[!VAR "Mux_IP" = "'PORT_MUX_ALT4'"!][!//
[!ELSEIF "($Mux_value_IP = 5)"!][!//
[!VAR "Mux_IP" = "'PORT_MUX_ALT5'"!][!//
[!ELSEIF "($Mux_value_IP = 6)"!][!//
[!VAR "Mux_IP" = "'PORT_MUX_ALT6'"!][!//
[!ELSEIF "($Mux_value_IP = 7)"!][!//
[!VAR "Mux_IP" = "'PORT_MUX_ALT7'"!][!//
[!ENDIF!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//
[!MACRO "GetPDO_IP"!][!//
[!NOCODE!][!//
[!IF "not(contains(node:fallback(./PortPinMode,'ADC'),'ADC'))"!][!//
    [!IF "text:match(./PortPinLevelValue,'PORT_PIN_LEVEL_LOW')"!][!//
        [!VAR "MacPinPDO_IP"="'0'"!][!//
    [!ELSEIF "text:match(./PortPinLevelValue,'PORT_PIN_LEVEL_HIGH')"!][!//
        [!VAR "MacPinPDO_IP"="'1'"!][!//
    [!ELSEIF "text:match(./PortPinLevelValue,'PORT_PIN_LEVEL_NOTCHANGED')"!][!//
        [!VAR "MacPinPDO_IP"="'2'"!][!//
    [!ELSE!]
        [!/*don't anything*/!]
    [!ENDIF!][!//
[!ELSE!][!//
    [!VAR "MacPinPDO_IP"="'2'"!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!VAR "InitValue_IP" = "$MacPinPDO_IP"!][!//
[!ENDMACRO!][!//
[!MACRO "GetPinData_IP"!][!//
[!VAR "PinPortIdx_IP" = "./PortPinPcr"!][!//
[!CALL "GetAltInputConf_IP"!][!//
[!NOCODE!][!//
[!IF "../../../../../PortGeneral/VirtWrapperSupport"!][!//
    [!VAR "imcrPdacSlot" = "0"!][!//
    [!VAR "mscrPdacSlot" = "0"!][!//
    [!IF "node:exists('ASPathDataOfSchema:/AUTOSAR/EcucDefs/Rm/RmGeneral/RmVirtWrapperConfigurable/')"!][!//
        [!IF "node:fallback(node:refs('ASPathDataOfSchema:/AUTOSAR/EcucDefs/Rm/RmGeneral/RmVirtWrapperConfigurable/'), false) = 'true'"!][!//
            [!LOOP "node:refs('ASPathDataOfSchema:/AUTOSAR/EcucDefs/Rm/RmConfigSet/Virt_Wrapper_Configuration/Mscr_Config_List')"!][!//
                [!IF "num:i(./MscrNumber) = $PinPortIdx_IP"!][!//
                    [!LOOP "text:split(./MscrMirror,'_')"!][!//
                        [!IF "contains(node:current(), 'PDAC')"!][!//
                            [!VAR "mscrPdacSlot" = "substring-after(node:current(),'PDAC')"!][!//
                        [!ENDIF!][!//
                    [!ENDLOOP!][!//
                [!ENDIF!][!//
            [!ENDLOOP!][!//
            [!LOOP "node:refs('ASPathDataOfSchema:/AUTOSAR/EcucDefs/Rm/RmConfigSet/Virt_Wrapper_Configuration/Imcr_Config_List')"!][!//
                [!IF "num:i(./ImcrNumber) = $InputMuxReg_IP"!][!//
                    [!LOOP "text:split(./ImcrMirror,'_')"!][!//
                        [!IF "contains(node:current(), 'PDAC')"!][!//
                            [!VAR "imcrPdacSlot" = "substring-after(node:current(),'PDAC')"!][!//
                        [!ENDIF!][!//
                    [!ENDLOOP!][!//
                [!ENDIF!][!//
            [!ENDLOOP!][!//
        [!ENDIF!][!//
    [!ELSE!][!//
        [!VAR "mscrPdacSlot" = "num:i(substring-after(./MscrPdacSlot,'PDAC'))"!][!//
        [!VAR "imcrPdacSlot" = "num:i(substring-after(./ImcrPdacSlot,'PDAC'))"!][!//
    [!ENDIF!][!//
    [!VAR "baseAddr" = "concat('SIUL2_PDAC',$mscrPdacSlot)"!][!//
[!ELSE!][!//
    [!VAR "baseAddr" = "'IP_SIUL2'"!][!//
[!ENDIF!][!//
[!VAR "base_IP" = "$baseAddr"!][!//
[!IF "../../../../../PortGeneral/VirtWrapperSupport"!][!//
[!VAR "mscrPdacValue_IP" = "$mscrPdacSlot"!][!//
[!VAR "imcrPdacValue_IP" = "$imcrPdacSlot"!][!//
[!ENDIF!][!//
[!VAR "PinPortIdx_IP" = "./PortPinPcr"!][!//
[!CALL "CalculateSSS_IO_IP"!][!//
[!IF "./PortPinSafeMode"!][!//
    [!VAR "SafeMode_IP" = "'PORT_SAFE_MODE_ENABLED'"!][!//
[!ELSE!][!//
    [!VAR "SafeMode_IP" = "'PORT_SAFE_MODE_DISABLED'"!][!//
[!ENDIF!][!//
[!IF "./PortPinPue"!][!//
    [!IF "./PortPinPus"!][!//
        [!VAR "PullConfig_IP" = "'PORT_INTERNAL_PULL_UP_ENABLED'"!][!//
    [!ELSE!][!//
        [!VAR "PullConfig_IP" = "'PORT_INTERNAL_PULL_DOWN_ENABLED'"!][!//
    [!ENDIF!][!//
[!ELSE!][!//
    [!VAR "PullConfig_IP" = "'PORT_INTERNAL_PULL_NOT_ENABLED'"!][!//
[!ENDIF!][!//
[!IF "./PortPinDse"!][!//
    [!VAR "DriveStrength_IP" = "'PORT_DRIVE_STRENTGTH_ENABLED'"!][!//
[!ELSE!][!//
    [!VAR "DriveStrength_IP" = "'PORT_DRIVE_STRENTGTH_DISABLED'"!][!//
[!ENDIF!][!//
[!IF "./PortPinIfe"!][!//
    [!VAR "InputFilter_IP" = "'PORT_INPUT_FILTER_ENABLED'"!][!//
[!ELSE!][!//
    [!VAR "InputFilter_IP" = "'PORT_INPUT_FILTER_DISABLED'"!][!//
[!ENDIF!][!//
[!IF "./PortPinPke"!][!//
    [!VAR "PullKeep_IP" = "'PORT_PULL_KEEP_ENABLED'"!][!//
[!ELSE!][!//
    [!VAR "PullKeep_IP" = "'PORT_PULL_KEEP_DISABLED'"!][!//
[!ENDIF!][!//
[!VAR "Invert_IP" = "'PORT_INVERT_DISABLED'"!][!//

[!/* Check if INV is enabled */!][!//
[!IF "(./PortPinInvertControl = 'true')"!][!//
    [!VAR "Invert_IP" = "'PORT_INVERT_ENABLED'"!][!//
[!ELSE!][!//
    [!VAR "Invert_IP" = "'PORT_INVERT_DISABLED'"!][!//
[!ENDIF!][!//

[!/* SRE bits */!][!//
[!IF "text:match(./PortPinSlewRate,'FASTEST_SETTING')"!][!//
    [!VAR "SlewRateCtrlSel_IP" = "'PORT_SLEW_RATE_FASTEST'"!][!//
[!ELSEIF "text:match(./PortPinSlewRate,'SLOWEST_SETTING')"!][!//
    [!VAR "SlewRateCtrlSel_IP" = "'PORT_SLEW_RATE_SLOWEST'"!][!//
[!ENDIF!][!//

[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetAdcInterleave_IP"!][!//
[!NOCODE!][!//
[!IF "contains(node:fallback(./PortPinMode,'ADC'),'ADC')"!][!//
    [!VAR "MacPinMode_IP"="concat(./PortPinSiul2Instance, '_PORT',number(./PortPinPcr), '_', ./PortPinMode)"!][!//
    [!VAR "AdcInterleaveValue"="number(substring-after(substring-before((substring-after($PinMap,$MacPinMode_IP)),'#'),'*'))"!][!//

    [!IF "not($AdcInterleaveValue = 65535)"!][!//
        [!VAR "AdcInterleave0_IP" = "concat(concat('MUX_MODE_EN_',concat(substring-after(./PortPinMode, '_'), '_')), num:i($AdcInterleaveValue))"!][!//
    [!ENDIF!][!//
[!ELSE!][!//
    [!VAR "AdcInterleave0_IP" = "'MUX_MODE_NOT_AVAILABLE'"!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/
#define PORT_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Port_MemMap.h"

[!INDENT "0"!][!//
    const Siul2_Port_Ip_PinSettingsConfig g_pin_mux_InitConfigArr[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][NUM_OF_CONFIGURED_PINS] =
[!ENDINDENT!][!//
{
[!SELECT "PortConfigSet"!][!//
[!VAR "count"= "1"!][!//
[!VAR "Maxcount"= "num:i(count(PortContainer/*/PortPin/*))-1"!][!//
[!LOOP "PortContainer/*/PortPin/*"!][!//
[!CALL "GetPinData_IP"!][!//
[!CALL "GetPDO_IP"!][!//
[!CALL "GetAdcInterleave_IP"!][!//
[!IF "$Maxcount>=$count"!][!//
    {
        .base                        = [!"$base_IP"!],
        .pinPortIdx                  = [!"$PinPortIdx_IP"!],
        .mux                         = [!"$Mux_IP"!],
        .safeMode                    = [!"$SafeMode_IP"!],
        .pullConfig                  = [!"$PullConfig_IP"!],
        .driveStrength               = [!"$DriveStrength_IP"!],
        .inputFilter                 = [!"$InputFilter_IP"!],
        .pullKeep                    = [!"$PullKeep_IP"!],
        .invert                      = [!"$Invert_IP"!],
        .slewRateCtrlSel             = [!"$SlewRateCtrlSel_IP"!],
        .inputBuffer                 = [!"$InputBuffer_IP"!],
        .outputBuffer                = [!"$OutputBuffer_IP"!],
        .adcInterleaves              = {
                                         [!"$AdcInterleave0_IP"!],
                                         [!"$AdcInterleave1_IP"!]
                                       },
        .initValue                   = [!"$InitValue_IP"!],
        .inputMuxReg                 = {
                                         [!"$InputMuxReg_IP"!]
                                       },
        .inputMux                    = {
                                         [!"$InputMux0_IP"!],
                                         [!"$InputMux1_IP"!],
                                         [!"$InputMux2_IP"!],
                                         [!"$InputMux3_IP"!],
                                         [!"$InputMux4_IP"!],
                                         [!"$InputMux5_IP"!],
                                         [!"$InputMux6_IP"!],
                                         [!"$InputMux7_IP"!]
                                       },
[!IF "../../../../../PortGeneral/VirtWrapperSupport"!][!//
[!CODE!][!//
        .mscrPdacSlot                    = [!"$mscrPdacValue_IP"!],
        .imcrPdacSlot                    = [!"$imcrPdacValue_IP"!]
[!ENDCODE!][!//
[!ENDIF!][!//
    },
[!ELSE!][!//
    {
        .base                        = [!"$base_IP"!],
        .pinPortIdx                  = [!"$PinPortIdx_IP"!],
        .mux                         = [!"$Mux_IP"!],
        .safeMode                    = [!"$SafeMode_IP"!],
        .pullConfig                  = [!"$PullConfig_IP"!],
        .driveStrength               = [!"$DriveStrength_IP"!],
        .inputFilter                 = [!"$InputFilter_IP"!],
        .pullKeep                    = [!"$PullKeep_IP"!],
        .invert                      = [!"$Invert_IP"!],
        .slewRateCtrlSel             = [!"$SlewRateCtrlSel_IP"!],
        .inputBuffer                 = [!"$InputBuffer_IP"!],
        .outputBuffer                = [!"$OutputBuffer_IP"!],
        .adcInterleaves              = {
                                         [!"$AdcInterleave0_IP"!],
                                         [!"$AdcInterleave1_IP"!]
                                       },
        .initValue                   = [!"$InitValue_IP"!],
        .inputMuxReg                 = {
                                         [!"$InputMuxReg_IP"!]
                                       },
        .inputMux                    = {
                                         [!"$InputMux0_IP"!],
                                         [!"$InputMux1_IP"!],
                                         [!"$InputMux2_IP"!],
                                         [!"$InputMux3_IP"!],
                                         [!"$InputMux4_IP"!],
                                         [!"$InputMux5_IP"!],
                                         [!"$InputMux6_IP"!],
                                         [!"$InputMux7_IP"!]
                                       },
[!IF "../../../../../PortGeneral/VirtWrapperSupport"!][!//
[!CODE!][!//
        .mscrPdacSlot                    = [!"$mscrPdacValue_IP"!],
        .imcrPdacSlot                    = [!"$imcrPdacValue_IP"!]
[!ENDCODE!][!//
[!ENDIF!][!//
    }
[!ENDIF!][!//
[!VAR "count"= "$count+1"!][!//
[!ENDLOOP!][!//
[!ENDSELECT!][!//
};

#define PORT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Port_MemMap.h"

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */
