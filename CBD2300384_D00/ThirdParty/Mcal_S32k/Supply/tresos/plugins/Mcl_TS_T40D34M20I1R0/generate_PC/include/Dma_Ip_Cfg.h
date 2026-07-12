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

#ifndef DMA_IP_CFG_H_
#define DMA_IP_CFG_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Dma_Ip_Types.h"
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
#include "Dma_Ip_[!"."!]_PBcfg.h"
[!ENDLOOP!][!//
[!ELSE!][!//
#include "Dma_Ip_PBcfg.h"
[!ENDIF!][!//

/*==================================================================================================
                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define DMA_IP_CFG_VENDOR_ID                       43
#define DMA_IP_CFG_AR_RELEASE_MAJOR_VERSION        4
#define DMA_IP_CFG_AR_RELEASE_MINOR_VERSION        4
#define DMA_IP_CFG_AR_RELEASE_REVISION_VERSION     0
#define DMA_IP_CFG_SW_MAJOR_VERSION                2
#define DMA_IP_CFG_SW_MINOR_VERSION                0
#define DMA_IP_CFG_SW_PATCH_VERSION                1

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
/* Check if header file and Dma_Ip_Types.h file are of the same vendor */
#if (DMA_IP_CFG_VENDOR_ID != DMA_IP_TYPES_VENDOR_ID)
    #error "Dma_Ip_Cfg.h and Dma_Ip_Types.h have different vendor ids"
#endif

/* Check if header file and Dma_Ip_Types.h file are of the same Autosar version */
#if ((DMA_IP_CFG_AR_RELEASE_MAJOR_VERSION != DMA_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (DMA_IP_CFG_AR_RELEASE_MINOR_VERSION != DMA_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (DMA_IP_CFG_AR_RELEASE_REVISION_VERSION != DMA_IP_TYPES_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Dma_Ip_Cfg.h and Dma_Ip_Types.h are different"
#endif

/* Check if header file and Dma_Ip_Types.h file are of the same Software version */
#if ((DMA_IP_CFG_SW_MAJOR_VERSION != DMA_IP_TYPES_SW_MAJOR_VERSION) || \
     (DMA_IP_CFG_SW_MINOR_VERSION != DMA_IP_TYPES_SW_MINOR_VERSION) || \
     (DMA_IP_CFG_SW_PATCH_VERSION != DMA_IP_TYPES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Dma_Ip_Cfg.h and Dma_Ip_Types.h are different"
#endif

[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
/* Check if header file and Dma_Ip_[!"."!]_PBcfg.h file are of the same vendor */
#if (DMA_IP_CFG_VENDOR_ID != DMA_IP_[!"."!]_PBCFG_VENDOR_ID)
    #error "Dma_Ip_Cfg.h and Dma_Ip_[!"."!]_PBcfg.h have different vendor ids"
#endif

/* Check if header file and Dma_Ip_[!"."!]_PBcfg.h file are of the same Autosar version */
#if ((DMA_IP_CFG_AR_RELEASE_MAJOR_VERSION != DMA_IP_[!"."!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (DMA_IP_CFG_AR_RELEASE_MINOR_VERSION != DMA_IP_[!"."!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (DMA_IP_CFG_AR_RELEASE_REVISION_VERSION != DMA_IP_[!"."!]_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Dma_Ip_Cfg.h and Dma_Ip_[!"."!]_PBcfg.h are different"
#endif

/* Check if header file and Dma_Ip_[!"."!]_PBcfg.h file are of the same Software version */
#if ((DMA_IP_CFG_SW_MAJOR_VERSION != DMA_IP_[!"."!]_PBCFG_SW_MAJOR_VERSION) || \
     (DMA_IP_CFG_SW_MINOR_VERSION != DMA_IP_[!"."!]_PBCFG_SW_MINOR_VERSION) || \
     (DMA_IP_CFG_SW_PATCH_VERSION != DMA_IP_[!"."!]_PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Dma_Ip_Cfg.h and Dma_Ip_[!"."!]_PBcfg.h are different"
#endif
[!ENDLOOP!][!//
[!ELSE!][!//
/* Check if header file and Dma_Ip_PBcfg.h file are of the same vendor */
#if (DMA_IP_CFG_VENDOR_ID != DMA_IP_PBCFG_VENDOR_ID)
    #error "Dma_Ip_Cfg.h and Dma_Ip_PBcfg.h have different vendor ids"
#endif

/* Check if header file and Dma_Ip_PBcfg.h file are of the same Autosar version */
#if ((DMA_IP_CFG_AR_RELEASE_MAJOR_VERSION != DMA_IP_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (DMA_IP_CFG_AR_RELEASE_MINOR_VERSION != DMA_IP_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (DMA_IP_CFG_AR_RELEASE_REVISION_VERSION != DMA_IP_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Dma_Ip_Cfg.h and Dma_Ip_PBcfg.h are different"
#endif

/* Check if header file and Dma_Ip_PBcfg.h file are of the same Software version */
#if ((DMA_IP_CFG_SW_MAJOR_VERSION != DMA_IP_PBCFG_SW_MAJOR_VERSION) || \
     (DMA_IP_CFG_SW_MINOR_VERSION != DMA_IP_PBCFG_SW_MINOR_VERSION) || \
     (DMA_IP_CFG_SW_PATCH_VERSION != DMA_IP_PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Dma_Ip_Cfg.h and Dma_Ip_PBcfg.h are different"
#endif
[!ENDIF!][!//

[!IF "node:value(./MclGeneral/MclDma/MclEnableDma)"!][!//

[!INCLUDE "Dma_Ip_RegOperations.m"!]
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Number Of Configured Logic Instances */
[!IF "node:value(./MclGeneral/MclEnableUserModeSupport)"!][!//
#define MCL_ENABLE_USER_MODE_SUPPORT    STD_ON
[!ELSE!][!//
#define MCL_ENABLE_USER_MODE_SUPPORT    STD_OFF
[!ENDIF!][!//

#if (STD_ON == MCL_ENABLE_USER_MODE_SUPPORT)
#define MCL_DMA_REG_PROT_AVAILABLE
#endif

#define DMA_IP_NOF_CFG_LOGIC_INSTANCES   ((uint32)[!CALL "Dma_Ip_GetNrOfLogicInstances"!]U)

[!LOOP "./MclConfig/dmaLogicInstance_ConfigType/*"!]
/* Logic Instance [!"@index"!] */
#define [!"node:value(dmaLogicInstance_IdName)"!]    ((uint32)[!"@index"!]U)
[!ENDLOOP!][!//

/* Number Of Configured Logic Channels */
#define DMA_IP_NOF_CFG_LOGIC_CHANNELS   ((uint32)[!CALL "Dma_Ip_GetNrOfLogicChannels"!]U)
[!LOOP "./MclConfig/dmaLogicChannel_Type/*"!][!//
[!VAR "LogicChannelName"="node:value(dmaLogicChannel_LogicName)"!]
/* Logic Channel [!"@index"!] */
#define [!"$LogicChannelName"!]      ((uint8)[!"@index"!]U)

[!IF "node:value(dmaLogicChannel_EnableScatterGather)"!][!//
/* Logic Channel [!"@index"!] Scatter/Gather Elements */
#define [!"$LogicChannelName"!]_NOF_CFG_SGA_ELEMENTS  ((uint32)[!CALL "Dma_Ip_GetNrOfSGElements_HardCodedPath"!]U)
[!LOOP "./dmaLogicChannel_ConfigType/dmaLogicChannel_ScatterGatherConfigType/dmaLogicChannelConfig_ScatterGatherArrayType/*"!][!//
[!VAR "ElementName"="node:value(dmaLogicChannelConfig_ScatterGatherElementConfigType/dmaLogicChannelConfig_ScatterGatherElementNameType)"!]
#define [!"$ElementName"!]       ((uint32)[!"@index"!]U)
[!ENDLOOP!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//


#define MCL_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "Mcl_MemMap.h"

/* DMA Initialization Structure */
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
extern const Dma_Ip_InitType Dma_Ip_xDmaInitPB_[!"."!];
[!ENDLOOP!][!//
[!ELSE!][!//
extern const Dma_Ip_InitType Dma_Ip_xDmaInitPB;
[!ENDIF!][!//

extern const Dma_Ip_LogicChannelConfigType Dma_Ip_xLogicChannelResetConfig;

extern const Dma_Ip_LogicInstanceConfigType Dma_Ip_xLogicInstanceResetConfig;

#define MCL_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "Mcl_MemMap.h"

#define MCL_START_SEC_CODE

#include "Mcl_MemMap.h"

[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!VAR "DerivativeName" = "text:toupper(substring-before(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative,'_'))"!]
[!ENDIF!][!//

 /* DMA IRQ Handlers */
ISR(Dma0_Ch0_IRQHandler);
ISR(Dma0_Ch1_IRQHandler);
ISR(Dma0_Ch2_IRQHandler);
ISR(Dma0_Ch3_IRQHandler);
ISR(Dma0_Ch4_IRQHandler);
ISR(Dma0_Ch5_IRQHandler);
ISR(Dma0_Ch6_IRQHandler);
ISR(Dma0_Ch7_IRQHandler);
ISR(Dma0_Ch8_IRQHandler);
ISR(Dma0_Ch9_IRQHandler);
ISR(Dma0_Ch10_IRQHandler);
ISR(Dma0_Ch11_IRQHandler);
ISR(Dma0_Ch12_IRQHandler);
ISR(Dma0_Ch13_IRQHandler);
ISR(Dma0_Ch14_IRQHandler);
ISR(Dma0_Ch15_IRQHandler);
ISR(Dma0_Ch16_IRQHandler);
ISR(Dma0_Ch17_IRQHandler);
ISR(Dma0_Ch18_IRQHandler);
ISR(Dma0_Ch19_IRQHandler);
ISR(Dma0_Ch20_IRQHandler);
ISR(Dma0_Ch21_IRQHandler);
ISR(Dma0_Ch22_IRQHandler);
ISR(Dma0_Ch23_IRQHandler);
ISR(Dma0_Ch24_IRQHandler);
ISR(Dma0_Ch25_IRQHandler);
ISR(Dma0_Ch26_IRQHandler);
ISR(Dma0_Ch27_IRQHandler);
ISR(Dma0_Ch28_IRQHandler);
ISR(Dma0_Ch29_IRQHandler);
ISR(Dma0_Ch30_IRQHandler);
ISR(Dma0_Ch31_IRQHandler);
 
#define MCL_STOP_SEC_CODE

#include "Mcl_MemMap.h"

[!ENDIF!][!//

#ifdef __cplusplus
}
#endif

#endif /* DMA_IP_CFG_H_ */

/*==================================================================================================
 *                                        END OF FILE
==================================================================================================*/
