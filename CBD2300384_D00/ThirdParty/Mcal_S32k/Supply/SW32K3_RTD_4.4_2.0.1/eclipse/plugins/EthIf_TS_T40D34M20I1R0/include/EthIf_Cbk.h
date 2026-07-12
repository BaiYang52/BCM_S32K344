/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : 
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

#ifndef ETHIF_CBK_H
#define ETHIF_CBK_H

/**
*   @file
*
*   @addtogroup ETHIF Ethernet Interface
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

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
#define ETHIF_VENDOR_ID                    43
#define ETHIF_MODULE_ID                    65
#define ETHIF_AR_RELEASE_MAJOR_VERSION     4
#define ETHIF_AR_RELEASE_MINOR_VERSION     4
#define ETHIF_AR_RELEASE_REVISION_VERSION  0
#define ETHIF_SW_MAJOR_VERSION             2
#define ETHIF_SW_MINOR_VERSION             0
#define ETHIF_SW_PATCH_VERSION             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if source file and Eth_GeneralTypes.h header file are of the same Autosar version */
    #if ((ETHIF_AR_RELEASE_MAJOR_VERSION != ETH_GENERALTYPES_AR_RELEASE_MAJOR_VERSION) || \
         (ETHIF_AR_RELEASE_MINOR_VERSION != ETH_GENERALTYPES_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of EthIf.h and Eth_GeneralTypes.h are different"
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
extern volatile uint32 EthIf_RxIndications[10];
extern volatile uint32 EthIf_TxConfirmations[10];
extern volatile boolean EthIf_ModeIndications[10];
extern volatile uint16 EthIf_ChecksumValue[10];

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

void EthIf_RxIndication(\
                        uint8 CtrlIdx,\
                        Eth_FrameType FrameType, \
                        boolean IsBroadcast, \
                        const uint8* PhysAddrPtr, \
                        const Eth_DataType* DataPtr,\
                        uint16 LenByte);

void EthIf_TxConfirmation(uint8 CtrlIdx, \
                          Eth_BufIdxType BufIdx, \
                          Std_ReturnType Result);

void EthIf_CtrlModeIndication( \
                                uint8 CtrlIdx, \
                                Eth_ModeType CtrlMode \
                             );

void EthIf_SwitchIngressTimeStampIndication(uint8 CtrlIdx,
                                            Eth_DataType* DataPtr,
                                            EthSwt_MgmtInfoType* MgmtInfoPtr,
                                            Eth_TimeStampType* timeStampPtr
                                           );

void EthIf_SwitchEgressTimeStampIndication(uint8 CtrlIdx,
                                           Eth_DataType* DataPtr,
                                           EthSwt_MgmtInfoType* MgmtInfoPtr,
                                           Eth_TimeStampType* timeStampPtr
                                          );

void EthIf_SwitchMgmtInfoIndication(uint8 CtrlIdx,
                                    Eth_DataType* DataPtr,
                                    EthSwt_MgmtInfoType* MgmtInfoPtr
                                   );
    
void EthIf_TrcvModeIndication(uint8 TrcvIdx,
                              EthTrcv_ModeType TrcvMode
                             );

void EthIf_SwitchPortModeIndication(uint8 SwitchIdx,
                                    uint8 SwitchPortIdx,
                                    EthTrcv_ModeType PortMode
                                   );

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* ETHIF_CBK_H */                                     
