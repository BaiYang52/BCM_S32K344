/*==================================================================================================
* Project : RTD AUTOSAR 4.4
* Platform : CORTEXM
* Peripheral : S32K3XX
* Dependencies : none
*
* Autosar Version : 4.4.0
* Autosar Revision : ASR_REL_4_4_REV_0000
* Autosar Conf.Variant :
* SW Version : 2.0.1
* Build Version : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
* (c) Copyright 2020 - 2022 NXP Semiconductors
* All Rights Reserved.
*
* NXP Confidential. This software is owned or controlled by NXP and may only be
* used strictly in accordance with the applicable license terms. By expressly
* accepting such terms or by downloading, installing, activating and/or otherwise
* using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms. If you do not agree to be
* bound by the applicable license terms, then you may not retain, install,
* activate or otherwise use the software.
==================================================================================================*/
#ifndef OSIF_CFG_H
#define OSIF_CFG_H

/**
*   @file
*
*   @addtogroup osif_drv
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "OsIf_ArchCfg.h"
#include "StandardTypes.h"

[!NOCODE!][!// Include specific header file 
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!// 
    [!IF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k312')"!][!// 
[!CODE!][!WS "0"!]#include "S32K312_SYSTICK.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "text:match(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k314')"!][!// 
[!CODE!][!WS "0"!]#include "S32K314_SYSTICK.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "text:match(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k324')"!][!// 
[!CODE!][!WS "0"!]#include "S32K324_SYSTICK.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "text:match(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k342')"!][!// 
[!CODE!][!WS "0"!]#include "S32K342_SYSTICK.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "text:match(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k344')"!][!// 
[!CODE!][!WS "0"!]#include "S32K344_SYSTICK.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "text:match(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k341')"!][!// 
[!CODE!][!WS "0"!]#include "S32K342_SYSTICK.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "text:match(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k322')"!][!// 
[!CODE!][!WS "0"!]#include "S32K322_SYSTICK.h"[!CR!][!ENDCODE!][!//
     [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32g2')"!][!// 
[!CODE!][!WS "0"!]#include "S32G274A_SYSTICK.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32g3')"!][!// 
[!CODE!][!WS "0"!]#include "S32G399A_SYSTICK.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r45')"!][!// 
[!CODE!][!WS "0"!]#include "S32R45_SYSTICK.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r41')"!][!// 
[!CODE!][!WS "0"!]#include "S32R41_SYSTICK.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 'saf8544')"!][!// 
[!CODE!][!WS "0"!]#include "SAF85XX_SYSTICK.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k116')"!][!// 
[!CODE!][!WS "0"!]#include "S32K116_SYSTICK.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k118')"!][!// 
[!CODE!][!WS "0"!]#include "S32K118_SYSTICK.h"[!CR!][!ENDCODE!][!// 
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k142w')"!][!// 
[!CODE!][!WS "0"!]#include "S32K142W_SYSTICK.h"[!CR!][!ENDCODE!][!// 
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k142')"!][!// 
[!CODE!][!WS "0"!]#include "S32K142_SYSTICK.h"[!CR!][!ENDCODE!][!// 
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k144w')"!][!// 
[!CODE!][!WS "0"!]#include "S32K144W_SYSTICK.h"[!CR!][!ENDCODE!][!// 
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k144')"!][!// 
[!CODE!][!WS "0"!]#include "S32K144_SYSTICK.h"[!CR!][!ENDCODE!][!// 
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k146')"!][!// 
[!CODE!][!WS "0"!]#include "S32K146_SYSTICK.h"[!CR!][!ENDCODE!][!// 
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k148')"!][!// 
[!CODE!][!WS "0"!]#include "S32K148_SYSTICK.h"[!CR!][!ENDCODE!][!// 
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32e')"!][!// 
[!CODE!][!WS "0"!]#include "S32E27_SYSTICK.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32z')"!][!// 
[!CODE!][!WS "0"!]#include "S32Z27_SYSTICK.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 'sja')"!][!// 
[!CODE!][!WS "0"!]#include "SJA1110.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ELSE!][!//
    [!ERROR "The Resource plugin or ResourceSubderivative field is not available for header file inclusion."!][!//
[!ENDIF!][!// 
[!ENDNOCODE!][!//
/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define OSIF_CFG_VENDOR_ID                    43
#define OSIF_CFG_AR_RELEASE_MAJOR_VERSION     4
#define OSIF_CFG_AR_RELEASE_MINOR_VERSION     4
#define OSIF_CFG_AR_RELEASE_REVISION_VERSION  0
#define OSIF_CFG_SW_MAJOR_VERSION             2
#define OSIF_CFG_SW_MINOR_VERSION             0
#define OSIF_CFG_SW_PATCH_VERSION             1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Checks against OsIf_ArchCfg.h */
#if (OSIF_CFG_VENDOR_ID != OSIF_ARCHCFG_VENDOR_ID)
    #error "OsIf_Cfg.h and OsIf_ArchCfg.h have different vendor ids"
