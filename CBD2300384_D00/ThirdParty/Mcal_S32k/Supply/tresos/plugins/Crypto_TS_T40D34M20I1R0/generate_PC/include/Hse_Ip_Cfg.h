/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : HSE
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

#ifndef HSE_IP_CFG_H
#define HSE_IP_CFG_H

/**
*   @file
*
*   @addtogroup CRYPTO
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
[!INDENT "0"!][!//
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!IF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32g2')"!][!//
#include "S32G274A_MU.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32g3')"!][!//
#include "S32G399A_MU.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r45')"!][!//
#include "S32R45_MU.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r41')"!][!//
#include "S32R41_MU.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 'saf8544')"!][!//
#include "SAF85XX_MU.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k312')"!][!//
#include "S32K312_MU.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k314')"!][!//
#include "S32K314_MU.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k324')"!][!//
#include "S32K324_MU.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k344')"!][!//
#include "S32K344_MU.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k342')"!][!//
#include "S32K342_MU.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k341')"!][!//
#include "S32K342_MU.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k322')"!][!//
#include "S32K322_MU.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32z27')"!][!//
#include "S32Z27_MU.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32e27')"!][!//
#include "S32E27_MU.h"
    [!ENDIF!][!//
[!ELSE!][!//
    [!ERROR "The Resource plugin or ResourceSubderivative field is not available for header file inclusion."!][!//
[!ENDIF!][!//
[!ENDINDENT!][!//
#include "OsIf.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define HSE_IP_CFG_VENDOR_ID_H                       43
#define HSE_IP_CFG_SW_MAJOR_VERSION_H                2
#define HSE_IP_CFG_SW_MINOR_VERSION_H                0
#define HSE_IP_CFG_SW_PATCH_VERSION_H                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/* Pre-processor switch to enable/disable development error detection for Hse Ip API */
#define HSE_IP_DEV_ERROR_DETECT                  ([!IF "CryptoGeneral/HseIpDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

[!NOCODE!][!//
    [!IF "node:value(CryptoGeneral/CryptoTimeoutMethod) = 'OSIF_COUNTER_SYSTEM'"!]
        [!VAR "StrCounterType" = "'SYSTEM'"!][!//
        [!INDENT "0"!][!//
        [!CODE!][!//
            /* Check if the chosen counter type is enabled */
            #if (STD_OFF == OSIF_USE_SYSTEM_TIMER)
                [!INDENT "4"!][!//
                #error "OSIF System counter is disabled"
                [!ENDINDENT!][!//
            #endif
        [!ENDCODE!][!//
        [!ENDINDENT!][!//
    [!ELSEIF "node:value(CryptoGeneral/CryptoTimeoutMethod) = 'OSIF_COUNTER_CUSTOM'"!]
        [!VAR "StrCounterType" = "'CUSTOM'"!][!//
        [!INDENT "0"!][!//
        [!CODE!][!//
            /* Check if the chosen counter type is enabled */
            #if (STD_OFF == OSIF_USE_CUSTOM_TIMER)
                [!INDENT "4"!][!//
                #error "OSIF Custom counter is disabled"
                [!ENDINDENT!][!//
            #endif
        [!ENDCODE!][!//
        [!ENDINDENT!][!//
    [!ELSE!][!//
        [!VAR "StrCounterType" = "'DUMMY'"!][!//
    [!ENDIF!][!//
[!ENDNOCODE!][!//

/* OsIf counter type used in timeout detection for HSE IP service request */
#define HSE_IP_TIMEOUT_OSIF_COUNTER_TYPE         (OSIF_COUNTER_[!"$StrCounterType"!])

[!INDENT "0"!][!//
/* Initializer for the MU Host base addresses */
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!IF "text:match(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32g[23]')"!][!//
#define MU_HOST_BASE_PTRS                        { IP_MU0__MUB, IP_MU1__MUB, IP_MU2__MUB, IP_MU3__MUB }
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r45')"!][!//
#define MU_HOST_BASE_PTRS                        { IP_MU0__MUB, IP_MU1__MUB, IP_MU2__MUB, IP_MU3__MUB }
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k3')"!][!//
/* For communication with the firmware only MU_0 and MU_1 are used, MU_2 is only used for communication between application cores */
#define MU_HOST_BASE_PTRS                        { IP_MU_0__MUB, IP_MU_1__MUB }
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r41')"!][!//
#define MU_HOST_BASE_PTRS                        { IP_MU0__MUB, IP_MU1__MUB }
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 'saf8544')"!][!//
#define MU_HOST_BASE_PTRS                        { IP_MU0__MUB, IP_MU1__MUB }
    [!ELSEIF "text:match(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32[ze]27')"!][!//
#define MU_HOST_BASE_PTRS                        { IP_MU0__MUB, IP_MU1__MUB, IP_MU2__MUB, IP_MU3__MUB, IP_MU4__MUB, IP_MU5__MUB, IP_MU6__MUB, IP_MU7__MUB }
    [!ENDIF!][!//
[!ELSE!][!//
    [!ERROR "The Resource plugin or ResourceSubderivative field is not available."!][!//
[!ENDIF!][!//
[!ENDINDENT!][!//

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

#endif /* HSE_IP_CFG_H */

