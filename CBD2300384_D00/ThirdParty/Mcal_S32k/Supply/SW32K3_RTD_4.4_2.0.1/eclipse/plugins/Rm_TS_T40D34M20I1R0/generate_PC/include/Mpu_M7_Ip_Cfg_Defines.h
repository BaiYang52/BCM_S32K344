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

#ifndef MPU_M7_IP_CFG_DEFINES_H_
#define MPU_M7_IP_CFG_DEFINES_H_

/**
*   @file Mpu_M7_Ip_CfgDefines.h
*
*   @addtogroup Mpu_M7_Ip Mpu M7 IPV Driver
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
                                         INCLUDE FILES
==================================================================================================*/
#include "StandardTypes.h"
[!NOCODE!][!// Include specific header file
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!IF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k312')"!][!//
[!CODE!][!WS "0"!]
#include "S32K312_MPU.h"
#include "S32K312_SCB.h"
#include "S32K312_MSCM.h"
[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k344')"!][!//
[!CODE!][!WS "0"!]
#include "S32K344_MPU.h"
#include "S32K344_SCB.h"
#include "S32K344_MSCM.h"
[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k342')"!][!//
[!CODE!][!WS "0"!]
#include "S32K342_MPU.h"
#include "S32K342_SCB.h"
#include "S32K342_MSCM.h"
[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k341')"!][!//
[!CODE!][!WS "0"!]
#include "S32K342_MPU.h"
#include "S32K342_SCB.h"
#include "S32K342_MSCM.h"
[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k322')"!][!//
[!CODE!][!WS "0"!]
#include "S32K322_MPU.h"
#include "S32K322_SCB.h"
#include "S32K322_MSCM.h"
[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k314')"!][!//
[!CODE!][!WS "0"!]
#include "S32K314_MPU.h"
#include "S32K314_SCB.h"
#include "S32K314_MSCM.h"
[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k324')"!][!//
[!CODE!][!WS "0"!]
#include "S32K324_MPU.h"
#include "S32K324_SCB.h"
#include "S32K324_MSCM.h"
[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r41')"!][!//
[!CODE!][!WS "0"!]
#include "S32R41_MPU.h"
#include "S32R41_SCB.h"
#include "S32R41_MSCM.h"
[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 'saf85')"!][!//
[!CODE!][!WS "0"!]
#include "SAF85XX_MPU.h"
#include "SAF85XX_SCB.h"
#include "SAF85XX_MSCM.h"
[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32g2')"!][!//
[!CODE!][!WS "0"!]
#include "S32G274A_MPU.h"
#include "S32G274A_SCB.h"
#include "S32G274A_MSCM.h"
[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32g3')"!][!//
[!CODE!][!WS "0"!]
#include "S32G399A_MPU.h"
#include "S32G399A_SCB.h"
#include "S32G399A_MSCM.h"
[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r45')"!][!//
[!CODE!][!WS "0"!]
#include "S32R45_MPU.h"
#include "S32R45_SCB.h"
#include "S32R45_MSCM.h"
[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_MPU_M7_IP_CFG_DEFINES_VENDOR_ID                    43
#define RM_MPU_M7_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION     4
#define RM_MPU_M7_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION     4
#define RM_MPU_M7_IP_CFG_DEFINES_AR_RELEASE_REVISION_VERSION  0
#define RM_MPU_M7_IP_CFG_DEFINES_SW_MAJOR_VERSION             2
#define RM_MPU_M7_IP_CFG_DEFINES_SW_MINOR_VERSION             0
#define RM_MPU_M7_IP_CFG_DEFINES_SW_PATCH_VERSION             1


/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Checks against StandardTypes.h */
#if ((RM_MPU_M7_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
    (RM_MPU_M7_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION  != STD_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Mpu_M7_Ip_Cfg_Defines.h and StandardTypes.h are different"
#endif
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
[!IF "RmGeneral/RmMpuM7Configurable"!][!//
/** @brief    Development error checking */
#define MPU_M7_IP_DEV_ERROR_DETECT                          ([!IF "RmConfigSet/MPU_M7_Configuration/MpuDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
/**
* @brief            Support for clearing cache.
*                   STD_ON: the Rm driver will clear cache before initialization of regions.
*                   STD_OFF: the Rm driver  will not clear cache before initialization of regions.
*/
#define MPU_M7_ENABLE_CLEAR_CACHE                            ([!IF "RmConfigSet/MPU_M7_Configuration/ClearCacheEnable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!IF "RmGeneral/RmEnableUserModeSupport"!][!//

/** @brief    User mode support          */
#define MPU_M7_IP_ENABLE_USER_MODE_SUPPORT
#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    #ifdef MPU_M7_IP_ENABLE_USER_MODE_SUPPORT
        #error "MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running MPU_M7 in user mode the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined"
    #endif /* (ifdef MPU_M7_IP_ENABLE_USER_MODE_SUPPORT) */
#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT */
[!ELSE!][!//
[!ENDIF!][!//

#define MPU_M7_IP_MIN_REGION_SIZE                           ([!"ecu:get('Rm.Mpu_M7.MinRegionSize')"!]U)
#define MPU_M7_IP_MIN_REGION_SIZE_WITH_SUBREGION            ([!"ecu:get('Rm.Mpu_M7.MinRegionSizeWithSubregion')"!]U)
#define MPU_M7_IP_EXECUTE_RIGHT_MASK                        ([!"ecu:get('Rm.Mpu_M7.ExecuteRightMask')"!]U)
#define MPU_M7_IP_SCB_CFSR_MMFSR_MASK_WITHOUT_VALID         ([!"ecu:get('Rm.Mpu_M7.MmfsrMaskWithoutValid')"!]U)
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

/** @} */

#endif /* MPU_M7_IP_CFG_DEFINES_H_ */