#endif
#if ((OSIF_CFG_AR_RELEASE_MAJOR_VERSION    != OSIF_ARCHCFG_AR_RELEASE_MAJOR_VERSION) || \
     (OSIF_CFG_AR_RELEASE_MINOR_VERSION    != OSIF_ARCHCFG_AR_RELEASE_MINOR_VERSION) || \
     (OSIF_CFG_AR_RELEASE_REVISION_VERSION != OSIF_ARCHCFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of OsIf_Cfg.h and OsIf_ArchCfg.h are different"
#endif
#if ((OSIF_CFG_SW_MAJOR_VERSION != OSIF_ARCHCFG_SW_MAJOR_VERSION) || \
     (OSIF_CFG_SW_MINOR_VERSION != OSIF_ARCHCFG_SW_MINOR_VERSION) || \
     (OSIF_CFG_SW_PATCH_VERSION != OSIF_ARCHCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of OsIf_Cfg.h and OsIf_ArchCfg.h are different"
#endif

/* Checks against StandardTypes.h */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((OSIF_CFG_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
         (OSIF_CFG_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of OsIf_Cfg.h and StandardTypes.h are different"
    #endif
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/* General OSIF configuration */
#define OSIF_MODULE_ID                   (255U)

#define OSIF_DRIVER_INSTANCE             ([!"OsIfGeneral/OsIfInstanceId"!]U)

#define OSIF_ENABLE_USER_MODE_SUPPORT    ([!IF "OsIfGeneral/OsIfEnableUserModeSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    #if (STD_ON == OSIF_ENABLE_USER_MODE_SUPPORT)
        #error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running OsIf in user mode, MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined.
    #endif /* (STD_ON == OSIF_ENABLE_USER_MODE_SUPPORT */
#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT */

#define OSIF_ENABLE_MULTICORE_SUPPORT    ([!IF "OsIfGeneral/OsIfMulticoreSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#define OSIF_MAX_COREIDX_SUPPORTED       ([!IF "OsIfGeneral/OsIfMulticoreSupport"!][!"num:i(node:fallback("->num:max(as:modconf('EcuC')/EcucHardware/*/EcucCoreDefinition/*/EcucCoreId) + 1", 1))"!][!ELSE!]1[!ENDIF!]U)  

#define OSIF_DEV_ERROR_DETECT            ([!IF "OsIfGeneral/OsIfDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

[!NOCODE!]

[!IF "node:containsValue(OsIfGeneral/OsIfOperatingSystemType/@name, 'OsIfAutosarOsType')"!]
[!CODE!]#define USING_OS_AUTOSAROS[!ENDCODE!]
[!ENDIF!]

[!IF "node:containsValue(OsIfGeneral/OsIfOperatingSystemType/@name, 'OsIfBaremetalType')"!]
[!CODE!]#define USING_OS_BAREMETAL[!ENDCODE!]
[!ENDIF!]

[!IF "node:containsValue(OsIfGeneral/OsIfOperatingSystemType/@name, 'OsIfFreeRtosType')"!]
[!CODE!]#define USING_OS_FREERTOS[!ENDCODE!]
[!ENDIF!]

[!IF "node:containsValue(OsIfGeneral/OsIfOperatingSystemType/@name, 'OsIfZephyrOsType')"!]
[!CODE!]#define USING_OS_ZEPHYR[!ENDCODE!]
[!ENDIF!]

[!ENDNOCODE!]

#define OSIF_USE_SYSTEM_TIMER            ([!IF "node:containsValue(OsIfGeneral/OsIfUseSystemTimer, 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#define OSIF_USE_CUSTOM_TIMER            ([!IF "node:containsValue(OsIfGeneral/OsIfUseCustomTimer, 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* Autosar OS Specific */


/* Baremetal Specific */
[!NOCODE!]
[!IF "node:containsValue(OsIfGeneral/OsIfOperatingSystemType/@name, 'OsIfBaremetalType') or node:containsValue(OsIfGeneral/OsIfOperatingSystemType/@name, 'OsIfFreeRtosType')"!]
[!CODE!][!//
#if (MCAL_PLATFORM_ARM == MCAL_ARM_MARCH)
    #define OSIF_USE_SYSTICK                 (STD_ON)
#else
    #define OSIF_USE_GENERICTIMER            (STD_ON)
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]


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
#endif /* __cplusplus */   
#endif /* OSIF_CFG_H */
/** @} */
