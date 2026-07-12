[!AUTOSPACING!][!//
[!CODE!][!//
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

#ifndef FLEXIO_MCL_IP_CFG_H_
#define FLEXIO_MCL_IP_CFG_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
[!NOCODE!][!//

[!CODE!][!WS "0"!]/* Include all variants header files. */[!CR!][!ENDCODE!][!//
[!IF "var:defined('postBuildVariant')"!][!//
    [!LOOP "variant:all()"!][!//
        [!CODE!][!WS "0"!]#include "Flexio_Mcl_Ip_[!"."!]_PBcfg.h"[!CR!][!ENDCODE!][!//
    [!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS "0"!]#include "Flexio_Mcl_Ip_PBcfg.h"[!ENDCODE!][!//
[!ENDIF!][!//

[!ENDNOCODE!][!//

#include "Flexio_Mcl_Ip_Cfg_Defines.h"

[!INCLUDE "Flexio_Ip_RegOperations.m"!]
/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLEXIO_MCL_IP_CFG_VENDOR_ID                    43
#define FLEXIO_MCL_IP_CFG_MODULE_ID                    255
#define FLEXIO_MCL_IP_CFG_AR_RELEASE_MAJOR_VERSION     4
#define FLEXIO_MCL_IP_CFG_AR_RELEASE_MINOR_VERSION     4
#define FLEXIO_MCL_IP_CFG_AR_RELEASE_REVISION_VERSION  0
#define FLEXIO_MCL_IP_CFG_SW_MAJOR_VERSION             2
#define FLEXIO_MCL_IP_CFG_SW_MINOR_VERSION             0
#define FLEXIO_MCL_IP_CFG_SW_PATCH_VERSION             1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if header file and Flexio_Mcl_Ip_Cfg_Defines.h file are of the same vendor */
#if (FLEXIO_MCL_IP_CFG_VENDOR_ID != FLEXIO_MCL_IP_CFG_DEFINES_VENDOR_ID)
    #error "Flexio_Mcl_Ip_Cfg.h and Flexio_Mcl_Ip_Cfg_Defines.h have different vendor ids"
#endif

/* Check if header file and Flexio_Mcl_Ip_Cfg_Defines.h file are of the same Autosar version */
#if ((FLEXIO_MCL_IP_CFG_AR_RELEASE_MAJOR_VERSION != FLEXIO_MCL_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION) || \
     (FLEXIO_MCL_IP_CFG_AR_RELEASE_MINOR_VERSION != FLEXIO_MCL_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION) || \
     (FLEXIO_MCL_IP_CFG_AR_RELEASE_REVISION_VERSION != FLEXIO_MCL_IP_CFG_DEFINES_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Flexio_Mcl_Ip_Cfg.h and Flexio_Mcl_Ip_Cfg_Defines.h are different"
#endif

/* Check if header file and Flexio_Mcl_Ip_Cfg_Defines.h file are of the same Software version */
#if ((FLEXIO_MCL_IP_CFG_SW_MAJOR_VERSION != FLEXIO_MCL_IP_CFG_DEFINES_SW_MAJOR_VERSION) || \
     (FLEXIO_MCL_IP_CFG_SW_MINOR_VERSION != FLEXIO_MCL_IP_CFG_DEFINES_SW_MINOR_VERSION) || \
     (FLEXIO_MCL_IP_CFG_SW_PATCH_VERSION != FLEXIO_MCL_IP_CFG_DEFINES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Flexio_Mcl_Ip_Cfg.h and Flexio_Mcl_Ip_Cfg_Defines.h are different"
#endif

[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
/* Check if header file and Flexio_Mcl_Ip_[!"."!]_PBcfg.h file are of the same vendor */
#if (FLEXIO_MCL_IP_CFG_VENDOR_ID != FLEXIO_MCL_IP_[!"."!]_PBCFG_VENDOR_ID)
    #error "Flexio_Mcl_Ip_Cfg.h and Flexio_Mcl_Ip_[!"."!]_PBcfg.h have different vendor ids"
#endif

/* Check if header file and Flexio_Mcl_Ip_[!"."!]_PBcfg.h file are of the same Autosar version */
#if ((FLEXIO_MCL_IP_CFG_AR_RELEASE_MAJOR_VERSION != FLEXIO_MCL_IP_[!"."!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (FLEXIO_MCL_IP_CFG_AR_RELEASE_MINOR_VERSION != FLEXIO_MCL_IP_[!"."!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (FLEXIO_MCL_IP_CFG_AR_RELEASE_REVISION_VERSION != FLEXIO_MCL_IP_[!"."!]_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Flexio_Mcl_Ip_Cfg.h and Flexio_Mcl_Ip_[!"."!]_PBcfg.h are different"
#endif

/* Check if header file and Flexio_Mcl_Ip_[!"."!]_PBcfg.h file are of the same Software version */
#if ((FLEXIO_MCL_IP_CFG_SW_MAJOR_VERSION != FLEXIO_MCL_IP_[!"."!]_PBCFG_SW_MAJOR_VERSION) || \
     (FLEXIO_MCL_IP_CFG_SW_MINOR_VERSION != FLEXIO_MCL_IP_[!"."!]_PBCFG_SW_MINOR_VERSION) || \
     (FLEXIO_MCL_IP_CFG_SW_PATCH_VERSION != FLEXIO_MCL_IP_[!"."!]_PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Flexio_Mcl_Ip_Cfg.h and Flexio_Mcl_Ip_[!"."!]_PBcfg.h are different"
#endif
[!ENDLOOP!][!//
[!ELSE!][!//
/* Check if header file and Flexio_Mcl_Ip_PBcfg.h file are of the same vendor */
#if (FLEXIO_MCL_IP_CFG_VENDOR_ID != FLEXIO_MCL_IP_PBCFG_VENDOR_ID)
    #error "Flexio_Mcl_Ip_Cfg.h and Flexio_Mcl_Ip_PBcfg.h have different vendor ids"
#endif

/* Check if header file and Flexio_Mcl_Ip_PBcfg.h file are of the same Autosar version */
#if ((FLEXIO_MCL_IP_CFG_AR_RELEASE_MAJOR_VERSION != FLEXIO_MCL_IP_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (FLEXIO_MCL_IP_CFG_AR_RELEASE_MINOR_VERSION != FLEXIO_MCL_IP_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (FLEXIO_MCL_IP_CFG_AR_RELEASE_REVISION_VERSION != FLEXIO_MCL_IP_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Flexio_Mcl_Ip_Cfg.h and Flexio_Mcl_Ip_PBcfg.h are different"
#endif

/* Check if header file and Flexio_Mcl_Ip_PBcfg.h file are of the same Software version */
#if ((FLEXIO_MCL_IP_CFG_SW_MAJOR_VERSION != FLEXIO_MCL_IP_PBCFG_SW_MAJOR_VERSION) || \
     (FLEXIO_MCL_IP_CFG_SW_MINOR_VERSION != FLEXIO_MCL_IP_PBCFG_SW_MINOR_VERSION) || \
     (FLEXIO_MCL_IP_CFG_SW_PATCH_VERSION != FLEXIO_MCL_IP_PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Flexio_Mcl_Ip_Cfg.h and Flexio_Mcl_Ip_PBcfg.h are different"
#endif
[!ENDIF!][!//

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
[!LOOP "./MclConfig/FlexioCommon/*"!][!//
    [!LOOP "FlexioMclLogicChannels/*"!][!//
        [!VAR "LogicChannelName"="node:name(.)"!]
/* Logic Channel [!"@index"!] */
#define MclConf_FlexioMclLogicChannels_[!"$LogicChannelName"!]      ((uint32)[!"FlexioMclChannelId"!])
    [!ENDLOOP!][!//
[!ENDLOOP!][!//

#define MCL_START_SEC_CONFIG_DATA_UNSPECIFIED
/* @violates @ref Mcl_Dma_h_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"

[!// ================================== FLEXIO COMMON =============================================*/
[!IF "MclGeneral/MclFlexioCommon/MclEnableFlexioCommon = 'true'"!][!//
    [!INCLUDE "Flexio_Ip_RegOperations.m"!]

[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
extern const Flexio_Ip_InstanceConfigType Flexio_Ip_xFlexioInit_[!"."!];
[!ENDLOOP!][!//
[!ELSE!][!//
extern const Flexio_Ip_InstanceConfigType Flexio_Ip_xFlexioInit;
[!ENDIF!][!//

[!ENDIF!]
[!// ================================== FLEXIO COMMON =============================================*/

#define MCL_STOP_SEC_CONFIG_DATA_UNSPECIFIED
/* @violates @ref Mcl_Dma_h_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* FLEXIO_MCL_IP_CFG_H_ */

[!ENDCODE!]
