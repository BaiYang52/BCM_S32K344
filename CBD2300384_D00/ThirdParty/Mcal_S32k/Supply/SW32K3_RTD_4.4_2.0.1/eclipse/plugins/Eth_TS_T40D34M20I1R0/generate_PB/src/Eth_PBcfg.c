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
/**
*   @file
*   @implements Eth_PBcfg.c_Artifact
*   @addtogroup ETH_DRIVER_CONFIGURATION Ethernet Driver Configuration
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
#include "Eth_Cfg.h"
#include "Eth_Ipw_Cfg.h"

#if STD_ON == ETH_DEM_EVENT_DETECT
    #include "Dem_IntErrId.h"
#endif

#if (STD_ON == ETH_MDIO_CLAUSE22_API) || (STD_ON == ETH_MDIO_CLAUSE45_API)
[!NOCODE!]
[!VAR "EthTrcvHeaderTable" = "''"!]
[!LOOP "EthConfigSet/EthCtrlConfig/*"!]
    [!IF "EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthTrcvDriverVendorIdAndApiInfix != string(null)"!]
        [!VAR "HeaderName" = "concat('EthTrcv_', EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthTrcvDriverVendorIdAndApiInfix)"!]
    [!ELSE!]
        [!VAR "HeaderName" = "'EthTrcv'"!]
    [!ENDIF!]
    [!IF "not(text:contains(text:split($EthTrcvHeaderTable), $HeaderName))"!]
[!CODE!][!//
    #include "[!"$HeaderName"!].h"
[!ENDCODE!]
    [!ENDIF!]
    [!VAR "EthTrcvHeaderTable" = "concat($EthTrcvHeaderTable, ' ', $HeaderName)"!]
[!ENDLOOP!]
[!ENDNOCODE!][!//
#endif

#if STD_ON == ETH_SWT_MANAGEMENT_SUPPORT_API
[!NOCODE!]
[!VAR "EthSwtHeaderTable" = "''"!]
[!LOOP "EthConfigSet/EthCtrlConfig/*"!]
    [!IF "EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthSwtDriverVendorIdAndApiInfix != string(null)"!]
        [!VAR "HeaderName" = "concat('EthSwt_', EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthSwtDriverVendorIdAndApiInfix)"!]
    [!ELSE!]
        [!VAR "HeaderName" = "'EthSwt'"!]
    [!ENDIF!]
    [!IF "not(text:contains(text:split($EthSwtHeaderTable), $HeaderName))"!]
[!CODE!][!//
    #include "[!"$HeaderName"!].h"
[!ENDCODE!]
    [!ENDIF!]
    [!VAR "EthSwtHeaderTable" = "concat($EthSwtHeaderTable, ' ', $HeaderName)"!]
[!ENDLOOP!]
[!ENDNOCODE!][!//
#endif

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C                     43
#define ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C      4
#define ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C      4
#define ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C   0
#define ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C              2
#define ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C              0
#define ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Eth_Cfg.h */
#if (ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != ETH_CFG_VENDOR_ID)
    #error "Eth[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Eth_Cfg.h have different vendor ids"
