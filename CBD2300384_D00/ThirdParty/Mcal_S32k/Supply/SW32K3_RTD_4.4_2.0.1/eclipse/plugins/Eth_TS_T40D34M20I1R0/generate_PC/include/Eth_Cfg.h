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

#ifndef ETH_CFG_H
#define ETH_CFG_H

/**
*   @file
*   @implements Eth_Cfg.h_Artifact
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
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!]#include "Eth_[!"."!]_PBcfg.h"[!CR!][!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!]#include "Eth_PBcfg.h"[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

#include "Eth_GeneralTypes.h"
#include "Mcal.h"
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ETH_CFG_VENDOR_ID                    43
#define ETH_CFG_AR_RELEASE_MAJOR_VERSION     4
#define ETH_CFG_AR_RELEASE_MINOR_VERSION     4
#define ETH_CFG_AR_RELEASE_REVISION_VERSION  0
#define ETH_CFG_SW_MAJOR_VERSION             2
#define ETH_CFG_SW_MINOR_VERSION             0
#define ETH_CFG_SW_PATCH_VERSION             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!][!//
/* Checks against Eth_[!"."!]_PBcfg.h */
#if (ETH_CFG_VENDOR_ID != ETH_PBCFG_[!"."!]_VENDOR_ID)
    #error "Eth_Cfg.h and Eth_[!"."!]_PBcfg.h have different vendor ids"
#endif
#if ((ETH_CFG_AR_RELEASE_MAJOR_VERSION    != ETH_PBCFG_[!"."!]_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_CFG_AR_RELEASE_MINOR_VERSION    != ETH_PBCFG_[!"."!]_AR_RELEASE_MINOR_VERSION) || \
     (ETH_CFG_AR_RELEASE_REVISION_VERSION != ETH_PBCFG_[!"."!]_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Eth_Cfg.h and Eth_[!"."!]_PBcfg.h are different"
#endif
#if ((ETH_CFG_SW_MAJOR_VERSION != ETH_PBCFG_[!"."!]_SW_MAJOR_VERSION) || \
     (ETH_CFG_SW_MINOR_VERSION != ETH_PBCFG_[!"."!]_SW_MINOR_VERSION) || \
     (ETH_CFG_SW_PATCH_VERSION != ETH_PBCFG_[!"."!]_SW_PATCH_VERSION))
    #error "Software Version Numbers of Eth_Cfg.h and Eth_[!"."!]_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!][!//
/* Checks against Eth_PBcfg.h */
#if (ETH_CFG_VENDOR_ID != ETH_PBCFG_VENDOR_ID)
    #error "Eth_Cfg.h and Eth_PBcfg.h have different vendor ids"
