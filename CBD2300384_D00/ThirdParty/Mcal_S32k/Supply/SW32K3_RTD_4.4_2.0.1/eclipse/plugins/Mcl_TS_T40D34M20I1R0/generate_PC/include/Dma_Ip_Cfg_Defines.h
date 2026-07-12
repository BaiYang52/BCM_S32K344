/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : DMA,CACHE,TRGMUX,LCU,EMIOS,FLEXIO
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

/* Prevention from multiple including the same header */
#ifndef DMA_IP_CFG_DEFINES_H_
#define DMA_IP_CFG_DEFINES_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "StandardTypes.h"
#include "BasicTypes.h"

/*==================================================================================================
                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define DMA_IP_CFG_DEFINES_VENDOR_ID                       43
#define DMA_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION        4
#define DMA_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION        4
#define DMA_IP_CFG_DEFINES_AR_RELEASE_REVISION_VERSION     0
#define DMA_IP_CFG_DEFINES_SW_MAJOR_VERSION                2
#define DMA_IP_CFG_DEFINES_SW_MINOR_VERSION                0
#define DMA_IP_CFG_DEFINES_SW_PATCH_VERSION                1

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if header file and StandardTypes header file are of the same Autosar version */
#if ((DMA_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
     (DMA_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Dma_Ip_Cfg_Defines.h and StandardTypes.h are different"
#endif
#endif

[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!VAR "DerivativeName" = "text:toupper(substring-before(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative,'_'))"!]
    [!IF "($DerivativeName = 'S32K322') or ($DerivativeName = 'S32K341')"!][!//
        [!VAR "DerivativeName" = "text:toupper('S32K342')"!][!//
    [!ENDIF!][!//
[!ENDIF!][!//

/*===============================================================================================
                                       DEFINES AND MACROS
===============================================================================================*/
/*-----------------------------------------------/
/  DMA IP USER MODE SUPPORT                      /
/-----------------------------------------------*/
[!IF "node:value(./MclGeneral/MclEnableUserModeSupport)"!][!//
#define DMA_IP_USER_MODE_SUPPORT_IS_AVAILABLE      STD_ON
#define DMA_IP_REG_PROT_AVAILABLE                  STD_ON
[!ELSE!][!//
#define DMA_IP_USER_MODE_SUPPORT_IS_AVAILABLE      STD_OFF
#define DMA_IP_REG_PROT_AVAILABLE                  STD_OFF
[!ENDIF!][!//

#define DMA_IP_MP_REG_PROT_AVAILABLE               STD_ON
#define DMA_IP_TCD_REG_PROT_AVAILABLE              STD_ON
#define DMA_IP_DMAMUX_REG_PROT_AVAILABLE           STD_ON

/*-----------------------------------------------/
/  DMAMUX IP SUPPORT                             /
/-----------------------------------------------*/
#define DMA_IP_DMAMUX_IS_AVAILABLE                 STD_ON

/*-----------------------------------------------/
/  DMA IP SUPPORT                                /
/-----------------------------------------------*/
[!IF "node:value(./MclGeneral/MclDma/MclEnableDma)"!][!//
#define DMA_IP_IS_AVAILABLE                        STD_ON

#define DMA_IP_DMACRC_IS_AVAILABLE                 STD_OFF

/*-----------------------------------------------/
/  DMA IP VIRTUAL ADDRESS MAPPING SUPPORT        /
/-----------------------------------------------*/
[!IF "node:value(./MclGeneral/MclEnableVirtualAddressMappingSupport)"!][!//
#define DMA_IP_VIRTUAL_ADDRESS_MAPPING_IS_AVAILABLE      STD_ON
[!ELSE!][!//
#define DMA_IP_VIRTUAL_ADDRESS_MAPPING_IS_AVAILABLE      STD_OFF
[!ENDIF!][!//

[!IF "node:value(./MclGeneral/MclEnableMulticoreSupport)='true' and (num:i(ecu:get('Mcl.MultiCore.Support')) = 1)"!][!//
#define DMA_IP_MULTICORE_IS_AVAILABLE              STD_ON
[!ELSE!][!//
#define DMA_IP_MULTICORE_IS_AVAILABLE              STD_OFF
[!ENDIF!][!//

#define DMA_IP_MASTER_ID_REPLICATION_IS_AVAILABLE  STD_ON

#define DMA_IP_BUFFERED_WRITES_IS_AVAILABLE        STD_ON

#define DMA_IP_STORE_DST_ADDR_IS_AVAILABLE         STD_ON

 #define DMA_IP_END_OF_PACKET_SIGNAL_IS_AVAILABLE   STD_ON
 
#define DMA_IP_PREEMPTION_IS_AVAILABLE             STD_ON

#define DMA_IP_DISABLE_PREEMPT_IS_AVAILABLE        STD_ON

#define DMA_IP_GROUP_PRIORITY_IS_AVAILABLE         STD_ON
[!ELSE!][!//
#define DMA_IP_IS_AVAILABLE                              STD_OFF

#define DMA_IP_DMACRC_IS_AVAILABLE                       STD_OFF

#define DMA_IP_VIRTUAL_ADDRESS_MAPPING_IS_AVAILABLE      STD_OFF

#define DMA_IP_MULTICORE_IS_AVAILABLE              STD_OFF

#define DMA_IP_MASTER_ID_REPLICATION_IS_AVAILABLE  STD_OFF

#define DMA_IP_BUFFERED_WRITES_IS_AVAILABLE        STD_OFF

#define DMA_IP_STORE_DST_ADDR_IS_AVAILABLE         STD_OFF

#define DMA_IP_END_OF_PACKET_SIGNAL_IS_AVAILABLE   STD_OFF

#define DMA_IP_PREEMPTION_IS_AVAILABLE             STD_OFF

#define DMA_IP_DISABLE_PREEMPT_IS_AVAILABLE        STD_OFF

#define DMA_IP_GROUP_PRIORITY_IS_AVAILABLE         STD_OFF
[!ENDIF!][!//

/* This option defines wether DMA Interrupts are suported per channel or per group of channels. */
 #define DMA_IP_IRQ_DISPATCHER_IS_AVAILABLE          STD_OFF
 
/*-----------------------------------------------/
/  DMA IP DEV ERROR DETECT SUPPORT               /
/-----------------------------------------------*/
#define DMA_IP_DEV_ERROR_DETECT                    STD_OFF

/*-----------------------------------------------/
/  DMAMUX NOT ALIGNED                            /
/-----------------------------------------------*/
#define DMAMUX_IP_NOT_ALIGNED                            STD_OFF

[!IF "node:value(./MclGeneral/MclDma/MclEnableDma)"!][!//
/*-----------------------------------------------/
/  DMA HARDWARE VERSION                          /
/-----------------------------------------------*/
#define DMA_IP_HWV2_IS_AVAILABLE                         STD_OFF
#define DMA_IP_HARDWARE_VERSION_2                        ((uint8)2U)
#define DMA_IP_HWV3_IS_AVAILABLE                         STD_ON
#define DMA_IP_HARDWARE_VERSION_3                        ((uint8)3U)

/*-----------------------------------------------/
/  DMA HARDWARE INSTANCES                        /
/-----------------------------------------------*/
 #define DMA_IP_HW_INST_0                        ((uint8)(0U))
 
/*-----------------------------------------------/
/  DMA HARDWARE CHANNELS                         /
/-----------------------------------------------*/
#define DMA_IP_HW_CH_0                          ((uint8)(0U))
#define DMA_IP_HW_CH_1                          ((uint8)(1U))
#define DMA_IP_HW_CH_2                          ((uint8)(2U))
#define DMA_IP_HW_CH_3                          ((uint8)(3U))
#define DMA_IP_HW_CH_4                          ((uint8)(4U))
#define DMA_IP_HW_CH_5                          ((uint8)(5U))
#define DMA_IP_HW_CH_6                          ((uint8)(6U))
#define DMA_IP_HW_CH_7                          ((uint8)(7U))
#define DMA_IP_HW_CH_8                          ((uint8)(8U))
#define DMA_IP_HW_CH_9                          ((uint8)(9U))
#define DMA_IP_HW_CH_10                         ((uint8)(10U))
#define DMA_IP_HW_CH_11                         ((uint8)(11U))
#define DMA_IP_HW_CH_12                         ((uint8)(12U))
#define DMA_IP_HW_CH_13                         ((uint8)(13U))
#define DMA_IP_HW_CH_14                         ((uint8)(14U))
#define DMA_IP_HW_CH_15                         ((uint8)(15U))
#define DMA_IP_HW_CH_16                         ((uint8)(16U))
#define DMA_IP_HW_CH_17                         ((uint8)(17U))
#define DMA_IP_HW_CH_18                         ((uint8)(18U))
#define DMA_IP_HW_CH_19                         ((uint8)(19U))
#define DMA_IP_HW_CH_20                         ((uint8)(20U))
#define DMA_IP_HW_CH_21                         ((uint8)(21U))
#define DMA_IP_HW_CH_22                         ((uint8)(22U))
#define DMA_IP_HW_CH_23                         ((uint8)(23U))
#define DMA_IP_HW_CH_24                         ((uint8)(24U))
#define DMA_IP_HW_CH_25                         ((uint8)(25U))
#define DMA_IP_HW_CH_26                         ((uint8)(26U))
#define DMA_IP_HW_CH_27                         ((uint8)(27U))
#define DMA_IP_HW_CH_28                         ((uint8)(28U))
#define DMA_IP_HW_CH_29                         ((uint8)(29U))
#define DMA_IP_HW_CH_30                         ((uint8)(30U))
#define DMA_IP_HW_CH_31                         ((uint8)(31U))

/*-----------------------------------------------/
/  DMA REQUESTS                                  /
/-----------------------------------------------*/

 #define DMA_IP_REQ_MUX0_DISABLED                ((uint8)0U)
#define DMA_IP_REQ_MUX0_SIUL2_0                 ((uint8)1U)
#define DMA_IP_REQ_MUX0_SIUL2_1                 ((uint8)2U)
#define DMA_IP_REQ_MUX0_SIUL2_2                 ((uint8)3U)
#define DMA_IP_REQ_MUX0_SIUL2_3                 ((uint8)4U)
#define DMA_IP_REQ_MUX0_SIUL2_4                 ((uint8)5U)
#define DMA_IP_REQ_MUX0_SIUL2_5                 ((uint8)6U)
#define DMA_IP_REQ_MUX0_SIUL2_6                 ((uint8)7U)
#define DMA_IP_REQ_MUX0_SIUL2_7                 ((uint8)8U)
#define DMA_IP_REQ_MUX0_BCTU_FIFO1              ((uint8)9U)
#define DMA_IP_REQ_MUX0_BCTU0                   ((uint8)10U)
#define DMA_IP_REQ_MUX0_BCTU1                   ((uint8)11U)
#define DMA_IP_REQ_MUX0_EMIOS0_0                ((uint8)12U)
#define DMA_IP_REQ_MUX0_EMIOS0_1                ((uint8)13U)
#define DMA_IP_REQ_MUX0_EMIOS0_9                ((uint8)14U)
#define DMA_IP_REQ_MUX0_EMIOS0_10               ((uint8)15U)
#define DMA_IP_REQ_MUX0_EMIOS1_0                ((uint8)16U)
#define DMA_IP_REQ_MUX0_EMIOS1_1                ((uint8)17U)
#define DMA_IP_REQ_MUX0_EMIOS1_9                ((uint8)18U)
#define DMA_IP_REQ_MUX0_EMIOS1_10               ((uint8)19U)

[!IF "($DerivativeName != 'S32K312') and ($DerivativeName != 'S32K342')"!][!//
#define DMA_IP_REQ_MUX0_EMIOS2_0                ((uint8)20U)
#define DMA_IP_REQ_MUX0_EMIOS2_1                ((uint8)21U)
#define DMA_IP_REQ_MUX0_EMIOS2_9                ((uint8)22U)
#define DMA_IP_REQ_MUX0_EMIOS2_10               ((uint8)23U)
[!ENDIF!][!//
#define DMA_IP_REQ_MUX0_LCU0                    ((uint8)24U)
#define DMA_IP_REQ_MUX0_LCU1                    ((uint8)25U)
#define DMA_IP_REQ_MUX0_FLEXCAN0                ((uint8)29U)
#define DMA_IP_REQ_MUX0_FLEXCAN1                ((uint8)30U)
#define DMA_IP_REQ_MUX0_FLEXCAN2                ((uint8)31U)
#define DMA_IP_REQ_MUX0_FLEXCAN3                ((uint8)32U)
#define DMA_IP_REQ_MUX0_FLEXIO_SHIFTER0_TIMER0  ((uint8)33U)
#define DMA_IP_REQ_MUX0_FLEXIO_SHIFTER1_TIMER1  ((uint8)34U)
#define DMA_IP_REQ_MUX0_FLEXIO_SHIFTER2_TIMER2  ((uint8)35U)
#define DMA_IP_REQ_MUX0_FLEXIO_SHIFTER3_TIMER3  ((uint8)36U)
[!IF "($DerivativeName != 'S32K312') and ($DerivativeName != 'S32K342')"!][!//
#define DMA_IP_REQ_MUX0_LPUART0_LPUART8_TX      ((uint8)37U)
#define DMA_IP_REQ_MUX0_LPUART0_LPUART8_RX      ((uint8)38U)
#define DMA_IP_REQ_MUX0_LPUART1_LPUART9_TX      ((uint8)39U)
#define DMA_IP_REQ_MUX0_LPUART1_LPUART9_RX      ((uint8)40U)
[!ELSE!][!//
#define DMA_IP_REQ_MUX0_LPUART0_TX              ((uint8)37U)
#define DMA_IP_REQ_MUX0_LPUART0_RX              ((uint8)38U)
#define DMA_IP_REQ_MUX0_LPUART1_TX              ((uint8)39U)
#define DMA_IP_REQ_MUX0_LPUART1_RX              ((uint8)40U)
[!ENDIF!][!//
#define DMA_IP_REQ_MUX0_LPI2C0_RX_SLAVE_RX      ((uint8)41U)
#define DMA_IP_REQ_MUX0_LPI2C0_TX_SLAVE_TX      ((uint8)42U)
#define DMA_IP_REQ_MUX0_LPSPI0_TX               ((uint8)43U)
#define DMA_IP_REQ_MUX0_LPSPI0_RX               ((uint8)44U)
#define DMA_IP_REQ_MUX0_LPSPI1_TX               ((uint8)45U)
#define DMA_IP_REQ_MUX0_LPSPI1_RX               ((uint8)46U)
#define DMA_IP_REQ_MUX0_LPSPI2_TX               ((uint8)47U)
#define DMA_IP_REQ_MUX0_LPSPI2_RX               ((uint8)48U)
#define DMA_IP_REQ_MUX0_LPSPI3_TX               ((uint8)49U)
#define DMA_IP_REQ_MUX0_LPSPI3_RX               ((uint8)50U)
[!IF "$DerivativeName = 'S32K312'"!][!//
#define DMA_IP_REQ_MUX0_I3C0_RX                 ((uint8)51U)
#define DMA_IP_REQ_MUX0_I3C0_TX                 ((uint8)52U)
[!ELSEIF "$DerivativeName = 'S32K342'"!][!//
#define DMA_IP_REQ_MUX0_I3C0_RX                 ((uint8)51U)
#define DMA_IP_REQ_MUX0_I3C0_TX                 ((uint8)52U)
#define DMA_IP_REQ_MUX0_QSPI_RX                 ((uint8)53U)
#define DMA_IP_REQ_MUX0_QSPI_TX                 ((uint8)54U)
#define DMA_IP_REQ_MUX0_SAI0_RX                 ((uint8)55U)
#define DMA_IP_REQ_MUX0_SAI0_TX                 ((uint8)56U)
[!ELSE!][!//
#define DMA_IP_REQ_MUX0_QSPI_RX                 ((uint8)53U)
#define DMA_IP_REQ_MUX0_QSPI_TX                 ((uint8)54U)
#define DMA_IP_REQ_MUX0_SAI0_RX                 ((uint8)55U)
#define DMA_IP_REQ_MUX0_SAI0_TX                 ((uint8)56U)
[!ENDIF!][!//
#define DMA_IP_REQ_MUX0_ADC0                    ((uint8)58U)
#define DMA_IP_REQ_MUX0_ADC1                    ((uint8)59U)
[!IF "$DerivativeName != 'S32K312'"!][!//
#define DMA_IP_REQ_MUX0_ADC2                    ((uint8)60U)
[!ENDIF!][!//
#define DMA_IP_REQ_MUX0_CMP0                    ((uint8)61U)
#define DMA_IP_REQ_MUX0_ALWAYS_ON0              ((uint8)62U)
#define DMA_IP_REQ_MUX0_ALWAYS_ON1              ((uint8)63U)
#define DMA_IP_REQ_MUX1_DISABLED                ((uint8)64U)
#define DMA_IP_REQ_MUX1_SIUL28                  ((uint8)65U)
#define DMA_IP_REQ_MUX1_SIUL29                  ((uint8)66U)
#define DMA_IP_REQ_MUX1_SIUL210                 ((uint8)67U)
#define DMA_IP_REQ_MUX1_SIUL211                 ((uint8)68U)
#define DMA_IP_REQ_MUX1_SIUL212                 ((uint8)69U)
#define DMA_IP_REQ_MUX1_SIUL213                 ((uint8)70U)
#define DMA_IP_REQ_MUX1_SIUL214                 ((uint8)71U)
#define DMA_IP_REQ_MUX1_SIUL215                 ((uint8)72U)
#define DMA_IP_REQ_MUX1_BCTU_FIFO2              ((uint8)73U)
[!IF "($DerivativeName != 'S32K312') and ($DerivativeName != 'S32K342')"!][!//
#define DMA_IP_REQ_MUX1_BCTU2                   ((uint8)74U)
[!ENDIF!][!//
#define DMA_IP_REQ_MUX1_EMIOS0_16               ((uint8)75U)
#define DMA_IP_REQ_MUX1_EMIOS0_17               ((uint8)76U)
#define DMA_IP_REQ_MUX1_EMIOS0_18               ((uint8)77U)
#define DMA_IP_REQ_MUX1_EMIOS0_19               ((uint8)78U)
#define DMA_IP_REQ_MUX1_EMIOS1_16               ((uint8)79U)
#define DMA_IP_REQ_MUX1_EMIOS1_17               ((uint8)80U)
#define DMA_IP_REQ_MUX1_EMIOS1_18               ((uint8)81U)
#define DMA_IP_REQ_MUX1_EMIOS1_19               ((uint8)82U)
[!IF "($DerivativeName != 'S32K312') and ($DerivativeName != 'S32K342')"!][!//
#define DMA_IP_REQ_MUX1_EMIOS2_16               ((uint8)83U)
#define DMA_IP_REQ_MUX1_EMIOS2_17               ((uint8)84U)
#define DMA_IP_REQ_MUX1_EMIOS2_18               ((uint8)85U)
#define DMA_IP_REQ_MUX1_EMIOS2_19               ((uint8)86U)
[!ENDIF!][!//
#define DMA_IP_REQ_MUX1_LCU0_1                  ((uint8)87U)
#define DMA_IP_REQ_MUX1_LCU0_2                  ((uint8)88U)
#define DMA_IP_REQ_MUX1_LCU1_1                  ((uint8)89U)
#define DMA_IP_REQ_MUX1_LCU1_2                  ((uint8)90U)
[!IF "$DerivativeName != 'S32K312'"!][!//
#define DMA_IP_REQ_MUX1_ENET_TIMER_CH0_1_2_3    ((uint8)91U)
[!ENDIF!][!//
[!IF "$DerivativeName != 'S32K342'"!][!//
#define DMA_IP_REQ_MUX1_FLEXCAN4                ((uint8)94U)
#define DMA_IP_REQ_MUX1_FLEXCAN5                ((uint8)95U)
[!ENDIF!][!//
#define DMA_IP_REQ_MUX1_FLEXIO_SHIFTER4_TIMER4  ((uint8)98U)
#define DMA_IP_REQ_MUX1_FLEXIO_SHIFTER5_TIMER5  ((uint8)99U)
#define DMA_IP_REQ_MUX1_FLEXIO_SHIFTER6_TIMER6  ((uint8)100U)
#define DMA_IP_REQ_MUX1_FLEXIO_SHIFTER7_TIMER7  ((uint8)101U)
[!IF "$DerivativeName = 'S32K312'"!][!//
#define DMA_IP_REQ_MUX1_LPUART2_TX              ((uint8)102U)
#define DMA_IP_REQ_MUX1_LPUART2_RX              ((uint8)103U)
#define DMA_IP_REQ_MUX1_LPUART3_TX              ((uint8)104U)
#define DMA_IP_REQ_MUX1_LPUART3_RX              ((uint8)105U)
#define DMA_IP_REQ_MUX1_LPUART4_TX              ((uint8)106U)
#define DMA_IP_REQ_MUX1_LPUART4_RX              ((uint8)107U)
#define DMA_IP_REQ_MUX1_LPUART5_TX              ((uint8)108U)
#define DMA_IP_REQ_MUX1_LPUART5_RX              ((uint8)109U)
#define DMA_IP_REQ_MUX1_LPUART6_TX              ((uint8)110U)
#define DMA_IP_REQ_MUX1_LPUART6_RX              ((uint8)111U)
#define DMA_IP_REQ_MUX1_LPUART7_TX              ((uint8)112U)
#define DMA_IP_REQ_MUX1_LPUART7_RX              ((uint8)113U)
[!ELSEIF "$DerivativeName = 'S32K342'"!][!//
#define DMA_IP_REQ_MUX1_LPUART2_TX              ((uint8)102U)
#define DMA_IP_REQ_MUX1_LPUART2_RX              ((uint8)103U)
#define DMA_IP_REQ_MUX1_LPUART3_TX              ((uint8)104U)
#define DMA_IP_REQ_MUX1_LPUART3_RX              ((uint8)105U)
[!ELSE!][!//
#define DMA_IP_REQ_MUX1_LPUART2_LPUART10_TX     ((uint8)102U)
#define DMA_IP_REQ_MUX1_LPUART2_LPUART10_RX     ((uint8)103U)
#define DMA_IP_REQ_MUX1_LPUART3_LPUART11_TX     ((uint8)104U)
#define DMA_IP_REQ_MUX1_LPUART3_LPUART11_RX     ((uint8)105U)
#define DMA_IP_REQ_MUX1_LPUART4_LPUART12_TX     ((uint8)106U)
#define DMA_IP_REQ_MUX1_LPUART4_LPUART12_RX     ((uint8)107U)
#define DMA_IP_REQ_MUX1_LPUART5_LPUART13_TX     ((uint8)108U)
#define DMA_IP_REQ_MUX1_LPUART5_LPUART13_RX     ((uint8)109U)
#define DMA_IP_REQ_MUX1_LPUART6_LPUART14_TX     ((uint8)110U)
#define DMA_IP_REQ_MUX1_LPUART6_LPUART14_RX     ((uint8)111U)
#define DMA_IP_REQ_MUX1_LPUART7_LPUART15_TX     ((uint8)112U)
#define DMA_IP_REQ_MUX1_LPUART7_LPUART15_RX     ((uint8)113U)
[!ENDIF!][!//
#define DMA_IP_REQ_MUX1_LPI2C1_RX_SLAVE_RX      ((uint8)114U)
#define DMA_IP_REQ_MUX1_LPI2C1_TX_SLAVE_TX      ((uint8)115U)
[!IF "$DerivativeName != 'S32K312'"!][!//
[!IF "$DerivativeName != 'S32K342'"!][!//
#define DMA_IP_REQ_MUX1_LPSPI4_TX               ((uint8)116U)
#define DMA_IP_REQ_MUX1_LPSPI4_RX               ((uint8)117U)
#define DMA_IP_REQ_MUX1_LPSPI5_TX               ((uint8)118U)
#define DMA_IP_REQ_MUX1_LPSPI5_RX               ((uint8)119U)
[!ENDIF!][!//
#define DMA_IP_REQ_MUX1_SAI1_RX                 ((uint8)120U)
#define DMA_IP_REQ_MUX1_SAI1_TX                 ((uint8)121U)
[!ENDIF!][!//
#define DMA_IP_REQ_MUX1_CMP1                    ((uint8)124U)
[!IF "($DerivativeName != 'S32K312') and ($DerivativeName != 'S32K342')"!][!//
#define DMA_IP_REQ_MUX1_CMP2                    ((uint8)125U)
[!ENDIF!][!//
#define DMA_IP_REQ_MUX1_ALWAYS_ON0              ((uint8)126U)
#define DMA_IP_REQ_MUX1_ALWAYS_ON1              ((uint8)127U)
 
/*-----------------------------------------------/
/  DMA GROUP PRIORITY                            /
/-----------------------------------------------*/
#define DMA_IP_GROUP_PRIO0                      ((uint8)(0U))
#define DMA_IP_GROUP_PRIO1                      ((uint8)(1U))
#define DMA_IP_GROUP_PRIO2                      ((uint8)(2U))
#define DMA_IP_GROUP_PRIO3                      ((uint8)(3U))
#define DMA_IP_GROUP_PRIO4                      ((uint8)(4U))
#define DMA_IP_GROUP_PRIO5                      ((uint8)(5U))
#define DMA_IP_GROUP_PRIO6                      ((uint8)(6U))
#define DMA_IP_GROUP_PRIO7                      ((uint8)(7U))
#define DMA_IP_GROUP_PRIO8                      ((uint8)(8U))
#define DMA_IP_GROUP_PRIO9                      ((uint8)(9U))
#define DMA_IP_GROUP_PRIO10                     ((uint8)(10U))
#define DMA_IP_GROUP_PRIO11                     ((uint8)(11U))
#define DMA_IP_GROUP_PRIO12                     ((uint8)(12U))
#define DMA_IP_GROUP_PRIO13                     ((uint8)(13U))
#define DMA_IP_GROUP_PRIO14                     ((uint8)(14U))
#define DMA_IP_GROUP_PRIO15                     ((uint8)(15U))
#define DMA_IP_GROUP_PRIO16                     ((uint8)(16U))
#define DMA_IP_GROUP_PRIO17                     ((uint8)(17U))
#define DMA_IP_GROUP_PRIO18                     ((uint8)(18U))
#define DMA_IP_GROUP_PRIO19                     ((uint8)(19U))
#define DMA_IP_GROUP_PRIO20                     ((uint8)(20U))
#define DMA_IP_GROUP_PRIO21                     ((uint8)(21U))
#define DMA_IP_GROUP_PRIO22                     ((uint8)(22U))
#define DMA_IP_GROUP_PRIO23                     ((uint8)(23U))
#define DMA_IP_GROUP_PRIO24                     ((uint8)(24U))
#define DMA_IP_GROUP_PRIO25                     ((uint8)(25U))
#define DMA_IP_GROUP_PRIO26                     ((uint8)(26U))
#define DMA_IP_GROUP_PRIO27                     ((uint8)(27U))
#define DMA_IP_GROUP_PRIO28                     ((uint8)(28U))
#define DMA_IP_GROUP_PRIO29                     ((uint8)(29U))
#define DMA_IP_GROUP_PRIO30                     ((uint8)(30U))
#define DMA_IP_GROUP_PRIO31                     ((uint8)(31U))

/*-----------------------------------------------/
/  DMA CHANNEL PRIORITY                          /
/-----------------------------------------------*/
#define DMA_IP_LEVEL_PRIO0                      ((uint8)(0U))
#define DMA_IP_LEVEL_PRIO1                      ((uint8)(1U))
#define DMA_IP_LEVEL_PRIO2                      ((uint8)(2U))
#define DMA_IP_LEVEL_PRIO3                      ((uint8)(3U))
#define DMA_IP_LEVEL_PRIO4                      ((uint8)(4U))
#define DMA_IP_LEVEL_PRIO5                      ((uint8)(5U))
#define DMA_IP_LEVEL_PRIO6                      ((uint8)(6U))
#define DMA_IP_LEVEL_PRIO7                      ((uint8)(7U))

/*-----------------------------------------------/
/  DMA TRANSFER SIZE                             /
/-----------------------------------------------*/
#define DMA_IP_TRANSFER_SIZE_1_BYTE             ((uint8)(0U))
#define DMA_IP_TRANSFER_SIZE_2_BYTE             ((uint8)(1U))
#define DMA_IP_TRANSFER_SIZE_4_BYTE             ((uint8)(2U))
#define DMA_IP_TRANSFER_SIZE_8_BYTE             ((uint8)(3U))
#define DMA_IP_TRANSFER_SIZE_16_BYTE            ((uint8)(4U))
#define DMA_IP_TRANSFER_SIZE_32_BYTE            ((uint8)(5U))
#define DMA_IP_TRANSFER_SIZE_64_BYTE            ((uint8)(6U))

/*-----------------------------------------------/
/  DMA BANDWIDTH CONTROL                         /
/-----------------------------------------------*/
#define DMA_IP_BWC_ENGINE_NO_STALL              ((uint8)(0U))
#define DMA_IP_BWC_ENGINE_HPE                     ((uint8)(1U))
#define DMA_IP_BWC_ENGINE_4CYCLE_STALL        ((uint8)(2U))
#define DMA_IP_BWC_ENGINE_8CYCLE_STALL        ((uint8)(3U))

/*-----------------------------------------------/
/  DMA CONVERT DCHPRI                            /
/-----------------------------------------------*/
#define DMA_CHN_TO_DCHPRI_INDEX(x)               (x)

[!ENDIF!][!//

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef DMA_IP_CFG_DEFINES_H_ */

/*==================================================================================================
 *                                        END OF FILE
==================================================================================================*/
