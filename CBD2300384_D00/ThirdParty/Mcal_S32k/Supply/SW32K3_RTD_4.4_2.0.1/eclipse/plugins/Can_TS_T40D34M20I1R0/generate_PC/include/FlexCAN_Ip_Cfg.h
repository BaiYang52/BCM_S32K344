
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

#ifndef FLEXCAN_FLEXCAN_IP_CFG_H_
#define FLEXCAN_FLEXCAN_IP_CFG_H_

/**
*   @file FlexCAN_Ip_Cfg.h
*
*   @addtogroup FlexCAN
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif
[!INDENT "0"!]
/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
    [!LOOP "variant:all()"!]
        [!CODE!]#include "FlexCAN_Ip_[!"."!]_PBcfg.h"[!ENDCODE!][!CR!]
    [!ENDLOOP!]
[!ELSE!]
    [!CODE!]#include "FlexCAN_Ip_PBcfg.h"[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]
#include "OsIf.h"
#include "Reg_eSys.h"
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLEXCAN_IP_CFG_VENDOR_ID_H                      43
#define FLEXCAN_IP_CFG_AR_RELEASE_MAJOR_VERSION_H       4
#define FLEXCAN_IP_CFG_AR_RELEASE_MINOR_VERSION_H       4
#define FLEXCAN_IP_CFG_AR_RELEASE_REVISION_VERSION_H    0
#define FLEXCAN_IP_CFG_SW_MAJOR_VERSION_H               2
#define FLEXCAN_IP_CFG_SW_MINOR_VERSION_H               0
#define FLEXCAN_IP_CFG_SW_PATCH_VERSION_H               1
/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
[!IF "var:defined('postBuildVariant')"!]
    [!LOOP "variant:all()"!]
        /* Check if current file and FlexCAN_Ip_[!"."!]_PBcfg.h header file are of the same vendor */
        #if (FLEXCAN_IP_CFG_VENDOR_ID_H != FLEXCAN_IP_VENDOR_ID_[!"."!]_PBCFG_H)
            #error "FlexCAN_Ip_Cfg.h and FlexCAN_Ip_[!"."!]_PBcfg.h have different vendor ids"
        #endif
        /* Check if current file and FlexCAN_Ip_[!"."!]_PBcfg.h header file are of the same Autosar version */
        #if ((FLEXCAN_IP_CFG_AR_RELEASE_MAJOR_VERSION_H   != FLEXCAN_IP_AR_RELEASE_MAJOR_VERSION_[!"."!]_PBCFG_H) || \
            (FLEXCAN_IP_CFG_AR_RELEASE_MINOR_VERSION_H    != FLEXCAN_IP_AR_RELEASE_MINOR_VERSION_[!"."!]_PBCFG_H) || \
            (FLEXCAN_IP_CFG_AR_RELEASE_REVISION_VERSION_H != FLEXCAN_IP_AR_RELEASE_REVISION_VERSION_[!"."!]_PBCFG_H) \
            )
            #error "AutoSar Version Numbers of FlexCAN_Ip_Cfg.h and FlexCAN_Ip_[!"."!]_PBcfg.h are different"
        #endif
        /* Check if current file and FlexCAN_Ip_[!"."!]_PBcfg.h header file are of the same Software version */
        #if ((FLEXCAN_IP_CFG_SW_MAJOR_VERSION_H != FLEXCAN_IP_SW_MAJOR_VERSION_[!"."!]_PBCFG_H) || \
            (FLEXCAN_IP_CFG_SW_MINOR_VERSION_H != FLEXCAN_IP_SW_MINOR_VERSION_[!"."!]_PBCFG_H) || \
            (FLEXCAN_IP_CFG_SW_PATCH_VERSION_H != FLEXCAN_IP_SW_PATCH_VERSION_[!"."!]_PBCFG_H) \
            )
            #error "Software Version Numbers of FlexCAN_Ip_Cfg.h and FlexCAN_Ip_[!"."!]_PBcfg.h are different"
        #endif
    [!ENDLOOP!]
