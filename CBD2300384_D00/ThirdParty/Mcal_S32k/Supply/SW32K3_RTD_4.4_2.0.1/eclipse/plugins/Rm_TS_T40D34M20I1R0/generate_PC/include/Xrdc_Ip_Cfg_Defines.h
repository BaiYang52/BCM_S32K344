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

#ifndef XRDC_IP_CFG_DEFINES_H
#define XRDC_IP_CFG_DEFINES_H

/**
*   @file Xrdc_Ip_Cfg_Defines.h
*
*   @addtogroup XRDC_IP XRDC IPV Driver
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
[!CODE!][!WS "0"!]#include "S32K312_XRDC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k344')"!][!//
[!CODE!][!WS "0"!]#include "S32K344_XRDC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k342')"!][!//
[!CODE!][!WS "0"!]#include "S32K342_XRDC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k341')"!][!//
[!CODE!][!WS "0"!]#include "S32K342_XRDC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k322')"!][!//
[!CODE!][!WS "0"!]#include "S32K322_XRDC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k314')"!][!//
[!CODE!][!WS "0"!]#include "S32K314_XRDC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k324')"!][!//
[!CODE!][!WS "0"!]#include "S32K324_XRDC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32g2')"!][!//
[!CODE!][!WS "0"!]#include "S32G274A_XRDC_0.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32G274A_XRDC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32g3')"!][!//
[!CODE!][!WS "0"!]#include "S32G399A_XRDC_0.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32G399A_XRDC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r45')"!][!//
[!CODE!][!WS "0"!]#include "S32R45_XRDC_0.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32R45_XRDC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r41')"!][!//
[!CODE!][!WS "0"!]#include "S32R41_XRDC_0.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32R41_XRDC_1.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 'saf85')"!][!//
[!CODE!][!WS "0"!]#include "SAF85XX_XRDC_0.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "SAF85XX_XRDC_1.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32e27')"!][!//
[!CODE!][!WS "0"!]#include "S32E27_XRDC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32E27_SMU_XRDC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32E27_RTU_XRDC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32s27')"!][!//
[!CODE!][!WS "0"!]#include "S32E27_XRDC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32E27_SMU_XRDC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32E27_RTU_XRDC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32z27')"!][!//
[!CODE!][!WS "0"!]#include "S32Z27_XRDC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32Z27_SMU_XRDC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32Z27_RTU_XRDC.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_XRDC_IP_CFG_DEFINES_VENDOR_ID                    43
#define RM_XRDC_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION     4
#define RM_XRDC_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION     4
#define RM_XRDC_IP_CFG_DEFINES_AR_RELEASE_REVISION_VERSION  0
#define RM_XRDC_IP_CFG_DEFINES_SW_MAJOR_VERSION             2
#define RM_XRDC_IP_CFG_DEFINES_SW_MINOR_VERSION             0
#define RM_XRDC_IP_CFG_DEFINES_SW_PATCH_VERSION             1
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
[!IF "RmGeneral/RmEnableXRDCSupport"!]
[!IF "RmGeneral/RmEnableUserModeSupport"!]
/** @brief    User mode support          */
#define XRDC_IP_ENABLE_USER_MODE_SUPPORT

#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    #ifdef XRDC_IP_ENABLE_USER_MODE_SUPPORT
        #error "MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running XRDC in user mode the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined"
    #endif /* (STD_ON == XRDC_IP_ENABLE_USER_MODE_SUPPORT) */
