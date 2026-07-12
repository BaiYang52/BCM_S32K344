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

#ifndef ADC_SAR_IP_CFGDEFINES_H
#define ADC_SAR_IP_CFGDEFINES_H

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
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

/* Important Note: The header warapper file depends on header platform and can not be used independently.
*  Do not change #include order in this file */
[!INDENT "0"!][!//
[!NOCODE!][!//
[!VAR "ShortDerivative" = "'X'"!][!//
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!VAR "DerivativeName" = "text:toupper(substring-before(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative,'_'))"!][!//
    [!IF "contains($DerivativeName, 'S32K3')"!][!//
        [!VAR "ShortDerivative" = "'S32K3'"!][!//
        [!IF "contains($DerivativeName, 'S32K341')"!][!//
            [!CODE!][!WS "0"!]#include "S32K342_ADC.h"[!CR!][!ENDCODE!][!//
            [!CODE!][!WS "0"!]#include "S32K342_TEMPSENSE.h"[!CR!][!ENDCODE!][!//
        [!ELSE!][!//
            [!CODE!][!WS "0"!]#include "[!"$DerivativeName"!]_ADC.h"[!CR!][!ENDCODE!][!//
            [!CODE!][!WS "0"!]#include "[!"$DerivativeName"!]_TEMPSENSE.h"[!CR!][!ENDCODE!][!//
        [!ENDIF!][!//
    [!ELSEIF "contains($DerivativeName, 'S32G')"!][!//
        [!VAR "ShortDerivative" = "'S32XX'"!][!//
        [!IF "contains($DerivativeName, 'S32G2')"!][!//
            [!CODE!][!WS "0"!]#include "S32G274A_ADC.h"[!CR!][!ENDCODE!][!//
        [!ELSEIF "contains($DerivativeName, 'S32G3')"!][!//
            [!CODE!][!WS "0"!]#include "S32G399A_ADC.h"[!CR!][!ENDCODE!][!//
        [!ENDIF!][!//
    [!ELSEIF "contains($DerivativeName, 'S32R45')"!][!//
        [!VAR "ShortDerivative" = "'S32XX'"!][!//
        [!CODE!][!WS "0"!]#include "S32R45_ADC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains($DerivativeName, 'S32R41')"!][!//
        [!VAR "ShortDerivative" = "'S32XX'"!][!//
        [!CODE!][!WS "0"!]#include "S32R41_ADC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains($DerivativeName, 'SAF85')"!][!//
        [!VAR "ShortDerivative" = "'SAF85XX'"!][!//
        [!CODE!][!WS "0"!]#include "SAF85XX_ADC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains($DerivativeName, 'S32E2')"!][!//
        [!VAR "ShortDerivative" = "'S32XX'"!][!//
        [!CODE!][!WS "0"!]#include "S32E27_ADC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains($DerivativeName, 'S32Z2')"!][!//
        [!VAR "ShortDerivative" = "'S32XX'"!][!//
        [!CODE!][!WS "0"!]#include "S32Z27_ADC.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!IF "$ShortDerivative != 'X'"!][!//
    [!CODE!][!WS "0"!]#include "Adc_Sar_Ip_HeaderWrapper_[!"$ShortDerivative"!].h"[!CR!][!ENDCODE!][!//
[!ELSE!][!//
    [!CODE!][!WS "0"!]#error "Unknown platform!"[!CR!][!ENDCODE!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!ENDINDENT!][!//
#include "OsIf.h"

[!NOCODE!][!//
[!INCLUDE "Adc_VersionCheck_Src.m"!][!//
[!INCLUDE "Adc_RegOperations.m"!][!//
[!ENDNOCODE!][!//
/*==================================================================================================
*                                SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ADC_SAR_IP_VENDOR_ID_CFGDEFINES                      43
#define ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_CFGDEFINES       4
#define ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_CFGDEFINES       4
#define ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_CFGDEFINES    0
#define ADC_SAR_IP_SW_MAJOR_VERSION_CFGDEFINES               2
#define ADC_SAR_IP_SW_MINOR_VERSION_CFGDEFINES               0
#define ADC_SAR_IP_SW_PATCH_VERSION_CFGDEFINES               1
/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/

/* Check if Adc_Sar_Ip_CfgDefines.h file and Adc_Sar_Ip_HeaderWrapper_[!"$ShortDerivative"!].h file are of the same vendor */
#if (ADC_SAR_IP_VENDOR_ID_CFGDEFINES != ADC_SAR_IP_VENDOR_ID_HEADERWRAPPER_[!"$ShortDerivative"!])
    #error "Adc_Sar_Ip_CfgDefines.h and Adc_Sar_Ip_HeaderWrapper_[!"$ShortDerivative"!].h have different vendor ids"
#endif

/* Check if Adc_Sar_Ip_CfgDefines.h file and Adc_Sar_Ip_HeaderWrapper_[!"$ShortDerivative"!].h file are of the same Autosar version */
#if ((ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_CFGDEFINES != ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_HEADERWRAPPER_[!"$ShortDerivative"!]) || \
     (ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_CFGDEFINES != ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_HEADERWRAPPER_[!"$ShortDerivative"!]) || \
     (ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_CFGDEFINES != ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_HEADERWRAPPER_[!"$ShortDerivative"!]) \
    )
    #error "AutoSar Version Numbers of Adc_Sar_Ip_CfgDefines.h and Adc_Sar_Ip_HeaderWrapper_[!"$ShortDerivative"!].h are different"
#endif

/* Check if Adc_Sar_Ip_CfgDefines.h file and Adc_Sar_Ip_HeaderWrapper_[!"$ShortDerivative"!].h file are of the same Software version */
#if ((ADC_SAR_IP_SW_MAJOR_VERSION_CFGDEFINES != ADC_SAR_IP_SW_MAJOR_VERSION_HEADERWRAPPER_[!"$ShortDerivative"!]) || \
     (ADC_SAR_IP_SW_MINOR_VERSION_CFGDEFINES != ADC_SAR_IP_SW_MINOR_VERSION_HEADERWRAPPER_[!"$ShortDerivative"!]) || \
     (ADC_SAR_IP_SW_PATCH_VERSION_CFGDEFINES != ADC_SAR_IP_SW_PATCH_VERSION_HEADERWRAPPER_[!"$ShortDerivative"!]) \
    )
  #error "Software Version Numbers of Adc_Sar_Ip_CfgDefines.h and Adc_Sar_Ip_HeaderWrapper_[!"$ShortDerivative"!].h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check AutoSar version */
#if ((ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_CFGDEFINES != OSIF_AR_RELEASE_MAJOR_VERSION) || \
     (ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_CFGDEFINES != OSIF_AR_RELEASE_MINOR_VERSION)    \
    )
    #error "AutoSar Version Numbers of Adc_Sar_Ip_CfgDefines.h and OsIf.h are different"
#endif
#endif /* DISABLE_MCAL_INTERMODULE_ASR_CHECK */

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                        LOCAL MACROS
==================================================================================================*/

[!CALL "AdcSarIpCfgDefinesMacro"!][!//

/*==================================================================================================
*                                       LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL VARIABLES
==================================================================================================*/

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

#endif /* ADC_SAR_IP_CFGDEFINES_H */
[!ENDCODE!]