[!ELSE!]
    /* Check if current file and FlexCAN_Ip_PBcfg.h header file are of the same vendor */
    #if (FLEXCAN_IP_CFG_VENDOR_ID_H != FLEXCAN_IP_VENDOR_ID_PBCFG_H)
        #error "FlexCAN_Ip_Cfg.h and FlexCAN_Ip_PBcfg.h have different vendor ids"
    #endif
    /* Check if current file and FlexCAN_Ip_PBcfg.h header file are of the same Autosar version */
    #if ((FLEXCAN_IP_CFG_AR_RELEASE_MAJOR_VERSION_H   != FLEXCAN_IP_AR_RELEASE_MAJOR_VERSION_PBCFG_H) || \
        (FLEXCAN_IP_CFG_AR_RELEASE_MINOR_VERSION_H    != FLEXCAN_IP_AR_RELEASE_MINOR_VERSION_PBCFG_H) || \
        (FLEXCAN_IP_CFG_AR_RELEASE_REVISION_VERSION_H != FLEXCAN_IP_AR_RELEASE_REVISION_VERSION_PBCFG_H) \
        )
        #error "AutoSar Version Numbers of FlexCAN_Ip_Cfg.h and FlexCAN_Ip_PBcfg.h are different"
    #endif
    /* Check if current file and FlexCAN_Ip_PBcfg.h header file are of the same Software version */
    #if ((FLEXCAN_IP_CFG_SW_MAJOR_VERSION_H != FLEXCAN_IP_SW_MAJOR_VERSION_PBCFG_H) || \
        (FLEXCAN_IP_CFG_SW_MINOR_VERSION_H != FLEXCAN_IP_SW_MINOR_VERSION_PBCFG_H) || \
        (FLEXCAN_IP_CFG_SW_PATCH_VERSION_H != FLEXCAN_IP_SW_PATCH_VERSION_PBCFG_H) \
        )
        #error "Software Version Numbers of FlexCAN_Ip_Cfg.h and FlexCAN_Ip_PBcfg.h are different"
    #endif
