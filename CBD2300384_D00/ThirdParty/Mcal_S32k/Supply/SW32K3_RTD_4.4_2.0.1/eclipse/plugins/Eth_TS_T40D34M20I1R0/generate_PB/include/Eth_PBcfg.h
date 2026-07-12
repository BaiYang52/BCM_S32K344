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

#ifndef ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_H
#define ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_H

/**
*   @file
*
*   @addtogroup ETH_DRIVER_CONFIGURATION Ethernet Driver Configuration
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/

/*==================================================================================================
                                SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID                     43
#define ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION      4
#define ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION      4
#define ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION   0
#define ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION              2
#define ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION              0
#define ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION              1

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
                                       LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
[!NOCODE!]
[!IF "not((IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1))"!]
[!IF "var:defined('postBuildVariant')"!]
  [!VAR "postBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!]
[!CODE!]#define ETH_CONFIG_[!"$postBuildVariant"!]_PB \[!CR!][!ENDCODE!]
[!ELSE!]
  [!VAR "postBuildVariantNameUnderscore" = "string(null)"!]
[!CODE!]#define ETH_CONFIG_PB \[!CR!][!ENDCODE!]
[!ENDIF!]
    [!IF "EthGeneral/EthGeneralVendorSpecific/EthMulticoreSupport = 'true'"!]
        [!VAR "PartitionTable" = "''"!]
        [!LOOP "EthConfigSet/EthCtrlConfig/*/EthCtrlEcucPartitionRef"!]
            [!VAR "partitionShortName" = "node:name(node:ref(.))"!]
            [!IF "not(text:contains(text:split($PartitionTable), $partitionShortName))"!]
                [!CODE!][!WS "4"!]extern const Eth_ConfigType Eth_Config[!"$postBuildVariantNameUnderscore"!]_[!"$partitionShortName"!]; \[!CR!][!ENDCODE!]
            [!VAR "PartitionTable" = "concat($PartitionTable, ' ', $partitionShortName)"!]
            [!ENDIF!]
        [!ENDLOOP!]
    [!ELSE!]
        [!CODE!][!WS "4"!]extern const Eth_ConfigType Eth_Config[!"$postBuildVariantNameUnderscore"!];[!ENDCODE!]
    [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!]


