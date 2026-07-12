/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : DMA,CACHE,TRGMUX,LCU,EMIOS,FLEXIO
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

#ifndef CDD_MCL_CFG_H_
#define CDD_MCL_CFG_H_

#ifdef __cplusplus
extern "C"{
#endif

/**
* @page misra_violations MISRA-C:2012 violations
**/

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Mcl_Types.h"
#include "Mcal.h"
#include "Dma_Ip_Cfg.h"
#include "Lcu_Ip_Cfg.h"
#include "Trgmux_Ip_Cfg.h"
#include "Flexio_Mcl_Ip_Cfg.h"
#include "Emios_Mcl_Ip_Cfg.h"

[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
#include "CDD_Mcl_[!"."!]_PBcfg.h"
[!ENDLOOP!]
[!ELSE!]
#include "CDD_Mcl_PBcfg.h"
[!ENDIF!]

/*==================================================================================================
                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CDD_MCL_CFG_VENDOR_ID                       43
#define CDD_MCL_CFG_AR_RELEASE_MAJOR_VERSION        4
#define CDD_MCL_CFG_AR_RELEASE_MINOR_VERSION        4
#define CDD_MCL_CFG_AR_RELEASE_REVISION_VERSION     0
#define CDD_MCL_CFG_SW_MAJOR_VERSION                2
#define CDD_MCL_CFG_SW_MINOR_VERSION                0
#define CDD_MCL_CFG_SW_PATCH_VERSION                1
/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if header file and Mcal header file are of the same Autosar version */
#if ((CDD_MCL_CFG_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
     (CDD_MCL_CFG_AR_RELEASE_MINOR_VERSION != MCAL_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of CDD_Mcl_Cfg.h and Mcal.h are different"
#endif
#endif

/* Check if header file and Mcl_Types.h file are of the same vendor */
#if (CDD_MCL_CFG_VENDOR_ID != MCL_TYPES_VENDOR_ID)
    #error "CDD_Mcl_Cfg.h and Mcl_Types.h have different vendor ids"
#endif

/* Check if header file and Mcl_Types.h file are of the same Autosar version */
#if ((CDD_MCL_CFG_AR_RELEASE_MAJOR_VERSION != MCL_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (CDD_MCL_CFG_AR_RELEASE_MINOR_VERSION != MCL_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (CDD_MCL_CFG_AR_RELEASE_REVISION_VERSION != MCL_TYPES_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of CDD_Mcl_Cfg.h and Mcl_Types.h are different"
#endif

/* Check if header file and Mcl_Types.h file are of the same Software version */
#if ((CDD_MCL_CFG_SW_MAJOR_VERSION != MCL_TYPES_SW_MAJOR_VERSION) || \
     (CDD_MCL_CFG_SW_MINOR_VERSION != MCL_TYPES_SW_MINOR_VERSION) || \
     (CDD_MCL_CFG_SW_PATCH_VERSION != MCL_TYPES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Mcl_Cfg.h and Mcl_Types.h are different"
#endif

/* Check if header file and Dma_Ip_Cfg.h file are of the same vendor */
#if (CDD_MCL_CFG_VENDOR_ID != DMA_IP_CFG_VENDOR_ID)
    #error "CDD_Mcl_Cfg.h and Dma_Ip_Cfg.h have different vendor ids"
#endif

/* Check if header file and Dma_Ip_Cfg.h file are of the same Autosar version */
#if ((CDD_MCL_CFG_AR_RELEASE_MAJOR_VERSION != DMA_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (CDD_MCL_CFG_AR_RELEASE_MINOR_VERSION != DMA_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (CDD_MCL_CFG_AR_RELEASE_REVISION_VERSION != DMA_IP_CFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of CDD_Mcl_Cfg.h and Dma_Ip_Cfg.h are different"
#endif

/* Check if header file and Dma_Ip_Cfg.h file are of the same Software version */
#if ((CDD_MCL_CFG_SW_MAJOR_VERSION != DMA_IP_CFG_SW_MAJOR_VERSION) || \
     (CDD_MCL_CFG_SW_MINOR_VERSION != DMA_IP_CFG_SW_MINOR_VERSION) || \
     (CDD_MCL_CFG_SW_PATCH_VERSION != DMA_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Mcl_Cfg.h and Dma_Ip_Cfg.h are different"
#endif

/* Check if header file and Lcu_Ip_Cfg.h file are of the same vendor */
#if (CDD_MCL_CFG_VENDOR_ID != LCU_IP_CFG_VENDOR_ID)
    #error "CDD_Mcl_Cfg.h and Lcu_Ip_Cfg.h have different vendor ids"
#endif

/* Check if header file and Lcu_Ip_Cfg.h file are of the same Autosar version */
#if ((CDD_MCL_CFG_AR_RELEASE_MAJOR_VERSION != LCU_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (CDD_MCL_CFG_AR_RELEASE_MINOR_VERSION != LCU_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (CDD_MCL_CFG_AR_RELEASE_REVISION_VERSION != LCU_IP_CFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of CDD_Mcl_Cfg.h and Lcu_Ip_Cfg.h are different"
#endif

/* Check if header file and Lcu_Ip_Cfg.h file are of the same Software version */
#if ((CDD_MCL_CFG_SW_MAJOR_VERSION != LCU_IP_CFG_SW_MAJOR_VERSION) || \
     (CDD_MCL_CFG_SW_MINOR_VERSION != LCU_IP_CFG_SW_MINOR_VERSION) || \
     (CDD_MCL_CFG_SW_PATCH_VERSION != LCU_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Mcl_Cfg.h and Lcu_Ip_Cfg.h are different"
#endif

/* Check if header file and Trgmux_Ip_Cfg.h file are of the same vendor */
#if (CDD_MCL_CFG_VENDOR_ID != TRGMUX_IP_CFG_VENDOR_ID)
    #error "CDD_Mcl_Cfg.h and Trgmux_Ip_Cfg.h have different vendor ids"
#endif

/* Check if header file and Trgmux_Ip_Cfg.h file are of the same Autosar version */
#if ((CDD_MCL_CFG_AR_RELEASE_MAJOR_VERSION != TRGMUX_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (CDD_MCL_CFG_AR_RELEASE_MINOR_VERSION != TRGMUX_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (CDD_MCL_CFG_AR_RELEASE_REVISION_VERSION != TRGMUX_IP_CFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of CDD_Mcl_Cfg.h and Trgmux_Ip_Cfg.h are different"
#endif

/* Check if header file and Trgmux_Ip_Cfg.h file are of the same Software version */
#if ((CDD_MCL_CFG_SW_MAJOR_VERSION != TRGMUX_IP_CFG_SW_MAJOR_VERSION) || \
     (CDD_MCL_CFG_SW_MINOR_VERSION != TRGMUX_IP_CFG_SW_MINOR_VERSION) || \
     (CDD_MCL_CFG_SW_PATCH_VERSION != TRGMUX_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Mcl_Cfg.h and Trgmux_Ip_Cfg.h are different"
#endif

/* Check if header file and Flexio_Mcl_Ip_Cfg.h file are of the same vendor */
#if (CDD_MCL_CFG_VENDOR_ID != FLEXIO_MCL_IP_CFG_VENDOR_ID)
    #error "CDD_Mcl_Cfg.h and Flexio_Mcl_Ip_Cfg.h have different vendor ids"
#endif

/* Check if header file and Flexio_Mcl_Ip_Cfg.h file are of the same Autosar version */
#if ((CDD_MCL_CFG_AR_RELEASE_MAJOR_VERSION != FLEXIO_MCL_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (CDD_MCL_CFG_AR_RELEASE_MINOR_VERSION != FLEXIO_MCL_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (CDD_MCL_CFG_AR_RELEASE_REVISION_VERSION != FLEXIO_MCL_IP_CFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of CDD_Mcl_Cfg.h and Flexio_Mcl_Ip_Cfg.h are different"
#endif

/* Check if header file and Flexio_Mcl_Ip_Cfg.h file are of the same Software version */
#if ((CDD_MCL_CFG_SW_MAJOR_VERSION != FLEXIO_MCL_IP_CFG_SW_MAJOR_VERSION) || \
     (CDD_MCL_CFG_SW_MINOR_VERSION != FLEXIO_MCL_IP_CFG_SW_MINOR_VERSION) || \
     (CDD_MCL_CFG_SW_PATCH_VERSION != FLEXIO_MCL_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Mcl_Cfg.h and Flexio_Mcl_Ip_Cfg.h are different"
#endif

/* Check if header file and Emios_Mcl_Ip_Cfg.h file are of the same vendor */
#if (CDD_MCL_CFG_VENDOR_ID != EMIOS_MCL_IP_CFG_VENDOR_ID)
    #error "CDD_Mcl_Cfg.h and Emios_Mcl_Ip_Cfg.h have different vendor ids"
#endif

/* Check if header file and Emios_Mcl_Ip_Cfg.h file are of the same Autosar version */
#if ((CDD_MCL_CFG_AR_RELEASE_MAJOR_VERSION != EMIOS_MCL_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (CDD_MCL_CFG_AR_RELEASE_MINOR_VERSION != EMIOS_MCL_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (CDD_MCL_CFG_AR_RELEASE_REVISION_VERSION != EMIOS_MCL_IP_CFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of CDD_Mcl_Cfg.h and Emios_Mcl_Ip_Cfg.h are different"
#endif

/* Check if header file and Emios_Mcl_Ip_Cfg.h file are of the same Software version */
#if ((CDD_MCL_CFG_SW_MAJOR_VERSION != EMIOS_MCL_IP_CFG_SW_MAJOR_VERSION) || \
     (CDD_MCL_CFG_SW_MINOR_VERSION != EMIOS_MCL_IP_CFG_SW_MINOR_VERSION) || \
     (CDD_MCL_CFG_SW_PATCH_VERSION != EMIOS_MCL_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Mcl_Cfg.h and Emios_Mcl_Ip_Cfg.h are different"
#endif

[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
/* Check if header file and CDD_Mcl_[!"."!]_PBcfg.h file are of the same vendor */
#if (CDD_MCL_CFG_VENDOR_ID != CDD_MCL_[!"."!]_PBCFG_VENDOR_ID)
    #error "CDD_Mcl_Cfg.h and CDD_Mcl_[!"."!]_PBcfg.h have different vendor ids"
#endif

/* Check if header file and CDD_Mcl_[!"."!]_PBcfg.h file are of the same Autosar version */
#if ((CDD_MCL_CFG_AR_RELEASE_MAJOR_VERSION != CDD_MCL_[!"."!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (CDD_MCL_CFG_AR_RELEASE_MINOR_VERSION != CDD_MCL_[!"."!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (CDD_MCL_CFG_AR_RELEASE_REVISION_VERSION != CDD_MCL_[!"."!]_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of CDD_Mcl_Cfg.h and CDD_Mcl_[!"."!]_PBcfg.h are different"
#endif

/* Check if header file and CDD_Mcl_[!"."!]_PBcfg.h file are of the same Software version */
#if ((CDD_MCL_CFG_SW_MAJOR_VERSION != CDD_MCL_[!"."!]_PBCFG_SW_MAJOR_VERSION) || \
     (CDD_MCL_CFG_SW_MINOR_VERSION != CDD_MCL_[!"."!]_PBCFG_SW_MINOR_VERSION) || \
     (CDD_MCL_CFG_SW_PATCH_VERSION != CDD_MCL_[!"."!]_PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Mcl_Cfg.h and CDD_Mcl_[!"."!]_PBcfg.h are different"
#endif
[!ENDLOOP!][!//
[!ELSE!][!//
/* Check if header file and CDD_Mcl_PBcfg.h file are of the same vendor */
#if (CDD_MCL_CFG_VENDOR_ID != CDD_MCL_PBCFG_VENDOR_ID)
    #error "CDD_Mcl_Cfg.h and CDD_Mcl_PBcfg.h have different vendor ids"
#endif

/* Check if header file and CDD_Mcl_PBcfg.h file are of the same Autosar version */
#if ((CDD_MCL_CFG_AR_RELEASE_MAJOR_VERSION != CDD_MCL_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (CDD_MCL_CFG_AR_RELEASE_MINOR_VERSION != CDD_MCL_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (CDD_MCL_CFG_AR_RELEASE_REVISION_VERSION != CDD_MCL_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of CDD_Mcl_Cfg.h and CDD_Mcl_PBcfg.h are different"
#endif

/* Check if header file and CDD_Mcl_PBcfg.h file are of the same Software version */
#if ((CDD_MCL_CFG_SW_MAJOR_VERSION != CDD_MCL_PBCFG_SW_MAJOR_VERSION) || \
     (CDD_MCL_CFG_SW_MINOR_VERSION != CDD_MCL_PBCFG_SW_MINOR_VERSION) || \
     (CDD_MCL_CFG_SW_PATCH_VERSION != CDD_MCL_PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Mcl_Cfg.h and CDD_Mcl_PBcfg.h are different"
#endif
[!ENDIF!][!//

/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/
/*-----------------------------------------------/
/  DEM & DET                                     /
/-----------------------------------------------*/
#define MCL_DEM_IS_AVAILABLE                       STD_OFF


[!IF "node:value(./MclGeneral/MclEnableDevErrorDetect)"!][!//
#define MCL_DET_IS_AVAILABLE                       STD_ON
[!ELSE!][!//
#define MCL_DET_IS_AVAILABLE                       STD_OFF
[!ENDIF!][!//

/*-----------------------------------------------/
/  DMA                                           /
/-----------------------------------------------*/
#define MCL_DMA_CRC_IS_AVAILABLE                   DMA_IP_DMACRC_IS_AVAILABLE

#define MCL_DMA_MASTER_ID_REPLICATION_IS_AVAILABLE DMA_IP_MASTER_ID_REPLICATION_IS_AVAILABLE

#define MCL_DMA_BUFFERED_WRITES_IS_AVAILABLE       DMA_IP_BUFFERED_WRITES_IS_AVAILABLE

#define MCL_DMA_STORE_DST_ADDR_IS_AVAILABLE        DMA_IP_STORE_DST_ADDR_IS_AVAILABLE

#define MCL_DMA_END_OF_PACKET_SIGNAL_IS_AVAILABLE  DMA_IP_END_OF_PACKET_SIGNAL_IS_AVAILABLE

#define MCL_DMA_PREEMPTION_IS_AVAILABLE            DMA_IP_PREEMPTION_IS_AVAILABLE

#define MCL_DMA_DISABLE_PREEMPT_IS_AVAILABLE       DMA_IP_DISABLE_PREEMPT_IS_AVAILABLE

/*-----------------------------------------------/
/  CACHE                                         /
/-----------------------------------------------*/

/*-----------------------------------------------/
/  TRGMUX                                        /
/-----------------------------------------------*/

/*-----------------------------------------------/
/  EMIOS                                         /
/-----------------------------------------------*/

/*-----------------------------------------------/
/  LCU                                           /
/-----------------------------------------------*/
#define MCL_LCU_SYNC_FUNC_IS_AVAILABLE            LCU_IP_SYNC_FUNC_IS_AVAILABLE

#define MCL_LCU_ASYNC_FUNC_IS_AVAILABLE           LCU_IP_ASYNC_FUNC_IS_AVAILABLE

/*-----------------------------------------------/
/  USERMODE                                      /
/-----------------------------------------------*/
[!IF "node:value(./MclGeneral/MclEnableUserModeSupport)"!][!//
#define MCL_USER_MODE_SUPPORT_IS_AVAILABLE         STD_ON
[!ELSE!][!//
#define MCL_USER_MODE_SUPPORT_IS_AVAILABLE         STD_OFF
[!ENDIF!][!//

#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    #if (STD_ON == MCL_USER_MODE_SUPPORT_IS_AVAILABLE)
        #error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running Mcl in user mode, the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined
    #endif /* (STD_ON == MCL_USER_MODE_SUPPORT_IS_AVAILABLE) */
#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT */

/*-----------------------------------------------/
/  PRECOMPILE                                    /
/-----------------------------------------------*/
[!IF "(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)"!][!//
/* Mcl Pre Compile Switch */
#define MCL_PRECOMPILE_SUPPORT                     STD_ON
[!ELSE!][!//
#define MCL_PRECOMPILE_SUPPORT                     STD_OFF
[!ENDIF!][!//

/*==================================================================================================
                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif    /* CDD_MCL_CFG_H_ */

/*==================================================================================================
 *                                        END OF FILE
==================================================================================================*/