#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT */
[!ENDIF!][!//

[!IF "RmConfigSet/Xrdc_Configuration/XrdcRegistersLock"!][!//
#define XRDC_IP_CFG_INFO_8 \
[!NOCODE!][!//
[!FOR "i" = "1" TO "num:i(ecu:get('Rm.XRDC.XRDCInstancesCount'))"!][!//
[!VAR "InstanceName" = "text:tolower(substring-after(ecu:list('Rm.XRDC.XRDCInstances.List')[position()=$i],'_'))"!]
[!CODE!][!//
static const uint8 Xrdc_[!"$InstanceName"!]_Master[[!"num:i(count(ecu:list(text:replace('Rm.XRDC.Master.Mdac.InstanceName.List','InstanceName',$InstanceName))))"!]] = {[!"text:replaceAll(ecu:get(text:replace('Rm.XRDC.Master.Mdac.InstanceName.List','InstanceName',$InstanceName)),'XRDC','(uint8)XRDC')"!]};\
static const uint8 Xrdc_[!"$InstanceName"!]_MrcInstances[[!"num:i(count(ecu:list(text:replace('Rm.XRDC.Mrc.InstanceName.List','InstanceName',$InstanceName))))"!]] = {[!"text:replaceAll(ecu:get(text:replace('Rm.XRDC.Mrc.InstanceName.List','InstanceName',$InstanceName)),'XRDC','(uint8)XRDC')"!]};[!IF "num:i($i)<(num:i(ecu:get('Rm.XRDC.XRDCInstancesCount')))"!]\[!CR!][!ELSE!][!CR!][!ENDIF!][!//
[!ENDCODE!][!//
[!ENDFOR!][!//
[!ENDNOCODE!][!//

#define XRDC_IP_CFG_INFO_16 \
[!NOCODE!][!//
[!FOR "i" = "1" TO "num:i(ecu:get('Rm.XRDC.XRDCInstancesCount'))"!][!//
[!VAR "InstanceName" = "text:tolower(substring-after(ecu:list('Rm.XRDC.XRDCInstances.List')[position()=$i],'_'))"!]
[!CODE!][!//
static const uint16 Xrdc_[!"$InstanceName"!]_Perlist[[!"num:i(count(ecu:list(text:replace('Rm.XRDC.PdacNumber.Instance.List','Instance',$InstanceName))))"!]] = {[!"ecu:get(text:replace('Rm.XRDC.PdacNumber.Instance.List','Instance',$InstanceName))"!]};[!IF "num:i($i)<(num:i(ecu:get('Rm.XRDC.XRDCInstancesCount')))"!]\[!CR!][!ELSE!][!CR!][!ENDIF!][!//
[!ENDCODE!][!//
[!ENDFOR!][!//
[!ENDNOCODE!][!//

#define XRDC_MASTER_INSTANCE                         {[!FOR "i" = "1" TO "num:i(ecu:get('Rm.XRDC.XRDCInstancesCount'))"!]Xrdc_[!"text:tolower(substring-after(ecu:list('Rm.XRDC.XRDCInstances.List')[position()=$i],'_'))"!]_Master[!IF "num:i($i)<(num:i(ecu:get('Rm.XRDC.XRDCInstancesCount')))"!],[!ELSE!][!ENDIF!][!ENDFOR!]}
#define XRDC_PER_INSTANCE                            {[!FOR "i" = "1" TO "num:i(ecu:get('Rm.XRDC.XRDCInstancesCount'))"!]Xrdc_[!"text:tolower(substring-after(ecu:list('Rm.XRDC.XRDCInstances.List')[position()=$i],'_'))"!]_Perlist[!IF "num:i($i)<(num:i(ecu:get('Rm.XRDC.XRDCInstancesCount')))"!],[!ELSE!][!ENDIF!][!ENDFOR!]}
#define XRDC_NUM_PER_INSTANCE                        {[!FOR "i" = "1" TO "num:i(ecu:get('Rm.XRDC.XRDCInstancesCount'))"!]XRDC_[!"substring-after(ecu:list('Rm.XRDC.XRDCInstances.List')[position()=$i],'_')"!]_NUM_PER[!IF "num:i($i)<(num:i(ecu:get('Rm.XRDC.XRDCInstancesCount')))"!],[!ELSE!][!ENDIF!][!ENDFOR!]}
#define XRDC_MRC_INSTANCE                            {[!FOR "i" = "1" TO "num:i(ecu:get('Rm.XRDC.XRDCInstancesCount'))"!]Xrdc_[!"text:tolower(substring-after(ecu:list('Rm.XRDC.XRDCInstances.List')[position()=$i],'_'))"!]_MrcInstances[!IF "num:i($i)<(num:i(ecu:get('Rm.XRDC.XRDCInstancesCount')))"!],[!ELSE!][!ENDIF!][!ENDFOR!]}

[!NOCODE!][!//
[!FOR "i" = "1" TO "num:i(ecu:get('Rm.XRDC.XRDCInstancesCount'))"!][!//
[!VAR "InstanceName" = "text:tolower(substring-after(ecu:list('Rm.XRDC.XRDCInstances.List')[position()=$i],'_'))"!]
[!CODE!][!//
#define XRDC_[!"text:toupper($InstanceName)"!]_NUM_PER                        (uint8)([!"num:i(count(ecu:list(text:replace('Rm.XRDC.PdacNumber.Instance.List','Instance',$InstanceName))))"!]U)

[!ENDCODE!][!//
[!ENDFOR!][!//
[!ENDNOCODE!][!//
[!ENDIF!][!//
#define XRDC_COUNT                                   (uint32)([!"ecu:get('Rm.XRDC.XRDCInstancesCount')"!]UL)

#define XRDC_NUMBER_OF_DOMAINID                      (uint32)([!"num:i(count(ecu:list('Rm.XRDC.DomainID')))"!]UL)

#define XRDC_BASE_ADDRS                              { IP_XRDC_BASE }

#define XRDC_MOD_BASEADDR_ADDR32(instance)           (Xrdc_Ip_InstanceAddress[instance])

#define XRDC_CR_OFFSET                               (uint32)(0x0U)

#define XRDC_HWCFG0_OFFSET                           (uint32)(0xF0U)
#define XRDC_HWCFG1_OFFSET                           (uint32)(0xF4U)
#define XRDC_HWCFG2_OFFSET                           (uint32)(0xF8U)

#define XRDC_MDACFG_OFFSET                           (uint32)(0x100U)

#define XRDC_MRCFG_OFFSET                            (uint32)(0x140U)

#define XRDC_DERRLOC_OFFSET                          (uint32)(0x200U)

#define XRDC_DERR_W_OFFSET                           (uint32)(0x400U)

#define XRDC_PID_OFFSET                              (uint32)(0x700U)

#define XRDC_MDA_W_DFMT0_OFFSET                      (uint32)(0x800U)

#define XRDC_PDAC_W0_OFFSET                          (uint32)(0x1000U)

#define XRDC_PDAC_W1_OFFSET                          (uint32)(0x1004U)

#define XRDC_MRGD_OFFSET                             (uint32)(0x2000U)
[!ENDIF!][!//

#define XRDC_MAX_OF_ERROR_RECORDED                   (uint32)([!"ecu:get('Rm.XRDC.ErrorInstancesCount')"!]UL)

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

#endif /* XRDC_IP_DEVICE_REGISTERS_H */