[!NOCODE!]
[!IF "node:containsValue(EthConfigSet/EthCtrlConfig/*/EthCtrlEnableMii, 'true')"!]
[!CODE!][!//
#ifndef ETH_MDIO_CLAUSE22_API
    #define ETH_MDIO_CLAUSE22_API    (STD_ON)
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

[!NOCODE!]
[!IF "node:containsValue(EthConfigSet/EthCtrlConfig/*/EthCtrlEnableMmd, 'true')"!]
[!CODE!][!//
#ifndef ETH_MDIO_CLAUSE45_API
    #define ETH_MDIO_CLAUSE45_API    (STD_ON)
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

[!NOCODE!]
[!IF "node:containsValue(EthConfigSet/EthCtrlConfig/*/EthCtrlEnableRxInterrupt, 'true')"!]
[!CODE!][!//
#ifndef ETH_RX_IRQ_ENABLED
    #define ETH_RX_IRQ_ENABLED      (STD_ON)
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

[!NOCODE!]
[!IF "node:containsValue(EthConfigSet/EthCtrlConfig/*/EthCtrlEnableTxInterrupt, 'true')"!]
[!CODE!][!//
#ifndef ETH_TX_IRQ_ENABLED
    #define ETH_TX_IRQ_ENABLED        (STD_ON)
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

[!NOCODE!]
[!LOOP "EthConfigSet/EthCtrlConfig/*"!]
[!CODE!][!//
#ifndef EthConf_EthCtrlConfig_[!"node:name(.)"!]
    /*! @brief Controller symbolic name to be passed to API functions that require CtrlIdx */
    #define EthConf_EthCtrlConfig_[!"node:name(.)"!]    ([!"num:i(EthCtrlIdx)"!]U)
#elif (EthConf_EthCtrlConfig_[!"node:name(.)"!] != [!"num:i(EthCtrlIdx)"!])
    #error "[TPS_ECUC_06074] Invalid configuration due to symbolic name values"
#endif
[!ENDCODE!]
[!ENDLOOP!]
[!ENDNOCODE!]

[!NOCODE!]
[!LOOP "EthConfigSet/EthCtrlConfig/*/EthCtrlConfigIngress/EthCtrlConfigIngressFifo/*"!]
[!CODE!][!//
#ifndef EthConf_EthCtrlConfigIngressFifo_[!"node:name(.)"!]
    /*! @brief Controller symbolic name to be passed to API functions that require FifoIdx */
    #define EthConf_EthCtrlConfigIngressFifo_[!"node:name(.)"!]        ([!"num:i(EthCtrlConfigIngressFifoIdx)"!]U)
#elif (EthConf_EthCtrlConfigIngressFifo_[!"node:name(.)"!] != [!"num:i(EthCtrlConfigIngressFifoIdx)"!])
    #error "[TPS_ECUC_06074] Invalid configuration due to symbolic name values"
#endif
[!ENDCODE!]
[!ENDLOOP!]
[!ENDNOCODE!]

[!NOCODE!]
[!VAR "EthCtrlMaxTxFifosSupported" = "num:i(node:fallback('->num:max(EthConfigSet/EthCtrlConfig/*/EthCtrlConfigEgress/EthCtrlConfigEgressFifo/*/EthCtrlConfigEgressFifoIdx)', 0) + 1)"!]
[!CODE!][!//
/* Maximum number of configured Tx FIFOs */
#ifndef ETH_MAX_TXFIFO_SUPPORTED
    /*! @brief Maximum number of configured Tx FIFOs */
    #define ETH_MAX_TXFIFO_SUPPORTED        ([!"num:i($EthCtrlMaxTxFifosSupported)"!]U)
#elif (ETH_MAX_TXFIFO_SUPPORTED < [!"num:i($EthCtrlMaxTxFifosSupported)"!])
    #undef ETH_MAX_TXFIFO_SUPPORTED
    #define ETH_MAX_TXFIFO_SUPPORTED        ([!"num:i($EthCtrlMaxTxFifosSupported)"!]U) 
#endif
[!ENDCODE!]
[!ENDNOCODE!]

[!NOCODE!]
[!VAR "EthCtrlMaxRxFifosSupported" = "num:i(node:fallback('->num:max(EthConfigSet/EthCtrlConfig/*/EthCtrlConfigIngress/EthCtrlConfigIngressFifo/*/EthCtrlConfigIngressFifoIdx)', 0) + 1)"!]
[!CODE!][!//
/* Maximum number of configured Rx FIFOs */
#ifndef ETH_MAX_RXFIFO_SUPPORTED
    /*! @brief Maximum number of configured Rx FIFOs */
    #define ETH_MAX_RXFIFO_SUPPORTED        ([!"num:i($EthCtrlMaxRxFifosSupported)"!]U)
#elif (ETH_MAX_RXFIFO_SUPPORTED < [!"num:i($EthCtrlMaxRxFifosSupported)"!])
    #undef ETH_MAX_RXFIFO_SUPPORTED
    #define ETH_MAX_RXFIFO_SUPPORTED        ([!"num:i($EthCtrlMaxRxFifosSupported)"!]U) 
#endif
[!ENDCODE!]
[!ENDNOCODE!]

[!NOCODE!]
[!VAR "EthCtrlMaxTxBuffersSupported" = "num:i(node:fallback('->num:max(EthConfigSet/EthCtrlConfig/*/EthCtrlConfigEgress/EthCtrlConfigEgressFifo/*/EthCtrlConfigEgressFifoBufTotal)', 1))"!]
[!CODE!][!//
/* Maximum number of configured buffers per Tx FIFO */
#ifndef ETH_MAX_TXBUFF_SUPPORTED
    /*! @brief Maximum number of configured buffers per Tx FIFO */
    #define ETH_MAX_TXBUFF_SUPPORTED        ([!"num:i($EthCtrlMaxTxBuffersSupported)"!]U)
#elif (ETH_MAX_TXBUFF_SUPPORTED < [!"num:i($EthCtrlMaxTxBuffersSupported)"!])
    #undef ETH_MAX_TXBUFF_SUPPORTED
    #define ETH_MAX_TXBUFF_SUPPORTED        ([!"num:i($EthCtrlMaxTxBuffersSupported)"!]U) 
#endif
[!ENDCODE!]
[!ENDNOCODE!]

[!NOCODE!]
[!VAR "EthCtrlMaxRxBuffersSupported" = "num:i(node:fallback('->num:max(EthConfigSet/EthCtrlConfig/*/EthCtrlConfigIngress/EthCtrlConfigIngressFifo/*/EthCtrlConfigIngressFifoBufTotal)', 1))"!]
[!CODE!][!//
/* Maximum number of configured buffers per Rx FIFO */
#ifndef ETH_MAX_RXBUFF_SUPPORTED
    /*! @brief Maximum number of configured buffers per Rx FIFO */
    #define ETH_MAX_RXBUFF_SUPPORTED        ([!"num:i($EthCtrlMaxRxBuffersSupported)"!]U)
#elif (ETH_MAX_RXBUFF_SUPPORTED < [!"num:i($EthCtrlMaxRxBuffersSupported)"!])
    #undef ETH_MAX_RXBUFF_SUPPORTED
    #define ETH_MAX_RXBUFF_SUPPORTED        ([!"num:i($EthCtrlMaxRxBuffersSupported)"!]U) 
#endif
[!ENDCODE!]
[!ENDNOCODE!]

/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_H */
