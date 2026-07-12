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

#ifndef ETH_IPW_CFG_H
#define ETH_IPW_CFG_H

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
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!]#include "Eth_Ipw_[!"."!]_PBcfg.h"[!CR!][!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!]#include "Eth_Ipw_PBcfg.h"[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

#include "Eth_Cfg.h"
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ETH_IPW_CFG_VENDOR_ID                    43
#define ETH_IPW_CFG_AR_RELEASE_MAJOR_VERSION     4
#define ETH_IPW_CFG_AR_RELEASE_MINOR_VERSION     4
#define ETH_IPW_CFG_AR_RELEASE_REVISION_VERSION  0
#define ETH_IPW_CFG_SW_MAJOR_VERSION             2
#define ETH_IPW_CFG_SW_MINOR_VERSION             0
#define ETH_IPW_CFG_SW_PATCH_VERSION             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!][!//
/* Checks against Eth_Ipw_[!"."!]_PBcfg.h */
#if (ETH_IPW_CFG_VENDOR_ID != ETH_IPW_PBCFG_[!"."!]_VENDOR_ID)
    #error "Eth_Ipw_Cfg.h and Eth_Ipw_[!"."!]_PBcfg.h have different vendor ids"
#endif
#if ((ETH_IPW_CFG_AR_RELEASE_MAJOR_VERSION    != ETH_IPW_PBCFG_[!"."!]_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_IPW_CFG_AR_RELEASE_MINOR_VERSION    != ETH_IPW_PBCFG_[!"."!]_AR_RELEASE_MINOR_VERSION) || \
     (ETH_IPW_CFG_AR_RELEASE_REVISION_VERSION != ETH_IPW_PBCFG_[!"."!]_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Eth_Ipw_Cfg.h and Eth_Ipw_[!"."!]_PBcfg.h are different"
#endif
#if ((ETH_IPW_CFG_SW_MAJOR_VERSION != ETH_IPW_PBCFG_[!"."!]_SW_MAJOR_VERSION) || \
     (ETH_IPW_CFG_SW_MINOR_VERSION != ETH_IPW_PBCFG_[!"."!]_SW_MINOR_VERSION) || \
     (ETH_IPW_CFG_SW_PATCH_VERSION != ETH_IPW_PBCFG_[!"."!]_SW_PATCH_VERSION))
    #error "Software Version Numbers of Eth_Ipw_Cfg.h and Eth_Ipw_[!"."!]_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!][!//
/* Checks against Eth_Ipw_PBcfg.h */
#if (ETH_IPW_CFG_VENDOR_ID != ETH_IPW_PBCFG_VENDOR_ID)
    #error "Eth_Ipw_Cfg.h and Eth_Ipw_PBcfg.h have different vendor ids"
#endif
#if ((ETH_IPW_CFG_AR_RELEASE_MAJOR_VERSION    != ETH_IPW_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_IPW_CFG_AR_RELEASE_MINOR_VERSION    != ETH_IPW_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (ETH_IPW_CFG_AR_RELEASE_REVISION_VERSION != ETH_IPW_PBCFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Eth_Ipw_Cfg.h and Eth_Ipw_PBcfg.h are different"
#endif
#if ((ETH_IPW_CFG_SW_MAJOR_VERSION != ETH_IPW_PBCFG_SW_MAJOR_VERSION) || \
     (ETH_IPW_CFG_SW_MINOR_VERSION != ETH_IPW_PBCFG_SW_MINOR_VERSION) || \
     (ETH_IPW_CFG_SW_PATCH_VERSION != ETH_IPW_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Eth_Ipw_Cfg.h and Eth_Ipw_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

/* Checks against Eth_Cfg.h */
#if (ETH_IPW_CFG_VENDOR_ID != ETH_CFG_VENDOR_ID)
    #error "Eth_Ipw_Cfg.h and Eth_Cfg.h have different vendor ids"
#endif
#if ((ETH_IPW_CFG_AR_RELEASE_MAJOR_VERSION    != ETH_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_IPW_CFG_AR_RELEASE_MINOR_VERSION    != ETH_CFG_AR_RELEASE_MINOR_VERSION) || \
     (ETH_IPW_CFG_AR_RELEASE_REVISION_VERSION != ETH_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Eth_Ipw_Cfg.h and Eth_Cfg.h are different"
#endif
#if ((ETH_IPW_CFG_SW_MAJOR_VERSION != ETH_CFG_SW_MAJOR_VERSION) || \
     (ETH_IPW_CFG_SW_MINOR_VERSION != ETH_CFG_SW_MINOR_VERSION) || \
     (ETH_IPW_CFG_SW_PATCH_VERSION != ETH_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Eth_Ipw_Cfg.h and Eth_Cfg.h are different"
#endif
/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
[!NOCODE!]
[!CODE!]#define ETH_IPW_CONFIG_EXT \[!CR!][!ENDCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!][!//
    [!CODE!][!WS "4"!]ETH_IPW_CONFIG_[!"."!]_PB \[!CR!][!ENDCODE!]
[!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS "4"!]ETH_IPW_CONFIG_PB[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
#if STD_ON == ETH_DEM_EVENT_DETECT
/**
 * @brief    Type for holding DEM events
 * @details  It contains the necessary events to report to the DEM module
 */
typedef struct
{
    Mcal_DemErrorType ETH_IPW_E_SAFETY_Cfg;

} Eth_Ipw_DemEventsType;
#endif

/* Forward IPW structure declarations */
struct sGmac_CtrlConfigType;

/**
* @brief   The structure contains the hardware controller configuration type.
*/
typedef struct sEth_Ipw_CtrlConfigType
{
    const struct sGmac_CtrlConfigType *Eth_Ipw_pGmacCtrlConfig;
#if (STD_ON == ETH_DEM_EVENT_DETECT)
    const Eth_Ipw_DemEventsType Eth_Ipw_DemEventsList;
#endif

} Eth_Ipw_CtrlConfigType;

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* ETH_IPW_CFG_H */
