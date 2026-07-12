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
#include "Mcal.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define GMAC_IP_CFG_VENDOR_ID_C                     43
#define GMAC_IP_CFG_AR_RELEASE_MAJOR_VERSION_C      4
#define GMAC_IP_CFG_AR_RELEASE_MINOR_VERSION_C      4
#define GMAC_IP_CFG_AR_RELEASE_REVISION_VERSION_C   0
#define GMAC_IP_CFG_SW_MAJOR_VERSION_C              2
#define GMAC_IP_CFG_SW_MINOR_VERSION_C              0
#define GMAC_IP_CFG_SW_PATCH_VERSION_C              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Gmac_Ip_Types.h */
#if (GMAC_IP_CFG_VENDOR_ID_C != GMAC_IP_TYPES_VENDOR_ID)
    #error "Gmac_Ip_Cfg.c and Gmac_Ip_Types.h have different vendor ids"
#endif
#if ((GMAC_IP_CFG_AR_RELEASE_MAJOR_VERSION_C    != GMAC_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (GMAC_IP_CFG_AR_RELEASE_MINOR_VERSION_C    != GMAC_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (GMAC_IP_CFG_AR_RELEASE_REVISION_VERSION_C != GMAC_IP_TYPES_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Gmac_Ip_Cfg.h and Gmac_Ip_Types.h are different"
#endif
#if ((GMAC_IP_CFG_SW_MAJOR_VERSION_C != GMAC_IP_TYPES_SW_MAJOR_VERSION) || \
     (GMAC_IP_CFG_SW_MINOR_VERSION_C != GMAC_IP_TYPES_SW_MINOR_VERSION) || \
     (GMAC_IP_CFG_SW_PATCH_VERSION_C != GMAC_IP_TYPES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Gmac_Ip_Cfg.c and Gmac_Ip_Types.h are different"
#endif

/* Checks against Gmac_Ip_Cfg.h */
#if (GMAC_IP_CFG_VENDOR_ID_C != GMAC_IP_CFG_VENDOR_ID)
    #error "Gmac_Ip_Cfg.c and Gmac_Ip_Cfg.h have different vendor ids"
#endif
#if ((GMAC_IP_CFG_AR_RELEASE_MAJOR_VERSION_C    != GMAC_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (GMAC_IP_CFG_AR_RELEASE_MINOR_VERSION_C    != GMAC_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (GMAC_IP_CFG_AR_RELEASE_REVISION_VERSION_C != GMAC_IP_CFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Gmac_Ip_Cfg.h and Gmac_Ip_Cfg.h are different"
#endif
#if ((GMAC_IP_CFG_SW_MAJOR_VERSION_C != GMAC_IP_CFG_SW_MAJOR_VERSION) || \
     (GMAC_IP_CFG_SW_MINOR_VERSION_C != GMAC_IP_CFG_SW_MINOR_VERSION) || \
     (GMAC_IP_CFG_SW_PATCH_VERSION_C != GMAC_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Gmac_Ip_Cfg.c and Gmac_Ip_Cfg.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Mcal.h */
    #if ((GMAC_IP_CFG_AR_RELEASE_MAJOR_VERSION_C != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (GMAC_IP_CFG_AR_RELEASE_MINOR_VERSION_C != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Gmac_Ip_Cfg.h and Mcal.h are different"
    #endif
#endif

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
#define ETH_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Eth_MemMap.h"

[!NOCODE!]
    [!VAR "MaxCtrlIdx" = "count(ecu:list('Eth.EthGeneral.EthController.List')) - 1"!]
    [!FOR "CtrlIdx" = "0" TO "$MaxCtrlIdx"!]

        [!VAR "MaxCtrlRxRingIdx" = "ecu:list('Eth.EthCtrlConfig.GMAC.MaxRxQueueSupport.List')[num:i($CtrlIdx + 1)] - 1"!]
        [!FOR "RingIdx" = "0" TO "$MaxCtrlRxRingIdx"!]
[!CODE!]
#ifdef GMAC_[!"num:i($CtrlIdx)"!]_RXRING_[!"num:i($RingIdx)"!]_DESCR
    /*! @brief Reception buffer descriptors for Rx Ring [!"num:i($RingIdx)"!] */
    VAR_ALIGN(Gmac_Ip_BufferDescriptorType GMAC_[!"num:i($CtrlIdx)"!]_RxRing_[!"num:i($RingIdx)"!]_DescBuffer[GMAC_[!"num:i($CtrlIdx)"!]_MAX_RXBUFF_SUPPORTED], FEATURE_GMAC_BUFFDESCR_ALIGNMENT_BYTES)
#endif
#ifdef GMAC_[!"num:i($CtrlIdx)"!]_RXRING_[!"num:i($RingIdx)"!]_DATA
    /*! @brief Reception data buffers for Rx Ring [!"num:i($RingIdx)"!] */
    VAR_ALIGN(uint8 GMAC_[!"num:i($CtrlIdx)"!]_RxRing_[!"num:i($RingIdx)"!]_DataBuffer[(GMAC_[!"num:i($CtrlIdx)"!]_MAX_RXBUFF_SUPPORTED * GMAC_[!"num:i($CtrlIdx)"!]_MAX_RXBUFFLEN_SUPPORTED)], FEATURE_GMAC_BUFF_ALIGNMENT_BYTES)
#endif
[!ENDCODE!]
        [!ENDFOR!][!// RingIdx

        [!VAR "MaxCtrlTxRingIdx" = "ecu:list('Eth.EthCtrlConfig.GMAC.MaxTxQueueSupport.List')[num:i($CtrlIdx + 1)] - 1"!]
        [!FOR "RingIdx" = "0" TO "$MaxCtrlTxRingIdx"!]
[!CODE!]
#ifdef GMAC_[!"num:i($CtrlIdx)"!]_TXRING_[!"num:i($RingIdx)"!]_DESCR
    /*! @brief Transmission buffer descriptors for Tx Ring [!"num:i($RingIdx)"!] */
    VAR_ALIGN(Gmac_Ip_BufferDescriptorType GMAC_[!"num:i($CtrlIdx)"!]_TxRing_[!"num:i($RingIdx)"!]_DescBuffer[GMAC_[!"num:i($CtrlIdx)"!]_MAX_TXBUFF_SUPPORTED], FEATURE_GMAC_BUFFDESCR_ALIGNMENT_BYTES)
#endif
#ifdef GMAC_[!"num:i($CtrlIdx)"!]_TXRING_[!"num:i($RingIdx)"!]_DATA
    /*! @brief Transmission data buffers for Tx Ring [!"num:i($RingIdx)"!] */
    VAR_ALIGN(uint8 GMAC_[!"num:i($CtrlIdx)"!]_TxRing_[!"num:i($RingIdx)"!]_DataBuffer[(GMAC_[!"num:i($CtrlIdx)"!]_MAX_TXBUFF_SUPPORTED * GMAC_[!"num:i($CtrlIdx)"!]_MAX_TXBUFFLEN_SUPPORTED)], FEATURE_GMAC_BUFF_ALIGNMENT_BYTES)
#endif
[!ENDCODE!]
        [!ENDFOR!][!// RingIdx

    [!ENDFOR!][!// CtrlIdx
[!ENDNOCODE!]

#define ETH_STOP_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Eth_MemMap.h"


#define ETH_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Eth_MemMap.h"

/* Instance state structures used to hold driver runtime data */
[!NOCODE!]
    [!VAR "MaxCtrlIdx" = "count(ecu:list('Eth.EthGeneral.EthController.List')) - 1"!]
    [!FOR "CtrlIdx" = "0" TO "$MaxCtrlIdx"!]
[!CODE!]
#ifdef INST_GMAC_[!"num:i($CtrlIdx)"!]
    Gmac_Ip_StateType GMAC_[!"num:i($CtrlIdx)"!]_StateStructure;
#endif
[!ENDCODE!]
    [!ENDFOR!][!// CtrlIdx
[!ENDNOCODE!][!//

#define ETH_STOP_SEC_VAR_CLEARED_UNSPECIFIED
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
