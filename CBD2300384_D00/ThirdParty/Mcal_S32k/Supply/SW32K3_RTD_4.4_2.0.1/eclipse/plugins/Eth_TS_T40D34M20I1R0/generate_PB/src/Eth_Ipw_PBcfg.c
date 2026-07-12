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
*
*   @addtogroup ETH_DRIVER_CONFIGURATION Ethernet Driver Configurations
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
#include "Eth_Ipw_Cfg.h"
#include "Gmac_Ip_Cfg.h"

#if STD_ON == ETH_DEM_EVENT_DETECT
    #include "Dem_IntErrId.h"
#endif

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C                     43
#define ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C      4
#define ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C      4
#define ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C   0
#define ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C              2
#define ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C              0
#define ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Eth_Ipw_Cfg.h */
#if (ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != ETH_IPW_CFG_VENDOR_ID)
    #error "Eth_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Eth_Ipw_Cfg.h have different vendor ids"
#endif
#if ((ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != ETH_IPW_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != ETH_IPW_CFG_AR_RELEASE_MINOR_VERSION) || \
     (ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != ETH_IPW_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Eth_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Eth_Ipw_Cfg.h are different"
#endif
#if ((ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != ETH_IPW_CFG_SW_MAJOR_VERSION) || \
     (ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != ETH_IPW_CFG_SW_MINOR_VERSION) || \
     (ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != ETH_IPW_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Eth_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Eth_Ipw_Cfg.h are different"
#endif

/* Checks against Gmac_Ip_Cfg.h */
#if (ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != GMAC_IP_CFG_VENDOR_ID)
    #error "Eth_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Gmac_Ip_Cfg.h have different vendor ids"
#endif
#if ((ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != GMAC_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != GMAC_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != GMAC_IP_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Eth_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Gmac_Ip_Cfg.h are different"
#endif
#if ((ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != GMAC_IP_CFG_SW_MAJOR_VERSION) || \
     (ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != GMAC_IP_CFG_SW_MINOR_VERSION) || \
     (ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != GMAC_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Eth_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Gmac_Ip_Cfg.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
  #if STD_ON == ETH_DEM_EVENT_DETECT
    /* Checks against Dem_IntErrId.h */
    #if ((ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C != DEM_INTERRID_AR_RELEASE_MAJOR_VERSION) || \
         (ETH_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C != DEM_INTERRID_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Eth_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Dem_IntErrId.h are different"
    #endif
  #endif
#endif

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
#define ETH_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"

/**
* @brief   Export GMAC configurations.
*/
GMAC_CONFIG_EXT

#define ETH_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
[!MACRO "GetDemEventError", "ErrorName"!][!//
[!NOCODE!]
    [!VAR "ErrorSymbolicName" = "num:i(0)"!]
    [!VAR "ErrorEnabled"      = "false()"!]

    [!IF "(node:value(../../../EthGeneral/EthGeneralVendorSpecific/EthDisableDemEventDetect) = 'false') and node:containsValue(EthCtrlVendorSpecific/EthCtrlConfigSafety/*, 'true')"!]
        [!SELECT "EthCtrlVendorSpecific/EthCtrlConfigSafety"!]
            [!IF "node:exists($ErrorName) and node:refvalid($ErrorName)"!]
                [!VAR "ErrorEnabled" = "true()"!]
                [!VAR "ErrorRef" = "node:ref($ErrorName)"!]
                [!VAR "ErrorSymbolicName" = "concat('DemConf_DemEventParameter_', node:name(node:ref($ErrorRef)))"!]
            [!ENDIF!]
        [!ENDSELECT!]
    [!ENDIF!]

    [!CODE!](uint32)[!IF "$ErrorEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!], [!"$ErrorSymbolicName"!][!ENDCODE!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


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
const Eth_Ipw_CtrlConfigType Eth_Ipw_aCtrlConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"$NumOfEthCtrls"!]U] =
{
[!ENDCODE!]
    [!VAR "FirstCtrl" = "1"!]
    [!LOOP "EthConfigSet/EthCtrlConfig/*"!]
        [!VAR "CtrlHwId"  = "num:i(EthCtrlIdx)"!]
[!CODE!][!//
    [!IF "num:i($FirstCtrl)=0"!],[!ENDIF!][!VAR "FirstCtrl"="0"!]
    /* The configuration structure for Eth_Ipw_aCtrlConfig[[!"num:i(@index)"!]U] */
    {
        &Gmac_aCtrlConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"$CtrlHwId"!]U]
#if (STD_ON == ETH_DEM_EVENT_DETECT)
       ,{
            { [!CALL "GetDemEventError", "ErrorName" = "'ETH_E_SAFETY'"!] }
        }
#endif
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
