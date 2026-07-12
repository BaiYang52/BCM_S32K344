/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : I2S
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

[!CODE!][!//
#ifndef I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_H
#define I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_H
[!ENDCODE!][!//

/**
*   @file
*
*   @addtogroup i2s_ipw_config I2s IPW Configuration
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
#include "StandardTypes.h"
#include "I2s_Ipw_Types.h"
[!IF "GeneralConfiguration/I2sSaiSupport/I2sSaiEnable"!][!//
#include "Sai_Ip_Cfg.h"
[!ENDIF!][!//
[!IF "GeneralConfiguration/I2sFlexioSupport/I2sFlexioI2sEnable"!][!//
#include "Flexio_I2s_Ip_Cfg.h"
[!ENDIF!][!//

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define  I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_VENDOR_ID                    43
#define  I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION     4
#define  I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION     4
#define  I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION  0
#define  I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION             2
#define  I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION             0
#define  I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION             1

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if current file and StandardTypes header file are of the same Autosar version */
#if ((I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
     (I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of I2s_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and StandardTypes.h are different"
#endif
#endif /* DISABLE_MCAL_INTERMODULE_ASR_CHECK */

/* Check if current file and I2s_Ipw_Types header file are of the same vendor */
#if (I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_VENDOR_ID != I2S_IPW_TYPES_VENDOR_ID)
#error "I2s_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and I2s_Ipw_Types.h have different vendor ids"
#endif

/* Check if current file and I2s_Ipw_Types header file are of the same Autosar version */
#if ((I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION     != I2S_IPW_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION     != I2S_IPW_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION  != I2S_IPW_TYPES_AR_RELEASE_REVISION_VERSION))
#error "AutoSar Version Numbers of I2s_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and I2s_Ipw_Types.h are different"
#endif

/* Check if current file and I2s_Ipw_Types header file are of the same software version */
#if ((I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION != I2S_IPW_TYPES_SW_MAJOR_VERSION) || \
     (I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION != I2S_IPW_TYPES_SW_MINOR_VERSION) || \
     (I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION != I2S_IPW_TYPES_SW_PATCH_VERSION))
#error "Software Version Numbers of I2s_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and I2s_Ipw_Types.h are different"
#endif

[!IF "GeneralConfiguration/I2sSaiSupport/I2sSaiEnable"!][!//
/* Check if current file and Sai_Ip_Cfg header file are of the same vendor */
#if (I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_VENDOR_ID != SAI_IP_CFG_VENDOR_ID)
#error "I2s_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Sai_Ip_Cfg.h have different vendor ids"
#endif

/* Check if current file and Sai_Ip_Cfg header file are of the same Autosar version */
#if ((I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION     != SAI_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION     != SAI_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION  != SAI_IP_CFG_AR_RELEASE_REVISION_VERSION))
#error "AutoSar Version Numbers of I2s_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Sai_Ip_Cfg.h are different"
#endif

/* Check if current file and Sai_Ip_Cfg header file are of the same software version */
#if ((I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION != SAI_IP_CFG_SW_MAJOR_VERSION) || \
     (I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION != SAI_IP_CFG_SW_MINOR_VERSION) || \
     (I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION != SAI_IP_CFG_SW_PATCH_VERSION))
#error "Software Version Numbers of I2s_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Sai_Ip_Cfg.h are different"
#endif

[!ENDIF!][!//
[!IF "GeneralConfiguration/I2sFlexioSupport/I2sFlexioI2sEnable"!][!//
/* Check if current file and Flexio_I2s_Ip_Cfg header file are of the same vendor */
#if (I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_VENDOR_ID != FLEXIO_I2S_IP_CFG_VENDOR_ID)
#error "I2s_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Flexio_I2s_Ip_Cfg.h have different vendor ids"
#endif

/* Check if current file and Flexio_I2s_Ip_Cfg header file are of the same Autosar version */
#if ((I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION     != FLEXIO_I2S_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION     != FLEXIO_I2S_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION  != FLEXIO_I2S_IP_CFG_AR_RELEASE_REVISION_VERSION))
#error "AutoSar Version Numbers of I2s_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Flexio_I2s_Ip_Cfg.h are different"
#endif

/* Check if current file and Flexio_I2s_Ip_Cfg header file are of the same software version */
#if ((I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION != FLEXIO_I2S_IP_CFG_SW_MAJOR_VERSION) || \
     (I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION != FLEXIO_I2S_IP_CFG_SW_MINOR_VERSION) || \
     (I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION != FLEXIO_I2S_IP_CFG_SW_PATCH_VERSION))
#error "Software Version Numbers of I2s_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Flexio_I2s_Ip_Cfg.h are different"
#endif

[!ENDIF!][!//
[!NOCODE!][!//
[!//INCLUDE "I2s_VersionCheck_Src.m"!][!// TODO: add version checking
[!INCLUDE "I2s_RegOperations.m"!][!//
[!ENDNOCODE!][!//

/*==================================================================================================
*                         LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

#define I2S_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "I2s_MemMap.h"

[!CALL "I2s_Ipw_Config" , "MacroGenerationType" = "'declaration'"!][!//

[!CALL "I2s_Ipw_Cfg" , "MacroGenerationType" = "'declaration'"!][!//

#define I2S_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "I2s_MemMap.h"

/*==================================================================================================
*                                  LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL FUNCTIONS
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

[!CODE!][!//
#endif /* I2S_IPW_[!IF "var:defined('postBuildVariant')"!][!"text:toupper($postBuildVariant)"!]_[!ENDIF!]PBCFG_H */
[!ENDCODE!][!//
