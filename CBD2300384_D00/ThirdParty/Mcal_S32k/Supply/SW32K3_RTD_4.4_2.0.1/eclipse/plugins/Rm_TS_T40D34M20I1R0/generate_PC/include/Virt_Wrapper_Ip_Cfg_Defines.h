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

#ifndef VIRT_WRAPPER_IP_CFG_DEFINES_H
#define VIRT_WRAPPER_IP_CFG_DEFINES_H

/**
*   @file Virt_Wrapper_Ip_Cfg_Defines.h
*
*   @addtogroup Virt_Wrapper_Ip Virt Wrapper IPV Driver
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
[!CODE!][!WS "0"!]#include "S32K312_VIRT_WRAPPER.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k344')"!][!//
[!CODE!][!WS "0"!]#include "S32K344_VIRT_WRAPPER.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k342')"!][!//
[!CODE!][!WS "0"!]#include "S32K342_VIRT_WRAPPER.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k341')"!][!//
[!CODE!][!WS "0"!]#include "S32K342_VIRT_WRAPPER.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k322')"!][!//
[!CODE!][!WS "0"!]#include "S32K322_VIRT_WRAPPER.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k314')"!][!//
[!CODE!][!WS "0"!]#include "S32K314_VIRT_WRAPPER.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k324')"!][!//
[!CODE!][!WS "0"!]#include "S32K324_VIRT_WRAPPER.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32e27')"!][!//
[!CODE!][!WS "0"!]#include "S32E27_VIRT_WRAP.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32z27')"!][!//
[!CODE!][!WS "0"!]#include "S32Z27_VIRT_WRAP.h"[!CR!][!ENDCODE!][!//
    [!ELSE!][!//
[!CODE!][!WS "0"!]#error "Unknown platform"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_VIRT_WRAPPER_IP_CFG_DEFINES_VENDOR_ID                    43
#define RM_VIRT_WRAPPER_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION     4
#define RM_VIRT_WRAPPER_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION     4
#define RM_VIRT_WRAPPER_IP_CFG_DEFINES_AR_RELEASE_REVISION_VERSION  0
#define RM_VIRT_WRAPPER_IP_CFG_DEFINES_SW_MAJOR_VERSION             2
#define RM_VIRT_WRAPPER_IP_CFG_DEFINES_SW_MINOR_VERSION             0
#define RM_VIRT_WRAPPER_IP_CFG_DEFINES_SW_PATCH_VERSION             1
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
[!IF "RmGeneral/RmVirtWrapperConfigurable"!]
[!IF "RmGeneral/RmEnableUserModeSupport"!]
/** @brief    User mode support          */
#define VIRT_WRAPPER_IP_ENABLE_USER_MODE_SUPPORT

#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    #ifdef VIRT_WRAPPER_IP_ENABLE_USER_MODE_SUPPORT
        #error "MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running VIRT_WRAPPER in user mode the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined"
    #endif /* (VIRT_WRAPPER_IP_ENABLE_USER_MODE_SUPPORT) */
#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT */
[!ELSE!][!ENDIF!]


#define  RM_IP_ENABLE_VIRT_WRAPPER                            ([!IF "RmGeneral/RmVirtWrapperConfigurable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#define  VIRT_WRAPPER_IP_DEV_ERROR_DETECT                     ([!IF "RmConfigSet/Virt_Wrapper_Configuration/VirtWrapperDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!ENDIF!]

#define VIRT_WRAPPER_BASE_WITH_GAP {[!"ecu:get("Rm.VirtWrapper.PhysicalInstances.List")"!]}
#define VIRT_WRAPPER_BASE_WITH_GAP_CNT ([!"num:i(count(ecu:list("Rm.VirtWrapper.PhysicalInstances.List")))"!]UL)

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

#endif /* VIRT_WRAPPER_IP_DEVICE_REGISTERS_H */

