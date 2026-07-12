
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : FLEXCAN
*   Dependencies         : 
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
#ifndef FLEXCAN_IP_CFGDEFINES_H
#define FLEXCAN_IP_CFGDEFINES_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit

==================================================================================================*/
[!INDENT "0"!]
[!NOCODE!][!// Include headers for FlexCAN_Ip_DeviceReg.h
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!// 
    [!VAR "DerivativeName" = "text:toupper(substring-before(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative,'_'))"!]
   
    [!IF "contains($DerivativeName, 'S32K3')"!][!//  
        [!IF "contains($DerivativeName, 'S32K341')"!][!//
            /* Use k342 header files instead of K341 that is not available */
            [!VAR "DerivativeName" = "'S32K342'"!]
        [!ENDIF!][!//
        [!CODE!][!WS "0"!]
        #include "[!"$DerivativeName"!].h"
        [!CR!][!ENDCODE!][!//
    [!ELSE!]
        [!CODE!][!WS "0"!]
        #error "Unknown platform!"
        [!CR!][!ENDCODE!][!//
    [!ENDIF!][!// 
[!ELSE!]
    [!CODE!][!WS "0"!]
    #error "Unknown platform!"
    [!CR!][!ENDCODE!][!//
[!ENDIF!][!// 
[!ENDNOCODE!][!//
[!ENDINDENT!]
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/*
* @file           FlexCAN_Ip_CfgDefines.h
*/
#define FLEXCAN_IP_CFGDEFINES_VENDOR_ID_H                      43
#define FLEXCAN_IP_CFGDEFINES_AR_RELEASE_MAJOR_VERSION_H       4
#define FLEXCAN_IP_CFGDEFINES_AR_RELEASE_MINOR_VERSION_H       4
#define FLEXCAN_IP_CFGDEFINES_AR_RELEASE_REVISION_VERSION_H    0
#define FLEXCAN_IP_CFGDEFINES_SW_MAJOR_VERSION_H               2
#define FLEXCAN_IP_CFGDEFINES_SW_MINOR_VERSION_H               0
#define FLEXCAN_IP_CFGDEFINES_SW_PATCH_VERSION_H               1
/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FLEXCAN_IP_CFGDEFINES_H */
