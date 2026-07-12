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
* @section EthTrcv_c_REF_3
* Violates MISRA 2004 Required Rule 8.10, All declarations and definitions of objects
* or functions at file scope shall have internal linkage unless external linkage is required
* This warning appears when defining functions that will be used by the upper layers.
* The function in the file are APIs and will be used in the application.
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
#include "EthTrcv.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ETHTRCV_VENDOR_ID_C                      43
#define ETHTRCV_MODULE_ID_C                      73
#define ETHTRCV_AR_RELEASE_MAJOR_VERSION_C       4
#define ETHTRCV_AR_RELEASE_MINOR_VERSION_C       4
#define ETHTRCV_AR_RELEASE_REVISION_VERSION_C    0
#define ETHTRCV_SW_MAJOR_VERSION_C               2
#define ETHTRCV_SW_MINOR_VERSION_C               0
#define ETHTRCV_SW_PATCH_VERSION_C               1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and EthTrcv header file are of the same vendor */
#if (ETHTRCV_VENDOR_ID_C != ETHTRCV_VENDOR_ID)
    #error "EthTrcv.c and EthTrcv.h have different vendor ids"
#endif
/* Check if current file and EthTrcv header file are of the same module id */
#if (ETHTRCV_MODULE_ID_C != ETHTRCV_MODULE_ID)
    #error "EthTrcv.c and EthTrcv.h have different module ids"
#endif
/* Check if current file and EthTrcv header file are of the same Autosar version */
#if ((ETHTRCV_AR_RELEASE_MAJOR_VERSION_C    != ETHTRCV_AR_RELEASE_MAJOR_VERSION) || \
     (ETHTRCV_AR_RELEASE_MINOR_VERSION_C    != ETHTRCV_AR_RELEASE_MINOR_VERSION) || \
     (ETHTRCV_AR_RELEASE_REVISION_VERSION_C != ETHTRCV_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of EthTrcv.c and EthTrcv.h are different"
#endif
/* Check if current file and EthTrcv header file are of the same Software version */
#if ((ETHTRCV_SW_MAJOR_VERSION_C != ETHTRCV_SW_MAJOR_VERSION) || \
     (ETHTRCV_SW_MINOR_VERSION_C != ETHTRCV_SW_MINOR_VERSION) || \
     (ETHTRCV_SW_PATCH_VERSION_C != ETHTRCV_SW_PATCH_VERSION))
    #error "Software Version Numbers of EthTrcv.c and EthTrcv.h are different"
#endif

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

/**
* @brief          This function handles the received Ethernet frame.
* @details        Function should parse the received frame and pass the gathered
*                 information to the appropriate upper layer module.
* @param[in]      CtrlIdx Index of the controller which read Mii.
* @param[in]      TrcvIdx The Id of transceiver was read Mii
* @param[in]      RegIdx The Id or Address of Register was read
* @param[in]      RegVal The value of register.
*
* @implements EthTrcv_ReadMiiIndication_Activity
*
* @violates @ref EthTrcv_c_REF_3 Violates MISRA 2004 Required Rule 8.10 This is an interrupt handler
*/
void EthTrcv_ReadMiiIndication  (
                                        uint8 CtrlIdx, \
                                        uint8 TrcvIdx, \
                                        uint8 RegIdx, \
                                        uint16 RegVal \
                                    )
{
    /* This is an empty stub function */
    (void)CtrlIdx;
    (void)TrcvIdx;
    (void)RegIdx;
    (void)RegVal;
}

/*================================================================================================*/
/**
* @brief          This function handles the received Ethernet frame.
* @details        Function should parse the received frame and pass the gathered
*                 information to the appropriate upper layer module.
* @param[in]      CtrlIdx Index of the controller which write Mii.
* @param[in]      TrcvIdx The Id of transceiver was write Mii
* @param[in]      RegIdx The Id or Address of Register was write
*
* @implements EthTrcv_WriteMiiIndication_Activity
*
* @violates @ref EthTrcv_c_REF_3 Violates MISRA 2004 Required Rule 8.10 This is an interrupt handler
*/
void EthTrcv_WriteMiiIndication (
                                        uint8 CtrlIdx, \
                                        uint8 TrcvIdx, \
                                        uint8 RegIdx \
                                    )
{
    /* This is an empty stub function */
    (void)CtrlIdx;
    (void)TrcvIdx;
    (void)RegIdx;
}


#ifdef __cplusplus
}
#endif

/** @} */