#endif
#if ((ETH_CFG_AR_RELEASE_MAJOR_VERSION    != ETH_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_CFG_AR_RELEASE_MINOR_VERSION    != ETH_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (ETH_CFG_AR_RELEASE_REVISION_VERSION != ETH_PBCFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Eth_Cfg.h and Eth_PBcfg.h are different"
#endif
#if ((ETH_CFG_SW_MAJOR_VERSION != ETH_PBCFG_SW_MAJOR_VERSION) || \
     (ETH_CFG_SW_MINOR_VERSION != ETH_PBCFG_SW_MINOR_VERSION) || \
     (ETH_CFG_SW_PATCH_VERSION != ETH_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Eth_Cfg.h and Eth_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Eth_GeneralTypes.h */
    #if ((ETH_CFG_AR_RELEASE_MAJOR_VERSION != ETH_GENERALTYPES_AR_RELEASE_MAJOR_VERSION) || \
         (ETH_CFG_AR_RELEASE_MINOR_VERSION != ETH_GENERALTYPES_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Eth_Cfg.h and Eth_GeneralTypes.h are different"
    #endif

    /* Checks against Mcal.h */
    #if ((ETH_CFG_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (ETH_CFG_AR_RELEASE_MINOR_VERSION != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Eth_Cfg.h and Mcal.h are different"
    #endif
#endif
/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
[!NOCODE!]
[!IF "not((IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1))"!]
[!CODE!]#define ETH_CONFIG_EXT \[!CR!][!ENDCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!][!//
    [!CODE!][!WS "4"!]ETH_CONFIG_[!"."!]_PB \[!CR!][!ENDCODE!]
[!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS "4"!]ETH_CONFIG_PB[!ENDCODE!]
[!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!]

/*! @brief InstanceId of this module instance. If only one instance is present it shall have the Id 0 */
#define ETH_DRIVER_INSTANCE             ([!"num:i(EthGeneral/EthIndex)"!]U)    /* Instance number assigned to the driver */

/*! @brief Enables / Disables user mode support */
#define ETH_ENABLE_USER_MODE_SUPPORT    ([!IF "EthGeneral/EthGeneralVendorSpecific/EthEnableUserModeSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    #if (STD_ON == ETH_ENABLE_USER_MODE_SUPPORT)
        #error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running ETH in user mode, MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined.
    #endif /* (STD_ON == ETH_ENABLE_USER_MODE_SUPPORT */
#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT */

/*! @brief Enables / Disables multicore support */
#define ETH_MULTICORE_SUPPORT           ([!IF "EthGeneral/EthGeneralVendorSpecific/EthMulticoreSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*! @brief Number of cores in the system as defined in EcuC */
#define ETH_MAX_COREIDX_SUPPORTED       (uint32)([!IF "EthGeneral/EthGeneralVendorSpecific/EthMulticoreSupport"!][!"num:i(node:fallback("->num:max(as:modconf('EcuC')/EcucHardware/*/EcucCoreDefinition/*/EcucCoreId) + 1", 1))"!][!ELSE!]1[!ENDIF!]U)  

/*! @brief Enables / Disables PreCompile support when there is a single configuration */
#define ETH_PRECOMPILE_SUPPORT          ([!IF "(IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*! @brief Enables / Disables DET development errors detection and reporting */
#define ETH_DEV_ERROR_DETECT            ([!IF "EthGeneral/EthDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*! @brief Enables / Disables DEM events detection and reporting */
#define ETH_DEM_EVENT_DETECT            ([!IF "EthGeneral/EthGeneralVendorSpecific/EthDisableDemEventDetect"!]STD_OFF[!ELSE!]STD_ON[!ENDIF!])

/*! @brief Enables / Disables hardware offloading for IPv4 checksums */
#define ETH_ENABLE_OFFLOAD_CRC_IPV4     ([!IF "EthGeneral/EthCtrlOffloading/EthCtrlEnableOffloadChecksumIPv4"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*! @brief Enables / Disables hardware offloading for ICMP checksums */
#define ETH_ENABLE_OFFLOAD_CRC_ICMP     ([!IF "EthGeneral/EthCtrlOffloading/EthCtrlEnableOffloadChecksumICMP"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*! @brief Enables / Disables hardware offloading for TCP checksums */
#define ETH_ENABLE_OFFLOAD_CRC_TCP      ([!IF "EthGeneral/EthCtrlOffloading/EthCtrlEnableOffloadChecksumTCP"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*! @brief Enables / Disables hardware offloading for UDP checksums */
#define ETH_ENABLE_OFFLOAD_CRC_UDP      ([!IF "EthGeneral/EthCtrlOffloading/EthCtrlEnableOffloadChecksumUDP"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*! @brief Enables / Disables hardware offloading for checksums */
#define ETH_ENABLE_OFFLOAD_CRC          ([!IF "node:containsValue(EthGeneral/EthCtrlOffloading/*, 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*! @brief Enables / Disables the Switch Management APIs */
#define ETH_SWT_MANAGEMENT_SUPPORT_API  ([!IF "EthGeneral/EthGeneralVendorSpecific/EthSwtManagementSupportApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*! @brief Enables / Disables Eth_GetVersionInfo API */
#define ETH_VERSION_INFO_API            ([!IF "EthGeneral/EthVersionInfoApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*! @brief Enables / Disables Eth_UpdatePhysAddrFilter API */
#define ETH_UPDATE_PHYS_ADDR_FILTER_API ([!IF "EthGeneral/EthGeneralVendorSpecific/EthUpdatePhysAddrFilterApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*! @brief Enables / Disables Eth_GetCounterValues API */
#define ETH_GET_COUNTER_API             ([!IF "EthGeneral/EthGetCounterValuesApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*! @brief Enables / Disables Eth_GetTxStats API */
#define ETH_GET_TXSTATS_API             ([!IF "EthGeneral/EthGetTxStatsApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*! @brief Enables / Disables Eth_GetRxStats API */
#define ETH_GET_RXSTATS_API             ([!IF "EthGeneral/EthGetRxStatsApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*! @brief Enables / Disables Eth_GetTxErrorCounterValues API */
#define ETH_GET_TXERROR_COUNTER_API     ([!IF "EthGeneral/EthGetTxErrorCounterValuesApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/*! @brief Enables / Disables the GlobalTime APIs used amongst others by Global Time Synchronization over Ethernet */
#define ETH_GLOBAL_TIME_API             ([!IF "EthGeneral/EthGlobalTimeSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* PreCompile parameters residing in container EthCtrlConfig which has attribute postBuildVariantMultiplicity = true */
#ifndef ETH_MDIO_CLAUSE22_API    /* Will eventually be defined in Eth_<PBVName>_PBcfg.h */
  /*! @brief Enables / Disables Management Data I/O (MDIO) Clause 22for transceiver accesses */
  #define ETH_MDIO_CLAUSE22_API         (STD_OFF)
#endif

#ifndef ETH_MDIO_CLAUSE45_API    /* Will eventually be defined in Eth_<PBVName>_PBcfg.h */
  /*! @brief Enables / Disables Management Data I/O (MDIO) Clause45 for transceiver accesses */
  #define ETH_MDIO_CLAUSE45_API         (STD_OFF)
#endif

#ifndef ETH_RX_IRQ_ENABLED    /* Will eventually be defined in Eth_<PBVName>_PBcfg.h */
  /*! @brief Enables / Disables channel interrupts on Rx */
  #define ETH_RX_IRQ_ENABLED            (STD_OFF)
#endif

#ifndef ETH_TX_IRQ_ENABLED    /* Will eventually be defined in Eth_<PBVName>_PBcfg.h */
  /*! @brief Enables / Disables channel interrupts on Tx */
  #define ETH_TX_IRQ_ENABLED            (STD_OFF)
#endif

/*! @brief Number of controllers configured by the driver */
#define ETH_MAX_CTRLIDX_SUPPORTED       ([!"num:i(EthGeneral/EthMaxCtrlsSupported)"!]U)     /* Maximum configured controller hardware index + 1 (might be different from the total number of hardware controllers available on the platform) */

/*! @brief Number of VLAN Priority Code Points */
#define ETH_MAX_VLAN_PCP                (8U)

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
typedef struct
{
    uint16 BufferCount;    /* Total number of buffers across all Tx FIFOs */
    uint8 FifoCount;        /* Total number of configured Tx FIFOs */
    uint8 VlanPcpToFifoIdx[ETH_MAX_VLAN_PCP];   /* Map between VLAN PCPs and Tx FIFOs */

} Eth_EgressCfgType;

typedef struct
{
    uint16 BufferCount;    /* Total number of buffers across all Rx FIFOs */
    uint8 FifoCount;        /* Total number of configured Rx FIFOs */

} Eth_IngressCfgType;

typedef struct
{
    uint32 ModuleClock;                 /* Frequency of the application interface (in Hz) */
    uint32 TimeStampClockPeriodPs;      /* Number of picoseconds in a PTP clock period */
    uint32 TimeStampReqClockPeriodNs;   /* Number of nanoseconds in a PTP clock period for the required accuracy */

} Eth_ClockCfgType;

#if STD_ON == ETH_DEM_EVENT_DETECT
/**
 * @brief    Type for holding DEM events
 * @details  It contains the necessary events to report to the DEM module
 */
typedef struct
{
    Mcal_DemErrorType ETH_E_ACCESS_Cfg;
    Mcal_DemErrorType ETH_E_RX_FRAMES_LOST_Cfg;
    Mcal_DemErrorType ETH_E_CRC_Cfg;
    Mcal_DemErrorType ETH_E_UNDERSIZEFRAME_Cfg;
    Mcal_DemErrorType ETH_E_OVERSIZEFRAME_Cfg;
    Mcal_DemErrorType ETH_E_ALIGNMENT_Cfg;
    Mcal_DemErrorType ETH_E_SINGLECOLLISION_Cfg;
    Mcal_DemErrorType ETH_E_MULTIPLECOLLISION_Cfg;
    Mcal_DemErrorType ETH_E_LATECOLLISION_Cfg;

} Eth_DemEventsType;
#endif

#if STD_ON == ETH_DEM_EVENT_DETECT
/**
 * @brief    Type for enumerating DEM error IDs
 * @details  It contains a mapping between DEM events to error IDs
 */
typedef enum
{
    ETH_E_ACCESS            = 0U,
    ETH_E_RX_FRAMES_LOST    = 1U,
    ETH_E_CRC               = 2U,
    ETH_E_UNDERSIZEFRAME    = 3U,
    ETH_E_OVERSIZEFRAME     = 4U,
    ETH_E_ALIGNMENT         = 5U,
    ETH_E_SINGLECOLLISION   = 6U,
    ETH_E_MULTIPLECOLLISION = 7U,
    ETH_E_LATECOLLISION     = 8U,
    ETH_NUMBER_OF_DEM_IDS   = 9U   

} Eth_DemErrorIdType;
#endif


#if (STD_ON == ETH_MDIO_CLAUSE22_API) || (STD_ON == ETH_MDIO_CLAUSE45_API)
/**
 * @brief    Type for holding function pointers to the EthTrcv
 * @details  It contains the needed function pointers to EthTrcv functions.
 */
typedef struct
{
    void (*readMiiIndicationFunction)(uint8 CtrlIdx, uint8 TrcvIdx, uint8 RegIdx, uint16 RegValPtr);
    void (*writeMiiIndicationFunction)(uint8 CtrlIdx, uint8 TrcvIdx, uint8 RegIdx);

}Eth_EthTrcvFunctionsType;
#endif


#if STD_ON == ETH_SWT_MANAGEMENT_SUPPORT_API
/**
 * @brief    Type for holding function pointers to the EthSwt
 * @details  It contains the needed function pointers to EthSwt functions.
 */
typedef struct
{
    void (*TxAdaptBufferLengthFunction)(uint16* LengthPtr);
    Std_ReturnType (*TxPrepareFrameFunction)(uint8 CtrlIdx, Eth_BufIdxType BufIdx, uint8** DataPtr, uint16* LengthPtr);
    Std_ReturnType (*TxProcessFrameFunction)(uint8 CtrlIdx, Eth_BufIdxType BufIdx, uint8** DataPtr, uint16* LengthPtr);
    Std_ReturnType (*TxFinishedIndicationFunction)(uint8 CtrlIdx, Eth_BufIdxType BufIdx);
    Std_ReturnType (*RxProcessFrameFunction)(uint8 CtrlIdx, Eth_BufIdxType BufIdx, uint8** DataPtr, uint16* LengthPtr, boolean* IsMgmtFrameOnlyPtr);
    Std_ReturnType (*RxFinishedIndicationFunction)(uint8 CtrlIdx, Eth_BufIdxType BufIdx);

}Eth_EthSwtFunctionsType;
#endif

/* Forward IPW structure declarations */
struct sEth_Ipw_CtrlConfigType;

/**
* @brief         Type for Ethernet driver configuration values storage.
* @details       This type is used to collect and pass all configuration data
*                of one controller (except those which have only
*                precompile variant) to the Eth driver.
*/
typedef struct
{
    const struct sEth_Ipw_CtrlConfigType *Eth_pIpwCtrlConfig;
    const Eth_EgressCfgType *Eth_pEgressConfig;
    const Eth_IngressCfgType *Eth_pIngressConfig;
    const Eth_ClockCfgType *Eth_pClockConfig;
#if (STD_ON == ETH_DEM_EVENT_DETECT)
    const Eth_DemEventsType Eth_DemEventsList;
#endif
#if (STD_ON == ETH_MDIO_CLAUSE22_API) || (STD_ON == ETH_MDIO_CLAUSE45_API)
    const Eth_EthTrcvFunctionsType Eth_EthTrcvDriverFunctionList;  /**< @brief The structure with pointer functions to the EthTrcv driver functions */
#endif
#if STD_ON == ETH_SWT_MANAGEMENT_SUPPORT_API
    const Eth_EthSwtFunctionsType Eth_EthSwtDriverFunctionList; /**< @brief The structure with pointer functions to the EthSwt driver functions */
#endif

} Eth_CtrlCfgType;

/**
* @brief This type is used to group all multiple configurations.
* implements Eth_ConfigType_structure
*/   
typedef struct 
{
#if (STD_ON == ETH_MULTICORE_SUPPORT)
    uint32 PartitionCoreId;
#endif
    const Eth_CtrlCfgType *Eth_apCtrlConfig[ETH_MAX_CTRLIDX_SUPPORTED];

} Eth_ConfigType;

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

#endif /* ETH_CFG_H */