[!ENDIF!]

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and OsIf header file are of the same version */
    #if ((FLEXCAN_IP_CFG_AR_RELEASE_MAJOR_VERSION_H   != OSIF_AR_RELEASE_MAJOR_VERSION) || \
        (FLEXCAN_IP_CFG_AR_RELEASE_MINOR_VERSION_H    != OSIF_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AutoSar Version Numbers of FlexCAN_Ip_Cfg.h and OsIf.h are different"
    #endif

    /* Check if current file and Reg_eSys header file are of the same version */
    #if ((FLEXCAN_IP_CFG_AR_RELEASE_MAJOR_VERSION_H   != REG_ESYS_AR_RELEASE_MAJOR_VERSION) || \
        (FLEXCAN_IP_CFG_AR_RELEASE_MINOR_VERSION_H    != REG_ESYS_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AutoSar Version Numbers of FlexCAN_Ip_Cfg.h and Reg_eSys.h are different"
    #endif
#endif
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
/* External Structures generated by FlexCAN_Ip_PBCfg */
[!NOCODE!]
[!CODE!]#define FLEXCAN_IP_CONFIG_EXT \[!CR!][!ENDCODE!]
    [!INDENT "4"!]
    [!IF "var:defined('postBuildVariant')"!]
        [!VAR "variantIndex"="0"!]
        [!VAR "variantNumber"="variant:size()"!]
        [!LOOP "variant:all()"!]
            [!VAR "variantIndex"="$variantIndex + 1"!]
            [!CODE!]FLEXCAN_IP_[!"."!]_PB_CFG [!IF "$variantIndex < $variantNumber"!]\[!ENDIF!][!CR!][!ENDCODE!]
        [!ENDLOOP!]
    [!ELSE!]
        [!CODE!]FLEXCAN_IP_PB_CFG[!ENDCODE!]
    [!ENDIF!]
    [!ENDINDENT!]
[!CODE!]
/* for HLD, no need to extern states */
#define FLEXCAN_IP_STATE_EXT[!CR!]
[!ENDCODE!]

[!IF "ecu:get('Can.CanConfigSet.CanPretendedNetworking') = 'STD_ON' and ./CanGeneral/CanPublicIcomSupport = 'true'"!]
[!CODE!]#define FLEXCAN_IP_PN_CONFIG_EXT \[!CR!][!ENDCODE!]
    [!INDENT "4"!]
    [!IF "var:defined('postBuildVariant')"!]
        [!VAR "variantIndex"="0"!]
        [!VAR "variantNumber"="variant:size()"!]
        [!LOOP "variant:all()"!]
            [!VAR "variantIndex"="$variantIndex + 1"!]
            [!CODE!]FLEXCAN_IP_PN_[!"."!]_PB_CFG [!IF "$variantIndex < $variantNumber"!]\[!ENDIF!][!CR!][!ENDCODE!]
        [!ENDLOOP!]
    [!ELSE!]
        [!CODE!]FLEXCAN_IP_PN_PB_CFG[!ENDCODE!]
    [!ENDIF!]
    [!ENDINDENT!]
[!ENDIF!]
[!ENDNOCODE!]

[!SELECT "CanGeneral"!]
/*! @brief Enables / Disables user mode support */
#define FLEXCAN_IP_ENABLE_USER_MODE_SUPPORT    ([!IF "CanEnableUserModeSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    [!WS "4"!]#if (STD_ON == FLEXCAN_IP_ENABLE_USER_MODE_SUPPORT)
        [!WS "8"!]#error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running FLEXCAN in user mode, MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined.
    [!WS "4"!]#endif /* (STD_ON == FLEXCAN_IP_ENABLE_USER_MODE_SUPPORT */
#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT */

#if ((STD_ON == FLEXCAN_IP_ENABLE_USER_MODE_SUPPORT) && defined(MCAL_FLEXCAN_REG_PROT_AVAILABLE))
    [!WS "4"!]#if (STD_ON == MCAL_FLEXCAN_REG_PROT_AVAILABLE)
        [!WS "8"!]#define FLEXCAN_IP_SET_USER_ACCESS_ALLOWED_AVAILABLE      (STD_ON)
    [!WS "4"!]#else
        [!WS "8"!]#define FLEXCAN_IP_SET_USER_ACCESS_ALLOWED_AVAILABLE      (STD_OFF)
    [!WS "4"!]#endif
#else
    [!WS "4"!]#define FLEXCAN_IP_SET_USER_ACCESS_ALLOWED_AVAILABLE      (STD_OFF)
#endif

[!IF "ecu:has('Can.CanConfig.CtrlRegProtNumber') = 'true'"!]
#if (FLEXCAN_IP_SET_USER_ACCESS_ALLOWED_AVAILABLE == STD_ON)
    [!WS "4"!]/*! @brief Use to check if the instances support UAA bit or not */
    [!WS "4"!]#define FLEXCAN_IP_CTRL_REG_PROT_SUPPORT_U8[!WS "4"!]((uint8)[!"ecu:get('Can.CanConfig.CtrlRegProtNumber')"!]U)
#endif
[!ENDIF!]

/* Time out value in uS */
#define FLEXCAN_IP_TIMEOUT_DURATION[!WS "4"!]([!"num:i(num:f(./CanTimeoutDuration) * 1000000)"!]U)

/* This this will set the timer source for osif that will be used for timeout */
#define FLEXCAN_IP_SERVICE_TIMEOUT_TYPE[!WS "4"!]([!"./CanTimeoutMethod"!])
[!ENDSELECT!]

/* @brief Maximum number of Message Buffers supported for payload size 8 for any of the CAN instances */
#define FLEXCAN_IP_FEATURE_MAX_MB_NUM[!WS "4"!]([!"ecu:get('Can.CanConfigSet.CanMB')"!]U)

[!NOCODE!]
    [!FOR "i" = "1" TO "ecu:get('Can.CanConfigSet.CanController')"!]
        [!VAR "CanNo" = "$i - 1"!]
        [!VAR "Value" = "ecu:list('Can.CanConfigSet.CanController.NoMB')[num:i($i)]"!]
        [!CODE!]
        /* @brief Maximum number of Message Buffers supported for payload size 8 for CAN[!"num:i($CanNo)"!] */
        #define FLEXCAN_IP_FEATURE_INSTANCE_[!"num:i($CanNo)"!]_MAX_MB_NUM[!WS "4"!]([!"$Value"!]U)
        [!ENDCODE!]
    [!ENDFOR!]
[!ENDNOCODE!]

/* @brief Array of maximum number of Message Buffers supported for payload size 8 for all the CAN instances */
#define FLEXCAN_IP_FEATURE_MAX_MB_NUM_ARRAY { \
[!NOCODE!]
[!FOR "i" = "1" TO "ecu:get('Can.CanConfigSet.CanController')"!]
    [!VAR "CanNo" = "$i - 1"!]
    [!CODE!][!WS "24"!]FLEXCAN_IP_FEATURE_INSTANCE_[!"num:i($CanNo)"!]_MAX_MB_NUM[!IF "$i < ecu:get('Can.CanConfigSet.CanController')"!], \[!CR!][!ELSE!] \[!ENDIF!][!ENDCODE!]
[!ENDFOR!]
[!ENDNOCODE!]
}

/* @brief Has DMA enable (bit field MCR[DMA]). */
#define FLEXCAN_IP_FEATURE_HAS_DMA_ENABLE[!WS "4"!][!IF "node:exists(CanConfigSet/CanController/*[node:exists(CanRxFiFo) and CanRxFiFo = 'CanEnhanceFiFo' and CanRxFiFo/CanEnhanceFiFoDmaEnable = 'true'])"!]STD_ON[!ELSEIF "node:exists(CanConfigSet/CanController/*[node:exists(CanRxFiFo) and CanRxFiFo = 'CanLegacyFiFo' and CanRxFiFo/CanLegacyFiFoDmaEnable = 'true'])"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
/* @brief Has Supervisor Mode MCR[SUPV] */
#define FLEXCAN_IP_FEATURE_HAS_SUPV[!WS "4"!]([!"ecu:get('Can.CanConfig.SupvAvailable')"!])
/* @brief Has Flexible Data Rate */
#define FLEXCAN_IP_FEATURE_HAS_FD[!WS "4"!]([!"ecu:get('Can.CanConfigSet.CanFdEnable')"!])
/* @bried FlexCAN has Detection And Correction of Memory Errors */
#define FLEXCAN_IP_FEATURE_HAS_MEM_ERR_DET[!WS "4"!]([!"ecu:get('Can.CanConfig.MemoryECC')"!])
/* @brief Has FlexCAN Enhanced Rx FIFO mode */
#define FLEXCAN_IP_FEATURE_HAS_ENHANCED_RX_FIFO[!WS "4"!]([!"ecu:get('Can.CanConfigSet.CanRxFiFoEnhancedEnable')"!])
/* @brief Has FlexCAN expandable memory */
#define FLEXCAN_IP_FEATURE_HAS_EXPANDABLE_MEMORY[!WS "4"!]([!"ecu:get('Can.CanConfigSet.CanExpandedMBMemoryListEnable')"!])
/* @brief Has FlexCAN Timestamp enabled */
#define FLEXCAN_IP_FEATURE_HAS_TS_ENABLE[!WS "4"!][!IF "(ecu:get('Can.CanConfigSet.TimeStampSupport')='STD_ON') and node:exists(CanGeneral/CanTimeStamp)"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
/* @brief Has FlexCAN High Resolution Timer for Time stamp CAN Message */
#define FLEXCAN_IP_FEATURE_HAS_HR_TIMER[!WS "4"!][!IF "(ecu:get('Can.CanConfigSet.TimeStampSupport')='STD_ON') and ecu:get('Can.CanConfigSet.TimeStampSrcList') != '' and node:exists(CanGeneral/CanTimeStamp)"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
[!IF "(ecu:get('Can.CanConfigSet.TimeStampSupport')='STD_ON') and ecu:get('Can.CanConfigSet.TimeStampSrcList') != ''"!]
#if (FLEXCAN_IP_FEATURE_HAS_HR_TIMER == STD_ON)
    [!VAR "sel_idx" = "0"!]
    [!LOOP "ecu:list('Can.CanConfigSet.TimeStampSrcList')"!]
    #define [!"."!]_VALUE   ([!"num:i($sel_idx)"!]U)
    [!VAR "sel_idx" = "$sel_idx + 1"!]
    [!ENDLOOP!]
    #define FLEXCAN_IP_HR_TIMESTAMP_SRC_SELECTABLE [!IF "count(ecu:list('Can.CanConfigSet.TimeStampSrcList')) > 1"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#endif /* (FLEXCAN_IP_FEATURE_HAS_HR_TIMER == STD_ON) */
[!ENDIF!]
/* @brief Has FlexCAN Enhanced Rx FIFO has interrupts available */
#define FLEXCAN_IP_FEATURE_HAS_ENHANCED_RX_FIFO_INTERRUPT[!WS "4"!](STD_OFF)
#define FLEXCAN_IP_FEATURE_HAS_PE_CLKSRC_SELECT[!WS "4"!]([!"ecu:get('Can.CanConfig.CtrlClksrcAvailable')"!])
#define FLEXCAN_IP_FEATURE_HAS_WAKE_UP_IRQ[!WS "4"!](STD_OFF)
/* @brief Enable use of Enhanced CBT time segments and ENTDC */
#define FLEXCAN_IP_FEATURE_HAS_ENHANCE_CBT[!WS "4"!]([!"ecu:get('Can.CanConfig.EnhanceCBTSuport')"!])
/* @brief Has FD Iso Option Mode  */
#define FLEXCAN_IP_FEATURE_SWITCHINGISOMODE[!WS "4"!]([!"ecu:get('Can.CanConfig.SwichingIsoMode')"!])
/* @brief Has Protocol exception Mode */
#define FLEXCAN_IP_FEATURE_PROTOCOLEXCEPTION[!WS "4"!]([!"ecu:get('Can.CanConfigSet.ProtocolException')"!])
/* @brief Has Edge filter Feature */
#define FLEXCAN_IP_FEATURE_EDGEFILTER[!WS "4"!]([!"ecu:get('Can.CanConfigSet.EdgeFilter')"!])
/* @brief Define if global variables need to be placed in non-cache area or not */
#define FLEXCAN_IP_FEATURE_NO_CACHE_NEEDED[!WS "4"!][!IF "num:i(count(./CanGeneral/CanEcucPartitionRef/*)) > 1"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!][!CR!]
/* @brief Has Pretending Network Feature */
#define FLEXCAN_IP_FEATURE_HAS_PRETENDED_NETWORKING[!WS "4"!]([!IF "(ecu:get('Can.CanConfigSet.CanPretendedNetworking') = 'STD_ON') and (./CanGeneral/CanPublicIcomSupport = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!NOCODE!]
[!SELECT "CanConfigSet"!]
[!VAR "MbIntFlag" = "0"!]
[!LOOP "node:order(./CanController/*, './CanControllerId')"!]
    [!VAR "CtrlID" = "./CanControllerId"!]
    [!VAR "RxIntFlag" = "0"!]
    [!VAR "TxIntFlag" = "0"!]
    [!IF "./CanRxProcessing != 'POLLING'"!]
        [!VAR "RxIntFlag" = "1"!]
    [!ELSEIF "./CanTxProcessing != 'POLLING'"!]
        [!VAR "TxIntFlag" = "1"!]
    [!ENDIF!]
    [!IF "num:i($RxIntFlag) = 1 or num:i($TxIntFlag) = 1"!]
        [!VAR "MbIntFlag" = "1"!]
        [!// #define CAN_FLEXCAN_[!"substring-after(./CanHwChannel, '_')"!]_MB_INTERRUPT_SUPPORT!]
    [!ENDIF!]
[!ENDLOOP!]
[!ENDSELECT!]
[!ENDNOCODE!]
/**
*   @brief      Can Rx/Tx common interrupt support
*/
#define FLEXCAN_IP_MB_INTERRUPT_SUPPORT[!WS "4"!][!IF "num:i($MbIntFlag) = 1"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

[!NOCODE!]
[!SELECT "CanConfigSet"!]
[!VAR "BusOffIntFlag" = "0"!]
[!LOOP "node:order(./CanController/*, './CanControllerId')"!]
    [!IF "./CanBusoffProcessing = 'INTERRUPT'"!]
        [!VAR "BusOffIntFlag" = "1"!]
        [!//#define CAN_FLEXCAN_[!"substring-after(./CanHwChannel, '_')"!]_BUSOFF_INTERRUPT_SUPPORT!]
    [!ENDIF!]
[!ENDLOOP!]
[!ENDSELECT!]
[!ENDNOCODE!]
/**
*   @brief      Can Bus Off interrupt support
*/
#define FLEXCAN_IP_BUSOFF_INTERRUPT_SUPPORT[!WS "4"!][!IF "num:i($BusOffIntFlag) = 1"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

[!NOCODE!]
[!SELECT "CanConfigSet"!]
[!VAR "ErrIntFlag" = "0"!]
[!LOOP "node:order(./CanController/*, './CanControllerId')"!]
    [!IF "node:exists(./CanErrorNotification)"!]
        [!VAR "ErrIntFlag" = "1"!]
        [!//#define CAN_FLEXCAN_[!"substring-after(./CanHwChannel, '_')"!]_ERROR_INTERRUPT_SUPPORT!]
    [!ENDIF!]
[!ENDLOOP!]
[!ENDSELECT!]
[!ENDNOCODE!]
/**
*   @brief      Can Error interrupt support
*/
#define FLEXCAN_IP_ERROR_INTERRUPT_SUPPORT[!WS "4"!][!IF "num:i($ErrIntFlag) = 1"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/* Defines the No Of Message Buffers Partions Suppport MBDSR regions */
#define FLEXCAN_IP_FEATURE_MBDSR_COUNT[!WS "4"!]([!"ecu:get('Can.CanConfigSet.NumOfRegion')"!]U)

/* Enable Development Error Detection */
#define FLEXCAN_IP_DEV_ERROR_DETECT[!WS "4"!][!IF "./CanGeneral/CanDevErrorDetect = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

[!ENDINDENT!]
#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @} */

#endif /* FLEXCAN_FLEXCAN_IP_CFG_H_ */
