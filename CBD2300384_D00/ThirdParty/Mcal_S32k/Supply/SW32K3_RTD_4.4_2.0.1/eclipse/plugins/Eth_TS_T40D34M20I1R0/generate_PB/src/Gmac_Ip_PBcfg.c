/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : GMAC
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
/*   @implements Gmac_Ip_PBcfg.c_Artifact */
/**
*   @file
*
*   @addtogroup GMAC_DRIVER_CONFIGURATION GMAC Driver Configuration
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
#include "Gmac_Ip_Types.h"
#include "Gmac_Ip_Cfg.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C                     43
#define GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C      4
#define GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C      4
#define GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C   0
#define GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C              2
#define GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C              0
#define GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Gmac_Ip_Types.h */
#if (GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != GMAC_IP_TYPES_VENDOR_ID)
    #error "Gmac_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Gmac_Ip_Types.h have different vendor ids"
#endif
#if ((GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != GMAC_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != GMAC_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != GMAC_IP_TYPES_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Gmac_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Gmac_Ip_Types.h are different"
#endif
#if ((GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != GMAC_IP_TYPES_SW_MAJOR_VERSION) || \
     (GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != GMAC_IP_TYPES_SW_MINOR_VERSION) || \
     (GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != GMAC_IP_TYPES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Gmac_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Gmac_Ip_Types.h are different"
#endif

/* Checks against Gmac_Ip_Cfg.h */
#if (GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != GMAC_IP_CFG_VENDOR_ID)
    #error "Gmac_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Gmac_Ip_Cfg.h have different vendor ids"
#endif
#if ((GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != GMAC_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != GMAC_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != GMAC_IP_CFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Gmac_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Gmac_Ip_Cfg.h are different"
#endif
#if ((GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != GMAC_IP_CFG_SW_MAJOR_VERSION) || \
     (GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != GMAC_IP_CFG_SW_MINOR_VERSION) || \
     (GMAC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != GMAC_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Gmac_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Gmac_Ip_Cfg.h are different"
#endif

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
[!MACRO "GetPhysicalAddress"!][!//
[!NOCODE!]
    [!VAR "PhysAddr" = "'00:00:00:00:00:00'"!]
    [!IF "node:exists(EthCtrlPhyAddress)"!]
        [!VAR "PhysAddr" = "node:value(EthCtrlPhyAddress)"!]
    [!ENDIF!]

    [!FOR "AddrByteIdx" = "1" TO "6"!]
        [!CODE!][!"concat('0x', text:split($PhysAddr, ':')[num:i($AddrByteIdx)])"!]U[!IF "$AddrByteIdx < 6"!], [!ENDIF!][!ENDCODE!]
    [!ENDFOR!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetMacLayerType"!][!//
[!NOCODE!]
    [!VAR "MacLayer" = "string(null)"!]

    [!IF "substring-after(EthCtrlMacLayerType, 'ETH_MAC_LAYER_TYPE_') = 'XMII'"!]
        [!IF "EthCtrlMacLayerSubType = 'STANDARD'"!]
            [!VAR "MacLayer" = "'MII'"!]
        [!ELSEIF "EthCtrlMacLayerSubType = 'SERIAL'"!]
            [!VAR "MacLayer" = "'SMII'"!]
        [!ELSEIF "EthCtrlMacLayerSubType = 'REDUCED'"!]
            [!VAR "MacLayer" = "'RMII'"!]
        [!ENDIF!]
    [!ELSEIF "substring-after(EthCtrlMacLayerType, 'ETH_MAC_LAYER_TYPE_') = 'XGMII'"!]
        [!IF "EthCtrlMacLayerSubType = 'STANDARD'"!]
            [!VAR "MacLayer" = "'GMII'"!]
        [!ELSEIF "EthCtrlMacLayerSubType = 'SERIAL'"!]
            [!VAR "MacLayer" = "'SGMII'"!]
        [!ELSEIF "EthCtrlMacLayerSubType = 'REDUCED'"!]
            [!VAR "MacLayer" = "'RGMII'"!]
        [!ENDIF!]
    [!ENDIF!]

    [!IF "$MacLayer = string(null)"!][!ERROR!]Unknown or unsupported media independent interface! ([!"EthCtrlMacLayerType"!], [!"EthCtrlMacLayerSubType"!])[!ENDERROR!][!ENDIF!]
    [!CODE!][!"$MacLayer"!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetSafetyMask"!][!//
[!NOCODE!]
    [!VAR "SafetyMask" = "'0U'"!]

    [!IF "node:containsValue(EthCtrlVendorSpecific/EthCtrlConfigSafety/*, 'true')"!]
        [!VAR "SafetyMask" = "'(uint32)GMAC_SAF_ERR_CSR_EMPTY_DATA'"!]
    [!ENDIF!]

    [!IF "text:tolower(EthCtrlVendorSpecific/EthCtrlConfigSafety/EthCtrlEnableEcc) = 'true'"!]
        [!VAR "SafetyMask" = "concat($SafetyMask, ' | (uint32)GMAC_SAF_ERR_ECC')"!]
    [!ENDIF!]

    [!IF "text:tolower(EthCtrlVendorSpecific/EthCtrlConfigSafety/EthCtrlEnableDpp) = 'true'"!]
        [!VAR "SafetyMask" = "concat($SafetyMask, ' | (uint32)GMAC_SAF_ERR_DATA_PATH_PARITY')"!]
    [!ENDIF!]

    [!IF "text:tolower(EthCtrlVendorSpecific/EthCtrlConfigSafety/EthCtrlEnableFsmSpp) = 'true'"!]
        [!VAR "SafetyMask" = "concat($SafetyMask, ' | (uint32)GMAC_SAF_ERR_FSM_STATE_PARITY')"!]
    [!ENDIF!]

    [!IF "text:tolower(EthCtrlVendorSpecific/EthCtrlConfigSafety/EthCtrlEnableFsmTimeout) = 'true'"!]
        [!VAR "SafetyMask" = "concat($SafetyMask, ' | (uint32)GMAC_SAF_ERR_FSM_TIMEOUT | (uint32)GMAC_SAF_ERR_MASTER_INTERFACE_TIMEOUT')"!]
    [!ENDIF!]

    [!CODE!][!"$SafetyMask"!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetMacConfigMask"!][!//
[!NOCODE!]
    [!VAR "MacCfgMask" = "'0U'"!]

    [!LOOP "EthCtrlVendorSpecific/EthCtrlConfigMac/*"!]
        [!IF "node:value(.) = 'true'"!]
            [!VAR "MacCfgMask" = "concat($MacCfgMask, ' | (uint32)GMAC_', @name)"!]
        [!ENDIF!]
    [!ENDLOOP!]

    [!IF "node:containsValue(../../../EthGeneral/EthCtrlOffloading/*, 'true')"!]
        [!VAR "MacCfgMask" = "concat($MacCfgMask, ' | (uint32)GMAC_MAC_CONFIG_CHECKSUM_OFFLOAD')"!]
    [!ENDIF!]

    [!CODE!][!"$MacCfgMask"!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetMacPktFilterMask"!][!//
[!NOCODE!]
    [!VAR "MacPktFilterMask" = "'0U'"!]

    [!LOOP "EthCtrlVendorSpecific/EthCtrlConfigPacketFilter/*"!]
        [!IF "node:value(.) = 'true'"!]
            [!VAR "MacPktFilterMask" = "concat($MacPktFilterMask, ' | (uint32)GMAC_', @name)"!]
        [!ENDIF!]
    [!ENDLOOP!]

    [!CODE!][!"$MacPktFilterMask"!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetPriorityMask", "QueueType"!][!//
[!NOCODE!]
    [!VAR "PriorityMask"     = "string(null)"!]
    [!VAR "PriorityListPath" = "concat('EthCtrlConfig', $QueueType, 'FifoPriorityAssignment')"!]

    [!LOOP "node:order(node:ref($PriorityListPath)/*, 'node:value(.)')"!]
        [!IF "$PriorityMask = string(null)"!]
            [!VAR "PriorityMask" = "concat($PriorityMask, '(uint8)GMAC_VLAN_PRIORITY_', node:value(.))"!]
        [!ELSE!]
            [!VAR "PriorityMask" = "concat($PriorityMask, ' | (uint8)GMAC_VLAN_PRIORITY_', node:value(.))"!]
        [!ENDIF!]
    [!ENDLOOP!]

    [!CODE!][!IF "$PriorityMask = string(null)"!]0U[!ELSE!][!"$PriorityMask"!][!ENDIF!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetDmaBurstLength", "QueueType", "DataBusWidthInBytes", "MaxDataBusBurstLength"!][!//
[!NOCODE!]
    [!VAR "BufferLength" = "node:value(concat('EthCtrlConfig', $QueueType, 'FifoBufLenByte'))"!]
    [!VAR "BufferCount"  = "node:value(concat('EthCtrlConfig', $QueueType, 'FifoBufTotal'))"!]

    [!VAR "v1" = "num:i($BufferLength div $DataBusWidthInBytes)"!]
    [!VAR "v2" = "num:i(($BufferLength * $BufferCount div $DataBusWidthInBytes) div 2)"!]
    [!VAR "v3" = "num:i(2 * $MaxDataBusBurstLength)"!]

    [!VAR "BurstLength" = "num:min(text:split(concat($v1, ' ', $v2, ' ', $v3)))"!]

    [!// Round up BurstLength to next power of 2
    [!VAR "BurstLengthPow2" = "1"!]
    [!FOR "pow" = "1" TO "8"!]
        [!IF "$BurstLengthPow2 < $BurstLength"!]
            [!VAR "BurstLengthPow2" = "$BurstLengthPow2 * 2"!]
        [!ELSE!]
            [!BREAK!]
        [!ENDIF!]
    [!ENDFOR!]

    [!CODE!][!"num:i($BurstLengthPow2)"!]U[!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetTxSchedulingAlgorithm"!][!//
[!NOCODE!]
    [!VAR "TxSchedAlgo" = "string(null)"!]
    [!VAR "NodeValue" = "node:ref(EthCtrlConfigEgress/EthCtrlConfigEgressLastSchedulerRef)/EthCtrlConfigSchedulerVendorSpecific/EthTxSchedulerAlgorithm"!]

    [!IF "$NodeValue = 'WEIGHTED_ROUND_ROBIN'"!]
        [!VAR "TxSchedAlgo" = "'WRR'"!]
    [!ELSEIF "$NodeValue = 'WEIGHTED_FAIR_QUEUING'"!]
        [!VAR "TxSchedAlgo" = "'WFQ'"!]
    [!ELSEIF "$NodeValue = 'DEFICIT_WEIGHTED_ROUND_ROBIN'"!]
        [!VAR "TxSchedAlgo" = "'DWRR'"!]
    [!ELSEIF "$NodeValue = 'STRICT_PRIORITY'"!]
        [!VAR "TxSchedAlgo" = "'SP'"!]
    [!ENDIF!]

    [!IF "$TxSchedAlgo = string(null)"!][!ERROR!]Unknown or unsupported Tx Scheduling Algorithm! ([!"$NodeValue"!])[!ENDERROR!][!ENDIF!]
    [!CODE!][!"$TxSchedAlgo"!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetPortTransferRate"!][!//
[!NOCODE!]
    [!VAR "portTransferRate" = "0"!]

    [!IF "substring-after(EthCtrlMacLayerSpeed, 'ETH_MAC_LAYER_SPEED_') = '10M'"!]
        [!VAR "portTransferRate" = "10000000"!]
    [!ELSEIF "substring-after(EthCtrlMacLayerSpeed, 'ETH_MAC_LAYER_SPEED_') = '100M'"!]
        [!VAR "portTransferRate" = "100000000"!]      
    [!ELSEIF "substring-after(EthCtrlMacLayerSpeed, 'ETH_MAC_LAYER_SPEED_') = '200M'"!]
        [!VAR "portTransferRate" = "200000000"!]       
    [!ELSEIF "substring-after(EthCtrlMacLayerSpeed, 'ETH_MAC_LAYER_SPEED_') = '1G'"!]
        [!VAR "portTransferRate" = "1000000000"!]
    [!ELSEIF "substring-after(EthCtrlMacLayerSpeed, 'ETH_MAC_LAYER_SPEED_') = '2500M'"!]
        [!VAR "portTransferRate" = "2500000000"!]
    [!ELSEIF "substring-after(EthCtrlMacLayerSpeed, 'ETH_MAC_LAYER_SPEED_') = '10G'"!]
        [!VAR "portTransferRate" = "10000000000"!]
    [!ENDIF!]

    [!IF "$portTransferRate = 0"!][!ERROR!]Unknown or unsupported MAC Layer Speed! ([!"EthCtrlMacLayerSpeed"!])[!ENDERROR!][!ENDIF!]
    [!CODE!][!"num:i($portTransferRate)"!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetPortBitsPerCycle"!][!//
[!NOCODE!]
    [!VAR "portTransferRate" = "0"!]
    [!VAR "portClockFrequency" = "0"!]

    [!IF "substring-after(EthCtrlMacLayerSpeed, 'ETH_MAC_LAYER_SPEED_') = '10M'"!]
        [!VAR "portTransferRate"   = "10000000"!]
        [!VAR "portClockFrequency" = "2500000"!]
    [!ELSEIF "substring-after(EthCtrlMacLayerSpeed, 'ETH_MAC_LAYER_SPEED_') = '100M'"!]
        [!VAR "portTransferRate"   = "100000000"!]
        [!VAR "portClockFrequency" = "25000000"!]       
    [!ELSEIF "substring-after(EthCtrlMacLayerSpeed, 'ETH_MAC_LAYER_SPEED_') = '200M'"!]
        [!VAR "portTransferRate"   = "200000000"!]
        [!VAR "portClockFrequency" = "50000000"!]        
    [!ELSEIF "substring-after(EthCtrlMacLayerSpeed, 'ETH_MAC_LAYER_SPEED_') = '1G'"!]
        [!VAR "portTransferRate"   = "1000000000"!]
        [!VAR "portClockFrequency" = "125000000"!]
    [!ELSEIF "substring-after(EthCtrlMacLayerSpeed, 'ETH_MAC_LAYER_SPEED_') = '2500M'"!]
        [!VAR "portTransferRate"   = "2500000000"!]
        [!VAR "portClockFrequency" = "312500000"!]
    [!ELSEIF "substring-after(EthCtrlMacLayerSpeed, 'ETH_MAC_LAYER_SPEED_') = '10G'"!]
        [!VAR "portTransferRate"   = "10000000000"!]
        [!VAR "portClockFrequency" = "833333333"!]
    [!ENDIF!]

    [!IF "$portTransferRate = 0"!][!ERROR!]Unknown or unsupported MAC Layer Speed! ([!"EthCtrlMacLayerSpeed"!])[!ENDERROR!][!ENDIF!]
    [!CODE!][!"num:i($portTransferRate div $portClockFrequency)"!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetCbsInfo", "QueueNum", "PortTransferRate", "PortBitsPerCycle"!][!//
[!NOCODE!]
[!// Assumes we are already in a EthCtrlConfigSchedulerPredecessor
    [!SELECT "node:ref(EthCtrlConfigSchedulerPredecessorRef)"!]
        [!VAR "TxFifoIdx" = "node:ref(EthCtrlConfigShaperPredecessorFifoRef)/EthCtrlConfigEgressFifoIdx"!]
        [!IF "num:i($TxFifoIdx) = num:i($QueueNum)"!]
            [!VAR "idleSlopeCredit"       = "EthCtrlConfigShaperIdleSlope"!]
            [!VAR "sendSlopeCredit"       = "$PortTransferRate - $idleSlopeCredit"!]
            [!VAR "idleBitsPerCycle"      = "($idleSlopeCredit div $PortTransferRate) * $PortBitsPerCycle"!]
            [!VAR "sendBitsPerCycle"      = "($sendSlopeCredit div $PortTransferRate) * $PortBitsPerCycle"!]
            [!VAR "highCreditBitsLimit"   = "EthCtrlConfigShaperVendorSpecific/EthCtrlConfigHiCredit"!]
            [!VAR "lowCreditBitsLimit"    = "EthCtrlConfigShaperVendorSpecific/EthCtrlConfigLoCredit"!]

            [!VAR "PARAM_QUEUE_OP_MODE"     = "'GMAC_OP_MODE_AVB'"!]
            [!VAR "PARAM_WEIGHT"            = "num:i(0)"!]
            [!VAR "PARAM_IDLE_SLOPE_CREDIT" = "num:i(round($idleBitsPerCycle * 1024))"!]
            [!VAR "PARAM_SEND_SLOPE_CREDIT" = "num:i(round($sendBitsPerCycle * 1024))"!]
            [!VAR "PARAM_HIGH_CREDIT"       = "num:i($highCreditBitsLimit    * 1024)"!]
            [!VAR "PARAM_LOW_CREDIT"        = "num:i(-$lowCreditBitsLimit    * 1024)"!]
        [!ENDIF!]
    [!ENDSELECT!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetDcbInfo", "QueueNum", "SchedulingAlgorithm"!][!//
[!NOCODE!]
[!// Assumes we are already in a EthCtrlConfigSchedulerPredecessor
    [!VAR "TxFifoIdx" = "node:ref(EthCtrlConfigSchedulerPredecessorRef)/EthCtrlConfigEgressFifoIdx"!]
    [!IF "num:i($TxFifoIdx) = num:i($QueueNum)"!]
       [!VAR "Bandwidth" = "EthCtrlConfigSchedulerPredecessorVendorSpecific/EthQueueBandwidth"!]
       [!VAR "Weight"    = "0"!]

       [!IF "$SchedulingAlgorithm = 'WEIGHTED_ROUND_ROBIN'"!]
           [!VAR "MaxWeightInPackets" = "node:value(../../EthCtrlConfigSchedulerVendorSpecific/EthTotalPacketsPerCycle)"!]
           [!VAR "Weight" = "round(($Bandwidth div 100) * $MaxWeightInPackets)"!]
       [!ELSEIF "$SchedulingAlgorithm = 'WEIGHTED_FAIR_QUEUING'"!]
           [!VAR "Weight" = "round(((100 - $Bandwidth) div 100) * 16383)"!]
       [!ELSEIF "$SchedulingAlgorithm = 'DEFICIT_WEIGHTED_ROUND_ROBIN'"!]
           [!VAR "MaxQuantumInBytes" = "node:value(../../EthCtrlConfigSchedulerVendorSpecific/EthTotalQuantumPerCycle)"!]
           [!VAR "Weight" = "round(($Bandwidth div 100) * $MaxQuantumInBytes)"!]
       [!ELSEIF "$SchedulingAlgorithm = 'STRICT_PRIORITY'"!]
            [!VAR "Weight" = "0"!]
       [!ELSE!]
           [!ERROR!]Unknown or unsupported scheduling algorithm! ([!"$SchedulingAlgorithm"!])[!ENDERROR!]
       [!ENDIF!]

       [!VAR "PARAM_QUEUE_OP_MODE"     = "'GMAC_OP_MODE_DCB_GEN'"!]
       [!VAR "PARAM_WEIGHT"            = "num:i($Weight)"!]
       [!VAR "PARAM_IDLE_SLOPE_CREDIT" = "num:i(0)"!]
       [!VAR "PARAM_SEND_SLOPE_CREDIT" = "num:i(0)"!]
       [!VAR "PARAM_HIGH_CREDIT"       = "num:i(0)"!]
       [!VAR "PARAM_LOW_CREDIT"        = "num:i(0)"!]
    [!ENDIF!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GenerateSchedulerInformation", "QueueNum", "TransferRate", "BitsPerCycle"!][!//
[!NOCODE!]
    [!SELECT "node:ref(../../EthCtrlConfigEgressLastSchedulerRef)"!]
        [!LOOP "EthCtrlConfigSchedulerPredecessor/*"!]
            [!IF "node:name(node:ref(EthCtrlConfigSchedulerPredecessorRef)/..) = 'EthCtrlConfigShaper'"!]
                [!CALL "GetCbsInfo",
                        "QueueNum" = "$QueueNum",
                        "PortTransferRate" = "$TransferRate",
                        "PortBitsPerCycle" = "$BitsPerCycle"!]
            [!ELSE!]
                [!CALL "GetDcbInfo",
                        "QueueNum" = "$QueueNum",
                        "SchedulingAlgorithm" = "../../EthCtrlConfigSchedulerVendorSpecific/EthTxSchedulerAlgorithm"!]
           [!ENDIF!]
        [!ENDLOOP!]
    [!ENDSELECT!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "SecondNanoSecondSplit", "NanoSecond"!][!//
    [!NOCODE!]
        [!VAR "cycleSecond" = "num:i(num:div($NanoSecond,1000000000))"!]
        [!VAR "cycleNanoSecond" = "num:i($NanoSecond - cycleSecond)"!]
    [!ENDNOCODE!]
[!ENDMACRO!][!//

[!MACRO "GateControl"!][!//
[!NOCODE!][!//
        [!VAR "GateNums" = "num:i(count(EthGateControlFifo/*) - 1)"!]
        [!VAR "GateControlReg" = "num:i(0)"!]
        [!FOR "FifoIdx" = "0" TO "$GateNums"!]
            [!LOOP "EthGateControlFifo/*"!]
                [!VAR "GateFifoIdx" = "node:ref(EthCtrlConfigFifoRef)/EthCtrlConfigEgressFifoIdx"!]
                [!IF "$GateFifoIdx = $FifoIdx"!]
                    [!IF "EthCtrlConfigGateStatus = 'OPEN'"!]
                        [!VAR "GateControlReg" = "bit:or(bit:shl(1,$FifoIdx),$GateControlReg)"!]
                    [!ENDIF!]
                    [!BREAK!]
                [!ENDIF!]
            [!ENDLOOP!]
            [!VAR "FifoIdx" = "$FifoIdx + 1"!]
        [!ENDFOR!]
    [!CODE!] [!"$GateControlReg"!]U[!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "PreemptionClassic"!][!//
[!NOCODE!][!//
        [!VAR "GateNums" = "num:i(count(EthPreemptionClassification/*) - 1)"!]
        [!VAR "PreempClassReg" = "num:i(0)"!]
        [!FOR "FifoIdx" = "0" TO "$GateNums"!]
            [!LOOP "EthPreemptionClassification/*"!]
                [!VAR "PreemptionFifoIdx" = "node:ref(EthPreemptionFifoRef)/EthCtrlConfigEgressFifoIdx"!]
                [!IF "$PreemptionFifoIdx = $FifoIdx"!]
                    [!IF "EthCtrlConfigPreemptionClassification = 'PREEMPTABLE'"!]
                        [!VAR "PreempClassReg" = "bit:or(bit:shl(1,$FifoIdx),$PreempClassReg)"!]
                    [!ENDIF!]
                    [!BREAK!]
                [!ENDIF!]
            [!ENDLOOP!]
            [!VAR "FifoIdx" = "$FifoIdx + 1"!]
        [!ENDFOR!]
        
    [!CODE!] [!"$PreempClassReg"!]U[!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//
/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define GMAC_MAC_ADDR_LENGTH        (6U)

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
#define ETH_START_SEC_CODE
#include "Eth_MemMap.h"

/*! @brief Channel callbacks external declarations */
[!NOCODE!]
[!IF "EthConfigSet/EthCtrlConfig/*/EthCtrlEnableRxInterrupt"!]
[!CODE!]extern void Eth_RxIrqCallback(const uint8 CtrlIdx, const uint8 DMAChannel);[!CR!][!ENDCODE!]
[!ENDIF!]

[!IF "EthConfigSet/EthCtrlConfig/*/EthCtrlEnableTxInterrupt"!]
[!CODE!]extern void Eth_TxIrqCallback(const uint8 CtrlIdx, const uint8 DMAChannel);[!CR!][!ENDCODE!]
[!ENDIF!]

[!IF "node:containsValue(EthConfigSet/EthCtrlConfig/*/EthCtrlVendorSpecific/EthCtrlConfigSafety/*, 'true')"!]
[!CODE!]extern void Eth_Ipw_SafetyIrqCallback(const uint8 CtrlIdx, const Gmac_Ip_SafetyErrorType Error);[!CR!][!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

#define ETH_STOP_SEC_CODE
#include "Eth_MemMap.h"

/*==================================================================================================
*                                      BUFFER DECLARATIONS
==================================================================================================*/
#define ETH_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Eth_MemMap.h"

[!NOCODE!]
    [!LOOP "node:order(EthConfigSet/EthCtrlConfig/*, 'EthCtrlIdx')"!]
        [!VAR "CtrlIdx" = "node:value(EthCtrlIdx)"!]

        [!LOOP "node:order(EthCtrlConfigIngress/EthCtrlConfigIngressFifo/*, 'EthCtrlConfigIngressFifoIdx')"!]
            [!VAR "RingIdx" = "node:value(EthCtrlConfigIngressFifoIdx)"!]
[!CODE!]
extern Gmac_Ip_BufferDescriptorType GMAC_[!"num:i($CtrlIdx)"!]_RxRing_[!"num:i($RingIdx)"!]_DescBuffer[];
extern uint8 GMAC_[!"num:i($CtrlIdx)"!]_RxRing_[!"num:i($RingIdx)"!]_DataBuffer[];
[!ENDCODE!]
        [!ENDLOOP!][!// RingIdx

        [!LOOP "node:order(EthCtrlConfigEgress/EthCtrlConfigEgressFifo/*, 'EthCtrlConfigEgressFifoIdx')"!]
            [!VAR "RingIdx" = "node:value(EthCtrlConfigEgressFifoIdx)"!]
[!CODE!]
extern Gmac_Ip_BufferDescriptorType GMAC_[!"num:i($CtrlIdx)"!]_TxRing_[!"num:i($RingIdx)"!]_DescBuffer[];
extern uint8 GMAC_[!"num:i($CtrlIdx)"!]_TxRing_[!"num:i($RingIdx)"!]_DataBuffer[];
[!ENDCODE!]
        [!ENDLOOP!][!// RingIdx

    [!ENDLOOP!][!// CtrlIdx
[!ENDNOCODE!]

#define ETH_STOP_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Eth_MemMap.h"

/*==================================================================================================
*                                   STATE STRUCTURE DECLARATIONS
==================================================================================================*/
#define ETH_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Eth_MemMap.h"

[!NOCODE!]
    [!LOOP "node:order(EthConfigSet/EthCtrlConfig/*, 'EthCtrlIdx')"!]
        [!VAR "CtrlIdx" = "node:value(EthCtrlIdx)"!]
[!CODE!]
extern Gmac_Ip_StateType GMAC_[!"num:i($CtrlIdx)"!]_StateStructure;
[!ENDCODE!]
    [!ENDLOOP!][!// CtrlIdx
[!ENDNOCODE!]

#define ETH_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Eth_MemMap.h"

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/
#define ETH_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"


/*! @brief The MAC address(es) of the configured controller(s) */
[!NOCODE!]
    [!VAR "NumOfEthCtrls" = "count(EthConfigSet/EthCtrlConfig/*)"!]
    [!IF "$NumOfEthCtrls > 0"!]
        [!LOOP "node:order(EthConfigSet/EthCtrlConfig/*, 'EthCtrlIdx')"!]
[!CODE!][!//
static const uint8 GMAC_[!"num:i(EthCtrlIdx)"!]_au8MacAddrPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][GMAC_MAC_ADDR_LENGTH] = { [!CALL "GetPhysicalAddress"!] };
[!ENDCODE!]
        [!ENDLOOP!][!// EthCtrlConfig/*
    [!ENDIF!][!// $NumOfEthCtrls > 0
[!ENDNOCODE!]

[!NOCODE!]
    [!VAR "NumOfEthCtrls" = "count(EthConfigSet/EthCtrlConfig/*)"!]
    [!IF "$NumOfEthCtrls > 0"!]
        [!LOOP "node:order(EthConfigSet/EthCtrlConfig/*, 'EthCtrlIdx')"!]
            [!VAR "CtrlIdx" = "node:value(EthCtrlIdx)"!]
            [!VAR "NumOfRxFifos" = "count(EthCtrlConfigIngress/EthCtrlConfigIngressFifo/*)"!]
            [!IF "$NumOfRxFifos > 0"!]
[!CODE!]
/*! @brief Reception ring configuration structures */
static const Gmac_Ip_RxRingConfigType GMAC_[!"num:i($CtrlIdx)"!]_aRxRingConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($NumOfRxFifos)"!]U] =
{
[!ENDCODE!]
    [!VAR "FirstFifo" = "1"!]
    [!LOOP "node:order(EthCtrlConfigIngress/EthCtrlConfigIngressFifo/*, 'EthCtrlConfigIngressFifoIdx')"!]
[!CODE!][!//
    [!IF "num:i($FirstFifo)=0"!],[!ENDIF!][!VAR "FirstFifo"="0"!]
    /* The configuration structure for Rx Ring [!"num:i(EthCtrlConfigIngressFifoIdx)"!] */
    {
        /*.ringDesc = */GMAC_[!"num:i($CtrlIdx)"!]_RxRing_[!"num:i(EthCtrlConfigIngressFifoIdx)"!]_DescBuffer,
        /*.callback = */[!IF "../../../EthCtrlEnableRxInterrupt"!]Eth_RxIrqCallback[!ELSE!]NULL_PTR[!ENDIF!],
        /*.buffer = */GMAC_[!"num:i($CtrlIdx)"!]_RxRing_[!"num:i(EthCtrlConfigIngressFifoIdx)"!]_DataBuffer,
        /*.interrupts = */(uint32)[!IF "../../../EthCtrlEnableRxInterrupt"!]GMAC_CH_INTERRUPT_RI[!ELSE!]0U[!ENDIF!],
        /*.bufferLen = */[!"num:i(EthCtrlConfigIngressFifoBufLenByte)"!]U,
        /*.ringSize = */[!"num:i(EthCtrlConfigIngressFifoBufTotal)"!]U,
        /*.priorityMask = */[!CALL "GetPriorityMask",
                                   "QueueType" = "'Ingress'"!],
        /*.dmaBurstLength = */[!CALL "GetDmaBurstLength",
                                     "QueueType" = "'Ingress'",
                                     "DataBusWidthInBytes" = "ecu:list('Eth.EthCtrlConfig.GMAC.DataBusWidthInBytes.List')[$CtrlIdx + 1]",
                                     "MaxDataBusBurstLength" = "ecu:list('Eth.EthCtrlConfig.GMAC.MaxDataBusBurstLength.List')[$CtrlIdx + 1]"!]
    }
[!ENDCODE!]
    [!ENDLOOP!][!// EthCtrlConfigIngress/EthCtrlConfigIngressFifo/*
[!CODE!]
};
[!ENDCODE!]
            [!ENDIF!][!// $NumOfRxFifos > 0
        [!ENDLOOP!][!// EthCtrlConfig/*
    [!ENDIF!][!// $NumOfEthCtrls > 0
[!ENDNOCODE!]


[!NOCODE!]
    [!VAR "NumOfEthCtrls" = "count(EthConfigSet/EthCtrlConfig/*)"!]
    [!IF "$NumOfEthCtrls > 0"!]
        [!LOOP "node:order(EthConfigSet/EthCtrlConfig/*, 'EthCtrlIdx')"!]
            [!VAR "CtrlIdx" = "node:value(EthCtrlIdx)"!]
            [!VAR "NumOfTxFifos" = "count(EthCtrlConfigEgress/EthCtrlConfigEgressFifo/*)"!]
            [!IF "$NumOfTxFifos > 0"!]
                [!VAR "transferRate"!][!CALL "GetPortTransferRate"!][!ENDVAR!]
                [!VAR "bitsPerCycle"!][!CALL "GetPortBitsPerCycle"!][!ENDVAR!]
[!CODE!]
/*! @brief Transmission ring configuration structures */
static const Gmac_Ip_TxRingConfigType GMAC_[!"num:i($CtrlIdx)"!]_aTxRingConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($NumOfTxFifos)"!]U] =
{
[!ENDCODE!]
    [!VAR "FirstFifo" = "1"!]
    [!LOOP "node:order(EthCtrlConfigEgress/EthCtrlConfigEgressFifo/*, 'EthCtrlConfigEgressFifoIdx')"!]
        [!CALL "GenerateSchedulerInformation",
               "QueueNum" = "num:i(EthCtrlConfigEgressFifoIdx)",
               "TransferRate" = "$transferRate",
               "BitsPerCycle" = "$bitsPerCycle"!]
[!CODE!][!//
    [!IF "num:i($FirstFifo)=0"!],[!ENDIF!][!VAR "FirstFifo"="0"!]
    /* The configuration structure for Tx Ring [!"num:i(EthCtrlConfigEgressFifoIdx)"!] */
    {
        /*.weight = */[!"$PARAM_WEIGHT"!]U,
        /*.idleSlopeCredit = */[!"$PARAM_IDLE_SLOPE_CREDIT"!]U,
        /*.sendSlopeCredit = */[!"$PARAM_SEND_SLOPE_CREDIT"!]U,
        /*.hiCredit = */[!"$PARAM_HIGH_CREDIT"!]U,
        /*.loCredit = */[!"$PARAM_LOW_CREDIT"!],
        /*.ringDesc = */GMAC_[!"num:i($CtrlIdx)"!]_TxRing_[!"num:i(EthCtrlConfigEgressFifoIdx)"!]_DescBuffer,
        /*.callback = */[!IF "../../../EthCtrlEnableTxInterrupt"!]Eth_TxIrqCallback[!ELSE!]NULL_PTR[!ENDIF!],
        /*.buffer = */GMAC_[!"num:i($CtrlIdx)"!]_TxRing_[!"num:i(EthCtrlConfigEgressFifoIdx)"!]_DataBuffer,
        /*.interrupts = */(uint32)[!IF "../../../EthCtrlEnableTxInterrupt"!]GMAC_CH_INTERRUPT_TI[!ELSE!]0U[!ENDIF!],
        /*.bufferLen = */[!"num:i(EthCtrlConfigEgressFifoBufLenByte)"!]U,
        /*.ringSize = */[!"num:i(EthCtrlConfigEgressFifoBufTotal)"!]U,
        /*.priorityMask = */[!CALL "GetPriorityMask",
                               "QueueType" = "'Egress'"!],
        /*.dmaBurstLength = */[!CALL "GetDmaBurstLength",
                                     "QueueType" = "'Egress'",
                                     "DataBusWidthInBytes" = "ecu:list('Eth.EthCtrlConfig.GMAC.DataBusWidthInBytes.List')[$CtrlIdx + 1]",
                                     "MaxDataBusBurstLength" = "ecu:list('Eth.EthCtrlConfig.GMAC.MaxDataBusBurstLength.List')[$CtrlIdx + 1]"!],
        /*.queueOpMode = */[!"$PARAM_QUEUE_OP_MODE"!]
    }
[!ENDCODE!]
    [!ENDLOOP!][!// EthCtrlConfigEgress/EthCtrlConfigEgressFifo/*
[!CODE!]
};
[!ENDCODE!]
            [!ENDIF!][!// $NumOfTxFifos > 0
        [!ENDLOOP!][!// EthCtrlConfig/*
    [!ENDIF!][!// $NumOfEthCtrls > 0
[!ENDNOCODE!]

/*! @brief Module configuration structure */
[!NOCODE!]
    [!VAR "NumOfEthCtrls" = "count(EthConfigSet/EthCtrlConfig/*)"!]
    [!IF "$NumOfEthCtrls > 0"!]
        [!LOOP "node:order(EthConfigSet/EthCtrlConfig/*, 'EthCtrlIdx')"!]
[!CODE!]
static const Gmac_Ip_ConfigType GMAC_[!"num:i(EthCtrlIdx)"!]_InitConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    /*.rxRingCount = */[!"num:i(count(EthCtrlConfigIngress/EthCtrlConfigIngressFifo/*))"!]U,
    /*.txRingCount = */[!"num:i(count(EthCtrlConfigEgress/EthCtrlConfigEgressFifo/*))"!]U,
#if (FEATURE_GMAC_ASP_ALL || FEATURE_GMAC_ASP_ECC)
    /*.safetyInterrupts = */[!CALL "GetSafetyMask"!],
    /*.safetyCallback = */[!IF "node:containsValue(EthCtrlVendorSpecific/EthCtrlConfigSafety/*, 'true')"!]Eth_Ipw_SafetyIrqCallback[!ELSE!]NULL_PTR[!ENDIF!],
#endif
    /*.interrupts = */0U,
    /*.callback = */NULL_PTR,
    /*.miiMode = */GMAC_[!CALL "GetMacLayerType"!]_MODE,
    /*.txSchedAlgo = */GMAC_SCHED_ALGO_[!CALL "GetTxSchedulingAlgorithm"!],
    /*.speed = */GMAC_SPEED_[!"substring-after(EthCtrlMacLayerSpeed, 'ETH_MAC_LAYER_SPEED_')"!],
    /*.duplex = */GMAC_[!"substring-after(EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthDuplexMode, 'ETH_')"!],
    /*.macConfig = */[!CALL "GetMacConfigMask"!],
    /*.macPktFilterConfig = */[!CALL "GetMacPktFilterMask"!],
    /*.enableCtrl = */(boolean)FALSE
};
[!ENDCODE!]
        [!ENDLOOP!][!// EthCtrlConfig/*
    [!ENDIF!][!// $NumOfEthCtrls > 0
[!ENDNOCODE!]

[!NOCODE!]
    [!VAR "NumOfEthCtrls" = "count(EthConfigSet/EthCtrlConfig/*)"!]
    [!IF "$NumOfEthCtrls > 0"!]
        [!LOOP "node:order(EthConfigSet/EthCtrlConfig/*, 'EthCtrlIdx')"!]
            [!VAR "CtrlIdx" = "node:value(EthCtrlIdx)"!]
            [!VAR "NumOfTimeShaper" = "num:i(count(EthCtrlConfigTimeAwareShaper/EthCtrlConfigTimeAwareShaper/*))"!]
            [!LOOP "EthCtrlConfigTimeAwareShaper/EthCtrlConfigTimeAwareShaper/*"!]
                [!VAR "CycleNanoSecond" = "EthGateConfigCycleTime"!]
                [!CALL "SecondNanoSecondSplit", "NanoSecond" = "$CycleNanoSecond"!]
                [!VAR "NumOfGateList" = "num:i(count(EthGateControlList/*))"!]
                [!VAR "PreempClassReg"!][!CALL "PreemptionClassic"!][!ENDVAR!]
                [!VAR "EthReleaseAdvanceTime" = "num:i(EthReleaseAdvanceTime)"!]
                [!VAR "EthHoldAdvanceTime" = "num:i(EthHoldAdvanceTime)"!]
                [!VAR "EthGateConfigExtendTime" = "num:i(EthGateConfigExtendTime)"!]
                [!VAR "EthGateConfigBaseTimeInNanoSecond" = "num:i(EthGateConfigBaseTimeInNanoSecond)"!]
                [!VAR "EthGateConfigBaseTimeInSecond" = "num:i(EthGateConfigBaseTimeInSecond)"!]
            [!ENDLOOP!]
[!CODE!]
static const Gmac_Ip_TxGateControl Gmac_Ip_GateControlListPB[!"num:i($CtrlIdx)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!IF "$NumOfTimeShaper > 0"!][!"$NumOfGateList"!][!ELSE!]1[!ENDIF!]U] =
{
[!ENDCODE!]
    [!VAR "FirstGate" = "1"!]
    [!IF "$NumOfTimeShaper > 0"!]
        [!LOOP "EthCtrlConfigTimeAwareShaper/EthCtrlConfigTimeAwareShaper/*"!]
        [!LOOP "EthGateControlList/*"!]
        [!VAR "timeInterval" = "num:i(EthCtrlConfigInterval)"!]
[!CODE!][!//
    [!IF "num:i($FirstGate) = 0"!],[!ENDIF!][!VAR "FirstGate" = "0"!]
    {
        /*.timeInterval = */[!"$timeInterval"!]U,
        /*.gateControlFifo = */[!CALL "GateControl"!]
    }
[!ENDCODE!][!//
        [!ENDLOOP!]
        [!ENDLOOP!]
    [!ELSE!]
[!CODE!][!//
    {
        /*.timeInterval = */0U,
        /*.gateControlFifo = */0U
    }
[!ENDCODE!][!//  
    [!ENDIF!]
[!CODE!]
};
[!ENDCODE!]
[!CODE!]
static const Gmac_Ip_TxTimeAwareShaper GMAC_[!"num:i($CtrlIdx)"!]_pTxTimeShaperPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] = 
{
    /*.baseTimeSecond = */[!IF "$NumOfTimeShaper > 0"!][!"$EthGateConfigBaseTimeInSecond"!][!ELSE!]0[!ENDIF!]U,
    /*.baseTimenanoSecond = */[!IF "$NumOfTimeShaper > 0"!][!"$EthGateConfigBaseTimeInNanoSecond"!][!ELSE!]0[!ENDIF!]U,
    /*.cycleTimeSecond = */[!IF "$NumOfTimeShaper > 0"!][!"$cycleSecond"!][!ELSE!]0[!ENDIF!]U,
    /*.cycleTimeNanoSecond = */[!IF "$NumOfTimeShaper > 0"!][!"$cycleNanoSecond"!][!ELSE!]0[!ENDIF!]U,
    /*.extendedTime = */[!IF "$NumOfTimeShaper > 0"!][!"$EthGateConfigExtendTime"!][!ELSE!]0[!ENDIF!]U,
    /*.gateControlListDepth = */[!IF "$NumOfTimeShaper > 0"!][!"num:i($NumOfGateList)"!][!ELSE!]0[!ENDIF!]U,
    /*.releaseAdvanceTime = */[!IF "EthCtrlEnablePreemtion = 'true'"!][!"$EthReleaseAdvanceTime"!]U[!ELSE!]0U[!ENDIF!],
    /*.holdAdvanceTime = */[!IF "EthCtrlEnablePreemtion = 'true'"!][!"$EthHoldAdvanceTime"!]U[!ELSE!]0U[!ENDIF!],
    /*.preemptionClassification = */[!IF "EthCtrlEnablePreemtion = 'true'"!][!"$PreempClassReg"!]U[!ELSE!]0U[!ENDIF!],
    /*.GateControlList = */Gmac_Ip_GateControlListPB[!"num:i($CtrlIdx)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
};
[!ENDCODE!]
        [!ENDLOOP!]
    [!ENDIF!] 
[!ENDNOCODE!]

#define ETH_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
#define ETH_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"

[!NOCODE!]
    [!VAR "NumOfEthCtrls" = "num:i(count(EthConfigSet/EthCtrlConfig/*))"!]
    [!IF "num:i($NumOfEthCtrls) > 0"!]
[!CODE!]
const Gmac_CtrlConfigType Gmac_aCtrlConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"$NumOfEthCtrls"!]U] =
{
[!ENDCODE!]
    [!VAR "FirstCtrl" = "1"!]
    [!LOOP "EthConfigSet/EthCtrlConfig/*"!]
        [!VAR "CtrlHwId"  = "num:i(EthCtrlIdx)"!]
[!CODE!][!//
    [!IF "num:i($FirstCtrl)=0"!],[!ENDIF!][!VAR "FirstCtrl"="0"!]
    /* The configuration structure for Gmac_aCtrlConfig[[!"num:i(@index)"!]U] */
    {
        &GMAC_[!"$CtrlHwId"!]_StateStructure,
        &GMAC_[!"$CtrlHwId"!]_InitConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
        &GMAC_[!"$CtrlHwId"!]_aRxRingConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][0U],
        &GMAC_[!"$CtrlHwId"!]_aTxRingConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][0U],
        &GMAC_[!"$CtrlHwId"!]_au8MacAddrPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][0U],
        &GMAC_[!"$CtrlHwId"!]_pTxTimeShaperPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
    }
[!ENDCODE!]
    [!ENDLOOP!][!// EthCtrlConfig/*
[!CODE!][!//
};
[!ENDCODE!]
    [!ENDIF!][!// num:i($NumOfEthCtrls) > 0
[!ENDNOCODE!]

#define ETH_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */
