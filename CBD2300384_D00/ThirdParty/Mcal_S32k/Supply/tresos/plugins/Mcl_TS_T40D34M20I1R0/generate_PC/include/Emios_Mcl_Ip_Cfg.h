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

#ifndef EMIOS_MCL_IP_CFG_H_
#define EMIOS_MCL_IP_CFG_H_
/**
*   @file Emios_Mcl_Ip_Cfg.h
*
*   @version 2.0.1
*
*   @brief   AUTOSAR Mcl - MCL driver header file.
*   @details 
*
*   @addtogroup MCL_DRIVER MCL Driver
*   @{
*/
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

[!INCLUDE "Emios_Ip_RegOperations.m"!][!//
[!CODE!][!WS "0"!]/* Include all variants header files. */[!CR!][!ENDCODE!][!//
[!IF "var:defined('postBuildVariant')"!][!//
    [!LOOP "variant:all()"!][!//
        [!CODE!][!WS "0"!]#include "Emios_Mcl_Ip_[!"."!]_PBcfg.h"[!CR!][!ENDCODE!][!//
    [!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS "0"!]#include "Emios_Mcl_Ip_PBcfg.h"[!ENDCODE!][!//
[!ENDIF!][!//

[!ENDNOCODE!][!//

/*==================================================================================================
*                               HEADER FILE VERSION INFORMATION
==================================================================================================*/
#define EMIOS_MCL_IP_CFG_VENDOR_ID                       43
#define EMIOS_MCL_IP_CFG_AR_RELEASE_MAJOR_VERSION        4
#define EMIOS_MCL_IP_CFG_AR_RELEASE_MINOR_VERSION        4
#define EMIOS_MCL_IP_CFG_AR_RELEASE_REVISION_VERSION     0
#define EMIOS_MCL_IP_CFG_SW_MAJOR_VERSION                2
#define EMIOS_MCL_IP_CFG_SW_MINOR_VERSION                0
#define EMIOS_MCL_IP_CFG_SW_PATCH_VERSION                1
/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
#if (EMIOS_MCL_IP_CFG_VENDOR_ID != EMIOS_MCL_IP_[!"."!]_PBCFG_VENDOR_ID)
    #error "Emios_Mcl_Ip_Cfg.h and Emios_Mcl_Ip_[!"."!]_PBcfg.h have different vendor ids"
#endif

/* Check if  header file and Emios_Mcl_Ip_[!"."!]_PBcfg.h file are of the same Autosar version */
#if ((EMIOS_MCL_IP_CFG_AR_RELEASE_MAJOR_VERSION != EMIOS_MCL_IP_[!"."!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_MCL_IP_CFG_AR_RELEASE_MINOR_VERSION != EMIOS_MCL_IP_[!"."!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_MCL_IP_CFG_AR_RELEASE_REVISION_VERSION != EMIOS_MCL_IP_[!"."!]_PBCFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Emios_Mcl_Ip_Cfg.h and Emios_Mcl_Ip_[!"."!]_PBcfg.h are different"
#endif

/* Check if header file and Emios_Mcl_Ip_[!"."!]_PBcfg.h file are of the same Software version */
#if ((EMIOS_MCL_IP_CFG_SW_MAJOR_VERSION != EMIOS_MCL_IP_[!"."!]_PBCFG_SW_MAJOR_VERSION) || \
     (EMIOS_MCL_IP_CFG_SW_MINOR_VERSION != EMIOS_MCL_IP_[!"."!]_PBCFG_SW_MINOR_VERSION) || \
     (EMIOS_MCL_IP_CFG_SW_PATCH_VERSION != EMIOS_MCL_IP_[!"."!]_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Emios_Mcl_Ip_Cfg.h and Emios_Mcl_Ip_[!"."!]_PBcfg.h are different"
#endif
[!ENDLOOP!][!//
[!ELSE!][!//
#if (EMIOS_MCL_IP_CFG_VENDOR_ID != EMIOS_MCL_IP_PBCFG_VENDOR_ID)
    #error "Emios_Mcl_Ip_Cfg.h and Emios_Mcl_Ip_PBcfg.h have different vendor ids"
#endif

/* Check if  header file and Emios_Mcl_Ip_PBcfg.h file are of the same Autosar version */
#if ((EMIOS_MCL_IP_CFG_AR_RELEASE_MAJOR_VERSION != EMIOS_MCL_IP_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_MCL_IP_CFG_AR_RELEASE_MINOR_VERSION != EMIOS_MCL_IP_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_MCL_IP_CFG_AR_RELEASE_REVISION_VERSION != EMIOS_MCL_IP_PBCFG_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Emios_Mcl_Ip_Cfg.h and Emios_Mcl_Ip_PBcfg.h are different"
#endif

/* Check if header file and Emios_Mcl_Ip_PBcfg.h file are of the same Software version */
#if ((EMIOS_MCL_IP_CFG_SW_MAJOR_VERSION != EMIOS_MCL_IP_PBCFG_SW_MAJOR_VERSION) || \
     (EMIOS_MCL_IP_CFG_SW_MINOR_VERSION != EMIOS_MCL_IP_PBCFG_SW_MINOR_VERSION) || \
     (EMIOS_MCL_IP_CFG_SW_PATCH_VERSION != EMIOS_MCL_IP_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Emios_Mcl_Ip_Cfg.h and Emios_Mcl_Ip_PBcfg.h are different"
#endif
[!ENDIF!][!//

/* Check if header file and Emios_Mcl_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h file are of the same vendor */
#if (EMIOS_MCL_IP_CFG_VENDOR_ID != EMIOS_MCL_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_VENDOR_ID)
    #error "Emios_Mcl_Ip_Cfg.h  and Emios_Mcl_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h have different vendor ids"
#endif

/* Check if header file and Emios_Mcl_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h file are of the same Autosar version */
#if ((EMIOS_MCL_IP_CFG_AR_RELEASE_MAJOR_VERSION != EMIOS_MCL_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_MCL_IP_CFG_AR_RELEASE_MINOR_VERSION != EMIOS_MCL_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_MCL_IP_CFG_AR_RELEASE_REVISION_VERSION != EMIOS_MCL_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Emios_Mcl_Ip_Cfg.h and Emios_Mcl_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h are different"
#endif

/* Check if header file and Emios_Mcl_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h file are of the same Software version */
#if ((EMIOS_MCL_IP_CFG_SW_MAJOR_VERSION != EMIOS_MCL_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MAJOR_VERSION) || \
     (EMIOS_MCL_IP_CFG_SW_MINOR_VERSION != EMIOS_MCL_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MINOR_VERSION) || \
     (EMIOS_MCL_IP_CFG_SW_PATCH_VERSION != EMIOS_MCL_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Emios_Mcl_Ip_Cfg.h and Emios_Mcl_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h are different"
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/* Enable/Disable EMIOS support from MCL. */
[!IF "node:value(./MclGeneral/MclEmiosCommon/MclEnableEmiosCommon)"!][!//
#define EMIOS_IP_IS_AVAILABLE                      STD_ON
[!ELSE!][!//
#define EMIOS_IP_IS_AVAILABLE                      STD_OFF
[!ENDIF!][!//

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

#endif /* EMIOS_MCL_IP_CFG_H_ */

[!ENDCODE!]
