/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : FLEXIO_SENT_IP
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
*   @file    Sent_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c
*
*   @version 2.0.1
*
*   @brief   AUTOSAR Sent - module interface
*   @details Configuration Structures for PostBuild.
*
*   @addtogroup SENT_DRIVER
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

#include "Flexio_Sent_Ip_Cfg.h"
#include "Sent_Ipw_Cfg.h"
#include "Sent_Ipw_Types.h"

[!AUTOSPACING!][!//
[!NOCODE!][!//
[!INCLUDE "Sent_VersionCheck_Src_PB.m"!][!//
[!ENDNOCODE!][!//
[!//
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/*
* @file           Sent_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c
*/
#define SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C                       43
#define SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C        4
#define SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C        4
#define SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C     0
#define SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C                2
#define SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C                0
#define SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C                1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and SENT configuration header file are of the same vendor */
#if (SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != SENT_IPW_CFG_VENDOR_ID)
    #error "Sent_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Sent_Ipw_Cfg.h have different vendor ids" 
#endif
/* Check if current file and SENT header file are of the same Autosar version */
#if ((SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C     != SENT_IPW_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C     != SENT_IPW_CFG_AR_RELEASE_MINOR_VERSION) || \
     (SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C  != SENT_IPW_CFG_AR_RELEASE_REVISION_VERSION)\
    )
    #error "AutoSar Version Numbers of Sent_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Sent_Ipw_Cfg.h are different" 
#endif
/* Check if current file and SENT header file are of the same Software version */
#if ((SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != SENT_IPW_CFG_SW_MAJOR_VERSION) || \
     (SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != SENT_IPW_CFG_SW_MINOR_VERSION) || \
     (SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != SENT_IPW_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Sent_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Sent_Ipw_Cfg.h are different" 
#endif

/* Check if current file and SENT configuration header file are of the same vendor */
#if (SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != FLEXIO_SENT_IP_CFG_VENDOR_ID)
    #error "Sent_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_Sent_Ip_Cfg.h have different vendor ids" 
#endif
/* Check if current file and SENT header file are of the same Autosar version */
#if ((SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C     != FLEXIO_SENT_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C     != FLEXIO_SENT_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C  != FLEXIO_SENT_IP_CFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Sent_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_Sent_Ip_Cfg.h are different" 
#endif
/* Check if current file and SENT header file are of the same Software version */
#if ((SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != FLEXIO_SENT_IP_CFG_SW_MAJOR_VERSION) || \
     (SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != FLEXIO_SENT_IP_CFG_SW_MINOR_VERSION) || \
     (SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != FLEXIO_SENT_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Sent_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_Sent_Ip_Cfg.h are different" 
#endif

/* Check if current file and SENT configuration header file are of the same vendor */
#if (SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != SENT_IPW_TYPES_VENDOR_ID)
    #error "Sent_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Sent_Ipw_Types.h have different vendor ids" 
#endif
/* Check if current file and SENT header file are of the same Autosar version */
#if ((SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C     != SENT_IPW_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C     != SENT_IPW_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C  != SENT_IPW_TYPES_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Sent_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Sent_Ipw_Types.h are different" 
#endif
/* Check if current file and SENT header file are of the same Software version */
#if ((SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != SENT_IPW_TYPES_SW_MAJOR_VERSION) || \
     (SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != SENT_IPW_TYPES_SW_MINOR_VERSION) || \
     (SENT_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != SENT_IPW_TYPES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Sent_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Sent_Ipw_Types.h are different" 
#endif


/*==================================================================================================
*                                   GLOBAL FUNCTION PROTOTYPES
==================================================================================================*/
/**
* @brief   Export FLEXIO configurations.
*/
FLEXIO_SENT_IP_CONFIG_EXT
/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
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

[!INDENT "0"!]
#define SENT_START_SEC_CONFIG_DATA_UNSPECIFIED
/** @violates @ref Sent_Ipw_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_1 MISRA 2012 Required Directive 4.10*/
/** @violates @ref Sent_Ipw_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_2 MISRA 2012 Advisory Rule 20.1.*/
#include "Sent_MemMap.h"

[!SELECT "SentConfigSet"!]
    [!NOCODE!]
        [!VAR "SENT_CONTROLLER_CONFIG_CNT" = "num:i(0)"!]
        [!VAR "SENT_CHANNEL_CONFIG_CNT" = "num:i(0)"!]
        [!VAR "SENT_CONTROLLER_CONFIGURED" = "num:i(count(SentControllerConfig/*))"!]
        [!VAR "SENT_CHANNEL_CONFIG_CONFIGURED" = "num:i(count(SentControllerConfig/*/SentChannelConfig/*))"!]
    [!ENDNOCODE!]
/**
*   @brief  Sent Controller Object's pointer structure
*/

const Sent_Ipw_CtrlConfigType Sent_Ipw_aCtrlConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"$SENT_CONTROLLER_CONFIGURED"!]U]=
{
    [!INDENT "4"!]
        [!VAR "CtrIdx" = "0"!]
        [!LOOP "node:order(SentControllerConfig/*, './SentControllerId')"!]
            [!VAR "CtrIdx" = "num:i($CtrIdx + 1)"!]
            {
            [!INDENT "8"!]
                &Flexio_aCtrlConfigPB[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i(./SentControllerId)"!]U]
            [!ENDINDENT!]
            }[!IF "$CtrIdx < $SENT_CONTROLLER_CONFIGURED"!],[!ENDIF!][!CR!]
        [!ENDLOOP!]
    [!ENDINDENT!]
};

[!ENDSELECT!]
[!ENDINDENT!]
/*==================================================================================================
*                                      GLOBAL VARIABLES
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

#define SENT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
/* @violates @ref Sent_Ipw_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_1 MISRA 2012 Required Directive 4.10*/
/* @violates @ref Sent_Ipw_PBcfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_c_REF_2 MISRA 2012 Advisory Rule 20.1.*/
#include "Sent_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
