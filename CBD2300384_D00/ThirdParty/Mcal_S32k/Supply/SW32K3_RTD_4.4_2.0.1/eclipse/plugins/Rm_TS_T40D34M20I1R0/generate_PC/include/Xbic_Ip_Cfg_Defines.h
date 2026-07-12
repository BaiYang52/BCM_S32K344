/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : 
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

#ifndef XBIC_IP_CFG_DEFINES_H
#define XBIC_IP_CFG_DEFINES_H

/**
*   @file Xbic_Ip_Cfg_Defines.h
*
*   @addtogroup XBIC_IP XBIC IPV Driver
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
[!NOCODE!][!// Include specific header file
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!IF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k312')"!][!//
[!CODE!][!WS "0"!]#include "S32K312_XBIC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k314')"!][!//
[!CODE!][!WS "0"!]#include "S32K314_XBIC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k324')"!][!//
[!CODE!][!WS "0"!]#include "S32K324_XBIC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k344')"!][!//
[!CODE!][!WS "0"!]#include "S32K344_XBIC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k342')"!][!//
[!CODE!][!WS "0"!]#include "S32K342_XBIC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k341')"!][!//
[!CODE!][!WS "0"!]#include "S32K342_XBIC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k322')"!][!//
[!CODE!][!WS "0"!]#include "S32K322_XBIC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r41')"!][!//
[!CODE!][!WS "0"!]#include "S32R41_XBIC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 'saf85')"!][!//
[!CODE!][!WS "0"!]#include "SAF85XX_XBIC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32e27')"!][!//
[!CODE!][!WS "0"!]#include "S32E27_XBIC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32s27')"!][!//
[!CODE!][!WS "0"!]#include "S32E27_XBIC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32z27')"!][!//
[!CODE!][!WS "0"!]#include "S32Z27_XBIC.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_XBIC_IP_CFG_DEFINES_VENDOR_ID                    43
#define RM_XBIC_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION     4
#define RM_XBIC_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION     4
#define RM_XBIC_IP_CFG_DEFINES_AR_RELEASE_REVISION_VERSION  0
#define RM_XBIC_IP_CFG_DEFINES_SW_MAJOR_VERSION             2
#define RM_XBIC_IP_CFG_DEFINES_SW_MINOR_VERSION             0
#define RM_XBIC_IP_CFG_DEFINES_SW_PATCH_VERSION             1
/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/


/*==================================================================================================
*                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/


/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
[!IF "RmGeneral/RmXbicConfigurable"!]
#define RM_IP_ENABLE_XBIC                              ([!IF "RmGeneral/RmXbicConfigurable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#define XBIC_IP_DEV_ERROR_DETECT                       ([!IF "RmConfigSet/Xbic_Configuration/XbicDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!ENDIF!]

[!IF "RmGeneral/RmEnableUserModeSupport"!]
/** @brief    User mode support          */
#define XBIC_IP_ENABLE_USER_MODE_SUPPORT

#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    #ifdef XBIC_IP_ENABLE_USER_MODE_SUPPORT
        #error "MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running XBIC in user mode the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined"
    #endif /* (STD_ON == XBIC_IP_ENABLE_USER_MODE_SUPPORT) */
#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT */
[!ELSE!][!ENDIF!]

/** @brief    Macro representing of support instance XBIC */
#define XBIC_IP_INSTANCE_COUNT              XBIC_INSTANCE_COUNT
/** @brief    Array initializer of XBIC peripheral base address */
#define XBIC_IP_ARRAY_BASE_ADDRS             IP_XBIC_BASE_ADDRS

#define XBIC_IP_NUM_SLAVE              (8U)

#define XBIC_IP_NUM_MASTER             (8U)

#define XBIC_IP_MAX_SLOT_MASTER_ID     (0xFU)

/* Definition of XBIC Instances */
[!FOR "counter" = "1" TO "num:i(count(ecu:list('Rm.Xbic.XbicInstances.List')))"!][!//
#define [!"ecu:list('Rm.Xbic.XbicInstances.List')[position() = $counter]"!]                  (uint32)([!"num:i(($counter)-1)"!]U)
[!ENDFOR!][!//

/*==================================================================================================
*                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/


/*==================================================================================================
*                                      FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* XBIC_IP_CFG_DEFINES_H */