#endif
#if ((ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != ETH_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != ETH_CFG_AR_RELEASE_MINOR_VERSION) || \
     (ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != ETH_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Eth[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Eth_Cfg.h are different"
#endif
#if ((ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != ETH_CFG_SW_MAJOR_VERSION) || \
     (ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != ETH_CFG_SW_MINOR_VERSION) || \
     (ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != ETH_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Eth[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Eth_Cfg.h are different"
#endif

/* Checks against Eth_Ipw_Cfg.h */
#if (ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != ETH_IPW_CFG_VENDOR_ID)
    #error "Eth[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Eth_Ipw_Cfg.h have different vendor ids"
#endif
#if ((ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != ETH_IPW_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != ETH_IPW_CFG_AR_RELEASE_MINOR_VERSION) || \
     (ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != ETH_IPW_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Eth[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Eth_Ipw_Cfg.h are different"
#endif
#if ((ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != ETH_IPW_CFG_SW_MAJOR_VERSION) || \
     (ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != ETH_IPW_CFG_SW_MINOR_VERSION) || \
     (ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != ETH_IPW_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Eth[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Eth_Ipw_Cfg.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
  #if STD_ON == ETH_DEM_EVENT_DETECT
    /* Checks against Dem_IntErrId.h */
    #if ((ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C != DEM_INTERRID_AR_RELEASE_MAJOR_VERSION) || \
         (ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C != DEM_INTERRID_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Eth[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Dem_IntErrId.h are different"
    #endif
  #endif

  #if (STD_ON == ETH_MDIO_CLAUSE22_API) || (STD_ON == ETH_MDIO_CLAUSE45_API)
[!NOCODE!]
    [!LOOP "text:split($EthTrcvHeaderTable)"!]
[!CODE!]
    /* Checks against [!"."!].h */
    #if ((ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C != [!"text:toupper(.)"!]_AR_RELEASE_MAJOR_VERSION) || \
         (ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C != [!"text:toupper(.)"!]_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Eth[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and [!"."!].h are different"
    #endif
[!ENDCODE!]
    [!ENDLOOP!]
[!ENDNOCODE!]
  #endif

  #if STD_ON == ETH_SWT_MANAGEMENT_SUPPORT_API
[!NOCODE!]
    [!LOOP "text:split($EthSwtHeaderTable)"!]
[!CODE!]
    /* Checks against [!"."!].h */
    #if ((ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C != [!"text:toupper(.)"!]_AR_RELEASE_MAJOR_VERSION) || \
         (ETH_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C != [!"text:toupper(.)"!]_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Eth[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and [!"."!].h are different"
    #endif
[!ENDCODE!]
    [!ENDLOOP!]
[!ENDNOCODE!]
  #endif
#endif

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
#define ETH_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"

/**
* @brief   Export IPW configurations.
*/
ETH_IPW_CONFIG_EXT

#define ETH_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
[!MACRO "GetIngressBufferCount"!][!//
[!NOCODE!]
    [!VAR "IngressBufferCount" = "num:i(0)"!]
    [!LOOP "EthCtrlConfigIngress/EthCtrlConfigIngressFifo/*"!]
        [!VAR "IngressBufferCount" = "num:i($IngressBufferCount) + num:i(EthCtrlConfigIngressFifoBufTotal)"!]
    [!ENDLOOP!]
    [!CODE!][!"num:i($IngressBufferCount)"!]U[!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetEgressBufferCount"!][!//
[!NOCODE!]
    [!VAR "EgressBufferCount" = "num:i(0)"!]
    [!LOOP "EthCtrlConfigEgress/EthCtrlConfigEgressFifo/*"!]
        [!VAR "EgressBufferCount" = "num:i($EgressBufferCount) + num:i(EthCtrlConfigEgressFifoBufTotal)"!]
    [!ENDLOOP!]
    [!CODE!][!"num:i($EgressBufferCount)"!]U[!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetVlanPcpToTxFifoIdxMap"!][!//
[!NOCODE!]
    [!FOR "Priority" = "0" TO "7"!]
        [!VAR "EgressFifoIdx" = "0"!]
        [!LOOP "EthCtrlConfigEgress/EthCtrlConfigEgressFifo/*/EthCtrlConfigEgressFifoPriorityAssignment/*"!]
            [!IF "num:i($Priority) = num:i(.)"!]
                [!VAR "EgressFifoIdx" = "num:i(../../EthCtrlConfigEgressFifoIdx)"!]
                [!BREAK!]
            [!ENDIF!]
        [!ENDLOOP!]
        [!CODE!][!"num:i($EgressFifoIdx)"!]U[!IF "$Priority < 7"!], [!ENDIF!][!ENDCODE!]
    [!ENDFOR!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetDemEventError", "ErrorName"!][!//
[!NOCODE!]
    [!VAR "ErrorSymbolicName" = "num:i(0)"!]
    [!VAR "ErrorEnabled"      = "false()"!]

    [!IF "(node:value(../../../EthGeneral/EthGeneralVendorSpecific/EthDisableDemEventDetect) = 'false') and node:exists(EthDemEventParameterRefs)"!]
        [!SELECT "EthDemEventParameterRefs"!]
            [!IF "node:exists($ErrorName) and node:refvalid($ErrorName)"!]
                [!VAR "ErrorEnabled" = "true()"!]
                [!VAR "ErrorRef" = "node:ref($ErrorName)"!]
                [!VAR "ErrorSymbolicName" = "concat('DemConf_DemEventParameter_', node:name($ErrorRef))"!]
            [!ENDIF!]
        [!ENDSELECT!]
    [!ENDIF!]

    [!CODE!](uint32)[!IF "$ErrorEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!], [!"$ErrorSymbolicName"!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//
/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
#define ETH_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"

[!NOCODE!]
    [!VAR "NumOfEthCtrls" = "count(EthConfigSet/EthCtrlConfig/*)"!]
    [!IF "$NumOfEthCtrls > 0"!]
[!CODE!]
static const Eth_EgressCfgType Eth_aEgressConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($NumOfEthCtrls)"!]U] =
{
[!ENDCODE!]

    [!VAR "FirstCtrl" = "1"!]
    [!LOOP "EthConfigSet/EthCtrlConfig/*"!]
[!CODE!][!//
    [!IF "num:i($FirstCtrl)=0"!],[!ENDIF!][!VAR "FirstCtrl"="0"!]
    /* The configuration structure for Eth_aEgressConfigPB[[!"num:i(@index)"!]U] - IP_[!"num:i(EthCtrlIdx)"!] */
    {
        [!CALL "GetEgressBufferCount"!], /* Total number of buffers across all Tx FIFOs */
        [!"num:i(count(EthCtrlConfigEgress/EthCtrlConfigEgressFifo/*))"!]U, /* Total number of configured Tx FIFOs */
        { [!CALL "GetVlanPcpToTxFifoIdxMap"!] } /* Map between VLAN PCPs and Tx FIFOs */
    }
[!ENDCODE!]
    [!ENDLOOP!][!// EthCtrlConfig/*
[!CODE!]
};
[!ENDCODE!]
    [!ENDIF!][!// $NumOfEthCtrls > 0
[!ENDNOCODE!]

[!NOCODE!]
    [!VAR "NumOfEthCtrls" = "count(EthConfigSet/EthCtrlConfig/*)"!]
    [!IF "$NumOfEthCtrls > 0"!]
[!CODE!]
static const Eth_IngressCfgType Eth_aIngressConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($NumOfEthCtrls)"!]U] =
{
[!ENDCODE!]

    [!VAR "FirstCtrl" = "1"!]
    [!LOOP "EthConfigSet/EthCtrlConfig/*"!]
[!CODE!][!//
    [!IF "num:i($FirstCtrl)=0"!],[!ENDIF!][!VAR "FirstCtrl"="0"!]
    /* The configuration structure for Eth_aIngressConfigPB[[!"num:i(@index)"!]U] - IP_[!"num:i(EthCtrlIdx)"!] */
    {
        [!CALL "GetIngressBufferCount"!], /* Total number of buffers across all Rx FIFOs */
        [!"num:i(count(EthCtrlConfigIngress/EthCtrlConfigIngressFifo/*))"!]U /* Total number of configured Rx FIFOs */
    }
[!ENDCODE!]
    [!ENDLOOP!][!// EthCtrlConfig/*
[!CODE!]
};
[!ENDCODE!]
    [!ENDIF!][!// $NumOfEthCtrls > 0
[!ENDNOCODE!]

[!NOCODE!]
    [!VAR "NumOfEthCtrls" = "count(EthConfigSet/EthCtrlConfig/*)"!]
    [!IF "$NumOfEthCtrls > 0"!]
[!CODE!]
static const Eth_ClockCfgType Eth_aClockConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($NumOfEthCtrls)"!]U] =
{
[!ENDCODE!]
    [!VAR "FirstCtrl" = "1"!]
    [!LOOP "EthConfigSet/EthCtrlConfig/*"!]
[!CODE!][!//
    [!IF "num:i($FirstCtrl)=0"!],[!ENDIF!][!VAR "FirstCtrl"="0"!]
    /* The configuration structure for Eth_aClockConfigPB[[!"num:i(@index)"!]U] - IP_[!"num:i(EthCtrlIdx)"!] */
    {
        [!"num:i(round(node:ref(EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthModuleReferenceClock)/McuClockReferencePointFrequency))"!]U, /* Frequency of the application interface (in Hz) */
        [!IF "node:exists(EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthTimeStampReferenceClock)"!][!"num:i(round(1000000000000 div node:ref(EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthTimeStampReferenceClock)/McuClockReferencePointFrequency))"!][!ELSE!]0[!ENDIF!]U, /* Number of picoseconds in a PTP clock period */
        [!IF "node:exists(EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthTimeStampRequiredAccuracy)"!][!"num:i(node:value(EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthTimeStampRequiredAccuracy))"!][!ELSE!]0[!ENDIF!]U  /* Number of nanoseconds in a PTP clock period for the required accuracy */
    }
[!ENDCODE!]
    [!ENDLOOP!][!// EthCtrlConfig/*
[!CODE!]
};
[!ENDCODE!]
    [!ENDIF!][!// $NumOfEthCtrls > 0
[!ENDNOCODE!]

[!NOCODE!]
    [!VAR "NumOfEthCtrls" = "count(EthConfigSet/EthCtrlConfig/*)"!]
    [!IF "$NumOfEthCtrls > 0"!]
[!CODE!]
static const Eth_CtrlCfgType Eth_aCtrlConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($NumOfEthCtrls)"!]U] =
{
[!ENDCODE!]

    [!VAR "FirstCtrl" = "1"!]
    [!LOOP "EthConfigSet/EthCtrlConfig/*"!]
[!CODE!][!//
    [!IF "num:i($FirstCtrl)=0"!],[!ENDIF!][!VAR "FirstCtrl"="0"!]
    /* The configuration structure for Eth_aCtrlConfigPB[[!"num:i(@index)"!]U] - IP_[!"num:i(EthCtrlIdx)"!] */
    {
        &Eth_Ipw_aCtrlConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i(@index)"!]U],
        &Eth_aEgressConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i(@index)"!]U],
        &Eth_aIngressConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i(@index)"!]U],
        &Eth_aClockConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i(@index)"!]U]
#if (STD_ON == ETH_DEM_EVENT_DETECT)
       ,{
            { [!CALL "GetDemEventError", "ErrorName" = "'ETH_E_ACCESS'"!] },
            { [!CALL "GetDemEventError", "ErrorName" = "'ETH_E_RX_FRAMES_LOST'"!] },
            { [!CALL "GetDemEventError", "ErrorName" = "'ETH_E_CRC'"!] },
            { [!CALL "GetDemEventError", "ErrorName" = "'ETH_E_UNDERSIZEFRAME'"!] },
            { [!CALL "GetDemEventError", "ErrorName" = "'ETH_E_OVERSIZEFRAME'"!] },
            { [!CALL "GetDemEventError", "ErrorName" = "'ETH_E_ALIGNMENT'"!] },
            { [!CALL "GetDemEventError", "ErrorName" = "'ETH_E_SINGLECOLLISION'"!] },
            { [!CALL "GetDemEventError", "ErrorName" = "'ETH_E_MULTIPLECOLLISION'"!] },
            { [!CALL "GetDemEventError", "ErrorName" = "'ETH_E_LATECOLLISION'"!] }
        }
#endif
#if (STD_ON == ETH_MDIO_CLAUSE22_API) || (STD_ON == ETH_MDIO_CLAUSE45_API)
       ,{
            &EthTrcv_[!IF "EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthTrcvDriverVendorIdAndApiInfix != """!][!"EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthTrcvDriverVendorIdAndApiInfix"!]_[!ENDIF!]ReadMiiIndication,
            &EthTrcv_[!IF "EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthTrcvDriverVendorIdAndApiInfix != """!][!"EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthTrcvDriverVendorIdAndApiInfix"!]_[!ENDIF!]WriteMiiIndication
        }
#endif
#if STD_ON == ETH_SWT_MANAGEMENT_SUPPORT_API
       ,{
            &EthSwt_[!IF "EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthSwtDriverVendorIdAndApiInfix != """!][!"EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthSwtDriverVendorIdAndApiInfix"!]_[!ENDIF!]EthTxAdaptBufferLength,
            &EthSwt_[!IF "EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthSwtDriverVendorIdAndApiInfix != """!][!"EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthSwtDriverVendorIdAndApiInfix"!]_[!ENDIF!]EthTxPrepareFrame,
            &EthSwt_[!IF "EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthSwtDriverVendorIdAndApiInfix != """!][!"EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthSwtDriverVendorIdAndApiInfix"!]_[!ENDIF!]EthTxProcessFrame,
            &EthSwt_[!IF "EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthSwtDriverVendorIdAndApiInfix != """!][!"EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthSwtDriverVendorIdAndApiInfix"!]_[!ENDIF!]EthTxFinishedIndication,
            &EthSwt_[!IF "EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthSwtDriverVendorIdAndApiInfix != """!][!"EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthSwtDriverVendorIdAndApiInfix"!]_[!ENDIF!]EthRxProcessFrame,
            &EthSwt_[!IF "EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthSwtDriverVendorIdAndApiInfix != """!][!"EthCtrlVendorSpecific/EthCtrlConfigGeneral/EthEthSwtDriverVendorIdAndApiInfix"!]_[!ENDIF!]EthRxFinishedIndication
        }
#endif
    }
[!ENDCODE!]
    [!ENDLOOP!][!// EthCtrlConfig/*
[!CODE!]
};
[!ENDCODE!]
    [!ENDIF!][!// $NumOfEthCtrls > 0
[!ENDNOCODE!]

#define ETH_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
[!MACRO "GetCtrlConfigStructName", "CtrlHwIndex", "EcucPartitionRef"!][!//
[!NOCODE!]
    [!VAR "CtrlConfigStructureName" = "'NULL_PTR'"!]
    [!VAR "postBuildVariantNameUnderscore" = "string(null)"!]
    [!IF "var:defined('postBuildVariant')"!]
        [!VAR "postBuildVariantNameUnderscore" = "concat('_', $postBuildVariant)"!]
    [!ENDIF!]
    [!VAR "Position" = "string(null)"!]

    [!LOOP "EthConfigSet/EthCtrlConfig/*"!]
        [!IF "num:i(EthCtrlIdx) = num:i($CtrlHwIndex)"!]
            [!IF "$EcucPartitionRef != string(null)"!]
                [!IF "node:exists(EthCtrlEcucPartitionRef) and node:refvalid(EthCtrlEcucPartitionRef)"!]
                    [!IF "node:value(EthCtrlEcucPartitionRef) = $EcucPartitionRef"!]
                        [!VAR "Position" = "num:i(@index)"!]
                        [!VAR "CtrlConfigStructureName" = "concat('&Eth_aCtrlConfigPB', $postBuildVariantNameUnderscore, '[', $Position, 'U]')"!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ELSE!]
                [!VAR "Position" = "num:i(@index)"!]
                [!VAR "CtrlConfigStructureName" = "concat('&Eth_aCtrlConfigPB', $postBuildVariantNameUnderscore, '[', $Position, 'U]')"!]
            [!ENDIF!]
            [!BREAK!]
        [!ENDIF!]
    [!ENDLOOP!]

    [!CODE!][!"$CtrlConfigStructureName"!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetConfigStructName", "EcucPartitionRef"!][!//
[!NOCODE!]
    [!VAR "ConfigStructureName" = "'NULL_PTR'"!]  
    [!LOOP "EthConfigSet/EthCtrlConfig/*"!]
        [!IF "node:exists(EthCtrlEcucPartitionRef) and node:refvalid(EthCtrlEcucPartitionRef)"!]
            [!IF "node:value(EthCtrlEcucPartitionRef) = $EcucPartitionRef"!]
                [!VAR "CtrlPartitionShortName" = "node:name(node:ref(EthCtrlEcucPartitionRef))"!]
                [!VAR "ConfigStructureName"    = "concat('&Eth_xPredefinedConfig_', $CtrlPartitionShortName)"!]
                [!BREAK!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]

    [!CODE!][!"$ConfigStructureName"!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetCoreId", "EcucPartitionRef"!][!//
[!NOCODE!]
    [!VAR "CoreId" = "num:i(-1)"!]
    [!SELECT "as:modconf('Os')[1]/OsApplication/*/OsAppEcucPartitionRef"!]
        [!IF "node:value(.) = $EcucPartitionRef"!]
            [!IF "node:refvalid(../OsApplicationCoreRef)"!]
                [!VAR "CoreId" = "num:i(node:value(node:ref(../OsApplicationCoreRef)/EcucCoreId))"!]
                [!BREAK!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDSELECT!]
    [!IF "$CoreId = num:i(-1)"!][!ERROR!]The referenced ECUC partition [!"$EcucPartitionRef"!] in EthEcucPartitionRef is not associated with any OsApplication or the OsApplication does not have a valid OsApplicationCoreRef[!ENDERROR!][!ENDIF!]

    [!CODE!][!"$CoreId"!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "GetEcucPartitionRef", "CoreId"!][!//
[!NOCODE!]
    [!VAR "EcucPartitionRef" = "string(null)"!]
    [!SELECT "as:modconf('Os')[1]/OsApplication/*/OsApplicationCoreRef"!]
        [!IF "node:refvalid(.) "!]
            [!IF "num:i($CoreId) = num:i(node:value(node:ref(.)/EcucCoreId))"!]
                [!VAR "EcucPartitionRef" = "node:value(../OsAppEcucPartitionRef)"!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDSELECT!]

    [!CODE!][!"$EcucPartitionRef"!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//


[!MACRO "GenerateSingleCoreConfig"!][!//
[!NOCODE!]
    [!VAR "NumOfEthCtrls" = "count(EthConfigSet/EthCtrlConfig/*)"!]
    [!IF "$NumOfEthCtrls > 0"!]
[!CODE!]
const Eth_ConfigType [!IF "(IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1)"!]Eth_xPredefinedConfig[!ELSE!]Eth_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ENDIF!] =
{
    {
[!ENDCODE!]
        [!VAR "MaxHwCtrlIdx" = "num:i(EthGeneral/EthMaxCtrlsSupported - 1)"!]
        [!FOR "CtrlHwIdx" = "0" TO "$MaxHwCtrlIdx"!]
[!CODE!]
        [!CALL "GetCtrlConfigStructName", "CtrlHwIndex" = "$CtrlHwIdx"!][!IF "$CtrlHwIdx!=$MaxHwCtrlIdx"!],[!ENDIF!] /* IP_[!"$CtrlHwIdx"!] */[!//
[!ENDCODE!]
        [!ENDFOR!][!// CtrlHwIdx
[!CODE!]
    }
};
[!ENDCODE!]
    [!ENDIF!][!// $NumOfEthCtrls > 0
[!ENDNOCODE!][!//
[!ENDMACRO!][!//


[!MACRO "GenerateMultiCorePartitionConfig"!][!//
[!NOCODE!]
    [!LOOP "EthGeneral/EthEcucPartitionRef/*"!]
        [!VAR "partitionRef"       = "node:value(.)"!]
        [!VAR "partitionShortName" = "node:name(node:ref(.))"!]
        [!VAR "partitionCoreIdx"!][!CALL "GetCoreId", "EcucPartitionRef" = "$partitionRef"!][!ENDVAR!]
        [!SELECT "../../.."!]
[!CODE!]
[!IF "(IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1)"!][!//
static const Eth_ConfigType Eth_xPredefinedConfig_[!"$partitionShortName"!] =
[!ELSE!][!//
const Eth_ConfigType Eth_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_[!"$partitionShortName"!] =
[!ENDIF!][!//
{
    [!"num:i($partitionCoreIdx)"!]U,
    {
[!ENDCODE!]
        [!VAR "MaxHwCtrlIdx" = "num:i(EthGeneral/EthMaxCtrlsSupported - 1)"!]
        [!FOR "CtrlHwIdx" = "0" TO "$MaxHwCtrlIdx"!]
[!CODE!]
        [!CALL "GetCtrlConfigStructName", "CtrlHwIndex" = "$CtrlHwIdx", "EcucPartitionRef" = "$partitionRef"!][!IF "$CtrlHwIdx!=$MaxHwCtrlIdx"!],[!ENDIF!] /* IP_[!"$CtrlHwIdx"!] */[!//
[!ENDCODE!]
        [!ENDFOR!][!// CtrlHwIdx
[!CODE!]
    }
};
[!ENDCODE!]
        [!ENDSELECT!]
    [!ENDLOOP!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//


[!MACRO "GenerateMultiCorePredefinedConfig"!][!//
[!NOCODE!]
    [!IF "(IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1)"!][!//
[!CODE!]
const Eth_ConfigType * const Eth_apxPredefinedConfig[ETH_MAX_COREIDX_SUPPORTED] =
{
[!ENDCODE!]
    [!VAR "MaxCoreIdx"!][!"num:i(node:fallback("->num:max(as:modconf('EcuC')/EcucHardware/*/EcucCoreDefinition/*/EcucCoreId)", -1))"!][!ENDVAR!]
    [!FOR "CoreIdx" = "0" TO "$MaxCoreIdx"!]
        [!VAR "OsEcucPartitionRef"!][!CALL "GetEcucPartitionRef", "CoreId" = "$CoreIdx"!][!ENDVAR!]
[!CODE!]
    [!CALL "GetConfigStructName", "EcucPartitionRef" = "$OsEcucPartitionRef"!][!IF "$CoreIdx!=$MaxCoreIdx"!],[!ENDIF!] /*EcucCoreId = [!"$CoreIdx"!] */[!//
[!ENDCODE!]
    [!ENDFOR!][!// CoreIdx
[!CODE!]
};
[!ENDCODE!]
    [!ENDIF!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
#define ETH_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"

[!IF "EthGeneral/EthGeneralVendorSpecific/EthMulticoreSupport = 'true'"!]
    [!CALL "GenerateMultiCorePartitionConfig"!]
    [!IF "(IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1)"!]
        [!CALL "GenerateMultiCorePredefinedConfig"!]
    [!ENDIF!]
[!ELSE!]
    [!CALL "GenerateSingleCoreConfig"!]
[!ENDIF!]

#define ETH_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"

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
