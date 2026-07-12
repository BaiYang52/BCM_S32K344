/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : STUB
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

#ifndef ETHTRCV_H
#define ETHTRCV_H

/**
*   @file
*
*   @addtogroup ETHTRCV Ethernet Transceiver
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/**
* @page misra_violations MISRA-C:2004 violations
*
* @section [global] 
* Violates MISRA 2004 Required Rule 1.4, The compiler/linker shall be checked to ensure that 31 character significance 
* and case  sensitivity are supported for external identifiers.
* All compilers used support more than 31 chars for identifiers.
*
* @section [global]
* Violates MISRA 2004 Required Rule 5.1, Identifiers (internal and external) shall not rely
* on the significance of more than 31 characters. 
* All compilers used support more than 31 chars for identifiers.
*
* @section [global]
* Violates MISRA 2004 Required Rule 20.2, The names of standard library macros,
* objects and functions shall not be reused. All defines starting with upper
* letter E are reserved for future use inside <error.h> header file.
* All module define options and enumerations are restricted by coding rules
* and must follow format <MSN>_<NAME> where MSN is equal to ETHTRCV.
*/
/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Eth_GeneralTypes.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ETHTRCV_VENDOR_ID                    43
#define ETHTRCV_MODULE_ID                    73
#define ETHTRCV_AR_RELEASE_MAJOR_VERSION     4
#define ETHTRCV_AR_RELEASE_MINOR_VERSION     4
#define ETHTRCV_AR_RELEASE_REVISION_VERSION  0
#define ETHTRCV_SW_MAJOR_VERSION             2
#define ETHTRCV_SW_MINOR_VERSION             0
#define ETHTRCV_SW_PATCH_VERSION             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if source file and Eth.h header file are of the same Autosar version */
    #if ((ETHTRCV_AR_RELEASE_MAJOR_VERSION != ETH_GENERALTYPES_AR_RELEASE_MAJOR_VERSION) || \
         (ETHTRCV_AR_RELEASE_MINOR_VERSION != ETH_GENERALTYPES_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of EthTrcv.h and Eth_GeneralTypes.h are different"
    #endif
#endif

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                            ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

extern void EthTrcv_ReadMiiIndication  (
                                        uint8 CtrlIdx, \
                                        uint8 TrcvIdx, \
                                        uint8 RegIdx, \
                                        uint16 RegVal \
                                    );

extern void EthTrcv_WriteMiiIndication (
                                        uint8 CtrlIdx, \
                                        uint8 TrcvIdx, \
                                        uint8 RegIdx \
                                    );
#ifdef __cplusplus
}
#endif

/** @} */

#endif /* ETHTRCV_H */
