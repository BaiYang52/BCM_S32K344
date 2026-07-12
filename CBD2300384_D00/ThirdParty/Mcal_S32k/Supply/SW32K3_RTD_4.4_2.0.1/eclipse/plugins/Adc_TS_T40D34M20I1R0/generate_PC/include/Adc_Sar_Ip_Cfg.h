[!CODE!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : ADC_SAR
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

#ifndef ADC_SAR_IP_CFG_H
#define ADC_SAR_IP_CFG_H

/**
*   @file
*
*   @addtogroup adc_sar_ip_config Adc Sar IPL Configuration
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
#include "Adc_Sar_Ip_Types.h"
[!NOCODE!][!//
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!]#include "Adc_Sar_Ip_[!"."!]_PBcfg.h"[!CR!][!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!]#include "Adc_Sar_Ip_PBcfg.h"[!CR!][!ENDCODE!]
[!ENDIF!]

[!INCLUDE "Adc_VersionCheck_Inc.m"!][!//
[!INCLUDE "Adc_RegOperations.m"!][!//
[!//[!INCLUDE "Adc_Bctu_Cfg.m"!][!//
[!ENDNOCODE!][!//

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define ADC_SAR_IP_VENDOR_ID_CFG                      43
#define ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_CFG       4
#define ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_CFG       4
#define ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_CFG    0
#define ADC_SAR_IP_SW_MAJOR_VERSION_CFG               2
#define ADC_SAR_IP_SW_MINOR_VERSION_CFG               0
#define ADC_SAR_IP_SW_PATCH_VERSION_CFG               1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if Adc_Sar_Ip_Cfg.h file and Adc_Sar_Ip_Types.h file are of the same vendor */
#if (ADC_SAR_IP_VENDOR_ID_CFG != ADC_SAR_IP_VENDOR_ID_TYPES)
    #error "Adc_Sar_Ip_Cfg.h and Adc_Sar_Ip_Types.h have different vendor ids"
#endif

/* Check if Adc_Sar_Ip_Cfg.h file and Adc_Sar_Ip_Types.h file are of the same Autosar version */
#if ((ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_CFG != ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_TYPES) || \
     (ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_CFG != ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_TYPES) || \
     (ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_CFG != ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_TYPES) \
    )
    #error "AutoSar Version Numbers of Adc_Sar_Ip_Cfg.h and Adc_Sar_Ip_Types.h are different"
#endif

/* Check if Adc_Sar_Ip_Cfg.h file and Adc_Sar_Ip_Types.h file are of the same Software version */
#if ((ADC_SAR_IP_SW_MAJOR_VERSION_CFG != ADC_SAR_IP_SW_MAJOR_VERSION_TYPES) || \
     (ADC_SAR_IP_SW_MINOR_VERSION_CFG != ADC_SAR_IP_SW_MINOR_VERSION_TYPES) || \
     (ADC_SAR_IP_SW_PATCH_VERSION_CFG != ADC_SAR_IP_SW_PATCH_VERSION_TYPES) \
    )
  #error "Software Version Numbers of Adc_Sar_Ip_Cfg.h and Adc_Sar_Ip_Types.h are different"
#endif
[!NOCODE!][!//
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!]
/* Check if Adc_Sar_Ip_Cfg.h file and Adc_Sar_Ip_[!"text:toupper(.)"!]_PBcfg.h file are of the same vendor */
#if (ADC_SAR_IP_VENDOR_ID_CFG != ADC_SAR_IP_VENDOR_ID_[!"text:toupper(.)"!]_PBCFG)
    #error "Adc_Sar_Ip_Cfg.h and Adc_Sar_Ip_[!"text:toupper(.)"!]_PBcfg.h have different vendor ids"
#endif

/* Check if Adc_Sar_Ip_Cfg.h file and Adc_Sar_Ip_[!"text:toupper(.)"!]_PBcfg.h file are of the same Autosar version */
#if ((ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_CFG != ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_[!"text:toupper(.)"!]_PBCFG) || \
     (ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_CFG != ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_[!"text:toupper(.)"!]_PBCFG) || \
     (ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_CFG != ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_[!"text:toupper(.)"!]_PBCFG) \
    )
    #error "AutoSar Version Numbers of Adc_Sar_Ip_Cfg.h and Adc_Sar_Ip_[!"text:toupper(.)"!]_PBcfg.h are different"
#endif

/* Check if Adc_Sar_Ip_Cfg.h file and Adc_Sar_Ip_[!"text:toupper(.)"!]_PBcfg.h file are of the same Software version */
#if ((ADC_SAR_IP_SW_MAJOR_VERSION_CFG != ADC_SAR_IP_SW_MAJOR_VERSION_[!"text:toupper(.)"!]_PBCFG) || \
     (ADC_SAR_IP_SW_MINOR_VERSION_CFG != ADC_SAR_IP_SW_MINOR_VERSION_[!"text:toupper(.)"!]_PBCFG) || \
     (ADC_SAR_IP_SW_PATCH_VERSION_CFG != ADC_SAR_IP_SW_PATCH_VERSION_[!"text:toupper(.)"!]_PBCFG) \
    )
  #error "Software Version Numbers of Adc_Sar_Ip_Cfg.h and Adc_Sar_Ip_[!"text:toupper(.)"!]_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!]
/* Check if Adc_Sar_Ip_Cfg.h file and Adc_Sar_Ip_PBcfg.h file are of the same vendor */
#if (ADC_SAR_IP_VENDOR_ID_CFG != ADC_SAR_IP_VENDOR_ID_PBCFG)
    #error "Adc_Sar_Ip_Cfg.h and Adc_Sar_Ip_PBcfg.h have different vendor ids"
#endif

/* Check if Adc_Sar_Ip_Cfg.h file and Adc_Sar_Ip_PBcfg.h file are of the same Autosar version */
#if ((ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_CFG != ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_PBCFG) || \
     (ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_CFG != ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_PBCFG) || \
     (ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_CFG != ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_PBCFG) \
    )
    #error "AutoSar Version Numbers of Adc_Sar_Ip_Cfg.h and Adc_Sar_Ip_PBcfg.h are different"
#endif

/* Check if Adc_Sar_Ip_Cfg.h file and Adc_Sar_Ip_PBcfg.h file are of the same Software version */
#if ((ADC_SAR_IP_SW_MAJOR_VERSION_CFG != ADC_SAR_IP_SW_MAJOR_VERSION_PBCFG) || \
     (ADC_SAR_IP_SW_MINOR_VERSION_CFG != ADC_SAR_IP_SW_MINOR_VERSION_PBCFG) || \
     (ADC_SAR_IP_SW_PATCH_VERSION_CFG != ADC_SAR_IP_SW_PATCH_VERSION_PBCFG) \
    )
  #error "Software Version Numbers of Adc_Sar_Ip_Cfg.h and Adc_Sar_Ip_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!][!//

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* ADC_SAR_IP_CFG_H */
[!ENDCODE!]